//Recoded balatro, based on my_abstract.md
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


//enum is like #define, but it allows for several definitions under one name, like a struct
//It makes code more readable, and easier to use with switch cases in the USECARDf function
enum CARDTYPE { 
    COMMON,  //this is 0
    RARE,    //this is 1, so on
    SINCARD, 
    DEF
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
    EFF_DEBUFF,
    EFF_INTEREST,   // Accounting Masterclass
    EFF_STEAL_HP, 
    EFF_STEAL_IRON,
    EFF_STEAL_ATK,
    EFF_SET_HP,
    EFF_SET_ATK,
    EFF_BATTLETURN,
    EFF_SETUPTURN,
    EFF_PERMBTURNS,
    EFF_PERMSTURNS,
    EFF_SPECIAL,      //Jackpot, Go Fish, etc.
    DEF_HP,
    DEF_HITS,
    DEF_SPECIAL
};
enum DRAWBACK{
    DB_HP,
    DB_ATK,
    DB_IRON,
    DB_BATTLETURNS,
    DB_SETUPTURNS,
    DB_PBATTLETURNS,
    DB_NONE
};
enum MODE{
    SHOP,
    INV,
    DIS
};
enum PHASE{
    BATTLE,
    SETUP
};
enum SUCCESSPARITY{
    SUCCESS,
    INVALID,
    BACKED,
    REROLL
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
    float multvalue; 
    int db_addvalue;
    float db_multvalue;
    //For defence cards, addvalue is the hits left, multvalue is the hp, <------------------------------------
    //db is the dmg reduction (- dmg is addvalue, %dmg is multvalue)<---------------------- IMPORTANT READ IT ME PLEASE
};

struct CARDDESC{     //Each card's effect and type
    int id;
    char type[15];
    char desc[125];
};

struct Player{
    char name[30];
    float hp;
    float atk;
    int iron;              // ALL CURRENCY MENTIONS ARE IN IRON SUPPLEMENTS
    int ironinterest;
    int interestcap;
    int consecutive_rest;
    int turns[2]; //[0] is battle, [1] is setup
    enum PHASE current_phase; 
    int turns_def[2];
    struct CARD equipdef;
    struct CARD cardinv[10];
    int invspace;
};

