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

#அந்தர்பாவ "../../../hb-ot-var-common.hh"

#அந்தர்பாவ "../../../hb-font.hh"
#அந்தர்பாவ "../../../hb-cache.hh"


நாமாகாஶ OT {


/*
 * Attachment List Table
 */

/* Array of contour point indices--in increasing numerical order */
கட்டமைப்பு AttachPoint : Array16Of<HBUINT16>
{
  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);
    auto *out = c->serializer->start_embed (*இது);
    return_trace (out->serialize (c->serializer, + iter ()));
  }
};

/* AttachList is referenced by both GDEF attachListOffset and
 * attachListOffset2. The v1.4 top-level offset widens, but the
 * AttachList table itself keeps Offset16 internals. */
கட்டமைப்பு AttachList
{
  unsigned முழு get_attach_points (hb_codepoint_t glyph_id,
				  unsigned முழு start_offset,
				  unsigned முழு *point_மொத்தம் /* IN/OUT */,
				  unsigned முழு *point_array /* OUT */) மாறிலி
  {
    unsigned முழு index = (இது+coverage).get_coverage (glyph_id);
    எனில் (index == NOT_COVERED)
    {
      எனில் (point_மொத்தம்)
	*point_மொத்தம் = 0;
      திரும்பு 0;
    }

    மாறிலி AttachPoint &points = இது+attachPoint[index];

    எனில் (point_மொத்தம் && point_array)
    {
      + points.as_array ().sub_array (start_offset, point_மொத்தம்)
      | hb_sink (hb_array (point_array, *point_மொத்தம்))
      ;
    }

    திரும்பு points.len;
  }

  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);
    மாறிலி hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    மாறிலி hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*இது);
    எனில் (unlikely (!c->serializer->extend_min (out))) return_trace (பொய்);

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (இது+coverage, attachPoint)
    | hb_filter (glyphset, hb_first)
    | hb_filter (subset_offset_array (c, out->attachPoint, இது), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;
    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (தருக்க (new_coverage));
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (coverage.sanitize (c, இது) && attachPoint.sanitize (c, இது));
  }

  பாதுகாக்கப்பட்ட:
  Offset16To<Coverage>
		coverage;		/* Offset to Coverage table -- from
					 * beginning of AttachList table */
  Array16OfOffset16To<AttachPoint>
		attachPoint;		/* Array of AttachPoint tables
					 * in Coverage Index order */
  பொது:
  DEFINE_SIZE_ARRAY (4, attachPoint);
};

/*
 * Ligature Caret Table
 */

கட்டமைப்பு Caret_மதிப்பு_Format1
{
  friend கட்டமைப்பு Caret_மதிப்பு;
  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);
    auto *out = c->serializer->embed (இது);
    எனில் (unlikely (!out)) return_trace (பொய்);
    return_trace (உண்மை);
  }

  தனிப்பட்ட:
  hb_position_t get_caret_மதிப்பு (hb_font_t *font, hb_direction_t direction) மாறிலி
  {
    திரும்பு HB_DIRECTION_IS_HORIZONTAL (direction) ? font->em_scale_x (coordinate) : font->em_scale_y (coordinate);
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது));
  }

  பாதுகாக்கப்பட்ட:
  HBUINT16	caret_மதிப்பு_Format;	/* Format identifier--format = 1 */
  FWORD		coordinate;		/* X or Y value, in design units */
  பொது:
  DEFINE_SIZE_STATIC (4);
};

கட்டமைப்பு Caret_மதிப்பு_Format2
{
  friend கட்டமைப்பு Caret_மதிப்பு;
  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);
    auto *out = c->serializer->embed (இது);
    எனில் (unlikely (!out)) return_trace (பொய்);
    return_trace (உண்மை);
  }

  தனிப்பட்ட:
  hb_position_t get_caret_மதிப்பு (hb_font_t *font, hb_direction_t direction, hb_codepoint_t glyph_id) மாறிலி
  {
    hb_position_t x, y;
    font->get_glyph_contour_point_for_origin (glyph_id, caret_மதிப்பு_Point, direction, &x, &y);
    திரும்பு HB_DIRECTION_IS_HORIZONTAL (direction) ? x : y;
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது));
  }

  பாதுகாக்கப்பட்ட:
  HBUINT16	caret_மதிப்பு_Format;	/* Format identifier--format = 2 */
  HBUINT16	caret_மதிப்பு_Point;	/* Contour point index on glyph */
  பொது:
  DEFINE_SIZE_STATIC (4);
};

