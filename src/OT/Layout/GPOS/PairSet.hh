#ifndef OT_LAYOUT_GPOS_PAIRSET_HH
#define OT_LAYOUT_GPOS_PAIRSET_HH

#ضمن "PairValueRecord.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {


/* Pair records vary in glyph width; their value traversal need not. */
بنية pair_set_نيلاي_سموا_t
{
  ثابت نيلاي_Base *base;
  glyph_data_t glyphs;
  ثابت نيلاي *values;
  unsigned کيراءن;
  unsigned record_size;

  unsigned get_glyph (unsigned i) ثابت
  {
    glyph_data_t glyph = glyphs;
    glyph.data = (ثابت حرف *) glyph.data + i * record_size;
    أرجع glyph[0];
  }

  ثابت نيلاي *get_نيلاي_سموا (unsigned i) ثابت
  { أرجع reinterpret_cast<ثابت نيلاي *> ((ثابت حرف *) values + i * record_size); }

  hb_pair_t<unsigned, unsigned>
  compute_effective_نيلاي_formats (ثابت hb_set_t &glyphset,
                                   ثابت نيلاي_Format نيلاي_Formats[2],
                                   منطقي strip_hints, منطقي strip_empty,
                                   ثابت hb_hashmap_t<unsigned, hb_pair_t<unsigned, صحيح>> *varidx_delta_map) ثابت
  {
    unsigned len1 = نيلاي_Formats[0].get_len ();
    unsigned format1 = 0, format2 = 0;
    لكل (unsigned i = 0; i < کيراءن; i++)
    {
      إذا (!glyphset.has (get_glyph (i))) استمر;
      ثابت نيلاي *v = get_نيلاي_سموا (i);
      format1 |= نيلاي_Formats[0].get_effective_format (v, strip_hints, strip_empty, base, varidx_delta_map);
      format2 |= نيلاي_Formats[1].get_effective_format (v + len1, strip_hints, strip_empty, base, varidx_delta_map);
    }
    أرجع hb_pair (format1, format2);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ثابت نيلاي_Format نيلاي_Formats[2]) ثابت
  {
    unsigned len1 = نيلاي_Formats[0].get_len ();
    unsigned len2 = نيلاي_Formats[1].get_len ();
    لكل (unsigned i = 0; i < کيراءن; i++)
    {
      إذا (!c->glyph_set->has (get_glyph (i))) استمر;
      ثابت hb_array_t<ثابت نيلاي> v = hb_array (get_نيلاي_سموا (i), len1 + len2);
      إذا (نيلاي_Formats[0].has_device ())
        نيلاي_Formats[0].collect_variation_indices (c, base, v.sub_array (0, len1));
      إذا (نيلاي_Formats[1].has_device ())
        نيلاي_Formats[1].collect_variation_indices (c, base, v.sub_array (len1, len2));
    }
  }
};