struct Player player[2] = {
    //Name       HP     ATK  IRON  Interest Interestcap Restcount  Battle Setup  Phase   Default turn amt   EquippedDefs INVENTORY   Space filled
    {"Player 1", 1000,  5,   10,   1,       30,         0,         {2,    2},    SETUP,  {2, 2},            {0},         {0},        0},
    {"Player 2", 1000,  5,   14,   1,       30,         0,         {2,    2},    SETUP,  {2, 2},            {0},         {0},        0}
};
struct CARD cards[] = {  //COMPLETE DATABASE OF EVERY CARD IN THE GAME
//A few notes:
//any negative values indicate an effect requiring stat values outside of the card's effect (ex. set atk to iron)
//EFF_SPECIAL are any cards that can't fit into the main use card system
//   ID    Name                         Type      Cost   Target   EffectType      Drawback          Chance   Good/benefit Values  Bad/drawback Values
    {1,   "Sharpening Stone",           COMMON,   3,     SELF,    EFF_ATK,        DB_NONE,            100,     5,        1.0,     0,        1.0},
    {2,   "Fabric Softener",            COMMON,   10,    OPP,     EFF_SPECIAL,    DB_NONE,            100,     0,        0,       1,        25.0},
    {3,   "Banana Farm",                COMMON,   10,     SELF,    EFF_IRON,       DB_NONE,            100,     10,       1.1,     0,        1.0},
    {4,   "Baguette Farm",              COMMON,   10,     SELF,    EFF_ATK,        DB_NONE,            100,     10,       1.1,     0,        1.0},
    {5,   "Cheesecake Farm",            COMMON,   10,     SELF,    EFF_HP,         DB_NONE,            100,     10,       0.05,    0,        1.0},
    {6,   "No more honk shoo",          COMMON,   12,    OPP,     EFF_SETUPTURN,  DB_NONE,            50,      0,        1.0,     1,        1.0},
    {7,   "Cat sitting on your lap",    COMMON,   14,    OPP,     EFF_BATTLETURN, DB_NONE,            50,      0,        1.0,     1,        1.0},
    {8,   "Double Edged Sword",         COMMON,   15,    SELF,    EFF_ATK,        DB_HP,              100,     15,       1.0,     15,       1.0},
    {9,   "Accounting Masterclass",     COMMON,   15,    SELF,    EFF_INTEREST,   DB_IRON,            100,     1,        1.0,     0,        0.9},
    {10,  "Wheel of Fortune",           COMMON,   5,     SELF,    EFF_ATK,        DB_NONE,            25,      0,        1.25,    0,        1.0},
    {11,  "Scalper",                    COMMON,   42,    SELF,    EFF_SPECIAL,    DB_NONE,            100,     0,        1.0,     0,        1.0},
    {12,  "Sin of Greed",               COMMON,   50,    SELF,    EFF_SPECIAL,    DB_NONE,            100,     0,        1.0,     1,        1.0},
    {13,  "Yummy yummy in my tummy",    COMMON,   26,    SELF,    EFF_STEAL_HP,   DB_NONE,            100,     25,       0.0,     0,        1.0},
    {14,  "Non-consentual Robbery",     COMMON,   15,    SELF,    EFF_STEAL_IRON, DB_NONE,            100,     20,       0.0,     0,        1.0},
    {15,  "Limiter off",                COMMON,   35,    SELF,    EFF_SPECIAL,    DB_NONE,            100,     5,        1.0,     0,        1.0},
    {16,  "Prescription",               COMMON,   8,     SELF,    EFF_IRON,       DB_NONE,            100,    -1,        1.0,     0,        1.0},
    {17,  "Burn Out",                   COMMON,   19,    SELF,    EFF_IRON,       DB_SETUPTURNS,      100,     0,        2.0,     1,        1.0},
    {18,  "Investment",                 COMMON,   10,    SELF,    EFF_INTEREST,   DB_PBATTLETURNS,    100,     2,        1.0,     1,        1.0},
    {19,  "Leech",                      COMMON,   30,    SELF,    EFF_STEAL_ATK,  DB_NONE,            100,     0,        0.2,     0,        1.0}, 
    {20,  "Bad Day",                    COMMON,   25,    SELF,    EFF_ATK,        DB_BATTLETURNS,     100,     0,        3.0,     1,        1.0},
    {21,  "I have no enemies",          COMMON,   18,    SELF,    EFF_SETUPTURN,  DB_ATK,             100,     1,        1.0,     0,        0.75},
    //Defence cards
    {22,  "Boiling Pot",                DEF,      15,    SELF,    DEF_HITS,       DB_NONE,            100,     4,        0.0,     0,        0.8},
    {23,  "Frying Pan",                 DEF,      15,    SELF,    DEF_HP,         DB_NONE,            100,     0,       -1.0,     20,       0.0},
    {24,  "Strong Stance",              DEF,      75,    SELF,    DEF_HP,         DB_NONE,            100,     0,       -1.0,     0,        0.5},
    {25,  "Barrier",                    DEF,      150,   SELF,    DEF_HITS,       DB_NONE,            100,     2,        0.0,     0,        0.0},
    //All defence cards past this point require special cases
    {26,  "Last Stand",                 DEF,      100,   SELF,    DEF_HITS,       DB_NONE,            100,     10,      -1.0,     0,        0.0},
    {27,  "Mirror Force",               DEF,      47,    SELF,    DEF_HITS,       DB_NONE,            100,     2,        0.0,     0,        0.5},
    {28,  "No Pain No Gain",            DEF,      86,    SELF,    DEF_HP,         DB_NONE,            100,     10,      -1.0,     0,        1.0},
    {29,  "Gluttony",                   DEF,      215,   SELF,    DEF_HITS,       DB_NONE,            100,     3,        1.0,     0,        0.0},
    {30,  "Trap card",                  DEF,      135,   SELF,    DEF_HITS,       DB_NONE,            100,     1,        1.0,     0,        0.0},
    //Rare Cards 
    {31,  "Oops, all 57 leaf clovers!", RARE,     157,   SELF,    EFF_SPECIAL,    DB_NONE,            100,     1,        1.0,     0,        1.0},
    {32,  "Irony",                      RARE,     98,    SELF,    EFF_ATK,        DB_NONE,            100,    -1,        1.0,     0,        1.0},
    {33,  "Tomato Tomato",              RARE,     50,    SELF,    EFF_SET_ATK,    DB_NONE,            100,    -1,        1.0,     0,        1.0},
    {34,  "Hail Mary",                  RARE,     30,    OPP,     EFF_SET_HP,     DB_NONE,            1,      -2000,     1.0,     0,        1.0},
    {35,  "Risky Business",             RARE,     93,    BOTH,    EFF_ATK,        DB_NONE,            100,     0,        5.0,     0,        5.0},
    {36,  "JACKPOT!!!!",                RARE,     77,    SELF,    EFF_SPECIAL,    DB_NONE,            7,       0,        1.0,     0,        1.0},
    {37,  "Heavenly Restriction",       RARE,     10,    SELF,    EFF_SPECIAL,    DB_NONE,            100,     0,        10.0,    0,        1.0},
    {38,  "Mutual Assured Destruction", RARE,     235,   SELF,    EFF_ATK,        DB_HP,              100,     500,      1.0,     0,        0.5},
    {39,  "Go Fish",                    RARE,     125,   OPP,     EFF_SPECIAL,    DB_NONE,            100,     0,        1.0,     0,        1.0},
    {40,  "Trolley Problem",            RARE,     55,    SELF,    EFF_SPECIAL,    DB_NONE,            100,     0,        2.0,     0,        0.5},
    {41,  "Pay to Win",                 RARE,     50,    SELF,    EFF_ATK,        DB_IRON,            100,    -1,        1.0,    -1,        1.0},
    {42,  "Pot of Greed",               RARE,     200,   SELF,    EFF_SPECIAL,    DB_NONE,            100,     0,        1.0,     1,        1.0},
    {43,  "Goose Encounter",            RARE,     52,    OPP,     EFF_IRON,       DB_IRON,            100,     0,        1.0,     5,        0.7},
    {44,  "I forgot",                   RARE,     48,    SELF,    EFF_SPECIAL,    DB_NONE,            100,     0,        1.0,     0,        1.0},
    {45,  "God of Destruction",         RARE,     124,   SELF,    EFF_PERMBTURNS, DB_NONE,            100,     3,        1.0,     0,        1.0},
    {46,  "Yoinky Sploinky",            RARE,     120,   SELF,    EFF_STEAL_IRON, DB_NONE,            100,     0,        0.5,     0,        1.0},
    //Sincards
    {47,  "Main Character Syndrome",    SINCARD,  400,   SELF,    EFF_SPECIAL,    DB_NONE},
    {48,  "Oops, all 3249 leaf clovers!",SINCARD, 400,   SELF,    EFF_SPECIAL,    DB_NONE},
};
#define NUMCOMMONCARD 21
#define NUMRARECARD 15
#define NUMSINCARD 2
#define NUMDEFENDCARD 9
#define TOTAL_CARDS (sizeof(cards) / sizeof(cards[0]))

