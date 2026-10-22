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

#ଅନ୍ତର୍ଭାଵ "../../../hb-ot-var-common.hh"

#ଅନ୍ତର୍ଭାଵ "../../../hb-font.hh"
#ଅନ୍ତର୍ଭାଵ "../../../hb-cache.hh"


ନାମାକାଶ OT {


/*
 * Attachment List Table
 */

/* Array of contour point indices--in increasing numerical order */
ସଂରଚନା AttachPoint : Array16Of<HBUINT16>
{
  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    auto *out = c->serializer->start_embed (*ଏହି);
    return_trace (out->serialize (c->serializer, + iter ()));
  }
};

/* AttachList is referenced by both GDEF attachListOffset and
 * attachListOffset2. The v1.4 top-level offset widens, but the
 * AttachList table itself keeps Offset16 internals. */
ସଂରଚନା AttachList
{
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_attach_points (hb_codepoint_t glyph_id,
				  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start_offset,
				  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *point_ଗଣନା /* IN/OUT */,
				  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *point_array /* OUT */) ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = (ଏହି+coverage).get_coverage (glyph_id);
    ଯଦି (index == NOT_COVERED)
    {
      ଯଦି (point_ଗଣନା)
	*point_ଗଣନା = 0;
      ଫେରନ୍ତୁ 0;
    }

    ଅଚଳ AttachPoint &points = ଏହି+attachPoint[index];

    ଯଦି (point_ଗଣନା && point_array)
    {
      + points.as_array ().sub_array (start_offset, point_ଗଣନା)
      | hb_sink (hb_array (point_array, *point_ଗଣନା))
      ;
    }

    ଫେରନ୍ତୁ points.len;
  }

  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    ଅଚଳ hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ଅଚଳ hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*ଏହି);
    ଯଦି (unlikely (!c->serializer->extend_min (out))) return_trace (ମିଥ୍ୟା);

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (ଏହି+coverage, attachPoint)
    | hb_filter (glyphset, hb_first)
    | hb_filter (subset_offset_array (c, out->attachPoint, ଏହି), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;
    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (ତାର୍କିକ (new_coverage));
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (coverage.sanitize (c, ଏହି) && attachPoint.sanitize (c, ଏହି));
  }

  ସୁରକ୍ଷିତ:
  Offset16To<Coverage>
		coverage;		/* Offset to Coverage table -- from
					 * beginning of AttachList table */
  Array16OfOffset16To<AttachPoint>
		attachPoint;		/* Array of AttachPoint tables
					 * in Coverage Index order */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (4, attachPoint);
};

/*
 * Ligature Caret Table
 */

ସଂରଚନା Caret_ମୂଲ୍ୟ_Format1
{
  friend ସଂରଚନା Caret_ମୂଲ୍ୟ;
  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    auto *out = c->serializer->embed (ଏହି);
    ଯଦି (unlikely (!out)) return_trace (ମିଥ୍ୟା);
    return_trace (ସତ୍ୟ);
  }

  ବ୍ୟକ୍ତିଗତ:
  hb_position_t get_caret_ମୂଲ୍ୟ (hb_font_t *font, hb_direction_t direction) ଅଚଳ
  {
    ଫେରନ୍ତୁ HB_DIRECTION_IS_HORIZONTAL (direction) ? font->em_scale_x (coordinate) : font->em_scale_y (coordinate);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି));
  }

  ସୁରକ୍ଷିତ:
  HBUINT16	caret_ମୂଲ୍ୟ_Format;	/* Format identifier--format = 1 */
  FWORD		coordinate;		/* X or Y value, in design units */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (4);
};

ସଂରଚନା Caret_ମୂଲ୍ୟ_Format2
{
  friend ସଂରଚନା Caret_ମୂଲ୍ୟ;
  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    auto *out = c->serializer->embed (ଏହି);
    ଯଦି (unlikely (!out)) return_trace (ମିଥ୍ୟା);
    return_trace (ସତ୍ୟ);
  }

  ବ୍ୟକ୍ତିଗତ:
  hb_position_t get_caret_ମୂଲ୍ୟ (hb_font_t *font, hb_direction_t direction, hb_codepoint_t glyph_id) ଅଚଳ
  {
    hb_position_t x, y;
    font->get_glyph_contour_point_for_origin (glyph_id, caret_ମୂଲ୍ୟ_Point, direction, &x, &y);
    ଫେରନ୍ତୁ HB_DIRECTION_IS_HORIZONTAL (direction) ? x : y;
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି));
  }

  ସୁରକ୍ଷିତ:
  HBUINT16	caret_ମୂଲ୍ୟ_Format;	/* Format identifier--format = 2 */
  HBUINT16	caret_ମୂଲ୍ୟ_Point;	/* Contour point index on glyph */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (4);
};

