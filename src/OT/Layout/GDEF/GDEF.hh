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

#که_نه_تعریف_شوی OT_LAYOUT_GDEF_GDEF_HH
#تعریف_کړه OT_LAYOUT_GDEF_GDEF_HH

#ضمن "../../../hb-ot-var-common.hh"

#ضمن "../../../hb-font.hh"
#ضمن "../../../hb-cache.hh"


نطاق OT {


/*
 * Attachment List Table
 */

/* Array of contour point indices--in increasing numerical order */
بنية Attach_ټکی : Array16_د<HBUINT16>
{
  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خپلکاری *بهر = c->serializer->پیل_embed (*هذا);
    return_trace (بهر->serialize (c->serializer, + iter ()));
  }
};

/* AttachList is referenced by both GDEF attachListOffset and
 * attachListOffset2. The v1.4 top-level offset widens, but the
 * AttachList table itself keeps Offset16 internals. */
بنية AttachList
{
  بینښه صحيح get_attach_points (hb_codepoint_t glyph_id,
				  بینښه صحيح پیل_دننه_واټن,
				  بینښه صحيح *ټکی_شمېر /* IN/OUT */,
				  بینښه صحيح *ټکی_array /*بهر*/) ثابت
  {
    بینښه صحيح شاخص_2 = (هذا+coverage).get_coverage (glyph_id);
    إذا (شاخص_2 == NOT_COVERED)
    {
      إذا (ټکی_شمېر)
	*ټکی_شمېر = 0;
      أرجع 0;
    }

    ثابت Attach_ټکی &points = هذا+attach_ټکی[شاخص_2];

    إذا (ټکی_شمېر && ټکی_array)
    {
      + points.as_array ().sub_array (پیل_دننه_واټن, ټکی_شمېر)
      | hb_sink (hb_array (ټکی_array, *ټکی_شمېر))
      ;
    }

    أرجع points.len;
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_سېټ_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خپلکاری *بهر = c->serializer->پیل_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (بهر))) return_trace (خطأ);

    hb_sorted_ویکټور_t<hb_codepoint_t> new_coverage;
    + hb_zip (هذا+coverage, attach_ټکی)
    | hb_filter (glyphset, hb_لومړی)
    | hb_filter (subset_دننه_واټن_array (c, بهر->attach_ټکی, هذا), hb_دوهم)
    | hb_map (hb_لومړی)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;
    بهر->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (منطقي (new_coverage));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (coverage.sanitize (c, هذا) && attach_ټکی.sanitize (c, هذا));
  }

  محمي:
  Offset16_منزل<Coverage>
		coverage;		/* Offset to Coverage table -- from
					 * beginning of AttachList table */
  Array16_د_Offset16_منزل<Attach_ټکی>
		attach_ټکی;		/* Array of AttachPoint tables
					 * in Coverage Index order */
  عام:
  DEFINE_اندازه_ARRAY (4, attach_ټکی);
};

/*
 * Ligature Caret Table
 */

بنية Caret_ارزښت_Format1
{
  ملګری بنية Caret_ارزښت;
  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خپلکاری *بهر = c->serializer->embed (هذا);
    إذا (unlikely (!بهر)) return_trace (خطأ);
    return_trace (ریښتیا);
  }

  خاص:
  hb_position_t get_caret_ارزښت (hb_font_t *font, hb_direction_t direction) ثابت
  {
    أرجع HB_DIRECTION_IS_HORIZONTAL (direction) ? font->em_scale_x (coordinate) : font->em_scale_y (coordinate);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  محمي:
  HBUINT16	caret_ارزښت_Format;	/* Format identifier--format = 1 */
  FWORD		coordinate;		/* X or Y value, in design units */
  عام:
  DEFINE_اندازه_STATIC (4);
};

بنية Caret_ارزښت_Format2
{
  ملګری بنية Caret_ارزښت;
  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خپلکاری *بهر = c->serializer->embed (هذا);
    إذا (unlikely (!بهر)) return_trace (خطأ);
    return_trace (ریښتیا);
  }

  خاص:
  hb_position_t get_caret_ارزښت (hb_font_t *font, hb_direction_t direction, hb_codepoint_t glyph_id) ثابت
  {
    hb_position_t x, y;
    font->get_glyph_contour_ټکی_for_origin (glyph_id, caret_ارزښت_ټکی, direction, &x, &y);
    أرجع HB_DIRECTION_IS_HORIZONTAL (direction) ? x : y;
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  محمي:
  HBUINT16	caret_ارزښت_Format;	/* Format identifier--format = 2 */
  HBUINT16	caret_ارزښت_ټکی;	/* Contour point index on glyph */
  عام:
  DEFINE_اندازه_STATIC (4);
};

