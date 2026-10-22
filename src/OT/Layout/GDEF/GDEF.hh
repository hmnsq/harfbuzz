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

#अन्तर्भाव "../../../hb-ot-var-common.hh"

#अन्तर्भाव "../../../hb-font.hh"
#अन्तर्भाव "../../../hb-cache.hh"


नामाकाश OT {


/*
 * Attachment List Table
 */

/* Array of contour point indices--in increasing numerical order */
संरचना AttachPoint : Array16Of<HBUINT16>
{
  तार्किक subset (hb_subset_context_t *c) अचर
  {
    TRACE_SUBSET (एतत्);
    स्वतः *out = c->serializer->start_embed (*एतत्);
    return_trace (out->serialize (c->serializer, + iter ()));
  }
};

/* AttachList is referenced by both GDEF attachListOffset and
 * attachListOffset2. The v1.4 top-level offset widens, but the
 * AttachList table itself keeps Offset16 internals. */
संरचना AttachList
{
  अचिन्हित पूर्णाङ्क get_attach_points (hb_codepoint_t glyph_id,
				  अचिन्हित पूर्णाङ्क start_offset,
				  अचिन्हित पूर्णाङ्क *point_गन्ती /* IN/OUT */,
				  अचिन्हित पूर्णाङ्क *point_array /* OUT */) अचर
  {
    अचिन्हित पूर्णाङ्क index = (एतत्+coverage).get_coverage (glyph_id);
    यदि (index == NOT_COVERED)
    {
      यदि (point_गन्ती)
	*point_गन्ती = 0;
      फर्काऊ 0;
    }

    अचर AttachPoint &points = एतत्+attachPoint[index];

    यदि (point_गन्ती && point_array)
    {
      + points.as_array ().sub_array (start_offset, point_गन्ती)
      | hb_sink (hb_array (point_array, *point_गन्ती))
      ;
    }

    फर्काऊ points.len;
  }

  तार्किक subset (hb_subset_context_t *c) अचर
  {
    TRACE_SUBSET (एतत्);
    अचर hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    अचर hb_map_t &glyph_map = *c->plan->glyph_map;

    स्वतः *out = c->serializer->start_embed (*एतत्);
    यदि (unlikely (!c->serializer->extend_min (out))) return_trace (असत्य);

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (एतत्+coverage, attachPoint)
    | hb_filter (glyphset, hb_first)
    | hb_filter (subset_offset_array (c, out->attachPoint, एतत्), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;
    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (तार्किक (new_coverage));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (coverage.sanitize (c, एतत्) && attachPoint.sanitize (c, एतत्));
  }

  रक्षित:
  Offset16To<Coverage>
		coverage;		/* Offset to Coverage table -- from
					 * beginning of AttachList table */
  Array16OfOffset16To<AttachPoint>
		attachPoint;		/* Array of AttachPoint tables
					 * in Coverage Index order */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (4, attachPoint);
};

/*
 * Ligature Caret Table
 */

संरचना Caret_मान_Format1
{
  मित्र संरचना Caret_मान;
  तार्किक subset (hb_subset_context_t *c) अचर
  {
    TRACE_SUBSET (एतत्);
    स्वतः *out = c->serializer->embed (एतत्);
    यदि (unlikely (!out)) return_trace (असत्य);
    return_trace (सत्य);
  }

  गुप्त:
  hb_position_t get_caret_मान (hb_font_t *font, hb_direction_t direction) अचर
  {
    फर्काऊ HB_DIRECTION_IS_HORIZONTAL (direction) ? font->em_scale_x (coordinate) : font->em_scale_y (coordinate);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्));
  }

  रक्षित:
  HBUINT16	caret_मान_Format;	/* Format identifier--format = 1 */
  FWORD		coordinate;		/* X or Y value, in design units */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (4);
};

संरचना Caret_मान_Format2
{
  मित्र संरचना Caret_मान;
  तार्किक subset (hb_subset_context_t *c) अचर
  {
    TRACE_SUBSET (एतत्);
    स्वतः *out = c->serializer->embed (एतत्);
    यदि (unlikely (!out)) return_trace (असत्य);
    return_trace (सत्य);
  }

  गुप्त:
  hb_position_t get_caret_मान (hb_font_t *font, hb_direction_t direction, hb_codepoint_t glyph_id) अचर
  {
    hb_position_t x, y;
    font->get_glyph_contour_point_for_origin (glyph_id, caret_मान_Point, direction, &x, &y);
    फर्काऊ HB_DIRECTION_IS_HORIZONTAL (direction) ? x : y;
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्));
  }

  रक्षित:
  HBUINT16	caret_मान_Format;	/* Format identifier--format = 2 */
  HBUINT16	caret_मान_Point;	/* Contour point index on glyph */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (4);
};

