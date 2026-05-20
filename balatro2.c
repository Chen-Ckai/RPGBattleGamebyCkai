//Recoded balatro, based on my_abstract.md
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


//enum is like #define, but it allows for several definitions under one name, like a struct
//It makes code more readable, and easier to use with switch cases
enum CARDTYPE { 
    COMMON,  //this is 1
    DEFENCE, //this is 2
    RARE     //this is 3, so on
};
enum TARGET {
    SELF,
    OPP,
    BOTH
};
enum EFFECTTYPE { 
    HP,
    ATK,
    GOLD,
    CHANCE,
};
enum EFFECTDEGREE{
    ADD,
    MULT,
    PERCENT
};

struct CARD{
    int id;
    char name[40];
    enum CARDTYPE type;
    int cost;
    enum TARGET target;
    enum EFFECTTYPE efftype;
    int procchance;
    int effvalue;
    enum EFFECTDEGREE effdegree;
}; 

struct Player{
    char name[40];
    float hp;
    float atk;
    int gold;
    int consecutive_rest;
    struct CARD inventory[10];
    int invfill;
};

struct Player player[2] = {
    {"Player 1", 1000, 5, 10, 0, {0}, 0},
    {"Player 2", 1000, 5, 10, 0, {0}, 0},
};

struct CARD card_database[] = {
//   ID    Name             Type      Cost   Target   EffectType   Chance   Effectvalue    EffectDegree
    {1,   "Banana Farm",    COMMON,   5,     SELF,    GOLD,        100,     20,            PERCENT},
};


int Ran_100(){ //Random generation for all chance mechanics
    return rand() % 100 + 1;
}

void Clear(){ //Clear Terminal, purely cosmetic
    printf("\e[1;1H\e[2J");
}

bool Own_Item(int p, int num, char i_type){
    bool do_u_have_it = false;
    return do_u_have_it;
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
    
    
    return 0;
}