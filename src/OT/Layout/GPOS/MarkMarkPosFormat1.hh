#ifndef OT_LAYOUT_GPOS_MARKMARKPOSFORMAT1_HH
#define OT_LAYOUT_GPOS_MARKMARKPOSFORMAT1_HH

#include "MarkMarkPosFormat1.hh"

namespace OT {
namespace Layout {
namespace GPOS_impl {

template <typename Types>
sa_nracanaa MarkMarkPosFormat1_2
{
  rak_sita:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                mark1Coverage;          /* Offset to Combining Mark1 Coverage
                                         * table--from beginning of MarkMarkPos
                                         * subtable */
  typename Types::template LOffsetTo<Coverage>
                mark2Coverage;          /* Offset to Combining Mark2 Coverage
                                         * table--from beginning of MarkMarkPos
                                         * subtable */
  HBUINT16      classCount;             /* Number of defined mark classes */
  typename Types::template LOffsetTo<MarkArray<Types>>
                mark1Array;             /* Offset to Mark1Array table--from
                                         * beginning of MarkMarkPos subtable */
  typename Types::template LOffsetTo<AnchorMatrix<Types, HBUINT16>>
                mark2Array;             /* Offset to Mark2Array table--from
                                         * beginning of MarkMarkPos subtable */
  public:
  DEFINE_SIZE_STATIC (4 + 4 * Types::LOffset::static_size);

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this) &&
                  mark1Coverage.sanitize (c, this) &&
                  mark2Coverage.sanitize (c, this) &&
                  mark1Array.sanitize (c, this) &&
		  hb_barrier () &&
                  mark2Array.sanitize (c, this, (unsigned ចំនួនគត់) classCount));
  }

  bool intersects (const hb_set_t *glyphs) const
  {
    ត្រឡប់ (this+mark1Coverage).intersects (glyphs) &&
           (this+mark2Coverage).intersects (glyphs);
  }

  void closure_lookups (hb_closure_lookups_context_t *c) const {}

  void collect_variation_indices (hb_collect_variation_indices_context_t *c) const
  {
    (this+mark1Array).collect_variation_indices (c, this+mark1Coverage);

    hb_map_t klass_mapping;
    Markclass_closure_and_remap_indexes (this+mark1Coverage, this+mark1Array, *c->glyph_set, &klass_mapping);

    hb_sorted_vector_t<unsigned> mark2_indexes;
    collect_anchor_indexes (this+mark2Coverage, (this+mark2Array).rows,
                            classCount, *c->glyph_set, klass_mapping,
                            mark2_indexes);
    (this+mark2Array).collect_variation_indices (c, mark2_indexes.iter ());
  }

  void collect_glyphs (hb_collect_glyphs_context_t *c) const
  {
    ប្រសិនបើ (unlikely (!(this+mark1Coverage).collect_coverage (c->input))) ត្រឡប់;
    ប្រសិនបើ (unlikely (!(this+mark2Coverage).collect_coverage (c->input))) ត្រឡប់;
  }

  void collect_second_glyphs (hb_set_digest_t *digest) const
  { (this+mark2Coverage).collect_coverage (digest); }

  const Coverage &get_coverage () const { ត្រឡប់ this+mark1Coverage; }

