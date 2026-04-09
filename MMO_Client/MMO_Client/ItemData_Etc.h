#pragma once
#include "ItemData.h"

class CItemData_Etc : public CItemData
{
public:
	CItemData_Etc() = default;
	virtual ~CItemData_Etc() = default;

	void        Set_EtcType(ETC_TYPE eType);
	ETC_TYPE    Get_EtcType() const { return m_eEtcType; }

private:
	ETC_TYPE    m_eEtcType = ETC_END;
};

