#ifndef JSON_HELPER_H
#define JSON_HELPER_H
#include "json.h"
#include <map>

bool ReadJsonInt(const Json::Value& node,const std::string& childName,int&val);
bool ReadJsonInt(const Json::Value& node,const std::string& childName,unsigned char&val);
bool ReadJsonInt(const Json::Value& node,const std::string& childName,unsigned short&val);
bool ReadJsonBool(const Json::Value& node,const std::string& childName,bool&val);
bool ReadJsonTimet(const Json::Value& node,const std::string& childName,time_t&val);

bool ReadJsonStrOrInt(const Json::Value& node,const std::string& childName,char* val,size_t val_size);

bool ReadJsonStrArray(const Json::Value& node ,const std::string& childName,std::vector<std::string> &array);

bool ReadJsonStr(const Json::Value& node,const std::string& childName,std::string&val);
bool ReadJsonChars(const Json::Value& node,const std::string& childName,char*val,size_t val_size);
bool ReadJsonSubNode(const Json::Value& node,const std::string& subNodeName,Json::Value& subNode);


std::string GetJsonParseError();
std::map<std::string,std::string> ReadJsonMapFromFile(const std::string& file_path);


#endif // JSON_HELPER_H
