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

#جے_بنا_تعریف OT_LAYOUT_GDEF_GDEF_HH
#تعریف_کرو OT_LAYOUT_GDEF_GDEF_HH

#ضمن "../../../hb-ot-var-common.hh"

#ضمن "../../../hb-font.hh"
#ضمن "../../../hb-cache.hh"


نطاق OT {


/*
 * Attachment List Table
 */

/* Array of contour point indices--in increasing numerical order */
بنية Attach_نقطہ : Array16_دا<HBUINT16>
{
  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *باہرلا = c->serializer->شروع_embed (*هذا);
    return_trace (باہرلا->serialize (c->serializer, + iter ()));
  }
};

/* AttachList is referenced by both GDEF attachListOffset and
 * attachListOffset2. The v1.4 top-level offset widens, but the
 * AttachList table itself keeps Offset16 internals. */
بنية AttachList
{
  بنانشانی صحيح get_attach_points (hb_codepoint_t glyph_id,
				  بنانشانی صحيح شروع_وتھ,
				  بنانشانی صحيح *نقطہ_گنتی /* IN/OUT */,
				  بنانشانی صحيح *نقطہ_array /*باہرلا*/) ثابت
  {
    بنانشانی صحيح اشاریہ_2 = (هذا+coverage).get_coverage (glyph_id);
    إذا (اشاریہ_2 == NOT_COVERED)
    {
      إذا (نقطہ_گنتی)
	*نقطہ_گنتی = 0;
      أرجع 0;
    }

    ثابت Attach_نقطہ &points = هذا+attach_نقطہ[اشاریہ_2];

    إذا (نقطہ_گنتی && نقطہ_array)
    {
      + points.as_array ().sub_array (شروع_وتھ, نقطہ_گنتی)
      | hb_sink (hb_array (نقطہ_array, *نقطہ_گنتی))
      ;
    }

    أرجع points.len;
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_سیٹ_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خودکار *باہرلا = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (باہرلا))) return_trace (خطأ);

    hb_sorted_سمتیہ_t<hb_codepoint_t> new_coverage;
    + hb_zip (هذا+coverage, attach_نقطہ)
    | hb_filter (glyphset, hb_پہلا)
    | hb_filter (subset_وتھ_array (c, باہرلا->attach_نقطہ, هذا), hb_دوجا)
    | hb_map (hb_پہلا)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;
    باہرلا->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (منطقي (new_coverage));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (coverage.sanitize (c, هذا) && attach_نقطہ.sanitize (c, هذا));
  }

  محمي:
  Offset16_منزل<Coverage>
		coverage;		/* Offset to Coverage table -- from
					 * beginning of AttachList table */
  Array16_دا_Offset16_منزل<Attach_نقطہ>
		attach_نقطہ;		/* Array of AttachPoint tables
					 * in Coverage Index order */
  عام:
  DEFINE_ناپ_ARRAY (4, attach_نقطہ);
};

/*
 * Ligature Caret Table
 */

بنية Caret_قدر_Format1
{
  یار بنية Caret_قدر;
  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *باہرلا = c->serializer->embed (هذا);
    إذا (unlikely (!باہرلا)) return_trace (خطأ);
    return_trace (سچ);
  }

  خاص:
  hb_position_t get_caret_قدر (hb_font_t *font, hb_direction_t direction) ثابت
  {
    أرجع HB_DIRECTION_IS_HORIZONTAL (direction) ? font->em_scale_x (coordinate) : font->em_scale_y (coordinate);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  محمي:
  HBUINT16	caret_قدر_Format;	/* Format identifier--format = 1 */
  FWORD		coordinate;		/* X or Y value, in design units */
  عام:
  DEFINE_ناپ_STATIC (4);
};

بنية Caret_قدر_Format2
{
  یار بنية Caret_قدر;
  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *باہرلا = c->serializer->embed (هذا);
    إذا (unlikely (!باہرلا)) return_trace (خطأ);
    return_trace (سچ);
  }

  خاص:
  hb_position_t get_caret_قدر (hb_font_t *font, hb_direction_t direction, hb_codepoint_t glyph_id) ثابت
  {
    hb_position_t x, y;
    font->get_glyph_contour_نقطہ_for_origin (glyph_id, caret_قدر_نقطہ, direction, &x, &y);
    أرجع HB_DIRECTION_IS_HORIZONTAL (direction) ? x : y;
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  محمي:
  HBUINT16	caret_قدر_Format;	/* Format identifier--format = 2 */
  HBUINT16	caret_قدر_نقطہ;	/* Contour point index on glyph */
  عام:
  DEFINE_ناپ_STATIC (4);
};

