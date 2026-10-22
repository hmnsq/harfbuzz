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

#അന്തര്ഭാവ "../../../hb-ot-var-common.hh"

#അന്തര്ഭാവ "../../../hb-font.hh"
#അന്തര്ഭാവ "../../../hb-cache.hh"


നാമാകാശ OT {


/*
 * Attachment List Table
 */

/* Array of contour point indices--in increasing numerical order */
ഘടന AttachPoint : Array16Of<HBUINT16>
{
  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    auto *out = c->serializer->start_embed (*ഇത്);
    return_trace (out->serialize (c->serializer, + iter ()));
  }
};

/* AttachList is referenced by both GDEF attachListOffset and
 * attachListOffset2. The v1.4 top-level offset widens, but the
 * AttachList table itself keeps Offset16 internals. */
ഘടന AttachList
{
  unsigned പൂർണ്ണസംഖ്യ get_attach_points (hb_codepoint_t glyph_id,
				  unsigned പൂർണ്ണസംഖ്യ start_offset,
				  unsigned പൂർണ്ണസംഖ്യ *point_എണ്ണം /* IN/OUT */,
				  unsigned പൂർണ്ണസംഖ്യ *point_array /* OUT */) സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ index = (ഇത്+coverage).get_coverage (glyph_id);
    എങ്കിൽ (index == NOT_COVERED)
    {
      എങ്കിൽ (point_എണ്ണം)
	*point_എണ്ണം = 0;
      തിരികെ 0;
    }

    സ്ഥിരം AttachPoint &points = ഇത്+attachPoint[index];

    എങ്കിൽ (point_എണ്ണം && point_array)
    {
      + points.as_array ().sub_array (start_offset, point_എണ്ണം)
      | hb_sink (hb_array (point_array, *point_എണ്ണം))
      ;
    }

    തിരികെ points.len;
  }

  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    സ്ഥിരം hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    സ്ഥിരം hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*ഇത്);
    എങ്കിൽ (unlikely (!c->serializer->extend_min (out))) return_trace (അസത്യം);

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (ഇത്+coverage, attachPoint)
    | hb_filter (glyphset, hb_first)
    | hb_filter (subset_offset_array (c, out->attachPoint, ഇത്), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;
    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (തർക്കം (new_coverage));
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (coverage.sanitize (c, ഇത്) && attachPoint.sanitize (c, ഇത്));
  }

  സംരക്ഷിത:
  Offset16To<Coverage>
		coverage;		/* Offset to Coverage table -- from
					 * beginning of AttachList table */
  Array16OfOffset16To<AttachPoint>
		attachPoint;		/* Array of AttachPoint tables
					 * in Coverage Index order */
  പൊതു:
  DEFINE_SIZE_ARRAY (4, attachPoint);
};

/*
 * Ligature Caret Table
 */

ഘടന Caret_മൂല്യം_Format1
{
  friend ഘടന Caret_മൂല്യം;
  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    auto *out = c->serializer->embed (ഇത്);
    എങ്കിൽ (unlikely (!out)) return_trace (അസത്യം);
    return_trace (സത്യം);
  }

  സ്വകാര്യ:
  hb_position_t get_caret_മൂല്യം (hb_font_t *font, hb_direction_t direction) സ്ഥിരം
  {
    തിരികെ HB_DIRECTION_IS_HORIZONTAL (direction) ? font->em_scale_x (coordinate) : font->em_scale_y (coordinate);
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്));
  }

  സംരക്ഷിത:
  HBUINT16	caret_മൂല്യം_Format;	/* Format identifier--format = 1 */
  FWORD		coordinate;		/* X or Y value, in design units */
  പൊതു:
  DEFINE_SIZE_STATIC (4);
};

ഘടന Caret_മൂല്യം_Format2
{
  friend ഘടന Caret_മൂല്യം;
  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    auto *out = c->serializer->embed (ഇത്);
    എങ്കിൽ (unlikely (!out)) return_trace (അസത്യം);
    return_trace (സത്യം);
  }

  സ്വകാര്യ:
  hb_position_t get_caret_മൂല്യം (hb_font_t *font, hb_direction_t direction, hb_codepoint_t glyph_id) സ്ഥിരം
  {
    hb_position_t x, y;
    font->get_glyph_contour_point_for_origin (glyph_id, caret_മൂല്യം_Point, direction, &x, &y);
    തിരികെ HB_DIRECTION_IS_HORIZONTAL (direction) ? x : y;
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്));
  }

  സംരക്ഷിത:
  HBUINT16	caret_മൂല്യം_Format;	/* Format identifier--format = 2 */
  HBUINT16	caret_മൂല്യം_Point;	/* Contour point index on glyph */
  പൊതു:
  DEFINE_SIZE_STATIC (4);
};

