




/*
 * =====================================================================================
 *
 *       Filename:  ayato.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  04/01/23 00:12:50
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Luciano Jadur 
 *   Organization:  Yashiro Commision
 *
 * =====================================================================================
 */
#include <stdlib.h>
//#include <math.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define CRIT_HIT(rgn, character)  ( ((rgn) <= (character)->crate) ? (character)->cdmg/100.0 : 0 )


#define RED 	"\033[31;0;0m"
#define BLU 	"\033[34;0;0m"
#define RESET	"\033[0m"

#define LYNEY_BA_90     318

#define PROP_ARROW_9    2.940
#define PYROTH_STRIKE_9 3.600
#define SPIRITV_THORN_9 0.640

#define SKILL_9         2.214
#define SKILL_BONUS_9	0.83

#define BURST_INIT_9    7.038
#define BURST_FINAL_9   2.346


typedef struct {
	int base_atk;
	int atk;
	double hp;
	int em;
	double dmg;
	double crate;
	double cdmg;
	double conditional_dmg[3];		// 0: CA dmg, 1: Skill dmg, 2: Burst dmg
} character_t;

typedef struct {
	char *name;
	int base_atk;
	double main_stat;
} weapon_t;

typedef enum {CRIT_RATE, CRIT_DMG} goblet_t;


/*
 * setup */
void setup(character_t *ch, weapon_t *weapon, char *set, goblet_t goblet);
/*
* adds weapon's main stat and passive into the stats of the character*/
void weapon_add_stats(weapon_t *weapon, character_t *ch);
/*
 * same as weapon */
void set_add_stats(char *set, character_t *ch);

/*
 * dmg per Elemental skill (total)*/
double dpCA_string(character_t *ch, char* , size_t instances);
/*
 * dmg per Burst (tick at S-T, no quad. scaling)*/
double dpe(character_t *ch, char* set, int stacks);
/*
 * dmg per Rotation (Burst total + 1 E total)*/
double dpr(character_t *ch, char* set);

/*
 *  */
double dmg(character_t *ch, double multiplier, double add_base_bonus, int dmg_buff_index);
/* 
 * returns crt dmg multiplier if crit <= ayato.crate  (i.e. crit is inside the
 * range)*/
double crit_hit(double crit, character_t *ayato);

void print_stats(character_t *ch);


int main(void){
	srand((unsigned) time(NULL));
	weapon_t signature_wp = {"the_first_great_magic", 608, 0.662};
/*
 *	+ ---------- +
 *	| Lavawalker |
 *	+ ---------- +
*/
	char *lw = "LW"; 
	character_t *lyney_lw = malloc(sizeof(character_t));
	if (lyney_lw == NULL){
		printf("error at creating instance of lyney_lw (malloc)\n");
		goto free_lw;
	}
	setup(lyney_lw, &signature_wp, lw, CRIT_RATE);
	print_stats(lyney_lw);
	double total_CA_dmg_lw = dpCA_string(lyney_lw, "LW", 3);
	printf("\nTotal dmg (Lavawalker) = " BLU "%.f\n\n" RESET, total_CA_dmg_lw);
/* 
 *	+ --------- +
 *	| Shimenawa |
 *	+ --------- +
 */	
	char *shim = "SR";
	character_t *lyney_sr = malloc(sizeof(character_t));
	if (lyney_sr == NULL){
		printf("error at creating instance of lyney_sr (malloc)\n");
		goto free_sr;
	}
	setup(lyney_sr, &signature_wp, shim, CRIT_RATE);
	print_stats(lyney_sr);

	double total_CA_dmg_sr = dpCA_string(lyney_sr, "SR", 3);
	printf("\nTotal dmg (Shimenawa) = " RED "%.f\n\n" RESET, total_CA_dmg_sr);
	goto success;

free_sr:
	free(lyney_sr);
free_lw:
	free(lyney_lw);
	return 1;
	
success:
	return 0;
}


double dpCA_string(character_t *ch, char* set, size_t instances){
	double total_dmg = 0;

	for (size_t i = 0; i < instances; i++){
		total_dmg += dmg(ch, PROP_ARROW_9, 0, 0);			//CA #i					0s
		total_dmg += dmg(ch, PYROTH_STRIKE_9, 0, 0);		//Hat explosion #i		2.0s
	}
	
	return total_dmg;
}

double dpe(character_t *ch, char* set, int stacks){
	return (dmg(ch, SKILL_BONUS_9, ch->atk * SKILL_BONUS_9 * stacks, 1));
}


