#include "StdAfx.h"
#include "ICA.h"
#include "stdlib.h"
#include "time.h"

ICA::ICA(CTG *tg,Platform *pl,int nDecades, int nRevRate, int nImpireNum, int nCountry)
{
	m_pTg=tg;
	m_pPlatform=pl;
	m_nDecade=nDecades;
	m_nRevRate=nRevRate;
	m_nImpireNum=nImpireNum;
	m_nCountry=nCountry;
	m_nCountryParam=pl->GetSize();
	m_nParamRange=tg->GetDegree();
	m_nInitiatedCountriesNum=0;
	m_Impires=new Country*[m_nImpireNum];
	m_pCountries=new Country[m_nCountry];
	m_fCostArray=new float [m_nCountry];
	
}
void ICA::InitCountries(int nCountries,int**CountryIdx, int nCountryParam){
	if(nCountryParam!=m_nCountryParam){
		cout<<"\nInitial countery len error\n";
		return;
	}
	if(nCountries>m_nCountry){
		cout<<"\nInitial country Num error\n";
		return;
	}
	for(int i=0; i<nCountries; i++){
		m_pCountries[i].bIsImpire=false;
		m_pCountries[i].bIsRevoluted=false;
		m_pCountries[i].pCountryParams=new GraphNode*[m_nCountryParam];
		m_pCountries[i].fCost=1000000;
		m_pCountries[i].nImpId=i;
		for(int j=0;j<m_nCountryParam;j++){
			int x=CountryIdx[i][j];
			if(x<m_nParamRange){
				m_pCountries[i].pCountryParams[j]=m_pTg->GetPtrNode(x);
			}else{
				m_DummyNode.SetNameId(x,"Dummy");
				m_pCountries[i].pCountryParams[j]=&m_DummyNode;
			}
		}
		m_fCostArray[i]=CostFunc(&m_pCountries[i]);
	}
	m_nInitiatedCountriesNum=nCountries;
}
void ICA::GenerateCountries(){
	
	srand(time(NULL));
	for(int i=m_nInitiatedCountriesNum;i<m_nCountry;i++){
		m_pCountries[i].bIsImpire=false;
		m_pCountries[i].bIsRevoluted=false;
		m_pCountries[i].pCountryParams=new GraphNode*[m_nCountryParam];
		m_pCountries[i].fCost=1000000;
		m_pCountries[i].nImpId=i;
		int *paramVals=new int[m_nCountryParam];
		for(int k=0;k<m_nCountryParam;k++){
			paramVals[k]=-1;
		}
		int x=(rand()%m_nCountryParam);
		int j=0;
		if(x<m_nParamRange){
			paramVals[j]=x;
			m_pCountries[i].pCountryParams[j++]=m_pTg->GetPtrNode(x);
		}else{
			m_DummyNode.SetNameId(x,"Dummy");
			paramVals[j]=x;
			m_pCountries[i].pCountryParams[j++]=&m_DummyNode;
		}
		while(j<m_nCountryParam){
			x=(rand()%m_nCountryParam);
			int k=0;
			for(;k<j;k++){
				if(paramVals[k]==x){
					break;
				}
			}
			if(k==j){
				if(x<m_nParamRange){
					paramVals[j]=x;
					m_pCountries[i].pCountryParams[j++]=m_pTg->GetPtrNode(x);
				}else{
					paramVals[j]=x;
					m_DummyNode.SetNameId(x,"Dummy");
					m_pCountries[i].pCountryParams[j++]=&m_DummyNode;
				}
			}
		}
		if(paramVals) delete[] paramVals;
	}
	CalcCountryCost();
}
void ICA::SortCounriesByCost(){
	for(int i=0;i<m_nCountry-1;i++){
		for(int j=i+1;j<m_nCountry;j++){
			if(m_pCountries[i].fCost>m_pCountries[j].fCost){
				Country cn=m_pCountries[i];
				m_pCountries[i]=m_pCountries[j];
				m_pCountries[j]=cn;
			}
		}
	}

}
int ICA::GetColoniesNum(int nImpCountryId){
	int n=0;
	for(int i=0;i<m_nCountry;i++){
		if(!m_pCountries[i].bIsImpire && m_pCountries[i].nImpId==nImpCountryId){
			n++;
		}
	}
	return n;
}
void ICA::SelectIpimres(){
	//sort contries by their cost
	SortCounriesByCost();
	//select the low cost countries as Impires
	for(int i=0;i<m_nImpireNum;i++){
		m_pCountries[i].bIsImpire=true;
		m_pCountries[i].nImpId=i;
		m_Impires[i]=&m_pCountries[i];
	}
	//Distribute other countries as colony to Impires
	int nTurn=0;
	for(int i=0;i<m_nCountry;i++){
		if(!m_pCountries[i].bIsImpire){
			int id=GetImpirePtr(m_Impires[nTurn])->nImpId;
			m_pCountries[i].nImpId=id;
			nTurn=(nTurn+1)% m_nImpireNum;
		}
	}
	
}
void ICA::ChangeCulture(Country &clone){
	//we want to change n random parameters of each colony to it's impire
	int n=(rand()%m_nCountryParam)+1;
	for(int k=0;k<n;k++){
		int nImp=clone.nImpId;
		//select the parameter x randomly of the impire
		int x=(rand()%m_nCountryParam);
		for(int i=0;i<m_nCountryParam;i++){
			if(clone.pCountryParams[i]==m_pCountries[nImp].pCountryParams[x]){
				clone.pCountryParams[i]=clone.pCountryParams[x];
				clone.pCountryParams[x]=m_pCountries[nImp].pCountryParams[x];
				break;
			}
		}
	}
}
void ICA::Revoloution(){
	int nRandNum=m_nRevRate*m_nCountry/100;
	for(int n=0;n<nRandNum;n++){
		int i=rand()%m_nCountry;
		if(!m_pCountries[i].bIsImpire){
			int x=(rand()%m_nCountryParam);
			int y=(rand()%m_nCountryParam);
			GraphNode * t=m_pCountries[i].pCountryParams[x];
			m_pCountries[i].pCountryParams[x]=m_pCountries[i].pCountryParams[y];
			m_pCountries[i].pCountryParams[y]=t;
		}
	}
}
void ICA::Assimilation(){
	for(int i=0;i<m_nCountry;i++){
		if(!m_pCountries[i].bIsImpire){
			ChangeCulture(m_pCountries[i]);
		}
	}
}
void ICA::CalcCountryCost(){
	for(int i=0;i<m_nCountry;i++){
		m_pCountries[i].fCost=CostFunc(&m_pCountries[i]);
	}
}
Country* ICA::GetImpirePtr(Country *imp){
	for(int i=0;i<m_nImpireNum;i++){
		if(m_Impires[i]==imp){
			return(m_Impires[i]);
		}
	}
return NULL;

}
void ICA::ChangeColoniesImpire(int OldImpireCountryId, int NewImpireCountryId){
	for(int i=0;i<m_nCountry;i++){
		if(!m_pCountries[i].bIsImpire && m_pCountries[i].nImpId==OldImpireCountryId){
			m_pCountries[i].nImpId=NewImpireCountryId;
		}
	}
	
}
void ICA::MergEqualImps(){
	Country *c=GetPowerfullImpire();
	float MinCost=c->fCost;
	for(int i=0; i<m_nImpireNum; i++){
		//test float Cost=CalcImpPower(i);
		float Cost=m_Impires[i]->fCost;
		if(Cost==MinCost && c->nImpId!=m_Impires[i]->nImpId){
			m_Impires[i]->bIsImpire=false;
			m_Impires[i]->nImpId=c->nImpId;
			ChangeColoniesImpire(i,c->nImpId);
		}
	}

}
Country* ICA::GetPowerFullColony(Country * imp,int &CountryIdx){
	Country * c=NULL;
	float min=1000000;
	if(GetColoniesNum(imp->nImpId)==0)
		return imp;
	for(int i=0;i<m_nCountry;i++){
		if(!m_pCountries[i].bIsImpire && m_pCountries[i].nImpId==imp->nImpId){
			if(m_pCountries[i].fCost<min){
				min=m_pCountries[i].fCost;
				c=&m_pCountries[i];
				CountryIdx=i;
			}
		}
	}
	return c;
}
void ICA::Exchange(){
	CalcCountryCost();
	//Find the powerfull colony of each impire
	for(int i=0;i<m_nImpireNum;i++){
		int CountryIdx=-1;
		Country * colony=GetPowerFullColony(m_Impires[i],CountryIdx );
		//swap the colony and it's Impire if the colony's cost is less than
		if(colony->fCost<m_Impires[i]->fCost){
			// i is go to imp
			colony->bIsImpire=true;
			colony->nImpId=CountryIdx;		
			//impId is go to clone
			m_Impires[i]->bIsImpire=false;
			m_Impires[i]->nImpId=CountryIdx;
			//set the Impire list pointer to new impire
			m_Impires[i]=colony;
			//update colonies nImpId of down impire to new ipmire
			ChangeColoniesImpire(i,CountryIdx);
			//cout<<"exchange->ChangeColoniesImpire\n";Show();
		}
	}
}
float ICA::CalcImpPower(int nImpId){
	int n=m_Impires[nImpId]->nImpId;
	float totalCost=0;
	for(int i=0;i<m_nCountry;i++){
		if(!m_pCountries[i].bIsImpire && m_pCountries[i].nImpId==n){
			totalCost+=m_pCountries[i].fCost;
		}
	}
	return totalCost;
}
void ICA::Show(){
	/*for(int i=0;i<m_nImpireNum;i++){
		cout<<"\nImp:"<<i<<"country: "<<m_Impires[i]->nImpId<<endl;
		for(int j=0;j<m_nCountry;j++){
			if(!m_pCountries[j].bIsImpire && m_pCountries[j].nImpId==m_Impires[i]->nImpId)
				cout<<j<<"\t";
		}
	}*/
}
Country* ICA::GetPowerfullImpire(){
	Country *c;
	float min=1000000;
	for(int i=0; i<m_nImpireNum; i++){
		//test float Cost=CalcImpPower(i);
		float Cost=m_Impires[i]->fCost;
		if(Cost<min){
			min=Cost;
			c=m_Impires[i];
		}
	}
	return c;
}

