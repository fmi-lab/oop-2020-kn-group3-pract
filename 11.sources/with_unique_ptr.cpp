#include <cmath>
#include <iostream>
#include <string>

const float E = 2.7f;
const float eps = 0.001f;

struct Set
{
	virtual bool check(float) const = 0;
	virtual Set* clone() const = 0;
	virtual ~Set() {}
};

template<typename T>
struct UniquePointer {
	T* ptr;

	UniquePointer(T const& other) : ptr(other.clone()) {}

	UniquePointer(UniquePointer const& other) : ptr(other.ptr->clone()) {}

	virtual ~UniquePointer() {
		delete ptr;
	}
};

struct Intersect : Set
{
private:
	// owner of a b
	const UniquePointer<Set> a, b;

public:
	Intersect(Set const& a, Set const& b) : a( a ), b(b) {}

	bool check(float x) const override { return a.ptr->check(x) && b.ptr->check(x); }

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
private:
	const float a, b;

public:
	Interval(const float a, const float b) : a(a), b(b) {}

	bool check(float x) const override { return a < x && b > x; }

	Interval* clone() const override { return new Interval(*this); }
};

struct TanDomain : Set
{
	bool check(float x) const override
	{
		return fmod(x - M_PI / 2, M_PI) > eps || fmod(x - M_PI / 2, M_PI) < -eps;
	}

	TanDomain* clone() const override { return new TanDomain(); }
};

class RealFunc
{
protected:
	std::string name;

	virtual float evalAt(float) = 0;

	const UniquePointer<Set> domain;

public:
	const Set& getDomain() const { return *domain.ptr; }

	RealFunc(UniquePointer<Set> const& domain, std::string name)
	    : name(name), domain(domain)
	{
	}

	virtual RealFunc* clone() const = 0;

	// staticly bound
	float safeEval(float x)
	{
		if (!domain.ptr->check(x)) {
			std::cerr << "Invalid argument " << x << " was given to " << name
			          << "." << std::endl;
			throw;
		}

		// dynamic dispatch
		return evalAt(x);
	}

	virtual ~RealFunc() {};
};

struct ComposeSet : Set
{
private:
	const UniquePointer<RealFunc> a, b;

public:
	ComposeSet(UniquePointer<RealFunc> a, UniquePointer<RealFunc> b) : a(a), b(b) {}

	bool check(float x) const override { return a.ptr->getDomain().check(b.ptr->safeEval(x)); }

	ComposeSet* clone() const override { return new ComposeSet(*this); }
};

struct Exponent : RealFunc
{
private:
	virtual float evalAt(float x) override { return std::pow(E, x); }

public:
	Exponent() : RealFunc{AllReals{}, "exp"} {}

	virtual RealFunc* clone() const override { return new Exponent(*this); }
};

struct Log : RealFunc
{
private:
	float evalAt(float x) override { return std::log(x); }

public:
	Log() : RealFunc{Positives{}, "log"} {}

	virtual RealFunc* clone() const override { return new Log(*this); }
};

struct BoyanFunction : RealFunc
{
private:
	int n;
	int* myBigArr = new int[100]; // Illustration of the need of virtual d-tors

	float evalAt(float x) override
	{
		n++;
		if (n >= 10) return 0;
		return x;
	}

public:
	// Unless virtual, will not be called when delete-ing through a RealFunc ptr
	virtual ~BoyanFunction() override { delete[] myBigArr; }
};

struct Compose : RealFunc
{
private:
	const UniquePointer<RealFunc> f, g;
	
	float evalAt(float x) override { return f.ptr->safeEval(g.ptr->safeEval(x)); }

public:
    Compose(UniquePointer<RealFunc> f, UniquePointer<RealFunc> g)
	    : RealFunc{ComposeSet{f, g}, "Compose"}, f(f), g(g)
	{
	}

	virtual RealFunc* clone() const override { return new Compose(*this); }
};

struct Sin : RealFunc
{
private:
	float evalAt(float x) override { return std::sin(x); }

public:
	Sin() : RealFunc{AllReals{}, "sin"} {}

	virtual RealFunc* clone() const override { return new Sin(*this); }
};

struct Tan : RealFunc
{
private:
	float evalAt(float x) override { return std::tan(x); }

public:
	Tan() : RealFunc{TanDomain{}, "tan"} {}

	virtual RealFunc* clone() const override { return new Tan(*this); }
};

struct Pow : RealFunc
{
private:
	const float exp;
	
	float evalAt(float x) override { return std::pow(x, exp); }

public:
	Pow(const float exp) : RealFunc{Positives{}, "pow"}, exp(exp) {}

	virtual RealFunc* clone() const override { return new Pow(*this); }
};

struct Sum : public RealFunc
{
private:
	const UniquePointer<RealFunc> f, g;

	float evalAt(float x) override { return f.ptr->safeEval(x) + g.ptr->safeEval(x); }

public:
	Sum(UniquePointer<RealFunc> f, UniquePointer<RealFunc>  g)
	    : RealFunc{Intersect{{f.ptr->getDomain()}, {g.ptr->getDomain()}},
	               "Sum"},
	      f(f), g(g)
	{
	}

	Sum(Sum const&) = default;

	virtual RealFunc* clone() const override { return new Sum(*this); }

	~Sum() {}
};

struct Product : public RealFunc
{
private:
	const UniquePointer<RealFunc> f, g;

	float evalAt(float x) override { return f.ptr->safeEval(x) * g.ptr->safeEval(x); }

public:
	Product(UniquePointer<RealFunc> f, UniquePointer<RealFunc> g)
	    : RealFunc{Intersect{{f.ptr->getDomain()}, {g.ptr->getDomain()}},
	               "Sum"},
	      f(f), g(g)
	{
	}

	Product(Product const&) = default;

	virtual RealFunc* clone() const override { return new Product(*this); }

	~Product() {}
};

Product operator*(RealFunc& a, RealFunc& b) { return {a, b}; }

Sum operator+(RealFunc& a, RealFunc& b) { return {a, b}; }

Compose operator>>(RealFunc& a, RealFunc& b) { return {a, b}; }

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
	Compose comp{exp, sum};

	RealFunc* base = &comp;
	std::cout << " dsadsa " << std::endl;
	std::cout << base->safeEval(3) << std::endl;

	Intersect str{sum.getDomain(), base->getDomain()};

	// problem with Compose's defined domain - example
	Log log;
	Sin sin;
	Compose comp2{log, sin};
	base = &comp2;
	std::cout << base->safeEval(3 * M_PI / 2) << std::endl;
}