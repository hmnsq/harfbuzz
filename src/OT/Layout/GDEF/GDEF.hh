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

#ifndef OT_LAYOUT_GDEF_GDEF_HH
#define OT_LAYOUT_GDEF_GDEF_HH

#अन्तर्भाव "../../../hb-ot-var-common.hh"

#अन्तर्भाव "../../../hb-font.hh"
#अन्तर्भाव "../../../hb-cache.hh"


नामाकाश OT {


/*
 * Attachment List Table
 */

/* Array of contour point indices--in increasing numerical order */
संरचना AttachPoint : Array16Of<HBUINT16>
{
  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    auto *out = c->serializer->start_embed (*एतत्);
    return_trace (out->serialize (c->serializer, + iter ()));
  }
};

/* AttachList is referenced by both GDEF attachListOffset and
 * attachListOffset2. The v1.4 top-level offset widens, but the
 * AttachList table itself keeps Offset16 internals. */
संरचना AttachList
{
  unsigned पूर्णाङ्क get_attach_points (hb_codepoint_t glyph_id,
				  unsigned पूर्णाङ्क start_offset,
				  unsigned पूर्णाङ्क *point_गणनम् /* IN/OUT */,
				  unsigned पूर्णाङ्क *point_array /* OUT */) ध्रुव
  {
    unsigned पूर्णाङ्क index = (एतत्+coverage).get_coverage (glyph_id);
    सचे (index == NOT_COVERED)
    {
      सचे (point_गणनम्)
	*point_गणनम् = 0;
      पटिनिवत्त 0;
    }

    ध्रुव AttachPoint &points = एतत्+attachPoint[index];

    सचे (point_गणनम् && point_array)
    {
      + points.as_array ().sub_array (start_offset, point_गणनम्)
      | hb_sink (hb_array (point_array, *point_गणनम्))
      ;
    }

    पटिनिवत्त points.len;
  }

  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    ध्रुव hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ध्रुव hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*एतत्);
    सचे (unlikely (!c->serializer->extend_min (out))) return_trace (मिच्छा);

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (एतत्+coverage, attachPoint)
    | hb_filter (glyphset, hb_first)
    | hb_filter (subset_offset_array (c, out->attachPoint, एतत्), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;
    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (तार्किक (new_coverage));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (coverage.sanitize (c, एतत्) && attachPoint.sanitize (c, एतत्));
  }

  रक्षित:
  Offset16To<Coverage>
		coverage;		/* Offset to Coverage table -- from
					 * beginning of AttachList table */
  Array16OfOffset16To<AttachPoint>
		attachPoint;		/* Array of AttachPoint tables
					 * in Coverage Index order */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (4, attachPoint);
};

/*
 * Ligature Caret Table
 */

संरचना Caret_मानम्_Format1
{
  friend संरचना Caret_मानम्;
  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    auto *out = c->serializer->embed (एतत्);
    सचे (unlikely (!out)) return_trace (मिच्छा);
    return_trace (सच्च);
  }

  गुप्त:
  hb_position_t get_caret_मानम् (hb_font_t *font, hb_direction_t direction) ध्रुव
  {
    पटिनिवत्त HB_DIRECTION_IS_HORIZONTAL (direction) ? font->em_scale_x (coordinate) : font->em_scale_y (coordinate);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्));
  }

  रक्षित:
  HBUINT16	caret_मानम्_Format;	/* Format identifier--format = 1 */
  FWORD		coordinate;		/* X or Y value, in design units */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (4);
};

संरचना Caret_मानम्_Format2
{
  friend संरचना Caret_मानम्;
  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    auto *out = c->serializer->embed (एतत्);
    सचे (unlikely (!out)) return_trace (मिच्छा);
    return_trace (सच्च);
  }

  गुप्त:
  hb_position_t get_caret_मानम् (hb_font_t *font, hb_direction_t direction, hb_codepoint_t glyph_id) ध्रुव
  {
    hb_position_t x, y;
    font->get_glyph_contour_point_for_origin (glyph_id, caret_मानम्_Point, direction, &x, &y);
    पटिनिवत्त HB_DIRECTION_IS_HORIZONTAL (direction) ? x : y;
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्));
  }

  रक्षित:
  HBUINT16	caret_मानम्_Format;	/* Format identifier--format = 2 */
  HBUINT16	caret_मानम्_Point;	/* Contour point index on glyph */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (4);
};

