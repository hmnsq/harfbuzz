#ifndef OT_GLYF_GLYF_HELPERS_HH
#define OT_GLYF_GLYF_HELPERS_HH


#അന്തര്ഭാവ "../../hb-open-type.hh"
#അന്തര്ഭാവ "../../hb-subset-plan.hh"

#അന്തര്ഭാവ "loca.hh"


നാമാകാശ OT {
നാമാകാശ glyf_impl {


template<typename IteratorIn, typename TypeOut,
	 hb_requires (hb_is_source_of (IteratorIn, unsigned പൂർണ്ണസംഖ്യ))>
സ്ഥിതം ശൂന്യം
_write_loca (IteratorIn&& it,
	     സ്ഥിരം hb_sorted_vector_t<hb_codepoint_pair_t> new_to_old_gid_list,
	     തർക്കം short_offsets,
	     TypeOut *dest,
	     unsigned അക്കം_offsets)
{
  unsigned വലത്_shift = short_offsets ? 1 : 0;
  unsigned offset = 0;
  TypeOut value;
  value = 0;
  *dest++ = value;
  hb_codepoint_t last = 0;
  വേണ്ടി (auto _ : new_to_old_gid_list)
  {
    hb_codepoint_t gid = _.first;
    വേണ്ടി (; last < gid; last++)
    {
      DEBUG_സന്ദേശം (SUBSET, nullptr, "loca entry empty offset %u", offset);
      *dest++ = value;
    }

    unsigned padded_size = *it++;
    offset += padded_size;
    DEBUG_സന്ദേശം (SUBSET, nullptr, "loca entry gid %" PRIu32 " offset %u padded-size %u", gid, offset, padded_size);
    value = offset >> വലത്_shift;
    *dest++ = value;

    last++; // Skip over gid
  }
  unsigned അക്കം_glyphs = അക്കം_offsets - 1;
  വേണ്ടി (; last < അക്കം_glyphs; last++)
  {
    DEBUG_സന്ദേശം (SUBSET, nullptr, "loca entry empty offset %u", offset);
    *dest++ = value;
  }
}

സ്ഥിതം തർക്കം
_add_head_and_set_loca_version (hb_subset_plan_t *plan, തർക്കം use_short_loca)
{
  hb_blob_t *head_blob = hb_sanitize_context_t ().reference_table<head> (plan->source);
  hb_blob_t *head_അഭാജ്യം_blob = hb_blob_copy_writable_or_fail (head_blob);
  hb_blob_destroy (head_blob);

  എങ്കിൽ (unlikely (!head_അഭാജ്യം_blob))
    തിരികെ അസത്യം;

  head *head_അഭാജ്യം = (head *) hb_blob_get_data_writable (head_അഭാജ്യം_blob, nullptr);
  head_അഭാജ്യം->indexToLocFormat = use_short_loca ? 0 : 1;
  എങ്കിൽ (plan->normalized_coords)
  {
    head_അഭാജ്യം->xMin = plan->head_maxp_info.xMin;
    head_അഭാജ്യം->xMax = plan->head_maxp_info.xMax;
    head_അഭാജ്യം->yMin = plan->head_maxp_info.yMin;
    head_അഭാജ്യം->yMax = plan->head_maxp_info.yMax;

    unsigned orig_flag = head_അഭാജ്യം->flags;
    എങ്കിൽ (plan->head_maxp_info.allXMinIsLsb)
      orig_flag |= 1 << 1;
    അല്ലെങ്കിൽ
      orig_flag &= ~(1 << 1);
    head_അഭാജ്യം->flags = orig_flag;
  }
  തർക്കം success = plan->add_table (HB_OT_TAG_head, head_അഭാജ്യം_blob);

  hb_blob_destroy (head_അഭാജ്യം_blob);
  തിരികെ success;
}

template<typename Iterator,
	 hb_requires (hb_is_source_of (Iterator, unsigned പൂർണ്ണസംഖ്യ))>
സ്ഥിതം തർക്കം
_add_loca_and_head (hb_subset_context_t *c,
		    Iterator padded_offsets,
		    തർക്കം use_short_loca,
		    തർക്കം extended)
{
  unsigned അക്കം_offsets = c->plan->അക്കം_output_glyphs () + 1;
  unsigned entry_size = use_short_loca ? 2 : 4;

  അക്ഷരം *loca_അഭാജ്യം_data = (അക്ഷരം *) hb_malloc2 (അക്കം_offsets, entry_size);

  എങ്കിൽ (unlikely (!loca_അഭാജ്യം_data)) തിരികെ അസത്യം;

  DEBUG_സന്ദേശം (SUBSET, nullptr, "loca entry_size %u num_offsets %u size %u",
	     entry_size, അക്കം_offsets, entry_size * അക്കം_offsets);

  എങ്കിൽ (use_short_loca)
    _write_loca (padded_offsets, c->plan->new_to_old_gid_list, സത്യം, (HBUINT16 *) loca_അഭാജ്യം_data, അക്കം_offsets);
  അല്ലെങ്കിൽ
    _write_loca (padded_offsets, c->plan->new_to_old_gid_list, അസത്യം, (HBUINT32 *) loca_അഭാജ്യം_data, അക്കം_offsets);

  hb_blob_t *loca_blob = hb_blob_create (loca_അഭാജ്യം_data,
					 entry_size * അക്കം_offsets,
					 HB_MEMORY_MODE_WRITABLE,
					 loca_അഭാജ്യം_data,
					 hb_free);

  തർക്കം result = c->plan->add_table (extended ? HB_OT_TAG_LOCA : HB_OT_TAG_loca, loca_blob)
	     && _add_head_and_set_loca_version (c->plan, use_short_loca);

  hb_blob_destroy (loca_blob);
  തിരികെ result;
}


} /* namespace glyf_impl */
} /* namespace OT */


#endif /* OT_GLYF_GLYF_HELPERS_HH */
