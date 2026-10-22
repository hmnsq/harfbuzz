/*
 * Copyright © 1998-2004  David Turner and Werner Lemberg
 * Copyright © 2006  Behdad Esfahbod
 * Copyright © 2007,2008,2009  Red Hat, Inc.
 * Copyright © 2012,2013  Google, Inc.
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

#అన్తర్భావ "hb.hh"

#నిర్వచితం_కాకపోతే HB_NO_OT_LAYOUT

#నిర్వచితమైతే HB_NO_OT_ట్యాగ్
#దోషం "Cannot compile hb-ot-layout.cc with HB_NO_OT_TAG."
#అయితే_ముగింపు

#అన్తర్భావ "hb-open-type.hh"
#అన్తర్భావ "hb-ot-layout.hh"
#అన్తర్భావ "hb-ot-face.hh"
#అన్తర్భావ "hb-ot-map.hh"
#అన్తర్భావ "hb-map.hh"

#అన్తర్భావ "hb-ot-kern-table.hh"
#అన్తర్భావ "hb-ot-layout-gdef-table.hh"
#అన్తర్భావ "hb-ot-layout-gsub-table.hh"
#అన్తర్భావ "hb-ot-layout-gpos-table.hh"
#అన్తర్భావ "hb-ot-layout-base-table.hh"
#అన్తర్భావ "hb-ot-layout-jstf-table.hh" // Just so we compile it; unused otherwise.
#అన్తర్భావ "hb-ot-name-table.hh"
#అన్తర్భావ "hb-ot-os2-table.hh"

#అన్తర్భావ "hb-aat-layout-morx-table.hh"
#అన్తర్భావ "hb-aat-layout-opbd-table.hh" // Just so we compile it; unused otherwise.

ఉపయోగించి OT::Layout::GSUB;
ఉపయోగించి OT::Layout::GPOS;

/**
 * SECTION:hb-ot-layout
 * @title: hb-ot-layout
 * @short_description: OpenType Layout
 * @include: hb-ot.h
 *
 * Functions for querying OpenType Layout features in the font face.
 * See the [OpenType specification](http://www.microsoft.com/typography/otspec/)
 * for details.
 **/


/*
 * kern
 */

#నిర్వచితం_కాకపోతే HB_NO_OT_KERN
/**
 * hb_ot_layout_has_kerning:
 * @face: The #hb_face_t to work on
 *
 * Tests whether a face includes any kerning data in the 'kern' table.
 * Does NOT test for kerning lookups in the GPOS table.
 *
 * Return value: `true` if data found, `false` otherwise
 *
 **/
తార్కిక
hb_ot_layout_has_kerning (hb_face_t *face)
{
  తిరిగి face->పట్టిక.kern->పట్టిక->has_దత్తాంశం ();
}

/**
 * hb_ot_layout_has_machine_kerning:
 * @face: The #hb_face_t to work on
 *
 * Tests whether a face includes any state-machine kerning in the 'kern' table.
 * Does NOT examine the GPOS table.
 *
 * Return value: `true` if data found, `false` otherwise
 *
 **/
తార్కిక
hb_ot_layout_has_machine_kerning (hb_face_t *face)
{
  తిరిగి face->పట్టిక.kern->పట్టిక->has_state_machine ();
}

/**
 * hb_ot_layout_has_cross_kerning:
 * @face: The #hb_face_t to work on
 *
 * Tests whether a face has any cross-stream kerning (i.e., kerns
 * that make adjustments perpendicular to the direction of the text
 * flow: Y adjustments in horizontal text or X adjustments in
 * vertical text) in the 'kern' table.
 *
 * Does NOT examine the GPOS table.
 *
 * Return value: `true` is data found, `false` otherwise
 *
 **/
తార్కిక
hb_ot_layout_has_cross_kerning (hb_face_t *face)
{
  తిరిగి face->పట్టిక.kern->పట్టిక->has_cross_ప్రవాహం ();
}

శూన్య
hb_ot_layout_kern (స్థిరరాశి hb_ot_shape_plan_t *plan,
		   hb_font_t *font,
		   hb_బఫరు_t  *బఫరు)
{
  స్వయంచాలక &accel = *font->face->పట్టిక.kern;
  hb_blob_t *blob = accel.get_blob ();

  AAT::hb_aat_apply_context_t c (plan, font, బఫరు, blob);

  ఒకవేళ (!బఫరు->సందేశం (font, "start table kern")) తిరిగి;
  c.బఫరు_glyph_సమితి = accel.scratch.create_బఫరు_glyph_సమితి ();
  accel.apply (&c);
  accel.scratch.destroy_బఫరు_glyph_సమితి (c.బఫరు_glyph_సమితి);
  (శూన్య) బఫరు->సందేశం (font, "end table kern");
}
#అయితే_ముగింపు


/*
 * GDEF
 */

తార్కిక
OT::GDEF::is_blocklisted (hb_blob_t *blob,
			  hb_face_t *face) స్థిరరాశి
{
#నిర్వచితమైతే HB_NO_OT_LAYOUT_BLOCKLIST
  తిరిగి అబద్ధం;
#అయితే_ముగింపు
  /* The ugly business of blocklisting individual fonts' tables happen here!
   * See this thread for why we finally had to bend in and do this:
   * https://lists.freedesktop.org/archives/harfbuzz/2016-February/005489.html
   *
   * In certain versions of Times New Roman Italic and Bold Italic,
   * ASCII double quotation mark U+0022 has wrong glyph class 3 (mark)
   * in GDEF.  Many versions of Tahoma have bad GDEF tables that
   * incorrectly classify some spacing marks such as certain IPA
   * symbols as glyph class 3. So do older versions of Microsoft
   * Himalaya, and the version of Cantarell shipped by Ubuntu 16.04.
   *
   * Nuke the GDEF tables of to avoid unwanted width-zeroing.
   *
   * See https://bugzilla.mozilla.org/show_bug.cgi?id=1279925
   *     https://bugzilla.mozilla.org/show_bug.cgi?id=1279693
   *     https://bugzilla.mozilla.org/show_bug.cgi?id=1279875
   */
  ఎంపిక HB_CODEPOINT_ENCODE3(blob->నిడివి,
			      face->పట్టిక.GSUB->పట్టిక.get_నిడివి (),
			      face->పట్టిక.GPOS->పట్టిక.get_నిడివి ())
  {
    /* sha1sum:c5ee92f0bca4bfb7d06c4d03e8cf9f9cf75d2e8a Windows 7? timesi.ttf */
    స్థితి HB_CODEPOINT_ENCODE3 (442, 2874, 42038):
    /* sha1sum:37fc8c16a0894ab7b749e35579856c73c840867b Windows 7? timesbi.ttf */
    స్థితి HB_CODEPOINT_ENCODE3 (430, 2874, 40662):
    /* sha1sum:19fc45110ea6cd3cdd0a5faca256a3797a069a80 Windows 7 timesi.ttf */
    స్థితి HB_CODEPOINT_ENCODE3 (442, 2874, 39116):
    /* sha1sum:6d2d3c9ed5b7de87bc84eae0df95ee5232ecde26 Windows 7 timesbi.ttf */
    స్థితి HB_CODEPOINT_ENCODE3 (430, 2874, 39374):
    /* sha1sum:8583225a8b49667c077b3525333f84af08c6bcd8 OS X 10.11.3 Times New Roman Italic.ttf */
    స్థితి HB_CODEPOINT_ENCODE3 (490, 3046, 41638):
    /* sha1sum:ec0f5a8751845355b7c3271d11f9918a966cb8c9 OS X 10.11.3 Times New Roman Bold Italic.ttf */
    స్థితి HB_CODEPOINT_ENCODE3 (478, 3046, 41902):
    /* sha1sum:96eda93f7d33e79962451c6c39a6b51ee893ce8c  tahoma.ttf from Windows 8 */
    స్థితి HB_CODEPOINT_ENCODE3 (898, 12554, 46470):
    /* sha1sum:20928dc06014e0cd120b6fc942d0c3b1a46ac2bc  tahomabd.ttf from Windows 8 */
    స్థితి HB_CODEPOINT_ENCODE3 (910, 12566, 47732):
    /* sha1sum:4f95b7e4878f60fa3a39ca269618dfde9721a79e  tahoma.ttf from Windows 8.1 */
    స్థితి HB_CODEPOINT_ENCODE3 (928, 23298, 59332):
    /* sha1sum:6d400781948517c3c0441ba42acb309584b73033  tahomabd.ttf from Windows 8.1 */
    స్థితి HB_CODEPOINT_ENCODE3 (940, 23310, 60732):
    /* tahoma.ttf v6.04 from Windows 8.1 x64, see https://bugzilla.mozilla.org/show_bug.cgi?id=1279925 */
    స్థితి HB_CODEPOINT_ENCODE3 (964, 23836, 60072):
    /* tahomabd.ttf v6.04 from Windows 8.1 x64, see https://bugzilla.mozilla.org/show_bug.cgi?id=1279925 */
    స్థితి HB_CODEPOINT_ENCODE3 (976, 23832, 61456):
    /* sha1sum:e55fa2dfe957a9f7ec26be516a0e30b0c925f846  tahoma.ttf from Windows 10 */
    స్థితి HB_CODEPOINT_ENCODE3 (994, 24474, 60336):
    /* sha1sum:7199385abb4c2cc81c83a151a7599b6368e92343  tahomabd.ttf from Windows 10 */
    స్థితి HB_CODEPOINT_ENCODE3 (1006, 24470, 61740):
    /* tahoma.ttf v6.91 from Windows 10 x64, see https://bugzilla.mozilla.org/show_bug.cgi?id=1279925 */
    స్థితి HB_CODEPOINT_ENCODE3 (1006, 24576, 61346):
    /* tahomabd.ttf v6.91 from Windows 10 x64, see https://bugzilla.mozilla.org/show_bug.cgi?id=1279925 */
    స్థితి HB_CODEPOINT_ENCODE3 (1018, 24572, 62828):
    /* sha1sum:b9c84d820c49850d3d27ec498be93955b82772b5  tahoma.ttf from Windows 10 AU */
    స్థితి HB_CODEPOINT_ENCODE3 (1006, 24576, 61352):
    /* sha1sum:2bdfaab28174bdadd2f3d4200a30a7ae31db79d2  tahomabd.ttf from Windows 10 AU */
    స్థితి HB_CODEPOINT_ENCODE3 (1018, 24572, 62834):
    /* sha1sum:b0d36cf5a2fbe746a3dd277bffc6756a820807a7  Tahoma.ttf from Mac OS X 10.9 */
    స్థితి HB_CODEPOINT_ENCODE3 (832, 7324, 47162):
    /* sha1sum:12fc4538e84d461771b30c18b5eb6bd434e30fba  Tahoma Bold.ttf from Mac OS X 10.9 */
    స్థితి HB_CODEPOINT_ENCODE3 (844, 7302, 45474):
    /* sha1sum:eb8afadd28e9cf963e886b23a30b44ab4fd83acc  himalaya.ttf from Windows 7 */
    స్థితి HB_CODEPOINT_ENCODE3 (180, 13054, 7254):
    /* sha1sum:73da7f025b238a3f737aa1fde22577a6370f77b0  himalaya.ttf from Windows 8 */
    స్థితి HB_CODEPOINT_ENCODE3 (192, 12638, 7254):
    /* sha1sum:6e80fd1c0b059bbee49272401583160dc1e6a427  himalaya.ttf from Windows 8.1 */
    స్థితి HB_CODEPOINT_ENCODE3 (192, 12690, 7254):
    /* 8d9267aea9cd2c852ecfb9f12a6e834bfaeafe44  cantarell-fonts-0.0.21/otf/Cantarell-Regular.otf */
    /* 983988ff7b47439ab79aeaf9a45bd4a2c5b9d371  cantarell-fonts-0.0.21/otf/Cantarell-Oblique.otf */
    స్థితి HB_CODEPOINT_ENCODE3 (188, 248, 3852):
    /* 2c0c90c6f6087ffbfea76589c93113a9cbb0e75f  cantarell-fonts-0.0.21/otf/Cantarell-Bold.otf */
    /* 55461f5b853c6da88069ffcdf7f4dd3f8d7e3e6b  cantarell-fonts-0.0.21/otf/Cantarell-Bold-Oblique.otf */
    స్థితి HB_CODEPOINT_ENCODE3 (188, 264, 3426):
    /* d125afa82a77a6475ac0e74e7c207914af84b37a padauk-2.80/Padauk.ttf RHEL 7.2 */
    స్థితి HB_CODEPOINT_ENCODE3 (1058, 47032, 11818):
    /* 0f7b80437227b90a577cc078c0216160ae61b031 padauk-2.80/Padauk-Bold.ttf RHEL 7.2*/
    స్థితి HB_CODEPOINT_ENCODE3 (1046, 47030, 12600):
    /* d3dde9aa0a6b7f8f6a89ef1002e9aaa11b882290 padauk-2.80/Padauk.ttf Ubuntu 16.04 */
    స్థితి HB_CODEPOINT_ENCODE3 (1058, 71796, 16770):
    /* 5f3c98ccccae8a953be2d122c1b3a77fd805093f padauk-2.80/Padauk-Bold.ttf Ubuntu 16.04 */
    స్థితి HB_CODEPOINT_ENCODE3 (1046, 71790, 17862):
    /* 6c93b63b64e8b2c93f5e824e78caca555dc887c7 padauk-2.80/Padauk-book.ttf */
    స్థితి HB_CODEPOINT_ENCODE3 (1046, 71788, 17112):
    /* d89b1664058359b8ec82e35d3531931125991fb9 padauk-2.80/Padauk-bookbold.ttf */
    స్థితి HB_CODEPOINT_ENCODE3 (1058, 71794, 17514):
    /* 824cfd193aaf6234b2b4dc0cf3c6ef576c0d00ef padauk-3.0/Padauk-book.ttf */
    స్థితి HB_CODEPOINT_ENCODE3 (1330, 109904, 57938):
    /* 91fcc10cf15e012d27571e075b3b4dfe31754a8a padauk-3.0/Padauk-bookbold.ttf */
    స్థితి HB_CODEPOINT_ENCODE3 (1330, 109904, 58972):
    /* sha1sum: c26e41d567ed821bed997e937bc0c41435689e85  Padauk.ttf
     *  "Padauk Regular" "Version 2.5", see https://crbug.com/681813 */
    స్థితి HB_CODEPOINT_ENCODE3 (1004, 59092, 14836):
    /* 88d2006ca084f04af2df1954ed714a8c71e8400f  Courier New.ttf from macOS 15 */
    స్థితి HB_CODEPOINT_ENCODE3 (588, 5078, 14418):
    /* 608e3ebb6dd1aee521cff08eb07d500a2c59df68  Courier New Bold.ttf from macOS 15 */
    స్థితి HB_CODEPOINT_ENCODE3 (588, 5078, 14238):
    /* d13221044ff054efd78f1cd8631b853c3ce85676  cour.ttf from Windows 10 */
    స్థితి HB_CODEPOINT_ENCODE3 (894, 17162, 33960):
    /* 68ed4a22d8067fcf1622ac6f6e2f4d3a2e3ec394  courbd.ttf from Windows 10 */
    స్థితి HB_CODEPOINT_ENCODE3 (894, 17154, 34472):
    /* 4cdb0259c96b7fd7c103821bb8f08f7cc6b211d7  cour.ttf from Windows 8.1 */
    స్థితి HB_CODEPOINT_ENCODE3 (816, 7868, 17052):
    /* 920483d8a8ed37f7f0afdabbe7f679aece7c75d8  courbd.ttf from Windows 8.1 */
    స్థితి HB_CODEPOINT_ENCODE3 (816, 7868, 17138):
      తిరిగి నిజం;
  }
  తిరిగి అబద్ధం;
}

మూస <తార్కిక సమితి_Digest>
స్థిర శూన్య
hb_ot_layout_సమితి_glyph_props (hb_font_t *font,
			       hb_బఫరు_t *బఫరు)
{
  hb_బఫరు_assert_gsubgpos_vars (బఫరు);

  స్థిరరాశి స్వయంచాలక &gdef = *font->face->పట్టిక.GDEF;
  గుర్తులేని పూర్ణాంకం లెక్క = బఫరు->len;
  hb_glyph_info_t *info = బఫరు->info;
  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
  {
    ఒకవేళ (సమితి_Digest)
      బఫరు->digest.కూడు (info[i].codepoint);
    hb_glyph_info_సమితి_glyph_props (&info[i], gdef.get_glyph_props (info[i].codepoint));
    _hb_glyph_info_clear_lig_props (&info[i]);
  }
}

/* Public API */

/**
 * hb_ot_layout_has_glyph_classes:
 * @face: #hb_face_t to work upon
 *
 * Tests whether a face has any glyph classes defined in its GDEF table.
 *
 * Return value: `true` if data found, `false` otherwise
 *
 **/
hb_bool_t
hb_ot_layout_has_glyph_classes (hb_face_t *face)
{
  తిరిగి face->పట్టిక.GDEF->పట్టిక->has_glyph_classes ();
}

/**
 * hb_ot_layout_get_glyph_class:
 * @face: The #hb_face_t to work on
 * @glyph: The #hb_codepoint_t code point to query
 *
 * Fetches the GDEF class of the requested glyph in the specified face.
 *
 * Return value: The #hb_ot_layout_glyph_class_t glyph class of the given code
 * point in the GDEF table of the face.
 *
 * Since: 0.9.7
 **/
