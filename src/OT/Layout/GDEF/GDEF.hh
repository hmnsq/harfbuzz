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

#અન્તર્ભાવ "../../../hb-ot-var-common.hh"

#અન્તર્ભાવ "../../../hb-font.hh"
#અન્તર્ભાવ "../../../hb-cache.hh"


નામાકાશ OT {


/*
 * Attachment List Table
 */

/* Array of contour point indices--in increasing numerical order */
સંરચના AttachPoint : Array16Of<HBUINT16>
{
  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);
    auto *out = c->serializer->start_embed (*આ);
    return_trace (out->serialize (c->serializer, + iter ()));
  }
};

/* AttachList is referenced by both GDEF attachListOffset and
 * attachListOffset2. The v1.4 top-level offset widens, but the
 * AttachList table itself keeps Offset16 internals. */
સંરચના AttachList
{
  unsigned પૂર્ણાંક get_attach_points (hb_codepoint_t glyph_id,
				  unsigned પૂર્ણાંક start_offset,
				  unsigned પૂર્ણાંક *point_ગણતરી /* IN/OUT */,
				  unsigned પૂર્ણાંક *point_array /* OUT */) અચલ
  {
    unsigned પૂર્ણાંક index = (આ+coverage).get_coverage (glyph_id);
    જો (index == NOT_COVERED)
    {
      જો (point_ગણતરી)
	*point_ગણતરી = 0;
      પાછા 0;
    }

    અચલ AttachPoint &points = આ+attachPoint[index];

    જો (point_ગણતરી && point_array)
    {
      + points.as_array ().sub_array (start_offset, point_ગણતરી)
      | hb_sink (hb_array (point_array, *point_ગણતરી))
      ;
    }

    પાછા points.len;
  }

  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);
    અચલ hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    અચલ hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*આ);
    જો (unlikely (!c->serializer->extend_min (out))) return_trace (ખોટું);

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (આ+coverage, attachPoint)
    | hb_filter (glyphset, hb_first)
    | hb_filter (subset_offset_array (c, out->attachPoint, આ), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;
    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (તાર્કિક (new_coverage));
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (coverage.sanitize (c, આ) && attachPoint.sanitize (c, આ));
  }

  સુરક્ષિત:
  Offset16To<Coverage>
		coverage;		/* Offset to Coverage table -- from
					 * beginning of AttachList table */
  Array16OfOffset16To<AttachPoint>
		attachPoint;		/* Array of AttachPoint tables
					 * in Coverage Index order */
  જાહેર:
  DEFINE_SIZE_ARRAY (4, attachPoint);
};

/*
 * Ligature Caret Table
 */

સંરચના Caret_કિંમત_Format1
{
  friend સંરચના Caret_કિંમત;
  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);
    auto *out = c->serializer->embed (આ);
    જો (unlikely (!out)) return_trace (ખોટું);
    return_trace (સાચું);
  }

  ખાનગી:
  hb_position_t get_caret_કિંમત (hb_font_t *font, hb_direction_t direction) અચલ
  {
    પાછા HB_DIRECTION_IS_HORIZONTAL (direction) ? font->em_scale_x (coordinate) : font->em_scale_y (coordinate);
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ));
  }

  સુરક્ષિત:
  HBUINT16	caret_કિંમત_Format;	/* Format identifier--format = 1 */
  FWORD		coordinate;		/* X or Y value, in design units */
  જાહેર:
  DEFINE_SIZE_STATIC (4);
};

સંરચના Caret_કિંમત_Format2
{
  friend સંરચના Caret_કિંમત;
  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);
    auto *out = c->serializer->embed (આ);
    જો (unlikely (!out)) return_trace (ખોટું);
    return_trace (સાચું);
  }

  ખાનગી:
  hb_position_t get_caret_કિંમત (hb_font_t *font, hb_direction_t direction, hb_codepoint_t glyph_id) અચલ
  {
    hb_position_t x, y;
    font->get_glyph_contour_point_for_origin (glyph_id, caret_કિંમત_Point, direction, &x, &y);
    પાછા HB_DIRECTION_IS_HORIZONTAL (direction) ? x : y;
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ));
  }

  સુરક્ષિત:
  HBUINT16	caret_કિંમત_Format;	/* Format identifier--format = 2 */
  HBUINT16	caret_કિંમત_Point;	/* Contour point index on glyph */
  જાહેર:
  DEFINE_SIZE_STATIC (4);
};

