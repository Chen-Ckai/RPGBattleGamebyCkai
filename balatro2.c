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
    EFF_HP,
    EFF_ATK,
    EFF_IRON,
    EFF_DEF,
    EFF_INTEREST,   // Accounting Masterclass
    EFF_STEAL_HP, 
    EFF_STEAL_IRON,
    EFF_STEAL_ATK,
    EFF_BATTLETURNS,
    EFF_SETUPTURNS,
    EFF_RESTCOUNT,
    EFF_DEBUFF,
    EFF_SPECIAL      //Jackpot, Go Fish, etc.
};
enum DRAWBACK{
    DB_HP,
    DB_ATK,
    DB_IRON,
    DB_BATTLETURNS,
    DB_SETUPTURNS,
    NONE
};
enum DEFCONDITION{
    DEF_HP,
    DEF_PHASES
};
enum MODE{
    SHOP,
    INV,
    DIS
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
    int db_addvalue;
    float db_multvalue;
}; 

struct DEFCARD{
    int id;
    char name[40];

};

struct CARDDESC{
    int id;
    char desc[100];
};

struct Player{
    char name[40];
    float hp;
    float maxhp;
    float atk;
    int atkcount;
    int iron;              // ALL CURRENCY MENTIONS ARE IN IRON SUPPLEMENTS
    int consecutive_rest;
    struct CARD cardinv[10];
    struct DEFCARD definv[10];
    int invspace;
};

struct Player player[2] = {
    //Name       HP    MaxHP  ATK  Atk Count IRON  RESTCOUNT  INVENTORY & SPACE
    {"Player 1", 1000, 1000,  5,   1,        10,   0,         {0},        10},
    {"Player 2", 1000, 1000,  5,   1,        10,   0,         {0},        10}
};

struct CARD cards[] = {
//   ID    Name                Type      Cost   Target   EffectType      Drawback    Chance   Added Values       Minused Values
    {1,   "Sharpening Stone",  COMMON,   5,     SELF,    EFF_ATK,        NONE,       100,     20,       1.0,     0,        1.0},
    {2,   "Fabric Softener",   COMMON,   10,    OPP,     EFF_DEF,        NONE,       100,     25,       1.0,     0,        1.0},
    {3,   "Banana Farm",       COMMON,   7,     SELF,    EFF_IRON,       NONE,       100,     10,       1.2,     0,        1.0},
    {4,   "Baguette Farm",     COMMON,   7,     SELF,    EFF_ATK,        NONE,       100,     10,       1.2,     0,        1.0},
    {5,   "Cheesecake Farm",   COMMON,   7,     SELF,    EFF_HP,         NONE,       100,     10,       1.2,     0,        1.0},
    {6,   "Insomnia",          COMMON,   5,     OPP,     EFF_SETUPTURNS, NONE,       50,      1,        1.0,     0,        1.0},
};
int ccamt = 6, rramt = 0;
int tcamt = 6;

struct DEFCARD defcards[] = {
//   ID    Name             Type      Cost   Target   EffectType   Chance   Effectvalue    EffectDegree
    
};
int dcamt = 0;

struct CARDDESC carddesc[] = {
    {1, "Increases the damage of all hits by 5"},
    {2, "Decreases the enemy's equipped defence card's HP by 25"},
    {3, "Gain 10%% of your current iron + 10"},
    {4, "Gain 10%% of your current attack stat + 10"},
    {5, "Gain 10%% of your current hp + 10"},
    {6, "50%% chance to make the enemy lose one setup turn"},
};

int Ran_100(){ //Random generation for all chance mechanics
    return rand() % 100 + 1;
}

