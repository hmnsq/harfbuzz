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

#ଅନ୍ତର୍ଭାଵ "hb.hh"

#ifndef HB_NO_STYLE

#ଅନ୍ତର୍ଭାଵ "hb-ot-var-avar-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-var-fvar-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-stat-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-os2-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-head-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-post-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-face.hh"

/**
 * SECTION:hb-style
 * @title: hb-style
 * @short_description: Font Styles
 * @include: hb.h
 *
 * Functions for fetching style information from fonts.
 **/

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଦଶମିକ
_hb_angle_to_ratio (ଦଶମିକ a)
{
  ଫେରନ୍ତୁ tanf (a * -HB_PI / 180.f);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଦଶମିକ
_hb_ratio_to_angle (ଦଶମିକ r)
{
  ଫେରନ୍ତୁ atanf (r) * -180.f / HB_PI;
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
ଦଶମିକ
hb_style_get_ମୂଲ୍ୟ (hb_font_t *font, hb_style_tag_t style_tag)
{
  ଯଦି (unlikely (style_tag == HB_STYLE_TAG_SLANT_RATIO))
    ଫେରନ୍ତୁ _hb_angle_to_ratio (hb_style_get_ମୂଲ୍ୟ (font, HB_STYLE_TAG_SLANT_ANGLE));

  hb_face_t *face = font->face;

#ifndef HB_NO_VAR
  hb_ot_var_axis_info_t axis;
  ଯଦି (hb_ot_var_find_axis_info (face, style_tag, &axis))
  {
    ଯଦି (axis.axis_index < font->ଅଙ୍କ_coords) ଫେରନ୍ତୁ font->design_coords[axis.axis_index];
    /* If a face is variable, fvar's default_value is better than STAT records */
    ଫେରନ୍ତୁ axis.default_ମୂଲ୍ୟ;
  }
#endif

  ଯଦି (style_tag == HB_STYLE_TAG_OPTICAL_SIZE && font->ptem)
    ଫେରନ୍ତୁ font->ptem;

  /* STAT */
  ଦଶମିକ value;
  ଯଦି (face->table.STAT->get_ମୂଲ୍ୟ (style_tag, &value))
    ଫେରନ୍ତୁ value;

  ବାଛ ((unsigned) style_tag)
  {
  ସ୍ଥିତି HB_STYLE_TAG_ITALIC:
    ଫେରନ୍ତୁ face->table.OS2->is_italic () || face->table.head->is_italic () ? 1 : 0;
  ସ୍ଥିତି HB_STYLE_TAG_OPTICAL_SIZE:
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା lower, design, upper;
    ଫେରନ୍ତୁ face->table.OS2->v5 ().get_optical_size (&lower, &upper)
	   ? (ଦଶମିକ) (lower + upper) / 2.f
	   : hb_ot_layout_get_size_params (face, &design, nullptr, nullptr, nullptr, nullptr)
	   ? design / 10.f
	   : 12.f;
  }
  ସ୍ଥିତି HB_STYLE_TAG_SLANT_ANGLE:
  {
    ଦଶମିକ angle = face->table.post->table->italicAngle.to_float ();

    ଯଦି (font->slant)
      angle = _hb_ratio_to_angle (font->slant + _hb_angle_to_ratio (angle));

    ଫେରନ୍ତୁ angle;
  }
  ସ୍ଥିତି HB_STYLE_TAG_WIDTH:
    ଫେରନ୍ତୁ face->table.OS2->has_data ()
	   ? face->table.OS2->get_width ()
	   : (face->table.head->is_condensed () ? 75 :
	      face->table.head->is_expanded () ? 125 :
	      100);
  ସ୍ଥିତି HB_STYLE_TAG_WEIGHT:
    ଫେରନ୍ତୁ face->table.OS2->has_data ()
	   ? face->table.OS2->usWeightClass
	   : (face->table.head->is_bold () ? 700 : 400);
  ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
    ଫେରନ୍ତୁ 0;
  }
}

#endif
