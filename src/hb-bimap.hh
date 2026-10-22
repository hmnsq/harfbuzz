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

#اگر_تعریف_نشده HB_BIMAP_HH
#تعریف_کن HB_BIMAP_HH

#ضمن "hb.hh"
#ضمن "hb-map.hh"

/* Bi-directional map */
بنية hb_bimap_t
{
  فراغ reset ()
  {
    forw_map.reset ();
    پشت_map.reset ();
  }

  فراغ alloc (بیعلامت برداشت)
  {
    forw_map.alloc (برداشت);
    پشت_map.alloc (برداشت);
  }

  منطقي درون_error () ثابت { أرجع forw_map.درون_error () || پشت_map.درون_error (); }

  فراغ مجموعه (hb_codepoint_t lhs, hb_codepoint_t rhs)
  {
    إذا (درون_error ()) أرجع;
    إذا (unlikely (lhs == HB_MAP_مقدار_INVALID)) أرجع;
    إذا (unlikely (rhs == HB_MAP_مقدار_INVALID)) { del (lhs); أرجع; }

    forw_map.مجموعه (lhs, rhs);
    إذا (unlikely (درون_error ())) أرجع;

    پشت_map.مجموعه (rhs, lhs);
    إذا (unlikely (درون_error ())) forw_map.del (lhs);
  }

  hb_codepoint_t get (hb_codepoint_t lhs) ثابت { أرجع forw_map.get (lhs); }
  hb_codepoint_t backward (hb_codepoint_t rhs) ثابت { أرجع پشت_map.get (rhs); }

  hb_codepoint_t عملگر [] (hb_codepoint_t lhs) ثابت { أرجع get (lhs); }
  منطقي has (hb_codepoint_t lhs) ثابت { أرجع forw_map.has (lhs); }


  فراغ del (hb_codepoint_t lhs)
  {
    پشت_map.del (get (lhs));
    forw_map.del (lhs);
  }

  فراغ clear ()
  {
    forw_map.clear ();
    پشت_map.clear ();
  }

  منطقي is_empty () ثابت { أرجع forw_map.is_empty (); }

  بیعلامت صحيح get_population () ثابت { أرجع forw_map.get_population (); }

  محمي:
  hb_map_t  forw_map;
  hb_map_t  پشت_map;

  عام:
  خودکار کلیدها () ثابت HB_AUTO_RETURN (+ forw_map.کلیدها())
  خودکار values () ثابت HB_AUTO_RETURN (+ forw_map.values())
  خودکار iter () ثابت HB_AUTO_RETURN (+ forw_map.iter())
};

/* Incremental bimap: only lhs is given, rhs is incrementally assigned */
بنية hb_inc_bimap_t
{
  منطقي درون_error () ثابت { أرجع forw_map.درون_error () || پشت_map.درون_error (); }

  بیعلامت صحيح get_population () ثابت { أرجع forw_map.get_population (); }

  فراغ reset ()
  {
    forw_map.reset ();
    پشت_map.reset ();
  }

  فراغ alloc (بیعلامت برداشت)
  {
    forw_map.alloc (برداشت);
    پشت_map.alloc (برداشت);
  }

  فراغ clear ()
  {
    forw_map.clear ();
    پشت_map.clear ();
  }

  /* Add a mapping from lhs to rhs with a unique value if lhs is unknown.
   * Return the rhs value as the result.
   */
  hb_codepoint_t جمع_کن (hb_codepoint_t lhs)
  {
    hb_codepoint_t  rhs = forw_map[lhs];
    إذا (rhs == HB_MAP_مقدار_INVALID)
    {
      rhs = پشت_map.درازی;
      forw_map.مجموعه (lhs, rhs);
      پشت_map.push (lhs);
    }
    أرجع rhs;
  }

  hb_codepoint_t skip ()
  {
    hb_codepoint_t شروع = پشت_map.درازی;
    پشت_map.push (HB_MAP_مقدار_INVALID);
    أرجع شروع;
  }

  hb_codepoint_t skip (بیعلامت شمار)
  {
    hb_codepoint_t شروع = پشت_map.درازی;
    پشت_map.alloc (پشت_map.درازی + شمار);
    لكل (بیعلامت i = 0; i < شمار; i++)
      پشت_map.push (HB_MAP_مقدار_INVALID);
    أرجع شروع;
  }

  hb_codepoint_t get_بعدی_مقدار () ثابت
  { أرجع پشت_map.درازی; }

  فراغ جمع_کن_مجموعه (ثابت hb_مجموعه_t *مجموعه)
  {
    لكل (خودکار i : *مجموعه) جمع_کن (i);
  }

  /* Create an identity map. */
  منطقي عنصر_خنثی (بیعلامت صحيح اندازه)
  {
    clear ();
    لكل (hb_codepoint_t i = 0; i < اندازه; i++) جمع_کن (i);
    أرجع !درون_error ();
  }

  محمي:
  ساكن صحيح cmp_id (ثابت فراغ* a, ثابت فراغ* b)
  { أرجع (صحيح)*(ثابت hb_codepoint_t *)a - (صحيح)*(ثابت hb_codepoint_t *)b; }

  عام:
  /* Optional: after finished adding all mappings in a random order,
   * reassign rhs to lhs so that they are in the same order. */
  فراغ sort ()
  {
    hb_codepoint_t  شمار = get_population ();
    hb_وکتور_t <hb_codepoint_t> work;
    إذا (unlikely (!work.resize_dirty  (شمار))) أرجع;

    لكل (hb_codepoint_t rhs = 0; rhs < شمار; rhs++)
      work.arrayZ[rhs] = پشت_map[rhs];

    work.qsort (cmp_id);

    clear ();
    لكل (hb_codepoint_t rhs = 0; rhs < شمار; rhs++)
      جمع_کن (work.arrayZ[rhs]);
  }

  hb_codepoint_t get (hb_codepoint_t lhs) ثابت { أرجع forw_map.get (lhs); }
  hb_codepoint_t backward (hb_codepoint_t rhs) ثابت { أرجع پشت_map[rhs]; }

  hb_codepoint_t عملگر [] (hb_codepoint_t lhs) ثابت { أرجع get (lhs); }
  منطقي has (hb_codepoint_t lhs) ثابت { أرجع forw_map.has (lhs); }

  محمي:
  hb_map_t forw_map;
  hb_وکتور_t<hb_codepoint_t> پشت_map;

  عام:
  خودکار کلیدها () ثابت HB_AUTO_RETURN (+ پشت_map.iter())
};

#پایان_اگر /* HB_BIMAP_HH */
