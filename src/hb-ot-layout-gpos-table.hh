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

#ifndef HB_OT_LAYOUT_GPOS_TABLE_HH
#define HB_OT_LAYOUT_GPOS_TABLE_HH

#অন্তর্ভাৱ "OT/Layout/GPOS/GPOS.hh"

নামাকাশ OT {
নামাকাশ Layout {
নামাকাশ GPOS_impl {

// TODO(garretrieger): Move into new layout directory.
/* Out-of-class implementation for methods recursing */
#ifndef HB_NO_OT_LAYOUT
template <typename context_t>
/*static*/ typename context_t::return_t PosLookup::dispatch_recurse_func (context_t *c, unsigned পূর্ণাঙ্ক lookup_index)
{
  ধ্রুৱ PosLookup &l = c->face->table.GPOS.get_relaxed ()->table->get_lookup (lookup_index);
  ঘূৰাই l.dispatch (c);
}

template <>
অন্তঃস্থ hb_closure_lookups_context_t::return_t
PosLookup::dispatch_recurse_func<hb_closure_lookups_context_t> (hb_closure_lookups_context_t *c, unsigned this_index)
{
  ধ্রুৱ PosLookup &l = c->face->table.GPOS.get_relaxed ()->table->get_lookup (this_index);
  ঘূৰাই l.closure_lookups (c, this_index);
}

template <>
অন্তঃস্থ তার্কিক PosLookup::dispatch_recurse_func<hb_ot_apply_context_t> (hb_ot_apply_context_t *c, unsigned পূর্ণাঙ্ক lookup_index)
{
  auto *gpos = c->face->table.GPOS.get_relaxed ();
  ধ্রুৱ PosLookup &l = gpos->table->get_lookup (lookup_index);
  unsigned পূর্ণাঙ্ক saved_lookup_props = c->lookup_props;
  unsigned পূর্ণাঙ্ক saved_lookup_index = c->lookup_index;
  c->set_lookup_index (lookup_index);
  c->set_lookup_props (l.get_props ());

  uint32_t স্তূপ_match_positions[8];
  hb_vector_t<uint32_t> saved_match_positions;
  saved_match_positions.set_storage (স্তূপ_match_positions);
  hb_swap (c->match_positions, saved_match_positions);

  তার্কিক ret = মিছা;
  auto *accel = gpos->get_accel (lookup_index);
  ret = accel && accel->apply (c, মিছা);

  c->set_lookup_index (saved_lookup_index);
  c->set_lookup_props (saved_lookup_props);

  hb_swap (c->match_positions, saved_match_positions);

  ঘূৰাই ret;
}
#endif

} /* namespace GPOS_impl */
} /* namespace Layout */
} /* namespace OT */


#endif /* HB_OT_LAYOUT_GPOS_TABLE_HH */
