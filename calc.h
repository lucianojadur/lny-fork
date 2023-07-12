#ifndef CALC_H
#define CALC_H

#include <stdlib.h>
#include "character.h"
#include "lyney.h"
#include "macros.h"


/*
 * dmg per Elemental skill (total)*/
double dpCA_string(character_t *ch, size_t instances);
/*
 * dmg per Burst (tick at S-T, no quad. scaling)*/
double dpe(character_t *ch, int stacks);
/*
 * dmg per Rotation (Burst total + 1 E total)*/
unsigned int dpr(character_t *ch);
/*
 *  */
double dpq(character_t *ch, int *stacks);
/*
 *  */
//double dmg(character_t *ch, double multiplier, double add_base_bonus, int dmg_buff_index);


#endif