संरचना Caret_मानम्_Format3
{
  friend संरचना Caret_मानम्;

  hb_position_t get_caret_मानम् (hb_font_t *font, hb_direction_t direction,
				 ध्रुव ItemVariationStore &var_store) ध्रुव
  {
    पटिनिवत्त HB_DIRECTION_IS_HORIZONTAL (direction) ?
	   hb_saturate_add (font->em_scale_x (coordinate), (एतत्+deviceTable).get_x_delta (font, var_store)) :
	   hb_saturate_add (font->em_scale_y (coordinate), (एतत्+deviceTable).get_y_delta (font, var_store));
  }

  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    auto *out = c->serializer->start_embed (*एतत्);
    सचे (!c->serializer->embed (caret_मानम्_Format)) return_trace (मिच्छा);
    सचे (!c->serializer->embed (coordinate)) return_trace (मिच्छा);

    unsigned varidx = (एतत्+deviceTable).get_variation_index ();
    hb_pair_t<unsigned, पूर्णाङ्क> *new_varidx_delta;
    सचे (c->plan->layout_variation_idx_delta_map.has (varidx, &new_varidx_delta)) {
      uint32_t new_varidx = hb_first (*new_varidx_delta);
      पूर्णाङ्क delta = hb_second (*new_varidx_delta);
      सचे (delta != 0)
      {
        सचे (!c->serializer->check_assign (out->coordinate, coordinate + delta, HB_SERIALIZE_ERROR_INT_OVERFLOW))
          return_trace (मिच्छा);
      }

      सचे (new_varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
        return_trace (c->serializer->check_assign (out->caret_मानम्_Format, 1, HB_SERIALIZE_ERROR_INT_OVERFLOW));
    }

    सचे (!c->serializer->embed (deviceTable))
      return_trace (मिच्छा);

    return_trace (out->deviceTable.serialize_copy (c->serializer, deviceTable, एतत्, c->serializer->to_bias (out),
						   hb_serialize_context_t::Head, &c->plan->layout_variation_idx_delta_map));
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) ध्रुव
  { (एतत्+deviceTable).collect_variation_indices (c); }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) && deviceTable.sanitize (c, एतत्));
  }

  रक्षित:
  HBUINT16	caret_मानम्_Format;	/* Format identifier--format = 3 */
  FWORD		coordinate;		/* X or Y value, in design units */
  Offset16To<Device>
		deviceTable;		/* Offset to Device table for X or Y
					 * value--from beginning of CaretValue
					 * table */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (6);
};

संरचना Caret_मानम्
{
  hb_position_t get_caret_मानम् (hb_font_t *font,
				 hb_direction_t direction,
				 hb_codepoint_t glyph_id,
				 ध्रुव ItemVariationStore &var_store) ध्रुव
  {
    विकल्प (u.format.v) {
    पक्ष 1: hb_barrier (); पटिनिवत्त u.format1.get_caret_मानम् (font, direction);
    पक्ष 2: hb_barrier (); पटिनिवत्त u.format2.get_caret_मानम् (font, direction, glyph_id);
    पक्ष 3: hb_barrier (); पटिनिवत्त u.format3.get_caret_मानम् (font, direction, var_store);
    स्वभाव:पटिनिवत्त 0;
    }
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ध्रुव
  {
    सचे (unlikely (!c->may_dispatch (एतत्, &u.format.v))) पटिनिवत्त c->no_dispatch_return_मानम् ();
    TRACE_DISPATCH (एतत्, u.format.v);
    विकल्प (u.format.v) {
    पक्ष 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    पक्ष 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    पक्ष 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    स्वभाव:return_trace (c->default_return_मानम् ());
    }
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) ध्रुव
  {
    विकल्प (u.format.v) {
    पक्ष 1:
    पक्ष 2:
      पटिनिवत्त;
    पक्ष 3:
      hb_barrier ();
      u.format3.collect_variation_indices (c);
      पटिनिवत्त;
    स्वभाव: पटिनिवत्त;
    }
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    सचे (!u.format.v.sanitize (c)) return_trace (मिच्छा);
    hb_barrier ();
    विकल्प (u.format.v) {
    पक्ष 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    पक्ष 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    पक्ष 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    स्वभाव:return_trace (सच्च);
    }
  }

  रक्षित:
  सङ्घ {
  संरचना { HBUINT16 v; }	format;		/* Format identifier */
  Caret_मानम्_Format1	format1;
  Caret_मानम्_Format2	format2;
  Caret_मानम्_Format3	format3;
  } u;
  सार्वजनिक:
  DEFINE_SIZE_UNION (2, format.v);
};

संरचना LigGlyph
{
  unsigned get_lig_carets (hb_font_t            *font,
			   hb_direction_t        direction,
			   hb_codepoint_t        glyph_id,
			   ध्रुव ItemVariationStore &var_store,
			   unsigned              start_offset,
			   unsigned             *caret_गणनम् /* IN/OUT */,
			   hb_position_t        *caret_array /* OUT */) ध्रुव
  {
    सचे (caret_गणनम् && caret_array)
    {
      + carets.as_array ().sub_array (start_offset, caret_गणनम्)
      | hb_map (hb_add (एतत्))
      | hb_map ([&] (ध्रुव Caret_मानम् &value) { पटिनिवत्त value.get_caret_मानम् (font, direction, glyph_id, var_store); })
      | hb_sink (hb_array (caret_array, *caret_गणनम्))
      ;
    }

    पटिनिवत्त carets.len;
  }

  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    auto *out = c->serializer->start_embed (*एतत्);
    सचे (unlikely (!c->serializer->extend_min (out))) return_trace (मिच्छा);

    + hb_iter (carets)
    | hb_apply (subset_offset_array (c, out->carets, एतत्))
    ;

    return_trace (तार्किक (out->carets));
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) ध्रुव
  {
    अत्थाय (ध्रुव Offset16To<Caret_मानम्>& offset : carets.iter ())
      (एतत्+offset).collect_variation_indices (c);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (carets.sanitize (c, एतत्));
  }

  रक्षित:
  Array16OfOffset16To<Caret_मानम्>
		carets;			/* Offset array of CaretValue tables
					 * --from beginning of LigGlyph table
					 * --in increasing coordinate order */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (2, carets);
};

/* LigCaretList2 widens the LigCaretList coverage, count, and LigGlyph
 * offsets. The LigGlyph table itself keeps Offset16 caretValueOffsets. */