સંરચના Caret_કિંમત_Format3
{
  friend સંરચના Caret_કિંમત;

  hb_position_t get_caret_કિંમત (hb_font_t *font, hb_direction_t direction,
				 અચલ ItemVariationStore &var_store) અચલ
  {
    પાછા HB_DIRECTION_IS_HORIZONTAL (direction) ?
	   hb_saturate_add (font->em_scale_x (coordinate), (આ+deviceTable).get_x_delta (font, var_store)) :
	   hb_saturate_add (font->em_scale_y (coordinate), (આ+deviceTable).get_y_delta (font, var_store));
  }

  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);
    auto *out = c->serializer->start_embed (*આ);
    જો (!c->serializer->embed (caret_કિંમત_Format)) return_trace (ખોટું);
    જો (!c->serializer->embed (coordinate)) return_trace (ખોટું);

    unsigned varidx = (આ+deviceTable).get_variation_index ();
    hb_pair_t<unsigned, પૂર્ણાંક> *new_varidx_delta;
    જો (c->plan->layout_variation_idx_delta_map.has (varidx, &new_varidx_delta)) {
      uint32_t new_varidx = hb_first (*new_varidx_delta);
      પૂર્ણાંક delta = hb_second (*new_varidx_delta);
      જો (delta != 0)
      {
        જો (!c->serializer->check_assign (out->coordinate, coordinate + delta, HB_SERIALIZE_ERROR_INT_OVERFLOW))
          return_trace (ખોટું);
      }

      જો (new_varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
        return_trace (c->serializer->check_assign (out->caret_કિંમત_Format, 1, HB_SERIALIZE_ERROR_INT_OVERFLOW));
    }

    જો (!c->serializer->embed (deviceTable))
      return_trace (ખોટું);

    return_trace (out->deviceTable.serialize_copy (c->serializer, deviceTable, આ, c->serializer->to_bias (out),
						   hb_serialize_context_t::Head, &c->plan->layout_variation_idx_delta_map));
  }

  શૂન્ય collect_variation_indices (hb_collect_variation_indices_context_t *c) અચલ
  { (આ+deviceTable).collect_variation_indices (c); }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) && deviceTable.sanitize (c, આ));
  }

  સુરક્ષિત:
  HBUINT16	caret_કિંમત_Format;	/* Format identifier--format = 3 */
  FWORD		coordinate;		/* X or Y value, in design units */
  Offset16To<Device>
		deviceTable;		/* Offset to Device table for X or Y
					 * value--from beginning of CaretValue
					 * table */
  જાહેર:
  DEFINE_SIZE_STATIC (6);
};

સંરચના Caret_કિંમત
{
  hb_position_t get_caret_કિંમત (hb_font_t *font,
				 hb_direction_t direction,
				 hb_codepoint_t glyph_id,
				 અચલ ItemVariationStore &var_store) અચલ
  {
    પસંદગી (u.format.v) {
    સ્થિતિ 1: hb_barrier (); પાછા u.format1.get_caret_કિંમત (font, direction);
    સ્થિતિ 2: hb_barrier (); પાછા u.format2.get_caret_કિંમત (font, direction, glyph_id);
    સ્થિતિ 3: hb_barrier (); પાછા u.format3.get_caret_કિંમત (font, direction, var_store);
    મૂળભૂત:પાછા 0;
    }
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) અચલ
  {
    જો (unlikely (!c->may_dispatch (આ, &u.format.v))) પાછા c->no_dispatch_return_કિંમત ();
    TRACE_DISPATCH (આ, u.format.v);
    પસંદગી (u.format.v) {
    સ્થિતિ 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    સ્થિતિ 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    સ્થિતિ 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    મૂળભૂત:return_trace (c->default_return_કિંમત ());
    }
  }

  શૂન્ય collect_variation_indices (hb_collect_variation_indices_context_t *c) અચલ
  {
    પસંદગી (u.format.v) {
    સ્થિતિ 1:
    સ્થિતિ 2:
      પાછા;
    સ્થિતિ 3:
      hb_barrier ();
      u.format3.collect_variation_indices (c);
      પાછા;
    મૂળભૂત: પાછા;
    }
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    જો (!u.format.v.sanitize (c)) return_trace (ખોટું);
    hb_barrier ();
    પસંદગી (u.format.v) {
    સ્થિતિ 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    સ્થિતિ 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    સ્થિતિ 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    મૂળભૂત:return_trace (સાચું);
    }
  }

  સુરક્ષિત:
  સંઘ {
  સંરચના { HBUINT16 v; }	format;		/* Format identifier */
  Caret_કિંમત_Format1	format1;
  Caret_કિંમત_Format2	format2;
  Caret_કિંમત_Format3	format3;
  } u;
  જાહેર:
  DEFINE_SIZE_UNION (2, format.v);
};

સંરચના LigGlyph
{
  unsigned get_lig_carets (hb_font_t            *font,
			   hb_direction_t        direction,
			   hb_codepoint_t        glyph_id,
			   અચલ ItemVariationStore &var_store,
			   unsigned              start_offset,
			   unsigned             *caret_ગણતરી /* IN/OUT */,
			   hb_position_t        *caret_array /* OUT */) અચલ
  {
    જો (caret_ગણતરી && caret_array)
    {
      + carets.as_array ().sub_array (start_offset, caret_ગણતરી)
      | hb_map (hb_add (આ))
      | hb_map ([&] (અચલ Caret_કિંમત &value) { પાછા value.get_caret_કિંમત (font, direction, glyph_id, var_store); })
      | hb_sink (hb_array (caret_array, *caret_ગણતરી))
      ;
    }

    પાછા carets.len;
  }

  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);
    auto *out = c->serializer->start_embed (*આ);
    જો (unlikely (!c->serializer->extend_min (out))) return_trace (ખોટું);

    + hb_iter (carets)
    | hb_apply (subset_offset_array (c, out->carets, આ))
    ;

    return_trace (તાર્કિક (out->carets));
  }

  શૂન્ય collect_variation_indices (hb_collect_variation_indices_context_t *c) અચલ
  {
    માટે (અચલ Offset16To<Caret_કિંમત>& offset : carets.iter ())
      (આ+offset).collect_variation_indices (c);
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (carets.sanitize (c, આ));
  }

  સુરક્ષિત:
  Array16OfOffset16To<Caret_કિંમત>
		carets;			/* Offset array of CaretValue tables
					 * --from beginning of LigGlyph table
					 * --in increasing coordinate order */
  જાહેર:
  DEFINE_SIZE_ARRAY (2, carets);
};

