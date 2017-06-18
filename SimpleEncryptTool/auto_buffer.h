/*************************************************************************
* @file AutoBuffer.h
* @brief  ��ĳЩ���ϣ�������Ҫ��һ�������ڶ�̬����һ���ڴ棬Ȼ���ں����˳�ǰ
          ɾ��������ڴ棬�����װ��������һ������/ɾ����������������ڴ�����
          �ű����ʵ������������������յ����������ڶ��д���ʵ������
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
	* ��ȡ��������Ŀռ����ָ��
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
	* ����ָ����С�Ŀռ�
	* @param[in] alloc_size	Ҫ����Ŀռ�Ĵ�С
	* @param[in] is_zero_memory Defaults to false.	�Ƿ�Է���Ŀռ���0
	* @return bool
	*/
	bool Allocate(uint32_t alloc_size, bool is_zero_memory = false);

	/*
	* @brief release 
	* 
	* �ͷŵ���������������ڴ�ռ�
	*/
	void Release();

	/*
	* @brief getSize 
	* 
	* ��ȡ����������������ڴ�ռ�Ĵ�С
	* @return uint32_t  
	*/
	inline uint32_t GetSize() const 
	{
		return buffer_size_;
	}

	/*
	* @brief isAllocated 
	* 
	* ���������Ŀռ��Ƿ��Ѵ���
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
	* ȱʡ���캯��
	*/
	AutoBuffer();

	/*
	* @brief Constructor for AutoBuffer.
	* 
	* �������캯��
	* @param[in] buffer 
	*/
	AutoBuffer(const AutoBuffer& buffer);

	/*
	* @brief Destructor for AutoBuffer.
	* 
	* ��������
	*/
	virtual ~AutoBuffer();

	/*
	* @brief operator= 
	* 
	* ���ظ�ֵ������
	* @param[in] buffer 
	* @return AutoBuffer&  
	*/
	AutoBuffer& operator = (const AutoBuffer& buffer);

	/*
	* @brief readFileDataIn 
	* 
	* ���ļ�file_name������ȫ�����뵽��autobuffer����Ļ�������
	* @param[in] file_name	Ҫ��ȡ���ļ�������
	* @return bool			�ɹ�����true��ʧ�ܷ���false
	* @param[in] binary_mode Defaults to true.
	*/
	bool ReadFromFile(const char* file_name,bool binary_mode = true);

	/*
	* @brief writeToFile 
	* 
	* Detailed description.
	* @param[in] file_name						Ҫд����ļ���
	* @param[in] written_len						Ҫд����ֽڳ���
	* @param[in] binary_mode Defaults to true.	�Ƿ��Զ����Ƶķ�ʽд��
	* @param[in] append Defaults to true.		�Ƿ���׷�ӵķ�ʽд�룬falseΪ���¸�д
	* @return bool  
	*/
	bool WriteToFile(const char* file_name,uint32_t written_len,bool binary_mode = true, bool append = true);

	// ��ĳ���ڴ�󶨵���autobuffer�������ɱ���������ڰ�ǰ���ͷű�����֮ǰ����Ķ���
	void AttachToBuffer(uint8_t* buffer,uint32_t buf_size);
private:
	/*
	GCC���棺��operator new�����ܷ��� NULL��������������Ϊ��throw()��(�� -fcheck-new ������)
	C++�е�operator new��new operator������ȥͦ��ȴ������֮��
	operator new
	��1�� ֻ������Ҫ��Ŀռ䣬��������ض���Ĺ��캯�������޷�������Ҫ�����Ŀռ�ʱ����
	->�����new_handler�������new_handler������
	->���ûҪ���׳��쳣����nothrow����������ִ��bad_alloc�쳣�����򷵻�0
	��2�� ���Ա�����
	��3�� ����ʱ���������ͱ�������Ϊvoid*
	��4�� ����ʱ����һ���������ͱ���Ϊ���Ҫ�����ռ�Ĵ�С���ֽڣ�������Ϊsize_t
	��5�� ����ʱ�����Դ���������

	new operator
	��1�� ����operator new�����㹻�Ŀռ䣬��������ض���Ĺ��캯��
	��2�� �����Ա�����

	��Ӧ�أ�operator delete��delete operator�����Ƶ����ԡ�
	*/
	/*
	* @brief new 
	* 
	* ���ص�new��������ʹ�ñ��������ڶ��д���
	* @param[in] obj_size 
	* @return void* operator  
	*/
	static void* operator new(size_t obj_size);

	/*
	* @brief new[] 
	* 
	* ���ص�new[]��������ʹ�ñ��������ڶ��д���
	* @param[in] array_size 
	* @return void*
	*/
	static void* operator new[](size_t array_size);

protected:
	char*		buffer_;		 /**< Ҫ����Ŀռ� */
	uint32_t	buffer_size_;		 /**< ������Ŀռ�ĳ��� */
};

#endif // auto_buffer_h__