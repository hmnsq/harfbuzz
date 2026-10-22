#ifndef OT_LAYOUT_GPOS_SINGLEPOSFORMAT1_HH
#define OT_LAYOUT_GPOS_SINGLEPOSFORMAT1_HH

#ଅନ୍ତର୍ଭାଵ "Common.hh"
#ଅନ୍ତର୍ଭାଵ "ValueFormat.hh"

ନାମାକାଶ OT {
ନାମାକାଶ Layout {
ନାମାକାଶ GPOS_impl {

template <typename Types>
ସଂରଚନା SinglePosFormat1_3 : ମୂଲ୍ୟ_Base
{
  ସୁରକ୍ଷିତ:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of subtable */
  ମୂଲ୍ୟ_Format   ମୂଲ୍ୟ_Format_2;            /* Defines the types of data in the
                                         * ValueRecord */
  ମୂଲ୍ୟ_Record   values;                 /* Defines positioning
                                         * value(s)--applied to all glyphs in
                                         * the Coverage table */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + ମୂଲ୍ୟ_Format::static_size, values);

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) &&
                  coverage.sanitize (c, ଏହି) &&
		  hb_barrier () &&
                  /* The coverage  table may use a range to represent a set
                   * of glyphs, which means a small number of bytes can
                   * generate a large glyph set. Manually modify the
                   * sanitizer max ops to take this into account.
                   *
                   * Note: This check *must* be right after coverage sanitize. */
                  c->check_ops ((ଏହି + coverage).get_population () >> 1) &&
                  ମୂଲ୍ୟ_Format_2.sanitize_ମୂଲ୍ୟ (c, ଏହି, values));

  }

  ତାର୍କିକ intersects (ଅଚଳ hb_set_t *glyphs) ଅଚଳ
  { ଫେରନ୍ତୁ (ଏହି+coverage).intersects (glyphs); }

  ଶୂନ୍ୟ closure_lookups (hb_closure_lookups_context_t *c) ଅଚଳ {}
  ଶୂନ୍ୟ collect_variation_indices (hb_collect_variation_indices_context_t *c) ଅଚଳ
  {
    ଯଦି (!ମୂଲ୍ୟ_Format_2.has_device ()) ଫେରନ୍ତୁ;

    hb_set_t intersection;
    (ଏହି+coverage).intersect_set (*c->glyph_set, intersection);
    ଯଦି (!intersection) ଫେରନ୍ତୁ;

    ମୂଲ୍ୟ_Format_2.collect_variation_indices (c, ଏହି, values.as_array (ମୂଲ୍ୟ_Format_2.get_len ()));
  }

  ଶୂନ୍ୟ collect_glyphs (hb_collect_glyphs_context_t *c) ଅଚଳ
  { ଯଦି (unlikely (!(ଏହି+coverage).collect_coverage (c->ନିବେଶ))) ଫେରନ୍ତୁ; }

  ଅଚଳ Coverage &get_coverage () ଅଚଳ { ଫେରନ୍ତୁ ଏହି+coverage; }

  ମୂଲ୍ୟ_Format get_ମୂଲ୍ୟ_format () ଅଚଳ { ଫେରନ୍ତୁ ମୂଲ୍ୟ_Format_2; }

  ତାର୍କିକ apply (hb_ot_apply_context_t *c) ଅଚଳ
  {
    TRACE_APPLY (ଏହି);
    hb_buffer_t *buffer = c->buffer;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = (ଏହି+coverage).get_coverage  (buffer->cur().codepoint);
    ଯଦି (index == NOT_COVERED) return_trace (ମିଥ୍ୟା);

    ଯଦି (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "positioning glyph at %u",
			  c->buffer->idx);
    }

    ମୂଲ୍ୟ_Format_2.apply_ମୂଲ୍ୟ (c, ଏହି, values, buffer->cur_pos());

    ଯଦି (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "positioned glyph at %u",
			  c->buffer->idx);
    }

    buffer->idx++;
    return_trace (ସତ୍ୟ);
  }

  ତାର୍କିକ
  position_single (hb_font_t           *font,
		   hb_blob_t           *table_blob,
		   hb_direction_t       direction,
		   hb_codepoint_t       gid,
		   hb_glyph_position_t &pos) ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = (ଏହି+coverage).get_coverage  (gid);
    ଯଦି (likely (index == NOT_COVERED)) ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ମୂଲ୍ୟ_Format_2.position_single (font, table_blob, direction, ଏହି,
                                 values, pos);
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  template<typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  ଶୂନ୍ୟ serialize (hb_serialize_context_t *c,
                  ଅଚଳ ମୂଲ୍ୟ_Base *src,
                  ମୂଲ୍ୟ_Format src_format,
                  Iterator it,
                  ମୂଲ୍ୟ_Format newFormat,
                  ଅଚଳ hb_hashmap_t<unsigned, hb_pair_t<unsigned, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>> *layout_variation_idx_delta_map)
  {
    ଯଦି (unlikely (!c->extend_min (ଏହି))) ଫେରନ୍ତୁ;
    ଯଦି (unlikely (!c->check_assign (ମୂଲ୍ୟ_Format_2,
                                    newFormat,
                                    HB_SERIALIZE_ERROR_INT_OVERFLOW))) ଫେରନ୍ତୁ;

    ପାଇଁ (ଅଚଳ hb_array_t<ଅଚଳ ମୂଲ୍ୟ>& _ : + it | hb_map (hb_second))
    {
      src_format.copy_ମୂଲ୍ୟଗୁଡ଼ିକ (c, newFormat, src,  &_, layout_variation_idx_delta_map);
      // Only serialize the first entry in the iterator, the rest are assumed to
      // be the same.
      ଭାଙ୍ଗ;
    }

    auto glyphs =
    + it
    | hb_map_retains_sorting (hb_first)
    ;

    coverage.serialize_serialize (c, glyphs);
  }

  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    ଅଚଳ hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ଅଚଳ hb_map_t &glyph_map = *c->plan->glyph_map;

    hb_set_t intersection;
    (ଏହି+coverage).intersect_set (glyphset, intersection);

    unsigned new_format = ମୂଲ୍ୟ_Format_2;

    ଯଦି (c->plan->normalized_coords)
    {
      new_format = ମୂଲ୍ୟ_Format_2.get_effective_format (values.arrayZ, ମିଥ୍ୟା, ମିଥ୍ୟା, ଏହି, &c->plan->layout_variation_idx_delta_map);
    }
    /* do not strip hints for VF */
    ଅନ୍ୟଥା ଯଦି (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
    {
      hb_blob_t* blob = hb_face_reference_table (c->plan->source, HB_TAG ('f','v','a','r'));
      ତାର୍କିକ has_fvar = (blob != hb_blob_get_empty ());
      hb_blob_destroy (blob);

      ତାର୍କିକ strip = !has_fvar;
      /* special case: strip hints when a VF has no GDEF varstore after
       * subsetting*/
      ଯଦି (has_fvar && !c->plan->has_gdef_varstore)
        strip = ସତ୍ୟ;
      new_format = ମୂଲ୍ୟ_Format_2.get_effective_format (values.arrayZ,
                                                     strip, /* strip hints */
                                                     ସତ୍ୟ, /* strip empty */
                                                     ଏହି, nullptr);
    }

    auto it =
    + hb_iter (intersection)
    | hb_map_retains_sorting (glyph_map)
    | hb_zip (hb_repeat (values.as_array (ମୂଲ୍ୟ_Format_2.get_len ())))
    ;

    ତାର୍କିକ ret = ତାର୍କିକ (it);
    SinglePos_serialize (c->serializer, ଏହି, ମୂଲ୍ୟ_Format_2, it, &c->plan->layout_variation_idx_delta_map, new_format);
    return_trace (ret);
  }
};

}
}
}

#endif /* OT_LAYOUT_GPOS_SINGLEPOSFORMAT1_HH */
