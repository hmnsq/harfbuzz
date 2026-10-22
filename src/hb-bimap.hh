/*
 * Copyright © 2019 Adobe Inc.
 *
 *  This is part of HarfBuzz, a text shaping library.
 *
 * Permission is hereby granted, without written agreement and without
 * license or royalty fees, to use, copy, modify, and distribute this
 * software and its documentation for any purpose, provided that the
 * above copyright notice and the following two paragraphs appear in
 * all copies of this software.
 *
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
 * ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN
 * IF THE COPYRIGHT HOLDER HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 * THE COPYRIGHT HOLDER SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING,
 * BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE COPYRIGHT HOLDER HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 *
 * Adobe Author(s): Michiharu Ariza
 */

#ifndef HB_BIMAP_HH
#define HB_BIMAP_HH

#અન્તર્ભાવ "hb.hh"
#અન્તર્ભાવ "hb-map.hh"

/* Bi-directional map */
સંરચના hb_bimap_t
{
  શૂન્ય reset ()
  {
    forw_map.reset ();
    back_map.reset ();
  }

  શૂન્ય alloc (unsigned કાઢો)
  {
    forw_map.alloc (કાઢો);
    back_map.alloc (કાઢો);
  }

  તાર્કિક in_error () અચલ { પાછા forw_map.in_error () || back_map.in_error (); }

  શૂન્ય set (hb_codepoint_t lhs, hb_codepoint_t rhs)
  {
    જો (in_error ()) પાછા;
    જો (unlikely (lhs == HB_MAP_કિંમત_INVALID)) પાછા;
    જો (unlikely (rhs == HB_MAP_કિંમત_INVALID)) { del (lhs); પાછા; }

    forw_map.set (lhs, rhs);
    જો (unlikely (in_error ())) પાછા;

    back_map.set (rhs, lhs);
    જો (unlikely (in_error ())) forw_map.del (lhs);
  }

  hb_codepoint_t get (hb_codepoint_t lhs) અચલ { પાછા forw_map.get (lhs); }
  hb_codepoint_t backward (hb_codepoint_t rhs) અચલ { પાછા back_map.get (rhs); }

  hb_codepoint_t operator [] (hb_codepoint_t lhs) અચલ { પાછા get (lhs); }
  તાર્કિક has (hb_codepoint_t lhs) અચલ { પાછા forw_map.has (lhs); }


  શૂન્ય del (hb_codepoint_t lhs)
  {
    back_map.del (get (lhs));
    forw_map.del (lhs);
  }

  શૂન્ય clear ()
  {
    forw_map.clear ();
    back_map.clear ();
  }

  તાર્કિક is_empty () અચલ { પાછા forw_map.is_empty (); }

  unsigned પૂર્ણાંક get_population () અચલ { પાછા forw_map.get_population (); }

  સુરક્ષિત:
  hb_map_t  forw_map;
  hb_map_t  back_map;

  જાહેર:
  auto ચાવીઓ () અચલ HB_AUTO_RETURN (+ forw_map.ચાવીઓ())
  auto values () અચલ HB_AUTO_RETURN (+ forw_map.values())
  auto iter () અચલ HB_AUTO_RETURN (+ forw_map.iter())
};

/* Incremental bimap: only lhs is given, rhs is incrementally assigned */
સંરચના hb_inc_bimap_t
{
  તાર્કિક in_error () અચલ { પાછા forw_map.in_error () || back_map.in_error (); }

  unsigned પૂર્ણાંક get_population () અચલ { પાછા forw_map.get_population (); }

  શૂન્ય reset ()
  {
    forw_map.reset ();
    back_map.reset ();
  }

  શૂન્ય alloc (unsigned કાઢો)
  {
    forw_map.alloc (કાઢો);
    back_map.alloc (કાઢો);
  }

  શૂન્ય clear ()
  {
    forw_map.clear ();
    back_map.clear ();
  }

  /* Add a mapping from lhs to rhs with a unique value if lhs is unknown.
   * Return the rhs value as the result.
   */
  hb_codepoint_t add (hb_codepoint_t lhs)
  {
    hb_codepoint_t  rhs = forw_map[lhs];
    જો (rhs == HB_MAP_કિંમત_INVALID)
    {
      rhs = back_map.length;
      forw_map.set (lhs, rhs);
      back_map.push (lhs);
    }
    પાછા rhs;
  }

  hb_codepoint_t skip ()
  {
    hb_codepoint_t start = back_map.length;
    back_map.push (HB_MAP_કિંમત_INVALID);
    પાછા start;
  }

  hb_codepoint_t skip (unsigned ગણતરી)
  {
    hb_codepoint_t start = back_map.length;
    back_map.alloc (back_map.length + ગણતરી);
    માટે (unsigned i = 0; i < ગણતરી; i++)
      back_map.push (HB_MAP_કિંમત_INVALID);
    પાછા start;
  }

  hb_codepoint_t get_next_કિંમત () અચલ
  { પાછા back_map.length; }

  શૂન્ય add_set (અચલ hb_set_t *set)
  {
    માટે (auto i : *set) add (i);
  }

  /* Create an identity map. */
  તાર્કિક identity (unsigned પૂર્ણાંક size)
  {
    clear ();
    માટે (hb_codepoint_t i = 0; i < size; i++) add (i);
    પાછા !in_error ();
  }

  સુરક્ષિત:
  સ્થિર પૂર્ણાંક cmp_id (અચલ શૂન્ય* a, અચલ શૂન્ય* b)
  { પાછા (પૂર્ણાંક)*(અચલ hb_codepoint_t *)a - (પૂર્ણાંક)*(અચલ hb_codepoint_t *)b; }

  જાહેર:
  /* Optional: after finished adding all mappings in a random order,
   * reassign rhs to lhs so that they are in the same order. */
  શૂન્ય sort ()
  {
    hb_codepoint_t  ગણતરી = get_population ();
    hb_vector_t <hb_codepoint_t> work;
    જો (unlikely (!work.resize_dirty  (ગણતરી))) પાછા;

    માટે (hb_codepoint_t rhs = 0; rhs < ગણતરી; rhs++)
      work.arrayZ[rhs] = back_map[rhs];

    work.qsort (cmp_id);

    clear ();
    માટે (hb_codepoint_t rhs = 0; rhs < ગણતરી; rhs++)
      add (work.arrayZ[rhs]);
  }

  hb_codepoint_t get (hb_codepoint_t lhs) અચલ { પાછા forw_map.get (lhs); }
  hb_codepoint_t backward (hb_codepoint_t rhs) અચલ { પાછા back_map[rhs]; }

  hb_codepoint_t operator [] (hb_codepoint_t lhs) અચલ { પાછા get (lhs); }
  તાર્કિક has (hb_codepoint_t lhs) અચલ { પાછા forw_map.has (lhs); }

  સુરક્ષિત:
  hb_map_t forw_map;
  hb_vector_t<hb_codepoint_t> back_map;

  જાહેર:
  auto ચાવીઓ () અચલ HB_AUTO_RETURN (+ back_map.iter())
};

#endif /* HB_BIMAP_HH */
