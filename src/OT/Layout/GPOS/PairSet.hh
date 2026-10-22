#ifndef OT_LAYOUT_GPOS_PAIRSET_HH
#define OT_LAYOUT_GPOS_PAIRSET_HH

#ضمن "PairValueRecord.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {


/* Pair records vary in glyph width; their value traversal need not. */
بنية pair_set_درجوجي_t
{
  ثابت درج_Base *base;
  glyph_data_t glyphs;
  ثابت درج *values;
  unsigned كدي;
  unsigned record_size;

  unsigned get_glyph (unsigned i) ثابت
  {
    glyph_data_t glyph = glyphs;
    glyph.data = (ثابت حرف *) glyph.data + i * record_size;
    أرجع glyph[0];
  }

  ثابت درج *get_درجوجي (unsigned i) ثابت
  { أرجع reinterpret_cast<ثابت درج *> ((ثابت حرف *) values + i * record_size); }

  hb_pair_t<unsigned, unsigned>
  compute_effective_درج_formats (ثابت hb_set_t &glyphset,
                                   ثابت درج_Format درج_Formats[2],
                                   منطقي strip_hints, منطقي strip_empty,
                                   ثابت hb_hashmap_t<unsigned, hb_pair_t<unsigned, صحيح>> *varidx_delta_map) ثابت
  {
    unsigned len1 = درج_Formats[0].get_len ();
    unsigned format1 = 0, format2 = 0;
    لكل (unsigned i = 0; i < كدي; i++)
    {
      إذا (!glyphset.has (get_glyph (i))) استمر;
      ثابت درج *v = get_درجوجي (i);
      format1 |= درج_Formats[0].get_effective_format (v, strip_hints, strip_empty, base, varidx_delta_map);
      format2 |= درج_Formats[1].get_effective_format (v + len1, strip_hints, strip_empty, base, varidx_delta_map);
    }
    أرجع hb_pair (format1, format2);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ثابت درج_Format درج_Formats[2]) ثابت
  {
    unsigned len1 = درج_Formats[0].get_len ();
    unsigned len2 = درج_Formats[1].get_len ();
    لكل (unsigned i = 0; i < كدي; i++)
    {
      إذا (!c->glyph_set->has (get_glyph (i))) استمر;
      ثابت hb_array_t<ثابت درج> v = hb_array (get_درجوجي (i), len1 + len2);
      إذا (درج_Formats[0].has_device ())
        درج_Formats[0].collect_variation_indices (c, base, v.sub_array (0, len1));
      إذا (درج_Formats[1].has_device ())
        درج_Formats[1].collect_variation_indices (c, base, v.sub_array (len1, len2));
    }
  }
};

