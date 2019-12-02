#include "StdAfx.h"
#include "TG.h"

CTG::CTG(char* strGraphFile, Platform *pl)
{
	m_nGrDegree=0;
	m_pNodes= new GraphNode[m_nGrDegree];
	m_strGraphFile=strGraphFile;
	m_pPlatform=pl;
	
}

CTG::~CTG(void)
{
	if(m_pNodes) delete[] m_pNodes;
	if(m_strNodeNames) delete[]m_strNodeNames;
}

int CTG::GetIdofNode(char name[20]){
	int nRes=-1;
	for(int i=0;i<m_nGrDegree;i++){
		if(!strcmp(name, m_strNodeNames[i])){
			nRes=i;
			break;
		}
	}
	return nRes;
}
void CTG::InitNodesName(){
	char **strNodeNames=new char*[1000];
	ifstream f(m_strGraphFile,ios::in | ios::binary);
	if(!f){
		cout<<"can not open file:" <<m_strGraphFile<<endl;
		exit(0);
	}
	f>>m_strName;
	cout<<"\n--------------Graph "<<m_strName<<" will be initialized--------------\n";
	
	int nline=0;
	int nFound=0;
	while(!f.eof()){
		char ConnInf[80];
		f.getline(ConnInf,sizeof(ConnInf));
		nline++;
		char str[3][20];
		//cout<<ConnInf<<endl;
		int n= sscanf(ConnInf,"%s\t%s\t%s",str[0],str[1],str[2]);
		if(n!=3) continue;
		for(int k=0;k<2;k++){
			bool bFound=false;
			for(int i=0;i<nFound && !bFound;i++){
				if(!strcmp(str[k], strNodeNames[i])){
					bFound=true;;
				}
			}
			if(!bFound){
				strNodeNames[nFound]=new char[strlen(str[k])];
				strcpy(strNodeNames[nFound++],str[k]);
			}
		}

	}
	f.close();
	m_nGrDegree=nFound;
	if(m_nGrDegree<1){
		cout<<"Graph Degree Error\n";
		exit(0);
	}
	//cout<<"\nGraph Degree="<<m_nGrDegree<<endl<<"Graph Nodes Names:\n";
	m_strNodeNames=new char*[m_nGrDegree];
	for(int i=0; i<m_nGrDegree;i++){
		m_strNodeNames[i]=new char[strlen(strNodeNames[i])];
		strcpy(m_strNodeNames[i],strNodeNames[i]);
		//cout<<i<<"\t"<<m_strNodeNames[i]<<endl;;
	}
	cout<<"Degree="<<m_nGrDegree<<endl;
	delete[] strNodeNames;
}

