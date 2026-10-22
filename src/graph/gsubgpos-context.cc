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

#ضمن "gsubgpos-graph.hh"

نطاق گراف {

گراف_نتیجہ_t<gsubgpos_گراف_context_t> gsubgpos_گراف_context_t::create (hb_ٹیگ_t جدول_ٹیگ_3,
                                                                           گراف_t& گراف_2)
{
  gsubgpos_گراف_context_t context (جدول_ٹیگ_3, گراف_2);

  إذا (unlikely (جدول_ٹیگ_3 != HB_OT_ٹیگ_GPOS
                && جدول_ٹیگ_3 != HB_OT_ٹیگ_GSUB))
    أرجع Err(INVALID_دلیل);

  TRY_سونپو (ثابت GSTAR* gstar, گراف::GSTAR::گراف_منزل_gstar (context.گراف));
  TRY(gstar->لبھو_lookups (context.گراف, context.lookups));
  TRY_سونپو (context.lookup_list_اشاریہ, gstar->get_lookup_list_اشاریہ (context.گراف));

  أرجع context;
}

gsubgpos_گراف_context_t::gsubgpos_گراف_context_t (hb_ٹیگ_t جدول_ٹیگ_3,
                                                    گراف_t& گراف_2)
    : جدول_ٹیگ_2 (جدول_ٹیگ_3),
      گراف (گراف_2),
      lookup_list_اشاریہ (0),
      lookups ()
{}

گراف_نتیجہ_t<بنانشانی> gsubgpos_گراف_context_t::create_گرہ (بنانشانی ناپ)
{
  حرف* بفر = (حرف*) hb_calloc (1, ناپ);
  إذا (unlikely (!بفر))
    أرجع Err(ونڈ_FAILURE);

  خودکار res = جوڑو_بفر (بفر);
  إذا (unlikely (!res.is_ok ())) {
    // Allocation did not get stored for freeing later.
    hb_free (بفر);
    أرجع Err(res.error ());
  }

  أرجع گراف.new_گرہ (بفر, بفر + ناپ);
}

بنانشانی gsubgpos_گراف_context_t::عدد_non_ext_subtables ()  {
  بنانشانی گنتی = 0;
  لكل (خودکار l : lookups.values ())
  {
    إذا (l->is_extension (جدول_ٹیگ_2)) استمر;
    گنتی += l->نمبر_دا_subtables ();
  }
  أرجع گنتی;
}

}
