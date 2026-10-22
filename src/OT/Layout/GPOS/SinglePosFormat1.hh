#ifndef OT_LAYOUT_GPOS_SINGLEPOSFORMAT1_HH
#define OT_LAYOUT_GPOS_SINGLEPOSFORMAT1_HH

#ضمن "Common.hh"
#ضمن "ValueFormat.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {

template <typename Types>
بنية SinglePosFormat1_3 : ValueBase
{
  محمي:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of subtable */
  ValueFormat   valueFormat;            /* Defines the types of data in the
                                         * ValueRecord */
  ValueRecord   values;                 /* Defines positioning
                                         * value(s)--applied to all glyphs in
                                         * the Coverage table */
  عام:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + ValueFormat::static_size, values);

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
                  coverage.sanitize (c, هذا) &&
		  hb_barrier () &&
                  /* The coverage  table may use a range to represent a set
                   * of glyphs, which means a small number of bytes can
                   * generate a large glyph set. Manually modify the
                   * sanitizer max ops to take this into account.
                   *
                   * Note: This check *must* be right after coverage sanitize. */
                  c->check_ops ((هذا + coverage).get_population () >> 1) &&
                  valueFormat.sanitize_value (c, هذا, values));

  }

  منطقي intersects (ثابت hb_set_t *glyphs) ثابت
  { أرجع (هذا+coverage).intersects (glyphs); }

  فراغ closure_lookups (hb_closure_lookups_context_t *c) ثابت {}
  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    𐤀𐤌 (!valueFormat.has_device ()) أرجع;

    hb_set_t intersection;
    (هذا+coverage).intersect_set (*c->glyph_set, intersection);
    𐤀𐤌 (!intersection) أرجع;

    valueFormat.collect_variation_indices (c, هذا, values.as_array (valueFormat.get_len ()));
  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  { 𐤀𐤌 (unlikely (!(هذا+coverage).collect_coverage (c->input))) أرجع; }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+coverage; }

  ValueFormat get_value_format () ثابت { أرجع valueFormat; }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);
    hb_buffer_t *buffer = c->buffer;
    unsigned صحيح index = (هذا+coverage).get_coverage  (buffer->cur().codepoint);
    𐤀𐤌 (index == NOT_COVERED) return_trace (خطأ);

    𐤀𐤌 (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "positioning glyph at %u",
			  c->buffer->idx);
    }

    valueFormat.apply_value (c, هذا, values, buffer->cur_pos());

    𐤀𐤌 (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "positioned glyph at %u",
			  c->buffer->idx);
    }

    buffer->idx++;
    return_trace (true);
  }

  منطقي
  position_single (hb_font_t           *font,
		   hb_blob_t           *table_blob,
		   hb_direction_t       direction,
		   hb_codepoint_t       gid,
		   hb_glyph_position_t &pos) ثابت
  {
    unsigned صحيح index = (هذا+coverage).get_coverage  (gid);
    𐤀𐤌 (likely (index == NOT_COVERED)) أرجع خطأ;

    valueFormat.position_single (font, table_blob, direction, هذا,
                                 values, pos);
    أرجع true;
  }

  template<typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  فراغ serialize (hb_serialize_context_t *c,
                  ثابت ValueBase *src,
                  ValueFormat src_format,
                  Iterator it,
                  ValueFormat newFormat,
                  ثابت hb_hashmap_t<unsigned, hb_pair_t<unsigned, صحيح>> *layout_variation_idx_delta_map)
  {
    𐤀𐤌 (unlikely (!c->extend_min (هذا))) أرجع;
    𐤀𐤌 (unlikely (!c->check_assign (valueFormat,
                                    newFormat,
                                    HB_SERIALIZE_ERROR_INT_OVERFLOW))) أرجع;

    لكل (ثابت hb_array_t<ثابت Value>& _ : + it | hb_map (hb_second))
    {
      src_format.copy_values (c, newFormat, src,  &_, layout_variation_idx_delta_map);
      // Only serialize the first entry in the iterator, the rest are assumed to
      // be the same.
      اكسر;
    }

    auto glyphs =
    + it
    | hb_map_retains_sorting (hb_first)
    ;

    coverage.serialize_serialize (c, glyphs);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    hb_set_t intersection;
    (هذا+coverage).intersect_set (glyphset, intersection);

    unsigned new_format = valueFormat;

    𐤀𐤌 (c->plan->normalized_coords)
    {
      new_format = valueFormat.get_effective_format (values.arrayZ, خطأ, خطأ, هذا, &c->plan->layout_variation_idx_delta_map);
    }
    /* do not strip hints for VF */
    وإلا 𐤀𐤌 (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
    {
      hb_blob_t* blob = hb_face_reference_table (c->plan->source, HB_TAG ('f','v','a','r'));
      منطقي has_fvar = (blob != hb_blob_get_empty ());
      hb_blob_destroy (blob);

      منطقي strip = !has_fvar;
      /* special case: strip hints when a VF has no GDEF varstore after
       * subsetting*/
      𐤀𐤌 (has_fvar && !c->plan->has_gdef_varstore)
        strip = true;
      new_format = valueFormat.get_effective_format (values.arrayZ,
                                                     strip, /* strip hints */
                                                     true, /* strip empty */
                                                     هذا, nullptr);
    }

    auto it =
    + hb_iter (intersection)
    | hb_map_retains_sorting (glyph_map)
    | hb_zip (hb_repeat (values.as_array (valueFormat.get_len ())))
    ;

    منطقي ret = منطقي (it);
    SinglePos_serialize (c->serializer, هذا, valueFormat, it, &c->plan->layout_variation_idx_delta_map, new_format);
    return_trace (ret);
  }
};

}
}
}

#endif /* OT_LAYOUT_GPOS_SINGLEPOSFORMAT1_HH */
