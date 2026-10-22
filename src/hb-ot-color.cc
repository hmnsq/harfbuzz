/*
 * Copyright © 2016  Google, Inc.
 * Copyright © 2018  Ebrahim Byagowi
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
 * Google Author(s): Sascha Brawer, Behdad Esfahbod
 */

#అన్తర్భావ "hb.hh"

#నిర్వచితం_కాకపోతే HB_NO_COLOR

#అన్తర్భావ "hb-ot.h"

#అన్తర్భావ "OT/Color/CBDT/CBDT.hh"
#అన్తర్భావ "OT/Color/COLR/COLR.hh"
#అన్తర్భావ "OT/Color/CPAL/CPAL.hh"
#అన్తర్భావ "OT/Color/sbix/sbix.hh"
#నిర్వచితం_కాకపోతే HB_NO_SVG
#అన్తర్భావ "OT/Color/svg/svg.hh"
#అయితే_ముగింపు


/**
 * SECTION:hb-ot-color
 * @title: hb-ot-color
 * @short_description: OpenType Color Fonts
 * @include: hb-ot.h
 *
 * Functions for fetching color-font information from OpenType font faces.
 *
 * HarfBuzz supports `COLR`/`CPAL`, `sbix`, `CBDT`, and `SVG` color fonts.
 **/


/*
 * CPAL
 */


/**
 * hb_ot_color_has_palettes:
 * @face: #hb_face_t to work upon
 *
 * Tests whether a face includes a `CPAL` color-palette table.
 *
 * Return value: `true` if data found, `false` otherwise
 *
 * Since: 2.1.0
 */
hb_bool_t
hb_ot_color_has_palettes (hb_face_t *face)
{
  తిరిగి face->పట్టిక.CPAL->has_దత్తాంశం ();
}

/**
 * hb_ot_color_palette_get_count:
 * @face: #hb_face_t to work upon
 *
 * Fetches the number of color palettes in a face.
 *
 * Return value: the number of palettes found
 *
 * Since: 2.1.0
 */
గుర్తులేని పూర్ణాంకం
hb_ot_color_palette_get_లెక్క (hb_face_t *face)
{
  తిరిగి face->పట్టిక.CPAL->get_palette_లెక్క ();
}

/**
 * hb_ot_color_palette_get_name_id:
 * @face: #hb_face_t to work upon
 * @palette_index: The index of the color palette
 *
 * Fetches the `name` table Name ID that provides display names for
 * a `CPAL` color palette.
 *
 * Palette display names can be generic (e.g., "Default") or provide
 * specific, themed names (e.g., "Spring", "Summer", "Fall", and "Winter").
 *
 * Return value: the Named ID found for the palette.
 * If the requested palette has no name the result is #HB_OT_NAME_ID_INVALID.
 *
 * Since: 2.1.0
 */
hb_ot_name_id_t
hb_ot_color_palette_get_name_id (hb_face_t *face,
				 గుర్తులేని పూర్ణాంకం palette_సూచీ)
{
  తిరిగి face->పట్టిక.CPAL->get_palette_name_id (palette_సూచీ);
}

/**
 * hb_ot_color_palette_color_get_name_id:
 * @face: #hb_face_t to work upon
 * @color_index: The index of the color
 *
 * Fetches the `name` table Name ID that provides display names for
 * the specified color in a face's `CPAL` color palette.
 *
 * Display names can be generic (e.g., "Background") or specific
 * (e.g., "Eye color").
 *
 * Return value: the Name ID found for the color.
 *
 * Since: 2.1.0
 */
hb_ot_name_id_t
hb_ot_color_palette_color_get_name_id (hb_face_t *face,
				       గుర్తులేని పూర్ణాంకం color_సూచీ)
{
  తిరిగి face->పట్టిక.CPAL->get_color_name_id (color_సూచీ);
}

/**
 * hb_ot_color_palette_get_flags:
 * @face: #hb_face_t to work upon
 * @palette_index: The index of the color palette
 *
 * Fetches the flags defined for a color palette.
 *
 * Return value: the #hb_ot_color_palette_flags_t of the requested color palette
 *
 * Since: 2.1.0
 */
hb_ot_color_palette_flags_t
hb_ot_color_palette_get_flags (hb_face_t *face,
			       గుర్తులేని పూర్ణాంకం palette_సూచీ)
{
  తిరిగి face->పట్టిక.CPAL->get_palette_flags (palette_సూచీ);
}