template <typename Types>
संरचना LigCaretList
{
  unsigned पूर्णाङ्क get_lig_carets (hb_font_t *font,
			       hb_direction_t direction,
			       hb_codepoint_t glyph_id,
			       ध्रुव ItemVariationStore &var_store,
			       unsigned पूर्णाङ्क start_offset,
			       unsigned पूर्णाङ्क *caret_गणनम् /* IN/OUT */,
			       hb_position_t *caret_array /* OUT */) ध्रुव
  {
    unsigned पूर्णाङ्क index = (एतत्+coverage).get_coverage (glyph_id);
    सचे (index == NOT_COVERED)
    {
      सचे (caret_गणनम्)
	*caret_गणनम् = 0;
      पटिनिवत्त 0;
    }
    ध्रुव LigGlyph &lig_glyph = एतत्+ligGlyph[index];
    पटिनिवत्त lig_glyph.get_lig_carets (font, direction, glyph_id, var_store, start_offset, caret_गणनम्, caret_array);
  }

  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    ध्रुव hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ध्रुव hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*एतत्);
    सचे (unlikely (!c->serializer->extend_min (out))) return_trace (मिच्छा);

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (एतत्+coverage, ligGlyph)
    | hb_filter (glyphset, hb_first)
    | hb_filter (subset_offset_array (c, out->ligGlyph, एतत्), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;
    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (तार्किक (new_coverage));
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) ध्रुव
  {
    + hb_zip (एतत्+coverage, ligGlyph)
    | hb_filter (c->glyph_set, hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (एतत्))
    | hb_apply ([c] (ध्रुव LigGlyph& _) { _.collect_variation_indices (c); })
    ;
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (coverage.sanitize (c, एतत्) && ligGlyph.sanitize (c, एतत्));
  }

  रक्षित:
  typename Types::template LOffsetTo<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of LigCaretList table */
  typename Types::template ArrayOf<typename Types::template OffsetTo<LigGlyph>>
		ligGlyph;		/* Array of LigGlyph tables
					 * in Coverage Index order */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (Types::LOffset::static_size + Types::HBUINT::static_size, ligGlyph);
};

#ifndef HB_NO_BEYOND_64K
static_assert (LigCaretList<MediumTypes>::min_size ==
	       Offset32::static_size + HBUINT24::static_size, "");
#endif


संरचना MarkGlyphSetsFormat1
{
  तार्किक covers (unsigned पूर्णाङ्क set_index, hb_codepoint_t glyph_id) ध्रुव
  { पटिनिवत्त (एतत्+coverage[set_index]).get_coverage (glyph_id) != NOT_COVERED; }

  शून्य collect_used_mark_sets (ध्रुव hb_set_t& glyph_set,
                               hb_set_t& used_mark_sets /* OUT */) ध्रुव
  {
    unsigned i = 0;
    अत्थाय (ध्रुव auto &offset : coverage)
     {
       ध्रुव auto &cov = एतत्+offset;
       सचे (cov.intersects (&glyph_set))
         used_mark_sets.add (i);

       i++;
     }
  }

  template <typename set_t>
  शून्य collect_coverage (hb_vector_t<set_t> &sets) ध्रुव
  {
     अत्थाय (ध्रुव auto &offset : coverage)
     {
       ध्रुव auto &cov = एतत्+offset;
       cov.collect_coverage (sets.push ());
     }
  }

  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    auto *out = c->serializer->start_embed (*एतत्);
    सचे (unlikely (!c->serializer->extend_min (out))) return_trace (मिच्छा);
    out->format = format;

    तार्किक ret = सच्च;
    अत्थाय (ध्रुव Offset32To<Coverage>& offset : coverage.iter ())
    {
      auto snap = c->serializer->snapshot ();
      auto *o = out->coverage.serialize_append (c->serializer);
      सचे (unlikely (!o))
      {
	ret = मिच्छा;
	भङ्ग;
      }

      //skip empty coverage
      c->serializer->push ();
      तार्किक res = मिच्छा;
      सचे (offset) res = c->dispatch (एतत्+offset);
      सचे (!res)
      {
        c->serializer->निष्कासय_discard ();
        c->serializer->revert (snap);
        (out->coverage.len)--;
        अनुवर्त;
      }
      c->serializer->add_link (*o, c->serializer->निष्कासय_pack ());
    }

    return_trace (ret && out->coverage.len);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (coverage.sanitize (c, एतत्));
  }

  रक्षित:
  HBUINT16	format;			/* Format identifier--format = 1 */
  Array16Of<Offset32To<Coverage>>
		coverage;		/* Array of long offsets to mark set
					 * coverage tables */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (4, coverage);
};

