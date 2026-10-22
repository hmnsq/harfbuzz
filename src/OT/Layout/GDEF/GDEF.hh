/*
 * Copyright © 2007,2008,2009  Red Hat, Inc.
 * Copyright © 2010,2011,2012  Google, Inc.
 *
 *  This is part of HarfBuzz, a text shaping library.
 *
 * Permission is hereby granted, without written agreement and without
 * license or royalty fees, to use, copy, modify, and distribute this
 * software and its documentation for any purpose, provided that the
 * above copyright notice and the following two paragraphs appear in
 * all copies of this software.
 *
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
 * ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN
 * IF THE COPYRIGHT HOLDER HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 * THE COPYRIGHT HOLDER SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING,
 * BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE COPYRIGHT HOLDER HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 *
 * Red Hat Author(s): Behdad Esfahbod
 * Google Author(s): Behdad Esfahbod
 */

#अगर_अघोषित OT_LAYOUT_GDEF_GDEF_HH
#घोषणा OT_LAYOUT_GDEF_GDEF_HH

#समावेश "../../../hb-ot-var-common.hh"

#समावेश "../../../hb-font.hh"
#समावेश "../../../hb-cache.hh"


namespace OT {


/*
 * Attachment List Table
 */

/* Array of contour point indices--in increasing numerical order */
काष्ठा AttachPoint : Array16Of<HBUINT16>
{
  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);
    स्वतः *out = c->serializer->start_embed (*यह);
    return_trace (out->serialize (c->serializer, + iter ()));
  }
};

/* AttachList is referenced by both GDEF attachListOffset and
 * attachListOffset2. The v1.4 top-level offset widens, but the
 * AttachList table itself keeps Offset16 internals. */
काष्ठा AttachList
{
  अचिन्हित पूर्णांक get_attach_points (hb_codepoint_t glyph_id,
				  अचिन्हित पूर्णांक start_offset,
				  अचिन्हित पूर्णांक *point_गिनती /* IN/OUT */,
				  अचिन्हित पूर्णांक *point_array /* OUT */) स्थिर
  {
    अचिन्हित पूर्णांक सूचकांक_2 = (यह+coverage).get_coverage (glyph_id);
    अगर (सूचकांक_2 == NOT_COVERED)
    {
      अगर (point_गिनती)
	*point_गिनती = 0;
      वापस 0;
    }

    स्थिर AttachPoint &points = यह+attachPoint[सूचकांक_2];

    अगर (point_गिनती && point_array)
    {
      + points.as_array ().sub_array (start_offset, point_गिनती)
      | hb_sink (hb_array (point_array, *point_गिनती))
      ;
    }

    वापस points.len;
  }

  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);
    स्थिर hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    स्थिर hb_map_t &glyph_map = *c->plan->glyph_map;

    स्वतः *out = c->serializer->start_embed (*यह);
    अगर (unlikely (!c->serializer->extend_min (out))) return_trace (false);

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (यह+coverage, attachPoint)
    | hb_filter (glyphset, hb_first)
    | hb_filter (subset_offset_array (c, out->attachPoint, यह), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;
    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (bool (new_coverage));
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (coverage.sanitize (c, यह) && attachPoint.sanitize (c, यह));
  }

  रक्षित:
  Offset16To<Coverage>
		coverage;		/* Offset to Coverage table -- from
					 * beginning of AttachList table */
  Array16OfOffset16To<AttachPoint>
		attachPoint;		/* Array of AttachPoint tables
					 * in Coverage Index order */
  खुला:
  DEFINE_आकार_ARRAY (4, attachPoint);
};

/*
 * Ligature Caret Table
 */

काष्ठा Caret_मूल्य_Format1
{
  मित्र काष्ठा Caret_मूल्य;
  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);
    स्वतः *out = c->serializer->embed (यह);
    अगर (unlikely (!out)) return_trace (false);
    return_trace (true);
  }

  निजी:
  hb_position_t get_caret_मूल्य (hb_font_t *font, hb_direction_t direction) स्थिर
  {
    वापस HB_DIRECTION_IS_HORIZONTAL (direction) ? font->em_scale_x (coordinate) : font->em_scale_y (coordinate);
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह));
  }

  रक्षित:
  HBUINT16	caret_मूल्य_Format;	/* Format identifier--format = 1 */
  FWORD		coordinate;		/* X or Y value, in design units */
  खुला:
  DEFINE_आकार_STATIC (4);
};

काष्ठा Caret_मूल्य_Format2
{
  मित्र काष्ठा Caret_मूल्य;
  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);
    स्वतः *out = c->serializer->embed (यह);
    अगर (unlikely (!out)) return_trace (false);
    return_trace (true);
  }

  निजी:
  hb_position_t get_caret_मूल्य (hb_font_t *font, hb_direction_t direction, hb_codepoint_t glyph_id) स्थिर
  {
    hb_position_t x, y;
    font->get_glyph_contour_point_for_origin (glyph_id, caret_मूल्य_Point, direction, &x, &y);
    वापस HB_DIRECTION_IS_HORIZONTAL (direction) ? x : y;
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह));
  }

  रक्षित:
  HBUINT16	caret_मूल्य_Format;	/* Format identifier--format = 2 */
  HBUINT16	caret_मूल्य_Point;	/* Contour point index on glyph */
  खुला:
  DEFINE_आकार_STATIC (4);
};

