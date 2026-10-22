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
	     unsigned اڠک_offsets)
{
  unsigned کانن_shift = short_offsets ? 1 : 0;
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
      DEBUG_ڤسنن (SUBSET, nullptr, "loca entry empty offset %u", offset);
      *dest++ = value;
    }

    unsigned padded_size = *it++;
    offset += padded_size;
    DEBUG_ڤسنن (SUBSET, nullptr, "loca entry gid %" PRIu32 " offset %u padded-size %u", gid, offset, padded_size);
    value = offset >> کانن_shift;
    *dest++ = value;

    last++; // Skip over gid
  }
  unsigned اڠک_glyphs = اڠک_offsets - 1;
  لكل (; last < اڠک_glyphs; last++)
  {
    DEBUG_ڤسنن (SUBSET, nullptr, "loca entry empty offset %u", offset);
    *dest++ = value;
  }
}

ساكن منطقي
_add_head_and_set_loca_version (hb_subset_plan_t *plan, منطقي use_short_loca)
{
  hb_blob_t *head_blob = hb_sanitize_context_t ().reference_table<head> (plan->source);
  hb_blob_t *head_ڤردان_blob = hb_blob_copy_writable_or_fail (head_blob);
  hb_blob_destroy (head_blob);

  إذا (unlikely (!head_ڤردان_blob))
    أرجع خطأ;

  head *head_ڤردان = (head *) hb_blob_get_data_writable (head_ڤردان_blob, nullptr);
  head_ڤردان->indexToLocFormat = use_short_loca ? 0 : 1;
  إذا (plan->normalized_coords)
  {
    head_ڤردان->xMin = plan->head_maxp_info.xMin;
    head_ڤردان->xMax = plan->head_maxp_info.xMax;
    head_ڤردان->yMin = plan->head_maxp_info.yMin;
    head_ڤردان->yMax = plan->head_maxp_info.yMax;

    unsigned orig_flag = head_ڤردان->flags;
    إذا (plan->head_maxp_info.allXMinIsLsb)
      orig_flag |= 1 << 1;
    وإلا
      orig_flag &= ~(1 << 1);
    head_ڤردان->flags = orig_flag;
  }
  منطقي success = plan->add_table (HB_OT_TAG_head, head_ڤردان_blob);

  hb_blob_destroy (head_ڤردان_blob);
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
  unsigned اڠک_offsets = c->plan->اڠک_output_glyphs () + 1;
  unsigned entry_size = use_short_loca ? 2 : 4;

  حرف *loca_ڤردان_data = (حرف *) hb_malloc2 (اڠک_offsets, entry_size);

  إذا (unlikely (!loca_ڤردان_data)) أرجع خطأ;

  DEBUG_ڤسنن (SUBSET, nullptr, "loca entry_size %u num_offsets %u size %u",
	     entry_size, اڠک_offsets, entry_size * اڠک_offsets);

  إذا (use_short_loca)
    _write_loca (padded_offsets, c->plan->new_to_old_gid_list, true, (HBUINT16 *) loca_ڤردان_data, اڠک_offsets);
  وإلا
    _write_loca (padded_offsets, c->plan->new_to_old_gid_list, خطأ, (HBUINT32 *) loca_ڤردان_data, اڠک_offsets);

  hb_blob_t *loca_blob = hb_blob_create (loca_ڤردان_data,
					 entry_size * اڠک_offsets,
					 HB_MEMORY_MODE_WRITABLE,
					 loca_ڤردان_data,
					 hb_free);

  منطقي result = c->plan->add_table (extended ? HB_OT_TAG_LOCA : HB_OT_TAG_loca, loca_blob)
	     && _add_head_and_set_loca_version (c->plan, use_short_loca);

  hb_blob_destroy (loca_blob);
  أرجع result;
}


} /* namespace glyf_impl */
} /* namespace OT */


#endif /* OT_GLYF_GLYF_HELPERS_HH */
