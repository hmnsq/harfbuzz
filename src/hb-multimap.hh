/*
 * Copyright © 2022  Behdad Esfahbod
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
 */

#جيڪڏهن_وصف_نه_ٿيل HB_MULTIMAP_HH
#وصف_ڪريو HB_MULTIMAP_HH

#ضمن "hb.hh"
#ضمن "hb-map.hh"
#ضمن "hb-vector.hh"


/*
 * hb_multimap_t
 */

بنية hb_multimap_t
{
  فراغ جوڙيو (hb_codepoint_t k, hb_codepoint_t v)
  {
    hb_ويڪٽر_t<hb_codepoint_t> *m;
    إذا (multiples.has (k, &m))
    {
      m->push (v);
      أرجع;
    }

    hb_codepoint_t *old_v;
    إذا (singulars.has (k, &old_v))
    {
      hb_codepoint_t old = *old_v;
      singulars.del (k);

      multiples.سيٽ (k, hb_ويڪٽر_t<hb_codepoint_t> {old, v});
      أرجع;
    }

    singulars.سيٽ (k, v);
  }

  hb_array_t<ثابت hb_codepoint_t> get (hb_codepoint_t k) ثابت
  {
    ثابت hb_codepoint_t *v;
    إذا (singulars.has (k, &v))
      أرجع hb_array (v, 1);

    hb_ويڪٽر_t<hb_codepoint_t> *m;
    إذا (multiples.has (k, &m))
      أرجع m->as_array ();

    أرجع hb_array_t<ثابت hb_codepoint_t> ();
  }

  منطقي اندريون_error () ثابت
  {
    إذا (singulars.اندريون_error () || multiples.اندريون_error ())
      أرجع سچ;
    لكل (ثابت خودڪار &m : multiples.قدرون_ref ())
      إذا (m.اندريون_error ())
        أرجع سچ;
    أرجع خطأ;
  }

  فراغ alloc (بنانشان سائيز)
  {
    singulars.alloc (سائيز);
  }

  محمي:
  hb_map_t singulars;
  hb_hashmap_t<hb_codepoint_t, hb_ويڪٽر_t<hb_codepoint_t>> multiples;
};



#جيڪڏهن_پورو /* HB_MULTIMAP_HH */
