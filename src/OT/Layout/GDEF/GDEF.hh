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

#অন্তর্ভাব "../../../hb-ot-var-common.hh"

#অন্তর্ভাব "../../../hb-font.hh"
#অন্তর্ভাব "../../../hb-cache.hh"


নামাকাশ OT {


/*
 * Attachment List Table
 */

/* Array of contour point indices--in increasing numerical order */
সংরচনা AttachPoint : Array16Of<HBUINT16>
{
  তার্কিক subset (hb_subset_context_t *c) ধ্রুব
  {
    TRACE_SUBSET (এতত্);
    auto *out = c->serializer->start_embed (*এতত্);
    return_trace (out->serialize (c->serializer, + iter ()));
  }
};

/* AttachList is referenced by both GDEF attachListOffset and
 * attachListOffset2. The v1.4 top-level offset widens, but the
 * AttachList table itself keeps Offset16 internals. */
সংরচনা AttachList
{
  unsigned পূর্ণাঙ্ক get_attach_points (hb_codepoint_t glyph_id,
				  unsigned পূর্ণাঙ্ক start_offset,
				  unsigned পূর্ণাঙ্ক *point_গুনতি /* IN/OUT */,
				  unsigned পূর্ণাঙ্ক *point_array /* OUT */) ধ্রুব
  {
    unsigned পূর্ণাঙ্ক index = (এতত্+coverage).get_coverage (glyph_id);
    যদি (index == NOT_COVERED)
    {
      যদি (point_গুনতি)
	*point_গুনতি = 0;
      প্রত্যাগম 0;
    }

    ধ্রুব AttachPoint &points = এতত্+attachPoint[index];

    যদি (point_গুনতি && point_array)
    {
      + points.as_array ().sub_array (start_offset, point_গুনতি)
      | hb_sink (hb_array (point_array, *point_গুনতি))
      ;
    }

    প্রত্যাগম points.len;
  }

  তার্কিক subset (hb_subset_context_t *c) ধ্রুব
  {
    TRACE_SUBSET (এতত্);
    ধ্রুব hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ধ্রুব hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*এতত্);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (অসত্য);

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (এতত্+coverage, attachPoint)
    | hb_filter (glyphset, hb_first)
    | hb_filter (subset_offset_array (c, out->attachPoint, এতত্), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;
    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (তার্কিক (new_coverage));
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (coverage.sanitize (c, এতত্) && attachPoint.sanitize (c, এতত্));
  }

  রক্ষিত:
  Offset16To<Coverage>
		coverage;		/* Offset to Coverage table -- from
					 * beginning of AttachList table */
  Array16OfOffset16To<AttachPoint>
		attachPoint;		/* Array of AttachPoint tables
					 * in Coverage Index order */
  সার্বজনিক:
  DEFINE_SIZE_ARRAY (4, attachPoint);
};

/*
 * Ligature Caret Table
 */

সংরচনা Caret_মান_Format1
{
  friend সংরচনা Caret_মান;
  তার্কিক subset (hb_subset_context_t *c) ধ্রুব
  {
    TRACE_SUBSET (এতত্);
    auto *out = c->serializer->embed (এতত্);
    যদি (unlikely (!out)) return_trace (অসত্য);
    return_trace (সত্য);
  }

  গুপ্ত:
  hb_position_t get_caret_মান (hb_font_t *font, hb_direction_t direction) ধ্রুব
  {
    প্রত্যাগম HB_DIRECTION_IS_HORIZONTAL (direction) ? font->em_scale_x (coordinate) : font->em_scale_y (coordinate);
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্));
  }

  রক্ষিত:
  HBUINT16	caret_মান_Format;	/* Format identifier--format = 1 */
  FWORD		coordinate;		/* X or Y value, in design units */
  সার্বজনিক:
  DEFINE_SIZE_STATIC (4);
};

সংরচনা Caret_মান_Format2
{
  friend সংরচনা Caret_মান;
  তার্কিক subset (hb_subset_context_t *c) ধ্রুব
  {
    TRACE_SUBSET (এতত্);
    auto *out = c->serializer->embed (এতত্);
    যদি (unlikely (!out)) return_trace (অসত্য);
    return_trace (সত্য);
  }

  গুপ্ত:
  hb_position_t get_caret_মান (hb_font_t *font, hb_direction_t direction, hb_codepoint_t glyph_id) ধ্রুব
  {
    hb_position_t x, y;
    font->get_glyph_contour_point_for_origin (glyph_id, caret_মান_Point, direction, &x, &y);
    প্রত্যাগম HB_DIRECTION_IS_HORIZONTAL (direction) ? x : y;
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্));
  }

  রক্ষিত:
  HBUINT16	caret_মান_Format;	/* Format identifier--format = 2 */
  HBUINT16	caret_মান_Point;	/* Contour point index on glyph */
  সার্বজনিক:
  DEFINE_SIZE_STATIC (4);
};

