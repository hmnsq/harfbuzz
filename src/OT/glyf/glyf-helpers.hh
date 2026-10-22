#ifndef OT_GLYF_GLYF_HELPERS_HH
#define OT_GLYF_GLYF_HELPERS_HH


#شامل "../../hb-open-type.hh"
#شامل "../../hb-subset-plan.hh"

#شامل "loca.hh"


فضاینام OT {
فضاینام glyf_impl {


template<typename IteratorIn, typename TypeOut,
	 hb_requires (hb_is_source_of (IteratorIn, unsigned صحیح))>
ایستا تهی
_write_loca (IteratorIn&& it,
	     ثابت hb_sorted_vector_t<hb_codepoint_pair_t> new_to_old_gid_list,
	     منطقی short_offsets,
	     TypeOut *dest,
	     unsigned رقم_offsets)
{
  unsigned راست_shift = short_offsets ? 1 : 0;
  unsigned offset = 0;
  TypeOut value;
  value = 0;
  *dest++ = value;
  hb_codepoint_t last = 0;
  برای (auto _ : new_to_old_gid_list)
  {
    hb_codepoint_t gid = _.first;
    برای (; last < gid; last++)
    {
      DEBUG_پیام (SUBSET, nullptr, "loca entry empty offset %u", offset);
      *dest++ = value;
    }

    unsigned padded_size = *it++;
    offset += padded_size;
    DEBUG_پیام (SUBSET, nullptr, "loca entry gid %" PRIu32 " offset %u padded-size %u", gid, offset, padded_size);
    value = offset >> راست_shift;
    *dest++ = value;

    last++; // Skip over gid
  }
  unsigned رقم_glyphs = رقم_offsets - 1;
  برای (; last < رقم_glyphs; last++)
  {
    DEBUG_پیام (SUBSET, nullptr, "loca entry empty offset %u", offset);
    *dest++ = value;
  }
}

ایستا منطقی
_add_head_and_set_loca_version (hb_subset_plan_t *plan, منطقی use_short_loca)
{
  hb_blob_t *head_blob = hb_sanitize_context_t ().reference_table<head> (plan->source);
  hb_blob_t *head_اول_blob = hb_blob_copy_writable_or_fail (head_blob);
  hb_blob_destroy (head_blob);

  اگر (unlikely (!head_اول_blob))
    بازگشت نادرست;

  head *head_اول = (head *) hb_blob_get_data_writable (head_اول_blob, nullptr);
  head_اول->indexToLocFormat = use_short_loca ? 0 : 1;
  اگر (plan->normalized_coords)
  {
    head_اول->xMin = plan->head_maxp_info.xMin;
    head_اول->xMax = plan->head_maxp_info.xMax;
    head_اول->yMin = plan->head_maxp_info.yMin;
    head_اول->yMax = plan->head_maxp_info.yMax;

    unsigned orig_flag = head_اول->flags;
    اگر (plan->head_maxp_info.allXMinIsLsb)
      orig_flag |= 1 << 1;
    وگرنه
      orig_flag &= ~(1 << 1);
    head_اول->flags = orig_flag;
  }
  منطقی success = plan->add_table (HB_OT_TAG_head, head_اول_blob);

  hb_blob_destroy (head_اول_blob);
  بازگشت success;
}

template<typename Iterator,
	 hb_requires (hb_is_source_of (Iterator, unsigned صحیح))>
ایستا منطقی
_add_loca_and_head (hb_subset_context_t *c,
		    Iterator padded_offsets,
		    منطقی use_short_loca,
		    منطقی extended)
{
  unsigned رقم_offsets = c->plan->رقم_output_glyphs () + 1;
  unsigned entry_size = use_short_loca ? 2 : 4;

  نویسه *loca_اول_data = (نویسه *) hb_malloc2 (رقم_offsets, entry_size);

  اگر (unlikely (!loca_اول_data)) بازگشت نادرست;

  DEBUG_پیام (SUBSET, nullptr, "loca entry_size %u num_offsets %u size %u",
	     entry_size, رقم_offsets, entry_size * رقم_offsets);

  اگر (use_short_loca)
    _write_loca (padded_offsets, c->plan->new_to_old_gid_list, درست, (HBUINT16 *) loca_اول_data, رقم_offsets);
  وگرنه
    _write_loca (padded_offsets, c->plan->new_to_old_gid_list, نادرست, (HBUINT32 *) loca_اول_data, رقم_offsets);

  hb_blob_t *loca_blob = hb_blob_create (loca_اول_data,
					 entry_size * رقم_offsets,
					 HB_MEMORY_MODE_WRITABLE,
					 loca_اول_data,
					 hb_free);

  منطقی result = c->plan->add_table (extended ? HB_OT_TAG_LOCA : HB_OT_TAG_loca, loca_blob)
	     && _add_head_and_set_loca_version (c->plan, use_short_loca);

  hb_blob_destroy (loca_blob);
  بازگشت result;
}


} /* namespace glyf_impl */
} /* namespace OT */


#endif /* OT_GLYF_GLYF_HELPERS_HH */
