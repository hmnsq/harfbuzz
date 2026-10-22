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

#اگر_غیر_تعریف_شدہ OT_LAYOUT_GDEF_GDEF_HH
#تعریف_کرو OT_LAYOUT_GDEF_GDEF_HH

#شامل "../../../hb-ot-var-common.hh"

#شامل "../../../hb-font.hh"
#شامل "../../../hb-cache.hh"


نامخلا OT {


/*
 * Attachment List Table
 */

/* Array of contour point indices--in increasing numerical order */
ساخت Attach_نقطہ : Array16_کا<HBUINT16>
{
  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);
    خودکار *برآمد = c->serializer->آغاز_embed (*یہ);
    return_trace (برآمد->serialize (c->serializer, + iter ()));
  }
};

/* AttachList is referenced by both GDEF attachListOffset and
 * attachListOffset2. The v1.4 top-level offset widens, but the
 * AttachList table itself keeps Offset16 internals. */
ساخت AttachList
{
  غیرعلامتی صحیح get_attach_points (hb_codepoint_t glyph_id,
				  غیرعلامتی صحیح آغاز_انحراف,
				  غیرعلامتی صحیح *نقطہ_گنتی /* IN/OUT */,
				  غیرعلامتی صحیح *نقطہ_array /*برآمد*/) ثابت
  {
    غیرعلامتی صحیح اشاریہ_2 = (یہ+coverage).get_coverage (glyph_id);
    اگر (اشاریہ_2 == NOT_COVERED)
    {
      اگر (نقطہ_گنتی)
	*نقطہ_گنتی = 0;
      واپس 0;
    }

    ثابت Attach_نقطہ &points = یہ+attach_نقطہ[اشاریہ_2];

    اگر (نقطہ_گنتی && نقطہ_array)
    {
      + points.as_array ().sub_array (آغاز_انحراف, نقطہ_گنتی)
      | hb_sink (hb_array (نقطہ_array, *نقطہ_گنتی))
      ;
    }

    واپس points.len;
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);
    ثابت hb_سیٹ_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خودکار *برآمد = c->serializer->آغاز_embed (*یہ);
    اگر (unlikely (!c->serializer->extend_min (برآمد))) return_trace (جھوٹ);

    hb_sorted_سمتیہ_t<hb_codepoint_t> new_coverage;
    + hb_zip (یہ+coverage, attach_نقطہ)
    | hb_filter (glyphset, hb_پہلا)
    | hb_filter (subset_انحراف_array (c, برآمد->attach_نقطہ, یہ), hb_دوسرا)
    | hb_map (hb_پہلا)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;
    برآمد->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (منطقی (new_coverage));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (coverage.sanitize (c, یہ) && attach_نقطہ.sanitize (c, یہ));
  }

  محفوظ:
  Offset16_مقصد<Coverage>
		coverage;		/* Offset to Coverage table -- from
					 * beginning of AttachList table */
  Array16_کا_Offset16_مقصد<Attach_نقطہ>
		attach_نقطہ;		/* Array of AttachPoint tables
					 * in Coverage Index order */
  عوامی:
  DEFINE_سائز_ARRAY (4, attach_نقطہ);
};

/*
 * Ligature Caret Table
 */

ساخت Caret_قدر_Format1
{
  دوست ساخت Caret_قدر;
  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);
    خودکار *برآمد = c->serializer->embed (یہ);
    اگر (unlikely (!برآمد)) return_trace (جھوٹ);
    return_trace (سچ);
  }

  نجی:
  hb_position_t get_caret_قدر (hb_font_t *font, hb_direction_t direction) ثابت
  {
    واپس HB_DIRECTION_IS_HORIZONTAL (direction) ? font->em_scale_x (coordinate) : font->em_scale_y (coordinate);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ));
  }

  محفوظ:
  HBUINT16	caret_قدر_Format;	/* Format identifier--format = 1 */
  FWORD		coordinate;		/* X or Y value, in design units */
  عوامی:
  DEFINE_سائز_STATIC (4);
};

ساخت Caret_قدر_Format2
{
  دوست ساخت Caret_قدر;
  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);
    خودکار *برآمد = c->serializer->embed (یہ);
    اگر (unlikely (!برآمد)) return_trace (جھوٹ);
    return_trace (سچ);
  }

  نجی:
  hb_position_t get_caret_قدر (hb_font_t *font, hb_direction_t direction, hb_codepoint_t glyph_id) ثابت
  {
    hb_position_t x, y;
    font->get_glyph_contour_نقطہ_for_origin (glyph_id, caret_قدر_نقطہ, direction, &x, &y);
    واپس HB_DIRECTION_IS_HORIZONTAL (direction) ? x : y;
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ));
  }

  محفوظ:
  HBUINT16	caret_قدر_Format;	/* Format identifier--format = 2 */
  HBUINT16	caret_قدر_نقطہ;	/* Contour point index on glyph */
  عوامی:
  DEFINE_سائز_STATIC (4);
};

