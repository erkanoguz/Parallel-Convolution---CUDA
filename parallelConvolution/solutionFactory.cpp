

#include "solutionFactory.h"



ImageOperation* solutionFactory::getimageOperation()
{
	ImageOperation* imOp = new ImageOperation();
	return imOp;
}


ConvolutionOperation* solutionFactory::getConvOp()
{
	ConvolutionOperation* ConvOp = new ConvolutionOperation();
	return ConvOp;
}

