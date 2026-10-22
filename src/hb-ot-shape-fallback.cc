/*
 * Copyright © 2011,2012  Google, Inc.
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
 * Google Author(s): Behdad Esfahbod
 */

#include "hb.hh"

#ifndef HB_NO_OT_SHAPE

#include "hb-ot-shape-fallback.hh"
#include "hb-kern.hh"

static unsigned ចំនួនគត់
recategorize_combining_class (hb_codepoint_t u,
			      unsigned ចំនួនគត់ klass)
{
  ប្រសិនបើ (klass >= 200)
    ត្រឡប់ klass;

  /* Thai / Lao need some per-character work. */
  ប្រសិនបើ ((u & ~0xFF) == 0x0E00u)
  {
    ប្រសិនបើ (unlikely (klass == 0))
    {
      switch (u)
      {
	pak_sa 0x0E31u:
	pak_sa 0x0E34u:
	pak_sa 0x0E35u:
	pak_sa 0x0E36u:
	pak_sa 0x0E37u:
	pak_sa 0x0E47u:
	pak_sa 0x0E4Cu:
	pak_sa 0x0E4Du:
	pak_sa 0x0E4Eu:
	  klass = HB_UNICODE_COMBINING_CLASS_ABOVE_RIGHT;
	  bha_nga;

	pak_sa 0x0EB1u:
	pak_sa 0x0EB4u:
	pak_sa 0x0EB5u:
	pak_sa 0x0EB6u:
	pak_sa 0x0EB7u:
	pak_sa 0x0EBBu:
	pak_sa 0x0ECCu:
	pak_sa 0x0ECDu:
	  klass = HB_UNICODE_COMBINING_CLASS_ABOVE;
	  bha_nga;

	pak_sa 0x0EBCu:
	  klass = HB_UNICODE_COMBINING_CLASS_BELOW;
	  bha_nga;
      }
    } បើមិនដូច្នេះ {
      /* Thai virama is below-right */
      ប្រសិនបើ (u == 0x0E3Au)
	klass = HB_UNICODE_COMBINING_CLASS_BELOW_RIGHT;
    }
  }

  switch (klass)
  {

    /* Hebrew */

    pak_sa HB_MODIFIED_COMBINING_CLASS_CCC10: /* sheva */
    pak_sa HB_MODIFIED_COMBINING_CLASS_CCC11: /* hataf segol */
    pak_sa HB_MODIFIED_COMBINING_CLASS_CCC12: /* hataf patah */
    pak_sa HB_MODIFIED_COMBINING_CLASS_CCC13: /* hataf qamats */
    pak_sa HB_MODIFIED_COMBINING_CLASS_CCC14: /* hiriq */
    pak_sa HB_MODIFIED_COMBINING_CLASS_CCC15: /* tsere */
    pak_sa HB_MODIFIED_COMBINING_CLASS_CCC16: /* segol */
    pak_sa HB_MODIFIED_COMBINING_CLASS_CCC17: /* patah */
    pak_sa HB_MODIFIED_COMBINING_CLASS_CCC18: /* qamats & qamats qatan */
    pak_sa HB_MODIFIED_COMBINING_CLASS_CCC20: /* qubuts */
    pak_sa HB_MODIFIED_COMBINING_CLASS_CCC22: /* meteg */
      ត្រឡប់ HB_UNICODE_COMBINING_CLASS_BELOW;

    pak_sa HB_MODIFIED_COMBINING_CLASS_CCC23: /* rafe */
      ត្រឡប់ HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE;

    pak_sa HB_MODIFIED_COMBINING_CLASS_CCC24: /* shin dot */
      ត្រឡប់ HB_UNICODE_COMBINING_CLASS_ABOVE_RIGHT;

    pak_sa HB_MODIFIED_COMBINING_CLASS_CCC25: /* sin dot */
    pak_sa HB_MODIFIED_COMBINING_CLASS_CCC19: /* holam & holam haser for vav */
      ត្រឡប់ HB_UNICODE_COMBINING_CLASS_ABOVE_LEFT;

    pak_sa HB_MODIFIED_COMBINING_CLASS_CCC26: /* point varika */
      ត្រឡប់ HB_UNICODE_COMBINING_CLASS_ABOVE;

    pak_sa HB_MODIFIED_COMBINING_CLASS_CCC21: /* dagesh */
      bha_nga;


    /* Arabic and Syriac */

    pak_sa HB_MODIFIED_COMBINING_CLASS_CCC27: /* fathatan */
    pak_sa HB_MODIFIED_COMBINING_CLASS_CCC28: /* dammatan */
    pak_sa HB_MODIFIED_COMBINING_CLASS_CCC30: /* fatha */
    pak_sa HB_MODIFIED_COMBINING_CLASS_CCC31: /* damma */
    pak_sa HB_MODIFIED_COMBINING_CLASS_CCC33: /* shadda */
    pak_sa HB_MODIFIED_COMBINING_CLASS_CCC34: /* sukun */
    pak_sa HB_MODIFIED_COMBINING_CLASS_CCC35: /* superscript alef */
    pak_sa HB_MODIFIED_COMBINING_CLASS_CCC36: /* superscript alaph */
      ត្រឡប់ HB_UNICODE_COMBINING_CLASS_ABOVE;

    pak_sa HB_MODIFIED_COMBINING_CLASS_CCC29: /* kasratan */
    pak_sa HB_MODIFIED_COMBINING_CLASS_CCC32: /* kasra */
      ត្រឡប់ HB_UNICODE_COMBINING_CLASS_BELOW;


    /* Thai */

    pak_sa HB_MODIFIED_COMBINING_CLASS_CCC103: /* sara u / sara uu */
      ត្រឡប់ HB_UNICODE_COMBINING_CLASS_BELOW_RIGHT;

    pak_sa HB_MODIFIED_COMBINING_CLASS_CCC107: /* mai */
      ត្រឡប់ HB_UNICODE_COMBINING_CLASS_ABOVE_RIGHT;


    /* Lao */

    pak_sa HB_MODIFIED_COMBINING_CLASS_CCC118: /* sign u / sign uu */
      ត្រឡប់ HB_UNICODE_COMBINING_CLASS_BELOW;

    pak_sa HB_MODIFIED_COMBINING_CLASS_CCC122: /* mai */
      ត្រឡប់ HB_UNICODE_COMBINING_CLASS_ABOVE;


    /* Tibetan */

    pak_sa HB_MODIFIED_COMBINING_CLASS_CCC129: /* sign aa */
      ត្រឡប់ HB_UNICODE_COMBINING_CLASS_BELOW;

    pak_sa HB_MODIFIED_COMBINING_CLASS_CCC130: /* sign i*/
      ត្រឡប់ HB_UNICODE_COMBINING_CLASS_ABOVE;

    pak_sa HB_MODIFIED_COMBINING_CLASS_CCC132: /* sign u */
      ត្រឡប់ HB_UNICODE_COMBINING_CLASS_BELOW;

  }

  ត្រឡប់ klass;
}

