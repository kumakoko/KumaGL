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

	file.seekg(0, std::ios::end); //�Ƶ�β��
	fileLen = file.tellg();

	if (!autoBuffer.Allocate((uint32_t)fileLen, true))
		return false;

	file.seekg(0, std::ios::beg);
	file.read(autoBuffer, fileLen); // ��������

	uint32_t encryptoKey = (uint32_t)(rand() % 255);		// �������һ����Կ��
	CryptoXOR::Encode(autoBuffer, (uint8_t)encryptoKey);

	file.seekp(0, std::ios::beg);							// д����ܺ������
	file.write(kEncryptSignature, kEncryptSignatureLen);	// ��д���ʶ�ַ���
	file.write((char*)&encryptoKey, kEncryptKeyLen);						// ��д����Կ
	file.write(autoBuffer.GetBuffer(), fileLen);
	file.close();											// �������ܺ���ļ�����Ҫ��δ���ܵ��ļ�����4���ֽڵ�

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

	file.seekg(0, std::ios::end); //�Ƶ�β��
	fileLen = (uint32_t)file.tellg();

	if (fileLen <= kEncryptInfoLen)
		return false;

	fileLen -= kEncryptInfoLen;

	if (!autoBuffer.Allocate(fileLen, true))
		return false;

	file.seekg(kEncryptSignatureLen, std::ios::beg);
	file.read((char*)&key, kEncryptKeyLen);			// ����key
	file.read(autoBuffer, fileLen);					// ��������
	file.close();

	CryptoXOR::Decode(autoBuffer, (uint8_t)key);

	file.open(fileName, std::ios::out | std::ios::trunc | std::ios::binary);

	if (!file.is_open())
		return false;

	file.seekp(0, std::ios::beg);					// д����ܺ������
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

	file.seekg(0, std::ios::end); //�Ƶ�β��
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
	file.read((char*)&key, 4);	// ����key
	file.read(autoBuffer, fileLen);		// ��������
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

	file.seekg(0, std::ios::end); //�Ƶ�β��
	fileLen = (uint32_t)file.tellg();

	if (fileLen < kEncryptInfoLen) // ���Ȳ������϶���δ���ܵ�
	{
		encrypted = false;
	}
	else
	{
		// ���벢���ǰ10���ֽ�
		file.seekg(0, std::ios::beg); //�Ƶ�β��
		file.read(encryptSignature, sizeof(char)*kEncryptSignatureLen);
		encrypted = strcmp(encryptSignature, kEncryptSignature) ? false : true;
	}

	if (crc32) // Ҫ��������crc32��
	{
		autoBuffer.Allocate(fileLen, true);
		file.seekg(0, std::ios::beg); // �Ƶ�ͷ��
		file.read(autoBuffer, fileLen);
		*crc32 = CryptoCRC32::Encode(autoBuffer.GetUnsignedBuffer(), (int32_t)fileLen, true);
	}

	if (encryptedKey && encrypted)
	{
		uint32_t key = 0;
		file.seekg(kEncryptSignatureLen, std::ios::beg); // �Ƶ���Կ��
		file.read((char*)&key, kEncryptKeyLen);
		*encryptedKey = (uint8_t)key;
	}

	file.close();
	return true;
}