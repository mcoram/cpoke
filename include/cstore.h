/* -*- mode: c; -*- */

#ifndef _CSTORE_H
#define _CSTORE_H

/* ========================================================================= */

#include "store.h"
#include "pokedex.h"
#include "moves.h"
#include "ptypes.h"
#include <stdlib.h>
#include <stdint.h>


/* ------------------------------------------------------------------------- */

struct cstore_aux_s {
  uint16_t        mons_cnt;
  uint16_t        moves_cnt;
  pdex_mon_t   *  mons_by_name;
  store_move_t *  moves_by_id;
  store_move_t *  moves_by_name;
};
typedef struct cstore_aux_s  cstore_aux_t;

#define as_csa( STORE_PTR )  ( (cstore_aux_t *) ( STORE_PTR )->aux )

typedef store_t cstore_t;


/* ------------------------------------------------------------------------- */

bool cstore_has( store_t * cstore, store_key_t key );
int  cstore_get( store_t * cstore, store_key_t key, void ** val );
int  cstore_get_str( store_t * cstore, const char *, void ** val );
int  cstore_get_str_t( store_t      *  cstore,
                       store_type_t    val_type,
                       const char   *  key,
                       void         ** val
                     );
int  cstore_init( store_t * cstore, void * pokedex );
void cstore_free( store_t * cstore );


/* ------------------------------------------------------------------------- */

int cstore_get_pokemon( cstore_t   *  cstore,
                        uint16_t      dex_num,
                        uint8_t       form_idx,
                        pdex_mon_t ** mon
                      );

int cstore_get_pokemon_by_name( cstore_t   *  cstore,
                                const char *  name,
                                pdex_mon_t ** mon
                              );


/* ------------------------------------------------------------------------- */

int cstore_get_move( cstore_t     *  cstore,
                     uint16_t        move_id,
                     store_move_t ** move
                   );

int cstore_get_move_by_name( cstore_t     *  cstore,
                             const char   *  name,
                             store_move_t ** move
                           );

/* ------------------------------------------------------------------------- */

  static inline int
cstore_export( store_t      * cstore,
               store_sink_t   sink_type,
               void         * target
             )
{
  return STORE_ERROR_NOT_DEFINED;
}

  static inline int
cstore_add( store_t * cstore, store_key_t key, void * val )
{
  return STORE_ERROR_NOT_WRITABLE;
}

  static inline int
cstore_set( store_t * cstore, store_key_t key, void * val )
{
  return STORE_ERROR_NOT_WRITABLE;
}


/* ------------------------------------------------------------------------- */

#define def_cstore()                                                        \
  {                                                                         \
    .name      = "Game Master Static",                                      \
    .flags     = SF_OFFICIAL_DATA_M | SF_STANDARD_KEY_M | SF_TYPED_M |      \
                 SF_GET_STRING_M | SF_GET_TYPED_STRING_M,                   \
    .has       = cstore_has,                                                \
    .get       = cstore_get,                                                \
    .get_str   = cstore_get_str,                                            \
    .get_str_t = cstore_get_str_t,                                          \
    .add       = cstore_add,                                                \
    .set       = cstore_set,                                                \
    .export    = cstore_export,                                             \
    .init      = cstore_init,                                               \
    .free      = cstore_free,                                               \
    .aux       = NULL                                                       \
  }


/* ------------------------------------------------------------------------- */

#ifdef CSTORE_GLOBAL_STORE

static store_t CSTORE = def_cstore();

#define CS_has( KEY )       cstore_has( & CSTORE, ( KEY ) )
#define CS_get( KEY, VAL )                                                    \
  cstore_get( & CSTORE, ( KEY ), (void **) ( VAL ) )
#define CS_get_str( KEY, VAL )                                                \
  cstore_get_str( & CSTORE, ( KEY ), (void **) ( VAL ) )
#define CS_get_str_t( TYPE, KEY, VAL )                                        \
  cstore_get_str_t( & CSTORE, ( TYPE ), ( KEY ), (void **) ( VAL ) )
#define CS_add( KEY, VAL )                                                    \
  cstore_add( & CSTORE, ( KEY ), (void *) ( VAL ) )
#define CS_set( KEY, VAL )                                                    \
  cstore_set( & CSTORE, ( KEY ), (void *) ( VAL ) )
#define CS_init()  cstore_init( & CSTORE, NULL )
#define CS_free()  cstore_free( & CSTORE )
#define CS_export( SINK, TARGET )                                             \
  cstore_export( & CSTORE, ( SINK ), ( TARGET ) )

#define CS_get_pokemon( DEX, FORM, VAL )                                      \
  cstore_get_pokemon( & CSTORE, ( DEX ), ( FORM ), ( VAL ) )
#define CS_get_pokemon_by_name( NAME, VAL )                                   \
  cstore_get_pokemon_by_name( & CSTORE, ( NAME ), ( VAL ) )

#define CS_get_move( MOVE_IDX, VAL )                                          \
  cstore_get_move( & CSTORE, ( MOVE_IDX ), ( VAL ) )
#define CS_get_move_by_name( NAME, VAL )                                      \
  cstore_get_move_by_name( & CSTORE, ( NAME ), ( VAL ) )

#endif


/* ------------------------------------------------------------------------- */



/* ========================================================================= */

#endif /* cstore.h */

/* vim: set filetype=c : */