काष्ठा Caret_मूल्य_Format3
{
  मित्र काष्ठा Caret_मूल्य;

  hb_position_t get_caret_मूल्य (hb_font_t *font, hb_direction_t direction,
				 स्थिर ItemVariationStore &var_store) स्थिर
  {
    वापस HB_DIRECTION_IS_HORIZONTAL (direction) ?
	   hb_saturate_add (font->em_scale_x (coordinate), (यह+deviceTable).get_x_delta (font, var_store)) :
	   hb_saturate_add (font->em_scale_y (coordinate), (यह+deviceTable).get_y_delta (font, var_store));
  }

  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);
    स्वतः *out = c->serializer->start_embed (*यह);
    अगर (!c->serializer->embed (caret_मूल्य_Format)) return_trace (false);
    अगर (!c->serializer->embed (coordinate)) return_trace (false);

    अचिन्हित varidx = (यह+deviceTable).get_variation_सूचकांक ();
    hb_pair_t<अचिन्हित, पूर्णांक> *new_varidx_delta;
    अगर (c->plan->layout_variation_idx_delta_map.has (varidx, &new_varidx_delta)) {
      uint32_t new_varidx = hb_first (*new_varidx_delta);
      पूर्णांक delta = hb_second (*new_varidx_delta);
      अगर (delta != 0)
      {
        अगर (!c->serializer->check_assign (out->coordinate, coordinate + delta, HB_SERIALIZE_ERROR_INT_OVERFLOW))
          return_trace (false);
      }

      अगर (new_varidx == HB_OT_LAYOUT_NO_VARIATIONS_सूचकांक)
        return_trace (c->serializer->check_assign (out->caret_मूल्य_Format, 1, HB_SERIALIZE_ERROR_INT_OVERFLOW));
    }

    अगर (!c->serializer->embed (deviceTable))
      return_trace (false);

    return_trace (out->deviceTable.serialize_copy (c->serializer, deviceTable, यह, c->serializer->to_bias (out),
						   hb_serialize_context_t::शीर्ष, &c->plan->layout_variation_idx_delta_map));
  }

  व्योम collect_variation_indices (hb_collect_variation_indices_context_t *c) स्थिर
  { (यह+deviceTable).collect_variation_indices (c); }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) && deviceTable.sanitize (c, यह));
  }

  रक्षित:
  HBUINT16	caret_मूल्य_Format;	/* Format identifier--format = 3 */
  FWORD		coordinate;		/* X or Y value, in design units */
  Offset16To<Device>
		deviceTable;		/* Offset to Device table for X or Y
					 * value--from beginning of CaretValue
					 * table */
  खुला:
  DEFINE_आकार_STATIC (6);
};

काष्ठा Caret_मूल्य
{
  hb_position_t get_caret_मूल्य (hb_font_t *font,
				 hb_direction_t direction,
				 hb_codepoint_t glyph_id,
				 स्थिर ItemVariationStore &var_store) स्थिर
  {
    चयन (u.format.v) {
    हाल 1: hb_barrier (); वापस u.format1.get_caret_मूल्य (font, direction);
    हाल 2: hb_barrier (); वापस u.format2.get_caret_मूल्य (font, direction, glyph_id);
    हाल 3: hb_barrier (); वापस u.format3.get_caret_मूल्य (font, direction, var_store);
    शेष:वापस 0;
    }
  }

  ढाँचा <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) स्थिर
  {
    अगर (unlikely (!c->may_dispatch (यह, &u.format.v))) वापस c->no_dispatch_return_मूल्य ();
    TRACE_DISPATCH (यह, u.format.v);
    चयन (u.format.v) {
    हाल 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    हाल 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    हाल 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    शेष:return_trace (c->default_return_मूल्य ());
    }
  }

  व्योम collect_variation_indices (hb_collect_variation_indices_context_t *c) स्थिर
  {
    चयन (u.format.v) {
    हाल 1:
    हाल 2:
      वापस;
    हाल 3:
      hb_barrier ();
      u.format3.collect_variation_indices (c);
      वापस;
    शेष: वापस;
    }
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    अगर (!u.format.v.sanitize (c)) return_trace (false);
    hb_barrier ();
    चयन (u.format.v) {
    हाल 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    हाल 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    हाल 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    शेष:return_trace (true);
    }
  }

  रक्षित:
  जोड़ {
  काष्ठा { HBUINT16 v; }	format;		/* Format identifier */
  Caret_मूल्य_Format1	format1;
  Caret_मूल्य_Format2	format2;
  Caret_मूल्य_Format3	format3;
  } u;
  खुला:
  DEFINE_आकार_UNION (2, format.v);
};

काष्ठा LigGlyph
{
  अचिन्हित get_lig_carets (hb_font_t            *font,
			   hb_direction_t        direction,
			   hb_codepoint_t        glyph_id,
			   स्थिर ItemVariationStore &var_store,
			   अचिन्हित              start_offset,
			   अचिन्हित             *caret_गिनती /* IN/OUT */,
			   hb_position_t        *caret_array /* OUT */) स्थिर
  {
    अगर (caret_गिनती && caret_array)
    {
      + carets.as_array ().sub_array (start_offset, caret_गिनती)
      | hb_map (hb_add (यह))
      | hb_map ([&] (स्थिर Caret_मूल्य &value) { वापस value.get_caret_मूल्य (font, direction, glyph_id, var_store); })
      | hb_sink (hb_array (caret_array, *caret_गिनती))
      ;
    }

    वापस carets.len;
  }

  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);
    स्वतः *out = c->serializer->start_embed (*यह);
    अगर (unlikely (!c->serializer->extend_min (out))) return_trace (false);

    + hb_iter (carets)
    | hb_apply (subset_offset_array (c, out->carets, यह))
    ;

    return_trace (bool (out->carets));
  }

  व्योम collect_variation_indices (hb_collect_variation_indices_context_t *c) स्थिर
  {
    क्रम (स्थिर Offset16To<Caret_मूल्य>& offset : carets.iter ())
      (यह+offset).collect_variation_indices (c);
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (carets.sanitize (c, यह));
  }

  रक्षित:
  Array16OfOffset16To<Caret_मूल्य>
		carets;			/* Offset array of CaretValue tables
					 * --from beginning of LigGlyph table
					 * --in increasing coordinate order */
  खुला:
  DEFINE_आकार_ARRAY (2, carets);
};

