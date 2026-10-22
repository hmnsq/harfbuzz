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

#جيڪڏهن_وصف_نه_ٿيل OT_LAYOUT_GDEF_GDEF_HH
#وصف_ڪريو OT_LAYOUT_GDEF_GDEF_HH

#ضمن "../../../hb-ot-var-common.hh"

#ضمن "../../../hb-font.hh"
#ضمن "../../../hb-cache.hh"


نطاق OT {


/*
 * Attachment List Table
 */

/* Array of contour point indices--in increasing numerical order */
بنية Attach_نقطو : Array16_جو<HBUINT16>
{
  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودڪار *ٻاهريون = c->serializer->شروعات_embed (*هذا);
    return_trace (ٻاهريون->serialize (c->serializer, + iter ()));
  }
};

/* AttachList is referenced by both GDEF attachListOffset and
 * attachListOffset2. The v1.4 top-level offset widens, but the
 * AttachList table itself keeps Offset16 internals. */
بنية AttachList
{
  بنانشان صحيح get_attach_points (hb_codepoint_t glyph_id,
				  بنانشان صحيح شروعات_وٿي,
				  بنانشان صحيح *نقطو_ڳڻپ /* IN/OUT */,
				  بنانشان صحيح *نقطو_array /*ٻاهريون*/) ثابت
  {
    بنانشان صحيح اشاريو_2 = (هذا+coverage).get_coverage (glyph_id);
    إذا (اشاريو_2 == NOT_COVERED)
    {
      إذا (نقطو_ڳڻپ)
	*نقطو_ڳڻپ = 0;
      أرجع 0;
    }

    ثابت Attach_نقطو &points = هذا+attach_نقطو[اشاريو_2];

    إذا (نقطو_ڳڻپ && نقطو_array)
    {
      + points.as_array ().sub_array (شروعات_وٿي, نقطو_ڳڻپ)
      | hb_sink (hb_array (نقطو_array, *نقطو_ڳڻپ))
      ;
    }

    أرجع points.len;
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_سيٽ_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خودڪار *ٻاهريون = c->serializer->شروعات_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (ٻاهريون))) return_trace (خطأ);

    hb_sorted_ويڪٽر_t<hb_codepoint_t> new_coverage;
    + hb_zip (هذا+coverage, attach_نقطو)
    | hb_filter (glyphset, hb_پهريون)
    | hb_filter (subset_وٿي_array (c, ٻاهريون->attach_نقطو, هذا), hb_ٻيو)
    | hb_map (hb_پهريون)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;
    ٻاهريون->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (منطقي (new_coverage));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (coverage.sanitize (c, هذا) && attach_نقطو.sanitize (c, هذا));
  }

  محمي:
  Offset16_منزل<Coverage>
		coverage;		/* Offset to Coverage table -- from
					 * beginning of AttachList table */
  Array16_جو_Offset16_منزل<Attach_نقطو>
		attach_نقطو;		/* Array of AttachPoint tables
					 * in Coverage Index order */
  عام:
  DEFINE_سائيز_ARRAY (4, attach_نقطو);
};

/*
 * Ligature Caret Table
 */

بنية Caret_قدر_Format1
{
  دوست بنية Caret_قدر;
  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودڪار *ٻاهريون = c->serializer->embed (هذا);
    إذا (unlikely (!ٻاهريون)) return_trace (خطأ);
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
  DEFINE_سائيز_STATIC (4);
};

بنية Caret_قدر_Format2
{
  دوست بنية Caret_قدر;
  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودڪار *ٻاهريون = c->serializer->embed (هذا);
    إذا (unlikely (!ٻاهريون)) return_trace (خطأ);
    return_trace (سچ);
  }

  خاص:
  hb_position_t get_caret_قدر (hb_font_t *font, hb_direction_t direction, hb_codepoint_t glyph_id) ثابت
  {
    hb_position_t x, y;
    font->get_glyph_contour_نقطو_for_origin (glyph_id, caret_قدر_نقطو, direction, &x, &y);
    أرجع HB_DIRECTION_IS_HORIZONTAL (direction) ? x : y;
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  محمي:
  HBUINT16	caret_قدر_Format;	/* Format identifier--format = 2 */
  HBUINT16	caret_قدر_نقطو;	/* Contour point index on glyph */
  عام:
  DEFINE_سائيز_STATIC (4);
};

