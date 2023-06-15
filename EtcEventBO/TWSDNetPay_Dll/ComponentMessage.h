#ifndef COMPONENT_MESSAGE_H
#define COMPONENT_MESSAGE_H

#include "StandTask.h"
#include <list>

namespace trawe {
   
    class CMessageTerminal;
    struct MessageInfo;
    class CComponentMessage : public CStandTask
    {
    public:
        CComponentMessage(CMessageTerminal* recv);
        virtual ~CComponentMessage();
    public:
        virtual void RunBody();
    public:
        void NewMessage(int destKey, char *message, int len, int srcKey);
    private:
        CRITICAL_SECTION m_MsgAccessMutex;
        std::list<MessageInfo*> m_MsgQueue;
         CMessageTerminal* const m_MsgReciver;
    private:
        CComponentMessage(const CComponentMessage&);
        CComponentMessage& operator = (const CComponentMessage&);
    
    };

    class CMessageTerminal
    {
    public:
        CMessageTerminal(){}
        virtual ~CMessageTerminal(){}
    public:
        virtual int ExecuteMessage(int src_key,char* msg,int msg_len){return 0;}
    private:
        CMessageTerminal(const CMessageTerminal&);
        CMessageTerminal& operator = (const CMessageTerminal&);
    };
    struct MessageInfo
    {
        int dest_key;
        int src_key;
        char* message;
        int message_len;
        MessageInfo()
        {
            memset(this,0,sizeof(MessageInfo));
        }
    };

} // name


#endif // COMPONENT_MESSAGE_H
/**
 * 消息处理
 */