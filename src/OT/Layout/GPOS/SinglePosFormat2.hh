#ifndef OT_LAYOUT_GPOS_SINGLEPOSFORMAT2_HH
#define OT_LAYOUT_GPOS_SINGLEPOSFORMAT2_HH

#ಅನ್ತರ್ಭಾವ "Common.hh"

ನಾಮಾಕಾಶ OT {
ನಾಮಾಕಾಶ Layout {
ನಾಮಾಕಾಶ GPOS_impl {

/* Keep the iterator type independent of the source table width so
 * the output serializers can share their instantiations. */
ಸಂರಚನಾ single_pos_ಮೌಲ್ಯ_mapper_t
{
  ಧ್ರುವ hb_map_t &glyph_map;
  ಧ್ರುವ hb_array_t<ಧ್ರುವ ಮೌಲ್ಯ> &values;
  ಧ್ರುವ unsigned &sub_length;

  hb_pair_t<hb_codepoint_t, hb_array_t<ಧ್ರುವ ಮೌಲ್ಯ>>
  operator () (ಧ್ರುವ hb_pair_t<hb_codepoint_t, unsigned> &_) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ hb_pair (glyph_map[_.first],
                    values.sub_array (_.second * sub_length, sub_length));
  }
};

template <typename Types>
ಸಂರಚನಾ SinglePosFormat2_4 : ಮೌಲ್ಯ_Base
{
  ರಕ್ಷಿತ:
  HBUINT16      format;                 /* Format identifier--format = 2 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of subtable */
  ಮೌಲ್ಯ_Format   ಮೌಲ್ಯ_Format_2;            /* Defines the types of data in the
                                         * ValueRecord */
  typename Types::HBUINT
                ಮೌಲ್ಯ_ಎಣಿಕೆ;             /* Number of ValueRecords */
  ಮೌಲ್ಯ_Record   values;                 /* Array of ValueRecords--positioning
                                         * values applied to glyphs */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + ಮೌಲ್ಯ_Format::static_size + Types::HBUINT::static_size, values);

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) &&
                  coverage.sanitize (c, ಏತತ್) &&
                  ಮೌಲ್ಯ_Format_2.sanitize_ಮೌಲ್ಯಗಳು (c, ಏತತ್, values, ಮೌಲ್ಯ_ಎಣಿಕೆ));
  }

  ತಾರ್ಕಿಕ intersects (ಧ್ರುವ hb_set_t *glyphs) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ (ಏತತ್+coverage).intersects (glyphs); }

  ಶೂನ್ಯ closure_lookups (hb_closure_lookups_context_t *c) ಧ್ರುವ {}
  ಶೂನ್ಯ collect_variation_indices (hb_collect_variation_indices_context_t *c) ಧ್ರುವ
  {
    ಯದಿ (!ಮೌಲ್ಯ_Format_2.has_device ()) ಪ್ರತ್ಯಾಗಮ;

    auto it =
    + hb_zip (ಏತತ್+coverage, hb_range ((unsigned) ಮೌಲ್ಯ_ಎಣಿಕೆ))
    | hb_filter (c->glyph_set, hb_first)
    ;

    ಯದಿ (!it) ಪ್ರತ್ಯಾಗಮ;

    unsigned sub_length = ಮೌಲ್ಯ_Format_2.get_len ();
    ಧ್ರುವ hb_array_t<ಧ್ರುವ ಮೌಲ್ಯ> ಮೌಲ್ಯಗಳು_array = values.as_array (ಮೌಲ್ಯ_ಎಣಿಕೆ * sub_length);

    ಪ್ರತಿ (unsigned i : + it
                      | hb_map (hb_second))
      ಮೌಲ್ಯ_Format_2.collect_variation_indices (c, ಏತತ್, ಮೌಲ್ಯಗಳು_array.sub_array (i * sub_length, sub_length));

  }

  ಶೂನ್ಯ collect_glyphs (hb_collect_glyphs_context_t *c) ಧ್ರುವ
  { ಯದಿ (unlikely (!(ಏತತ್+coverage).collect_coverage (c->ಒಳಹರಿವು))) ಪ್ರತ್ಯಾಗಮ; }

  ಧ್ರುವ Coverage &get_coverage () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ಏತತ್+coverage; }

  ಮೌಲ್ಯ_Format get_ಮೌಲ್ಯ_format () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ಮೌಲ್ಯ_Format_2; }

  ತಾರ್ಕಿಕ apply (hb_ot_apply_context_t *c) ಧ್ರುವ
  {
    TRACE_APPLY (ಏತತ್);
    hb_buffer_t *buffer = c->buffer;
    unsigned ಪೂರ್ಣಾಙ್ಕ index = (ಏತತ್+coverage).get_coverage  (buffer->cur().codepoint);
    ಯದಿ (index == NOT_COVERED) return_trace (ಅಸತ್ಯ);

    ಯದಿ (unlikely (index >= ಮೌಲ್ಯ_ಎಣಿಕೆ)) return_trace (ಅಸತ್ಯ);

    ಯದಿ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "positioning glyph at %u",
			  c->buffer->idx);
    }

    ಮೌಲ್ಯ_Format_2.apply_ಮೌಲ್ಯ (c, ಏತತ್,
                             &values[index * ಮೌಲ್ಯ_Format_2.get_len ()],
                             buffer->cur_pos());

    ಯದಿ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "positioned glyph at %u",
			  c->buffer->idx);
    }

    buffer->idx++;
    return_trace (ಸತ್ಯ);
  }

  ತಾರ್ಕಿಕ
  position_single (hb_font_t           *font,
		   hb_blob_t           *table_blob,
		   hb_direction_t       direction,
		   hb_codepoint_t       gid,
		   hb_glyph_position_t &pos) ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ index = (ಏತತ್+coverage).get_coverage  (gid);
    ಯದಿ (likely (index == NOT_COVERED)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಯದಿ (unlikely (index >= ಮೌಲ್ಯ_ಎಣಿಕೆ)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಮೌಲ್ಯ_Format_2.position_single (font, table_blob, direction, ಏತತ್,
                                 &values[index * ಮೌಲ್ಯ_Format_2.get_len ()], pos);
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }


  template<typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  ಶೂನ್ಯ serialize (hb_serialize_context_t *c,
                  ಧ್ರುವ ಮೌಲ್ಯ_Base *src,
                  ಮೌಲ್ಯ_Format src_format,
                  Iterator it,
                  ಮೌಲ್ಯ_Format newFormat,
                  ಧ್ರುವ hb_hashmap_t<unsigned, hb_pair_t<unsigned, ಪೂರ್ಣಾಙ್ಕ>> *layout_variation_idx_delta_map)
  {
    auto out = c->extend_min (ಏತತ್);
    ಯದಿ (unlikely (!out)) ಪ್ರತ್ಯಾಗಮ;
    ಯದಿ (unlikely (!c->check_assign (ಮೌಲ್ಯ_Format_2, newFormat, HB_SERIALIZE_ERROR_INT_OVERFLOW))) ಪ್ರತ್ಯಾಗಮ;
    ಯದಿ (unlikely (!c->check_assign (ಮೌಲ್ಯ_ಎಣಿಕೆ, it.len (), HB_SERIALIZE_ERROR_ARRAY_OVERFLOW))) ಪ್ರತ್ಯಾಗಮ;

    + it
    | hb_map (hb_second)
    | hb_apply ([&] (hb_array_t<ಧ್ರುವ ಮೌಲ್ಯ> _)
    { src_format.copy_ಮೌಲ್ಯಗಳು (c, newFormat, src, &_, layout_variation_idx_delta_map); })
    ;

    auto glyphs =
    + it
    | hb_map_retains_sorting (hb_first)
    ;

    coverage.serialize_serialize (c, glyphs);
  }

  template<typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  unsigned compute_effective_format (ಧ್ರುವ hb_face_t *face,
                                     Iterator it,
                                     ತಾರ್ಕಿಕ is_instancing, ತಾರ್ಕಿಕ strip_hints,
                                     ತಾರ್ಕಿಕ has_gdef_varstore,
                                     ಧ್ರುವ hb_hashmap_t<unsigned, hb_pair_t<unsigned, ಪೂರ್ಣಾಙ್ಕ>> *varidx_delta_map) ಧ್ರುವ
  {
    hb_blob_t* blob = hb_face_reference_table (face, HB_TAG ('f','v','a','r'));
    ತಾರ್ಕಿಕ has_fvar = (blob != hb_blob_get_empty ());
    hb_blob_destroy (blob);

    unsigned new_format = 0;
    ಯದಿ (is_instancing)
    {
      new_format = new_format | ಮೌಲ್ಯ_Format_2.get_effective_format (+ it | hb_map (hb_second), ಅಸತ್ಯ, ಅಸತ್ಯ, ಏತತ್, varidx_delta_map);
    }
    /* do not strip hints for VF */
    ಅನ್ಯಥಾ ಯದಿ (strip_hints)
    {
      ತಾರ್ಕಿಕ strip = !has_fvar;
      ಯದಿ (has_fvar && !has_gdef_varstore)
        strip = ಸತ್ಯ;
      new_format = new_format | ಮೌಲ್ಯ_Format_2.get_effective_format (+ it | hb_map (hb_second), strip, ಸತ್ಯ, ಏತತ್, nullptr);
    }
    ಅನ್ಯಥಾ
      new_format = ಮೌಲ್ಯ_Format_2;

    ಪ್ರತ್ಯಾಗಮ new_format;
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    ಧ್ರುವ hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ಧ್ರುವ hb_map_t &glyph_map = *c->plan->glyph_map;

    unsigned sub_length = ಮೌಲ್ಯ_Format_2.get_len ();
    auto ಮೌಲ್ಯಗಳು_array = values.as_array ((unsigned) ಮೌಲ್ಯ_ಎಣಿಕೆ * sub_length);

    auto it =
    + hb_zip (ಏತತ್+coverage, hb_range ((unsigned) ಮೌಲ್ಯ_ಎಣಿಕೆ))
    | hb_filter (glyphset, hb_first)
    | hb_map_retains_sorting (single_pos_ಮೌಲ್ಯ_mapper_t {glyph_map,
                                                       ಮೌಲ್ಯಗಳು_array,
                                                       sub_length})
    ;

    unsigned new_format = compute_effective_format (c->plan->source, it,
                                                    ತಾರ್ಕಿಕ (c->plan->normalized_coords),
                                                    ತಾರ್ಕಿಕ (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING),
                                                    c->plan->has_gdef_varstore,
                                                    &c->plan->layout_variation_idx_delta_map);
    ತಾರ್ಕಿಕ ret = ತಾರ್ಕಿಕ (it);
    SinglePos_serialize (c->serializer, ಏತತ್, ಮೌಲ್ಯ_Format_2, it, &c->plan->layout_variation_idx_delta_map, new_format);
    return_trace (ret);
  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_SINGLEPOSFORMAT2_HH */
