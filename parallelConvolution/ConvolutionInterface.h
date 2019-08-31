#ifndef CONVOLUTION_INTERFACE_H
#define CONVOLUTION_INTERFACE_H


/*CUDA LIBRARIES*/
#include <helper_cuda.h>
#include "device_launch_parameters.h"
#include "cuda_runtime.h"

#include "ImageOperation.h"

class  ConvolutionInterface
{
public:

	/*********IMAGE OPERATION**********/
	virtual void imageOperaiton(ImageOperation*&) = 0;

	/**************INITIALIZE************/
	virtual void Init(ImageOperation*& ImOp, char* path) = 0;


	/*********CPU**********/
	virtual void convolutionRowCPU(const float*, const float*, float*, int, int, int) = 0;
	virtual void convolutionColCPU(const float*, const float*, float*, int, int, int) = 0;
	virtual void runCPUConvolution() = 0;
	
	/*********GPU**********/
	virtual void runGPUConvolution() = 0;



	/*Visualizing*/
	virtual void showResult() = 0; 

};

#endif // !CONVOLUTION_INTERFACE_H