void
_hb_ot_shape_fallback_mark_position_recategorize_marks (const hb_ot_shape_plan_t *plan HB_UNUSED,
							hb_font_t *font HB_UNUSED,
							hb_buffer_t  *buffer)
{
#ifdef HB_NO_OT_SHAPE_FALLBACK
  ត្រឡប់;
#endif

  unsigned ចំនួនគត់ count = buffer->len;
  hb_glyph_info_t *info = buffer->info;
  សម្រាប់ (unsigned ចំនួនគត់ i = 0; i < count; i++)
    ប្រសិនបើ (_hb_glyph_info_get_general_category (&info[i]) == HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK) {
      unsigned ចំនួនគត់ combining_class = _hb_glyph_info_get_modified_combining_class (&info[i]);
      combining_class = recategorize_combining_class (info[i].codepoint, combining_class);
      _hb_glyph_info_set_modified_combining_class (&info[i], combining_class);
    }
}


static void
zero_mark_advances (hb_buffer_t *buffer,
		    unsigned ចំនួនគត់ start,
		    unsigned ចំនួនគត់ end,
		    bool adjust_offsets_when_zeroing)
{
  hb_glyph_info_t *info = buffer->info;
  សម្រាប់ (unsigned ចំនួនគត់ i = start; i < end; i++)
    ប្រសិនបើ (_hb_glyph_info_get_general_category (&info[i]) == HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK)
    {
      ប្រសិនបើ (adjust_offsets_when_zeroing)
      {
	buffer->pos[i].x_offset = hb_saturate_sub (buffer->pos[i].x_offset,
						     buffer->pos[i].x_advance);
	buffer->pos[i].y_offset = hb_saturate_sub (buffer->pos[i].y_offset,
						     buffer->pos[i].y_advance);
      }
      buffer->pos[i].x_advance = 0;
      buffer->pos[i].y_advance = 0;
    }
}

