#اگر_تعریف_نشده OT_LAYOUT_GSUB_LIGATURE_HH
#تعریف_کن OT_LAYOUT_GSUB_LIGATURE_HH

#ضمن "Common.hh"

نطاق OT {
نطاق Layout {
نطاق GSUB_impl {

قالب <نام_نوع Types>
بنية Ligature
{
  عام:
  نام_نوع Types::HBGlyphID
		ligGlyph;               /* GlyphID of ligature to substitute */
  Headless_Array16_از<نام_نوع Types::HBGlyphID>
		component;              /* Array of component GlyphIDs--start
                                         * with the second  component--ordered
                                         * in writing direction */
  عام:
  DEFINE_اندازه_ARRAY (Types::اندازه + 2, component);

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (ligGlyph.sanitize (c) && component.sanitize (c));
  }

  منطقي intersects (ثابت hb_مجموعه_t *glyphs) ثابت
  { أرجع hb_all (component, glyphs); }

  منطقي intersects_lig_glyph (ثابت hb_مجموعه_t *glyphs) ثابت
  { أرجع glyphs->has(ligGlyph); }

  فراغ بستگی (hb_بستگی_context_t *c) ثابت
  {
    إذا (!intersects (c->glyphs)) أرجع;
    c->output->جمع_کن (ligGlyph);
  }

  فراغ depend (hb_depend_context_t *c, hb_codepoint_t اولی) ثابت
  {
    // Build the complete ligature set upfront before adding any edges
    hb_مجموعه_t complete_ligset;
    complete_ligset.جمع_کن (اولی);
    + hb_iter (component) | hb_sink (complete_ligset);
    إذا (unlikely (complete_ligset.درون_error ()))
    {
      c->depend_داده->fail ();
      أرجع;
    }

    منطقي ligset_created;
    hb_codepoint_t ligset_idx = c->depend_داده->پیدا_کن_or_create_مجموعه (complete_ligset,
                                                                    &ligset_created);
    إذا (unlikely (ligset_idx == HB_CODEPOINT_INVALID))
      أرجع;

    // Track whether any edge using this ligset_idx was actually added
    منطقي any_added = خطأ;

    // Now add one edge for each glyph in the complete, immutable set
    + hb_iter (complete_ligset)
    | hb_apply ([&] (hb_codepoint_t gid) {
        إذا (c->depend_داده->جمع_کن_gsub_lookup (gid, c->lookup_شاخص, ligGlyph, ligset_idx))
          any_added = درست;
      })
    ;

    // If no edges were added, a newly allocated set is unused - free it for reuse
    إذا (!any_added && ligset_created)
      c->depend_داده->discard_مجموعه (ligset_idx);
  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    c->درون_داد->جمع_کن_array (component.arrayZ, component.get_درازی ());
    c->output->جمع_کن (ligGlyph);
  }

  قالب <نام_نوع مجموعه_t>
  فراغ collect_دومی (مجموعه_t &s) ثابت
  {
    إذا (unlikely (!component.get_درازی ()))
    {
      // A ligature without any components. Anything matches.
      s = مجموعه_t::پر ();
      أرجع;
    }
    s.جمع_کن (component.arrayZ[0]);
  }

  منطقي would_apply (hb_would_apply_context_t *c) ثابت
  {
    إذا (c->len != component.lenP1)
      أرجع خطأ;

    لكل (بیعلامت صحيح i = 1; i < c->len; i++)
      إذا (likely (c->glyphs[i] != component[i]))
        أرجع خطأ;

    أرجع درست;
  }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);
    بیعلامت صحيح شمار = component.lenP1;

    إذا (unlikely (!شمار)) return_trace (خطأ);

    /* Special-case to make it in-place and not consider this
     * as a "ligated" substitution. */
    إذا (unlikely (شمار == 1))
    {

      إذا (HB_میانگیر_پیام_بیشتر && c->میانگیر->messaging ())
      {
	c->میانگیر->sync_so_far ();
	c->میانگیر->پیام (c->font,
			    "replacing glyph at %u (ligature substitution)",
			    c->میانگیر->idx);
      }

      c->replace_glyph (ligGlyph);

      إذا (HB_میانگیر_پیام_بیشتر && c->میانگیر->messaging ())
      {
	c->میانگیر->پیام (c->font,
			    "replaced glyph at %u (ligature substitution)",
			    c->میانگیر->idx - 1u);
      }

      return_trace (درست);
    }

    بیعلامت صحيح مجموع_component_شمار = 0;

    إذا (unlikely (شمار > HB_MAX_CONTEXT_درازی)) أرجع خطأ;
    بیعلامت صحيح تطابق_انتها = 0;

    إذا (likely (!تطابق_درون_داد (c, شمار,
                              &component[1],
                              تطابق_glyph,
                              اشاره_تهی,
                              &تطابق_انتها,
                              &مجموع_component_شمار)))
    {
      c->میانگیر->unsafe_مقصد_concat (c->میانگیر->idx, تطابق_انتها);
      return_trace (خطأ);
    }

    بیعلامت pos = 0;
    إذا (HB_میانگیر_پیام_بیشتر && c->میانگیر->messaging ())
    {
      بیعلامت delta = c->میانگیر->sync_so_far ();

      pos = c->میانگیر->idx;

      حرف buf[HB_MAX_CONTEXT_درازی * 16] = {0};
      حرف *p = buf;

      تطابق_انتها += delta;
      لكل (بیعلامت i = 0; i < شمار; i++)
      {
	c->تطابق_positions[i] += delta;
	إذا (i)
	  *p++ = ',';
	snprintf (p, حجم(buf) - (p - buf), "%u", c->تطابق_positions[i]);
	p += طول_رشته(p);
      }

      c->میانگیر->پیام (c->font,
			  "ligating glyphs at %s",
			  buf);
    }

    ligate_درون_داد (c,
                  شمار,
                  تطابق_انتها,
                  ligGlyph,
                  مجموع_component_شمار);

    إذا (HB_میانگیر_پیام_بیشتر && c->میانگیر->messaging ())
    {
      c->میانگیر->sync_so_far ();
      c->میانگیر->پیام (c->font,
			  "ligated glyph at %u",
			  pos);
    }

    return_trace (درست);
  }

  قالب <نام_نوع Iterator,
            hb_requires (hb_is_منبع_از (Iterator, hb_codepoint_t))>
  منطقي serialize (hb_serialize_context_t *c,
                  hb_codepoint_t ligature,
                  Iterator components /* Starting from second */)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);
    ligGlyph = ligature;
    إذا (unlikely (!component.serialize (c, components))) return_trace (خطأ);
    return_trace (درست);
  }

  منطقي subset (hb_subset_context_t *c, بیعلامت coverage_idx) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_مجموعه_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    إذا (!intersects (&glyphset) || !glyphset.has (ligGlyph)) return_trace (خطأ);
    // Ensure Coverage table is always packed after this.
    c->serializer->جمع_کن_virtual_link (coverage_idx);

    خودکار it =
      + hb_iter (component)
      | hb_map (glyph_map)
      ;

    خودکار *بیرون = c->serializer->شروع_embed (*هذا);
    return_trace (بیرون->serialize (c->serializer,
                                  glyph_map[ligGlyph],
                                  it));  }
};


}
}
}

#پایان_اگر  /* OT_LAYOUT_GSUB_LIGATURE_HH */
