/*
 * =====================================================================================
 *
 *       Filename:  lyney.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  04/01/23 00:12:50
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Luciano Jadur 
 *   Organization:  Fontaine :p
 *
 * =====================================================================================
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "character.h"
#include "lyney.h"
#include "calc.h"


static void build_and_show(character_t *ch, weapon_t *weapon, artifacts_t set, circlet_t goblet);
static unsigned int run(weapon_t *weapon, artifacts_t set, circlet_t circlet);


// main stats arbitrary increases
const double pyro_res = 0.25, atk_rolls = 0.105 + 0.093, bennet_buff = (178+620)*1.12;	// 80/90, Alley Flash lv 90, Burst lv 12, pre c6
const double crit_dmg_rolls = 0.148 + 0.21 + 0.14 + 0.204;
const double crit_rate_rolls = 0.074 + 0.105 + 0.039; 


int main(void){
	int ret = 1;
	unsigned int total_dpr = 0;

	srand((unsigned) time(NULL));

	weapon_t *signature = character_weapon_create("the_first_great_magic", 608, 0.662);
	if (signature == NULL){
		fprintf(stderr, "error at creating the instance of weapon\n");
		goto free_sig;
	}
	
	weapon_t *polar = character_weapon_create("polar_star", 608, 0.331);
	if (signature == NULL){
		fprintf(stderr, "error at creating the instance of weapon\n");
		goto free_polar;
	}

	weapon_t *harp = character_weapon_create("skyward_harp", 674, 0.221);
	if (signature == NULL){
		fprintf(stderr, "error at creating the instance of weapon\n");
		goto free_harp;
	}

	weapon_t *pulse = character_weapon_create("thundering_pulse", 608, 0.662);
	if (signature == NULL){
		fprintf(stderr, "error at creating the instance of weapon\n");
		goto free_pulse;
	}

	weapon_t *aqua = character_weapon_create("aqua_simulacra", 542, 0.882);
	if (signature == NULL){
		fprintf(stderr, "error at creating the instance of weapon\n");
		goto free_aqua;
	}
	
/*
 *	+ ---------- +
 *	| Lavawalker |
 *	+ ---------- +
*/
	total_dpr = run(signature, LAVAWALKER, CRIT_RATE); 
	printf("\nTotal dmg (Lavawalker) = " BLU "%u\n-------------------\n\n" RESET, total_dpr);
/* 
 *	+ --------- +
 *	| Shimenawa |
 *	+ --------- +
 */	
	total_dpr = run(signature, SHIMENAWA, CRIT_RATE); 
	printf("\nTotal dmg (Shimenawa) = " RED "%u\n-------------------\n\n" RESET, total_dpr);
/* 
 *	+ ----------------- +
 *	| Wanderer's Troupe |
 *	+ ----------------- +
 */	
	total_dpr = run(signature, WANDERER_TROUPE, CRIT_RATE); 
	printf("\nTotal dmg (Wanderer's Troupe) = " RED "%u\n-------------------\n\n" RESET, total_dpr);

	ret = 0;
	goto success;

/*
 * END 
 */

free_aqua:
	character_weapon_destroy(aqua);
free_pulse:
	character_weapon_destroy(pulse);
free_harp:
	character_weapon_destroy(harp);
free_polar:
	character_weapon_destroy(polar);
free_sig:
	character_weapon_destroy(signature);

success:
	return ret;
}


static
void build_and_show(character_t *ch, weapon_t *weapon, artifacts_t set, circlet_t goblet){
	character_setup(ch, weapon, set, goblet);
	character_add_substats(ch, bennet_buff, pyro_res + atk_rolls, crit_rate_rolls, crit_dmg_rolls, 0);
	character_print_stats(ch);
}


static
unsigned int run(weapon_t *weapon, artifacts_t set, circlet_t circlet){
	character_t *lyney = character_create("Lyney", LYNEY_BA_90);	//repeated bruh, must refactor
	if (lyney == NULL)
		return -1;
	
	build_and_show(lyney, weapon, set, circlet);
	int dmg = 0;
	for (int i = 0; i < 100; i++){
		dmg += dpr(lyney);
	}
	free(lyney);

	return dmg/100;
}




/*
double crit_hit(double rgn, character_t *ch){
	return (rgn <= ch->crate) ? ch->cdmg : 0;
}
*/