ഘടന Caret_മൂല്യം_Format3
{
  friend ഘടന Caret_മൂല്യം;

  hb_position_t get_caret_മൂല്യം (hb_font_t *font, hb_direction_t direction,
				 സ്ഥിരം ItemVariationStore &var_store) സ്ഥിരം
  {
    തിരികെ HB_DIRECTION_IS_HORIZONTAL (direction) ?
	   hb_saturate_add (font->em_scale_x (coordinate), (ഇത്+deviceTable).get_x_delta (font, var_store)) :
	   hb_saturate_add (font->em_scale_y (coordinate), (ഇത്+deviceTable).get_y_delta (font, var_store));
  }

  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    auto *out = c->serializer->start_embed (*ഇത്);
    എങ്കിൽ (!c->serializer->embed (caret_മൂല്യം_Format)) return_trace (അസത്യം);
    എങ്കിൽ (!c->serializer->embed (coordinate)) return_trace (അസത്യം);

    unsigned varidx = (ഇത്+deviceTable).get_variation_index ();
    hb_pair_t<unsigned, പൂർണ്ണസംഖ്യ> *new_varidx_delta;
    എങ്കിൽ (c->plan->layout_variation_idx_delta_map.has (varidx, &new_varidx_delta)) {
      uint32_t new_varidx = hb_first (*new_varidx_delta);
      പൂർണ്ണസംഖ്യ delta = hb_second (*new_varidx_delta);
      എങ്കിൽ (delta != 0)
      {
        എങ്കിൽ (!c->serializer->check_assign (out->coordinate, coordinate + delta, HB_SERIALIZE_ERROR_INT_OVERFLOW))
          return_trace (അസത്യം);
      }

      എങ്കിൽ (new_varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
        return_trace (c->serializer->check_assign (out->caret_മൂല്യം_Format, 1, HB_SERIALIZE_ERROR_INT_OVERFLOW));
    }

    എങ്കിൽ (!c->serializer->embed (deviceTable))
      return_trace (അസത്യം);

    return_trace (out->deviceTable.serialize_copy (c->serializer, deviceTable, ഇത്, c->serializer->to_bias (out),
						   hb_serialize_context_t::Head, &c->plan->layout_variation_idx_delta_map));
  }

  ശൂന്യം collect_variation_indices (hb_collect_variation_indices_context_t *c) സ്ഥിരം
  { (ഇത്+deviceTable).collect_variation_indices (c); }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്) && deviceTable.sanitize (c, ഇത്));
  }

  സംരക്ഷിത:
  HBUINT16	caret_മൂല്യം_Format;	/* Format identifier--format = 3 */
  FWORD		coordinate;		/* X or Y value, in design units */
  Offset16To<Device>
		deviceTable;		/* Offset to Device table for X or Y
					 * value--from beginning of CaretValue
					 * table */
  പൊതു:
  DEFINE_SIZE_STATIC (6);
};

ഘടന Caret_മൂല്യം
{
  hb_position_t get_caret_മൂല്യം (hb_font_t *font,
				 hb_direction_t direction,
				 hb_codepoint_t glyph_id,
				 സ്ഥിരം ItemVariationStore &var_store) സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (u.format.v) {
    അവസ്ഥ 1: hb_barrier (); തിരികെ u.format1.get_caret_മൂല്യം (font, direction);
    അവസ്ഥ 2: hb_barrier (); തിരികെ u.format2.get_caret_മൂല്യം (font, direction, glyph_id);
    അവസ്ഥ 3: hb_barrier (); തിരികെ u.format3.get_caret_മൂല്യം (font, direction, var_store);
    സ്ഥിരസ്ഥിതി:തിരികെ 0;
    }
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) സ്ഥിരം
  {
    എങ്കിൽ (unlikely (!c->may_dispatch (ഇത്, &u.format.v))) തിരികെ c->no_dispatch_return_മൂല്യം ();
    TRACE_DISPATCH (ഇത്, u.format.v);
    തിരഞ്ഞെടുപ്പ് (u.format.v) {
    അവസ്ഥ 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    അവസ്ഥ 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    അവസ്ഥ 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    സ്ഥിരസ്ഥിതി:return_trace (c->default_return_മൂല്യം ());
    }
  }

  ശൂന്യം collect_variation_indices (hb_collect_variation_indices_context_t *c) സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (u.format.v) {
    അവസ്ഥ 1:
    അവസ്ഥ 2:
      തിരികെ;
    അവസ്ഥ 3:
      hb_barrier ();
      u.format3.collect_variation_indices (c);
      തിരികെ;
    സ്ഥിരസ്ഥിതി: തിരികെ;
    }
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    എങ്കിൽ (!u.format.v.sanitize (c)) return_trace (അസത്യം);
    hb_barrier ();
    തിരഞ്ഞെടുപ്പ് (u.format.v) {
    അവസ്ഥ 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    അവസ്ഥ 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    അവസ്ഥ 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    സ്ഥിരസ്ഥിതി:return_trace (സത്യം);
    }
  }

  സംരക്ഷിത:
  സംഘം {
  ഘടന { HBUINT16 v; }	format;		/* Format identifier */
  Caret_മൂല്യം_Format1	format1;
  Caret_മൂല്യം_Format2	format2;
  Caret_മൂല്യം_Format3	format3;
  } u;
  പൊതു:
  DEFINE_SIZE_UNION (2, format.v);
};

ഘടന LigGlyph
{
  unsigned get_lig_carets (hb_font_t            *font,
			   hb_direction_t        direction,
			   hb_codepoint_t        glyph_id,
			   സ്ഥിരം ItemVariationStore &var_store,
			   unsigned              start_offset,
			   unsigned             *caret_എണ്ണം /* IN/OUT */,
			   hb_position_t        *caret_array /* OUT */) സ്ഥിരം
  {
    എങ്കിൽ (caret_എണ്ണം && caret_array)
    {
      + carets.as_array ().sub_array (start_offset, caret_എണ്ണം)
      | hb_map (hb_add (ഇത്))
      | hb_map ([&] (സ്ഥിരം Caret_മൂല്യം &value) { തിരികെ value.get_caret_മൂല്യം (font, direction, glyph_id, var_store); })
      | hb_sink (hb_array (caret_array, *caret_എണ്ണം))
      ;
    }

    തിരികെ carets.len;
  }

  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    auto *out = c->serializer->start_embed (*ഇത്);
    എങ്കിൽ (unlikely (!c->serializer->extend_min (out))) return_trace (അസത്യം);

    + hb_iter (carets)
    | hb_apply (subset_offset_array (c, out->carets, ഇത്))
    ;

    return_trace (തർക്കം (out->carets));
  }

  ശൂന്യം collect_variation_indices (hb_collect_variation_indices_context_t *c) സ്ഥിരം
  {
    വേണ്ടി (സ്ഥിരം Offset16To<Caret_മൂല്യം>& offset : carets.iter ())
      (ഇത്+offset).collect_variation_indices (c);
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (carets.sanitize (c, ഇത്));
  }

  സംരക്ഷിത:
  Array16OfOffset16To<Caret_മൂല്യം>
		carets;			/* Offset array of CaretValue tables
					 * --from beginning of LigGlyph table
					 * --in increasing coordinate order */
  പൊതു:
  DEFINE_SIZE_ARRAY (2, carets);
};

