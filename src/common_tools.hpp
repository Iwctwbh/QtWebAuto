#pragma once
#ifndef COMMON_TOOLS_HPP
#define COMMON_TOOLS_HPP

#include <opencv2/opencv.hpp>
#include <QImage>

class CommonTools final
{
public:
	static QImage Mat2QImage(cv::Mat const& src)
	{
		cv::Mat temp; // make the same cv::Mat
		cvtColor(src, temp, cv::COLOR_BGR2RGB); // cvtColor Makes a copt, that what i need
		QImage dest(static_cast<const uchar*>(temp.data), temp.cols, temp.rows, temp.step, QImage::Format_RGB888);
		dest.bits(); // enforce deep copy, see documentation 
		// of QImage::QImage ( const uchar * data, int width, int height, Format format )
		return dest;
	}

	static cv::Mat QImageToMat(const QImage& image)
	{
		cv::Mat out;
		switch (image.format()) {
			case QImage::Format_Invalid:
			{
				const cv::Mat empty;
				empty.copyTo(out);
				break;
			}
			case QImage::Format_RGB32:
			{
				const cv::Mat view(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
				view.copyTo(out);
				break;
			}
			case QImage::Format_RGB888:
			{
				const cv::Mat view(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
				cv::cvtColor(view, out, cv::COLOR_RGB2BGR);
				break;
			}
			default:
			{
				const QImage conv = image.convertToFormat(QImage::Format_ARGB32);
				const cv::Mat view(conv.height(), conv.width(), CV_8UC4, (void*)conv.constBits(), conv.bytesPerLine());
				view.copyTo(out);
				break;
			}
		}
		return out;
	}
};
#endif
