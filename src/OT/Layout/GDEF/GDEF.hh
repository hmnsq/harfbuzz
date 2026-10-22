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

#include "../../../hb-ot-var-common.hh"

#include "../../../hb-font.hh"
#include "../../../hb-cache.hh"


namespace OT {


/*
 * Attachment List Table
 */

/* Array of contour point indices--in increasing numerical order */
struct AttachPoint : Array16Of<HBUINT16>
{
  bool subset (hb_subset_context_t *c) const
  {
    TRACE_SUBSET (this);
    auto *out = c->serializer->start_embed (*this);
    return_trace (out->serialize (c->serializer, + iter ()));
  }
};

/* AttachList is referenced by both GDEF attachListOffset and
 * attachListOffset2. The v1.4 top-level offset widens, but the
 * AttachList table itself keeps Offset16 internals. */
struct AttachList
{
  unsigned 整数 get_attach_points (hb_codepoint_t glyph_id,
				  unsigned 整数 start_offset,
				  unsigned 整数 *point_count /* IN/OUT */,
				  unsigned 整数 *point_array /* OUT */) const
  {
    unsigned 整数 index = (this+coverage).get_coverage (glyph_id);
    如果 (index == NOT_COVERED)
    {
      如果 (point_count)
	*point_count = 0;
      返回 0;
    }

    const AttachPoint &points = this+attachPoint[index];

    如果 (point_count && point_array)
    {
      + points.as_array ().sub_array (start_offset, point_count)
      | hb_sink (hb_array (point_array, *point_count))
      ;
    }

    返回 points.len;
  }

  bool subset (hb_subset_context_t *c) const
  {
    TRACE_SUBSET (this);
    const hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    const hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*this);
    如果 (unlikely (!c->serializer->extend_min (out))) return_trace (false);

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (this+coverage, attachPoint)
    | hb_filter (glyphset, hb_first)
    | hb_filter (subset_offset_array (c, out->attachPoint, this), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;
    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (bool (new_coverage));
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (coverage.sanitize (c, this) && attachPoint.sanitize (c, this));
  }

  protected:
  Offset16To<Coverage>
		coverage;		/* Offset to Coverage table -- from
					 * beginning of AttachList table */
  Array16OfOffset16To<AttachPoint>
		attachPoint;		/* Array of AttachPoint tables
					 * in Coverage Index order */
  public:
  DEFINE_SIZE_ARRAY (4, attachPoint);
};

/*
 * Ligature Caret Table
 */

struct CaretValueFormat1
{
  friend struct CaretValue;
  bool subset (hb_subset_context_t *c) const
  {
    TRACE_SUBSET (this);
    auto *out = c->serializer->embed (this);
    如果 (unlikely (!out)) return_trace (false);
    return_trace (true);
  }

  private:
  hb_position_t get_caret_value (hb_font_t *font, hb_direction_t direction) const
  {
    返回 HB_DIRECTION_IS_HORIZONTAL (direction) ? font->em_scale_x (coordinate) : font->em_scale_y (coordinate);
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this));
  }

  protected:
  HBUINT16	caretValueFormat;	/* Format identifier--format = 1 */
  FWORD		coordinate;		/* X or Y value, in design units */
  public:
  DEFINE_SIZE_STATIC (4);
};

struct CaretValueFormat2
{
  friend struct CaretValue;
  bool subset (hb_subset_context_t *c) const
  {
    TRACE_SUBSET (this);
    auto *out = c->serializer->embed (this);
    如果 (unlikely (!out)) return_trace (false);
    return_trace (true);
  }

  private:
  hb_position_t get_caret_value (hb_font_t *font, hb_direction_t direction, hb_codepoint_t glyph_id) const
  {
    hb_position_t x, y;
    font->get_glyph_contour_point_for_origin (glyph_id, caretValuePoint, direction, &x, &y);
    返回 HB_DIRECTION_IS_HORIZONTAL (direction) ? x : y;
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this));
  }

  protected:
  HBUINT16	caretValueFormat;	/* Format identifier--format = 2 */
  HBUINT16	caretValuePoint;	/* Contour point index on glyph */
  public:
  DEFINE_SIZE_STATIC (4);
};

struct CaretValueFormat3
{
  friend struct CaretValue;

  hb_position_t get_caret_value (hb_font_t *font, hb_direction_t direction,
				 const ItemVariationStore &var_store) const
  {
    返回 HB_DIRECTION_IS_HORIZONTAL (direction) ?
	   hb_saturate_add (font->em_scale_x (coordinate), (this+deviceTable).get_x_delta (font, var_store)) :
	   hb_saturate_add (font->em_scale_y (coordinate), (this+deviceTable).get_y_delta (font, var_store));
  }

