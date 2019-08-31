#ifndef IMAGE_INTERFACE_H
#define IMAGE_INTERFACE_H


/*Opencv Libraries*/
#include <opencv2/highgui.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

class ImageInterface
{
public:

	virtual void imageRead(char*) = 0;							// Read image from specified path
	virtual void Mat2Ptr(Mat&, float*&, int, int) = 0;			// Convert image from Mat type to float 
	virtual void Ptr2Mat(float*, Mat&, int, int) = 0;			// Convert image from float to Mat type
	virtual void padding(float*, float*, int, int) = 0;			// Zero padding to perform convolution
	virtual void kernelImplementation() = 0;					// implement kernel row and column
	virtual void getRowKernel(float*) = 0;						// get row kernel
	virtual void getColKernel(float*) = 0;						// get row kernel
	virtual void setZero(float*, int, int) = 0;					// Set input image to zero
	virtual void uint2float(Mat&) = 0;							// Convert input image uint to float
	virtual void float2uint(Mat&) = 0;							// Convert input image float to uint
	virtual Mat getImage() = 0;									// get image
};


#endif // !IMAGE_INTERFACE_H