hb_ot_layout_glyph_class_t
hb_ot_layout_get_glyph_class (hb_face_t      *face,
			      hb_codepoint_t  glyph)
{
  తిరిగి (hb_ot_layout_glyph_class_t) face->పట్టిక.GDEF->పట్టిక->get_glyph_class (glyph);
}

/**
 * hb_ot_layout_get_glyphs_in_class:
 * @face: The #hb_face_t to work on
 * @klass: The #hb_ot_layout_glyph_class_t GDEF class to retrieve
 * @glyphs: (out): The #hb_set_t set of all glyphs belonging to the requested
 *          class.
 *
 * Retrieves the set of all glyphs from the face that belong to the requested
 * glyph class in the face's GDEF table.
 *
 * Since: 0.9.7
 **/
శూన్య
hb_ot_layout_get_glyphs_లోపల_class (hb_face_t                  *face,
				  hb_ot_layout_glyph_class_t  klass,
				  hb_సమితి_t                   *glyphs /*బహిర్గతం*/)
{
  తిరిగి face->పట్టిక.GDEF->పట్టిక->get_glyphs_లోపల_class (klass, glyphs);
}

#నిర్వచితం_కాకపోతే HB_NO_LAYOUT_వాడని
/**
 * hb_ot_layout_get_attach_points:
 * @face: The #hb_face_t to work on
 * @glyph: The #hb_codepoint_t code point to query
 * @start_offset: offset of the first attachment point to retrieve
 * @point_count: (inout) (nullable): Input = the maximum number of attachment points to return;
 *               Output = the actual number of attachment points returned (may be zero)
 * @point_array: (out) (array length=point_count) (nullable): The array of attachment points found for the query
 *
 * Fetches a list of all attachment points for the specified glyph in the GDEF
 * table of the face. The list returned will begin at the offset provided.
 *
 * Useful if the client program wishes to cache the list.
 *
 * Return value: Total number of attachment points for @glyph.
 *
 **/
గుర్తులేని పూర్ణాంకం
hb_ot_layout_get_attach_points (hb_face_t      *face,
				hb_codepoint_t  glyph,
				గుర్తులేని పూర్ణాంకం    ప్రారంభం_స్థానభ్రంశం,
				గుర్తులేని పూర్ణాంకం   *బిందువు_లెక్క /* IN/OUT */,
				గుర్తులేని పూర్ణాంకం   *బిందువు_array /*బహిర్గతం*/)
{
  తిరిగి face->పట్టిక.GDEF->పట్టిక->get_attach_points (glyph,
						     ప్రారంభం_స్థానభ్రంశం,
						     బిందువు_లెక్క,
						     బిందువు_array);
}
/**
 * hb_ot_layout_get_ligature_carets:
 * @font: The #hb_font_t to work on
 * @direction: The #hb_direction_t text direction to use
 * @glyph: The #hb_codepoint_t code point to query
 * @start_offset: offset of the first caret position to retrieve
 * @caret_count: (inout) (nullable): Input = the maximum number of caret positions to return;
 *               Output = the actual number of caret positions returned (may be zero)
 * @caret_array: (out) (array length=caret_count) (nullable): The array of caret positions found for the query
 *
 * Fetches a list of the caret positions defined for a ligature glyph in the GDEF
 * table of the font. The list returned will begin at the offset provided.
 *
 * Note that a ligature that is formed from n characters will have n-1
 * caret positions. The first character is not represented in the array,
 * since its caret position is the glyph position.
 *
 * The positions returned by this function are 'unshaped', and will have to
 * be fixed up for kerning that may be applied to the ligature glyph.
 *
 * Return value: Total number of ligature caret positions for @glyph.
 *
 **/
గుర్తులేని పూర్ణాంకం
hb_ot_layout_get_ligature_carets (hb_font_t      *font,
				  hb_direction_t  direction,
				  hb_codepoint_t  glyph,
				  గుర్తులేని పూర్ణాంకం    ప్రారంభం_స్థానభ్రంశం,
				  గుర్తులేని పూర్ణాంకం   *caret_లెక్క /* IN/OUT */,
				  hb_position_t  *caret_array /*బహిర్గతం*/)
{
  తిరిగి font->face->పట్టిక.GDEF->పట్టిక->get_lig_carets (font, direction, glyph, ప్రారంభం_స్థానభ్రంశం, caret_లెక్క, caret_array);
}
#అయితే_ముగింపు


/*
 * GSUB/GPOS
 */

తార్కిక
GSUB::is_blocklisted (hb_blob_t *blob HB_వాడని,
			  hb_face_t *face) స్థిరరాశి
{
#నిర్వచితమైతే HB_NO_OT_LAYOUT_BLOCKLIST
  తిరిగి అబద్ధం;
#అయితే_ముగింపు
  తిరిగి అబద్ధం;
}

తార్కిక
GPOS::is_blocklisted (hb_blob_t *blob HB_వాడని,
			  hb_face_t *face HB_వాడని) స్థిరరాశి
{
#నిర్వచితమైతే HB_NO_OT_LAYOUT_BLOCKLIST
  తిరిగి అబద్ధం;
#అయితే_ముగింపు
  తిరిగి అబద్ధం;
}

స్థిర స్థిరరాశి OT::GSUBGPOS&
get_gsubgpos_పట్టిక (hb_face_t *face,
		    hb_ట్యాగ్_t   పట్టిక_ట్యాగ్_2)
{
  ఎంపిక (పట్టిక_ట్యాగ్_2) {
    స్థితి HB_OT_ట్యాగ్_GSUB: తిరిగి *face->పట్టిక.GSUB->పట్టిక;
    స్థితి HB_OT_ట్యాగ్_GPOS: తిరిగి *face->పట్టిక.GPOS->పట్టిక;
    అప్రమేయ:             తిరిగి Null (OT::GSUBGPOS);
  }
}


/**
 * hb_ot_layout_table_get_script_tags:
 * @face: #hb_face_t to work upon
 * @table_tag: #HB_OT_TAG_GSUB or #HB_OT_TAG_GPOS
 * @start_offset: offset of the first script tag to retrieve
 * @script_count: (inout) (nullable): Input = the maximum number of script tags to return;
 *                Output = the actual number of script tags returned (may be zero)
 * @script_tags: (out) (array length=script_count) (nullable): The array of #hb_tag_t script tags found for the query
 *
 * Fetches a list of all scripts enumerated in the specified face's GSUB table
 * or GPOS table. The list returned will begin at the offset provided.
 *
 * Return value: Total number of script tags.
 *
 **/
గుర్తులేని పూర్ణాంకం
hb_ot_layout_పట్టిక_get_script_ట్యాగులు (hb_face_t    *face,
				    hb_ట్యాగ్_t      పట్టిక_ట్యాగ్_2,
				    గుర్తులేని పూర్ణాంకం  ప్రారంభం_స్థానభ్రంశం,
				    గుర్తులేని పూర్ణాంకం *script_లెక్క /* IN/OUT */,
				    hb_ట్యాగ్_t     *script_ట్యాగులు  /*బహిర్గతం*/)
{
  స్థిరరాశి OT::GSUBGPOS &g = get_gsubgpos_పట్టిక (face, పట్టిక_ట్యాగ్_2);

  తిరిగి g.get_script_ట్యాగులు (ప్రారంభం_స్థానభ్రంశం, script_లెక్క, script_ట్యాగులు);
}

#నిర్వచించు HB_OT_ట్యాగ్_LATIN_SCRIPT		HB_ట్యాగ్ ('l', 'a', 't', 'n')

/**
 * hb_ot_layout_table_find_script:
 * @face: #hb_face_t to work upon
 * @table_tag: #HB_OT_TAG_GSUB or #HB_OT_TAG_GPOS
 * @script_tag: #hb_tag_t of the script tag requested
 * @script_index: (out): The index of the requested script tag
 *
 * Fetches the index if a given script tag in the specified face's GSUB table
 * or GPOS table.
 *
 * Return value: `true` if the script is found, `false` otherwise
 *
 **/
hb_bool_t
hb_ot_layout_పట్టిక_కనుగొను_script (hb_face_t    *face,
				hb_ట్యాగ్_t      పట్టిక_ట్యాగ్_2,
				hb_ట్యాగ్_t      script_ట్యాగ్,
				గుర్తులేని పూర్ణాంకం *script_సూచీ /*బహిర్గతం*/)
{
  static_assert ((OT::సూచీ::NOT_దొరికింది_సూచీ == HB_OT_LAYOUT_NO_SCRIPT_సూచీ), "");
  స్థిరరాశి OT::GSUBGPOS &g = get_gsubgpos_పట్టిక (face, పట్టిక_ట్యాగ్_2);

  ఒకవేళ (g.కనుగొను_script_సూచీ (script_ట్యాగ్, script_సూచీ))
    తిరిగి నిజం;

  /* try finding 'DFLT' */
  ఒకవేళ (g.కనుగొను_script_సూచీ (HB_OT_ట్యాగ్_DEFAULT_SCRIPT, script_సూచీ))
    తిరిగి అబద్ధం;

  /* try with 'dflt'; MS site has had typos and many fonts use it now :(.
   * including many versions of DejaVu Sans Mono! */
  ఒకవేళ (g.కనుగొను_script_సూచీ (HB_OT_ట్యాగ్_DEFAULT_LANGUAGE, script_సూచీ))
    తిరిగి అబద్ధం;

  /* try with 'latn'; some old fonts put their features there even though
     they're really trying to support Thai, for example :( */
  ఒకవేళ (g.కనుగొను_script_సూచీ (HB_OT_ట్యాగ్_LATIN_SCRIPT, script_సూచీ))
    తిరిగి అబద్ధం;

  ఒకవేళ (script_సూచీ) *script_సూచీ = HB_OT_LAYOUT_NO_SCRIPT_సూచీ;
  తిరిగి అబద్ధం;
}

#నిర్వచితం_కాకపోతే HB_DISABLE_DEPRECATED
/**
 * hb_ot_layout_table_choose_script:
 * @face: #hb_face_t to work upon
 * @table_tag: #HB_OT_TAG_GSUB or #HB_OT_TAG_GPOS
 * @script_tags: Array of #hb_tag_t script tags
 * @script_index: (out): The index of the chosen script
 * @chosen_script: (out): #hb_tag_t of the chosen script
 *
 * Deprecated since 2.0.0
 **/
hb_bool_t
hb_ot_layout_పట్టిక_ఎంచుకో_script (hb_face_t      *face,
				  hb_ట్యాగ్_t        పట్టిక_ట్యాగ్_2,
				  స్థిరరాశి hb_ట్యాగ్_t *script_ట్యాగులు,
				  గుర్తులేని పూర్ణాంకం   *script_సూచీ  /*బహిర్గతం*/,
				  hb_ట్యాగ్_t       *chosen_script /*బహిర్గతం*/)
{
  స్థిరరాశి hb_ట్యాగ్_t *t;
  కొరకు (t = script_ట్యాగులు; *t; t++);
  తిరిగి hb_ot_layout_పట్టిక_select_script (face, పట్టిక_ట్యాగ్_2, t - script_ట్యాగులు, script_ట్యాగులు, script_సూచీ, chosen_script);
}
#అయితే_ముగింపు

/**
 * hb_ot_layout_table_select_script:
 * @face: #hb_face_t to work upon
 * @table_tag: #HB_OT_TAG_GSUB or #HB_OT_TAG_GPOS
 * @script_count: Number of script tags in the array
 * @script_tags: Array of #hb_tag_t script tags
 * @script_index: (out) (nullable): The index of the requested script
 * @chosen_script: (out) (nullable): #hb_tag_t of the requested script
 *
 * Selects an OpenType script for @table_tag from the @script_tags array.
 *
 * If the table does not have any of the requested scripts, then `DFLT`,
 * `dflt`, and `latn` tags are tried in that order. If the table still does not
 * have any of these scripts, @script_index is set to
 * #HB_OT_LAYOUT_NO_SCRIPT_INDEX and @chosen_script is set to #HB_TAG_NONE.
 *
 * Return value:
 * `true` if one of the requested scripts is selected, `false` if a fallback
 * script is selected or if no scripts are selected.
 *
 * Since: 2.0.0
 **/
hb_bool_t
hb_ot_layout_పట్టిక_select_script (hb_face_t      *face,
				  hb_ట్యాగ్_t        పట్టిక_ట్యాగ్_2,
				  గుర్తులేని పూర్ణాంకం    script_లెక్క,
				  స్థిరరాశి hb_ట్యాగ్_t *script_ట్యాగులు,
				  గుర్తులేని పూర్ణాంకం   *script_సూచీ  /*బహిర్గతం*/,
				  hb_ట్యాగ్_t       *chosen_script /*బహిర్గతం*/)
{
  static_assert ((OT::సూచీ::NOT_దొరికింది_సూచీ == HB_OT_LAYOUT_NO_SCRIPT_సూచీ), "");
  స్థిరరాశి OT::GSUBGPOS &g = get_gsubgpos_పట్టిక (face, పట్టిక_ట్యాగ్_2);
  గుర్తులేని పూర్ణాంకం i;

  కొరకు (i = 0; i < script_లెక్క; i++)
  {
    ఒకవేళ (g.కనుగొను_script_సూచీ (script_ట్యాగులు[i], script_సూచీ))
    {
      ఒకవేళ (chosen_script)
	*chosen_script = script_ట్యాగులు[i];
      తిరిగి నిజం;
    }
  }

  /* try finding 'DFLT' */
  ఒకవేళ (g.కనుగొను_script_సూచీ (HB_OT_ట్యాగ్_DEFAULT_SCRIPT, script_సూచీ)) {
    ఒకవేళ (chosen_script)
      *chosen_script = HB_OT_ట్యాగ్_DEFAULT_SCRIPT;
    తిరిగి అబద్ధం;
  }

  /* try with 'dflt'; MS site has had typos and many fonts use it now :( */
  ఒకవేళ (g.కనుగొను_script_సూచీ (HB_OT_ట్యాగ్_DEFAULT_LANGUAGE, script_సూచీ)) {
    ఒకవేళ (chosen_script)
      *chosen_script = HB_OT_ట్యాగ్_DEFAULT_LANGUAGE;
    తిరిగి అబద్ధం;
  }

  /* try with 'latn'; some old fonts put their features there even though
     they're really trying to support Thai, for example :( */
  ఒకవేళ (g.కనుగొను_script_సూచీ (HB_OT_ట్యాగ్_LATIN_SCRIPT, script_సూచీ)) {
    ఒకవేళ (chosen_script)
      *chosen_script = HB_OT_ట్యాగ్_LATIN_SCRIPT;
    తిరిగి అబద్ధం;
  }

  ఒకవేళ (script_సూచీ) *script_సూచీ = HB_OT_LAYOUT_NO_SCRIPT_సూచీ;
  ఒకవేళ (chosen_script)
    *chosen_script = HB_ట్యాగ్_NONE;
  తిరిగి అబద్ధం;
}


/**
 * hb_ot_layout_table_get_feature_tags:
 * @face: #hb_face_t to work upon
 * @table_tag: #HB_OT_TAG_GSUB or #HB_OT_TAG_GPOS
 * @start_offset: offset of the first feature tag to retrieve
 * @feature_count: (inout) (nullable): Input = the maximum number of feature tags to return;
 *                 Output = the actual number of feature tags returned (may be zero)
 * @feature_tags: (out) (array length=feature_count) (nullable): Array of feature tags found in the table
 *
 * Fetches a list of all feature tags in the given face's GSUB or GPOS table.
 * Note that there might be duplicate feature tags, belonging to different
 * script/language-system pairs of the table.
 *
 * Return value: Total number of feature tags.
 *
 * Since: 0.6.0
 *
 **/
గుర్తులేని పూర్ణాంకం
hb_ot_layout_పట్టిక_get_feature_ట్యాగులు (hb_face_t    *face,
				     hb_ట్యాగ్_t      పట్టిక_ట్యాగ్_2,
				     గుర్తులేని పూర్ణాంకం  ప్రారంభం_స్థానభ్రంశం,
				     గుర్తులేని పూర్ణాంకం *feature_లెక్క_2 /* IN/OUT */,
				     hb_ట్యాగ్_t     *feature_ట్యాగులు  /*బహిర్గతం*/)
{
  స్థిరరాశి OT::GSUBGPOS &g = get_gsubgpos_పట్టిక (face, పట్టిక_ట్యాగ్_2);

  తిరిగి g.get_feature_ట్యాగులు (ప్రారంభం_స్థానభ్రంశం, feature_లెక్క_2, feature_ట్యాగులు);
}


/**
 * hb_ot_layout_table_find_feature:
 * @face: #hb_face_t to work upon
 * @table_tag: #HB_OT_TAG_GSUB or #HB_OT_TAG_GPOS
 * @feature_tag: The #hb_tag_t of the requested feature tag
 * @feature_index: (out): The index of the requested feature
 *
 * Fetches the index for a given feature tag in the specified face's GSUB table
 * or GPOS table.
 *
 * Return value: `true` if the feature is found, `false` otherwise
 *
 * Since: 0.6.0
 *
 **/
