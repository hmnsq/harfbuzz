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

گراف_نتيجو_t<gsubgpos_گراف_context_t> gsubgpos_گراف_context_t::create (hb_ٽيگ_t جدول_ٽيگ_3,
                                                                           گراف_t& گراف_2)
{
  gsubgpos_گراف_context_t context (جدول_ٽيگ_3, گراف_2);

  إذا (unlikely (جدول_ٽيگ_3 != HB_OT_ٽيگ_GPOS
                && جدول_ٽيگ_3 != HB_OT_ٽيگ_GSUB))
    أرجع Err(INVALID_دليل);

  TRY_سونپيو (ثابت GSTAR* gstar, گراف::GSTAR::گراف_منزل_gstar (context.گراف));
  TRY(gstar->ڳوليو_lookups (context.گراف, context.lookups));
  TRY_سونپيو (context.lookup_list_اشاريو, gstar->get_lookup_list_اشاريو (context.گراف));

  أرجع context;
}

gsubgpos_گراف_context_t::gsubgpos_گراف_context_t (hb_ٽيگ_t جدول_ٽيگ_3,
                                                    گراف_t& گراف_2)
    : جدول_ٽيگ_2 (جدول_ٽيگ_3),
      گراف (گراف_2),
      lookup_list_اشاريو (0),
      lookups ()
{}

گراف_نتيجو_t<بنانشان> gsubgpos_گراف_context_t::create_ڳنڍ (بنانشان سائيز)
{
  حرف* بفر = (حرف*) hb_calloc (1, سائيز);
  إذا (unlikely (!بفر))
    أرجع Err(ورهاست_FAILURE);

  خودڪار res = جوڙيو_بفر (بفر);
  إذا (unlikely (!res.is_ok ())) {
    // Allocation did not get stored for freeing later.
    hb_free (بفر);
    أرجع Err(res.error ());
  }

  أرجع گراف.new_ڳنڍ (بفر, بفر + سائيز);
}

بنانشان gsubgpos_گراف_context_t::انگ_non_ext_subtables ()  {
  بنانشان ڳڻپ = 0;
  لكل (خودڪار l : lookups.values ())
  {
    إذا (l->is_extension (جدول_ٽيگ_2)) استمر;
    ڳڻپ += l->عدد_جو_subtables ();
  }
  أرجع ڳڻپ;
}

}
