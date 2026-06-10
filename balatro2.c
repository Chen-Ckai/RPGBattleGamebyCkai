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
    EFF_DEF,
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
    EFF_RESTCOUNT,
    EFF_SPECIAL,      //Jackpot, Go Fish, etc.
    DEF_HP,
    DEF_TURNS,
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
    //For defence cards, addvalue is the turn duration, multvalue is the hp, <------------------------------------
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
    int consecutive_rest;
    int turns[2];
    enum PHASE current_phase; 
    int turns_def[2];
    struct CARD equipdef;
    struct CARD cardinv[10];
    int invspace;
    int irondefic[2];
};

struct Player player[2] = {
    //Name       HP     ATK  IRON  Interest   RESTCOUNT  Battle Setup  Phase   Default turn amt   EquippedDefs INVENTORY & SPACE filled
    {"Player 1", 1000,  5,   10,   2,         0,         {2,    2},    SETUP,  {2, 2},            {0},         {0},        0,          {0}},
    {"Player 2", 1000,  5,   15,   3,         0,         {2,    2},    SETUP,  {2, 2},            {0},         {0},        0,          {0}}
};
struct CARD cards[] = {  //COMPLETE DATABASE OF EVERY CARD IN THE GAME
//   ID    Name                         Type      Cost   Target   EffectType      Drawback            Chance   Positive Values    Negative Values
    {1,   "Sharpening Stone",           COMMON,   2,     SELF,    EFF_ATK,        DB_NONE,            100,     5,        1.0,     0,        1.0},
    {2,   "Fabric Softener",            COMMON,   10,    OPP,     EFF_DEF,        DB_NONE,            100,     25,       1.0,     0,        1.0},
    {3,   "Banana Farm",                COMMON,   7,     SELF,    EFF_IRON,       DB_NONE,            100,     10,       1.1,     0,        1.0},
    {4,   "Baguette Farm",              COMMON,   7,     SELF,    EFF_ATK,        DB_NONE,            100,     10,       1.1,     0,        1.0},
    {5,   "Cheesecake Farm",            COMMON,   7,     SELF,    EFF_HP,         DB_NONE,            100,     10,       1.1,     0,        1.0},
    {6,   "No more honk shoo",          COMMON,   12,    OPP,     EFF_SETUPTURN,  DB_NONE,            50,      0,        1.0,     1,        1.0},
    {7,   "Cat sitting on your lap",    COMMON,   14,    OPP,     EFF_BATTLETURN, DB_NONE,            50,      0,        1.0,     1,        1.0},
    {8,   "Double Edged Sword",         COMMON,   15,    SELF,    EFF_ATK,        DB_HP,              100,     15,       1.0,     15,       1.0},
    {9,   "Accounting Masterclass",     COMMON,   15,    SELF,    EFF_INTEREST,   DB_NONE,            100,     1,        1.0,     0,        0.9},
    {10,  "Wheel of Fortune",           COMMON,   5,     SELF,    EFF_ATK,        DB_NONE,            25,      0,        1.25,    0,        1.0},
    {11,  "Semi-consentual Robbery",    COMMON,   13,    OPP,     EFF_SPECIAL,    DB_NONE,            100,     0,        1.0,     0,        1.0},
    {12,  "Sin of Greed",               COMMON,   50,    SELF,    EFF_SPECIAL,    DB_NONE,            100,     0,        1.0,     1,        1.0},
    {13,  "Yummy yummy in my tummy",    COMMON,   26,    SELF,    EFF_STEAL_HP,   DB_NONE,            100,     0,        0.05,    0,        1.0},
    {14,  "Non-consentual Robbery",     COMMON,   20,    SELF,    EFF_STEAL_IRON, DB_NONE,            100,     10,       1.0,     0,        1.0},
    {15,  "Anesthesia",                 COMMON,   12,    SELF,    EFF_RESTCOUNT,  DB_NONE,            100,     5,        1.0,     0,        1.0},
    {16,  "Prescription",               COMMON,   8,     SELF,    EFF_IRON,       DB_NONE,            100,     -1,       1.0,     0,        1.0},
    {17,  "Burn Out",                   COMMON,   19,    SELF,    EFF_IRON,       DB_SETUPTURNS,      100,     0,        2.0,     1,        1.0},
    {18,  "Investment",                 COMMON,   10,    SELF,    EFF_INTEREST,   DB_PBATTLETURNS,    100,     3,        1.0,     1,        1.0},
    {19,  "Famine",                     COMMON,   10,    OPP,     EFF_DEBUFF,     DB_NONE,            100,     1,        1.0,     0,        1.0},
    {20,  "Bad Day",                    COMMON,   25,    SELF,    EFF_ATK,        DB_PBATTLETURNS,    100,     0,        3.0,     1,        1.0},
    {21,  "I have no enemies",          COMMON,   18,    SELF,    EFF_SETUPTURN,  DB_ATK,             100,     1,        1.0,     0,        0.75},
    {22,  "Boiling Pot",                DEF,      15,    SELF,    DEF_TURNS,      DB_NONE,            100,     4,        1.0,     0,        0.1},
    {23,  "Frying Pan",                 DEF,      15,    SELF,    DEF_HP,         DB_NONE,            100,     0,        1.0,     20,       0.0},
    {24,  "Strong Stance",              DEF,      75,    SELF,    DEF_HP,         DB_NONE,            100,     0,        1.0,     0,        0.5},
    {25,  "Barrier",                    DEF,      150,   SELF,    DEF_TURNS,      DB_NONE,            100,     3,        1.0,     0,        1.0},
    {26,  "Last Stand",                 DEF,      100,   SELF,    DEF_SPECIAL,    DB_NONE,            100,     0,        1.0,     0,        1.0},
    {27,  "Mirror Force",               DEF,      47,    SELF,    DEF_SPECIAL,    DB_NONE,            100,     0,        1.0,     0,        0.5},
    {28,  "Glass shield",               DEF,      25,    SELF,    DEF_HP,         DB_NONE,            100,     0,        1.0,     0,        1.0},
    {29,  "Placeholder Card 29",        DEF,      0,     SELF,    EFF_HP,         DB_NONE,            100,     0,        1.0,     0,        1.0},
    {30,  "Placeholder Card 30",        DEF,      0,     SELF,    EFF_HP,         DB_NONE,            100,     0,        1.0,     0,        1.0},
    {31,  "Oops, all 57 leaf clovers!", RARE,     157,   SELF,    EFF_SPECIAL,    DB_NONE,            100,     1,        1.0,     0,        1.0},
    {32,  "Irony",                      RARE,     0,     SELF,    EFF_SET_ATK,    DB_NONE,            100,     -1,       1.0,     0,        1.0},
    {33,  "Tomato Tomato",              RARE,     50,    SELF,    EFF_SET_ATK,    DB_NONE,            100,     -1,       1.0,     0,        1.0},
    {34,  "Hail Mary",                  RARE,     75,    OPP,     EFF_SET_HP,     DB_NONE,            1,       0,        1.0,     0,        1.0},
    {35,  "Risky Business",             RARE,     93,    BOTH,    EFF_ATK,        DB_NONE,            100,     0,        1.0,     0,        5.0},
    {36,  "JACKPOT!!!!",                RARE,     0,     SELF,    EFF_SPECIAL,    DB_NONE,            7,       0,        1.0,     0,        1.0},
    {37,  "Heavenly Restriction",       RARE,     1,     SELF,    EFF_SPECIAL,    DB_NONE,            100,     0,        1.0,     0,        1.0},
    {41,  "Main Character Syndrome",    SINCARD,  400,   SELF,    EFF_SPECIAL},
    {42,  "Oops, all 3249 leaf clovers!",SINCARD, 400,   SELF,    EFF_SPECIAL},
};
#define ccamt 21
#define rcamt 7
#define scamt 2
#define dcamt 4
#define TOTAL_CARDS (sizeof(cards) / sizeof(cards[0]))

