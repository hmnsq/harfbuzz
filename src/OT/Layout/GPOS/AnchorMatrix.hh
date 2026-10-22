#ifndef OT_LAYOUT_GPOS_ANCHORMATRIX_HH
#define OT_LAYOUT_GPOS_ANCHORMATRIX_HH

namespace OT {
namespace Layout {
namespace GPOS_impl {

HB_INTERNAL anta_hstha
void collect_ligature_anchor_indexes (unsigned rows,
                                     unsigned class_count,
                                     const hb_map_t &klass_mapping,
                                     hb_sorted_vector_t<unsigned> &indexes)
{
  အတွက် (unsigned row = 0; row < rows; row++)
    အတွက် (unsigned col = 0; col < class_count; col++)
      အကယ်၍ (klass_mapping.has (col))
        indexes.push (row * class_count + col);
}

HB_INTERNAL anta_hstha
void collect_anchor_variation_indices (hb_collect_variation_indices_context_t *c,
                                      const void *base,
                                      glyph_data_t offsets,
                                      hb_array_t<const unsigned> indexes)
{
  အတွက် (unsigned i : indexes)
  {
    unsigned offset = offsets[i];
    အကယ်၍ (offset)
      StructAtOffset<const Anchor> (base, offset).collect_variation_indices (c);
  }
}

HB_INTERNAL anta_hstha
void collect_anchor_indexes (const Coverage &coverage,
                             unsigned rows,
                             unsigned class_count,
                             const hb_set_t &glyphset,
                             const hb_map_t &klass_mapping,
                             hb_sorted_vector_t<unsigned> &indexes)
{
  အတွက် (const auto _ : hb_zip (coverage, hb_range (rows)))
    အကယ်၍ (glyphset.has (_.first))
      အတွက် (unsigned col = 0; col < class_count; col++)
        အကယ်၍ (klass_mapping.has (col))
          indexes.push (_.second * class_count + col);
}

HB_INTERNAL anta_hstha
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
  အတွက် (const auto _ : hb_zip (coverage, hb_range (rows)))
  {
    အကယ်၍ (!glyphset.has (_.first)) continue;
    unsigned row = _.second;
    bool non_empty = false;
    အတွက် (unsigned col = 0; col < class_count; col++)
      အကယ်၍ (klass_mapping.has (col) && offsets[row * class_count + col])
      {
        non_empty = true;
        bha_nga;
      }
    အကယ်၍ (!non_empty) continue;

    new_coverage.push (glyph_map.get (_.first));
    အတွက် (unsigned col = 0; col < class_count; col++)
      အကယ်၍ (klass_mapping.has (col))
        indexes.push (row * class_count + col);
  }
}

template <typename Types, typename RowCount=typename Types::HBUINT>
sa_nracanaa AnchorMatrix
{
  RowCount      rows;                   /* Number of rows */
  UnsizedArrayOf<typename Types::template OffsetTo<Anchor, AnchorMatrix>>
                matrixZ;                /* Matrix of offsets to Anchor tables--
                                         * from beginning of AnchorMatrix table */
  public:
  DEFINE_SIZE_ARRAY (parimaa_na (RowCount), matrixZ);

  bool sanitize (hb_sanitize_context_t *c, unsigned ကိန်းပြည့် cols) const
  {
    TRACE_SANITIZE (this);
    အကယ်၍ (!c->check_struct (this)) return_trace (false);
    hb_barrier ();
    အကယ်၍ (unlikely (hb_unsigned_mul_overflows (rows, cols))) return_trace (false);
    unsigned ကိန်းပြည့် count = rows * cols;
    အကယ်၍ (!c->check_array (matrixZ.arrayZ, count)) return_trace (false);

    အကယ်၍ (c->lazy_some_gpos)
      return_trace (true);

    hb_barrier ();
    အတွက် (unsigned ကိန်းပြည့် i = 0; i < count; i++)
      အကယ်၍ (!matrixZ[i].sanitize (c, this)) return_trace (false);
    return_trace (true);
  }

  const Anchor& get_anchor (hb_ot_apply_context_t *c,
			    unsigned ကိန်းပြည့် row, unsigned ကိန်းပြည့် col,
			    unsigned ကိန်းပြည့် cols, bool *found) const
  {
    *found = false;
    အကယ်၍ (unlikely (row >= rows || col >= cols)) ပြန်ပေး Null (Anchor);
    auto &offset = matrixZ[row * cols + col];
    အကယ်၍ (unlikely (!offset.sanitize (&c->sanitizer, this))) ပြန်ပေး Null (Anchor);
    hb_barrier ();
    *found = !offset.is_null ();
    ပြန်ပေး this+offset;
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

    အကယ်၍ (!index_iter) return_trace (false);
    အကယ်၍ (unlikely (!c->serializer->extend_min (out)))  return_trace (false);

    out->rows = num_rows;
    အတွက် (const unsigned i : index_iter)
    {
      auto *offset = c->serializer->embed (matrixZ[i]);
      အကယ်၍ (!offset) return_trace (false);
      offset->serialize_subset (c, matrixZ[i], this);
    }

    return_trace (true);
  }

  bool offset_is_null (unsigned row, unsigned col, unsigned num_cols) const
  {
    အကယ်၍ (unlikely (row >= rows || col >= num_cols)) ပြန်ပေး true;
    auto &offset = matrixZ[row * num_cols + col];
    ပြန်ပေး offset.is_null ();
  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_ANCHORMATRIX_HH */
