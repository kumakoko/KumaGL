/**************************************************************************************************************************
Copyright(C) 2014-2017 www.xionggf.com

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy,
modify, merge, publish, distribute,sublicense, and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the
Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM,OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**************************************************************************************************************************/
#include <cstring>
#include <cstdlib>
#include <map>
#include <vector>
#include <cstdio>

class DemoClassA
{
public:
	DemoClassA() :num_(0), name_(nullptr)
	{
		std::printf("DemoClassA::DemoClassA()\n");
	}

	~DemoClassA()
	{
		if (name_)
		{
			std::printf("DemoClassA::~DemoClassA(%d,%s)\n", num_, name_);
			delete[] name_;
			name_ = nullptr;
		}
		else
		{
			std::printf("DemoClassA::~DemoClassA()\n");
		}
	}

	DemoClassA(int num, const char* name)
	{
		num_ = num;
		auto len = std::strlen(name) + 1;
		name_ = new char[len];
		memset(name_, 0, len);
		std::strcpy(name_, name);
		std::printf("DemoClassA::DemoClassA(%d,%s)\n", num_, name_);
	}

	DemoClassA(const DemoClassA& rhs)
	{
		num_ = rhs.num_;
		auto len = std::strlen(rhs.name_) + 1;
		name_ = new char[len];
		memset(name_, 0, len);
		std::strcpy(name_, rhs.name_);
		std::printf("DemoClassA::DemoClassA(const DemoClassA& rhs)\n");
	}

	DemoClassA(DemoClassA&& rhs)
	{
		num_ = rhs.num_;
		name_ = rhs.name_;
		rhs.num_ = 0;
		rhs.name_ = nullptr;
		std::printf("DemoClassA::DemoClassA(DemoClassA&& rhs %d, %s)\n",num_,name_);
	}

	DemoClassA& operator = (const DemoClassA& rhs)
	{
		if (name_)
		{
			delete[] name_;
			name_ = nullptr;
		}

		num_ = rhs.num_;
		auto len = std::strlen(rhs.name_) + 1;
		name_ = new char[len];
		memset(name_, 0, len);
		std::strcpy(name_, rhs.name_);
		std::printf("DemoClassA::operator = (const DemoClassA& rhs)\n");
		return *this;
	}

	DemoClassA& operator = (DemoClassA&& rhs)
	{
		if (name_)
		{
			delete[] name_;
			name_ = nullptr;
		}

		num_ = rhs.num_;
		name_ = rhs.name_;
		rhs.num_ = 0;
		rhs.name_ = nullptr;
		std::printf("DemoClassA::operator = (DemoClassA&& rhs)\n");
		return *this;
	}

	void PrintInfo()
	{
		if (name_)
			std::printf("DemoClassA: I am %s.My number is %d\n", name_, num_);
		else
			std::printf("DemoClassA: I am a dummy\n");
	}

	void ChangeNumber(int new_num)
	{
		num_ = new_num;
	}

private:
	int num_ = 0;
	char* name_ = nullptr;
};

class DemoClassB
{
public:
	DemoClassB() :num_(0), name_(nullptr)
	{
		std::printf("DemoClassB::DemoClassB()\n");
	}

	~DemoClassB()
	{
		if (name_)
		{
			std::printf("DemoClassB::~DemoClassB(%d,%s)\n", num_, name_);
			delete[] name_;
			name_ = nullptr;
		}
		else
		{
			std::printf("DemoClassB::~DemoClassB()\n");
		}
	}

	DemoClassB(int num, const char* name)
	{
		num_ = num;
		auto len = std::strlen(name) + 1;
		name_ = new char[len];
		memset(name_, 0, len);
		std::strcpy(name_, name);
		std::printf("DemoClassB::DemoClassB(%d,%s)\n", num_, name_);
	}

	DemoClassB(const DemoClassB& rhs)
	{
		num_ = rhs.num_;
		auto len = std::strlen(rhs.name_) + 1;
		name_ = new char[len];
		memset(name_, 0, len);
		std::strcpy(name_, rhs.name_);
		std::printf("DemoClassB::DemoClassB(const DemoClassB& rhs %d,%s)\n", num_, name_);
	}

	DemoClassB& operator = (const DemoClassB& rhs)
	{
		if (name_)
		{
			delete[] name_;
			name_ = nullptr;
		}

		num_ = rhs.num_;
		auto len = std::strlen(rhs.name_) + 1;
		name_ = new char[len];
		memset(name_, 0, len);
		std::strcpy(name_, rhs.name_);
		std::printf("DemoClassB::operator = (const DemoClassB& rhs)\n");
		return *this;
	}

	void PrintInfo()
	{
		if (name_)
			std::printf("DemoClassB: I am %s.My number is %d\n", name_, num_);
		else
			std::printf("DemoClassB: I am a dummy\n");
	}

	void ChangeNumber(int new_num)
	{
		num_ = new_num;
	}

private:
	int		num_;
	char*	name_;
};


DemoClassA MakeDemoClassAInstance1(int num, const char* name)
{
	return DemoClassA(num, name);
}

DemoClassA MakeDemoClassAInstance2(int num, const char* name)
{
	DemoClassA a(num, name);
	return a;
}

DemoClassB MakeDemoClassBInstance1(int num, const char* name)
{
	// 因为这里没有别的操作，只是一个简单的return，所以就算不开优化
	// 这个MakeDemoClassBInstance1函数实质上是等同于不存在的，
	return DemoClassB(num, name);
}

