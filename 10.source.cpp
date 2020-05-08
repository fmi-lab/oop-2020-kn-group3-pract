#include <cmath>
#include <iostream>

const float E = 2.7f;

struct Set
{
	virtual bool check(float) const = 0;
	virtual Set* clone() const = 0;
	virtual ~Set() {}
};

struct Intersect : Set
{
	const Set *a, *b;

	Intersect(const Set* a, const Set* b) : a(a), b(b) {}

	bool check(float x) const override { return a->check(x) && b->check(x); }

	Intersect* clone() const override { return new Intersect(*this); }

	virtual ~Intersect() {}
};

struct AllReals : Set
{
	bool check(float) const override { return true; }

	AllReals* clone() const override { return new AllReals(); }
};

struct Positives : Set
{
	bool check(float x) const override { return x > 0; }

	Positives* clone() const override { return new Positives(); }
};

// @TODO Define the sets of positive, nonnegative numbers and sets of intervals
// of the form (a, b)

class RealFunc
{
protected:
	std::string name;

	virtual float evalAt(float) = 0;

public:
	const Set* const domain;

	RealFunc(Set const& domain, std::string name)
	    : name(name), domain(domain.clone())
	{
	}

	// staticly bound
	float safeEval(float x)
	{
		if (!domain->check(x)) {
			std::cerr << "Invalid argument " << x << " was given to " << name
			          << "." << std::endl;
			throw;
		}

		// dynamic dispatch
		return evalAt(x);
	}

	virtual ~RealFunc() { delete domain; };
};

struct Exponent : RealFunc
{
	Exponent() : RealFunc{AllReals{}, "exp"} {}

	virtual float evalAt(float x) override { return std::pow(E, x); }
};

struct Log : RealFunc
{
	Log() : RealFunc{Positives{}, "log"} {}

	float evalAt(float x) override { return std::log(x); }
};

struct BoyanFunction : RealFunc
{
	int n;
	int* myBigArr = new int[100]; // Illustration of the need of virtual d-tors

	float evalAt(float x) override
	{
		n++;
		if (n >= 10) return 0;
		return x;
	}

	// Unless virtual, will not be called when delete-ing through a RealFunc ptr
	virtual ~BoyanFunction() override { delete[] myBigArr; }
};

struct Compose : RealFunc
{
	RealFunc *f, *g;

	Compose(RealFunc* f, RealFunc* g)
	    : RealFunc{*g->domain, "Compose"}, f(f), g(g)
	{
	}

	float evalAt(float x) override
	{
		return f->safeEval(g->safeEval(x));
	}
};

struct Sin : RealFunc
{
	Sin() : RealFunc{AllReals{}, "sin"} {}

	float evalAt(float x) override { return std::sin(x); }
};

// @TODO Define `struct Tan` and carefully construct its domain Set.

// @TODO Define `struct Pow` which raises `x` to a power, specified as a
// constructor argument.

// @TODO Define `struct Sum` which represents the sum of all functions in a
// given array.

// @TODO Define global operators `*`, `+`, and `>>` which respectively act as
// multiplication, addition and composition of functions.

struct Sum : public RealFunc
{
	RealFunc *f, *g;

	Sum(RealFunc* f, RealFunc* g)
	    : RealFunc{Intersect{f->domain, g->domain}, "Sum"}, f(f), g(g)
	{
	}

	float evalAt(float x) override { return f->safeEval(x) + g->safeEval(x); }
};

Sum operator+(RealFunc& a, RealFunc& b) { return {&a, &b}; }

int main()
{
	Log log;
	Sin sin;

	Sum sum = log + sin;

	Exponent exp;
	Compose comp{&exp, &sum};

	RealFunc* base = &comp;
	std::cout << base->safeEval(3) << std::endl;
}