  bool subset (hb_subset_context_t *c) const
  {
    TRACE_SUBSET (this);
    auto *out = c->serializer->start_embed (*this);
    如果 (!c->serializer->embed (caretValueFormat)) return_trace (false);
    如果 (!c->serializer->embed (coordinate)) return_trace (false);

    unsigned varidx = (this+deviceTable).get_variation_index ();
    hb_pair_t<unsigned, 整数> *new_varidx_delta;
    如果 (c->plan->layout_variation_idx_delta_map.has (varidx, &new_varidx_delta)) {
      uint32_t new_varidx = hb_first (*new_varidx_delta);
      整数 delta = hb_second (*new_varidx_delta);
      如果 (delta != 0)
      {
        如果 (!c->serializer->check_assign (out->coordinate, coordinate + delta, HB_SERIALIZE_ERROR_INT_OVERFLOW))
          return_trace (false);
      }

      如果 (new_varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
        return_trace (c->serializer->check_assign (out->caretValueFormat, 1, HB_SERIALIZE_ERROR_INT_OVERFLOW));
    }

    如果 (!c->serializer->embed (deviceTable))
      return_trace (false);

    return_trace (out->deviceTable.serialize_copy (c->serializer, deviceTable, this, c->serializer->to_bias (out),
						   hb_serialize_context_t::Head, &c->plan->layout_variation_idx_delta_map));
  }

  void collect_variation_indices (hb_collect_variation_indices_context_t *c) const
  { (this+deviceTable).collect_variation_indices (c); }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this) && deviceTable.sanitize (c, this));
  }

  protected:
  HBUINT16	caretValueFormat;	/* Format identifier--format = 3 */
  FWORD		coordinate;		/* X or Y value, in design units */
  Offset16To<Device>
		deviceTable;		/* Offset to Device table for X or Y
					 * value--from beginning of CaretValue
					 * table */
  public:
  DEFINE_SIZE_STATIC (6);
};

struct CaretValue
{
  hb_position_t get_caret_value (hb_font_t *font,
				 hb_direction_t direction,
				 hb_codepoint_t glyph_id,
				 const ItemVariationStore &var_store) const
  {
    switch (u.format.v) {
    case 1: hb_barrier (); 返回 u.format1.get_caret_value (font, direction);
    case 2: hb_barrier (); 返回 u.format2.get_caret_value (font, direction, glyph_id);
    case 3: hb_barrier (); 返回 u.format3.get_caret_value (font, direction, var_store);
    default:返回 0;
    }
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) const
  {
    如果 (unlikely (!c->may_dispatch (this, &u.format.v))) 返回 c->no_dispatch_return_value ();
    TRACE_DISPATCH (this, u.format.v);
    switch (u.format.v) {
    case 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    case 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    case 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    default:return_trace (c->default_return_value ());
    }
  }

  void collect_variation_indices (hb_collect_variation_indices_context_t *c) const
  {
    switch (u.format.v) {
    case 1:
    case 2:
      返回;
    case 3:
      hb_barrier ();
      u.format3.collect_variation_indices (c);
      返回;
    default: 返回;
    }
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    如果 (!u.format.v.sanitize (c)) return_trace (false);
    hb_barrier ();
    switch (u.format.v) {
    case 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    case 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    case 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    default:return_trace (true);
    }
  }

  protected:
  union {
  struct { HBUINT16 v; }	format;		/* Format identifier */
  CaretValueFormat1	format1;
  CaretValueFormat2	format2;
  CaretValueFormat3	format3;
  } u;
  public:
  DEFINE_SIZE_UNION (2, format.v);
};

struct LigGlyph
{
  unsigned get_lig_carets (hb_font_t            *font,
			   hb_direction_t        direction,
			   hb_codepoint_t        glyph_id,
			   const ItemVariationStore &var_store,
			   unsigned              start_offset,
			   unsigned             *caret_count /* IN/OUT */,
			   hb_position_t        *caret_array /* OUT */) const
  {
    如果 (caret_count && caret_array)
    {
      + carets.as_array ().sub_array (start_offset, caret_count)
      | hb_map (hb_add (this))
      | hb_map ([&] (const CaretValue &value) { 返回 value.get_caret_value (font, direction, glyph_id, var_store); })
      | hb_sink (hb_array (caret_array, *caret_count))
      ;
    }

    返回 carets.len;
  }

  bool subset (hb_subset_context_t *c) const
  {
    TRACE_SUBSET (this);
    auto *out = c->serializer->start_embed (*this);
    如果 (unlikely (!c->serializer->extend_min (out))) return_trace (false);

    + hb_iter (carets)
    | hb_apply (subset_offset_array (c, out->carets, this))
    ;

    return_trace (bool (out->carets));
  }

  void collect_variation_indices (hb_collect_variation_indices_context_t *c) const
  {
    为 (const Offset16To<CaretValue>& offset : carets.iter ())
      (this+offset).collect_variation_indices (c);
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (carets.sanitize (c, this));
  }

