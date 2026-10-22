#ifndef OT_GLYF_GLYF_HELPERS_HH
#define OT_GLYF_GLYF_HELPERS_HH


#অন্তর্ভাব "../../hb-open-type.hh"
#অন্তর্ভাব "../../hb-subset-plan.hh"

#অন্তর্ভাব "loca.hh"


নামাকাশ OT {
নামাকাশ glyf_impl {


template<typename IteratorIn, typename TypeOut,
	 hb_requires (hb_is_source_of (IteratorIn, unsigned পূর্ণসংখ্যা))>
স্থির শূন্য
_write_loca (IteratorIn&& it,
	     ধ্রুবক hb_sorted_vector_t<hb_codepoint_pair_t> new_to_old_gid_list,
	     যৌক্তিক short_offsets,
	     TypeOut *dest,
	     unsigned অঙ্ক_offsets)
{
  unsigned ডান_shift = short_offsets ? 1 : 0;
  unsigned offset = 0;
  TypeOut value;
  value = 0;
  *dest++ = value;
  hb_codepoint_t last = 0;
  জন্য (auto _ : new_to_old_gid_list)
  {
    hb_codepoint_t gid = _.first;
    জন্য (; last < gid; last++)
    {
      DEBUG_বার্তা (SUBSET, nullptr, "loca entry empty offset %u", offset);
      *dest++ = value;
    }

    unsigned padded_আকার = *it++;
    offset += padded_আকার;
    DEBUG_বার্তা (SUBSET, nullptr, "loca entry gid %" PRIu32 " offset %u padded-size %u", gid, offset, padded_আকার);
    value = offset >> ডান_shift;
    *dest++ = value;

    last++; // Skip over gid
  }
  unsigned অঙ্ক_glyphs = অঙ্ক_offsets - 1;
  জন্য (; last < অঙ্ক_glyphs; last++)
  {
    DEBUG_বার্তা (SUBSET, nullptr, "loca entry empty offset %u", offset);
    *dest++ = value;
  }
}

স্থির যৌক্তিক
add_শীর্ষ_and_set_loca_version (hb_subset_plan_t *plan, যৌক্তিক use_short_loca)
{
  hb_blob_t *শীর্ষ_blob = hb_sanitize_context_t ().reference_table<শীর্ষ_2> (plan->source);
  hb_blob_t *শীর্ষ_মৌলিক_blob = hb_blob_copy_writable_or_fail (শীর্ষ_blob);
  hb_blob_destroy (শীর্ষ_blob);

  যদি (unlikely (!শীর্ষ_মৌলিক_blob))
    ফেরত মিথ্যা;

  শীর্ষ_2 *শীর্ষ_মৌলিক = (শীর্ষ_2 *) hb_blob_get_তথ্য_writable (শীর্ষ_মৌলিক_blob, nullptr);
  শীর্ষ_মৌলিক->সূচক_To_Loc_Format = use_short_loca ? 0 : 1;
  যদি (plan->normalized_coords)
  {
    শীর্ষ_মৌলিক->xMin = plan->শীর্ষ_maxp_info.xMin;
    শীর্ষ_মৌলিক->xMax = plan->শীর্ষ_maxp_info.xMax;
    শীর্ষ_মৌলিক->yMin = plan->শীর্ষ_maxp_info.yMin;
    শীর্ষ_মৌলিক->yMax = plan->শীর্ষ_maxp_info.yMax;

    unsigned orig_flag = শীর্ষ_মৌলিক->flags;
    যদি (plan->শীর্ষ_maxp_info.allXMinIsLsb)
      orig_flag |= 1 << 1;
    অন্যথা
      orig_flag &= ~(1 << 1);
    শীর্ষ_মৌলিক->flags = orig_flag;
  }
  যৌক্তিক success = plan->add_table (HB_OT_TAG_শীর্ষ, শীর্ষ_মৌলিক_blob);

  hb_blob_destroy (শীর্ষ_মৌলিক_blob);
  ফেরত success;
}

template<typename Iterator,
	 hb_requires (hb_is_source_of (Iterator, unsigned পূর্ণসংখ্যা))>
স্থির যৌক্তিক
add_loca_and_শীর্ষ (hb_subset_context_t *c,
		    Iterator padded_offsets,
		    যৌক্তিক use_short_loca,
		    যৌক্তিক extended)
{
  unsigned অঙ্ক_offsets = c->plan->অঙ্ক_output_glyphs () + 1;
  unsigned entry_আকার = use_short_loca ? 2 : 4;

  অক্ষর *loca_মৌলিক_তথ্য = (অক্ষর *) hb_malloc2 (অঙ্ক_offsets, entry_আকার);

  যদি (unlikely (!loca_মৌলিক_তথ্য)) ফেরত মিথ্যা;

  DEBUG_বার্তা (SUBSET, nullptr, "loca entry_size %u num_offsets %u size %u",
	     entry_আকার, অঙ্ক_offsets, entry_আকার * অঙ্ক_offsets);

  যদি (use_short_loca)
    _write_loca (padded_offsets, c->plan->new_to_old_gid_list, সত্য, (HBUINT16 *) loca_মৌলিক_তথ্য, অঙ্ক_offsets);
  অন্যথা
    _write_loca (padded_offsets, c->plan->new_to_old_gid_list, মিথ্যা, (HBUINT32 *) loca_মৌলিক_তথ্য, অঙ্ক_offsets);

  hb_blob_t *loca_blob = hb_blob_create (loca_মৌলিক_তথ্য,
					 entry_আকার * অঙ্ক_offsets,
					 HB_MEMORY_MODE_WRITABLE,
					 loca_মৌলিক_তথ্য,
					 hb_free);

  যৌক্তিক ফলাফল = c->plan->add_table (extended ? HB_OT_TAG_LOCA : HB_OT_TAG_loca, loca_blob)
	     && add_শীর্ষ_and_set_loca_version (c->plan, use_short_loca);

  hb_blob_destroy (loca_blob);
  ফেরত ফলাফল;
}


} /* namespace glyf_impl */
} /* namespace OT */


#endif /* OT_GLYF_GLYF_HELPERS_HH */