بنية Caret_ارزښت_Format3
{
  ملګری بنية Caret_ارزښت;

  hb_position_t get_caret_ارزښت (hb_font_t *font, hb_direction_t direction,
				 ثابت توکی_Variation_Store &var_store) ثابت
  {
    أرجع HB_DIRECTION_IS_HORIZONTAL (direction) ?
	   hb_saturate_جمع_کړه (font->em_scale_x (coordinate), (هذا+device_جدول).get_x_delta (font, var_store)) :
	   hb_saturate_جمع_کړه (font->em_scale_y (coordinate), (هذا+device_جدول).get_y_delta (font, var_store));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خپلکاری *بهر = c->serializer->پیل_embed (*هذا);
    إذا (!c->serializer->embed (caret_ارزښت_Format)) return_trace (خطأ);
    إذا (!c->serializer->embed (coordinate)) return_trace (خطأ);

    بینښه varidx = (هذا+device_جدول).get_variation_شاخص ();
    hb_pair_t<بینښه, صحيح> *new_varidx_delta;
    إذا (c->plan->layout_variation_idx_delta_map.has (varidx, &new_varidx_delta)) {
      uint32_t new_varidx = hb_لومړی (*new_varidx_delta);
      صحيح delta = hb_دوهم (*new_varidx_delta);
      إذا (delta != 0)
      {
        إذا (!c->serializer->check_ټاکل (بهر->coordinate, coordinate + delta, HB_SERIALIZE_ERROR_INT_ډکېدنه))
          return_trace (خطأ);
      }

      إذا (new_varidx == HB_OT_LAYOUT_NO_VARIATIONS_شاخص)
        return_trace (c->serializer->check_ټاکل (بهر->caret_ارزښت_Format, 1, HB_SERIALIZE_ERROR_INT_ډکېدنه));
    }

    إذا (!c->serializer->embed (device_جدول))
      return_trace (خطأ);

    return_trace (بهر->device_جدول.serialize_copy (c->serializer, device_جدول, هذا, c->serializer->منزل_bias (بهر),
						   hb_serialize_context_t::سر, &c->plan->layout_variation_idx_delta_map));
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  { (هذا+device_جدول).collect_variation_indices (c); }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && device_جدول.sanitize (c, هذا));
  }

  محمي:
  HBUINT16	caret_ارزښت_Format;	/* Format identifier--format = 3 */
  FWORD		coordinate;		/* X or Y value, in design units */
  Offset16_منزل<Device>
		device_جدول;		/* Offset to Device table for X or Y
					 * value--from beginning of CaretValue
					 * table */
  عام:
  DEFINE_اندازه_STATIC (6);
};

بنية Caret_ارزښت
{
  hb_position_t get_caret_ارزښت (hb_font_t *font,
				 hb_direction_t direction,
				 hb_codepoint_t glyph_id,
				 ثابت توکی_Variation_Store &var_store) ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.get_caret_ارزښت (font, direction);
    حالة 2: hb_barrier (); أرجع u.format2.get_caret_ارزښت (font, direction, glyph_id);
    حالة 3: hb_barrier (); أرجع u.format3.get_caret_ارزښت (font, direction, var_store);
    افتراضي:أرجع 0;
    }
  }

  کينډۍ <ډول_نوم context_t, ډول_نوم ...Ts>
  ډول_نوم context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    إذا (unlikely (!c->may_dispatch (هذا, &u.format.v))) أرجع c->no_dispatch_return_ارزښت ();
    TRACE_DISPATCH (هذا, u.format.v);
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    حالة 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    حالة 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    افتراضي:return_trace (c->default_return_ارزښت ());
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
    افتراضي:return_trace (ریښتیا);
    }
  }

  محمي:
  اتحاد {
  بنية { HBUINT16 v; }	format;		/* Format identifier */
  Caret_ارزښت_Format1	format1;
  Caret_ارزښت_Format2	format2;
  Caret_ارزښت_Format3	format3;
  } u;
  عام:
  DEFINE_اندازه_UNION (2, format.v);
};

بنية LigGlyph
{
  بینښه get_lig_carets (hb_font_t            *font,
			   hb_direction_t        direction,
			   hb_codepoint_t        glyph_id,
			   ثابت توکی_Variation_Store &var_store,
			   بینښه              پیل_دننه_واټن,
			   بینښه             *caret_شمېر /* IN/OUT */,
			   hb_position_t        *caret_array /*بهر*/) ثابت
  {
    إذا (caret_شمېر && caret_array)
    {
      + carets.as_array ().sub_array (پیل_دننه_واټن, caret_شمېر)
      | hb_map (hb_جمع_کړه (هذا))
      | hb_map ([&] (ثابت Caret_ارزښت &value) { أرجع value.get_caret_ارزښت (font, direction, glyph_id, var_store); })
      | hb_sink (hb_array (caret_array, *caret_شمېر))
      ;
    }

    أرجع carets.len;
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خپلکاری *بهر = c->serializer->پیل_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (بهر))) return_trace (خطأ);

    + hb_iter (carets)
    | hb_apply (subset_دننه_واټن_array (c, بهر->carets, هذا))
    ;

    return_trace (منطقي (بهر->carets));
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    لكل (ثابت Offset16_منزل<Caret_ارزښت>& دننه_واټن : carets.iter ())
      (هذا+دننه_واټن).collect_variation_indices (c);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (carets.sanitize (c, هذا));
  }

  محمي:
  Array16_د_Offset16_منزل<Caret_ارزښت>
		carets;			/* Offset array of CaretValue tables
					 * --from beginning of LigGlyph table
					 * --in increasing coordinate order */
  عام:
  DEFINE_اندازه_ARRAY (2, carets);
};

/* LigCaretList2 widens the LigCaretList coverage, count, and LigGlyph
 * offsets. The LigGlyph table itself keeps Offset16 caretValueOffsets. */
