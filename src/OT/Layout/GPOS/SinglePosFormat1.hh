#ifndef OT_LAYOUT_GPOS_SINGLEPOSFORMAT1_HH
#define OT_LAYOUT_GPOS_SINGLEPOSFORMAT1_HH

#അന്തര്ഭാവ "Common.hh"
#അന്തര്ഭാവ "ValueFormat.hh"

നാമാകാശ OT {
നാമാകാശ Layout {
നാമാകാശ GPOS_impl {

template <typename Types>
ഘടന SinglePosFormat1_3 : മൂല്യം_Base
{
  സംരക്ഷിത:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of subtable */
  മൂല്യം_Format   മൂല്യം_Format_2;            /* Defines the types of data in the
                                         * ValueRecord */
  മൂല്യം_Record   values;                 /* Defines positioning
                                         * value(s)--applied to all glyphs in
                                         * the Coverage table */
  പൊതു:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + മൂല്യം_Format::static_size, values);

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്) &&
                  coverage.sanitize (c, ഇത്) &&
		  hb_barrier () &&
                  /* The coverage  table may use a range to represent a set
                   * of glyphs, which means a small number of bytes can
                   * generate a large glyph set. Manually modify the
                   * sanitizer max ops to take this into account.
                   *
                   * Note: This check *must* be right after coverage sanitize. */
                  c->check_ops ((ഇത് + coverage).get_population () >> 1) &&
                  മൂല്യം_Format_2.sanitize_മൂല്യം (c, ഇത്, values));

  }

  തർക്കം intersects (സ്ഥിരം hb_set_t *glyphs) സ്ഥിരം
  { തിരികെ (ഇത്+coverage).intersects (glyphs); }

  ശൂന്യം closure_lookups (hb_closure_lookups_context_t *c) സ്ഥിരം {}
  ശൂന്യം collect_variation_indices (hb_collect_variation_indices_context_t *c) സ്ഥിരം
  {
    എങ്കിൽ (!മൂല്യം_Format_2.has_device ()) തിരികെ;

    hb_set_t intersection;
    (ഇത്+coverage).intersect_set (*c->glyph_set, intersection);
    എങ്കിൽ (!intersection) തിരികെ;

    മൂല്യം_Format_2.collect_variation_indices (c, ഇത്, values.as_array (മൂല്യം_Format_2.get_len ()));
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

    എങ്കിൽ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "positioning glyph at %u",
			  c->buffer->idx);
    }

    മൂല്യം_Format_2.apply_മൂല്യം (c, ഇത്, values, buffer->cur_pos());

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

    മൂല്യം_Format_2.position_single (font, table_blob, direction, ഇത്,
                                 values, pos);
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
    എങ്കിൽ (unlikely (!c->extend_min (ഇത്))) തിരികെ;
    എങ്കിൽ (unlikely (!c->check_assign (മൂല്യം_Format_2,
                                    newFormat,
                                    HB_SERIALIZE_ERROR_INT_OVERFLOW))) തിരികെ;

    വേണ്ടി (സ്ഥിരം hb_array_t<സ്ഥിരം മൂല്യം>& _ : + it | hb_map (hb_second))
    {
      src_format.copy_മൂല്യങ്ങൾ (c, newFormat, src,  &_, layout_variation_idx_delta_map);
      // Only serialize the first entry in the iterator, the rest are assumed to
      // be the same.
      നിർത്തുക;
    }

    auto glyphs =
    + it
    | hb_map_retains_sorting (hb_first)
    ;

    coverage.serialize_serialize (c, glyphs);
  }

  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    സ്ഥിരം hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    സ്ഥിരം hb_map_t &glyph_map = *c->plan->glyph_map;

    hb_set_t intersection;
    (ഇത്+coverage).intersect_set (glyphset, intersection);

    unsigned new_format = മൂല്യം_Format_2;

    എങ്കിൽ (c->plan->normalized_coords)
    {
      new_format = മൂല്യം_Format_2.get_effective_format (values.arrayZ, അസത്യം, അസത്യം, ഇത്, &c->plan->layout_variation_idx_delta_map);
    }
    /* do not strip hints for VF */
    അല്ലെങ്കിൽ എങ്കിൽ (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
    {
      hb_blob_t* blob = hb_face_reference_table (c->plan->source, HB_TAG ('f','v','a','r'));
      തർക്കം has_fvar = (blob != hb_blob_get_empty ());
      hb_blob_destroy (blob);

      തർക്കം strip = !has_fvar;
      /* special case: strip hints when a VF has no GDEF varstore after
       * subsetting*/
      എങ്കിൽ (has_fvar && !c->plan->has_gdef_varstore)
        strip = സത്യം;
      new_format = മൂല്യം_Format_2.get_effective_format (values.arrayZ,
                                                     strip, /* strip hints */
                                                     സത്യം, /* strip empty */
                                                     ഇത്, nullptr);
    }

    auto it =
    + hb_iter (intersection)
    | hb_map_retains_sorting (glyph_map)
    | hb_zip (hb_repeat (values.as_array (മൂല്യം_Format_2.get_len ())))
    ;

    തർക്കം ret = തർക്കം (it);
    SinglePos_serialize (c->serializer, ഇത്, മൂല്യം_Format_2, it, &c->plan->layout_variation_idx_delta_map, new_format);
    return_trace (ret);
  }
};

}
}
}

#endif /* OT_LAYOUT_GPOS_SINGLEPOSFORMAT1_HH */
