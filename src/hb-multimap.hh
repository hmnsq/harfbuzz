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
    यदि (multiples.has (k, &m))
    {
      m->push (v);
      फर्काऊ;
    }

    hb_codepoint_t *old_v;
    यदि (singulars.has (k, &old_v))
    {
      hb_codepoint_t old = *old_v;
      singulars.del (k);

      multiples.set (k, hb_vector_t<hb_codepoint_t> {old, v});
      फर्काऊ;
    }

    singulars.set (k, v);
  }

  hb_array_t<अचर hb_codepoint_t> get (hb_codepoint_t k) अचर
  {
    अचर hb_codepoint_t *v;
    यदि (singulars.has (k, &v))
      फर्काऊ hb_array (v, 1);

    hb_vector_t<hb_codepoint_t> *m;
    यदि (multiples.has (k, &m))
      फर्काऊ m->as_array ();

    फर्काऊ hb_array_t<अचर hb_codepoint_t> ();
  }

  तार्किक in_error () अचर
  {
    यदि (singulars.in_error () || multiples.in_error ())
      फर्काऊ सत्य;
    लागि (अचर स्वतः &m : multiples.मानहरू_ref ())
      यदि (m.in_error ())
        फर्काऊ सत्य;
    फर्काऊ असत्य;
  }

  शून्य alloc (अचिन्हित size)
  {
    singulars.alloc (size);
  }

  रक्षित:
  hb_map_t singulars;
  hb_hashmap_t<hb_codepoint_t, hb_vector_t<hb_codepoint_t>> multiples;
};



#endif /* HB_MULTIMAP_HH */