/* LigCaretList2 widens the LigCaretList coverage, count, and LigGlyph
 * offsets. The LigGlyph table itself keeps Offset16 caretValueOffsets. */
template <typename Types>
સંરચના LigCaretList
{
  unsigned પૂર્ણાંક get_lig_carets (hb_font_t *font,
			       hb_direction_t direction,
			       hb_codepoint_t glyph_id,
			       અચલ ItemVariationStore &var_store,
			       unsigned પૂર્ણાંક start_offset,
			       unsigned પૂર્ણાંક *caret_ગણતરી /* IN/OUT */,
			       hb_position_t *caret_array /* OUT */) અચલ
  {
    unsigned પૂર્ણાંક index = (આ+coverage).get_coverage (glyph_id);
    જો (index == NOT_COVERED)
    {
      જો (caret_ગણતરી)
	*caret_ગણતરી = 0;
      પાછા 0;
    }
    અચલ LigGlyph &lig_glyph = આ+ligGlyph[index];
    પાછા lig_glyph.get_lig_carets (font, direction, glyph_id, var_store, start_offset, caret_ગણતરી, caret_array);
  }

  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);
    અચલ hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    અચલ hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*આ);
    જો (unlikely (!c->serializer->extend_min (out))) return_trace (ખોટું);

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (આ+coverage, ligGlyph)
    | hb_filter (glyphset, hb_first)
    | hb_filter (subset_offset_array (c, out->ligGlyph, આ), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;
    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (તાર્કિક (new_coverage));
  }

  શૂન્ય collect_variation_indices (hb_collect_variation_indices_context_t *c) અચલ
  {
    + hb_zip (આ+coverage, ligGlyph)
    | hb_filter (c->glyph_set, hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (આ))
    | hb_apply ([c] (અચલ LigGlyph& _) { _.collect_variation_indices (c); })
    ;
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (coverage.sanitize (c, આ) && ligGlyph.sanitize (c, આ));
  }

  સુરક્ષિત:
  typename Types::template LOffsetTo<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of LigCaretList table */
  typename Types::template ArrayOf<typename Types::template OffsetTo<LigGlyph>>
		ligGlyph;		/* Array of LigGlyph tables
					 * in Coverage Index order */
  જાહેર:
  DEFINE_SIZE_ARRAY (Types::LOffset::static_size + Types::HBUINT::static_size, ligGlyph);
};

#ifndef HB_NO_BEYOND_64K
static_assert (LigCaretList<MediumTypes>::min_size ==
	       Offset32::static_size + HBUINT24::static_size, "");
#endif


સંરચના MarkGlyphSetsFormat1
{
  તાર્કિક covers (unsigned પૂર્ણાંક set_index, hb_codepoint_t glyph_id) અચલ
  { પાછા (આ+coverage[set_index]).get_coverage (glyph_id) != NOT_COVERED; }

  શૂન્ય collect_used_mark_sets (અચલ hb_set_t& glyph_set,
                               hb_set_t& used_mark_sets /* OUT */) અચલ
  {
    unsigned i = 0;
    માટે (અચલ auto &offset : coverage)
     {
       અચલ auto &cov = આ+offset;
       જો (cov.intersects (&glyph_set))
         used_mark_sets.add (i);

       i++;
     }
  }

  template <typename set_t>
  શૂન્ય collect_coverage (hb_vector_t<set_t> &sets) અચલ
  {
     માટે (અચલ auto &offset : coverage)
     {
       અચલ auto &cov = આ+offset;
       cov.collect_coverage (sets.push ());
     }
  }

  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);
    auto *out = c->serializer->start_embed (*આ);
    જો (unlikely (!c->serializer->extend_min (out))) return_trace (ખોટું);
    out->format = format;

    તાર્કિક ret = સાચું;
    માટે (અચલ Offset32To<Coverage>& offset : coverage.iter ())
    {
      auto snap = c->serializer->snapshot ();
      auto *o = out->coverage.serialize_append (c->serializer);
      જો (unlikely (!o))
      {
	ret = ખોટું;
	તોડો;
      }

      //skip empty coverage
      c->serializer->push ();
      તાર્કિક res = ખોટું;
      જો (offset) res = c->dispatch (આ+offset);
      જો (!res)
      {
        c->serializer->કાઢો_discard ();
        c->serializer->revert (snap);
        (out->coverage.len)--;
        ચાલુ;
      }
      c->serializer->add_link (*o, c->serializer->કાઢો_pack ());
    }

    return_trace (ret && out->coverage.len);
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (coverage.sanitize (c, આ));
  }

  સુરક્ષિત:
  HBUINT16	format;			/* Format identifier--format = 1 */
  Array16Of<Offset32To<Coverage>>
		coverage;		/* Array of long offsets to mark set
					 * coverage tables */
  જાહેર:
  DEFINE_SIZE_ARRAY (4, coverage);
};