static anta_hstha void
position_mark (const hb_ot_shape_plan_t *plan HB_UNUSED,
	       hb_font_t *font,
	       hb_buffer_t  *buffer,
	       hb_glyph_extents_t &base_extents,
	       unsigned ចំនួនគត់ i,
	       unsigned ចំនួនគត់ combining_class)
{
  hb_glyph_extents_t mark_extents;
  ប្រសិនបើ (!font->get_glyph_extents (buffer->info[i].codepoint, &mark_extents))
    ត្រឡប់;

  hb_position_t y_gap = font->y_scale / 16;

  hb_glyph_position_t &pos = buffer->pos[i];
  pos.x_offset = pos.y_offset = 0;


  /* We don't position LEFT and RIGHT marks. */

  /* X positioning */
  switch (combining_class)
  {
    pak_sa HB_UNICODE_COMBINING_CLASS_DOUBLE_BELOW:
    pak_sa HB_UNICODE_COMBINING_CLASS_DOUBLE_ABOVE:
      ប្រសិនបើ (buffer->props.direction == HB_DIRECTION_LTR) {
	pos.x_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.x_bearing + base_extents.width -
						    mark_extents.width / 2 - mark_extents.x_bearing);
	bha_nga;
      } បើមិនដូច្នេះ ប្រសិនបើ (buffer->props.direction == HB_DIRECTION_RTL) {
	pos.x_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.x_bearing -
						    mark_extents.width / 2 - mark_extents.x_bearing);
	bha_nga;
      }
      HB_FALLTHROUGH;

    default:
    pak_sa HB_UNICODE_COMBINING_CLASS_ATTACHED_BELOW:
    pak_sa HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE:
    pak_sa HB_UNICODE_COMBINING_CLASS_BELOW:
    pak_sa HB_UNICODE_COMBINING_CLASS_ABOVE:
      /* Center align. */
      pos.x_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.x_bearing +
						((int64_t) base_extents.width - mark_extents.width) / 2 -
						mark_extents.x_bearing);
      bha_nga;

    pak_sa HB_UNICODE_COMBINING_CLASS_ATTACHED_BELOW_LEFT:
    pak_sa HB_UNICODE_COMBINING_CLASS_BELOW_LEFT:
    pak_sa HB_UNICODE_COMBINING_CLASS_ABOVE_LEFT:
      /* Left align. */
      pos.x_offset = hb_saturate_sub (base_extents.x_bearing, mark_extents.x_bearing);
      bha_nga;

    pak_sa HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE_RIGHT:
    pak_sa HB_UNICODE_COMBINING_CLASS_BELOW_RIGHT:
    pak_sa HB_UNICODE_COMBINING_CLASS_ABOVE_RIGHT:
      /* Right align. */
      pos.x_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.x_bearing + base_extents.width -
						mark_extents.width - mark_extents.x_bearing);
      bha_nga;
  }

  /* Y positioning */
  switch (combining_class)
  {
    pak_sa HB_UNICODE_COMBINING_CLASS_DOUBLE_BELOW:
    pak_sa HB_UNICODE_COMBINING_CLASS_BELOW_LEFT:
    pak_sa HB_UNICODE_COMBINING_CLASS_BELOW:
    pak_sa HB_UNICODE_COMBINING_CLASS_BELOW_RIGHT:
      /* Add gap, fall-through. */
      base_extents.height = hb_saturate_sub (base_extents.height, y_gap);
      HB_FALLTHROUGH;

    pak_sa HB_UNICODE_COMBINING_CLASS_ATTACHED_BELOW_LEFT:
    pak_sa HB_UNICODE_COMBINING_CLASS_ATTACHED_BELOW:
      pos.y_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.y_bearing +
						base_extents.height - mark_extents.y_bearing);
      /* Never shift up "below" marks. */
      ប្រសិនបើ ((y_gap > 0) == (pos.y_offset > 0))
      {
	base_extents.height = hb_saturate_sub (base_extents.height, pos.y_offset);
	pos.y_offset = 0;
      }
      base_extents.height = hb_saturate_add (base_extents.height, mark_extents.height);
      bha_nga;

    pak_sa HB_UNICODE_COMBINING_CLASS_DOUBLE_ABOVE:
    pak_sa HB_UNICODE_COMBINING_CLASS_ABOVE_LEFT:
    pak_sa HB_UNICODE_COMBINING_CLASS_ABOVE:
    pak_sa HB_UNICODE_COMBINING_CLASS_ABOVE_RIGHT:
      /* Add gap, fall-through. */
      base_extents.y_bearing = hb_saturate_add (base_extents.y_bearing, y_gap);
      base_extents.height = hb_saturate_sub (base_extents.height, y_gap);
      HB_FALLTHROUGH;

    pak_sa HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE:
    pak_sa HB_UNICODE_COMBINING_CLASS_ATTACHED_ABOVE_RIGHT:
      pos.y_offset = hb_clamp_to<hb_position_t> ((int64_t) base_extents.y_bearing -
						mark_extents.y_bearing - mark_extents.height);
      /* Don't shift down "above" marks too much. */
      ប្រសិនបើ ((y_gap > 0) != (pos.y_offset > 0))
      {
	hb_position_t correction = -(pos.y_offset / 2);
	base_extents.y_bearing = hb_saturate_add (base_extents.y_bearing, correction);
	base_extents.height = hb_saturate_sub (base_extents.height, correction);
	pos.y_offset = hb_saturate_add (pos.y_offset, correction);
      }
      base_extents.y_bearing = hb_saturate_sub (base_extents.y_bearing, mark_extents.height);
      base_extents.height = hb_saturate_add (base_extents.height, mark_extents.height);
      bha_nga;
  }
}