struct CARDDESC carddesc[] = {
    {1, "Common", "Gain 5 attack"},
    {2, "Common", "Reduce enemy's equipped defence card's HP by 25/ hit count by 1"},
    {3, "Common", "Gain 10% of your current iron + 10"},
    {4, "Common", "Gain 10% of your current attack stat + 10"},
    {5, "Common", "Gain 5% of your current hp + 10"},
    {6, "Common", "50% chance to make the enemy lose one setup turn"},
    {7, "Common", "50% chance to make the enemy lose one battle turn"},
    {8, "Common", "Sacrifice 15 hp for 15 attack"},
    {9, "Common", "Gain an additional iron during interest, lose 10% of your iron"},
    {10, "Common", "25% chance to gain x1.25 attack"},
    {11, "Common", "Take a random card from your opponent"},
    {12, "Common", "Permanently gain a turn in both phases"},
    {13, "Common", "Gain 25 of your opponent's hp"},
    {14, "Common", "Steal 20 iron fron your opponent"},
    {15, "Common", "Increase resting iron interest cap by 5"},
    {16, "Common", "Gain 10% of the opponent's iron"},
    {17, "Common", "Sacrifice one set up turn for x1.75 attack"},
    {18, "Common", "Gain 2 more iron / 5 during interest, permanently lose a battle turn"},
    {19, "Common", "Steal 20% of your opponent's atk"},
    {20, "Common", "Gain x3 attack, lose a battle turn"},
    {21, "Common", "Gain a setup turn, lose 25% of your attack"},
    //Defence cards
    {22, "Defence", "Reduces damage taken by 20%, lasts for 4 hits"},
    {23, "Defence", "Reduces damage taken by 20, has HP equal to 7% of yours"},
    {24, "Defence", "Reduces damage taken by 50%, has HP equal to 15% of yours"},
    {25, "Defence", "For the next 2 hits, any and all damage taken is negated"},
    {26, "Defence", "When equipped; HP is set to 1, for 7 hits, all damage taken is negated"},
    {27, "Defence", "Next 2 hits: 50% of any damage taken is reflected to the opponent"},
    {28, "Defence", "10 damage is negated turned into iron, has HP equal to 10% of yours"},
    {29, "Defence", "Any damage is absorbed and turned to HP, lasts for 3 hits"},
    {30, "Defence", "For one hit, all damage is negated, and gain a turn in both phases"},
    //Rare cards
    {31, "Rare", "All % effects gain 57% (ex. 1% -> 58%)."},
    {32, "Rare", "Gain attack equal to your opponent's iron."},
    {33, "Rare", "Attack is set to 10% of your HP."}, 
    {34, "Rare", "1% chance to instantly kill the opponent"},
    {35, "Rare", "Both players gain x3 attack."},
    {36, "Rare", "7% chance to gain x7 attack, x7 iron, and 777 HP."},
    {37, "Rare", "Gain x10 attack. When a cycle ends, all iron is converted to attack."},
    {38, "Rare", "Gain 500 attack. Lose 50% HP."},
    {39, "Rare", "Choose a card ID. If your opponent owns that card, they lose that card."},
    {40, "Rare", "Pick between x2 attack and x2 iron. The other stat is halved."},
    {41, "Rare", "All iron is converted to attack."},
    {42, "Rare", "Gain 2 cards (if possible)."},
    {43, "Rare", "Opponent loses 30% of their iron + 5"},
    {44, "Rare", "Takes the effect of a random card (not including defence cards)"},
    {45, "Rare", "Permanently gain 3 battle turns"},
    {46, "Rare", "Steal 50% of the opponent's iron"},
    //Sincards
    {47, "Pride", "Gain 4 turns in both phases"},
    {48, "Greed", "All % effects gain x57 (ex. 10% -> 570%)"},
    {49, "Gluttony", "A stat of your choice is multiplied by 21.47"},
    {50, "Wrath", "Attacking 5 times wins the game"},
};

//Special global variables
int clovercount[2] = {0, 0};
bool heavenrest[2] = {false, false};
bool shop_needs_refresh = true; 


int Ran_100(){ //Random generation for all chance mechanics
    return rand() % 100 + 1;
}

//Generate a _____ card (based on the function)
int Ran_Com(){ //Common card
    return rand() % NUMCOMMONCARD;
}
int Ran_Rar(){ //Rare card
    return rand() % NUMRARECARD + (NUMCOMMONCARD + NUMDEFENDCARD); //This makes sure it can only return rare card ids
}
int Ran_Def(){ //Defence card
    return rand() % NUMDEFENDCARD + NUMCOMMONCARD; //This makes sure it can only return def card ids
}
int Ran_Sin(){ //Sincard
    //return rand() % NUMSINCARD + NUMRARECARD + NUMCOMMONCARD + NUMDEFENDCARD;
}

void clear(){ //Clear Terminal, purely cosmetic
    printf("\e[1;1H\e[2J");
}
void FULLCARDPRINT(int p, enum MODE mode, struct CARD c[], int ccount){
    printf("%-6s%-40s%-15s%-85s", "ID", "Name", "Type", "Description");
    if (mode == SHOP){
        printf("%s", "Supplement Cost");
    }
    printf("\n\n");
    for (int i = 0; i < ccount; i++){
        printf("%-6i%-40s%-15s%-85s", c[i].id, c[i].name, carddesc[c[i].id - 1].type, carddesc[c[i].id - 1].desc);
        if (mode == SHOP){
            printf("%i", c[i].cost);
        }
        printf("\n\n");
    }
    return;
}

int CHOICECHECK(int p, enum MODE mode, struct CARD c[], int ccount, int *out_choice){
    printf("Please enter the ID of the card you'd like to select\n");
    printf("Enter -1 if you want to go back to the menu\n");
    if (mode == SHOP){
        printf("Or, if you would like to reroll the shop, enter 0\n");
    }
    int choice;
    scanf("%i", &choice);
    getchar();
    if (choice == -1){
        printf("Backed out. \n\n");
        return BACKED;
    }
    else if (choice == 0 && mode == SHOP){
        return REROLL;
    }
    bool douownthis = false;
    int thiswasntoffered = 0;
    for (int i = 0; i < ccount; i++){
        if (c[i].id == choice && mode != SHOP){
            douownthis = true;
        }
        if (c[i].id == choice){
            thiswasntoffered += 1;
        }
    }
    
    if (choice < 1 || choice > TOTAL_CARDS){
        printf("How do you miss the mark that much dude, that isn't even a card in the game \n\n");
        return INVALID;
    }
    else if (!douownthis && mode != SHOP){
        printf("You don't own this. Please pick a card that you actually own.\n\n");
        return INVALID;
    }
    else if (thiswasntoffered == 0){
        printf("This card isn't in the collection shown dumdum \n\n");
        return INVALID;
    }
    else if (mode == SHOP && player[p].iron < cards[choice - 1].cost){
        printf("you'll die by buying this right now, your life depends on those iron supplements bro \n\n");
        return INVALID;
    }
    else{
        *out_choice = choice;
        return SUCCESS;
    }
}

