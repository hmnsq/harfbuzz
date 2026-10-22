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
				  unsigned صحيح *point_عدد /* IN/OUT */,
				  unsigned صحيح *point_array /* OUT */) ثابت
  {
    unsigned صحيح index = (هذا+coverage).get_coverage (glyph_id);
    إذا (index == NOT_COVERED)
    {
      إذا (point_عدد)
	*point_عدد = 0;
      أرجع 0;
    }

    ثابت AttachPoint &points = هذا+attachPoint[index];

    إذا (point_عدد && point_array)
    {
      + points.as_array ().sub_array (start_offset, point_عدد)
      | hb_sink (hb_array (point_array, *point_عدد))
      ;
    }

    أرجع points.len;
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (out))) return_trace (خطأ);

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

بنية Caret_قيمة_Format1
{
  friend بنية Caret_قيمة;
  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    auto *out = c->serializer->embed (هذا);
    إذا (unlikely (!out)) return_trace (خطأ);
    return_trace (true);
  }

  خاص:
  hb_position_t get_caret_قيمة (hb_font_t *font, hb_direction_t direction) ثابت
  {
    أرجع HB_DIRECTION_IS_HORIZONTAL (direction) ? font->em_scale_x (coordinate) : font->em_scale_y (coordinate);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  محمي:
  HBUINT16	caret_قيمة_Format;	/* Format identifier--format = 1 */
  FWORD		coordinate;		/* X or Y value, in design units */
  عام:
  DEFINE_SIZE_STATIC (4);
};

بنية Caret_قيمة_Format2
{
  friend بنية Caret_قيمة;
  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    auto *out = c->serializer->embed (هذا);
    إذا (unlikely (!out)) return_trace (خطأ);
    return_trace (true);
  }

  خاص:
  hb_position_t get_caret_قيمة (hb_font_t *font, hb_direction_t direction, hb_codepoint_t glyph_id) ثابت
  {
    hb_position_t x, y;
    font->get_glyph_contour_point_for_origin (glyph_id, caret_قيمة_Point, direction, &x, &y);
    أرجع HB_DIRECTION_IS_HORIZONTAL (direction) ? x : y;
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  محمي:
  HBUINT16	caret_قيمة_Format;	/* Format identifier--format = 2 */
  HBUINT16	caret_قيمة_Point;	/* Contour point index on glyph */
  عام:
  DEFINE_SIZE_STATIC (4);
};

