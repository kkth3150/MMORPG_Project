#pragma once
class CTimer_Manager
{
private:
	CTimer_Manager();
	~CTimer_Manager();

public:

	static	CTimer_Manager* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CTimer_Manager;
		}
		return m_pInstance;
	}

	static void	Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

public:
	void Initialize();
	void Update();

	float Get_DeltaTime() const { return m_fDeltaTime; }

private:

	static	CTimer_Manager* m_pInstance;
	LARGE_INTEGER m_Frequency;
	LARGE_INTEGER m_PrevTime;

	float m_fDeltaTime;
};