ساخت Caret_قدر_Format3
{
  دوست ساخت Caret_قدر;

  hb_position_t get_caret_قدر (hb_font_t *font, hb_direction_t direction,
				 ثابت شے_Variation_Store &var_store) ثابت
  {
    واپس HB_DIRECTION_IS_HORIZONTAL (direction) ?
	   hb_saturate_جوڑو (font->em_scale_x (coordinate), (یہ+device_جدول).get_x_delta (font, var_store)) :
	   hb_saturate_جوڑو (font->em_scale_y (coordinate), (یہ+device_جدول).get_y_delta (font, var_store));
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);
    خودکار *برآمد = c->serializer->آغاز_embed (*یہ);
    اگر (!c->serializer->embed (caret_قدر_Format)) return_trace (جھوٹ);
    اگر (!c->serializer->embed (coordinate)) return_trace (جھوٹ);

    غیرعلامتی varidx = (یہ+device_جدول).get_variation_اشاریہ ();
    hb_pair_t<غیرعلامتی, صحیح> *new_varidx_delta;
    اگر (c->plan->layout_variation_idx_delta_map.has (varidx, &new_varidx_delta)) {
      uint32_t new_varidx = hb_پہلا (*new_varidx_delta);
      صحیح delta = hb_دوسرا (*new_varidx_delta);
      اگر (delta != 0)
      {
        اگر (!c->serializer->check_تفویض (برآمد->coordinate, coordinate + delta, HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز))
          return_trace (جھوٹ);
      }

      اگر (new_varidx == HB_OT_LAYOUT_NO_VARIATIONS_اشاریہ)
        return_trace (c->serializer->check_تفویض (برآمد->caret_قدر_Format, 1, HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز));
    }

    اگر (!c->serializer->embed (device_جدول))
      return_trace (جھوٹ);

    return_trace (برآمد->device_جدول.serialize_copy (c->serializer, device_جدول, یہ, c->serializer->مقصد_bias (برآمد),
						   hb_serialize_context_t::سر, &c->plan->layout_variation_idx_delta_map));
  }

  خالی collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  { (یہ+device_جدول).collect_variation_indices (c); }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) && device_جدول.sanitize (c, یہ));
  }

  محفوظ:
  HBUINT16	caret_قدر_Format;	/* Format identifier--format = 3 */
  FWORD		coordinate;		/* X or Y value, in design units */
  Offset16_مقصد<Device>
		device_جدول;		/* Offset to Device table for X or Y
					 * value--from beginning of CaretValue
					 * table */
  عوامی:
  DEFINE_سائز_STATIC (6);
};

ساخت Caret_قدر
{
  hb_position_t get_caret_قدر (hb_font_t *font,
				 hb_direction_t direction,
				 hb_codepoint_t glyph_id,
				 ثابت شے_Variation_Store &var_store) ثابت
  {
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); واپس u.format1.get_caret_قدر (font, direction);
    حالت 2: hb_barrier (); واپس u.format2.get_caret_قدر (font, direction, glyph_id);
    حالت 3: hb_barrier (); واپس u.format3.get_caret_قدر (font, direction, var_store);
    طےشدہ:واپس 0;
    }
  }

  سانچہ <قسم_نام context_t, قسم_نام ...Ts>
  قسم_نام context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    اگر (unlikely (!c->may_dispatch (یہ, &u.format.v))) واپس c->no_dispatch_return_قدر ();
    TRACE_DISPATCH (یہ, u.format.v);
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    حالت 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    حالت 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    طےشدہ:return_trace (c->default_return_قدر ());
    }
  }

  خالی collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    انتخاب (u.format.v) {
    حالت 1:
    حالت 2:
      واپس;
    حالت 3:
      hb_barrier ();
      u.format3.collect_variation_indices (c);
      واپس;
    طےشدہ: واپس;
    }
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    اگر (!u.format.v.sanitize (c)) return_trace (جھوٹ);
    hb_barrier ();
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    حالت 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    حالت 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    طےشدہ:return_trace (سچ);
    }
  }

  محفوظ:
  اتحاد {
  ساخت { HBUINT16 v; }	format;		/* Format identifier */
  Caret_قدر_Format1	format1;
  Caret_قدر_Format2	format2;
  Caret_قدر_Format3	format3;
  } u;
  عوامی:
  DEFINE_سائز_UNION (2, format.v);
};

ساخت LigGlyph
{
  غیرعلامتی get_lig_carets (hb_font_t            *font,
			   hb_direction_t        direction,
			   hb_codepoint_t        glyph_id,
			   ثابت شے_Variation_Store &var_store,
			   غیرعلامتی              آغاز_انحراف,
			   غیرعلامتی             *caret_گنتی /* IN/OUT */,
			   hb_position_t        *caret_array /*برآمد*/) ثابت
  {
    اگر (caret_گنتی && caret_array)
    {
      + carets.as_array ().sub_array (آغاز_انحراف, caret_گنتی)
      | hb_map (hb_جوڑو (یہ))
      | hb_map ([&] (ثابت Caret_قدر &value) { واپس value.get_caret_قدر (font, direction, glyph_id, var_store); })
      | hb_sink (hb_array (caret_array, *caret_گنتی))
      ;
    }

    واپس carets.len;
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);
    خودکار *برآمد = c->serializer->آغاز_embed (*یہ);
    اگر (unlikely (!c->serializer->extend_min (برآمد))) return_trace (جھوٹ);

    + hb_iter (carets)
    | hb_apply (subset_انحراف_array (c, برآمد->carets, یہ))
    ;

    return_trace (منطقی (برآمد->carets));
  }

  خالی collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    برائے (ثابت Offset16_مقصد<Caret_قدر>& انحراف : carets.iter ())
      (یہ+انحراف).collect_variation_indices (c);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (carets.sanitize (c, یہ));
  }

  محفوظ:
  Array16_کا_Offset16_مقصد<Caret_قدر>
		carets;			/* Offset array of CaretValue tables
					 * --from beginning of LigGlyph table
					 * --in increasing coordinate order */
  عوامی:
  DEFINE_سائز_ARRAY (2, carets);
};

/* LigCaretList2 widens the LigCaretList coverage, count, and LigGlyph
 * offsets. The LigGlyph table itself keeps Offset16 caretValueOffsets. */
