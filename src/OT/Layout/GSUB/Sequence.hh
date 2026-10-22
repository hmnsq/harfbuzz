#अगर_अघोषित OT_LAYOUT_GSUB_SEQUENCE_HH
#घोषणा OT_LAYOUT_GSUB_SEQUENCE_HH

#समावेश "Common.hh"

namespace OT {
namespace Layout {
namespace GSUB_impl {

ढाँचा <typename Types>
काष्ठा Sequence
{
  रक्षित:
  Array16Of<typename Types::HBGlyphID>
                substitute;             /* String of GlyphIDs to substitute */
  खुला:
  DEFINE_आकार_ARRAY (2, substitute);

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (substitute.sanitize (c));
  }

  bool intersects (स्थिर hb_set_t *glyphs) स्थिर
  { वापस hb_all (substitute, glyphs); }

  व्योम closure (hb_closure_context_t *c) स्थिर
  { c->output->add_array (substitute.arrayZ, substitute.len); }

  व्योम depend (hb_depend_context_t *c, hb_codepoint_t source) स्थिर
  {
    + hb_iter (substitute)
    | hb_apply ([&] (स्थिर hb_codepoint_t &target) { c->depend_तथ्य->add_gsub_lookup (source, c->lookup_सूचकांक, target); })
    ;
  }

  व्योम collect_glyphs (hb_collect_glyphs_context_t *c) स्थिर
  { c->output->add_array (substitute.arrayZ, substitute.len); }

  bool apply (hb_ot_apply_context_t *c) स्थिर
  {
    TRACE_APPLY (यह);
    अचिन्हित पूर्णांक गिनती = substitute.len;

    /* Special-case to make it in-place and not consider this
     * as a "multiplied" substitution. */
    अगर (unlikely (गिनती == 1))
    {
      अगर (HB_बफर_MESSAGE_MORE && c->बफर->messaging ())
      {
	c->बफर->sync_so_far ();
	c->बफर->message (c->font,
			    "replacing glyph at %u (multiple substitution)",
			    c->बफर->idx);
      }

      c->replace_glyph (substitute.arrayZ[0]);

      अगर (HB_बफर_MESSAGE_MORE && c->बफर->messaging ())
      {
	c->बफर->message (c->font,
			    "replaced glyph at %u (multiple substitution)",
			    c->बफर->idx - 1u);
      }

      return_trace (true);
    }
    /* Spec disallows this, but Uniscribe allows it.
     * https://github.com/harfbuzz/harfbuzz/issues/253 */
    अन्यथा अगर (unlikely (गिनती == 0))
    {
      अगर (HB_बफर_MESSAGE_MORE && c->बफर->messaging ())
      {
	c->बफर->sync_so_far ();
	c->बफर->message (c->font,
			    "deleting glyph at %u (multiple substitution)",
			    c->बफर->idx);
      }

      c->बफर->delete_glyph ();

      अगर (HB_बफर_MESSAGE_MORE && c->बफर->messaging ())
      {
	c->बफर->sync_so_far ();
	c->बफर->message (c->font,
			    "deleted glyph at %u (multiple substitution)",
			    c->बफर->idx);
      }

      return_trace (true);
    }

    अगर (HB_बफर_MESSAGE_MORE && c->बफर->messaging ())
    {
      c->बफर->sync_so_far ();
      c->बफर->message (c->font,
			  "multiplying glyph at %u",
			  c->बफर->idx);
    }

    अचिन्हित पूर्णांक klass = _hb_glyph_info_is_ligature (&c->बफर->cur()) ?
                         HB_OT_LAYOUT_GLYPH_PROPS_BASE_GLYPH : 0;
    अचिन्हित lig_id = _hb_glyph_info_get_lig_id (&c->बफर->cur());

    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
    {
      /* If is attached to a ligature, don't disturb that.
       * https://github.com/harfbuzz/harfbuzz/issues/3069 */
      अगर (!lig_id)
        _hb_glyph_info_set_lig_props_for_component (&c->बफर->cur(), i);
      c->output_glyph_for_component (substitute.arrayZ[i], klass);
    }
    c->बफर->skip_glyph ();

    अगर (HB_बफर_MESSAGE_MORE && c->बफर->messaging ())
    {
      c->बफर->sync_so_far ();

      अक्षर buf[HB_MAX_CONTEXT_लंबाई * 16] = {0};
      अक्षर *p = buf;

      क्रम (अचिन्हित i = c->बफर->idx - गिनती; i < c->बफर->idx; i++)
      {
	अगर (buf < p && माप(buf) - 1u > अचिन्हित (p - buf))
	  *p++ = ',';
	snprintf (p, माप(buf) - (p - buf), "%u", i);
	p += म_माप(p);
      }

      c->बफर->message (c->font,
			  "multiplied glyphs at %s",
			  buf);
    }

    return_trace (true);
  }

  ढाँचा <typename Iterator,
            hb_requires (hb_is_source_of (Iterator, hb_codepoint_t))>
  bool serialize (hb_serialize_context_t *c,
                  Iterator subst)
  {
    TRACE_SERIALIZE (यह);
    return_trace (substitute.serialize (c, subst));
  }

  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);
    स्थिर hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    स्थिर hb_map_t &glyph_map = *c->plan->glyph_map;

    अगर (!intersects (&glyphset)) return_trace (false);

    स्वतः it =
    + hb_iter (substitute)
    | hb_map (glyph_map)
    ;

    स्वतः *out = c->serializer->start_embed (*यह);
    return_trace (out->serialize (c->serializer, it));
  }
};


}
}
}


#पूर्ण_अगर /* OT_LAYOUT_GSUB_SEQUENCE_HH */