/* LigCaretList2 widens the LigCaretList coverage, count, and LigGlyph
 * offsets. The LigGlyph table itself keeps Offset16 caretValueOffsets. */
ढाँचा <typename Types>
काष्ठा LigCaretList
{
  अचिन्हित पूर्णांक get_lig_carets (hb_font_t *font,
			       hb_direction_t direction,
			       hb_codepoint_t glyph_id,
			       स्थिर ItemVariationStore &var_store,
			       अचिन्हित पूर्णांक start_offset,
			       अचिन्हित पूर्णांक *caret_गिनती /* IN/OUT */,
			       hb_position_t *caret_array /* OUT */) स्थिर
  {
    अचिन्हित पूर्णांक सूचकांक_2 = (यह+coverage).get_coverage (glyph_id);
    अगर (सूचकांक_2 == NOT_COVERED)
    {
      अगर (caret_गिनती)
	*caret_गिनती = 0;
      वापस 0;
    }
    स्थिर LigGlyph &lig_glyph = यह+ligGlyph[सूचकांक_2];
    वापस lig_glyph.get_lig_carets (font, direction, glyph_id, var_store, start_offset, caret_गिनती, caret_array);
  }

  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);
    स्थिर hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    स्थिर hb_map_t &glyph_map = *c->plan->glyph_map;

    स्वतः *out = c->serializer->start_embed (*यह);
    अगर (unlikely (!c->serializer->extend_min (out))) return_trace (false);

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (यह+coverage, ligGlyph)
    | hb_filter (glyphset, hb_first)
    | hb_filter (subset_offset_array (c, out->ligGlyph, यह), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;
    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (bool (new_coverage));
  }

  व्योम collect_variation_indices (hb_collect_variation_indices_context_t *c) स्थिर
  {
    + hb_zip (यह+coverage, ligGlyph)
    | hb_filter (c->glyph_set, hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (यह))
    | hb_apply ([c] (स्थिर LigGlyph& _) { _.collect_variation_indices (c); })
    ;
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (coverage.sanitize (c, यह) && ligGlyph.sanitize (c, यह));
  }

  रक्षित:
  typename Types::ढाँचा LOffsetTo<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of LigCaretList table */
  typename Types::ढाँचा ArrayOf<typename Types::ढाँचा OffsetTo<LigGlyph>>
		ligGlyph;		/* Array of LigGlyph tables
					 * in Coverage Index order */
  खुला:
  DEFINE_आकार_ARRAY (Types::LOffset::static_आकार + Types::HBUINT::static_आकार, ligGlyph);
};

#अगर_अघोषित HB_NO_BEYOND_64K
static_assert (LigCaretList<MediumTypes>::min_आकार ==
	       Offset32::static_आकार + HBUINT24::static_आकार, "");
#पूर्ण_अगर


काष्ठा MarkGlyphSetsFormat1
{
  bool covers (अचिन्हित पूर्णांक set_सूचकांक, hb_codepoint_t glyph_id) स्थिर
  { वापस (यह+coverage[set_सूचकांक]).get_coverage (glyph_id) != NOT_COVERED; }

  व्योम collect_used_mark_sets (स्थिर hb_set_t& glyph_set,
                               hb_set_t& used_mark_sets /* OUT */) स्थिर
  {
    अचिन्हित i = 0;
    क्रम (स्थिर स्वतः &offset : coverage)
     {
       स्थिर स्वतः &cov = यह+offset;
       अगर (cov.intersects (&glyph_set))
         used_mark_sets.add (i);

       i++;
     }
  }

  ढाँचा <typename set_t>
  व्योम collect_coverage (hb_vector_t<set_t> &sets) स्थिर
  {
     क्रम (स्थिर स्वतः &offset : coverage)
     {
       स्थिर स्वतः &cov = यह+offset;
       cov.collect_coverage (sets.push ());
     }
  }

  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);
    स्वतः *out = c->serializer->start_embed (*यह);
    अगर (unlikely (!c->serializer->extend_min (out))) return_trace (false);
    out->format = format;

    bool ret = true;
    क्रम (स्थिर Offset32To<Coverage>& offset : coverage.iter ())
    {
      स्वतः snap = c->serializer->snapshot ();
      स्वतः *o = out->coverage.serialize_append (c->serializer);
      अगर (unlikely (!o))
      {
	ret = false;
	अवरोध;
      }

      //skip empty coverage
      c->serializer->push ();
      bool res = false;
      अगर (offset) res = c->dispatch (यह+offset);
      अगर (!res)
      {
        c->serializer->निकालो_discard ();
        c->serializer->revert (snap);
        (out->coverage.len)--;
        जारी;
      }
      c->serializer->add_link (*o, c->serializer->निकालो_pack ());
    }

    return_trace (ret && out->coverage.len);
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (coverage.sanitize (c, यह));
  }

  रक्षित:
  HBUINT16	format;			/* Format identifier--format = 1 */
  Array16Of<Offset32To<Coverage>>
		coverage;		/* Array of long offsets to mark set
					 * coverage tables */
  खुला:
  DEFINE_आकार_ARRAY (4, coverage);
};

