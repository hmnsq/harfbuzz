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

#که_نه_تعریف_شوی HB_OT_LAYOUT_GPOS_جدول_HH
#تعریف_کړه HB_OT_LAYOUT_GPOS_جدول_HH

#ضمن "OT/Layout/GPOS/GPOS.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {

// TODO(garretrieger): Move into new layout directory.
/* Out-of-class implementation for methods recursing */
#که_نه_تعریف_شوی HB_NO_OT_LAYOUT
کينډۍ <ډول_نوم context_t>
/*static*/ ډول_نوم context_t::return_t PosLookup::dispatch_recurse_func (context_t *c, بینښه صحيح lookup_شاخص)
{
  ثابت PosLookup &l = c->face->جدول.GPOS.get_relaxed ()->جدول->get_lookup (lookup_شاخص);
  أرجع l.dispatch (c);
}

کينډۍ <>
مضمن hb_تړون_lookups_context_t::return_t
PosLookup::dispatch_recurse_func<hb_تړون_lookups_context_t> (hb_تړون_lookups_context_t *c, بینښه this_شاخص)
{
  ثابت PosLookup &l = c->face->جدول.GPOS.get_relaxed ()->جدول->get_lookup (this_شاخص);
  أرجع l.تړون_lookups (c, this_شاخص);
}

کينډۍ <>
مضمن منطقي PosLookup::dispatch_recurse_func<hb_ot_apply_context_t> (hb_ot_apply_context_t *c, بینښه صحيح lookup_شاخص)
{
  خپلکاری *gpos = c->face->جدول.GPOS.get_relaxed ();
  ثابت PosLookup &l = gpos->جدول->get_lookup (lookup_شاخص);
  بینښه صحيح saved_lookup_props = c->lookup_props;
  بینښه صحيح saved_lookup_شاخص = c->lookup_شاخص;
  c->سېټ_lookup_شاخص (lookup_شاخص);
  c->سېټ_lookup_props (l.get_props ());

  uint32_t ډېری_سمون_positions[8];
  hb_ویکټور_t<uint32_t> saved_سمون_positions;
  saved_سمون_positions.سېټ_storage (ډېری_سمون_positions);
  hb_بدلون (c->سمون_positions, saved_سمون_positions);

  منطقي ret = خطأ;
  خپلکاری *accel = gpos->get_accel (lookup_شاخص);
  ret = accel && accel->apply (c, خطأ);

  c->سېټ_lookup_شاخص (saved_lookup_شاخص);
  c->سېټ_lookup_props (saved_lookup_props);

  hb_بدلون (c->سمون_positions, saved_سمون_positions);

  أرجع ret;
}
#د_که_پای

} /* namespace GPOS_impl */
} /* namespace Layout */
} /* namespace OT */


#د_که_پای /* HB_OT_LAYOUT_GPOS_TABLE_HH */
