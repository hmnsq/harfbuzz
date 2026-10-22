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

#ಅನ್ತರ್ಭಾವ "../../../hb-ot-var-common.hh"

#ಅನ್ತರ್ಭಾವ "../../../hb-font.hh"
#ಅನ್ತರ್ಭಾವ "../../../hb-cache.hh"


ನಾಮಾಕಾಶ OT {


/*
 * Attachment List Table
 */

/* Array of contour point indices--in increasing numerical order */
ಸಂರಚನಾ AttachPoint : Array16Of<HBUINT16>
{
  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    auto *out = c->serializer->start_embed (*ಏತತ್);
    return_trace (out->serialize (c->serializer, + iter ()));
  }
};

/* AttachList is referenced by both GDEF attachListOffset and
 * attachListOffset2. The v1.4 top-level offset widens, but the
 * AttachList table itself keeps Offset16 internals. */
ಸಂರಚನಾ AttachList
{
  unsigned ಪೂರ್ಣಾಙ್ಕ get_attach_points (hb_codepoint_t glyph_id,
				  unsigned ಪೂರ್ಣಾಙ್ಕ start_offset,
				  unsigned ಪೂರ್ಣಾಙ್ಕ *point_ಎಣಿಕೆ /* IN/OUT */,
				  unsigned ಪೂರ್ಣಾಙ್ಕ *point_array /* OUT */) ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ index = (ಏತತ್+coverage).get_coverage (glyph_id);
    ಯದಿ (index == NOT_COVERED)
    {
      ಯದಿ (point_ಎಣಿಕೆ)
	*point_ಎಣಿಕೆ = 0;
      ಪ್ರತ್ಯಾಗಮ 0;
    }

    ಧ್ರುವ AttachPoint &points = ಏತತ್+attachPoint[index];

    ಯದಿ (point_ಎಣಿಕೆ && point_array)
    {
      + points.as_array ().sub_array (start_offset, point_ಎಣಿಕೆ)
      | hb_sink (hb_array (point_array, *point_ಎಣಿಕೆ))
      ;
    }

    ಪ್ರತ್ಯಾಗಮ points.len;
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    ಧ್ರುವ hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ಧ್ರುವ hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*ಏತತ್);
    ಯದಿ (unlikely (!c->serializer->extend_min (out))) return_trace (ಅಸತ್ಯ);

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (ಏತತ್+coverage, attachPoint)
    | hb_filter (glyphset, hb_first)
    | hb_filter (subset_offset_array (c, out->attachPoint, ಏತತ್), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;
    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (ತಾರ್ಕಿಕ (new_coverage));
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (coverage.sanitize (c, ಏತತ್) && attachPoint.sanitize (c, ಏತತ್));
  }

  ರಕ್ಷಿತ:
  Offset16To<Coverage>
		coverage;		/* Offset to Coverage table -- from
					 * beginning of AttachList table */
  Array16OfOffset16To<AttachPoint>
		attachPoint;		/* Array of AttachPoint tables
					 * in Coverage Index order */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (4, attachPoint);
};

/*
 * Ligature Caret Table
 */

ಸಂರಚನಾ Caret_ಮೌಲ್ಯ_Format1
{
  friend ಸಂರಚನಾ Caret_ಮೌಲ್ಯ;
  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    auto *out = c->serializer->embed (ಏತತ್);
    ಯದಿ (unlikely (!out)) return_trace (ಅಸತ್ಯ);
    return_trace (ಸತ್ಯ);
  }

  ಗುಪ್ತ:
  hb_position_t get_caret_ಮೌಲ್ಯ (hb_font_t *font, hb_direction_t direction) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ HB_DIRECTION_IS_HORIZONTAL (direction) ? font->em_scale_x (coordinate) : font->em_scale_y (coordinate);
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್));
  }

  ರಕ್ಷಿತ:
  HBUINT16	caret_ಮೌಲ್ಯ_Format;	/* Format identifier--format = 1 */
  FWORD		coordinate;		/* X or Y value, in design units */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (4);
};

ಸಂರಚನಾ Caret_ಮೌಲ್ಯ_Format2
{
  friend ಸಂರಚನಾ Caret_ಮೌಲ್ಯ;
  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    auto *out = c->serializer->embed (ಏತತ್);
    ಯದಿ (unlikely (!out)) return_trace (ಅಸತ್ಯ);
    return_trace (ಸತ್ಯ);
  }

  ಗುಪ್ತ:
  hb_position_t get_caret_ಮೌಲ್ಯ (hb_font_t *font, hb_direction_t direction, hb_codepoint_t glyph_id) ಧ್ರುವ
  {
    hb_position_t x, y;
    font->get_glyph_contour_point_for_origin (glyph_id, caret_ಮೌಲ್ಯ_Point, direction, &x, &y);
    ಪ್ರತ್ಯಾಗಮ HB_DIRECTION_IS_HORIZONTAL (direction) ? x : y;
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್));
  }

  ರಕ್ಷಿತ:
  HBUINT16	caret_ಮೌಲ್ಯ_Format;	/* Format identifier--format = 2 */
  HBUINT16	caret_ಮೌಲ್ಯ_Point;	/* Contour point index on glyph */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (4);
};

