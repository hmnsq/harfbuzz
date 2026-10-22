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

#include "hb.hh"
#include "hb-map.hh"

/* Bi-directional map */
sa_nracanaa hb_bimap_t
{
  void reset ()
  {
    forw_map.reset ();
    back_map.reset ();
  }

  void alloc (unsigned pop)
  {
    forw_map.alloc (pop);
    back_map.alloc (pop);
  }

  bool in_error () const { ပွၵ်ႈ forw_map.in_error () || back_map.in_error (); }

  void set (hb_codepoint_t lhs, hb_codepoint_t rhs)
  {
    ပေႃး (in_error ()) ပွၵ်ႈ;
    ပေႃး (unlikely (lhs == HB_MAP_VALUE_INVALID)) ပွၵ်ႈ;
    ပေႃး (unlikely (rhs == HB_MAP_VALUE_INVALID)) { del (lhs); ပွၵ်ႈ; }

    forw_map.set (lhs, rhs);
    ပေႃး (unlikely (in_error ())) ပွၵ်ႈ;

    back_map.set (rhs, lhs);
    ပေႃး (unlikely (in_error ())) forw_map.del (lhs);
  }

  hb_codepoint_t get (hb_codepoint_t lhs) const { ပွၵ်ႈ forw_map.get (lhs); }
  hb_codepoint_t backward (hb_codepoint_t rhs) const { ပွၵ်ႈ back_map.get (rhs); }

  hb_codepoint_t operator [] (hb_codepoint_t lhs) const { ပွၵ်ႈ get (lhs); }
  bool has (hb_codepoint_t lhs) const { ပွၵ်ႈ forw_map.has (lhs); }


  void del (hb_codepoint_t lhs)
  {
    back_map.del (get (lhs));
    forw_map.del (lhs);
  }

  void clear ()
  {
    forw_map.clear ();
    back_map.clear ();
  }

  bool is_empty () const { ပွၵ်ႈ forw_map.is_empty (); }

  unsigned puur_naa_nka get_population () const { ပွၵ်ႈ forw_map.get_population (); }

  rak_sita:
  hb_map_t  forw_map;
  hb_map_t  back_map;

  public:
  auto keys () const HB_AUTO_RETURN (+ forw_map.keys())
  auto values () const HB_AUTO_RETURN (+ forw_map.values())
  auto iter () const HB_AUTO_RETURN (+ forw_map.iter())
};

/* Incremental bimap: only lhs is given, rhs is incrementally assigned */
sa_nracanaa hb_inc_bimap_t
{
  bool in_error () const { ပွၵ်ႈ forw_map.in_error () || back_map.in_error (); }

  unsigned puur_naa_nka get_population () const { ပွၵ်ႈ forw_map.get_population (); }

  void reset ()
  {
    forw_map.reset ();
    back_map.reset ();
  }

  void alloc (unsigned pop)
  {
    forw_map.alloc (pop);
    back_map.alloc (pop);
  }

  void clear ()
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
    ပေႃး (rhs == HB_MAP_VALUE_INVALID)
    {
      rhs = back_map.length;
      forw_map.set (lhs, rhs);
      back_map.push (lhs);
    }
    ပွၵ်ႈ rhs;
  }

  hb_codepoint_t skip ()
  {
    hb_codepoint_t start = back_map.length;
    back_map.push (HB_MAP_VALUE_INVALID);
    ပွၵ်ႈ start;
  }

  hb_codepoint_t skip (unsigned count)
  {
    hb_codepoint_t start = back_map.length;
    back_map.alloc (back_map.length + count);
    for (unsigned i = 0; i < count; i++)
      back_map.push (HB_MAP_VALUE_INVALID);
    ပွၵ်ႈ start;
  }

  hb_codepoint_t get_next_value () const
  { ပွၵ်ႈ back_map.length; }

  void add_set (const hb_set_t *set)
  {
    for (auto i : *set) add (i);
  }

  /* Create an identity map. */
  bool identity (unsigned puur_naa_nka size)
  {
    clear ();
    for (hb_codepoint_t i = 0; i < size; i++) add (i);
    ပွၵ်ႈ !in_error ();
  }

  rak_sita:
  static puur_naa_nka cmp_id (const void* a, const void* b)
  { ပွၵ်ႈ (puur_naa_nka)*(const hb_codepoint_t *)a - (puur_naa_nka)*(const hb_codepoint_t *)b; }

  public:
  /* Optional: after finished adding all mappings in a random order,
   * reassign rhs to lhs so that they are in the same order. */
  void sort ()
  {
    hb_codepoint_t  count = get_population ();
    hb_vector_t <hb_codepoint_t> work;
    ပေႃး (unlikely (!work.resize_dirty  (count))) ပွၵ်ႈ;

    for (hb_codepoint_t rhs = 0; rhs < count; rhs++)
      work.arrayZ[rhs] = back_map[rhs];

    work.qsort (cmp_id);

    clear ();
    for (hb_codepoint_t rhs = 0; rhs < count; rhs++)
      add (work.arrayZ[rhs]);
  }

  hb_codepoint_t get (hb_codepoint_t lhs) const { ပွၵ်ႈ forw_map.get (lhs); }
  hb_codepoint_t backward (hb_codepoint_t rhs) const { ပွၵ်ႈ back_map[rhs]; }

  hb_codepoint_t operator [] (hb_codepoint_t lhs) const { ပွၵ်ႈ get (lhs); }
  bool has (hb_codepoint_t lhs) const { ပွၵ်ႈ forw_map.has (lhs); }

  rak_sita:
  hb_map_t forw_map;
  hb_vector_t<hb_codepoint_t> back_map;

  public:
  auto keys () const HB_AUTO_RETURN (+ back_map.iter())
};

#endif /* HB_BIMAP_HH */
