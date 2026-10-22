#اگر_تعریف_نشده OT_LAYOUT_GSUB_MULTIPLESUBSTFORMAT1_HH
#تعریف_کن OT_LAYOUT_GSUB_MULTIPLESUBSTFORMAT1_HH

#ضمن "Common.hh"
#ضمن "Sequence.hh"

نطاق OT {
نطاق Layout {
نطاق GSUB_impl {

قالب <نام_نوع Types>
بنية مضرب_Subst_Format1_2
{
  محمي:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  نام_نوع Types::قالب LOffset_مقصد<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  نام_نوع Types::قالب Array_از<نام_نوع Types::قالب جابه_جایی_مقصد<Sequence<Types>>>
                sequence;               /* Array of Sequence tables
                                         * ordered by Coverage Index */
  عام:
  DEFINE_اندازه_ARRAY (2 + Types::LOffset::static_اندازه + Types::HBUINT::static_اندازه, sequence);

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (coverage.sanitize (c, هذا) && sequence.sanitize (c, هذا));
  }

  منطقي intersects (ثابت hb_مجموعه_t *glyphs) ثابت
  { أرجع (هذا+coverage).intersects (glyphs); }

  منطقي may_have_non_1to1 () ثابت
  { أرجع درست; }

  فراغ بستگی (hb_بستگی_context_t *c) ثابت
  {
    + hb_zip (هذا+coverage, sequence)
    | hb_filter (c->والد_فعال_glyphs (), hb_اولی)
    | hb_map (hb_دومی)
    | hb_map (hb_جمع_کن (هذا))
    | hb_apply ([c] (ثابت Sequence<Types> &_) { _.بستگی (c); })
    ;
  }

  فراغ depend (hb_depend_context_t *c) ثابت
  {
    // Filter by parent_active_glyphs like closure does
    + hb_zip (هذا+coverage, sequence)
    | hb_filter (c->والد_فعال_glyphs (), hb_اولی)
    | hb_apply ([&] (ثابت hb_pair_t<hb_codepoint_t, ثابت نام_نوع Types::قالب جابه_جایی_مقصد<Sequence<Types>>&> &_)
                {
                  ثابت Sequence<Types>& s = هذا+_.second;
                  s.depend (c, _.اولی);
                })
    ;
  }

  فراغ بستگی_lookups (hb_بستگی_lookups_context_t *c) ثابت {}

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    إذا (unlikely (!(هذا+coverage).collect_coverage (c->درون_داد))) أرجع;
    + hb_zip (هذا+coverage, sequence)
    | hb_map (hb_دومی)
    | hb_map (hb_جمع_کن (هذا))
    | hb_apply ([c] (ثابت Sequence<Types> &_) { _.collect_glyphs (c); })
    ;
  }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+coverage; }

  منطقي would_apply (hb_would_apply_context_t *c) ثابت
  { أرجع c->len == 1 && (هذا+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);

    بیعلامت صحيح شاخص_2 = (هذا+coverage).get_coverage (c->میانگیر->cur().codepoint);
    إذا (شاخص_2 == NOT_COVERED) return_trace (خطأ);

    return_trace ((هذا+sequence[شاخص_2]).apply (c));
  }

  قالب<نام_نوع Iterator,
           hb_requires (hb_is_sorted_iterator (Iterator))>
  منطقي serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    TRACE_SERIALIZE (هذا);
    خودکار sequences =
      + it
      | hb_map (hb_دومی)
      ;
    خودکار glyphs =
      + it
      | hb_map_retains_ترتیبدهی (hb_اولی)
      ;
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);

    إذا (unlikely (!sequence.serialize (c, sequences.درازی))) return_trace (خطأ);

    لكل (خودکار& pair : hb_zip (sequences, sequence))
    {
      إذا (unlikely (!pair.second
		    .serialize_serialize (c, pair.اولی)))
        return_trace (خطأ);
    }

    return_trace (coverage.serialize_serialize (c, glyphs));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_مجموعه_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خودکار *بیرون = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (بیرون))) return_trace (خطأ);
    بیرون->format = format;

    hb_sorted_وکتور_t<hb_codepoint_t> new_coverage;
    + hb_zip (هذا+coverage, sequence)
    | hb_filter (glyphset, hb_اولی)
    | hb_filter (subset_جابه_جایی_array (c, بیرون->sequence, هذا), hb_دومی)
    | hb_map (hb_اولی)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;
    بیرون->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (منطقي (new_coverage));
  }
};

}
}
}


#پایان_اگر /* OT_LAYOUT_GSUB_MULTIPLESUBSTFORMAT1_HH */
