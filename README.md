# Parallel-Convolution---CUDA
 Separable image convolution using CUDA
 
 
Convolutions are used by many application for engineering. In this project CUDA is used for an efficient and high performance implementation of separable convolutoion filter. All code is developed with Opencv 4.1.0 and CUDA 10.1 libraries on Visiual Studio 2019 platform. Sequential convolution is performed on CPU and parallel convolution is performed on GPU using global memory. In CPU result:0.0034949, in GPU result: 2.66e-05. 

**CONVOLUTION**

In mathematically, Convolution is a mathematical way of combining two signals to form a third signal.

![Ekran Alıntısı](https://user-images.githubusercontent.com/47758157/64062368-2383b580-cbee-11e9-958c-1615c1acfd30.PNG)

In discrete time.

![discretetime](https://user-images.githubusercontent.com/47758157/64062404-a9076580-cbee-11e9-8a26-6d92d181f730.PNG)


Convolution can be implemented into two dimensions.

![2d](https://user-images.githubusercontent.com/47758157/64062422-c4727080-cbee-11e9-8c83-211e1979beeb.PNG)


In image processing, convolution kernel is a scalar product. This scalar product is a parallel operation that is well suited to computation on highly parallel hardware such as the GPU 

![exampleOfConv](https://user-images.githubusercontent.com/47758157/64062436-dd7b2180-cbee-11e9-8c8c-6685a07cdc3b.PNG)


Generally, two dimensional convolution filter requires n * m multiplications. Separable filter is a composition of two one dimensional filter. A separable filter can be divided into one dimensional filters. So that, Convolutions requires m + n multiplications. 

Two dimensional filter

![kernel](https://user-images.githubusercontent.com/47758157/64062463-4793c680-cbef-11e9-9534-e9c294e1596a.PNG)

Divided filters into one dimensional row and column filters,

row filter -> ![rowKernel](https://user-images.githubusercontent.com/47758157/64062465-4793c680-cbef-11e9-910f-69897fe34274.PNG)         column filter ->       ![colKernel](https://user-images.githubusercontent.com/47758157/64062567-a0b02a00-cbf0-11e9-8509-0fc807cbbe5e.PNG)




Inpu image:

![inputImage](https://user-images.githubusercontent.com/47758157/64062468-482c5d00-cbef-11e9-824b-b9ce67775666.PNG)

Output image:

![outputImage](https://user-images.githubusercontent.com/47758157/64062464-4793c680-cbef-11e9-9494-87a6878270eb.PNG)

