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

#اگر_تعریف_نشده OT_LAYOUT_GDEF_GDEF_HH
#تعریف_کن OT_LAYOUT_GDEF_GDEF_HH

#ضمن "../../../hb-ot-var-common.hh"

#ضمن "../../../hb-font.hh"
#ضمن "../../../hb-cache.hh"


نطاق OT {


/*
 * Attachment List Table
 */

/* Array of contour point indices--in increasing numerical order */
بنية Attach_نقطه : Array16_از<HBUINT16>
{
  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *بیرون = c->serializer->شروع_embed (*هذا);
    return_trace (بیرون->serialize (c->serializer, + iter ()));
  }
};

/* AttachList is referenced by both GDEF attachListOffset and
 * attachListOffset2. The v1.4 top-level offset widens, but the
 * AttachList table itself keeps Offset16 internals. */
بنية AttachList
{
  بیعلامت صحيح get_attach_points (hb_codepoint_t glyph_id,
				  بیعلامت صحيح شروع_جابه_جایی,
				  بیعلامت صحيح *نقطه_شمار /* IN/OUT */,
				  بیعلامت صحيح *نقطه_array /*بیرون*/) ثابت
  {
    بیعلامت صحيح شاخص_2 = (هذا+coverage).get_coverage (glyph_id);
    إذا (شاخص_2 == NOT_COVERED)
    {
      إذا (نقطه_شمار)
	*نقطه_شمار = 0;
      أرجع 0;
    }

    ثابت Attach_نقطه &points = هذا+attach_نقطه[شاخص_2];

    إذا (نقطه_شمار && نقطه_array)
    {
      + points.as_array ().sub_array (شروع_جابه_جایی, نقطه_شمار)
      | hb_sink (hb_array (نقطه_array, *نقطه_شمار))
      ;
    }

    أرجع points.len;
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_مجموعه_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خودکار *بیرون = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (بیرون))) return_trace (خطأ);

    hb_sorted_وکتور_t<hb_codepoint_t> new_coverage;
    + hb_zip (هذا+coverage, attach_نقطه)
    | hb_filter (glyphset, hb_اولی)
    | hb_filter (subset_جابه_جایی_array (c, بیرون->attach_نقطه, هذا), hb_دومی)
    | hb_map (hb_اولی)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;
    بیرون->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (منطقي (new_coverage));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (coverage.sanitize (c, هذا) && attach_نقطه.sanitize (c, هذا));
  }

  محمي:
  Offset16_مقصد<Coverage>
		coverage;		/* Offset to Coverage table -- from
					 * beginning of AttachList table */
  Array16_از_Offset16_مقصد<Attach_نقطه>
		attach_نقطه;		/* Array of AttachPoint tables
					 * in Coverage Index order */
  عام:
  DEFINE_اندازه_ARRAY (4, attach_نقطه);
};

/*
 * Ligature Caret Table
 */

بنية Caret_مقدار_Format1
{
  دوست بنية Caret_مقدار;
  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *بیرون = c->serializer->embed (هذا);
    إذا (unlikely (!بیرون)) return_trace (خطأ);
    return_trace (درست);
  }

  خاص:
  hb_position_t get_caret_مقدار (hb_font_t *font, hb_direction_t direction) ثابت
  {
    أرجع HB_DIRECTION_IS_HORIZONTAL (direction) ? font->em_scale_x (coordinate) : font->em_scale_y (coordinate);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  محمي:
  HBUINT16	caret_مقدار_Format;	/* Format identifier--format = 1 */
  FWORD		coordinate;		/* X or Y value, in design units */
  عام:
  DEFINE_اندازه_STATIC (4);
};

بنية Caret_مقدار_Format2
{
  دوست بنية Caret_مقدار;
  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *بیرون = c->serializer->embed (هذا);
    إذا (unlikely (!بیرون)) return_trace (خطأ);
    return_trace (درست);
  }

  خاص:
  hb_position_t get_caret_مقدار (hb_font_t *font, hb_direction_t direction, hb_codepoint_t glyph_id) ثابت
  {
    hb_position_t x, y;
    font->get_glyph_contour_نقطه_for_origin (glyph_id, caret_مقدار_نقطه, direction, &x, &y);
    أرجع HB_DIRECTION_IS_HORIZONTAL (direction) ? x : y;
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  محمي:
  HBUINT16	caret_مقدار_Format;	/* Format identifier--format = 2 */
  HBUINT16	caret_مقدار_نقطه;	/* Contour point index on glyph */
  عام:
  DEFINE_اندازه_STATIC (4);
};