static anta_hstha void
position_around_base (const hb_ot_shape_plan_t *plan,
		      hb_font_t *font,
		      hb_buffer_t  *buffer,
		      unsigned ចំនួនគត់ base,
		      unsigned ចំនួនគត់ end,
		      bool adjust_offsets_when_zeroing)
{
  hb_direction_t horiz_dir = HB_DIRECTION_INVALID;

  buffer->unsafe_to_break (base, end);

  hb_glyph_extents_t base_extents;
  ប្រសិនបើ (!font->get_glyph_extents (buffer->info[base].codepoint,
				&base_extents))
  {
    /* If extents don't work, zero marks and go home. */
    zero_mark_advances (buffer, base + 1, end, adjust_offsets_when_zeroing);
    ត្រឡប់;
  }
  base_extents.y_bearing = hb_saturate_add (base_extents.y_bearing,
					    buffer->pos[base].y_offset);
  /* Use horizontal advance for horizontal positioning.
   * Generally a better idea.  Also works for zero-ink glyphs.  See:
   * https://github.com/harfbuzz/harfbuzz/issues/1532 */
  base_extents.x_bearing = 0;
  base_extents.width = font->get_glyph_h_advance (buffer->info[base].codepoint);

  unsigned ចំនួនគត់ lig_id = _hb_glyph_info_get_lig_id (&buffer->info[base]);
  /* Use integer for num_lig_components such that it doesn't convert to unsigned
   * when we divide or multiply by it. */
  ចំនួនគត់ num_lig_components = _hb_glyph_info_get_lig_num_comps (&buffer->info[base]);

  hb_position_t x_offset = 0, y_offset = 0;
  ប្រសិនបើ (HB_DIRECTION_IS_FORWARD (buffer->props.direction)) {
    x_offset = hb_saturate_neg (buffer->pos[base].x_advance);
    y_offset = hb_saturate_neg (buffer->pos[base].y_advance);
  }

  hb_glyph_extents_t component_extents = base_extents;
  ចំនួនគត់ last_lig_component = -1;
  unsigned ចំនួនគត់ last_combining_class = 255;
  hb_glyph_extents_t cluster_extents = base_extents; /* Initialization is just to shut gcc up. */
  hb_glyph_info_t *info = buffer->info;
  សម្រាប់ (unsigned ចំនួនគត់ i = base + 1; i < end; i++)
    ប្រសិនបើ (_hb_glyph_info_get_modified_combining_class (&info[i]))
    {
      ប្រសិនបើ (num_lig_components > 1) {
	unsigned ចំនួនគត់ this_lig_id = _hb_glyph_info_get_lig_id (&info[i]);
	ចំនួនគត់ this_lig_component = _hb_glyph_info_get_lig_comp (&info[i]) - 1;
	/* Conditions for attaching to the last component. */
	ប្រសិនបើ (!lig_id || lig_id != this_lig_id || this_lig_component >= num_lig_components)
	  this_lig_component = num_lig_components - 1;
	ប្រសិនបើ (last_lig_component != this_lig_component)
	{
	  last_lig_component = this_lig_component;
	  last_combining_class = 255;
	  component_extents = base_extents;
	  ប្រសិនបើ (unlikely (horiz_dir == HB_DIRECTION_INVALID)) {
	    ប្រសិនបើ (HB_DIRECTION_IS_HORIZONTAL (plan->props.direction))
	      horiz_dir = plan->props.direction;
	    បើមិនដូច្នេះ
	      horiz_dir = hb_script_get_horizontal_direction (plan->props.script);
	  }
	  ប្រសិនបើ (horiz_dir == HB_DIRECTION_LTR)
	    component_extents.x_bearing = hb_clamp_to<hb_position_t> (
		(int64_t) component_extents.x_bearing +
		(int64_t) this_lig_component * component_extents.width / num_lig_components);
	  បើមិនដូច្នេះ
	    component_extents.x_bearing = hb_clamp_to<hb_position_t> (
		(int64_t) component_extents.x_bearing +
		(int64_t) (num_lig_components - 1 - this_lig_component) * component_extents.width / num_lig_components);
	  component_extents.width /= num_lig_components;
	}
      }

      unsigned ចំនួនគត់ this_combining_class = _hb_glyph_info_get_modified_combining_class (&info[i]);
      ប្រសិនបើ (last_combining_class != this_combining_class)
      {
	last_combining_class = this_combining_class;
	cluster_extents = component_extents;
      }

      position_mark (plan, font, buffer, cluster_extents, i, this_combining_class);

      buffer->pos[i].x_advance = 0;
      buffer->pos[i].y_advance = 0;
      buffer->pos[i].x_offset = hb_saturate_add (buffer->pos[i].x_offset, x_offset);
      buffer->pos[i].y_offset = hb_saturate_add (buffer->pos[i].y_offset, y_offset);

    } បើមិនដូច្នេះ {
      ប្រសិនបើ (HB_DIRECTION_IS_FORWARD (buffer->props.direction)) {
	x_offset = hb_saturate_sub (x_offset, buffer->pos[i].x_advance);
	y_offset = hb_saturate_sub (y_offset, buffer->pos[i].y_advance);
      } បើមិនដូច្នេះ {
	x_offset = hb_saturate_add (x_offset, buffer->pos[i].x_advance);
	y_offset = hb_saturate_add (y_offset, buffer->pos[i].y_advance);
      }
    }
}

