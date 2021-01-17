#pragma once

// ConsoleApplication1.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include"iostream"
#include"fstream"
#include "string.h"
#include "stdlib.h"
#include "CL/cl.h"
#include <stdio.h> 
#include"vector"

#pragma warning( disable : 4996 )

using namespace std;

void check_result(const float *buf, const int len) {
	int i;
	for (i = 0; i < len; i++) {
		cout << buf[i] << endl;
	}
	if (i == len)
		cout << len << " Result ok." << endl;
}

int huygensCalculate(Field *input, Field *output,float distance) {

	vector<Cell*> *in_intensity = input->getIntensity();
	vector<Cell*> *out_intensity = output->getIntensity();
	float dim = input->getInfo()->getDim();

	int INPUT_SIZE = in_intensity->size();
	const int INPUT_BUF_SIZE = INPUT_SIZE * sizeof(float);
	const int OUTPUT_SIZE = out_intensity->size();
	const int OUTPUT_BUF_SIZE = OUTPUT_SIZE * sizeof(float);
	
	vector<float> a(INPUT_BUF_SIZE);
	vector<float> b(INPUT_BUF_SIZE);
	vector<float> c(INPUT_BUF_SIZE);
	vector<float> d(INPUT_BUF_SIZE);
	vector<float> e(OUTPUT_BUF_SIZE);
	vector<float> f(OUTPUT_BUF_SIZE);
	vector<float> g(OUTPUT_BUF_SIZE);
	vector<float> h(OUTPUT_BUF_SIZE);
	for (int i = 0; i < INPUT_SIZE; i++) {
		a[i] = in_intensity->at(i)->getAmplitude();
		b[i] = in_intensity->at(i)->getPhase();
		c[i] = in_intensity->at(i)->getX();
		d[i] = in_intensity->at(i)->getY();
	}
	for (int i = 0; i < OUTPUT_SIZE; i++) {
		e[i] = out_intensity->at(i)->getAmplitude();
		f[i] = out_intensity->at(i)->getPhase();
		g[i] = out_intensity->at(i)->getX();
		h[i] = out_intensity->at(i)->getY();
	}
	cl_int ret;
	// step 1: get platform 
	cl_uint num_platforms;
	ret = clGetPlatformIDs(0, NULL, &num_platforms);
	// get platform number 
	if ((CL_SUCCESS != ret) || (num_platforms < 1)) {
		cout << "Error getting platform number: " << ret << endl;
		return 0;
	}
	cl_platform_id platform_id = NULL;
	ret = clGetPlatformIDs(1, &platform_id, NULL);
	// get first platform id 
	if (CL_SUCCESS != ret) {
		cout << "Error getting platform id: " << ret << endl;
		return 0;
	}
	//** step 2: get device 
	cl_uint num_devices;
	clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 0, NULL, &num_devices);
	if ((CL_SUCCESS != ret) || (num_devices < 1)) {
		cout << "Error getting GPU device number: " << ret << endl;
		return 0;
	}
	cl_device_id device_id = NULL;
	clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, NULL);
	if (CL_SUCCESS != ret) {
		cout << "Error getting GPU device id: " << ret << endl;
		return 0;
	}
	//** step 3: create context 
	cl_context_properties props[] = { CL_CONTEXT_PLATFORM, (cl_context_properties)platform_id, 0 };
	cl_context context = NULL;
	context = clCreateContext(props, 1, &device_id, NULL, NULL, &ret);
	if ((CL_SUCCESS != ret) || (NULL == context)) {
		cout << "Error creating context: " << ret << endl;
		return 0;
	}
	//** step 4: create command queue 
	cl_command_queue command_queue = NULL;
	command_queue = clCreateCommandQueue(context, device_id, 0, &ret);
	if ((CL_SUCCESS != ret) || (NULL == command_queue)) {
		cout << "Error creating command queue: " << ret << endl;
		return 0;
	}
	//** step 5: create memory object 
	cl_mem InAm = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, INPUT_BUF_SIZE, &a[0], &ret);
	if ((CL_SUCCESS != ret) || (NULL == InAm)) {
		cout << "Error creating opencl memory object: " << ret << endl;
		return 0;
	}
	cl_mem InPh = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, INPUT_BUF_SIZE, &b[0], &ret);
	if ((CL_SUCCESS != ret) || (NULL == InPh)) {
		cout << "Error creating opencl memory object: " << ret << endl;
		return 0;
	}
	cl_mem InX = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, INPUT_BUF_SIZE, &c[0], &ret);
	if ((CL_SUCCESS != ret) || (NULL == InX)) {
		cout << "Error creating opencl memory object: " << ret << endl;
		return 0;
	}
	cl_mem InY = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, INPUT_BUF_SIZE, &d[0], &ret);
	if ((CL_SUCCESS != ret) || (NULL == InY)) {
		cout << "Error creating opencl memory object: " << ret << endl;
		return 0;
	}
	cl_mem  OutAm= clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, OUTPUT_BUF_SIZE, &e[0], &ret);
	if ((CL_SUCCESS != ret) || (NULL == OutAm)) {
		cout << "Error creating opencl memory object: " << ret << endl;
		return 0;
	}
	cl_mem  OutPh= clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, OUTPUT_BUF_SIZE, &f[0], &ret);
	if ((CL_SUCCESS != ret) || (NULL == OutPh)) {
		cout << "Error creating opencl memory object: " << ret << endl;
		return 0;
	}
	cl_mem OutX = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, OUTPUT_BUF_SIZE, &g[0], &ret);
	if ((CL_SUCCESS != ret) || (NULL == OutX)) {
		cout << "Error creating opencl memory object: " << ret << endl;
		return 0;
	}
	cl_mem OutY = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, OUTPUT_BUF_SIZE, &h[0], &ret);
	if ((CL_SUCCESS != ret) || (NULL == OutY)) {
		cout << "Error creating opencl memory object: " << ret << endl;
		return 0;
	}
	cl_mem Distance = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, sizeof(float), &distance, &ret);
	if ((CL_SUCCESS != ret) || (NULL == Distance)) {
		cout << "Error creating opencl memory object: " << ret << endl;
		return 0;
	}
	cl_mem Size = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, sizeof(int), &INPUT_SIZE, &ret);
	if ((CL_SUCCESS != ret) || (NULL == Size)) {
		cout << "Error creating opencl memory object: " << ret << endl;
		return 0;
	}
	cl_mem Dim = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, sizeof(float), &dim, &ret);
	if ((CL_SUCCESS != ret) || (NULL == Dim)) {
		cout << "Error creating opencl memory object: " << ret << endl;
		return 0;
	}
	//** step 6: create program 
	ifstream in("huygens.cl", std::ios_base::binary);
	if (!in.good()) {
		cout << "Error reading kernel file" << endl;
		return 0;
	}
	// get file length
	in.seekg(0, std::ios_base::end);
	size_t length = in.tellg();
	in.seekg(0, std::ios_base::beg);
	// read program source
	std::vector<char> data(length + 1);
	in.read(&data[0], length);
	data[length] = 0;
	// create and build program 
	const char* kernelSource = &data[0];
	cl_program program = NULL;
	// create program 
	program = clCreateProgramWithSource(context, 1, (const char**)&kernelSource, NULL, &ret);
	if ((CL_SUCCESS != ret) || (NULL == program)) {
		cout << "Error creating program: " << ret << endl;
		return 0;
	}
	// build program 
	ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
	if (CL_SUCCESS != ret) {
		cout << "Error building program: " << ret << endl;
		return 0;
	}
	//** step 7: create kernel 
	cl_kernel kernel = NULL;
	kernel = clCreateKernel(program, "test", &ret);
	if ((CL_SUCCESS != ret) || (NULL == kernel)) {
		cout << "Error creating kernel: " << ret << endl;
		return 0;
	}
	//** step 8: set kernel arguments 
	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), &InAm);
	if (CL_SUCCESS != ret) {
		cout << "Error setting kernel argument: " << ret << endl;
		return 0;
	}
	ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), &InPh);
	if (CL_SUCCESS != ret) {
		cout << "Error setting kernel argument: " << ret << endl;
		return 0;
	}
	ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), &InX);
	if (CL_SUCCESS != ret) {
		cout << "Error setting kernel argument: " << ret << endl;
		return 0;
	}
	ret = clSetKernelArg(kernel, 3, sizeof(cl_mem), &InY);
	if (CL_SUCCESS != ret) {
		cout << "Error setting kernel argument: " << ret << endl;
		return 0;
	}
	ret = clSetKernelArg(kernel, 4, sizeof(cl_mem), &OutAm);
	if (CL_SUCCESS != ret) {
		cout << "Error setting kernel argument: " << ret << endl;
		return 0;
	}
	ret = clSetKernelArg(kernel, 5, sizeof(cl_mem), &OutPh);
	if (CL_SUCCESS != ret) {
		cout << "Error setting kernel argument: " << ret << endl;
		return 0;
	}
	ret = clSetKernelArg(kernel, 6, sizeof(cl_mem), &OutX);
	if (CL_SUCCESS != ret) {
		cout << "Error setting kernel argument: " << ret << endl;
		return 0;
	}
	ret = clSetKernelArg(kernel, 7, sizeof(cl_mem), &OutY);
	if (CL_SUCCESS != ret) {
		cout << "Error setting kernel argument: " << ret << endl;
		return 0;
	}
	ret = clSetKernelArg(kernel, 8, sizeof(cl_mem), &Distance);
	if (CL_SUCCESS != ret) {
		cout << "Error setting kernel argument: " << ret << endl;
		return 0;
	}
	ret = clSetKernelArg(kernel, 9, sizeof(cl_mem), &Size);
	if (CL_SUCCESS != ret) {
		cout << "Error setting kernel argument: " << ret << endl;
		return 0;
	}
	ret = clSetKernelArg(kernel, 10, sizeof(cl_mem), &Dim);
	if (CL_SUCCESS != ret) {
		cout << "Error setting kernel argument: " << ret << endl;
		return 0;
	}
	//** step 9: set work group size 
	cl_uint work_dim = 3; // in most opencl device, max dimition is 3 
	size_t global_work_size[] = { OUTPUT_SIZE, 1, 1 };
	size_t *local_work_size = NULL; // let opencl device determine how to break work items into work groups
	// step 10: run kernel 
	ret = clEnqueueNDRangeKernel(command_queue, kernel, work_dim, NULL, global_work_size, local_work_size, 0, NULL, NULL);
	if (CL_SUCCESS != ret) {
		cout << "Error enqueue NDRange: " << ret << endl;
		return 0;
	}
	//** step 11: get result 
	float *OutputAmplitude = (float *)clEnqueueMapBuffer(command_queue, OutAm, CL_TRUE, CL_MAP_READ | CL_MAP_WRITE, 0, OUTPUT_BUF_SIZE, 0, NULL, NULL, &ret);
	if ((CL_SUCCESS != ret) || (NULL == OutputAmplitude)) {
		cout << "Error map buffer: " << ret << endl;
		return 0;
	}
	float *OutputPhase = (float *)clEnqueueMapBuffer(command_queue, OutPh, CL_TRUE, CL_MAP_READ | CL_MAP_WRITE, 0, OUTPUT_BUF_SIZE, 0, NULL, NULL, &ret);
	if ((CL_SUCCESS != ret) || (NULL == OutputPhase)) {
		cout << "Error map buffer: " << ret << endl;
		return 0;
	}
	// check result 
	int number = output->getInfo()->getNumber();
	int k = 0;
	for(int i=0;i<number;i++)
		for (int j = 0; j < number; j++) {
			if (output->isAvailable(i, j)) {
				out_intensity->at(k)->setAmplitude(OutputAmplitude[i]);
				out_intensity->at(k)->setPhase(OutputPhase[i]);
			}
			else {
				
			}
		}
	for(int i=0;i<OUTPUT_SIZE;i++){
		out_intensity->at(i)->setAmplitude(OutputAmplitude[i]);
		out_intensity->at(i)->setPhase(OutputPhase[i]);
	}
	//** step 12: release all resources 
	if (NULL != kernel) clReleaseKernel(kernel);
	if (NULL != program) clReleaseProgram(program);
	if (NULL != InAm) clReleaseMemObject(InAm);
	if (NULL != InPh) clReleaseMemObject(InPh);
	if (NULL != InX) clReleaseMemObject(InX);
	if (NULL != InY) clReleaseMemObject(InY);
	if (NULL != OutAm) clReleaseMemObject(OutAm);
	if (NULL != OutPh) clReleaseMemObject(OutPh);
	if (NULL != OutX) clReleaseMemObject(OutX);
	if (NULL != OutY) clReleaseMemObject(OutY);
	if (NULL != Distance) clReleaseMemObject(Distance);
	if (NULL != Size) clReleaseMemObject(Size);
	if (NULL != Dim) clReleaseMemObject(Dim);
	if (NULL != command_queue) clReleaseCommandQueue(command_queue);
	if (NULL != context) clReleaseContext(context);
	a.clear();
	b.clear();
	c.clear();
	d.clear();
	e.clear();
	f.clear();
	g.clear();
	h.clear();
	
	cout << "OpenCL Huygens Diffraction finished" << endl;
	return -1;
}