ಸಂರಚನಾ Caret_ಮೌಲ್ಯ_Format3
{
  friend ಸಂರಚನಾ Caret_ಮೌಲ್ಯ;

  hb_position_t get_caret_ಮೌಲ್ಯ (hb_font_t *font, hb_direction_t direction,
				 ಧ್ರುವ ItemVariationStore &var_store) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ HB_DIRECTION_IS_HORIZONTAL (direction) ?
	   hb_saturate_add (font->em_scale_x (coordinate), (ಏತತ್+deviceTable).get_x_delta (font, var_store)) :
	   hb_saturate_add (font->em_scale_y (coordinate), (ಏತತ್+deviceTable).get_y_delta (font, var_store));
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    auto *out = c->serializer->start_embed (*ಏತತ್);
    ಯದಿ (!c->serializer->embed (caret_ಮೌಲ್ಯ_Format)) return_trace (ಅಸತ್ಯ);
    ಯದಿ (!c->serializer->embed (coordinate)) return_trace (ಅಸತ್ಯ);

    unsigned varidx = (ಏತತ್+deviceTable).get_variation_index ();
    hb_pair_t<unsigned, ಪೂರ್ಣಾಙ್ಕ> *new_varidx_delta;
    ಯದಿ (c->plan->layout_variation_idx_delta_map.has (varidx, &new_varidx_delta)) {
      uint32_t new_varidx = hb_first (*new_varidx_delta);
      ಪೂರ್ಣಾಙ್ಕ delta = hb_second (*new_varidx_delta);
      ಯದಿ (delta != 0)
      {
        ಯದಿ (!c->serializer->check_assign (out->coordinate, coordinate + delta, HB_SERIALIZE_ERROR_INT_OVERFLOW))
          return_trace (ಅಸತ್ಯ);
      }

      ಯದಿ (new_varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
        return_trace (c->serializer->check_assign (out->caret_ಮೌಲ್ಯ_Format, 1, HB_SERIALIZE_ERROR_INT_OVERFLOW));
    }

    ಯದಿ (!c->serializer->embed (deviceTable))
      return_trace (ಅಸತ್ಯ);

    return_trace (out->deviceTable.serialize_copy (c->serializer, deviceTable, ಏತತ್, c->serializer->to_bias (out),
						   hb_serialize_context_t::Head, &c->plan->layout_variation_idx_delta_map));
  }

  ಶೂನ್ಯ collect_variation_indices (hb_collect_variation_indices_context_t *c) ಧ್ರುವ
  { (ಏತತ್+deviceTable).collect_variation_indices (c); }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) && deviceTable.sanitize (c, ಏತತ್));
  }

  ರಕ್ಷಿತ:
  HBUINT16	caret_ಮೌಲ್ಯ_Format;	/* Format identifier--format = 3 */
  FWORD		coordinate;		/* X or Y value, in design units */
  Offset16To<Device>
		deviceTable;		/* Offset to Device table for X or Y
					 * value--from beginning of CaretValue
					 * table */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (6);
};

ಸಂರಚನಾ Caret_ಮೌಲ್ಯ
{
  hb_position_t get_caret_ಮೌಲ್ಯ (hb_font_t *font,
				 hb_direction_t direction,
				 hb_codepoint_t glyph_id,
				 ಧ್ರುವ ItemVariationStore &var_store) ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ 1: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format1.get_caret_ಮೌಲ್ಯ (font, direction);
    ಪಕ್ಷ 2: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format2.get_caret_ಮೌಲ್ಯ (font, direction, glyph_id);
    ಪಕ್ಷ 3: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format3.get_caret_ಮೌಲ್ಯ (font, direction, var_store);
    ಸ್ವಭಾವ:ಪ್ರತ್ಯಾಗಮ 0;
    }
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ಧ್ರುವ
  {
    ಯದಿ (unlikely (!c->may_dispatch (ಏತತ್, &u.format.v))) ಪ್ರತ್ಯಾಗಮ c->no_dispatch_return_ಮೌಲ್ಯ ();
    TRACE_DISPATCH (ಏತತ್, u.format.v);
    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    ಪಕ್ಷ 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    ಪಕ್ಷ 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    ಸ್ವಭಾವ:return_trace (c->default_return_ಮೌಲ್ಯ ());
    }
  }

  ಶೂನ್ಯ collect_variation_indices (hb_collect_variation_indices_context_t *c) ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ 1:
    ಪಕ್ಷ 2:
      ಪ್ರತ್ಯಾಗಮ;
    ಪಕ್ಷ 3:
      hb_barrier ();
      u.format3.collect_variation_indices (c);
      ಪ್ರತ್ಯಾಗಮ;
    ಸ್ವಭಾವ: ಪ್ರತ್ಯಾಗಮ;
    }
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    ಯದಿ (!u.format.v.sanitize (c)) return_trace (ಅಸತ್ಯ);
    hb_barrier ();
    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    ಪಕ್ಷ 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    ಪಕ್ಷ 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    ಸ್ವಭಾವ:return_trace (ಸತ್ಯ);
    }
  }

  ರಕ್ಷಿತ:
  ಸಙ್ಘ {
  ಸಂರಚನಾ { HBUINT16 v; }	format;		/* Format identifier */
  Caret_ಮೌಲ್ಯ_Format1	format1;
  Caret_ಮೌಲ್ಯ_Format2	format2;
  Caret_ಮೌಲ್ಯ_Format3	format3;
  } u;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_UNION (2, format.v);
};

ಸಂರಚನಾ LigGlyph
{
  unsigned get_lig_carets (hb_font_t            *font,
			   hb_direction_t        direction,
			   hb_codepoint_t        glyph_id,
			   ಧ್ರುವ ItemVariationStore &var_store,
			   unsigned              start_offset,
			   unsigned             *caret_ಎಣಿಕೆ /* IN/OUT */,
			   hb_position_t        *caret_array /* OUT */) ಧ್ರುವ
  {
    ಯದಿ (caret_ಎಣಿಕೆ && caret_array)
    {
      + carets.as_array ().sub_array (start_offset, caret_ಎಣಿಕೆ)
      | hb_map (hb_add (ಏತತ್))
      | hb_map ([&] (ಧ್ರುವ Caret_ಮೌಲ್ಯ &value) { ಪ್ರತ್ಯಾಗಮ value.get_caret_ಮೌಲ್ಯ (font, direction, glyph_id, var_store); })
      | hb_sink (hb_array (caret_array, *caret_ಎಣಿಕೆ))
      ;
    }

    ಪ್ರತ್ಯಾಗಮ carets.len;
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    auto *out = c->serializer->start_embed (*ಏತತ್);
    ಯದಿ (unlikely (!c->serializer->extend_min (out))) return_trace (ಅಸತ್ಯ);

    + hb_iter (carets)
    | hb_apply (subset_offset_array (c, out->carets, ಏತತ್))
    ;

    return_trace (ತಾರ್ಕಿಕ (out->carets));
  }

  ಶೂನ್ಯ collect_variation_indices (hb_collect_variation_indices_context_t *c) ಧ್ರುವ
  {
    ಪ್ರತಿ (ಧ್ರುವ Offset16To<Caret_ಮೌಲ್ಯ>& offset : carets.iter ())
      (ಏತತ್+offset).collect_variation_indices (c);
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (carets.sanitize (c, ಏತತ್));
  }

  ರಕ್ಷಿತ:
  Array16OfOffset16To<Caret_ಮೌಲ್ಯ>
		carets;			/* Offset array of CaretValue tables
					 * --from beginning of LigGlyph table
					 * --in increasing coordinate order */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (2, carets);
};

