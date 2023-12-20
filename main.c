//
//  main.c
//  SMMarble
//
//  Created by Juyeop Kim on 2023/11/05.
//

#include <time.h>
#include <string.h>
#include "smm_object.h"
#include "smm_database.h"
#include "smm_common.h"

#define BOARDFILEPATH "marbleBoardConfig.txt"
#define FOODFILEPATH "marbleFoodConfig.txt"
#define FESTFILEPATH "marbleFestivalConfig.txt"

#define MAX_NODE 		100

//board configuration parameters
static int board_nr;
static int food_nr;
static int festival_nr;

static int player_nr;


typedef struct player{
	int energy;
	int position;
	char name[MAX_CHARNAME];
	int accumCredit;
	int flag_graduate;
	
}player_t;

static player_t cur_player[MAX_PLAYER]; //curplayer라는 배열안에 player_t 정보들이 들어감. 


#if 0
static int player_energy[MAX_PLAYER];
static int player_position[MAX_PLAYER];
static char player_name[MAX_PLAYER][MAX_CHARNAME];
#endif //아마 위에 player_t를 원래 나열한거, 구조체로 바꾸기 전. 

//function prototypes
#if 0
int isGraduated(void); //check if any player is graduated 졸업했는지 확인하는 함수 
void generatePlayers(int n, int initEnergy); //generate a new player 새로운 플레이어 생성 
void printGrades(int player); //완 //print grade history of the player 플레이어의 강의 이름, 학점, 성적을 전부 출력 
void goForward(int player, int step); //make player go "step" steps on the board (check if player is graduated)
//플레이어 이동 
void printPlayerStatus(void); //print all player status at the beginning of each turn 매턴 플레이어 상태 출력 
float calcAverageGrade(int player); //calculate average grade of the player 왜 있는거지 
smmGrade_e takeLecture(int player, char *lectureName, int credit); //take the lecture (insert a grade of the player)
//플레이어가 어떤 강의를 들었고 얻은 에너지? 
void* findGrade(int player, char *lectureName); //find the grade from the player's grade history
//성적표에서 학점 찾기 
#endif

void printGrades(int player){
	int i;
	void *gradePtr;
	for (i=0; i<smmdb_len(LISTNO_OFFSET_GRADE + player); i++) 
	{
		gradePtr = smmdb_getData(LISTNO_OFFSET_GRADE + player, i);
		printf("%s : %i\n", smmobj_getNodename(gradePtr), smmobj_getNodeGrade(gradePtr));
	}
}

void printPlayerStatus(void)
{
    int i;
     
    for (i=0;i<player_nr;i++)
    {
    	printf("%s : credit %i, energy %i, position %i\n", 
                	cur_player[i].name,
                    cur_player[i].accumCredit,
                    cur_player[i].energy,
                    cur_player[i].position);
    }
}

void generatePlayers(int n, int initEnergy)
{
	int i;
	for (i=0; i<n; i++){
		
		//input name
		printf("input player %i's name:", i); //안내 문구 입력 
		scanf("%s", cur_player[i].name);
		fflush(stdin);
	
		//input position
		//player_position= 0;
		cur_player[i].position=0;
		
		//input energy
		//cur_player[i].energy= initEnergy; //초기설정한 에너지 값;
		cur_player[i].energy = initEnergy;
		cur_player[i].accumCredit =0;
		cur_player[i].flag_graduate = 0;
	}
}


int rolldie(int player)
{
    char c;
    printf(" Press any key to roll a die (press g to see grade): ");
    c = getchar();
    fflush(stdin);
    
    if (c == 'g')
        printGrades(player);
    
    return (rand()%MAX_DIE + 1);
}
 
//action code when a player stays at a node
void actionNode(int player)
{
	void *boardPtr = smmdb_getData(LISTNO_NODE, cur_player[player].position);
	int type = smmObj_getNodeType( boardPtr);
	char *name = smmObj_getNodeName( boardPtr);
	void *gradePtr;
	
    switch(type)
    {
        //case lecture:
        case SMMNODE_TYPE_LECTURE: //강의라는 노드에 도착했을때 
        	
        	cur_player[player].accumCredit += smmObj_getNodeCredit(boardPtr); //학점 증가  
        	cur_player[player].energy -= smmObj_getNodeEnergy(boardPtr);//에너지 감소 
        	
        	gradePtr = smmObj_genObject(name,  smmObjType_grade, 0, smmObj_getNodeCredit(boardPtr), 0, cur_player[player].accumCredit);
        	smmdb_addTail(LISTNO_OFFSET_GRADE + player, gradePtr);
        	
        	break;
        	
        	
        default:
            break;
    }
}

