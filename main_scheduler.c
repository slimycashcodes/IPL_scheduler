#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "playoffs.h"

#define MAX_TEAMS 8
#define MAX_MATCHES 56

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

// GLOBAL ARRAYS
Team teams[MAX_TEAMS];
Match matches[MAX_MATCHES];
PointsTable points[MAX_TEAMS];
int matchCount = 0;

void initializeTeams() {
    char *teamNames[MAX_TEAMS] = {"MI", "CSK", "RCB", "KKR", "SRH", "DC", "PBKS", "RR"};
    char *venues[MAX_TEAMS] = {"Mumbai", "Chennai", "Bangalore", "Kolkata", "Hyderabad", "Delhi", "Punjab", "Jaipur"};

    for (int i = 0; i < MAX_TEAMS; i++) {
        strcpy(teams[i].name, teamNames[i]);
        strcpy(teams[i].homeVenue, venues[i]);
        strcpy(points[i].team, teamNames[i]);
        points[i].matchesPlayed = points[i].wins = points[i].losses = points[i].points = 0;
        points[i].runsScored = points[i].runsConceded = 0;
        points[i].netRunRate = 0.0;
    }
}

void generateSchedule() {
    int day = 1, month = 4;
    int pairs[MAX_TEAMS][MAX_TEAMS] = {0};
    int teamMatchCount[MAX_TEAMS] = {0}; // 🔧 NEW: track per-team match count
    int homeVenueUsed[MAX_TEAMS][MAX_TEAMS] = {0};
    int c = 0;

    while (matchCount < MAX_MATCHES) {
        if (c > 1000) {
            printf("Schedule generation failed: too many retries.\n");
            break;
        }

        int team1 = rand() % MAX_TEAMS;
        int team2 = rand() % MAX_TEAMS;

        if (team1 == team2 || pairs[team1][team2] >= 2 || 
            teamMatchCount[team1] >= 14 || teamMatchCount[team2] >= 14) {
            c++;
            continue;
        }

        if (matchCount > 0) {
            if (strcmp(teams[team1].name, matches[matchCount - 1].team1) == 0 ||
                strcmp(teams[team1].name, matches[matchCount - 1].team2) == 0 ||
                strcmp(teams[team2].name, matches[matchCount - 1].team1) == 0 ||
                strcmp(teams[team2].name, matches[matchCount - 1].team2) == 0) {
                c++;
                continue;
            }
        }

        // Assign home/away
        if (homeVenueUsed[team1][team2] == 0) {
            strcpy(matches[matchCount].team1, teams[team1].name);
            strcpy(matches[matchCount].team2, teams[team2].name);
            strcpy(matches[matchCount].venue, teams[team1].homeVenue);
            homeVenueUsed[team1][team2] = 1;
        } else {
            strcpy(matches[matchCount].team1, teams[team2].name);
            strcpy(matches[matchCount].team2, teams[team1].name);
            strcpy(matches[matchCount].venue, teams[team2].homeVenue);
        }

        // Date and time
        sprintf(matches[matchCount].date, "2025-%02d-%02d", month, day);
        sprintf(matches[matchCount].time, (matchCount % 8 == 0) ? "3:30 PM" : "7:30 PM");

        // Update trackers
        teamMatchCount[team1]++;
        teamMatchCount[team2]++;
        pairs[team1][team2]++;
        pairs[team2][team1]++;
        matchCount++;

        // Calendar shift
        if (++day > 30) {
            day = 1;
            month++;
        }

        c++;
    }
}




void updatePointsTable(char winner[], char loser[], int runsWinner, int runsLoser) {
    for (int i = 0; i < MAX_TEAMS; i++) {
        if (strcmp(points[i].team, winner) == 0) {
            points[i].matchesPlayed++;
            points[i].wins++;
            points[i].points += 2;
            points[i].runsScored += runsWinner;
            points[i].runsConceded += runsLoser;
        } else if (strcmp(points[i].team, loser) == 0) {
            points[i].matchesPlayed++;
            points[i].losses++;
            points[i].runsScored += runsLoser;
            points[i].runsConceded += runsWinner;
        }
    }
}

