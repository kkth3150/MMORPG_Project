#include "pch.h"
#include "ItemData_Etc.h"

static const ETC_DATA s_EtcTable[ETC_END] =
{
    { L"ºí·¯µå½ºÅæ",    L"BloodStone" },
    { L"ºí·çÁª",        L"BlueGem"    },
    { L"ÇÁ¸®ÁòÁª",      L"PrizmGem"   },
    { L"·¹µåÁª",        L"RedGem"     },
    { L"³ú¼ö",          L"Brain"      },
    { L"¹ßÅé",          L"Claw"       },
    { L"±Í_0",          L"Ear_0"      },
    { L"±Í_1",          L"Ear_1"      },
    { L"±Í_2",          L"Ear_2"      },
    { L"ÀÌ»¡",          L"Tooth"      },
};

void CItemData_Etc::Set_EtcType(ETC_TYPE eType)
{
    m_eEtcType = eType;

    const ETC_DATA& data = s_EtcTable[eType];
    lstrcpy(m_szName, data.szName);
    lstrcpy(m_szIconKey, data.szIconKey);
    m_eType = ITEM_ETC;
}