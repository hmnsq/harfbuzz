#ifndef OT_LAYOUT_GPOS_SINGLEPOSFORMAT2_HH
#define OT_LAYOUT_GPOS_SINGLEPOSFORMAT2_HH

#شامل "Common.hh"

فضاینام OT {
فضاینام Layout {
فضاینام GPOS_impl {

/* Keep the iterator type independent of the source table width so
 * the output serializers can share their instantiations. */
ساختار single_pos_مقدار_mapper_t
{
  ثابت hb_map_t &glyph_map;
  ثابت hb_array_t<ثابت مقدار> &values;
  ثابت unsigned &sub_length;

  hb_pair_t<hb_codepoint_t, hb_array_t<ثابت مقدار>>
  operator () (ثابت hb_pair_t<hb_codepoint_t, unsigned> &_) ثابت
  {
    بازگشت hb_pair (glyph_map[_.first],
                    values.sub_array (_.second * sub_length, sub_length));
  }
};

template <typename Types>
ساختار SinglePosFormat2_4 : مقدار_Base
{
  محافظتشده:
  HBUINT16      format;                 /* Format identifier--format = 2 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of subtable */
  مقدار_Format   مقدار_Format_2;            /* Defines the types of data in the
                                         * ValueRecord */
  typename Types::HBUINT
                مقدار_شمار;             /* Number of ValueRecords */
  مقدار_Record   values;                 /* Array of ValueRecords--positioning
                                         * values applied to glyphs */
  عمومی:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + مقدار_Format::static_size + Types::HBUINT::static_size, values);

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) &&
                  coverage.sanitize (c, این) &&
                  مقدار_Format_2.sanitize_مقادیر (c, این, values, مقدار_شمار));
  }

  منطقی intersects (ثابت hb_set_t *glyphs) ثابت
  { بازگشت (این+coverage).intersects (glyphs); }

  تهی closure_lookups (hb_closure_lookups_context_t *c) ثابت {}
  تهی collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    اگر (!مقدار_Format_2.has_device ()) بازگشت;

    auto it =
    + hb_zip (این+coverage, hb_range ((unsigned) مقدار_شمار))
    | hb_filter (c->glyph_set, hb_first)
    ;

    اگر (!it) بازگشت;

    unsigned sub_length = مقدار_Format_2.get_len ();
    ثابت hb_array_t<ثابت مقدار> مقادیر_array = values.as_array (مقدار_شمار * sub_length);

    برای (unsigned i : + it
                      | hb_map (hb_second))
      مقدار_Format_2.collect_variation_indices (c, این, مقادیر_array.sub_array (i * sub_length, sub_length));

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

    اگر (unlikely (index >= مقدار_شمار)) return_trace (نادرست);

    اگر (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "positioning glyph at %u",
			  c->buffer->idx);
    }

    مقدار_Format_2.apply_مقدار (c, این,
                             &values[index * مقدار_Format_2.get_len ()],
                             buffer->cur_pos());

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
    اگر (unlikely (index >= مقدار_شمار)) بازگشت نادرست;

    مقدار_Format_2.position_single (font, table_blob, direction, این,
                                 &values[index * مقدار_Format_2.get_len ()], pos);
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
    auto out = c->extend_min (این);
    اگر (unlikely (!out)) بازگشت;
    اگر (unlikely (!c->check_assign (مقدار_Format_2, newFormat, HB_SERIALIZE_ERROR_INT_OVERFLOW))) بازگشت;
    اگر (unlikely (!c->check_assign (مقدار_شمار, it.len (), HB_SERIALIZE_ERROR_ARRAY_OVERFLOW))) بازگشت;

    + it
    | hb_map (hb_second)
    | hb_apply ([&] (hb_array_t<ثابت مقدار> _)
    { src_format.copy_مقادیر (c, newFormat, src, &_, layout_variation_idx_delta_map); })
    ;

    auto glyphs =
    + it
    | hb_map_retains_sorting (hb_first)
    ;

    coverage.serialize_serialize (c, glyphs);
  }

  template<typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  unsigned compute_effective_format (ثابت hb_face_t *face,
                                     Iterator it,
                                     منطقی is_instancing, منطقی strip_hints,
                                     منطقی has_gdef_varstore,
                                     ثابت hb_hashmap_t<unsigned, hb_pair_t<unsigned, صحیح>> *varidx_delta_map) ثابت
  {
    hb_blob_t* blob = hb_face_reference_table (face, HB_TAG ('f','v','a','r'));
    منطقی has_fvar = (blob != hb_blob_get_empty ());
    hb_blob_destroy (blob);

    unsigned new_format = 0;
    اگر (is_instancing)
    {
      new_format = new_format | مقدار_Format_2.get_effective_format (+ it | hb_map (hb_second), نادرست, نادرست, این, varidx_delta_map);
    }
    /* do not strip hints for VF */
    وگرنه اگر (strip_hints)
    {
      منطقی strip = !has_fvar;
      اگر (has_fvar && !has_gdef_varstore)
        strip = درست;
      new_format = new_format | مقدار_Format_2.get_effective_format (+ it | hb_map (hb_second), strip, درست, این, nullptr);
    }
    وگرنه
      new_format = مقدار_Format_2;

    بازگشت new_format;
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);
    ثابت hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    unsigned sub_length = مقدار_Format_2.get_len ();
    auto مقادیر_array = values.as_array ((unsigned) مقدار_شمار * sub_length);

    auto it =
    + hb_zip (این+coverage, hb_range ((unsigned) مقدار_شمار))
    | hb_filter (glyphset, hb_first)
    | hb_map_retains_sorting (single_pos_مقدار_mapper_t {glyph_map,
                                                       مقادیر_array,
                                                       sub_length})
    ;

    unsigned new_format = compute_effective_format (c->plan->source, it,
                                                    منطقی (c->plan->normalized_coords),
                                                    منطقی (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING),
                                                    c->plan->has_gdef_varstore,
                                                    &c->plan->layout_variation_idx_delta_map);
    منطقی ret = منطقی (it);
    SinglePos_serialize (c->serializer, این, مقدار_Format_2, it, &c->plan->layout_variation_idx_delta_map, new_format);
    return_trace (ret);
  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_SINGLEPOSFORMAT2_HH */