بنية Caret_مقدار_Format3
{
  دوست بنية Caret_مقدار;

  hb_position_t get_caret_مقدار (hb_font_t *font, hb_direction_t direction,
				 ثابت قلم_Variation_Store &var_store) ثابت
  {
    أرجع HB_DIRECTION_IS_HORIZONTAL (direction) ?
	   hb_saturate_جمع_کن (font->em_scale_x (coordinate), (هذا+device_جدول).get_x_delta (font, var_store)) :
	   hb_saturate_جمع_کن (font->em_scale_y (coordinate), (هذا+device_جدول).get_y_delta (font, var_store));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *بیرون = c->serializer->شروع_embed (*هذا);
    إذا (!c->serializer->embed (caret_مقدار_Format)) return_trace (خطأ);
    إذا (!c->serializer->embed (coordinate)) return_trace (خطأ);

    بیعلامت varidx = (هذا+device_جدول).get_variation_شاخص ();
    hb_pair_t<بیعلامت, صحيح> *new_varidx_delta;
    إذا (c->plan->layout_variation_idx_delta_map.has (varidx, &new_varidx_delta)) {
      uint32_t new_varidx = hb_اولی (*new_varidx_delta);
      صحيح delta = hb_دومی (*new_varidx_delta);
      إذا (delta != 0)
      {
        إذا (!c->serializer->check_تخصیص_بده (بیرون->coordinate, coordinate + delta, HB_SERIALIZE_ERROR_INT_سرریز))
          return_trace (خطأ);
      }

      إذا (new_varidx == HB_OT_LAYOUT_NO_VARIATIONS_شاخص)
        return_trace (c->serializer->check_تخصیص_بده (بیرون->caret_مقدار_Format, 1, HB_SERIALIZE_ERROR_INT_سرریز));
    }

    إذا (!c->serializer->embed (device_جدول))
      return_trace (خطأ);

    return_trace (بیرون->device_جدول.serialize_copy (c->serializer, device_جدول, هذا, c->serializer->مقصد_bias (بیرون),
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
  HBUINT16	caret_مقدار_Format;	/* Format identifier--format = 3 */
  FWORD		coordinate;		/* X or Y value, in design units */
  Offset16_مقصد<Device>
		device_جدول;		/* Offset to Device table for X or Y
					 * value--from beginning of CaretValue
					 * table */
  عام:
  DEFINE_اندازه_STATIC (6);
};

بنية Caret_مقدار
{
  hb_position_t get_caret_مقدار (hb_font_t *font,
				 hb_direction_t direction,
				 hb_codepoint_t glyph_id,
				 ثابت قلم_Variation_Store &var_store) ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.get_caret_مقدار (font, direction);
    حالة 2: hb_barrier (); أرجع u.format2.get_caret_مقدار (font, direction, glyph_id);
    حالة 3: hb_barrier (); أرجع u.format3.get_caret_مقدار (font, direction, var_store);
    افتراضي:أرجع 0;
    }
  }

  قالب <نام_نوع context_t, نام_نوع ...Ts>
  نام_نوع context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    إذا (unlikely (!c->may_dispatch (هذا, &u.format.v))) أرجع c->no_dispatch_return_مقدار ();
    TRACE_DISPATCH (هذا, u.format.v);
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    حالة 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    حالة 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    افتراضي:return_trace (c->default_return_مقدار ());
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
    افتراضي:return_trace (درست);
    }
  }

  محمي:
  اتحاد {
  بنية { HBUINT16 v; }	format;		/* Format identifier */
  Caret_مقدار_Format1	format1;
  Caret_مقدار_Format2	format2;
  Caret_مقدار_Format3	format3;
  } u;
  عام:
  DEFINE_اندازه_UNION (2, format.v);
};

بنية LigGlyph
{
  بیعلامت get_lig_carets (hb_font_t            *font,
			   hb_direction_t        direction,
			   hb_codepoint_t        glyph_id,
			   ثابت قلم_Variation_Store &var_store,
			   بیعلامت              شروع_جابه_جایی,
			   بیعلامت             *caret_شمار /* IN/OUT */,
			   hb_position_t        *caret_array /*بیرون*/) ثابت
  {
    إذا (caret_شمار && caret_array)
    {
      + carets.as_array ().sub_array (شروع_جابه_جایی, caret_شمار)
      | hb_map (hb_جمع_کن (هذا))
      | hb_map ([&] (ثابت Caret_مقدار &value) { أرجع value.get_caret_مقدار (font, direction, glyph_id, var_store); })
      | hb_sink (hb_array (caret_array, *caret_شمار))
      ;
    }

    أرجع carets.len;
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *بیرون = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (بیرون))) return_trace (خطأ);

    + hb_iter (carets)
    | hb_apply (subset_جابه_جایی_array (c, بیرون->carets, هذا))
    ;

    return_trace (منطقي (بیرون->carets));
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    لكل (ثابت Offset16_مقصد<Caret_مقدار>& جابه_جایی : carets.iter ())
      (هذا+جابه_جایی).collect_variation_indices (c);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (carets.sanitize (c, هذا));
  }

  محمي:
  Array16_از_Offset16_مقصد<Caret_مقدار>
		carets;			/* Offset array of CaretValue tables
					 * --from beginning of LigGlyph table
					 * --in increasing coordinate order */
  عام:
  DEFINE_اندازه_ARRAY (2, carets);
};

/* LigCaretList2 widens the LigCaretList coverage, count, and LigGlyph
 * offsets. The LigGlyph table itself keeps Offset16 caretValueOffsets. */