/* LigCaretList2 widens the LigCaretList coverage, count, and LigGlyph
 * offsets. The LigGlyph table itself keeps Offset16 caretValueOffsets. */
template <typename Types>
ಸಂರಚನಾ LigCaretList
{
  unsigned ಪೂರ್ಣಾಙ್ಕ get_lig_carets (hb_font_t *font,
			       hb_direction_t direction,
			       hb_codepoint_t glyph_id,
			       ಧ್ರುವ ItemVariationStore &var_store,
			       unsigned ಪೂರ್ಣಾಙ್ಕ start_offset,
			       unsigned ಪೂರ್ಣಾಙ್ಕ *caret_ಎಣಿಕೆ /* IN/OUT */,
			       hb_position_t *caret_array /* OUT */) ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ index = (ಏತತ್+coverage).get_coverage (glyph_id);
    ಯದಿ (index == NOT_COVERED)
    {
      ಯದಿ (caret_ಎಣಿಕೆ)
	*caret_ಎಣಿಕೆ = 0;
      ಪ್ರತ್ಯಾಗಮ 0;
    }
    ಧ್ರುವ LigGlyph &lig_glyph = ಏತತ್+ligGlyph[index];
    ಪ್ರತ್ಯಾಗಮ lig_glyph.get_lig_carets (font, direction, glyph_id, var_store, start_offset, caret_ಎಣಿಕೆ, caret_array);
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    ಧ್ರುವ hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ಧ್ರುವ hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*ಏತತ್);
    ಯದಿ (unlikely (!c->serializer->extend_min (out))) return_trace (ಅಸತ್ಯ);

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (ಏತತ್+coverage, ligGlyph)
    | hb_filter (glyphset, hb_first)
    | hb_filter (subset_offset_array (c, out->ligGlyph, ಏತತ್), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;
    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (ತಾರ್ಕಿಕ (new_coverage));
  }

  ಶೂನ್ಯ collect_variation_indices (hb_collect_variation_indices_context_t *c) ಧ್ರುವ
  {
    + hb_zip (ಏತತ್+coverage, ligGlyph)
    | hb_filter (c->glyph_set, hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (ಏತತ್))
    | hb_apply ([c] (ಧ್ರುವ LigGlyph& _) { _.collect_variation_indices (c); })
    ;
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (coverage.sanitize (c, ಏತತ್) && ligGlyph.sanitize (c, ಏತತ್));
  }

  ರಕ್ಷಿತ:
  typename Types::template LOffsetTo<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of LigCaretList table */
  typename Types::template ArrayOf<typename Types::template OffsetTo<LigGlyph>>
		ligGlyph;		/* Array of LigGlyph tables
					 * in Coverage Index order */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (Types::LOffset::static_size + Types::HBUINT::static_size, ligGlyph);
};

#ifndef HB_NO_BEYOND_64K
static_assert (LigCaretList<MediumTypes>::min_size ==
	       Offset32::static_size + HBUINT24::static_size, "");
#endif


ಸಂರಚನಾ MarkGlyphSetsFormat1
{
  ತಾರ್ಕಿಕ covers (unsigned ಪೂರ್ಣಾಙ್ಕ set_index, hb_codepoint_t glyph_id) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ (ಏತತ್+coverage[set_index]).get_coverage (glyph_id) != NOT_COVERED; }

  ಶೂನ್ಯ collect_used_mark_sets (ಧ್ರುವ hb_set_t& glyph_set,
                               hb_set_t& used_mark_sets /* OUT */) ಧ್ರುವ
  {
    unsigned i = 0;
    ಪ್ರತಿ (ಧ್ರುವ auto &offset : coverage)
     {
       ಧ್ರುವ auto &cov = ಏತತ್+offset;
       ಯದಿ (cov.intersects (&glyph_set))
         used_mark_sets.add (i);

       i++;
     }
  }

  template <typename set_t>
  ಶೂನ್ಯ collect_coverage (hb_vector_t<set_t> &sets) ಧ್ರುವ
  {
     ಪ್ರತಿ (ಧ್ರುವ auto &offset : coverage)
     {
       ಧ್ರುವ auto &cov = ಏತತ್+offset;
       cov.collect_coverage (sets.push ());
     }
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    auto *out = c->serializer->start_embed (*ಏತತ್);
    ಯದಿ (unlikely (!c->serializer->extend_min (out))) return_trace (ಅಸತ್ಯ);
    out->format = format;

    ತಾರ್ಕಿಕ ret = ಸತ್ಯ;
    ಪ್ರತಿ (ಧ್ರುವ Offset32To<Coverage>& offset : coverage.iter ())
    {
      auto snap = c->serializer->snapshot ();
      auto *o = out->coverage.serialize_append (c->serializer);
      ಯದಿ (unlikely (!o))
      {
	ret = ಅಸತ್ಯ;
	ಭಙ್ಗ;
      }

      //skip empty coverage
      c->serializer->push ();
      ತಾರ್ಕಿಕ res = ಅಸತ್ಯ;
      ಯದಿ (offset) res = c->dispatch (ಏತತ್+offset);
      ಯದಿ (!res)
      {
        c->serializer->ತೆಗೆ_discard ();
        c->serializer->revert (snap);
        (out->coverage.len)--;
        ಅನುವರ್ತ;
      }
      c->serializer->add_link (*o, c->serializer->ತೆಗೆ_pack ());
    }

    return_trace (ret && out->coverage.len);
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (coverage.sanitize (c, ಏತತ್));
  }

  ರಕ್ಷಿತ:
  HBUINT16	format;			/* Format identifier--format = 1 */
  Array16Of<Offset32To<Coverage>>
		coverage;		/* Array of long offsets to mark set
					 * coverage tables */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (4, coverage);
};

