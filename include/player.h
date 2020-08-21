/* -*- mode: c; -*- */

#ifndef _PLAYER_H
#define _PLAYER_H

/* ========================================================================= */

#include <stdint.h>
#include <stdbool.h>
#include "util/bits.h"
#include "pokemon.h"


/* ------------------------------------------------------------------------ */

typedef struct {
  roster_t roster;
  uint16_t wins;
  uint16_t battles;
} player_t;


uint16_t SWITCH_TIME = 60000;


typedef enum packed {
  USER_CONTROLLED, NOVICE, RIVAL, ELITE, CHAMPION
} ai_level_t;


typedef enum packed {
  DEFAULT_STRAT, SHIELD_STRAT, SWITCH_BASIC, SWITCH_FARM, SWITCH_ADVANCED,
  FARM_ENERGY, OVERFARM, BAIT_SHIELDS, WAIT_CLOCK, PRESERVE_SWITCH_ADVANTAGE,
  ADVANCED_SHIELDING, BAD_DECISION_PROTECTION, SACRIFICIAL_SWAP
} strategy_t;

const uint8_t NUM_STRATS = SACRIFICIAL_SWAP + 1;

typedef uint16_t strat_mask_t;
#define get_strat_mask( strat )  ( (strat_mask_t) to_mask( ( strat ) ) )

const strat_mask_t DEFAULT_STRAT_M    = get_strat_mask( DEFAULT_STRAT );
const strat_mask_t SHIELD_STRAT_M     = get_strat_mask( SHIELD_STRAT );
const strat_mask_t SWITCH_BASIC_M     = get_strat_mask( SWITCH_BASIC );
const strat_mask_t SWITCH_FARM_M      = get_strat_mask( SWITCH_FARM );
const strat_mask_t SWITCH_ADVANCED_M  = get_strat_mask( SWITCH_ADVANCED );
const strat_mask_t FARM_ENERGY_M      = get_strat_mask( FARM_ENERGY );
const strat_mask_t OVERFARM_M         = get_strat_mask( OVERFARM );
const strat_mask_t BAIT_SHIELDS_M     = get_strat_mask( BAIT_SHIELDS );
const strat_mask_t WAIT_CLOCK_M       = get_strat_mask( WAIT_CLOCK );
const strat_mask_t SACRIFICIAL_SWAP_M = get_strat_mask( SACRIFICIAL_SWAP );
const strat_mask_t
PRESERVE_SWITCH_ADVANTAGE_M = get_strat_mask( PRESERVE_SWITCH_ADVANTAGE );
const strat_mask_t
ADVANCED_SHIELDING_M        = get_strat_mask( ADVANCED_SHIELDING );
const strat_mask_t
BAD_DECISION_PROTECTION_M   = get_strat_mask( BAD_DECISION_PROTECTION );


/**
 * Don't even try to compile this without GCC.
 * The ordering of Bitfields is NOT guaranteed by a lot of compilers.
 * GCC is love, GCC is life.
 */
typedef union {
  strat_mask_t mask;
  struct {
    uint8_t _unused                   : 4;
    bool    sacrificial_swap          : 1;
    bool    bad_decision_protection   : 1;
    bool    advanced_shielding        : 1;
    bool    preserve_switch_advantage : 1;
    bool    wait_clock                : 1;
    bool    bait_shileds              : 1;
    bool    overfarm                  : 1;
    bool    farm_energy               : 1;
    bool    switch_advanced           : 1;
    bool    switch_farm               : 1;
    bool    switch_basic              : 1;
    bool    shield                    : 1;
  } flags;
} strategies_t transparent;


typedef struct {
  bool         two_charged_moves     : 1;
  uint16_t     iv_combo_range        : 15;  /*   200-3000  */
  uint8_t      energy_guess_accuracy : 4;   /*    0-15     */
  uint8_t      reaction_time         : 4;   /* 0, 4, 8, 12 */
  uint8_t      move_guess_certainty  : 2;   /*    0-3      */
  strategies_t strategies;
} ai_t;


const ai_t AI_ARCHETYPES[] = {
  /* User Controlled */ {
    .two_charged_moves     = true,
    .iv_combo_range        = 0,
    .energy_guess_accuracy = 0,
    .reaction_time         = 0,
    .move_guess_certainty  = 0,
    .strategies            = DEFAULT_STRAT_M
  }, /* Novice */ {
    .two_charged_moves     = false,
    .iv_combo_range        = 3000,
    .energy_guess_accuracy = 15,
    .reaction_time         = 12,
    .move_guess_certainty  = 0,
    .strategies            = SHIELD_STRAT_M
  }, /* Rival */ {
    .two_charged_moves     = true,
    .iv_combo_range        = 2000,
    .energy_guess_accuracy = 10,
    .reaction_time         = 8,
    .move_guess_certainty  = 1,
    .strategies            = SHIELD_STRAT_M | SWITCH_BASIC_M
  }, /* Elite */ {
    .two_charged_moves     = true,
    .iv_combo_range        = 1000,
    .energy_guess_accuracy = 5,
    .reaction_time         = 4,
    .move_guess_certainty  = 2,
    .strategies            = SHIELD_STRAT_M | SWITCH_BASIC_M | FARM_ENERGY_M |
                             BAIT_SHIELDS_M
  }, /* Champion */ {
    .two_charged_moves     = true,
    .iv_combo_range        = 200,
    .energy_guess_accuracy = 0,
    .reaction_time         = 0,
    .move_guess_certainty  = 3,
    .strategies            = SHIELD_STRAT_M | SWITCH_BASIC_M | FARM_ENERGY_M |
                             OVERFARM_M | BAIT_SHIELDS_M | WAIT_CLOCK_M |
                             PRESERVE_SWITCH_ADVANTAGE_M |
                             ADVANCED_SHIELDING_M |
                             BAD_DECISION_PROTECTION_M | SACRIFICIAL_SWAP_M
  }
};


typedef struct {
  pvp_team_t team;
  ai_level_t ai_level;
  uint8_t    active_pokemon : 2;   /*    0-2   */
  uint8_t    shields        : 2;   /*    0-2   */
  uint16_t   switch_timer;         /* 0-60,000 */
} pvp_player_t;


uint8_t
get_remaining_pokemon( pvp_player_t * player ) {
  assert( player != NULL );
  uint8_t rem = 0;
  /* If you wanted to be REAL cool you would unroll this into a one liner... */
  for ( pvp_pokemon_t * p = player->team;
        p < player->team + 3;
        rem += ( ( p != NULL ) && ( p->hp > 0 ) ), p++
      );
  assert( rem <= 3 );
  return rem;
}


bool
use_shield( pvp_player_t * player ) {
  assert( player != NULL );
  if ( player->shields > 0 ) {
    player->shields--;
    return true;
  }
  return false;
}


void
start_switch_timer( pvp_player_t * player ) {
  assert( player != NULL );
  player->switch_timer = SWITCH_TIME;
}


void
decr_switch_timer( pvp_player_t * player, uint16_t delta_time ) {
  assert( player != NULL );
  if ( player->switch_timer > 0 )
    player->switch_timer = max( 0, ( player->switch_timer - delta_time ) );
}


#define get_active_pokemon( PLAYER )                                          \
  ( ( PLAYER ).team[( PLAYER ).active_pokemon] )



/* ========================================================================= */

#endif /* player.h */

/* vim: set filetype=c : */
