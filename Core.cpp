#include "StdAfx.h"
#include "Core.h"

Core::Core(float fFreq, int nCommVol,int nId)
{
	m_fFerq=fFreq;
	for(int i=0; i<4;i++) m_pLink[i]=NULL;
	m_bfree=true;
	m_nLinkNum=0;
	m_nCoreId=nId;
	m_nLinkBw=nCommVol;
	m_nTotalReqBw=0;
	m_AssignTask=NULL;
	m_pTaskGraph=NULL;
}
bool Core::IsFree(){
	return m_bfree;
}
float Core::GetFreq(){
	return m_fFerq;
}
int Core::GetLinkNum(){
	return m_nLinkNum;
}
int Core::GetFreeLinkNumInRegion(CTG *tg){
int cnt=0;
	for(int i=0;i<4;i++){
		if(!m_pLink[i]) continue;
		if(m_pLink[i]->GetGraphPtr()==tg && m_pLink[i]->IsFree())
			cnt++;
	}	
	return cnt;
}
void Core::SetLinkPtr(DIRECTION d, Core* ptr){
	m_pLink[d]=ptr;
	m_nLinkNum++;
}
Core* Core::GetLinkPtr(int i){
	if(i<0 || i>3) 
		return NULL;
	if(m_pLink[i]) 
		return m_pLink[i];
	else
		return NULL;

}
Core* Core::GetRightPtr(){
	if(m_pLink[RIGHT]) 
		return m_pLink[RIGHT];
	else
		return NULL;

}
Core* Core::GetLeftPtr(){
	if(m_pLink[LEFT]) 
		return m_pLink[LEFT];
	else
		return NULL;

}
Core* Core::GetDownPtr(){
	if(m_pLink[DOWN]) 
		return m_pLink[DOWN];
	else
		return NULL;

}
Core* Core::GetUpPtr(){
	if(m_pLink[UP]) 
		return m_pLink[UP];
	else
		return NULL;

}
Core::~Core(void)
{
}
void Core::AssignTask(GraphNode *Task){
	m_AssignTask=Task;
	m_nTotalReqBw=m_AssignTask->GetCommVol();
	m_bfree=false;
}
int Core::GetId(){
	return m_nCoreId;
}
int Core::GetTaskId(){
	int nRes=-1;
	if(m_AssignTask)
	nRes=m_AssignTask->GetId();
	return nRes;
}
char* Core::GetTaskName(){
	
	return (m_AssignTask)? m_AssignTask->GetName():"NULL";
}
Core * Core::GetMaxFreeCoreLinked(){
	Core *cr=NULL;
	int max=0;
	for(int i=0;i<4;i++){
		if(m_pLink[i] && m_pLink[i]->IsFree() && m_pLink[i]->GetLinkNum()>max){
			max=m_pLink[i]->GetLinkNum();
			cr=m_pLink[i];
		}
	}
	return cr;
}
Core * Core::GetMaxFreeCoreLinkedInRegion(){
	Core *cr=NULL;
	int max=0;
	for(int i=0;i<4;i++){
		if(m_pLink[i] && m_pLink[i]->IsFree() && 
			m_pLink[i]->GetGraphPtr()==m_pTaskGraph &&
			m_pLink[i]->GetLinkNumOfTgRegion(m_pTaskGraph)>max){
			max=m_pLink[i]->GetLinkNum();
			cr=m_pLink[i];
		}
	}
	return cr;
}
Core * Core::GetMinFreeCoreLinkedInRegion(){
	Core *cr=NULL;
	int min=1000000;
	for(int i=0;i<4;i++){
		if(m_pLink[i] && m_pLink[i]->IsFree() && 
			m_pLink[i]->GetGraphPtr()==m_pTaskGraph &&
			m_pLink[i]->GetLinkNumOfTgRegion(m_pTaskGraph)<min){
			min=m_pLink[i]->GetLinkNum();
			cr=m_pLink[i];
		}
	}
	return cr;
}
bool Core::IsFreeLink(int i){
	if(i<0 || i>3) 
		return false;
	if(m_pLink[i]) 
		return m_pLink[i]->IsFree();
	else 
		return false;
}
GraphNode* Core::GetTaskPtr(){
	return m_AssignTask;
}
void Core::SetGraphPtr(CTG *ptr){
	m_pTaskGraph=ptr;
}
CTG* Core::GetGraphPtr(){
	return m_pTaskGraph;
}
int Core::GetLinkNumOfTgRegion(CTG *tg){
	int cnt=0;
	for(int i=0;i<4;i++){
		if(!m_pLink[i]) continue;
		if(m_pLink[i]->GetGraphPtr()==tg)
			cnt++;
	}	
	return cnt;
}
void Core::Free(){
	m_bfree=true;
	m_AssignTask=NULL;
	m_pTaskGraph=NULL;
}