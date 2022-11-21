#include <windows.h>
#include <filter.h>
#include <stdio.h>

FILTER_DLL filter = {
    FILTER_FLAG_ALWAYS_ACTIVE | FILTER_FLAG_NO_CONFIG,
    NULL,NULL,
    const_cast<char*>("マウス横ホイール"),
    NULL,NULL,NULL,
    NULL,NULL,
    NULL,NULL,NULL,
    NULL,
    func_init,
};
EXTERN_C FILTER_DLL __declspec(dllexport)* __stdcall GetFilterTable(void) {
    return &filter;
}


static int exedit_base;
BOOL(*exedit_func_WndProc)(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam, void* editp, void* fp);

FILTER* get_exeditfp(FILTER* fp) {
    SYS_INFO si;
    fp->exfunc->get_sys_info(NULL, &si);

    for (int i = 0; i < si.filter_n; i++) {
        FILTER* tfp = (FILTER*)fp->exfunc->get_filterp(i);
        if (tfp->information != NULL) {
            if (!strcmp(tfp->information, "拡張編集(exedit) version 0.92 by ＫＥＮくん")) return tfp;
        }
    }
    return NULL;
}


BOOL exedit_Replace8(int exedit_address, byte new_8) {
    DWORD oldProtect;
    byte* address = (byte*)(exedit_address + exedit_base);
    if (!VirtualProtect(address, 1, PAGE_EXECUTE_READWRITE, &oldProtect)) {
        return FALSE;
    }
    *address = new_8;
    return VirtualProtect(address, 1, oldProtect, &oldProtect);
}



BOOL exedit_func_WndProc_wrap(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam, void* editp, void* fp) {
    if (message == WM_MOUSEHWHEEL) {
        if (((FILTER*)fp)->exfunc->is_saving(editp) == 0) {
            if ((wparam & 8) == 0) {
                int wheel_right = (short)(wparam >> 0x10);
                if (wheel_right != 0) {
                    int* timeline_zoom = (int*)(exedit_base + 0xa3fc8);
                    int* timeline_pos = (int*)(exedit_base + 0x1a52f0);
                    reinterpret_cast<void(__cdecl*)(int)>(exedit_base + 0x38c70)(*timeline_pos + (int)((wheel_right * 2 - 1 & 0xffffff38) + 100) * 10000 / *timeline_zoom);
                    return 0;
                }
            }
        }
    }
    return exedit_func_WndProc(hwnd, message, wparam, lparam, editp, fp);
}


BOOL func_init(FILTER* fp) {
    FILTER* exeditfp = get_exeditfp(fp);
    if (exeditfp == NULL) {
        MessageBoxA(fp->hwnd, "拡張編集0.92が見つかりませんでした", fp->name, MB_OK);
        return TRUE;
    }
    exedit_base = (int)exeditfp->dll_hinst;

    exedit_Replace8(0x3decd, 0x7c); // 縦ホイールを縦スクロールに、Alt+縦ホイールを横スクロールにする

    exedit_func_WndProc = exeditfp->func_WndProc;
    exeditfp->func_WndProc = exedit_func_WndProc_wrap;

    return TRUE;
}
