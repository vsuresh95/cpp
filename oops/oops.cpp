#include <iostream>
#include <string>

using namespace std;

class university
{
	protected:
		string name;
		int ID;

	public:
		// constructor
		// when you have a const in the constructor, it is required
		// to pass an initialization list where these const values
		// are assigned something when the class is created. Similarly,
		// when you create a const variable in a normal setting, you always 
		// initialize it while declaring it.
		university(const string &myName, const int &myID) : name(myName), ID(myID)
		{
			cout << "Inside copy constructor for university, Name = " <<
				name << ", ID = " << ID << endl;
		};

		// class functions always have an implicit <class name>* this i.e.,
		// class C { void f(int x);
		// actually looks like... 
		// class C { void f(C* this, int x);
		// the const makes the implicit this a const variable so that the class
		// object cannot be modified.
		// 
		// There are ways to workaround this: such as by passing another object of
		// the same class by reference for which the const will not apply.
		// class C { void f(C& obj);
		// actually looks like... 
		// class C { void f(C* this, C& obj);
		const string getName() const { return name; }

		const int getID() const	{ return ID; }

		// Polymorphism
		// virtual indicates that this function can be overriden by a child class
		// 
		// incase you define as...
		// virtual string getDescription() const = 0;
		// ... then this function must always have an equivalent function in the child
		// class and the one in the parent class is like a placeholder
		virtual void getDescription() const
		{
			cout << "Inside getDescription for university, Name = " <<
				name << ", ID = " << ID << endl;
		}

	protected:
		void getUniversitySecret() const
		{
			cout << "University secret is xyz 123" << endl;
		}
};

class department : public university
{
	protected:
		int depID;

	public:
		department(const string& myName, const int myID, const int myDepID) : university(myName, myID), depID(myDepID)
		{
			cout << "Inside constructor for department, Name = " <<
				name << ", ID = " << ID << ", department ID = " << depID << endl;

			getUniversitySecret();
		}

		void getDescription() const
		{
			cout << "Inside getDescription for department, Name = " <<
				name << ", ID = " << ID << ", department ID = " << depID << endl;
		}

	protected:
		void getDepartmentSecret()
		{
			cout << "Department secret is abc 456" << endl;
		}
};

class student : public department
{
	public:
		int studentID;

		student(const string& myName, const int myStudentID) : department(myName, myStudentID, myStudentID), studentID(myStudentID)
		{
			cout << "Inside constructor for student, ID = " << studentID << endl;
			
			// the 2 functions getDepartmentSecret() and getUniversitySecret() are 
			// not accessible through an object of the student class because they
			// are protected functions and can be accessed from within the student
			// class only.
			getDepartmentSecret();

			getUniversitySecret();
		}
};

// we pass argc and argv[] in case we want to pass
// command line options directly to the c++ code
int main (int argc, char* argv[])
{
	const char* hello_world = "Hello World!";
	std::cout << hello_world << std::endl;

	// we create a student object and a department object with
	// some initialized values
	student s1("Vignesh", 1234);

	department d1("ECE", 5678, 1010);

	// here, s1.getID/s1.getName do not work. This could be because
	// the university is not the direct base class to the student class.
	cout << "ID for department from base class is " << d1.getID() << endl;

	cout << "Name for department from base class is " << d1.getName() << endl;

	// this should access the overriden function from the department class
	// rather than the university class
	d1.getDescription();

	return 0;
}