સંરચના MarkGlyphSets
{
  તાર્કિક covers (unsigned પૂર્ણાંક set_index, hb_codepoint_t glyph_id) અચલ
  {
    પસંદગી (u.format.v) {
    સ્થિતિ 1: hb_barrier (); પાછા u.format1.covers (set_index, glyph_id);
    મૂળભૂત:પાછા ખોટું;
    }
  }

  template <typename set_t>
  શૂન્ય collect_coverage (hb_vector_t<set_t> &sets) અચલ
  {
    પસંદગી (u.format.v) {
    સ્થિતિ 1: hb_barrier (); u.format1.collect_coverage (sets); પાછા;
    મૂળભૂત:પાછા;
    }
  }

  શૂન્ય collect_used_mark_sets (અચલ hb_set_t& glyph_set,
                               hb_set_t& used_mark_sets /* OUT */) અચલ
  {
    પસંદગી (u.format.v) {
    સ્થિતિ 1: hb_barrier (); u.format1.collect_used_mark_sets (glyph_set, used_mark_sets); પાછા;
    મૂળભૂત:પાછા;
    }
  }

  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);
    પસંદગી (u.format.v) {
    સ્થિતિ 1: hb_barrier (); return_trace (u.format1.subset (c));
    મૂળભૂત:return_trace (ખોટું);
    }
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    જો (!u.format.v.sanitize (c)) return_trace (ખોટું);
    hb_barrier ();
    પસંદગી (u.format.v) {
    સ્થિતિ 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    મૂળભૂત:return_trace (સાચું);
    }
  }

  સુરક્ષિત:
  સંઘ {
  સંરચના { HBUINT16 v; }	format;		/* Format identifier */
  MarkGlyphSetsFormat1	format1;
  } u;
  જાહેર:
  DEFINE_SIZE_UNION (2, format.v);
};


/*
 * GDEF -- Glyph Definition
 * https://docs.microsoft.com/en-us/typography/opentype/spec/gdef
 */


સંરચના GDEF
{
  સ્થિર constexpr hb_tag_t tableTag = HB_OT_TAG_GDEF;

  ગણના GlyphClasses {
    UnclassifiedGlyph	= 0,
    BaseGlyph		= 1,
    LigatureGlyph	= 2,
    MarkGlyph		= 3,
    ComponentGlyph	= 4
  };

  size_t get_size () અચલ
  {
    જો (version.major != 1) પાછા version.static_size;

    પાછા min_size +
	   (version.to_int () >= 0x00010002u ? markGlyphSetsDef.static_size : 0) +
	   (version.to_int () >= 0x00010003u ? varStore.static_size : 0) +
#ifndef HB_NO_BEYOND_64K
	   (version.to_int () >= 0x00010004u ? 5 * Offset32::static_size : 0)
#નહીંતર
	   0
#endif
	   ;
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    જો (unlikely (!version.sanitize (c) ||
		  version.major != 1 ||
		  !c->check_range (આ, get_size ())))
      return_trace (ખોટું);
    hb_barrier ();

    તાર્કિક glyph_class_def_ok = glyphClassDef.sanitize (c, આ);
    તાર્કિક attach_list_ok = attachList.sanitize (c, આ);
    તાર્કિક lig_caret_list_ok = ligCaretList.sanitize (c, આ);
    તાર્કિક mark_attach_class_def_ok = markAttachClassDef.sanitize (c, આ);
    તાર્કિક mark_glyph_sets_def_ok = version.to_int () < 0x00010002u ||
				  markGlyphSetsDef.sanitize (c, આ);

#ifndef HB_NO_BEYOND_64K
    જો (version.to_int () >= 0x00010004u)
    {
      જો (glyphClassDef2) glyph_class_def_ok = glyphClassDef2.sanitize (c, આ);
      જો (attachList2) attach_list_ok = attachList2.sanitize (c, આ);
      જો (ligCaretList2) lig_caret_list_ok = ligCaretList2.sanitize (c, આ);
      જો (markAttachClassDef2) mark_attach_class_def_ok = markAttachClassDef2.sanitize (c, આ);
      જો (markGlyphSetsDef2) mark_glyph_sets_def_ok = markGlyphSetsDef2.sanitize (c, આ);
    }
#endif

    return_trace (glyph_class_def_ok &&
		  attach_list_ok &&
		  lig_caret_list_ok &&
		  mark_attach_class_def_ok &&
		  mark_glyph_sets_def_ok &&
		  (version.to_int () < 0x00010003u || varStore.sanitize (c, આ)));
  }

  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);
    જો (version.major != 1) return_trace (ખોટું);

    auto *out = c->serializer->start_embed (*આ);
    જો (unlikely (!c->serializer->extend_min (out))) return_trace (ખોટું);

    // Push var store first (if it's needed) so that it's last in the
    // serialization order. Some font consumers assume that varstore runs to
    // the end of the GDEF table.
    // See: https://github.com/harfbuzz/harfbuzz/issues/4636
    auto snapshot_version0 = c->serializer->snapshot ();
    જો (unlikely (version.to_int () >= 0x00010002u && hb_barrier () &&
		  !c->serializer->embed (markGlyphSetsDef)))
      return_trace (ખોટું);

    auto snapshot_version2 = c->serializer->snapshot ();
    જો (unlikely (version.to_int () >= 0x00010003u && hb_barrier () &&
		  !c->serializer->embed (varStore)))
      return_trace (ખોટું);

    તાર્કિક use_v14 = ખોટું;
