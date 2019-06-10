#include "way_ent.h"

void CWayEntStart::Precache()
{
    m_iTrail = PRECACHE_MODEL("sprites/smoke.spr");
}

void CWayEntStart::Spawn()
{
   UTIL_SetOrigin(pev, pev->origin);

    SetThink(&CWayEntStart::StartThink);

    m_fCreated = FALSE;

    pev->nextthink = gpGlobals->time + 0.1;
}

void CWayEntStart::StartThink()
{
    if(!m_fCreated)
    {
        m_pBeam = CBeam::BeamCreate("sprites/rope.spr", 32);
        if (m_pBeam)
        {
                m_pBeam->PointsInit(pev->origin, pev->origin + Vector(0,0,72));
                m_pBeam->SetBrightness(200);
                m_pBeam->SetColor(0, 255, 0);
                m_pBeam->SetNoise(0);
                m_pBeam->SetFrame(0);
                m_pBeam->SetScrollRate(10);
                m_pBeam->RelinkBeam();

                m_fCreated = TRUE;
        }
    }

    pev->nextthink = gpGlobals->time + 0.1;
}

void CWayEntStart::CreatePoint(const Vector origin)
{
    Spawn();
}

void *CWayEntStart::Remove(CWayEntStart *start)
{
    if (start->m_pBeam)
    {
       UTIL_Remove(start->m_pBeam);
       start->m_pBeam = NULL;
    }

    UTIL_Remove(start);
}

LINK_ENTITY_TO_CLASS(way_start, CWayEntStart)


void CWayEntEnd::Spawn()
{
   UTIL_SetOrigin(pev, pev->origin);
   SetThink(&CWayEntEnd::EndThink);

   m_fCreated = FALSE;

   pev->nextthink = gpGlobals->time + 0.1;
}


void CWayEntEnd::EndThink()
{
    if(!m_fCreated)
    {
        m_pBeam = CBeam::BeamCreate("sprites/rope.spr", 32);
        if (m_pBeam)
        {
                m_pBeam->PointsInit(pev->origin, pev->origin + Vector(0,0,72));
                m_pBeam->SetBrightness(200);
                m_pBeam->SetColor(255, 0, 0);
                m_pBeam->SetNoise(0);
                m_pBeam->SetFrame(0);
                m_pBeam->SetScrollRate(10);
                m_pBeam->RelinkBeam();

                m_fCreated = TRUE;
        }
    }
}
LINK_ENTITY_TO_CLASS(way_end, CWayEntEnd)
