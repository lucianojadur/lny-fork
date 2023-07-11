#ifndef MACROS_H
#define MACROS_H


#define CRIT_HIT(rgn, character)   ((rgn) <= crit_rate(character) ? crit_dmg(character)/100.0 : 0 )

#define RED 	"\033[31;0;0m"
#define BLU 	"\033[34;0;0m"
#define RESET	"\033[0m"

typedef enum {CRIT_RATE, CRIT_DMG} goblet_t;


#endif