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

स्थिर ओळीत दशांश
_hb_angle_to_ratio (दशांश a)
{
  परत tanf (a * -HB_PI / 180.f);
}

स्थिर ओळीत दशांश
_hb_ratio_to_angle (दशांश r)
{
  परत atanf (r) * -180.f / HB_PI;
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
दशांश
hb_style_get_मूल्य (hb_font_t *font, hb_style_tag_t style_tag)
{
  जर (unlikely (style_tag == HB_STYLE_TAG_SLANT_RATIO))
    परत _hb_angle_to_ratio (hb_style_get_मूल्य (font, HB_STYLE_TAG_SLANT_ANGLE));

  hb_face_t *face = font->face;

#ifndef HB_NO_VAR
  hb_ot_var_axis_info_t axis;
  जर (hb_ot_var_find_axis_info (face, style_tag, &axis))
  {
    जर (axis.axis_index < font->अंक_coords) परत font->design_coords[axis.axis_index];
    /* If a face is variable, fvar's default_value is better than STAT records */
    परत axis.default_मूल्य;
  }
#endif

  जर (style_tag == HB_STYLE_TAG_OPTICAL_SIZE && font->ptem)
    परत font->ptem;

  /* STAT */
  दशांश value;
  जर (face->table.STAT->get_मूल्य (style_tag, &value))
    परत value;

  निवड ((अचिन्हित) style_tag)
  {
  स्थिती HB_STYLE_TAG_ITALIC:
    परत face->table.OS2->is_italic () || face->table.head->is_italic () ? 1 : 0;
  स्थिती HB_STYLE_TAG_OPTICAL_SIZE:
  {
    अचिन्हित पूर्णांक lower, design, upper;
    परत face->table.OS2->v5 ().get_optical_size (&lower, &upper)
	   ? (दशांश) (lower + upper) / 2.f
	   : hb_ot_layout_get_size_params (face, &design, nullptr, nullptr, nullptr, nullptr)
	   ? design / 10.f
	   : 12.f;
  }
  स्थिती HB_STYLE_TAG_SLANT_ANGLE:
  {
    दशांश angle = face->table.post->table->italicAngle.to_float ();

    जर (font->slant)
      angle = _hb_ratio_to_angle (font->slant + _hb_angle_to_ratio (angle));

    परत angle;
  }
  स्थिती HB_STYLE_TAG_WIDTH:
    परत face->table.OS2->has_data ()
	   ? face->table.OS2->get_width ()
	   : (face->table.head->is_condensed () ? 75 :
	      face->table.head->is_expanded () ? 125 :
	      100);
  स्थिती HB_STYLE_TAG_WEIGHT:
    परत face->table.OS2->has_data ()
	   ? face->table.OS2->usWeightClass
	   : (face->table.head->is_bold () ? 700 : 400);
  पूर्वनिर्धारित:
    परत 0;
  }
}

#endif
