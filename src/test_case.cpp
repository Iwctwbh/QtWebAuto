#include "test_case.h"

#include <QJsonArray>

template<typename A, typename B,
	std::enable_if_t<std::is_integral_v<A> || std::is_enum_v<A>>* = nullptr,
	std::enable_if_t<std::is_enum_v<B> || std::is_integral_v<B>>* = nullptr>
bool operator==(A a, B b)
{
	return a == static_cast<A>(b);
}

template<typename A, typename B,
	std::enable_if_t<std::is_integral_v<A> || std::is_enum_v<A>>* = nullptr,
	std::enable_if_t<std::is_enum_v<B> || std::is_integral_v<B>>* = nullptr>
bool operator>=(A a, B b)
{
	return a >= static_cast<A>(b);
}

void TestCase::SetUrl(const QUrl& arg_url)
{
	url_ = arg_url;
}

void TestCase::SetLogLevel(const QString& arg_log_level)
{
	log_level_ = hash_log_level_.value(arg_log_level);
}

void TestCase::SetShowWebViewTime(const QString& arg_show_web_view_time)
{
	show_web_view_time_ = has_show_web_view_time_.value(arg_show_web_view_time);
}

void TestCase::SetRunningInterval(const int& arg_interval)
{
	running_interval_ = arg_interval;
}

void TestCase::SetCursorFollow(const bool& arg_is_cursor_follow)
{
	is_cursor_follow_ = arg_is_cursor_follow;
}

void TestCase::InsertRunBeforeStep(const QString& arg_run_before_step)
{
	list_run_before_step_.push_back(arg_run_before_step);
}

void TestCase::InsertCaseStep(const QUrl& arg_url, const TestCaseStep& arg_case_step)
{
	case_steps_.insert(arg_url, arg_case_step);
}

void TestCase::SetStopStep(const QString& arg_stop_step)
{
	stop_step_ = arg_stop_step;
}

void TestCase::SetCountStopStep(const int& arg_count_stop_step)
{
	count_stop_step_ = arg_count_stop_step;
}

const QUrl& TestCase::GetUrl(void)
{
	return url_;
}

bool TestCase::CheckShowWebView(const ShowWebViewTime& arg_show_web_view_time)
{
	return operator==(show_web_view_time_, arg_show_web_view_time);
}

const int& TestCase::GetRunningInterval(void)
{
	return running_interval_;
}

const QList<QString>& TestCase::GetRunBeforeStep(void)
{
	return list_run_before_step_;
}

const QString& TestCase::GetStopStep(void)
{
	return stop_step_;
}

bool TestCase::CheckStopStep(const QString& arg_url)
{
	if (const QRegularExpressionMatch match{ regex_url_.match(arg_url) }; match.hasMatch())
	{
		if (QString::compare(stop_step_, match.captured()) == 0)
		{
			if (--count_stop_step_ == 0)
			{
				return true;
			}
		}
	}
	return false;
}

TestCaseStep TestCase::GetCaseStep(const QUrl& arg_url)
{
	if (const QRegularExpressionMatch match{ regex_url_.match(arg_url.toString()) }; match.hasMatch())
	{
		return case_steps_.value(match.captured());
	}
	return {};
}

const bool& TestCase::CheckIsWait(void)
{
	return is_wait_;
}

const bool& TestCase::CheckIsEvent(void)
{
	return is_event_;
}

QWidget* TestCase::GetWebViewWidget()
{
	foreach(QObject * object, TestCase::web_view_->children())
	{
		if (QQuickWidget * widget{ qobject_cast<QQuickWidget*>(object) })
		{
			return widget;
		}
	}
	return nullptr;
}

void TestCase::Log(const QString& arg_log, const LogType& arg_log_type)
{
	if (arg_log_type == LogType::kEvent && is_record_ == false)
	{
		return;
	}
	if (operator>=(log_level_, arg_log_type))
	{
		qDebug().noquote() << arg_log;
	}
}