  protected:
  Array16OfOffset16To<CaretValue>
		carets;			/* Offset array of CaretValue tables
					 * --from beginning of LigGlyph table
					 * --in increasing coordinate order */
  public:
  DEFINE_SIZE_ARRAY (2, carets);
};

/* LigCaretList2 widens the LigCaretList coverage, count, and LigGlyph
 * offsets. The LigGlyph table itself keeps Offset16 caretValueOffsets. */
template <typename Types>
struct LigCaretList
{
  unsigned 整数 get_lig_carets (hb_font_t *font,
			       hb_direction_t direction,
			       hb_codepoint_t glyph_id,
			       const ItemVariationStore &var_store,
			       unsigned 整数 start_offset,
			       unsigned 整数 *caret_count /* IN/OUT */,
			       hb_position_t *caret_array /* OUT */) const
  {
    unsigned 整数 index = (this+coverage).get_coverage (glyph_id);
    如果 (index == NOT_COVERED)
    {
      如果 (caret_count)
	*caret_count = 0;
      返回 0;
    }
    const LigGlyph &lig_glyph = this+ligGlyph[index];
    返回 lig_glyph.get_lig_carets (font, direction, glyph_id, var_store, start_offset, caret_count, caret_array);
  }

  bool subset (hb_subset_context_t *c) const
  {
    TRACE_SUBSET (this);
    const hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    const hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*this);
    如果 (unlikely (!c->serializer->extend_min (out))) return_trace (false);

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (this+coverage, ligGlyph)
    | hb_filter (glyphset, hb_first)
    | hb_filter (subset_offset_array (c, out->ligGlyph, this), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;
    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (bool (new_coverage));
  }

  void collect_variation_indices (hb_collect_variation_indices_context_t *c) const
  {
    + hb_zip (this+coverage, ligGlyph)
    | hb_filter (c->glyph_set, hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (this))
    | hb_apply ([c] (const LigGlyph& _) { _.collect_variation_indices (c); })
    ;
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (coverage.sanitize (c, this) && ligGlyph.sanitize (c, this));
  }

  protected:
  typename Types::template LOffsetTo<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of LigCaretList table */
  typename Types::template ArrayOf<typename Types::template OffsetTo<LigGlyph>>
		ligGlyph;		/* Array of LigGlyph tables
					 * in Coverage Index order */
  public:
  DEFINE_SIZE_ARRAY (Types::LOffset::static_size + Types::HBUINT::static_size, ligGlyph);
};

#ifndef HB_NO_BEYOND_64K
static_assert (LigCaretList<MediumTypes>::min_size ==
	       Offset32::static_size + HBUINT24::static_size, "");
#endif


struct MarkGlyphSetsFormat1
{
  bool covers (unsigned 整数 set_index, hb_codepoint_t glyph_id) const
  { 返回 (this+coverage[set_index]).get_coverage (glyph_id) != NOT_COVERED; }

  void collect_used_mark_sets (const hb_set_t& glyph_set,
                               hb_set_t& used_mark_sets /* OUT */) const
  {
    unsigned i = 0;
    为 (const auto &offset : coverage)
     {
       const auto &cov = this+offset;
       如果 (cov.intersects (&glyph_set))
         used_mark_sets.add (i);

       i++;
     }
  }

  template <typename set_t>
  void collect_coverage (hb_vector_t<set_t> &sets) const
  {
     为 (const auto &offset : coverage)
     {
       const auto &cov = this+offset;
       cov.collect_coverage (sets.push ());
     }
  }

  bool subset (hb_subset_context_t *c) const
  {
    TRACE_SUBSET (this);
    auto *out = c->serializer->start_embed (*this);
    如果 (unlikely (!c->serializer->extend_min (out))) return_trace (false);
    out->format = format;

    bool ret = true;
    为 (const Offset32To<Coverage>& offset : coverage.iter ())
    {
      auto snap = c->serializer->snapshot ();
      auto *o = out->coverage.serialize_append (c->serializer);
      如果 (unlikely (!o))
      {
	ret = false;
	break;
      }

      //skip empty coverage
      c->serializer->push ();
      bool res = false;
      如果 (offset) res = c->dispatch (this+offset);
      如果 (!res)
      {
        c->serializer->pop_discard ();
        c->serializer->revert (snap);
        (out->coverage.len)--;
        continue;
      }
      c->serializer->add_link (*o, c->serializer->pop_pack ());
    }

    return_trace (ret && out->coverage.len);
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (coverage.sanitize (c, this));
  }

  protected:
  HBUINT16	format;			/* Format identifier--format = 1 */
  Array16Of<Offset32To<Coverage>>
		coverage;		/* Array of long offsets to mark set
					 * coverage tables */
  public:
  DEFINE_SIZE_ARRAY (4, coverage);
};

