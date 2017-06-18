/*************************************************************************
* @file AutoBuffer.h
* @brief  在某些场合，经常需要在一个函数内动态分配一段内存，然后在函数退出前
          删除掉这段内存，本类封装了这样的一个分配/删除操作，并且这段内存能随
          着本类的实例对象的析构而被回收掉本来不能在堆中创建实例对象
* @author x.k.xiong
* @date 2013/3/22 9:39:14
* @version 
*************************************************************************/
#ifndef auto_buffer_h__
#define auto_buffer_h__

#include <cstdint>

class AutoBuffer
{
public:
	/*
	* @brief char*
	* 
	* Detailed description.
	* @return operator  
	*/
	inline operator char*()
	{
		return buffer_;
	}

	/*
	* @brief char* 
	* 
	* Detailed description.
	* @return operator const  
	*/
	inline operator const char*() const
	{
		return buffer_;
	}

	/*
	* @brief getBuffer 
	* 
	* 获取到所分配的空间的首指针
	* @return const char*  
	*/
	inline const char* GetBuffer() const
	{
		return buffer_;
	}

	inline uint8_t* GetUnsignedBuffer()
	{
		return (uint8_t*)buffer_;
	}

	inline const uint8_t* GetUnsignedBuffer() const
	{
		return (uint8_t*)buffer_; 
	}

	/*
	* @brief allocate 
	* 
	* 分配指定大小的空间
	* @param[in] alloc_size	要分配的空间的大小
	* @param[in] is_zero_memory Defaults to false.	是否对分配的空间清0
	* @return bool
	*/
	bool Allocate(uint32_t alloc_size, bool is_zero_memory = false);

	/*
	* @brief release 
	* 
	* 释放掉本对象所管理的内存空间
	*/
	void Release();

	/*
	* @brief getSize 
	* 
	* 获取到本对象所管理的内存空间的大小
	* @return uint32_t  
	*/
	inline uint32_t GetSize() const 
	{
		return buffer_size_;
	}

	/*
	* @brief isAllocated 
	* 
	* 检查所管理的空间是否已创建
	* @return bool  
	*/
	inline bool IsAllocated() const
	{
		return (buffer_ != 0);
	}

	/*
	* @brief Constructor for AutoBuffer.
	* 
	* Detailed description.
	* @param[in] alloc_size 
	* @param[in] is_zero_memory Defaults to false.
	*/
	AutoBuffer(uint32_t alloc_size, bool is_zero_memory = false);

	/*
	* @brief Constructor for AutoBuffer.
	* 
	* 缺省构造函数
	*/
	AutoBuffer();

	/*
	* @brief Constructor for AutoBuffer.
	* 
	* 拷贝构造函数
	* @param[in] buffer 
	*/
	AutoBuffer(const AutoBuffer& buffer);

	/*
	* @brief Destructor for AutoBuffer.
	* 
	* 析构函数
	*/
	virtual ~AutoBuffer();

	/*
	* @brief operator= 
	* 
	* 重载赋值操作符
	* @param[in] buffer 
	* @return AutoBuffer&  
	*/
	AutoBuffer& operator = (const AutoBuffer& buffer);

	/*
	* @brief readFileDataIn 
	* 
	* 把文件file_name的内容全部读入到本autobuffer管理的缓冲区中
	* @param[in] file_name	要读取的文件的名字
	* @return bool			成功返回true，失败返回false
	* @param[in] binary_mode Defaults to true.
	*/
	bool ReadFromFile(const char* file_name,bool binary_mode = true);

	/*
	* @brief writeToFile 
	* 
	* Detailed description.
	* @param[in] file_name						要写入的文件名
	* @param[in] written_len						要写入的字节长度
	* @param[in] binary_mode Defaults to true.	是否以二进制的方式写入
	* @param[in] append Defaults to true.		是否以追加的方式写入，false为重新覆写
	* @return bool  
	*/
	bool WriteToFile(const char* file_name,uint32_t written_len,bool binary_mode = true, bool append = true);

	// 把某段内存绑定到本autobuffer，即交由本对象管理，在绑定前会释放本对象之前管理的对象
	void AttachToBuffer(uint8_t* buffer,uint32_t buf_size);
private:
	/*
	GCC警告：‘operator new’不能返回 NULL，除非它被声明为‘throw()’(或 -fcheck-new 起作用)
	C++中的operator new与new operator，看上去挺像，却有天壤之别。
	operator new
	（1） 只分配所要求的空间，不调用相关对象的构造函数。当无法满足所要求分配的空间时，则
	->如果有new_handler，则调用new_handler，否则
	->如果没要求不抛出异常（以nothrow参数表达），则执行bad_alloc异常，否则返回0
	（2） 可以被重载
	（3） 重载时，返回类型必须声明为void*
	（4） 重载时，第一个参数类型必须为表达要求分配空间的大小（字节），类型为size_t
	（5） 重载时，可以带其它参数

	new operator
	（1） 调用operator new分配足够的空间，并调用相关对象的构造函数
	（2） 不可以被重载

	相应地，operator delete与delete operator有相似的特性。
	*/
	/*
	* @brief new 
	* 
	* 重载掉new操作符，使得本对象不能在堆中创建
	* @param[in] obj_size 
	* @return void* operator  
	*/
	static void* operator new(size_t obj_size);

	/*
	* @brief new[] 
	* 
	* 重载掉new[]操作符，使得本对象不能在堆中创建
	* @param[in] array_size 
	* @return void*
	*/
	static void* operator new[](size_t array_size);

protected:
	char*		buffer_;		 /**< 要分配的空间 */
	uint32_t	buffer_size_;		 /**< 所分配的空间的长度 */
};

#endif // auto_buffer_h__