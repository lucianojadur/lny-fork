
#include <time.h>
#include "calc.h"


typedef double (*getter_t)(character_t*);    //pointer to character_t method

static const getter_t conditional_buffs[] = {	// all this bs bc of repecting encapsuling brrruhhhh
	[CA_DMG_BUFF] = character_CA_dmg,
	[SKILL_DMG_BUFF] = character_E_dmg,
	[BURST_DMG_BUFF] = character_Q_dmg
};

static
double dmg(character_t* ch, double multiplier, double add_base_bonus, conditional_dmg_t DMG_BUFF_TAG);



double dpCA_string(character_t *ch, char* set, size_t instances){
	double total_dmg = 0;
	for (size_t i = 0; i < instances; i++){
		total_dmg += dmg(ch, PROP_ARROW_9, 0, CA_DMG_BUFF);			//CA #i					0s
		total_dmg += dmg(ch, PYROTH_STRIKE_9, 0, CA_DMG_BUFF);		//Hat explosion #i		2.0s
	}
	
	return total_dmg;
}

double dpe(character_t *ch, char* set, int stacks){
	return (dmg(ch, SKILL_BONUS_9, character_atk(ch) * SKILL_BONUS_9 * stacks, 1));
}

static
double dmg(character_t* ch, double multiplier, double add_base_bonus, conditional_dmg_t DMG_BUFF_TAG){
	double crit = (double)rand()/(double)(RAND_MAX);
	return (character_atk(ch) * multiplier + add_base_bonus)				//base hit dmg
			* (1 + character_dmg(ch) + conditional_buffs[DMG_BUFF_TAG](ch))	//dmg bonus% + conditional dmg bonus method
			* (1 + CRIT_HIT(crit, ch)) 										//crit
			* (1 + 0.15)			 										//enemy elemental resistance
			* 0.5;															//enemy def
//	printf("Hit n°. %ld:\t%f\t", n, hit);
//	printf(crit <= ch->crate ? "(crit)\n" : "\n");
}