#ifndef OT_LAYOUT_GPOS_PAIRSET_HH
#define OT_LAYOUT_GPOS_PAIRSET_HH

#ضمن "PairValueRecord.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {


/* Pair records vary in glyph width; their value traversal need not. */
بنية pair_set_قيم_t
{
  ثابت قيمة_Base *base;
  glyph_data_t glyphs;
  ثابت قيمة *values;
  unsigned تعداد_2;
  unsigned record_size;

  unsigned get_glyph (unsigned i) ثابت
  {
    glyph_data_t glyph = glyphs;
    glyph.data = (ثابت حرف *) glyph.data + i * record_size;
    إرجاع glyph[0];
  }

  ثابت قيمة *get_قيم (unsigned i) ثابت
  { إرجاع reinterpret_cast<ثابت قيمة *> ((ثابت حرف *) values + i * record_size); }

  hb_pair_t<unsigned, unsigned>
  compute_effective_قيمة_formats (ثابت hb_set_t &glyphset,
                                   ثابت قيمة_Format قيمة_Formats[2],
                                   منطقي strip_hints, منطقي strip_empty,
                                   ثابت hb_hashmap_t<unsigned, hb_pair_t<unsigned, صحيح>> *varidx_delta_map) ثابت
  {
    unsigned len1 = قيمة_Formats[0].get_len ();
    unsigned format1 = 0, format2 = 0;
    لكل (unsigned i = 0; i < تعداد_2; i++)
    {
      إذا (!glyphset.has (get_glyph (i))) استمر;
      ثابت قيمة *v = get_قيم (i);
      format1 |= قيمة_Formats[0].get_effective_format (v, strip_hints, strip_empty, base, varidx_delta_map);
      format2 |= قيمة_Formats[1].get_effective_format (v + len1, strip_hints, strip_empty, base, varidx_delta_map);
    }
    إرجاع hb_pair (format1, format2);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ثابت قيمة_Format قيمة_Formats[2]) ثابت
  {
    unsigned len1 = قيمة_Formats[0].get_len ();
    unsigned len2 = قيمة_Formats[1].get_len ();
    لكل (unsigned i = 0; i < تعداد_2; i++)
    {
      إذا (!c->glyph_set->has (get_glyph (i))) استمر;
      ثابت hb_array_t<ثابت قيمة> v = hb_array (get_قيم (i), len1 + len2);
      إذا (قيمة_Formats[0].has_device ())
        قيمة_Formats[0].collect_variation_indices (c, base, v.sub_array (0, len1));
      إذا (قيمة_Formats[1].has_device ())
        قيمة_Formats[1].collect_variation_indices (c, base, v.sub_array (len1, len2));
    }
  }
};

