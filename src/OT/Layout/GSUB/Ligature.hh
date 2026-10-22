#جے_بنا_تعریف OT_LAYOUT_GSUB_LIGATURE_HH
#تعریف_کرو OT_LAYOUT_GSUB_LIGATURE_HH

#ضمن "Common.hh"

نطاق OT {
نطاق Layout {
نطاق GSUB_impl {

سانچا <قسم_ناں Types>
بنية Ligature
{
  عام:
  قسم_ناں Types::HBGlyphID
		ligGlyph;               /* GlyphID of ligature to substitute */
  Headless_Array16_دا<قسم_ناں Types::HBGlyphID>
		component;              /* Array of component GlyphIDs--start
                                         * with the second  component--ordered
                                         * in writing direction */
  عام:
  DEFINE_ناپ_ARRAY (Types::ناپ + 2, component);

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (ligGlyph.sanitize (c) && component.sanitize (c));
  }

  منطقي intersects (ثابت hb_سیٹ_t *glyphs) ثابت
  { أرجع hb_all (component, glyphs); }

  منطقي intersects_lig_glyph (ثابت hb_سیٹ_t *glyphs) ثابت
  { أرجع glyphs->has(ligGlyph); }

  فراغ گھیرا (hb_گھیرا_context_t *c) ثابت
  {
    إذا (!intersects (c->glyphs)) أرجع;
    c->output->جوڑو (ligGlyph);
  }

  فراغ depend (hb_depend_context_t *c, hb_codepoint_t پہلا) ثابت
  {
    // Build the complete ligature set upfront before adding any edges
    hb_سیٹ_t complete_ligset;
    complete_ligset.جوڑو (پہلا);
    + hb_iter (component) | hb_sink (complete_ligset);
    إذا (unlikely (complete_ligset.وچ_error ()))
    {
      c->depend_انکڑے->fail ();
      أرجع;
    }

    منطقي ligset_created;
    hb_codepoint_t ligset_idx = c->depend_انکڑے->لبھو_or_create_سیٹ (complete_ligset,
                                                                    &ligset_created);
    إذا (unlikely (ligset_idx == HB_CODEPOINT_INVALID))
      أرجع;

    // Track whether any edge using this ligset_idx was actually added
    منطقي any_added = خطأ;

    // Now add one edge for each glyph in the complete, immutable set
    + hb_iter (complete_ligset)
    | hb_apply ([&] (hb_codepoint_t gid) {
        إذا (c->depend_انکڑے->جوڑو_gsub_lookup (gid, c->lookup_اشاریہ, ligGlyph, ligset_idx))
          any_added = سچ;
      })
    ;

    // If no edges were added, a newly allocated set is unused - free it for reuse
    إذا (!any_added && ligset_created)
      c->depend_انکڑے->discard_سیٹ (ligset_idx);
  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    c->داخلہ->جوڑو_array (component.arrayZ, component.get_لمائی ());
    c->output->جوڑو (ligGlyph);
  }

  سانچا <قسم_ناں سیٹ_t>
  فراغ collect_دوجا (سیٹ_t &s) ثابت
  {
    إذا (unlikely (!component.get_لمائی ()))
    {
      // A ligature without any components. Anything matches.
      s = سیٹ_t::بھریا ();
      أرجع;
    }
    s.جوڑو (component.arrayZ[0]);
  }

  منطقي would_apply (hb_would_apply_context_t *c) ثابت
  {
    إذا (c->len != component.lenP1)
      أرجع خطأ;

    لكل (بنانشانی صحيح i = 1; i < c->len; i++)
      إذا (likely (c->glyphs[i] != component[i]))
        أرجع خطأ;

    أرجع سچ;
  }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);
    بنانشانی صحيح گنتی = component.lenP1;

    إذا (unlikely (!گنتی)) return_trace (خطأ);

    /* Special-case to make it in-place and not consider this
     * as a "ligated" substitution. */
    إذا (unlikely (گنتی == 1))
    {

      إذا (HB_بفر_سنیہا_ہور && c->بفر->messaging ())
      {
	c->بفر->sync_so_far ();
	c->بفر->سنیہا (c->font,
			    "replacing glyph at %u (ligature substitution)",
			    c->بفر->idx);
      }

      c->replace_glyph (ligGlyph);

      إذا (HB_بفر_سنیہا_ہور && c->بفر->messaging ())
      {
	c->بفر->سنیہا (c->font,
			    "replaced glyph at %u (ligature substitution)",
			    c->بفر->idx - 1u);
      }

      return_trace (سچ);
    }

    بنانشانی صحيح کل_component_گنتی = 0;

    إذا (unlikely (گنتی > HB_MAX_CONTEXT_لمائی)) أرجع خطأ;
    بنانشانی صحيح میل_سرا = 0;

    إذا (likely (!میل_داخلہ (c, گنتی,
                              &component[1],
                              میل_glyph,
                              خالی_سوچک,
                              &میل_سرا,
                              &کل_component_گنتی)))
    {
      c->بفر->unsafe_منزل_concat (c->بفر->idx, میل_سرا);
      return_trace (خطأ);
    }

    بنانشانی pos = 0;
    إذا (HB_بفر_سنیہا_ہور && c->بفر->messaging ())
    {
      بنانشانی delta = c->بفر->sync_so_far ();

      pos = c->بفر->idx;

      حرف buf[HB_MAX_CONTEXT_لمائی * 16] = {0};
      حرف *p = buf;

      میل_سرا += delta;
      لكل (بنانشانی i = 0; i < گنتی; i++)
      {
	c->میل_positions[i] += delta;
	إذا (i)
	  *p++ = ',';
	snprintf (p, حجم(buf) - (p - buf), "%u", c->میل_positions[i]);
	p += لڑی_لمبائی(p);
      }

      c->بفر->سنیہا (c->font,
			  "ligating glyphs at %s",
			  buf);
    }

    ligate_داخلہ (c,
                  گنتی,
                  میل_سرا,
                  ligGlyph,
                  کل_component_گنتی);

    إذا (HB_بفر_سنیہا_ہور && c->بفر->messaging ())
    {
      c->بفر->sync_so_far ();
      c->بفر->سنیہا (c->font,
			  "ligated glyph at %u",
			  pos);
    }

    return_trace (سچ);
  }

  سانچا <قسم_ناں Iterator,
            hb_requires (hb_is_سروت_دا (Iterator, hb_codepoint_t))>
  منطقي serialize (hb_serialize_context_t *c,
                  hb_codepoint_t ligature,
                  Iterator components /* Starting from second */)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);
    ligGlyph = ligature;
    إذا (unlikely (!component.serialize (c, components))) return_trace (خطأ);
    return_trace (سچ);
  }

  منطقي subset (hb_subset_context_t *c, بنانشانی coverage_idx) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_سیٹ_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    إذا (!intersects (&glyphset) || !glyphset.has (ligGlyph)) return_trace (خطأ);
    // Ensure Coverage table is always packed after this.
    c->serializer->جوڑو_virtual_link (coverage_idx);

    خودکار it =
      + hb_iter (component)
      | hb_map (glyph_map)
      ;

    خودکار *باہرلا = c->serializer->شروع_embed (*هذا);
    return_trace (باہرلا->serialize (c->serializer,
                                  glyph_map[ligGlyph],
                                  it));  }
};


}
}
}

#جے_انت  /* OT_LAYOUT_GSUB_LIGATURE_HH */