Country *ICA::GetWeekestImpire(int &weekImpId){
	Country *week;
	float max=-1;
	for(int i=0; i<m_nImpireNum; i++){
		float Cost=m_Impires[i]->fCost;
		if(Cost>max){
			max=Cost;
			week=m_Impires[i];
			weekImpId=i;
		}
	}
	return week;
}
void ICA::MoveCountry(){
	int WeekImpId;
	if(m_nImpireNum==1) return;
	//find the weekest impire and it's pointer id in impire list
	Country *week=GetWeekestImpire(WeekImpId);
	for(int i=0;i<m_nCountry;i++){
		if(!m_pCountries[i].bIsImpire && m_pCountries[i].nImpId==week->nImpId){
			m_pCountries[i].nImpId=m_Impires[(WeekImpId+1)%m_nImpireNum]->nImpId;
			break;
		}
	}
}
void ICA::EliminateWeeksImp(){
	if(m_nImpireNum==1) return;
	int weekImpId;
	Country* week=GetWeekestImpire(weekImpId);// find the weekest
	if(GetColoniesNum(week->nImpId)>1) return;
	int nTurn=m_Impires[(weekImpId+1)%m_nImpireNum]->nImpId;//choose another impire
	//change week to colony of other impire
	week->bIsImpire=false;
	week->nImpId=nTurn;
	//change the colonies of down impire to other impire
	ChangeColoniesImpire(weekImpId,nTurn);
	//cout<<"EliminateWeeksImp->ChangeColoniesImpire\n";Show();
	//eliminate the week from the impire list
	for(int i=weekImpId;i<m_nImpireNum-1;i++){
		m_Impires[i]=m_Impires[i+1];
	}
	//cout<<"Imp "<<weekImpId<<" is down\n";
	m_nImpireNum--;
	
}
void ICA::Composition(){

}
void ICA::Compet(){
	GenerateCountries();
	int nLastBestCost=-1;
	int nDecade=0;
	SelectIpimres();
	int nOldcnt=0;
	while(m_nImpireNum!=1 && nDecade!=m_nDecade && nOldcnt<10){
		Assimilation();
		Revoloution();
		Exchange();//Calulate the countries cost,exchange the strong countries with their impires
		MoveCountry(); //From the weekest Impire to another
		EliminateWeeksImp();
		nDecade++;
		int nBestCost=GetPowerfullImpire()->fCost;
		char ch[2]="\r";
		if(nBestCost!=nLastBestCost){
			strncpy(ch,"\n",2);
			cout<<"Decade= "<<nDecade<<"\tImp num= "<<m_nImpireNum<<"\tBest Cost= "<<GetPowerfullImpire()->fCost<<"\n";
			nLastBestCost=nBestCost;
			nOldcnt=0;
		}
		 else{
			 cout<<"Decade= "<<nDecade<<"\tImp num= "<<m_nImpireNum<<"\tBest Cost= "<<GetPowerfullImpire()->fCost<<"\t"<<ch;
			 nOldcnt++;
		 }
		MergEqualImps();
	}
	Map();
}
void ICA::Map(){
	m_pPlatform->SetGraphPtr(m_pTg);
	Country* c=GetPowerfullImpire();
	for(int i=0;i<m_nCountryParam;i++){
		if(c->pCountryParams[i]==&m_DummyNode){
			continue;
		}
		c->pCountryParams[i]->Map2Core(m_pPlatform->GetCorePtrOfIndex(i));
		
	}
}
ICA::~ICA(void)
{
	if(m_pCountries) delete[] m_pCountries;
	if(m_fCostArray) delete[] m_fCostArray;
}
int ICA::GetParamIdx(Country* cn, int nodeId){
	for(int i=0;i<m_nCountryParam;i++){
		if(cn->pCountryParams[i]->GetId()==nodeId){
			return i;
		}
	}
	return -1;
}
float ICA::CostFunc(Country *cn){
	float d=0;
	for(int i=0;i<m_nCountryParam;i++){
		GraphNode * nd=cn->pCountryParams[i];
		for(int j=0;j<nd->GetDegree();j++){
			int k=GetParamIdx(cn,nd->GetLink(j)->GetId());
			if(k==-1){
				cout<<"index error in ICA::costfunc\n"; 
				exit(0);
			}
			if(i>k){
				float cv=m_pTg->GetLinkVol(cn->pCountryParams[i],cn->pCountryParams[k]);
				d+=m_pPlatform->GetManhatanDis(i,k)*cv;

			}
		}
	}
	return d;
	//return max;
}