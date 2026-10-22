#ifndef OT_LAYOUT_GPOS_SINGLEPOSFORMAT1_HH
#define OT_LAYOUT_GPOS_SINGLEPOSFORMAT1_HH

#شامل "Common.hh"
#شامل "ValueFormat.hh"

فضاینام OT {
فضاینام Layout {
فضاینام GPOS_impl {

template <typename Types>
ساختار SinglePosFormat1_3 : مقدار_Base
{
  محافظتشده:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of subtable */
  مقدار_Format   مقدار_Format_2;            /* Defines the types of data in the
                                         * ValueRecord */
  مقدار_Record   values;                 /* Defines positioning
                                         * value(s)--applied to all glyphs in
                                         * the Coverage table */
  عمومی:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + مقدار_Format::static_size, values);

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) &&
                  coverage.sanitize (c, این) &&
		  hb_barrier () &&
                  /* The coverage  table may use a range to represent a set
                   * of glyphs, which means a small number of bytes can
                   * generate a large glyph set. Manually modify the
                   * sanitizer max ops to take this into account.
                   *
                   * Note: This check *must* be right after coverage sanitize. */
                  c->check_ops ((این + coverage).get_population () >> 1) &&
                  مقدار_Format_2.sanitize_مقدار (c, این, values));

  }

  منطقی intersects (ثابت hb_set_t *glyphs) ثابت
  { بازگشت (این+coverage).intersects (glyphs); }

  تهی closure_lookups (hb_closure_lookups_context_t *c) ثابت {}
  تهی collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    اگر (!مقدار_Format_2.has_device ()) بازگشت;

    hb_set_t intersection;
    (این+coverage).intersect_set (*c->glyph_set, intersection);
    اگر (!intersection) بازگشت;

    مقدار_Format_2.collect_variation_indices (c, این, values.as_array (مقدار_Format_2.get_len ()));
  }

  تهی collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  { اگر (unlikely (!(این+coverage).collect_coverage (c->درون_داد))) بازگشت; }

  ثابت Coverage &get_coverage () ثابت { بازگشت این+coverage; }

  مقدار_Format get_مقدار_format () ثابت { بازگشت مقدار_Format_2; }

  منطقی apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (این);
    hb_buffer_t *buffer = c->buffer;
    unsigned صحیح index = (این+coverage).get_coverage  (buffer->cur().codepoint);
    اگر (index == NOT_COVERED) return_trace (نادرست);

    اگر (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "positioning glyph at %u",
			  c->buffer->idx);
    }

    مقدار_Format_2.apply_مقدار (c, این, values, buffer->cur_pos());

    اگر (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "positioned glyph at %u",
			  c->buffer->idx);
    }

    buffer->idx++;
    return_trace (درست);
  }

  منطقی
  position_single (hb_font_t           *font,
		   hb_blob_t           *table_blob,
		   hb_direction_t       direction,
		   hb_codepoint_t       gid,
		   hb_glyph_position_t &pos) ثابت
  {
    unsigned صحیح index = (این+coverage).get_coverage  (gid);
    اگر (likely (index == NOT_COVERED)) بازگشت نادرست;

    مقدار_Format_2.position_single (font, table_blob, direction, این,
                                 values, pos);
    بازگشت درست;
  }

  template<typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  تهی serialize (hb_serialize_context_t *c,
                  ثابت مقدار_Base *src,
                  مقدار_Format src_format,
                  Iterator it,
                  مقدار_Format newFormat,
                  ثابت hb_hashmap_t<unsigned, hb_pair_t<unsigned, صحیح>> *layout_variation_idx_delta_map)
  {
    اگر (unlikely (!c->extend_min (این))) بازگشت;
    اگر (unlikely (!c->check_assign (مقدار_Format_2,
                                    newFormat,
                                    HB_SERIALIZE_ERROR_INT_OVERFLOW))) بازگشت;

    برای (ثابت hb_array_t<ثابت مقدار>& _ : + it | hb_map (hb_second))
    {
      src_format.copy_مقادیر (c, newFormat, src,  &_, layout_variation_idx_delta_map);
      // Only serialize the first entry in the iterator, the rest are assumed to
      // be the same.
      بشکن;
    }

    auto glyphs =
    + it
    | hb_map_retains_sorting (hb_first)
    ;

    coverage.serialize_serialize (c, glyphs);
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);
    ثابت hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    hb_set_t intersection;
    (این+coverage).intersect_set (glyphset, intersection);

    unsigned new_format = مقدار_Format_2;

    اگر (c->plan->normalized_coords)
    {
      new_format = مقدار_Format_2.get_effective_format (values.arrayZ, نادرست, نادرست, این, &c->plan->layout_variation_idx_delta_map);
    }
    /* do not strip hints for VF */
    وگرنه اگر (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
    {
      hb_blob_t* blob = hb_face_reference_table (c->plan->source, HB_TAG ('f','v','a','r'));
      منطقی has_fvar = (blob != hb_blob_get_empty ());
      hb_blob_destroy (blob);

      منطقی strip = !has_fvar;
      /* special case: strip hints when a VF has no GDEF varstore after
       * subsetting*/
      اگر (has_fvar && !c->plan->has_gdef_varstore)
        strip = درست;
      new_format = مقدار_Format_2.get_effective_format (values.arrayZ,
                                                     strip, /* strip hints */
                                                     درست, /* strip empty */
                                                     این, nullptr);
    }

    auto it =
    + hb_iter (intersection)
    | hb_map_retains_sorting (glyph_map)
    | hb_zip (hb_repeat (values.as_array (مقدار_Format_2.get_len ())))
    ;

    منطقی ret = منطقی (it);
    SinglePos_serialize (c->serializer, این, مقدار_Format_2, it, &c->plan->layout_variation_idx_delta_map, new_format);
    return_trace (ret);
  }
};

}
}
}

#endif /* OT_LAYOUT_GPOS_SINGLEPOSFORMAT1_HH */
