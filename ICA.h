#pragma once
#include "Platform.h"
#include "iostream"
#include "TG.h"
#include "GraphNode.h"
using namespace std;
struct Country{
	GraphNode** pCountryParams;
	bool bIsRevoluted;
	bool bIsImpire;
	int nImpId;
	float fCost;
};
/*struct Impire{
	Country	*ImpCountry;
	Country **clonies;
	float totalCost;
	int nImpId;
	int nColoniesNum;
	
};*/
class ICA
{
private:
	Platform *m_pPlatform;
	CTG *m_pTg;
	int m_nDecade;
	int m_nRevRate;
	int m_nImpireNum;
	int m_nCountry;
	int m_nCountryParam;
	int m_nParamRange;
	int m_nInitiatedCountriesNum;
	Country *m_pCountries;
	Country **m_Impires;
	//Impire *m_Impires;
	float *m_fCostArray;
	//void CalcColonyCost();
	int GetColoniesNum(int nImpCountryId);
	void SortCounriesByCost();
	void ChangeCulture(Country &clone);
	Country* GetWeekestImpire(int &weekImpId);
	Country* GetPowerfullImpire();
	float CostFunc(Country * cn);
	int GetParamIdx(Country* cn, int nodeId);
	GraphNode m_DummyNode;
	void Map();
	Country* GetPowerFullColony(Country * imp,int &CountryIdx);
	//Country* GetCPtr(Country *clnPtr);
	Country* GetImpirePtr(Country *imp);
	void ChangeColoniesImpire(int OldImpireCountryId, int NewImpireCountryId);
	void MergEqualImps();
public:
	ICA(CTG *tg,Platform *pl,int nDecades, int nRevRate, int nImpireNum, int nCountry);
	void Compet();
	void Show();
	~ICA(void);
	void GenerateCountries();
	void SelectIpimres();
	void Assimilation();
	void Revoloution();
	void Exchange();//Calulate the countries cost,exchange the strong countries with related impires
	//void CalcImpPower();
	float CalcImpPower(int nImpId);
	void MoveCountry(); //From the weekest Impire to another
	void EliminateWeeksImp();
	void Composition();
	void InitCountries(int nCountries,int** CountryIdx, int nCountryParam);
	
	void CalcCountryCost();

	
};
