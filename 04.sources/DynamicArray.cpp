#include <iostream>
using namespace std;

struct Point
{
	int x;
	int y;

	// Structs can have functions *inside* them! We call them "methods".
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

int count_alloc = 0;

// NOTE: A TEMPLATE!
// Lets us generate structs based on a given type (one or more) which we choose
// on object construction (we call this "template instantiation").
template <typename T> struct DynamicArray
{
private:
	// Usage of T!
	// Gets replaced with the actual type we pass. E.g. DynamicArray<Circle>
	T* array;
	unsigned size;

public:
	/*
	 * Constructor - an automatically called function on object initialization.
	 * @TODO: check what happens if you pass n = 0, i.e. what does `new T[0]`
	 * do. Look it up on the internet and explain what you observed.
	 *
	 * @TODO. Proper initialization of the member fields
	 */
	DynamicArray(unsigned n) : array{new T[n]}, size{n}
	{
		cout << "Dynamic array allocated #" << (count_alloc++) << endl;
	}

	unsigned getSize() { return this->size; }

	T& operator[](unsigned i) { return array[i]; }

	/*
	 * Pushes an element at the end of the array, or rather creates a new array
	 * from the old with one element more at the right end.
	 */
	void push(const T& newElement)
	{
		// 1. Allocate memory for the updated array of size one more than
		// this->array.
		// @TODO think: how to optimize memory allocations when push() is called
		// multiple times on the same instance.
		T* extended = new T[size + 1];

		// 2. Move everything from this->array to `extended`.
		// Elements range from the 0th to (size - 1)th index.
		for (int i = 0; i < size; ++i)
			extended[i] = array[i];

		// 3. Note that we didn't put anything in the last position of
		// `extended` - `extended[size]` (remember, it's size is (size + 1)).
		// We copy the element passed as an argument into the new array.
		extended[size] = newElement;

		// 4. NOTE: The old memory is currently referenced by the current value
		// of this->array. We free the memory for the old array as it's not
		// needed anymore.
		delete[] array;

		size++;

		// 5. Unless we execute the following line, this->array will point to
		// the old memory block which we just released back to the operating
		// system. We finish by reassigning `this->array` to point to the newly
		// allocated `extended`.
		array = extended;

		// @TODO think: what would happen if we swap the last two actions?
	}

	/*
	 * Removes the n'th element, or rather creates a new array from the old with
	 * its n'th element removed.
	 */
	void deleteAt(unsigned n)
	{
		// Allocate memory for the shrunken array
		T* shrunken = new T[size - 1];

		// Copy everything from `array` into `shrunken`, except for the array[n]
		// NOTE: Can be done with 2 `for` loops without conditional blocks.
		for (unsigned i = 0; i < size; ++i) {
			if (i < n) shrunken[i] = array[i];

			if (i > n) shrunken[i - 1] = array[i];
		}

		// NOTE: The old memory is currently referenced by the current value of
		// this->array. We free the memory for the old array as it's not needed
		// anymore.
		delete[] array;

		size--;

		// Unless we execute the following line, this->array will point to the
		// old memory block which we just released back to the operating system.
		// We finish by reassigning `this->array` to point to the newly
		// allocated `extended`.
		array = shrunken;
	}

	/*
	 * Destructor - a special function, automatically called when the object
	 * leaves scope (the set of curly braces it was defiend within).
	 *
	 * An easy way not to forget to free whatever you allocated!
	 */
	~DynamicArray()
	{
		cout << "Before delete" << endl;
		delete[] array;
		cout << "After delete" << endl;
	}
};

struct Window
{
	// @TODO use DynamicArray instead of bare pointers
	DynamicArray<Circle> circles;
	DynamicArray<Rectangle> rectangles;

	void print()
	{
		// @TODO Figure out what to do with 'n' here.

		for (unsigned i = 0; i < circles.getSize(); ++i) {
			circles[i].print();
		}
		for (unsigned i = 0; i < rectangles.getSize(); ++i) {
			rectangles[i].print();
		}
	}
};

int main()
{
	Point p{5, 10};
	// Rectangle r{p, 3, 4};
	Circle c{p, 6};

	// Method calls! Note the special syntax.
	p.print(); // The `this` inside the print() function equals `&p`.
	c.print();

	unsigned n = 2; // Too lazy to input ever time.

	// NOTE: Template instantiation. Generates a struct where every `T` is
	// replaced with `Circle`, and compiles it separately.
	// NOTE: We call the constructor with 'n' as an argument
	DynamicArray<Circle> circles{n};

	cout << circles.getSize();
	circles.push(c);
	cout << circles.getSize();

	DynamicArray<Rectangle> rectangles{n};

	Window w1{circles, rectangles};
	Window w2{circles, rectangles};
	w1.print();
	cout << endl;
	w2.print();

	// NOTE the double delete error! Fix it by implementing a copy constructor
	// for DynamicArray
	return 0;
}
