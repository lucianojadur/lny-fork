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
#include "output.h"
#include "macros.h"


static void build_and_show(character_t *ch, weapon_t *weapon, artifacts_t set, circlet_t goblet);
static int run(weapon_t *weapon, artifacts_t set, circlet_t circlet);


const char *(set_name[]) = {
	[LAVAWALKER]		= "lw",
	[SHIMENAWA] 		= "sr",
	[WANDERER_TROUPE] 	= "wt",
	[MAREC_HUNTER]		= "mh",
	[ATK_ATK]			= "atk"
};

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
	printf("[Lavawalker]\n\n");
	total_dpr = run(signature, LAVAWALKER, CRIT_RATE); 
	printf("\nTotal dmg = " BLU "%d\n-------------------\n\n" RESET, total_dpr);
/* 
 *	+ --------- +
 *	| Shimenawa |
 *	+ --------- +
 */	
	printf("[Shimenawa]\n\n");
	total_dpr = run(signature, SHIMENAWA, CRIT_RATE); 
	printf("\nTotal dmg = " RED "%d\n-------------------\n\n" RESET, total_dpr);
/* 
 *	+ ----------------- +
 *	| Wanderer's Troupe |
 *	+ ----------------- +
 */	
	printf("[Wanderer's Troupe]\n\n");
	total_dpr = run(signature, WANDERER_TROUPE, CRIT_RATE); 
	printf("\nTotal dmg = " RED "%d\n-------------------\n\n" RESET, total_dpr);

	ret = 0;

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

	return ret;
}


static
void build_and_show(character_t *ch, weapon_t *weapon, artifacts_t set, circlet_t goblet){
	character_setup(ch, weapon, set, goblet);
	character_add_substats(ch, bennet_buff, pyro_res + atk_rolls, crit_rate_rolls, crit_dmg_rolls, 0);
	character_print_stats(ch);
}


static
int run(weapon_t *weapon, artifacts_t set, circlet_t circlet){
	int ret = -1;
	
	character_t *lyney = character_create("Lyney", LYNEY_BA_90);	//repeated bruh, must refactor
	if (lyney == NULL)
		return ret;
	
	size_t dmg_values = 1000;
	int *values = calloc(dmg_values, sizeof(int));
	if (values  == NULL) goto close_values;

	FILE *fd_out = output("lyney", weapon_name(weapon), set_name[set]);
	if (fd_out == NULL) goto close_fd;

	build_and_show(lyney, weapon, set, circlet);
	int dmg = 0;
	//
	//START SIMULATION
	for (int i = 0; i < 40000; i++){
		int current_dmg = dpr(lyney);
		values[HASH_DMG(current_dmg)]++;		
		dmg += current_dmg/100;		// to not get out of range. avg <current_dmg>/100 = 5500, this times 40k...
	}
	//
	// output file writing 
	for (int i = 0; i < dmg_values; i++)
		output_write_line(fd_out, DEHASH_DMG(i), values[i]);		
	
	ret = dmg/400;	// (dmg * 100) / 40000

close_fd:
	fclose(fd_out);
close_values:
	free(values);
	free(lyney);

	return ret;
}



/*
double crit_hit(double rgn, character_t *ch){
	return (rgn <= ch->crate) ? ch->cdmg : 0;
}
*/