struct MarkGlyphSets
{
  bool covers (unsigned 整数 set_index, hb_codepoint_t glyph_id) const
  {
    switch (u.format.v) {
    case 1: hb_barrier (); 返回 u.format1.covers (set_index, glyph_id);
    default:返回 false;
    }
  }

  template <typename set_t>
  void collect_coverage (hb_vector_t<set_t> &sets) const
  {
    switch (u.format.v) {
    case 1: hb_barrier (); u.format1.collect_coverage (sets); 返回;
    default:返回;
    }
  }

  void collect_used_mark_sets (const hb_set_t& glyph_set,
                               hb_set_t& used_mark_sets /* OUT */) const
  {
    switch (u.format.v) {
    case 1: hb_barrier (); u.format1.collect_used_mark_sets (glyph_set, used_mark_sets); 返回;
    default:返回;
    }
  }

  bool subset (hb_subset_context_t *c) const
  {
    TRACE_SUBSET (this);
    switch (u.format.v) {
    case 1: hb_barrier (); return_trace (u.format1.subset (c));
    default:return_trace (false);
    }
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    如果 (!u.format.v.sanitize (c)) return_trace (false);
    hb_barrier ();
    switch (u.format.v) {
    case 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    default:return_trace (true);
    }
  }

  protected:
  union {
  struct { HBUINT16 v; }	format;		/* Format identifier */
  MarkGlyphSetsFormat1	format1;
  } u;
  public:
  DEFINE_SIZE_UNION (2, format.v);
};


/*
 * GDEF -- Glyph Definition
 * https://docs.microsoft.com/en-us/typography/opentype/spec/gdef
 */


struct GDEF
{
  static constexpr hb_tag_t tableTag = HB_OT_TAG_GDEF;

  enum GlyphClasses {
    UnclassifiedGlyph	= 0,
    BaseGlyph		= 1,
    LigatureGlyph	= 2,
    MarkGlyph		= 3,
    ComponentGlyph	= 4
  };

  size_t get_size () const
  {
    如果 (version.major != 1) 返回 version.static_size;

    返回 min_size +
	   (version.to_int () >= 0x00010002u ? markGlyphSetsDef.static_size : 0) +
	   (version.to_int () >= 0x00010003u ? varStore.static_size : 0) +
#ifndef HB_NO_BEYOND_64K
	   (version.to_int () >= 0x00010004u ? 5 * Offset32::static_size : 0)
#否则
	   0
#endif
	   ;
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    如果 (unlikely (!version.sanitize (c) ||
		  version.major != 1 ||
		  !c->check_range (this, get_size ())))
      return_trace (false);
    hb_barrier ();

    bool glyph_class_def_ok = glyphClassDef.sanitize (c, this);
    bool attach_list_ok = attachList.sanitize (c, this);
    bool lig_caret_list_ok = ligCaretList.sanitize (c, this);
    bool mark_attach_class_def_ok = markAttachClassDef.sanitize (c, this);
    bool mark_glyph_sets_def_ok = version.to_int () < 0x00010002u ||
				  markGlyphSetsDef.sanitize (c, this);

#ifndef HB_NO_BEYOND_64K
    如果 (version.to_int () >= 0x00010004u)
    {
      如果 (glyphClassDef2) glyph_class_def_ok = glyphClassDef2.sanitize (c, this);
      如果 (attachList2) attach_list_ok = attachList2.sanitize (c, this);
      如果 (ligCaretList2) lig_caret_list_ok = ligCaretList2.sanitize (c, this);
      如果 (markAttachClassDef2) mark_attach_class_def_ok = markAttachClassDef2.sanitize (c, this);
      如果 (markGlyphSetsDef2) mark_glyph_sets_def_ok = markGlyphSetsDef2.sanitize (c, this);
    }
#endif

    return_trace (glyph_class_def_ok &&
		  attach_list_ok &&
		  lig_caret_list_ok &&
		  mark_attach_class_def_ok &&
		  mark_glyph_sets_def_ok &&
		  (version.to_int () < 0x00010003u || varStore.sanitize (c, this)));
  }

