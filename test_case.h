#pragma once
#ifndef TEST_CASE_H
#define TEST_CASE_H

#include "test_case_step.h"

class TestCase
{
public:
	static void SetUrl(const QUrl&);
	static void SetRunningInterval(const int&);
	static void InsertCaseStep(const QUrl&, const TestCaseStep&);
	static void SetStopStep(const QString&);

	static const QUrl& GetUrl(void);
	static const int& GetRunningInterval(void);
	static const QString& GetStopStep(void);
	static bool CheckStopStep(const QString&);
	static TestCaseStep GetCaseStep(const QUrl&);

private:
	inline static QUrl url_{};
	inline static int RunningInterval{};
	inline static QHash<QUrl, TestCaseStep> case_steps_{};
	inline static QString stop_step_{};
};
#endif