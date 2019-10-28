#include "precompiled.h"

LINK_ENTITY_TO_CLASS(monster_scientist, CStub)
LINK_ENTITY_TO_CLASS(hostage_entity, CStub)
LINK_ENTITY_TO_CLASS(func_hostage_rescue, CStub)

void CStub::Spawn()
{
	pev->flags = FL_KILLME;
	return;
}