తార్కిక
hb_ot_layout_పట్టిక_కనుగొను_feature (hb_face_t    *face,
				 hb_ట్యాగ్_t      పట్టిక_ట్యాగ్_2,
				 hb_ట్యాగ్_t      feature_ట్యాగ్,
				 గుర్తులేని పూర్ణాంకం *feature_సూచీ /*బహిర్గతం*/)
{
  static_assert ((OT::సూచీ::NOT_దొరికింది_సూచీ == HB_OT_LAYOUT_NO_FEATURE_సూచీ), "");
  స్థిరరాశి OT::GSUBGPOS &g = get_gsubgpos_పట్టిక (face, పట్టిక_ట్యాగ్_2);

  గుర్తులేని పూర్ణాంకం అంకె_features = g.get_feature_లెక్క ();
  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < అంకె_features; i++)
  {
    ఒకవేళ (feature_ట్యాగ్ == g.get_feature_ట్యాగ్ (i)) {
      ఒకవేళ (feature_సూచీ) *feature_సూచీ = i;
      తిరిగి నిజం;
    }
  }

  ఒకవేళ (feature_సూచీ) *feature_సూచీ = HB_OT_LAYOUT_NO_FEATURE_సూచీ;
  తిరిగి అబద్ధం;
}


/**
 * hb_ot_layout_script_get_language_tags:
 * @face: #hb_face_t to work upon
 * @table_tag: #HB_OT_TAG_GSUB or #HB_OT_TAG_GPOS
 * @script_index: The index of the requested script tag
 * @start_offset: offset of the first language tag to retrieve
 * @language_count: (inout) (nullable): Input = the maximum number of language tags to return;
 *                  Output = the actual number of language tags returned (may be zero)
 * @language_tags: (out) (array length=language_count) (nullable): Array of language tags found in the table
 *
 * Fetches a list of language tags in the given face's GSUB or GPOS table, underneath
 * the specified script index. The list returned will begin at the offset provided.
 *
 * Return value: Total number of language tags.
 *
 * Since: 0.6.0
 *
 **/
గుర్తులేని పూర్ణాంకం
hb_ot_layout_script_get_language_ట్యాగులు (hb_face_t    *face,
				       hb_ట్యాగ్_t      పట్టిక_ట్యాగ్_2,
				       గుర్తులేని పూర్ణాంకం  script_సూచీ,
				       గుర్తులేని పూర్ణాంకం  ప్రారంభం_స్థానభ్రంశం,
				       గుర్తులేని పూర్ణాంకం *language_లెక్క /* IN/OUT */,
				       hb_ట్యాగ్_t     *language_ట్యాగులు  /*బహిర్గతం*/)
{
  స్థిరరాశి OT::Script &s = get_gsubgpos_పట్టిక (face, పట్టిక_ట్యాగ్_2).get_script (script_సూచీ);

  తిరిగి s.get_lang_sys_ట్యాగులు (ప్రారంభం_స్థానభ్రంశం, language_లెక్క, language_ట్యాగులు);
}


#నిర్వచితం_కాకపోతే HB_DISABLE_DEPRECATED
/**
 * hb_ot_layout_script_find_language:
 * @face: #hb_face_t to work upon
 * @table_tag: #HB_OT_TAG_GSUB or #HB_OT_TAG_GPOS
 * @script_index: The index of the requested script tag
 * @language_tag: The #hb_tag_t of the requested language
 * @language_index: The index of the requested language
 *
 * Fetches the index of a given language tag in the specified face's GSUB table
 * or GPOS table, underneath the specified script tag.
 *
 * Return value: `true` if the language tag is found, `false` otherwise
 *
 * Since: 0.6.0
 * Deprecated: 2.0.0
 **/
hb_bool_t
hb_ot_layout_script_కనుగొను_language (hb_face_t    *face,
				   hb_ట్యాగ్_t      పట్టిక_ట్యాగ్_2,
				   గుర్తులేని పూర్ణాంకం  script_సూచీ,
				   hb_ట్యాగ్_t      language_ట్యాగ్,
				   గుర్తులేని పూర్ణాంకం *language_సూచీ)
{
  తిరిగి hb_ot_layout_script_select_language (face,
					      పట్టిక_ట్యాగ్_2,
					      script_సూచీ,
					      1,
					      &language_ట్యాగ్,
					      language_సూచీ);
}
#అయితే_ముగింపు


/**
 * hb_ot_layout_script_select_language2:
 * @face: #hb_face_t to work upon
 * @table_tag: #HB_OT_TAG_GSUB or #HB_OT_TAG_GPOS
 * @script_index: The index of the requested script tag
 * @language_count: The number of languages in the specified script
 * @language_tags: The array of language tags
 * @language_index: (out): The index of the chosen language
 * @chosen_language: (out): #hb_tag_t of the chosen language
 *
 * Fetches the index of the first language tag fom @language_tags that is present
 * in the specified face's GSUB or GPOS table, underneath the specified script
 * index.
 *
 * If none of the given language tags is found, `false` is returned and
 * @language_index is set to #HB_OT_LAYOUT_DEFAULT_LANGUAGE_INDEX and
 * @chosen_language is set to #HB_TAG_NONE.
 *
 * Return value: `true` if one of the given language tags is found, `false` otherwise
 *
 * Since: 7.0.0
 **/
hb_bool_t
hb_ot_layout_script_select_language2 (hb_face_t      *face,
				     hb_ట్యాగ్_t        పట్టిక_ట్యాగ్_2,
				     గుర్తులేని పూర్ణాంకం    script_సూచీ,
				     గుర్తులేని పూర్ణాంకం    language_లెక్క,
				     స్థిరరాశి hb_ట్యాగ్_t *language_ట్యాగులు,
				     గుర్తులేని పూర్ణాంకం   *language_సూచీ /*బహిర్గతం*/,
				     hb_ట్యాగ్_t       *chosen_language /*బహిర్గతం*/)
{
  static_assert ((OT::సూచీ::NOT_దొరికింది_సూచీ == HB_OT_LAYOUT_DEFAULT_LANGUAGE_సూచీ), "");
  స్థిరరాశి OT::Script &s = get_gsubgpos_పట్టిక (face, పట్టిక_ట్యాగ్_2).get_script (script_సూచీ);
  గుర్తులేని పూర్ణాంకం i;

  కొరకు (i = 0; i < language_లెక్క; i++)
  {
    ఒకవేళ (s.కనుగొను_lang_sys_సూచీ (language_ట్యాగులు[i], language_సూచీ))
    {
      ఒకవేళ (chosen_language)
        *chosen_language = language_ట్యాగులు[i];
      తిరిగి నిజం;
    }
  }

  /* try finding 'dflt' */
  ఒకవేళ (s.కనుగొను_lang_sys_సూచీ (HB_OT_ట్యాగ్_DEFAULT_LANGUAGE, language_సూచీ))
  {
    ఒకవేళ (chosen_language)
      *chosen_language = HB_OT_ట్యాగ్_DEFAULT_LANGUAGE;
    తిరిగి అబద్ధం;
  }

  ఒకవేళ (language_సూచీ)
    *language_సూచీ = HB_OT_LAYOUT_DEFAULT_LANGUAGE_సూచీ;
  ఒకవేళ (chosen_language)
    *chosen_language = HB_ట్యాగ్_NONE;
  తిరిగి అబద్ధం;
}

/**
 * hb_ot_layout_script_select_language:
 * @face: #hb_face_t to work upon
 * @table_tag: #HB_OT_TAG_GSUB or #HB_OT_TAG_GPOS
 * @script_index: The index of the requested script tag
 * @language_count: The number of languages in the specified script
 * @language_tags: The array of language tags
 * @language_index: (out): The index of the requested language
 *
 * Fetches the index of the first language tag fom @language_tags that is present
 * in the specified face's GSUB or GPOS table, underneath the specified script
 * index.
 *
 * If none of the given language tags is found, `false` is returned and
 * @language_index is set to the default language index.
 *
 * Return value: `true` if one of the given language tags is found, `false` otherwise
 *
 * Since: 2.0.0
 **/
hb_bool_t
hb_ot_layout_script_select_language (hb_face_t      *face,
				     hb_ట్యాగ్_t        పట్టిక_ట్యాగ్_2,
				     గుర్తులేని పూర్ణాంకం    script_సూచీ,
				     గుర్తులేని పూర్ణాంకం    language_లెక్క,
				     స్థిరరాశి hb_ట్యాగ్_t *language_ట్యాగులు,
				     గుర్తులేని పూర్ణాంకం   *language_సూచీ /*బహిర్గతం*/)
{
  తిరిగి hb_ot_layout_script_select_language2 (face, పట్టిక_ట్యాగ్_2,
					       script_సూచీ,
					       language_లెక్క, language_ట్యాగులు,
					       language_సూచీ, శూన్యసూచిక);
}

/**
 * hb_ot_layout_language_get_required_feature_index:
 * @face: #hb_face_t to work upon
 * @table_tag: #HB_OT_TAG_GSUB or #HB_OT_TAG_GPOS
 * @script_index: The index of the requested script tag
 * @language_index: The index of the requested language tag
 * @feature_index: (out): The index of the requested feature
 *
 * Fetches the index of a requested feature in the given face's GSUB or GPOS table,
 * underneath the specified script and language.
 *
 * Return value: `true` if the feature is found, `false` otherwise
 *
 * Since: 0.6.0
 *
 **/
hb_bool_t
hb_ot_layout_language_get_required_feature_సూచీ (hb_face_t    *face,
						  hb_ట్యాగ్_t      పట్టిక_ట్యాగ్_2,
						  గుర్తులేని పూర్ణాంకం  script_సూచీ,
						  గుర్తులేని పూర్ణాంకం  language_సూచీ,
						  గుర్తులేని పూర్ణాంకం *feature_సూచీ /*బహిర్గతం*/)
{
  తిరిగి hb_ot_layout_language_get_required_feature (face,
						     పట్టిక_ట్యాగ్_2,
						     script_సూచీ,
						     language_సూచీ,
						     feature_సూచీ,
						     శూన్యసూచిక);
}


/**
 * hb_ot_layout_language_get_required_feature:
 * @face: #hb_face_t to work upon
 * @table_tag: #HB_OT_TAG_GSUB or #HB_OT_TAG_GPOS
 * @script_index: The index of the requested script tag
 * @language_index: The index of the requested language tag
 * @feature_index: (out): The index of the requested feature
 * @feature_tag: (out): The #hb_tag_t of the requested feature
 *
 * Fetches the tag of a requested feature index in the given face's GSUB or GPOS table,
 * underneath the specified script and language.
 *
 * Return value: `true` if the feature is found, `false` otherwise
 *
 * Since: 0.9.30
 **/
hb_bool_t
hb_ot_layout_language_get_required_feature (hb_face_t    *face,
					    hb_ట్యాగ్_t      పట్టిక_ట్యాగ్_2,
					    గుర్తులేని పూర్ణాంకం  script_సూచీ,
					    గుర్తులేని పూర్ణాంకం  language_సూచీ,
					    గుర్తులేని పూర్ణాంకం *feature_సూచీ /*బహిర్గతం*/,
					    hb_ట్యాగ్_t     *feature_ట్యాగ్   /*బహిర్గతం*/)
{
  స్థిరరాశి OT::GSUBGPOS &g = get_gsubgpos_పట్టిక (face, పట్టిక_ట్యాగ్_2);
  స్థిరరాశి OT::LangSys &l = g.get_script (script_సూచీ).get_lang_sys (language_సూచీ);

  గుర్తులేని పూర్ణాంకం సూచీ_2 = l.get_required_feature_సూచీ ();
  ఒకవేళ (feature_సూచీ) *feature_సూచీ = సూచీ_2;
  ఒకవేళ (feature_ట్యాగ్) *feature_ట్యాగ్ = g.get_feature_ట్యాగ్ (సూచీ_2);

  తిరిగి l.has_required_feature ();
}


/**
 * hb_ot_layout_language_get_feature_indexes:
 * @face: #hb_face_t to work upon
 * @table_tag: #HB_OT_TAG_GSUB or #HB_OT_TAG_GPOS
 * @script_index: The index of the requested script tag
 * @language_index: The index of the requested language tag
 * @start_offset: offset of the first feature tag to retrieve
 * @feature_count: (inout) (nullable): Input = the maximum number of feature tags to return;
 *                 Output: the actual number of feature tags returned (may be zero)
 * @feature_indexes: (out) (array length=feature_count) (nullable): The array of feature indexes found for the query
 *
 * Fetches a list of all features in the specified face's GSUB table
 * or GPOS table, underneath the specified script and language. The list
 * returned will begin at the offset provided.
 *
 * Return value: Total number of features.
 *
 * Since: 0.6.0
 *
 **/
గుర్తులేని పూర్ణాంకం
hb_ot_layout_language_get_feature_indexes (hb_face_t    *face,
					   hb_ట్యాగ్_t      పట్టిక_ట్యాగ్_2,
					   గుర్తులేని పూర్ణాంకం  script_సూచీ,
					   గుర్తులేని పూర్ణాంకం  language_సూచీ,
					   గుర్తులేని పూర్ణాంకం  ప్రారంభం_స్థానభ్రంశం,
					   గుర్తులేని పూర్ణాంకం *feature_లెక్క_2   /* IN/OUT */,
					   గుర్తులేని పూర్ణాంకం *feature_indexes /*బహిర్గతం*/)
{
  స్థిరరాశి OT::GSUBGPOS &g = get_gsubgpos_పట్టిక (face, పట్టిక_ట్యాగ్_2);
  స్థిరరాశి OT::LangSys &l = g.get_script (script_సూచీ).get_lang_sys (language_సూచీ);

  తిరిగి l.get_feature_indexes (ప్రారంభం_స్థానభ్రంశం, feature_లెక్క_2, feature_indexes);
}


/**
 * hb_ot_layout_language_get_feature_tags:
 * @face: #hb_face_t to work upon
 * @table_tag: #HB_OT_TAG_GSUB or #HB_OT_TAG_GPOS
 * @script_index: The index of the requested script tag
 * @language_index: The index of the requested language tag
 * @start_offset: offset of the first feature tag to retrieve
 * @feature_count: (inout) (nullable): Input = the maximum number of feature tags to return;
 *                 Output = the actual number of feature tags returned (may be zero)
 * @feature_tags: (out) (array length=feature_count) (nullable): The array of #hb_tag_t feature tags found for the query
 *
 * Fetches a list of all features in the specified face's GSUB table
 * or GPOS table, underneath the specified script and language. The list
 * returned will begin at the offset provided.
 *
 * Return value: Total number of feature tags.
 *
 * Since: 0.6.0
 *
 **/
గుర్తులేని పూర్ణాంకం
hb_ot_layout_language_get_feature_ట్యాగులు (hb_face_t    *face,
					hb_ట్యాగ్_t      పట్టిక_ట్యాగ్_2,
					గుర్తులేని పూర్ణాంకం  script_సూచీ,
					గుర్తులేని పూర్ణాంకం  language_సూచీ,
					గుర్తులేని పూర్ణాంకం  ప్రారంభం_స్థానభ్రంశం,
					గుర్తులేని పూర్ణాంకం *feature_లెక్క_2 /* IN/OUT */,
					hb_ట్యాగ్_t     *feature_ట్యాగులు  /*బహిర్గతం*/)
{
  స్థిరరాశి OT::GSUBGPOS &g = get_gsubgpos_పట్టిక (face, పట్టిక_ట్యాగ్_2);
  స్థిరరాశి OT::LangSys &l = g.get_script (script_సూచీ).get_lang_sys (language_సూచీ);

  static_assert ((పరిమాణం (గుర్తులేని పూర్ణాంకం) == పరిమాణం (hb_ట్యాగ్_t)), "");
  గుర్తులేని పూర్ణాంకం ret = l.get_feature_indexes (ప్రారంభం_స్థానభ్రంశం, feature_లెక్క_2, (గుర్తులేని పూర్ణాంకం *) feature_ట్యాగులు);

  ఒకవేళ (feature_లెక్క_2 && feature_ట్యాగులు) {
    గుర్తులేని పూర్ణాంకం లెక్క = *feature_లెక్క_2;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
      feature_ట్యాగులు[i] = g.get_feature_ట్యాగ్ ((గుర్తులేని పూర్ణాంకం) feature_ట్యాగులు[i]);
  }

  తిరిగి ret;
}


/**
 * hb_ot_layout_language_find_feature:
 * @face: #hb_face_t to work upon
 * @table_tag: #HB_OT_TAG_GSUB or #HB_OT_TAG_GPOS
 * @script_index: The index of the requested script tag
 * @language_index: The index of the requested language tag
 * @feature_tag: #hb_tag_t of the feature tag requested
 * @feature_index: (out): The index of the requested feature
 *
 * Fetches the index of a given feature tag in the specified face's GSUB table
 * or GPOS table, underneath the specified script and language.
 *
 * Return value: `true` if the feature is found, `false` otherwise
 *
 * Since: 0.6.0
 *
 **/