کينډۍ <ډول_نوم Types>
بنية LigCaretList
{
  بینښه صحيح get_lig_carets (hb_font_t *font,
			       hb_direction_t direction,
			       hb_codepoint_t glyph_id,
			       ثابت توکی_Variation_Store &var_store,
			       بینښه صحيح پیل_دننه_واټن,
			       بینښه صحيح *caret_شمېر /* IN/OUT */,
			       hb_position_t *caret_array /*بهر*/) ثابت
  {
    بینښه صحيح شاخص_2 = (هذا+coverage).get_coverage (glyph_id);
    إذا (شاخص_2 == NOT_COVERED)
    {
      إذا (caret_شمېر)
	*caret_شمېر = 0;
      أرجع 0;
    }
    ثابت LigGlyph &lig_glyph = هذا+ligGlyph[شاخص_2];
    أرجع lig_glyph.get_lig_carets (font, direction, glyph_id, var_store, پیل_دننه_واټن, caret_شمېر, caret_array);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_سېټ_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خپلکاری *بهر = c->serializer->پیل_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (بهر))) return_trace (خطأ);

    hb_sorted_ویکټور_t<hb_codepoint_t> new_coverage;
    + hb_zip (هذا+coverage, ligGlyph)
    | hb_filter (glyphset, hb_لومړی)
    | hb_filter (subset_دننه_واټن_array (c, بهر->ligGlyph, هذا), hb_دوهم)
    | hb_map (hb_لومړی)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;
    بهر->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (منطقي (new_coverage));
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    + hb_zip (هذا+coverage, ligGlyph)
    | hb_filter (c->glyph_سېټ, hb_لومړی)
    | hb_map (hb_دوهم)
    | hb_map (hb_جمع_کړه (هذا))
    | hb_apply ([c] (ثابت LigGlyph& _) { _.collect_variation_indices (c); })
    ;
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (coverage.sanitize (c, هذا) && ligGlyph.sanitize (c, هذا));
  }

  محمي:
  ډول_نوم Types::کينډۍ LOffset_منزل<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of LigCaretList table */
  ډول_نوم Types::کينډۍ Array_د<ډول_نوم Types::کينډۍ دننه_واټن_منزل<LigGlyph>>
		ligGlyph;		/* Array of LigGlyph tables
					 * in Coverage Index order */
  عام:
  DEFINE_اندازه_ARRAY (Types::LOffset::static_اندازه + Types::HBUINT::static_اندازه, ligGlyph);
};

#که_نه_تعریف_شوی HB_NO_BEYOND_64K
static_assert (LigCaretList<MediumTypes>::min_اندازه ==
	       Offset32::static_اندازه + HBUINT24::static_اندازه, "");
#د_که_پای


بنية MarkGlyphSetsFormat1
{
  منطقي covers (بینښه صحيح سېټ_شاخص, hb_codepoint_t glyph_id) ثابت
  { أرجع (هذا+coverage[سېټ_شاخص]).get_coverage (glyph_id) != NOT_COVERED; }

  فراغ collect_کارول_شوی_mark_sets (ثابت hb_سېټ_t& glyph_سېټ,
                               hb_سېټ_t& کارول_شوی_mark_sets /*بهر*/) ثابت
  {
    بینښه i = 0;
    لكل (ثابت خپلکاری &دننه_واټن : coverage)
     {
       ثابت خپلکاری &cov = هذا+دننه_واټن;
       إذا (cov.intersects (&glyph_سېټ))
         کارول_شوی_mark_sets.جمع_کړه (i);

       i++;
     }
  }

  کينډۍ <ډول_نوم سېټ_t>
  فراغ collect_coverage (hb_ویکټور_t<سېټ_t> &sets) ثابت
  {
     لكل (ثابت خپلکاری &دننه_واټن : coverage)
     {
       ثابت خپلکاری &cov = هذا+دننه_واټن;
       cov.collect_coverage (sets.push ());
     }
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خپلکاری *بهر = c->serializer->پیل_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (بهر))) return_trace (خطأ);
    بهر->format = format;

    منطقي ret = ریښتیا;
    لكل (ثابت Offset32_منزل<Coverage>& دننه_واټن : coverage.iter ())
    {
      خپلکاری snap = c->serializer->snapshot ();
      خپلکاری *o = بهر->coverage.serialize_append (c->serializer);
      إذا (unlikely (!o))
      {
	ret = خطأ;
	اكسر;
      }

      //skip empty coverage
      c->serializer->push ();
      منطقي res = خطأ;
      إذا (دننه_واټن) res = c->dispatch (هذا+دننه_واټن);
      إذا (!res)
      {
        c->serializer->راوباسه_discard ();
        c->serializer->revert (snap);
        (بهر->coverage.len)--;
        استمر;
      }
      c->serializer->جمع_کړه_link (*o, c->serializer->راوباسه_pack ());
    }

    return_trace (ret && بهر->coverage.len);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (coverage.sanitize (c, هذا));
  }

  محمي:
  HBUINT16	format;			/* Format identifier--format = 1 */
  Array16_د<Offset32_منزل<Coverage>>
		coverage;		/* Array of long offsets to mark set
					 * coverage tables */
  عام:
  DEFINE_اندازه_ARRAY (4, coverage);
};

بنية MarkGlyphSets
{
  منطقي covers (بینښه صحيح سېټ_شاخص, hb_codepoint_t glyph_id) ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.covers (سېټ_شاخص, glyph_id);
    افتراضي:أرجع خطأ;
    }
  }

  کينډۍ <ډول_نوم سېټ_t>
  فراغ collect_coverage (hb_ویکټور_t<سېټ_t> &sets) ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); u.format1.collect_coverage (sets); أرجع;
    افتراضي:أرجع;
    }
  }

  فراغ collect_کارول_شوی_mark_sets (ثابت hb_سېټ_t& glyph_سېټ,
                               hb_سېټ_t& کارول_شوی_mark_sets /*بهر*/) ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); u.format1.collect_کارول_شوی_mark_sets (glyph_سېټ, کارول_شوی_mark_sets); أرجع;
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
    افتراضي:return_trace (ریښتیا);
    }
  }

  محمي:
  اتحاد {
  بنية { HBUINT16 v; }	format;		/* Format identifier */
  MarkGlyphSetsFormat1	format1;
  } u;
  عام:
  DEFINE_اندازه_UNION (2, format.v);
};