கட்டமைப்பு Caret_மதிப்பு_Format3
{
  friend கட்டமைப்பு Caret_மதிப்பு;

  hb_position_t get_caret_மதிப்பு (hb_font_t *font, hb_direction_t direction,
				 மாறிலி ItemVariationStore &var_store) மாறிலி
  {
    திரும்பு HB_DIRECTION_IS_HORIZONTAL (direction) ?
	   hb_saturate_add (font->em_scale_x (coordinate), (இது+deviceTable).get_x_delta (font, var_store)) :
	   hb_saturate_add (font->em_scale_y (coordinate), (இது+deviceTable).get_y_delta (font, var_store));
  }

  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);
    auto *out = c->serializer->start_embed (*இது);
    எனில் (!c->serializer->embed (caret_மதிப்பு_Format)) return_trace (பொய்);
    எனில் (!c->serializer->embed (coordinate)) return_trace (பொய்);

    unsigned varidx = (இது+deviceTable).get_variation_index ();
    hb_pair_t<unsigned, முழு> *new_varidx_delta;
    எனில் (c->plan->layout_variation_idx_delta_map.has (varidx, &new_varidx_delta)) {
      uint32_t new_varidx = hb_first (*new_varidx_delta);
      முழு delta = hb_second (*new_varidx_delta);
      எனில் (delta != 0)
      {
        எனில் (!c->serializer->check_assign (out->coordinate, coordinate + delta, HB_SERIALIZE_ERROR_INT_OVERFLOW))
          return_trace (பொய்);
      }

      எனில் (new_varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
        return_trace (c->serializer->check_assign (out->caret_மதிப்பு_Format, 1, HB_SERIALIZE_ERROR_INT_OVERFLOW));
    }

    எனில் (!c->serializer->embed (deviceTable))
      return_trace (பொய்);

    return_trace (out->deviceTable.serialize_copy (c->serializer, deviceTable, இது, c->serializer->to_bias (out),
						   hb_serialize_context_t::Head, &c->plan->layout_variation_idx_delta_map));
  }

  வெறுமை collect_variation_indices (hb_collect_variation_indices_context_t *c) மாறிலி
  { (இது+deviceTable).collect_variation_indices (c); }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) && deviceTable.sanitize (c, இது));
  }

  பாதுகாக்கப்பட்ட:
  HBUINT16	caret_மதிப்பு_Format;	/* Format identifier--format = 3 */
  FWORD		coordinate;		/* X or Y value, in design units */
  Offset16To<Device>
		deviceTable;		/* Offset to Device table for X or Y
					 * value--from beginning of CaretValue
					 * table */
  பொது:
  DEFINE_SIZE_STATIC (6);
};

கட்டமைப்பு Caret_மதிப்பு
{
  hb_position_t get_caret_மதிப்பு (hb_font_t *font,
				 hb_direction_t direction,
				 hb_codepoint_t glyph_id,
				 மாறிலி ItemVariationStore &var_store) மாறிலி
  {
    தேர்வு (u.format.v) {
    நிலை 1: hb_barrier (); திரும்பு u.format1.get_caret_மதிப்பு (font, direction);
    நிலை 2: hb_barrier (); திரும்பு u.format2.get_caret_மதிப்பு (font, direction, glyph_id);
    நிலை 3: hb_barrier (); திரும்பு u.format3.get_caret_மதிப்பு (font, direction, var_store);
    இயல்பு:திரும்பு 0;
    }
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) மாறிலி
  {
    எனில் (unlikely (!c->may_dispatch (இது, &u.format.v))) திரும்பு c->no_dispatch_return_மதிப்பு ();
    TRACE_DISPATCH (இது, u.format.v);
    தேர்வு (u.format.v) {
    நிலை 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    நிலை 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    நிலை 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    இயல்பு:return_trace (c->default_return_மதிப்பு ());
    }
  }

  வெறுமை collect_variation_indices (hb_collect_variation_indices_context_t *c) மாறிலி
  {
    தேர்வு (u.format.v) {
    நிலை 1:
    நிலை 2:
      திரும்பு;
    நிலை 3:
      hb_barrier ();
      u.format3.collect_variation_indices (c);
      திரும்பு;
    இயல்பு: திரும்பு;
    }
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    எனில் (!u.format.v.sanitize (c)) return_trace (பொய்);
    hb_barrier ();
    தேர்வு (u.format.v) {
    நிலை 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    நிலை 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    நிலை 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    இயல்பு:return_trace (உண்மை);
    }
  }

  பாதுகாக்கப்பட்ட:
  ஒன்றிப்பு {
  கட்டமைப்பு { HBUINT16 v; }	format;		/* Format identifier */
  Caret_மதிப்பு_Format1	format1;
  Caret_மதிப்பு_Format2	format2;
  Caret_மதிப்பு_Format3	format3;
  } u;
  பொது:
  DEFINE_SIZE_UNION (2, format.v);
};

கட்டமைப்பு LigGlyph
{
  unsigned get_lig_carets (hb_font_t            *font,
			   hb_direction_t        direction,
			   hb_codepoint_t        glyph_id,
			   மாறிலி ItemVariationStore &var_store,
			   unsigned              start_offset,
			   unsigned             *caret_மொத்தம் /* IN/OUT */,
			   hb_position_t        *caret_array /* OUT */) மாறிலி
  {
    எனில் (caret_மொத்தம் && caret_array)
    {
      + carets.as_array ().sub_array (start_offset, caret_மொத்தம்)
      | hb_map (hb_add (இது))
      | hb_map ([&] (மாறிலி Caret_மதிப்பு &value) { திரும்பு value.get_caret_மதிப்பு (font, direction, glyph_id, var_store); })
      | hb_sink (hb_array (caret_array, *caret_மொத்தம்))
      ;
    }

    திரும்பு carets.len;
  }

  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);
    auto *out = c->serializer->start_embed (*இது);
    எனில் (unlikely (!c->serializer->extend_min (out))) return_trace (பொய்);

    + hb_iter (carets)
    | hb_apply (subset_offset_array (c, out->carets, இது))
    ;

    return_trace (தருக்க (out->carets));
  }

  வெறுமை collect_variation_indices (hb_collect_variation_indices_context_t *c) மாறிலி
  {
    க்கு (மாறிலி Offset16To<Caret_மதிப்பு>& offset : carets.iter ())
      (இது+offset).collect_variation_indices (c);
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (carets.sanitize (c, இது));
  }

  பாதுகாக்கப்பட்ட:
  Array16OfOffset16To<Caret_மதிப்பு>
		carets;			/* Offset array of CaretValue tables
					 * --from beginning of LigGlyph table
					 * --in increasing coordinate order */
  பொது:
  DEFINE_SIZE_ARRAY (2, carets);
};

