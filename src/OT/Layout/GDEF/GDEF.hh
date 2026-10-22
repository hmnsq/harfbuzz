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

#شامل "../../../hb-ot-var-common.hh"

#شامل "../../../hb-font.hh"
#شامل "../../../hb-cache.hh"


فضاینام OT {


/*
 * Attachment List Table
 */

/* Array of contour point indices--in increasing numerical order */
ساختار AttachPoint : Array16Of<HBUINT16>
{
  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);
    auto *out = c->serializer->start_embed (*این);
    return_trace (out->serialize (c->serializer, + iter ()));
  }
};

/* AttachList is referenced by both GDEF attachListOffset and
 * attachListOffset2. The v1.4 top-level offset widens, but the
 * AttachList table itself keeps Offset16 internals. */
ساختار AttachList
{
  unsigned صحیح get_attach_points (hb_codepoint_t glyph_id,
				  unsigned صحیح start_offset,
				  unsigned صحیح *point_شمار /* IN/OUT */,
				  unsigned صحیح *point_array /* OUT */) ثابت
  {
    unsigned صحیح index = (این+coverage).get_coverage (glyph_id);
    اگر (index == NOT_COVERED)
    {
      اگر (point_شمار)
	*point_شمار = 0;
      بازگشت 0;
    }

    ثابت AttachPoint &points = این+attachPoint[index];

    اگر (point_شمار && point_array)
    {
      + points.as_array ().sub_array (start_offset, point_شمار)
      | hb_sink (hb_array (point_array, *point_شمار))
      ;
    }

    بازگشت points.len;
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);
    ثابت hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*این);
    اگر (unlikely (!c->serializer->extend_min (out))) return_trace (نادرست);

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (این+coverage, attachPoint)
    | hb_filter (glyphset, hb_first)
    | hb_filter (subset_offset_array (c, out->attachPoint, این), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;
    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (منطقی (new_coverage));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (coverage.sanitize (c, این) && attachPoint.sanitize (c, این));
  }

  محافظتشده:
  Offset16To<Coverage>
		coverage;		/* Offset to Coverage table -- from
					 * beginning of AttachList table */
  Array16OfOffset16To<AttachPoint>
		attachPoint;		/* Array of AttachPoint tables
					 * in Coverage Index order */
  عمومی:
  DEFINE_SIZE_ARRAY (4, attachPoint);
};

/*
 * Ligature Caret Table
 */

ساختار Caret_مقدار_Format1
{
  friend ساختار Caret_مقدار;
  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);
    auto *out = c->serializer->embed (این);
    اگر (unlikely (!out)) return_trace (نادرست);
    return_trace (درست);
  }

  خصوصی:
  hb_position_t get_caret_مقدار (hb_font_t *font, hb_direction_t direction) ثابت
  {
    بازگشت HB_DIRECTION_IS_HORIZONTAL (direction) ? font->em_scale_x (coordinate) : font->em_scale_y (coordinate);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این));
  }

  محافظتشده:
  HBUINT16	caret_مقدار_Format;	/* Format identifier--format = 1 */
  FWORD		coordinate;		/* X or Y value, in design units */
  عمومی:
  DEFINE_SIZE_STATIC (4);
};

ساختار Caret_مقدار_Format2
{
  friend ساختار Caret_مقدار;
  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);
    auto *out = c->serializer->embed (این);
    اگر (unlikely (!out)) return_trace (نادرست);
    return_trace (درست);
  }

  خصوصی:
  hb_position_t get_caret_مقدار (hb_font_t *font, hb_direction_t direction, hb_codepoint_t glyph_id) ثابت
  {
    hb_position_t x, y;
    font->get_glyph_contour_point_for_origin (glyph_id, caret_مقدار_Point, direction, &x, &y);
    بازگشت HB_DIRECTION_IS_HORIZONTAL (direction) ? x : y;
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این));
  }

  محافظتشده:
  HBUINT16	caret_مقدار_Format;	/* Format identifier--format = 2 */
  HBUINT16	caret_مقدار_Point;	/* Contour point index on glyph */
  عمومی:
  DEFINE_SIZE_STATIC (4);
};

