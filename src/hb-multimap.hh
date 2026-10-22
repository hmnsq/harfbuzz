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

#अन्तर्भाव "hb.hh"
#अन्तर्भाव "hb-map.hh"
#अन्तर्भाव "hb-vector.hh"


/*
 * hb_multimap_t
 */

संरचना hb_multimap_t
{
  शून्य add (hb_codepoint_t k, hb_codepoint_t v)
  {
    hb_vector_t<hb_codepoint_t> *m;
    सचे (multiples.has (k, &m))
    {
      m->push (v);
      पटिनिवत्त;
    }

    hb_codepoint_t *old_v;
    सचे (singulars.has (k, &old_v))
    {
      hb_codepoint_t old = *old_v;
      singulars.del (k);

      multiples.set (k, hb_vector_t<hb_codepoint_t> {old, v});
      पटिनिवत्त;
    }

    singulars.set (k, v);
  }

  hb_array_t<ध्रुव hb_codepoint_t> get (hb_codepoint_t k) ध्रुव
  {
    ध्रुव hb_codepoint_t *v;
    सचे (singulars.has (k, &v))
      पटिनिवत्त hb_array (v, 1);

    hb_vector_t<hb_codepoint_t> *m;
    सचे (multiples.has (k, &m))
      पटिनिवत्त m->as_array ();

    पटिनिवत्त hb_array_t<ध्रुव hb_codepoint_t> ();
  }

  तार्किक in_error () ध्रुव
  {
    सचे (singulars.in_error () || multiples.in_error ())
      पटिनिवत्त सच्च;
    अत्थाय (ध्रुव auto &m : multiples.मानानि_ref ())
      सचे (m.in_error ())
        पटिनिवत्त सच्च;
    पटिनिवत्त मिच्छा;
  }

  शून्य alloc (unsigned size)
  {
    singulars.alloc (size);
  }

  रक्षित:
  hb_map_t singulars;
  hb_hashmap_t<hb_codepoint_t, hb_vector_t<hb_codepoint_t>> multiples;
};



#endif /* HB_MULTIMAP_HH */
