// GW.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

/*

GodsWar Packet Sniffer
copyright @ 2016 - John Smith

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include <Windows.h>
#include <string.h>
#include <iostream>
#include <detours.h>
#include <winsock.h>
#include <stdlib.h>
#include <sstream>
#include <cstring>
#include <algorithm>
#include <iomanip>
#include "rlutil.h"

#pragma comment(lib, "detours.lib")
#pragma comment(lib, "ws2_32.lib")

using namespace std;

int (WINAPI *pSend)(SOCKET s, const char* buf, int len, int flags) = send;
int WINAPI MySend(SOCKET s, const char* buf, int len, int flags);
int (WINAPI *pRecv)(SOCKET s, char* buf, int len, int flags) = recv;
int WINAPI MyRecv(SOCKET s, char* buf, int len, int flags);


void LogPacket(std::ostream& Logger, const char *buf, int len) {
	//Logger << "        0  1  2  3  4  5  6  7   8  9  A  B  C  D  E  F\n";
	//Logger << "       -- -- -- -- -- -- -- --  -- -- -- -- -- -- -- --\n";
	rlutil::setColor(14);
	Logger << "0000   ";

	for (int i = 0; i < len; ++i) {
		if (i != 0 && i % 16 == 0) {
			Logger << "  ";

			int line = (i / 16) - 1;

			for (int j = 0; j < 16; ++j) {
				char c = buf[line * 16 + j];

				if (c >= 32 && c <= 126) {
					Logger << c;
				}
				else {
					Logger << '.';
				}
			}

			Logger << "\n" << std::hex << std::setw(4) << std::setfill('0') << i << std::dec << std::setw(0) << "   ";
		}
		else if (i % 16 == 8) {
			Logger << ' ';
		}

		Logger << std::hex << std::setw(2) << std::setfill('0') << (int(buf[i]) & 0xFF) << ' ';
		Logger << std::dec << std::setw(0);

		if (i == len - 1) {
			int remaining = 16 - (len % 16);
			int fill = (remaining * 3) + 2;

			if (remaining >= 8) {
				++fill;
			}

			for (int j = 0; j < fill; ++j) {
				Logger << ' ';
			}

			int line = (i - ((len % 16) - 1)) / 16;

			for (int k = 0; k < (len % 16); ++k) {
				char c = buf[line * 16 + k];

				if (c >= 32 && c <= 126) {
					Logger << c;
				}
				else {
					Logger << '.';
				}
			}
		}
	}

	Logger << "\n\n";
}


void OutputDebug(string str)
{
	rlutil::setColor(10);
	cout << str << endl;
	rlutil::setColor(15);
}

void OutputWarning(string str)
{
	rlutil::setColor(12);
	cout << str << endl;
	rlutil::setColor(15);
}

int WINAPI MySend(SOCKET s, const char* buf, int len, int flags)
{
	OutputWarning("[+] SENT");
	LogPacket(cout, buf, len);
	return pSend(s, buf, len, flags);
}

int WINAPI MyRecv(SOCKET s, char* buf, int len, int flags)
{
	OutputWarning("[+] RECV");
	LogPacket(cout, buf, sizeof(buf));
	return pRecv(s, buf, len, flags);
}

VOID CreateHook()
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)pSend, MySend);
	OutputDebug("[+] Hooked Send!");
	DetourAttach(&(PVOID&)pRecv, MyRecv);
	OutputDebug("[+] Hooked Recv!");
	cout << endl;
	DetourTransactionCommit();
}
VOID DestroyHook()
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourDetach(&(PVOID&)pSend, MySend);
	DetourDetach(&(PVOID&)pRecv, MyRecv);
	DetourTransactionCommit();
	OutputDebug("[+] Successfully UnHooked!");
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  dwReason, LPVOID lpReserved
)
{
	//General Initialization
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	DisableThreadLibraryCalls(hModule);

	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		OutputDebug("#######################################################");
		OutputDebug("#                                                     #");
		OutputDebug("#                GodsWar Packet Sniffer               #");
		OutputDebug("#                    by John Smith                    #");
		OutputDebug("#                                                     #");
		OutputDebug("#######################################################\n");

		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)CreateHook, 0, 0, 0);
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)DestroyHook, 0, 0, 0);
		return TRUE;
		break;
	}
	return TRUE;
}

extern "C" __declspec(dllexport) void DUMMY(void) {
	return;
}