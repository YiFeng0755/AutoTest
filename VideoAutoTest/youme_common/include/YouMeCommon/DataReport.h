#pragma once
#include <YouMeCommon/record.h>
#include <assert.h>
#include <YouMeCommon/Log.h>
#include <YouMeCommon/YouMeDataChannel.h>
//TComm  ���ݹ���ͷ����ͬ��������Ҫ�Ĺ������ֿ��ܲ�һ����������������һ������ͷ�ĸ���
/************************************************************************/
/*              struct CCommonData9 { //struct of commondata
				std::string m_strGuid; //Guid
				unsigned char m_uchGuid_type; //Ψһ��ʾ����
				std::string m_strVersion; //�汾��
				std::string m_strManufacturer; //����
				std::string m_strBrand; //Ʒ��
				std::string m_strNet_supplier; //�����ṩ��
				std::string m_strPlatform_version; //ƽ̨�汾��

				CCommonData9(void) :
				m_uchGuid_type(0)
				{
				m_strGuid = "";
				m_strVersion = "";
				m_strManufacturer = "";
				m_strBrand = "";
				m_strNet_supplier = "";
				m_strPlatform_version = "";
				}

				void LoadToRecord(youmecommon::CRecord& oCommonDataRecord) const {
				oCommonDataRecord.SetData(m_strGuid.c_str());
				oCommonDataRecord.SetData(m_uchGuid_type);
				oCommonDataRecord.SetData(m_strVersion.c_str());
				oCommonDataRecord.SetData(m_strManufacturer.c_str());
				oCommonDataRecord.SetData(m_strBrand.c_str());
				oCommonDataRecord.SetData(m_strNet_supplier.c_str());
				oCommonDataRecord.SetData(m_strPlatform_version.c_str());
				}

				};                                                        */
/************************************************************************/



//TBody  �����壬��Ҫ�ϱ���������
/************************************************************************/
/*      struct CBodyData1011 { //struct of Recorddata
		unsigned short m_body1; //
		unsigned int m_body2; //
		std::string m_strBody3;

		CBodyData1011(void) :
		m_body1(0),
		m_body2(0),
		m_strBody3(0)
		{
		}
		void LoadToRecord(youmecommon::CRecord& oRecordDataRecord) const {
		oRecordDataRecord.SetData(m_body1);
		oRecordDataRecord.SetData(m_body2);
		oRecordDataRecord.SetData(m_strBody3.c_str());
		}

		};                                                                */
/************************************************************************/


//TPram  ��������һЩ������UDP ����Ϣ�����ط������˾Ͷ��ˣ�TCP 
/************************************************************************/
/*              struct CReportParam1011 { //struct of ReportPram
				static unsigned short m_usCmd;
				static bool m_bUseTCP;
				unsigned int m_uiTcpPort;
				unsigned int m_uiUdpPort;
				static XString m_strDomain;
				
				};

				unsigned short CReportPram1011::m_usCmd = (1011);
				int CReportPram1011::m_uiTcpPort= 443;
				int CReportPram1011::m_uiUdpPort = 8000;
				std::string CReportPram1011::m_strDomain = __XT("youme.im.com");   
				bool CReportPram1011::m_bUseTCP = true;   */
/************************************************************************/

template < class TBody, class TParam>
class CDataReport {
public:
	CDataReport(CYouMeDataChannel* pDataChanel, unsigned short usCmd, unsigned short usVer,bool bUseTCP) {
		//����ͷ����Ϣ��ֻ��һ��������
		m_recordData.SetData(usCmd);
		//��ǰ�ϱ��汾�ţ��˰汾�źͿͻ����ϱ�����û�й�ϵ
		m_recordData.SetData(usVer);
		m_recordData.SetData(TParam::m_strIdentify.c_str());
		m_pDataChanel = pDataChanel;
		m_bUseTCP = bUseTCP;
#ifdef _DEBUG
		m_bSetBody = false;
#endif // _DEBUG
	}

	~CDataReport(void) {
		
	}

	//������Ϣ��
	void SetBody(const TBody& oBody)
	{
		oBody.LoadToRecord(m_recordData);
#ifdef _DEBUG
		m_bSetBody = true;
#endif // _DEBUG
	}
	void Report();
private:
	CDataReport(const CDataReport< TBody, TParam>& oDataReport);
	CDataReport& operator=(const CDataReport< TBody, TParam>& oDataReport);

private:	
	youmecommon::CRecord m_recordData;
	CYouMeDataChannel* m_pDataChanel;
	bool m_bUseTCP;
#ifdef _DEBUG
	bool m_bSetBody;
#endif // _DEBUG

};

template <class TBody, class TParam>
void CDataReport<TBody, TParam>::Report()
{
#ifdef _DEBUG
	if ((!m_bSetBody) ||(NULL == m_pDataChanel))
	{
		assert(false);
		return;
	}
#endif // _DEBUG
	int iPort = TParam::m_uiUdpPort;
	if (m_bUseTCP)
	{
		iPort = TParam::m_uiTcpPort;
	}
	m_pDataChanel->SendData(TParam::m_strDomain, iPort, m_bUseTCP, m_recordData.GetBuf(), m_recordData.GetDataLen());
}

