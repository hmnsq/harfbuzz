#اگر_غیر_تعریف_شدہ OT_LAYOUT_GSUB_SEQUENCE_HH
#تعریف_کرو OT_LAYOUT_GSUB_SEQUENCE_HH

#شامل "Common.hh"

نامخلا OT {
نامخلا Layout {
نامخلا GSUB_impl {

سانچہ <قسم_نام Types>
ساخت Sequence
{
  محفوظ:
  Array16_کا<قسم_نام Types::HBGlyphID>
                substitute;             /* String of GlyphIDs to substitute */
  عوامی:
  DEFINE_سائز_ARRAY (2, substitute);

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (substitute.sanitize (c));
  }

  منطقی intersects (ثابت hb_سیٹ_t *glyphs) ثابت
  { واپس hb_all (substitute, glyphs); }

  خالی احاطہ (hb_احاطہ_context_t *c) ثابت
  { c->output->جوڑو_array (substitute.arrayZ, substitute.len); }

  خالی depend (hb_depend_context_t *c, hb_codepoint_t ماخذ) ثابت
  {
    + hb_iter (substitute)
    | hb_apply ([&] (ثابت hb_codepoint_t &target) { c->depend_ڈیٹا->جوڑو_gsub_lookup (ماخذ, c->lookup_اشاریہ, target); })
    ;
  }

  خالی collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  { c->output->جوڑو_array (substitute.arrayZ, substitute.len); }

  منطقی apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (یہ);
    غیرعلامتی صحیح گنتی = substitute.len;

    /* Special-case to make it in-place and not consider this
     * as a "multiplied" substitution. */
    اگر (unlikely (گنتی == 1))
    {
      اگر (HB_بفر_پیغام_مزید && c->بفر->messaging ())
      {
	c->بفر->sync_so_far ();
	c->بفر->پیغام (c->font,
			    "replacing glyph at %u (multiple substitution)",
			    c->بفر->idx);
      }

      c->replace_glyph (substitute.arrayZ[0]);

      اگر (HB_بفر_پیغام_مزید && c->بفر->messaging ())
      {
	c->بفر->پیغام (c->font,
			    "replaced glyph at %u (multiple substitution)",
			    c->بفر->idx - 1u);
      }

      return_trace (سچ);
    }
    /* Spec disallows this, but Uniscribe allows it.
     * https://github.com/harfbuzz/harfbuzz/issues/253 */
    ورنہ اگر (unlikely (گنتی == 0))
    {
      اگر (HB_بفر_پیغام_مزید && c->بفر->messaging ())
      {
	c->بفر->sync_so_far ();
	c->بفر->پیغام (c->font,
			    "deleting glyph at %u (multiple substitution)",
			    c->بفر->idx);
      }

      c->بفر->delete_glyph ();

      اگر (HB_بفر_پیغام_مزید && c->بفر->messaging ())
      {
	c->بفر->sync_so_far ();
	c->بفر->پیغام (c->font,
			    "deleted glyph at %u (multiple substitution)",
			    c->بفر->idx);
      }

      return_trace (سچ);
    }

    اگر (HB_بفر_پیغام_مزید && c->بفر->messaging ())
    {
      c->بفر->sync_so_far ();
      c->بفر->پیغام (c->font,
			  "multiplying glyph at %u",
			  c->بفر->idx);
    }

    غیرعلامتی صحیح klass = _hb_glyph_info_is_ligature (&c->بفر->cur()) ?
                         HB_OT_LAYOUT_GLYPH_PROPS_بنیاد_GLYPH : 0;
    غیرعلامتی lig_id = _hb_glyph_info_get_lig_id (&c->بفر->cur());

    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
    {
      /* If is attached to a ligature, don't disturb that.
       * https://github.com/harfbuzz/harfbuzz/issues/3069 */
      اگر (!lig_id)
        hb_glyph_info_سیٹ_lig_props_for_component (&c->بفر->cur(), i);
      c->output_glyph_for_component (substitute.arrayZ[i], klass);
    }
    c->بفر->skip_glyph ();

    اگر (HB_بفر_پیغام_مزید && c->بفر->messaging ())
    {
      c->بفر->sync_so_far ();

      حرف buf[HB_MAX_CONTEXT_لمبائی * 16] = {0};
      حرف *p = buf;

      برائے (غیرعلامتی i = c->بفر->idx - گنتی; i < c->بفر->idx; i++)
      {
	اگر (buf < p && حجم(buf) - 1u > غیرعلامتی (p - buf))
	  *p++ = ',';
	snprintf (p, حجم(buf) - (p - buf), "%u", i);
	p += لڑی_لمبائی(p);
      }

      c->بفر->پیغام (c->font,
			  "multiplied glyphs at %s",
			  buf);
    }

    return_trace (سچ);
  }

  سانچہ <قسم_نام Iterator,
            hb_requires (hb_is_ماخذ_کا (Iterator, hb_codepoint_t))>
  منطقی serialize (hb_serialize_context_t *c,
                  Iterator subst)
  {
    TRACE_SERIALIZE (یہ);
    return_trace (substitute.serialize (c, subst));
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);
    ثابت hb_سیٹ_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    اگر (!intersects (&glyphset)) return_trace (جھوٹ);

    خودکار it =
    + hb_iter (substitute)
    | hb_map (glyph_map)
    ;

    خودکار *برآمد = c->serializer->آغاز_embed (*یہ);
    return_trace (برآمد->serialize (c->serializer, it));
  }
};


}
}
}


#اگر_ختم /* OT_LAYOUT_GSUB_SEQUENCE_HH */