void CTG::InitGraph(){
	InitNodesName();
	ifstream f(m_strGraphFile,ios::in | ios::binary);
	if(!f){
		cout<<"can not open file:" <<m_strGraphFile<<endl;
		exit(0);
	}
	m_pNodes= new GraphNode[m_nGrDegree];
	for(int i=0;i<m_nGrDegree;i++){
		m_pNodes[i].SetNameId(i,m_strNodeNames[i]);
	}
	int nline=0;
	while(!f.eof()){
		char ConnInf[80];
		f.getline(ConnInf,sizeof(ConnInf));
		//if(f.eof()) break;
		nline++;
		int sourceId,sinkId;
		char source[20],sink[20];
		char strCv[20];
		float cv;
		int n=sscanf(ConnInf,"%s\t%s\t%s",source,sink,strCv);
		if(n!=3) continue;
		n=sscanf(strCv,"%f",&cv);
		if(n!=1){
			cout<<cout<<"Parse Error in dataline "<<nline<<": comm.Vol "<<strCv<<" is not valid\n";
			continue;
		}
		sourceId=GetIdofNode(source);
		sinkId=GetIdofNode(sink);
		
		
		if(sourceId>=m_nGrDegree || sourceId<0){
			cout<<"Parse Error in dataline "<<nline<<": the node with id "<<sourceId<<" is not valid\n";
			exit(0);
		}
		if(sinkId>=m_nGrDegree || sinkId<0){
			cout<<"Parse Error in dataline "<<nline<<": the node with id "<<sinkId<<" is not valid\n";
			exit(0);
		}
		//cout<<sourceId<<"\t"<<sinkId<<"\t"<<cv<<"\n";
		pair<char* , char*> Connection;
		Connection=make_pair(m_pNodes[sourceId].GetName(),m_pNodes[sinkId].GetName());
		m_CommLinks[Connection]=cv;
		Connection=make_pair(m_pNodes[sinkId].GetName(),m_pNodes[sourceId].GetName());
		m_CommLinks[Connection]=cv;
		m_pNodes[sourceId].AddLink(&m_pNodes[sinkId],cv);
		m_pNodes[sinkId].AddLink(&m_pNodes[sourceId],cv);
		
	}
	f.close();
	for(int i=0;i<m_nGrDegree;i++){
		m_pNodes[i].CalculateTotalDeg();
		//cout<<"\nW("<<m_pNodes[i].GetName()<<")= "<<m_pNodes[i].GetWight();
	}
//	Partition();

}
GraphNode *CTG::GetMaxDegNode(){
	int max=0;
	GraphNode *MaxNode;
	for(int i=0;i<m_nGrDegree;i++){
		if(m_pNodes[i].m_nDegree>max){
			MaxNode=&m_pNodes[i];
			max=MaxNode->m_nDegree;
		}
	}
	return MaxNode;
}
GraphNode* CTG::GetUnMappedMaxDegNode(){
	int max=0;
	GraphNode* MaxNode;//=NULL;
	for(int i=0;i<m_nGrDegree;i++){
		if(m_pNodes[i].m_nDegree>max && !m_pNodes[i].m_bIsMapped){
			MaxNode=&m_pNodes[i];
			max=MaxNode->m_nDegree;
		}
	}
	return MaxNode;
}
GraphNode* CTG::GetUnMappedMinDegNode(){
	int min=1000000;
	GraphNode* MinNode;//=NULL;
	for(int i=0;i<m_nGrDegree;i++){
		if(m_pNodes[i].m_nDegree<min && !m_pNodes[i].m_bIsMapped){
			MinNode=&m_pNodes[i];
			min=MinNode->m_nDegree;
		}
	}
	return MinNode;
}
GraphNode* CTG::GetUnMappedMaxWightNode(){
	int max=0;
	GraphNode* MaxNode=NULL;
	for(int i=0;i<m_nGrDegree;i++){
		if(m_pNodes[i].GetWight()>max && !m_pNodes[i].m_bIsMapped){
			MaxNode=&m_pNodes[i];
			max=MaxNode->GetWight();
		}
	}
	return MaxNode;
}
GraphNode* CTG::GetMaxBwNode(){
	float max=0;
	GraphNode *MaxNode;
	for(int i=0;i<m_nGrDegree;i++){
		if(m_pNodes[i].m_nTotalCV>max){
			MaxNode=&m_pNodes[i];
			max=MaxNode->m_nTotalCV;
		}
	}
	return MaxNode;
}
GraphNode* CTG::GetNextMaxBwNode(){
	float max=0;
	int id=-1;
	GraphNode *MaxNode=NULL;
	for(int i=0;i<m_nGrDegree;i++){
		if(m_pNodes[i].m_nTotalCV>max && !m_pNodes[i].m_bIsSelect4Part){
			MaxNode=&m_pNodes[i];
			max=MaxNode->m_nTotalCV;
			id=i;
		}
	}
	if(MaxNode){
		m_pNodes[id].m_bIsSelect4Part=true;
	}
	return MaxNode;
}

bool CTG::IsMapped(){
	bool bRes=true;
	for(int i=0;i<m_nGrDegree;i++){
		if(!m_pNodes[i].m_bIsMapped){
			bRes=false;
			break;
		}
	}
	return bRes;
}
void CTG::Display(){
	cout<<"\n------------------------Communication Task Graph Info--------------------------\n\n";
	cout<<"src\t\tdest\tcomm_vol\n";
	for(int i=0;i<m_nGrDegree;i++){
		m_pNodes[i].ShowStat();
	}
}
int CTG::GetManhatanDis(int n , int m){
	int nCoreId=m_pNodes[n].m_pCore->GetId();
	int mCoreId=m_pNodes[m].m_pCore->GetId();
	int d1=abs(nCoreId/(m_pPlatform->GetColSize())- mCoreId/(m_pPlatform->GetColSize()));
	int d2=abs(nCoreId%(m_pPlatform->GetColSize())- mCoreId%(m_pPlatform->GetColSize()));
	//cout<<"\nMhnDist of Task( "<<n<<", "<<m<< ")= "<<d1+d2<<endl;
	return d1+d2;
}