  bool subset (hb_subset_context_t *c) const
  {
    TRACE_SUBSET (this);
    如果 (version.major != 1) return_trace (false);

    auto *out = c->serializer->start_embed (*this);
    如果 (unlikely (!c->serializer->extend_min (out))) return_trace (false);

    // Push var store first (if it's needed) so that it's last in the
    // serialization order. Some font consumers assume that varstore runs to
    // the end of the GDEF table.
    // See: https://github.com/harfbuzz/harfbuzz/issues/4636
    auto snapshot_version0 = c->serializer->snapshot ();
    如果 (unlikely (version.to_int () >= 0x00010002u && hb_barrier () &&
		  !c->serializer->embed (markGlyphSetsDef)))
      return_trace (false);

    auto snapshot_version2 = c->serializer->snapshot ();
    如果 (unlikely (version.to_int () >= 0x00010003u && hb_barrier () &&
		  !c->serializer->embed (varStore)))
      return_trace (false);

    bool use_v14 = false;
#ifndef HB_NO_BEYOND_64K
    use_v14 = version.to_int () >= 0x00010004u &&
	      (glyphClassDef2 || attachList2 || ligCaretList2 ||
	       markAttachClassDef2 || markGlyphSetsDef2);
    如果 (unlikely (version.to_int () >= 0x00010004u && hb_barrier () &&
		  !c->serializer->extend_size (out, get_size ())))
      return_trace (false);
#endif

    bool subset_varstore = false;
    unsigned varstore_index = (unsigned) -1;
    如果 (version.to_int () >= 0x00010003u && hb_barrier ())
    {
      如果 (c->plan->all_axes_pinned)
	out->varStore = 0;
      否则 如果 (c->plan->normalized_coords)
      {
	如果 (varStore)
	{
	  item_variations_t item_vars;
	  如果 (unlikely (!c->serializer->check_success (
                  item_vars.instantiate (this+varStore, c->plan, true, true,
                                          c->plan->gdef_varstore_inner_maps.as_array (), true))))
            return_trace (false);
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
      否则
      {
	subset_varstore = out->varStore.serialize_subset (c, varStore, this,
							  c->plan->gdef_varstore_inner_maps.as_array ());
	varstore_index = c->serializer->last_added_child_index();
      }
    }

    out->version.major = version.major;
    out->version.minor = version.minor;

    如果 (!subset_varstore && version.to_int () >= 0x00010002u && !use_v14) {
      c->serializer->revert (snapshot_version2);
    }
    如果 (!subset_varstore)
      out->varStore = 0;
    否则
      c->plan->has_gdef_varstore = true;

    bool subset_markglyphsetsdef = false;
    如果 (version.to_int () >= 0x00010002u && hb_barrier ())
    {
#ifndef HB_NO_BEYOND_64K
      如果 (use_v14 && markGlyphSetsDef2)
      {
	out->markGlyphSetsDef = 0;
	subset_markglyphsetsdef = serialize_subset_offset (c, out->markGlyphSetsDef2,
							   markGlyphSetsDef2, this);
      }
      否则
#endif
      {
#ifndef HB_NO_BEYOND_64K
	如果 (use_v14) out->markGlyphSetsDef2 = 0;
#endif
	subset_markglyphsetsdef = out->markGlyphSetsDef.serialize_subset (c, markGlyphSetsDef, this);
      }
    }

    如果 (use_v14)
    {
      out->version.minor = 4;
    } 否则 如果 (subset_varstore) {
      out->version.minor = 3;
    } 否则 如果 (subset_markglyphsetsdef) {
      out->version.minor = 2;
    } 否则  {
      out->version.minor = 0;
      c->serializer->revert (snapshot_version0);
    }

    bool subset_glyphclassdef = false;
    bool subset_attachlist = false;
    bool subset_markattachclassdef = false;
    bool subset_ligcaretlist = false;

#ifndef HB_NO_BEYOND_64K
    如果 (use_v14 && glyphClassDef2)
    {
      out->glyphClassDef = 0;
      subset_glyphclassdef = serialize_subset_offset (c, out->glyphClassDef2,
						      glyphClassDef2, this, nullptr, false, true);
    }
    否则
#endif
    {
#ifndef HB_NO_BEYOND_64K
      如果 (use_v14) out->glyphClassDef2 = 0;
#endif
      subset_glyphclassdef = out->glyphClassDef.serialize_subset (c, glyphClassDef, this, nullptr, false, true);
    }

#ifndef HB_NO_BEYOND_64K
    如果 (use_v14 && attachList2)
    {
      out->attachList = 0;
      subset_attachlist = serialize_subset_offset (c, out->attachList2, attachList2, this);
    }
    否则
#endif
    {
#ifndef HB_NO_BEYOND_64K
      如果 (use_v14) out->attachList2 = 0;
#endif
      subset_attachlist = out->attachList.serialize_subset (c, attachList, this);
    }

#ifndef HB_NO_BEYOND_64K
    如果 (use_v14 && markAttachClassDef2)
    {
      out->markAttachClassDef = 0;
      subset_markattachclassdef = serialize_subset_offset (c, out->markAttachClassDef2,
							   markAttachClassDef2, this, nullptr, false, true);
    }
    否则
#endif
    {
#ifndef HB_NO_BEYOND_64K
      如果 (use_v14) out->markAttachClassDef2 = 0;
#endif
      subset_markattachclassdef = out->markAttachClassDef.serialize_subset (c, markAttachClassDef,
									   this, nullptr, false, true);
    }

#ifndef HB_NO_BEYOND_64K
    如果 (use_v14 && ligCaretList2)
    {
      out->ligCaretList = 0;
      subset_ligcaretlist = serialize_subset_offset (c, out->ligCaretList2, ligCaretList2, this);
    }
    否则
#endif
    {
#ifndef HB_NO_BEYOND_64K
      如果 (use_v14) out->ligCaretList2 = 0;
#endif
      subset_ligcaretlist = out->ligCaretList.serialize_subset (c, ligCaretList, this);
    }

    如果 (subset_varstore && varstore_index != (unsigned) -1) {
      c->serializer->repack_last(varstore_index);
    }

    return_trace (subset_glyphclassdef || subset_attachlist ||
		  subset_ligcaretlist || subset_markattachclassdef ||
		  (out->version.to_int () >= 0x00010002u && subset_markglyphsetsdef) ||
		  (out->version.to_int () >= 0x00010003u && subset_varstore));
  }

  bool has_glyph_classes () const
  {
#ifndef HB_NO_BEYOND_64K
    如果 (version.to_int () >= 0x00010004u && glyphClassDef2) 返回 true;
#endif
    返回 glyphClassDef != 0;
  }
  const ClassDef &get_glyph_class_def () const
  {
#ifndef HB_NO_BEYOND_64K
    如果 (version.to_int () >= 0x00010004u && glyphClassDef2) 返回 this+glyphClassDef2;
#endif
    返回 this+glyphClassDef;
  }
  bool has_attach_list () const
  {
#ifndef HB_NO_BEYOND_64K
    如果 (version.to_int () >= 0x00010004u && attachList2) 返回 true;
#endif
    返回 attachList != 0;
  }
  const AttachList &get_attach_list () const
  {
#ifndef HB_NO_BEYOND_64K
    如果 (version.to_int () >= 0x00010004u && attachList2) 返回 this+attachList2;
#endif
    返回 this+attachList;
  }
  bool has_lig_carets () const
  {
#ifndef HB_NO_BEYOND_64K
    如果 (version.to_int () >= 0x00010004u && ligCaretList2) 返回 true;
#endif
    返回 ligCaretList != 0;
  }
  bool has_mark_attachment_types () const
  {
#ifndef HB_NO_BEYOND_64K
    如果 (version.to_int () >= 0x00010004u && markAttachClassDef2) 返回 true;
#endif
    返回 markAttachClassDef != 0;
  }
  const ClassDef &get_mark_attach_class_def () const
  {
#ifndef HB_NO_BEYOND_64K
    如果 (version.to_int () >= 0x00010004u && markAttachClassDef2) 返回 this+markAttachClassDef2;
#endif
    返回 this+markAttachClassDef;
  }
  bool has_mark_glyph_sets () const
  {
    如果 (version.to_int () < 0x00010002u) 返回 false;
#ifndef HB_NO_BEYOND_64K
    如果 (version.to_int () >= 0x00010004u && markGlyphSetsDef2) 返回 true;
#endif
    返回 markGlyphSetsDef != 0;
  }
  const MarkGlyphSets &get_mark_glyph_sets () const
  {
    如果 (version.to_int () < 0x00010002u) 返回 Null(MarkGlyphSets);
#ifndef HB_NO_BEYOND_64K
    如果 (version.to_int () >= 0x00010004u && markGlyphSetsDef2) 返回 this+markGlyphSetsDef2;
#endif
    返回 this+markGlyphSetsDef;
  }
  bool has_var_store () const
  {
    返回 version.to_int () >= 0x00010003u && hb_barrier () && varStore != 0;
  }
  const ItemVariationStore &get_var_store () const
  {
    返回 version.to_int () >= 0x00010003u && hb_barrier () ? this+varStore : Null(ItemVariationStore);
  }


  bool has_data () const { 返回 version.to_int (); }
  unsigned 整数 get_glyph_class (hb_codepoint_t glyph) const
  { 返回 get_glyph_class_def ().get_class (glyph); }
  void get_glyphs_in_class (unsigned 整数 klass, hb_set_t *glyphs) const
  { get_glyph_class_def ().collect_class (glyphs, klass); }

  unsigned 整数 get_mark_attachment_type (hb_codepoint_t glyph) const
  { 返回 get_mark_attach_class_def ().get_class (glyph); }

  unsigned 整数 get_attach_points (hb_codepoint_t glyph_id,
				  unsigned 整数 start_offset,
				  unsigned 整数 *point_count /* IN/OUT */,
				  unsigned 整数 *point_array /* OUT */) const
  { 返回 get_attach_list ().get_attach_points (glyph_id, start_offset, point_count, point_array); }

  unsigned 整数 get_lig_carets (hb_font_t *font,
			       hb_direction_t direction,
			       hb_codepoint_t glyph_id,
			       unsigned 整数 start_offset,
			       unsigned 整数 *caret_count /* IN/OUT */,
			       hb_position_t *caret_array /* OUT */) const
  {
#ifndef HB_NO_BEYOND_64K
    如果 (version.to_int () >= 0x00010004u && ligCaretList2)
      返回 (this+ligCaretList2).get_lig_carets (font, direction, glyph_id, get_var_store (),
						  start_offset, caret_count, caret_array);
#endif
    返回 (this+ligCaretList).get_lig_carets (font, direction, glyph_id, get_var_store (),
					       start_offset, caret_count, caret_array);
  }

  bool mark_set_covers (unsigned 整数 set_index, hb_codepoint_t glyph_id) const
  { 返回 get_mark_glyph_sets ().covers (set_index, glyph_id); }

  /* glyph_props is a 16-bit integer where the lower 8-bit have bits representing
   * glyph class and other bits, and high 8-bit the mark attachment type (if any).
   * Not to be confused with lookup_props which is very similar. */
  unsigned 整数 get_glyph_props (hb_codepoint_t glyph) const
  {
    unsigned 整数 klass = get_glyph_class (glyph);

    static_assert (((unsigned 整数) HB_OT_LAYOUT_GLYPH_PROPS_BASE_GLYPH == (unsigned 整数) LookupFlag::IgnoreBaseGlyphs), "");
    static_assert (((unsigned 整数) HB_OT_LAYOUT_GLYPH_PROPS_LIGATURE == (unsigned 整数) LookupFlag::IgnoreLigatures), "");
    static_assert (((unsigned 整数) HB_OT_LAYOUT_GLYPH_PROPS_MARK == (unsigned 整数) LookupFlag::IgnoreMarks), "");

    switch (klass) {
    default:			返回 HB_OT_LAYOUT_GLYPH_CLASS_UNCLASSIFIED;
    case BaseGlyph:		返回 HB_OT_LAYOUT_GLYPH_PROPS_BASE_GLYPH;
    case LigatureGlyph:		返回 HB_OT_LAYOUT_GLYPH_PROPS_LIGATURE;
    case MarkGlyph:
	  klass = get_mark_attachment_type (glyph);
	  返回 HB_OT_LAYOUT_GLYPH_PROPS_MARK | (klass << 8);
    }
  }

  HB_INTERNAL bool is_blocklisted (hb_blob_t *blob,
				   hb_face_t *face) const;

  struct accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      table = hb_sanitize_context_t ().reference_table<GDEF> (face);
      如果 (unlikely (table->is_blocklisted (table.get_blob (), face)))
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
	如果 (!sets.in_error () &&
	    mark_glyph_set_bitmaps.resize (sets.length))
	  为 (unsigned i = 0; i < sets.length; i++)
	  {
	    auto &bitmap = mark_glyph_set_bitmaps.arrayZ[i];
	    bitmap.bias = HB_SET_VALUE_INVALID;
	    const auto &set = sets.arrayZ[i];
	    如果 (unlikely (set.in_error ())) continue;
	    hb_codepoint_t min_gid = HB_SET_VALUE_INVALID;
	    hb_codepoint_t max_gid = HB_SET_VALUE_INVALID;
	    hb_set_next (&set, &min_gid);
	    hb_set_previous (&set, &max_gid);
	    如果 (max_gid == HB_SET_VALUE_INVALID)
	      continue;
	    如果 (max_gid - min_gid >= hb_bit_page_t::PAGE_BITS)
	    {
	      /* Too wide for one page; a digest pre-filter instead. */
	      为 (hb_codepoint_t gid : set)
		bitmap.u.digest.add (gid);
	      continue;
	    }
	    bitmap.bias = min_gid;
	    为 (hb_codepoint_t gid : set)
	      bitmap.u.page.add (gid - min_gid);
	  }
      }
#endif
    }
    ~accelerator_t () { table.destroy (); }

