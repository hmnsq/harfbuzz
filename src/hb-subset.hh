/*
 * Copyright © 2018  Google, Inc.
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
 * Google Author(s): Garret Rieger, Roderick Sheeter
 */

#నిర్వచితం_కాకపోతే HB_SUBSET_HH
#నిర్వచించు HB_SUBSET_HH


#అన్తర్భావ "hb.hh"

#అన్తర్భావ "hb-subset.h"

#అన్తర్భావ "hb-machinery.hh"
#అన్తర్భావ "hb-serialize.hh"
#అన్తర్భావ "hb-subset-input.hh"
#అన్తర్భావ "hb-subset-plan.hh"

నిర్మాణం hb_subset_context_t :
       hb_dispatch_context_t<hb_subset_context_t, తార్కిక, HB_DEBUG_SUBSET>
{
  స్థిరరాశి అక్షరం *get_name () { తిరిగి "SUBSET"; }
  స్థిర return_t default_return_విలువ () { తిరిగి నిజం; }

  అంతరంగ:
  మూస <రకం_పేరు T, రకం_పేరు ...Ts> స్వయంచాలక
  _dispatch (స్థిరరాశి T &obj, hb_ప్రాధాన్యత<1>, Ts&&... ds) HB_AUTO_RETURN
  ( obj.subset (ఇది, std::forward<Ts> (ds)...) )
  మూస <రకం_పేరు T, రకం_పేరు ...Ts> స్వయంచాలక
  _dispatch (స్థిరరాశి T &obj, hb_ప్రాధాన్యత<0>, Ts&&... ds) HB_AUTO_RETURN
  ( obj.dispatch (ఇది, std::forward<Ts> (ds)...) )
  బహిరంగ:
  మూస <రకం_పేరు T, రకం_పేరు ...Ts> స్వయంచాలక
  dispatch (స్థిరరాశి T &obj, Ts&&... ds) HB_AUTO_RETURN
  ( _dispatch (obj, hb_prioritize, std::forward<Ts> (ds)...) )

  hb_blob_t *ఆధారం_blob;
  hb_subset_plan_t *plan;
  hb_serialize_context_t *serializer;
  hb_ట్యాగ్_t పట్టిక_ట్యాగ్_2;

  hb_subset_context_t (hb_blob_t *ఆధారం_blob_2,
		       hb_subset_plan_t *plan_,
		       hb_serialize_context_t *serializer_,
		       hb_ట్యాగ్_t పట్టిక_ట్యాగ్_3) :
		        ఆధారం_blob (ఆధారం_blob_2),
			plan (plan_),
			serializer (serializer_),
			పట్టిక_ట్యాగ్_2 (పట్టిక_ట్యాగ్_3) {}
};

#అయితే_ముగింపు /* HB_SUBSET_HH */
