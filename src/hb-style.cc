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

#అన్తర్భావ "hb.hh"

#నిర్వచితం_కాకపోతే HB_NO_STYLE

#అన్తర్భావ "hb-ot-var-avar-table.hh"
#అన్తర్భావ "hb-ot-var-fvar-table.hh"
#అన్తర్భావ "hb-ot-stat-table.hh"
#అన్తర్భావ "hb-ot-os2-table.hh"
#అన్తర్భావ "hb-ot-head-table.hh"
#అన్తర్భావ "hb-ot-post-table.hh"
#అన్తర్భావ "hb-ot-face.hh"

/**
 * SECTION:hb-style
 * @title: hb-style
 * @short_description: Font Styles
 * @include: hb.h
 *
 * Functions for fetching style information from fonts.
 **/

స్థిర అంతఃవరుస దశాంశం
hb_angle_గమ్యం_నిష్పత్తి (దశాంశం a)
{
  తిరిగి tanf (a * -HB_PI / 180.f);
}

స్థిర అంతఃవరుస దశాంశం
hb_నిష్పత్తి_గమ్యం_angle (దశాంశం r)
{
  తిరిగి atanf (r) * -180.f / HB_PI;
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
దశాంశం
hb_style_get_విలువ (hb_font_t *font, hb_style_ట్యాగ్_t style_ట్యాగ్)
{
  ఒకవేళ (unlikely (style_ట్యాగ్ == HB_STYLE_ట్యాగ్_SLANT_నిష్పత్తి))
    తిరిగి hb_angle_గమ్యం_నిష్పత్తి (hb_style_get_విలువ (font, HB_STYLE_ట్యాగ్_SLANT_ANGLE));

  hb_face_t *face = font->face;

#నిర్వచితం_కాకపోతే HB_NO_VAR
  hb_ot_var_axis_info_t axis;
  ఒకవేళ (hb_ot_var_కనుగొను_axis_info (face, style_ట్యాగ్, &axis))
  {
    ఒకవేళ (axis.axis_సూచీ < font->అంకె_coords) తిరిగి font->design_coords[axis.axis_సూచీ];
    /* If a face is variable, fvar's default_value is better than STAT records */
    తిరిగి axis.default_విలువ;
  }
#అయితే_ముగింపు

  ఒకవేళ (style_ట్యాగ్ == HB_STYLE_ట్యాగ్_OPTICAL_కొలత && font->ptem)
    తిరిగి font->ptem;

  /* STAT */
  దశాంశం value;
  ఒకవేళ (face->పట్టిక.STAT->get_విలువ (style_ట్యాగ్, &value))
    తిరిగి value;

  ఎంపిక ((గుర్తులేని) style_ట్యాగ్)
  {
  స్థితి HB_STYLE_ట్యాగ్_ITALIC:
    తిరిగి face->పట్టిక.OS2->is_italic () || face->పట్టిక.తల_2->is_italic () ? 1 : 0;
  స్థితి HB_STYLE_ట్యాగ్_OPTICAL_కొలత:
  {
    గుర్తులేని పూర్ణాంకం క్రింది, design, పై;
    తిరిగి face->పట్టిక.OS2->v5 ().get_optical_కొలత (&క్రింది, &పై)
	   ? (దశాంశం) (క్రింది + పై) / 2.f
	   : hb_ot_layout_get_కొలత_params (face, &design, శూన్యసూచిక, శూన్యసూచిక, శూన్యసూచిక, శూన్యసూచిక)
	   ? design / 10.f
	   : 12.f;
  }
  స్థితి HB_STYLE_ట్యాగ్_SLANT_ANGLE:
  {
    దశాంశం angle = face->పట్టిక.post->పట్టిక->italicAngle.గమ్యం_float ();

    ఒకవేళ (font->slant)
      angle = hb_నిష్పత్తి_గమ్యం_angle (font->slant + hb_angle_గమ్యం_నిష్పత్తి (angle));

    తిరిగి angle;
  }
  స్థితి HB_STYLE_ట్యాగ్_వెడల్పు:
    తిరిగి face->పట్టిక.OS2->has_దత్తాంశం ()
	   ? face->పట్టిక.OS2->get_వెడల్పు ()
	   : (face->పట్టిక.తల_2->is_condensed () ? 75 :
	      face->పట్టిక.తల_2->is_expanded () ? 125 :
	      100);
  స్థితి HB_STYLE_ట్యాగ్_బరువు:
    తిరిగి face->పట్టిక.OS2->has_దత్తాంశం ()
	   ? face->పట్టిక.OS2->us_బరువు_Class
	   : (face->పట్టిక.తల_2->is_bold () ? 700 : 400);
  అప్రమేయ:
    తిరిగి 0;
  }
}

#అయితే_ముగింపు
