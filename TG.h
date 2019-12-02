#pragma once
#include "GraphNode.h"
#include "iostream"
#include "fstream"
#include "string.h"
#include "Platform.h"
#include "math.h"
#include <map>
using namespace std;
class Platform;
class GraphNode;

class CTG
{
	friend void DepthMap(CTG &, Platform &);
	friend void LevelMap(CTG &, Platform &);
	friend void HMap(CTG &, Platform &);

private:
	int m_nGrDegree;
	GraphNode *m_pNodes;
	char* m_strGraphFile, m_strName[30];
	Platform *m_pPlatform;
	char **m_strNodeNames;
	void InitNodesName();
	map <pair<char*,char*>,float> m_CommLinks;
public:
	void InitGraph();
	void Display();
	GraphNode *GetMaxDegNode();
	GraphNode *GetUnMappedMaxDegNode();
	GraphNode *GetUnMappedMinDegNode();
	GraphNode *GetMaxBwNode();
	GraphNode *GetUnMappedMaxWightNode();
	bool IsMapped();
	CTG( char* strGraphFile, Platform * pl=NULL);
	~CTG(void);
	int GetIdofNode(char name[20]);
	int GetManhatanDis(int n , int m);
	int GetManhatanDis();
	int GetMaxManhatanDis();
	int GetPnltManhatanDis();
	int GetSuitablePlatsize();
	char* GetGraphName();
	int GetDegree();
	float CalcPower();
	float CalcPower(float nExePow);
	void SetPlatPtr(Platform *pl);
	GraphNode * GetPtrNode(int nodeId);
	void UnMap();
	void Partition(CTG &parGr,float &interCommVol);
	GraphNode* GetNextMaxBwNode();
	float GetLinkVol(GraphNode* src, GraphNode *Dest);
};