  bool apply (hb_ot_apply_context_t *c) const
  {
    TRACE_APPLY (this);
    hb_buffer_t *buffer = c->buffer;
    unsigned ចំនួនគត់ mark1_index = (this+mark1Coverage).get_coverage  (buffer->cur().codepoint);
    ប្រសិនបើ (likely (mark1_index == NOT_COVERED)) return_trace (false);

    /* now we search backwards for a suitable mark glyph until a non-mark glyph */
    auto &skippy_iter = c->iter_input;
    skippy_iter.reset_fast (buffer->idx);
    skippy_iter.set_lookup_props (c->lookup_props & ~(uint32_t)LookupFlag::IgnoreFlags);
    unsigned unsafe_from;
    ប្រសិនបើ (unlikely (!skippy_iter.prev (&unsafe_from)))
    {
      buffer->unsafe_to_concat_from_outbuffer (unsafe_from, buffer->idx + 1);
      return_trace (false);
    }

    ប្រសិនបើ (likely (!_hb_glyph_info_is_mark (&buffer->info[skippy_iter.idx])))
    {
      buffer->unsafe_to_concat_from_outbuffer (skippy_iter.idx, buffer->idx + 1);
      return_trace (false);
    }

    unsigned ចំនួនគត់ j = skippy_iter.idx;

    unsigned ចំនួនគត់ id1 = _hb_glyph_info_get_lig_id (&buffer->cur());
    unsigned ចំនួនគត់ id2 = _hb_glyph_info_get_lig_id (&buffer->info[j]);
    unsigned ចំនួនគត់ comp1 = _hb_glyph_info_get_lig_comp (&buffer->cur());
    unsigned ចំនួនគត់ comp2 = _hb_glyph_info_get_lig_comp (&buffer->info[j]);

    ប្រសិនបើ (likely (id1 == id2))
    {
      ប្រសិនបើ (id1 == 0) /* Marks belonging to the same base. */
        goto good;
      បើមិនដូច្នេះ ប្រសិនបើ (comp1 == comp2) /* Marks belonging to the same ligature component. */
        goto good;
    }
    បើមិនដូច្នេះ
    {
      /* If ligature ids don't match, it may be the case that one of the marks
       * itself is a ligature.  In which case match. */
      ប្រសិនបើ ((id1 > 0 && !comp1) || (id2 > 0 && !comp2))
        goto good;
    }

    /* Didn't match. */
    buffer->unsafe_to_concat_from_outbuffer (skippy_iter.idx, buffer->idx + 1);
    return_trace (false);

    good:
    unsigned ចំនួនគត់ mark2_index = (this+mark2Coverage).get_coverage  (buffer->info[j].codepoint);
    ប្រសិនបើ (mark2_index == NOT_COVERED)
    {
      buffer->unsafe_to_concat_from_outbuffer (skippy_iter.idx, buffer->idx + 1);
      return_trace (false);
    }

    return_trace ((this+mark1Array).apply (c, mark1_index, mark2_index, this+mark2Array, classCount, j));
  }

  bool subset (hb_subset_context_t *c) const
  {
    TRACE_SUBSET (this);
    const hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    const hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*this);
    ប្រសិនបើ (unlikely (!c->serializer->extend_min (out))) return_trace (false);
    out->format = format;

    hb_map_t klass_mapping;
    Markclass_closure_and_remap_indexes (this+mark1Coverage, this+mark1Array, glyphset, &klass_mapping);

    ប្រសិនបើ (!klass_mapping.get_population ()) return_trace (false);
    out->classCount = klass_mapping.get_population ();

    auto mark1_iter =
    + hb_zip (this+mark1Coverage, this+mark1Array)
    | hb_filter (glyphset, hb_first)
    ;

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + mark1_iter
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    ប្រសិនបើ (!out->mark1Coverage.serialize_serialize (c->serializer, new_coverage.iter ()))
      return_trace (false);

    ប្រសិនបើ (unlikely (!out->mark1Array.serialize_subset (c, mark1Array, this,
						     (this+mark1Coverage).iter (),
						     &klass_mapping)))
      return_trace (false);

    new_coverage.reset ();
    hb_sorted_vector_t<unsigned> mark2_indexes;
    const auto &mark2_array = this+mark2Array;
    collect_subset_anchor_indexes (this+mark2Coverage, mark2_array.rows,
                                   classCount, mark2_array.matrixZ.arrayZ,
                                   glyphset, glyph_map, klass_mapping,
                                   new_coverage, mark2_indexes);

    ប្រសិនបើ (!new_coverage) return_trace (false);
    ប្រសិនបើ (!out->mark2Coverage.serialize_serialize (c->serializer, new_coverage.iter ()))
      return_trace (false);

    return_trace (out->mark2Array.serialize_subset (c, mark2Array, this,
						    new_coverage.length,
						    mark2_indexes.iter ()));

  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_MARKMARKPOSFORMAT1_HH */
