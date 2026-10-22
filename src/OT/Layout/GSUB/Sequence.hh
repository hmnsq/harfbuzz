#ifndef OT_LAYOUT_GSUB_SEQUENCE_HH
#define OT_LAYOUT_GSUB_SEQUENCE_HH

#অন্তর্ভাব "Common.hh"

নামাকাশ OT {
নামাকাশ Layout {
নামাকাশ GSUB_impl {

template <typename Types>
কাঠামো Sequence
{
  সুরক্ষিত:
  Array16Of<typename Types::HBGlyphID>
                substitute;             /* String of GlyphIDs to substitute */
  সর্বজনীন:
  DEFINE_আকার_ARRAY (2, substitute);

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (substitute.sanitize (c));
  }

  যৌক্তিক intersects (ধ্রুবক hb_set_t *glyphs) ধ্রুবক
  { ফেরত hb_all (substitute, glyphs); }

  শূন্য closure (hb_closure_context_t *c) ধ্রুবক
  { c->output->add_array (substitute.arrayZ, substitute.len); }

  শূন্য depend (hb_depend_context_t *c, hb_codepoint_t source) ধ্রুবক
  {
    + hb_iter (substitute)
    | hb_apply ([&] (ধ্রুবক hb_codepoint_t &target) { c->depend_তথ্য->add_gsub_lookup (source, c->lookup_সূচক, target); })
    ;
  }

  শূন্য collect_glyphs (hb_collect_glyphs_context_t *c) ধ্রুবক
  { c->output->add_array (substitute.arrayZ, substitute.len); }

  যৌক্তিক apply (hb_ot_apply_context_t *c) ধ্রুবক
  {
    TRACE_APPLY (এই);
    unsigned পূর্ণসংখ্যা গুনতি = substitute.len;

    /* Special-case to make it in-place and not consider this
     * as a "multiplied" substitution. */
    যদি (unlikely (গুনতি == 1))
    {
      যদি (HB_বাফার_MESSAGE_MORE && c->বাফার->messaging ())
      {
	c->বাফার->sync_so_far ();
	c->বাফার->message (c->font,
			    "replacing glyph at %u (multiple substitution)",
			    c->বাফার->idx);
      }

      c->replace_glyph (substitute.arrayZ[0]);

      যদি (HB_বাফার_MESSAGE_MORE && c->বাফার->messaging ())
      {
	c->বাফার->message (c->font,
			    "replaced glyph at %u (multiple substitution)",
			    c->বাফার->idx - 1u);
      }

      return_trace (সত্য);
    }
    /* Spec disallows this, but Uniscribe allows it.
     * https://github.com/harfbuzz/harfbuzz/issues/253 */
    অন্যথা যদি (unlikely (গুনতি == 0))
    {
      যদি (HB_বাফার_MESSAGE_MORE && c->বাফার->messaging ())
      {
	c->বাফার->sync_so_far ();
	c->বাফার->message (c->font,
			    "deleting glyph at %u (multiple substitution)",
			    c->বাফার->idx);
      }

      c->বাফার->delete_glyph ();

      যদি (HB_বাফার_MESSAGE_MORE && c->বাফার->messaging ())
      {
	c->বাফার->sync_so_far ();
	c->বাফার->message (c->font,
			    "deleted glyph at %u (multiple substitution)",
			    c->বাফার->idx);
      }

      return_trace (সত্য);
    }

    যদি (HB_বাফার_MESSAGE_MORE && c->বাফার->messaging ())
    {
      c->বাফার->sync_so_far ();
      c->বাফার->message (c->font,
			  "multiplying glyph at %u",
			  c->বাফার->idx);
    }

    unsigned পূর্ণসংখ্যা klass = _hb_glyph_info_is_ligature (&c->বাফার->cur()) ?
                         HB_OT_LAYOUT_GLYPH_PROPS_BASE_GLYPH : 0;
    unsigned lig_id = _hb_glyph_info_get_lig_id (&c->বাফার->cur());

    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
    {
      /* If is attached to a ligature, don't disturb that.
       * https://github.com/harfbuzz/harfbuzz/issues/3069 */
      যদি (!lig_id)
        _hb_glyph_info_set_lig_props_for_component (&c->বাফার->cur(), i);
      c->output_glyph_for_component (substitute.arrayZ[i], klass);
    }
    c->বাফার->skip_glyph ();

    যদি (HB_বাফার_MESSAGE_MORE && c->বাফার->messaging ())
    {
      c->বাফার->sync_so_far ();

      অক্ষর buf[HB_MAX_CONTEXT_দৈর্ঘ্য * 16] = {0};
      অক্ষর *p = buf;

      জন্য (unsigned i = c->বাফার->idx - গুনতি; i < c->বাফার->idx; i++)
      {
	যদি (buf < p && আকার(buf) - 1u > unsigned (p - buf))
	  *p++ = ',';
	snprintf (p, আকার(buf) - (p - buf), "%u", i);
	p += strlen(p);
      }

      c->বাফার->message (c->font,
			  "multiplied glyphs at %s",
			  buf);
    }

    return_trace (সত্য);
  }

  template <typename Iterator,
            hb_requires (hb_is_source_of (Iterator, hb_codepoint_t))>
  যৌক্তিক serialize (hb_serialize_context_t *c,
                  Iterator subst)
  {
    TRACE_SERIALIZE (এই);
    return_trace (substitute.serialize (c, subst));
  }

  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    ধ্রুবক hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ধ্রুবক hb_map_t &glyph_map = *c->plan->glyph_map;

    যদি (!intersects (&glyphset)) return_trace (মিথ্যা);

    auto it =
    + hb_iter (substitute)
    | hb_map (glyph_map)
    ;

    auto *out = c->serializer->start_embed (*এই);
    return_trace (out->serialize (c->serializer, it));
  }
};


}
}
}


#endif /* OT_LAYOUT_GSUB_SEQUENCE_HH */
