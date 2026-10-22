#ifndef OT_GLYF_GLYF_HELPERS_HH
#define OT_GLYF_GLYF_HELPERS_HH


#अन्तर्भाव "../../hb-open-type.hh"
#अन्तर्भाव "../../hb-subset-plan.hh"

#अन्तर्भाव "loca.hh"


नामाकाश OT {
नामाकाश glyf_impl {


template<typename IteratorIn, typename TypeOut,
	 hb_requires (hb_is_source_of (IteratorIn, unsigned पूर्णाङ्क))>
स्थिर शून्य
_write_loca (IteratorIn&& it,
	     ध्रुव hb_sorted_vector_t<hb_codepoint_pair_t> new_to_old_gid_list,
	     तार्किक short_offsets,
	     TypeOut *dest,
	     unsigned अङ्कः_offsets)
{
  unsigned दक्षिणः_shift = short_offsets ? 1 : 0;
  unsigned offset = 0;
  TypeOut value;
  value = 0;
  *dest++ = value;
  hb_codepoint_t last = 0;
  अत्थाय (auto _ : new_to_old_gid_list)
  {
    hb_codepoint_t gid = _.first;
    अत्थाय (; last < gid; last++)
    {
      DEBUG_सन्देशः (SUBSET, nullptr, "loca entry empty offset %u", offset);
      *dest++ = value;
    }

    unsigned padded_size = *it++;
    offset += padded_size;
    DEBUG_सन्देशः (SUBSET, nullptr, "loca entry gid %" PRIu32 " offset %u padded-size %u", gid, offset, padded_size);
    value = offset >> दक्षिणः_shift;
    *dest++ = value;

    last++; // Skip over gid
  }
  unsigned अङ्कः_glyphs = अङ्कः_offsets - 1;
  अत्थाय (; last < अङ्कः_glyphs; last++)
  {
    DEBUG_सन्देशः (SUBSET, nullptr, "loca entry empty offset %u", offset);
    *dest++ = value;
  }
}

स्थिर तार्किक
_add_head_and_set_loca_version (hb_subset_plan_t *plan, तार्किक use_short_loca)
{
  hb_blob_t *head_blob = hb_sanitize_context_t ().reference_table<head> (plan->source);
  hb_blob_t *head_अविभाज्यः_blob = hb_blob_copy_writable_or_fail (head_blob);
  hb_blob_destroy (head_blob);

  सचे (unlikely (!head_अविभाज्यः_blob))
    पटिनिवत्त मिच्छा;

  head *head_अविभाज्यः = (head *) hb_blob_get_data_writable (head_अविभाज्यः_blob, nullptr);
  head_अविभाज्यः->indexToLocFormat = use_short_loca ? 0 : 1;
  सचे (plan->normalized_coords)
  {
    head_अविभाज्यः->xMin = plan->head_maxp_info.xMin;
    head_अविभाज्यः->xMax = plan->head_maxp_info.xMax;
    head_अविभाज्यः->yMin = plan->head_maxp_info.yMin;
    head_अविभाज्यः->yMax = plan->head_maxp_info.yMax;

    unsigned orig_flag = head_अविभाज्यः->flags;
    सचे (plan->head_maxp_info.allXMinIsLsb)
      orig_flag |= 1 << 1;
    अञ्ञथा
      orig_flag &= ~(1 << 1);
    head_अविभाज्यः->flags = orig_flag;
  }
  तार्किक success = plan->add_table (HB_OT_TAG_head, head_अविभाज्यः_blob);

  hb_blob_destroy (head_अविभाज्यः_blob);
  पटिनिवत्त success;
}

template<typename Iterator,
	 hb_requires (hb_is_source_of (Iterator, unsigned पूर्णाङ्क))>
स्थिर तार्किक
_add_loca_and_head (hb_subset_context_t *c,
		    Iterator padded_offsets,
		    तार्किक use_short_loca,
		    तार्किक extended)
{
  unsigned अङ्कः_offsets = c->plan->अङ्कः_output_glyphs () + 1;
  unsigned entry_size = use_short_loca ? 2 : 4;

  वर्ण *loca_अविभाज्यः_data = (वर्ण *) hb_malloc2 (अङ्कः_offsets, entry_size);

  सचे (unlikely (!loca_अविभाज्यः_data)) पटिनिवत्त मिच्छा;

  DEBUG_सन्देशः (SUBSET, nullptr, "loca entry_size %u num_offsets %u size %u",
	     entry_size, अङ्कः_offsets, entry_size * अङ्कः_offsets);

  सचे (use_short_loca)
    _write_loca (padded_offsets, c->plan->new_to_old_gid_list, सच्च, (HBUINT16 *) loca_अविभाज्यः_data, अङ्कः_offsets);
  अञ्ञथा
    _write_loca (padded_offsets, c->plan->new_to_old_gid_list, मिच्छा, (HBUINT32 *) loca_अविभाज्यः_data, अङ्कः_offsets);

  hb_blob_t *loca_blob = hb_blob_create (loca_अविभाज्यः_data,
					 entry_size * अङ्कः_offsets,
					 HB_MEMORY_MODE_WRITABLE,
					 loca_अविभाज्यः_data,
					 hb_free);

  तार्किक result = c->plan->add_table (extended ? HB_OT_TAG_LOCA : HB_OT_TAG_loca, loca_blob)
	     && _add_head_and_set_loca_version (c->plan, use_short_loca);

  hb_blob_destroy (loca_blob);
  पटिनिवत्त result;
}


} /* namespace glyf_impl */
} /* namespace OT */


#endif /* OT_GLYF_GLYF_HELPERS_HH */
