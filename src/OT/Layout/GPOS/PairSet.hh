#ifndef OT_LAYOUT_GPOS_PAIRSET_HH
#define OT_LAYOUT_GPOS_PAIRSET_HH

#شامل "PairValueRecord.hh"

فضاینام OT {
فضاینام Layout {
فضاینام GPOS_impl {


/* Pair records vary in glyph width; their value traversal need not. */
ساختار pair_set_مقادیر_t
{
  ثابت مقدار_Base *base;
  glyph_data_t glyphs;
  ثابت مقدار *values;
  unsigned شمار;
  unsigned record_size;

  unsigned get_glyph (unsigned i) ثابت
  {
    glyph_data_t glyph = glyphs;
    glyph.data = (ثابت نویسه *) glyph.data + i * record_size;
    بازگشت glyph[0];
  }

  ثابت مقدار *get_مقادیر (unsigned i) ثابت
  { بازگشت reinterpret_cast<ثابت مقدار *> ((ثابت نویسه *) values + i * record_size); }

  hb_pair_t<unsigned, unsigned>
  compute_effective_مقدار_formats (ثابت hb_set_t &glyphset,
                                   ثابت مقدار_Format مقدار_Formats[2],
                                   منطقی strip_hints, منطقی strip_empty,
                                   ثابت hb_hashmap_t<unsigned, hb_pair_t<unsigned, صحیح>> *varidx_delta_map) ثابت
  {
    unsigned len1 = مقدار_Formats[0].get_len ();
    unsigned format1 = 0, format2 = 0;
    برای (unsigned i = 0; i < شمار; i++)
    {
      اگر (!glyphset.has (get_glyph (i))) ادامه;
      ثابت مقدار *v = get_مقادیر (i);
      format1 |= مقدار_Formats[0].get_effective_format (v, strip_hints, strip_empty, base, varidx_delta_map);
      format2 |= مقدار_Formats[1].get_effective_format (v + len1, strip_hints, strip_empty, base, varidx_delta_map);
    }
    بازگشت hb_pair (format1, format2);
  }

  تهی collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ثابت مقدار_Format مقدار_Formats[2]) ثابت
  {
    unsigned len1 = مقدار_Formats[0].get_len ();
    unsigned len2 = مقدار_Formats[1].get_len ();
    برای (unsigned i = 0; i < شمار; i++)
    {
      اگر (!c->glyph_set->has (get_glyph (i))) ادامه;
      ثابت hb_array_t<ثابت مقدار> v = hb_array (get_مقادیر (i), len1 + len2);
      اگر (مقدار_Formats[0].has_device ())
        مقدار_Formats[0].collect_variation_indices (c, base, v.sub_array (0, len1));
      اگر (مقدار_Formats[1].has_device ())
        مقدار_Formats[1].collect_variation_indices (c, base, v.sub_array (len1, len2));
    }
  }
};

template <typename Types>
ساختار PairSet : مقدار_Base
{
  template <typename Types2>
  friend ساختار PairPosFormat1_3;

  using Pair_مقدار_Record = GPOS_impl::Pair_مقدار_Record<Types>;

  محافظتشده:
  typename Types::HBUINT
                        len;    /* Number of PairValueRecords */
  Pair_مقدار_Record       first_Pair_مقدار_Record;
                                /* Array of PairValueRecords--ordered
                                 * by GlyphID of the second glyph */
  عمومی:
  DEFINE_SIZE_MIN (Types::HBUINT::static_size);

  ایستا size_t get_size (unsigned len1, unsigned len2)
  {
    بازگشت Types::HBGlyphID::static_size + مقدار::static_size * (len1 + len2);
  }
  ایستا size_t get_size (ثابت مقدار_Format مقدار_Formats[2])
  {
    unsigned len1 = مقدار_Formats[0].get_len ();
    unsigned len2 = مقدار_Formats[1].get_len ();
    بازگشت get_size (len1, len2);
  }

  ساختار sanitize_closure_t
  {
    ثابت مقدار_Format *مقدار_Formats;
    unsigned صحیح len1; /* valueFormats[0].get_len() */
    size_t stride; /* bytes */
  };

  منطقی sanitize (hb_sanitize_context_t *c, ثابت sanitize_closure_t *closure) ثابت
  {
    TRACE_SANITIZE (این);
    اگر (!(c->check_struct (این) &&
	  hb_barrier () &&
          c->check_range (&first_Pair_مقدار_Record,
                          len,
                          closure->stride))) return_trace (نادرست);
    hb_barrier ();

    unsigned صحیح شمار = len;
    ثابت Pair_مقدار_Record *record = &first_Pair_مقدار_Record;
    return_trace (c->lazy_some_gpos ||
		  (closure->مقدار_Formats[0].sanitize_مقادیر_stride_unsafe (c, این, &record->values[0], شمار, closure->stride) &&
                   closure->مقدار_Formats[1].sanitize_مقادیر_stride_unsafe (c, این, &record->values[closure->len1], شمار, closure->stride)));
  }

  منطقی intersects (ثابت hb_set_t *glyphs,
                   ثابت مقدار_Format *مقدار_Formats) ثابت
  {
    unsigned record_size = get_size (مقدار_Formats);

    ثابت Pair_مقدار_Record *record = &first_Pair_مقدار_Record;
    unsigned صحیح شمار = len;
    برای (unsigned صحیح i = 0; i < شمار; i++)
    {
      اگر (glyphs->has (record->secondGlyph))
        بازگشت درست;
      record = &StructAtOffset<ثابت Pair_مقدار_Record> (record, record_size);
    }
    بازگشت نادرست;
  }

  تهی collect_glyphs (hb_collect_glyphs_context_t *c,
                       ثابت مقدار_Format *مقدار_Formats) ثابت
  {
    unsigned record_size = get_size (مقدار_Formats);

    ثابت Pair_مقدار_Record *record = &first_Pair_مقدار_Record;
    c->درون_داد->add_array (&record->secondGlyph, len, record_size);
  }

  template <typename set_t>
  تهی collect_second_glyphs (set_t *glyphs,
			      ثابت مقدار_Format *مقدار_Formats) ثابت
  {
    unsigned record_size = get_size (مقدار_Formats);
    ثابت Pair_مقدار_Record *record = &first_Pair_مقدار_Record;
    glyphs->add_array (&record->secondGlyph, len, record_size);
  }

  pair_set_مقادیر_t get_مقادیر_view (ثابت مقدار_Format مقدار_Formats[2]) ثابت
  {
    بازگشت {این, &first_Pair_مقدار_Record.secondGlyph, &first_Pair_مقدار_Record.values[0],
            (unsigned) len, (unsigned) get_size (مقدار_Formats)};
  }

  تهی collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ثابت مقدار_Format *مقدار_Formats) ثابت
  {
    get_مقادیر_view (مقدار_Formats).collect_variation_indices (c, مقدار_Formats);
  }

  منطقی apply (hb_ot_apply_context_t *c,
              ثابت مقدار_Format *مقدار_Formats,
              unsigned صحیح pos) ثابت
  {
    TRACE_APPLY (این);
    hb_buffer_t *buffer = c->buffer;
    unsigned صحیح len1 = مقدار_Formats[0].get_len ();
    unsigned صحیح len2 = مقدار_Formats[1].get_len ();
    unsigned record_size = get_size (len1, len2);

    ثابت Pair_مقدار_Record *record = hb_bsearch (buffer->info[pos].codepoint,
                                                &first_Pair_مقدار_Record,
                                                (unsigned) len,
                                                record_size);
    اگر (record)
    {
      اگر (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "try kerning glyphs at %u,%u",
			    c->buffer->idx, pos);
      }

      منطقی applied_first = len1 && مقدار_Formats[0].apply_مقدار (c, این, &record->values[0], buffer->cur_pos());
      منطقی applied_second = len2 && مقدار_Formats[1].apply_مقدار (c, این, &record->values[len1], buffer->pos[pos]);

      اگر (applied_first || applied_second)
	اگر (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
	{
	  c->buffer->message (c->font,
			      "kerned glyphs at %u,%u",
			      c->buffer->idx, pos);
	}

      اگر (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "tried kerning glyphs at %u,%u",
			    c->buffer->idx, pos);
      }

      اگر (applied_first || applied_second)
        buffer->unsafe_to_break (buffer->idx, pos + 1);
      وگرنه
        /* Even a zero-valued pair record is a concat hazard. */
        buffer->unsafe_to_concat (buffer->idx, pos + 1);

      اگر (len2)
      {
	pos++;
      // https://github.com/harfbuzz/harfbuzz/issues/3824
      // https://github.com/harfbuzz/harfbuzz/issues/3888#issuecomment-1326781116
      buffer->unsafe_to_break (buffer->idx, pos + 1);
      }

      buffer->idx = pos;
      return_trace (درست);
    }
    buffer->unsafe_to_concat (buffer->idx, pos + 1);
    return_trace (نادرست);
  }

  منطقی subset (hb_subset_context_t *c,
               ثابت مقدار_Format مقدار_Formats[2],
               ثابت مقدار_Format newFormats[2]) ثابت
  {
    TRACE_SUBSET (این);
    auto snap = c->serializer->snapshot ();

    auto *out = c->serializer->start_embed (*این);
    اگر (unlikely (!c->serializer->extend_min (out))) return_trace (نادرست);
    out->len = 0;

    ثابت hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    unsigned len1 = مقدار_Formats[0].get_len ();
    unsigned len2 = مقدار_Formats[1].get_len ();
    unsigned record_size = get_size (len1, len2);

    pair_مقدار_record_context_t context =
    {
      این,
      مقدار_Formats,
      newFormats,
      len1,
      &glyph_map,
      &c->plan->layout_variation_idx_delta_map
    };

    ثابت Pair_مقدار_Record *record = &first_Pair_مقدار_Record;
    unsigned شمار = len, رقم = 0;
    برای (unsigned i = 0; i < شمار; i++)
    {
      اگر (glyphset.has (record->secondGlyph)
         && record->subset (c, &context)) رقم++;
      record = &StructAtOffset<ثابت Pair_مقدار_Record> (record, record_size);
    }

    out->len = رقم;
    اگر (!رقم) c->serializer->revert (snap);
    return_trace (رقم);
  }
};


}
}
}

#endif  // OT_LAYOUT_GPOS_PAIRSET_HH
