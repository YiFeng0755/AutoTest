#ifndef CPROFILEDB_H
#define CPROFILEDB_H
#include <YouMeCommon/CrossPlatformDefine/PlatformDef.h>
#include <mutex>
#include <SqliteOperator.h>
const XString s_szProfileTableName[] = {
	__XT("settings")};


const XString CREATE_TABLE_SETTINGS = \
__XT("create table settings(key text,value text)");


const XString szProfileCreateTableSQL[] = {
	CREATE_TABLE_SETTINGS
};
class CProfileDB
{
public:

	bool getSetting(const std::string& strKey, XString& strValue);
	bool setSetting(const std::string& strKey, const XString& strValue);
	bool deleteByKey(const std::string& strKey);

	CProfileDB(const XString& strDbPath);
private:
	//本地db
	youmecommon::CSqliteDb m_sqliteDb;
	std::mutex m_utex;

};

#endif // CPROFILEDB_H