ساختار Caret_مقدار_Format3
{
  friend ساختار Caret_مقدار;

  hb_position_t get_caret_مقدار (hb_font_t *font, hb_direction_t direction,
				 ثابت ItemVariationStore &var_store) ثابت
  {
    بازگشت HB_DIRECTION_IS_HORIZONTAL (direction) ?
	   hb_saturate_add (font->em_scale_x (coordinate), (این+deviceTable).get_x_delta (font, var_store)) :
	   hb_saturate_add (font->em_scale_y (coordinate), (این+deviceTable).get_y_delta (font, var_store));
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);
    auto *out = c->serializer->start_embed (*این);
    اگر (!c->serializer->embed (caret_مقدار_Format)) return_trace (نادرست);
    اگر (!c->serializer->embed (coordinate)) return_trace (نادرست);

    unsigned varidx = (این+deviceTable).get_variation_index ();
    hb_pair_t<unsigned, صحیح> *new_varidx_delta;
    اگر (c->plan->layout_variation_idx_delta_map.has (varidx, &new_varidx_delta)) {
      uint32_t new_varidx = hb_first (*new_varidx_delta);
      صحیح delta = hb_second (*new_varidx_delta);
      اگر (delta != 0)
      {
        اگر (!c->serializer->check_assign (out->coordinate, coordinate + delta, HB_SERIALIZE_ERROR_INT_OVERFLOW))
          return_trace (نادرست);
      }

      اگر (new_varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
        return_trace (c->serializer->check_assign (out->caret_مقدار_Format, 1, HB_SERIALIZE_ERROR_INT_OVERFLOW));
    }

    اگر (!c->serializer->embed (deviceTable))
      return_trace (نادرست);

    return_trace (out->deviceTable.serialize_copy (c->serializer, deviceTable, این, c->serializer->to_bias (out),
						   hb_serialize_context_t::Head, &c->plan->layout_variation_idx_delta_map));
  }

  تهی collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  { (این+deviceTable).collect_variation_indices (c); }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) && deviceTable.sanitize (c, این));
  }

  محافظتشده:
  HBUINT16	caret_مقدار_Format;	/* Format identifier--format = 3 */
  FWORD		coordinate;		/* X or Y value, in design units */
  Offset16To<Device>
		deviceTable;		/* Offset to Device table for X or Y
					 * value--from beginning of CaretValue
					 * table */
  عمومی:
  DEFINE_SIZE_STATIC (6);
};

ساختار Caret_مقدار
{
  hb_position_t get_caret_مقدار (hb_font_t *font,
				 hb_direction_t direction,
				 hb_codepoint_t glyph_id,
				 ثابت ItemVariationStore &var_store) ثابت
  {
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); بازگشت u.format1.get_caret_مقدار (font, direction);
    حالت 2: hb_barrier (); بازگشت u.format2.get_caret_مقدار (font, direction, glyph_id);
    حالت 3: hb_barrier (); بازگشت u.format3.get_caret_مقدار (font, direction, var_store);
    پیشفرض:بازگشت 0;
    }
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    اگر (unlikely (!c->may_dispatch (این, &u.format.v))) بازگشت c->no_dispatch_return_مقدار ();
    TRACE_DISPATCH (این, u.format.v);
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    حالت 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    حالت 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    پیشفرض:return_trace (c->default_return_مقدار ());
    }
  }

  تهی collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    انتخاب (u.format.v) {
    حالت 1:
    حالت 2:
      بازگشت;
    حالت 3:
      hb_barrier ();
      u.format3.collect_variation_indices (c);
      بازگشت;
    پیشفرض: بازگشت;
    }
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    اگر (!u.format.v.sanitize (c)) return_trace (نادرست);
    hb_barrier ();
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    حالت 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    حالت 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    پیشفرض:return_trace (درست);
    }
  }

  محافظتشده:
  اجتماع {
  ساختار { HBUINT16 v; }	format;		/* Format identifier */
  Caret_مقدار_Format1	format1;
  Caret_مقدار_Format2	format2;
  Caret_مقدار_Format3	format3;
  } u;
  عمومی:
  DEFINE_SIZE_UNION (2, format.v);
};

ساختار LigGlyph
{
  unsigned get_lig_carets (hb_font_t            *font,
			   hb_direction_t        direction,
			   hb_codepoint_t        glyph_id,
			   ثابت ItemVariationStore &var_store,
			   unsigned              start_offset,
			   unsigned             *caret_شمار /* IN/OUT */,
			   hb_position_t        *caret_array /* OUT */) ثابت
  {
    اگر (caret_شمار && caret_array)
    {
      + carets.as_array ().sub_array (start_offset, caret_شمار)
      | hb_map (hb_add (این))
      | hb_map ([&] (ثابت Caret_مقدار &value) { بازگشت value.get_caret_مقدار (font, direction, glyph_id, var_store); })
      | hb_sink (hb_array (caret_array, *caret_شمار))
      ;
    }

    بازگشت carets.len;
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);
    auto *out = c->serializer->start_embed (*این);
    اگر (unlikely (!c->serializer->extend_min (out))) return_trace (نادرست);

    + hb_iter (carets)
    | hb_apply (subset_offset_array (c, out->carets, این))
    ;

    return_trace (منطقی (out->carets));
  }

  تهی collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    برای (ثابت Offset16To<Caret_مقدار>& offset : carets.iter ())
      (این+offset).collect_variation_indices (c);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (carets.sanitize (c, این));
  }

  محافظتشده:
  Array16OfOffset16To<Caret_مقدار>
		carets;			/* Offset array of CaretValue tables
					 * --from beginning of LigGlyph table
					 * --in increasing coordinate order */
  عمومی:
  DEFINE_SIZE_ARRAY (2, carets);
};