ସଂରଚନା Caret_ମୂଲ୍ୟ_Format3
{
  friend ସଂରଚନା Caret_ମୂଲ୍ୟ;

  hb_position_t get_caret_ମୂଲ୍ୟ (hb_font_t *font, hb_direction_t direction,
				 ଅଚଳ ItemVariationStore &var_store) ଅଚଳ
  {
    ଫେରନ୍ତୁ HB_DIRECTION_IS_HORIZONTAL (direction) ?
	   hb_saturate_add (font->em_scale_x (coordinate), (ଏହି+deviceTable).get_x_delta (font, var_store)) :
	   hb_saturate_add (font->em_scale_y (coordinate), (ଏହି+deviceTable).get_y_delta (font, var_store));
  }

  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    auto *out = c->serializer->start_embed (*ଏହି);
    ଯଦି (!c->serializer->embed (caret_ମୂଲ୍ୟ_Format)) return_trace (ମିଥ୍ୟା);
    ଯଦି (!c->serializer->embed (coordinate)) return_trace (ମିଥ୍ୟା);

    unsigned varidx = (ଏହି+deviceTable).get_variation_index ();
    hb_pair_t<unsigned, ପୂର୍ଣ୍ଣସଂଖ୍ୟା> *new_varidx_delta;
    ଯଦି (c->plan->layout_variation_idx_delta_map.has (varidx, &new_varidx_delta)) {
      uint32_t new_varidx = hb_first (*new_varidx_delta);
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା delta = hb_second (*new_varidx_delta);
      ଯଦି (delta != 0)
      {
        ଯଦି (!c->serializer->check_assign (out->coordinate, coordinate + delta, HB_SERIALIZE_ERROR_INT_OVERFLOW))
          return_trace (ମିଥ୍ୟା);
      }

      ଯଦି (new_varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
        return_trace (c->serializer->check_assign (out->caret_ମୂଲ୍ୟ_Format, 1, HB_SERIALIZE_ERROR_INT_OVERFLOW));
    }

    ଯଦି (!c->serializer->embed (deviceTable))
      return_trace (ମିଥ୍ୟା);

    return_trace (out->deviceTable.serialize_copy (c->serializer, deviceTable, ଏହି, c->serializer->to_bias (out),
						   hb_serialize_context_t::Head, &c->plan->layout_variation_idx_delta_map));
  }

  ଶୂନ୍ୟ collect_variation_indices (hb_collect_variation_indices_context_t *c) ଅଚଳ
  { (ଏହି+deviceTable).collect_variation_indices (c); }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) && deviceTable.sanitize (c, ଏହି));
  }

  ସୁରକ୍ଷିତ:
  HBUINT16	caret_ମୂଲ୍ୟ_Format;	/* Format identifier--format = 3 */
  FWORD		coordinate;		/* X or Y value, in design units */
  Offset16To<Device>
		deviceTable;		/* Offset to Device table for X or Y
					 * value--from beginning of CaretValue
					 * table */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (6);
};

ସଂରଚନା Caret_ମୂଲ୍ୟ
{
  hb_position_t get_caret_ମୂଲ୍ୟ (hb_font_t *font,
				 hb_direction_t direction,
				 hb_codepoint_t glyph_id,
				 ଅଚଳ ItemVariationStore &var_store) ଅଚଳ
  {
    ବାଛ (u.format.v) {
    ସ୍ଥିତି 1: hb_barrier (); ଫେରନ୍ତୁ u.format1.get_caret_ମୂଲ୍ୟ (font, direction);
    ସ୍ଥିତି 2: hb_barrier (); ଫେରନ୍ତୁ u.format2.get_caret_ମୂଲ୍ୟ (font, direction, glyph_id);
    ସ୍ଥିତି 3: hb_barrier (); ଫେରନ୍ତୁ u.format3.get_caret_ମୂଲ୍ୟ (font, direction, var_store);
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:ଫେରନ୍ତୁ 0;
    }
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ଅଚଳ
  {
    ଯଦି (unlikely (!c->may_dispatch (ଏହି, &u.format.v))) ଫେରନ୍ତୁ c->no_dispatch_return_ମୂଲ୍ୟ ();
    TRACE_DISPATCH (ଏହି, u.format.v);
    ବାଛ (u.format.v) {
    ସ୍ଥିତି 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    ସ୍ଥିତି 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    ସ୍ଥିତି 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:return_trace (c->default_return_ମୂଲ୍ୟ ());
    }
  }

  ଶୂନ୍ୟ collect_variation_indices (hb_collect_variation_indices_context_t *c) ଅଚଳ
  {
    ବାଛ (u.format.v) {
    ସ୍ଥିତି 1:
    ସ୍ଥିତି 2:
      ଫେରନ୍ତୁ;
    ସ୍ଥିତି 3:
      hb_barrier ();
      u.format3.collect_variation_indices (c);
      ଫେରନ୍ତୁ;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ: ଫେରନ୍ତୁ;
    }
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    ଯଦି (!u.format.v.sanitize (c)) return_trace (ମିଥ୍ୟା);
    hb_barrier ();
    ବାଛ (u.format.v) {
    ସ୍ଥିତି 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    ସ୍ଥିତି 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    ସ୍ଥିତି 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:return_trace (ସତ୍ୟ);
    }
  }

  ସୁରକ୍ଷିତ:
  ସଂଘ {
  ସଂରଚନା { HBUINT16 v; }	format;		/* Format identifier */
  Caret_ମୂଲ୍ୟ_Format1	format1;
  Caret_ମୂଲ୍ୟ_Format2	format2;
  Caret_ମୂଲ୍ୟ_Format3	format3;
  } u;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_UNION (2, format.v);
};

ସଂରଚନା LigGlyph
{
  unsigned get_lig_carets (hb_font_t            *font,
			   hb_direction_t        direction,
			   hb_codepoint_t        glyph_id,
			   ଅଚଳ ItemVariationStore &var_store,
			   unsigned              start_offset,
			   unsigned             *caret_ଗଣନା /* IN/OUT */,
			   hb_position_t        *caret_array /* OUT */) ଅଚଳ
  {
    ଯଦି (caret_ଗଣନା && caret_array)
    {
      + carets.as_array ().sub_array (start_offset, caret_ଗଣନା)
      | hb_map (hb_add (ଏହି))
      | hb_map ([&] (ଅଚଳ Caret_ମୂଲ୍ୟ &value) { ଫେରନ୍ତୁ value.get_caret_ମୂଲ୍ୟ (font, direction, glyph_id, var_store); })
      | hb_sink (hb_array (caret_array, *caret_ଗଣନା))
      ;
    }

    ଫେରନ୍ତୁ carets.len;
  }

  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    auto *out = c->serializer->start_embed (*ଏହି);
    ଯଦି (unlikely (!c->serializer->extend_min (out))) return_trace (ମିଥ୍ୟା);

    + hb_iter (carets)
    | hb_apply (subset_offset_array (c, out->carets, ଏହି))
    ;

    return_trace (ତାର୍କିକ (out->carets));
  }

  ଶୂନ୍ୟ collect_variation_indices (hb_collect_variation_indices_context_t *c) ଅଚଳ
  {
    ପାଇଁ (ଅଚଳ Offset16To<Caret_ମୂଲ୍ୟ>& offset : carets.iter ())
      (ଏହି+offset).collect_variation_indices (c);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (carets.sanitize (c, ଏହି));
  }

  ସୁରକ୍ଷିତ:
  Array16OfOffset16To<Caret_ମୂଲ୍ୟ>
		carets;			/* Offset array of CaretValue tables
					 * --from beginning of LigGlyph table
					 * --in increasing coordinate order */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (2, carets);
};