/* LigCaretList2 widens the LigCaretList coverage, count, and LigGlyph
 * offsets. The LigGlyph table itself keeps Offset16 caretValueOffsets. */
template <typename Types>
ഘടന LigCaretList
{
  unsigned പൂർണ്ണസംഖ്യ get_lig_carets (hb_font_t *font,
			       hb_direction_t direction,
			       hb_codepoint_t glyph_id,
			       സ്ഥിരം ItemVariationStore &var_store,
			       unsigned പൂർണ്ണസംഖ്യ start_offset,
			       unsigned പൂർണ്ണസംഖ്യ *caret_എണ്ണം /* IN/OUT */,
			       hb_position_t *caret_array /* OUT */) സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ index = (ഇത്+coverage).get_coverage (glyph_id);
    എങ്കിൽ (index == NOT_COVERED)
    {
      എങ്കിൽ (caret_എണ്ണം)
	*caret_എണ്ണം = 0;
      തിരികെ 0;
    }
    സ്ഥിരം LigGlyph &lig_glyph = ഇത്+ligGlyph[index];
    തിരികെ lig_glyph.get_lig_carets (font, direction, glyph_id, var_store, start_offset, caret_എണ്ണം, caret_array);
  }

  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    സ്ഥിരം hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    സ്ഥിരം hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*ഇത്);
    എങ്കിൽ (unlikely (!c->serializer->extend_min (out))) return_trace (അസത്യം);

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (ഇത്+coverage, ligGlyph)
    | hb_filter (glyphset, hb_first)
    | hb_filter (subset_offset_array (c, out->ligGlyph, ഇത്), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;
    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (തർക്കം (new_coverage));
  }

  ശൂന്യം collect_variation_indices (hb_collect_variation_indices_context_t *c) സ്ഥിരം
  {
    + hb_zip (ഇത്+coverage, ligGlyph)
    | hb_filter (c->glyph_set, hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (ഇത്))
    | hb_apply ([c] (സ്ഥിരം LigGlyph& _) { _.collect_variation_indices (c); })
    ;
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (coverage.sanitize (c, ഇത്) && ligGlyph.sanitize (c, ഇത്));
  }

  സംരക്ഷിത:
  typename Types::template LOffsetTo<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of LigCaretList table */
  typename Types::template ArrayOf<typename Types::template OffsetTo<LigGlyph>>
		ligGlyph;		/* Array of LigGlyph tables
					 * in Coverage Index order */
  പൊതു:
  DEFINE_SIZE_ARRAY (Types::LOffset::static_size + Types::HBUINT::static_size, ligGlyph);
};

#ifndef HB_NO_BEYOND_64K
static_assert (LigCaretList<MediumTypes>::min_size ==
	       Offset32::static_size + HBUINT24::static_size, "");
#endif


ഘടന MarkGlyphSetsFormat1
{
  തർക്കം covers (unsigned പൂർണ്ണസംഖ്യ set_index, hb_codepoint_t glyph_id) സ്ഥിരം
  { തിരികെ (ഇത്+coverage[set_index]).get_coverage (glyph_id) != NOT_COVERED; }

  ശൂന്യം collect_used_mark_sets (സ്ഥിരം hb_set_t& glyph_set,
                               hb_set_t& used_mark_sets /* OUT */) സ്ഥിരം
  {
    unsigned i = 0;
    വേണ്ടി (സ്ഥിരം auto &offset : coverage)
     {
       സ്ഥിരം auto &cov = ഇത്+offset;
       എങ്കിൽ (cov.intersects (&glyph_set))
         used_mark_sets.add (i);

       i++;
     }
  }

  template <typename set_t>
  ശൂന്യം collect_coverage (hb_vector_t<set_t> &sets) സ്ഥിരം
  {
     വേണ്ടി (സ്ഥിരം auto &offset : coverage)
     {
       സ്ഥിരം auto &cov = ഇത്+offset;
       cov.collect_coverage (sets.push ());
     }
  }

  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    auto *out = c->serializer->start_embed (*ഇത്);
    എങ്കിൽ (unlikely (!c->serializer->extend_min (out))) return_trace (അസത്യം);
    out->format = format;

    തർക്കം ret = സത്യം;
    വേണ്ടി (സ്ഥിരം Offset32To<Coverage>& offset : coverage.iter ())
    {
      auto snap = c->serializer->snapshot ();
      auto *o = out->coverage.serialize_append (c->serializer);
      എങ്കിൽ (unlikely (!o))
      {
	ret = അസത്യം;
	നിർത്തുക;
      }

      //skip empty coverage
      c->serializer->push ();
      തർക്കം res = അസത്യം;
      എങ്കിൽ (offset) res = c->dispatch (ഇത്+offset);
      എങ്കിൽ (!res)
      {
        c->serializer->എടുക്കുക_discard ();
        c->serializer->revert (snap);
        (out->coverage.len)--;
        തുടരുക;
      }
      c->serializer->add_link (*o, c->serializer->എടുക്കുക_pack ());
    }

    return_trace (ret && out->coverage.len);
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (coverage.sanitize (c, ഇത്));
  }

  സംരക്ഷിത:
  HBUINT16	format;			/* Format identifier--format = 1 */
  Array16Of<Offset32To<Coverage>>
		coverage;		/* Array of long offsets to mark set
					 * coverage tables */
  പൊതു:
  DEFINE_SIZE_ARRAY (4, coverage);
};