/**
 * hb_ot_color_palette_get_colors:
 * @face: #hb_face_t to work upon
 * @palette_index: the index of the color palette to query
 * @start_offset: offset of the first color to retrieve
 * @color_count: (inout) (optional): Input = the maximum number of colors to return;
 *               Output = the actual number of colors returned (may be zero)
 * @colors: (out) (array length=color_count) (nullable): The array of #hb_color_t records found
 *
 * Fetches a list of the colors in a color palette.
 *
 * After calling this function, @colors will be filled with the palette
 * colors. If @colors is NULL, the function will just return the number
 * of total colors without storing any actual colors; this can be used
 * for allocating a buffer of suitable size before calling
 * hb_ot_color_palette_get_colors() a second time.
 *
 * The RGBA values in the palette are unpremultiplied. See the
 * OpenType spec [CPAL](https://learn.microsoft.com/en-us/typography/opentype/spec/cpal)
 * section for details.
 *
 * Return value: the total number of colors in the palette
 *
 * Since: 2.1.0
 */
గుర్తులేని పూర్ణాంకం
hb_ot_color_palette_get_colors (hb_face_t     *face,
				గుర్తులేని పూర్ణాంకం   palette_సూచీ,
				గుర్తులేని పూర్ణాంకం   ప్రారంభం_స్థానభ్రంశం,
				గుర్తులేని పూర్ణాంకం  *colors_లెక్క  /* IN/OUT.  May be NULL. */,
				hb_color_t    *colors        /* OUT.     May be NULL. */)
{
  తిరిగి face->పట్టిక.CPAL->get_palette_colors (palette_సూచీ, ప్రారంభం_స్థానభ్రంశం, colors_లెక్క, colors);
}


/*
 * COLR
 */

/**
 * hb_ot_color_has_layers:
 * @face: #hb_face_t to work upon
 *
 * Tests whether a face includes a `COLR` table
 * with data according to COLRv0.
 *
 * Return value: `true` if data found, `false` otherwise
 *
 * Since: 2.1.0
 */
hb_bool_t
hb_ot_color_has_layers (hb_face_t *face)
{
  తిరిగి face->పట్టిక.COLR->colr->has_v0_దత్తాంశం ();
}

/**
 * hb_ot_color_has_paint:
 * @face: #hb_face_t to work upon
 *
 * Tests where a face includes a `COLR` table
 * with data according to COLRv1.
 *
 * Return value: `true` if data found, `false` otherwise
 *
 * Since: 7.0.0
 */
hb_bool_t
hb_ot_color_has_paint (hb_face_t *face)
{
  తిరిగి face->పట్టిక.COLR->colr->has_v1_దత్తాంశం ();
}

/**
 * hb_ot_color_glyph_has_paint:
 * @face: #hb_face_t to work upon
 * @glyph: The glyph index to query
 *
 * Tests where a face includes COLRv1 paint
 * data for @glyph.
 *
 * Return value: `true` if data found, `false` otherwise
 *
 * Since: 7.0.0
 */
hb_bool_t
hb_ot_color_glyph_has_paint (hb_face_t      *face,
                             hb_codepoint_t  glyph)
{
  తిరిగి face->పట్టిక.COLR->colr->has_paint_for_glyph (glyph);
}

/**
 * hb_ot_color_glyph_get_layers:
 * @face: #hb_face_t to work upon
 * @glyph: The glyph index to query
 * @start_offset: offset of the first layer to retrieve
 * @layer_count: (inout) (optional): Input = the maximum number of layers to return;
 *         Output = the actual number of layers returned (may be zero)
 * @layers: (out) (array length=layer_count) (nullable): The array of layers found
 *
 * Fetches a list of all color layers for the specified glyph index in the specified
 * face. The list returned will begin at the offset provided.
 *
 * Return value: Total number of layers available for the glyph index queried
 *
 * Since: 2.1.0
 */
గుర్తులేని పూర్ణాంకం
hb_ot_color_glyph_get_layers (hb_face_t           *face,
			      hb_codepoint_t       glyph,
			      గుర్తులేని పూర్ణాంకం         ప్రారంభం_స్థానభ్రంశం,
			      గుర్తులేని పూర్ణాంకం        *layer_లెక్క, /* IN/OUT.  May be NULL. */
			      hb_ot_color_layer_t *layers /* OUT.     May be NULL. */)
{
  తిరిగి face->పట్టిక.COLR->colr->get_glyph_layers (glyph, ప్రారంభం_స్థానభ్రంశం, layer_లెక్క, layers);
}


/*
 * SVG
 */

#నిర్వచితం_కాకపోతే HB_NO_SVG
/**
 * hb_ot_color_has_svg:
 * @face: #hb_face_t to work upon.
 *
 * Tests whether a face includes any `SVG` glyph images.
 *
 * Return value: `true` if data found, `false` otherwise.
 *
 * Since: 2.1.0
 */
hb_bool_t
hb_ot_color_has_svg (hb_face_t *face)
{
  తిరిగి face->పట్టిక.SVG->has_దత్తాంశం ();
}

/**
 * hb_ot_color_get_svg_document_count:
 * @face: #hb_face_t to work upon.
 *
 * Gets the number of SVG documents in the face `SVG` table.
 *
 * Return value: number of SVG documents in the face.
 *
 * Since: 12.1.0
 */