بنية Caret_قدر_Format3
{
  یار بنية Caret_قدر;

  hb_position_t get_caret_قدر (hb_font_t *font, hb_direction_t direction,
				 ثابت شے_Variation_Store &var_store) ثابت
  {
    أرجع HB_DIRECTION_IS_HORIZONTAL (direction) ?
	   hb_saturate_جوڑو (font->em_scale_x (coordinate), (هذا+device_جدول).get_x_delta (font, var_store)) :
	   hb_saturate_جوڑو (font->em_scale_y (coordinate), (هذا+device_جدول).get_y_delta (font, var_store));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *باہرلا = c->serializer->شروع_embed (*هذا);
    إذا (!c->serializer->embed (caret_قدر_Format)) return_trace (خطأ);
    إذا (!c->serializer->embed (coordinate)) return_trace (خطأ);

    بنانشانی varidx = (هذا+device_جدول).get_variation_اشاریہ ();
    hb_pair_t<بنانشانی, صحيح> *new_varidx_delta;
    إذا (c->plan->layout_variation_idx_delta_map.has (varidx, &new_varidx_delta)) {
      uint32_t new_varidx = hb_پہلا (*new_varidx_delta);
      صحيح delta = hb_دوجا (*new_varidx_delta);
      إذا (delta != 0)
      {
        إذا (!c->serializer->check_سونپو (باہرلا->coordinate, coordinate + delta, HB_SERIALIZE_ERROR_INT_حد_توں_ودھ))
          return_trace (خطأ);
      }

      إذا (new_varidx == HB_OT_LAYOUT_NO_VARIATIONS_اشاریہ)
        return_trace (c->serializer->check_سونپو (باہرلا->caret_قدر_Format, 1, HB_SERIALIZE_ERROR_INT_حد_توں_ودھ));
    }

    إذا (!c->serializer->embed (device_جدول))
      return_trace (خطأ);

    return_trace (باہرلا->device_جدول.serialize_copy (c->serializer, device_جدول, هذا, c->serializer->منزل_bias (باہرلا),
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
  HBUINT16	caret_قدر_Format;	/* Format identifier--format = 3 */
  FWORD		coordinate;		/* X or Y value, in design units */
  Offset16_منزل<Device>
		device_جدول;		/* Offset to Device table for X or Y
					 * value--from beginning of CaretValue
					 * table */
  عام:
  DEFINE_ناپ_STATIC (6);
};

بنية Caret_قدر
{
  hb_position_t get_caret_قدر (hb_font_t *font,
				 hb_direction_t direction,
				 hb_codepoint_t glyph_id,
				 ثابت شے_Variation_Store &var_store) ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.get_caret_قدر (font, direction);
    حالة 2: hb_barrier (); أرجع u.format2.get_caret_قدر (font, direction, glyph_id);
    حالة 3: hb_barrier (); أرجع u.format3.get_caret_قدر (font, direction, var_store);
    افتراضي:أرجع 0;
    }
  }

  سانچا <قسم_ناں context_t, قسم_ناں ...Ts>
  قسم_ناں context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    إذا (unlikely (!c->may_dispatch (هذا, &u.format.v))) أرجع c->no_dispatch_return_قدر ();
    TRACE_DISPATCH (هذا, u.format.v);
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    حالة 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    حالة 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    افتراضي:return_trace (c->default_return_قدر ());
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
    افتراضي:return_trace (سچ);
    }
  }

  محمي:
  اتحاد {
  بنية { HBUINT16 v; }	format;		/* Format identifier */
  Caret_قدر_Format1	format1;
  Caret_قدر_Format2	format2;
  Caret_قدر_Format3	format3;
  } u;
  عام:
  DEFINE_ناپ_UNION (2, format.v);
};

بنية LigGlyph
{
  بنانشانی get_lig_carets (hb_font_t            *font,
			   hb_direction_t        direction,
			   hb_codepoint_t        glyph_id,
			   ثابت شے_Variation_Store &var_store,
			   بنانشانی              شروع_وتھ,
			   بنانشانی             *caret_گنتی /* IN/OUT */,
			   hb_position_t        *caret_array /*باہرلا*/) ثابت
  {
    إذا (caret_گنتی && caret_array)
    {
      + carets.as_array ().sub_array (شروع_وتھ, caret_گنتی)
      | hb_map (hb_جوڑو (هذا))
      | hb_map ([&] (ثابت Caret_قدر &value) { أرجع value.get_caret_قدر (font, direction, glyph_id, var_store); })
      | hb_sink (hb_array (caret_array, *caret_گنتی))
      ;
    }

    أرجع carets.len;
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *باہرلا = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (باہرلا))) return_trace (خطأ);

    + hb_iter (carets)
    | hb_apply (subset_وتھ_array (c, باہرلا->carets, هذا))
    ;

    return_trace (منطقي (باہرلا->carets));
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    لكل (ثابت Offset16_منزل<Caret_قدر>& وتھ : carets.iter ())
      (هذا+وتھ).collect_variation_indices (c);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (carets.sanitize (c, هذا));
  }

  محمي:
  Array16_دا_Offset16_منزل<Caret_قدر>
		carets;			/* Offset array of CaretValue tables
					 * --from beginning of LigGlyph table
					 * --in increasing coordinate order */
  عام:
  DEFINE_ناپ_ARRAY (2, carets);
};

/* LigCaretList2 widens the LigCaretList coverage, count, and LigGlyph
 * offsets. The LigGlyph table itself keeps Offset16 caretValueOffsets. */
