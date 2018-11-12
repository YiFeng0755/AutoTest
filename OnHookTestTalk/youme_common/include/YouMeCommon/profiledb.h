#ifndef CPROFILEDB_H
#define CPROFILEDB_H
#include <YouMeCommon/CrossPlatformDefine/PlatformDef.h>
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

	CProfileDB(const XString& strDbPath);
private:
	//����db
	youmecommon::CSqliteDb m_sqliteDb;
};

#endif // CPROFILEDB_H
