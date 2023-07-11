




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
//#include <math.h>
#include <stdio.h>
#include <time.h>
#include "character.h"
#include "lyney.h"
#include "calc.h"



/* 
 * returns crt dmg multiplier if crit <= ayato.crate  (i.e. crit is inside the
 * range)*/
double crit_hit(double crit, character_t *ayato);



int main(void){
	srand((unsigned) time(NULL));
	weapon_t *weapon = character_weapon_create("the_first_great_magic", 608, 0.662);
	
	if (weapon == NULL){
		fprintf(stderr, "error at creating the instance of weapon\n");
		goto exit_error;
	}
	
	//
	// main stats arbitrary increases
	double pyro_res = 0.25, atk_rolls = 0.105 + 0.933, bennet_buff = (178+620)*1.12;	// 80/90, Alley Flash lv 90, Burst lv 12, pre c6
	double crit_dmg_rolls = 0.148 + 0.21 + 0.14 + 0.204;
	double crit_rate_rolls = 0.074 + 0.105 + 0.060 + 0.039; 
/*
 *	+ ---------- +
 *	| Lavawalker |
 *	+ ---------- +
*/
	char *lw = "LW"; 
	character_t *lyney_lw = character_create("Lyney", LYNEY_BA_90);
	if (lyney_lw == NULL){
		printf("error at creating instance of lyney_lw (malloc)\n");
		goto free_weapon;
	}
	//
	//setup
	character_setup(lyney_lw, weapon, lw, CRIT_RATE);
	character_add_substats(lyney_lw, bennet_buff, pyro_res + atk_rolls, crit_rate_rolls, crit_dmg_rolls, 0);
	character_print_stats(lyney_lw);

	double total_CA_dmg_lw = dpCA_string(lyney_lw, "LW", 3);
	printf("\nTotal dmg (Lavawalker) = " BLU "%.f\n\n" RESET, total_CA_dmg_lw);
/* 
 *	+ --------- +
 *	| Shimenawa |
 *	+ --------- +
 */	
	char *shim = "SR";
	character_t *lyney_sr = character_create("Lyney", LYNEY_BA_90);	//repeated bruh, must refactor
	if (lyney_sr == NULL){
		printf("error at creating instance of lyney_sr (malloc)\n");
		goto free_lw;
	}

	character_setup(lyney_sr, weapon, shim, CRIT_RATE);
	character_add_substats(lyney_sr, bennet_buff, pyro_res + atk_rolls, crit_rate_rolls, crit_dmg_rolls, 0);
	character_print_stats(lyney_sr);

	double total_CA_dmg_sr = dpCA_string(lyney_sr, "SR", 3);
	printf("\nTotal dmg (Shimenawa) = " RED "%.f\n\n" RESET, total_CA_dmg_sr);

	goto success;

free_lw:
	free(lyney_lw);
free_weapon:
	free(weapon);
exit_error:
	return 1;
	
success:
	return 0;
}





/*
double crit_hit(double rgn, character_t *ch){
	return (rgn <= ch->crate) ? ch->cdmg : 0;
}
*/

