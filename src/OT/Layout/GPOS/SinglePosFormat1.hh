#ifndef OT_LAYOUT_GPOS_SINGLEPOSFORMAT1_HH
#define OT_LAYOUT_GPOS_SINGLEPOSFORMAT1_HH

#અન્તર્ભાવ "Common.hh"
#અન્તર્ભાવ "ValueFormat.hh"

નામાકાશ OT {
નામાકાશ Layout {
નામાકાશ GPOS_impl {

template <typename Types>
સંરચના SinglePosFormat1_3 : કિંમત_Base
{
  સુરક્ષિત:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of subtable */
  કિંમત_Format   કિંમત_Format_2;            /* Defines the types of data in the
                                         * ValueRecord */
  કિંમત_Record   values;                 /* Defines positioning
                                         * value(s)--applied to all glyphs in
                                         * the Coverage table */
  જાહેર:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + કિંમત_Format::static_size, values);

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) &&
                  coverage.sanitize (c, આ) &&
		  hb_barrier () &&
                  /* The coverage  table may use a range to represent a set
                   * of glyphs, which means a small number of bytes can
                   * generate a large glyph set. Manually modify the
                   * sanitizer max ops to take this into account.
                   *
                   * Note: This check *must* be right after coverage sanitize. */
                  c->check_ops ((આ + coverage).get_population () >> 1) &&
                  કિંમત_Format_2.sanitize_કિંમત (c, આ, values));

  }

  તાર્કિક intersects (અચલ hb_set_t *glyphs) અચલ
  { પાછા (આ+coverage).intersects (glyphs); }

  શૂન્ય closure_lookups (hb_closure_lookups_context_t *c) અચલ {}
  શૂન્ય collect_variation_indices (hb_collect_variation_indices_context_t *c) અચલ
  {
    જો (!કિંમત_Format_2.has_device ()) પાછા;

    hb_set_t intersection;
    (આ+coverage).intersect_set (*c->glyph_set, intersection);
    જો (!intersection) પાછા;

    કિંમત_Format_2.collect_variation_indices (c, આ, values.as_array (કિંમત_Format_2.get_len ()));
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

    જો (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "positioning glyph at %u",
			  c->buffer->idx);
    }

    કિંમત_Format_2.apply_કિંમત (c, આ, values, buffer->cur_pos());

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

    કિંમત_Format_2.position_single (font, table_blob, direction, આ,
                                 values, pos);
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
    જો (unlikely (!c->extend_min (આ))) પાછા;
    જો (unlikely (!c->check_assign (કિંમત_Format_2,
                                    newFormat,
                                    HB_SERIALIZE_ERROR_INT_OVERFLOW))) પાછા;

    માટે (અચલ hb_array_t<અચલ કિંમત>& _ : + it | hb_map (hb_second))
    {
      src_format.copy_કિંમતો (c, newFormat, src,  &_, layout_variation_idx_delta_map);
      // Only serialize the first entry in the iterator, the rest are assumed to
      // be the same.
      તોડો;
    }

    auto glyphs =
    + it
    | hb_map_retains_sorting (hb_first)
    ;

    coverage.serialize_serialize (c, glyphs);
  }

  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);
    અચલ hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    અચલ hb_map_t &glyph_map = *c->plan->glyph_map;

    hb_set_t intersection;
    (આ+coverage).intersect_set (glyphset, intersection);

    unsigned new_format = કિંમત_Format_2;

    જો (c->plan->normalized_coords)
    {
      new_format = કિંમત_Format_2.get_effective_format (values.arrayZ, ખોટું, ખોટું, આ, &c->plan->layout_variation_idx_delta_map);
    }
    /* do not strip hints for VF */
    નહીંતર જો (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
    {
      hb_blob_t* blob = hb_face_reference_table (c->plan->source, HB_TAG ('f','v','a','r'));
      તાર્કિક has_fvar = (blob != hb_blob_get_empty ());
      hb_blob_destroy (blob);

      તાર્કિક strip = !has_fvar;
      /* special case: strip hints when a VF has no GDEF varstore after
       * subsetting*/
      જો (has_fvar && !c->plan->has_gdef_varstore)
        strip = સાચું;
      new_format = કિંમત_Format_2.get_effective_format (values.arrayZ,
                                                     strip, /* strip hints */
                                                     સાચું, /* strip empty */
                                                     આ, nullptr);
    }

    auto it =
    + hb_iter (intersection)
    | hb_map_retains_sorting (glyph_map)
    | hb_zip (hb_repeat (values.as_array (કિંમત_Format_2.get_len ())))
    ;

    તાર્કિક ret = તાર્કિક (it);
    SinglePos_serialize (c->serializer, આ, કિંમત_Format_2, it, &c->plan->layout_variation_idx_delta_map, new_format);
    return_trace (ret);
  }
};

}
}
}

#endif /* OT_LAYOUT_GPOS_SINGLEPOSFORMAT1_HH */