/*
 * GDEF -- Glyph Definition
 * https://docs.microsoft.com/en-us/typography/opentype/spec/gdef
 */


بنية GDEF
{
  ساكن constexpr hb_نښان_t جدول_نښان = HB_OT_نښان_GDEF;

  تعداد GlyphClasses {
    UnclassifiedGlyph	= 0,
    بنسټ_Glyph		= 1,
    LigatureGlyph	= 2,
    MarkGlyph		= 3,
    ComponentGlyph	= 4
  };

  size_t get_اندازه () ثابت
  {
    إذا (نسخه.major != 1) أرجع نسخه.static_اندازه;

    أرجع min_اندازه +
	   (نسخه.منزل_int () >= 0x00010002u ? markGlyphSetsDef.static_اندازه : 0) +
	   (نسخه.منزل_int () >= 0x00010003u ? varStore.static_اندازه : 0) +
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
	   (نسخه.منزل_int () >= 0x00010004u ? 5 * Offset32::static_اندازه : 0)
#وإلا
	   0
#د_که_پای
	   ;
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!نسخه.sanitize (c) ||
		  نسخه.major != 1 ||
		  !c->check_range (هذا, get_اندازه ())))
      return_trace (خطأ);
    hb_barrier ();

    منطقي glyph_class_def_ok = glyphClassDef.sanitize (c, هذا);
    منطقي attach_list_ok = attachList.sanitize (c, هذا);
    منطقي lig_caret_list_ok = ligCaretList.sanitize (c, هذا);
    منطقي mark_attach_class_def_ok = markAttachClassDef.sanitize (c, هذا);
    منطقي mark_glyph_sets_def_ok = نسخه.منزل_int () < 0x00010002u ||
				  markGlyphSetsDef.sanitize (c, هذا);

#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    إذا (نسخه.منزل_int () >= 0x00010004u)
    {
      إذا (glyphClassDef2) glyph_class_def_ok = glyphClassDef2.sanitize (c, هذا);
      إذا (attachList2) attach_list_ok = attachList2.sanitize (c, هذا);
      إذا (ligCaretList2) lig_caret_list_ok = ligCaretList2.sanitize (c, هذا);
      إذا (markAttachClassDef2) mark_attach_class_def_ok = markAttachClassDef2.sanitize (c, هذا);
      إذا (markGlyphSetsDef2) mark_glyph_sets_def_ok = markGlyphSetsDef2.sanitize (c, هذا);
    }
#د_که_پای

    return_trace (glyph_class_def_ok &&
		  attach_list_ok &&
		  lig_caret_list_ok &&
		  mark_attach_class_def_ok &&
		  mark_glyph_sets_def_ok &&
		  (نسخه.منزل_int () < 0x00010003u || varStore.sanitize (c, هذا)));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (نسخه.major != 1) return_trace (خطأ);

    خپلکاری *بهر = c->serializer->پیل_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (بهر))) return_trace (خطأ);

    // Push var store first (if it's needed) so that it's last in the
    // serialization order. Some font consumers assume that varstore runs to
    // the end of the GDEF table.
    // See: https://github.com/harfbuzz/harfbuzz/issues/4636
    خپلکاری snapshot_version0 = c->serializer->snapshot ();
    إذا (unlikely (نسخه.منزل_int () >= 0x00010002u && hb_barrier () &&
		  !c->serializer->embed (markGlyphSetsDef)))
      return_trace (خطأ);

    خپلکاری snapshot_version2 = c->serializer->snapshot ();
    إذا (unlikely (نسخه.منزل_int () >= 0x00010003u && hb_barrier () &&
		  !c->serializer->embed (varStore)))
      return_trace (خطأ);

    منطقي use_v14 = خطأ;
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    use_v14 = نسخه.منزل_int () >= 0x00010004u &&
	      (glyphClassDef2 || attachList2 || ligCaretList2 ||
	       markAttachClassDef2 || markGlyphSetsDef2);
    إذا (unlikely (نسخه.منزل_int () >= 0x00010004u && hb_barrier () &&
		  !c->serializer->extend_اندازه (بهر, get_اندازه ())))
      return_trace (خطأ);
