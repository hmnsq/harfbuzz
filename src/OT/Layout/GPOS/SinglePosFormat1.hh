#ifndef OT_LAYOUT_GPOS_SINGLEPOSFORMAT1_HH
#define OT_LAYOUT_GPOS_SINGLEPOSFORMAT1_HH

#ಅನ್ತರ್ಭಾವ "Common.hh"
#ಅನ್ತರ್ಭಾವ "ValueFormat.hh"

ನಾಮಾಕಾಶ OT {
ನಾಮಾಕಾಶ Layout {
ನಾಮಾಕಾಶ GPOS_impl {

template <typename Types>
ಸಂರಚನಾ SinglePosFormat1_3 : ಮೌಲ್ಯ_Base
{
  ರಕ್ಷಿತ:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of subtable */
  ಮೌಲ್ಯ_Format   ಮೌಲ್ಯ_Format_2;            /* Defines the types of data in the
                                         * ValueRecord */
  ಮೌಲ್ಯ_Record   values;                 /* Defines positioning
                                         * value(s)--applied to all glyphs in
                                         * the Coverage table */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + ಮೌಲ್ಯ_Format::static_size, values);

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) &&
                  coverage.sanitize (c, ಏತತ್) &&
		  hb_barrier () &&
                  /* The coverage  table may use a range to represent a set
                   * of glyphs, which means a small number of bytes can
                   * generate a large glyph set. Manually modify the
                   * sanitizer max ops to take this into account.
                   *
                   * Note: This check *must* be right after coverage sanitize. */
                  c->check_ops ((ಏತತ್ + coverage).get_population () >> 1) &&
                  ಮೌಲ್ಯ_Format_2.sanitize_ಮೌಲ್ಯ (c, ಏತತ್, values));

  }

  ತಾರ್ಕಿಕ intersects (ಧ್ರುವ hb_set_t *glyphs) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ (ಏತತ್+coverage).intersects (glyphs); }

  ಶೂನ್ಯ closure_lookups (hb_closure_lookups_context_t *c) ಧ್ರುವ {}
  ಶೂನ್ಯ collect_variation_indices (hb_collect_variation_indices_context_t *c) ಧ್ರುವ
  {
    ಯದಿ (!ಮೌಲ್ಯ_Format_2.has_device ()) ಪ್ರತ್ಯಾಗಮ;

    hb_set_t intersection;
    (ಏತತ್+coverage).intersect_set (*c->glyph_set, intersection);
    ಯದಿ (!intersection) ಪ್ರತ್ಯಾಗಮ;

    ಮೌಲ್ಯ_Format_2.collect_variation_indices (c, ಏತತ್, values.as_array (ಮೌಲ್ಯ_Format_2.get_len ()));
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

    ಯದಿ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "positioning glyph at %u",
			  c->buffer->idx);
    }

    ಮೌಲ್ಯ_Format_2.apply_ಮೌಲ್ಯ (c, ಏತತ್, values, buffer->cur_pos());

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

    ಮೌಲ್ಯ_Format_2.position_single (font, table_blob, direction, ಏತತ್,
                                 values, pos);
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
    ಯದಿ (unlikely (!c->extend_min (ಏತತ್))) ಪ್ರತ್ಯಾಗಮ;
    ಯದಿ (unlikely (!c->check_assign (ಮೌಲ್ಯ_Format_2,
                                    newFormat,
                                    HB_SERIALIZE_ERROR_INT_OVERFLOW))) ಪ್ರತ್ಯಾಗಮ;

    ಪ್ರತಿ (ಧ್ರುವ hb_array_t<ಧ್ರುವ ಮೌಲ್ಯ>& _ : + it | hb_map (hb_second))
    {
      src_format.copy_ಮೌಲ್ಯಗಳು (c, newFormat, src,  &_, layout_variation_idx_delta_map);
      // Only serialize the first entry in the iterator, the rest are assumed to
      // be the same.
      ಭಙ್ಗ;
    }

    auto glyphs =
    + it
    | hb_map_retains_sorting (hb_first)
    ;

    coverage.serialize_serialize (c, glyphs);
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    ಧ್ರುವ hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ಧ್ರುವ hb_map_t &glyph_map = *c->plan->glyph_map;

    hb_set_t intersection;
    (ಏತತ್+coverage).intersect_set (glyphset, intersection);

    unsigned new_format = ಮೌಲ್ಯ_Format_2;

    ಯದಿ (c->plan->normalized_coords)
    {
      new_format = ಮೌಲ್ಯ_Format_2.get_effective_format (values.arrayZ, ಅಸತ್ಯ, ಅಸತ್ಯ, ಏತತ್, &c->plan->layout_variation_idx_delta_map);
    }
    /* do not strip hints for VF */
    ಅನ್ಯಥಾ ಯದಿ (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
    {
      hb_blob_t* blob = hb_face_reference_table (c->plan->source, HB_TAG ('f','v','a','r'));
      ತಾರ್ಕಿಕ has_fvar = (blob != hb_blob_get_empty ());
      hb_blob_destroy (blob);

      ತಾರ್ಕಿಕ strip = !has_fvar;
      /* special case: strip hints when a VF has no GDEF varstore after
       * subsetting*/
      ಯದಿ (has_fvar && !c->plan->has_gdef_varstore)
        strip = ಸತ್ಯ;
      new_format = ಮೌಲ್ಯ_Format_2.get_effective_format (values.arrayZ,
                                                     strip, /* strip hints */
                                                     ಸತ್ಯ, /* strip empty */
                                                     ಏತತ್, nullptr);
    }

    auto it =
    + hb_iter (intersection)
    | hb_map_retains_sorting (glyph_map)
    | hb_zip (hb_repeat (values.as_array (ಮೌಲ್ಯ_Format_2.get_len ())))
    ;

    ತಾರ್ಕಿಕ ret = ತಾರ್ಕಿಕ (it);
    SinglePos_serialize (c->serializer, ಏತತ್, ಮೌಲ್ಯ_Format_2, it, &c->plan->layout_variation_idx_delta_map, new_format);
    return_trace (ret);
  }
};

}
}
}

#endif /* OT_LAYOUT_GPOS_SINGLEPOSFORMAT1_HH */