سانچہ <قسم_نام Types>
ساخت LigCaretList
{
  غیرعلامتی صحیح get_lig_carets (hb_font_t *font,
			       hb_direction_t direction,
			       hb_codepoint_t glyph_id,
			       ثابت شے_Variation_Store &var_store,
			       غیرعلامتی صحیح آغاز_انحراف,
			       غیرعلامتی صحیح *caret_گنتی /* IN/OUT */,
			       hb_position_t *caret_array /*برآمد*/) ثابت
  {
    غیرعلامتی صحیح اشاریہ_2 = (یہ+coverage).get_coverage (glyph_id);
    اگر (اشاریہ_2 == NOT_COVERED)
    {
      اگر (caret_گنتی)
	*caret_گنتی = 0;
      واپس 0;
    }
    ثابت LigGlyph &lig_glyph = یہ+ligGlyph[اشاریہ_2];
    واپس lig_glyph.get_lig_carets (font, direction, glyph_id, var_store, آغاز_انحراف, caret_گنتی, caret_array);
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);
    ثابت hb_سیٹ_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خودکار *برآمد = c->serializer->آغاز_embed (*یہ);
    اگر (unlikely (!c->serializer->extend_min (برآمد))) return_trace (جھوٹ);

    hb_sorted_سمتیہ_t<hb_codepoint_t> new_coverage;
    + hb_zip (یہ+coverage, ligGlyph)
    | hb_filter (glyphset, hb_پہلا)
    | hb_filter (subset_انحراف_array (c, برآمد->ligGlyph, یہ), hb_دوسرا)
    | hb_map (hb_پہلا)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;
    برآمد->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (منطقی (new_coverage));
  }

  خالی collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    + hb_zip (یہ+coverage, ligGlyph)
    | hb_filter (c->glyph_سیٹ, hb_پہلا)
    | hb_map (hb_دوسرا)
    | hb_map (hb_جوڑو (یہ))
    | hb_apply ([c] (ثابت LigGlyph& _) { _.collect_variation_indices (c); })
    ;
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (coverage.sanitize (c, یہ) && ligGlyph.sanitize (c, یہ));
  }

  محفوظ:
  قسم_نام Types::سانچہ LOffset_مقصد<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of LigCaretList table */
  قسم_نام Types::سانچہ Array_کا<قسم_نام Types::سانچہ انحراف_مقصد<LigGlyph>>
		ligGlyph;		/* Array of LigGlyph tables
					 * in Coverage Index order */
  عوامی:
  DEFINE_سائز_ARRAY (Types::LOffset::static_سائز + Types::HBUINT::static_سائز, ligGlyph);
};

#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
static_assert (LigCaretList<MediumTypes>::min_سائز ==
	       Offset32::static_سائز + HBUINT24::static_سائز, "");
#اگر_ختم


ساخت MarkGlyphSetsFormat1
{
  منطقی covers (غیرعلامتی صحیح سیٹ_اشاریہ, hb_codepoint_t glyph_id) ثابت
  { واپس (یہ+coverage[سیٹ_اشاریہ]).get_coverage (glyph_id) != NOT_COVERED; }

  خالی collect_مستعمل_mark_sets (ثابت hb_سیٹ_t& glyph_سیٹ,
                               hb_سیٹ_t& مستعمل_mark_sets /*برآمد*/) ثابت
  {
    غیرعلامتی i = 0;
    برائے (ثابت خودکار &انحراف : coverage)
     {
       ثابت خودکار &cov = یہ+انحراف;
       اگر (cov.intersects (&glyph_سیٹ))
         مستعمل_mark_sets.جوڑو (i);

       i++;
     }
  }

  سانچہ <قسم_نام سیٹ_t>
  خالی collect_coverage (hb_سمتیہ_t<سیٹ_t> &sets) ثابت
  {
     برائے (ثابت خودکار &انحراف : coverage)
     {
       ثابت خودکار &cov = یہ+انحراف;
       cov.collect_coverage (sets.push ());
     }
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);
    خودکار *برآمد = c->serializer->آغاز_embed (*یہ);
    اگر (unlikely (!c->serializer->extend_min (برآمد))) return_trace (جھوٹ);
    برآمد->format = format;

    منطقی ret = سچ;
    برائے (ثابت Offset32_مقصد<Coverage>& انحراف : coverage.iter ())
    {
      خودکار snap = c->serializer->snapshot ();
      خودکار *o = برآمد->coverage.serialize_append (c->serializer);
      اگر (unlikely (!o))
      {
	ret = جھوٹ;
	توڑو;
      }

      //skip empty coverage
      c->serializer->push ();
      منطقی res = جھوٹ;
      اگر (انحراف) res = c->dispatch (یہ+انحراف);
      اگر (!res)
      {
        c->serializer->نکالو_discard ();
        c->serializer->revert (snap);
        (برآمد->coverage.len)--;
        جاری;
      }
      c->serializer->جوڑو_link (*o, c->serializer->نکالو_pack ());
    }

    return_trace (ret && برآمد->coverage.len);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (coverage.sanitize (c, یہ));
  }

  محفوظ:
  HBUINT16	format;			/* Format identifier--format = 1 */
  Array16_کا<Offset32_مقصد<Coverage>>
		coverage;		/* Array of long offsets to mark set
					 * coverage tables */
  عوامی:
  DEFINE_سائز_ARRAY (4, coverage);
};

