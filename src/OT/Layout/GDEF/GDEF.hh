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

#ضمن "../../../hb-ot-var-common.hh"

#ضمن "../../../hb-font.hh"
#ضمن "../../../hb-cache.hh"


نطاق OT {


/*
 * Attachment List Table
 */

/* Array of contour point indices--in increasing numerical order */
بنية AttachPoint : Array16Of<HBUINT16>
{
  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    auto *out = c->serializer->start_embed (*هذا);
    return_trace (out->serialize (c->serializer, + iter ()));
  }
};

/* AttachList is referenced by both GDEF attachListOffset and
 * attachListOffset2. The v1.4 top-level offset widens, but the
 * AttachList table itself keeps Offset16 internals. */
بنية AttachList
{
  unsigned صحيح get_attach_points (hb_codepoint_t glyph_id,
				  unsigned صحيح start_offset,
				  unsigned صحيح *point_count /* IN/OUT */,
				  unsigned صحيح *point_array /* OUT */) ثابت
  {
    unsigned صحيح index = (هذا+coverage).get_coverage (glyph_id);
    ܐܢ (index == NOT_COVERED)
    {
      ܐܢ (point_count)
	*point_count = 0;
      ܦܢܐ 0;
    }

    ثابت AttachPoint &points = هذا+attachPoint[index];

    ܐܢ (point_count && point_array)
    {
      + points.as_array ().sub_array (start_offset, point_count)
      | hb_sink (hb_array (point_array, *point_count))
      ;
    }

    ܦܢܐ points.len;
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*هذا);
    ܐܢ (unlikely (!c->serializer->extend_min (out))) return_trace (ܕܓܠܐ);

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (هذا+coverage, attachPoint)
    | hb_filter (glyphset, hb_first)
    | hb_filter (subset_offset_array (c, out->attachPoint, هذا), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;
    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (منطقي (new_coverage));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (coverage.sanitize (c, هذا) && attachPoint.sanitize (c, هذا));
  }

  محمي:
  Offset16To<Coverage>
		coverage;		/* Offset to Coverage table -- from
					 * beginning of AttachList table */
  Array16OfOffset16To<AttachPoint>
		attachPoint;		/* Array of AttachPoint tables
					 * in Coverage Index order */
  عام:
  DEFINE_SIZE_ARRAY (4, attachPoint);
};

/*
 * Ligature Caret Table
 */

بنية CaretValueFormat1
{
  friend بنية CaretValue;
  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    auto *out = c->serializer->embed (هذا);
    ܐܢ (unlikely (!out)) return_trace (ܕܓܠܐ);
    return_trace (ܫܪܪܐ);
  }

  خاص:
  hb_position_t get_caret_value (hb_font_t *font, hb_direction_t direction) ثابت
  {
    ܦܢܐ HB_DIRECTION_IS_HORIZONTAL (direction) ? font->em_scale_x (coordinate) : font->em_scale_y (coordinate);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  محمي:
  HBUINT16	caretValueFormat;	/* Format identifier--format = 1 */
  FWORD		coordinate;		/* X or Y value, in design units */
  عام:
  DEFINE_SIZE_STATIC (4);
};

بنية CaretValueFormat2
{
  friend بنية CaretValue;
  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    auto *out = c->serializer->embed (هذا);
    ܐܢ (unlikely (!out)) return_trace (ܕܓܠܐ);
    return_trace (ܫܪܪܐ);
  }

  خاص:
  hb_position_t get_caret_value (hb_font_t *font, hb_direction_t direction, hb_codepoint_t glyph_id) ثابت
  {
    hb_position_t x, y;
    font->get_glyph_contour_point_for_origin (glyph_id, caretValuePoint, direction, &x, &y);
    ܦܢܐ HB_DIRECTION_IS_HORIZONTAL (direction) ? x : y;
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  محمي:
  HBUINT16	caretValueFormat;	/* Format identifier--format = 2 */
  HBUINT16	caretValuePoint;	/* Contour point index on glyph */
  عام:
  DEFINE_SIZE_STATIC (4);
};

