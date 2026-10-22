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

#جے_بنا_تعریف HB_BIMAP_HH
#تعریف_کرو HB_BIMAP_HH

#ضمن "hb.hh"
#ضمن "hb-map.hh"

/* Bi-directional map */
بنية hb_bimap_t
{
  فراغ reset ()
  {
    forw_map.reset ();
    پٹھ_map.reset ();
  }

  فراغ alloc (بنانشانی نکالو)
  {
    forw_map.alloc (نکالو);
    پٹھ_map.alloc (نکالو);
  }

  منطقي وچ_error () ثابت { أرجع forw_map.وچ_error () || پٹھ_map.وچ_error (); }

  فراغ سیٹ (hb_codepoint_t lhs, hb_codepoint_t rhs)
  {
    إذا (وچ_error ()) أرجع;
    إذا (unlikely (lhs == HB_MAP_قدر_INVALID)) أرجع;
    إذا (unlikely (rhs == HB_MAP_قدر_INVALID)) { del (lhs); أرجع; }

    forw_map.سیٹ (lhs, rhs);
    إذا (unlikely (وچ_error ())) أرجع;

    پٹھ_map.سیٹ (rhs, lhs);
    إذا (unlikely (وچ_error ())) forw_map.del (lhs);
  }

  hb_codepoint_t get (hb_codepoint_t lhs) ثابت { أرجع forw_map.get (lhs); }
  hb_codepoint_t backward (hb_codepoint_t rhs) ثابت { أرجع پٹھ_map.get (rhs); }

  hb_codepoint_t عامل [] (hb_codepoint_t lhs) ثابت { أرجع get (lhs); }
  منطقي has (hb_codepoint_t lhs) ثابت { أرجع forw_map.has (lhs); }


  فراغ del (hb_codepoint_t lhs)
  {
    پٹھ_map.del (get (lhs));
    forw_map.del (lhs);
  }

  فراغ clear ()
  {
    forw_map.clear ();
    پٹھ_map.clear ();
  }

  منطقي is_empty () ثابت { أرجع forw_map.is_empty (); }

  بنانشانی صحيح get_population () ثابت { أرجع forw_map.get_population (); }

  محمي:
  hb_map_t  forw_map;
  hb_map_t  پٹھ_map;

  عام:
  خودکار کلیدیں () ثابت HB_AUTO_RETURN (+ forw_map.کلیدیں())
  خودکار values () ثابت HB_AUTO_RETURN (+ forw_map.values())
  خودکار iter () ثابت HB_AUTO_RETURN (+ forw_map.iter())
};

/* Incremental bimap: only lhs is given, rhs is incrementally assigned */
بنية hb_inc_bimap_t
{
  منطقي وچ_error () ثابت { أرجع forw_map.وچ_error () || پٹھ_map.وچ_error (); }

  بنانشانی صحيح get_population () ثابت { أرجع forw_map.get_population (); }

  فراغ reset ()
  {
    forw_map.reset ();
    پٹھ_map.reset ();
  }

  فراغ alloc (بنانشانی نکالو)
  {
    forw_map.alloc (نکالو);
    پٹھ_map.alloc (نکالو);
  }

  فراغ clear ()
  {
    forw_map.clear ();
    پٹھ_map.clear ();
  }

  /* Add a mapping from lhs to rhs with a unique value if lhs is unknown.
   * Return the rhs value as the result.
   */
  hb_codepoint_t جوڑو (hb_codepoint_t lhs)
  {
    hb_codepoint_t  rhs = forw_map[lhs];
    إذا (rhs == HB_MAP_قدر_INVALID)
    {
      rhs = پٹھ_map.لمائی;
      forw_map.سیٹ (lhs, rhs);
      پٹھ_map.push (lhs);
    }
    أرجع rhs;
  }

  hb_codepoint_t skip ()
  {
    hb_codepoint_t شروع = پٹھ_map.لمائی;
    پٹھ_map.push (HB_MAP_قدر_INVALID);
    أرجع شروع;
  }

  hb_codepoint_t skip (بنانشانی گنتی)
  {
    hb_codepoint_t شروع = پٹھ_map.لمائی;
    پٹھ_map.alloc (پٹھ_map.لمائی + گنتی);
    لكل (بنانشانی i = 0; i < گنتی; i++)
      پٹھ_map.push (HB_MAP_قدر_INVALID);
    أرجع شروع;
  }

  hb_codepoint_t get_اگلا_قدر () ثابت
  { أرجع پٹھ_map.لمائی; }

  فراغ جوڑو_سیٹ (ثابت hb_سیٹ_t *سیٹ)
  {
    لكل (خودکار i : *سیٹ) جوڑو (i);
  }

  /* Create an identity map. */
  منطقي پچھان_تت (بنانشانی صحيح ناپ)
  {
    clear ();
    لكل (hb_codepoint_t i = 0; i < ناپ; i++) جوڑو (i);
    أرجع !وچ_error ();
  }

  محمي:
  ساكن صحيح cmp_id (ثابت فراغ* a, ثابت فراغ* b)
  { أرجع (صحيح)*(ثابت hb_codepoint_t *)a - (صحيح)*(ثابت hb_codepoint_t *)b; }

  عام:
  /* Optional: after finished adding all mappings in a random order,
   * reassign rhs to lhs so that they are in the same order. */
  فراغ sort ()
  {
    hb_codepoint_t  گنتی = get_population ();
    hb_سمتیہ_t <hb_codepoint_t> work;
    إذا (unlikely (!work.resize_dirty  (گنتی))) أرجع;

    لكل (hb_codepoint_t rhs = 0; rhs < گنتی; rhs++)
      work.arrayZ[rhs] = پٹھ_map[rhs];

    work.qsort (cmp_id);

    clear ();
    لكل (hb_codepoint_t rhs = 0; rhs < گنتی; rhs++)
      جوڑو (work.arrayZ[rhs]);
  }

  hb_codepoint_t get (hb_codepoint_t lhs) ثابت { أرجع forw_map.get (lhs); }
  hb_codepoint_t backward (hb_codepoint_t rhs) ثابت { أرجع پٹھ_map[rhs]; }

  hb_codepoint_t عامل [] (hb_codepoint_t lhs) ثابت { أرجع get (lhs); }
  منطقي has (hb_codepoint_t lhs) ثابت { أرجع forw_map.has (lhs); }

  محمي:
  hb_map_t forw_map;
  hb_سمتیہ_t<hb_codepoint_t> پٹھ_map;

  عام:
  خودکار کلیدیں () ثابت HB_AUTO_RETURN (+ پٹھ_map.iter())
};

#جے_انت /* HB_BIMAP_HH */
