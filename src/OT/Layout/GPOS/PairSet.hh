#که_نه_تعریف_شوی OT_LAYOUT_GPOS_PAIRSET_HH
#تعریف_کړه OT_LAYOUT_GPOS_PAIRSET_HH

#ضمن "PairValueRecord.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {


/* Pair records vary in glyph width; their value traversal need not. */
بنية pair_سېټ_ارزښتونه_t
{
  ثابت ارزښت_بنسټ *بنسټ;
  glyph_معلومات_t glyphs;
  ثابت ارزښت *values;
  بینښه شمېر;
  بینښه ریکارډ_اندازه;

  بینښه get_glyph (بینښه i) ثابت
  {
    glyph_معلومات_t glyph = glyphs;
    glyph.معلومات = (ثابت حرف *) glyph.معلومات + i * ریکارډ_اندازه;
    أرجع glyph[0];
  }

  ثابت ارزښت *get_ارزښتونه (بینښه i) ثابت
  { أرجع reinterpret_cast<ثابت ارزښت *> ((ثابت حرف *) values + i * ریکارډ_اندازه); }

  hb_pair_t<بینښه, بینښه>
  compute_effective_ارزښت_formats (ثابت hb_سېټ_t &glyphset,
                                   ثابت ارزښت_Format ارزښت_Formats[2],
                                   منطقي strip_hints, منطقي strip_تشه,
                                   ثابت hb_hashmap_t<بینښه, hb_pair_t<بینښه, صحيح>> *varidx_delta_map) ثابت
  {
    بینښه len1 = ارزښت_Formats[0].get_len ();
    بینښه format1 = 0, format2 = 0;
    لكل (بینښه i = 0; i < شمېر; i++)
    {
      إذا (!glyphset.has (get_glyph (i))) استمر;
      ثابت ارزښت *v = get_ارزښتونه (i);
      format1 |= ارزښت_Formats[0].get_effective_format (v, strip_hints, strip_تشه, بنسټ, varidx_delta_map);
      format2 |= ارزښت_Formats[1].get_effective_format (v + len1, strip_hints, strip_تشه, بنسټ, varidx_delta_map);
    }
    أرجع hb_pair (format1, format2);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ثابت ارزښت_Format ارزښت_Formats[2]) ثابت
  {
    بینښه len1 = ارزښت_Formats[0].get_len ();
    بینښه len2 = ارزښت_Formats[1].get_len ();
    لكل (بینښه i = 0; i < شمېر; i++)
    {
      إذا (!c->glyph_سېټ->has (get_glyph (i))) استمر;
      ثابت hb_array_t<ثابت ارزښت> v = hb_array (get_ارزښتونه (i), len1 + len2);
      إذا (ارزښت_Formats[0].has_device ())
        ارزښت_Formats[0].collect_variation_indices (c, بنسټ, v.sub_array (0, len1));
      إذا (ارزښت_Formats[1].has_device ())
        ارزښت_Formats[1].collect_variation_indices (c, بنسټ, v.sub_array (len1, len2));
    }
  }
};

