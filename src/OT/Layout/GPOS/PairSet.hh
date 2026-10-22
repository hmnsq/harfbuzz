#اگر_تعریف_نشده OT_LAYOUT_GPOS_PAIRSET_HH
#تعریف_کن OT_LAYOUT_GPOS_PAIRSET_HH

#ضمن "PairValueRecord.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {


/* Pair records vary in glyph width; their value traversal need not. */
بنية pair_مجموعه_مقادیر_t
{
  ثابت مقدار_پایه *پایه;
  glyph_داده_t glyphs;
  ثابت مقدار *values;
  بیعلامت شمار;
  بیعلامت رکورد_اندازه;

  بیعلامت get_glyph (بیعلامت i) ثابت
  {
    glyph_داده_t glyph = glyphs;
    glyph.داده = (ثابت حرف *) glyph.داده + i * رکورد_اندازه;
    أرجع glyph[0];
  }

  ثابت مقدار *get_مقادیر (بیعلامت i) ثابت
  { أرجع reinterpret_cast<ثابت مقدار *> ((ثابت حرف *) values + i * رکورد_اندازه); }

  hb_pair_t<بیعلامت, بیعلامت>
  compute_effective_مقدار_formats (ثابت hb_مجموعه_t &glyphset,
                                   ثابت مقدار_Format مقدار_Formats[2],
                                   منطقي strip_hints, منطقي strip_خالی,
                                   ثابت hb_hashmap_t<بیعلامت, hb_pair_t<بیعلامت, صحيح>> *varidx_delta_map) ثابت
  {
    بیعلامت len1 = مقدار_Formats[0].get_len ();
    بیعلامت format1 = 0, format2 = 0;
    لكل (بیعلامت i = 0; i < شمار; i++)
    {
      إذا (!glyphset.has (get_glyph (i))) استمر;
      ثابت مقدار *v = get_مقادیر (i);
      format1 |= مقدار_Formats[0].get_effective_format (v, strip_hints, strip_خالی, پایه, varidx_delta_map);
      format2 |= مقدار_Formats[1].get_effective_format (v + len1, strip_hints, strip_خالی, پایه, varidx_delta_map);
    }
    أرجع hb_pair (format1, format2);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ثابت مقدار_Format مقدار_Formats[2]) ثابت
  {
    بیعلامت len1 = مقدار_Formats[0].get_len ();
    بیعلامت len2 = مقدار_Formats[1].get_len ();
    لكل (بیعلامت i = 0; i < شمار; i++)
    {
      إذا (!c->glyph_مجموعه->has (get_glyph (i))) استمر;
      ثابت hb_array_t<ثابت مقدار> v = hb_array (get_مقادیر (i), len1 + len2);
      إذا (مقدار_Formats[0].has_device ())
        مقدار_Formats[0].collect_variation_indices (c, پایه, v.sub_array (0, len1));
      إذا (مقدار_Formats[1].has_device ())
        مقدار_Formats[1].collect_variation_indices (c, پایه, v.sub_array (len1, len2));
    }
  }
};