    unsigned 整数 get_glyph_props (hb_codepoint_t glyph) const
    {
      unsigned v;

#ifndef HB_NO_GDEF_CACHE
      如果 (glyph_props_cache.get (glyph, &v))
        返回 v;
#endif

      v = table->get_glyph_props (glyph);

#ifndef HB_NO_GDEF_CACHE
      如果 (likely (table.get_blob ())) // Don't try setting if we are the null instance!
	glyph_props_cache.set (glyph, v);
#endif

      返回 v;

    }

    HB_ALWAYS_INLINE
    bool mark_set_covers (unsigned 整数 set_index, hb_codepoint_t glyph_id) const
    {
#ifndef HB_NO_GDEF_CACHE
      // We can access arrayZ directly because of sanitize_lookup_props() guarantee.
      const auto &bitmap = mark_glyph_set_bitmaps.arrayZ[set_index];
      如果 (likely (bitmap.bias != HB_SET_VALUE_INVALID))
      {
	hb_codepoint_t biased = glyph_id - bitmap.bias;
	返回 biased < hb_bit_page_t::PAGE_BITS &&
	       bitmap.u.page.get (biased);
      }
      返回 mark_set_covers_slow (set_index, glyph_id);
#否则
      返回 table->mark_set_covers (set_index, glyph_id);
#endif
    }

#ifndef HB_NO_GDEF_CACHE
    HB_NEVER_INLINE
    bool mark_set_covers_slow (unsigned 整数 set_index, hb_codepoint_t glyph_id) const
    {
      const auto &bitmap = mark_glyph_set_bitmaps.arrayZ[set_index];
      如果 (!bitmap.u.digest.may_have (glyph_id))
	返回 false;
      返回 table->mark_set_covers (set_index, glyph_id);
    }
#endif

