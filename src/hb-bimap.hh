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

#شامل "hb.hh"
#شامل "hb-map.hh"

/* Bi-directional map */
ساختار hb_bimap_t
{
  تهی reset ()
  {
    forw_map.reset ();
    back_map.reset ();
  }

  تهی alloc (unsigned برداشت)
  {
    forw_map.alloc (برداشت);
    back_map.alloc (برداشت);
  }

  منطقی in_error () ثابت { بازگشت forw_map.in_error () || back_map.in_error (); }

  تهی set (hb_codepoint_t lhs, hb_codepoint_t rhs)
  {
    اگر (in_error ()) بازگشت;
    اگر (unlikely (lhs == HB_MAP_مقدار_INVALID)) بازگشت;
    اگر (unlikely (rhs == HB_MAP_مقدار_INVALID)) { del (lhs); بازگشت; }

    forw_map.set (lhs, rhs);
    اگر (unlikely (in_error ())) بازگشت;

    back_map.set (rhs, lhs);
    اگر (unlikely (in_error ())) forw_map.del (lhs);
  }

  hb_codepoint_t get (hb_codepoint_t lhs) ثابت { بازگشت forw_map.get (lhs); }
  hb_codepoint_t backward (hb_codepoint_t rhs) ثابت { بازگشت back_map.get (rhs); }

  hb_codepoint_t operator [] (hb_codepoint_t lhs) ثابت { بازگشت get (lhs); }
  منطقی has (hb_codepoint_t lhs) ثابت { بازگشت forw_map.has (lhs); }


  تهی del (hb_codepoint_t lhs)
  {
    back_map.del (get (lhs));
    forw_map.del (lhs);
  }

  تهی clear ()
  {
    forw_map.clear ();
    back_map.clear ();
  }

  منطقی is_empty () ثابت { بازگشت forw_map.is_empty (); }

  unsigned صحیح get_population () ثابت { بازگشت forw_map.get_population (); }

  محافظتشده:
  hb_map_t  forw_map;
  hb_map_t  back_map;

  عمومی:
  auto کلیدها () ثابت HB_AUTO_RETURN (+ forw_map.کلیدها())
  auto values () ثابت HB_AUTO_RETURN (+ forw_map.values())
  auto iter () ثابت HB_AUTO_RETURN (+ forw_map.iter())
};

/* Incremental bimap: only lhs is given, rhs is incrementally assigned */
ساختار hb_inc_bimap_t
{
  منطقی in_error () ثابت { بازگشت forw_map.in_error () || back_map.in_error (); }

  unsigned صحیح get_population () ثابت { بازگشت forw_map.get_population (); }

  تهی reset ()
  {
    forw_map.reset ();
    back_map.reset ();
  }

  تهی alloc (unsigned برداشت)
  {
    forw_map.alloc (برداشت);
    back_map.alloc (برداشت);
  }

  تهی clear ()
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
    اگر (rhs == HB_MAP_مقدار_INVALID)
    {
      rhs = back_map.length;
      forw_map.set (lhs, rhs);
      back_map.push (lhs);
    }
    بازگشت rhs;
  }

  hb_codepoint_t skip ()
  {
    hb_codepoint_t start = back_map.length;
    back_map.push (HB_MAP_مقدار_INVALID);
    بازگشت start;
  }

  hb_codepoint_t skip (unsigned شمار)
  {
    hb_codepoint_t start = back_map.length;
    back_map.alloc (back_map.length + شمار);
    برای (unsigned i = 0; i < شمار; i++)
      back_map.push (HB_MAP_مقدار_INVALID);
    بازگشت start;
  }

  hb_codepoint_t get_next_مقدار () ثابت
  { بازگشت back_map.length; }

  تهی add_set (ثابت hb_set_t *set)
  {
    برای (auto i : *set) add (i);
  }

  /* Create an identity map. */
  منطقی identity (unsigned صحیح size)
  {
    clear ();
    برای (hb_codepoint_t i = 0; i < size; i++) add (i);
    بازگشت !in_error ();
  }

  محافظتشده:
  ایستا صحیح cmp_id (ثابت تهی* a, ثابت تهی* b)
  { بازگشت (صحیح)*(ثابت hb_codepoint_t *)a - (صحیح)*(ثابت hb_codepoint_t *)b; }

  عمومی:
  /* Optional: after finished adding all mappings in a random order,
   * reassign rhs to lhs so that they are in the same order. */
  تهی sort ()
  {
    hb_codepoint_t  شمار = get_population ();
    hb_vector_t <hb_codepoint_t> work;
    اگر (unlikely (!work.resize_dirty  (شمار))) بازگشت;

    برای (hb_codepoint_t rhs = 0; rhs < شمار; rhs++)
      work.arrayZ[rhs] = back_map[rhs];

    work.qsort (cmp_id);

    clear ();
    برای (hb_codepoint_t rhs = 0; rhs < شمار; rhs++)
      add (work.arrayZ[rhs]);
  }

  hb_codepoint_t get (hb_codepoint_t lhs) ثابت { بازگشت forw_map.get (lhs); }
  hb_codepoint_t backward (hb_codepoint_t rhs) ثابت { بازگشت back_map[rhs]; }

  hb_codepoint_t operator [] (hb_codepoint_t lhs) ثابت { بازگشت get (lhs); }
  منطقی has (hb_codepoint_t lhs) ثابت { بازگشت forw_map.has (lhs); }

  محافظتشده:
  hb_map_t forw_map;
  hb_vector_t<hb_codepoint_t> back_map;

  عمومی:
  auto کلیدها () ثابت HB_AUTO_RETURN (+ back_map.iter())
};

#endif /* HB_BIMAP_HH */
