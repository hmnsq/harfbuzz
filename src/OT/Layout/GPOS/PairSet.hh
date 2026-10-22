#ifndef OT_LAYOUT_GPOS_PAIRSET_HH
#define OT_LAYOUT_GPOS_PAIRSET_HH

#ಅನ್ತರ್ಭಾವ "PairValueRecord.hh"

ನಾಮಾಕಾಶ OT {
ನಾಮಾಕಾಶ Layout {
ನಾಮಾಕಾಶ GPOS_impl {


/* Pair records vary in glyph width; their value traversal need not. */
ಸಂರಚನಾ pair_set_ಮೌಲ್ಯಗಳು_t
{
  ಧ್ರುವ ಮೌಲ್ಯ_Base *base;
  glyph_data_t glyphs;
  ಧ್ರುವ ಮೌಲ್ಯ *values;
  unsigned ಎಣಿಕೆ;
  unsigned record_size;

  unsigned get_glyph (unsigned i) ಧ್ರುವ
  {
    glyph_data_t glyph = glyphs;
    glyph.data = (ಧ್ರುವ ವರ್ಣ *) glyph.data + i * record_size;
    ಪ್ರತ್ಯಾಗಮ glyph[0];
  }

  ಧ್ರುವ ಮೌಲ್ಯ *get_ಮೌಲ್ಯಗಳು (unsigned i) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ reinterpret_cast<ಧ್ರುವ ಮೌಲ್ಯ *> ((ಧ್ರುವ ವರ್ಣ *) values + i * record_size); }

  hb_pair_t<unsigned, unsigned>
  compute_effective_ಮೌಲ್ಯ_formats (ಧ್ರುವ hb_set_t &glyphset,
                                   ಧ್ರುವ ಮೌಲ್ಯ_Format ಮೌಲ್ಯ_Formats[2],
                                   ತಾರ್ಕಿಕ strip_hints, ತಾರ್ಕಿಕ strip_empty,
                                   ಧ್ರುವ hb_hashmap_t<unsigned, hb_pair_t<unsigned, ಪೂರ್ಣಾಙ್ಕ>> *varidx_delta_map) ಧ್ರುವ
  {
    unsigned len1 = ಮೌಲ್ಯ_Formats[0].get_len ();
    unsigned format1 = 0, format2 = 0;
    ಪ್ರತಿ (unsigned i = 0; i < ಎಣಿಕೆ; i++)
    {
      ಯದಿ (!glyphset.has (get_glyph (i))) ಅನುವರ್ತ;
      ಧ್ರುವ ಮೌಲ್ಯ *v = get_ಮೌಲ್ಯಗಳು (i);
      format1 |= ಮೌಲ್ಯ_Formats[0].get_effective_format (v, strip_hints, strip_empty, base, varidx_delta_map);
      format2 |= ಮೌಲ್ಯ_Formats[1].get_effective_format (v + len1, strip_hints, strip_empty, base, varidx_delta_map);
    }
    ಪ್ರತ್ಯಾಗಮ hb_pair (format1, format2);
  }

  ಶೂನ್ಯ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ಧ್ರುವ ಮೌಲ್ಯ_Format ಮೌಲ್ಯ_Formats[2]) ಧ್ರುವ
  {
    unsigned len1 = ಮೌಲ್ಯ_Formats[0].get_len ();
    unsigned len2 = ಮೌಲ್ಯ_Formats[1].get_len ();
    ಪ್ರತಿ (unsigned i = 0; i < ಎಣಿಕೆ; i++)
    {
      ಯದಿ (!c->glyph_set->has (get_glyph (i))) ಅನುವರ್ತ;
      ಧ್ರುವ hb_array_t<ಧ್ರುವ ಮೌಲ್ಯ> v = hb_array (get_ಮೌಲ್ಯಗಳು (i), len1 + len2);
      ಯದಿ (ಮೌಲ್ಯ_Formats[0].has_device ())
        ಮೌಲ್ಯ_Formats[0].collect_variation_indices (c, base, v.sub_array (0, len1));
      ಯದಿ (ಮೌಲ್ಯ_Formats[1].has_device ())
        ಮೌಲ್ಯ_Formats[1].collect_variation_indices (c, base, v.sub_array (len1, len2));
    }
  }
};

template <typename Types>
ಸಂರಚನಾ PairSet : ಮೌಲ್ಯ_Base
{
  template <typename Types2>
  friend ಸಂರಚನಾ PairPosFormat1_3;

  using Pair_ಮೌಲ್ಯ_Record = GPOS_impl::Pair_ಮೌಲ್ಯ_Record<Types>;

  ರಕ್ಷಿತ:
  typename Types::HBUINT
                        len;    /* Number of PairValueRecords */
  Pair_ಮೌಲ್ಯ_Record       first_Pair_ಮೌಲ್ಯ_Record;
                                /* Array of PairValueRecords--ordered
                                 * by GlyphID of the second glyph */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_MIN (Types::HBUINT::static_size);

  ಸ್ಥಿರ size_t get_size (unsigned len1, unsigned len2)
  {
    ಪ್ರತ್ಯಾಗಮ Types::HBGlyphID::static_size + ಮೌಲ್ಯ::static_size * (len1 + len2);
  }
  ಸ್ಥಿರ size_t get_size (ಧ್ರುವ ಮೌಲ್ಯ_Format ಮೌಲ್ಯ_Formats[2])
  {
    unsigned len1 = ಮೌಲ್ಯ_Formats[0].get_len ();
    unsigned len2 = ಮೌಲ್ಯ_Formats[1].get_len ();
    ಪ್ರತ್ಯಾಗಮ get_size (len1, len2);
  }

  ಸಂರಚನಾ sanitize_closure_t
  {
    ಧ್ರುವ ಮೌಲ್ಯ_Format *ಮೌಲ್ಯ_Formats;
    unsigned ಪೂರ್ಣಾಙ್ಕ len1; /* valueFormats[0].get_len() */
    size_t stride; /* bytes */
  };

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c, ಧ್ರುವ sanitize_closure_t *closure) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    ಯದಿ (!(c->check_struct (ಏತತ್) &&
	  hb_barrier () &&
          c->check_range (&first_Pair_ಮೌಲ್ಯ_Record,
                          len,
                          closure->stride))) return_trace (ಅಸತ್ಯ);
    hb_barrier ();

    unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = len;
    ಧ್ರುವ Pair_ಮೌಲ್ಯ_Record *record = &first_Pair_ಮೌಲ್ಯ_Record;
    return_trace (c->lazy_some_gpos ||
		  (closure->ಮೌಲ್ಯ_Formats[0].sanitize_ಮೌಲ್ಯಗಳು_stride_unsafe (c, ಏತತ್, &record->values[0], ಎಣಿಕೆ, closure->stride) &&
                   closure->ಮೌಲ್ಯ_Formats[1].sanitize_ಮೌಲ್ಯಗಳು_stride_unsafe (c, ಏತತ್, &record->values[closure->len1], ಎಣಿಕೆ, closure->stride)));
  }

  ತಾರ್ಕಿಕ intersects (ಧ್ರುವ hb_set_t *glyphs,
                   ಧ್ರುವ ಮೌಲ್ಯ_Format *ಮೌಲ್ಯ_Formats) ಧ್ರುವ
  {
    unsigned record_size = get_size (ಮೌಲ್ಯ_Formats);

    ಧ್ರುವ Pair_ಮೌಲ್ಯ_Record *record = &first_Pair_ಮೌಲ್ಯ_Record;
    unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = len;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
    {
      ಯದಿ (glyphs->has (record->secondGlyph))
        ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
      record = &StructAtOffset<ಧ್ರುವ Pair_ಮೌಲ್ಯ_Record> (record, record_size);
    }
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }

  ಶೂನ್ಯ collect_glyphs (hb_collect_glyphs_context_t *c,
                       ಧ್ರುವ ಮೌಲ್ಯ_Format *ಮೌಲ್ಯ_Formats) ಧ್ರುವ
  {
    unsigned record_size = get_size (ಮೌಲ್ಯ_Formats);

    ಧ್ರುವ Pair_ಮೌಲ್ಯ_Record *record = &first_Pair_ಮೌಲ್ಯ_Record;
    c->ಒಳಹರಿವು->add_array (&record->secondGlyph, len, record_size);
  }

  template <typename set_t>
  ಶೂನ್ಯ collect_second_glyphs (set_t *glyphs,
			      ಧ್ರುವ ಮೌಲ್ಯ_Format *ಮೌಲ್ಯ_Formats) ಧ್ರುವ
  {
    unsigned record_size = get_size (ಮೌಲ್ಯ_Formats);
    ಧ್ರುವ Pair_ಮೌಲ್ಯ_Record *record = &first_Pair_ಮೌಲ್ಯ_Record;
    glyphs->add_array (&record->secondGlyph, len, record_size);
  }

  pair_set_ಮೌಲ್ಯಗಳು_t get_ಮೌಲ್ಯಗಳು_view (ಧ್ರುವ ಮೌಲ್ಯ_Format ಮೌಲ್ಯ_Formats[2]) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ {ಏತತ್, &first_Pair_ಮೌಲ್ಯ_Record.secondGlyph, &first_Pair_ಮೌಲ್ಯ_Record.values[0],
            (unsigned) len, (unsigned) get_size (ಮೌಲ್ಯ_Formats)};
  }

  ಶೂನ್ಯ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ಧ್ರುವ ಮೌಲ್ಯ_Format *ಮೌಲ್ಯ_Formats) ಧ್ರುವ
  {
    get_ಮೌಲ್ಯಗಳು_view (ಮೌಲ್ಯ_Formats).collect_variation_indices (c, ಮೌಲ್ಯ_Formats);
  }

  ತಾರ್ಕಿಕ apply (hb_ot_apply_context_t *c,
              ಧ್ರುವ ಮೌಲ್ಯ_Format *ಮೌಲ್ಯ_Formats,
              unsigned ಪೂರ್ಣಾಙ್ಕ pos) ಧ್ರುವ
  {
    TRACE_APPLY (ಏತತ್);
    hb_buffer_t *buffer = c->buffer;
    unsigned ಪೂರ್ಣಾಙ್ಕ len1 = ಮೌಲ್ಯ_Formats[0].get_len ();
    unsigned ಪೂರ್ಣಾಙ್ಕ len2 = ಮೌಲ್ಯ_Formats[1].get_len ();
    unsigned record_size = get_size (len1, len2);

    ಧ್ರುವ Pair_ಮೌಲ್ಯ_Record *record = hb_bsearch (buffer->info[pos].codepoint,
                                                &first_Pair_ಮೌಲ್ಯ_Record,
                                                (unsigned) len,
                                                record_size);
    ಯದಿ (record)
    {
      ಯದಿ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "try kerning glyphs at %u,%u",
			    c->buffer->idx, pos);
      }

      ತಾರ್ಕಿಕ applied_first = len1 && ಮೌಲ್ಯ_Formats[0].apply_ಮೌಲ್ಯ (c, ಏತತ್, &record->values[0], buffer->cur_pos());
      ತಾರ್ಕಿಕ applied_second = len2 && ಮೌಲ್ಯ_Formats[1].apply_ಮೌಲ್ಯ (c, ಏತತ್, &record->values[len1], buffer->pos[pos]);

      ಯದಿ (applied_first || applied_second)
	ಯದಿ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
	{
	  c->buffer->message (c->font,
			      "kerned glyphs at %u,%u",
			      c->buffer->idx, pos);
	}

      ಯದಿ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "tried kerning glyphs at %u,%u",
			    c->buffer->idx, pos);
      }

      ಯದಿ (applied_first || applied_second)
        buffer->unsafe_to_break (buffer->idx, pos + 1);
      ಅನ್ಯಥಾ
        /* Even a zero-valued pair record is a concat hazard. */
        buffer->unsafe_to_concat (buffer->idx, pos + 1);

      ಯದಿ (len2)
      {
	pos++;
      // https://github.com/harfbuzz/harfbuzz/issues/3824
      // https://github.com/harfbuzz/harfbuzz/issues/3888#issuecomment-1326781116
      buffer->unsafe_to_break (buffer->idx, pos + 1);
      }

      buffer->idx = pos;
      return_trace (ಸತ್ಯ);
    }
    buffer->unsafe_to_concat (buffer->idx, pos + 1);
    return_trace (ಅಸತ್ಯ);
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c,
               ಧ್ರುವ ಮೌಲ್ಯ_Format ಮೌಲ್ಯ_Formats[2],
               ಧ್ರುವ ಮೌಲ್ಯ_Format newFormats[2]) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    auto snap = c->serializer->snapshot ();

    auto *out = c->serializer->start_embed (*ಏತತ್);
    ಯದಿ (unlikely (!c->serializer->extend_min (out))) return_trace (ಅಸತ್ಯ);
    out->len = 0;

    ಧ್ರುವ hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ಧ್ರುವ hb_map_t &glyph_map = *c->plan->glyph_map;

    unsigned len1 = ಮೌಲ್ಯ_Formats[0].get_len ();
    unsigned len2 = ಮೌಲ್ಯ_Formats[1].get_len ();
    unsigned record_size = get_size (len1, len2);

    pair_ಮೌಲ್ಯ_record_context_t context =
    {
      ಏತತ್,
      ಮೌಲ್ಯ_Formats,
      newFormats,
      len1,
      &glyph_map,
      &c->plan->layout_variation_idx_delta_map
    };

    ಧ್ರುವ Pair_ಮೌಲ್ಯ_Record *record = &first_Pair_ಮೌಲ್ಯ_Record;
    unsigned ಎಣಿಕೆ = len, ಅಂಕೆ = 0;
    ಪ್ರತಿ (unsigned i = 0; i < ಎಣಿಕೆ; i++)
    {
      ಯದಿ (glyphset.has (record->secondGlyph)
         && record->subset (c, &context)) ಅಂಕೆ++;
      record = &StructAtOffset<ಧ್ರುವ Pair_ಮೌಲ್ಯ_Record> (record, record_size);
    }

    out->len = ಅಂಕೆ;
    ಯದಿ (!ಅಂಕೆ) c->serializer->revert (snap);
    return_trace (ಅಂಕೆ);
  }
};


}
}
}

#endif  // OT_LAYOUT_GPOS_PAIRSET_HH
