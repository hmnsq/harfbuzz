#جيڪڏهن_وصف_نه_ٿيل OT_LAYOUT_GSUB_SEQUENCE_HH
#وصف_ڪريو OT_LAYOUT_GSUB_SEQUENCE_HH

#ضمن "Common.hh"

نطاق OT {
نطاق Layout {
نطاق GSUB_impl {

سانچو <قسم_نالو Types>
بنية Sequence
{
  محمي:
  Array16_جو<قسم_نالو Types::HBGlyphID>
                substitute;             /* String of GlyphIDs to substitute */
  عام:
  DEFINE_سائيز_ARRAY (2, substitute);

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (substitute.sanitize (c));
  }

  منطقي intersects (ثابت hb_سيٽ_t *glyphs) ثابت
  { أرجع hb_all (substitute, glyphs); }

  فراغ گهيرو (hb_گهيرو_context_t *c) ثابت
  { c->output->جوڙيو_array (substitute.arrayZ, substitute.len); }

  فراغ depend (hb_depend_context_t *c, hb_codepoint_t ذريعو) ثابت
  {
    + hb_iter (substitute)
    | hb_apply ([&] (ثابت hb_codepoint_t &target) { c->depend_معلومات->جوڙيو_gsub_lookup (ذريعو, c->lookup_اشاريو, target); })
    ;
  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  { c->output->جوڙيو_array (substitute.arrayZ, substitute.len); }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);
    بنانشان صحيح ڳڻپ = substitute.len;

    /* Special-case to make it in-place and not consider this
     * as a "multiplied" substitution. */
    إذا (unlikely (ڳڻپ == 1))
    {
      إذا (HB_بفر_پيغام_وڌيڪ && c->بفر->messaging ())
      {
	c->بفر->sync_so_far ();
	c->بفر->پيغام (c->font,
			    "replacing glyph at %u (multiple substitution)",
			    c->بفر->idx);
      }

      c->replace_glyph (substitute.arrayZ[0]);

      إذا (HB_بفر_پيغام_وڌيڪ && c->بفر->messaging ())
      {
	c->بفر->پيغام (c->font,
			    "replaced glyph at %u (multiple substitution)",
			    c->بفر->idx - 1u);
      }

      return_trace (سچ);
    }
    /* Spec disallows this, but Uniscribe allows it.
     * https://github.com/harfbuzz/harfbuzz/issues/253 */
    وإلا إذا (unlikely (ڳڻپ == 0))
    {
      إذا (HB_بفر_پيغام_وڌيڪ && c->بفر->messaging ())
      {
	c->بفر->sync_so_far ();
	c->بفر->پيغام (c->font,
			    "deleting glyph at %u (multiple substitution)",
			    c->بفر->idx);
      }

      c->بفر->delete_glyph ();

      إذا (HB_بفر_پيغام_وڌيڪ && c->بفر->messaging ())
      {
	c->بفر->sync_so_far ();
	c->بفر->پيغام (c->font,
			    "deleted glyph at %u (multiple substitution)",
			    c->بفر->idx);
      }

      return_trace (سچ);
    }

    إذا (HB_بفر_پيغام_وڌيڪ && c->بفر->messaging ())
    {
      c->بفر->sync_so_far ();
      c->بفر->پيغام (c->font,
			  "multiplying glyph at %u",
			  c->بفر->idx);
    }

    بنانشان صحيح klass = _hb_glyph_info_is_ligature (&c->بفر->cur()) ?
                         HB_OT_LAYOUT_GLYPH_PROPS_بنياد_GLYPH : 0;
    بنانشان lig_id = _hb_glyph_info_get_lig_id (&c->بفر->cur());

    لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
    {
      /* If is attached to a ligature, don't disturb that.
       * https://github.com/harfbuzz/harfbuzz/issues/3069 */
      إذا (!lig_id)
        hb_glyph_info_سيٽ_lig_props_for_component (&c->بفر->cur(), i);
      c->output_glyph_for_component (substitute.arrayZ[i], klass);
    }
    c->بفر->skip_glyph ();

    إذا (HB_بفر_پيغام_وڌيڪ && c->بفر->messaging ())
    {
      c->بفر->sync_so_far ();

      حرف buf[HB_MAX_CONTEXT_ڊيگهائي * 16] = {0};
      حرف *p = buf;

      لكل (بنانشان i = c->بفر->idx - ڳڻپ; i < c->بفر->idx; i++)
      {
	إذا (buf < p && حجم(buf) - 1u > بنانشان (p - buf))
	  *p++ = ',';
	snprintf (p, حجم(buf) - (p - buf), "%u", i);
	p += لڙي_ڊيگهه(p);
      }

      c->بفر->پيغام (c->font,
			  "multiplied glyphs at %s",
			  buf);
    }

    return_trace (سچ);
  }

  سانچو <قسم_نالو Iterator,
            hb_requires (hb_is_ذريعو_جو (Iterator, hb_codepoint_t))>
  منطقي serialize (hb_serialize_context_t *c,
                  Iterator subst)
  {
    TRACE_SERIALIZE (هذا);
    return_trace (substitute.serialize (c, subst));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_سيٽ_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    إذا (!intersects (&glyphset)) return_trace (خطأ);

    خودڪار it =
    + hb_iter (substitute)
    | hb_map (glyph_map)
    ;

    خودڪار *ٻاهريون = c->serializer->شروعات_embed (*هذا);
    return_trace (ٻاهريون->serialize (c->serializer, it));
  }
};


}
}
}


#جيڪڏهن_پورو /* OT_LAYOUT_GSUB_SEQUENCE_HH */