بنية CaretValueFormat3
{
  friend بنية CaretValue;

  hb_position_t get_caret_value (hb_font_t *font, hb_direction_t direction,
				 ثابت ItemVariationStore &var_store) ثابت
  {
    ܦܢܐ HB_DIRECTION_IS_HORIZONTAL (direction) ?
	   hb_saturate_add (font->em_scale_x (coordinate), (هذا+deviceTable).get_x_delta (font, var_store)) :
	   hb_saturate_add (font->em_scale_y (coordinate), (هذا+deviceTable).get_y_delta (font, var_store));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    auto *out = c->serializer->start_embed (*هذا);
    ܐܢ (!c->serializer->embed (caretValueFormat)) return_trace (ܕܓܠܐ);
    ܐܢ (!c->serializer->embed (coordinate)) return_trace (ܕܓܠܐ);

    unsigned varidx = (هذا+deviceTable).get_variation_index ();
    hb_pair_t<unsigned, صحيح> *new_varidx_delta;
    ܐܢ (c->plan->layout_variation_idx_delta_map.has (varidx, &new_varidx_delta)) {
      uint32_t new_varidx = hb_first (*new_varidx_delta);
      صحيح delta = hb_second (*new_varidx_delta);
      ܐܢ (delta != 0)
      {
        ܐܢ (!c->serializer->check_assign (out->coordinate, coordinate + delta, HB_SERIALIZE_ERROR_INT_OVERFLOW))
          return_trace (ܕܓܠܐ);
      }

      ܐܢ (new_varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
        return_trace (c->serializer->check_assign (out->caretValueFormat, 1, HB_SERIALIZE_ERROR_INT_OVERFLOW));
    }

    ܐܢ (!c->serializer->embed (deviceTable))
      return_trace (ܕܓܠܐ);

    return_trace (out->deviceTable.serialize_copy (c->serializer, deviceTable, هذا, c->serializer->to_bias (out),
						   hb_serialize_context_t::Head, &c->plan->layout_variation_idx_delta_map));
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  { (هذا+deviceTable).collect_variation_indices (c); }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && deviceTable.sanitize (c, هذا));
  }

  محمي:
  HBUINT16	caretValueFormat;	/* Format identifier--format = 3 */
  FWORD		coordinate;		/* X or Y value, in design units */
  Offset16To<Device>
		deviceTable;		/* Offset to Device table for X or Y
					 * value--from beginning of CaretValue
					 * table */
  عام:
  DEFINE_SIZE_STATIC (6);
};

بنية CaretValue
{
  hb_position_t get_caret_value (hb_font_t *font,
				 hb_direction_t direction,
				 hb_codepoint_t glyph_id,
				 ثابت ItemVariationStore &var_store) ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); ܦܢܐ u.format1.get_caret_value (font, direction);
    حالة 2: hb_barrier (); ܦܢܐ u.format2.get_caret_value (font, direction, glyph_id);
    حالة 3: hb_barrier (); ܦܢܐ u.format3.get_caret_value (font, direction, var_store);
    افتراضي:ܦܢܐ 0;
    }
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    ܐܢ (unlikely (!c->may_dispatch (هذا, &u.format.v))) ܦܢܐ c->no_dispatch_return_value ();
    TRACE_DISPATCH (هذا, u.format.v);
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    حالة 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    حالة 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    افتراضي:return_trace (c->default_return_value ());
    }
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    بدل (u.format.v) {
    حالة 1:
    حالة 2:
      ܦܢܐ;
    حالة 3:
      hb_barrier ();
      u.format3.collect_variation_indices (c);
      ܦܢܐ;
    افتراضي: ܦܢܐ;
    }
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    ܐܢ (!u.format.v.sanitize (c)) return_trace (ܕܓܠܐ);
    hb_barrier ();
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    حالة 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    حالة 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    افتراضي:return_trace (ܫܪܪܐ);
    }
  }

  محمي:
  اتحاد {
  بنية { HBUINT16 v; }	format;		/* Format identifier */
  CaretValueFormat1	format1;
  CaretValueFormat2	format2;
  CaretValueFormat3	format3;
  } u;
  عام:
  DEFINE_SIZE_UNION (2, format.v);
};

بنية LigGlyph
{
  unsigned get_lig_carets (hb_font_t            *font,
			   hb_direction_t        direction,
			   hb_codepoint_t        glyph_id,
			   ثابت ItemVariationStore &var_store,
			   unsigned              start_offset,
			   unsigned             *caret_count /* IN/OUT */,
			   hb_position_t        *caret_array /* OUT */) ثابت
  {
    ܐܢ (caret_count && caret_array)
    {
      + carets.as_array ().sub_array (start_offset, caret_count)
      | hb_map (hb_add (هذا))
      | hb_map ([&] (ثابت CaretValue &value) { ܦܢܐ value.get_caret_value (font, direction, glyph_id, var_store); })
      | hb_sink (hb_array (caret_array, *caret_count))
      ;
    }

    ܦܢܐ carets.len;
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    auto *out = c->serializer->start_embed (*هذا);
    ܐܢ (unlikely (!c->serializer->extend_min (out))) return_trace (ܕܓܠܐ);

    + hb_iter (carets)
    | hb_apply (subset_offset_array (c, out->carets, هذا))
    ;

    return_trace (منطقي (out->carets));
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    لكل (ثابت Offset16To<CaretValue>& offset : carets.iter ())
      (هذا+offset).collect_variation_indices (c);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (carets.sanitize (c, هذا));
  }

  محمي:
  Array16OfOffset16To<CaretValue>
		carets;			/* Offset array of CaretValue tables
					 * --from beginning of LigGlyph table
					 * --in increasing coordinate order */
  عام:
  DEFINE_SIZE_ARRAY (2, carets);
};

/* LigCaretList2 widens the LigCaretList coverage, count, and LigGlyph
 * offsets. The LigGlyph table itself keeps Offset16 caretValueOffsets. */