#ifndef HB_NO_BEYOND_64K
    use_v14 = version.to_int () >= 0x00010004u &&
	      (glyphClassDef2 || attachList2 || ligCaretList2 ||
	       markAttachClassDef2 || markGlyphSetsDef2);
    જો (unlikely (version.to_int () >= 0x00010004u && hb_barrier () &&
		  !c->serializer->extend_size (out, get_size ())))
      return_trace (ખોટું);
#endif

    તાર્કિક subset_varstore = ખોટું;
    unsigned varstore_index = (unsigned) -1;
    જો (version.to_int () >= 0x00010003u && hb_barrier ())
    {
      જો (c->plan->all_axes_pinned)
	out->varStore = 0;
      નહીંતર જો (c->plan->normalized_coords)
      {
	જો (varStore)
	{
	  item_variations_t item_vars;
	  જો (unlikely (!c->serializer->check_success (
                  item_vars.instantiate (આ+varStore, c->plan, સાચું, સાચું,
                                          c->plan->gdef_varstore_inner_maps.as_array (), સાચું))))
            return_trace (ખોટું);
	  subset_varstore = out->varStore.serialize_serialize (c->serializer,
							     item_vars.has_long_word (),
							     c->plan->axis_tags,
							     item_vars.get_region_list (),
							     item_vars.get_vardata_encodings ());
	  varstore_index = c->serializer->last_added_child_index();
	  remap_varidx_after_instantiation (item_vars.get_varidx_map (),
					    c->plan->layout_variation_idx_delta_map);
          remap_varidx_after_instantiation (item_vars.get_varidx_map (),
                                            c->plan->layout_condition_idx_કિંમત_map);
	}
      }
      નહીંતર
      {
	subset_varstore = out->varStore.serialize_subset (c, varStore, આ,
							  c->plan->gdef_varstore_inner_maps.as_array ());
	varstore_index = c->serializer->last_added_child_index();
      }
    }

    out->version.major = version.major;
    out->version.minor = version.minor;

    જો (!subset_varstore && version.to_int () >= 0x00010002u && !use_v14) {
      c->serializer->revert (snapshot_version2);
    }
    જો (!subset_varstore)
      out->varStore = 0;
    નહીંતર
      c->plan->has_gdef_varstore = સાચું;

    તાર્કિક subset_markglyphsetsdef = ખોટું;
    જો (version.to_int () >= 0x00010002u && hb_barrier ())
    {
#ifndef HB_NO_BEYOND_64K
      જો (use_v14 && markGlyphSetsDef2)
      {
	out->markGlyphSetsDef = 0;
	subset_markglyphsetsdef = serialize_subset_offset (c, out->markGlyphSetsDef2,
							   markGlyphSetsDef2, આ);
      }
      નહીંતર
#endif
      {
#ifndef HB_NO_BEYOND_64K
	જો (use_v14) out->markGlyphSetsDef2 = 0;
#endif
	subset_markglyphsetsdef = out->markGlyphSetsDef.serialize_subset (c, markGlyphSetsDef, આ);
      }
    }

    જો (use_v14)
    {
      out->version.minor = 4;
    } નહીંતર જો (subset_varstore) {
      out->version.minor = 3;
    } નહીંતર જો (subset_markglyphsetsdef) {
      out->version.minor = 2;
    } નહીંતર  {
      out->version.minor = 0;
      c->serializer->revert (snapshot_version0);
    }

    તાર્કિક subset_glyphclassdef = ખોટું;
    તાર્કિક subset_attachlist = ખોટું;
    તાર્કિક subset_markattachclassdef = ખોટું;
    તાર્કિક subset_ligcaretlist = ખોટું;

#ifndef HB_NO_BEYOND_64K
    જો (use_v14 && glyphClassDef2)
    {
      out->glyphClassDef = 0;
      subset_glyphclassdef = serialize_subset_offset (c, out->glyphClassDef2,
						      glyphClassDef2, આ, nullptr, ખોટું, સાચું);
    }
    નહીંતર
#endif
    {
#ifndef HB_NO_BEYOND_64K
      જો (use_v14) out->glyphClassDef2 = 0;
#endif
      subset_glyphclassdef = out->glyphClassDef.serialize_subset (c, glyphClassDef, આ, nullptr, ખોટું, સાચું);
    }

#ifndef HB_NO_BEYOND_64K
    જો (use_v14 && attachList2)
    {
      out->attachList = 0;
      subset_attachlist = serialize_subset_offset (c, out->attachList2, attachList2, આ);
    }
    નહીંતર
#endif
    {
#ifndef HB_NO_BEYOND_64K
      જો (use_v14) out->attachList2 = 0;
#endif
      subset_attachlist = out->attachList.serialize_subset (c, attachList, આ);
    }

#ifndef HB_NO_BEYOND_64K
    જો (use_v14 && markAttachClassDef2)
    {
      out->markAttachClassDef = 0;
      subset_markattachclassdef = serialize_subset_offset (c, out->markAttachClassDef2,
							   markAttachClassDef2, આ, nullptr, ખોટું, સાચું);
    }
    નહીંતર