/* LigCaretList2 widens the LigCaretList coverage, count, and LigGlyph
 * offsets. The LigGlyph table itself keeps Offset16 caretValueOffsets. */
template <typename Types>
ساختار LigCaretList
{
  unsigned صحیح get_lig_carets (hb_font_t *font,
			       hb_direction_t direction,
			       hb_codepoint_t glyph_id,
			       ثابت ItemVariationStore &var_store,
			       unsigned صحیح start_offset,
			       unsigned صحیح *caret_شمار /* IN/OUT */,
			       hb_position_t *caret_array /* OUT */) ثابت
  {
    unsigned صحیح index = (این+coverage).get_coverage (glyph_id);
    اگر (index == NOT_COVERED)
    {
      اگر (caret_شمار)
	*caret_شمار = 0;
      بازگشت 0;
    }
    ثابت LigGlyph &lig_glyph = این+ligGlyph[index];
    بازگشت lig_glyph.get_lig_carets (font, direction, glyph_id, var_store, start_offset, caret_شمار, caret_array);
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);
    ثابت hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*این);
    اگر (unlikely (!c->serializer->extend_min (out))) return_trace (نادرست);

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (این+coverage, ligGlyph)
    | hb_filter (glyphset, hb_first)
    | hb_filter (subset_offset_array (c, out->ligGlyph, این), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;
    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (منطقی (new_coverage));
  }

  تهی collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    + hb_zip (این+coverage, ligGlyph)
    | hb_filter (c->glyph_set, hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (این))
    | hb_apply ([c] (ثابت LigGlyph& _) { _.collect_variation_indices (c); })
    ;
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (coverage.sanitize (c, این) && ligGlyph.sanitize (c, این));
  }

  محافظتشده:
  typename Types::template LOffsetTo<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of LigCaretList table */
  typename Types::template ArrayOf<typename Types::template OffsetTo<LigGlyph>>
		ligGlyph;		/* Array of LigGlyph tables
					 * in Coverage Index order */
  عمومی:
  DEFINE_SIZE_ARRAY (Types::LOffset::static_size + Types::HBUINT::static_size, ligGlyph);
};

#ifndef HB_NO_BEYOND_64K
static_assert (LigCaretList<MediumTypes>::min_size ==
	       Offset32::static_size + HBUINT24::static_size, "");
#endif


ساختار MarkGlyphSetsFormat1
{
  منطقی covers (unsigned صحیح set_index, hb_codepoint_t glyph_id) ثابت
  { بازگشت (این+coverage[set_index]).get_coverage (glyph_id) != NOT_COVERED; }

  تهی collect_used_mark_sets (ثابت hb_set_t& glyph_set,
                               hb_set_t& used_mark_sets /* OUT */) ثابت
  {
    unsigned i = 0;
    برای (ثابت auto &offset : coverage)
     {
       ثابت auto &cov = این+offset;
       اگر (cov.intersects (&glyph_set))
         used_mark_sets.add (i);

       i++;
     }
  }

  template <typename set_t>
  تهی collect_coverage (hb_vector_t<set_t> &sets) ثابت
  {
     برای (ثابت auto &offset : coverage)
     {
       ثابت auto &cov = این+offset;
       cov.collect_coverage (sets.push ());
     }
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);
    auto *out = c->serializer->start_embed (*این);
    اگر (unlikely (!c->serializer->extend_min (out))) return_trace (نادرست);
    out->format = format;

    منطقی ret = درست;
    برای (ثابت Offset32To<Coverage>& offset : coverage.iter ())
    {
      auto snap = c->serializer->snapshot ();
      auto *o = out->coverage.serialize_append (c->serializer);
      اگر (unlikely (!o))
      {
	ret = نادرست;
	بشکن;
      }

      //skip empty coverage
      c->serializer->push ();
      منطقی res = نادرست;
      اگر (offset) res = c->dispatch (این+offset);
      اگر (!res)
      {
        c->serializer->برداشت_discard ();
        c->serializer->revert (snap);
        (out->coverage.len)--;
        ادامه;
      }
      c->serializer->add_link (*o, c->serializer->برداشت_pack ());
    }

    return_trace (ret && out->coverage.len);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (coverage.sanitize (c, این));
  }

  محافظتشده:
  HBUINT16	format;			/* Format identifier--format = 1 */
  Array16Of<Offset32To<Coverage>>
		coverage;		/* Array of long offsets to mark set
					 * coverage tables */
  عمومی:
  DEFINE_SIZE_ARRAY (4, coverage);
};

