#ifndef XML_HELPER_H
#define XML_HELPER_H
#include "Markup.h"
#include <string>

bool ReadXMLStr(CMarkup& xml , const std::string& nodeName, std::string& nodeVal);
bool ReadXMLCStr(CMarkup& xml , const std::string& nodeName, char*result,int resultLen);
bool ReadXMLInt(CMarkup& xml , const std::string& nodeName, int& nodeVal);
bool ReadXMLTimet(CMarkup& xml , const std::string& nodeName, time_t& nodeVal);

bool ReadXMLStrOrInt(CMarkup& xml , const std::string& nodeName, char * result,int resultLen);
bool IntoXMLSubNode(CMarkup& xml, const std::string& nodeName);
bool OutXML2Parent(CMarkup& xml);

std::string GetXMLParseError();

#endif // XML_HELPER_H
