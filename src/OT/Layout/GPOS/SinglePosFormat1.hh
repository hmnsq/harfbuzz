#अगर_अघोषित OT_LAYOUT_GPOS_SINGLEPOSFORMAT1_HH
#घोषणा OT_LAYOUT_GPOS_SINGLEPOSFORMAT1_HH

#समावेश "Common.hh"
#समावेश "ValueFormat.hh"

namespace OT {
namespace Layout {
namespace GPOS_impl {

ढाँचा <typename Types>
काष्ठा SinglePosFormat1_3 : मूल्य_Base
{
  रक्षित:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::ढाँचा LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of subtable */
  मूल्य_Format   मूल्य_Format_2;            /* Defines the types of data in the
                                         * ValueRecord */
  मूल्य_Record   values;                 /* Defines positioning
                                         * value(s)--applied to all glyphs in
                                         * the Coverage table */
  खुला:
  DEFINE_आकार_ARRAY (2 + Types::LOffset::static_आकार + मूल्य_Format::static_आकार, values);

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) &&
                  coverage.sanitize (c, यह) &&
		  hb_barrier () &&
                  /* The coverage  table may use a range to represent a set
                   * of glyphs, which means a small number of bytes can
                   * generate a large glyph set. Manually modify the
                   * sanitizer max ops to take this into account.
                   *
                   * Note: This check *must* be right after coverage sanitize. */
                  c->check_ops ((यह + coverage).get_population () >> 1) &&
                  मूल्य_Format_2.sanitize_मूल्य (c, यह, values));

  }

  bool intersects (स्थिर hb_set_t *glyphs) स्थिर
  { वापस (यह+coverage).intersects (glyphs); }

  व्योम closure_lookups (hb_closure_lookups_context_t *c) स्थिर {}
  व्योम collect_variation_indices (hb_collect_variation_indices_context_t *c) स्थिर
  {
    अगर (!मूल्य_Format_2.has_device ()) वापस;

    hb_set_t intersection;
    (यह+coverage).intersect_set (*c->glyph_set, intersection);
    अगर (!intersection) वापस;

    मूल्य_Format_2.collect_variation_indices (c, यह, values.as_array (मूल्य_Format_2.get_len ()));
  }

  व्योम collect_glyphs (hb_collect_glyphs_context_t *c) स्थिर
  { अगर (unlikely (!(यह+coverage).collect_coverage (c->निवेश))) वापस; }

  स्थिर Coverage &get_coverage () स्थिर { वापस यह+coverage; }

  मूल्य_Format get_मूल्य_format () स्थिर { वापस मूल्य_Format_2; }

  bool apply (hb_ot_apply_context_t *c) स्थिर
  {
    TRACE_APPLY (यह);
    hb_बफर_t *बफर = c->बफर;
    अचिन्हित पूर्णांक सूचकांक_2 = (यह+coverage).get_coverage  (बफर->cur().codepoint);
    अगर (सूचकांक_2 == NOT_COVERED) return_trace (false);

    अगर (HB_बफर_MESSAGE_MORE && c->बफर->messaging ())
    {
      c->बफर->message (c->font,
			  "positioning glyph at %u",
			  c->बफर->idx);
    }

    मूल्य_Format_2.apply_मूल्य (c, यह, values, बफर->cur_pos());

    अगर (HB_बफर_MESSAGE_MORE && c->बफर->messaging ())
    {
      c->बफर->message (c->font,
			  "positioned glyph at %u",
			  c->बफर->idx);
    }

    बफर->idx++;
    return_trace (true);
  }

  bool
  position_single (hb_font_t           *font,
		   hb_blob_t           *table_blob,
		   hb_direction_t       direction,
		   hb_codepoint_t       gid,
		   hb_glyph_position_t &pos) स्थिर
  {
    अचिन्हित पूर्णांक सूचकांक_2 = (यह+coverage).get_coverage  (gid);
    अगर (likely (सूचकांक_2 == NOT_COVERED)) वापस false;

    मूल्य_Format_2.position_single (font, table_blob, direction, यह,
                                 values, pos);
    वापस true;
  }

  ढाँचा<typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  व्योम serialize (hb_serialize_context_t *c,
                  स्थिर मूल्य_Base *src,
                  मूल्य_Format src_format,
                  Iterator it,
                  मूल्य_Format newFormat,
                  स्थिर hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णांक>> *layout_variation_idx_delta_map)
  {
    अगर (unlikely (!c->extend_min (यह))) वापस;
    अगर (unlikely (!c->check_assign (मूल्य_Format_2,
                                    newFormat,
                                    HB_SERIALIZE_ERROR_INT_OVERFLOW))) वापस;

    क्रम (स्थिर hb_array_t<स्थिर मूल्य>& _ : + it | hb_map (hb_second))
    {
      src_format.copy_मूल्यावली (c, newFormat, src,  &_, layout_variation_idx_delta_map);
      // Only serialize the first entry in the iterator, the rest are assumed to
      // be the same.
      अवरोध;
    }

    स्वतः glyphs =
    + it
    | hb_map_retains_sorting (hb_first)
    ;

    coverage.serialize_serialize (c, glyphs);
  }

  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);
    स्थिर hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    स्थिर hb_map_t &glyph_map = *c->plan->glyph_map;

    hb_set_t intersection;
    (यह+coverage).intersect_set (glyphset, intersection);

    अचिन्हित new_format = मूल्य_Format_2;

    अगर (c->plan->normalized_coords)
    {
      new_format = मूल्य_Format_2.get_effective_format (values.arrayZ, false, false, यह, &c->plan->layout_variation_idx_delta_map);
    }
    /* do not strip hints for VF */
    अन्यथा अगर (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
    {
      hb_blob_t* blob = hb_face_reference_table (c->plan->source, HB_TAG ('f','v','a','r'));
      bool has_fvar = (blob != hb_blob_get_empty ());
      hb_blob_destroy (blob);

      bool strip = !has_fvar;
      /* special case: strip hints when a VF has no GDEF varstore after
       * subsetting*/
      अगर (has_fvar && !c->plan->has_gdef_varstore)
        strip = true;
      new_format = मूल्य_Format_2.get_effective_format (values.arrayZ,
                                                     strip, /* strip hints */
                                                     true, /* strip empty */
                                                     यह, nullptr);
    }

    स्वतः it =
    + hb_iter (intersection)
    | hb_map_retains_sorting (glyph_map)
    | hb_zip (hb_repeat (values.as_array (मूल्य_Format_2.get_len ())))
    ;

    bool ret = bool (it);
    SinglePos_serialize (c->serializer, यह, मूल्य_Format_2, it, &c->plan->layout_variation_idx_delta_map, new_format);
    return_trace (ret);
  }
};

}
}
}

#पूर्ण_अगर /* OT_LAYOUT_GPOS_SINGLEPOSFORMAT1_HH */
