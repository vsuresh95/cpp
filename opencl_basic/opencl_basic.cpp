#include <iostream>
#include <OpenCL/opencl.h>
#include <cassert>
#include <benchmark/benchmark.h>
	
// the kernel that we want to execute on the device.
// here, you are doing an addition of elements in an array.
const char* kernelAdd =
{
	"__kernel void add (__global int* data)\n"
	"{\n" 
	"	int work_item_id = get_global_id(0);\n"
	"	data[work_item_id] *= 2;\n"
	"}\n"
};

int main (int argc, char* argv[])
{
	cl_int ret_val;

	// getting the platform ID that can used - here we are getting only one
	cl_platform_id platformID;
	cl_uint numPlatforms;
	if(clGetPlatformIDs(1, &platformID, &numPlatforms))
		std::cout << "clGetPlatformIDs failed!" << std::endl;

	// getting OpenCL device ID for our GPU - here too, we are getting only one
	cl_device_id deviceID;
	cl_uint numDevices;
	if(clGetDeviceIDs(platformID, CL_DEVICE_TYPE_GPU, 1, &deviceID, &numDevices))
		std::cout << "clGetDeviceIDs failed!" << std::endl;

	// printing out some device info. here we have chosen CL_DEVICE_NAME.
	// you can choose any others by referring
	// https://www.khronos.org/registry/OpenCL/sdk/1.0/docs/man/xhtml/clGetDeviceInfo.html
	typedef char typeInfo;
	size_t sizeInfo = 16*sizeof(typeInfo);
	typeInfo* deviceInfo = new typeInfo(sizeInfo); 
	if(clGetDeviceInfo(deviceID, CL_DEVICE_NAME, sizeInfo, (void*) deviceInfo, NULL))
		std::cout << "clGetDeviceInfo failed!" << std::endl;

	std::cout << "CL_DEVICE_NAME = " << deviceInfo << ", platform ID = ";
	std::cout << platformID << ", deviceID = " << deviceID << std::endl;
	
	// set up a context for our device
	cl_context_properties contextProp[3] = {CL_CONTEXT_PLATFORM, (cl_context_properties) platformID, 0};
	cl_context context = clCreateContext(contextProp, 1, &deviceID, NULL, NULL, &ret_val);
	if (ret_val)
		std::cout << "clCreateContext failed!" << std::endl;

	// set up a queue for our device
	cl_command_queue queue = clCreateCommandQueue(context, deviceID, (cl_command_queue_properties) NULL, &ret_val);
	if (ret_val)
		std::cout << "clCreateCommandQueue failed!" << std::endl;

#if 0
	typedef char typeProg;
	size_t programInfo = 1000*sizeof(typeProg);
	typeProg* programStatus = new typeProg(programInfo);
	if((clGetProgramBuildInfo(program, deviceID, CL_PROGRAM_BUILD_LOG, programInfo, (void *) programStatus, NULL)))
		std::cout << "clGetProgramInfo failed!" << std::endl;

	std::cout << programStatus << std::endl;
#endif

	// creating our data set that we want to compute on
	int N = 1 << 16;
	size_t data_size = sizeof(int) * N;
	int* input_data = new int[N];
	int* output_data = new int[N];

	for (int i = 0; i < N; i++)
	{
		input_data[i] = rand() % 1000;
	}

	// create a buffer to where you will eventually enqueue the program for the device
	cl_mem buffer = clCreateBuffer(context, CL_MEM_READ_WRITE, data_size, NULL, &ret_val);
	if (ret_val)
		std::cout << "clCreateBuffer failed!" << std::endl;

	// copying our data set to the buffer
	if((clEnqueueWriteBuffer(queue, buffer, CL_TRUE, 0, data_size, input_data, 0, NULL, NULL)))
		std::cout << "clEnqueueWriteBuffer failed!" << std::endl;

	// we compile the device program with our source above and create a kernel for it.
	// also, we are allowed to create a device program with a binary that we can point to.
	cl_program program = clCreateProgramWithSource(context, 1, (const char**) &kernelAdd, NULL, &ret_val); 
	if (ret_val)
		std::cout << "clCreateProgramWithSource failed!" << std::endl; 

	if((clBuildProgram(program, 1, &deviceID, NULL, NULL, NULL)))
		std::cout << "clBuildProgram failed!" << std::endl; 

	cl_kernel kernel = clCreateKernel(program, "add", &ret_val);
	if (ret_val)
		std::cout << "clCreateKernel failed! ret_val = " << ret_val << std::endl;

	// configure options to find the arguments to the kernel
	if((clSetKernelArg(kernel, 0, sizeof(buffer), &buffer)))
		std::cout << "clSetKernelArg failed!" << std::endl;

	// the total number of work items that we want to use
	const size_t global_dimensions[3] = {data_size, 0, 0};
	if((clEnqueueNDRangeKernel(queue, kernel, 1, NULL, global_dimensions, NULL, 0, NULL, NULL)))
		std::cout << "clEnqueueNDRangeKernel failed!" << std::endl;

	// read back output into another buffer
	ret_val = clEnqueueReadBuffer(queue, buffer, CL_TRUE, 0, data_size, output_data, 0, NULL, NULL);
	if(ret_val)
		std::cout << "clEnqueueReadBuffer failed! ret_val = " << ret_val << std::endl;

#if 0
	// wait for finish - this is to synchronize with the device
	if(clFinish(queue))
		std::cout << "clFinish failed!" << std::endl; 
#endif

	std::cout << "Kernel completed" << std::endl;

	// Release kernel, program, and memory objects
	if(clReleaseMemObject(buffer))
		std::cout << "clReleaseMemObject failed!" << std::endl;

	if(clReleaseKernel(kernel))
		std::cout << "clReleaseKernel failed!" << std::endl;

	if(clReleaseProgram(program))
		std::cout << "clReleaseProgram failed!" << std::endl;

	if(clReleaseCommandQueue(queue))
		std::cout << "clReleaseCommandQueue failed!" << std::endl;

	if(clReleaseContext(context))
		std::cout << "clReleaseContext failed!" << std::endl; 

	for (int i = 0; i < N; i++)
	{
		assert(output_data[i] == input_data[i]*2);
	}

	delete[] input_data;
	delete[] output_data;

	return 0;
}