ساخت MarkGlyphSets
{
  منطقی covers (غیرعلامتی صحیح سیٹ_اشاریہ, hb_codepoint_t glyph_id) ثابت
  {
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); واپس u.format1.covers (سیٹ_اشاریہ, glyph_id);
    طےشدہ:واپس جھوٹ;
    }
  }

  سانچہ <قسم_نام سیٹ_t>
  خالی collect_coverage (hb_سمتیہ_t<سیٹ_t> &sets) ثابت
  {
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); u.format1.collect_coverage (sets); واپس;
    طےشدہ:واپس;
    }
  }

  خالی collect_مستعمل_mark_sets (ثابت hb_سیٹ_t& glyph_سیٹ,
                               hb_سیٹ_t& مستعمل_mark_sets /*برآمد*/) ثابت
  {
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); u.format1.collect_مستعمل_mark_sets (glyph_سیٹ, مستعمل_mark_sets); واپس;
    طےشدہ:واپس;
    }
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); return_trace (u.format1.subset (c));
    طےشدہ:return_trace (جھوٹ);
    }
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    اگر (!u.format.v.sanitize (c)) return_trace (جھوٹ);
    hb_barrier ();
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    طےشدہ:return_trace (سچ);
    }
  }

  محفوظ:
  اتحاد {
  ساخت { HBUINT16 v; }	format;		/* Format identifier */
  MarkGlyphSetsFormat1	format1;
  } u;
  عوامی:
  DEFINE_سائز_UNION (2, format.v);
};


/*
 * GDEF -- Glyph Definition
 * https://docs.microsoft.com/en-us/typography/opentype/spec/gdef
 */