بنية Caret_قيمة_Format3
{
  friend بنية Caret_قيمة;

  hb_position_t get_caret_قيمة (hb_font_t *font, hb_direction_t direction,
				 ثابت ItemVariationStore &var_store) ثابت
  {
    أرجع HB_DIRECTION_IS_HORIZONTAL (direction) ?
	   hb_saturate_add (font->em_scale_x (coordinate), (هذا+deviceTable).get_x_delta (font, var_store)) :
	   hb_saturate_add (font->em_scale_y (coordinate), (هذا+deviceTable).get_y_delta (font, var_store));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    auto *out = c->serializer->start_embed (*هذا);
    إذا (!c->serializer->embed (caret_قيمة_Format)) return_trace (خطأ);
    إذا (!c->serializer->embed (coordinate)) return_trace (خطأ);

    unsigned varidx = (هذا+deviceTable).get_variation_index ();
    hb_pair_t<unsigned, صحيح> *new_varidx_delta;
    إذا (c->plan->layout_variation_idx_delta_map.has (varidx, &new_varidx_delta)) {
      uint32_t new_varidx = hb_first (*new_varidx_delta);
      صحيح delta = hb_second (*new_varidx_delta);
      إذا (delta != 0)
      {
        إذا (!c->serializer->check_assign (out->coordinate, coordinate + delta, HB_SERIALIZE_ERROR_INT_OVERFLOW))
          return_trace (خطأ);
      }

      إذا (new_varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
        return_trace (c->serializer->check_assign (out->caret_قيمة_Format, 1, HB_SERIALIZE_ERROR_INT_OVERFLOW));
    }

    إذا (!c->serializer->embed (deviceTable))
      return_trace (خطأ);

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
  HBUINT16	caret_قيمة_Format;	/* Format identifier--format = 3 */
  FWORD		coordinate;		/* X or Y value, in design units */
  Offset16To<Device>
		deviceTable;		/* Offset to Device table for X or Y
					 * value--from beginning of CaretValue
					 * table */
  عام:
  DEFINE_SIZE_STATIC (6);
};

بنية Caret_قيمة
{
  hb_position_t get_caret_قيمة (hb_font_t *font,
				 hb_direction_t direction,
				 hb_codepoint_t glyph_id,
				 ثابت ItemVariationStore &var_store) ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.get_caret_قيمة (font, direction);
    حالة 2: hb_barrier (); أرجع u.format2.get_caret_قيمة (font, direction, glyph_id);
    حالة 3: hb_barrier (); أرجع u.format3.get_caret_قيمة (font, direction, var_store);
    افتراضي:أرجع 0;
    }
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    إذا (unlikely (!c->may_dispatch (هذا, &u.format.v))) أرجع c->no_dispatch_return_قيمة ();
    TRACE_DISPATCH (هذا, u.format.v);
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    حالة 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    حالة 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    افتراضي:return_trace (c->default_return_قيمة ());
    }
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    بدل (u.format.v) {
    حالة 1:
    حالة 2:
      أرجع;
    حالة 3:
      hb_barrier ();
      u.format3.collect_variation_indices (c);
      أرجع;
    افتراضي: أرجع;
    }
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (!u.format.v.sanitize (c)) return_trace (خطأ);
    hb_barrier ();
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    حالة 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    حالة 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    افتراضي:return_trace (true);
    }
  }

  محمي:
  اتحاد {
  بنية { HBUINT16 v; }	format;		/* Format identifier */
  Caret_قيمة_Format1	format1;
  Caret_قيمة_Format2	format2;
  Caret_قيمة_Format3	format3;
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
			   unsigned             *caret_عدد /* IN/OUT */,
			   hb_position_t        *caret_array /* OUT */) ثابت
  {
    إذا (caret_عدد && caret_array)
    {
      + carets.as_array ().sub_array (start_offset, caret_عدد)
      | hb_map (hb_add (هذا))
      | hb_map ([&] (ثابت Caret_قيمة &value) { أرجع value.get_caret_قيمة (font, direction, glyph_id, var_store); })
      | hb_sink (hb_array (caret_array, *caret_عدد))
      ;
    }

    أرجع carets.len;
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    auto *out = c->serializer->start_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (out))) return_trace (خطأ);

    + hb_iter (carets)
    | hb_apply (subset_offset_array (c, out->carets, هذا))
    ;

    return_trace (منطقي (out->carets));
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    لكل (ثابت Offset16To<Caret_قيمة>& offset : carets.iter ())
      (هذا+offset).collect_variation_indices (c);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (carets.sanitize (c, هذا));
  }

  محمي:
  Array16OfOffset16To<Caret_قيمة>
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
			       unsigned صحيح *caret_عدد /* IN/OUT */,
			       hb_position_t *caret_array /* OUT */) ثابت
  {
    unsigned صحيح index = (هذا+coverage).get_coverage (glyph_id);
    إذا (index == NOT_COVERED)
    {
      إذا (caret_عدد)
	*caret_عدد = 0;
      أرجع 0;
    }
    ثابت LigGlyph &lig_glyph = هذا+ligGlyph[index];
    أرجع lig_glyph.get_lig_carets (font, direction, glyph_id, var_store, start_offset, caret_عدد, caret_array);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (out))) return_trace (خطأ);

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
  { أرجع (هذا+coverage[set_index]).get_coverage (glyph_id) != NOT_COVERED; }

  فراغ collect_used_mark_sets (ثابت hb_set_t& glyph_set,
                               hb_set_t& used_mark_sets /* OUT */) ثابت
  {
    unsigned i = 0;
    لكل (ثابت auto &offset : coverage)
     {
       ثابت auto &cov = هذا+offset;
       إذا (cov.intersects (&glyph_set))
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
    إذا (unlikely (!c->serializer->extend_min (out))) return_trace (خطأ);
    out->format = format;

    منطقي ret = true;
    لكل (ثابت Offset32To<Coverage>& offset : coverage.iter ())
    {
      auto snap = c->serializer->snapshot ();
      auto *o = out->coverage.serialize_append (c->serializer);
      إذا (unlikely (!o))
      {
	ret = خطأ;
	اكسر;
      }

      //skip empty coverage
      c->serializer->push ();
      منطقي res = خطأ;
      إذا (offset) res = c->dispatch (هذا+offset);
      إذا (!res)
      {
        c->serializer->اسحب_discard ();
        c->serializer->revert (snap);
        (out->coverage.len)--;
        استمر;
      }
      c->serializer->add_link (*o, c->serializer->اسحب_pack ());
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
    حالة 1: hb_barrier (); أرجع u.format1.covers (set_index, glyph_id);
    افتراضي:أرجع خطأ;
    }
  }

  template <typename set_t>
  فراغ collect_coverage (hb_vector_t<set_t> &sets) ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); u.format1.collect_coverage (sets); أرجع;
    افتراضي:أرجع;
    }
  }

  فراغ collect_used_mark_sets (ثابت hb_set_t& glyph_set,
                               hb_set_t& used_mark_sets /* OUT */) ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); u.format1.collect_used_mark_sets (glyph_set, used_mark_sets); أرجع;
    افتراضي:أرجع;
    }
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (u.format1.subset (c));
    افتراضي:return_trace (خطأ);
    }
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (!u.format.v.sanitize (c)) return_trace (خطأ);
    hb_barrier ();
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    افتراضي:return_trace (true);
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
    إذا (version.major != 1) أرجع version.static_size;

    أرجع min_size +
	   (version.to_int () >= 0x00010002u ? markGlyphSetsDef.static_size : 0) +
	   (version.to_int () >= 0x00010003u ? varStore.static_size : 0) +
