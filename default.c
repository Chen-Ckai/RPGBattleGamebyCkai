//Recoded balatro
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

//declaring 3 strucutres

struct Weapon{
    int id;
    char name[40];
    float damage;
    int cooldown;
    int cost;
    int hitchance;
}; //6 attributes

struct Defence{
    int id;
    char name[40];
    float hitpoints;
    float DamageDeduction;
    int cost;
}; //5 attributes

struct Player{
    char name[40];
    float HP;
    int gold;
    int consecutive_rest;
    struct Weapon owned_weapons[10];
    struct Defence owned_defences[10];
    struct Defence equipped_defence;
};

struct Player players[2] = {
        {"Player 1", 1000, 1000, 0, {0}, {0}, {0}},
        {"Player 2", 1000, 1000, 0, {0}, {0}, {0}}
};

struct Weapon WColl[10] = { //Mentions of WColl in the code refer to this array of weapons
    {1, "Tachyon Lance", 3000, 20, 1500, 75}, 
    {2, "Plasma Lanucher", 400, 2, 700, 100},
    {3, "Zip Zap", 50, 1, 500, 100},
    {4, "Gauss Canon", 1000, 5, 1500, 50}, 
    {5, "Cabine Gun", 300, 1, 1000, 100},
    {6, "Hand Gun", 100, 2, 500, 100},
    {7, "Dark Saber", 2000, 5, 1000, 90},
    {8, "Plasma Knife", 300, 2, 500, 100},
    {9, "Reaper's Scythe", 1500, 5, 800, 95},
    {10, "Aetherophasic Engine", 2000, 1, 800, 85}
};

struct Defence DefColl[10] = { //All mentions of DefColl in the code refer to this array of defences
    {1, "Dark Matter Shield", 4000, 1, 1100},
    {2, "Energy Deflector", 2000, 1, 800},
    {3, "Basic Sheild", 700, 1, 500},
    {4, "Gravitational Refractor", 1000, 0.5, 1100},
    {5, "Hyper Array", 1000, 0.8, 800},
    {6, "Pulsing Array", 500, 0.9, 500},
    {7, "Zero-point Armor", 300, 0.3, 1000},
    {8, "Power Armor", 300, 0.4, 800},
    {9, "Space Armor", 200, 0.6, 500},
    {10, "Dimensional Lens", 500, 0.2, 800}  
};

int Ran_100(){ //Random generation for all chance mechanics
    return rand() % 100 + 1;
}

void Clear(){ //Clear Terminal, purely cosmetic
    printf("\e[1;1H\e[2J");
}

bool Own_Item(int p, int num, char i_type){
    bool do_u_have_it = false;
    if (i_type == 'w'){
        if (players[p].owned_weapons[num].id != 0){
            do_u_have_it = true;
        }
    }
    else{
        if (players[p].equipped_defence.id != 0){
            do_u_have_it = true;
        }
        else if (players[p].owned_defences[num].id != 0){
            do_u_have_it = true;
        }
    }
    return do_u_have_it;
}

int ATK(int p, int opp){ //Attack enemy function, takes in player index and opponent index as parameters
    printf("Attacking enemy. They currently have %g HP.\n", players[opp].HP);
    printf("Owned weapons:\n");
    printf("%-10s%-25s%-20s%-20s%-20s\n", "ID", "Name", "Damage", "Cooldown", "Hit Chance");
    for (int i = 0; i < 10; i++) {
        if (Own_Item(p, i, 'w')) { //Only display weapons the player owns
            struct Weapon w = players[p].owned_weapons[i];
            printf("%-10i%-25s%-20.0f%-20i%i%%\n", w.id, w.name, w.damage, w.cooldown, w.hitchance);
        }
    }
    printf("Enter a weapon's ID to attack with it:\n\n");
    int choice;
    scanf("%i", &choice);
    choice--; //Adjust for 0 indexing
    if (choice == -1){
        Clear();
        printf("Exit code entered. Returning to menu.\n\n");
        return -1;
    }
    else if (choice > 9 || choice < 0 || players[p].owned_weapons[choice].id == 0) { //Invalid input
        Clear();
        printf("Invalid weapon choice. Returning to menu.\n\n");
        return -1;
    } else {
        bool has_def = false;
        float dmg = players[p].owned_weapons[choice].damage;
        float def_dmg = dmg * players[opp].equipped_defence.DamageDeduction;
        if (Own_Item(opp, 0, 'd')){
            has_def = true;
            dmg *= (1 - players[opp].equipped_defence.DamageDeduction);
        }
        if (Ran_100() >= players[p].owned_weapons[choice].hitchance) {
            Clear();
            printf("A swing and a miss.\n\n");
        }
        else {
            players[opp].HP -= dmg;
            Clear();
            printf("Your attack hit! You dealt %g damage to your opponent. They have %g HP remaining.\n\n", dmg, players[opp].HP);
            if (has_def){
                players[opp].equipped_defence.hitpoints -= def_dmg;
                if (players[opp].equipped_defence.hitpoints <= 0){
                    printf("Your opponent's equipped defence broke!\n\n");
                    players[opp].equipped_defence = (struct Defence){0}; //Reset equipped defence
                }
                else {
                    printf("Your opponent's equipped defence took %g damage. It has %g hit points remaining.\n\n", def_dmg, players[opp].equipped_defence.hitpoints);
                }
            }
        }
    }
    players[p].consecutive_rest = 0; //reset cons rest when doing anything besides resting, only activate after the entire action is done tho in case of changed mind
    return 0;
}

