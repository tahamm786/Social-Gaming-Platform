#include <stdio.h>
#include <string.h> 

typedef struct {
    int game_id;
    int player_id;
    int score;
    int no_wins;
    int no_played;
    int start_date;
    int end_date;
} user_game;

typedef struct {
    int player_id;
    char pswd[9];
    char name[16];
    int reg_date;
    user_game games_played[500];
} player;

typedef struct {
    int game_id;
    char pswd[9];
    char name[16];
    int num_players_reqd;
} game_master;

typedef struct {
    int game_id;
    int active_users;
    int date_start;
} game_detail;

typedef struct {
    int guest_id;
    char name[16];
    int join_date;
    user_game games_played[500];
} guest;

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

int current_active_guests = 0;
static int player_id = 0;

int loadLastID() {
    FILE *fptr = fopen("last_id.dat", "rb");
    int last_id = 0;

    if (fptr == NULL) {
        // File doesn’t exist yet — first run
        return 0;
    }

    fread(&last_id, sizeof(int), 1, fptr);
    fclose(fptr);
    return last_id;
}

void saveLastID(int id) {
    FILE *fptr = fopen("last_id.dat", "wb");
    if (fptr != NULL) {
        fwrite(&id, sizeof(int), 1, fptr);
        fclose(fptr);
    }
}

int addPlayer() {
    FILE *fptr = fopen("players.txt", "a");
    if (fptr == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    char name[16];
    char pswd[9];
    int reg_date;

    printf("Enter Your Name Without Space: ");
    scanf("%s", name);
    printf("Enter Your Password of 8 Characters: ");
    scanf("%s", pswd);
    printf("Enter Date without space as DDMMYY: ");
    scanf("%d", &reg_date);
    
    if (player_id == 0) {
        player_id = loadLastID() + 1;
    }

    int index = player_id - 1;

    players[index].player_id = player_id;
    strcpy(players[index].name, name);
    strcpy(players[index].pswd, pswd);
    players[index].reg_date = reg_date;

    fprintf(fptr, "ID: %d\n", players[index].player_id);
    fprintf(fptr, "Name: %s\n", players[index].name);
    fprintf(fptr, "Password: %s\n", players[index].pswd);
    fprintf(fptr, "Date: %d\n\n", players[index].reg_date);

    printf("Account created successfully!\n");
    printf("\n--- Player Info ---\n");
    printf("ID: %d\n", players[index].player_id);
    printf("Name: %s\n", players[index].name);
    printf("Password: %s\n", players[index].pswd);
    printf("Date: %d\n", players[index].reg_date);

    saveLastID(player_id);

    fclose(fptr);
    return 1;
}

int main() {
    addPlayer();
}
