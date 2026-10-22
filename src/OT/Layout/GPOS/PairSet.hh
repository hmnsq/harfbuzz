#جے_بنا_تعریف OT_LAYOUT_GPOS_PAIRSET_HH
#تعریف_کرو OT_LAYOUT_GPOS_PAIRSET_HH

#ضمن "PairValueRecord.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {


/* Pair records vary in glyph width; their value traversal need not. */
بنية pair_سیٹ_اقدار_t
{
  ثابت قدر_ادھار *ادھار;
  glyph_انکڑے_t glyphs;
  ثابت قدر *values;
  بنانشانی گنتی;
  بنانشانی ریکارڈ_ناپ;

  بنانشانی get_glyph (بنانشانی i) ثابت
  {
    glyph_انکڑے_t glyph = glyphs;
    glyph.انکڑے = (ثابت حرف *) glyph.انکڑے + i * ریکارڈ_ناپ;
    أرجع glyph[0];
  }

  ثابت قدر *get_اقدار (بنانشانی i) ثابت
  { أرجع reinterpret_cast<ثابت قدر *> ((ثابت حرف *) values + i * ریکارڈ_ناپ); }

  hb_pair_t<بنانشانی, بنانشانی>
  compute_effective_قدر_formats (ثابت hb_سیٹ_t &glyphset,
                                   ثابت قدر_Format قدر_Formats[2],
                                   منطقي strip_hints, منطقي strip_خالی_تھاواں,
                                   ثابت hb_hashmap_t<بنانشانی, hb_pair_t<بنانشانی, صحيح>> *varidx_delta_map) ثابت
  {
    بنانشانی len1 = قدر_Formats[0].get_len ();
    بنانشانی format1 = 0, format2 = 0;
    لكل (بنانشانی i = 0; i < گنتی; i++)
    {
      إذا (!glyphset.has (get_glyph (i))) استمر;
      ثابت قدر *v = get_اقدار (i);
      format1 |= قدر_Formats[0].get_effective_format (v, strip_hints, strip_خالی_تھاواں, ادھار, varidx_delta_map);
      format2 |= قدر_Formats[1].get_effective_format (v + len1, strip_hints, strip_خالی_تھاواں, ادھار, varidx_delta_map);
    }
    أرجع hb_pair (format1, format2);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ثابت قدر_Format قدر_Formats[2]) ثابت
  {
    بنانشانی len1 = قدر_Formats[0].get_len ();
    بنانشانی len2 = قدر_Formats[1].get_len ();
    لكل (بنانشانی i = 0; i < گنتی; i++)
    {
      إذا (!c->glyph_سیٹ->has (get_glyph (i))) استمر;
      ثابت hb_array_t<ثابت قدر> v = hb_array (get_اقدار (i), len1 + len2);
      إذا (قدر_Formats[0].has_device ())
        قدر_Formats[0].collect_variation_indices (c, ادھار, v.sub_array (0, len1));
      إذا (قدر_Formats[1].has_device ())
        قدر_Formats[1].collect_variation_indices (c, ادھار, v.sub_array (len1, len2));
    }
  }
};

