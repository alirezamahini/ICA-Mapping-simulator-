#pragma once
#include "Core.h"
#include "iostream"
using namespace std;
#define GR_MAX_ND	300
class Platform;
class GraphNode
{
	friend class CTG;
	friend void DepthMap(CTG &, Platform &);
	friend void LevelMap(CTG &, Platform &);
	friend void HMap(CTG &, Platform &);

	private:
	int m_nId;
	char m_strTaskName[20];
	int m_nDegree;
	int m_nSumofConnectedDeg;
	GraphNode *m_pLinks[GR_MAX_ND];
	float m_nLinkCommVol[GR_MAX_ND];
	Core *m_pCore;
	float m_nTotalCV;
	bool m_bIsMapped;
	bool m_bIsSelect4Part;
public:
	void CalculateTotalDeg();
	void AddLink(GraphNode *nd,float cv);
	void ShowStat();
	GraphNode* GetFirstUnMappedChild();
	GraphNode* GetMaxUnMappedChild();
	GraphNode* GetMinUnMappedChild();
	GraphNode* GetMaxChild();
	GraphNode* GetNextMaxChild();
	GraphNode* GetLink(int i);
	Core* GetPtrCore();
	int GetWight();
	GraphNode(void);
	~GraphNode(void);
	void SetNameId(int id, char* name);
	int GetId();
	char* GetName();
	void Map2Core(Core *cr);
	float GetCommVol(int nLink);
	float GetCommVol();
	float GetCommVol(GraphNode* node);
	int GetDegree();
	void UnMap();
	bool IsMapped();
	bool IsUsed4Par();
	bool HasUnSelectedChild();

};
