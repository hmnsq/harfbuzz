#ifndef OT_LAYOUT_GPOS_SINGLEPOSFORMAT1_HH
#define OT_LAYOUT_GPOS_SINGLEPOSFORMAT1_HH

#অন্তর্ভাব "Common.hh"
#অন্তর্ভাব "ValueFormat.hh"

নামাকাশ OT {
নামাকাশ Layout {
নামাকাশ GPOS_impl {

template <typename Types>
কাঠামো SinglePosFormat1_3 : মান_Base
{
  সুরক্ষিত:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of subtable */
  মান_Format   মান_Format_2;            /* Defines the types of data in the
                                         * ValueRecord */
  মান_Record   values;                 /* Defines positioning
                                         * value(s)--applied to all glyphs in
                                         * the Coverage table */
  সর্বজনীন:
  DEFINE_আকার_ARRAY (2 + Types::LOffset::static_আকার + মান_Format::static_আকার, values);

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) &&
                  coverage.sanitize (c, এই) &&
		  hb_barrier () &&
                  /* The coverage  table may use a range to represent a set
                   * of glyphs, which means a small number of bytes can
                   * generate a large glyph set. Manually modify the
                   * sanitizer max ops to take this into account.
                   *
                   * Note: This check *must* be right after coverage sanitize. */
                  c->check_ops ((এই + coverage).get_population () >> 1) &&
                  মান_Format_2.sanitize_মান (c, এই, values));

  }

  যৌক্তিক intersects (ধ্রুবক hb_set_t *glyphs) ধ্রুবক
  { ফেরত (এই+coverage).intersects (glyphs); }

  শূন্য closure_lookups (hb_closure_lookups_context_t *c) ধ্রুবক {}
  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c) ধ্রুবক
  {
    যদি (!মান_Format_2.has_device ()) ফেরত;

    hb_set_t intersection;
    (এই+coverage).intersect_set (*c->glyph_set, intersection);
    যদি (!intersection) ফেরত;

    মান_Format_2.collect_variation_indices (c, এই, values.as_array (মান_Format_2.get_len ()));
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

    যদি (HB_বাফার_MESSAGE_MORE && c->বাফার->messaging ())
    {
      c->বাফার->message (c->font,
			  "positioning glyph at %u",
			  c->বাফার->idx);
    }

    মান_Format_2.apply_মান (c, এই, values, বাফার->cur_pos());

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

    মান_Format_2.position_single (font, table_blob, direction, এই,
                                 values, pos);
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
    যদি (unlikely (!c->extend_min (এই))) ফেরত;
    যদি (unlikely (!c->check_assign (মান_Format_2,
                                    newFormat,
                                    HB_SERIALIZE_ERROR_INT_OVERFLOW))) ফেরত;

    জন্য (ধ্রুবক hb_array_t<ধ্রুবক মান>& _ : + it | hb_map (hb_second))
    {
      src_format.copy_মানসমূহ (c, newFormat, src,  &_, layout_variation_idx_delta_map);
      // Only serialize the first entry in the iterator, the rest are assumed to
      // be the same.
      বিরতি;
    }

    auto glyphs =
    + it
    | hb_map_retains_sorting (hb_first)
    ;

    coverage.serialize_serialize (c, glyphs);
  }

  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    ধ্রুবক hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ধ্রুবক hb_map_t &glyph_map = *c->plan->glyph_map;

    hb_set_t intersection;
    (এই+coverage).intersect_set (glyphset, intersection);

    unsigned new_format = মান_Format_2;

    যদি (c->plan->normalized_coords)
    {
      new_format = মান_Format_2.get_effective_format (values.arrayZ, মিথ্যা, মিথ্যা, এই, &c->plan->layout_variation_idx_delta_map);
    }
    /* do not strip hints for VF */
    অন্যথা যদি (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
    {
      hb_blob_t* blob = hb_face_reference_table (c->plan->source, HB_TAG ('f','v','a','r'));
      যৌক্তিক has_fvar = (blob != hb_blob_get_empty ());
      hb_blob_destroy (blob);

      যৌক্তিক strip = !has_fvar;
      /* special case: strip hints when a VF has no GDEF varstore after
       * subsetting*/
      যদি (has_fvar && !c->plan->has_gdef_varstore)
        strip = সত্য;
      new_format = মান_Format_2.get_effective_format (values.arrayZ,
                                                     strip, /* strip hints */
                                                     সত্য, /* strip empty */
                                                     এই, nullptr);
    }

    auto it =
    + hb_iter (intersection)
    | hb_map_retains_sorting (glyph_map)
    | hb_zip (hb_repeat (values.as_array (মান_Format_2.get_len ())))
    ;

    যৌক্তিক ret = যৌক্তিক (it);
    SinglePos_serialize (c->serializer, এই, মান_Format_2, it, &c->plan->layout_variation_idx_delta_map, new_format);
    return_trace (ret);
  }
};

}
}
}

#endif /* OT_LAYOUT_GPOS_SINGLEPOSFORMAT1_HH */
