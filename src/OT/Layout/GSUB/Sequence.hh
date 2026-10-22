#اگر_تعریف_نشده OT_LAYOUT_GSUB_SEQUENCE_HH
#تعریف_کن OT_LAYOUT_GSUB_SEQUENCE_HH

#ضمن "Common.hh"

نطاق OT {
نطاق Layout {
نطاق GSUB_impl {

قالب <نام_نوع Types>
بنية Sequence
{
  محمي:
  Array16_از<نام_نوع Types::HBGlyphID>
                substitute;             /* String of GlyphIDs to substitute */
  عام:
  DEFINE_اندازه_ARRAY (2, substitute);

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (substitute.sanitize (c));
  }

  منطقي intersects (ثابت hb_مجموعه_t *glyphs) ثابت
  { أرجع hb_all (substitute, glyphs); }

  فراغ بستگی (hb_بستگی_context_t *c) ثابت
  { c->output->جمع_کن_array (substitute.arrayZ, substitute.len); }

  فراغ depend (hb_depend_context_t *c, hb_codepoint_t منبع) ثابت
  {
    + hb_iter (substitute)
    | hb_apply ([&] (ثابت hb_codepoint_t &target) { c->depend_داده->جمع_کن_gsub_lookup (منبع, c->lookup_شاخص, target); })
    ;
  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  { c->output->جمع_کن_array (substitute.arrayZ, substitute.len); }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);
    بیعلامت صحيح شمار = substitute.len;

    /* Special-case to make it in-place and not consider this
     * as a "multiplied" substitution. */
    إذا (unlikely (شمار == 1))
    {
      إذا (HB_میانگیر_پیام_بیشتر && c->میانگیر->messaging ())
      {
	c->میانگیر->sync_so_far ();
	c->میانگیر->پیام (c->font,
			    "replacing glyph at %u (multiple substitution)",
			    c->میانگیر->idx);
      }

      c->replace_glyph (substitute.arrayZ[0]);

      إذا (HB_میانگیر_پیام_بیشتر && c->میانگیر->messaging ())
      {
	c->میانگیر->پیام (c->font,
			    "replaced glyph at %u (multiple substitution)",
			    c->میانگیر->idx - 1u);
      }

      return_trace (درست);
    }
    /* Spec disallows this, but Uniscribe allows it.
     * https://github.com/harfbuzz/harfbuzz/issues/253 */
    وإلا إذا (unlikely (شمار == 0))
    {
      إذا (HB_میانگیر_پیام_بیشتر && c->میانگیر->messaging ())
      {
	c->میانگیر->sync_so_far ();
	c->میانگیر->پیام (c->font,
			    "deleting glyph at %u (multiple substitution)",
			    c->میانگیر->idx);
      }

      c->میانگیر->delete_glyph ();

      إذا (HB_میانگیر_پیام_بیشتر && c->میانگیر->messaging ())
      {
	c->میانگیر->sync_so_far ();
	c->میانگیر->پیام (c->font,
			    "deleted glyph at %u (multiple substitution)",
			    c->میانگیر->idx);
      }

      return_trace (درست);
    }

    إذا (HB_میانگیر_پیام_بیشتر && c->میانگیر->messaging ())
    {
      c->میانگیر->sync_so_far ();
      c->میانگیر->پیام (c->font,
			  "multiplying glyph at %u",
			  c->میانگیر->idx);
    }

    بیعلامت صحيح klass = _hb_glyph_info_is_ligature (&c->میانگیر->cur()) ?
                         HB_OT_LAYOUT_GLYPH_PROPS_پایه_GLYPH : 0;
    بیعلامت lig_id = _hb_glyph_info_get_lig_id (&c->میانگیر->cur());

    لكل (بیعلامت صحيح i = 0; i < شمار; i++)
    {
      /* If is attached to a ligature, don't disturb that.
       * https://github.com/harfbuzz/harfbuzz/issues/3069 */
      إذا (!lig_id)
        hb_glyph_info_مجموعه_lig_props_for_component (&c->میانگیر->cur(), i);
      c->output_glyph_for_component (substitute.arrayZ[i], klass);
    }
    c->میانگیر->skip_glyph ();

    إذا (HB_میانگیر_پیام_بیشتر && c->میانگیر->messaging ())
    {
      c->میانگیر->sync_so_far ();

      حرف buf[HB_MAX_CONTEXT_درازی * 16] = {0};
      حرف *p = buf;

      لكل (بیعلامت i = c->میانگیر->idx - شمار; i < c->میانگیر->idx; i++)
      {
	إذا (buf < p && حجم(buf) - 1u > بیعلامت (p - buf))
	  *p++ = ',';
	snprintf (p, حجم(buf) - (p - buf), "%u", i);
	p += طول_رشته(p);
      }

      c->میانگیر->پیام (c->font,
			  "multiplied glyphs at %s",
			  buf);
    }

    return_trace (درست);
  }

  قالب <نام_نوع Iterator,
            hb_requires (hb_is_منبع_از (Iterator, hb_codepoint_t))>
  منطقي serialize (hb_serialize_context_t *c,
                  Iterator subst)
  {
    TRACE_SERIALIZE (هذا);
    return_trace (substitute.serialize (c, subst));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_مجموعه_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    إذا (!intersects (&glyphset)) return_trace (خطأ);

    خودکار it =
    + hb_iter (substitute)
    | hb_map (glyph_map)
    ;

    خودکار *بیرون = c->serializer->شروع_embed (*هذا);
    return_trace (بیرون->serialize (c->serializer, it));
  }
};


}
}
}


#پایان_اگر /* OT_LAYOUT_GSUB_SEQUENCE_HH */