قالب <نام_نوع Types>
بنية LigCaretList
{
  بیعلامت صحيح get_lig_carets (hb_font_t *font,
			       hb_direction_t direction,
			       hb_codepoint_t glyph_id,
			       ثابت قلم_Variation_Store &var_store,
			       بیعلامت صحيح شروع_جابه_جایی,
			       بیعلامت صحيح *caret_شمار /* IN/OUT */,
			       hb_position_t *caret_array /*بیرون*/) ثابت
  {
    بیعلامت صحيح شاخص_2 = (هذا+coverage).get_coverage (glyph_id);
    إذا (شاخص_2 == NOT_COVERED)
    {
      إذا (caret_شمار)
	*caret_شمار = 0;
      أرجع 0;
    }
    ثابت LigGlyph &lig_glyph = هذا+ligGlyph[شاخص_2];
    أرجع lig_glyph.get_lig_carets (font, direction, glyph_id, var_store, شروع_جابه_جایی, caret_شمار, caret_array);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_مجموعه_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خودکار *بیرون = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (بیرون))) return_trace (خطأ);

    hb_sorted_وکتور_t<hb_codepoint_t> new_coverage;
    + hb_zip (هذا+coverage, ligGlyph)
    | hb_filter (glyphset, hb_اولی)
    | hb_filter (subset_جابه_جایی_array (c, بیرون->ligGlyph, هذا), hb_دومی)
    | hb_map (hb_اولی)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;
    بیرون->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (منطقي (new_coverage));
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    + hb_zip (هذا+coverage, ligGlyph)
    | hb_filter (c->glyph_مجموعه, hb_اولی)
    | hb_map (hb_دومی)
    | hb_map (hb_جمع_کن (هذا))
    | hb_apply ([c] (ثابت LigGlyph& _) { _.collect_variation_indices (c); })
    ;
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (coverage.sanitize (c, هذا) && ligGlyph.sanitize (c, هذا));
  }

  محمي:
  نام_نوع Types::قالب LOffset_مقصد<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of LigCaretList table */
  نام_نوع Types::قالب Array_از<نام_نوع Types::قالب جابه_جایی_مقصد<LigGlyph>>
		ligGlyph;		/* Array of LigGlyph tables
					 * in Coverage Index order */
  عام:
  DEFINE_اندازه_ARRAY (Types::LOffset::static_اندازه + Types::HBUINT::static_اندازه, ligGlyph);
};

#اگر_تعریف_نشده HB_NO_BEYOND_64K
static_assert (LigCaretList<MediumTypes>::min_اندازه ==
	       Offset32::static_اندازه + HBUINT24::static_اندازه, "");
#پایان_اگر


بنية MarkGlyphSetsFormat1
{
  منطقي covers (بیعلامت صحيح مجموعه_شاخص, hb_codepoint_t glyph_id) ثابت
  { أرجع (هذا+coverage[مجموعه_شاخص]).get_coverage (glyph_id) != NOT_COVERED; }

  فراغ collect_استفاده_شده_mark_sets (ثابت hb_مجموعه_t& glyph_مجموعه,
                               hb_مجموعه_t& استفاده_شده_mark_sets /*بیرون*/) ثابت
  {
    بیعلامت i = 0;
    لكل (ثابت خودکار &جابه_جایی : coverage)
     {
       ثابت خودکار &cov = هذا+جابه_جایی;
       إذا (cov.intersects (&glyph_مجموعه))
         استفاده_شده_mark_sets.جمع_کن (i);

       i++;
     }
  }

  قالب <نام_نوع مجموعه_t>
  فراغ collect_coverage (hb_وکتور_t<مجموعه_t> &sets) ثابت
  {
     لكل (ثابت خودکار &جابه_جایی : coverage)
     {
       ثابت خودکار &cov = هذا+جابه_جایی;
       cov.collect_coverage (sets.push ());
     }
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *بیرون = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (بیرون))) return_trace (خطأ);
    بیرون->format = format;

    منطقي ret = درست;
    لكل (ثابت Offset32_مقصد<Coverage>& جابه_جایی : coverage.iter ())
    {
      خودکار snap = c->serializer->snapshot ();
      خودکار *o = بیرون->coverage.serialize_append (c->serializer);
      إذا (unlikely (!o))
      {
	ret = خطأ;
	اكسر;
      }

      //skip empty coverage
      c->serializer->push ();
      منطقي res = خطأ;
      إذا (جابه_جایی) res = c->dispatch (هذا+جابه_جایی);
      إذا (!res)
      {
        c->serializer->برداشت_discard ();
        c->serializer->revert (snap);
        (بیرون->coverage.len)--;
        استمر;
      }
      c->serializer->جمع_کن_link (*o, c->serializer->برداشت_pack ());
    }

    return_trace (ret && بیرون->coverage.len);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (coverage.sanitize (c, هذا));
  }

  محمي:
  HBUINT16	format;			/* Format identifier--format = 1 */
  Array16_از<Offset32_مقصد<Coverage>>
		coverage;		/* Array of long offsets to mark set
					 * coverage tables */
  عام:
  DEFINE_اندازه_ARRAY (4, coverage);
};

