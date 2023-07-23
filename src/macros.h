#ifndef MACROS_H
#define MACROS_H


#define CRIT_HIT(rgn, character)   ((rgn) <= crit_rate(character) ? crit_dmg(character) : 0 )

#define HASH_DMG(value) ((value)/1000)
#define DEHASH_DMG(value) ((value)*1000)


#define RED 	"\033[31;0;0m"
#define BLU 	"\033[34;0;0m"
#define RESET	"\033[0m"

typedef enum {CRIT_RATE, CRIT_DMG} circlet_t;

typedef enum {
    LAVAWALKER,
    SHIMENAWA,
    WANDERER_TROUPE,
    MAREC_HUNTER,
    ATK_ATK
} artifacts_t;



#endif