/* LigCaretList2 widens the LigCaretList coverage, count, and LigGlyph
 * offsets. The LigGlyph table itself keeps Offset16 caretValueOffsets. */
template <typename Types>
கட்டமைப்பு LigCaretList
{
  unsigned முழு get_lig_carets (hb_font_t *font,
			       hb_direction_t direction,
			       hb_codepoint_t glyph_id,
			       மாறிலி ItemVariationStore &var_store,
			       unsigned முழு start_offset,
			       unsigned முழு *caret_மொத்தம் /* IN/OUT */,
			       hb_position_t *caret_array /* OUT */) மாறிலி
  {
    unsigned முழு index = (இது+coverage).get_coverage (glyph_id);
    எனில் (index == NOT_COVERED)
    {
      எனில் (caret_மொத்தம்)
	*caret_மொத்தம் = 0;
      திரும்பு 0;
    }
    மாறிலி LigGlyph &lig_glyph = இது+ligGlyph[index];
    திரும்பு lig_glyph.get_lig_carets (font, direction, glyph_id, var_store, start_offset, caret_மொத்தம், caret_array);
  }

  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);
    மாறிலி hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    மாறிலி hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*இது);
    எனில் (unlikely (!c->serializer->extend_min (out))) return_trace (பொய்);

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (இது+coverage, ligGlyph)
    | hb_filter (glyphset, hb_first)
    | hb_filter (subset_offset_array (c, out->ligGlyph, இது), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;
    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (தருக்க (new_coverage));
  }

  வெறுமை collect_variation_indices (hb_collect_variation_indices_context_t *c) மாறிலி
  {
    + hb_zip (இது+coverage, ligGlyph)
    | hb_filter (c->glyph_set, hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (இது))
    | hb_apply ([c] (மாறிலி LigGlyph& _) { _.collect_variation_indices (c); })
    ;
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (coverage.sanitize (c, இது) && ligGlyph.sanitize (c, இது));
  }

  பாதுகாக்கப்பட்ட:
  typename Types::template LOffsetTo<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of LigCaretList table */
  typename Types::template ArrayOf<typename Types::template OffsetTo<LigGlyph>>
		ligGlyph;		/* Array of LigGlyph tables
					 * in Coverage Index order */
  பொது:
  DEFINE_SIZE_ARRAY (Types::LOffset::static_size + Types::HBUINT::static_size, ligGlyph);
};

#ifndef HB_NO_BEYOND_64K
static_assert (LigCaretList<MediumTypes>::min_size ==
	       Offset32::static_size + HBUINT24::static_size, "");
#endif


கட்டமைப்பு MarkGlyphSetsFormat1
{
  தருக்க covers (unsigned முழு set_index, hb_codepoint_t glyph_id) மாறிலி
  { திரும்பு (இது+coverage[set_index]).get_coverage (glyph_id) != NOT_COVERED; }

  வெறுமை collect_used_mark_sets (மாறிலி hb_set_t& glyph_set,
                               hb_set_t& used_mark_sets /* OUT */) மாறிலி
  {
    unsigned i = 0;
    க்கு (மாறிலி auto &offset : coverage)
     {
       மாறிலி auto &cov = இது+offset;
       எனில் (cov.intersects (&glyph_set))
         used_mark_sets.add (i);

       i++;
     }
  }

  template <typename set_t>
  வெறுமை collect_coverage (hb_vector_t<set_t> &sets) மாறிலி
  {
     க்கு (மாறிலி auto &offset : coverage)
     {
       மாறிலி auto &cov = இது+offset;
       cov.collect_coverage (sets.push ());
     }
  }

  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);
    auto *out = c->serializer->start_embed (*இது);
    எனில் (unlikely (!c->serializer->extend_min (out))) return_trace (பொய்);
    out->format = format;

    தருக்க ret = உண்மை;
    க்கு (மாறிலி Offset32To<Coverage>& offset : coverage.iter ())
    {
      auto snap = c->serializer->snapshot ();
      auto *o = out->coverage.serialize_append (c->serializer);
      எனில் (unlikely (!o))
      {
	ret = பொய்;
	நிறுத்து;
      }

      //skip empty coverage
      c->serializer->push ();
      தருக்க res = பொய்;
      எனில் (offset) res = c->dispatch (இது+offset);
      எனில் (!res)
      {
        c->serializer->எடு_discard ();
        c->serializer->revert (snap);
        (out->coverage.len)--;
        தொடர்;
      }
      c->serializer->add_link (*o, c->serializer->எடு_pack ());
    }

    return_trace (ret && out->coverage.len);
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (coverage.sanitize (c, இது));
  }

  பாதுகாக்கப்பட்ட:
  HBUINT16	format;			/* Format identifier--format = 1 */
  Array16Of<Offset32To<Coverage>>
		coverage;		/* Array of long offsets to mark set
					 * coverage tables */
  பொது:
  DEFINE_SIZE_ARRAY (4, coverage);
};