DemoClassB MakeDemoClassBInstance2(int num, const char* name)
{
	// 因为DemoClassB没有移动构造函数，所以返回b对象所对应的临时
	// 对象时，会调用DemoClassB的拷贝构造函数生成这个临时对象
	DemoClassB b(num, name);
	return b;
}

void main()
{
	std::vector<DemoClassA> VecDemoClassA;
	std::vector<DemoClassB> VecDemoClassB;

	VecDemoClassA.reserve(10);
	VecDemoClassB.reserve(10);

	// 从执行结果可以看出，调用MakeDemoClassAInstance1函数后，只发生了调用DemoClassA
	// 构造函数这一次函数调用。所以哪怕没有开任何的优化开关，这个MakeDemoClassAInstance1
	// 函数直接优化掉了
	std::printf("MakeDemoClassAInstance1 begins ---->>>>\n");
	auto dca1 = MakeDemoClassAInstance1(0, "John");
	std::printf("MakeDemoClassAInstance1 ends ------<<<<\n\n");

	// 从执行结果可以看出，调用MakeDemoClassAInstance2函数后，调用DemoClassA
	// 构造函数1次，移动构造函数1次，析构函数一次，也就是说，如果类明确定了它的移动构造函数
	// 则发生临时对象的生成的传递时，优先调用移动构造函数
	std::printf("MakeDemoClassAInstance2 begins ---->>>>\n");
	auto dca2 = MakeDemoClassAInstance2(1, "Bob");
	std::printf("MakeDemoClassAInstance2 ends ------<<<<\n\n");

	// 从执行结果可以看出，调用MakeDemoClassBInstance1函数后，只发生了调用DemoClassB
	// 构造函数这一次函数调用。所以哪怕没有开任何的优化开关，这个MakeDemoClassBInstance1
	// 函数直接优化掉了
	std::printf("MakeDemoClassBInstance1 begins ---->>>>\n");
	auto dcb1 = MakeDemoClassBInstance1(0, "Lucy");
	std::printf("MakeDemoClassBInstance1 ends ------<<<<\n\n");

	// 从执行结果可以看出，调用MakeDemoClassBInstance2函数后，调用DemoClassB
	// 构造函数1次，拷贝构造函数1次，析构函数一次，也就是说，如果类没有明确定了它的移动构造
	// 函数，则发生临时对象的生成的传递时，使用拷贝构造函数
	std::printf("MakeDemoClassBInstance2 begins ---->>>>\n");
	auto dcb2 = MakeDemoClassBInstance2(1, "Lily");
	std::printf("MakeDemoClassBInstance2 ends ------<<<<\n\n");

	// 从执行结果可以看出，std::vector的push_back方法会拷贝一个副本到vector里面，因为
	// 且“DemoClassA(4, "Ken")”这样的语句生成了一个右值对象，且DemoClassA有移动构造函
	// 数，所以是使用了移动构造函数，构造了一个对象副本放进vector里
	std::printf("std::vector<DemoClassA>::push_back begins ---->>>>\n");
	VecDemoClassA.push_back(DemoClassA(4, "Ken"));
	std::printf("std::vector<DemoClassA>::push_back ends ------<<<<\n\n");

	// 从执行结果可以看出，如果先构造一个DemoClassA的实例对象dcb11，然后再调用std::vector的
	// push_back方法时，如果不使用std::move方法将dcb11转为右值的话，调用拷贝构造函数构造一个
	// 副本，如使用std::move方法将dcb11转为右值，则调用移动构造函数构造副本放进vector
	// 一般地，被std::move处理过的对象，它持有的变量被偷了，所以一般不要随便再继续使用它
	std::printf("std::vector<DemoClassA>::push_back with move begins ---->>>>\n");
	auto dcb11 = DemoClassA(74, "Obama");
	VecDemoClassA.push_back(dcb11);
	VecDemoClassA.push_back(std::move(dcb11));
	dcb11.PrintInfo(); // dcb11的name_指向的资源已经被偷走了，name_已经被置为nullptr
	std::printf("std::vector<DemoClassA>::push_back with move ends ------<<<<\n\n");

	// 从执行结果可以看出，std::vector的push_back方法会拷贝一个副本到vector里面，因为
	// DemoClassA只有拷贝构造函数，所以是使用了拷贝构造函数构造了一个对象副本放进vector里
	std::printf("std::vector<DemoClassB>::push_back begins ---->>>>\n");
	VecDemoClassB.push_back(DemoClassB(5, "David"));
	std::printf("std::vector<DemoClassB>::push_back ends ------<<<<\n\n");

	// 从执行结果可以看出，emplace_back会直接生成给一个临时对象push到vector里
	std::printf("std::vector<DemoClassA>::emplace_back begins ---->>>>\n");
	VecDemoClassA.emplace_back(14, "Kate");
	std::printf("std::vector<DemoClassA>::emplace_back ends ------<<<<\n\n");

	// 从执行结果可以看出，emplace_back会直接生成给一个临时对象push到vector里
	std::printf("std::vector<DemoClassB>::emplace_back begins ---->>>>\n");
	VecDemoClassB.emplace_back(24, "Fancy");
	std::printf("std::vector<DemoClassB>::emplace_back ends ------<<<<\n\n");

	std::map<int, DemoClassA> MapDemoClassA;
	MapDemoClassA.emplace(1, DemoClassA(1, "Zangief"));
	MapDemoClassA.emplace(1, DemoClassA(2, "Xavi"));
	MapDemoClassA.emplace(1, DemoClassA(3, "Loong"));

	system("PAUSE");
}