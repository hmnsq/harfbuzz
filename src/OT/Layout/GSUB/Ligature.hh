#جيڪڏهن_وصف_نه_ٿيل OT_LAYOUT_GSUB_LIGATURE_HH
#وصف_ڪريو OT_LAYOUT_GSUB_LIGATURE_HH

#ضمن "Common.hh"

نطاق OT {
نطاق Layout {
نطاق GSUB_impl {

سانچو <قسم_نالو Types>
بنية Ligature
{
  عام:
  قسم_نالو Types::HBGlyphID
		ligGlyph;               /* GlyphID of ligature to substitute */
  Headless_Array16_جو<قسم_نالو Types::HBGlyphID>
		component;              /* Array of component GlyphIDs--start
                                         * with the second  component--ordered
                                         * in writing direction */
  عام:
  DEFINE_سائيز_ARRAY (Types::سائيز + 2, component);

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (ligGlyph.sanitize (c) && component.sanitize (c));
  }

  منطقي intersects (ثابت hb_سيٽ_t *glyphs) ثابت
  { أرجع hb_all (component, glyphs); }

  منطقي intersects_lig_glyph (ثابت hb_سيٽ_t *glyphs) ثابت
  { أرجع glyphs->has(ligGlyph); }

  فراغ گهيرو (hb_گهيرو_context_t *c) ثابت
  {
    إذا (!intersects (c->glyphs)) أرجع;
    c->output->جوڙيو (ligGlyph);
  }

  فراغ depend (hb_depend_context_t *c, hb_codepoint_t پهريون) ثابت
  {
    // Build the complete ligature set upfront before adding any edges
    hb_سيٽ_t complete_ligset;
    complete_ligset.جوڙيو (پهريون);
    + hb_iter (component) | hb_sink (complete_ligset);
    إذا (unlikely (complete_ligset.اندريون_error ()))
    {
      c->depend_معلومات->fail ();
      أرجع;
    }

    منطقي ligset_created;
    hb_codepoint_t ligset_idx = c->depend_معلومات->ڳوليو_or_create_سيٽ (complete_ligset,
                                                                    &ligset_created);
    إذا (unlikely (ligset_idx == HB_CODEPOINT_INVALID))
      أرجع;

    // Track whether any edge using this ligset_idx was actually added
    منطقي any_added = خطأ;

    // Now add one edge for each glyph in the complete, immutable set
    + hb_iter (complete_ligset)
    | hb_apply ([&] (hb_codepoint_t gid) {
        إذا (c->depend_معلومات->جوڙيو_gsub_lookup (gid, c->lookup_اشاريو, ligGlyph, ligset_idx))
          any_added = سچ;
      })
    ;

    // If no edges were added, a newly allocated set is unused - free it for reuse
    إذا (!any_added && ligset_created)
      c->depend_معلومات->discard_سيٽ (ligset_idx);
  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    c->داخلا_2->جوڙيو_array (component.arrayZ, component.get_ڊيگهائي ());
    c->output->جوڙيو (ligGlyph);
  }

  سانچو <قسم_نالو سيٽ_t>
  فراغ collect_ٻيو (سيٽ_t &s) ثابت
  {
    إذا (unlikely (!component.get_ڊيگهائي ()))
    {
      // A ligature without any components. Anything matches.
      s = سيٽ_t::ڀريل ();
      أرجع;
    }
    s.جوڙيو (component.arrayZ[0]);
  }

  منطقي would_apply (hb_would_apply_context_t *c) ثابت
  {
    إذا (c->len != component.lenP1)
      أرجع خطأ;

    لكل (بنانشان صحيح i = 1; i < c->len; i++)
      إذا (likely (c->glyphs[i] != component[i]))
        أرجع خطأ;

    أرجع سچ;
  }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);
    بنانشان صحيح ڳڻپ = component.lenP1;

    إذا (unlikely (!ڳڻپ)) return_trace (خطأ);

    /* Special-case to make it in-place and not consider this
     * as a "ligated" substitution. */
    إذا (unlikely (ڳڻپ == 1))
    {

      إذا (HB_بفر_پيغام_وڌيڪ && c->بفر->messaging ())
      {
	c->بفر->sync_so_far ();
	c->بفر->پيغام (c->font,
			    "replacing glyph at %u (ligature substitution)",
			    c->بفر->idx);
      }

      c->replace_glyph (ligGlyph);

      إذا (HB_بفر_پيغام_وڌيڪ && c->بفر->messaging ())
      {
	c->بفر->پيغام (c->font,
			    "replaced glyph at %u (ligature substitution)",
			    c->بفر->idx - 1u);
      }

      return_trace (سچ);
    }

    بنانشان صحيح ڪل_component_ڳڻپ = 0;

    إذا (unlikely (ڳڻپ > HB_MAX_CONTEXT_ڊيگهائي)) أرجع خطأ;
    بنانشان صحيح ميلاپ_پڇاڙي = 0;

    إذا (likely (!ميلاپ_داخلا (c, ڳڻپ,
                              &component[1],
                              ميلاپ_glyph,
                              خالي_اشارو,
                              &ميلاپ_پڇاڙي,
                              &ڪل_component_ڳڻپ)))
    {
      c->بفر->unsafe_منزل_concat (c->بفر->idx, ميلاپ_پڇاڙي);
      return_trace (خطأ);
    }

    بنانشان pos = 0;
    إذا (HB_بفر_پيغام_وڌيڪ && c->بفر->messaging ())
    {
      بنانشان delta = c->بفر->sync_so_far ();

      pos = c->بفر->idx;

      حرف buf[HB_MAX_CONTEXT_ڊيگهائي * 16] = {0};
      حرف *p = buf;

      ميلاپ_پڇاڙي += delta;
      لكل (بنانشان i = 0; i < ڳڻپ; i++)
      {
	c->ميلاپ_positions[i] += delta;
	إذا (i)
	  *p++ = ',';
	snprintf (p, حجم(buf) - (p - buf), "%u", c->ميلاپ_positions[i]);
	p += لڙي_ڊيگهه(p);
      }

      c->بفر->پيغام (c->font,
			  "ligating glyphs at %s",
			  buf);
    }

    ligate_داخلا (c,
                  ڳڻپ,
                  ميلاپ_پڇاڙي,
                  ligGlyph,
                  ڪل_component_ڳڻپ);

    إذا (HB_بفر_پيغام_وڌيڪ && c->بفر->messaging ())
    {
      c->بفر->sync_so_far ();
      c->بفر->پيغام (c->font,
			  "ligated glyph at %u",
			  pos);
    }

    return_trace (سچ);
  }

  سانچو <قسم_نالو Iterator,
            hb_requires (hb_is_ذريعو_جو (Iterator, hb_codepoint_t))>
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

  منطقي subset (hb_subset_context_t *c, بنانشان coverage_idx) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_سيٽ_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    إذا (!intersects (&glyphset) || !glyphset.has (ligGlyph)) return_trace (خطأ);
    // Ensure Coverage table is always packed after this.
    c->serializer->جوڙيو_virtual_link (coverage_idx);

    خودڪار it =
      + hb_iter (component)
      | hb_map (glyph_map)
      ;

    خودڪار *ٻاهريون = c->serializer->شروعات_embed (*هذا);
    return_trace (ٻاهريون->serialize (c->serializer,
                                  glyph_map[ligGlyph],
                                  it));  }
};


}
}
}

#جيڪڏهن_پورو  /* OT_LAYOUT_GSUB_LIGATURE_HH */
