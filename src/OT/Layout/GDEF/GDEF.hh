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
কাঠামো AttachPoint : Array16Of<HBUINT16>
{
  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    auto *out = c->serializer->start_embed (*এই);
    return_trace (out->serialize (c->serializer, + iter ()));
  }
};

/* AttachList is referenced by both GDEF attachListOffset and
 * attachListOffset2. The v1.4 top-level offset widens, but the
 * AttachList table itself keeps Offset16 internals. */
কাঠামো AttachList
{
  unsigned পূর্ণসংখ্যা get_attach_points (hb_codepoint_t glyph_id,
				  unsigned পূর্ণসংখ্যা start_offset,
				  unsigned পূর্ণসংখ্যা *point_গুনতি /* IN/OUT */,
				  unsigned পূর্ণসংখ্যা *point_array /* OUT */) ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা সূচক_2 = (এই+coverage).get_coverage (glyph_id);
    যদি (সূচক_2 == NOT_COVERED)
    {
      যদি (point_গুনতি)
	*point_গুনতি = 0;
      ফেরত 0;
    }

    ধ্রুবক AttachPoint &points = এই+attachPoint[সূচক_2];

    যদি (point_গুনতি && point_array)
    {
      + points.as_array ().sub_array (start_offset, point_গুনতি)
      | hb_sink (hb_array (point_array, *point_গুনতি))
      ;
    }

    ফেরত points.len;
  }

  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    ধ্রুবক hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ধ্রুবক hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*এই);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (মিথ্যা);

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (এই+coverage, attachPoint)
    | hb_filter (glyphset, hb_first)
    | hb_filter (subset_offset_array (c, out->attachPoint, এই), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;
    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (যৌক্তিক (new_coverage));
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (coverage.sanitize (c, এই) && attachPoint.sanitize (c, এই));
  }

  সুরক্ষিত:
  Offset16To<Coverage>
		coverage;		/* Offset to Coverage table -- from
					 * beginning of AttachList table */
  Array16OfOffset16To<AttachPoint>
		attachPoint;		/* Array of AttachPoint tables
					 * in Coverage Index order */
  সর্বজনীন:
  DEFINE_আকার_ARRAY (4, attachPoint);
};

/*
 * Ligature Caret Table
 */

কাঠামো Caret_মান_Format1
{
  friend কাঠামো Caret_মান;
  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    auto *out = c->serializer->embed (এই);
    যদি (unlikely (!out)) return_trace (মিথ্যা);
    return_trace (সত্য);
  }

  ব্যক্তিগত:
  hb_position_t get_caret_মান (hb_font_t *font, hb_direction_t direction) ধ্রুবক
  {
    ফেরত HB_DIRECTION_IS_HORIZONTAL (direction) ? font->em_scale_x (coordinate) : font->em_scale_y (coordinate);
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই));
  }

  সুরক্ষিত:
  HBUINT16	caret_মান_Format;	/* Format identifier--format = 1 */
  FWORD		coordinate;		/* X or Y value, in design units */
  সর্বজনীন:
  DEFINE_আকার_STATIC (4);
};

কাঠামো Caret_মান_Format2
{
  friend কাঠামো Caret_মান;
  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    auto *out = c->serializer->embed (এই);
    যদি (unlikely (!out)) return_trace (মিথ্যা);
    return_trace (সত্য);
  }

  ব্যক্তিগত:
  hb_position_t get_caret_মান (hb_font_t *font, hb_direction_t direction, hb_codepoint_t glyph_id) ধ্রুবক
  {
    hb_position_t x, y;
    font->get_glyph_contour_point_for_origin (glyph_id, caret_মান_Point, direction, &x, &y);
    ফেরত HB_DIRECTION_IS_HORIZONTAL (direction) ? x : y;
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই));
  }

  সুরক্ষিত:
  HBUINT16	caret_মান_Format;	/* Format identifier--format = 2 */
  HBUINT16	caret_মান_Point;	/* Contour point index on glyph */
  সর্বজনীন:
  DEFINE_আকার_STATIC (4);
};

