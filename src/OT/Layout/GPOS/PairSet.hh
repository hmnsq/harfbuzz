#ifndef OT_LAYOUT_GPOS_PAIRSET_HH
#define OT_LAYOUT_GPOS_PAIRSET_HH

#ضمن "PairValueRecord.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {


/* Pair records vary in glyph width; their value traversal need not. */
بنية pair_set_اقدار_t
{
  ثابت قدر_Base *base;
  glyph_data_t glyphs;
  ثابت قدر *values;
  unsigned گنتی;
  unsigned record_size;

  unsigned get_glyph (unsigned i) ثابت
  {
    glyph_data_t glyph = glyphs;
    glyph.data = (ثابت حرف *) glyph.data + i * record_size;
    أرجع glyph[0];
  }

  ثابت قدر *get_اقدار (unsigned i) ثابت
  { أرجع reinterpret_cast<ثابت قدر *> ((ثابت حرف *) values + i * record_size); }

  hb_pair_t<unsigned, unsigned>
  compute_effective_قدر_formats (ثابت hb_set_t &glyphset,
                                   ثابت قدر_Format قدر_Formats[2],
                                   منطقي strip_hints, منطقي strip_empty,
                                   ثابت hb_hashmap_t<unsigned, hb_pair_t<unsigned, صحيح>> *varidx_delta_map) ثابت
  {
    unsigned len1 = قدر_Formats[0].get_len ();
    unsigned format1 = 0, format2 = 0;
    لكل (unsigned i = 0; i < گنتی; i++)
    {
      إذا (!glyphset.has (get_glyph (i))) استمر;
      ثابت قدر *v = get_اقدار (i);
      format1 |= قدر_Formats[0].get_effective_format (v, strip_hints, strip_empty, base, varidx_delta_map);
      format2 |= قدر_Formats[1].get_effective_format (v + len1, strip_hints, strip_empty, base, varidx_delta_map);
    }
    أرجع hb_pair (format1, format2);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ثابت قدر_Format قدر_Formats[2]) ثابت
  {
    unsigned len1 = قدر_Formats[0].get_len ();
    unsigned len2 = قدر_Formats[1].get_len ();
    لكل (unsigned i = 0; i < گنتی; i++)
    {
      إذا (!c->glyph_set->has (get_glyph (i))) استمر;
      ثابت hb_array_t<ثابت قدر> v = hb_array (get_اقدار (i), len1 + len2);
      إذا (قدر_Formats[0].has_device ())
        قدر_Formats[0].collect_variation_indices (c, base, v.sub_array (0, len1));
      إذا (قدر_Formats[1].has_device ())
        قدر_Formats[1].collect_variation_indices (c, base, v.sub_array (len1, len2));
    }
  }
};