بنية MarkGlyphSets
{
  منطقي covers (بیعلامت صحيح مجموعه_شاخص, hb_codepoint_t glyph_id) ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.covers (مجموعه_شاخص, glyph_id);
    افتراضي:أرجع خطأ;
    }
  }

  قالب <نام_نوع مجموعه_t>
  فراغ collect_coverage (hb_وکتور_t<مجموعه_t> &sets) ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); u.format1.collect_coverage (sets); أرجع;
    افتراضي:أرجع;
    }
  }

  فراغ collect_استفاده_شده_mark_sets (ثابت hb_مجموعه_t& glyph_مجموعه,
                               hb_مجموعه_t& استفاده_شده_mark_sets /*بیرون*/) ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); u.format1.collect_استفاده_شده_mark_sets (glyph_مجموعه, استفاده_شده_mark_sets); أرجع;
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
    افتراضي:return_trace (درست);
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
  ساكن constexpr hb_برچسب_t جدول_برچسب = HB_OT_برچسب_GDEF;

  تعداد GlyphClasses {
    UnclassifiedGlyph	= 0,
    پایه_Glyph		= 1,
    LigatureGlyph	= 2,
    MarkGlyph		= 3,
    ComponentGlyph	= 4
  };

  size_t get_اندازه () ثابت
  {
    إذا (نسخه.major != 1) أرجع نسخه.static_اندازه;

    أرجع min_اندازه +
	   (نسخه.مقصد_int () >= 0x00010002u ? markGlyphSetsDef.static_اندازه : 0) +
	   (نسخه.مقصد_int () >= 0x00010003u ? varStore.static_اندازه : 0) +
#اگر_تعریف_نشده HB_NO_BEYOND_64K
	   (نسخه.مقصد_int () >= 0x00010004u ? 5 * Offset32::static_اندازه : 0)
#وإلا
	   0
#پایان_اگر
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
    منطقي mark_glyph_sets_def_ok = نسخه.مقصد_int () < 0x00010002u ||
				  markGlyphSetsDef.sanitize (c, هذا);

#اگر_تعریف_نشده HB_NO_BEYOND_64K
    إذا (نسخه.مقصد_int () >= 0x00010004u)
    {
      إذا (glyphClassDef2) glyph_class_def_ok = glyphClassDef2.sanitize (c, هذا);
      إذا (attachList2) attach_list_ok = attachList2.sanitize (c, هذا);
      إذا (ligCaretList2) lig_caret_list_ok = ligCaretList2.sanitize (c, هذا);
      إذا (markAttachClassDef2) mark_attach_class_def_ok = markAttachClassDef2.sanitize (c, هذا);
      إذا (markGlyphSetsDef2) mark_glyph_sets_def_ok = markGlyphSetsDef2.sanitize (c, هذا);
    }
#پایان_اگر

    return_trace (glyph_class_def_ok &&
		  attach_list_ok &&
		  lig_caret_list_ok &&
		  mark_attach_class_def_ok &&
		  mark_glyph_sets_def_ok &&
		  (نسخه.مقصد_int () < 0x00010003u || varStore.sanitize (c, هذا)));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (نسخه.major != 1) return_trace (خطأ);

    خودکار *بیرون = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (بیرون))) return_trace (خطأ);

    // Push var store first (if it's needed) so that it's last in the
    // serialization order. Some font consumers assume that varstore runs to
    // the end of the GDEF table.
    // See: https://github.com/harfbuzz/harfbuzz/issues/4636
    خودکار snapshot_version0 = c->serializer->snapshot ();
    إذا (unlikely (نسخه.مقصد_int () >= 0x00010002u && hb_barrier () &&
		  !c->serializer->embed (markGlyphSetsDef)))
      return_trace (خطأ);

    خودکار snapshot_version2 = c->serializer->snapshot ();
    إذا (unlikely (نسخه.مقصد_int () >= 0x00010003u && hb_barrier () &&
		  !c->serializer->embed (varStore)))
      return_trace (خطأ);

    منطقي use_v14 = خطأ;
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    use_v14 = نسخه.مقصد_int () >= 0x00010004u &&
	      (glyphClassDef2 || attachList2 || ligCaretList2 ||
	       markAttachClassDef2 || markGlyphSetsDef2);
    إذا (unlikely (نسخه.مقصد_int () >= 0x00010004u && hb_barrier () &&
		  !c->serializer->extend_اندازه (بیرون, get_اندازه ())))
      return_trace (خطأ);
#پایان_اگر

    منطقي subset_varstore = خطأ;
    بیعلامت varstore_شاخص = (بیعلامت) -1;
    إذا (نسخه.مقصد_int () >= 0x00010003u && hb_barrier ())
    {
      إذا (c->plan->all_axes_pinned)
	بیرون->varStore = 0;
      وإلا إذا (c->plan->normalized_coords)
      {
	إذا (varStore)
	{
	  قلم_variations_t قلم_vars;
	  إذا (unlikely (!c->serializer->check_success (
                  قلم_vars.instantiate (هذا+varStore, c->plan, درست, درست,
                                          c->plan->gdef_varstore_inner_maps.as_array (), درست))))
            return_trace (خطأ);
	  subset_varstore = بیرون->varStore.serialize_serialize (c->serializer,
							     قلم_vars.has_long_کلمه (),
							     c->plan->axis_برچسبها,
							     قلم_vars.get_region_list (),
							     قلم_vars.get_vardata_encodings ());
	  varstore_شاخص = c->serializer->آخرین_added_فرزند_شاخص();
	  remap_varidx_after_instantiation (قلم_vars.get_varidx_map (),
					    c->plan->layout_variation_idx_delta_map);
          remap_varidx_after_instantiation (قلم_vars.get_varidx_map (),
                                            c->plan->layout_condition_idx_مقدار_map);
	}
      }
      وإلا
      {
	subset_varstore = بیرون->varStore.serialize_subset (c, varStore, هذا,
							  c->plan->gdef_varstore_inner_maps.as_array ());
	varstore_شاخص = c->serializer->آخرین_added_فرزند_شاخص();
      }
    }

    بیرون->نسخه.major = نسخه.major;
    بیرون->نسخه.minor = نسخه.minor;

    إذا (!subset_varstore && نسخه.مقصد_int () >= 0x00010002u && !use_v14) {
      c->serializer->revert (snapshot_version2);
    }
    إذا (!subset_varstore)
      بیرون->varStore = 0;
    وإلا
      c->plan->has_gdef_varstore = درست;

    منطقي subset_markglyphsetsdef = خطأ;
    إذا (نسخه.مقصد_int () >= 0x00010002u && hb_barrier ())
    {
#اگر_تعریف_نشده HB_NO_BEYOND_64K
      إذا (use_v14 && markGlyphSetsDef2)
      {
	بیرون->markGlyphSetsDef = 0;
	subset_markglyphsetsdef = serialize_subset_جابه_جایی (c, بیرون->markGlyphSetsDef2,
							   markGlyphSetsDef2, هذا);
      }
      وإلا
#پایان_اگر
      {
#اگر_تعریف_نشده HB_NO_BEYOND_64K
	إذا (use_v14) بیرون->markGlyphSetsDef2 = 0;
#پایان_اگر
	subset_markglyphsetsdef = بیرون->markGlyphSetsDef.serialize_subset (c, markGlyphSetsDef, هذا);
      }
    }

    إذا (use_v14)
    {
      بیرون->نسخه.minor = 4;
    } وإلا إذا (subset_varstore) {
      بیرون->نسخه.minor = 3;
    } وإلا إذا (subset_markglyphsetsdef) {
      بیرون->نسخه.minor = 2;
    } وإلا  {
      بیرون->نسخه.minor = 0;
      c->serializer->revert (snapshot_version0);
    }

    منطقي subset_glyphclassdef = خطأ;
    منطقي subset_attachlist = خطأ;
    منطقي subset_markattachclassdef = خطأ;
    منطقي subset_ligcaretlist = خطأ;

