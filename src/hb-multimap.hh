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

#અન્તર્ભાવ "hb.hh"
#અન્તર્ભાવ "hb-map.hh"
#અન્તર્ભાવ "hb-vector.hh"


/*
 * hb_multimap_t
 */

સંરચના hb_multimap_t
{
  શૂન્ય add (hb_codepoint_t k, hb_codepoint_t v)
  {
    hb_vector_t<hb_codepoint_t> *m;
    જો (multiples.has (k, &m))
    {
      m->push (v);
      પાછા;
    }

    hb_codepoint_t *old_v;
    જો (singulars.has (k, &old_v))
    {
      hb_codepoint_t old = *old_v;
      singulars.del (k);

      multiples.set (k, hb_vector_t<hb_codepoint_t> {old, v});
      પાછા;
    }

    singulars.set (k, v);
  }

  hb_array_t<અચલ hb_codepoint_t> get (hb_codepoint_t k) અચલ
  {
    અચલ hb_codepoint_t *v;
    જો (singulars.has (k, &v))
      પાછા hb_array (v, 1);

    hb_vector_t<hb_codepoint_t> *m;
    જો (multiples.has (k, &m))
      પાછા m->as_array ();

    પાછા hb_array_t<અચલ hb_codepoint_t> ();
  }

  તાર્કિક in_error () અચલ
  {
    જો (singulars.in_error () || multiples.in_error ())
      પાછા સાચું;
    માટે (અચલ auto &m : multiples.કિંમતો_ref ())
      જો (m.in_error ())
        પાછા સાચું;
    પાછા ખોટું;
  }

  શૂન્ય alloc (unsigned size)
  {
    singulars.alloc (size);
  }

  સુરક્ષિત:
  hb_map_t singulars;
  hb_hashmap_t<hb_codepoint_t, hb_vector_t<hb_codepoint_t>> multiples;
};



#endif /* HB_MULTIMAP_HH */