قالب <نام_نوع Types>
بنية Pair_مجموعه : مقدار_پایه
{
  قالب <نام_نوع Types2>
  دوست بنية PairPosFormat1_3;

  استفاده Pair_مقدار_رکورد = GPOS_impl::Pair_مقدار_رکورد<Types>;

  محمي:
  نام_نوع Types::HBUINT
                        len;    /* Number of PairValueRecords */
  Pair_مقدار_رکورد       اولی_Pair_مقدار_رکورد;
                                /* Array of PairValueRecords--ordered
                                 * by GlyphID of the second glyph */
  عام:
  DEFINE_اندازه_MIN (Types::HBUINT::static_اندازه);

  ساكن size_t get_اندازه (بیعلامت len1, بیعلامت len2)
  {
    أرجع Types::HBGlyphID::static_اندازه + مقدار::static_اندازه * (len1 + len2);
  }
  ساكن size_t get_اندازه (ثابت مقدار_Format مقدار_Formats[2])
  {
    بیعلامت len1 = مقدار_Formats[0].get_len ();
    بیعلامت len2 = مقدار_Formats[1].get_len ();
    أرجع get_اندازه (len1, len2);
  }

  بنية sanitize_بستگی_t
  {
    ثابت مقدار_Format *مقدار_Formats;
    بیعلامت صحيح len1; /* valueFormats[0].get_len() */
    size_t stride; /*بایتها*/
  };

  منطقي sanitize (hb_sanitize_context_t *c, ثابت sanitize_بستگی_t *بستگی) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (!(c->check_struct (هذا) &&
	  hb_barrier () &&
          c->check_range (&اولی_Pair_مقدار_رکورد,
                          len,
                          بستگی->stride))) return_trace (خطأ);
    hb_barrier ();

    بیعلامت صحيح شمار = len;
    ثابت Pair_مقدار_رکورد *رکورد = &اولی_Pair_مقدار_رکورد;
    return_trace (c->lazy_some_gpos ||
		  (بستگی->مقدار_Formats[0].sanitize_مقادیر_stride_unsafe (c, هذا, &رکورد->values[0], شمار, بستگی->stride) &&
                   بستگی->مقدار_Formats[1].sanitize_مقادیر_stride_unsafe (c, هذا, &رکورد->values[بستگی->len1], شمار, بستگی->stride)));
  }

  منطقي intersects (ثابت hb_مجموعه_t *glyphs,
                   ثابت مقدار_Format *مقدار_Formats) ثابت
  {
    بیعلامت رکورد_اندازه = get_اندازه (مقدار_Formats);

    ثابت Pair_مقدار_رکورد *رکورد = &اولی_Pair_مقدار_رکورد;
    بیعلامت صحيح شمار = len;
    لكل (بیعلامت صحيح i = 0; i < شمار; i++)
    {
      إذا (glyphs->has (رکورد->دومی_Glyph))
        أرجع درست;
      رکورد = &Struct_At_جابه_جایی<ثابت Pair_مقدار_رکورد> (رکورد, رکورد_اندازه);
    }
    أرجع خطأ;
  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c,
                       ثابت مقدار_Format *مقدار_Formats) ثابت
  {
    بیعلامت رکورد_اندازه = get_اندازه (مقدار_Formats);

    ثابت Pair_مقدار_رکورد *رکورد = &اولی_Pair_مقدار_رکورد;
    c->درون_داد->جمع_کن_array (&رکورد->دومی_Glyph, len, رکورد_اندازه);
  }

  قالب <نام_نوع مجموعه_t>
  فراغ collect_دومی_glyphs (مجموعه_t *glyphs,
			      ثابت مقدار_Format *مقدار_Formats) ثابت
  {
    بیعلامت رکورد_اندازه = get_اندازه (مقدار_Formats);
    ثابت Pair_مقدار_رکورد *رکورد = &اولی_Pair_مقدار_رکورد;
    glyphs->جمع_کن_array (&رکورد->دومی_Glyph, len, رکورد_اندازه);
  }

  pair_مجموعه_مقادیر_t get_مقادیر_view (ثابت مقدار_Format مقدار_Formats[2]) ثابت
  {
    أرجع {هذا, &اولی_Pair_مقدار_رکورد.دومی_Glyph, &اولی_Pair_مقدار_رکورد.values[0],
            (بیعلامت) len, (بیعلامت) get_اندازه (مقدار_Formats)};
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ثابت مقدار_Format *مقدار_Formats) ثابت
  {
    get_مقادیر_view (مقدار_Formats).collect_variation_indices (c, مقدار_Formats);
  }

  منطقي apply (hb_ot_apply_context_t *c,
              ثابت مقدار_Format *مقدار_Formats,
              بیعلامت صحيح pos) ثابت
  {
    TRACE_APPLY (هذا);
    hb_میانگیر_t *میانگیر = c->میانگیر;
    بیعلامت صحيح len1 = مقدار_Formats[0].get_len ();
    بیعلامت صحيح len2 = مقدار_Formats[1].get_len ();
    بیعلامت رکورد_اندازه = get_اندازه (len1, len2);

    ثابت Pair_مقدار_رکورد *رکورد = hb_bsearch (میانگیر->info[pos].codepoint,
                                                &اولی_Pair_مقدار_رکورد,
                                                (بیعلامت) len,
                                                رکورد_اندازه);
    إذا (رکورد)
    {
      إذا (HB_میانگیر_پیام_بیشتر && c->میانگیر->messaging ())
      {
	c->میانگیر->پیام (c->font,
			    "try kerning glyphs at %u,%u",
			    c->میانگیر->idx, pos);
      }

      منطقي applied_اولی = len1 && مقدار_Formats[0].apply_مقدار (c, هذا, &رکورد->values[0], میانگیر->cur_pos());
      منطقي applied_دومی = len2 && مقدار_Formats[1].apply_مقدار (c, هذا, &رکورد->values[len1], میانگیر->pos[pos]);

      إذا (applied_اولی || applied_دومی)
	إذا (HB_میانگیر_پیام_بیشتر && c->میانگیر->messaging ())
	{
	  c->میانگیر->پیام (c->font,
			      "kerned glyphs at %u,%u",
			      c->میانگیر->idx, pos);
	}

      إذا (HB_میانگیر_پیام_بیشتر && c->میانگیر->messaging ())
      {
	c->میانگیر->پیام (c->font,
			    "tried kerning glyphs at %u,%u",
			    c->میانگیر->idx, pos);
      }

      إذا (applied_اولی || applied_دومی)
        میانگیر->unsafe_مقصد_break (میانگیر->idx, pos + 1);
      وإلا
        /* Even a zero-valued pair record is a concat hazard. */
        میانگیر->unsafe_مقصد_concat (میانگیر->idx, pos + 1);

      إذا (len2)
      {
	pos++;
      // https://github.com/harfbuzz/harfbuzz/issues/3824
      // https://github.com/harfbuzz/harfbuzz/issues/3888#issuecomment-1326781116
      میانگیر->unsafe_مقصد_break (میانگیر->idx, pos + 1);
      }

      میانگیر->idx = pos;
      return_trace (درست);
    }
    میانگیر->unsafe_مقصد_concat (میانگیر->idx, pos + 1);
    return_trace (خطأ);
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت مقدار_Format مقدار_Formats[2],
               ثابت مقدار_Format newFormats[2]) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار snap = c->serializer->snapshot ();

    خودکار *بیرون = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (بیرون))) return_trace (خطأ);
    بیرون->len = 0;

    ثابت hb_مجموعه_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    بیعلامت len1 = مقدار_Formats[0].get_len ();
    بیعلامت len2 = مقدار_Formats[1].get_len ();
    بیعلامت رکورد_اندازه = get_اندازه (len1, len2);

    pair_مقدار_رکورد_context_t context =
    {
      هذا,
      مقدار_Formats,
      newFormats,
      len1,
      &glyph_map,
      &c->plan->layout_variation_idx_delta_map
    };

    ثابت Pair_مقدار_رکورد *رکورد = &اولی_Pair_مقدار_رکورد;
    بیعلامت شمار = len, رقم = 0;
    لكل (بیعلامت i = 0; i < شمار; i++)
    {
      إذا (glyphset.has (رکورد->دومی_Glyph)
         && رکورد->subset (c, &context)) رقم++;
      رکورد = &Struct_At_جابه_جایی<ثابت Pair_مقدار_رکورد> (رکورد, رکورد_اندازه);
    }

    بیرون->len = رقم;
    إذا (!رقم) c->serializer->revert (snap);
    return_trace (رقم);
  }
};


}
}
}

#پایان_اگر  // OT_LAYOUT_GPOS_PAIRSET_HH