কাঠামো Caret_মান_Format3
{
  friend কাঠামো Caret_মান;

  hb_position_t get_caret_মান (hb_font_t *font, hb_direction_t direction,
				 ধ্রুবক ItemVariationStore &var_store) ধ্রুবক
  {
    ফেরত HB_DIRECTION_IS_HORIZONTAL (direction) ?
	   hb_saturate_add (font->em_scale_x (coordinate), (এই+deviceTable).get_x_delta (font, var_store)) :
	   hb_saturate_add (font->em_scale_y (coordinate), (এই+deviceTable).get_y_delta (font, var_store));
  }

  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    auto *out = c->serializer->start_embed (*এই);
    যদি (!c->serializer->embed (caret_মান_Format)) return_trace (মিথ্যা);
    যদি (!c->serializer->embed (coordinate)) return_trace (মিথ্যা);

    unsigned varidx = (এই+deviceTable).get_variation_সূচক ();
    hb_pair_t<unsigned, পূর্ণসংখ্যা> *new_varidx_delta;
    যদি (c->plan->layout_variation_idx_delta_map.has (varidx, &new_varidx_delta)) {
      uint32_t new_varidx = hb_first (*new_varidx_delta);
      পূর্ণসংখ্যা delta = hb_second (*new_varidx_delta);
      যদি (delta != 0)
      {
        যদি (!c->serializer->check_assign (out->coordinate, coordinate + delta, HB_SERIALIZE_ERROR_INT_OVERFLOW))
          return_trace (মিথ্যা);
      }

      যদি (new_varidx == HB_OT_LAYOUT_NO_VARIATIONS_সূচক)
        return_trace (c->serializer->check_assign (out->caret_মান_Format, 1, HB_SERIALIZE_ERROR_INT_OVERFLOW));
    }

    যদি (!c->serializer->embed (deviceTable))
      return_trace (মিথ্যা);

    return_trace (out->deviceTable.serialize_copy (c->serializer, deviceTable, এই, c->serializer->to_bias (out),
						   hb_serialize_context_t::শীর্ষ, &c->plan->layout_variation_idx_delta_map));
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c) ধ্রুবক
  { (এই+deviceTable).collect_variation_indices (c); }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) && deviceTable.sanitize (c, এই));
  }

  সুরক্ষিত:
  HBUINT16	caret_মান_Format;	/* Format identifier--format = 3 */
  FWORD		coordinate;		/* X or Y value, in design units */
  Offset16To<Device>
		deviceTable;		/* Offset to Device table for X or Y
					 * value--from beginning of CaretValue
					 * table */
  সর্বজনীন:
  DEFINE_আকার_STATIC (6);
};

কাঠামো Caret_মান
{
  hb_position_t get_caret_মান (hb_font_t *font,
				 hb_direction_t direction,
				 hb_codepoint_t glyph_id,
				 ধ্রুবক ItemVariationStore &var_store) ধ্রুবক
  {
    নির্বাচন (u.format.v) {
    ক্ষেত্র 1: hb_barrier (); ফেরত u.format1.get_caret_মান (font, direction);
    ক্ষেত্র 2: hb_barrier (); ফেরত u.format2.get_caret_মান (font, direction, glyph_id);
    ক্ষেত্র 3: hb_barrier (); ফেরত u.format3.get_caret_মান (font, direction, var_store);
    পূর্বনির্ধারিত:ফেরত 0;
    }
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ধ্রুবক
  {
    যদি (unlikely (!c->may_dispatch (এই, &u.format.v))) ফেরত c->no_dispatch_return_মান ();
    TRACE_DISPATCH (এই, u.format.v);
    নির্বাচন (u.format.v) {
    ক্ষেত্র 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    ক্ষেত্র 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    ক্ষেত্র 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    পূর্বনির্ধারিত:return_trace (c->default_return_মান ());
    }
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c) ধ্রুবক
  {
    নির্বাচন (u.format.v) {
    ক্ষেত্র 1:
    ক্ষেত্র 2:
      ফেরত;
    ক্ষেত্র 3:
      hb_barrier ();
      u.format3.collect_variation_indices (c);
      ফেরত;
    পূর্বনির্ধারিত: ফেরত;
    }
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    যদি (!u.format.v.sanitize (c)) return_trace (মিথ্যা);
    hb_barrier ();
    নির্বাচন (u.format.v) {
    ক্ষেত্র 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    ক্ষেত্র 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    ক্ষেত্র 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    পূর্বনির্ধারিত:return_trace (সত্য);
    }
  }

  সুরক্ষিত:
  সংঘ {
  কাঠামো { HBUINT16 v; }	format;		/* Format identifier */
  Caret_মান_Format1	format1;
  Caret_মান_Format2	format2;
  Caret_মান_Format3	format3;
  } u;
  সর্বজনীন:
  DEFINE_আকার_UNION (2, format.v);
};

কাঠামো LigGlyph
{
  unsigned get_lig_carets (hb_font_t            *font,
			   hb_direction_t        direction,
			   hb_codepoint_t        glyph_id,
			   ধ্রুবক ItemVariationStore &var_store,
			   unsigned              start_offset,
			   unsigned             *caret_গুনতি /* IN/OUT */,
			   hb_position_t        *caret_array /* OUT */) ধ্রুবক
  {
    যদি (caret_গুনতি && caret_array)
    {
      + carets.as_array ().sub_array (start_offset, caret_গুনতি)
      | hb_map (hb_add (এই))
      | hb_map ([&] (ধ্রুবক Caret_মান &value) { ফেরত value.get_caret_মান (font, direction, glyph_id, var_store); })
      | hb_sink (hb_array (caret_array, *caret_গুনতি))
      ;
    }

    ফেরত carets.len;
  }

  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    auto *out = c->serializer->start_embed (*এই);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (মিথ্যা);

    + hb_iter (carets)
    | hb_apply (subset_offset_array (c, out->carets, এই))
    ;

    return_trace (যৌক্তিক (out->carets));
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c) ধ্রুবক
  {
    জন্য (ধ্রুবক Offset16To<Caret_মান>& offset : carets.iter ())
      (এই+offset).collect_variation_indices (c);
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (carets.sanitize (c, এই));
  }

  সুরক্ষিত:
  Array16OfOffset16To<Caret_মান>
		carets;			/* Offset array of CaretValue tables
					 * --from beginning of LigGlyph table
					 * --in increasing coordinate order */
  সর্বজনীন:
  DEFINE_আকার_ARRAY (2, carets);
};