int fresnelCalculate(Field *input, Field *output, float distance) {

	vector<Cell*> *in_intensity = input->getIntensity();
	vector<Cell*> *out_intensity = output->getIntensity();
	float dim = input->getInfo()->getDim();

	int INPUT_SIZE = in_intensity->size();
	const int INPUT_BUF_SIZE = INPUT_SIZE * sizeof(float);
	const int OUTPUT_SIZE = out_intensity->size();
	const int OUTPUT_BUF_SIZE = OUTPUT_SIZE * sizeof(float);

	vector<float> a(INPUT_BUF_SIZE);
	vector<float> b(INPUT_BUF_SIZE);
	vector<float> c(INPUT_BUF_SIZE);
	vector<float> d(INPUT_BUF_SIZE);
	vector<float> e(OUTPUT_BUF_SIZE);
	vector<float> f(OUTPUT_BUF_SIZE);
	vector<float> g(OUTPUT_BUF_SIZE);
	vector<float> h(OUTPUT_BUF_SIZE);
	for (int i = 0; i < INPUT_SIZE; i++) {
		a[i] = in_intensity->at(i)->getAmplitude();
		b[i] = in_intensity->at(i)->getPhase();
		c[i] = in_intensity->at(i)->getX();
		d[i] = in_intensity->at(i)->getY();
	}
	for (int i = 0; i < OUTPUT_SIZE; i++) {
		e[i] = out_intensity->at(i)->getAmplitude();
		f[i] = out_intensity->at(i)->getPhase();
		g[i] = out_intensity->at(i)->getX();
		h[i] = out_intensity->at(i)->getY();
	}
	cl_int ret;
	// step 1: get platform 
	cl_uint num_platforms;
	ret = clGetPlatformIDs(0, NULL, &num_platforms);
	// get platform number 
	if ((CL_SUCCESS != ret) || (num_platforms < 1)) {
		cout << "Error getting platform number: " << ret << endl;
		return 0;
	}
	cl_platform_id platform_id = NULL;
	ret = clGetPlatformIDs(1, &platform_id, NULL);
	// get first platform id 
	if (CL_SUCCESS != ret) {
		cout << "Error getting platform id: " << ret << endl;
		return 0;
	}
	//** step 2: get device 
	cl_uint num_devices;
	clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 0, NULL, &num_devices);
	if ((CL_SUCCESS != ret) || (num_devices < 1)) {
		cout << "Error getting GPU device number: " << ret << endl;
		return 0;
	}
	cl_device_id device_id = NULL;
	clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, NULL);
	if (CL_SUCCESS != ret) {
		cout << "Error getting GPU device id: " << ret << endl;
		return 0;
	}
	//** step 3: create context 
	cl_context_properties props[] = { CL_CONTEXT_PLATFORM, (cl_context_properties)platform_id, 0 };
	cl_context context = NULL;
	context = clCreateContext(props, 1, &device_id, NULL, NULL, &ret);
	if ((CL_SUCCESS != ret) || (NULL == context)) {
		cout << "Error creating context: " << ret << endl;
		return 0;
	}
	//** step 4: create command queue 
	cl_command_queue command_queue = NULL;
	command_queue = clCreateCommandQueue(context, device_id, 0, &ret);
	if ((CL_SUCCESS != ret) || (NULL == command_queue)) {
		cout << "Error creating command queue: " << ret << endl;
		return 0;
	}
	//** step 5: create memory object 
	cl_mem InAm = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, INPUT_BUF_SIZE, &a[0], &ret);
	if ((CL_SUCCESS != ret) || (NULL == InAm)) {
		cout << "Error creating opencl memory object: " << ret << endl;
		return 0;
	}
	cl_mem InPh = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, INPUT_BUF_SIZE, &b[0], &ret);
	if ((CL_SUCCESS != ret) || (NULL == InPh)) {
		cout << "Error creating opencl memory object: " << ret << endl;
		return 0;
	}
	cl_mem InX = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, INPUT_BUF_SIZE, &c[0], &ret);
	if ((CL_SUCCESS != ret) || (NULL == InX)) {
		cout << "Error creating opencl memory object: " << ret << endl;
		return 0;
	}
	cl_mem InY = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, INPUT_BUF_SIZE, &d[0], &ret);
	if ((CL_SUCCESS != ret) || (NULL == InY)) {
		cout << "Error creating opencl memory object: " << ret << endl;
		return 0;
	}
	cl_mem  OutAm = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, OUTPUT_BUF_SIZE, &e[0], &ret);
	if ((CL_SUCCESS != ret) || (NULL == OutAm)) {
		cout << "Error creating opencl memory object: " << ret << endl;
		return 0;
	}
	cl_mem  OutPh = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, OUTPUT_BUF_SIZE, &f[0], &ret);
	if ((CL_SUCCESS != ret) || (NULL == OutPh)) {
		cout << "Error creating opencl memory object: " << ret << endl;
		return 0;
	}
	cl_mem OutX = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, OUTPUT_BUF_SIZE, &g[0], &ret);
	if ((CL_SUCCESS != ret) || (NULL == OutX)) {
		cout << "Error creating opencl memory object: " << ret << endl;
		return 0;
	}
	cl_mem OutY = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, OUTPUT_BUF_SIZE, &h[0], &ret);
	if ((CL_SUCCESS != ret) || (NULL == OutY)) {
		cout << "Error creating opencl memory object: " << ret << endl;
		return 0;
	}
	cl_mem Distance = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, sizeof(float), &distance, &ret);
	if ((CL_SUCCESS != ret) || (NULL == Distance)) {
		cout << "Error creating opencl memory object: " << ret << endl;
		return 0;
	}
	cl_mem Size = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, sizeof(int), &INPUT_SIZE, &ret);
	if ((CL_SUCCESS != ret) || (NULL == Size)) {
		cout << "Error creating opencl memory object: " << ret << endl;
		return 0;
	}
	cl_mem Dim = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, sizeof(float), &dim, &ret);
	if ((CL_SUCCESS != ret) || (NULL == Dim)) {
		cout << "Error creating opencl memory object: " << ret << endl;
		return 0;
	}
	//** step 6: create program 
	ifstream in("fresnel.cl", std::ios_base::binary);
	if (!in.good()) {
		cout << "Error reading kernel file" << endl;
		return 0;
	}
	// get file length
	in.seekg(0, std::ios_base::end);
	size_t length = in.tellg();
	in.seekg(0, std::ios_base::beg);
	// read program source
	std::vector<char> data(length + 1);
	in.read(&data[0], length);
	data[length] = 0;
	// create and build program 
	const char* kernelSource = &data[0];
	cl_program program = NULL;
	// create program 
	program = clCreateProgramWithSource(context, 1, (const char**)&kernelSource, NULL, &ret);
	if ((CL_SUCCESS != ret) || (NULL == program)) {
		cout << "Error creating program: " << ret << endl;
		return 0;
	}
	// build program 
	ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
	if (CL_SUCCESS != ret) {
		cout << "Error building program: " << ret << endl;
		return 0;
	}
	//** step 7: create kernel 
	cl_kernel kernel = NULL;
	kernel = clCreateKernel(program, "test", &ret);
	if ((CL_SUCCESS != ret) || (NULL == kernel)) {
		cout << "Error creating kernel: " << ret << endl;
		return 0;
	}
	//** step 8: set kernel arguments 
	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), &InAm);
	if (CL_SUCCESS != ret) {
		cout << "Error setting kernel argument: " << ret << endl;
		return 0;
	}
	ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), &InPh);
	if (CL_SUCCESS != ret) {
		cout << "Error setting kernel argument: " << ret << endl;
		return 0;
	}
	ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), &InX);
	if (CL_SUCCESS != ret) {
		cout << "Error setting kernel argument: " << ret << endl;
		return 0;
	}
	ret = clSetKernelArg(kernel, 3, sizeof(cl_mem), &InY);
	if (CL_SUCCESS != ret) {
		cout << "Error setting kernel argument: " << ret << endl;
		return 0;
	}
	ret = clSetKernelArg(kernel, 4, sizeof(cl_mem), &OutAm);
	if (CL_SUCCESS != ret) {
		cout << "Error setting kernel argument: " << ret << endl;
		return 0;
	}
	ret = clSetKernelArg(kernel, 5, sizeof(cl_mem), &OutPh);
	if (CL_SUCCESS != ret) {
		cout << "Error setting kernel argument: " << ret << endl;
		return 0;
	}
	ret = clSetKernelArg(kernel, 6, sizeof(cl_mem), &OutX);
	if (CL_SUCCESS != ret) {
		cout << "Error setting kernel argument: " << ret << endl;
		return 0;
	}
	ret = clSetKernelArg(kernel, 7, sizeof(cl_mem), &OutY);
	if (CL_SUCCESS != ret) {
		cout << "Error setting kernel argument: " << ret << endl;
		return 0;
	}
	ret = clSetKernelArg(kernel, 8, sizeof(cl_mem), &Distance);
	if (CL_SUCCESS != ret) {
		cout << "Error setting kernel argument: " << ret << endl;
		return 0;
	}
	ret = clSetKernelArg(kernel, 9, sizeof(cl_mem), &Size);
	if (CL_SUCCESS != ret) {
		cout << "Error setting kernel argument: " << ret << endl;
		return 0;
	}
	ret = clSetKernelArg(kernel, 10, sizeof(cl_mem), &Dim);
	if (CL_SUCCESS != ret) {
		cout << "Error setting kernel argument: " << ret << endl;
		return 0;
	}
	//** step 9: set work group size 
	cl_uint work_dim = 3; // in most opencl device, max dimition is 3 
	size_t global_work_size[] = { OUTPUT_SIZE, 1, 1 };
	size_t *local_work_size = NULL; // let opencl device determine how to break work items into work groups
									// step 10: run kernel 
	ret = clEnqueueNDRangeKernel(command_queue, kernel, work_dim, NULL, global_work_size, local_work_size, 0, NULL, NULL);
	if (CL_SUCCESS != ret) {
		cout << "Error enqueue NDRange: " << ret << endl;
		return 0;
	}
	//** step 11: get result 
	float *OutputAmplitude = (float *)clEnqueueMapBuffer(command_queue, OutAm, CL_TRUE, CL_MAP_READ | CL_MAP_WRITE, 0, OUTPUT_BUF_SIZE, 0, NULL, NULL, &ret);
	if ((CL_SUCCESS != ret) || (NULL == OutputAmplitude)) {
		cout << "Error map buffer: " << ret << endl;
		return 0;
	}
	float *OutputPhase = (float *)clEnqueueMapBuffer(command_queue, OutPh, CL_TRUE, CL_MAP_READ | CL_MAP_WRITE, 0, OUTPUT_BUF_SIZE, 0, NULL, NULL, &ret);
	if ((CL_SUCCESS != ret) || (NULL == OutputPhase)) {
		cout << "Error map buffer: " << ret << endl;
		return 0;
	}
	
	for (int i = 0; i<OUTPUT_SIZE; i++) {
		out_intensity->at(i)->setAmplitude(OutputAmplitude[i]);
		out_intensity->at(i)->setPhase(OutputPhase[i]);
	}
	//** step 12: release all resources 
	if (NULL != kernel) clReleaseKernel(kernel);
	if (NULL != program) clReleaseProgram(program);
	if (NULL != InAm) clReleaseMemObject(InAm);
	if (NULL != InPh) clReleaseMemObject(InPh);
	if (NULL != InX) clReleaseMemObject(InX);
	if (NULL != InY) clReleaseMemObject(InY);
	if (NULL != OutAm) clReleaseMemObject(OutAm);
	if (NULL != OutPh) clReleaseMemObject(OutPh);
	if (NULL != OutX) clReleaseMemObject(OutX);
	if (NULL != OutY) clReleaseMemObject(OutY);
	if (NULL != Distance) clReleaseMemObject(Distance);
	if (NULL != Size) clReleaseMemObject(Size);
	if (NULL != Dim) clReleaseMemObject(Dim);
	if (NULL != command_queue) clReleaseCommandQueue(command_queue);
	if (NULL != context) clReleaseContext(context);
	a.clear();
	b.clear();
	c.clear();
	d.clear();
	e.clear();
	f.clear();
	g.clear();
	h.clear();

	cout << "OpenCL Fresnel Diffraction finished" << endl;
	return -1;
}