سانچا <قسم_ناں Types>
بنية LigCaretList
{
  بنانشانی صحيح get_lig_carets (hb_font_t *font,
			       hb_direction_t direction,
			       hb_codepoint_t glyph_id,
			       ثابت شے_Variation_Store &var_store,
			       بنانشانی صحيح شروع_وتھ,
			       بنانشانی صحيح *caret_گنتی /* IN/OUT */,
			       hb_position_t *caret_array /*باہرلا*/) ثابت
  {
    بنانشانی صحيح اشاریہ_2 = (هذا+coverage).get_coverage (glyph_id);
    إذا (اشاریہ_2 == NOT_COVERED)
    {
      إذا (caret_گنتی)
	*caret_گنتی = 0;
      أرجع 0;
    }
    ثابت LigGlyph &lig_glyph = هذا+ligGlyph[اشاریہ_2];
    أرجع lig_glyph.get_lig_carets (font, direction, glyph_id, var_store, شروع_وتھ, caret_گنتی, caret_array);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_سیٹ_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خودکار *باہرلا = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (باہرلا))) return_trace (خطأ);

    hb_sorted_سمتیہ_t<hb_codepoint_t> new_coverage;
    + hb_zip (هذا+coverage, ligGlyph)
    | hb_filter (glyphset, hb_پہلا)
    | hb_filter (subset_وتھ_array (c, باہرلا->ligGlyph, هذا), hb_دوجا)
    | hb_map (hb_پہلا)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;
    باہرلا->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (منطقي (new_coverage));
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    + hb_zip (هذا+coverage, ligGlyph)
    | hb_filter (c->glyph_سیٹ, hb_پہلا)
    | hb_map (hb_دوجا)
    | hb_map (hb_جوڑو (هذا))
    | hb_apply ([c] (ثابت LigGlyph& _) { _.collect_variation_indices (c); })
    ;
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (coverage.sanitize (c, هذا) && ligGlyph.sanitize (c, هذا));
  }

  محمي:
  قسم_ناں Types::سانچا LOffset_منزل<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of LigCaretList table */
  قسم_ناں Types::سانچا Array_دا<قسم_ناں Types::سانچا وتھ_منزل<LigGlyph>>
		ligGlyph;		/* Array of LigGlyph tables
					 * in Coverage Index order */
  عام:
  DEFINE_ناپ_ARRAY (Types::LOffset::static_ناپ + Types::HBUINT::static_ناپ, ligGlyph);
};

#جے_بنا_تعریف HB_NO_BEYOND_64K
static_assert (LigCaretList<MediumTypes>::min_ناپ ==
	       Offset32::static_ناپ + HBUINT24::static_ناپ, "");
#جے_انت


بنية MarkGlyphSetsFormat1
{
  منطقي covers (بنانشانی صحيح سیٹ_اشاریہ, hb_codepoint_t glyph_id) ثابت
  { أرجع (هذا+coverage[سیٹ_اشاریہ]).get_coverage (glyph_id) != NOT_COVERED; }

  فراغ collect_ورتیا_mark_sets (ثابت hb_سیٹ_t& glyph_سیٹ,
                               hb_سیٹ_t& ورتیا_mark_sets /*باہرلا*/) ثابت
  {
    بنانشانی i = 0;
    لكل (ثابت خودکار &وتھ : coverage)
     {
       ثابت خودکار &cov = هذا+وتھ;
       إذا (cov.intersects (&glyph_سیٹ))
         ورتیا_mark_sets.جوڑو (i);

       i++;
     }
  }

  سانچا <قسم_ناں سیٹ_t>
  فراغ collect_coverage (hb_سمتیہ_t<سیٹ_t> &sets) ثابت
  {
     لكل (ثابت خودکار &وتھ : coverage)
     {
       ثابت خودکار &cov = هذا+وتھ;
       cov.collect_coverage (sets.push ());
     }
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *باہرلا = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (باہرلا))) return_trace (خطأ);
    باہرلا->format = format;

    منطقي ret = سچ;
    لكل (ثابت Offset32_منزل<Coverage>& وتھ : coverage.iter ())
    {
      خودکار snap = c->serializer->snapshot ();
      خودکار *o = باہرلا->coverage.serialize_append (c->serializer);
      إذا (unlikely (!o))
      {
	ret = خطأ;
	اكسر;
      }

      //skip empty coverage
      c->serializer->push ();
      منطقي res = خطأ;
      إذا (وتھ) res = c->dispatch (هذا+وتھ);
      إذا (!res)
      {
        c->serializer->نکالو_discard ();
        c->serializer->revert (snap);
        (باہرلا->coverage.len)--;
        استمر;
      }
      c->serializer->جوڑو_link (*o, c->serializer->نکالو_pack ());
    }

    return_trace (ret && باہرلا->coverage.len);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (coverage.sanitize (c, هذا));
  }

  محمي:
  HBUINT16	format;			/* Format identifier--format = 1 */
  Array16_دا<Offset32_منزل<Coverage>>
		coverage;		/* Array of long offsets to mark set
					 * coverage tables */
  عام:
  DEFINE_ناپ_ARRAY (4, coverage);
};

بنية MarkGlyphSets
{
  منطقي covers (بنانشانی صحيح سیٹ_اشاریہ, hb_codepoint_t glyph_id) ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.covers (سیٹ_اشاریہ, glyph_id);
    افتراضي:أرجع خطأ;
    }
  }

  سانچا <قسم_ناں سیٹ_t>
  فراغ collect_coverage (hb_سمتیہ_t<سیٹ_t> &sets) ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); u.format1.collect_coverage (sets); أرجع;
    افتراضي:أرجع;
    }
  }

  فراغ collect_ورتیا_mark_sets (ثابت hb_سیٹ_t& glyph_سیٹ,
                               hb_سیٹ_t& ورتیا_mark_sets /*باہرلا*/) ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); u.format1.collect_ورتیا_mark_sets (glyph_سیٹ, ورتیا_mark_sets); أرجع;
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
    افتراضي:return_trace (سچ);
    }
  }

  محمي:
  اتحاد {
  بنية { HBUINT16 v; }	format;		/* Format identifier */
  MarkGlyphSetsFormat1	format1;
  } u;
  عام:
  DEFINE_ناپ_UNION (2, format.v);
};


/*
 * GDEF -- Glyph Definition
 * https://docs.microsoft.com/en-us/typography/opentype/spec/gdef
 */


