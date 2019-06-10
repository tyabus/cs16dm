#ifndef WAY_ENT_H
#define WAY_ENT_H

#include "precompiled.h"

class CWayEntStart : public CBaseEntity
{
public:
    void Spawn();
    void Precache();

    void EXPORT StartThink();

    void CreatePoint(const Vector origin);
    void *Remove(CWayEntStart *start);

    int m_iTrail;
    CBeam *m_pBeam;
    BOOL m_fCreated;
};


class CWayEntEnd : public CWayEntStart
{
public:
    void Spawn();

    void EXPORT EndThink();
};


#endif
