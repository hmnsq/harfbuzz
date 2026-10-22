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

#که_نه_تعریف_شوی HB_BIMAP_HH
#تعریف_کړه HB_BIMAP_HH

#ضمن "hb.hh"
#ضمن "hb-map.hh"

/* Bi-directional map */
بنية hb_bimap_t
{
  فراغ reset ()
  {
    forw_map.reset ();
    شا_map.reset ();
  }

  فراغ alloc (بینښه راوباسه)
  {
    forw_map.alloc (راوباسه);
    شا_map.alloc (راوباسه);
  }

  منطقي دننه_error () ثابت { أرجع forw_map.دننه_error () || شا_map.دننه_error (); }

  فراغ سېټ (hb_codepoint_t lhs, hb_codepoint_t rhs)
  {
    إذا (دننه_error ()) أرجع;
    إذا (unlikely (lhs == HB_MAP_ارزښت_INVALID)) أرجع;
    إذا (unlikely (rhs == HB_MAP_ارزښت_INVALID)) { del (lhs); أرجع; }

    forw_map.سېټ (lhs, rhs);
    إذا (unlikely (دننه_error ())) أرجع;

    شا_map.سېټ (rhs, lhs);
    إذا (unlikely (دننه_error ())) forw_map.del (lhs);
  }

  hb_codepoint_t get (hb_codepoint_t lhs) ثابت { أرجع forw_map.get (lhs); }
  hb_codepoint_t backward (hb_codepoint_t rhs) ثابت { أرجع شا_map.get (rhs); }

  hb_codepoint_t چلوونکی [] (hb_codepoint_t lhs) ثابت { أرجع get (lhs); }
  منطقي has (hb_codepoint_t lhs) ثابت { أرجع forw_map.has (lhs); }


  فراغ del (hb_codepoint_t lhs)
  {
    شا_map.del (get (lhs));
    forw_map.del (lhs);
  }

  فراغ clear ()
  {
    forw_map.clear ();
    شا_map.clear ();
  }

  منطقي is_empty () ثابت { أرجع forw_map.is_empty (); }

  بینښه صحيح get_population () ثابت { أرجع forw_map.get_population (); }

  محمي:
  hb_map_t  forw_map;
  hb_map_t  شا_map;

  عام:
  خپلکاری کلیګانې () ثابت HB_AUTO_RETURN (+ forw_map.کلیګانې())
  خپلکاری values () ثابت HB_AUTO_RETURN (+ forw_map.values())
  خپلکاری iter () ثابت HB_AUTO_RETURN (+ forw_map.iter())
};

/* Incremental bimap: only lhs is given, rhs is incrementally assigned */
بنية hb_inc_bimap_t
{
  منطقي دننه_error () ثابت { أرجع forw_map.دننه_error () || شا_map.دننه_error (); }

  بینښه صحيح get_population () ثابت { أرجع forw_map.get_population (); }

  فراغ reset ()
  {
    forw_map.reset ();
    شا_map.reset ();
  }

  فراغ alloc (بینښه راوباسه)
  {
    forw_map.alloc (راوباسه);
    شا_map.alloc (راوباسه);
  }

  فراغ clear ()
  {
    forw_map.clear ();
    شا_map.clear ();
  }

  /* Add a mapping from lhs to rhs with a unique value if lhs is unknown.
   * Return the rhs value as the result.
   */
  hb_codepoint_t جمع_کړه (hb_codepoint_t lhs)
  {
    hb_codepoint_t  rhs = forw_map[lhs];
    إذا (rhs == HB_MAP_ارزښت_INVALID)
    {
      rhs = شا_map.اوږدتیا;
      forw_map.سېټ (lhs, rhs);
      شا_map.push (lhs);
    }
    أرجع rhs;
  }

  hb_codepoint_t skip ()
  {
    hb_codepoint_t پیل = شا_map.اوږدتیا;
    شا_map.push (HB_MAP_ارزښت_INVALID);
    أرجع پیل;
  }

  hb_codepoint_t skip (بینښه شمېر)
  {
    hb_codepoint_t پیل = شا_map.اوږدتیا;
    شا_map.alloc (شا_map.اوږدتیا + شمېر);
    لكل (بینښه i = 0; i < شمېر; i++)
      شا_map.push (HB_MAP_ارزښت_INVALID);
    أرجع پیل;
  }

  hb_codepoint_t get_بل_ارزښت () ثابت
  { أرجع شا_map.اوږدتیا; }

  فراغ جمع_کړه_سېټ (ثابت hb_سېټ_t *سېټ)
  {
    لكل (خپلکاری i : *سېټ) جمع_کړه (i);
  }

  /* Create an identity map. */
  منطقي بې_اغېزه_عنصر (بینښه صحيح اندازه)
  {
    clear ();
    لكل (hb_codepoint_t i = 0; i < اندازه; i++) جمع_کړه (i);
    أرجع !دننه_error ();
  }

  محمي:
  ساكن صحيح cmp_id (ثابت فراغ* a, ثابت فراغ* b)
  { أرجع (صحيح)*(ثابت hb_codepoint_t *)a - (صحيح)*(ثابت hb_codepoint_t *)b; }

  عام:
  /* Optional: after finished adding all mappings in a random order,
   * reassign rhs to lhs so that they are in the same order. */
  فراغ sort ()
  {
    hb_codepoint_t  شمېر = get_population ();
    hb_ویکټور_t <hb_codepoint_t> work;
    إذا (unlikely (!work.resize_dirty  (شمېر))) أرجع;

    لكل (hb_codepoint_t rhs = 0; rhs < شمېر; rhs++)
      work.arrayZ[rhs] = شا_map[rhs];

    work.qsort (cmp_id);

    clear ();
    لكل (hb_codepoint_t rhs = 0; rhs < شمېر; rhs++)
      جمع_کړه (work.arrayZ[rhs]);
  }

  hb_codepoint_t get (hb_codepoint_t lhs) ثابت { أرجع forw_map.get (lhs); }
  hb_codepoint_t backward (hb_codepoint_t rhs) ثابت { أرجع شا_map[rhs]; }

  hb_codepoint_t چلوونکی [] (hb_codepoint_t lhs) ثابت { أرجع get (lhs); }
  منطقي has (hb_codepoint_t lhs) ثابت { أرجع forw_map.has (lhs); }

  محمي:
  hb_map_t forw_map;
  hb_ویکټور_t<hb_codepoint_t> شا_map;

  عام:
  خپلکاری کلیګانې () ثابت HB_AUTO_RETURN (+ شا_map.iter())
};

#د_که_پای /* HB_BIMAP_HH */