#د_که_پای

    منطقي subset_varstore = خطأ;
    بینښه varstore_شاخص = (بینښه) -1;
    إذا (نسخه.منزل_int () >= 0x00010003u && hb_barrier ())
    {
      إذا (c->plan->all_axes_pinned)
	بهر->varStore = 0;
      وإلا إذا (c->plan->normalized_coords)
      {
	إذا (varStore)
	{
	  توکی_variations_t توکی_vars;
	  إذا (unlikely (!c->serializer->check_success (
                  توکی_vars.instantiate (هذا+varStore, c->plan, ریښتیا, ریښتیا,
                                          c->plan->gdef_varstore_inner_maps.as_array (), ریښتیا))))
            return_trace (خطأ);
	  subset_varstore = بهر->varStore.serialize_serialize (c->serializer,
							     توکی_vars.has_long_کلمه (),
							     c->plan->axis_نښانونه,
							     توکی_vars.get_region_list (),
							     توکی_vars.get_vardata_encodings ());
	  varstore_شاخص = c->serializer->وروستی_added_ماشوم_شاخص();
	  remap_varidx_after_instantiation (توکی_vars.get_varidx_map (),
					    c->plan->layout_variation_idx_delta_map);
          remap_varidx_after_instantiation (توکی_vars.get_varidx_map (),
                                            c->plan->layout_condition_idx_ارزښت_map);
	}
      }
      وإلا
      {
	subset_varstore = بهر->varStore.serialize_subset (c, varStore, هذا,
							  c->plan->gdef_varstore_inner_maps.as_array ());
	varstore_شاخص = c->serializer->وروستی_added_ماشوم_شاخص();
      }
    }

    بهر->نسخه.major = نسخه.major;
    بهر->نسخه.minor = نسخه.minor;

    إذا (!subset_varstore && نسخه.منزل_int () >= 0x00010002u && !use_v14) {
      c->serializer->revert (snapshot_version2);
    }
    إذا (!subset_varstore)
      بهر->varStore = 0;
    وإلا
      c->plan->has_gdef_varstore = ریښتیا;

    منطقي subset_markglyphsetsdef = خطأ;
    إذا (نسخه.منزل_int () >= 0x00010002u && hb_barrier ())
    {
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
      إذا (use_v14 && markGlyphSetsDef2)
      {
	بهر->markGlyphSetsDef = 0;
	subset_markglyphsetsdef = serialize_subset_دننه_واټن (c, بهر->markGlyphSetsDef2,
							   markGlyphSetsDef2, هذا);
      }
      وإلا
#د_که_پای
      {
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
	إذا (use_v14) بهر->markGlyphSetsDef2 = 0;
#د_که_پای
	subset_markglyphsetsdef = بهر->markGlyphSetsDef.serialize_subset (c, markGlyphSetsDef, هذا);
      }
    }

    إذا (use_v14)
    {
      بهر->نسخه.minor = 4;
    } وإلا إذا (subset_varstore) {
      بهر->نسخه.minor = 3;
    } وإلا إذا (subset_markglyphsetsdef) {
      بهر->نسخه.minor = 2;
    } وإلا  {
      بهر->نسخه.minor = 0;
      c->serializer->revert (snapshot_version0);
    }

    منطقي subset_glyphclassdef = خطأ;
    منطقي subset_attachlist = خطأ;
    منطقي subset_markattachclassdef = خطأ;
    منطقي subset_ligcaretlist = خطأ;

#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    إذا (use_v14 && glyphClassDef2)
    {
      بهر->glyphClassDef = 0;
      subset_glyphclassdef = serialize_subset_دننه_واټن (c, بهر->glyphClassDef2,
						      glyphClassDef2, هذا, تش_اشاره, خطأ, ریښتیا);
    }
    وإلا
#د_که_پای
    {
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
      إذا (use_v14) بهر->glyphClassDef2 = 0;
#د_که_پای
      subset_glyphclassdef = بهر->glyphClassDef.serialize_subset (c, glyphClassDef, هذا, تش_اشاره, خطأ, ریښتیا);
    }

#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    إذا (use_v14 && attachList2)
    {
      بهر->attachList = 0;
      subset_attachlist = serialize_subset_دننه_واټن (c, بهر->attachList2, attachList2, هذا);
    }
    وإلا
#د_که_پای
    {
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
      إذا (use_v14) بهر->attachList2 = 0;
#د_که_پای
      subset_attachlist = بهر->attachList.serialize_subset (c, attachList, هذا);
    }

#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    إذا (use_v14 && markAttachClassDef2)
    {
      بهر->markAttachClassDef = 0;
      subset_markattachclassdef = serialize_subset_دننه_واټن (c, بهر->markAttachClassDef2,
							   markAttachClassDef2, هذا, تش_اشاره, خطأ, ریښتیا);
    }
    وإلا
#د_که_پای
    {
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
      إذا (use_v14) بهر->markAttachClassDef2 = 0;
#د_که_پای
      subset_markattachclassdef = بهر->markAttachClassDef.serialize_subset (c, markAttachClassDef,
									   هذا, تش_اشاره, خطأ, ریښتیا);
    }

#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    إذا (use_v14 && ligCaretList2)
    {
      بهر->ligCaretList = 0;
      subset_ligcaretlist = serialize_subset_دننه_واټن (c, بهر->ligCaretList2, ligCaretList2, هذا);
    }
    وإلا