ساختار MarkGlyphSets
{
  منطقی covers (unsigned صحیح set_index, hb_codepoint_t glyph_id) ثابت
  {
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); بازگشت u.format1.covers (set_index, glyph_id);
    پیشفرض:بازگشت نادرست;
    }
  }

  template <typename set_t>
  تهی collect_coverage (hb_vector_t<set_t> &sets) ثابت
  {
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); u.format1.collect_coverage (sets); بازگشت;
    پیشفرض:بازگشت;
    }
  }

  تهی collect_used_mark_sets (ثابت hb_set_t& glyph_set,
                               hb_set_t& used_mark_sets /* OUT */) ثابت
  {
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); u.format1.collect_used_mark_sets (glyph_set, used_mark_sets); بازگشت;
    پیشفرض:بازگشت;
    }
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); return_trace (u.format1.subset (c));
    پیشفرض:return_trace (نادرست);
    }
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    اگر (!u.format.v.sanitize (c)) return_trace (نادرست);
    hb_barrier ();
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    پیشفرض:return_trace (درست);
    }
  }

  محافظتشده:
  اجتماع {
  ساختار { HBUINT16 v; }	format;		/* Format identifier */
  MarkGlyphSetsFormat1	format1;
  } u;
  عمومی:
  DEFINE_SIZE_UNION (2, format.v);
};


/*
 * GDEF -- Glyph Definition
 * https://docs.microsoft.com/en-us/typography/opentype/spec/gdef
 */


