#ifndef SDK_VALIDATE_H
#define SDK_VALIDATE_H

#include <YouMeCommon/CrossPlatformDefine/PlatformDef.h>
#include <YouMeCommon/CrossPlatformDefine/IYouMeSystemProvider.h>
#include <XCondWait.h>
#include <YouMeCommon/XAny.h>
#include <YouMeCommon/pb/youme_comm_conf.pb.h>
#include <YouMeCommon/profiledb.h>
namespace youmecommon
{
	enum SDKValidateErrorcode
	{
		SDKValidateErrorcode_Success=0,
		SDKValidateErrorcode_NotInit =1,
		SDKValidateErrorcode_ValidateIng =2,
		SDKValidateErrorcode_NetworkInvalid=3,
		SDKValidateErrorcode_ILLEGAL_SDK=4,
		SDKValidateErrorcode_Abort=5,
		SDKValidateErrorcode_ProtoError=6,
		SDKValidateErrorcode_ServerError=7,
		SDKValidateErrorcode_DNSParseError=8,
		SDKValidateErrorcode_InvalidAppkey = 9,		// appkey 无效，检查平台是否配置了相应的 appky
		SDKValidateErrorcode_InvalidFailed = 10,	// SDK 验证失败，检查前后台加解密使用的 key
		SDKValidateErrorcode_Fail=1000
	};

	struct SDKInvalidateParam
	{
		int serviceID;
		YOUMEServiceProtocol::SERVICE_TYPE serviceType;
		std::string sdkName;
		int protocolVersion;
		XString domain;
        std::vector<short> port;
		std::vector<XString> defaultIP;

		SDKInvalidateParam() : serviceID(0), protocolVersion(0), port(0){}
	};

	class CRSAUtil;

	class CSDKValidate
	{
	public:
		//可以传入空，如果不需要保存ip地址的话
		CSDKValidate(IYouMeSystemProvider* pSystemProvier, CProfileDB* pProfileDB);
		//开始验证,函数是阻塞的，需要调用者开线程,只提供一个同步的接口，外部如果需要取消的话 sdkValidWait 设置信号即可
		SDKValidateErrorcode StartValidate(SDKInvalidateParam& invalidataParam, std::map<std::string, CXAny>&configurations, CXCondWait* sdkValidWait,const XString& strZone=__XT(""));

	private:
		bool InterParseSecret(const XString& strSecret,CRSAUtil& rsa);
		void EncryDecryptPacketBody(unsigned char* buffer, int bufferLen, unsigned char* key, int keyLen);
		void GenerateRandKey(unsigned char* key, int keyLen);
		SDKValidateErrorcode OnSDKValidateRsp(YOUMEServiceProtocol::CommConfRsp& rsp, std::map<std::string, CXAny>& configurations);

		static XUINT64 m_nMsgSerial;
		CProfileDB* m_pProfileDB;
		IYouMeSystemProvider* m_pSystemProvider;
	};

}

#endif
