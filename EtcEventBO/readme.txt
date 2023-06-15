日期:2019-03-04
-----------------------------------------
更新内容:
1、修改PayUnit.cpp行197，如果需要撤单，使用扣款参数里面支付标识

日期:2019-02-22
-----------------------------------------
更新内容：
1、RunParameter中增加GetDuplicateDebitShortestMinutes函数，用于获取重复交易有效期配置。
2、ETCPayUnit中增加NewDebitAbstract、QueryDebitAbstract方法，用于支持重复交易有效期判断。
3、ETCPayUnit.cpp增加299-306行，在收到交易请求后，会判断该车牌最近是否交易过。
4、ETCPayUnit.cpp增加500-507行，在收到服务器交易回馈后，如果交易是成功交易，则缓存该信息。


日期:2019-01-29
-----------------------------------------
更新内容：
1、增加PayHelper/PayUnit/PayManage/PayHttpSync，用于1.5支付逻辑调整
2、ThreadManage模块增加StartNewPay ,QueryCurrentPayState ,CancelCurrentPay ,CancelTargetPay ,GetCurrentDebitResult ,InitAgencyPay ,QueryVirtualCardDebit ,AgencyDebitQuery ,TransferImage ,SetAgencyPara函数，用于支持1.5支付逻辑调整
3、TWSDNetPay_Dll模块增加IF_DebitMoney_Future,IF_DebitCancel_Future,IF_GetDebitResult_Future支持1.5支付逻辑调整。
4、DataBaseHelper.h/DataBaseHelper.cpp中增加DataBaseAdd ,DataBaseQuery ,DataBaseDelete支持1.5新的撤单缓存操作。
5、修改ThreadDevice.cpp第764-772行，补充对NFC的扩展细节展示。
6、修改ETCPayUnit中的HasUnrecoverableError,OccurSystemFatalError，去掉了锁控制，以处理多线程访问长时间等待。也可以通过单独的系统错误锁来修复，此处考虑不必要。
7、修改ETCPayUnit.h，增加m_DataBaseLock，表示数据库访问锁。
8、修改ETCPayUnit.cpp，将SaveDebitRecord，MoveDebitRequestFromIOToDataBase以及309行、366行的锁控制，使用单独的数据库锁替换原有的支付锁。
9、修改SystemDiary模块，增加CFunctionCallTrace，用于根据过程调用。
10、删除ETCPayHelper中GetTraceMsgHeader，CInterfaceTraceInfo以及日志操作相关宏定义。统一在SystemDiary模块实现此功能。
11、修改ETCPayUnit.cpp中的SendOneDebit，如果发生系统错误，则不再发生调用。如果发生网络错误或者云端解析失败，则会休眠5秒重发，而不是立即重发
12、修改HttpConnection网络处理模块，增加SetHttpOption函数，用于设置发送参数。
13、DataBaseHelper.cpp中，数据库操作函数，增加处理，跟踪数据库操作耗时。

-----------------------------------------

日期:2019-01-22
更新内容：
1、修改ETCPayUnit.cpp中的286-298行，如果发生系统故障，不再处理任何数据
2、修改ETCPayUnit.cpp中的第332，339行，如果发生网络错误，进行下次请求前休眠5秒
-----------------------------------------

日期:2019-01-22
更新内容：
1、修改文件TWSDNetPay_Dll.cpp第4917-4920行，增加天津虚拟卡代扣中的支付标识和订单号解析。
2、修改RunParameter.cpp中的274行、294行，增加ETC单独IP和端口配置。
-----------------------------------------

日期:2019-01-17
更新内容：
1、修改文件ETCPayUnit.h/ETCPayUnit.cpp，增加方法MoveDebitRequestFromIOToDataBase，用于将IO中的扣款请求转移到数据库。
2、修改文件ETCPayUnit.h/ETCPayUnit.cpp，增加方法SaveDebitRequestToIO，用于将扣款请求参数先保存到IO中。
3、修改文件TWSDNetPay_Dll.cpp中的5539、5549-5551行，增加日志，方便测试人员查看结果。
-----------------------------------------

日期:2019-01-15
更新内容:
1、修改文件ETCPayHelper.h/ETCPayHelper.cpp 增加TW_DEBUG_VARIABLE，用于支持调试变量
2、修改文件ETCPayHelper.cpp中ParseOBUWhiteListRecordFromServer的实现，增加QueryIdentifier
的解析判断。
3、修改文件ETCPayHelper.cpp的587-596行
4、修改文件ETCPayUnit.cpp中的491-496行，修复检测时间判断问题。
5、修改文件TWSDNetPay_Dll.cpp,增加QueryAgentDebitEtcUp_Stake ,QueryAgentDebitResultEtcUp_Stake, DebitMoneyEtcUp_Stake,提供给甘肃模拟测试。
6、修改文件TypeHelper.cpp中的GetFileModifyTime，修改了CreatFile的调用参数，使其只打开存在的文件。


-----------------------------------------
日期:2019-01-11
更新内容：
1、新增文件ETCAuthenticateUnit.cpp\ETCAuthenticateUnit.h，用于处理甘肃ETC白名单鉴权
2、新增文件ETCPayHelper.cpp\ETCPayHelper.h，用于辅助住处ETC函数转换
3、新增文件ETCPayManage.cpp\ETCPayManage.h，提供甘肃ETC的完整业务功能
4、新增文件ETCPayUnit.cpp\ETCPayUnit.h，甘肃ETC支付功能和支付结果查询功能
5、新增文件RunParameter.cpp\RunParameter.h，系统运行参数配置
6、新增文件DataBaseHelper.cpp\DataBaseHelper.h，数据库辅助处理功能，增删改查。
-----------------------------------------
日期:2018-10-26
更新内容：
1、对天津虚拟卡的新需求做出了支持
2、增加了对支付模式的支持。
-----------------------------------------
日期:2018-09-30
更新内容：
1、增加了对天津虚拟卡的支持，扣款结果新增了ETC执行码字段。