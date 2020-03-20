#include <iostream>
using namespace std;

template <typename T> class Matrix
{
private: // Note: private by default, no need to specify.
	T** data;
	unsigned cols, rows;

public:
	Matrix(unsigned rows, unsigned cols)
	    // Init member fields. Note that cols{cols} works as expected - it
	    // assigns the argument `cols` to `this->cols`.
	    : data{new T*[rows]}, cols{cols}, rows{rows}
	{
		for (unsigned i = 0; i < rows; ++i) {
			data[i] = new T[cols];
		}
	}

	unsigned getRows() { return rows; }
	unsigned getCols() { return cols; }

	// NOTE: Return by reference!
	T& get(int row, int col) { return data[row][col]; }

	void set(int row, int col, T const& newValue) { data[row][col] = newValue; }

	~Matrix()
	{
		for (unsigned i = 0; i < rows; ++i) {
			delete[] data[i];
		}
		delete[] data;
	}
};

int main()
{
	Matrix<int> matrix{5, 5};

	matrix.set(2, 3, 10);
	matrix.get(2, 2) = 10; // .get() returned by reference, so we can assign!

	cout << matrix.get(3, 3) << "; " << matrix.get(2, 2) << endl;
}