काष्ठा MarkGlyphSets
{
  bool covers (अचिन्हित पूर्णांक set_सूचकांक, hb_codepoint_t glyph_id) स्थिर
  {
    चयन (u.format.v) {
    हाल 1: hb_barrier (); वापस u.format1.covers (set_सूचकांक, glyph_id);
    शेष:वापस false;
    }
  }

  ढाँचा <typename set_t>
  व्योम collect_coverage (hb_vector_t<set_t> &sets) स्थिर
  {
    चयन (u.format.v) {
    हाल 1: hb_barrier (); u.format1.collect_coverage (sets); वापस;
    शेष:वापस;
    }
  }

  व्योम collect_used_mark_sets (स्थिर hb_set_t& glyph_set,
                               hb_set_t& used_mark_sets /* OUT */) स्थिर
  {
    चयन (u.format.v) {
    हाल 1: hb_barrier (); u.format1.collect_used_mark_sets (glyph_set, used_mark_sets); वापस;
    शेष:वापस;
    }
  }

  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);
    चयन (u.format.v) {
    हाल 1: hb_barrier (); return_trace (u.format1.subset (c));
    शेष:return_trace (false);
    }
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    अगर (!u.format.v.sanitize (c)) return_trace (false);
    hb_barrier ();
    चयन (u.format.v) {
    हाल 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    शेष:return_trace (true);
    }
  }

  रक्षित:
  जोड़ {
  काष्ठा { HBUINT16 v; }	format;		/* Format identifier */
  MarkGlyphSetsFormat1	format1;
  } u;
  खुला:
  DEFINE_आकार_UNION (2, format.v);
};


/*
 * GDEF -- Glyph Definition
 * https://docs.microsoft.com/en-us/typography/opentype/spec/gdef
 */


काष्ठा GDEF
{
  अटल constexpr hb_tag_t tableTag = HB_OT_TAG_GDEF;

  क्रमागत GlyphClasses {
    UnclassifiedGlyph	= 0,
    BaseGlyph		= 1,
    LigatureGlyph	= 2,
    MarkGlyph		= 3,
    ComponentGlyph	= 4
  };

  माप_प्रकार get_आकार () स्थिर
  {
    अगर (version.major != 1) वापस version.static_आकार;

    वापस min_आकार +
	   (version.to_int () >= 0x00010002u ? markGlyphSetsDef.static_आकार : 0) +
	   (version.to_int () >= 0x00010003u ? varStore.static_आकार : 0) +
#अगर_अघोषित HB_NO_BEYOND_64K
	   (version.to_int () >= 0x00010004u ? 5 * Offset32::static_आकार : 0)
#अन्यथा
	   0
#पूर्ण_अगर
	   ;
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    अगर (unlikely (!version.sanitize (c) ||
		  version.major != 1 ||
		  !c->check_range (यह, get_आकार ())))
      return_trace (false);
    hb_barrier ();

    bool glyph_class_def_ok = glyphClassDef.sanitize (c, यह);
    bool attach_list_ok = attachList.sanitize (c, यह);
    bool lig_caret_list_ok = ligCaretList.sanitize (c, यह);
    bool mark_attach_class_def_ok = markAttachClassDef.sanitize (c, यह);
    bool mark_glyph_sets_def_ok = version.to_int () < 0x00010002u ||
				  markGlyphSetsDef.sanitize (c, यह);

#अगर_अघोषित HB_NO_BEYOND_64K
    अगर (version.to_int () >= 0x00010004u)
    {
      अगर (glyphClassDef2) glyph_class_def_ok = glyphClassDef2.sanitize (c, यह);
      अगर (attachList2) attach_list_ok = attachList2.sanitize (c, यह);
      अगर (ligCaretList2) lig_caret_list_ok = ligCaretList2.sanitize (c, यह);
      अगर (markAttachClassDef2) mark_attach_class_def_ok = markAttachClassDef2.sanitize (c, यह);
      अगर (markGlyphSetsDef2) mark_glyph_sets_def_ok = markGlyphSetsDef2.sanitize (c, यह);
    }
#पूर्ण_अगर

    return_trace (glyph_class_def_ok &&
		  attach_list_ok &&
		  lig_caret_list_ok &&
		  mark_attach_class_def_ok &&
		  mark_glyph_sets_def_ok &&
		  (version.to_int () < 0x00010003u || varStore.sanitize (c, यह)));
  }

  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);
    अगर (version.major != 1) return_trace (false);

    स्वतः *out = c->serializer->start_embed (*यह);
    अगर (unlikely (!c->serializer->extend_min (out))) return_trace (false);

    // Push var store first (if it's needed) so that it's last in the
    // serialization order. Some font consumers assume that varstore runs to
    // the end of the GDEF table.
    // See: https://github.com/harfbuzz/harfbuzz/issues/4636
    स्वतः snapshot_version0 = c->serializer->snapshot ();
    अगर (unlikely (version.to_int () >= 0x00010002u && hb_barrier () &&
		  !c->serializer->embed (markGlyphSetsDef)))
      return_trace (false);

    स्वतः snapshot_version2 = c->serializer->snapshot ();
    अगर (unlikely (version.to_int () >= 0x00010003u && hb_barrier () &&
		  !c->serializer->embed (varStore)))
      return_trace (false);

    bool use_v14 = false;
#अगर_अघोषित HB_NO_BEYOND_64K
    use_v14 = version.to_int () >= 0x00010004u &&
	      (glyphClassDef2 || attachList2 || ligCaretList2 ||
	       markAttachClassDef2 || markGlyphSetsDef2);
    अगर (unlikely (version.to_int () >= 0x00010004u && hb_barrier () &&
		  !c->serializer->extend_आकार (out, get_आकार ())))
      return_trace (false);