संरचना MarkGlyphSets
{
  तार्किक covers (unsigned पूर्णाङ्क set_index, hb_codepoint_t glyph_id) ध्रुव
  {
    विकल्प (u.format.v) {
    पक्ष 1: hb_barrier (); पटिनिवत्त u.format1.covers (set_index, glyph_id);
    स्वभाव:पटिनिवत्त मिच्छा;
    }
  }

  template <typename set_t>
  शून्य collect_coverage (hb_vector_t<set_t> &sets) ध्रुव
  {
    विकल्प (u.format.v) {
    पक्ष 1: hb_barrier (); u.format1.collect_coverage (sets); पटिनिवत्त;
    स्वभाव:पटिनिवत्त;
    }
  }

  शून्य collect_used_mark_sets (ध्रुव hb_set_t& glyph_set,
                               hb_set_t& used_mark_sets /* OUT */) ध्रुव
  {
    विकल्प (u.format.v) {
    पक्ष 1: hb_barrier (); u.format1.collect_used_mark_sets (glyph_set, used_mark_sets); पटिनिवत्त;
    स्वभाव:पटिनिवत्त;
    }
  }

  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    विकल्प (u.format.v) {
    पक्ष 1: hb_barrier (); return_trace (u.format1.subset (c));
    स्वभाव:return_trace (मिच्छा);
    }
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    सचे (!u.format.v.sanitize (c)) return_trace (मिच्छा);
    hb_barrier ();
    विकल्प (u.format.v) {
    पक्ष 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    स्वभाव:return_trace (सच्च);
    }
  }

  रक्षित:
  सङ्घ {
  संरचना { HBUINT16 v; }	format;		/* Format identifier */
  MarkGlyphSetsFormat1	format1;
  } u;
  सार्वजनिक:
  DEFINE_SIZE_UNION (2, format.v);
};


/*
 * GDEF -- Glyph Definition
 * https://docs.microsoft.com/en-us/typography/opentype/spec/gdef
 */