#د_که_پای
    {
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
      إذا (use_v14) بهر->ligCaretList2 = 0;
#د_که_پای
      subset_ligcaretlist = بهر->ligCaretList.serialize_subset (c, ligCaretList, هذا);
    }

    إذا (subset_varstore && varstore_شاخص != (بینښه) -1) {
      c->serializer->repack_وروستی(varstore_شاخص);
    }

    return_trace (subset_glyphclassdef || subset_attachlist ||
		  subset_ligcaretlist || subset_markattachclassdef ||
		  (بهر->نسخه.منزل_int () >= 0x00010002u && subset_markglyphsetsdef) ||
		  (بهر->نسخه.منزل_int () >= 0x00010003u && subset_varstore));
  }

  منطقي has_glyph_classes () ثابت
  {
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    إذا (نسخه.منزل_int () >= 0x00010004u && glyphClassDef2) أرجع ریښتیا;
#د_که_پای
    أرجع glyphClassDef != 0;
  }
  ثابت ClassDef &get_glyph_class_def () ثابت
  {
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    إذا (نسخه.منزل_int () >= 0x00010004u && glyphClassDef2) أرجع هذا+glyphClassDef2;
#د_که_پای
    أرجع هذا+glyphClassDef;
  }
  منطقي has_attach_list () ثابت
  {
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    إذا (نسخه.منزل_int () >= 0x00010004u && attachList2) أرجع ریښتیا;
#د_که_پای
    أرجع attachList != 0;
  }
  ثابت AttachList &get_attach_list () ثابت
  {
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    إذا (نسخه.منزل_int () >= 0x00010004u && attachList2) أرجع هذا+attachList2;
#د_که_پای
    أرجع هذا+attachList;
  }
  منطقي has_lig_carets () ثابت
  {
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    إذا (نسخه.منزل_int () >= 0x00010004u && ligCaretList2) أرجع ریښتیا;
#د_که_پای
    أرجع ligCaretList != 0;
  }
  منطقي has_mark_attachment_types () ثابت
  {
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    إذا (نسخه.منزل_int () >= 0x00010004u && markAttachClassDef2) أرجع ریښتیا;
#د_که_پای
    أرجع markAttachClassDef != 0;
  }
  ثابت ClassDef &get_mark_attach_class_def () ثابت
  {
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    إذا (نسخه.منزل_int () >= 0x00010004u && markAttachClassDef2) أرجع هذا+markAttachClassDef2;
#د_که_پای
    أرجع هذا+markAttachClassDef;
  }
  منطقي has_mark_glyph_sets () ثابت
  {
    إذا (نسخه.منزل_int () < 0x00010002u) أرجع خطأ;
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    إذا (نسخه.منزل_int () >= 0x00010004u && markGlyphSetsDef2) أرجع ریښتیا;
#د_که_پای
    أرجع markGlyphSetsDef != 0;
  }
  ثابت MarkGlyphSets &get_mark_glyph_sets () ثابت
  {
    إذا (نسخه.منزل_int () < 0x00010002u) أرجع Null(MarkGlyphSets);
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    إذا (نسخه.منزل_int () >= 0x00010004u && markGlyphSetsDef2) أرجع هذا+markGlyphSetsDef2;
#د_که_پای
    أرجع هذا+markGlyphSetsDef;
  }
  منطقي has_var_store () ثابت
  {
    أرجع نسخه.منزل_int () >= 0x00010003u && hb_barrier () && varStore != 0;
  }
  ثابت توکی_Variation_Store &get_var_store () ثابت
  {
    أرجع نسخه.منزل_int () >= 0x00010003u && hb_barrier () ? هذا+varStore : Null(توکی_Variation_Store);
  }


  منطقي has_معلومات () ثابت { أرجع نسخه.منزل_int (); }
  بینښه صحيح get_glyph_class (hb_codepoint_t glyph) ثابت
  { أرجع get_glyph_class_def ().get_class (glyph); }
  فراغ get_glyphs_دننه_class (بینښه صحيح klass, hb_سېټ_t *glyphs) ثابت
  { get_glyph_class_def ().collect_class (glyphs, klass); }

  بینښه صحيح get_mark_attachment_type (hb_codepoint_t glyph) ثابت
  { أرجع get_mark_attach_class_def ().get_class (glyph); }

  بینښه صحيح get_attach_points (hb_codepoint_t glyph_id,
				  بینښه صحيح پیل_دننه_واټن,
				  بینښه صحيح *ټکی_شمېر /* IN/OUT */,
				  بینښه صحيح *ټکی_array /*بهر*/) ثابت
  { أرجع get_attach_list ().get_attach_points (glyph_id, پیل_دننه_واټن, ټکی_شمېر, ټکی_array); }

  بینښه صحيح get_lig_carets (hb_font_t *font,
			       hb_direction_t direction,
			       hb_codepoint_t glyph_id,
			       بینښه صحيح پیل_دننه_واټن,
			       بینښه صحيح *caret_شمېر /* IN/OUT */,
			       hb_position_t *caret_array /*بهر*/) ثابت
  {
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    إذا (نسخه.منزل_int () >= 0x00010004u && ligCaretList2)
      أرجع (هذا+ligCaretList2).get_lig_carets (font, direction, glyph_id, get_var_store (),
						  پیل_دننه_واټن, caret_شمېر, caret_array);
#د_که_پای
    أرجع (هذا+ligCaretList).get_lig_carets (font, direction, glyph_id, get_var_store (),
					       پیل_دننه_واټن, caret_شمېر, caret_array);
  }

  منطقي mark_سېټ_covers (بینښه صحيح سېټ_شاخص, hb_codepoint_t glyph_id) ثابت
  { أرجع get_mark_glyph_sets ().covers (سېټ_شاخص, glyph_id); }

  /* glyph_props is a 16-bit integer where the lower 8-bit have bits representing
   * glyph class and other bits, and high 8-bit the mark attachment type (if any).
   * Not to be confused with lookup_props which is very similar. */
  بینښه صحيح get_glyph_props (hb_codepoint_t glyph) ثابت
  {
    بینښه صحيح klass = get_glyph_class (glyph);

    static_assert (((بینښه صحيح) HB_OT_LAYOUT_GLYPH_PROPS_بنسټ_GLYPH == (بینښه صحيح) LookupFlag::Ignore_بنسټ_Glyphs), "");
    static_assert (((بینښه صحيح) HB_OT_LAYOUT_GLYPH_PROPS_LIGATURE == (بینښه صحيح) LookupFlag::IgnoreLigatures), "");
    static_assert (((بینښه صحيح) HB_OT_LAYOUT_GLYPH_PROPS_MARK == (بینښه صحيح) LookupFlag::IgnoreMarks), "");

    بدل (klass) {
    افتراضي:			أرجع HB_OT_LAYOUT_GLYPH_CLASS_UNCLASSIFIED;
    حالة بنسټ_Glyph:		أرجع HB_OT_LAYOUT_GLYPH_PROPS_بنسټ_GLYPH;
    حالة LigatureGlyph:		أرجع HB_OT_LAYOUT_GLYPH_PROPS_LIGATURE;
    حالة MarkGlyph:
	  klass = get_mark_attachment_type (glyph);
	  أرجع HB_OT_LAYOUT_GLYPH_PROPS_MARK | (klass << 8);
    }
  }

  HB_دننی منطقي is_blocklisted (hb_blob_t *blob,
				   hb_face_t *face) ثابت;

  بنية accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      جدول = hb_sanitize_context_t ().reference_جدول<GDEF> (face);
      إذا (unlikely (جدول->is_blocklisted (جدول.get_blob (), face)))
      {
	hb_blob_destroy (جدول.get_blob ());
	جدول = hb_blob_get_تشه ();
      }

