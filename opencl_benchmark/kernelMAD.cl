__kernel void array_add (__global float* a, __global float* b, __global float* c, __global float* d)
{
	int id = get_global_id(0);
	d[id] = mad(a[id], b[id], c[id]);
}