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

#నిర్వచితం_కాకపోతే HB_OT_LAYOUT_GPOS_పట్టిక_HH
#నిర్వచించు HB_OT_LAYOUT_GPOS_పట్టిక_HH

#అన్తర్భావ "OT/Layout/GPOS/GPOS.hh"

నామాకాశ OT {
నామాకాశ Layout {
నామాకాశ GPOS_impl {

// TODO(garretrieger): Move into new layout directory.
/* Out-of-class implementation for methods recursing */
#నిర్వచితం_కాకపోతే HB_NO_OT_LAYOUT
మూస <రకం_పేరు context_t>
/*static*/ రకం_పేరు context_t::return_t PosLookup::dispatch_recurse_func (context_t *c, గుర్తులేని పూర్ణాంకం lookup_సూచీ)
{
  స్థిరరాశి PosLookup &l = c->face->పట్టిక.GPOS.get_relaxed ()->పట్టిక->get_lookup (lookup_సూచీ);
  తిరిగి l.dispatch (c);
}

మూస <>
అంతఃవరుస hb_సంవృతం_lookups_context_t::return_t
PosLookup::dispatch_recurse_func<hb_సంవృతం_lookups_context_t> (hb_సంవృతం_lookups_context_t *c, గుర్తులేని this_సూచీ)
{
  స్థిరరాశి PosLookup &l = c->face->పట్టిక.GPOS.get_relaxed ()->పట్టిక->get_lookup (this_సూచీ);
  తిరిగి l.సంవృతం_lookups (c, this_సూచీ);
}

మూస <>
అంతఃవరుస తార్కిక PosLookup::dispatch_recurse_func<hb_ot_apply_context_t> (hb_ot_apply_context_t *c, గుర్తులేని పూర్ణాంకం lookup_సూచీ)
{
  స్వయంచాలక *gpos = c->face->పట్టిక.GPOS.get_relaxed ();
  స్థిరరాశి PosLookup &l = gpos->పట్టిక->get_lookup (lookup_సూచీ);
  గుర్తులేని పూర్ణాంకం saved_lookup_props = c->lookup_props;
  గుర్తులేని పూర్ణాంకం saved_lookup_సూచీ = c->lookup_సూచీ;
  c->సమితి_lookup_సూచీ (lookup_సూచీ);
  c->సమితి_lookup_props (l.get_props ());

  uint32_t దొంతర_సరిపోలిక_positions[8];
  hb_సదిశ_t<uint32_t> saved_సరిపోలిక_positions;
  saved_సరిపోలిక_positions.సమితి_storage (దొంతర_సరిపోలిక_positions);
  hb_మార్పిడి (c->సరిపోలిక_positions, saved_సరిపోలిక_positions);

  తార్కిక ret = అబద్ధం;
  స్వయంచాలక *accel = gpos->get_accel (lookup_సూచీ);
  ret = accel && accel->apply (c, అబద్ధం);

  c->సమితి_lookup_సూచీ (saved_lookup_సూచీ);
  c->సమితి_lookup_props (saved_lookup_props);

  hb_మార్పిడి (c->సరిపోలిక_positions, saved_సరిపోలిక_positions);

  తిరిగి ret;
}
#అయితే_ముగింపు

} /* namespace GPOS_impl */
} /* namespace Layout */
} /* namespace OT */


#అయితే_ముగింపు /* HB_OT_LAYOUT_GPOS_TABLE_HH */
