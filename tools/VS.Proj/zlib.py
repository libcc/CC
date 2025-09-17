import os
from VCXProj import VCXProj 

LIBSources = [
"zlib/adler32.c",
"zlib/compress.c",
"zlib/crc32.c",
"zlib/deflate.c",
"zlib/infback.c",
"zlib/inffast.c",
"zlib/inflate.c",
"zlib/inftrees.c",
"zlib/trees.c",
"zlib/uncompr.c",
"zlib/zutil.c"
]
LIBHeaders = [
]

if __name__ == "__main__":
	vcxproj = VCXProj("zlib","StaticLibrary","./")
	vcxproj.OutDir = "..\\lib"

	vcxproj.addIncludePath(["C:\\240216\\third-party","C:\\240216\\01\\include"])
	vcxproj.addLibraryPath(["C:\\240216\\third-party\\lib\\$(Platform)\\$(Configuration)","C:\\240216\\01\\lib\\$(Platform)\\$(Configuration)"])
	vcxproj.addSource(LIBSources)
	vcxproj.addHeader(LIBHeaders)
	vcxproj.addMacros("Debug",["_LIB"])
	vcxproj.addMacros("Release",["_LIB"])
	vcxproj.build()