संरचना GDEF
{
  स्थिर constexpr hb_tag_t tableTag = HB_OT_TAG_GDEF;

  गणना GlyphClasses {
    UnclassifiedGlyph	= 0,
    BaseGlyph		= 1,
    LigatureGlyph	= 2,
    MarkGlyph		= 3,
    ComponentGlyph	= 4
  };

  size_t get_size () ध्रुव
  {
    सचे (version.major != 1) पटिनिवत्त version.static_size;

    पटिनिवत्त min_size +
	   (version.to_int () >= 0x00010002u ? markGlyphSetsDef.static_size : 0) +
	   (version.to_int () >= 0x00010003u ? varStore.static_size : 0) +
#ifndef HB_NO_BEYOND_64K
	   (version.to_int () >= 0x00010004u ? 5 * Offset32::static_size : 0)
#अञ्ञथा
	   0
#endif
	   ;
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    सचे (unlikely (!version.sanitize (c) ||
		  version.major != 1 ||
		  !c->check_range (एतत्, get_size ())))
      return_trace (मिच्छा);
    hb_barrier ();

    तार्किक glyph_class_def_ok = glyphClassDef.sanitize (c, एतत्);
    तार्किक attach_list_ok = attachList.sanitize (c, एतत्);
    तार्किक lig_caret_list_ok = ligCaretList.sanitize (c, एतत्);
    तार्किक mark_attach_class_def_ok = markAttachClassDef.sanitize (c, एतत्);
    तार्किक mark_glyph_sets_def_ok = version.to_int () < 0x00010002u ||
				  markGlyphSetsDef.sanitize (c, एतत्);

#ifndef HB_NO_BEYOND_64K
    सचे (version.to_int () >= 0x00010004u)
    {
      सचे (glyphClassDef2) glyph_class_def_ok = glyphClassDef2.sanitize (c, एतत्);
      सचे (attachList2) attach_list_ok = attachList2.sanitize (c, एतत्);
      सचे (ligCaretList2) lig_caret_list_ok = ligCaretList2.sanitize (c, एतत्);
      सचे (markAttachClassDef2) mark_attach_class_def_ok = markAttachClassDef2.sanitize (c, एतत्);
      सचे (markGlyphSetsDef2) mark_glyph_sets_def_ok = markGlyphSetsDef2.sanitize (c, एतत्);
    }
#endif

    return_trace (glyph_class_def_ok &&
		  attach_list_ok &&
		  lig_caret_list_ok &&
		  mark_attach_class_def_ok &&
		  mark_glyph_sets_def_ok &&
		  (version.to_int () < 0x00010003u || varStore.sanitize (c, एतत्)));
  }

  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    सचे (version.major != 1) return_trace (मिच्छा);

    auto *out = c->serializer->start_embed (*एतत्);
    सचे (unlikely (!c->serializer->extend_min (out))) return_trace (मिच्छा);

    // Push var store first (if it's needed) so that it's last in the
    // serialization order. Some font consumers assume that varstore runs to
    // the end of the GDEF table.
    // See: https://github.com/harfbuzz/harfbuzz/issues/4636
    auto snapshot_version0 = c->serializer->snapshot ();
    सचे (unlikely (version.to_int () >= 0x00010002u && hb_barrier () &&
		  !c->serializer->embed (markGlyphSetsDef)))
      return_trace (मिच्छा);

    auto snapshot_version2 = c->serializer->snapshot ();
    सचे (unlikely (version.to_int () >= 0x00010003u && hb_barrier () &&
		  !c->serializer->embed (varStore)))
      return_trace (मिच्छा);

    तार्किक use_v14 = मिच्छा;
#ifndef HB_NO_BEYOND_64K
    use_v14 = version.to_int () >= 0x00010004u &&
	      (glyphClassDef2 || attachList2 || ligCaretList2 ||
	       markAttachClassDef2 || markGlyphSetsDef2);
    सचे (unlikely (version.to_int () >= 0x00010004u && hb_barrier () &&
		  !c->serializer->extend_size (out, get_size ())))
      return_trace (मिच्छा);
#endif

    तार्किक subset_varstore = मिच्छा;
    unsigned varstore_index = (unsigned) -1;
    सचे (version.to_int () >= 0x00010003u && hb_barrier ())
    {
      सचे (c->plan->all_axes_pinned)
	out->varStore = 0;
      अञ्ञथा सचे (c->plan->normalized_coords)
      {
	सचे (varStore)
	{
	  item_variations_t item_vars;
	  सचे (unlikely (!c->serializer->check_success (
                  item_vars.instantiate (एतत्+varStore, c->plan, सच्च, सच्च,
                                          c->plan->gdef_varstore_inner_maps.as_array (), सच्च))))
            return_trace (मिच्छा);
	  subset_varstore = out->varStore.serialize_serialize (c->serializer,
							     item_vars.has_long_word (),
							     c->plan->axis_tags,
							     item_vars.get_region_list (),
							     item_vars.get_vardata_encodings ());
	  varstore_index = c->serializer->last_added_child_index();
	  remap_varidx_after_instantiation (item_vars.get_varidx_map (),
					    c->plan->layout_variation_idx_delta_map);
          remap_varidx_after_instantiation (item_vars.get_varidx_map (),
                                            c->plan->layout_condition_idx_मानम्_map);
	}
      }
      अञ्ञथा
      {
	subset_varstore = out->varStore.serialize_subset (c, varStore, एतत्,
							  c->plan->gdef_varstore_inner_maps.as_array ());
	varstore_index = c->serializer->last_added_child_index();
      }
    }

    out->version.major = version.major;
    out->version.minor = version.minor;

    सचे (!subset_varstore && version.to_int () >= 0x00010002u && !use_v14) {
      c->serializer->revert (snapshot_version2);
    }
    सचे (!subset_varstore)
      out->varStore = 0;
    अञ्ञथा
      c->plan->has_gdef_varstore = सच्च;

    तार्किक subset_markglyphsetsdef = मिच्छा;
    सचे (version.to_int () >= 0x00010002u && hb_barrier ())
    {
#ifndef HB_NO_BEYOND_64K
      सचे (use_v14 && markGlyphSetsDef2)
      {
	out->markGlyphSetsDef = 0;
	subset_markglyphsetsdef = serialize_subset_offset (c, out->markGlyphSetsDef2,
							   markGlyphSetsDef2, एतत्);
      }
      अञ्ञथा
#endif
      {
#ifndef HB_NO_BEYOND_64K
	सचे (use_v14) out->markGlyphSetsDef2 = 0;
#endif
	subset_markglyphsetsdef = out->markGlyphSetsDef.serialize_subset (c, markGlyphSetsDef, एतत्);
      }
    }

    सचे (use_v14)
    {
      out->version.minor = 4;
    } अञ्ञथा सचे (subset_varstore) {
      out->version.minor = 3;
    } अञ्ञथा सचे (subset_markglyphsetsdef) {
      out->version.minor = 2;
    } अञ्ञथा  {
      out->version.minor = 0;
      c->serializer->revert (snapshot_version0);
    }

    तार्किक subset_glyphclassdef = मिच्छा;
    तार्किक subset_attachlist = मिच्छा;
    तार्किक subset_markattachclassdef = मिच्छा;
    तार्किक subset_ligcaretlist = मिच्छा;

#ifndef HB_NO_BEYOND_64K
    सचे (use_v14 && glyphClassDef2)
    {
      out->glyphClassDef = 0;
      subset_glyphclassdef = serialize_subset_offset (c, out->glyphClassDef2,
						      glyphClassDef2, एतत्, nullptr, मिच्छा, सच्च);
    }
    अञ्ञथा
#endif
    {
#ifndef HB_NO_BEYOND_64K
      सचे (use_v14) out->glyphClassDef2 = 0;
#endif
      subset_glyphclassdef = out->glyphClassDef.serialize_subset (c, glyphClassDef, एतत्, nullptr, मिच्छा, सच्च);
    }

#ifndef HB_NO_BEYOND_64K
    सचे (use_v14 && attachList2)
    {
      out->attachList = 0;
      subset_attachlist = serialize_subset_offset (c, out->attachList2, attachList2, एतत्);
    }
    अञ्ञथा
#endif
    {
#ifndef HB_NO_BEYOND_64K
      सचे (use_v14) out->attachList2 = 0;
#endif
      subset_attachlist = out->attachList.serialize_subset (c, attachList, एतत्);
    }

#ifndef HB_NO_BEYOND_64K
    सचे (use_v14 && markAttachClassDef2)
    {
      out->markAttachClassDef = 0;
      subset_markattachclassdef = serialize_subset_offset (c, out->markAttachClassDef2,
							   markAttachClassDef2, एतत्, nullptr, मिच्छा, सच्च);
    }
    अञ्ञथा
#endif
    {
#ifndef HB_NO_BEYOND_64K
      सचे (use_v14) out->markAttachClassDef2 = 0;
#endif
      subset_markattachclassdef = out->markAttachClassDef.serialize_subset (c, markAttachClassDef,
									   एतत्, nullptr, मिच्छा, सच्च);
    }

#ifndef HB_NO_BEYOND_64K
    सचे (use_v14 && ligCaretList2)
    {
      out->ligCaretList = 0;
      subset_ligcaretlist = serialize_subset_offset (c, out->ligCaretList2, ligCaretList2, एतत्);
    }
    अञ्ञथा
#endif
    {
#ifndef HB_NO_BEYOND_64K
      सचे (use_v14) out->ligCaretList2 = 0;
#endif
      subset_ligcaretlist = out->ligCaretList.serialize_subset (c, ligCaretList, एतत्);
    }

    सचे (subset_varstore && varstore_index != (unsigned) -1) {
      c->serializer->repack_last(varstore_index);
    }

    return_trace (subset_glyphclassdef || subset_attachlist ||
		  subset_ligcaretlist || subset_markattachclassdef ||
		  (out->version.to_int () >= 0x00010002u && subset_markglyphsetsdef) ||
		  (out->version.to_int () >= 0x00010003u && subset_varstore));
  }

  तार्किक has_glyph_classes () ध्रुव
  {
#ifndef HB_NO_BEYOND_64K
    सचे (version.to_int () >= 0x00010004u && glyphClassDef2) पटिनिवत्त सच्च;
#endif
    पटिनिवत्त glyphClassDef != 0;
  }
  ध्रुव ClassDef &get_glyph_class_def () ध्रुव
  {
#ifndef HB_NO_BEYOND_64K
    सचे (version.to_int () >= 0x00010004u && glyphClassDef2) पटिनिवत्त एतत्+glyphClassDef2;
#endif
    पटिनिवत्त एतत्+glyphClassDef;
  }
  तार्किक has_attach_list () ध्रुव
  {
#ifndef HB_NO_BEYOND_64K
    सचे (version.to_int () >= 0x00010004u && attachList2) पटिनिवत्त सच्च;
#endif
    पटिनिवत्त attachList != 0;
  }
  ध्रुव AttachList &get_attach_list () ध्रुव
  {
#ifndef HB_NO_BEYOND_64K
    सचे (version.to_int () >= 0x00010004u && attachList2) पटिनिवत्त एतत्+attachList2;
#endif
    पटिनिवत्त एतत्+attachList;
  }
  तार्किक has_lig_carets () ध्रुव
  {
#ifndef HB_NO_BEYOND_64K
    सचे (version.to_int () >= 0x00010004u && ligCaretList2) पटिनिवत्त सच्च;
#endif
    पटिनिवत्त ligCaretList != 0;
  }
  तार्किक has_mark_attachment_types () ध्रुव
  {
#ifndef HB_NO_BEYOND_64K
    सचे (version.to_int () >= 0x00010004u && markAttachClassDef2) पटिनिवत्त सच्च;
#endif
    पटिनिवत्त markAttachClassDef != 0;
  }
  ध्रुव ClassDef &get_mark_attach_class_def () ध्रुव
  {
#ifndef HB_NO_BEYOND_64K
    सचे (version.to_int () >= 0x00010004u && markAttachClassDef2) पटिनिवत्त एतत्+markAttachClassDef2;
#endif
    पटिनिवत्त एतत्+markAttachClassDef;
  }
  तार्किक has_mark_glyph_sets () ध्रुव
  {
    सचे (version.to_int () < 0x00010002u) पटिनिवत्त मिच्छा;
#ifndef HB_NO_BEYOND_64K
    सचे (version.to_int () >= 0x00010004u && markGlyphSetsDef2) पटिनिवत्त सच्च;
#endif
    पटिनिवत्त markGlyphSetsDef != 0;
  }
  ध्रुव MarkGlyphSets &get_mark_glyph_sets () ध्रुव
  {
    सचे (version.to_int () < 0x00010002u) पटिनिवत्त Null(MarkGlyphSets);
#ifndef HB_NO_BEYOND_64K
    सचे (version.to_int () >= 0x00010004u && markGlyphSetsDef2) पटिनिवत्त एतत्+markGlyphSetsDef2;
#endif
    पटिनिवत्त एतत्+markGlyphSetsDef;
  }
  तार्किक has_var_store () ध्रुव
  {
    पटिनिवत्त version.to_int () >= 0x00010003u && hb_barrier () && varStore != 0;
  }
  ध्रुव ItemVariationStore &get_var_store () ध्रुव
  {
    पटिनिवत्त version.to_int () >= 0x00010003u && hb_barrier () ? एतत्+varStore : Null(ItemVariationStore);
  }


  तार्किक has_data () ध्रुव { पटिनिवत्त version.to_int (); }
  unsigned पूर्णाङ्क get_glyph_class (hb_codepoint_t glyph) ध्रुव
  { पटिनिवत्त get_glyph_class_def ().get_class (glyph); }
  शून्य get_glyphs_in_class (unsigned पूर्णाङ्क klass, hb_set_t *glyphs) ध्रुव
  { get_glyph_class_def ().collect_class (glyphs, klass); }

  unsigned पूर्णाङ्क get_mark_attachment_type (hb_codepoint_t glyph) ध्रुव
  { पटिनिवत्त get_mark_attach_class_def ().get_class (glyph); }

  unsigned पूर्णाङ्क get_attach_points (hb_codepoint_t glyph_id,
				  unsigned पूर्णाङ्क start_offset,
				  unsigned पूर्णाङ्क *point_गणनम् /* IN/OUT */,
				  unsigned पूर्णाङ्क *point_array /* OUT */) ध्रुव
  { पटिनिवत्त get_attach_list ().get_attach_points (glyph_id, start_offset, point_गणनम्, point_array); }

  unsigned पूर्णाङ्क get_lig_carets (hb_font_t *font,
			       hb_direction_t direction,
			       hb_codepoint_t glyph_id,
			       unsigned पूर्णाङ्क start_offset,
			       unsigned पूर्णाङ्क *caret_गणनम् /* IN/OUT */,
			       hb_position_t *caret_array /* OUT */) ध्रुव
  {
#ifndef HB_NO_BEYOND_64K
    सचे (version.to_int () >= 0x00010004u && ligCaretList2)
      पटिनिवत्त (एतत्+ligCaretList2).get_lig_carets (font, direction, glyph_id, get_var_store (),
						  start_offset, caret_गणनम्, caret_array);
#endif
    पटिनिवत्त (एतत्+ligCaretList).get_lig_carets (font, direction, glyph_id, get_var_store (),
					       start_offset, caret_गणनम्, caret_array);
  }

  तार्किक mark_set_covers (unsigned पूर्णाङ्क set_index, hb_codepoint_t glyph_id) ध्रुव
  { पटिनिवत्त get_mark_glyph_sets ().covers (set_index, glyph_id); }

  /* glyph_props is a 16-bit integer where the lower 8-bit have bits representing
   * glyph class and other bits, and high 8-bit the mark attachment type (if any).
   * Not to be confused with lookup_props which is very similar. */
  unsigned पूर्णाङ्क get_glyph_props (hb_codepoint_t glyph) ध्रुव
  {
    unsigned पूर्णाङ्क klass = get_glyph_class (glyph);

    static_assert (((unsigned पूर्णाङ्क) HB_OT_LAYOUT_GLYPH_PROPS_BASE_GLYPH == (unsigned पूर्णाङ्क) LookupFlag::IgnoreBaseGlyphs), "");
    static_assert (((unsigned पूर्णाङ्क) HB_OT_LAYOUT_GLYPH_PROPS_LIGATURE == (unsigned पूर्णाङ्क) LookupFlag::IgnoreLigatures), "");
    static_assert (((unsigned पूर्णाङ्क) HB_OT_LAYOUT_GLYPH_PROPS_MARK == (unsigned पूर्णाङ्क) LookupFlag::IgnoreMarks), "");

    विकल्प (klass) {
    स्वभाव:			पटिनिवत्त HB_OT_LAYOUT_GLYPH_CLASS_UNCLASSIFIED;
    पक्ष BaseGlyph:		पटिनिवत्त HB_OT_LAYOUT_GLYPH_PROPS_BASE_GLYPH;
    पक्ष LigatureGlyph:		पटिनिवत्त HB_OT_LAYOUT_GLYPH_PROPS_LIGATURE;
    पक्ष MarkGlyph:
	  klass = get_mark_attachment_type (glyph);
	  पटिनिवत्त HB_OT_LAYOUT_GLYPH_PROPS_MARK | (klass << 8);
    }
  }

  HB_INTERNAL तार्किक is_blocklisted (hb_blob_t *blob,
				   hb_face_t *face) ध्रुव;

  संरचना accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      table = hb_sanitize_context_t ().reference_table<GDEF> (face);
      सचे (unlikely (table->is_blocklisted (table.get_blob (), face)))
      {
	hb_blob_destroy (table.get_blob ());
	table = hb_blob_get_empty ();
      }

