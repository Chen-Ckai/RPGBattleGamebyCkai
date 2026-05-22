//Recoded balatro, based on my_abstract.md
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


//enum is like #define, but it allows for several definitions under one name, like a struct
//It makes code more readable, and easier to use with switch cases
enum CARDTYPE { 
    COMMON,  //this is 0
    RARE     //this is 1, so on
};
enum TARGET {
    SELF, 
    OPP,
    BOTH
};
enum EFFECTTYPE { 
    HP,
    ATK,
    IRON,
    INTEREST, //Wall Street
    STEAL_HP, 
    STEAL_IRON,
    STEAL_ATK,
    BATTLETURNS,
    SETUPTURNS,
    RESTCOUNT,
    SPECIAL      //Jackpot, Go Fish, etc.
};
enum DRAWBACK{
    ATK,
    IRON,
    HP,
    BATTLETURNS,
    SETUPTURNS,
    NONE
};
enum DEFCONDITION{
    HP,
    PHASES
};

struct CARD{
    int id;
    char name[40];
    enum CARDTYPE type;
    int cost;
    enum TARGET target;
    enum EFFECTTYPE efftype;
    enum DRAWBACK drawback;
    int procchance;
    int addvalue;
    float multvalue;             //For percents and multipliers
}; 

struct DEFCARD{
    int id;
    char name[40];

};

struct Player{
    char name[40];
    float hp;
    float maxhp;
    float atk;
    int iron;              // ALL CURRENCY MENTIONS ARE IN IRON SUPPLEMENTS
    int consecutive_rest;
    struct CARD carinv[10];
    struct DEFCARD definv[10];
    int invspace;
};

struct Player player[2] = {
    //Name       HP    MaxHP  ATK  IRON  RESTCOUNT  INVENTORY & SPACE
    {"Player 1", 1000, 1000,  5,   10,   0,         {0},        10},
    {"Player 2", 1000, 1000,  5,   10,   0,         {0},        10}
};

struct CARD cards[] = {
//   ID    Name             Type      Cost   Target   EffectType  Drawback  Chance   AddValue  MultValue
    {0,   "Banana Farm",    COMMON,   5,     SELF,    IRON,       NONE,       100,     20,       1.1},
};

struct CARD defcards[] = {
//   ID    Name             Type      Cost   Target   EffectType   Chance   Effectvalue    EffectDegree
    
};

int Ran_100(){ //Random generation for all chance mechanics
    return rand() % 100 + 1;
}

int Ran_Com(){
    //return rand() % # of common cards             return a common card id for the shop
}

int Ran_Rar(){
    //return rand() % # of rare cards               return a rare card id for the shop
}

int Ran_Def(){
    //return rand() % # of defence cards            return a def card id for the shop
}

void clear(){ //Clear Terminal, purely cosmetic
    printf("\e[1;1H\e[2J");
}

int ATK(int p, int opp){
    getchar(); //Buffer from enter from the menu
    printf("Attacking enemy. Are you really sure you'd like to do this? (y/n)\n");
    char choice;
    scanf("%c", &choice);
    if (choice == 'n'){
        clear();
        printf("ok\n\n");
        return -1;
    }
    else if (choice == 'y'){
        printf("ok\n\n");
        float dmg = player[p].atk;
        player[opp].hp -= dmg;
        printf("Dealt %g damage! Your enemy now has %g hp.", dmg, player[opp].hp);
    }
    else{
        clear();
        printf("Invalid input dummy, im forcing you back to the menu \n\n");
        return -1;
    }
    return 0;
}

int DEF(int p){
    printf("This action will allow you to equip a defence card in your inventory. ");
}

int USECARD(int useid, int p, int opp){
    int targets[2];
    int targetcount = 1;
    switch (cards[useid].target){
        case SELF:
            targets[0] = p;
            break;
        case OPP:
            targets[1] = opp;
            break;
        case BOTH:
            targets[0] = p, targets[1] = opp;
            break;
    }
    for (int i = 0; i < targetcount; i++){
        switch (cards[useid].efftype){
            case HP:
                player[targets[i]].hp += cards[useid].addvalue;
                player[targets[i]].hp *= cards[useid].multvalue;
                break;
            case SPECIAL:
                if (useid == 32){

                }
        }
        switch (cards[useid].drawback){
            case HP:
                player[targets[i]].hp -= cards[useid].addvalue;
        }
    }
    return 0;
}

int SHOP(int p){
    printf("Welcome to the SHOP!\n\n");
    printf("How many iron supplements would you like to commit? ");
    return 0;
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