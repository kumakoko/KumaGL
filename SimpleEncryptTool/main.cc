#include "type_define.h"
#include "boost/filesystem.hpp"
#include "auto_buffer.h"
#include <iostream>
#include <string>
#include "simple_file_crypto.h"

using namespace std;
using namespace boost;

string cur_work_dir;

void menu()
{
	cout<<endl;
	cout<<"-------------MENU-------------"<<endl;
	cout<<"                              "<<endl;
	cout<<"0 �˳�����0"<<endl;
	cout<<"1 ���ù���·����1"<<endl;
	cout<<"2 �������ܹ���·���µ��ļ���2"<<endl;
	cout<<"3 �������ܵ����ļ���3"<<endl;
	cout<<"------------------------------"<<endl;
	cout<<endl;
}

void encrypt_single_file()
{
	string filename;
	cout<<"������Ҫ���ܵ��ļ�����"<<endl;
	cin>>filename;
	filename = cur_work_dir + "/" + filename;

	bool ret = filesystem::is_regular_file(filesystem::path(filename));

	if (!ret)
	{
		cout<<"������ļ�����һ����Ч���ļ����˳�"<<endl;
		return;
	}

	cout<<"�ļ�"<<filename<<"������"<<endl;
	AutoBuffer buf;
	ret = SimpleFileCrypto::encryptFileInPlace(filename.c_str(),buf);

	if (ret )
	{
		cout<<"���ܳɹ�"<<endl;
	}
	else
		cout<<"����ʧ��"<<endl;
}


int main()
{
	filesystem::path cur_work_path = filesystem::current_path();
	cur_work_dir = cur_work_path.string();
	cout<<"��ǰ����Ŀ¼�� "<<endl;
	cout<<cur_work_dir<<endl;

	menu();

	char input_data;

	while(true)
	{
		cin>>input_data;

		switch(input_data)
		{
		case '1':
			break;
		case '2':
			break;
		case '3':
			encrypt_single_file();
			break;
		case '0':
			exit(0);
		default:
			cout<<"�����ѡ��������ѡ��"<<endl;
			break;
		}

		menu();
	}

	return 0;
}