int DEF(int p, int opp){
    Clear();
    printf("Equipping defence. You currently have %g HP.\n", players[p].HP);
    printf("Owned defences:\n\n");
    printf("%-10s%-25s%-20s%-20s\n", "ID", "Name", "Hit Points", "Damage Deduction");
    for (int i = 0; i < 10; i++) {
        if (players[p].owned_defences[i].id != 0){
            struct Defence d = players[p].owned_defences[i];
            printf("%-10i%-25s%-20g%-20.1f\n", d.id, d.name, d.hitpoints, d.DamageDeduction);
        }
    }
    printf("Enter a defence's ID to equip it:\n\n");
    int choice;
    scanf("%i", &choice);
    choice--; 
    if (choice == -1){
        Clear();
        printf("Exit code entered. Returning to menu.\n\n");
        return -1;
    }
    else if (choice > 9 || choice < 0 || !Own_Item(p, choice, 'd')) { //Invalid input
        Clear();
        printf("Invalid defence choice. Returning to menu.\n\n");
        return -1;
    } else {
        Clear();
        struct Defence placeholder = players[p].owned_defences[choice];
        players[p].owned_defences[choice] = players[p].equipped_defence;        //Switch equipped defence with chosen
        players[p].equipped_defence = placeholder;
        printf("Equipped %s!\n", players[p].equipped_defence.name);
    }
    players[p].consecutive_rest = 0; //reset cons rest when doing anything besides resting, only activate after the entire action is done tho in case of changed mind
    return 0;
}

int SHOP(int p, int opp){
    Clear();
    printf("Welcome to the shop. What kind of item would you like to buy? (1. Weapon, 2. Defence)\n");
    int item_type;
    scanf("%i", &item_type);
    if (item_type == 0){
        Clear();
        printf("Exit code entered. Returning to menu.\n");
        return -1;
    }
    else if (item_type != 1 && item_type != 2) {
        Clear();
        printf("Invalid item type. Returning to menu.\n");
        return -1;
    }
    else {
        Clear();
        printf("You currently have %i gold. \n\n", players[p].gold);
        switch(item_type){
            case 1:                            //Weapon shop
                printf("Here are the weapons available for purchase:\n\n");
                printf("%-10s%-30s%-20s%-20s%-20s%-20s\n", "ID", "Name", "Damage", "Cooldown", "Hit Chance (%)", "Cost (in gold)");
                for (int i = 0; i < 10; i++) {
                    if (players[p].owned_weapons[i].id == 0) { //Only display weapons the player doesn't already own
                        struct Weapon w = WColl[i];
                        printf("%-10i%-30s%-20g%-20i%-20i%-20i\n", w.id, w.name, w.damage, w.cooldown, w.hitchance, w.cost);
                    }
                }
                printf("\nPlease enter the ID of the weapon you wish to purchase:\n");
                int w_choice;
                scanf("%i", &w_choice);
                w_choice--; //Adjust for 0 indexing
                if (w_choice == -1){
                    Clear();
                    printf("Exit code entered. Returning to menu.\n");
                    return -1;
                }
                else if (w_choice > 9 || w_choice < 0 || players[p].owned_weapons[w_choice].id != 0) { //Invalid input
                    Clear();
                    printf("Invalid weapon choice. Returning to menu.\n");
                    return -1;
                } else if (players[p].gold < WColl[w_choice].cost) { //Not enough gold
                    Clear();
                    printf("You don't have enough gold to purchase this weapon (too broke). Returning to menu.\n");
                    return -1;
                } else {
                    Clear();
                    players[p].gold -= WColl[w_choice].cost;
                    players[p].owned_weapons[w_choice] = WColl[w_choice];
                    printf("Purchased %s!\n", WColl[w_choice].name);
                }
                break;
            case 2:                                    //Defence shop
                printf("Here are the defences available for purchase:\n\n");
                printf("%-5s%-30s%-20s%-20s%-20s\n", "ID", "Name", "Hit Points", "Damage Deduction", "Cost (in gold)");
                for (int i = 0; i < 10; i++){
                    if (players[p].owned_defences[i].id == 0){
                        struct Defence d = DefColl[i];
                        printf("%-5i%-30s%-20g%g%%%20i\n", d.id, d.name, d.hitpoints, d.DamageDeduction * 100, d.cost);
                    }
                }
                printf("\nPlease enter the ID of the defence you wish to purchase:\n");
                int d_choice;
                scanf("%i", &d_choice);
                d_choice--; //Adjust for 0 indexing
                if (d_choice == -1){
                    Clear();
                    printf("Exit code entered. Returning to menu.\n\n");
                    return -1;
                }
                else if (d_choice > 9 || d_choice < 0 || players[p].owned_weapons[d_choice].id != 0){
                    Clear();
                    printf("Invalid defence choice. Returning to menu.\n\n");
                    return -1;
                }
                else if (players[p].gold < DefColl[d_choice].cost){
                    Clear();
                    printf("My friend, you are too broke for this defence item. Returning to menu.\n\n");
                    return -1;
                }
                else {
                    Clear();
                    players[p].owned_defences[d_choice] = DefColl[d_choice];
                    printf("Purchased %s!\n\n", DefColl[d_choice].name);
                }
                break;
        }
    }
    return 0;
}