hb_bool_t
hb_ot_layout_language_కనుగొను_feature (hb_face_t    *face,
				    hb_ట్యాగ్_t      పట్టిక_ట్యాగ్_2,
				    గుర్తులేని పూర్ణాంకం  script_సూచీ,
				    గుర్తులేని పూర్ణాంకం  language_సూచీ,
				    hb_ట్యాగ్_t      feature_ట్యాగ్,
				    గుర్తులేని పూర్ణాంకం *feature_సూచీ /*బహిర్గతం*/)
{
  static_assert ((OT::సూచీ::NOT_దొరికింది_సూచీ == HB_OT_LAYOUT_NO_FEATURE_సూచీ), "");
  స్థిరరాశి OT::GSUBGPOS &g = get_gsubgpos_పట్టిక (face, పట్టిక_ట్యాగ్_2);
  స్థిరరాశి OT::LangSys &l = g.get_script (script_సూచీ).get_lang_sys (language_సూచీ);

  గుర్తులేని పూర్ణాంకం అంకె_features = l.get_feature_లెక్క ();
  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < అంకె_features; i++) {
    గుర్తులేని పూర్ణాంకం f_సూచీ = l.get_feature_సూచీ (i);

    ఒకవేళ (feature_ట్యాగ్ == g.get_feature_ట్యాగ్ (f_సూచీ)) {
      ఒకవేళ (feature_సూచీ) *feature_సూచీ = f_సూచీ;
      తిరిగి నిజం;
    }
  }

  ఒకవేళ (feature_సూచీ) *feature_సూచీ = HB_OT_LAYOUT_NO_FEATURE_సూచీ;
  తిరిగి అబద్ధం;
}


/**
 * hb_ot_layout_feature_get_lookups:
 * @face: #hb_face_t to work upon
 * @table_tag: #HB_OT_TAG_GSUB or #HB_OT_TAG_GPOS
 * @feature_index: The index of the requested feature
 * @start_offset: offset of the first lookup to retrieve
 * @lookup_count: (inout) (nullable): Input = the maximum number of lookups to return;
 *                Output = the actual number of lookups returned (may be zero)
 * @lookup_indexes: (out) (array length=lookup_count) (nullable): The array of lookup indexes found for the query
 *
 * Fetches a list of all lookups enumerated for the specified feature, in
 * the specified face's GSUB table or GPOS table. The list returned will
 * begin at the offset provided.
 *
 * Return value: Total number of lookups.
 *
 * Since: 0.9.7
 **/
గుర్తులేని పూర్ణాంకం
hb_ot_layout_feature_get_lookups (hb_face_t    *face,
				  hb_ట్యాగ్_t      పట్టిక_ట్యాగ్_2,
				  గుర్తులేని పూర్ణాంకం  feature_సూచీ,
				  గుర్తులేని పూర్ణాంకం  ప్రారంభం_స్థానభ్రంశం,
				  గుర్తులేని పూర్ణాంకం *lookup_లెక్క   /* IN/OUT */,
				  గుర్తులేని పూర్ణాంకం *lookup_indexes /*బహిర్గతం*/)
{
  తిరిగి hb_ot_layout_feature_with_variations_get_lookups (face,
							   పట్టిక_ట్యాగ్_2,
							   feature_సూచీ,
							   HB_OT_LAYOUT_NO_VARIATIONS_సూచీ,
							   ప్రారంభం_స్థానభ్రంశం,
							   lookup_లెక్క,
							   lookup_indexes);
}


/**
 * hb_ot_layout_table_get_lookup_count:
 * @face: #hb_face_t to work upon
 * @table_tag: #HB_OT_TAG_GSUB or #HB_OT_TAG_GPOS
 *
 * Fetches the total number of lookups enumerated in the specified
 * face's GSUB table or GPOS table.
 *
 * Return value: Total number of lookups.
 *
 * Since: 0.9.22
 **/
గుర్తులేని పూర్ణాంకం
hb_ot_layout_పట్టిక_get_lookup_లెక్క (hb_face_t    *face,
				     hb_ట్యాగ్_t      పట్టిక_ట్యాగ్_2)
{
  తిరిగి get_gsubgpos_పట్టిక (face, పట్టిక_ట్యాగ్_2).get_lookup_లెక్క ();
}


నిర్మాణం hb_collect_features_context_t
{
  hb_collect_features_context_t (hb_face_t *face,
				 hb_ట్యాగ్_t   పట్టిక_ట్యాగ్_2,
				 hb_సమితి_t  *feature_indices_,
				 స్థిరరాశి hb_ట్యాగ్_t *features)

    : g (get_gsubgpos_పట్టిక (face, పట్టిక_ట్యాగ్_2)),
      feature_indices (feature_indices_),
      has_feature_filter (అబద్ధం),
      script_లెక్క (0),langsys_లెక్క (0), feature_సూచీ_లెక్క (0)
  {
    compute_feature_filter (features);
  }

  శూన్య compute_feature_filter (స్థిరరాశి hb_ట్యాగ్_t *features)
  {
    ఒకవేళ (features == శూన్యసూచిక)
    {
      has_feature_filter = అబద్ధం;
      తిరిగి;
    }

    has_feature_filter = నిజం;
    hb_సమితి_t features_సమితి;
    కొరకు (; *features; features++)
      features_సమితి.కూడు (*features);

    కొరకు (గుర్తులేని i = 0; i < g.get_feature_లెక్క (); i++)
    {
      hb_ట్యాగ్_t ట్యాగ్_2 = g.get_feature_ట్యాగ్ (i);
      ఒకవేళ (features_సమితి.has (ట్యాగ్_2))
	feature_indices_filter.కూడు(i);
    }
  }

  తార్కిక సందర్శించిన (స్థిరరాశి OT::Script &s)
  {
    /* We might have Null() object here.  Don't want to involve
     * that in the memoize.  So, detect empty objects and return. */
    ఒకవేళ (unlikely (!s.has_default_lang_sys () &&
		  !s.get_lang_sys_లెక్క ()))
      తిరిగి నిజం;

    ఒకవేళ (script_లెక్క++ > HB_MAX_SCRIPTS)
      తిరిగి నిజం;

    తిరిగి సందర్శించిన (s, సందర్శించిన_script);
  }
  తార్కిక సందర్శించిన (స్థిరరాశి OT::LangSys &l)
  {
    /* We might have Null() object here.  Don't want to involve
     * that in the memoize.  So, detect empty objects and return. */
    ఒకవేళ (unlikely (!l.has_required_feature () &&
		  !l.get_feature_లెక్క ()))
      తిరిగి నిజం;

    ఒకవేళ (langsys_లెక్క++ > HB_MAX_LANGSYS)
      తిరిగి నిజం;

    తిరిగి సందర్శించిన (l, సందర్శించిన_langsys);
  }

  తార్కిక సందర్శించిన_feature_indices (గుర్తులేని లెక్క)
  {
    feature_సూచీ_లెక్క += లెక్క;
    తిరిగి feature_సూచీ_లెక్క > HB_MAX_FEATURE_INDICES;
  }

  అంతరంగ:
  మూస <రకం_పేరు T>
  తార్కిక సందర్శించిన (స్థిరరాశి T &p, hb_సమితి_t &సందర్శించిన_సమితి)
  {
    hb_codepoint_t delta = (hb_codepoint_t) ((uintptr_t) &p - (uintptr_t) &g);
     ఒకవేళ (సందర్శించిన_సమితి.has (delta))
      తిరిగి నిజం;

    సందర్శించిన_సమితి.కూడు (delta);
    తిరిగి అబద్ధం;
  }

  బహిరంగ:
  స్థిరరాశి OT::GSUBGPOS &g;
  hb_సమితి_t *feature_indices;
  hb_సమితి_t  feature_indices_filter;
  తార్కిక has_feature_filter;

  అంతరంగ:
  hb_సమితి_t సందర్శించిన_script;
  hb_సమితి_t సందర్శించిన_langsys;
  గుర్తులేని పూర్ణాంకం script_లెక్క;
  గుర్తులేని పూర్ణాంకం langsys_లెక్క;
  గుర్తులేని పూర్ణాంకం feature_సూచీ_లెక్క;
};

స్థిర శూన్య
langsys_collect_features (hb_collect_features_context_t *c,
			  స్థిరరాశి OT::LangSys  &l)
{
  ఒకవేళ (c->సందర్శించిన (l)) తిరిగి;

  ఒకవేళ (!c->has_feature_filter)
  {
    /* All features. */
    ఒకవేళ (l.has_required_feature () && !c->సందర్శించిన_feature_indices (1))
      c->feature_indices->కూడు (l.get_required_feature_సూచీ ());

    // TODO(garretrieger): filter out indices >= feature count?
    ఒకవేళ (!c->సందర్శించిన_feature_indices (l.feature_సూచీ_2.len))
      l.కూడు_feature_indexes_గమ్యం (c->feature_indices);
  }
  లేకపోతే
  {
    ఒకవేళ (c->feature_indices_filter.is_empty()) తిరిగి;
    గుర్తులేని పూర్ణాంకం అంకె_features = l.get_feature_లెక్క ();
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < అంకె_features; i++)
    {
      గుర్తులేని పూర్ణాంకం feature_సూచీ = l.get_feature_సూచీ (i);
      ఒకవేళ (!c->feature_indices_filter.has (feature_సూచీ)) కొనసాగు;

      c->feature_indices->కూడు (feature_సూచీ);
      c->feature_indices_filter.del (feature_సూచీ);
    }
  }
}

స్థిర శూన్య
script_collect_features (hb_collect_features_context_t *c,
			 స్థిరరాశి OT::Script   &s,
			 స్థిరరాశి hb_ట్యాగ్_t *languages)
{
  ఒకవేళ (c->సందర్శించిన (s)) తిరిగి;

  ఒకవేళ (!languages)
  {
    /* All languages. */
    ఒకవేళ (s.has_default_lang_sys ())
      langsys_collect_features (c,
				s.get_default_lang_sys ());


    గుర్తులేని పూర్ణాంకం లెక్క = s.get_lang_sys_లెక్క ();
    కొరకు (గుర్తులేని పూర్ణాంకం language_సూచీ = 0; language_సూచీ < లెక్క; language_సూచీ++)
      langsys_collect_features (c,
				s.get_lang_sys (language_సూచీ));
  }
  లేకపోతే
  {
    కొరకు (; *languages; languages++)
    {
      గుర్తులేని పూర్ణాంకం language_సూచీ;
      ఒకవేళ (s.కనుగొను_lang_sys_సూచీ (*languages, &language_సూచీ))
	langsys_collect_features (c,
				  s.get_lang_sys (language_సూచీ));

    }
  }
}


/**
 * hb_ot_layout_collect_features:
 * @face: #hb_face_t to work upon
 * @table_tag: #HB_OT_TAG_GSUB or #HB_OT_TAG_GPOS
 * @scripts: (nullable) (array zero-terminated=1): The array of scripts to collect features for,
 *   terminated by %HB_TAG_NONE
 * @languages: (nullable) (array zero-terminated=1): The array of languages to collect features for,
 *   terminated by %HB_TAG_NONE
 * @features: (nullable) (array zero-terminated=1): The array of features to collect,
 *   terminated by %HB_TAG_NONE
 * @feature_indexes: (out): The set of feature indexes found for the query
 *
 * Fetches a list of all feature indexes in the specified face's GSUB table
 * or GPOS table, underneath the specified scripts, languages, and features.
 * If no list of scripts is provided, all scripts will be queried. If no list
 * of languages is provided, all languages will be queried. If no list of
 * features is provided, all features will be queried.
 *
 * Since: 1.8.5
 **/
శూన్య
hb_ot_layout_collect_features (hb_face_t      *face,
			       hb_ట్యాగ్_t        పట్టిక_ట్యాగ్_2,
			       స్థిరరాశి hb_ట్యాగ్_t *scripts,
			       స్థిరరాశి hb_ట్యాగ్_t *languages,
			       స్థిరరాశి hb_ట్యాగ్_t *features,
			       hb_సమితి_t       *feature_indexes /*బహిర్గతం*/)
{
  hb_collect_features_context_t c (face, పట్టిక_ట్యాగ్_2, feature_indexes, features);
  ఒకవేళ (!scripts)
  {
    /* All scripts. */
    గుర్తులేని పూర్ణాంకం లెక్క = c.g.get_script_లెక్క ();
    కొరకు (గుర్తులేని పూర్ణాంకం script_సూచీ = 0; script_సూచీ < లెక్క; script_సూచీ++)
      script_collect_features (&c,
			       c.g.get_script (script_సూచీ),
			       languages);
  }
  లేకపోతే
  {
    కొరకు (; *scripts; scripts++)
    {
      గుర్తులేని పూర్ణాంకం script_సూచీ;
      ఒకవేళ (c.g.కనుగొను_script_సూచీ (*scripts, &script_సూచీ))
	script_collect_features (&c,
				 c.g.get_script (script_సూచీ),
				 languages);
    }
  }
}

/**
 * hb_ot_layout_collect_features_map:
 * @face: #hb_face_t to work upon
 * @table_tag: #HB_OT_TAG_GSUB or #HB_OT_TAG_GPOS
 * @script_index: The index of the requested script tag
 * @language_index: The index of the requested language tag
 * @feature_map: (out): The map of feature tag to feature index.
 *
 * Fetches the mapping from feature tags to feature indexes for
 * the specified script and language.
 *
 * Since: 8.1.0
 **/
శూన్య
hb_ot_layout_collect_features_map (hb_face_t      *face,
				   hb_ట్యాగ్_t        పట్టిక_ట్యాగ్_2,
				   గుర్తులేని        script_సూచీ,
				   గుర్తులేని        language_సూచీ,
				   hb_map_t       *feature_map /*బహిర్గతం*/)
{
  స్థిరరాశి OT::GSUBGPOS &g = get_gsubgpos_పట్టిక (face, పట్టిక_ట్యాగ్_2);
  స్థిరరాశి OT::LangSys &l = g.get_script (script_సూచీ).get_lang_sys (language_సూచీ);

  గుర్తులేని పూర్ణాంకం లెక్క = l.get_feature_indexes (0, శూన్యసూచిక, శూన్యసూచిక);
  feature_map->alloc (లెక్క);

  /* Loop in reverse, such that earlier entries win. That emulates
   * a linear search, which seems to be what other implementations do.
   * We found that with arialuni_t.ttf, the "ur" language system has
   * duplicate features, and the earlier ones work but not later ones.
   */
  కొరకు (గుర్తులేని పూర్ణాంకం i = లెక్క; i; i--)
  {
    గుర్తులేని feature_సూచీ = 0;
    గుర్తులేని feature_లెక్క_2 = 1;
    l.get_feature_indexes (i - 1, &feature_లెక్క_2, &feature_సూచీ);
    ఒకవేళ (!feature_లెక్క_2)
      ఆపు;
    hb_ట్యాగ్_t feature_ట్యాగ్ = g.get_feature_ట్యాగ్ (feature_సూచీ);
    feature_map->సమితి (feature_ట్యాగ్, feature_సూచీ);
  }
}


/**
 * hb_ot_layout_collect_lookups:
 * @face: #hb_face_t to work upon
 * @table_tag: #HB_OT_TAG_GSUB or #HB_OT_TAG_GPOS
 * @scripts: (nullable) (array zero-terminated=1): The array of scripts to collect lookups for,
 *   terminated by %HB_TAG_NONE
 * @languages: (nullable) (array zero-terminated=1): The array of languages to collect lookups for,
 *   terminated by %HB_TAG_NONE
 * @features: (nullable) (array zero-terminated=1): The array of features to collect lookups for,
 *   terminated by %HB_TAG_NONE
 * @lookup_indexes: (out): The array of lookup indexes found for the query
 *
 * Fetches a list of all feature-lookup indexes in the specified face's GSUB
 * table or GPOS table, underneath the specified scripts, languages, and
 * features. If no list of scripts is provided, all scripts will be queried.
 * If no list of languages is provided, all languages will be queried. If no
 * list of features is provided, all features will be queried.
 *
 * Since: 0.9.8
 **/
శూన్య
hb_ot_layout_collect_lookups (hb_face_t      *face,
			      hb_ట్యాగ్_t        పట్టిక_ట్యాగ్_2,
			      స్థిరరాశి hb_ట్యాగ్_t *scripts,
			      స్థిరరాశి hb_ట్యాగ్_t *languages,
			      స్థిరరాశి hb_ట్యాగ్_t *features,
			      hb_సమితి_t       *lookup_indexes /*బహిర్గతం*/)
{
  స్థిరరాశి OT::GSUBGPOS &g = get_gsubgpos_పట్టిక (face, పట్టిక_ట్యాగ్_2);

  hb_సమితి_t feature_indexes;
  hb_ot_layout_collect_features (face, పట్టిక_ట్యాగ్_2, scripts, languages, features, &feature_indexes);

  కొరకు (స్వయంచాలక feature_సూచీ : feature_indexes)
    g.get_feature (feature_సూచీ).కూడు_lookup_indexes_గమ్యం (lookup_indexes);

  g.feature_variation_collect_lookups (&feature_indexes, శూన్యసూచిక, lookup_indexes);
}


#నిర్వచితం_కాకపోతే HB_NO_LAYOUT_COLLECT_GLYPHS
/**
 * hb_ot_layout_lookup_collect_glyphs:
 * @face: #hb_face_t to work upon
 * @table_tag: #HB_OT_TAG_GSUB or #HB_OT_TAG_GPOS
 * @lookup_index: The index of the feature lookup to query
 * @glyphs_before: (out) (nullable): Array of glyphs preceding the substitution range
 * @glyphs_input: (out) (nullable): Array of input glyphs that would be substituted by the lookup
 * @glyphs_after: (out) (nullable): Array of glyphs following the substitution range
 * @glyphs_output: (out) (nullable): Array of glyphs that would be the substituted output of the lookup
 *
 * Fetches a list of all glyphs affected by the specified lookup in the
 * specified face's GSUB table or GPOS table.
 *
 * Since: 0.9.7
 **/
