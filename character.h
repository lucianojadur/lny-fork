#ifndef CHARACTER_H
#define CHARACTER_H

#include "macros.h"

typedef struct character character_t;
typedef struct  weapon weapon_t;

typedef enum {
    CA_DMG_BUFF,
    SKILL_DMG_BUFF,
    BURST_DMG_BUFF
} conditional_dmg_t;

character_t* character_create(const char *name, int base_atk);

weapon_t* character_weapon_create(const char *name, int base_atk, double main_stat);

/*
 * setup */
void character_setup(character_t *ch, weapon_t *weapon, artifacts_t set, circlet_t goblet);

void character_add_substats(character_t *ch, int flat_atk, double atk, double crit_rate, double crit_dmg, double em);

int character_atk(character_t *ch);

double character_dmg(character_t *ch);

double character_CA_dmg(character_t *ch);

double character_E_dmg(character_t *ch);

double character_Q_dmg(character_t *ch);

double crit_rate(character_t *ch);

double crit_dmg(character_t *ch);

void character_weapon_destroy(weapon_t *w);

void character_print_stats(character_t *ch);

#endif