#ifndef HB_NO_GDEF_CACHE
      /* Flatten each mark set whose glyphs span fewer than PAGE_BITS
       * gids into a single bit-page biased at its first glyph, for
       * O(1) exact answers.  Wider sets, or on allocation failure,
       * fall back to the table. */
      {
	hb_vector_t<hb_set_t> sets;
	table->get_mark_glyph_sets ().collect_coverage (sets);
	सचे (!sets.in_error () &&
	    mark_glyph_set_bitmaps.resize (sets.length))
	  अत्थाय (unsigned i = 0; i < sets.length; i++)
	  {
	    auto &bitmap = mark_glyph_set_bitmaps.arrayZ[i];
	    bitmap.bias = HB_SET_मानम्_INVALID;
	    ध्रुव auto &set = sets.arrayZ[i];
	    सचे (unlikely (set.in_error ())) अनुवर्त;
	    hb_codepoint_t min_gid = HB_SET_मानम्_INVALID;
	    hb_codepoint_t max_gid = HB_SET_मानम्_INVALID;
	    hb_set_next (&set, &min_gid);
	    hb_set_previous (&set, &max_gid);
	    सचे (max_gid == HB_SET_मानम्_INVALID)
	      अनुवर्त;
	    सचे (max_gid - min_gid >= hb_bit_page_t::PAGE_BITS)
	    {
	      /* Too wide for one page; a digest pre-filter instead. */
	      अत्थाय (hb_codepoint_t gid : set)
		bitmap.u.digest.add (gid);
	      अनुवर्त;
	    }
	    bitmap.bias = min_gid;
	    अत्थाय (hb_codepoint_t gid : set)
	      bitmap.u.page.add (gid - min_gid);
	  }
      }
