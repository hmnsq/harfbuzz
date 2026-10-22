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
  स्थिरांक hb_map_t &glyph_map;
  स्थिरांक hb_array_t<स्थिरांक मूल्य> &values;
  स्थिरांक अचिन्हित &sub_length;

  hb_pair_t<hb_codepoint_t, hb_array_t<स्थिरांक मूल्य>>
  चालक () (स्थिरांक hb_pair_t<hb_codepoint_t, अचिन्हित> &_) स्थिरांक
  {
    परत hb_pair (glyph_map[_.first],
                    values.sub_array (_.second * sub_length, sub_length));
  }
};

ढाँचा <typename Types>
संरचना SinglePosFormat2_4 : मूल्य_Base
{
  संरक्षित:
  HBUINT16      format;                 /* Format identifier--format = 2 */
  typename Types::ढाँचा LOffsetTo<Coverage>
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

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) &&
                  coverage.sanitize (c, हे) &&
                  मूल्य_Format_2.sanitize_मूल्ये (c, हे, values, मूल्य_मोजणी));
  }

  तार्किक intersects (स्थिरांक hb_set_t *glyphs) स्थिरांक
  { परत (हे+coverage).intersects (glyphs); }

  शून्य closure_lookups (hb_closure_lookups_context_t *c) स्थिरांक {}
  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) स्थिरांक
  {
    जर (!मूल्य_Format_2.has_device ()) परत;

    स्वतः it =
    + hb_zip (हे+coverage, hb_range ((अचिन्हित) मूल्य_मोजणी))
    | hb_filter (c->glyph_set, hb_first)
    ;

    जर (!it) परत;

    अचिन्हित sub_length = मूल्य_Format_2.get_len ();
    स्थिरांक hb_array_t<स्थिरांक मूल्य> मूल्ये_array = values.as_array (मूल्य_मोजणी * sub_length);

    साठी (अचिन्हित i : + it
                      | hb_map (hb_second))
      मूल्य_Format_2.collect_variation_indices (c, हे, मूल्ये_array.sub_array (i * sub_length, sub_length));

  }

  शून्य collect_glyphs (hb_collect_glyphs_context_t *c) स्थिरांक
  { जर (unlikely (!(हे+coverage).collect_coverage (c->आदान))) परत; }

  स्थिरांक Coverage &get_coverage () स्थिरांक { परत हे+coverage; }

  मूल्य_Format get_मूल्य_format () स्थिरांक { परत मूल्य_Format_2; }

  तार्किक apply (hb_ot_apply_context_t *c) स्थिरांक
  {
    TRACE_APPLY (हे);
    hb_buffer_t *buffer = c->buffer;
    अचिन्हित पूर्णांक index = (हे+coverage).get_coverage  (buffer->cur().codepoint);
    जर (index == NOT_COVERED) return_trace (असत्य);

    जर (unlikely (index >= मूल्य_मोजणी)) return_trace (असत्य);

    जर (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "positioning glyph at %u",
			  c->buffer->idx);
    }

    मूल्य_Format_2.apply_मूल्य (c, हे,
                             &values[index * मूल्य_Format_2.get_len ()],
                             buffer->cur_pos());

    जर (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
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
		   hb_glyph_position_t &pos) स्थिरांक
  {
    अचिन्हित पूर्णांक index = (हे+coverage).get_coverage  (gid);
    जर (likely (index == NOT_COVERED)) परत असत्य;
    जर (unlikely (index >= मूल्य_मोजणी)) परत असत्य;

    मूल्य_Format_2.position_single (font, table_blob, direction, हे,
                                 &values[index * मूल्य_Format_2.get_len ()], pos);
    परत सत्य;
  }


  ढाँचा<typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  शून्य serialize (hb_serialize_context_t *c,
                  स्थिरांक मूल्य_Base *src,
                  मूल्य_Format src_format,
                  Iterator it,
                  मूल्य_Format newFormat,
                  स्थिरांक hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णांक>> *layout_variation_idx_delta_map)
  {
    स्वतः out = c->extend_min (हे);
    जर (unlikely (!out)) परत;
    जर (unlikely (!c->check_assign (मूल्य_Format_2, newFormat, HB_SERIALIZE_ERROR_INT_OVERFLOW))) परत;
    जर (unlikely (!c->check_assign (मूल्य_मोजणी, it.len (), HB_SERIALIZE_ERROR_ARRAY_OVERFLOW))) परत;

    + it
    | hb_map (hb_second)
    | hb_apply ([&] (hb_array_t<स्थिरांक मूल्य> _)
    { src_format.copy_मूल्ये (c, newFormat, src, &_, layout_variation_idx_delta_map); })
    ;

    स्वतः glyphs =
    + it
    | hb_map_retains_sorting (hb_first)
    ;

    coverage.serialize_serialize (c, glyphs);
  }

  ढाँचा<typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  अचिन्हित compute_effective_format (स्थिरांक hb_face_t *face,
                                     Iterator it,
                                     तार्किक is_instancing, तार्किक strip_hints,
                                     तार्किक has_gdef_varstore,
                                     स्थिरांक hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णांक>> *varidx_delta_map) स्थिरांक
  {
    hb_blob_t* blob = hb_face_reference_table (face, HB_TAG ('f','v','a','r'));
    तार्किक has_fvar = (blob != hb_blob_get_empty ());
    hb_blob_destroy (blob);

    अचिन्हित new_format = 0;
    जर (is_instancing)
    {
      new_format = new_format | मूल्य_Format_2.get_effective_format (+ it | hb_map (hb_second), असत्य, असत्य, हे, varidx_delta_map);
    }
    /* do not strip hints for VF */
    अन्यथा जर (strip_hints)
    {
      तार्किक strip = !has_fvar;
      जर (has_fvar && !has_gdef_varstore)
        strip = सत्य;
      new_format = new_format | मूल्य_Format_2.get_effective_format (+ it | hb_map (hb_second), strip, सत्य, हे, nullptr);
    }
    अन्यथा
      new_format = मूल्य_Format_2;

    परत new_format;
  }

  तार्किक subset (hb_subset_context_t *c) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्थिरांक hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    स्थिरांक hb_map_t &glyph_map = *c->plan->glyph_map;

    अचिन्हित sub_length = मूल्य_Format_2.get_len ();
    स्वतः मूल्ये_array = values.as_array ((अचिन्हित) मूल्य_मोजणी * sub_length);

    स्वतः it =
    + hb_zip (हे+coverage, hb_range ((अचिन्हित) मूल्य_मोजणी))
    | hb_filter (glyphset, hb_first)
    | hb_map_retains_sorting (single_pos_मूल्य_mapper_t {glyph_map,
                                                       मूल्ये_array,
                                                       sub_length})
    ;

    अचिन्हित new_format = compute_effective_format (c->plan->source, it,
                                                    तार्किक (c->plan->normalized_coords),
                                                    तार्किक (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING),
                                                    c->plan->has_gdef_varstore,
                                                    &c->plan->layout_variation_idx_delta_map);
    तार्किक ret = तार्किक (it);
    SinglePos_serialize (c->serializer, हे, मूल्य_Format_2, it, &c->plan->layout_variation_idx_delta_map, new_format);
    return_trace (ret);
  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_SINGLEPOSFORMAT2_HH */