ಸಂರಚನಾ MarkGlyphSets
{
  ತಾರ್ಕಿಕ covers (unsigned ಪೂರ್ಣಾಙ್ಕ set_index, hb_codepoint_t glyph_id) ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ 1: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format1.covers (set_index, glyph_id);
    ಸ್ವಭಾವ:ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
  }

  template <typename set_t>
  ಶೂನ್ಯ collect_coverage (hb_vector_t<set_t> &sets) ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ 1: hb_barrier (); u.format1.collect_coverage (sets); ಪ್ರತ್ಯಾಗಮ;
    ಸ್ವಭಾವ:ಪ್ರತ್ಯಾಗಮ;
    }
  }

  ಶೂನ್ಯ collect_used_mark_sets (ಧ್ರುವ hb_set_t& glyph_set,
                               hb_set_t& used_mark_sets /* OUT */) ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ 1: hb_barrier (); u.format1.collect_used_mark_sets (glyph_set, used_mark_sets); ಪ್ರತ್ಯಾಗಮ;
    ಸ್ವಭಾವ:ಪ್ರತ್ಯಾಗಮ;
    }
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ 1: hb_barrier (); return_trace (u.format1.subset (c));
    ಸ್ವಭಾವ:return_trace (ಅಸತ್ಯ);
    }
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    ಯದಿ (!u.format.v.sanitize (c)) return_trace (ಅಸತ್ಯ);
    hb_barrier ();
    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    ಸ್ವಭಾವ:return_trace (ಸತ್ಯ);
    }
  }

  ರಕ್ಷಿತ:
  ಸಙ್ಘ {
  ಸಂರಚನಾ { HBUINT16 v; }	format;		/* Format identifier */
  MarkGlyphSetsFormat1	format1;
  } u;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_UNION (2, format.v);
};


/*
 * GDEF -- Glyph Definition
 * https://docs.microsoft.com/en-us/typography/opentype/spec/gdef
 */