#पूर्ण_अगर

    bool subset_varstore = false;
    अचिन्हित varstore_सूचकांक = (अचिन्हित) -1;
    अगर (version.to_int () >= 0x00010003u && hb_barrier ())
    {
      अगर (c->plan->all_axes_pinned)
	out->varStore = 0;
      अन्यथा अगर (c->plan->normalized_coords)
      {
	अगर (varStore)
	{
	  item_variations_t item_vars;
	  अगर (unlikely (!c->serializer->check_success (
                  item_vars.instantiate (यह+varStore, c->plan, true, true,
                                          c->plan->gdef_varstore_inner_maps.as_array (), true))))
            return_trace (false);
	  subset_varstore = out->varStore.serialize_serialize (c->serializer,
							     item_vars.has_long_word (),
							     c->plan->axis_tags,
							     item_vars.get_region_list (),
							     item_vars.get_vardata_encodings ());
	  varstore_सूचकांक = c->serializer->last_added_child_सूचकांक();
	  remap_varidx_after_instantiation (item_vars.get_varidx_map (),
					    c->plan->layout_variation_idx_delta_map);
          remap_varidx_after_instantiation (item_vars.get_varidx_map (),
                                            c->plan->layout_condition_idx_मूल्य_map);
	}
      }
      अन्यथा
      {
	subset_varstore = out->varStore.serialize_subset (c, varStore, यह,
							  c->plan->gdef_varstore_inner_maps.as_array ());
	varstore_सूचकांक = c->serializer->last_added_child_सूचकांक();
      }
    }

    out->version.major = version.major;
    out->version.minor = version.minor;

    अगर (!subset_varstore && version.to_int () >= 0x00010002u && !use_v14) {
      c->serializer->revert (snapshot_version2);
    }
    अगर (!subset_varstore)
      out->varStore = 0;
    अन्यथा
      c->plan->has_gdef_varstore = true;

    bool subset_markglyphsetsdef = false;
    अगर (version.to_int () >= 0x00010002u && hb_barrier ())
    {
#अगर_अघोषित HB_NO_BEYOND_64K
      अगर (use_v14 && markGlyphSetsDef2)
      {
	out->markGlyphSetsDef = 0;
	subset_markglyphsetsdef = serialize_subset_offset (c, out->markGlyphSetsDef2,
							   markGlyphSetsDef2, यह);
      }
      अन्यथा
#पूर्ण_अगर
      {
#अगर_अघोषित HB_NO_BEYOND_64K
	अगर (use_v14) out->markGlyphSetsDef2 = 0;
#पूर्ण_अगर
	subset_markglyphsetsdef = out->markGlyphSetsDef.serialize_subset (c, markGlyphSetsDef, यह);
      }
    }

    अगर (use_v14)
    {
      out->version.minor = 4;
    } अन्यथा अगर (subset_varstore) {
      out->version.minor = 3;
    } अन्यथा अगर (subset_markglyphsetsdef) {
      out->version.minor = 2;
    } अन्यथा  {
      out->version.minor = 0;
      c->serializer->revert (snapshot_version0);
    }

    bool subset_glyphclassdef = false;
    bool subset_attachlist = false;
    bool subset_markattachclassdef = false;
    bool subset_ligcaretlist = false;

#अगर_अघोषित HB_NO_BEYOND_64K
    अगर (use_v14 && glyphClassDef2)
    {
      out->glyphClassDef = 0;
      subset_glyphclassdef = serialize_subset_offset (c, out->glyphClassDef2,
						      glyphClassDef2, यह, nullptr, false, true);
    }
    अन्यथा
#पूर्ण_अगर
    {
#अगर_अघोषित HB_NO_BEYOND_64K
      अगर (use_v14) out->glyphClassDef2 = 0;
#पूर्ण_अगर
      subset_glyphclassdef = out->glyphClassDef.serialize_subset (c, glyphClassDef, यह, nullptr, false, true);
    }

#अगर_अघोषित HB_NO_BEYOND_64K
    अगर (use_v14 && attachList2)
    {
      out->attachList = 0;
      subset_attachlist = serialize_subset_offset (c, out->attachList2, attachList2, यह);
    }
    अन्यथा
#पूर्ण_अगर
    {
#अगर_अघोषित HB_NO_BEYOND_64K
      अगर (use_v14) out->attachList2 = 0;
#पूर्ण_अगर
      subset_attachlist = out->attachList.serialize_subset (c, attachList, यह);
    }

#अगर_अघोषित HB_NO_BEYOND_64K
    अगर (use_v14 && markAttachClassDef2)
    {
      out->markAttachClassDef = 0;
      subset_markattachclassdef = serialize_subset_offset (c, out->markAttachClassDef2,
							   markAttachClassDef2, यह, nullptr, false, true);
    }
    अन्यथा
#पूर्ण_अगर
    {
#अगर_अघोषित HB_NO_BEYOND_64K
      अगर (use_v14) out->markAttachClassDef2 = 0;
#पूर्ण_अगर
      subset_markattachclassdef = out->markAttachClassDef.serialize_subset (c, markAttachClassDef,
									   यह, nullptr, false, true);
    }

#अगर_अघोषित HB_NO_BEYOND_64K
    अगर (use_v14 && ligCaretList2)
    {
      out->ligCaretList = 0;
      subset_ligcaretlist = serialize_subset_offset (c, out->ligCaretList2, ligCaretList2, यह);
    }
    अन्यथा