بنية Caret_قدر_Format3
{
  دوست بنية Caret_قدر;

  hb_position_t get_caret_قدر (hb_font_t *font, hb_direction_t direction,
				 ثابت شي_Variation_Store &var_store) ثابت
  {
    أرجع HB_DIRECTION_IS_HORIZONTAL (direction) ?
	   hb_saturate_جوڙيو (font->em_scale_x (coordinate), (هذا+device_جدول).get_x_delta (font, var_store)) :
	   hb_saturate_جوڙيو (font->em_scale_y (coordinate), (هذا+device_جدول).get_y_delta (font, var_store));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودڪار *ٻاهريون = c->serializer->شروعات_embed (*هذا);
    إذا (!c->serializer->embed (caret_قدر_Format)) return_trace (خطأ);
    إذا (!c->serializer->embed (coordinate)) return_trace (خطأ);

    بنانشان varidx = (هذا+device_جدول).get_variation_اشاريو ();
    hb_pair_t<بنانشان, صحيح> *new_varidx_delta;
    إذا (c->plan->layout_variation_idx_delta_map.has (varidx, &new_varidx_delta)) {
      uint32_t new_varidx = hb_پهريون (*new_varidx_delta);
      صحيح delta = hb_ٻيو (*new_varidx_delta);
      إذا (delta != 0)
      {
        إذا (!c->serializer->check_سونپيو (ٻاهريون->coordinate, coordinate + delta, HB_SERIALIZE_ERROR_INT_حد_کان_مٿي))
          return_trace (خطأ);
      }

      إذا (new_varidx == HB_OT_LAYOUT_NO_VARIATIONS_اشاريو)
        return_trace (c->serializer->check_سونپيو (ٻاهريون->caret_قدر_Format, 1, HB_SERIALIZE_ERROR_INT_حد_کان_مٿي));
    }

    إذا (!c->serializer->embed (device_جدول))
      return_trace (خطأ);

    return_trace (ٻاهريون->device_جدول.serialize_copy (c->serializer, device_جدول, هذا, c->serializer->منزل_bias (ٻاهريون),
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
  DEFINE_سائيز_STATIC (6);
};

بنية Caret_قدر
{
  hb_position_t get_caret_قدر (hb_font_t *font,
				 hb_direction_t direction,
				 hb_codepoint_t glyph_id,
				 ثابت شي_Variation_Store &var_store) ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.get_caret_قدر (font, direction);
    حالة 2: hb_barrier (); أرجع u.format2.get_caret_قدر (font, direction, glyph_id);
    حالة 3: hb_barrier (); أرجع u.format3.get_caret_قدر (font, direction, var_store);
    افتراضي:أرجع 0;
    }
  }

  سانچو <قسم_نالو context_t, قسم_نالو ...Ts>
  قسم_نالو context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
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
  DEFINE_سائيز_UNION (2, format.v);
};

بنية LigGlyph
{
  بنانشان get_lig_carets (hb_font_t            *font,
			   hb_direction_t        direction,
			   hb_codepoint_t        glyph_id,
			   ثابت شي_Variation_Store &var_store,
			   بنانشان              شروعات_وٿي,
			   بنانشان             *caret_ڳڻپ /* IN/OUT */,
			   hb_position_t        *caret_array /*ٻاهريون*/) ثابت
  {
    إذا (caret_ڳڻپ && caret_array)
    {
      + carets.as_array ().sub_array (شروعات_وٿي, caret_ڳڻپ)
      | hb_map (hb_جوڙيو (هذا))
      | hb_map ([&] (ثابت Caret_قدر &value) { أرجع value.get_caret_قدر (font, direction, glyph_id, var_store); })
      | hb_sink (hb_array (caret_array, *caret_ڳڻپ))
      ;
    }

    أرجع carets.len;
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودڪار *ٻاهريون = c->serializer->شروعات_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (ٻاهريون))) return_trace (خطأ);

    + hb_iter (carets)
    | hb_apply (subset_وٿي_array (c, ٻاهريون->carets, هذا))
    ;

    return_trace (منطقي (ٻاهريون->carets));
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    لكل (ثابت Offset16_منزل<Caret_قدر>& وٿي : carets.iter ())
      (هذا+وٿي).collect_variation_indices (c);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (carets.sanitize (c, هذا));
  }

  محمي:
  Array16_جو_Offset16_منزل<Caret_قدر>
		carets;			/* Offset array of CaretValue tables
					 * --from beginning of LigGlyph table
					 * --in increasing coordinate order */
  عام:
  DEFINE_سائيز_ARRAY (2, carets);
};

/* LigCaretList2 widens the LigCaretList coverage, count, and LigGlyph
 * offsets. The LigGlyph table itself keeps Offset16 caretValueOffsets. */
سانچو <قسم_نالو Types>
بنية LigCaretList
{
  بنانشان صحيح get_lig_carets (hb_font_t *font,
			       hb_direction_t direction,
			       hb_codepoint_t glyph_id,
			       ثابت شي_Variation_Store &var_store,
			       بنانشان صحيح شروعات_وٿي,
			       بنانشان صحيح *caret_ڳڻپ /* IN/OUT */,
			       hb_position_t *caret_array /*ٻاهريون*/) ثابت
  {
    بنانشان صحيح اشاريو_2 = (هذا+coverage).get_coverage (glyph_id);
    إذا (اشاريو_2 == NOT_COVERED)
    {
      إذا (caret_ڳڻپ)
	*caret_ڳڻپ = 0;
      أرجع 0;
    }
    ثابت LigGlyph &lig_glyph = هذا+ligGlyph[اشاريو_2];
    أرجع lig_glyph.get_lig_carets (font, direction, glyph_id, var_store, شروعات_وٿي, caret_ڳڻپ, caret_array);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_سيٽ_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خودڪار *ٻاهريون = c->serializer->شروعات_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (ٻاهريون))) return_trace (خطأ);

    hb_sorted_ويڪٽر_t<hb_codepoint_t> new_coverage;
    + hb_zip (هذا+coverage, ligGlyph)
    | hb_filter (glyphset, hb_پهريون)
    | hb_filter (subset_وٿي_array (c, ٻاهريون->ligGlyph, هذا), hb_ٻيو)
    | hb_map (hb_پهريون)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;
    ٻاهريون->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (منطقي (new_coverage));
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    + hb_zip (هذا+coverage, ligGlyph)
    | hb_filter (c->glyph_سيٽ, hb_پهريون)
    | hb_map (hb_ٻيو)
    | hb_map (hb_جوڙيو (هذا))
    | hb_apply ([c] (ثابت LigGlyph& _) { _.collect_variation_indices (c); })
    ;
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (coverage.sanitize (c, هذا) && ligGlyph.sanitize (c, هذا));
  }

  محمي:
  قسم_نالو Types::سانچو LOffset_منزل<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of LigCaretList table */
  قسم_نالو Types::سانچو Array_جو<قسم_نالو Types::سانچو وٿي_منزل<LigGlyph>>
		ligGlyph;		/* Array of LigGlyph tables
					 * in Coverage Index order */
  عام:
  DEFINE_سائيز_ARRAY (Types::LOffset::static_سائيز + Types::HBUINT::static_سائيز, ligGlyph);
};