void calculateNRR() {
    for (int i = 0; i < MAX_TEAMS; i++) {
        if (points[i].matchesPlayed > 0) {
            float overs = points[i].matchesPlayed * 20.0;
            points[i].netRunRate = ((float)points[i].runsScored / overs) -
                                   ((float)points[i].runsConceded / overs);
        }
    }
}

void simulateMatches() {
    for (int i = 0; i < matchCount; i++) {
        int runs1 = 120 + rand() % 80;
        int runs2 = 120 + rand() % 80;

        char *winner = runs1 >= runs2 ? matches[i].team1 : matches[i].team2;
        char *loser = runs1 >= runs2 ? matches[i].team2 : matches[i].team1;
        updatePointsTable(winner, loser, runs1 > runs2 ? runs1 : runs2, runs1 > runs2 ? runs2 : runs1);
    }
    calculateNRR();
}

void sim_match(){
    int htoh[10][10] = {{}};
    for(int i=0;i<matchCount;i++){

    }
}

void displayPointsTable() {
    for (int i = 0; i < MAX_TEAMS - 1; i++) {
        for (int j = 0; j < MAX_TEAMS - i - 1; j++) {
            if (points[j].points < points[j + 1].points ||
                (points[j].points == points[j + 1].points &&
                 points[j].netRunRate < points[j + 1].netRunRate)) {
                PointsTable temp = points[j];
                points[j] = points[j + 1];
                points[j + 1] = temp;
            }
        }
    }

    printf("\n******** Final Points Table ********\n");
    printf("Team\tMP\tW\tL\tPts\tNRR\n");

    PlayoffTeam toppers[MAX_TOPTEAMS];
    for (int i = 0; i < MAX_TEAMS; i++) {
        if (i < MAX_TOPTEAMS) {
            strcpy(toppers[i].team, points[i].team);
            toppers[i].netRunRate = points[i].netRunRate;
        }
        printf("%s\t%d\t%d\t%d\t%d\t%.2f\n", points[i].team, points[i].matchesPlayed,
               points[i].wins, points[i].losses, points[i].points, points[i].netRunRate);
    }

    char user_choice;
    printf("Do you want to simulate the playoffs (y/n): ");
    scanf(" %c", &user_choice);
    if (user_choice == 'y') {
        real(toppers);
    }
}

void displaySchedule() {
    FILE *fp = fopen("schedule.txt", "w");
    printf("\n******** IPL Match Schedule ********\n");
    for (int i = 0; i < matchCount; i++) {
        if (i % 8 == 0) printf("\n");

        printf("%2d Match: %4s vs %4s | Date: %s | Time: %s | Venue: %s\n", i + 1,
               matches[i].team1, matches[i].team2, matches[i].date,
               matches[i].time, matches[i].venue);

        fprintf(fp, "%2d Match: %4s vs %4s | Date: %s | Time: %s | Venue: %s\n", i + 1,
                matches[i].team1, matches[i].team2, matches[i].date,
                matches[i].time, matches[i].venue);
    }
    fclose(fp);
}

void displayTeamSchedule(char team[]) {
    printf("\n******** %s Team Specific Schedule ********\n", team);
    for (int i = 0; i < matchCount; i++) {
        if (strcmp(matches[i].team1, team) == 0 || strcmp(matches[i].team2, team) == 0) {
            printf("%2d Match: %4s vs %4s | Date: %s | Time: %s | Venue: %s\n", i + 1,
                   matches[i].team1, matches[i].team2, matches[i].date,
                   matches[i].time, matches[i].venue);
        }
    }
}

int main() {
    srand(time(NULL));
    initializeTeams();
    generateSchedule();

    printf("Total Matches Scheduled: %d\n", matchCount);
    displaySchedule();

    char user_choice;
    printf("Do you want to see any team's specific schedule (y/n): ");
    scanf(" %c", &user_choice);
    while (user_choice == 'y') {
        char user_team[5];
        printf("Enter the team's short name in all caps: ");
        scanf("%s", user_team);
        displayTeamSchedule(user_team);
        printf("Do you want to see any other team's specific schedule (y/n): ");
        scanf(" %c", &user_choice);
    }

    simulateMatches();
    displayPointsTable();
    return 0;
}
