
#ifndef CTIME_H
#define CTIME_H

#define MILISECOND_DIVISOR 1000.0


class CTime
{
private:

	static CTime* m_pcInstance;

	double m_dDeltaTime;
	double m_dFrequency;

	int m_nStartTime;
	int m_nPreviousTime;
	int m_nCurrentTime;

public:

	CTime();
	~CTime() = default;

	static CTime* GetInstance();
	static void DeleteInstance();

	bool Initialize();
	bool Update();

	float DeltaTime() {	return (float)m_dDeltaTime; };
	float CurrentTime();
};

#endif
