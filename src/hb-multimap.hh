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

#ಅನ್ತರ್ಭಾವ "hb.hh"
#ಅನ್ತರ್ಭಾವ "hb-map.hh"
#ಅನ್ತರ್ಭಾವ "hb-vector.hh"


/*
 * hb_multimap_t
 */

ಸಂರಚನಾ hb_multimap_t
{
  ಶೂನ್ಯ add (hb_codepoint_t k, hb_codepoint_t v)
  {
    hb_vector_t<hb_codepoint_t> *m;
    ಯದಿ (multiples.has (k, &m))
    {
      m->push (v);
      ಪ್ರತ್ಯಾಗಮ;
    }

    hb_codepoint_t *old_v;
    ಯದಿ (singulars.has (k, &old_v))
    {
      hb_codepoint_t old = *old_v;
      singulars.del (k);

      multiples.set (k, hb_vector_t<hb_codepoint_t> {old, v});
      ಪ್ರತ್ಯಾಗಮ;
    }

    singulars.set (k, v);
  }

  hb_array_t<ಧ್ರುವ hb_codepoint_t> get (hb_codepoint_t k) ಧ್ರುವ
  {
    ಧ್ರುವ hb_codepoint_t *v;
    ಯದಿ (singulars.has (k, &v))
      ಪ್ರತ್ಯಾಗಮ hb_array (v, 1);

    hb_vector_t<hb_codepoint_t> *m;
    ಯದಿ (multiples.has (k, &m))
      ಪ್ರತ್ಯಾಗಮ m->as_array ();

    ಪ್ರತ್ಯಾಗಮ hb_array_t<ಧ್ರುವ hb_codepoint_t> ();
  }

  ತಾರ್ಕಿಕ in_error () ಧ್ರುವ
  {
    ಯದಿ (singulars.in_error () || multiples.in_error ())
      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    ಪ್ರತಿ (ಧ್ರುವ auto &m : multiples.ಮೌಲ್ಯಗಳು_ref ())
      ಯದಿ (m.in_error ())
        ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }

  ಶೂನ್ಯ alloc (unsigned size)
  {
    singulars.alloc (size);
  }

  ರಕ್ಷಿತ:
  hb_map_t singulars;
  hb_hashmap_t<hb_codepoint_t, hb_vector_t<hb_codepoint_t>> multiples;
};



#endif /* HB_MULTIMAP_HH */
