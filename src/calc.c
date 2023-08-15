
#include <time.h>
#include "calc.h"
#include <stdio.h>

typedef double (*getter_t)(character_t*);    //pointer to character_t method

static const getter_t conditional_buffs[] = {	// all this bs bc of repecting encapsuling brrruhhhh
	[CA_DMG_BUFF] = character_CA_dmg,
	[SKILL_DMG_BUFF] = character_E_dmg,
	[BURST_DMG_BUFF] = character_Q_dmg
};

//static
//double dmg(character_t* ch, double multiplier, double add_base_bonus, conditional_dmg_t DMG_BUFF_TAG);


unsigned int dpr(character_t *ch){
	int stacks = 2;
	double ca_total_dmg = dpCA_string(ch, stacks);	//  2 stacks
	double burst_total_dmg = dpq(ch, &stacks);		// +1 stack
	ca_total_dmg += dpCA_string(ch, 1);				// +1 stack
 	double skill_total_dmg = dpe(ch, ++stacks);		// -4 stacks
	
	return ca_total_dmg + skill_total_dmg + burst_total_dmg;
}

double dpCA_string(character_t *ch, size_t instances){
	double total_dmg = 0;
	for (size_t i = 0; i < instances; i++){
		total_dmg += dmg(ch, PROP_ARROW_9, character_atk(ch) * 0.8, CA_DMG_BUFF);			//CA #i					0s
		total_dmg += dmg(ch, PYROTH_STRIKE_9, character_atk(ch) * 0.8, CA_DMG_BUFF);		//Hat explosion #i		2.0s
	}
	
	return total_dmg;
}


double dpe(character_t *ch, int stacks){
	return (dmg(ch, SKILL_9, (character_atk(ch) * SKILL_BONUS_9 * stacks), SKILL_DMG_BUFF));
}


double dpq(character_t *ch , int *stacks){
	(*stacks)++;
	double first_hit = dmg(ch, BURST_INIT_9, 0, BURST_DMG_BUFF);
	double second_hit = dmg(ch, BURST_FINAL_9, 0, BURST_DMG_BUFF);
	return first_hit + second_hit;
}

//static
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
