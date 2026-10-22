#ifndef OT_LAYOUT_GSUB_LIGATURESUBSTFORMAT1_HH
#define OT_LAYOUT_GSUB_LIGATURESUBSTFORMAT1_HH

#include "Common.hh"
#include "LigatureSet.hh"

namespace OT {
namespace Layout {
namespace GSUB_impl {

template <typename Types>
sa_nracanaa LigatureSubstFormat1_2
{
  rak_sita:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  typename Types::template ArrayOf<typename Types::template OffsetTo<LigatureSet<Types>>>
                ligatureSet;            /* Array LigatureSet tables
                                         * ordered by Coverage Index */
  public:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + Types::HBUINT::static_size, ligatureSet);

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (coverage.sanitize (c, this) && ligatureSet.sanitize (c, this));
  }

  bool intersects (const hb_set_t *glyphs) const
  {
    คืน
    + hb_zip (this+coverage, ligatureSet)
    | hb_filter (*glyphs, hb_first)
    | hb_map (hb_second)
    | hb_map ([this, glyphs] (const typename Types::template OffsetTo<LigatureSet<Types>> &_)
              { คืน (this+_).intersects (glyphs); })
    | hb_any
    ;
  }

  bool may_have_non_1to1 () const
  { คืน true; }

  void depend (hb_depend_context_t *c) const
  {
    + hb_zip (this+coverage, ligatureSet)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_apply ([&] (const hb_pair_t<hb_codepoint_t, const typename Types::template OffsetTo<LigatureSet<Types>>&> &_)
                {
                  const LigatureSet<Types>& ls = this+_.second;
                  ls.depend (c, _.first);
                })
    ;
  }

  void closure (hb_closure_context_t *c) const
  {
    + hb_zip (this+coverage, ligatureSet)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (this))
    | hb_apply ([c] (const LigatureSet<Types> &_) { _.closure (c); })
    ;

  }

  void closure_lookups (hb_closure_lookups_context_t *c) const {}

  void collect_glyphs (hb_collect_glyphs_context_t *c) const
  {
    ถ้า (unlikely (!(this+coverage).collect_coverage (c->input))) คืน;

    + hb_zip (this+coverage, ligatureSet)
    | hb_map (hb_second)
    | hb_map (hb_add (this))
    | hb_apply ([c] (const LigatureSet<Types> &_) { _.collect_glyphs (c); })
    ;
  }

  void collect_second_glyphs (hb_set_digest_t *digest) const
  {
    + hb_iter (ligatureSet)
    | hb_map (hb_add (this))
    | hb_apply ([digest] (const LigatureSet<Types> &_) { _.collect_seconds (*digest); })
    ;
  }

  const Coverage &get_coverage () const { คืน this+coverage; }

  bool would_apply (hb_would_apply_context_t *c) const
  {
    unsigned จำนวนเต็ม index = (this+coverage).get_coverage (c->glyphs[0]);
    ถ้า (likely (index == NOT_COVERED)) คืน false;

    const auto &lig_set = this+ligatureSet[index];
    คืน lig_set.would_apply (c);
  }

  sa_nracanaa external_cache_t
  {
    hb_ot_layout_mapping_cache_t coverage;
    hb_set_digest_t seconds;
  };
  void *external_cache_create () const
  {
    external_cache_t *cache = (external_cache_t *) hb_malloc (parimaa_na (external_cache_t));
    ถ้า (likely (cache))
    {
      cache->coverage.clear ();

      cache->seconds.init ();
      + hb_iter (ligatureSet)
      | hb_map (hb_add (this))
      | hb_apply ([cache] (const LigatureSet<Types> &_) { _.collect_seconds (cache->seconds); })
      ;
    }
    คืน cache;
  }

  bool apply (hb_ot_apply_context_t *c, void *external_cache) const
  {
    TRACE_APPLY (this);
    hb_buffer_t *buffer = c->buffer;

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    const hb_set_digest_t *seconds = cache ? &cache->seconds : nullptr;
    unsigned จำนวนเต็ม index = (this+coverage).get_coverage (buffer->cur().codepoint, cache ? &cache->coverage : nullptr);
#มิฉะนั้น
    const hb_set_digest_t *seconds = nullptr;
    unsigned จำนวนเต็ม index = (this+coverage).get_coverage (buffer->cur().codepoint);
#endif
    ถ้า (index == NOT_COVERED) return_trace (false);

    const auto &lig_set = this+ligatureSet[index];
    return_trace (lig_set.apply (c, seconds));
  }

  template <typename GlyphID>
  bool serialize (hb_serialize_context_t *c,
                  hb_sorted_array_t<GlyphID> first_glyphs,
                  hb_array_t<const unsigned จำนวนเต็ม> ligature_per_first_glyph_count_list,
                  hb_array_t<GlyphID> ligatures_list,
                  hb_array_t<const unsigned จำนวนเต็ม> component_count_list,
                  hb_array_t<GlyphID> component_list /* Starting from second for each ligature */)
  {
    TRACE_SERIALIZE (this);
    ถ้า (unlikely (!c->extend_min (this))) return_trace (false);
    ถ้า (unlikely (!ligatureSet.serialize (c, first_glyphs.length))) return_trace (false);
    สำหรับ (unsigned จำนวนเต็ม i = 0; i < first_glyphs.length; i++)
    {
      unsigned จำนวนเต็ม ligature_count = ligature_per_first_glyph_count_list[i];
      ถ้า (unlikely (!ligatureSet[i]
                        .serialize_serialize (c,
                                              ligatures_list.sub_array (0, ligature_count),
                                              component_count_list.sub_array (0, ligature_count),
                                              component_list))) return_trace (false);
      ligatures_list += ligature_count;
      component_count_list += ligature_count;
    }
    return_trace (coverage.serialize_serialize (c, first_glyphs));
  }

  bool subset (hb_subset_context_t *c) const
  {
    TRACE_SUBSET (this);
    const hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    const hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*this);
    ถ้า (unlikely (!c->serializer->extend_min (out))) return_trace (false);
    out->format = format;

    // Due to a bug in some older versions of windows 7 the Coverage table must be
    // packed after the LigatureSet and Ligature tables, so serialize Coverage first
    // which places it last in the packed order.
    hb_set_t new_coverage;
    + hb_zip (this+coverage, hb_iter (ligatureSet) | hb_map (hb_add (this)))
    | hb_filter (glyphset, hb_first)
    | hb_filter ([&] (const LigatureSet<Types>& _) {
      คืน _.intersects_lig_glyph (&glyphset);
    }, hb_second)
    | hb_map (hb_first)
    | hb_sink (new_coverage);

    ถ้า (!c->serializer->push<Coverage> ()
        ->serialize (c->serializer,
                     + new_coverage.iter () | hb_map_retains_sorting (glyph_map)))
    {
      c->serializer->pop_discard ();
      return_trace (false);
    }

    unsigned coverage_idx = c->serializer->pop_pack ();
     c->serializer->add_link (out->coverage, coverage_idx);

    + hb_zip (this+coverage, ligatureSet)
    | hb_filter (new_coverage, hb_first)
    | hb_map (hb_second)
    // to ensure that the repacker always orders the coverage table after the LigatureSet
    // and LigatureSubtable's they will be linked to the Coverage table via a virtual link
    // the coverage table object idx is passed down to facilitate this.
    | hb_apply (subset_offset_array (c, out->ligatureSet, this, coverage_idx))
    ;

    return_trace (bool (new_coverage));
  }
};

}
}
}

#endif  /* OT_LAYOUT_GSUB_LIGATURESUBSTFORMAT1_HH */
