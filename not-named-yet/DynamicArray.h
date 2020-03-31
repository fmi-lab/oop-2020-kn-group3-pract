#pragma once

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
		//cout << "Dynamic array allocated #" << (count_alloc++) << endl;
	}

	DynamicArray() : array{nullptr}, size{0} {}

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
		//cout << "Before delete" << endl;
		delete[] array;
		//cout << "After delete" << endl;
	}
};
