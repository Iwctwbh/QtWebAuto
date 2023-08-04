#pragma once
#ifndef CAPTURE_IN_REAL_TIME_HPP
#define CAPTURE_IN_REAL_TIME_HPP

#include <QObject>

class CaptureInRealTime final : public QObject
{
	Q_OBJECT
public:
	explicit CaptureInRealTime(QObject* parent = nullptr);

	void CaptureScreenShot(void);

	inline static int time_interval_{ 1000 }; // ms
	inline static int after_identification_wait_time_{ 3000 }; // ms
	inline static int identification_count_limit_{ 3 };

private:
	QTimer* timer_{};

signals:
	void throw_error(void);
};
#endif
