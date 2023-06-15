/*
* @Author: 朱亚冰
* @Date:   2017-12-07
* @Last Modified by:   windf
* @Last Modified time: 2018-12-07
*/

#include "JsonHelper.h"
#include "TypeHelper.h"
#include <fstream>
#include <Windows.h>

struct JsonLastError
{
    //Json::Value node;
    std::string visit;
};

static std::map<int,JsonLastError> g_JsonErrorMap;
void SetJsonParseError(const Json::Value& node,const std::string& child_name)
{
    int curr_thread_id = GetCurrentThreadId();
   // g_JsonErrorMap[curr_thread_id].node = node;
    g_JsonErrorMap[curr_thread_id].visit = child_name;

}


bool ReadJsonInt(const Json::Value& node,const std::string& childName,int&val)
{
    if(childName.empty())
    {
        return false;
    }
    if (Json::intValue != node[childName].type())
    {
        SetJsonParseError(node,childName);
        return false;
    }
    val = node[childName].asInt();

    return true;
}
bool ReadJsonStr(const Json::Value& node,const std::string& childName,std::string&val)
{
     if(childName.empty())
    {
        return false;
    }
    if (Json::stringValue != node[childName].type())
    {
        SetJsonParseError(node,childName);
        return false;
    }
    val = node[childName].asString();

    return true;
}

bool ReadJsonSubNode(const Json::Value& node,const std::string& subNodeName,Json::Value& subNode)
{
    if(subNodeName.empty())
    {
        return false;
    }
    if(!node.isMember(subNodeName))
    {
        SetJsonParseError(node,subNodeName);
        return false;
    }
    subNode = node[subNodeName];
    return true;
}

bool ReadJsonInt(const Json::Value& node,const std::string& childName,unsigned char&val)
{
    int intVal;
    if(!ReadJsonInt(node,childName,intVal))
    {
        SetJsonParseError(node,childName);
        return false;
    }
    val = intVal;
    return true;
}
bool ReadJsonInt(const Json::Value& node,const std::string& childName,unsigned short&val)
{
    int intVal;
    if(!ReadJsonInt(node,childName,intVal))
    {
        SetJsonParseError(node,childName);
        return false;
    }
    val = intVal;
    return true;
}

bool ReadJsonBool(const Json::Value& node,const std::string& childName,bool&val)
{
    if(childName.empty())
    {
        return false;
    }
    if (Json::booleanValue != node[childName].type())
    {
        SetJsonParseError(node,childName);
        return false;
    }
    val = node[childName].asBool();

    return true;
}

bool ReadJsonChars(const Json::Value& node,const std::string& childName,char*val,size_t val_size)
{
    if(val_size<2)
    {
        return false;
    }
    std::string strVal;
    if(!ReadJsonStr(node,childName,strVal))
    {
        SetJsonParseError(node,childName);
        return false;
    }
    strncpy(val,strVal.c_str(),val_size-1);
    return true;
}


bool ReadJsonStrOrInt(const Json::Value& node,const std::string& childName,char* val,size_t val_size)
{
	if(val_size<2)
    {
        return false;
    }
    std::string strVal;
	int int_val;
    if(ReadJsonStr(node,childName,strVal))
    {
		strncpy(val,strVal.c_str(),val_size-1);
        return true;
    }
	else if(ReadJsonInt(node,childName,int_val))
	{
		_snprintf(val,val_size-1,"%d",int_val);
		return true;
	}
	else
	{
        SetJsonParseError(node,childName);
		return false;
	}
 
    return true;
}

bool ReadJsonStrArray(const Json::Value& node ,const std::string& childName,std::vector<std::string> &array)
{
    Json::Value array_node = node[childName];
	if(array_node.type()!=Json::arrayValue)
    {
        SetJsonParseError(node,childName);
        return false;
    }
    for(int i=0;i<array_node.size();++i)
    {
        array.push_back(array_node[i].asString());
    }
    return true;
}
bool ReadJsonTimet(const Json::Value& node,const std::string& childName,time_t&val)
{
    std::string time_str;
    if(!ReadJsonStr(node,childName,time_str))
    {
        SetJsonParseError(node,childName);
        return false;
    }
    val = Str2ChinaTime(time_str);
    return true;
}

std::map<std::string,std::string> ReadJsonMapFromFile(const std::string& file_path)
{
    std::map<std::string,std::string> result;
    std::ifstream config_stream(file_path.c_str());
    if (!config_stream.is_open())
    {
        return result;
    }
    std::istreambuf_iterator<char> beg(config_stream), end;
    std::string dll_cfg_str(beg, end);
    config_stream.close();
    
    Json::Reader json_reader;
    Json::Value root_node;
    if (!json_reader.parse(dll_cfg_str, root_node))
    {
        return result;
    }

    Json::Value::Members json_mem_list = root_node.getMemberNames();
    for (Json::Value::Members::const_iterator it = json_mem_list.begin(); it != json_mem_list.end(); ++it)
    {
        if (root_node[*it].type() == Json::stringValue)
        {
            result[*it] = root_node[*it].asString();
        }
    }
    return result;

}

std::string GetJsonParseError()
{
    JsonLastError jfe = g_JsonErrorMap[GetCurrentThreadId()];
	/*if(jfe.node.type() == Json::nullValue)
	{
		return "Parse OK";
	}*/
	return "节点" +jfe.visit + "解析错误";
	
}
