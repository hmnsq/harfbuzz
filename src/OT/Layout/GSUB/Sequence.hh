#جے_بنا_تعریف OT_LAYOUT_GSUB_SEQUENCE_HH
#تعریف_کرو OT_LAYOUT_GSUB_SEQUENCE_HH

#ضمن "Common.hh"

نطاق OT {
نطاق Layout {
نطاق GSUB_impl {

سانچا <قسم_ناں Types>
بنية Sequence
{
  محمي:
  Array16_دا<قسم_ناں Types::HBGlyphID>
                substitute;             /* String of GlyphIDs to substitute */
  عام:
  DEFINE_ناپ_ARRAY (2, substitute);

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (substitute.sanitize (c));
  }

  منطقي intersects (ثابت hb_سیٹ_t *glyphs) ثابت
  { أرجع hb_all (substitute, glyphs); }

  فراغ گھیرا (hb_گھیرا_context_t *c) ثابت
  { c->output->جوڑو_array (substitute.arrayZ, substitute.len); }

  فراغ depend (hb_depend_context_t *c, hb_codepoint_t سروت) ثابت
  {
    + hb_iter (substitute)
    | hb_apply ([&] (ثابت hb_codepoint_t &target) { c->depend_انکڑے->جوڑو_gsub_lookup (سروت, c->lookup_اشاریہ, target); })
    ;
  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  { c->output->جوڑو_array (substitute.arrayZ, substitute.len); }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);
    بنانشانی صحيح گنتی = substitute.len;

    /* Special-case to make it in-place and not consider this
     * as a "multiplied" substitution. */
    إذا (unlikely (گنتی == 1))
    {
      إذا (HB_بفر_سنیہا_ہور && c->بفر->messaging ())
      {
	c->بفر->sync_so_far ();
	c->بفر->سنیہا (c->font,
			    "replacing glyph at %u (multiple substitution)",
			    c->بفر->idx);
      }

      c->replace_glyph (substitute.arrayZ[0]);

      إذا (HB_بفر_سنیہا_ہور && c->بفر->messaging ())
      {
	c->بفر->سنیہا (c->font,
			    "replaced glyph at %u (multiple substitution)",
			    c->بفر->idx - 1u);
      }

      return_trace (سچ);
    }
    /* Spec disallows this, but Uniscribe allows it.
     * https://github.com/harfbuzz/harfbuzz/issues/253 */
    وإلا إذا (unlikely (گنتی == 0))
    {
      إذا (HB_بفر_سنیہا_ہور && c->بفر->messaging ())
      {
	c->بفر->sync_so_far ();
	c->بفر->سنیہا (c->font,
			    "deleting glyph at %u (multiple substitution)",
			    c->بفر->idx);
      }

      c->بفر->delete_glyph ();

      إذا (HB_بفر_سنیہا_ہور && c->بفر->messaging ())
      {
	c->بفر->sync_so_far ();
	c->بفر->سنیہا (c->font,
			    "deleted glyph at %u (multiple substitution)",
			    c->بفر->idx);
      }

      return_trace (سچ);
    }

    إذا (HB_بفر_سنیہا_ہور && c->بفر->messaging ())
    {
      c->بفر->sync_so_far ();
      c->بفر->سنیہا (c->font,
			  "multiplying glyph at %u",
			  c->بفر->idx);
    }

    بنانشانی صحيح klass = _hb_glyph_info_is_ligature (&c->بفر->cur()) ?
                         HB_OT_LAYOUT_GLYPH_PROPS_ادھار_GLYPH : 0;
    بنانشانی lig_id = _hb_glyph_info_get_lig_id (&c->بفر->cur());

    لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
    {
      /* If is attached to a ligature, don't disturb that.
       * https://github.com/harfbuzz/harfbuzz/issues/3069 */
      إذا (!lig_id)
        hb_glyph_info_سیٹ_lig_props_for_component (&c->بفر->cur(), i);
      c->output_glyph_for_component (substitute.arrayZ[i], klass);
    }
    c->بفر->skip_glyph ();

    إذا (HB_بفر_سنیہا_ہور && c->بفر->messaging ())
    {
      c->بفر->sync_so_far ();

      حرف buf[HB_MAX_CONTEXT_لمائی * 16] = {0};
      حرف *p = buf;

      لكل (بنانشانی i = c->بفر->idx - گنتی; i < c->بفر->idx; i++)
      {
	إذا (buf < p && حجم(buf) - 1u > بنانشانی (p - buf))
	  *p++ = ',';
	snprintf (p, حجم(buf) - (p - buf), "%u", i);
	p += لڑی_لمبائی(p);
      }

      c->بفر->سنیہا (c->font,
			  "multiplied glyphs at %s",
			  buf);
    }

    return_trace (سچ);
  }

  سانچا <قسم_ناں Iterator,
            hb_requires (hb_is_سروت_دا (Iterator, hb_codepoint_t))>
  منطقي serialize (hb_serialize_context_t *c,
                  Iterator subst)
  {
    TRACE_SERIALIZE (هذا);
    return_trace (substitute.serialize (c, subst));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_سیٹ_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    إذا (!intersects (&glyphset)) return_trace (خطأ);

    خودکار it =
    + hb_iter (substitute)
    | hb_map (glyph_map)
    ;

    خودکار *باہرلا = c->serializer->شروع_embed (*هذا);
    return_trace (باہرلا->serialize (c->serializer, it));
  }
};


}
}
}


#جے_انت /* OT_LAYOUT_GSUB_SEQUENCE_HH */