கட்டமைப்பு MarkGlyphSets
{
  தருக்க covers (unsigned முழு set_index, hb_codepoint_t glyph_id) மாறிலி
  {
    தேர்வு (u.format.v) {
    நிலை 1: hb_barrier (); திரும்பு u.format1.covers (set_index, glyph_id);
    இயல்பு:திரும்பு பொய்;
    }
  }

  template <typename set_t>
  வெறுமை collect_coverage (hb_vector_t<set_t> &sets) மாறிலி
  {
    தேர்வு (u.format.v) {
    நிலை 1: hb_barrier (); u.format1.collect_coverage (sets); திரும்பு;
    இயல்பு:திரும்பு;
    }
  }

  வெறுமை collect_used_mark_sets (மாறிலி hb_set_t& glyph_set,
                               hb_set_t& used_mark_sets /* OUT */) மாறிலி
  {
    தேர்வு (u.format.v) {
    நிலை 1: hb_barrier (); u.format1.collect_used_mark_sets (glyph_set, used_mark_sets); திரும்பு;
    இயல்பு:திரும்பு;
    }
  }

  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);
    தேர்வு (u.format.v) {
    நிலை 1: hb_barrier (); return_trace (u.format1.subset (c));
    இயல்பு:return_trace (பொய்);
    }
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    எனில் (!u.format.v.sanitize (c)) return_trace (பொய்);
    hb_barrier ();
    தேர்வு (u.format.v) {
    நிலை 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    இயல்பு:return_trace (உண்மை);
    }
  }

  பாதுகாக்கப்பட்ட:
  ஒன்றிப்பு {
  கட்டமைப்பு { HBUINT16 v; }	format;		/* Format identifier */
  MarkGlyphSetsFormat1	format1;
  } u;
  பொது:
  DEFINE_SIZE_UNION (2, format.v);
};


/*
 * GDEF -- Glyph Definition
 * https://docs.microsoft.com/en-us/typography/opentype/spec/gdef
 */


கட்டமைப்பு GDEF
{
  நிலையான constexpr hb_tag_t tableTag = HB_OT_TAG_GDEF;

  எண்ணிக்கை GlyphClasses {
    UnclassifiedGlyph	= 0,
    BaseGlyph		= 1,
    LigatureGlyph	= 2,
    MarkGlyph		= 3,
    ComponentGlyph	= 4
  };

  size_t get_size () மாறிலி
  {
    எனில் (version.major != 1) திரும்பு version.static_size;

    திரும்பு min_size +
	   (version.to_int () >= 0x00010002u ? markGlyphSetsDef.static_size : 0) +
	   (version.to_int () >= 0x00010003u ? varStore.static_size : 0) +
#ifndef HB_NO_BEYOND_64K
	   (version.to_int () >= 0x00010004u ? 5 * Offset32::static_size : 0)
#இல்லையெனில்
	   0
#endif
	   ;
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    எனில் (unlikely (!version.sanitize (c) ||
		  version.major != 1 ||
		  !c->check_range (இது, get_size ())))
      return_trace (பொய்);
    hb_barrier ();

    தருக்க glyph_class_def_ok = glyphClassDef.sanitize (c, இது);
    தருக்க attach_list_ok = attachList.sanitize (c, இது);
    தருக்க lig_caret_list_ok = ligCaretList.sanitize (c, இது);
    தருக்க mark_attach_class_def_ok = markAttachClassDef.sanitize (c, இது);
    தருக்க mark_glyph_sets_def_ok = version.to_int () < 0x00010002u ||
				  markGlyphSetsDef.sanitize (c, இது);

#ifndef HB_NO_BEYOND_64K
    எனில் (version.to_int () >= 0x00010004u)
    {
      எனில் (glyphClassDef2) glyph_class_def_ok = glyphClassDef2.sanitize (c, இது);
      எனில் (attachList2) attach_list_ok = attachList2.sanitize (c, இது);
      எனில் (ligCaretList2) lig_caret_list_ok = ligCaretList2.sanitize (c, இது);
      எனில் (markAttachClassDef2) mark_attach_class_def_ok = markAttachClassDef2.sanitize (c, இது);
      எனில் (markGlyphSetsDef2) mark_glyph_sets_def_ok = markGlyphSetsDef2.sanitize (c, இது);
    }
#endif

    return_trace (glyph_class_def_ok &&
		  attach_list_ok &&
		  lig_caret_list_ok &&
		  mark_attach_class_def_ok &&
		  mark_glyph_sets_def_ok &&
		  (version.to_int () < 0x00010003u || varStore.sanitize (c, இது)));
  }

  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);
    எனில் (version.major != 1) return_trace (பொய்);

    auto *out = c->serializer->start_embed (*இது);
    எனில் (unlikely (!c->serializer->extend_min (out))) return_trace (பொய்);

    // Push var store first (if it's needed) so that it's last in the
    // serialization order. Some font consumers assume that varstore runs to
    // the end of the GDEF table.
    // See: https://github.com/harfbuzz/harfbuzz/issues/4636
    auto snapshot_version0 = c->serializer->snapshot ();
    எனில் (unlikely (version.to_int () >= 0x00010002u && hb_barrier () &&
		  !c->serializer->embed (markGlyphSetsDef)))
      return_trace (பொய்);

    auto snapshot_version2 = c->serializer->snapshot ();
    எனில் (unlikely (version.to_int () >= 0x00010003u && hb_barrier () &&
		  !c->serializer->embed (varStore)))
      return_trace (பொய்);

    தருக்க use_v14 = பொய்;
