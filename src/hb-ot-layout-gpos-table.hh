/*
 * Copyright © 2007,2008,2009,2010  Red Hat, Inc.
 * Copyright © 2010,2012,2013  Google, Inc.
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
 * Red Hat Author(s): Behdad Esfahbod
 * Google Author(s): Behdad Esfahbod
 */

#جے_بنا_تعریف HB_OT_LAYOUT_GPOS_جدول_HH
#تعریف_کرو HB_OT_LAYOUT_GPOS_جدول_HH

#ضمن "OT/Layout/GPOS/GPOS.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {

// TODO(garretrieger): Move into new layout directory.
/* Out-of-class implementation for methods recursing */
#جے_بنا_تعریف HB_NO_OT_LAYOUT
سانچا <قسم_ناں context_t>
/*static*/ قسم_ناں context_t::return_t PosLookup::dispatch_recurse_func (context_t *c, بنانشانی صحيح lookup_اشاریہ)
{
  ثابت PosLookup &l = c->face->جدول.GPOS.get_relaxed ()->جدول->get_lookup (lookup_اشاریہ);
  أرجع l.dispatch (c);
}

سانچا <>
مضمن hb_گھیرا_lookups_context_t::return_t
PosLookup::dispatch_recurse_func<hb_گھیرا_lookups_context_t> (hb_گھیرا_lookups_context_t *c, بنانشانی this_اشاریہ)
{
  ثابت PosLookup &l = c->face->جدول.GPOS.get_relaxed ()->جدول->get_lookup (this_اشاریہ);
  أرجع l.گھیرا_lookups (c, this_اشاریہ);
}

سانچا <>
مضمن منطقي PosLookup::dispatch_recurse_func<hb_ot_apply_context_t> (hb_ot_apply_context_t *c, بنانشانی صحيح lookup_اشاریہ)
{
  خودکار *gpos = c->face->جدول.GPOS.get_relaxed ();
  ثابت PosLookup &l = gpos->جدول->get_lookup (lookup_اشاریہ);
  بنانشانی صحيح saved_lookup_props = c->lookup_props;
  بنانشانی صحيح saved_lookup_اشاریہ = c->lookup_اشاریہ;
  c->سیٹ_lookup_اشاریہ (lookup_اشاریہ);
  c->سیٹ_lookup_props (l.get_props ());

  uint32_t ڈھیر_میل_positions[8];
  hb_سمتیہ_t<uint32_t> saved_میل_positions;
  saved_میل_positions.سیٹ_storage (ڈھیر_میل_positions);
  hb_وٹاندرا (c->میل_positions, saved_میل_positions);

  منطقي ret = خطأ;
  خودکار *accel = gpos->get_accel (lookup_اشاریہ);
  ret = accel && accel->apply (c, خطأ);

  c->سیٹ_lookup_اشاریہ (saved_lookup_اشاریہ);
  c->سیٹ_lookup_props (saved_lookup_props);

  hb_وٹاندرا (c->میل_positions, saved_میل_positions);

  أرجع ret;
}
#جے_انت

} /* namespace GPOS_impl */
} /* namespace Layout */
} /* namespace OT */


#جے_انت /* HB_OT_LAYOUT_GPOS_TABLE_HH */