    unsigned sanitize_lookup_props (unsigned lookup_props) const
    {
#ifndef HB_NO_GDEF_CACHE
      如果 (lookup_props & LookupFlag::UseMarkFilteringSet &&
	  (lookup_props >> 16) >= mark_glyph_set_bitmaps.length)
      {
        // Invalid mark filtering set index; unset the flag.
	lookup_props &= ~LookupFlag::UseMarkFilteringSet;
      }
#endif
      返回 lookup_props;
    }

    hb_blob_ptr_t<GDEF> table;
#ifndef HB_NO_GDEF_CACHE
    struct mark_glyph_set_bitmap_t
    {
      /* Gid of the first bit (not page-aligned) for an exact one-page
       * set, or HB_SET_VALUE_INVALID; then digest is a pre-filter for
       * the table.  The all-zeros state is valid for both members. */
      hb_codepoint_t bias = HB_SET_VALUE_INVALID;
      union u_t {
	u_t () : page () {}
	hb_bit_page_t page;
	hb_set_digest_t digest;
      } u;
    };

    hb_vector_t<mark_glyph_set_bitmap_t> mark_glyph_set_bitmaps;
    mutable hb_cache_t<21, 3> glyph_props_cache;
    static_assert (sizeof (glyph_props_cache) == 512, "");
#endif
  };

  void collect_variation_indices (hb_collect_variation_indices_context_t *c) const
  {
#ifndef HB_NO_BEYOND_64K
    如果 (version.to_int () >= 0x00010004u && ligCaretList2)
    {
      (this+ligCaretList2).collect_variation_indices (c);
      返回;
    }
#endif
    (this+ligCaretList).collect_variation_indices (c);
  }

  protected:
  template <typename Key>
  static void remap_varidx_after_instantiation (const hb_map_t& varidx_map,
                                                hb_hashmap_t<Key, hb_pair_t<unsigned, 整数>>& layout_variation_idx_delta_map /* IN/OUT */)
  {
    /* varidx_map is empty which means varstore is empty after instantiation,
     * no variations, map all varidx to HB_OT_LAYOUT_NO_VARIATIONS_INDEX.
     * varidx_map doesn't have original varidx, indicating delta row is all
     * zeros, map varidx to HB_OT_LAYOUT_NO_VARIATIONS_INDEX */
    为 (auto _ : layout_variation_idx_delta_map.iter_ref ())
    {
      /* old_varidx->(varidx, delta) mapping generated for subsetting, then this
       * varidx is used as key of varidx_map during instantiation */
      uint32_t varidx = _.second.first;
      uint32_t *new_varidx;
      如果 (varidx_map.has (varidx, &new_varidx))
	_.second.first = *new_varidx;
      否则
	_.second.first = HB_OT_LAYOUT_NO_VARIATIONS_INDEX;
    }
  }

  template <typename OffsetOut, typename OffsetIn, typename Base, typename ...Ts>
  static bool serialize_subset_offset (hb_subset_context_t *c,
				       OffsetOut& out,
				       const OffsetIn& in,
				       const Base *base,
				       Ts&&... ds)
  {
    out = 0;
    如果 (in.is_null ()) 返回 false;

    auto *s = c->serializer;
    s->push ();
    bool ret = c->dispatch (base+in, std::forward<Ts> (ds)...);
    如果 (ret) s->add_link (out, s->pop_pack ());
    否则 s->pop_discard ();
    返回 ret;
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
  public:
  DEFINE_SIZE_MIN (12);
};

struct GDEF_accelerator_t : GDEF::accelerator_t {
  GDEF_accelerator_t (hb_face_t *face) : GDEF::accelerator_t (face) {}
};

} /* namespace OT */


#endif /* OT_LAYOUT_GDEF_GDEF_HH */
