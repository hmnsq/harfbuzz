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

#شامل "gsubgpos-graph.hh"

نامخلا گراف {

گراف_نتیجہ_t<gsubgpos_گراف_context_t> gsubgpos_گراف_context_t::create (hb_لیبل_t جدول_لیبل_3,
                                                                           گراف_t& گراف_2)
{
  gsubgpos_گراف_context_t context (جدول_لیبل_3, گراف_2);

  اگر (unlikely (جدول_لیبل_3 != HB_OT_لیبل_GPOS
                && جدول_لیبل_3 != HB_OT_لیبل_GSUB))
    واپس Err(INVALID_دلیل);

  TRY_تفویض (ثابت GSTAR* gstar, گراف::GSTAR::گراف_مقصد_gstar (context.گراف));
  TRY(gstar->ڈھونڈو_lookups (context.گراف, context.lookups));
  TRY_تفویض (context.lookup_list_اشاریہ, gstar->get_lookup_list_اشاریہ (context.گراف));

  واپس context;
}

gsubgpos_گراف_context_t::gsubgpos_گراف_context_t (hb_لیبل_t جدول_لیبل_3,
                                                    گراف_t& گراف_2)
    : جدول_لیبل_2 (جدول_لیبل_3),
      گراف (گراف_2),
      lookup_list_اشاریہ (0),
      lookups ()
{}

گراف_نتیجہ_t<غیرعلامتی> gsubgpos_گراف_context_t::create_گرہ (غیرعلامتی سائز)
{
  حرف* بفر = (حرف*) hb_calloc (1, سائز);
  اگر (unlikely (!بفر))
    واپس Err(تخصیص_FAILURE);

  خودکار res = جوڑو_بفر (بفر);
  اگر (unlikely (!res.is_ok ())) {
    // Allocation did not get stored for freeing later.
    hb_free (بفر);
    واپس Err(res.error ());
  }

  واپس گراف.new_گرہ (بفر, بفر + سائز);
}

غیرعلامتی gsubgpos_گراف_context_t::عدد_non_ext_subtables ()  {
  غیرعلامتی گنتی = 0;
  برائے (خودکار l : lookups.values ())
  {
    اگر (l->is_extension (جدول_لیبل_2)) جاری;
    گنتی += l->نمبر_کا_subtables ();
  }
  واپس گنتی;
}

}
