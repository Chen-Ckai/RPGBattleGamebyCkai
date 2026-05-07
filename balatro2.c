#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

//declaring 3 strucutres
struct Player{
    char name[40];
    float HP;
    int gold;
    int consecutive_rest;
    bool owned_weapons[10];
    bool owned_defences[10];
    int equipped_defence;
    float defence_hp;
}; //8 attributes

struct Weapon{
    char type[40];
    char name[40];
    float damage;
    int cooldown;
    float cost;
    int hitchance;
}; //6 attributes

struct Defence{
    char type[40];
    char name[40];
    float hitpoints;
    float DamageDeduction;
    float cost;
}; //5 attributes

struct Player players[2] = { //Each player requires the following attributes: name, HP, gold, consecutive rest turns, owned weapons, owned defences, equipped defence index, and defence HP.
        {"Player 1", 1000, 500, 0, {false}, {false}, -1, 0},
        {"Player 2", 1000, 500, 0, {false}, {false}, -1, 0}
};

struct Weapon WColl[10] = { //Mentions of WColl in the code refer to this array of weapons
    {"Energy", "Tachyon Lance", 3000, 20, 1500, 75}, 
    {"Energy", "Plasma Lanucher", 400, 2, 700, 100},
    {"Energy", "Zip Zap", 50, 1, 500, 100},
    {"Kinetic", "Gauss Canon", 1000, 5, 1500, 50}, 
    {"Kinetic", "Cabine Gun", 300, 1, 1000, 100},
    {"Kinetic", "Hand Gun", 100, 2, 500, 100},
    {"Melee", "Dark Saber", 2000, 5, 1000, 90},
    {"Melee", "Plasma Knife", 300, 2, 500, 100},
    {"Melee", "Reaper's Scythe", 1500, 5, 800, 95},
    {"Mystic", "Aetherophasic Engine", 2000, 1, 800, 85}
};

struct Defence DefColl[10] = { //All mentions of DefColl in the code refer to this array of defences
    {"Energy", "Dark Matter Shield", 4000, 1, 1100},
    {"Energy", "Energy Deflector", 2000, 1, 800},
    {"Energy", "Basic Sheild", 700, 1, 500},
    {"Kinetic", "Gravitational Refractor", 1000, 0.5, 1100},
    {"Kinetic", "Hyper Array", 1000, 0.8, 800},
    {"Kinetic", "Pulsing Array", 500, 0.9, 500},
    {"Melee", "Zero-point Armor", 300, 0.3, 1000},
    {"Melee", "Power Armor", 300, 0.4, 800},
    {"Melee", "Space Armor", 200, 0.6, 500},
    {"Mystic", "Dimensional Lens", 500, 0.2, 800}  
};

int Ran_100(){ //Random generation for chance based items
    return rand() % 100 + 1;
}

void clear(){ //Clear Terminal, purely cosmetic
    printf("\e[1;1H\e[2J");
}

int ATK(int p){ //Attack enemy function, takes in player index as parameter
    printf("Attacking enemy. They currently have %.0f HP.\n", players[1 - p].HP);
    printf("Choose a weapon:\n");
    for(int i = 0; i < 10; i++){
        if(players[p].owned_weapons[i]){
            printf("Weapon ID: %i. %s (Damage: %.0f, Cooldown: %i, Hit Chance: %i%%)\n", i + 1, WColl[i].name, WColl[i].damage, WColl[i].cooldown, WColl[i].hitchance);
        }
    }
    int choice;
    scanf("%i", &choice);
    choice -= 1;
    if (choice > 9 || choice < 0 || !players[p].owned_weapons[choice]) { //Invalid input
        printf("Invalid weapon choice. Returning to menu.\n");
        return -1;
    } else {
        bool has_def = false;
        if (Ran_100() <= WColl[choice].hitchance) {
            float dmg = WColl[choice].damage;
            if (players[1 - p].equipped_defence != -1) { //Check if enemy has def equip
                dmg *= (1 - DefColl[players[1 - p].equipped_defence].DamageDeduction);
                float def_dmg = WColl[choice].damage - dmg; //Calculate damage to defence
                players[1 - p].defence_hp -= def_dmg; //Apply damage to equipped defence
                has_def = true;
            }
            players[1 - p].HP -= dmg;
            printf("Hit! Dealt %.0g damage to opponent.\n", dmg);
        } else {
            printf("Missed!\n");
        }
        if (has_def){
            if (players[1 - p].defence_hp <= 0){ //Check if defence is broken
                printf("Your enemy's %s was broken!\n", DefColl[players[1 - p].equipped_defence].name);
                players[1 - p].equipped_defence = -1;
                players[1 - p].defence_hp = 0;
            }
            else{
                printf("Your enemy's %s has %.0g HP left.\n", DefColl[players[1 - p].equipped_defence].name, players[1 - p].defence_hp);
            }
        }
        printf("Your enemy now has %.0g HP.\n", players[1 - p].HP);
    }
}