#endif
    }
    ~accelerator_t () { table.destroy (); }

    unsigned पूर्णाङ्क get_glyph_props (hb_codepoint_t glyph) ध्रुव
    {
      unsigned v;

#ifndef HB_NO_GDEF_CACHE
      सचे (glyph_props_cache.get (glyph, &v))
        पटिनिवत्त v;
#endif

      v = table->get_glyph_props (glyph);

#ifndef HB_NO_GDEF_CACHE
      सचे (likely (table.get_blob ())) // Don't try setting if we are the null instance!
	glyph_props_cache.set (glyph, v);
#endif

      पटिनिवत्त v;

    }

    HB_ALWAYS_INLINE
    तार्किक mark_set_covers (unsigned पूर्णाङ्क set_index, hb_codepoint_t glyph_id) ध्रुव
    {
#ifndef HB_NO_GDEF_CACHE
      // We can access arrayZ directly because of sanitize_lookup_props() guarantee.
      ध्रुव auto &bitmap = mark_glyph_set_bitmaps.arrayZ[set_index];
      सचे (likely (bitmap.bias != HB_SET_मानम्_INVALID))
      {
	hb_codepoint_t biased = glyph_id - bitmap.bias;
	पटिनिवत्त biased < hb_bit_page_t::PAGE_BITS &&
	       bitmap.u.page.get (biased);
      }
      पटिनिवत्त mark_set_covers_slow (set_index, glyph_id);
#अञ्ञथा
      पटिनिवत्त table->mark_set_covers (set_index, glyph_id);
#endif
    }