శూన్య
hb_ot_layout_lookup_collect_glyphs (hb_face_t    *face,
				    hb_ట్యాగ్_t      పట్టిక_ట్యాగ్_2,
				    గుర్తులేని పూర్ణాంకం  lookup_సూచీ,
				    hb_సమితి_t     *glyphs_before, /* OUT.  May be NULL */
				    hb_సమితి_t     *glyphs_ఆదానం,  /* OUT.  May be NULL */
				    hb_సమితి_t     *glyphs_after,  /* OUT.  May be NULL */
				    hb_సమితి_t     *glyphs_output  /* OUT.  May be NULL */)
{
  OT::hb_collect_glyphs_context_t c (face,
				     glyphs_before,
				     glyphs_ఆదానం,
				     glyphs_after,
				     glyphs_output);

  ఎంపిక (పట్టిక_ట్యాగ్_2)
  {
    స్థితి HB_OT_ట్యాగ్_GSUB:
    {
      స్థిరరాశి OT::SubstLookup& l = face->పట్టిక.GSUB->పట్టిక->get_lookup (lookup_సూచీ);
      l.collect_glyphs (&c);
      తిరిగి;
    }
    స్థితి HB_OT_ట్యాగ్_GPOS:
    {
      స్థిరరాశి OT::PosLookup& l = face->పట్టిక.GPOS->పట్టిక->get_lookup (lookup_సూచీ);
      l.collect_glyphs (&c);
      తిరిగి;
    }
  }
}
#అయితే_ముగింపు


/* Variations support */

తార్కిక
hb_ot_layout_పట్టిక_get_feature_variations_state (hb_face_t             *face,
						   hb_ట్యాగ్_t               పట్టిక_ట్యాగ్_2,
						   స్థిరరాశి పూర్ణాంకం             *coords,
						   గుర్తులేని పూర్ణాంకం           అంకె_coords,
						   గుర్తులేని పూర్ణాంకం          *variations_సూచీ,
						   hb_సదిశ_t<గుర్తులేని> *lookup_conditions)
{
  స్థిరరాశి OT::GSUBGPOS &g = get_gsubgpos_పట్టిక (face, పట్టిక_ట్యాగ్_2);
  స్థిరరాశి OT::GDEF &gdef = *face->పట్టిక.GDEF->పట్టిక;

  స్వయంచాలక instancer = OT::అంశం_Var_Store_Instancer (&gdef.get_var_store (), శూన్యసూచిక,
					      hb_array (coords, అంకె_coords));

  తిరిగి g.get_feature_variations_state (coords,
					 అంకె_coords,
					 variations_సూచీ,
					 &instancer,
					 lookup_conditions);
}

తార్కిక
_hb_ot_layout_feature_get_lookup_variations (hb_face_t *face,
                                             hb_ట్యాగ్_t పట్టిక_ట్యాగ్_2,
                                             గుర్తులేని feature_సూచీ,
                                             గుర్తులేని variations_సూచీ,
                                             hb_sorted_array_t<స్థిరరాశి గుర్తులేని> conditions,
                                             hb_సమితి_t *lookup_indexes)
{
  తిరిగి get_gsubgpos_పట్టిక (face, పట్టిక_ట్యాగ్_2).get_lookup_variations (
      feature_సూచీ, variations_సూచీ, conditions, lookup_indexes);
}


/**
 * hb_ot_layout_table_find_feature_variations:
 * @face: #hb_face_t to work upon
 * @table_tag: #HB_OT_TAG_GSUB or #HB_OT_TAG_GPOS
 * @coords: The variation coordinates to query
 * @num_coords: The number of variation coordinates
 * @variations_index: (out): The array of feature variations found for the query
 *
 * Fetches a list of feature variations in the specified face's GSUB table
 * or GPOS table, at the specified variation coordinates.
 *
 * Return value: `true` if feature variations were found, `false` otherwise.
 *
 * Since: 1.4.0
 *
 **/
hb_bool_t
hb_ot_layout_పట్టిక_కనుగొను_feature_variations (hb_face_t    *face,
					    hb_ట్యాగ్_t      పట్టిక_ట్యాగ్_2,
					    స్థిరరాశి పూర్ణాంకం    *coords,
					    గుర్తులేని పూర్ణాంకం  అంకె_coords,
					    గుర్తులేని పూర్ణాంకం *variations_సూచీ /* out */)
{
  స్థిరరాశి OT::GSUBGPOS &g = get_gsubgpos_పట్టిక (face, పట్టిక_ట్యాగ్_2);
  స్థిరరాశి OT::GDEF &gdef = *face->పట్టిక.GDEF->పట్టిక;

  స్వయంచాలక instancer = OT::అంశం_Var_Store_Instancer(&gdef.get_var_store(), శూన్యసూచిక,
					     hb_array (coords, అంకె_coords));

  తిరిగి g.కనుగొను_variations_సూచీ (coords, అంకె_coords, variations_సూచీ, &instancer);
}


/**
 * hb_ot_layout_feature_with_variations_get_lookups:
 * @face: #hb_face_t to work upon
 * @table_tag: #HB_OT_TAG_GSUB or #HB_OT_TAG_GPOS
 * @feature_index: The index of the feature to query
 * @variations_index: The index of the feature variation to query
 * @start_offset: offset of the first lookup to retrieve
 * @lookup_count: (inout) (nullable): Input = the maximum number of lookups to return;
 *                Output = the actual number of lookups returned (may be zero)
 * @lookup_indexes: (out) (array length=lookup_count) (nullable): The array of lookups found for the query
 *
 * Fetches a list of all lookups enumerated for the specified feature, in
 * the specified face's GSUB table or GPOS table, enabled at the specified
 * variations index. The list returned will begin at the offset provided.
 *
 * Return value: Total number of lookups.
 *
 * Since: 1.4.0
 *
 **/
గుర్తులేని పూర్ణాంకం
hb_ot_layout_feature_with_variations_get_lookups (hb_face_t    *face,
						  hb_ట్యాగ్_t      పట్టిక_ట్యాగ్_2,
						  గుర్తులేని పూర్ణాంకం  feature_సూచీ,
						  గుర్తులేని పూర్ణాంకం  variations_సూచీ,
						  గుర్తులేని పూర్ణాంకం  ప్రారంభం_స్థానభ్రంశం,
						  గుర్తులేని పూర్ణాంకం *lookup_లెక్క /* IN/OUT */,
						  గుర్తులేని పూర్ణాంకం *lookup_indexes /*బహిర్గతం*/)
{
  static_assert ((OT::FeatureVariations::NOT_దొరికింది_సూచీ == HB_OT_LAYOUT_NO_VARIATIONS_సూచీ), "");
  స్థిరరాశి OT::GSUBGPOS &g = get_gsubgpos_పట్టిక (face, పట్టిక_ట్యాగ్_2);

  స్థిరరాశి OT::Feature &f = g.get_feature_variation (feature_సూచీ, variations_సూచీ);

  తిరిగి f.get_lookup_indexes (ప్రారంభం_స్థానభ్రంశం, lookup_లెక్క, lookup_indexes);
}


/*
 * OT::GSUB
 */


/**
 * hb_ot_layout_has_substitution:
 * @face: #hb_face_t to work upon
 *
 * Tests whether the specified face includes any GSUB substitutions.
 *
 * Return value: `true` if data found, `false` otherwise
 *
 * Since: 0.6.0
 *
 **/
hb_bool_t
hb_ot_layout_has_substitution (hb_face_t *face)
{
  తిరిగి face->పట్టిక.GSUB->పట్టిక->has_దత్తాంశం ();
}


/**
 * hb_ot_layout_lookup_would_substitute:
 * @face: #hb_face_t to work upon
 * @lookup_index: The index of the lookup to query
 * @glyphs: The sequence of glyphs to query for substitution
 * @glyphs_length: The length of the glyph sequence
 * @zero_context: #hb_bool_t indicating whether pre-/post-context are disallowed
 * in substitutions
 *
 * Tests whether a specified lookup in the specified face would
 * trigger a substitution on the given glyph sequence.
 *
 * Return value: `true` if a substitution would be triggered, `false` otherwise
 *
 * Since: 0.9.7
 **/
hb_bool_t
hb_ot_layout_lookup_would_substitute (hb_face_t            *face,
				      గుర్తులేని పూర్ణాంకం          lookup_సూచీ,
				      స్థిరరాశి hb_codepoint_t *glyphs,
				      గుర్తులేని పూర్ణాంకం          glyphs_నిడివి,
				      hb_bool_t             zero_context)
{
  స్వయంచాలక &gsub = face->పట్టిక.GSUB;
  ఒకవేళ (unlikely (lookup_సూచీ >= gsub->lookup_లెక్క)) తిరిగి అబద్ధం;
  OT::hb_would_apply_context_t c (face, glyphs, glyphs_నిడివి, (తార్కిక) zero_context);

  స్థిరరాశి OT::SubstLookup& l = gsub->పట్టిక->get_lookup (lookup_సూచీ);
  స్వయంచాలక *accel = gsub->get_accel (lookup_సూచీ);
  తిరిగి accel && l.would_apply (&c, accel);
}


/**
 * hb_ot_layout_substitute_start:
 * @font: #hb_font_t to use
 * @buffer: #hb_buffer_t buffer to work upon
 *
 * Called before substitution lookups are performed, to ensure that glyph
 * class and other properties are set on the glyphs in the buffer.
 *
 **/
శూన్య
hb_ot_layout_substitute_ప్రారంభం (hb_font_t    *font,
			       hb_బఫరు_t  *బఫరు)
{
  hb_ot_layout_సమితి_glyph_props<అబద్ధం> (font, బఫరు);
}

శూన్య
hb_ot_layout_substitute_ప్రారంభం_with_digest (hb_font_t    *font,
					   hb_బఫరు_t  *బఫరు)
{
  బఫరు->digest = hb_సమితి_digest_t ();
  hb_ot_layout_సమితి_glyph_props<నిజం> (font, బఫరు);
}

/**
 * hb_ot_layout_lookup_substitute_closure:
 * @face: #hb_face_t to work upon
 * @lookup_index: index of the feature lookup to query
 * @glyphs: (out): Array of glyphs comprising the transitive closure of the lookup
 *
 * Compute the transitive closure of glyphs needed for a
 * specified lookup.
 *
 * Since: 0.9.7
 **/
శూన్య
hb_ot_layout_lookup_substitute_సంవృతం (hb_face_t    *face,
					గుర్తులేని పూర్ణాంకం  lookup_సూచీ,
					hb_సమితి_t     *glyphs /*బహిర్గతం*/)
{
  hb_map_t పూర్తి_lookups_glyph_లెక్క;
  hb_hashmap_t<గుర్తులేని, hb::unique_ptr<hb_సమితి_t>> పూర్తి_lookups_glyph_సమితి;
  OT::hb_సంవృతం_context_t c (face, glyphs, &పూర్తి_lookups_glyph_లెక్క, &పూర్తి_lookups_glyph_సమితి);

  స్థిరరాశి OT::SubstLookup& l = face->పట్టిక.GSUB->పట్టిక->get_lookup (lookup_సూచీ);

  l.సంవృతం (&c, lookup_సూచీ);
}

/**
 * hb_ot_layout_lookups_substitute_closure:
 * @face: #hb_face_t to work upon
 * @lookups: The set of lookups to query
 * @glyphs: (out): Array of glyphs comprising the transitive closure of the lookups
 *
 * Compute the transitive closure of glyphs needed for all of the
 * provided lookups.
 *
 * Since: 1.8.1
 **/
శూన్య
hb_ot_layout_lookups_substitute_సంవృతం (hb_face_t      *face,
					 స్థిరరాశి hb_సమితి_t *lookups,
					 hb_సమితి_t       *glyphs /*బహిర్గతం*/)
{
  hb_map_t పూర్తి_lookups_glyph_లెక్క;
  hb_hashmap_t<గుర్తులేని, hb::unique_ptr<hb_సమితి_t>> పూర్తి_lookups_glyph_సమితి;
  OT::hb_సంవృతం_context_t c (face, glyphs, &పూర్తి_lookups_glyph_లెక్క, &పూర్తి_lookups_glyph_సమితి);
  స్థిరరాశి GSUB& gsub = *face->పట్టిక.GSUB->పట్టిక;

  గుర్తులేని పూర్ణాంకం iteration_లెక్క = 0;
  గుర్తులేని పూర్ణాంకం glyphs_నిడివి;
  చేయి
  {
    c.reset_lookup_సందర్శించు_లెక్క ();
    glyphs_నిడివి = glyphs->get_population ();
    ఒకవేళ (lookups)
    {
      కొరకు (స్వయంచాలక lookup_సూచీ : *lookups)
	gsub.get_lookup (lookup_సూచీ).సంవృతం (&c, lookup_సూచీ);
    }
    లేకపోతే
    {
      కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < gsub.get_lookup_లెక్క (); i++)
	gsub.get_lookup (i).సంవృతం (&c, i);
    }
  } అయితే (iteration_లెక్క++ <= HB_సంవృతం_MAX_STAGES &&
	   glyphs_నిడివి != glyphs->get_population ());
}

/*
 * GPOS
 */


/**
 * hb_ot_layout_has_positioning:
 * @face: #hb_face_t to work upon
 *
 * Tests whether the specified face includes any GPOS positioning.
 *
 * Return value: `true` if the face has GPOS data, `false` otherwise
 *
 **/
hb_bool_t
hb_ot_layout_has_positioning (hb_face_t *face)
{
  తిరిగి face->పట్టిక.GPOS->పట్టిక->has_దత్తాంశం ();
}

/**
 * hb_ot_layout_position_start:
 * @font: #hb_font_t to use
 * @buffer: #hb_buffer_t buffer to work upon
 *
 * Called before positioning lookups are performed, to ensure that glyph
 * attachment types and glyph-attachment chains are set for the glyphs in the buffer.
 *
 **/
శూన్య
hb_ot_layout_position_ప్రారంభం (hb_font_t *font, hb_బఫరు_t *బఫరు)
{
  GPOS::position_ప్రారంభం (font, బఫరు);
}


/**
 * hb_ot_layout_position_finish_advances:
 * @font: #hb_font_t to use
 * @buffer: #hb_buffer_t buffer to work upon
 *
 * Called after positioning lookups are performed, to finish glyph advances.
 *
 **/
శూన్య
hb_ot_layout_position_ముగింపు_advances (hb_font_t *font, hb_బఫరు_t *బఫరు)
{
  GPOS::position_ముగింపు_advances (font, బఫరు);
}

/**
 * hb_ot_layout_position_finish_offsets:
 * @font: #hb_font_t to use
 * @buffer: #hb_buffer_t buffer to work upon
 *
 * Called after positioning lookups are performed, to finish glyph offsets.
 *
 **/
శూన్య
hb_ot_layout_position_ముగింపు_offsets (hb_font_t *font, hb_బఫరు_t *బఫరు)
{
  GPOS::position_ముగింపు_offsets (font, బఫరు);
}


#నిర్వచితం_కాకపోతే HB_NO_LAYOUT_FEATURE_PARAMS
/**
 * hb_ot_layout_get_size_params:
 * @face: #hb_face_t to work upon
 * @design_size: (out): The design size of the face
 * @subfamily_id: (out): The identifier of the face within the font subfamily
 * @subfamily_name_id: (out): The ‘name’ table name ID of the face within the font subfamily
 * @range_start: (out): The minimum size of the recommended size range for the face
 * @range_end: (out): The maximum size of the recommended size range for the face
 *
 * Fetches optical-size feature data (i.e., the `size` feature from GPOS). Note that
 * the subfamily_id and the subfamily name string (accessible via the subfamily_name_id)
 * as used here are defined as pertaining only to fonts within a font family that differ
 * specifically in their respective size ranges; other ways to differentiate fonts within
 * a subfamily are not covered by the `size` feature.
 *
 * For more information on this distinction, see the [`size` feature documentation](
 * https://docs.microsoft.com/en-us/typography/opentype/spec/features_pt#tag-size).
 *
 * Return value: `true` if data found, `false` otherwise
 *
 * Since: 0.9.10
 **/
