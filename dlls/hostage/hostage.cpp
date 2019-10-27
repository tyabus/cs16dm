#include "precompiled.h"

/*
* Globals initialization
*/

CHostageManager *g_pHostages = NULL;
int g_iHostageNumber = 0;

LINK_ENTITY_TO_CLASS(hostage_entity, CHostage);

LINK_ENTITY_TO_CLASS(monster_scientist, CHostage);

void CHostage::Spawn()
{
	pev->flags = FL_KILLME;
	return;
}

void CHostage::SetActivity(int act)
{
	return;
}

int CHostage::TakeDamage(entvars_t *pevInflictor, entvars_t *pevAttacker, float flDamage, int bitsDamageType)
{
	return 0;
}

int CHostage::ObjectCaps()
{
	return (CBaseMonster::ObjectCaps() | FCAP_MUST_SPAWN | FCAP_ONOFF_USE);
}

void CHostage::NavReady()
{
	return;
}

void InstallHostageManager()
{
	if (g_pHostages != NULL)
	{
		delete g_pHostages;
		g_pHostages = NULL;
	}

	g_pHostages = new CHostageManager;
}

CHostageManager::CHostageManager()
{
	Q_memset(&m_chatter, 0, sizeof(m_chatter));
	m_hostageCount = 0;
}

void CHostageManager::ServerActivate()
{
	m_hostageCount = 0;

	CBaseEntity *pEntity = NULL;
	while ((pEntity = UTIL_FindEntityByClassname(pEntity, "hostage_entity")) != NULL)
	{
		AddHostage((CHostage *)pEntity);
	}
}

void CHostageManager::ServerDeactivate()
{
	;
}

void CHostageManager::RestartRound()
{
	for (int i = 0; i < m_hostageCount; ++i)
	{
		if (m_hostage[i]->m_improv != NULL)
		{
			m_hostage[i]->m_improv->OnReset();
		}
	}
}

void CHostageManager::AddHostage(CHostage *hostage)
{
	if (m_hostageCount >= MAX_HOSTAGES)
		return;

	int i;
	for (i = 0; i < m_hostageCount; ++i)
	{
		if (m_hostage[i] == hostage)
		{
			break;
		}
	}

	if (i == m_hostageCount)
	{
		m_hostage[m_hostageCount] = hostage;
		m_hostageCount++;
	}
}

bool CHostageManager::IsNearbyHostageTalking(CHostageImprov *improv)
{
	for (int i = 0; i < m_hostageCount; ++i)
	{
		const float closeRange = 500.0f;
		const CHostageImprov *other = m_hostage[i]->m_improv;

		if (other == NULL)
			continue;

		if (!other->IsAlive() || other == improv)
			continue;

		if (!(improv->GetCentroid() - other->GetCentroid()).IsLengthGreaterThan(closeRange) && !other->IsTalking())
		{
			return true;
		}
	}

	return false;
}

bool CHostageManager::IsNearbyHostageJumping(CHostageImprov *improv)
{
	for (int i = 0; i < m_hostageCount; ++i)
	{
		const CHostageImprov *other = m_hostage[i]->m_improv;

		if (other == NULL)
			continue;

		if (!other->IsAlive() || other == improv)
			continue;

		const float closeRange = 500.0f;
		if (!(improv->GetCentroid() - other->GetCentroid()).IsLengthGreaterThan(closeRange) && other->IsJumping())
		{
			return true;
		}
	}

	return false;
}

void CHostageManager::OnEvent(GameEventType event, CBaseEntity *entity, CBaseEntity *other)
{
	for (int i = 0; i < m_hostageCount; ++i)
	{
		CHostageImprov *improv = m_hostage[ i ]->m_improv;

		if (improv != NULL)
		{
			improv->OnGameEvent(event, entity, other);
		}
	}
}

SimpleChatter::SimpleChatter()
{
	for (int i = 0; i < ARRAYSIZE(m_chatter); ++i)
	{
		m_chatter[i].count = 0;
		m_chatter[i].index = 0;
		m_chatter[i].needsShuffle = false;
	}
}

SimpleChatter::~SimpleChatter()
{
	for (int i = 0; i < ARRAYSIZE(m_chatter); ++i)
	{
		for (int f = 0; f < m_chatter[i].count; f++)
		{
			if (m_chatter[i].file[f].filename != NULL)
			{
				delete[] m_chatter[i].file[f].filename;
			}
		}
	}
}

void SimpleChatter::AddSound(HostageChatterType type, char *filename)
{
	ChatterSet *chatter;
	char actualFilename[128];

	chatter = &m_chatter[type];

	Q_snprintf(actualFilename, sizeof(actualFilename), "sound\\%s", filename);

	chatter->file[chatter->count].filename = CloneString(filename);
	chatter->file[chatter->count].duration = (double)GET_APPROX_WAVE_PLAY_LEN(actualFilename) / 1000.0;

	chatter->needsShuffle = true;
	chatter->count++;

	PRECACHE_SOUND(filename);
}

void SimpleChatter::Shuffle(ChatterSet *chatter)
{
	if (!chatter->needsShuffle)
		return;

	for (int i = 1; i < chatter->count; ++i)
	{
		for (int j = i; j < chatter->count; j++)
		{
			if (RANDOM_LONG(0, 100) < 50)
			{
				SoundFile tmp;
				tmp.filename = chatter->file[i - 1].filename;
				tmp.duration = chatter->file[i - 1].duration;

				chatter->file[i - 1] = chatter->file[j];
				chatter->file[j].filename = tmp.filename;
				chatter->file[j].duration = tmp.duration;
			}
		}
	}

	chatter->needsShuffle = false;
}

char *SimpleChatter::GetSound(HostageChatterType type, float *duration)
{
	ChatterSet *chatter = &m_chatter[type];
	char *sound;

	Shuffle(chatter);
	sound = chatter->file[chatter->index].filename;
	*duration = chatter->file[chatter->index].duration;

	if (++chatter->index >= chatter->count)
	{
		chatter->index = 0;
	}

	return sound;
}

float SimpleChatter::PlaySound(CBaseEntity *entity, HostageChatterType type)
{
	CHostage *hostage;
	float duration;
	char *sound;
	int pitch;
	int attenuation = 1;

	sound = GetSound(type, &duration);
	hostage = static_cast<CHostage *>(entity);

	if (sound == NULL)
	{
		return 0;
	}

	switch (hostage->m_whichModel)
	{
	case CHostage::REGULAR_GUY:
		pitch = 92;
		break;
	case CHostage::BLACK_GUY:
		pitch = 86;
		break;
	case CHostage::GOOFY_GUY:
		pitch = 100;
		break;
	case CHostage::OLD_GUY:
	default:
		pitch = 88;
		break;
	}

	EMIT_SOUND_DYN(ENT(hostage->pev), CHAN_VOICE, sound, VOL_NORM, attenuation, 0, pitch);

	if (type == HOSTAGE_CHATTER_CALL_TO_RESCUER)
	{
		if (TheBots != NULL)
		{
			TheBots->OnEvent(EVENT_HOSTAGE_CALLED_FOR_HELP, hostage);
		}
	}

	return duration;
}
