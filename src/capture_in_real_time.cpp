#include "capture_in_real_time.h"

#include <QDebug>
#include <QThread>

#include "test_case.h"
#include "common_tools.hpp"
#include "image_identification.hpp"

CaptureInRealTime::CaptureInRealTime(QObject* parent) : QObject(parent)
{

}

void CaptureInRealTime::CaptureScreenShot()
{
	qDebug() << "Start Capture";

	if (TestCase::web_view_default_widget_)
	{
		qDebug() << "has web view";
		const QPixmap picture{ TestCase::web_view_default_widget_->grab(QRect(0, 0, TestCase::web_view_default_widget_->width(), TestCase::web_view_default_widget_->height())) };
		const cv::Mat capture{ CommonTools::QImageToMat(picture.toImage())};

		const cv::Mat find_image = cv::imread("resources/Error.png", cv::IMREAD_GRAYSCALE);
		//cv::imshow("Find Image", find_image);
		ImageIdentification::FindPositions(capture, find_image);

		//cv::imshow("Capture", capture);
	}

	qDebug() << "End Capture";
}