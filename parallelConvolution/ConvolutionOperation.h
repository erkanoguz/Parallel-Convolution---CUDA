
#ifndef CONVOLUTION_OPERATION_H
#define CONVOLUTION_OPERATION_H


#include "ConvolutionInterface.h"

#include <chrono>

using namespace std::chrono;

class ConvolutionOperation :public ConvolutionInterface
{

	/*********IMAGE OPERATION**********/
	void imageOperaiton(ImageOperation*&);


	/**************INITIALIZE*************/
	void Init(ImageOperation*& ImOp, char* path);

	/*********CPU**********/
	// Sepratable convolution
	void convolutionRowCPU(const float*, const float*, float*, int, int, int);		// row convolution		
	void convolutionColCPU(const float*, const float*, float*, int, int, int);		// column convolution
	void runCPUConvolution();														// perform CPU convolution
	
	/*********GPU**********/
	void runGPUConvolution();	// Perform Convolution on GPU

	/*Compare Result*/
	void CompareResult();


	/*Visualizing*/
	void showResult();

private:

	/****VARIABLES******/

	// Common Variable
	Mat image;						// input image thath will be processed
	ImageOperation* ImOp;			// Image operation 
	int rows, cols;					// image columns and rows
	int paddedRows, paddedCols;		// padded image size
	float* inputIm;				

	float* kernelRow, * kernelCol;

	// CPU Variable
	float* CPUInputIm, *CPUTempIm, *CPUOutputIm;
	Mat OutputIm;
	duration <double> CPUElapsedTime;

	// GPU Variable
	float* GPUInputIm, * GPUTempIm, * GPUOutputIm;
	float* dev_inputIm, * dev_tempIm, *dev_outputIm;
	float* dev_kernelRow, *dev_kernelCol;
	int kernelByteSize;
	int imageByteSize;
	int paddedImageByteSize;
	Mat GPUOutputImageResult;
	duration <double> GPUElapsedTime;
};

#endif // !CONVOLUTION_OPERATION_H