#पूर्ण_अगर
    {
#अगर_अघोषित HB_NO_BEYOND_64K
      अगर (use_v14) out->ligCaretList2 = 0;
#पूर्ण_अगर
      subset_ligcaretlist = out->ligCaretList.serialize_subset (c, ligCaretList, यह);
    }

    अगर (subset_varstore && varstore_सूचकांक != (अचिन्हित) -1) {
      c->serializer->repack_last(varstore_सूचकांक);
    }

    return_trace (subset_glyphclassdef || subset_attachlist ||
		  subset_ligcaretlist || subset_markattachclassdef ||
		  (out->version.to_int () >= 0x00010002u && subset_markglyphsetsdef) ||
		  (out->version.to_int () >= 0x00010003u && subset_varstore));
  }

  bool has_glyph_classes () स्थिर
  {
#अगर_अघोषित HB_NO_BEYOND_64K
    अगर (version.to_int () >= 0x00010004u && glyphClassDef2) वापस true;
#पूर्ण_अगर
    वापस glyphClassDef != 0;
  }
  स्थिर ClassDef &get_glyph_class_def () स्थिर
  {
#अगर_अघोषित HB_NO_BEYOND_64K
    अगर (version.to_int () >= 0x00010004u && glyphClassDef2) वापस यह+glyphClassDef2;
#पूर्ण_अगर
    वापस यह+glyphClassDef;
  }
  bool has_attach_list () स्थिर
  {
#अगर_अघोषित HB_NO_BEYOND_64K
    अगर (version.to_int () >= 0x00010004u && attachList2) वापस true;
#पूर्ण_अगर
    वापस attachList != 0;
  }
  स्थिर AttachList &get_attach_list () स्थिर
  {
#अगर_अघोषित HB_NO_BEYOND_64K
    अगर (version.to_int () >= 0x00010004u && attachList2) वापस यह+attachList2;
#पूर्ण_अगर
    वापस यह+attachList;
  }
  bool has_lig_carets () स्थिर
  {
#अगर_अघोषित HB_NO_BEYOND_64K
    अगर (version.to_int () >= 0x00010004u && ligCaretList2) वापस true;
#पूर्ण_अगर
    वापस ligCaretList != 0;
  }
  bool has_mark_attachment_types () स्थिर
  {
#अगर_अघोषित HB_NO_BEYOND_64K
    अगर (version.to_int () >= 0x00010004u && markAttachClassDef2) वापस true;
#पूर्ण_अगर
    वापस markAttachClassDef != 0;
  }
  स्थिर ClassDef &get_mark_attach_class_def () स्थिर
  {
#अगर_अघोषित HB_NO_BEYOND_64K
    अगर (version.to_int () >= 0x00010004u && markAttachClassDef2) वापस यह+markAttachClassDef2;
#पूर्ण_अगर
    वापस यह+markAttachClassDef;
  }
  bool has_mark_glyph_sets () स्थिर
  {
    अगर (version.to_int () < 0x00010002u) वापस false;
#अगर_अघोषित HB_NO_BEYOND_64K
    अगर (version.to_int () >= 0x00010004u && markGlyphSetsDef2) वापस true;
#पूर्ण_अगर
    वापस markGlyphSetsDef != 0;
  }
  स्थिर MarkGlyphSets &get_mark_glyph_sets () स्थिर
  {
    अगर (version.to_int () < 0x00010002u) वापस Null(MarkGlyphSets);
#अगर_अघोषित HB_NO_BEYOND_64K
    अगर (version.to_int () >= 0x00010004u && markGlyphSetsDef2) वापस यह+markGlyphSetsDef2;
#पूर्ण_अगर
    वापस यह+markGlyphSetsDef;
  }
  bool has_var_store () स्थिर
  {
    वापस version.to_int () >= 0x00010003u && hb_barrier () && varStore != 0;
  }
  स्थिर ItemVariationStore &get_var_store () स्थिर
  {
    वापस version.to_int () >= 0x00010003u && hb_barrier () ? यह+varStore : Null(ItemVariationStore);
  }


  bool has_तथ्य () स्थिर { वापस version.to_int (); }
  अचिन्हित पूर्णांक get_glyph_class (hb_codepoint_t glyph) स्थिर
  { वापस get_glyph_class_def ().get_class (glyph); }
  व्योम get_glyphs_in_class (अचिन्हित पूर्णांक klass, hb_set_t *glyphs) स्थिर
  { get_glyph_class_def ().collect_class (glyphs, klass); }

  अचिन्हित पूर्णांक get_mark_attachment_type (hb_codepoint_t glyph) स्थिर
  { वापस get_mark_attach_class_def ().get_class (glyph); }

  अचिन्हित पूर्णांक get_attach_points (hb_codepoint_t glyph_id,
				  अचिन्हित पूर्णांक start_offset,
				  अचिन्हित पूर्णांक *point_गिनती /* IN/OUT */,
				  अचिन्हित पूर्णांक *point_array /* OUT */) स्थिर
  { वापस get_attach_list ().get_attach_points (glyph_id, start_offset, point_गिनती, point_array); }

  अचिन्हित पूर्णांक get_lig_carets (hb_font_t *font,
			       hb_direction_t direction,
			       hb_codepoint_t glyph_id,
			       अचिन्हित पूर्णांक start_offset,
			       अचिन्हित पूर्णांक *caret_गिनती /* IN/OUT */,
			       hb_position_t *caret_array /* OUT */) स्थिर
  {
#अगर_अघोषित HB_NO_BEYOND_64K
    अगर (version.to_int () >= 0x00010004u && ligCaretList2)
      वापस (यह+ligCaretList2).get_lig_carets (font, direction, glyph_id, get_var_store (),
						  start_offset, caret_गिनती, caret_array);
#पूर्ण_अगर
    वापस (यह+ligCaretList).get_lig_carets (font, direction, glyph_id, get_var_store (),
					       start_offset, caret_गिनती, caret_array);
  }

  bool mark_set_covers (अचिन्हित पूर्णांक set_सूचकांक, hb_codepoint_t glyph_id) स्थिर
  { वापस get_mark_glyph_sets ().covers (set_सूचकांक, glyph_id); }

  /* glyph_props is a 16-bit integer where the lower 8-bit have bits representing
   * glyph class and other bits, and high 8-bit the mark attachment type (if any).
   * Not to be confused with lookup_props which is very similar. */
  अचिन्हित पूर्णांक get_glyph_props (hb_codepoint_t glyph) स्थिर
  {
    अचिन्हित पूर्णांक klass = get_glyph_class (glyph);

    static_assert (((अचिन्हित पूर्णांक) HB_OT_LAYOUT_GLYPH_PROPS_BASE_GLYPH == (अचिन्हित पूर्णांक) LookupFlag::IgnoreBaseGlyphs), "");
    static_assert (((अचिन्हित पूर्णांक) HB_OT_LAYOUT_GLYPH_PROPS_LIGATURE == (अचिन्हित पूर्णांक) LookupFlag::IgnoreLigatures), "");
    static_assert (((अचिन्हित पूर्णांक) HB_OT_LAYOUT_GLYPH_PROPS_MARK == (अचिन्हित पूर्णांक) LookupFlag::IgnoreMarks), "");

    चयन (klass) {
    शेष:			वापस HB_OT_LAYOUT_GLYPH_CLASS_UNCLASSIFIED;
    हाल BaseGlyph:		वापस HB_OT_LAYOUT_GLYPH_PROPS_BASE_GLYPH;
    हाल LigatureGlyph:		वापस HB_OT_LAYOUT_GLYPH_PROPS_LIGATURE;
    हाल MarkGlyph:
	  klass = get_mark_attachment_type (glyph);
	  वापस HB_OT_LAYOUT_GLYPH_PROPS_MARK | (klass << 8);
    }
  }

  HB_INTERNAL bool is_blocklisted (hb_blob_t *blob,
				   hb_face_t *face) स्थिर;

  काष्ठा accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      table = hb_sanitize_context_t ().reference_table<GDEF> (face);
      अगर (unlikely (table->is_blocklisted (table.get_blob (), face)))
      {
	hb_blob_destroy (table.get_blob ());
	table = hb_blob_get_empty ();
      }

