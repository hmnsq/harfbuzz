#अगर_अघोषित OT_LAYOUT_GPOS_SINGLEPOSFORMAT2_HH
#घोषणा OT_LAYOUT_GPOS_SINGLEPOSFORMAT2_HH

#समावेश "Common.hh"

namespace OT {
namespace Layout {
namespace GPOS_impl {

/* Keep the iterator type independent of the source table width so
 * the output serializers can share their instantiations. */
काष्ठा single_pos_मूल्य_mapper_t
{
  स्थिर hb_map_t &glyph_map;
  स्थिर hb_array_t<स्थिर मूल्य> &values;
  स्थिर अचिन्हित &sub_लंबाई;

  hb_pair_t<hb_codepoint_t, hb_array_t<स्थिर मूल्य>>
  चालक () (स्थिर hb_pair_t<hb_codepoint_t, अचिन्हित> &_) स्थिर
  {
    वापस hb_pair (glyph_map[_.first],
                    values.sub_array (_.second * sub_लंबाई, sub_लंबाई));
  }
};

ढाँचा <typename Types>
काष्ठा SinglePosFormat2_4 : मूल्य_Base
{
  रक्षित:
  HBUINT16      format;                 /* Format identifier--format = 2 */
  typename Types::ढाँचा LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of subtable */
  मूल्य_Format   मूल्य_Format_2;            /* Defines the types of data in the
                                         * ValueRecord */
  typename Types::HBUINT
                मूल्य_गिनती;             /* Number of ValueRecords */
  मूल्य_Record   values;                 /* Array of ValueRecords--positioning
                                         * values applied to glyphs */
  खुला:
  DEFINE_आकार_ARRAY (2 + Types::LOffset::static_आकार + मूल्य_Format::static_आकार + Types::HBUINT::static_आकार, values);

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) &&
                  coverage.sanitize (c, यह) &&
                  मूल्य_Format_2.sanitize_मूल्यावली (c, यह, values, मूल्य_गिनती));
  }

  bool intersects (स्थिर hb_set_t *glyphs) स्थिर
  { वापस (यह+coverage).intersects (glyphs); }

  व्योम closure_lookups (hb_closure_lookups_context_t *c) स्थिर {}
  व्योम collect_variation_indices (hb_collect_variation_indices_context_t *c) स्थिर
  {
    अगर (!मूल्य_Format_2.has_device ()) वापस;

    स्वतः it =
    + hb_zip (यह+coverage, hb_range ((अचिन्हित) मूल्य_गिनती))
    | hb_filter (c->glyph_set, hb_first)
    ;

    अगर (!it) वापस;

    अचिन्हित sub_लंबाई = मूल्य_Format_2.get_len ();
    स्थिर hb_array_t<स्थिर मूल्य> मूल्यावली_array = values.as_array (मूल्य_गिनती * sub_लंबाई);

    क्रम (अचिन्हित i : + it
                      | hb_map (hb_second))
      मूल्य_Format_2.collect_variation_indices (c, यह, मूल्यावली_array.sub_array (i * sub_लंबाई, sub_लंबाई));

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

    अगर (unlikely (सूचकांक_2 >= मूल्य_गिनती)) return_trace (false);

    अगर (HB_बफर_MESSAGE_MORE && c->बफर->messaging ())
    {
      c->बफर->message (c->font,
			  "positioning glyph at %u",
			  c->बफर->idx);
    }

    मूल्य_Format_2.apply_मूल्य (c, यह,
                             &values[सूचकांक_2 * मूल्य_Format_2.get_len ()],
                             बफर->cur_pos());

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
    अगर (unlikely (सूचकांक_2 >= मूल्य_गिनती)) वापस false;

    मूल्य_Format_2.position_single (font, table_blob, direction, यह,
                                 &values[सूचकांक_2 * मूल्य_Format_2.get_len ()], pos);
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
    स्वतः out = c->extend_min (यह);
    अगर (unlikely (!out)) वापस;
    अगर (unlikely (!c->check_assign (मूल्य_Format_2, newFormat, HB_SERIALIZE_ERROR_INT_OVERFLOW))) वापस;
    अगर (unlikely (!c->check_assign (मूल्य_गिनती, it.len (), HB_SERIALIZE_ERROR_ARRAY_OVERFLOW))) वापस;

    + it
    | hb_map (hb_second)
    | hb_apply ([&] (hb_array_t<स्थिर मूल्य> _)
    { src_format.copy_मूल्यावली (c, newFormat, src, &_, layout_variation_idx_delta_map); })
    ;

    स्वतः glyphs =
    + it
    | hb_map_retains_sorting (hb_first)
    ;

    coverage.serialize_serialize (c, glyphs);
  }

  ढाँचा<typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  अचिन्हित compute_effective_format (स्थिर hb_face_t *face,
                                     Iterator it,
                                     bool is_instancing, bool strip_hints,
                                     bool has_gdef_varstore,
                                     स्थिर hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णांक>> *varidx_delta_map) स्थिर
  {
    hb_blob_t* blob = hb_face_reference_table (face, HB_TAG ('f','v','a','r'));
    bool has_fvar = (blob != hb_blob_get_empty ());
    hb_blob_destroy (blob);

    अचिन्हित new_format = 0;
    अगर (is_instancing)
    {
      new_format = new_format | मूल्य_Format_2.get_effective_format (+ it | hb_map (hb_second), false, false, यह, varidx_delta_map);
    }
    /* do not strip hints for VF */
    अन्यथा अगर (strip_hints)
    {
      bool strip = !has_fvar;
      अगर (has_fvar && !has_gdef_varstore)
        strip = true;
      new_format = new_format | मूल्य_Format_2.get_effective_format (+ it | hb_map (hb_second), strip, true, यह, nullptr);
    }
    अन्यथा
      new_format = मूल्य_Format_2;

    वापस new_format;
  }

  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);
    स्थिर hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    स्थिर hb_map_t &glyph_map = *c->plan->glyph_map;

    अचिन्हित sub_लंबाई = मूल्य_Format_2.get_len ();
    स्वतः मूल्यावली_array = values.as_array ((अचिन्हित) मूल्य_गिनती * sub_लंबाई);

    स्वतः it =
    + hb_zip (यह+coverage, hb_range ((अचिन्हित) मूल्य_गिनती))
    | hb_filter (glyphset, hb_first)
    | hb_map_retains_sorting (single_pos_मूल्य_mapper_t {glyph_map,
                                                       मूल्यावली_array,
                                                       sub_लंबाई})
    ;

    अचिन्हित new_format = compute_effective_format (c->plan->source, it,
                                                    bool (c->plan->normalized_coords),
                                                    bool (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING),
                                                    c->plan->has_gdef_varstore,
                                                    &c->plan->layout_variation_idx_delta_map);
    bool ret = bool (it);
    SinglePos_serialize (c->serializer, यह, मूल्य_Format_2, it, &c->plan->layout_variation_idx_delta_map, new_format);
    return_trace (ret);
  }
};


}
}
}

#पूर्ण_अगर /* OT_LAYOUT_GPOS_SINGLEPOSFORMAT2_HH */
