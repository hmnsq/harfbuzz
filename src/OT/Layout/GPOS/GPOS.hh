#ifndef OT_LAYOUT_GPOS_GPOS_HH
#define OT_LAYOUT_GPOS_GPOS_HH

#include "../../../hb-ot-layout-common.hh"
#include "../../../hb-ot-layout-gsubgpos.hh"
#include "Common.hh"
#include "PosLookup.hh"

namespace OT {

using Layout::GPOS_impl::PosLookup;

namespace Layout {

static void
propagate_attachment_offsets (hb_glyph_position_t *pos,
                              unsigned 정수 len,
                              unsigned 정수 i,
                              hb_direction_t direction,
                              unsigned nesting_level = HB_MAX_NESTING_LEVEL);

/*
 * GPOS -- Glyph Positioning
 * https://docs.microsoft.com/en-us/typography/opentype/spec/gpos
 */

struct GPOS : GSUBGPOS
{
  static constexpr hb_tag_t tableTag = HB_OT_TAG_GPOS;

  using Lookup = PosLookup;

  const PosLookup& get_lookup (unsigned 정수 i) const
  { 반환 static_cast<const PosLookup &> (GSUBGPOS::get_lookup (i)); }

  static inline void position_start (hb_font_t *font, hb_buffer_t *buffer);
  static inline void position_finish_advances (hb_font_t *font, hb_buffer_t *buffer);
  static inline void position_finish_offsets (hb_font_t *font, hb_buffer_t *buffer);

  bool subset (hb_subset_context_t *c) const
  {
    hb_subset_layout_context_t l (c, tableTag);
    반환 GSUBGPOS::subset<PosLookup> (&l);
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (GSUBGPOS::sanitize<PosLookup> (c));
  }

  HB_INTERNAL bool is_blocklisted (hb_blob_t *blob,
                                   hb_face_t *face) const;

  void collect_variation_indices (hb_collect_variation_indices_context_t *c) const
  {
    위해 (unsigned i = 0; i < GSUBGPOS::get_lookup_count (); i++)
    {
      만약 (!c->gpos_lookups->has (i)) continue;
      const PosLookup &l = get_lookup (i);
      l.dispatch (c);
    }
  }

  void closure_lookups (hb_face_t      *face,
                        const hb_set_t *glyphs,
                        hb_set_t       *lookup_indexes /* IN/OUT */) const
  { GSUBGPOS::closure_lookups<PosLookup> (face, glyphs, lookup_indexes); }