template <typename Types>
بنية PairSet : نيلاي_Base
{
  template <typename Types2>
  friend بنية PairPosFormat1_3;

  using Pair_نيلاي_Record = GPOS_impl::Pair_نيلاي_Record<Types>;

  محمي:
  typename Types::HBUINT
                        len;    /* Number of PairValueRecords */
  Pair_نيلاي_Record       first_Pair_نيلاي_Record;
                                /* Array of PairValueRecords--ordered
                                 * by GlyphID of the second glyph */
  عام:
  DEFINE_SIZE_MIN (Types::HBUINT::static_size);

  ساكن size_t get_size (unsigned len1, unsigned len2)
  {
    أرجع Types::HBGlyphID::static_size + نيلاي::static_size * (len1 + len2);
  }
  ساكن size_t get_size (ثابت نيلاي_Format نيلاي_Formats[2])
  {
    unsigned len1 = نيلاي_Formats[0].get_len ();
    unsigned len2 = نيلاي_Formats[1].get_len ();
    أرجع get_size (len1, len2);
  }

  بنية sanitize_closure_t
  {
    ثابت نيلاي_Format *نيلاي_Formats;
    unsigned صحيح len1; /* valueFormats[0].get_len() */
    size_t stride; /* bytes */
  };

  منطقي sanitize (hb_sanitize_context_t *c, ثابت sanitize_closure_t *closure) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (!(c->check_struct (هذا) &&
	  hb_barrier () &&
          c->check_range (&first_Pair_نيلاي_Record,
                          len,
                          closure->stride))) return_trace (خطأ);
    hb_barrier ();

    unsigned صحيح کيراءن = len;
    ثابت Pair_نيلاي_Record *record = &first_Pair_نيلاي_Record;
    return_trace (c->lazy_some_gpos ||
		  (closure->نيلاي_Formats[0].sanitize_نيلاي_سموا_stride_unsafe (c, هذا, &record->values[0], کيراءن, closure->stride) &&
                   closure->نيلاي_Formats[1].sanitize_نيلاي_سموا_stride_unsafe (c, هذا, &record->values[closure->len1], کيراءن, closure->stride)));
  }

  منطقي intersects (ثابت hb_set_t *glyphs,
                   ثابت نيلاي_Format *نيلاي_Formats) ثابت
  {
    unsigned record_size = get_size (نيلاي_Formats);

    ثابت Pair_نيلاي_Record *record = &first_Pair_نيلاي_Record;
    unsigned صحيح کيراءن = len;
    لكل (unsigned صحيح i = 0; i < کيراءن; i++)
    {
      إذا (glyphs->has (record->secondGlyph))
        أرجع true;
      record = &StructAtOffset<ثابت Pair_نيلاي_Record> (record, record_size);
    }
    أرجع خطأ;
  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c,
                       ثابت نيلاي_Format *نيلاي_Formats) ثابت
  {
    unsigned record_size = get_size (نيلاي_Formats);

    ثابت Pair_نيلاي_Record *record = &first_Pair_نيلاي_Record;
    c->ماسوقن->add_array (&record->secondGlyph, len, record_size);
  }

  template <typename set_t>
  فراغ collect_second_glyphs (set_t *glyphs,
			      ثابت نيلاي_Format *نيلاي_Formats) ثابت
  {
    unsigned record_size = get_size (نيلاي_Formats);
    ثابت Pair_نيلاي_Record *record = &first_Pair_نيلاي_Record;
    glyphs->add_array (&record->secondGlyph, len, record_size);
  }

  pair_set_نيلاي_سموا_t get_نيلاي_سموا_view (ثابت نيلاي_Format نيلاي_Formats[2]) ثابت
  {
    أرجع {هذا, &first_Pair_نيلاي_Record.secondGlyph, &first_Pair_نيلاي_Record.values[0],
            (unsigned) len, (unsigned) get_size (نيلاي_Formats)};
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ثابت نيلاي_Format *نيلاي_Formats) ثابت
  {
    get_نيلاي_سموا_view (نيلاي_Formats).collect_variation_indices (c, نيلاي_Formats);
  }

  منطقي apply (hb_ot_apply_context_t *c,
              ثابت نيلاي_Format *نيلاي_Formats,
              unsigned صحيح pos) ثابت
  {
    TRACE_APPLY (هذا);
    hb_buffer_t *buffer = c->buffer;
    unsigned صحيح len1 = نيلاي_Formats[0].get_len ();
    unsigned صحيح len2 = نيلاي_Formats[1].get_len ();
    unsigned record_size = get_size (len1, len2);

    ثابت Pair_نيلاي_Record *record = hb_bsearch (buffer->info[pos].codepoint,
                                                &first_Pair_نيلاي_Record,
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

      منطقي applied_first = len1 && نيلاي_Formats[0].apply_نيلاي (c, هذا, &record->values[0], buffer->cur_pos());
      منطقي applied_second = len2 && نيلاي_Formats[1].apply_نيلاي (c, هذا, &record->values[len1], buffer->pos[pos]);

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
               ثابت نيلاي_Format نيلاي_Formats[2],
               ثابت نيلاي_Format newFormats[2]) ثابت
  {
    TRACE_SUBSET (هذا);
    auto snap = c->serializer->snapshot ();

    auto *out = c->serializer->start_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (out))) return_trace (خطأ);
    out->len = 0;

    ثابت hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    unsigned len1 = نيلاي_Formats[0].get_len ();
    unsigned len2 = نيلاي_Formats[1].get_len ();
    unsigned record_size = get_size (len1, len2);

    pair_نيلاي_record_context_t context =
    {
      هذا,
      نيلاي_Formats,
      newFormats,
      len1,
      &glyph_map,
      &c->plan->layout_variation_idx_delta_map
    };

    ثابت Pair_نيلاي_Record *record = &first_Pair_نيلاي_Record;
    unsigned کيراءن = len, اڠک = 0;
    لكل (unsigned i = 0; i < کيراءن; i++)
    {
      إذا (glyphset.has (record->secondGlyph)
         && record->subset (c, &context)) اڠک++;
      record = &StructAtOffset<ثابت Pair_نيلاي_Record> (record, record_size);
    }

    out->len = اڠک;
    إذا (!اڠک) c->serializer->revert (snap);
    return_trace (اڠک);
  }
};


}
}
}

#endif  // OT_LAYOUT_GPOS_PAIRSET_HH