ಸಂರಚನಾ GDEF
{
  ಸ್ಥಿರ constexpr hb_tag_t tableTag = HB_OT_TAG_GDEF;

  ಗಣನಾ GlyphClasses {
    UnclassifiedGlyph	= 0,
    BaseGlyph		= 1,
    LigatureGlyph	= 2,
    MarkGlyph		= 3,
    ComponentGlyph	= 4
  };

  size_t get_size () ಧ್ರುವ
  {
    ಯದಿ (version.major != 1) ಪ್ರತ್ಯಾಗಮ version.static_size;

    ಪ್ರತ್ಯಾಗಮ min_size +
	   (version.to_int () >= 0x00010002u ? markGlyphSetsDef.static_size : 0) +
	   (version.to_int () >= 0x00010003u ? varStore.static_size : 0) +
#ifndef HB_NO_BEYOND_64K
	   (version.to_int () >= 0x00010004u ? 5 * Offset32::static_size : 0)
#ಅನ್ಯಥಾ
	   0
#endif
	   ;
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    ಯದಿ (unlikely (!version.sanitize (c) ||
		  version.major != 1 ||
		  !c->check_range (ಏತತ್, get_size ())))
      return_trace (ಅಸತ್ಯ);
    hb_barrier ();

    ತಾರ್ಕಿಕ glyph_class_def_ok = glyphClassDef.sanitize (c, ಏತತ್);
    ತಾರ್ಕಿಕ attach_list_ok = attachList.sanitize (c, ಏತತ್);
    ತಾರ್ಕಿಕ lig_caret_list_ok = ligCaretList.sanitize (c, ಏತತ್);
    ತಾರ್ಕಿಕ mark_attach_class_def_ok = markAttachClassDef.sanitize (c, ಏತತ್);
    ತಾರ್ಕಿಕ mark_glyph_sets_def_ok = version.to_int () < 0x00010002u ||
				  markGlyphSetsDef.sanitize (c, ಏತತ್);

#ifndef HB_NO_BEYOND_64K
    ಯದಿ (version.to_int () >= 0x00010004u)
    {
      ಯದಿ (glyphClassDef2) glyph_class_def_ok = glyphClassDef2.sanitize (c, ಏತತ್);
      ಯದಿ (attachList2) attach_list_ok = attachList2.sanitize (c, ಏತತ್);
      ಯದಿ (ligCaretList2) lig_caret_list_ok = ligCaretList2.sanitize (c, ಏತತ್);
      ಯದಿ (markAttachClassDef2) mark_attach_class_def_ok = markAttachClassDef2.sanitize (c, ಏತತ್);
      ಯದಿ (markGlyphSetsDef2) mark_glyph_sets_def_ok = markGlyphSetsDef2.sanitize (c, ಏತತ್);
    }
#endif

    return_trace (glyph_class_def_ok &&
		  attach_list_ok &&
		  lig_caret_list_ok &&
		  mark_attach_class_def_ok &&
		  mark_glyph_sets_def_ok &&
		  (version.to_int () < 0x00010003u || varStore.sanitize (c, ಏತತ್)));
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    ಯದಿ (version.major != 1) return_trace (ಅಸತ್ಯ);

    auto *out = c->serializer->start_embed (*ಏತತ್);
    ಯದಿ (unlikely (!c->serializer->extend_min (out))) return_trace (ಅಸತ್ಯ);

    // Push var store first (if it's needed) so that it's last in the
    // serialization order. Some font consumers assume that varstore runs to
    // the end of the GDEF table.
    // See: https://github.com/harfbuzz/harfbuzz/issues/4636
    auto snapshot_version0 = c->serializer->snapshot ();
    ಯದಿ (unlikely (version.to_int () >= 0x00010002u && hb_barrier () &&
		  !c->serializer->embed (markGlyphSetsDef)))
      return_trace (ಅಸತ್ಯ);

    auto snapshot_version2 = c->serializer->snapshot ();
    ಯದಿ (unlikely (version.to_int () >= 0x00010003u && hb_barrier () &&
		  !c->serializer->embed (varStore)))
      return_trace (ಅಸತ್ಯ);

    ತಾರ್ಕಿಕ use_v14 = ಅಸತ್ಯ;
#ifndef HB_NO_BEYOND_64K
    use_v14 = version.to_int () >= 0x00010004u &&
	      (glyphClassDef2 || attachList2 || ligCaretList2 ||
	       markAttachClassDef2 || markGlyphSetsDef2);
    ಯದಿ (unlikely (version.to_int () >= 0x00010004u && hb_barrier () &&
		  !c->serializer->extend_size (out, get_size ())))
      return_trace (ಅಸತ್ಯ);
#endif

    ತಾರ್ಕಿಕ subset_varstore = ಅಸತ್ಯ;
    unsigned varstore_index = (unsigned) -1;
    ಯದಿ (version.to_int () >= 0x00010003u && hb_barrier ())
    {
      ಯದಿ (c->plan->all_axes_pinned)
	out->varStore = 0;
      ಅನ್ಯಥಾ ಯದಿ (c->plan->normalized_coords)
      {
	ಯದಿ (varStore)
	{
	  item_variations_t item_vars;
	  ಯದಿ (unlikely (!c->serializer->check_success (
                  item_vars.instantiate (ಏತತ್+varStore, c->plan, ಸತ್ಯ, ಸತ್ಯ,
                                          c->plan->gdef_varstore_inner_maps.as_array (), ಸತ್ಯ))))
            return_trace (ಅಸತ್ಯ);
	  subset_varstore = out->varStore.serialize_serialize (c->serializer,
							     item_vars.has_long_word (),
							     c->plan->axis_tags,
							     item_vars.get_region_list (),
							     item_vars.get_vardata_encodings ());
	  varstore_index = c->serializer->last_added_child_index();
	  remap_varidx_after_instantiation (item_vars.get_varidx_map (),
					    c->plan->layout_variation_idx_delta_map);
          remap_varidx_after_instantiation (item_vars.get_varidx_map (),
                                            c->plan->layout_condition_idx_ಮೌಲ್ಯ_map);
	}
      }
      ಅನ್ಯಥಾ
      {
	subset_varstore = out->varStore.serialize_subset (c, varStore, ಏತತ್,
							  c->plan->gdef_varstore_inner_maps.as_array ());
	varstore_index = c->serializer->last_added_child_index();
      }
    }

    out->version.major = version.major;
    out->version.minor = version.minor;

    ಯದಿ (!subset_varstore && version.to_int () >= 0x00010002u && !use_v14) {
      c->serializer->revert (snapshot_version2);
    }
    ಯದಿ (!subset_varstore)
      out->varStore = 0;
    ಅನ್ಯಥಾ
      c->plan->has_gdef_varstore = ಸತ್ಯ;

    ತಾರ್ಕಿಕ subset_markglyphsetsdef = ಅಸತ್ಯ;
    ಯದಿ (version.to_int () >= 0x00010002u && hb_barrier ())
    {
#ifndef HB_NO_BEYOND_64K
      ಯದಿ (use_v14 && markGlyphSetsDef2)
      {
	out->markGlyphSetsDef = 0;
	subset_markglyphsetsdef = serialize_subset_offset (c, out->markGlyphSetsDef2,
							   markGlyphSetsDef2, ಏತತ್);
      }
      ಅನ್ಯಥಾ
#endif
      {
#ifndef HB_NO_BEYOND_64K
	ಯದಿ (use_v14) out->markGlyphSetsDef2 = 0;
#endif
	subset_markglyphsetsdef = out->markGlyphSetsDef.serialize_subset (c, markGlyphSetsDef, ಏತತ್);
      }
    }

    ಯದಿ (use_v14)
    {
      out->version.minor = 4;
    } ಅನ್ಯಥಾ ಯದಿ (subset_varstore) {
      out->version.minor = 3;
    } ಅನ್ಯಥಾ ಯದಿ (subset_markglyphsetsdef) {
      out->version.minor = 2;
    } ಅನ್ಯಥಾ  {
      out->version.minor = 0;
      c->serializer->revert (snapshot_version0);
    }

    ತಾರ್ಕಿಕ subset_glyphclassdef = ಅಸತ್ಯ;
    ತಾರ್ಕಿಕ subset_attachlist = ಅಸತ್ಯ;
    ತಾರ್ಕಿಕ subset_markattachclassdef = ಅಸತ್ಯ;
    ತಾರ್ಕಿಕ subset_ligcaretlist = ಅಸತ್ಯ;

#ifndef HB_NO_BEYOND_64K
    ಯದಿ (use_v14 && glyphClassDef2)
    {
      out->glyphClassDef = 0;
      subset_glyphclassdef = serialize_subset_offset (c, out->glyphClassDef2,
						      glyphClassDef2, ಏತತ್, nullptr, ಅಸತ್ಯ, ಸತ್ಯ);
    }
    ಅನ್ಯಥಾ
#endif
    {
#ifndef HB_NO_BEYOND_64K
      ಯದಿ (use_v14) out->glyphClassDef2 = 0;
#endif
      subset_glyphclassdef = out->glyphClassDef.serialize_subset (c, glyphClassDef, ಏತತ್, nullptr, ಅಸತ್ಯ, ಸತ್ಯ);
    }

#ifndef HB_NO_BEYOND_64K
    ಯದಿ (use_v14 && attachList2)
    {
      out->attachList = 0;
      subset_attachlist = serialize_subset_offset (c, out->attachList2, attachList2, ಏತತ್);
    }
    ಅನ್ಯಥಾ
#endif
    {
#ifndef HB_NO_BEYOND_64K
      ಯದಿ (use_v14) out->attachList2 = 0;
#endif
      subset_attachlist = out->attachList.serialize_subset (c, attachList, ಏತತ್);
    }

#ifndef HB_NO_BEYOND_64K
    ಯದಿ (use_v14 && markAttachClassDef2)
    {
      out->markAttachClassDef = 0;
      subset_markattachclassdef = serialize_subset_offset (c, out->markAttachClassDef2,
							   markAttachClassDef2, ಏತತ್, nullptr, ಅಸತ್ಯ, ಸತ್ಯ);
    }
    ಅನ್ಯಥಾ
#endif
    {
#ifndef HB_NO_BEYOND_64K
      ಯದಿ (use_v14) out->markAttachClassDef2 = 0;
#endif
      subset_markattachclassdef = out->markAttachClassDef.serialize_subset (c, markAttachClassDef,
									   ಏತತ್, nullptr, ಅಸತ್ಯ, ಸತ್ಯ);
    }

#ifndef HB_NO_BEYOND_64K
    ಯದಿ (use_v14 && ligCaretList2)
    {
      out->ligCaretList = 0;
      subset_ligcaretlist = serialize_subset_offset (c, out->ligCaretList2, ligCaretList2, ಏತತ್);
    }
    ಅನ್ಯಥಾ
#endif
    {
#ifndef HB_NO_BEYOND_64K
      ಯದಿ (use_v14) out->ligCaretList2 = 0;
#endif
      subset_ligcaretlist = out->ligCaretList.serialize_subset (c, ligCaretList, ಏತತ್);
    }

    ಯದಿ (subset_varstore && varstore_index != (unsigned) -1) {
      c->serializer->repack_last(varstore_index);
    }

    return_trace (subset_glyphclassdef || subset_attachlist ||
		  subset_ligcaretlist || subset_markattachclassdef ||
		  (out->version.to_int () >= 0x00010002u && subset_markglyphsetsdef) ||
		  (out->version.to_int () >= 0x00010003u && subset_varstore));
  }

  ತಾರ್ಕಿಕ has_glyph_classes () ಧ್ರುವ
  {
#ifndef HB_NO_BEYOND_64K
    ಯದಿ (version.to_int () >= 0x00010004u && glyphClassDef2) ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
#endif
    ಪ್ರತ್ಯಾಗಮ glyphClassDef != 0;
  }
  ಧ್ರುವ ClassDef &get_glyph_class_def () ಧ್ರುವ
  {
#ifndef HB_NO_BEYOND_64K
    ಯದಿ (version.to_int () >= 0x00010004u && glyphClassDef2) ಪ್ರತ್ಯಾಗಮ ಏತತ್+glyphClassDef2;
#endif
    ಪ್ರತ್ಯಾಗಮ ಏತತ್+glyphClassDef;
  }
  ತಾರ್ಕಿಕ has_attach_list () ಧ್ರುವ
  {
#ifndef HB_NO_BEYOND_64K
    ಯದಿ (version.to_int () >= 0x00010004u && attachList2) ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
#endif
    ಪ್ರತ್ಯಾಗಮ attachList != 0;
  }
  ಧ್ರುವ AttachList &get_attach_list () ಧ್ರುವ
  {
#ifndef HB_NO_BEYOND_64K
    ಯದಿ (version.to_int () >= 0x00010004u && attachList2) ಪ್ರತ್ಯಾಗಮ ಏತತ್+attachList2;
#endif
    ಪ್ರತ್ಯಾಗಮ ಏತತ್+attachList;
  }
  ತಾರ್ಕಿಕ has_lig_carets () ಧ್ರುವ
  {
#ifndef HB_NO_BEYOND_64K
    ಯದಿ (version.to_int () >= 0x00010004u && ligCaretList2) ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
#endif
    ಪ್ರತ್ಯಾಗಮ ligCaretList != 0;
  }
  ತಾರ್ಕಿಕ has_mark_attachment_types () ಧ್ರುವ
  {
#ifndef HB_NO_BEYOND_64K
    ಯದಿ (version.to_int () >= 0x00010004u && markAttachClassDef2) ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
#endif
    ಪ್ರತ್ಯಾಗಮ markAttachClassDef != 0;
  }
  ಧ್ರುವ ClassDef &get_mark_attach_class_def () ಧ್ರುವ
  {
#ifndef HB_NO_BEYOND_64K
    ಯದಿ (version.to_int () >= 0x00010004u && markAttachClassDef2) ಪ್ರತ್ಯಾಗಮ ಏತತ್+markAttachClassDef2;
#endif
    ಪ್ರತ್ಯಾಗಮ ಏತತ್+markAttachClassDef;
  }
  ತಾರ್ಕಿಕ has_mark_glyph_sets () ಧ್ರುವ
  {
    ಯದಿ (version.to_int () < 0x00010002u) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
#ifndef HB_NO_BEYOND_64K
    ಯದಿ (version.to_int () >= 0x00010004u && markGlyphSetsDef2) ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
#endif
    ಪ್ರತ್ಯಾಗಮ markGlyphSetsDef != 0;
  }
  ಧ್ರುವ MarkGlyphSets &get_mark_glyph_sets () ಧ್ರುವ
  {
    ಯದಿ (version.to_int () < 0x00010002u) ಪ್ರತ್ಯಾಗಮ Null(MarkGlyphSets);
#ifndef HB_NO_BEYOND_64K
    ಯದಿ (version.to_int () >= 0x00010004u && markGlyphSetsDef2) ಪ್ರತ್ಯಾಗಮ ಏತತ್+markGlyphSetsDef2;
#endif
    ಪ್ರತ್ಯಾಗಮ ಏತತ್+markGlyphSetsDef;
  }
  ತಾರ್ಕಿಕ has_var_store () ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ version.to_int () >= 0x00010003u && hb_barrier () && varStore != 0;
  }
  ಧ್ರುವ ItemVariationStore &get_var_store () ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ version.to_int () >= 0x00010003u && hb_barrier () ? ಏತತ್+varStore : Null(ItemVariationStore);
  }


  ತಾರ್ಕಿಕ has_data () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ version.to_int (); }
  unsigned ಪೂರ್ಣಾಙ್ಕ get_glyph_class (hb_codepoint_t glyph) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ get_glyph_class_def ().get_class (glyph); }
  ಶೂನ್ಯ get_glyphs_in_class (unsigned ಪೂರ್ಣಾಙ್ಕ klass, hb_set_t *glyphs) ಧ್ರುವ
  { get_glyph_class_def ().collect_class (glyphs, klass); }

  unsigned ಪೂರ್ಣಾಙ್ಕ get_mark_attachment_type (hb_codepoint_t glyph) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ get_mark_attach_class_def ().get_class (glyph); }

  unsigned ಪೂರ್ಣಾಙ್ಕ get_attach_points (hb_codepoint_t glyph_id,
				  unsigned ಪೂರ್ಣಾಙ್ಕ start_offset,
				  unsigned ಪೂರ್ಣಾಙ್ಕ *point_ಎಣಿಕೆ /* IN/OUT */,
				  unsigned ಪೂರ್ಣಾಙ್ಕ *point_array /* OUT */) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ get_attach_list ().get_attach_points (glyph_id, start_offset, point_ಎಣಿಕೆ, point_array); }

  unsigned ಪೂರ್ಣಾಙ್ಕ get_lig_carets (hb_font_t *font,
			       hb_direction_t direction,
			       hb_codepoint_t glyph_id,
			       unsigned ಪೂರ್ಣಾಙ್ಕ start_offset,
			       unsigned ಪೂರ್ಣಾಙ್ಕ *caret_ಎಣಿಕೆ /* IN/OUT */,
			       hb_position_t *caret_array /* OUT */) ಧ್ರುವ
  {
#ifndef HB_NO_BEYOND_64K
    ಯದಿ (version.to_int () >= 0x00010004u && ligCaretList2)
      ಪ್ರತ್ಯಾಗಮ (ಏತತ್+ligCaretList2).get_lig_carets (font, direction, glyph_id, get_var_store (),
						  start_offset, caret_ಎಣಿಕೆ, caret_array);
#endif
    ಪ್ರತ್ಯಾಗಮ (ಏತತ್+ligCaretList).get_lig_carets (font, direction, glyph_id, get_var_store (),
					       start_offset, caret_ಎಣಿಕೆ, caret_array);
  }

  ತಾರ್ಕಿಕ mark_set_covers (unsigned ಪೂರ್ಣಾಙ್ಕ set_index, hb_codepoint_t glyph_id) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ get_mark_glyph_sets ().covers (set_index, glyph_id); }

  /* glyph_props is a 16-bit integer where the lower 8-bit have bits representing
   * glyph class and other bits, and high 8-bit the mark attachment type (if any).
   * Not to be confused with lookup_props which is very similar. */
  unsigned ಪೂರ್ಣಾಙ್ಕ get_glyph_props (hb_codepoint_t glyph) ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ klass = get_glyph_class (glyph);

    static_assert (((unsigned ಪೂರ್ಣಾಙ್ಕ) HB_OT_LAYOUT_GLYPH_PROPS_BASE_GLYPH == (unsigned ಪೂರ್ಣಾಙ್ಕ) LookupFlag::IgnoreBaseGlyphs), "");
    static_assert (((unsigned ಪೂರ್ಣಾಙ್ಕ) HB_OT_LAYOUT_GLYPH_PROPS_LIGATURE == (unsigned ಪೂರ್ಣಾಙ್ಕ) LookupFlag::IgnoreLigatures), "");
    static_assert (((unsigned ಪೂರ್ಣಾಙ್ಕ) HB_OT_LAYOUT_GLYPH_PROPS_MARK == (unsigned ಪೂರ್ಣಾಙ್ಕ) LookupFlag::IgnoreMarks), "");

    ವಿಕಲ್ಪ (klass) {
    ಸ್ವಭಾವ:			ಪ್ರತ್ಯಾಗಮ HB_OT_LAYOUT_GLYPH_CLASS_UNCLASSIFIED;
    ಪಕ್ಷ BaseGlyph:		ಪ್ರತ್ಯಾಗಮ HB_OT_LAYOUT_GLYPH_PROPS_BASE_GLYPH;
    ಪಕ್ಷ LigatureGlyph:		ಪ್ರತ್ಯಾಗಮ HB_OT_LAYOUT_GLYPH_PROPS_LIGATURE;
    ಪಕ್ಷ MarkGlyph:
	  klass = get_mark_attachment_type (glyph);
	  ಪ್ರತ್ಯಾಗಮ HB_OT_LAYOUT_GLYPH_PROPS_MARK | (klass << 8);
    }
  }

  HB_INTERNAL ತಾರ್ಕಿಕ is_blocklisted (hb_blob_t *blob,
				   hb_face_t *face) ಧ್ರುವ;

  ಸಂರಚನಾ accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      table = hb_sanitize_context_t ().reference_table<GDEF> (face);
      ಯದಿ (unlikely (table->is_blocklisted (table.get_blob (), face)))
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
	ಯದಿ (!sets.in_error () &&
	    mark_glyph_set_bitmaps.resize (sets.length))
	  ಪ್ರತಿ (unsigned i = 0; i < sets.length; i++)
	  {
	    auto &bitmap = mark_glyph_set_bitmaps.arrayZ[i];
	    bitmap.bias = HB_SET_ಮೌಲ್ಯ_INVALID;
	    ಧ್ರುವ auto &set = sets.arrayZ[i];
	    ಯದಿ (unlikely (set.in_error ())) ಅನುವರ್ತ;
	    hb_codepoint_t min_gid = HB_SET_ಮೌಲ್ಯ_INVALID;
	    hb_codepoint_t max_gid = HB_SET_ಮೌಲ್ಯ_INVALID;
	    hb_set_next (&set, &min_gid);
	    hb_set_previous (&set, &max_gid);
	    ಯದಿ (max_gid == HB_SET_ಮೌಲ್ಯ_INVALID)
	      ಅನುವರ್ತ;
	    ಯದಿ (max_gid - min_gid >= hb_bit_page_t::PAGE_BITS)
	    {
	      /* Too wide for one page; a digest pre-filter instead. */
	      ಪ್ರತಿ (hb_codepoint_t gid : set)
		bitmap.u.digest.add (gid);
	      ಅನುವರ್ತ;
	    }
	    bitmap.bias = min_gid;
	    ಪ್ರತಿ (hb_codepoint_t gid : set)
	      bitmap.u.page.add (gid - min_gid);
	  }
      }
