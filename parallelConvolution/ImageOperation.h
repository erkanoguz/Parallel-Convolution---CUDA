#ifndef IMAGE_OPERATION_H
#define IMAGE_OPERATION_H

#include "ImageInterface.h"
#include <assert.h>


#define KERNEL_SIZE 3


class ImageOperation :public ImageInterface 
{
public:
	void imageRead(char*);						// Read image
	void Mat2Ptr(Mat&, float*&, int, int);		// Convert image from Mat to float
	void Ptr2Mat(float*, Mat&, int, int);		// Convert image from float to Mat
	void padding(float*, float*, int, int);		// zero padding 
	void kernelImplementation();				// Implement kernel values
	void getRowKernel(float*);					// get row kernel
	void getColKernel(float*);					// get row kernel
	void setZero(float*, int, int);				// Set all values of input image to zero
	void uint2float(Mat&);						// Convert input image uint 2 float
	void float2uint(Mat&);						// Convert input image float to uint
	Mat getImage();								// get image													
		
private:
	
	/******Functions*******/
	void splitImage();				// Split channels of image
	
									
	/******Variables******/
	Mat image3D;					// 3D image
	Mat image;						// 1D image

	//Kernel varibles
	float* rowKernel;
	float* colKernel;	

	//image parameters
	int rows, cols;
};

#endif // !IMAGE_OPERATION_H