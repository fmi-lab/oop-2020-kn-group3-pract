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

struct NonNegatives : Set
{
	bool check(float x) const override { return x >= 0; }

	NonNegatives* clone() const override { return new NonNegatives(); }
};

struct Interval : Set
{
	const float a, b;

	Interval(const float a, const float b) : a(a), b(b) {}

	bool check(float x) const override { return a < x && b > x; }

	Interval* clone() const override { return new Interval(*this); }
};

struct TanDomain : Set
{
	bool check(float x) const override { return ceilf(x / (M_PI / 2)) != floorf(x / (M_PI / 2)) ||  (int)(x / (M_PI / 2)) % 2 == 0; }

	TanDomain* clone() const override { return new TanDomain(); }
};

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

protected:
	virtual float evalAt(float x) override { return std::pow(E, x); }
};

struct Log : RealFunc
{
	Log() : RealFunc{Positives{}, "log"} {}

protected:
	float evalAt(float x) override { return std::log(x); }
};

struct BoyanFunction : RealFunc
{
	int n;
	int* myBigArr = new int[100]; // Illustration of the need of virtual d-tors

protected:
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

protected:
	float evalAt(float x) override
	{
		return f->safeEval(g->safeEval(x));
	}
};

struct Sin : RealFunc
{
	Sin() : RealFunc{AllReals{}, "sin"} {}

protected:
	float evalAt(float x) override { return std::sin(x); }
};

struct Tan : RealFunc
{
	Tan() : RealFunc{TanDomain{}, "tan"} {}

protected:
	float evalAt(float x) override { return std::tan(x); }
};

struct Pow : RealFunc
{
	const float exp;

	Pow(const float exp) : RealFunc{Positives{}, "pow"}, exp(exp) {}

protected:
	float evalAt(float x) override { return std::pow(x, exp); }
};

struct Sum : public RealFunc
{
	RealFunc *f, *g;

	Sum(RealFunc* f, RealFunc* g)
	    : RealFunc{Intersect{f->domain, g->domain}, "Sum"}, f(f), g(g)
	{
	}

protected:
	float evalAt(float x) override { return f->safeEval(x) + g->safeEval(x); }
};

struct Product : public RealFunc
{
	RealFunc *f, *g;

	Product(RealFunc* f, RealFunc* g)
	    : RealFunc{Intersect{f->domain, g->domain}, "Product"}, f(f), g(g)
	{
	}

protected:
	float evalAt(float x) override { return f->safeEval(x) * g->safeEval(x); }
};

Product operator*(RealFunc& a, RealFunc& b) { return {&a, &b}; }

Sum operator+(RealFunc& a, RealFunc& b) { return {&a, &b}; }

Compose operator>>(RealFunc& a, RealFunc& b) { return {&a, &b}; }

int main()
{
	Log log;
	Sin sin;

	Sum sum = log + sin;

	Exponent exp;
	Compose comp{&exp, &sum};

	RealFunc* base = &comp;
	std::cout << base->safeEval(3) << std::endl;

	// problem with Compose's defined domain - example
	Compose comp2{&log, &sin};
	base = &comp2;
	std::cout << base->safeEval(3 * M_PI / 2) << std::endl;
}