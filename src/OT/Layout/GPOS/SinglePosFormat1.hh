#ifndef OT_LAYOUT_GPOS_SINGLEPOSFORMAT1_HH
#define OT_LAYOUT_GPOS_SINGLEPOSFORMAT1_HH

#include "Common.hh"
#include "ValueFormat.hh"

namespace OT {
namespace Layout {
namespace GPOS_impl {

template <typename Types>
struct SinglePosFormat1_3 : ValueBase
{
  protected:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of subtable */
  ValueFormat   valueFormat;            /* Defines the types of data in the
                                         * ValueRecord */
  ValueRecord   values;                 /* Defines positioning
                                         * value(s)--applied to all glyphs in
                                         * the Coverage table */
  public:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + ValueFormat::static_size, values);

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this) &&
                  coverage.sanitize (c, this) &&
		  hb_barrier () &&
                  /* The coverage  table may use a range to represent a set
                   * of glyphs, which means a small number of bytes can
                   * generate a large glyph set. Manually modify the
                   * sanitizer max ops to take this into account.
                   *
                   * Note: This check *must* be right after coverage sanitize. */
                  c->check_ops ((this + coverage).get_population () >> 1) &&
                  valueFormat.sanitize_value (c, this, values));

  }

  bool intersects (const hb_set_t *glyphs) const
  { 반환 (this+coverage).intersects (glyphs); }

  void closure_lookups (hb_closure_lookups_context_t *c) const {}
  void collect_variation_indices (hb_collect_variation_indices_context_t *c) const
  {
    만약 (!valueFormat.has_device ()) 반환;

    hb_set_t intersection;
    (this+coverage).intersect_set (*c->glyph_set, intersection);
    만약 (!intersection) 반환;

    valueFormat.collect_variation_indices (c, this, values.as_array (valueFormat.get_len ()));
  }

  void collect_glyphs (hb_collect_glyphs_context_t *c) const
  { 만약 (unlikely (!(this+coverage).collect_coverage (c->input))) 반환; }

  const Coverage &get_coverage () const { 반환 this+coverage; }

  ValueFormat get_value_format () const { 반환 valueFormat; }

  bool apply (hb_ot_apply_context_t *c) const
  {
    TRACE_APPLY (this);
    hb_buffer_t *buffer = c->buffer;
    unsigned 정수 index = (this+coverage).get_coverage  (buffer->cur().codepoint);
    만약 (index == NOT_COVERED) return_trace (false);

    만약 (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "positioning glyph at %u",
			  c->buffer->idx);
    }

    valueFormat.apply_value (c, this, values, buffer->cur_pos());

    만약 (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "positioned glyph at %u",
			  c->buffer->idx);
    }

    buffer->idx++;
    return_trace (true);
  }

  bool
  position_single (hb_font_t           *font,
		   hb_blob_t           *table_blob,
		   hb_direction_t       direction,
		   hb_codepoint_t       gid,
		   hb_glyph_position_t &pos) const
  {
    unsigned 정수 index = (this+coverage).get_coverage  (gid);
    만약 (likely (index == NOT_COVERED)) 반환 false;

    valueFormat.position_single (font, table_blob, direction, this,
                                 values, pos);
    반환 true;
  }

  template<typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  void serialize (hb_serialize_context_t *c,
                  const ValueBase *src,
                  ValueFormat src_format,
                  Iterator it,
                  ValueFormat newFormat,
                  const hb_hashmap_t<unsigned, hb_pair_t<unsigned, 정수>> *layout_variation_idx_delta_map)
  {
    만약 (unlikely (!c->extend_min (this))) 반환;
    만약 (unlikely (!c->check_assign (valueFormat,
                                    newFormat,
                                    HB_SERIALIZE_ERROR_INT_OVERFLOW))) 반환;

    위해 (const hb_array_t<const Value>& _ : + it | hb_map (hb_second))
    {
      src_format.copy_values (c, newFormat, src,  &_, layout_variation_idx_delta_map);
      // Only serialize the first entry in the iterator, the rest are assumed to
      // be the same.
      break;
    }

    auto glyphs =
    + it
    | hb_map_retains_sorting (hb_first)
    ;

    coverage.serialize_serialize (c, glyphs);
  }

  bool subset (hb_subset_context_t *c) const
  {
    TRACE_SUBSET (this);
    const hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    const hb_map_t &glyph_map = *c->plan->glyph_map;

    hb_set_t intersection;
    (this+coverage).intersect_set (glyphset, intersection);

    unsigned new_format = valueFormat;

    만약 (c->plan->normalized_coords)
    {
      new_format = valueFormat.get_effective_format (values.arrayZ, false, false, this, &c->plan->layout_variation_idx_delta_map);
    }
    /* do not strip hints for VF */
    아니면 만약 (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
    {
      hb_blob_t* blob = hb_face_reference_table (c->plan->source, HB_TAG ('f','v','a','r'));
      bool has_fvar = (blob != hb_blob_get_empty ());
      hb_blob_destroy (blob);

      bool strip = !has_fvar;
      /* special case: strip hints when a VF has no GDEF varstore after
       * subsetting*/
      만약 (has_fvar && !c->plan->has_gdef_varstore)
        strip = true;
      new_format = valueFormat.get_effective_format (values.arrayZ,
                                                     strip, /* strip hints */
                                                     true, /* strip empty */
                                                     this, nullptr);
    }

    auto it =
    + hb_iter (intersection)
    | hb_map_retains_sorting (glyph_map)
    | hb_zip (hb_repeat (values.as_array (valueFormat.get_len ())))
    ;

    bool ret = bool (it);
    SinglePos_serialize (c->serializer, this, valueFormat, it, &c->plan->layout_variation_idx_delta_map, new_format);
    return_trace (ret);
  }
};

}
}
}

#endif /* OT_LAYOUT_GPOS_SINGLEPOSFORMAT1_HH */