#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
static_assert (LigCaretList<MediumTypes>::min_سائيز ==
	       Offset32::static_سائيز + HBUINT24::static_سائيز, "");
#جيڪڏهن_پورو


بنية MarkGlyphSetsFormat1
{
  منطقي covers (بنانشان صحيح سيٽ_اشاريو, hb_codepoint_t glyph_id) ثابت
  { أرجع (هذا+coverage[سيٽ_اشاريو]).get_coverage (glyph_id) != NOT_COVERED; }

  فراغ collect_استعمال_ٿيل_mark_sets (ثابت hb_سيٽ_t& glyph_سيٽ,
                               hb_سيٽ_t& استعمال_ٿيل_mark_sets /*ٻاهريون*/) ثابت
  {
    بنانشان i = 0;
    لكل (ثابت خودڪار &وٿي : coverage)
     {
       ثابت خودڪار &cov = هذا+وٿي;
       إذا (cov.intersects (&glyph_سيٽ))
         استعمال_ٿيل_mark_sets.جوڙيو (i);

       i++;
     }
  }

  سانچو <قسم_نالو سيٽ_t>
  فراغ collect_coverage (hb_ويڪٽر_t<سيٽ_t> &sets) ثابت
  {
     لكل (ثابت خودڪار &وٿي : coverage)
     {
       ثابت خودڪار &cov = هذا+وٿي;
       cov.collect_coverage (sets.push ());
     }
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودڪار *ٻاهريون = c->serializer->شروعات_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (ٻاهريون))) return_trace (خطأ);
    ٻاهريون->format = format;

    منطقي ret = سچ;
    لكل (ثابت Offset32_منزل<Coverage>& وٿي : coverage.iter ())
    {
      خودڪار snap = c->serializer->snapshot ();
      خودڪار *o = ٻاهريون->coverage.serialize_append (c->serializer);
      إذا (unlikely (!o))
      {
	ret = خطأ;
	اكسر;
      }

      //skip empty coverage
      c->serializer->push ();
      منطقي res = خطأ;
      إذا (وٿي) res = c->dispatch (هذا+وٿي);
      إذا (!res)
      {
        c->serializer->ڪڍو_discard ();
        c->serializer->revert (snap);
        (ٻاهريون->coverage.len)--;
        استمر;
      }
      c->serializer->جوڙيو_link (*o, c->serializer->ڪڍو_pack ());
    }

    return_trace (ret && ٻاهريون->coverage.len);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (coverage.sanitize (c, هذا));
  }

  محمي:
  HBUINT16	format;			/* Format identifier--format = 1 */
  Array16_جو<Offset32_منزل<Coverage>>
		coverage;		/* Array of long offsets to mark set
					 * coverage tables */
  عام:
  DEFINE_سائيز_ARRAY (4, coverage);
};

بنية MarkGlyphSets
{
  منطقي covers (بنانشان صحيح سيٽ_اشاريو, hb_codepoint_t glyph_id) ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.covers (سيٽ_اشاريو, glyph_id);
    افتراضي:أرجع خطأ;
    }
  }

  سانچو <قسم_نالو سيٽ_t>
  فراغ collect_coverage (hb_ويڪٽر_t<سيٽ_t> &sets) ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); u.format1.collect_coverage (sets); أرجع;
    افتراضي:أرجع;
    }
  }

  فراغ collect_استعمال_ٿيل_mark_sets (ثابت hb_سيٽ_t& glyph_سيٽ,
                               hb_سيٽ_t& استعمال_ٿيل_mark_sets /*ٻاهريون*/) ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); u.format1.collect_استعمال_ٿيل_mark_sets (glyph_سيٽ, استعمال_ٿيل_mark_sets); أرجع;
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
  DEFINE_سائيز_UNION (2, format.v);
};


/*
 * GDEF -- Glyph Definition
 * https://docs.microsoft.com/en-us/typography/opentype/spec/gdef
 */


