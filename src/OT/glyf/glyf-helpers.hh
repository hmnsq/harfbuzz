#నిర్వచితం_కాకపోతే OT_GLYF_GLYF_HELPERS_HH
#నిర్వచించు OT_GLYF_GLYF_HELPERS_HH


#అన్తర్భావ "../../hb-open-type.hh"
#అన్తర్భావ "../../hb-subset-plan.hh"

#అన్తర్భావ "loca.hh"


నామాకాశ OT {
నామాకాశ glyf_impl {


మూస<రకం_పేరు Iterator_లోపల, రకం_పేరు Type_బహిర్గతం,
	 hb_requires (hb_is_ఆధారం_యొక్క (Iterator_లోపల, గుర్తులేని పూర్ణాంకం))>
స్థిర శూన్య
_write_loca (Iterator_లోపల&& it,
	     స్థిరరాశి hb_sorted_సదిశ_t<hb_codepoint_pair_t> new_గమ్యం_old_gid_list,
	     తార్కిక short_offsets,
	     Type_బహిర్గతం *dest,
	     గుర్తులేని అంకె_offsets)
{
  గుర్తులేని కుడి_జరుపు = short_offsets ? 1 : 0;
  గుర్తులేని స్థానభ్రంశం = 0;
  Type_బహిర్గతం value;
  value = 0;
  *dest++ = value;
  hb_codepoint_t చివరి = 0;
  కొరకు (స్వయంచాలక _ : new_గమ్యం_old_gid_list)
  {
    hb_codepoint_t gid = _.మొదటి;
    కొరకు (; చివరి < gid; చివరి++)
    {
      DEBUG_సందేశం (SUBSET, శూన్యసూచిక, "loca entry empty offset %u", స్థానభ్రంశం);
      *dest++ = value;
    }

    గుర్తులేని padded_కొలత = *it++;
    స్థానభ్రంశం += padded_కొలత;
    DEBUG_సందేశం (SUBSET, శూన్యసూచిక, "loca entry gid %" PRIu32 " offset %u padded-size %u", gid, స్థానభ్రంశం, padded_కొలత);
    value = స్థానభ్రంశం >> కుడి_జరుపు;
    *dest++ = value;

    చివరి++; // Skip over gid
  }
  గుర్తులేని అంకె_glyphs = అంకె_offsets - 1;
  కొరకు (; చివరి < అంకె_glyphs; చివరి++)
  {
    DEBUG_సందేశం (SUBSET, శూన్యసూచిక, "loca entry empty offset %u", స్థానభ్రంశం);
    *dest++ = value;
  }
}

స్థిర తార్కిక
కూడు_తల_and_సమితి_loca_సంస్కరణ (hb_subset_plan_t *plan, తార్కిక use_short_loca)
{
  hb_blob_t *తల_blob = hb_sanitize_context_t ().reference_పట్టిక<తల_2> (plan->ఆధారం);
  hb_blob_t *తల_ప్రధానసంఖ్య_blob = hb_blob_copy_writable_or_fail (తల_blob);
  hb_blob_destroy (తల_blob);

  ఒకవేళ (unlikely (!తల_ప్రధానసంఖ్య_blob))
    తిరిగి అబద్ధం;

  తల_2 *తల_ప్రధానసంఖ్య = (తల_2 *) hb_blob_get_దత్తాంశం_writable (తల_ప్రధానసంఖ్య_blob, శూన్యసూచిక);
  తల_ప్రధానసంఖ్య->సూచీ_గమ్యం_Loc_Format = use_short_loca ? 0 : 1;
  ఒకవేళ (plan->normalized_coords)
  {
    తల_ప్రధానసంఖ్య->xMin = plan->తల_maxp_info.xMin;
    తల_ప్రధానసంఖ్య->xMax = plan->తల_maxp_info.xMax;
    తల_ప్రధానసంఖ్య->yMin = plan->తల_maxp_info.yMin;
    తల_ప్రధానసంఖ్య->yMax = plan->తల_maxp_info.yMax;

    గుర్తులేని orig_flag = తల_ప్రధానసంఖ్య->flags;
    ఒకవేళ (plan->తల_maxp_info.allXMinIsLsb)
      orig_flag |= 1 << 1;
    లేకపోతే
      orig_flag &= ~(1 << 1);
    తల_ప్రధానసంఖ్య->flags = orig_flag;
  }
  తార్కిక success = plan->కూడు_పట్టిక (HB_OT_ట్యాగ్_తల, తల_ప్రధానసంఖ్య_blob);

  hb_blob_destroy (తల_ప్రధానసంఖ్య_blob);
  తిరిగి success;
}

మూస<రకం_పేరు Iterator,
	 hb_requires (hb_is_ఆధారం_యొక్క (Iterator, గుర్తులేని పూర్ణాంకం))>
స్థిర తార్కిక
కూడు_loca_and_తల (hb_subset_context_t *c,
		    Iterator padded_offsets,
		    తార్కిక use_short_loca,
		    తార్కిక extended)
{
  గుర్తులేని అంకె_offsets = c->plan->అంకె_output_glyphs () + 1;
  గుర్తులేని నమోదు_కొలత = use_short_loca ? 2 : 4;

  అక్షరం *loca_ప్రధానసంఖ్య_దత్తాంశం = (అక్షరం *) hb_malloc2 (అంకె_offsets, నమోదు_కొలత);

  ఒకవేళ (unlikely (!loca_ప్రధానసంఖ్య_దత్తాంశం)) తిరిగి అబద్ధం;

  DEBUG_సందేశం (SUBSET, శూన్యసూచిక, "loca entry_size %u num_offsets %u size %u",
	     నమోదు_కొలత, అంకె_offsets, నమోదు_కొలత * అంకె_offsets);

  ఒకవేళ (use_short_loca)
    _write_loca (padded_offsets, c->plan->new_గమ్యం_old_gid_list, నిజం, (HBUINT16 *) loca_ప్రధానసంఖ్య_దత్తాంశం, అంకె_offsets);
  లేకపోతే
    _write_loca (padded_offsets, c->plan->new_గమ్యం_old_gid_list, అబద్ధం, (HBUINT32 *) loca_ప్రధానసంఖ్య_దత్తాంశం, అంకె_offsets);

  hb_blob_t *loca_blob = hb_blob_create (loca_ప్రధానసంఖ్య_దత్తాంశం,
					 నమోదు_కొలత * అంకె_offsets,
					 HB_జ్ఞాపకం_బహుళకం_WRITABLE,
					 loca_ప్రధానసంఖ్య_దత్తాంశం,
					 hb_free);

  తార్కిక ఫలితం = c->plan->కూడు_పట్టిక (extended ? HB_OT_ట్యాగ్_LOCA : HB_OT_ట్యాగ్_loca, loca_blob)
	     && కూడు_తల_and_సమితి_loca_సంస్కరణ (c->plan, use_short_loca);

  hb_blob_destroy (loca_blob);
  తిరిగి ఫలితం;
}


} /* namespace glyf_impl */
} /* namespace OT */


#అయితే_ముగింపు /* OT_GLYF_GLYF_HELPERS_HH */