ഘടന MarkGlyphSets
{
  തർക്കം covers (unsigned പൂർണ്ണസംഖ്യ set_index, hb_codepoint_t glyph_id) സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (u.format.v) {
    അവസ്ഥ 1: hb_barrier (); തിരികെ u.format1.covers (set_index, glyph_id);
    സ്ഥിരസ്ഥിതി:തിരികെ അസത്യം;
    }
  }

  template <typename set_t>
  ശൂന്യം collect_coverage (hb_vector_t<set_t> &sets) സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (u.format.v) {
    അവസ്ഥ 1: hb_barrier (); u.format1.collect_coverage (sets); തിരികെ;
    സ്ഥിരസ്ഥിതി:തിരികെ;
    }
  }

  ശൂന്യം collect_used_mark_sets (സ്ഥിരം hb_set_t& glyph_set,
                               hb_set_t& used_mark_sets /* OUT */) സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (u.format.v) {
    അവസ്ഥ 1: hb_barrier (); u.format1.collect_used_mark_sets (glyph_set, used_mark_sets); തിരികെ;
    സ്ഥിരസ്ഥിതി:തിരികെ;
    }
  }

  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    തിരഞ്ഞെടുപ്പ് (u.format.v) {
    അവസ്ഥ 1: hb_barrier (); return_trace (u.format1.subset (c));
    സ്ഥിരസ്ഥിതി:return_trace (അസത്യം);
    }
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    എങ്കിൽ (!u.format.v.sanitize (c)) return_trace (അസത്യം);
    hb_barrier ();
    തിരഞ്ഞെടുപ്പ് (u.format.v) {
    അവസ്ഥ 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    സ്ഥിരസ്ഥിതി:return_trace (സത്യം);
    }
  }

  സംരക്ഷിത:
  സംഘം {
  ഘടന { HBUINT16 v; }	format;		/* Format identifier */
  MarkGlyphSetsFormat1	format1;
  } u;
  പൊതു:
  DEFINE_SIZE_UNION (2, format.v);
};


/*
 * GDEF -- Glyph Definition
 * https://docs.microsoft.com/en-us/typography/opentype/spec/gdef
 */


