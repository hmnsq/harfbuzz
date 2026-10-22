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

#ضمن "hb.hh"
#ضمن "hb-map.hh"

/* Bi-directional map */
بنية hb_bimap_t
{
  فراغ reset ()
  {
    forw_map.reset ();
    back_map.reset ();
  }

  فراغ alloc (unsigned pop)
  {
    forw_map.alloc (pop);
    back_map.alloc (pop);
  }

  منطقي in_error () ثابت { החזר forw_map.in_error () || back_map.in_error (); }

  فراغ set (hb_codepoint_t lhs, hb_codepoint_t rhs)
  {
    אם (in_error ()) החזר;
    אם (unlikely (lhs == HB_MAP_VALUE_INVALID)) החזר;
    אם (unlikely (rhs == HB_MAP_VALUE_INVALID)) { del (lhs); החזר; }

    forw_map.set (lhs, rhs);
    אם (unlikely (in_error ())) החזר;

    back_map.set (rhs, lhs);
    אם (unlikely (in_error ())) forw_map.del (lhs);
  }

  hb_codepoint_t get (hb_codepoint_t lhs) ثابت { החזר forw_map.get (lhs); }
  hb_codepoint_t backward (hb_codepoint_t rhs) ثابت { החזר back_map.get (rhs); }

  hb_codepoint_t operator [] (hb_codepoint_t lhs) ثابت { החזר get (lhs); }
  منطقي has (hb_codepoint_t lhs) ثابت { החזר forw_map.has (lhs); }


  فراغ del (hb_codepoint_t lhs)
  {
    back_map.del (get (lhs));
    forw_map.del (lhs);
  }

  فراغ clear ()
  {
    forw_map.clear ();
    back_map.clear ();
  }

  منطقي is_empty () ثابت { החזר forw_map.is_empty (); }

  unsigned صحيح get_population () ثابت { החזר forw_map.get_population (); }

  محمي:
  hb_map_t  forw_map;
  hb_map_t  back_map;

  عام:
  auto keys () ثابت HB_AUTO_RETURN (+ forw_map.keys())
  auto values () ثابت HB_AUTO_RETURN (+ forw_map.values())
  auto iter () ثابت HB_AUTO_RETURN (+ forw_map.iter())
};

/* Incremental bimap: only lhs is given, rhs is incrementally assigned */
بنية hb_inc_bimap_t
{
  منطقي in_error () ثابت { החזר forw_map.in_error () || back_map.in_error (); }

  unsigned صحيح get_population () ثابت { החזר forw_map.get_population (); }

  فراغ reset ()
  {
    forw_map.reset ();
    back_map.reset ();
  }

  فراغ alloc (unsigned pop)
  {
    forw_map.alloc (pop);
    back_map.alloc (pop);
  }

  فراغ clear ()
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
    אם (rhs == HB_MAP_VALUE_INVALID)
    {
      rhs = back_map.length;
      forw_map.set (lhs, rhs);
      back_map.push (lhs);
    }
    החזר rhs;
  }

  hb_codepoint_t skip ()
  {
    hb_codepoint_t start = back_map.length;
    back_map.push (HB_MAP_VALUE_INVALID);
    החזר start;
  }

  hb_codepoint_t skip (unsigned count)
  {
    hb_codepoint_t start = back_map.length;
    back_map.alloc (back_map.length + count);
    עבור (unsigned i = 0; i < count; i++)
      back_map.push (HB_MAP_VALUE_INVALID);
    החזר start;
  }

  hb_codepoint_t get_next_value () ثابت
  { החזר back_map.length; }

  فراغ add_set (ثابت hb_set_t *set)
  {
    עבור (auto i : *set) add (i);
  }

  /* Create an identity map. */
  منطقي identity (unsigned صحيح size)
  {
    clear ();
    עבור (hb_codepoint_t i = 0; i < size; i++) add (i);
    החזר !in_error ();
  }

  محمي:
  ساكن صحيح cmp_id (ثابت فراغ* a, ثابت فراغ* b)
  { החזר (صحيح)*(ثابت hb_codepoint_t *)a - (صحيح)*(ثابت hb_codepoint_t *)b; }

  عام:
  /* Optional: after finished adding all mappings in a random order,
   * reassign rhs to lhs so that they are in the same order. */
  فراغ sort ()
  {
    hb_codepoint_t  count = get_population ();
    hb_vector_t <hb_codepoint_t> work;
    אם (unlikely (!work.resize_dirty  (count))) החזר;

    עבור (hb_codepoint_t rhs = 0; rhs < count; rhs++)
      work.arrayZ[rhs] = back_map[rhs];

    work.qsort (cmp_id);

    clear ();
    עבור (hb_codepoint_t rhs = 0; rhs < count; rhs++)
      add (work.arrayZ[rhs]);
  }

  hb_codepoint_t get (hb_codepoint_t lhs) ثابت { החזר forw_map.get (lhs); }
  hb_codepoint_t backward (hb_codepoint_t rhs) ثابت { החזר back_map[rhs]; }

  hb_codepoint_t operator [] (hb_codepoint_t lhs) ثابت { החזר get (lhs); }
  منطقي has (hb_codepoint_t lhs) ثابت { החזר forw_map.has (lhs); }

  محمي:
  hb_map_t forw_map;
  hb_vector_t<hb_codepoint_t> back_map;

  عام:
  auto keys () ثابت HB_AUTO_RETURN (+ back_map.iter())
};

#endif /* HB_BIMAP_HH */
