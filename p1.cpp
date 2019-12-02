//Defines the entry point for the console application.
//

#include "stdafx.h"
#include "platform.h"
#include "TG.h"
#include "GraphNode.h"
#include "ICA.h"
#include "timer.h"
#define RT_CONSUMED_POW	0.3935
#define LINK_CONSUMED_POW	0.0798	
#define DECADE_NUM	200
#define IMP_NUM		10
#define	CNT_NUM		100
#define REVUL_RATE	20
#define GRAPH_NAME	"./Graphs/vopd.gr"

int _tmain(int argc, _TCHAR* argv[])
{
	Platform pl(4,4,RT_CONSUMED_POW,LINK_CONSUMED_POW);
	CTG tg(GRAPH_NAME,&pl);
	tg.InitGraph();
	CTimer Timer;
	cout<<"\n\n--------------------------MApping Details--------------------------------\n";
	Timer.Start();
	ICA ICAM(&tg,&pl,DECADE_NUM,REVUL_RATE,IMP_NUM,CNT_NUM);
	ICAM.Compet();
	unsigned long int t=Timer.GetElapsedTime();
	Timer.Stop();
	pl.Display();	
	cout<<"\n\n---------------------------PENALTIES-------------------------------------\n";
	cout<<"\n\nPenalty Manhatan Distance ="<<tg.GetPnltManhatanDis()<<endl;
	cout<<"\n\n============================RESULTS======================================\n";
	cout<<"\tManhatan Distance\t\tConsumed Power(microj)\n";
	cout<<"-------------------------------------------------------------------------\n";
	cout<<"Total\t\tMaximum\t\tIntet_Tiles\tNetwork\t\tTotal\n";
	cout<<"-------------------------------------------------------------------------\n";
	cout<<"\n"<<tg.GetManhatanDis()<<"\t\t"<<tg.GetMaxManhatanDis()<<"\t\t";
	cout<<0<<"\t\t"<<tg.CalcPower()<<"\t\t"<<tg.CalcPower()<<endl;
	cout<<"\n\=========================================================================\n";
	cout<<"Total Mapping Time is: "<<t/1000<<"(msec)\n";
	Platform pl1(2,3,RT_CONSUMED_POW,LINK_CONSUMED_POW);
	CTG tg1(GRAPH_NAME,&pl);
	tg1.InitGraph();
	CTG parTg("", &pl1);
	float InCom=0;
	Timer.Start();
	tg1.Partition(parTg,InCom);
	t=Timer.GetElapsedTime();
	Timer.Stop();
	parTg.Display();
	cout<<"Partition Time is: "<<t/1000<<"(msec)\n";
	cout<<"\n\n--------------------------MApping Details--------------------------------\n";
	Timer.Start();
	ICA PICAM(&parTg,&pl1,DECADE_NUM,REVUL_RATE,IMP_NUM,CNT_NUM);
	PICAM.Compet();
	t=Timer.GetElapsedTime();
	Timer.Stop();
	pl1.Display();	
	cout<<"\n\n---------------------------PENALTIES-------------------------------------\n";
	cout<<"\n\nPenalty Manhatan Distance ="<<parTg.GetPnltManhatanDis()<<endl;
	cout<<"\n\n============================RESULTS======================================\n";
	cout<<"\tManhatan Distance\t\tConsumed Power(microj)\n";
	cout<<"-------------------------------------------------------------------------\n";
	cout<<"Total\t\tMaximum\t\tIntet_Tiles\tNetwork\t\tTotal\n";
	cout<<"-------------------------------------------------------------------------\n";
	cout<<"\n"<<parTg.GetManhatanDis()<<"\t\t"<<parTg.GetMaxManhatanDis()<<"\t\t";
	cout<<InCom*(RT_CONSUMED_POW+8*LINK_CONSUMED_POW)<<"\t\t"<<parTg.CalcPower()<<"\t\t"<<parTg.CalcPower(InCom*(10*LINK_CONSUMED_POW))<<endl;
	cout<<"\n\=========================================================================\n";
	cout<<"Total Mapping Time is: "<<t/1000<<"(msec)\n";
	return 0;
}