#ifndef HB_NO_BEYOND_64K
	   (version.to_int () >= 0x00010004u ? 5 * Offset32::static_size : 0)
#وإلا
	   0
#endif
	   ;
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!version.sanitize (c) ||
		  version.major != 1 ||
		  !c->check_range (هذا, get_size ())))
      return_trace (خطأ);
    hb_barrier ();

    منطقي glyph_class_def_ok = glyphClassDef.sanitize (c, هذا);
    منطقي attach_list_ok = attachList.sanitize (c, هذا);
    منطقي lig_caret_list_ok = ligCaretList.sanitize (c, هذا);
    منطقي mark_attach_class_def_ok = markAttachClassDef.sanitize (c, هذا);
    منطقي mark_glyph_sets_def_ok = version.to_int () < 0x00010002u ||
				  markGlyphSetsDef.sanitize (c, هذا);

#ifndef HB_NO_BEYOND_64K
    إذا (version.to_int () >= 0x00010004u)
    {
      إذا (glyphClassDef2) glyph_class_def_ok = glyphClassDef2.sanitize (c, هذا);
      إذا (attachList2) attach_list_ok = attachList2.sanitize (c, هذا);
      إذا (ligCaretList2) lig_caret_list_ok = ligCaretList2.sanitize (c, هذا);
      إذا (markAttachClassDef2) mark_attach_class_def_ok = markAttachClassDef2.sanitize (c, هذا);
      إذا (markGlyphSetsDef2) mark_glyph_sets_def_ok = markGlyphSetsDef2.sanitize (c, هذا);
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
    إذا (version.major != 1) return_trace (خطأ);

    auto *out = c->serializer->start_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (out))) return_trace (خطأ);

    // Push var store first (if it's needed) so that it's last in the
    // serialization order. Some font consumers assume that varstore runs to
    // the end of the GDEF table.
    // See: https://github.com/harfbuzz/harfbuzz/issues/4636
    auto snapshot_version0 = c->serializer->snapshot ();
    إذا (unlikely (version.to_int () >= 0x00010002u && hb_barrier () &&
		  !c->serializer->embed (markGlyphSetsDef)))
      return_trace (خطأ);

    auto snapshot_version2 = c->serializer->snapshot ();
    إذا (unlikely (version.to_int () >= 0x00010003u && hb_barrier () &&
		  !c->serializer->embed (varStore)))
      return_trace (خطأ);

    منطقي use_v14 = خطأ;
