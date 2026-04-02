#pragma once
#include "Zone.h"
#include <thread>
#include <atomic>
#include <mutex>

class CMap_Manager
{
public:
    static CMap_Manager* Get_Instance()
    {
        if (!m_pInstance)
            m_pInstance = new CMap_Manager;
        return m_pInstance;
    }
    static void Destroy_Instance()
    {
        delete m_pInstance;
        m_pInstance = nullptr;
    }
public:
    void    Initialize();
    void    Update();
    void    Release();

    void    Change_Zone(ZONE_ID eID);
    void    Change_Zone_Async(ZONE_ID eID);

    bool    Is_Loading()        const { return m_bLoading.load(); }
    float   Get_LoadProgress()  const { return m_fLoadProgress.load(); }
    ZONE_ID Get_CurZoneID()     const;
    CZone* Get_CurZone()       const { return m_pCurZone; }

    bool        Is_Movable(int x, int z);
    TILE_TYPE   Get_TileType(int x, int z);
    void        Render(ID2D1RenderTarget* pRT);

private:
    CZone* Create_Zone(ZONE_ID eID);
    void    Load_Zone(ZONE_ID eID);
    void    Unload_Zone(ZONE_ID eID);


private:
    std::vector<CZone*>     m_vecZone;
    CZone*                  m_pCurZone = nullptr;
    CZone*                  m_pPendingZone = nullptr;

    std::atomic<bool>       m_bLoading{ false };
    std::atomic<float>      m_fLoadProgress{ 0.f };
    std::mutex              m_ZoneMutex;

private:
    CMap_Manager() {}
    ~CMap_Manager() { Release(); }

private:
    static CMap_Manager*    m_pInstance;

};