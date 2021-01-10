__kernel void array_add (__global float* a, __global float* b, __global float* c)
{
	int id = get_global_id(0);
	c[id] = a[id] * b[id];
}