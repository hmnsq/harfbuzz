#جيڪڏهن_وصف_نه_ٿيل OT_LAYOUT_GPOS_PAIRSET_HH
#وصف_ڪريو OT_LAYOUT_GPOS_PAIRSET_HH

#ضمن "PairValueRecord.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {


/* Pair records vary in glyph width; their value traversal need not. */
بنية pair_سيٽ_قدرون_t
{
  ثابت قدر_بنياد *بنياد;
  glyph_معلومات_t glyphs;
  ثابت قدر *values;
  بنانشان ڳڻپ;
  بنانشان رڪارڊ_سائيز;

  بنانشان get_glyph (بنانشان i) ثابت
  {
    glyph_معلومات_t glyph = glyphs;
    glyph.معلومات = (ثابت حرف *) glyph.معلومات + i * رڪارڊ_سائيز;
    أرجع glyph[0];
  }

  ثابت قدر *get_قدرون (بنانشان i) ثابت
  { أرجع reinterpret_cast<ثابت قدر *> ((ثابت حرف *) values + i * رڪارڊ_سائيز); }

  hb_pair_t<بنانشان, بنانشان>
  compute_effective_قدر_formats (ثابت hb_سيٽ_t &glyphset,
                                   ثابت قدر_Format قدر_Formats[2],
                                   منطقي strip_hints, منطقي strip_خالي_جايون,
                                   ثابت hb_hashmap_t<بنانشان, hb_pair_t<بنانشان, صحيح>> *varidx_delta_map) ثابت
  {
    بنانشان len1 = قدر_Formats[0].get_len ();
    بنانشان format1 = 0, format2 = 0;
    لكل (بنانشان i = 0; i < ڳڻپ; i++)
    {
      إذا (!glyphset.has (get_glyph (i))) استمر;
      ثابت قدر *v = get_قدرون (i);
      format1 |= قدر_Formats[0].get_effective_format (v, strip_hints, strip_خالي_جايون, بنياد, varidx_delta_map);
      format2 |= قدر_Formats[1].get_effective_format (v + len1, strip_hints, strip_خالي_جايون, بنياد, varidx_delta_map);
    }
    أرجع hb_pair (format1, format2);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ثابت قدر_Format قدر_Formats[2]) ثابت
  {
    بنانشان len1 = قدر_Formats[0].get_len ();
    بنانشان len2 = قدر_Formats[1].get_len ();
    لكل (بنانشان i = 0; i < ڳڻپ; i++)
    {
      إذا (!c->glyph_سيٽ->has (get_glyph (i))) استمر;
      ثابت hb_array_t<ثابت قدر> v = hb_array (get_قدرون (i), len1 + len2);
      إذا (قدر_Formats[0].has_device ())
        قدر_Formats[0].collect_variation_indices (c, بنياد, v.sub_array (0, len1));
      إذا (قدر_Formats[1].has_device ())
        قدر_Formats[1].collect_variation_indices (c, بنياد, v.sub_array (len1, len2));
    }
  }
};

