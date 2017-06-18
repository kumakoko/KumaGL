#include "crypto_crc32.h"

#define FIRST_TABLE_CONSTANT 0xEDB88320
#define SECOND_TABLE_CONSTANT 0x04c11db7

uint32_t CryptoCRC32::crc32_table1_[256];
uint32_t CryptoCRC32::crc32_table2_[256];
bool CryptoCRC32::table_init_ = false;

void CryptoCRC32::InitTable()
{
	uint32_t crc1 = 0;
	uint32_t crc2 = 0;

	for (int32_t i = 0; i < 256; i++)
	{
		crc1 = i;

		for (int32_t j = 0; j < 8; j++)
		{
			if (crc1 & 1)
			{
				crc1 = (crc1 >> 1) ^ FIRST_TABLE_CONSTANT;
			}
			else
			{
				crc1 = crc1 >> 1;
			}
		}

		crc32_table1_[i] = crc1;
		//----------------------------------------------------------------------
		crc2 = i;

		for (int32_t k = 0; k < 8; k++)
		{
			if (crc2 & 1)
			{
				crc2 = (crc2 >> 1) ^ SECOND_TABLE_CONSTANT;
			}
			else
			{
				crc2 = crc2 >> 1;
			}
		}

		crc32_table2_[i] = crc2;

	}

	table_init_ = true;
}

uint32_t CryptoCRC32::Encode(const uint8_t* buf, int32_t len, bool use_first_table)
{
	uint32_t ret = 0xFFFFFFFF;
	int32_t i = 0;

	if (!table_init_)
	{
		InitTable();
	}

	if (use_first_table)
	{
		for (i = 0; i < len; i++)
		{
			ret = crc32_table1_[((ret & 0xFF) ^ buf[i])] ^ (ret >> 8);
		}
	}
	else
	{
		for (i = 0; i < len; i++)
		{
			ret = crc32_table2_[((ret & 0xFF) ^ buf[i])] ^ (ret >> 8);
		}
	}

	ret = ~ret;
	return ret;
}