#include "capture_in_real_time.h"

#include <QDebug>
#include <QDir>
#include <QThread>
#include <QTest>

#include "test_case.h"
#include "common_tools.hpp"
#include "image_identification.hpp"

CaptureInRealTime::CaptureInRealTime(QObject* parent) : QObject(parent)
{
	timer_ = new QTimer(this);
	timer_->setInterval(time_interval_);
	QObject::connect(timer_, &QTimer::timeout, this, &CaptureInRealTime::CaptureScreenShot);
	timer_->start();
}

void CaptureInRealTime::CaptureScreenShot()
{
	if (TestCase::web_view_default_widget_)
	{
		const QPixmap picture{ TestCase::web_view_default_widget_->grab(QRect(0, 0, TestCase::web_view_default_widget_->width(), TestCase::web_view_default_widget_->height())) };
		const cv::Mat capture{ CommonTools::QImageToMat(picture.toImage())};

		const QDir dir{ TestCase::folder_path_image_identification_ };
		const QStringList list_files{ dir.entryList(QStringList{ "*.png" }, QDir::Files) };
		std::ranges::for_each(list_files, [&capture, this](const QString& s)
			{
				const cv::Mat find_image = cv::imread((TestCase::folder_path_image_identification_ + "/" + s).toStdString(), cv::IMREAD_GRAYSCALE);
				if (ImageIdentification::FindPositions(capture, find_image))
				{
					timer_->stop();
					emit throw_error();
					if (identification_count_limit_ != 1)
					{
						--identification_count_limit_;
						QTest::qWait(after_identification_wait_time_);
						timer_->start();
					}
					return;
				}
			});
	}
}