بنية GDEF
{
  ساكن constexpr hb_ٽيگ_t جدول_ٽيگ = HB_OT_ٽيگ_GDEF;

  تعداد GlyphClasses {
    UnclassifiedGlyph	= 0,
    بنياد_Glyph		= 1,
    LigatureGlyph	= 2,
    MarkGlyph		= 3,
    ComponentGlyph	= 4
  };

  size_t get_سائيز () ثابت
  {
    إذا (نسخو.major != 1) أرجع نسخو.static_سائيز;

    أرجع min_سائيز +
	   (نسخو.منزل_int () >= 0x00010002u ? markGlyphSetsDef.static_سائيز : 0) +
	   (نسخو.منزل_int () >= 0x00010003u ? varStore.static_سائيز : 0) +
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
	   (نسخو.منزل_int () >= 0x00010004u ? 5 * Offset32::static_سائيز : 0)
#وإلا
	   0
#جيڪڏهن_پورو
	   ;
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!نسخو.sanitize (c) ||
		  نسخو.major != 1 ||
		  !c->check_range (هذا, get_سائيز ())))
      return_trace (خطأ);
    hb_barrier ();

    منطقي glyph_class_def_ok = glyphClassDef.sanitize (c, هذا);
    منطقي attach_list_ok = attachList.sanitize (c, هذا);
    منطقي lig_caret_list_ok = ligCaretList.sanitize (c, هذا);
    منطقي mark_attach_class_def_ok = markAttachClassDef.sanitize (c, هذا);
    منطقي mark_glyph_sets_def_ok = نسخو.منزل_int () < 0x00010002u ||
				  markGlyphSetsDef.sanitize (c, هذا);

#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    إذا (نسخو.منزل_int () >= 0x00010004u)
    {
      إذا (glyphClassDef2) glyph_class_def_ok = glyphClassDef2.sanitize (c, هذا);
      إذا (attachList2) attach_list_ok = attachList2.sanitize (c, هذا);
      إذا (ligCaretList2) lig_caret_list_ok = ligCaretList2.sanitize (c, هذا);
      إذا (markAttachClassDef2) mark_attach_class_def_ok = markAttachClassDef2.sanitize (c, هذا);
      إذا (markGlyphSetsDef2) mark_glyph_sets_def_ok = markGlyphSetsDef2.sanitize (c, هذا);
    }
#جيڪڏهن_پورو

    return_trace (glyph_class_def_ok &&
		  attach_list_ok &&
		  lig_caret_list_ok &&
		  mark_attach_class_def_ok &&
		  mark_glyph_sets_def_ok &&
		  (نسخو.منزل_int () < 0x00010003u || varStore.sanitize (c, هذا)));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (نسخو.major != 1) return_trace (خطأ);

    خودڪار *ٻاهريون = c->serializer->شروعات_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (ٻاهريون))) return_trace (خطأ);

    // Push var store first (if it's needed) so that it's last in the
    // serialization order. Some font consumers assume that varstore runs to
    // the end of the GDEF table.
    // See: https://github.com/harfbuzz/harfbuzz/issues/4636
    خودڪار snapshot_version0 = c->serializer->snapshot ();
    إذا (unlikely (نسخو.منزل_int () >= 0x00010002u && hb_barrier () &&
		  !c->serializer->embed (markGlyphSetsDef)))
      return_trace (خطأ);

    خودڪار snapshot_version2 = c->serializer->snapshot ();
    إذا (unlikely (نسخو.منزل_int () >= 0x00010003u && hb_barrier () &&
		  !c->serializer->embed (varStore)))
      return_trace (خطأ);

    منطقي use_v14 = خطأ;
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    use_v14 = نسخو.منزل_int () >= 0x00010004u &&
	      (glyphClassDef2 || attachList2 || ligCaretList2 ||
	       markAttachClassDef2 || markGlyphSetsDef2);
    إذا (unlikely (نسخو.منزل_int () >= 0x00010004u && hb_barrier () &&
		  !c->serializer->extend_سائيز (ٻاهريون, get_سائيز ())))
      return_trace (خطأ);
