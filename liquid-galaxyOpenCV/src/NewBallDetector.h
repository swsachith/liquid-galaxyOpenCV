#include <core/core.hpp>
#include <highgui/highgui.hpp>
#include <imgproc/imgproc.hpp>

#include <iostream>

// this function works in OpenCV 2.x versions
// this is also the Object-oriented version
int detectBall_new() {

	cv::VideoCapture capWebcam(0);
	if (capWebcam.isOpened() == false) {
		std::cout << "error: Web cam cannot be accessed\n";
		return (1);
	}

	cv::Mat matOriginal;
	cv::Mat matProcessed;

	std::vector<cv::Vec3f> vecCircles;
	std::vector<cv::Vec3f>::iterator circleIterator;

	char checkForEscapeKey = 0;

	while (checkForEscapeKey != 27) {
		if (capWebcam.read(matOriginal) == NULL) {
			std::cout << "error: frame cannot be read from webcam\n";
			break;
		}
		// filter the image
		cv::inRange(matOriginal, cv::Scalar(175, 0, 0),
				cv::Scalar(256, 100, 100), matProcessed);

		//use gausian blur with a 9px x 9px window
		cv::GaussianBlur(matProcessed, matProcessed, cv::Size(9, 9), 1.5);

		//get the circles
		cv::HoughCircles(matProcessed, vecCircles, CV_HOUGH_GRADIENT,
				matProcessed.rows / 4, 100, 50, 10, 400);

		for (circleIterator = vecCircles.begin();
				circleIterator != vecCircles.end(); circleIterator++) {
			std::cout << "ball position x = " << (*circleIterator)[0] << "y = "
					<< (*circleIterator)[1] << "radius = "
					<< (*circleIterator)[2] << "\n";

			//draw the center
			cv::circle(matOriginal,
					cv::Point((int) (*circleIterator)[0],
							(int) (*circleIterator)[1]), 3,
					cv::Scalar(0, 255, 0), CV_FILLED);
			//draw the circle
			cv::circle(matOriginal,
					cv::Point((int) (*circleIterator)[0],
							(int) (*circleIterator)[1]),
					(int) (*circleIterator)[2], cv::Scalar(0, 0, 255), 3);
		} //end for
		cv::imshow("Original", matOriginal);
		cv::imshow("Processed", matProcessed);

		checkForEscapeKey = cv::waitKey(10);
	} // end while

}