#که_نه_تعریف_شوی HB_NO_GDEF_CACHE
      /* Flatten each mark set whose glyphs span fewer than PAGE_BITS
       * gids into a single bit-page biased at its first glyph, for
       * O(1) exact answers.  Wider sets, or on allocation failure,
       * fall back to the table. */
      {
	hb_ویکټور_t<hb_سېټ_t> sets;
	جدول->get_mark_glyph_sets ().collect_coverage (sets);
	إذا (!sets.دننه_error () &&
	    mark_glyph_سېټ_bitmaps.resize (sets.اوږدتیا))
	  لكل (بینښه i = 0; i < sets.اوږدتیا; i++)
	  {
	    خپلکاری &bitmap = mark_glyph_سېټ_bitmaps.arrayZ[i];
	    bitmap.bias = HB_سېټ_ارزښت_INVALID;
	    ثابت خپلکاری &سېټ = sets.arrayZ[i];
	    إذا (unlikely (سېټ.دننه_error ())) استمر;
	    hb_codepoint_t min_gid = HB_سېټ_ارزښت_INVALID;
	    hb_codepoint_t max_gid = HB_سېټ_ارزښت_INVALID;
	    hb_سېټ_بل (&سېټ, &min_gid);
	    hb_سېټ_پخوانی (&سېټ, &max_gid);
	    إذا (max_gid == HB_سېټ_ارزښت_INVALID)
	      استمر;
	    إذا (max_gid - min_gid >= hb_بټ_پاڼه_t::پاڼه_بټونه)
	    {
	      /* Too wide for one page; a digest pre-filter instead. */
	      لكل (hb_codepoint_t gid : سېټ)
		bitmap.u.digest.جمع_کړه (gid);
	      استمر;
	    }
	    bitmap.bias = min_gid;
	    لكل (hb_codepoint_t gid : سېټ)
	      bitmap.u.پاڼه.جمع_کړه (gid - min_gid);
	  }
      }
#د_که_پای
    }
    ~accelerator_t () { جدول.destroy (); }

    بینښه صحيح get_glyph_props (hb_codepoint_t glyph) ثابت
    {
      بینښه v;

#که_نه_تعریف_شوی HB_NO_GDEF_CACHE
      إذا (glyph_props_cache.get (glyph, &v))
        أرجع v;
#د_که_پای

      v = جدول->get_glyph_props (glyph);

#که_نه_تعریف_شوی HB_NO_GDEF_CACHE
      إذا (likely (جدول.get_blob ())) // Don't try setting if we are the null instance!
	glyph_props_cache.سېټ (glyph, v);
#د_که_پای

      أرجع v;

    }

    HB_ALWAYS_INLINE
    منطقي mark_سېټ_covers (بینښه صحيح سېټ_شاخص, hb_codepoint_t glyph_id) ثابت
    {
#که_نه_تعریف_شوی HB_NO_GDEF_CACHE
      // We can access arrayZ directly because of sanitize_lookup_props() guarantee.
      ثابت خپلکاری &bitmap = mark_glyph_سېټ_bitmaps.arrayZ[سېټ_شاخص];
      إذا (likely (bitmap.bias != HB_سېټ_ارزښت_INVALID))
      {
	hb_codepoint_t biased = glyph_id - bitmap.bias;
	أرجع biased < hb_بټ_پاڼه_t::پاڼه_بټونه &&
	       bitmap.u.پاڼه.get (biased);
      }
      أرجع mark_سېټ_covers_slow (سېټ_شاخص, glyph_id);
#وإلا
      أرجع جدول->mark_سېټ_covers (سېټ_شاخص, glyph_id);
#د_که_پای
    }

#که_نه_تعریف_شوی HB_NO_GDEF_CACHE
    HB_NEVER_INLINE
    منطقي mark_سېټ_covers_slow (بینښه صحيح سېټ_شاخص, hb_codepoint_t glyph_id) ثابت
    {
      ثابت خپلکاری &bitmap = mark_glyph_سېټ_bitmaps.arrayZ[سېټ_شاخص];
      إذا (!bitmap.u.digest.may_have (glyph_id))
	أرجع خطأ;
      أرجع جدول->mark_سېټ_covers (سېټ_شاخص, glyph_id);
    }
#د_که_پای

    بینښه sanitize_lookup_props (بینښه lookup_props) ثابت
    {
#که_نه_تعریف_شوی HB_NO_GDEF_CACHE
      إذا (lookup_props & LookupFlag::Use_Mark_Filtering_سېټ &&
	  (lookup_props >> 16) >= mark_glyph_سېټ_bitmaps.اوږدتیا)
      {
        // Invalid mark filtering set index; unset the flag.
	lookup_props &= ~LookupFlag::Use_Mark_Filtering_سېټ;
      }
#د_که_پای
      أرجع lookup_props;
    }

    hb_blob_ptr_t<GDEF> جدول;
