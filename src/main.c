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
#include <string.h>
#include <time.h>
#include "character.h"
#include "lyney.h"
#include "calc.h"
#include "output.h"
#include "macros.h"


static void build_and_show(character_t *ch, weapon_t *weapon, artifacts_t set, circlet_t goblet);
static int run(weapon_t *weapon, artifacts_t set, circlet_t circlet);
static bool cla_check(int argc, const char* argv[], double *cr);

const char *(set_name[]) = {
	[LAVAWALKER]		= "lw",
	[SHIMENAWA] 		= "sr",
	[WANDERER_TROUPE] 	= "wt",
	[MAREC_HUNTER]		= "mh",
	[ATK_ATK]			= "atk"
};


// main stats arbitrary increases
const double PYRO_RES = 0.25, atk_rolls = 0.105 + 0.093, bennet_buff = (178+620)*1.12 + 926*0.2;	// 80/90, Alley Flash lv 90, Burst lv 12, pre c6
const double CRIT_DMG_ROLLS = 0.148 + 0.21 + 0.14 + 0.204;
const double CRIT_RATE_ROLLS = 0.06 + 0.090 + 0.027; 
double cr = CRIT_RATE_ROLLS;

int iterations = 100000;	// default


int main(int argc, const char* argv[]){
	int ret = 1;
	unsigned int total_dpr = 0;
	
	if (!cla_check(argc, argv, &cr)){
		fprintf(stderr, "Usage:\n./run [-cr] [<crit_rate>]\n\nwhere <crit_rate> is a number.\n\n");
		return ret;
	}

	srand((unsigned) time(NULL));
	
	weapon_t *weapon = NULL;

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
	printf("\nAvg DPR = " BLU "%d\n-------------------\n\n" RESET, total_dpr);
/* 
 *	+ --------- +
 *	| Shimenawa |
 *	+ --------- +
 */	
	printf("[Shimenawa]\n\n");
	total_dpr = run(signature, SHIMENAWA, CRIT_RATE); 
	printf("\nAvg DPR = " RED "%d\n-------------------\n\n" RESET, total_dpr);
/* 
 *	+ ----------------- +
 *	| Wanderer's Troupe |
 *	+ ----------------- +
 */	
	printf("[Wanderer's Troupe]\n\n");
	total_dpr = run(signature, WANDERER_TROUPE, CRIT_RATE); 
	printf("\nAvg DPR = " RED "%d\n-------------------\n\n" RESET, total_dpr);
/* 
 *	+ ------------------- +
 *	| Marechaussee Hunter |
 *	+ ------------------- +
 */	
	printf("[Marechaussee Hunter]\n\n");
	total_dpr = run(signature, MAREC_HUNTER, CRIT_DMG); 
	printf("\nAvg DPR = " RED "%d\n-------------------\n\n" RESET, total_dpr);

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
	character_add_substats(ch, bennet_buff, PYRO_RES + atk_rolls, cr, CRIT_DMG_ROLLS, 0, set);
	character_print_stats(ch, stdout);
}


static
int run(weapon_t *weapon, artifacts_t set, circlet_t circlet){
	int ret = -1;
	
	character_t *lyney = character_create(LYNEY_BA_90);	//repeated, must refactor
	if (lyney == NULL)
		return ret;
	
	int *output_values = calloc(1000-OUTPUT_DMG_VALUES_FLOOR, sizeof(int));
	if (output_values  == NULL) goto close_values;

	FILE *fd_out = output("lyney", weapon_name(weapon), set_name[set]);
	if (fd_out == NULL) goto close_fd;

	build_and_show(lyney, weapon, set, circlet);
	int dmg = 0;

	output_write_header(fd_out);
	//
	//START SIMULATION
	for (int i = 0; i < iterations; i++){
		int current_dmg = dpr(lyney);
		output_values[HASH_DMG(current_dmg)]++;		
		dmg += current_dmg/100;			// to not get out of range.
	}						// avg <current_dmg>/100 = 5500, this times 40k...
	//
	// CSV WRITING 
	for (int i = 0; i < 1000-OUTPUT_DMG_VALUES_FLOOR; i++)	// (truncated to +200k dpr)
		output_write_line(fd_out, DEHASH_DMG(i), output_values[i]);		
	
	ret = dmg/(iterations/100);	// (dmg * 100) / #iterations

close_fd:
	fclose(fd_out);
close_values:
	free(output_values);
	free(lyney);

	return ret;
}


static
bool cla_check(int argc, const char* argv[], double *cr){
	if (argc == 1)
		return true;
	else if ((argc != 3 && argc != 5) || 
			(argc == 3 && (strcmp("-cr", argv[1]) && strcmp("-i", argv[1])))
			)
		return false;
	//
	// parse command line arguments
	char *endp;
	for(size_t i = 1; i < argc; i++){
		if(strcmp(argv[i], "-cr") == 0){
			*cr = strtod(argv[i+1], &endp);
			if (argv[i+1] != endp && (*endp == ' ' || *endp == '\0')){
				printf("crit rate = %f\n", *cr);
				*cr /= (*cr > 1) ? 100.0 : 1;
				*cr -= 0.245 + 0.311;	//gonna be added to the base 24.5 cr% in character_add_substats()
			}		
		}
		else if(strcmp(argv[i], "-i") == 0){
			iterations = strtod(argv[i+1], &endp);
			printf("# of iterations = %d\n", (int)iterations);
		}
	}
	return true;
}

