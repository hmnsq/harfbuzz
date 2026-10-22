#ifndef OT_LAYOUT_GPOS_SINGLEPOSFORMAT1_HH
#define OT_LAYOUT_GPOS_SINGLEPOSFORMAT1_HH

#অন্তর্ভাৱ "Common.hh"
#অন্তর্ভাৱ "ValueFormat.hh"

নামাকাশ OT {
নামাকাশ Layout {
নামাকাশ GPOS_impl {

template <typename Types>
সংরচনা SinglePosFormat1_3 : মান_Base
{
  রক্ষিত:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of subtable */
  মান_Format   মান_Format_2;            /* Defines the types of data in the
                                         * ValueRecord */
  মান_Record   values;                 /* Defines positioning
                                         * value(s)--applied to all glyphs in
                                         * the Coverage table */
  সার্ৱজনিক:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + মান_Format::static_size, values);

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) &&
                  coverage.sanitize (c, এতত্) &&
		  hb_barrier () &&
                  /* The coverage  table may use a range to represent a set
                   * of glyphs, which means a small number of bytes can
                   * generate a large glyph set. Manually modify the
                   * sanitizer max ops to take this into account.
                   *
                   * Note: This check *must* be right after coverage sanitize. */
                  c->check_ops ((এতত্ + coverage).get_population () >> 1) &&
                  মান_Format_2.sanitize_মান (c, এতত্, values));

  }

  তার্কিক intersects (ধ্রুৱ hb_set_t *glyphs) ধ্রুৱ
  { ঘূৰাই (এতত্+coverage).intersects (glyphs); }

  শূন্য closure_lookups (hb_closure_lookups_context_t *c) ধ্রুৱ {}
  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c) ধ্রুৱ
  {
    যদি (!মান_Format_2.has_device ()) ঘূৰাই;

    hb_set_t intersection;
    (এতত্+coverage).intersect_set (*c->glyph_set, intersection);
    যদি (!intersection) ঘূৰাই;

    মান_Format_2.collect_variation_indices (c, এতত্, values.as_array (মান_Format_2.get_len ()));
  }

  শূন্য collect_glyphs (hb_collect_glyphs_context_t *c) ধ্রুৱ
  { যদি (unlikely (!(এতত্+coverage).collect_coverage (c->প্ৰবেশ))) ঘূৰাই; }

  ধ্রুৱ Coverage &get_coverage () ধ্রুৱ { ঘূৰাই এতত্+coverage; }

  মান_Format get_মান_format () ধ্রুৱ { ঘূৰাই মান_Format_2; }

  তার্কিক apply (hb_ot_apply_context_t *c) ধ্রুৱ
  {
    TRACE_APPLY (এতত্);
    hb_buffer_t *buffer = c->buffer;
    unsigned পূর্ণাঙ্ক index = (এতত্+coverage).get_coverage  (buffer->cur().codepoint);
    যদি (index == NOT_COVERED) return_trace (মিছা);

    যদি (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "positioning glyph at %u",
			  c->buffer->idx);
    }

    মান_Format_2.apply_মান (c, এতত্, values, buffer->cur_pos());

    যদি (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "positioned glyph at %u",
			  c->buffer->idx);
    }

    buffer->idx++;
    return_trace (সঁচা);
  }

  তার্কিক
  position_single (hb_font_t           *font,
		   hb_blob_t           *table_blob,
		   hb_direction_t       direction,
		   hb_codepoint_t       gid,
		   hb_glyph_position_t &pos) ধ্রুৱ
  {
    unsigned পূর্ণাঙ্ক index = (এতত্+coverage).get_coverage  (gid);
    যদি (likely (index == NOT_COVERED)) ঘূৰাই মিছা;

    মান_Format_2.position_single (font, table_blob, direction, এতত্,
                                 values, pos);
    ঘূৰাই সঁচা;
  }

  template<typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  শূন্য serialize (hb_serialize_context_t *c,
                  ধ্রুৱ মান_Base *src,
                  মান_Format src_format,
                  Iterator it,
                  মান_Format newFormat,
                  ধ্রুৱ hb_hashmap_t<unsigned, hb_pair_t<unsigned, পূর্ণাঙ্ক>> *layout_variation_idx_delta_map)
  {
    যদি (unlikely (!c->extend_min (এতত্))) ঘূৰাই;
    যদি (unlikely (!c->check_assign (মান_Format_2,
                                    newFormat,
                                    HB_SERIALIZE_ERROR_INT_OVERFLOW))) ঘূৰাই;

    বাবে (ধ্রুৱ hb_array_t<ধ্রুৱ মান>& _ : + it | hb_map (hb_second))
    {
      src_format.copy_মানসমূহ (c, newFormat, src,  &_, layout_variation_idx_delta_map);
      // Only serialize the first entry in the iterator, the rest are assumed to
      // be the same.
      ভঙ্গ;
    }

    auto glyphs =
    + it
    | hb_map_retains_sorting (hb_first)
    ;

    coverage.serialize_serialize (c, glyphs);
  }

  তার্কিক subset (hb_subset_context_t *c) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    ধ্রুৱ hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ধ্রুৱ hb_map_t &glyph_map = *c->plan->glyph_map;

    hb_set_t intersection;
    (এতত্+coverage).intersect_set (glyphset, intersection);

    unsigned new_format = মান_Format_2;

    যদি (c->plan->normalized_coords)
    {
      new_format = মান_Format_2.get_effective_format (values.arrayZ, মিছা, মিছা, এতত্, &c->plan->layout_variation_idx_delta_map);
    }
    /* do not strip hints for VF */
    অন্যথা যদি (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
    {
      hb_blob_t* blob = hb_face_reference_table (c->plan->source, HB_TAG ('f','v','a','r'));
      তার্কিক has_fvar = (blob != hb_blob_get_empty ());
      hb_blob_destroy (blob);

      তার্কিক strip = !has_fvar;
      /* special case: strip hints when a VF has no GDEF varstore after
       * subsetting*/
      যদি (has_fvar && !c->plan->has_gdef_varstore)
        strip = সঁচা;
      new_format = মান_Format_2.get_effective_format (values.arrayZ,
                                                     strip, /* strip hints */
                                                     সঁচা, /* strip empty */
                                                     এতত্, nullptr);
    }

    auto it =
    + hb_iter (intersection)
    | hb_map_retains_sorting (glyph_map)
    | hb_zip (hb_repeat (values.as_array (মান_Format_2.get_len ())))
    ;

    তার্কিক ret = তার্কিক (it);
    SinglePos_serialize (c->serializer, এতত্, মান_Format_2, it, &c->plan->layout_variation_idx_delta_map, new_format);
    return_trace (ret);
  }
};

}
}
}

#endif /* OT_LAYOUT_GPOS_SINGLEPOSFORMAT1_HH */