#अगर_अघोषित HB_NO_GDEF_CACHE
      /* Flatten each mark set whose glyphs span fewer than PAGE_BITS
       * gids into a single bit-page biased at its first glyph, for
       * O(1) exact answers.  Wider sets, or on allocation failure,
       * fall back to the table. */
      {
	hb_vector_t<hb_set_t> sets;
	table->get_mark_glyph_sets ().collect_coverage (sets);
	अगर (!sets.in_error () &&
	    mark_glyph_set_bitmaps.resize (sets.लंबाई))
	  क्रम (अचिन्हित i = 0; i < sets.लंबाई; i++)
	  {
	    स्वतः &bitmap = mark_glyph_set_bitmaps.arrayZ[i];
	    bitmap.bias = HB_SET_मूल्य_INVALID;
	    स्थिर स्वतः &set = sets.arrayZ[i];
	    अगर (unlikely (set.in_error ())) जारी;
	    hb_codepoint_t min_gid = HB_SET_मूल्य_INVALID;
	    hb_codepoint_t max_gid = HB_SET_मूल्य_INVALID;
	    hb_set_अग्र (&set, &min_gid);
	    hb_set_previous (&set, &max_gid);
	    अगर (max_gid == HB_SET_मूल्य_INVALID)
	      जारी;
	    अगर (max_gid - min_gid >= hb_bit_page_t::PAGE_BITS)
	    {
	      /* Too wide for one page; a digest pre-filter instead. */
	      क्रम (hb_codepoint_t gid : set)
		bitmap.u.digest.add (gid);
	      जारी;
	    }
	    bitmap.bias = min_gid;
	    क्रम (hb_codepoint_t gid : set)
	      bitmap.u.page.add (gid - min_gid);
	  }
      }
#पूर्ण_अगर
    }
    ~accelerator_t () { table.destroy (); }

    अचिन्हित पूर्णांक get_glyph_props (hb_codepoint_t glyph) स्थिर
    {
      अचिन्हित v;

#अगर_अघोषित HB_NO_GDEF_CACHE
      अगर (glyph_props_cache.लो (glyph, &v))
        वापस v;
#पूर्ण_अगर

      v = table->get_glyph_props (glyph);

#अगर_अघोषित HB_NO_GDEF_CACHE
      अगर (likely (table.get_blob ())) // Don't try setting if we are the null instance!
	glyph_props_cache.set (glyph, v);
#पूर्ण_अगर

      वापस v;

    }

    HB_ALWAYS_INLINE
    bool mark_set_covers (अचिन्हित पूर्णांक set_सूचकांक, hb_codepoint_t glyph_id) स्थिर
    {
#अगर_अघोषित HB_NO_GDEF_CACHE
      // We can access arrayZ directly because of sanitize_lookup_props() guarantee.
      स्थिर स्वतः &bitmap = mark_glyph_set_bitmaps.arrayZ[set_सूचकांक];
      अगर (likely (bitmap.bias != HB_SET_मूल्य_INVALID))
      {
	hb_codepoint_t biased = glyph_id - bitmap.bias;
	वापस biased < hb_bit_page_t::PAGE_BITS &&
	       bitmap.u.page.लो (biased);
      }
      वापस mark_set_covers_slow (set_सूचकांक, glyph_id);
#अन्यथा
      वापस table->mark_set_covers (set_सूचकांक, glyph_id);
#पूर्ण_अगर
    }

#अगर_अघोषित HB_NO_GDEF_CACHE
    HB_NEVER_INLINE
    bool mark_set_covers_slow (अचिन्हित पूर्णांक set_सूचकांक, hb_codepoint_t glyph_id) स्थिर
    {
      स्थिर स्वतः &bitmap = mark_glyph_set_bitmaps.arrayZ[set_सूचकांक];
      अगर (!bitmap.u.digest.may_have (glyph_id))
	वापस false;
      वापस table->mark_set_covers (set_सूचकांक, glyph_id);
    }
#पूर्ण_अगर

    अचिन्हित sanitize_lookup_props (अचिन्हित lookup_props) स्थिर
    {
#अगर_अघोषित HB_NO_GDEF_CACHE
      अगर (lookup_props & LookupFlag::UseMarkFilteringSet &&
	  (lookup_props >> 16) >= mark_glyph_set_bitmaps.लंबाई)
      {
        // Invalid mark filtering set index; unset the flag.
	lookup_props &= ~LookupFlag::UseMarkFilteringSet;
      }
#पूर्ण_अगर
      वापस lookup_props;
    }

    hb_blob_ptr_t<GDEF> table;