#جيڪڏهن_پورو

    منطقي subset_varstore = خطأ;
    بنانشان varstore_اشاريو = (بنانشان) -1;
    إذا (نسخو.منزل_int () >= 0x00010003u && hb_barrier ())
    {
      إذا (c->plan->all_axes_pinned)
	ٻاهريون->varStore = 0;
      وإلا إذا (c->plan->normalized_coords)
      {
	إذا (varStore)
	{
	  شي_variations_t شي_vars;
	  إذا (unlikely (!c->serializer->check_success (
                  شي_vars.instantiate (هذا+varStore, c->plan, سچ, سچ,
                                          c->plan->gdef_varstore_inner_maps.as_array (), سچ))))
            return_trace (خطأ);
	  subset_varstore = ٻاهريون->varStore.serialize_serialize (c->serializer,
							     شي_vars.has_long_لفظ (),
							     c->plan->axis_ٽيگ_فهرست,
							     شي_vars.get_region_list (),
							     شي_vars.get_vardata_encodings ());
	  varstore_اشاريو = c->serializer->آخري_added_ٻار_اشاريو();
	  remap_varidx_after_instantiation (شي_vars.get_varidx_map (),
					    c->plan->layout_variation_idx_delta_map);
          remap_varidx_after_instantiation (شي_vars.get_varidx_map (),
                                            c->plan->layout_condition_idx_قدر_map);
	}
      }
      وإلا
      {
	subset_varstore = ٻاهريون->varStore.serialize_subset (c, varStore, هذا,
							  c->plan->gdef_varstore_inner_maps.as_array ());
	varstore_اشاريو = c->serializer->آخري_added_ٻار_اشاريو();
      }
    }

    ٻاهريون->نسخو.major = نسخو.major;
    ٻاهريون->نسخو.minor = نسخو.minor;

    إذا (!subset_varstore && نسخو.منزل_int () >= 0x00010002u && !use_v14) {
      c->serializer->revert (snapshot_version2);
    }
    إذا (!subset_varstore)
      ٻاهريون->varStore = 0;
    وإلا
      c->plan->has_gdef_varstore = سچ;

    منطقي subset_markglyphsetsdef = خطأ;
    إذا (نسخو.منزل_int () >= 0x00010002u && hb_barrier ())
    {
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
      إذا (use_v14 && markGlyphSetsDef2)
      {
	ٻاهريون->markGlyphSetsDef = 0;
	subset_markglyphsetsdef = serialize_subset_وٿي (c, ٻاهريون->markGlyphSetsDef2,
							   markGlyphSetsDef2, هذا);
      }
      وإلا
#جيڪڏهن_پورو
      {
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
	إذا (use_v14) ٻاهريون->markGlyphSetsDef2 = 0;
#جيڪڏهن_پورو
	subset_markglyphsetsdef = ٻاهريون->markGlyphSetsDef.serialize_subset (c, markGlyphSetsDef, هذا);
      }
    }

    إذا (use_v14)
    {
      ٻاهريون->نسخو.minor = 4;
    } وإلا إذا (subset_varstore) {
      ٻاهريون->نسخو.minor = 3;
    } وإلا إذا (subset_markglyphsetsdef) {
      ٻاهريون->نسخو.minor = 2;
    } وإلا  {
      ٻاهريون->نسخو.minor = 0;
      c->serializer->revert (snapshot_version0);
    }

    منطقي subset_glyphclassdef = خطأ;
    منطقي subset_attachlist = خطأ;
    منطقي subset_markattachclassdef = خطأ;
    منطقي subset_ligcaretlist = خطأ;

#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    إذا (use_v14 && glyphClassDef2)
    {
      ٻاهريون->glyphClassDef = 0;
      subset_glyphclassdef = serialize_subset_وٿي (c, ٻاهريون->glyphClassDef2,
						      glyphClassDef2, هذا, خالي_اشارو, خطأ, سچ);
    }
    وإلا
#جيڪڏهن_پورو
    {
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
      إذا (use_v14) ٻاهريون->glyphClassDef2 = 0;
#جيڪڏهن_پورو
      subset_glyphclassdef = ٻاهريون->glyphClassDef.serialize_subset (c, glyphClassDef, هذا, خالي_اشارو, خطأ, سچ);
    }

#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    إذا (use_v14 && attachList2)
    {
      ٻاهريون->attachList = 0;
      subset_attachlist = serialize_subset_وٿي (c, ٻاهريون->attachList2, attachList2, هذا);
    }
    وإلا
#جيڪڏهن_پورو
    {
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
      إذا (use_v14) ٻاهريون->attachList2 = 0;
#جيڪڏهن_پورو
      subset_attachlist = ٻاهريون->attachList.serialize_subset (c, attachList, هذا);
    }

#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    إذا (use_v14 && markAttachClassDef2)
    {
      ٻاهريون->markAttachClassDef = 0;
      subset_markattachclassdef = serialize_subset_وٿي (c, ٻاهريون->markAttachClassDef2,
							   markAttachClassDef2, هذا, خالي_اشارو, خطأ, سچ);
    }
    وإلا
#جيڪڏهن_پورو
    {
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
      إذا (use_v14) ٻاهريون->markAttachClassDef2 = 0;
#جيڪڏهن_پورو
      subset_markattachclassdef = ٻاهريون->markAttachClassDef.serialize_subset (c, markAttachClassDef,
									   هذا, خالي_اشارو, خطأ, سچ);
    }

#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    إذا (use_v14 && ligCaretList2)
    {
      ٻاهريون->ligCaretList = 0;
      subset_ligcaretlist = serialize_subset_وٿي (c, ٻاهريون->ligCaretList2, ligCaretList2, هذا);
    }
    وإلا
