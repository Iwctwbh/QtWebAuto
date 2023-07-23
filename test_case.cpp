#include "test_case.h"

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

void TestCase::InsertCaseStep(const QUrl& arg_url, const TestCaseStep& arg_case_step)
{
	case_steps_.insert(arg_url, arg_case_step);
}

void TestCase::SetStopStep(const QString& arg_stop_step)
{
	stop_step_ = arg_stop_step;
}

const QUrl& TestCase::GetUrl(void)
{
	return url_;
}

const bool& TestCase::CheckShowWebView(const ShowWebViewTime& arg_show_web_view_time)
{
	return show_web_view_time_ == arg_show_web_view_time;
}

const int& TestCase::GetRunningInterval(void)
{
	return running_interval_;
}

const QString& TestCase::GetStopStep(void)
{
	return stop_step_;
}

bool TestCase::CheckStopStep(const QString& arg_url)
{
	const QRegularExpressionMatch match = regex_url_.match(arg_url);
	auto b = match.captured();
	return QString::compare(stop_step_, match.captured()) == 0;
}

TestCaseStep TestCase::GetCaseStep(const QUrl& arg_url)
{
	return case_steps_.value(arg_url);
}

void TestCase::Log(const QString& arg_log, const LogType& arg_log_type)
{
	if (has_mapping_log_type_.value(arg_log_type) + has_mapping_log_level_.value(log_level_) > 0)
	{
		qDebug().noquote() << arg_log;
	}
}
