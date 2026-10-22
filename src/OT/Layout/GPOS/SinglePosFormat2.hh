#ifndef OT_LAYOUT_GPOS_SINGLEPOSFORMAT2_HH
#define OT_LAYOUT_GPOS_SINGLEPOSFORMAT2_HH

#অন্তর্ভাব "Common.hh"

নামাকাশ OT {
নামাকাশ Layout {
নামাকাশ GPOS_impl {

/* Keep the iterator type independent of the source table width so
 * the output serializers can share their instantiations. */
সংরচনা single_pos_মান_mapper_t
{
  ধ্রুব hb_map_t &glyph_map;
  ধ্রুব hb_array_t<ধ্রুব মান> &values;
  ধ্রুব unsigned &sub_length;

  hb_pair_t<hb_codepoint_t, hb_array_t<ধ্রুব মান>>
  operator () (ধ্রুব hb_pair_t<hb_codepoint_t, unsigned> &_) ধ্রুব
  {
    প্রত্যাগম hb_pair (glyph_map[_.first],
                    values.sub_array (_.second * sub_length, sub_length));
  }
};

template <typename Types>
সংরচনা SinglePosFormat2_4 : মান_Base
{
  রক্ষিত:
  HBUINT16      format;                 /* Format identifier--format = 2 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of subtable */
  মান_Format   মান_Format_2;            /* Defines the types of data in the
                                         * ValueRecord */
  typename Types::HBUINT
                মান_গুনতি;             /* Number of ValueRecords */
  মান_Record   values;                 /* Array of ValueRecords--positioning
                                         * values applied to glyphs */
  সার্বজনিক:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + মান_Format::static_size + Types::HBUINT::static_size, values);

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) &&
                  coverage.sanitize (c, এতত্) &&
                  মান_Format_2.sanitize_মানসমূহ (c, এতত্, values, মান_গুনতি));
  }

  তার্কিক intersects (ধ্রুব hb_set_t *glyphs) ধ্রুব
  { প্রত্যাগম (এতত্+coverage).intersects (glyphs); }

  শূন্য closure_lookups (hb_closure_lookups_context_t *c) ধ্রুব {}
  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c) ধ্রুব
  {
    যদি (!মান_Format_2.has_device ()) প্রত্যাগম;

    auto it =
    + hb_zip (এতত্+coverage, hb_range ((unsigned) মান_গুনতি))
    | hb_filter (c->glyph_set, hb_first)
    ;

    যদি (!it) প্রত্যাগম;

    unsigned sub_length = মান_Format_2.get_len ();
    ধ্রুব hb_array_t<ধ্রুব মান> মানসমূহ_array = values.as_array (মান_গুনতি * sub_length);

    প্রতি (unsigned i : + it
                      | hb_map (hb_second))
      মান_Format_2.collect_variation_indices (c, এতত্, মানসমূহ_array.sub_array (i * sub_length, sub_length));

  }

  শূন্য collect_glyphs (hb_collect_glyphs_context_t *c) ধ্রুব
  { যদি (unlikely (!(এতত্+coverage).collect_coverage (c->প্রবেশ))) প্রত্যাগম; }

  ধ্রুব Coverage &get_coverage () ধ্রুব { প্রত্যাগম এতত্+coverage; }

  মান_Format get_মান_format () ধ্রুব { প্রত্যাগম মান_Format_2; }

  তার্কিক apply (hb_ot_apply_context_t *c) ধ্রুব
  {
    TRACE_APPLY (এতত্);
    hb_buffer_t *buffer = c->buffer;
    unsigned পূর্ণাঙ্ক index = (এতত্+coverage).get_coverage  (buffer->cur().codepoint);
    যদি (index == NOT_COVERED) return_trace (অসত্য);

    যদি (unlikely (index >= মান_গুনতি)) return_trace (অসত্য);

    যদি (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "positioning glyph at %u",
			  c->buffer->idx);
    }

    মান_Format_2.apply_মান (c, এতত্,
                             &values[index * মান_Format_2.get_len ()],
                             buffer->cur_pos());

    যদি (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "positioned glyph at %u",
			  c->buffer->idx);
    }

    buffer->idx++;
    return_trace (সত্য);
  }

  তার্কিক
  position_single (hb_font_t           *font,
		   hb_blob_t           *table_blob,
		   hb_direction_t       direction,
		   hb_codepoint_t       gid,
		   hb_glyph_position_t &pos) ধ্রুব
  {
    unsigned পূর্ণাঙ্ক index = (এতত্+coverage).get_coverage  (gid);
    যদি (likely (index == NOT_COVERED)) প্রত্যাগম অসত্য;
    যদি (unlikely (index >= মান_গুনতি)) প্রত্যাগম অসত্য;

    মান_Format_2.position_single (font, table_blob, direction, এতত্,
                                 &values[index * মান_Format_2.get_len ()], pos);
    প্রত্যাগম সত্য;
  }


  template<typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  শূন্য serialize (hb_serialize_context_t *c,
                  ধ্রুব মান_Base *src,
                  মান_Format src_format,
                  Iterator it,
                  মান_Format newFormat,
                  ধ্রুব hb_hashmap_t<unsigned, hb_pair_t<unsigned, পূর্ণাঙ্ক>> *layout_variation_idx_delta_map)
  {
    auto out = c->extend_min (এতত্);
    যদি (unlikely (!out)) প্রত্যাগম;
    যদি (unlikely (!c->check_assign (মান_Format_2, newFormat, HB_SERIALIZE_ERROR_INT_OVERFLOW))) প্রত্যাগম;
    যদি (unlikely (!c->check_assign (মান_গুনতি, it.len (), HB_SERIALIZE_ERROR_ARRAY_OVERFLOW))) প্রত্যাগম;

    + it
    | hb_map (hb_second)
    | hb_apply ([&] (hb_array_t<ধ্রুব মান> _)
    { src_format.copy_মানসমূহ (c, newFormat, src, &_, layout_variation_idx_delta_map); })
    ;

    auto glyphs =
    + it
    | hb_map_retains_sorting (hb_first)
    ;

    coverage.serialize_serialize (c, glyphs);
  }

  template<typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  unsigned compute_effective_format (ধ্রুব hb_face_t *face,
                                     Iterator it,
                                     তার্কিক is_instancing, তার্কিক strip_hints,
                                     তার্কিক has_gdef_varstore,
                                     ধ্রুব hb_hashmap_t<unsigned, hb_pair_t<unsigned, পূর্ণাঙ্ক>> *varidx_delta_map) ধ্রুব
  {
    hb_blob_t* blob = hb_face_reference_table (face, HB_TAG ('f','v','a','r'));
    তার্কিক has_fvar = (blob != hb_blob_get_empty ());
    hb_blob_destroy (blob);

    unsigned new_format = 0;
    যদি (is_instancing)
    {
      new_format = new_format | মান_Format_2.get_effective_format (+ it | hb_map (hb_second), অসত্য, অসত্য, এতত্, varidx_delta_map);
    }
    /* do not strip hints for VF */
    অন্যথা যদি (strip_hints)
    {
      তার্কিক strip = !has_fvar;
      যদি (has_fvar && !has_gdef_varstore)
        strip = সত্য;
      new_format = new_format | মান_Format_2.get_effective_format (+ it | hb_map (hb_second), strip, সত্য, এতত্, nullptr);
    }
    অন্যথা
      new_format = মান_Format_2;

    প্রত্যাগম new_format;
  }

  তার্কিক subset (hb_subset_context_t *c) ধ্রুব
  {
    TRACE_SUBSET (এতত্);
    ধ্রুব hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ধ্রুব hb_map_t &glyph_map = *c->plan->glyph_map;

    unsigned sub_length = মান_Format_2.get_len ();
    auto মানসমূহ_array = values.as_array ((unsigned) মান_গুনতি * sub_length);

    auto it =
    + hb_zip (এতত্+coverage, hb_range ((unsigned) মান_গুনতি))
    | hb_filter (glyphset, hb_first)
    | hb_map_retains_sorting (single_pos_মান_mapper_t {glyph_map,
                                                       মানসমূহ_array,
                                                       sub_length})
    ;

    unsigned new_format = compute_effective_format (c->plan->source, it,
                                                    তার্কিক (c->plan->normalized_coords),
                                                    তার্কিক (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING),
                                                    c->plan->has_gdef_varstore,
                                                    &c->plan->layout_variation_idx_delta_map);
    তার্কিক ret = তার্কিক (it);
    SinglePos_serialize (c->serializer, এতত্, মান_Format_2, it, &c->plan->layout_variation_idx_delta_map, new_format);
    return_trace (ret);
  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_SINGLEPOSFORMAT2_HH */
