#ifndef __TINYXML_TOOL_H__
#define __TINYXML_TOOL_H__
#include "tinyxml.h"


class tinyxml_tool
{
public:
	static bool get_int_attribute(TiXmlElement* pElm, const char* name, int& v);
	static bool get_str_attribute(TiXmlElement* pElm, const char* name, char* pszval, int nszlen);
	static bool get_int64_attribute(TiXmlElement* pElm, const char* name, long long& v);
	static bool get_int_attribute(TiXmlElement* pElm, const char* name, long& v);
	static bool get_int_attribute_defaultvalue(TiXmlElement* pElm, const char* name, int& v, int DefaultValue);
    static bool get_double_attribute(TiXmlElement* pElm, const char* name, double& v);


};

#define xml_int tinyxml_tool::get_int_attribute
#define xml_str tinyxml_tool::get_str_attribute
#define xml_int64 tinyxml_tool::get_int64_attribute
#define xml_double tinyxml_tool::get_double_attribute

#endif//__TINYXML_TOOL_H__