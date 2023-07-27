#include "test_case_step.h"

void TestCaseStep::SetUrl(const QUrl& arg_url)
{
	url_ = arg_url;
}

void TestCaseStep::SetIsAutomatic(const bool& arg_is_automatic)
{
	is_automatic_ = arg_is_automatic;
}

void TestCaseStep::InsertAutomatic(const QString& arg_automatic)
{
	list_automatic_.push_back(arg_automatic);
}

void TestCaseStep::InsertNonAutomatic(const QString& arg_non_automatic)
{
	list_non_automatic_.push_back(arg_non_automatic);
}

void TestCaseStep::ReplaceLastNonAutomatic(const QString& arg_non_automatic)
{
	list_non_automatic_.replace(list_non_automatic_.size() - 1, arg_non_automatic);
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