کينډۍ <ډول_نوم Types>
بنية Pair_سېټ : ارزښت_بنسټ
{
  کينډۍ <ډول_نوم Types2>
  ملګری بنية PairPosFormat1_3;

  کارول Pair_ارزښت_ریکارډ = GPOS_impl::Pair_ارزښت_ریکارډ<Types>;

  محمي:
  ډول_نوم Types::HBUINT
                        len;    /* Number of PairValueRecords */
  Pair_ارزښت_ریکارډ       لومړی_Pair_ارزښت_ریکارډ;
                                /* Array of PairValueRecords--ordered
                                 * by GlyphID of the second glyph */
  عام:
  DEFINE_اندازه_MIN (Types::HBUINT::static_اندازه);

  ساكن size_t get_اندازه (بینښه len1, بینښه len2)
  {
    أرجع Types::HBGlyphID::static_اندازه + ارزښت::static_اندازه * (len1 + len2);
  }
  ساكن size_t get_اندازه (ثابت ارزښت_Format ارزښت_Formats[2])
  {
    بینښه len1 = ارزښت_Formats[0].get_len ();
    بینښه len2 = ارزښت_Formats[1].get_len ();
    أرجع get_اندازه (len1, len2);
  }

  بنية sanitize_تړون_t
  {
    ثابت ارزښت_Format *ارزښت_Formats;
    بینښه صحيح len1; /* valueFormats[0].get_len() */
    size_t stride; /*بایټونه*/
  };

  منطقي sanitize (hb_sanitize_context_t *c, ثابت sanitize_تړون_t *تړون) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (!(c->check_struct (هذا) &&
	  hb_barrier () &&
          c->check_range (&لومړی_Pair_ارزښت_ریکارډ,
                          len,
                          تړون->stride))) return_trace (خطأ);
    hb_barrier ();

    بینښه صحيح شمېر = len;
    ثابت Pair_ارزښت_ریکارډ *ریکارډ = &لومړی_Pair_ارزښت_ریکارډ;
    return_trace (c->lazy_some_gpos ||
		  (تړون->ارزښت_Formats[0].sanitize_ارزښتونه_stride_unsafe (c, هذا, &ریکارډ->values[0], شمېر, تړون->stride) &&
                   تړون->ارزښت_Formats[1].sanitize_ارزښتونه_stride_unsafe (c, هذا, &ریکارډ->values[تړون->len1], شمېر, تړون->stride)));
  }

  منطقي intersects (ثابت hb_سېټ_t *glyphs,
                   ثابت ارزښت_Format *ارزښت_Formats) ثابت
  {
    بینښه ریکارډ_اندازه = get_اندازه (ارزښت_Formats);

    ثابت Pair_ارزښت_ریکارډ *ریکارډ = &لومړی_Pair_ارزښت_ریکارډ;
    بینښه صحيح شمېر = len;
    لكل (بینښه صحيح i = 0; i < شمېر; i++)
    {
      إذا (glyphs->has (ریکارډ->دوهم_Glyph))
        أرجع ریښتیا;
      ریکارډ = &Struct_At_دننه_واټن<ثابت Pair_ارزښت_ریکارډ> (ریکارډ, ریکارډ_اندازه);
    }
    أرجع خطأ;
  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c,
                       ثابت ارزښت_Format *ارزښت_Formats) ثابت
  {
    بینښه ریکارډ_اندازه = get_اندازه (ارزښت_Formats);

    ثابت Pair_ارزښت_ریکارډ *ریکارډ = &لومړی_Pair_ارزښت_ریکارډ;
    c->ننوت->جمع_کړه_array (&ریکارډ->دوهم_Glyph, len, ریکارډ_اندازه);
  }

  کينډۍ <ډول_نوم سېټ_t>
  فراغ collect_دوهم_glyphs (سېټ_t *glyphs,
			      ثابت ارزښت_Format *ارزښت_Formats) ثابت
  {
    بینښه ریکارډ_اندازه = get_اندازه (ارزښت_Formats);
    ثابت Pair_ارزښت_ریکارډ *ریکارډ = &لومړی_Pair_ارزښت_ریکارډ;
    glyphs->جمع_کړه_array (&ریکارډ->دوهم_Glyph, len, ریکارډ_اندازه);
  }

  pair_سېټ_ارزښتونه_t get_ارزښتونه_view (ثابت ارزښت_Format ارزښت_Formats[2]) ثابت
  {
    أرجع {هذا, &لومړی_Pair_ارزښت_ریکارډ.دوهم_Glyph, &لومړی_Pair_ارزښت_ریکارډ.values[0],
            (بینښه) len, (بینښه) get_اندازه (ارزښت_Formats)};
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ثابت ارزښت_Format *ارزښت_Formats) ثابت
  {
    get_ارزښتونه_view (ارزښت_Formats).collect_variation_indices (c, ارزښت_Formats);
  }

  منطقي apply (hb_ot_apply_context_t *c,
              ثابت ارزښت_Format *ارزښت_Formats,
              بینښه صحيح pos) ثابت
  {
    TRACE_APPLY (هذا);
    hb_بفر_t *بفر = c->بفر;
    بینښه صحيح len1 = ارزښت_Formats[0].get_len ();
    بینښه صحيح len2 = ارزښت_Formats[1].get_len ();
    بینښه ریکارډ_اندازه = get_اندازه (len1, len2);

    ثابت Pair_ارزښت_ریکارډ *ریکارډ = hb_bsearch (بفر->info[pos].codepoint,
                                                &لومړی_Pair_ارزښت_ریکارډ,
                                                (بینښه) len,
                                                ریکارډ_اندازه);
    إذا (ریکارډ)
    {
      إذا (HB_بفر_پیغام_نور && c->بفر->messaging ())
      {
	c->بفر->پیغام (c->font,
			    "try kerning glyphs at %u,%u",
			    c->بفر->idx, pos);
      }

      منطقي applied_لومړی = len1 && ارزښت_Formats[0].apply_ارزښت (c, هذا, &ریکارډ->values[0], بفر->cur_pos());
      منطقي applied_دوهم = len2 && ارزښت_Formats[1].apply_ارزښت (c, هذا, &ریکارډ->values[len1], بفر->pos[pos]);

      إذا (applied_لومړی || applied_دوهم)
	إذا (HB_بفر_پیغام_نور && c->بفر->messaging ())
	{
	  c->بفر->پیغام (c->font,
			      "kerned glyphs at %u,%u",
			      c->بفر->idx, pos);
	}

      إذا (HB_بفر_پیغام_نور && c->بفر->messaging ())
      {
	c->بفر->پیغام (c->font,
			    "tried kerning glyphs at %u,%u",
			    c->بفر->idx, pos);
      }

      إذا (applied_لومړی || applied_دوهم)
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
      return_trace (ریښتیا);
    }
    بفر->unsafe_منزل_concat (بفر->idx, pos + 1);
    return_trace (خطأ);
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت ارزښت_Format ارزښت_Formats[2],
               ثابت ارزښت_Format newFormats[2]) ثابت
  {
    TRACE_SUBSET (هذا);
    خپلکاری snap = c->serializer->snapshot ();

    خپلکاری *بهر = c->serializer->پیل_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (بهر))) return_trace (خطأ);
    بهر->len = 0;

    ثابت hb_سېټ_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    بینښه len1 = ارزښت_Formats[0].get_len ();
    بینښه len2 = ارزښت_Formats[1].get_len ();
    بینښه ریکارډ_اندازه = get_اندازه (len1, len2);

    pair_ارزښت_ریکارډ_context_t context =
    {
      هذا,
      ارزښت_Formats,
      newFormats,
      len1,
      &glyph_map,
      &c->plan->layout_variation_idx_delta_map
    };

    ثابت Pair_ارزښت_ریکارډ *ریکارډ = &لومړی_Pair_ارزښت_ریکارډ;
    بینښه شمېر = len, عدد = 0;
    لكل (بینښه i = 0; i < شمېر; i++)
    {
      إذا (glyphset.has (ریکارډ->دوهم_Glyph)
         && ریکارډ->subset (c, &context)) عدد++;
      ریکارډ = &Struct_At_دننه_واټن<ثابت Pair_ارزښت_ریکارډ> (ریکارډ, ریکارډ_اندازه);
    }

    بهر->len = عدد;
    إذا (!عدد) c->serializer->revert (snap);
    return_trace (عدد);
  }
};


}
}
}

#د_که_پای  // OT_LAYOUT_GPOS_PAIRSET_HH
