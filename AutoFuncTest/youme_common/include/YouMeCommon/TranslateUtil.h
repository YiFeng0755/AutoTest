#pragma once
#include <YouMeCommon/CrossPlatformDefine/PlatformDef.h>

/************************************************************************/
/*
g_mapLanguage["en"] = __XT("Ӣ��+Ӣ��");
g_mapLanguage["en-us"] = __XT("Ӣ��+����");
g_mapLanguage["ar"] = __XT("��������+");
g_mapLanguage["ar-ae"] = __XT("��������+����������������");
g_mapLanguage["ar-bh"] = __XT("��������+����");
g_mapLanguage["ar-dz"] = __XT("��������+����������");
g_mapLanguage["ar-eg"] = __XT("��������+����");
g_mapLanguage["ar-iq"] = __XT("��������+������");
g_mapLanguage["ar-jo"] = __XT("��������+Լ��");
g_mapLanguage["ar-kw"] = __XT("��������+������");
g_mapLanguage["ar-lb"] = __XT("��������+�����");
g_mapLanguage["ar-ly"] = __XT("��������+������");
g_mapLanguage["ar-ma"] = __XT("��������+Ħ���");
g_mapLanguage["ar-om"] = __XT("��������+����");
g_mapLanguage["ar-qa"] = __XT("��������+������");
g_mapLanguage["ar-sa"] = __XT("��������+ɳ�ذ�����");
g_mapLanguage["ar-sd"] = __XT("��������+�յ�");
g_mapLanguage["ar-sy"] = __XT("��������+������");
g_mapLanguage["ar-tn"] = __XT("��������+ͻ��˹");
g_mapLanguage["ar-ye"] = __XT("��������+Ҳ��");
g_mapLanguage["be"] = __XT("�׶���˹��+");
g_mapLanguage["be-by"] = __XT("�׶���˹��+�׶���˹");
g_mapLanguage["bg"] = __XT("����������+");
g_mapLanguage["bg-bg"] = __XT("����������+��������");
g_mapLanguage["ca"] = __XT("��̩��������+");
g_mapLanguage["ca-es"] = __XT("��̩��������+������");
g_mapLanguage["ca-es-euro"] = __XT("��̩��������+������,euro");
g_mapLanguage["cs"] = __XT("�ݿ���+");
g_mapLanguage["cs-cz"] = __XT("�ݿ���+�ݿ˹��͹�");
g_mapLanguage["da"] = __XT("������+");
g_mapLanguage["da-dk"] = __XT("������+����");
g_mapLanguage["de"] = __XT("����+");
g_mapLanguage["de-at"] = __XT("����+�µ���");
g_mapLanguage["de-at-euro"] = __XT("����+�µ���,euro");
g_mapLanguage["de-ch"] = __XT("����+��ʿ");
g_mapLanguage["de-de"] = __XT("����+�¹�");
g_mapLanguage["de-de-euro"] = __XT("����+�¹�,euro");
g_mapLanguage["de-lu"] = __XT("����+¬ɭ��");
g_mapLanguage["de-lu-euro"] = __XT("����+¬ɭ��,euro");
g_mapLanguage["el"] = __XT("ϣ����+");
g_mapLanguage["el-gr"] = __XT("ϣ����+ϣ��");
g_mapLanguage["en-au"] = __XT("Ӣ��+�Ĵ�����");
g_mapLanguage["en-ca"] = __XT("Ӣ��+���ô�");
g_mapLanguage["en-gb"] = __XT("Ӣ��+Ӣ��");
g_mapLanguage["en-ie"] = __XT("Ӣ��+������");
g_mapLanguage["en-ie-euro"] = __XT("Ӣ��+������,euro");
g_mapLanguage["en-nz"] = __XT("Ӣ��+������");
g_mapLanguage["en-za"] = __XT("Ӣ��+�Ϸ�");
g_mapLanguage["es"] = __XT("��������+");
g_mapLanguage["es-bo"] = __XT("��������+����ά��");
g_mapLanguage["es-ar"] = __XT("��������+����͢");
g_mapLanguage["es-cl"] = __XT("��������+����");
g_mapLanguage["es-co"] = __XT("��������+���ױ���");
g_mapLanguage["es-cr"] = __XT("��������+��˹�����");
g_mapLanguage["es-do"] = __XT("��������+������ӹ��͹�");
g_mapLanguage["es-ec"] = __XT("��������+��϶��");
g_mapLanguage["es-es"] = __XT("��������+������");
g_mapLanguage["es-es-euro"] = __XT("��������+������,euro");
g_mapLanguage["es-gt"] = __XT("��������+Σ������");
g_mapLanguage["es-hn"] = __XT("��������+�鶼��˹");
g_mapLanguage["es-mx"] = __XT("��������+ī����");
g_mapLanguage["es-ni"] = __XT("��������+�������");
g_mapLanguage["et"] = __XT("��ɳ������+");
g_mapLanguage["es-pa"] = __XT("��������+������");
g_mapLanguage["es-pe"] = __XT("��������+��³");
g_mapLanguage["es-pr"] = __XT("��������+�������");
g_mapLanguage["es-py"] = __XT("��������+������");
g_mapLanguage["es-sv"] = __XT("��������+�����߶�");
g_mapLanguage["es-uy"] = __XT("��������+������");
g_mapLanguage["es-ve"] = __XT("��������+ί������");
g_mapLanguage["et-ee"] = __XT("��ɳ������+��ɳ����");
g_mapLanguage["fi"] = __XT("������+");
g_mapLanguage["fi-fi"] = __XT("������+����");
g_mapLanguage["fi-fi-euro"] = __XT("������+����,euro");
g_mapLanguage["fr"] = __XT("����+");
g_mapLanguage["fr-be"] = __XT("����+����ʱ");
g_mapLanguage["fr-be-euro"] = __XT("����+����ʱ,euro");
g_mapLanguage["fr-ca"] = __XT("����+���ô�");
g_mapLanguage["fr-ch"] = __XT("����+��ʿ");
g_mapLanguage["fr-fr"] = __XT("����+����");
g_mapLanguage["fr-fr-euro"] = __XT("����+����,euro");
g_mapLanguage["fr-lu"] = __XT("����+¬ɭ��");
g_mapLanguage["fr-lu-euro"] = __XT("����+¬ɭ��,euro");
g_mapLanguage["hr"] = __XT("���޵�����+");
g_mapLanguage["hr-hr"] = __XT("���޵�����+���޵���");
g_mapLanguage["hu"] = __XT("��������+");
g_mapLanguage["hu-hu"] = __XT("��������+������");
g_mapLanguage["is"] = __XT("������+");
g_mapLanguage["is-is"] = __XT("������+����");
g_mapLanguage["it"] = __XT("�������+");
g_mapLanguage["it-ch"] = __XT("�������+��ʿ");
g_mapLanguage["it-it"] = __XT("�������+�����");
g_mapLanguage["it-it-euro"] = __XT("�������+�����,euro");
g_mapLanguage["iw"] = __XT("ϣ������+");
g_mapLanguage["iw-il"] = __XT("ϣ������+��ɫ��");
g_mapLanguage["ja"] = __XT("����+");
g_mapLanguage["ja-jp"] = __XT("����+�ձ�");
g_mapLanguage["ko"] = __XT("������+");
g_mapLanguage["ko-kr"] = __XT("������+�ϳ���");
g_mapLanguage["lt"] = __XT("��������+");
g_mapLanguage["lt-lt"] = __XT("��������+������");
g_mapLanguage["lv"] = __XT("����ά����+����");
g_mapLanguage["lv-lv"] = __XT("����ά����+����");
g_mapLanguage["mk"] = __XT("�������+");
g_mapLanguage["mk-mk"] = __XT("�������+���������");
g_mapLanguage["nl"] = __XT("������+");
g_mapLanguage["nl-be"] = __XT("������+����ʱ");
g_mapLanguage["nl-be-euro"] = __XT("������+����ʱ,euro");
g_mapLanguage["nl-nl"] = __XT("������+����");
g_mapLanguage["nl-nl-euro"] = __XT("������+����,euro");
g_mapLanguage["no"] = __XT("Ų����+");
g_mapLanguage["no-no"] = __XT("Ų����+Ų��");
g_mapLanguage["no-no-ny"] = __XT("Ų����+Ų��,nynorsk");
g_mapLanguage["pl"] = __XT("������+");
g_mapLanguage["pl-pl"] = __XT("������+����");
g_mapLanguage["pt"] = __XT("��������+");
g_mapLanguage["pt-br"] = __XT("��������+����");
g_mapLanguage["pt-pt"] = __XT("��������+������");
g_mapLanguage["pt-pt-euro"] = __XT("��������+������,euro");
g_mapLanguage["ro"] = __XT("����������+");
g_mapLanguage["ro-ro"] = __XT("����������+��������");
g_mapLanguage["ru"] = __XT("����+");
g_mapLanguage["ru-ru"] = __XT("����+����˹");
g_mapLanguage["sh"] = __XT("������˹-���޵�����+");
g_mapLanguage["sh-yu"] = __XT("������˹-���޵�����+��˹����");
g_mapLanguage["sk"] = __XT("˹�工����+");
g_mapLanguage["sk-sk"] = __XT("˹�工����+˹�工��");
g_mapLanguage["sl"] = __XT("˹����������+");
g_mapLanguage["sl-si"] = __XT("˹����������+˹��������");
g_mapLanguage["sq"] = __XT("������������+");
g_mapLanguage["sq-al"] = __XT("������������+����������");
g_mapLanguage["sr"] = __XT("����ά����+");
g_mapLanguage["sr-yu"] = __XT("����ά����+��˹����");
g_mapLanguage["sv"] = __XT("�����+");
g_mapLanguage["sv-se"] = __XT("�����+���");
g_mapLanguage["th"] = __XT("̩��+");
g_mapLanguage["th-th"] = __XT("̩��+̩��");
g_mapLanguage["tr"] = __XT("��������+");
g_mapLanguage["tr-tr"] = __XT("��������+������");
g_mapLanguage["uk"] = __XT("�ڿ�����+");
g_mapLanguage["uk-ua"] = __XT("�ڿ�����+�ڿ���");
g_mapLanguage["zh"] = __XT("����+");
g_mapLanguage["zh-cn"] = __XT("����+�й�");
g_mapLanguage["zh-hk"] = __XT("����+���");
g_mapLanguage["zh-tw"] = __XT("����+̨��");                                                                     */
/************************************************************************/


class CTranslateUtil
{
public:
	void Init(const XString& strDomain,const XString& strCigPath,const XString& strRegex);
	//���Ԫ�����ƶ� auto�Ļ�������᷵��ʶ�������Ԫ����
	XString Translate(const XString& strSrc,const XString&strOrigLanguage,  const XString& strDestLanguage,XString& strOrigLanguageRet);


private:
	int GetTKK(const std::string& strSrc);
	void GetTk(const std::string& strSrc, std::string& strTk);
	XINT64 GetMagic(XINT64 ullMagic, const std::string& strExpr);


	XString m_strDomain;
	XString m_strTranslateURL;
	std::string m_strRegex;

	//��Ҫ�����tkk ����
	XINT64 m_ullFirstTKK;
	XINT64 m_ullSecondTKK;
};

