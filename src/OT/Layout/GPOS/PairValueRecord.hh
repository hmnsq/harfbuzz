#ifndef OT_LAYOUT_GPOS_PAIRVALUERECORD_HH
#define OT_LAYOUT_GPOS_PAIRVALUERECORD_HH

#include "ValueFormat.hh"

namespace OT {
namespace Layout {
namespace GPOS_impl {


sa_nracanaa pair_value_record_context_t
{
  const ValueBase     *base;
  const ValueFormat   *valueFormats;
  const ValueFormat   *newFormats;
  unsigned            len1; /* valueFormats[0].get_len() */
  const hb_map_t      *glyph_map;
  const hb_hashmap_t<unsigned, hb_pair_t<unsigned, ကိန်းပြည့်>> *layout_variation_idx_delta_map;

  void copy_values (hb_serialize_context_t *s, const Value *values) const
  {
    valueFormats[0].copy_values (s, newFormats[0], base, values,
                                layout_variation_idx_delta_map);
    valueFormats[1].copy_values (s, newFormats[1], base, values + len1,
                                layout_variation_idx_delta_map);
  }
};

template <typename Types>
sa_nracanaa PairValueRecord
{
  template <typename Types2>
  friend sa_nracanaa PairSet;

  rak_sita:
  typename Types::HBGlyphID
	        secondGlyph;            /* GlyphID of second glyph in the
                                         * pair--first glyph is listed in the
                                         * Coverage table */
  ValueRecord   values;                 /* Positioning data for the first glyph
                                         * followed by for second glyph */
  public:
  DEFINE_SIZE_ARRAY (Types::HBGlyphID::static_size, values);

  ကိန်းပြည့် cmp (hb_codepoint_t k) const
  { ပြန်ပေး secondGlyph.cmp (k); }

  bool subset (hb_subset_context_t *c,
               pair_value_record_context_t *closure) const
  {
    TRACE_SERIALIZE (this);
    auto *s = c->serializer;
    auto *out = s->start_embed (*this);
    အကယ်၍ (unlikely (!s->extend_min (out))) return_trace (false);

    out->secondGlyph = (*closure->glyph_map)[secondGlyph];

    closure->copy_values (s, values);

    return_trace (true);
  }

  void collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  const ValueFormat *valueFormats,
                                  const ValueBase *base) const
  {
    unsigned record1_len = valueFormats[0].get_len ();
    unsigned record2_len = valueFormats[1].get_len ();
    const hb_array_t<const Value> values_array = values.as_array (record1_len + record2_len);

    အကယ်၍ (valueFormats[0].has_device ())
      valueFormats[0].collect_variation_indices (c, base, values_array.sub_array (0, record1_len));

    အကယ်၍ (valueFormats[1].has_device ())
      valueFormats[1].collect_variation_indices (c, base, values_array.sub_array (record1_len, record2_len));
  }

  bool intersects (const hb_set_t& glyphset) const
  {
    ပြန်ပေး glyphset.has(secondGlyph);
  }

  const Value* get_values_1 () const
  {
    ပြန်ပေး &values[0];
  }

  const Value* get_values_2 (ValueFormat format1) const
  {
    ပြန်ပေး &values[format1.get_len ()];
  }
};


}
}
}

#endif  // OT_LAYOUT_GPOS_PAIRVALUERECORD_HH