ساخت GDEF
{
  ساکن constexpr hb_لیبل_t جدول_لیبل = HB_OT_لیبل_GDEF;

  شمار GlyphClasses {
    UnclassifiedGlyph	= 0,
    بنیاد_Glyph		= 1,
    LigatureGlyph	= 2,
    MarkGlyph		= 3,
    ComponentGlyph	= 4
  };

  size_t get_سائز () ثابت
  {
    اگر (نسخہ.major != 1) واپس نسخہ.static_سائز;

    واپس min_سائز +
	   (نسخہ.مقصد_int () >= 0x00010002u ? markGlyphSetsDef.static_سائز : 0) +
	   (نسخہ.مقصد_int () >= 0x00010003u ? varStore.static_سائز : 0) +
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
	   (نسخہ.مقصد_int () >= 0x00010004u ? 5 * Offset32::static_سائز : 0)
#ورنہ
	   0
#اگر_ختم
	   ;
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    اگر (unlikely (!نسخہ.sanitize (c) ||
		  نسخہ.major != 1 ||
		  !c->check_range (یہ, get_سائز ())))
      return_trace (جھوٹ);
    hb_barrier ();

    منطقی glyph_class_def_ok = glyphClassDef.sanitize (c, یہ);
    منطقی attach_list_ok = attachList.sanitize (c, یہ);
    منطقی lig_caret_list_ok = ligCaretList.sanitize (c, یہ);
    منطقی mark_attach_class_def_ok = markAttachClassDef.sanitize (c, یہ);
    منطقی mark_glyph_sets_def_ok = نسخہ.مقصد_int () < 0x00010002u ||
				  markGlyphSetsDef.sanitize (c, یہ);

#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    اگر (نسخہ.مقصد_int () >= 0x00010004u)
    {
      اگر (glyphClassDef2) glyph_class_def_ok = glyphClassDef2.sanitize (c, یہ);
      اگر (attachList2) attach_list_ok = attachList2.sanitize (c, یہ);
      اگر (ligCaretList2) lig_caret_list_ok = ligCaretList2.sanitize (c, یہ);
      اگر (markAttachClassDef2) mark_attach_class_def_ok = markAttachClassDef2.sanitize (c, یہ);
      اگر (markGlyphSetsDef2) mark_glyph_sets_def_ok = markGlyphSetsDef2.sanitize (c, یہ);
    }
#اگر_ختم

    return_trace (glyph_class_def_ok &&
		  attach_list_ok &&
		  lig_caret_list_ok &&
		  mark_attach_class_def_ok &&
		  mark_glyph_sets_def_ok &&
		  (نسخہ.مقصد_int () < 0x00010003u || varStore.sanitize (c, یہ)));
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);
    اگر (نسخہ.major != 1) return_trace (جھوٹ);

    خودکار *برآمد = c->serializer->آغاز_embed (*یہ);
    اگر (unlikely (!c->serializer->extend_min (برآمد))) return_trace (جھوٹ);

    // Push var store first (if it's needed) so that it's last in the
    // serialization order. Some font consumers assume that varstore runs to
    // the end of the GDEF table.
    // See: https://github.com/harfbuzz/harfbuzz/issues/4636
    خودکار snapshot_version0 = c->serializer->snapshot ();
    اگر (unlikely (نسخہ.مقصد_int () >= 0x00010002u && hb_barrier () &&
		  !c->serializer->embed (markGlyphSetsDef)))
      return_trace (جھوٹ);

    خودکار snapshot_version2 = c->serializer->snapshot ();
    اگر (unlikely (نسخہ.مقصد_int () >= 0x00010003u && hb_barrier () &&
		  !c->serializer->embed (varStore)))
      return_trace (جھوٹ);

    منطقی use_v14 = جھوٹ;
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    use_v14 = نسخہ.مقصد_int () >= 0x00010004u &&
	      (glyphClassDef2 || attachList2 || ligCaretList2 ||
	       markAttachClassDef2 || markGlyphSetsDef2);
    اگر (unlikely (نسخہ.مقصد_int () >= 0x00010004u && hb_barrier () &&
		  !c->serializer->extend_سائز (برآمد, get_سائز ())))
      return_trace (جھوٹ);
#اگر_ختم

    منطقی subset_varstore = جھوٹ;
    غیرعلامتی varstore_اشاریہ = (غیرعلامتی) -1;
    اگر (نسخہ.مقصد_int () >= 0x00010003u && hb_barrier ())
    {
      اگر (c->plan->all_axes_pinned)
	برآمد->varStore = 0;
      ورنہ اگر (c->plan->normalized_coords)
      {
	اگر (varStore)
	{
	  شے_variations_t شے_vars;
	  اگر (unlikely (!c->serializer->check_success (
                  شے_vars.instantiate (یہ+varStore, c->plan, سچ, سچ,
                                          c->plan->gdef_varstore_inner_maps.as_array (), سچ))))
            return_trace (جھوٹ);
	  subset_varstore = برآمد->varStore.serialize_serialize (c->serializer,
							     شے_vars.has_long_لفظ (),
							     c->plan->axis_لیبل_فہرست,
							     شے_vars.get_region_list (),
							     شے_vars.get_vardata_encodings ());
	  varstore_اشاریہ = c->serializer->آخری_added_بچہ_اشاریہ();
	  remap_varidx_after_instantiation (شے_vars.get_varidx_map (),
					    c->plan->layout_variation_idx_delta_map);
          remap_varidx_after_instantiation (شے_vars.get_varidx_map (),
                                            c->plan->layout_condition_idx_قدر_map);
	}
      }
      ورنہ
      {
	subset_varstore = برآمد->varStore.serialize_subset (c, varStore, یہ,
							  c->plan->gdef_varstore_inner_maps.as_array ());
	varstore_اشاریہ = c->serializer->آخری_added_بچہ_اشاریہ();
      }
    }

    برآمد->نسخہ.major = نسخہ.major;
    برآمد->نسخہ.minor = نسخہ.minor;

    اگر (!subset_varstore && نسخہ.مقصد_int () >= 0x00010002u && !use_v14) {
      c->serializer->revert (snapshot_version2);
    }
    اگر (!subset_varstore)
      برآمد->varStore = 0;
    ورنہ
      c->plan->has_gdef_varstore = سچ;

    منطقی subset_markglyphsetsdef = جھوٹ;
    اگر (نسخہ.مقصد_int () >= 0x00010002u && hb_barrier ())
    {
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
      اگر (use_v14 && markGlyphSetsDef2)
      {
	برآمد->markGlyphSetsDef = 0;
	subset_markglyphsetsdef = serialize_subset_انحراف (c, برآمد->markGlyphSetsDef2,
							   markGlyphSetsDef2, یہ);
      }
      ورنہ
#اگر_ختم
      {
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
	اگر (use_v14) برآمد->markGlyphSetsDef2 = 0;
#اگر_ختم
	subset_markglyphsetsdef = برآمد->markGlyphSetsDef.serialize_subset (c, markGlyphSetsDef, یہ);
      }
    }

    اگر (use_v14)
    {
      برآمد->نسخہ.minor = 4;
    } ورنہ اگر (subset_varstore) {
      برآمد->نسخہ.minor = 3;
    } ورنہ اگر (subset_markglyphsetsdef) {
      برآمد->نسخہ.minor = 2;
    } ورنہ  {
      برآمد->نسخہ.minor = 0;
      c->serializer->revert (snapshot_version0);
    }

    منطقی subset_glyphclassdef = جھوٹ;
    منطقی subset_attachlist = جھوٹ;
    منطقی subset_markattachclassdef = جھوٹ;
    منطقی subset_ligcaretlist = جھوٹ;

#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    اگر (use_v14 && glyphClassDef2)
    {
      برآمد->glyphClassDef = 0;
      subset_glyphclassdef = serialize_subset_انحراف (c, برآمد->glyphClassDef2,
						      glyphClassDef2, یہ, خالی_اشاریہ, جھوٹ, سچ);
    }
    ورنہ
#اگر_ختم
    {
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
      اگر (use_v14) برآمد->glyphClassDef2 = 0;
#اگر_ختم
      subset_glyphclassdef = برآمد->glyphClassDef.serialize_subset (c, glyphClassDef, یہ, خالی_اشاریہ, جھوٹ, سچ);
    }

#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    اگر (use_v14 && attachList2)
    {
      برآمد->attachList = 0;
      subset_attachlist = serialize_subset_انحراف (c, برآمد->attachList2, attachList2, یہ);
    }
    ورنہ
#اگر_ختم
    {
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
      اگر (use_v14) برآمد->attachList2 = 0;
#اگر_ختم
      subset_attachlist = برآمد->attachList.serialize_subset (c, attachList, یہ);
    }

#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    اگر (use_v14 && markAttachClassDef2)
    {
      برآمد->markAttachClassDef = 0;
      subset_markattachclassdef = serialize_subset_انحراف (c, برآمد->markAttachClassDef2,
							   markAttachClassDef2, یہ, خالی_اشاریہ, جھوٹ, سچ);
    }
    ورنہ
#اگر_ختم
    {
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
      اگر (use_v14) برآمد->markAttachClassDef2 = 0;
#اگر_ختم
      subset_markattachclassdef = برآمد->markAttachClassDef.serialize_subset (c, markAttachClassDef,
									   یہ, خالی_اشاریہ, جھوٹ, سچ);
    }

#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    اگر (use_v14 && ligCaretList2)
    {
      برآمد->ligCaretList = 0;
      subset_ligcaretlist = serialize_subset_انحراف (c, برآمد->ligCaretList2, ligCaretList2, یہ);
    }
    ورنہ
#اگر_ختم
    {
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
      اگر (use_v14) برآمد->ligCaretList2 = 0;
#اگر_ختم
      subset_ligcaretlist = برآمد->ligCaretList.serialize_subset (c, ligCaretList, یہ);
    }

    اگر (subset_varstore && varstore_اشاریہ != (غیرعلامتی) -1) {
      c->serializer->repack_آخری(varstore_اشاریہ);
    }

    return_trace (subset_glyphclassdef || subset_attachlist ||
		  subset_ligcaretlist || subset_markattachclassdef ||
		  (برآمد->نسخہ.مقصد_int () >= 0x00010002u && subset_markglyphsetsdef) ||
		  (برآمد->نسخہ.مقصد_int () >= 0x00010003u && subset_varstore));
  }

  منطقی has_glyph_classes () ثابت
  {
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    اگر (نسخہ.مقصد_int () >= 0x00010004u && glyphClassDef2) واپس سچ;
#اگر_ختم
    واپس glyphClassDef != 0;
  }
  ثابت ClassDef &get_glyph_class_def () ثابت
  {
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    اگر (نسخہ.مقصد_int () >= 0x00010004u && glyphClassDef2) واپس یہ+glyphClassDef2;
#اگر_ختم
    واپس یہ+glyphClassDef;
  }
  منطقی has_attach_list () ثابت
  {
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    اگر (نسخہ.مقصد_int () >= 0x00010004u && attachList2) واپس سچ;
#اگر_ختم
    واپس attachList != 0;
  }
  ثابت AttachList &get_attach_list () ثابت
  {
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    اگر (نسخہ.مقصد_int () >= 0x00010004u && attachList2) واپس یہ+attachList2;
#اگر_ختم
    واپس یہ+attachList;
  }
  منطقی has_lig_carets () ثابت
  {
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    اگر (نسخہ.مقصد_int () >= 0x00010004u && ligCaretList2) واپس سچ;
#اگر_ختم
    واپس ligCaretList != 0;
  }
  منطقی has_mark_attachment_types () ثابت
  {
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    اگر (نسخہ.مقصد_int () >= 0x00010004u && markAttachClassDef2) واپس سچ;
#اگر_ختم
    واپس markAttachClassDef != 0;
  }
  ثابت ClassDef &get_mark_attach_class_def () ثابت
  {
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    اگر (نسخہ.مقصد_int () >= 0x00010004u && markAttachClassDef2) واپس یہ+markAttachClassDef2;
#اگر_ختم
    واپس یہ+markAttachClassDef;
  }
  منطقی has_mark_glyph_sets () ثابت
  {
    اگر (نسخہ.مقصد_int () < 0x00010002u) واپس جھوٹ;
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    اگر (نسخہ.مقصد_int () >= 0x00010004u && markGlyphSetsDef2) واپس سچ;
#اگر_ختم
    واپس markGlyphSetsDef != 0;
  }
  ثابت MarkGlyphSets &get_mark_glyph_sets () ثابت
  {
    اگر (نسخہ.مقصد_int () < 0x00010002u) واپس Null(MarkGlyphSets);
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    اگر (نسخہ.مقصد_int () >= 0x00010004u && markGlyphSetsDef2) واپس یہ+markGlyphSetsDef2;
#اگر_ختم
    واپس یہ+markGlyphSetsDef;
  }
  منطقی has_var_store () ثابت
  {
    واپس نسخہ.مقصد_int () >= 0x00010003u && hb_barrier () && varStore != 0;
  }
  ثابت شے_Variation_Store &get_var_store () ثابت
  {
    واپس نسخہ.مقصد_int () >= 0x00010003u && hb_barrier () ? یہ+varStore : Null(شے_Variation_Store);
  }


  منطقی has_ڈیٹا () ثابت { واپس نسخہ.مقصد_int (); }
  غیرعلامتی صحیح get_glyph_class (hb_codepoint_t glyph) ثابت
  { واپس get_glyph_class_def ().get_class (glyph); }
  خالی get_glyphs_اندر_class (غیرعلامتی صحیح klass, hb_سیٹ_t *glyphs) ثابت
  { get_glyph_class_def ().collect_class (glyphs, klass); }

  غیرعلامتی صحیح get_mark_attachment_type (hb_codepoint_t glyph) ثابت
  { واپس get_mark_attach_class_def ().get_class (glyph); }

  غیرعلامتی صحیح get_attach_points (hb_codepoint_t glyph_id,
				  غیرعلامتی صحیح آغاز_انحراف,
				  غیرعلامتی صحیح *نقطہ_گنتی /* IN/OUT */,
				  غیرعلامتی صحیح *نقطہ_array /*برآمد*/) ثابت
  { واپس get_attach_list ().get_attach_points (glyph_id, آغاز_انحراف, نقطہ_گنتی, نقطہ_array); }

  غیرعلامتی صحیح get_lig_carets (hb_font_t *font,
			       hb_direction_t direction,
			       hb_codepoint_t glyph_id,
			       غیرعلامتی صحیح آغاز_انحراف,
			       غیرعلامتی صحیح *caret_گنتی /* IN/OUT */,
			       hb_position_t *caret_array /*برآمد*/) ثابت
  {
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    اگر (نسخہ.مقصد_int () >= 0x00010004u && ligCaretList2)
      واپس (یہ+ligCaretList2).get_lig_carets (font, direction, glyph_id, get_var_store (),
						  آغاز_انحراف, caret_گنتی, caret_array);
#اگر_ختم
    واپس (یہ+ligCaretList).get_lig_carets (font, direction, glyph_id, get_var_store (),
					       آغاز_انحراف, caret_گنتی, caret_array);
  }

  منطقی mark_سیٹ_covers (غیرعلامتی صحیح سیٹ_اشاریہ, hb_codepoint_t glyph_id) ثابت
  { واپس get_mark_glyph_sets ().covers (سیٹ_اشاریہ, glyph_id); }

  /* glyph_props is a 16-bit integer where the lower 8-bit have bits representing
   * glyph class and other bits, and high 8-bit the mark attachment type (if any).
   * Not to be confused with lookup_props which is very similar. */
  غیرعلامتی صحیح get_glyph_props (hb_codepoint_t glyph) ثابت
  {
    غیرعلامتی صحیح klass = get_glyph_class (glyph);

    static_assert (((غیرعلامتی صحیح) HB_OT_LAYOUT_GLYPH_PROPS_بنیاد_GLYPH == (غیرعلامتی صحیح) LookupFlag::Ignore_بنیاد_Glyphs), "");
    static_assert (((غیرعلامتی صحیح) HB_OT_LAYOUT_GLYPH_PROPS_LIGATURE == (غیرعلامتی صحیح) LookupFlag::IgnoreLigatures), "");
    static_assert (((غیرعلامتی صحیح) HB_OT_LAYOUT_GLYPH_PROPS_MARK == (غیرعلامتی صحیح) LookupFlag::IgnoreMarks), "");

    انتخاب (klass) {
    طےشدہ:			واپس HB_OT_LAYOUT_GLYPH_CLASS_UNCLASSIFIED;
    حالت بنیاد_Glyph:		واپس HB_OT_LAYOUT_GLYPH_PROPS_بنیاد_GLYPH;
    حالت LigatureGlyph:		واپس HB_OT_LAYOUT_GLYPH_PROPS_LIGATURE;
    حالت MarkGlyph:
	  klass = get_mark_attachment_type (glyph);
	  واپس HB_OT_LAYOUT_GLYPH_PROPS_MARK | (klass << 8);
    }
  }

  HB_اندرونی منطقی is_blocklisted (hb_blob_t *blob,
				   hb_face_t *face) ثابت;

  ساخت accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      جدول = hb_sanitize_context_t ().reference_جدول<GDEF> (face);
      اگر (unlikely (جدول->is_blocklisted (جدول.get_blob (), face)))
      {
	hb_blob_destroy (جدول.get_blob ());
	جدول = hb_blob_get_خالی_خانے ();
      }

