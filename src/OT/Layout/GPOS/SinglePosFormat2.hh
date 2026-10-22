#ifndef OT_LAYOUT_GPOS_SINGLEPOSFORMAT2_HH
#define OT_LAYOUT_GPOS_SINGLEPOSFORMAT2_HH

#અન્તર્ભાવ "Common.hh"

નામાકાશ OT {
નામાકાશ Layout {
નામાકાશ GPOS_impl {

/* Keep the iterator type independent of the source table width so
 * the output serializers can share their instantiations. */
સંરચના single_pos_કિંમત_mapper_t
{
  અચલ hb_map_t &glyph_map;
  અચલ hb_array_t<અચલ કિંમત> &values;
  અચલ unsigned &sub_length;

  hb_pair_t<hb_codepoint_t, hb_array_t<અચલ કિંમત>>
  operator () (અચલ hb_pair_t<hb_codepoint_t, unsigned> &_) અચલ
  {
    પાછા hb_pair (glyph_map[_.first],
                    values.sub_array (_.second * sub_length, sub_length));
  }
};

template <typename Types>
સંરચના SinglePosFormat2_4 : કિંમત_Base
{
  સુરક્ષિત:
  HBUINT16      format;                 /* Format identifier--format = 2 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of subtable */
  કિંમત_Format   કિંમત_Format_2;            /* Defines the types of data in the
                                         * ValueRecord */
  typename Types::HBUINT
                કિંમત_ગણતરી;             /* Number of ValueRecords */
  કિંમત_Record   values;                 /* Array of ValueRecords--positioning
                                         * values applied to glyphs */
  જાહેર:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + કિંમત_Format::static_size + Types::HBUINT::static_size, values);

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) &&
                  coverage.sanitize (c, આ) &&
                  કિંમત_Format_2.sanitize_કિંમતો (c, આ, values, કિંમત_ગણતરી));
  }

  તાર્કિક intersects (અચલ hb_set_t *glyphs) અચલ
  { પાછા (આ+coverage).intersects (glyphs); }

  શૂન્ય closure_lookups (hb_closure_lookups_context_t *c) અચલ {}
  શૂન્ય collect_variation_indices (hb_collect_variation_indices_context_t *c) અચલ
  {
    જો (!કિંમત_Format_2.has_device ()) પાછા;

    auto it =
    + hb_zip (આ+coverage, hb_range ((unsigned) કિંમત_ગણતરી))
    | hb_filter (c->glyph_set, hb_first)
    ;

    જો (!it) પાછા;

    unsigned sub_length = કિંમત_Format_2.get_len ();
    અચલ hb_array_t<અચલ કિંમત> કિંમતો_array = values.as_array (કિંમત_ગણતરી * sub_length);

    માટે (unsigned i : + it
                      | hb_map (hb_second))
      કિંમત_Format_2.collect_variation_indices (c, આ, કિંમતો_array.sub_array (i * sub_length, sub_length));

  }

  શૂન્ય collect_glyphs (hb_collect_glyphs_context_t *c) અચલ
  { જો (unlikely (!(આ+coverage).collect_coverage (c->આગત))) પાછા; }

  અચલ Coverage &get_coverage () અચલ { પાછા આ+coverage; }

  કિંમત_Format get_કિંમત_format () અચલ { પાછા કિંમત_Format_2; }

  તાર્કિક apply (hb_ot_apply_context_t *c) અચલ
  {
    TRACE_APPLY (આ);
    hb_buffer_t *buffer = c->buffer;
    unsigned પૂર્ણાંક index = (આ+coverage).get_coverage  (buffer->cur().codepoint);
    જો (index == NOT_COVERED) return_trace (ખોટું);

    જો (unlikely (index >= કિંમત_ગણતરી)) return_trace (ખોટું);

    જો (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "positioning glyph at %u",
			  c->buffer->idx);
    }

    કિંમત_Format_2.apply_કિંમત (c, આ,
                             &values[index * કિંમત_Format_2.get_len ()],
                             buffer->cur_pos());

    જો (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "positioned glyph at %u",
			  c->buffer->idx);
    }

    buffer->idx++;
    return_trace (સાચું);
  }

  તાર્કિક
  position_single (hb_font_t           *font,
		   hb_blob_t           *table_blob,
		   hb_direction_t       direction,
		   hb_codepoint_t       gid,
		   hb_glyph_position_t &pos) અચલ
  {
    unsigned પૂર્ણાંક index = (આ+coverage).get_coverage  (gid);
    જો (likely (index == NOT_COVERED)) પાછા ખોટું;
    જો (unlikely (index >= કિંમત_ગણતરી)) પાછા ખોટું;

    કિંમત_Format_2.position_single (font, table_blob, direction, આ,
                                 &values[index * કિંમત_Format_2.get_len ()], pos);
    પાછા સાચું;
  }


  template<typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  શૂન્ય serialize (hb_serialize_context_t *c,
                  અચલ કિંમત_Base *src,
                  કિંમત_Format src_format,
                  Iterator it,
                  કિંમત_Format newFormat,
                  અચલ hb_hashmap_t<unsigned, hb_pair_t<unsigned, પૂર્ણાંક>> *layout_variation_idx_delta_map)
  {
    auto out = c->extend_min (આ);
    જો (unlikely (!out)) પાછા;
    જો (unlikely (!c->check_assign (કિંમત_Format_2, newFormat, HB_SERIALIZE_ERROR_INT_OVERFLOW))) પાછા;
    જો (unlikely (!c->check_assign (કિંમત_ગણતરી, it.len (), HB_SERIALIZE_ERROR_ARRAY_OVERFLOW))) પાછા;

    + it
    | hb_map (hb_second)
    | hb_apply ([&] (hb_array_t<અચલ કિંમત> _)
    { src_format.copy_કિંમતો (c, newFormat, src, &_, layout_variation_idx_delta_map); })
    ;

    auto glyphs =
    + it
    | hb_map_retains_sorting (hb_first)
    ;

    coverage.serialize_serialize (c, glyphs);
  }

  template<typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  unsigned compute_effective_format (અચલ hb_face_t *face,
                                     Iterator it,
                                     તાર્કિક is_instancing, તાર્કિક strip_hints,
                                     તાર્કિક has_gdef_varstore,
                                     અચલ hb_hashmap_t<unsigned, hb_pair_t<unsigned, પૂર્ણાંક>> *varidx_delta_map) અચલ
  {
    hb_blob_t* blob = hb_face_reference_table (face, HB_TAG ('f','v','a','r'));
    તાર્કિક has_fvar = (blob != hb_blob_get_empty ());
    hb_blob_destroy (blob);

    unsigned new_format = 0;
    જો (is_instancing)
    {
      new_format = new_format | કિંમત_Format_2.get_effective_format (+ it | hb_map (hb_second), ખોટું, ખોટું, આ, varidx_delta_map);
    }
    /* do not strip hints for VF */
    નહીંતર જો (strip_hints)
    {
      તાર્કિક strip = !has_fvar;
      જો (has_fvar && !has_gdef_varstore)
        strip = સાચું;
      new_format = new_format | કિંમત_Format_2.get_effective_format (+ it | hb_map (hb_second), strip, સાચું, આ, nullptr);
    }
    નહીંતર
      new_format = કિંમત_Format_2;

    પાછા new_format;
  }

  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);
    અચલ hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    અચલ hb_map_t &glyph_map = *c->plan->glyph_map;

    unsigned sub_length = કિંમત_Format_2.get_len ();
    auto કિંમતો_array = values.as_array ((unsigned) કિંમત_ગણતરી * sub_length);

    auto it =
    + hb_zip (આ+coverage, hb_range ((unsigned) કિંમત_ગણતરી))
    | hb_filter (glyphset, hb_first)
    | hb_map_retains_sorting (single_pos_કિંમત_mapper_t {glyph_map,
                                                       કિંમતો_array,
                                                       sub_length})
    ;

    unsigned new_format = compute_effective_format (c->plan->source, it,
                                                    તાર્કિક (c->plan->normalized_coords),
                                                    તાર્કિક (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING),
                                                    c->plan->has_gdef_varstore,
                                                    &c->plan->layout_variation_idx_delta_map);
    તાર્કિક ret = તાર્કિક (it);
    SinglePos_serialize (c->serializer, આ, કિંમત_Format_2, it, &c->plan->layout_variation_idx_delta_map, new_format);
    return_trace (ret);
  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_SINGLEPOSFORMAT2_HH */
