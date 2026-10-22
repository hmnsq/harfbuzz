#اگر_غیر_تعریف_شدہ OT_LAYOUT_GSUB_LIGATURE_HH
#تعریف_کرو OT_LAYOUT_GSUB_LIGATURE_HH

#شامل "Common.hh"

نامخلا OT {
نامخلا Layout {
نامخلا GSUB_impl {

سانچہ <قسم_نام Types>
ساخت Ligature
{
  عوامی:
  قسم_نام Types::HBGlyphID
		ligGlyph;               /* GlyphID of ligature to substitute */
  Headless_Array16_کا<قسم_نام Types::HBGlyphID>
		component;              /* Array of component GlyphIDs--start
                                         * with the second  component--ordered
                                         * in writing direction */
  عوامی:
  DEFINE_سائز_ARRAY (Types::سائز + 2, component);

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (ligGlyph.sanitize (c) && component.sanitize (c));
  }

  منطقی intersects (ثابت hb_سیٹ_t *glyphs) ثابت
  { واپس hb_all (component, glyphs); }

  منطقی intersects_lig_glyph (ثابت hb_سیٹ_t *glyphs) ثابت
  { واپس glyphs->has(ligGlyph); }

  خالی احاطہ (hb_احاطہ_context_t *c) ثابت
  {
    اگر (!intersects (c->glyphs)) واپس;
    c->output->جوڑو (ligGlyph);
  }

  خالی depend (hb_depend_context_t *c, hb_codepoint_t پہلا) ثابت
  {
    // Build the complete ligature set upfront before adding any edges
    hb_سیٹ_t complete_ligset;
    complete_ligset.جوڑو (پہلا);
    + hb_iter (component) | hb_sink (complete_ligset);
    اگر (unlikely (complete_ligset.اندر_error ()))
    {
      c->depend_ڈیٹا->fail ();
      واپس;
    }

    منطقی ligset_created;
    hb_codepoint_t ligset_idx = c->depend_ڈیٹا->ڈھونڈو_or_create_سیٹ (complete_ligset,
                                                                    &ligset_created);
    اگر (unlikely (ligset_idx == HB_CODEPOINT_INVALID))
      واپس;

    // Track whether any edge using this ligset_idx was actually added
    منطقی any_added = جھوٹ;

    // Now add one edge for each glyph in the complete, immutable set
    + hb_iter (complete_ligset)
    | hb_apply ([&] (hb_codepoint_t gid) {
        اگر (c->depend_ڈیٹا->جوڑو_gsub_lookup (gid, c->lookup_اشاریہ, ligGlyph, ligset_idx))
          any_added = سچ;
      })
    ;

    // If no edges were added, a newly allocated set is unused - free it for reuse
    اگر (!any_added && ligset_created)
      c->depend_ڈیٹا->discard_سیٹ (ligset_idx);
  }

  خالی collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    c->داخلہ->جوڑو_array (component.arrayZ, component.get_لمبائی ());
    c->output->جوڑو (ligGlyph);
  }

  سانچہ <قسم_نام سیٹ_t>
  خالی collect_دوسرا (سیٹ_t &s) ثابت
  {
    اگر (unlikely (!component.get_لمبائی ()))
    {
      // A ligature without any components. Anything matches.
      s = سیٹ_t::بھرا ();
      واپس;
    }
    s.جوڑو (component.arrayZ[0]);
  }

  منطقی would_apply (hb_would_apply_context_t *c) ثابت
  {
    اگر (c->len != component.lenP1)
      واپس جھوٹ;

    برائے (غیرعلامتی صحیح i = 1; i < c->len; i++)
      اگر (likely (c->glyphs[i] != component[i]))
        واپس جھوٹ;

    واپس سچ;
  }

  منطقی apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (یہ);
    غیرعلامتی صحیح گنتی = component.lenP1;

    اگر (unlikely (!گنتی)) return_trace (جھوٹ);

    /* Special-case to make it in-place and not consider this
     * as a "ligated" substitution. */
    اگر (unlikely (گنتی == 1))
    {

      اگر (HB_بفر_پیغام_مزید && c->بفر->messaging ())
      {
	c->بفر->sync_so_far ();
	c->بفر->پیغام (c->font,
			    "replacing glyph at %u (ligature substitution)",
			    c->بفر->idx);
      }

      c->replace_glyph (ligGlyph);

      اگر (HB_بفر_پیغام_مزید && c->بفر->messaging ())
      {
	c->بفر->پیغام (c->font,
			    "replaced glyph at %u (ligature substitution)",
			    c->بفر->idx - 1u);
      }

      return_trace (سچ);
    }

    غیرعلامتی صحیح کل_component_گنتی = 0;

    اگر (unlikely (گنتی > HB_MAX_CONTEXT_لمبائی)) واپس جھوٹ;
    غیرعلامتی صحیح مطابقت_انتہا = 0;

    اگر (likely (!مطابقت_داخلہ (c, گنتی,
                              &component[1],
                              مطابقت_glyph,
                              خالی_اشاریہ,
                              &مطابقت_انتہا,
                              &کل_component_گنتی)))
    {
      c->بفر->unsafe_مقصد_concat (c->بفر->idx, مطابقت_انتہا);
      return_trace (جھوٹ);
    }

    غیرعلامتی pos = 0;
    اگر (HB_بفر_پیغام_مزید && c->بفر->messaging ())
    {
      غیرعلامتی delta = c->بفر->sync_so_far ();

      pos = c->بفر->idx;

      حرف buf[HB_MAX_CONTEXT_لمبائی * 16] = {0};
      حرف *p = buf;

      مطابقت_انتہا += delta;
      برائے (غیرعلامتی i = 0; i < گنتی; i++)
      {
	c->مطابقت_positions[i] += delta;
	اگر (i)
	  *p++ = ',';
	snprintf (p, حجم(buf) - (p - buf), "%u", c->مطابقت_positions[i]);
	p += لڑی_لمبائی(p);
      }

      c->بفر->پیغام (c->font,
			  "ligating glyphs at %s",
			  buf);
    }

    ligate_داخلہ (c,
                  گنتی,
                  مطابقت_انتہا,
                  ligGlyph,
                  کل_component_گنتی);

    اگر (HB_بفر_پیغام_مزید && c->بفر->messaging ())
    {
      c->بفر->sync_so_far ();
      c->بفر->پیغام (c->font,
			  "ligated glyph at %u",
			  pos);
    }

    return_trace (سچ);
  }

  سانچہ <قسم_نام Iterator,
            hb_requires (hb_is_ماخذ_کا (Iterator, hb_codepoint_t))>
  منطقی serialize (hb_serialize_context_t *c,
                  hb_codepoint_t ligature,
                  Iterator components /* Starting from second */)
  {
    TRACE_SERIALIZE (یہ);
    اگر (unlikely (!c->extend_min (یہ))) return_trace (جھوٹ);
    ligGlyph = ligature;
    اگر (unlikely (!component.serialize (c, components))) return_trace (جھوٹ);
    return_trace (سچ);
  }

  منطقی subset (hb_subset_context_t *c, غیرعلامتی coverage_idx) ثابت
  {
    TRACE_SUBSET (یہ);
    ثابت hb_سیٹ_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    اگر (!intersects (&glyphset) || !glyphset.has (ligGlyph)) return_trace (جھوٹ);
    // Ensure Coverage table is always packed after this.
    c->serializer->جوڑو_virtual_link (coverage_idx);

    خودکار it =
      + hb_iter (component)
      | hb_map (glyph_map)
      ;

    خودکار *برآمد = c->serializer->آغاز_embed (*یہ);
    return_trace (برآمد->serialize (c->serializer,
                                  glyph_map[ligGlyph],
                                  it));  }
};


}
}
}

#اگر_ختم  /* OT_LAYOUT_GSUB_LIGATURE_HH */