/* LigCaretList2 widens the LigCaretList coverage, count, and LigGlyph
 * offsets. The LigGlyph table itself keeps Offset16 caretValueOffsets. */
template <typename Types>
কাঠামো LigCaretList
{
  unsigned পূর্ণসংখ্যা get_lig_carets (hb_font_t *font,
			       hb_direction_t direction,
			       hb_codepoint_t glyph_id,
			       ধ্রুবক ItemVariationStore &var_store,
			       unsigned পূর্ণসংখ্যা start_offset,
			       unsigned পূর্ণসংখ্যা *caret_গুনতি /* IN/OUT */,
			       hb_position_t *caret_array /* OUT */) ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা সূচক_2 = (এই+coverage).get_coverage (glyph_id);
    যদি (সূচক_2 == NOT_COVERED)
    {
      যদি (caret_গুনতি)
	*caret_গুনতি = 0;
      ফেরত 0;
    }
    ধ্রুবক LigGlyph &lig_glyph = এই+ligGlyph[সূচক_2];
    ফেরত lig_glyph.get_lig_carets (font, direction, glyph_id, var_store, start_offset, caret_গুনতি, caret_array);
  }

  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    ধ্রুবক hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ধ্রুবক hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*এই);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (মিথ্যা);

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (এই+coverage, ligGlyph)
    | hb_filter (glyphset, hb_first)
    | hb_filter (subset_offset_array (c, out->ligGlyph, এই), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;
    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (যৌক্তিক (new_coverage));
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c) ধ্রুবক
  {
    + hb_zip (এই+coverage, ligGlyph)
    | hb_filter (c->glyph_set, hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (এই))
    | hb_apply ([c] (ধ্রুবক LigGlyph& _) { _.collect_variation_indices (c); })
    ;
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (coverage.sanitize (c, এই) && ligGlyph.sanitize (c, এই));
  }

  সুরক্ষিত:
  typename Types::template LOffsetTo<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of LigCaretList table */
  typename Types::template ArrayOf<typename Types::template OffsetTo<LigGlyph>>
		ligGlyph;		/* Array of LigGlyph tables
					 * in Coverage Index order */
  সর্বজনীন:
  DEFINE_আকার_ARRAY (Types::LOffset::static_আকার + Types::HBUINT::static_আকার, ligGlyph);
};

#ifndef HB_NO_BEYOND_64K
static_assert (LigCaretList<MediumTypes>::min_আকার ==
	       Offset32::static_আকার + HBUINT24::static_আকার, "");
#endif


কাঠামো MarkGlyphSetsFormat1
{
  যৌক্তিক covers (unsigned পূর্ণসংখ্যা set_সূচক, hb_codepoint_t glyph_id) ধ্রুবক
  { ফেরত (এই+coverage[set_সূচক]).get_coverage (glyph_id) != NOT_COVERED; }

  শূন্য collect_used_mark_sets (ধ্রুবক hb_set_t& glyph_set,
                               hb_set_t& used_mark_sets /* OUT */) ধ্রুবক
  {
    unsigned i = 0;
    জন্য (ধ্রুবক auto &offset : coverage)
     {
       ধ্রুবক auto &cov = এই+offset;
       যদি (cov.intersects (&glyph_set))
         used_mark_sets.add (i);

       i++;
     }
  }

  template <typename set_t>
  শূন্য collect_coverage (hb_vector_t<set_t> &sets) ধ্রুবক
  {
     জন্য (ধ্রুবক auto &offset : coverage)
     {
       ধ্রুবক auto &cov = এই+offset;
       cov.collect_coverage (sets.push ());
     }
  }

  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    auto *out = c->serializer->start_embed (*এই);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (মিথ্যা);
    out->format = format;

    যৌক্তিক ret = সত্য;
    জন্য (ধ্রুবক Offset32To<Coverage>& offset : coverage.iter ())
    {
      auto snap = c->serializer->snapshot ();
      auto *o = out->coverage.serialize_append (c->serializer);
      যদি (unlikely (!o))
      {
	ret = মিথ্যা;
	বিরতি;
      }

      //skip empty coverage
      c->serializer->push ();
      যৌক্তিক res = মিথ্যা;
      যদি (offset) res = c->dispatch (এই+offset);
      যদি (!res)
      {
        c->serializer->বের_করো_discard ();
        c->serializer->revert (snap);
        (out->coverage.len)--;
        চালু;
      }
      c->serializer->add_link (*o, c->serializer->বের_করো_pack ());
    }

    return_trace (ret && out->coverage.len);
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (coverage.sanitize (c, এই));
  }

  সুরক্ষিত:
  HBUINT16	format;			/* Format identifier--format = 1 */
  Array16Of<Offset32To<Coverage>>
		coverage;		/* Array of long offsets to mark set
					 * coverage tables */
  সর্বজনীন:
  DEFINE_আকার_ARRAY (4, coverage);
};