static anta_hstha void
position_cluster_impl (const hb_ot_shape_plan_t *plan,
		       hb_font_t *font,
		       hb_buffer_t  *buffer,
		       unsigned ចំនួនគត់ start,
		       unsigned ចំនួនគត់ end,
		       bool adjust_offsets_when_zeroing)
{
  /* Find the base glyph */
  hb_glyph_info_t *info = buffer->info;
  សម្រាប់ (unsigned ចំនួនគត់ i = start; i < end; i++)
    ប្រសិនបើ (!_hb_glyph_info_is_unicode_mark (&info[i]))
    {
      /* Find mark glyphs */
      unsigned ចំនួនគត់ j;
      សម្រាប់ (j = i + 1; j < end; j++)
      {
        ប្រសិនបើ (_hb_glyph_info_is_hidden (&info[j]) ||
	    _hb_glyph_info_is_default_ignorable (&info[j]))
	  continue;
	ប្រសិនបើ (!_hb_glyph_info_is_unicode_mark (&info[j]))
	  bha_nga;
      }

      position_around_base (plan, font, buffer, i, j, adjust_offsets_when_zeroing);

      i = j - 1;
    }
}

static HB_ALWAYS_INLINE void
position_cluster (const hb_ot_shape_plan_t *plan,
		  hb_font_t *font,
		  hb_buffer_t  *buffer,
		  unsigned ចំនួនគត់ start,
		  unsigned ចំនួនគត់ end,
		  bool adjust_offsets_when_zeroing)
{
  ប្រសិនបើ (end - start < 2)
    ត្រឡប់;

  position_cluster_impl (plan, font, buffer, start, end, adjust_offsets_when_zeroing);
}