  typedef GSUBGPOS::accelerator_t<GPOS> accelerator_t;
};


static void
propagate_attachment_offsets (hb_glyph_position_t *pos,
                              unsigned 정수 len,
                              unsigned 정수 i,
                              hb_direction_t direction,
                              unsigned nesting_level)
{
  /* Adjusts offsets of attached glyphs (both cursive and mark) to accumulate
   * offset of glyph they are attached to. */
  정수 chain = pos[i].attach_chain();
  정수 type = pos[i].attach_type();

  pos[i].attach_chain() = 0;

  unsigned 정수 j = (정수) i + chain;

  만약 (unlikely (j >= len))
    반환;

  만약 (unlikely (!nesting_level))
    반환;

  만약 (pos[j].attach_chain())
    propagate_attachment_offsets (pos, len, j, direction, nesting_level - 1);

  assert (!!(type & GPOS_impl::ATTACH_TYPE_MARK) ^ !!(type & GPOS_impl::ATTACH_TYPE_CURSIVE));

  만약 (type & GPOS_impl::ATTACH_TYPE_CURSIVE)
  {
    만약 (HB_DIRECTION_IS_HORIZONTAL (direction))
      pos[i].y_offset = hb_saturate_add (pos[i].y_offset, pos[j].y_offset);
    아니면
      pos[i].x_offset = hb_saturate_add (pos[i].x_offset, pos[j].x_offset);
  }
  아니면 /*if (type & GPOS_impl::ATTACH_TYPE_MARK)*/
  {
    만약 (HB_DIRECTION_IS_HORIZONTAL (direction))
      pos[i].x_offset = hb_saturate_add (pos[i].x_offset, pos[j].x_offset);
    아니면
      pos[i].y_offset = hb_saturate_add (pos[i].y_offset, pos[j].y_offset);

    // i is the position of the mark; j is the base.
    만약 (j < i)
    {
      /* This is the common case: mark follows base.
       * And currently the only way in OpenType. */
      만약 (HB_DIRECTION_IS_FORWARD (direction))
	위해 (unsigned 정수 k = j; k < i; k++) {
	  pos[i].x_offset = hb_saturate_sub (pos[i].x_offset, pos[k].x_advance);
	  pos[i].y_offset = hb_saturate_sub (pos[i].y_offset, pos[k].y_advance);
	}
      아니면
	위해 (unsigned 정수 k = j + 1; k < i + 1; k++) {
	  pos[i].x_offset = hb_saturate_add (pos[i].x_offset, pos[k].x_advance);
	  pos[i].y_offset = hb_saturate_add (pos[i].y_offset, pos[k].y_advance);
	}
    }
    아니면 // j > i
    {
      /* This can happen with `kerx`: a mark attaching
       * to a base after it in the logical order. */
      만약 (HB_DIRECTION_IS_FORWARD (direction))
	위해 (unsigned 정수 k = i; k < j; k++) {
	  pos[i].x_offset = hb_saturate_add (pos[i].x_offset, pos[k].x_advance);
	  pos[i].y_offset = hb_saturate_add (pos[i].y_offset, pos[k].y_advance);
	}
      아니면
	위해 (unsigned 정수 k = i + 1; k < j + 1; k++) {
	  pos[i].x_offset = hb_saturate_sub (pos[i].x_offset, pos[k].x_advance);
	  pos[i].y_offset = hb_saturate_sub (pos[i].y_offset, pos[k].y_advance);
	}
    }
  }
}

void
GPOS::position_start (hb_font_t *font HB_UNUSED, hb_buffer_t *buffer)
{
  unsigned 정수 count = buffer->len;
  위해 (unsigned 정수 i = 0; i < count; i++)
    buffer->pos[i].attach_chain() = buffer->pos[i].attach_type() = 0;
}

void
GPOS::position_finish_advances (hb_font_t *font HB_UNUSED, hb_buffer_t *buffer HB_UNUSED)
{
  //_hb_buffer_assert_gsubgpos_vars (buffer);
}

void
GPOS::position_finish_offsets (hb_font_t *font, hb_buffer_t *buffer)
{
  _hb_buffer_assert_gsubgpos_vars (buffer);

  unsigned 정수 len;
  hb_glyph_position_t *pos = hb_buffer_get_glyph_positions (buffer, &len);
  hb_direction_t direction = buffer->props.direction;

  /* Handle attachments */
  만약 (buffer->scratch_flags & HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT)
  {
    auto *pos = buffer->pos;
    // https://github.com/harfbuzz/harfbuzz/issues/5514
    만약 (HB_DIRECTION_IS_FORWARD (direction))
    {
      위해 (unsigned i = 0; i < len; i++)
	만약 (pos[i].attach_chain())
	  propagate_attachment_offsets (pos, len, i, direction);
    } 아니면 {
      위해 (unsigned i = len; i-- > 0; )
	만약 (pos[i].attach_chain())
	  propagate_attachment_offsets (pos, len, i, direction);
    }
  }

  만약 (unlikely (font->slant_xy) &&
      HB_DIRECTION_IS_HORIZONTAL (direction))
  {
    /* Slanting shaping results is only supported for horizontal text,
     * as it gets weird otherwise. */
    위해 (unsigned i = 0; i < len; i++)
      만약 (unlikely (pos[i].y_offset))
        pos[i].x_offset = hb_clamp_to<hb_position_t> ((double) pos[i].x_offset +
						      (double) roundf (font->slant_xy * pos[i].y_offset));
  }
}

}

struct GPOS_accelerator_t : Layout::GPOS::accelerator_t {
  GPOS_accelerator_t (hb_face_t *face) : Layout::GPOS::accelerator_t (face) {}
};

}

#endif  /* OT_LAYOUT_GPOS_GPOS_HH */