ساختار GDEF
{
  ایستا constexpr hb_tag_t tableTag = HB_OT_TAG_GDEF;

  شمارش GlyphClasses {
    UnclassifiedGlyph	= 0,
    BaseGlyph		= 1,
    LigatureGlyph	= 2,
    MarkGlyph		= 3,
    ComponentGlyph	= 4
  };

  size_t get_size () ثابت
  {
    اگر (version.major != 1) بازگشت version.static_size;

    بازگشت min_size +
	   (version.to_int () >= 0x00010002u ? markGlyphSetsDef.static_size : 0) +
	   (version.to_int () >= 0x00010003u ? varStore.static_size : 0) +
#ifndef HB_NO_BEYOND_64K
	   (version.to_int () >= 0x00010004u ? 5 * Offset32::static_size : 0)
#وگرنه
	   0
#endif
	   ;
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    اگر (unlikely (!version.sanitize (c) ||
		  version.major != 1 ||
		  !c->check_range (این, get_size ())))
      return_trace (نادرست);
    hb_barrier ();

    منطقی glyph_class_def_ok = glyphClassDef.sanitize (c, این);
    منطقی attach_list_ok = attachList.sanitize (c, این);
    منطقی lig_caret_list_ok = ligCaretList.sanitize (c, این);
    منطقی mark_attach_class_def_ok = markAttachClassDef.sanitize (c, این);
    منطقی mark_glyph_sets_def_ok = version.to_int () < 0x00010002u ||
				  markGlyphSetsDef.sanitize (c, این);

#ifndef HB_NO_BEYOND_64K
    اگر (version.to_int () >= 0x00010004u)
    {
      اگر (glyphClassDef2) glyph_class_def_ok = glyphClassDef2.sanitize (c, این);
      اگر (attachList2) attach_list_ok = attachList2.sanitize (c, این);
      اگر (ligCaretList2) lig_caret_list_ok = ligCaretList2.sanitize (c, این);
      اگر (markAttachClassDef2) mark_attach_class_def_ok = markAttachClassDef2.sanitize (c, این);
      اگر (markGlyphSetsDef2) mark_glyph_sets_def_ok = markGlyphSetsDef2.sanitize (c, این);
    }
#endif

    return_trace (glyph_class_def_ok &&
		  attach_list_ok &&
		  lig_caret_list_ok &&
		  mark_attach_class_def_ok &&
		  mark_glyph_sets_def_ok &&
		  (version.to_int () < 0x00010003u || varStore.sanitize (c, این)));
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);
    اگر (version.major != 1) return_trace (نادرست);

    auto *out = c->serializer->start_embed (*این);
    اگر (unlikely (!c->serializer->extend_min (out))) return_trace (نادرست);

    // Push var store first (if it's needed) so that it's last in the
    // serialization order. Some font consumers assume that varstore runs to
    // the end of the GDEF table.
    // See: https://github.com/harfbuzz/harfbuzz/issues/4636
    auto snapshot_version0 = c->serializer->snapshot ();
    اگر (unlikely (version.to_int () >= 0x00010002u && hb_barrier () &&
		  !c->serializer->embed (markGlyphSetsDef)))
      return_trace (نادرست);

    auto snapshot_version2 = c->serializer->snapshot ();
    اگر (unlikely (version.to_int () >= 0x00010003u && hb_barrier () &&
		  !c->serializer->embed (varStore)))
      return_trace (نادرست);

    منطقی use_v14 = نادرست;
#ifndef HB_NO_BEYOND_64K
    use_v14 = version.to_int () >= 0x00010004u &&
	      (glyphClassDef2 || attachList2 || ligCaretList2 ||
	       markAttachClassDef2 || markGlyphSetsDef2);
    اگر (unlikely (version.to_int () >= 0x00010004u && hb_barrier () &&
		  !c->serializer->extend_size (out, get_size ())))
      return_trace (نادرست);
#endif

    منطقی subset_varstore = نادرست;
    unsigned varstore_index = (unsigned) -1;
    اگر (version.to_int () >= 0x00010003u && hb_barrier ())
    {
      اگر (c->plan->all_axes_pinned)
	out->varStore = 0;
      وگرنه اگر (c->plan->normalized_coords)
      {
	اگر (varStore)
	{
	  item_variations_t item_vars;
	  اگر (unlikely (!c->serializer->check_success (
                  item_vars.instantiate (این+varStore, c->plan, درست, درست,
                                          c->plan->gdef_varstore_inner_maps.as_array (), درست))))
            return_trace (نادرست);
	  subset_varstore = out->varStore.serialize_serialize (c->serializer,
							     item_vars.has_long_word (),
							     c->plan->axis_tags,
							     item_vars.get_region_list (),
							     item_vars.get_vardata_encodings ());
	  varstore_index = c->serializer->last_added_child_index();
	  remap_varidx_after_instantiation (item_vars.get_varidx_map (),
					    c->plan->layout_variation_idx_delta_map);
          remap_varidx_after_instantiation (item_vars.get_varidx_map (),
                                            c->plan->layout_condition_idx_مقدار_map);
	}
      }
      وگرنه
      {
	subset_varstore = out->varStore.serialize_subset (c, varStore, این,
							  c->plan->gdef_varstore_inner_maps.as_array ());
	varstore_index = c->serializer->last_added_child_index();
      }
    }

    out->version.major = version.major;
    out->version.minor = version.minor;

    اگر (!subset_varstore && version.to_int () >= 0x00010002u && !use_v14) {
      c->serializer->revert (snapshot_version2);
    }
    اگر (!subset_varstore)
      out->varStore = 0;
    وگرنه
      c->plan->has_gdef_varstore = درست;

    منطقی subset_markglyphsetsdef = نادرست;
    اگر (version.to_int () >= 0x00010002u && hb_barrier ())
    {
#ifndef HB_NO_BEYOND_64K
      اگر (use_v14 && markGlyphSetsDef2)
      {
	out->markGlyphSetsDef = 0;
	subset_markglyphsetsdef = serialize_subset_offset (c, out->markGlyphSetsDef2,
							   markGlyphSetsDef2, این);
      }
      وگرنه
#endif
      {
#ifndef HB_NO_BEYOND_64K
	اگر (use_v14) out->markGlyphSetsDef2 = 0;
#endif
	subset_markglyphsetsdef = out->markGlyphSetsDef.serialize_subset (c, markGlyphSetsDef, این);
      }
    }

    اگر (use_v14)
    {
      out->version.minor = 4;
    } وگرنه اگر (subset_varstore) {
      out->version.minor = 3;
    } وگرنه اگر (subset_markglyphsetsdef) {
      out->version.minor = 2;
    } وگرنه  {
      out->version.minor = 0;
      c->serializer->revert (snapshot_version0);
    }

    منطقی subset_glyphclassdef = نادرست;
    منطقی subset_attachlist = نادرست;
    منطقی subset_markattachclassdef = نادرست;
    منطقی subset_ligcaretlist = نادرست;

#ifndef HB_NO_BEYOND_64K
    اگر (use_v14 && glyphClassDef2)
    {
      out->glyphClassDef = 0;
      subset_glyphclassdef = serialize_subset_offset (c, out->glyphClassDef2,
						      glyphClassDef2, این, nullptr, نادرست, درست);
    }
    وگرنه
#endif
    {
#ifndef HB_NO_BEYOND_64K
      اگر (use_v14) out->glyphClassDef2 = 0;
#endif
      subset_glyphclassdef = out->glyphClassDef.serialize_subset (c, glyphClassDef, این, nullptr, نادرست, درست);
    }

#ifndef HB_NO_BEYOND_64K
    اگر (use_v14 && attachList2)
    {
      out->attachList = 0;
      subset_attachlist = serialize_subset_offset (c, out->attachList2, attachList2, این);
    }
    وگرنه
#endif
    {
#ifndef HB_NO_BEYOND_64K
      اگر (use_v14) out->attachList2 = 0;
#endif
      subset_attachlist = out->attachList.serialize_subset (c, attachList, این);
    }

#ifndef HB_NO_BEYOND_64K
    اگر (use_v14 && markAttachClassDef2)
    {
      out->markAttachClassDef = 0;
      subset_markattachclassdef = serialize_subset_offset (c, out->markAttachClassDef2,
							   markAttachClassDef2, این, nullptr, نادرست, درست);
    }
    وگرنه
#endif
    {
#ifndef HB_NO_BEYOND_64K
      اگر (use_v14) out->markAttachClassDef2 = 0;
#endif
      subset_markattachclassdef = out->markAttachClassDef.serialize_subset (c, markAttachClassDef,
									   این, nullptr, نادرست, درست);
    }

#ifndef HB_NO_BEYOND_64K
    اگر (use_v14 && ligCaretList2)
    {
      out->ligCaretList = 0;
      subset_ligcaretlist = serialize_subset_offset (c, out->ligCaretList2, ligCaretList2, این);
    }
    وگرنه
#endif
    {
#ifndef HB_NO_BEYOND_64K
      اگر (use_v14) out->ligCaretList2 = 0;
#endif
      subset_ligcaretlist = out->ligCaretList.serialize_subset (c, ligCaretList, این);
    }

    اگر (subset_varstore && varstore_index != (unsigned) -1) {
      c->serializer->repack_last(varstore_index);
    }

    return_trace (subset_glyphclassdef || subset_attachlist ||
		  subset_ligcaretlist || subset_markattachclassdef ||
		  (out->version.to_int () >= 0x00010002u && subset_markglyphsetsdef) ||
		  (out->version.to_int () >= 0x00010003u && subset_varstore));
  }

  منطقی has_glyph_classes () ثابت
  {
#ifndef HB_NO_BEYOND_64K
    اگر (version.to_int () >= 0x00010004u && glyphClassDef2) بازگشت درست;
#endif
    بازگشت glyphClassDef != 0;
  }
  ثابت ClassDef &get_glyph_class_def () ثابت
  {
#ifndef HB_NO_BEYOND_64K
    اگر (version.to_int () >= 0x00010004u && glyphClassDef2) بازگشت این+glyphClassDef2;
#endif
    بازگشت این+glyphClassDef;
  }
  منطقی has_attach_list () ثابت
  {
#ifndef HB_NO_BEYOND_64K
    اگر (version.to_int () >= 0x00010004u && attachList2) بازگشت درست;
#endif
    بازگشت attachList != 0;
  }
  ثابت AttachList &get_attach_list () ثابت
  {
#ifndef HB_NO_BEYOND_64K
    اگر (version.to_int () >= 0x00010004u && attachList2) بازگشت این+attachList2;
#endif
    بازگشت این+attachList;
  }
  منطقی has_lig_carets () ثابت
  {
#ifndef HB_NO_BEYOND_64K
    اگر (version.to_int () >= 0x00010004u && ligCaretList2) بازگشت درست;
#endif
    بازگشت ligCaretList != 0;
  }
  منطقی has_mark_attachment_types () ثابت
  {
#ifndef HB_NO_BEYOND_64K
    اگر (version.to_int () >= 0x00010004u && markAttachClassDef2) بازگشت درست;
#endif
    بازگشت markAttachClassDef != 0;
  }
  ثابت ClassDef &get_mark_attach_class_def () ثابت
  {
#ifndef HB_NO_BEYOND_64K
    اگر (version.to_int () >= 0x00010004u && markAttachClassDef2) بازگشت این+markAttachClassDef2;
#endif
    بازگشت این+markAttachClassDef;
  }
  منطقی has_mark_glyph_sets () ثابت
  {
    اگر (version.to_int () < 0x00010002u) بازگشت نادرست;
#ifndef HB_NO_BEYOND_64K
    اگر (version.to_int () >= 0x00010004u && markGlyphSetsDef2) بازگشت درست;
#endif
    بازگشت markGlyphSetsDef != 0;
  }
  ثابت MarkGlyphSets &get_mark_glyph_sets () ثابت
  {
    اگر (version.to_int () < 0x00010002u) بازگشت Null(MarkGlyphSets);
#ifndef HB_NO_BEYOND_64K
    اگر (version.to_int () >= 0x00010004u && markGlyphSetsDef2) بازگشت این+markGlyphSetsDef2;
#endif
    بازگشت این+markGlyphSetsDef;
  }
  منطقی has_var_store () ثابت
  {
    بازگشت version.to_int () >= 0x00010003u && hb_barrier () && varStore != 0;
  }
  ثابت ItemVariationStore &get_var_store () ثابت
  {
    بازگشت version.to_int () >= 0x00010003u && hb_barrier () ? این+varStore : Null(ItemVariationStore);
  }


  منطقی has_data () ثابت { بازگشت version.to_int (); }
  unsigned صحیح get_glyph_class (hb_codepoint_t glyph) ثابت
  { بازگشت get_glyph_class_def ().get_class (glyph); }
  تهی get_glyphs_in_class (unsigned صحیح klass, hb_set_t *glyphs) ثابت
  { get_glyph_class_def ().collect_class (glyphs, klass); }

  unsigned صحیح get_mark_attachment_type (hb_codepoint_t glyph) ثابت
  { بازگشت get_mark_attach_class_def ().get_class (glyph); }

  unsigned صحیح get_attach_points (hb_codepoint_t glyph_id,
				  unsigned صحیح start_offset,
				  unsigned صحیح *point_شمار /* IN/OUT */,
				  unsigned صحیح *point_array /* OUT */) ثابت
  { بازگشت get_attach_list ().get_attach_points (glyph_id, start_offset, point_شمار, point_array); }

  unsigned صحیح get_lig_carets (hb_font_t *font,
			       hb_direction_t direction,
			       hb_codepoint_t glyph_id,
			       unsigned صحیح start_offset,
			       unsigned صحیح *caret_شمار /* IN/OUT */,
			       hb_position_t *caret_array /* OUT */) ثابت
  {
#ifndef HB_NO_BEYOND_64K
    اگر (version.to_int () >= 0x00010004u && ligCaretList2)
      بازگشت (این+ligCaretList2).get_lig_carets (font, direction, glyph_id, get_var_store (),
						  start_offset, caret_شمار, caret_array);
#endif
    بازگشت (این+ligCaretList).get_lig_carets (font, direction, glyph_id, get_var_store (),
					       start_offset, caret_شمار, caret_array);
  }

  منطقی mark_set_covers (unsigned صحیح set_index, hb_codepoint_t glyph_id) ثابت
  { بازگشت get_mark_glyph_sets ().covers (set_index, glyph_id); }

  /* glyph_props is a 16-bit integer where the lower 8-bit have bits representing
   * glyph class and other bits, and high 8-bit the mark attachment type (if any).
   * Not to be confused with lookup_props which is very similar. */
  unsigned صحیح get_glyph_props (hb_codepoint_t glyph) ثابت
  {
    unsigned صحیح klass = get_glyph_class (glyph);

    static_assert (((unsigned صحیح) HB_OT_LAYOUT_GLYPH_PROPS_BASE_GLYPH == (unsigned صحیح) LookupFlag::IgnoreBaseGlyphs), "");
    static_assert (((unsigned صحیح) HB_OT_LAYOUT_GLYPH_PROPS_LIGATURE == (unsigned صحیح) LookupFlag::IgnoreLigatures), "");
    static_assert (((unsigned صحیح) HB_OT_LAYOUT_GLYPH_PROPS_MARK == (unsigned صحیح) LookupFlag::IgnoreMarks), "");

    انتخاب (klass) {
    پیشفرض:			بازگشت HB_OT_LAYOUT_GLYPH_CLASS_UNCLASSIFIED;
    حالت BaseGlyph:		بازگشت HB_OT_LAYOUT_GLYPH_PROPS_BASE_GLYPH;
    حالت LigatureGlyph:		بازگشت HB_OT_LAYOUT_GLYPH_PROPS_LIGATURE;
    حالت MarkGlyph:
	  klass = get_mark_attachment_type (glyph);
	  بازگشت HB_OT_LAYOUT_GLYPH_PROPS_MARK | (klass << 8);
    }
  }

  HB_INTERNAL منطقی is_blocklisted (hb_blob_t *blob,
				   hb_face_t *face) ثابت;

  ساختار accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      table = hb_sanitize_context_t ().reference_table<GDEF> (face);
      اگر (unlikely (table->is_blocklisted (table.get_blob (), face)))
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
	اگر (!sets.in_error () &&
	    mark_glyph_set_bitmaps.resize (sets.length))
	  برای (unsigned i = 0; i < sets.length; i++)
	  {
	    auto &bitmap = mark_glyph_set_bitmaps.arrayZ[i];
	    bitmap.bias = HB_SET_مقدار_INVALID;
	    ثابت auto &set = sets.arrayZ[i];
	    اگر (unlikely (set.in_error ())) ادامه;
	    hb_codepoint_t min_gid = HB_SET_مقدار_INVALID;
	    hb_codepoint_t max_gid = HB_SET_مقدار_INVALID;
	    hb_set_next (&set, &min_gid);
	    hb_set_previous (&set, &max_gid);
	    اگر (max_gid == HB_SET_مقدار_INVALID)
	      ادامه;
	    اگر (max_gid - min_gid >= hb_bit_page_t::PAGE_BITS)
	    {
	      /* Too wide for one page; a digest pre-filter instead. */
	      برای (hb_codepoint_t gid : set)
		bitmap.u.digest.add (gid);
	      ادامه;
	    }
	    bitmap.bias = min_gid;
	    برای (hb_codepoint_t gid : set)
	      bitmap.u.page.add (gid - min_gid);
	  }
      }