/* LigCaretList2 widens the LigCaretList coverage, count, and LigGlyph
 * offsets. The LigGlyph table itself keeps Offset16 caretValueOffsets. */
template <typename Types>
ସଂରଚନା LigCaretList
{
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_lig_carets (hb_font_t *font,
			       hb_direction_t direction,
			       hb_codepoint_t glyph_id,
			       ଅଚଳ ItemVariationStore &var_store,
			       unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start_offset,
			       unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *caret_ଗଣନା /* IN/OUT */,
			       hb_position_t *caret_array /* OUT */) ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = (ଏହି+coverage).get_coverage (glyph_id);
    ଯଦି (index == NOT_COVERED)
    {
      ଯଦି (caret_ଗଣନା)
	*caret_ଗଣନା = 0;
      ଫେରନ୍ତୁ 0;
    }
    ଅଚଳ LigGlyph &lig_glyph = ଏହି+ligGlyph[index];
    ଫେରନ୍ତୁ lig_glyph.get_lig_carets (font, direction, glyph_id, var_store, start_offset, caret_ଗଣନା, caret_array);
  }

  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    ଅଚଳ hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ଅଚଳ hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*ଏହି);
    ଯଦି (unlikely (!c->serializer->extend_min (out))) return_trace (ମିଥ୍ୟା);

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (ଏହି+coverage, ligGlyph)
    | hb_filter (glyphset, hb_first)
    | hb_filter (subset_offset_array (c, out->ligGlyph, ଏହି), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;
    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (ତାର୍କିକ (new_coverage));
  }

  ଶୂନ୍ୟ collect_variation_indices (hb_collect_variation_indices_context_t *c) ଅଚଳ
  {
    + hb_zip (ଏହି+coverage, ligGlyph)
    | hb_filter (c->glyph_set, hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (ଏହି))
    | hb_apply ([c] (ଅଚଳ LigGlyph& _) { _.collect_variation_indices (c); })
    ;
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (coverage.sanitize (c, ଏହି) && ligGlyph.sanitize (c, ଏହି));
  }

  ସୁରକ୍ଷିତ:
  typename Types::template LOffsetTo<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of LigCaretList table */
  typename Types::template ArrayOf<typename Types::template OffsetTo<LigGlyph>>
		ligGlyph;		/* Array of LigGlyph tables
					 * in Coverage Index order */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (Types::LOffset::static_size + Types::HBUINT::static_size, ligGlyph);
};

#ifndef HB_NO_BEYOND_64K
static_assert (LigCaretList<MediumTypes>::min_size ==
	       Offset32::static_size + HBUINT24::static_size, "");
#endif


ସଂରଚନା MarkGlyphSetsFormat1
{
  ତାର୍କିକ covers (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା set_index, hb_codepoint_t glyph_id) ଅଚଳ
  { ଫେରନ୍ତୁ (ଏହି+coverage[set_index]).get_coverage (glyph_id) != NOT_COVERED; }

  ଶୂନ୍ୟ collect_used_mark_sets (ଅଚଳ hb_set_t& glyph_set,
                               hb_set_t& used_mark_sets /* OUT */) ଅଚଳ
  {
    unsigned i = 0;
    ପାଇଁ (ଅଚଳ auto &offset : coverage)
     {
       ଅଚଳ auto &cov = ଏହି+offset;
       ଯଦି (cov.intersects (&glyph_set))
         used_mark_sets.add (i);

       i++;
     }
  }

  template <typename set_t>
  ଶୂନ୍ୟ collect_coverage (hb_vector_t<set_t> &sets) ଅଚଳ
  {
     ପାଇଁ (ଅଚଳ auto &offset : coverage)
     {
       ଅଚଳ auto &cov = ଏହି+offset;
       cov.collect_coverage (sets.push ());
     }
  }

  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    auto *out = c->serializer->start_embed (*ଏହି);
    ଯଦି (unlikely (!c->serializer->extend_min (out))) return_trace (ମିଥ୍ୟା);
    out->format = format;

    ତାର୍କିକ ret = ସତ୍ୟ;
    ପାଇଁ (ଅଚଳ Offset32To<Coverage>& offset : coverage.iter ())
    {
      auto snap = c->serializer->snapshot ();
      auto *o = out->coverage.serialize_append (c->serializer);
      ଯଦି (unlikely (!o))
      {
	ret = ମିଥ୍ୟା;
	ଭାଙ୍ଗ;
      }

      //skip empty coverage
      c->serializer->push ();
      ତାର୍କିକ res = ମିଥ୍ୟା;
      ଯଦି (offset) res = c->dispatch (ଏହି+offset);
      ଯଦି (!res)
      {
        c->serializer->କାଢ଼_discard ();
        c->serializer->revert (snap);
        (out->coverage.len)--;
        ଜାରି;
      }
      c->serializer->add_link (*o, c->serializer->କାଢ଼_pack ());
    }

    return_trace (ret && out->coverage.len);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (coverage.sanitize (c, ଏହି));
  }

  ସୁରକ୍ଷିତ:
  HBUINT16	format;			/* Format identifier--format = 1 */
  Array16Of<Offset32To<Coverage>>
		coverage;		/* Array of long offsets to mark set
					 * coverage tables */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (4, coverage);
};

