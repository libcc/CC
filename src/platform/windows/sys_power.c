#include <libcc/platform/windows.h>
#include <libcc/power.h>

/**/
bool_t _cc_get_sys_power_info(_CC_POWER_STATE_ENUM_ *state, int32_t *seconds, byte_t *percent) {
#ifndef __CC_WINRT__
    SYSTEM_POWER_STATUS status;
    bool_t need_details = false;

    /* This API should exist back to Win95. */
    if (!GetSystemPowerStatus(&status)) {
        /* !!! FIXME: push GetLastError() */
        *state = _CC_POWERSTATE_UNKNOWN_;
    } else if (status.BatteryFlag == 0xFF) {
        /* unknown state */
        *state = _CC_POWERSTATE_UNKNOWN_;
    } else if (status.BatteryFlag & (1 << 7)) {
        /* no battery */
        *state = _CC_POWERSTATE_NO_BATTERY_;
    } else if (status.BatteryFlag & (1 << 3)) {
        /* charging */
        *state = _CC_POWERSTATE_CHARGING_;
        need_details = true;
    } else if (status.ACLineStatus == 1) {
        /* on AC, not charging. */
        *state = _CC_POWERSTATE_CHARGED_;
        need_details = true;
    } else {
        /* not on AC. */
        *state = _CC_POWERSTATE_ON_BATTERY_;
        need_details = true;
    }

    *percent = -1;
    *seconds = -1;

    if (need_details) {
        const byte_t pct = (byte_t)status.BatteryLifePercent;
        const int32_t secs = (int32_t)status.BatteryLifeTime;
        /* 255 == unknown */
        if (pct != 0xFF) {
            /* clamp between 0%, 100% */
            *percent = (pct > 100) ? 100 : pct;
        }
        /* ((DWORD)-1) == unknown */
        if (secs != 0xFFFFFFFF) {
            *seconds = secs;
        }
    }
    return true;
#else
    /* Notes:
        - the Win32 function, GetSystemPowerStatus, is not available for use on WinRT
        - Windows Phone 8 has a 'Battery' class, which is documented as available for C++
        - More info on WP8's Battery class can be found at
       http://msdn.microsoft.com/library/windowsphone/develop/jj207231
    */
#endif
}
