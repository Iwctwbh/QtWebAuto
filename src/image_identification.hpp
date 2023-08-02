#pragma once
#ifndef IMAGE_IDENTIFICATION_HPP
#define IMAGE_IDENTIFICATION_HPP

#include <opencv2/opencv.hpp>

class ImageIdentification
{
public:
	static void FindPositions(const cv::Mat& arg_large_image, const cv::Mat& arg_small_image)
	{
		cv::Mat large_image = arg_large_image.clone();
		cv::Mat small_image = arg_small_image.clone();

		// 小图规格
		const int small_image_h = small_image.rows;
		const int small_image_w = small_image.cols;

		// 将大图和小图转换为RGB格式
		cv::cvtColor(large_image, large_image, cv::COLOR_RGB2RGBA);
		cv::cvtColor(small_image, small_image, cv::COLOR_RGB2RGBA);

		// 对比大图与小图
		cv::Mat res;
		cv::matchTemplate(large_image, small_image, res, cv::TM_CCOEFF_NORMED);

		// 寻找最匹配位置
		double min_val, max_val;
		cv::Point min_loc, max_loc;
		cv::minMaxLoc(res, &min_val, &max_val, &min_loc, &max_loc);

		qDebug() << "min_val:" << min_val;
		qDebug() << "max_val:" << max_val;

		if (max_val < 0.5)
		{
			std::cout << "Not Found" << std::endl;
			return;
		}
		else
		{
			std::cout << "------------------------Found!!!!!!!!!!!!!-----------------------------" << std::endl;
		}

		const cv::Point top_left = max_loc;
		const cv::Point bottom_right(max_loc.x + small_image_w, max_loc.y + small_image_h);

		// 在大图上画矩形标记匹配位置
		cv::rectangle(large_image, top_left, bottom_right, cv::Scalar(255, 0, 0), 2);

		// 显示结果
		//cv::imshow("positions in large image", large_image);
		//cv::waitKey(0);
	}
};
#endif