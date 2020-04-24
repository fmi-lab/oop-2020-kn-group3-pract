#include <cmath>
#include <iostream>

const float E = 2.7;

// Global @TODO: Add const wherever applicable

// @TODO Convert to struct
float exponent(float x) { return std::pow(E, x); }

struct Set
{
	bool (*check)(float);
};

// @TODO Define the sets of positive, nonnegative numbers and sets of intervals
// of the form (a, b)

struct RealFunc
{
	// @TODO Declare `virtual RealFunc differentiate() = 0;` and implement in
	// all subclasses
	virtual float evalAt(float) = 0;

	// @NOTE for me about next time:
	// * make `evalAt()` protected
	// * add a protected `Set domain` member
	// * create a public non-virtual `float operator()(float)` that:
	// 		* checks if x \in domain
	// 		* throws an error if not
	// 		* calls the virtual evalAt(x) otherwise
};

struct Log : RealFunc
{
	float evalAt(float x) override
	{
		if (x <= 0) {
			std::cerr << "Log expects positive numbers!\n"
			          << x << " was given.";
			throw;
		}
		return std::log(x);
	}

	// @TODO Define a const member `name`; initialize properly in all subclasses

	// @TODO Add a non-virtual (why?) method `string print(string)` which
	// returns the name of the functions and the passed string in braces.
	// An exmaple print result could be `sin(log(x))` or `e^(x)`
};

struct BoyanFunction : RealFunc
{
	int n;

	float evalAt(float x) override
	{
		n++;
		if (n >= 10) return 0;
		return x;
	}
};

struct Compose : RealFunc
{
	RealFunc *f, *g;
	// Set domain;

	Compose(RealFunc* f, RealFunc* g) : f(f), g(g) {}

	float evalAt(float x) override
	{
		// @TODO check if in domain: if (g->domain(x) && f->domain(g(x)))
		return f->evalAt(g->evalAt(x));
	}
};

struct Sin : RealFunc
{
	float evalAt(float x) override { return std::sin(x); }
};

// @TODO Define `struct Tan` and carefully construct its domain Set.

// @TODO Define `struct Pow` which raises `x` to a power, specified as a
// constructor argument.

// @TODO Define `struct Sum` which represents the sum of all functions in a
// given array.

// @TODO Define global operators `*`, `+`, and `>>` which respectively act as
// multiplication, addition and composition of functions.

int main()
{
	Log log;
	Sin sin;
	Compose f{&sin, &log};
	Compose g{&log, &f};

	std::cout << g.evalAt(4) << std::endl;
}