struct CARDDESC carddesc[] = {
    {1, "Common", "Increases the damage of all hits by 5"},
    {2, "Common", "Decreases the enemy's equipped defence card's HP by 25"},
    {3, "Common", "Gain 10%% of your current iron + 10"},
    {4, "Common", "Gain 10%% of your current attack stat + 10"},
    {5, "Common", "Gain 10%% of your current hp + 10"},
    {6, "Common", "50%% chance to make the enemy lose one setup turn"},
    {7, "Common", "50%% chance to make the enemy lose one battle turn"},
    {8, "Common", "Sacrifice 15 hp for 15 attack"},
    {9, "Common", "Gain an additional iron during interest, lose 10%% of your iron"},
    {10, "Common", "25%% chance to gain x1.25 attack"},
    {11, "Common", "Gain a card from your opponent of their choice"},
    {12, "Common", "Gain a turn in both phases"},
    {13, "Common", "Gain 5%% of your opponent's hp"},
    {14, "Common", "Steal 10 iron fron your opponent"},
    {15, "Common", "Increase consecutive rest count by 5"},
    {16, "Common", "Gain 10%% of the opponent's iron"},
    {17, "Common", "Sacrifice one set up turn for x1.75 attack"},
    {18, "Common", "Gain three more iron during interest gains, lose a battle turn"},
    {19, "Common", "Add one stack of iron deficiency to your opponent"},
    {20, "Common", "Gain x3 attack, lose a battle turn"},
    {21, "Common", "Gain a setup turn, lose 25%% of your attack"},
    {22, "Defence", "#22"},
    {23, "Defence", "#23"},
    {24, "Defence", "#24"},
    {25, "Defence", "#25"},
    {31, "Rare", "You are lucky."},
    {32, "Rare", "Totally unintended pun"},
    {33, "Rare", "It's pronounced tomahto not tomayto"},
    {34, "Rare", "KOBE"},
    {35, "Rare", "On one hand, I get x3 attack, but so does my opponent..."},
    {36, "Rare", "get in the car boys we goin to vegas"},
    {37, "Rare", "Just one more thing. Destroy it all for me, okay?"},
    {41, "Sincardnate", "The spotlight is on you."},
    {42, "Sincardnate", "You are REALLY lucky. No really. Like STUPID lucky."},
    {43, "Sincardnate", "Note. This does not actually cause a stack overflow"},
    {44, "Sincardnate", "EXODIA...  OBLITERATE!!!!!!"},
};

