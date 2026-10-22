#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OT_LAYOUT_GPOS_SINGLEPOSFORMAT1_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_LAYOUT_GPOS_SINGLEPOSFORMAT1_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "Common.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "ValueFormat.hh"

ਨਾਮਾਕਾਸ਼ OT {
ਨਾਮਾਕਾਸ਼ Layout {
ਨਾਮਾਕਾਸ਼ GPOS_impl {

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types>
ਬਣਤਰ SinglePosFormat1_3 : ਮੁੱਲ_ਅਧਾਰ
{
  ਸੁਰੱਖਿਅਤ:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ LOffset_ਮੰਜ਼ਿਲ<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of subtable */
  ਮੁੱਲ_Format   ਮੁੱਲ_Format_2;            /* Defines the types of data in the
                                         * ValueRecord */
  ਮੁੱਲ_ਰਿਕਾਰਡ   values;                 /* Defines positioning
                                         * value(s)--applied to all glyphs in
                                         * the Coverage table */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (2 + Types::LOffset::static_ਮਾਪ + ਮੁੱਲ_Format::static_ਮਾਪ, values);

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) &&
                  coverage.sanitize (c, ਇਹ) &&
		  hb_barrier () &&
                  /* The coverage  table may use a range to represent a set
                   * of glyphs, which means a small number of bytes can
                   * generate a large glyph set. Manually modify the
                   * sanitizer max ops to take this into account.
                   *
                   * Note: This check *must* be right after coverage sanitize. */
                  c->check_ops ((ਇਹ + coverage).get_population () >> 1) &&
                  ਮੁੱਲ_Format_2.sanitize_ਮੁੱਲ (c, ਇਹ, values));

  }

  ਤਾਰਕਿਕ intersects (ਅਚਲ hb_ਸੈੱਟ_t *glyphs) ਅਚਲ
  { ਵਾਪਸ (ਇਹ+coverage).intersects (glyphs); }

  ਖਾਲੀ ਘੇਰਾ_lookups (hb_ਘੇਰਾ_lookups_context_t *c) ਅਚਲ {}
  ਖਾਲੀ collect_variation_indices (hb_collect_variation_indices_context_t *c) ਅਚਲ
  {
    ਜੇ (!ਮੁੱਲ_Format_2.has_device ()) ਵਾਪਸ;

    hb_ਸੈੱਟ_t intersection;
    (ਇਹ+coverage).intersect_ਸੈੱਟ (*c->glyph_ਸੈੱਟ, intersection);
    ਜੇ (!intersection) ਵਾਪਸ;

    ਮੁੱਲ_Format_2.collect_variation_indices (c, ਇਹ, values.as_array (ਮੁੱਲ_Format_2.get_len ()));
  }

  ਖਾਲੀ collect_glyphs (hb_collect_glyphs_context_t *c) ਅਚਲ
  { ਜੇ (unlikely (!(ਇਹ+coverage).collect_coverage (c->ਨਿਵੇਸ਼))) ਵਾਪਸ; }

  ਅਚਲ Coverage &get_coverage () ਅਚਲ { ਵਾਪਸ ਇਹ+coverage; }

  ਮੁੱਲ_Format get_ਮੁੱਲ_format () ਅਚਲ { ਵਾਪਸ ਮੁੱਲ_Format_2; }

  ਤਾਰਕਿਕ apply (hb_ot_apply_context_t *c) ਅਚਲ
  {
    TRACE_APPLY (ਇਹ);
    hb_ਬਫਰ_t *ਬਫਰ = c->ਬਫਰ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੂਚਕਾਂਕ_2 = (ਇਹ+coverage).get_coverage  (ਬਫਰ->cur().codepoint);
    ਜੇ (ਸੂਚਕਾਂਕ_2 == NOT_COVERED) return_trace (ਝੂਠ);

    ਜੇ (HB_ਬਫਰ_ਸੁਨੇਹਾ_ਹੋਰ && c->ਬਫਰ->messaging ())
    {
      c->ਬਫਰ->ਸੁਨੇਹਾ (c->font,
			  "positioning glyph at %u",
			  c->ਬਫਰ->idx);
    }

    ਮੁੱਲ_Format_2.apply_ਮੁੱਲ (c, ਇਹ, values, ਬਫਰ->cur_pos());

    ਜੇ (HB_ਬਫਰ_ਸੁਨੇਹਾ_ਹੋਰ && c->ਬਫਰ->messaging ())
    {
      c->ਬਫਰ->ਸੁਨੇਹਾ (c->font,
			  "positioned glyph at %u",
			  c->ਬਫਰ->idx);
    }

    ਬਫਰ->idx++;
    return_trace (ਸੱਚ);
  }

  ਤਾਰਕਿਕ
  position_single (hb_font_t           *font,
		   hb_blob_t           *ਸਾਰਣੀ_blob,
		   hb_direction_t       direction,
		   hb_codepoint_t       gid,
		   hb_glyph_position_t &pos) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੂਚਕਾਂਕ_2 = (ਇਹ+coverage).get_coverage  (gid);
    ਜੇ (likely (ਸੂਚਕਾਂਕ_2 == NOT_COVERED)) ਵਾਪਸ ਝੂਠ;

    ਮੁੱਲ_Format_2.position_single (font, ਸਾਰਣੀ_blob, direction, ਇਹ,
                                 values, pos);
    ਵਾਪਸ ਸੱਚ;
  }

  ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  ਖਾਲੀ serialize (hb_serialize_context_t *c,
                  ਅਚਲ ਮੁੱਲ_ਅਧਾਰ *src,
                  ਮੁੱਲ_Format src_format,
                  Iterator it,
                  ਮੁੱਲ_Format newFormat,
                  ਅਚਲ hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਪੂਰਨ>> *layout_variation_idx_delta_map)
  {
    ਜੇ (unlikely (!c->extend_min (ਇਹ))) ਵਾਪਸ;
    ਜੇ (unlikely (!c->check_ਸੌਂਪੋ (ਮੁੱਲ_Format_2,
                                    newFormat,
                                    HB_SERIALIZE_ERROR_INT_ਉਛਾਲ))) ਵਾਪਸ;

    ਲਈ (ਅਚਲ hb_array_t<ਅਚਲ ਮੁੱਲ>& _ : + it | hb_map (hb_ਦੂਜਾ))
    {
      src_format.copy_ਮੁੱਲਾਂ (c, newFormat, src,  &_, layout_variation_idx_delta_map);
      // Only serialize the first entry in the iterator, the rest are assumed to
      // be the same.
      ਰੋਕੋ;
    }

    ਸਵੈਚਾਲਿਤ glyphs =
    + it
    | hb_map_retains_ਛਾਂਟੀ (hb_ਪਹਿਲਾ)
    ;

    coverage.serialize_serialize (c, glyphs);
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਅਚਲ hb_ਸੈੱਟ_t &glyphset = *c->plan->glyphset_gsub ();
    ਅਚਲ hb_map_t &glyph_map = *c->plan->glyph_map;

    hb_ਸੈੱਟ_t intersection;
    (ਇਹ+coverage).intersect_ਸੈੱਟ (glyphset, intersection);

    ਬਿਨਾਨਿਸ਼ਾਨੀ new_format = ਮੁੱਲ_Format_2;

    ਜੇ (c->plan->normalized_coords)
    {
      new_format = ਮੁੱਲ_Format_2.get_effective_format (values.arrayZ, ਝੂਠ, ਝੂਠ, ਇਹ, &c->plan->layout_variation_idx_delta_map);
    }
    /* do not strip hints for VF */
    ਨਹੀਂਤਾਂ ਜੇ (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
    {
      hb_blob_t* blob = hb_face_reference_ਸਾਰਣੀ (c->plan->ਸਰੋਤ, HB_ਟੈਗ ('f','v','a','r'));
      ਤਾਰਕਿਕ has_fvar = (blob != hb_blob_get_ਖਾਲੀ_ਥਾਵਾਂ ());
      hb_blob_destroy (blob);

      ਤਾਰਕਿਕ strip = !has_fvar;
      /* special case: strip hints when a VF has no GDEF varstore after
       * subsetting*/
      ਜੇ (has_fvar && !c->plan->has_gdef_varstore)
        strip = ਸੱਚ;
      new_format = ਮੁੱਲ_Format_2.get_effective_format (values.arrayZ,
                                                     strip, /* strip hints */
                                                     ਸੱਚ, /* strip empty */
                                                     ਇਹ, ਖਾਲੀ_ਸੂਚਕ);
    }

    ਸਵੈਚਾਲਿਤ it =
    + hb_iter (intersection)
    | hb_map_retains_ਛਾਂਟੀ (glyph_map)
    | hb_zip (hb_repeat (values.as_array (ਮੁੱਲ_Format_2.get_len ())))
    ;

    ਤਾਰਕਿਕ ret = ਤਾਰਕਿਕ (it);
    SinglePos_serialize (c->serializer, ਇਹ, ਮੁੱਲ_Format_2, it, &c->plan->layout_variation_idx_delta_map, new_format);
    return_trace (ret);
  }
};

}
}
}

#ਜੇ_ਅੰਤ /* OT_LAYOUT_GPOS_SINGLEPOSFORMAT1_HH */
