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

#അന്തര്ഭാവ "hb.hh"
#അന്തര്ഭാവ "hb-map.hh"
#അന്തര്ഭാവ "hb-vector.hh"


/*
 * hb_multimap_t
 */

ഘടന hb_multimap_t
{
  ശൂന്യം add (hb_codepoint_t k, hb_codepoint_t v)
  {
    hb_vector_t<hb_codepoint_t> *m;
    എങ്കിൽ (multiples.has (k, &m))
    {
      m->push (v);
      തിരികെ;
    }

    hb_codepoint_t *old_v;
    എങ്കിൽ (singulars.has (k, &old_v))
    {
      hb_codepoint_t old = *old_v;
      singulars.del (k);

      multiples.set (k, hb_vector_t<hb_codepoint_t> {old, v});
      തിരികെ;
    }

    singulars.set (k, v);
  }

  hb_array_t<സ്ഥിരം hb_codepoint_t> get (hb_codepoint_t k) സ്ഥിരം
  {
    സ്ഥിരം hb_codepoint_t *v;
    എങ്കിൽ (singulars.has (k, &v))
      തിരികെ hb_array (v, 1);

    hb_vector_t<hb_codepoint_t> *m;
    എങ്കിൽ (multiples.has (k, &m))
      തിരികെ m->as_array ();

    തിരികെ hb_array_t<സ്ഥിരം hb_codepoint_t> ();
  }

  തർക്കം in_error () സ്ഥിരം
  {
    എങ്കിൽ (singulars.in_error () || multiples.in_error ())
      തിരികെ സത്യം;
    വേണ്ടി (സ്ഥിരം auto &m : multiples.മൂല്യങ്ങൾ_ref ())
      എങ്കിൽ (m.in_error ())
        തിരികെ സത്യം;
    തിരികെ അസത്യം;
  }

  ശൂന്യം alloc (unsigned size)
  {
    singulars.alloc (size);
  }

  സംരക്ഷിത:
  hb_map_t singulars;
  hb_hashmap_t<hb_codepoint_t, hb_vector_t<hb_codepoint_t>> multiples;
};



#endif /* HB_MULTIMAP_HH */
