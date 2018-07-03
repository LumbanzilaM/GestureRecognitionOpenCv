#include "stdafx.h"
#include "HandRegistrationHandler.h"


HandRegistrationHandler::HandRegistrationHandler()
{
	squareSize = 20;
	bool lol;
	lol = face_cascade.load(face_cascade_name);
}


HandRegistrationHandler::~HandRegistrationHandler()
{
}

void HandRegistrationHandler::InitHandRegistration(MyCamImage *camImg)
{
	;
	camImg->readImage(&registrationImg);
	AddSquareRegistration();
	/*rois.push_back(Rect(registrationImg.cols / 2, registrationImg.rows / 2.5, squareSize, squareSize));
	rois.push_back(Rect(registrationImg.cols / 2.5, registrationImg.rows / 1.8, squareSize, squareSize));*/

	//registrationImg(rois[rois.size - 1]);
	while (1) {

		camImg->readImage(&registrationImg);

		for each (Rect rect in rois)
		{
			rectangle(registrationImg, rect, Scalar(255, 0, 0), 2);
		}

		camImg->showImage(&registrationImg);

		if (waitKey(30) == 'n')
		{
			camImg->readImage(&registrationImg);
			break;
		}
	}
}


void HandRegistrationHandler::RegisterHand()
{
	int i = 0;
	for each (Rect rect in rois)
	{
		Mat registrationImgHSV;
		cvtColor(registrationImg(rect), registrationImgHSV, CV_RGB2YCrCb);
		Scalar median = mean(registrationImgHSV);
		medians.push_back(median);
		double a = median[0];
		double b = median[1];
		double c = median[2];
		i++;
	}
	//FindAndHideFace(registrationImg);
	//FindHand(registrationImg);
}

void HandRegistrationHandler::FindAndHideFace(Mat &src)
{
	Mat srcGray;
	std::vector<Rect> faces;
	cvtColor(src, srcGray, CV_RGB2GRAY);
	face_cascade.detectMultiScale(srcGray, faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(100, 100));
	size_t lol = faces.size();
	for each (Rect face in faces)
	{
		rectangle(src, face, Scalar(0), -2);
		Face = face;
	}
	rectangle(src, Face, Scalar(0), -2);
}

Mat HandRegistrationHandler::FindHand(Mat src)
{
	Mat result;
	Mat srcHSV;
	
	
	cvtColor(src, srcHSV, CV_RGB2YCrCb);
	FindAndHideFace(srcHSV);
	rectangle(src, Face, Scalar(0), 2);


	//RotatedRect rotRect = CamShift(src, Face, TermCriteria( TermCriteria::EPS | TermCriteria::COUNT, 10, 1 ));
	
	for each (Scalar median in medians)
	{
		Scalar lower(median[0] - 10, median[1] - 10, median[2] - 10);
		Scalar upper(median[0] + 15, median[1] + 15, median[2] + 15);
		Mat thresholdImg;


		inRange(srcHSV, lower, upper, thresholdImg);
		//threshold(srcHSV, thresholdImg, median, 255, THRESH_BINARY_INV + THRESH_OTSU);
		thresholds.push_back(thresholdImg);
		//imshow("tmp", thresholdImg);
	}


	/*Mat lol = thresholds[0] + thresholds[1] + thresholds[2] + thresholds[3] + thresholds[4] + thresholds[5];

	imshow("resultExpected", lol);*/
	result = thresholds[0];
	for each (Mat thresholdImg in thresholds)
	{
		result += thresholdImg;
	}
	thresholds.clear();
	medianBlur(result, result, 7);
	//imshow("result", result);
	return result;
}

Mat HandRegistrationHandler::getRegistrationImg()
{
	return registrationImg;
}

void HandRegistrationHandler::AddSquareRegistration()
{
	rois.push_back(Rect(registrationImg.cols / 5, registrationImg.rows / 5, squareSize, squareSize));
	rois.push_back(Rect(registrationImg.cols / 7, registrationImg.rows / 3, squareSize, squareSize));
	rois.push_back(Rect(registrationImg.cols / 4, registrationImg.rows / 3, squareSize, squareSize));
	rois.push_back(Rect(registrationImg.cols / 5, registrationImg.rows / 2.5, squareSize, squareSize));
	rois.push_back(Rect(registrationImg.cols / 4.5, registrationImg.rows / 2, squareSize, squareSize));
	rois.push_back(Rect(registrationImg.cols / 6, registrationImg.rows / 2, squareSize, squareSize));
	;
}

void HandRegistrationHandler::InitFaceDetection()
{

}



