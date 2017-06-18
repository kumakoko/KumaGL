/**************************************************************************
* @file CryptoXOR.h
* @brief ʹ���������ļ򵥼��ܷ����������ܵ�Դ�ֽڴ���Ŀ���ֽڴ��ǵȳ���
* @author x.k.xiong
* @date 2013/3/21 17:46:04
* @version 
**************************************************************************/
#ifndef crypto_xor_h__
#define crypto_xor_h__

#include <cstdint>

class AutoBuffer;

class CryptoXOR
{
private:
	static const uint8_t key_table_[256];
public:
	/*
	* @brief encode 
	* 
	* ��Դ�ֽڴ�data��in-place��д�ķ�ʽ����������
	* @param[in] data		Դ�ֽڴ���ͬʱҲ��ΪĿ���ֽڴ�
	* @param[in] data_len	Դ�ֽڴ��ĳ���
	* @param[in] key		���ӽ��ܵ���Կ��һ���򵥵�8λ����
	* @return size_t		����Դ�ֽڴ��ĳ���
	*/
	static size_t Encode(uint8_t* data, size_t data_len, uint8_t key);
	/*
	* @brief decode 
	* 
	* ��Դ�ֽڴ�data��in-place��д�ķ�ʽ����������
	* @param[in] data		Դ�ֽڴ���ͬʱҲ��ΪĿ���ֽڴ�
	* @param[in] data_len	Դ�ֽڴ��ĳ���
	* @param[in] key		���ӽ��ܵ���Կ��һ���򵥵�8λ����
	* @return size_t		����Դ�ֽڴ��ĳ���
	*/
	static size_t Decode(uint8_t* data, size_t data_len, uint8_t key);

	static size_t Encode(AutoBuffer& auto_buf,uint8_t key);

	static size_t Decode(AutoBuffer& auto_buf,uint8_t key);

	/*
	* @brief encodeTo 
	* 
	* ��Դ�ֽڴ�data����������
	* @param[in] data		Դ�ֽڴ�
	* @param[in] data_len	Դ�ֽڴ��ĳ���
	* @param[in] key		���ӽ��ܵ���Կ��һ���򵥵�8λ����
	* @return uint8_t*		��heap�д���������Ŀ���ֽڴ���ָ�룬ʧ�ܷ���0�����ú����߸���ɾ����Ŀ���ֽڴ�
	*/
	static uint8_t* EncodeTo(const uint8_t* data, size_t data_len, uint8_t key);

	/*
	* @brief decodeFrom 
	* 
	* ��Դ�ֽڴ�data��in-place��д�ķ�ʽ����������
	* @param[in] data		Դ�ֽڴ�
	* @param[in] data_len	Դ�ֽڴ��ĳ���
	* @param[in] key		���ӽ��ܵ���Կ��һ���򵥵�8λ����
	* @return uint8_t*		��heap�д���������Ŀ���ֽڴ���ָ�룬ʧ�ܷ���0�����ú����߸���ɾ����Ŀ���ֽڴ�
	*/
	static uint8_t* DecodeFrom(const uint8_t* data, size_t data_len, uint8_t key);
};

#endif // crypto_xor_h__