بنية GDEF
{
  ساكن constexpr hb_ٹیگ_t جدول_ٹیگ = HB_OT_ٹیگ_GDEF;

  تعداد GlyphClasses {
    UnclassifiedGlyph	= 0,
    ادھار_Glyph		= 1,
    LigatureGlyph	= 2,
    MarkGlyph		= 3,
    ComponentGlyph	= 4
  };

  size_t get_ناپ () ثابت
  {
    إذا (نسخہ.major != 1) أرجع نسخہ.static_ناپ;

    أرجع min_ناپ +
	   (نسخہ.منزل_int () >= 0x00010002u ? markGlyphSetsDef.static_ناپ : 0) +
	   (نسخہ.منزل_int () >= 0x00010003u ? varStore.static_ناپ : 0) +
#جے_بنا_تعریف HB_NO_BEYOND_64K
	   (نسخہ.منزل_int () >= 0x00010004u ? 5 * Offset32::static_ناپ : 0)
#وإلا
	   0
#جے_انت
	   ;
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!نسخہ.sanitize (c) ||
		  نسخہ.major != 1 ||
		  !c->check_range (هذا, get_ناپ ())))
      return_trace (خطأ);
    hb_barrier ();

    منطقي glyph_class_def_ok = glyphClassDef.sanitize (c, هذا);
    منطقي attach_list_ok = attachList.sanitize (c, هذا);
    منطقي lig_caret_list_ok = ligCaretList.sanitize (c, هذا);
    منطقي mark_attach_class_def_ok = markAttachClassDef.sanitize (c, هذا);
    منطقي mark_glyph_sets_def_ok = نسخہ.منزل_int () < 0x00010002u ||
				  markGlyphSetsDef.sanitize (c, هذا);

#جے_بنا_تعریف HB_NO_BEYOND_64K
    إذا (نسخہ.منزل_int () >= 0x00010004u)
    {
      إذا (glyphClassDef2) glyph_class_def_ok = glyphClassDef2.sanitize (c, هذا);
      إذا (attachList2) attach_list_ok = attachList2.sanitize (c, هذا);
      إذا (ligCaretList2) lig_caret_list_ok = ligCaretList2.sanitize (c, هذا);
      إذا (markAttachClassDef2) mark_attach_class_def_ok = markAttachClassDef2.sanitize (c, هذا);
      إذا (markGlyphSetsDef2) mark_glyph_sets_def_ok = markGlyphSetsDef2.sanitize (c, هذا);
    }
#جے_انت

    return_trace (glyph_class_def_ok &&
		  attach_list_ok &&
		  lig_caret_list_ok &&
		  mark_attach_class_def_ok &&
		  mark_glyph_sets_def_ok &&
		  (نسخہ.منزل_int () < 0x00010003u || varStore.sanitize (c, هذا)));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (نسخہ.major != 1) return_trace (خطأ);

    خودکار *باہرلا = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (باہرلا))) return_trace (خطأ);

    // Push var store first (if it's needed) so that it's last in the
    // serialization order. Some font consumers assume that varstore runs to
    // the end of the GDEF table.
    // See: https://github.com/harfbuzz/harfbuzz/issues/4636
    خودکار snapshot_version0 = c->serializer->snapshot ();
    إذا (unlikely (نسخہ.منزل_int () >= 0x00010002u && hb_barrier () &&
		  !c->serializer->embed (markGlyphSetsDef)))
      return_trace (خطأ);

    خودکار snapshot_version2 = c->serializer->snapshot ();
    إذا (unlikely (نسخہ.منزل_int () >= 0x00010003u && hb_barrier () &&
		  !c->serializer->embed (varStore)))
      return_trace (خطأ);

    منطقي use_v14 = خطأ;
#جے_بنا_تعریف HB_NO_BEYOND_64K
    use_v14 = نسخہ.منزل_int () >= 0x00010004u &&
	      (glyphClassDef2 || attachList2 || ligCaretList2 ||
	       markAttachClassDef2 || markGlyphSetsDef2);
    إذا (unlikely (نسخہ.منزل_int () >= 0x00010004u && hb_barrier () &&
		  !c->serializer->extend_ناپ (باہرلا, get_ناپ ())))
      return_trace (خطأ);
