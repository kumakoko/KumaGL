/**************************************************************************
* @file StringTool.h
* @brief 
* @author Xiong Xinke
* @date 2013/3/19 14:24:08
* @version 
*************************************************************************/
#ifndef string_tool_h__
#define string_tool_h__

class StringTool
{
public:
	/*
	* @brief TrimLeft 
	* 
	* 如果最左边的字符是target，就清理掉它
	* @param[in] str			要清理的字符串
	* @param[in] target			要清理的字符
	*/
	static void TrimLeft(std::string& str,char target);

	/*
	* @brief TrimLeft 
	* 
	* Detailed description.
	* @param[in] str 
	* @param[in] target_str 
	*/
	static void TrimLeft(std::string& str, const char* target_str);

	/*
	* @brief TrimRight 
	* 
	* 如果最右边的字符是target，就清理掉它
	* @param[in] str			要清理的字符串
	* @param[in] target			要清理的字符
	*/
	static void TrimRight(std::string& str, char target);

	/*
	* @brief TrimRight 
	* 
	* Detailed description.
	* @param[in] str 
	* @param[in] target_str 
	*/
	static void TrimRight(std::string& str, const char* target_str);

	/*
	* @brief StringTool::LeftSubString 
	* 
	* Detailed description.
	* @param[in] str 
	* @param[in] count 
	* @return std::string  
	*/
	static std::string LeftSubString(const std::string& str, std::string::size_type count);

	/*
	* @brief RightSubString 
	* 
	* Detailed description.
	* @param[in] str 
	* @param[in] count 
	* @return std::string  
	*/
	static std::string RightSubString(const std::string& str, std::string::size_type count);

	/*
	* @brief MidSubString 
	* 
	* Detailed description.
	* @param[in] str 
	* @param[in] first 
	* @return std::string  
	*/
	static std::string MidSubString(const std::string& str,std::string::size_type first);

	/*
	* @brief MidSubString 
	* 
	* Detailed description.
	* @param[in] str 
	* @param[in] first 
	* @param[in] count 
	* @return std::string  
	*/
	static std::string MidSubString(const std::string& str, int first, int count);

	/*
	* @brief CopyToBuffer 
	* 
	* Detailed description.
	* @param[in] str 
	* @param[in] copy_size 
	* @return char*  
	*/
	static char* CopyToBuffer(const std::string& str, uint32_t* copy_size);
};

#endif // string_tool_h__