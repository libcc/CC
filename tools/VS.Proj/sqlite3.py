import os
from VCXProj import VCXProj 

LIBSources = [
"sqlite3/sqlite3.c"
]
LIBHeaders = [
"sqlite3/sqlite3.h"
]

if __name__ == "__main__":
	vcxproj = VCXProj("sqlite3","StaticLibrary","./")
	vcxproj.OutDir = "..\\lib"

	vcxproj.addIncludePath(["C:\\240216\\third-party","C:\\240216\\01\\include"])
	vcxproj.addLibraryPath(["C:\\240216\\third-party\\lib\\$(Platform)\\$(Configuration)","C:\\240216\\01\\lib\\$(Platform)\\$(Configuration)"])
	vcxproj.addSource(LIBSources)
	vcxproj.addHeader(LIBHeaders)
	vcxproj.addMacros("Debug",["_LIB"])
	vcxproj.addMacros("Release",["_LIB"])
	vcxproj.build()



