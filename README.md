windows河南
    
  修改日志：2021-11-17  
1.撤单处理单元，与扣款处理单元分别在独立的线程中处理。  
2.新增撤单处理单元源文件。  
3.优化初始化信息中 "站点编码#站名称" 的解析，截取站名称时，去掉#号。  
4.初始化信息当前时间根据实际发送A5指令的时间赋值，避免接收到设备的D2指令时，还是发送原来保存的初始化信息。  
5.补全异常捕捉源代码。  
