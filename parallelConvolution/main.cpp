#include <iostream>
#include "solutionFactory.h"

using namespace std;



int main()
{	
	char* path = "image.png";

	ImageOperation* ImOp = solutionFactory::getimageOperation();
	ConvolutionInterface* ConvOp = solutionFactory::getConvOp();

	ConvOp->Init(ImOp,path);
	ConvOp->runCPUConvolution();
	ConvOp->runGPUConvolution();
	ConvOp->showResult();


	waitKey(0);
	
}