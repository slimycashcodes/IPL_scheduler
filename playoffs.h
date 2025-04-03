#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#ifndef PLAYOFFS_H
#define PLAYOFFS_H

#define MAX_TOPTEAMS 4

typedef struct {
    char team[20];
    float netRunRate;
} PlayoffTeam;

#endif // PLAYOFFS_H

// Function to simulate a playoff match
char* simulateMatch(PlayoffTeam team1, PlayoffTeam team2) {
    int winnerIndex = rand() % 2;
    return (winnerIndex == 0) ? team1.team : team2.team;
}

// Function to simulate the IPL playoffs
void simulatePlayoffs(PlayoffTeam topTeams[]) {
    char qualifier1Winner[20], qualifier1Loser[20], eliminatorWinner[20], qualifier2Winner[20], champion[20];

    // **Qualifier 1: Rank 1 vs Rank 2**
    printf("\nQualifier 1: %s vs %s\n", topTeams[0].team, topTeams[1].team);
    strcpy(qualifier1Winner, simulateMatch(topTeams[0], topTeams[1]));
    strcpy(qualifier1Loser, (strcmp(qualifier1Winner, topTeams[0].team) == 0) ? topTeams[1].team : topTeams[0].team);
    printf("Winner: %s (Advances to Final)\n", qualifier1Winner);
    printf("Loser: %s (Goes to Qualifier 2)\n\n", qualifier1Loser);

    // **Eliminator: Rank 3 vs Rank 4**
    printf("Eliminator: %s vs %s\n", topTeams[2].team, topTeams[3].team);
    strcpy(eliminatorWinner, simulateMatch(topTeams[2], topTeams[3]));
    printf("Winner: %s (Goes to Qualifier 2)\n", eliminatorWinner);
    printf("Loser: %s (Eliminated)\n\n", (strcmp(eliminatorWinner, topTeams[2].team) == 0) ? topTeams[3].team : topTeams[2].team);


    PlayoffTeam teamA = {"", 0}, teamB = {"", 0};
    strcpy(teamA.team, qualifier1Loser);
    strcpy(teamB.team, eliminatorWinner);
    
    printf("Qualifier 2: %s vs %s\n", teamA.team, teamB.team);
    strcpy(qualifier2Winner, simulateMatch(teamA, teamB));
    printf("Winner: %s (Advances to Final)\n\n", qualifier2Winner);

    PlayoffTeam finalA = {"", 0}, finalB = {"", 0};
    strcpy(finalA.team, qualifier1Winner);
    strcpy(finalB.team, qualifier2Winner);

    printf("FINAL: %s vs %s\n", finalA.team, finalB.team);
    strcpy(champion, simulateMatch(finalA, finalB));
    printf("Champion: %s \n", champion);
}

// Main Function
int real(PlayoffTeam *topTeams) {
    srand(time(NULL));

    printf("\n***** IPL Playoffs Simulation *****\n");
    simulatePlayoffs(topTeams);

    return 0;
}
