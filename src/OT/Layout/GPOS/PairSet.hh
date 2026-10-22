#ifndef OT_LAYOUT_GPOS_PAIRSET_HH
#define OT_LAYOUT_GPOS_PAIRSET_HH

#include "PairValueRecord.hh"

namespace OT {
namespace Layout {
namespace GPOS_impl {


/* Pair records vary in glyph width; their value traversal need not. */
struct pair_set_values_t
{
  const ValueBase *base;
  glyph_data_t glyphs;
  const Value *values;
  unsigned count;
  unsigned record_size;

  unsigned get_glyph (unsigned i) const
  {
    glyph_data_t glyph = glyphs;
    glyph.data = (const char *) glyph.data + i * record_size;
    вернуть glyph[0];
  }

  const Value *get_values (unsigned i) const
  { вернуть reinterpret_cast<const Value *> ((const char *) values + i * record_size); }

  hb_pair_t<unsigned, unsigned>
  compute_effective_value_formats (const hb_set_t &glyphset,
                                   const ValueFormat valueFormats[2],
                                   bool strip_hints, bool strip_empty,
                                   const hb_hashmap_t<unsigned, hb_pair_t<unsigned, целое>> *varidx_delta_map) const
  {
    unsigned len1 = valueFormats[0].get_len ();
    unsigned format1 = 0, format2 = 0;
    для (unsigned i = 0; i < count; i++)
    {
      если (!glyphset.has (get_glyph (i))) continue;
      const Value *v = get_values (i);
      format1 |= valueFormats[0].get_effective_format (v, strip_hints, strip_empty, base, varidx_delta_map);
      format2 |= valueFormats[1].get_effective_format (v + len1, strip_hints, strip_empty, base, varidx_delta_map);
    }
    вернуть hb_pair (format1, format2);
  }

  void collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  const ValueFormat valueFormats[2]) const
  {
    unsigned len1 = valueFormats[0].get_len ();
    unsigned len2 = valueFormats[1].get_len ();
    для (unsigned i = 0; i < count; i++)
    {
      если (!c->glyph_set->has (get_glyph (i))) continue;
      const hb_array_t<const Value> v = hb_array (get_values (i), len1 + len2);
      если (valueFormats[0].has_device ())
        valueFormats[0].collect_variation_indices (c, base, v.sub_array (0, len1));
      если (valueFormats[1].has_device ())
        valueFormats[1].collect_variation_indices (c, base, v.sub_array (len1, len2));
    }
  }
};

template <typename Types>
struct PairSet : ValueBase
{
  template <typename Types2>
  friend struct PairPosFormat1_3;

  using PairValueRecord = GPOS_impl::PairValueRecord<Types>;

  protected:
  typename Types::HBUINT
                        len;    /* Number of PairValueRecords */
  PairValueRecord       firstPairValueRecord;
                                /* Array of PairValueRecords--ordered
                                 * by GlyphID of the second glyph */
  public:
  DEFINE_SIZE_MIN (Types::HBUINT::static_size);

  static size_t get_size (unsigned len1, unsigned len2)
  {
    вернуть Types::HBGlyphID::static_size + Value::static_size * (len1 + len2);
  }
  static size_t get_size (const ValueFormat valueFormats[2])
  {
    unsigned len1 = valueFormats[0].get_len ();
    unsigned len2 = valueFormats[1].get_len ();
    вернуть get_size (len1, len2);
  }

  struct sanitize_closure_t
  {
    const ValueFormat *valueFormats;
    unsigned целое len1; /* valueFormats[0].get_len() */
    size_t stride; /* bytes */
  };

  bool sanitize (hb_sanitize_context_t *c, const sanitize_closure_t *closure) const
  {
    TRACE_SANITIZE (this);
    если (!(c->check_struct (this) &&
	  hb_barrier () &&
          c->check_range (&firstPairValueRecord,
                          len,
                          closure->stride))) return_trace (false);
    hb_barrier ();

    unsigned целое count = len;
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
    unsigned целое count = len;
    для (unsigned целое i = 0; i < count; i++)
    {
      если (glyphs->has (record->secondGlyph))
        вернуть true;
      record = &StructAtOffset<const PairValueRecord> (record, record_size);
    }
    вернуть false;
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
    вернуть {this, &firstPairValueRecord.secondGlyph, &firstPairValueRecord.values[0],
            (unsigned) len, (unsigned) get_size (valueFormats)};
  }

  void collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  const ValueFormat *valueFormats) const
  {
    get_values_view (valueFormats).collect_variation_indices (c, valueFormats);
  }

  bool apply (hb_ot_apply_context_t *c,
              const ValueFormat *valueFormats,
              unsigned целое pos) const
  {
    TRACE_APPLY (this);
    hb_buffer_t *buffer = c->buffer;
    unsigned целое len1 = valueFormats[0].get_len ();
    unsigned целое len2 = valueFormats[1].get_len ();
    unsigned record_size = get_size (len1, len2);

    const PairValueRecord *record = hb_bsearch (buffer->info[pos].codepoint,
                                                &firstPairValueRecord,
                                                (unsigned) len,
                                                record_size);
    если (record)
    {
      если (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "try kerning glyphs at %u,%u",
			    c->buffer->idx, pos);
      }

      bool applied_first = len1 && valueFormats[0].apply_value (c, this, &record->values[0], buffer->cur_pos());
      bool applied_second = len2 && valueFormats[1].apply_value (c, this, &record->values[len1], buffer->pos[pos]);

      если (applied_first || applied_second)
	если (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
	{
	  c->buffer->message (c->font,
			      "kerned glyphs at %u,%u",
			      c->buffer->idx, pos);
	}

      если (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "tried kerning glyphs at %u,%u",
			    c->buffer->idx, pos);
      }

      если (applied_first || applied_second)
        buffer->unsafe_to_break (buffer->idx, pos + 1);
      иначе
        /* Even a zero-valued pair record is a concat hazard. */
        buffer->unsafe_to_concat (buffer->idx, pos + 1);

      если (len2)
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
    если (unlikely (!c->serializer->extend_min (out))) return_trace (false);
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
    для (unsigned i = 0; i < count; i++)
    {
      если (glyphset.has (record->secondGlyph)
         && record->subset (c, &context)) num++;
      record = &StructAtOffset<const PairValueRecord> (record, record_size);
    }

    out->len = num;
    если (!num) c->serializer->revert (snap);
    return_trace (num);
  }
};


}
}
}

#endif  // OT_LAYOUT_GPOS_PAIRSET_HH
