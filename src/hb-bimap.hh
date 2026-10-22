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

#অন্তর্ভাব "hb.hh"
#অন্তর্ভাব "hb-map.hh"

/* Bi-directional map */
কাঠামো hb_bimap_t
{
  শূন্য reset ()
  {
    forw_map.reset ();
    back_map.reset ();
  }

  শূন্য alloc (unsigned বের_করো)
  {
    forw_map.alloc (বের_করো);
    back_map.alloc (বের_করো);
  }

  যৌক্তিক in_error () ধ্রুবক { ফেরত forw_map.in_error () || back_map.in_error (); }

  শূন্য set (hb_codepoint_t lhs, hb_codepoint_t rhs)
  {
    যদি (in_error ()) ফেরত;
    যদি (unlikely (lhs == HB_MAP_মান_INVALID)) ফেরত;
    যদি (unlikely (rhs == HB_MAP_মান_INVALID)) { del (lhs); ফেরত; }

    forw_map.set (lhs, rhs);
    যদি (unlikely (in_error ())) ফেরত;

    back_map.set (rhs, lhs);
    যদি (unlikely (in_error ())) forw_map.del (lhs);
  }

  hb_codepoint_t get (hb_codepoint_t lhs) ধ্রুবক { ফেরত forw_map.get (lhs); }
  hb_codepoint_t backward (hb_codepoint_t rhs) ধ্রুবক { ফেরত back_map.get (rhs); }

  hb_codepoint_t operator [] (hb_codepoint_t lhs) ধ্রুবক { ফেরত get (lhs); }
  যৌক্তিক has (hb_codepoint_t lhs) ধ্রুবক { ফেরত forw_map.has (lhs); }


  শূন্য del (hb_codepoint_t lhs)
  {
    back_map.del (get (lhs));
    forw_map.del (lhs);
  }

  শূন্য clear ()
  {
    forw_map.clear ();
    back_map.clear ();
  }

  যৌক্তিক is_empty () ধ্রুবক { ফেরত forw_map.is_empty (); }

  unsigned পূর্ণসংখ্যা get_population () ধ্রুবক { ফেরত forw_map.get_population (); }

  সুরক্ষিত:
  hb_map_t  forw_map;
  hb_map_t  back_map;

  সর্বজনীন:
  auto চাবিগুলি () ধ্রুবক HB_AUTO_RETURN (+ forw_map.চাবিগুলি())
  auto values () ধ্রুবক HB_AUTO_RETURN (+ forw_map.values())
  auto iter () ধ্রুবক HB_AUTO_RETURN (+ forw_map.iter())
};

/* Incremental bimap: only lhs is given, rhs is incrementally assigned */
কাঠামো hb_inc_bimap_t
{
  যৌক্তিক in_error () ধ্রুবক { ফেরত forw_map.in_error () || back_map.in_error (); }

  unsigned পূর্ণসংখ্যা get_population () ধ্রুবক { ফেরত forw_map.get_population (); }

  শূন্য reset ()
  {
    forw_map.reset ();
    back_map.reset ();
  }

  শূন্য alloc (unsigned বের_করো)
  {
    forw_map.alloc (বের_করো);
    back_map.alloc (বের_করো);
  }

  শূন্য clear ()
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
    যদি (rhs == HB_MAP_মান_INVALID)
    {
      rhs = back_map.দৈর্ঘ্য;
      forw_map.set (lhs, rhs);
      back_map.push (lhs);
    }
    ফেরত rhs;
  }

  hb_codepoint_t skip ()
  {
    hb_codepoint_t start = back_map.দৈর্ঘ্য;
    back_map.push (HB_MAP_মান_INVALID);
    ফেরত start;
  }

  hb_codepoint_t skip (unsigned গুনতি)
  {
    hb_codepoint_t start = back_map.দৈর্ঘ্য;
    back_map.alloc (back_map.দৈর্ঘ্য + গুনতি);
    জন্য (unsigned i = 0; i < গুনতি; i++)
      back_map.push (HB_MAP_মান_INVALID);
    ফেরত start;
  }

  hb_codepoint_t get_পরবর্তী_মান () ধ্রুবক
  { ফেরত back_map.দৈর্ঘ্য; }

  শূন্য add_set (ধ্রুবক hb_set_t *set)
  {
    জন্য (auto i : *set) add (i);
  }

  /* Create an identity map. */
  যৌক্তিক identity (unsigned পূর্ণসংখ্যা আকার_2)
  {
    clear ();
    জন্য (hb_codepoint_t i = 0; i < আকার_2; i++) add (i);
    ফেরত !in_error ();
  }

  সুরক্ষিত:
  স্থির পূর্ণসংখ্যা cmp_id (ধ্রুবক শূন্য* a, ধ্রুবক শূন্য* b)
  { ফেরত (পূর্ণসংখ্যা)*(ধ্রুবক hb_codepoint_t *)a - (পূর্ণসংখ্যা)*(ধ্রুবক hb_codepoint_t *)b; }

  সর্বজনীন:
  /* Optional: after finished adding all mappings in a random order,
   * reassign rhs to lhs so that they are in the same order. */
  শূন্য sort ()
  {
    hb_codepoint_t  গুনতি = get_population ();
    hb_vector_t <hb_codepoint_t> work;
    যদি (unlikely (!work.resize_dirty  (গুনতি))) ফেরত;

    জন্য (hb_codepoint_t rhs = 0; rhs < গুনতি; rhs++)
      work.arrayZ[rhs] = back_map[rhs];

    work.qsort (cmp_id);

    clear ();
    জন্য (hb_codepoint_t rhs = 0; rhs < গুনতি; rhs++)
      add (work.arrayZ[rhs]);
  }

  hb_codepoint_t get (hb_codepoint_t lhs) ধ্রুবক { ফেরত forw_map.get (lhs); }
  hb_codepoint_t backward (hb_codepoint_t rhs) ধ্রুবক { ফেরত back_map[rhs]; }

  hb_codepoint_t operator [] (hb_codepoint_t lhs) ধ্রুবক { ফেরত get (lhs); }
  যৌক্তিক has (hb_codepoint_t lhs) ধ্রুবক { ফেরত forw_map.has (lhs); }

  সুরক্ষিত:
  hb_map_t forw_map;
  hb_vector_t<hb_codepoint_t> back_map;

  সর্বজনীন:
  auto চাবিগুলি () ধ্রুবক HB_AUTO_RETURN (+ back_map.iter())
};

#endif /* HB_BIMAP_HH */
