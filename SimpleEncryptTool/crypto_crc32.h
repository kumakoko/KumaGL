/**************************************************************************
* @file CryptoCRC32.h
* @brief
* @author x.k.xiong
* @date 2013/3/21 18:44:41
* @version
**************************************************************************/
#ifndef crypto_crc32_h__
#define crypto_crc32_h__

#include <cstdint>

class CryptoCRC32
{
private:
	static uint32_t crc32_table1_[256];
	static uint32_t crc32_table2_[256];
	static bool		table_init_;
public:
	static void InitTable();
public:
	/*
	* @brief encode
	*
	* 根据源数据区的数据，生成一个CRC32整数值
	* @param[in] buf			源数据区
	* @param[in] len			源数据区字节数
	* @param[in] use_first_table	使用CRC32生成参数表1，为true，使用CRC32生成参数表2，为false
	* @return uint32_t			返回得到的CRC32值
	*/
	static uint32_t Encode(const uint8_t* buf, int32_t len, bool use_first_table);
};


#endif // crypto_crc32_h__