int REST(int p){
    getchar(); //Buffer from int main enter press
    printf("Would you like to rest? (y/n)\n");
    char r_choice;
    scanf("%c", &r_choice);
    if (r_choice == 'n'){
        Clear();
        printf("ok sure\n\n");
        return -1;
    }
    else{
        Clear();
        int gold_gain = 100 + 50 * players[p].consecutive_rest;
        players[p].consecutive_rest++;
        printf("You gained %i gold from resting. Consecutive rest count is at %i. \n", gold_gain, players[p].consecutive_rest);
        players[p].gold += gold_gain;
        printf("You have %i gold in total. \n\n", players[p].gold);
    }
    return 0;
}

void DISSTATS(int p){
    printf("HP: %g\n", players[p].HP);
    printf("Gold: %i\n", players[p].gold);
    printf("Owned Weapons: \n\n");
    for (int i = 0; i < 10; i++){
        if (players[p].owned_weapons[i].id != 0){
            struct Weapon w = players[p].owned_weapons[i];
            printf("%");
        }
    }
}

//main program
int main(){ //Main program loop
    srand(time(NULL));
    int p = 0;
    int opp = 1;
    Clear();
    printf("Welcome to Generic RPG game\n");
    printf("Would you like to view the user's manual? (y/n)\n");
    char manual_choice;
    scanf(" %c", &manual_choice);
    getchar();
    Clear();
    if (manual_choice == 'y') {
        printf("User's Manual:\n");
        printf("There are 5 actions you can take on your turn:\n");
        printf("1. Attack: Choose a weapon in your inventory to attack your opponent, provided that you have one available. Each weapon has different stats.\n");
        printf("2. Defend: Expend a turn to equip a defence to protect yourself from your opponent's attack, provided that you have one available. Each defence has different stats.\n");
        printf("3. Purchase: Buy new weapons or defences to improve your combat abilities. Note: Bought defence will not be auto-equipped.\n");
        printf("4. Rest: Gain 100 gold by resting. You gain a bonus for resting consecutive turns.\n");
        printf("5. Display Stats: View your current stats and inventory. This will not cost you your turn.\n");
        printf("Also note: entering 0 at any menu will return you to the main menu, allowing you to choose a different action.\n");
    }
    else{
        printf("get on with it then\n");
    }
    printf("\nPress enter to continue\n");
    getchar();
    Clear();
    while (players[0].HP > 0 && players[1].HP > 0){
        printf("Player %i's turn\n", p + 1);
        printf("Actions:\n\n1. Attack\n2. Defend\n3. Purchase\n4. Rest\n5. Display Stats\n");
        printf("Which action would you like to take? (Enter the corresponding number)\n");
        int action;
        scanf("%i", &action);
        Clear();
        switch(action){ // a switch case is an easier and cleaner if else chain
            case 1: //Atk case
                bool can_atk = false;
                for (int i = 0; i < 10; i++){
                    if (players[p].owned_weapons[i].id != 0){
                        can_atk = true;
                        break;
                    }
                }
                if (!can_atk) {
                    Clear();
                    printf("You have no weapons to attack with. Please buy one at the shop.\n\n");
                    continue;
                }
                else if (ATK(p, opp) == -1){
                    continue;
                }
                break;
            case 2:
                bool can_def = false;
                for (int i = 0; i < 10; i++){
                    if (players[p].owned_defences[i].id != 0){
                        can_def = true;
                        break;
                    }
                }
                if (!can_def){
                    Clear();
                    printf("You have no defences to defend with idiot, buy some in the shop. \n\n");
                    continue;
                }
                else if (DEF(p, opp) == -1){
                    continue;
                }
                break;
            case 3:
                Clear();
                if (SHOP(p, opp) == -1){
                    continue;
                }
                break;
            case 4:
                Clear();
                if (REST(p) == -1){
                    continue;
                }
                break;
            case 5:
                continue;
            default:
                printf("Invalid action. Please enter a valid menu option.\n");
                continue;
        }
        p = 1 - p; //Switch player
        opp = 1 - opp; //Switch opponent
    }
    
    if (players[0].HP <= 0){
        printf("Player 2 wins!\n");
    }
    else{
        printf("Player 1 wins!\n");
    }
    
    return 0;
}