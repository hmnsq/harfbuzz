#اگر_غیر_تعریف_شدہ OT_LAYOUT_GSUB_MULTIPLESUBSTFORMAT1_HH
#تعریف_کرو OT_LAYOUT_GSUB_MULTIPLESUBSTFORMAT1_HH

#شامل "Common.hh"
#شامل "Sequence.hh"

نامخلا OT {
نامخلا Layout {
نامخلا GSUB_impl {

سانچہ <قسم_نام Types>
ساخت مضرب_Subst_Format1_2
{
  محفوظ:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  قسم_نام Types::سانچہ LOffset_مقصد<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  قسم_نام Types::سانچہ Array_کا<قسم_نام Types::سانچہ انحراف_مقصد<Sequence<Types>>>
                sequence;               /* Array of Sequence tables
                                         * ordered by Coverage Index */
  عوامی:
  DEFINE_سائز_ARRAY (2 + Types::LOffset::static_سائز + Types::HBUINT::static_سائز, sequence);

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (coverage.sanitize (c, یہ) && sequence.sanitize (c, یہ));
  }

  منطقی intersects (ثابت hb_سیٹ_t *glyphs) ثابت
  { واپس (یہ+coverage).intersects (glyphs); }

  منطقی may_have_non_1to1 () ثابت
  { واپس سچ; }

  خالی احاطہ (hb_احاطہ_context_t *c) ثابت
  {
    + hb_zip (یہ+coverage, sequence)
    | hb_filter (c->والد_فعال_glyphs (), hb_پہلا)
    | hb_map (hb_دوسرا)
    | hb_map (hb_جوڑو (یہ))
    | hb_apply ([c] (ثابت Sequence<Types> &_) { _.احاطہ (c); })
    ;
  }

  خالی depend (hb_depend_context_t *c) ثابت
  {
    // Filter by parent_active_glyphs like closure does
    + hb_zip (یہ+coverage, sequence)
    | hb_filter (c->والد_فعال_glyphs (), hb_پہلا)
    | hb_apply ([&] (ثابت hb_pair_t<hb_codepoint_t, ثابت قسم_نام Types::سانچہ انحراف_مقصد<Sequence<Types>>&> &_)
                {
                  ثابت Sequence<Types>& s = یہ+_.second;
                  s.depend (c, _.پہلا);
                })
    ;
  }

  خالی احاطہ_lookups (hb_احاطہ_lookups_context_t *c) ثابت {}

  خالی collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    اگر (unlikely (!(یہ+coverage).collect_coverage (c->داخلہ))) واپس;
    + hb_zip (یہ+coverage, sequence)
    | hb_map (hb_دوسرا)
    | hb_map (hb_جوڑو (یہ))
    | hb_apply ([c] (ثابت Sequence<Types> &_) { _.collect_glyphs (c); })
    ;
  }

  ثابت Coverage &get_coverage () ثابت { واپس یہ+coverage; }

  منطقی would_apply (hb_would_apply_context_t *c) ثابت
  { واپس c->len == 1 && (یہ+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  منطقی apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (یہ);

    غیرعلامتی صحیح اشاریہ_2 = (یہ+coverage).get_coverage (c->بفر->cur().codepoint);
    اگر (اشاریہ_2 == NOT_COVERED) return_trace (جھوٹ);

    return_trace ((یہ+sequence[اشاریہ_2]).apply (c));
  }

  سانچہ<قسم_نام Iterator,
           hb_requires (hb_is_sorted_iterator (Iterator))>
  منطقی serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    TRACE_SERIALIZE (یہ);
    خودکار sequences =
      + it
      | hb_map (hb_دوسرا)
      ;
    خودکار glyphs =
      + it
      | hb_map_retains_ترتیب_کاری (hb_پہلا)
      ;
    اگر (unlikely (!c->extend_min (یہ))) return_trace (جھوٹ);

    اگر (unlikely (!sequence.serialize (c, sequences.لمبائی))) return_trace (جھوٹ);

    برائے (خودکار& pair : hb_zip (sequences, sequence))
    {
      اگر (unlikely (!pair.second
		    .serialize_serialize (c, pair.پہلا)))
        return_trace (جھوٹ);
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
    + hb_zip (یہ+coverage, sequence)
    | hb_filter (glyphset, hb_پہلا)
    | hb_filter (subset_انحراف_array (c, برآمد->sequence, یہ), hb_دوسرا)
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


#اگر_ختم /* OT_LAYOUT_GSUB_MULTIPLESUBSTFORMAT1_HH */
