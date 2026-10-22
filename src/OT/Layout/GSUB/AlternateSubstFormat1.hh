#اگر_غیر_تعریف_شدہ OT_LAYOUT_GSUB_ALTERNATESUBSTFORMAT1_HH
#تعریف_کرو OT_LAYOUT_GSUB_ALTERNATESUBSTFORMAT1_HH

#شامل "AlternateSet.hh"
#شامل "Common.hh"

نامخلا OT {
نامخلا Layout {
نامخلا GSUB_impl {

سانچہ <قسم_نام Types>
ساخت AlternateSubstFormat1_2
{
  محفوظ:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  قسم_نام Types::سانچہ LOffset_مقصد<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  قسم_نام Types::سانچہ Array_کا<قسم_نام Types::سانچہ انحراف_مقصد<Alternate_سیٹ<Types>>>
                alternate_سیٹ;           /* Array of AlternateSet tables
                                         * ordered by Coverage Index */
  عوامی:
  DEFINE_سائز_ARRAY (2 + Types::LOffset::static_سائز + Types::HBUINT::static_سائز, alternate_سیٹ);

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (coverage.sanitize (c, یہ) && alternate_سیٹ.sanitize (c, یہ));
  }

  منطقی intersects (ثابت hb_سیٹ_t *glyphs) ثابت
  { واپس (یہ+coverage).intersects (glyphs); }

  منطقی may_have_non_1to1 () ثابت
  { واپس جھوٹ; }

  خالی depend (hb_depend_context_t *c) ثابت
  {
    // Filter by parent_active_glyphs like closure does
    + hb_zip (یہ+coverage, alternate_سیٹ)
    | hb_filter (c->والد_فعال_glyphs (), hb_پہلا)
    | hb_apply ([&] (ثابت hb_pair_t<hb_codepoint_t, ثابت قسم_نام Types::سانچہ انحراف_مقصد<Alternate_سیٹ<Types>>&> &_)
                {
                  ثابت Alternate_سیٹ<Types>& as = یہ+_.second;
                  as.depend (c, _.پہلا);
                })
    ;
  }

  خالی احاطہ (hb_احاطہ_context_t *c) ثابت
  {
    + hb_zip (یہ+coverage, alternate_سیٹ)
    | hb_filter (c->والد_فعال_glyphs (), hb_پہلا)
    | hb_map (hb_دوسرا)
    | hb_map (hb_جوڑو (یہ))
    | hb_apply ([c] (ثابت Alternate_سیٹ<Types> &_) { _.احاطہ (c); })
    ;
  }

  خالی احاطہ_lookups (hb_احاطہ_lookups_context_t *c) ثابت {}

  خالی collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    اگر (unlikely (!(یہ+coverage).collect_coverage (c->داخلہ))) واپس;
    + hb_zip (یہ+coverage, alternate_سیٹ)
    | hb_map (hb_دوسرا)
    | hb_map (hb_جوڑو (یہ))
    | hb_apply ([c] (ثابت Alternate_سیٹ<Types> &_) { _.collect_glyphs (c); })
    ;
  }

  ثابت Coverage &get_coverage () ثابت { واپس یہ+coverage; }

  منطقی would_apply (hb_would_apply_context_t *c) ثابت
  { واپس c->len == 1 && (یہ+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  غیرعلامتی
  get_glyph_alternates (hb_codepoint_t  gid,
                        غیرعلامتی        آغاز_انحراف,
                        غیرعلامتی       *alternate_گنتی  /* IN/OUT.  May be NULL. */,
                        hb_codepoint_t *alternate_glyphs /* OUT.     May be NULL. */) ثابت
  { واپس (یہ+alternate_سیٹ[(یہ+coverage).get_coverage (gid)])
           .get_alternates (آغاز_انحراف, alternate_گنتی, alternate_glyphs); }

  خالی
  collect_glyph_alternates (hb_map_t  *alternate_گنتی /* IN/OUT */,
			    hb_map_t  *alternate_glyphs /* IN/OUT */) ثابت
  {
    + hb_iter (alternate_سیٹ)
    | hb_map (hb_جوڑو (یہ))
    | hb_zip (یہ+coverage)
    | hb_apply ([&] (ثابت hb_pair_t<ثابت Alternate_سیٹ<Types> &, hb_codepoint_t> _) {
		  _.پہلا.collect_alternates (_.second, alternate_گنتی, alternate_glyphs);
		})
    ;
  }

  منطقی apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (یہ);

    غیرعلامتی صحیح اشاریہ_2 = (یہ+coverage).get_coverage (c->بفر->cur().codepoint);
    اگر (اشاریہ_2 == NOT_COVERED) return_trace (جھوٹ);

    return_trace ((یہ+alternate_سیٹ[اشاریہ_2]).apply (c));
  }

  سانچہ <قسم_نام GlyphID>
  منطقی serialize (hb_serialize_context_t *c,
                  hb_sorted_array_t<GlyphID> glyphs,
                  hb_array_t<ثابت غیرعلامتی صحیح> alternate_len_list,
                  hb_array_t<GlyphID> alternate_glyphs_list)
  {
    TRACE_SERIALIZE (یہ);
    اگر (unlikely (!c->extend_min (یہ))) return_trace (جھوٹ);
    اگر (unlikely (!alternate_سیٹ.serialize (c, glyphs.لمبائی))) return_trace (جھوٹ);
    برائے (غیرعلامتی صحیح i = 0; i < glyphs.لمبائی; i++)
    {
      غیرعلامتی صحیح alternate_len = alternate_len_list[i];
      اگر (unlikely (!alternate_سیٹ[i]
                        .serialize_serialize (c, alternate_glyphs_list.sub_array (0, alternate_len))))
        return_trace (جھوٹ);
      alternate_glyphs_list += alternate_len;
    }
    return_trace (coverage.serialize_serialize (c, glyphs));
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);
    ثابت hb_سیٹ_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خودکار *برآمد = c->serializer->آغاز_embed (*یہ);
    اگر (unlikely (!c->serializer->extend_min (برآمد))) return_trace (جھوٹ);
    برآمد->format = format;

    hb_sorted_سمتیہ_t<hb_codepoint_t> new_coverage;
    + hb_zip (یہ+coverage, alternate_سیٹ)
    | hb_filter (glyphset, hb_پہلا)
    | hb_filter (subset_انحراف_array (c, برآمد->alternate_سیٹ, یہ), hb_دوسرا)
    | hb_map (hb_پہلا)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;
    برآمد->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (منطقی (new_coverage));
  }
};

}
}
}

#اگر_ختم  /* OT_LAYOUT_GSUB_ALTERNATESUBSTFORMAT1_HH */
