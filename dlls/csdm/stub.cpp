#include "precompiled.h"

LINK_ENTITY_TO_CLASS(monster_scientist, CStub)
LINK_ENTITY_TO_CLASS(hostage_entity, CStub)
LINK_ENTITY_TO_CLASS(info_hostage_rescue, CStub)
LINK_ENTITY_TO_CLASS(func_hostage_rescue, CStub)
LINK_ENTITY_TO_CLASS(weapon_shield, CStub)

void CStub::Spawn()
{
	pev->flags = FL_KILLME;
	return;
}