#ifndef HB_NO_BEYOND_64K
    use_v14 = version.to_int () >= 0x00010004u &&
	      (glyphClassDef2 || attachList2 || ligCaretList2 ||
	       markAttachClassDef2 || markGlyphSetsDef2);
    إذا (unlikely (version.to_int () >= 0x00010004u && hb_barrier () &&
		  !c->serializer->extend_size (out, get_size ())))
      return_trace (خطأ);
#endif

    منطقي subset_varstore = خطأ;
    unsigned varstore_index = (unsigned) -1;
    إذا (version.to_int () >= 0x00010003u && hb_barrier ())
    {
      إذا (c->plan->all_axes_pinned)
	out->varStore = 0;
      وإلا إذا (c->plan->normalized_coords)
      {
	إذا (varStore)
	{
	  item_variations_t item_vars;
	  إذا (unlikely (!c->serializer->check_success (
                  item_vars.instantiate (هذا+varStore, c->plan, true, true,
                                          c->plan->gdef_varstore_inner_maps.as_array (), true))))
            return_trace (خطأ);
	  subset_varstore = out->varStore.serialize_serialize (c->serializer,
							     item_vars.has_long_word (),
							     c->plan->axis_tags,
							     item_vars.get_region_list (),
							     item_vars.get_vardata_encodings ());
	  varstore_index = c->serializer->last_added_child_index();
	  remap_varidx_after_instantiation (item_vars.get_varidx_map (),
					    c->plan->layout_variation_idx_delta_map);
          remap_varidx_after_instantiation (item_vars.get_varidx_map (),
                                            c->plan->layout_condition_idx_قيمة_map);
	}
      }
      وإلا
      {
	subset_varstore = out->varStore.serialize_subset (c, varStore, هذا,
							  c->plan->gdef_varstore_inner_maps.as_array ());
	varstore_index = c->serializer->last_added_child_index();
      }
    }

    out->version.major = version.major;
    out->version.minor = version.minor;

    إذا (!subset_varstore && version.to_int () >= 0x00010002u && !use_v14) {
      c->serializer->revert (snapshot_version2);
    }
    إذا (!subset_varstore)
      out->varStore = 0;
    وإلا
      c->plan->has_gdef_varstore = true;

    منطقي subset_markglyphsetsdef = خطأ;
    إذا (version.to_int () >= 0x00010002u && hb_barrier ())
    {
#ifndef HB_NO_BEYOND_64K
      إذا (use_v14 && markGlyphSetsDef2)
      {
	out->markGlyphSetsDef = 0;
	subset_markglyphsetsdef = serialize_subset_offset (c, out->markGlyphSetsDef2,
							   markGlyphSetsDef2, هذا);
      }
      وإلا
#endif
      {
#ifndef HB_NO_BEYOND_64K
	إذا (use_v14) out->markGlyphSetsDef2 = 0;
#endif
	subset_markglyphsetsdef = out->markGlyphSetsDef.serialize_subset (c, markGlyphSetsDef, هذا);
      }
    }

    إذا (use_v14)
    {
      out->version.minor = 4;
    } وإلا إذا (subset_varstore) {
      out->version.minor = 3;
    } وإلا إذا (subset_markglyphsetsdef) {
      out->version.minor = 2;
    } وإلا  {
      out->version.minor = 0;
      c->serializer->revert (snapshot_version0);
    }

    منطقي subset_glyphclassdef = خطأ;
    منطقي subset_attachlist = خطأ;
    منطقي subset_markattachclassdef = خطأ;
    منطقي subset_ligcaretlist = خطأ;

#ifndef HB_NO_BEYOND_64K
    إذا (use_v14 && glyphClassDef2)
    {
      out->glyphClassDef = 0;
      subset_glyphclassdef = serialize_subset_offset (c, out->glyphClassDef2,
						      glyphClassDef2, هذا, nullptr, خطأ, true);
    }
    وإلا