template <typename Types>
بنية LigCaretList
{
  unsigned صحيح get_lig_carets (hb_font_t *font,
			       hb_direction_t direction,
			       hb_codepoint_t glyph_id,
			       ثابت ItemVariationStore &var_store,
			       unsigned صحيح start_offset,
			       unsigned صحيح *caret_count /* IN/OUT */,
			       hb_position_t *caret_array /* OUT */) ثابت
  {
    unsigned صحيح index = (هذا+coverage).get_coverage (glyph_id);
    ܐܢ (index == NOT_COVERED)
    {
      ܐܢ (caret_count)
	*caret_count = 0;
      ܦܢܐ 0;
    }
    ثابت LigGlyph &lig_glyph = هذا+ligGlyph[index];
    ܦܢܐ lig_glyph.get_lig_carets (font, direction, glyph_id, var_store, start_offset, caret_count, caret_array);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*هذا);
    ܐܢ (unlikely (!c->serializer->extend_min (out))) return_trace (ܕܓܠܐ);

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (هذا+coverage, ligGlyph)
    | hb_filter (glyphset, hb_first)
    | hb_filter (subset_offset_array (c, out->ligGlyph, هذا), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;
    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (منطقي (new_coverage));
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    + hb_zip (هذا+coverage, ligGlyph)
    | hb_filter (c->glyph_set, hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (هذا))
    | hb_apply ([c] (ثابت LigGlyph& _) { _.collect_variation_indices (c); })
    ;
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (coverage.sanitize (c, هذا) && ligGlyph.sanitize (c, هذا));
  }

  محمي:
  typename Types::template LOffsetTo<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of LigCaretList table */
  typename Types::template ArrayOf<typename Types::template OffsetTo<LigGlyph>>
		ligGlyph;		/* Array of LigGlyph tables
					 * in Coverage Index order */
  عام:
  DEFINE_SIZE_ARRAY (Types::LOffset::static_size + Types::HBUINT::static_size, ligGlyph);
};

#ifndef HB_NO_BEYOND_64K
static_assert (LigCaretList<MediumTypes>::min_size ==
	       Offset32::static_size + HBUINT24::static_size, "");
#endif


بنية MarkGlyphSetsFormat1
{
  منطقي covers (unsigned صحيح set_index, hb_codepoint_t glyph_id) ثابت
  { ܦܢܐ (هذا+coverage[set_index]).get_coverage (glyph_id) != NOT_COVERED; }

  فراغ collect_used_mark_sets (ثابت hb_set_t& glyph_set,
                               hb_set_t& used_mark_sets /* OUT */) ثابت
  {
    unsigned i = 0;
    لكل (ثابت auto &offset : coverage)
     {
       ثابت auto &cov = هذا+offset;
       ܐܢ (cov.intersects (&glyph_set))
         used_mark_sets.add (i);

       i++;
     }
  }

  template <typename set_t>
  فراغ collect_coverage (hb_vector_t<set_t> &sets) ثابت
  {
     لكل (ثابت auto &offset : coverage)
     {
       ثابت auto &cov = هذا+offset;
       cov.collect_coverage (sets.push ());
     }
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    auto *out = c->serializer->start_embed (*هذا);
    ܐܢ (unlikely (!c->serializer->extend_min (out))) return_trace (ܕܓܠܐ);
    out->format = format;

    منطقي ret = ܫܪܪܐ;
    لكل (ثابت Offset32To<Coverage>& offset : coverage.iter ())
    {
      auto snap = c->serializer->snapshot ();
      auto *o = out->coverage.serialize_append (c->serializer);
      ܐܢ (unlikely (!o))
      {
	ret = ܕܓܠܐ;
	اكسر;
      }

      //skip empty coverage
      c->serializer->push ();
      منطقي res = ܕܓܠܐ;
      ܐܢ (offset) res = c->dispatch (هذا+offset);
      ܐܢ (!res)
      {
        c->serializer->pop_discard ();
        c->serializer->revert (snap);
        (out->coverage.len)--;
        استمر;
      }
      c->serializer->add_link (*o, c->serializer->pop_pack ());
    }

    return_trace (ret && out->coverage.len);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (coverage.sanitize (c, هذا));
  }

  محمي:
  HBUINT16	format;			/* Format identifier--format = 1 */
  Array16Of<Offset32To<Coverage>>
		coverage;		/* Array of long offsets to mark set
					 * coverage tables */
  عام:
  DEFINE_SIZE_ARRAY (4, coverage);
};

بنية MarkGlyphSets
{
  منطقي covers (unsigned صحيح set_index, hb_codepoint_t glyph_id) ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); ܦܢܐ u.format1.covers (set_index, glyph_id);
    افتراضي:ܦܢܐ ܕܓܠܐ;
    }
  }

  template <typename set_t>
  فراغ collect_coverage (hb_vector_t<set_t> &sets) ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); u.format1.collect_coverage (sets); ܦܢܐ;
    افتراضي:ܦܢܐ;
    }
  }

  فراغ collect_used_mark_sets (ثابت hb_set_t& glyph_set,
                               hb_set_t& used_mark_sets /* OUT */) ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); u.format1.collect_used_mark_sets (glyph_set, used_mark_sets); ܦܢܐ;
    افتراضي:ܦܢܐ;
    }
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (u.format1.subset (c));
    افتراضي:return_trace (ܕܓܠܐ);
    }
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    ܐܢ (!u.format.v.sanitize (c)) return_trace (ܕܓܠܐ);
    hb_barrier ();
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    افتراضي:return_trace (ܫܪܪܐ);
    }
  }

  محمي:
  اتحاد {
  بنية { HBUINT16 v; }	format;		/* Format identifier */
  MarkGlyphSetsFormat1	format1;
  } u;
  عام:
  DEFINE_SIZE_UNION (2, format.v);
};