#اگر_غیر_تعریف_شدہ HB_NO_GDEF_CACHE
      /* Flatten each mark set whose glyphs span fewer than PAGE_BITS
       * gids into a single bit-page biased at its first glyph, for
       * O(1) exact answers.  Wider sets, or on allocation failure,
       * fall back to the table. */
      {
	hb_سمتیہ_t<hb_سیٹ_t> sets;
	جدول->get_mark_glyph_sets ().collect_coverage (sets);
	اگر (!sets.اندر_error () &&
	    mark_glyph_سیٹ_bitmaps.resize (sets.لمبائی))
	  برائے (غیرعلامتی i = 0; i < sets.لمبائی; i++)
	  {
	    خودکار &bitmap = mark_glyph_سیٹ_bitmaps.arrayZ[i];
	    bitmap.bias = HB_سیٹ_قدر_INVALID;
	    ثابت خودکار &سیٹ = sets.arrayZ[i];
	    اگر (unlikely (سیٹ.اندر_error ())) جاری;
	    hb_codepoint_t min_gid = HB_سیٹ_قدر_INVALID;
	    hb_codepoint_t max_gid = HB_سیٹ_قدر_INVALID;
	    hb_سیٹ_اگلا (&سیٹ, &min_gid);
	    hb_سیٹ_پچھلا (&سیٹ, &max_gid);
	    اگر (max_gid == HB_سیٹ_قدر_INVALID)
	      جاری;
	    اگر (max_gid - min_gid >= hb_ثنائی_ہندسہ_صفحہ_t::صفحہ_بٹ)
	    {
	      /* Too wide for one page; a digest pre-filter instead. */
	      برائے (hb_codepoint_t gid : سیٹ)
		bitmap.u.digest.جوڑو (gid);
	      جاری;
	    }
	    bitmap.bias = min_gid;
	    برائے (hb_codepoint_t gid : سیٹ)
	      bitmap.u.صفحہ.جوڑو (gid - min_gid);
	  }
      }
