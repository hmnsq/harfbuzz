#ifndef OT_LAYOUT_GPOS_SINGLEPOSFORMAT1_HH
#define OT_LAYOUT_GPOS_SINGLEPOSFORMAT1_HH

#अन्तर्भाव "Common.hh"
#अन्तर्भाव "ValueFormat.hh"

नामाकाश OT {
नामाकाश Layout {
नामाकाश GPOS_impl {

template <typename Types>
संरचना SinglePosFormat1_3 : मानम्_Base
{
  रक्षित:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of subtable */
  मानम्_Format   मानम्_Format_2;            /* Defines the types of data in the
                                         * ValueRecord */
  मानम्_Record   values;                 /* Defines positioning
                                         * value(s)--applied to all glyphs in
                                         * the Coverage table */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + मानम्_Format::static_size, values);

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
                  coverage.sanitize (c, एतत्) &&
		  hb_barrier () &&
                  /* The coverage  table may use a range to represent a set
                   * of glyphs, which means a small number of bytes can
                   * generate a large glyph set. Manually modify the
                   * sanitizer max ops to take this into account.
                   *
                   * Note: This check *must* be right after coverage sanitize. */
                  c->check_ops ((एतत् + coverage).get_population () >> 1) &&
                  मानम्_Format_2.sanitize_मानम् (c, एतत्, values));

  }

  तार्किक intersects (ध्रुव hb_set_t *glyphs) ध्रुव
  { प्रत्यागम (एतत्+coverage).intersects (glyphs); }

  शून्य closure_lookups (hb_closure_lookups_context_t *c) ध्रुव {}
  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) ध्रुव
  {
    जइ (!मानम्_Format_2.has_device ()) प्रत्यागम;

    hb_set_t intersection;
    (एतत्+coverage).intersect_set (*c->glyph_set, intersection);
    जइ (!intersection) प्रत्यागम;

    मानम्_Format_2.collect_variation_indices (c, एतत्, values.as_array (मानम्_Format_2.get_len ()));
  }

  शून्य collect_glyphs (hb_collect_glyphs_context_t *c) ध्रुव
  { जइ (unlikely (!(एतत्+coverage).collect_coverage (c->निवेशः))) प्रत्यागम; }

  ध्रुव Coverage &get_coverage () ध्रुव { प्रत्यागम एतत्+coverage; }

  मानम्_Format get_मानम्_format () ध्रुव { प्रत्यागम मानम्_Format_2; }

  तार्किक apply (hb_ot_apply_context_t *c) ध्रुव
  {
    TRACE_APPLY (एतत्);
    hb_buffer_t *buffer = c->buffer;
    unsigned पूर्णाङ्क index = (एतत्+coverage).get_coverage  (buffer->cur().codepoint);
    जइ (index == NOT_COVERED) return_trace (मिच्छ);

    जइ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "positioning glyph at %u",
			  c->buffer->idx);
    }

    मानम्_Format_2.apply_मानम् (c, एतत्, values, buffer->cur_pos());

    जइ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "positioned glyph at %u",
			  c->buffer->idx);
    }

    buffer->idx++;
    return_trace (सच्च);
  }

  तार्किक
  position_single (hb_font_t           *font,
		   hb_blob_t           *table_blob,
		   hb_direction_t       direction,
		   hb_codepoint_t       gid,
		   hb_glyph_position_t &pos) ध्रुव
  {
    unsigned पूर्णाङ्क index = (एतत्+coverage).get_coverage  (gid);
    जइ (likely (index == NOT_COVERED)) प्रत्यागम मिच्छ;

    मानम्_Format_2.position_single (font, table_blob, direction, एतत्,
                                 values, pos);
    प्रत्यागम सच्च;
  }

  template<typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  शून्य serialize (hb_serialize_context_t *c,
                  ध्रुव मानम्_Base *src,
                  मानम्_Format src_format,
                  Iterator it,
                  मानम्_Format newFormat,
                  ध्रुव hb_hashmap_t<unsigned, hb_pair_t<unsigned, पूर्णाङ्क>> *layout_variation_idx_delta_map)
  {
    जइ (unlikely (!c->extend_min (एतत्))) प्रत्यागम;
    जइ (unlikely (!c->check_assign (मानम्_Format_2,
                                    newFormat,
                                    HB_SERIALIZE_ERROR_INT_OVERFLOW))) प्रत्यागम;

    प्रति (ध्रुव hb_array_t<ध्रुव मानम्>& _ : + it | hb_map (hb_second))
    {
      src_format.copy_मानानि (c, newFormat, src,  &_, layout_variation_idx_delta_map);
      // Only serialize the first entry in the iterator, the rest are assumed to
      // be the same.
      भङ्ग;
    }

    auto glyphs =
    + it
    | hb_map_retains_sorting (hb_first)
    ;

    coverage.serialize_serialize (c, glyphs);
  }

  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    ध्रुव hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ध्रुव hb_map_t &glyph_map = *c->plan->glyph_map;

    hb_set_t intersection;
    (एतत्+coverage).intersect_set (glyphset, intersection);

    unsigned new_format = मानम्_Format_2;

    जइ (c->plan->normalized_coords)
    {
      new_format = मानम्_Format_2.get_effective_format (values.arrayZ, मिच्छ, मिच्छ, एतत्, &c->plan->layout_variation_idx_delta_map);
    }
    /* do not strip hints for VF */
    अण्णहा जइ (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
    {
      hb_blob_t* blob = hb_face_reference_table (c->plan->source, HB_TAG ('f','v','a','r'));
      तार्किक has_fvar = (blob != hb_blob_get_empty ());
      hb_blob_destroy (blob);

      तार्किक strip = !has_fvar;
      /* special case: strip hints when a VF has no GDEF varstore after
       * subsetting*/
      जइ (has_fvar && !c->plan->has_gdef_varstore)
        strip = सच्च;
      new_format = मानम्_Format_2.get_effective_format (values.arrayZ,
                                                     strip, /* strip hints */
                                                     सच्च, /* strip empty */
                                                     एतत्, nullptr);
    }

    auto it =
    + hb_iter (intersection)
    | hb_map_retains_sorting (glyph_map)
    | hb_zip (hb_repeat (values.as_array (मानम्_Format_2.get_len ())))
    ;

    तार्किक ret = तार्किक (it);
    SinglePos_serialize (c->serializer, एतत्, मानम्_Format_2, it, &c->plan->layout_variation_idx_delta_map, new_format);
    return_trace (ret);
  }
};

}
}
}

#endif /* OT_LAYOUT_GPOS_SINGLEPOSFORMAT1_HH */
