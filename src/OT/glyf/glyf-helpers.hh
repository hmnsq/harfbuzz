#ifndef OT_GLYF_GLYF_HELPERS_HH
#define OT_GLYF_GLYF_HELPERS_HH


#ଅନ୍ତର୍ଭାଵ "../../hb-open-type.hh"
#ଅନ୍ତର୍ଭାଵ "../../hb-subset-plan.hh"

#ଅନ୍ତର୍ଭାଵ "loca.hh"


ନାମାକାଶ OT {
ନାମାକାଶ glyf_impl {


template<typename IteratorIn, typename TypeOut,
	 hb_requires (hb_is_source_of (IteratorIn, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା))>
ସ୍ଥିର ଶୂନ୍ୟ
_write_loca (IteratorIn&& it,
	     ଅଚଳ hb_sorted_vector_t<hb_codepoint_pair_t> new_to_old_gid_list,
	     ତାର୍କିକ short_offsets,
	     TypeOut *dest,
	     unsigned ଅଙ୍କ_offsets)
{
  unsigned ଡାହାଣ_shift = short_offsets ? 1 : 0;
  unsigned offset = 0;
  TypeOut value;
  value = 0;
  *dest++ = value;
  hb_codepoint_t last = 0;
  ପାଇଁ (auto _ : new_to_old_gid_list)
  {
    hb_codepoint_t gid = _.first;
    ପାଇଁ (; last < gid; last++)
    {
      DEBUG_ସନ୍ଦେଶ (SUBSET, nullptr, "loca entry empty offset %u", offset);
      *dest++ = value;
    }

    unsigned padded_size = *it++;
    offset += padded_size;
    DEBUG_ସନ୍ଦେଶ (SUBSET, nullptr, "loca entry gid %" PRIu32 " offset %u padded-size %u", gid, offset, padded_size);
    value = offset >> ଡାହାଣ_shift;
    *dest++ = value;

    last++; // Skip over gid
  }
  unsigned ଅଙ୍କ_glyphs = ଅଙ୍କ_offsets - 1;
  ପାଇଁ (; last < ଅଙ୍କ_glyphs; last++)
  {
    DEBUG_ସନ୍ଦେଶ (SUBSET, nullptr, "loca entry empty offset %u", offset);
    *dest++ = value;
  }
}

ସ୍ଥିର ତାର୍କିକ
_add_head_and_set_loca_version (hb_subset_plan_t *plan, ତାର୍କିକ use_short_loca)
{
  hb_blob_t *head_blob = hb_sanitize_context_t ().reference_table<head> (plan->source);
  hb_blob_t *head_ମୌଳିକ_blob = hb_blob_copy_writable_or_fail (head_blob);
  hb_blob_destroy (head_blob);

  ଯଦି (unlikely (!head_ମୌଳିକ_blob))
    ଫେରନ୍ତୁ ମିଥ୍ୟା;

  head *head_ମୌଳିକ = (head *) hb_blob_get_data_writable (head_ମୌଳିକ_blob, nullptr);
  head_ମୌଳିକ->indexToLocFormat = use_short_loca ? 0 : 1;
  ଯଦି (plan->normalized_coords)
  {
    head_ମୌଳିକ->xMin = plan->head_maxp_info.xMin;
    head_ମୌଳିକ->xMax = plan->head_maxp_info.xMax;
    head_ମୌଳିକ->yMin = plan->head_maxp_info.yMin;
    head_ମୌଳିକ->yMax = plan->head_maxp_info.yMax;

    unsigned orig_flag = head_ମୌଳିକ->flags;
    ଯଦି (plan->head_maxp_info.allXMinIsLsb)
      orig_flag |= 1 << 1;
    ଅନ୍ୟଥା
      orig_flag &= ~(1 << 1);
    head_ମୌଳିକ->flags = orig_flag;
  }
  ତାର୍କିକ success = plan->add_table (HB_OT_TAG_head, head_ମୌଳିକ_blob);

  hb_blob_destroy (head_ମୌଳିକ_blob);
  ଫେରନ୍ତୁ success;
}

template<typename Iterator,
	 hb_requires (hb_is_source_of (Iterator, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା))>
ସ୍ଥିର ତାର୍କିକ
_add_loca_and_head (hb_subset_context_t *c,
		    Iterator padded_offsets,
		    ତାର୍କିକ use_short_loca,
		    ତାର୍କିକ extended)
{
  unsigned ଅଙ୍କ_offsets = c->plan->ଅଙ୍କ_output_glyphs () + 1;
  unsigned entry_size = use_short_loca ? 2 : 4;

  ଅକ୍ଷର *loca_ମୌଳିକ_data = (ଅକ୍ଷର *) hb_malloc2 (ଅଙ୍କ_offsets, entry_size);

  ଯଦି (unlikely (!loca_ମୌଳିକ_data)) ଫେରନ୍ତୁ ମିଥ୍ୟା;

  DEBUG_ସନ୍ଦେଶ (SUBSET, nullptr, "loca entry_size %u num_offsets %u size %u",
	     entry_size, ଅଙ୍କ_offsets, entry_size * ଅଙ୍କ_offsets);

  ଯଦି (use_short_loca)
    _write_loca (padded_offsets, c->plan->new_to_old_gid_list, ସତ୍ୟ, (HBUINT16 *) loca_ମୌଳିକ_data, ଅଙ୍କ_offsets);
  ଅନ୍ୟଥା
    _write_loca (padded_offsets, c->plan->new_to_old_gid_list, ମିଥ୍ୟା, (HBUINT32 *) loca_ମୌଳିକ_data, ଅଙ୍କ_offsets);

  hb_blob_t *loca_blob = hb_blob_create (loca_ମୌଳିକ_data,
					 entry_size * ଅଙ୍କ_offsets,
					 HB_MEMORY_MODE_WRITABLE,
					 loca_ମୌଳିକ_data,
					 hb_free);

  ତାର୍କିକ result = c->plan->add_table (extended ? HB_OT_TAG_LOCA : HB_OT_TAG_loca, loca_blob)
	     && _add_head_and_set_loca_version (c->plan, use_short_loca);

  hb_blob_destroy (loca_blob);
  ଫେରନ୍ତୁ result;
}


} /* namespace glyf_impl */
} /* namespace OT */


#endif /* OT_GLYF_GLYF_HELPERS_HH */
