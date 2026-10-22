#ifndef OT_LAYOUT_GPOS_SINGLEPOSFORMAT1_HH
#define OT_LAYOUT_GPOS_SINGLEPOSFORMAT1_HH

#अन्तर्भाव "Common.hh"
#अन्तर्भाव "ValueFormat.hh"

नामाकाश OT {
नामाकाश Layout {
नामाकाश GPOS_impl {

ढाँचा <typename Types>
संरचना SinglePosFormat1_3 : मूल्य_Base
{
  संरक्षित:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::ढाँचा LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of subtable */
  मूल्य_Format   मूल्य_Format_2;            /* Defines the types of data in the
                                         * ValueRecord */
  मूल्य_Record   values;                 /* Defines positioning
                                         * value(s)--applied to all glyphs in
                                         * the Coverage table */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + मूल्य_Format::static_size, values);

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) &&
                  coverage.sanitize (c, हे) &&
		  hb_barrier () &&
                  /* The coverage  table may use a range to represent a set
                   * of glyphs, which means a small number of bytes can
                   * generate a large glyph set. Manually modify the
                   * sanitizer max ops to take this into account.
                   *
                   * Note: This check *must* be right after coverage sanitize. */
                  c->check_ops ((हे + coverage).get_population () >> 1) &&
                  मूल्य_Format_2.sanitize_मूल्य (c, हे, values));

  }

  तार्किक intersects (स्थिरांक hb_set_t *glyphs) स्थिरांक
  { परत (हे+coverage).intersects (glyphs); }

  शून्य closure_lookups (hb_closure_lookups_context_t *c) स्थिरांक {}
  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) स्थिरांक
  {
    जर (!मूल्य_Format_2.has_device ()) परत;

    hb_set_t intersection;
    (हे+coverage).intersect_set (*c->glyph_set, intersection);
    जर (!intersection) परत;

    मूल्य_Format_2.collect_variation_indices (c, हे, values.as_array (मूल्य_Format_2.get_len ()));
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

    जर (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "positioning glyph at %u",
			  c->buffer->idx);
    }

    मूल्य_Format_2.apply_मूल्य (c, हे, values, buffer->cur_pos());

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

    मूल्य_Format_2.position_single (font, table_blob, direction, हे,
                                 values, pos);
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
    जर (unlikely (!c->extend_min (हे))) परत;
    जर (unlikely (!c->check_assign (मूल्य_Format_2,
                                    newFormat,
                                    HB_SERIALIZE_ERROR_INT_OVERFLOW))) परत;

    साठी (स्थिरांक hb_array_t<स्थिरांक मूल्य>& _ : + it | hb_map (hb_second))
    {
      src_format.copy_मूल्ये (c, newFormat, src,  &_, layout_variation_idx_delta_map);
      // Only serialize the first entry in the iterator, the rest are assumed to
      // be the same.
      थांबा;
    }

    स्वतः glyphs =
    + it
    | hb_map_retains_sorting (hb_first)
    ;

    coverage.serialize_serialize (c, glyphs);
  }

  तार्किक subset (hb_subset_context_t *c) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्थिरांक hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    स्थिरांक hb_map_t &glyph_map = *c->plan->glyph_map;

    hb_set_t intersection;
    (हे+coverage).intersect_set (glyphset, intersection);

    अचिन्हित new_format = मूल्य_Format_2;

    जर (c->plan->normalized_coords)
    {
      new_format = मूल्य_Format_2.get_effective_format (values.arrayZ, असत्य, असत्य, हे, &c->plan->layout_variation_idx_delta_map);
    }
    /* do not strip hints for VF */
    अन्यथा जर (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
    {
      hb_blob_t* blob = hb_face_reference_table (c->plan->source, HB_TAG ('f','v','a','r'));
      तार्किक has_fvar = (blob != hb_blob_get_empty ());
      hb_blob_destroy (blob);

      तार्किक strip = !has_fvar;
      /* special case: strip hints when a VF has no GDEF varstore after
       * subsetting*/
      जर (has_fvar && !c->plan->has_gdef_varstore)
        strip = सत्य;
      new_format = मूल्य_Format_2.get_effective_format (values.arrayZ,
                                                     strip, /* strip hints */
                                                     सत्य, /* strip empty */
                                                     हे, nullptr);
    }

    स्वतः it =
    + hb_iter (intersection)
    | hb_map_retains_sorting (glyph_map)
    | hb_zip (hb_repeat (values.as_array (मूल्य_Format_2.get_len ())))
    ;

    तार्किक ret = तार्किक (it);
    SinglePos_serialize (c->serializer, हे, मूल्य_Format_2, it, &c->plan->layout_variation_idx_delta_map, new_format);
    return_trace (ret);
  }
};

}
}
}

#endif /* OT_LAYOUT_GPOS_SINGLEPOSFORMAT1_HH */