কাঠামো MarkGlyphSets
{
  যৌক্তিক covers (unsigned পূর্ণসংখ্যা set_সূচক, hb_codepoint_t glyph_id) ধ্রুবক
  {
    নির্বাচন (u.format.v) {
    ক্ষেত্র 1: hb_barrier (); ফেরত u.format1.covers (set_সূচক, glyph_id);
    পূর্বনির্ধারিত:ফেরত মিথ্যা;
    }
  }

  template <typename set_t>
  শূন্য collect_coverage (hb_vector_t<set_t> &sets) ধ্রুবক
  {
    নির্বাচন (u.format.v) {
    ক্ষেত্র 1: hb_barrier (); u.format1.collect_coverage (sets); ফেরত;
    পূর্বনির্ধারিত:ফেরত;
    }
  }

  শূন্য collect_used_mark_sets (ধ্রুবক hb_set_t& glyph_set,
                               hb_set_t& used_mark_sets /* OUT */) ধ্রুবক
  {
    নির্বাচন (u.format.v) {
    ক্ষেত্র 1: hb_barrier (); u.format1.collect_used_mark_sets (glyph_set, used_mark_sets); ফেরত;
    পূর্বনির্ধারিত:ফেরত;
    }
  }

  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    নির্বাচন (u.format.v) {
    ক্ষেত্র 1: hb_barrier (); return_trace (u.format1.subset (c));
    পূর্বনির্ধারিত:return_trace (মিথ্যা);
    }
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    যদি (!u.format.v.sanitize (c)) return_trace (মিথ্যা);
    hb_barrier ();
    নির্বাচন (u.format.v) {
    ক্ষেত্র 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    পূর্বনির্ধারিত:return_trace (সত্য);
    }
  }

  সুরক্ষিত:
  সংঘ {
  কাঠামো { HBUINT16 v; }	format;		/* Format identifier */
  MarkGlyphSetsFormat1	format1;
  } u;
  সর্বজনীন:
  DEFINE_আকার_UNION (2, format.v);
};


/*
 * GDEF -- Glyph Definition
 * https://docs.microsoft.com/en-us/typography/opentype/spec/gdef
 */


