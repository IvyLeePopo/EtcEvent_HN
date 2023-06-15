/*
* @Author: 朱亚冰
* @Date:   2018-11-16
* @Last Modified by:   windf
* @Last Modified time: 2018-11-21
*/
#include "ComponentMessage.h"

namespace trawe
{

CComponentMessage::CComponentMessage(CMessageTerminal* recv)
    : m_MsgReciver(recv)
{
    ::InitializeCriticalSection(&m_MsgAccessMutex);
}
CComponentMessage::~CComponentMessage()
{
	Stop();
    ::DeleteCriticalSection(&m_MsgAccessMutex);
}
void CComponentMessage::RunBody()
{
    MessageInfo* one_msg = 0;
    {   //take one msg
        CScopeLock msg_lock(&m_MsgAccessMutex);
        if (!m_MsgQueue.empty())
        {
            one_msg = m_MsgQueue.front();
            m_MsgQueue.pop_front();
        }
    }
    if (one_msg != 0)
    {
        if (m_MsgReciver != 0)
        {
            m_MsgReciver->ExecuteMessage(one_msg->src_key, one_msg->message, one_msg->message_len);
        }
        delete one_msg->message;
        one_msg->message = 0;
        delete one_msg;
        one_msg = 0;
    }
    Sleep(1000);
}

//  放入新的消息
void CComponentMessage::NewMessage(int destKey, char *message, int len, int srcKey)
{
    CScopeLock msg_lock(&m_MsgAccessMutex);
    if (m_MsgQueue.size() > 100)
    {
        return ;
    }
    MessageInfo* new_msg = new MessageInfo();
    if (new_msg == 0)
    {
        return;
    }
    new_msg->dest_key = destKey;
    new_msg->src_key = srcKey;
    new_msg->message_len = len;
    new_msg->message = new char[len];
    if (new_msg->message == 0)
    {
        delete new_msg;
        return ;
    }
    memcpy(new_msg->message, message, len);
    m_MsgQueue.push_back(new_msg);
}


}



