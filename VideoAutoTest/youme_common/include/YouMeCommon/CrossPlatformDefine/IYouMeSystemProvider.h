//
//  Header.h
//  YouMeIM
//
//  Created by zxxing on 16/5/12.
//
//

#ifndef YOUME_PACKET_HEAD_INFO_20160512__H
#define YOUME_PACKET_HEAD_INFO_20160512__H

#include "PlatformDef.h"

class IYouMeSystemProvider
{
public:
	virtual ~IYouMeSystemProvider()
	{

	}
public:
	//set 接口，一些通用的字段
	virtual void setPackageName(const XString& strPackageName)
	{
		m_strPackageName = strPackageName;
	}
	virtual void setSDKVersion(int iSDKVer)
	{
		m_iSDKVer = iSDKVer;
	}
	virtual void setAppKey(const XString& strAppKey)
	{
		m_strAppKey = strAppKey;
	}
	virtual void setAppSecret(const XString& strAppSecret)
	{
		m_strAppSecrit = strAppSecret;
	}
	virtual void setAudioCachePath(const XString& path)
	{
		m_strAudioCachePath = path;
	}
	//get 接口
    virtual XString getBrand () = 0;
    virtual XString getSystemVersion () = 0;
    virtual XString getCpuArchive () = 0;
	virtual XString getPackageName()
	{
		return m_strPackageName;
	}
    virtual XString getUUID () = 0;
    virtual XString getModel () = 0;
    virtual XString getCpuChip () = 0;
    virtual XString getDocumentPath () = 0;
	virtual XString getCachePath() = 0;
	virtual XString getAppKey()
	{
		return m_strAppKey;
	}
	virtual XString getAppSecret()
	{
		return m_strAppSecrit;
	}
	virtual int getSDKVersion()
	{
		return m_iSDKVer;
	}
	virtual XString getAudioCachePath()
	{
		return m_strAudioCachePath;
	}
private:
	XString m_strPackageName;
	XString m_strAppKey;
	XString m_strAppSecrit;
	XString m_strAudioCachePath;
	int m_iSDKVer=0;
};

#endif /* YOUME_PACKET_HEAD_INFO_20160512__H */
