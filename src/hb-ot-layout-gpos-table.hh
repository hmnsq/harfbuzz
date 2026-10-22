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

#جيڪڏهن_وصف_نه_ٿيل HB_OT_LAYOUT_GPOS_جدول_HH
#وصف_ڪريو HB_OT_LAYOUT_GPOS_جدول_HH

#ضمن "OT/Layout/GPOS/GPOS.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {

// TODO(garretrieger): Move into new layout directory.
/* Out-of-class implementation for methods recursing */
#جيڪڏهن_وصف_نه_ٿيل HB_NO_OT_LAYOUT
سانچو <قسم_نالو context_t>
/*static*/ قسم_نالو context_t::return_t PosLookup::dispatch_recurse_func (context_t *c, بنانشان صحيح lookup_اشاريو)
{
  ثابت PosLookup &l = c->face->جدول.GPOS.get_relaxed ()->جدول->get_lookup (lookup_اشاريو);
  أرجع l.dispatch (c);
}

سانچو <>
مضمن hb_گهيرو_lookups_context_t::return_t
PosLookup::dispatch_recurse_func<hb_گهيرو_lookups_context_t> (hb_گهيرو_lookups_context_t *c, بنانشان this_اشاريو)
{
  ثابت PosLookup &l = c->face->جدول.GPOS.get_relaxed ()->جدول->get_lookup (this_اشاريو);
  أرجع l.گهيرو_lookups (c, this_اشاريو);
}

سانچو <>
مضمن منطقي PosLookup::dispatch_recurse_func<hb_ot_apply_context_t> (hb_ot_apply_context_t *c, بنانشان صحيح lookup_اشاريو)
{
  خودڪار *gpos = c->face->جدول.GPOS.get_relaxed ();
  ثابت PosLookup &l = gpos->جدول->get_lookup (lookup_اشاريو);
  بنانشان صحيح saved_lookup_props = c->lookup_props;
  بنانشان صحيح saved_lookup_اشاريو = c->lookup_اشاريو;
  c->سيٽ_lookup_اشاريو (lookup_اشاريو);
  c->سيٽ_lookup_props (l.get_props ());

  uint32_t ڍير_ميلاپ_positions[8];
  hb_ويڪٽر_t<uint32_t> saved_ميلاپ_positions;
  saved_ميلاپ_positions.سيٽ_storage (ڍير_ميلاپ_positions);
  hb_مٽا_سٽا (c->ميلاپ_positions, saved_ميلاپ_positions);

  منطقي ret = خطأ;
  خودڪار *accel = gpos->get_accel (lookup_اشاريو);
  ret = accel && accel->apply (c, خطأ);

  c->سيٽ_lookup_اشاريو (saved_lookup_اشاريو);
  c->سيٽ_lookup_props (saved_lookup_props);

  hb_مٽا_سٽا (c->ميلاپ_positions, saved_ميلاپ_positions);

  أرجع ret;
}
#جيڪڏهن_پورو

} /* namespace GPOS_impl */
} /* namespace Layout */
} /* namespace OT */


#جيڪڏهن_پورو /* HB_OT_LAYOUT_GPOS_TABLE_HH */
