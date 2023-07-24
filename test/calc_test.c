#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "testlib.h"
#include "../src/calc.h"
#include "../src/macros.h"
#include "../src/character.h"
#include "../src/lyney.h"


static void test01_CA_hits_dmg_get_different_crit_rgn();

static int setup(character_t **c);


character_t *lyney;


int main(void){
    if (setup(&lyney) != 0){
		fprintf(stderr, "Error creating the character\n");
		return 1;
	}
//	character_print_stats(lyney, stdout);
   	srand((unsigned) time(NULL));
    
	run_test(test01_CA_hits_dmg_get_different_crit_rgn);

	free(lyney);
	return 0;
}



static
void test01_CA_hits_dmg_get_different_crit_rgn(){
	
	bool not_same = false;
	double prop_dmg[] = {0,0,0,0,0,0,0,0,0,0};
	double strike_dmg[] = {0,0,0,0,0,0,0,0,0,0};
    size_t i = 0;
	//
	// dp_CA_string() loop emulation
	while (!not_same && i < 10){
		prop_dmg[i] = dmg(lyney, PROP_ARROW_9, 0, CA_DMG_BUFF);		//CA #i			
		strike_dmg[i] = dmg(lyney, PYROTH_STRIKE_9, 0, CA_DMG_BUFF);//Hat explosion #i
        if (i > 0){
       		not_same = ((prop_dmg[i] != prop_dmg[i-1])				// if one and only one differs from previous value
            	    ^ (strike_dmg[i] != strike_dmg [i-1])); 		// then their crits are independent
        }
        i++;
	}

	ASSERT_TRUE("TEST 01: different CAs hits (different multipliers) have their own rgn for crit", not_same);	
}


static
int setup(character_t **c){
	*c = character_create("Lyney", LYNEY_BA_90);
	if (c == NULL)
		return -1;

	weapon_t *w = character_weapon_create("the_first_great_magic", 608, 0.662);
	if (w == NULL){
		fprintf(stderr, "error at creating the instance of weapon\n");
		free (w);
		free (*c);
		return -1;
	}
	character_setup(*c, w, LAVAWALKER, CRIT_RATE);       // + 31.1 CR
	character_add_substats(*c, 100, 0.5, 0.0, 100.0, 0); // around 50% crit rate

	return 0;
}