int ATKf(int p, int opp){
    clear();
    printf("Attacking enemy. Are you really sure you'd like to do this? (y/n)\n");
    char choice;
    scanf(" %c", &choice);
    getchar();
    clear();
    if (choice == 'n'){
        printf("ok\n\n");
        return BACKED;
    }
    else if (choice == 'y'){
        printf("ok\n\n");
        float dmg = player[p].atk;
        struct CARD def = player[opp].equipdef;
        if (def.id != 0){
            printf("Your enemy's defence card, %s, activated! \n", cards[def.id - 1].name);
            printf("Its effect: %s\n\n", carddesc[def.id - 1].desc);
            float def_dmg = dmg;
            float initial_dmg = dmg;
            dmg -= def.db_addvalue;
            dmg *= def.db_multvalue;
            def_dmg = initial_dmg - dmg;
            if (dmg <= 0){
                dmg = 0;
                printf("their defence card negated all damage\n");
            }
            else{
                printf("Their defence card negated %g damage\n\n", def_dmg);
            }
            getchar();
            switch (def.id){
                case 27: //Mirror Force
                    float reflect_dmg = dmg;
                    player[p].hp -= reflect_dmg;
                    printf("Your enemy's Mirror force reflected %g damage back at you\n\n", reflect_dmg);
                    break;
                case 28: //No Pain No Gain
                    player[opp].iron += 10;
                    printf("Your enemy's No Pain No Gain converted 10 damage into iron. They gain 10 iron.\n\n");
                    break;
                case 29: //
                    player[opp].hp += initial_dmg;
                    printf("Your enemy's Gluttony absorbed the damage and healed them for %g HP.\n\n", initial_dmg);
                    break;
                case 30:
            }
            switch (def.efftype){
                //Only deduct hp if the type of def is HP
                case DEF_HP:
                    player[opp].equipdef.multvalue -= def_dmg;
                    printf("It is at %g hp.\n\n", player[opp].equipdef.multvalue);
                    if (player[opp].equipdef.multvalue <= 0){
                        printf("Your enemy's defence card shattered!\n\n");
                        player[opp].equipdef.id = 0;
                    }
                    break;
                case DEF_HITS: 
                    player[opp].equipdef.addvalue--;
                    printf("It can take %i more hits\n\n", player[opp].equipdef.addvalue);
                    if (player[opp].equipdef.addvalue <= 0){
                        printf("Your enemy's defence card shattered!\n\n");
                        player[opp].equipdef.id = 0;
                    }
                    break;
            }
        }
        getchar();
        player[opp].hp -= dmg;
        printf("Your enemy took %g damage! They have %g hp. \n\n", dmg, player[opp].hp);
    }
    else{
        printf("Invalid input dummy, im forcing you back to the menu \n\n");
        return INVALID;
    }
    return SUCCESS;
}
int ExecuteCard(int p, int opp, int useid){
    useid--; //0 indexing
    int targets[2];
    int targetcount = 0;
    switch (cards[useid].target){
        case SELF:
            targets[0] = p, targets[1] = opp;
            targetcount = 1;
            break;
        case OPP:
            targets[0] = opp, targets[1] = p;
            targetcount = 1;
            break;
        case BOTH:
            targets[0] = p, targets[1] = opp;
            targetcount = 2;
            break;
    }
    if (100 - cards[useid].procchance < Ran_100() + (57 * clovercount[p])){
        int addvalue = cards[useid].addvalue;
        float multvalue = cards[useid].multvalue + clovercount[p];
        int db_addvalue = cards[useid].db_addvalue;
        float db_multvalue = cards[useid].db_multvalue;
        for (int i = 0; i < targetcount; i++){
            switch (cards[useid].efftype){
                case EFF_HP:
                    player[targets[i]].hp += addvalue;
                    player[targets[i]].hp *= multvalue;
                    break;
                case EFF_IRON:
                    player[targets[i]].iron += addvalue;
                    player[targets[i]].iron *= multvalue;
                    break;
                case EFF_ATK:
                    player[targets[i]].atk += addvalue;
                    player[targets[i]].atk *= multvalue;
                    break;
                case EFF_INTEREST:
                    player[targets[i]].ironinterest += addvalue;
                    break;
                

                /////Issue with stealing effects!!!!!
                case EFF_STEAL_HP:
                    if (player[targets[1-i]].iron < addvalue){
                        addvalue = player[targets[1-i]].hp;
                    }
                    player[targets[i]].hp += addvalue;
                    player[targets[i]].hp *= player[opp].hp * (1 + multvalue);
                    player[targets[1-i]].hp -= addvalue;
                    player[targets[1-i]].hp *= (1 - multvalue);
                    break;
                case EFF_STEAL_IRON:
                    if (player[targets[1-i]].iron < addvalue){
                        addvalue = player[targets[1-i]].iron;
                    }
                    player[targets[i]].iron += addvalue;
                    player[targets[i]].iron *= player[opp].iron * (1 + multvalue);
                    player[targets[1-i]].iron -= addvalue;
                    player[targets[1-i]].iron *= (1 - multvalue);
                    break;
                case EFF_STEAL_ATK:
                    if (player[targets[1-i]].iron < addvalue){
                        addvalue = player[targets[1-i]].iron;
                    }
                    player[targets[i]].atk += addvalue;
                    player[targets[i]].atk *= player[opp].iron * (1 + multvalue);
                    player[targets[1-i]].atk -= addvalue;
                    player[targets[1-i]].atk *= (1 - multvalue);
                    break;
                case EFF_SET_ATK:
                    player[targets[i]].atk = addvalue;
                    break;
                case EFF_SET_HP:
                    player[targets[i]].hp = addvalue;
                    break;
                case EFF_PERMBTURNS:
                    player[targets[i]].turns_def[0] += addvalue; //PERM also increases current turns, bleeds into next case to do so
                case EFF_BATTLETURN:
                    player[targets[i]].turns[0] += addvalue;
                    break;
                case EFF_PERMSTURNS:
                    player[targets[i]].turns_def[1] += addvalue;
                case EFF_SETUPTURN:
                    player[targets[i]].turns[1] += addvalue;
                    break;
                case EFF_SPECIAL:
                    switch (cards[useid].id){
                        case 2:                                    //Fabric Softener
                            switch (player[opp].equipdef.efftype){
                                case DEF_HP:
                                    player[opp].equipdef.multvalue -= db_multvalue;
                                    break;
                                case DEF_HITS:
                                    player[opp].equipdef.addvalue -= db_addvalue;
                                    break;
                                default:
                                    printf("The enemy doesn't have a defence card equipped\n\n");
                                    getchar();
                                    clear();
                                    return INVALID;
                            }
                            break;
                        case 11:                                    //Scalper
                            if (player[opp].invspace <= 0){
                                printf("Your enemy has no cards to donate\n\n");
                                return INVALID;  
                            }
                            while (true){
                                int scalpran = rand() % 10;
                                if (player[opp].cardinv[scalpran].id != 0){
                                    for (int s = 0; s < 10; s++){
                                        if (player[p].cardinv[s].id == 0){
                                            player[p].cardinv[s] = player[opp].cardinv[scalpran];
                                            player[opp].cardinv[scalpran].id = 0;
                                            player[p].invspace++;
                                            player[opp].invspace--;
                                            printf("Scalper stole the card %s from the opponent\n\n", player[p].cardinv[s].name);
                                            getchar();
                                            break;
                                            }
                                    }
                                }
                            }
                            return 0;
                        case 12:                           //Sin of greed
                            player[targets[i]].turns[0]++;
                            player[targets[i]].turns_def[0]++;
                            player[targets[i]].turns[1]++;
                            player[targets[i]].turns_def[1]++;
                            break;
                        case 15:                           //Limiter off
                            player[p].interestcap += 5;
                            break;
                        case 31:                           //Clover
                            clovercount[p]++;
                            break;
                        case 36:                            //Jackpot
                            player[p].iron *= 7;
                            player[p].atk *= 7;
                            player[p].hp += 777;
                            printf("You just hit the JACKPOT!!! \n");
                            printf("Gained x7 iron, x7 attack, and +777 hp\n\n");
                            getchar();
                            return SUCCESS;
                        case 37:                           //Heavenly Restriction
                            player[p].atk *= 10;
                            player[p].iron = 0;
                            heavenrest[p] = true;
                            break;
                        case 39:                      //Go fish
                            printf("%s, what card do you think your opponent has?\n", player[p].name);
                            printf("Enter a card's ID to guess (1-53)\n");
                            int guess;
                            scanf("%i", &guess);
                            getchar();
                            for (int a = 0; a < 10; a++){
                                if (player[opp].cardinv[a].id == guess){
                                    player[opp].cardinv[a].id = 0;
                                    player[opp].invspace--;
                                    printf("Your opponent had card number %i!\n", guess);
                                    printf("They have lost that card\n\n");
                                    getchar();
                                    return SUCCESS;
                                }
                            }
                            printf("Go fish lol\n");
                            getchar();
                            break;
                        case 40:           //Trolley problem
                            printf("Two stats are on different rails.\n");
                            printf("On the left, you'll see your attack stat.\n");
                            printf("And on the right you'll see your iron supplements.\n");
                            printf("Which do you save? (1 for attack, 2 for iron)\n");
                            printf("No backing out now...\n");
                            int trolley_choice;
                            scanf("%i", &trolley_choice);
                            switch (trolley_choice){
                                case 1:
                                    player[p].atk *= 2;
                                    player[p].iron *= 0.5;
                                    printf("Your attack doubles, but your iron is halved.\n");
                                    getchar();
                                    return SUCCESS;
                                case 2:
                                    player[p].iron *= 2;
                                    player[p].atk *= 0.5;
                                    printf("Your iron doubles, but your attack is halved.\n");
                                    getchar();
                                    return SUCCESS;
                                default:
                                    printf("In your indecisiveness, you fail to save any of them.");
                                    printf("Both stats are halved.");
                                    player[p].iron *= 0.5;
                                    player[p].atk *= 0.5;
                                    break;
                            }
                            break;
                        default:
                            printf("There is a problem mr coder\n\n");
                            break;
                    }
                    break;
                case DEF_HP:
                case DEF_HITS:
                    if (useid == 25) {//Last stand check
                        player[p].hp = 1;
                    }
                    player[p].equipdef = cards[useid];
                    for (int idx = 0; idx < player[p].invspace; idx++){
                        if (player[p].equipdef.id - 1 == player[p].cardinv[idx].id){
                            player[p].cardinv[idx].id = 0;
                        }
                    }
                    break;
                default:
                    printf("Something went wrong mr coder\n");
            }
            switch (cards[useid].drawback){
                case DB_NONE:
                    break;
                case DB_HP:
                    player[targets[i]].hp -= cards[useid].db_addvalue;
                    player[targets[i]].hp *= cards[useid].db_multvalue;
                    break;
                case DB_ATK:
                    player[targets[i]].atk -= cards[useid].db_addvalue;
                    player[targets[i]].atk *= cards[useid].db_multvalue;
                    break;
                case DB_IRON:
                    player[targets[i]].iron -= cards[useid].db_addvalue;
                    player[targets[i]].iron *= cards[useid].db_multvalue;
                    break;
                case DB_BATTLETURNS:
                    player[targets[i]].turns[0] -= cards[useid].db_addvalue;
                    break;
                case DB_SETUPTURNS:
                    player[targets[i]].turns[1] -= cards[useid].db_addvalue;
                    break;
                case DB_PBATTLETURNS:
                    player[targets[i]].turns[0] -= cards[useid].db_addvalue;
                    player[targets[i]].turns_def[0] -= cards[useid].db_addvalue;
                    break;
            }
        }
        printf("Successfully used %s \n", cards[useid].name);
        printf("Card's effect: %s \n", carddesc[useid].desc);
        getchar();
    }
    else {
        printf("Your card failed the performance check lol \n\n");
    }
    return SUCCESS;
}
int USECARDf(int p, int opp){
    clear();
    printf("This action will allow you to use a card.\n");
    printf("Here is your current inventory: \n");
    struct CARD tempc[10];
    int tempc_i = 0;
    for (int i = 0; i < 10; i++){
        if (player[p].cardinv[i].id != 0){
            tempc[tempc_i] = player[p].cardinv[i];
            tempc_i++;
        }
    }
    int useid;
    int p_useid;
    bool choicecont = true;
    while (choicecont){
        FULLCARDPRINT(p, INV, tempc, tempc_i);
        printf("\n");
        enum SUCCESSPARITY status = CHOICECHECK(p, INV, tempc, tempc_i, &useid);
        switch (status){
            case INVALID:
                getchar();
                clear();
                break;
            case BACKED:
                return BACKED;
            case SUCCESS:
                for (int i = 0; i < 10; i++){
                    if (player[p].cardinv[i].id == useid){
                        p_useid = i;
                    }
                }
                choicecont = false;
                break;
        }
    }
    player[p].cardinv[p_useid].id = 0;
    player[p].invspace--;
    switch(ExecuteCard(p, opp, useid)){
        case INVALID:
            return INVALID;
        case SUCCESS:
            break;
    }
    return SUCCESS;
}

