#include "auto_buffer.h"
#include <cstdlib>
#include <cstring>
#include <iosfwd>
#include <fstream>

AutoBuffer::AutoBuffer():buffer_size_(0),buffer_(0)
{
}

AutoBuffer::AutoBuffer(uint32_t alloc_size, bool is_zero_memory):buffer_size_(0),buffer_(0)
{
	this->Allocate(alloc_size, is_zero_memory);
}

AutoBuffer::~AutoBuffer()
{
	this->Release();
}

void AutoBuffer::Release()
{
	if (buffer_)
	{
		delete [] buffer_;
		buffer_size_ = 0;
		buffer_ = 0;
	}
}

bool AutoBuffer::Allocate(uint32_t alloc_size, bool is_zero_memory)
{
	if (alloc_size != buffer_size_)
	{
		this->Release();
	}
	else
	{
		if (is_zero_memory)
			std::memset(buffer_, 0, alloc_size);

		return true;
	}

	if (alloc_size > 0)
	{
		buffer_ = new char [alloc_size];

		if (!buffer_)
			return false;

		if (is_zero_memory)
			std::memset(buffer_, 0, alloc_size);

		buffer_size_ = alloc_size;
	}
	else 
	{
		buffer_ = 0;
		return false;
	}

	return true;
}

AutoBuffer::AutoBuffer(const AutoBuffer& buffer)
{
	buffer_ = 0;
	buffer_size_ = 0;

	if (buffer.buffer_)
	{
		this->Allocate(buffer.buffer_size_);
		std::memcpy(buffer_, buffer.buffer_, buffer.buffer_size_);
	}	
}

AutoBuffer& AutoBuffer::operator=(const AutoBuffer& buffer)
{
	if (this == &buffer)
	{
		return *this;
	}

	this->Release();

	if (buffer.buffer_)
	{
		Allocate(buffer.buffer_size_);
		std::memcpy(buffer_, buffer.buffer_, buffer.buffer_size_);
	}

	return *this;
}

bool AutoBuffer::ReadFromFile(const char* file_name,bool binary_mode)
{
	std::ios_base::openmode openMode = std::ios::in;

	if (binary_mode)
		openMode |= std::ios_base::binary;

	std::ifstream file(file_name,openMode);

	if (!file.is_open()) // ��ʧ��
	{
		return false;
	}

	file.seekg(0,std::ios::end); //�Ƶ�β��
	uint32_t fileLen = (uint32_t)file.tellg(); // ��ȡ���ļ��Ĵ�С

	/* ��������ֱ�Ӷ�ȡȫ���ļ�����̬�ڴ��еķ�����ֻ���ʺ�����
	����̫��������Ŀǰ�����ǵ��ļ�������̫�󣬿���ֱ��������*/
	bool ret = this->Allocate(fileLen,true);

	if (!ret)
		return false;

	file.seekg(0,std::ios::beg);
	file.read(buffer_,fileLen); // ���뵽�ڴ�
	file.close();
	return true;
}

bool AutoBuffer::WriteToFile(const char* file_name,uint32_t written_len,bool binary_mode, bool append)
{
	std::ios_base::openmode openMode = std::ios::out;

	if (binary_mode)
		openMode |= std::ios_base::binary;

	openMode |= append ? std::ios_base::app : std::ios_base::trunc;

	std::ofstream file(file_name,std::ios::out|std::ios::trunc|std::ios::binary);

	if (!file.is_open())
	{
		return false;
	}

	written_len = written_len > buffer_size_ ? buffer_size_ : written_len;
	file.write(buffer_,written_len);  // д���ļ�ͷ
	file.close();
	return true;
}

void AutoBuffer::AttachToBuffer(uint8_t* buffer,uint32_t buf_size)
{
	this->Release();
	buffer_ = (char*)buffer_;
	buffer_size_ = buf_size;
}