সংরচনা Caret_মান_Format3
{
  friend সংরচনা Caret_মান;

  hb_position_t get_caret_মান (hb_font_t *font, hb_direction_t direction,
				 ধ্রুব ItemVariationStore &var_store) ধ্রুব
  {
    প্রত্যাগম HB_DIRECTION_IS_HORIZONTAL (direction) ?
	   hb_saturate_add (font->em_scale_x (coordinate), (এতত্+deviceTable).get_x_delta (font, var_store)) :
	   hb_saturate_add (font->em_scale_y (coordinate), (এতত্+deviceTable).get_y_delta (font, var_store));
  }

  তার্কিক subset (hb_subset_context_t *c) ধ্রুব
  {
    TRACE_SUBSET (এতত্);
    auto *out = c->serializer->start_embed (*এতত্);
    যদি (!c->serializer->embed (caret_মান_Format)) return_trace (অসত্য);
    যদি (!c->serializer->embed (coordinate)) return_trace (অসত্য);

    unsigned varidx = (এতত্+deviceTable).get_variation_index ();
    hb_pair_t<unsigned, পূর্ণাঙ্ক> *new_varidx_delta;
    যদি (c->plan->layout_variation_idx_delta_map.has (varidx, &new_varidx_delta)) {
      uint32_t new_varidx = hb_first (*new_varidx_delta);
      পূর্ণাঙ্ক delta = hb_second (*new_varidx_delta);
      যদি (delta != 0)
      {
        যদি (!c->serializer->check_assign (out->coordinate, coordinate + delta, HB_SERIALIZE_ERROR_INT_OVERFLOW))
          return_trace (অসত্য);
      }

      যদি (new_varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
        return_trace (c->serializer->check_assign (out->caret_মান_Format, 1, HB_SERIALIZE_ERROR_INT_OVERFLOW));
    }

    যদি (!c->serializer->embed (deviceTable))
      return_trace (অসত্য);

    return_trace (out->deviceTable.serialize_copy (c->serializer, deviceTable, এতত্, c->serializer->to_bias (out),
						   hb_serialize_context_t::Head, &c->plan->layout_variation_idx_delta_map));
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c) ধ্রুব
  { (এতত্+deviceTable).collect_variation_indices (c); }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) && deviceTable.sanitize (c, এতত্));
  }

  রক্ষিত:
  HBUINT16	caret_মান_Format;	/* Format identifier--format = 3 */
  FWORD		coordinate;		/* X or Y value, in design units */
  Offset16To<Device>
		deviceTable;		/* Offset to Device table for X or Y
					 * value--from beginning of CaretValue
					 * table */
  সার্বজনিক:
  DEFINE_SIZE_STATIC (6);
};