কাঠামো GDEF
{
  স্থির constexpr hb_tag_t tableTag = HB_OT_TAG_GDEF;

  গণনা GlyphClasses {
    UnclassifiedGlyph	= 0,
    BaseGlyph		= 1,
    LigatureGlyph	= 2,
    MarkGlyph		= 3,
    ComponentGlyph	= 4
  };

  size_t get_আকার () ধ্রুবক
  {
    যদি (version.major != 1) ফেরত version.static_আকার;

    ফেরত min_আকার +
	   (version.to_int () >= 0x00010002u ? markGlyphSetsDef.static_আকার : 0) +
	   (version.to_int () >= 0x00010003u ? varStore.static_আকার : 0) +
#ifndef HB_NO_BEYOND_64K
	   (version.to_int () >= 0x00010004u ? 5 * Offset32::static_আকার : 0)
#অন্যথা
	   0
#endif
	   ;
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    যদি (unlikely (!version.sanitize (c) ||
		  version.major != 1 ||
		  !c->check_range (এই, get_আকার ())))
      return_trace (মিথ্যা);
    hb_barrier ();

    যৌক্তিক glyph_class_def_ok = glyphClassDef.sanitize (c, এই);
    যৌক্তিক attach_list_ok = attachList.sanitize (c, এই);
    যৌক্তিক lig_caret_list_ok = ligCaretList.sanitize (c, এই);
    যৌক্তিক mark_attach_class_def_ok = markAttachClassDef.sanitize (c, এই);
    যৌক্তিক mark_glyph_sets_def_ok = version.to_int () < 0x00010002u ||
				  markGlyphSetsDef.sanitize (c, এই);

#ifndef HB_NO_BEYOND_64K
    যদি (version.to_int () >= 0x00010004u)
    {
      যদি (glyphClassDef2) glyph_class_def_ok = glyphClassDef2.sanitize (c, এই);
      যদি (attachList2) attach_list_ok = attachList2.sanitize (c, এই);
      যদি (ligCaretList2) lig_caret_list_ok = ligCaretList2.sanitize (c, এই);
      যদি (markAttachClassDef2) mark_attach_class_def_ok = markAttachClassDef2.sanitize (c, এই);
      যদি (markGlyphSetsDef2) mark_glyph_sets_def_ok = markGlyphSetsDef2.sanitize (c, এই);
    }
#endif

    return_trace (glyph_class_def_ok &&
		  attach_list_ok &&
		  lig_caret_list_ok &&
		  mark_attach_class_def_ok &&
		  mark_glyph_sets_def_ok &&
		  (version.to_int () < 0x00010003u || varStore.sanitize (c, এই)));
  }

  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    যদি (version.major != 1) return_trace (মিথ্যা);

    auto *out = c->serializer->start_embed (*এই);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (মিথ্যা);

    // Push var store first (if it's needed) so that it's last in the
    // serialization order. Some font consumers assume that varstore runs to
    // the end of the GDEF table.
    // See: https://github.com/harfbuzz/harfbuzz/issues/4636
    auto snapshot_version0 = c->serializer->snapshot ();
    যদি (unlikely (version.to_int () >= 0x00010002u && hb_barrier () &&
		  !c->serializer->embed (markGlyphSetsDef)))
      return_trace (মিথ্যা);

    auto snapshot_version2 = c->serializer->snapshot ();
    যদি (unlikely (version.to_int () >= 0x00010003u && hb_barrier () &&
		  !c->serializer->embed (varStore)))
      return_trace (মিথ্যা);

    যৌক্তিক use_v14 = মিথ্যা;
#ifndef HB_NO_BEYOND_64K
    use_v14 = version.to_int () >= 0x00010004u &&
	      (glyphClassDef2 || attachList2 || ligCaretList2 ||
	       markAttachClassDef2 || markGlyphSetsDef2);
    যদি (unlikely (version.to_int () >= 0x00010004u && hb_barrier () &&
		  !c->serializer->extend_আকার (out, get_আকার ())))
      return_trace (মিথ্যা);
#endif

    যৌক্তিক subset_varstore = মিথ্যা;
    unsigned varstore_সূচক = (unsigned) -1;
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
                  item_vars.instantiate (এই+varStore, c->plan, সত্য, সত্য,
                                          c->plan->gdef_varstore_inner_maps.as_array (), সত্য))))
            return_trace (মিথ্যা);
	  subset_varstore = out->varStore.serialize_serialize (c->serializer,
							     item_vars.has_long_word (),
							     c->plan->axis_tags,
							     item_vars.get_region_list (),
							     item_vars.get_vardata_encodings ());
	  varstore_সূচক = c->serializer->last_added_child_সূচক();
	  remap_varidx_after_instantiation (item_vars.get_varidx_map (),
					    c->plan->layout_variation_idx_delta_map);
          remap_varidx_after_instantiation (item_vars.get_varidx_map (),
                                            c->plan->layout_condition_idx_মান_map);
	}
      }
      অন্যথা
      {
	subset_varstore = out->varStore.serialize_subset (c, varStore, এই,
							  c->plan->gdef_varstore_inner_maps.as_array ());
	varstore_সূচক = c->serializer->last_added_child_সূচক();
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

    যৌক্তিক subset_markglyphsetsdef = মিথ্যা;
    যদি (version.to_int () >= 0x00010002u && hb_barrier ())
    {
#ifndef HB_NO_BEYOND_64K
      যদি (use_v14 && markGlyphSetsDef2)
      {
	out->markGlyphSetsDef = 0;
	subset_markglyphsetsdef = serialize_subset_offset (c, out->markGlyphSetsDef2,
							   markGlyphSetsDef2, এই);
      }
      অন্যথা
#endif
      {
#ifndef HB_NO_BEYOND_64K
	যদি (use_v14) out->markGlyphSetsDef2 = 0;
#endif
	subset_markglyphsetsdef = out->markGlyphSetsDef.serialize_subset (c, markGlyphSetsDef, এই);
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

    যৌক্তিক subset_glyphclassdef = মিথ্যা;
    যৌক্তিক subset_attachlist = মিথ্যা;
    যৌক্তিক subset_markattachclassdef = মিথ্যা;
    যৌক্তিক subset_ligcaretlist = মিথ্যা;

#ifndef HB_NO_BEYOND_64K
    যদি (use_v14 && glyphClassDef2)
    {
      out->glyphClassDef = 0;
      subset_glyphclassdef = serialize_subset_offset (c, out->glyphClassDef2,
						      glyphClassDef2, এই, nullptr, মিথ্যা, সত্য);
    }
    অন্যথা
#endif
    {
#ifndef HB_NO_BEYOND_64K
      যদি (use_v14) out->glyphClassDef2 = 0;
#endif
      subset_glyphclassdef = out->glyphClassDef.serialize_subset (c, glyphClassDef, এই, nullptr, মিথ্যা, সত্য);
    }

#ifndef HB_NO_BEYOND_64K
    যদি (use_v14 && attachList2)
    {
      out->attachList = 0;
      subset_attachlist = serialize_subset_offset (c, out->attachList2, attachList2, এই);
    }
    অন্যথা
#endif
    {
#ifndef HB_NO_BEYOND_64K
      যদি (use_v14) out->attachList2 = 0;
#endif
      subset_attachlist = out->attachList.serialize_subset (c, attachList, এই);
    }

#ifndef HB_NO_BEYOND_64K
    যদি (use_v14 && markAttachClassDef2)
    {
      out->markAttachClassDef = 0;
      subset_markattachclassdef = serialize_subset_offset (c, out->markAttachClassDef2,
							   markAttachClassDef2, এই, nullptr, মিথ্যা, সত্য);
    }
    অন্যথা
#endif
    {
#ifndef HB_NO_BEYOND_64K
      যদি (use_v14) out->markAttachClassDef2 = 0;
#endif
      subset_markattachclassdef = out->markAttachClassDef.serialize_subset (c, markAttachClassDef,
									   এই, nullptr, মিথ্যা, সত্য);
    }

#ifndef HB_NO_BEYOND_64K
    যদি (use_v14 && ligCaretList2)
    {
      out->ligCaretList = 0;
      subset_ligcaretlist = serialize_subset_offset (c, out->ligCaretList2, ligCaretList2, এই);
    }
    অন্যথা
#endif
    {
#ifndef HB_NO_BEYOND_64K
      যদি (use_v14) out->ligCaretList2 = 0;
#endif
      subset_ligcaretlist = out->ligCaretList.serialize_subset (c, ligCaretList, এই);
    }

    যদি (subset_varstore && varstore_সূচক != (unsigned) -1) {
      c->serializer->repack_last(varstore_সূচক);
    }

    return_trace (subset_glyphclassdef || subset_attachlist ||
		  subset_ligcaretlist || subset_markattachclassdef ||
		  (out->version.to_int () >= 0x00010002u && subset_markglyphsetsdef) ||
		  (out->version.to_int () >= 0x00010003u && subset_varstore));
  }

  যৌক্তিক has_glyph_classes () ধ্রুবক
  {
#ifndef HB_NO_BEYOND_64K
    যদি (version.to_int () >= 0x00010004u && glyphClassDef2) ফেরত সত্য;
#endif
    ফেরত glyphClassDef != 0;
  }
  ধ্রুবক ClassDef &get_glyph_class_def () ধ্রুবক
  {
#ifndef HB_NO_BEYOND_64K
    যদি (version.to_int () >= 0x00010004u && glyphClassDef2) ফেরত এই+glyphClassDef2;
#endif
    ফেরত এই+glyphClassDef;
  }
  যৌক্তিক has_attach_list () ধ্রুবক
  {
#ifndef HB_NO_BEYOND_64K
    যদি (version.to_int () >= 0x00010004u && attachList2) ফেরত সত্য;
#endif
    ফেরত attachList != 0;
  }
  ধ্রুবক AttachList &get_attach_list () ধ্রুবক
  {
#ifndef HB_NO_BEYOND_64K
    যদি (version.to_int () >= 0x00010004u && attachList2) ফেরত এই+attachList2;
#endif
    ফেরত এই+attachList;
  }
  যৌক্তিক has_lig_carets () ধ্রুবক
  {
#ifndef HB_NO_BEYOND_64K
    যদি (version.to_int () >= 0x00010004u && ligCaretList2) ফেরত সত্য;
#endif
    ফেরত ligCaretList != 0;
  }
  যৌক্তিক has_mark_attachment_types () ধ্রুবক
  {
#ifndef HB_NO_BEYOND_64K
    যদি (version.to_int () >= 0x00010004u && markAttachClassDef2) ফেরত সত্য;
#endif
    ফেরত markAttachClassDef != 0;
  }
  ধ্রুবক ClassDef &get_mark_attach_class_def () ধ্রুবক
  {
#ifndef HB_NO_BEYOND_64K
    যদি (version.to_int () >= 0x00010004u && markAttachClassDef2) ফেরত এই+markAttachClassDef2;
#endif
    ফেরত এই+markAttachClassDef;
  }
  যৌক্তিক has_mark_glyph_sets () ধ্রুবক
  {
    যদি (version.to_int () < 0x00010002u) ফেরত মিথ্যা;
#ifndef HB_NO_BEYOND_64K
    যদি (version.to_int () >= 0x00010004u && markGlyphSetsDef2) ফেরত সত্য;
#endif
    ফেরত markGlyphSetsDef != 0;
  }
  ধ্রুবক MarkGlyphSets &get_mark_glyph_sets () ধ্রুবক
  {
    যদি (version.to_int () < 0x00010002u) ফেরত Null(MarkGlyphSets);
#ifndef HB_NO_BEYOND_64K
    যদি (version.to_int () >= 0x00010004u && markGlyphSetsDef2) ফেরত এই+markGlyphSetsDef2;
#endif
    ফেরত এই+markGlyphSetsDef;
  }
  যৌক্তিক has_var_store () ধ্রুবক
  {
    ফেরত version.to_int () >= 0x00010003u && hb_barrier () && varStore != 0;
  }
  ধ্রুবক ItemVariationStore &get_var_store () ধ্রুবক
  {
    ফেরত version.to_int () >= 0x00010003u && hb_barrier () ? এই+varStore : Null(ItemVariationStore);
  }


  যৌক্তিক has_তথ্য () ধ্রুবক { ফেরত version.to_int (); }
  unsigned পূর্ণসংখ্যা get_glyph_class (hb_codepoint_t glyph) ধ্রুবক
  { ফেরত get_glyph_class_def ().get_class (glyph); }
  শূন্য get_glyphs_in_class (unsigned পূর্ণসংখ্যা klass, hb_set_t *glyphs) ধ্রুবক
  { get_glyph_class_def ().collect_class (glyphs, klass); }

  unsigned পূর্ণসংখ্যা get_mark_attachment_type (hb_codepoint_t glyph) ধ্রুবক
  { ফেরত get_mark_attach_class_def ().get_class (glyph); }

  unsigned পূর্ণসংখ্যা get_attach_points (hb_codepoint_t glyph_id,
				  unsigned পূর্ণসংখ্যা start_offset,
				  unsigned পূর্ণসংখ্যা *point_গুনতি /* IN/OUT */,
				  unsigned পূর্ণসংখ্যা *point_array /* OUT */) ধ্রুবক
  { ফেরত get_attach_list ().get_attach_points (glyph_id, start_offset, point_গুনতি, point_array); }

  unsigned পূর্ণসংখ্যা get_lig_carets (hb_font_t *font,
			       hb_direction_t direction,
			       hb_codepoint_t glyph_id,
			       unsigned পূর্ণসংখ্যা start_offset,
			       unsigned পূর্ণসংখ্যা *caret_গুনতি /* IN/OUT */,
			       hb_position_t *caret_array /* OUT */) ধ্রুবক
  {
#ifndef HB_NO_BEYOND_64K
    যদি (version.to_int () >= 0x00010004u && ligCaretList2)
      ফেরত (এই+ligCaretList2).get_lig_carets (font, direction, glyph_id, get_var_store (),
						  start_offset, caret_গুনতি, caret_array);
#endif
    ফেরত (এই+ligCaretList).get_lig_carets (font, direction, glyph_id, get_var_store (),
					       start_offset, caret_গুনতি, caret_array);
  }

  যৌক্তিক mark_set_covers (unsigned পূর্ণসংখ্যা set_সূচক, hb_codepoint_t glyph_id) ধ্রুবক
  { ফেরত get_mark_glyph_sets ().covers (set_সূচক, glyph_id); }

  /* glyph_props is a 16-bit integer where the lower 8-bit have bits representing
   * glyph class and other bits, and high 8-bit the mark attachment type (if any).
   * Not to be confused with lookup_props which is very similar. */
  unsigned পূর্ণসংখ্যা get_glyph_props (hb_codepoint_t glyph) ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা klass = get_glyph_class (glyph);

    static_assert (((unsigned পূর্ণসংখ্যা) HB_OT_LAYOUT_GLYPH_PROPS_BASE_GLYPH == (unsigned পূর্ণসংখ্যা) LookupFlag::IgnoreBaseGlyphs), "");
    static_assert (((unsigned পূর্ণসংখ্যা) HB_OT_LAYOUT_GLYPH_PROPS_LIGATURE == (unsigned পূর্ণসংখ্যা) LookupFlag::IgnoreLigatures), "");
    static_assert (((unsigned পূর্ণসংখ্যা) HB_OT_LAYOUT_GLYPH_PROPS_MARK == (unsigned পূর্ণসংখ্যা) LookupFlag::IgnoreMarks), "");

    নির্বাচন (klass) {
    পূর্বনির্ধারিত:			ফেরত HB_OT_LAYOUT_GLYPH_CLASS_UNCLASSIFIED;
    ক্ষেত্র BaseGlyph:		ফেরত HB_OT_LAYOUT_GLYPH_PROPS_BASE_GLYPH;
    ক্ষেত্র LigatureGlyph:		ফেরত HB_OT_LAYOUT_GLYPH_PROPS_LIGATURE;
    ক্ষেত্র MarkGlyph:
	  klass = get_mark_attachment_type (glyph);
	  ফেরত HB_OT_LAYOUT_GLYPH_PROPS_MARK | (klass << 8);
    }
  }

  HB_INTERNAL যৌক্তিক is_blocklisted (hb_blob_t *blob,
				   hb_face_t *face) ধ্রুবক;

  কাঠামো accelerator_t
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
	    mark_glyph_set_bitmaps.resize (sets.দৈর্ঘ্য))
	  জন্য (unsigned i = 0; i < sets.দৈর্ঘ্য; i++)
	  {
	    auto &bitmap = mark_glyph_set_bitmaps.arrayZ[i];
	    bitmap.bias = HB_SET_মান_INVALID;
	    ধ্রুবক auto &set = sets.arrayZ[i];
	    যদি (unlikely (set.in_error ())) চালু;
	    hb_codepoint_t min_gid = HB_SET_মান_INVALID;
	    hb_codepoint_t max_gid = HB_SET_মান_INVALID;
	    hb_set_পরবর্তী (&set, &min_gid);
	    hb_set_previous (&set, &max_gid);
	    যদি (max_gid == HB_SET_মান_INVALID)
	      চালু;
	    যদি (max_gid - min_gid >= hb_bit_page_t::PAGE_BITS)
	    {
	      /* Too wide for one page; a digest pre-filter instead. */
	      জন্য (hb_codepoint_t gid : set)
		bitmap.u.digest.add (gid);
	      চালু;
	    }
	    bitmap.bias = min_gid;
	    জন্য (hb_codepoint_t gid : set)
	      bitmap.u.page.add (gid - min_gid);
	  }
      }