سانچا <قسم_ناں Types>
بنية Pair_سیٹ : قدر_ادھار
{
  سانچا <قسم_ناں Types2>
  یار بنية PairPosFormat1_3;

  ورتوں Pair_قدر_ریکارڈ = GPOS_impl::Pair_قدر_ریکارڈ<Types>;

  محمي:
  قسم_ناں Types::HBUINT
                        len;    /* Number of PairValueRecords */
  Pair_قدر_ریکارڈ       پہلا_Pair_قدر_ریکارڈ;
                                /* Array of PairValueRecords--ordered
                                 * by GlyphID of the second glyph */
  عام:
  DEFINE_ناپ_MIN (Types::HBUINT::static_ناپ);

  ساكن size_t get_ناپ (بنانشانی len1, بنانشانی len2)
  {
    أرجع Types::HBGlyphID::static_ناپ + قدر::static_ناپ * (len1 + len2);
  }
  ساكن size_t get_ناپ (ثابت قدر_Format قدر_Formats[2])
  {
    بنانشانی len1 = قدر_Formats[0].get_len ();
    بنانشانی len2 = قدر_Formats[1].get_len ();
    أرجع get_ناپ (len1, len2);
  }

  بنية sanitize_گھیرا_t
  {
    ثابت قدر_Format *قدر_Formats;
    بنانشانی صحيح len1; /* valueFormats[0].get_len() */
    size_t stride; /*بائٹاں*/
  };

  منطقي sanitize (hb_sanitize_context_t *c, ثابت sanitize_گھیرا_t *گھیرا) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (!(c->check_struct (هذا) &&
	  hb_barrier () &&
          c->check_range (&پہلا_Pair_قدر_ریکارڈ,
                          len,
                          گھیرا->stride))) return_trace (خطأ);
    hb_barrier ();

    بنانشانی صحيح گنتی = len;
    ثابت Pair_قدر_ریکارڈ *ریکارڈ = &پہلا_Pair_قدر_ریکارڈ;
    return_trace (c->lazy_some_gpos ||
		  (گھیرا->قدر_Formats[0].sanitize_اقدار_stride_unsafe (c, هذا, &ریکارڈ->values[0], گنتی, گھیرا->stride) &&
                   گھیرا->قدر_Formats[1].sanitize_اقدار_stride_unsafe (c, هذا, &ریکارڈ->values[گھیرا->len1], گنتی, گھیرا->stride)));
  }

  منطقي intersects (ثابت hb_سیٹ_t *glyphs,
                   ثابت قدر_Format *قدر_Formats) ثابت
  {
    بنانشانی ریکارڈ_ناپ = get_ناپ (قدر_Formats);

    ثابت Pair_قدر_ریکارڈ *ریکارڈ = &پہلا_Pair_قدر_ریکارڈ;
    بنانشانی صحيح گنتی = len;
    لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
    {
      إذا (glyphs->has (ریکارڈ->دوجا_Glyph))
        أرجع سچ;
      ریکارڈ = &Struct_At_وتھ<ثابت Pair_قدر_ریکارڈ> (ریکارڈ, ریکارڈ_ناپ);
    }
    أرجع خطأ;
  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c,
                       ثابت قدر_Format *قدر_Formats) ثابت
  {
    بنانشانی ریکارڈ_ناپ = get_ناپ (قدر_Formats);

    ثابت Pair_قدر_ریکارڈ *ریکارڈ = &پہلا_Pair_قدر_ریکارڈ;
    c->داخلہ->جوڑو_array (&ریکارڈ->دوجا_Glyph, len, ریکارڈ_ناپ);
  }

  سانچا <قسم_ناں سیٹ_t>
  فراغ collect_دوجا_glyphs (سیٹ_t *glyphs,
			      ثابت قدر_Format *قدر_Formats) ثابت
  {
    بنانشانی ریکارڈ_ناپ = get_ناپ (قدر_Formats);
    ثابت Pair_قدر_ریکارڈ *ریکارڈ = &پہلا_Pair_قدر_ریکارڈ;
    glyphs->جوڑو_array (&ریکارڈ->دوجا_Glyph, len, ریکارڈ_ناپ);
  }

  pair_سیٹ_اقدار_t get_اقدار_view (ثابت قدر_Format قدر_Formats[2]) ثابت
  {
    أرجع {هذا, &پہلا_Pair_قدر_ریکارڈ.دوجا_Glyph, &پہلا_Pair_قدر_ریکارڈ.values[0],
            (بنانشانی) len, (بنانشانی) get_ناپ (قدر_Formats)};
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ثابت قدر_Format *قدر_Formats) ثابت
  {
    get_اقدار_view (قدر_Formats).collect_variation_indices (c, قدر_Formats);
  }

  منطقي apply (hb_ot_apply_context_t *c,
              ثابت قدر_Format *قدر_Formats,
              بنانشانی صحيح pos) ثابت
  {
    TRACE_APPLY (هذا);
    hb_بفر_t *بفر = c->بفر;
    بنانشانی صحيح len1 = قدر_Formats[0].get_len ();
    بنانشانی صحيح len2 = قدر_Formats[1].get_len ();
    بنانشانی ریکارڈ_ناپ = get_ناپ (len1, len2);

    ثابت Pair_قدر_ریکارڈ *ریکارڈ = hb_bsearch (بفر->info[pos].codepoint,
                                                &پہلا_Pair_قدر_ریکارڈ,
                                                (بنانشانی) len,
                                                ریکارڈ_ناپ);
    إذا (ریکارڈ)
    {
      إذا (HB_بفر_سنیہا_ہور && c->بفر->messaging ())
      {
	c->بفر->سنیہا (c->font,
			    "try kerning glyphs at %u,%u",
			    c->بفر->idx, pos);
      }

      منطقي applied_پہلا = len1 && قدر_Formats[0].apply_قدر (c, هذا, &ریکارڈ->values[0], بفر->cur_pos());
      منطقي applied_دوجا = len2 && قدر_Formats[1].apply_قدر (c, هذا, &ریکارڈ->values[len1], بفر->pos[pos]);

      إذا (applied_پہلا || applied_دوجا)
	إذا (HB_بفر_سنیہا_ہور && c->بفر->messaging ())
	{
	  c->بفر->سنیہا (c->font,
			      "kerned glyphs at %u,%u",
			      c->بفر->idx, pos);
	}

      إذا (HB_بفر_سنیہا_ہور && c->بفر->messaging ())
      {
	c->بفر->سنیہا (c->font,
			    "tried kerning glyphs at %u,%u",
			    c->بفر->idx, pos);
      }

      إذا (applied_پہلا || applied_دوجا)
        بفر->unsafe_منزل_break (بفر->idx, pos + 1);
      وإلا
        /* Even a zero-valued pair record is a concat hazard. */
        بفر->unsafe_منزل_concat (بفر->idx, pos + 1);

      إذا (len2)
      {
	pos++;
      // https://github.com/harfbuzz/harfbuzz/issues/3824
      // https://github.com/harfbuzz/harfbuzz/issues/3888#issuecomment-1326781116
      بفر->unsafe_منزل_break (بفر->idx, pos + 1);
      }

      بفر->idx = pos;
      return_trace (سچ);
    }
    بفر->unsafe_منزل_concat (بفر->idx, pos + 1);
    return_trace (خطأ);
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت قدر_Format قدر_Formats[2],
               ثابت قدر_Format newFormats[2]) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار snap = c->serializer->snapshot ();

    خودکار *باہرلا = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (باہرلا))) return_trace (خطأ);
    باہرلا->len = 0;

    ثابت hb_سیٹ_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    بنانشانی len1 = قدر_Formats[0].get_len ();
    بنانشانی len2 = قدر_Formats[1].get_len ();
    بنانشانی ریکارڈ_ناپ = get_ناپ (len1, len2);

    pair_قدر_ریکارڈ_context_t context =
    {
      هذا,
      قدر_Formats,
      newFormats,
      len1,
      &glyph_map,
      &c->plan->layout_variation_idx_delta_map
    };

    ثابت Pair_قدر_ریکارڈ *ریکارڈ = &پہلا_Pair_قدر_ریکارڈ;
    بنانشانی گنتی = len, عدد = 0;
    لكل (بنانشانی i = 0; i < گنتی; i++)
    {
      إذا (glyphset.has (ریکارڈ->دوجا_Glyph)
         && ریکارڈ->subset (c, &context)) عدد++;
      ریکارڈ = &Struct_At_وتھ<ثابت Pair_قدر_ریکارڈ> (ریکارڈ, ریکارڈ_ناپ);
    }

    باہرلا->len = عدد;
    إذا (!عدد) c->serializer->revert (snap);
    return_trace (عدد);
  }
};


}
}
}

#جے_انت  // OT_LAYOUT_GPOS_PAIRSET_HH
