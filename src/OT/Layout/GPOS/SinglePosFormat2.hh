#ifndef OT_LAYOUT_GPOS_SINGLEPOSFORMAT2_HH
#define OT_LAYOUT_GPOS_SINGLEPOSFORMAT2_HH

#অন্তর্ভাব "Common.hh"

নামাকাশ OT {
নামাকাশ Layout {
নামাকাশ GPOS_impl {

/* Keep the iterator type independent of the source table width so
 * the output serializers can share their instantiations. */
কাঠামো single_pos_মান_mapper_t
{
  ধ্রুবক hb_map_t &glyph_map;
  ধ্রুবক hb_array_t<ধ্রুবক মান> &values;
  ধ্রুবক unsigned &sub_দৈর্ঘ্য;

  hb_pair_t<hb_codepoint_t, hb_array_t<ধ্রুবক মান>>
  operator () (ধ্রুবক hb_pair_t<hb_codepoint_t, unsigned> &_) ধ্রুবক
  {
    ফেরত hb_pair (glyph_map[_.first],
                    values.sub_array (_.second * sub_দৈর্ঘ্য, sub_দৈর্ঘ্য));
  }
};

template <typename Types>
কাঠামো SinglePosFormat2_4 : মান_Base
{
  সুরক্ষিত:
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
  সর্বজনীন:
  DEFINE_আকার_ARRAY (2 + Types::LOffset::static_আকার + মান_Format::static_আকার + Types::HBUINT::static_আকার, values);

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) &&
                  coverage.sanitize (c, এই) &&
                  মান_Format_2.sanitize_মানসমূহ (c, এই, values, মান_গুনতি));
  }

  যৌক্তিক intersects (ধ্রুবক hb_set_t *glyphs) ধ্রুবক
  { ফেরত (এই+coverage).intersects (glyphs); }

  শূন্য closure_lookups (hb_closure_lookups_context_t *c) ধ্রুবক {}
  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c) ধ্রুবক
  {
    যদি (!মান_Format_2.has_device ()) ফেরত;

    auto it =
    + hb_zip (এই+coverage, hb_range ((unsigned) মান_গুনতি))
    | hb_filter (c->glyph_set, hb_first)
    ;

    যদি (!it) ফেরত;

    unsigned sub_দৈর্ঘ্য = মান_Format_2.get_len ();
    ধ্রুবক hb_array_t<ধ্রুবক মান> মানসমূহ_array = values.as_array (মান_গুনতি * sub_দৈর্ঘ্য);

    জন্য (unsigned i : + it
                      | hb_map (hb_second))
      মান_Format_2.collect_variation_indices (c, এই, মানসমূহ_array.sub_array (i * sub_দৈর্ঘ্য, sub_দৈর্ঘ্য));

  }

  শূন্য collect_glyphs (hb_collect_glyphs_context_t *c) ধ্রুবক
  { যদি (unlikely (!(এই+coverage).collect_coverage (c->প্রবেশ))) ফেরত; }

  ধ্রুবক Coverage &get_coverage () ধ্রুবক { ফেরত এই+coverage; }

  মান_Format get_মান_format () ধ্রুবক { ফেরত মান_Format_2; }

  যৌক্তিক apply (hb_ot_apply_context_t *c) ধ্রুবক
  {
    TRACE_APPLY (এই);
    hb_বাফার_t *বাফার = c->বাফার;
    unsigned পূর্ণসংখ্যা সূচক_2 = (এই+coverage).get_coverage  (বাফার->cur().codepoint);
    যদি (সূচক_2 == NOT_COVERED) return_trace (মিথ্যা);

    যদি (unlikely (সূচক_2 >= মান_গুনতি)) return_trace (মিথ্যা);

    যদি (HB_বাফার_MESSAGE_MORE && c->বাফার->messaging ())
    {
      c->বাফার->message (c->font,
			  "positioning glyph at %u",
			  c->বাফার->idx);
    }

    মান_Format_2.apply_মান (c, এই,
                             &values[সূচক_2 * মান_Format_2.get_len ()],
                             বাফার->cur_pos());

    যদি (HB_বাফার_MESSAGE_MORE && c->বাফার->messaging ())
    {
      c->বাফার->message (c->font,
			  "positioned glyph at %u",
			  c->বাফার->idx);
    }

    বাফার->idx++;
    return_trace (সত্য);
  }

  যৌক্তিক
  position_single (hb_font_t           *font,
		   hb_blob_t           *table_blob,
		   hb_direction_t       direction,
		   hb_codepoint_t       gid,
		   hb_glyph_position_t &pos) ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা সূচক_2 = (এই+coverage).get_coverage  (gid);
    যদি (likely (সূচক_2 == NOT_COVERED)) ফেরত মিথ্যা;
    যদি (unlikely (সূচক_2 >= মান_গুনতি)) ফেরত মিথ্যা;

    মান_Format_2.position_single (font, table_blob, direction, এই,
                                 &values[সূচক_2 * মান_Format_2.get_len ()], pos);
    ফেরত সত্য;
  }


  template<typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  শূন্য serialize (hb_serialize_context_t *c,
                  ধ্রুবক মান_Base *src,
                  মান_Format src_format,
                  Iterator it,
                  মান_Format newFormat,
                  ধ্রুবক hb_hashmap_t<unsigned, hb_pair_t<unsigned, পূর্ণসংখ্যা>> *layout_variation_idx_delta_map)
  {
    auto out = c->extend_min (এই);
    যদি (unlikely (!out)) ফেরত;
    যদি (unlikely (!c->check_assign (মান_Format_2, newFormat, HB_SERIALIZE_ERROR_INT_OVERFLOW))) ফেরত;
    যদি (unlikely (!c->check_assign (মান_গুনতি, it.len (), HB_SERIALIZE_ERROR_ARRAY_OVERFLOW))) ফেরত;

    + it
    | hb_map (hb_second)
    | hb_apply ([&] (hb_array_t<ধ্রুবক মান> _)
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
  unsigned compute_effective_format (ধ্রুবক hb_face_t *face,
                                     Iterator it,
                                     যৌক্তিক is_instancing, যৌক্তিক strip_hints,
                                     যৌক্তিক has_gdef_varstore,
                                     ধ্রুবক hb_hashmap_t<unsigned, hb_pair_t<unsigned, পূর্ণসংখ্যা>> *varidx_delta_map) ধ্রুবক
  {
    hb_blob_t* blob = hb_face_reference_table (face, HB_TAG ('f','v','a','r'));
    যৌক্তিক has_fvar = (blob != hb_blob_get_empty ());
    hb_blob_destroy (blob);

    unsigned new_format = 0;
    যদি (is_instancing)
    {
      new_format = new_format | মান_Format_2.get_effective_format (+ it | hb_map (hb_second), মিথ্যা, মিথ্যা, এই, varidx_delta_map);
    }
    /* do not strip hints for VF */
    অন্যথা যদি (strip_hints)
    {
      যৌক্তিক strip = !has_fvar;
      যদি (has_fvar && !has_gdef_varstore)
        strip = সত্য;
      new_format = new_format | মান_Format_2.get_effective_format (+ it | hb_map (hb_second), strip, সত্য, এই, nullptr);
    }
    অন্যথা
      new_format = মান_Format_2;

    ফেরত new_format;
  }

  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    ধ্রুবক hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ধ্রুবক hb_map_t &glyph_map = *c->plan->glyph_map;

    unsigned sub_দৈর্ঘ্য = মান_Format_2.get_len ();
    auto মানসমূহ_array = values.as_array ((unsigned) মান_গুনতি * sub_দৈর্ঘ্য);

    auto it =
    + hb_zip (এই+coverage, hb_range ((unsigned) মান_গুনতি))
    | hb_filter (glyphset, hb_first)
    | hb_map_retains_sorting (single_pos_মান_mapper_t {glyph_map,
                                                       মানসমূহ_array,
                                                       sub_দৈর্ঘ্য})
    ;

    unsigned new_format = compute_effective_format (c->plan->source, it,
                                                    যৌক্তিক (c->plan->normalized_coords),
                                                    যৌক্তিক (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING),
                                                    c->plan->has_gdef_varstore,
                                                    &c->plan->layout_variation_idx_delta_map);
    যৌক্তিক ret = যৌক্তিক (it);
    SinglePos_serialize (c->serializer, এই, মান_Format_2, it, &c->plan->layout_variation_idx_delta_map, new_format);
    return_trace (ret);
  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_SINGLEPOSFORMAT2_HH */
