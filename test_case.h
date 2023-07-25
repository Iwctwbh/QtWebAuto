#pragma once
#ifndef TEST_CASE_H
#define TEST_CASE_H

#include <QRegularExpression>
#include <QWebEngineView>
#include "test_case_step.h"

class TestCase
{
public:
	enum LogType
	{
		kLog,
		kUrl,
		kJavascript
	};

	enum ShowWebViewTime
	{
		kNo,
		kStart,
		kCompleted
	};

	static void SetUrl(const QUrl&);
	static void SetLogLevel(const QString&);
	static void SetShowWebViewTime(const QString&);
	static void SetRunningInterval(const int&);
	static void InsertRunBeforeStep(const QString&);
	static void InsertCaseStep(const QUrl&, const TestCaseStep&);
	static void SetStopStep(const QString&);

	static const QUrl& GetUrl(void);
	static bool CheckShowWebView(const ShowWebViewTime&);
	static const int& GetRunningInterval(void);
	static const QList<QString>& GetRunBeforeStep(void);
	static const QString& GetStopStep(void);
	static bool CheckStopStep(const QString&);
	static TestCaseStep GetCaseStep(const QUrl&);

	static void Log(const QString&, const LogType&);
	static void CheckAndRun(const QWebEngineView&, const QString&);

private:
	enum LogLevel
	{
		kNull,
		kInfo,
		kDebug
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

	inline static const QHash<LogType, int> has_mapping_log_type_
	{
		{ kLog, 0 },
		{ kUrl, 0 },
		{ kJavascript, -1 }
	};

	inline static const QHash<LogLevel, int> has_mapping_log_level_
	{
		{ kNull, 0 },
		{ kInfo, 1 },
		{ kDebug, 2 }
	};

	inline static const QHash<QString, ShowWebViewTime> has_show_web_view_time_
	{
		{ "start", kStart },
		{ "Start", kStart },
		{ "End", kCompleted },
		{ "end", kCompleted },
		{ "completed", kCompleted },
		{ "Completed", kCompleted }
	};

	inline static QUrl url_{};
	inline static LogLevel log_level_{};
	inline static ShowWebViewTime show_web_view_time_{};
	inline static int running_interval_{};
	inline static QList<QString> list_run_before_step_{};
	inline static QHash<QUrl, TestCaseStep> case_steps_{};
	inline static QString stop_step_{};
	inline static QRegularExpression regex_url_{R"(https?:\/\/[-\w\.\/]+)"};
	inline static QRegularExpression regex_wait_{R"(^@{WAIT *?, *?\d+( *?, *?((.*?)|("?.*?)\"))?}@$)"};
	inline static QRegularExpression regex_round_{R"(https?:\/\/[-\w\.\/]+)"};
};
#endif