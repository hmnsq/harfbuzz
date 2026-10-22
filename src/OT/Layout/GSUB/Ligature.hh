#که_نه_تعریف_شوی OT_LAYOUT_GSUB_LIGATURE_HH
#تعریف_کړه OT_LAYOUT_GSUB_LIGATURE_HH

#ضمن "Common.hh"

نطاق OT {
نطاق Layout {
نطاق GSUB_impl {

کينډۍ <ډول_نوم Types>
بنية Ligature
{
  عام:
  ډول_نوم Types::HBGlyphID
		ligGlyph;               /* GlyphID of ligature to substitute */
  Headless_Array16_د<ډول_نوم Types::HBGlyphID>
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

  منطقي intersects (ثابت hb_سېټ_t *glyphs) ثابت
  { أرجع hb_all (component, glyphs); }

  منطقي intersects_lig_glyph (ثابت hb_سېټ_t *glyphs) ثابت
  { أرجع glyphs->has(ligGlyph); }

  فراغ تړون (hb_تړون_context_t *c) ثابت
  {
    إذا (!intersects (c->glyphs)) أرجع;
    c->output->جمع_کړه (ligGlyph);
  }

  فراغ depend (hb_depend_context_t *c, hb_codepoint_t لومړی) ثابت
  {
    // Build the complete ligature set upfront before adding any edges
    hb_سېټ_t complete_ligset;
    complete_ligset.جمع_کړه (لومړی);
    + hb_iter (component) | hb_sink (complete_ligset);
    إذا (unlikely (complete_ligset.دننه_error ()))
    {
      c->depend_معلومات->fail ();
      أرجع;
    }

    منطقي ligset_created;
    hb_codepoint_t ligset_idx = c->depend_معلومات->پیدا_کړه_or_create_سېټ (complete_ligset,
                                                                    &ligset_created);
    إذا (unlikely (ligset_idx == HB_CODEPOINT_INVALID))
      أرجع;

    // Track whether any edge using this ligset_idx was actually added
    منطقي any_added = خطأ;

    // Now add one edge for each glyph in the complete, immutable set
    + hb_iter (complete_ligset)
    | hb_apply ([&] (hb_codepoint_t gid) {
        إذا (c->depend_معلومات->جمع_کړه_gsub_lookup (gid, c->lookup_شاخص, ligGlyph, ligset_idx))
          any_added = ریښتیا;
      })
    ;

    // If no edges were added, a newly allocated set is unused - free it for reuse
    إذا (!any_added && ligset_created)
      c->depend_معلومات->discard_سېټ (ligset_idx);
  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    c->ننوت->جمع_کړه_array (component.arrayZ, component.get_اوږدتیا ());
    c->output->جمع_کړه (ligGlyph);
  }

  کينډۍ <ډول_نوم سېټ_t>
  فراغ collect_دوهم (سېټ_t &s) ثابت
  {
    إذا (unlikely (!component.get_اوږدتیا ()))
    {
      // A ligature without any components. Anything matches.
      s = سېټ_t::ډک ();
      أرجع;
    }
    s.جمع_کړه (component.arrayZ[0]);
  }

  منطقي would_apply (hb_would_apply_context_t *c) ثابت
  {
    إذا (c->len != component.lenP1)
      أرجع خطأ;

    لكل (بینښه صحيح i = 1; i < c->len; i++)
      إذا (likely (c->glyphs[i] != component[i]))
        أرجع خطأ;

    أرجع ریښتیا;
  }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);
    بینښه صحيح شمېر = component.lenP1;

    إذا (unlikely (!شمېر)) return_trace (خطأ);

    /* Special-case to make it in-place and not consider this
     * as a "ligated" substitution. */
    إذا (unlikely (شمېر == 1))
    {

      إذا (HB_بفر_پیغام_نور && c->بفر->messaging ())
      {
	c->بفر->sync_so_far ();
	c->بفر->پیغام (c->font,
			    "replacing glyph at %u (ligature substitution)",
			    c->بفر->idx);
      }

      c->replace_glyph (ligGlyph);

      إذا (HB_بفر_پیغام_نور && c->بفر->messaging ())
      {
	c->بفر->پیغام (c->font,
			    "replaced glyph at %u (ligature substitution)",
			    c->بفر->idx - 1u);
      }

      return_trace (ریښتیا);
    }

    بینښه صحيح ټول_component_شمېر = 0;

    إذا (unlikely (شمېر > HB_MAX_CONTEXT_اوږدتیا)) أرجع خطأ;
    بینښه صحيح سمون_پای_ټکی = 0;

    إذا (likely (!سمون_ننوت (c, شمېر,
                              &component[1],
                              سمون_glyph,
                              تش_اشاره,
                              &سمون_پای_ټکی,
                              &ټول_component_شمېر)))
    {
      c->بفر->unsafe_منزل_concat (c->بفر->idx, سمون_پای_ټکی);
      return_trace (خطأ);
    }

    بینښه pos = 0;
    إذا (HB_بفر_پیغام_نور && c->بفر->messaging ())
    {
      بینښه delta = c->بفر->sync_so_far ();

      pos = c->بفر->idx;

      حرف buf[HB_MAX_CONTEXT_اوږدتیا * 16] = {0};
      حرف *p = buf;

      سمون_پای_ټکی += delta;
      لكل (بینښه i = 0; i < شمېر; i++)
      {
	c->سمون_positions[i] += delta;
	إذا (i)
	  *p++ = ',';
	snprintf (p, حجم(buf) - (p - buf), "%u", c->سمون_positions[i]);
	p += مزي_اوږدوالی(p);
      }

      c->بفر->پیغام (c->font,
			  "ligating glyphs at %s",
			  buf);
    }

    ligate_ننوت (c,
                  شمېر,
                  سمون_پای_ټکی,
                  ligGlyph,
                  ټول_component_شمېر);

    إذا (HB_بفر_پیغام_نور && c->بفر->messaging ())
    {
      c->بفر->sync_so_far ();
      c->بفر->پیغام (c->font,
			  "ligated glyph at %u",
			  pos);
    }

    return_trace (ریښتیا);
  }

  کينډۍ <ډول_نوم Iterator,
            hb_requires (hb_is_سرچینه_د (Iterator, hb_codepoint_t))>
  منطقي serialize (hb_serialize_context_t *c,
                  hb_codepoint_t ligature,
                  Iterator components /* Starting from second */)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);
    ligGlyph = ligature;
    إذا (unlikely (!component.serialize (c, components))) return_trace (خطأ);
    return_trace (ریښتیا);
  }

  منطقي subset (hb_subset_context_t *c, بینښه coverage_idx) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_سېټ_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    إذا (!intersects (&glyphset) || !glyphset.has (ligGlyph)) return_trace (خطأ);
    // Ensure Coverage table is always packed after this.
    c->serializer->جمع_کړه_virtual_link (coverage_idx);

    خپلکاری it =
      + hb_iter (component)
      | hb_map (glyph_map)
      ;

    خپلکاری *بهر = c->serializer->پیل_embed (*هذا);
    return_trace (بهر->serialize (c->serializer,
                                  glyph_map[ligGlyph],
                                  it));  }
};


}
}
}

#د_که_پای  /* OT_LAYOUT_GSUB_LIGATURE_HH */