#ifndef HB_NO_BEYOND_64K
    use_v14 = version.to_int () >= 0x00010004u &&
	      (glyphClassDef2 || attachList2 || ligCaretList2 ||
	       markAttachClassDef2 || markGlyphSetsDef2);
    எனில் (unlikely (version.to_int () >= 0x00010004u && hb_barrier () &&
		  !c->serializer->extend_size (out, get_size ())))
      return_trace (பொய்);
#endif

    தருக்க subset_varstore = பொய்;
    unsigned varstore_index = (unsigned) -1;
    எனில் (version.to_int () >= 0x00010003u && hb_barrier ())
    {
      எனில் (c->plan->all_axes_pinned)
	out->varStore = 0;
      இல்லையெனில் எனில் (c->plan->normalized_coords)
      {
	எனில் (varStore)
	{
	  item_variations_t item_vars;
	  எனில் (unlikely (!c->serializer->check_success (
                  item_vars.instantiate (இது+varStore, c->plan, உண்மை, உண்மை,
                                          c->plan->gdef_varstore_inner_maps.as_array (), உண்மை))))
            return_trace (பொய்);
	  subset_varstore = out->varStore.serialize_serialize (c->serializer,
							     item_vars.has_long_word (),
							     c->plan->axis_tags,
							     item_vars.get_region_list (),
							     item_vars.get_vardata_encodings ());
	  varstore_index = c->serializer->last_added_child_index();
	  remap_varidx_after_instantiation (item_vars.get_varidx_map (),
					    c->plan->layout_variation_idx_delta_map);
          remap_varidx_after_instantiation (item_vars.get_varidx_map (),
                                            c->plan->layout_condition_idx_மதிப்பு_map);
	}
      }
      இல்லையெனில்
      {
	subset_varstore = out->varStore.serialize_subset (c, varStore, இது,
							  c->plan->gdef_varstore_inner_maps.as_array ());
	varstore_index = c->serializer->last_added_child_index();
      }
    }

    out->version.major = version.major;
    out->version.minor = version.minor;

    எனில் (!subset_varstore && version.to_int () >= 0x00010002u && !use_v14) {
      c->serializer->revert (snapshot_version2);
    }
    எனில் (!subset_varstore)
      out->varStore = 0;
    இல்லையெனில்
      c->plan->has_gdef_varstore = உண்மை;

    தருக்க subset_markglyphsetsdef = பொய்;
    எனில் (version.to_int () >= 0x00010002u && hb_barrier ())
    {
#ifndef HB_NO_BEYOND_64K
      எனில் (use_v14 && markGlyphSetsDef2)
      {
	out->markGlyphSetsDef = 0;
	subset_markglyphsetsdef = serialize_subset_offset (c, out->markGlyphSetsDef2,
							   markGlyphSetsDef2, இது);
      }
      இல்லையெனில்
#endif
      {
#ifndef HB_NO_BEYOND_64K
	எனில் (use_v14) out->markGlyphSetsDef2 = 0;
#endif
	subset_markglyphsetsdef = out->markGlyphSetsDef.serialize_subset (c, markGlyphSetsDef, இது);
      }
    }

    எனில் (use_v14)
    {
      out->version.minor = 4;
    } இல்லையெனில் எனில் (subset_varstore) {
      out->version.minor = 3;
    } இல்லையெனில் எனில் (subset_markglyphsetsdef) {
      out->version.minor = 2;
    } இல்லையெனில்  {
      out->version.minor = 0;
      c->serializer->revert (snapshot_version0);
    }

    தருக்க subset_glyphclassdef = பொய்;
    தருக்க subset_attachlist = பொய்;
    தருக்க subset_markattachclassdef = பொய்;
    தருக்க subset_ligcaretlist = பொய்;

#ifndef HB_NO_BEYOND_64K
    எனில் (use_v14 && glyphClassDef2)
    {
      out->glyphClassDef = 0;
      subset_glyphclassdef = serialize_subset_offset (c, out->glyphClassDef2,
						      glyphClassDef2, இது, nullptr, பொய், உண்மை);
    }
    இல்லையெனில்
#endif
    {
#ifndef HB_NO_BEYOND_64K
      எனில் (use_v14) out->glyphClassDef2 = 0;
#endif
      subset_glyphclassdef = out->glyphClassDef.serialize_subset (c, glyphClassDef, இது, nullptr, பொய், உண்மை);
    }

#ifndef HB_NO_BEYOND_64K
    எனில் (use_v14 && attachList2)
    {
      out->attachList = 0;
      subset_attachlist = serialize_subset_offset (c, out->attachList2, attachList2, இது);
    }
    இல்லையெனில்
#endif
    {
#ifndef HB_NO_BEYOND_64K
      எனில் (use_v14) out->attachList2 = 0;
#endif
      subset_attachlist = out->attachList.serialize_subset (c, attachList, இது);
    }

#ifndef HB_NO_BEYOND_64K
    எனில் (use_v14 && markAttachClassDef2)
    {
      out->markAttachClassDef = 0;
      subset_markattachclassdef = serialize_subset_offset (c, out->markAttachClassDef2,
							   markAttachClassDef2, இது, nullptr, பொய், உண்மை);
    }
    இல்லையெனில்
