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

#اگر_غیر_تعریف_شدہ HB_BIMAP_HH
#تعریف_کرو HB_BIMAP_HH

#شامل "hb.hh"
#شامل "hb-map.hh"

/* Bi-directional map */
ساخت hb_bimap_t
{
  خالی reset ()
  {
    forw_map.reset ();
    عقب_map.reset ();
  }

  خالی alloc (غیرعلامتی نکالو)
  {
    forw_map.alloc (نکالو);
    عقب_map.alloc (نکالو);
  }

  منطقی اندر_error () ثابت { واپس forw_map.اندر_error () || عقب_map.اندر_error (); }

  خالی سیٹ (hb_codepoint_t lhs, hb_codepoint_t rhs)
  {
    اگر (اندر_error ()) واپس;
    اگر (unlikely (lhs == HB_MAP_قدر_INVALID)) واپس;
    اگر (unlikely (rhs == HB_MAP_قدر_INVALID)) { del (lhs); واپس; }

    forw_map.سیٹ (lhs, rhs);
    اگر (unlikely (اندر_error ())) واپس;

    عقب_map.سیٹ (rhs, lhs);
    اگر (unlikely (اندر_error ())) forw_map.del (lhs);
  }

  hb_codepoint_t get (hb_codepoint_t lhs) ثابت { واپس forw_map.get (lhs); }
  hb_codepoint_t backward (hb_codepoint_t rhs) ثابت { واپس عقب_map.get (rhs); }

  hb_codepoint_t عامل [] (hb_codepoint_t lhs) ثابت { واپس get (lhs); }
  منطقی has (hb_codepoint_t lhs) ثابت { واپس forw_map.has (lhs); }


  خالی del (hb_codepoint_t lhs)
  {
    عقب_map.del (get (lhs));
    forw_map.del (lhs);
  }

  خالی clear ()
  {
    forw_map.clear ();
    عقب_map.clear ();
  }

  منطقی is_empty () ثابت { واپس forw_map.is_empty (); }

  غیرعلامتی صحیح get_population () ثابت { واپس forw_map.get_population (); }

  محفوظ:
  hb_map_t  forw_map;
  hb_map_t  عقب_map;

  عوامی:
  خودکار کلیدیں () ثابت HB_AUTO_RETURN (+ forw_map.کلیدیں())
  خودکار values () ثابت HB_AUTO_RETURN (+ forw_map.values())
  خودکار iter () ثابت HB_AUTO_RETURN (+ forw_map.iter())
};

/* Incremental bimap: only lhs is given, rhs is incrementally assigned */
ساخت hb_inc_bimap_t
{
  منطقی اندر_error () ثابت { واپس forw_map.اندر_error () || عقب_map.اندر_error (); }

  غیرعلامتی صحیح get_population () ثابت { واپس forw_map.get_population (); }

  خالی reset ()
  {
    forw_map.reset ();
    عقب_map.reset ();
  }

  خالی alloc (غیرعلامتی نکالو)
  {
    forw_map.alloc (نکالو);
    عقب_map.alloc (نکالو);
  }

  خالی clear ()
  {
    forw_map.clear ();
    عقب_map.clear ();
  }

  /* Add a mapping from lhs to rhs with a unique value if lhs is unknown.
   * Return the rhs value as the result.
   */
  hb_codepoint_t جوڑو (hb_codepoint_t lhs)
  {
    hb_codepoint_t  rhs = forw_map[lhs];
    اگر (rhs == HB_MAP_قدر_INVALID)
    {
      rhs = عقب_map.لمبائی;
      forw_map.سیٹ (lhs, rhs);
      عقب_map.push (lhs);
    }
    واپس rhs;
  }

  hb_codepoint_t skip ()
  {
    hb_codepoint_t آغاز = عقب_map.لمبائی;
    عقب_map.push (HB_MAP_قدر_INVALID);
    واپس آغاز;
  }

  hb_codepoint_t skip (غیرعلامتی گنتی)
  {
    hb_codepoint_t آغاز = عقب_map.لمبائی;
    عقب_map.alloc (عقب_map.لمبائی + گنتی);
    برائے (غیرعلامتی i = 0; i < گنتی; i++)
      عقب_map.push (HB_MAP_قدر_INVALID);
    واپس آغاز;
  }

  hb_codepoint_t get_اگلا_قدر () ثابت
  { واپس عقب_map.لمبائی; }

  خالی جوڑو_سیٹ (ثابت hb_سیٹ_t *سیٹ)
  {
    برائے (خودکار i : *سیٹ) جوڑو (i);
  }

  /* Create an identity map. */
  منطقی شناختی_عنصر (غیرعلامتی صحیح سائز)
  {
    clear ();
    برائے (hb_codepoint_t i = 0; i < سائز; i++) جوڑو (i);
    واپس !اندر_error ();
  }

  محفوظ:
  ساکن صحیح cmp_id (ثابت خالی* a, ثابت خالی* b)
  { واپس (صحیح)*(ثابت hb_codepoint_t *)a - (صحیح)*(ثابت hb_codepoint_t *)b; }

  عوامی:
  /* Optional: after finished adding all mappings in a random order,
   * reassign rhs to lhs so that they are in the same order. */
  خالی sort ()
  {
    hb_codepoint_t  گنتی = get_population ();
    hb_سمتیہ_t <hb_codepoint_t> work;
    اگر (unlikely (!work.resize_dirty  (گنتی))) واپس;

    برائے (hb_codepoint_t rhs = 0; rhs < گنتی; rhs++)
      work.arrayZ[rhs] = عقب_map[rhs];

    work.qsort (cmp_id);

    clear ();
    برائے (hb_codepoint_t rhs = 0; rhs < گنتی; rhs++)
      جوڑو (work.arrayZ[rhs]);
  }

  hb_codepoint_t get (hb_codepoint_t lhs) ثابت { واپس forw_map.get (lhs); }
  hb_codepoint_t backward (hb_codepoint_t rhs) ثابت { واپس عقب_map[rhs]; }

  hb_codepoint_t عامل [] (hb_codepoint_t lhs) ثابت { واپس get (lhs); }
  منطقی has (hb_codepoint_t lhs) ثابت { واپس forw_map.has (lhs); }

  محفوظ:
  hb_map_t forw_map;
  hb_سمتیہ_t<hb_codepoint_t> عقب_map;

  عوامی:
  خودکار کلیدیں () ثابت HB_AUTO_RETURN (+ عقب_map.iter())
};

#اگر_ختم /* HB_BIMAP_HH */
