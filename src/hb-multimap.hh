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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_MULTIMAP_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_MULTIMAP_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-map.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-vector.hh"


/*
 * hb_multimap_t
 */

ਬਣਤਰ hb_multimap_t
{
  ਖਾਲੀ ਜੋੜੋ (hb_codepoint_t k, hb_codepoint_t v)
  {
    hb_ਸਦਿਸ਼_t<hb_codepoint_t> *m;
    ਜੇ (multiples.has (k, &m))
    {
      m->push (v);
      ਵਾਪਸ;
    }

    hb_codepoint_t *old_v;
    ਜੇ (singulars.has (k, &old_v))
    {
      hb_codepoint_t old = *old_v;
      singulars.del (k);

      multiples.ਸੈੱਟ (k, hb_ਸਦਿਸ਼_t<hb_codepoint_t> {old, v});
      ਵਾਪਸ;
    }

    singulars.ਸੈੱਟ (k, v);
  }

  hb_array_t<ਅਚਲ hb_codepoint_t> get (hb_codepoint_t k) ਅਚਲ
  {
    ਅਚਲ hb_codepoint_t *v;
    ਜੇ (singulars.has (k, &v))
      ਵਾਪਸ hb_array (v, 1);

    hb_ਸਦਿਸ਼_t<hb_codepoint_t> *m;
    ਜੇ (multiples.has (k, &m))
      ਵਾਪਸ m->as_array ();

    ਵਾਪਸ hb_array_t<ਅਚਲ hb_codepoint_t> ();
  }

  ਤਾਰਕਿਕ ਵਿੱਚ_error () ਅਚਲ
  {
    ਜੇ (singulars.ਵਿੱਚ_error () || multiples.ਵਿੱਚ_error ())
      ਵਾਪਸ ਸੱਚ;
    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ &m : multiples.ਮੁੱਲਾਂ_ref ())
      ਜੇ (m.ਵਿੱਚ_error ())
        ਵਾਪਸ ਸੱਚ;
    ਵਾਪਸ ਝੂਠ;
  }

  ਖਾਲੀ alloc (ਬਿਨਾਨਿਸ਼ਾਨੀ ਮਾਪ)
  {
    singulars.alloc (ਮਾਪ);
  }

  ਸੁਰੱਖਿਅਤ:
  hb_map_t singulars;
  hb_hashmap_t<hb_codepoint_t, hb_ਸਦਿਸ਼_t<hb_codepoint_t>> multiples;
};



#ਜੇ_ਅੰਤ /* HB_MULTIMAP_HH */
