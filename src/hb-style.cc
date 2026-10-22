/*
 * Copyright © 2019  Ebrahim Byagowi
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
 */

#अन्तर्भाव "hb.hh"

#ifndef HB_NO_STYLE

#अन्तर्भाव "hb-ot-var-avar-table.hh"
#अन्तर्भाव "hb-ot-var-fvar-table.hh"
#अन्तर्भाव "hb-ot-stat-table.hh"
#अन्तर्भाव "hb-ot-os2-table.hh"
#अन्तर्भाव "hb-ot-head-table.hh"
#अन्तर्भाव "hb-ot-post-table.hh"
#अन्तर्भाव "hb-ot-face.hh"

/**
 * SECTION:hb-style
 * @title: hb-style
 * @short_description: Font Styles
 * @include: hb.h
 *
 * Functions for fetching style information from fonts.
 **/

स्थिर अन्तःस्थ भिन्न
_hb_angle_to_ratio (भिन्न a)
{
  फर्काऊ tanf (a * -HB_PI / 180.f);
}

स्थिर अन्तःस्थ भिन्न
_hb_ratio_to_angle (भिन्न r)
{
  फर्काऊ atanf (r) * -180.f / HB_PI;
}

/**
 * hb_style_get_value:
 * @font: a #hb_font_t object.
 * @style_tag: a style tag.
 *
 * Searches variation axes of a #hb_font_t object for a specific axis first,
 * if not set, first tries to get default style values in `STAT` table
 * then tries to polyfill from different tables of the font.
 *
 * Returns: Corresponding axis or default value to a style tag.
 *
 * Since: 3.0.0
 **/
भिन्न
hb_style_get_मान (hb_font_t *font, hb_style_tag_t style_tag)
{
  यदि (unlikely (style_tag == HB_STYLE_TAG_SLANT_RATIO))
    फर्काऊ _hb_angle_to_ratio (hb_style_get_मान (font, HB_STYLE_TAG_SLANT_ANGLE));

  hb_face_t *face = font->face;

#ifndef HB_NO_VAR
  hb_ot_var_axis_info_t axis;
  यदि (hb_ot_var_find_axis_info (face, style_tag, &axis))
  {
    यदि (axis.axis_index < font->अङ्क_coords) फर्काऊ font->design_coords[axis.axis_index];
    /* If a face is variable, fvar's default_value is better than STAT records */
    फर्काऊ axis.default_मान;
  }
#endif

  यदि (style_tag == HB_STYLE_TAG_OPTICAL_SIZE && font->ptem)
    फर्काऊ font->ptem;

  /* STAT */
  भिन्न value;
  यदि (face->table.STAT->get_मान (style_tag, &value))
    फर्काऊ value;

  विकल्प ((अचिन्हित) style_tag)
  {
  पक्ष HB_STYLE_TAG_ITALIC:
    फर्काऊ face->table.OS2->is_italic () || face->table.head->is_italic () ? 1 : 0;
  पक्ष HB_STYLE_TAG_OPTICAL_SIZE:
  {
    अचिन्हित पूर्णाङ्क lower, design, upper;
    फर्काऊ face->table.OS2->v5 ().get_optical_size (&lower, &upper)
	   ? (भिन्न) (lower + upper) / 2.f
	   : hb_ot_layout_get_size_params (face, &design, nullptr, nullptr, nullptr, nullptr)
	   ? design / 10.f
	   : 12.f;
  }
  पक्ष HB_STYLE_TAG_SLANT_ANGLE:
  {
    भिन्न angle = face->table.post->table->italicAngle.to_float ();

    यदि (font->slant)
      angle = _hb_ratio_to_angle (font->slant + _hb_angle_to_ratio (angle));

    फर्काऊ angle;
  }
  पक्ष HB_STYLE_TAG_WIDTH:
    फर्काऊ face->table.OS2->has_data ()
	   ? face->table.OS2->get_width ()
	   : (face->table.head->is_condensed () ? 75 :
	      face->table.head->is_expanded () ? 125 :
	      100);
  पक्ष HB_STYLE_TAG_WEIGHT:
    फर्काऊ face->table.OS2->has_data ()
	   ? face->table.OS2->usWeightClass
	   : (face->table.head->is_bold () ? 700 : 400);
  स्वभाव:
    फर्काऊ 0;
  }
}

#endif
