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

#ଅନ୍ତର୍ଭାଵ "hb.hh"
#ଅନ୍ତର୍ଭାଵ "hb-map.hh"
#ଅନ୍ତର୍ଭାଵ "hb-vector.hh"


/*
 * hb_multimap_t
 */

ସଂରଚନା hb_multimap_t
{
  ଶୂନ୍ୟ add (hb_codepoint_t k, hb_codepoint_t v)
  {
    hb_vector_t<hb_codepoint_t> *m;
    ଯଦି (multiples.has (k, &m))
    {
      m->push (v);
      ଫେରନ୍ତୁ;
    }

    hb_codepoint_t *old_v;
    ଯଦି (singulars.has (k, &old_v))
    {
      hb_codepoint_t old = *old_v;
      singulars.del (k);

      multiples.set (k, hb_vector_t<hb_codepoint_t> {old, v});
      ଫେରନ୍ତୁ;
    }

    singulars.set (k, v);
  }

  hb_array_t<ଅଚଳ hb_codepoint_t> get (hb_codepoint_t k) ଅଚଳ
  {
    ଅଚଳ hb_codepoint_t *v;
    ଯଦି (singulars.has (k, &v))
      ଫେରନ୍ତୁ hb_array (v, 1);

    hb_vector_t<hb_codepoint_t> *m;
    ଯଦି (multiples.has (k, &m))
      ଫେରନ୍ତୁ m->as_array ();

    ଫେରନ୍ତୁ hb_array_t<ଅଚଳ hb_codepoint_t> ();
  }

  ତାର୍କିକ in_error () ଅଚଳ
  {
    ଯଦି (singulars.in_error () || multiples.in_error ())
      ଫେରନ୍ତୁ ସତ୍ୟ;
    ପାଇଁ (ଅଚଳ auto &m : multiples.ମୂଲ୍ୟଗୁଡ଼ିକ_ref ())
      ଯଦି (m.in_error ())
        ଫେରନ୍ତୁ ସତ୍ୟ;
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }

  ଶୂନ୍ୟ alloc (unsigned size)
  {
    singulars.alloc (size);
  }

  ସୁରକ୍ଷିତ:
  hb_map_t singulars;
  hb_hashmap_t<hb_codepoint_t, hb_vector_t<hb_codepoint_t>> multiples;
};



#endif /* HB_MULTIMAP_HH */
