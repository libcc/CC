import os
from VCXProj import VCXProj 


WidgetsSources = [
	"src/widgets/generic/generic.c",
	"src/widgets/generic/gzip.c",
	"src/widgets/generic/WS.c",
	"src/widgets/db/mysql.c",
	"src/widgets/db/sqlite.c",
	"src/widgets/db/sqlsvr.c",
	"src/widgets/ini/ini.c",
	"src/widgets/ini/ini.parser.c",
	"src/widgets/xml/xml.c",
	"src/widgets/xml/xml.parser.c",
	"src/widgets/json/json.array.c",
	"src/widgets/json/json.c",
	"src/widgets/json/json.object.c",
	"src/widgets/json/json.parser.c",
	"src/widgets/http/header.c",
	"src/widgets/http/request.parser.c",
	"src/widgets/http/response.parser.c",
	"src/widgets/url_request/http/url_request.c",
	"src/widgets/url_request/http/url_response.c",
	"src/widgets/event/event.c",
	"src/widgets/event/loop.c",
	"src/widgets/event/buffer.c",
	"src/widgets/event/select.c",
	"src/widgets/event/tcp.c",
	"src/widgets/event/timeout.c",
	"src/widgets/event/OpenSSL.c",
	"src/widgets/event/windows/sys_iocp.c",
	"src/widgets/event/windows/sys_io_context.c",
	"src/widgets/event/windows/sys_WSA.c",
	"src/widgets/widgets.c",
	"src/widgets/main.c"
]
WidgetsHeaders = [
	"include/libcc/widgets/event.h",
	"include/libcc/widgets/http.h",
	"include/libcc/widgets/ini.h",
	"include/libcc/widgets/json.h",
	"include/libcc/widgets/socks5.h",
	"include/libcc/widgets/sql.h",
	"include/libcc/widgets/timeout.h",
	"include/libcc/widgets/WS.h",
	"include/libcc/widgets/xml.h",
	"include/libcc/widgets/dns.h",
	"include/libcc/widgets/dylib.h",
	"include/libcc/widgets/ftp.h",
	"include/libcc/widgets/ip_locator.h",
	"include/libcc/widgets/smtp.h",
	"include/libcc/widgets/OpenSSL.h",
	"include/libcc/widgets/gzip.h",
	"include/libcc/widgets/url_request.h",
	"include/libcc/widgets/widgets.h",
	"src/widgets/event/event.c.h",
	"src/widgets/event/windows/sys_iocp.h",
	"src/widgets/event/windows/sys_socket_c.h",
	"src/widgets/generic/generic.c.h",
	"src/widgets/ini/ini.c.h",
	"src/widgets/json/json.c.h",
	"src/widgets/xml/xml.c.h"
]

if __name__ == "__main__":

	vcxproj = VCXProj("libcc.widgets","DynamicLibrary","../../proj.Win/")
	vcxproj.OutDir = "..\\bin"
	vcxproj.ImportLibrary = "..\\lib"

	vcxproj.addIncludePath([
		"C:\\0216\\third-party",
		"C:\\0216\\libcc\\include"
		])
	vcxproj.addLibraryPath([
		"C:\\0216\\third-party\\lib\\$(Platform)\\$(Configuration)",
		"C:\\0216\\libcc\\lib\\$(Platform)\\$(Configuration)"
		])
	vcxproj.addSource(WidgetsSources)
	vcxproj.addHeader(WidgetsHeaders)
	Librarys = ["libcc.lib","libmysql.lib","sqlite3.lib","zlib.lib","libssl.lib","libcrypto.lib"]
	Macros = ["_CC_API_WIDGETS_USE_DYNAMIC_","_CC_USE_OPENSSL_","_WINDOWS","_USRDLL"]
	vcxproj.addLibrarys("Debug",Librarys)
	vcxproj.addLibrarys("Release",Librarys)
	vcxproj.addMacros("Debug",Macros)
	vcxproj.addMacros("Release",Macros)
	vcxproj.build()