#اگر_تعریف_نشده HB_NO_BEYOND_64K
    إذا (use_v14 && glyphClassDef2)
    {
      بیرون->glyphClassDef = 0;
      subset_glyphclassdef = serialize_subset_جابه_جایی (c, بیرون->glyphClassDef2,
						      glyphClassDef2, هذا, اشاره_تهی, خطأ, درست);
    }
    وإلا
#پایان_اگر
    {
#اگر_تعریف_نشده HB_NO_BEYOND_64K
      إذا (use_v14) بیرون->glyphClassDef2 = 0;
#پایان_اگر
      subset_glyphclassdef = بیرون->glyphClassDef.serialize_subset (c, glyphClassDef, هذا, اشاره_تهی, خطأ, درست);
    }

#اگر_تعریف_نشده HB_NO_BEYOND_64K
    إذا (use_v14 && attachList2)
    {
      بیرون->attachList = 0;
      subset_attachlist = serialize_subset_جابه_جایی (c, بیرون->attachList2, attachList2, هذا);
    }
    وإلا
#پایان_اگر
    {
#اگر_تعریف_نشده HB_NO_BEYOND_64K
      إذا (use_v14) بیرون->attachList2 = 0;
#پایان_اگر
      subset_attachlist = بیرون->attachList.serialize_subset (c, attachList, هذا);
    }

#اگر_تعریف_نشده HB_NO_BEYOND_64K
    إذا (use_v14 && markAttachClassDef2)
    {
      بیرون->markAttachClassDef = 0;
      subset_markattachclassdef = serialize_subset_جابه_جایی (c, بیرون->markAttachClassDef2,
							   markAttachClassDef2, هذا, اشاره_تهی, خطأ, درست);
    }
    وإلا
#پایان_اگر
    {
#اگر_تعریف_نشده HB_NO_BEYOND_64K
      إذا (use_v14) بیرون->markAttachClassDef2 = 0;
#پایان_اگر
      subset_markattachclassdef = بیرون->markAttachClassDef.serialize_subset (c, markAttachClassDef,
									   هذا, اشاره_تهی, خطأ, درست);
    }

#اگر_تعریف_نشده HB_NO_BEYOND_64K
    إذا (use_v14 && ligCaretList2)
    {
      بیرون->ligCaretList = 0;
      subset_ligcaretlist = serialize_subset_جابه_جایی (c, بیرون->ligCaretList2, ligCaretList2, هذا);
    }
    وإلا