/*
 * GDEF -- Glyph Definition
 * https://docs.microsoft.com/en-us/typography/opentype/spec/gdef
 */


بنية GDEF
{
  ساكن constexpr hb_tag_t tableTag = HB_OT_TAG_GDEF;

  تعداد GlyphClasses {
    UnclassifiedGlyph	= 0,
    BaseGlyph		= 1,
    LigatureGlyph	= 2,
    MarkGlyph		= 3,
    ComponentGlyph	= 4
  };

  size_t get_size () ثابت
  {
    ܐܢ (version.major != 1) ܦܢܐ version.static_size;

    ܦܢܐ min_size +
	   (version.to_int () >= 0x00010002u ? markGlyphSetsDef.static_size : 0) +
	   (version.to_int () >= 0x00010003u ? varStore.static_size : 0) +
#ifndef HB_NO_BEYOND_64K
	   (version.to_int () >= 0x00010004u ? 5 * Offset32::static_size : 0)
#ܐܚܪܢܐ
	   0
#endif
	   ;
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    ܐܢ (unlikely (!version.sanitize (c) ||
		  version.major != 1 ||
		  !c->check_range (هذا, get_size ())))
      return_trace (ܕܓܠܐ);
    hb_barrier ();

    منطقي glyph_class_def_ok = glyphClassDef.sanitize (c, هذا);
    منطقي attach_list_ok = attachList.sanitize (c, هذا);
    منطقي lig_caret_list_ok = ligCaretList.sanitize (c, هذا);
    منطقي mark_attach_class_def_ok = markAttachClassDef.sanitize (c, هذا);
    منطقي mark_glyph_sets_def_ok = version.to_int () < 0x00010002u ||
				  markGlyphSetsDef.sanitize (c, هذا);

#ifndef HB_NO_BEYOND_64K
    ܐܢ (version.to_int () >= 0x00010004u)
    {
      ܐܢ (glyphClassDef2) glyph_class_def_ok = glyphClassDef2.sanitize (c, هذا);
      ܐܢ (attachList2) attach_list_ok = attachList2.sanitize (c, هذا);
      ܐܢ (ligCaretList2) lig_caret_list_ok = ligCaretList2.sanitize (c, هذا);
      ܐܢ (markAttachClassDef2) mark_attach_class_def_ok = markAttachClassDef2.sanitize (c, هذا);
      ܐܢ (markGlyphSetsDef2) mark_glyph_sets_def_ok = markGlyphSetsDef2.sanitize (c, هذا);
    }
#endif

    return_trace (glyph_class_def_ok &&
		  attach_list_ok &&
		  lig_caret_list_ok &&
		  mark_attach_class_def_ok &&
		  mark_glyph_sets_def_ok &&
		  (version.to_int () < 0x00010003u || varStore.sanitize (c, هذا)));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ܐܢ (version.major != 1) return_trace (ܕܓܠܐ);

    auto *out = c->serializer->start_embed (*هذا);
    ܐܢ (unlikely (!c->serializer->extend_min (out))) return_trace (ܕܓܠܐ);

    // Push var store first (if it's needed) so that it's last in the
    // serialization order. Some font consumers assume that varstore runs to
    // the end of the GDEF table.
    // See: https://github.com/harfbuzz/harfbuzz/issues/4636
    auto snapshot_version0 = c->serializer->snapshot ();
    ܐܢ (unlikely (version.to_int () >= 0x00010002u && hb_barrier () &&
		  !c->serializer->embed (markGlyphSetsDef)))
      return_trace (ܕܓܠܐ);

    auto snapshot_version2 = c->serializer->snapshot ();
    ܐܢ (unlikely (version.to_int () >= 0x00010003u && hb_barrier () &&
		  !c->serializer->embed (varStore)))
      return_trace (ܕܓܠܐ);

    منطقي use_v14 = ܕܓܠܐ;
#ifndef HB_NO_BEYOND_64K
    use_v14 = version.to_int () >= 0x00010004u &&
	      (glyphClassDef2 || attachList2 || ligCaretList2 ||
	       markAttachClassDef2 || markGlyphSetsDef2);
    ܐܢ (unlikely (version.to_int () >= 0x00010004u && hb_barrier () &&
		  !c->serializer->extend_size (out, get_size ())))
      return_trace (ܕܓܠܐ);
#endif

    منطقي subset_varstore = ܕܓܠܐ;
    unsigned varstore_index = (unsigned) -1;
    ܐܢ (version.to_int () >= 0x00010003u && hb_barrier ())
    {
      ܐܢ (c->plan->all_axes_pinned)
	out->varStore = 0;
      ܐܚܪܢܐ ܐܢ (c->plan->normalized_coords)
      {
	ܐܢ (varStore)
	{
	  item_variations_t item_vars;
	  ܐܢ (unlikely (!c->serializer->check_success (
                  item_vars.instantiate (هذا+varStore, c->plan, ܫܪܪܐ, ܫܪܪܐ,
                                          c->plan->gdef_varstore_inner_maps.as_array (), ܫܪܪܐ))))
            return_trace (ܕܓܠܐ);
	  subset_varstore = out->varStore.serialize_serialize (c->serializer,
							     item_vars.has_long_word (),
							     c->plan->axis_tags,
							     item_vars.get_region_list (),
							     item_vars.get_vardata_encodings ());
	  varstore_index = c->serializer->last_added_child_index();
	  remap_varidx_after_instantiation (item_vars.get_varidx_map (),
					    c->plan->layout_variation_idx_delta_map);
          remap_varidx_after_instantiation (item_vars.get_varidx_map (),
                                            c->plan->layout_condition_idx_value_map);
	}
      }
      ܐܚܪܢܐ
      {
	subset_varstore = out->varStore.serialize_subset (c, varStore, هذا,
							  c->plan->gdef_varstore_inner_maps.as_array ());
	varstore_index = c->serializer->last_added_child_index();
      }
    }

    out->version.major = version.major;
    out->version.minor = version.minor;

    ܐܢ (!subset_varstore && version.to_int () >= 0x00010002u && !use_v14) {
      c->serializer->revert (snapshot_version2);
    }
    ܐܢ (!subset_varstore)
      out->varStore = 0;
    ܐܚܪܢܐ
      c->plan->has_gdef_varstore = ܫܪܪܐ;

    منطقي subset_markglyphsetsdef = ܕܓܠܐ;
    ܐܢ (version.to_int () >= 0x00010002u && hb_barrier ())
    {
#ifndef HB_NO_BEYOND_64K
      ܐܢ (use_v14 && markGlyphSetsDef2)
      {
	out->markGlyphSetsDef = 0;
	subset_markglyphsetsdef = serialize_subset_offset (c, out->markGlyphSetsDef2,
							   markGlyphSetsDef2, هذا);
      }
      ܐܚܪܢܐ
#endif
      {
#ifndef HB_NO_BEYOND_64K
	ܐܢ (use_v14) out->markGlyphSetsDef2 = 0;
#endif
	subset_markglyphsetsdef = out->markGlyphSetsDef.serialize_subset (c, markGlyphSetsDef, هذا);
      }
    }

    ܐܢ (use_v14)
    {
      out->version.minor = 4;
    } ܐܚܪܢܐ ܐܢ (subset_varstore) {
      out->version.minor = 3;
    } ܐܚܪܢܐ ܐܢ (subset_markglyphsetsdef) {
      out->version.minor = 2;
    } ܐܚܪܢܐ  {
      out->version.minor = 0;
      c->serializer->revert (snapshot_version0);
    }

    منطقي subset_glyphclassdef = ܕܓܠܐ;
    منطقي subset_attachlist = ܕܓܠܐ;
    منطقي subset_markattachclassdef = ܕܓܠܐ;
    منطقي subset_ligcaretlist = ܕܓܠܐ;

#ifndef HB_NO_BEYOND_64K
    ܐܢ (use_v14 && glyphClassDef2)
    {
      out->glyphClassDef = 0;
      subset_glyphclassdef = serialize_subset_offset (c, out->glyphClassDef2,
						      glyphClassDef2, هذا, nullptr, ܕܓܠܐ, ܫܪܪܐ);
    }
    ܐܚܪܢܐ
#endif
    {
#ifndef HB_NO_BEYOND_64K
      ܐܢ (use_v14) out->glyphClassDef2 = 0;
#endif
      subset_glyphclassdef = out->glyphClassDef.serialize_subset (c, glyphClassDef, هذا, nullptr, ܕܓܠܐ, ܫܪܪܐ);
    }

#ifndef HB_NO_BEYOND_64K
    ܐܢ (use_v14 && attachList2)
    {
      out->attachList = 0;
      subset_attachlist = serialize_subset_offset (c, out->attachList2, attachList2, هذا);
    }
    ܐܚܪܢܐ
#endif
    {
#ifndef HB_NO_BEYOND_64K
      ܐܢ (use_v14) out->attachList2 = 0;
#endif
      subset_attachlist = out->attachList.serialize_subset (c, attachList, هذا);
    }

#ifndef HB_NO_BEYOND_64K
    ܐܢ (use_v14 && markAttachClassDef2)
    {
      out->markAttachClassDef = 0;
      subset_markattachclassdef = serialize_subset_offset (c, out->markAttachClassDef2,
							   markAttachClassDef2, هذا, nullptr, ܕܓܠܐ, ܫܪܪܐ);
    }
    ܐܚܪܢܐ
#endif
    {
#ifndef HB_NO_BEYOND_64K
      ܐܢ (use_v14) out->markAttachClassDef2 = 0;
#endif
      subset_markattachclassdef = out->markAttachClassDef.serialize_subset (c, markAttachClassDef,
									   هذا, nullptr, ܕܓܠܐ, ܫܪܪܐ);
    }

#ifndef HB_NO_BEYOND_64K
    ܐܢ (use_v14 && ligCaretList2)
    {
      out->ligCaretList = 0;
      subset_ligcaretlist = serialize_subset_offset (c, out->ligCaretList2, ligCaretList2, هذا);
    }
    ܐܚܪܢܐ
#endif
    {
#ifndef HB_NO_BEYOND_64K
      ܐܢ (use_v14) out->ligCaretList2 = 0;
#endif
      subset_ligcaretlist = out->ligCaretList.serialize_subset (c, ligCaretList, هذا);
    }

    ܐܢ (subset_varstore && varstore_index != (unsigned) -1) {
      c->serializer->repack_last(varstore_index);
    }

    return_trace (subset_glyphclassdef || subset_attachlist ||
		  subset_ligcaretlist || subset_markattachclassdef ||
		  (out->version.to_int () >= 0x00010002u && subset_markglyphsetsdef) ||
		  (out->version.to_int () >= 0x00010003u && subset_varstore));
  }

  منطقي has_glyph_classes () ثابت
  {
#ifndef HB_NO_BEYOND_64K
    ܐܢ (version.to_int () >= 0x00010004u && glyphClassDef2) ܦܢܐ ܫܪܪܐ;
#endif
    ܦܢܐ glyphClassDef != 0;
  }
  ثابت ClassDef &get_glyph_class_def () ثابت
  {
#ifndef HB_NO_BEYOND_64K
    ܐܢ (version.to_int () >= 0x00010004u && glyphClassDef2) ܦܢܐ هذا+glyphClassDef2;
#endif
    ܦܢܐ هذا+glyphClassDef;
  }
  منطقي has_attach_list () ثابت
  {
#ifndef HB_NO_BEYOND_64K
    ܐܢ (version.to_int () >= 0x00010004u && attachList2) ܦܢܐ ܫܪܪܐ;
#endif
    ܦܢܐ attachList != 0;
  }
  ثابت AttachList &get_attach_list () ثابت
  {
#ifndef HB_NO_BEYOND_64K
    ܐܢ (version.to_int () >= 0x00010004u && attachList2) ܦܢܐ هذا+attachList2;
#endif
    ܦܢܐ هذا+attachList;
  }
  منطقي has_lig_carets () ثابت
  {
#ifndef HB_NO_BEYOND_64K
    ܐܢ (version.to_int () >= 0x00010004u && ligCaretList2) ܦܢܐ ܫܪܪܐ;
#endif
    ܦܢܐ ligCaretList != 0;
  }
  منطقي has_mark_attachment_types () ثابت
  {
#ifndef HB_NO_BEYOND_64K
    ܐܢ (version.to_int () >= 0x00010004u && markAttachClassDef2) ܦܢܐ ܫܪܪܐ;
#endif
    ܦܢܐ markAttachClassDef != 0;
  }
  ثابت ClassDef &get_mark_attach_class_def () ثابت
  {
#ifndef HB_NO_BEYOND_64K
    ܐܢ (version.to_int () >= 0x00010004u && markAttachClassDef2) ܦܢܐ هذا+markAttachClassDef2;
#endif
    ܦܢܐ هذا+markAttachClassDef;
  }
  منطقي has_mark_glyph_sets () ثابت
  {
    ܐܢ (version.to_int () < 0x00010002u) ܦܢܐ ܕܓܠܐ;
#ifndef HB_NO_BEYOND_64K
    ܐܢ (version.to_int () >= 0x00010004u && markGlyphSetsDef2) ܦܢܐ ܫܪܪܐ;
#endif
    ܦܢܐ markGlyphSetsDef != 0;
  }
  ثابت MarkGlyphSets &get_mark_glyph_sets () ثابت
  {
    ܐܢ (version.to_int () < 0x00010002u) ܦܢܐ Null(MarkGlyphSets);
#ifndef HB_NO_BEYOND_64K
    ܐܢ (version.to_int () >= 0x00010004u && markGlyphSetsDef2) ܦܢܐ هذا+markGlyphSetsDef2;
#endif
    ܦܢܐ هذا+markGlyphSetsDef;
  }
  منطقي has_var_store () ثابت
  {
    ܦܢܐ version.to_int () >= 0x00010003u && hb_barrier () && varStore != 0;
  }
  ثابت ItemVariationStore &get_var_store () ثابت
  {
    ܦܢܐ version.to_int () >= 0x00010003u && hb_barrier () ? هذا+varStore : Null(ItemVariationStore);
  }


  منطقي has_data () ثابت { ܦܢܐ version.to_int (); }
  unsigned صحيح get_glyph_class (hb_codepoint_t glyph) ثابت
  { ܦܢܐ get_glyph_class_def ().get_class (glyph); }
  فراغ get_glyphs_in_class (unsigned صحيح klass, hb_set_t *glyphs) ثابت
  { get_glyph_class_def ().collect_class (glyphs, klass); }

  unsigned صحيح get_mark_attachment_type (hb_codepoint_t glyph) ثابت
  { ܦܢܐ get_mark_attach_class_def ().get_class (glyph); }

  unsigned صحيح get_attach_points (hb_codepoint_t glyph_id,
				  unsigned صحيح start_offset,
				  unsigned صحيح *point_count /* IN/OUT */,
				  unsigned صحيح *point_array /* OUT */) ثابت
  { ܦܢܐ get_attach_list ().get_attach_points (glyph_id, start_offset, point_count, point_array); }

  unsigned صحيح get_lig_carets (hb_font_t *font,
			       hb_direction_t direction,
			       hb_codepoint_t glyph_id,
			       unsigned صحيح start_offset,
			       unsigned صحيح *caret_count /* IN/OUT */,
			       hb_position_t *caret_array /* OUT */) ثابت
  {
#ifndef HB_NO_BEYOND_64K
    ܐܢ (version.to_int () >= 0x00010004u && ligCaretList2)
      ܦܢܐ (هذا+ligCaretList2).get_lig_carets (font, direction, glyph_id, get_var_store (),
						  start_offset, caret_count, caret_array);
#endif
    ܦܢܐ (هذا+ligCaretList).get_lig_carets (font, direction, glyph_id, get_var_store (),
					       start_offset, caret_count, caret_array);
  }

  منطقي mark_set_covers (unsigned صحيح set_index, hb_codepoint_t glyph_id) ثابت
  { ܦܢܐ get_mark_glyph_sets ().covers (set_index, glyph_id); }

  /* glyph_props is a 16-bit integer where the lower 8-bit have bits representing
   * glyph class and other bits, and high 8-bit the mark attachment type (if any).
   * Not to be confused with lookup_props which is very similar. */
  unsigned صحيح get_glyph_props (hb_codepoint_t glyph) ثابت
  {
    unsigned صحيح klass = get_glyph_class (glyph);

    static_assert (((unsigned صحيح) HB_OT_LAYOUT_GLYPH_PROPS_BASE_GLYPH == (unsigned صحيح) LookupFlag::IgnoreBaseGlyphs), "");
    static_assert (((unsigned صحيح) HB_OT_LAYOUT_GLYPH_PROPS_LIGATURE == (unsigned صحيح) LookupFlag::IgnoreLigatures), "");
    static_assert (((unsigned صحيح) HB_OT_LAYOUT_GLYPH_PROPS_MARK == (unsigned صحيح) LookupFlag::IgnoreMarks), "");

    بدل (klass) {
    افتراضي:			ܦܢܐ HB_OT_LAYOUT_GLYPH_CLASS_UNCLASSIFIED;
    حالة BaseGlyph:		ܦܢܐ HB_OT_LAYOUT_GLYPH_PROPS_BASE_GLYPH;
    حالة LigatureGlyph:		ܦܢܐ HB_OT_LAYOUT_GLYPH_PROPS_LIGATURE;
    حالة MarkGlyph:
	  klass = get_mark_attachment_type (glyph);
	  ܦܢܐ HB_OT_LAYOUT_GLYPH_PROPS_MARK | (klass << 8);
    }
  }

  HB_INTERNAL منطقي is_blocklisted (hb_blob_t *blob,
				   hb_face_t *face) ثابت;

  بنية accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      table = hb_sanitize_context_t ().reference_table<GDEF> (face);
      ܐܢ (unlikely (table->is_blocklisted (table.get_blob (), face)))
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
	ܐܢ (!sets.in_error () &&
	    mark_glyph_set_bitmaps.resize (sets.length))
	  لكل (unsigned i = 0; i < sets.length; i++)
	  {
	    auto &bitmap = mark_glyph_set_bitmaps.arrayZ[i];
	    bitmap.bias = HB_SET_VALUE_INVALID;
	    ثابت auto &set = sets.arrayZ[i];
	    ܐܢ (unlikely (set.in_error ())) استمر;
	    hb_codepoint_t min_gid = HB_SET_VALUE_INVALID;
	    hb_codepoint_t max_gid = HB_SET_VALUE_INVALID;
	    hb_set_next (&set, &min_gid);
	    hb_set_previous (&set, &max_gid);
	    ܐܢ (max_gid == HB_SET_VALUE_INVALID)
	      استمر;
	    ܐܢ (max_gid - min_gid >= hb_bit_page_t::PAGE_BITS)
	    {
	      /* Too wide for one page; a digest pre-filter instead. */
	      لكل (hb_codepoint_t gid : set)
		bitmap.u.digest.add (gid);
	      استمر;
	    }
	    bitmap.bias = min_gid;
	    لكل (hb_codepoint_t gid : set)
	      bitmap.u.page.add (gid - min_gid);
	  }
      }
