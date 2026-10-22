#ifndef OT_GLYF_GLYF_HELPERS_HH
#define OT_GLYF_GLYF_HELPERS_HH


#ಅನ್ತರ್ಭಾವ "../../hb-open-type.hh"
#ಅನ್ತರ್ಭಾವ "../../hb-subset-plan.hh"

#ಅನ್ತರ್ಭಾವ "loca.hh"


ನಾಮಾಕಾಶ OT {
ನಾಮಾಕಾಶ glyf_impl {


template<typename IteratorIn, typename TypeOut,
	 hb_requires (hb_is_source_of (IteratorIn, unsigned ಪೂರ್ಣಾಙ್ಕ))>
ಸ್ಥಿರ ಶೂನ್ಯ
_write_loca (IteratorIn&& it,
	     ಧ್ರುವ hb_sorted_vector_t<hb_codepoint_pair_t> new_to_old_gid_list,
	     ತಾರ್ಕಿಕ short_offsets,
	     TypeOut *dest,
	     unsigned ಅಂಕೆ_offsets)
{
  unsigned ಬಲ_shift = short_offsets ? 1 : 0;
  unsigned offset = 0;
  TypeOut value;
  value = 0;
  *dest++ = value;
  hb_codepoint_t last = 0;
  ಪ್ರತಿ (auto _ : new_to_old_gid_list)
  {
    hb_codepoint_t gid = _.first;
    ಪ್ರತಿ (; last < gid; last++)
    {
      DEBUG_ಸಂದೇಶ (SUBSET, nullptr, "loca entry empty offset %u", offset);
      *dest++ = value;
    }

    unsigned padded_size = *it++;
    offset += padded_size;
    DEBUG_ಸಂದೇಶ (SUBSET, nullptr, "loca entry gid %" PRIu32 " offset %u padded-size %u", gid, offset, padded_size);
    value = offset >> ಬಲ_shift;
    *dest++ = value;

    last++; // Skip over gid
  }
  unsigned ಅಂಕೆ_glyphs = ಅಂಕೆ_offsets - 1;
  ಪ್ರತಿ (; last < ಅಂಕೆ_glyphs; last++)
  {
    DEBUG_ಸಂದೇಶ (SUBSET, nullptr, "loca entry empty offset %u", offset);
    *dest++ = value;
  }
}

ಸ್ಥಿರ ತಾರ್ಕಿಕ
_add_head_and_set_loca_version (hb_subset_plan_t *plan, ತಾರ್ಕಿಕ use_short_loca)
{
  hb_blob_t *head_blob = hb_sanitize_context_t ().reference_table<head> (plan->source);
  hb_blob_t *head_ಅವಿಭಾಜ್ಯ_blob = hb_blob_copy_writable_or_fail (head_blob);
  hb_blob_destroy (head_blob);

  ಯದಿ (unlikely (!head_ಅವಿಭಾಜ್ಯ_blob))
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

  head *head_ಅವಿಭಾಜ್ಯ = (head *) hb_blob_get_data_writable (head_ಅವಿಭಾಜ್ಯ_blob, nullptr);
  head_ಅವಿಭಾಜ್ಯ->indexToLocFormat = use_short_loca ? 0 : 1;
  ಯದಿ (plan->normalized_coords)
  {
    head_ಅವಿಭಾಜ್ಯ->xMin = plan->head_maxp_info.xMin;
    head_ಅವಿಭಾಜ್ಯ->xMax = plan->head_maxp_info.xMax;
    head_ಅವಿಭಾಜ್ಯ->yMin = plan->head_maxp_info.yMin;
    head_ಅವಿಭಾಜ್ಯ->yMax = plan->head_maxp_info.yMax;

    unsigned orig_flag = head_ಅವಿಭಾಜ್ಯ->flags;
    ಯದಿ (plan->head_maxp_info.allXMinIsLsb)
      orig_flag |= 1 << 1;
    ಅನ್ಯಥಾ
      orig_flag &= ~(1 << 1);
    head_ಅವಿಭಾಜ್ಯ->flags = orig_flag;
  }
  ತಾರ್ಕಿಕ success = plan->add_table (HB_OT_TAG_head, head_ಅವಿಭಾಜ್ಯ_blob);

  hb_blob_destroy (head_ಅವಿಭಾಜ್ಯ_blob);
  ಪ್ರತ್ಯಾಗಮ success;
}

template<typename Iterator,
	 hb_requires (hb_is_source_of (Iterator, unsigned ಪೂರ್ಣಾಙ್ಕ))>
ಸ್ಥಿರ ತಾರ್ಕಿಕ
_add_loca_and_head (hb_subset_context_t *c,
		    Iterator padded_offsets,
		    ತಾರ್ಕಿಕ use_short_loca,
		    ತಾರ್ಕಿಕ extended)
{
  unsigned ಅಂಕೆ_offsets = c->plan->ಅಂಕೆ_output_glyphs () + 1;
  unsigned entry_size = use_short_loca ? 2 : 4;

  ವರ್ಣ *loca_ಅವಿಭಾಜ್ಯ_data = (ವರ್ಣ *) hb_malloc2 (ಅಂಕೆ_offsets, entry_size);

  ಯದಿ (unlikely (!loca_ಅವಿಭಾಜ್ಯ_data)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

  DEBUG_ಸಂದೇಶ (SUBSET, nullptr, "loca entry_size %u num_offsets %u size %u",
	     entry_size, ಅಂಕೆ_offsets, entry_size * ಅಂಕೆ_offsets);

  ಯದಿ (use_short_loca)
    _write_loca (padded_offsets, c->plan->new_to_old_gid_list, ಸತ್ಯ, (HBUINT16 *) loca_ಅವಿಭಾಜ್ಯ_data, ಅಂಕೆ_offsets);
  ಅನ್ಯಥಾ
    _write_loca (padded_offsets, c->plan->new_to_old_gid_list, ಅಸತ್ಯ, (HBUINT32 *) loca_ಅವಿಭಾಜ್ಯ_data, ಅಂಕೆ_offsets);

  hb_blob_t *loca_blob = hb_blob_create (loca_ಅವಿಭಾಜ್ಯ_data,
					 entry_size * ಅಂಕೆ_offsets,
					 HB_MEMORY_MODE_WRITABLE,
					 loca_ಅವಿಭಾಜ್ಯ_data,
					 hb_free);

  ತಾರ್ಕಿಕ result = c->plan->add_table (extended ? HB_OT_TAG_LOCA : HB_OT_TAG_loca, loca_blob)
	     && _add_head_and_set_loca_version (c->plan, use_short_loca);

  hb_blob_destroy (loca_blob);
  ಪ್ರತ್ಯಾಗಮ result;
}


} /* namespace glyf_impl */
} /* namespace OT */


#endif /* OT_GLYF_GLYF_HELPERS_HH */
