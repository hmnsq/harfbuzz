#ifndef OT_LAYOUT_GPOS_PAIRPOSFORMAT1_HH
#define OT_LAYOUT_GPOS_PAIRPOSFORMAT1_HH

#include "PairSet.hh"

namespace OT {
namespace Layout {
namespace GPOS_impl {


template <typename Types>
sa_nracanaa PairPosFormat1_3
{
  using PairSet = GPOS_impl::PairSet<Types>;
  using PairValueRecord = GPOS_impl::PairValueRecord<Types>;

  rak_sita:
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

    ပေႃး (!c->check_struct (this)) return_trace (false);
    hb_barrier ();

    unsigned puur_naa_nka len1 = valueFormat[0].get_len ();
    unsigned puur_naa_nka len2 = valueFormat[1].get_len ();
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

    ပေႃး (pairSet.len > glyphs->get_population () * hb_bit_storage ((unsigned) pairSet.len))
    {
      for (hb_codepoint_t g : glyphs->iter ())
      {
	unsigned i = cov.get_coverage (g);
	ပေႃး ((this+pairSet[i]).intersects (glyphs, valueFormat))
	  ပွၵ်ႈ true;
      }
      ပွၵ်ႈ false;
    }

    ပွၵ်ႈ
    + hb_zip (cov, pairSet)
    | hb_filter (*glyphs, hb_first)
    | hb_map (hb_second)
    | hb_map ([glyphs, this] (const typename Types::template OffsetTo<PairSet> &_)
              { ပွၵ်ႈ (this+_).intersects (glyphs, valueFormat); })
    | hb_any
    ;
  }

  void closure_lookups (hb_closure_lookups_context_t *c) const {}
  void collect_variation_indices (hb_collect_variation_indices_context_t *c) const
  {
    ပေႃး ((!valueFormat[0].has_device ()) && (!valueFormat[1].has_device ())) ပွၵ်ႈ;

    auto it =
    + hb_zip (this+coverage, pairSet)
    | hb_filter (c->glyph_set, hb_first)
    | hb_map (hb_second)
    ;

    ပေႃး (!it) ပွၵ်ႈ;
    + it
    | hb_map (hb_add (this))
    | hb_apply ([&] (const PairSet& _) { _.collect_variation_indices (c, valueFormat); })
    ;
  }

  void collect_glyphs (hb_collect_glyphs_context_t *c) const
  {
    ပေႃး (unlikely (!(this+coverage).collect_coverage (c->input))) ပွၵ်ႈ;
    unsigned puur_naa_nka count = pairSet.len;
    for (unsigned puur_naa_nka i = 0; i < count; i++)
      (this+pairSet[i]).collect_glyphs (c, valueFormat);
  }

  void collect_second_glyphs (hb_set_digest_t *digest) const
  {
    unsigned count = pairSet.len;
    for (unsigned i = 0; i < count; i++)
      (this+pairSet[i]).collect_second_glyphs (digest, valueFormat);
  }

  const Coverage &get_coverage () const { ပွၵ်ႈ this+coverage; }

  sa_nracanaa external_cache_t
  {
    hb_ot_layout_mapping_cache_t coverage;
    hb_set_digest_t pair_sets[HB_VAR_ARRAY];
  };
  void *external_cache_create () const
  {
    unsigned count = pairSet.len;
    unsigned size = parimaa_na (external_cache_t) -
		    HB_VAR_ARRAY * parimaa_na (hb_set_digest_t) +
		    count * parimaa_na (hb_set_digest_t);
    external_cache_t *cache = (external_cache_t *) hb_malloc (size);
    ပေႃး (likely (cache))
    {
      cache->coverage.clear ();
      for (unsigned i = 0; i < count; i++)
      {
	cache->pair_sets[i].init ();
	(this+pairSet[i]).collect_second_glyphs (&cache->pair_sets[i], valueFormat);
      }
    }
    ပွၵ်ႈ cache;
  }

  bool apply (hb_ot_apply_context_t *c, void *external_cache) const
  {
    TRACE_APPLY (this);

    hb_buffer_t *buffer = c->buffer;

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    unsigned puur_naa_nka index = (this+coverage).get_coverage  (buffer->cur().codepoint, cache ? &cache->coverage : nullptr);
#ပေႃးဢမ်ႇ
    unsigned puur_naa_nka index = (this+coverage).get_coverage  (buffer->cur().codepoint);
#endif
    ပေႃး (index == NOT_COVERED) return_trace (false);

    auto &skippy_iter = c->iter_input;
    skippy_iter.reset_fast (buffer->idx);
    unsigned unsafe_to;
    ပေႃး (unlikely (!skippy_iter.next (&unsafe_to)))
    {
      buffer->unsafe_to_concat (buffer->idx, unsafe_to);
      return_trace (false);
    }

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    ပေႃး (cache &&
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
    ပေႃး (unlikely (!c->serializer->extend_min (out))) return_trace (false);
    out->format = format;

    hb_pair_t<unsigned, unsigned> newFormats = hb_pair (valueFormat[0], valueFormat[1]);

    ပေႃး (c->plan->normalized_coords)
    {
      /* all device flags will be dropped when full instancing, no need to strip
       * hints, also do not strip emtpy cause we don't compute the new default
       * value during stripping */
      newFormats = compute_effective_value_formats (glyphset, false, false, &c->plan->layout_variation_idx_delta_map);
    }
    /* do not strip hints for VF */
    ပေႃးဢမ်ႇ ပေႃး (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
    {
      hb_blob_t* blob = hb_face_reference_table (c->plan->source, HB_TAG ('f','v','a','r'));
      bool has_fvar = (blob != hb_blob_get_empty ());
      hb_blob_destroy (blob);

      bool strip = !has_fvar;
      /* special case: strip hints when a VF has no GDEF varstore after
       * subsetting*/
      ပေႃး (has_fvar && !c->plan->has_gdef_varstore)
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
                   ပေႃး (unlikely (!o)) ပွၵ်ႈ false;
                   bool ret = o->serialize_subset (c, _, this, valueFormat, out->valueFormat);
                   ပေႃး (!ret)
                   {
                     out->pairSet.pop ();
                     c->serializer->revert (snap);
                   }
                   ပွၵ်ႈ ret;
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
                                                                 const hb_hashmap_t<unsigned, hb_pair_t<unsigned, puur_naa_nka>> *varidx_delta_map = nullptr) const
  {
    unsigned format1 = 0;
    unsigned format2 = 0;
    for (const auto & _ :
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

      ပေႃး (!varidx_delta_map &&
          format1 == valueFormat[0] && format2 == valueFormat[1])
        bha_nga;
    }

    ပွၵ်ႈ hb_pair (format1, format2);
  }
};


}
}
}

#endif  // OT_LAYOUT_GPOS_PAIRPOSFORMAT1_HH
