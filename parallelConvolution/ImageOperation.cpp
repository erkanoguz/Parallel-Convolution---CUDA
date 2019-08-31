#include "ImageOperation.h"

/*Read Image*/
void ImageOperation::imageRead(char* path)
{
	image3D = imread(path);			// Read 3D image
	
	if (image3D.empty()) // check image is read properly
		cout << "ERROR! image cannot read!" << endl;
	else
		cout << "input Image is read!" << endl;
	

	/*image parameters*/
	rows = image3D.rows;	// number of rows of image3D
	cols = image3D.cols;	// number of columns of image3D
	

	/*3d -> 1D*/
	splitImage();		


}

/*convert image pointer to Mat*/
void ImageOperation::Ptr2Mat(float* inputIm, Mat& outputIm, int row, int col)
{
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			outputIm.at<float>(i, j) = *(inputIm + i * col + j);
		}
	}
}

/*Convert image Mat to pointer*/
void ImageOperation::Mat2Ptr(Mat& inputIm, float*& outputIm, int row, int col)
{
	outputIm = (float*)inputIm.data;
}

/*Zero Padding*/
void ImageOperation::padding(float* inputIm, float* outputIm, int row, int col)
{
	int inputCols = col - 2;		// input number of column 

	for (int i = 1; i < row - 1; i++)
	{
		for (int j = 1; j < col - 1; j++)
		{
			*(outputIm + i * col + j) = *(inputIm + (i - 1) * inputCols + (j - 1));
		}
	}
}

/*Kernel implementation*/
void ImageOperation::kernelImplementation()
{
	/*
	row kernel = [-1 0 -1]
	columnf kernel = [1 2 1]
	*/

	rowKernel = new float[KERNEL_SIZE];
	colKernel = new float[KERNEL_SIZE];

	// Row kernel implementation
	*(rowKernel + 0) = -1;
	*(rowKernel + 1) = 0;
	*(rowKernel + 2) = 1;

	// Col kernel implementation
	*(colKernel + 0) = 1;
	*(colKernel + 1) = 2;
	*(colKernel + 2) = 1;
}

/*Zet zero all memeber of image*/
void ImageOperation::setZero(float* inputIm, int row, int col)
{
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			*(inputIm + col * i + j) = 0;
		}
	}
}

void ImageOperation::splitImage()
{
	vector<Mat> channels;
	image3D.convertTo(image3D, CV_32F);						// convert image 32 bit float type  
	split(image3D, channels);								// split image
	image = channels[2];									// Take one of the channels of image3D
}

void ImageOperation::uint2float(Mat& inputIm)
{
	
	inputIm.convertTo(inputIm, CV_32F);
}

void ImageOperation::float2uint(Mat& inputIm)
{

	inputIm.convertTo(inputIm, CV_8U);
}

Mat ImageOperation::getImage() 
{
	return image;
}

/********************Get Kernels*****************/
void ImageOperation::getColKernel(float* kernel)
{
	*(kernel + 0) = *(colKernel + 0);
	*(kernel + 1) = *(colKernel + 1);
	*(kernel + 2) = *(colKernel + 2);
}

void ImageOperation::getRowKernel(float* kernel)
{
	*(kernel + 0) = *(rowKernel + 0);
	*(kernel + 1) = *(rowKernel + 1);
	*(kernel + 2) = *(rowKernel + 2);
}