#endif
    {
#ifndef HB_NO_BEYOND_64K
      எனில் (use_v14) out->markAttachClassDef2 = 0;
#endif
      subset_markattachclassdef = out->markAttachClassDef.serialize_subset (c, markAttachClassDef,
									   இது, nullptr, பொய், உண்மை);
    }

#ifndef HB_NO_BEYOND_64K
    எனில் (use_v14 && ligCaretList2)
    {
      out->ligCaretList = 0;
      subset_ligcaretlist = serialize_subset_offset (c, out->ligCaretList2, ligCaretList2, இது);
    }
    இல்லையெனில்
#endif
    {
#ifndef HB_NO_BEYOND_64K
      எனில் (use_v14) out->ligCaretList2 = 0;
#endif
      subset_ligcaretlist = out->ligCaretList.serialize_subset (c, ligCaretList, இது);
    }

    எனில் (subset_varstore && varstore_index != (unsigned) -1) {
      c->serializer->repack_last(varstore_index);
    }

    return_trace (subset_glyphclassdef || subset_attachlist ||
		  subset_ligcaretlist || subset_markattachclassdef ||
		  (out->version.to_int () >= 0x00010002u && subset_markglyphsetsdef) ||
		  (out->version.to_int () >= 0x00010003u && subset_varstore));
  }

  தருக்க has_glyph_classes () மாறிலி
  {
#ifndef HB_NO_BEYOND_64K
    எனில் (version.to_int () >= 0x00010004u && glyphClassDef2) திரும்பு உண்மை;
#endif
    திரும்பு glyphClassDef != 0;
  }
  மாறிலி ClassDef &get_glyph_class_def () மாறிலி
  {
#ifndef HB_NO_BEYOND_64K
    எனில் (version.to_int () >= 0x00010004u && glyphClassDef2) திரும்பு இது+glyphClassDef2;
#endif
    திரும்பு இது+glyphClassDef;
  }
  தருக்க has_attach_list () மாறிலி
  {
#ifndef HB_NO_BEYOND_64K
    எனில் (version.to_int () >= 0x00010004u && attachList2) திரும்பு உண்மை;
#endif
    திரும்பு attachList != 0;
  }
  மாறிலி AttachList &get_attach_list () மாறிலி
  {
#ifndef HB_NO_BEYOND_64K
    எனில் (version.to_int () >= 0x00010004u && attachList2) திரும்பு இது+attachList2;
#endif
    திரும்பு இது+attachList;
  }
  தருக்க has_lig_carets () மாறிலி
  {
#ifndef HB_NO_BEYOND_64K
    எனில் (version.to_int () >= 0x00010004u && ligCaretList2) திரும்பு உண்மை;
#endif
    திரும்பு ligCaretList != 0;
  }
  தருக்க has_mark_attachment_types () மாறிலி
  {
#ifndef HB_NO_BEYOND_64K
    எனில் (version.to_int () >= 0x00010004u && markAttachClassDef2) திரும்பு உண்மை;
#endif
    திரும்பு markAttachClassDef != 0;
  }
  மாறிலி ClassDef &get_mark_attach_class_def () மாறிலி
  {
#ifndef HB_NO_BEYOND_64K
    எனில் (version.to_int () >= 0x00010004u && markAttachClassDef2) திரும்பு இது+markAttachClassDef2;
#endif
    திரும்பு இது+markAttachClassDef;
  }
  தருக்க has_mark_glyph_sets () மாறிலி
  {
    எனில் (version.to_int () < 0x00010002u) திரும்பு பொய்;
#ifndef HB_NO_BEYOND_64K
    எனில் (version.to_int () >= 0x00010004u && markGlyphSetsDef2) திரும்பு உண்மை;
#endif
    திரும்பு markGlyphSetsDef != 0;
  }
  மாறிலி MarkGlyphSets &get_mark_glyph_sets () மாறிலி
  {
    எனில் (version.to_int () < 0x00010002u) திரும்பு Null(MarkGlyphSets);
#ifndef HB_NO_BEYOND_64K
    எனில் (version.to_int () >= 0x00010004u && markGlyphSetsDef2) திரும்பு இது+markGlyphSetsDef2;
#endif
    திரும்பு இது+markGlyphSetsDef;
  }
  தருக்க has_var_store () மாறிலி
  {
    திரும்பு version.to_int () >= 0x00010003u && hb_barrier () && varStore != 0;
  }
  மாறிலி ItemVariationStore &get_var_store () மாறிலி
  {
    திரும்பு version.to_int () >= 0x00010003u && hb_barrier () ? இது+varStore : Null(ItemVariationStore);
  }


  தருக்க has_data () மாறிலி { திரும்பு version.to_int (); }
  unsigned முழு get_glyph_class (hb_codepoint_t glyph) மாறிலி
  { திரும்பு get_glyph_class_def ().get_class (glyph); }
  வெறுமை get_glyphs_in_class (unsigned முழு klass, hb_set_t *glyphs) மாறிலி
  { get_glyph_class_def ().collect_class (glyphs, klass); }

  unsigned முழு get_mark_attachment_type (hb_codepoint_t glyph) மாறிலி
  { திரும்பு get_mark_attach_class_def ().get_class (glyph); }

  unsigned முழு get_attach_points (hb_codepoint_t glyph_id,
				  unsigned முழு start_offset,
				  unsigned முழு *point_மொத்தம் /* IN/OUT */,
				  unsigned முழு *point_array /* OUT */) மாறிலி
  { திரும்பு get_attach_list ().get_attach_points (glyph_id, start_offset, point_மொத்தம், point_array); }

  unsigned முழு get_lig_carets (hb_font_t *font,
			       hb_direction_t direction,
			       hb_codepoint_t glyph_id,
			       unsigned முழு start_offset,
			       unsigned முழு *caret_மொத்தம் /* IN/OUT */,
			       hb_position_t *caret_array /* OUT */) மாறிலி
  {
#ifndef HB_NO_BEYOND_64K
    எனில் (version.to_int () >= 0x00010004u && ligCaretList2)
      திரும்பு (இது+ligCaretList2).get_lig_carets (font, direction, glyph_id, get_var_store (),
						  start_offset, caret_மொத்தம், caret_array);
#endif
    திரும்பு (இது+ligCaretList).get_lig_carets (font, direction, glyph_id, get_var_store (),
					       start_offset, caret_மொத்தம், caret_array);
  }

  தருக்க mark_set_covers (unsigned முழு set_index, hb_codepoint_t glyph_id) மாறிலி
  { திரும்பு get_mark_glyph_sets ().covers (set_index, glyph_id); }

  /* glyph_props is a 16-bit integer where the lower 8-bit have bits representing
   * glyph class and other bits, and high 8-bit the mark attachment type (if any).
   * Not to be confused with lookup_props which is very similar. */
  unsigned முழு get_glyph_props (hb_codepoint_t glyph) மாறிலி
  {
    unsigned முழு klass = get_glyph_class (glyph);

    static_assert (((unsigned முழு) HB_OT_LAYOUT_GLYPH_PROPS_BASE_GLYPH == (unsigned முழு) LookupFlag::IgnoreBaseGlyphs), "");
    static_assert (((unsigned முழு) HB_OT_LAYOUT_GLYPH_PROPS_LIGATURE == (unsigned முழு) LookupFlag::IgnoreLigatures), "");
    static_assert (((unsigned முழு) HB_OT_LAYOUT_GLYPH_PROPS_MARK == (unsigned முழு) LookupFlag::IgnoreMarks), "");

    தேர்வு (klass) {
    இயல்பு:			திரும்பு HB_OT_LAYOUT_GLYPH_CLASS_UNCLASSIFIED;
    நிலை BaseGlyph:		திரும்பு HB_OT_LAYOUT_GLYPH_PROPS_BASE_GLYPH;
    நிலை LigatureGlyph:		திரும்பு HB_OT_LAYOUT_GLYPH_PROPS_LIGATURE;
    நிலை MarkGlyph:
	  klass = get_mark_attachment_type (glyph);
	  திரும்பு HB_OT_LAYOUT_GLYPH_PROPS_MARK | (klass << 8);
    }
  }

  HB_INTERNAL தருக்க is_blocklisted (hb_blob_t *blob,
				   hb_face_t *face) மாறிலி;

  கட்டமைப்பு accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      table = hb_sanitize_context_t ().reference_table<GDEF> (face);
      எனில் (unlikely (table->is_blocklisted (table.get_blob (), face)))
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
	எனில் (!sets.in_error () &&
	    mark_glyph_set_bitmaps.resize (sets.length))
	  க்கு (unsigned i = 0; i < sets.length; i++)
	  {
	    auto &bitmap = mark_glyph_set_bitmaps.arrayZ[i];
	    bitmap.bias = HB_SET_மதிப்பு_INVALID;
	    மாறிலி auto &set = sets.arrayZ[i];
	    எனில் (unlikely (set.in_error ())) தொடர்;
	    hb_codepoint_t min_gid = HB_SET_மதிப்பு_INVALID;
	    hb_codepoint_t max_gid = HB_SET_மதிப்பு_INVALID;
	    hb_set_next (&set, &min_gid);
	    hb_set_previous (&set, &max_gid);
	    எனில் (max_gid == HB_SET_மதிப்பு_INVALID)
	      தொடர்;
	    எனில் (max_gid - min_gid >= hb_bit_page_t::PAGE_BITS)
	    {
	      /* Too wide for one page; a digest pre-filter instead. */
	      க்கு (hb_codepoint_t gid : set)
		bitmap.u.digest.add (gid);
	      தொடர்;
	    }
	    bitmap.bias = min_gid;
	    க்கு (hb_codepoint_t gid : set)
	      bitmap.u.page.add (gid - min_gid);
	  }
      }
