#ifndef OT_LAYOUT_GPOS_PAIRSET_HH
#define OT_LAYOUT_GPOS_PAIRSET_HH

#અન્તર્ભાવ "PairValueRecord.hh"

નામાકાશ OT {
નામાકાશ Layout {
નામાકાશ GPOS_impl {


/* Pair records vary in glyph width; their value traversal need not. */
સંરચના pair_set_કિંમતો_t
{
  અચલ કિંમત_Base *base;
  glyph_data_t glyphs;
  અચલ કિંમત *values;
  unsigned ગણતરી;
  unsigned record_size;

  unsigned get_glyph (unsigned i) અચલ
  {
    glyph_data_t glyph = glyphs;
    glyph.data = (અચલ અક્ષર *) glyph.data + i * record_size;
    પાછા glyph[0];
  }

  અચલ કિંમત *get_કિંમતો (unsigned i) અચલ
  { પાછા reinterpret_cast<અચલ કિંમત *> ((અચલ અક્ષર *) values + i * record_size); }

  hb_pair_t<unsigned, unsigned>
  compute_effective_કિંમત_formats (અચલ hb_set_t &glyphset,
                                   અચલ કિંમત_Format કિંમત_Formats[2],
                                   તાર્કિક strip_hints, તાર્કિક strip_empty,
                                   અચલ hb_hashmap_t<unsigned, hb_pair_t<unsigned, પૂર્ણાંક>> *varidx_delta_map) અચલ
  {
    unsigned len1 = કિંમત_Formats[0].get_len ();
    unsigned format1 = 0, format2 = 0;
    માટે (unsigned i = 0; i < ગણતરી; i++)
    {
      જો (!glyphset.has (get_glyph (i))) ચાલુ;
      અચલ કિંમત *v = get_કિંમતો (i);
      format1 |= કિંમત_Formats[0].get_effective_format (v, strip_hints, strip_empty, base, varidx_delta_map);
      format2 |= કિંમત_Formats[1].get_effective_format (v + len1, strip_hints, strip_empty, base, varidx_delta_map);
    }
    પાછા hb_pair (format1, format2);
  }

  શૂન્ય collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  અચલ કિંમત_Format કિંમત_Formats[2]) અચલ
  {
    unsigned len1 = કિંમત_Formats[0].get_len ();
    unsigned len2 = કિંમત_Formats[1].get_len ();
    માટે (unsigned i = 0; i < ગણતરી; i++)
    {
      જો (!c->glyph_set->has (get_glyph (i))) ચાલુ;
      અચલ hb_array_t<અચલ કિંમત> v = hb_array (get_કિંમતો (i), len1 + len2);
      જો (કિંમત_Formats[0].has_device ())
        કિંમત_Formats[0].collect_variation_indices (c, base, v.sub_array (0, len1));
      જો (કિંમત_Formats[1].has_device ())
        કિંમત_Formats[1].collect_variation_indices (c, base, v.sub_array (len1, len2));
    }
  }
};

template <typename Types>
સંરચના PairSet : કિંમત_Base
{
  template <typename Types2>
  friend સંરચના PairPosFormat1_3;

  using Pair_કિંમત_Record = GPOS_impl::Pair_કિંમત_Record<Types>;

  સુરક્ષિત:
  typename Types::HBUINT
                        len;    /* Number of PairValueRecords */
  Pair_કિંમત_Record       first_Pair_કિંમત_Record;
                                /* Array of PairValueRecords--ordered
                                 * by GlyphID of the second glyph */
  જાહેર:
  DEFINE_SIZE_MIN (Types::HBUINT::static_size);

  સ્થિર size_t get_size (unsigned len1, unsigned len2)
  {
    પાછા Types::HBGlyphID::static_size + કિંમત::static_size * (len1 + len2);
  }
  સ્થિર size_t get_size (અચલ કિંમત_Format કિંમત_Formats[2])
  {
    unsigned len1 = કિંમત_Formats[0].get_len ();
    unsigned len2 = કિંમત_Formats[1].get_len ();
    પાછા get_size (len1, len2);
  }

  સંરચના sanitize_closure_t
  {
    અચલ કિંમત_Format *કિંમત_Formats;
    unsigned પૂર્ણાંક len1; /* valueFormats[0].get_len() */
    size_t stride; /* bytes */
  };

  તાર્કિક sanitize (hb_sanitize_context_t *c, અચલ sanitize_closure_t *closure) અચલ
  {
    TRACE_SANITIZE (આ);
    જો (!(c->check_struct (આ) &&
	  hb_barrier () &&
          c->check_range (&first_Pair_કિંમત_Record,
                          len,
                          closure->stride))) return_trace (ખોટું);
    hb_barrier ();

    unsigned પૂર્ણાંક ગણતરી = len;
    અચલ Pair_કિંમત_Record *record = &first_Pair_કિંમત_Record;
    return_trace (c->lazy_some_gpos ||
		  (closure->કિંમત_Formats[0].sanitize_કિંમતો_stride_unsafe (c, આ, &record->values[0], ગણતરી, closure->stride) &&
                   closure->કિંમત_Formats[1].sanitize_કિંમતો_stride_unsafe (c, આ, &record->values[closure->len1], ગણતરી, closure->stride)));
  }

  તાર્કિક intersects (અચલ hb_set_t *glyphs,
                   અચલ કિંમત_Format *કિંમત_Formats) અચલ
  {
    unsigned record_size = get_size (કિંમત_Formats);

    અચલ Pair_કિંમત_Record *record = &first_Pair_કિંમત_Record;
    unsigned પૂર્ણાંક ગણતરી = len;
    માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
    {
      જો (glyphs->has (record->secondGlyph))
        પાછા સાચું;
      record = &StructAtOffset<અચલ Pair_કિંમત_Record> (record, record_size);
    }
    પાછા ખોટું;
  }

  શૂન્ય collect_glyphs (hb_collect_glyphs_context_t *c,
                       અચલ કિંમત_Format *કિંમત_Formats) અચલ
  {
    unsigned record_size = get_size (કિંમત_Formats);

    અચલ Pair_કિંમત_Record *record = &first_Pair_કિંમત_Record;
    c->આગત->add_array (&record->secondGlyph, len, record_size);
  }

  template <typename set_t>
  શૂન્ય collect_second_glyphs (set_t *glyphs,
			      અચલ કિંમત_Format *કિંમત_Formats) અચલ
  {
    unsigned record_size = get_size (કિંમત_Formats);
    અચલ Pair_કિંમત_Record *record = &first_Pair_કિંમત_Record;
    glyphs->add_array (&record->secondGlyph, len, record_size);
  }

  pair_set_કિંમતો_t get_કિંમતો_view (અચલ કિંમત_Format કિંમત_Formats[2]) અચલ
  {
    પાછા {આ, &first_Pair_કિંમત_Record.secondGlyph, &first_Pair_કિંમત_Record.values[0],
            (unsigned) len, (unsigned) get_size (કિંમત_Formats)};
  }

  શૂન્ય collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  અચલ કિંમત_Format *કિંમત_Formats) અચલ
  {
    get_કિંમતો_view (કિંમત_Formats).collect_variation_indices (c, કિંમત_Formats);
  }

  તાર્કિક apply (hb_ot_apply_context_t *c,
              અચલ કિંમત_Format *કિંમત_Formats,
              unsigned પૂર્ણાંક pos) અચલ
  {
    TRACE_APPLY (આ);
    hb_buffer_t *buffer = c->buffer;
    unsigned પૂર્ણાંક len1 = કિંમત_Formats[0].get_len ();
    unsigned પૂર્ણાંક len2 = કિંમત_Formats[1].get_len ();
    unsigned record_size = get_size (len1, len2);

    અચલ Pair_કિંમત_Record *record = hb_bsearch (buffer->info[pos].codepoint,
                                                &first_Pair_કિંમત_Record,
                                                (unsigned) len,
                                                record_size);
    જો (record)
    {
      જો (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "try kerning glyphs at %u,%u",
			    c->buffer->idx, pos);
      }

      તાર્કિક applied_first = len1 && કિંમત_Formats[0].apply_કિંમત (c, આ, &record->values[0], buffer->cur_pos());
      તાર્કિક applied_second = len2 && કિંમત_Formats[1].apply_કિંમત (c, આ, &record->values[len1], buffer->pos[pos]);

      જો (applied_first || applied_second)
	જો (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
	{
	  c->buffer->message (c->font,
			      "kerned glyphs at %u,%u",
			      c->buffer->idx, pos);
	}

      જો (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "tried kerning glyphs at %u,%u",
			    c->buffer->idx, pos);
      }

      જો (applied_first || applied_second)
        buffer->unsafe_to_break (buffer->idx, pos + 1);
      નહીંતર
        /* Even a zero-valued pair record is a concat hazard. */
        buffer->unsafe_to_concat (buffer->idx, pos + 1);

      જો (len2)
      {
	pos++;
      // https://github.com/harfbuzz/harfbuzz/issues/3824
      // https://github.com/harfbuzz/harfbuzz/issues/3888#issuecomment-1326781116
      buffer->unsafe_to_break (buffer->idx, pos + 1);
      }

      buffer->idx = pos;
      return_trace (સાચું);
    }
    buffer->unsafe_to_concat (buffer->idx, pos + 1);
    return_trace (ખોટું);
  }

  તાર્કિક subset (hb_subset_context_t *c,
               અચલ કિંમત_Format કિંમત_Formats[2],
               અચલ કિંમત_Format newFormats[2]) અચલ
  {
    TRACE_SUBSET (આ);
    auto snap = c->serializer->snapshot ();

    auto *out = c->serializer->start_embed (*આ);
    જો (unlikely (!c->serializer->extend_min (out))) return_trace (ખોટું);
    out->len = 0;

    અચલ hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    અચલ hb_map_t &glyph_map = *c->plan->glyph_map;

    unsigned len1 = કિંમત_Formats[0].get_len ();
    unsigned len2 = કિંમત_Formats[1].get_len ();
    unsigned record_size = get_size (len1, len2);

    pair_કિંમત_record_context_t context =
    {
      આ,
      કિંમત_Formats,
      newFormats,
      len1,
      &glyph_map,
      &c->plan->layout_variation_idx_delta_map
    };

    અચલ Pair_કિંમત_Record *record = &first_Pair_કિંમત_Record;
    unsigned ગણતરી = len, અંક = 0;
    માટે (unsigned i = 0; i < ગણતરી; i++)
    {
      જો (glyphset.has (record->secondGlyph)
         && record->subset (c, &context)) અંક++;
      record = &StructAtOffset<અચલ Pair_કિંમત_Record> (record, record_size);
    }

    out->len = અંક;
    જો (!અંક) c->serializer->revert (snap);
    return_trace (અંક);
  }
};


}
}
}

#endif  // OT_LAYOUT_GPOS_PAIRSET_HH
