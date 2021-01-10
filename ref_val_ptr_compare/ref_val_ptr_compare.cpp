#include <iostream>
#include <chrono>
#include <memory>
#include <vector>
#include <cassert>

using namespace std;
using namespace std::chrono;

high_resolution_clock::time_point get_time()
{
	return high_resolution_clock::now();
}

template <typename T>
void print_vector (vector<T>& _vec, const char* name)
{
	typename vector<T>::iterator it;

	cout << "Contents of vector " << name << " is:" << endl;

	// initialize 0,1,2,3...(size-1) in the vector
	for (it = _vec.begin(); it < _vec.end(); it++) cout << *it << ' ';

	cout << endl;
}

template <typename T>
void init_vector (vector<T>& _init, const int size, T start, T incr, int zero)
{
	// initialize 0,1,2,3...(size-1) in the vector
	for (int i = 0; i < size; i++) _init.push_back(start + zero*incr);
}

template <typename T> 
void add_vector_by_ref (vector<T>& _a, vector<T>& _b, vector<T>& _c)
{
	int i = 0;

	while(i < _c.size())
	{
		_c.at(i) = _a.at(i) * _b.at(i);
		i++;
	}
}

template <typename T> 
vector<T> add_vector_by_val (vector<T> _a, vector<T> _b)
{
	int i = 0;
	vector<T> _c;

	while(i < _c.size())
	{
		_c.at(i) = _a.at(i) * _b.at(i);
		i++;
	}

	return _c;
}

template <typename T> 
void add_vector_by_ptr (shared_ptr<vector<T>> _a, shared_ptr<vector<T>> _b, shared_ptr<vector<T>> _c)
{
	int i = 0;

	while(i < _c->size())
	{
		_c->at(i) = _a->at(i) * _b->at(i);
		i++;
	}
}

int main (int argc, char* argv[])
{
	typedef double vec_type;

	vector<vec_type> _vector_a;
	vector<vec_type> _vector_b;
	vector<vec_type> _vector_c_by_ref;
	vector<vec_type> _vector_c_by_val;
	vector<vec_type> _vector_c_by_ptr;

	const int vector_size = 80000000;

	init_vector(_vector_a, vector_size, (vec_type) 0.125, (vec_type) 0.125, 1);
	init_vector(_vector_b, vector_size, (vec_type) 5.125, (vec_type) 0.125, 1);
	init_vector(_vector_c_by_ref, vector_size, (vec_type) 0, (vec_type) 0, 0);
	init_vector(_vector_c_by_val, vector_size, (vec_type) 0, (vec_type) 0, 0);
	init_vector(_vector_c_by_ptr, vector_size, (vec_type) 0, (vec_type) 0, 0);

	shared_ptr<vector<vec_type>> _ptr_a = make_shared<vector<vec_type>>(_vector_a);
	shared_ptr<vector<vec_type>> _ptr_b = make_shared<vector<vec_type>>(_vector_b);
	shared_ptr<vector<vec_type>> _ptr_c = make_shared<vector<vec_type>>(_vector_c_by_ptr);

	auto begin = get_time();
	
	add_vector_by_ref(_vector_a, _vector_b, _vector_c_by_ref);

	auto ref = get_time();

	_vector_c_by_val = add_vector_by_val(_vector_a, _vector_b);

	auto val = get_time();

	add_vector_by_ptr(_ptr_a, _ptr_b, _ptr_c);

	auto end = get_time();

	duration<double, milli> _dur_by_ref = ref - begin;
	duration<double, milli> _dur_by_val = val - ref;
	duration<double, milli> _dur_by_ptr = end - val;

	cout << "Time taken for pass by reference = " << _dur_by_ref.count() << "ms" << endl;
	cout << "Time taken for pass by value = " << _dur_by_val.count() << "ms" << endl;
	cout << "Time taken for pass by pointer = " << _dur_by_ptr.count() << "ms" << endl;

	for (vec_type i = 0; i < _vector_c_by_val.size(); i++)
	{
		assert(_vector_c_by_val.at(i) == _vector_a.at(i) * _vector_b.at(i));
		assert(_vector_c_by_ref.at(i) == _vector_a.at(i) * _vector_b.at(i));
		assert(_vector_c_by_ptr.at(i) == _vector_a.at(i) * _vector_b.at(i));
	}

	return 0;
}