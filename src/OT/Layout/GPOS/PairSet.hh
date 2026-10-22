#اگر_غیر_تعریف_شدہ OT_LAYOUT_GPOS_PAIRSET_HH
#تعریف_کرو OT_LAYOUT_GPOS_PAIRSET_HH

#شامل "PairValueRecord.hh"

نامخلا OT {
نامخلا Layout {
نامخلا GPOS_impl {


/* Pair records vary in glyph width; their value traversal need not. */
ساخت pair_سیٹ_اقدار_t
{
  ثابت قدر_بنیاد *بنیاد;
  glyph_ڈیٹا_t glyphs;
  ثابت قدر *values;
  غیرعلامتی گنتی;
  غیرعلامتی ریکارڈ_سائز;

  غیرعلامتی get_glyph (غیرعلامتی i) ثابت
  {
    glyph_ڈیٹا_t glyph = glyphs;
    glyph.ڈیٹا = (ثابت حرف *) glyph.ڈیٹا + i * ریکارڈ_سائز;
    واپس glyph[0];
  }

  ثابت قدر *get_اقدار (غیرعلامتی i) ثابت
  { واپس reinterpret_cast<ثابت قدر *> ((ثابت حرف *) values + i * ریکارڈ_سائز); }

  hb_pair_t<غیرعلامتی, غیرعلامتی>
  compute_effective_قدر_formats (ثابت hb_سیٹ_t &glyphset,
                                   ثابت قدر_Format قدر_Formats[2],
                                   منطقی strip_hints, منطقی strip_خالی_خانے,
                                   ثابت hb_hashmap_t<غیرعلامتی, hb_pair_t<غیرعلامتی, صحیح>> *varidx_delta_map) ثابت
  {
    غیرعلامتی len1 = قدر_Formats[0].get_len ();
    غیرعلامتی format1 = 0, format2 = 0;
    برائے (غیرعلامتی i = 0; i < گنتی; i++)
    {
      اگر (!glyphset.has (get_glyph (i))) جاری;
      ثابت قدر *v = get_اقدار (i);
      format1 |= قدر_Formats[0].get_effective_format (v, strip_hints, strip_خالی_خانے, بنیاد, varidx_delta_map);
      format2 |= قدر_Formats[1].get_effective_format (v + len1, strip_hints, strip_خالی_خانے, بنیاد, varidx_delta_map);
    }
    واپس hb_pair (format1, format2);
  }

  خالی collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ثابت قدر_Format قدر_Formats[2]) ثابت
  {
    غیرعلامتی len1 = قدر_Formats[0].get_len ();
    غیرعلامتی len2 = قدر_Formats[1].get_len ();
    برائے (غیرعلامتی i = 0; i < گنتی; i++)
    {
      اگر (!c->glyph_سیٹ->has (get_glyph (i))) جاری;
      ثابت hb_array_t<ثابت قدر> v = hb_array (get_اقدار (i), len1 + len2);
      اگر (قدر_Formats[0].has_device ())
        قدر_Formats[0].collect_variation_indices (c, بنیاد, v.sub_array (0, len1));
      اگر (قدر_Formats[1].has_device ())
        قدر_Formats[1].collect_variation_indices (c, بنیاد, v.sub_array (len1, len2));
    }
  }
};

سانچہ <قسم_نام Types>
ساخت Pair_سیٹ : قدر_بنیاد
{
  سانچہ <قسم_نام Types2>
  دوست ساخت PairPosFormat1_3;

  استعمال Pair_قدر_ریکارڈ = GPOS_impl::Pair_قدر_ریکارڈ<Types>;

  محفوظ:
  قسم_نام Types::HBUINT
                        len;    /* Number of PairValueRecords */
  Pair_قدر_ریکارڈ       پہلا_Pair_قدر_ریکارڈ;
                                /* Array of PairValueRecords--ordered
                                 * by GlyphID of the second glyph */
  عوامی:
  DEFINE_سائز_MIN (Types::HBUINT::static_سائز);

  ساکن size_t get_سائز (غیرعلامتی len1, غیرعلامتی len2)
  {
    واپس Types::HBGlyphID::static_سائز + قدر::static_سائز * (len1 + len2);
  }
  ساکن size_t get_سائز (ثابت قدر_Format قدر_Formats[2])
  {
    غیرعلامتی len1 = قدر_Formats[0].get_len ();
    غیرعلامتی len2 = قدر_Formats[1].get_len ();
    واپس get_سائز (len1, len2);
  }

  ساخت sanitize_احاطہ_t
  {
    ثابت قدر_Format *قدر_Formats;
    غیرعلامتی صحیح len1; /* valueFormats[0].get_len() */
    size_t stride; /*بائٹیں*/
  };

  منطقی sanitize (hb_sanitize_context_t *c, ثابت sanitize_احاطہ_t *احاطہ) ثابت
  {
    TRACE_SANITIZE (یہ);
    اگر (!(c->check_struct (یہ) &&
	  hb_barrier () &&
          c->check_range (&پہلا_Pair_قدر_ریکارڈ,
                          len,
                          احاطہ->stride))) return_trace (جھوٹ);
    hb_barrier ();

    غیرعلامتی صحیح گنتی = len;
    ثابت Pair_قدر_ریکارڈ *ریکارڈ = &پہلا_Pair_قدر_ریکارڈ;
    return_trace (c->lazy_some_gpos ||
		  (احاطہ->قدر_Formats[0].sanitize_اقدار_stride_unsafe (c, یہ, &ریکارڈ->values[0], گنتی, احاطہ->stride) &&
                   احاطہ->قدر_Formats[1].sanitize_اقدار_stride_unsafe (c, یہ, &ریکارڈ->values[احاطہ->len1], گنتی, احاطہ->stride)));
  }

  منطقی intersects (ثابت hb_سیٹ_t *glyphs,
                   ثابت قدر_Format *قدر_Formats) ثابت
  {
    غیرعلامتی ریکارڈ_سائز = get_سائز (قدر_Formats);

    ثابت Pair_قدر_ریکارڈ *ریکارڈ = &پہلا_Pair_قدر_ریکارڈ;
    غیرعلامتی صحیح گنتی = len;
    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
    {
      اگر (glyphs->has (ریکارڈ->دوسرا_Glyph))
        واپس سچ;
      ریکارڈ = &Struct_At_انحراف<ثابت Pair_قدر_ریکارڈ> (ریکارڈ, ریکارڈ_سائز);
    }
    واپس جھوٹ;
  }

  خالی collect_glyphs (hb_collect_glyphs_context_t *c,
                       ثابت قدر_Format *قدر_Formats) ثابت
  {
    غیرعلامتی ریکارڈ_سائز = get_سائز (قدر_Formats);

    ثابت Pair_قدر_ریکارڈ *ریکارڈ = &پہلا_Pair_قدر_ریکارڈ;
    c->داخلہ->جوڑو_array (&ریکارڈ->دوسرا_Glyph, len, ریکارڈ_سائز);
  }

  سانچہ <قسم_نام سیٹ_t>
  خالی collect_دوسرا_glyphs (سیٹ_t *glyphs,
			      ثابت قدر_Format *قدر_Formats) ثابت
  {
    غیرعلامتی ریکارڈ_سائز = get_سائز (قدر_Formats);
    ثابت Pair_قدر_ریکارڈ *ریکارڈ = &پہلا_Pair_قدر_ریکارڈ;
    glyphs->جوڑو_array (&ریکارڈ->دوسرا_Glyph, len, ریکارڈ_سائز);
  }

  pair_سیٹ_اقدار_t get_اقدار_view (ثابت قدر_Format قدر_Formats[2]) ثابت
  {
    واپس {یہ, &پہلا_Pair_قدر_ریکارڈ.دوسرا_Glyph, &پہلا_Pair_قدر_ریکارڈ.values[0],
            (غیرعلامتی) len, (غیرعلامتی) get_سائز (قدر_Formats)};
  }

  خالی collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ثابت قدر_Format *قدر_Formats) ثابت
  {
    get_اقدار_view (قدر_Formats).collect_variation_indices (c, قدر_Formats);
  }

  منطقی apply (hb_ot_apply_context_t *c,
              ثابت قدر_Format *قدر_Formats,
              غیرعلامتی صحیح pos) ثابت
  {
    TRACE_APPLY (یہ);
    hb_بفر_t *بفر = c->بفر;
    غیرعلامتی صحیح len1 = قدر_Formats[0].get_len ();
    غیرعلامتی صحیح len2 = قدر_Formats[1].get_len ();
    غیرعلامتی ریکارڈ_سائز = get_سائز (len1, len2);

    ثابت Pair_قدر_ریکارڈ *ریکارڈ = hb_bsearch (بفر->info[pos].codepoint,
                                                &پہلا_Pair_قدر_ریکارڈ,
                                                (غیرعلامتی) len,
                                                ریکارڈ_سائز);
    اگر (ریکارڈ)
    {
      اگر (HB_بفر_پیغام_مزید && c->بفر->messaging ())
      {
	c->بفر->پیغام (c->font,
			    "try kerning glyphs at %u,%u",
			    c->بفر->idx, pos);
      }

      منطقی applied_پہلا = len1 && قدر_Formats[0].apply_قدر (c, یہ, &ریکارڈ->values[0], بفر->cur_pos());
      منطقی applied_دوسرا = len2 && قدر_Formats[1].apply_قدر (c, یہ, &ریکارڈ->values[len1], بفر->pos[pos]);

      اگر (applied_پہلا || applied_دوسرا)
	اگر (HB_بفر_پیغام_مزید && c->بفر->messaging ())
	{
	  c->بفر->پیغام (c->font,
			      "kerned glyphs at %u,%u",
			      c->بفر->idx, pos);
	}

      اگر (HB_بفر_پیغام_مزید && c->بفر->messaging ())
      {
	c->بفر->پیغام (c->font,
			    "tried kerning glyphs at %u,%u",
			    c->بفر->idx, pos);
      }

      اگر (applied_پہلا || applied_دوسرا)
        بفر->unsafe_مقصد_break (بفر->idx, pos + 1);
      ورنہ
        /* Even a zero-valued pair record is a concat hazard. */
        بفر->unsafe_مقصد_concat (بفر->idx, pos + 1);

      اگر (len2)
      {
	pos++;
      // https://github.com/harfbuzz/harfbuzz/issues/3824
      // https://github.com/harfbuzz/harfbuzz/issues/3888#issuecomment-1326781116
      بفر->unsafe_مقصد_break (بفر->idx, pos + 1);
      }

      بفر->idx = pos;
      return_trace (سچ);
    }
    بفر->unsafe_مقصد_concat (بفر->idx, pos + 1);
    return_trace (جھوٹ);
  }

  منطقی subset (hb_subset_context_t *c,
               ثابت قدر_Format قدر_Formats[2],
               ثابت قدر_Format newFormats[2]) ثابت
  {
    TRACE_SUBSET (یہ);
    خودکار snap = c->serializer->snapshot ();

    خودکار *برآمد = c->serializer->آغاز_embed (*یہ);
    اگر (unlikely (!c->serializer->extend_min (برآمد))) return_trace (جھوٹ);
    برآمد->len = 0;

    ثابت hb_سیٹ_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    غیرعلامتی len1 = قدر_Formats[0].get_len ();
    غیرعلامتی len2 = قدر_Formats[1].get_len ();
    غیرعلامتی ریکارڈ_سائز = get_سائز (len1, len2);

    pair_قدر_ریکارڈ_context_t context =
    {
      یہ,
      قدر_Formats,
      newFormats,
      len1,
      &glyph_map,
      &c->plan->layout_variation_idx_delta_map
    };

    ثابت Pair_قدر_ریکارڈ *ریکارڈ = &پہلا_Pair_قدر_ریکارڈ;
    غیرعلامتی گنتی = len, عدد = 0;
    برائے (غیرعلامتی i = 0; i < گنتی; i++)
    {
      اگر (glyphset.has (ریکارڈ->دوسرا_Glyph)
         && ریکارڈ->subset (c, &context)) عدد++;
      ریکارڈ = &Struct_At_انحراف<ثابت Pair_قدر_ریکارڈ> (ریکارڈ, ریکارڈ_سائز);
    }

    برآمد->len = عدد;
    اگر (!عدد) c->serializer->revert (snap);
    return_trace (عدد);
  }
};


}
}
}

#اگر_ختم  // OT_LAYOUT_GPOS_PAIRSET_HH