int DEF(int p){ //Equip defence function, takes in player index as parameter
    bool has_def = false;
    for (int i = 0; i < 10; i++){
        if (players[p].owned_defences[i]) {
            has_def = true;
            printf("Defence ID: %i. %s (Remaining HP: %.0g, Damage Deduction: %.0g%%)\n", i + 1, DefColl[i].name, players[p].defence_hp, DefColl[i].DamageDeduction * 100);
        }
    }
    if (!has_def) {
        printf("You have no defences to equip. Please enter the shop to purchase one.\n");
        return -1;
    } else {
        printf("Please enter the ID of the defence you want to equip:\n");
        int d_choice;
        scanf("%i", &d_choice);
        d_choice = d_choice - 1; //Adjust for 0 indexing
        if (d_choice > 9 || d_choice < 0 || !players[p].owned_defences[d_choice]) { //Invalid input
            printf("Invalid defence choice. Returning to menu.\n");
            return -1;
        } else{
            players[p].equipped_defence = d_choice;
            players[p].defence_hp = DefColl[d_choice].hitpoints;
            printf("Equipped %s!\n", DefColl[d_choice].name);
        }
    }
}

int SHOP(int p){ //Shop function, takes in player index as parameter
    printf("What kind of item would you like to buy? (1. Weapon, 2. Defence)\n");
    int item_type;
    scanf("%i", &item_type);
    printf("You have %i gold.\n", players[p].gold);
    if (item_type != 1 && item_type != 2) {
        printf("Invalid item type. Returning to menu.\n");
        return -1;
    }
    switch(item_type){
        case 1:
            printf("Available weapons:\n");
            for(int i = 0; i < 10; i++){
                if(!players[p].owned_weapons[i]){ //Only show weapons that the player doesn't already own
                    printf("Weapon ID: %i. %s (Damage: %.0g, Cooldown: %i, Hit Chance: %i%%, Cost: %.0g)\n", i + 1, WColl[i].name, WColl[i].damage, WColl[i].cooldown, WColl[i].hitchance, WColl[i].cost);
                }
            }
            printf("Please enter the ID of the weapon you wish to purchase:\n");
            int w_choice;
            scanf("%i", &w_choice);
            if (w_choice > 10 || w_choice < 1 || players[p].owned_weapons[w_choice - 1]) { //Invalid input
                printf("Invalid weapon choice. Returning to menu.\n");
                return -1;
            } else if (players[p].gold < WColl[w_choice - 1].cost) { //Not enough gold
                printf("Not enough gold. Returning to menu.\n");
                return -1;
            } else {
                players[p].gold -= WColl[w_choice - 1].cost;
                players[p].owned_weapons[w_choice - 1] = true;
                printf("Purchased %s!\n", WColl[w_choice - 1].name);
            }
            break;
        case 2:
            printf("Available defences:\n");
            for(int i = 0; i < 10; i++){
                if(!players[p].owned_defences[i]){ //Only show defences that the player doesn't already own
                    printf("Defence ID: %i. %s (Hitpoints: %.0g, Damage Deduction: %.0g%%, Cost: %.0g)\n", i + 1, DefColl[i].name, DefColl[i].hitpoints, DefColl[i].DamageDeduction * 100, DefColl[i].cost);
                }
            }
            printf("Please enter the ID of the defence you wish to purchase:\n");
            int d_choice;
            scanf("%i", &d_choice);
            if (d_choice > 10 || d_choice < 1 || players[p].owned_defences[d_choice - 1]) {
                printf("Invalid choice. Returning to menu.\n");
                return -1;
            } else if (players[p].gold < DefColl[d_choice - 1].cost) { //Not enough gold
                printf("Not enough gold. Returning to menu.\n");
                return -1;
            }
            else { 
                players[p].owned_defences[d_choice - 1] = true;
                //printf("Added %s to your inventory. \n", players[p].owned_defences[d_choice - 1].name);
            }
            break;
        default:
            printf("Invalid item type. Returning to menu.\n");
            return -1;
    }
}
int REST(int p){ //Rest function, takes in player index as parameter
    printf("Would you like to rest? (y/n)\n");
    char rest_choice;
    scanf(" %c", &rest_choice);
    if (rest_choice == 'y') {
        int add_g = 100 + 50 * players[p].consecutive_rest;
        players[p].gold += add_g;
        printf("You rested and gained %.0i gold!\n", add_g);
        players[p].consecutive_rest++;
        printf("Consecutive rest count is at %i.\n", players[p].consecutive_rest);
    } else {
        printf("Rest cancelled. Returning to menu.\n");
        return -1;
    }
}