#جيڪڏهن_پورو
    {
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
      إذا (use_v14) ٻاهريون->ligCaretList2 = 0;
#جيڪڏهن_پورو
      subset_ligcaretlist = ٻاهريون->ligCaretList.serialize_subset (c, ligCaretList, هذا);
    }

    إذا (subset_varstore && varstore_اشاريو != (بنانشان) -1) {
      c->serializer->repack_آخري(varstore_اشاريو);
    }

    return_trace (subset_glyphclassdef || subset_attachlist ||
		  subset_ligcaretlist || subset_markattachclassdef ||
		  (ٻاهريون->نسخو.منزل_int () >= 0x00010002u && subset_markglyphsetsdef) ||
		  (ٻاهريون->نسخو.منزل_int () >= 0x00010003u && subset_varstore));
  }

  منطقي has_glyph_classes () ثابت
  {
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    إذا (نسخو.منزل_int () >= 0x00010004u && glyphClassDef2) أرجع سچ;
#جيڪڏهن_پورو
    أرجع glyphClassDef != 0;
  }
  ثابت ClassDef &get_glyph_class_def () ثابت
  {
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    إذا (نسخو.منزل_int () >= 0x00010004u && glyphClassDef2) أرجع هذا+glyphClassDef2;
#جيڪڏهن_پورو
    أرجع هذا+glyphClassDef;
  }
  منطقي has_attach_list () ثابت
  {
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    إذا (نسخو.منزل_int () >= 0x00010004u && attachList2) أرجع سچ;
#جيڪڏهن_پورو
    أرجع attachList != 0;
  }
  ثابت AttachList &get_attach_list () ثابت
  {
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    إذا (نسخو.منزل_int () >= 0x00010004u && attachList2) أرجع هذا+attachList2;
#جيڪڏهن_پورو
    أرجع هذا+attachList;
  }
  منطقي has_lig_carets () ثابت
  {
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    إذا (نسخو.منزل_int () >= 0x00010004u && ligCaretList2) أرجع سچ;
#جيڪڏهن_پورو
    أرجع ligCaretList != 0;
  }
  منطقي has_mark_attachment_types () ثابت
  {
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    إذا (نسخو.منزل_int () >= 0x00010004u && markAttachClassDef2) أرجع سچ;
#جيڪڏهن_پورو
    أرجع markAttachClassDef != 0;
  }
  ثابت ClassDef &get_mark_attach_class_def () ثابت
  {
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    إذا (نسخو.منزل_int () >= 0x00010004u && markAttachClassDef2) أرجع هذا+markAttachClassDef2;
#جيڪڏهن_پورو
    أرجع هذا+markAttachClassDef;
  }
  منطقي has_mark_glyph_sets () ثابت
  {
    إذا (نسخو.منزل_int () < 0x00010002u) أرجع خطأ;
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    إذا (نسخو.منزل_int () >= 0x00010004u && markGlyphSetsDef2) أرجع سچ;
#جيڪڏهن_پورو
    أرجع markGlyphSetsDef != 0;
  }
  ثابت MarkGlyphSets &get_mark_glyph_sets () ثابت
  {
    إذا (نسخو.منزل_int () < 0x00010002u) أرجع Null(MarkGlyphSets);
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    إذا (نسخو.منزل_int () >= 0x00010004u && markGlyphSetsDef2) أرجع هذا+markGlyphSetsDef2;
#جيڪڏهن_پورو
    أرجع هذا+markGlyphSetsDef;
  }
  منطقي has_var_store () ثابت
  {
    أرجع نسخو.منزل_int () >= 0x00010003u && hb_barrier () && varStore != 0;
  }
  ثابت شي_Variation_Store &get_var_store () ثابت
  {
    أرجع نسخو.منزل_int () >= 0x00010003u && hb_barrier () ? هذا+varStore : Null(شي_Variation_Store);
  }


  منطقي has_معلومات () ثابت { أرجع نسخو.منزل_int (); }
  بنانشان صحيح get_glyph_class (hb_codepoint_t glyph) ثابت
  { أرجع get_glyph_class_def ().get_class (glyph); }
  فراغ get_glyphs_اندريون_class (بنانشان صحيح klass, hb_سيٽ_t *glyphs) ثابت
  { get_glyph_class_def ().collect_class (glyphs, klass); }

  بنانشان صحيح get_mark_attachment_type (hb_codepoint_t glyph) ثابت
  { أرجع get_mark_attach_class_def ().get_class (glyph); }

  بنانشان صحيح get_attach_points (hb_codepoint_t glyph_id,
				  بنانشان صحيح شروعات_وٿي,
				  بنانشان صحيح *نقطو_ڳڻپ /* IN/OUT */,
				  بنانشان صحيح *نقطو_array /*ٻاهريون*/) ثابت
  { أرجع get_attach_list ().get_attach_points (glyph_id, شروعات_وٿي, نقطو_ڳڻپ, نقطو_array); }

  بنانشان صحيح get_lig_carets (hb_font_t *font,
			       hb_direction_t direction,
			       hb_codepoint_t glyph_id,
			       بنانشان صحيح شروعات_وٿي,
			       بنانشان صحيح *caret_ڳڻپ /* IN/OUT */,
			       hb_position_t *caret_array /*ٻاهريون*/) ثابت
  {
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    إذا (نسخو.منزل_int () >= 0x00010004u && ligCaretList2)
      أرجع (هذا+ligCaretList2).get_lig_carets (font, direction, glyph_id, get_var_store (),
						  شروعات_وٿي, caret_ڳڻپ, caret_array);
#جيڪڏهن_پورو
    أرجع (هذا+ligCaretList).get_lig_carets (font, direction, glyph_id, get_var_store (),
					       شروعات_وٿي, caret_ڳڻپ, caret_array);
  }

  منطقي mark_سيٽ_covers (بنانشان صحيح سيٽ_اشاريو, hb_codepoint_t glyph_id) ثابت
  { أرجع get_mark_glyph_sets ().covers (سيٽ_اشاريو, glyph_id); }

  /* glyph_props is a 16-bit integer where the lower 8-bit have bits representing
   * glyph class and other bits, and high 8-bit the mark attachment type (if any).
   * Not to be confused with lookup_props which is very similar. */
  بنانشان صحيح get_glyph_props (hb_codepoint_t glyph) ثابت
  {
    بنانشان صحيح klass = get_glyph_class (glyph);

    static_assert (((بنانشان صحيح) HB_OT_LAYOUT_GLYPH_PROPS_بنياد_GLYPH == (بنانشان صحيح) LookupFlag::Ignore_بنياد_Glyphs), "");
    static_assert (((بنانشان صحيح) HB_OT_LAYOUT_GLYPH_PROPS_LIGATURE == (بنانشان صحيح) LookupFlag::IgnoreLigatures), "");
    static_assert (((بنانشان صحيح) HB_OT_LAYOUT_GLYPH_PROPS_MARK == (بنانشان صحيح) LookupFlag::IgnoreMarks), "");

    بدل (klass) {
    افتراضي:			أرجع HB_OT_LAYOUT_GLYPH_CLASS_UNCLASSIFIED;
    حالة بنياد_Glyph:		أرجع HB_OT_LAYOUT_GLYPH_PROPS_بنياد_GLYPH;
    حالة LigatureGlyph:		أرجع HB_OT_LAYOUT_GLYPH_PROPS_LIGATURE;
    حالة MarkGlyph:
	  klass = get_mark_attachment_type (glyph);
	  أرجع HB_OT_LAYOUT_GLYPH_PROPS_MARK | (klass << 8);
    }
  }

  HB_اندروني منطقي is_blocklisted (hb_blob_t *blob,
				   hb_face_t *face) ثابت;

  بنية accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      جدول = hb_sanitize_context_t ().reference_جدول<GDEF> (face);
      إذا (unlikely (جدول->is_blocklisted (جدول.get_blob (), face)))
      {
	hb_blob_destroy (جدول.get_blob ());
	جدول = hb_blob_get_خالي_جايون ();
      }

