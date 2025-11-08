#include <stdio.h>
#include <string.h> 
#include <time.h>    
#include <stdlib.h>
#define MAX_PLAYERS 200
#define MAX_GUESTS 300


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
    user_game games_played[20];
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

player players[MAX_PLAYERS];
guest guests[MAX_GUESTS];

int guest_id;
int player_id;
int date;

int GAMEHUB(int id);

int loadPlayerID() {
    FILE *fptr = fopen("player_ids.dat", "rb");
    

    if (fptr == NULL) {
        
        return 0;
    }

    fread(&player_id, sizeof(int), 1, fptr);
    fclose(fptr);

    if(player_id>MAX_PLAYERS){
        printf("PLAYERS EXCEEDED LIMIT!");
        return -1;
    }
    else{
        return player_id;}
}

int loadGuestID() {
    FILE *fptr = fopen("guest_ids.dat", "rb");
    

    if (fptr == NULL) {
        
        return 0;
    }

    fread(&guest_id, sizeof(int), 1, fptr);
    fclose(fptr);

    if(guest_id>MAX_GUESTS){
        printf("GUESTS EXCEEDED LIMIT!");
        return -1;
    }
    else{
        return guest_id;}
    
}

void savePlayerID(int id) {
    FILE *fptr = fopen("player_ids.dat", "wb");
    if (fptr != NULL) {
        fwrite(&id, sizeof(int), 1, fptr);
        fclose(fptr);
    }
}

void saveGuestID(int id) {
    FILE *fptr = fopen("guest_ids.dat", "wb");
    if (fptr != NULL) {
        fwrite(&id, sizeof(int), 1, fptr);
        fclose(fptr);
    }
}

void FileToArr(){

    FILE *fptr = fopen("players.txt", "r");
    if (fptr == NULL) {
        printf("No existing player records.\n");
        return;
    }


    for (int i = 0; i < player_id;i++){
        fscanf(fptr, "%d %s %s %d", &players[i].player_id, players[i].name, players[i].pswd, &players[i].reg_date);
    }

    fclose(fptr);
    printf("Records Loaded into Memory\n");
}

int SignUp() {
    FILE *fptr = fopen("players.txt", "a");
    if (fptr == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    char name[16];
    char pswd[9];
    int reg_date=date;

    printf("Enter Your Name Without Space: ");
    scanf("%s", name);
    printf("Enter Your Password of 8 Characters: ");
    scanf("%s", pswd);
    

    int index = player_id ;

    players[index].player_id = player_id+1;
    strcpy(players[index].name, name);
    strcpy(players[index].pswd, pswd);
    players[index].reg_date = reg_date;

    fprintf(fptr, "%d\t", players[index].player_id);
    fprintf(fptr, "%s\t", players[index].name);
    fprintf(fptr, "%s\t", players[index].pswd);
    fprintf(fptr, "%d\n", players[index].reg_date);

    printf("Account created successfully!\n");
    printf("\n--- Player Info ---\n");
    printf("ID: %d\n", players[index].player_id);
    printf("Name: %s\n", players[index].name);
    printf("Password: %s\n", players[index].pswd);
    printf("Date: %d\n", players[index].reg_date);

    player_id += 1;
    savePlayerID(player_id);

    fclose(fptr);

    GAMEHUB(player_id);

    return 1;
}

int GAMES(){
        FILE *fptr = fopen("games.txt", "r");
        if (fptr == NULL) {
            printf("Error opening file.\n");
            return 0;
        }

        char line[500];
        while (fgets(line, sizeof(line), fptr)) {
            printf("%s", line);
        }

        fclose(fptr);

        return 1;
}

int GAMEHUB(int id){

    int again = 1;

    while(again){

        int opt;
        printf("Which of the game would you like to play ? Enter ID of game\n");


        scanf("%d", &opt);
        
        int score = rand() % 100 + 1;

        printf("YOU HAVE PLAYED ! YOUR SCORE IS %d\n", score);

        FILE *fgame = fopen("game_stats.txt", "a");

        fprintf(fgame, "%d\t%d\t%d\t%d\n", opt, player_id, score, date);

        fclose(fgame);

        printf("Game Data Stored Successfully\n");

        printf("Do you want to play another game? (1 = Yes, 0 = No): ");
        scanf("%d", &again);
    }
    return 1;
}

int LOGIN(){
    printf("Login TODO");
}

int GUEST(){
    printf("GUEST TODO");
}

int EXIT(){
    printf("EXIT TODO");
}

int main() {

    srand(time(NULL));
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    date = (tm.tm_mday * 10000) + ((tm.tm_mon + 1) * 100) + (tm.tm_year % 100);


    player_id = loadPlayerID();
    guest_id = loadGuestID();
    FileToArr();

    char opt = 'y';

    while(opt=='y'){

        printf("1. SIGNUP\n"
               "2. LOGIN\n"
               "3. GUEST\n"
               "4. GAMES\n"
               "5. EXIT\n");

        scanf(" %c", &opt);

        if(opt=='1'){
            SignUp();
        }
        else if(opt=='2'){
            LOGIN();
        }
        else if(opt=='3'){
            GUEST();
        }
        else if(opt=='4'){
            GAMES();
        }
        else if(opt=='5'){
            EXIT();
        }
        else{
            printf("Invalid Choice");
        }

        printf("Would you like to continue? (y/n)");
        scanf(" %c", &opt);
    }

    return 1;

}
