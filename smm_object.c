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

char* smmObj_getTypeName(int type)
{
	return (char*)smmNodeName[type];
}


/*
typedef enum smmObjGrade {
	smmObjGrade_Ap = 0,
	smmObjGrade_A0,
	smmObjGrade_Am,
	smmObjGrade_Bp,
	smmObjGrade_B0,
	smmObjGrade_Cp,
	smmObjGrade_C0,
	smmObjGrade_Cm
} smmObjGrade_e;
*/
/*
static char smmObj_name[MAX_NODE][MAX_CHARNAME];
static int smmObj_type[MAX_NODE];
static int smmObj_credit[MAX_NODE];
static int smmObj_energy[MAX_NODE];
static int smmObj_noNode=0;
*/
//�̰� ����ü�� �ٲ�� ��

typedef struct smmObject{
	char name[MAX_CHARNAME];
	smmObjType_e objType;
	int type;
	int credit;
	int energy;
	smmObjGrade_e grade;
} smmObject_t;

//struct smmObj_t smm_node[MAX_NODE];
//static int smmObj_noNode=0;

//object generation
void* smmObj_getNode(char* name, smmObjType_e objType, int type, int credit, int energy, smmObjGrade_e grade)
{
	smmObject_t* ptr;
	
	ptr = (smmObject_t*)malloc(sizeof(smmObject_t));
	
	strcpy(ptr->name ,name);
	ptr->objType = objType;
	ptr->type = type;
	ptr->credit = credit;
	ptr->energy = energy;
	ptr->grade = grade;
	
	return (void*)ptr;
   
}
char* smmObj_getNodeName(void* obj)
{
	smmObject_t* ptr = (smmObject_t*)obj;
	
	return ptr->name;
}

int smmObj_getNodeType(void* obj)
{
	smmObject_t* ptr = (smmObject_t*)obj;
	
	return ptr->type;
}


int smmObj_getNodeCredit(void* obj )
{
	smmObject_t* ptr = (smmObject_t*)obj;
	
	return ptr->credit;
}
int smmObj_getNodeenergy(void* obj)
{
	smmObject_t* ptr = (smmObject_t*)obj;
	
	return ptr->energy;
}

//member retrieving



//element to string