#پایان_اگر
    {
#اگر_تعریف_نشده HB_NO_BEYOND_64K
      إذا (use_v14) بیرون->ligCaretList2 = 0;
#پایان_اگر
      subset_ligcaretlist = بیرون->ligCaretList.serialize_subset (c, ligCaretList, هذا);
    }

    إذا (subset_varstore && varstore_شاخص != (بیعلامت) -1) {
      c->serializer->repack_آخرین(varstore_شاخص);
    }

    return_trace (subset_glyphclassdef || subset_attachlist ||
		  subset_ligcaretlist || subset_markattachclassdef ||
		  (بیرون->نسخه.مقصد_int () >= 0x00010002u && subset_markglyphsetsdef) ||
		  (بیرون->نسخه.مقصد_int () >= 0x00010003u && subset_varstore));
  }

  منطقي has_glyph_classes () ثابت
  {
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    إذا (نسخه.مقصد_int () >= 0x00010004u && glyphClassDef2) أرجع درست;
#پایان_اگر
    أرجع glyphClassDef != 0;
  }
  ثابت ClassDef &get_glyph_class_def () ثابت
  {
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    إذا (نسخه.مقصد_int () >= 0x00010004u && glyphClassDef2) أرجع هذا+glyphClassDef2;
#پایان_اگر
    أرجع هذا+glyphClassDef;
  }
  منطقي has_attach_list () ثابت
  {
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    إذا (نسخه.مقصد_int () >= 0x00010004u && attachList2) أرجع درست;
#پایان_اگر
    أرجع attachList != 0;
  }
  ثابت AttachList &get_attach_list () ثابت
  {
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    إذا (نسخه.مقصد_int () >= 0x00010004u && attachList2) أرجع هذا+attachList2;
#پایان_اگر
    أرجع هذا+attachList;
  }
  منطقي has_lig_carets () ثابت
  {
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    إذا (نسخه.مقصد_int () >= 0x00010004u && ligCaretList2) أرجع درست;
#پایان_اگر
    أرجع ligCaretList != 0;
  }
  منطقي has_mark_attachment_types () ثابت
  {
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    إذا (نسخه.مقصد_int () >= 0x00010004u && markAttachClassDef2) أرجع درست;
#پایان_اگر
    أرجع markAttachClassDef != 0;
  }
  ثابت ClassDef &get_mark_attach_class_def () ثابت
  {
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    إذا (نسخه.مقصد_int () >= 0x00010004u && markAttachClassDef2) أرجع هذا+markAttachClassDef2;
#پایان_اگر
    أرجع هذا+markAttachClassDef;
  }
  منطقي has_mark_glyph_sets () ثابت
  {
    إذا (نسخه.مقصد_int () < 0x00010002u) أرجع خطأ;
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    إذا (نسخه.مقصد_int () >= 0x00010004u && markGlyphSetsDef2) أرجع درست;
#پایان_اگر
    أرجع markGlyphSetsDef != 0;
  }
  ثابت MarkGlyphSets &get_mark_glyph_sets () ثابت
  {
    إذا (نسخه.مقصد_int () < 0x00010002u) أرجع Null(MarkGlyphSets);
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    إذا (نسخه.مقصد_int () >= 0x00010004u && markGlyphSetsDef2) أرجع هذا+markGlyphSetsDef2;
#پایان_اگر
    أرجع هذا+markGlyphSetsDef;
  }
  منطقي has_var_store () ثابت
  {
    أرجع نسخه.مقصد_int () >= 0x00010003u && hb_barrier () && varStore != 0;
  }
  ثابت قلم_Variation_Store &get_var_store () ثابت
  {
    أرجع نسخه.مقصد_int () >= 0x00010003u && hb_barrier () ? هذا+varStore : Null(قلم_Variation_Store);
  }


  منطقي has_داده () ثابت { أرجع نسخه.مقصد_int (); }
  بیعلامت صحيح get_glyph_class (hb_codepoint_t glyph) ثابت
  { أرجع get_glyph_class_def ().get_class (glyph); }
  فراغ get_glyphs_درون_class (بیعلامت صحيح klass, hb_مجموعه_t *glyphs) ثابت
  { get_glyph_class_def ().collect_class (glyphs, klass); }

  بیعلامت صحيح get_mark_attachment_type (hb_codepoint_t glyph) ثابت
  { أرجع get_mark_attach_class_def ().get_class (glyph); }

  بیعلامت صحيح get_attach_points (hb_codepoint_t glyph_id,
				  بیعلامت صحيح شروع_جابه_جایی,
				  بیعلامت صحيح *نقطه_شمار /* IN/OUT */,
				  بیعلامت صحيح *نقطه_array /*بیرون*/) ثابت
  { أرجع get_attach_list ().get_attach_points (glyph_id, شروع_جابه_جایی, نقطه_شمار, نقطه_array); }

  بیعلامت صحيح get_lig_carets (hb_font_t *font,
			       hb_direction_t direction,
			       hb_codepoint_t glyph_id,
			       بیعلامت صحيح شروع_جابه_جایی,
			       بیعلامت صحيح *caret_شمار /* IN/OUT */,
			       hb_position_t *caret_array /*بیرون*/) ثابت
  {
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    إذا (نسخه.مقصد_int () >= 0x00010004u && ligCaretList2)
      أرجع (هذا+ligCaretList2).get_lig_carets (font, direction, glyph_id, get_var_store (),
						  شروع_جابه_جایی, caret_شمار, caret_array);
#پایان_اگر
    أرجع (هذا+ligCaretList).get_lig_carets (font, direction, glyph_id, get_var_store (),
					       شروع_جابه_جایی, caret_شمار, caret_array);
  }

  منطقي mark_مجموعه_covers (بیعلامت صحيح مجموعه_شاخص, hb_codepoint_t glyph_id) ثابت
  { أرجع get_mark_glyph_sets ().covers (مجموعه_شاخص, glyph_id); }

  /* glyph_props is a 16-bit integer where the lower 8-bit have bits representing
   * glyph class and other bits, and high 8-bit the mark attachment type (if any).
   * Not to be confused with lookup_props which is very similar. */
  بیعلامت صحيح get_glyph_props (hb_codepoint_t glyph) ثابت
  {
    بیعلامت صحيح klass = get_glyph_class (glyph);

    static_assert (((بیعلامت صحيح) HB_OT_LAYOUT_GLYPH_PROPS_پایه_GLYPH == (بیعلامت صحيح) LookupFlag::Ignore_پایه_Glyphs), "");
    static_assert (((بیعلامت صحيح) HB_OT_LAYOUT_GLYPH_PROPS_LIGATURE == (بیعلامت صحيح) LookupFlag::IgnoreLigatures), "");
    static_assert (((بیعلامت صحيح) HB_OT_LAYOUT_GLYPH_PROPS_MARK == (بیعلامت صحيح) LookupFlag::IgnoreMarks), "");

    بدل (klass) {
    افتراضي:			أرجع HB_OT_LAYOUT_GLYPH_CLASS_UNCLASSIFIED;
    حالة پایه_Glyph:		أرجع HB_OT_LAYOUT_GLYPH_PROPS_پایه_GLYPH;
    حالة LigatureGlyph:		أرجع HB_OT_LAYOUT_GLYPH_PROPS_LIGATURE;
    حالة MarkGlyph:
	  klass = get_mark_attachment_type (glyph);
	  أرجع HB_OT_LAYOUT_GLYPH_PROPS_MARK | (klass << 8);
    }
  }

  HB_داخلی منطقي is_blocklisted (hb_blob_t *blob,
				   hb_face_t *face) ثابت;

  بنية accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      جدول = hb_sanitize_context_t ().reference_جدول<GDEF> (face);
      إذا (unlikely (جدول->is_blocklisted (جدول.get_blob (), face)))
      {
	hb_blob_destroy (جدول.get_blob ());
	جدول = hb_blob_get_خالی ();
      }

