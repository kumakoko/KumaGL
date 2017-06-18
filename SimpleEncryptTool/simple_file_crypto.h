#include "type_define.h"
#include "auto_buffer.h"

class SimpleFileCrypto
{
public:
	/**************************************************************************
	* @brief encryptFileInPlace
	*
	* ��ԭʼ�ļ����С��͵ؼ򵥼��ܡ����ڼ���ʱ������д��һ��10�ֽڳ��ļ��ܱ�־�ַ���
	* IVGENCRYPT,Ȼ����������һ��32λ�Ĵ�0��255���������Ϊ������Կ�����Ҵ���Կ����
	* ������д����ܺ���ļ����ļ�ͷ,Ҳ����˵���ܺ���ļ������ԭʼ�ļ���14���ֽڡ���
	* �ҽ����ԭʼ�ļ�����ɼ��ܺ���ļ�
	* @param[in] fileName		Ҫ���ܵ��ļ���
	* @param[in] autoBuffer		�������н����õ�autoBuffer
	* @return bool				���ܳɹ�����true��ʧ�ܷ���false
	**************************************************************************/
	static bool encryptFileInPlace(const char* fileName, AutoBuffer& autoBuffer);

	/**************************************************************************
	* @brief decryptFileInPlace
	*
	* ��ԭʼ�ļ����С��͵ؼ򵥽��ܡ����ļ�ͷ��ǰ�ĸ��ֽڽ��ᱻ��Ϊ�Ǽ�����Կ��Ҳ��
	* ��˵�𣬽��ܺ���ļ���ԭʼ�ļ���14���ֽڣ����һ��ԭʼ�ļ�����ɽ��ܺ���ļ�
	* @param[in] fileName		Ҫ���ܵ��ļ���
	* @param[in] autoBuffer		�������н����õ�autoBuffer
	* @return bool				���ܳɹ�����true��ʧ�ܷ���false
	**************************************************************************/
	static bool decryptFileInPlace(const char* fileName, AutoBuffer& autoBuffer);

	/**************************************************************************
	* @brief decryptFile
	*
	* ��һ���ļ����н��ܣ�Ȼ��ѽ��ܺ�Ľ���ŵ�autoBuffer����Ļ�������ȥ
	* @param[in] fileName
	* @param[in] autoBuffer
	* @return bool
	**************************************************************************/
	static bool decryptFile(const char* fileName, AutoBuffer& autoBuffer);

	/**************************************************************************
	* @brief checkFileEncryptAndCRC32
	*
	*  ���һ���ļ��Ƿ񱻼򵥼��ܣ����һ�ȡ���ļ���CRC32У���
	* @param[in] fileName		Ҫ�����ļ���
	* @param[in] autoBuffer		�������м���õ�autoBuffer
	* @param[in] encrypted		���ļ��Ƿ������
	* @param[in] crc32			���ر��ļ���crc32ֵ
	* @param[in] encryptedKey	������ļ������ˣ����ر��ļ�����Կ
	* @return bool
	**************************************************************************/
	static bool checkFileEncryptAndCRC32(const char* fileName, AutoBuffer& autoBuffer, bool& encrypted, uint32_t* crc32, uint8_t* encryptedKey);
};