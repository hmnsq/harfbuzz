#ifndef OT_LAYOUT_GPOS_PAIRSET_HH
#define OT_LAYOUT_GPOS_PAIRSET_HH

#ଅନ୍ତର୍ଭାଵ "PairValueRecord.hh"

ନାମାକାଶ OT {
ନାମାକାଶ Layout {
ନାମାକାଶ GPOS_impl {


/* Pair records vary in glyph width; their value traversal need not. */
ସଂରଚନା pair_set_ମୂଲ୍ୟଗୁଡ଼ିକ_t
{
  ଅଚଳ ମୂଲ୍ୟ_Base *base;
  glyph_data_t glyphs;
  ଅଚଳ ମୂଲ୍ୟ *values;
  unsigned ଗଣନା_2;
  unsigned record_size;

  unsigned get_glyph (unsigned i) ଅଚଳ
  {
    glyph_data_t glyph = glyphs;
    glyph.data = (ଅଚଳ ଅକ୍ଷର *) glyph.data + i * record_size;
    ଫେରନ୍ତୁ glyph[0];
  }

  ଅଚଳ ମୂଲ୍ୟ *get_ମୂଲ୍ୟଗୁଡ଼ିକ (unsigned i) ଅଚଳ
  { ଫେରନ୍ତୁ reinterpret_cast<ଅଚଳ ମୂଲ୍ୟ *> ((ଅଚଳ ଅକ୍ଷର *) values + i * record_size); }

  hb_pair_t<unsigned, unsigned>
  compute_effective_ମୂଲ୍ୟ_formats (ଅଚଳ hb_set_t &glyphset,
                                   ଅଚଳ ମୂଲ୍ୟ_Format ମୂଲ୍ୟ_Formats[2],
                                   ତାର୍କିକ strip_hints, ତାର୍କିକ strip_empty,
                                   ଅଚଳ hb_hashmap_t<unsigned, hb_pair_t<unsigned, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>> *varidx_delta_map) ଅଚଳ
  {
    unsigned len1 = ମୂଲ୍ୟ_Formats[0].get_len ();
    unsigned format1 = 0, format2 = 0;
    ପାଇଁ (unsigned i = 0; i < ଗଣନା_2; i++)
    {
      ଯଦି (!glyphset.has (get_glyph (i))) ଜାରି;
      ଅଚଳ ମୂଲ୍ୟ *v = get_ମୂଲ୍ୟଗୁଡ଼ିକ (i);
      format1 |= ମୂଲ୍ୟ_Formats[0].get_effective_format (v, strip_hints, strip_empty, base, varidx_delta_map);
      format2 |= ମୂଲ୍ୟ_Formats[1].get_effective_format (v + len1, strip_hints, strip_empty, base, varidx_delta_map);
    }
    ଫେରନ୍ତୁ hb_pair (format1, format2);
  }

  ଶୂନ୍ୟ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ଅଚଳ ମୂଲ୍ୟ_Format ମୂଲ୍ୟ_Formats[2]) ଅଚଳ
  {
    unsigned len1 = ମୂଲ୍ୟ_Formats[0].get_len ();
    unsigned len2 = ମୂଲ୍ୟ_Formats[1].get_len ();
    ପାଇଁ (unsigned i = 0; i < ଗଣନା_2; i++)
    {
      ଯଦି (!c->glyph_set->has (get_glyph (i))) ଜାରି;
      ଅଚଳ hb_array_t<ଅଚଳ ମୂଲ୍ୟ> v = hb_array (get_ମୂଲ୍ୟଗୁଡ଼ିକ (i), len1 + len2);
      ଯଦି (ମୂଲ୍ୟ_Formats[0].has_device ())
        ମୂଲ୍ୟ_Formats[0].collect_variation_indices (c, base, v.sub_array (0, len1));
      ଯଦି (ମୂଲ୍ୟ_Formats[1].has_device ())
        ମୂଲ୍ୟ_Formats[1].collect_variation_indices (c, base, v.sub_array (len1, len2));
    }
  }
};

template <typename Types>
ସଂରଚନା PairSet : ମୂଲ୍ୟ_Base
{
  template <typename Types2>
  friend ସଂରଚନା PairPosFormat1_3;

  using Pair_ମୂଲ୍ୟ_Record = GPOS_impl::Pair_ମୂଲ୍ୟ_Record<Types>;

  ସୁରକ୍ଷିତ:
  typename Types::HBUINT
                        len;    /* Number of PairValueRecords */
  Pair_ମୂଲ୍ୟ_Record       first_Pair_ମୂଲ୍ୟ_Record;
                                /* Array of PairValueRecords--ordered
                                 * by GlyphID of the second glyph */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_MIN (Types::HBUINT::static_size);

  ସ୍ଥିର size_t get_size (unsigned len1, unsigned len2)
  {
    ଫେରନ୍ତୁ Types::HBGlyphID::static_size + ମୂଲ୍ୟ::static_size * (len1 + len2);
  }
  ସ୍ଥିର size_t get_size (ଅଚଳ ମୂଲ୍ୟ_Format ମୂଲ୍ୟ_Formats[2])
  {
    unsigned len1 = ମୂଲ୍ୟ_Formats[0].get_len ();
    unsigned len2 = ମୂଲ୍ୟ_Formats[1].get_len ();
    ଫେରନ୍ତୁ get_size (len1, len2);
  }

  ସଂରଚନା sanitize_closure_t
  {
    ଅଚଳ ମୂଲ୍ୟ_Format *ମୂଲ୍ୟ_Formats;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା len1; /* valueFormats[0].get_len() */
    size_t stride; /* bytes */
  };

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c, ଅଚଳ sanitize_closure_t *closure) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    ଯଦି (!(c->check_struct (ଏହି) &&
	  hb_barrier () &&
          c->check_range (&first_Pair_ମୂଲ୍ୟ_Record,
                          len,
                          closure->stride))) return_trace (ମିଥ୍ୟା);
    hb_barrier ();

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = len;
    ଅଚଳ Pair_ମୂଲ୍ୟ_Record *record = &first_Pair_ମୂଲ୍ୟ_Record;
    return_trace (c->lazy_some_gpos ||
		  (closure->ମୂଲ୍ୟ_Formats[0].sanitize_ମୂଲ୍ୟଗୁଡ଼ିକ_stride_unsafe (c, ଏହି, &record->values[0], ଗଣନା_2, closure->stride) &&
                   closure->ମୂଲ୍ୟ_Formats[1].sanitize_ମୂଲ୍ୟଗୁଡ଼ିକ_stride_unsafe (c, ଏହି, &record->values[closure->len1], ଗଣନା_2, closure->stride)));
  }

  ତାର୍କିକ intersects (ଅଚଳ hb_set_t *glyphs,
                   ଅଚଳ ମୂଲ୍ୟ_Format *ମୂଲ୍ୟ_Formats) ଅଚଳ
  {
    unsigned record_size = get_size (ମୂଲ୍ୟ_Formats);

    ଅଚଳ Pair_ମୂଲ୍ୟ_Record *record = &first_Pair_ମୂଲ୍ୟ_Record;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = len;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
    {
      ଯଦି (glyphs->has (record->secondGlyph))
        ଫେରନ୍ତୁ ସତ୍ୟ;
      record = &StructAtOffset<ଅଚଳ Pair_ମୂଲ୍ୟ_Record> (record, record_size);
    }
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }

  ଶୂନ୍ୟ collect_glyphs (hb_collect_glyphs_context_t *c,
                       ଅଚଳ ମୂଲ୍ୟ_Format *ମୂଲ୍ୟ_Formats) ଅଚଳ
  {
    unsigned record_size = get_size (ମୂଲ୍ୟ_Formats);

    ଅଚଳ Pair_ମୂଲ୍ୟ_Record *record = &first_Pair_ମୂଲ୍ୟ_Record;
    c->ନିବେଶ->add_array (&record->secondGlyph, len, record_size);
  }

  template <typename set_t>
  ଶୂନ୍ୟ collect_second_glyphs (set_t *glyphs,
			      ଅଚଳ ମୂଲ୍ୟ_Format *ମୂଲ୍ୟ_Formats) ଅଚଳ
  {
    unsigned record_size = get_size (ମୂଲ୍ୟ_Formats);
    ଅଚଳ Pair_ମୂଲ୍ୟ_Record *record = &first_Pair_ମୂଲ୍ୟ_Record;
    glyphs->add_array (&record->secondGlyph, len, record_size);
  }

  pair_set_ମୂଲ୍ୟଗୁଡ଼ିକ_t get_ମୂଲ୍ୟଗୁଡ଼ିକ_view (ଅଚଳ ମୂଲ୍ୟ_Format ମୂଲ୍ୟ_Formats[2]) ଅଚଳ
  {
    ଫେରନ୍ତୁ {ଏହି, &first_Pair_ମୂଲ୍ୟ_Record.secondGlyph, &first_Pair_ମୂଲ୍ୟ_Record.values[0],
            (unsigned) len, (unsigned) get_size (ମୂଲ୍ୟ_Formats)};
  }

  ଶୂନ୍ୟ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ଅଚଳ ମୂଲ୍ୟ_Format *ମୂଲ୍ୟ_Formats) ଅଚଳ
  {
    get_ମୂଲ୍ୟଗୁଡ଼ିକ_view (ମୂଲ୍ୟ_Formats).collect_variation_indices (c, ମୂଲ୍ୟ_Formats);
  }

  ତାର୍କିକ apply (hb_ot_apply_context_t *c,
              ଅଚଳ ମୂଲ୍ୟ_Format *ମୂଲ୍ୟ_Formats,
              unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା pos) ଅଚଳ
  {
    TRACE_APPLY (ଏହି);
    hb_buffer_t *buffer = c->buffer;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା len1 = ମୂଲ୍ୟ_Formats[0].get_len ();
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା len2 = ମୂଲ୍ୟ_Formats[1].get_len ();
    unsigned record_size = get_size (len1, len2);

    ଅଚଳ Pair_ମୂଲ୍ୟ_Record *record = hb_bsearch (buffer->info[pos].codepoint,
                                                &first_Pair_ମୂଲ୍ୟ_Record,
                                                (unsigned) len,
                                                record_size);
    ଯଦି (record)
    {
      ଯଦି (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "try kerning glyphs at %u,%u",
			    c->buffer->idx, pos);
      }

      ତାର୍କିକ applied_first = len1 && ମୂଲ୍ୟ_Formats[0].apply_ମୂଲ୍ୟ (c, ଏହି, &record->values[0], buffer->cur_pos());
      ତାର୍କିକ applied_second = len2 && ମୂଲ୍ୟ_Formats[1].apply_ମୂଲ୍ୟ (c, ଏହି, &record->values[len1], buffer->pos[pos]);

      ଯଦି (applied_first || applied_second)
	ଯଦି (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
	{
	  c->buffer->message (c->font,
			      "kerned glyphs at %u,%u",
			      c->buffer->idx, pos);
	}

      ଯଦି (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "tried kerning glyphs at %u,%u",
			    c->buffer->idx, pos);
      }

      ଯଦି (applied_first || applied_second)
        buffer->unsafe_to_break (buffer->idx, pos + 1);
      ଅନ୍ୟଥା
        /* Even a zero-valued pair record is a concat hazard. */
        buffer->unsafe_to_concat (buffer->idx, pos + 1);

      ଯଦି (len2)
      {
	pos++;
      // https://github.com/harfbuzz/harfbuzz/issues/3824
      // https://github.com/harfbuzz/harfbuzz/issues/3888#issuecomment-1326781116
      buffer->unsafe_to_break (buffer->idx, pos + 1);
      }

      buffer->idx = pos;
      return_trace (ସତ୍ୟ);
    }
    buffer->unsafe_to_concat (buffer->idx, pos + 1);
    return_trace (ମିଥ୍ୟା);
  }

  ତାର୍କିକ subset (hb_subset_context_t *c,
               ଅଚଳ ମୂଲ୍ୟ_Format ମୂଲ୍ୟ_Formats[2],
               ଅଚଳ ମୂଲ୍ୟ_Format newFormats[2]) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    auto snap = c->serializer->snapshot ();

    auto *out = c->serializer->start_embed (*ଏହି);
    ଯଦି (unlikely (!c->serializer->extend_min (out))) return_trace (ମିଥ୍ୟା);
    out->len = 0;

    ଅଚଳ hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ଅଚଳ hb_map_t &glyph_map = *c->plan->glyph_map;

    unsigned len1 = ମୂଲ୍ୟ_Formats[0].get_len ();
    unsigned len2 = ମୂଲ୍ୟ_Formats[1].get_len ();
    unsigned record_size = get_size (len1, len2);

    pair_ମୂଲ୍ୟ_record_context_t context =
    {
      ଏହି,
      ମୂଲ୍ୟ_Formats,
      newFormats,
      len1,
      &glyph_map,
      &c->plan->layout_variation_idx_delta_map
    };

    ଅଚଳ Pair_ମୂଲ୍ୟ_Record *record = &first_Pair_ମୂଲ୍ୟ_Record;
    unsigned ଗଣନା_2 = len, ଅଙ୍କ = 0;
    ପାଇଁ (unsigned i = 0; i < ଗଣନା_2; i++)
    {
      ଯଦି (glyphset.has (record->secondGlyph)
         && record->subset (c, &context)) ଅଙ୍କ++;
      record = &StructAtOffset<ଅଚଳ Pair_ମୂଲ୍ୟ_Record> (record, record_size);
    }

    out->len = ଅଙ୍କ;
    ଯଦି (!ଅଙ୍କ) c->serializer->revert (snap);
    return_trace (ଅଙ୍କ);
  }
};


}
}
}

#endif  // OT_LAYOUT_GPOS_PAIRSET_HH
