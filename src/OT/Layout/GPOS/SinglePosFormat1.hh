#ifndef OT_LAYOUT_GPOS_SINGLEPOSFORMAT1_HH
#define OT_LAYOUT_GPOS_SINGLEPOSFORMAT1_HH

#அந்தர்பாவ "Common.hh"
#அந்தர்பாவ "ValueFormat.hh"

நாமாகாஶ OT {
நாமாகாஶ Layout {
நாமாகாஶ GPOS_impl {

template <typename Types>
கட்டமைப்பு SinglePosFormat1_3 : மதிப்பு_Base
{
  பாதுகாக்கப்பட்ட:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of subtable */
  மதிப்பு_Format   மதிப்பு_Format_2;            /* Defines the types of data in the
                                         * ValueRecord */
  மதிப்பு_Record   values;                 /* Defines positioning
                                         * value(s)--applied to all glyphs in
                                         * the Coverage table */
  பொது:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + மதிப்பு_Format::static_size, values);

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) &&
                  coverage.sanitize (c, இது) &&
		  hb_barrier () &&
                  /* The coverage  table may use a range to represent a set
                   * of glyphs, which means a small number of bytes can
                   * generate a large glyph set. Manually modify the
                   * sanitizer max ops to take this into account.
                   *
                   * Note: This check *must* be right after coverage sanitize. */
                  c->check_ops ((இது + coverage).get_population () >> 1) &&
                  மதிப்பு_Format_2.sanitize_மதிப்பு (c, இது, values));

  }

  தருக்க intersects (மாறிலி hb_set_t *glyphs) மாறிலி
  { திரும்பு (இது+coverage).intersects (glyphs); }

  வெறுமை closure_lookups (hb_closure_lookups_context_t *c) மாறிலி {}
  வெறுமை collect_variation_indices (hb_collect_variation_indices_context_t *c) மாறிலி
  {
    எனில் (!மதிப்பு_Format_2.has_device ()) திரும்பு;

    hb_set_t intersection;
    (இது+coverage).intersect_set (*c->glyph_set, intersection);
    எனில் (!intersection) திரும்பு;

    மதிப்பு_Format_2.collect_variation_indices (c, இது, values.as_array (மதிப்பு_Format_2.get_len ()));
  }

  வெறுமை collect_glyphs (hb_collect_glyphs_context_t *c) மாறிலி
  { எனில் (unlikely (!(இது+coverage).collect_coverage (c->உள்ளீடு))) திரும்பு; }

  மாறிலி Coverage &get_coverage () மாறிலி { திரும்பு இது+coverage; }

  மதிப்பு_Format get_மதிப்பு_format () மாறிலி { திரும்பு மதிப்பு_Format_2; }

  தருக்க apply (hb_ot_apply_context_t *c) மாறிலி
  {
    TRACE_APPLY (இது);
    hb_buffer_t *buffer = c->buffer;
    unsigned முழு index = (இது+coverage).get_coverage  (buffer->cur().codepoint);
    எனில் (index == NOT_COVERED) return_trace (பொய்);

    எனில் (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "positioning glyph at %u",
			  c->buffer->idx);
    }

    மதிப்பு_Format_2.apply_மதிப்பு (c, இது, values, buffer->cur_pos());

    எனில் (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "positioned glyph at %u",
			  c->buffer->idx);
    }

    buffer->idx++;
    return_trace (உண்மை);
  }

  தருக்க
  position_single (hb_font_t           *font,
		   hb_blob_t           *table_blob,
		   hb_direction_t       direction,
		   hb_codepoint_t       gid,
		   hb_glyph_position_t &pos) மாறிலி
  {
    unsigned முழு index = (இது+coverage).get_coverage  (gid);
    எனில் (likely (index == NOT_COVERED)) திரும்பு பொய்;

    மதிப்பு_Format_2.position_single (font, table_blob, direction, இது,
                                 values, pos);
    திரும்பு உண்மை;
  }

  template<typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  வெறுமை serialize (hb_serialize_context_t *c,
                  மாறிலி மதிப்பு_Base *src,
                  மதிப்பு_Format src_format,
                  Iterator it,
                  மதிப்பு_Format newFormat,
                  மாறிலி hb_hashmap_t<unsigned, hb_pair_t<unsigned, முழு>> *layout_variation_idx_delta_map)
  {
    எனில் (unlikely (!c->extend_min (இது))) திரும்பு;
    எனில் (unlikely (!c->check_assign (மதிப்பு_Format_2,
                                    newFormat,
                                    HB_SERIALIZE_ERROR_INT_OVERFLOW))) திரும்பு;

    க்கு (மாறிலி hb_array_t<மாறிலி மதிப்பு>& _ : + it | hb_map (hb_second))
    {
      src_format.copy_மதிப்புகள் (c, newFormat, src,  &_, layout_variation_idx_delta_map);
      // Only serialize the first entry in the iterator, the rest are assumed to
      // be the same.
      நிறுத்து;
    }

    auto glyphs =
    + it
    | hb_map_retains_sorting (hb_first)
    ;

    coverage.serialize_serialize (c, glyphs);
  }

  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);
    மாறிலி hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    மாறிலி hb_map_t &glyph_map = *c->plan->glyph_map;

    hb_set_t intersection;
    (இது+coverage).intersect_set (glyphset, intersection);

    unsigned new_format = மதிப்பு_Format_2;

    எனில் (c->plan->normalized_coords)
    {
      new_format = மதிப்பு_Format_2.get_effective_format (values.arrayZ, பொய், பொய், இது, &c->plan->layout_variation_idx_delta_map);
    }
    /* do not strip hints for VF */
    இல்லையெனில் எனில் (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
    {
      hb_blob_t* blob = hb_face_reference_table (c->plan->source, HB_TAG ('f','v','a','r'));
      தருக்க has_fvar = (blob != hb_blob_get_empty ());
      hb_blob_destroy (blob);

      தருக்க strip = !has_fvar;
      /* special case: strip hints when a VF has no GDEF varstore after
       * subsetting*/
      எனில் (has_fvar && !c->plan->has_gdef_varstore)
        strip = உண்மை;
      new_format = மதிப்பு_Format_2.get_effective_format (values.arrayZ,
                                                     strip, /* strip hints */
                                                     உண்மை, /* strip empty */
                                                     இது, nullptr);
    }

    auto it =
    + hb_iter (intersection)
    | hb_map_retains_sorting (glyph_map)
    | hb_zip (hb_repeat (values.as_array (மதிப்பு_Format_2.get_len ())))
    ;

    தருக்க ret = தருக்க (it);
    SinglePos_serialize (c->serializer, இது, மதிப்பு_Format_2, it, &c->plan->layout_variation_idx_delta_map, new_format);
    return_trace (ret);
  }
};

}
}
}

#endif /* OT_LAYOUT_GPOS_SINGLEPOSFORMAT1_HH */
