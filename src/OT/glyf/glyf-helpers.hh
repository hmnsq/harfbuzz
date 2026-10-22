#ifndef OT_GLYF_GLYF_HELPERS_HH
#define OT_GLYF_GLYF_HELPERS_HH


#অন্তর্ভাৱ "../../hb-open-type.hh"
#অন্তর্ভাৱ "../../hb-subset-plan.hh"

#অন্তর্ভাৱ "loca.hh"


নামাকাশ OT {
নামাকাশ glyf_impl {


template<typename IteratorIn, typename TypeOut,
	 hb_requires (hb_is_source_of (IteratorIn, unsigned পূর্ণাঙ্ক))>
স্থির শূন্য
_write_loca (IteratorIn&& it,
	     ধ্রুৱ hb_sorted_vector_t<hb_codepoint_pair_t> new_to_old_gid_list,
	     তার্কিক short_offsets,
	     TypeOut *dest,
	     unsigned অঙ্ক_offsets)
{
  unsigned ডান_shift = short_offsets ? 1 : 0;
  unsigned offset = 0;
  TypeOut value;
  value = 0;
  *dest++ = value;
  hb_codepoint_t last = 0;
  বাবে (auto _ : new_to_old_gid_list)
  {
    hb_codepoint_t gid = _.first;
    বাবে (; last < gid; last++)
    {
      DEBUG_বাৰ্তা (SUBSET, nullptr, "loca entry empty offset %u", offset);
      *dest++ = value;
    }

    unsigned padded_size = *it++;
    offset += padded_size;
    DEBUG_বাৰ্তা (SUBSET, nullptr, "loca entry gid %" PRIu32 " offset %u padded-size %u", gid, offset, padded_size);
    value = offset >> ডান_shift;
    *dest++ = value;

    last++; // Skip over gid
  }
  unsigned অঙ্ক_glyphs = অঙ্ক_offsets - 1;
  বাবে (; last < অঙ্ক_glyphs; last++)
  {
    DEBUG_বাৰ্তা (SUBSET, nullptr, "loca entry empty offset %u", offset);
    *dest++ = value;
  }
}

স্থির তার্কিক
_add_head_and_set_loca_version (hb_subset_plan_t *plan, তার্কিক use_short_loca)
{
  hb_blob_t *head_blob = hb_sanitize_context_t ().reference_table<head> (plan->source);
  hb_blob_t *head_মৌলিক_blob = hb_blob_copy_writable_or_fail (head_blob);
  hb_blob_destroy (head_blob);

  যদি (unlikely (!head_মৌলিক_blob))
    ঘূৰাই মিছা;

  head *head_মৌলিক = (head *) hb_blob_get_data_writable (head_মৌলিক_blob, nullptr);
  head_মৌলিক->indexToLocFormat = use_short_loca ? 0 : 1;
  যদি (plan->normalized_coords)
  {
    head_মৌলিক->xMin = plan->head_maxp_info.xMin;
    head_মৌলিক->xMax = plan->head_maxp_info.xMax;
    head_মৌলিক->yMin = plan->head_maxp_info.yMin;
    head_মৌলিক->yMax = plan->head_maxp_info.yMax;

    unsigned orig_flag = head_মৌলিক->flags;
    যদি (plan->head_maxp_info.allXMinIsLsb)
      orig_flag |= 1 << 1;
    অন্যথা
      orig_flag &= ~(1 << 1);
    head_মৌলিক->flags = orig_flag;
  }
  তার্কিক success = plan->add_table (HB_OT_TAG_head, head_মৌলিক_blob);

  hb_blob_destroy (head_মৌলিক_blob);
  ঘূৰাই success;
}

template<typename Iterator,
	 hb_requires (hb_is_source_of (Iterator, unsigned পূর্ণাঙ্ক))>
স্থির তার্কিক
_add_loca_and_head (hb_subset_context_t *c,
		    Iterator padded_offsets,
		    তার্কিক use_short_loca,
		    তার্কিক extended)
{
  unsigned অঙ্ক_offsets = c->plan->অঙ্ক_output_glyphs () + 1;
  unsigned entry_size = use_short_loca ? 2 : 4;

  ৱর্ণ *loca_মৌলিক_data = (ৱর্ণ *) hb_malloc2 (অঙ্ক_offsets, entry_size);

  যদি (unlikely (!loca_মৌলিক_data)) ঘূৰাই মিছা;

  DEBUG_বাৰ্তা (SUBSET, nullptr, "loca entry_size %u num_offsets %u size %u",
	     entry_size, অঙ্ক_offsets, entry_size * অঙ্ক_offsets);

  যদি (use_short_loca)
    _write_loca (padded_offsets, c->plan->new_to_old_gid_list, সঁচা, (HBUINT16 *) loca_মৌলিক_data, অঙ্ক_offsets);
  অন্যথা
    _write_loca (padded_offsets, c->plan->new_to_old_gid_list, মিছা, (HBUINT32 *) loca_মৌলিক_data, অঙ্ক_offsets);

  hb_blob_t *loca_blob = hb_blob_create (loca_মৌলিক_data,
					 entry_size * অঙ্ক_offsets,
					 HB_MEMORY_MODE_WRITABLE,
					 loca_মৌলিক_data,
					 hb_free);

  তার্কিক result = c->plan->add_table (extended ? HB_OT_TAG_LOCA : HB_OT_TAG_loca, loca_blob)
	     && _add_head_and_set_loca_version (c->plan, use_short_loca);

  hb_blob_destroy (loca_blob);
  ঘূৰাই result;
}


} /* namespace glyf_impl */
} /* namespace OT */


#endif /* OT_GLYF_GLYF_HELPERS_HH */