int Ran_Com(int p){
    return rand() % ccamt;
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
void FULLCARDPRINT(int p, enum MODE mode, struct CARD c[], int ccount){
    struct CARD *pcard = c;
    if (mode == SHOP){
        printf("%-6s%-40s%-100s", "ID", "Name", "Description");
        if (mode == SHOP){
            printf("%-20s", "Iron Cost");
        }
        printf("\n");
        for (int i = 0; i < ccount; i++){
            printf("%-6i%-40s%-100s", pcard[i].id, pcard[i].name, carddesc[pcard[i].id - 1].desc);
            if (mode == SHOP){
                printf("%-20i", pcard[i].cost);
            }
            printf("\n");
        }
    }
    else{

    }
    return;
}

int CHOICECHECK(int p, bool owned, enum MODE mode, struct CARD c[], int ccount){
    printf("Please enter the ID of the card you'd like to select:\n");
    int choice;
    scanf("%i", &choice);
    bool douownthis = false;
    bool wasthisoffered = false;
    for (int i = 0; i < ccount; i++){
        if (c[i].id == choice && mode != SHOP){
            douownthis = true;
            break;
        }
        if (c[i].id != choice && mode == SHOP){
            wasthisoffered = false;
            break;
        }
    }
    if (choice == -1){
        printf("Exit code entered. \n\n");
        return -1;
    }
    else if (douownthis){
        printf("You don't own this. Please pick a card that you actually own.\n\n");
        return -1;
    }
    else if (wasthisoffered){
        printf("This item isn't in the collection shown idiot \n\n");
        return -1;
    }
    else if (choice < 1 || choice > 32){
        printf("Invalid choice. Pick a valid card ID please. \n\n");
        return -1;
    }
    else if (mode == SHOP && player[p].iron < cards[choice].cost){
        printf("Too broke for this lol, pick a card that you can afford \n\n");
        return -1;
    }
    else{
        return choice;
    }
}

int ATKf(int p, int opp){
    printf("Attacking enemy. Are you really sure you'd like to do this? (y/n)\n");
    char choice;
    scanf(" %c", &choice);
    if (choice == 'n'){
        clear();
        printf("ok\n\n");
        return -1;
    }
    else if (choice == 'y'){
        clear();
        printf("ok\n\n");
        float dmg = player[p].atk;
        player[opp].hp -= dmg;
        printf("Dealt %g damage! Your enemy now has %g hp. \n\n", dmg, player[opp].hp);
    }
    else{
        clear();
        printf("Invalid input dummy, im forcing you back to the menu \n\n");
        return -1;
    }
    return 0;
}

int DEFf(int p){
    printf("This action will allow you to equip a defence card in your inventory. ");
}

int USECARDf(int useid, int p, int opp){
    int targets[2];
    int targetcount = 0;
    switch (cards[useid].target){
        case SELF:
            targets[0] = p;
            targetcount = 1;
            break;
        case OPP:
            targets[0] = opp;
            targetcount = 1;
            break;
        case BOTH:
            targets[0] = p, targets[1] = opp;
            targetcount = 2;
            break;
    }
    for (int i = 0; i < targetcount; i++){
        switch (cards[useid].efftype){
            case EFF_HP:
                player[targets[i]].hp += cards[useid].addvalue;
                player[targets[i]].hp *= cards[useid].multvalue;
                break;
            case EFF_SPECIAL:
                switch (useid){

                }
        }
        switch (cards[useid].drawback){
            case DB_HP:
                player[targets[i]].hp -= cards[useid].db_addvalue;
        }
    }
    return 0;
}

int SHOPf(int p){
    printf("=---------Welcome to the SHOP!----------=\n\n");
    printf("Here are your options: \n");
    int rcardc = 0, ccardc = 100, rang;
    int ccount = 6;
    struct CARD tempc[ccount];
    int i = 0;
    while (i < ccount) {
        if (Ran_100() <= ccardc) {
            int rang;
            bool is_duplicate = true;
            while (is_duplicate){
                rang = Ran_Com(p);
                is_duplicate = false;
                for (int a = 0; a < i; a++) {
                    if (tempc[a].id == cards[rang].id) {
                        is_duplicate = true; // Busted! It's a duplicate.
                        break; 
                    }
                }
            } 
            tempc[i] = cards[rang];
            i++;
        }
    }
    int choice = 0;
    while (true){
        FULLCARDPRINT(p, SHOP, tempc, ccount);
        choice = CHOICECHECK(p, false, SHOP, tempc, ccount);
        switch (choice){
            case -1:
                return -1;
            default: 
                for (int i = 0; i < 10; i++){
                    if (player[p].cardinv[i].id == 0){
                        choice--;
                        player[p].cardinv[i] = cards[choice];
                        player[p].iron -= cards[choice].cost;
                        printf("Bought %s!\n\n", cards[choice].name);
                        return 0;
                    }
                }
        }
    }
    return 0;
}

//main program
int main(){ //Main program loop
    srand(time(NULL));
    clear();
    printf("Welcome to Generic RPG game\n");
    printf("Would you like to view the user's manual? (y/n)\n");
    char manual_choice;
    scanf(" %c", &manual_choice);
    clear();
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
    clear();
    int p = 0;
    int opp = 1;
    while (player[p].hp > 0 || player[opp].hp > 0){
        printf("=-------Main Menu--------=\n\n");
        printf("Player %i, please choose one of the options below\n\n", p + 1);
        printf("1. Attack your enemy\n");
        printf("2. Equip a defence card\n");
        printf("3. Use a card in your inventory\n");
        printf("4. Discard a card in your inventory\n");
        printf("5. Enter the shop\n");
        printf("6. Display your stats\n");
        printf("7. Forfeit\n\n");
        printf("Which action would you like to take? (Enter the corresponding number)\n");
        int action;
        scanf("%i", &action);
        clear();
        switch(action){
            case 1:
                if (ATKf(p, opp) == -1){
                    continue;
                }
                break;
            case 2:
                if (DEFf(p) == -1){
                    continue;
                }
                break;
            case 3:
                printf("Use a card in your inventory\n");
                break;
            case 4:
                printf("Discard a card in your inventory\n");
                break;
            case 5:
                if (SHOPf(p) == -1){
                    continue;
                }
                break;
            case 6:
                printf("Display your stats\n");
                break;
            case 7:
                printf("Forfeit\n");
                player[p].hp = 0;
                break;
            default:
                printf("Invalid input dummy, im forcing you back to the menu \n\n");
        }
        //Switching players
        p = 1 - p;
        opp = 1 - opp;
    }
    
    
    return 0;
}