hb_bool_t
hb_ot_layout_get_కొలత_params (hb_face_t       *face,
			      గుర్తులేని పూర్ణాంకం    *design_కొలత_2,       /* OUT.  May be NULL */
			      గుర్తులేని పూర్ణాంకం    *subfamily_id,      /* OUT.  May be NULL */
			      hb_ot_name_id_t *subfamily_name_id, /* OUT.  May be NULL */
			      గుర్తులేని పూర్ణాంకం    *range_ప్రారంభం_2,       /* OUT.  May be NULL */
			      గుర్తులేని పూర్ణాంకం    *range_చివర_2          /* OUT.  May be NULL */)
{
  స్థిరరాశి GPOS &gpos = *face->పట్టిక.GPOS->పట్టిక;
  స్థిరరాశి hb_ట్యాగ్_t ట్యాగ్_2 = HB_ట్యాగ్ ('s','i','z','e');

  గుర్తులేని పూర్ణాంకం అంకె_features = gpos.get_feature_లెక్క ();
  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < అంకె_features; i++)
  {
    ఒకవేళ (ట్యాగ్_2 == gpos.get_feature_ట్యాగ్ (i))
    {
      స్థిరరాశి OT::Feature &f = gpos.get_feature (i);
      స్థిరరాశి OT::Feature_Params_కొలత &params = f.get_feature_params ().get_కొలత_params (ట్యాగ్_2);

      ఒకవేళ (params.design_కొలత)
      {
	ఒకవేళ (design_కొలత_2) *design_కొలత_2 = params.design_కొలత;
	ఒకవేళ (subfamily_id) *subfamily_id = params.subfamilyID;
	ఒకవేళ (subfamily_name_id) *subfamily_name_id = params.subfamilyNameID;
	ఒకవేళ (range_ప్రారంభం_2) *range_ప్రారంభం_2 = params.range_ప్రారంభం;
	ఒకవేళ (range_చివర_2) *range_చివర_2 = params.range_చివర;

	తిరిగి నిజం;
      }
    }
  }

  ఒకవేళ (design_కొలత_2) *design_కొలత_2 = 0;
  ఒకవేళ (subfamily_id) *subfamily_id = 0;
  ఒకవేళ (subfamily_name_id) *subfamily_name_id = HB_OT_NAME_ID_INVALID;
  ఒకవేళ (range_ప్రారంభం_2) *range_ప్రారంభం_2 = 0;
  ఒకవేళ (range_చివర_2) *range_చివర_2 = 0;

  తిరిగి అబద్ధం;
}


/**
 * hb_ot_layout_feature_get_name_ids:
 * @face: #hb_face_t to work upon
 * @table_tag: table tag to query, "GSUB" or "GPOS".
 * @feature_index: index of feature to query.
 * @label_id: (out) (nullable): The ‘name’ table name ID that specifies a string
 *            for a user-interface label for this feature.
 * @tooltip_id: (out) (nullable): The ‘name’ table name ID that specifies a string
 *              that an application can use for tooltip text for this
 *              feature.
 * @sample_id: (out) (nullable): The ‘name’ table name ID that specifies sample text
 *             that illustrates the effect of this feature.
 * @num_named_parameters: (out) (nullable):  Number of named parameters.
 * @first_param_id: (out) (nullable): The first ‘name’ table name ID used to specify
 *                  strings for user-interface labels for the feature
 *                  parameters. (Must be zero if numParameters is zero.)
 *
 * Fetches name indices from feature parameters for "Stylistic Set" ('ssXX') or
 * "Character Variant" ('cvXX') features.
 *
 * Return value: `true` if data found, `false` otherwise
 *
 * Since: 2.0.0
 **/
hb_bool_t
hb_ot_layout_feature_get_name_ids (hb_face_t       *face,
				   hb_ట్యాగ్_t         పట్టిక_ట్యాగ్_2,
				   గుర్తులేని పూర్ణాంకం     feature_సూచీ,
				   hb_ot_name_id_t *label_id,             /* OUT.  May be NULL */
				   hb_ot_name_id_t *tooltip_id,           /* OUT.  May be NULL */
				   hb_ot_name_id_t *sample_id,            /* OUT.  May be NULL */
				   గుర్తులేని పూర్ణాంకం    *అంకె_named_parameters, /* OUT.  May be NULL */
				   hb_ot_name_id_t *మొదటి_param_id        /* OUT.  May be NULL */)
{
  స్థిరరాశి OT::GSUBGPOS &g = get_gsubgpos_పట్టిక (face, పట్టిక_ట్యాగ్_2);

  hb_ట్యాగ్_t feature_ట్యాగ్ = g.get_feature_ట్యాగ్ (feature_సూచీ);
  స్థిరరాశి OT::Feature &f = g.get_feature (feature_సూచీ);

  స్థిరరాశి OT::FeatureParams &feature_params = f.get_feature_params ();
  ఒకవేళ (&feature_params != &Null (OT::FeatureParams))
  {
    స్థిరరాశి OT::Feature_Params_Stylistic_సమితి& ss_params =
      feature_params.get_stylistic_సమితి_params (feature_ట్యాగ్);
    ఒకవేళ (&ss_params != &Null (OT::Feature_Params_Stylistic_సమితి)) /* ssXX */
    {
      ఒకవేళ (label_id) *label_id = ss_params.uiNameID;
      // ssXX features don't have the rest
      ఒకవేళ (tooltip_id) *tooltip_id = HB_OT_NAME_ID_INVALID;
      ఒకవేళ (sample_id) *sample_id = HB_OT_NAME_ID_INVALID;
      ఒకవేళ (అంకె_named_parameters) *అంకె_named_parameters = 0;
      ఒకవేళ (మొదటి_param_id) *మొదటి_param_id = HB_OT_NAME_ID_INVALID;
      తిరిగి నిజం;
    }
    స్థిరరాశి OT::FeatureParamsCharacterVariants& cv_params =
      feature_params.get_character_variants_params (feature_ట్యాగ్);
    ఒకవేళ (&cv_params != &Null (OT::FeatureParamsCharacterVariants)) /* cvXX */
    {
      ఒకవేళ (label_id) *label_id = cv_params.featUILableNameID;
      ఒకవేళ (tooltip_id) *tooltip_id = cv_params.feat_UITooltip_పాఠ్యం_Name_ID;
      ఒకవేళ (sample_id) *sample_id = cv_params.sample_పాఠ్యం_Name_ID;
      ఒకవేళ (అంకె_named_parameters) *అంకె_named_parameters = cv_params.అంకె_Named_Parameters;
      ఒకవేళ (మొదటి_param_id) *మొదటి_param_id = cv_params.మొదటి_Param_UILabel_Name_ID;
      తిరిగి నిజం;
    }
  }

  ఒకవేళ (label_id) *label_id = HB_OT_NAME_ID_INVALID;
  ఒకవేళ (tooltip_id) *tooltip_id = HB_OT_NAME_ID_INVALID;
  ఒకవేళ (sample_id) *sample_id = HB_OT_NAME_ID_INVALID;
  ఒకవేళ (అంకె_named_parameters) *అంకె_named_parameters = 0;
  ఒకవేళ (మొదటి_param_id) *మొదటి_param_id = HB_OT_NAME_ID_INVALID;
  తిరిగి అబద్ధం;
}
/**
 * hb_ot_layout_feature_get_characters:
 * @face: #hb_face_t to work upon
 * @table_tag: table tag to query, "GSUB" or "GPOS".
 * @feature_index: index of feature to query.
 * @start_offset: offset of the first character to retrieve
 * @char_count: (inout) (nullable): Input = the maximum number of characters to return;
 *              Output = the actual number of characters returned (may be zero)
 * @characters: (out caller-allocates) (array length=char_count) (nullable): A buffer pointer.
 *              The Unicode codepoints of the characters for which this feature provides
 *               glyph variants.
 *
 * Fetches a list of the characters defined as having a variant under the specified
 * "Character Variant" ("cvXX") feature tag.
 *
 * Return value: Number of total sample characters in the cvXX feature.
 *
 * Since: 2.0.0
 **/
గుర్తులేని పూర్ణాంకం
hb_ot_layout_feature_get_characters (hb_face_t      *face,
				     hb_ట్యాగ్_t        పట్టిక_ట్యాగ్_2,
				     గుర్తులేని పూర్ణాంకం    feature_సూచీ,
				     గుర్తులేని పూర్ణాంకం    ప్రారంభం_స్థానభ్రంశం,
				     గుర్తులేని పూర్ణాంకం   *char_లెక్క, /* IN/OUT.  May be NULL */
				     hb_codepoint_t *characters  /* OUT.     May be NULL */)
{
  స్థిరరాశి OT::GSUBGPOS &g = get_gsubgpos_పట్టిక (face, పట్టిక_ట్యాగ్_2);
  తిరిగి g.get_feature (feature_సూచీ)
	  .get_feature_params ()
	  .get_character_variants_params(g.get_feature_ట్యాగ్ (feature_సూచీ))
	  .get_characters (ప్రారంభం_స్థానభ్రంశం, char_లెక్క, characters);
}
#అయితే_ముగింపు


/*
 * Parts of different types are implemented here such that they have direct
 * access to GSUB/GPOS lookups.
 */


నిర్మాణం GSUBProxy
{
  స్థిర constexpr గుర్తులేని పట్టిక_సూచీ = 0u;
  స్థిర constexpr తార్కిక always_inplace = అబద్ధం;
  రకనిర్వచనం OT::SubstLookup Lookup;

  GSUBProxy (hb_face_t *face) :
    accel (*face->పట్టిక.GSUB) {}

  స్థిరరాశి GSUB::accelerator_t &accel;
};

నిర్మాణం GPOSProxy
{
  స్థిర constexpr గుర్తులేని పట్టిక_సూచీ = 1u;
  స్థిర constexpr తార్కిక always_inplace = నిజం;
  రకనిర్వచనం OT::PosLookup Lookup;

  GPOSProxy (hb_face_t *face) :
    accel (*face->పట్టిక.GPOS) {}

  స్థిరరాశి GPOS::accelerator_t &accel;
};


స్థిర అంతఃవరుస తార్కిక
apply_forward (OT::hb_ot_apply_context_t *c,
	       స్థిరరాశి OT::hb_ot_layout_lookup_accelerator_t &accel)
{
  తార్కిక use_hot_subtable_cache = accel.cache_enter (c);

  తార్కిక ret = అబద్ధం;
  hb_బఫరు_t *బఫరు = c->బఫరు;
  అయితే (బఫరు->successful)
  {
    hb_glyph_info_t *info = బఫరు->info;
    గుర్తులేని j = బఫరు->idx;
    అయితే (j < బఫరు->len &&
	   !(accel.digest.may_have (info[j].codepoint) &&
	     (info[j].ముసుగు & c->lookup_ముసుగు) &&
	     c->check_glyph_property (&info[j], c->lookup_props)))
      j++;
    ఒకవేళ (unlikely (j > బఫరు->idx && !బఫరు->తదుపరి_glyphs (j - బఫరు->idx)))
      ఆపు;
    ఒకవేళ (బఫరు->idx >= బఫరు->len)
      ఆపు;

    ఒకవేళ (accel.apply (c, use_hot_subtable_cache))
      ret = నిజం;
    లేకపోతే
      (శూన్య) బఫరు->తదుపరి_glyph ();
  }

  ఒకవేళ (use_hot_subtable_cache)
    accel.cache_leave (c);

  తిరిగి ret;
}

స్థిర అంతఃవరుస తార్కిక
apply_backward (OT::hb_ot_apply_context_t *c,
	       స్థిరరాశి OT::hb_ot_layout_lookup_accelerator_t &accel)
{
  తార్కిక ret = అబద్ధం;
  hb_బఫరు_t *బఫరు = c->బఫరు;
  చేయి
  {
    స్వయంచాలక &cur = బఫరు->cur();
    ఒకవేళ (accel.digest.may_have (cur.codepoint) &&
	(cur.ముసుగు & c->lookup_ముసుగు) &&
	c->check_glyph_property (&cur, c->lookup_props))
      ret |= accel.apply (c, అబద్ధం);

    /* The reverse lookup doesn't "advance" cursor (for good reason). */
    బఫరు->idx--;
  }
  అయితే ((పూర్ణాంకం) బఫరు->idx >= 0);
  తిరిగి ret;
}

మూస <రకం_పేరు Proxy>
స్థిర అంతఃవరుస తార్కిక
apply_string (OT::hb_ot_apply_context_t *c,
	      స్థిరరాశి రకం_పేరు Proxy::Lookup &lookup,
	      స్థిరరాశి OT::hb_ot_layout_lookup_accelerator_t &accel)
{
  hb_బఫరు_t *బఫరు = c->బఫరు;

  ఒకవేళ (unlikely (!బఫరు->len || !c->lookup_ముసుగు))
    తిరిగి అబద్ధం;

  తార్కిక ret = అబద్ధం;

  c->సమితి_lookup_props (lookup.get_props ());

  ఒకవేళ (likely (!lookup.is_తిరగేయి ()))
  {
    /* in/out forward substitution/positioning */
    ఒకవేళ (!Proxy::always_inplace)
      బఫరు->clear_output ();

    బఫరు->idx = 0;
    ret = apply_forward (c, accel);

    ఒకవేళ (!Proxy::always_inplace)
      బఫరు->సమకాలీకరణ ();
  }
  లేకపోతే
  {
    /* in-place backward substitution/positioning */
    ప్రతిజ్ఞా (!బఫరు->have_output);
    బఫరు->idx = బఫరు->len - 1;
    ret = apply_backward (c, accel);
  }

  తిరిగి ret;
}

మూస <రకం_పేరు Proxy>
అంతఃవరుస శూన్య hb_ot_map_t::apply (స్థిరరాశి Proxy &proxy,
				స్థిరరాశి hb_ot_shape_plan_t *plan,
				hb_font_t *font,
				hb_బఫరు_t *బఫరు) స్థిరరాశి
{
  స్థిరరాశి గుర్తులేని పూర్ణాంకం పట్టిక_సూచీ = proxy.పట్టిక_సూచీ;
  గుర్తులేని పూర్ణాంకం i = 0;

  స్వయంచాలక *font_దత్తాంశం = font->దత్తాంశం.ot.get ();
  స్వయంచాలక *var_store_cache = (OT::hb_scalar_cache_t *) font_దత్తాంశం;

  OT::hb_ot_apply_context_t c (పట్టిక_సూచీ, font, బఫరు, proxy.accel.get_blob (), var_store_cache);
  c.సమితి_recurse_func (Proxy::Lookup::మూస dispatch_recurse_func<OT::hb_ot_apply_context_t>);

  కొరకు (గుర్తులేని పూర్ణాంకం stage_సూచీ = 0; stage_సూచీ < stages[పట్టిక_సూచీ].నిడివి; stage_సూచీ++)
  {
    స్థిరరాశి stage_map_t *stage = &stages[పట్టిక_సూచీ][stage_సూచీ];
    కొరకు (; i < stage->చివరి_lookup; i++)
    {
      స్వయంచాలక &lookup = lookups[పట్టిక_సూచీ][i];

      గుర్తులేని పూర్ణాంకం lookup_సూచీ = lookup.సూచీ_2;

      స్వయంచాలక *accel = proxy.accel.get_accel (lookup_సూచీ);
      ఒకవేళ (unlikely (!accel)) కొనసాగు;

      ఒకవేళ (బఫరు->messaging () &&
	  !బఫరు->సందేశం (font, "start lookup %u feature '%c%c%c%c'", lookup_సూచీ, HB_UNTAG (lookup.feature_ట్యాగ్))) కొనసాగు;

      /* Only try applying the lookup if there is any overlap. */
      ఒకవేళ (accel->digest.may_intersect (బఫరు->digest) &&
	  ((బఫరు->flags & HB_బఫరు_FLAG_PRODUCE_UNSAFE_గమ్యం_CONCAT) ||
	   accel->digest_రెండవ.may_intersect (బఫరు->digest)))
      {
	c.సమితి_lookup_సూచీ (lookup_సూచీ);
	c.సమితి_lookup_ముసుగు (lookup.ముసుగు, అబద్ధం);
	c.సమితి_auto_zwj (lookup.auto_zwj, అబద్ధం);
	c.సమితి_auto_zwnj (lookup.auto_zwnj, అబద్ధం);
	c.సమితి_random (lookup.random);
	c.సమితి_చొప్పున_syllable (lookup.చొప్పున_syllable, అబద్ధం);
	/* apply_string's set_lookup_props initializes the iterators. */

	apply_string<Proxy> (&c,
			     proxy.accel.పట్టిక->get_lookup (lookup_సూచీ),
			     *accel);
      }
      లేకపోతే ఒకవేళ (బఫరు->messaging ())
	(శూన్య) బఫరు->సందేశం (font, "skipped lookup %u feature '%c%c%c%c' because no glyph matches", lookup_సూచీ, HB_UNTAG (lookup.feature_ట్యాగ్));

      ఒకవేళ (బఫరు->messaging ())
	(శూన్య) బఫరు->సందేశం (font, "end lookup %u feature '%c%c%c%c'", lookup_సూచీ, HB_UNTAG (lookup.feature_ట్యాగ్));
    }

    ఒకవేళ (stage->pause_func)
    {
      ఒకవేళ (stage->pause_func (plan, font, బఫరు))
      {
	/* Refresh working buffer digest since buffer changed. */
	బఫరు->update_digest ();
      }
    }
  }
}

శూన్య hb_ot_map_t::substitute (స్థిరరాశి hb_ot_shape_plan_t *plan, hb_font_t *font, hb_బఫరు_t *బఫరు) స్థిరరాశి
{
  GSUBProxy proxy (font->face);
  అక్షరం ట్యాగ్_2[5] = {0};
  hb_ట్యాగ్_గమ్యం_string (chosen_script[0], ట్యాగ్_2);
  ఒకవేళ (బఫరు->messaging () &&
      !బఫరు->సందేశం (font, "start table GSUB script tag '%s'", ట్యాగ్_2)) తిరిగి;
  apply (proxy, plan, font, బఫరు);
  ఒకవేళ (బఫరు->messaging ())
    (శూన్య) బఫరు->సందేశం (font, "end table GSUB script tag '%s'", ట్యాగ్_2);
}