ഘടന GDEF
{
  സ്ഥിതം constexpr hb_tag_t tableTag = HB_OT_TAG_GDEF;

  എണ്ണൽ GlyphClasses {
    UnclassifiedGlyph	= 0,
    BaseGlyph		= 1,
    LigatureGlyph	= 2,
    MarkGlyph		= 3,
    ComponentGlyph	= 4
  };

  size_t get_size () സ്ഥിരം
  {
    എങ്കിൽ (version.major != 1) തിരികെ version.static_size;

    തിരികെ min_size +
	   (version.to_int () >= 0x00010002u ? markGlyphSetsDef.static_size : 0) +
	   (version.to_int () >= 0x00010003u ? varStore.static_size : 0) +
#ifndef HB_NO_BEYOND_64K
	   (version.to_int () >= 0x00010004u ? 5 * Offset32::static_size : 0)
#അല്ലെങ്കിൽ
	   0
#endif
	   ;
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    എങ്കിൽ (unlikely (!version.sanitize (c) ||
		  version.major != 1 ||
		  !c->check_range (ഇത്, get_size ())))
      return_trace (അസത്യം);
    hb_barrier ();

    തർക്കം glyph_class_def_ok = glyphClassDef.sanitize (c, ഇത്);
    തർക്കം attach_list_ok = attachList.sanitize (c, ഇത്);
    തർക്കം lig_caret_list_ok = ligCaretList.sanitize (c, ഇത്);
    തർക്കം mark_attach_class_def_ok = markAttachClassDef.sanitize (c, ഇത്);
    തർക്കം mark_glyph_sets_def_ok = version.to_int () < 0x00010002u ||
				  markGlyphSetsDef.sanitize (c, ഇത്);

#ifndef HB_NO_BEYOND_64K
    എങ്കിൽ (version.to_int () >= 0x00010004u)
    {
      എങ്കിൽ (glyphClassDef2) glyph_class_def_ok = glyphClassDef2.sanitize (c, ഇത്);
      എങ്കിൽ (attachList2) attach_list_ok = attachList2.sanitize (c, ഇത്);
      എങ്കിൽ (ligCaretList2) lig_caret_list_ok = ligCaretList2.sanitize (c, ഇത്);
      എങ്കിൽ (markAttachClassDef2) mark_attach_class_def_ok = markAttachClassDef2.sanitize (c, ഇത്);
      എങ്കിൽ (markGlyphSetsDef2) mark_glyph_sets_def_ok = markGlyphSetsDef2.sanitize (c, ഇത്);
    }
#endif

    return_trace (glyph_class_def_ok &&
		  attach_list_ok &&
		  lig_caret_list_ok &&
		  mark_attach_class_def_ok &&
		  mark_glyph_sets_def_ok &&
		  (version.to_int () < 0x00010003u || varStore.sanitize (c, ഇത്)));
  }

  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    എങ്കിൽ (version.major != 1) return_trace (അസത്യം);

    auto *out = c->serializer->start_embed (*ഇത്);
    എങ്കിൽ (unlikely (!c->serializer->extend_min (out))) return_trace (അസത്യം);

    // Push var store first (if it's needed) so that it's last in the
    // serialization order. Some font consumers assume that varstore runs to
    // the end of the GDEF table.
    // See: https://github.com/harfbuzz/harfbuzz/issues/4636
    auto snapshot_version0 = c->serializer->snapshot ();
    എങ്കിൽ (unlikely (version.to_int () >= 0x00010002u && hb_barrier () &&
		  !c->serializer->embed (markGlyphSetsDef)))
      return_trace (അസത്യം);

    auto snapshot_version2 = c->serializer->snapshot ();
    എങ്കിൽ (unlikely (version.to_int () >= 0x00010003u && hb_barrier () &&
		  !c->serializer->embed (varStore)))
      return_trace (അസത്യം);

    തർക്കം use_v14 = അസത്യം;
#ifndef HB_NO_BEYOND_64K
    use_v14 = version.to_int () >= 0x00010004u &&
	      (glyphClassDef2 || attachList2 || ligCaretList2 ||
	       markAttachClassDef2 || markGlyphSetsDef2);
    എങ്കിൽ (unlikely (version.to_int () >= 0x00010004u && hb_barrier () &&
		  !c->serializer->extend_size (out, get_size ())))
      return_trace (അസത്യം);
#endif

    തർക്കം subset_varstore = അസത്യം;
    unsigned varstore_index = (unsigned) -1;
    എങ്കിൽ (version.to_int () >= 0x00010003u && hb_barrier ())
    {
      എങ്കിൽ (c->plan->all_axes_pinned)
	out->varStore = 0;
      അല്ലെങ്കിൽ എങ്കിൽ (c->plan->normalized_coords)
      {
	എങ്കിൽ (varStore)
	{
	  item_variations_t item_vars;
	  എങ്കിൽ (unlikely (!c->serializer->check_success (
                  item_vars.instantiate (ഇത്+varStore, c->plan, സത്യം, സത്യം,
                                          c->plan->gdef_varstore_inner_maps.as_array (), സത്യം))))
            return_trace (അസത്യം);
	  subset_varstore = out->varStore.serialize_serialize (c->serializer,
							     item_vars.has_long_word (),
							     c->plan->axis_tags,
							     item_vars.get_region_list (),
							     item_vars.get_vardata_encodings ());
	  varstore_index = c->serializer->last_added_child_index();
	  remap_varidx_after_instantiation (item_vars.get_varidx_map (),
					    c->plan->layout_variation_idx_delta_map);
          remap_varidx_after_instantiation (item_vars.get_varidx_map (),
                                            c->plan->layout_condition_idx_മൂല്യം_map);
	}
      }
      അല്ലെങ്കിൽ
      {
	subset_varstore = out->varStore.serialize_subset (c, varStore, ഇത്,
							  c->plan->gdef_varstore_inner_maps.as_array ());
	varstore_index = c->serializer->last_added_child_index();
      }
    }

    out->version.major = version.major;
    out->version.minor = version.minor;

    എങ്കിൽ (!subset_varstore && version.to_int () >= 0x00010002u && !use_v14) {
      c->serializer->revert (snapshot_version2);
    }
    എങ്കിൽ (!subset_varstore)
      out->varStore = 0;
    അല്ലെങ്കിൽ
      c->plan->has_gdef_varstore = സത്യം;

    തർക്കം subset_markglyphsetsdef = അസത്യം;
    എങ്കിൽ (version.to_int () >= 0x00010002u && hb_barrier ())
    {
#ifndef HB_NO_BEYOND_64K
      എങ്കിൽ (use_v14 && markGlyphSetsDef2)
      {
	out->markGlyphSetsDef = 0;
	subset_markglyphsetsdef = serialize_subset_offset (c, out->markGlyphSetsDef2,
							   markGlyphSetsDef2, ഇത്);
      }
      അല്ലെങ്കിൽ
#endif
      {
#ifndef HB_NO_BEYOND_64K
	എങ്കിൽ (use_v14) out->markGlyphSetsDef2 = 0;
#endif
	subset_markglyphsetsdef = out->markGlyphSetsDef.serialize_subset (c, markGlyphSetsDef, ഇത്);
      }
    }

    എങ്കിൽ (use_v14)
    {
      out->version.minor = 4;
    } അല്ലെങ്കിൽ എങ്കിൽ (subset_varstore) {
      out->version.minor = 3;
    } അല്ലെങ്കിൽ എങ്കിൽ (subset_markglyphsetsdef) {
      out->version.minor = 2;
    } അല്ലെങ്കിൽ  {
      out->version.minor = 0;
      c->serializer->revert (snapshot_version0);
    }

    തർക്കം subset_glyphclassdef = അസത്യം;
    തർക്കം subset_attachlist = അസത്യം;
    തർക്കം subset_markattachclassdef = അസത്യം;
    തർക്കം subset_ligcaretlist = അസത്യം;

#ifndef HB_NO_BEYOND_64K
    എങ്കിൽ (use_v14 && glyphClassDef2)
    {
      out->glyphClassDef = 0;
      subset_glyphclassdef = serialize_subset_offset (c, out->glyphClassDef2,
						      glyphClassDef2, ഇത്, nullptr, അസത്യം, സത്യം);
    }
    അല്ലെങ്കിൽ
#endif
    {
#ifndef HB_NO_BEYOND_64K
      എങ്കിൽ (use_v14) out->glyphClassDef2 = 0;
#endif
      subset_glyphclassdef = out->glyphClassDef.serialize_subset (c, glyphClassDef, ഇത്, nullptr, അസത്യം, സത്യം);
    }

#ifndef HB_NO_BEYOND_64K
    എങ്കിൽ (use_v14 && attachList2)
    {
      out->attachList = 0;
      subset_attachlist = serialize_subset_offset (c, out->attachList2, attachList2, ഇത്);
    }
    അല്ലെങ്കിൽ
#endif
    {
#ifndef HB_NO_BEYOND_64K
      എങ്കിൽ (use_v14) out->attachList2 = 0;
#endif
      subset_attachlist = out->attachList.serialize_subset (c, attachList, ഇത്);
    }

#ifndef HB_NO_BEYOND_64K
    എങ്കിൽ (use_v14 && markAttachClassDef2)
    {
      out->markAttachClassDef = 0;
      subset_markattachclassdef = serialize_subset_offset (c, out->markAttachClassDef2,
							   markAttachClassDef2, ഇത്, nullptr, അസത്യം, സത്യം);
    }
    അല്ലെങ്കിൽ
#endif
    {
#ifndef HB_NO_BEYOND_64K
      എങ്കിൽ (use_v14) out->markAttachClassDef2 = 0;
#endif
      subset_markattachclassdef = out->markAttachClassDef.serialize_subset (c, markAttachClassDef,
									   ഇത്, nullptr, അസത്യം, സത്യം);
    }

#ifndef HB_NO_BEYOND_64K
    എങ്കിൽ (use_v14 && ligCaretList2)
    {
      out->ligCaretList = 0;
      subset_ligcaretlist = serialize_subset_offset (c, out->ligCaretList2, ligCaretList2, ഇത്);
    }
    അല്ലെങ്കിൽ
#endif
    {
#ifndef HB_NO_BEYOND_64K
      എങ്കിൽ (use_v14) out->ligCaretList2 = 0;
#endif
      subset_ligcaretlist = out->ligCaretList.serialize_subset (c, ligCaretList, ഇത്);
    }

    എങ്കിൽ (subset_varstore && varstore_index != (unsigned) -1) {
      c->serializer->repack_last(varstore_index);
    }

    return_trace (subset_glyphclassdef || subset_attachlist ||
		  subset_ligcaretlist || subset_markattachclassdef ||
		  (out->version.to_int () >= 0x00010002u && subset_markglyphsetsdef) ||
		  (out->version.to_int () >= 0x00010003u && subset_varstore));
  }

  തർക്കം has_glyph_classes () സ്ഥിരം
  {
#ifndef HB_NO_BEYOND_64K
    എങ്കിൽ (version.to_int () >= 0x00010004u && glyphClassDef2) തിരികെ സത്യം;
#endif
    തിരികെ glyphClassDef != 0;
  }
  സ്ഥിരം ClassDef &get_glyph_class_def () സ്ഥിരം
  {
#ifndef HB_NO_BEYOND_64K
    എങ്കിൽ (version.to_int () >= 0x00010004u && glyphClassDef2) തിരികെ ഇത്+glyphClassDef2;
#endif
    തിരികെ ഇത്+glyphClassDef;
  }
  തർക്കം has_attach_list () സ്ഥിരം
  {
#ifndef HB_NO_BEYOND_64K
    എങ്കിൽ (version.to_int () >= 0x00010004u && attachList2) തിരികെ സത്യം;
#endif
    തിരികെ attachList != 0;
  }
  സ്ഥിരം AttachList &get_attach_list () സ്ഥിരം
  {
#ifndef HB_NO_BEYOND_64K
    എങ്കിൽ (version.to_int () >= 0x00010004u && attachList2) തിരികെ ഇത്+attachList2;
#endif
    തിരികെ ഇത്+attachList;
  }
  തർക്കം has_lig_carets () സ്ഥിരം
  {
#ifndef HB_NO_BEYOND_64K
    എങ്കിൽ (version.to_int () >= 0x00010004u && ligCaretList2) തിരികെ സത്യം;
#endif
    തിരികെ ligCaretList != 0;
  }
  തർക്കം has_mark_attachment_types () സ്ഥിരം
  {
#ifndef HB_NO_BEYOND_64K
    എങ്കിൽ (version.to_int () >= 0x00010004u && markAttachClassDef2) തിരികെ സത്യം;
#endif
    തിരികെ markAttachClassDef != 0;
  }
  സ്ഥിരം ClassDef &get_mark_attach_class_def () സ്ഥിരം
  {
#ifndef HB_NO_BEYOND_64K
    എങ്കിൽ (version.to_int () >= 0x00010004u && markAttachClassDef2) തിരികെ ഇത്+markAttachClassDef2;
#endif
    തിരികെ ഇത്+markAttachClassDef;
  }
  തർക്കം has_mark_glyph_sets () സ്ഥിരം
  {
    എങ്കിൽ (version.to_int () < 0x00010002u) തിരികെ അസത്യം;
#ifndef HB_NO_BEYOND_64K
    എങ്കിൽ (version.to_int () >= 0x00010004u && markGlyphSetsDef2) തിരികെ സത്യം;
#endif
    തിരികെ markGlyphSetsDef != 0;
  }
  സ്ഥിരം MarkGlyphSets &get_mark_glyph_sets () സ്ഥിരം
  {
    എങ്കിൽ (version.to_int () < 0x00010002u) തിരികെ Null(MarkGlyphSets);
#ifndef HB_NO_BEYOND_64K
    എങ്കിൽ (version.to_int () >= 0x00010004u && markGlyphSetsDef2) തിരികെ ഇത്+markGlyphSetsDef2;
#endif
    തിരികെ ഇത്+markGlyphSetsDef;
  }
  തർക്കം has_var_store () സ്ഥിരം
  {
    തിരികെ version.to_int () >= 0x00010003u && hb_barrier () && varStore != 0;
  }
  സ്ഥിരം ItemVariationStore &get_var_store () സ്ഥിരം
  {
    തിരികെ version.to_int () >= 0x00010003u && hb_barrier () ? ഇത്+varStore : Null(ItemVariationStore);
  }


  തർക്കം has_data () സ്ഥിരം { തിരികെ version.to_int (); }
  unsigned പൂർണ്ണസംഖ്യ get_glyph_class (hb_codepoint_t glyph) സ്ഥിരം
  { തിരികെ get_glyph_class_def ().get_class (glyph); }
  ശൂന്യം get_glyphs_in_class (unsigned പൂർണ്ണസംഖ്യ klass, hb_set_t *glyphs) സ്ഥിരം
  { get_glyph_class_def ().collect_class (glyphs, klass); }

  unsigned പൂർണ്ണസംഖ്യ get_mark_attachment_type (hb_codepoint_t glyph) സ്ഥിരം
  { തിരികെ get_mark_attach_class_def ().get_class (glyph); }

  unsigned പൂർണ്ണസംഖ്യ get_attach_points (hb_codepoint_t glyph_id,
				  unsigned പൂർണ്ണസംഖ്യ start_offset,
				  unsigned പൂർണ്ണസംഖ്യ *point_എണ്ണം /* IN/OUT */,
				  unsigned പൂർണ്ണസംഖ്യ *point_array /* OUT */) സ്ഥിരം
  { തിരികെ get_attach_list ().get_attach_points (glyph_id, start_offset, point_എണ്ണം, point_array); }

  unsigned പൂർണ്ണസംഖ്യ get_lig_carets (hb_font_t *font,
			       hb_direction_t direction,
			       hb_codepoint_t glyph_id,
			       unsigned പൂർണ്ണസംഖ്യ start_offset,
			       unsigned പൂർണ്ണസംഖ്യ *caret_എണ്ണം /* IN/OUT */,
			       hb_position_t *caret_array /* OUT */) സ്ഥിരം
  {
#ifndef HB_NO_BEYOND_64K
    എങ്കിൽ (version.to_int () >= 0x00010004u && ligCaretList2)
      തിരികെ (ഇത്+ligCaretList2).get_lig_carets (font, direction, glyph_id, get_var_store (),
						  start_offset, caret_എണ്ണം, caret_array);
#endif
    തിരികെ (ഇത്+ligCaretList).get_lig_carets (font, direction, glyph_id, get_var_store (),
					       start_offset, caret_എണ്ണം, caret_array);
  }

  തർക്കം mark_set_covers (unsigned പൂർണ്ണസംഖ്യ set_index, hb_codepoint_t glyph_id) സ്ഥിരം
  { തിരികെ get_mark_glyph_sets ().covers (set_index, glyph_id); }

  /* glyph_props is a 16-bit integer where the lower 8-bit have bits representing
   * glyph class and other bits, and high 8-bit the mark attachment type (if any).
   * Not to be confused with lookup_props which is very similar. */
  unsigned പൂർണ്ണസംഖ്യ get_glyph_props (hb_codepoint_t glyph) സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ klass = get_glyph_class (glyph);

    static_assert (((unsigned പൂർണ്ണസംഖ്യ) HB_OT_LAYOUT_GLYPH_PROPS_BASE_GLYPH == (unsigned പൂർണ്ണസംഖ്യ) LookupFlag::IgnoreBaseGlyphs), "");
    static_assert (((unsigned പൂർണ്ണസംഖ്യ) HB_OT_LAYOUT_GLYPH_PROPS_LIGATURE == (unsigned പൂർണ്ണസംഖ്യ) LookupFlag::IgnoreLigatures), "");
    static_assert (((unsigned പൂർണ്ണസംഖ്യ) HB_OT_LAYOUT_GLYPH_PROPS_MARK == (unsigned പൂർണ്ണസംഖ്യ) LookupFlag::IgnoreMarks), "");

    തിരഞ്ഞെടുപ്പ് (klass) {
    സ്ഥിരസ്ഥിതി:			തിരികെ HB_OT_LAYOUT_GLYPH_CLASS_UNCLASSIFIED;
    അവസ്ഥ BaseGlyph:		തിരികെ HB_OT_LAYOUT_GLYPH_PROPS_BASE_GLYPH;
    അവസ്ഥ LigatureGlyph:		തിരികെ HB_OT_LAYOUT_GLYPH_PROPS_LIGATURE;
    അവസ്ഥ MarkGlyph:
	  klass = get_mark_attachment_type (glyph);
	  തിരികെ HB_OT_LAYOUT_GLYPH_PROPS_MARK | (klass << 8);
    }
  }

  HB_INTERNAL തർക്കം is_blocklisted (hb_blob_t *blob,
				   hb_face_t *face) സ്ഥിരം;

  ഘടന accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      table = hb_sanitize_context_t ().reference_table<GDEF> (face);
      എങ്കിൽ (unlikely (table->is_blocklisted (table.get_blob (), face)))
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
	എങ്കിൽ (!sets.in_error () &&
	    mark_glyph_set_bitmaps.resize (sets.length))
	  വേണ്ടി (unsigned i = 0; i < sets.length; i++)
	  {
	    auto &bitmap = mark_glyph_set_bitmaps.arrayZ[i];
	    bitmap.bias = HB_SET_മൂല്യം_INVALID;
	    സ്ഥിരം auto &set = sets.arrayZ[i];
	    എങ്കിൽ (unlikely (set.in_error ())) തുടരുക;
	    hb_codepoint_t min_gid = HB_SET_മൂല്യം_INVALID;
	    hb_codepoint_t max_gid = HB_SET_മൂല്യം_INVALID;
	    hb_set_next (&set, &min_gid);
	    hb_set_previous (&set, &max_gid);
	    എങ്കിൽ (max_gid == HB_SET_മൂല്യം_INVALID)
	      തുടരുക;
	    എങ്കിൽ (max_gid - min_gid >= hb_bit_page_t::PAGE_BITS)
	    {
	      /* Too wide for one page; a digest pre-filter instead. */
	      വേണ്ടി (hb_codepoint_t gid : set)
		bitmap.u.digest.add (gid);
	      തുടരുക;
	    }
	    bitmap.bias = min_gid;
	    വേണ്ടി (hb_codepoint_t gid : set)
	      bitmap.u.page.add (gid - min_gid);
	  }
      }