#endif
    }
    ~accelerator_t () { table.destroy (); }

    unsigned صحيح get_glyph_props (hb_codepoint_t glyph) ثابت
    {
      unsigned v;

#ifndef HB_NO_GDEF_CACHE
      ܐܢ (glyph_props_cache.get (glyph, &v))
        ܦܢܐ v;
#endif

      v = table->get_glyph_props (glyph);

#ifndef HB_NO_GDEF_CACHE
      ܐܢ (likely (table.get_blob ())) // Don't try setting if we are the null instance!
	glyph_props_cache.set (glyph, v);
#endif

      ܦܢܐ v;

    }

    HB_ALWAYS_INLINE
    منطقي mark_set_covers (unsigned صحيح set_index, hb_codepoint_t glyph_id) ثابت
    {
#ifndef HB_NO_GDEF_CACHE
      // We can access arrayZ directly because of sanitize_lookup_props() guarantee.
      ثابت auto &bitmap = mark_glyph_set_bitmaps.arrayZ[set_index];
      ܐܢ (likely (bitmap.bias != HB_SET_VALUE_INVALID))
      {
	hb_codepoint_t biased = glyph_id - bitmap.bias;
	ܦܢܐ biased < hb_bit_page_t::PAGE_BITS &&
	       bitmap.u.page.get (biased);
      }
      ܦܢܐ mark_set_covers_slow (set_index, glyph_id);
#ܐܚܪܢܐ
      ܦܢܐ table->mark_set_covers (set_index, glyph_id);
#endif
    }

