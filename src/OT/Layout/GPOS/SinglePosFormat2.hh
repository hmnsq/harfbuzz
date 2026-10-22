#ifndef OT_LAYOUT_GPOS_SINGLEPOSFORMAT2_HH
#define OT_LAYOUT_GPOS_SINGLEPOSFORMAT2_HH

#अन्तर्भाव "Common.hh"

नामाकाश OT {
नामाकाश Layout {
नामाकाश GPOS_impl {

/* Keep the iterator type independent of the source table width so
 * the output serializers can share their instantiations. */
संरचना single_pos_मूल्य_mapper_t
{
  ध्रुव hb_map_t &glyph_map;
  ध्रुव hb_array_t<ध्रुव मूल्य> &values;
  ध्रुव unsigned &sub_length;

  hb_pair_t<hb_codepoint_t, hb_array_t<ध्रुव मूल्य>>
  operator () (ध्रुव hb_pair_t<hb_codepoint_t, unsigned> &_) ध्रुव
  {
    प्रत्यागम hb_pair (glyph_map[_.first],
                    values.sub_array (_.second * sub_length, sub_length));
  }
};

template <typename Types>
संरचना SinglePosFormat2_4 : मूल्य_Base
{
  रक्षित:
  HBUINT16      format;                 /* Format identifier--format = 2 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of subtable */
  मूल्य_Format   मूल्य_Format_2;            /* Defines the types of data in the
                                         * ValueRecord */
  typename Types::HBUINT
                मूल्य_मोजणी;             /* Number of ValueRecords */
  मूल्य_Record   values;                 /* Array of ValueRecords--positioning
                                         * values applied to glyphs */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + मूल्य_Format::static_size + Types::HBUINT::static_size, values);

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
                  coverage.sanitize (c, एतत्) &&
                  मूल्य_Format_2.sanitize_मूल्ये (c, एतत्, values, मूल्य_मोजणी));
  }

  तार्किक intersects (ध्रुव hb_set_t *glyphs) ध्रुव
  { प्रत्यागम (एतत्+coverage).intersects (glyphs); }

  शून्य closure_lookups (hb_closure_lookups_context_t *c) ध्रुव {}
  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) ध्रुव
  {
    यदि (!मूल्य_Format_2.has_device ()) प्रत्यागम;

    auto it =
    + hb_zip (एतत्+coverage, hb_range ((unsigned) मूल्य_मोजणी))
    | hb_filter (c->glyph_set, hb_first)
    ;

    यदि (!it) प्रत्यागम;

    unsigned sub_length = मूल्य_Format_2.get_len ();
    ध्रुव hb_array_t<ध्रुव मूल्य> मूल्ये_array = values.as_array (मूल्य_मोजणी * sub_length);

    प्रति (unsigned i : + it
                      | hb_map (hb_second))
      मूल्य_Format_2.collect_variation_indices (c, एतत्, मूल्ये_array.sub_array (i * sub_length, sub_length));

  }

  शून्य collect_glyphs (hb_collect_glyphs_context_t *c) ध्रुव
  { यदि (unlikely (!(एतत्+coverage).collect_coverage (c->आदान))) प्रत्यागम; }

  ध्रुव Coverage &get_coverage () ध्रुव { प्रत्यागम एतत्+coverage; }

  मूल्य_Format get_मूल्य_format () ध्रुव { प्रत्यागम मूल्य_Format_2; }

  तार्किक apply (hb_ot_apply_context_t *c) ध्रुव
  {
    TRACE_APPLY (एतत्);
    hb_buffer_t *buffer = c->buffer;
    unsigned पूर्णाङ्क index = (एतत्+coverage).get_coverage  (buffer->cur().codepoint);
    यदि (index == NOT_COVERED) return_trace (असत्य);

    यदि (unlikely (index >= मूल्य_मोजणी)) return_trace (असत्य);

    यदि (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "positioning glyph at %u",
			  c->buffer->idx);
    }

    मूल्य_Format_2.apply_मूल्य (c, एतत्,
                             &values[index * मूल्य_Format_2.get_len ()],
                             buffer->cur_pos());

    यदि (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "positioned glyph at %u",
			  c->buffer->idx);
    }

    buffer->idx++;
    return_trace (सत्य);
  }

  तार्किक
  position_single (hb_font_t           *font,
		   hb_blob_t           *table_blob,
		   hb_direction_t       direction,
		   hb_codepoint_t       gid,
		   hb_glyph_position_t &pos) ध्रुव
  {
    unsigned पूर्णाङ्क index = (एतत्+coverage).get_coverage  (gid);
    यदि (likely (index == NOT_COVERED)) प्रत्यागम असत्य;
    यदि (unlikely (index >= मूल्य_मोजणी)) प्रत्यागम असत्य;

    मूल्य_Format_2.position_single (font, table_blob, direction, एतत्,
                                 &values[index * मूल्य_Format_2.get_len ()], pos);
    प्रत्यागम सत्य;
  }


  template<typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  शून्य serialize (hb_serialize_context_t *c,
                  ध्रुव मूल्य_Base *src,
                  मूल्य_Format src_format,
                  Iterator it,
                  मूल्य_Format newFormat,
                  ध्रुव hb_hashmap_t<unsigned, hb_pair_t<unsigned, पूर्णाङ्क>> *layout_variation_idx_delta_map)
  {
    auto out = c->extend_min (एतत्);
    यदि (unlikely (!out)) प्रत्यागम;
    यदि (unlikely (!c->check_assign (मूल्य_Format_2, newFormat, HB_SERIALIZE_ERROR_INT_OVERFLOW))) प्रत्यागम;
    यदि (unlikely (!c->check_assign (मूल्य_मोजणी, it.len (), HB_SERIALIZE_ERROR_ARRAY_OVERFLOW))) प्रत्यागम;

    + it
    | hb_map (hb_second)
    | hb_apply ([&] (hb_array_t<ध्रुव मूल्य> _)
    { src_format.copy_मूल्ये (c, newFormat, src, &_, layout_variation_idx_delta_map); })
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
    यदि (is_instancing)
    {
      new_format = new_format | मूल्य_Format_2.get_effective_format (+ it | hb_map (hb_second), असत्य, असत्य, एतत्, varidx_delta_map);
    }
    /* do not strip hints for VF */
    अन्यथा यदि (strip_hints)
    {
      तार्किक strip = !has_fvar;
      यदि (has_fvar && !has_gdef_varstore)
        strip = सत्य;
      new_format = new_format | मूल्य_Format_2.get_effective_format (+ it | hb_map (hb_second), strip, सत्य, एतत्, nullptr);
    }
    अन्यथा
      new_format = मूल्य_Format_2;

    प्रत्यागम new_format;
  }

  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    ध्रुव hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ध्रुव hb_map_t &glyph_map = *c->plan->glyph_map;

    unsigned sub_length = मूल्य_Format_2.get_len ();
    auto मूल्ये_array = values.as_array ((unsigned) मूल्य_मोजणी * sub_length);

    auto it =
    + hb_zip (एतत्+coverage, hb_range ((unsigned) मूल्य_मोजणी))
    | hb_filter (glyphset, hb_first)
    | hb_map_retains_sorting (single_pos_मूल्य_mapper_t {glyph_map,
                                                       मूल्ये_array,
                                                       sub_length})
    ;

    unsigned new_format = compute_effective_format (c->plan->source, it,
                                                    तार्किक (c->plan->normalized_coords),
                                                    तार्किक (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING),
                                                    c->plan->has_gdef_varstore,
                                                    &c->plan->layout_variation_idx_delta_map);
    तार्किक ret = तार्किक (it);
    SinglePos_serialize (c->serializer, एतत्, मूल्य_Format_2, it, &c->plan->layout_variation_idx_delta_map, new_format);
    return_trace (ret);
  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_SINGLEPOSFORMAT2_HH */
