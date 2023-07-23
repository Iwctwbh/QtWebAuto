#include "test_case_step.h"

void TestCaseStep::SetUrl(const QUrl& url)
{
	url_ = url;
}

void TestCaseStep::SetIsAutomatic(const bool& flag)
{
	is_automatic_ = flag;
}

void TestCaseStep::InsertAutomatic(const QString& automatic)
{
	list_automatic_.push_back(automatic);
}

void TestCaseStep::InsertNonAutomatic(const QString& non_automatic)
{
	list_non_automatic_.push_back(non_automatic);
}

const QUrl& TestCaseStep::GetUrl(void) const
{
	return url_;
}

const bool& TestCaseStep::CheckIsAutomatic(void) const
{
	return is_automatic_;
}

const QList<QString>& TestCaseStep::GetAutomatic(void) const
{
	return list_automatic_;
}

const QList<QString>& TestCaseStep::GetNonAutomatic(void) const
{
	return list_non_automatic_;
}
