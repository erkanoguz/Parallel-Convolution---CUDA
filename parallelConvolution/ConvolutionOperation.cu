#include "ConvolutionOperation.h"


/*Get Image Operation*/
void ConvolutionOperation::imageOperaiton(ImageOperation*& ImOp)
{
	this->ImOp = ImOp;
}



/*Initializing all process*/
void ConvolutionOperation::Init(ImageOperation*& ImOp, char* path)
{
	/*Inýtialize Image Operaitons*/
	imageOperaiton(ImOp);

	/*Read image from specified path*/
	ImOp->imageRead(path);

	
	/*Get Kernels*/
	kernelCol = new float[KERNEL_SIZE];
	kernelRow = new float[KERNEL_SIZE];

	ImOp->kernelImplementation();

	ImOp->getColKernel(kernelCol);
	ImOp->getRowKernel(kernelRow);
	

	/*Get image that will be convoluted*/
	image = ImOp->getImage();
	
	/*Get image size*/
	rows = image.rows;										// number of input image rows 
	cols = image.cols;										// number of input image columns
	paddedCols = rows + ceil(KERNEL_SIZE / float(2));		// number of padded image rows
	paddedRows = cols + ceil(KERNEL_SIZE / float(2));		// number of padded image columns
	
	/*Convert input image from Mat to float*/
	inputIm = new float[rows * cols];
	ImOp->Mat2Ptr(image, inputIm, rows, cols);


	/*Convert image uint to float*/
	ImOp->uint2float(image);

	/*CPU Initializing*/
	CPUInputIm = new float[paddedRows * paddedCols];
	CPUTempIm = new float[rows * cols];
	CPUOutputIm = new float[rows * cols];

	ImOp->padding(inputIm, CPUInputIm, paddedRows, paddedCols);
	ImOp->setZero(CPUTempIm, rows, cols);
	ImOp->setZero(CPUOutputIm, rows, cols);
	OutputIm = Mat::zeros(rows , cols , CV_32F);


	/*GPU Initializing*/
	kernelByteSize = sizeof(float) * KERNEL_SIZE;
	imageByteSize = sizeof(float) * rows * cols;
	paddedImageByteSize = sizeof(float) * paddedRows * paddedCols;

	GPUInputIm = new float[paddedRows * paddedCols];
	GPUTempIm = new float[rows * cols];
	GPUOutputIm = new float[rows * cols];

	ImOp->padding(inputIm, GPUInputIm, paddedRows, paddedCols);
	ImOp->setZero(GPUTempIm, rows, cols);
	ImOp->setZero(GPUOutputIm, rows, cols);

	GPUOutputImageResult = Mat::zeros(rows, cols, CV_32F);
}


void ConvolutionOperation::showResult()
{
	/*Show input image*/
	ImOp->float2uint(image);
	imshow("Input Image", image);
	
	/*Show CPU Result*/
	ImOp->float2uint(OutputIm);
	imshow("CPU Convolution Result", OutputIm);

	/*Show GPU Result*/
	ImOp->float2uint(GPUOutputImageResult);
	imshow("GPU Convolution Result", GPUOutputImageResult);
	
}



/******************************************************************************************************************************************/
/*-------------------------------------------------------CPU CONVOLUTION------------------------------------------------------------------*/
/******************************************************************************************************************************************/

void ConvolutionOperation::convolutionRowCPU(const float* source, const float* kernel, float* output, int xSize, int ySize, int kernel_size)
{

	float* temp = new float[kernel_size];
	int outRow = xSize - 2;
	int outCol = ySize - 2;

	for (int i = floor(kernel_size / 2); i < xSize - (kernel_size / 2); i++)
	{
		for (int j = floor(kernel_size / 2); j < ySize - floor(kernel_size / 2); j++)
		{
			for (int c = 0; c < 3; c++)
			{
				*(temp + c) = *(kernel + c) * *(source + i * ySize + (j + (c - kernel_size + 2)));
			}
			*(output + (i - 1) * outCol + (j - 1)) = *(temp + 0) + *(temp + 1) + *(temp + 2);
		}
	}

}

void ConvolutionOperation::convolutionColCPU(const float* source, const float* kernel, float* output, int xSize, int ySize, int kernel_size)
{

	float* temp = new float[kernel_size];
	int outRow = xSize - 2;
	int outCol = ySize - 2;

	for (int i = floor(kernel_size / 2); i < xSize - (kernel_size / 2); i++)
	{
		for (int j = floor(kernel_size / 2); j < ySize - floor(kernel_size / 2); j++)
		{
			for (int c = 0; c < 3; c++)
			{
				*(temp + c) = *(kernel + c) * *(source + (i + (c - kernel_size + 2)) * ySize + j);
			}
			*(output + (i - 1) * outCol + (j - 1)) = *(temp + 0) + *(temp + 1) + *(temp + 2);
		}
	}

}