#endif
    {
#ifndef HB_NO_BEYOND_64K
      જો (use_v14) out->markAttachClassDef2 = 0;
#endif
      subset_markattachclassdef = out->markAttachClassDef.serialize_subset (c, markAttachClassDef,
									   આ, nullptr, ખોટું, સાચું);
    }

#ifndef HB_NO_BEYOND_64K
    જો (use_v14 && ligCaretList2)
    {
      out->ligCaretList = 0;
      subset_ligcaretlist = serialize_subset_offset (c, out->ligCaretList2, ligCaretList2, આ);
    }
    નહીંતર
#endif
    {
#ifndef HB_NO_BEYOND_64K
      જો (use_v14) out->ligCaretList2 = 0;
#endif
      subset_ligcaretlist = out->ligCaretList.serialize_subset (c, ligCaretList, આ);
    }

    જો (subset_varstore && varstore_index != (unsigned) -1) {
      c->serializer->repack_last(varstore_index);
    }

    return_trace (subset_glyphclassdef || subset_attachlist ||
		  subset_ligcaretlist || subset_markattachclassdef ||
		  (out->version.to_int () >= 0x00010002u && subset_markglyphsetsdef) ||
		  (out->version.to_int () >= 0x00010003u && subset_varstore));
  }

  તાર્કિક has_glyph_classes () અચલ
  {
#ifndef HB_NO_BEYOND_64K
    જો (version.to_int () >= 0x00010004u && glyphClassDef2) પાછા સાચું;
#endif
    પાછા glyphClassDef != 0;
  }
  અચલ ClassDef &get_glyph_class_def () અચલ
  {
#ifndef HB_NO_BEYOND_64K
    જો (version.to_int () >= 0x00010004u && glyphClassDef2) પાછા આ+glyphClassDef2;
#endif
    પાછા આ+glyphClassDef;
  }
  તાર્કિક has_attach_list () અચલ
  {
#ifndef HB_NO_BEYOND_64K
    જો (version.to_int () >= 0x00010004u && attachList2) પાછા સાચું;
#endif
    પાછા attachList != 0;
  }
  અચલ AttachList &get_attach_list () અચલ
  {
#ifndef HB_NO_BEYOND_64K
    જો (version.to_int () >= 0x00010004u && attachList2) પાછા આ+attachList2;
#endif
    પાછા આ+attachList;
  }
  તાર્કિક has_lig_carets () અચલ
  {
#ifndef HB_NO_BEYOND_64K
    જો (version.to_int () >= 0x00010004u && ligCaretList2) પાછા સાચું;
#endif
    પાછા ligCaretList != 0;
  }
  તાર્કિક has_mark_attachment_types () અચલ
  {
#ifndef HB_NO_BEYOND_64K
    જો (version.to_int () >= 0x00010004u && markAttachClassDef2) પાછા સાચું;
#endif
    પાછા markAttachClassDef != 0;
  }
  અચલ ClassDef &get_mark_attach_class_def () અચલ
  {
#ifndef HB_NO_BEYOND_64K
    જો (version.to_int () >= 0x00010004u && markAttachClassDef2) પાછા આ+markAttachClassDef2;
#endif
    પાછા આ+markAttachClassDef;
  }
  તાર્કિક has_mark_glyph_sets () અચલ
  {
    જો (version.to_int () < 0x00010002u) પાછા ખોટું;
#ifndef HB_NO_BEYOND_64K
    જો (version.to_int () >= 0x00010004u && markGlyphSetsDef2) પાછા સાચું;
#endif
    પાછા markGlyphSetsDef != 0;
  }
  અચલ MarkGlyphSets &get_mark_glyph_sets () અચલ
  {
    જો (version.to_int () < 0x00010002u) પાછા Null(MarkGlyphSets);
#ifndef HB_NO_BEYOND_64K
    જો (version.to_int () >= 0x00010004u && markGlyphSetsDef2) પાછા આ+markGlyphSetsDef2;
#endif
    પાછા આ+markGlyphSetsDef;
  }
  તાર્કિક has_var_store () અચલ
  {
    પાછા version.to_int () >= 0x00010003u && hb_barrier () && varStore != 0;
  }
  અચલ ItemVariationStore &get_var_store () અચલ
  {
    પાછા version.to_int () >= 0x00010003u && hb_barrier () ? આ+varStore : Null(ItemVariationStore);
  }


  તાર્કિક has_data () અચલ { પાછા version.to_int (); }
  unsigned પૂર્ણાંક get_glyph_class (hb_codepoint_t glyph) અચલ
  { પાછા get_glyph_class_def ().get_class (glyph); }
  શૂન્ય get_glyphs_in_class (unsigned પૂર્ણાંક klass, hb_set_t *glyphs) અચલ
  { get_glyph_class_def ().collect_class (glyphs, klass); }

  unsigned પૂર્ણાંક get_mark_attachment_type (hb_codepoint_t glyph) અચલ
  { પાછા get_mark_attach_class_def ().get_class (glyph); }

  unsigned પૂર્ણાંક get_attach_points (hb_codepoint_t glyph_id,
				  unsigned પૂર્ણાંક start_offset,
				  unsigned પૂર્ણાંક *point_ગણતરી /* IN/OUT */,
				  unsigned પૂર્ણાંક *point_array /* OUT */) અચલ
  { પાછા get_attach_list ().get_attach_points (glyph_id, start_offset, point_ગણતરી, point_array); }

  unsigned પૂર્ણાંક get_lig_carets (hb_font_t *font,
			       hb_direction_t direction,
			       hb_codepoint_t glyph_id,
			       unsigned પૂર્ણાંક start_offset,
			       unsigned પૂર્ણાંક *caret_ગણતરી /* IN/OUT */,
			       hb_position_t *caret_array /* OUT */) અચલ
  {
#ifndef HB_NO_BEYOND_64K
    જો (version.to_int () >= 0x00010004u && ligCaretList2)
      પાછા (આ+ligCaretList2).get_lig_carets (font, direction, glyph_id, get_var_store (),
						  start_offset, caret_ગણતરી, caret_array);
#endif
    પાછા (આ+ligCaretList).get_lig_carets (font, direction, glyph_id, get_var_store (),
					       start_offset, caret_ગણતરી, caret_array);
  }

  તાર્કિક mark_set_covers (unsigned પૂર્ણાંક set_index, hb_codepoint_t glyph_id) અચલ
  { પાછા get_mark_glyph_sets ().covers (set_index, glyph_id); }

  /* glyph_props is a 16-bit integer where the lower 8-bit have bits representing
   * glyph class and other bits, and high 8-bit the mark attachment type (if any).
   * Not to be confused with lookup_props which is very similar. */
  unsigned પૂર્ણાંક get_glyph_props (hb_codepoint_t glyph) અચલ
  {
    unsigned પૂર્ણાંક klass = get_glyph_class (glyph);

    static_assert (((unsigned પૂર્ણાંક) HB_OT_LAYOUT_GLYPH_PROPS_BASE_GLYPH == (unsigned પૂર્ણાંક) LookupFlag::IgnoreBaseGlyphs), "");
    static_assert (((unsigned પૂર્ણાંક) HB_OT_LAYOUT_GLYPH_PROPS_LIGATURE == (unsigned પૂર્ણાંક) LookupFlag::IgnoreLigatures), "");
    static_assert (((unsigned પૂર્ણાંક) HB_OT_LAYOUT_GLYPH_PROPS_MARK == (unsigned પૂર્ણાંક) LookupFlag::IgnoreMarks), "");

    પસંદગી (klass) {
    મૂળભૂત:			પાછા HB_OT_LAYOUT_GLYPH_CLASS_UNCLASSIFIED;
    સ્થિતિ BaseGlyph:		પાછા HB_OT_LAYOUT_GLYPH_PROPS_BASE_GLYPH;
    સ્થિતિ LigatureGlyph:		પાછા HB_OT_LAYOUT_GLYPH_PROPS_LIGATURE;
    સ્થિતિ MarkGlyph:
	  klass = get_mark_attachment_type (glyph);
	  પાછા HB_OT_LAYOUT_GLYPH_PROPS_MARK | (klass << 8);
    }
  }

  HB_INTERNAL તાર્કિક is_blocklisted (hb_blob_t *blob,
				   hb_face_t *face) અચલ;

  સંરચના accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      table = hb_sanitize_context_t ().reference_table<GDEF> (face);
      જો (unlikely (table->is_blocklisted (table.get_blob (), face)))
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
	જો (!sets.in_error () &&
	    mark_glyph_set_bitmaps.resize (sets.length))
	  માટે (unsigned i = 0; i < sets.length; i++)
	  {
	    auto &bitmap = mark_glyph_set_bitmaps.arrayZ[i];
	    bitmap.bias = HB_SET_કિંમત_INVALID;
	    અચલ auto &set = sets.arrayZ[i];
	    જો (unlikely (set.in_error ())) ચાલુ;
	    hb_codepoint_t min_gid = HB_SET_કિંમત_INVALID;
	    hb_codepoint_t max_gid = HB_SET_કિંમત_INVALID;
	    hb_set_next (&set, &min_gid);
	    hb_set_previous (&set, &max_gid);
	    જો (max_gid == HB_SET_કિંમત_INVALID)
	      ચાલુ;
	    જો (max_gid - min_gid >= hb_bit_page_t::PAGE_BITS)
	    {
	      /* Too wide for one page; a digest pre-filter instead. */
	      માટે (hb_codepoint_t gid : set)
		bitmap.u.digest.add (gid);
	      ચાલુ;
	    }
	    bitmap.bias = min_gid;
	    માટે (hb_codepoint_t gid : set)
	      bitmap.u.page.add (gid - min_gid);
	  }
      }
