#ifndef OT_LAYOUT_GPOS_PAIRSET_HH
#define OT_LAYOUT_GPOS_PAIRSET_HH

#अन्तर्भाव "PairValueRecord.hh"

नामाकाश OT {
नामाकाश Layout {
नामाकाश GPOS_impl {


/* Pair records vary in glyph width; their value traversal need not. */
संरचना pair_set_मानहरू_t
{
  अचर मान_Base *base;
  glyph_data_t glyphs;
  अचर मान *values;
  अचिन्हित गन्ती;
  अचिन्हित record_size;

  अचिन्हित get_glyph (अचिन्हित i) अचर
  {
    glyph_data_t glyph = glyphs;
    glyph.data = (अचर वर्ण *) glyph.data + i * record_size;
    फर्काऊ glyph[0];
  }

  अचर मान *get_मानहरू (अचिन्हित i) अचर
  { फर्काऊ reinterpret_cast<अचर मान *> ((अचर वर्ण *) values + i * record_size); }

  hb_pair_t<अचिन्हित, अचिन्हित>
  compute_effective_मान_formats (अचर hb_set_t &glyphset,
                                   अचर मान_Format मान_Formats[2],
                                   तार्किक strip_hints, तार्किक strip_empty,
                                   अचर hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णाङ्क>> *varidx_delta_map) अचर
  {
    अचिन्हित len1 = मान_Formats[0].get_len ();
    अचिन्हित format1 = 0, format2 = 0;
    लागि (अचिन्हित i = 0; i < गन्ती; i++)
    {
      यदि (!glyphset.has (get_glyph (i))) जारी;
      अचर मान *v = get_मानहरू (i);
      format1 |= मान_Formats[0].get_effective_format (v, strip_hints, strip_empty, base, varidx_delta_map);
      format2 |= मान_Formats[1].get_effective_format (v + len1, strip_hints, strip_empty, base, varidx_delta_map);
    }
    फर्काऊ hb_pair (format1, format2);
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  अचर मान_Format मान_Formats[2]) अचर
  {
    अचिन्हित len1 = मान_Formats[0].get_len ();
    अचिन्हित len2 = मान_Formats[1].get_len ();
    लागि (अचिन्हित i = 0; i < गन्ती; i++)
    {
      यदि (!c->glyph_set->has (get_glyph (i))) जारी;
      अचर hb_array_t<अचर मान> v = hb_array (get_मानहरू (i), len1 + len2);
      यदि (मान_Formats[0].has_device ())
        मान_Formats[0].collect_variation_indices (c, base, v.sub_array (0, len1));
      यदि (मान_Formats[1].has_device ())
        मान_Formats[1].collect_variation_indices (c, base, v.sub_array (len1, len2));
    }
  }
};

ढाँचा <typename Types>
संरचना PairSet : मान_Base
{
  ढाँचा <typename Types2>
  मित्र संरचना PairPosFormat1_3;

  using Pair_मान_Record = GPOS_impl::Pair_मान_Record<Types>;

  रक्षित:
  typename Types::HBUINT
                        len;    /* Number of PairValueRecords */
  Pair_मान_Record       first_Pair_मान_Record;
                                /* Array of PairValueRecords--ordered
                                 * by GlyphID of the second glyph */
  सार्वजनिक:
  DEFINE_SIZE_MIN (Types::HBUINT::static_size);

  स्थिर माप_प्रकार get_size (अचिन्हित len1, अचिन्हित len2)
  {
    फर्काऊ Types::HBGlyphID::static_size + मान::static_size * (len1 + len2);
  }
  स्थिर माप_प्रकार get_size (अचर मान_Format मान_Formats[2])
  {
    अचिन्हित len1 = मान_Formats[0].get_len ();
    अचिन्हित len2 = मान_Formats[1].get_len ();
    फर्काऊ get_size (len1, len2);
  }

  संरचना sanitize_closure_t
  {
    अचर मान_Format *मान_Formats;
    अचिन्हित पूर्णाङ्क len1; /* valueFormats[0].get_len() */
    माप_प्रकार stride; /* bytes */
  };

  तार्किक sanitize (hb_sanitize_context_t *c, अचर sanitize_closure_t *closure) अचर
  {
    TRACE_SANITIZE (एतत्);
    यदि (!(c->check_struct (एतत्) &&
	  hb_barrier () &&
          c->check_range (&first_Pair_मान_Record,
                          len,
                          closure->stride))) return_trace (असत्य);
    hb_barrier ();

    अचिन्हित पूर्णाङ्क गन्ती = len;
    अचर Pair_मान_Record *record = &first_Pair_मान_Record;
    return_trace (c->lazy_some_gpos ||
		  (closure->मान_Formats[0].sanitize_मानहरू_stride_unsafe (c, एतत्, &record->values[0], गन्ती, closure->stride) &&
                   closure->मान_Formats[1].sanitize_मानहरू_stride_unsafe (c, एतत्, &record->values[closure->len1], गन्ती, closure->stride)));
  }

  तार्किक intersects (अचर hb_set_t *glyphs,
                   अचर मान_Format *मान_Formats) अचर
  {
    अचिन्हित record_size = get_size (मान_Formats);

    अचर Pair_मान_Record *record = &first_Pair_मान_Record;
    अचिन्हित पूर्णाङ्क गन्ती = len;
    लागि (अचिन्हित पूर्णाङ्क i = 0; i < गन्ती; i++)
    {
      यदि (glyphs->has (record->secondGlyph))
        फर्काऊ सत्य;
      record = &StructAtOffset<अचर Pair_मान_Record> (record, record_size);
    }
    फर्काऊ असत्य;
  }

  शून्य collect_glyphs (hb_collect_glyphs_context_t *c,
                       अचर मान_Format *मान_Formats) अचर
  {
    अचिन्हित record_size = get_size (मान_Formats);

    अचर Pair_मान_Record *record = &first_Pair_मान_Record;
    c->आगत->add_array (&record->secondGlyph, len, record_size);
  }

  ढाँचा <typename set_t>
  शून्य collect_second_glyphs (set_t *glyphs,
			      अचर मान_Format *मान_Formats) अचर
  {
    अचिन्हित record_size = get_size (मान_Formats);
    अचर Pair_मान_Record *record = &first_Pair_मान_Record;
    glyphs->add_array (&record->secondGlyph, len, record_size);
  }

  pair_set_मानहरू_t get_मानहरू_view (अचर मान_Format मान_Formats[2]) अचर
  {
    फर्काऊ {एतत्, &first_Pair_मान_Record.secondGlyph, &first_Pair_मान_Record.values[0],
            (अचिन्हित) len, (अचिन्हित) get_size (मान_Formats)};
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  अचर मान_Format *मान_Formats) अचर
  {
    get_मानहरू_view (मान_Formats).collect_variation_indices (c, मान_Formats);
  }

  तार्किक apply (hb_ot_apply_context_t *c,
              अचर मान_Format *मान_Formats,
              अचिन्हित पूर्णाङ्क pos) अचर
  {
    TRACE_APPLY (एतत्);
    hb_buffer_t *buffer = c->buffer;
    अचिन्हित पूर्णाङ्क len1 = मान_Formats[0].get_len ();
    अचिन्हित पूर्णाङ्क len2 = मान_Formats[1].get_len ();
    अचिन्हित record_size = get_size (len1, len2);

    अचर Pair_मान_Record *record = hb_bsearch (buffer->info[pos].codepoint,
                                                &first_Pair_मान_Record,
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

      तार्किक applied_first = len1 && मान_Formats[0].apply_मान (c, एतत्, &record->values[0], buffer->cur_pos());
      तार्किक applied_second = len2 && मान_Formats[1].apply_मान (c, एतत्, &record->values[len1], buffer->pos[pos]);

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
               अचर मान_Format मान_Formats[2],
               अचर मान_Format newFormats[2]) अचर
  {
    TRACE_SUBSET (एतत्);
    स्वतः snap = c->serializer->snapshot ();

    स्वतः *out = c->serializer->start_embed (*एतत्);
    यदि (unlikely (!c->serializer->extend_min (out))) return_trace (असत्य);
    out->len = 0;

    अचर hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    अचर hb_map_t &glyph_map = *c->plan->glyph_map;

    अचिन्हित len1 = मान_Formats[0].get_len ();
    अचिन्हित len2 = मान_Formats[1].get_len ();
    अचिन्हित record_size = get_size (len1, len2);

    pair_मान_record_context_t context =
    {
      एतत्,
      मान_Formats,
      newFormats,
      len1,
      &glyph_map,
      &c->plan->layout_variation_idx_delta_map
    };

    अचर Pair_मान_Record *record = &first_Pair_मान_Record;
    अचिन्हित गन्ती = len, अङ्क = 0;
    लागि (अचिन्हित i = 0; i < गन्ती; i++)
    {
      यदि (glyphset.has (record->secondGlyph)
         && record->subset (c, &context)) अङ्क++;
      record = &StructAtOffset<अचर Pair_मान_Record> (record, record_size);
    }

    out->len = अङ्क;
    यदि (!अङ्क) c->serializer->revert (snap);
    return_trace (अङ्क);
  }
};


}
}
}

#endif  // OT_LAYOUT_GPOS_PAIRSET_HH
