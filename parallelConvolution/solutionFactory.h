
#ifndef SOLUTION_FACTORY_H
#define SOLUTION_FACTORY_H

#include "ImageOperation.h"
#include "ConvolutionOperation.h"

class solutionFactory
{
public:
	static ImageOperation* getimageOperation();
	static ConvolutionOperation* getConvOp();
};


#endif // !SOLUTION_FACTORY_H
