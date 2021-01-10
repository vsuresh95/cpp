#include <iostream>
#include <vector>
#include <memory> // for smart pointers
#include <algorithm> // for copy
#include <iterator> // for ostream iterator to print out vector

using namespace std;

// unique pointer is used for exclusive ownership resource 
// management, in other words, only one address in the stack can
// point to the resource (on the heap) at a time.
// unique pointers can be created, moved and (implicitly at the end of scope)
// destroyed. They cannot be copied and do not have a copy constructor.
// unique_ptr<class/type> name (class/type, deleter t)
// By default, deleter will 'delete' the allocated memroy and 'null' the 
// pointer. However, if you have a pointer to a file or other resource that
// cannot be 'deleted', you can define your own deleter. This deleter would accept
// the pointer to this resource as a parameter.
// 
// shared pointer is used for shared ownership resource management.
// there is a nice analogy about light being a resource in a hall that
// everybody inside has 'access' to. Whoever goes out of the hall last
// switches it off. When you enter you add a token (and increment a refcount),
// and when you leave you take a token. Similarly the last address that points
// to a resource must delete the resource. A shared pointer, apart from
// pointing to the controlled object (the resource), also has a pointer to a
// control block containing the refcount, a pointer to the resource itself,
// any custom deleter and a weak pointer. The extra pointer to the resource
// is needed in case you nest objects and the offset might be lost.
// More details here: https://youtu.be/xGDLkt-jBJ4?t=1451
// 
// weak pointer is like a shared pointer but it can be 'dangling', in that it
// can point to deallocated memory and still have a control block. A dangling
// weak pointer is not useful and cannot be deferenced, but it can be converted
// to a shared pointer. Weak pointers can be useful in a circular reference: 
// such as in a graph, where one node points to another using shared pointers,
// and so on and eventually you might end up with a loop. If a loop occurs, none
// of the shared pointers can deallocate themselves because they are, by recursion,
// stalling their own lifetimes. In such a case, weak pointers can be helpful
// in that they can be deallocated while still existing.

// a templatized function to print any vector, parameter includes its name
template <typename U>
void print_vector(vector<U>& _vec, const char* _vec_name)
{
	cout << "******* print_vector ********" << endl;
	cout << "The size of " << _vec_name << " is " << _vec.size() << endl;
	cout << "The contents of " << _vec_name << " are:" << endl;
	copy(_vec.begin(), _vec.end(), ostream_iterator<U> (cout, " "));
	cout << endl;
	cout << "*****************************" << endl;
}

// create a class for the workload that can be used to convolve/multiply with
// the input activations.
template <typename T>
class workload
{
	public:
		int stride;
		int size;
		// if you want declare an iterator to vector<T> here, you need to 'typename'
		// the iterator as the compiler would not have the definition for T should be.
		// For example...
		// typename vector<T>::iterator it = tensor.begin();
		vector<T> tensor;

		// constructor accepts the members of the class as parameters
		// and assigns them in an initialization list
		workload (int _stride, int _size, vector<T>& _tensor) : 
			stride (_stride), size (_size), tensor (_tensor) {}

		void print_workload (const char* name)
		{
			cout << "The stride of workload " << name << " is " << stride << endl;
			print_vector(tensor, "tensor");
		}

		// multiplies the tensor with another vector
		// returns a shared pointer to the calling function
		shared_ptr<vector<T>> multiply (vector<T>& activations)
		{
			shared_ptr<vector<T>> result = make_shared<vector<T>>();
			int index = 0;

			for (auto a : activations)
			{
				result->push_back(a * tensor.at(index++));
			}
		
			cout << "result.unique() = " << result.unique() << endl;
			cout << "result.use_count() = " << result.use_count() << endl;
			cout << "result.owner_get() = " << result.get() << endl;

			return result;
		}
};

int main (int argc, char* argv[])
{
	vector<double> activations;
	vector<double> weights; 

	for (double i = 1, j = 10; i < 10, j > 1; i += 1.1, j -= 1.1)
	{
		activations.push_back(i);
		weights.push_back(j);
	}

	print_vector(activations, "activations");

	// not using new is a clean way to create smart pointers
	unique_ptr<workload<double>> w1 = make_unique<workload<double>> (3, 9, weights);
	w1->print_workload("w1");

	shared_ptr<vector<double>> result = w1->multiply(activations);

	// need to deference the unique pointer before printing the vector here
	print_vector(*result, "result");

	cout << "result.unique() = " << result.unique() << endl;
	cout << "result.use_count() = " << result.use_count() << endl;
	cout << "result.owner_get() = " << result.get() << endl;

	return 0;
}