संरचना Caret_मान_Format3
{
  मित्र संरचना Caret_मान;

  hb_position_t get_caret_मान (hb_font_t *font, hb_direction_t direction,
				 अचर ItemVariationStore &var_store) अचर
  {
    फर्काऊ HB_DIRECTION_IS_HORIZONTAL (direction) ?
	   hb_saturate_add (font->em_scale_x (coordinate), (एतत्+deviceTable).get_x_delta (font, var_store)) :
	   hb_saturate_add (font->em_scale_y (coordinate), (एतत्+deviceTable).get_y_delta (font, var_store));
  }

  तार्किक subset (hb_subset_context_t *c) अचर
  {
    TRACE_SUBSET (एतत्);
    स्वतः *out = c->serializer->start_embed (*एतत्);
    यदि (!c->serializer->embed (caret_मान_Format)) return_trace (असत्य);
    यदि (!c->serializer->embed (coordinate)) return_trace (असत्य);

    अचिन्हित varidx = (एतत्+deviceTable).get_variation_index ();
    hb_pair_t<अचिन्हित, पूर्णाङ्क> *new_varidx_delta;
    यदि (c->plan->layout_variation_idx_delta_map.has (varidx, &new_varidx_delta)) {
      uint32_t new_varidx = hb_first (*new_varidx_delta);
      पूर्णाङ्क delta = hb_second (*new_varidx_delta);
      यदि (delta != 0)
      {
        यदि (!c->serializer->check_assign (out->coordinate, coordinate + delta, HB_SERIALIZE_ERROR_INT_OVERFLOW))
          return_trace (असत्य);
      }

      यदि (new_varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
        return_trace (c->serializer->check_assign (out->caret_मान_Format, 1, HB_SERIALIZE_ERROR_INT_OVERFLOW));
    }

    यदि (!c->serializer->embed (deviceTable))
      return_trace (असत्य);

    return_trace (out->deviceTable.serialize_copy (c->serializer, deviceTable, एतत्, c->serializer->to_bias (out),
						   hb_serialize_context_t::Head, &c->plan->layout_variation_idx_delta_map));
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) अचर
  { (एतत्+deviceTable).collect_variation_indices (c); }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) && deviceTable.sanitize (c, एतत्));
  }

  रक्षित:
  HBUINT16	caret_मान_Format;	/* Format identifier--format = 3 */
  FWORD		coordinate;		/* X or Y value, in design units */
  Offset16To<Device>
		deviceTable;		/* Offset to Device table for X or Y
					 * value--from beginning of CaretValue
					 * table */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (6);
};

संरचना Caret_मान
{
  hb_position_t get_caret_मान (hb_font_t *font,
				 hb_direction_t direction,
				 hb_codepoint_t glyph_id,
				 अचर ItemVariationStore &var_store) अचर
  {
    विकल्प (u.format.v) {
    पक्ष 1: hb_barrier (); फर्काऊ u.format1.get_caret_मान (font, direction);
    पक्ष 2: hb_barrier (); फर्काऊ u.format2.get_caret_मान (font, direction, glyph_id);
    पक्ष 3: hb_barrier (); फर्काऊ u.format3.get_caret_मान (font, direction, var_store);
    स्वभाव:फर्काऊ 0;
    }
  }

  ढाँचा <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) अचर
  {
    यदि (unlikely (!c->may_dispatch (एतत्, &u.format.v))) फर्काऊ c->no_dispatch_return_मान ();
    TRACE_DISPATCH (एतत्, u.format.v);
    विकल्प (u.format.v) {
    पक्ष 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    पक्ष 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    पक्ष 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    स्वभाव:return_trace (c->default_return_मान ());
    }
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) अचर
  {
    विकल्प (u.format.v) {
    पक्ष 1:
    पक्ष 2:
      फर्काऊ;
    पक्ष 3:
      hb_barrier ();
      u.format3.collect_variation_indices (c);
      फर्काऊ;
    स्वभाव: फर्काऊ;
    }
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    यदि (!u.format.v.sanitize (c)) return_trace (असत्य);
    hb_barrier ();
    विकल्प (u.format.v) {
    पक्ष 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    पक्ष 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    पक्ष 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    स्वभाव:return_trace (सत्य);
    }
  }

  रक्षित:
  सङ्घ {
  संरचना { HBUINT16 v; }	format;		/* Format identifier */
  Caret_मान_Format1	format1;
  Caret_मान_Format2	format2;
  Caret_मान_Format3	format3;
  } u;
  सार्वजनिक:
  DEFINE_SIZE_UNION (2, format.v);
};

संरचना LigGlyph
{
  अचिन्हित get_lig_carets (hb_font_t            *font,
			   hb_direction_t        direction,
			   hb_codepoint_t        glyph_id,
			   अचर ItemVariationStore &var_store,
			   अचिन्हित              start_offset,
			   अचिन्हित             *caret_गन्ती /* IN/OUT */,
			   hb_position_t        *caret_array /* OUT */) अचर
  {
    यदि (caret_गन्ती && caret_array)
    {
      + carets.as_array ().sub_array (start_offset, caret_गन्ती)
      | hb_map (hb_add (एतत्))
      | hb_map ([&] (अचर Caret_मान &value) { फर्काऊ value.get_caret_मान (font, direction, glyph_id, var_store); })
      | hb_sink (hb_array (caret_array, *caret_गन्ती))
      ;
    }

    फर्काऊ carets.len;
  }

  तार्किक subset (hb_subset_context_t *c) अचर
  {
    TRACE_SUBSET (एतत्);
    स्वतः *out = c->serializer->start_embed (*एतत्);
    यदि (unlikely (!c->serializer->extend_min (out))) return_trace (असत्य);

    + hb_iter (carets)
    | hb_apply (subset_offset_array (c, out->carets, एतत्))
    ;

    return_trace (तार्किक (out->carets));
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) अचर
  {
    लागि (अचर Offset16To<Caret_मान>& offset : carets.iter ())
      (एतत्+offset).collect_variation_indices (c);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (carets.sanitize (c, एतत्));
  }

  रक्षित:
  Array16OfOffset16To<Caret_मान>
		carets;			/* Offset array of CaretValue tables
					 * --from beginning of LigGlyph table
					 * --in increasing coordinate order */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (2, carets);
};