#अगर_अघोषित HB_NO_GDEF_CACHE
    काष्ठा mark_glyph_set_bitmap_t
    {
      /* Gid of the first bit (not page-aligned) for an exact one-page
       * set, or HB_SET_VALUE_INVALID; then digest is a pre-filter for
       * the table.  The all-zeros state is valid for both members. */
      hb_codepoint_t bias = HB_SET_मूल्य_INVALID;
      जोड़ u_t {
	u_t () : page () {}
	hb_bit_page_t page;
	hb_set_digest_t digest;
      } u;
    };

    hb_vector_t<mark_glyph_set_bitmap_t> mark_glyph_set_bitmaps;
    mutable hb_cache_t<21, 3> glyph_props_cache;
    static_assert (माप (glyph_props_cache) == 512, "");
#पूर्ण_अगर
  };

  व्योम collect_variation_indices (hb_collect_variation_indices_context_t *c) स्थिर
  {
#अगर_अघोषित HB_NO_BEYOND_64K
    अगर (version.to_int () >= 0x00010004u && ligCaretList2)
    {
      (यह+ligCaretList2).collect_variation_indices (c);
      वापस;
    }
#पूर्ण_अगर
    (यह+ligCaretList).collect_variation_indices (c);
  }

  रक्षित:
  ढाँचा <typename कुंजी>
  अटल व्योम remap_varidx_after_instantiation (स्थिर hb_map_t& varidx_map,
                                                hb_hashmap_t<कुंजी, hb_pair_t<अचिन्हित, पूर्णांक>>& layout_variation_idx_delta_map /* IN/OUT */)
  {
    /* varidx_map is empty which means varstore is empty after instantiation,
     * no variations, map all varidx to HB_OT_LAYOUT_NO_VARIATIONS_INDEX.
     * varidx_map doesn't have original varidx, indicating delta row is all
     * zeros, map varidx to HB_OT_LAYOUT_NO_VARIATIONS_INDEX */
    क्रम (स्वतः _ : layout_variation_idx_delta_map.iter_ref ())
    {
      /* old_varidx->(varidx, delta) mapping generated for subsetting, then this
       * varidx is used as key of varidx_map during instantiation */
      uint32_t varidx = _.second.first;
      uint32_t *new_varidx;
      अगर (varidx_map.has (varidx, &new_varidx))
	_.second.first = *new_varidx;
      अन्यथा
	_.second.first = HB_OT_LAYOUT_NO_VARIATIONS_सूचकांक;
    }
  }

  ढाँचा <typename OffsetOut, typename OffsetIn, typename Base, typename ...Ts>
  अटल bool serialize_subset_offset (hb_subset_context_t *c,
				       OffsetOut& out,
				       स्थिर OffsetIn& in,
				       स्थिर Base *base,
				       Ts&&... ds)
  {
    out = 0;
    अगर (in.is_null ()) वापस false;

    स्वतः *s = c->serializer;
    s->push ();
    bool ret = c->dispatch (base+in, std::forward<Ts> (ds)...);
    अगर (ret) s->add_link (out, s->निकालो_pack ());
    अन्यथा s->निकालो_discard ();
    वापस ret;
  }

  FixedVersion<>version;		/* Version of the GDEF table--currently
					 * 0x00010004u */
  Offset16To<ClassDef>
		glyphClassDef;		/* Offset to class definition table
					 * for glyph type--from beginning of
					 * GDEF header (may be Null) */
  Offset16To<AttachList>
		attachList;		/* Offset to list of glyphs with
					 * attachment points--from beginning
					 * of GDEF header (may be Null) */
  Offset16To<LigCaretList<SmallTypes>>
		ligCaretList;		/* Offset to list of positioning points
					 * for ligature carets--from beginning
					 * of GDEF header (may be Null) */
  Offset16To<ClassDef>
		markAttachClassDef;	/* Offset to class definition table for
					 * mark attachment type--from beginning
					 * of GDEF header (may be Null) */
  Offset16To<MarkGlyphSets>
		markGlyphSetsDef;	/* Offset to the table of mark set
					 * definitions--from beginning of GDEF
					 * header (may be NULL).  Introduced
					 * in version 0x00010002. */
  Offset32To<ItemVariationStore>
		varStore;		/* Offset to the table of Item Variation
					 * Store--from beginning of GDEF
					 * header (may be NULL).  Introduced
					 * in version 0x00010003. */
#अगर_अघोषित HB_NO_BEYOND_64K
  Offset32To<ClassDef>
		glyphClassDef2;		/* 32-bit offset to class definition
					 * table for glyph type.  Introduced
					 * in version 0x00010004. */
  Offset32To<AttachList>
		attachList2;		/* 32-bit offset to list of glyphs
					 * with attachment points.  Introduced
					 * in version 0x00010004. */
  Offset32To<LigCaretList<MediumTypes>>
		ligCaretList2;		/* 32-bit offset to LigCaretList2.
					 * Introduced in version 0x00010004. */
  Offset32To<ClassDef>
		markAttachClassDef2;	/* 32-bit offset to class definition
					 * table for mark attachment type.
					 * Introduced in version 0x00010004. */
  Offset32To<MarkGlyphSets>
		markGlyphSetsDef2;	/* 32-bit offset to the table of mark
					 * set definitions.  Introduced in
					 * version 0x00010004. */
#पूर्ण_अगर
  खुला:
  DEFINE_आकार_MIN (12);
};

काष्ठा GDEF_accelerator_t : GDEF::accelerator_t {
  GDEF_accelerator_t (hb_face_t *face) : GDEF::accelerator_t (face) {}
};

} /* namespace OT */


#पूर्ण_अगर /* OT_LAYOUT_GDEF_GDEF_HH */
