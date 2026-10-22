#ifndef OT_LAYOUT_GPOS_PAIRSET_HH
#define OT_LAYOUT_GPOS_PAIRSET_HH

#अन्तर्भाव "PairValueRecord.hh"

नामाकाश OT {
नामाकाश Layout {
नामाकाश GPOS_impl {


/* Pair records vary in glyph width; their value traversal need not. */
संरचना pair_set_मानानि_t
{
  ध्रुव मानम्_Base *base;
  glyph_data_t glyphs;
  ध्रुव मानम् *values;
  unsigned गणनम्;
  unsigned record_size;

  unsigned get_glyph (unsigned i) ध्रुव
  {
    glyph_data_t glyph = glyphs;
    glyph.data = (ध्रुव वर्ण *) glyph.data + i * record_size;
    पटिनिवत्त glyph[0];
  }

  ध्रुव मानम् *get_मानानि (unsigned i) ध्रुव
  { पटिनिवत्त reinterpret_cast<ध्रुव मानम् *> ((ध्रुव वर्ण *) values + i * record_size); }

  hb_pair_t<unsigned, unsigned>
  compute_effective_मानम्_formats (ध्रुव hb_set_t &glyphset,
                                   ध्रुव मानम्_Format मानम्_Formats[2],
                                   तार्किक strip_hints, तार्किक strip_empty,
                                   ध्रुव hb_hashmap_t<unsigned, hb_pair_t<unsigned, पूर्णाङ्क>> *varidx_delta_map) ध्रुव
  {
    unsigned len1 = मानम्_Formats[0].get_len ();
    unsigned format1 = 0, format2 = 0;
    अत्थाय (unsigned i = 0; i < गणनम्; i++)
    {
      सचे (!glyphset.has (get_glyph (i))) अनुवर्त;
      ध्रुव मानम् *v = get_मानानि (i);
      format1 |= मानम्_Formats[0].get_effective_format (v, strip_hints, strip_empty, base, varidx_delta_map);
      format2 |= मानम्_Formats[1].get_effective_format (v + len1, strip_hints, strip_empty, base, varidx_delta_map);
    }
    पटिनिवत्त hb_pair (format1, format2);
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ध्रुव मानम्_Format मानम्_Formats[2]) ध्रुव
  {
    unsigned len1 = मानम्_Formats[0].get_len ();
    unsigned len2 = मानम्_Formats[1].get_len ();
    अत्थाय (unsigned i = 0; i < गणनम्; i++)
    {
      सचे (!c->glyph_set->has (get_glyph (i))) अनुवर्त;
      ध्रुव hb_array_t<ध्रुव मानम्> v = hb_array (get_मानानि (i), len1 + len2);
      सचे (मानम्_Formats[0].has_device ())
        मानम्_Formats[0].collect_variation_indices (c, base, v.sub_array (0, len1));
      सचे (मानम्_Formats[1].has_device ())
        मानम्_Formats[1].collect_variation_indices (c, base, v.sub_array (len1, len2));
    }
  }
};

template <typename Types>
संरचना PairSet : मानम्_Base
{
  template <typename Types2>
  friend संरचना PairPosFormat1_3;

  using Pair_मानम्_Record = GPOS_impl::Pair_मानम्_Record<Types>;

  रक्षित:
  typename Types::HBUINT
                        len;    /* Number of PairValueRecords */
  Pair_मानम्_Record       first_Pair_मानम्_Record;
                                /* Array of PairValueRecords--ordered
                                 * by GlyphID of the second glyph */
  सार्वजनिक:
  DEFINE_SIZE_MIN (Types::HBUINT::static_size);

  स्थिर size_t get_size (unsigned len1, unsigned len2)
  {
    पटिनिवत्त Types::HBGlyphID::static_size + मानम्::static_size * (len1 + len2);
  }
  स्थिर size_t get_size (ध्रुव मानम्_Format मानम्_Formats[2])
  {
    unsigned len1 = मानम्_Formats[0].get_len ();
    unsigned len2 = मानम्_Formats[1].get_len ();
    पटिनिवत्त get_size (len1, len2);
  }

  संरचना sanitize_closure_t
  {
    ध्रुव मानम्_Format *मानम्_Formats;
    unsigned पूर्णाङ्क len1; /* valueFormats[0].get_len() */
    size_t stride; /* bytes */
  };

  तार्किक sanitize (hb_sanitize_context_t *c, ध्रुव sanitize_closure_t *closure) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    सचे (!(c->check_struct (एतत्) &&
	  hb_barrier () &&
          c->check_range (&first_Pair_मानम्_Record,
                          len,
                          closure->stride))) return_trace (मिच्छा);
    hb_barrier ();

    unsigned पूर्णाङ्क गणनम् = len;
    ध्रुव Pair_मानम्_Record *record = &first_Pair_मानम्_Record;
    return_trace (c->lazy_some_gpos ||
		  (closure->मानम्_Formats[0].sanitize_मानानि_stride_unsafe (c, एतत्, &record->values[0], गणनम्, closure->stride) &&
                   closure->मानम्_Formats[1].sanitize_मानानि_stride_unsafe (c, एतत्, &record->values[closure->len1], गणनम्, closure->stride)));
  }

  तार्किक intersects (ध्रुव hb_set_t *glyphs,
                   ध्रुव मानम्_Format *मानम्_Formats) ध्रुव
  {
    unsigned record_size = get_size (मानम्_Formats);

    ध्रुव Pair_मानम्_Record *record = &first_Pair_मानम्_Record;
    unsigned पूर्णाङ्क गणनम् = len;
    अत्थाय (unsigned पूर्णाङ्क i = 0; i < गणनम्; i++)
    {
      सचे (glyphs->has (record->secondGlyph))
        पटिनिवत्त सच्च;
      record = &StructAtOffset<ध्रुव Pair_मानम्_Record> (record, record_size);
    }
    पटिनिवत्त मिच्छा;
  }

  शून्य collect_glyphs (hb_collect_glyphs_context_t *c,
                       ध्रुव मानम्_Format *मानम्_Formats) ध्रुव
  {
    unsigned record_size = get_size (मानम्_Formats);

    ध्रुव Pair_मानम्_Record *record = &first_Pair_मानम्_Record;
    c->निवेशः->add_array (&record->secondGlyph, len, record_size);
  }

  template <typename set_t>
  शून्य collect_second_glyphs (set_t *glyphs,
			      ध्रुव मानम्_Format *मानम्_Formats) ध्रुव
  {
    unsigned record_size = get_size (मानम्_Formats);
    ध्रुव Pair_मानम्_Record *record = &first_Pair_मानम्_Record;
    glyphs->add_array (&record->secondGlyph, len, record_size);
  }

  pair_set_मानानि_t get_मानानि_view (ध्रुव मानम्_Format मानम्_Formats[2]) ध्रुव
  {
    पटिनिवत्त {एतत्, &first_Pair_मानम्_Record.secondGlyph, &first_Pair_मानम्_Record.values[0],
            (unsigned) len, (unsigned) get_size (मानम्_Formats)};
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ध्रुव मानम्_Format *मानम्_Formats) ध्रुव
  {
    get_मानानि_view (मानम्_Formats).collect_variation_indices (c, मानम्_Formats);
  }

  तार्किक apply (hb_ot_apply_context_t *c,
              ध्रुव मानम्_Format *मानम्_Formats,
              unsigned पूर्णाङ्क pos) ध्रुव
  {
    TRACE_APPLY (एतत्);
    hb_buffer_t *buffer = c->buffer;
    unsigned पूर्णाङ्क len1 = मानम्_Formats[0].get_len ();
    unsigned पूर्णाङ्क len2 = मानम्_Formats[1].get_len ();
    unsigned record_size = get_size (len1, len2);

    ध्रुव Pair_मानम्_Record *record = hb_bsearch (buffer->info[pos].codepoint,
                                                &first_Pair_मानम्_Record,
                                                (unsigned) len,
                                                record_size);
    सचे (record)
    {
      सचे (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "try kerning glyphs at %u,%u",
			    c->buffer->idx, pos);
      }

      तार्किक applied_first = len1 && मानम्_Formats[0].apply_मानम् (c, एतत्, &record->values[0], buffer->cur_pos());
      तार्किक applied_second = len2 && मानम्_Formats[1].apply_मानम् (c, एतत्, &record->values[len1], buffer->pos[pos]);

      सचे (applied_first || applied_second)
	सचे (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
	{
	  c->buffer->message (c->font,
			      "kerned glyphs at %u,%u",
			      c->buffer->idx, pos);
	}

      सचे (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "tried kerning glyphs at %u,%u",
			    c->buffer->idx, pos);
      }

      सचे (applied_first || applied_second)
        buffer->unsafe_to_break (buffer->idx, pos + 1);
      अञ्ञथा
        /* Even a zero-valued pair record is a concat hazard. */
        buffer->unsafe_to_concat (buffer->idx, pos + 1);

      सचे (len2)
      {
	pos++;
      // https://github.com/harfbuzz/harfbuzz/issues/3824
      // https://github.com/harfbuzz/harfbuzz/issues/3888#issuecomment-1326781116
      buffer->unsafe_to_break (buffer->idx, pos + 1);
      }

      buffer->idx = pos;
      return_trace (सच्च);
    }
    buffer->unsafe_to_concat (buffer->idx, pos + 1);
    return_trace (मिच्छा);
  }

  तार्किक subset (hb_subset_context_t *c,
               ध्रुव मानम्_Format मानम्_Formats[2],
               ध्रुव मानम्_Format newFormats[2]) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    auto snap = c->serializer->snapshot ();

    auto *out = c->serializer->start_embed (*एतत्);
    सचे (unlikely (!c->serializer->extend_min (out))) return_trace (मिच्छा);
    out->len = 0;

    ध्रुव hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ध्रुव hb_map_t &glyph_map = *c->plan->glyph_map;

    unsigned len1 = मानम्_Formats[0].get_len ();
    unsigned len2 = मानम्_Formats[1].get_len ();
    unsigned record_size = get_size (len1, len2);

    pair_मानम्_record_context_t context =
    {
      एतत्,
      मानम्_Formats,
      newFormats,
      len1,
      &glyph_map,
      &c->plan->layout_variation_idx_delta_map
    };

    ध्रुव Pair_मानम्_Record *record = &first_Pair_मानम्_Record;
    unsigned गणनम् = len, अङ्कः = 0;
    अत्थाय (unsigned i = 0; i < गणनम्; i++)
    {
      सचे (glyphset.has (record->secondGlyph)
         && record->subset (c, &context)) अङ्कः++;
      record = &StructAtOffset<ध्रुव Pair_मानम्_Record> (record, record_size);
    }

    out->len = अङ्कः;
    सचे (!अङ्कः) c->serializer->revert (snap);
    return_trace (अङ्कः);
  }
};


}
}
}

#endif  // OT_LAYOUT_GPOS_PAIRSET_HH