#جے_انت

    منطقي subset_varstore = خطأ;
    بنانشانی varstore_اشاریہ = (بنانشانی) -1;
    إذا (نسخہ.منزل_int () >= 0x00010003u && hb_barrier ())
    {
      إذا (c->plan->all_axes_pinned)
	باہرلا->varStore = 0;
      وإلا إذا (c->plan->normalized_coords)
      {
	إذا (varStore)
	{
	  شے_variations_t شے_vars;
	  إذا (unlikely (!c->serializer->check_success (
                  شے_vars.instantiate (هذا+varStore, c->plan, سچ, سچ,
                                          c->plan->gdef_varstore_inner_maps.as_array (), سچ))))
            return_trace (خطأ);
	  subset_varstore = باہرلا->varStore.serialize_serialize (c->serializer,
							     شے_vars.has_long_لفظ (),
							     c->plan->axis_ٹیگ_سوچی,
							     شے_vars.get_region_list (),
							     شے_vars.get_vardata_encodings ());
	  varstore_اشاریہ = c->serializer->آخری_added_بچہ_اشاریہ();
	  remap_varidx_after_instantiation (شے_vars.get_varidx_map (),
					    c->plan->layout_variation_idx_delta_map);
          remap_varidx_after_instantiation (شے_vars.get_varidx_map (),
                                            c->plan->layout_condition_idx_قدر_map);
	}
      }
      وإلا
      {
	subset_varstore = باہرلا->varStore.serialize_subset (c, varStore, هذا,
							  c->plan->gdef_varstore_inner_maps.as_array ());
	varstore_اشاریہ = c->serializer->آخری_added_بچہ_اشاریہ();
      }
    }

    باہرلا->نسخہ.major = نسخہ.major;
    باہرلا->نسخہ.minor = نسخہ.minor;

    إذا (!subset_varstore && نسخہ.منزل_int () >= 0x00010002u && !use_v14) {
      c->serializer->revert (snapshot_version2);
    }
    إذا (!subset_varstore)
      باہرلا->varStore = 0;
    وإلا
      c->plan->has_gdef_varstore = سچ;

    منطقي subset_markglyphsetsdef = خطأ;
    إذا (نسخہ.منزل_int () >= 0x00010002u && hb_barrier ())
    {
#جے_بنا_تعریف HB_NO_BEYOND_64K
      إذا (use_v14 && markGlyphSetsDef2)
      {
	باہرلا->markGlyphSetsDef = 0;
	subset_markglyphsetsdef = serialize_subset_وتھ (c, باہرلا->markGlyphSetsDef2,
							   markGlyphSetsDef2, هذا);
      }
      وإلا
#جے_انت
      {
#جے_بنا_تعریف HB_NO_BEYOND_64K
	إذا (use_v14) باہرلا->markGlyphSetsDef2 = 0;
#جے_انت
	subset_markglyphsetsdef = باہرلا->markGlyphSetsDef.serialize_subset (c, markGlyphSetsDef, هذا);
      }
    }

    إذا (use_v14)
    {
      باہرلا->نسخہ.minor = 4;
    } وإلا إذا (subset_varstore) {
      باہرلا->نسخہ.minor = 3;
    } وإلا إذا (subset_markglyphsetsdef) {
      باہرلا->نسخہ.minor = 2;
    } وإلا  {
      باہرلا->نسخہ.minor = 0;
      c->serializer->revert (snapshot_version0);
    }

    منطقي subset_glyphclassdef = خطأ;
    منطقي subset_attachlist = خطأ;
    منطقي subset_markattachclassdef = خطأ;
    منطقي subset_ligcaretlist = خطأ;

#جے_بنا_تعریف HB_NO_BEYOND_64K
    إذا (use_v14 && glyphClassDef2)
    {
      باہرلا->glyphClassDef = 0;
      subset_glyphclassdef = serialize_subset_وتھ (c, باہرلا->glyphClassDef2,
						      glyphClassDef2, هذا, خالی_سوچک, خطأ, سچ);
    }
    وإلا
#جے_انت
    {
#جے_بنا_تعریف HB_NO_BEYOND_64K
      إذا (use_v14) باہرلا->glyphClassDef2 = 0;
#جے_انت
      subset_glyphclassdef = باہرلا->glyphClassDef.serialize_subset (c, glyphClassDef, هذا, خالی_سوچک, خطأ, سچ);
    }

#جے_بنا_تعریف HB_NO_BEYOND_64K
    إذا (use_v14 && attachList2)
    {
      باہرلا->attachList = 0;
      subset_attachlist = serialize_subset_وتھ (c, باہرلا->attachList2, attachList2, هذا);
    }
    وإلا
#جے_انت
    {
#جے_بنا_تعریف HB_NO_BEYOND_64K
      إذا (use_v14) باہرلا->attachList2 = 0;
#جے_انت
      subset_attachlist = باہرلا->attachList.serialize_subset (c, attachList, هذا);
    }

#جے_بنا_تعریف HB_NO_BEYOND_64K
    إذا (use_v14 && markAttachClassDef2)
    {
      باہرلا->markAttachClassDef = 0;
      subset_markattachclassdef = serialize_subset_وتھ (c, باہرلا->markAttachClassDef2,
							   markAttachClassDef2, هذا, خالی_سوچک, خطأ, سچ);
    }
    وإلا
#جے_انت
    {
#جے_بنا_تعریف HB_NO_BEYOND_64K
      إذا (use_v14) باہرلا->markAttachClassDef2 = 0;
#جے_انت
      subset_markattachclassdef = باہرلا->markAttachClassDef.serialize_subset (c, markAttachClassDef,
									   هذا, خالی_سوچک, خطأ, سچ);
    }

#جے_بنا_تعریف HB_NO_BEYOND_64K
    إذا (use_v14 && ligCaretList2)
    {
      باہرلا->ligCaretList = 0;
      subset_ligcaretlist = serialize_subset_وتھ (c, باہرلا->ligCaretList2, ligCaretList2, هذا);
    }
    وإلا