గుర్తులేని పూర్ణాంకం
hb_ot_color_get_svg_document_లెక్క (hb_face_t *face)
{
  తిరిగి face->పట్టిక.SVG->get_document_లెక్క ();
}

/**
 * hb_ot_color_glyph_get_svg_document_index:
 * @face: #hb_face_t to work upon.
 * @glyph: glyph ID to query.
 * @svg_document_index: (out) (nullable): output SVG document index.
 *
 * Gets the `SVG`-table document index associated with a glyph.
 *
 * Return value: `true` if @glyph maps to an SVG document, `false` otherwise.
 *
 * Since: 12.1.0
 */
hb_bool_t
hb_ot_color_glyph_get_svg_document_సూచీ (hb_face_t      *face,
                                          hb_codepoint_t  glyph,
                                          గుర్తులేని పూర్ణాంకం   *svg_document_సూచీ)
{
  గుర్తులేని doc_సూచీ = 0;
  hb_bool_t ret = face->పట్టిక.SVG->get_glyph_document_సూచీ (glyph, &doc_సూచీ);
  ఒకవేళ (ret && svg_document_సూచీ)
    *svg_document_సూచీ = doc_సూచీ;
  తిరిగి ret;
}

/**
 * hb_ot_color_get_svg_document_glyph_range:
 * @face: #hb_face_t to work upon.
 * @svg_document_index: SVG document index.
 * @start_glyph_id: (out) (nullable): output start glyph ID.
 * @end_glyph_id: (out) (nullable): output end glyph ID.
 *
 * Gets the glyph range covered by an `SVG`-table document index.
 *
 * Return value: `true` if @svg_document_index is valid, `false` otherwise.
 *
 * Since: 13.0.0
 */
hb_bool_t
hb_ot_color_get_svg_document_glyph_range (hb_face_t      *face,
                                          గుర్తులేని పూర్ణాంకం    svg_document_సూచీ,
                                          hb_codepoint_t *ప్రారంభం_glyph_id,
                                          hb_codepoint_t *చివర_glyph_id)
{
  తిరిగి face->పట్టిక.SVG->get_document_glyph_range (svg_document_సూచీ,
                                                    ప్రారంభం_glyph_id,
                                                    చివర_glyph_id);
}

/**
 * hb_ot_color_glyph_reference_svg:
 * @face: #hb_face_t to work upon
 * @glyph: a svg glyph index
 *
 * Fetches the SVG document for a glyph. The blob may be either plain text or gzip-encoded.
 *
 * If the glyph has no SVG document, the singleton empty blob is returned.
 *
 * Return value: (transfer full): An #hb_blob_t containing the SVG document of the glyph, if available
 *
 * Since: 2.1.0
 */
hb_blob_t *
hb_ot_color_glyph_reference_svg (hb_face_t *face, hb_codepoint_t glyph)
{
  తిరిగి face->పట్టిక.SVG->reference_blob_for_glyph (glyph);
}
#అయితే_ముగింపు


/*
 * PNG: CBDT or sbix
 */

/**
 * hb_ot_color_has_png:
 * @face: #hb_face_t to work upon
 *
 * Tests whether a face has PNG glyph images (either in `CBDT` or `sbix` tables).
 *
 * Return value: `true` if data found, `false` otherwise
 *
 * Since: 2.1.0
 */
hb_bool_t
hb_ot_color_has_png (hb_face_t *face)
{
  తిరిగి face->పట్టిక.CBDT->has_దత్తాంశం () || face->పట్టిక.sbix->has_దత్తాంశం ();
}

/**
 * hb_ot_color_glyph_reference_png:
 * @font: #hb_font_t to work upon
 * @glyph: a glyph index
 *
 * Fetches the PNG image for a glyph. This function takes a font object, not a face object,
 * as input. To get an optimally sized PNG blob, the PPEM values must be set on the @font
 * object. If PPEM is unset, the blob returned will be the largest PNG available.
 *
 * If the glyph has no PNG image, the singleton empty blob is returned.
 *
 * Return value: (transfer full): An #hb_blob_t containing the PNG image for the glyph, if available
 *
 * Since: 2.1.0
 */
hb_blob_t *
hb_ot_color_glyph_reference_png (hb_font_t *font, hb_codepoint_t  glyph)
{
  hb_blob_t *blob = hb_blob_get_ఖాళీలు ();

  ఒకవేళ (font->face->పట్టిక.sbix->has_దత్తాంశం ())
    blob = font->face->పట్టిక.sbix->reference_png (font, glyph, శూన్యసూచిక, శూన్యసూచిక, శూన్యసూచిక);

  ఒకవేళ (!blob->నిడివి && font->face->పట్టిక.CBDT->has_దత్తాంశం ())
    blob = font->face->పట్టిక.CBDT->reference_png (font, glyph);

  తిరిగి blob;
}


#అయితే_ముగింపు
