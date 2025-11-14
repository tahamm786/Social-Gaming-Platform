#include <stdio.h>
#include <string.h> 
#include <time.h>    
#include <stdlib.h>
#define MAX_PLAYERS 300
#define MAX_GUESTS 300

typedef struct {
    int player_id;
    char pswd[9];
    char name[16];
    int reg_date;
} player;

typedef struct {
    int guest_id;
    char name[16];
    int join_date;
} guest;

player players[MAX_PLAYERS];
guest guests[MAX_GUESTS];

int guest_id;
int player_id;
int date;

int GAMEHUB(int id);
void GUEST_TO_PLAYER(int id);

int loadPlayerID() {
    FILE *fptr = fopen("player_ids.dat", "rb");
    

    if (fptr == NULL) {
        
        return 0;
    }

    fread(&player_id, sizeof(int), 1, fptr);
    fclose(fptr);

    if(player_id>MAX_PLAYERS){
        printf("PLAYERS EXCEEDED LIMIT!\n");
        return -1;
    }
    else{
        return player_id;}
}

int loadGuestID() {
    FILE *fptr = fopen("guest_ids.dat", "rb");
    

    if (fptr == NULL) {
        
        return 301;
    }

    fread(&guest_id, sizeof(int), 1, fptr);
    fclose(fptr);

    if(guest_id-300>MAX_GUESTS){
        printf("GUESTS EXCEEDED LIMIT!\n");
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

int SIGNUP() {

    if (player_id == -1) {
        printf("Cannot create new player — maximum player limit reached!\n");
        return 1;
    }

    FILE *fptr = fopen("players.txt", "a");
    if (fptr == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    char name[16];
    char pswd[9];
    int reg_date=date;

    printf("----------------------------------------\n");
    printf("Enter Your Name Without Space: ");
    scanf("%15s", name);
    printf("Enter Your Password of 8 Characters: ");
    scanf("%8s", pswd);
    

    int index = player_id ;

    players[index].player_id = player_id+1;
    strcpy(players[index].name, name);
    strcpy(players[index].pswd, pswd);
    players[index].reg_date = reg_date;

    fprintf(fptr, "%d\t", players[index].player_id);
    fprintf(fptr, "%s\t", players[index].name);
    fprintf(fptr, "%s\t", players[index].pswd);
    fprintf(fptr, "%d\n", players[index].reg_date);

    printf("\nAccount created successfully!\n");
    printf("\n--- Player Info ---\n");
    printf("ID: %d\n", players[index].player_id);
    printf("Name: %s\n", players[index].name);
    printf("Password: %s\n", players[index].pswd);
    printf("Date: %d\n", players[index].reg_date);
    printf("----------------------------------------\n");

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

        printf("----------------------------------------\n");
        char line[500];
        while (fgets(line, sizeof(line), fptr)) {
            printf("%s", line);
        }
        printf("----------------------------------------\n");

        fclose(fptr);

        return 1;
}

void UPDATE_ACTIVE_PLAYERS(int game_id) {
    FILE *fptr = fopen("active_games.txt", "r");
    FILE *temp = fopen("temp.txt", "w");

    if (!temp) {
        printf("Error opening temp file.\n");
        return;
    }

    int id, count;
    int found = 0;

    if (fptr == NULL) {
        fprintf(temp, "%d\t1\n", game_id);
        fclose(temp);
        rename("temp.txt", "active_games.txt");
        return;
    }

    while (fscanf(fptr, "%d %d", &id, &count) == 2) {
        if (id == game_id) {
            count++;
            found = 1;
        }
        fprintf(temp, "%d\t%d\n", id, count);
    }

    if (!found) {
        fprintf(temp, "%d\t1\n", game_id);
    }

    fclose(fptr);
    fclose(temp);

    remove("active_games.txt");
    rename("temp.txt", "active_games.txt");
}


int GAMEHUB(int id){

    int again = 1;

    while(again){

        GAMES();
        int opt;
        printf("Which game would you like to play? Enter ID of game: ");
        scanf("%d", &opt);
        
        int score = rand() % 100 + 1;
        int completed;

        if(score > 90){
            completed = 1;
        }
        else{
            completed = 0; 
        }
        printf("\n----------------------------------------\n");
        printf("YOU HAVE PLAYED! YOUR SCORE IS %d\n", score);
        if (completed)
        {
            printf("Congratulations! You completed the game!\n");
        } else {
            printf("Game played but not completed. Try again to complete it!\n");
        }

        printf("----------------------------------------\n");

        FILE *fgame = fopen("game_stats.txt", "a");
        if(fgame == NULL){
            printf("Error opening game stats file!\n");
        } else {
            fprintf(fgame, "%d\t%d\t%d\t%d\t%d\n", opt, id, score, date, completed);
            fclose(fgame);
        }

        UPDATE_ACTIVE_PLAYERS(opt);
        printf("Game Data Stored Successfully\n");

        printf("Do you want to play another game? (1 = Yes, 0 = No): ");
        scanf("%d", &again);
    }

    if (id >= 301) { 
        char choice;
        printf("----------------------------------------\n");
        printf("\nWould you like to sign up and save your progress? (y/n): ");
        scanf(" %c", &choice);
        if (choice == 'y' || choice == 'Y') {
            GUEST_TO_PLAYER(id);
        } else {
            printf("Your guest data will remain temporary and will be deleted 20 days from your first time playing.\n");
        }
    }

    return 1;
}


int LOGIN(){

    FILE *fptr = fopen("players.txt", "r");
    if (fptr == NULL) {
        printf("Error opening players file!\n");
        return 1;
    }

    int input_id = 0, pid = 0, reg_date = 0;
    char name[16], pswd[9], file_pswd[9];
    int found = 0, tries = 0;
    int exit_login = 0;

    while (!exit_login) { 
        printf("\nEnter ID (or -1 to quit): ");
        scanf("%d", &input_id);

        if (input_id == -1) {
            printf("Exiting login.\n");
            exit_login = 1;
        } else {
            rewind(fptr);
            found = 0;

            // Search for the ID in file
            while (fscanf(fptr, "%d %s %s %d", &pid, name, file_pswd, &reg_date) == 4 && !found) {
                if (pid == input_id) {
                    found = 1;
                    break;
                }
            }

            if (found) {
                tries = 3;
                while (tries > 0 && !exit_login) {
                    printf("Enter Password: ");
                    scanf(" %8s", pswd);

                    if (strcmp(pswd, file_pswd) == 0) {
                        printf("Logged in successfully! Welcome, %s.\n", name);
                        fclose(fptr);
                        GAMEHUB(pid);
                        exit_login = 1;
                    } else {
                        tries--;
                        if (tries > 0)
                            printf("Incorrect Password. Attempts left: %d\n", tries);
                        else
                            printf("Too many failed attempts.\n");
                    }
                }
            } else {
                printf("Invalid ID. Try again.\n");
            }
        }
    }

    fclose(fptr);
    return 1;
}

void CLEANUP_OLD_GUESTS() {
    FILE *f = fopen("guests.txt", "r");
    if (f == NULL) {
        printf("No guest file found.\n");
        return;
    }

    FILE *temp = fopen("temp.txt", "w");
    if (temp == NULL) {
        printf("Error creating temporary file.\n");
        fclose(f);
        return;
    }

    guest g;
    int deleted_ids[300];
    int del_count = 0;

    time_t now = time(NULL);

    while (fscanf(f, "%d %63s %d", &g.guest_id, g.name, &g.join_date) == 3) {
        struct tm tm_join = {0};

        int year = g.join_date / 10000;
        int month = (g.join_date / 100) % 100;
        int day = g.join_date % 100;

       
        tm_join.tm_year = year - 1900;
        tm_join.tm_mon  = month - 1;
        tm_join.tm_mday = day;
        tm_join.tm_hour = 0;
        tm_join.tm_min  = 0;
        tm_join.tm_sec  = 0;
        tm_join.tm_isdst = -1;

        time_t join_time = mktime(&tm_join);
        if (join_time == (time_t)-1) {
            printf("Error calculating date for guest ID %d.\n", g.guest_id);
            continue;
        }

        double diff_seconds = difftime(now, join_time);
        int diff_days = (int)(diff_seconds / (60 * 60 * 24));

        if (diff_days <= 20) {
            fprintf(temp, "%d\t%s\t%d\n", g.guest_id, g.name, g.join_date);
        } else {
            deleted_ids[del_count++] = g.guest_id;
        }
    }

    fclose(f);
    fclose(temp);

    remove("guests.txt");
    rename("temp.txt", "guests.txt");

    // --- Remove matching entries from game_stats.txt ---
    FILE *fs = fopen("game_stats.txt", "r");
    if (fs == NULL) {
        printf("No game stats file found.\n");
        printf("Guest cleanup complete — old guest accounts removed.\n");
        return;
    }

    FILE *ts = fopen("temp_stats.txt", "w");
    if (ts == NULL) {
        printf("Error creating temporary stats file.\n");
        fclose(fs);
        return;
    }

    int opt, id, score, stat_date, completed;

    while (fscanf(fs, "%d %d %d %d %d", &opt, &id, &score, &stat_date, &completed) == 5) {
        int keep = 1;
        for (int i = 0; i < del_count; i++) {
            if (id == deleted_ids[i]) {
                keep = 0;
                break;
            }
        }

        if (keep) {
            fprintf(ts, "%d\t%d\t%d\t%d\t%d\n", opt, id, score, stat_date, completed);
        }
    }

    fclose(fs);
    fclose(ts);

    remove("game_stats.txt");
    rename("temp_stats.txt", "game_stats.txt");

    printf("Guest cleanup complete : old guest accounts and their stats removed.\n");
}   



void GUEST_TO_PLAYER(int guest_id) {
    FILE *fguest = fopen("guests.txt", "r");
    if (!fguest) {
        printf("Error opening guests.txt for reading.\n");
        return;
    }

    FILE *ftemp = fopen("temp_guests.txt", "w");
    if (!ftemp) {
        printf("Error creating temporary guest file.\n");
        fclose(fguest);
        return;
    }

    FILE *fplayer = fopen("players.txt", "a");
    if (!fplayer) {
        printf("Error opening players.txt for appending.\n");
        fclose(fguest);
        fclose(ftemp);
        return;
    }

    guest g;
    int found = 0;
    int new_player_id = player_id + 1;

    while (fscanf(fguest, "%d %s %d", &g.guest_id, g.name, &g.join_date) == 3) {
        if (g.guest_id == guest_id) {
            found = 1;
            fprintf(fplayer, "%d\t%s\tguest123\t%d\n", new_player_id, g.name, date);
        } else {
            fprintf(ftemp, "%d\t%s\t%d\n", g.guest_id, g.name, g.join_date);
        }
    }

    fclose(fguest);
    fclose(ftemp);
    fclose(fplayer);

    remove("guests.txt");
    rename("temp_guests.txt", "guests.txt");

    if (!found) {
        printf("Guest record not found for migration.\n");
        return;
    }

    player_id = new_player_id;
    savePlayerID(player_id);

    FILE *fs = fopen("game_stats.txt", "r");
    if (!fs) {
        printf("No game stats file found.\n");
        return;
    }

    FILE *ts = fopen("temp_stats.txt", "w");
    if (!ts) {
        printf("Error creating temporary stats file.\n");
        fclose(fs);
        return;
    }

    int game_id, id, score, stat_date, completed;
    while (fscanf(fs, "%d %d %d %d %d", &game_id, &id, &score, &stat_date, &completed) == 5) {
        if (id == guest_id)
            id = new_player_id;
        fprintf(ts, "%d\t%d\t%d\t%d\t%d\n", game_id, id, score, stat_date, completed);
    }

    fclose(fs);
    fclose(ts);

    remove("game_stats.txt");
    rename("temp_stats.txt", "game_stats.txt");

    printf("----------------------------------------\n");
    printf("\nGuest successfully converted to Player!\n");
    printf("New Player ID: %d (Password: guest123)\n", new_player_id);
    printf("Your game history (including completion info) has been preserved.\n\n");
}



int GUEST() {

    if (guest_id == -1) {
        printf("Cannot create new guest — maximum guest limit reached!\n");
        return 1;
    }

    printf("Do you already have a Guest ID? (y/n): ");
    char choice;
    scanf(" %c", &choice);

    if (choice == 'y' || choice == 'Y') {
        int old_id;
        printf("Enter your Guest ID: ");
        scanf("%d", &old_id);

        FILE *fptr = fopen("guests.txt", "r");
        if (fptr == NULL) {
            printf("No guest records found!\n");
            return 1;
        }

        int id, join_date;
        char name[16];
        int found = 0;

        while (fscanf(fptr, "%d %15s %d", &id, name, &join_date) == 3) {
            if (id == old_id) {
                found = 1;

                int days_since_join = date - join_date; 
                if (days_since_join > 15) {
                    printf("\nYour 15-day guest access period has expired!\n");
                    printf("You joined on %d, which was %d days ago.\n", join_date, days_since_join);
                    printf("Please register as a player to continue.\n");
                    fclose(fptr);
                    return 1;
                }

                printf("\nWelcome back, %s!\n", name);
                printf("Your join date: %d\n", join_date);
                printf("Guest account valid for %d more days.\n", 15 - days_since_join);

                fclose(fptr);
                GAMEHUB(old_id);   
                return 1;
            }
        }

        fclose(fptr);
        if (!found) {
            printf("Guest ID not found or expired.\n");
        }
        return 1;
    }

    // ---------- NEW GUEST ACCOUNT CREATION ----------
    char name[16];
    int reg_date = date;

    printf("Enter Name without space: ");
    scanf("%15s", name);

    FILE *fptr = fopen("guests.txt", "a");
    if (fptr == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    int index = guest_id - 301;
    guests[index].guest_id = guest_id;
    strcpy(guests[index].name, name);
    guests[index].join_date = reg_date;

    fprintf(fptr, "%d\t%s\t%d\n", guests[index].guest_id, guests[index].name, guests[index].join_date);
    fclose(fptr);

    printf("\nAccount created successfully!\n");
    printf("\n--- Guest Info ---\n");
    printf("ID: %d\n", guests[index].guest_id);
    printf("Name: %s\n", guests[index].name);
    printf("Date: %d\n", guests[index].join_date);
    printf("Your guest account will expire in 15 days.\n");

    GAMEHUB(guest_id);

    guest_id += 1;
    saveGuestID(guest_id);

    return 1;
}



void DISPLAY_ACC(){
    int id;
    printf("\nEnter your ID to view account details: ");
    scanf("%d", &id);

    int found = 0;

    // --- Check if Player ---
    if (id <= 300) {
        FILE *fp = fopen("players.txt", "r");
        if (!fp) {
            printf("No player records found.\n");
            return;
        }

        int pid, reg_date;
        char name[16], pswd[9];
        while (fscanf(fp, "%d %s %s %d", &pid, name, pswd, &reg_date) == 4) {
            if (pid == id) {
                printf("\n===== PLAYER ACCOUNT INFO =====\n");
                printf("Player ID: %d\n", pid);
                printf("Name     : %s\n", name);
                printf("Password : %s\n", pswd);
                printf("Reg Date : %d\n", reg_date);
                found = 1;
                break;
            }
        }
        fclose(fp);
    } 
    // --- Check if Guest ---
    else {
        FILE *fg = fopen("guests.txt", "r");
        if (!fg) {
            printf("No guest records found.\n");
            return;
        }

        int gid, join_date;
        char name[16];
        while (fscanf(fg, "%d %s %d", &gid, name, &join_date) == 3) {
            if (gid == id) {
                printf("\n===== GUEST ACCOUNT INFO =====\n");
                printf("Guest ID : %d\n", gid);
                printf("Name     : %s\n", name);
                printf("Join Date: %d\n", join_date);
                printf("(Note: Guest data will expire after 20 days unless converted to Player)\n");
                found = 1;
                break;
            }
        }
        fclose(fg);
    }

    if (!found) {
        printf("No account found for ID %d.\n", id);
        return;
    }

    // --- Display Game Stats ---
    FILE *fs = fopen("game_stats.txt", "r");
    if (!fs) {
        printf("No game stats found.\n");
        return;
    }

    int game_id, pid, score, stat_date, completed;
    int stat_found = 0;

    printf("\n===== GAME HISTORY =====\n");
    printf("%s %s %s %s\n", "GameID", "Score", "Date", "Completed");
    printf("-------------------------------------\n");

    while (fscanf(fs, "%d %d %d %d %d", &game_id, &pid, &score, &stat_date, &completed) == 5) {
        if (pid == id) {
            printf("%d %d %d %s\n", 
                   game_id, score, stat_date, completed ? "Completed" : "Not Completed");
            stat_found = 1;
        }
    }

    if (!stat_found) {
        printf("No games played yet.\n");
    }

    fclose(fs);
    printf("-------------------------------------\n");
}

void ACTIVE_USERS() {
    int K;
    printf("\nEnter the minimum number of games (K): ");
    scanf("%d", &K);

    FILE *fs = fopen("game_stats.txt", "r");
    if (!fs) {
        printf("No game stats found.\n");
        return;
    }

    int id, game_id, score, date, completed;
    int play_count[605] = {0};  

    while (fscanf(fs, "%d %d %d %d %d", &game_id, &id, &score, &date, &completed) == 5) {
        if (id > 0 && id < 700)
            play_count[id]++;
    }
    fclose(fs);

    printf("\n===== ACTIVE USERS (Played more than %d games) =====\n", K);
    printf("%s %s %s\n", "UserID", "Name", "Games");
    printf("-------------------------------------------\n");

    // --- Check players file ---
    FILE *fp = fopen("players.txt", "r");
    if (fp) {
        int pid, reg_date;
        char name[16], pswd[9];
        while (fscanf(fp, "%d %s %s %d", &pid, name, pswd, &reg_date) == 4) {
            if (play_count[pid] > K) {
                printf("%-10d %-15s %-10d\n", pid, name, play_count[pid]);
            }
        }
        fclose(fp);
    }

    // --- Check guests file ---
    FILE *fg = fopen("guests.txt", "r");
    if (fg) {
        int gid, join_date;
        char name[16];
        while (fscanf(fg, "%d %s %d", &gid, name, &join_date) == 3) {
            if (play_count[gid] > K) {
                printf("%-10d %-15s %-10d\n", gid, name, play_count[gid]);
            }
        }
        fclose(fg);
    }

    printf("-------------------------------------------\n");
    printf("End of active user list.\n");
}

void INACTIVE_USERS() {
    FILE *fs = fopen("game_stats.txt", "r");
    if (!fs) {
        printf("No game stats file found.\n");
        return;
    }

    int game_id, user_id, score, date, completed;
    typedef struct {
        int game_id;
        int user_ids[700];
        int count;
    } GameInactive;

    GameInactive games[20]; 
    int gameCount = 0;

    
    while (fscanf(fs, "%d %d %d %d %d", &game_id, &user_id, &score, &date, &completed) == 5) {
        if (completed == 0) {
            int found = 0;
            for (int i = 0; i < gameCount; i++) {
                if (games[i].game_id == game_id) {
                    games[i].user_ids[games[i].count++] = user_id;
                    found = 1;
                    break;
                }
            }
            if (!found && gameCount < 50) {
                games[gameCount].game_id = game_id;
                games[gameCount].user_ids[0] = user_id;
                games[gameCount].count = 1;
                gameCount++;
            }
        }
    }
    fclose(fs);

    if (gameCount == 0) {
        printf("No inactive users found — all games completed!\n");
        return;
    }

    printf("\n===== GAME-WISE INACTIVE USERS =====\n");

    for (int i = 0; i < gameCount; i++) {
        printf("\nGame ID: %d\n", games[i].game_id);
        printf("%s %s\n", "UserID", "Name");
        printf("-------------------------\n");

        // Open both player and guest files to get names
        FILE *fp = fopen("players.txt", "r");
        FILE *fg = fopen("guests.txt", "r");

        for (int j = 0; j < games[i].count; j++) {
            int uid = games[i].user_ids[j];
            int pid, reg_date;
            char name[16], pswd[9];
            int printed = 0;

            if (fp) {
                rewind(fp);
                while (fscanf(fp, "%d %s %s %d", &pid, name, pswd, &reg_date) == 4) {
                    if (pid == uid) {
                        printf("%d %s\n", pid, name);
                        printed = 1;
                        break;
                    }
                }
            }

            if (!printed && fg) {
                int gid, join_date;
                rewind(fg);
                while (fscanf(fg, "%d %s %d", &gid, name, &join_date) == 3) {
                    if (gid == uid) {
                        printf("%d %s\n", gid, name);
                        break;
                    }
                }
            }
        }

        if (fp) fclose(fp);
        if (fg) fclose(fg);
        printf("-------------------------\n");
    }

    printf("End of inactive user report.\n");
}

void GAME_LEADERBOARD() {
    FILE *fs = fopen("game_stats.txt", "r");
    if (fs == NULL) {
        printf("No game stats file found.\n");
        return;
    }

    int G;
    printf("Enter Game ID (G): ");
    scanf("%d", &G);

    typedef struct {
        int id;
        int score;
        int date;
        int completed;
    } ActivePlayer;

    ActivePlayer list[300];
    int count = 0;

    int game_id, id, score, date, completed;

    // read stats file
    while (fscanf(fs, "%d %d %d %d %d", &game_id, &id, &score, &date, &completed) == 5) {
        if (game_id == G) {
            list[count].id = id;
            list[count].score = score;
            list[count].date = date;
            list[count].completed = completed;
            count++;
        }
    }
    fclose(fs);

    if (count == 0) {
        printf("No players found for this game.\n");
        return;
    }

    
    for (int i = 0; i < count - 1; i++) {
        int swapped = 0;
        for (int j = 0; j < count - i - 1; j++) {
            if (list[j].score < list[j + 1].score) {
                ActivePlayer temp = list[j];
                list[j] = list[j + 1];
                list[j + 1] = temp;
                swapped = 1;
            }
        }
    
        swapped = 0; 
    }

    printf("\n--- Players currently playing Game %d (sorted by score desc) ---\n", G);
    printf("ID\tScore\tDate\tCompleted\n");
    printf("-----------------------------------------\n");
    for (int i = 0; i < count; i++) {
        if (list[i].completed == 1)
            printf("%d\t%d\t%d\t\tCompleted\n", list[i].id, list[i].score, list[i].date);
        else
            printf("%d\t%d\t%d\t\tNot Completed\n", list[i].id, list[i].score, list[i].date);
    }
}

void HIGHEST_GAME_SCORES() {
    FILE *fs = fopen("game_stats.txt", "r");
    if (fs == NULL) {
        printf("No game stats file found.\n");
        return;
    }

    typedef struct {
        int game_id;
        int top_score;
    } GameScore;

    GameScore games[300];
    int count = 0;

    int game_id, id, score, date, completed;
    int found;

    while (fscanf(fs, "%d %d %d %d %d", &game_id, &id, &score, &date, &completed) == 5) {
        found = 0;
        for (int i = 0; i < count; i++) {
            if (games[i].game_id == game_id) {
                if (score > games[i].top_score) {
                    games[i].top_score = score;
                }
                found = 1;
                break;
            }
        }
        if (!found) {
            games[count].game_id = game_id;
            games[count].top_score = score;
            count++;
        }
    }
    fclose(fs);

    if (count == 0) {
        printf("No game data found.\n");
        return;
    }

    for (int i = 0; i < count - 1; i++) {
        int swapped = 0;
        for (int j = 0; j < count - i - 1; j++) {
            if (games[j].top_score < games[j + 1].top_score) {
                GameScore temp = games[j];
                games[j] = games[j + 1];
                games[j + 1] = temp;
                swapped = 1;
            }
        }
        swapped = 0;
    }

    printf("\n Top 5 Games with Highest Scores \n");
    printf("----------------------------------------\n");
    printf("Rank\tGame ID\tHighest Score\n");
    printf("----------------------------------------\n");

    int limit;
    if (count < 5)      
        limit = count;
    else
        limit = 5;

    for (int i = 0; i < limit; i++) {
        printf("%d\t%d\t%d\n", i + 1, games[i].game_id, games[i].top_score);
    }
    printf("----------------------------------------\n");
}

void PG() {
    int P, G;
    printf("\nEnter Player ID (P): ");
    scanf("%d", &P);
    printf("Enter Game ID (G): ");
    scanf("%d", &G);

    FILE *fs = fopen("game_stats.txt", "r");
    if (fs == NULL) {
        printf("No game stats file found.\n");
        return;
    }

    typedef struct {
        int id;
        int score;
        int date;
        int completed;
    } GamePlayer;

    GamePlayer list[300];
    int count = 0;

    int game_id, id, score, date, completed;

    while (fscanf(fs, "%d %d %d %d %d", &game_id, &id, &score, &date, &completed) == 5) {
        if (game_id == G && id != P) {
            list[count].id = id;
            list[count].score = score;
            list[count].date = date;
            list[count].completed = completed;
            count++;
        }
    }
    fclose(fs);

    if (count == 0) {
        printf("\nNo other players found for Game ID %d (besides Player %d).\n", G, P);
        return;
    }

    printf("\n===== OTHER PLAYERS WHO PLAYED GAME %d =====\n", G);
    printf("%-10s %-15s %-10s %-12s %-12s\n", "UserID", "Name", "Score", "Date", "Completed");
    printf("---------------------------------------------------------------\n");

    for (int i = 0; i < count; i++) {
        int id = list[i].id;
        char name[16] = "Unknown";
        int found = 0;

        FILE *fp = fopen("players.txt", "r");
        if (fp) {
            int pid, reg_date;
            char pswd[9];
            while (fscanf(fp, "%d %s %s %d", &pid, name, pswd, &reg_date) == 4) {
                if (pid == id) {
                    found = 1;
                    break;
                }
            }
            fclose(fp);
        }

        if (!found) {
            FILE *fg = fopen("guests.txt", "r");
            if (fg) {
                int gid, join_date;
                while (fscanf(fg, "%d %s %d", &gid, name, &join_date) == 3) {
                    if (gid == id) {
                        found = 1;
                        break;
                    }
                }
                fclose(fg);
            }
        }

        printf("%-10d %-15s %-10d %-12d %-12s\n",
               id, name, list[i].score, list[i].date,
               list[i].completed ? "Yes" : "No");
    }

    printf("---------------------------------------------------------------\n");
}


int EXIT(){
    printf("EXITING \n THANK YOU FOR PLAYING! COME AGAIN !\n");
    printf("----------------------------------------\n");
    exit(0);
}



int main() {

    srand(time(NULL));
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    date = ((tm.tm_year + 1900) * 10000) + ((tm.tm_mon + 1) * 100) + tm.tm_mday;

    player_id = loadPlayerID();
    guest_id = loadGuestID();
    CLEANUP_OLD_GUESTS();

    char opt = 'y';

    printf("========================================\n");
    printf("   WELCOME TO SOCIAL GAMING PLATFORM \n");
    printf("========================================\n");

    while(opt=='y'){

        printf("1. SIGNUP\n"
               "2. LOGIN\n"
               "3. GUEST\n"
               "4. GAMES\n"
               "5. DISPLAY ACC INFO\n"
               "6. DISPLAY ACTIVE USERS PLAYING >N GAMES\n"
               "7. DISPLAY INACTIVE USERS PER GAME\n"
               "8. GAME LEADERBOARD\n"
               "9. GAMES WITH HIGHEST SCORES\n"
               "p. PG\n"
               "0. EXIT\n");

        scanf(" %c", &opt);

        if(opt=='1'){
            SIGNUP();
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
            DISPLAY_ACC();
        }
        else if(opt=='6'){
            ACTIVE_USERS();
        }
        else if(opt=='7'){
            INACTIVE_USERS();
        }
        else if(opt=='8'){
            GAME_LEADERBOARD();
        }
        else if(opt=='9'){
            HIGHEST_GAME_SCORES();
        }
        else if(opt=='0'){
            EXIT();
        }
        else if(opt=='p'){
            PG();
        }
        else{
            printf("Invalid Choice\n");
        }

        printf("Would you like to continue? (y/n)\n");
        scanf(" %c", &opt);
    }

    return 1;

}