#جے_انت
    {
#جے_بنا_تعریف HB_NO_BEYOND_64K
      إذا (use_v14) باہرلا->ligCaretList2 = 0;
#جے_انت
      subset_ligcaretlist = باہرلا->ligCaretList.serialize_subset (c, ligCaretList, هذا);
    }

    إذا (subset_varstore && varstore_اشاریہ != (بنانشانی) -1) {
      c->serializer->repack_آخری(varstore_اشاریہ);
    }

    return_trace (subset_glyphclassdef || subset_attachlist ||
		  subset_ligcaretlist || subset_markattachclassdef ||
		  (باہرلا->نسخہ.منزل_int () >= 0x00010002u && subset_markglyphsetsdef) ||
		  (باہرلا->نسخہ.منزل_int () >= 0x00010003u && subset_varstore));
  }

  منطقي has_glyph_classes () ثابت
  {
#جے_بنا_تعریف HB_NO_BEYOND_64K
    إذا (نسخہ.منزل_int () >= 0x00010004u && glyphClassDef2) أرجع سچ;
#جے_انت
    أرجع glyphClassDef != 0;
  }
  ثابت ClassDef &get_glyph_class_def () ثابت
  {
#جے_بنا_تعریف HB_NO_BEYOND_64K
    إذا (نسخہ.منزل_int () >= 0x00010004u && glyphClassDef2) أرجع هذا+glyphClassDef2;
#جے_انت
    أرجع هذا+glyphClassDef;
  }
  منطقي has_attach_list () ثابت
  {
#جے_بنا_تعریف HB_NO_BEYOND_64K
    إذا (نسخہ.منزل_int () >= 0x00010004u && attachList2) أرجع سچ;
#جے_انت
    أرجع attachList != 0;
  }
  ثابت AttachList &get_attach_list () ثابت
  {
#جے_بنا_تعریف HB_NO_BEYOND_64K
    إذا (نسخہ.منزل_int () >= 0x00010004u && attachList2) أرجع هذا+attachList2;
#جے_انت
    أرجع هذا+attachList;
  }
  منطقي has_lig_carets () ثابت
  {
#جے_بنا_تعریف HB_NO_BEYOND_64K
    إذا (نسخہ.منزل_int () >= 0x00010004u && ligCaretList2) أرجع سچ;
#جے_انت
    أرجع ligCaretList != 0;
  }
  منطقي has_mark_attachment_types () ثابت
  {
#جے_بنا_تعریف HB_NO_BEYOND_64K
    إذا (نسخہ.منزل_int () >= 0x00010004u && markAttachClassDef2) أرجع سچ;
#جے_انت
    أرجع markAttachClassDef != 0;
  }
  ثابت ClassDef &get_mark_attach_class_def () ثابت
  {
#جے_بنا_تعریف HB_NO_BEYOND_64K
    إذا (نسخہ.منزل_int () >= 0x00010004u && markAttachClassDef2) أرجع هذا+markAttachClassDef2;
#جے_انت
    أرجع هذا+markAttachClassDef;
  }
  منطقي has_mark_glyph_sets () ثابت
  {
    إذا (نسخہ.منزل_int () < 0x00010002u) أرجع خطأ;
#جے_بنا_تعریف HB_NO_BEYOND_64K
    إذا (نسخہ.منزل_int () >= 0x00010004u && markGlyphSetsDef2) أرجع سچ;
#جے_انت
    أرجع markGlyphSetsDef != 0;
  }
  ثابت MarkGlyphSets &get_mark_glyph_sets () ثابت
  {
    إذا (نسخہ.منزل_int () < 0x00010002u) أرجع Null(MarkGlyphSets);
#جے_بنا_تعریف HB_NO_BEYOND_64K
    إذا (نسخہ.منزل_int () >= 0x00010004u && markGlyphSetsDef2) أرجع هذا+markGlyphSetsDef2;
#جے_انت
    أرجع هذا+markGlyphSetsDef;
  }
  منطقي has_var_store () ثابت
  {
    أرجع نسخہ.منزل_int () >= 0x00010003u && hb_barrier () && varStore != 0;
  }
  ثابت شے_Variation_Store &get_var_store () ثابت
  {
    أرجع نسخہ.منزل_int () >= 0x00010003u && hb_barrier () ? هذا+varStore : Null(شے_Variation_Store);
  }


  منطقي has_انکڑے () ثابت { أرجع نسخہ.منزل_int (); }
  بنانشانی صحيح get_glyph_class (hb_codepoint_t glyph) ثابت
  { أرجع get_glyph_class_def ().get_class (glyph); }
  فراغ get_glyphs_وچ_class (بنانشانی صحيح klass, hb_سیٹ_t *glyphs) ثابت
  { get_glyph_class_def ().collect_class (glyphs, klass); }

  بنانشانی صحيح get_mark_attachment_type (hb_codepoint_t glyph) ثابت
  { أرجع get_mark_attach_class_def ().get_class (glyph); }

  بنانشانی صحيح get_attach_points (hb_codepoint_t glyph_id,
				  بنانشانی صحيح شروع_وتھ,
				  بنانشانی صحيح *نقطہ_گنتی /* IN/OUT */,
				  بنانشانی صحيح *نقطہ_array /*باہرلا*/) ثابت
  { أرجع get_attach_list ().get_attach_points (glyph_id, شروع_وتھ, نقطہ_گنتی, نقطہ_array); }

  بنانشانی صحيح get_lig_carets (hb_font_t *font,
			       hb_direction_t direction,
			       hb_codepoint_t glyph_id,
			       بنانشانی صحيح شروع_وتھ,
			       بنانشانی صحيح *caret_گنتی /* IN/OUT */,
			       hb_position_t *caret_array /*باہرلا*/) ثابت
  {
#جے_بنا_تعریف HB_NO_BEYOND_64K
    إذا (نسخہ.منزل_int () >= 0x00010004u && ligCaretList2)
      أرجع (هذا+ligCaretList2).get_lig_carets (font, direction, glyph_id, get_var_store (),
						  شروع_وتھ, caret_گنتی, caret_array);
#جے_انت
    أرجع (هذا+ligCaretList).get_lig_carets (font, direction, glyph_id, get_var_store (),
					       شروع_وتھ, caret_گنتی, caret_array);
  }

  منطقي mark_سیٹ_covers (بنانشانی صحيح سیٹ_اشاریہ, hb_codepoint_t glyph_id) ثابت
  { أرجع get_mark_glyph_sets ().covers (سیٹ_اشاریہ, glyph_id); }

  /* glyph_props is a 16-bit integer where the lower 8-bit have bits representing
   * glyph class and other bits, and high 8-bit the mark attachment type (if any).
   * Not to be confused with lookup_props which is very similar. */
  بنانشانی صحيح get_glyph_props (hb_codepoint_t glyph) ثابت
  {
    بنانشانی صحيح klass = get_glyph_class (glyph);

    static_assert (((بنانشانی صحيح) HB_OT_LAYOUT_GLYPH_PROPS_ادھار_GLYPH == (بنانشانی صحيح) LookupFlag::Ignore_ادھار_Glyphs), "");
    static_assert (((بنانشانی صحيح) HB_OT_LAYOUT_GLYPH_PROPS_LIGATURE == (بنانشانی صحيح) LookupFlag::IgnoreLigatures), "");
    static_assert (((بنانشانی صحيح) HB_OT_LAYOUT_GLYPH_PROPS_MARK == (بنانشانی صحيح) LookupFlag::IgnoreMarks), "");

    بدل (klass) {
    افتراضي:			أرجع HB_OT_LAYOUT_GLYPH_CLASS_UNCLASSIFIED;
    حالة ادھار_Glyph:		أرجع HB_OT_LAYOUT_GLYPH_PROPS_ادھار_GLYPH;
    حالة LigatureGlyph:		أرجع HB_OT_LAYOUT_GLYPH_PROPS_LIGATURE;
    حالة MarkGlyph:
	  klass = get_mark_attachment_type (glyph);
	  أرجع HB_OT_LAYOUT_GLYPH_PROPS_MARK | (klass << 8);
    }
  }

  HB_اندرونی منطقي is_blocklisted (hb_blob_t *blob,
				   hb_face_t *face) ثابت;

  بنية accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      جدول = hb_sanitize_context_t ().reference_جدول<GDEF> (face);
      إذا (unlikely (جدول->is_blocklisted (جدول.get_blob (), face)))
      {
	hb_blob_destroy (جدول.get_blob ());
	جدول = hb_blob_get_خالی_تھاواں ();
      }

