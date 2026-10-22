#ifndef OT_LAYOUT_GPOS_SINGLEPOSFORMAT2_HH
#define OT_LAYOUT_GPOS_SINGLEPOSFORMAT2_HH

#अन्तर्भाव "Common.hh"

नामाकाश OT {
नामाकाश Layout {
नामाकाश GPOS_impl {

/* Keep the iterator type independent of the source table width so
 * the output serializers can share their instantiations. */
संरचना single_pos_मानम्_mapper_t
{
  ध्रुव hb_map_t &glyph_map;
  ध्रुव hb_array_t<ध्रुव मानम्> &values;
  ध्रुव unsigned &sub_length;

  hb_pair_t<hb_codepoint_t, hb_array_t<ध्रुव मानम्>>
  operator () (ध्रुव hb_pair_t<hb_codepoint_t, unsigned> &_) ध्रुव
  {
    प्रत्यागम hb_pair (glyph_map[_.first],
                    values.sub_array (_.second * sub_length, sub_length));
  }
};

template <typename Types>
संरचना SinglePosFormat2_4 : मानम्_Base
{
  रक्षित:
  HBUINT16      format;                 /* Format identifier--format = 2 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of subtable */
  मानम्_Format   मानम्_Format_2;            /* Defines the types of data in the
                                         * ValueRecord */
  typename Types::HBUINT
                मानम्_गणनम्;             /* Number of ValueRecords */
  मानम्_Record   values;                 /* Array of ValueRecords--positioning
                                         * values applied to glyphs */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + मानम्_Format::static_size + Types::HBUINT::static_size, values);

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
                  coverage.sanitize (c, एतत्) &&
                  मानम्_Format_2.sanitize_मानानि (c, एतत्, values, मानम्_गणनम्));
  }

  तार्किक intersects (ध्रुव hb_set_t *glyphs) ध्रुव
  { प्रत्यागम (एतत्+coverage).intersects (glyphs); }

  शून्य closure_lookups (hb_closure_lookups_context_t *c) ध्रुव {}
  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) ध्रुव
  {
    जइ (!मानम्_Format_2.has_device ()) प्रत्यागम;

    auto it =
    + hb_zip (एतत्+coverage, hb_range ((unsigned) मानम्_गणनम्))
    | hb_filter (c->glyph_set, hb_first)
    ;

    जइ (!it) प्रत्यागम;

    unsigned sub_length = मानम्_Format_2.get_len ();
    ध्रुव hb_array_t<ध्रुव मानम्> मानानि_array = values.as_array (मानम्_गणनम् * sub_length);

    प्रति (unsigned i : + it
                      | hb_map (hb_second))
      मानम्_Format_2.collect_variation_indices (c, एतत्, मानानि_array.sub_array (i * sub_length, sub_length));

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

    जइ (unlikely (index >= मानम्_गणनम्)) return_trace (मिच्छ);

    जइ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "positioning glyph at %u",
			  c->buffer->idx);
    }

    मानम्_Format_2.apply_मानम् (c, एतत्,
                             &values[index * मानम्_Format_2.get_len ()],
                             buffer->cur_pos());

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
    जइ (unlikely (index >= मानम्_गणनम्)) प्रत्यागम मिच्छ;

    मानम्_Format_2.position_single (font, table_blob, direction, एतत्,
                                 &values[index * मानम्_Format_2.get_len ()], pos);
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
    auto out = c->extend_min (एतत्);
    जइ (unlikely (!out)) प्रत्यागम;
    जइ (unlikely (!c->check_assign (मानम्_Format_2, newFormat, HB_SERIALIZE_ERROR_INT_OVERFLOW))) प्रत्यागम;
    जइ (unlikely (!c->check_assign (मानम्_गणनम्, it.len (), HB_SERIALIZE_ERROR_ARRAY_OVERFLOW))) प्रत्यागम;

    + it
    | hb_map (hb_second)
    | hb_apply ([&] (hb_array_t<ध्रुव मानम्> _)
    { src_format.copy_मानानि (c, newFormat, src, &_, layout_variation_idx_delta_map); })
    ;

    auto glyphs =
    + it
    | hb_map_retains_sorting (hb_first)
    ;

    coverage.serialize_serialize (c, glyphs);
  }

  template<typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  unsigned compute_effective_format (ध्रुव hb_face_t *face,
                                     Iterator it,
                                     तार्किक is_instancing, तार्किक strip_hints,
                                     तार्किक has_gdef_varstore,
                                     ध्रुव hb_hashmap_t<unsigned, hb_pair_t<unsigned, पूर्णाङ्क>> *varidx_delta_map) ध्रुव
  {
    hb_blob_t* blob = hb_face_reference_table (face, HB_TAG ('f','v','a','r'));
    तार्किक has_fvar = (blob != hb_blob_get_empty ());
    hb_blob_destroy (blob);

    unsigned new_format = 0;
    जइ (is_instancing)
    {
      new_format = new_format | मानम्_Format_2.get_effective_format (+ it | hb_map (hb_second), मिच्छ, मिच्छ, एतत्, varidx_delta_map);
    }
    /* do not strip hints for VF */
    अण्णहा जइ (strip_hints)
    {
      तार्किक strip = !has_fvar;
      जइ (has_fvar && !has_gdef_varstore)
        strip = सच्च;
      new_format = new_format | मानम्_Format_2.get_effective_format (+ it | hb_map (hb_second), strip, सच्च, एतत्, nullptr);
    }
    अण्णहा
      new_format = मानम्_Format_2;

    प्रत्यागम new_format;
  }

  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    ध्रुव hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ध्रुव hb_map_t &glyph_map = *c->plan->glyph_map;

    unsigned sub_length = मानम्_Format_2.get_len ();
    auto मानानि_array = values.as_array ((unsigned) मानम्_गणनम् * sub_length);

    auto it =
    + hb_zip (एतत्+coverage, hb_range ((unsigned) मानम्_गणनम्))
    | hb_filter (glyphset, hb_first)
    | hb_map_retains_sorting (single_pos_मानम्_mapper_t {glyph_map,
                                                       मानानि_array,
                                                       sub_length})
    ;

    unsigned new_format = compute_effective_format (c->plan->source, it,
                                                    तार्किक (c->plan->normalized_coords),
                                                    तार्किक (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING),
                                                    c->plan->has_gdef_varstore,
                                                    &c->plan->layout_variation_idx_delta_map);
    तार्किक ret = तार्किक (it);
    SinglePos_serialize (c->serializer, एतत्, मानम्_Format_2, it, &c->plan->layout_variation_idx_delta_map, new_format);
    return_trace (ret);
  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_SINGLEPOSFORMAT2_HH */
