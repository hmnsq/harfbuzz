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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OT_LAYOUT_GPOS_ਸਾਰਣੀ_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_LAYOUT_GPOS_ਸਾਰਣੀ_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "OT/Layout/GPOS/GPOS.hh"

ਨਾਮਾਕਾਸ਼ OT {
ਨਾਮਾਕਾਸ਼ Layout {
ਨਾਮਾਕਾਸ਼ GPOS_impl {

// TODO(garretrieger): Move into new layout directory.
/* Out-of-class implementation for methods recursing */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_LAYOUT
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ context_t>
/*static*/ ਕਿਸਮ_ਨਾਂ context_t::return_t PosLookup::dispatch_recurse_func (context_t *c, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ lookup_ਸੂਚਕਾਂਕ)
{
  ਅਚਲ PosLookup &l = c->face->ਸਾਰਣੀ.GPOS.get_relaxed ()->ਸਾਰਣੀ->get_lookup (lookup_ਸੂਚਕਾਂਕ);
  ਵਾਪਸ l.dispatch (c);
}

ਸਾਂਚਾ <>
ਸਤਰਅੰਦਰ hb_ਘੇਰਾ_lookups_context_t::return_t
PosLookup::dispatch_recurse_func<hb_ਘੇਰਾ_lookups_context_t> (hb_ਘੇਰਾ_lookups_context_t *c, ਬਿਨਾਨਿਸ਼ਾਨੀ this_ਸੂਚਕਾਂਕ)
{
  ਅਚਲ PosLookup &l = c->face->ਸਾਰਣੀ.GPOS.get_relaxed ()->ਸਾਰਣੀ->get_lookup (this_ਸੂਚਕਾਂਕ);
  ਵਾਪਸ l.ਘੇਰਾ_lookups (c, this_ਸੂਚਕਾਂਕ);
}

ਸਾਂਚਾ <>
ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ PosLookup::dispatch_recurse_func<hb_ot_apply_context_t> (hb_ot_apply_context_t *c, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ lookup_ਸੂਚਕਾਂਕ)
{
  ਸਵੈਚਾਲਿਤ *gpos = c->face->ਸਾਰਣੀ.GPOS.get_relaxed ();
  ਅਚਲ PosLookup &l = gpos->ਸਾਰਣੀ->get_lookup (lookup_ਸੂਚਕਾਂਕ);
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ saved_lookup_props = c->lookup_props;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ saved_lookup_ਸੂਚਕਾਂਕ = c->lookup_ਸੂਚਕਾਂਕ;
  c->ਸੈੱਟ_lookup_ਸੂਚਕਾਂਕ (lookup_ਸੂਚਕਾਂਕ);
  c->ਸੈੱਟ_lookup_props (l.get_props ());

  uint32_t ਢੇਰ_ਮੇਲ_positions[8];
  hb_ਸਦਿਸ਼_t<uint32_t> saved_ਮੇਲ_positions;
  saved_ਮੇਲ_positions.ਸੈੱਟ_storage (ਢੇਰ_ਮੇਲ_positions);
  hb_ਅਦਲਾ_ਬਦਲੀ (c->ਮੇਲ_positions, saved_ਮੇਲ_positions);

  ਤਾਰਕਿਕ ret = ਝੂਠ;
  ਸਵੈਚਾਲਿਤ *accel = gpos->get_accel (lookup_ਸੂਚਕਾਂਕ);
  ret = accel && accel->apply (c, ਝੂਠ);

  c->ਸੈੱਟ_lookup_ਸੂਚਕਾਂਕ (saved_lookup_ਸੂਚਕਾਂਕ);
  c->ਸੈੱਟ_lookup_props (saved_lookup_props);

  hb_ਅਦਲਾ_ਬਦਲੀ (c->ਮੇਲ_positions, saved_ਮੇਲ_positions);

  ਵਾਪਸ ret;
}
#ਜੇ_ਅੰਤ

} /* namespace GPOS_impl */
} /* namespace Layout */
} /* namespace OT */


#ਜੇ_ਅੰਤ /* HB_OT_LAYOUT_GPOS_TABLE_HH */
