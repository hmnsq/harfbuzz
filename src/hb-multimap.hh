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

#ifndef HB_MULTIMAP_HH
#define HB_MULTIMAP_HH

#অন্তর্ভাব "hb.hh"
#অন্তর্ভাব "hb-map.hh"
#অন্তর্ভাব "hb-vector.hh"


/*
 * hb_multimap_t
 */

কাঠামো hb_multimap_t
{
  শূন্য add (hb_codepoint_t k, hb_codepoint_t v)
  {
    hb_vector_t<hb_codepoint_t> *m;
    যদি (multiples.has (k, &m))
    {
      m->push (v);
      ফেরত;
    }

    hb_codepoint_t *old_v;
    যদি (singulars.has (k, &old_v))
    {
      hb_codepoint_t old = *old_v;
      singulars.del (k);

      multiples.set (k, hb_vector_t<hb_codepoint_t> {old, v});
      ফেরত;
    }

    singulars.set (k, v);
  }

  hb_array_t<ধ্রুবক hb_codepoint_t> get (hb_codepoint_t k) ধ্রুবক
  {
    ধ্রুবক hb_codepoint_t *v;
    যদি (singulars.has (k, &v))
      ফেরত hb_array (v, 1);

    hb_vector_t<hb_codepoint_t> *m;
    যদি (multiples.has (k, &m))
      ফেরত m->as_array ();

    ফেরত hb_array_t<ধ্রুবক hb_codepoint_t> ();
  }

  যৌক্তিক in_error () ধ্রুবক
  {
    যদি (singulars.in_error () || multiples.in_error ())
      ফেরত সত্য;
    জন্য (ধ্রুবক auto &m : multiples.মানসমূহ_ref ())
      যদি (m.in_error ())
        ফেরত সত্য;
    ফেরত মিথ্যা;
  }

  শূন্য alloc (unsigned আকার_2)
  {
    singulars.alloc (আকার_2);
  }

  সুরক্ষিত:
  hb_map_t singulars;
  hb_hashmap_t<hb_codepoint_t, hb_vector_t<hb_codepoint_t>> multiples;
};



#endif /* HB_MULTIMAP_HH */
