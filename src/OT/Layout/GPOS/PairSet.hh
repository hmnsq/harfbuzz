#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OT_LAYOUT_GPOS_PAIRSET_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_LAYOUT_GPOS_PAIRSET_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "PairValueRecord.hh"

ਨਾਮਾਕਾਸ਼ OT {
ਨਾਮਾਕਾਸ਼ Layout {
ਨਾਮਾਕਾਸ਼ GPOS_impl {


/* Pair records vary in glyph width; their value traversal need not. */
ਬਣਤਰ pair_ਸੈੱਟ_ਮੁੱਲਾਂ_t
{
  ਅਚਲ ਮੁੱਲ_ਅਧਾਰ *ਅਧਾਰ;
  glyph_ਅੰਕੜੇ_t glyphs;
  ਅਚਲ ਮੁੱਲ *values;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਰਿਕਾਰਡ_ਮਾਪ;

  ਬਿਨਾਨਿਸ਼ਾਨੀ get_glyph (ਬਿਨਾਨਿਸ਼ਾਨੀ i) ਅਚਲ
  {
    glyph_ਅੰਕੜੇ_t glyph = glyphs;
    glyph.ਅੰਕੜੇ = (ਅਚਲ ਅੱਖਰ *) glyph.ਅੰਕੜੇ + i * ਰਿਕਾਰਡ_ਮਾਪ;
    ਵਾਪਸ glyph[0];
  }

  ਅਚਲ ਮੁੱਲ *get_ਮੁੱਲਾਂ (ਬਿਨਾਨਿਸ਼ਾਨੀ i) ਅਚਲ
  { ਵਾਪਸ reinterpret_cast<ਅਚਲ ਮੁੱਲ *> ((ਅਚਲ ਅੱਖਰ *) values + i * ਰਿਕਾਰਡ_ਮਾਪ); }

  hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਬਿਨਾਨਿਸ਼ਾਨੀ>
  compute_effective_ਮੁੱਲ_formats (ਅਚਲ hb_ਸੈੱਟ_t &glyphset,
                                   ਅਚਲ ਮੁੱਲ_Format ਮੁੱਲ_Formats[2],
                                   ਤਾਰਕਿਕ strip_hints, ਤਾਰਕਿਕ strip_ਖਾਲੀ_ਥਾਵਾਂ,
                                   ਅਚਲ hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਪੂਰਨ>> *varidx_delta_map) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ len1 = ਮੁੱਲ_Formats[0].get_len ();
    ਬਿਨਾਨਿਸ਼ਾਨੀ format1 = 0, format2 = 0;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      ਜੇ (!glyphset.has (get_glyph (i))) ਜਾਰੀ;
      ਅਚਲ ਮੁੱਲ *v = get_ਮੁੱਲਾਂ (i);
      format1 |= ਮੁੱਲ_Formats[0].get_effective_format (v, strip_hints, strip_ਖਾਲੀ_ਥਾਵਾਂ, ਅਧਾਰ, varidx_delta_map);
      format2 |= ਮੁੱਲ_Formats[1].get_effective_format (v + len1, strip_hints, strip_ਖਾਲੀ_ਥਾਵਾਂ, ਅਧਾਰ, varidx_delta_map);
    }
    ਵਾਪਸ hb_pair (format1, format2);
  }

  ਖਾਲੀ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ਅਚਲ ਮੁੱਲ_Format ਮੁੱਲ_Formats[2]) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ len1 = ਮੁੱਲ_Formats[0].get_len ();
    ਬਿਨਾਨਿਸ਼ਾਨੀ len2 = ਮੁੱਲ_Formats[1].get_len ();
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      ਜੇ (!c->glyph_ਸੈੱਟ->has (get_glyph (i))) ਜਾਰੀ;
      ਅਚਲ hb_array_t<ਅਚਲ ਮੁੱਲ> v = hb_array (get_ਮੁੱਲਾਂ (i), len1 + len2);
      ਜੇ (ਮੁੱਲ_Formats[0].has_device ())
        ਮੁੱਲ_Formats[0].collect_variation_indices (c, ਅਧਾਰ, v.sub_array (0, len1));
      ਜੇ (ਮੁੱਲ_Formats[1].has_device ())
        ਮੁੱਲ_Formats[1].collect_variation_indices (c, ਅਧਾਰ, v.sub_array (len1, len2));
    }
  }
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types>
ਬਣਤਰ Pair_ਸੈੱਟ : ਮੁੱਲ_ਅਧਾਰ
{
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types2>
  ਮਿੱਤਰ ਬਣਤਰ PairPosFormat1_3;

  ਵਰਤੋਂ Pair_ਮੁੱਲ_ਰਿਕਾਰਡ = GPOS_impl::Pair_ਮੁੱਲ_ਰਿਕਾਰਡ<Types>;

  ਸੁਰੱਖਿਅਤ:
  ਕਿਸਮ_ਨਾਂ Types::HBUINT
                        len;    /* Number of PairValueRecords */
  Pair_ਮੁੱਲ_ਰਿਕਾਰਡ       ਪਹਿਲਾ_Pair_ਮੁੱਲ_ਰਿਕਾਰਡ;
                                /* Array of PairValueRecords--ordered
                                 * by GlyphID of the second glyph */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_MIN (Types::HBUINT::static_ਮਾਪ);

  ਸਥਿਰ size_t get_ਮਾਪ (ਬਿਨਾਨਿਸ਼ਾਨੀ len1, ਬਿਨਾਨਿਸ਼ਾਨੀ len2)
  {
    ਵਾਪਸ Types::HBGlyphID::static_ਮਾਪ + ਮੁੱਲ::static_ਮਾਪ * (len1 + len2);
  }
  ਸਥਿਰ size_t get_ਮਾਪ (ਅਚਲ ਮੁੱਲ_Format ਮੁੱਲ_Formats[2])
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ len1 = ਮੁੱਲ_Formats[0].get_len ();
    ਬਿਨਾਨਿਸ਼ਾਨੀ len2 = ਮੁੱਲ_Formats[1].get_len ();
    ਵਾਪਸ get_ਮਾਪ (len1, len2);
  }

  ਬਣਤਰ sanitize_ਘੇਰਾ_t
  {
    ਅਚਲ ਮੁੱਲ_Format *ਮੁੱਲ_Formats;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ len1; /* valueFormats[0].get_len() */
    size_t stride; /*ਬਾਈਟਾਂ*/
  };

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c, ਅਚਲ sanitize_ਘੇਰਾ_t *ਘੇਰਾ) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    ਜੇ (!(c->check_struct (ਇਹ) &&
	  hb_barrier () &&
          c->check_range (&ਪਹਿਲਾ_Pair_ਮੁੱਲ_ਰਿਕਾਰਡ,
                          len,
                          ਘੇਰਾ->stride))) return_trace (ਝੂਠ);
    hb_barrier ();

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = len;
    ਅਚਲ Pair_ਮੁੱਲ_ਰਿਕਾਰਡ *ਰਿਕਾਰਡ = &ਪਹਿਲਾ_Pair_ਮੁੱਲ_ਰਿਕਾਰਡ;
    return_trace (c->lazy_some_gpos ||
		  (ਘੇਰਾ->ਮੁੱਲ_Formats[0].sanitize_ਮੁੱਲਾਂ_stride_unsafe (c, ਇਹ, &ਰਿਕਾਰਡ->values[0], ਗਿਣਤੀ_2, ਘੇਰਾ->stride) &&
                   ਘੇਰਾ->ਮੁੱਲ_Formats[1].sanitize_ਮੁੱਲਾਂ_stride_unsafe (c, ਇਹ, &ਰਿਕਾਰਡ->values[ਘੇਰਾ->len1], ਗਿਣਤੀ_2, ਘੇਰਾ->stride)));
  }

  ਤਾਰਕਿਕ intersects (ਅਚਲ hb_ਸੈੱਟ_t *glyphs,
                   ਅਚਲ ਮੁੱਲ_Format *ਮੁੱਲ_Formats) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਰਿਕਾਰਡ_ਮਾਪ = get_ਮਾਪ (ਮੁੱਲ_Formats);

    ਅਚਲ Pair_ਮੁੱਲ_ਰਿਕਾਰਡ *ਰਿਕਾਰਡ = &ਪਹਿਲਾ_Pair_ਮੁੱਲ_ਰਿਕਾਰਡ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = len;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      ਜੇ (glyphs->has (ਰਿਕਾਰਡ->ਦੂਜਾ_Glyph))
        ਵਾਪਸ ਸੱਚ;
      ਰਿਕਾਰਡ = &Struct_At_ਵਿੱਥ<ਅਚਲ Pair_ਮੁੱਲ_ਰਿਕਾਰਡ> (ਰਿਕਾਰਡ, ਰਿਕਾਰਡ_ਮਾਪ);
    }
    ਵਾਪਸ ਝੂਠ;
  }

  ਖਾਲੀ collect_glyphs (hb_collect_glyphs_context_t *c,
                       ਅਚਲ ਮੁੱਲ_Format *ਮੁੱਲ_Formats) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਰਿਕਾਰਡ_ਮਾਪ = get_ਮਾਪ (ਮੁੱਲ_Formats);

    ਅਚਲ Pair_ਮੁੱਲ_ਰਿਕਾਰਡ *ਰਿਕਾਰਡ = &ਪਹਿਲਾ_Pair_ਮੁੱਲ_ਰਿਕਾਰਡ;
    c->ਨਿਵੇਸ਼->ਜੋੜੋ_array (&ਰਿਕਾਰਡ->ਦੂਜਾ_Glyph, len, ਰਿਕਾਰਡ_ਮਾਪ);
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਸੈੱਟ_t>
  ਖਾਲੀ collect_ਦੂਜਾ_glyphs (ਸੈੱਟ_t *glyphs,
			      ਅਚਲ ਮੁੱਲ_Format *ਮੁੱਲ_Formats) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਰਿਕਾਰਡ_ਮਾਪ = get_ਮਾਪ (ਮੁੱਲ_Formats);
    ਅਚਲ Pair_ਮੁੱਲ_ਰਿਕਾਰਡ *ਰਿਕਾਰਡ = &ਪਹਿਲਾ_Pair_ਮੁੱਲ_ਰਿਕਾਰਡ;
    glyphs->ਜੋੜੋ_array (&ਰਿਕਾਰਡ->ਦੂਜਾ_Glyph, len, ਰਿਕਾਰਡ_ਮਾਪ);
  }

  pair_ਸੈੱਟ_ਮੁੱਲਾਂ_t get_ਮੁੱਲਾਂ_view (ਅਚਲ ਮੁੱਲ_Format ਮੁੱਲ_Formats[2]) ਅਚਲ
  {
    ਵਾਪਸ {ਇਹ, &ਪਹਿਲਾ_Pair_ਮੁੱਲ_ਰਿਕਾਰਡ.ਦੂਜਾ_Glyph, &ਪਹਿਲਾ_Pair_ਮੁੱਲ_ਰਿਕਾਰਡ.values[0],
            (ਬਿਨਾਨਿਸ਼ਾਨੀ) len, (ਬਿਨਾਨਿਸ਼ਾਨੀ) get_ਮਾਪ (ਮੁੱਲ_Formats)};
  }

  ਖਾਲੀ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ਅਚਲ ਮੁੱਲ_Format *ਮੁੱਲ_Formats) ਅਚਲ
  {
    get_ਮੁੱਲਾਂ_view (ਮੁੱਲ_Formats).collect_variation_indices (c, ਮੁੱਲ_Formats);
  }

  ਤਾਰਕਿਕ apply (hb_ot_apply_context_t *c,
              ਅਚਲ ਮੁੱਲ_Format *ਮੁੱਲ_Formats,
              ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ pos) ਅਚਲ
  {
    TRACE_APPLY (ਇਹ);
    hb_ਬਫਰ_t *ਬਫਰ = c->ਬਫਰ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ len1 = ਮੁੱਲ_Formats[0].get_len ();
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ len2 = ਮੁੱਲ_Formats[1].get_len ();
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਰਿਕਾਰਡ_ਮਾਪ = get_ਮਾਪ (len1, len2);

    ਅਚਲ Pair_ਮੁੱਲ_ਰਿਕਾਰਡ *ਰਿਕਾਰਡ = hb_bsearch (ਬਫਰ->info[pos].codepoint,
                                                &ਪਹਿਲਾ_Pair_ਮੁੱਲ_ਰਿਕਾਰਡ,
                                                (ਬਿਨਾਨਿਸ਼ਾਨੀ) len,
                                                ਰਿਕਾਰਡ_ਮਾਪ);
    ਜੇ (ਰਿਕਾਰਡ)
    {
      ਜੇ (HB_ਬਫਰ_ਸੁਨੇਹਾ_ਹੋਰ && c->ਬਫਰ->messaging ())
      {
	c->ਬਫਰ->ਸੁਨੇਹਾ (c->font,
			    "try kerning glyphs at %u,%u",
			    c->ਬਫਰ->idx, pos);
      }

      ਤਾਰਕਿਕ applied_ਪਹਿਲਾ = len1 && ਮੁੱਲ_Formats[0].apply_ਮੁੱਲ (c, ਇਹ, &ਰਿਕਾਰਡ->values[0], ਬਫਰ->cur_pos());
      ਤਾਰਕਿਕ applied_ਦੂਜਾ = len2 && ਮੁੱਲ_Formats[1].apply_ਮੁੱਲ (c, ਇਹ, &ਰਿਕਾਰਡ->values[len1], ਬਫਰ->pos[pos]);

      ਜੇ (applied_ਪਹਿਲਾ || applied_ਦੂਜਾ)
	ਜੇ (HB_ਬਫਰ_ਸੁਨੇਹਾ_ਹੋਰ && c->ਬਫਰ->messaging ())
	{
	  c->ਬਫਰ->ਸੁਨੇਹਾ (c->font,
			      "kerned glyphs at %u,%u",
			      c->ਬਫਰ->idx, pos);
	}

      ਜੇ (HB_ਬਫਰ_ਸੁਨੇਹਾ_ਹੋਰ && c->ਬਫਰ->messaging ())
      {
	c->ਬਫਰ->ਸੁਨੇਹਾ (c->font,
			    "tried kerning glyphs at %u,%u",
			    c->ਬਫਰ->idx, pos);
      }

      ਜੇ (applied_ਪਹਿਲਾ || applied_ਦੂਜਾ)
        ਬਫਰ->unsafe_ਮੰਜ਼ਿਲ_break (ਬਫਰ->idx, pos + 1);
      ਨਹੀਂਤਾਂ
        /* Even a zero-valued pair record is a concat hazard. */
        ਬਫਰ->unsafe_ਮੰਜ਼ਿਲ_concat (ਬਫਰ->idx, pos + 1);

      ਜੇ (len2)
      {
	pos++;
      // https://github.com/harfbuzz/harfbuzz/issues/3824
      // https://github.com/harfbuzz/harfbuzz/issues/3888#issuecomment-1326781116
      ਬਫਰ->unsafe_ਮੰਜ਼ਿਲ_break (ਬਫਰ->idx, pos + 1);
      }

      ਬਫਰ->idx = pos;
      return_trace (ਸੱਚ);
    }
    ਬਫਰ->unsafe_ਮੰਜ਼ਿਲ_concat (ਬਫਰ->idx, pos + 1);
    return_trace (ਝੂਠ);
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c,
               ਅਚਲ ਮੁੱਲ_Format ਮੁੱਲ_Formats[2],
               ਅਚਲ ਮੁੱਲ_Format newFormats[2]) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਸਵੈਚਾਲਿਤ snap = c->serializer->snapshot ();

    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->ਸ਼ੁਰੂ_embed (*ਇਹ);
    ਜੇ (unlikely (!c->serializer->extend_min (ਬਾਹਰਲਾ))) return_trace (ਝੂਠ);
    ਬਾਹਰਲਾ->len = 0;

    ਅਚਲ hb_ਸੈੱਟ_t &glyphset = *c->plan->glyphset_gsub ();
    ਅਚਲ hb_map_t &glyph_map = *c->plan->glyph_map;

    ਬਿਨਾਨਿਸ਼ਾਨੀ len1 = ਮੁੱਲ_Formats[0].get_len ();
    ਬਿਨਾਨਿਸ਼ਾਨੀ len2 = ਮੁੱਲ_Formats[1].get_len ();
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਰਿਕਾਰਡ_ਮਾਪ = get_ਮਾਪ (len1, len2);

    pair_ਮੁੱਲ_ਰਿਕਾਰਡ_context_t context =
    {
      ਇਹ,
      ਮੁੱਲ_Formats,
      newFormats,
      len1,
      &glyph_map,
      &c->plan->layout_variation_idx_delta_map
    };

    ਅਚਲ Pair_ਮੁੱਲ_ਰਿਕਾਰਡ *ਰਿਕਾਰਡ = &ਪਹਿਲਾ_Pair_ਮੁੱਲ_ਰਿਕਾਰਡ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = len, ਅੰਕ = 0;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      ਜੇ (glyphset.has (ਰਿਕਾਰਡ->ਦੂਜਾ_Glyph)
         && ਰਿਕਾਰਡ->subset (c, &context)) ਅੰਕ++;
      ਰਿਕਾਰਡ = &Struct_At_ਵਿੱਥ<ਅਚਲ Pair_ਮੁੱਲ_ਰਿਕਾਰਡ> (ਰਿਕਾਰਡ, ਰਿਕਾਰਡ_ਮਾਪ);
    }

    ਬਾਹਰਲਾ->len = ਅੰਕ;
    ਜੇ (!ਅੰਕ) c->serializer->revert (snap);
    return_trace (ਅੰਕ);
  }
};


}
}
}

#ਜੇ_ਅੰਤ  // OT_LAYOUT_GPOS_PAIRSET_HH
