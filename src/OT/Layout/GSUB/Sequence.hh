#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OT_LAYOUT_GSUB_SEQUENCE_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_LAYOUT_GSUB_SEQUENCE_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "Common.hh"

ਨਾਮਾਕਾਸ਼ OT {
ਨਾਮਾਕਾਸ਼ Layout {
ਨਾਮਾਕਾਸ਼ GSUB_impl {

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types>
ਬਣਤਰ Sequence
{
  ਸੁਰੱਖਿਅਤ:
  Array16_ਦਾ<ਕਿਸਮ_ਨਾਂ Types::HBGlyphID>
                substitute;             /* String of GlyphIDs to substitute */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (2, substitute);

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (substitute.sanitize (c));
  }

  ਤਾਰਕਿਕ intersects (ਅਚਲ hb_ਸੈੱਟ_t *glyphs) ਅਚਲ
  { ਵਾਪਸ hb_all (substitute, glyphs); }

  ਖਾਲੀ ਘੇਰਾ (hb_ਘੇਰਾ_context_t *c) ਅਚਲ
  { c->output->ਜੋੜੋ_array (substitute.arrayZ, substitute.len); }

  ਖਾਲੀ depend (hb_depend_context_t *c, hb_codepoint_t ਸਰੋਤ) ਅਚਲ
  {
    + hb_iter (substitute)
    | hb_apply ([&] (ਅਚਲ hb_codepoint_t &target) { c->depend_ਅੰਕੜੇ->ਜੋੜੋ_gsub_lookup (ਸਰੋਤ, c->lookup_ਸੂਚਕਾਂਕ, target); })
    ;
  }

  ਖਾਲੀ collect_glyphs (hb_collect_glyphs_context_t *c) ਅਚਲ
  { c->output->ਜੋੜੋ_array (substitute.arrayZ, substitute.len); }

  ਤਾਰਕਿਕ apply (hb_ot_apply_context_t *c) ਅਚਲ
  {
    TRACE_APPLY (ਇਹ);
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = substitute.len;

    /* Special-case to make it in-place and not consider this
     * as a "multiplied" substitution. */
    ਜੇ (unlikely (ਗਿਣਤੀ_2 == 1))
    {
      ਜੇ (HB_ਬਫਰ_ਸੁਨੇਹਾ_ਹੋਰ && c->ਬਫਰ->messaging ())
      {
	c->ਬਫਰ->sync_so_far ();
	c->ਬਫਰ->ਸੁਨੇਹਾ (c->font,
			    "replacing glyph at %u (multiple substitution)",
			    c->ਬਫਰ->idx);
      }

      c->replace_glyph (substitute.arrayZ[0]);

      ਜੇ (HB_ਬਫਰ_ਸੁਨੇਹਾ_ਹੋਰ && c->ਬਫਰ->messaging ())
      {
	c->ਬਫਰ->ਸੁਨੇਹਾ (c->font,
			    "replaced glyph at %u (multiple substitution)",
			    c->ਬਫਰ->idx - 1u);
      }

      return_trace (ਸੱਚ);
    }
    /* Spec disallows this, but Uniscribe allows it.
     * https://github.com/harfbuzz/harfbuzz/issues/253 */
    ਨਹੀਂਤਾਂ ਜੇ (unlikely (ਗਿਣਤੀ_2 == 0))
    {
      ਜੇ (HB_ਬਫਰ_ਸੁਨੇਹਾ_ਹੋਰ && c->ਬਫਰ->messaging ())
      {
	c->ਬਫਰ->sync_so_far ();
	c->ਬਫਰ->ਸੁਨੇਹਾ (c->font,
			    "deleting glyph at %u (multiple substitution)",
			    c->ਬਫਰ->idx);
      }

      c->ਬਫਰ->delete_glyph ();

      ਜੇ (HB_ਬਫਰ_ਸੁਨੇਹਾ_ਹੋਰ && c->ਬਫਰ->messaging ())
      {
	c->ਬਫਰ->sync_so_far ();
	c->ਬਫਰ->ਸੁਨੇਹਾ (c->font,
			    "deleted glyph at %u (multiple substitution)",
			    c->ਬਫਰ->idx);
      }

      return_trace (ਸੱਚ);
    }

    ਜੇ (HB_ਬਫਰ_ਸੁਨੇਹਾ_ਹੋਰ && c->ਬਫਰ->messaging ())
    {
      c->ਬਫਰ->sync_so_far ();
      c->ਬਫਰ->ਸੁਨੇਹਾ (c->font,
			  "multiplying glyph at %u",
			  c->ਬਫਰ->idx);
    }

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ klass = _hb_glyph_info_is_ligature (&c->ਬਫਰ->cur()) ?
                         HB_OT_LAYOUT_GLYPH_PROPS_ਅਧਾਰ_GLYPH : 0;
    ਬਿਨਾਨਿਸ਼ਾਨੀ lig_id = _hb_glyph_info_get_lig_id (&c->ਬਫਰ->cur());

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      /* If is attached to a ligature, don't disturb that.
       * https://github.com/harfbuzz/harfbuzz/issues/3069 */
      ਜੇ (!lig_id)
        hb_glyph_info_ਸੈੱਟ_lig_props_for_component (&c->ਬਫਰ->cur(), i);
      c->output_glyph_for_component (substitute.arrayZ[i], klass);
    }
    c->ਬਫਰ->skip_glyph ();

    ਜੇ (HB_ਬਫਰ_ਸੁਨੇਹਾ_ਹੋਰ && c->ਬਫਰ->messaging ())
    {
      c->ਬਫਰ->sync_so_far ();

      ਅੱਖਰ buf[HB_MAX_CONTEXT_ਲੰਮਾਈ * 16] = {0};
      ਅੱਖਰ *p = buf;

      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = c->ਬਫਰ->idx - ਗਿਣਤੀ_2; i < c->ਬਫਰ->idx; i++)
      {
	ਜੇ (buf < p && ਆਕਾਰ(buf) - 1u > ਬਿਨਾਨਿਸ਼ਾਨੀ (p - buf))
	  *p++ = ',';
	snprintf (p, ਆਕਾਰ(buf) - (p - buf), "%u", i);
	p += ਲੜੀ_ਲੰਬਾਈ(p);
      }

      c->ਬਫਰ->ਸੁਨੇਹਾ (c->font,
			  "multiplied glyphs at %s",
			  buf);
    }

    return_trace (ਸੱਚ);
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Iterator,
            hb_requires (hb_is_ਸਰੋਤ_ਦਾ (Iterator, hb_codepoint_t))>
  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c,
                  Iterator subst)
  {
    TRACE_SERIALIZE (ਇਹ);
    return_trace (substitute.serialize (c, subst));
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਅਚਲ hb_ਸੈੱਟ_t &glyphset = *c->plan->glyphset_gsub ();
    ਅਚਲ hb_map_t &glyph_map = *c->plan->glyph_map;

    ਜੇ (!intersects (&glyphset)) return_trace (ਝੂਠ);

    ਸਵੈਚਾਲਿਤ it =
    + hb_iter (substitute)
    | hb_map (glyph_map)
    ;

    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->ਸ਼ੁਰੂ_embed (*ਇਹ);
    return_trace (ਬਾਹਰਲਾ->serialize (c->serializer, it));
  }
};


}
}
}


#ਜੇ_ਅੰਤ /* OT_LAYOUT_GSUB_SEQUENCE_HH */