#endif
    }
    ~accelerator_t () { table.destroy (); }

    unsigned صحیح get_glyph_props (hb_codepoint_t glyph) ثابت
    {
      unsigned v;

#ifndef HB_NO_GDEF_CACHE
      اگر (glyph_props_cache.get (glyph, &v))
        بازگشت v;
#endif

      v = table->get_glyph_props (glyph);

#ifndef HB_NO_GDEF_CACHE
      اگر (likely (table.get_blob ())) // Don't try setting if we are the null instance!
	glyph_props_cache.set (glyph, v);
#endif

      بازگشت v;

    }

    HB_ALWAYS_INLINE
    منطقی mark_set_covers (unsigned صحیح set_index, hb_codepoint_t glyph_id) ثابت
    {
#ifndef HB_NO_GDEF_CACHE
      // We can access arrayZ directly because of sanitize_lookup_props() guarantee.
      ثابت auto &bitmap = mark_glyph_set_bitmaps.arrayZ[set_index];
      اگر (likely (bitmap.bias != HB_SET_مقدار_INVALID))
      {
	hb_codepoint_t biased = glyph_id - bitmap.bias;
	بازگشت biased < hb_bit_page_t::PAGE_BITS &&
	       bitmap.u.page.get (biased);
      }
      بازگشت mark_set_covers_slow (set_index, glyph_id);
#وگرنه
      بازگشت table->mark_set_covers (set_index, glyph_id);
#endif
    }