سانچو <قسم_نالو Types>
بنية Pair_سيٽ : قدر_بنياد
{
  سانچو <قسم_نالو Types2>
  دوست بنية PairPosFormat1_3;

  استعمال Pair_قدر_رڪارڊ = GPOS_impl::Pair_قدر_رڪارڊ<Types>;

  محمي:
  قسم_نالو Types::HBUINT
                        len;    /* Number of PairValueRecords */
  Pair_قدر_رڪارڊ       پهريون_Pair_قدر_رڪارڊ;
                                /* Array of PairValueRecords--ordered
                                 * by GlyphID of the second glyph */
  عام:
  DEFINE_سائيز_MIN (Types::HBUINT::static_سائيز);

  ساكن size_t get_سائيز (بنانشان len1, بنانشان len2)
  {
    أرجع Types::HBGlyphID::static_سائيز + قدر::static_سائيز * (len1 + len2);
  }
  ساكن size_t get_سائيز (ثابت قدر_Format قدر_Formats[2])
  {
    بنانشان len1 = قدر_Formats[0].get_len ();
    بنانشان len2 = قدر_Formats[1].get_len ();
    أرجع get_سائيز (len1, len2);
  }

  بنية sanitize_گهيرو_t
  {
    ثابت قدر_Format *قدر_Formats;
    بنانشان صحيح len1; /* valueFormats[0].get_len() */
    size_t stride; /*بائيٽون*/
  };

  منطقي sanitize (hb_sanitize_context_t *c, ثابت sanitize_گهيرو_t *گهيرو) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (!(c->check_struct (هذا) &&
	  hb_barrier () &&
          c->check_range (&پهريون_Pair_قدر_رڪارڊ,
                          len,
                          گهيرو->stride))) return_trace (خطأ);
    hb_barrier ();

    بنانشان صحيح ڳڻپ = len;
    ثابت Pair_قدر_رڪارڊ *رڪارڊ = &پهريون_Pair_قدر_رڪارڊ;
    return_trace (c->lazy_some_gpos ||
		  (گهيرو->قدر_Formats[0].sanitize_قدرون_stride_unsafe (c, هذا, &رڪارڊ->values[0], ڳڻپ, گهيرو->stride) &&
                   گهيرو->قدر_Formats[1].sanitize_قدرون_stride_unsafe (c, هذا, &رڪارڊ->values[گهيرو->len1], ڳڻپ, گهيرو->stride)));
  }

  منطقي intersects (ثابت hb_سيٽ_t *glyphs,
                   ثابت قدر_Format *قدر_Formats) ثابت
  {
    بنانشان رڪارڊ_سائيز = get_سائيز (قدر_Formats);

    ثابت Pair_قدر_رڪارڊ *رڪارڊ = &پهريون_Pair_قدر_رڪارڊ;
    بنانشان صحيح ڳڻپ = len;
    لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
    {
      إذا (glyphs->has (رڪارڊ->ٻيو_Glyph))
        أرجع سچ;
      رڪارڊ = &Struct_At_وٿي<ثابت Pair_قدر_رڪارڊ> (رڪارڊ, رڪارڊ_سائيز);
    }
    أرجع خطأ;
  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c,
                       ثابت قدر_Format *قدر_Formats) ثابت
  {
    بنانشان رڪارڊ_سائيز = get_سائيز (قدر_Formats);

    ثابت Pair_قدر_رڪارڊ *رڪارڊ = &پهريون_Pair_قدر_رڪارڊ;
    c->داخلا_2->جوڙيو_array (&رڪارڊ->ٻيو_Glyph, len, رڪارڊ_سائيز);
  }

  سانچو <قسم_نالو سيٽ_t>
  فراغ collect_ٻيو_glyphs (سيٽ_t *glyphs,
			      ثابت قدر_Format *قدر_Formats) ثابت
  {
    بنانشان رڪارڊ_سائيز = get_سائيز (قدر_Formats);
    ثابت Pair_قدر_رڪارڊ *رڪارڊ = &پهريون_Pair_قدر_رڪارڊ;
    glyphs->جوڙيو_array (&رڪارڊ->ٻيو_Glyph, len, رڪارڊ_سائيز);
  }

  pair_سيٽ_قدرون_t get_قدرون_view (ثابت قدر_Format قدر_Formats[2]) ثابت
  {
    أرجع {هذا, &پهريون_Pair_قدر_رڪارڊ.ٻيو_Glyph, &پهريون_Pair_قدر_رڪارڊ.values[0],
            (بنانشان) len, (بنانشان) get_سائيز (قدر_Formats)};
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ثابت قدر_Format *قدر_Formats) ثابت
  {
    get_قدرون_view (قدر_Formats).collect_variation_indices (c, قدر_Formats);
  }

  منطقي apply (hb_ot_apply_context_t *c,
              ثابت قدر_Format *قدر_Formats,
              بنانشان صحيح pos) ثابت
  {
    TRACE_APPLY (هذا);
    hb_بفر_t *بفر = c->بفر;
    بنانشان صحيح len1 = قدر_Formats[0].get_len ();
    بنانشان صحيح len2 = قدر_Formats[1].get_len ();
    بنانشان رڪارڊ_سائيز = get_سائيز (len1, len2);

    ثابت Pair_قدر_رڪارڊ *رڪارڊ = hb_bsearch (بفر->info[pos].codepoint,
                                                &پهريون_Pair_قدر_رڪارڊ,
                                                (بنانشان) len,
                                                رڪارڊ_سائيز);
    إذا (رڪارڊ)
    {
      إذا (HB_بفر_پيغام_وڌيڪ && c->بفر->messaging ())
      {
	c->بفر->پيغام (c->font,
			    "try kerning glyphs at %u,%u",
			    c->بفر->idx, pos);
      }

      منطقي applied_پهريون = len1 && قدر_Formats[0].apply_قدر (c, هذا, &رڪارڊ->values[0], بفر->cur_pos());
      منطقي applied_ٻيو = len2 && قدر_Formats[1].apply_قدر (c, هذا, &رڪارڊ->values[len1], بفر->pos[pos]);

      إذا (applied_پهريون || applied_ٻيو)
	إذا (HB_بفر_پيغام_وڌيڪ && c->بفر->messaging ())
	{
	  c->بفر->پيغام (c->font,
			      "kerned glyphs at %u,%u",
			      c->بفر->idx, pos);
	}

      إذا (HB_بفر_پيغام_وڌيڪ && c->بفر->messaging ())
      {
	c->بفر->پيغام (c->font,
			    "tried kerning glyphs at %u,%u",
			    c->بفر->idx, pos);
      }

      إذا (applied_پهريون || applied_ٻيو)
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
    خودڪار snap = c->serializer->snapshot ();

    خودڪار *ٻاهريون = c->serializer->شروعات_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (ٻاهريون))) return_trace (خطأ);
    ٻاهريون->len = 0;

    ثابت hb_سيٽ_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    بنانشان len1 = قدر_Formats[0].get_len ();
    بنانشان len2 = قدر_Formats[1].get_len ();
    بنانشان رڪارڊ_سائيز = get_سائيز (len1, len2);

    pair_قدر_رڪارڊ_context_t context =
    {
      هذا,
      قدر_Formats,
      newFormats,
      len1,
      &glyph_map,
      &c->plan->layout_variation_idx_delta_map
    };

    ثابت Pair_قدر_رڪارڊ *رڪارڊ = &پهريون_Pair_قدر_رڪارڊ;
    بنانشان ڳڻپ = len, انگ = 0;
    لكل (بنانشان i = 0; i < ڳڻپ; i++)
    {
      إذا (glyphset.has (رڪارڊ->ٻيو_Glyph)
         && رڪارڊ->subset (c, &context)) انگ++;
      رڪارڊ = &Struct_At_وٿي<ثابت Pair_قدر_رڪارڊ> (رڪارڊ, رڪارڊ_سائيز);
    }

    ٻاهريون->len = انگ;
    إذا (!انگ) c->serializer->revert (snap);
    return_trace (انگ);
  }
};


}
}
}

#جيڪڏهن_پورو  // OT_LAYOUT_GPOS_PAIRSET_HH