double dmg(character_t* ch, double multiplier, double add_base_bonus, int dmg_buff_index){
	double crit = (double)rand()/(double)(RAND_MAX);
	return (ch->atk * multiplier + add_base_bonus)	//base hit dmg
			* (1 + ch->dmg + ch->conditional_dmg[dmg_buff_index])			//dmg bonus% +
			* (1 + CRIT_HIT(crit, ch)) 				//crit
			* (1 + 0.15)			 				//enemy elemental resistance
			* 0.5;									//enemy def
//	printf("Hit n°. %ld:\t%f\t", n, hit);
//	printf(crit <= ch->crate ? "(crit)\n" : "\n");
}


void setup(character_t *ch, weapon_t *weapon, char *set, goblet_t circlet){
	ch->base_atk = LYNEY_BA_90 + weapon->base_atk;
	ch->atk	= ch->base_atk * 1.46 + 311;
	ch->hp	= 20000;
	ch->em = 0;
	ch->dmg	= 1.46;
	ch->crate = 0.245;
	ch->cdmg = 0.500;
	ch->conditional_dmg [0]= 0.0;
	ch->conditional_dmg [1]= 0.0;
	ch->conditional_dmg [2]= 0.0;

	switch (circlet){
		case CRIT_RATE: ch->crate += 0.311; break;
		case CRIT_DMG: ch->cdmg += 0.611; break;
		default: ch->crate += 0.0; ch->cdmg += 0.0; break;
	}
	weapon_add_stats(weapon, ch);
	set_add_stats(set, ch);
}

void weapon_add_stats(weapon_t *weapon, character_t *ch){
	if (!strcmp(weapon->name, "the_first_great_magic") ){
		ch->atk += ch->base_atk*0.52;
		ch->cdmg += weapon->main_stat;
	}
	else if (!strcmp(weapon->name, "thundering_pulse")){
		ch->atk += ch->base_atk * 0.2;
		ch->cdmg += weapon->main_stat;	
	}
	else if (!strcmp(weapon->name, "polar_star")){
		ch->atk += ch->base_atk * 0.3;				//assumed 3 stacks
		ch->dmg += 0.12;
		ch->crate += weapon->main_stat;	
	}
    else if (!strcmp(weapon->name, "aqua_simulacra")){				
		ch->dmg += 0.2;
		ch->cdmg += weapon->main_stat;
	}
	else if (!strcmp(weapon->name, "skyward_harp")){				
		ch->cdmg += 0.2;
		ch->crate += weapon->main_stat;
	}
	else if (!strcmp(weapon->name, "amos_bow")){				
		ch->conditional_dmg[0] += 0.12 + 0.4;						//assumed 5 stacks
		ch->atk += ch->base_atk * weapon->main_stat;
	}
	else if (!strcmp(weapon->name, "song_of_stillness")){				
		ch->conditional_dmg[0] += 0.32;						//assuming always gets healed and R5
		ch->atk += ch->base_atk * weapon->main_stat;
	}
	else if (!strcmp(weapon->name, "blazing_sun")){				
		ch->conditional_dmg[0] += 0.28;						//assuming that the debuff on the target enemy is permanent (must check) 
		ch->crate += weapon->main_stat;
	}
	else if (!strcmp(weapon->name, "mounn_moon")){				
		ch->conditional_dmg[2] += 0.6;						//R5 with Sucrose, Bennet, Xiangling
		ch->atk += ch->base_atk * weapon->main_stat;
	}
	else if (!strcmp(weapon->name, "blackcliff")){				
		ch->cdmg += weapon->main_stat;
	}

}

void set_add_stats(char *set, character_t *ch){
	if (strcmp(set, "LW") == 0){			//4PC Lavawalker
		ch->dmg += 0.35;
	} else if (strcmp(set, "SR") == 0){	    //4PC Shimenawa
		ch->conditional_dmg[0] += 0.5;
		ch->atk += ch->base_atk * 0.18;
	}
	//TODO: agregar sets
}

double crit_hit(double rgn, character_t *ch){
	return (rgn <= ch->crate) ? ch->cdmg : 0;
}

void print_stats(character_t *ch){
	printf("Base Atk: %d\n"
			"Atk: %d\n"
			"Dmg%%: %.2f\n"
			"CR: %.2f\n"
			"CD: %.2f\n"
			"EM: %d\n"
			"CA dmg%%: %.2f\n"
			"Skill dmg%%: %.2f\n"
			"Burst dmg%%: %.2f\n", ch->base_atk, ch->atk, ch->dmg, ch->crate, ch->cdmg, ch->em,
								ch->conditional_dmg[0], ch->conditional_dmg[1], ch->conditional_dmg[2]);
}