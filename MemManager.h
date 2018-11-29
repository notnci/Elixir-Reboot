#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <tchar.h>

struct Module
{
	DWORD dwBase;
	DWORD dwSize;
};

class MemManager
{
public:
	Module cDll;
	Module eDll;
	DWORD pID;
	static HANDLE hProc;

	
	bool attachProc(const char* procName)
	{
		HANDLE hPID = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
		PROCESSENTRY32 procEntry;
		procEntry.dwSize = sizeof(procEntry);
		const WCHAR* procNameCh;
		int numChar = MultiByteToWideChar(CP_ACP, 0, procName, -1, NULL, 0);
		procNameCh = new WCHAR[numChar];
		MultiByteToWideChar(CP_ACP, 0, procName, -1, (LPWSTR)procNameCh, numChar);

		do
			if (!wcscmp(procEntry.szExeFile, procNameCh))
			{
				pID = procEntry.th32ProcessID;
				CloseHandle(hPID);
				this->hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, this->pID);
				return true;
			}
		while (Process32Next(hPID, &procEntry));
		CloseHandle(hPID);
		return false;
	}

	Module getModule(DWORD processID, const char* moduleName)
	{
		DWORD_PTR dwModuleBaseAddress = 0;
		Module result;

		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processID);
		if (hSnapshot != INVALID_HANDLE_VALUE)
		{
			MODULEENTRY32 ModuleEntry32;
			ModuleEntry32.dwSize = sizeof(MODULEENTRY32);
			if (Module32First(hSnapshot, &ModuleEntry32))
			{
				do
				{
					std::string moduleNameString(moduleName);
					std::wstring moduleNameWideString(moduleNameString.begin(), moduleNameString.end());
					if (_tcsicmp(ModuleEntry32.szModule, moduleNameWideString.c_str()) == 0)
					{
						dwModuleBaseAddress = (DWORD_PTR)ModuleEntry32.modBaseAddr;
						result.dwBase = dwModuleBaseAddress;
						result.dwSize = ModuleEntry32.modBaseSize;
						break;
					}
				} while (Module32Next(hSnapshot, &ModuleEntry32));
			}
			CloseHandle(hSnapshot);
		}
		return result;
	}

	template <class c>
	c ReadMem(DWORD dwAddress)
	{
		c val;
		ReadProcessMemory(hProc, (LPVOID)dwAddress, &val, sizeof(c), NULL);
		return val;
	}
	template <class c>
	BOOL WriteMem(DWORD dwAddress, c valToWrite)
	{
		return WriteProcessMemory(hProc, (LPVOID)dwAddress, &valToWrite, sizeof(c), NULL);
	}


	static DWORD FindPattern(DWORD start, DWORD size, const char* sig, const char* mask)
	{
		BYTE* data = new BYTE[size];
		unsigned long bytesRead;
		if (!ReadProcessMemory(hProc, (LPVOID)start, data, size, &bytesRead))
		{
			delete[] sig;
			delete[] data;
			return NULL;
		}

		for (DWORD i = 0; i < size; i++)
		{
			if (DataCompare((const BYTE*)(data + i), (const BYTE*)sig, mask))
			{
				delete[] sig;
				delete[] data;
				return start + i;
			}
		}
		delete[] sig;
		delete[] data;
		return NULL;
	}

	static DWORD FindPattern(DWORD base, DWORD size, char pattern[], char mask[])
	{
		for (DWORD retAddress = base; retAddress < (base + size - strlen(mask)); retAddress++)
		{
			if (*(BYTE*)retAddress == (pattern[0] & 0xff) || mask[0] == '?')
			{
				DWORD startSearch = retAddress;
				for (int i = 0; mask[i] != '\0'; i++, startSearch++)
				{
					if ((pattern[i] & 0xff) != *(BYTE*)startSearch && mask[i] != '?')
						break;

					if (((pattern[i] & 0xff) == *(BYTE*)startSearch || mask[i] == '?') && mask[i + 1] == '\0')
						return retAddress;
				}
			}
		}

		return NULL;
	}

	static DWORD FindPatternArray(DWORD start, DWORD size, const char* mask, int count, ...)
	{
		char* sig = new char[count + 1];
		va_list ap;
		va_start(ap, count);
		for (int i = 0; i < count; i++)
		{
			char read = va_arg(ap, char);
			sig[i] = read;
		}
		va_end(ap);
		sig[count] = '\0';
		return FindPattern(start, size, sig, mask);
	}

	static bool DataCompare(const BYTE* pData, const BYTE* pMask, const char* pszMask)
	{
		for (; *pszMask; ++pszMask, ++pData, ++pMask)
		{
			if (*pszMask == 'x' && *pData != *pMask)
			{
				return false;
			}
		}
		return (*pszMask == NULL);
	}

	MemManager();
	~MemManager();
		
};


