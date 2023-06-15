#ifndef UNI_QR_PROTOCOL_H
#define UNI_QR_PROTOCOL_H
#include <list>
#include <vector>
#include <string>

#define COMITEM_MAXSIZE  20
#define COMDATA_MAXLEN 5120

#define MAKEINTBIG(fir,sec,thir,four) ( MAKELONG(MAKEWORD((four),(thir)),MAKEWORD((sec),(fir) )) )
#define MAKEWORDBIG(fir,sec) ( MAKEWORD((sec),(fir)) )

struct ACKItem
{
    unsigned short id;
    int state;
};


struct UniQrDriver
{
    std::list<unsigned char> uncomplete_datas;
    std::list<ACKItem> ack_group;
    unsigned short next_send_id;
    unsigned int device_state;
    std::string serial_no;
    std::string last_qrcode;
    bool initdev_flag;
	bool isEtcEventResp;
	unsigned short EtcEventId;
	char EtcEventRespContent[2*1024];
};

//ETC特情处理接口
std::string PackDataInitTerminal(unsigned short id);
std::string PackDataEventDealStart(const char* ParamContext, int iParamSize, unsigned short id);
std::string PackDataEventCheckVehQueue(const char* ParamContext, int iParamSize, unsigned short id);
std::string PackDataEventCheckVehInfo(const char* ParamContext, int iParamSize, unsigned short id);
std::string PackDataEventCheckEntryInfo(const char* ParamContext, int iParamSize, unsigned short id);
std::string PackDataEventShowFeeInfo(const char* ParamContext, int iParamSize, unsigned short id);
std::string PackDataEventPayResultShow(const char* ParamContext, int iParamSize, unsigned short id);
std::string PackDataEventStartScan(int iFeeValue, unsigned short id);
std::string PackDataEventStopScan(unsigned short id);
std::string PackDataEventDealStop(const char* ParamContext, int iParamSize, unsigned short id);
std::string PackDataEventCardOperationNotify(const char* ParamContext, int iParamSize, unsigned short id);
std::string PackDataEventModifyVehQueue(const char* ParamContext, int iParamSize, unsigned short id);
std::string PackDataEventFeeAuthorize(const char* ParamContext, int iParamSize, unsigned short id);
std::string PackDataEventAuthorize(const char* ParamContext, int iParamSize, unsigned short id);
std::string PackDataEventDelVehQueueResult(const char* ParamContext, int iParamSize, unsigned short id);

//读取设备序列号
std::string PackDataDeviceNo(const char* ParamContext, int iParamSize, unsigned short id);

std::string PackDataQRcodeDisplay(const char* ParamContext, int iParamSize, unsigned short id);
std::string PackDataFeeDisplay(const char* ParamContext, int iParamSize, unsigned short id);
std::string PackDataPayResultDisplay(const char* ParamContext, int iParamSize, unsigned short id);
std::string PackDataClearScreen(const char* ParamContext, int iParamSize, unsigned short id);

bool CheckTargetACK(const UniQrDriver& urd,unsigned short id);

void ParseProtocolData(UniQrDriver& uqd,std::vector<std::string>& c0_queue);
void ParseProtocolACK(const std::vector<unsigned char>&ack_data,UniQrDriver& uqd);
void ParseProtocolHeart(const std::vector<unsigned char>&heart_data,UniQrDriver& uqd);


//特情处理应答
void ParseProtocolEventDealStartResp(const std::vector<unsigned char>&qr_data, UniQrDriver& uqd);
void ParseProtocolEventCheckVehQueueResp(const std::vector<unsigned char>&qr_data, UniQrDriver& uqd);
void ParseProtocolEventCheckVehInfoResp(const std::vector<unsigned char>&qr_data, UniQrDriver& uqd);
void ParseProtocolEventCheckEntryInfoResp(const std::vector<unsigned char>&qr_data, UniQrDriver& uqd);
void ParseProtocolEventShowFeeInfoResp(const std::vector<unsigned char>&qr_data, UniQrDriver& uqd);
void ParseProtocolEventPayResultShowResp(const std::vector<unsigned char>&qr_data, UniQrDriver& uqd);
void ParseProtocolEventDealStopResp(const std::vector<unsigned char>&qr_data, UniQrDriver& uqd);
void ParseProtocolStartScanResp(const std::vector<unsigned char>&qr_data, UniQrDriver& uqd);
void ParseProtocolStopScanResp(const std::vector<unsigned char>&qr_data, UniQrDriver& uqd);
void ParseProtocolAuditResp(const std::vector<unsigned char>&qr_data, UniQrDriver& uqd);
void ParseProtocolEventCardOperationNotifyResp(const std::vector<unsigned char>&qr_data, UniQrDriver& uqd);
void ParseProtocolEventModifyVehQueueResp(const std::vector<unsigned char>&qr_data, UniQrDriver& uqd);
void ParseProtocolEventFeeAuthorize(const std::vector<unsigned char>&qr_data, UniQrDriver& uqd);
void ParseProtocolEventButtonClicked(const std::vector<unsigned char>&qr_data, UniQrDriver& uqd);
void ParseProtocolEventAuthorize(const std::vector<unsigned char>&qr_data, UniQrDriver& uqd);
void ParseProtocolEventDelVehQueueCmd(const std::vector<unsigned char>&qr_data, UniQrDriver& uqd);
void ParseProtocolEventDelVehQueueResultResp(const std::vector<unsigned char>&qr_data, UniQrDriver& uqd);



//解析设备序列号
void ParseProtocolDeviceNo(const std::vector<unsigned char>&qr_data, UniQrDriver& uqd);

CString ParseQRcode(char *szParamContext);

bool TestBit(int n,int pos);

bool EscapeComProtocol(const std::vector<unsigned char>& src, std::vector<unsigned char>&dst ); //非手动申请内存形式
bool UnEscapeComProtocol(const std::vector<unsigned char>& src, std::vector<unsigned char>&dst ); //非手动申请内存形式

std::string Bin2Hex(const std::vector<unsigned char>& src,bool with_space=true);
unsigned short crc16(const std::vector<unsigned char>::const_iterator& it_first,const std::vector<unsigned char>::const_iterator& it_second);
std::string GetProviceNameByID(unsigned short id);

std::string modifyA5CurrentTime(std::string A5Json);
bool Hex2Bin(const char* hex_buf,char* bin_buf,size_t hex_len);

#endif // UNI_QR_PROTOCOL_H