#ifndef HB_NO_GDEF_CACHE
    HB_NEVER_INLINE
    तार्किक mark_set_covers_slow (unsigned पूर्णाङ्क set_index, hb_codepoint_t glyph_id) ध्रुव
    {
      ध्रुव auto &bitmap = mark_glyph_set_bitmaps.arrayZ[set_index];
      सचे (!bitmap.u.digest.may_have (glyph_id))
	पटिनिवत्त मिच्छा;
      पटिनिवत्त table->mark_set_covers (set_index, glyph_id);
    }
#endif

    unsigned sanitize_lookup_props (unsigned lookup_props) ध्रुव
    {
#ifndef HB_NO_GDEF_CACHE
      सचे (lookup_props & LookupFlag::UseMarkFilteringSet &&
	  (lookup_props >> 16) >= mark_glyph_set_bitmaps.length)
      {
        // Invalid mark filtering set index; unset the flag.
	lookup_props &= ~LookupFlag::UseMarkFilteringSet;
      }
#endif
      पटिनिवत्त lookup_props;
    }

    hb_blob_ptr_t<GDEF> table;
#ifndef HB_NO_GDEF_CACHE
    संरचना mark_glyph_set_bitmap_t
    {
      /* Gid of the first bit (not page-aligned) for an exact one-page
       * set, or HB_SET_VALUE_INVALID; then digest is a pre-filter for
       * the table.  The all-zeros state is valid for both members. */
      hb_codepoint_t bias = HB_SET_मानम्_INVALID;
      सङ्घ u_t {
	u_t () : page () {}
	hb_bit_page_t page;
	hb_set_digest_t digest;
      } u;
    };

    hb_vector_t<mark_glyph_set_bitmap_t> mark_glyph_set_bitmaps;
    mutable hb_cache_t<21, 3> glyph_props_cache;
    static_assert (परिमाण (glyph_props_cache) == 512, "");
#endif
  };

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) ध्रुव
  {
#ifndef HB_NO_BEYOND_64K
    सचे (version.to_int () >= 0x00010004u && ligCaretList2)
    {
      (एतत्+ligCaretList2).collect_variation_indices (c);
      पटिनिवत्त;
    }
#endif
    (एतत्+ligCaretList).collect_variation_indices (c);
  }

  रक्षित:
  template <typename कुञ्चिका>
  स्थिर शून्य remap_varidx_after_instantiation (ध्रुव hb_map_t& varidx_map,
                                                hb_hashmap_t<कुञ्चिका, hb_pair_t<unsigned, पूर्णाङ्क>>& layout_variation_idx_delta_map /* IN/OUT */)
  {
    /* varidx_map is empty which means varstore is empty after instantiation,
     * no variations, map all varidx to HB_OT_LAYOUT_NO_VARIATIONS_INDEX.
     * varidx_map doesn't have original varidx, indicating delta row is all
     * zeros, map varidx to HB_OT_LAYOUT_NO_VARIATIONS_INDEX */
    अत्थाय (auto _ : layout_variation_idx_delta_map.iter_ref ())
    {
      /* old_varidx->(varidx, delta) mapping generated for subsetting, then this
       * varidx is used as key of varidx_map during instantiation */
      uint32_t varidx = _.second.first;
      uint32_t *new_varidx;
      सचे (varidx_map.has (varidx, &new_varidx))
	_.second.first = *new_varidx;
      अञ्ञथा
	_.second.first = HB_OT_LAYOUT_NO_VARIATIONS_INDEX;
    }
  }

  template <typename OffsetOut, typename OffsetIn, typename Base, typename ...Ts>
  स्थिर तार्किक serialize_subset_offset (hb_subset_context_t *c,
				       OffsetOut& out,
				       ध्रुव OffsetIn& in,
				       ध्रुव Base *base,
				       Ts&&... ds)
  {
    out = 0;
    सचे (in.is_null ()) पटिनिवत्त मिच्छा;

    auto *s = c->serializer;
    s->push ();
    तार्किक ret = c->dispatch (base+in, std::forward<Ts> (ds)...);
    सचे (ret) s->add_link (out, s->निष्कासय_pack ());
    अञ्ञथा s->निष्कासय_discard ();
    पटिनिवत्त ret;
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
#ifndef HB_NO_BEYOND_64K
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
#endif
  सार्वजनिक:
  DEFINE_SIZE_MIN (12);
};

संरचना GDEF_accelerator_t : GDEF::accelerator_t {
  GDEF_accelerator_t (hb_face_t *face) : GDEF::accelerator_t (face) {}
};

} /* namespace OT */


#endif /* OT_LAYOUT_GDEF_GDEF_HH */
