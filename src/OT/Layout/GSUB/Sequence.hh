#که_نه_تعریف_شوی OT_LAYOUT_GSUB_SEQUENCE_HH
#تعریف_کړه OT_LAYOUT_GSUB_SEQUENCE_HH

#ضمن "Common.hh"

نطاق OT {
نطاق Layout {
نطاق GSUB_impl {

کينډۍ <ډول_نوم Types>
بنية Sequence
{
  محمي:
  Array16_د<ډول_نوم Types::HBGlyphID>
                substitute;             /* String of GlyphIDs to substitute */
  عام:
  DEFINE_اندازه_ARRAY (2, substitute);

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (substitute.sanitize (c));
  }

  منطقي intersects (ثابت hb_سېټ_t *glyphs) ثابت
  { أرجع hb_all (substitute, glyphs); }

  فراغ تړون (hb_تړون_context_t *c) ثابت
  { c->output->جمع_کړه_array (substitute.arrayZ, substitute.len); }

  فراغ depend (hb_depend_context_t *c, hb_codepoint_t سرچینه) ثابت
  {
    + hb_iter (substitute)
    | hb_apply ([&] (ثابت hb_codepoint_t &target) { c->depend_معلومات->جمع_کړه_gsub_lookup (سرچینه, c->lookup_شاخص, target); })
    ;
  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  { c->output->جمع_کړه_array (substitute.arrayZ, substitute.len); }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);
    بینښه صحيح شمېر = substitute.len;

    /* Special-case to make it in-place and not consider this
     * as a "multiplied" substitution. */
    إذا (unlikely (شمېر == 1))
    {
      إذا (HB_بفر_پیغام_نور && c->بفر->messaging ())
      {
	c->بفر->sync_so_far ();
	c->بفر->پیغام (c->font,
			    "replacing glyph at %u (multiple substitution)",
			    c->بفر->idx);
      }

      c->replace_glyph (substitute.arrayZ[0]);

      إذا (HB_بفر_پیغام_نور && c->بفر->messaging ())
      {
	c->بفر->پیغام (c->font,
			    "replaced glyph at %u (multiple substitution)",
			    c->بفر->idx - 1u);
      }

      return_trace (ریښتیا);
    }
    /* Spec disallows this, but Uniscribe allows it.
     * https://github.com/harfbuzz/harfbuzz/issues/253 */
    وإلا إذا (unlikely (شمېر == 0))
    {
      إذا (HB_بفر_پیغام_نور && c->بفر->messaging ())
      {
	c->بفر->sync_so_far ();
	c->بفر->پیغام (c->font,
			    "deleting glyph at %u (multiple substitution)",
			    c->بفر->idx);
      }

      c->بفر->delete_glyph ();

      إذا (HB_بفر_پیغام_نور && c->بفر->messaging ())
      {
	c->بفر->sync_so_far ();
	c->بفر->پیغام (c->font,
			    "deleted glyph at %u (multiple substitution)",
			    c->بفر->idx);
      }

      return_trace (ریښتیا);
    }

    إذا (HB_بفر_پیغام_نور && c->بفر->messaging ())
    {
      c->بفر->sync_so_far ();
      c->بفر->پیغام (c->font,
			  "multiplying glyph at %u",
			  c->بفر->idx);
    }

    بینښه صحيح klass = _hb_glyph_info_is_ligature (&c->بفر->cur()) ?
                         HB_OT_LAYOUT_GLYPH_PROPS_بنسټ_GLYPH : 0;
    بینښه lig_id = _hb_glyph_info_get_lig_id (&c->بفر->cur());

    لكل (بینښه صحيح i = 0; i < شمېر; i++)
    {
      /* If is attached to a ligature, don't disturb that.
       * https://github.com/harfbuzz/harfbuzz/issues/3069 */
      إذا (!lig_id)
        hb_glyph_info_سېټ_lig_props_for_component (&c->بفر->cur(), i);
      c->output_glyph_for_component (substitute.arrayZ[i], klass);
    }
    c->بفر->skip_glyph ();

    إذا (HB_بفر_پیغام_نور && c->بفر->messaging ())
    {
      c->بفر->sync_so_far ();

      حرف buf[HB_MAX_CONTEXT_اوږدتیا * 16] = {0};
      حرف *p = buf;

      لكل (بینښه i = c->بفر->idx - شمېر; i < c->بفر->idx; i++)
      {
	إذا (buf < p && حجم(buf) - 1u > بینښه (p - buf))
	  *p++ = ',';
	snprintf (p, حجم(buf) - (p - buf), "%u", i);
	p += مزي_اوږدوالی(p);
      }

      c->بفر->پیغام (c->font,
			  "multiplied glyphs at %s",
			  buf);
    }

    return_trace (ریښتیا);
  }

  کينډۍ <ډول_نوم Iterator,
            hb_requires (hb_is_سرچینه_د (Iterator, hb_codepoint_t))>
  منطقي serialize (hb_serialize_context_t *c,
                  Iterator subst)
  {
    TRACE_SERIALIZE (هذا);
    return_trace (substitute.serialize (c, subst));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_سېټ_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    إذا (!intersects (&glyphset)) return_trace (خطأ);

    خپلکاری it =
    + hb_iter (substitute)
    | hb_map (glyph_map)
    ;

    خپلکاری *بهر = c->serializer->پیل_embed (*هذا);
    return_trace (بهر->serialize (c->serializer, it));
  }
};


}
}
}


#د_که_پای /* OT_LAYOUT_GSUB_SEQUENCE_HH */
