#include "CBaseEntity.h"
CBaseEntity* pEntity = new CBaseEntity();

CBaseEntity::CBaseEntity(){}

CBaseEntity::~CBaseEntity(){}

DWORD CBaseEntity::GetEntity(DWORD entity)
{
	return m->ReadMem<DWORD>(entity);
}

int CBaseEntity::GetHealth(DWORD entity)
{
	return m->ReadMem<int>(entity + offsets::m_iHealth);
}

int CBaseEntity::GetTeamNum(DWORD entity)
{
	return m->ReadMem<int>(entity + offsets::teamNum);
}

int CBaseEntity::GetEntityIndex(DWORD entity)
{
	return m->ReadMem<int>(entity + offsets::dwGlowIdx);
}

int CBaseEntity::GetFlags(DWORD entity)
{
	return m->ReadMem<int>(entity + offsets::m_fFlags);
}

int CBaseEntity::GetMoveType(DWORD entity)
{
	return m->ReadMem<int>(entity + offsets::m_MoveType);
}

bool CBaseEntity::GetInAir(DWORD entity)
{
	return GetFlags(entity) == 256 || GetFlags(entity) == 258 || GetFlags(entity) == 260 || GetFlags(entity) == 262;
}