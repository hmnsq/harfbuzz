#ifndef OT_LAYOUT_GPOS_SINGLEPOSFORMAT2_HH
#define OT_LAYOUT_GPOS_SINGLEPOSFORMAT2_HH

#अन्तर्भाव "Common.hh"

नामाकाश OT {
नामाकाश Layout {
नामाकाश GPOS_impl {

/* Keep the iterator type independent of the source table width so
 * the output serializers can share their instantiations. */
संरचना single_pos_मान_mapper_t
{
  अचर hb_map_t &glyph_map;
  अचर hb_array_t<अचर मान> &values;
  अचर अचिन्हित &sub_length;

  hb_pair_t<hb_codepoint_t, hb_array_t<अचर मान>>
  चालक () (अचर hb_pair_t<hb_codepoint_t, अचिन्हित> &_) अचर
  {
    फर्काऊ hb_pair (glyph_map[_.first],
                    values.sub_array (_.second * sub_length, sub_length));
  }
};

ढाँचा <typename Types>
संरचना SinglePosFormat2_4 : मान_Base
{
  रक्षित:
  HBUINT16      format;                 /* Format identifier--format = 2 */
  typename Types::ढाँचा LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of subtable */
  मान_Format   मान_Format_2;            /* Defines the types of data in the
                                         * ValueRecord */
  typename Types::HBUINT
                मान_गन्ती;             /* Number of ValueRecords */
  मान_Record   values;                 /* Array of ValueRecords--positioning
                                         * values applied to glyphs */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + मान_Format::static_size + Types::HBUINT::static_size, values);

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
                  coverage.sanitize (c, एतत्) &&
                  मान_Format_2.sanitize_मानहरू (c, एतत्, values, मान_गन्ती));
  }

  तार्किक intersects (अचर hb_set_t *glyphs) अचर
  { फर्काऊ (एतत्+coverage).intersects (glyphs); }

  शून्य closure_lookups (hb_closure_lookups_context_t *c) अचर {}
  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) अचर
  {
    यदि (!मान_Format_2.has_device ()) फर्काऊ;

    स्वतः it =
    + hb_zip (एतत्+coverage, hb_range ((अचिन्हित) मान_गन्ती))
    | hb_filter (c->glyph_set, hb_first)
    ;

    यदि (!it) फर्काऊ;

    अचिन्हित sub_length = मान_Format_2.get_len ();
    अचर hb_array_t<अचर मान> मानहरू_array = values.as_array (मान_गन्ती * sub_length);

    लागि (अचिन्हित i : + it
                      | hb_map (hb_second))
      मान_Format_2.collect_variation_indices (c, एतत्, मानहरू_array.sub_array (i * sub_length, sub_length));

  }

  शून्य collect_glyphs (hb_collect_glyphs_context_t *c) अचर
  { यदि (unlikely (!(एतत्+coverage).collect_coverage (c->आगत))) फर्काऊ; }

  अचर Coverage &get_coverage () अचर { फर्काऊ एतत्+coverage; }

  मान_Format get_मान_format () अचर { फर्काऊ मान_Format_2; }

  तार्किक apply (hb_ot_apply_context_t *c) अचर
  {
    TRACE_APPLY (एतत्);
    hb_buffer_t *buffer = c->buffer;
    अचिन्हित पूर्णाङ्क index = (एतत्+coverage).get_coverage  (buffer->cur().codepoint);
    यदि (index == NOT_COVERED) return_trace (असत्य);

    यदि (unlikely (index >= मान_गन्ती)) return_trace (असत्य);

    यदि (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "positioning glyph at %u",
			  c->buffer->idx);
    }

    मान_Format_2.apply_मान (c, एतत्,
                             &values[index * मान_Format_2.get_len ()],
                             buffer->cur_pos());

    यदि (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
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
		   hb_glyph_position_t &pos) अचर
  {
    अचिन्हित पूर्णाङ्क index = (एतत्+coverage).get_coverage  (gid);
    यदि (likely (index == NOT_COVERED)) फर्काऊ असत्य;
    यदि (unlikely (index >= मान_गन्ती)) फर्काऊ असत्य;

    मान_Format_2.position_single (font, table_blob, direction, एतत्,
                                 &values[index * मान_Format_2.get_len ()], pos);
    फर्काऊ सत्य;
  }


  ढाँचा<typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  शून्य serialize (hb_serialize_context_t *c,
                  अचर मान_Base *src,
                  मान_Format src_format,
                  Iterator it,
                  मान_Format newFormat,
                  अचर hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णाङ्क>> *layout_variation_idx_delta_map)
  {
    स्वतः out = c->extend_min (एतत्);
    यदि (unlikely (!out)) फर्काऊ;
    यदि (unlikely (!c->check_assign (मान_Format_2, newFormat, HB_SERIALIZE_ERROR_INT_OVERFLOW))) फर्काऊ;
    यदि (unlikely (!c->check_assign (मान_गन्ती, it.len (), HB_SERIALIZE_ERROR_ARRAY_OVERFLOW))) फर्काऊ;

    + it
    | hb_map (hb_second)
    | hb_apply ([&] (hb_array_t<अचर मान> _)
    { src_format.copy_मानहरू (c, newFormat, src, &_, layout_variation_idx_delta_map); })
    ;

    स्वतः glyphs =
    + it
    | hb_map_retains_sorting (hb_first)
    ;

    coverage.serialize_serialize (c, glyphs);
  }

  ढाँचा<typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  अचिन्हित compute_effective_format (अचर hb_face_t *face,
                                     Iterator it,
                                     तार्किक is_instancing, तार्किक strip_hints,
                                     तार्किक has_gdef_varstore,
                                     अचर hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णाङ्क>> *varidx_delta_map) अचर
  {
    hb_blob_t* blob = hb_face_reference_table (face, HB_TAG ('f','v','a','r'));
    तार्किक has_fvar = (blob != hb_blob_get_empty ());
    hb_blob_destroy (blob);

    अचिन्हित new_format = 0;
    यदि (is_instancing)
    {
      new_format = new_format | मान_Format_2.get_effective_format (+ it | hb_map (hb_second), असत्य, असत्य, एतत्, varidx_delta_map);
    }
    /* do not strip hints for VF */
    अन्यथा यदि (strip_hints)
    {
      तार्किक strip = !has_fvar;
      यदि (has_fvar && !has_gdef_varstore)
        strip = सत्य;
      new_format = new_format | मान_Format_2.get_effective_format (+ it | hb_map (hb_second), strip, सत्य, एतत्, nullptr);
    }
    अन्यथा
      new_format = मान_Format_2;

    फर्काऊ new_format;
  }

  तार्किक subset (hb_subset_context_t *c) अचर
  {
    TRACE_SUBSET (एतत्);
    अचर hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    अचर hb_map_t &glyph_map = *c->plan->glyph_map;

    अचिन्हित sub_length = मान_Format_2.get_len ();
    स्वतः मानहरू_array = values.as_array ((अचिन्हित) मान_गन्ती * sub_length);

    स्वतः it =
    + hb_zip (एतत्+coverage, hb_range ((अचिन्हित) मान_गन्ती))
    | hb_filter (glyphset, hb_first)
    | hb_map_retains_sorting (single_pos_मान_mapper_t {glyph_map,
                                                       मानहरू_array,
                                                       sub_length})
    ;

    अचिन्हित new_format = compute_effective_format (c->plan->source, it,
                                                    तार्किक (c->plan->normalized_coords),
                                                    तार्किक (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING),
                                                    c->plan->has_gdef_varstore,
                                                    &c->plan->layout_variation_idx_delta_map);
    तार्किक ret = तार्किक (it);
    SinglePos_serialize (c->serializer, एतत्, मान_Format_2, it, &c->plan->layout_variation_idx_delta_map, new_format);
    return_trace (ret);
  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_SINGLEPOSFORMAT2_HH */