#endif
    }
    ~accelerator_t () { table.destroy (); }

    unsigned முழு get_glyph_props (hb_codepoint_t glyph) மாறிலி
    {
      unsigned v;

#ifndef HB_NO_GDEF_CACHE
      எனில் (glyph_props_cache.get (glyph, &v))
        திரும்பு v;
#endif

      v = table->get_glyph_props (glyph);

#ifndef HB_NO_GDEF_CACHE
      எனில் (likely (table.get_blob ())) // Don't try setting if we are the null instance!
	glyph_props_cache.set (glyph, v);
#endif

      திரும்பு v;

    }

    HB_ALWAYS_INLINE
    தருக்க mark_set_covers (unsigned முழு set_index, hb_codepoint_t glyph_id) மாறிலி
    {
#ifndef HB_NO_GDEF_CACHE
      // We can access arrayZ directly because of sanitize_lookup_props() guarantee.
      மாறிலி auto &bitmap = mark_glyph_set_bitmaps.arrayZ[set_index];
      எனில் (likely (bitmap.bias != HB_SET_மதிப்பு_INVALID))
      {
	hb_codepoint_t biased = glyph_id - bitmap.bias;
	திரும்பு biased < hb_bit_page_t::PAGE_BITS &&
	       bitmap.u.page.get (biased);
      }
      திரும்பு mark_set_covers_slow (set_index, glyph_id);
#இல்லையெனில்
      திரும்பு table->mark_set_covers (set_index, glyph_id);
#endif
    }

