#ifndef OT_LAYOUT_GPOS_PAIRSET_HH
#define OT_LAYOUT_GPOS_PAIRSET_HH

#ضمن "PairValueRecord.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {


/* Pair records vary in glyph width; their value traversal need not. */
بنية pair_set_values_t
{
  ثابت ValueBase *base;
  glyph_data_t glyphs;
  ثابت Value *values;
  unsigned count;
  unsigned record_size;

  unsigned get_glyph (unsigned i) ثابت
  {
    glyph_data_t glyph = glyphs;
    glyph.data = (ثابت حرف *) glyph.data + i * record_size;
    ܦܢܐ glyph[0];
  }

  ثابت Value *get_values (unsigned i) ثابت
  { ܦܢܐ reinterpret_cast<ثابت Value *> ((ثابت حرف *) values + i * record_size); }

  hb_pair_t<unsigned, unsigned>
  compute_effective_value_formats (ثابت hb_set_t &glyphset,
                                   ثابت ValueFormat valueFormats[2],
                                   منطقي strip_hints, منطقي strip_empty,
                                   ثابت hb_hashmap_t<unsigned, hb_pair_t<unsigned, صحيح>> *varidx_delta_map) ثابت
  {
    unsigned len1 = valueFormats[0].get_len ();
    unsigned format1 = 0, format2 = 0;
    لكل (unsigned i = 0; i < count; i++)
    {
      ܐܢ (!glyphset.has (get_glyph (i))) استمر;
      ثابت Value *v = get_values (i);
      format1 |= valueFormats[0].get_effective_format (v, strip_hints, strip_empty, base, varidx_delta_map);
      format2 |= valueFormats[1].get_effective_format (v + len1, strip_hints, strip_empty, base, varidx_delta_map);
    }
    ܦܢܐ hb_pair (format1, format2);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ثابت ValueFormat valueFormats[2]) ثابت
  {
    unsigned len1 = valueFormats[0].get_len ();
    unsigned len2 = valueFormats[1].get_len ();
    لكل (unsigned i = 0; i < count; i++)
    {
      ܐܢ (!c->glyph_set->has (get_glyph (i))) استمر;
      ثابت hb_array_t<ثابت Value> v = hb_array (get_values (i), len1 + len2);
      ܐܢ (valueFormats[0].has_device ())
        valueFormats[0].collect_variation_indices (c, base, v.sub_array (0, len1));
      ܐܢ (valueFormats[1].has_device ())
        valueFormats[1].collect_variation_indices (c, base, v.sub_array (len1, len2));
    }
  }
};