int Ran_100(){ //Random generation for all chance mechanics
    return rand() % 100 + 1;
}

//Generate a _____ card (based on the function)
int Ran_Com(){
    return rand() % ccamt;
}
int Ran_Rar(){
    return rand() % rcamt + (ccamt + dcamt); //This makes sure it can only return rare card ids
}
int Ran_Def(){
    return rand() % dcamt + ccamt;
}
int Ran_Sin(){
    //return rand() % lcamt + rcamt + ccamt + dcamt;
}
/*
int FindCardIndex(int card_id) {
    for (int i = 0; i < TOTAL_CARDS; i++) {
        if (cards[i].id == card_id){
            return i;
        }
    }
    return -1;
}
int FindDescIndex(int card_id) {
    for (int i = 0; i < TOTAL_CARDS; i++) {
        if (carddesc[i].id == card_id) {
            return i;
        }
    }
    return -1;
}
*/
void clear(){ //Clear Terminal, purely cosmetic
    printf("\e[1;1H\e[2J");
}
void FULLCARDPRINT(int p, enum MODE mode, struct CARD c[], int ccount){
    printf("%-6s%-40s%-15s%-80s", "ID", "Name", "Type", "Description");
    if (mode == SHOP){
        printf("%-20s", "Iron Supplement Cost");
    }
    printf("\n");
    for (int i = 0; i < ccount; i++){
        printf("%-6i%-40s%-15s%-80s", c[i].id, c[i].name, carddesc[c[i].id - 1].type, carddesc[c[i].id - 1].desc);
        if (mode == SHOP){
            printf("%-20i", c[i].cost);
        }
        printf("\n");
    }
    return;
}

