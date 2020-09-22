/* -*- mode: c; -*- */

#ifndef _PLAYER_H
#define _PLAYER_H

/* ========================================================================= */

#include "ai/ai.h"
#include "battle.h"
#include "pokemon.h"
#include <stdbool.h>
#include <stdint.h>


/* ------------------------------------------------------------------------- */

struct player_s {
  roster_t roster;
  uint16_t wins;
  uint16_t battles;
} packed;
typedef struct player_s  player_t;


/* ------------------------------------------------------------------------- */

struct pvp_player_s {
  ai_t *          ai;
  struct { pvp_pokemon_t mon0, mon1, mon2; } team;
  uint8_t         active_pokemon : 2;   /* 0-2  */
  uint8_t         shields        : 2;   /* 0-2  */
  uint8_t         switch_turns   : 4;   /* 0-12 */
} packed;
typedef struct pvp_player_s  pvp_player_t;

static const pvp_player_t PVP_PLAYER_NULL = {
  .ai             = NULL,
  .team           = { PVP_MON_NULL, PVP_MON_NULL, PVP_MON_NULL },
  .active_pokemon = 0,
  .shields        = 2,
  .switch_turns   = 0
};


/* ------------------------------------------------------------------------- */

#define get_active_pokemon( PLAYER )                                          \
  ( ( ( PLAYER ).active_pokemon == 0 ) ? ( PLAYER ).team.mon0 :               \
    ( ( PLAYER ).active_pokemon == 1 ) ? ( PLAYER ).team.mon1                 \
                                       : ( PLAYER ).team.mon2                 \
  )


/* ------------------------------------------------------------------------- */

uint8_t get_remaining_pokemon( pvp_player_t * player );
bool    use_shield( pvp_player_t * player );
void    start_switch_timer( pvp_player_t * player );
void    decr_switch_timer( pvp_player_t * player, uint8_t delta_turns );


/* ------------------------------------------------------------------------- */



/* ========================================================================= */

#endif /* player.h */

/* vim: set filetype=c : */
