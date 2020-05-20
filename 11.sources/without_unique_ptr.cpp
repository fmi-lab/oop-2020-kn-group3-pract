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
	// owner of a b
	const Set *a, *b;

	Intersect(const Set* a, const Set* b) : a(a->clone()), b(b->clone()) {}
	Intersect(Intersect const& other) : a(other.a->clone()), b(other.b->clone())
	{
	}

	bool check(float x) const override { return a->check(x) && b->check(x); }

	Intersect* clone() const override { return new Intersect(*this); }

	virtual ~Intersect() { delete a; delete b; }
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

	const Set* const domain;

public:
	const Set& getDomain() const { return *domain; }

	RealFunc(Set const& domain, std::string name)
	    : name(name), domain(domain.clone())
	{
	}

	virtual RealFunc* clone() const = 0;

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

	virtual RealFunc* clone() const override { return new Exponent(*this); }
	virtual float evalAt(float x) override { return std::pow(E, x); }
};

struct Log : RealFunc
{
	Log() : RealFunc{Positives{}, "log"} {}

	virtual RealFunc* clone() const override { return new Log(*this); }
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
	    : RealFunc{g->getDomain(), "Compose"}, f(f), g(g)
	{
	}

	virtual RealFunc* clone() const override { return new Compose(*this); }

	float evalAt(float x) override { return f->safeEval(g->safeEval(x)); }
};

struct Sin : RealFunc
{
	Sin() : RealFunc{AllReals{}, "sin"} {}

	virtual RealFunc* clone() const override { return new Sin(*this); }
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
private:
	RealFunc *f, *g;

public:
	Sum(RealFunc* f, RealFunc* g)
	    : RealFunc{Intersect{&f->getDomain(), &g->getDomain()},
	               "Sum"},
	      f(f->clone()), g(g->clone())
	{
	}

	float evalAt(float x) override { return f->safeEval(x) + g->safeEval(x); }

	virtual RealFunc* clone() const override { return new Sum(*this); }

	~Sum() { delete f; delete g; }
};

Sum operator+(RealFunc& a, RealFunc& b) { return {&a, &b}; }

Sum complexFunction()
{
	// dangling pointer
	Log xlog;
	Sin xsin;

	// Copy elision
	return xlog + xsin;
}

int main()
{
	Sum sum = complexFunction();

	Exponent exp;
	Compose comp{&exp, &sum};

	RealFunc* base = &comp;
	std::cout << " dsadsa " << std::endl;
	std::cout << base->safeEval(3) << std::endl;

	Intersect str{&sum.getDomain(), &base->getDomain()};
}
