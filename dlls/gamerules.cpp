#include "precompiled.h"

Vector Spawns[128];
int sn = 0;

/*
* Globals initialization
*/
CHalfLifeMultiplay *g_pGameRules = NULL;

BOOL CGameRules::CanHaveAmmo(CBasePlayer *pPlayer, const char *pszAmmoName, int iMaxCarry)
{
	int iAmmoIndex;

	if (pszAmmoName != NULL)
	{
		iAmmoIndex = pPlayer->GetAmmoIndex(pszAmmoName);

		if (iAmmoIndex > -1)
		{
			if (pPlayer->AmmoInventory(iAmmoIndex) < iMaxCarry)
			{
				// player has room for more of this type of ammo
				return TRUE;
			}
		}
	}

	return FALSE;
}

Vector CGameRules::FixSpawn(Vector spawn, BOOL IsBot)
{
    if(sn >= CVAR_GET_FLOAT("max_rand_spawns"))
        return Spawns[RANDOM_LONG(0, sn-1)];

    else if(CVAR_GET_FLOAT("rand_spawn") == 0 || CVAR_GET_FLOAT("bots_rand_spawn_only") > 0 && IsBot)
        return spawn;

    TraceResult tr;
    int i = 0;

		while( i < 10 ) // 10 player heights
		{
            Vector point = spawn + Vector(RANDOM_FLOAT(-5000, 5000),RANDOM_FLOAT(-5000, 5000), RANDOM_FLOAT(1, 100));

            UTIL_TraceHull( point, point, ignore_monsters, human_hull, NULL, &tr );

            if( !tr.fStartSolid && !tr.fAllSolid)
            {
                Spawns[sn] = point;
                sn++;
                return point;
            }
			i--;
			if( i >= 0 )
				i=1;
		}

	return spawn;
}

edict_t *CGameRules::GetPlayerSpawnSpot(CBasePlayer *pPlayer)
{
    edict_t *pentSpawnSpot = NULL;
    CBaseEntity *ent = NULL;
    BOOL Bot;

    if(pPlayer->IsBot())
        Bot = TRUE;
    else
        Bot = FALSE;

    // gat valid spawn point
    if(CVAR_GET_FLOAT("team_deathmatch") <= 0 || pPlayer->setspawn == true && ((ent = UTIL_FindEntityByClassname(0, "info_player_deathmatch")) != NULL))
        pentSpawnSpot = hEntSelectSpawnPoint(pPlayer);
    else
        pentSpawnSpot = EntSelectSpawnPoint(pPlayer);
    // Move the player to the place it said.
    pPlayer->pev->origin = FixSpawn(VARS(pentSpawnSpot)->origin + Vector(0,0,1), Bot);

    pPlayer->pev->v_angle = g_vecZero;
    pPlayer->pev->velocity = g_vecZero;
    pPlayer->pev->angles = VARS(pentSpawnSpot)->angles;
    pPlayer->pev->punchangle = g_vecZero;
    pPlayer->pev->fixangle = 1;

    return pentSpawnSpot;
}

BOOL CGameRules::CanHavePlayerItem(CBasePlayer *pPlayer, CBasePlayerItem *pWeapon)
{
	// only living players can have items
	if (pPlayer->pev->deadflag != DEAD_NO)
	{
		return FALSE;
	}

	CCSBotManager *ctrl = TheCSBots();

	if (pPlayer->IsBot() && ctrl != NULL && !ctrl->IsWeaponUseable(pWeapon))
	{
		return FALSE;
	}

	if (pWeapon->pszAmmo1())
	{
		if (!CanHaveAmmo(pPlayer, pWeapon->pszAmmo1(), pWeapon->iMaxAmmo1()))
		{
			// we can't carry anymore ammo for this gun. We can only
			// have the gun if we aren't already carrying one of this type
			if (pPlayer->HasPlayerItem(pWeapon))
			{
				return FALSE;
			}
		}
	}
	else
	{
		// weapon doesn't use ammo, don't take another if you already have it.
		if (pPlayer->HasPlayerItem(pWeapon))
		{
			return FALSE;
		}
	}

	// note: will fall through to here if GetItemInfo doesn't fill the struct!
	return TRUE;
}

void CGameRules::RefreshSkillData()
{
	int iSkill = (int)CVAR_GET_FLOAT("skill");

	if (iSkill < 1)
		iSkill = 1;

	else if (iSkill > 3)
		iSkill = 3;

	gSkillData.iSkillLevel = iSkill;
	ALERT(at_console, "\nGAME SKILL LEVEL:%d\n", iSkill);

	gSkillData.monDmg12MM = 8;
	gSkillData.monDmgMP5 = 3;
	gSkillData.monDmg9MM = 5;
	gSkillData.suitchargerCapacity = 75;
	gSkillData.batteryCapacity = 15;
	gSkillData.healthchargerCapacity = 50;
	gSkillData.healthkitCapacity = 15;
}

CGameRules *InstallGameRules()
{
	SERVER_COMMAND("exec game.cfg\n");
	SERVER_EXECUTE();

	return new CHalfLifeMultiplay;
}