#endif
    }
    ~accelerator_t () { table.destroy (); }

    unsigned পূর্ণসংখ্যা get_glyph_props (hb_codepoint_t glyph) ধ্রুবক
    {
      unsigned v;

#ifndef HB_NO_GDEF_CACHE
      যদি (glyph_props_cache.get (glyph, &v))
        ফেরত v;
#endif

      v = table->get_glyph_props (glyph);

#ifndef HB_NO_GDEF_CACHE
      যদি (likely (table.get_blob ())) // Don't try setting if we are the null instance!
	glyph_props_cache.set (glyph, v);
#endif

      ফেরত v;

    }

    HB_ALWAYS_INLINE
    যৌক্তিক mark_set_covers (unsigned পূর্ণসংখ্যা set_সূচক, hb_codepoint_t glyph_id) ধ্রুবক
    {
#ifndef HB_NO_GDEF_CACHE
      // We can access arrayZ directly because of sanitize_lookup_props() guarantee.
      ধ্রুবক auto &bitmap = mark_glyph_set_bitmaps.arrayZ[set_সূচক];
      যদি (likely (bitmap.bias != HB_SET_মান_INVALID))
      {
	hb_codepoint_t biased = glyph_id - bitmap.bias;
	ফেরত biased < hb_bit_page_t::PAGE_BITS &&
	       bitmap.u.page.get (biased);
      }
      ফেরত mark_set_covers_slow (set_সূচক, glyph_id);
#অন্যথা
      ফেরত table->mark_set_covers (set_সূচক, glyph_id);
#endif
    }