/* LigCaretList2 widens the LigCaretList coverage, count, and LigGlyph
 * offsets. The LigGlyph table itself keeps Offset16 caretValueOffsets. */
ढाँचा <typename Types>
संरचना LigCaretList
{
  अचिन्हित पूर्णाङ्क get_lig_carets (hb_font_t *font,
			       hb_direction_t direction,
			       hb_codepoint_t glyph_id,
			       अचर ItemVariationStore &var_store,
			       अचिन्हित पूर्णाङ्क start_offset,
			       अचिन्हित पूर्णाङ्क *caret_गन्ती /* IN/OUT */,
			       hb_position_t *caret_array /* OUT */) अचर
  {
    अचिन्हित पूर्णाङ्क index = (एतत्+coverage).get_coverage (glyph_id);
    यदि (index == NOT_COVERED)
    {
      यदि (caret_गन्ती)
	*caret_गन्ती = 0;
      फर्काऊ 0;
    }
    अचर LigGlyph &lig_glyph = एतत्+ligGlyph[index];
    फर्काऊ lig_glyph.get_lig_carets (font, direction, glyph_id, var_store, start_offset, caret_गन्ती, caret_array);
  }

  तार्किक subset (hb_subset_context_t *c) अचर
  {
    TRACE_SUBSET (एतत्);
    अचर hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    अचर hb_map_t &glyph_map = *c->plan->glyph_map;

    स्वतः *out = c->serializer->start_embed (*एतत्);
    यदि (unlikely (!c->serializer->extend_min (out))) return_trace (असत्य);

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (एतत्+coverage, ligGlyph)
    | hb_filter (glyphset, hb_first)
    | hb_filter (subset_offset_array (c, out->ligGlyph, एतत्), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;
    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (तार्किक (new_coverage));
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) अचर
  {
    + hb_zip (एतत्+coverage, ligGlyph)
    | hb_filter (c->glyph_set, hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (एतत्))
    | hb_apply ([c] (अचर LigGlyph& _) { _.collect_variation_indices (c); })
    ;
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (coverage.sanitize (c, एतत्) && ligGlyph.sanitize (c, एतत्));
  }

  रक्षित:
  typename Types::ढाँचा LOffsetTo<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of LigCaretList table */
  typename Types::ढाँचा ArrayOf<typename Types::ढाँचा OffsetTo<LigGlyph>>
		ligGlyph;		/* Array of LigGlyph tables
					 * in Coverage Index order */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (Types::LOffset::static_size + Types::HBUINT::static_size, ligGlyph);
};

#ifndef HB_NO_BEYOND_64K
static_assert (LigCaretList<MediumTypes>::min_size ==
	       Offset32::static_size + HBUINT24::static_size, "");
#endif


संरचना MarkGlyphSetsFormat1
{
  तार्किक covers (अचिन्हित पूर्णाङ्क set_index, hb_codepoint_t glyph_id) अचर
  { फर्काऊ (एतत्+coverage[set_index]).get_coverage (glyph_id) != NOT_COVERED; }

  शून्य collect_used_mark_sets (अचर hb_set_t& glyph_set,
                               hb_set_t& used_mark_sets /* OUT */) अचर
  {
    अचिन्हित i = 0;
    लागि (अचर स्वतः &offset : coverage)
     {
       अचर स्वतः &cov = एतत्+offset;
       यदि (cov.intersects (&glyph_set))
         used_mark_sets.add (i);

       i++;
     }
  }

  ढाँचा <typename set_t>
  शून्य collect_coverage (hb_vector_t<set_t> &sets) अचर
  {
     लागि (अचर स्वतः &offset : coverage)
     {
       अचर स्वतः &cov = एतत्+offset;
       cov.collect_coverage (sets.push ());
     }
  }

  तार्किक subset (hb_subset_context_t *c) अचर
  {
    TRACE_SUBSET (एतत्);
    स्वतः *out = c->serializer->start_embed (*एतत्);
    यदि (unlikely (!c->serializer->extend_min (out))) return_trace (असत्य);
    out->format = format;

    तार्किक ret = सत्य;
    लागि (अचर Offset32To<Coverage>& offset : coverage.iter ())
    {
      स्वतः snap = c->serializer->snapshot ();
      स्वतः *o = out->coverage.serialize_append (c->serializer);
      यदि (unlikely (!o))
      {
	ret = असत्य;
	रोक;
      }

      //skip empty coverage
      c->serializer->push ();
      तार्किक res = असत्य;
      यदि (offset) res = c->dispatch (एतत्+offset);
      यदि (!res)
      {
        c->serializer->झिक_discard ();
        c->serializer->revert (snap);
        (out->coverage.len)--;
        जारी;
      }
      c->serializer->add_link (*o, c->serializer->झिक_pack ());
    }

    return_trace (ret && out->coverage.len);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (coverage.sanitize (c, एतत्));
  }

  रक्षित:
  HBUINT16	format;			/* Format identifier--format = 1 */
  Array16Of<Offset32To<Coverage>>
		coverage;		/* Array of long offsets to mark set
					 * coverage tables */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (4, coverage);
};

संरचना MarkGlyphSets
{
  तार्किक covers (अचिन्हित पूर्णाङ्क set_index, hb_codepoint_t glyph_id) अचर
  {
    विकल्प (u.format.v) {
    पक्ष 1: hb_barrier (); फर्काऊ u.format1.covers (set_index, glyph_id);
    स्वभाव:फर्काऊ असत्य;
    }
  }

  ढाँचा <typename set_t>
  शून्य collect_coverage (hb_vector_t<set_t> &sets) अचर
  {
    विकल्प (u.format.v) {
    पक्ष 1: hb_barrier (); u.format1.collect_coverage (sets); फर्काऊ;
    स्वभाव:फर्काऊ;
    }
  }

  शून्य collect_used_mark_sets (अचर hb_set_t& glyph_set,
                               hb_set_t& used_mark_sets /* OUT */) अचर
  {
    विकल्प (u.format.v) {
    पक्ष 1: hb_barrier (); u.format1.collect_used_mark_sets (glyph_set, used_mark_sets); फर्काऊ;
    स्वभाव:फर्काऊ;
    }
  }

  तार्किक subset (hb_subset_context_t *c) अचर
  {
    TRACE_SUBSET (एतत्);
    विकल्प (u.format.v) {
    पक्ष 1: hb_barrier (); return_trace (u.format1.subset (c));
    स्वभाव:return_trace (असत्य);
    }
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    यदि (!u.format.v.sanitize (c)) return_trace (असत्य);
    hb_barrier ();
    विकल्प (u.format.v) {
    पक्ष 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    स्वभाव:return_trace (सत्य);
    }
  }

  रक्षित:
  सङ्घ {
  संरचना { HBUINT16 v; }	format;		/* Format identifier */
  MarkGlyphSetsFormat1	format1;
  } u;
  सार्वजनिक:
  DEFINE_SIZE_UNION (2, format.v);
};


/*
 * GDEF -- Glyph Definition
 * https://docs.microsoft.com/en-us/typography/opentype/spec/gdef
 */


संरचना GDEF
{
  स्थिर constexpr hb_tag_t tableTag = HB_OT_TAG_GDEF;

  गणना GlyphClasses {
    UnclassifiedGlyph	= 0,
    BaseGlyph		= 1,
    LigatureGlyph	= 2,
    MarkGlyph		= 3,
    ComponentGlyph	= 4
  };

  माप_प्रकार get_size () अचर
  {
    यदि (version.major != 1) फर्काऊ version.static_size;

    फर्काऊ min_size +
	   (version.to_int () >= 0x00010002u ? markGlyphSetsDef.static_size : 0) +
	   (version.to_int () >= 0x00010003u ? varStore.static_size : 0) +
#ifndef HB_NO_BEYOND_64K
	   (version.to_int () >= 0x00010004u ? 5 * Offset32::static_size : 0)
#अन्यथा
	   0
#endif
	   ;
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    यदि (unlikely (!version.sanitize (c) ||
		  version.major != 1 ||
		  !c->check_range (एतत्, get_size ())))
      return_trace (असत्य);
    hb_barrier ();

    तार्किक glyph_class_def_ok = glyphClassDef.sanitize (c, एतत्);
    तार्किक attach_list_ok = attachList.sanitize (c, एतत्);
    तार्किक lig_caret_list_ok = ligCaretList.sanitize (c, एतत्);
    तार्किक mark_attach_class_def_ok = markAttachClassDef.sanitize (c, एतत्);
    तार्किक mark_glyph_sets_def_ok = version.to_int () < 0x00010002u ||
				  markGlyphSetsDef.sanitize (c, एतत्);

#ifndef HB_NO_BEYOND_64K
    यदि (version.to_int () >= 0x00010004u)
    {
      यदि (glyphClassDef2) glyph_class_def_ok = glyphClassDef2.sanitize (c, एतत्);
      यदि (attachList2) attach_list_ok = attachList2.sanitize (c, एतत्);
      यदि (ligCaretList2) lig_caret_list_ok = ligCaretList2.sanitize (c, एतत्);
      यदि (markAttachClassDef2) mark_attach_class_def_ok = markAttachClassDef2.sanitize (c, एतत्);
      यदि (markGlyphSetsDef2) mark_glyph_sets_def_ok = markGlyphSetsDef2.sanitize (c, एतत्);
    }
#endif

    return_trace (glyph_class_def_ok &&
		  attach_list_ok &&
		  lig_caret_list_ok &&
		  mark_attach_class_def_ok &&
		  mark_glyph_sets_def_ok &&
		  (version.to_int () < 0x00010003u || varStore.sanitize (c, एतत्)));
  }

  तार्किक subset (hb_subset_context_t *c) अचर
  {
    TRACE_SUBSET (एतत्);
    यदि (version.major != 1) return_trace (असत्य);

    स्वतः *out = c->serializer->start_embed (*एतत्);
    यदि (unlikely (!c->serializer->extend_min (out))) return_trace (असत्य);

    // Push var store first (if it's needed) so that it's last in the
    // serialization order. Some font consumers assume that varstore runs to
    // the end of the GDEF table.
    // See: https://github.com/harfbuzz/harfbuzz/issues/4636
    स्वतः snapshot_version0 = c->serializer->snapshot ();
    यदि (unlikely (version.to_int () >= 0x00010002u && hb_barrier () &&
		  !c->serializer->embed (markGlyphSetsDef)))
      return_trace (असत्य);

    स्वतः snapshot_version2 = c->serializer->snapshot ();
    यदि (unlikely (version.to_int () >= 0x00010003u && hb_barrier () &&
		  !c->serializer->embed (varStore)))
      return_trace (असत्य);

    तार्किक use_v14 = असत्य;
#ifndef HB_NO_BEYOND_64K
    use_v14 = version.to_int () >= 0x00010004u &&
	      (glyphClassDef2 || attachList2 || ligCaretList2 ||
	       markAttachClassDef2 || markGlyphSetsDef2);
    यदि (unlikely (version.to_int () >= 0x00010004u && hb_barrier () &&
		  !c->serializer->extend_size (out, get_size ())))
      return_trace (असत्य);
#endif

    तार्किक subset_varstore = असत्य;
    अचिन्हित varstore_index = (अचिन्हित) -1;
    यदि (version.to_int () >= 0x00010003u && hb_barrier ())
    {
      यदि (c->plan->all_axes_pinned)
	out->varStore = 0;
      अन्यथा यदि (c->plan->normalized_coords)
      {
	यदि (varStore)
	{
	  item_variations_t item_vars;
	  यदि (unlikely (!c->serializer->check_success (
                  item_vars.instantiate (एतत्+varStore, c->plan, सत्य, सत्य,
                                          c->plan->gdef_varstore_inner_maps.as_array (), सत्य))))
            return_trace (असत्य);
	  subset_varstore = out->varStore.serialize_serialize (c->serializer,
							     item_vars.has_long_word (),
							     c->plan->axis_tags,
							     item_vars.get_region_list (),
							     item_vars.get_vardata_encodings ());
	  varstore_index = c->serializer->last_added_child_index();
	  remap_varidx_after_instantiation (item_vars.get_varidx_map (),
					    c->plan->layout_variation_idx_delta_map);
          remap_varidx_after_instantiation (item_vars.get_varidx_map (),
                                            c->plan->layout_condition_idx_मान_map);
	}
      }
      अन्यथा
      {
	subset_varstore = out->varStore.serialize_subset (c, varStore, एतत्,
							  c->plan->gdef_varstore_inner_maps.as_array ());
	varstore_index = c->serializer->last_added_child_index();
      }
    }

    out->version.major = version.major;
    out->version.minor = version.minor;

    यदि (!subset_varstore && version.to_int () >= 0x00010002u && !use_v14) {
      c->serializer->revert (snapshot_version2);
    }
    यदि (!subset_varstore)
      out->varStore = 0;
    अन्यथा
      c->plan->has_gdef_varstore = सत्य;

    तार्किक subset_markglyphsetsdef = असत्य;
    यदि (version.to_int () >= 0x00010002u && hb_barrier ())
    {
#ifndef HB_NO_BEYOND_64K
      यदि (use_v14 && markGlyphSetsDef2)
      {
	out->markGlyphSetsDef = 0;
	subset_markglyphsetsdef = serialize_subset_offset (c, out->markGlyphSetsDef2,
							   markGlyphSetsDef2, एतत्);
      }
      अन्यथा
#endif
      {
#ifndef HB_NO_BEYOND_64K
	यदि (use_v14) out->markGlyphSetsDef2 = 0;
#endif
	subset_markglyphsetsdef = out->markGlyphSetsDef.serialize_subset (c, markGlyphSetsDef, एतत्);
      }
    }

    यदि (use_v14)
    {
      out->version.minor = 4;
    } अन्यथा यदि (subset_varstore) {
      out->version.minor = 3;
    } अन्यथा यदि (subset_markglyphsetsdef) {
      out->version.minor = 2;
    } अन्यथा  {
      out->version.minor = 0;
      c->serializer->revert (snapshot_version0);
    }

    तार्किक subset_glyphclassdef = असत्य;
    तार्किक subset_attachlist = असत्य;
    तार्किक subset_markattachclassdef = असत्य;
    तार्किक subset_ligcaretlist = असत्य;

#ifndef HB_NO_BEYOND_64K
    यदि (use_v14 && glyphClassDef2)
    {
      out->glyphClassDef = 0;
      subset_glyphclassdef = serialize_subset_offset (c, out->glyphClassDef2,
						      glyphClassDef2, एतत्, nullptr, असत्य, सत्य);
    }
    अन्यथा
#endif
    {
#ifndef HB_NO_BEYOND_64K
      यदि (use_v14) out->glyphClassDef2 = 0;
#endif
      subset_glyphclassdef = out->glyphClassDef.serialize_subset (c, glyphClassDef, एतत्, nullptr, असत्य, सत्य);
    }

#ifndef HB_NO_BEYOND_64K
    यदि (use_v14 && attachList2)
    {
      out->attachList = 0;
      subset_attachlist = serialize_subset_offset (c, out->attachList2, attachList2, एतत्);
    }
    अन्यथा
#endif
    {
#ifndef HB_NO_BEYOND_64K
      यदि (use_v14) out->attachList2 = 0;
#endif
      subset_attachlist = out->attachList.serialize_subset (c, attachList, एतत्);
    }

#ifndef HB_NO_BEYOND_64K
    यदि (use_v14 && markAttachClassDef2)
    {
      out->markAttachClassDef = 0;
      subset_markattachclassdef = serialize_subset_offset (c, out->markAttachClassDef2,
							   markAttachClassDef2, एतत्, nullptr, असत्य, सत्य);
    }
    अन्यथा
#endif
    {
#ifndef HB_NO_BEYOND_64K
      यदि (use_v14) out->markAttachClassDef2 = 0;
#endif
      subset_markattachclassdef = out->markAttachClassDef.serialize_subset (c, markAttachClassDef,
									   एतत्, nullptr, असत्य, सत्य);
    }

#ifndef HB_NO_BEYOND_64K
    यदि (use_v14 && ligCaretList2)
    {
      out->ligCaretList = 0;
      subset_ligcaretlist = serialize_subset_offset (c, out->ligCaretList2, ligCaretList2, एतत्);
    }
    अन्यथा
#endif
    {
#ifndef HB_NO_BEYOND_64K
      यदि (use_v14) out->ligCaretList2 = 0;
#endif
      subset_ligcaretlist = out->ligCaretList.serialize_subset (c, ligCaretList, एतत्);
    }

    यदि (subset_varstore && varstore_index != (अचिन्हित) -1) {
      c->serializer->repack_last(varstore_index);
    }

    return_trace (subset_glyphclassdef || subset_attachlist ||
		  subset_ligcaretlist || subset_markattachclassdef ||
		  (out->version.to_int () >= 0x00010002u && subset_markglyphsetsdef) ||
		  (out->version.to_int () >= 0x00010003u && subset_varstore));
  }

  तार्किक has_glyph_classes () अचर
  {
#ifndef HB_NO_BEYOND_64K
    यदि (version.to_int () >= 0x00010004u && glyphClassDef2) फर्काऊ सत्य;
#endif
    फर्काऊ glyphClassDef != 0;
  }
  अचर ClassDef &get_glyph_class_def () अचर
  {
#ifndef HB_NO_BEYOND_64K
    यदि (version.to_int () >= 0x00010004u && glyphClassDef2) फर्काऊ एतत्+glyphClassDef2;
#endif
    फर्काऊ एतत्+glyphClassDef;
  }
  तार्किक has_attach_list () अचर
  {
#ifndef HB_NO_BEYOND_64K
    यदि (version.to_int () >= 0x00010004u && attachList2) फर्काऊ सत्य;
#endif
    फर्काऊ attachList != 0;
  }
  अचर AttachList &get_attach_list () अचर
  {
#ifndef HB_NO_BEYOND_64K
    यदि (version.to_int () >= 0x00010004u && attachList2) फर्काऊ एतत्+attachList2;
#endif
    फर्काऊ एतत्+attachList;
  }
  तार्किक has_lig_carets () अचर
  {
#ifndef HB_NO_BEYOND_64K
    यदि (version.to_int () >= 0x00010004u && ligCaretList2) फर्काऊ सत्य;
#endif
    फर्काऊ ligCaretList != 0;
  }
  तार्किक has_mark_attachment_types () अचर
  {
#ifndef HB_NO_BEYOND_64K
    यदि (version.to_int () >= 0x00010004u && markAttachClassDef2) फर्काऊ सत्य;
#endif
    फर्काऊ markAttachClassDef != 0;
  }
  अचर ClassDef &get_mark_attach_class_def () अचर
  {
#ifndef HB_NO_BEYOND_64K
    यदि (version.to_int () >= 0x00010004u && markAttachClassDef2) फर्काऊ एतत्+markAttachClassDef2;
#endif
    फर्काऊ एतत्+markAttachClassDef;
  }
  तार्किक has_mark_glyph_sets () अचर
  {
    यदि (version.to_int () < 0x00010002u) फर्काऊ असत्य;
#ifndef HB_NO_BEYOND_64K
    यदि (version.to_int () >= 0x00010004u && markGlyphSetsDef2) फर्काऊ सत्य;
#endif
    फर्काऊ markGlyphSetsDef != 0;
  }
  अचर MarkGlyphSets &get_mark_glyph_sets () अचर
  {
    यदि (version.to_int () < 0x00010002u) फर्काऊ Null(MarkGlyphSets);
#ifndef HB_NO_BEYOND_64K
    यदि (version.to_int () >= 0x00010004u && markGlyphSetsDef2) फर्काऊ एतत्+markGlyphSetsDef2;
#endif
    फर्काऊ एतत्+markGlyphSetsDef;
  }
  तार्किक has_var_store () अचर
  {
    फर्काऊ version.to_int () >= 0x00010003u && hb_barrier () && varStore != 0;
  }
  अचर ItemVariationStore &get_var_store () अचर
  {
    फर्काऊ version.to_int () >= 0x00010003u && hb_barrier () ? एतत्+varStore : Null(ItemVariationStore);
  }


  तार्किक has_data () अचर { फर्काऊ version.to_int (); }
  अचिन्हित पूर्णाङ्क get_glyph_class (hb_codepoint_t glyph) अचर
  { फर्काऊ get_glyph_class_def ().get_class (glyph); }
  शून्य get_glyphs_in_class (अचिन्हित पूर्णाङ्क klass, hb_set_t *glyphs) अचर
  { get_glyph_class_def ().collect_class (glyphs, klass); }

  अचिन्हित पूर्णाङ्क get_mark_attachment_type (hb_codepoint_t glyph) अचर
  { फर्काऊ get_mark_attach_class_def ().get_class (glyph); }

  अचिन्हित पूर्णाङ्क get_attach_points (hb_codepoint_t glyph_id,
				  अचिन्हित पूर्णाङ्क start_offset,
				  अचिन्हित पूर्णाङ्क *point_गन्ती /* IN/OUT */,
				  अचिन्हित पूर्णाङ्क *point_array /* OUT */) अचर
  { फर्काऊ get_attach_list ().get_attach_points (glyph_id, start_offset, point_गन्ती, point_array); }

  अचिन्हित पूर्णाङ्क get_lig_carets (hb_font_t *font,
			       hb_direction_t direction,
			       hb_codepoint_t glyph_id,
			       अचिन्हित पूर्णाङ्क start_offset,
			       अचिन्हित पूर्णाङ्क *caret_गन्ती /* IN/OUT */,
			       hb_position_t *caret_array /* OUT */) अचर
  {
#ifndef HB_NO_BEYOND_64K
    यदि (version.to_int () >= 0x00010004u && ligCaretList2)
      फर्काऊ (एतत्+ligCaretList2).get_lig_carets (font, direction, glyph_id, get_var_store (),
						  start_offset, caret_गन्ती, caret_array);
#endif
    फर्काऊ (एतत्+ligCaretList).get_lig_carets (font, direction, glyph_id, get_var_store (),
					       start_offset, caret_गन्ती, caret_array);
  }

  तार्किक mark_set_covers (अचिन्हित पूर्णाङ्क set_index, hb_codepoint_t glyph_id) अचर
  { फर्काऊ get_mark_glyph_sets ().covers (set_index, glyph_id); }

  /* glyph_props is a 16-bit integer where the lower 8-bit have bits representing
   * glyph class and other bits, and high 8-bit the mark attachment type (if any).
   * Not to be confused with lookup_props which is very similar. */
  अचिन्हित पूर्णाङ्क get_glyph_props (hb_codepoint_t glyph) अचर
  {
    अचिन्हित पूर्णाङ्क klass = get_glyph_class (glyph);

    static_assert (((अचिन्हित पूर्णाङ्क) HB_OT_LAYOUT_GLYPH_PROPS_BASE_GLYPH == (अचिन्हित पूर्णाङ्क) LookupFlag::IgnoreBaseGlyphs), "");
    static_assert (((अचिन्हित पूर्णाङ्क) HB_OT_LAYOUT_GLYPH_PROPS_LIGATURE == (अचिन्हित पूर्णाङ्क) LookupFlag::IgnoreLigatures), "");
    static_assert (((अचिन्हित पूर्णाङ्क) HB_OT_LAYOUT_GLYPH_PROPS_MARK == (अचिन्हित पूर्णाङ्क) LookupFlag::IgnoreMarks), "");

    विकल्प (klass) {
    स्वभाव:			फर्काऊ HB_OT_LAYOUT_GLYPH_CLASS_UNCLASSIFIED;
    पक्ष BaseGlyph:		फर्काऊ HB_OT_LAYOUT_GLYPH_PROPS_BASE_GLYPH;
    पक्ष LigatureGlyph:		फर्काऊ HB_OT_LAYOUT_GLYPH_PROPS_LIGATURE;
    पक्ष MarkGlyph:
	  klass = get_mark_attachment_type (glyph);
	  फर्काऊ HB_OT_LAYOUT_GLYPH_PROPS_MARK | (klass << 8);
    }
  }

  HB_INTERNAL तार्किक is_blocklisted (hb_blob_t *blob,
				   hb_face_t *face) अचर;

  संरचना accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      table = hb_sanitize_context_t ().reference_table<GDEF> (face);
      यदि (unlikely (table->is_blocklisted (table.get_blob (), face)))
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
	यदि (!sets.in_error () &&
	    mark_glyph_set_bitmaps.resize (sets.length))
	  लागि (अचिन्हित i = 0; i < sets.length; i++)
	  {
	    स्वतः &bitmap = mark_glyph_set_bitmaps.arrayZ[i];
	    bitmap.bias = HB_SET_मान_INVALID;
	    अचर स्वतः &set = sets.arrayZ[i];
	    यदि (unlikely (set.in_error ())) जारी;
	    hb_codepoint_t min_gid = HB_SET_मान_INVALID;
	    hb_codepoint_t max_gid = HB_SET_मान_INVALID;
	    hb_set_next (&set, &min_gid);
	    hb_set_previous (&set, &max_gid);
	    यदि (max_gid == HB_SET_मान_INVALID)
	      जारी;
	    यदि (max_gid - min_gid >= hb_bit_page_t::PAGE_BITS)
	    {
	      /* Too wide for one page; a digest pre-filter instead. */
	      लागि (hb_codepoint_t gid : set)
		bitmap.u.digest.add (gid);
	      जारी;
	    }
	    bitmap.bias = min_gid;
	    लागि (hb_codepoint_t gid : set)
	      bitmap.u.page.add (gid - min_gid);
	  }
      }
#endif
    }
    ~accelerator_t () { table.destroy (); }

    अचिन्हित पूर्णाङ्क get_glyph_props (hb_codepoint_t glyph) अचर
    {
      अचिन्हित v;

#ifndef HB_NO_GDEF_CACHE
      यदि (glyph_props_cache.get (glyph, &v))
        फर्काऊ v;
#endif

      v = table->get_glyph_props (glyph);

#ifndef HB_NO_GDEF_CACHE
      यदि (likely (table.get_blob ())) // Don't try setting if we are the null instance!
	glyph_props_cache.set (glyph, v);
#endif

      फर्काऊ v;

    }

    HB_ALWAYS_INLINE
    तार्किक mark_set_covers (अचिन्हित पूर्णाङ्क set_index, hb_codepoint_t glyph_id) अचर
    {
#ifndef HB_NO_GDEF_CACHE
      // We can access arrayZ directly because of sanitize_lookup_props() guarantee.
      अचर स्वतः &bitmap = mark_glyph_set_bitmaps.arrayZ[set_index];
      यदि (likely (bitmap.bias != HB_SET_मान_INVALID))
      {
	hb_codepoint_t biased = glyph_id - bitmap.bias;
	फर्काऊ biased < hb_bit_page_t::PAGE_BITS &&
	       bitmap.u.page.get (biased);
      }
      फर्काऊ mark_set_covers_slow (set_index, glyph_id);
#अन्यथा
      फर्काऊ table->mark_set_covers (set_index, glyph_id);
#endif
    }

