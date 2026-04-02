#pragma once
#include "Zone.h"

class CZone_Town : public CZone
{
public:
    CZone_Town() = default;
    virtual ~CZone_Town() = default;

    virtual void    Build() override;
    virtual ZONE_ID Get_ZoneID() const override { return ZONE_TOWN; }
    virtual void    Spawn_Objects() override;
    virtual void    Clear_Objects() override;

};