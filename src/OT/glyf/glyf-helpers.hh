#ifndef OT_GLYF_GLYF_HELPERS_HH
#define OT_GLYF_GLYF_HELPERS_HH


#ضمن "../../hb-open-type.hh"
#ضمن "../../hb-subset-plan.hh"

#ضمن "loca.hh"


نطاق OT {
نطاق glyf_impl {


template<typename IteratorIn, typename TypeOut,
	 hb_requires (hb_is_source_of (IteratorIn, unsigned صحيح))>
ساكن فراغ
_write_loca (IteratorIn&& it,
	     ثابت hb_sorted_vector_t<hb_codepoint_pair_t> new_to_old_gid_list,
	     منطقي short_offsets,
	     TypeOut *dest,
	     unsigned عدد_offsets)
{
  unsigned دایاں_shift = short_offsets ? 1 : 0;
  unsigned offset = 0;
  TypeOut value;
  value = 0;
  *dest++ = value;
  hb_codepoint_t last = 0;
  لكل (auto _ : new_to_old_gid_list)
  {
    hb_codepoint_t gid = _.first;
    لكل (; last < gid; last++)
    {
      DEBUG_پیغام (SUBSET, nullptr, "loca entry empty offset %u", offset);
      *dest++ = value;
    }

    unsigned padded_size = *it++;
    offset += padded_size;
    DEBUG_پیغام (SUBSET, nullptr, "loca entry gid %" PRIu32 " offset %u padded-size %u", gid, offset, padded_size);
    value = offset >> دایاں_shift;
    *dest++ = value;

    last++; // Skip over gid
  }
  unsigned عدد_glyphs = عدد_offsets - 1;
  لكل (; last < عدد_glyphs; last++)
  {
    DEBUG_پیغام (SUBSET, nullptr, "loca entry empty offset %u", offset);
    *dest++ = value;
  }
}

ساكن منطقي
_add_head_and_set_loca_version (hb_subset_plan_t *plan, منطقي use_short_loca)
{
  hb_blob_t *head_blob = hb_sanitize_context_t ().reference_table<head> (plan->source);
  hb_blob_t *head_مفرد_blob = hb_blob_copy_writable_or_fail (head_blob);
  hb_blob_destroy (head_blob);

  إذا (unlikely (!head_مفرد_blob))
    أرجع خطأ;

  head *head_مفرد = (head *) hb_blob_get_data_writable (head_مفرد_blob, nullptr);
  head_مفرد->indexToLocFormat = use_short_loca ? 0 : 1;
  إذا (plan->normalized_coords)
  {
    head_مفرد->xMin = plan->head_maxp_info.xMin;
    head_مفرد->xMax = plan->head_maxp_info.xMax;
    head_مفرد->yMin = plan->head_maxp_info.yMin;
    head_مفرد->yMax = plan->head_maxp_info.yMax;

    unsigned orig_flag = head_مفرد->flags;
    إذا (plan->head_maxp_info.allXMinIsLsb)
      orig_flag |= 1 << 1;
    وإلا
      orig_flag &= ~(1 << 1);
    head_مفرد->flags = orig_flag;
  }
  منطقي success = plan->add_table (HB_OT_TAG_head, head_مفرد_blob);

  hb_blob_destroy (head_مفرد_blob);
  أرجع success;
}

template<typename Iterator,
	 hb_requires (hb_is_source_of (Iterator, unsigned صحيح))>
ساكن منطقي
_add_loca_and_head (hb_subset_context_t *c,
		    Iterator padded_offsets,
		    منطقي use_short_loca,
		    منطقي extended)
{
  unsigned عدد_offsets = c->plan->عدد_output_glyphs () + 1;
  unsigned entry_size = use_short_loca ? 2 : 4;

  حرف *loca_مفرد_data = (حرف *) hb_malloc2 (عدد_offsets, entry_size);

  إذا (unlikely (!loca_مفرد_data)) أرجع خطأ;

  DEBUG_پیغام (SUBSET, nullptr, "loca entry_size %u num_offsets %u size %u",
	     entry_size, عدد_offsets, entry_size * عدد_offsets);

  إذا (use_short_loca)
    _write_loca (padded_offsets, c->plan->new_to_old_gid_list, true, (HBUINT16 *) loca_مفرد_data, عدد_offsets);
  وإلا
    _write_loca (padded_offsets, c->plan->new_to_old_gid_list, خطأ, (HBUINT32 *) loca_مفرد_data, عدد_offsets);

  hb_blob_t *loca_blob = hb_blob_create (loca_مفرد_data,
					 entry_size * عدد_offsets,
					 HB_MEMORY_MODE_WRITABLE,
					 loca_مفرد_data,
					 hb_free);

  منطقي result = c->plan->add_table (extended ? HB_OT_TAG_LOCA : HB_OT_TAG_loca, loca_blob)
	     && _add_head_and_set_loca_version (c->plan, use_short_loca);

  hb_blob_destroy (loca_blob);
  أرجع result;
}


} /* namespace glyf_impl */
} /* namespace OT */


#endif /* OT_GLYF_GLYF_HELPERS_HH */
