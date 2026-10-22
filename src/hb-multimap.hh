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

#నిర్వచితం_కాకపోతే HB_MULTIMAP_HH
#నిర్వచించు HB_MULTIMAP_HH

#అన్తర్భావ "hb.hh"
#అన్తర్భావ "hb-map.hh"
#అన్తర్భావ "hb-vector.hh"


/*
 * hb_multimap_t
 */

నిర్మాణం hb_multimap_t
{
  శూన్య కూడు (hb_codepoint_t k, hb_codepoint_t v)
  {
    hb_సదిశ_t<hb_codepoint_t> *m;
    ఒకవేళ (multiples.has (k, &m))
    {
      m->push (v);
      తిరిగి;
    }

    hb_codepoint_t *old_v;
    ఒకవేళ (singulars.has (k, &old_v))
    {
      hb_codepoint_t old = *old_v;
      singulars.del (k);

      multiples.సమితి (k, hb_సదిశ_t<hb_codepoint_t> {old, v});
      తిరిగి;
    }

    singulars.సమితి (k, v);
  }

  hb_array_t<స్థిరరాశి hb_codepoint_t> get (hb_codepoint_t k) స్థిరరాశి
  {
    స్థిరరాశి hb_codepoint_t *v;
    ఒకవేళ (singulars.has (k, &v))
      తిరిగి hb_array (v, 1);

    hb_సదిశ_t<hb_codepoint_t> *m;
    ఒకవేళ (multiples.has (k, &m))
      తిరిగి m->as_array ();

    తిరిగి hb_array_t<స్థిరరాశి hb_codepoint_t> ();
  }

  తార్కిక లోపల_error () స్థిరరాశి
  {
    ఒకవేళ (singulars.లోపల_error () || multiples.లోపల_error ())
      తిరిగి నిజం;
    కొరకు (స్థిరరాశి స్వయంచాలక &m : multiples.విలువలు_ref ())
      ఒకవేళ (m.లోపల_error ())
        తిరిగి నిజం;
    తిరిగి అబద్ధం;
  }

  శూన్య alloc (గుర్తులేని కొలత)
  {
    singulars.alloc (కొలత);
  }

  రక్షిత:
  hb_map_t singulars;
  hb_hashmap_t<hb_codepoint_t, hb_సదిశ_t<hb_codepoint_t>> multiples;
};



#అయితే_ముగింపు /* HB_MULTIMAP_HH */