#اگر_تعریف_نشده HB_NO_GDEF_CACHE
      /* Flatten each mark set whose glyphs span fewer than PAGE_BITS
       * gids into a single bit-page biased at its first glyph, for
       * O(1) exact answers.  Wider sets, or on allocation failure,
       * fall back to the table. */
      {
	hb_وکتور_t<hb_مجموعه_t> sets;
	جدول->get_mark_glyph_sets ().collect_coverage (sets);
	إذا (!sets.درون_error () &&
	    mark_glyph_مجموعه_bitmaps.resize (sets.درازی))
	  لكل (بیعلامت i = 0; i < sets.درازی; i++)
	  {
	    خودکار &bitmap = mark_glyph_مجموعه_bitmaps.arrayZ[i];
	    bitmap.bias = HB_مجموعه_مقدار_INVALID;
	    ثابت خودکار &مجموعه = sets.arrayZ[i];
	    إذا (unlikely (مجموعه.درون_error ())) استمر;
	    hb_codepoint_t min_gid = HB_مجموعه_مقدار_INVALID;
	    hb_codepoint_t max_gid = HB_مجموعه_مقدار_INVALID;
	    hb_مجموعه_بعدی (&مجموعه, &min_gid);
	    hb_مجموعه_قبلی (&مجموعه, &max_gid);
	    إذا (max_gid == HB_مجموعه_مقدار_INVALID)
	      استمر;
	    إذا (max_gid - min_gid >= hb_بیت_صفحه_t::صفحه_بیتها)
	    {
	      /* Too wide for one page; a digest pre-filter instead. */
	      لكل (hb_codepoint_t gid : مجموعه)
		bitmap.u.digest.جمع_کن (gid);
	      استمر;
	    }
	    bitmap.bias = min_gid;
	    لكل (hb_codepoint_t gid : مجموعه)
	      bitmap.u.صفحه.جمع_کن (gid - min_gid);
	  }
      }
#پایان_اگر
    }
    ~accelerator_t () { جدول.destroy (); }

    بیعلامت صحيح get_glyph_props (hb_codepoint_t glyph) ثابت
    {
      بیعلامت v;

#اگر_تعریف_نشده HB_NO_GDEF_CACHE
      إذا (glyph_props_cache.get (glyph, &v))
        أرجع v;
#پایان_اگر

      v = جدول->get_glyph_props (glyph);

#اگر_تعریف_نشده HB_NO_GDEF_CACHE
      إذا (likely (جدول.get_blob ())) // Don't try setting if we are the null instance!
	glyph_props_cache.مجموعه (glyph, v);
#پایان_اگر

      أرجع v;

    }

    HB_ALWAYS_INLINE
    منطقي mark_مجموعه_covers (بیعلامت صحيح مجموعه_شاخص, hb_codepoint_t glyph_id) ثابت
    {
#اگر_تعریف_نشده HB_NO_GDEF_CACHE
      // We can access arrayZ directly because of sanitize_lookup_props() guarantee.
      ثابت خودکار &bitmap = mark_glyph_مجموعه_bitmaps.arrayZ[مجموعه_شاخص];
      إذا (likely (bitmap.bias != HB_مجموعه_مقدار_INVALID))
      {
	hb_codepoint_t biased = glyph_id - bitmap.bias;
	أرجع biased < hb_بیت_صفحه_t::صفحه_بیتها &&
	       bitmap.u.صفحه.get (biased);
      }
      أرجع mark_مجموعه_covers_slow (مجموعه_شاخص, glyph_id);
#وإلا
      أرجع جدول->mark_مجموعه_covers (مجموعه_شاخص, glyph_id);
#پایان_اگر
    }

#اگر_تعریف_نشده HB_NO_GDEF_CACHE
    HB_NEVER_INLINE
    منطقي mark_مجموعه_covers_slow (بیعلامت صحيح مجموعه_شاخص, hb_codepoint_t glyph_id) ثابت
    {
      ثابت خودکار &bitmap = mark_glyph_مجموعه_bitmaps.arrayZ[مجموعه_شاخص];
      إذا (!bitmap.u.digest.may_have (glyph_id))
	أرجع خطأ;
      أرجع جدول->mark_مجموعه_covers (مجموعه_شاخص, glyph_id);
    }
#پایان_اگر

    بیعلامت sanitize_lookup_props (بیعلامت lookup_props) ثابت
    {
#اگر_تعریف_نشده HB_NO_GDEF_CACHE
      إذا (lookup_props & LookupFlag::Use_Mark_Filtering_مجموعه &&
	  (lookup_props >> 16) >= mark_glyph_مجموعه_bitmaps.درازی)
      {
        // Invalid mark filtering set index; unset the flag.
	lookup_props &= ~LookupFlag::Use_Mark_Filtering_مجموعه;
      }
#پایان_اگر
      أرجع lookup_props;
    }

    hb_blob_ptr_t<GDEF> جدول;
