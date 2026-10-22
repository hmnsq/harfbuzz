#ifndef OT_LAYOUT_GPOS_PAIRSET_HH
#define OT_LAYOUT_GPOS_PAIRSET_HH

#அந்தர்பாவ "PairValueRecord.hh"

நாமாகாஶ OT {
நாமாகாஶ Layout {
நாமாகாஶ GPOS_impl {


/* Pair records vary in glyph width; their value traversal need not. */
கட்டமைப்பு pair_set_மதிப்புகள்_t
{
  மாறிலி மதிப்பு_Base *base;
  glyph_data_t glyphs;
  மாறிலி மதிப்பு *values;
  unsigned மொத்தம்;
  unsigned record_size;

  unsigned get_glyph (unsigned i) மாறிலி
  {
    glyph_data_t glyph = glyphs;
    glyph.data = (மாறிலி எழுத்து *) glyph.data + i * record_size;
    திரும்பு glyph[0];
  }

  மாறிலி மதிப்பு *get_மதிப்புகள் (unsigned i) மாறிலி
  { திரும்பு reinterpret_cast<மாறிலி மதிப்பு *> ((மாறிலி எழுத்து *) values + i * record_size); }

  hb_pair_t<unsigned, unsigned>
  compute_effective_மதிப்பு_formats (மாறிலி hb_set_t &glyphset,
                                   மாறிலி மதிப்பு_Format மதிப்பு_Formats[2],
                                   தருக்க strip_hints, தருக்க strip_empty,
                                   மாறிலி hb_hashmap_t<unsigned, hb_pair_t<unsigned, முழு>> *varidx_delta_map) மாறிலி
  {
    unsigned len1 = மதிப்பு_Formats[0].get_len ();
    unsigned format1 = 0, format2 = 0;
    க்கு (unsigned i = 0; i < மொத்தம்; i++)
    {
      எனில் (!glyphset.has (get_glyph (i))) தொடர்;
      மாறிலி மதிப்பு *v = get_மதிப்புகள் (i);
      format1 |= மதிப்பு_Formats[0].get_effective_format (v, strip_hints, strip_empty, base, varidx_delta_map);
      format2 |= மதிப்பு_Formats[1].get_effective_format (v + len1, strip_hints, strip_empty, base, varidx_delta_map);
    }
    திரும்பு hb_pair (format1, format2);
  }

  வெறுமை collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  மாறிலி மதிப்பு_Format மதிப்பு_Formats[2]) மாறிலி
  {
    unsigned len1 = மதிப்பு_Formats[0].get_len ();
    unsigned len2 = மதிப்பு_Formats[1].get_len ();
    க்கு (unsigned i = 0; i < மொத்தம்; i++)
    {
      எனில் (!c->glyph_set->has (get_glyph (i))) தொடர்;
      மாறிலி hb_array_t<மாறிலி மதிப்பு> v = hb_array (get_மதிப்புகள் (i), len1 + len2);
      எனில் (மதிப்பு_Formats[0].has_device ())
        மதிப்பு_Formats[0].collect_variation_indices (c, base, v.sub_array (0, len1));
      எனில் (மதிப்பு_Formats[1].has_device ())
        மதிப்பு_Formats[1].collect_variation_indices (c, base, v.sub_array (len1, len2));
    }
  }
};

template <typename Types>
கட்டமைப்பு PairSet : மதிப்பு_Base
{
  template <typename Types2>
  friend கட்டமைப்பு PairPosFormat1_3;

  using Pair_மதிப்பு_Record = GPOS_impl::Pair_மதிப்பு_Record<Types>;

  பாதுகாக்கப்பட்ட:
  typename Types::HBUINT
                        len;    /* Number of PairValueRecords */
  Pair_மதிப்பு_Record       first_Pair_மதிப்பு_Record;
                                /* Array of PairValueRecords--ordered
                                 * by GlyphID of the second glyph */
  பொது:
  DEFINE_SIZE_MIN (Types::HBUINT::static_size);

  நிலையான size_t get_size (unsigned len1, unsigned len2)
  {
    திரும்பு Types::HBGlyphID::static_size + மதிப்பு::static_size * (len1 + len2);
  }
  நிலையான size_t get_size (மாறிலி மதிப்பு_Format மதிப்பு_Formats[2])
  {
    unsigned len1 = மதிப்பு_Formats[0].get_len ();
    unsigned len2 = மதிப்பு_Formats[1].get_len ();
    திரும்பு get_size (len1, len2);
  }

  கட்டமைப்பு sanitize_closure_t
  {
    மாறிலி மதிப்பு_Format *மதிப்பு_Formats;
    unsigned முழு len1; /* valueFormats[0].get_len() */
    size_t stride; /* bytes */
  };

  தருக்க sanitize (hb_sanitize_context_t *c, மாறிலி sanitize_closure_t *closure) மாறிலி
  {
    TRACE_SANITIZE (இது);
    எனில் (!(c->check_struct (இது) &&
	  hb_barrier () &&
          c->check_range (&first_Pair_மதிப்பு_Record,
                          len,
                          closure->stride))) return_trace (பொய்);
    hb_barrier ();

    unsigned முழு மொத்தம் = len;
    மாறிலி Pair_மதிப்பு_Record *record = &first_Pair_மதிப்பு_Record;
    return_trace (c->lazy_some_gpos ||
		  (closure->மதிப்பு_Formats[0].sanitize_மதிப்புகள்_stride_unsafe (c, இது, &record->values[0], மொத்தம், closure->stride) &&
                   closure->மதிப்பு_Formats[1].sanitize_மதிப்புகள்_stride_unsafe (c, இது, &record->values[closure->len1], மொத்தம், closure->stride)));
  }

  தருக்க intersects (மாறிலி hb_set_t *glyphs,
                   மாறிலி மதிப்பு_Format *மதிப்பு_Formats) மாறிலி
  {
    unsigned record_size = get_size (மதிப்பு_Formats);

    மாறிலி Pair_மதிப்பு_Record *record = &first_Pair_மதிப்பு_Record;
    unsigned முழு மொத்தம் = len;
    க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
    {
      எனில் (glyphs->has (record->secondGlyph))
        திரும்பு உண்மை;
      record = &StructAtOffset<மாறிலி Pair_மதிப்பு_Record> (record, record_size);
    }
    திரும்பு பொய்;
  }

  வெறுமை collect_glyphs (hb_collect_glyphs_context_t *c,
                       மாறிலி மதிப்பு_Format *மதிப்பு_Formats) மாறிலி
  {
    unsigned record_size = get_size (மதிப்பு_Formats);

    மாறிலி Pair_மதிப்பு_Record *record = &first_Pair_மதிப்பு_Record;
    c->உள்ளீடு->add_array (&record->secondGlyph, len, record_size);
  }

  template <typename set_t>
  வெறுமை collect_second_glyphs (set_t *glyphs,
			      மாறிலி மதிப்பு_Format *மதிப்பு_Formats) மாறிலி
  {
    unsigned record_size = get_size (மதிப்பு_Formats);
    மாறிலி Pair_மதிப்பு_Record *record = &first_Pair_மதிப்பு_Record;
    glyphs->add_array (&record->secondGlyph, len, record_size);
  }

  pair_set_மதிப்புகள்_t get_மதிப்புகள்_view (மாறிலி மதிப்பு_Format மதிப்பு_Formats[2]) மாறிலி
  {
    திரும்பு {இது, &first_Pair_மதிப்பு_Record.secondGlyph, &first_Pair_மதிப்பு_Record.values[0],
            (unsigned) len, (unsigned) get_size (மதிப்பு_Formats)};
  }

  வெறுமை collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  மாறிலி மதிப்பு_Format *மதிப்பு_Formats) மாறிலி
  {
    get_மதிப்புகள்_view (மதிப்பு_Formats).collect_variation_indices (c, மதிப்பு_Formats);
  }

  தருக்க apply (hb_ot_apply_context_t *c,
              மாறிலி மதிப்பு_Format *மதிப்பு_Formats,
              unsigned முழு pos) மாறிலி
  {
    TRACE_APPLY (இது);
    hb_buffer_t *buffer = c->buffer;
    unsigned முழு len1 = மதிப்பு_Formats[0].get_len ();
    unsigned முழு len2 = மதிப்பு_Formats[1].get_len ();
    unsigned record_size = get_size (len1, len2);

    மாறிலி Pair_மதிப்பு_Record *record = hb_bsearch (buffer->info[pos].codepoint,
                                                &first_Pair_மதிப்பு_Record,
                                                (unsigned) len,
                                                record_size);
    எனில் (record)
    {
      எனில் (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "try kerning glyphs at %u,%u",
			    c->buffer->idx, pos);
      }

      தருக்க applied_first = len1 && மதிப்பு_Formats[0].apply_மதிப்பு (c, இது, &record->values[0], buffer->cur_pos());
      தருக்க applied_second = len2 && மதிப்பு_Formats[1].apply_மதிப்பு (c, இது, &record->values[len1], buffer->pos[pos]);

      எனில் (applied_first || applied_second)
	எனில் (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
	{
	  c->buffer->message (c->font,
			      "kerned glyphs at %u,%u",
			      c->buffer->idx, pos);
	}

      எனில் (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "tried kerning glyphs at %u,%u",
			    c->buffer->idx, pos);
      }

      எனில் (applied_first || applied_second)
        buffer->unsafe_to_break (buffer->idx, pos + 1);
      இல்லையெனில்
        /* Even a zero-valued pair record is a concat hazard. */
        buffer->unsafe_to_concat (buffer->idx, pos + 1);

      எனில் (len2)
      {
	pos++;
      // https://github.com/harfbuzz/harfbuzz/issues/3824
      // https://github.com/harfbuzz/harfbuzz/issues/3888#issuecomment-1326781116
      buffer->unsafe_to_break (buffer->idx, pos + 1);
      }

      buffer->idx = pos;
      return_trace (உண்மை);
    }
    buffer->unsafe_to_concat (buffer->idx, pos + 1);
    return_trace (பொய்);
  }

  தருக்க subset (hb_subset_context_t *c,
               மாறிலி மதிப்பு_Format மதிப்பு_Formats[2],
               மாறிலி மதிப்பு_Format newFormats[2]) மாறிலி
  {
    TRACE_SUBSET (இது);
    auto snap = c->serializer->snapshot ();

    auto *out = c->serializer->start_embed (*இது);
    எனில் (unlikely (!c->serializer->extend_min (out))) return_trace (பொய்);
    out->len = 0;

    மாறிலி hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    மாறிலி hb_map_t &glyph_map = *c->plan->glyph_map;

    unsigned len1 = மதிப்பு_Formats[0].get_len ();
    unsigned len2 = மதிப்பு_Formats[1].get_len ();
    unsigned record_size = get_size (len1, len2);

    pair_மதிப்பு_record_context_t context =
    {
      இது,
      மதிப்பு_Formats,
      newFormats,
      len1,
      &glyph_map,
      &c->plan->layout_variation_idx_delta_map
    };

    மாறிலி Pair_மதிப்பு_Record *record = &first_Pair_மதிப்பு_Record;
    unsigned மொத்தம் = len, இலக்கம் = 0;
    க்கு (unsigned i = 0; i < மொத்தம்; i++)
    {
      எனில் (glyphset.has (record->secondGlyph)
         && record->subset (c, &context)) இலக்கம்++;
      record = &StructAtOffset<மாறிலி Pair_மதிப்பு_Record> (record, record_size);
    }

    out->len = இலக்கம்;
    எனில் (!இலக்கம்) c->serializer->revert (snap);
    return_trace (இலக்கம்);
  }
};


}
}
}

#endif  // OT_LAYOUT_GPOS_PAIRSET_HH
