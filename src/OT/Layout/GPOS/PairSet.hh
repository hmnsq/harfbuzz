#ifndef OT_LAYOUT_GPOS_PAIRSET_HH
#define OT_LAYOUT_GPOS_PAIRSET_HH

#include "PairValueRecord.hh"

namespace OT {
namespace Layout {
namespace GPOS_impl {


/* Pair records vary in glyph width; their value traversal need not. */
sa_nracanaa pair_set_values_t
{
  const ValueBase *base;
  glyph_data_t glyphs;
  const Value *values;
  unsigned count;
  unsigned record_size;

  unsigned get_glyph (unsigned i) const
  {
    glyph_data_t glyph = glyphs;
    glyph.data = (const var_na *) glyph.data + i * record_size;
    คืน glyph[0];
  }

  const Value *get_values (unsigned i) const
  { คืน reinterpret_cast<const Value *> ((const var_na *) values + i * record_size); }

  hb_pair_t<unsigned, unsigned>
  compute_effective_value_formats (const hb_set_t &glyphset,
                                   const ValueFormat valueFormats[2],
                                   bool strip_hints, bool strip_empty,
                                   const hb_hashmap_t<unsigned, hb_pair_t<unsigned, จำนวนเต็ม>> *varidx_delta_map) const
  {
    unsigned len1 = valueFormats[0].get_len ();
    unsigned format1 = 0, format2 = 0;
    สำหรับ (unsigned i = 0; i < count; i++)
    {
      ถ้า (!glyphset.has (get_glyph (i))) continue;
      const Value *v = get_values (i);
      format1 |= valueFormats[0].get_effective_format (v, strip_hints, strip_empty, base, varidx_delta_map);
      format2 |= valueFormats[1].get_effective_format (v + len1, strip_hints, strip_empty, base, varidx_delta_map);
    }
    คืน hb_pair (format1, format2);
  }

  void collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  const ValueFormat valueFormats[2]) const
  {
    unsigned len1 = valueFormats[0].get_len ();
    unsigned len2 = valueFormats[1].get_len ();
    สำหรับ (unsigned i = 0; i < count; i++)
    {
      ถ้า (!c->glyph_set->has (get_glyph (i))) continue;
      const hb_array_t<const Value> v = hb_array (get_values (i), len1 + len2);
      ถ้า (valueFormats[0].has_device ())
        valueFormats[0].collect_variation_indices (c, base, v.sub_array (0, len1));
      ถ้า (valueFormats[1].has_device ())
        valueFormats[1].collect_variation_indices (c, base, v.sub_array (len1, len2));
    }
  }
};

template <typename Types>
sa_nracanaa PairSet : ValueBase
{
  template <typename Types2>
  friend sa_nracanaa PairPosFormat1_3;

  using PairValueRecord = GPOS_impl::PairValueRecord<Types>;

  rak_sita:
  typename Types::HBUINT
                        len;    /* Number of PairValueRecords */
  PairValueRecord       firstPairValueRecord;
                                /* Array of PairValueRecords--ordered
                                 * by GlyphID of the second glyph */
  public:
  DEFINE_SIZE_MIN (Types::HBUINT::static_size);

  static size_t get_size (unsigned len1, unsigned len2)
  {
    คืน Types::HBGlyphID::static_size + Value::static_size * (len1 + len2);
  }
  static size_t get_size (const ValueFormat valueFormats[2])
  {
    unsigned len1 = valueFormats[0].get_len ();
    unsigned len2 = valueFormats[1].get_len ();
    คืน get_size (len1, len2);
  }

  sa_nracanaa sanitize_closure_t
  {
    const ValueFormat *valueFormats;
    unsigned จำนวนเต็ม len1; /* valueFormats[0].get_len() */
    size_t stride; /* bytes */
  };

  bool sanitize (hb_sanitize_context_t *c, const sanitize_closure_t *closure) const
  {
    TRACE_SANITIZE (this);
    ถ้า (!(c->check_struct (this) &&
	  hb_barrier () &&
          c->check_range (&firstPairValueRecord,
                          len,
                          closure->stride))) return_trace (false);
    hb_barrier ();

    unsigned จำนวนเต็ม count = len;
    const PairValueRecord *record = &firstPairValueRecord;
    return_trace (c->lazy_some_gpos ||
		  (closure->valueFormats[0].sanitize_values_stride_unsafe (c, this, &record->values[0], count, closure->stride) &&
                   closure->valueFormats[1].sanitize_values_stride_unsafe (c, this, &record->values[closure->len1], count, closure->stride)));
  }

  bool intersects (const hb_set_t *glyphs,
                   const ValueFormat *valueFormats) const
  {
    unsigned record_size = get_size (valueFormats);

    const PairValueRecord *record = &firstPairValueRecord;
    unsigned จำนวนเต็ม count = len;
    สำหรับ (unsigned จำนวนเต็ม i = 0; i < count; i++)
    {
      ถ้า (glyphs->has (record->secondGlyph))
        คืน true;
      record = &StructAtOffset<const PairValueRecord> (record, record_size);
    }
    คืน false;
  }

  void collect_glyphs (hb_collect_glyphs_context_t *c,
                       const ValueFormat *valueFormats) const
  {
    unsigned record_size = get_size (valueFormats);

    const PairValueRecord *record = &firstPairValueRecord;
    c->input->add_array (&record->secondGlyph, len, record_size);
  }

  template <typename set_t>
  void collect_second_glyphs (set_t *glyphs,
			      const ValueFormat *valueFormats) const
  {
    unsigned record_size = get_size (valueFormats);
    const PairValueRecord *record = &firstPairValueRecord;
    glyphs->add_array (&record->secondGlyph, len, record_size);
  }

  pair_set_values_t get_values_view (const ValueFormat valueFormats[2]) const
  {
    คืน {this, &firstPairValueRecord.secondGlyph, &firstPairValueRecord.values[0],
            (unsigned) len, (unsigned) get_size (valueFormats)};
  }

  void collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  const ValueFormat *valueFormats) const
  {
    get_values_view (valueFormats).collect_variation_indices (c, valueFormats);
  }

