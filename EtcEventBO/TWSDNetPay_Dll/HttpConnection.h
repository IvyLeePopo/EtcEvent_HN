#ifndef HTTP_CONNECTION_H
#define HTTP_CONNECTION_H

/**
 * Http����ͨѶ��
 * ����http��������������
 * ChangeRemotePeer���÷�������·��Ϣ
 * Post������Post����
 * LastHttpErrroDesc����ȡ��������ʧ������
 */
#include <afxinet.h>
#include <string>

struct HttpOption
{
    int ConnectTimeoutMsecs;
    int SendTimeoutMsecs;
    int ReceveTimeoutMsecs;
    HttpOption()
    {
        memset(this,0,sizeof(HttpOption));
    }
};


class HttpConnection
{
public:
    HttpConnection();
    ~HttpConnection();

public:
    void SetHttpOption(const HttpOption& option);
    void ChangeRemotePeer(const std::string& ip, int port);/*Զ����Ϣ*/
    bool Post(const std::string& path,const std::string& data,std::string& FeedBack);/*��������*/
    std::string LastHttpErrorDesc();

	//�����ⲿ��ӡ��Ϣ
	std::string m_IpStr;
	int m_Port;

private:
    bool OpenConn(CInternetSession& session,CHttpConnection *&httpConn,CHttpFile *&httpFile,const std::string& path);
    bool PostData(CHttpFile *&httpFile,const std::string&data);
    bool ReadResponse(CHttpFile *&httpFile,std::string&feedBack);
    bool CloseConn(CHttpConnection *&httpConn,CHttpFile *&httpFile);
private:
    std::string m_HttpErrorDesc;
	// int m_Port; //�����ⲿ��ӡ��Ϣ
	HttpOption m_Option;
	//  std::string m_IpStr;//�����ⲿ��ӡ��Ϣ


private:
    HttpConnection(const HttpConnection&);
    HttpConnection& operator = (const HttpConnection&);

};





#endif // HTTP_CONNECTION_H
