#include "type_define.h"
#include "auto_buffer.h"

class SimpleFileCrypto
{
public:
	/**************************************************************************
	* @brief encryptFileInPlace
	*
	* 对原始文件进行【就地简单加密】，在加密时，首先写入一个10字节长的加密标志字符串
	* IVGENCRYPT,然后会随机生成一个32位的从0到255的随机数作为加密密钥，并且此密钥将会
	* 紧接着写入加密后的文件的文件头,也就是说加密后的文件将会比原始文件多14个字节。并
	* 且将会把原始文件代替成加密后的文件
	* @param[in] fileName		要加密的文件名
	* @param[in] autoBuffer		用来进行解密用的autoBuffer
	* @return bool				加密成功返回true，失败返回false
	**************************************************************************/
	static bool encryptFileInPlace(const char* fileName, AutoBuffer& autoBuffer);

	/**************************************************************************
	* @brief decryptFileInPlace
	*
	* 对原始文件进行【就地简单解密】，文件头的前四个字节将会被视为是加密密钥，也就
	* 是说吗，解密后的文件比原始文件少14个字节，并且会把原始文件代替成解密后的文件
	* @param[in] fileName		要解密的文件名
	* @param[in] autoBuffer		用来进行解密用的autoBuffer
	* @return bool				加密成功返回true，失败返回false
	**************************************************************************/
	static bool decryptFileInPlace(const char* fileName, AutoBuffer& autoBuffer);

	/**************************************************************************
	* @brief decryptFile
	*
	* 把一个文件进行解密，然后把解密后的结果放到autoBuffer管理的缓冲区中去
	* @param[in] fileName
	* @param[in] autoBuffer
	* @return bool
	**************************************************************************/
	static bool decryptFile(const char* fileName, AutoBuffer& autoBuffer);

	/**************************************************************************
	* @brief checkFileEncryptAndCRC32
	*
	*  检测一个文件是否被简单加密，并且获取此文件的CRC32校验和
	* @param[in] fileName		要检查的文件名
	* @param[in] autoBuffer		用来进行检查用的autoBuffer
	* @param[in] encrypted		本文件是否加密了
	* @param[in] crc32			返回本文件的crc32值
	* @param[in] encryptedKey	如果本文件加密了，返回本文件的密钥
	* @return bool
	**************************************************************************/
	static bool checkFileEncryptAndCRC32(const char* fileName, AutoBuffer& autoBuffer, bool& encrypted, uint32_t* crc32, uint8_t* encryptedKey);
};