#endif
    }
    ~accelerator_t () { table.destroy (); }

    unsigned പൂർണ്ണസംഖ്യ get_glyph_props (hb_codepoint_t glyph) സ്ഥിരം
    {
      unsigned v;

#ifndef HB_NO_GDEF_CACHE
      എങ്കിൽ (glyph_props_cache.get (glyph, &v))
        തിരികെ v;
#endif

      v = table->get_glyph_props (glyph);

#ifndef HB_NO_GDEF_CACHE
      എങ്കിൽ (likely (table.get_blob ())) // Don't try setting if we are the null instance!
	glyph_props_cache.set (glyph, v);
#endif

      തിരികെ v;

    }

    HB_ALWAYS_INLINE
    തർക്കം mark_set_covers (unsigned പൂർണ്ണസംഖ്യ set_index, hb_codepoint_t glyph_id) സ്ഥിരം
    {
#ifndef HB_NO_GDEF_CACHE
      // We can access arrayZ directly because of sanitize_lookup_props() guarantee.
      സ്ഥിരം auto &bitmap = mark_glyph_set_bitmaps.arrayZ[set_index];
      എങ്കിൽ (likely (bitmap.bias != HB_SET_മൂല്യം_INVALID))
      {
	hb_codepoint_t biased = glyph_id - bitmap.bias;
	തിരികെ biased < hb_bit_page_t::PAGE_BITS &&
	       bitmap.u.page.get (biased);
      }
      തിരികെ mark_set_covers_slow (set_index, glyph_id);
#അല്ലെങ്കിൽ
      തിരികെ table->mark_set_covers (set_index, glyph_id);
#endif
    }