int CHOICECHECK(int p, enum MODE mode, struct CARD c[], int ccount){
    printf("Please enter the ID of the card you'd like to select:\n");
    printf("(Enter 0 if you want to go back)\n");
    int choice;
    scanf("%i", &choice);
    getchar();
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
    clear();
    if (choice == 0){
        printf("Backed out. \n\n");
        return -2;
    }
    else if (choice < 1 || choice > TOTAL_CARDS){
        printf("How do you miss the mark that much dude, that isn't even a card in the game \n\n");
        return -1;
    }
    else if (!douownthis && mode != SHOP){
        printf("You don't own this. Please pick a card that you actually own.\n\n");
        return -1;
    }
    else if (thiswasntoffered == 0){
        printf("This card isn't in the collection shown dumdum \n\n");
        return -1;
    }
    else if (mode == SHOP && player[p].iron < cards[choice - 1].cost){
        printf("Too broke for this lol, pick a card that you can afford \n\n");
        return -1;
    }
    else{
        return choice;
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
        return -1;
    }
    else if (choice == 'y'){
        printf("ok\n\n");
        float dmg = player[p].atk;
        struct CARD def = player[opp].equipdef;
        if (def.id != 0){
            float def_dmg = dmg;
            float initial_dmg = dmg;
            dmg -= def.db_addvalue;
            dmg *= def.db_multvalue;
            if (dmg <= 0){
                dmg = 0;
            }
            def_dmg = initial_dmg - dmg;
            printf("Your enemy's defence card negated %g damage! \n", def_dmg);
            switch (def.efftype){
                //Only deduct hp if the type of def is HP
                case DEF_HP:
                    player[opp].equipdef.multvalue -= def_dmg;
                    printf("It is at %g hp.", player[opp].equipdef.multvalue);
                    break;
            }
            if (player[opp].equipdef.multvalue <= 0){
                printf("Your enemy's defence card broke! \n\n");
                player[opp].equipdef.id = 0;
            }
        }
        player[opp].hp -= dmg;
        printf("Dealt %g damage! Your enemy now has %g hp. \n\n", dmg, player[opp].hp);
    }
    else{
        printf("Invalid input dummy, im forcing you back to the menu \n\n");
        return -1;
    }
    return 0;
}

//any needed values when using cards
int clovercount = 0;
bool heavenrest = false;
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
    while (true){
        FULLCARDPRINT(p, INV, tempc, player[p].invspace);
        printf("\n");
        useid = CHOICECHECK(p, INV, tempc, player[p].invspace);
        if (useid == -1){
            continue;
        }
        else if(useid == -2){
            return -1;
        }
        else{
            break;
        }
    }
    useid--; //0 indexing
    int p_useid;
    for (int i = 0; i < player[p].invspace; i++){
        if (cards[useid].id == player[p].cardinv[i].id){
            p_useid = i;
            break;
        }
    }
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
    if (100 - cards[useid].procchance < Ran_100()){
        int addvalue = cards[useid].addvalue;
        float multvalue = cards[useid].multvalue + clovercount;
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
                case EFF_STEAL_HP:
                    player[targets[i]].hp += addvalue;
                    player[targets[i]].hp *= multvalue;
                    player[targets[1-i]].hp -= addvalue;
                    player[targets[1-i]].hp *= multvalue;
                    break;
                case EFF_STEAL_IRON:
                    player[targets[i]].iron += addvalue;
                    player[targets[i]].iron *= multvalue;
                    player[targets[1-i]].iron -= addvalue;
                    player[targets[1-i]].iron *= multvalue;
                    break;
                case EFF_STEAL_ATK:
                    player[targets[i]].atk += addvalue;
                    player[targets[i]].atk *= multvalue;
                    player[targets[1-i]].atk -= addvalue;
                    player[targets[1-i]].atk *= multvalue;
                    break;
                case EFF_SET_ATK:
                    player[targets[i]].atk = addvalue;
                    break;
                case EFF_BATTLETURN:
                    player[targets[i]].turns[0] += addvalue;
                    break;
                case EFF_SETUPTURN:
                    player[targets[i]].turns[0] += addvalue;
                    break;
                case EFF_PERMBTURNS:
                    player[targets[i]].turns[0] += addvalue;
                    player[targets[i]].turns_def[0] += addvalue;
                    break;
                case EFF_PERMSTURNS:
                    player[targets[i]].turns[0] += addvalue;
                    player[targets[i]].turns_def[0] += addvalue;
                    break;
                case EFF_RESTCOUNT:
                    player[targets[i]].consecutive_rest += addvalue;
                    break;
                case EFF_SPECIAL:
                    switch (useid){
                        case 35:
                            player[p].iron *= 7;
                            player[p].atk *= 7;
                            player[p].hp += 777;
                            printf("You just hit the JACKPOT!!! \n");
                            break;
                        case 36:
                            player[p].atk *= 10;
                            player[p].iron = 0;
                            heavenrest = true;
                            break;
                        default:
                            break;
                    }
                    break;
                case DEF_HP:
                case DEF_TURNS:
                case DEF_SPECIAL:
                    struct CARD empty;
                    player[p].equipdef = cards[useid];
                    for (int idx = 0; idx < player[p].invspace; idx++){
                        if (player[p].equipdef.id - 1 == player[p].cardinv[idx].id){
                            player[p].cardinv[idx] = empty;
                        }
                    }
                    printf("Equipped %s \n", cards[useid].name);
                    printf("Card's effect: %s \n", carddesc[useid].desc);
                    return 0;
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
        player[p].invspace--;
        player[p].cardinv[p_useid].id = 0;
    }
    else {
        printf("Your card failed the performance check lol \n\n");
        player[p].cardinv[p_useid].id = 0;
    }
    return 0;
}

