#ifndef OT_LAYOUT_GPOS_PAIRPOSFORMAT1_HH
#define OT_LAYOUT_GPOS_PAIRPOSFORMAT1_HH

#include "PairSet.hh"

namespace OT {
namespace Layout {
namespace GPOS_impl {


template <typename Types>
struct PairPosFormat1_3
{
  using PairSet = GPOS_impl::PairSet<Types>;
  using PairValueRecord = GPOS_impl::PairValueRecord<Types>;

  protected:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of subtable */
  ValueFormat   valueFormat[2];         /* [0] Defines the types of data in
                                         * ValueRecord1--for the first glyph
                                         * in the pair--may be zero (0) */
                                        /* [1] Defines the types of data in
                                         * ValueRecord2--for the second glyph
                                         * in the pair--may be zero (0) */
  typename Types::template ArrayOf<typename Types::template OffsetTo<PairSet>>
                pairSet;                /* Array of PairSet tables ordered by
                                         * Coverage Index */
  public:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + 2 * ValueFormat::static_size + Types::HBUINT::static_size, pairSet);

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);

    如果 (!c->check_struct (this)) return_trace (false);
    hb_barrier ();

    unsigned 整数 len1 = valueFormat[0].get_len ();
    unsigned 整数 len2 = valueFormat[1].get_len ();
    typename PairSet::sanitize_closure_t closure =
    {
      valueFormat,
      len1,
      PairSet::get_size (len1, len2)
    };

    return_trace (coverage.sanitize (c, this) && pairSet.sanitize (c, this, &closure));
  }

  bool intersects (const hb_set_t *glyphs) const
  {
    auto &cov = this+coverage;

    如果 (pairSet.len > glyphs->get_population () * hb_bit_storage ((unsigned) pairSet.len))
    {
      为 (hb_codepoint_t g : glyphs->iter ())
      {
	unsigned i = cov.get_coverage (g);
	如果 ((this+pairSet[i]).intersects (glyphs, valueFormat))
	  返回 true;
      }
      返回 false;
    }

    返回
    + hb_zip (cov, pairSet)
    | hb_filter (*glyphs, hb_first)
    | hb_map (hb_second)
    | hb_map ([glyphs, this] (const typename Types::template OffsetTo<PairSet> &_)
              { 返回 (this+_).intersects (glyphs, valueFormat); })
    | hb_any
    ;
  }

  void closure_lookups (hb_closure_lookups_context_t *c) const {}
  void collect_variation_indices (hb_collect_variation_indices_context_t *c) const
  {
    如果 ((!valueFormat[0].has_device ()) && (!valueFormat[1].has_device ())) 返回;

    auto it =
    + hb_zip (this+coverage, pairSet)
    | hb_filter (c->glyph_set, hb_first)
    | hb_map (hb_second)
    ;

    如果 (!it) 返回;
    + it
    | hb_map (hb_add (this))
    | hb_apply ([&] (const PairSet& _) { _.collect_variation_indices (c, valueFormat); })
    ;
  }

  void collect_glyphs (hb_collect_glyphs_context_t *c) const
  {
    如果 (unlikely (!(this+coverage).collect_coverage (c->input))) 返回;
    unsigned 整数 count = pairSet.len;
    为 (unsigned 整数 i = 0; i < count; i++)
      (this+pairSet[i]).collect_glyphs (c, valueFormat);
  }

  void collect_second_glyphs (hb_set_digest_t *digest) const
  {
    unsigned count = pairSet.len;
    为 (unsigned i = 0; i < count; i++)
      (this+pairSet[i]).collect_second_glyphs (digest, valueFormat);
  }

  const Coverage &get_coverage () const { 返回 this+coverage; }

  struct external_cache_t
  {
    hb_ot_layout_mapping_cache_t coverage;
    hb_set_digest_t pair_sets[HB_VAR_ARRAY];
  };
  void *external_cache_create () const
  {
    unsigned count = pairSet.len;
    unsigned size = sizeof (external_cache_t) -
		    HB_VAR_ARRAY * sizeof (hb_set_digest_t) +
		    count * sizeof (hb_set_digest_t);
    external_cache_t *cache = (external_cache_t *) hb_malloc (size);
    如果 (likely (cache))
    {
      cache->coverage.clear ();
      为 (unsigned i = 0; i < count; i++)
      {
	cache->pair_sets[i].init ();
	(this+pairSet[i]).collect_second_glyphs (&cache->pair_sets[i], valueFormat);
      }
    }
    返回 cache;
  }