void ConvolutionOperation::runCPUConvolution()
{

	auto startTime = high_resolution_clock::now();
	convolutionColCPU(CPUInputIm, kernelCol, CPUTempIm, paddedRows, paddedCols, KERNEL_SIZE);
	ImOp->padding(CPUTempIm, CPUInputIm, paddedRows, paddedCols);
	convolutionRowCPU(CPUInputIm, kernelRow, CPUOutputIm, paddedRows, paddedCols, KERNEL_SIZE);
	auto endTime = high_resolution_clock::now();
	CPUElapsedTime = endTime - startTime;
	cout << "CPU elapsed time: " << CPUElapsedTime.count() << endl;
	
	/*Float to Mat output image*/
	ImOp->Ptr2Mat(CPUOutputIm, OutputIm, rows, cols);
	

}


/******************************************************************************************************************************************/
/*-------------------------------------------------------GPU CONVOLUTION------------------------------------------------------------------*/
/******************************************************************************************************************************************/

__global__ void GPUconvolutionCol(float* source, float* kernel, float* output, int xSize, int ySize, int kernel_size)
{

	int outRow = xSize - 2;
	int outCol = ySize - 2;

	int i = threadIdx.x + blockDim.x * blockIdx.x + 1;
	int j = threadIdx.y + blockDim.y * blockIdx.y + 1;

	if (i >= 1 && i < xSize - 1 && j >= 1 && j < ySize - 1)
	{
		*(output + (i - 1) * outCol + (j - 1)) = 0;
		for (int c = 0; c < 3; c++)
		{
			*(output + (i - 1) * outCol + (j - 1)) += *(kernel + c) * *(source + (i + (c - kernel_size + 2)) * ySize + j);
		}
	}
}

__global__ void paddingImageGPU(float* source, float* padded, int xPaddedSize, int yPaddedSize)
{

	int ySize = yPaddedSize - 2;
	int i = threadIdx.x + blockIdx.x * blockDim.x + 1;
	int j = threadIdx.y + blockIdx.y * blockDim.y + 1;


	if (i >= 1 && i < xPaddedSize - 1 && j >= 1 && j < xPaddedSize - 1)
	{
		*(padded + i * yPaddedSize + j) = *(source + (i - 1) * ySize + (j - 1));
	}
}

__global__ void GPUconvolutionRow(float* source, float* kernel, float* output, int xSize, int ySize, int kernel_size)
{

	//float* temp = new float[kernel_size];
	int outRow = xSize - 2;
	int outCol = ySize - 2;

	int i = threadIdx.x + blockIdx.x * blockDim.x + 1;
	int j = threadIdx.y + blockIdx.y * blockDim.y + 1;

	if (i >= 1 && i < xSize - 1 && j >= 1 && j < ySize - 1)
	{
		*(output + (i - 1) * outCol + (j - 1)) = 0;
		for (int c = 0; c < 3; c++)
		{
			*(output + (i - 1) * outCol + (j - 1)) += *(kernel + c) * *(source + i * ySize + (j + (c - kernel_size + 2)));
		}
	}
}

void ConvolutionOperation::runGPUConvolution()
{

	/*Allocate device memory*/
	cudaMalloc((void**)&dev_inputIm, paddedImageByteSize);
	cudaMalloc((void**)&dev_tempIm, imageByteSize);
	cudaMalloc((void**)& dev_outputIm, imageByteSize);
	cudaMalloc((void**)&dev_kernelRow, kernelByteSize);
	cudaMalloc((void**)& dev_kernelCol, kernelByteSize);
	
	/*Transfer Variable from gost to device*/
	cudaMemcpy(dev_inputIm, GPUInputIm, paddedImageByteSize, cudaMemcpyHostToDevice);
	cudaMemcpy(dev_tempIm, GPUTempIm, imageByteSize, cudaMemcpyHostToDevice);
	cudaMemcpy(dev_outputIm, GPUOutputIm, imageByteSize, cudaMemcpyHostToDevice);
	cudaMemcpy(dev_kernelRow, kernelRow, kernelByteSize, cudaMemcpyHostToDevice);
	cudaMemcpy(dev_kernelCol, kernelCol, kernelByteSize, cudaMemcpyHostToDevice);
	
	/*CUDA configuration*/

	int xDim = 16;
	int yDim = 16;
	dim3 block(xDim, yDim);
	dim3 grid((paddedRows + xDim - 1) / xDim, (paddedCols + yDim - 1) / yDim);

	auto startTime = high_resolution_clock::now();
	GPUconvolutionCol << <grid, block >> > (dev_inputIm, dev_kernelCol, dev_tempIm, paddedRows, paddedCols, KERNEL_SIZE);
	paddingImageGPU << <grid, block >> > (dev_tempIm, dev_inputIm, paddedRows, paddedCols);
	GPUconvolutionRow << <grid, block >> > (dev_inputIm, dev_kernelRow, dev_outputIm, paddedRows, paddedCols, KERNEL_SIZE);
	auto endTime = high_resolution_clock::now();

	duration <double> GPUElapsedTime = endTime - startTime;

	cout << "GPU elapsed time: " << GPUElapsedTime.count() << endl;

	/*GPU convolution result*/
	cudaMemcpy(GPUOutputIm, dev_outputIm, imageByteSize, cudaMemcpyDeviceToHost);

	ImOp->Ptr2Mat(GPUOutputIm, GPUOutputImageResult, rows, cols);

}

