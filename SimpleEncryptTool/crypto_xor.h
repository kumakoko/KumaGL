/**************************************************************************
* @file CryptoXOR.h
* @brief 使用异或操作的简单加密方案，异或加密的源字节串和目标字节串是等长的
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
	* 对源字节串data以in-place改写的方式进行异或加密
	* @param[in] data		源字节串，同时也作为目标字节串
	* @param[in] data_len	源字节串的长度
	* @param[in] key		异或加解密的密钥，一个简单的8位整数
	* @return size_t		返回源字节串的长度
	*/
	static size_t Encode(uint8_t* data, size_t data_len, uint8_t key);
	/*
	* @brief decode 
	* 
	* 对源字节串data以in-place改写的方式进行异或解密
	* @param[in] data		源字节串，同时也作为目标字节串
	* @param[in] data_len	源字节串的长度
	* @param[in] key		异或加解密的密钥，一个简单的8位整数
	* @return size_t		返回源字节串的长度
	*/
	static size_t Decode(uint8_t* data, size_t data_len, uint8_t key);

	static size_t Encode(AutoBuffer& auto_buf,uint8_t key);

	static size_t Decode(AutoBuffer& auto_buf,uint8_t key);

	/*
	* @brief encodeTo 
	* 
	* 对源字节串data进行异或加密
	* @param[in] data		源字节串
	* @param[in] data_len	源字节串的长度
	* @param[in] key		异或加解密的密钥，一个简单的8位整数
	* @return uint8_t*		从heap中创建出来的目标字节串首指针，失败返回0。调用函数者负责删除此目标字节串
	*/
	static uint8_t* EncodeTo(const uint8_t* data, size_t data_len, uint8_t key);

	/*
	* @brief decodeFrom 
	* 
	* 对源字节串data以in-place改写的方式进行异或解密
	* @param[in] data		源字节串
	* @param[in] data_len	源字节串的长度
	* @param[in] key		异或加解密的密钥，一个简单的8位整数
	* @return uint8_t*		从heap中创建出来的目标字节串首指针，失败返回0。调用函数者负责删除此目标字节串
	*/
	static uint8_t* DecodeFrom(const uint8_t* data, size_t data_len, uint8_t key);
};

#endif // crypto_xor_h__