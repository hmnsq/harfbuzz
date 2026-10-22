/*
 * Copyright © 2022  Google, Inc.
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
 *
 * Google Author(s): Garret Rieger
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "graph.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "../hb-ot-layout-gsubgpos.hh"

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ਗ੍ਰਾਫ_GSUBGPOS_CONTEXT_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਗ੍ਰਾਫ_GSUBGPOS_CONTEXT_HH

ਨਾਮਾਕਾਸ਼ ਗ੍ਰਾਫ {

ਬਣਤਰ Lookup;

ਬਣਤਰ gsubgpos_ਗ੍ਰਾਫ_context_t
{
  HB_ਅੰਦਰੂਨੀ ਸਥਿਰ ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<gsubgpos_ਗ੍ਰਾਫ_context_t> create (hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ_3,
                                                                      ਗ੍ਰਾਫ_t& ਗ੍ਰਾਫ_2);

  hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ_2;
  ਗ੍ਰਾਫ_t& ਗ੍ਰਾਫ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ lookup_list_ਸੂਚਕਾਂਕ;
  hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਗ੍ਰਾਫ::Lookup*> lookups;
  hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਬਿਨਾਨਿਸ਼ਾਨੀ> subtable_ਮੰਜ਼ਿਲ_extension;
  hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ>> ਵੰਡੋ_subtables;


  HB_ਅੰਦਰੂਨੀ ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> create_ਗੰਢ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਮਾਪ);

  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ> ਜੋੜੋ_ਬਫਰ (ਅੱਖਰ* ਬਫਰ)
  {
    ਵਾਪਸ ਗ੍ਰਾਫ.ਜੋੜੋ_ਬਫਰ (ਬਫਰ);
  }

 ਨਿਜੀ:
  HB_ਅੰਦਰੂਨੀ gsubgpos_ਗ੍ਰਾਫ_context_t (hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ_3,
                                        ਗ੍ਰਾਫ_t& ਗ੍ਰਾਫ_2);

  HB_ਅੰਦਰੂਨੀ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_non_ext_subtables ();
};

}

#ਜੇ_ਅੰਤ  // GRAPH_GSUBGPOS_CONTEXT
