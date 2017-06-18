#include "project_pch.h"
#include "string_tool.h"

void StringTool::TrimLeft(std::string& str,char target)
{
	std::string::size_type pos = str.find_first_not_of(target);

	if (pos == std::string::npos)
		str.erase();
	if (pos)
		str.erase(0, pos);
}

void StringTool::TrimLeft(std::string& str, const char* target_str)
{
	std::string::size_type pos = str.find_first_not_of(target_str);

	if (pos == std::string::npos)
		str.erase();
	if (pos)
		str.erase(0, pos);
}

void StringTool::TrimRight(std::string& str, char target)
{
	std::string::size_type pos = str.find_last_not_of(target);

	if (pos == std::string::npos)
		str.erase();

	str.erase(++pos);
}

void StringTool::TrimRight(std::string& str, const char* target_str)
{
	std::string::size_type pos = str.find_last_not_of(target_str);

	if (pos == std::string::npos)
		str.erase();

	str.erase(++pos);
}

std::string StringTool::LeftSubString(const std::string& str, std::string::size_type count)
{
	return str.substr(0, count);
}

std::string StringTool::RightSubString(const std::string& str, std::string::size_type count)
{
	count = str.size() < count ? str.size() : count;
	return str.substr(str.size() - count);
}

std::string StringTool::MidSubString(const std::string& str,std::string::size_type first) 
{
	return str.substr(first);
}

std::string StringTool::MidSubString(const std::string& str, int first, int count)
{
	return str.substr(first, count);
}

char* StringTool::CopyToBuffer(const std::string& str, uint32_t* copy_size)
{
	if (str.length() == 0)
		return 0;

	std::string::size_type strLen = str.length() + 1;
	char* buffer = new char[strLen];
	std::memset(buffer,0,sizeof(char)*strLen);
	std::strcpy(buffer,str.c_str());

	if (copy_size)
		*copy_size = (uint32_t)strLen;

	return buffer;
}