#endif
    }
    ~accelerator_t () { table.destroy (); }

    unsigned પૂર્ણાંક get_glyph_props (hb_codepoint_t glyph) અચલ
    {
      unsigned v;

#ifndef HB_NO_GDEF_CACHE
      જો (glyph_props_cache.get (glyph, &v))
        પાછા v;
#endif

      v = table->get_glyph_props (glyph);

#ifndef HB_NO_GDEF_CACHE
      જો (likely (table.get_blob ())) // Don't try setting if we are the null instance!
	glyph_props_cache.set (glyph, v);
#endif

      પાછા v;

    }

    HB_ALWAYS_INLINE
    તાર્કિક mark_set_covers (unsigned પૂર્ણાંક set_index, hb_codepoint_t glyph_id) અચલ
    {
#ifndef HB_NO_GDEF_CACHE
      // We can access arrayZ directly because of sanitize_lookup_props() guarantee.
      અચલ auto &bitmap = mark_glyph_set_bitmaps.arrayZ[set_index];
      જો (likely (bitmap.bias != HB_SET_કિંમત_INVALID))
      {
	hb_codepoint_t biased = glyph_id - bitmap.bias;
	પાછા biased < hb_bit_page_t::PAGE_BITS &&
	       bitmap.u.page.get (biased);
      }
      પાછા mark_set_covers_slow (set_index, glyph_id);
#નહીંતર
      પાછા table->mark_set_covers (set_index, glyph_id);
#endif
    }

