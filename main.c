#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_TEAMS 8
#define MAX_MATCHES 56

// Structure Definitions
typedef struct {
    char name[20];
    char homeVenue[30];
} Team;

typedef struct {
    char team1[20], team2[20];
    char venue[30];
    char date[15];
    char time[10];
} Match;

typedef struct {
    char team[20];
    int matchesPlayed, wins, losses, points;
    int runsScored, runsConceded;
    float netRunRate;
} PointsTable;

// Global Variables
Team teams[MAX_TEAMS];
Match matches[MAX_MATCHES];
PointsTable points[MAX_TEAMS];
int matchCount = 0;

// Function to initialize teams
void initializeTeams() {
    char *teamNames[MAX_TEAMS] = {"MI", "CSK", "RCB", "KKR", "SRH", "DC", "PBKS", "RR"};
    char *venues[MAX_TEAMS] = {"Mumbai", "Chennai", "Bangalore", "Kolkata", "Hyderabad", "Delhi", "Punjab", "Jaipur"};
    
    for (int i = 0; i < MAX_TEAMS; i++) {
        strcpy(teams[i].name, teamNames[i]);
        strcpy(teams[i].homeVenue, venues[i]);
        strcpy(points[i].team, teamNames[i]);
        points[i].matchesPlayed = 0;
        points[i].wins = 0;
        points[i].losses = 0;
        points[i].points = 0;
        points[i].runsScored = 0;
        points[i].runsConceded = 0;
        points[i].netRunRate = 0.0;
    }
}


void shuffleTeams(int *teamOrder) {
    for (int i = MAX_TEAMS - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = teamOrder[i];
        teamOrder[i] = teamOrder[j];
        teamOrder[j] = temp;
    }
}

// Function to generate a random match schedule
void generateSchedule() {
    int teamOrder[MAX_TEAMS];
    for (int i = 0; i < MAX_TEAMS; i++) teamOrder[i] = i;
    shuffleTeams(teamOrder);
    
    int day = 1, month = 4;
    int used[MAX_TEAMS] = {0};
    
    while (matchCount < MAX_MATCHES) {
        shuffleTeams(teamOrder);
        for (int i = 0; i < MAX_TEAMS; i += 2) {
            if (used[teamOrder[i]] == 0 && used[teamOrder[i + 1]] == 0) {
                sprintf(matches[matchCount].team1, "%s", teams[teamOrder[i]].name);
                sprintf(matches[matchCount].team2, "%s", teams[teamOrder[i + 1]].name);
                sprintf(matches[matchCount].venue, "%s", teams[teamOrder[i]].homeVenue);
                sprintf(matches[matchCount].date, "2025-%02d-%02d", month, day);
                sprintf(matches[matchCount].time, (matchCount % 7 == 0) ? "3:30 PM" : "7:30 PM");
                matchCount++;
                if (day >= 30) {
                    month++;
                    day = 1;
                }
                day += 1;
                used[teamOrder[i]] = 1;
                used[teamOrder[i + 1]] = 1;
            }
        }
        memset(used, 0, sizeof(used));
    }
}

// Function to update points table and NRR
void updatePointsTable(char winner[], char loser[], int runsWinner, int runsLoser) {
    for (int i = 0; i < MAX_TEAMS; i++) {
        if (strcmp(points[i].team, winner) == 0) {
            points[i].wins++;
            points[i].points += 2;
            points[i].runsScored += runsWinner;
            points[i].runsConceded += runsLoser;
        } else if (strcmp(points[i].team, loser) == 0) {
            points[i].losses++;
            points[i].runsScored += runsLoser;
            points[i].runsConceded += runsWinner;
        }
        points[i].matchesPlayed++;
    }
}

// Function to calculate Net Run Rate (NRR)
void calculateNRR() {
    for (int i = 0; i < MAX_TEAMS; i++) {
        if (points[i].matchesPlayed > 0) {
            points[i].netRunRate = (float)(points[i].runsScored - points[i].runsConceded) / points[i].matchesPlayed;
        }
    }
}

// Function to simulate matches
void simulateMatches() {
    for (int i = 0; i < matchCount; i++) {
        int winnerIndex = rand() % 2;
        char winner[20], loser[20];

        // Generate random runs scored for both teams
        int runs1 = 120 + rand() % 80;  // Between 120 and 200
        int runs2 = 120 + rand() % 80;  // Between 120 and 200

        if (winnerIndex == 0) {
            strcpy(winner, matches[i].team1);
            strcpy(loser, matches[i].team2);
        } else {
            strcpy(winner, matches[i].team2);
            strcpy(loser, matches[i].team1);
            int temp = runs1;
            runs1 = runs2;
            runs2 = temp;
        }

        updatePointsTable(winner, loser, runs1, runs2);
    }

    calculateNRR();
}

// Function to display points table (sorted by points and NRR)
void displayPointsTable() {
    // Sort teams by points first, then by NRR
    for (int i = 0; i < MAX_TEAMS - 1; i++) {
        for (int j = 0; j < MAX_TEAMS - i - 1; j++) {
            if (points[j].points < points[j + 1].points ||
                (points[j].points == points[j + 1].points && points[j].netRunRate < points[j + 1].netRunRate)) {
                PointsTable temp = points[j];
                points[j] = points[j + 1];
                points[j + 1] = temp;
            }
        }
    }

    printf("\n******** Final Points Table ********\n");
    printf("Team\tMP\tW\tL\tPts\tNRR\n");
    for (int i = 0; i < MAX_TEAMS; i++) {
        printf("%s\t%d\t%d\t%d\t%d\t%.2f\n", points[i].team, points[i].wins+points[i].losses,
               points[i].wins, points[i].losses, points[i].points, points[i].netRunRate);
    }
}

// Function to display match schedule
void displaySchedule() {
    printf("\n******** IPL Match Schedule ********\n");
    for (int i = 0; i < matchCount; i++) {
        printf("Match: %s vs %s | Date: %s | Time: %s | Venue: %s\n",
               matches[i].team1, matches[i].team2, matches[i].date, matches[i].time, matches[i].venue);
    }
}

// Main Function
int main() {
    srand(time(NULL));
    initializeTeams();
    generateSchedule();
    displaySchedule();
    
    simulateMatches(); 
    displayPointsTable(); 
    
    return 0;
}
