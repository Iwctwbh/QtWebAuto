#include <QHash>
#include "test_case.h"

void TestCase::SetUrl(const QUrl& url)
{
	url_ = url;
}

void TestCase::SetRunningInterval(const int& interval)
{
	RunningInterval = interval;
}

void TestCase::InsertCaseStep(const QUrl& url, const TestCaseStep& case_step)
{
	case_steps_.insert(url, case_step);
}

void TestCase::SetStopStep(const QString& stop_step)
{
	stop_step_ = stop_step;
}

const QUrl& TestCase::GetUrl(void)
{
	return url_;
}

const int& TestCase::GetRunningInterval(void)
{
	return RunningInterval;
}

const QString& TestCase::GetStopStep(void)
{
	return stop_step_;
}

bool TestCase::CheckStopStep(const QString& stop_step)
{
	return QString::compare(stop_step_, stop_step) == 0;
}

TestCaseStep TestCase::GetCaseStep(const QUrl& url)
{
	return case_steps_.value(url);
}
