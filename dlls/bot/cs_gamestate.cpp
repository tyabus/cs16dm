#include "precompiled.h"

CSGameState::CSGameState(CCSBot *owner)
{
	m_owner = owner;
	m_isRoundOver = false;
	m_bombState = MOVING;

	m_lastSawBomber.Invalidate();
	m_lastSawLooseBomb.Invalidate();
	m_isPlantedBombPosKnown = false;
	m_plantedBombsite = UNKNOWN;

	m_bombsiteCount = 0;
	m_bombsiteSearchIndex = 0;
}

// Reset at round start

void CSGameState::Reset()
{
	int i;
	CCSBotManager *ctrl = TheCSBots();

	m_isRoundOver = false;

	// bomb
	m_bombState = MOVING;
	m_lastSawBomber.Invalidate();
	m_lastSawLooseBomb.Invalidate();
	m_bombsiteCount = ctrl->GetZoneCount();

	m_isPlantedBombPosKnown = false;
	m_plantedBombsite = UNKNOWN;

	for (i = 0; i < m_bombsiteCount; ++i)
	{
		m_isBombsiteClear[i] = false;
		m_bombsiteSearchOrder[i] = i;
	}

	// shuffle the bombsite search order
	// allows T's to plant at random site, and CT's to search in a random order
	// NOTE: VS6 std::random_shuffle() doesn't work well with an array of two elements (most maps)
	for (i = 0; i < m_bombsiteCount; ++i)
	{
		int swap = m_bombsiteSearchOrder[i];
		int rnd = RANDOM_LONG(i, m_bombsiteCount - 1);

		m_bombsiteSearchOrder[i] = m_bombsiteSearchOrder[rnd];
		m_bombsiteSearchOrder[rnd] = swap;
	}

	m_bombsiteSearchIndex = 0;
}

// Update game state based on events we have received

void CSGameState::OnEvent(GameEventType event, CBaseEntity *entity, CBaseEntity *other)
{
	switch (event)
	{
	case EVENT_BOMB_PLANTED:
		SetBombState(PLANTED);
		if (m_owner->m_iTeam == TERRORIST && other != NULL)
		{
			UpdatePlantedBomb(&other->pev->origin);
		}
		break;
	case EVENT_BOMB_DEFUSED:
		SetBombState(DEFUSED);
		break;
	case EVENT_BOMB_EXPLODED:
		SetBombState(EXPLODED);
		break;
	case EVENT_TERRORISTS_WIN:
	case EVENT_CTS_WIN:
	case EVENT_ROUND_DRAW:
		m_isRoundOver = true;
		break;
	default:
		break;
	}
}

// True if round has been won or lost (but not yet reset)

bool CSGameState::IsRoundOver() const
{
	return m_isRoundOver;
}

void CSGameState::SetBombState(BombState state)
{
	// if state changed, reset "last seen" timestamps
	if (m_bombState != state)
	{
		m_bombState = state;
	}
}

void CSGameState::UpdateLooseBomb(const Vector *pos)
{
	m_looseBombPos = *pos;
	m_lastSawLooseBomb.Reset();

	// we saw the loose bomb, update our state
	SetBombState(LOOSE);
}

float CSGameState::TimeSinceLastSawLooseBomb() const
{
	return m_lastSawLooseBomb.GetElapsedTime();
}

bool CSGameState::IsLooseBombLocationKnown() const
{
	if (m_bombState != LOOSE)
		return false;

	return (m_lastSawLooseBomb.HasStarted()) ? true : false;
}

void CSGameState::UpdateBomber(const Vector *pos)
{
	m_bomberPos = *pos;
	m_lastSawBomber.Reset();

	// we saw the bomber, update our state
	SetBombState(MOVING);
}

float CSGameState::TimeSinceLastSawBomber() const
{
	return m_lastSawBomber.GetElapsedTime();
}

bool CSGameState::IsPlantedBombLocationKnown() const
{
	if (m_bombState != PLANTED)
		return false;

	return m_isPlantedBombPosKnown;
}

// Return the zone index of the planted bombsite, or UNKNOWN

int CSGameState::GetPlantedBombsite() const
{
	if (m_bombState != PLANTED)
		return UNKNOWN;

	return m_plantedBombsite;
}

// Return true if we are currently in the bombsite where the bomb is planted

bool CSGameState::IsAtPlantedBombsite() const
{
	if (m_bombState != PLANTED)
		return false;

	CCSBotManager *ctrl = TheCSBots();
	const CCSBotManager::Zone *zone = ctrl->GetClosestZone(&m_owner->pev->origin);

	if (zone != NULL)
	{
		return (m_plantedBombsite == zone->m_index);
	}

	return false;
}

// Return the zone index of the next bombsite to search

int CSGameState::GetNextBombsiteToSearch()
{
	if (m_bombsiteCount <= 0)
		return 0;

	int i;
	// return next non-cleared bombsite index
	for (i = m_bombsiteSearchIndex; i < m_bombsiteCount; ++i)
	{
		int z = m_bombsiteSearchOrder[i];
		if (!m_isBombsiteClear[z])
		{
			m_bombsiteSearchIndex = i;
			return z;
		}
	}

	// all the bombsites are clear, someone must have been mistaken - start search over
	for (i = 0; i < m_bombsiteCount; ++i)
	{
		m_isBombsiteClear[i] = false;
	}

	m_bombsiteSearchIndex = 0;
	return GetNextBombsiteToSearch();
}

// Returns position of bomb in its various states (moving, loose, planted),
// or NULL if we don't know where the bomb is

const Vector *CSGameState::GetBombPosition() const
{
	switch (m_bombState)
	{
		case MOVING:
		{
			if (!m_lastSawBomber.HasStarted())
				return NULL;

			return &m_bomberPos;
		}
		case LOOSE:
		{
			if (IsLooseBombLocationKnown())
				return &m_looseBombPos;

			return NULL;
		}
		case PLANTED:
		{
			if (IsPlantedBombLocationKnown())
				return &m_plantedBombPos;

			return NULL;
		}
	}

	return NULL;
}

// We see the planted bomb at 'pos'

void CSGameState::UpdatePlantedBomb(const Vector *pos)
{
	CCSBotManager *ctrl = TheCSBots();
	const CCSBotManager::Zone *zone = ctrl->GetClosestZone(pos);

	if (zone == NULL)
	{
		CONSOLE_ECHO("ERROR: Bomb planted outside of a zone!\n");
		m_plantedBombsite = UNKNOWN;
	}
	else
	{
		m_plantedBombsite = zone->m_index;
	}

	m_plantedBombPos = *pos;
	m_isPlantedBombPosKnown = true;
	SetBombState(PLANTED);
}

// Someone told us where the bomb is planted

void CSGameState::MarkBombsiteAsPlanted(int zoneIndex)
{
	m_plantedBombsite = zoneIndex;
	SetBombState(PLANTED);
}

// Someone told us a bombsite is clear

void CSGameState::ClearBombsite(int zoneIndex)
{
	if (zoneIndex >= 0 && zoneIndex < m_bombsiteCount)
		m_isBombsiteClear[zoneIndex] = true;
}

bool CSGameState::IsBombsiteClear(int zoneIndex) const
{
	if (zoneIndex >= 0 && zoneIndex < m_bombsiteCount)
		return m_isBombsiteClear[zoneIndex];

	return false;
}