#ifndef HB_NO_GDEF_CACHE
    HB_NEVER_INLINE
    तार्किक mark_set_covers_slow (अचिन्हित पूर्णाङ्क set_index, hb_codepoint_t glyph_id) अचर
    {
      अचर स्वतः &bitmap = mark_glyph_set_bitmaps.arrayZ[set_index];
      यदि (!bitmap.u.digest.may_have (glyph_id))
	फर्काऊ असत्य;
      फर्काऊ table->mark_set_covers (set_index, glyph_id);
    }
#endif

    अचिन्हित sanitize_lookup_props (अचिन्हित lookup_props) अचर
    {
#ifndef HB_NO_GDEF_CACHE
      यदि (lookup_props & LookupFlag::UseMarkFilteringSet &&
	  (lookup_props >> 16) >= mark_glyph_set_bitmaps.length)
      {
        // Invalid mark filtering set index; unset the flag.
	lookup_props &= ~LookupFlag::UseMarkFilteringSet;
      }
#endif
      फर्काऊ lookup_props;
    }

    hb_blob_ptr_t<GDEF> table;
#ifndef HB_NO_GDEF_CACHE
    संरचना mark_glyph_set_bitmap_t
    {
      /* Gid of the first bit (not page-aligned) for an exact one-page
       * set, or HB_SET_VALUE_INVALID; then digest is a pre-filter for
       * the table.  The all-zeros state is valid for both members. */
      hb_codepoint_t bias = HB_SET_मान_INVALID;
      सङ्घ u_t {
	u_t () : page () {}
	hb_bit_page_t page;
	hb_set_digest_t digest;
      } u;
    };

    hb_vector_t<mark_glyph_set_bitmap_t> mark_glyph_set_bitmaps;
    mutable hb_cache_t<21, 3> glyph_props_cache;
    static_assert (परिमाण (glyph_props_cache) == 512, "");