#endif
    {
#ifndef HB_NO_BEYOND_64K
      إذا (use_v14) out->glyphClassDef2 = 0;
#endif
      subset_glyphclassdef = out->glyphClassDef.serialize_subset (c, glyphClassDef, هذا, nullptr, خطأ, true);
    }

#ifndef HB_NO_BEYOND_64K
    إذا (use_v14 && attachList2)
    {
      out->attachList = 0;
      subset_attachlist = serialize_subset_offset (c, out->attachList2, attachList2, هذا);
    }
    وإلا
#endif
    {
#ifndef HB_NO_BEYOND_64K
      إذا (use_v14) out->attachList2 = 0;
#endif
      subset_attachlist = out->attachList.serialize_subset (c, attachList, هذا);
    }

#ifndef HB_NO_BEYOND_64K
    إذا (use_v14 && markAttachClassDef2)
    {
      out->markAttachClassDef = 0;
      subset_markattachclassdef = serialize_subset_offset (c, out->markAttachClassDef2,
							   markAttachClassDef2, هذا, nullptr, خطأ, true);
    }
    وإلا
#endif
    {
#ifndef HB_NO_BEYOND_64K
      إذا (use_v14) out->markAttachClassDef2 = 0;
#endif
      subset_markattachclassdef = out->markAttachClassDef.serialize_subset (c, markAttachClassDef,
									   هذا, nullptr, خطأ, true);
    }

#ifndef HB_NO_BEYOND_64K
    إذا (use_v14 && ligCaretList2)
    {
      out->ligCaretList = 0;
      subset_ligcaretlist = serialize_subset_offset (c, out->ligCaretList2, ligCaretList2, هذا);
    }
    وإلا
