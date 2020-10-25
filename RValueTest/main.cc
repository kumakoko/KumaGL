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
	// ��Ϊ����û�б�Ĳ�����ֻ��һ���򵥵�return�����Ծ��㲻���Ż�
	// ���MakeDemoClassBInstance1����ʵ�����ǵ�ͬ�ڲ����ڵģ�
	return DemoClassB(num, name);
}

DemoClassB MakeDemoClassBInstance2(int num, const char* name)
{
	// ��ΪDemoClassBû���ƶ����캯�������Է���b��������Ӧ����ʱ
	// ����ʱ�������DemoClassB�Ŀ������캯�����������ʱ����
	DemoClassB b(num, name);
	return b;
}

void main()
{
	std::vector<DemoClassA> VecDemoClassA;
	std::vector<DemoClassB> VecDemoClassB;

	VecDemoClassA.reserve(10);
	VecDemoClassB.reserve(10);

	// ��ִ�н�����Կ���������MakeDemoClassAInstance1������ֻ�����˵���DemoClassA
	// ���캯����һ�κ������á���������û�п��κε��Ż����أ����MakeDemoClassAInstance1
	// ����ֱ���Ż�����
	std::printf("MakeDemoClassAInstance1 begins ---->>>>\n");
	auto dca1 = MakeDemoClassAInstance1(0, "John");
	std::printf("MakeDemoClassAInstance1 ends ------<<<<\n\n");

	// ��ִ�н�����Կ���������MakeDemoClassAInstance2�����󣬵���DemoClassA
	// ���캯��1�Σ��ƶ����캯��1�Σ���������һ�Σ�Ҳ����˵���������ȷ���������ƶ����캯��
	// ������ʱ��������ɵĴ���ʱ�����ȵ����ƶ����캯��
	std::printf("MakeDemoClassAInstance2 begins ---->>>>\n");
	auto dca2 = MakeDemoClassAInstance2(1, "Bob");
	std::printf("MakeDemoClassAInstance2 ends ------<<<<\n\n");

	// ��ִ�н�����Կ���������MakeDemoClassBInstance1������ֻ�����˵���DemoClassB
	// ���캯����һ�κ������á���������û�п��κε��Ż����أ����MakeDemoClassBInstance1
	// ����ֱ���Ż�����
	std::printf("MakeDemoClassBInstance1 begins ---->>>>\n");
	auto dcb1 = MakeDemoClassBInstance1(0, "Lucy");
	std::printf("MakeDemoClassBInstance1 ends ------<<<<\n\n");

	// ��ִ�н�����Կ���������MakeDemoClassBInstance2�����󣬵���DemoClassB
	// ���캯��1�Σ��������캯��1�Σ���������һ�Σ�Ҳ����˵�������û����ȷ���������ƶ�����
	// ������������ʱ��������ɵĴ���ʱ��ʹ�ÿ������캯��
	std::printf("MakeDemoClassBInstance2 begins ---->>>>\n");
	auto dcb2 = MakeDemoClassBInstance2(1, "Lily");
	std::printf("MakeDemoClassBInstance2 ends ------<<<<\n\n");

	// ��ִ�н�����Կ�����std::vector��push_back�����´��һ��������vector���棬��Ϊ
	// �ҡ�DemoClassA(4, "Ken")�����������������һ����ֵ������DemoClassA���ƶ����캯
	// ����������ʹ�����ƶ����캯����������һ�����󸱱��Ž�vector��
	std::printf("std::vector<DemoClassA>::push_back begins ---->>>>\n");
	VecDemoClassA.push_back(DemoClassA(4, "Ken"));
	std::printf("std::vector<DemoClassA>::push_back ends ------<<<<\n\n");

	// ��ִ�н�����Կ���������ȹ���һ��DemoClassA��ʵ������dcb11��Ȼ���ٵ���std::vector��
	// push_back����ʱ�������ʹ��std::move������dcb11תΪ��ֵ�Ļ������ÿ������캯������һ��
	// ��������ʹ��std::move������dcb11תΪ��ֵ��������ƶ����캯�����츱���Ž�vector
	// һ��أ���std::move������Ķ��������еı�����͵�ˣ�����һ�㲻Ҫ����ټ���ʹ����
	std::printf("std::vector<DemoClassA>::push_back with move begins ---->>>>\n");
	auto dcb11 = DemoClassA(74, "Obama");
	VecDemoClassA.push_back(dcb11);
	VecDemoClassA.push_back(std::move(dcb11));
	dcb11.PrintInfo(); // dcb11��name_ָ�����Դ�Ѿ���͵���ˣ�name_�Ѿ�����Ϊnullptr
	std::printf("std::vector<DemoClassA>::push_back with move ends ------<<<<\n\n");

	// ��ִ�н�����Կ�����std::vector��push_back�����´��һ��������vector���棬��Ϊ
	// DemoClassAֻ�п������캯����������ʹ���˿������캯��������һ�����󸱱��Ž�vector��
	std::printf("std::vector<DemoClassB>::push_back begins ---->>>>\n");
	VecDemoClassB.push_back(DemoClassB(5, "David"));
	std::printf("std::vector<DemoClassB>::push_back ends ------<<<<\n\n");

	// ��ִ�н�����Կ�����emplace_back��ֱ�����ɸ�һ����ʱ����push��vector��
	std::printf("std::vector<DemoClassA>::emplace_back begins ---->>>>\n");
	VecDemoClassA.emplace_back(14, "Kate");
	std::printf("std::vector<DemoClassA>::emplace_back ends ------<<<<\n\n");

	// ��ִ�н�����Կ�����emplace_back��ֱ�����ɸ�һ����ʱ����push��vector��
	std::printf("std::vector<DemoClassB>::emplace_back begins ---->>>>\n");
	VecDemoClassB.emplace_back(24, "Fancy");
	std::printf("std::vector<DemoClassB>::emplace_back ends ------<<<<\n\n");

	std::map<int, DemoClassA> MapDemoClassA;
	MapDemoClassA.emplace(1, DemoClassA(1, "Zangief"));
	MapDemoClassA.emplace(1, DemoClassA(2, "Xavi"));
	MapDemoClassA.emplace(1, DemoClassA(3, "Loong"));

	system("PAUSE");
}