#اگر_ختم
    }
    ~accelerator_t () { جدول.destroy (); }

    غیرعلامتی صحیح get_glyph_props (hb_codepoint_t glyph) ثابت
    {
      غیرعلامتی v;

#اگر_غیر_تعریف_شدہ HB_NO_GDEF_CACHE
      اگر (glyph_props_cache.get (glyph, &v))
        واپس v;
#اگر_ختم

      v = جدول->get_glyph_props (glyph);

#اگر_غیر_تعریف_شدہ HB_NO_GDEF_CACHE
      اگر (likely (جدول.get_blob ())) // Don't try setting if we are the null instance!
	glyph_props_cache.سیٹ (glyph, v);
#اگر_ختم

      واپس v;

    }

    HB_ALWAYS_INLINE
    منطقی mark_سیٹ_covers (غیرعلامتی صحیح سیٹ_اشاریہ, hb_codepoint_t glyph_id) ثابت
    {
#اگر_غیر_تعریف_شدہ HB_NO_GDEF_CACHE
      // We can access arrayZ directly because of sanitize_lookup_props() guarantee.
      ثابت خودکار &bitmap = mark_glyph_سیٹ_bitmaps.arrayZ[سیٹ_اشاریہ];
      اگر (likely (bitmap.bias != HB_سیٹ_قدر_INVALID))
      {
	hb_codepoint_t biased = glyph_id - bitmap.bias;
	واپس biased < hb_ثنائی_ہندسہ_صفحہ_t::صفحہ_بٹ &&
	       bitmap.u.صفحہ.get (biased);
      }
      واپس mark_سیٹ_covers_slow (سیٹ_اشاریہ, glyph_id);
#ورنہ
      واپس جدول->mark_سیٹ_covers (سیٹ_اشاریہ, glyph_id);
#اگر_ختم
    }

