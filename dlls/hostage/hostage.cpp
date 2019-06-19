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

	if (g_bIsCzeroGame)
	{
		m_chatter.AddSound(HOSTAGE_CHATTER_START_FOLLOW, "hostage/huse/getouttahere.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_START_FOLLOW, "hostage/huse/illfollow.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_START_FOLLOW, "hostage/huse/letsdoit.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_START_FOLLOW, "hostage/huse/letsgo.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_START_FOLLOW, "hostage/huse/letshurry.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_START_FOLLOW, "hostage/huse/letsmove.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_START_FOLLOW, "hostage/huse/okletsgo.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_START_FOLLOW, "hostage/huse/youlead.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_STOP_FOLLOW, "hostage/hunuse/comeback.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_STOP_FOLLOW, "hostage/hunuse/dontleaveme.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_STOP_FOLLOW, "hostage/hunuse/illstayhere.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_STOP_FOLLOW, "hostage/hunuse/notleaveme.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_STOP_FOLLOW, "hostage/hunuse/yeahillstay.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_INTIMIDATED, "hostage/hseenbyt/donthurtme1.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_INTIMIDATED, "hostage/hseenbyt/donthurtme2.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_INTIMIDATED, "hostage/hseenbyt/dontkill.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_INTIMIDATED, "hostage/hseenbyt/endpeace.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_INTIMIDATED, "hostage/hseenbyt/nevernegotiate.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_INTIMIDATED, "hostage/hseenbyt/nottellanything.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_INTIMIDATED, "hostage/hseenbyt/surrender.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_INTIMIDATED, "hostage/hseenbyt/whatdoyou.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_INTIMIDATED, "hostage/hseenbyt/whyareyou.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_RETREAT, "hostage/hseenbyt/donthurtme1.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_RETREAT, "hostage/hseenbyt/donthurtme2.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_RETREAT, "hostage/hseenbyt/dontkill.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_RETREAT, "hostage/hseenbyt/dontkill.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_RETREAT, "hostage/hretreat/illgoback1.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_RETREAT, "hostage/hretreat/illgoback2.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_RETREAT, "hostage/hretreat/okokgoing.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_RETREAT, "hostage/hretreat/okokgoing2.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_RETREAT, "hostage/hretreat/sorry1.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_RETREAT, "hostage/hretreat/sorry2.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_RETREAT, "hostage/hretreat/sorry3.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_PAIN, "hostage/hpain/hpain1.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_PAIN, "hostage/hpain/hpain2.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_PAIN, "hostage/hpain/hpain3.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_PAIN, "hostage/hpain/hpain4.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_PAIN, "hostage/hpain/hpain5.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_PAIN, "hostage/hpain/hpain6.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_SCARED_OF_GUNFIRE, "hostage/hreactions/awwcrap1.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_SCARED_OF_GUNFIRE, "hostage/hreactions/awwcrap2.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_SCARED_OF_GUNFIRE, "hostage/hreactions/getdown1.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_SCARED_OF_GUNFIRE, "hostage/hreactions/getdown2.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_SCARED_OF_GUNFIRE, "hostage/hreactions/getdown3.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_SCARED_OF_GUNFIRE, "hostage/hreactions/lookout1.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_SCARED_OF_GUNFIRE, "hostage/hreactions/lookout2.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_SCARED_OF_GUNFIRE, "hostage/hreactions/lookshooting1.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_SCARED_OF_GUNFIRE, "hostage/hreactions/lookshooting2.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_SCARED_OF_GUNFIRE, "hostage/hreactions/lookshooting3.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_SCARED_OF_GUNFIRE, "hostage/hreactions/ohgod1.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_SCARED_OF_GUNFIRE, "hostage/hreactions/ohgod2.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_SCARED_OF_MURDER, "hostage/hreactions/awwcrap1.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_SCARED_OF_MURDER, "hostage/hreactions/awwcrap2.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_SCARED_OF_MURDER, "hostage/hreactions/deargod1.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_SCARED_OF_MURDER, "hostage/hreactions/deargod2.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_SCARED_OF_MURDER, "hostage/hreactions/deargod3.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_SCARED_OF_MURDER, "hostage/hreactions/ohgod1.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_SCARED_OF_MURDER, "hostage/hreactions/ohgod2.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_SCARED_OF_MURDER, "hostage/hreactions/ohno1.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_SCARED_OF_MURDER, "hostage/hreactions/ohno2.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_SCARED_OF_MURDER, "hostage/hreactions/awww2.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_SCARED_OF_MURDER, "hostage/hreactions/awww4.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_LOOK_OUT, "hostage/hreactions/awwcrap1.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_LOOK_OUT, "hostage/hreactions/awwcrap2.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_LOOK_OUT, "hostage/hreactions/lookout1.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_LOOK_OUT, "hostage/hreactions/lookout2.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_PLEASE_RESCUE_ME, "hostage/hseenbyct/areyousave.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_PLEASE_RESCUE_ME, "hostage/hseenbyct/getmeoutta1.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_PLEASE_RESCUE_ME, "hostage/hseenbyct/getmeoutta2.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_PLEASE_RESCUE_ME, "hostage/hseenbyct/imahostage.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_PLEASE_RESCUE_ME, "hostage/hseenbyct/rescueme1.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_PLEASE_RESCUE_ME, "hostage/hseenbyct/rescueme2.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_PLEASE_RESCUE_ME, "hostage/hseenbyct/theyregonna.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_IMPATIENT_FOR_RESCUE, "hostage/hseenbyct/areyousave.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_IMPATIENT_FOR_RESCUE, "hostage/hseenbyct/getmeoutta1.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_IMPATIENT_FOR_RESCUE, "hostage/hseenbyct/getmeoutta2.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_IMPATIENT_FOR_RESCUE, "hostage/hseenbyct/rescueme1.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_IMPATIENT_FOR_RESCUE, "hostage/hseenbyct/rescueme2.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_SEE_RESCUE_ZONE, "hostage/hseezone/almostouttahere.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_SEE_RESCUE_ZONE, "hostage/hseezone/almostthere.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_SEE_RESCUE_ZONE, "hostage/hseezone/keepgoing.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_SEE_RESCUE_ZONE, "hostage/hseezone/notfar.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_CTS_WIN, "hostage/hctwin/alldead.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_CTS_WIN, "hostage/hctwin/goodnews.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_CTS_WIN, "hostage/hctwin/outtahere.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_CTS_WIN, "hostage/hctwin/over1.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_CTS_WIN, "hostage/hctwin/over2.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_CTS_WIN, "hostage/hctwin/over3.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_CTS_WIN, "hostage/hctwin/over4.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_CTS_WIN, "hostage/hctwin/over5.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_CTS_WIN, "hostage/hctwin/relief.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_TERRORISTS_WIN, "hostage/htwin/doomed.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_TERRORISTS_WIN, "hostage/htwin/godno.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_TERRORISTS_WIN, "hostage/htwin/nowwhat.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_TERRORISTS_WIN, "hostage/htwin/ohman.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_TERRORISTS_WIN, "hostage/htwin/ohno.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_RESCUED, "hostage/hrescued/finally.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_RESCUED, "hostage/hrescued/finallysafe.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_RESCUED, "hostage/hrescued/thankyou.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_RESCUED, "hostage/hrescued/wemadeit1.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_RESCUED, "hostage/hrescued/wemadeit2.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_RESCUED, "hostage/hrescued/wemadeit3.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_WARN_NEARBY, "hostage/hwarn/becareful1.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_WARN_NEARBY, "hostage/hwarn/becareful2.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_WARN_NEARBY, "hostage/hwarn/stillaround1.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_WARN_NEARBY, "hostage/hwarn/stillaround2.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_WARN_SPOTTED, "hostage/hwarn/lookout1.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_WARN_SPOTTED, "hostage/hwarn/lookout2.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_WARN_SPOTTED, "hostage/hwarn/overthere1.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_WARN_SPOTTED, "hostage/hwarn/overthere2.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_CALL_TO_RESCUER, "hostage/hwavect/helpme1.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_CALL_TO_RESCUER, "hostage/hwavect/helpme2.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_CALL_TO_RESCUER, "hostage/hwavect/hey1.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_CALL_TO_RESCUER, "hostage/hwavect/hey2.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_CALL_TO_RESCUER, "hostage/hwavect/overhere1.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_COUGH, "hostage/hsmoke/cough1.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_COUGH, "hostage/hsmoke/cough2.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_COUGH, "hostage/hsmoke/cough3.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_COUGH, "hostage/hsmoke/cough4.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_COUGH, "hostage/hsmoke/cough5.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_COUGH, "hostage/hsmoke/cough6.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_BLINDED, "hostage/hflash/cantsee.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_BLINDED, "hostage/hflash/myeyes.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_SAW_HE_GRENADE, "hostage/hgrenade/grenade1.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_SAW_HE_GRENADE, "hostage/hgrenade/grenade2.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_SAW_HE_GRENADE, "hostage/hgrenade/run.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_SAW_HE_GRENADE, "hostage/hgrenade/watchout.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_DEATH_CRY, "hostage/hdie/hdeath1.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_DEATH_CRY, "hostage/hdie/hdeath2.wav");
		m_chatter.AddSound(HOSTAGE_CHATTER_DEATH_CRY, "hostage/hdie/hdeath3.wav");
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
