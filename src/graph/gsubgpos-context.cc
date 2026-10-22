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

#ਅਨ੍ਤਰ੍ਭਾਵ "gsubgpos-graph.hh"

ਨਾਮਾਕਾਸ਼ ਗ੍ਰਾਫ {

ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<gsubgpos_ਗ੍ਰਾਫ_context_t> gsubgpos_ਗ੍ਰਾਫ_context_t::create (hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ_3,
                                                                           ਗ੍ਰਾਫ_t& ਗ੍ਰਾਫ_2)
{
  gsubgpos_ਗ੍ਰਾਫ_context_t context (ਸਾਰਣੀ_ਟੈਗ_3, ਗ੍ਰਾਫ_2);

  ਜੇ (unlikely (ਸਾਰਣੀ_ਟੈਗ_3 != HB_OT_ਟੈਗ_GPOS
                && ਸਾਰਣੀ_ਟੈਗ_3 != HB_OT_ਟੈਗ_GSUB))
    ਵਾਪਸ Err(INVALID_ਦਲੀਲ);

  TRY_ਸੌਂਪੋ (ਅਚਲ GSTAR* gstar, ਗ੍ਰਾਫ::GSTAR::ਗ੍ਰਾਫ_ਮੰਜ਼ਿਲ_gstar (context.ਗ੍ਰਾਫ));
  TRY(gstar->ਲੱਭੋ_lookups (context.ਗ੍ਰਾਫ, context.lookups));
  TRY_ਸੌਂਪੋ (context.lookup_list_ਸੂਚਕਾਂਕ, gstar->get_lookup_list_ਸੂਚਕਾਂਕ (context.ਗ੍ਰਾਫ));

  ਵਾਪਸ context;
}

gsubgpos_ਗ੍ਰਾਫ_context_t::gsubgpos_ਗ੍ਰਾਫ_context_t (hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ_3,
                                                    ਗ੍ਰਾਫ_t& ਗ੍ਰਾਫ_2)
    : ਸਾਰਣੀ_ਟੈਗ_2 (ਸਾਰਣੀ_ਟੈਗ_3),
      ਗ੍ਰਾਫ (ਗ੍ਰਾਫ_2),
      lookup_list_ਸੂਚਕਾਂਕ (0),
      lookups ()
{}

ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> gsubgpos_ਗ੍ਰਾਫ_context_t::create_ਗੰਢ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਮਾਪ)
{
  ਅੱਖਰ* ਬਫਰ = (ਅੱਖਰ*) hb_calloc (1, ਮਾਪ);
  ਜੇ (unlikely (!ਬਫਰ))
    ਵਾਪਸ Err(ਵੰਡ_FAILURE);

  ਸਵੈਚਾਲਿਤ res = ਜੋੜੋ_ਬਫਰ (ਬਫਰ);
  ਜੇ (unlikely (!res.is_ok ())) {
    // Allocation did not get stored for freeing later.
    hb_free (ਬਫਰ);
    ਵਾਪਸ Err(res.error ());
  }

  ਵਾਪਸ ਗ੍ਰਾਫ.new_ਗੰਢ (ਬਫਰ, ਬਫਰ + ਮਾਪ);
}

ਬਿਨਾਨਿਸ਼ਾਨੀ gsubgpos_ਗ੍ਰਾਫ_context_t::ਅੰਕ_non_ext_subtables ()  {
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = 0;
  ਲਈ (ਸਵੈਚਾਲਿਤ l : lookups.values ())
  {
    ਜੇ (l->is_extension (ਸਾਰਣੀ_ਟੈਗ_2)) ਜਾਰੀ;
    ਗਿਣਤੀ_2 += l->ਸੰਖਿਆ_ਦਾ_subtables ();
  }
  ਵਾਪਸ ਗਿਣਤੀ_2;
}

}