ସଂରଚନା MarkGlyphSets
{
  ତାର୍କିକ covers (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା set_index, hb_codepoint_t glyph_id) ଅଚଳ
  {
    ବାଛ (u.format.v) {
    ସ୍ଥିତି 1: hb_barrier (); ଫେରନ୍ତୁ u.format1.covers (set_index, glyph_id);
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
  }

  template <typename set_t>
  ଶୂନ୍ୟ collect_coverage (hb_vector_t<set_t> &sets) ଅଚଳ
  {
    ବାଛ (u.format.v) {
    ସ୍ଥିତି 1: hb_barrier (); u.format1.collect_coverage (sets); ଫେରନ୍ତୁ;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:ଫେରନ୍ତୁ;
    }
  }

  ଶୂନ୍ୟ collect_used_mark_sets (ଅଚଳ hb_set_t& glyph_set,
                               hb_set_t& used_mark_sets /* OUT */) ଅଚଳ
  {
    ବାଛ (u.format.v) {
    ସ୍ଥିତି 1: hb_barrier (); u.format1.collect_used_mark_sets (glyph_set, used_mark_sets); ଫେରନ୍ତୁ;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:ଫେରନ୍ତୁ;
    }
  }

  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    ବାଛ (u.format.v) {
    ସ୍ଥିତି 1: hb_barrier (); return_trace (u.format1.subset (c));
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:return_trace (ମିଥ୍ୟା);
    }
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    ଯଦି (!u.format.v.sanitize (c)) return_trace (ମିଥ୍ୟା);
    hb_barrier ();
    ବାଛ (u.format.v) {
    ସ୍ଥିତି 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:return_trace (ସତ୍ୟ);
    }
  }

  ସୁରକ୍ଷିତ:
  ସଂଘ {
  ସଂରଚନା { HBUINT16 v; }	format;		/* Format identifier */
  MarkGlyphSetsFormat1	format1;
  } u;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_UNION (2, format.v);
};


/*
 * GDEF -- Glyph Definition
 * https://docs.microsoft.com/en-us/typography/opentype/spec/gdef
 */


