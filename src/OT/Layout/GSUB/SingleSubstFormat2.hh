#اگر_غیر_تعریف_شدہ OT_LAYOUT_GSUB_SINGLESUBSTFORMAT2_HH
#تعریف_کرو OT_LAYOUT_GSUB_SINGLESUBSTFORMAT2_HH

#شامل "Common.hh"

نامخلا OT {
نامخلا Layout {
نامخلا GSUB_impl {

سانچہ <قسم_نام Types>
ساخت SingleSubstFormat2_4
{
  محفوظ:
  HBUINT16      format;                 /* Format identifier--format = 2 */
  قسم_نام Types::سانچہ LOffset_مقصد<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  قسم_نام Types::سانچہ Array_کا<قسم_نام Types::HBGlyphID>
                substitute;             /* Array of substitute
                                         * GlyphIDs--ordered by Coverage Index */

  عوامی:
  DEFINE_سائز_ARRAY (2 + Types::LOffset::static_سائز + Types::HBUINT::static_سائز, substitute);

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (coverage.sanitize (c, یہ) && substitute.sanitize (c));
  }

  منطقی intersects (ثابت hb_سیٹ_t *glyphs) ثابت
  { واپس (یہ+coverage).intersects (glyphs); }

  منطقی may_have_non_1to1 () ثابت
  { واپس جھوٹ; }

  خالی depend (hb_depend_context_t *c) ثابت
  {
    خودکار &cov = یہ+coverage;
    خودکار &glyph_سیٹ = c->والد_فعال_glyphs ();

    // Filter by active glyphs like closure does
    اگر (substitute.len > glyph_سیٹ.get_population () * 4)
    {
      برائے (خودکار g : glyph_سیٹ)
      {
	غیرعلامتی i = cov.get_coverage (g);
	اگر (i == NOT_COVERED || i >= substitute.len)
	  جاری;
	c->depend_ڈیٹا->جوڑو_gsub_lookup (g, c->lookup_اشاریہ, substitute.arrayZ[i]);
      }

      واپس;
    }

    + hb_zip (cov, substitute)
    | hb_filter (glyph_سیٹ, hb_پہلا)
    | hb_apply ([&] (ثابت hb_codepoint_pair_t &_) { c->depend_ڈیٹا->جوڑو_gsub_lookup (_.پہلا, c->lookup_اشاریہ, _.second); })
    ;
  }

  خالی احاطہ (hb_احاطہ_context_t *c) ثابت
  {
    خودکار &cov = یہ+coverage;
    خودکار &glyph_سیٹ = c->والد_فعال_glyphs ();

    اگر (substitute.len > glyph_سیٹ.get_population () * 4)
    {
      برائے (خودکار g : glyph_سیٹ)
      {
	غیرعلامتی i = cov.get_coverage (g);
	اگر (i == NOT_COVERED || i >= substitute.len)
	  جاری;
	c->output->جوڑو (substitute.arrayZ[i]);
      }

      واپس;
    }

    + hb_zip (cov, substitute)
    | hb_filter (glyph_سیٹ, hb_پہلا)
    | hb_map (hb_دوسرا)
    | hb_sink (c->output)
    ;
  }

  خالی احاطہ_lookups (hb_احاطہ_lookups_context_t *c) ثابت {}

  خالی collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    اگر (unlikely (!(یہ+coverage).collect_coverage (c->داخلہ))) واپس;
    + hb_zip (یہ+coverage, substitute)
    | hb_map (hb_دوسرا)
    | hb_sink (c->output)
    ;
  }

  ثابت Coverage &get_coverage () ثابت { واپس یہ+coverage; }

  منطقی would_apply (hb_would_apply_context_t *c) ثابت
  { واپس c->len == 1 && (یہ+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  غیرعلامتی
  get_glyph_alternates (hb_codepoint_t  glyph_id,
                        غیرعلامتی        آغاز_انحراف,
                        غیرعلامتی       *alternate_گنتی  /* IN/OUT.  May be NULL. */,
                        hb_codepoint_t *alternate_glyphs /* OUT.     May be NULL. */) ثابت
  {
    غیرعلامتی صحیح اشاریہ_2 = (یہ+coverage).get_coverage (glyph_id);
    اگر (likely (اشاریہ_2 == NOT_COVERED))
    {
      اگر (alternate_گنتی)
        *alternate_گنتی = 0;
      واپس 0;
    }

    اگر (alternate_گنتی && *alternate_گنتی && alternate_glyphs)
    {
      glyph_id = substitute[اشاریہ_2];

      *alternate_glyphs = glyph_id;
      *alternate_گنتی = 1;
    }

    واپس 1;
  }

  خالی
  collect_glyph_alternates (hb_map_t  *alternate_گنتی /* IN/OUT */,
			    hb_map_t  *alternate_glyphs /* IN/OUT */) ثابت
  {
    + hb_zip (یہ+coverage, substitute)
    | hb_apply ([&] (ثابت hb_pair_t<hb_codepoint_t, hb_codepoint_t> &p) -> خالی
		{ hb_collect_glyph_alternates_جوڑو (p.پہلا, p.second,
						    alternate_گنتی, alternate_glyphs); })
    ;
  }

  منطقی apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (یہ);
    غیرعلامتی صحیح اشاریہ_2 = (یہ+coverage).get_coverage (c->بفر->cur().codepoint);
    اگر (اشاریہ_2 == NOT_COVERED) return_trace (جھوٹ);

    اگر (unlikely (اشاریہ_2 >= substitute.len)) return_trace (جھوٹ);

    اگر (HB_بفر_پیغام_مزید && c->بفر->messaging ())
    {
      c->بفر->sync_so_far ();
      c->بفر->پیغام (c->font,
			  "replacing glyph at %u (single substitution)",
			  c->بفر->idx);
    }

    c->replace_glyph (substitute[اشاریہ_2]);

    اگر (HB_بفر_پیغام_مزید && c->بفر->messaging ())
    {
      c->بفر->پیغام (c->font,
			  "replaced glyph at %u (single substitution)",
			  c->بفر->idx - 1u);
    }

    return_trace (سچ);
  }

  سانچہ<قسم_نام Iterator,
           hb_requires (hb_is_sorted_ماخذ_کا (Iterator,
                                                hb_codepoint_pair_t))>
  منطقی serialize (hb_serialize_context_t *c,
                  Iterator it)
  {
    TRACE_SERIALIZE (یہ);
    خودکار substitutes =
      + it
      | hb_map (hb_دوسرا)
      ;
    خودکار glyphs =
      + it
      | hb_map_retains_ترتیب_کاری (hb_پہلا)
      ;
    اگر (unlikely (!c->extend_min (یہ))) return_trace (جھوٹ);
    اگر (unlikely (!substitute.serialize (c, substitutes))) return_trace (جھوٹ);
    اگر (unlikely (!coverage.serialize_serialize (c, glyphs))) return_trace (جھوٹ);
    return_trace (سچ);
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);
    ثابت hb_سیٹ_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خودکار it =
    + hb_zip (یہ+coverage, substitute)
    | hb_filter (glyphset, hb_پہلا)
    | hb_filter (glyphset, hb_دوسرا)
    | hb_map_retains_ترتیب_کاری ([&] (hb_pair_t<hb_codepoint_t, ثابت قسم_نام Types::HBGlyphID &> p) -> hb_codepoint_pair_t
                              { واپس hb_pair (glyph_map[p.پہلا], glyph_map[p.second]); })
    ;

    منطقی ret = منطقی (it);
    SingleSubst_serialize (c->serializer, it);
    return_trace (ret);
  }
};

}
}
}

#اگر_ختم /* OT_LAYOUT_GSUB_SINGLESUBSTFORMAT2_HH */