template <typename Types>
بنية PairSet : قيمة_Base
{
  template <typename Types2>
  friend بنية PairPosFormat1_3;

  using Pair_قيمة_Record = GPOS_impl::Pair_قيمة_Record<Types>;

  محمي:
  typename Types::HBUINT
                        len;    /* Number of PairValueRecords */
  Pair_قيمة_Record       first_Pair_قيمة_Record;
                                /* Array of PairValueRecords--ordered
                                 * by GlyphID of the second glyph */
  عام:
  DEFINE_SIZE_MIN (Types::HBUINT::static_size);

  ساكن size_t get_size (unsigned len1, unsigned len2)
  {
    إرجاع Types::HBGlyphID::static_size + قيمة::static_size * (len1 + len2);
  }
  ساكن size_t get_size (ثابت قيمة_Format قيمة_Formats[2])
  {
    unsigned len1 = قيمة_Formats[0].get_len ();
    unsigned len2 = قيمة_Formats[1].get_len ();
    إرجاع get_size (len1, len2);
  }

  بنية sanitize_closure_t
  {
    ثابت قيمة_Format *قيمة_Formats;
    unsigned صحيح len1; /* valueFormats[0].get_len() */
    size_t stride; /* bytes */
  };

  منطقي sanitize (hb_sanitize_context_t *c, ثابت sanitize_closure_t *closure) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (!(c->check_struct (هذا) &&
	  hb_barrier () &&
          c->check_range (&first_Pair_قيمة_Record,
                          len,
                          closure->stride))) return_trace (خطأ);
    hb_barrier ();

    unsigned صحيح تعداد_2 = len;
    ثابت Pair_قيمة_Record *record = &first_Pair_قيمة_Record;
    return_trace (c->lazy_some_gpos ||
		  (closure->قيمة_Formats[0].sanitize_قيم_stride_unsafe (c, هذا, &record->values[0], تعداد_2, closure->stride) &&
                   closure->قيمة_Formats[1].sanitize_قيم_stride_unsafe (c, هذا, &record->values[closure->len1], تعداد_2, closure->stride)));
  }

  منطقي intersects (ثابت hb_set_t *glyphs,
                   ثابت قيمة_Format *قيمة_Formats) ثابت
  {
    unsigned record_size = get_size (قيمة_Formats);

    ثابت Pair_قيمة_Record *record = &first_Pair_قيمة_Record;
    unsigned صحيح تعداد_2 = len;
    لكل (unsigned صحيح i = 0; i < تعداد_2; i++)
    {
      إذا (glyphs->has (record->secondGlyph))
        إرجاع true;
      record = &StructAtOffset<ثابت Pair_قيمة_Record> (record, record_size);
    }
    إرجاع خطأ;
  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c,
                       ثابت قيمة_Format *قيمة_Formats) ثابت
  {
    unsigned record_size = get_size (قيمة_Formats);

    ثابت Pair_قيمة_Record *record = &first_Pair_قيمة_Record;
    c->إدخال->add_array (&record->secondGlyph, len, record_size);
  }

  template <typename set_t>
  فراغ collect_second_glyphs (set_t *glyphs,
			      ثابت قيمة_Format *قيمة_Formats) ثابت
  {
    unsigned record_size = get_size (قيمة_Formats);
    ثابت Pair_قيمة_Record *record = &first_Pair_قيمة_Record;
    glyphs->add_array (&record->secondGlyph, len, record_size);
  }

  pair_set_قيم_t get_قيم_view (ثابت قيمة_Format قيمة_Formats[2]) ثابت
  {
    إرجاع {هذا, &first_Pair_قيمة_Record.secondGlyph, &first_Pair_قيمة_Record.values[0],
            (unsigned) len, (unsigned) get_size (قيمة_Formats)};
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ثابت قيمة_Format *قيمة_Formats) ثابت
  {
    get_قيم_view (قيمة_Formats).collect_variation_indices (c, قيمة_Formats);
  }

  منطقي apply (hb_ot_apply_context_t *c,
              ثابت قيمة_Format *قيمة_Formats,
              unsigned صحيح pos) ثابت
  {
    TRACE_APPLY (هذا);
    hb_buffer_t *buffer = c->buffer;
    unsigned صحيح len1 = قيمة_Formats[0].get_len ();
    unsigned صحيح len2 = قيمة_Formats[1].get_len ();
    unsigned record_size = get_size (len1, len2);

    ثابت Pair_قيمة_Record *record = hb_bsearch (buffer->info[pos].codepoint,
                                                &first_Pair_قيمة_Record,
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

      منطقي applied_first = len1 && قيمة_Formats[0].apply_قيمة (c, هذا, &record->values[0], buffer->cur_pos());
      منطقي applied_second = len2 && قيمة_Formats[1].apply_قيمة (c, هذا, &record->values[len1], buffer->pos[pos]);

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
               ثابت قيمة_Format قيمة_Formats[2],
               ثابت قيمة_Format newFormats[2]) ثابت
  {
    TRACE_SUBSET (هذا);
    auto snap = c->serializer->snapshot ();

    auto *out = c->serializer->start_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (out))) return_trace (خطأ);
    out->len = 0;

    ثابت hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    unsigned len1 = قيمة_Formats[0].get_len ();
    unsigned len2 = قيمة_Formats[1].get_len ();
    unsigned record_size = get_size (len1, len2);

    pair_قيمة_record_context_t context =
    {
      هذا,
      قيمة_Formats,
      newFormats,
      len1,
      &glyph_map,
      &c->plan->layout_variation_idx_delta_map
    };

    ثابت Pair_قيمة_Record *record = &first_Pair_قيمة_Record;
    unsigned تعداد_2 = len, رقم = 0;
    لكل (unsigned i = 0; i < تعداد_2; i++)
    {
      إذا (glyphset.has (record->secondGlyph)
         && record->subset (c, &context)) رقم++;
      record = &StructAtOffset<ثابت Pair_قيمة_Record> (record, record_size);
    }

    out->len = رقم;
    إذا (!رقم) c->serializer->revert (snap);
    return_trace (رقم);
  }
};


}
}
}

#endif  // OT_LAYOUT_GPOS_PAIRSET_HH
