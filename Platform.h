#pragma once
#include "Core.h"
#include "iostream"
using namespace std;
class CTG;
class Platform
{
	friend void DepthMap(CTG &, Platform &);
	friend void LevelMap(CTG &, Platform &);
	friend void HMap(CTG &, Platform &);

	private:
	Core **m_pCores;
	CTG *m_pTaskGraph;
	int m_nRow;
	int m_nCol;
	float m_fBitRtPow;//consumed power for transmit a bit via a router
	float m_fBitlnkPow;//consumed power for transmit a bit via a link
public:
	Platform* GetFreeSubSet(int nRow, int nCol, int startPos=0);
	Platform* GetBestFreeSubSet(int nRow, int nCol);
	Platform(int nRow, int nCol, float BitRoutePow,float BitLnkPow, float fFreq=1, int nCommVol=100);
	
	Platform(void);
	~Platform(void);
	void Display();
	int GetRowSize();
	int GetColSize();
	int GetDiameter();
	int GetSize();
	float GetBitRtPow();
	float GetBitLnkPow();
	Core *GetMaxFreeCore();
	Core *GetMaxFreeCoreOfTgRegion(CTG *tg);
	Core *GetMinFreeCoreOfTgRegion(CTG *tg);
	Core *GetNearestFreeCore(Core* pCore);
	Core *GetCorePtrOfIndex(int nIdx);
	int GetManhatanDis(Core *n, Core* m);
	int GetManhatanDis(int nCoreId, int mCoreId);
	void Copy(Platform *pl);
	void SetGraphPtr(CTG *ptr);
	CTG * GetGraphPtr();
	int GetCoresLinkNum();
	int* GetArrayofMappedNodes(CTG *tg, int &nLen);
};