ସଂରଚନା GDEF
{
  ସ୍ଥିର constexpr hb_tag_t tableTag = HB_OT_TAG_GDEF;

  ଗଣନା GlyphClasses {
    UnclassifiedGlyph	= 0,
    BaseGlyph		= 1,
    LigatureGlyph	= 2,
    MarkGlyph		= 3,
    ComponentGlyph	= 4
  };

  size_t get_size () ଅଚଳ
  {
    ଯଦି (version.major != 1) ଫେରନ୍ତୁ version.static_size;

    ଫେରନ୍ତୁ min_size +
	   (version.to_int () >= 0x00010002u ? markGlyphSetsDef.static_size : 0) +
	   (version.to_int () >= 0x00010003u ? varStore.static_size : 0) +
#ifndef HB_NO_BEYOND_64K
	   (version.to_int () >= 0x00010004u ? 5 * Offset32::static_size : 0)
#ଅନ୍ୟଥା
	   0
#endif
	   ;
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    ଯଦି (unlikely (!version.sanitize (c) ||
		  version.major != 1 ||
		  !c->check_range (ଏହି, get_size ())))
      return_trace (ମିଥ୍ୟା);
    hb_barrier ();

    ତାର୍କିକ glyph_class_def_ok = glyphClassDef.sanitize (c, ଏହି);
    ତାର୍କିକ attach_list_ok = attachList.sanitize (c, ଏହି);
    ତାର୍କିକ lig_caret_list_ok = ligCaretList.sanitize (c, ଏହି);
    ତାର୍କିକ mark_attach_class_def_ok = markAttachClassDef.sanitize (c, ଏହି);
    ତାର୍କିକ mark_glyph_sets_def_ok = version.to_int () < 0x00010002u ||
				  markGlyphSetsDef.sanitize (c, ଏହି);

#ifndef HB_NO_BEYOND_64K
    ଯଦି (version.to_int () >= 0x00010004u)
    {
      ଯଦି (glyphClassDef2) glyph_class_def_ok = glyphClassDef2.sanitize (c, ଏହି);
      ଯଦି (attachList2) attach_list_ok = attachList2.sanitize (c, ଏହି);
      ଯଦି (ligCaretList2) lig_caret_list_ok = ligCaretList2.sanitize (c, ଏହି);
      ଯଦି (markAttachClassDef2) mark_attach_class_def_ok = markAttachClassDef2.sanitize (c, ଏହି);
      ଯଦି (markGlyphSetsDef2) mark_glyph_sets_def_ok = markGlyphSetsDef2.sanitize (c, ଏହି);
    }
#endif

    return_trace (glyph_class_def_ok &&
		  attach_list_ok &&
		  lig_caret_list_ok &&
		  mark_attach_class_def_ok &&
		  mark_glyph_sets_def_ok &&
		  (version.to_int () < 0x00010003u || varStore.sanitize (c, ଏହି)));
  }

  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    ଯଦି (version.major != 1) return_trace (ମିଥ୍ୟା);

    auto *out = c->serializer->start_embed (*ଏହି);
    ଯଦି (unlikely (!c->serializer->extend_min (out))) return_trace (ମିଥ୍ୟା);

    // Push var store first (if it's needed) so that it's last in the
    // serialization order. Some font consumers assume that varstore runs to
    // the end of the GDEF table.
    // See: https://github.com/harfbuzz/harfbuzz/issues/4636
    auto snapshot_version0 = c->serializer->snapshot ();
    ଯଦି (unlikely (version.to_int () >= 0x00010002u && hb_barrier () &&
		  !c->serializer->embed (markGlyphSetsDef)))
      return_trace (ମିଥ୍ୟା);

    auto snapshot_version2 = c->serializer->snapshot ();
    ଯଦି (unlikely (version.to_int () >= 0x00010003u && hb_barrier () &&
		  !c->serializer->embed (varStore)))
      return_trace (ମିଥ୍ୟା);

    ତାର୍କିକ use_v14 = ମିଥ୍ୟା;
#ifndef HB_NO_BEYOND_64K
    use_v14 = version.to_int () >= 0x00010004u &&
	      (glyphClassDef2 || attachList2 || ligCaretList2 ||
	       markAttachClassDef2 || markGlyphSetsDef2);
    ଯଦି (unlikely (version.to_int () >= 0x00010004u && hb_barrier () &&
		  !c->serializer->extend_size (out, get_size ())))
      return_trace (ମିଥ୍ୟା);
#endif

    ତାର୍କିକ subset_varstore = ମିଥ୍ୟା;
    unsigned varstore_index = (unsigned) -1;
    ଯଦି (version.to_int () >= 0x00010003u && hb_barrier ())
    {
      ଯଦି (c->plan->all_axes_pinned)
	out->varStore = 0;
      ଅନ୍ୟଥା ଯଦି (c->plan->normalized_coords)
      {
	ଯଦି (varStore)
	{
	  item_variations_t item_vars;
	  ଯଦି (unlikely (!c->serializer->check_success (
                  item_vars.instantiate (ଏହି+varStore, c->plan, ସତ୍ୟ, ସତ୍ୟ,
                                          c->plan->gdef_varstore_inner_maps.as_array (), ସତ୍ୟ))))
            return_trace (ମିଥ୍ୟା);
	  subset_varstore = out->varStore.serialize_serialize (c->serializer,
							     item_vars.has_long_word (),
							     c->plan->axis_tags,
							     item_vars.get_region_list (),
							     item_vars.get_vardata_encodings ());
	  varstore_index = c->serializer->last_added_child_index();
	  remap_varidx_after_instantiation (item_vars.get_varidx_map (),
					    c->plan->layout_variation_idx_delta_map);
          remap_varidx_after_instantiation (item_vars.get_varidx_map (),
                                            c->plan->layout_condition_idx_ମୂଲ୍ୟ_map);
	}
      }
      ଅନ୍ୟଥା
      {
	subset_varstore = out->varStore.serialize_subset (c, varStore, ଏହି,
							  c->plan->gdef_varstore_inner_maps.as_array ());
	varstore_index = c->serializer->last_added_child_index();
      }
    }

    out->version.major = version.major;
    out->version.minor = version.minor;

    ଯଦି (!subset_varstore && version.to_int () >= 0x00010002u && !use_v14) {
      c->serializer->revert (snapshot_version2);
    }
    ଯଦି (!subset_varstore)
      out->varStore = 0;
    ଅନ୍ୟଥା
      c->plan->has_gdef_varstore = ସତ୍ୟ;

    ତାର୍କିକ subset_markglyphsetsdef = ମିଥ୍ୟା;
    ଯଦି (version.to_int () >= 0x00010002u && hb_barrier ())
    {
#ifndef HB_NO_BEYOND_64K
      ଯଦି (use_v14 && markGlyphSetsDef2)
      {
	out->markGlyphSetsDef = 0;
	subset_markglyphsetsdef = serialize_subset_offset (c, out->markGlyphSetsDef2,
							   markGlyphSetsDef2, ଏହି);
      }
      ଅନ୍ୟଥା
#endif
      {
#ifndef HB_NO_BEYOND_64K
	ଯଦି (use_v14) out->markGlyphSetsDef2 = 0;
#endif
	subset_markglyphsetsdef = out->markGlyphSetsDef.serialize_subset (c, markGlyphSetsDef, ଏହି);
      }
    }

    ଯଦି (use_v14)
    {
      out->version.minor = 4;
    } ଅନ୍ୟଥା ଯଦି (subset_varstore) {
      out->version.minor = 3;
    } ଅନ୍ୟଥା ଯଦି (subset_markglyphsetsdef) {
      out->version.minor = 2;
    } ଅନ୍ୟଥା  {
      out->version.minor = 0;
      c->serializer->revert (snapshot_version0);
    }

    ତାର୍କିକ subset_glyphclassdef = ମିଥ୍ୟା;
    ତାର୍କିକ subset_attachlist = ମିଥ୍ୟା;
    ତାର୍କିକ subset_markattachclassdef = ମିଥ୍ୟା;
    ତାର୍କିକ subset_ligcaretlist = ମିଥ୍ୟା;

#ifndef HB_NO_BEYOND_64K
    ଯଦି (use_v14 && glyphClassDef2)
    {
      out->glyphClassDef = 0;
      subset_glyphclassdef = serialize_subset_offset (c, out->glyphClassDef2,
						      glyphClassDef2, ଏହି, nullptr, ମିଥ୍ୟା, ସତ୍ୟ);
    }
    ଅନ୍ୟଥା
#endif
    {
#ifndef HB_NO_BEYOND_64K
      ଯଦି (use_v14) out->glyphClassDef2 = 0;
#endif
      subset_glyphclassdef = out->glyphClassDef.serialize_subset (c, glyphClassDef, ଏହି, nullptr, ମିଥ୍ୟା, ସତ୍ୟ);
    }

#ifndef HB_NO_BEYOND_64K
    ଯଦି (use_v14 && attachList2)
    {
      out->attachList = 0;
      subset_attachlist = serialize_subset_offset (c, out->attachList2, attachList2, ଏହି);
    }
    ଅନ୍ୟଥା
#endif
    {
#ifndef HB_NO_BEYOND_64K
      ଯଦି (use_v14) out->attachList2 = 0;
#endif
      subset_attachlist = out->attachList.serialize_subset (c, attachList, ଏହି);
    }

#ifndef HB_NO_BEYOND_64K
    ଯଦି (use_v14 && markAttachClassDef2)
    {
      out->markAttachClassDef = 0;
      subset_markattachclassdef = serialize_subset_offset (c, out->markAttachClassDef2,
							   markAttachClassDef2, ଏହି, nullptr, ମିଥ୍ୟା, ସତ୍ୟ);
    }
    ଅନ୍ୟଥା
#endif
    {
#ifndef HB_NO_BEYOND_64K
      ଯଦି (use_v14) out->markAttachClassDef2 = 0;
#endif
      subset_markattachclassdef = out->markAttachClassDef.serialize_subset (c, markAttachClassDef,
									   ଏହି, nullptr, ମିଥ୍ୟା, ସତ୍ୟ);
    }

#ifndef HB_NO_BEYOND_64K
    ଯଦି (use_v14 && ligCaretList2)
    {
      out->ligCaretList = 0;
      subset_ligcaretlist = serialize_subset_offset (c, out->ligCaretList2, ligCaretList2, ଏହି);
    }
    ଅନ୍ୟଥା
#endif
    {
#ifndef HB_NO_BEYOND_64K
      ଯଦି (use_v14) out->ligCaretList2 = 0;
#endif
      subset_ligcaretlist = out->ligCaretList.serialize_subset (c, ligCaretList, ଏହି);
    }

    ଯଦି (subset_varstore && varstore_index != (unsigned) -1) {
      c->serializer->repack_last(varstore_index);
    }

    return_trace (subset_glyphclassdef || subset_attachlist ||
		  subset_ligcaretlist || subset_markattachclassdef ||
		  (out->version.to_int () >= 0x00010002u && subset_markglyphsetsdef) ||
		  (out->version.to_int () >= 0x00010003u && subset_varstore));
  }

  ତାର୍କିକ has_glyph_classes () ଅଚଳ
  {
#ifndef HB_NO_BEYOND_64K
    ଯଦି (version.to_int () >= 0x00010004u && glyphClassDef2) ଫେରନ୍ତୁ ସତ୍ୟ;
#endif
    ଫେରନ୍ତୁ glyphClassDef != 0;
  }
  ଅଚଳ ClassDef &get_glyph_class_def () ଅଚଳ
  {
#ifndef HB_NO_BEYOND_64K
    ଯଦି (version.to_int () >= 0x00010004u && glyphClassDef2) ଫେରନ୍ତୁ ଏହି+glyphClassDef2;
#endif
    ଫେରନ୍ତୁ ଏହି+glyphClassDef;
  }
  ତାର୍କିକ has_attach_list () ଅଚଳ
  {
#ifndef HB_NO_BEYOND_64K
    ଯଦି (version.to_int () >= 0x00010004u && attachList2) ଫେରନ୍ତୁ ସତ୍ୟ;
#endif
    ଫେରନ୍ତୁ attachList != 0;
  }
  ଅଚଳ AttachList &get_attach_list () ଅଚଳ
  {
#ifndef HB_NO_BEYOND_64K
    ଯଦି (version.to_int () >= 0x00010004u && attachList2) ଫେରନ୍ତୁ ଏହି+attachList2;
#endif
    ଫେରନ୍ତୁ ଏହି+attachList;
  }
  ତାର୍କିକ has_lig_carets () ଅଚଳ
  {
#ifndef HB_NO_BEYOND_64K
    ଯଦି (version.to_int () >= 0x00010004u && ligCaretList2) ଫେରନ୍ତୁ ସତ୍ୟ;
#endif
    ଫେରନ୍ତୁ ligCaretList != 0;
  }
  ତାର୍କିକ has_mark_attachment_types () ଅଚଳ
  {
#ifndef HB_NO_BEYOND_64K
    ଯଦି (version.to_int () >= 0x00010004u && markAttachClassDef2) ଫେରନ୍ତୁ ସତ୍ୟ;
#endif
    ଫେରନ୍ତୁ markAttachClassDef != 0;
  }
  ଅଚଳ ClassDef &get_mark_attach_class_def () ଅଚଳ
  {
#ifndef HB_NO_BEYOND_64K
    ଯଦି (version.to_int () >= 0x00010004u && markAttachClassDef2) ଫେରନ୍ତୁ ଏହି+markAttachClassDef2;
#endif
    ଫେରନ୍ତୁ ଏହି+markAttachClassDef;
  }
  ତାର୍କିକ has_mark_glyph_sets () ଅଚଳ
  {
    ଯଦି (version.to_int () < 0x00010002u) ଫେରନ୍ତୁ ମିଥ୍ୟା;
#ifndef HB_NO_BEYOND_64K
    ଯଦି (version.to_int () >= 0x00010004u && markGlyphSetsDef2) ଫେରନ୍ତୁ ସତ୍ୟ;
#endif
    ଫେରନ୍ତୁ markGlyphSetsDef != 0;
  }
  ଅଚଳ MarkGlyphSets &get_mark_glyph_sets () ଅଚଳ
  {
    ଯଦି (version.to_int () < 0x00010002u) ଫେରନ୍ତୁ Null(MarkGlyphSets);
#ifndef HB_NO_BEYOND_64K
    ଯଦି (version.to_int () >= 0x00010004u && markGlyphSetsDef2) ଫେରନ୍ତୁ ଏହି+markGlyphSetsDef2;
#endif
    ଫେରନ୍ତୁ ଏହି+markGlyphSetsDef;
  }
  ତାର୍କିକ has_var_store () ଅଚଳ
  {
    ଫେରନ୍ତୁ version.to_int () >= 0x00010003u && hb_barrier () && varStore != 0;
  }
  ଅଚଳ ItemVariationStore &get_var_store () ଅଚଳ
  {
    ଫେରନ୍ତୁ version.to_int () >= 0x00010003u && hb_barrier () ? ଏହି+varStore : Null(ItemVariationStore);
  }


  ତାର୍କିକ has_data () ଅଚଳ { ଫେରନ୍ତୁ version.to_int (); }
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_glyph_class (hb_codepoint_t glyph) ଅଚଳ
  { ଫେରନ୍ତୁ get_glyph_class_def ().get_class (glyph); }
  ଶୂନ୍ୟ get_glyphs_in_class (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା klass, hb_set_t *glyphs) ଅଚଳ
  { get_glyph_class_def ().collect_class (glyphs, klass); }

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_mark_attachment_type (hb_codepoint_t glyph) ଅଚଳ
  { ଫେରନ୍ତୁ get_mark_attach_class_def ().get_class (glyph); }

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_attach_points (hb_codepoint_t glyph_id,
				  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start_offset,
				  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *point_ଗଣନା /* IN/OUT */,
				  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *point_array /* OUT */) ଅଚଳ
  { ଫେରନ୍ତୁ get_attach_list ().get_attach_points (glyph_id, start_offset, point_ଗଣନା, point_array); }

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_lig_carets (hb_font_t *font,
			       hb_direction_t direction,
			       hb_codepoint_t glyph_id,
			       unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start_offset,
			       unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *caret_ଗଣନା /* IN/OUT */,
			       hb_position_t *caret_array /* OUT */) ଅଚଳ
  {
#ifndef HB_NO_BEYOND_64K
    ଯଦି (version.to_int () >= 0x00010004u && ligCaretList2)
      ଫେରନ୍ତୁ (ଏହି+ligCaretList2).get_lig_carets (font, direction, glyph_id, get_var_store (),
						  start_offset, caret_ଗଣନା, caret_array);
#endif
    ଫେରନ୍ତୁ (ଏହି+ligCaretList).get_lig_carets (font, direction, glyph_id, get_var_store (),
					       start_offset, caret_ଗଣନା, caret_array);
  }

  ତାର୍କିକ mark_set_covers (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା set_index, hb_codepoint_t glyph_id) ଅଚଳ
  { ଫେରନ୍ତୁ get_mark_glyph_sets ().covers (set_index, glyph_id); }

  /* glyph_props is a 16-bit integer where the lower 8-bit have bits representing
   * glyph class and other bits, and high 8-bit the mark attachment type (if any).
   * Not to be confused with lookup_props which is very similar. */
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_glyph_props (hb_codepoint_t glyph) ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା klass = get_glyph_class (glyph);

    static_assert (((unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) HB_OT_LAYOUT_GLYPH_PROPS_BASE_GLYPH == (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) LookupFlag::IgnoreBaseGlyphs), "");
    static_assert (((unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) HB_OT_LAYOUT_GLYPH_PROPS_LIGATURE == (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) LookupFlag::IgnoreLigatures), "");
    static_assert (((unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) HB_OT_LAYOUT_GLYPH_PROPS_MARK == (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) LookupFlag::IgnoreMarks), "");

    ବାଛ (klass) {
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:			ଫେରନ୍ତୁ HB_OT_LAYOUT_GLYPH_CLASS_UNCLASSIFIED;
    ସ୍ଥିତି BaseGlyph:		ଫେରନ୍ତୁ HB_OT_LAYOUT_GLYPH_PROPS_BASE_GLYPH;
    ସ୍ଥିତି LigatureGlyph:		ଫେରନ୍ତୁ HB_OT_LAYOUT_GLYPH_PROPS_LIGATURE;
    ସ୍ଥିତି MarkGlyph:
	  klass = get_mark_attachment_type (glyph);
	  ଫେରନ୍ତୁ HB_OT_LAYOUT_GLYPH_PROPS_MARK | (klass << 8);
    }
  }

  HB_INTERNAL ତାର୍କିକ is_blocklisted (hb_blob_t *blob,
				   hb_face_t *face) ଅଚଳ;

  ସଂରଚନା accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      table = hb_sanitize_context_t ().reference_table<GDEF> (face);
      ଯଦି (unlikely (table->is_blocklisted (table.get_blob (), face)))
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
	ଯଦି (!sets.in_error () &&
	    mark_glyph_set_bitmaps.resize (sets.length))
	  ପାଇଁ (unsigned i = 0; i < sets.length; i++)
	  {
	    auto &bitmap = mark_glyph_set_bitmaps.arrayZ[i];
	    bitmap.bias = HB_SET_ମୂଲ୍ୟ_INVALID;
	    ଅଚଳ auto &set = sets.arrayZ[i];
	    ଯଦି (unlikely (set.in_error ())) ଜାରି;
	    hb_codepoint_t min_gid = HB_SET_ମୂଲ୍ୟ_INVALID;
	    hb_codepoint_t max_gid = HB_SET_ମୂଲ୍ୟ_INVALID;
	    hb_set_next (&set, &min_gid);
	    hb_set_previous (&set, &max_gid);
	    ଯଦି (max_gid == HB_SET_ମୂଲ୍ୟ_INVALID)
	      ଜାରି;
	    ଯଦି (max_gid - min_gid >= hb_bit_page_t::PAGE_BITS)
	    {
	      /* Too wide for one page; a digest pre-filter instead. */
	      ପାଇଁ (hb_codepoint_t gid : set)
		bitmap.u.digest.add (gid);
	      ଜାରି;
	    }
	    bitmap.bias = min_gid;
	    ପାଇଁ (hb_codepoint_t gid : set)
	      bitmap.u.page.add (gid - min_gid);
	  }
      }
