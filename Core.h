#pragma once
#include "Link.h"
#include "GraphNode.h"
#include "TG.h"
class GraphNode;
class CTG;
enum DIRECTION{LEFT,RIGHT,UP,DOWN};
class Core
{

private:
	float m_fFerq;
	bool m_bfree;
	Core *m_pLink[4];
	int m_nLinkNum;
	int m_nCoreId;
	GraphNode *m_AssignTask;
	int m_nLinkBw;
	float m_nTotalReqBw;
	CTG *m_pTaskGraph;

public:
	Core	(float fFreq=0, int nCommVol=100,int nId=-1 );
	~Core	(void);
	bool	IsFree();
	bool	IsFreeLink(int i);
	float	GetFreq();
	int		GetLinkNum();
	int		GetId();
	int		GetTaskId();
	GraphNode* GetTaskPtr();
	char*	GetTaskName();
	
	void	SetLinkPtr(DIRECTION d, Core* ptr);
	Core*	GetLinkPtr(int i);
	void	AssignTask(GraphNode *Task);
	Core *GetMaxFreeCoreLinked();
	Core *GetMaxFreeCoreLinkedInRegion();
	Core *GetMinFreeCoreLinkedInRegion();
	Core* GetRightPtr();
	Core* GetLeftPtr();
	Core* GetDownPtr();
	Core* GetUpPtr();
	void SetGraphPtr(CTG *ptr);
	CTG* GetGraphPtr();
	int GetLinkNumOfTgRegion(CTG *tg);
	int GetFreeLinkNumInRegion(CTG *tg);
	void Free();


};
