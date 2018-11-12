#pragma once
#include <lua-5.3.2/Luna.h>
class CLuaDownloadUploadManager :public youmecommon::CLuna<CLuaDownloadUploadManager>
{
public:
	CLuaDownloadUploadManager(youmecommon::lua_State *)
	{

	}
protected:
	int HttpRequest(youmecommon::lua_State* L);
public:
	static const char className[];
	static const FunctionType methods[];
	static const PropertyType properties[];

};
