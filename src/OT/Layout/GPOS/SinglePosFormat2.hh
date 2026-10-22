#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OT_LAYOUT_GPOS_SINGLEPOSFORMAT2_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_LAYOUT_GPOS_SINGLEPOSFORMAT2_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "Common.hh"

ਨਾਮਾਕਾਸ਼ OT {
ਨਾਮਾਕਾਸ਼ Layout {
ਨਾਮਾਕਾਸ਼ GPOS_impl {

/* Keep the iterator type independent of the source table width so
 * the output serializers can share their instantiations. */
ਬਣਤਰ single_pos_ਮੁੱਲ_mapper_t
{
  ਅਚਲ hb_map_t &glyph_map;
  ਅਚਲ hb_array_t<ਅਚਲ ਮੁੱਲ> &values;
  ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ &sub_ਲੰਮਾਈ;

  hb_pair_t<hb_codepoint_t, hb_array_t<ਅਚਲ ਮੁੱਲ>>
  ਸੰਚਾਲਕ () (ਅਚਲ hb_pair_t<hb_codepoint_t, ਬਿਨਾਨਿਸ਼ਾਨੀ> &_) ਅਚਲ
  {
    ਵਾਪਸ hb_pair (glyph_map[_.ਪਹਿਲਾ],
                    values.sub_array (_.second * sub_ਲੰਮਾਈ, sub_ਲੰਮਾਈ));
  }
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types>
ਬਣਤਰ SinglePosFormat2_4 : ਮੁੱਲ_ਅਧਾਰ
{
  ਸੁਰੱਖਿਅਤ:
  HBUINT16      format;                 /* Format identifier--format = 2 */
  ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ LOffset_ਮੰਜ਼ਿਲ<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of subtable */
  ਮੁੱਲ_Format   ਮੁੱਲ_Format_2;            /* Defines the types of data in the
                                         * ValueRecord */
  ਕਿਸਮ_ਨਾਂ Types::HBUINT
                ਮੁੱਲ_ਗਿਣਤੀ;             /* Number of ValueRecords */
  ਮੁੱਲ_ਰਿਕਾਰਡ   values;                 /* Array of ValueRecords--positioning
                                         * values applied to glyphs */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (2 + Types::LOffset::static_ਮਾਪ + ਮੁੱਲ_Format::static_ਮਾਪ + Types::HBUINT::static_ਮਾਪ, values);

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) &&
                  coverage.sanitize (c, ਇਹ) &&
                  ਮੁੱਲ_Format_2.sanitize_ਮੁੱਲਾਂ (c, ਇਹ, values, ਮੁੱਲ_ਗਿਣਤੀ));
  }

  ਤਾਰਕਿਕ intersects (ਅਚਲ hb_ਸੈੱਟ_t *glyphs) ਅਚਲ
  { ਵਾਪਸ (ਇਹ+coverage).intersects (glyphs); }

  ਖਾਲੀ ਘੇਰਾ_lookups (hb_ਘੇਰਾ_lookups_context_t *c) ਅਚਲ {}
  ਖਾਲੀ collect_variation_indices (hb_collect_variation_indices_context_t *c) ਅਚਲ
  {
    ਜੇ (!ਮੁੱਲ_Format_2.has_device ()) ਵਾਪਸ;

    ਸਵੈਚਾਲਿਤ it =
    + hb_zip (ਇਹ+coverage, hb_range ((ਬਿਨਾਨਿਸ਼ਾਨੀ) ਮੁੱਲ_ਗਿਣਤੀ))
    | hb_filter (c->glyph_ਸੈੱਟ, hb_ਪਹਿਲਾ)
    ;

    ਜੇ (!it) ਵਾਪਸ;

    ਬਿਨਾਨਿਸ਼ਾਨੀ sub_ਲੰਮਾਈ = ਮੁੱਲ_Format_2.get_len ();
    ਅਚਲ hb_array_t<ਅਚਲ ਮੁੱਲ> ਮੁੱਲਾਂ_array = values.as_array (ਮੁੱਲ_ਗਿਣਤੀ * sub_ਲੰਮਾਈ);

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i : + it
                      | hb_map (hb_ਦੂਜਾ))
      ਮੁੱਲ_Format_2.collect_variation_indices (c, ਇਹ, ਮੁੱਲਾਂ_array.sub_array (i * sub_ਲੰਮਾਈ, sub_ਲੰਮਾਈ));

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

    ਜੇ (unlikely (ਸੂਚਕਾਂਕ_2 >= ਮੁੱਲ_ਗਿਣਤੀ)) return_trace (ਝੂਠ);

    ਜੇ (HB_ਬਫਰ_ਸੁਨੇਹਾ_ਹੋਰ && c->ਬਫਰ->messaging ())
    {
      c->ਬਫਰ->ਸੁਨੇਹਾ (c->font,
			  "positioning glyph at %u",
			  c->ਬਫਰ->idx);
    }

    ਮੁੱਲ_Format_2.apply_ਮੁੱਲ (c, ਇਹ,
                             &values[ਸੂਚਕਾਂਕ_2 * ਮੁੱਲ_Format_2.get_len ()],
                             ਬਫਰ->cur_pos());

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
    ਜੇ (unlikely (ਸੂਚਕਾਂਕ_2 >= ਮੁੱਲ_ਗਿਣਤੀ)) ਵਾਪਸ ਝੂਠ;

    ਮੁੱਲ_Format_2.position_single (font, ਸਾਰਣੀ_blob, direction, ਇਹ,
                                 &values[ਸੂਚਕਾਂਕ_2 * ਮੁੱਲ_Format_2.get_len ()], pos);
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
    ਸਵੈਚਾਲਿਤ ਬਾਹਰਲਾ = c->extend_min (ਇਹ);
    ਜੇ (unlikely (!ਬਾਹਰਲਾ)) ਵਾਪਸ;
    ਜੇ (unlikely (!c->check_ਸੌਂਪੋ (ਮੁੱਲ_Format_2, newFormat, HB_SERIALIZE_ERROR_INT_ਉਛਾਲ))) ਵਾਪਸ;
    ਜੇ (unlikely (!c->check_ਸੌਂਪੋ (ਮੁੱਲ_ਗਿਣਤੀ, it.len (), HB_SERIALIZE_ERROR_ARRAY_ਉਛਾਲ))) ਵਾਪਸ;

    + it
    | hb_map (hb_ਦੂਜਾ)
    | hb_apply ([&] (hb_array_t<ਅਚਲ ਮੁੱਲ> _)
    { src_format.copy_ਮੁੱਲਾਂ (c, newFormat, src, &_, layout_variation_idx_delta_map); })
    ;

    ਸਵੈਚਾਲਿਤ glyphs =
    + it
    | hb_map_retains_ਛਾਂਟੀ (hb_ਪਹਿਲਾ)
    ;

    coverage.serialize_serialize (c, glyphs);
  }

  ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  ਬਿਨਾਨਿਸ਼ਾਨੀ compute_effective_format (ਅਚਲ hb_face_t *face,
                                     Iterator it,
                                     ਤਾਰਕਿਕ is_instancing, ਤਾਰਕਿਕ strip_hints,
                                     ਤਾਰਕਿਕ has_gdef_varstore,
                                     ਅਚਲ hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਪੂਰਨ>> *varidx_delta_map) ਅਚਲ
  {
    hb_blob_t* blob = hb_face_reference_ਸਾਰਣੀ (face, HB_ਟੈਗ ('f','v','a','r'));
    ਤਾਰਕਿਕ has_fvar = (blob != hb_blob_get_ਖਾਲੀ_ਥਾਵਾਂ ());
    hb_blob_destroy (blob);

    ਬਿਨਾਨਿਸ਼ਾਨੀ new_format = 0;
    ਜੇ (is_instancing)
    {
      new_format = new_format | ਮੁੱਲ_Format_2.get_effective_format (+ it | hb_map (hb_ਦੂਜਾ), ਝੂਠ, ਝੂਠ, ਇਹ, varidx_delta_map);
    }
    /* do not strip hints for VF */
    ਨਹੀਂਤਾਂ ਜੇ (strip_hints)
    {
      ਤਾਰਕਿਕ strip = !has_fvar;
      ਜੇ (has_fvar && !has_gdef_varstore)
        strip = ਸੱਚ;
      new_format = new_format | ਮੁੱਲ_Format_2.get_effective_format (+ it | hb_map (hb_ਦੂਜਾ), strip, ਸੱਚ, ਇਹ, ਖਾਲੀ_ਸੂਚਕ);
    }
    ਨਹੀਂਤਾਂ
      new_format = ਮੁੱਲ_Format_2;

    ਵਾਪਸ new_format;
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਅਚਲ hb_ਸੈੱਟ_t &glyphset = *c->plan->glyphset_gsub ();
    ਅਚਲ hb_map_t &glyph_map = *c->plan->glyph_map;

    ਬਿਨਾਨਿਸ਼ਾਨੀ sub_ਲੰਮਾਈ = ਮੁੱਲ_Format_2.get_len ();
    ਸਵੈਚਾਲਿਤ ਮੁੱਲਾਂ_array = values.as_array ((ਬਿਨਾਨਿਸ਼ਾਨੀ) ਮੁੱਲ_ਗਿਣਤੀ * sub_ਲੰਮਾਈ);

    ਸਵੈਚਾਲਿਤ it =
    + hb_zip (ਇਹ+coverage, hb_range ((ਬਿਨਾਨਿਸ਼ਾਨੀ) ਮੁੱਲ_ਗਿਣਤੀ))
    | hb_filter (glyphset, hb_ਪਹਿਲਾ)
    | hb_map_retains_ਛਾਂਟੀ (single_pos_ਮੁੱਲ_mapper_t {glyph_map,
                                                       ਮੁੱਲਾਂ_array,
                                                       sub_ਲੰਮਾਈ})
    ;

    ਬਿਨਾਨਿਸ਼ਾਨੀ new_format = compute_effective_format (c->plan->ਸਰੋਤ, it,
                                                    ਤਾਰਕਿਕ (c->plan->normalized_coords),
                                                    ਤਾਰਕਿਕ (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING),
                                                    c->plan->has_gdef_varstore,
                                                    &c->plan->layout_variation_idx_delta_map);
    ਤਾਰਕਿਕ ret = ਤਾਰਕਿਕ (it);
    SinglePos_serialize (c->serializer, ਇਹ, ਮੁੱਲ_Format_2, it, &c->plan->layout_variation_idx_delta_map, new_format);
    return_trace (ret);
  }
};


}
}
}

#ਜੇ_ਅੰਤ /* OT_LAYOUT_GPOS_SINGLEPOSFORMAT2_HH */
