#include <iostream>
using namespace std;

struct Point
{
	int x;
	int y;

	// Structs can have functions *inside* them! We call them "methods".
	// Look in the main function for example uses.
	void print()
	{
		// @Reminder: the type of `this` is `Point* const`. In other words, an
		// immutable (const) pointer to a mutable (non-const) Point instance.

		// @Reminder: all variables (member fields) of the object on which this
		// method is called (point.print()) are available like local variables
		// here.
		// Another way to write `x` is `this->x` or `(*this).x`.
		cout << "(" << x << ";" << y << ")";
	}
};

struct Circle
{
	// struct inside struct - no problem!
	// Benefits: we can reuse Point's functionality. E.g. we can call
	// `p.print()` instead of writing manually `cout << x << ' ' << y;`.
	Point p;
	double r;

	void print()
	{
		cout << r << " ";
		p.print();
		cout << endl;
	}
};

struct Rectangle
{
	Point p;
	double width;
	double height;

	void print()
	{
		cout << width << " " << height << " ";
		p.print();
		cout << endl;
	}
};

struct Window
{
	Circle* circles;
	Rectangle* rectangles;

	void print()
	{
		// @TODO Doesn't work! You'll see why. For now, just fix it.

		// for(Circle & c: circles) {
		//     c.print();
		// }
		// for(Rectangle & r: rectangles) {
		//     r.print();
		// }
	}
};

int count_alloc = 0;

struct DynamicArray
{
	Circle* circles;

	/*
	 * Constructor - a special function, automatically called on object
	 * initialization.
	 * @TODO: check what happens if you pass n = 0, i.e. what does `new T[0]`
	 * do. Look it up on the internet and explain what you observed.
	 *
	 * @TODO. Proper initialization of the member fields
	 */
	DynamicArray(unsigned n)
	{
		circles = new Circle[n];
		cout << "Dynamic array allocated #" << (count_alloc++) << endl;
	}

	/*
	 * Destructor - a special function, automatically called when the object
	 * leaves scope (the set of curly braces it was defiend within).
	 *
	 * An easy way not to forget to free whatever you allocated!
	 */
	~DynamicArray()
	{
		delete[] circles;
		cout << "~DynamicArray(): memory cleared." << endl;
	}
};

int main()
{
	Point p{5, 10};
	Rectangle r{p, 3, 4};
	Circle c{p, 6}; // We can also nest curly braces: {6, {5,10}}

	// Method calls! Note the special syntax.
	p.print(); // The `this` inside the print() function equals `&p`.
	c.print();

	unsigned n;
	cin >> n;

	// NOTE: We call the constructor here with an argument 'n':
	DynamicArray circles{n};

	// Compare the old initialization below ↓ with the new above ↑. Admit it
	// looks better. And remember it also lets us (almost) forget about delete.
	Rectangle* rectangles = new Rectangle[n];
	delete rectangles;

	// Window w1{circles, rectangles};
	// Window w2{circles, rectangles};
	// w1.print();
	// cout << endl;
	// w2.print();
	return 0;
}
