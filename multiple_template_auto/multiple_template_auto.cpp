#include <iostream>
#include <cctype>
#include <typeinfo>
#include <vector>

using namespace std;

template <typename T>
class container
{
	public:
		T element;

		container (T elem) : element(elem) {}

		void increment () { element++; }

		void print_elem (const char* name) { cout << "Value in " << name << " is " << element << endl; }

		void print_type (const char* name)
		{
			cout << "Type of element in " << name << " is " << typeid(element).name() << endl;
		}
};

template <>
class container <char>
{
	public:
		char element;

		container (char elem) : element(elem) {}

		void upper_case () { element = toupper(element); }

		void lower_case () { element = tolower(element); }

		void print_elem (const char* name) { cout << "Value in " << name << " is " << element << endl; }
};

template <>
class container <vector<int>>
{
	public:
		vector<int> vec_element;
		int element;

		container (int elem) : element(elem) 
		{
			vec_element.push_back(element);
		}
};

int main (int argc, char* argv[])
{
	// 1. trying an integer override for template
	container<int> c1(4.5);

	c1.increment();

	cout << "Type of c1 is " << typeid(c1).name() << endl;

	c1.print_elem("c1");

	c1.print_type("c1");

	// 2. trying a character override using the special template class
	container<char> c2('a');

	c2.upper_case();

	c2.print_elem("c2");

	c2.lower_case();

	c2.print_elem("c2");

	// 3. trying the normal override for a complex data structure, like an int vector here
	container<vector<int>> c_vec(5);

	cout << "Type of c_vec is " << typeid(c_vec).name() << endl;

	// 4. assign an auto variable to the begin iterator of the member vector
	// of the container, and then print out the type of that iterator
	// 
	// begin returns an iterator that points to the starting of the vector
	// 
	// iterators are part of the std::vector class of the standard library, and
	// you can declare iterators if you want iterate through the elements of a vector.
	// Further, these iterators are pointers to the elements of a vector, therefore,
	// an iterator = vector.begin() is a pointer to the first element.
	auto iter = c_vec.vec_element.begin();

	cout << "Type of iterator is " << typeid(iter).name() << endl;
	
	return 0;
}