int CTG::GetPnltManhatanDis(){
	int d=0;
	for(int i=0;i<m_nGrDegree;i++){
		for(int j=0;j<m_pNodes[i].m_nDegree;j++){
			int nodeId=m_pNodes[i].GetId();
			int linkId=m_pNodes[i].m_pLinks[j]->GetId();
			if(linkId<nodeId) continue;
			int dis=GetManhatanDis(nodeId,linkId);
			if(dis==1) continue;
			cout<<"penalty("<<m_pNodes[i].GetName() <<","<<m_pNodes[i].m_pLinks[j]->GetName() <<")="<<dis<<"\n";
			d+=dis;
		}
	}
	return d;
}

int CTG::GetManhatanDis(){
	int d=0;
	for(int i=0;i<m_nGrDegree;i++){
		for(int j=0;j<m_pNodes[i].m_nDegree;j++){
			int nodeId=m_pNodes[i].GetId();
			int linkId=m_pNodes[i].m_pLinks[j]->GetId();
			if(linkId<nodeId) continue;
			d+=GetManhatanDis(nodeId,linkId);
		}
	}
	return d;
}
int CTG::GetMaxManhatanDis(){
	int max=0;
	int source=-1,sink=-1;

	for(int i=0;i<m_nGrDegree;i++){
		for(int j=0;j<m_pNodes[i].m_nDegree;j++){
			int nodeId=m_pNodes[i].GetId();
			int linkId=m_pNodes[i].m_pLinks[j]->GetId();
			if(linkId<nodeId) continue;
			int d=GetManhatanDis(nodeId,linkId);
			if(d>max){
				max=d;
				source=nodeId;
				sink=linkId;
			}
		}
	}
	return max;
}
void CTG::SetPlatPtr(Platform *pl){
	m_pPlatform=pl;
}
float CTG::CalcPower(float nExePow){
	float ConsumedPow=0;
	float ConsCommPow=0;
	float ConsExecPow=nExePow;
	for(int i=0;i<m_nGrDegree;i++){
		for(int j=0;j<m_pNodes[i].m_nDegree;j++){
			int nodeId=m_pNodes[i].GetId();
			int linkId=m_pNodes[i].m_pLinks[j]->GetId();
			if(linkId<nodeId) continue;
			int d=GetManhatanDis(nodeId,linkId);
			ConsCommPow+=(m_pNodes[i].GetCommVol(j))*((d+1)*m_pPlatform->GetBitRtPow()+d*m_pPlatform->GetBitLnkPow());
		}
	}
	ConsumedPow=(ConsExecPow+ConsCommPow);
	return ConsumedPow;
}

