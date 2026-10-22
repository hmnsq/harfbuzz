#अगर_अघोषित OT_GLYF_GLYF_HELPERS_HH
#घोषणा OT_GLYF_GLYF_HELPERS_HH


#समावेश "../../hb-open-type.hh"
#समावेश "../../hb-subset-plan.hh"

#समावेश "loca.hh"


namespace OT {
namespace glyf_impl {


ढाँचा<typename IteratorIn, typename TypeOut,
	 hb_requires (hb_is_source_of (IteratorIn, अचिन्हित पूर्णांक))>
अटल व्योम
_write_loca (IteratorIn&& it,
	     स्थिर hb_sorted_vector_t<hb_codepoint_pair_t> new_to_old_gid_list,
	     bool short_offsets,
	     TypeOut *dest,
	     अचिन्हित अंक_offsets)
{
  अचिन्हित दायाँ_shift = short_offsets ? 1 : 0;
  अचिन्हित offset = 0;
  TypeOut value;
  value = 0;
  *dest++ = value;
  hb_codepoint_t last = 0;
  क्रम (स्वतः _ : new_to_old_gid_list)
  {
    hb_codepoint_t gid = _.first;
    क्रम (; last < gid; last++)
    {
      DEBUG_संदेश (SUBSET, nullptr, "loca entry empty offset %u", offset);
      *dest++ = value;
    }

    अचिन्हित padded_आकार = *it++;
    offset += padded_आकार;
    DEBUG_संदेश (SUBSET, nullptr, "loca entry gid %" PRIu32 " offset %u padded-size %u", gid, offset, padded_आकार);
    value = offset >> दायाँ_shift;
    *dest++ = value;

    last++; // Skip over gid
  }
  अचिन्हित अंक_glyphs = अंक_offsets - 1;
  क्रम (; last < अंक_glyphs; last++)
  {
    DEBUG_संदेश (SUBSET, nullptr, "loca entry empty offset %u", offset);
    *dest++ = value;
  }
}

अटल bool
add_शीर्ष_and_set_loca_version (hb_subset_plan_t *plan, bool use_short_loca)
{
  hb_blob_t *शीर्ष_blob = hb_sanitize_context_t ().reference_table<शीर्ष_2> (plan->source);
  hb_blob_t *शीर्ष_अभाज्य_blob = hb_blob_copy_writable_or_fail (शीर्ष_blob);
  hb_blob_destroy (शीर्ष_blob);

  अगर (unlikely (!शीर्ष_अभाज्य_blob))
    वापस false;

  शीर्ष_2 *शीर्ष_अभाज्य = (शीर्ष_2 *) hb_blob_get_तथ्य_writable (शीर्ष_अभाज्य_blob, nullptr);
  शीर्ष_अभाज्य->सूचकांक_To_Loc_Format = use_short_loca ? 0 : 1;
  अगर (plan->normalized_coords)
  {
    शीर्ष_अभाज्य->xMin = plan->शीर्ष_maxp_info.xMin;
    शीर्ष_अभाज्य->xMax = plan->शीर्ष_maxp_info.xMax;
    शीर्ष_अभाज्य->yMin = plan->शीर्ष_maxp_info.yMin;
    शीर्ष_अभाज्य->yMax = plan->शीर्ष_maxp_info.yMax;

    अचिन्हित orig_flag = शीर्ष_अभाज्य->flags;
    अगर (plan->शीर्ष_maxp_info.allXMinIsLsb)
      orig_flag |= 1 << 1;
    अन्यथा
      orig_flag &= ~(1 << 1);
    शीर्ष_अभाज्य->flags = orig_flag;
  }
  bool success = plan->add_table (HB_OT_TAG_शीर्ष, शीर्ष_अभाज्य_blob);

  hb_blob_destroy (शीर्ष_अभाज्य_blob);
  वापस success;
}

ढाँचा<typename Iterator,
	 hb_requires (hb_is_source_of (Iterator, अचिन्हित पूर्णांक))>
अटल bool
add_loca_and_शीर्ष (hb_subset_context_t *c,
		    Iterator padded_offsets,
		    bool use_short_loca,
		    bool extended)
{
  अचिन्हित अंक_offsets = c->plan->अंक_output_glyphs () + 1;
  अचिन्हित entry_आकार = use_short_loca ? 2 : 4;

  अक्षर *loca_अभाज्य_तथ्य = (अक्षर *) hb_malloc2 (अंक_offsets, entry_आकार);

  अगर (unlikely (!loca_अभाज्य_तथ्य)) वापस false;

  DEBUG_संदेश (SUBSET, nullptr, "loca entry_size %u num_offsets %u size %u",
	     entry_आकार, अंक_offsets, entry_आकार * अंक_offsets);

  अगर (use_short_loca)
    _write_loca (padded_offsets, c->plan->new_to_old_gid_list, true, (HBUINT16 *) loca_अभाज्य_तथ्य, अंक_offsets);
  अन्यथा
    _write_loca (padded_offsets, c->plan->new_to_old_gid_list, false, (HBUINT32 *) loca_अभाज्य_तथ्य, अंक_offsets);

  hb_blob_t *loca_blob = hb_blob_create (loca_अभाज्य_तथ्य,
					 entry_आकार * अंक_offsets,
					 HB_MEMORY_MODE_WRITABLE,
					 loca_अभाज्य_तथ्य,
					 hb_free);

  bool परिणाम = c->plan->add_table (extended ? HB_OT_TAG_LOCA : HB_OT_TAG_loca, loca_blob)
	     && add_शीर्ष_and_set_loca_version (c->plan, use_short_loca);

  hb_blob_destroy (loca_blob);
  वापस परिणाम;
}


} /* namespace glyf_impl */
} /* namespace OT */


#पूर्ण_अगर /* OT_GLYF_GLYF_HELPERS_HH */
