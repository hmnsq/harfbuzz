#ifndef OT_LAYOUT_GPOS_ANCHORMATRIX_HH
#define OT_LAYOUT_GPOS_ANCHORMATRIX_HH

namespace OT {
namespace Layout {
namespace GPOS_impl {

HB_INTERNAL inline
void collect_ligature_anchor_indexes (unsigned rows,
                                     unsigned class_count,
                                     const hb_map_t &klass_mapping,
                                     hb_sorted_vector_t<unsigned> &indexes)
{
  cho (unsigned row = 0; row < rows; row++)
    cho (unsigned col = 0; col < class_count; col++)
      nếu (klass_mapping.has (col))
        indexes.push (row * class_count + col);
}

HB_INTERNAL inline
void collect_anchor_variation_indices (hb_collect_variation_indices_context_t *c,
                                      const void *base,
                                      glyph_data_t offsets,
                                      hb_array_t<const unsigned> indexes)
{
  cho (unsigned i : indexes)
  {
    unsigned offset = offsets[i];
    nếu (offset)
      StructAtOffset<const Anchor> (base, offset).collect_variation_indices (c);
  }
}

HB_INTERNAL inline
void collect_anchor_indexes (const Coverage &coverage,
                             unsigned rows,
                             unsigned class_count,
                             const hb_set_t &glyphset,
                             const hb_map_t &klass_mapping,
                             hb_sorted_vector_t<unsigned> &indexes)
{
  cho (const auto _ : hb_zip (coverage, hb_range (rows)))
    nếu (glyphset.has (_.first))
      cho (unsigned col = 0; col < class_count; col++)
        nếu (klass_mapping.has (col))
          indexes.push (_.second * class_count + col);
}

HB_INTERNAL inline
void collect_subset_anchor_indexes (const Coverage &coverage,
                                    unsigned rows,
                                    unsigned class_count,
                                    glyph_data_t offsets,
                                    const hb_set_t &glyphset,
                                    const hb_map_t &glyph_map,
                                    const hb_map_t &klass_mapping,
                                    hb_sorted_vector_t<hb_codepoint_t> &new_coverage,
                                    hb_sorted_vector_t<unsigned> &indexes)
{
  cho (const auto _ : hb_zip (coverage, hb_range (rows)))
  {
    nếu (!glyphset.has (_.first)) continue;
    unsigned row = _.second;
    bool non_empty = false;
    cho (unsigned col = 0; col < class_count; col++)
      nếu (klass_mapping.has (col) && offsets[row * class_count + col])
      {
        non_empty = true;
        break;
      }
    nếu (!non_empty) continue;

    new_coverage.push (glyph_map.get (_.first));
    cho (unsigned col = 0; col < class_count; col++)
      nếu (klass_mapping.has (col))
        indexes.push (row * class_count + col);
  }
}

template <typename Types, typename RowCount=typename Types::HBUINT>
struct AnchorMatrix
{
  RowCount      rows;                   /* Number of rows */
  UnsizedArrayOf<typename Types::template OffsetTo<Anchor, AnchorMatrix>>
                matrixZ;                /* Matrix of offsets to Anchor tables--
                                         * from beginning of AnchorMatrix table */
  public:
  DEFINE_SIZE_ARRAY (sizeof (RowCount), matrixZ);

  bool sanitize (hb_sanitize_context_t *c, unsigned số_nguyên cols) const
  {
    TRACE_SANITIZE (this);
    nếu (!c->check_struct (this)) return_trace (false);
    hb_barrier ();
    nếu (unlikely (hb_unsigned_mul_overflows (rows, cols))) return_trace (false);
    unsigned số_nguyên count = rows * cols;
    nếu (!c->check_array (matrixZ.arrayZ, count)) return_trace (false);

    nếu (c->lazy_some_gpos)
      return_trace (true);

    hb_barrier ();
    cho (unsigned số_nguyên i = 0; i < count; i++)
      nếu (!matrixZ[i].sanitize (c, this)) return_trace (false);
    return_trace (true);
  }

  const Anchor& get_anchor (hb_ot_apply_context_t *c,
			    unsigned số_nguyên row, unsigned số_nguyên col,
			    unsigned số_nguyên cols, bool *found) const
  {
    *found = false;
    nếu (unlikely (row >= rows || col >= cols)) trả_về Null (Anchor);
    auto &offset = matrixZ[row * cols + col];
    nếu (unlikely (!offset.sanitize (&c->sanitizer, this))) trả_về Null (Anchor);
    hb_barrier ();
    *found = !offset.is_null ();
    trả_về this+offset;
  }

  void collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  hb_array_t<const unsigned> indexes) const
  {
    collect_anchor_variation_indices (c, this, matrixZ.arrayZ, indexes);
  }

  template <typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  bool subset (hb_subset_context_t *c,
               unsigned             num_rows,
               Iterator             index_iter) const
  {
    TRACE_SUBSET (this);

    auto *out = c->serializer->start_embed (this);

    nếu (!index_iter) return_trace (false);
    nếu (unlikely (!c->serializer->extend_min (out)))  return_trace (false);

    out->rows = num_rows;
    cho (const unsigned i : index_iter)
    {
      auto *offset = c->serializer->embed (matrixZ[i]);
      nếu (!offset) return_trace (false);
      offset->serialize_subset (c, matrixZ[i], this);
    }

    return_trace (true);
  }

  bool offset_is_null (unsigned row, unsigned col, unsigned num_cols) const
  {
    nếu (unlikely (row >= rows || col >= num_cols)) trả_về true;
    auto &offset = matrixZ[row * num_cols + col];
    trả_về offset.is_null ();
  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_ANCHORMATRIX_HH */
