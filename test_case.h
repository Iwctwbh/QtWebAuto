#pragma once
#ifndef TEST_CASE_H
#define TEST_CASE_H

#include <QRegularExpression>
#include <QWebEngineView>
#include <QMessageBox>
#include <QTimer>
#include <QEventLoop>
#include <QMouseEvent>
#include <QApplication>
#include <QInputMethodEvent>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QGraphicsSceneMouseEvent>
#include <QQuickWidget>

#include "test_case_step.h"

class TestCase
{
public:
	enum class LogType : int
	{
		kLog = 1,
		kUrl = 1,
		kJavascript = 2,
		kEvent = 2
	};

	enum class ShowWebViewTime : int
	{
		kNo,
		kStart,
		kCompleted
	};
	inline static const QRegularExpression regex_send_text_{R"(^@{ *?SEND_TEXT *?,.+?}@$)"};

	static void SetUrl(const QUrl&);
	static void SetLogLevel(const QString&);
	static void SetShowWebViewTime(const QString&);
	static void SetRunningInterval(const int&);
	static void InsertRunBeforeStep(const QString&);
	static void InsertCaseStep(const QUrl&, const TestCaseStep&);
	static void SetStopStep(const QString&);
	static void SetCountStopStep(const int&);
	static void SetIsRecord(const bool&);

	static const QUrl& GetUrl(void);
	static bool CheckShowWebView(const ShowWebViewTime&);
	static const int& GetRunningInterval(void);
	static const QList<QString>& GetRunBeforeStep(void);
	static const QString& GetStopStep(void);
	static bool CheckStopStep(const QString&);
	static TestCaseStep GetCaseStep(const QUrl&);
	static const bool& CheckIsWait(void);
	static const bool& CheckIsRecord(void);
	static const bool& CheckIsEvent(void);
	static QWidget* GetWebViewWidget(void);

	static void Log(const QString&, const LogType&);
	static void CheckAndRun(const QWebEngineView&, const QString&);
	static void SaveRecord(void);

	inline static QUrl current_url_{};
	inline static QList<QString> current_url_event_{};
	inline static bool is_record_{};
	inline static QJsonDocument json_document_{};
	inline static QString file_name_{};
	inline static QWebEngineView* web_view_{};
	inline static QWidget* web_view_widget_{};
	inline static QWidget* web_view_default_widget_{};

private:
	enum LogLevel
	{
		kNull = 0,
		kInfo = 1,
		kDebug = 2
	};

	inline static const QHash<QString, LogLevel> hash_log_level_
	{
		{ "Null", kNull },
		{ "null", kNull },
		{ "Info", kInfo },
		{ "Information", kInfo },
		{ "Debug", kDebug },
		{ "Debugger", kDebug }
	};

	inline static const QHash<QString, ShowWebViewTime> has_show_web_view_time_
	{
		{ "start", ShowWebViewTime::kStart },
		{ "Start", ShowWebViewTime::kStart },
		{ "End", ShowWebViewTime::kCompleted },
		{ "end", ShowWebViewTime::kCompleted },
		{ "completed", ShowWebViewTime::kCompleted },
		{ "Completed", ShowWebViewTime::kCompleted }
	};

	inline static QUrl url_{};
	inline static LogLevel log_level_{};
	inline static ShowWebViewTime show_web_view_time_{};
	inline static int running_interval_{};
	inline static QList<QString> list_run_before_step_{};
	inline static QHash<QUrl, TestCaseStep> case_steps_{};
	inline static QString stop_step_{};
	inline static int count_stop_step_{};
	inline static const QRegularExpression regex_url_{R"(https?:\/\/[-\w\.\/]+)"};
	inline static const QRegularExpression regex_wait_{R"(^@{ *?WAIT *?, *?\d+( *?, *?((.*?)|("?.*?)\"))?}@$)"};
	inline static const QRegularExpression regex_coordinate_{R"(^@{ *?COORDINATE *?, *?\d+ *?, *?\d+ *?}@$)"};
	inline static const QRegularExpression regex_key_{R"(^@{ *?KEY *?, *?\d+ *?}@$)"};
	inline static const QRegularExpression regex_scroll_{R"(^@{ *?SCROLL *?(, *?-?\d+ *?){8}}@$)"};
	inline static bool is_wait_{};
	inline static bool is_event_{};
};
#endif