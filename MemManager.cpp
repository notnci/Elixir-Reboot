#include "MemManager.h"

HANDLE MemManager::hProc;


MemManager::MemManager()
{
	this->hProc = NULL;
	this->pID = NULL;
	try
	{
		if (!attachProc("csgo.exe")) { throw 1; }
		this->cDll = getModule(pID, "client_panorama.dll");
		this->eDll = getModule(pID, "engine.dll"); //New beta update, don't question
	}
	catch (...)
	{
		MessageBoxA(NULL, "ERROR", "ERROR", MB_ICONSTOP | MB_OK);
	}


}

MemManager::~MemManager()
{
	CloseHandle(this->hProc);
}
