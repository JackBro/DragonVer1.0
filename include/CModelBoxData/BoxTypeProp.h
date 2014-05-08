
#ifndef _CMODEL_BOXTYPEPROP_H_
#define _CMODEL_BOXTYPEPROP_H_


#ifndef CMODELBOXDATA_DLL
	#define CMODELBOXDATA_DLL  __declspec(dllimport)
#endif

class CMODELBOXDATA_DLL CBoxTypeProp
{
public:
	char * GetFormatDesc(int fmt);
};


#endif