#جے_بنا_تعریف HB_NO_GDEF_CACHE
      /* Flatten each mark set whose glyphs span fewer than PAGE_BITS
       * gids into a single bit-page biased at its first glyph, for
       * O(1) exact answers.  Wider sets, or on allocation failure,
       * fall back to the table. */
      {
	hb_سمتیہ_t<hb_سیٹ_t> sets;
	جدول->get_mark_glyph_sets ().collect_coverage (sets);
	إذا (!sets.وچ_error () &&
	    mark_glyph_سیٹ_bitmaps.resize (sets.لمائی))
	  لكل (بنانشانی i = 0; i < sets.لمائی; i++)
	  {
	    خودکار &bitmap = mark_glyph_سیٹ_bitmaps.arrayZ[i];
	    bitmap.bias = HB_سیٹ_قدر_INVALID;
	    ثابت خودکار &سیٹ = sets.arrayZ[i];
	    إذا (unlikely (سیٹ.وچ_error ())) استمر;
	    hb_codepoint_t min_gid = HB_سیٹ_قدر_INVALID;
	    hb_codepoint_t max_gid = HB_سیٹ_قدر_INVALID;
	    hb_سیٹ_اگلا (&سیٹ, &min_gid);
	    hb_سیٹ_پچھلا (&سیٹ, &max_gid);
	    إذا (max_gid == HB_سیٹ_قدر_INVALID)
	      استمر;
	    إذا (max_gid - min_gid >= hb_ثنائی_ہندسہ_ورقہ_t::ورقہ_بٹ)
	    {
	      /* Too wide for one page; a digest pre-filter instead. */
	      لكل (hb_codepoint_t gid : سیٹ)
		bitmap.u.digest.جوڑو (gid);
	      استمر;
	    }
	    bitmap.bias = min_gid;
	    لكل (hb_codepoint_t gid : سیٹ)
	      bitmap.u.ورقہ.جوڑو (gid - min_gid);
	  }
      }
#جے_انت
    }
    ~accelerator_t () { جدول.destroy (); }

    بنانشانی صحيح get_glyph_props (hb_codepoint_t glyph) ثابت
    {
      بنانشانی v;

#جے_بنا_تعریف HB_NO_GDEF_CACHE
      إذا (glyph_props_cache.get (glyph, &v))
        أرجع v;
#جے_انت

      v = جدول->get_glyph_props (glyph);

#جے_بنا_تعریف HB_NO_GDEF_CACHE
      إذا (likely (جدول.get_blob ())) // Don't try setting if we are the null instance!
	glyph_props_cache.سیٹ (glyph, v);
#جے_انت

      أرجع v;

    }

    HB_ALWAYS_INLINE
    منطقي mark_سیٹ_covers (بنانشانی صحيح سیٹ_اشاریہ, hb_codepoint_t glyph_id) ثابت
    {
#جے_بنا_تعریف HB_NO_GDEF_CACHE
      // We can access arrayZ directly because of sanitize_lookup_props() guarantee.
      ثابت خودکار &bitmap = mark_glyph_سیٹ_bitmaps.arrayZ[سیٹ_اشاریہ];
      إذا (likely (bitmap.bias != HB_سیٹ_قدر_INVALID))
      {
	hb_codepoint_t biased = glyph_id - bitmap.bias;
	أرجع biased < hb_ثنائی_ہندسہ_ورقہ_t::ورقہ_بٹ &&
	       bitmap.u.ورقہ.get (biased);
      }
      أرجع mark_سیٹ_covers_slow (سیٹ_اشاریہ, glyph_id);
#وإلا
      أرجع جدول->mark_سیٹ_covers (سیٹ_اشاریہ, glyph_id);
#جے_انت
    }