#endif
    }
    ~accelerator_t () { table.destroy (); }

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_glyph_props (hb_codepoint_t glyph) ଅଚଳ
    {
      unsigned v;

#ifndef HB_NO_GDEF_CACHE
      ଯଦି (glyph_props_cache.get (glyph, &v))
        ଫେରନ୍ତୁ v;
#endif

      v = table->get_glyph_props (glyph);

#ifndef HB_NO_GDEF_CACHE
      ଯଦି (likely (table.get_blob ())) // Don't try setting if we are the null instance!
	glyph_props_cache.set (glyph, v);
#endif

      ଫେରନ୍ତୁ v;

    }

    HB_ALWAYS_INLINE
    ତାର୍କିକ mark_set_covers (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା set_index, hb_codepoint_t glyph_id) ଅଚଳ
    {
#ifndef HB_NO_GDEF_CACHE
      // We can access arrayZ directly because of sanitize_lookup_props() guarantee.
      ଅଚଳ auto &bitmap = mark_glyph_set_bitmaps.arrayZ[set_index];
      ଯଦି (likely (bitmap.bias != HB_SET_ମୂଲ୍ୟ_INVALID))
      {
	hb_codepoint_t biased = glyph_id - bitmap.bias;
	ଫେରନ୍ତୁ biased < hb_bit_page_t::PAGE_BITS &&
	       bitmap.u.page.get (biased);
      }
      ଫେରନ୍ତୁ mark_set_covers_slow (set_index, glyph_id);
#ଅନ୍ୟଥା
      ଫେରନ୍ତୁ table->mark_set_covers (set_index, glyph_id);
#endif
    }

