/*
 * Copyright © 2023  Google, Inc.
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
 */

#నిర్వచితం_కాకపోతే HB_OT_VAR_CVAR_పట్టిక_HH
#నిర్వచించు HB_OT_VAR_CVAR_పట్టిక_HH

#అన్తర్భావ "hb-ot-var-common.hh"
#అన్తర్భావ "hb-ot-var-fvar-table.hh"


నామాకాశ OT {
/*
 * cvar -- control value table (CVT) Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cvar
 */
#నిర్వచించు HB_OT_ట్యాగ్_cvar HB_ట్యాగ్('c','v','a','r')

నిర్మాణం cvar
{
  స్థిర constexpr hb_ట్యాగ్_t పట్టిక_ట్యాగ్ = HB_OT_ట్యాగ్_cvar;

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) &&
		  hb_barrier () &&
		  likely (సంస్కరణ.major == 1) &&
		  tuple_Variation_దత్తాంశం.sanitize (c));
  }

  స్థిరరాశి Tuple_Variation_దత్తాంశం<>* get_tuple_var_దత్తాంశం (శూన్య) స్థిరరాశి
  { తిరిగి &tuple_Variation_దత్తాంశం; }

  తార్కిక decompile_tuple_variations (గుర్తులేని axis_లెక్క,
                                   గుర్తులేని బిందువు_లెక్క,
                                   hb_blob_t *blob,
                                   తార్కిక is_gvar,
                                   స్థిరరాశి hb_map_t *axes_old_సూచీ_ట్యాగ్_map,
                                   Tuple_Variation_దత్తాంశం<>::tuple_variations_t& tuple_variations /*బహిర్గతం*/) స్థిరరాశి
  {
    hb_సదిశ_t<గుర్తులేని> shared_indices;
    Tuple_Variation_దత్తాంశం<>::tuple_iterator_t iterator;
    hb_బైట్లు_t var_దత్తాంశం_బైట్లు = blob->as_బైట్లు ().sub_array (4);
    ఒకవేళ (!Tuple_Variation_దత్తాంశం<>::get_tuple_iterator (var_దత్తాంశం_బైట్లు, axis_లెక్క, ఇది,
                                                 shared_indices, &iterator))
      తిరిగి అబద్ధం;

    తిరిగి tuple_Variation_దత్తాంశం.decompile_tuple_variations (బిందువు_లెక్క, is_gvar, iterator,
                                                          axes_old_సూచీ_ట్యాగ్_map,
                                                          shared_indices,
                                                          hb_array<స్థిరరాశి F2DOT14> (),
                                                          tuple_variations);
  }

  స్థిర తార్కిక calculate_cvt_deltas (గుర్తులేని axis_లెక్క,
                                    hb_array_t<పూర్ణాంకం> coords,
                                    గుర్తులేని అంకె_cvt_అంశం,
                                    స్థిరరాశి Tuple_Variation_దత్తాంశం<> *tuple_var_దత్తాంశం,
                                    స్థిరరాశి శూన్య *పునాది,
                                    గుర్తులేని var_దత్తాంశం_నిడివి,
                                    hb_సదిశ_t<దశాంశం>& cvt_deltas /*బహిర్గతం*/)
  {
    ఒకవేళ (!coords) తిరిగి నిజం;
    hb_సదిశ_t<గుర్తులేని> shared_indices;
    Tuple_Variation_దత్తాంశం<>::tuple_iterator_t iterator;
    hb_బైట్లు_t var_దత్తాంశం_బైట్లు = hb_బైట్లు_t (reinterpret_cast<స్థిరరాశి అక్షరం*> (tuple_var_దత్తాంశం), var_దత్తాంశం_నిడివి);
    ఒకవేళ (!Tuple_Variation_దత్తాంశం<>::get_tuple_iterator (var_దత్తాంశం_బైట్లు, axis_లెక్క, పునాది,
                                                 shared_indices, &iterator))
      తిరిగి నిజం; /* isn't applied at all */

    hb_array_t<స్థిరరాశి F2DOT14> shared_tuples = hb_array<F2DOT14> ();
    hb_సదిశ_t<గుర్తులేని> వ్యక్తిగత_indices;
    hb_సదిశ_t<పూర్ణాంకం> unpacked_deltas;

    చేయి
    {
      దశాంశం scalar = iterator.current_tuple->calculate_scalar (coords, axis_లెక్క, shared_tuples);
      ఒకవేళ (scalar == 0.f) కొనసాగు;
      స్థిరరాశి HBUINT8 *p = iterator.get_serialized_దత్తాంశం ();
      గుర్తులేని పూర్ణాంకం నిడివి = iterator.current_tuple->get_దత్తాంశం_కొలత ();
      ఒకవేళ (unlikely (!iterator.var_దత్తాంశం_బైట్లు.check_range (p, నిడివి)))
        తిరిగి అబద్ధం;

      స్థిరరాశి HBUINT8 *చివర = p + నిడివి;

      తార్కిక has_వ్యక్తిగత_points = iterator.current_tuple->has_వ్యక్తిగత_points ();
      ఒకవేళ (has_వ్యక్తిగత_points &&
          !Tuple_Variation_దత్తాంశం<>::decompile_points (p, వ్యక్తిగత_indices, చివర))
        తిరిగి అబద్ధం;
      స్థిరరాశి hb_సదిశ_t<గుర్తులేని పూర్ణాంకం> &indices = has_వ్యక్తిగత_points ? వ్యక్తిగత_indices : shared_indices;

      తార్కిక apply_గమ్యం_all = (indices.నిడివి == 0);
      గుర్తులేని అంకె_deltas = apply_గమ్యం_all ? అంకె_cvt_అంశం : indices.నిడివి;
      ఒకవేళ (unlikely (!unpacked_deltas.resize_dirty  (అంకె_deltas))) తిరిగి అబద్ధం;
      ఒకవేళ (unlikely (!Tuple_Variation_దత్తాంశం<>::decompile_deltas (p, unpacked_deltas, చివర))) తిరిగి అబద్ధం;

      కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < అంకె_deltas; i++)
      {
        గుర్తులేని పూర్ణాంకం idx = apply_గమ్యం_all ? i : indices[i];
        ఒకవేళ (unlikely (idx >= అంకె_cvt_అంశం)) కొనసాగు;
        ఒకవేళ (scalar != 1.0f) cvt_deltas[idx] += unpacked_deltas[i] * scalar ;
        లేకపోతే cvt_deltas[idx] += unpacked_deltas[i];
      }
    } అయితే (iterator.move_గమ్యం_తదుపరి ());

    తిరిగి నిజం;
  }
  
  తార్కిక serialize (hb_serialize_context_t *c,
                  Tuple_Variation_దత్తాంశం<>::tuple_variations_t& tuple_variations) స్థిరరాశి
  {
    TRACE_SERIALIZE (ఇది);
    ఒకవేళ (!tuple_variations) return_trace (అబద్ధం);
    ఒకవేళ (unlikely (!c->embed (సంస్కరణ))) return_trace (అబద్ధం);

    return_trace (tuple_Variation_దత్తాంశం.serialize (c, అబద్ధం, tuple_variations));
  }

  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    ఒకవేళ (c->plan->all_axes_pinned)
      return_trace (అబద్ధం);

    OT::Tuple_Variation_దత్తాంశం<>::tuple_variations_t tuple_variations;
    గుర్తులేని axis_లెక్క = c->plan->axes_old_సూచీ_ట్యాగ్_map.get_population ();

    స్థిరరాశి hb_ట్యాగ్_t cvt = HB_ట్యాగ్('c','v','t',' ');
    hb_blob_t *cvt_blob = hb_face_reference_పట్టిక (c->plan->ఆధారం, cvt);
    గుర్తులేని బిందువు_లెక్క = hb_blob_get_నిడివి (cvt_blob) / FWORD::static_కొలత;
    hb_blob_destroy (cvt_blob);

    ఒకవేళ (!decompile_tuple_variations (axis_లెక్క, బిందువు_లెక్క,
                                     c->ఆధారం_blob, అబద్ధం,
                                     &(c->plan->axes_old_సూచీ_ట్యాగ్_map),
                                     tuple_variations))
      return_trace (అబద్ధం);

    optimize_scratch_t scratch;
    ఒకవేళ (!tuple_variations.instantiate (c->plan->axes_location, c->plan->axes_triple_distances, scratch))
      return_trace (అబద్ధం);

    ఒకవేళ (!tuple_variations.compile_బైట్లు (c->plan->axes_సూచీ_map, c->plan->axes_old_సూచీ_ట్యాగ్_map,
                                         అబద్ధం /* do not use shared points */))
      return_trace (అబద్ధం);

    return_trace (serialize (c->serializer, tuple_variations));
  }

  స్థిర తార్కిక కూడు_cvt_and_apply_deltas (hb_subset_plan_t *plan,
                                        స్థిరరాశి Tuple_Variation_దత్తాంశం<> *tuple_var_దత్తాంశం,
                                        స్థిరరాశి శూన్య *పునాది)
  {
    స్థిరరాశి hb_ట్యాగ్_t cvt = HB_ట్యాగ్('c','v','t',' ');
    hb_blob_t *cvt_blob = hb_face_reference_పట్టిక (plan->ఆధారం, cvt);
    hb_blob_t *cvt_ప్రధానసంఖ్య_blob = hb_blob_copy_writable_or_fail (cvt_blob);
    hb_blob_destroy (cvt_blob);
  
    ఒకవేళ (unlikely (!cvt_ప్రధానసంఖ్య_blob))
      తిరిగి అబద్ధం;
 
    గుర్తులేని cvt_blob_నిడివి = hb_blob_get_నిడివి (cvt_ప్రధానసంఖ్య_blob);
    గుర్తులేని అంకె_cvt_అంశం = cvt_blob_నిడివి / FWORD::static_కొలత;

    hb_సదిశ_t<దశాంశం> cvt_deltas;
    ఒకవేళ (unlikely (!cvt_deltas.resize (అంకె_cvt_అంశం)))
    {
      hb_blob_destroy (cvt_ప్రధానసంఖ్య_blob);
      తిరిగి అబద్ధం;
    }

    /* Bound the tuple variation data by the actual cvar table length rather
     * than trusting tupleVarCount to walk the headers; an inflated count would
     * otherwise run get_size() off the end of the table. */
    hb_blob_t *cvar_blob = hb_face_reference_పట్టిక (plan->ఆధారం, cvar::పట్టిక_ట్యాగ్);
    గుర్తులేని cvar_నిడివి = hb_blob_get_నిడివి (cvar_blob);
    hb_blob_destroy (cvar_blob);
    గుర్తులేని var_దత్తాంశం_స్థానభ్రంశం = (గుర్తులేని) ((స్థిరరాశి అక్షరం *) tuple_var_దత్తాంశం - (స్థిరరాశి అక్షరం *) పునాది);
    గుర్తులేని var_దత్తాంశం_నిడివి = var_దత్తాంశం_స్థానభ్రంశం < cvar_నిడివి ? cvar_నిడివి - var_దత్తాంశం_స్థానభ్రంశం : 0;

    ఒకవేళ (!calculate_cvt_deltas (plan->normalized_coords.నిడివి, plan->normalized_coords.as_array (),
                               అంకె_cvt_అంశం, tuple_var_దత్తాంశం, పునాది, var_దత్తాంశం_నిడివి, cvt_deltas))
    {
      hb_blob_destroy (cvt_ప్రధానసంఖ్య_blob);
      తిరిగి అబద్ధం;
    }

    FWORD *cvt_ప్రధానసంఖ్య = (FWORD *) hb_blob_get_దత్తాంశం_writable (cvt_ప్రధానసంఖ్య_blob, శూన్యసూచిక);
    కొరకు (గుర్తులేని i = 0; i < అంకె_cvt_అంశం; i++)
      cvt_ప్రధానసంఖ్య[i] += (పూర్ణాంకం) roundf (cvt_deltas[i]);
    
    తార్కిక success = plan->కూడు_పట్టిక (cvt, cvt_ప్రధానసంఖ్య_blob);
    hb_blob_destroy (cvt_ప్రధానసంఖ్య_blob);
    తిరిగి success;
  }

  రక్షిత:
  Fixed_సంస్కరణ<>సంస్కరణ;		/* Version of the CVT variation table
					 * initially set to 0x00010000u */
  Tuple_Variation_దత్తాంశం<> tuple_Variation_దత్తాంశం; /* TupleVariationDate for cvar table */
  బహిరంగ:
  DEFINE_కొలత_MIN (8);
};

} /* namespace OT */


#అయితే_ముగింపు /* HB_OT_VAR_CVAR_TABLE_HH */
