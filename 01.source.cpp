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
	// Arrays of structs inside struct!
	Circle circles[2];
	Rectangle rectangles[2];

	void print()
	{
		// NOTE: funny syntax.
		// @TODO: Understand how it works by the end of the semester!
		for (Circle& c : circles) {
			c.print();
		}

		for (Rectangle& r : rectangles) {
			r.print();
		}
	}
};

int main()
{
	// Curly braces give initial values to structs.
	Point p{5, 10};
	Rectangle r{p, 3, 4};
	Circle c{p, 6}; // We can also nest curly braces: {6, {5,10}}

	// Method calls! Note the special syntax.
	p.print(); // The `this` inside the print() function equals `&p`.
	c.print();

	// More complex curly braces!
	Window w1{{c, c}, {r, r}};
	Window w2{{c, c}, {r, r}};

	w1.print();
	cout << endl;

	w2.print();
	return 0;
}
