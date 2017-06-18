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
	cout<<"0 退出程序按0"<<endl;
	cout<<"1 设置工作路径按1"<<endl;
	cout<<"2 批量加密工作路径下的文件按2"<<endl;
	cout<<"3 单独加密单个文件按3"<<endl;
	cout<<"------------------------------"<<endl;
	cout<<endl;
}

void encrypt_single_file()
{
	string filename;
	cout<<"请输入要加密的文件名："<<endl;
	cin>>filename;
	filename = cur_work_dir + "/" + filename;

	bool ret = filesystem::is_regular_file(filesystem::path(filename));

	if (!ret)
	{
		cout<<"输入的文件不是一个有效的文件，退出"<<endl;
		return;
	}

	cout<<"文件"<<filename<<"加密中"<<endl;
	AutoBuffer buf;
	ret = SimpleFileCrypto::encryptFileInPlace(filename.c_str(),buf);

	if (ret )
	{
		cout<<"加密成功"<<endl;
	}
	else
		cout<<"加密失败"<<endl;
}


int main()
{
	filesystem::path cur_work_path = filesystem::current_path();
	cur_work_dir = cur_work_path.string();
	cout<<"当前工作目录是 "<<endl;
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
			cout<<"错误的选择，请重新选择"<<endl;
			break;
		}

		menu();
	}

	return 0;
}