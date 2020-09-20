/* -*- mode: c; -*- */

/* ========================================================================= */

#include "pokedex.h"
#include "pokemon.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "util/test_util.h"
#include "data/test/roster.h"


/* ------------------------------------------------------------------------- */

const base_move_t vine_whip = {
  .move_id = 214,
  .type    = GRASS,
  .is_fast = true,
  .power   = 5,
  .energy  = 8,
};

const pvp_fast_move_t vine_whip_pvp = {
  vine_whip,
  .turns     = 1
};

const base_move_t power_whip = {
  .move_id = 118,
  .type    = GRASS,
  .is_fast = false,
  .power   = 90,
  .energy  = 50,
};

const pvp_charged_move_t power_whip_pvp = {
  power_whip,
  .buff      = NO_BUFF
};

uint16_t bulb_fast_move_ids[]    = { vine_whip.move_id };
uint16_t bulb_charged_move_ids[] = { power_whip.move_id };

const pdex_mon_t bulbasaur = {
  .dex_number        = 1,
  .name              = "BULBASAUR",
  .form_name         = "NORMAL",
  .family            = 1,
  .types             = GRASS_M | POISON_M,
  .base_stats        = { .attack = 118, .stamina = 128, .defense = 111 },
  .fast_move_ids     = bulb_fast_move_ids,
  .fast_moves_cnt    = 1,
  .charged_move_ids  = bulb_charged_move_ids,
  .charged_moves_cnt = 1,
  .form_idx          = 0,
  .next_form         = NULL,
  .hh_name           = 0,
  .hh_dex_num        = 0
};


/* ------------------------------------------------------------------------- */

/**
 * This "test" is essentially done at compilation time.
 */
  static bool
test_construct_pokemon( void )
{
  base_pokemon_t base_bulby_boi = {
    .pdex_mon = & bulbasaur,
    .level    = 20,
    .ivs      = { 10, 10, 10 }
  };

  roster_pokemon_t bulby_boi = {
    .base             = & base_bulby_boi,
    .fast_move_id     = vine_whip.move_id,
    .charged_move_ids = { power_whip.move_id, 0 }
  };

  roster_t my_roster = {
    .roster_pokemon = & bulby_boi,
    .roster_length  = 1
  };

  return true;
}


/* ------------------------------------------------------------------------- */

  static bool
test_get_cpm_for_level( void )
{
  expect( get_cpm_for_level( 1 ) == CPMS[0] );
  expect( get_cpm_for_level( 39.5 ) == CPMS[77] );
  expect( get_cpm_for_level( 40 ) == CPMS[78] );

  return true;
}


/* ------------------------------------------------------------------------- */

  static bool
test_cstore_base_mon_from_store( void )
{
  store_t cstore = CSTORE;

  base_pokemon_t mon = base_mon_from_store( & cstore, 1, 0, 15.0, 10, 11, 12 );
  expect( mon.pdex_mon != NULL );
  expect( mon.pdex_mon->dex_number == 1 );
  expect( mon.pdex_mon->form_idx == 0 );
  expect( mon.level == 15.0 );
  expect( mon.ivs.attack == 10 );
  expect( mon.ivs.stamina == 11 );
  expect( mon.ivs.defense == 12 );

  return true;
}


/* ------------------------------------------------------------------------- */

  static bool
test_cstore_roster( void )
{
  store_t cstore = CSTORE;

  roster_pokemon_t bulbasaur = {
    .base = & USER1_BASE_MONS[0],
    .fast_move_id = (uint16_t) USER1_BASE_MONS[0].pdex_mon->fast_move_ids[0],
    .charged_move_ids = {
      (uint16_t) USER1_BASE_MONS[0].pdex_mon->charged_move_ids[0],
      (uint16_t) USER1_BASE_MONS[0].pdex_mon->charged_move_ids[1]
    }
  };

  roster_pokemon_t charmander = {
    .base = & USER1_BASE_MONS[1],
    .fast_move_id = (uint16_t) USER1_BASE_MONS[1].pdex_mon->fast_move_ids[0],
    .charged_move_ids = {
      (uint16_t) USER1_BASE_MONS[1].pdex_mon->charged_move_ids[0],
      (uint16_t) USER1_BASE_MONS[1].pdex_mon->charged_move_ids[1]
    }
  };

  roster_pokemon_t squirtle = {
    .base = & USER1_BASE_MONS[2],
    .fast_move_id = (uint16_t) USER1_BASE_MONS[2].pdex_mon->fast_move_ids[0],
    .charged_move_ids = {
      (uint16_t) USER1_BASE_MONS[2].pdex_mon->charged_move_ids[0],
      (uint16_t) USER1_BASE_MONS[2].pdex_mon->charged_move_ids[1]
    }
  };

  roster_pokemon_t user1_rmons[] = { bulbasaur, charmander, squirtle };
  roster_t user1_roster = { .roster_pokemon = user1_rmons, .roster_length = 3 };

  expect( strcmp( user1_roster.roster_pokemon[0].base->pdex_mon->name,
                  "BULBASAUR"
                ) == 0
        );

  return true;
}

/* ------------------------------------------------------------------------- */

  bool
test_pokemon( void )
{
  bool rsl = true;
  CS_init();
  rsl &= do_test( construct_pokemon );
  rsl &= do_test( get_cpm_for_level );
  rsl &= do_test( cstore_roster );
  rsl &= do_test( cstore_base_mon_from_store );
  CS_free();
  return rsl;
}


/* ------------------------------------------------------------------------- */



/* ========================================================================= */

/* vim: set filetype=c : */