  bool apply (hb_ot_apply_context_t *c,
              const ValueFormat *valueFormats,
              unsigned จำนวนเต็ม pos) const
  {
    TRACE_APPLY (this);
    hb_buffer_t *buffer = c->buffer;
    unsigned จำนวนเต็ม len1 = valueFormats[0].get_len ();
    unsigned จำนวนเต็ม len2 = valueFormats[1].get_len ();
    unsigned record_size = get_size (len1, len2);

    const PairValueRecord *record = hb_bsearch (buffer->info[pos].codepoint,
                                                &firstPairValueRecord,
                                                (unsigned) len,
                                                record_size);
    ถ้า (record)
    {
      ถ้า (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "try kerning glyphs at %u,%u",
			    c->buffer->idx, pos);
      }

      bool applied_first = len1 && valueFormats[0].apply_value (c, this, &record->values[0], buffer->cur_pos());
      bool applied_second = len2 && valueFormats[1].apply_value (c, this, &record->values[len1], buffer->pos[pos]);

      ถ้า (applied_first || applied_second)
	ถ้า (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
	{
	  c->buffer->message (c->font,
			      "kerned glyphs at %u,%u",
			      c->buffer->idx, pos);
	}

      ถ้า (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "tried kerning glyphs at %u,%u",
			    c->buffer->idx, pos);
      }

      ถ้า (applied_first || applied_second)
        buffer->unsafe_to_break (buffer->idx, pos + 1);
      มิฉะนั้น
        /* Even a zero-valued pair record is a concat hazard. */
        buffer->unsafe_to_concat (buffer->idx, pos + 1);

      ถ้า (len2)
      {
	pos++;
      // https://github.com/harfbuzz/harfbuzz/issues/3824
      // https://github.com/harfbuzz/harfbuzz/issues/3888#issuecomment-1326781116
      buffer->unsafe_to_break (buffer->idx, pos + 1);
      }

      buffer->idx = pos;
      return_trace (true);
    }
    buffer->unsafe_to_concat (buffer->idx, pos + 1);
    return_trace (false);
  }

  bool subset (hb_subset_context_t *c,
               const ValueFormat valueFormats[2],
               const ValueFormat newFormats[2]) const
  {
    TRACE_SUBSET (this);
    auto snap = c->serializer->snapshot ();

    auto *out = c->serializer->start_embed (*this);
    ถ้า (unlikely (!c->serializer->extend_min (out))) return_trace (false);
    out->len = 0;

    const hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    const hb_map_t &glyph_map = *c->plan->glyph_map;

    unsigned len1 = valueFormats[0].get_len ();
    unsigned len2 = valueFormats[1].get_len ();
    unsigned record_size = get_size (len1, len2);

    pair_value_record_context_t context =
    {
      this,
      valueFormats,
      newFormats,
      len1,
      &glyph_map,
      &c->plan->layout_variation_idx_delta_map
    };

    const PairValueRecord *record = &firstPairValueRecord;
    unsigned count = len, num = 0;
    สำหรับ (unsigned i = 0; i < count; i++)
    {
      ถ้า (glyphset.has (record->secondGlyph)
         && record->subset (c, &context)) num++;
      record = &StructAtOffset<const PairValueRecord> (record, record_size);
    }

    out->len = num;
    ถ้า (!num) c->serializer->revert (snap);
    return_trace (num);
  }
};


}
}
}

#endif  // OT_LAYOUT_GPOS_PAIRSET_HH