#ifndef HB_NO_GDEF_CACHE
    HB_NEVER_INLINE
    ତାର୍କିକ mark_set_covers_slow (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା set_index, hb_codepoint_t glyph_id) ଅଚଳ
    {
      ଅଚଳ auto &bitmap = mark_glyph_set_bitmaps.arrayZ[set_index];
      ଯଦି (!bitmap.u.digest.may_have (glyph_id))
	ଫେରନ୍ତୁ ମିଥ୍ୟା;
      ଫେରନ୍ତୁ table->mark_set_covers (set_index, glyph_id);
    }
#endif

    unsigned sanitize_lookup_props (unsigned lookup_props) ଅଚଳ
    {
#ifndef HB_NO_GDEF_CACHE
      ଯଦି (lookup_props & LookupFlag::UseMarkFilteringSet &&
	  (lookup_props >> 16) >= mark_glyph_set_bitmaps.length)
      {
        // Invalid mark filtering set index; unset the flag.
	lookup_props &= ~LookupFlag::UseMarkFilteringSet;
      }
#endif
      ଫେରନ୍ତୁ lookup_props;
    }

    hb_blob_ptr_t<GDEF> table;
#ifndef HB_NO_GDEF_CACHE
    ସଂରଚନା mark_glyph_set_bitmap_t
    {
      /* Gid of the first bit (not page-aligned) for an exact one-page
       * set, or HB_SET_VALUE_INVALID; then digest is a pre-filter for
       * the table.  The all-zeros state is valid for both members. */
      hb_codepoint_t bias = HB_SET_ମୂଲ୍ୟ_INVALID;
      ସଂଘ u_t {
	u_t () : page () {}
	hb_bit_page_t page;
	hb_set_digest_t digest;
      } u;
    };

    hb_vector_t<mark_glyph_set_bitmap_t> mark_glyph_set_bitmaps;
    mutable hb_cache_t<21, 3> glyph_props_cache;
    static_assert (ଆକାର (glyph_props_cache) == 512, "");
