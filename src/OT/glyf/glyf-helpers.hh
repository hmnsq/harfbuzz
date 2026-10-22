#ifndef OT_GLYF_GLYF_HELPERS_HH
#define OT_GLYF_GLYF_HELPERS_HH


#अन्तर्भाव "../../hb-open-type.hh"
#अन्तर्भाव "../../hb-subset-plan.hh"

#अन्तर्भाव "loca.hh"


नामाकाश OT {
नामाकाश glyf_impl {


ढाँचा<typename IteratorIn, typename TypeOut,
	 hb_requires (hb_is_source_of (IteratorIn, अचिन्हित पूर्णाङ्क))>
स्थिर शून्य
_write_loca (IteratorIn&& it,
	     अचर hb_sorted_vector_t<hb_codepoint_pair_t> new_to_old_gid_list,
	     तार्किक short_offsets,
	     TypeOut *dest,
	     अचिन्हित अङ्क_offsets)
{
  अचिन्हित दायाँ_shift = short_offsets ? 1 : 0;
  अचिन्हित offset = 0;
  TypeOut value;
  value = 0;
  *dest++ = value;
  hb_codepoint_t last = 0;
  लागि (स्वतः _ : new_to_old_gid_list)
  {
    hb_codepoint_t gid = _.first;
    लागि (; last < gid; last++)
    {
      DEBUG_सन्देश (SUBSET, nullptr, "loca entry empty offset %u", offset);
      *dest++ = value;
    }

    अचिन्हित padded_size = *it++;
    offset += padded_size;
    DEBUG_सन्देश (SUBSET, nullptr, "loca entry gid %" PRIu32 " offset %u padded-size %u", gid, offset, padded_size);
    value = offset >> दायाँ_shift;
    *dest++ = value;

    last++; // Skip over gid
  }
  अचिन्हित अङ्क_glyphs = अङ्क_offsets - 1;
  लागि (; last < अङ्क_glyphs; last++)
  {
    DEBUG_सन्देश (SUBSET, nullptr, "loca entry empty offset %u", offset);
    *dest++ = value;
  }
}

स्थिर तार्किक
_add_head_and_set_loca_version (hb_subset_plan_t *plan, तार्किक use_short_loca)
{
  hb_blob_t *head_blob = hb_sanitize_context_t ().reference_table<head> (plan->source);
  hb_blob_t *head_अविभाज्य_blob = hb_blob_copy_writable_or_fail (head_blob);
  hb_blob_destroy (head_blob);

  यदि (unlikely (!head_अविभाज्य_blob))
    फर्काऊ असत्य;

  head *head_अविभाज्य = (head *) hb_blob_get_data_writable (head_अविभाज्य_blob, nullptr);
  head_अविभाज्य->indexToLocFormat = use_short_loca ? 0 : 1;
  यदि (plan->normalized_coords)
  {
    head_अविभाज्य->xMin = plan->head_maxp_info.xMin;
    head_अविभाज्य->xMax = plan->head_maxp_info.xMax;
    head_अविभाज्य->yMin = plan->head_maxp_info.yMin;
    head_अविभाज्य->yMax = plan->head_maxp_info.yMax;

    अचिन्हित orig_flag = head_अविभाज्य->flags;
    यदि (plan->head_maxp_info.allXMinIsLsb)
      orig_flag |= 1 << 1;
    अन्यथा
      orig_flag &= ~(1 << 1);
    head_अविभाज्य->flags = orig_flag;
  }
  तार्किक success = plan->add_table (HB_OT_TAG_head, head_अविभाज्य_blob);

  hb_blob_destroy (head_अविभाज्य_blob);
  फर्काऊ success;
}

ढाँचा<typename Iterator,
	 hb_requires (hb_is_source_of (Iterator, अचिन्हित पूर्णाङ्क))>
स्थिर तार्किक
_add_loca_and_head (hb_subset_context_t *c,
		    Iterator padded_offsets,
		    तार्किक use_short_loca,
		    तार्किक extended)
{
  अचिन्हित अङ्क_offsets = c->plan->अङ्क_output_glyphs () + 1;
  अचिन्हित entry_size = use_short_loca ? 2 : 4;

  वर्ण *loca_अविभाज्य_data = (वर्ण *) hb_malloc2 (अङ्क_offsets, entry_size);

  यदि (unlikely (!loca_अविभाज्य_data)) फर्काऊ असत्य;

  DEBUG_सन्देश (SUBSET, nullptr, "loca entry_size %u num_offsets %u size %u",
	     entry_size, अङ्क_offsets, entry_size * अङ्क_offsets);

  यदि (use_short_loca)
    _write_loca (padded_offsets, c->plan->new_to_old_gid_list, सत्य, (HBUINT16 *) loca_अविभाज्य_data, अङ्क_offsets);
  अन्यथा
    _write_loca (padded_offsets, c->plan->new_to_old_gid_list, असत्य, (HBUINT32 *) loca_अविभाज्य_data, अङ्क_offsets);

  hb_blob_t *loca_blob = hb_blob_create (loca_अविभाज्य_data,
					 entry_size * अङ्क_offsets,
					 HB_MEMORY_MODE_WRITABLE,
					 loca_अविभाज्य_data,
					 hb_free);

  तार्किक result = c->plan->add_table (extended ? HB_OT_TAG_LOCA : HB_OT_TAG_loca, loca_blob)
	     && _add_head_and_set_loca_version (c->plan, use_short_loca);

  hb_blob_destroy (loca_blob);
  फर्काऊ result;
}


} /* namespace glyf_impl */
} /* namespace OT */


#endif /* OT_GLYF_GLYF_HELPERS_HH */