#endif
    {
#ifndef HB_NO_BEYOND_64K
      إذا (use_v14) out->ligCaretList2 = 0;
#endif
      subset_ligcaretlist = out->ligCaretList.serialize_subset (c, ligCaretList, هذا);
    }

    إذا (subset_varstore && varstore_index != (unsigned) -1) {
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
    إذا (version.to_int () >= 0x00010004u && glyphClassDef2) أرجع true;
#endif
    أرجع glyphClassDef != 0;
  }
  ثابت ClassDef &get_glyph_class_def () ثابت
  {
#ifndef HB_NO_BEYOND_64K
    إذا (version.to_int () >= 0x00010004u && glyphClassDef2) أرجع هذا+glyphClassDef2;
#endif
    أرجع هذا+glyphClassDef;
  }
  منطقي has_attach_list () ثابت
  {
#ifndef HB_NO_BEYOND_64K
    إذا (version.to_int () >= 0x00010004u && attachList2) أرجع true;
#endif
    أرجع attachList != 0;
  }
  ثابت AttachList &get_attach_list () ثابت
  {
#ifndef HB_NO_BEYOND_64K
    إذا (version.to_int () >= 0x00010004u && attachList2) أرجع هذا+attachList2;
#endif
    أرجع هذا+attachList;
  }
  منطقي has_lig_carets () ثابت
  {
#ifndef HB_NO_BEYOND_64K
    إذا (version.to_int () >= 0x00010004u && ligCaretList2) أرجع true;
#endif
    أرجع ligCaretList != 0;
  }
  منطقي has_mark_attachment_types () ثابت
  {
#ifndef HB_NO_BEYOND_64K
    إذا (version.to_int () >= 0x00010004u && markAttachClassDef2) أرجع true;
#endif
    أرجع markAttachClassDef != 0;
  }
  ثابت ClassDef &get_mark_attach_class_def () ثابت
  {
#ifndef HB_NO_BEYOND_64K
    إذا (version.to_int () >= 0x00010004u && markAttachClassDef2) أرجع هذا+markAttachClassDef2;
#endif
    أرجع هذا+markAttachClassDef;
  }
  منطقي has_mark_glyph_sets () ثابت
  {
    إذا (version.to_int () < 0x00010002u) أرجع خطأ;
#ifndef HB_NO_BEYOND_64K
    إذا (version.to_int () >= 0x00010004u && markGlyphSetsDef2) أرجع true;
#endif
    أرجع markGlyphSetsDef != 0;
  }
  ثابت MarkGlyphSets &get_mark_glyph_sets () ثابت
  {
    إذا (version.to_int () < 0x00010002u) أرجع Null(MarkGlyphSets);
#ifndef HB_NO_BEYOND_64K
    إذا (version.to_int () >= 0x00010004u && markGlyphSetsDef2) أرجع هذا+markGlyphSetsDef2;
#endif
    أرجع هذا+markGlyphSetsDef;
  }
  منطقي has_var_store () ثابت
  {
    أرجع version.to_int () >= 0x00010003u && hb_barrier () && varStore != 0;
  }
  ثابت ItemVariationStore &get_var_store () ثابت
  {
    أرجع version.to_int () >= 0x00010003u && hb_barrier () ? هذا+varStore : Null(ItemVariationStore);
  }


  منطقي has_data () ثابت { أرجع version.to_int (); }
  unsigned صحيح get_glyph_class (hb_codepoint_t glyph) ثابت
  { أرجع get_glyph_class_def ().get_class (glyph); }
  فراغ get_glyphs_in_class (unsigned صحيح klass, hb_set_t *glyphs) ثابت
  { get_glyph_class_def ().collect_class (glyphs, klass); }

  unsigned صحيح get_mark_attachment_type (hb_codepoint_t glyph) ثابت
  { أرجع get_mark_attach_class_def ().get_class (glyph); }

  unsigned صحيح get_attach_points (hb_codepoint_t glyph_id,
				  unsigned صحيح start_offset,
				  unsigned صحيح *point_عدد /* IN/OUT */,
				  unsigned صحيح *point_array /* OUT */) ثابت
  { أرجع get_attach_list ().get_attach_points (glyph_id, start_offset, point_عدد, point_array); }

  unsigned صحيح get_lig_carets (hb_font_t *font,
			       hb_direction_t direction,
			       hb_codepoint_t glyph_id,
			       unsigned صحيح start_offset,
			       unsigned صحيح *caret_عدد /* IN/OUT */,
			       hb_position_t *caret_array /* OUT */) ثابت
  {
#ifndef HB_NO_BEYOND_64K
    إذا (version.to_int () >= 0x00010004u && ligCaretList2)
      أرجع (هذا+ligCaretList2).get_lig_carets (font, direction, glyph_id, get_var_store (),
						  start_offset, caret_عدد, caret_array);
#endif
    أرجع (هذا+ligCaretList).get_lig_carets (font, direction, glyph_id, get_var_store (),
					       start_offset, caret_عدد, caret_array);
  }

  منطقي mark_set_covers (unsigned صحيح set_index, hb_codepoint_t glyph_id) ثابت
  { أرجع get_mark_glyph_sets ().covers (set_index, glyph_id); }

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
    افتراضي:			أرجع HB_OT_LAYOUT_GLYPH_CLASS_UNCLASSIFIED;
    حالة BaseGlyph:		أرجع HB_OT_LAYOUT_GLYPH_PROPS_BASE_GLYPH;
    حالة LigatureGlyph:		أرجع HB_OT_LAYOUT_GLYPH_PROPS_LIGATURE;
    حالة MarkGlyph:
	  klass = get_mark_attachment_type (glyph);
	  أرجع HB_OT_LAYOUT_GLYPH_PROPS_MARK | (klass << 8);
    }
  }

  HB_INTERNAL منطقي is_blocklisted (hb_blob_t *blob,
				   hb_face_t *face) ثابت;

  بنية accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      table = hb_sanitize_context_t ().reference_table<GDEF> (face);
      إذا (unlikely (table->is_blocklisted (table.get_blob (), face)))
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
	إذا (!sets.in_error () &&
	    mark_glyph_set_bitmaps.resize (sets.length))
	  لكل (unsigned i = 0; i < sets.length; i++)
	  {
	    auto &bitmap = mark_glyph_set_bitmaps.arrayZ[i];
	    bitmap.bias = HB_SET_قيمة_INVALID;
	    ثابت auto &set = sets.arrayZ[i];
	    إذا (unlikely (set.in_error ())) استمر;
	    hb_codepoint_t min_gid = HB_SET_قيمة_INVALID;
	    hb_codepoint_t max_gid = HB_SET_قيمة_INVALID;
	    hb_set_next (&set, &min_gid);
	    hb_set_previous (&set, &max_gid);
	    إذا (max_gid == HB_SET_قيمة_INVALID)
	      استمر;
	    إذا (max_gid - min_gid >= hb_bit_page_t::PAGE_BITS)
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
      إذا (glyph_props_cache.get (glyph, &v))
        أرجع v;
#endif

      v = table->get_glyph_props (glyph);

#ifndef HB_NO_GDEF_CACHE
      إذا (likely (table.get_blob ())) // Don't try setting if we are the null instance!
	glyph_props_cache.set (glyph, v);
#endif

      أرجع v;

    }

    HB_ALWAYS_INLINE
    منطقي mark_set_covers (unsigned صحيح set_index, hb_codepoint_t glyph_id) ثابت
    {
#ifndef HB_NO_GDEF_CACHE
      // We can access arrayZ directly because of sanitize_lookup_props() guarantee.
      ثابت auto &bitmap = mark_glyph_set_bitmaps.arrayZ[set_index];
      إذا (likely (bitmap.bias != HB_SET_قيمة_INVALID))
      {
	hb_codepoint_t biased = glyph_id - bitmap.bias;
	أرجع biased < hb_bit_page_t::PAGE_BITS &&
	       bitmap.u.page.get (biased);
      }
      أرجع mark_set_covers_slow (set_index, glyph_id);
#وإلا
      أرجع table->mark_set_covers (set_index, glyph_id);
#endif
    }

