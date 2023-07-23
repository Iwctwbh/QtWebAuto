#include <QApplication>
#include <QWebEngineView>
#include <QTimer>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTest>
#include "test_case.h"

void check_true_exit(bool);
void check_false_exit(bool);

int main(int argc, char* argv[])
{
#pragma region 获取文件列表
	const QDir dir{ "." };
	QStringList list_files{ dir.entryList(QStringList{ "*.json" }, QDir::Files) };
	check_true_exit(list_files.length() < 1);
	QString file_case_name{ list_files.value(0) };

	if (int count = 0; list_files.length() > 1)
	{
		std::ranges::for_each(list_files, [&count](const QString& s)
			{
				qDebug().noquote() << count++ << ":" << s;
			});
#pragma region 选择Case，获取输入
		qDebug().noquote() << "Enter Number To Continue:";
		QTextStream stream{ stdin };
		const int id_case = stream.readLine().toInt();
		file_case_name = list_files.value(id_case);
#pragma endregion
	}
#pragma endregion



#pragma region 读取Case的Json文件
	QFile file_case_json{ file_case_name };
	check_false_exit(file_case_json.open(QIODevice::ReadOnly));
	qDebug().noquote() << "Load " + file_case_name;

	QJsonParseError json_error{ QJsonParseError::NoError };
	const QJsonDocument json_document{ QJsonDocument::fromJson(file_case_json.readAll(),&json_error) };
	check_true_exit(json_error.error != QJsonParseError::NoError || !json_document.isObject());

	const QJsonObject json_object{ json_document.object() };

	TestCase::SetUrl(json_object.value("Url").toString());
	TestCase::SetRunningInterval(json_object.value("Running-Interval").toInt());
	TestCase::SetStopStep(json_object.value("StopStep").toString());

	const QJsonObject json_object_steps{ json_object.value("Steps").toObject() };

	for (QJsonObject::const_iterator iterator = json_object_steps.constBegin(); iterator != json_object_steps.constEnd(); ++iterator)
	{
		TestCaseStep case_step{};
		QJsonObject json_object_step{ iterator.value().toObject() };
		qDebug().noquote() << iterator.key();
		case_step.SetUrl(QUrl{ iterator.key() });
		case_step.SetIsAutomatic(json_object_step.value("IsAutomatic").toBool());
		QJsonArray json_array_automatic{ json_object_step.value("Automatic").toArray() };
		QJsonArray json_array_non_automatic{ json_object_step.value("Non-Automatic").toArray() };
		for (QJsonArray::const_iterator it = json_array_automatic.constBegin(); it != json_array_automatic.constEnd(); ++it)
		{
			case_step.InsertAutomatic(it->toString());
		}

		for (QJsonArray::const_iterator it = json_array_non_automatic.constBegin(); it != json_array_non_automatic.constEnd(); ++it)
		{
			case_step.InsertNonAutomatic(it->toString());
		}
		TestCase::InsertCaseStep(iterator.key(), case_step);
	}
#pragma endregion
	QApplication app(argc, argv);
	QWebEngineView web_view;
	web_view.load(TestCase::GetUrl());
	web_view.resize(1920, 1080);
	web_view.show();

	bool is_completed{ false };

	QObject::connect(&web_view, &QWebEngineView::loadFinished, [&web_view, &is_completed]() -> void
		{
			qDebug().noquote() << web_view.url().toString() << "Page Load Completed";

			// completed at stop step
			if (TestCase::CheckStopStep(web_view.url().toString()))
			{
				is_completed = true;
			}

			// automatic run before stop step or is automatic is true
			if (!is_completed || TestCase::GetCaseStep(web_view.url()).CheckIsAutomatic())
			{
				std::ranges::for_each(TestCase::GetCaseStep(web_view.url()).GetAutomatic(), [&web_view](const QString& s)
					{
						qDebug().noquote() << "javascript:" << s;
						web_view.page()->runJavaScript(s);
						QTest::qWait(TestCase::GetRunningInterval());
					});
			}

			// non-automatic only run before stop step
			if (!is_completed)
			{
				std::ranges::for_each(TestCase::GetCaseStep(web_view.url()).GetNonAutomatic(), [&web_view](const QString& s)
					{
						qDebug().noquote() << "javascript:" << s;
						web_view.page()->runJavaScript(s);
						QTest::qWait(TestCase::GetRunningInterval());
					});
			}
		});

	/*QTimer timer{};
	timer.setSingleShot(true);
	QObject::connect(&timer, &QTimer::timeout, [&view]()
		{
			view.show();
		});
	timer.start(5000);*/

	return QApplication::exec();
}

void check_true_exit(bool flag)
{
	if (flag)
	{
		exit(0);
	}
}

void check_false_exit(bool flag)
{
	if (!flag)
	{
		exit(0);
	}
}