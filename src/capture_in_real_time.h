#pragma once
#ifndef CAPTURE_IN_REAL_TIME_HPP
#define CAPTURE_IN_REAL_TIME_HPP

#include <QObject>

class CaptureInRealTime final : public QObject
{
	Q_OBJECT
public:
	explicit CaptureInRealTime(QObject* parent = nullptr);

	static void CaptureScreenShot(void);

signals:
	void throw_error(void);
};
#endif
