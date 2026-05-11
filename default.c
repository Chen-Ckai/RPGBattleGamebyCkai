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
    float cost;
    int hitchance;
}; //6 attributes

struct Defence{
    int id;
    char name[40];
    float hitpoints;
    float DamageDeduction;
    float cost;
}; //5 attributes

struct Player{
    char name[40];
    float HP;
    int gold;
    int consecutive_rest;
    struct Weapon owned_weapons[10];
    struct Defence owned_defences[10];
    int owned_weapon_count;
    int owned_defence_count;
    int equipped_defence;
    float defence_hp;
};

struct Player players[2] = {
        {"Player 1", 1000, 500, 0, {{0}}, {{0}}, 0, 0, -1, 0},
        {"Player 2", 1000, 500, 0, {{0}}, {{0}}, 0, 0, -1, 0}
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

int ATK(int p, int opp){
    Clear();
    printf("Attacking enemy. They currently have %.0f HP.\n", players[opp].HP);
    printf("Choose a weapon:\n\n");
    printf("Owned weapons:\n");
    printf("%-10s%-20s%-20s%-20s%-20s\n", "ID", "Name", "Damage", "Cooldown", "Hit Chance");
    for (int i = 0; i < players[p].owned_weapon_count; i++) {
        struct Weapon w = players[p].owned_weapons[i];
        printf("%-10i%-20s%-20.0f%-20i%-20i%%\n", w.id, w.name, w.damage, w.cooldown, w.hitchance);
    }
    int choice;
    scanf("%i", &choice);
    if (choice == -1){
        Clear();
        printf("Exit code entered. Returning to menu.\n");
        return -1;
    }
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
        printf("Also note: entering -1 at any menu will return you to the main menu, allowing you to choose a different action.\n");
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
                if (players[p].owned_weapon_count == 0){
                    printf("You have no weapons to attack with. Please enter the shop to purchase one.\n");
                    break;
                }
                else if (ATK(p, opp) == -1){
                    continue;
                }
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
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