శూన్య hb_ot_map_t::position (స్థిరరాశి hb_ot_shape_plan_t *plan, hb_font_t *font, hb_బఫరు_t *బఫరు) స్థిరరాశి
{
  GPOSProxy proxy (font->face);
  అక్షరం ట్యాగ్_2[5] = {0};
  hb_ట్యాగ్_గమ్యం_string (chosen_script[0], ట్యాగ్_2);
  ఒకవేళ (బఫరు->messaging () &&
      !బఫరు->సందేశం (font, "start table GPOS script tag '%s'", ట్యాగ్_2)) తిరిగి;
  apply (proxy, plan, font, బఫరు);
  ఒకవేళ (బఫరు->messaging ())
    (శూన్య) బఫరు->సందేశం (font, "end table GPOS script tag '%s'", ట్యాగ్_2);
}

శూన్య
hb_ot_layout_substitute_lookup (OT::hb_ot_apply_context_t *c,
				స్థిరరాశి OT::SubstLookup &lookup,
				స్థిరరాశి OT::hb_ot_layout_lookup_accelerator_t &accel)
{
  apply_string<GSUBProxy> (c, lookup, accel);
}

#నిర్వచితం_కాకపోతే HB_NO_పునాది

స్థిర శూన్య
ఎంచుకో_పునాది_ట్యాగులు (hb_script_t    script,
		  hb_language_t  language,
		  hb_ట్యాగ్_t      *script_ట్యాగ్,
		  hb_ట్యాగ్_t      *language_ట్యాగ్)
{
  hb_ట్యాగ్_t script_ట్యాగులు[HB_OT_MAX_ట్యాగులు_చొప్పున_SCRIPT];
  గుర్తులేని script_లెక్క = ARRAY_నిడివి (script_ట్యాగులు);

  hb_ట్యాగ్_t language_ట్యాగులు[HB_OT_MAX_ట్యాగులు_చొప్పున_LANGUAGE];
  గుర్తులేని language_లెక్క = ARRAY_నిడివి (language_ట్యాగులు);

  hb_ot_ట్యాగులు_from_script_and_language (script, language,
				       &script_లెక్క, script_ట్యాగులు,
				       &language_లెక్క, language_ట్యాగులు);

  *script_ట్యాగ్ = script_లెక్క ? script_ట్యాగులు[script_లెక్క - 1] : HB_OT_ట్యాగ్_DEFAULT_SCRIPT;
  *language_ట్యాగ్ = language_లెక్క ? language_ట్యాగులు[language_లెక్క - 1] : HB_OT_ట్యాగ్_DEFAULT_LANGUAGE;
}

/**
 * hb_ot_layout_get_font_extents:
 * @font: a font
 * @direction: text direction.
 * @script_tag:  script tag.
 * @language_tag: language tag.
 * @extents: (out) (nullable): font extents if found.
 *
 * Fetches script/language-specific font extents.  These values are
 * looked up in the `BASE` table's `MinMax` records.
 *
 * If no such extents are found, the default extents for the font are
 * fetched. As such, the return value of this function can for the
 * most part be ignored.  Note that the per-script/language extents
 * do not have a line-gap value, and the line-gap is set to zero in
 * that case.
 *
 * Return value: `true` if found script/language-specific font extents.
 *
 * Since: 8.0.0
 **/
hb_bool_t
hb_ot_layout_get_font_extents (hb_font_t         *font,
			       hb_direction_t     direction,
			       hb_ట్యాగ్_t           script_ట్యాగ్,
			       hb_ట్యాగ్_t           language_ట్యాగ్,
			       hb_font_extents_t *extents)
{
  hb_position_t min = 0, max = 0;
  ఒకవేళ (font->face->పట్టిక.పునాది_4->get_min_max (font, direction, script_ట్యాగ్, language_ట్యాగ్, HB_ట్యాగ్_NONE,
					   &min, &max))
  {
    ఒకవేళ (extents)
    {
      extents->ascender  = max;
      extents->descender = min;
      extents->పంక్తి_gap  = 0;
    }
    తిరిగి నిజం;
  }

  hb_font_get_extents_for_direction (font, direction, extents);
  తిరిగి అబద్ధం;
}

/**
 * hb_ot_layout_get_font_extents2:
 * @font: a font
 * @direction: text direction.
 * @script:  script.
 * @language: (nullable): language.
 * @extents: (out) (nullable): font extents if found.
 *
 * Fetches script/language-specific font extents.  These values are
 * looked up in the `BASE` table's `MinMax` records.
 *
 * If no such extents are found, the default extents for the font are
 * fetched. As such, the return value of this function can for the
 * most part be ignored.  Note that the per-script/language extents
 * do not have a line-gap value, and the line-gap is set to zero in
 * that case.
 *
 * This function is like hb_ot_layout_get_font_extents() but takes
 * #hb_script_t and #hb_language_t instead of OpenType #hb_tag_t.
 *
 * Return value: `true` if found script/language-specific font extents.
 *
 * Since: 8.0.0
 **/
hb_bool_t
hb_ot_layout_get_font_extents2 (hb_font_t         *font,
				hb_direction_t     direction,
				hb_script_t        script,
				hb_language_t      language,
				hb_font_extents_t *extents)
{
  hb_ట్యాగ్_t script_ట్యాగ్, language_ట్యాగ్;
  ఎంచుకో_పునాది_ట్యాగులు (script, language, &script_ట్యాగ్, &language_ట్యాగ్);
  తిరిగి hb_ot_layout_get_font_extents (font,
					direction,
					script_ట్యాగ్,
					language_ట్యాగ్,
					extents);
}

/**
 * hb_ot_layout_get_horizontal_baseline_tag_for_script:
 * @script: a script tag.
 *
 * Fetches the dominant horizontal baseline tag used by @script.
 *
 * Return value: dominant baseline tag for the @script.
 *
 * Since: 4.0.0
 **/
hb_ot_layout_baseline_ట్యాగ్_t
hb_ot_layout_get_horizontal_baseline_ట్యాగ్_for_script (hb_script_t script)
{
  /* Keep in sync with hb_ot_layout_get_baseline_with_fallback */
  ఎంపిక ((పూర్ణాంకం) script)
  {
    /* Unicode-1.1 additions */
    స్థితి HB_SCRIPT_BENGALI:
    స్థితి HB_SCRIPT_DEVANAGARI:
    స్థితి HB_SCRIPT_GUJARATI:
    స్థితి HB_SCRIPT_GURMUKHI:
    /* Unicode-2.0 additions */
    స్థితి HB_SCRIPT_TIBETAN:
    /* Unicode-4.0 additions */
    స్థితి HB_SCRIPT_LIMBU:
    /* Unicode-4.1 additions */
    స్థితి HB_SCRIPT_SYLOTI_NAGRI:
    /* Unicode-5.0 additions */
    స్థితి HB_SCRIPT_PHAGS_PA:
    /* Unicode-5.2 additions */
    స్థితి HB_SCRIPT_MEETEI_MAYEK:
    /* Unicode-6.1 additions */
    స్థితి HB_SCRIPT_SHARADA:
    స్థితి HB_SCRIPT_TAKRI:
    /* Unicode-7.0 additions */
    స్థితి HB_SCRIPT_MODI:
    స్థితి HB_SCRIPT_SIDDHAM:
    స్థితి HB_SCRIPT_TIRHUTA:
    /* Unicode-9.0 additions */
    స్థితి HB_SCRIPT_MARCHEN:
    స్థితి HB_SCRIPT_NEWA:
    /* Unicode-10.0 additions */
    స్థితి HB_SCRIPT_SOYOMBO:
    స్థితి HB_SCRIPT_ZANABAZAR_వర్గం:
    /* Unicode-11.0 additions */
    స్థితి HB_SCRIPT_DOGRA:
    స్థితి HB_SCRIPT_GUNJALA_GONDI:
    /* Unicode-12.0 additions */
    స్థితి HB_SCRIPT_NANDINAGARI:
      తిరిగి HB_OT_LAYOUT_BASELINE_ట్యాగ్_HANGING;

    /* Unicode-1.1 additions */
    స్థితి HB_SCRIPT_HANGUL:
    స్థితి HB_SCRIPT_HAN:
    స్థితి HB_SCRIPT_HIRAGANA:
    స్థితి HB_SCRIPT_KATAKANA:
    /* Unicode-3.0 additions */
    స్థితి HB_SCRIPT_BOPOMOFO:
    /* Unicode-9.0 additions */
    స్థితి HB_SCRIPT_TANGUT:
    /* Unicode-10.0 additions */
    స్థితి HB_SCRIPT_NUSHU:
    /* Unicode-13.0 additions */
    స్థితి HB_SCRIPT_KHITAN_SMALL_SCRIPT:
    /* Unicode-18.0 additions */
    స్థితి HB_SCRIPT_JURCHEN:
    స్థితి HB_SCRIPT_SEAL:
      తిరిగి HB_OT_LAYOUT_BASELINE_ట్యాగ్_IDEO_FACE_BOTTOM_OR_ఎడమ;

    అప్రమేయ:
      తిరిగి HB_OT_LAYOUT_BASELINE_ట్యాగ్_ROMAN;
  }
}

/**
 * hb_ot_layout_get_baseline:
 * @font: a font
 * @baseline_tag: a baseline tag
 * @direction: text direction.
 * @script_tag:  script tag.
 * @language_tag: language tag, currently unused.
 * @coord: (out) (nullable): baseline value if found.
 *
 * Fetches a baseline value from the font.
 *
 * Return value: `true` if found baseline value in the font.
 *
 * Since: 2.6.0
 **/
hb_bool_t
hb_ot_layout_get_baseline (hb_font_t                   *font,
			   hb_ot_layout_baseline_ట్యాగ్_t  baseline_ట్యాగ్,
			   hb_direction_t               direction,
			   hb_ట్యాగ్_t                     script_ట్యాగ్,
			   hb_ట్యాగ్_t                     language_ట్యాగ్,
			   hb_position_t               *coord        /* OUT.  May be NULL. */)
{
  తిరిగి font->face->పట్టిక.పునాది_4->get_baseline (font, baseline_ట్యాగ్, direction, script_ట్యాగ్, language_ట్యాగ్, coord);
}

/**
 * hb_ot_layout_get_baseline2:
 * @font: a font
 * @baseline_tag: a baseline tag
 * @direction: text direction.
 * @script:  script.
 * @language: (nullable): language, currently unused.
 * @coord: (out) (nullable): baseline value if found.
 *
 * Fetches a baseline value from the font.
 *
 * This function is like hb_ot_layout_get_baseline() but takes
 * #hb_script_t and #hb_language_t instead of OpenType #hb_tag_t.
 *
 * Return value: `true` if found baseline value in the font.
 *
 * Since: 8.0.0
 **/
hb_bool_t
hb_ot_layout_get_baseline2 (hb_font_t                   *font,
			    hb_ot_layout_baseline_ట్యాగ్_t  baseline_ట్యాగ్,
			    hb_direction_t               direction,
			    hb_script_t                  script,
			    hb_language_t                language,
			    hb_position_t               *coord        /* OUT.  May be NULL. */)
{
  hb_ట్యాగ్_t script_ట్యాగ్, language_ట్యాగ్;
  ఎంచుకో_పునాది_ట్యాగులు (script, language, &script_ట్యాగ్, &language_ట్యాగ్);
  తిరిగి hb_ot_layout_get_baseline (font,
				    baseline_ట్యాగ్,
				    direction,
				    script_ట్యాగ్,
				    language_ట్యాగ్,
				    coord);
}

/**
 * hb_ot_layout_get_baseline_with_fallback:
 * @font: a font
 * @baseline_tag: a baseline tag
 * @direction: text direction.
 * @script_tag:  script tag.
 * @language_tag: language tag, currently unused.
 * @coord: (out): baseline value if found.
 *
 * Fetches a baseline value from the font, and synthesizes
 * it if the font does not have it.
 *
 * Since: 4.0.0
 **/