#ifndef HB_NO_GDEF_CACHE
    HB_NEVER_INLINE
    যৌক্তিক mark_set_covers_slow (unsigned পূর্ণসংখ্যা set_সূচক, hb_codepoint_t glyph_id) ধ্রুবক
    {
      ধ্রুবক auto &bitmap = mark_glyph_set_bitmaps.arrayZ[set_সূচক];
      যদি (!bitmap.u.digest.may_have (glyph_id))
	ফেরত মিথ্যা;
      ফেরত table->mark_set_covers (set_সূচক, glyph_id);
    }
#endif

    unsigned sanitize_lookup_props (unsigned lookup_props) ধ্রুবক
    {
#ifndef HB_NO_GDEF_CACHE
      যদি (lookup_props & LookupFlag::UseMarkFilteringSet &&
	  (lookup_props >> 16) >= mark_glyph_set_bitmaps.দৈর্ঘ্য)
      {
        // Invalid mark filtering set index; unset the flag.
	lookup_props &= ~LookupFlag::UseMarkFilteringSet;
      }
#endif
      ফেরত lookup_props;
    }

    hb_blob_ptr_t<GDEF> table;
#ifndef HB_NO_GDEF_CACHE
    কাঠামো mark_glyph_set_bitmap_t
    {
      /* Gid of the first bit (not page-aligned) for an exact one-page
       * set, or HB_SET_VALUE_INVALID; then digest is a pre-filter for
       * the table.  The all-zeros state is valid for both members. */
      hb_codepoint_t bias = HB_SET_মান_INVALID;
      সংঘ u_t {
	u_t () : page () {}
	hb_bit_page_t page;
	hb_set_digest_t digest;
      } u;
    };

    hb_vector_t<mark_glyph_set_bitmap_t> mark_glyph_set_bitmaps;
    mutable hb_cache_t<21, 3> glyph_props_cache;
    static_assert (আকার (glyph_props_cache) == 512, "");
