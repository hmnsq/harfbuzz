#ifndef OT_LAYOUT_GPOS_PAIRSET_HH
#define OT_LAYOUT_GPOS_PAIRSET_HH

#अन्तर्भाव "PairValueRecord.hh"

नामाकाश OT {
नामाकाश Layout {
नामाकाश GPOS_impl {


/* Pair records vary in glyph width; their value traversal need not. */
संरचना pair_set_मूल्यावली_t
{
  ध्रुव मूल्य_Base *base;
  glyph_data_t glyphs;
  ध्रुव मूल्य *values;
  अचिन्हित गिनती;
  अचिन्हित record_size;

  अचिन्हित get_glyph (अचिन्हित i) ध्रुव
  {
    glyph_data_t glyph = glyphs;
    glyph.data = (ध्रुव वर्ण *) glyph.data + i * record_size;
    प्रत्यागम glyph[0];
  }

  ध्रुव मूल्य *get_मूल्यावली (अचिन्हित i) ध्रुव
  { प्रत्यागम reinterpret_cast<ध्रुव मूल्य *> ((ध्रुव वर्ण *) values + i * record_size); }

  hb_pair_t<अचिन्हित, अचिन्हित>
  compute_effective_मूल्य_formats (ध्रुव hb_set_t &glyphset,
                                   ध्रुव मूल्य_Format मूल्य_Formats[2],
                                   तार्किक strip_hints, तार्किक strip_empty,
                                   ध्रुव hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णाङ्क>> *varidx_delta_map) ध्रुव
  {
    अचिन्हित len1 = मूल्य_Formats[0].get_len ();
    अचिन्हित format1 = 0, format2 = 0;
    प्रति (अचिन्हित i = 0; i < गिनती; i++)
    {
      यदि (!glyphset.has (get_glyph (i))) अनुवर्त;
      ध्रुव मूल्य *v = get_मूल्यावली (i);
      format1 |= मूल्य_Formats[0].get_effective_format (v, strip_hints, strip_empty, base, varidx_delta_map);
      format2 |= मूल्य_Formats[1].get_effective_format (v + len1, strip_hints, strip_empty, base, varidx_delta_map);
    }
    प्रत्यागम hb_pair (format1, format2);
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ध्रुव मूल्य_Format मूल्य_Formats[2]) ध्रुव
  {
    अचिन्हित len1 = मूल्य_Formats[0].get_len ();
    अचिन्हित len2 = मूल्य_Formats[1].get_len ();
    प्रति (अचिन्हित i = 0; i < गिनती; i++)
    {
      यदि (!c->glyph_set->has (get_glyph (i))) अनुवर्त;
      ध्रुव hb_array_t<ध्रुव मूल्य> v = hb_array (get_मूल्यावली (i), len1 + len2);
      यदि (मूल्य_Formats[0].has_device ())
        मूल्य_Formats[0].collect_variation_indices (c, base, v.sub_array (0, len1));
      यदि (मूल्य_Formats[1].has_device ())
        मूल्य_Formats[1].collect_variation_indices (c, base, v.sub_array (len1, len2));
    }
  }
};

ढाँचा <typename Types>
संरचना PairSet : मूल्य_Base
{
  ढाँचा <typename Types2>
  मित्र संरचना PairPosFormat1_3;

  using Pair_मूल्य_Record = GPOS_impl::Pair_मूल्य_Record<Types>;

  रक्षित:
  typename Types::HBUINT
                        len;    /* Number of PairValueRecords */
  Pair_मूल्य_Record       first_Pair_मूल्य_Record;
                                /* Array of PairValueRecords--ordered
                                 * by GlyphID of the second glyph */
  सार्वजनिक:
  DEFINE_SIZE_MIN (Types::HBUINT::static_size);

  स्थिर माप_प्रकार get_size (अचिन्हित len1, अचिन्हित len2)
  {
    प्रत्यागम Types::HBGlyphID::static_size + मूल्य::static_size * (len1 + len2);
  }
  स्थिर माप_प्रकार get_size (ध्रुव मूल्य_Format मूल्य_Formats[2])
  {
    अचिन्हित len1 = मूल्य_Formats[0].get_len ();
    अचिन्हित len2 = मूल्य_Formats[1].get_len ();
    प्रत्यागम get_size (len1, len2);
  }

  संरचना sanitize_closure_t
  {
    ध्रुव मूल्य_Format *मूल्य_Formats;
    अचिन्हित पूर्णाङ्क len1; /* valueFormats[0].get_len() */
    माप_प्रकार stride; /* bytes */
  };

  तार्किक sanitize (hb_sanitize_context_t *c, ध्रुव sanitize_closure_t *closure) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    यदि (!(c->check_struct (एतत्) &&
	  hb_barrier () &&
          c->check_range (&first_Pair_मूल्य_Record,
                          len,
                          closure->stride))) return_trace (असत्य);
    hb_barrier ();

    अचिन्हित पूर्णाङ्क गिनती = len;
    ध्रुव Pair_मूल्य_Record *record = &first_Pair_मूल्य_Record;
    return_trace (c->lazy_some_gpos ||
		  (closure->मूल्य_Formats[0].sanitize_मूल्यावली_stride_unsafe (c, एतत्, &record->values[0], गिनती, closure->stride) &&
                   closure->मूल्य_Formats[1].sanitize_मूल्यावली_stride_unsafe (c, एतत्, &record->values[closure->len1], गिनती, closure->stride)));
  }

  तार्किक intersects (ध्रुव hb_set_t *glyphs,
                   ध्रुव मूल्य_Format *मूल्य_Formats) ध्रुव
  {
    अचिन्हित record_size = get_size (मूल्य_Formats);

    ध्रुव Pair_मूल्य_Record *record = &first_Pair_मूल्य_Record;
    अचिन्हित पूर्णाङ्क गिनती = len;
    प्रति (अचिन्हित पूर्णाङ्क i = 0; i < गिनती; i++)
    {
      यदि (glyphs->has (record->secondGlyph))
        प्रत्यागम सत्य;
      record = &StructAtOffset<ध्रुव Pair_मूल्य_Record> (record, record_size);
    }
    प्रत्यागम असत्य;
  }

  शून्य collect_glyphs (hb_collect_glyphs_context_t *c,
                       ध्रुव मूल्य_Format *मूल्य_Formats) ध्रुव
  {
    अचिन्हित record_size = get_size (मूल्य_Formats);

    ध्रुव Pair_मूल्य_Record *record = &first_Pair_मूल्य_Record;
    c->निवेश->add_array (&record->secondGlyph, len, record_size);
  }

  ढाँचा <typename set_t>
  शून्य collect_second_glyphs (set_t *glyphs,
			      ध्रुव मूल्य_Format *मूल्य_Formats) ध्रुव
  {
    अचिन्हित record_size = get_size (मूल्य_Formats);
    ध्रुव Pair_मूल्य_Record *record = &first_Pair_मूल्य_Record;
    glyphs->add_array (&record->secondGlyph, len, record_size);
  }

  pair_set_मूल्यावली_t get_मूल्यावली_view (ध्रुव मूल्य_Format मूल्य_Formats[2]) ध्रुव
  {
    प्रत्यागम {एतत्, &first_Pair_मूल्य_Record.secondGlyph, &first_Pair_मूल्य_Record.values[0],
            (अचिन्हित) len, (अचिन्हित) get_size (मूल्य_Formats)};
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ध्रुव मूल्य_Format *मूल्य_Formats) ध्रुव
  {
    get_मूल्यावली_view (मूल्य_Formats).collect_variation_indices (c, मूल्य_Formats);
  }

  तार्किक apply (hb_ot_apply_context_t *c,
              ध्रुव मूल्य_Format *मूल्य_Formats,
              अचिन्हित पूर्णाङ्क pos) ध्रुव
  {
    TRACE_APPLY (एतत्);
    hb_buffer_t *buffer = c->buffer;
    अचिन्हित पूर्णाङ्क len1 = मूल्य_Formats[0].get_len ();
    अचिन्हित पूर्णाङ्क len2 = मूल्य_Formats[1].get_len ();
    अचिन्हित record_size = get_size (len1, len2);

    ध्रुव Pair_मूल्य_Record *record = hb_bsearch (buffer->info[pos].codepoint,
                                                &first_Pair_मूल्य_Record,
                                                (अचिन्हित) len,
                                                record_size);
    यदि (record)
    {
      यदि (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "try kerning glyphs at %u,%u",
			    c->buffer->idx, pos);
      }

      तार्किक applied_first = len1 && मूल्य_Formats[0].apply_मूल्य (c, एतत्, &record->values[0], buffer->cur_pos());
      तार्किक applied_second = len2 && मूल्य_Formats[1].apply_मूल्य (c, एतत्, &record->values[len1], buffer->pos[pos]);

      यदि (applied_first || applied_second)
	यदि (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
	{
	  c->buffer->message (c->font,
			      "kerned glyphs at %u,%u",
			      c->buffer->idx, pos);
	}

      यदि (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "tried kerning glyphs at %u,%u",
			    c->buffer->idx, pos);
      }

      यदि (applied_first || applied_second)
        buffer->unsafe_to_break (buffer->idx, pos + 1);
      अन्यथा
        /* Even a zero-valued pair record is a concat hazard. */
        buffer->unsafe_to_concat (buffer->idx, pos + 1);

      यदि (len2)
      {
	pos++;
      // https://github.com/harfbuzz/harfbuzz/issues/3824
      // https://github.com/harfbuzz/harfbuzz/issues/3888#issuecomment-1326781116
      buffer->unsafe_to_break (buffer->idx, pos + 1);
      }

      buffer->idx = pos;
      return_trace (सत्य);
    }
    buffer->unsafe_to_concat (buffer->idx, pos + 1);
    return_trace (असत्य);
  }

  तार्किक subset (hb_subset_context_t *c,
               ध्रुव मूल्य_Format मूल्य_Formats[2],
               ध्रुव मूल्य_Format newFormats[2]) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    स्वतः snap = c->serializer->snapshot ();

    स्वतः *out = c->serializer->start_embed (*एतत्);
    यदि (unlikely (!c->serializer->extend_min (out))) return_trace (असत्य);
    out->len = 0;

    ध्रुव hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ध्रुव hb_map_t &glyph_map = *c->plan->glyph_map;

    अचिन्हित len1 = मूल्य_Formats[0].get_len ();
    अचिन्हित len2 = मूल्य_Formats[1].get_len ();
    अचिन्हित record_size = get_size (len1, len2);

    pair_मूल्य_record_context_t context =
    {
      एतत्,
      मूल्य_Formats,
      newFormats,
      len1,
      &glyph_map,
      &c->plan->layout_variation_idx_delta_map
    };

    ध्रुव Pair_मूल्य_Record *record = &first_Pair_मूल्य_Record;
    अचिन्हित गिनती = len, अंक = 0;
    प्रति (अचिन्हित i = 0; i < गिनती; i++)
    {
      यदि (glyphset.has (record->secondGlyph)
         && record->subset (c, &context)) अंक++;
      record = &StructAtOffset<ध्रुव Pair_मूल्य_Record> (record, record_size);
    }

    out->len = अंक;
    यदि (!अंक) c->serializer->revert (snap);
    return_trace (अंक);
  }
};


}
}
}

#endif  // OT_LAYOUT_GPOS_PAIRSET_HH