#جيڪڏهن_وصف_نه_ٿيل HB_NO_GDEF_CACHE
      /* Flatten each mark set whose glyphs span fewer than PAGE_BITS
       * gids into a single bit-page biased at its first glyph, for
       * O(1) exact answers.  Wider sets, or on allocation failure,
       * fall back to the table. */
      {
	hb_ويڪٽر_t<hb_سيٽ_t> sets;
	جدول->get_mark_glyph_sets ().collect_coverage (sets);
	إذا (!sets.اندريون_error () &&
	    mark_glyph_سيٽ_bitmaps.resize (sets.ڊيگهائي))
	  لكل (بنانشان i = 0; i < sets.ڊيگهائي; i++)
	  {
	    خودڪار &bitmap = mark_glyph_سيٽ_bitmaps.arrayZ[i];
	    bitmap.bias = HB_سيٽ_قدر_INVALID;
	    ثابت خودڪار &سيٽ = sets.arrayZ[i];
	    إذا (unlikely (سيٽ.اندريون_error ())) استمر;
	    hb_codepoint_t min_gid = HB_سيٽ_قدر_INVALID;
	    hb_codepoint_t max_gid = HB_سيٽ_قدر_INVALID;
	    hb_سيٽ_ايندڙ (&سيٽ, &min_gid);
	    hb_سيٽ_اڳيون (&سيٽ, &max_gid);
	    إذا (max_gid == HB_سيٽ_قدر_INVALID)
	      استمر;
	    إذا (max_gid - min_gid >= hb_ٻٽو_انگ_صفحو_t::صفحو_بٽ)
	    {
	      /* Too wide for one page; a digest pre-filter instead. */
	      لكل (hb_codepoint_t gid : سيٽ)
		bitmap.u.digest.جوڙيو (gid);
	      استمر;
	    }
	    bitmap.bias = min_gid;
	    لكل (hb_codepoint_t gid : سيٽ)
	      bitmap.u.صفحو.جوڙيو (gid - min_gid);
	  }
      }
#جيڪڏهن_پورو
    }
    ~accelerator_t () { جدول.destroy (); }

    بنانشان صحيح get_glyph_props (hb_codepoint_t glyph) ثابت
    {
      بنانشان v;

#جيڪڏهن_وصف_نه_ٿيل HB_NO_GDEF_CACHE
      إذا (glyph_props_cache.get (glyph, &v))
        أرجع v;
#جيڪڏهن_پورو

      v = جدول->get_glyph_props (glyph);

#جيڪڏهن_وصف_نه_ٿيل HB_NO_GDEF_CACHE
      إذا (likely (جدول.get_blob ())) // Don't try setting if we are the null instance!
	glyph_props_cache.سيٽ (glyph, v);
#جيڪڏهن_پورو

      أرجع v;

    }

    HB_ALWAYS_INLINE
    منطقي mark_سيٽ_covers (بنانشان صحيح سيٽ_اشاريو, hb_codepoint_t glyph_id) ثابت
    {
#جيڪڏهن_وصف_نه_ٿيل HB_NO_GDEF_CACHE
      // We can access arrayZ directly because of sanitize_lookup_props() guarantee.
      ثابت خودڪار &bitmap = mark_glyph_سيٽ_bitmaps.arrayZ[سيٽ_اشاريو];
      إذا (likely (bitmap.bias != HB_سيٽ_قدر_INVALID))
      {
	hb_codepoint_t biased = glyph_id - bitmap.bias;
	أرجع biased < hb_ٻٽو_انگ_صفحو_t::صفحو_بٽ &&
	       bitmap.u.صفحو.get (biased);
      }
      أرجع mark_سيٽ_covers_slow (سيٽ_اشاريو, glyph_id);
#وإلا
      أرجع جدول->mark_سيٽ_covers (سيٽ_اشاريو, glyph_id);
#جيڪڏهن_پورو
    }