#ifndef HB_NO_GDEF_CACHE
    HB_NEVER_INLINE
    منطقی mark_set_covers_slow (unsigned صحیح set_index, hb_codepoint_t glyph_id) ثابت
    {
      ثابت auto &bitmap = mark_glyph_set_bitmaps.arrayZ[set_index];
      اگر (!bitmap.u.digest.may_have (glyph_id))
	بازگشت نادرست;
      بازگشت table->mark_set_covers (set_index, glyph_id);
    }
#endif

    unsigned sanitize_lookup_props (unsigned lookup_props) ثابت
    {
#ifndef HB_NO_GDEF_CACHE
      اگر (lookup_props & LookupFlag::UseMarkFilteringSet &&
	  (lookup_props >> 16) >= mark_glyph_set_bitmaps.length)
      {
        // Invalid mark filtering set index; unset the flag.
	lookup_props &= ~LookupFlag::UseMarkFilteringSet;
      }
#endif
      بازگشت lookup_props;
    }

    hb_blob_ptr_t<GDEF> table;
#ifndef HB_NO_GDEF_CACHE
    ساختار mark_glyph_set_bitmap_t
    {
      /* Gid of the first bit (not page-aligned) for an exact one-page
       * set, or HB_SET_VALUE_INVALID; then digest is a pre-filter for
       * the table.  The all-zeros state is valid for both members. */
      hb_codepoint_t bias = HB_SET_مقدار_INVALID;
      اجتماع u_t {
	u_t () : page () {}
	hb_bit_page_t page;
	hb_set_digest_t digest;
      } u;
    };

    hb_vector_t<mark_glyph_set_bitmap_t> mark_glyph_set_bitmaps;
    mutable hb_cache_t<21, 3> glyph_props_cache;
    static_assert (اندازه (glyph_props_cache) == 512, "");