void TestCase::CheckAndRun(const QWebEngineView& arg_view, const QString& arg_command)
{
	// @{WAIT,0,"Press any key to continue:"}@
	if (QRegularExpressionMatch match{ regex_wait_.match(arg_command.trimmed()) }; match.hasMatch())
	{
		QList<QString> list_command{ match.captured().trimmed().remove(0, 2).remove(QRegularExpression("}@$")).split(',') };

		// Remove WAIT
		list_command.removeFirst();

		// Take Time
		const int time{ list_command.takeFirst().toInt() };

		QMessageBox* msg_box{ new QMessageBox };
		msg_box->setAttribute(Qt::WA_DeleteOnClose);

		// Take Log
		if (const QString log{ list_command.join(',') }; !log.isEmpty())
		{
			//QMessageBox* msg_box{ new QMessageBox };
			msg_box->setWindowTitle("WAIT");
			msg_box->setText(log);
			msg_box->open();
		}

		QEventLoop event_loop{};

		if (time > 0)
		{
			QTimer::singleShot(time, msg_box, &QMessageBox::close);
			QTimer::singleShot(time, &event_loop, &QEventLoop::quit);
		}

		QObject::connect(msg_box, &QMessageBox::destroyed, &event_loop, &QEventLoop::quit);

		is_wait_ = true;
		event_loop.exec();
		is_wait_ = false;
		//msg_box->deleteLater();
	}
	else if (match = regex_coordinate_.match(arg_command.trimmed()); match.hasMatch())
	{
		QList<QString> list_command{ match.captured().trimmed().remove(0, 2).remove(QRegularExpression("}@$")).split(',') };

		// Remove COORDINATE
		list_command.removeFirst();

		// Take X
		const int x{ list_command.takeFirst().toInt() };

		// Take Y
		const int y{ list_command.takeLast().toInt() };

		if (is_cursor_follow_)
		{
			QCursor::setPos(x, y);
		}
		// Sent Event
		is_event_ = true;
		QMouseEvent event_press{ QMouseEvent::Type::MouseButtonPress, QPoint {x, y}, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier };
		QMouseEvent event_release{ QMouseEvent::Type::MouseButtonRelease, QPoint {x, y}, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier };

		QApplication::sendEvent(web_view_widget_, &event_press);
		QApplication::sendEvent(web_view_widget_, &event_release);
		is_event_ = false;
	}
	else if (match = regex_send_text_.match(arg_command.trimmed()); match.hasMatch())
	{
		QList<QString> list_command{ match.captured().trimmed().remove(0, 2).remove(QRegularExpression("}@$")).split(',') };

		// Remove SEN_TEXT
		list_command.removeFirst();
		foreach(QString s, QString{ list_command.join(',').trimmed().remove(QRegularExpression(R"(^")")).remove(QRegularExpression(R"("$)")) })
		{
			// Sent Event
			is_event_ = true;
			QKeyEvent event_press{ QKeyEvent::Type::KeyPress, s.at(0).unicode(), Qt::NoModifier, s };
			QKeyEvent event_release{ QKeyEvent::Type::KeyRelease, s.at(0).unicode(), Qt::NoModifier };

			QApplication::sendEvent(web_view_widget_, &event_press);
			QApplication::sendEvent(web_view_widget_, &event_release);
			is_event_ = false;
		}
	}
	else if (match = regex_key_.match(arg_command.trimmed()); match.hasMatch())
	{
		QList<QString> list_command{ match.captured().trimmed().remove(0, 2).remove(QRegularExpression("}@$")).split(',') };

		// Remove KEY
		list_command.removeFirst();

		// Take KeyCode
		int key = list_command.takeFirst().trimmed().toInt();

		// Sent Event
		is_event_ = true;
		QKeyEvent event_press{ QKeyEvent::Type::KeyPress, key, Qt::NoModifier };
		QKeyEvent event_release{ QKeyEvent::Type::KeyPress, key, Qt::NoModifier };
		QApplication::sendEvent(web_view_widget_, &event_press);
		is_event_ = false;
	}
	else if (match = regex_scroll_.match(arg_command.trimmed()); match.hasMatch())
	{
		QList<QString> list_command{ match.captured().trimmed().remove(0, 2).remove(QRegularExpression("}@$")).split(',') };

		// Remove SCROLL
		list_command.removeFirst();

		// Take KeyCode
		int x{ list_command.takeFirst().trimmed().toInt() };
		int y{ list_command.takeFirst().trimmed().toInt() };
		int global_x{ list_command.takeFirst().trimmed().toInt() };
		int global_y{ list_command.takeFirst().trimmed().toInt() };
		int pixel_delta_x{ list_command.takeFirst().trimmed().toInt() };
		int pixel_delta_y{ list_command.takeFirst().trimmed().toInt() };
		int angle_delta_x{ list_command.takeFirst().trimmed().toInt() };
		int angle_delta_y{ list_command.takeFirst().trimmed().toInt() };

		// Sent Event
		is_event_ = true;
		QWheelEvent event_wheel{ QPoint{x,y},QPoint{global_x,global_y},QPoint{pixel_delta_x,pixel_delta_y},QPoint{angle_delta_x,angle_delta_y},Qt::NoButton,Qt::NoModifier,Qt::NoScrollPhase,false };
		QApplication::sendEvent(web_view_widget_, &event_wheel);
		is_event_ = false;
	}
	else
	{
		arg_view.page()->runJavaScript(arg_command);
	}
}

void TestCase::SaveRecord(void)
{
	QJsonObject json_object{ json_document_.object() };
	QJsonObject json_object_steps{ json_object.value("Steps").toObject() };

	QJsonObject json_object_step{ json_object_steps.value(current_url_.toString()).toObject() };
	json_object_step.insert("Non-Automatic", QJsonArray::fromStringList(case_steps_.value(current_url_).GetNonAutomatic()));
	json_object_steps.insert(current_url_.toString(), json_object_step);
	json_object.insert("Steps", json_object_steps);
	json_document_ = QJsonDocument{ json_object };

	QFile file{ file_name_ };
	file.open(QIODevice::WriteOnly | QIODevice::Truncate);
	file.write(json_document_.toJson(QJsonDocument::Compact));
	file.close();
}