#endif
  };

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c) ধ্রুবক
  {
#ifndef HB_NO_BEYOND_64K
    যদি (version.to_int () >= 0x00010004u && ligCaretList2)
    {
      (এই+ligCaretList2).collect_variation_indices (c);
      ফেরত;
    }
#endif
    (এই+ligCaretList).collect_variation_indices (c);
  }

  সুরক্ষিত:
  template <typename চাবি>
  স্থির শূন্য remap_varidx_after_instantiation (ধ্রুবক hb_map_t& varidx_map,
                                                hb_hashmap_t<চাবি, hb_pair_t<unsigned, পূর্ণসংখ্যা>>& layout_variation_idx_delta_map /* IN/OUT */)
  {
    /* varidx_map is empty which means varstore is empty after instantiation,
     * no variations, map all varidx to HB_OT_LAYOUT_NO_VARIATIONS_INDEX.
     * varidx_map doesn't have original varidx, indicating delta row is all
     * zeros, map varidx to HB_OT_LAYOUT_NO_VARIATIONS_INDEX */
    জন্য (auto _ : layout_variation_idx_delta_map.iter_ref ())
    {
      /* old_varidx->(varidx, delta) mapping generated for subsetting, then this
       * varidx is used as key of varidx_map during instantiation */
      uint32_t varidx = _.second.first;
      uint32_t *new_varidx;
      যদি (varidx_map.has (varidx, &new_varidx))
	_.second.first = *new_varidx;
      অন্যথা
	_.second.first = HB_OT_LAYOUT_NO_VARIATIONS_সূচক;
    }
  }

  template <typename OffsetOut, typename OffsetIn, typename Base, typename ...Ts>
  স্থির যৌক্তিক serialize_subset_offset (hb_subset_context_t *c,
				       OffsetOut& out,
				       ধ্রুবক OffsetIn& in,
				       ধ্রুবক Base *base,
				       Ts&&... ds)
  {
    out = 0;
    যদি (in.is_null ()) ফেরত মিথ্যা;

    auto *s = c->serializer;
    s->push ();
    যৌক্তিক ret = c->dispatch (base+in, std::forward<Ts> (ds)...);
    যদি (ret) s->add_link (out, s->বের_করো_pack ());
    অন্যথা s->বের_করো_discard ();
    ফেরত ret;
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
  সর্বজনীন:
  DEFINE_আকার_MIN (12);
};

কাঠামো GDEF_accelerator_t : GDEF::accelerator_t {
  GDEF_accelerator_t (hb_face_t *face) : GDEF::accelerator_t (face) {}
};

} /* namespace OT */


#endif /* OT_LAYOUT_GDEF_GDEF_HH */