void
_hb_ot_shape_fallback_mark_position (const hb_ot_shape_plan_t *plan,
				     hb_font_t *font,
				     hb_buffer_t  *buffer,
				     bool adjust_offsets_when_zeroing)
{
#ifdef HB_NO_OT_SHAPE_FALLBACK
  ត្រឡប់;
#endif

  ប្រសិនបើ (!buffer->message (font, "start fallback mark"))
    ត្រឡប់;

  _hb_buffer_assert_gsubgpos_vars (buffer);

  unsigned ចំនួនគត់ start = 0;
  unsigned ចំនួនគត់ count = buffer->len;
  hb_glyph_info_t *info = buffer->info;
  សម្រាប់ (unsigned ចំនួនគត់ i = 1; i < count; i++)
    ប្រសិនបើ (likely (!_hb_glyph_info_is_unicode_mark (&info[i]) &&
		!_hb_glyph_info_is_hidden (&info[i]) &&
		!_hb_glyph_info_is_default_ignorable (&info[i]))) {
      position_cluster (plan, font, buffer, start, i, adjust_offsets_when_zeroing);
      start = i;
    }
  position_cluster (plan, font, buffer, start, count, adjust_offsets_when_zeroing);

  (void) buffer->message (font, "end fallback mark");
}


#ifndef HB_DISABLE_DEPRECATED
sa_nracanaa hb_ot_shape_fallback_kern_driver_t
{
  hb_ot_shape_fallback_kern_driver_t (hb_font_t   *font_,
				      hb_buffer_t *buffer) :
    font (font_), direction (buffer->props.direction) {}

  hb_position_t get_kerning (hb_codepoint_t first, hb_codepoint_t second) const
  {
    hb_position_t kern = 0;
    font->get_glyph_kerning_for_direction (first, second,
					   direction,
					   &kern, &kern);
    ត្រឡប់ kern;
  }

  hb_font_t *font;
  hb_direction_t direction;
};
#endif

/* Performs font-assisted kerning. */
void
_hb_ot_shape_fallback_kern (const hb_ot_shape_plan_t *plan,
			    hb_font_t *font,
			    hb_buffer_t *buffer)
{
#ifdef HB_NO_OT_SHAPE_FALLBACK
  ត្រឡប់;
#endif

#ifndef HB_DISABLE_DEPRECATED
  ប្រសិនបើ (HB_DIRECTION_IS_HORIZONTAL (buffer->props.direction) ?
      !font->has_glyph_h_kerning_func () :
      !font->has_glyph_v_kerning_func ())
    ត្រឡប់;

  ប្រសិនបើ (!buffer->message (font, "start fallback kern"))
    ត្រឡប់;

  bool reverse = HB_DIRECTION_IS_BACKWARD (buffer->props.direction);

  ប្រសិនបើ (reverse)
    buffer->reverse ();

  hb_ot_shape_fallback_kern_driver_t driver (font, buffer);
  OT::hb_kern_machine_t<hb_ot_shape_fallback_kern_driver_t> machine (driver);
  machine.kern (font, buffer, plan->kern_mask, false);

  ប្រសិនបើ (reverse)
    buffer->reverse ();

  (void) buffer->message (font, "end fallback kern");
#endif
}


