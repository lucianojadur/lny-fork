#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "character.h"
#include "lyney.h"  //should be the other way around with parametric data, sigh

struct character {
	int base_atk;
	int atk;
	double hp;
	int em;
	double dmg;
	double crate;
	double cdmg;
	double conditional_dmg[3];		// 0: CA dmg, 1: Skill dmg, 2: Burst dmg
};

struct weapon {
	char *name;
	int base_atk;
	double main_stat;
};


static void weapon_add_stats(character_t *ch, weapon_t *weapon);
static void set_add_stats(character_t *ch, artifacts_t set);


character_t* character_create(const char *name, int base_atk){
    character_t *new = malloc(sizeof(character_t));
    if (new == NULL)
        return NULL;

    new->base_atk = base_atk;;
    return new;
}

weapon_t* character_weapon_create(const char *name, int base_atk, double main_stat){
    weapon_t *new = malloc(sizeof(weapon_t));
	if (new == NULL)
    	return NULL;

	new->name = malloc(strlen(name) + 1);
	if (new->name == NULL){
		free(new);
		return NULL;
	}
	strcpy(new->name, name);
    new->base_atk = base_atk;
    new->main_stat = main_stat;
    return new;
}

int character_atk(character_t *ch){
    return ch->atk;
}

double character_dmg(character_t *ch){
    return ch->dmg;
}

double character_CA_dmg(character_t *ch){
    return ch->conditional_dmg[0];
}

double character_E_dmg(character_t *ch){
    return ch->conditional_dmg[1];
}

double character_Q_dmg(character_t *ch){
    return ch->conditional_dmg[2];
}

double crit_rate(character_t *ch){
    return ch->crate;
}

double crit_dmg(character_t *ch){     //should've done with python or fuck the design patterns out
    return ch->cdmg;
}

void character_weapon_destroy(weapon_t *w){
	free(w->name);
	free(w);
}



void character_setup(character_t *ch, weapon_t *weapon, artifacts_t set, circlet_t circlet){
	if (!ch){
		printf("character is null\n");
		return;
	}
	ch->base_atk += weapon->base_atk;
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
	weapon_add_stats(ch, weapon);
	set_add_stats(ch, set);
}

static
void weapon_add_stats(character_t *ch, weapon_t *weapon){
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

static
void set_add_stats(character_t *ch, artifacts_t  set){
	switch(set){
		case LAVAWALKER:
			ch->dmg += 0.35;
			break;
		case SHIMENAWA:
			ch->conditional_dmg[0] += 0.5;
			ch->atk += ch->base_atk * 0.18;
			break;
		case WANDERER_TROUPE:
			ch->em += 80;
			ch->conditional_dmg[0] += 0.35;
			break;
		case MAREC_HUNTER:
			break;
		case ATK_ATK:
			ch->atk += ch->base_atk * 0.36;
			break;
	}
	//TODO: agregar sets
}

void character_add_substats(character_t *ch, int flat_atk, double atk, double crit_rate, double crit_dmg, double em){
	ch->atk += ch->base_atk*atk + flat_atk;
	ch->crate += crit_rate;
	ch->cdmg += crit_dmg;
	ch->em +=em;
}

void character_print_stats(character_t *ch){
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