template <typename Types>
بنية PairSet : درج_Base
{
  template <typename Types2>
  friend بنية PairPosFormat1_3;

  using Pair_درج_Record = GPOS_impl::Pair_درج_Record<Types>;

  محمي:
  typename Types::HBUINT
                        len;    /* Number of PairValueRecords */
  Pair_درج_Record       first_Pair_درج_Record;
                                /* Array of PairValueRecords--ordered
                                 * by GlyphID of the second glyph */
  عام:
  DEFINE_SIZE_MIN (Types::HBUINT::static_size);

  ساكن size_t get_size (unsigned len1, unsigned len2)
  {
    أرجع Types::HBGlyphID::static_size + درج::static_size * (len1 + len2);
  }
  ساكن size_t get_size (ثابت درج_Format درج_Formats[2])
  {
    unsigned len1 = درج_Formats[0].get_len ();
    unsigned len2 = درج_Formats[1].get_len ();
    أرجع get_size (len1, len2);
  }

  بنية sanitize_closure_t
  {
    ثابت درج_Format *درج_Formats;
    unsigned صحيح len1; /* valueFormats[0].get_len() */
    size_t stride; /* bytes */
  };

  منطقي sanitize (hb_sanitize_context_t *c, ثابت sanitize_closure_t *closure) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (!(c->check_struct (هذا) &&
	  hb_barrier () &&
          c->check_range (&first_Pair_درج_Record,
                          len,
                          closure->stride))) return_trace (خطأ);
    hb_barrier ();

    unsigned صحيح كدي = len;
    ثابت Pair_درج_Record *record = &first_Pair_درج_Record;
    return_trace (c->lazy_some_gpos ||
		  (closure->درج_Formats[0].sanitize_درجوجي_stride_unsafe (c, هذا, &record->values[0], كدي, closure->stride) &&
                   closure->درج_Formats[1].sanitize_درجوجي_stride_unsafe (c, هذا, &record->values[closure->len1], كدي, closure->stride)));
  }

  منطقي intersects (ثابت hb_set_t *glyphs,
                   ثابت درج_Format *درج_Formats) ثابت
  {
    unsigned record_size = get_size (درج_Formats);

    ثابت Pair_درج_Record *record = &first_Pair_درج_Record;
    unsigned صحيح كدي = len;
    لكل (unsigned صحيح i = 0; i < كدي; i++)
    {
      إذا (glyphs->has (record->secondGlyph))
        أرجع true;
      record = &StructAtOffset<ثابت Pair_درج_Record> (record, record_size);
    }
    أرجع خطأ;
  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c,
                       ثابت درج_Format *درج_Formats) ثابت
  {
    unsigned record_size = get_size (درج_Formats);

    ثابت Pair_درج_Record *record = &first_Pair_درج_Record;
    c->شغرو->add_array (&record->secondGlyph, len, record_size);
  }

  template <typename set_t>
  فراغ collect_second_glyphs (set_t *glyphs,
			      ثابت درج_Format *درج_Formats) ثابت
  {
    unsigned record_size = get_size (درج_Formats);
    ثابت Pair_درج_Record *record = &first_Pair_درج_Record;
    glyphs->add_array (&record->secondGlyph, len, record_size);
  }

  pair_set_درجوجي_t get_درجوجي_view (ثابت درج_Format درج_Formats[2]) ثابت
  {
    أرجع {هذا, &first_Pair_درج_Record.secondGlyph, &first_Pair_درج_Record.values[0],
            (unsigned) len, (unsigned) get_size (درج_Formats)};
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ثابت درج_Format *درج_Formats) ثابت
  {
    get_درجوجي_view (درج_Formats).collect_variation_indices (c, درج_Formats);
  }

  منطقي apply (hb_ot_apply_context_t *c,
              ثابت درج_Format *درج_Formats,
              unsigned صحيح pos) ثابت
  {
    TRACE_APPLY (هذا);
    hb_buffer_t *buffer = c->buffer;
    unsigned صحيح len1 = درج_Formats[0].get_len ();
    unsigned صحيح len2 = درج_Formats[1].get_len ();
    unsigned record_size = get_size (len1, len2);

    ثابت Pair_درج_Record *record = hb_bsearch (buffer->info[pos].codepoint,
                                                &first_Pair_درج_Record,
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

      منطقي applied_first = len1 && درج_Formats[0].apply_درج (c, هذا, &record->values[0], buffer->cur_pos());
      منطقي applied_second = len2 && درج_Formats[1].apply_درج (c, هذا, &record->values[len1], buffer->pos[pos]);

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
               ثابت درج_Format درج_Formats[2],
               ثابت درج_Format newFormats[2]) ثابت
  {
    TRACE_SUBSET (هذا);
    auto snap = c->serializer->snapshot ();

    auto *out = c->serializer->start_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (out))) return_trace (خطأ);
    out->len = 0;

    ثابت hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    unsigned len1 = درج_Formats[0].get_len ();
    unsigned len2 = درج_Formats[1].get_len ();
    unsigned record_size = get_size (len1, len2);

    pair_درج_record_context_t context =
    {
      هذا,
      درج_Formats,
      newFormats,
      len1,
      &glyph_map,
      &c->plan->layout_variation_idx_delta_map
    };

    ثابت Pair_درج_Record *record = &first_Pair_درج_Record;
    unsigned كدي = len, لمب = 0;
    لكل (unsigned i = 0; i < كدي; i++)
    {
      إذا (glyphset.has (record->secondGlyph)
         && record->subset (c, &context)) لمب++;
      record = &StructAtOffset<ثابت Pair_درج_Record> (record, record_size);
    }

    out->len = لمب;
    إذا (!لمب) c->serializer->revert (snap);
    return_trace (لمب);
  }
};


}
}
}

#endif  // OT_LAYOUT_GPOS_PAIRSET_HH