template <typename Types>
بنية PairSet : قدر_Base
{
  template <typename Types2>
  friend بنية PairPosFormat1_3;

  using Pair_قدر_Record = GPOS_impl::Pair_قدر_Record<Types>;

  محمي:
  typename Types::HBUINT
                        len;    /* Number of PairValueRecords */
  Pair_قدر_Record       first_Pair_قدر_Record;
                                /* Array of PairValueRecords--ordered
                                 * by GlyphID of the second glyph */
  عام:
  DEFINE_SIZE_MIN (Types::HBUINT::static_size);

  ساكن size_t get_size (unsigned len1, unsigned len2)
  {
    أرجع Types::HBGlyphID::static_size + قدر::static_size * (len1 + len2);
  }
  ساكن size_t get_size (ثابت قدر_Format قدر_Formats[2])
  {
    unsigned len1 = قدر_Formats[0].get_len ();
    unsigned len2 = قدر_Formats[1].get_len ();
    أرجع get_size (len1, len2);
  }

  بنية sanitize_closure_t
  {
    ثابت قدر_Format *قدر_Formats;
    unsigned صحيح len1; /* valueFormats[0].get_len() */
    size_t stride; /* bytes */
  };

  منطقي sanitize (hb_sanitize_context_t *c, ثابت sanitize_closure_t *closure) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (!(c->check_struct (هذا) &&
	  hb_barrier () &&
          c->check_range (&first_Pair_قدر_Record,
                          len,
                          closure->stride))) return_trace (خطأ);
    hb_barrier ();

    unsigned صحيح گنتی = len;
    ثابت Pair_قدر_Record *record = &first_Pair_قدر_Record;
    return_trace (c->lazy_some_gpos ||
		  (closure->قدر_Formats[0].sanitize_اقدار_stride_unsafe (c, هذا, &record->values[0], گنتی, closure->stride) &&
                   closure->قدر_Formats[1].sanitize_اقدار_stride_unsafe (c, هذا, &record->values[closure->len1], گنتی, closure->stride)));
  }

  منطقي intersects (ثابت hb_set_t *glyphs,
                   ثابت قدر_Format *قدر_Formats) ثابت
  {
    unsigned record_size = get_size (قدر_Formats);

    ثابت Pair_قدر_Record *record = &first_Pair_قدر_Record;
    unsigned صحيح گنتی = len;
    لكل (unsigned صحيح i = 0; i < گنتی; i++)
    {
      إذا (glyphs->has (record->secondGlyph))
        أرجع true;
      record = &StructAtOffset<ثابت Pair_قدر_Record> (record, record_size);
    }
    أرجع خطأ;
  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c,
                       ثابت قدر_Format *قدر_Formats) ثابت
  {
    unsigned record_size = get_size (قدر_Formats);

    ثابت Pair_قدر_Record *record = &first_Pair_قدر_Record;
    c->داخلہ->add_array (&record->secondGlyph, len, record_size);
  }

  template <typename set_t>
  فراغ collect_second_glyphs (set_t *glyphs,
			      ثابت قدر_Format *قدر_Formats) ثابت
  {
    unsigned record_size = get_size (قدر_Formats);
    ثابت Pair_قدر_Record *record = &first_Pair_قدر_Record;
    glyphs->add_array (&record->secondGlyph, len, record_size);
  }

  pair_set_اقدار_t get_اقدار_view (ثابت قدر_Format قدر_Formats[2]) ثابت
  {
    أرجع {هذا, &first_Pair_قدر_Record.secondGlyph, &first_Pair_قدر_Record.values[0],
            (unsigned) len, (unsigned) get_size (قدر_Formats)};
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ثابت قدر_Format *قدر_Formats) ثابت
  {
    get_اقدار_view (قدر_Formats).collect_variation_indices (c, قدر_Formats);
  }

  منطقي apply (hb_ot_apply_context_t *c,
              ثابت قدر_Format *قدر_Formats,
              unsigned صحيح pos) ثابت
  {
    TRACE_APPLY (هذا);
    hb_buffer_t *buffer = c->buffer;
    unsigned صحيح len1 = قدر_Formats[0].get_len ();
    unsigned صحيح len2 = قدر_Formats[1].get_len ();
    unsigned record_size = get_size (len1, len2);

    ثابت Pair_قدر_Record *record = hb_bsearch (buffer->info[pos].codepoint,
                                                &first_Pair_قدر_Record,
                                                (unsigned) len,
                                                record_size);
    إذا (record)
    {
      إذا (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "try kerning glyphs at %u,%u",
			    c->buffer->idx, pos);
      }

      منطقي applied_first = len1 && قدر_Formats[0].apply_قدر (c, هذا, &record->values[0], buffer->cur_pos());
      منطقي applied_second = len2 && قدر_Formats[1].apply_قدر (c, هذا, &record->values[len1], buffer->pos[pos]);

      إذا (applied_first || applied_second)
	إذا (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
	{
	  c->buffer->message (c->font,
			      "kerned glyphs at %u,%u",
			      c->buffer->idx, pos);
	}

      إذا (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "tried kerning glyphs at %u,%u",
			    c->buffer->idx, pos);
      }

      إذا (applied_first || applied_second)
        buffer->unsafe_to_break (buffer->idx, pos + 1);
      وإلا
        /* Even a zero-valued pair record is a concat hazard. */
        buffer->unsafe_to_concat (buffer->idx, pos + 1);

      إذا (len2)
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
    return_trace (خطأ);
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت قدر_Format قدر_Formats[2],
               ثابت قدر_Format newFormats[2]) ثابت
  {
    TRACE_SUBSET (هذا);
    auto snap = c->serializer->snapshot ();

    auto *out = c->serializer->start_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (out))) return_trace (خطأ);
    out->len = 0;

    ثابت hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    unsigned len1 = قدر_Formats[0].get_len ();
    unsigned len2 = قدر_Formats[1].get_len ();
    unsigned record_size = get_size (len1, len2);

    pair_قدر_record_context_t context =
    {
      هذا,
      قدر_Formats,
      newFormats,
      len1,
      &glyph_map,
      &c->plan->layout_variation_idx_delta_map
    };

    ثابت Pair_قدر_Record *record = &first_Pair_قدر_Record;
    unsigned گنتی = len, عدد = 0;
    لكل (unsigned i = 0; i < گنتی; i++)
    {
      إذا (glyphset.has (record->secondGlyph)
         && record->subset (c, &context)) عدد++;
      record = &StructAtOffset<ثابت Pair_قدر_Record> (record, record_size);
    }

    out->len = عدد;
    إذا (!عدد) c->serializer->revert (snap);
    return_trace (عدد);
  }
};


}
}
}

#endif  // OT_LAYOUT_GPOS_PAIRSET_HH