সংরচনা Caret_মান
{
  hb_position_t get_caret_মান (hb_font_t *font,
				 hb_direction_t direction,
				 hb_codepoint_t glyph_id,
				 ধ্রুব ItemVariationStore &var_store) ধ্রুব
  {
    বিকল্প (u.format.v) {
    পক্ষ 1: hb_barrier (); প্রত্যাগম u.format1.get_caret_মান (font, direction);
    পক্ষ 2: hb_barrier (); প্রত্যাগম u.format2.get_caret_মান (font, direction, glyph_id);
    পক্ষ 3: hb_barrier (); প্রত্যাগম u.format3.get_caret_মান (font, direction, var_store);
    স্বভাব:প্রত্যাগম 0;
    }
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ধ্রুব
  {
    যদি (unlikely (!c->may_dispatch (এতত্, &u.format.v))) প্রত্যাগম c->no_dispatch_return_মান ();
    TRACE_DISPATCH (এতত্, u.format.v);
    বিকল্প (u.format.v) {
    পক্ষ 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    পক্ষ 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    পক্ষ 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    স্বভাব:return_trace (c->default_return_মান ());
    }
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c) ধ্রুব
  {
    বিকল্প (u.format.v) {
    পক্ষ 1:
    পক্ষ 2:
      প্রত্যাগম;
    পক্ষ 3:
      hb_barrier ();
      u.format3.collect_variation_indices (c);
      প্রত্যাগম;
    স্বভাব: প্রত্যাগম;
    }
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    যদি (!u.format.v.sanitize (c)) return_trace (অসত্য);
    hb_barrier ();
    বিকল্প (u.format.v) {
    পক্ষ 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    পক্ষ 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    পক্ষ 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    স্বভাব:return_trace (সত্য);
    }
  }

  রক্ষিত:
  সঙ্ঘ {
  সংরচনা { HBUINT16 v; }	format;		/* Format identifier */
  Caret_মান_Format1	format1;
  Caret_মান_Format2	format2;
  Caret_মান_Format3	format3;
  } u;
  সার্বজনিক:
  DEFINE_SIZE_UNION (2, format.v);
};

সংরচনা LigGlyph
{
  unsigned get_lig_carets (hb_font_t            *font,
			   hb_direction_t        direction,
			   hb_codepoint_t        glyph_id,
			   ধ্রুব ItemVariationStore &var_store,
			   unsigned              start_offset,
			   unsigned             *caret_গুনতি /* IN/OUT */,
			   hb_position_t        *caret_array /* OUT */) ধ্রুব
  {
    যদি (caret_গুনতি && caret_array)
    {
      + carets.as_array ().sub_array (start_offset, caret_গুনতি)
      | hb_map (hb_add (এতত্))
      | hb_map ([&] (ধ্রুব Caret_মান &value) { প্রত্যাগম value.get_caret_মান (font, direction, glyph_id, var_store); })
      | hb_sink (hb_array (caret_array, *caret_গুনতি))
      ;
    }

    প্রত্যাগম carets.len;
  }

  তার্কিক subset (hb_subset_context_t *c) ধ্রুব
  {
    TRACE_SUBSET (এতত্);
    auto *out = c->serializer->start_embed (*এতত্);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (অসত্য);

    + hb_iter (carets)
    | hb_apply (subset_offset_array (c, out->carets, এতত্))
    ;

    return_trace (তার্কিক (out->carets));
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c) ধ্রুব
  {
    প্রতি (ধ্রুব Offset16To<Caret_মান>& offset : carets.iter ())
      (এতত্+offset).collect_variation_indices (c);
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (carets.sanitize (c, এতত্));
  }

  রক্ষিত:
  Array16OfOffset16To<Caret_মান>
		carets;			/* Offset array of CaretValue tables
					 * --from beginning of LigGlyph table
					 * --in increasing coordinate order */
  সার্বজনিক:
  DEFINE_SIZE_ARRAY (2, carets);
};

/* LigCaretList2 widens the LigCaretList coverage, count, and LigGlyph
 * offsets. The LigGlyph table itself keeps Offset16 caretValueOffsets. */
template <typename Types>
সংরচনা LigCaretList
{
  unsigned পূর্ণাঙ্ক get_lig_carets (hb_font_t *font,
			       hb_direction_t direction,
			       hb_codepoint_t glyph_id,
			       ধ্রুব ItemVariationStore &var_store,
			       unsigned পূর্ণাঙ্ক start_offset,
			       unsigned পূর্ণাঙ্ক *caret_গুনতি /* IN/OUT */,
			       hb_position_t *caret_array /* OUT */) ধ্রুব
  {
    unsigned পূর্ণাঙ্ক index = (এতত্+coverage).get_coverage (glyph_id);
    যদি (index == NOT_COVERED)
    {
      যদি (caret_গুনতি)
	*caret_গুনতি = 0;
      প্রত্যাগম 0;
    }
    ধ্রুব LigGlyph &lig_glyph = এতত্+ligGlyph[index];
    প্রত্যাগম lig_glyph.get_lig_carets (font, direction, glyph_id, var_store, start_offset, caret_গুনতি, caret_array);
  }

  তার্কিক subset (hb_subset_context_t *c) ধ্রুব
  {
    TRACE_SUBSET (এতত্);
    ধ্রুব hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ধ্রুব hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*এতত্);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (অসত্য);

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (এতত্+coverage, ligGlyph)
    | hb_filter (glyphset, hb_first)
    | hb_filter (subset_offset_array (c, out->ligGlyph, এতত্), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;
    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (তার্কিক (new_coverage));
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c) ধ্রুব
  {
    + hb_zip (এতত্+coverage, ligGlyph)
    | hb_filter (c->glyph_set, hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (এতত্))
    | hb_apply ([c] (ধ্রুব LigGlyph& _) { _.collect_variation_indices (c); })
    ;
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (coverage.sanitize (c, এতত্) && ligGlyph.sanitize (c, এতত্));
  }

  রক্ষিত:
  typename Types::template LOffsetTo<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of LigCaretList table */
  typename Types::template ArrayOf<typename Types::template OffsetTo<LigGlyph>>
		ligGlyph;		/* Array of LigGlyph tables
					 * in Coverage Index order */
  সার্বজনিক:
  DEFINE_SIZE_ARRAY (Types::LOffset::static_size + Types::HBUINT::static_size, ligGlyph);
};

#ifndef HB_NO_BEYOND_64K
static_assert (LigCaretList<MediumTypes>::min_size ==
	       Offset32::static_size + HBUINT24::static_size, "");
#endif


সংরচনা MarkGlyphSetsFormat1
{
  তার্কিক covers (unsigned পূর্ণাঙ্ক set_index, hb_codepoint_t glyph_id) ধ্রুব
  { প্রত্যাগম (এতত্+coverage[set_index]).get_coverage (glyph_id) != NOT_COVERED; }

  শূন্য collect_used_mark_sets (ধ্রুব hb_set_t& glyph_set,
                               hb_set_t& used_mark_sets /* OUT */) ধ্রুব
  {
    unsigned i = 0;
    প্রতি (ধ্রুব auto &offset : coverage)
     {
       ধ্রুব auto &cov = এতত্+offset;
       যদি (cov.intersects (&glyph_set))
         used_mark_sets.add (i);

       i++;
     }
  }

  template <typename set_t>
  শূন্য collect_coverage (hb_vector_t<set_t> &sets) ধ্রুব
  {
     প্রতি (ধ্রুব auto &offset : coverage)
     {
       ধ্রুব auto &cov = এতত্+offset;
       cov.collect_coverage (sets.push ());
     }
  }

  তার্কিক subset (hb_subset_context_t *c) ধ্রুব
  {
    TRACE_SUBSET (এতত্);
    auto *out = c->serializer->start_embed (*এতত্);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (অসত্য);
    out->format = format;

    তার্কিক ret = সত্য;
    প্রতি (ধ্রুব Offset32To<Coverage>& offset : coverage.iter ())
    {
      auto snap = c->serializer->snapshot ();
      auto *o = out->coverage.serialize_append (c->serializer);
      যদি (unlikely (!o))
      {
	ret = অসত্য;
	ভঙ্গ;
      }

      //skip empty coverage
      c->serializer->push ();
      তার্কিক res = অসত্য;
      যদি (offset) res = c->dispatch (এতত্+offset);
      যদি (!res)
      {
        c->serializer->বের_করো_discard ();
        c->serializer->revert (snap);
        (out->coverage.len)--;
        অনুবর্ত;
      }
      c->serializer->add_link (*o, c->serializer->বের_করো_pack ());
    }

    return_trace (ret && out->coverage.len);
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (coverage.sanitize (c, এতত্));
  }

  রক্ষিত:
  HBUINT16	format;			/* Format identifier--format = 1 */
  Array16Of<Offset32To<Coverage>>
		coverage;		/* Array of long offsets to mark set
					 * coverage tables */
  সার্বজনিক:
  DEFINE_SIZE_ARRAY (4, coverage);
};

সংরচনা MarkGlyphSets
{
  তার্কিক covers (unsigned পূর্ণাঙ্ক set_index, hb_codepoint_t glyph_id) ধ্রুব
  {
    বিকল্প (u.format.v) {
    পক্ষ 1: hb_barrier (); প্রত্যাগম u.format1.covers (set_index, glyph_id);
    স্বভাব:প্রত্যাগম অসত্য;
    }
  }

  template <typename set_t>
  শূন্য collect_coverage (hb_vector_t<set_t> &sets) ধ্রুব
  {
    বিকল্প (u.format.v) {
    পক্ষ 1: hb_barrier (); u.format1.collect_coverage (sets); প্রত্যাগম;
    স্বভাব:প্রত্যাগম;
    }
  }

  শূন্য collect_used_mark_sets (ধ্রুব hb_set_t& glyph_set,
                               hb_set_t& used_mark_sets /* OUT */) ধ্রুব
  {
    বিকল্প (u.format.v) {
    পক্ষ 1: hb_barrier (); u.format1.collect_used_mark_sets (glyph_set, used_mark_sets); প্রত্যাগম;
    স্বভাব:প্রত্যাগম;
    }
  }

  তার্কিক subset (hb_subset_context_t *c) ধ্রুব
  {
    TRACE_SUBSET (এতত্);
    বিকল্প (u.format.v) {
    পক্ষ 1: hb_barrier (); return_trace (u.format1.subset (c));
    স্বভাব:return_trace (অসত্য);
    }
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    যদি (!u.format.v.sanitize (c)) return_trace (অসত্য);
    hb_barrier ();
    বিকল্প (u.format.v) {
    পক্ষ 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    স্বভাব:return_trace (সত্য);
    }
  }

  রক্ষিত:
  সঙ্ঘ {
  সংরচনা { HBUINT16 v; }	format;		/* Format identifier */
  MarkGlyphSetsFormat1	format1;
  } u;
  সার্বজনিক:
  DEFINE_SIZE_UNION (2, format.v);
};


/*
 * GDEF -- Glyph Definition
 * https://docs.microsoft.com/en-us/typography/opentype/spec/gdef
 */


সংরচনা GDEF
{
  স্থির constexpr hb_tag_t tableTag = HB_OT_TAG_GDEF;

  গণনা GlyphClasses {
    UnclassifiedGlyph	= 0,
    BaseGlyph		= 1,
    LigatureGlyph	= 2,
    MarkGlyph		= 3,
    ComponentGlyph	= 4
  };

  size_t get_size () ধ্রুব
  {
    যদি (version.major != 1) প্রত্যাগম version.static_size;

    প্রত্যাগম min_size +
	   (version.to_int () >= 0x00010002u ? markGlyphSetsDef.static_size : 0) +
	   (version.to_int () >= 0x00010003u ? varStore.static_size : 0) +
#ifndef HB_NO_BEYOND_64K
	   (version.to_int () >= 0x00010004u ? 5 * Offset32::static_size : 0)
#অন্যথা
	   0
#endif
	   ;
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    যদি (unlikely (!version.sanitize (c) ||
		  version.major != 1 ||
		  !c->check_range (এতত্, get_size ())))
      return_trace (অসত্য);
    hb_barrier ();

    তার্কিক glyph_class_def_ok = glyphClassDef.sanitize (c, এতত্);
    তার্কিক attach_list_ok = attachList.sanitize (c, এতত্);
    তার্কিক lig_caret_list_ok = ligCaretList.sanitize (c, এতত্);
    তার্কিক mark_attach_class_def_ok = markAttachClassDef.sanitize (c, এতত্);
    তার্কিক mark_glyph_sets_def_ok = version.to_int () < 0x00010002u ||
				  markGlyphSetsDef.sanitize (c, এতত্);

#ifndef HB_NO_BEYOND_64K
    যদি (version.to_int () >= 0x00010004u)
    {
      যদি (glyphClassDef2) glyph_class_def_ok = glyphClassDef2.sanitize (c, এতত্);
      যদি (attachList2) attach_list_ok = attachList2.sanitize (c, এতত্);
      যদি (ligCaretList2) lig_caret_list_ok = ligCaretList2.sanitize (c, এতত্);
      যদি (markAttachClassDef2) mark_attach_class_def_ok = markAttachClassDef2.sanitize (c, এতত্);
      যদি (markGlyphSetsDef2) mark_glyph_sets_def_ok = markGlyphSetsDef2.sanitize (c, এতত্);
    }
#endif

    return_trace (glyph_class_def_ok &&
		  attach_list_ok &&
		  lig_caret_list_ok &&
		  mark_attach_class_def_ok &&
		  mark_glyph_sets_def_ok &&
		  (version.to_int () < 0x00010003u || varStore.sanitize (c, এতত্)));
  }

  তার্কিক subset (hb_subset_context_t *c) ধ্রুব
  {
    TRACE_SUBSET (এতত্);
    যদি (version.major != 1) return_trace (অসত্য);

    auto *out = c->serializer->start_embed (*এতত্);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (অসত্য);

    // Push var store first (if it's needed) so that it's last in the
    // serialization order. Some font consumers assume that varstore runs to
    // the end of the GDEF table.
    // See: https://github.com/harfbuzz/harfbuzz/issues/4636
    auto snapshot_version0 = c->serializer->snapshot ();
    যদি (unlikely (version.to_int () >= 0x00010002u && hb_barrier () &&
		  !c->serializer->embed (markGlyphSetsDef)))
      return_trace (অসত্য);

    auto snapshot_version2 = c->serializer->snapshot ();
    যদি (unlikely (version.to_int () >= 0x00010003u && hb_barrier () &&
		  !c->serializer->embed (varStore)))
      return_trace (অসত্য);

    তার্কিক use_v14 = অসত্য;
#ifndef HB_NO_BEYOND_64K
    use_v14 = version.to_int () >= 0x00010004u &&
	      (glyphClassDef2 || attachList2 || ligCaretList2 ||
	       markAttachClassDef2 || markGlyphSetsDef2);
    যদি (unlikely (version.to_int () >= 0x00010004u && hb_barrier () &&
		  !c->serializer->extend_size (out, get_size ())))
      return_trace (অসত্য);
#endif

    তার্কিক subset_varstore = অসত্য;
    unsigned varstore_index = (unsigned) -1;
    যদি (version.to_int () >= 0x00010003u && hb_barrier ())
    {
      যদি (c->plan->all_axes_pinned)
	out->varStore = 0;
      অন্যথা যদি (c->plan->normalized_coords)
      {
	যদি (varStore)
	{
	  item_variations_t item_vars;
	  যদি (unlikely (!c->serializer->check_success (
                  item_vars.instantiate (এতত্+varStore, c->plan, সত্য, সত্য,
                                          c->plan->gdef_varstore_inner_maps.as_array (), সত্য))))
            return_trace (অসত্য);
	  subset_varstore = out->varStore.serialize_serialize (c->serializer,
							     item_vars.has_long_word (),
							     c->plan->axis_tags,
							     item_vars.get_region_list (),
							     item_vars.get_vardata_encodings ());
	  varstore_index = c->serializer->last_added_child_index();
	  remap_varidx_after_instantiation (item_vars.get_varidx_map (),
					    c->plan->layout_variation_idx_delta_map);
          remap_varidx_after_instantiation (item_vars.get_varidx_map (),
                                            c->plan->layout_condition_idx_মান_map);
	}
      }
      অন্যথা
      {
	subset_varstore = out->varStore.serialize_subset (c, varStore, এতত্,
							  c->plan->gdef_varstore_inner_maps.as_array ());
	varstore_index = c->serializer->last_added_child_index();
      }
    }

    out->version.major = version.major;
    out->version.minor = version.minor;

    যদি (!subset_varstore && version.to_int () >= 0x00010002u && !use_v14) {
      c->serializer->revert (snapshot_version2);
    }
    যদি (!subset_varstore)
      out->varStore = 0;
    অন্যথা
      c->plan->has_gdef_varstore = সত্য;

    তার্কিক subset_markglyphsetsdef = অসত্য;
    যদি (version.to_int () >= 0x00010002u && hb_barrier ())
    {
#ifndef HB_NO_BEYOND_64K
      যদি (use_v14 && markGlyphSetsDef2)
      {
	out->markGlyphSetsDef = 0;
	subset_markglyphsetsdef = serialize_subset_offset (c, out->markGlyphSetsDef2,
							   markGlyphSetsDef2, এতত্);
      }
      অন্যথা
#endif
      {
#ifndef HB_NO_BEYOND_64K
	যদি (use_v14) out->markGlyphSetsDef2 = 0;
#endif
	subset_markglyphsetsdef = out->markGlyphSetsDef.serialize_subset (c, markGlyphSetsDef, এতত্);
      }
    }

    যদি (use_v14)
    {
      out->version.minor = 4;
    } অন্যথা যদি (subset_varstore) {
      out->version.minor = 3;
    } অন্যথা যদি (subset_markglyphsetsdef) {
      out->version.minor = 2;
    } অন্যথা  {
      out->version.minor = 0;
      c->serializer->revert (snapshot_version0);
    }

    তার্কিক subset_glyphclassdef = অসত্য;
    তার্কিক subset_attachlist = অসত্য;
    তার্কিক subset_markattachclassdef = অসত্য;
    তার্কিক subset_ligcaretlist = অসত্য;

#ifndef HB_NO_BEYOND_64K
    যদি (use_v14 && glyphClassDef2)
    {
      out->glyphClassDef = 0;
      subset_glyphclassdef = serialize_subset_offset (c, out->glyphClassDef2,
						      glyphClassDef2, এতত্, nullptr, অসত্য, সত্য);
    }
    অন্যথা
#endif
    {
#ifndef HB_NO_BEYOND_64K
      যদি (use_v14) out->glyphClassDef2 = 0;
#endif
      subset_glyphclassdef = out->glyphClassDef.serialize_subset (c, glyphClassDef, এতত্, nullptr, অসত্য, সত্য);
    }

#ifndef HB_NO_BEYOND_64K
    যদি (use_v14 && attachList2)
    {
      out->attachList = 0;
      subset_attachlist = serialize_subset_offset (c, out->attachList2, attachList2, এতত্);
    }
    অন্যথা
#endif
    {
#ifndef HB_NO_BEYOND_64K
      যদি (use_v14) out->attachList2 = 0;
#endif
      subset_attachlist = out->attachList.serialize_subset (c, attachList, এতত্);
    }

#ifndef HB_NO_BEYOND_64K
    যদি (use_v14 && markAttachClassDef2)
    {
      out->markAttachClassDef = 0;
      subset_markattachclassdef = serialize_subset_offset (c, out->markAttachClassDef2,
							   markAttachClassDef2, এতত্, nullptr, অসত্য, সত্য);
    }
    অন্যথা
#endif
    {
#ifndef HB_NO_BEYOND_64K
      যদি (use_v14) out->markAttachClassDef2 = 0;
#endif
      subset_markattachclassdef = out->markAttachClassDef.serialize_subset (c, markAttachClassDef,
									   এতত্, nullptr, অসত্য, সত্য);
    }

#ifndef HB_NO_BEYOND_64K
    যদি (use_v14 && ligCaretList2)
    {
      out->ligCaretList = 0;
      subset_ligcaretlist = serialize_subset_offset (c, out->ligCaretList2, ligCaretList2, এতত্);
    }
    অন্যথা
#endif
    {
#ifndef HB_NO_BEYOND_64K
      যদি (use_v14) out->ligCaretList2 = 0;
#endif
      subset_ligcaretlist = out->ligCaretList.serialize_subset (c, ligCaretList, এতত্);
    }

    যদি (subset_varstore && varstore_index != (unsigned) -1) {
      c->serializer->repack_last(varstore_index);
    }

    return_trace (subset_glyphclassdef || subset_attachlist ||
		  subset_ligcaretlist || subset_markattachclassdef ||
		  (out->version.to_int () >= 0x00010002u && subset_markglyphsetsdef) ||
		  (out->version.to_int () >= 0x00010003u && subset_varstore));
  }

  তার্কিক has_glyph_classes () ধ্রুব
  {
#ifndef HB_NO_BEYOND_64K
    যদি (version.to_int () >= 0x00010004u && glyphClassDef2) প্রত্যাগম সত্য;
#endif
    প্রত্যাগম glyphClassDef != 0;
  }
  ধ্রুব ClassDef &get_glyph_class_def () ধ্রুব
  {
#ifndef HB_NO_BEYOND_64K
    যদি (version.to_int () >= 0x00010004u && glyphClassDef2) প্রত্যাগম এতত্+glyphClassDef2;
#endif
    প্রত্যাগম এতত্+glyphClassDef;
  }
  তার্কিক has_attach_list () ধ্রুব
  {
#ifndef HB_NO_BEYOND_64K
    যদি (version.to_int () >= 0x00010004u && attachList2) প্রত্যাগম সত্য;
#endif
    প্রত্যাগম attachList != 0;
  }
  ধ্রুব AttachList &get_attach_list () ধ্রুব
  {
#ifndef HB_NO_BEYOND_64K
    যদি (version.to_int () >= 0x00010004u && attachList2) প্রত্যাগম এতত্+attachList2;
#endif
    প্রত্যাগম এতত্+attachList;
  }
  তার্কিক has_lig_carets () ধ্রুব
  {
#ifndef HB_NO_BEYOND_64K
    যদি (version.to_int () >= 0x00010004u && ligCaretList2) প্রত্যাগম সত্য;
#endif
    প্রত্যাগম ligCaretList != 0;
  }
  তার্কিক has_mark_attachment_types () ধ্রুব
  {
#ifndef HB_NO_BEYOND_64K
    যদি (version.to_int () >= 0x00010004u && markAttachClassDef2) প্রত্যাগম সত্য;
#endif
    প্রত্যাগম markAttachClassDef != 0;
  }
  ধ্রুব ClassDef &get_mark_attach_class_def () ধ্রুব
  {
#ifndef HB_NO_BEYOND_64K
    যদি (version.to_int () >= 0x00010004u && markAttachClassDef2) প্রত্যাগম এতত্+markAttachClassDef2;
#endif
    প্রত্যাগম এতত্+markAttachClassDef;
  }
  তার্কিক has_mark_glyph_sets () ধ্রুব
  {
    যদি (version.to_int () < 0x00010002u) প্রত্যাগম অসত্য;
#ifndef HB_NO_BEYOND_64K
    যদি (version.to_int () >= 0x00010004u && markGlyphSetsDef2) প্রত্যাগম সত্য;
#endif
    প্রত্যাগম markGlyphSetsDef != 0;
  }
  ধ্রুব MarkGlyphSets &get_mark_glyph_sets () ধ্রুব
  {
    যদি (version.to_int () < 0x00010002u) প্রত্যাগম Null(MarkGlyphSets);
#ifndef HB_NO_BEYOND_64K
    যদি (version.to_int () >= 0x00010004u && markGlyphSetsDef2) প্রত্যাগম এতত্+markGlyphSetsDef2;
#endif
    প্রত্যাগম এতত্+markGlyphSetsDef;
  }
  তার্কিক has_var_store () ধ্রুব
  {
    প্রত্যাগম version.to_int () >= 0x00010003u && hb_barrier () && varStore != 0;
  }
  ধ্রুব ItemVariationStore &get_var_store () ধ্রুব
  {
    প্রত্যাগম version.to_int () >= 0x00010003u && hb_barrier () ? এতত্+varStore : Null(ItemVariationStore);
  }


  তার্কিক has_data () ধ্রুব { প্রত্যাগম version.to_int (); }
  unsigned পূর্ণাঙ্ক get_glyph_class (hb_codepoint_t glyph) ধ্রুব
  { প্রত্যাগম get_glyph_class_def ().get_class (glyph); }
  শূন্য get_glyphs_in_class (unsigned পূর্ণাঙ্ক klass, hb_set_t *glyphs) ধ্রুব
  { get_glyph_class_def ().collect_class (glyphs, klass); }

  unsigned পূর্ণাঙ্ক get_mark_attachment_type (hb_codepoint_t glyph) ধ্রুব
  { প্রত্যাগম get_mark_attach_class_def ().get_class (glyph); }

  unsigned পূর্ণাঙ্ক get_attach_points (hb_codepoint_t glyph_id,
				  unsigned পূর্ণাঙ্ক start_offset,
				  unsigned পূর্ণাঙ্ক *point_গুনতি /* IN/OUT */,
				  unsigned পূর্ণাঙ্ক *point_array /* OUT */) ধ্রুব
  { প্রত্যাগম get_attach_list ().get_attach_points (glyph_id, start_offset, point_গুনতি, point_array); }

  unsigned পূর্ণাঙ্ক get_lig_carets (hb_font_t *font,
			       hb_direction_t direction,
			       hb_codepoint_t glyph_id,
			       unsigned পূর্ণাঙ্ক start_offset,
			       unsigned পূর্ণাঙ্ক *caret_গুনতি /* IN/OUT */,
			       hb_position_t *caret_array /* OUT */) ধ্রুব
  {
#ifndef HB_NO_BEYOND_64K
    যদি (version.to_int () >= 0x00010004u && ligCaretList2)
      প্রত্যাগম (এতত্+ligCaretList2).get_lig_carets (font, direction, glyph_id, get_var_store (),
						  start_offset, caret_গুনতি, caret_array);
#endif
    প্রত্যাগম (এতত্+ligCaretList).get_lig_carets (font, direction, glyph_id, get_var_store (),
					       start_offset, caret_গুনতি, caret_array);
  }

  তার্কিক mark_set_covers (unsigned পূর্ণাঙ্ক set_index, hb_codepoint_t glyph_id) ধ্রুব
  { প্রত্যাগম get_mark_glyph_sets ().covers (set_index, glyph_id); }

  /* glyph_props is a 16-bit integer where the lower 8-bit have bits representing
   * glyph class and other bits, and high 8-bit the mark attachment type (if any).
   * Not to be confused with lookup_props which is very similar. */
  unsigned পূর্ণাঙ্ক get_glyph_props (hb_codepoint_t glyph) ধ্রুব
  {
    unsigned পূর্ণাঙ্ক klass = get_glyph_class (glyph);

    static_assert (((unsigned পূর্ণাঙ্ক) HB_OT_LAYOUT_GLYPH_PROPS_BASE_GLYPH == (unsigned পূর্ণাঙ্ক) LookupFlag::IgnoreBaseGlyphs), "");
    static_assert (((unsigned পূর্ণাঙ্ক) HB_OT_LAYOUT_GLYPH_PROPS_LIGATURE == (unsigned পূর্ণাঙ্ক) LookupFlag::IgnoreLigatures), "");
    static_assert (((unsigned পূর্ণাঙ্ক) HB_OT_LAYOUT_GLYPH_PROPS_MARK == (unsigned পূর্ণাঙ্ক) LookupFlag::IgnoreMarks), "");

    বিকল্প (klass) {
    স্বভাব:			প্রত্যাগম HB_OT_LAYOUT_GLYPH_CLASS_UNCLASSIFIED;
    পক্ষ BaseGlyph:		প্রত্যাগম HB_OT_LAYOUT_GLYPH_PROPS_BASE_GLYPH;
    পক্ষ LigatureGlyph:		প্রত্যাগম HB_OT_LAYOUT_GLYPH_PROPS_LIGATURE;
    পক্ষ MarkGlyph:
	  klass = get_mark_attachment_type (glyph);
	  প্রত্যাগম HB_OT_LAYOUT_GLYPH_PROPS_MARK | (klass << 8);
    }
  }

  HB_INTERNAL তার্কিক is_blocklisted (hb_blob_t *blob,
				   hb_face_t *face) ধ্রুব;

  সংরচনা accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      table = hb_sanitize_context_t ().reference_table<GDEF> (face);
      যদি (unlikely (table->is_blocklisted (table.get_blob (), face)))
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
	যদি (!sets.in_error () &&
	    mark_glyph_set_bitmaps.resize (sets.length))
	  প্রতি (unsigned i = 0; i < sets.length; i++)
	  {
	    auto &bitmap = mark_glyph_set_bitmaps.arrayZ[i];
	    bitmap.bias = HB_SET_মান_INVALID;
	    ধ্রুব auto &set = sets.arrayZ[i];
	    যদি (unlikely (set.in_error ())) অনুবর্ত;
	    hb_codepoint_t min_gid = HB_SET_মান_INVALID;
	    hb_codepoint_t max_gid = HB_SET_মান_INVALID;
	    hb_set_next (&set, &min_gid);
	    hb_set_previous (&set, &max_gid);
	    যদি (max_gid == HB_SET_মান_INVALID)
	      অনুবর্ত;
	    যদি (max_gid - min_gid >= hb_bit_page_t::PAGE_BITS)
	    {
	      /* Too wide for one page; a digest pre-filter instead. */
	      প্রতি (hb_codepoint_t gid : set)
		bitmap.u.digest.add (gid);
	      অনুবর্ত;
	    }
	    bitmap.bias = min_gid;
	    প্রতি (hb_codepoint_t gid : set)
	      bitmap.u.page.add (gid - min_gid);
	  }
      }
#endif
    }
    ~accelerator_t () { table.destroy (); }

    unsigned পূর্ণাঙ্ক get_glyph_props (hb_codepoint_t glyph) ধ্রুব
    {
      unsigned v;

#ifndef HB_NO_GDEF_CACHE
      যদি (glyph_props_cache.get (glyph, &v))
        প্রত্যাগম v;
#endif

      v = table->get_glyph_props (glyph);

#ifndef HB_NO_GDEF_CACHE
      যদি (likely (table.get_blob ())) // Don't try setting if we are the null instance!
	glyph_props_cache.set (glyph, v);
#endif

      প্রত্যাগম v;

    }

    HB_ALWAYS_INLINE
    তার্কিক mark_set_covers (unsigned পূর্ণাঙ্ক set_index, hb_codepoint_t glyph_id) ধ্রুব
    {
#ifndef HB_NO_GDEF_CACHE
      // We can access arrayZ directly because of sanitize_lookup_props() guarantee.
      ধ্রুব auto &bitmap = mark_glyph_set_bitmaps.arrayZ[set_index];
      যদি (likely (bitmap.bias != HB_SET_মান_INVALID))
      {
	hb_codepoint_t biased = glyph_id - bitmap.bias;
	প্রত্যাগম biased < hb_bit_page_t::PAGE_BITS &&
	       bitmap.u.page.get (biased);
      }
      প্রত্যাগম mark_set_covers_slow (set_index, glyph_id);
#অন্যথা
      প্রত্যাগম table->mark_set_covers (set_index, glyph_id);
#endif
    }

