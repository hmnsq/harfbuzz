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

#که_نه_تعریف_شوی HB_MULTIMAP_HH
#تعریف_کړه HB_MULTIMAP_HH

#ضمن "hb.hh"
#ضمن "hb-map.hh"
#ضمن "hb-vector.hh"


/*
 * hb_multimap_t
 */

بنية hb_multimap_t
{
  فراغ جمع_کړه (hb_codepoint_t k, hb_codepoint_t v)
  {
    hb_ویکټور_t<hb_codepoint_t> *m;
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

      multiples.سېټ (k, hb_ویکټور_t<hb_codepoint_t> {old, v});
      أرجع;
    }

    singulars.سېټ (k, v);
  }

  hb_array_t<ثابت hb_codepoint_t> get (hb_codepoint_t k) ثابت
  {
    ثابت hb_codepoint_t *v;
    إذا (singulars.has (k, &v))
      أرجع hb_array (v, 1);

    hb_ویکټور_t<hb_codepoint_t> *m;
    إذا (multiples.has (k, &m))
      أرجع m->as_array ();

    أرجع hb_array_t<ثابت hb_codepoint_t> ();
  }

  منطقي دننه_error () ثابت
  {
    إذا (singulars.دننه_error () || multiples.دننه_error ())
      أرجع ریښتیا;
    لكل (ثابت خپلکاری &m : multiples.ارزښتونه_ref ())
      إذا (m.دننه_error ())
        أرجع ریښتیا;
    أرجع خطأ;
  }

  فراغ alloc (بینښه اندازه)
  {
    singulars.alloc (اندازه);
  }

  محمي:
  hb_map_t singulars;
  hb_hashmap_t<hb_codepoint_t, hb_ویکټور_t<hb_codepoint_t>> multiples;
};



#د_که_پای /* HB_MULTIMAP_HH */
