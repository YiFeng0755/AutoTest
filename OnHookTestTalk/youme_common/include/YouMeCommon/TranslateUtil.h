#pragma once
#include <YouMeCommon/CrossPlatformDefine/PlatformDef.h>

/************************************************************************/
/*
g_mapLanguage["en"] = __XT("英语+英国");
g_mapLanguage["en-us"] = __XT("英语+美国");
g_mapLanguage["ar"] = __XT("阿拉伯语+");
g_mapLanguage["ar-ae"] = __XT("阿拉伯语+阿拉伯联合酋长国");
g_mapLanguage["ar-bh"] = __XT("阿拉伯语+巴林");
g_mapLanguage["ar-dz"] = __XT("阿拉伯语+阿尔及利亚");
g_mapLanguage["ar-eg"] = __XT("阿拉伯语+埃及");
g_mapLanguage["ar-iq"] = __XT("阿拉伯语+伊拉克");
g_mapLanguage["ar-jo"] = __XT("阿拉伯语+约旦");
g_mapLanguage["ar-kw"] = __XT("阿拉伯语+科威特");
g_mapLanguage["ar-lb"] = __XT("阿拉伯语+黎巴嫩");
g_mapLanguage["ar-ly"] = __XT("阿拉伯语+利比亚");
g_mapLanguage["ar-ma"] = __XT("阿拉伯语+摩洛哥");
g_mapLanguage["ar-om"] = __XT("阿拉伯语+阿曼");
g_mapLanguage["ar-qa"] = __XT("阿拉伯语+卡塔尔");
g_mapLanguage["ar-sa"] = __XT("阿拉伯语+沙特阿拉伯");
g_mapLanguage["ar-sd"] = __XT("阿拉伯语+苏丹");
g_mapLanguage["ar-sy"] = __XT("阿拉伯语+叙利亚");
g_mapLanguage["ar-tn"] = __XT("阿拉伯语+突尼斯");
g_mapLanguage["ar-ye"] = __XT("阿拉伯语+也门");
g_mapLanguage["be"] = __XT("白俄罗斯语+");
g_mapLanguage["be-by"] = __XT("白俄罗斯语+白俄罗斯");
g_mapLanguage["bg"] = __XT("保加利亚语+");
g_mapLanguage["bg-bg"] = __XT("保加利亚语+保加利亚");
g_mapLanguage["ca"] = __XT("加泰罗尼亚语+");
g_mapLanguage["ca-es"] = __XT("加泰罗尼亚语+西班牙");
g_mapLanguage["ca-es-euro"] = __XT("加泰罗尼亚语+西班牙,euro");
g_mapLanguage["cs"] = __XT("捷克语+");
g_mapLanguage["cs-cz"] = __XT("捷克语+捷克共和国");
g_mapLanguage["da"] = __XT("丹麦语+");
g_mapLanguage["da-dk"] = __XT("丹麦语+丹麦");
g_mapLanguage["de"] = __XT("德语+");
g_mapLanguage["de-at"] = __XT("德语+奥地利");
g_mapLanguage["de-at-euro"] = __XT("德语+奥地利,euro");
g_mapLanguage["de-ch"] = __XT("德语+瑞士");
g_mapLanguage["de-de"] = __XT("德语+德国");
g_mapLanguage["de-de-euro"] = __XT("德语+德国,euro");
g_mapLanguage["de-lu"] = __XT("德语+卢森堡");
g_mapLanguage["de-lu-euro"] = __XT("德语+卢森堡,euro");
g_mapLanguage["el"] = __XT("希腊语+");
g_mapLanguage["el-gr"] = __XT("希腊语+希腊");
g_mapLanguage["en-au"] = __XT("英语+澳大利亚");
g_mapLanguage["en-ca"] = __XT("英语+加拿大");
g_mapLanguage["en-gb"] = __XT("英语+英国");
g_mapLanguage["en-ie"] = __XT("英语+爱尔兰");
g_mapLanguage["en-ie-euro"] = __XT("英语+爱尔兰,euro");
g_mapLanguage["en-nz"] = __XT("英语+新西兰");
g_mapLanguage["en-za"] = __XT("英语+南非");
g_mapLanguage["es"] = __XT("西班牙语+");
g_mapLanguage["es-bo"] = __XT("西班牙语+玻利维亚");
g_mapLanguage["es-ar"] = __XT("西班牙语+阿根廷");
g_mapLanguage["es-cl"] = __XT("西班牙语+智利");
g_mapLanguage["es-co"] = __XT("西班牙语+哥伦比亚");
g_mapLanguage["es-cr"] = __XT("西班牙语+哥斯达黎加");
g_mapLanguage["es-do"] = __XT("西班牙语+多米尼加共和国");
g_mapLanguage["es-ec"] = __XT("西班牙语+厄瓜多尔");
g_mapLanguage["es-es"] = __XT("西班牙语+西班牙");
g_mapLanguage["es-es-euro"] = __XT("西班牙语+西班牙,euro");
g_mapLanguage["es-gt"] = __XT("西班牙语+危地马拉");
g_mapLanguage["es-hn"] = __XT("西班牙语+洪都拉斯");
g_mapLanguage["es-mx"] = __XT("西班牙语+墨西哥");
g_mapLanguage["es-ni"] = __XT("西班牙语+尼加拉瓜");
g_mapLanguage["et"] = __XT("爱沙尼亚语+");
g_mapLanguage["es-pa"] = __XT("西班牙语+巴拿马");
g_mapLanguage["es-pe"] = __XT("西班牙语+秘鲁");
g_mapLanguage["es-pr"] = __XT("西班牙语+波多黎哥");
g_mapLanguage["es-py"] = __XT("西班牙语+巴拉圭");
g_mapLanguage["es-sv"] = __XT("西班牙语+萨尔瓦多");
g_mapLanguage["es-uy"] = __XT("西班牙语+乌拉圭");
g_mapLanguage["es-ve"] = __XT("西班牙语+委内瑞拉");
g_mapLanguage["et-ee"] = __XT("爱沙尼亚语+爱沙尼亚");
g_mapLanguage["fi"] = __XT("芬兰语+");
g_mapLanguage["fi-fi"] = __XT("芬兰语+芬兰");
g_mapLanguage["fi-fi-euro"] = __XT("芬兰语+芬兰,euro");
g_mapLanguage["fr"] = __XT("法语+");
g_mapLanguage["fr-be"] = __XT("法语+比利时");
g_mapLanguage["fr-be-euro"] = __XT("法语+比利时,euro");
g_mapLanguage["fr-ca"] = __XT("法语+加拿大");
g_mapLanguage["fr-ch"] = __XT("法语+瑞士");
g_mapLanguage["fr-fr"] = __XT("法语+法国");
g_mapLanguage["fr-fr-euro"] = __XT("法语+法国,euro");
g_mapLanguage["fr-lu"] = __XT("法语+卢森堡");
g_mapLanguage["fr-lu-euro"] = __XT("法语+卢森堡,euro");
g_mapLanguage["hr"] = __XT("克罗地亚语+");
g_mapLanguage["hr-hr"] = __XT("克罗地亚语+克罗地亚");
g_mapLanguage["hu"] = __XT("匈牙利语+");
g_mapLanguage["hu-hu"] = __XT("匈牙利语+匈牙利");
g_mapLanguage["is"] = __XT("冰岛语+");
g_mapLanguage["is-is"] = __XT("冰岛语+冰岛");
g_mapLanguage["it"] = __XT("意大利语+");
g_mapLanguage["it-ch"] = __XT("意大利语+瑞士");
g_mapLanguage["it-it"] = __XT("意大利语+意大利");
g_mapLanguage["it-it-euro"] = __XT("意大利语+意大利,euro");
g_mapLanguage["iw"] = __XT("希伯来语+");
g_mapLanguage["iw-il"] = __XT("希伯来语+以色列");
g_mapLanguage["ja"] = __XT("日语+");
g_mapLanguage["ja-jp"] = __XT("日语+日本");
g_mapLanguage["ko"] = __XT("朝鲜语+");
g_mapLanguage["ko-kr"] = __XT("朝鲜语+南朝鲜");
g_mapLanguage["lt"] = __XT("立陶宛语+");
g_mapLanguage["lt-lt"] = __XT("立陶宛语+立陶宛");
g_mapLanguage["lv"] = __XT("拉托维亚语+列托");
g_mapLanguage["lv-lv"] = __XT("拉托维亚语+列托");
g_mapLanguage["mk"] = __XT("马其顿语+");
g_mapLanguage["mk-mk"] = __XT("马其顿语+马其顿王国");
g_mapLanguage["nl"] = __XT("荷兰语+");
g_mapLanguage["nl-be"] = __XT("荷兰语+比利时");
g_mapLanguage["nl-be-euro"] = __XT("荷兰语+比利时,euro");
g_mapLanguage["nl-nl"] = __XT("荷兰语+荷兰");
g_mapLanguage["nl-nl-euro"] = __XT("荷兰语+荷兰,euro");
g_mapLanguage["no"] = __XT("挪威语+");
g_mapLanguage["no-no"] = __XT("挪威语+挪威");
g_mapLanguage["no-no-ny"] = __XT("挪威语+挪威,nynorsk");
g_mapLanguage["pl"] = __XT("波兰语+");
g_mapLanguage["pl-pl"] = __XT("波兰语+波兰");
g_mapLanguage["pt"] = __XT("葡萄牙语+");
g_mapLanguage["pt-br"] = __XT("葡萄牙语+巴西");
g_mapLanguage["pt-pt"] = __XT("葡萄牙语+葡萄牙");
g_mapLanguage["pt-pt-euro"] = __XT("葡萄牙语+葡萄牙,euro");
g_mapLanguage["ro"] = __XT("罗马尼亚语+");
g_mapLanguage["ro-ro"] = __XT("罗马尼亚语+罗马尼亚");
g_mapLanguage["ru"] = __XT("俄语+");
g_mapLanguage["ru-ru"] = __XT("俄语+俄罗斯");
g_mapLanguage["sh"] = __XT("塞波尼斯-克罗地亚语+");
g_mapLanguage["sh-yu"] = __XT("塞波尼斯-克罗地亚语+南斯拉夫");
g_mapLanguage["sk"] = __XT("斯洛伐克语+");
g_mapLanguage["sk-sk"] = __XT("斯洛伐克语+斯洛伐克");
g_mapLanguage["sl"] = __XT("斯洛语尼亚语+");
g_mapLanguage["sl-si"] = __XT("斯洛语尼亚语+斯洛文尼亚");
g_mapLanguage["sq"] = __XT("阿尔巴尼亚语+");
g_mapLanguage["sq-al"] = __XT("阿尔巴尼亚语+阿尔巴尼亚");
g_mapLanguage["sr"] = __XT("塞尔维亚语+");
g_mapLanguage["sr-yu"] = __XT("塞尔维亚语+南斯拉夫");
g_mapLanguage["sv"] = __XT("瑞典语+");
g_mapLanguage["sv-se"] = __XT("瑞典语+瑞典");
g_mapLanguage["th"] = __XT("泰语+");
g_mapLanguage["th-th"] = __XT("泰语+泰国");
g_mapLanguage["tr"] = __XT("土耳其语+");
g_mapLanguage["tr-tr"] = __XT("土耳其语+土耳其");
g_mapLanguage["uk"] = __XT("乌克兰语+");
g_mapLanguage["uk-ua"] = __XT("乌克兰语+乌克兰");
g_mapLanguage["zh"] = __XT("汉语+");
g_mapLanguage["zh-cn"] = __XT("汉语+中国");
g_mapLanguage["zh-hk"] = __XT("汉语+香港");
g_mapLanguage["zh-tw"] = __XT("汉语+台湾");                                                                     */
/************************************************************************/


class CTranslateUtil
{
public:
	void Init(const XString& strDomain,const XString& strCigPath,const XString& strRegex);
	//如果元语言制定 auto的话，后面会返回识别出来的元语言
	XString Translate(const XString& strSrc,const XString&strOrigLanguage,  const XString& strDestLanguage,XString& strOrigLanguageRet);


private:
	int GetTKK(const std::string& strSrc);
	void GetTk(const std::string& strSrc, std::string& strTk);
	XINT64 GetMagic(XINT64 ullMagic, const std::string& strExpr);


	XString m_strDomain;
	XString m_strTranslateURL;
	std::string m_strRegex;

	//需要保存的tkk 参数
	XINT64 m_ullFirstTKK;
	XINT64 m_ullSecondTKK;
};