శూన్య
hb_ot_layout_get_baseline_with_fallback (hb_font_t                   *font,
					 hb_ot_layout_baseline_ట్యాగ్_t  baseline_ట్యాగ్,
					 hb_direction_t               direction,
					 hb_ట్యాగ్_t                     script_ట్యాగ్,
					 hb_ట్యాగ్_t                     language_ట్యాగ్,
					 hb_position_t               *coord /*బహిర్గతం*/)
{
  ఒకవేళ (hb_ot_layout_get_baseline (font,
				 baseline_ట్యాగ్,
				 direction,
				 script_ట్యాగ్,
				 language_ట్యాగ్,
				 coord))
    తిరిగి;

  /* Synthesize missing baselines.
   * See https://www.w3.org/TR/css-inline-3/#baseline-synthesis-fonts
   */
  ఎంపిక (baseline_ట్యాగ్)
  {
  స్థితి HB_OT_LAYOUT_BASELINE_ట్యాగ్_ROMAN:
    *coord = 0; // FIXME origin ?
    ఆపు;

  స్థితి HB_OT_LAYOUT_BASELINE_ట్యాగ్_MATH:
    {
      hb_codepoint_t glyph;
      hb_glyph_extents_t extents;
      ఒకవేళ (HB_DIRECTION_IS_HORIZONTAL (direction) &&
	  (hb_font_get_nominal_glyph (font, 0x2212u, &glyph) ||
	   hb_font_get_nominal_glyph (font, '-', &glyph)) &&
	  hb_font_get_glyph_extents (font, glyph, &extents))
      {
	*coord = hb_saturate_కూడు (extents.y_bearing, extents.ఎత్తు / 2);
      }
      లేకపోతే
      {
	hb_position_t x_ఎత్తు = font->y_scale / 2;
#నిర్వచితం_కాకపోతే HB_NO_METRICS
	hb_ot_metrics_get_position_with_fallback (font, HB_OT_METRICS_ట్యాగ్_X_ఎత్తు, &x_ఎత్తు);
#అయితే_ముగింపు
	*coord = x_ఎత్తు / 2;
      }
    }
    ఆపు;

  స్థితి HB_OT_LAYOUT_BASELINE_ట్యాగ్_IDEO_FACE_శిఖరం_OR_కుడి:
  స్థితి HB_OT_LAYOUT_BASELINE_ట్యాగ్_IDEO_FACE_BOTTOM_OR_ఎడమ:
    {
      hb_position_t embox_శిఖరం, embox_bottom;

      hb_ot_layout_get_baseline_with_fallback (font,
					       HB_OT_LAYOUT_BASELINE_ట్యాగ్_IDEO_EMBOX_శిఖరం_OR_కుడి,
					       direction,
					       script_ట్యాగ్,
					       language_ట్యాగ్,
					       &embox_శిఖరం);
      hb_ot_layout_get_baseline_with_fallback (font,
					       HB_OT_LAYOUT_BASELINE_ట్యాగ్_IDEO_EMBOX_BOTTOM_OR_ఎడమ,
					       direction,
					       script_ట్యాగ్,
					       language_ట్యాగ్,
					       &embox_bottom);

      ఒకవేళ (baseline_ట్యాగ్ == HB_OT_LAYOUT_BASELINE_ట్యాగ్_IDEO_FACE_శిఖరం_OR_కుడి)
	*coord = hb_saturate_కూడు (embox_శిఖరం,
				  hb_clamp_గమ్యం<hb_position_t> (((int64_t) embox_bottom - embox_శిఖరం) / 10));
      లేకపోతే
	*coord = hb_saturate_కూడు (embox_bottom,
				  hb_clamp_గమ్యం<hb_position_t> (((int64_t) embox_శిఖరం - embox_bottom) / 10));
    }
    ఆపు;

  స్థితి HB_OT_LAYOUT_BASELINE_ట్యాగ్_IDEO_EMBOX_శిఖరం_OR_కుడి:
    ఒకవేళ (hb_ot_layout_get_baseline (font,
				   HB_OT_LAYOUT_BASELINE_ట్యాగ్_IDEO_EMBOX_BOTTOM_OR_ఎడమ,
				   direction,
				   script_ట్యాగ్,
				   language_ట్యాగ్,
				   coord))
      *coord = hb_saturate_కూడు (*coord,
				HB_DIRECTION_IS_HORIZONTAL (direction) ? font->y_scale : font->x_scale);
    లేకపోతే
    {
      hb_font_extents_t font_extents;
      hb_font_get_extents_for_direction (font, direction, &font_extents);
      *coord = font_extents.ascender;
    }
    ఆపు;

  స్థితి HB_OT_LAYOUT_BASELINE_ట్యాగ్_IDEO_EMBOX_BOTTOM_OR_ఎడమ:
    ఒకవేళ (hb_ot_layout_get_baseline (font,
				   HB_OT_LAYOUT_BASELINE_ట్యాగ్_IDEO_EMBOX_శిఖరం_OR_కుడి,
				   direction,
				   script_ట్యాగ్,
				   language_ట్యాగ్,
				   coord))
      *coord = hb_saturate_sub (*coord,
				HB_DIRECTION_IS_HORIZONTAL (direction) ? font->y_scale : font->x_scale);
    లేకపోతే
    {
      hb_font_extents_t font_extents;
      hb_font_get_extents_for_direction (font, direction, &font_extents);
      *coord = font_extents.descender;
    }
    ఆపు;

  స్థితి HB_OT_LAYOUT_BASELINE_ట్యాగ్_HANGING:
    ఒకవేళ (HB_DIRECTION_IS_HORIZONTAL (direction))
    {
      hb_codepoint_t ch;
      hb_codepoint_t glyph;
      hb_glyph_extents_t extents;

      /* Keep in sync with hb_ot_layout_get_horizontal_baseline_for_script */
      ఎంపిక ((పూర్ణాంకం) script_ట్యాగ్)
      {
      /* Unicode-1.1 additions */
      స్థితి HB_SCRIPT_BENGALI:          ch = 0x0995u; ఆపు;
      స్థితి HB_SCRIPT_DEVANAGARI:       ch = 0x0915u; ఆపు;
      స్థితి HB_SCRIPT_GUJARATI:         ch = 0x0a95u; ఆపు;
      స్థితి HB_SCRIPT_GURMUKHI:         ch = 0x0a15u; ఆపు;
      /* Unicode-2.0 additions */
      స్థితి HB_SCRIPT_TIBETAN:          ch = 0x0f40u; ఆపు;
      /* Unicode-4.0 additions */
      స్థితి HB_SCRIPT_LIMBU:            ch = 0x1901u; ఆపు;
      /* Unicode-4.1 additions */
      స్థితి HB_SCRIPT_SYLOTI_NAGRI:     ch = 0xa807u; ఆపు;
      /* Unicode-5.0 additions */
      స్థితి HB_SCRIPT_PHAGS_PA:         ch = 0xa840u; ఆపు;
      /* Unicode-5.2 additions */
      స్థితి HB_SCRIPT_MEETEI_MAYEK:     ch = 0xabc0u; ఆపు;
      /* Unicode-6.1 additions */
      స్థితి HB_SCRIPT_SHARADA:          ch = 0x11191u; ఆపు;
      స్థితి HB_SCRIPT_TAKRI:            ch = 0x1168cu; ఆపు;
      /* Unicode-7.0 additions */
      స్థితి HB_SCRIPT_MODI:             ch = 0x1160eu;ఆపు;
      స్థితి HB_SCRIPT_SIDDHAM:          ch = 0x11590u; ఆపు;
      స్థితి HB_SCRIPT_TIRHUTA:          ch = 0x1148fu; ఆపు;
      /* Unicode-9.0 additions */
      స్థితి HB_SCRIPT_MARCHEN:          ch = 0x11c72u; ఆపు;
      స్థితి HB_SCRIPT_NEWA:             ch = 0x1140eu; ఆపు;
      /* Unicode-10.0 additions */
      స్థితి HB_SCRIPT_SOYOMBO:          ch = 0x11a5cu; ఆపు;
      స్థితి HB_SCRIPT_ZANABAZAR_వర్గం: ch = 0x11a0bu; ఆపు;
      /* Unicode-11.0 additions */
      స్థితి HB_SCRIPT_DOGRA:            ch = 0x1180au; ఆపు;
      స్థితి HB_SCRIPT_GUNJALA_GONDI:    ch = 0x11d6cu; ఆపు;
      /* Unicode-12.0 additions */
      స్థితి HB_SCRIPT_NANDINAGARI:      ch = 0x119b0u; ఆపు;
      అప్రమేయ:                         ch = 0;        ఆపు;
      }

      ఒకవేళ (ch &&
	  hb_font_get_nominal_glyph (font, ch, &glyph) &&
	  hb_font_get_glyph_extents (font, glyph, &extents))
	*coord = extents.y_bearing;
      లేకపోతే
	*coord = hb_clamp_గమ్యం<hb_position_t> ((int64_t) font->y_scale * 6 / 10); // FIXME makes assumptions about origin
    }
    లేకపోతే
      *coord = hb_clamp_గమ్యం<hb_position_t> ((int64_t) font->x_scale * 6 / 10); // FIXME makes assumptions about origin
    ఆపు;

  స్థితి HB_OT_LAYOUT_BASELINE_ట్యాగ్_IDEO_EMBOX_CENTRAL:
    {
      hb_position_t శిఖరం_2, bottom;
      hb_ot_layout_get_baseline_with_fallback (font,
					       HB_OT_LAYOUT_BASELINE_ట్యాగ్_IDEO_EMBOX_శిఖరం_OR_కుడి,
					       direction,
					       script_ట్యాగ్,
					       language_ట్యాగ్,
					       &శిఖరం_2);
      hb_ot_layout_get_baseline_with_fallback (font,
					       HB_OT_LAYOUT_BASELINE_ట్యాగ్_IDEO_EMBOX_BOTTOM_OR_ఎడమ,
					       direction,
					       script_ట్యాగ్,
					       language_ట్యాగ్,
					       &bottom);
      *coord = hb_clamp_గమ్యం<hb_position_t> (((int64_t) శిఖరం_2 + bottom) / 2);

    }
    ఆపు;

  స్థితి HB_OT_LAYOUT_BASELINE_ట్యాగ్_IDEO_FACE_CENTRAL:
    {
      hb_position_t శిఖరం_2, bottom;
      hb_ot_layout_get_baseline_with_fallback (font,
					       HB_OT_LAYOUT_BASELINE_ట్యాగ్_IDEO_FACE_శిఖరం_OR_కుడి,
					       direction,
					       script_ట్యాగ్,
					       language_ట్యాగ్,
					       &శిఖరం_2);
      hb_ot_layout_get_baseline_with_fallback (font,
					       HB_OT_LAYOUT_BASELINE_ట్యాగ్_IDEO_FACE_BOTTOM_OR_ఎడమ,
					       direction,
					       script_ట్యాగ్,
					       language_ట్యాగ్,
					       &bottom);
      *coord = hb_clamp_గమ్యం<hb_position_t> (((int64_t) శిఖరం_2 + bottom) / 2);

    }
    ఆపు;

  స్థితి HB_OT_LAYOUT_BASELINE_ట్యాగ్_MAX_విలువ:
  అప్రమేయ:
    *coord = 0;
    ఆపు;
  }
}

/**
 * hb_ot_layout_get_baseline_with_fallback2:
 * @font: a font
 * @baseline_tag: a baseline tag
 * @direction: text direction.
 * @script:  script.
 * @language: (nullable): language, currently unused.
 * @coord: (out): baseline value if found.
 *
 * Fetches a baseline value from the font, and synthesizes
 * it if the font does not have it.
 *
 * This function is like hb_ot_layout_get_baseline_with_fallback() but takes
 * #hb_script_t and #hb_language_t instead of OpenType #hb_tag_t.
 *
 * Since: 8.0.0
 **/
శూన్య
hb_ot_layout_get_baseline_with_fallback2 (hb_font_t                   *font,
					  hb_ot_layout_baseline_ట్యాగ్_t  baseline_ట్యాగ్,
					  hb_direction_t               direction,
					  hb_script_t                  script,
					  hb_language_t                language,
					  hb_position_t               *coord        /*బహిర్గతం*/)
{
  hb_ట్యాగ్_t script_ట్యాగ్, language_ట్యాగ్;
  ఎంచుకో_పునాది_ట్యాగులు (script, language, &script_ట్యాగ్, &language_ట్యాగ్);
  hb_ot_layout_get_baseline_with_fallback (font,
					   baseline_ట్యాగ్,
					   direction,
					   script_ట్యాగ్,
					   language_ట్యాగ్,
					   coord);
}

#అయితే_ముగింపు


#నిర్వచితం_కాకపోతే HB_NO_LAYOUT_RARELY_వాడిన
నిర్మాణం hb_get_glyph_alternates_dispatch_t :
       hb_dispatch_context_t<hb_get_glyph_alternates_dispatch_t, గుర్తులేని>
{
  స్థిర return_t default_return_విలువ () { తిరిగి 0; }
  తార్కిక stop_sublookup_iteration (return_t r) స్థిరరాశి { తిరిగి r; }

  అంతరంగ:
  మూస <రకం_పేరు T, రకం_పేరు ...Ts> స్వయంచాలక
  _dispatch (స్థిరరాశి T &obj, hb_ప్రాధాన్యత<1>, Ts&&... ds) HB_AUTO_RETURN
  ( obj.get_glyph_alternates (std::forward<Ts> (ds)...) )
  మూస <రకం_పేరు T, రకం_పేరు ...Ts> స్వయంచాలక
  _dispatch (స్థిరరాశి T &obj, hb_ప్రాధాన్యత<0>, Ts&&... ds) HB_AUTO_RETURN
  ( default_return_విలువ () )
  బహిరంగ:
  మూస <రకం_పేరు T, రకం_పేరు ...Ts> స్వయంచాలక
  dispatch (స్థిరరాశి T &obj, Ts&&... ds) HB_AUTO_RETURN
  ( _dispatch (obj, hb_prioritize, std::forward<Ts> (ds)...) )
};

/**
 * hb_ot_layout_lookup_get_glyph_alternates:
 * @face: a face.
 * @lookup_index: index of the feature lookup to query.
 * @glyph: a glyph id.
 * @start_offset: starting offset.
 * @alternate_count: (inout) (nullable): Input = the maximum number of alternate glyphs to return;
 *                   Output = the actual number of alternate glyphs returned (may be zero).
 * @alternate_glyphs: (out caller-allocates) (array length=alternate_count) (nullable): A glyphs buffer.
 *                    Alternate glyphs associated with the glyph id.
 *
 * Fetches alternates of a glyph from a given GSUB lookup index. Note that for one-to-one GSUB
 * glyph substitutions, this function fetches the substituted glyph.
 *
 * Return value: Total number of alternates found in the specific lookup index for the given glyph id.
 *
 * Since: 2.6.8
 **/
HB_EXTERN గుర్తులేని
hb_ot_layout_lookup_get_glyph_alternates (hb_face_t      *face,
					  గుర్తులేని        lookup_సూచీ,
					  hb_codepoint_t  glyph,
					  గుర్తులేని        ప్రారంభం_స్థానభ్రంశం,
					  గుర్తులేని       *alternate_లెక్క  /* IN/OUT.  May be NULL. */,
					  hb_codepoint_t *alternate_glyphs /* OUT.     May be NULL. */)
{
  hb_get_glyph_alternates_dispatch_t c;
  స్థిరరాశి OT::SubstLookup &lookup = face->పట్టిక.GSUB->పట్టిక->get_lookup (lookup_సూచీ);
  స్వయంచాలక ret = lookup.dispatch (&c, glyph, ప్రారంభం_స్థానభ్రంశం, alternate_లెక్క, alternate_glyphs);
  ఒకవేళ (!ret && alternate_లెక్క) *alternate_లెక్క = 0;
  తిరిగి ret;
}

నిర్మాణం hb_collect_glyph_alternates_dispatch_t :
       hb_dispatch_context_t<hb_collect_glyph_alternates_dispatch_t, తార్కిక>
{
  స్థిర return_t default_return_విలువ () { తిరిగి అబద్ధం; }
  తార్కిక stop_sublookup_iteration (return_t r) స్థిరరాశి { తిరిగి అబద్ధం; }

  అంతరంగ:
  మూస <రకం_పేరు T, రకం_పేరు ...Ts> స్వయంచాలక
  _dispatch (స్థిరరాశి T &obj, hb_ప్రాధాన్యత<1>, Ts&&... ds) HB_AUTO_RETURN
  ( (obj.collect_glyph_alternates (std::forward<Ts> (ds)...), నిజం) )
  మూస <రకం_పేరు T, రకం_పేరు ...Ts> స్వయంచాలక
  _dispatch (స్థిరరాశి T &obj, hb_ప్రాధాన్యత<0>, Ts&&... ds) HB_AUTO_RETURN
  ( default_return_విలువ () )
  బహిరంగ:
  మూస <రకం_పేరు T, రకం_పేరు ...Ts> స్వయంచాలక
  dispatch (స్థిరరాశి T &obj, Ts&&... ds) HB_AUTO_RETURN
  ( _dispatch (obj, hb_prioritize, std::forward<Ts> (ds)...) )
};

/**
 * hb_ot_layout_lookup_collect_glyph_alternates:
 * @face: a face.
 * @lookup_index: index of the feature lookup to query.
 * @alternate_count: (inout): mapping from glyph index to number of alternates for that glyph.
 * @alternate_glyphs: (inout): mapping from encoded glyph index and alternate index, to alternate glyph ids.
 *
 * Collects alternates of glyphs from a given GSUB lookup index.
 *
 * For one-to-one GSUB glyph substitutions, this function collects the
 * substituted glyph.
 *
 * For lookups that assign multiple alternates to a glyph, all alternate glyphs are collected.
 *
 * For other lookup types, nothing is performed and `false` is returned.
 *
 * The `alternate_count` mapping will contain the number of alternates for each glyph id.
 * Upon entry, this mapping should contain the glyph ids as keys, and the number of alternates
 * currently known for each glyph id as values.
 *
 * The `alternate_glyphs` mapping will contain the alternate glyph ids for each glyph id.
 * The mapping is encoded in the following way, upon entry and after processing:
 * If G is the glyph id, and A0, A1, ..., A(n-1) are the alternate glyph ids,
 * the mapping will contain the following entries: (G + (i << 24)) -> A(i)
 * for i = 0, 1, ..., n-1 where n is the number of alternates for G as per `alternate_count`.
 *
 * Return value: `true` if alternates were collected, `false` otherwise.
 * Since: 12.1.0
 */
HB_EXTERN hb_bool_t
hb_ot_layout_lookup_collect_glyph_alternates (hb_face_t *face,
					      గుర్తులేని   lookup_సూచీ,
					      hb_map_t  *alternate_లెక్క /* IN/OUT */,
					      hb_map_t  *alternate_glyphs /* IN/OUT */)
{
  hb_collect_glyph_alternates_dispatch_t c;
  స్థిరరాశి OT::SubstLookup &lookup = face->పట్టిక.GSUB->పట్టిక->get_lookup (lookup_సూచీ);
  తిరిగి lookup.dispatch (&c, alternate_లెక్క, alternate_glyphs);
}

నిర్మాణం hb_position_single_dispatch_t :
       hb_dispatch_context_t<hb_position_single_dispatch_t, తార్కిక>
{
  స్థిర return_t default_return_విలువ () { తిరిగి అబద్ధం; }
  తార్కిక stop_sublookup_iteration (return_t r) స్థిరరాశి { తిరిగి r; }

  అంతరంగ:
  మూస <రకం_పేరు T, రకం_పేరు ...Ts> స్వయంచాలక
  _dispatch (స్థిరరాశి T &obj, hb_ప్రాధాన్యత<1>, Ts&&... ds) HB_AUTO_RETURN
  ( obj.position_single (std::forward<Ts> (ds)...) )
  మూస <రకం_పేరు T, రకం_పేరు ...Ts> స్వయంచాలక
  _dispatch (స్థిరరాశి T &obj, hb_ప్రాధాన్యత<0>, Ts&&... ds) HB_AUTO_RETURN
  ( default_return_విలువ () )
  బహిరంగ:
  మూస <రకం_పేరు T, రకం_పేరు ...Ts> స్వయంచాలక
  dispatch (స్థిరరాశి T &obj, Ts&&... ds) HB_AUTO_RETURN
  ( _dispatch (obj, hb_prioritize, std::forward<Ts> (ds)...) )
};

/**
 * hb_ot_layout_lookup_get_optical_bound:
 * @font: a font.
 * @lookup_index: index of the feature lookup to query.
 * @direction: edge of the glyph to query.
 * @glyph: a glyph id.
 *
 * Fetches the optical bound of a glyph positioned at the margin of text.
 * The direction identifies which edge of the glyph to query.
 *
 * Return value: Adjustment value. Negative values mean the glyph will stick out of the margin.
 *
 * Since: 5.3.0
 **/
hb_position_t
hb_ot_layout_lookup_get_optical_bound (hb_font_t      *font,
				       గుర్తులేని        lookup_సూచీ,
				       hb_direction_t  direction,
				       hb_codepoint_t  glyph)
{
  స్థిరరాశి OT::PosLookup &lookup = font->face->పట్టిక.GPOS->పట్టిక->get_lookup (lookup_సూచీ);
  hb_blob_t *blob = font->face->పట్టిక.GPOS->get_blob ();
  hb_glyph_position_t pos = {0};
  hb_position_single_dispatch_t c;
  lookup.dispatch (&c, font, blob, direction, glyph, pos);
  hb_position_t ret = 0;
  ఎంపిక (direction)
  {
    స్థితి HB_DIRECTION_LTR:
      ret = pos.x_స్థానభ్రంశం_2;
      ఆపు;
    స్థితి HB_DIRECTION_RTL:
      ret = hb_saturate_sub (pos.x_advance, pos.x_స్థానభ్రంశం_2);
      ఆపు;
    స్థితి HB_DIRECTION_TTB:
      ret = pos.y_స్థానభ్రంశం_2;
      ఆపు;
    స్థితి HB_DIRECTION_BTT:
      ret = hb_saturate_sub (pos.y_advance, pos.y_స్థానభ్రంశం_2);
      ఆపు;
    స్థితి HB_DIRECTION_INVALID:
    అప్రమేయ:
      ఆపు;
  }
  తిరిగి ret;
}
#అయితే_ముగింపు


#అయితే_ముగింపు
