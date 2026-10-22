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

#جيڪڏهن_وصف_نه_ٿيل HB_BIMAP_HH
#وصف_ڪريو HB_BIMAP_HH

#ضمن "hb.hh"
#ضمن "hb-map.hh"

/* Bi-directional map */
بنية hb_bimap_t
{
  فراغ reset ()
  {
    forw_map.reset ();
    پٺ_map.reset ();
  }

  فراغ alloc (بنانشان ڪڍو)
  {
    forw_map.alloc (ڪڍو);
    پٺ_map.alloc (ڪڍو);
  }

  منطقي اندريون_error () ثابت { أرجع forw_map.اندريون_error () || پٺ_map.اندريون_error (); }

  فراغ سيٽ (hb_codepoint_t lhs, hb_codepoint_t rhs)
  {
    إذا (اندريون_error ()) أرجع;
    إذا (unlikely (lhs == HB_MAP_قدر_INVALID)) أرجع;
    إذا (unlikely (rhs == HB_MAP_قدر_INVALID)) { del (lhs); أرجع; }

    forw_map.سيٽ (lhs, rhs);
    إذا (unlikely (اندريون_error ())) أرجع;

    پٺ_map.سيٽ (rhs, lhs);
    إذا (unlikely (اندريون_error ())) forw_map.del (lhs);
  }

  hb_codepoint_t get (hb_codepoint_t lhs) ثابت { أرجع forw_map.get (lhs); }
  hb_codepoint_t backward (hb_codepoint_t rhs) ثابت { أرجع پٺ_map.get (rhs); }

  hb_codepoint_t عامل [] (hb_codepoint_t lhs) ثابت { أرجع get (lhs); }
  منطقي has (hb_codepoint_t lhs) ثابت { أرجع forw_map.has (lhs); }


  فراغ del (hb_codepoint_t lhs)
  {
    پٺ_map.del (get (lhs));
    forw_map.del (lhs);
  }

  فراغ clear ()
  {
    forw_map.clear ();
    پٺ_map.clear ();
  }

  منطقي is_empty () ثابت { أرجع forw_map.is_empty (); }

  بنانشان صحيح get_population () ثابت { أرجع forw_map.get_population (); }

  محمي:
  hb_map_t  forw_map;
  hb_map_t  پٺ_map;

  عام:
  خودڪار ڪنجيون () ثابت HB_AUTO_RETURN (+ forw_map.ڪنجيون())
  خودڪار values () ثابت HB_AUTO_RETURN (+ forw_map.values())
  خودڪار iter () ثابت HB_AUTO_RETURN (+ forw_map.iter())
};

/* Incremental bimap: only lhs is given, rhs is incrementally assigned */
بنية hb_inc_bimap_t
{
  منطقي اندريون_error () ثابت { أرجع forw_map.اندريون_error () || پٺ_map.اندريون_error (); }

  بنانشان صحيح get_population () ثابت { أرجع forw_map.get_population (); }

  فراغ reset ()
  {
    forw_map.reset ();
    پٺ_map.reset ();
  }

  فراغ alloc (بنانشان ڪڍو)
  {
    forw_map.alloc (ڪڍو);
    پٺ_map.alloc (ڪڍو);
  }

  فراغ clear ()
  {
    forw_map.clear ();
    پٺ_map.clear ();
  }

  /* Add a mapping from lhs to rhs with a unique value if lhs is unknown.
   * Return the rhs value as the result.
   */
  hb_codepoint_t جوڙيو (hb_codepoint_t lhs)
  {
    hb_codepoint_t  rhs = forw_map[lhs];
    إذا (rhs == HB_MAP_قدر_INVALID)
    {
      rhs = پٺ_map.ڊيگهائي;
      forw_map.سيٽ (lhs, rhs);
      پٺ_map.push (lhs);
    }
    أرجع rhs;
  }

  hb_codepoint_t skip ()
  {
    hb_codepoint_t شروعات = پٺ_map.ڊيگهائي;
    پٺ_map.push (HB_MAP_قدر_INVALID);
    أرجع شروعات;
  }

  hb_codepoint_t skip (بنانشان ڳڻپ)
  {
    hb_codepoint_t شروعات = پٺ_map.ڊيگهائي;
    پٺ_map.alloc (پٺ_map.ڊيگهائي + ڳڻپ);
    لكل (بنانشان i = 0; i < ڳڻپ; i++)
      پٺ_map.push (HB_MAP_قدر_INVALID);
    أرجع شروعات;
  }

  hb_codepoint_t get_ايندڙ_قدر () ثابت
  { أرجع پٺ_map.ڊيگهائي; }

  فراغ جوڙيو_سيٽ (ثابت hb_سيٽ_t *سيٽ)
  {
    لكل (خودڪار i : *سيٽ) جوڙيو (i);
  }

  /* Create an identity map. */
  منطقي سڃاڻپ_عنصر (بنانشان صحيح سائيز)
  {
    clear ();
    لكل (hb_codepoint_t i = 0; i < سائيز; i++) جوڙيو (i);
    أرجع !اندريون_error ();
  }

  محمي:
  ساكن صحيح cmp_id (ثابت فراغ* a, ثابت فراغ* b)
  { أرجع (صحيح)*(ثابت hb_codepoint_t *)a - (صحيح)*(ثابت hb_codepoint_t *)b; }

  عام:
  /* Optional: after finished adding all mappings in a random order,
   * reassign rhs to lhs so that they are in the same order. */
  فراغ sort ()
  {
    hb_codepoint_t  ڳڻپ = get_population ();
    hb_ويڪٽر_t <hb_codepoint_t> work;
    إذا (unlikely (!work.resize_dirty  (ڳڻپ))) أرجع;

    لكل (hb_codepoint_t rhs = 0; rhs < ڳڻپ; rhs++)
      work.arrayZ[rhs] = پٺ_map[rhs];

    work.qsort (cmp_id);

    clear ();
    لكل (hb_codepoint_t rhs = 0; rhs < ڳڻپ; rhs++)
      جوڙيو (work.arrayZ[rhs]);
  }

  hb_codepoint_t get (hb_codepoint_t lhs) ثابت { أرجع forw_map.get (lhs); }
  hb_codepoint_t backward (hb_codepoint_t rhs) ثابت { أرجع پٺ_map[rhs]; }

  hb_codepoint_t عامل [] (hb_codepoint_t lhs) ثابت { أرجع get (lhs); }
  منطقي has (hb_codepoint_t lhs) ثابت { أرجع forw_map.has (lhs); }

  محمي:
  hb_map_t forw_map;
  hb_ويڪٽر_t<hb_codepoint_t> پٺ_map;

  عام:
  خودڪار ڪنجيون () ثابت HB_AUTO_RETURN (+ پٺ_map.iter())
};

#جيڪڏهن_پورو /* HB_BIMAP_HH */