#جيڪڏهن_وصف_نه_ٿيل HB_NO_GDEF_CACHE
    HB_NEVER_INLINE
    منطقي mark_سيٽ_covers_slow (بنانشان صحيح سيٽ_اشاريو, hb_codepoint_t glyph_id) ثابت
    {
      ثابت خودڪار &bitmap = mark_glyph_سيٽ_bitmaps.arrayZ[سيٽ_اشاريو];
      إذا (!bitmap.u.digest.may_have (glyph_id))
	أرجع خطأ;
      أرجع جدول->mark_سيٽ_covers (سيٽ_اشاريو, glyph_id);
    }
#جيڪڏهن_پورو

    بنانشان sanitize_lookup_props (بنانشان lookup_props) ثابت
    {
#جيڪڏهن_وصف_نه_ٿيل HB_NO_GDEF_CACHE
      إذا (lookup_props & LookupFlag::Use_Mark_Filtering_سيٽ &&
	  (lookup_props >> 16) >= mark_glyph_سيٽ_bitmaps.ڊيگهائي)
      {
        // Invalid mark filtering set index; unset the flag.
	lookup_props &= ~LookupFlag::Use_Mark_Filtering_سيٽ;
      }
#جيڪڏهن_پورو
      أرجع lookup_props;
    }

    hb_blob_ptr_t<GDEF> جدول;
#جيڪڏهن_وصف_نه_ٿيل HB_NO_GDEF_CACHE
    بنية mark_glyph_سيٽ_bitmap_t
    {
      /* Gid of the first bit (not page-aligned) for an exact one-page
       * set, or HB_SET_VALUE_INVALID; then digest is a pre-filter for
       * the table.  The all-zeros state is valid for both members. */
      hb_codepoint_t bias = HB_سيٽ_قدر_INVALID;
      اتحاد u_t {
	u_t () : صفحو () {}
	hb_ٻٽو_انگ_صفحو_t صفحو;
	hb_سيٽ_digest_t digest;
      } u;
    };

    hb_ويڪٽر_t<mark_glyph_سيٽ_bitmap_t> mark_glyph_سيٽ_bitmaps;
    بدلجندڙ hb_cache_t<21, 3> glyph_props_cache;
    static_assert (حجم (glyph_props_cache) == 512, "");
#جيڪڏهن_پورو
  };

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    إذا (نسخو.منزل_int () >= 0x00010004u && ligCaretList2)
    {
      (هذا+ligCaretList2).collect_variation_indices (c);
      أرجع;
    }
#جيڪڏهن_پورو
    (هذا+ligCaretList).collect_variation_indices (c);
  }

  محمي:
  سانچو <قسم_نالو ڪنجي>
  ساكن فراغ remap_varidx_after_instantiation (ثابت hb_map_t& varidx_map,
                                                hb_hashmap_t<ڪنجي, hb_pair_t<بنانشان, صحيح>>& layout_variation_idx_delta_map /* IN/OUT */)
  {
    /* varidx_map is empty which means varstore is empty after instantiation,
     * no variations, map all varidx to HB_OT_LAYOUT_NO_VARIATIONS_INDEX.
     * varidx_map doesn't have original varidx, indicating delta row is all
     * zeros, map varidx to HB_OT_LAYOUT_NO_VARIATIONS_INDEX */
    لكل (خودڪار _ : layout_variation_idx_delta_map.iter_ref ())
    {
      /* old_varidx->(varidx, delta) mapping generated for subsetting, then this
       * varidx is used as key of varidx_map during instantiation */
      uint32_t varidx = _.second.پهريون;
      uint32_t *new_varidx;
      إذا (varidx_map.has (varidx, &new_varidx))
	_.second.پهريون = *new_varidx;
      وإلا
	_.second.پهريون = HB_OT_LAYOUT_NO_VARIATIONS_اشاريو;
    }
  }

  سانچو <قسم_نالو وٿي_ٻاهريون, قسم_نالو وٿي_اندريون, قسم_نالو بنياد_3, قسم_نالو ...Ts>
  ساكن منطقي serialize_subset_وٿي (hb_subset_context_t *c,
				       وٿي_ٻاهريون& ٻاهريون,
				       ثابت وٿي_اندريون& اندريون,
				       ثابت بنياد_3 *بنياد,
				       Ts&&... ds)
  {
    ٻاهريون = 0;
    إذا (اندريون.is_null ()) أرجع خطأ;

    خودڪار *s = c->serializer;
    s->push ();
    منطقي ret = c->dispatch (بنياد+اندريون, std::forward<Ts> (ds)...);
    إذا (ret) s->جوڙيو_link (ٻاهريون, s->ڪڍو_pack ());
    وإلا s->ڪڍو_discard ();
    أرجع ret;
  }

  Fixed_نسخو<>نسخو;		/* Version of the GDEF table--currently
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
  Offset32_منزل<شي_Variation_Store>
		varStore;		/* Offset to the table of Item Variation
					 * Store--from beginning of GDEF
					 * header (may be NULL).  Introduced
					 * in version 0x00010003. */
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
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
#جيڪڏهن_پورو
  عام:
  DEFINE_سائيز_MIN (12);
};

بنية GDEF_accelerator_t : GDEF::accelerator_t {
  GDEF_accelerator_t (hb_face_t *face) : GDEF::accelerator_t (face) {}
};

} /* namespace OT */


#جيڪڏهن_پورو /* OT_LAYOUT_GDEF_GDEF_HH */