#ifndef HB_NO_GDEF_CACHE
    HB_NEVER_INLINE
    તાર્કિક mark_set_covers_slow (unsigned પૂર્ણાંક set_index, hb_codepoint_t glyph_id) અચલ
    {
      અચલ auto &bitmap = mark_glyph_set_bitmaps.arrayZ[set_index];
      જો (!bitmap.u.digest.may_have (glyph_id))
	પાછા ખોટું;
      પાછા table->mark_set_covers (set_index, glyph_id);
    }
#endif

    unsigned sanitize_lookup_props (unsigned lookup_props) અચલ
    {
#ifndef HB_NO_GDEF_CACHE
      જો (lookup_props & LookupFlag::UseMarkFilteringSet &&
	  (lookup_props >> 16) >= mark_glyph_set_bitmaps.length)
      {
        // Invalid mark filtering set index; unset the flag.
	lookup_props &= ~LookupFlag::UseMarkFilteringSet;
      }
#endif
      પાછા lookup_props;
    }

    hb_blob_ptr_t<GDEF> table;
#ifndef HB_NO_GDEF_CACHE
    સંરચના mark_glyph_set_bitmap_t
    {
      /* Gid of the first bit (not page-aligned) for an exact one-page
       * set, or HB_SET_VALUE_INVALID; then digest is a pre-filter for
       * the table.  The all-zeros state is valid for both members. */
      hb_codepoint_t bias = HB_SET_કિંમત_INVALID;
      સંઘ u_t {
	u_t () : page () {}
	hb_bit_page_t page;
	hb_set_digest_t digest;
      } u;
    };

    hb_vector_t<mark_glyph_set_bitmap_t> mark_glyph_set_bitmaps;
    mutable hb_cache_t<21, 3> glyph_props_cache;
    static_assert (કદ (glyph_props_cache) == 512, "");
#endif
  };

  શૂન્ય collect_variation_indices (hb_collect_variation_indices_context_t *c) અચલ
  {
#ifndef HB_NO_BEYOND_64K
    જો (version.to_int () >= 0x00010004u && ligCaretList2)
    {
      (આ+ligCaretList2).collect_variation_indices (c);
      પાછા;
    }
#endif
    (આ+ligCaretList).collect_variation_indices (c);
  }

  સુરક્ષિત:
  template <typename ચાવી>
  સ્થિર શૂન્ય remap_varidx_after_instantiation (અચલ hb_map_t& varidx_map,
                                                hb_hashmap_t<ચાવી, hb_pair_t<unsigned, પૂર્ણાંક>>& layout_variation_idx_delta_map /* IN/OUT */)
  {
    /* varidx_map is empty which means varstore is empty after instantiation,
     * no variations, map all varidx to HB_OT_LAYOUT_NO_VARIATIONS_INDEX.
     * varidx_map doesn't have original varidx, indicating delta row is all
     * zeros, map varidx to HB_OT_LAYOUT_NO_VARIATIONS_INDEX */
    માટે (auto _ : layout_variation_idx_delta_map.iter_ref ())
    {
      /* old_varidx->(varidx, delta) mapping generated for subsetting, then this
       * varidx is used as key of varidx_map during instantiation */
      uint32_t varidx = _.second.first;
      uint32_t *new_varidx;
      જો (varidx_map.has (varidx, &new_varidx))
	_.second.first = *new_varidx;
      નહીંતર
	_.second.first = HB_OT_LAYOUT_NO_VARIATIONS_INDEX;
    }
  }

  template <typename OffsetOut, typename OffsetIn, typename Base, typename ...Ts>
  સ્થિર તાર્કિક serialize_subset_offset (hb_subset_context_t *c,
				       OffsetOut& out,
				       અચલ OffsetIn& in,
				       અચલ Base *base,
				       Ts&&... ds)
  {
    out = 0;
    જો (in.is_null ()) પાછા ખોટું;

    auto *s = c->serializer;
    s->push ();
    તાર્કિક ret = c->dispatch (base+in, std::forward<Ts> (ds)...);
    જો (ret) s->add_link (out, s->કાઢો_pack ());
    નહીંતર s->કાઢો_discard ();
    પાછા ret;
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
  જાહેર:
  DEFINE_SIZE_MIN (12);
};

સંરચના GDEF_accelerator_t : GDEF::accelerator_t {
  GDEF_accelerator_t (hb_face_t *face) : GDEF::accelerator_t (face) {}
};

} /* namespace OT */


#endif /* OT_LAYOUT_GDEF_GDEF_HH */
