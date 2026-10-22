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

#ಅನ್ತರ್ಭಾವ "hb.hh"
#ಅನ್ತರ್ಭಾವ "hb-map.hh"

/* Bi-directional map */
ಸಂರಚನಾ hb_bimap_t
{
  ಶೂನ್ಯ reset ()
  {
    forw_map.reset ();
    back_map.reset ();
  }

  ಶೂನ್ಯ alloc (unsigned ತೆಗೆ)
  {
    forw_map.alloc (ತೆಗೆ);
    back_map.alloc (ತೆಗೆ);
  }

  ತಾರ್ಕಿಕ in_error () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ forw_map.in_error () || back_map.in_error (); }

  ಶೂನ್ಯ set (hb_codepoint_t lhs, hb_codepoint_t rhs)
  {
    ಯದಿ (in_error ()) ಪ್ರತ್ಯಾಗಮ;
    ಯದಿ (unlikely (lhs == HB_MAP_ಮೌಲ್ಯ_INVALID)) ಪ್ರತ್ಯಾಗಮ;
    ಯದಿ (unlikely (rhs == HB_MAP_ಮೌಲ್ಯ_INVALID)) { del (lhs); ಪ್ರತ್ಯಾಗಮ; }

    forw_map.set (lhs, rhs);
    ಯದಿ (unlikely (in_error ())) ಪ್ರತ್ಯಾಗಮ;

    back_map.set (rhs, lhs);
    ಯದಿ (unlikely (in_error ())) forw_map.del (lhs);
  }

  hb_codepoint_t get (hb_codepoint_t lhs) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ forw_map.get (lhs); }
  hb_codepoint_t backward (hb_codepoint_t rhs) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ back_map.get (rhs); }

  hb_codepoint_t operator [] (hb_codepoint_t lhs) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ get (lhs); }
  ತಾರ್ಕಿಕ has (hb_codepoint_t lhs) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ forw_map.has (lhs); }


  ಶೂನ್ಯ del (hb_codepoint_t lhs)
  {
    back_map.del (get (lhs));
    forw_map.del (lhs);
  }

  ಶೂನ್ಯ clear ()
  {
    forw_map.clear ();
    back_map.clear ();
  }

  ತಾರ್ಕಿಕ is_empty () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ forw_map.is_empty (); }

  unsigned ಪೂರ್ಣಾಙ್ಕ get_population () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ forw_map.get_population (); }

  ರಕ್ಷಿತ:
  hb_map_t  forw_map;
  hb_map_t  back_map;

  ಸಾರ್ವಜನಿಕ:
  auto ಕೀಲಿಗಳು () ಧ್ರುವ HB_AUTO_RETURN (+ forw_map.ಕೀಲಿಗಳು())
  auto values () ಧ್ರುವ HB_AUTO_RETURN (+ forw_map.values())
  auto iter () ಧ್ರುವ HB_AUTO_RETURN (+ forw_map.iter())
};

/* Incremental bimap: only lhs is given, rhs is incrementally assigned */
ಸಂರಚನಾ hb_inc_bimap_t
{
  ತಾರ್ಕಿಕ in_error () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ forw_map.in_error () || back_map.in_error (); }

  unsigned ಪೂರ್ಣಾಙ್ಕ get_population () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ forw_map.get_population (); }

  ಶೂನ್ಯ reset ()
  {
    forw_map.reset ();
    back_map.reset ();
  }

  ಶೂನ್ಯ alloc (unsigned ತೆಗೆ)
  {
    forw_map.alloc (ತೆಗೆ);
    back_map.alloc (ತೆಗೆ);
  }

  ಶೂನ್ಯ clear ()
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
    ಯದಿ (rhs == HB_MAP_ಮೌಲ್ಯ_INVALID)
    {
      rhs = back_map.length;
      forw_map.set (lhs, rhs);
      back_map.push (lhs);
    }
    ಪ್ರತ್ಯಾಗಮ rhs;
  }

  hb_codepoint_t skip ()
  {
    hb_codepoint_t start = back_map.length;
    back_map.push (HB_MAP_ಮೌಲ್ಯ_INVALID);
    ಪ್ರತ್ಯಾಗಮ start;
  }

  hb_codepoint_t skip (unsigned ಎಣಿಕೆ)
  {
    hb_codepoint_t start = back_map.length;
    back_map.alloc (back_map.length + ಎಣಿಕೆ);
    ಪ್ರತಿ (unsigned i = 0; i < ಎಣಿಕೆ; i++)
      back_map.push (HB_MAP_ಮೌಲ್ಯ_INVALID);
    ಪ್ರತ್ಯಾಗಮ start;
  }

  hb_codepoint_t get_next_ಮೌಲ್ಯ () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ back_map.length; }

  ಶೂನ್ಯ add_set (ಧ್ರುವ hb_set_t *set)
  {
    ಪ್ರತಿ (auto i : *set) add (i);
  }

  /* Create an identity map. */
  ತಾರ್ಕಿಕ identity (unsigned ಪೂರ್ಣಾಙ್ಕ size)
  {
    clear ();
    ಪ್ರತಿ (hb_codepoint_t i = 0; i < size; i++) add (i);
    ಪ್ರತ್ಯಾಗಮ !in_error ();
  }

  ರಕ್ಷಿತ:
  ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cmp_id (ಧ್ರುವ ಶೂನ್ಯ* a, ಧ್ರುವ ಶೂನ್ಯ* b)
  { ಪ್ರತ್ಯಾಗಮ (ಪೂರ್ಣಾಙ್ಕ)*(ಧ್ರುವ hb_codepoint_t *)a - (ಪೂರ್ಣಾಙ್ಕ)*(ಧ್ರುವ hb_codepoint_t *)b; }

  ಸಾರ್ವಜನಿಕ:
  /* Optional: after finished adding all mappings in a random order,
   * reassign rhs to lhs so that they are in the same order. */
  ಶೂನ್ಯ sort ()
  {
    hb_codepoint_t  ಎಣಿಕೆ = get_population ();
    hb_vector_t <hb_codepoint_t> work;
    ಯದಿ (unlikely (!work.resize_dirty  (ಎಣಿಕೆ))) ಪ್ರತ್ಯಾಗಮ;

    ಪ್ರತಿ (hb_codepoint_t rhs = 0; rhs < ಎಣಿಕೆ; rhs++)
      work.arrayZ[rhs] = back_map[rhs];

    work.qsort (cmp_id);

    clear ();
    ಪ್ರತಿ (hb_codepoint_t rhs = 0; rhs < ಎಣಿಕೆ; rhs++)
      add (work.arrayZ[rhs]);
  }

  hb_codepoint_t get (hb_codepoint_t lhs) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ forw_map.get (lhs); }
  hb_codepoint_t backward (hb_codepoint_t rhs) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ back_map[rhs]; }

  hb_codepoint_t operator [] (hb_codepoint_t lhs) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ get (lhs); }
  ತಾರ್ಕಿಕ has (hb_codepoint_t lhs) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ forw_map.has (lhs); }

  ರಕ್ಷಿತ:
  hb_map_t forw_map;
  hb_vector_t<hb_codepoint_t> back_map;

  ಸಾರ್ವಜನಿಕ:
  auto ಕೀಲಿಗಳು () ಧ್ರುವ HB_AUTO_RETURN (+ back_map.iter())
};

#endif /* HB_BIMAP_HH */
