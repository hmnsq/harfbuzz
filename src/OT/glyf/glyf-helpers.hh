#ifndef OT_GLYF_GLYF_HELPERS_HH
#define OT_GLYF_GLYF_HELPERS_HH


#અન્તર્ભાવ "../../hb-open-type.hh"
#અન્તર્ભાવ "../../hb-subset-plan.hh"

#અન્તર્ભાવ "loca.hh"


નામાકાશ OT {
નામાકાશ glyf_impl {


template<typename IteratorIn, typename TypeOut,
	 hb_requires (hb_is_source_of (IteratorIn, unsigned પૂર્ણાંક))>
સ્થિર શૂન્ય
_write_loca (IteratorIn&& it,
	     અચલ hb_sorted_vector_t<hb_codepoint_pair_t> new_to_old_gid_list,
	     તાર્કિક short_offsets,
	     TypeOut *dest,
	     unsigned અંક_offsets)
{
  unsigned જમણું_shift = short_offsets ? 1 : 0;
  unsigned offset = 0;
  TypeOut value;
  value = 0;
  *dest++ = value;
  hb_codepoint_t last = 0;
  માટે (auto _ : new_to_old_gid_list)
  {
    hb_codepoint_t gid = _.first;
    માટે (; last < gid; last++)
    {
      DEBUG_સંદેશ (SUBSET, nullptr, "loca entry empty offset %u", offset);
      *dest++ = value;
    }

    unsigned padded_size = *it++;
    offset += padded_size;
    DEBUG_સંદેશ (SUBSET, nullptr, "loca entry gid %" PRIu32 " offset %u padded-size %u", gid, offset, padded_size);
    value = offset >> જમણું_shift;
    *dest++ = value;

    last++; // Skip over gid
  }
  unsigned અંક_glyphs = અંક_offsets - 1;
  માટે (; last < અંક_glyphs; last++)
  {
    DEBUG_સંદેશ (SUBSET, nullptr, "loca entry empty offset %u", offset);
    *dest++ = value;
  }
}

સ્થિર તાર્કિક
_add_head_and_set_loca_version (hb_subset_plan_t *plan, તાર્કિક use_short_loca)
{
  hb_blob_t *head_blob = hb_sanitize_context_t ().reference_table<head> (plan->source);
  hb_blob_t *head_અવિભાજ્ય_blob = hb_blob_copy_writable_or_fail (head_blob);
  hb_blob_destroy (head_blob);

  જો (unlikely (!head_અવિભાજ્ય_blob))
    પાછા ખોટું;

  head *head_અવિભાજ્ય = (head *) hb_blob_get_data_writable (head_અવિભાજ્ય_blob, nullptr);
  head_અવિભાજ્ય->indexToLocFormat = use_short_loca ? 0 : 1;
  જો (plan->normalized_coords)
  {
    head_અવિભાજ્ય->xMin = plan->head_maxp_info.xMin;
    head_અવિભાજ્ય->xMax = plan->head_maxp_info.xMax;
    head_અવિભાજ્ય->yMin = plan->head_maxp_info.yMin;
    head_અવિભાજ્ય->yMax = plan->head_maxp_info.yMax;

    unsigned orig_flag = head_અવિભાજ્ય->flags;
    જો (plan->head_maxp_info.allXMinIsLsb)
      orig_flag |= 1 << 1;
    નહીંતર
      orig_flag &= ~(1 << 1);
    head_અવિભાજ્ય->flags = orig_flag;
  }
  તાર્કિક success = plan->add_table (HB_OT_TAG_head, head_અવિભાજ્ય_blob);

  hb_blob_destroy (head_અવિભાજ્ય_blob);
  પાછા success;
}

template<typename Iterator,
	 hb_requires (hb_is_source_of (Iterator, unsigned પૂર્ણાંક))>
સ્થિર તાર્કિક
_add_loca_and_head (hb_subset_context_t *c,
		    Iterator padded_offsets,
		    તાર્કિક use_short_loca,
		    તાર્કિક extended)
{
  unsigned અંક_offsets = c->plan->અંક_output_glyphs () + 1;
  unsigned entry_size = use_short_loca ? 2 : 4;

  અક્ષર *loca_અવિભાજ્ય_data = (અક્ષર *) hb_malloc2 (અંક_offsets, entry_size);

  જો (unlikely (!loca_અવિભાજ્ય_data)) પાછા ખોટું;

  DEBUG_સંદેશ (SUBSET, nullptr, "loca entry_size %u num_offsets %u size %u",
	     entry_size, અંક_offsets, entry_size * અંક_offsets);

  જો (use_short_loca)
    _write_loca (padded_offsets, c->plan->new_to_old_gid_list, સાચું, (HBUINT16 *) loca_અવિભાજ્ય_data, અંક_offsets);
  નહીંતર
    _write_loca (padded_offsets, c->plan->new_to_old_gid_list, ખોટું, (HBUINT32 *) loca_અવિભાજ્ય_data, અંક_offsets);

  hb_blob_t *loca_blob = hb_blob_create (loca_અવિભાજ્ય_data,
					 entry_size * અંક_offsets,
					 HB_MEMORY_MODE_WRITABLE,
					 loca_અવિભાજ્ય_data,
					 hb_free);

  તાર્કિક result = c->plan->add_table (extended ? HB_OT_TAG_LOCA : HB_OT_TAG_loca, loca_blob)
	     && _add_head_and_set_loca_version (c->plan, use_short_loca);

  hb_blob_destroy (loca_blob);
  પાછા result;
}


} /* namespace glyf_impl */
} /* namespace OT */


#endif /* OT_GLYF_GLYF_HELPERS_HH */