#endif
  };

  تهی collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
#ifndef HB_NO_BEYOND_64K
    اگر (version.to_int () >= 0x00010004u && ligCaretList2)
    {
      (این+ligCaretList2).collect_variation_indices (c);
      بازگشت;
    }
#endif
    (این+ligCaretList).collect_variation_indices (c);
  }

  محافظتشده:
  template <typename کلید>
  ایستا تهی remap_varidx_after_instantiation (ثابت hb_map_t& varidx_map,
                                                hb_hashmap_t<کلید, hb_pair_t<unsigned, صحیح>>& layout_variation_idx_delta_map /* IN/OUT */)
  {
    /* varidx_map is empty which means varstore is empty after instantiation,
     * no variations, map all varidx to HB_OT_LAYOUT_NO_VARIATIONS_INDEX.
     * varidx_map doesn't have original varidx, indicating delta row is all
     * zeros, map varidx to HB_OT_LAYOUT_NO_VARIATIONS_INDEX */
    برای (auto _ : layout_variation_idx_delta_map.iter_ref ())
    {
      /* old_varidx->(varidx, delta) mapping generated for subsetting, then this
       * varidx is used as key of varidx_map during instantiation */
      uint32_t varidx = _.second.first;
      uint32_t *new_varidx;
      اگر (varidx_map.has (varidx, &new_varidx))
	_.second.first = *new_varidx;
      وگرنه
	_.second.first = HB_OT_LAYOUT_NO_VARIATIONS_INDEX;
    }
  }

  template <typename OffsetOut, typename OffsetIn, typename Base, typename ...Ts>
  ایستا منطقی serialize_subset_offset (hb_subset_context_t *c,
				       OffsetOut& out,
				       ثابت OffsetIn& in,
				       ثابت Base *base,
				       Ts&&... ds)
  {
    out = 0;
    اگر (in.is_null ()) بازگشت نادرست;

    auto *s = c->serializer;
    s->push ();
    منطقی ret = c->dispatch (base+in, std::forward<Ts> (ds)...);
    اگر (ret) s->add_link (out, s->برداشت_pack ());
    وگرنه s->برداشت_discard ();
    بازگشت ret;
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
  عمومی:
  DEFINE_SIZE_MIN (12);
};

ساختار GDEF_accelerator_t : GDEF::accelerator_t {
  GDEF_accelerator_t (hb_face_t *face) : GDEF::accelerator_t (face) {}
};

} /* namespace OT */


#endif /* OT_LAYOUT_GDEF_GDEF_HH */