#جے_بنا_تعریف HB_NO_GDEF_CACHE
    HB_NEVER_INLINE
    منطقي mark_سیٹ_covers_slow (بنانشانی صحيح سیٹ_اشاریہ, hb_codepoint_t glyph_id) ثابت
    {
      ثابت خودکار &bitmap = mark_glyph_سیٹ_bitmaps.arrayZ[سیٹ_اشاریہ];
      إذا (!bitmap.u.digest.may_have (glyph_id))
	أرجع خطأ;
      أرجع جدول->mark_سیٹ_covers (سیٹ_اشاریہ, glyph_id);
    }
#جے_انت

    بنانشانی sanitize_lookup_props (بنانشانی lookup_props) ثابت
    {
#جے_بنا_تعریف HB_NO_GDEF_CACHE
      إذا (lookup_props & LookupFlag::Use_Mark_Filtering_سیٹ &&
	  (lookup_props >> 16) >= mark_glyph_سیٹ_bitmaps.لمائی)
      {
        // Invalid mark filtering set index; unset the flag.
	lookup_props &= ~LookupFlag::Use_Mark_Filtering_سیٹ;
      }
#جے_انت
      أرجع lookup_props;
    }

    hb_blob_ptr_t<GDEF> جدول;
#جے_بنا_تعریف HB_NO_GDEF_CACHE
    بنية mark_glyph_سیٹ_bitmap_t
    {
      /* Gid of the first bit (not page-aligned) for an exact one-page
       * set, or HB_SET_VALUE_INVALID; then digest is a pre-filter for
       * the table.  The all-zeros state is valid for both members. */
      hb_codepoint_t bias = HB_سیٹ_قدر_INVALID;
      اتحاد u_t {
	u_t () : ورقہ () {}
	hb_ثنائی_ہندسہ_ورقہ_t ورقہ;
	hb_سیٹ_digest_t digest;
      } u;
    };

    hb_سمتیہ_t<mark_glyph_سیٹ_bitmap_t> mark_glyph_سیٹ_bitmaps;
    بدلنجوگ hb_cache_t<21, 3> glyph_props_cache;
    static_assert (حجم (glyph_props_cache) == 512, "");
#جے_انت
  };

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
#جے_بنا_تعریف HB_NO_BEYOND_64K
    إذا (نسخہ.منزل_int () >= 0x00010004u && ligCaretList2)
    {
      (هذا+ligCaretList2).collect_variation_indices (c);
      أرجع;
    }
#جے_انت
    (هذا+ligCaretList).collect_variation_indices (c);
  }

  محمي:
  سانچا <قسم_ناں کلید>
  ساكن فراغ remap_varidx_after_instantiation (ثابت hb_map_t& varidx_map,
                                                hb_hashmap_t<کلید, hb_pair_t<بنانشانی, صحيح>>& layout_variation_idx_delta_map /* IN/OUT */)
  {
    /* varidx_map is empty which means varstore is empty after instantiation,
     * no variations, map all varidx to HB_OT_LAYOUT_NO_VARIATIONS_INDEX.
     * varidx_map doesn't have original varidx, indicating delta row is all
     * zeros, map varidx to HB_OT_LAYOUT_NO_VARIATIONS_INDEX */
    لكل (خودکار _ : layout_variation_idx_delta_map.iter_ref ())
    {
      /* old_varidx->(varidx, delta) mapping generated for subsetting, then this
       * varidx is used as key of varidx_map during instantiation */
      uint32_t varidx = _.second.پہلا;
      uint32_t *new_varidx;
      إذا (varidx_map.has (varidx, &new_varidx))
	_.second.پہلا = *new_varidx;
      وإلا
	_.second.پہلا = HB_OT_LAYOUT_NO_VARIATIONS_اشاریہ;
    }
  }

  سانچا <قسم_ناں وتھ_باہرلا, قسم_ناں وتھ_وچ, قسم_ناں ادھار_3, قسم_ناں ...Ts>
  ساكن منطقي serialize_subset_وتھ (hb_subset_context_t *c,
				       وتھ_باہرلا& باہرلا,
				       ثابت وتھ_وچ& وچ,
				       ثابت ادھار_3 *ادھار,
				       Ts&&... ds)
  {
    باہرلا = 0;
    إذا (وچ.is_null ()) أرجع خطأ;

    خودکار *s = c->serializer;
    s->push ();
    منطقي ret = c->dispatch (ادھار+وچ, std::forward<Ts> (ds)...);
    إذا (ret) s->جوڑو_link (باہرلا, s->نکالو_pack ());
    وإلا s->نکالو_discard ();
    أرجع ret;
  }

  Fixed_نسخہ<>نسخہ;		/* Version of the GDEF table--currently
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
  Offset32_منزل<شے_Variation_Store>
		varStore;		/* Offset to the table of Item Variation
					 * Store--from beginning of GDEF
					 * header (may be NULL).  Introduced
					 * in version 0x00010003. */
#جے_بنا_تعریف HB_NO_BEYOND_64K
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
#جے_انت
  عام:
  DEFINE_ناپ_MIN (12);
};

بنية GDEF_accelerator_t : GDEF::accelerator_t {
  GDEF_accelerator_t (hb_face_t *face) : GDEF::accelerator_t (face) {}
};

} /* namespace OT */


#جے_انت /* OT_LAYOUT_GDEF_GDEF_HH */