#ifndef HB_NO_GDEF_CACHE
    HB_NEVER_INLINE
    منطقي mark_set_covers_slow (unsigned صحيح set_index, hb_codepoint_t glyph_id) ثابت
    {
      ثابت auto &bitmap = mark_glyph_set_bitmaps.arrayZ[set_index];
      إذا (!bitmap.u.digest.may_have (glyph_id))
	أرجع خطأ;
      أرجع table->mark_set_covers (set_index, glyph_id);
    }
#endif

    unsigned sanitize_lookup_props (unsigned lookup_props) ثابت
    {
#ifndef HB_NO_GDEF_CACHE
      إذا (lookup_props & LookupFlag::UseMarkFilteringSet &&
	  (lookup_props >> 16) >= mark_glyph_set_bitmaps.length)
      {
        // Invalid mark filtering set index; unset the flag.
	lookup_props &= ~LookupFlag::UseMarkFilteringSet;
      }
#endif
      أرجع lookup_props;
    }

    hb_blob_ptr_t<GDEF> table;
#ifndef HB_NO_GDEF_CACHE
    بنية mark_glyph_set_bitmap_t
    {
      /* Gid of the first bit (not page-aligned) for an exact one-page
       * set, or HB_SET_VALUE_INVALID; then digest is a pre-filter for
       * the table.  The all-zeros state is valid for both members. */
      hb_codepoint_t bias = HB_SET_قيمة_INVALID;
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
    إذا (version.to_int () >= 0x00010004u && ligCaretList2)
    {
      (هذا+ligCaretList2).collect_variation_indices (c);
      أرجع;
    }
#endif
    (هذا+ligCaretList).collect_variation_indices (c);
  }

  محمي:
  template <typename مفتاح>
  ساكن فراغ remap_varidx_after_instantiation (ثابت hb_map_t& varidx_map,
                                                hb_hashmap_t<مفتاح, hb_pair_t<unsigned, صحيح>>& layout_variation_idx_delta_map /* IN/OUT */)
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
      إذا (varidx_map.has (varidx, &new_varidx))
	_.second.first = *new_varidx;
      وإلا
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
    إذا (in.is_null ()) أرجع خطأ;

    auto *s = c->serializer;
    s->push ();
    منطقي ret = c->dispatch (base+in, std::forward<Ts> (ds)...);
    إذا (ret) s->add_link (out, s->اسحب_pack ());
    وإلا s->اسحب_discard ();
    أرجع ret;
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
