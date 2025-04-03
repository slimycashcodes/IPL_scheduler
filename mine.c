#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_TEAMS 10
#define MAX_MATCHES 90

typedef struct{
    char team[5];
    char home[10];
}TEAM;

typedef struct{
    char team1[5],team2[5],venue[10],date[10],time[10];
}MATCH;

typedef struct{
    char team[10];
    int played,won,loss,points;
    float nrr;
}POINTSTABLE;

TEAM teams[MAX_TEAMS];
MATCH matches[MAX_MATCHES];
POINTSTABLE table[MAX_TEAMS];
int match_count = 0;


void initialise_teams(){
    char *name_ptr[MAX_TEAMS] = {"MI","CSK","RCB","GT","LSG","KKR","RR","SRH","DC","PK"};
    char *venue_ptr[MAX_TEAMS] = {"MUMBAI","CHENNAI","BANGALORE","AHMEDABAD","LUCKNOW","KOLKATTA","JAIPUR","HYDERABAD","DELHI","PUNJAB"};
    for(int i=0;i<MAX_TEAMS;i++){
        strcpy(teams[i].team,name_ptr[i]);
        strcpy(teams[i].home,venue_ptr[i]);
        strcpy(table[i].team,name_ptr[i]);
        table[i].played = table[i].won = table[i].loss = table[i].points = 0;
        table[i].nrr = 0.0;
    }   
}

void shuffle(int *teamorder){
    for(int i=MAX_TEAMS-1;i>0;i--){
        int j = rand()%(i+1);
        int temp = teamorder[i];
        teamorder[i]=teamorder[j];
        teamorder[j]=temp;
    }
}

void schedule(){
    int teamorder[MAX_TEAMS];
    for(int i=0;i<MAX_TEAMS;i++) teamorder[i]=i;
    shuffle(teamorder);

    int day = 1;
    int month = 4;
    int used[MAX_TEAMS] = {0};

    while(match_count<MAX_MATCHES){
        shuffle(teamorder);
        for(int i=0 ; i<MAX_TEAMS ; i+=2){
            if(used[teamorder[i]]==0 && used[teamorder[i+1]] == 0){
                sprintf(matches[match_count].team1,"%s",teams[teamorder[i]].team);
                sprintf(matches[teamorder[i+1]],"%s",teams[match_count])
            }
        }
    }

}

int main() 
{

}