#ifndef HB_NO_GDEF_CACHE
    HB_NEVER_INLINE
    തർക്കം mark_set_covers_slow (unsigned പൂർണ്ണസംഖ്യ set_index, hb_codepoint_t glyph_id) സ്ഥിരം
    {
      സ്ഥിരം auto &bitmap = mark_glyph_set_bitmaps.arrayZ[set_index];
      എങ്കിൽ (!bitmap.u.digest.may_have (glyph_id))
	തിരികെ അസത്യം;
      തിരികെ table->mark_set_covers (set_index, glyph_id);
    }
#endif

    unsigned sanitize_lookup_props (unsigned lookup_props) സ്ഥിരം
    {
#ifndef HB_NO_GDEF_CACHE
      എങ്കിൽ (lookup_props & LookupFlag::UseMarkFilteringSet &&
	  (lookup_props >> 16) >= mark_glyph_set_bitmaps.length)
      {
        // Invalid mark filtering set index; unset the flag.
	lookup_props &= ~LookupFlag::UseMarkFilteringSet;
      }
#endif
      തിരികെ lookup_props;
    }

    hb_blob_ptr_t<GDEF> table;
#ifndef HB_NO_GDEF_CACHE
    ഘടന mark_glyph_set_bitmap_t
    {
      /* Gid of the first bit (not page-aligned) for an exact one-page
       * set, or HB_SET_VALUE_INVALID; then digest is a pre-filter for
       * the table.  The all-zeros state is valid for both members. */
      hb_codepoint_t bias = HB_SET_മൂല്യം_INVALID;
      സംഘം u_t {
	u_t () : page () {}
	hb_bit_page_t page;
	hb_set_digest_t digest;
      } u;
    };

    hb_vector_t<mark_glyph_set_bitmap_t> mark_glyph_set_bitmaps;
    mutable hb_cache_t<21, 3> glyph_props_cache;
    static_assert (വലുപ്പം (glyph_props_cache) == 512, "");
