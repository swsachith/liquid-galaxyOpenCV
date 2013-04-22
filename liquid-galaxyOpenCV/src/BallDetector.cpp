//============================================================================
// Name        : BallDetector.cpp
// Author      : Sachith Withana
// Version     : 1.0
// Description : controll liquid-galaxy with a ball.
//============================================================================
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <cvaux.h>
#include <cxcore.h>
#include <highgui.h>

int detectBall();

int main(int argc, char *argv[]) {
	detectBall();
	return 0;
}
int detectBall(){

	CvSize size640x480 = cvSize(640, 480);
	CvCapture* p_captureWebCam; // webcam stream
	IplImage* p_originalImage;
	IplImage* p_processedImage;
	CvMemStorage* p_strStorage;
	CvSeq* p_seqCircles;

	float* p_fltxyRadius; // pointer to the x,y and the radius of the object

	int i;

	char checkForEscapeKey; // exit when the escape key is pressed

	p_captureWebCam = cvCaptureFromCAM(0);

	if (p_captureWebCam == NULL) {
		printf("Error: Capture is null \n");
		getchar(); // wait till a key is pressed
		return -1;
	}
	//creating the two windows
	cvNamedWindow("Original", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("Processed", CV_WINDOW_AUTOSIZE);

	p_processedImage = cvCreateImage(size640x480, IPL_DEPTH_8U, 1); // processing a 8bit depth 1 channel gray scale image


	//detecting the ball
	while (1) {
		p_originalImage = cvQueryFrame(p_captureWebCam);
		if (p_originalImage == NULL) {
			printf("Error: original image source error");
			getchar();
			break;
		}

		// 2nd argument -> minimum filtering value
		//3rd argument -> max filtering value
		cvInRangeS(p_originalImage, CV_RGB(0,0,100), CV_RGB(50,50,255),
				p_processedImage);

		p_strStorage = cvCreateMemStorage(0); // to pass cvHoughCircles

		cvSmooth(p_processedImage, p_processedImage, CV_GAUSSIAN, 9, 9); // uses the gaussian filter

		p_seqCircles = cvHoughCircles(p_processedImage, p_strStorage,
				CV_HOUGH_GRADIENT, 2, p_processedImage->height / 4, 100, 50,
				10, 400);
		//last two parameters -> min and the max radius of the ball

		for (i = 0; i < p_seqCircles->total; i++) { // runs for each object detected
			p_fltxyRadius = (float*) cvGetSeqElem(p_seqCircles, i);
			printf("ball position x = %.2f y = %.2f r = %.2f", p_fltxyRadius[0],
					p_fltxyRadius[1], p_fltxyRadius[2]);

			//drawing the circles around the objects
			cvCircle(p_originalImage,
					cvPoint(cvRound(p_fltxyRadius[0]),
							cvRound(p_fltxyRadius[1])), 3, CV_RGB(0,0,255),
					CV_FILLED);
			cvCircle(p_originalImage,
					cvPoint(cvRound(p_fltxyRadius[0]),
							cvRound(p_fltxyRadius[1])),
					cvRound(p_fltxyRadius[2]), CV_RGB(255,0,0), 3);

		}
		//showing the image
		cvShowImage("Original", p_originalImage);
		cvShowImage("Processed", p_processedImage);

		cvReleaseMemStorage(&p_strStorage);

		checkForEscapeKey = cvWaitKey(10); // wait for 10 more seconds to check for esc key
		if (checkForEscapeKey == 27)
			break;
	}

	cvReleaseCapture(&p_captureWebCam);

	cvDestroyWindow("Original");
	cvDestroyWindow("Processed");

}
