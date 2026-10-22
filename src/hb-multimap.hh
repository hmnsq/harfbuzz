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

#அந்தர்பாவ "hb.hh"
#அந்தர்பாவ "hb-map.hh"
#அந்தர்பாவ "hb-vector.hh"


/*
 * hb_multimap_t
 */

கட்டமைப்பு hb_multimap_t
{
  வெறுமை add (hb_codepoint_t k, hb_codepoint_t v)
  {
    hb_vector_t<hb_codepoint_t> *m;
    எனில் (multiples.has (k, &m))
    {
      m->push (v);
      திரும்பு;
    }

    hb_codepoint_t *old_v;
    எனில் (singulars.has (k, &old_v))
    {
      hb_codepoint_t old = *old_v;
      singulars.del (k);

      multiples.set (k, hb_vector_t<hb_codepoint_t> {old, v});
      திரும்பு;
    }

    singulars.set (k, v);
  }

  hb_array_t<மாறிலி hb_codepoint_t> get (hb_codepoint_t k) மாறிலி
  {
    மாறிலி hb_codepoint_t *v;
    எனில் (singulars.has (k, &v))
      திரும்பு hb_array (v, 1);

    hb_vector_t<hb_codepoint_t> *m;
    எனில் (multiples.has (k, &m))
      திரும்பு m->as_array ();

    திரும்பு hb_array_t<மாறிலி hb_codepoint_t> ();
  }

  தருக்க in_error () மாறிலி
  {
    எனில் (singulars.in_error () || multiples.in_error ())
      திரும்பு உண்மை;
    க்கு (மாறிலி auto &m : multiples.மதிப்புகள்_ref ())
      எனில் (m.in_error ())
        திரும்பு உண்மை;
    திரும்பு பொய்;
  }

  வெறுமை alloc (unsigned size)
  {
    singulars.alloc (size);
  }

  பாதுகாக்கப்பட்ட:
  hb_map_t singulars;
  hb_hashmap_t<hb_codepoint_t, hb_vector_t<hb_codepoint_t>> multiples;
};



#endif /* HB_MULTIMAP_HH */
