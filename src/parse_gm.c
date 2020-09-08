/* -*- mode: c; -*- */

/* ========================================================================= */

#include "parse_gm.h"
#include "ext/jsmn.h"
#include "ext/jsmn_iterator.h"
#include "util/json_util.h"
#include "util/jsmn_iterator_stack.h"
#include "pokedex.h"
#include <stdio.h>
#include <stdlib.h>


/* ------------------------------------------------------------------------- */

  int
main( int argc, char * argv[], char ** envp )
{

  /* Parse file */
  gm_parser_t gparser;
  size_t rsl = init_gm_parser( "./data/GAME_MASTER.json", &gparser );
  assert( rsl != 0 );

#if 1
  int item_template_list_idx = json_find( gparser.buffer,
                                          gparser.tokens,
                                          jsoneq_str_p,
                                          (void *) "itemTemplate",
                                          gparser.tokens_cnt,
                                          0
                                        );
  assert( 0 < item_template_list_idx );
  item_template_list_idx++; /* currently it is the key, we want the value */

  /* We want to iterate over the item template list, then over key/vals  */
  jsmn_iterator_t items_list_iter;
  jsmn_iterator_t item_iter;
  jsmn_iterator_init( &items_list_iter,
                      gparser.tokens,
                      gparser.tokens_cnt,
                      item_template_list_idx
                    );

  /* Compile Regex to match Pokemon templateIds */
  regex_t pkmn_tmp_regex;
  int     rc_rsl = regcomp( &pkmn_tmp_regex, pokemon_template_pat, REG_NOSUB );
  assert( rc_rsl == 0 );

  /* Loop over all item templates looking for pokemon */
  jsmntok_t * item              = NULL;
  jsmntok_t * key               = NULL;
  jsmntok_t * val               = NULL;
  uint32_t    hint              = 0;
  int         first_pokemon_idx = 0;

  while( 0 < jsmn_iterator_next( &items_list_iter, NULL, &item, hint ) )
    {
      jsmn_iterator_init( &item_iter,
                          gparser.tokens,
                          gparser.tokens_cnt,
                          jsmn_iterator_position( &items_list_iter )
                        );

      int i = jsmn_iterator_find_next( gparser.buffer,
                                       &item_iter,
                                       &key,
                                       jsoneq_str_p,
                                       (void *) "templateId",
                                       &val,
                                       jsonmatch_str_p,
                                       (void *) &pkmn_tmp_regex,
                                       0
                                     );
      if ( i <= 0 )
        {
          hint = jsmn_iterator_position( &item_iter );
          continue;
        }

      if ( first_pokemon_idx == 0 )
        {
          first_pokemon_idx = jsmn_iterator_position( &items_list_iter );
        }

      pdex_mon_t mon;
      parse_pdex_mon( gparser.buffer,
                      gparser.tokens,
                      jsmn_iterator_position( &items_list_iter ),
                      gparser.tokens_cnt,
                      &mon
                    );
      print_pdex_mon( &mon );
    }
#else

  /* Compile Regex to match Pokemon templateIds */
  regex_t pkmn_tmp_regex;
  int     rc_rsl = regcomp( &pkmn_tmp_regex, pokemon_template_pat, REG_NOSUB );
  assert( rc_rsl == 0 );

  jsmn_iterator_stack_t iter_stack;
  jsmn_iterator_stack_init( &iter_stack,
                            gparser.tokens,
                            gparser.tokens_cnt,
                            8
                          );
  jsmn_iterator_stack_push( &iter_stack, 0 ); /* Top level */

  jsmntok_t * key;
  jsmntok_t * val;
  int idx = jsmn_iterator_find_key( gparser.buffer,
                                    current_iterator( &iter_stack ),
                                    &key,
                                    jsoneq_str_p,
                                    (void *) "itemTemplate",
                                    &val,
                                    0
                                  );
  jsmn_iterator_stack_push_curr( &iter_stack ); /* Items list */

  jsmntok_t * item;
  while( 0 < jsmn_iterator_next( current_iterator( &iter_stack ),
                                 NULL,
                                 &item,
                                 iter_stack.hint
                               )
       ) {
    jsmn_iterator_stack_push_curr( &iter_stack );
    idx = jsmn_iterator_find_next( gparser.buffer,
                                   current_iterator( &iter_stack ),
                                   &key,
                                   jsoneq_str_p,
                                   (void *) "templateId",
                                   &val,
                                   jsonmatch_str_p,
                                   (void *) &pkmn_tmp_regex,
                                   0
                                 );
    if ( idx <= 0 )
      {
        jsmn_iterator_stack_pop( &iter_stack );
        continue;
      }

    pdex_mon_t mon;
    parse_pdex_mon( gparser.buffer,
                    gparser.tokens,
                    jsmn_iterator_position( current_iterator( &iter_stack ) ),
                    gparser.tokens_cnt,
                    &mon
                  );
    print_pdex_mon( &mon );

    jsmn_iterator_stack_pop( &iter_stack );
  }

  jsmn_iterator_stack_free( &iter_stack );

#endif


  /* Cleanup */
  regfree( &pkmn_tmp_regex );
  free_gm_parser( &gparser );

  return EXIT_SUCCESS;
}



/* ========================================================================= */

/* vim: set filetype=c : */
