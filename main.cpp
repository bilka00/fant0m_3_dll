#include "main.h"
#include <windows.h>

DWORD addr;
UINT __stdcall jmpGetDriveTypeA(LPCTSTR lpRootPathName)
{
	//Если хотите продолжить выполнение функции можно сюда jmp ткнуть
    return 5; //Возвращаемый результат (как раз тот что нам нужно)
};
UINT __stdcall hookGetDriveTypeA(LPCTSTR lpRootPathName)
{
    return jmpGetDriveTypeA(lpRootPathName); //Здесь собстна можно сделать дабы выбирался реальный сидюк
};

extern "C" DLL_EXPORT BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    DWORD target;
    DWORD oldprotect;
    BYTE * pE9;
    DWORD * pofset;
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:

            target = (DWORD)GetProcAddress(GetModuleHandle("kernel32.dll"),"GetDriveTypeA");
            if(!VirtualProtect((void*)target,10,PAGE_EXECUTE_READWRITE,&oldprotect))return FALSE;
            addr = target + 5;
            pE9 = (BYTE*)target;
            pofset = (DWORD*)(pE9+1);
            *pE9 = 0xe9;
            *pofset = (DWORD)hookGetDriveTypeA - (target+5);
            break;

        case DLL_PROCESS_DETACH:
            // detach from process
            break;
    }
    return TRUE; // succesful
}