#endif
  };

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) अचर
  {
#ifndef HB_NO_BEYOND_64K
    यदि (version.to_int () >= 0x00010004u && ligCaretList2)
    {
      (एतत्+ligCaretList2).collect_variation_indices (c);
      फर्काऊ;
    }
#endif
    (एतत्+ligCaretList).collect_variation_indices (c);
  }

  रक्षित:
  ढाँचा <typename साँचो>
  स्थिर शून्य remap_varidx_after_instantiation (अचर hb_map_t& varidx_map,
                                                hb_hashmap_t<साँचो, hb_pair_t<अचिन्हित, पूर्णाङ्क>>& layout_variation_idx_delta_map /* IN/OUT */)
  {
    /* varidx_map is empty which means varstore is empty after instantiation,
     * no variations, map all varidx to HB_OT_LAYOUT_NO_VARIATIONS_INDEX.
     * varidx_map doesn't have original varidx, indicating delta row is all
     * zeros, map varidx to HB_OT_LAYOUT_NO_VARIATIONS_INDEX */
    लागि (स्वतः _ : layout_variation_idx_delta_map.iter_ref ())
    {
      /* old_varidx->(varidx, delta) mapping generated for subsetting, then this
       * varidx is used as key of varidx_map during instantiation */
      uint32_t varidx = _.second.first;
      uint32_t *new_varidx;
      यदि (varidx_map.has (varidx, &new_varidx))
	_.second.first = *new_varidx;
      अन्यथा
	_.second.first = HB_OT_LAYOUT_NO_VARIATIONS_INDEX;
    }
  }

  ढाँचा <typename OffsetOut, typename OffsetIn, typename Base, typename ...Ts>
  स्थिर तार्किक serialize_subset_offset (hb_subset_context_t *c,
				       OffsetOut& out,
				       अचर OffsetIn& in,
				       अचर Base *base,
				       Ts&&... ds)
  {
    out = 0;
    यदि (in.is_null ()) फर्काऊ असत्य;

    स्वतः *s = c->serializer;
    s->push ();
    तार्किक ret = c->dispatch (base+in, std::forward<Ts> (ds)...);
    यदि (ret) s->add_link (out, s->झिक_pack ());
    अन्यथा s->झिक_discard ();
    फर्काऊ ret;
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
  सार्वजनिक:
  DEFINE_SIZE_MIN (12);
};

संरचना GDEF_accelerator_t : GDEF::accelerator_t {
  GDEF_accelerator_t (hb_face_t *face) : GDEF::accelerator_t (face) {}
};

} /* namespace OT */


#endif /* OT_LAYOUT_GDEF_GDEF_HH */
