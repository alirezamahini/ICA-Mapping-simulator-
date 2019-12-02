#include "StdAfx.h"
#include "GraphNode.h"

GraphNode::GraphNode(void)
{
	m_nDegree=0;
	m_nSumofConnectedDeg=0;
	for(int i=0;i<GR_MAX_ND; i++)
		m_nLinkCommVol[i]=0;
	m_nTotalCV=0;
	m_bIsMapped=false;
	m_bIsSelect4Part=false;
}

GraphNode::~GraphNode(void)
{
}
int GraphNode::GetDegree()
{
	return m_nDegree;
}
void GraphNode::AddLink(GraphNode *nd,float cv){
	bool bExist=false;
	int	nIdx=0;
	for(int i=0;i<m_nDegree;i++){
		if(m_pLinks[i]->GetId()==nd->GetId()){
			bExist=true;
			nIdx=i;
			break;
		}
	}
	if(!bExist){
		m_pLinks[m_nDegree]=nd;
		m_nLinkCommVol[m_nDegree]=cv;
		m_nTotalCV+=cv;
		m_nDegree++;
		
	}/*test else if(m_nLinkCommVol[nIdx]<cv){
		m_nTotalCV+=cv-m_nLinkCommVol[nIdx];
		m_nLinkCommVol[nIdx]=cv;		
	}*/
	else{
		m_nTotalCV+=cv;
		m_nLinkCommVol[nIdx]+=cv;		
	}
}
void GraphNode::CalculateTotalDeg(){
	for(int i=0;i<m_nDegree;i++){
		m_nSumofConnectedDeg+=m_pLinks[i]->m_nDegree;
	}
}
void GraphNode::ShowStat(){

//	cout<<"\n\n-------------nodeNmae: "<<m_strTaskName<<"\t NodeId:\t"<< m_nId<<"-----------------------\n";
//	cout<<"\nnode degree: "<<m_nDegree<<"\trequired bandwidth= "<<m_nTotalCV<<endl;;
//	cout<<"links:\n ";
	for(int i=0;i<m_nDegree;i++)
		cout<<m_strTaskName<<"\t==>\t"<<m_pLinks[i]->m_strTaskName <<"\t"<<m_nLinkCommVol[i]<<endl;

}
void GraphNode::Map2Core(Core* cr){
	
	//cout<<"\nTask "<<m_strTaskName<<" with Id "<<m_nId<<" Mapped onto Core "<<cr->GetId()<<endl;
	m_bIsMapped=true;
	cr->AssignTask(this);
	m_pCore=cr;
}
GraphNode* GraphNode:: GetFirstUnMappedChild(){
	GraphNode *nd=NULL;
	for(int i=0;i<m_nDegree;i++){
		if(!m_pLinks[i]->m_bIsMapped){
			nd=m_pLinks[i];
			break;
		}
	}
	return nd;
}
GraphNode* GraphNode:: GetLink(int i){
	if(i<m_nDegree && m_pLinks[i])
		return m_pLinks[i];
	return NULL;
}
GraphNode* GraphNode:: GetMaxUnMappedChild(){
	GraphNode *nd=NULL;
	int max=0;
	for(int i=0;i<m_nDegree;i++){
		if(!m_pLinks[i]->m_bIsMapped && m_pLinks[i]->m_nDegree>max ){
			max=m_pLinks[i]->m_nDegree;
			nd=m_pLinks[i];
		}
	}
	return nd;
}
GraphNode* GraphNode:: GetMinUnMappedChild(){
	GraphNode *nd=NULL;
	int min=1000000;
	for(int i=0;i<m_nDegree;i++){
		if(!m_pLinks[i]->m_bIsMapped && m_pLinks[i]->m_nDegree<min ){
			min=m_pLinks[i]->m_nDegree;
			nd=m_pLinks[i];
		}
	}
	return nd;
}

GraphNode* GraphNode:: GetMaxChild(){
	GraphNode *nd=NULL;
	int max=0;
	for(int i=0;i<m_nDegree;i++){
		if(m_pLinks[i]->m_nDegree>max ){
			max=m_pLinks[i]->m_nDegree;
			nd=m_pLinks[i];
		}
	}
	return nd;
} bool GraphNode::IsUsed4Par(){
	return m_bIsSelect4Part;
}
GraphNode*  GraphNode:: GetNextMaxChild(){
	GraphNode *nd=NULL;
	int max=0;
	int id=-1;
	for(int i=0;i<m_nDegree;i++){
		if(m_pLinks[i]->IsUsed4Par()) continue;
		float cv=m_pLinks[i]->GetCommVol(this);
		if(cv>max && !(m_pLinks[i]->m_bIsSelect4Part) ){
			max=cv;
			nd=m_pLinks[i];
			id=i;
		}
	}
	if(id!=-1){
		m_pLinks[id]->m_bIsSelect4Part=true;
	}
	return nd;
}

void GraphNode ::SetNameId(int id,char *name){
	m_nId=id;
	strcpy(m_strTaskName,name);
}
int GraphNode::GetId(){
	return m_nId;
}
int GraphNode::GetWight(){
	return m_nDegree+m_nSumofConnectedDeg;
}
char * GraphNode::GetName(){
	return m_strTaskName;
}
float GraphNode::GetCommVol(int nLink){
	return m_nLinkCommVol[nLink];
}
float GraphNode::GetCommVol(GraphNode *nd){
	bool bExist=false;
	int	nIdx=0;
	float cv=-1;
	for(int i=0;i<m_nDegree;i++){
		if(m_pLinks[i]->GetId()==nd->GetId()){
			bExist=true;
			nIdx=i;
			break;
		}
	}
	if(!bExist){
		//cout<<"Error in commVol calculation\n";
	}
	else {
		cv=m_nLinkCommVol[nIdx];
	}
	return cv;
}
float GraphNode::GetCommVol(){
	return m_nTotalCV;
}
void GraphNode::UnMap(){
	m_bIsMapped=false;
	m_pCore->Free();
	m_pCore=NULL;
}
bool GraphNode::IsMapped(){
	return m_bIsMapped;
}
Core* GraphNode::GetPtrCore(){
	return m_pCore;
}
bool GraphNode::HasUnSelectedChild(){
	bool bRes= false;
	for(int i=0;i<m_nDegree;i++){
		if(!m_pLinks[i]->IsUsed4Par()){
			bRes=true;
			break;
		}
	}
	return bRes;
}