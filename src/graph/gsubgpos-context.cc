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

گراف_نتیجه_t<gsubgpos_گراف_context_t> gsubgpos_گراف_context_t::create (hb_برچسب_t جدول_برچسب_3,
                                                                           گراف_t& گراف_2)
{
  gsubgpos_گراف_context_t context (جدول_برچسب_3, گراف_2);

  إذا (unlikely (جدول_برچسب_3 != HB_OT_برچسب_GPOS
                && جدول_برچسب_3 != HB_OT_برچسب_GSUB))
    أرجع Err(INVALID_آرگومان);

  TRY_تخصیص_بده (ثابت GSTAR* gstar, گراف::GSTAR::گراف_مقصد_gstar (context.گراف));
  TRY(gstar->پیدا_کن_lookups (context.گراف, context.lookups));
  TRY_تخصیص_بده (context.lookup_list_شاخص, gstar->get_lookup_list_شاخص (context.گراف));

  أرجع context;
}

gsubgpos_گراف_context_t::gsubgpos_گراف_context_t (hb_برچسب_t جدول_برچسب_3,
                                                    گراف_t& گراف_2)
    : جدول_برچسب_2 (جدول_برچسب_3),
      گراف (گراف_2),
      lookup_list_شاخص (0),
      lookups ()
{}

گراف_نتیجه_t<بیعلامت> gsubgpos_گراف_context_t::create_گره (بیعلامت اندازه)
{
  حرف* میانگیر = (حرف*) hb_calloc (1, اندازه);
  إذا (unlikely (!میانگیر))
    أرجع Err(تخصیص_FAILURE);

  خودکار res = جمع_کن_میانگیر (میانگیر);
  إذا (unlikely (!res.is_ok ())) {
    // Allocation did not get stored for freeing later.
    hb_free (میانگیر);
    أرجع Err(res.error ());
  }

  أرجع گراف.new_گره (میانگیر, میانگیر + اندازه);
}

بیعلامت gsubgpos_گراف_context_t::رقم_non_ext_subtables ()  {
  بیعلامت شمار = 0;
  لكل (خودکار l : lookups.values ())
  {
    إذا (l->is_extension (جدول_برچسب_2)) استمر;
    شمار += l->عدد_از_subtables ();
  }
  أرجع شمار;
}

}
