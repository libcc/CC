
#include <libcc.h>
#include <libcc/widgets/widgets.h>


_CC_API_PRIVATE(void) _attach(void) {
	srand((uint32_t)time(nullptr));
    _cc_get_cpu_cores();
    _cc_install_socket();
	_cc_logger_debug("widgets attach");
}


_CC_API_PRIVATE(void) _detach(void) {
	_cc_uninstall_socket();
	_cc_logger_debug("widgets detach");

}

#ifdef __CC_WINDOWS__
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
    case DLL_PROCESS_ATTACH:
		_attach();
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		_detach();
		break;
	}
	return true;
}

#else

static __attribute__((constructor)) void _dynamic_attach(void) {
	_attach();
}

static __attribute__((destructor)) void _dynamic_detach(void) {
	_detach();
}
#endif