#ifndef HB_NO_GDEF_CACHE
    HB_NEVER_INLINE
    তার্কিক mark_set_covers_slow (unsigned পূর্ণাঙ্ক set_index, hb_codepoint_t glyph_id) ধ্রুব
    {
      ধ্রুব auto &bitmap = mark_glyph_set_bitmaps.arrayZ[set_index];
      যদি (!bitmap.u.digest.may_have (glyph_id))
	প্রত্যাগম অসত্য;
      প্রত্যাগম table->mark_set_covers (set_index, glyph_id);
    }
#endif

    unsigned sanitize_lookup_props (unsigned lookup_props) ধ্রুব
    {
#ifndef HB_NO_GDEF_CACHE
      যদি (lookup_props & LookupFlag::UseMarkFilteringSet &&
	  (lookup_props >> 16) >= mark_glyph_set_bitmaps.length)
      {
        // Invalid mark filtering set index; unset the flag.
	lookup_props &= ~LookupFlag::UseMarkFilteringSet;
      }
#endif
      প্রত্যাগম lookup_props;
    }

    hb_blob_ptr_t<GDEF> table;
#ifndef HB_NO_GDEF_CACHE
    সংরচনা mark_glyph_set_bitmap_t
    {
      /* Gid of the first bit (not page-aligned) for an exact one-page
       * set, or HB_SET_VALUE_INVALID; then digest is a pre-filter for
       * the table.  The all-zeros state is valid for both members. */
      hb_codepoint_t bias = HB_SET_মান_INVALID;
      সঙ্ঘ u_t {
	u_t () : page () {}
	hb_bit_page_t page;
	hb_set_digest_t digest;
      } u;
    };

    hb_vector_t<mark_glyph_set_bitmap_t> mark_glyph_set_bitmaps;
    mutable hb_cache_t<21, 3> glyph_props_cache;
    static_assert (পরিমাণ (glyph_props_cache) == 512, "");