#ifndef HB_NO_GDEF_CACHE
    HB_NEVER_INLINE
    منطقي mark_set_covers_slow (unsigned صحيح set_index, hb_codepoint_t glyph_id) ثابت
    {
      ثابت auto &bitmap = mark_glyph_set_bitmaps.arrayZ[set_index];
      ܐܢ (!bitmap.u.digest.may_have (glyph_id))
	ܦܢܐ ܕܓܠܐ;
      ܦܢܐ table->mark_set_covers (set_index, glyph_id);
    }
#endif

    unsigned sanitize_lookup_props (unsigned lookup_props) ثابت
    {
#ifndef HB_NO_GDEF_CACHE
      ܐܢ (lookup_props & LookupFlag::UseMarkFilteringSet &&
	  (lookup_props >> 16) >= mark_glyph_set_bitmaps.length)
      {
        // Invalid mark filtering set index; unset the flag.
	lookup_props &= ~LookupFlag::UseMarkFilteringSet;
      }
#endif
      ܦܢܐ lookup_props;
    }

    hb_blob_ptr_t<GDEF> table;
#ifndef HB_NO_GDEF_CACHE
    بنية mark_glyph_set_bitmap_t
    {
      /* Gid of the first bit (not page-aligned) for an exact one-page
       * set, or HB_SET_VALUE_INVALID; then digest is a pre-filter for
       * the table.  The all-zeros state is valid for both members. */
      hb_codepoint_t bias = HB_SET_VALUE_INVALID;
      اتحاد u_t {
	u_t () : page () {}
	hb_bit_page_t page;
	hb_set_digest_t digest;
      } u;
    };

    hb_vector_t<mark_glyph_set_bitmap_t> mark_glyph_set_bitmaps;
    mutable hb_cache_t<21, 3> glyph_props_cache;
    static_assert (حجم (glyph_props_cache) == 512, "");
