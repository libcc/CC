import os
from VCXProj import VCXProj 

Sources = [
	"tests/test_atomic.c"
]
Headers = [
]

if __name__ == "__main__":
	vcxproj = VCXProj("libcc.test","Application","../../proj.Win/")
	vcxproj.OutDir = "..\\bin"
	vcxproj.Subsystem = "Console"

	vcxproj.addIncludePath(["C:\\0216\\third-party","C:\\0216\\libcc\\include"])
	vcxproj.addLibraryPath(["C:\\0216\\third-party\\lib\\$(Platform)","C:\\0216\\libcc\\lib\\$(Platform)\\$(Configuration)"])
	vcxproj.addSource(Sources)
	vcxproj.addHeader(Headers)

	Librarys = ["libcc.lib","libcc.widgets.lib"]
	Macros = ["_WINDOWS","_CONSOLE"]

	vcxproj.addLibrarys("Debug",Librarys)
	vcxproj.addLibrarys("Release",Librarys)
	vcxproj.addMacros("Debug",Macros)
	vcxproj.addMacros("Release",Macros)
	vcxproj.build()



