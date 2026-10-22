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

نطاق ګراف {

ګراف_پایله_t<gsubgpos_ګراف_context_t> gsubgpos_ګراف_context_t::create (hb_نښان_t جدول_نښان_3,
                                                                           ګراف_t& ګراف_2)
{
  gsubgpos_ګراف_context_t context (جدول_نښان_3, ګراف_2);

  إذا (unlikely (جدول_نښان_3 != HB_OT_نښان_GPOS
                && جدول_نښان_3 != HB_OT_نښان_GSUB))
    أرجع Err(INVALID_دلیل);

  TRY_ټاکل (ثابت GSTAR* gstar, ګراف::GSTAR::ګراف_منزل_gstar (context.ګراف));
  TRY(gstar->پیدا_کړه_lookups (context.ګراف, context.lookups));
  TRY_ټاکل (context.lookup_list_شاخص, gstar->get_lookup_list_شاخص (context.ګراف));

  أرجع context;
}

gsubgpos_ګراف_context_t::gsubgpos_ګراف_context_t (hb_نښان_t جدول_نښان_3,
                                                    ګراف_t& ګراف_2)
    : جدول_نښان_2 (جدول_نښان_3),
      ګراف (ګراف_2),
      lookup_list_شاخص (0),
      lookups ()
{}

ګراف_پایله_t<بینښه> gsubgpos_ګراف_context_t::create_غوټه (بینښه اندازه)
{
  حرف* بفر = (حرف*) hb_calloc (1, اندازه);
  إذا (unlikely (!بفر))
    أرجع Err(ویش_FAILURE);

  خپلکاری res = جمع_کړه_بفر (بفر);
  إذا (unlikely (!res.is_ok ())) {
    // Allocation did not get stored for freeing later.
    hb_free (بفر);
    أرجع Err(res.error ());
  }

  أرجع ګراف.new_غوټه (بفر, بفر + اندازه);
}

بینښه gsubgpos_ګراف_context_t::عدد_non_ext_subtables ()  {
  بینښه شمېر = 0;
  لكل (خپلکاری l : lookups.values ())
  {
    إذا (l->is_extension (جدول_نښان_2)) استمر;
    شمېر += l->شمېره_د_subtables ();
  }
  أرجع شمېر;
}

}
