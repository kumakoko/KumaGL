#include <iosfwd>
#include <fstream>
#include <ctime>
#include "crypto_xor.h"
#include "crypto_crc32.h"
#include "simple_file_crypto.h"

static const char* kEncryptSignature = "GGSEFILE";
static const uint32_t kEncryptSignatureLen = 8;	// strlen(kEncryptSignature)
static const uint32_t kEncryptKeyLen = 4;			// sizeof(uint32_t)
static const uint32_t kEncryptInfoLen = 12;			// kEncryptSignatureLen + kEncryptKeyLen

bool SimpleFileCrypto::encryptFileInPlace(const char* fileName, AutoBuffer& autoBuffer)
{
	std::fstream file;
	std::fstream::pos_type fileLen;

	file.open(fileName, std::ios::in | std::ios::out | std::ios::binary);

	if (!file.is_open())
		return false;

	file.seekg(0, std::ios::end); //移到尾部
	fileLen = file.tellg();

	if (!autoBuffer.Allocate((uint32_t)fileLen, true))
		return false;

	file.seekg(0, std::ios::beg);
	file.read(autoBuffer, fileLen); // 读入数据

	uint32_t encryptoKey = (uint32_t)(rand() % 255);		// 随机生成一个密钥，
	CryptoXOR::Encode(autoBuffer, (uint8_t)encryptoKey);

	file.seekp(0, std::ios::beg);							// 写入加密后的数据
	file.write(kEncryptSignature, kEncryptSignatureLen);	// 先写入标识字符串
	file.write((char*)&encryptoKey, kEncryptKeyLen);						// 再写入密钥
	file.write(autoBuffer.GetBuffer(), fileLen);
	file.close();											// 经过加密后的文件，是要比未加密的文件，多4个字节的

	return true;
}

bool SimpleFileCrypto::decryptFileInPlace(const char* fileName, AutoBuffer& autoBuffer)
{
	std::fstream	file;
	uint32_t		fileLen = 0;
	uint32_t		key = 0;


	file.open(fileName, std::ios::in | std::ios::binary);

	if (!file.is_open())
		return false;

	file.seekg(0, std::ios::end); //移到尾部
	fileLen = (uint32_t)file.tellg();

	if (fileLen <= kEncryptInfoLen)
		return false;

	fileLen -= kEncryptInfoLen;

	if (!autoBuffer.Allocate(fileLen, true))
		return false;

	file.seekg(kEncryptSignatureLen, std::ios::beg);
	file.read((char*)&key, kEncryptKeyLen);			// 读入key
	file.read(autoBuffer, fileLen);					// 读入数据
	file.close();

	CryptoXOR::Decode(autoBuffer, (uint8_t)key);

	file.open(fileName, std::ios::out | std::ios::trunc | std::ios::binary);

	if (!file.is_open())
		return false;

	file.seekp(0, std::ios::beg);					// 写入解密后的数据
	file.write(autoBuffer.GetBuffer(), fileLen);
	file.close();

	return true;
}

bool SimpleFileCrypto::decryptFile(const char* fileName, AutoBuffer& autoBuffer)
{
	std::fstream	file;
	uint32_t		fileLen = 0;
	uint32_t		key = 0;

	file.open(fileName, std::ios::in | std::ios::binary);

	if (!file.is_open())
	{
		file.close();
		return false;
	}

	file.seekg(0, std::ios::end); //移到尾部
	fileLen = (uint32_t)file.tellg();

	if (fileLen < kEncryptInfoLen)
	{
		file.close();
		return false;
	}

	fileLen -= kEncryptInfoLen;

	if (!autoBuffer.Allocate(fileLen, true))
	{
		file.close();
		return false;
	}

	file.seekg(kEncryptSignatureLen, std::ios::beg);
	file.read((char*)&key, 4);	// 读入key
	file.read(autoBuffer, fileLen);		// 读入数据
	file.close();

	CryptoXOR::Decode(autoBuffer, (uint8_t)key);
	return true;
}

bool SimpleFileCrypto::checkFileEncryptAndCRC32(const char* fileName, AutoBuffer& autoBuffer, bool& encrypted, uint32_t* crc32, uint8_t* encryptedKey)
{
	std::ifstream file;
	std::fstream::pos_type fileLen;
	char encryptSignature[kEncryptSignatureLen + 1] = { 0 };

	file.open(fileName, std::ios::in | std::ios::binary);

	if (!file.is_open())
		return false;

	file.seekg(0, std::ios::end); //移到尾部
	fileLen = (uint32_t)file.tellg();

	if (fileLen < kEncryptInfoLen) // 长度不够，肯定是未加密的
	{
		encrypted = false;
	}
	else
	{
		// 读入并检查前10个字节
		file.seekg(0, std::ios::beg); //移到尾部
		file.read(encryptSignature, sizeof(char)*kEncryptSignatureLen);
		encrypted = strcmp(encryptSignature, kEncryptSignature) ? false : true;
	}

	if (crc32) // 要计算它的crc32码
	{
		autoBuffer.Allocate(fileLen, true);
		file.seekg(0, std::ios::beg); // 移到头部
		file.read(autoBuffer, fileLen);
		*crc32 = CryptoCRC32::Encode(autoBuffer.GetUnsignedBuffer(), (int32_t)fileLen, true);
	}

	if (encryptedKey && encrypted)
	{
		uint32_t key = 0;
		file.seekg(kEncryptSignatureLen, std::ios::beg); // 移到密钥处
		file.read((char*)&key, kEncryptKeyLen);
		*encryptedKey = (uint8_t)key;
	}

	file.close();
	return true;
}