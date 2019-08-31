#ifndef __SOLUTION_INTERFACE__H
#define __SOLUTION_INTERFACE__H




/*Opencv Libraries*/
#include <opencv2/highgui.hpp>
/*Opencv Libraries*/
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <string>
#include <iostream>

#include <helper_functions.h>
#include <helper_cuda.h>
#include "device_launch_parameters.h"
#include "cuda_runtime.h"

using namespace cv;
using namespace std;

class SolutionInterface
{
public:
	virtual ~SolutionInterface() {}
	virtual void imageRead(char*) = 0;
	virtual void convertImage(void) = 0;
	virtual void Mat2Ptr(Mat&, float*&) = 0;
	virtual void ptr2Mat(Mat&, float*, int, int) = 0;
	virtual void setZero(float*, int, int) = 0;
	virtual void paddingImage(const float*, float*, int, int) = 0;
	virtual void CPUInit(void) = 0;
	virtual void convolutionRowCPU(const float*, const float*, float*, int, int, int) = 0;
	virtual void convolutionColCPU(const float*, const float*, float*, int, int, int) = 0;
	virtual void runConvolutionCPU(void) = 0;
	virtual void kernelImplementation(float*, float*) = 0;
	virtual void CPUConvolution() = 0;
	virtual double getCPUTime() = 0;

	virtual void GPUInit(void) = 0;
	virtual void GPUConvolution() = 0;
	virtual void runConvolutionGPU(void) = 0;


	/*DEBUG*/

	virtual void testGPU() = 0;

	/*common*/
	virtual void showResult(void) = 0;



};

#endif