#ifndef HB_NO_GDEF_CACHE
    HB_NEVER_INLINE
    தருக்க mark_set_covers_slow (unsigned முழு set_index, hb_codepoint_t glyph_id) மாறிலி
    {
      மாறிலி auto &bitmap = mark_glyph_set_bitmaps.arrayZ[set_index];
      எனில் (!bitmap.u.digest.may_have (glyph_id))
	திரும்பு பொய்;
      திரும்பு table->mark_set_covers (set_index, glyph_id);
    }
#endif

    unsigned sanitize_lookup_props (unsigned lookup_props) மாறிலி
    {
#ifndef HB_NO_GDEF_CACHE
      எனில் (lookup_props & LookupFlag::UseMarkFilteringSet &&
	  (lookup_props >> 16) >= mark_glyph_set_bitmaps.length)
      {
        // Invalid mark filtering set index; unset the flag.
	lookup_props &= ~LookupFlag::UseMarkFilteringSet;
      }
#endif
      திரும்பு lookup_props;
    }

    hb_blob_ptr_t<GDEF> table;
#ifndef HB_NO_GDEF_CACHE
    கட்டமைப்பு mark_glyph_set_bitmap_t
    {
      /* Gid of the first bit (not page-aligned) for an exact one-page
       * set, or HB_SET_VALUE_INVALID; then digest is a pre-filter for
       * the table.  The all-zeros state is valid for both members. */
      hb_codepoint_t bias = HB_SET_மதிப்பு_INVALID;
      ஒன்றிப்பு u_t {
	u_t () : page () {}
	hb_bit_page_t page;
	hb_set_digest_t digest;
      } u;
    };

    hb_vector_t<mark_glyph_set_bitmap_t> mark_glyph_set_bitmaps;
    mutable hb_cache_t<21, 3> glyph_props_cache;
    static_assert (அளவு (glyph_props_cache) == 512, "");
#endif
  };

  வெறுமை collect_variation_indices (hb_collect_variation_indices_context_t *c) மாறிலி
  {
#ifndef HB_NO_BEYOND_64K
    எனில் (version.to_int () >= 0x00010004u && ligCaretList2)
    {
      (இது+ligCaretList2).collect_variation_indices (c);
      திரும்பு;
    }
#endif
    (இது+ligCaretList).collect_variation_indices (c);
  }

  பாதுகாக்கப்பட்ட:
  template <typename சாவி>
  நிலையான வெறுமை remap_varidx_after_instantiation (மாறிலி hb_map_t& varidx_map,
                                                hb_hashmap_t<சாவி, hb_pair_t<unsigned, முழு>>& layout_variation_idx_delta_map /* IN/OUT */)
  {
    /* varidx_map is empty which means varstore is empty after instantiation,
     * no variations, map all varidx to HB_OT_LAYOUT_NO_VARIATIONS_INDEX.
     * varidx_map doesn't have original varidx, indicating delta row is all
     * zeros, map varidx to HB_OT_LAYOUT_NO_VARIATIONS_INDEX */
    க்கு (auto _ : layout_variation_idx_delta_map.iter_ref ())
    {
      /* old_varidx->(varidx, delta) mapping generated for subsetting, then this
       * varidx is used as key of varidx_map during instantiation */
      uint32_t varidx = _.second.first;
      uint32_t *new_varidx;
      எனில் (varidx_map.has (varidx, &new_varidx))
	_.second.first = *new_varidx;
      இல்லையெனில்
	_.second.first = HB_OT_LAYOUT_NO_VARIATIONS_INDEX;
    }
  }

  template <typename OffsetOut, typename OffsetIn, typename Base, typename ...Ts>
  நிலையான தருக்க serialize_subset_offset (hb_subset_context_t *c,
				       OffsetOut& out,
				       மாறிலி OffsetIn& in,
				       மாறிலி Base *base,
				       Ts&&... ds)
  {
    out = 0;
    எனில் (in.is_null ()) திரும்பு பொய்;

    auto *s = c->serializer;
    s->push ();
    தருக்க ret = c->dispatch (base+in, std::forward<Ts> (ds)...);
    எனில் (ret) s->add_link (out, s->எடு_pack ());
    இல்லையெனில் s->எடு_discard ();
    திரும்பு ret;
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
  பொது:
  DEFINE_SIZE_MIN (12);
};

கட்டமைப்பு GDEF_accelerator_t : GDEF::accelerator_t {
  GDEF_accelerator_t (hb_face_t *face) : GDEF::accelerator_t (face) {}
};

} /* namespace OT */


#endif /* OT_LAYOUT_GDEF_GDEF_HH */