bool shopgend = false;
struct CARD SHOPGEN(int i, int p, int ccount, int scardc, int rcardc, int ccardc, struct CARD tempc[]){
    int ran_g = 0;
    enum CARDTYPE gentype;
    if (i >= 6){
        gentype = DEF;
    }
    else {
        gentype = COMMON;
    }
    int pool_start = 0;
    int pool_size = 0;
    switch (gentype){
        case DEF:
            pool_start = ccamt; // Defense cards start right after common cards
            pool_size = dcamt;
            break;
        case SINCARD:
    }
    int owned_count = 0;
    for (int idx = pool_start; idx < pool_start + pool_size; idx++) {
        // Check if player owns this card
        for (int b = 0; b < 10; b++) {
            if (player[p].cardinv[b].id == cards[idx].id) {
                owned_count++;
                break;
            }
        }
    }
    // If the player owns every single card in this category, break out early
    if (owned_count >= pool_size) {
        //Any card that can't generate is replaced with a common
        gentype = COMMON;
    }
    bool is_duplicate = true;
    while (is_duplicate){
        switch (gentype){
            //Generate card based on the rarity
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
        //Check for duplicates in shop pool
        is_duplicate = false;
        for (int a = 0; a < i; a++) {
            if (tempc[a].id == cards[ran_g].id) {
                is_duplicate = true; //Stop everything
                break; 
            }
        }
        //Make sure the card isn't in your inventory
        if (!is_duplicate) {
            for (int b = 0; b < 10; b++){
                if (player[p].cardinv[b].id == cards[ran_g].id){
                    is_duplicate = true;
                    break;
                }
            }
        }
    }
    tempc[i] = cards[ran_g];
}
int SHOPf(int p){
    clear();
    printf("=---------Welcome to the SHOP!----------=\n\n");
    int ccardc = 100, rcardc = 0, scardc = rcardc/10; //Chance for each card to appear
    int ccount = 8; //How many cards to generate
    int i = 0; 
    struct CARD tempc[ccount];
    while (i < ccount){
        SHOPGEN(i, p, ccount, scardc, rcardc, ccardc, tempc);
        i++;
    }
    shopgend = true; //The shop has generated, don't generate anymore
    int choice = 0;
    while (true){
        printf("You have %i iron supplements at the moment\n\n", player[p].iron);
        printf("Here are your options: \n");
        FULLCARDPRINT(p, SHOP, tempc, ccount);
        printf("\n");
        choice = CHOICECHECK(p, SHOP, tempc, ccount);
        switch (choice){
            case -1:
                break;
            case -2:
                return -1;
            default:
                for (int i = 0; i < 10; i++) { // Always check all 10 slots
                    if (player[p].cardinv[i].id == 0) { // Found a physically empty slot!
                        choice--; //0 indexing
                        player[p].cardinv[i] = cards[choice]; // Save the card to the empty slot
                        player[p].iron -= cards[choice].cost;  // Pay the iron cost
                        player[p].invspace++;                 // Increment total tracked cards
                        
                        printf("Bought %s!\n\n", cards[choice].name);
                        printf("Card's effect: %s \n\n", carddesc[choice].desc);
                        return 0; // Exit the shop function successfully!
                    }
                }
        }
    }
    return 0;
}
int RESTf(int p){
    printf("This action will make you rest. You are currently at %i consecutive rests. \n", player[p].consecutive_rest);
    printf("Are you sure you'd like to rest: (y/n) \n");
    char choice;
    scanf(" %c", &choice);
    if (choice == 'n'){
        printf("ok \n");
        return -1;
    }
    else if (choice == 'y'){
        printf("You have rested. \n");
        int irongain = 10 + player[p].consecutive_rest * 2;
        float hpgain = player[p].hp * 0.05;
        printf("Gained %i iron and %g hp", irongain, hpgain);
        return 0;
    }
    else {
        printf("dude i gave you a clear choice \n");
        return -1;
    }
}
void Turn_End(int *p, int *opp){
    player[*p].turns[player[*p].current_phase]--;
    if (player[*p].turns[player[*p].current_phase] <= 0) {
        switch (player[*p].current_phase){
            case SETUP:
                player[*p].current_phase = BATTLE;
                break;
            case BATTLE:
                player[*p].current_phase = SETUP;
                int irongain = player[*p].ironinterest * (player[*p].iron / 5); //Calculate interest
                player[*p].iron += irongain; //Calculate interest
                printf("Since a cycle has passed, you've levelled up\n");
                if (player[*p].equipdef.efftype == DEF_TURNS && player[*p].equipdef.id != 0){
                    player[*p].equipdef.addvalue--;
                    if (player[*p].equipdef.addvalue <= 0){
                        printf("Your defence card expired.\n\n");
                        player[*p].equipdef.id = 0;
                    }
                }
                break;
        }
        //Reset counter back to however many needed
        player[*p].turns[player[*p].current_phase] = player[*p].turns_def[player[*p].current_phase];    
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
                break;
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
    cards[22].multvalue = player[p].hp * 0.07;
    cards[23].multvalue = player[p].hp * 0.3;
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
    printf("Player 1, Enter your name (please don't use spaces): ");
    char newname[50];
    scanf("%s", newname);
    strcpy(player[p].name, newname);
    printf("Player 2, Enter your name (please don't use spaces): ");
    scanf("%s", newname);
    strcpy(player[opp].name, newname);
    clear();
    enum PHASE curphase;
    printf("Press enter to continue ");
    while (player[p].hp > 0 || player[opp].hp > 0){
        getchar();
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
        printf("6. Pass\n\n");
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
                    continue;
                }
                else if (ATKf(p, opp) == -1){
                    continue;
                }
                changeturn = true;
                player[p].iron += 10;
                break;
            case 2:
                if (player[p].invspace == 0){
                    printf("You don't have any cards to use idiot.\n\n");
                    continue;
                }
                else if (USECARDf(p, opp) == -1){
                    continue;
                }
                continue;
            case 3:
                if (player[p].invspace == 0){
                    printf("You don't have any cards to sell idiot.\n\n");
                    continue;
                }
                clear();
                printf("Sell a card in your inventory\n");
                break;
            case 4:
                if (player[p].invspace >= 10){
                    printf("You don't have any room to buy more cards\n");
                    continue;
                }
                else if (SHOPf(p) != -1){
                    changeturn = true;
                    break;
                }
                continue;
            case 5:
                printf("Display your stats\n");
                continue;
            case 6:
                printf("Forfeited your turn bruh\n");
                changeturn = true;
                break;
            case 57:
                printf("Debug money increase turn\n");
                player[p].iron += 700;
                continue;
            default:
                printf("How do you miss that much you disgrace to the earth. Your oxygen should be given to someone else. You know what? I'm skipping your turn scum\n\n");
                changeturn = true;
                break;
        }
        cards[15].addvalue = player[opp].iron / 10;  //Prescription's iron gain
        //Defence cards that need %based hp
        cards[22].multvalue = player[p].hp * 0.07;
        cards[23].multvalue = player[p].hp * 0.3;
        if (changeturn){
            Turn_End(&p, &opp);
        }
    }    
    return 0;
}