#endif
    }
    ~accelerator_t () { table.destroy (); }

    unsigned ಪೂರ್ಣಾಙ್ಕ get_glyph_props (hb_codepoint_t glyph) ಧ್ರುವ
    {
      unsigned v;

#ifndef HB_NO_GDEF_CACHE
      ಯದಿ (glyph_props_cache.get (glyph, &v))
        ಪ್ರತ್ಯಾಗಮ v;
#endif

      v = table->get_glyph_props (glyph);

#ifndef HB_NO_GDEF_CACHE
      ಯದಿ (likely (table.get_blob ())) // Don't try setting if we are the null instance!
	glyph_props_cache.set (glyph, v);
#endif

      ಪ್ರತ್ಯಾಗಮ v;

    }

    HB_ALWAYS_INLINE
    ತಾರ್ಕಿಕ mark_set_covers (unsigned ಪೂರ್ಣಾಙ್ಕ set_index, hb_codepoint_t glyph_id) ಧ್ರುವ
    {
#ifndef HB_NO_GDEF_CACHE
      // We can access arrayZ directly because of sanitize_lookup_props() guarantee.
      ಧ್ರುವ auto &bitmap = mark_glyph_set_bitmaps.arrayZ[set_index];
      ಯದಿ (likely (bitmap.bias != HB_SET_ಮೌಲ್ಯ_INVALID))
      {
	hb_codepoint_t biased = glyph_id - bitmap.bias;
	ಪ್ರತ್ಯಾಗಮ biased < hb_bit_page_t::PAGE_BITS &&
	       bitmap.u.page.get (biased);
      }
      ಪ್ರತ್ಯಾಗಮ mark_set_covers_slow (set_index, glyph_id);
#ಅನ್ಯಥಾ
      ಪ್ರತ್ಯಾಗಮ table->mark_set_covers (set_index, glyph_id);
#endif
    }

