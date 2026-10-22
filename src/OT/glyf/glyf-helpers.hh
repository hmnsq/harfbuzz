#ifndef OT_GLYF_GLYF_HELPERS_HH
#define OT_GLYF_GLYF_HELPERS_HH


#அந்தர்பாவ "../../hb-open-type.hh"
#அந்தர்பாவ "../../hb-subset-plan.hh"

#அந்தர்பாவ "loca.hh"


நாமாகாஶ OT {
நாமாகாஶ glyf_impl {


template<typename IteratorIn, typename TypeOut,
	 hb_requires (hb_is_source_of (IteratorIn, unsigned முழு))>
நிலையான வெறுமை
_write_loca (IteratorIn&& it,
	     மாறிலி hb_sorted_vector_t<hb_codepoint_pair_t> new_to_old_gid_list,
	     தருக்க short_offsets,
	     TypeOut *dest,
	     unsigned இலக்கம்_offsets)
{
  unsigned வலது_shift = short_offsets ? 1 : 0;
  unsigned offset = 0;
  TypeOut value;
  value = 0;
  *dest++ = value;
  hb_codepoint_t last = 0;
  க்கு (auto _ : new_to_old_gid_list)
  {
    hb_codepoint_t gid = _.first;
    க்கு (; last < gid; last++)
    {
      DEBUG_செய்தி (SUBSET, nullptr, "loca entry empty offset %u", offset);
      *dest++ = value;
    }

    unsigned padded_size = *it++;
    offset += padded_size;
    DEBUG_செய்தி (SUBSET, nullptr, "loca entry gid %" PRIu32 " offset %u padded-size %u", gid, offset, padded_size);
    value = offset >> வலது_shift;
    *dest++ = value;

    last++; // Skip over gid
  }
  unsigned இலக்கம்_glyphs = இலக்கம்_offsets - 1;
  க்கு (; last < இலக்கம்_glyphs; last++)
  {
    DEBUG_செய்தி (SUBSET, nullptr, "loca entry empty offset %u", offset);
    *dest++ = value;
  }
}

நிலையான தருக்க
_add_head_and_set_loca_version (hb_subset_plan_t *plan, தருக்க use_short_loca)
{
  hb_blob_t *head_blob = hb_sanitize_context_t ().reference_table<head> (plan->source);
  hb_blob_t *head_பகாஎண்_blob = hb_blob_copy_writable_or_fail (head_blob);
  hb_blob_destroy (head_blob);

  எனில் (unlikely (!head_பகாஎண்_blob))
    திரும்பு பொய்;

  head *head_பகாஎண் = (head *) hb_blob_get_data_writable (head_பகாஎண்_blob, nullptr);
  head_பகாஎண்->indexToLocFormat = use_short_loca ? 0 : 1;
  எனில் (plan->normalized_coords)
  {
    head_பகாஎண்->xMin = plan->head_maxp_info.xMin;
    head_பகாஎண்->xMax = plan->head_maxp_info.xMax;
    head_பகாஎண்->yMin = plan->head_maxp_info.yMin;
    head_பகாஎண்->yMax = plan->head_maxp_info.yMax;

    unsigned orig_flag = head_பகாஎண்->flags;
    எனில் (plan->head_maxp_info.allXMinIsLsb)
      orig_flag |= 1 << 1;
    இல்லையெனில்
      orig_flag &= ~(1 << 1);
    head_பகாஎண்->flags = orig_flag;
  }
  தருக்க success = plan->add_table (HB_OT_TAG_head, head_பகாஎண்_blob);

  hb_blob_destroy (head_பகாஎண்_blob);
  திரும்பு success;
}

template<typename Iterator,
	 hb_requires (hb_is_source_of (Iterator, unsigned முழு))>
நிலையான தருக்க
_add_loca_and_head (hb_subset_context_t *c,
		    Iterator padded_offsets,
		    தருக்க use_short_loca,
		    தருக்க extended)
{
  unsigned இலக்கம்_offsets = c->plan->இலக்கம்_output_glyphs () + 1;
  unsigned entry_size = use_short_loca ? 2 : 4;

  எழுத்து *loca_பகாஎண்_data = (எழுத்து *) hb_malloc2 (இலக்கம்_offsets, entry_size);

  எனில் (unlikely (!loca_பகாஎண்_data)) திரும்பு பொய்;

  DEBUG_செய்தி (SUBSET, nullptr, "loca entry_size %u num_offsets %u size %u",
	     entry_size, இலக்கம்_offsets, entry_size * இலக்கம்_offsets);

  எனில் (use_short_loca)
    _write_loca (padded_offsets, c->plan->new_to_old_gid_list, உண்மை, (HBUINT16 *) loca_பகாஎண்_data, இலக்கம்_offsets);
  இல்லையெனில்
    _write_loca (padded_offsets, c->plan->new_to_old_gid_list, பொய், (HBUINT32 *) loca_பகாஎண்_data, இலக்கம்_offsets);

  hb_blob_t *loca_blob = hb_blob_create (loca_பகாஎண்_data,
					 entry_size * இலக்கம்_offsets,
					 HB_MEMORY_MODE_WRITABLE,
					 loca_பகாஎண்_data,
					 hb_free);

  தருக்க result = c->plan->add_table (extended ? HB_OT_TAG_LOCA : HB_OT_TAG_loca, loca_blob)
	     && _add_head_and_set_loca_version (c->plan, use_short_loca);

  hb_blob_destroy (loca_blob);
  திரும்பு result;
}


} /* namespace glyf_impl */
} /* namespace OT */


#endif /* OT_GLYF_GLYF_HELPERS_HH */
