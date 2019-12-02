#pragma once
#include "Core.h"
class Core;
class Link
{
private:
	int m_nCommVol;
	Core *m_pCores[2];
public:
	Link(void);
	~Link(void);
};
