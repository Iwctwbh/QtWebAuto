#pragma once
#ifndef TEST_CASE_STEP_H
#define TEST_CASE_STEP_H

#include <QUrl>
class TestCaseStep
{
public:
	void SetUrl(const QUrl&);
	void SetIsAutomatic(const bool&);
	void InsertAutomatic(const QString&);
	void InsertNonAutomatic(const QString&);

	[[nodiscard]] const QUrl& GetUrl(void) const;
	[[nodiscard]] const bool& CheckIsAutomatic(void) const;
	[[nodiscard]] const QList<QString>& GetAutomatic(void) const;
	[[nodiscard]] const QList<QString>& GetNonAutomatic(void) const;


private:
	QUrl url_{};
	bool is_automatic_{};
	QList<QString> list_automatic_{};
	QList<QString> list_non_automatic_{};
};
#endif