#که_نه_تعریف_شوی HB_NO_GDEF_CACHE
    بنية mark_glyph_سېټ_bitmap_t
    {
      /* Gid of the first bit (not page-aligned) for an exact one-page
       * set, or HB_SET_VALUE_INVALID; then digest is a pre-filter for
       * the table.  The all-zeros state is valid for both members. */
      hb_codepoint_t bias = HB_سېټ_ارزښت_INVALID;
      اتحاد u_t {
	u_t () : پاڼه () {}
	hb_بټ_پاڼه_t پاڼه;
	hb_سېټ_digest_t digest;
      } u;
    };

    hb_ویکټور_t<mark_glyph_سېټ_bitmap_t> mark_glyph_سېټ_bitmaps;
    بدلیدونکی hb_cache_t<21, 3> glyph_props_cache;
    static_assert (حجم (glyph_props_cache) == 512, "");
#د_که_پای
  };

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    إذا (نسخه.منزل_int () >= 0x00010004u && ligCaretList2)
    {
      (هذا+ligCaretList2).collect_variation_indices (c);
      أرجع;
    }
#د_که_پای
    (هذا+ligCaretList).collect_variation_indices (c);
  }

  محمي:
  کينډۍ <ډول_نوم کلی>
  ساكن فراغ remap_varidx_after_instantiation (ثابت hb_map_t& varidx_map,
                                                hb_hashmap_t<کلی, hb_pair_t<بینښه, صحيح>>& layout_variation_idx_delta_map /* IN/OUT */)
  {
    /* varidx_map is empty which means varstore is empty after instantiation,
     * no variations, map all varidx to HB_OT_LAYOUT_NO_VARIATIONS_INDEX.
     * varidx_map doesn't have original varidx, indicating delta row is all
     * zeros, map varidx to HB_OT_LAYOUT_NO_VARIATIONS_INDEX */
    لكل (خپلکاری _ : layout_variation_idx_delta_map.iter_ref ())
    {
      /* old_varidx->(varidx, delta) mapping generated for subsetting, then this
       * varidx is used as key of varidx_map during instantiation */
      uint32_t varidx = _.second.لومړی;
      uint32_t *new_varidx;
      إذا (varidx_map.has (varidx, &new_varidx))
	_.second.لومړی = *new_varidx;
      وإلا
	_.second.لومړی = HB_OT_LAYOUT_NO_VARIATIONS_شاخص;
    }
  }

  کينډۍ <ډول_نوم دننه_واټن_بهر, ډول_نوم دننه_واټن_دننه, ډول_نوم بنسټ_3, ډول_نوم ...Ts>
  ساكن منطقي serialize_subset_دننه_واټن (hb_subset_context_t *c,
				       دننه_واټن_بهر& بهر,
				       ثابت دننه_واټن_دننه& دننه,
				       ثابت بنسټ_3 *بنسټ,
				       Ts&&... ds)
  {
    بهر = 0;
    إذا (دننه.is_null ()) أرجع خطأ;

    خپلکاری *s = c->serializer;
    s->push ();
    منطقي ret = c->dispatch (بنسټ+دننه, std::forward<Ts> (ds)...);
    إذا (ret) s->جمع_کړه_link (بهر, s->راوباسه_pack ());
    وإلا s->راوباسه_discard ();
    أرجع ret;
  }

  Fixed_نسخه<>نسخه;		/* Version of the GDEF table--currently
					 * 0x00010004u */
  Offset16_منزل<ClassDef>
		glyphClassDef;		/* Offset to class definition table
					 * for glyph type--from beginning of
					 * GDEF header (may be Null) */
  Offset16_منزل<AttachList>
		attachList;		/* Offset to list of glyphs with
					 * attachment points--from beginning
					 * of GDEF header (may be Null) */
  Offset16_منزل<LigCaretList<SmallTypes>>
		ligCaretList;		/* Offset to list of positioning points
					 * for ligature carets--from beginning
					 * of GDEF header (may be Null) */
  Offset16_منزل<ClassDef>
		markAttachClassDef;	/* Offset to class definition table for
					 * mark attachment type--from beginning
					 * of GDEF header (may be Null) */
  Offset16_منزل<MarkGlyphSets>
		markGlyphSetsDef;	/* Offset to the table of mark set
					 * definitions--from beginning of GDEF
					 * header (may be NULL).  Introduced
					 * in version 0x00010002. */
  Offset32_منزل<توکی_Variation_Store>
		varStore;		/* Offset to the table of Item Variation
					 * Store--from beginning of GDEF
					 * header (may be NULL).  Introduced
					 * in version 0x00010003. */
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
  Offset32_منزل<ClassDef>
		glyphClassDef2;		/* 32-bit offset to class definition
					 * table for glyph type.  Introduced
					 * in version 0x00010004. */
  Offset32_منزل<AttachList>
		attachList2;		/* 32-bit offset to list of glyphs
					 * with attachment points.  Introduced
					 * in version 0x00010004. */
  Offset32_منزل<LigCaretList<MediumTypes>>
		ligCaretList2;		/* 32-bit offset to LigCaretList2.
					 * Introduced in version 0x00010004. */
  Offset32_منزل<ClassDef>
		markAttachClassDef2;	/* 32-bit offset to class definition
					 * table for mark attachment type.
					 * Introduced in version 0x00010004. */
  Offset32_منزل<MarkGlyphSets>
		markGlyphSetsDef2;	/* 32-bit offset to the table of mark
					 * set definitions.  Introduced in
					 * version 0x00010004. */
#د_که_پای
  عام:
  DEFINE_اندازه_MIN (12);
};

بنية GDEF_accelerator_t : GDEF::accelerator_t {
  GDEF_accelerator_t (hb_face_t *face) : GDEF::accelerator_t (face) {}
};

} /* namespace OT */


#د_که_پای /* OT_LAYOUT_GDEF_GDEF_HH */
