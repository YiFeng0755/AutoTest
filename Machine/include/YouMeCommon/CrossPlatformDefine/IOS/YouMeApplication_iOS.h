//
//  Application.h
//  YouMeVoiceEngine
//
//  Created by wanglei on 15/9/17.
//  Copyright (c) 2015年 tencent. All rights reserved.
//
#include <map>
#include <YouMeCommon/CrossPlatformDefine/IYouMeSystemProvider.h>

class YouMeApplication_iOS: public IYouMeSystemProvider
{
public:
    YouMeApplication_iOS();
public:
    virtual XString getBrand ();
    virtual XString getSystemVersion ();
    virtual XString getCpuArchive ();
    virtual XString getPackageName ();
    virtual XString getUUID ();
    virtual XString getModel ();
    virtual XString getCpuChip ();
    virtual XString getDocumentPath ();
    virtual XString getCachePath();

private:
    XString getBundleValueByName (const XString &name);

    std::map<std::string,std::string> m_deviceNameMap;
};
