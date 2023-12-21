#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "smm_object.h"
#include "smm_database.h"
#include "smm_common.h"

#define BOARDFILEPATH "marbleBoardConfig.txt"
#define FOODFILEPATH "marbleFoodConfig.txt"
#define FESTFILEPATH "marbleFestivalConfig.txt"

#define MAX_NODE        100
#define MAX_PLAYER      10
#define MAX_CHARNAME    50
#define MAX_DIE         6

//board configuration parameters
static int board_nr;
static int food_nr;
static int festival_nr;

static int player_nr;

typedef struct player {
    int energy;
    int position;
    char name[MAX_CHARNAME];
    int accumCredit;
    int flag_graduate;

} player_t;

//function prototypes
void printGrades(int player);
void printPlayerStatus(player_t *cur_player);
void generatePlayers(player_t *cur_player, int n, int initEnergy);
int rolldie(int player);
void actionNode(player_t *cur_player, int player);
void goForward(player_t *cur_player, int player, int step);
int main(int argc, const char * argv[]);
float calcAverageGrade(int player);
smmObjGrade_e takeLecture(int player, char *lectureName, int credit);
void* findGrade(int player, char *lectureName);
int isGraduated(void);

void printGrades(int player) {
    // Your implementation here
    int i;
	void *gradePtr;
	for (i=0; i<smmdb_len(LISTNO_OFFSET_GRADE + player); i++) 
	{
		gradePtr = smmdb_getData(LISTNO_OFFSET_GRADE + player, i);
		printf("%s : %i\n", smmobj_getNodename(gradePtr), smmobj_getNodeGrade(gradePtr));
	}
}

void printPlayerStatus(player_t *cur_player) {
    int i;

    for (i = 0; i < player_nr; i++) {
        printf("%s : credit %i, energy %i, position %i\n",
            cur_player[i].name,
            cur_player[i].accumCredit,
            cur_player[i].energy,
            cur_player[i].position);
    }
}

void generatePlayers(player_t *cur_player, int n, int initEnergy) {
    int i;
    for (i = 0; i < n; i++) {

        //input name
        printf("input player %i's name:", i);
        scanf("%s", cur_player[i].name);
        while ((getchar()) != '\n'); // Clear input buffer

        //input position
        cur_player[i].position = 0;

        //input energy
        cur_player[i].energy = initEnergy;
        cur_player[i].accumCredit = 0;
        cur_player[i].flag_graduate = 0;
    }
}

int rolldie(int player) {
    char c;
    printf(" Press any key to roll a die (press g to see grade): ");
    c = getchar();
    fflush(stdin);
    
    if (c == 'g')
        printGrades(player);
    
    return (rand()%MAX_DIE + 1);
}

void actionNode(player_t *cur_player, int player) {
    void *boardPtr = smmdb_getData(LISTNO_NODE, cur_player[player].position);
	int type = smmObj_getNodeType( boardPtr);
	char *name = smmObj_getNodeName( boardPtr);
	void *gradePtr;
}

void goForward(player_t *cur_player, int player, int step) {
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
    int turn = 0;

    board_nr = 0;
    food_nr = 0;
    festival_nr = 0;

    srand(time(NULL));

    // 1. import parameters ---------------------------------------------------------------------------------
    // 1-1. boardConfig 
    if ((fp = fopen(BOARDFILEPATH, "r")) == NULL) {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", BOARDFILEPATH);
        getchar();
        return -1;
    }

    // Reading board component...
    // Your implementation here for reading board components

    fclose(fp);
    
    //1-2. food card config
    #if 0
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
        	cur_player[type].energy += foodObj;
        board_nr++;
    }
    fclose(fp);
    printf("Total number of food cards : %i\n", food_nr);
    #endif
    
    //1-3. festival card config
    #if 0
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

    // 2. Player configuration ---------------------------------------------------------------------------------
    do {
        printf("input player no.:");
        scanf("%d", &player_nr);
        while ((getchar()) != '\n'); // Clear input buffer
    } while (player_nr < 0 || player_nr > MAX_PLAYER);

    player_t *cur_player = (player_t*)malloc(player_nr * sizeof(player_t));
    if (!cur_player) {
        fprintf(stderr, "Memory allocation failed\n");
        return -1;
    }
    generatePlayers(cur_player, player_nr, initEnergy);

    // 3. SM Marble game starts ---------------------------------------------------------------------------------
    while (1) {
        int die_result;

        // Initial printing
        printPlayerStatus(cur_player);

        // Die rolling
        die_result = rolldie(turn);

        // Go forward
        goForward(cur_player, turn, die_result);

        // Take action at the node
        actionNode(cur_player, turn);

        // Next turn
        turn = (turn + 1) % player_nr;
    }

    free(cur_player);
    return 0;
}
