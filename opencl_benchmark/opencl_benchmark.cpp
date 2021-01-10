#include <iostream>
#include <fstream>
#include <OpenCL/opencl.h>
#include <cassert>
#include <benchmark/benchmark.h>

static void openCL_addBenchmark (benchmark::State& s)
{
	// get GPU device ID
	cl_platform_id platformID;
	cl_device_id deviceID;
	cl_uint numDevices;
	clGetPlatformIDs(1, &platformID, NULL);
	clGetDeviceIDs(NULL, CL_DEVICE_TYPE_GPU, 1, &deviceID, &numDevices);

	// set up the context for our GPU
	cl_context_properties contextProp[3] = {CL_CONTEXT_PLATFORM, (cl_context_properties) platformID, 0};
	cl_context context = clCreateContext(contextProp, 1, &deviceID, NULL, NULL, NULL);

	// set up queue
	cl_command_queue queue = clCreateCommandQueue(context, deviceID, (cl_command_queue_properties) NULL, NULL);

	// setting up our test data
	typedef float testType;
	int N = 1 << s.range(0);
	size_t data_size = sizeof(testType) * N;

	testType* a = new testType[N];
	testType* b = new testType[N];
	testType* c = new testType[N];

	for (int i = 0; i < N; i++)
	{
		a[i] = (rand() % 10000) / 3.14;
		b[i] = (rand() % 10000) / 1.732;
	}

	// create 3 buffers in device memory
	// here, we do not use host memory because read bandwidth would be slow
	// if that is needed, we need use CL_MEM_COPY_HOST_PTR and remove the clEnqueueWriteBuffers 
	cl_mem buffer_a = clCreateBuffer(context, CL_MEM_READ_ONLY, data_size, NULL, NULL);
	cl_mem buffer_b = clCreateBuffer(context, CL_MEM_READ_ONLY, data_size, NULL, NULL);
	cl_mem buffer_c = clCreateBuffer(context, CL_MEM_WRITE_ONLY, data_size, NULL, NULL);

	// copying our data set to the buffer
	clEnqueueWriteBuffer(queue, buffer_a, CL_TRUE, 0, data_size, a, 0, NULL, NULL);
	clEnqueueWriteBuffer(queue, buffer_b, CL_TRUE, 0, data_size, b, 0, NULL, NULL);

	const int MAX_SIZE = 1000;
	char* kernelAdd = new char[MAX_SIZE];

	std::fstream f_read;	
	f_read.open("kernelAdd.cl", std::fstream::in);
	f_read.read(kernelAdd, MAX_SIZE);
	f_read.close();

	// compile and build the program, create the kernel and pass the arguments
	cl_program program = clCreateProgramWithSource(context, 1, (const char**) &kernelAdd, NULL, NULL); 
	clBuildProgram(program, 1, &deviceID, NULL, NULL, NULL);

	cl_kernel kernel = clCreateKernel(program, "array_add", NULL);
	clSetKernelArg(kernel, 0, sizeof(buffer_a), &buffer_a);
	clSetKernelArg(kernel, 1, sizeof(buffer_b), &buffer_b);
	clSetKernelArg(kernel, 2, sizeof(buffer_c), &buffer_c);
	
	// starting the workload
	const size_t global_dimensions[3] = {data_size, 0, 0};

	while (s.KeepRunning())
	{
		clEnqueueNDRangeKernel(queue, kernel, 1, NULL, global_dimensions, NULL, 0, NULL, NULL); 
		clFinish(queue);
	}
}

BENCHMARK(openCL_addBenchmark)->DenseRange(20,26);

static void naive_addBenchmark (benchmark::State& s)
{ 
	// setting up our test data
	typedef float testType;
	int N = 1 << s.range(0);
	size_t data_size = sizeof(testType) * N;

	testType* a = new testType[N];
	testType* b = new testType[N];
	testType* c = new testType[N];

	for (int i = 0; i < N; i++)
	{
		a[i] = (rand() % 10000) / 3.14;
		b[i] = (rand() % 10000) / 1.732;
	}	

	while (s.KeepRunning())
	{
		for (int i = 0; i < N; i++)
		{
			c[i] = a[i] * b[i];
		}
		
		benchmark::DoNotOptimize(c);
	}
}

BENCHMARK(naive_addBenchmark)->DenseRange(20,26);

