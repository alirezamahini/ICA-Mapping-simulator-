#include "StdAfx.h"
#include "Platform.h"

Platform::Platform(void)
{
	m_pCores=NULL;
	m_nRow=0;
	m_nCol=0;
	m_pTaskGraph=NULL;
}
Platform::Platform(int nRow, int nCol, float BitRoutePow, float BitLnkPow, float fFreq, int nCommVol){
	m_nRow=nRow;
	m_nCol=nCol;
	m_pTaskGraph=NULL;
	m_pCores=new Core*[nRow*nCol];
	m_fBitRtPow=BitRoutePow;
	m_fBitlnkPow=BitLnkPow;
	for(int i=0; i<nRow*nCol; i++){
		m_pCores[i]= new Core(fFreq,nCommVol,i);
	}
	for(int i=0; i<nRow*nCol; i++){

		if(i%m_nCol!=0)
			m_pCores[i]->SetLinkPtr(LEFT,m_pCores[i-1]);
		if(i%m_nCol!=m_nCol-1)
			m_pCores[i]->SetLinkPtr(RIGHT,m_pCores[i+1]);
		if(i>=m_nCol)
			m_pCores[i]->SetLinkPtr(UP,m_pCores[i-m_nCol]);
		if(i<m_nCol*(m_nRow-1))
			m_pCores[i]->SetLinkPtr(DOWN,m_pCores[i+m_nCol]);
	}
		
}
Platform::~Platform(void)
{
	if(m_pCores) delete[] m_pCores;
}
void Platform::Display(){
	cout<<"\n\n--------------------------TaskNameAssignment-----------------------------"<<endl;
	for(int i=0;i<m_nRow*m_nCol;i++){
		if(i%m_nCol==0) cout<<endl;
		(m_pCores[i]->IsFree())?cout<<"-"<<"\t":cout<<m_pCores[i]->GetTaskName()<<"\t";
		
	}
}
Core *Platform :: GetMaxFreeCore(){
	int max=0;
	Core *cr=NULL;
	for(int i=0;i<m_nCol*m_nRow;i++){
		if(m_pCores[i]->GetLinkNum()>max && m_pCores[i]->IsFree()){
			max=m_pCores[i]->GetLinkNum();
			cr=m_pCores[i];
		}
	}
	return cr;
}
Core *Platform :: GetMaxFreeCoreOfTgRegion(CTG *tg){
	int max=0;
	Core *cr=NULL;
	for(int i=0;i<GetSize();i++){
		if(m_pCores[i]->GetGraphPtr()!=tg) continue;
		int LinkNum=m_pCores[i]->GetLinkNumOfTgRegion(tg);
		if(LinkNum>max && m_pCores[i]->IsFree()){
			max=LinkNum;			
			cr=m_pCores[i];
		}
	}
	return cr;
}
Core *Platform :: GetMinFreeCoreOfTgRegion(CTG *tg){
	int min=1000000;
	Core *cr=NULL;
	for(int i=0;i<GetSize();i++){
		if(m_pCores[i]->GetGraphPtr()!=tg) continue;
		int LinkNum=m_pCores[i]->GetLinkNumOfTgRegion(tg);
		if(LinkNum<min && m_pCores[i]->IsFree()){
			min=LinkNum;			
			cr=m_pCores[i];
		}
	}
	return cr;
}
Core *Platform :: GetNearestFreeCore(Core* pCore){
	Core *FwdCr=NULL;
	for(int i=pCore->GetId();i<m_nCol*m_nRow;i++){
		FwdCr=m_pCores[i]->GetMaxFreeCoreLinked();
		if(FwdCr){
			break;
		}
	}
	Core *BkwCr=NULL;
	for(int i=pCore->GetId();i<m_nCol*m_nRow;i++){
		BkwCr=m_pCores[i]->GetMaxFreeCoreLinked();
		if(BkwCr){
			break;
		}
	}
	Core * cr=NULL;
	if(FwdCr && !BkwCr) 
		cr=FwdCr;
	if(!FwdCr && BkwCr)
		cr=BkwCr;
	if(FwdCr && BkwCr){
		int d1= GetManhatanDis(pCore,FwdCr);
		int d2= GetManhatanDis(pCore,BkwCr);
		if(d1<=d2)
			cr=FwdCr;
		else
			cr=BkwCr;
	}

	return cr;
}
int Platform::GetManhatanDis(int nCoreId, int mCoreId){
		int d1=abs(nCoreId/GetColSize()- mCoreId/GetColSize());
	int d2=abs(nCoreId%GetColSize()- mCoreId%GetColSize());
	//cout<<"\nMhnDist of Task( "<<n<<", "<<m<< ")= "<<d1+d2<<endl;
	return d1+d2;
}
int Platform::GetManhatanDis(Core *n, Core* m){
	int nCoreId=-1;
	int mCoreId=-1;
	for(int i=0;i<GetSize();i++){
		if(m_pCores[i]->GetId()==n->GetId()){
			nCoreId=i;
			break;
		}
	}
	for(int i=0;i<GetSize();i++){
		if(m_pCores[i]->GetId()==m->GetId()){
			mCoreId=i;
			break;
		}
	}
	int d1=abs(nCoreId/GetColSize()- mCoreId/GetColSize());
	int d2=abs(nCoreId%GetColSize()- mCoreId%GetColSize());
	//cout<<"\nMhnDist of Task( "<<n<<", "<<m<< ")= "<<d1+d2<<endl;
	return d1+d2;
}
int Platform::GetSize(){
	return m_nCol*m_nRow;
}
int Platform::GetRowSize(){
	return m_nRow;
}
int Platform::GetColSize(){
	return m_nCol;
}
int Platform::GetDiameter(){
	return (m_nCol-1)*(m_nRow-1);
}
float Platform::GetBitLnkPow(){
	return m_fBitlnkPow;
}
float Platform::GetBitRtPow(){
	return m_fBitRtPow;
}
Platform* Platform:: GetFreeSubSet(int nRow, int nCol, int startPos){
	if(startPos>=GetSize()||startPos%m_nCol+nCol>m_nCol||startPos/m_nCol+nRow>m_nRow){
		//cout<<"start position Error\n;
		return NULL;
	}
	Platform* pl=new Platform(nRow,nCol,m_fBitRtPow,m_fBitlnkPow);
	
	for(int i=startPos,j=0;i<GetSize()&& j<pl->GetSize();i++){
		if(!m_pCores[i]->IsFree()) continue;
		pl->m_pCores[j]=m_pCores[i];
		int n=i;
		for(j=1;j<pl->GetSize();j++){
			if(j%nCol!=0){
				if(!m_pCores[n]->GetRightPtr()) {j=0; break;}//continue;
				if(!m_pCores[n]->GetRightPtr()->IsFree()) {j=0; break;}//continue;
				n=m_pCores[n]->GetRightPtr()->GetId();
			}else{
				if(i+m_nCol-nCol+1>=GetSize()) {j=0; break;}//continue;
				if(!m_pCores[n+m_nCol-nCol+1]->IsFree()) {j=0; break;}//continue;
				n+=m_nCol-nCol+1;
			}
			//cout<<"\nCore("<<m_pCores[i]->GetId()<<")->"<<j<<endl;
			pl->m_pCores[j]=m_pCores[n];
		}
	}
	return pl;
}
int Platform::GetCoresLinkNum(){
	int sum=0;
	for(int j=0;j<GetSize();j++){
		sum+=m_pCores[j]->GetLinkNum();
	}
	return sum;
}
Platform* Platform:: GetBestFreeSubSet(int nRow, int nCol){
	Platform* pl=NULL;//=new Platform(nRow,nCol);
	pl=GetFreeSubSet(nRow,nCol,0);
	if(!pl) return NULL;
	int max=pl->GetCoresLinkNum();
	for(int i=1;i<GetSize();i++){
		Platform* newPl=GetFreeSubSet(nRow,nCol,i);
		if(!newPl) continue;
		int sum=newPl->GetCoresLinkNum();
		if(sum>max){
			max=sum;
			pl=newPl;
		}
	}
	return pl;
}
void Platform::SetGraphPtr(CTG *ptr){
	m_pTaskGraph=ptr;
	for(int i=0;i<GetSize();i++)
		m_pCores[i]->SetGraphPtr(ptr);
}
CTG * Platform::GetGraphPtr(){
	return m_pTaskGraph;
}
void Platform::Copy(Platform *pl){
	if(pl->GetSize()>GetSize()){
		cout<<"\nsize error in copping of platform\n";
		exit(0);
	}

}
Core *Platform::GetCorePtrOfIndex(int nIdx){
	if(nIdx<GetSize())
		return m_pCores[nIdx];
	else 
		return NULL;

}
int* Platform::GetArrayofMappedNodes(CTG *tg, int &nLen){
	int *ret=new int[GetSize()];
	nLen=GetSize();
	for(int i=0;i<GetSize();i++){
		if(m_pCores[i]->IsFree()){
			ret[i]=tg->GetDegree();
		}else if(m_pCores[i]->GetGraphPtr()!=tg){
			ret[i]=tg->GetDegree();
		}else{
			ret[i]=m_pCores[i]->GetTaskId();
		}
		
	}
	return ret;
}