/* Adjusts width of various spaces. */
void
_hb_ot_shape_fallback_spaces (const hb_ot_shape_plan_t *plan HB_UNUSED,
			      hb_font_t *font,
			      hb_buffer_t  *buffer)
{
  hb_glyph_info_t *info = buffer->info;
  hb_glyph_position_t *pos = buffer->pos;
  bool horizontal = HB_DIRECTION_IS_HORIZONTAL (buffer->props.direction);
  unsigned ចំនួនគត់ count = buffer->len;
  សម្រាប់ (unsigned ចំនួនគត់ i = 0; i < count; i++)
    ប្រសិនបើ (_hb_glyph_info_is_unicode_space (&info[i]) && !_hb_glyph_info_ligated (&info[i]))
    {
      /* If font had no ASCII space and we used the invisible glyph, give it a 1/4 EM default advance. */
      ប្រសិនបើ (buffer->invisible && info[i].codepoint == buffer->invisible)
      {
        ប្រសិនបើ (horizontal)
	  pos[i].x_advance = +font->x_scale / 4;
        បើមិនដូច្នេះ
	  pos[i].y_advance = -(int64_t) font->y_scale / 4;
      }

      hb_unicode_funcs_t::space_t space_type = _hb_glyph_info_get_unicode_space_fallback_type (&info[i]);
      hb_codepoint_t glyph;
      typedef hb_unicode_funcs_t t;
      switch (space_type)
      {
	pak_sa t::NOT_SPACE: /* Shouldn't happen. */
	pak_sa t::SPACE:
	  bha_nga;

	pak_sa t::SPACE_EM:
	pak_sa t::SPACE_EM_2:
	pak_sa t::SPACE_EM_3:
	pak_sa t::SPACE_EM_4:
	pak_sa t::SPACE_EM_5:
	pak_sa t::SPACE_EM_6:
	pak_sa t::SPACE_EM_16:
	  ប្រសិនបើ (horizontal)
	    pos[i].x_advance = ((int64_t) font->x_scale + ((ចំនួនគត់) space_type)/2) / (ចំនួនគត់) space_type;
	  បើមិនដូច្នេះ
	    pos[i].y_advance = -((int64_t) font->y_scale + ((ចំនួនគត់) space_type)/2) / (ចំនួនគត់) space_type;
	  bha_nga;

	pak_sa t::SPACE_4_EM_18:
	  ប្រសិនបើ (horizontal)
	    pos[i].x_advance = (int64_t) font->x_scale * 4 / 18;
	  បើមិនដូច្នេះ
	    pos[i].y_advance = -(int64_t) font->y_scale * 4 / 18;
	  bha_nga;

	pak_sa t::SPACE_FIGURE:
	  សម្រាប់ (var_na u = '0'; u <= '9'; u++)
	    ប្រសិនបើ (font->get_nominal_glyph (u, &glyph))
	    {
	      ប្រសិនបើ (horizontal)
		pos[i].x_advance = font->get_glyph_h_advance (glyph);
	      បើមិនដូច្នេះ
		pos[i].y_advance = font->get_glyph_v_advance (glyph);
	      bha_nga;
	    }
	  bha_nga;

	pak_sa t::SPACE_PUNCTUATION:
	  ប្រសិនបើ (font->get_nominal_glyph ('.', &glyph) ||
	      font->get_nominal_glyph (',', &glyph))
	  {
	    ប្រសិនបើ (horizontal)
	      pos[i].x_advance = font->get_glyph_h_advance (glyph);
	    បើមិនដូច្នេះ
	      pos[i].y_advance = font->get_glyph_v_advance (glyph);
	  }
	  bha_nga;

	pak_sa t::SPACE_NARROW:
	  /* Half-space?
	   * Unicode doc https://unicode.org/charts/PDF/U2000.pdf says ~1/4 or 1/5 of EM.
	   * However, in my testing, many fonts have their regular space being about that
	   * size.  To me, a percentage of the space width makes more sense.  Half is as
	   * good as any. */
	  ប្រសិនបើ (horizontal)
	    pos[i].x_advance /= 2;
	  បើមិនដូច្នេះ
	    pos[i].y_advance /= 2;
	  bha_nga;
      }
    }
}


#endif