#اگر_غیر_تعریف_شدہ HB_NO_GDEF_CACHE
    HB_NEVER_INLINE
    منطقی mark_سیٹ_covers_slow (غیرعلامتی صحیح سیٹ_اشاریہ, hb_codepoint_t glyph_id) ثابت
    {
      ثابت خودکار &bitmap = mark_glyph_سیٹ_bitmaps.arrayZ[سیٹ_اشاریہ];
      اگر (!bitmap.u.digest.may_have (glyph_id))
	واپس جھوٹ;
      واپس جدول->mark_سیٹ_covers (سیٹ_اشاریہ, glyph_id);
    }
#اگر_ختم

    غیرعلامتی sanitize_lookup_props (غیرعلامتی lookup_props) ثابت
    {
#اگر_غیر_تعریف_شدہ HB_NO_GDEF_CACHE
      اگر (lookup_props & LookupFlag::Use_Mark_Filtering_سیٹ &&
	  (lookup_props >> 16) >= mark_glyph_سیٹ_bitmaps.لمبائی)
      {
        // Invalid mark filtering set index; unset the flag.
	lookup_props &= ~LookupFlag::Use_Mark_Filtering_سیٹ;
      }
#اگر_ختم
      واپس lookup_props;
    }

    hb_blob_ptr_t<GDEF> جدول;
#اگر_غیر_تعریف_شدہ HB_NO_GDEF_CACHE
    ساخت mark_glyph_سیٹ_bitmap_t
    {
      /* Gid of the first bit (not page-aligned) for an exact one-page
       * set, or HB_SET_VALUE_INVALID; then digest is a pre-filter for
       * the table.  The all-zeros state is valid for both members. */
      hb_codepoint_t bias = HB_سیٹ_قدر_INVALID;
      اتحاد u_t {
	u_t () : صفحہ () {}
	hb_ثنائی_ہندسہ_صفحہ_t صفحہ;
	hb_سیٹ_digest_t digest;
      } u;
    };

    hb_سمتیہ_t<mark_glyph_سیٹ_bitmap_t> mark_glyph_سیٹ_bitmaps;
    تغیرپذیر hb_cache_t<21, 3> glyph_props_cache;
    static_assert (حجم (glyph_props_cache) == 512, "");
#اگر_ختم
  };

  خالی collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    اگر (نسخہ.مقصد_int () >= 0x00010004u && ligCaretList2)
    {
      (یہ+ligCaretList2).collect_variation_indices (c);
      واپس;
    }
#اگر_ختم
    (یہ+ligCaretList).collect_variation_indices (c);
  }

  محفوظ:
  سانچہ <قسم_نام کلید>
  ساکن خالی remap_varidx_after_instantiation (ثابت hb_map_t& varidx_map,
                                                hb_hashmap_t<کلید, hb_pair_t<غیرعلامتی, صحیح>>& layout_variation_idx_delta_map /* IN/OUT */)
  {
    /* varidx_map is empty which means varstore is empty after instantiation,
     * no variations, map all varidx to HB_OT_LAYOUT_NO_VARIATIONS_INDEX.
     * varidx_map doesn't have original varidx, indicating delta row is all
     * zeros, map varidx to HB_OT_LAYOUT_NO_VARIATIONS_INDEX */
    برائے (خودکار _ : layout_variation_idx_delta_map.iter_ref ())
    {
      /* old_varidx->(varidx, delta) mapping generated for subsetting, then this
       * varidx is used as key of varidx_map during instantiation */
      uint32_t varidx = _.second.پہلا;
      uint32_t *new_varidx;
      اگر (varidx_map.has (varidx, &new_varidx))
	_.second.پہلا = *new_varidx;
      ورنہ
	_.second.پہلا = HB_OT_LAYOUT_NO_VARIATIONS_اشاریہ;
    }
  }

  سانچہ <قسم_نام انحراف_برآمد, قسم_نام انحراف_اندر, قسم_نام بنیاد_3, قسم_نام ...Ts>
  ساکن منطقی serialize_subset_انحراف (hb_subset_context_t *c,
				       انحراف_برآمد& برآمد,
				       ثابت انحراف_اندر& اندر,
				       ثابت بنیاد_3 *بنیاد,
				       Ts&&... ds)
  {
    برآمد = 0;
    اگر (اندر.is_null ()) واپس جھوٹ;

    خودکار *s = c->serializer;
    s->push ();
    منطقی ret = c->dispatch (بنیاد+اندر, std::forward<Ts> (ds)...);
    اگر (ret) s->جوڑو_link (برآمد, s->نکالو_pack ());
    ورنہ s->نکالو_discard ();
    واپس ret;
  }

  Fixed_نسخہ<>نسخہ;		/* Version of the GDEF table--currently
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
  Offset32_مقصد<شے_Variation_Store>
		varStore;		/* Offset to the table of Item Variation
					 * Store--from beginning of GDEF
					 * header (may be NULL).  Introduced
					 * in version 0x00010003. */
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
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
#اگر_ختم
  عوامی:
  DEFINE_سائز_MIN (12);
};

ساخت GDEF_accelerator_t : GDEF::accelerator_t {
  GDEF_accelerator_t (hb_face_t *face) : GDEF::accelerator_t (face) {}
};

} /* namespace OT */


#اگر_ختم /* OT_LAYOUT_GDEF_GDEF_HH */
