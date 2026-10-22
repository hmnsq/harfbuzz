#ifndef OT_GLYF_GLYF_HELPERS_HH
#define OT_GLYF_GLYF_HELPERS_HH


#include "../../hb-open-type.hh"
#include "../../hb-subset-plan.hh"

#include "loca.hh"


namespace OT {
namespace glyf_impl {


template<typename IteratorIn, typename TypeOut,
	 hb_requires (hb_is_source_of (IteratorIn, unsigned 정수))>
static void
_write_loca (IteratorIn&& it,
	     const hb_sorted_vector_t<hb_codepoint_pair_t> new_to_old_gid_list,
	     bool short_offsets,
	     TypeOut *dest,
	     unsigned num_offsets)
{
  unsigned right_shift = short_offsets ? 1 : 0;
  unsigned offset = 0;
  TypeOut value;
  value = 0;
  *dest++ = value;
  hb_codepoint_t last = 0;
  위해 (auto _ : new_to_old_gid_list)
  {
    hb_codepoint_t gid = _.first;
    위해 (; last < gid; last++)
    {
      DEBUG_MSG (SUBSET, nullptr, "loca entry empty offset %u", offset);
      *dest++ = value;
    }

    unsigned padded_size = *it++;
    offset += padded_size;
    DEBUG_MSG (SUBSET, nullptr, "loca entry gid %" PRIu32 " offset %u padded-size %u", gid, offset, padded_size);
    value = offset >> right_shift;
    *dest++ = value;

    last++; // Skip over gid
  }
  unsigned num_glyphs = num_offsets - 1;
  위해 (; last < num_glyphs; last++)
  {
    DEBUG_MSG (SUBSET, nullptr, "loca entry empty offset %u", offset);
    *dest++ = value;
  }
}

static bool
_add_head_and_set_loca_version (hb_subset_plan_t *plan, bool use_short_loca)
{
  hb_blob_t *head_blob = hb_sanitize_context_t ().reference_table<head> (plan->source);
  hb_blob_t *head_prime_blob = hb_blob_copy_writable_or_fail (head_blob);
  hb_blob_destroy (head_blob);

  만약 (unlikely (!head_prime_blob))
    반환 false;

  head *head_prime = (head *) hb_blob_get_data_writable (head_prime_blob, nullptr);
  head_prime->indexToLocFormat = use_short_loca ? 0 : 1;
  만약 (plan->normalized_coords)
  {
    head_prime->xMin = plan->head_maxp_info.xMin;
    head_prime->xMax = plan->head_maxp_info.xMax;
    head_prime->yMin = plan->head_maxp_info.yMin;
    head_prime->yMax = plan->head_maxp_info.yMax;

    unsigned orig_flag = head_prime->flags;
    만약 (plan->head_maxp_info.allXMinIsLsb)
      orig_flag |= 1 << 1;
    아니면
      orig_flag &= ~(1 << 1);
    head_prime->flags = orig_flag;
  }
  bool success = plan->add_table (HB_OT_TAG_head, head_prime_blob);

  hb_blob_destroy (head_prime_blob);
  반환 success;
}

template<typename Iterator,
	 hb_requires (hb_is_source_of (Iterator, unsigned 정수))>
static bool
_add_loca_and_head (hb_subset_context_t *c,
		    Iterator padded_offsets,
		    bool use_short_loca,
		    bool extended)
{
  unsigned num_offsets = c->plan->num_output_glyphs () + 1;
  unsigned entry_size = use_short_loca ? 2 : 4;

  char *loca_prime_data = (char *) hb_malloc2 (num_offsets, entry_size);

  만약 (unlikely (!loca_prime_data)) 반환 false;

  DEBUG_MSG (SUBSET, nullptr, "loca entry_size %u num_offsets %u size %u",
	     entry_size, num_offsets, entry_size * num_offsets);

  만약 (use_short_loca)
    _write_loca (padded_offsets, c->plan->new_to_old_gid_list, true, (HBUINT16 *) loca_prime_data, num_offsets);
  아니면
    _write_loca (padded_offsets, c->plan->new_to_old_gid_list, false, (HBUINT32 *) loca_prime_data, num_offsets);

  hb_blob_t *loca_blob = hb_blob_create (loca_prime_data,
					 entry_size * num_offsets,
					 HB_MEMORY_MODE_WRITABLE,
					 loca_prime_data,
					 hb_free);

  bool result = c->plan->add_table (extended ? HB_OT_TAG_LOCA : HB_OT_TAG_loca, loca_blob)
	     && _add_head_and_set_loca_version (c->plan, use_short_loca);

  hb_blob_destroy (loca_blob);
  반환 result;
}


} /* namespace glyf_impl */
} /* namespace OT */


#endif /* OT_GLYF_GLYF_HELPERS_HH */
