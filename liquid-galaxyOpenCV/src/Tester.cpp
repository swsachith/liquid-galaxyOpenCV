//============================================================================
// Name        : OpenCVLearning.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <cvaux.h>
#include <cxcore.h>
#include <highgui.h>

int test(int argc, char *argv[]) {

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

	while (1) {
		p_originalImage = cvQueryFrame(p_captureWebCam);
		if (p_originalImage == NULL) {
			printf("Error: original image source error");
			getchar();
			break;
		}

		// 2nd argument -> minimum filtering value
		//3rd argument -> max filtering value
		cvInRangeS(p_originalImage, CV_RGB(100,100,100), CV_RGB(200,200,200),
				p_processedImage);

		p_strStorage = cvCreateMemStorage(0); // to pass cvHoughCircles

		cvSmooth(p_processedImage, p_processedImage, CV_GAUSSIAN, 9, 9); // uses the gaussian filter

		p_seqCircles = cvHoughCircles(p_processedImage, p_strStorage,
				CV_HOUGH_GRADIENT, 2, p_processedImage->height / 4, 100, 150,
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

	return 0;
}
IplImage* getCameraFrame(CvCapture* &camera) {
	IplImage *frame;
	int w, h;

	// If the camera hasn't been initialized, then open it.
	if (!camera) {
		printf("Accessing the camera ...\n");
		camera = cvCreateCameraCapture(0);
		if (!camera) {
			printf("Couldn't access the camera.\n");
			exit(1);
		}
		// Try to set the camera resolution to 320 x 240.
		cvSetCaptureProperty(camera, CV_CAP_PROP_FRAME_WIDTH, 320);
		cvSetCaptureProperty(camera, CV_CAP_PROP_FRAME_HEIGHT, 240);
		// Get the first frame, to make sure the camera is initialized.
		frame = cvQueryFrame(camera);
		if (frame) {
			w = frame->width;
			h = frame->height;
			printf("Got the camera at %dx%d resolution.\n", w, h);
		}
		// Wait a little, so that the camera can auto-adjust its brightness.
		sleep(1000); // (in milliseconds)
	}

	// Wait until the next camera frame is ready, then grab it.
	frame = cvQueryFrame(camera);
	if (!frame) {
		printf("Couldn't grab a camera frame.\n");
		exit(1);
	}
	return frame;
}

