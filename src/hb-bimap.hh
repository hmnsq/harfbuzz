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

#ଅନ୍ତର୍ଭାଵ "hb.hh"
#ଅନ୍ତର୍ଭାଵ "hb-map.hh"

/* Bi-directional map */
ସଂରଚନା hb_bimap_t
{
  ଶୂନ୍ୟ reset ()
  {
    forw_map.reset ();
    back_map.reset ();
  }

  ଶୂନ୍ୟ alloc (unsigned କାଢ଼)
  {
    forw_map.alloc (କାଢ଼);
    back_map.alloc (କାଢ଼);
  }

  ତାର୍କିକ in_error () ଅଚଳ { ଫେରନ୍ତୁ forw_map.in_error () || back_map.in_error (); }

  ଶୂନ୍ୟ set (hb_codepoint_t lhs, hb_codepoint_t rhs)
  {
    ଯଦି (in_error ()) ଫେରନ୍ତୁ;
    ଯଦି (unlikely (lhs == HB_MAP_ମୂଲ୍ୟ_INVALID)) ଫେରନ୍ତୁ;
    ଯଦି (unlikely (rhs == HB_MAP_ମୂଲ୍ୟ_INVALID)) { del (lhs); ଫେରନ୍ତୁ; }

    forw_map.set (lhs, rhs);
    ଯଦି (unlikely (in_error ())) ଫେରନ୍ତୁ;

    back_map.set (rhs, lhs);
    ଯଦି (unlikely (in_error ())) forw_map.del (lhs);
  }

  hb_codepoint_t get (hb_codepoint_t lhs) ଅଚଳ { ଫେରନ୍ତୁ forw_map.get (lhs); }
  hb_codepoint_t backward (hb_codepoint_t rhs) ଅଚଳ { ଫେରନ୍ତୁ back_map.get (rhs); }

  hb_codepoint_t operator [] (hb_codepoint_t lhs) ଅଚଳ { ଫେରନ୍ତୁ get (lhs); }
  ତାର୍କିକ has (hb_codepoint_t lhs) ଅଚଳ { ଫେରନ୍ତୁ forw_map.has (lhs); }


  ଶୂନ୍ୟ del (hb_codepoint_t lhs)
  {
    back_map.del (get (lhs));
    forw_map.del (lhs);
  }

  ଶୂନ୍ୟ clear ()
  {
    forw_map.clear ();
    back_map.clear ();
  }

  ତାର୍କିକ is_empty () ଅଚଳ { ଫେରନ୍ତୁ forw_map.is_empty (); }

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_population () ଅଚଳ { ଫେରନ୍ତୁ forw_map.get_population (); }

  ସୁରକ୍ଷିତ:
  hb_map_t  forw_map;
  hb_map_t  back_map;

  ସର୍ବସାଧାରଣ:
  auto ଚାବିଗୁଡ଼ିକ () ଅଚଳ HB_AUTO_RETURN (+ forw_map.ଚାବିଗୁଡ଼ିକ())
  auto values () ଅଚଳ HB_AUTO_RETURN (+ forw_map.values())
  auto iter () ଅଚଳ HB_AUTO_RETURN (+ forw_map.iter())
};

/* Incremental bimap: only lhs is given, rhs is incrementally assigned */
ସଂରଚନା hb_inc_bimap_t
{
  ତାର୍କିକ in_error () ଅଚଳ { ଫେରନ୍ତୁ forw_map.in_error () || back_map.in_error (); }

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_population () ଅଚଳ { ଫେରନ୍ତୁ forw_map.get_population (); }

  ଶୂନ୍ୟ reset ()
  {
    forw_map.reset ();
    back_map.reset ();
  }

  ଶୂନ୍ୟ alloc (unsigned କାଢ଼)
  {
    forw_map.alloc (କାଢ଼);
    back_map.alloc (କାଢ଼);
  }

  ଶୂନ୍ୟ clear ()
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
    ଯଦି (rhs == HB_MAP_ମୂଲ୍ୟ_INVALID)
    {
      rhs = back_map.length;
      forw_map.set (lhs, rhs);
      back_map.push (lhs);
    }
    ଫେରନ୍ତୁ rhs;
  }

  hb_codepoint_t skip ()
  {
    hb_codepoint_t start = back_map.length;
    back_map.push (HB_MAP_ମୂଲ୍ୟ_INVALID);
    ଫେରନ୍ତୁ start;
  }

  hb_codepoint_t skip (unsigned ଗଣନା_2)
  {
    hb_codepoint_t start = back_map.length;
    back_map.alloc (back_map.length + ଗଣନା_2);
    ପାଇଁ (unsigned i = 0; i < ଗଣନା_2; i++)
      back_map.push (HB_MAP_ମୂଲ୍ୟ_INVALID);
    ଫେରନ୍ତୁ start;
  }

  hb_codepoint_t get_next_ମୂଲ୍ୟ () ଅଚଳ
  { ଫେରନ୍ତୁ back_map.length; }

  ଶୂନ୍ୟ add_set (ଅଚଳ hb_set_t *set)
  {
    ପାଇଁ (auto i : *set) add (i);
  }

  /* Create an identity map. */
  ତାର୍କିକ identity (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା size)
  {
    clear ();
    ପାଇଁ (hb_codepoint_t i = 0; i < size; i++) add (i);
    ଫେରନ୍ତୁ !in_error ();
  }

  ସୁରକ୍ଷିତ:
  ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp_id (ଅଚଳ ଶୂନ୍ୟ* a, ଅଚଳ ଶୂନ୍ୟ* b)
  { ଫେରନ୍ତୁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)*(ଅଚଳ hb_codepoint_t *)a - (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)*(ଅଚଳ hb_codepoint_t *)b; }

  ସର୍ବସାଧାରଣ:
  /* Optional: after finished adding all mappings in a random order,
   * reassign rhs to lhs so that they are in the same order. */
  ଶୂନ୍ୟ sort ()
  {
    hb_codepoint_t  ଗଣନା_2 = get_population ();
    hb_vector_t <hb_codepoint_t> work;
    ଯଦି (unlikely (!work.resize_dirty  (ଗଣନା_2))) ଫେରନ୍ତୁ;

    ପାଇଁ (hb_codepoint_t rhs = 0; rhs < ଗଣନା_2; rhs++)
      work.arrayZ[rhs] = back_map[rhs];

    work.qsort (cmp_id);

    clear ();
    ପାଇଁ (hb_codepoint_t rhs = 0; rhs < ଗଣନା_2; rhs++)
      add (work.arrayZ[rhs]);
  }

  hb_codepoint_t get (hb_codepoint_t lhs) ଅଚଳ { ଫେରନ୍ତୁ forw_map.get (lhs); }
  hb_codepoint_t backward (hb_codepoint_t rhs) ଅଚଳ { ଫେରନ୍ତୁ back_map[rhs]; }

  hb_codepoint_t operator [] (hb_codepoint_t lhs) ଅଚଳ { ଫେରନ୍ତୁ get (lhs); }
  ତାର୍କିକ has (hb_codepoint_t lhs) ଅଚଳ { ଫେରନ୍ତୁ forw_map.has (lhs); }

  ସୁରକ୍ଷିତ:
  hb_map_t forw_map;
  hb_vector_t<hb_codepoint_t> back_map;

  ସର୍ବସାଧାରଣ:
  auto ଚାବିଗୁଡ଼ିକ () ଅଚଳ HB_AUTO_RETURN (+ back_map.iter())
};

#endif /* HB_BIMAP_HH */