void DISSTATS(int p){ //Display stats function, takes in player index as parameter
    printf("Player %i's stats:\n", p + 1);
    printf("HP: %.0g\n", players[p].HP);
    printf("Gold: %i\n", players[p].gold);
    printf("Owned Weapons:\n\n");
    for (int i = 0; i < 10; i++){
        if (players[p].owned_weapons[i]){
            printf("%s\n", WColl[i].name);
        }
    }
    printf("Owned Defences:\n\n");
    for (int i = 0; i < 10; i++){
        if (players[p].owned_defences[i]){
            printf("%s\n", DefColl[i].name);
        }
    }
    printf("\nPress enter to continue\n");
    getchar();
    clear();
}

//main program
int main(){ //Main program loop
    srand(time(NULL));
    int p = 0;
    int opp = 1;
    printf("Welcome to Balatro 2\n");
    printf("Would you like to view the user's manual? (y/n)\n");
    char manual_choice;
    scanf(" %c", &manual_choice);
    getchar(); //Consume newline character left in buffer
    if (manual_choice == 'y') {
        printf("User's Manual:\n");
        printf("There are 5 actions you can take on your turn:\n");
        printf("1. Attack: Choose a weapon to attack your opponent, provided that you have one available. Each weapon has different stats.\n");
        printf("2. Defend: Expend a turn to equip a defence to protect yourself from your opponent's attack, provided that you have one available. Each defence has different stats.\n");
        printf("3. Purchase: Buy new weapons or defences to improve your combat abilities. Note: Bought defence will not be auto-equipped.\n");
        printf("4. Rest: Regain some gold by resting. You gain a bonus for resting consecutive turns.\n");
        printf("5. Display Stats: View your current stats and inventory.\n");
        printf("Also note: entering -1 at any menu will return you to the main menu, allowing you to choose a different action.\n");
    }
    else{
        printf("get on with it then\n");
    }
    printf("\nPress enter to continue\n");
    getchar();
    clear();
    while(players[0].HP > 0 && players[1].HP > 0){
        getchar();
        printf("Player %i's turn\n", p + 1);
        DISSTATS(p);
        printf("Actions:\n\n1. Attack\n2. Defend\n3. Purchase\n4. Rest\n5. Display Stats\n");
        int action;
        scanf("%i", &action);
        clear();
        switch(action){ // a switch case is an easier and cleaner if else chain
            case 1:
                players[p].consecutive_rest = 0; //Reset consecutive rest counter when not resting
                if (ATK(p) == -1){
                    continue;
                }
                players[p].gold += 50;
                printf("Passively gained 50 gold. \n");
                break;
            case 2:
                players[p].consecutive_rest = 0; //Reset consecutive rest counter when not resting
                if (DEF(p) == -1){
                    continue;
                }
                players[p].gold += 50;
                printf("Passively gained 50 gold. \n");
                break;
            case 3:
                players[p].consecutive_rest = 0; //Reset consecutive rest counter when not resting
                if (SHOP(p) == -1){
                    continue;
                }
                players[p].gold += 50;
                printf("Passively gained 50 gold. \n");
                break;
            case 4:
                if (REST(p) == -1){
                    continue;
                }
                break;
            case 5:
                DISSTATS(p);
                continue;
            default:
                printf("Invalid action. Returning to menu.\n");
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