#ifndef HB_NO_GDEF_CACHE
    HB_NEVER_INLINE
    ತಾರ್ಕಿಕ mark_set_covers_slow (unsigned ಪೂರ್ಣಾಙ್ಕ set_index, hb_codepoint_t glyph_id) ಧ್ರುವ
    {
      ಧ್ರುವ auto &bitmap = mark_glyph_set_bitmaps.arrayZ[set_index];
      ಯದಿ (!bitmap.u.digest.may_have (glyph_id))
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      ಪ್ರತ್ಯಾಗಮ table->mark_set_covers (set_index, glyph_id);
    }
#endif

    unsigned sanitize_lookup_props (unsigned lookup_props) ಧ್ರುವ
    {
#ifndef HB_NO_GDEF_CACHE
      ಯದಿ (lookup_props & LookupFlag::UseMarkFilteringSet &&
	  (lookup_props >> 16) >= mark_glyph_set_bitmaps.length)
      {
        // Invalid mark filtering set index; unset the flag.
	lookup_props &= ~LookupFlag::UseMarkFilteringSet;
      }
#endif
      ಪ್ರತ್ಯಾಗಮ lookup_props;
    }

    hb_blob_ptr_t<GDEF> table;
#ifndef HB_NO_GDEF_CACHE
    ಸಂರಚನಾ mark_glyph_set_bitmap_t
    {
      /* Gid of the first bit (not page-aligned) for an exact one-page
       * set, or HB_SET_VALUE_INVALID; then digest is a pre-filter for
       * the table.  The all-zeros state is valid for both members. */
      hb_codepoint_t bias = HB_SET_ಮೌಲ್ಯ_INVALID;
      ಸಙ್ಘ u_t {
	u_t () : page () {}
	hb_bit_page_t page;
	hb_set_digest_t digest;
      } u;
    };

    hb_vector_t<mark_glyph_set_bitmap_t> mark_glyph_set_bitmaps;
    mutable hb_cache_t<21, 3> glyph_props_cache;
    static_assert (ಪರಿಮಾಣ (glyph_props_cache) == 512, "");