#endif
  };

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
#ifndef HB_NO_BEYOND_64K
    ܐܢ (version.to_int () >= 0x00010004u && ligCaretList2)
    {
      (هذا+ligCaretList2).collect_variation_indices (c);
      ܦܢܐ;
    }
#endif
    (هذا+ligCaretList).collect_variation_indices (c);
  }

  محمي:
  template <typename Key>
  ساكن فراغ remap_varidx_after_instantiation (ثابت hb_map_t& varidx_map,
                                                hb_hashmap_t<Key, hb_pair_t<unsigned, صحيح>>& layout_variation_idx_delta_map /* IN/OUT */)
  {
    /* varidx_map is empty which means varstore is empty after instantiation,
     * no variations, map all varidx to HB_OT_LAYOUT_NO_VARIATIONS_INDEX.
     * varidx_map doesn't have original varidx, indicating delta row is all
     * zeros, map varidx to HB_OT_LAYOUT_NO_VARIATIONS_INDEX */
    لكل (auto _ : layout_variation_idx_delta_map.iter_ref ())
    {
      /* old_varidx->(varidx, delta) mapping generated for subsetting, then this
       * varidx is used as key of varidx_map during instantiation */
      uint32_t varidx = _.second.first;
      uint32_t *new_varidx;
      ܐܢ (varidx_map.has (varidx, &new_varidx))
	_.second.first = *new_varidx;
      ܐܚܪܢܐ
	_.second.first = HB_OT_LAYOUT_NO_VARIATIONS_INDEX;
    }
  }

  template <typename OffsetOut, typename OffsetIn, typename Base, typename ...Ts>
  ساكن منطقي serialize_subset_offset (hb_subset_context_t *c,
				       OffsetOut& out,
				       ثابت OffsetIn& in,
				       ثابت Base *base,
				       Ts&&... ds)
  {
    out = 0;
    ܐܢ (in.is_null ()) ܦܢܐ ܕܓܠܐ;

    auto *s = c->serializer;
    s->push ();
    منطقي ret = c->dispatch (base+in, std::forward<Ts> (ds)...);
    ܐܢ (ret) s->add_link (out, s->pop_pack ());
    ܐܚܪܢܐ s->pop_discard ();
    ܦܢܐ ret;
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
  عام:
  DEFINE_SIZE_MIN (12);
};

بنية GDEF_accelerator_t : GDEF::accelerator_t {
  GDEF_accelerator_t (hb_face_t *face) : GDEF::accelerator_t (face) {}
};

} /* namespace OT */


#endif /* OT_LAYOUT_GDEF_GDEF_HH */
