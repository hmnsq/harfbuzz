#ifndef OT_LAYOUT_GPOS_PAIRSET_HH
#define OT_LAYOUT_GPOS_PAIRSET_HH

#അന്തര്ഭാവ "PairValueRecord.hh"

നാമാകാശ OT {
നാമാകാശ Layout {
നാമാകാശ GPOS_impl {


/* Pair records vary in glyph width; their value traversal need not. */
ഘടന pair_set_മൂല്യങ്ങൾ_t
{
  സ്ഥിരം മൂല്യം_Base *base;
  glyph_data_t glyphs;
  സ്ഥിരം മൂല്യം *values;
  unsigned എണ്ണം;
  unsigned record_size;

  unsigned get_glyph (unsigned i) സ്ഥിരം
  {
    glyph_data_t glyph = glyphs;
    glyph.data = (സ്ഥിരം അക്ഷരം *) glyph.data + i * record_size;
    തിരികെ glyph[0];
  }

  സ്ഥിരം മൂല്യം *get_മൂല്യങ്ങൾ (unsigned i) സ്ഥിരം
  { തിരികെ reinterpret_cast<സ്ഥിരം മൂല്യം *> ((സ്ഥിരം അക്ഷരം *) values + i * record_size); }

  hb_pair_t<unsigned, unsigned>
  compute_effective_മൂല്യം_formats (സ്ഥിരം hb_set_t &glyphset,
                                   സ്ഥിരം മൂല്യം_Format മൂല്യം_Formats[2],
                                   തർക്കം strip_hints, തർക്കം strip_empty,
                                   സ്ഥിരം hb_hashmap_t<unsigned, hb_pair_t<unsigned, പൂർണ്ണസംഖ്യ>> *varidx_delta_map) സ്ഥിരം
  {
    unsigned len1 = മൂല്യം_Formats[0].get_len ();
    unsigned format1 = 0, format2 = 0;
    വേണ്ടി (unsigned i = 0; i < എണ്ണം; i++)
    {
      എങ്കിൽ (!glyphset.has (get_glyph (i))) തുടരുക;
      സ്ഥിരം മൂല്യം *v = get_മൂല്യങ്ങൾ (i);
      format1 |= മൂല്യം_Formats[0].get_effective_format (v, strip_hints, strip_empty, base, varidx_delta_map);
      format2 |= മൂല്യം_Formats[1].get_effective_format (v + len1, strip_hints, strip_empty, base, varidx_delta_map);
    }
    തിരികെ hb_pair (format1, format2);
  }

  ശൂന്യം collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  സ്ഥിരം മൂല്യം_Format മൂല്യം_Formats[2]) സ്ഥിരം
  {
    unsigned len1 = മൂല്യം_Formats[0].get_len ();
    unsigned len2 = മൂല്യം_Formats[1].get_len ();
    വേണ്ടി (unsigned i = 0; i < എണ്ണം; i++)
    {
      എങ്കിൽ (!c->glyph_set->has (get_glyph (i))) തുടരുക;
      സ്ഥിരം hb_array_t<സ്ഥിരം മൂല്യം> v = hb_array (get_മൂല്യങ്ങൾ (i), len1 + len2);
      എങ്കിൽ (മൂല്യം_Formats[0].has_device ())
        മൂല്യം_Formats[0].collect_variation_indices (c, base, v.sub_array (0, len1));
      എങ്കിൽ (മൂല്യം_Formats[1].has_device ())
        മൂല്യം_Formats[1].collect_variation_indices (c, base, v.sub_array (len1, len2));
    }
  }
};

template <typename Types>
ഘടന PairSet : മൂല്യം_Base
{
  template <typename Types2>
  friend ഘടന PairPosFormat1_3;

  using Pair_മൂല്യം_Record = GPOS_impl::Pair_മൂല്യം_Record<Types>;

  സംരക്ഷിത:
  typename Types::HBUINT
                        len;    /* Number of PairValueRecords */
  Pair_മൂല്യം_Record       first_Pair_മൂല്യം_Record;
                                /* Array of PairValueRecords--ordered
                                 * by GlyphID of the second glyph */
  പൊതു:
  DEFINE_SIZE_MIN (Types::HBUINT::static_size);

  സ്ഥിതം size_t get_size (unsigned len1, unsigned len2)
  {
    തിരികെ Types::HBGlyphID::static_size + മൂല്യം::static_size * (len1 + len2);
  }
  സ്ഥിതം size_t get_size (സ്ഥിരം മൂല്യം_Format മൂല്യം_Formats[2])
  {
    unsigned len1 = മൂല്യം_Formats[0].get_len ();
    unsigned len2 = മൂല്യം_Formats[1].get_len ();
    തിരികെ get_size (len1, len2);
  }

  ഘടന sanitize_closure_t
  {
    സ്ഥിരം മൂല്യം_Format *മൂല്യം_Formats;
    unsigned പൂർണ്ണസംഖ്യ len1; /* valueFormats[0].get_len() */
    size_t stride; /* bytes */
  };

  തർക്കം sanitize (hb_sanitize_context_t *c, സ്ഥിരം sanitize_closure_t *closure) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    എങ്കിൽ (!(c->check_struct (ഇത്) &&
	  hb_barrier () &&
          c->check_range (&first_Pair_മൂല്യം_Record,
                          len,
                          closure->stride))) return_trace (അസത്യം);
    hb_barrier ();

    unsigned പൂർണ്ണസംഖ്യ എണ്ണം = len;
    സ്ഥിരം Pair_മൂല്യം_Record *record = &first_Pair_മൂല്യം_Record;
    return_trace (c->lazy_some_gpos ||
		  (closure->മൂല്യം_Formats[0].sanitize_മൂല്യങ്ങൾ_stride_unsafe (c, ഇത്, &record->values[0], എണ്ണം, closure->stride) &&
                   closure->മൂല്യം_Formats[1].sanitize_മൂല്യങ്ങൾ_stride_unsafe (c, ഇത്, &record->values[closure->len1], എണ്ണം, closure->stride)));
  }

  തർക്കം intersects (സ്ഥിരം hb_set_t *glyphs,
                   സ്ഥിരം മൂല്യം_Format *മൂല്യം_Formats) സ്ഥിരം
  {
    unsigned record_size = get_size (മൂല്യം_Formats);

    സ്ഥിരം Pair_മൂല്യം_Record *record = &first_Pair_മൂല്യം_Record;
    unsigned പൂർണ്ണസംഖ്യ എണ്ണം = len;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
    {
      എങ്കിൽ (glyphs->has (record->secondGlyph))
        തിരികെ സത്യം;
      record = &StructAtOffset<സ്ഥിരം Pair_മൂല്യം_Record> (record, record_size);
    }
    തിരികെ അസത്യം;
  }

  ശൂന്യം collect_glyphs (hb_collect_glyphs_context_t *c,
                       സ്ഥിരം മൂല്യം_Format *മൂല്യം_Formats) സ്ഥിരം
  {
    unsigned record_size = get_size (മൂല്യം_Formats);

    സ്ഥിരം Pair_മൂല്യം_Record *record = &first_Pair_മൂല്യം_Record;
    c->നിവേശം->add_array (&record->secondGlyph, len, record_size);
  }

  template <typename set_t>
  ശൂന്യം collect_second_glyphs (set_t *glyphs,
			      സ്ഥിരം മൂല്യം_Format *മൂല്യം_Formats) സ്ഥിരം
  {
    unsigned record_size = get_size (മൂല്യം_Formats);
    സ്ഥിരം Pair_മൂല്യം_Record *record = &first_Pair_മൂല്യം_Record;
    glyphs->add_array (&record->secondGlyph, len, record_size);
  }

  pair_set_മൂല്യങ്ങൾ_t get_മൂല്യങ്ങൾ_view (സ്ഥിരം മൂല്യം_Format മൂല്യം_Formats[2]) സ്ഥിരം
  {
    തിരികെ {ഇത്, &first_Pair_മൂല്യം_Record.secondGlyph, &first_Pair_മൂല്യം_Record.values[0],
            (unsigned) len, (unsigned) get_size (മൂല്യം_Formats)};
  }

  ശൂന്യം collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  സ്ഥിരം മൂല്യം_Format *മൂല്യം_Formats) സ്ഥിരം
  {
    get_മൂല്യങ്ങൾ_view (മൂല്യം_Formats).collect_variation_indices (c, മൂല്യം_Formats);
  }

  തർക്കം apply (hb_ot_apply_context_t *c,
              സ്ഥിരം മൂല്യം_Format *മൂല്യം_Formats,
              unsigned പൂർണ്ണസംഖ്യ pos) സ്ഥിരം
  {
    TRACE_APPLY (ഇത്);
    hb_buffer_t *buffer = c->buffer;
    unsigned പൂർണ്ണസംഖ്യ len1 = മൂല്യം_Formats[0].get_len ();
    unsigned പൂർണ്ണസംഖ്യ len2 = മൂല്യം_Formats[1].get_len ();
    unsigned record_size = get_size (len1, len2);

    സ്ഥിരം Pair_മൂല്യം_Record *record = hb_bsearch (buffer->info[pos].codepoint,
                                                &first_Pair_മൂല്യം_Record,
                                                (unsigned) len,
                                                record_size);
    എങ്കിൽ (record)
    {
      എങ്കിൽ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "try kerning glyphs at %u,%u",
			    c->buffer->idx, pos);
      }

      തർക്കം applied_first = len1 && മൂല്യം_Formats[0].apply_മൂല്യം (c, ഇത്, &record->values[0], buffer->cur_pos());
      തർക്കം applied_second = len2 && മൂല്യം_Formats[1].apply_മൂല്യം (c, ഇത്, &record->values[len1], buffer->pos[pos]);

      എങ്കിൽ (applied_first || applied_second)
	എങ്കിൽ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
	{
	  c->buffer->message (c->font,
			      "kerned glyphs at %u,%u",
			      c->buffer->idx, pos);
	}

      എങ്കിൽ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "tried kerning glyphs at %u,%u",
			    c->buffer->idx, pos);
      }

      എങ്കിൽ (applied_first || applied_second)
        buffer->unsafe_to_break (buffer->idx, pos + 1);
      അല്ലെങ്കിൽ
        /* Even a zero-valued pair record is a concat hazard. */
        buffer->unsafe_to_concat (buffer->idx, pos + 1);

      എങ്കിൽ (len2)
      {
	pos++;
      // https://github.com/harfbuzz/harfbuzz/issues/3824
      // https://github.com/harfbuzz/harfbuzz/issues/3888#issuecomment-1326781116
      buffer->unsafe_to_break (buffer->idx, pos + 1);
      }

      buffer->idx = pos;
      return_trace (സത്യം);
    }
    buffer->unsafe_to_concat (buffer->idx, pos + 1);
    return_trace (അസത്യം);
  }

  തർക്കം subset (hb_subset_context_t *c,
               സ്ഥിരം മൂല്യം_Format മൂല്യം_Formats[2],
               സ്ഥിരം മൂല്യം_Format newFormats[2]) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    auto snap = c->serializer->snapshot ();

    auto *out = c->serializer->start_embed (*ഇത്);
    എങ്കിൽ (unlikely (!c->serializer->extend_min (out))) return_trace (അസത്യം);
    out->len = 0;

    സ്ഥിരം hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    സ്ഥിരം hb_map_t &glyph_map = *c->plan->glyph_map;

    unsigned len1 = മൂല്യം_Formats[0].get_len ();
    unsigned len2 = മൂല്യം_Formats[1].get_len ();
    unsigned record_size = get_size (len1, len2);

    pair_മൂല്യം_record_context_t context =
    {
      ഇത്,
      മൂല്യം_Formats,
      newFormats,
      len1,
      &glyph_map,
      &c->plan->layout_variation_idx_delta_map
    };

    സ്ഥിരം Pair_മൂല്യം_Record *record = &first_Pair_മൂല്യം_Record;
    unsigned എണ്ണം = len, അക്കം = 0;
    വേണ്ടി (unsigned i = 0; i < എണ്ണം; i++)
    {
      എങ്കിൽ (glyphset.has (record->secondGlyph)
         && record->subset (c, &context)) അക്കം++;
      record = &StructAtOffset<സ്ഥിരം Pair_മൂല്യം_Record> (record, record_size);
    }

    out->len = അക്കം;
    എങ്കിൽ (!അക്കം) c->serializer->revert (snap);
    return_trace (അക്കം);
  }
};


}
}
}

#endif  // OT_LAYOUT_GPOS_PAIRSET_HH