#endif
  };

  ಶೂನ್ಯ collect_variation_indices (hb_collect_variation_indices_context_t *c) ಧ್ರುವ
  {
#ifndef HB_NO_BEYOND_64K
    ಯದಿ (version.to_int () >= 0x00010004u && ligCaretList2)
    {
      (ಏತತ್+ligCaretList2).collect_variation_indices (c);
      ಪ್ರತ್ಯಾಗಮ;
    }
#endif
    (ಏತತ್+ligCaretList).collect_variation_indices (c);
  }

  ರಕ್ಷಿತ:
  template <typename ಕೀಲಿ>
  ಸ್ಥಿರ ಶೂನ್ಯ remap_varidx_after_instantiation (ಧ್ರುವ hb_map_t& varidx_map,
                                                hb_hashmap_t<ಕೀಲಿ, hb_pair_t<unsigned, ಪೂರ್ಣಾಙ್ಕ>>& layout_variation_idx_delta_map /* IN/OUT */)
  {
    /* varidx_map is empty which means varstore is empty after instantiation,
     * no variations, map all varidx to HB_OT_LAYOUT_NO_VARIATIONS_INDEX.
     * varidx_map doesn't have original varidx, indicating delta row is all
     * zeros, map varidx to HB_OT_LAYOUT_NO_VARIATIONS_INDEX */
    ಪ್ರತಿ (auto _ : layout_variation_idx_delta_map.iter_ref ())
    {
      /* old_varidx->(varidx, delta) mapping generated for subsetting, then this
       * varidx is used as key of varidx_map during instantiation */
      uint32_t varidx = _.second.first;
      uint32_t *new_varidx;
      ಯದಿ (varidx_map.has (varidx, &new_varidx))
	_.second.first = *new_varidx;
      ಅನ್ಯಥಾ
	_.second.first = HB_OT_LAYOUT_NO_VARIATIONS_INDEX;
    }
  }

  template <typename OffsetOut, typename OffsetIn, typename Base, typename ...Ts>
  ಸ್ಥಿರ ತಾರ್ಕಿಕ serialize_subset_offset (hb_subset_context_t *c,
				       OffsetOut& out,
				       ಧ್ರುವ OffsetIn& in,
				       ಧ್ರುವ Base *base,
				       Ts&&... ds)
  {
    out = 0;
    ಯದಿ (in.is_null ()) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    auto *s = c->serializer;
    s->push ();
    ತಾರ್ಕಿಕ ret = c->dispatch (base+in, std::forward<Ts> (ds)...);
    ಯದಿ (ret) s->add_link (out, s->ತೆಗೆ_pack ());
    ಅನ್ಯಥಾ s->ತೆಗೆ_discard ();
    ಪ್ರತ್ಯಾಗಮ ret;
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
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_MIN (12);
};

ಸಂರಚನಾ GDEF_accelerator_t : GDEF::accelerator_t {
  GDEF_accelerator_t (hb_face_t *face) : GDEF::accelerator_t (face) {}
};

} /* namespace OT */


#endif /* OT_LAYOUT_GDEF_GDEF_HH */
