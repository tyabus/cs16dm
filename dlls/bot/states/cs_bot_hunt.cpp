#include "precompiled.h"

// Begin the hunt

void HuntState::OnEnter(CCSBot *me)
{
	// lurking death
	if (me->IsUsingKnife() && me->IsWellPastSafe() && !me->IsHurrying())
		me->Walk();
	else
		me->Run();

	me->StandUp();
	me->SetDisposition(CCSBot::ENGAGE_AND_INVESTIGATE);
	me->SetTask(CCSBot::SEEK_AND_DESTROY);
	me->DestroyPath();
}

// Hunt down our enemies

void HuntState::OnUpdate(CCSBot *me)
{
	// if we've been hunting for a long time, drop into Idle for a moment to
	// select something else to do
	const float huntingTooLongTime = 30.0f;
	if (gpGlobals->time - me->GetStateTimestamp() > huntingTooLongTime)
	{
		// stop being a rogue and do the scenario, since there must not be many enemies left to hunt
		me->PrintIfWatched("Giving up hunting, and being a rogue\n");
		me->SetRogue(false);
		me->Idle();
		return;
	}

	CCSBotManager *ctrl = TheCSBots();

	// scenario logic
	if (ctrl->GetScenario() == CCSBotManager::SCENARIO_DEFUSE_BOMB)
	{
		if (me->m_iTeam == TERRORIST)
		{
			// if we have the bomb and it's time to plant, or we happen to be in a bombsite and it seems safe, do it
			if (me->IsCarryingBomb())
			{
				const float safeTime = 3.0f;
				if (ctrl->IsTimeToPlantBomb() || (me->IsAtBombsite() && gpGlobals->time - me->GetLastSawEnemyTimestamp() > safeTime))
				{
					me->Idle();
					return;
				}
			}

			// if we notice the bomb lying on the ground, go get it
			if (me->NoticeLooseBomb())
			{
				me->FetchBomb();
				return;
			}

			// if bomb has been planted, and we hear it, move to a hiding spot near the bomb and watch it
			const Vector *bombPos = me->GetGameState()->GetBombPosition();
			if (!me->IsRogue() && me->GetGameState()->IsBombPlanted() && bombPos != NULL)
			{
				me->SetTask(CCSBot::GUARD_TICKING_BOMB);
				me->Hide(TheNavAreaGrid.GetNavArea(bombPos));
				return;
			}
		}
		// CT
		else
		{
			if (!me->IsRogue() && me->CanSeeLooseBomb())
			{
				// if we are near the loose bomb and can see it, hide nearby and guard it
				me->SetTask(CCSBot::GUARD_LOOSE_BOMB);
				me->Hide(ctrl->GetLooseBombArea());
				me->GetChatter()->AnnouncePlan("GoingToGuardLooseBomb", ctrl->GetLooseBombArea()->GetPlace());
				return;
			}
			else if (ctrl->IsBombPlanted())
			{
				// rogues will defuse a bomb, but not guard the defuser
				if (!me->IsRogue() || !ctrl->GetBombDefuser())
				{
					// search for the planted bomb to defuse
					me->Idle();
					return;
				}
			}
		}
	}
	// listen for enemy noises
	if (me->ShouldInvestigateNoise())
	{
		me->InvestigateNoise();
		return;
	}

	// look around
	me->UpdateLookAround();

	// if we have reached our destination area, pick a new one
	// if our path fails, pick a new one
	if (me->GetLastKnownArea() == m_huntArea || me->UpdatePathMovement() != CCSBot::PROGRESSING)
	{
		m_huntArea = NULL;
		float oldest = 0.0f;

		int areaCount = 0;
		const float minSize = 150.0f;

		FOR_EACH_LL (TheNavAreaList, it)
		{
			CNavArea *area = TheNavAreaList[it];

			++areaCount;

			// skip the small areas
			const Extent *extent = area->GetExtent();
			if (extent->hi.x - extent->lo.x < minSize || extent->hi.y - extent->lo.y < minSize)
				continue;

			// keep track of the least recently cleared area
			float age = gpGlobals->time - area->GetClearedTimestamp(me->m_iTeam - 1);
			if (age > oldest)
			{
				oldest = age;
				m_huntArea = area;
			}
		}

		// if all the areas were too small, pick one at random
		int which = RANDOM_LONG(0, areaCount - 1);

		areaCount = 0;
		FOR_EACH_LL (TheNavAreaList, it2)
		{
			m_huntArea = TheNavAreaList[it2];

			if (which == areaCount)
				break;

			--which;
		}

		if (m_huntArea != NULL)
		{
			// create a new path to a far away area of the map
			me->ComputePath(m_huntArea, NULL, SAFEST_ROUTE);
		}
	}
}

// Done hunting

void HuntState::OnExit(CCSBot *me)
{
	;
}