#اگر_تعریف_نشده HB_NO_GDEF_CACHE
    بنية mark_glyph_مجموعه_bitmap_t
    {
      /* Gid of the first bit (not page-aligned) for an exact one-page
       * set, or HB_SET_VALUE_INVALID; then digest is a pre-filter for
       * the table.  The all-zeros state is valid for both members. */
      hb_codepoint_t bias = HB_مجموعه_مقدار_INVALID;
      اتحاد u_t {
	u_t () : صفحه () {}
	hb_بیت_صفحه_t صفحه;
	hb_مجموعه_digest_t digest;
      } u;
    };

    hb_وکتور_t<mark_glyph_مجموعه_bitmap_t> mark_glyph_مجموعه_bitmaps;
    تغییرپذیر hb_cache_t<21, 3> glyph_props_cache;
    static_assert (حجم (glyph_props_cache) == 512, "");
#پایان_اگر
  };

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    إذا (نسخه.مقصد_int () >= 0x00010004u && ligCaretList2)
    {
      (هذا+ligCaretList2).collect_variation_indices (c);
      أرجع;
    }
#پایان_اگر
    (هذا+ligCaretList).collect_variation_indices (c);
  }

  محمي:
  قالب <نام_نوع کلید>
  ساكن فراغ remap_varidx_after_instantiation (ثابت hb_map_t& varidx_map,
                                                hb_hashmap_t<کلید, hb_pair_t<بیعلامت, صحيح>>& layout_variation_idx_delta_map /* IN/OUT */)
  {
    /* varidx_map is empty which means varstore is empty after instantiation,
     * no variations, map all varidx to HB_OT_LAYOUT_NO_VARIATIONS_INDEX.
     * varidx_map doesn't have original varidx, indicating delta row is all
     * zeros, map varidx to HB_OT_LAYOUT_NO_VARIATIONS_INDEX */
    لكل (خودکار _ : layout_variation_idx_delta_map.iter_ref ())
    {
      /* old_varidx->(varidx, delta) mapping generated for subsetting, then this
       * varidx is used as key of varidx_map during instantiation */
      uint32_t varidx = _.second.اولی;
      uint32_t *new_varidx;
      إذا (varidx_map.has (varidx, &new_varidx))
	_.second.اولی = *new_varidx;
      وإلا
	_.second.اولی = HB_OT_LAYOUT_NO_VARIATIONS_شاخص;
    }
  }

  قالب <نام_نوع جابه_جایی_بیرون, نام_نوع جابه_جایی_درون, نام_نوع پایه_3, نام_نوع ...Ts>
  ساكن منطقي serialize_subset_جابه_جایی (hb_subset_context_t *c,
				       جابه_جایی_بیرون& بیرون,
				       ثابت جابه_جایی_درون& درون,
				       ثابت پایه_3 *پایه,
				       Ts&&... ds)
  {
    بیرون = 0;
    إذا (درون.is_null ()) أرجع خطأ;

    خودکار *s = c->serializer;
    s->push ();
    منطقي ret = c->dispatch (پایه+درون, std::forward<Ts> (ds)...);
    إذا (ret) s->جمع_کن_link (بیرون, s->برداشت_pack ());
    وإلا s->برداشت_discard ();
    أرجع ret;
  }

  Fixed_نسخه<>نسخه;		/* Version of the GDEF table--currently
					 * 0x00010004u */
  Offset16_مقصد<ClassDef>
		glyphClassDef;		/* Offset to class definition table
					 * for glyph type--from beginning of
					 * GDEF header (may be Null) */
  Offset16_مقصد<AttachList>
		attachList;		/* Offset to list of glyphs with
					 * attachment points--from beginning
					 * of GDEF header (may be Null) */
  Offset16_مقصد<LigCaretList<SmallTypes>>
		ligCaretList;		/* Offset to list of positioning points
					 * for ligature carets--from beginning
					 * of GDEF header (may be Null) */
  Offset16_مقصد<ClassDef>
		markAttachClassDef;	/* Offset to class definition table for
					 * mark attachment type--from beginning
					 * of GDEF header (may be Null) */
  Offset16_مقصد<MarkGlyphSets>
		markGlyphSetsDef;	/* Offset to the table of mark set
					 * definitions--from beginning of GDEF
					 * header (may be NULL).  Introduced
					 * in version 0x00010002. */
  Offset32_مقصد<قلم_Variation_Store>
		varStore;		/* Offset to the table of Item Variation
					 * Store--from beginning of GDEF
					 * header (may be NULL).  Introduced
					 * in version 0x00010003. */
#اگر_تعریف_نشده HB_NO_BEYOND_64K
  Offset32_مقصد<ClassDef>
		glyphClassDef2;		/* 32-bit offset to class definition
					 * table for glyph type.  Introduced
					 * in version 0x00010004. */
  Offset32_مقصد<AttachList>
		attachList2;		/* 32-bit offset to list of glyphs
					 * with attachment points.  Introduced
					 * in version 0x00010004. */
  Offset32_مقصد<LigCaretList<MediumTypes>>
		ligCaretList2;		/* 32-bit offset to LigCaretList2.
					 * Introduced in version 0x00010004. */
  Offset32_مقصد<ClassDef>
		markAttachClassDef2;	/* 32-bit offset to class definition
					 * table for mark attachment type.
					 * Introduced in version 0x00010004. */
  Offset32_مقصد<MarkGlyphSets>
		markGlyphSetsDef2;	/* 32-bit offset to the table of mark
					 * set definitions.  Introduced in
					 * version 0x00010004. */
#پایان_اگر
  عام:
  DEFINE_اندازه_MIN (12);
};

بنية GDEF_accelerator_t : GDEF::accelerator_t {
  GDEF_accelerator_t (hb_face_t *face) : GDEF::accelerator_t (face) {}
};

} /* namespace OT */


#پایان_اگر /* OT_LAYOUT_GDEF_GDEF_HH */
