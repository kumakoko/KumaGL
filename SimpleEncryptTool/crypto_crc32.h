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
	* ����Դ�����������ݣ�����һ��CRC32����ֵ
	* @param[in] buf			Դ������
	* @param[in] len			Դ�������ֽ���
	* @param[in] use_first_table	ʹ��CRC32���ɲ�����1��Ϊtrue��ʹ��CRC32���ɲ�����2��Ϊfalse
	* @return uint32_t			���صõ���CRC32ֵ
	*/
	static uint32_t Encode(const uint8_t* buf, int32_t len, bool use_first_table);
};


#endif // crypto_crc32_h__