#endif
  };

  ശൂന്യം collect_variation_indices (hb_collect_variation_indices_context_t *c) സ്ഥിരം
  {
#ifndef HB_NO_BEYOND_64K
    എങ്കിൽ (version.to_int () >= 0x00010004u && ligCaretList2)
    {
      (ഇത്+ligCaretList2).collect_variation_indices (c);
      തിരികെ;
    }
#endif
    (ഇത്+ligCaretList).collect_variation_indices (c);
  }

  സംരക്ഷിത:
  template <typename താക്കോൽ>
  സ്ഥിതം ശൂന്യം remap_varidx_after_instantiation (സ്ഥിരം hb_map_t& varidx_map,
                                                hb_hashmap_t<താക്കോൽ, hb_pair_t<unsigned, പൂർണ്ണസംഖ്യ>>& layout_variation_idx_delta_map /* IN/OUT */)
  {
    /* varidx_map is empty which means varstore is empty after instantiation,
     * no variations, map all varidx to HB_OT_LAYOUT_NO_VARIATIONS_INDEX.
     * varidx_map doesn't have original varidx, indicating delta row is all
     * zeros, map varidx to HB_OT_LAYOUT_NO_VARIATIONS_INDEX */
    വേണ്ടി (auto _ : layout_variation_idx_delta_map.iter_ref ())
    {
      /* old_varidx->(varidx, delta) mapping generated for subsetting, then this
       * varidx is used as key of varidx_map during instantiation */
      uint32_t varidx = _.second.first;
      uint32_t *new_varidx;
      എങ്കിൽ (varidx_map.has (varidx, &new_varidx))
	_.second.first = *new_varidx;
      അല്ലെങ്കിൽ
	_.second.first = HB_OT_LAYOUT_NO_VARIATIONS_INDEX;
    }
  }

  template <typename OffsetOut, typename OffsetIn, typename Base, typename ...Ts>
  സ്ഥിതം തർക്കം serialize_subset_offset (hb_subset_context_t *c,
				       OffsetOut& out,
				       സ്ഥിരം OffsetIn& in,
				       സ്ഥിരം Base *base,
				       Ts&&... ds)
  {
    out = 0;
    എങ്കിൽ (in.is_null ()) തിരികെ അസത്യം;

    auto *s = c->serializer;
    s->push ();
    തർക്കം ret = c->dispatch (base+in, std::forward<Ts> (ds)...);
    എങ്കിൽ (ret) s->add_link (out, s->എടുക്കുക_pack ());
    അല്ലെങ്കിൽ s->എടുക്കുക_discard ();
    തിരികെ ret;
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
  പൊതു:
  DEFINE_SIZE_MIN (12);
};

ഘടന GDEF_accelerator_t : GDEF::accelerator_t {
  GDEF_accelerator_t (hb_face_t *face) : GDEF::accelerator_t (face) {}
};

} /* namespace OT */


#endif /* OT_LAYOUT_GDEF_GDEF_HH */
