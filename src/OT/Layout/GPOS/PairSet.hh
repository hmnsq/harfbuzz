#ifndef OT_LAYOUT_GPOS_PAIRSET_HH
#define OT_LAYOUT_GPOS_PAIRSET_HH

#অন্তর্ভাব "PairValueRecord.hh"

নামাকাশ OT {
নামাকাশ Layout {
নামাকাশ GPOS_impl {


/* Pair records vary in glyph width; their value traversal need not. */
সংরচনা pair_set_মানসমূহ_t
{
  ধ্রুব মান_Base *base;
  glyph_data_t glyphs;
  ধ্রুব মান *values;
  unsigned গুনতি;
  unsigned record_size;

  unsigned get_glyph (unsigned i) ধ্রুব
  {
    glyph_data_t glyph = glyphs;
    glyph.data = (ধ্রুব বর্ণ *) glyph.data + i * record_size;
    প্রত্যাগম glyph[0];
  }

  ধ্রুব মান *get_মানসমূহ (unsigned i) ধ্রুব
  { প্রত্যাগম reinterpret_cast<ধ্রুব মান *> ((ধ্রুব বর্ণ *) values + i * record_size); }

  hb_pair_t<unsigned, unsigned>
  compute_effective_মান_formats (ধ্রুব hb_set_t &glyphset,
                                   ধ্রুব মান_Format মান_Formats[2],
                                   তার্কিক strip_hints, তার্কিক strip_empty,
                                   ধ্রুব hb_hashmap_t<unsigned, hb_pair_t<unsigned, পূর্ণাঙ্ক>> *varidx_delta_map) ধ্রুব
  {
    unsigned len1 = মান_Formats[0].get_len ();
    unsigned format1 = 0, format2 = 0;
    প্রতি (unsigned i = 0; i < গুনতি; i++)
    {
      যদি (!glyphset.has (get_glyph (i))) অনুবর্ত;
      ধ্রুব মান *v = get_মানসমূহ (i);
      format1 |= মান_Formats[0].get_effective_format (v, strip_hints, strip_empty, base, varidx_delta_map);
      format2 |= মান_Formats[1].get_effective_format (v + len1, strip_hints, strip_empty, base, varidx_delta_map);
    }
    প্রত্যাগম hb_pair (format1, format2);
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ধ্রুব মান_Format মান_Formats[2]) ধ্রুব
  {
    unsigned len1 = মান_Formats[0].get_len ();
    unsigned len2 = মান_Formats[1].get_len ();
    প্রতি (unsigned i = 0; i < গুনতি; i++)
    {
      যদি (!c->glyph_set->has (get_glyph (i))) অনুবর্ত;
      ধ্রুব hb_array_t<ধ্রুব মান> v = hb_array (get_মানসমূহ (i), len1 + len2);
      যদি (মান_Formats[0].has_device ())
        মান_Formats[0].collect_variation_indices (c, base, v.sub_array (0, len1));
      যদি (মান_Formats[1].has_device ())
        মান_Formats[1].collect_variation_indices (c, base, v.sub_array (len1, len2));
    }
  }
};

template <typename Types>
সংরচনা PairSet : মান_Base
{
  template <typename Types2>
  friend সংরচনা PairPosFormat1_3;

  using Pair_মান_Record = GPOS_impl::Pair_মান_Record<Types>;

  রক্ষিত:
  typename Types::HBUINT
                        len;    /* Number of PairValueRecords */
  Pair_মান_Record       first_Pair_মান_Record;
                                /* Array of PairValueRecords--ordered
                                 * by GlyphID of the second glyph */
  সার্বজনিক:
  DEFINE_SIZE_MIN (Types::HBUINT::static_size);

  স্থির size_t get_size (unsigned len1, unsigned len2)
  {
    প্রত্যাগম Types::HBGlyphID::static_size + মান::static_size * (len1 + len2);
  }
  স্থির size_t get_size (ধ্রুব মান_Format মান_Formats[2])
  {
    unsigned len1 = মান_Formats[0].get_len ();
    unsigned len2 = মান_Formats[1].get_len ();
    প্রত্যাগম get_size (len1, len2);
  }

  সংরচনা sanitize_closure_t
  {
    ধ্রুব মান_Format *মান_Formats;
    unsigned পূর্ণাঙ্ক len1; /* valueFormats[0].get_len() */
    size_t stride; /* bytes */
  };

  তার্কিক sanitize (hb_sanitize_context_t *c, ধ্রুব sanitize_closure_t *closure) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    যদি (!(c->check_struct (এতত্) &&
	  hb_barrier () &&
          c->check_range (&first_Pair_মান_Record,
                          len,
                          closure->stride))) return_trace (অসত্য);
    hb_barrier ();

    unsigned পূর্ণাঙ্ক গুনতি = len;
    ধ্রুব Pair_মান_Record *record = &first_Pair_মান_Record;
    return_trace (c->lazy_some_gpos ||
		  (closure->মান_Formats[0].sanitize_মানসমূহ_stride_unsafe (c, এতত্, &record->values[0], গুনতি, closure->stride) &&
                   closure->মান_Formats[1].sanitize_মানসমূহ_stride_unsafe (c, এতত্, &record->values[closure->len1], গুনতি, closure->stride)));
  }

  তার্কিক intersects (ধ্রুব hb_set_t *glyphs,
                   ধ্রুব মান_Format *মান_Formats) ধ্রুব
  {
    unsigned record_size = get_size (মান_Formats);

    ধ্রুব Pair_মান_Record *record = &first_Pair_মান_Record;
    unsigned পূর্ণাঙ্ক গুনতি = len;
    প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
    {
      যদি (glyphs->has (record->secondGlyph))
        প্রত্যাগম সত্য;
      record = &StructAtOffset<ধ্রুব Pair_মান_Record> (record, record_size);
    }
    প্রত্যাগম অসত্য;
  }

  শূন্য collect_glyphs (hb_collect_glyphs_context_t *c,
                       ধ্রুব মান_Format *মান_Formats) ধ্রুব
  {
    unsigned record_size = get_size (মান_Formats);

    ধ্রুব Pair_মান_Record *record = &first_Pair_মান_Record;
    c->প্রবেশ->add_array (&record->secondGlyph, len, record_size);
  }

  template <typename set_t>
  শূন্য collect_second_glyphs (set_t *glyphs,
			      ধ্রুব মান_Format *মান_Formats) ধ্রুব
  {
    unsigned record_size = get_size (মান_Formats);
    ধ্রুব Pair_মান_Record *record = &first_Pair_মান_Record;
    glyphs->add_array (&record->secondGlyph, len, record_size);
  }

  pair_set_মানসমূহ_t get_মানসমূহ_view (ধ্রুব মান_Format মান_Formats[2]) ধ্রুব
  {
    প্রত্যাগম {এতত্, &first_Pair_মান_Record.secondGlyph, &first_Pair_মান_Record.values[0],
            (unsigned) len, (unsigned) get_size (মান_Formats)};
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ধ্রুব মান_Format *মান_Formats) ধ্রুব
  {
    get_মানসমূহ_view (মান_Formats).collect_variation_indices (c, মান_Formats);
  }

  তার্কিক apply (hb_ot_apply_context_t *c,
              ধ্রুব মান_Format *মান_Formats,
              unsigned পূর্ণাঙ্ক pos) ধ্রুব
  {
    TRACE_APPLY (এতত্);
    hb_buffer_t *buffer = c->buffer;
    unsigned পূর্ণাঙ্ক len1 = মান_Formats[0].get_len ();
    unsigned পূর্ণাঙ্ক len2 = মান_Formats[1].get_len ();
    unsigned record_size = get_size (len1, len2);

    ধ্রুব Pair_মান_Record *record = hb_bsearch (buffer->info[pos].codepoint,
                                                &first_Pair_মান_Record,
                                                (unsigned) len,
                                                record_size);
    যদি (record)
    {
      যদি (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "try kerning glyphs at %u,%u",
			    c->buffer->idx, pos);
      }

      তার্কিক applied_first = len1 && মান_Formats[0].apply_মান (c, এতত্, &record->values[0], buffer->cur_pos());
      তার্কিক applied_second = len2 && মান_Formats[1].apply_মান (c, এতত্, &record->values[len1], buffer->pos[pos]);

      যদি (applied_first || applied_second)
	যদি (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
	{
	  c->buffer->message (c->font,
			      "kerned glyphs at %u,%u",
			      c->buffer->idx, pos);
	}

      যদি (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "tried kerning glyphs at %u,%u",
			    c->buffer->idx, pos);
      }

      যদি (applied_first || applied_second)
        buffer->unsafe_to_break (buffer->idx, pos + 1);
      অন্যথা
        /* Even a zero-valued pair record is a concat hazard. */
        buffer->unsafe_to_concat (buffer->idx, pos + 1);

      যদি (len2)
      {
	pos++;
      // https://github.com/harfbuzz/harfbuzz/issues/3824
      // https://github.com/harfbuzz/harfbuzz/issues/3888#issuecomment-1326781116
      buffer->unsafe_to_break (buffer->idx, pos + 1);
      }

      buffer->idx = pos;
      return_trace (সত্য);
    }
    buffer->unsafe_to_concat (buffer->idx, pos + 1);
    return_trace (অসত্য);
  }

  তার্কিক subset (hb_subset_context_t *c,
               ধ্রুব মান_Format মান_Formats[2],
               ধ্রুব মান_Format newFormats[2]) ধ্রুব
  {
    TRACE_SUBSET (এতত্);
    auto snap = c->serializer->snapshot ();

    auto *out = c->serializer->start_embed (*এতত্);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (অসত্য);
    out->len = 0;

    ধ্রুব hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ধ্রুব hb_map_t &glyph_map = *c->plan->glyph_map;

    unsigned len1 = মান_Formats[0].get_len ();
    unsigned len2 = মান_Formats[1].get_len ();
    unsigned record_size = get_size (len1, len2);

    pair_মান_record_context_t context =
    {
      এতত্,
      মান_Formats,
      newFormats,
      len1,
      &glyph_map,
      &c->plan->layout_variation_idx_delta_map
    };

    ধ্রুব Pair_মান_Record *record = &first_Pair_মান_Record;
    unsigned গুনতি = len, অঙ্ক = 0;
    প্রতি (unsigned i = 0; i < গুনতি; i++)
    {
      যদি (glyphset.has (record->secondGlyph)
         && record->subset (c, &context)) অঙ্ক++;
      record = &StructAtOffset<ধ্রুব Pair_মান_Record> (record, record_size);
    }

    out->len = অঙ্ক;
    যদি (!অঙ্ক) c->serializer->revert (snap);
    return_trace (অঙ্ক);
  }
};


}
}
}

#endif  // OT_LAYOUT_GPOS_PAIRSET_HH
