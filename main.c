#include <stdio.h>
#include <string.h> 

typedef struct {
    char game_id[4];
    int score;
    int no_wins;
    int no_played;
    int start_date;
    int end_date;
} user_game;

typedef struct {
    int player_id;
    char pswd[9];
    char name[30];
    int reg_date;
    user_game games_played[500];
} player;

typedef struct {
    int game_id;
    char name[30];
    int num_players_reqd;
} game_master;

typedef struct {
    int game_id;
    int active_users;
    int date_start;
} game_detail;

typedef struct {
    int guest_id;
    char name[30];
    int join_date;
    user_game games_played[500];
} guest;

FILE *fptr;
fptr=fopen()

player players[200];
game_master game_masters[100];
game_detail games[100];
guest guests[300];

// to get most active users games first

void swap(game_detail *a, game_detail *b) {
    game_detail temp = *a;
    *a = *b;
    *b = temp;
}

int partition(game_detail arr[], int low, int high) {
    int pivot = arr[high].active_users;
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (arr[j].active_users > pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }

    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

void qSort(game_detail arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        qSort(arr, low, pi - 1);
        qSort(arr, pi + 1, high);
    }
}

int current_active_players = 0;
int current_active_guests = 0;
int player_id = 1;

void addPlayer() {

    char name[30];
    char pswd[9];
    int reg_date;

    printf("Enter Your Name Without Space: ");
    scanf("%s", name);
    printf("Enter Your Password of 8 Characters: ");
    scanf("%s", pswd);
    printf("Enter Date without space as DDMMYY: ");
    scanf("%d", &reg_date);
    

    players[current_active_players].player_id= player_id;
    strcpy(players[current_active_players].name, name);
    strcpy(players[current_active_players].pswd, pswd);
    players[current_active_players].reg_date = reg_date;
    player_id += 1;
    current_active_players += 1;

    printf("Account created successfully!\n");
    printf("\n--- Player Info ---\n");
    printf("ID: %d\n", players[current_active_players - 1].player_id);
    printf("Name: %s\n", players[current_active_players - 1].name);
    printf("Password: %s\n", players[current_active_players - 1].pswd);
    printf("Date: %d\n", players[current_active_players - 1].reg_date);

    //printf("Which of these popular games would you like to start with?\n");
}

int main(){

    
    addPlayer();
}