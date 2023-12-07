//
//  smm_node.c
//  SMMarble
//
//  Created by Juyeop Kim on 2023/11/05.
//

#include "smm_common.h"
#include "smm_object.h"
#include <string.h>

#define MAX_NODETYPE    7
#define MAX_GRADE       9
#define MAX_NODE		100


static char smmNodeName[SMMNODE_TYPE_MAX][MAX_CHARNAME] = {
	"lecture",
	"restaurant",
	"laboratory",
	"home",
	"gotolab",
	"foodChance",
	"festival"
};

char* smmObj_getNodeName(int type)
{
	return (char*)smmNodeName[type];
}

#if 0
static char smmObj_name[MAX_NODE][MAX_CHARNAME];
static int smmObj_type[MAX_NODE];
static int smmObj_credit[MAX_NODE];
static int smmObj_energy[MAX_NODE];
static int smmObj_noNode=0;
//이거 구조체로 바꿔야 함
#endif

static int smmObj_noNode=0;

struct smmObj_t{
	char smmObj_name[MAX_CHARNAME];
	int smmObj_type;
	int smmObj_credit;
	int smmObj_energy;
};

struct smmObj_t smm_node[MAX_NODE]

//object generation
void smmObj_getNode(char* name, int type, int credit, int energy)
{
	#if 0
	strcpy(smmObj_name[smmObj_noNode],name);
    smmObj_type[smmObj_noNode] = type;
    smmObj_credit[smmObj_noNode] = credit;
    smmObj_energy[smmObj_noNode] = energy;
    // 요기도 바뀌어야함 구조체로 
    #endif
    
    strcpy(smm_node[smmObj_noNode].name, name);
    smm_node[smmObj_noNode].type= type;
    smm_node[smmObj_noNode].credit = credit;
    smm_node[smmObj_noNode].energy= energy;
    
    
    smmObj_noNode++;
}

char mmObj_getNodeName(int node_nr)
{
	return smm_node[nde_nr].type;
}

int smmObj_getNodeType(int node_nr)
{
	return smm_node[nde_nr].credit;
}
int smmObj_getNodeenergy(int node_nr)
{
	return smm_node[node_nr].energy;
}

//member retrieving



//element to string
char* smmObj_getNodeName(smmNode_e type)
{
    return smmNodeName[type];
}

char* smmObj_getGradeName(smmGrade_e grade)
{
    return smmGradeName[grade];
}

