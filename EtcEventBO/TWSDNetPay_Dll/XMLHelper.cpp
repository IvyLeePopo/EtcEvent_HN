#include "XMLHelper.h"
#include "TypeHelper.h"
#include <map>
#include<list>

std::string GetXMLParseError();
struct XMLLastError
{
    CMarkup xml;
    std::string visit;
};
static std::map<int ,XMLLastError> g_XMLErrorMap;
void SetXMLParseError(CMarkup& xml, const std::string & node_name)
{
    int curr_thread_id = GetCurrentThreadId();
    g_XMLErrorMap[curr_thread_id].xml = xml;
    g_XMLErrorMap[curr_thread_id].visit = node_name;
}

bool ReadXMLStr(CMarkup& xml , const std::string& nodeName, std::string& nodeVal)
{
    xml.ResetMainPos();
    if (!xml.FindElem(nodeName.c_str()))
    {
        SetXMLParseError(xml,nodeName);
        return false;
    }
    nodeVal =  xml.GetData() ;
    return true;
}

bool ReadXMLCStr(CMarkup& xml , const std::string& nodeName, char*result,int resultLen)
{
    std::string strVal;
    if(!ReadXMLStr(xml,nodeName,strVal))
    {
        SetXMLParseError(xml,nodeName);
        return false;
    }
    strncpy(result,strVal.c_str(),resultLen);
    return true;
}
bool ReadXMLInt(CMarkup& xml , const std::string& nodeName, int& nodeVal)
{
    xml.ResetMainPos();
    if (!xml.FindElem(nodeName.c_str()))
    {
        SetXMLParseError(xml,nodeName);
        return false;
    }
    nodeVal =  _ttoi(xml.GetData()) ;
    return true;
}

bool ReadXMLTimet(CMarkup& xml , const std::string& nodeName, time_t& nodeVal)
{
    std::string time_str;
    if(!ReadXMLStr(xml,nodeName,time_str))
    {
        SetXMLParseError(xml,nodeName);
        return false;
    }
    nodeVal = Str2ChinaTime(time_str);
	return true;
}
bool IntoXMLSubNode(CMarkup& xml, const std::string& nodeName)
{
    xml.ResetMainPos();
    if (!xml.FindElem(nodeName.c_str()))
    {
        SetXMLParseError(xml,nodeName);
        return false;
    }
    xml.IntoElem();
    return true;
}
bool OutXML2Parent(CMarkup& xml)
{
    xml.OutOfElem();
    return true;
}

std::string GetXMLParseError()
{
    XMLLastError xle = g_XMLErrorMap[GetCurrentThreadId()];
	if(!xle.xml.IsWellFormed())
	{
		return "XML格式不正确";
	}
	else
	{
		std::string tag_name = (LPCSTR)xle.xml.GetTagName();
		tag_name += (LPCSTR)xle.xml.GetChildTagName();
		
		return "节点解析错误->" +xle.visit;
	}
	
	return "XML未知错误";
}