void SHOPGEN(int i, int p, int ccount, int *scardc, int *rcardc, struct CARD shop_cards[]){
    int ran_g = 0;
    enum CARDTYPE gentype;
    
    if (i >= 6){
        //Last two cards in the shop must be defence
        gentype = DEF;
    }
    else {
        int ran_t = Ran_100();
        
        if (*scardc > 0 && (ran_t <= *scardc || *scardc >= 100)){
            gentype = SINCARD;    
        }
        else if (*rcardc > 0 && (ran_t <= *rcardc || *rcardc >= 100)){
            gentype = RARE;
        }
        else {
            gentype = COMMON;
        }
    }
    
    int pool_start = 0;
    int pool_size = 0;
    switch (gentype){
        case COMMON:  
            pool_start = 0; 
            pool_size = NUMCOMMONCARD; 
            break;
        case DEF:     
            pool_start = NUMCOMMONCARD; 
            pool_size = NUMDEFENDCARD; 
            break;
        case RARE:    
            pool_start = NUMCOMMONCARD + NUMDEFENDCARD; 
            pool_size = NUMRARECARD; 
            break;
        case SINCARD: 
            pool_start = NUMCOMMONCARD + NUMDEFENDCARD + NUMRARECARD; 
            pool_size = NUMSINCARD; 
            break;
    }
    
    int owned_count = 0;
    for (int idx = pool_start; idx < pool_start + pool_size; idx++) {
        for (int b = 0; b < 10; b++) {
            if (player[p].cardinv[b].id == cards[idx].id) {
                owned_count++;
                break;
            }
        }
    }
    if (owned_count >= pool_size) {
        gentype = COMMON;
    }
    
    bool is_duplicate = true;
    while (is_duplicate){
        switch (gentype){
            case COMMON:  
                ran_g = Ran_Com(); 
                break;
            case RARE:    
                ran_g = Ran_Rar(); 
                break;
            case SINCARD: 
                ran_g = Ran_Sin(); 
                break;
            case DEF:     
                ran_g = Ran_Def(); 
                break;
        }
        
        is_duplicate = false;
        for (int a = 0; a < i; a++) {
            if (shop_cards[a].id == cards[ran_g].id) {
                is_duplicate = true; 
                break; 
            }
        }
        if (!is_duplicate) {
            for (int b = 0; b < 10; b++){
                if (player[p].cardinv[b].id == cards[ran_g].id){
                    is_duplicate = true;
                    break;
                }
            }
        }
    }
    
    switch (gentype){
        case SINCARD:
            *scardc -= 100;
            if (*scardc < 0) {
                *scardc = 0;
            }
            break;
        case RARE:
            *rcardc -= 100;
            if (*rcardc < 0) {
                *rcardc = 0;
            }
            *scardc = *rcardc / 10; 
            break;
    }
    //slot the chosen card into the shop array
    shop_cards[i] = cards[ran_g];
}
int IronCommit(int p){
    printf("How much iron would you like to commit?\n");
    printf("(Enter -1 to return to the main menu)\n");
    int comamt;
    scanf("%i", &comamt);
    getchar();
    switch (comamt){
        case -1:
            printf("Backed out \n\n");
            return BACKED;
        default:
            if (comamt < 0){
                printf("You can't do that\n\n");
                getchar();
                clear();
                return INVALID;
            }
            else if (comamt > player[p].iron){
                printf("Doing this will kill you, your life depends on these supplements man\n\n");
                getchar();
                clear();
                return INVALID;
            }
            else{
                player[p].iron -= comamt;
                printf("Committed %i iron.\n\n", comamt);
                getchar();
                clear();
                return comamt;
            }
    }
}
int SHOPf(int p){
    clear();
    printf("=---------Welcome to the SHOP!----------=\n\n");
    //Static struct array, keeps the shop intact 
    static struct CARD shop_cards[8];
    int ccount = 8; 
    int rcardc = 0, scardc = 0; 
    int reroll_count = 0;
    int comamt;
    if (shop_needs_refresh){
        comamt = IronCommit(p);
        switch (comamt){
            case INVALID:
            case BACKED:
                return BACKED;
            default:
                rcardc = comamt;
                scardc = rcardc / 10;
                break;
        }
    }
    while (true){
        bool bought_card = false;
        if (shop_needs_refresh) { //Does the shop need to refresh?
            //Generate new cards
            for (int i = 0; i < ccount; i++){
                SHOPGEN(i, p, ccount, &scardc, &rcardc, shop_cards);
            }
            shop_needs_refresh = false;
        } 
        else {
            printf("[!] Retrieving available items from your previous shop visit...\n\n");
        }
        printf("You have %i iron supplements at the moment\n\n", player[p].iron);
        printf("Here are your options: \n");
        FULLCARDPRINT(p, SHOP, shop_cards, ccount);
        printf("\n");
        int c_choice; //Chosen Card ID

        //Status of the choicecheck
        enum SUCCESSPARITY status = CHOICECHECK(p, SHOP, shop_cards, ccount, &c_choice);
        switch (status){
            case INVALID:
                getchar();
                clear();
                break;
            case BACKED:
                shop_needs_refresh = false;
                return BACKED;
            case SUCCESS:
                if (player[p].invspace >= 10){
                    printf("You don't have room for this card\n");
                    return INVALID;
                }
                c_choice--; //Zero indexing
                for (int i = 0; i < 10; i++) { 
                    if (player[p].cardinv[i].id == 0) { 
                        player[p].cardinv[i] = cards[c_choice]; 
                        player[p].iron -= cards[c_choice].cost;  
                        player[p].invspace++;           
                        bought_card = true;
                        printf("Bought %s!\n\n", cards[c_choice].name);
                        printf("Card's effect: %s \n\n", carddesc[c_choice].desc);
                        printf("You can only buy one card per shop.\n");
                        printf("To buy more, you'll need to reroll the shop.\n\n");
                        getchar();
                        break;
                    }
                }
            case REROLL:
                int reroll_cost = 5 + (5 * reroll_count);
                printf("It will cost %i iron to reroll the shop. Is this ok? (y/n)\n\n", reroll_cost);
                char yn;
                scanf(" %c", &yn);
                getchar();
                if (yn == 'n'){
                    printf("Ok, shop isn't rerolled\n\n");
                    if (bought_card){
                        shop_needs_refresh = false;
                        bought_card = false;
                        return SUCCESS;
                        break;
                    }
                    else{
                        getchar();
                        clear();
                        break;
                    }
                }
                else if (yn == 'y'){
                    if (player[p].iron < reroll_cost){
                        printf("You'll die doing this. Your life depends on these supplements man\n\n");
                        getchar();
                        clear();
                    }
                    else{
                        player[p].iron -= reroll_cost;
                        printf("Shop has been rerolled.\n\n");
                        shop_needs_refresh = true;
                        bought_card = false;
                        reroll_count++;
                        getchar();
                        clear();
                    }
                }
                break;
        }
    }
    return SUCCESS;
}
int RESTf(int p){
    printf("This action will make you rest. You are currently at %i consecutive rests. \n", player[p].consecutive_rest);
    if (player[p].consecutive_rest >= 4){
        printf("Warning: this is a lot of iron gain at once.\n");
        printf("It could be harmful.\n");
    }
    printf("Are you sure you'd like to rest? (y/n): \n");
    char choice;
    scanf(" %c", &choice);
    getchar();
    if (choice == 'n'){
        printf("ok \n");
        return BACKED;
    }
    else if (choice == 'y'){
        printf("You have rested. \n");
        int add_iron = player[p].consecutive_rest; //Additional iron per 5 iron
        if (player[p].consecutive_rest >= 2){
            add_iron = 2;
        }
        // Iron = interest + additional * total iron / 5
        int irongain = (player[p].ironinterest + add_iron) * (player[p].iron / 5);
        
        //Guarantee one iron if you don't have enough for an interest threshold
        if (irongain <= 0){
            irongain = 1;
        }
        //Cap resting interest
        else if (irongain > player[p].interestcap){
            irongain = player[p].interestcap;
        }
        player[p].iron += irongain;
        
        printf("Gained %i iron\n", irongain);
        int hpgain = 10;
        if (player[p].consecutive_rest >= 4 && Ran_100() <= 60){
            hpgain = -50;
            printf("Iron supplements course through your veins. This isn't a good thing though.\n");
            printf("Lost 50 hp.");
        }
        else{
            printf("Gained %i hp.\n", hpgain);
        }
        player[p].consecutive_rest++;
        player[p].hp += hpgain;
        return SUCCESS;
    }
    else {
        printf("dude i gave you a clear choice \n");
        return INVALID;
    }
}
void Turn_End(int *p, int *opp){
    getchar();
    shop_needs_refresh = true; //If a turn-ending action occurs, refresh the shop
    player[*p].turns[player[*p].current_phase]--;
    if (player[*p].turns[player[*p].current_phase] <= 0) {
        switch (player[*p].current_phase){
            case SETUP:
                player[*p].current_phase = BATTLE;
                break;
            case BATTLE:
                player[*p].current_phase = SETUP;
                int irongain = player[*p].ironinterest * (player[*p].iron / 5); //Calculate interest
                if (irongain <= 0){
                    irongain = 1;
                }
                else if (irongain > player[*p].interestcap * 2){
                    irongain = player[*p].interestcap * 2;
                }
                player[*p].iron += irongain; //Calculate interest
                clear();
                printf("Iron interest has been applied. You have gained %i iron supplements.\n", irongain);
                break;
        }
        //Reset counter back to however many needed
        player[*p].turns[player[*p].current_phase] = player[*p].turns_def[player[*p].current_phase];
        
        //Only take iron away if a cycle ended
        if (heavenrest[*p] == true){
            player[*p].atk += player[*p].iron;
            player[*p].iron = 0;
            printf("You are heavenly restricted, all your iron was converted to attack.\n");
            getchar();
        }
        
        //Switch players, reset turn counter
        *p = 1 - *p;
        *opp = 1 - *opp;
    }
}
void Tutorial_Thomah(){
    getchar();
    printf("Not Generic RPG Roguelike Game is most likely unlike any game you've ever played before. \n");
    getchar();
    printf("The goal of the game is to reduce your enemy's hp to 0 by attacking them. \n");
    getchar();
    printf("There are several things in this game you can use to assist in your objective. \n");
    printf("Enter the corresponding number to learn about each action: \n");
    while(true){
        //Options go here

        printf("(Enter -1 if you are ready to play)\n");
        int choice;
        scanf("%i", &choice);
        getchar();
        clear();
        switch (choice){
            case -1:
                printf("Ok get ready...");
                return;
            case 1:
                printf("In this game, you'll need to attack your enemy to get their hp to 0. \n");
                printf("When attacking, you deal damage to your opponent equal to your attack stat. \n");
                getchar();
                printf("Note that by attacking, you will also trigger the effect of whatever defence card the opponent has equipped.\n");
                printf("(See cards for more information)\n");
                getchar();
                clear();
                break; //not ckai was here!!!! :)))))))) (Mr Zhang, this was a friend of mine, please pay him no attention)
            case 2:
                
                getchar();
                clear();
                break;
            case 3:
                printf("Each player has their very own inventory to store the cards used in this game. \n");
                printf("Each person can only hold 10 cards at a time. If you try to get above this, you'll be asked to lose a card.\n");
                getchar();
                clear();
                break;
            default:
                printf("That option doesn't exist yet\n");
        }
        printf("Anything else you need to know? \n");
    }
}
//main program
int main(){ //Main program loop
    srand(time(NULL));
    int p = 0;
    int opp = 1;
    cards[15].addvalue = player[opp].iron / 10;  //Prescription's iron gain
    //Defence cards that need %based hp
    cards[22].multvalue = player[p].hp * 0.07; //Frying pan
    cards[23].multvalue = player[p].hp * 0.3;  //Strong stance
    cards[27].multvalue = player[p].hp * 0.1;  //No pain no gain
    //Rare cards
    cards[31].addvalue = player[opp].iron;
    cards[32].addvalue = player[p].hp / 10;
    clear();
    printf("Welcome to Not Generic RPG Roguelike game\n");
    printf("Would you like to view the tutorial? (y/n)\n");
    char manual_choice;
    scanf(" %c", &manual_choice);
    clear();
    if (manual_choice == 'y') {
        Tutorial_Thomah();
    }
    else{
        printf("get on with it then\n");
    }
    printf("\nPress enter to continue\n");
    getchar();
    clear();

    printf("Player 1, Enter your name (please don't use spaces, max 30 characters): ");
    char newname[30];
    scanf("%s", newname);
    strcpy(player[p].name, newname);
    printf("Player 2, Enter your name (please don't use spaces, max 30 characters): ");
    scanf("%s", newname);
    strcpy(player[opp].name, newname);
    clear();

    enum PHASE curphase;
    printf("Press enter to continue ");
    getchar();
    while (player[p].hp > 0 && player[opp].hp > 0){
        clear();
        printf("=-------Main Menu--------=\n\n");
        printf("It is %s's turn\n\n", player[p].name);
        printf("HP: %-15gAttack: %-15gIron Supplements: %-15i\n\n", player[p].hp, player[p].atk, player[p].iron);
        curphase = player[p].current_phase; 
        switch (curphase){
            case SETUP:
                printf("It is currently the setup phase (%i turns left)\n\n", player[p].turns[player[p].current_phase]);
                break;
            case BATTLE:
                printf("It is currently the battle phase (you can attack now) (%i turns left)\n\n", player[p].turns[player[p].current_phase]);
                break;
        }
        printf("Player %s, please choose one of the options below\n\n", player[p].name);
        printf("1. Attack your enemy\n");
        printf("2. Use a card in your inventory\n");
        printf("3. Sell a card in your inventory\n");
        printf("4. Enter the shop\n");
        printf("5. Display your stats\n");
        printf("6. Rest\n\n");
        printf("DEBUG ONLY 57. Gain 700 iron\n\n");
        printf("Which action would you like to take? (Enter the corresponding number)\n");
        int action;
        scanf("%i", &action);
        bool changeturn = false;
        getchar();
        switch(action){
            case 1:
                // No attack if it isn't the battle phase
                if (curphase == SETUP){
                    printf("You cannot attack now, it is the set up phase right now \n\n");
                    getchar();
                    continue;
                }
                switch (ATKf(p, opp)){
                    case SUCCESS:
                        changeturn = true;  
                        player[p].iron += 5;
                        printf("Performed an action. Passively gained 5 iron. \n\n");
                        break;
                    case INVALID:
                    case BACKED:
                        getchar();
                        continue;
                }
                break;
            case 2:
                if (player[p].invspace == 0){
                    printf("You don't have any cards to use idiot.\n\n");
                    getchar();
                    continue;
                }
                USECARDf(p, opp);
                continue;
            case 3:
                if (player[p].invspace == 0){
                    printf("You don't have any cards to sell idiot.\n\n");
                    getchar();
                    continue;
                }
                clear();
                printf("Sell a card in your inventory\n");
                break;
            case 4:
                if (player[p].invspace >= 10){
                    printf("You don't have any room to buy more cards\n");
                    getchar();
                    continue;
                }
                switch (SHOPf(p)){
                    case SUCCESS:
                        changeturn = true;
                        break;
                    case INVALID:
                    case BACKED:
                        continue;
                }
                break;
            case 5:
                printf("Display your stats\n");
                continue;
            case 6:
                switch (RESTf(p)){
                    case SUCCESS:
                        break;
                    case INVALID:
                    case BACKED:
                        getchar();
                        continue;
                }
                changeturn = true;
                break;
            case 57:
                printf("Debug money increase turn\n");
                getchar();
                player[p].iron += 700;
                continue;
            default:
                printf("How do you miss that much you disgrace to the earth. Your oxygen should be given to someone else. You know what? I'm skipping your turn scum\n\n");
                getchar();
                changeturn = true;
                break;
        }
        //Modularly change every card's values that are based on player stats
        cards[15].addvalue = player[opp].iron / 10;  //Prescription's iron gain
        //Defence cards that need %based hp
        cards[22].multvalue = player[p].hp * 0.07;
        cards[23].multvalue = player[p].hp * 0.15;
        if (changeturn){
            if (action != 6){
                player[p].consecutive_rest = 0;
            }
            Turn_End(&p, &opp);
        }
    }    
    if (player[0].hp <= 0){
        p = 1;
        opp = 0;
    }
    else{
        p = 0;
        opp = 1;
    }
    clear();
    printf("%s is dead\n", player[opp].name);
    printf("%s won the game!!!!!!!!", player[p].name);
    return 0;
}