void goFoward(int player, int step) {
	
	void *boardPtr;
	cur_player[player].position += step;
	boardPtr = smmdb_getData(LISTNO_NODE, cur_player[player].position);
	
	
	printf("%s go to node %i (name: %s)\n",
				cur_player[player].name,
				cur_player[player].position,
				smmObj_getNodeName(boardPtr));
}


int main(int argc, const char * argv[]) {
    
    FILE* fp;
    char name[MAX_CHARNAME];
    int type;
    int credit;
    int energy;
    int i;
    int initEnergy;
    int turn=0;
    
    board_nr = 0;
    food_nr = 0;
    festival_nr = 0;
    
    srand(time(NULL));
    
    
    //1. import parameters ---------------------------------------------------------------------------------
    //1-1. boardConfig 
    if ((fp = fopen(BOARDFILEPATH,"r")) == NULL)
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", BOARDFILEPATH);
        getchar();
        return -1;
    }
    
    printf("Reading board component......\n");
    while (fscanf(fp,"%s%i%i%i",name,&type, &credit, &energy)==4 ) //read a node parameter set
    {
        //store the parameter set
        void *boardObj = smmobj_genObject(name, smmObjType_board, type, credit, energy, 0);
        smmdb_addTail(LISTNO_NODE, boardObj);
        
        if (type == SMMNODE_TYPE_HOME)
        	initEnergy = energy;
        board_nr++;
    }
    fclose(fp);
    printf("Total number of board nodes : %i\n", board_nr);
    
    for ( i=0; i<board_nr; i++)
    {
    	void *boardObj = smmdb_getdata(LISTNO_NODE,i);
    	
    	
    	printf("node %i : %s, %i(%s), credit %i, energy %i\n",
					 i, smmObj_getNodename(boardObj),
					 smmObj_getNodetype(boardObj),
					 smmObj_getTypeName(smmobj_getNodeType(boardObj)),
					 smmObj_getNodeCredit(boardObj),
					 smmObj_getNodeEnergy(boardObj));
    }
    
    
    //2. food card config 
    if ((fp = fopen(FOODFILEPATH,"r")) == NULL)
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", FOODFILEPATH);
        return -1;
    }
    
    printf("\n\nReading food card component......\n");
    while (fscanf(fp,"%s%i%i",name, &type, &energy)==3) //read a food parameter set
    {
        //store the parameter set
        void *foodObj = smmobj_genfood(name, type, energy);
        smmdb_addTail(LISTNO_NODE, boardObj);
        
        if (type == SMMNODE_TYPE_RESTAURANT || type == SMMNODE_TYPE_FOODCHANCE)
        	cur_player[type].energy += ;
        board_nr++;
    }
    fclose(fp);
    printf("Total number of food cards : %i\n", food_nr);
    #endif
    
    #if 0 
    //3. festival card config 
    if ((fp = fopen(FESTFILEPATH,"r")) == NULL)
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", FESTFILEPATH);
        return -1;
    }
    
    printf("\n\nReading festival card component......\n");
    while (fscanf(fp,"%s%i",name,&type)==2) //read a festival card string
    {
        //store the parameter set
    }
    fclose(fp);
    printf("Total number of festival cards : %i\n", festival_nr);
    #endif
    
    //2. Player configuration ---------------------------------------------------------------------------------
    
    do
    {
        //input player number to player_nr
        printf("input player no.:");
    	scanf("%d", &player_nr);
    	fflush(stdin); //원하지 않는 입력 (엔터 등)을 없앰 scanf뒤에 꼭 넣는게 좋음 
    
    }
    while (player_nr<0 || player_nr > MAX_PLAYER); 
    
    //static player_t cur_player[MAX_PLAYER]; // 배열 선언을 주석 처리합니다.
	static player_t *cur_player;
    cur_player = (player_t*)malloc(player_nr*sizeof(player_t));
    generatePlayers(player_nr, initEnergy);
    
    //3. SM Marble game starts ---------------------------------------------------------------------------------
    while (1) //is anybody graduated?
    {
        int die_result;
        
        //4-1. initial printing
        printPlayerStatus();
        
        //4-2. die rolling (if not in experiment) 
        die_result = rolldie(turn);
        
        //4-3. go forward
        goForward(turn, die_result);

		//4-4. take action at the destination node of the board
        actionNode(turn);
        
        //4-5. next turn
        turn = (turn + 1)%player_nr;
        
    }
    
    
    
    system("PAUSE");
    free(cur_player);
    return 0;
}