  bool apply (hb_ot_apply_context_t *c, void *external_cache) const
  {
    TRACE_APPLY (this);

    hb_buffer_t *buffer = c->buffer;

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    unsigned 整数 index = (this+coverage).get_coverage  (buffer->cur().codepoint, cache ? &cache->coverage : nullptr);
#否则
    unsigned 整数 index = (this+coverage).get_coverage  (buffer->cur().codepoint);
#endif
    如果 (index == NOT_COVERED) return_trace (false);

    auto &skippy_iter = c->iter_input;
    skippy_iter.reset_fast (buffer->idx);
    unsigned unsafe_to;
    如果 (unlikely (!skippy_iter.next (&unsafe_to)))
    {
      buffer->unsafe_to_concat (buffer->idx, unsafe_to);
      return_trace (false);
    }

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    如果 (cache &&
	index < pairSet.len &&
	!cache->pair_sets[index].may_have (buffer->info[skippy_iter.idx].codepoint))
    {
      buffer->unsafe_to_concat (buffer->idx, skippy_iter.idx + 1);
      return_trace (false);
    }
#endif

    return_trace ((this+pairSet[index]).apply (c, valueFormat, skippy_iter.idx));
  }

  bool subset (hb_subset_context_t *c) const
  {
    TRACE_SUBSET (this);

    const hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    const hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*this);
    如果 (unlikely (!c->serializer->extend_min (out))) return_trace (false);
    out->format = format;

    hb_pair_t<unsigned, unsigned> newFormats = hb_pair (valueFormat[0], valueFormat[1]);

    如果 (c->plan->normalized_coords)
    {
      /* all device flags will be dropped when full instancing, no need to strip
       * hints, also do not strip emtpy cause we don't compute the new default
       * value during stripping */
      newFormats = compute_effective_value_formats (glyphset, false, false, &c->plan->layout_variation_idx_delta_map);
    }
    /* do not strip hints for VF */
    否则 如果 (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
    {
      hb_blob_t* blob = hb_face_reference_table (c->plan->source, HB_TAG ('f','v','a','r'));
      bool has_fvar = (blob != hb_blob_get_empty ());
      hb_blob_destroy (blob);

      bool strip = !has_fvar;
      /* special case: strip hints when a VF has no GDEF varstore after
       * subsetting*/
      如果 (has_fvar && !c->plan->has_gdef_varstore)
        strip = true;
      newFormats = compute_effective_value_formats (glyphset, strip, true);
    }

    out->valueFormat[0] = newFormats.first;
    out->valueFormat[1] = newFormats.second;

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;

    + hb_zip (this+coverage, pairSet)
    | hb_filter (glyphset, hb_first)
    | hb_filter ([this, c, out] (const typename Types::template OffsetTo<PairSet>& _)
                 {
                   auto snap = c->serializer->snapshot ();
                   auto *o = out->pairSet.serialize_append (c->serializer);
                   如果 (unlikely (!o)) 返回 false;
                   bool ret = o->serialize_subset (c, _, this, valueFormat, out->valueFormat);
                   如果 (!ret)
                   {
                     out->pairSet.pop ();
                     c->serializer->revert (snap);
                   }
                   返回 ret;
                 },
                 hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());

    return_trace (bool (new_coverage));
  }


  hb_pair_t<unsigned, unsigned> compute_effective_value_formats (const hb_set_t& glyphset,
                                                                 bool strip_hints, bool strip_empty,
                                                                 const hb_hashmap_t<unsigned, hb_pair_t<unsigned, 整数>> *varidx_delta_map = nullptr) const
  {
    unsigned format1 = 0;
    unsigned format2 = 0;
    为 (const auto & _ :
	  + hb_zip (this+coverage, pairSet)
	  | hb_filter (glyphset, hb_first)
	  | hb_map (hb_second)
	)
    {
      const PairSet& set = (this + _);
      auto formats = set.get_values_view (valueFormat).compute_effective_value_formats (
          glyphset, valueFormat, strip_hints, strip_empty, varidx_delta_map);
      format1 |= formats.first;
      format2 |= formats.second;

      如果 (!varidx_delta_map &&
          format1 == valueFormat[0] && format2 == valueFormat[1])
        break;
    }

    返回 hb_pair (format1, format2);
  }
};


}
}
}

#endif  // OT_LAYOUT_GPOS_PAIRPOSFORMAT1_HH