static void openCL_MADBenchmark (benchmark::State& s)
{
	// get GPU device ID
	cl_platform_id platformID;
	cl_device_id deviceID;
	cl_uint numDevices;
	clGetPlatformIDs(1, &platformID, NULL);
	clGetDeviceIDs(NULL, CL_DEVICE_TYPE_GPU, 1, &deviceID, &numDevices);

	// set up the context for our GPU
	cl_context_properties contextProp[3] = {CL_CONTEXT_PLATFORM, (cl_context_properties) platformID, 0};
	cl_context context = clCreateContext(contextProp, 1, &deviceID, NULL, NULL, NULL);

	// set up queue
	cl_command_queue queue = clCreateCommandQueue(context, deviceID, (cl_command_queue_properties) NULL, NULL);

	// setting up our test data
	typedef float testType;
	int N = 1 << s.range(0);
	size_t data_size = sizeof(testType) * N;

	testType* a = new testType[N];
	testType* b = new testType[N];
	testType* c = new testType[N];
	testType* d = new testType[N];

	for (int i = 0; i < N; i++)
	{
		a[i] = (rand() % 10000) / 3.14;
		b[i] = (rand() % 10000) / 1.732;
		c[i] = (rand() % 10000) / 2.87;
	}

	// create 3 buffers in device memory
	// here, we do not use host memory because read bandwidth would be slow
	// if that is needed, we need use CL_MEM_COPY_HOST_PTR and remove the clEnqueueWriteBuffers 
	cl_mem buffer_a = clCreateBuffer(context, CL_MEM_READ_ONLY, data_size, NULL, NULL);
	cl_mem buffer_b = clCreateBuffer(context, CL_MEM_READ_ONLY, data_size, NULL, NULL);
	cl_mem buffer_c = clCreateBuffer(context, CL_MEM_READ_ONLY, data_size, NULL, NULL);
	cl_mem buffer_d = clCreateBuffer(context, CL_MEM_WRITE_ONLY, data_size, NULL, NULL);

	// copying our data set to the buffer
	clEnqueueWriteBuffer(queue, buffer_a, CL_TRUE, 0, data_size, a, 0, NULL, NULL);
	clEnqueueWriteBuffer(queue, buffer_b, CL_TRUE, 0, data_size, b, 0, NULL, NULL);
	clEnqueueWriteBuffer(queue, buffer_c, CL_TRUE, 0, data_size, c, 0, NULL, NULL);

	const int MAX_SIZE = 1000;
	char* kernelMAD = new char[MAX_SIZE];

	std::fstream f_read;	
	f_read.open("kernelMAD.cl", std::fstream::in);
	f_read.read(kernelMAD, MAX_SIZE);
	f_read.close();

	// compile and build the program, create the kernel and pass the arguments
	cl_program program = clCreateProgramWithSource(context, 1, (const char**) &kernelMAD, NULL, NULL); 
	clBuildProgram(program, 1, &deviceID, NULL, NULL, NULL);

	cl_kernel kernel = clCreateKernel(program, "array_add", NULL);
	clSetKernelArg(kernel, 0, sizeof(buffer_a), &buffer_a);
	clSetKernelArg(kernel, 1, sizeof(buffer_b), &buffer_b);
	clSetKernelArg(kernel, 2, sizeof(buffer_c), &buffer_c);
	clSetKernelArg(kernel, 3, sizeof(buffer_d), &buffer_d);
	
	// starting the workload
	const size_t global_dimensions[3] = {data_size, 0, 0};

	while (s.KeepRunning())
	{
		clEnqueueNDRangeKernel(queue, kernel, 1, NULL, global_dimensions, NULL, 0, NULL, NULL); 
		clFinish(queue);
	}
}

BENCHMARK(openCL_MADBenchmark)->DenseRange(20,26);

static void naive_MADBenchmark (benchmark::State& s)
{ 
	// setting up our test data
	typedef float testType;
	int N = 1 << s.range(0);
	size_t data_size = sizeof(testType) * N;

	testType* a = new testType[N];
	testType* b = new testType[N];
	testType* c = new testType[N];
	testType* d = new testType[N];

	for (int i = 0; i < N; i++)
	{
		a[i] = (rand() % 10000) / 3.14;
		b[i] = (rand() % 10000) / 1.732;
		c[i] = (rand() % 10000) / 2.87;
	}	

	while (s.KeepRunning())
	{
		for (int i = 0; i < N; i++)
		{
			d[i] = (a[i] * b[i]) + c[i];
		}
		
		benchmark::DoNotOptimize(d);
	}
}

BENCHMARK(naive_MADBenchmark)->DenseRange(20,26);

BENCHMARK_MAIN();
