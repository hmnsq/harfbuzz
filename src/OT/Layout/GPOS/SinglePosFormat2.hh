#ifndef OT_LAYOUT_GPOS_SINGLEPOSFORMAT2_HH
#define OT_LAYOUT_GPOS_SINGLEPOSFORMAT2_HH

#അന്തര്ഭാവ "Common.hh"

നാമാകാശ OT {
നാമാകാശ Layout {
നാമാകാശ GPOS_impl {

/* Keep the iterator type independent of the source table width so
 * the output serializers can share their instantiations. */
ഘടന single_pos_മൂല്യം_mapper_t
{
  സ്ഥിരം hb_map_t &glyph_map;
  സ്ഥിരം hb_array_t<സ്ഥിരം മൂല്യം> &values;
  സ്ഥിരം unsigned &sub_length;

  hb_pair_t<hb_codepoint_t, hb_array_t<സ്ഥിരം മൂല്യം>>
  operator () (സ്ഥിരം hb_pair_t<hb_codepoint_t, unsigned> &_) സ്ഥിരം
  {
    തിരികെ hb_pair (glyph_map[_.first],
                    values.sub_array (_.second * sub_length, sub_length));
  }
};

template <typename Types>
ഘടന SinglePosFormat2_4 : മൂല്യം_Base
{
  സംരക്ഷിത:
  HBUINT16      format;                 /* Format identifier--format = 2 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of subtable */
  മൂല്യം_Format   മൂല്യം_Format_2;            /* Defines the types of data in the
                                         * ValueRecord */
  typename Types::HBUINT
                മൂല്യം_എണ്ണം;             /* Number of ValueRecords */
  മൂല്യം_Record   values;                 /* Array of ValueRecords--positioning
                                         * values applied to glyphs */
  പൊതു:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + മൂല്യം_Format::static_size + Types::HBUINT::static_size, values);

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്) &&
                  coverage.sanitize (c, ഇത്) &&
                  മൂല്യം_Format_2.sanitize_മൂല്യങ്ങൾ (c, ഇത്, values, മൂല്യം_എണ്ണം));
  }

  തർക്കം intersects (സ്ഥിരം hb_set_t *glyphs) സ്ഥിരം
  { തിരികെ (ഇത്+coverage).intersects (glyphs); }

  ശൂന്യം closure_lookups (hb_closure_lookups_context_t *c) സ്ഥിരം {}
  ശൂന്യം collect_variation_indices (hb_collect_variation_indices_context_t *c) സ്ഥിരം
  {
    എങ്കിൽ (!മൂല്യം_Format_2.has_device ()) തിരികെ;

    auto it =
    + hb_zip (ഇത്+coverage, hb_range ((unsigned) മൂല്യം_എണ്ണം))
    | hb_filter (c->glyph_set, hb_first)
    ;

    എങ്കിൽ (!it) തിരികെ;

    unsigned sub_length = മൂല്യം_Format_2.get_len ();
    സ്ഥിരം hb_array_t<സ്ഥിരം മൂല്യം> മൂല്യങ്ങൾ_array = values.as_array (മൂല്യം_എണ്ണം * sub_length);

    വേണ്ടി (unsigned i : + it
                      | hb_map (hb_second))
      മൂല്യം_Format_2.collect_variation_indices (c, ഇത്, മൂല്യങ്ങൾ_array.sub_array (i * sub_length, sub_length));

  }

  ശൂന്യം collect_glyphs (hb_collect_glyphs_context_t *c) സ്ഥിരം
  { എങ്കിൽ (unlikely (!(ഇത്+coverage).collect_coverage (c->നിവേശം))) തിരികെ; }

  സ്ഥിരം Coverage &get_coverage () സ്ഥിരം { തിരികെ ഇത്+coverage; }

  മൂല്യം_Format get_മൂല്യം_format () സ്ഥിരം { തിരികെ മൂല്യം_Format_2; }

  തർക്കം apply (hb_ot_apply_context_t *c) സ്ഥിരം
  {
    TRACE_APPLY (ഇത്);
    hb_buffer_t *buffer = c->buffer;
    unsigned പൂർണ്ണസംഖ്യ index = (ഇത്+coverage).get_coverage  (buffer->cur().codepoint);
    എങ്കിൽ (index == NOT_COVERED) return_trace (അസത്യം);

    എങ്കിൽ (unlikely (index >= മൂല്യം_എണ്ണം)) return_trace (അസത്യം);

    എങ്കിൽ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "positioning glyph at %u",
			  c->buffer->idx);
    }

    മൂല്യം_Format_2.apply_മൂല്യം (c, ഇത്,
                             &values[index * മൂല്യം_Format_2.get_len ()],
                             buffer->cur_pos());

    എങ്കിൽ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "positioned glyph at %u",
			  c->buffer->idx);
    }

    buffer->idx++;
    return_trace (സത്യം);
  }

  തർക്കം
  position_single (hb_font_t           *font,
		   hb_blob_t           *table_blob,
		   hb_direction_t       direction,
		   hb_codepoint_t       gid,
		   hb_glyph_position_t &pos) സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ index = (ഇത്+coverage).get_coverage  (gid);
    എങ്കിൽ (likely (index == NOT_COVERED)) തിരികെ അസത്യം;
    എങ്കിൽ (unlikely (index >= മൂല്യം_എണ്ണം)) തിരികെ അസത്യം;

    മൂല്യം_Format_2.position_single (font, table_blob, direction, ഇത്,
                                 &values[index * മൂല്യം_Format_2.get_len ()], pos);
    തിരികെ സത്യം;
  }


  template<typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  ശൂന്യം serialize (hb_serialize_context_t *c,
                  സ്ഥിരം മൂല്യം_Base *src,
                  മൂല്യം_Format src_format,
                  Iterator it,
                  മൂല്യം_Format newFormat,
                  സ്ഥിരം hb_hashmap_t<unsigned, hb_pair_t<unsigned, പൂർണ്ണസംഖ്യ>> *layout_variation_idx_delta_map)
  {
    auto out = c->extend_min (ഇത്);
    എങ്കിൽ (unlikely (!out)) തിരികെ;
    എങ്കിൽ (unlikely (!c->check_assign (മൂല്യം_Format_2, newFormat, HB_SERIALIZE_ERROR_INT_OVERFLOW))) തിരികെ;
    എങ്കിൽ (unlikely (!c->check_assign (മൂല്യം_എണ്ണം, it.len (), HB_SERIALIZE_ERROR_ARRAY_OVERFLOW))) തിരികെ;

    + it
    | hb_map (hb_second)
    | hb_apply ([&] (hb_array_t<സ്ഥിരം മൂല്യം> _)
    { src_format.copy_മൂല്യങ്ങൾ (c, newFormat, src, &_, layout_variation_idx_delta_map); })
    ;

    auto glyphs =
    + it
    | hb_map_retains_sorting (hb_first)
    ;

    coverage.serialize_serialize (c, glyphs);
  }

  template<typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  unsigned compute_effective_format (സ്ഥിരം hb_face_t *face,
                                     Iterator it,
                                     തർക്കം is_instancing, തർക്കം strip_hints,
                                     തർക്കം has_gdef_varstore,
                                     സ്ഥിരം hb_hashmap_t<unsigned, hb_pair_t<unsigned, പൂർണ്ണസംഖ്യ>> *varidx_delta_map) സ്ഥിരം
  {
    hb_blob_t* blob = hb_face_reference_table (face, HB_TAG ('f','v','a','r'));
    തർക്കം has_fvar = (blob != hb_blob_get_empty ());
    hb_blob_destroy (blob);

    unsigned new_format = 0;
    എങ്കിൽ (is_instancing)
    {
      new_format = new_format | മൂല്യം_Format_2.get_effective_format (+ it | hb_map (hb_second), അസത്യം, അസത്യം, ഇത്, varidx_delta_map);
    }
    /* do not strip hints for VF */
    അല്ലെങ്കിൽ എങ്കിൽ (strip_hints)
    {
      തർക്കം strip = !has_fvar;
      എങ്കിൽ (has_fvar && !has_gdef_varstore)
        strip = സത്യം;
      new_format = new_format | മൂല്യം_Format_2.get_effective_format (+ it | hb_map (hb_second), strip, സത്യം, ഇത്, nullptr);
    }
    അല്ലെങ്കിൽ
      new_format = മൂല്യം_Format_2;

    തിരികെ new_format;
  }

  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    സ്ഥിരം hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    സ്ഥിരം hb_map_t &glyph_map = *c->plan->glyph_map;

    unsigned sub_length = മൂല്യം_Format_2.get_len ();
    auto മൂല്യങ്ങൾ_array = values.as_array ((unsigned) മൂല്യം_എണ്ണം * sub_length);

    auto it =
    + hb_zip (ഇത്+coverage, hb_range ((unsigned) മൂല്യം_എണ്ണം))
    | hb_filter (glyphset, hb_first)
    | hb_map_retains_sorting (single_pos_മൂല്യം_mapper_t {glyph_map,
                                                       മൂല്യങ്ങൾ_array,
                                                       sub_length})
    ;

    unsigned new_format = compute_effective_format (c->plan->source, it,
                                                    തർക്കം (c->plan->normalized_coords),
                                                    തർക്കം (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING),
                                                    c->plan->has_gdef_varstore,
                                                    &c->plan->layout_variation_idx_delta_map);
    തർക്കം ret = തർക്കം (it);
    SinglePos_serialize (c->serializer, ഇത്, മൂല്യം_Format_2, it, &c->plan->layout_variation_idx_delta_map, new_format);
    return_trace (ret);
  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_SINGLEPOSFORMAT2_HH */