#endif
  };

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c) ধ্রুব
  {
#ifndef HB_NO_BEYOND_64K
    যদি (version.to_int () >= 0x00010004u && ligCaretList2)
    {
      (এতত্+ligCaretList2).collect_variation_indices (c);
      প্রত্যাগম;
    }
#endif
    (এতত্+ligCaretList).collect_variation_indices (c);
  }

  রক্ষিত:
  template <typename চাবি>
  স্থির শূন্য remap_varidx_after_instantiation (ধ্রুব hb_map_t& varidx_map,
                                                hb_hashmap_t<চাবি, hb_pair_t<unsigned, পূর্ণাঙ্ক>>& layout_variation_idx_delta_map /* IN/OUT */)
  {
    /* varidx_map is empty which means varstore is empty after instantiation,
     * no variations, map all varidx to HB_OT_LAYOUT_NO_VARIATIONS_INDEX.
     * varidx_map doesn't have original varidx, indicating delta row is all
     * zeros, map varidx to HB_OT_LAYOUT_NO_VARIATIONS_INDEX */
    প্রতি (auto _ : layout_variation_idx_delta_map.iter_ref ())
    {
      /* old_varidx->(varidx, delta) mapping generated for subsetting, then this
       * varidx is used as key of varidx_map during instantiation */
      uint32_t varidx = _.second.first;
      uint32_t *new_varidx;
      যদি (varidx_map.has (varidx, &new_varidx))
	_.second.first = *new_varidx;
      অন্যথা
	_.second.first = HB_OT_LAYOUT_NO_VARIATIONS_INDEX;
    }
  }

  template <typename OffsetOut, typename OffsetIn, typename Base, typename ...Ts>
  স্থির তার্কিক serialize_subset_offset (hb_subset_context_t *c,
				       OffsetOut& out,
				       ধ্রুব OffsetIn& in,
				       ধ্রুব Base *base,
				       Ts&&... ds)
  {
    out = 0;
    যদি (in.is_null ()) প্রত্যাগম অসত্য;

    auto *s = c->serializer;
    s->push ();
    তার্কিক ret = c->dispatch (base+in, std::forward<Ts> (ds)...);
    যদি (ret) s->add_link (out, s->বের_করো_pack ());
    অন্যথা s->বের_করো_discard ();
    প্রত্যাগম ret;
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
  সার্বজনিক:
  DEFINE_SIZE_MIN (12);
};

সংরচনা GDEF_accelerator_t : GDEF::accelerator_t {
  GDEF_accelerator_t (hb_face_t *face) : GDEF::accelerator_t (face) {}
};

} /* namespace OT */


#endif /* OT_LAYOUT_GDEF_GDEF_HH */