float CTG::CalcPower(){
	float ConsumedPow=0;
	float ConsCommPow=0;
	float ConsExecPow=0;
	for(int i=0;i<m_nGrDegree;i++){
		for(int j=0;j<m_pNodes[i].m_nDegree;j++){
			int nodeId=m_pNodes[i].GetId();
			int linkId=m_pNodes[i].m_pLinks[j]->GetId();
			if(linkId<nodeId) continue;
			int d=GetManhatanDis(nodeId,linkId);
			ConsCommPow+=(m_pNodes[i].GetCommVol(j))*((d+1)*m_pPlatform->GetBitRtPow()+d*m_pPlatform->GetBitLnkPow());
		}
	}
	ConsumedPow=(ConsExecPow+ConsCommPow);
	return ConsumedPow;
}
int CTG::GetSuitablePlatsize(){
	return ceil(sqrt((float)m_nGrDegree));
}
char* CTG::GetGraphName(){
	return (m_strName)? m_strName:"noName";
}
int CTG::GetDegree(){
	return m_nGrDegree;
}
GraphNode * CTG::GetPtrNode(int nodeId){
	for(int i=0;i<m_nGrDegree;i++){
		if(m_pNodes[i].GetId()==nodeId)
			return &m_pNodes[i];
	}
	return NULL;
}
void CTG::UnMap(){
	for(int i=0;i<m_nGrDegree;i++){
		m_pNodes[i].UnMap();
	}
}
void CTG::Partition(CTG &parGr, float &interCommVol){
	//CTG parGr("",NULL);
	GraphNode *n[GR_MAX_ND][4]={NULL,NULL,NULL,NULL};
	char strNode[GR_MAX_ND][10];
	interCommVol=0;
	cout<<"Partitions constructed as follow:\n";
	for(int i=0; i<m_nGrDegree;i++){
		int j=0;
		n[parGr.m_nGrDegree][0]=GetNextMaxBwNode();
		if(!n[parGr.m_nGrDegree][0]) break;
		cout<<"\n partition"<<parGr.m_nGrDegree<<": "<<n[parGr.m_nGrDegree][0]->GetName()<<"\t";
		for(; j<n[parGr.m_nGrDegree][0]->GetDegree()&&j<3&&n[parGr.m_nGrDegree][0]->HasUnSelectedChild();){ 
			n[parGr.m_nGrDegree][++j]=n[parGr.m_nGrDegree][0]->GetNextMaxChild();
			cout<<n[parGr.m_nGrDegree][j]->GetName()<<"\t";
		}
		sprintf(strNode[parGr.GetDegree()],"T%d",parGr.GetDegree());
		parGr.m_nGrDegree++;
	}		
	for(int i=0; i<parGr.GetDegree();i++){
		for(int j=0;j<3;j++){
			for(int k=j+1;k<4;k++){
				if(!n[i][j] || !n[i][k]) continue;
				pair<char*,char*> connection;
				connection=make_pair(n[i][j]->GetName(),n[i][k]->GetName());
				map<pair<char*,char*>,float> ::iterator it;
				it = m_CommLinks.find(connection);
				if (it != m_CommLinks.end()){  
					float cv=m_CommLinks[connection];
					interCommVol+=cv;
				}
			}
		}
	}
	sprintf(parGr.m_strName,"%s%s","pr_",m_strName);
	parGr.m_strNodeNames=new char*[parGr.GetDegree()];
	for(int i=0; i<parGr.GetDegree();i++){
		parGr.m_strNodeNames[i]=new char[strlen(strNode[i])];
		strcpy(parGr.m_strNodeNames[i],strNode[i]);
	}
	parGr.m_pNodes= new GraphNode[parGr.GetDegree()];
	for(int i=0;i<parGr.GetDegree();i++){
		parGr.m_pNodes[i].SetNameId(i,parGr.m_strNodeNames[i]);
	}
	for(int k=0; k<parGr.m_nGrDegree;k++){//search in previous levels for conction
		for(int p=0;p<parGr.m_nGrDegree;p++){
			if(p==k) continue;
			for(int j=0;j<4;j++){
				if(!n[k][j]) continue;
				for(int m=0; m<4;m++){
					if(!n[p][m]) continue;
					float cv=n[p][m]->GetCommVol(n[k][j]);
					if(cv!=-1){//today test
						pair<char* , char*> Connection;
						Connection=make_pair(parGr.m_pNodes[k].GetName(),parGr.m_pNodes[p].GetName());
						parGr.m_CommLinks[Connection]=cv;
						Connection=make_pair(parGr.m_pNodes[p].GetName(),parGr.m_pNodes[k].GetName());
						parGr.m_CommLinks[Connection]=cv;
						//
						parGr.m_pNodes[k].AddLink(&parGr.m_pNodes[p],cv);
					}
				}
			}
		}
	}
	for(int i=0;i<parGr.m_nGrDegree;i++){
		parGr.m_pNodes[i].CalculateTotalDeg();
		//cout<<"\nW("<<m_pNodes[i].GetName()<<")= "<<m_pNodes[i].GetWight();
	}
	cout<<"Degree="<<parGr.m_nGrDegree<<endl;

}
float CTG::GetLinkVol(GraphNode* src, GraphNode *Dest){
	pair<char*,char*> connection;
	float cv=0;
	connection=make_pair(src->GetName(),Dest->GetName());
	map<pair<char*,char*>,float> ::iterator it;
	it = m_CommLinks.find(connection);
	if (it != m_CommLinks.end()){  
		cv=m_CommLinks[connection];
	}
	if(!cv)
		cv=0;
	return cv;
}