template <typename Types>
بنية PairSet : ValueBase
{
  template <typename Types2>
  friend بنية PairPosFormat1_3;

  using PairValueRecord = GPOS_impl::PairValueRecord<Types>;

  محمي:
  typename Types::HBUINT
                        len;    /* Number of PairValueRecords */
  PairValueRecord       firstPairValueRecord;
                                /* Array of PairValueRecords--ordered
                                 * by GlyphID of the second glyph */
  عام:
  DEFINE_SIZE_MIN (Types::HBUINT::static_size);

  ساكن size_t get_size (unsigned len1, unsigned len2)
  {
    ܦܢܐ Types::HBGlyphID::static_size + Value::static_size * (len1 + len2);
  }
  ساكن size_t get_size (ثابت ValueFormat valueFormats[2])
  {
    unsigned len1 = valueFormats[0].get_len ();
    unsigned len2 = valueFormats[1].get_len ();
    ܦܢܐ get_size (len1, len2);
  }

  بنية sanitize_closure_t
  {
    ثابت ValueFormat *valueFormats;
    unsigned صحيح len1; /* valueFormats[0].get_len() */
    size_t stride; /* bytes */
  };

  منطقي sanitize (hb_sanitize_context_t *c, ثابت sanitize_closure_t *closure) ثابت
  {
    TRACE_SANITIZE (هذا);
    ܐܢ (!(c->check_struct (هذا) &&
	  hb_barrier () &&
          c->check_range (&firstPairValueRecord,
                          len,
                          closure->stride))) return_trace (ܕܓܠܐ);
    hb_barrier ();

    unsigned صحيح count = len;
    ثابت PairValueRecord *record = &firstPairValueRecord;
    return_trace (c->lazy_some_gpos ||
		  (closure->valueFormats[0].sanitize_values_stride_unsafe (c, هذا, &record->values[0], count, closure->stride) &&
                   closure->valueFormats[1].sanitize_values_stride_unsafe (c, هذا, &record->values[closure->len1], count, closure->stride)));
  }

  منطقي intersects (ثابت hb_set_t *glyphs,
                   ثابت ValueFormat *valueFormats) ثابت
  {
    unsigned record_size = get_size (valueFormats);

    ثابت PairValueRecord *record = &firstPairValueRecord;
    unsigned صحيح count = len;
    لكل (unsigned صحيح i = 0; i < count; i++)
    {
      ܐܢ (glyphs->has (record->secondGlyph))
        ܦܢܐ ܫܪܪܐ;
      record = &StructAtOffset<ثابت PairValueRecord> (record, record_size);
    }
    ܦܢܐ ܕܓܠܐ;
  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c,
                       ثابت ValueFormat *valueFormats) ثابت
  {
    unsigned record_size = get_size (valueFormats);

    ثابت PairValueRecord *record = &firstPairValueRecord;
    c->input->add_array (&record->secondGlyph, len, record_size);
  }

  template <typename set_t>
  فراغ collect_second_glyphs (set_t *glyphs,
			      ثابت ValueFormat *valueFormats) ثابت
  {
    unsigned record_size = get_size (valueFormats);
    ثابت PairValueRecord *record = &firstPairValueRecord;
    glyphs->add_array (&record->secondGlyph, len, record_size);
  }

  pair_set_values_t get_values_view (ثابت ValueFormat valueFormats[2]) ثابت
  {
    ܦܢܐ {هذا, &firstPairValueRecord.secondGlyph, &firstPairValueRecord.values[0],
            (unsigned) len, (unsigned) get_size (valueFormats)};
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ثابت ValueFormat *valueFormats) ثابت
  {
    get_values_view (valueFormats).collect_variation_indices (c, valueFormats);
  }

  منطقي apply (hb_ot_apply_context_t *c,
              ثابت ValueFormat *valueFormats,
              unsigned صحيح pos) ثابت
  {
    TRACE_APPLY (هذا);
    hb_buffer_t *buffer = c->buffer;
    unsigned صحيح len1 = valueFormats[0].get_len ();
    unsigned صحيح len2 = valueFormats[1].get_len ();
    unsigned record_size = get_size (len1, len2);

    ثابت PairValueRecord *record = hb_bsearch (buffer->info[pos].codepoint,
                                                &firstPairValueRecord,
                                                (unsigned) len,
                                                record_size);
    ܐܢ (record)
    {
      ܐܢ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "try kerning glyphs at %u,%u",
			    c->buffer->idx, pos);
      }

      منطقي applied_first = len1 && valueFormats[0].apply_value (c, هذا, &record->values[0], buffer->cur_pos());
      منطقي applied_second = len2 && valueFormats[1].apply_value (c, هذا, &record->values[len1], buffer->pos[pos]);

      ܐܢ (applied_first || applied_second)
	ܐܢ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
	{
	  c->buffer->message (c->font,
			      "kerned glyphs at %u,%u",
			      c->buffer->idx, pos);
	}

      ܐܢ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "tried kerning glyphs at %u,%u",
			    c->buffer->idx, pos);
      }

      ܐܢ (applied_first || applied_second)
        buffer->unsafe_to_break (buffer->idx, pos + 1);
      ܐܚܪܢܐ
        /* Even a zero-valued pair record is a concat hazard. */
        buffer->unsafe_to_concat (buffer->idx, pos + 1);

      ܐܢ (len2)
      {
	pos++;
      // https://github.com/harfbuzz/harfbuzz/issues/3824
      // https://github.com/harfbuzz/harfbuzz/issues/3888#issuecomment-1326781116
      buffer->unsafe_to_break (buffer->idx, pos + 1);
      }

      buffer->idx = pos;
      return_trace (ܫܪܪܐ);
    }
    buffer->unsafe_to_concat (buffer->idx, pos + 1);
    return_trace (ܕܓܠܐ);
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت ValueFormat valueFormats[2],
               ثابت ValueFormat newFormats[2]) ثابت
  {
    TRACE_SUBSET (هذا);
    auto snap = c->serializer->snapshot ();

    auto *out = c->serializer->start_embed (*هذا);
    ܐܢ (unlikely (!c->serializer->extend_min (out))) return_trace (ܕܓܠܐ);
    out->len = 0;

    ثابت hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    unsigned len1 = valueFormats[0].get_len ();
    unsigned len2 = valueFormats[1].get_len ();
    unsigned record_size = get_size (len1, len2);

    pair_value_record_context_t context =
    {
      هذا,
      valueFormats,
      newFormats,
      len1,
      &glyph_map,
      &c->plan->layout_variation_idx_delta_map
    };

    ثابت PairValueRecord *record = &firstPairValueRecord;
    unsigned count = len, num = 0;
    لكل (unsigned i = 0; i < count; i++)
    {
      ܐܢ (glyphset.has (record->secondGlyph)
         && record->subset (c, &context)) num++;
      record = &StructAtOffset<ثابت PairValueRecord> (record, record_size);
    }

    out->len = num;
    ܐܢ (!num) c->serializer->revert (snap);
    return_trace (num);
  }
};


}
}
}

#endif  // OT_LAYOUT_GPOS_PAIRSET_HH