#endif
  };

  ଶୂନ୍ୟ collect_variation_indices (hb_collect_variation_indices_context_t *c) ଅଚଳ
  {
#ifndef HB_NO_BEYOND_64K
    ଯଦି (version.to_int () >= 0x00010004u && ligCaretList2)
    {
      (ଏହି+ligCaretList2).collect_variation_indices (c);
      ଫେରନ୍ତୁ;
    }
#endif
    (ଏହି+ligCaretList).collect_variation_indices (c);
  }

  ସୁରକ୍ଷିତ:
  template <typename ଚାବି>
  ସ୍ଥିର ଶୂନ୍ୟ remap_varidx_after_instantiation (ଅଚଳ hb_map_t& varidx_map,
                                                hb_hashmap_t<ଚାବି, hb_pair_t<unsigned, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>>& layout_variation_idx_delta_map /* IN/OUT */)
  {
    /* varidx_map is empty which means varstore is empty after instantiation,
     * no variations, map all varidx to HB_OT_LAYOUT_NO_VARIATIONS_INDEX.
     * varidx_map doesn't have original varidx, indicating delta row is all
     * zeros, map varidx to HB_OT_LAYOUT_NO_VARIATIONS_INDEX */
    ପାଇଁ (auto _ : layout_variation_idx_delta_map.iter_ref ())
    {
      /* old_varidx->(varidx, delta) mapping generated for subsetting, then this
       * varidx is used as key of varidx_map during instantiation */
      uint32_t varidx = _.second.first;
      uint32_t *new_varidx;
      ଯଦି (varidx_map.has (varidx, &new_varidx))
	_.second.first = *new_varidx;
      ଅନ୍ୟଥା
	_.second.first = HB_OT_LAYOUT_NO_VARIATIONS_INDEX;
    }
  }

  template <typename OffsetOut, typename OffsetIn, typename Base, typename ...Ts>
  ସ୍ଥିର ତାର୍କିକ serialize_subset_offset (hb_subset_context_t *c,
				       OffsetOut& out,
				       ଅଚଳ OffsetIn& in,
				       ଅଚଳ Base *base,
				       Ts&&... ds)
  {
    out = 0;
    ଯଦି (in.is_null ()) ଫେରନ୍ତୁ ମିଥ୍ୟା;

    auto *s = c->serializer;
    s->push ();
    ତାର୍କିକ ret = c->dispatch (base+in, std::forward<Ts> (ds)...);
    ଯଦି (ret) s->add_link (out, s->କାଢ଼_pack ());
    ଅନ୍ୟଥା s->କାଢ଼_discard ();
    ଫେରନ୍ତୁ ret;
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
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_MIN (12);
};

ସଂରଚନା GDEF_accelerator_t : GDEF::accelerator_t {
  GDEF_accelerator_t (hb_face_t *face) : GDEF::accelerator_t (face) {}
};

} /* namespace OT */


#endif /* OT_LAYOUT_GDEF_GDEF_HH */
