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

#شامل "hb.hh"

#اگر_غیر_تعریف_شدہ HB_NO_OT_LAYOUT

#اگر_تعریف_شدہ HB_NO_OT_لیبل
#غلطی "Cannot compile hb-ot-layout.cc with HB_NO_OT_TAG."
#اگر_ختم

#شامل "hb-open-type.hh"
#شامل "hb-ot-layout.hh"
#شامل "hb-ot-face.hh"
#شامل "hb-ot-map.hh"
#شامل "hb-map.hh"

#شامل "hb-ot-kern-table.hh"
#شامل "hb-ot-layout-gdef-table.hh"
#شامل "hb-ot-layout-gsub-table.hh"
#شامل "hb-ot-layout-gpos-table.hh"
#شامل "hb-ot-layout-base-table.hh"
#شامل "hb-ot-layout-jstf-table.hh" // Just so we compile it; unused otherwise.
#شامل "hb-ot-name-table.hh"
#شامل "hb-ot-os2-table.hh"

#شامل "hb-aat-layout-morx-table.hh"
#شامل "hb-aat-layout-opbd-table.hh" // Just so we compile it; unused otherwise.

استعمال OT::Layout::GSUB;
استعمال OT::Layout::GPOS;

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

#اگر_غیر_تعریف_شدہ HB_NO_OT_KERN
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
منطقی
hb_ot_layout_has_kerning (hb_face_t *face)
{
  واپس face->جدول.kern->جدول->has_ڈیٹا ();
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
منطقی
hb_ot_layout_has_machine_kerning (hb_face_t *face)
{
  واپس face->جدول.kern->جدول->has_state_machine ();
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
منطقی
hb_ot_layout_has_cross_kerning (hb_face_t *face)
{
  واپس face->جدول.kern->جدول->has_cross_دھارا ();
}

خالی
hb_ot_layout_kern (ثابت hb_ot_shape_plan_t *plan,
		   hb_font_t *font,
		   hb_بفر_t  *بفر)
{
  خودکار &accel = *font->face->جدول.kern;
  hb_blob_t *blob = accel.get_blob ();

  AAT::hb_aat_apply_context_t c (plan, font, بفر, blob);

  اگر (!بفر->پیغام (font, "start table kern")) واپس;
  c.بفر_glyph_سیٹ = accel.scratch.create_بفر_glyph_سیٹ ();
  accel.apply (&c);
  accel.scratch.destroy_بفر_glyph_سیٹ (c.بفر_glyph_سیٹ);
  (خالی) بفر->پیغام (font, "end table kern");
}
#اگر_ختم


/*
 * GDEF
 */

منطقی
OT::GDEF::is_blocklisted (hb_blob_t *blob,
			  hb_face_t *face) ثابت
{
#اگر_تعریف_شدہ HB_NO_OT_LAYOUT_BLOCKLIST
  واپس جھوٹ;
#اگر_ختم
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
  انتخاب HB_CODEPOINT_ENCODE3(blob->لمبائی,
			      face->جدول.GSUB->جدول.get_لمبائی (),
			      face->جدول.GPOS->جدول.get_لمبائی ())
  {
    /* sha1sum:c5ee92f0bca4bfb7d06c4d03e8cf9f9cf75d2e8a Windows 7? timesi.ttf */
    حالت HB_CODEPOINT_ENCODE3 (442, 2874, 42038):
    /* sha1sum:37fc8c16a0894ab7b749e35579856c73c840867b Windows 7? timesbi.ttf */
    حالت HB_CODEPOINT_ENCODE3 (430, 2874, 40662):
    /* sha1sum:19fc45110ea6cd3cdd0a5faca256a3797a069a80 Windows 7 timesi.ttf */
    حالت HB_CODEPOINT_ENCODE3 (442, 2874, 39116):
    /* sha1sum:6d2d3c9ed5b7de87bc84eae0df95ee5232ecde26 Windows 7 timesbi.ttf */
    حالت HB_CODEPOINT_ENCODE3 (430, 2874, 39374):
    /* sha1sum:8583225a8b49667c077b3525333f84af08c6bcd8 OS X 10.11.3 Times New Roman Italic.ttf */
    حالت HB_CODEPOINT_ENCODE3 (490, 3046, 41638):
    /* sha1sum:ec0f5a8751845355b7c3271d11f9918a966cb8c9 OS X 10.11.3 Times New Roman Bold Italic.ttf */
    حالت HB_CODEPOINT_ENCODE3 (478, 3046, 41902):
    /* sha1sum:96eda93f7d33e79962451c6c39a6b51ee893ce8c  tahoma.ttf from Windows 8 */
    حالت HB_CODEPOINT_ENCODE3 (898, 12554, 46470):
    /* sha1sum:20928dc06014e0cd120b6fc942d0c3b1a46ac2bc  tahomabd.ttf from Windows 8 */
    حالت HB_CODEPOINT_ENCODE3 (910, 12566, 47732):
    /* sha1sum:4f95b7e4878f60fa3a39ca269618dfde9721a79e  tahoma.ttf from Windows 8.1 */
    حالت HB_CODEPOINT_ENCODE3 (928, 23298, 59332):
    /* sha1sum:6d400781948517c3c0441ba42acb309584b73033  tahomabd.ttf from Windows 8.1 */
    حالت HB_CODEPOINT_ENCODE3 (940, 23310, 60732):
    /* tahoma.ttf v6.04 from Windows 8.1 x64, see https://bugzilla.mozilla.org/show_bug.cgi?id=1279925 */
    حالت HB_CODEPOINT_ENCODE3 (964, 23836, 60072):
    /* tahomabd.ttf v6.04 from Windows 8.1 x64, see https://bugzilla.mozilla.org/show_bug.cgi?id=1279925 */
    حالت HB_CODEPOINT_ENCODE3 (976, 23832, 61456):
    /* sha1sum:e55fa2dfe957a9f7ec26be516a0e30b0c925f846  tahoma.ttf from Windows 10 */
    حالت HB_CODEPOINT_ENCODE3 (994, 24474, 60336):
    /* sha1sum:7199385abb4c2cc81c83a151a7599b6368e92343  tahomabd.ttf from Windows 10 */
    حالت HB_CODEPOINT_ENCODE3 (1006, 24470, 61740):
    /* tahoma.ttf v6.91 from Windows 10 x64, see https://bugzilla.mozilla.org/show_bug.cgi?id=1279925 */
    حالت HB_CODEPOINT_ENCODE3 (1006, 24576, 61346):
    /* tahomabd.ttf v6.91 from Windows 10 x64, see https://bugzilla.mozilla.org/show_bug.cgi?id=1279925 */
    حالت HB_CODEPOINT_ENCODE3 (1018, 24572, 62828):
    /* sha1sum:b9c84d820c49850d3d27ec498be93955b82772b5  tahoma.ttf from Windows 10 AU */
    حالت HB_CODEPOINT_ENCODE3 (1006, 24576, 61352):
    /* sha1sum:2bdfaab28174bdadd2f3d4200a30a7ae31db79d2  tahomabd.ttf from Windows 10 AU */
    حالت HB_CODEPOINT_ENCODE3 (1018, 24572, 62834):
    /* sha1sum:b0d36cf5a2fbe746a3dd277bffc6756a820807a7  Tahoma.ttf from Mac OS X 10.9 */
    حالت HB_CODEPOINT_ENCODE3 (832, 7324, 47162):
    /* sha1sum:12fc4538e84d461771b30c18b5eb6bd434e30fba  Tahoma Bold.ttf from Mac OS X 10.9 */
    حالت HB_CODEPOINT_ENCODE3 (844, 7302, 45474):
    /* sha1sum:eb8afadd28e9cf963e886b23a30b44ab4fd83acc  himalaya.ttf from Windows 7 */
    حالت HB_CODEPOINT_ENCODE3 (180, 13054, 7254):
    /* sha1sum:73da7f025b238a3f737aa1fde22577a6370f77b0  himalaya.ttf from Windows 8 */
    حالت HB_CODEPOINT_ENCODE3 (192, 12638, 7254):
    /* sha1sum:6e80fd1c0b059bbee49272401583160dc1e6a427  himalaya.ttf from Windows 8.1 */
    حالت HB_CODEPOINT_ENCODE3 (192, 12690, 7254):
    /* 8d9267aea9cd2c852ecfb9f12a6e834bfaeafe44  cantarell-fonts-0.0.21/otf/Cantarell-Regular.otf */
    /* 983988ff7b47439ab79aeaf9a45bd4a2c5b9d371  cantarell-fonts-0.0.21/otf/Cantarell-Oblique.otf */
    حالت HB_CODEPOINT_ENCODE3 (188, 248, 3852):
    /* 2c0c90c6f6087ffbfea76589c93113a9cbb0e75f  cantarell-fonts-0.0.21/otf/Cantarell-Bold.otf */
    /* 55461f5b853c6da88069ffcdf7f4dd3f8d7e3e6b  cantarell-fonts-0.0.21/otf/Cantarell-Bold-Oblique.otf */
    حالت HB_CODEPOINT_ENCODE3 (188, 264, 3426):
    /* d125afa82a77a6475ac0e74e7c207914af84b37a padauk-2.80/Padauk.ttf RHEL 7.2 */
    حالت HB_CODEPOINT_ENCODE3 (1058, 47032, 11818):
    /* 0f7b80437227b90a577cc078c0216160ae61b031 padauk-2.80/Padauk-Bold.ttf RHEL 7.2*/
    حالت HB_CODEPOINT_ENCODE3 (1046, 47030, 12600):
    /* d3dde9aa0a6b7f8f6a89ef1002e9aaa11b882290 padauk-2.80/Padauk.ttf Ubuntu 16.04 */
    حالت HB_CODEPOINT_ENCODE3 (1058, 71796, 16770):
    /* 5f3c98ccccae8a953be2d122c1b3a77fd805093f padauk-2.80/Padauk-Bold.ttf Ubuntu 16.04 */
    حالت HB_CODEPOINT_ENCODE3 (1046, 71790, 17862):
    /* 6c93b63b64e8b2c93f5e824e78caca555dc887c7 padauk-2.80/Padauk-book.ttf */
    حالت HB_CODEPOINT_ENCODE3 (1046, 71788, 17112):
    /* d89b1664058359b8ec82e35d3531931125991fb9 padauk-2.80/Padauk-bookbold.ttf */
    حالت HB_CODEPOINT_ENCODE3 (1058, 71794, 17514):
    /* 824cfd193aaf6234b2b4dc0cf3c6ef576c0d00ef padauk-3.0/Padauk-book.ttf */
    حالت HB_CODEPOINT_ENCODE3 (1330, 109904, 57938):
    /* 91fcc10cf15e012d27571e075b3b4dfe31754a8a padauk-3.0/Padauk-bookbold.ttf */
    حالت HB_CODEPOINT_ENCODE3 (1330, 109904, 58972):
    /* sha1sum: c26e41d567ed821bed997e937bc0c41435689e85  Padauk.ttf
     *  "Padauk Regular" "Version 2.5", see https://crbug.com/681813 */
    حالت HB_CODEPOINT_ENCODE3 (1004, 59092, 14836):
    /* 88d2006ca084f04af2df1954ed714a8c71e8400f  Courier New.ttf from macOS 15 */
    حالت HB_CODEPOINT_ENCODE3 (588, 5078, 14418):
    /* 608e3ebb6dd1aee521cff08eb07d500a2c59df68  Courier New Bold.ttf from macOS 15 */
    حالت HB_CODEPOINT_ENCODE3 (588, 5078, 14238):
    /* d13221044ff054efd78f1cd8631b853c3ce85676  cour.ttf from Windows 10 */
    حالت HB_CODEPOINT_ENCODE3 (894, 17162, 33960):
    /* 68ed4a22d8067fcf1622ac6f6e2f4d3a2e3ec394  courbd.ttf from Windows 10 */
    حالت HB_CODEPOINT_ENCODE3 (894, 17154, 34472):
    /* 4cdb0259c96b7fd7c103821bb8f08f7cc6b211d7  cour.ttf from Windows 8.1 */
    حالت HB_CODEPOINT_ENCODE3 (816, 7868, 17052):
    /* 920483d8a8ed37f7f0afdabbe7f679aece7c75d8  courbd.ttf from Windows 8.1 */
    حالت HB_CODEPOINT_ENCODE3 (816, 7868, 17138):
      واپس سچ;
  }
  واپس جھوٹ;
}

سانچہ <منطقی سیٹ_Digest>
ساکن خالی
hb_ot_layout_سیٹ_glyph_props (hb_font_t *font,
			       hb_بفر_t *بفر)
{
  hb_بفر_assert_gsubgpos_vars (بفر);

  ثابت خودکار &gdef = *font->face->جدول.GDEF;
  غیرعلامتی صحیح گنتی = بفر->len;
  hb_glyph_info_t *info = بفر->info;
  برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
  {
    اگر (سیٹ_Digest)
      بفر->digest.جوڑو (info[i].codepoint);
    hb_glyph_info_سیٹ_glyph_props (&info[i], gdef.get_glyph_props (info[i].codepoint));
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
  واپس face->جدول.GDEF->جدول->has_glyph_classes ();
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
  واپس (hb_ot_layout_glyph_class_t) face->جدول.GDEF->جدول->get_glyph_class (glyph);
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
خالی
hb_ot_layout_get_glyphs_اندر_class (hb_face_t                  *face,
				  hb_ot_layout_glyph_class_t  klass,
				  hb_سیٹ_t                   *glyphs /*برآمد*/)
{
  واپس face->جدول.GDEF->جدول->get_glyphs_اندر_class (klass, glyphs);
}

#اگر_غیر_تعریف_شدہ HB_NO_LAYOUT_غیر_مستعمل
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
غیرعلامتی صحیح
hb_ot_layout_get_attach_points (hb_face_t      *face,
				hb_codepoint_t  glyph,
				غیرعلامتی صحیح    آغاز_انحراف,
				غیرعلامتی صحیح   *نقطہ_گنتی /* IN/OUT */,
				غیرعلامتی صحیح   *نقطہ_array /*برآمد*/)
{
  واپس face->جدول.GDEF->جدول->get_attach_points (glyph,
						     آغاز_انحراف,
						     نقطہ_گنتی,
						     نقطہ_array);
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
غیرعلامتی صحیح
hb_ot_layout_get_ligature_carets (hb_font_t      *font,
				  hb_direction_t  direction,
				  hb_codepoint_t  glyph,
				  غیرعلامتی صحیح    آغاز_انحراف,
				  غیرعلامتی صحیح   *caret_گنتی /* IN/OUT */,
				  hb_position_t  *caret_array /*برآمد*/)
{
  واپس font->face->جدول.GDEF->جدول->get_lig_carets (font, direction, glyph, آغاز_انحراف, caret_گنتی, caret_array);
}
#اگر_ختم


/*
 * GSUB/GPOS
 */

منطقی
GSUB::is_blocklisted (hb_blob_t *blob HB_غیر_مستعمل,
			  hb_face_t *face) ثابت
{
#اگر_تعریف_شدہ HB_NO_OT_LAYOUT_BLOCKLIST
  واپس جھوٹ;
#اگر_ختم
  واپس جھوٹ;
}

منطقی
GPOS::is_blocklisted (hb_blob_t *blob HB_غیر_مستعمل,
			  hb_face_t *face HB_غیر_مستعمل) ثابت
{
#اگر_تعریف_شدہ HB_NO_OT_LAYOUT_BLOCKLIST
  واپس جھوٹ;
#اگر_ختم
  واپس جھوٹ;
}

ساکن ثابت OT::GSUBGPOS&
get_gsubgpos_جدول (hb_face_t *face,
		    hb_لیبل_t   جدول_لیبل_2)
{
  انتخاب (جدول_لیبل_2) {
    حالت HB_OT_لیبل_GSUB: واپس *face->جدول.GSUB->جدول;
    حالت HB_OT_لیبل_GPOS: واپس *face->جدول.GPOS->جدول;
    طےشدہ:             واپس Null (OT::GSUBGPOS);
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
غیرعلامتی صحیح
hb_ot_layout_جدول_get_script_لیبل_فہرست (hb_face_t    *face,
				    hb_لیبل_t      جدول_لیبل_2,
				    غیرعلامتی صحیح  آغاز_انحراف,
				    غیرعلامتی صحیح *script_گنتی /* IN/OUT */,
				    hb_لیبل_t     *script_لیبل_فہرست  /*برآمد*/)
{
  ثابت OT::GSUBGPOS &g = get_gsubgpos_جدول (face, جدول_لیبل_2);

  واپس g.get_script_لیبل_فہرست (آغاز_انحراف, script_گنتی, script_لیبل_فہرست);
}

#تعریف_کرو HB_OT_لیبل_LATIN_SCRIPT		HB_لیبل ('l', 'a', 't', 'n')

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
hb_ot_layout_جدول_ڈھونڈو_script (hb_face_t    *face,
				hb_لیبل_t      جدول_لیبل_2,
				hb_لیبل_t      script_لیبل,
				غیرعلامتی صحیح *script_اشاریہ /*برآمد*/)
{
  static_assert ((OT::اشاریہ::NOT_ملا_اشاریہ == HB_OT_LAYOUT_NO_SCRIPT_اشاریہ), "");
  ثابت OT::GSUBGPOS &g = get_gsubgpos_جدول (face, جدول_لیبل_2);

  اگر (g.ڈھونڈو_script_اشاریہ (script_لیبل, script_اشاریہ))
    واپس سچ;

  /* try finding 'DFLT' */
  اگر (g.ڈھونڈو_script_اشاریہ (HB_OT_لیبل_DEFAULT_SCRIPT, script_اشاریہ))
    واپس جھوٹ;

  /* try with 'dflt'; MS site has had typos and many fonts use it now :(.
   * including many versions of DejaVu Sans Mono! */
  اگر (g.ڈھونڈو_script_اشاریہ (HB_OT_لیبل_DEFAULT_LANGUAGE, script_اشاریہ))
    واپس جھوٹ;

  /* try with 'latn'; some old fonts put their features there even though
     they're really trying to support Thai, for example :( */
  اگر (g.ڈھونڈو_script_اشاریہ (HB_OT_لیبل_LATIN_SCRIPT, script_اشاریہ))
    واپس جھوٹ;

  اگر (script_اشاریہ) *script_اشاریہ = HB_OT_LAYOUT_NO_SCRIPT_اشاریہ;
  واپس جھوٹ;
}

#اگر_غیر_تعریف_شدہ HB_DISABLE_DEPRECATED
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
hb_ot_layout_جدول_انتخاب_script (hb_face_t      *face,
				  hb_لیبل_t        جدول_لیبل_2,
				  ثابت hb_لیبل_t *script_لیبل_فہرست,
				  غیرعلامتی صحیح   *script_اشاریہ  /*برآمد*/,
				  hb_لیبل_t       *chosen_script /*برآمد*/)
{
  ثابت hb_لیبل_t *t;
  برائے (t = script_لیبل_فہرست; *t; t++);
  واپس hb_ot_layout_جدول_select_script (face, جدول_لیبل_2, t - script_لیبل_فہرست, script_لیبل_فہرست, script_اشاریہ, chosen_script);
}
#اگر_ختم

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
hb_ot_layout_جدول_select_script (hb_face_t      *face,
				  hb_لیبل_t        جدول_لیبل_2,
				  غیرعلامتی صحیح    script_گنتی,
				  ثابت hb_لیبل_t *script_لیبل_فہرست,
				  غیرعلامتی صحیح   *script_اشاریہ  /*برآمد*/,
				  hb_لیبل_t       *chosen_script /*برآمد*/)
{
  static_assert ((OT::اشاریہ::NOT_ملا_اشاریہ == HB_OT_LAYOUT_NO_SCRIPT_اشاریہ), "");
  ثابت OT::GSUBGPOS &g = get_gsubgpos_جدول (face, جدول_لیبل_2);
  غیرعلامتی صحیح i;

  برائے (i = 0; i < script_گنتی; i++)
  {
    اگر (g.ڈھونڈو_script_اشاریہ (script_لیبل_فہرست[i], script_اشاریہ))
    {
      اگر (chosen_script)
	*chosen_script = script_لیبل_فہرست[i];
      واپس سچ;
    }
  }

  /* try finding 'DFLT' */
  اگر (g.ڈھونڈو_script_اشاریہ (HB_OT_لیبل_DEFAULT_SCRIPT, script_اشاریہ)) {
    اگر (chosen_script)
      *chosen_script = HB_OT_لیبل_DEFAULT_SCRIPT;
    واپس جھوٹ;
  }

  /* try with 'dflt'; MS site has had typos and many fonts use it now :( */
  اگر (g.ڈھونڈو_script_اشاریہ (HB_OT_لیبل_DEFAULT_LANGUAGE, script_اشاریہ)) {
    اگر (chosen_script)
      *chosen_script = HB_OT_لیبل_DEFAULT_LANGUAGE;
    واپس جھوٹ;
  }

  /* try with 'latn'; some old fonts put their features there even though
     they're really trying to support Thai, for example :( */
  اگر (g.ڈھونڈو_script_اشاریہ (HB_OT_لیبل_LATIN_SCRIPT, script_اشاریہ)) {
    اگر (chosen_script)
      *chosen_script = HB_OT_لیبل_LATIN_SCRIPT;
    واپس جھوٹ;
  }

  اگر (script_اشاریہ) *script_اشاریہ = HB_OT_LAYOUT_NO_SCRIPT_اشاریہ;
  اگر (chosen_script)
    *chosen_script = HB_لیبل_NONE;
  واپس جھوٹ;
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
غیرعلامتی صحیح
hb_ot_layout_جدول_get_feature_لیبل_فہرست (hb_face_t    *face,
				     hb_لیبل_t      جدول_لیبل_2,
				     غیرعلامتی صحیح  آغاز_انحراف,
				     غیرعلامتی صحیح *feature_گنتی_2 /* IN/OUT */,
				     hb_لیبل_t     *feature_لیبل_فہرست  /*برآمد*/)
{
  ثابت OT::GSUBGPOS &g = get_gsubgpos_جدول (face, جدول_لیبل_2);

  واپس g.get_feature_لیبل_فہرست (آغاز_انحراف, feature_گنتی_2, feature_لیبل_فہرست);
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
منطقی
hb_ot_layout_جدول_ڈھونڈو_feature (hb_face_t    *face,
				 hb_لیبل_t      جدول_لیبل_2,
				 hb_لیبل_t      feature_لیبل,
				 غیرعلامتی صحیح *feature_اشاریہ /*برآمد*/)
{
  static_assert ((OT::اشاریہ::NOT_ملا_اشاریہ == HB_OT_LAYOUT_NO_FEATURE_اشاریہ), "");
  ثابت OT::GSUBGPOS &g = get_gsubgpos_جدول (face, جدول_لیبل_2);

  غیرعلامتی صحیح عدد_features = g.get_feature_گنتی ();
  برائے (غیرعلامتی صحیح i = 0; i < عدد_features; i++)
  {
    اگر (feature_لیبل == g.get_feature_لیبل (i)) {
      اگر (feature_اشاریہ) *feature_اشاریہ = i;
      واپس سچ;
    }
  }

  اگر (feature_اشاریہ) *feature_اشاریہ = HB_OT_LAYOUT_NO_FEATURE_اشاریہ;
  واپس جھوٹ;
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
غیرعلامتی صحیح
hb_ot_layout_script_get_language_لیبل_فہرست (hb_face_t    *face,
				       hb_لیبل_t      جدول_لیبل_2,
				       غیرعلامتی صحیح  script_اشاریہ,
				       غیرعلامتی صحیح  آغاز_انحراف,
				       غیرعلامتی صحیح *language_گنتی /* IN/OUT */,
				       hb_لیبل_t     *language_لیبل_فہرست  /*برآمد*/)
{
  ثابت OT::Script &s = get_gsubgpos_جدول (face, جدول_لیبل_2).get_script (script_اشاریہ);

  واپس s.get_lang_sys_لیبل_فہرست (آغاز_انحراف, language_گنتی, language_لیبل_فہرست);
}


#اگر_غیر_تعریف_شدہ HB_DISABLE_DEPRECATED
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
hb_ot_layout_script_ڈھونڈو_language (hb_face_t    *face,
				   hb_لیبل_t      جدول_لیبل_2,
				   غیرعلامتی صحیح  script_اشاریہ,
				   hb_لیبل_t      language_لیبل,
				   غیرعلامتی صحیح *language_اشاریہ)
{
  واپس hb_ot_layout_script_select_language (face,
					      جدول_لیبل_2,
					      script_اشاریہ,
					      1,
					      &language_لیبل,
					      language_اشاریہ);
}
#اگر_ختم


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
				     hb_لیبل_t        جدول_لیبل_2,
				     غیرعلامتی صحیح    script_اشاریہ,
				     غیرعلامتی صحیح    language_گنتی,
				     ثابت hb_لیبل_t *language_لیبل_فہرست,
				     غیرعلامتی صحیح   *language_اشاریہ /*برآمد*/,
				     hb_لیبل_t       *chosen_language /*برآمد*/)
{
  static_assert ((OT::اشاریہ::NOT_ملا_اشاریہ == HB_OT_LAYOUT_DEFAULT_LANGUAGE_اشاریہ), "");
  ثابت OT::Script &s = get_gsubgpos_جدول (face, جدول_لیبل_2).get_script (script_اشاریہ);
  غیرعلامتی صحیح i;

  برائے (i = 0; i < language_گنتی; i++)
  {
    اگر (s.ڈھونڈو_lang_sys_اشاریہ (language_لیبل_فہرست[i], language_اشاریہ))
    {
      اگر (chosen_language)
        *chosen_language = language_لیبل_فہرست[i];
      واپس سچ;
    }
  }

  /* try finding 'dflt' */
  اگر (s.ڈھونڈو_lang_sys_اشاریہ (HB_OT_لیبل_DEFAULT_LANGUAGE, language_اشاریہ))
  {
    اگر (chosen_language)
      *chosen_language = HB_OT_لیبل_DEFAULT_LANGUAGE;
    واپس جھوٹ;
  }

  اگر (language_اشاریہ)
    *language_اشاریہ = HB_OT_LAYOUT_DEFAULT_LANGUAGE_اشاریہ;
  اگر (chosen_language)
    *chosen_language = HB_لیبل_NONE;
  واپس جھوٹ;
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
				     hb_لیبل_t        جدول_لیبل_2,
				     غیرعلامتی صحیح    script_اشاریہ,
				     غیرعلامتی صحیح    language_گنتی,
				     ثابت hb_لیبل_t *language_لیبل_فہرست,
				     غیرعلامتی صحیح   *language_اشاریہ /*برآمد*/)
{
  واپس hb_ot_layout_script_select_language2 (face, جدول_لیبل_2,
					       script_اشاریہ,
					       language_گنتی, language_لیبل_فہرست,
					       language_اشاریہ, خالی_اشاریہ);
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
hb_ot_layout_language_get_required_feature_اشاریہ (hb_face_t    *face,
						  hb_لیبل_t      جدول_لیبل_2,
						  غیرعلامتی صحیح  script_اشاریہ,
						  غیرعلامتی صحیح  language_اشاریہ,
						  غیرعلامتی صحیح *feature_اشاریہ /*برآمد*/)
{
  واپس hb_ot_layout_language_get_required_feature (face,
						     جدول_لیبل_2,
						     script_اشاریہ,
						     language_اشاریہ,
						     feature_اشاریہ,
						     خالی_اشاریہ);
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
					    hb_لیبل_t      جدول_لیبل_2,
					    غیرعلامتی صحیح  script_اشاریہ,
					    غیرعلامتی صحیح  language_اشاریہ,
					    غیرعلامتی صحیح *feature_اشاریہ /*برآمد*/,
					    hb_لیبل_t     *feature_لیبل   /*برآمد*/)
{
  ثابت OT::GSUBGPOS &g = get_gsubgpos_جدول (face, جدول_لیبل_2);
  ثابت OT::LangSys &l = g.get_script (script_اشاریہ).get_lang_sys (language_اشاریہ);

  غیرعلامتی صحیح اشاریہ_2 = l.get_required_feature_اشاریہ ();
  اگر (feature_اشاریہ) *feature_اشاریہ = اشاریہ_2;
  اگر (feature_لیبل) *feature_لیبل = g.get_feature_لیبل (اشاریہ_2);

  واپس l.has_required_feature ();
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
غیرعلامتی صحیح
hb_ot_layout_language_get_feature_indexes (hb_face_t    *face,
					   hb_لیبل_t      جدول_لیبل_2,
					   غیرعلامتی صحیح  script_اشاریہ,
					   غیرعلامتی صحیح  language_اشاریہ,
					   غیرعلامتی صحیح  آغاز_انحراف,
					   غیرعلامتی صحیح *feature_گنتی_2   /* IN/OUT */,
					   غیرعلامتی صحیح *feature_indexes /*برآمد*/)
{
  ثابت OT::GSUBGPOS &g = get_gsubgpos_جدول (face, جدول_لیبل_2);
  ثابت OT::LangSys &l = g.get_script (script_اشاریہ).get_lang_sys (language_اشاریہ);

  واپس l.get_feature_indexes (آغاز_انحراف, feature_گنتی_2, feature_indexes);
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
غیرعلامتی صحیح
hb_ot_layout_language_get_feature_لیبل_فہرست (hb_face_t    *face,
					hb_لیبل_t      جدول_لیبل_2,
					غیرعلامتی صحیح  script_اشاریہ,
					غیرعلامتی صحیح  language_اشاریہ,
					غیرعلامتی صحیح  آغاز_انحراف,
					غیرعلامتی صحیح *feature_گنتی_2 /* IN/OUT */,
					hb_لیبل_t     *feature_لیبل_فہرست  /*برآمد*/)
{
  ثابت OT::GSUBGPOS &g = get_gsubgpos_جدول (face, جدول_لیبل_2);
  ثابت OT::LangSys &l = g.get_script (script_اشاریہ).get_lang_sys (language_اشاریہ);

  static_assert ((حجم (غیرعلامتی صحیح) == حجم (hb_لیبل_t)), "");
  غیرعلامتی صحیح ret = l.get_feature_indexes (آغاز_انحراف, feature_گنتی_2, (غیرعلامتی صحیح *) feature_لیبل_فہرست);

  اگر (feature_گنتی_2 && feature_لیبل_فہرست) {
    غیرعلامتی صحیح گنتی = *feature_گنتی_2;
    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
      feature_لیبل_فہرست[i] = g.get_feature_لیبل ((غیرعلامتی صحیح) feature_لیبل_فہرست[i]);
  }

  واپس ret;
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
hb_ot_layout_language_ڈھونڈو_feature (hb_face_t    *face,
				    hb_لیبل_t      جدول_لیبل_2,
				    غیرعلامتی صحیح  script_اشاریہ,
				    غیرعلامتی صحیح  language_اشاریہ,
				    hb_لیبل_t      feature_لیبل,
				    غیرعلامتی صحیح *feature_اشاریہ /*برآمد*/)
{
  static_assert ((OT::اشاریہ::NOT_ملا_اشاریہ == HB_OT_LAYOUT_NO_FEATURE_اشاریہ), "");
  ثابت OT::GSUBGPOS &g = get_gsubgpos_جدول (face, جدول_لیبل_2);
  ثابت OT::LangSys &l = g.get_script (script_اشاریہ).get_lang_sys (language_اشاریہ);

  غیرعلامتی صحیح عدد_features = l.get_feature_گنتی ();
  برائے (غیرعلامتی صحیح i = 0; i < عدد_features; i++) {
    غیرعلامتی صحیح f_اشاریہ = l.get_feature_اشاریہ (i);

    اگر (feature_لیبل == g.get_feature_لیبل (f_اشاریہ)) {
      اگر (feature_اشاریہ) *feature_اشاریہ = f_اشاریہ;
      واپس سچ;
    }
  }

  اگر (feature_اشاریہ) *feature_اشاریہ = HB_OT_LAYOUT_NO_FEATURE_اشاریہ;
  واپس جھوٹ;
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
غیرعلامتی صحیح
hb_ot_layout_feature_get_lookups (hb_face_t    *face,
				  hb_لیبل_t      جدول_لیبل_2,
				  غیرعلامتی صحیح  feature_اشاریہ,
				  غیرعلامتی صحیح  آغاز_انحراف,
				  غیرعلامتی صحیح *lookup_گنتی   /* IN/OUT */,
				  غیرعلامتی صحیح *lookup_indexes /*برآمد*/)
{
  واپس hb_ot_layout_feature_with_variations_get_lookups (face,
							   جدول_لیبل_2,
							   feature_اشاریہ,
							   HB_OT_LAYOUT_NO_VARIATIONS_اشاریہ,
							   آغاز_انحراف,
							   lookup_گنتی,
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
غیرعلامتی صحیح
hb_ot_layout_جدول_get_lookup_گنتی (hb_face_t    *face,
				     hb_لیبل_t      جدول_لیبل_2)
{
  واپس get_gsubgpos_جدول (face, جدول_لیبل_2).get_lookup_گنتی ();
}


ساخت hb_collect_features_context_t
{
  hb_collect_features_context_t (hb_face_t *face,
				 hb_لیبل_t   جدول_لیبل_2,
				 hb_سیٹ_t  *feature_indices_,
				 ثابت hb_لیبل_t *features)

    : g (get_gsubgpos_جدول (face, جدول_لیبل_2)),
      feature_indices (feature_indices_),
      has_feature_filter (جھوٹ),
      script_گنتی (0),langsys_گنتی (0), feature_اشاریہ_گنتی (0)
  {
    compute_feature_filter (features);
  }

  خالی compute_feature_filter (ثابت hb_لیبل_t *features)
  {
    اگر (features == خالی_اشاریہ)
    {
      has_feature_filter = جھوٹ;
      واپس;
    }

    has_feature_filter = سچ;
    hb_سیٹ_t features_سیٹ;
    برائے (; *features; features++)
      features_سیٹ.جوڑو (*features);

    برائے (غیرعلامتی i = 0; i < g.get_feature_گنتی (); i++)
    {
      hb_لیبل_t لیبل_2 = g.get_feature_لیبل (i);
      اگر (features_سیٹ.has (لیبل_2))
	feature_indices_filter.جوڑو(i);
    }
  }

  منطقی دیکھا (ثابت OT::Script &s)
  {
    /* We might have Null() object here.  Don't want to involve
     * that in the memoize.  So, detect empty objects and return. */
    اگر (unlikely (!s.has_default_lang_sys () &&
		  !s.get_lang_sys_گنتی ()))
      واپس سچ;

    اگر (script_گنتی++ > HB_MAX_SCRIPTS)
      واپس سچ;

    واپس دیکھا (s, دیکھا_script);
  }
  منطقی دیکھا (ثابت OT::LangSys &l)
  {
    /* We might have Null() object here.  Don't want to involve
     * that in the memoize.  So, detect empty objects and return. */
    اگر (unlikely (!l.has_required_feature () &&
		  !l.get_feature_گنتی ()))
      واپس سچ;

    اگر (langsys_گنتی++ > HB_MAX_LANGSYS)
      واپس سچ;

    واپس دیکھا (l, دیکھا_langsys);
  }

  منطقی دیکھا_feature_indices (غیرعلامتی گنتی)
  {
    feature_اشاریہ_گنتی += گنتی;
    واپس feature_اشاریہ_گنتی > HB_MAX_FEATURE_INDICES;
  }

  نجی:
  سانچہ <قسم_نام T>
  منطقی دیکھا (ثابت T &p, hb_سیٹ_t &دیکھا_سیٹ)
  {
    hb_codepoint_t delta = (hb_codepoint_t) ((uintptr_t) &p - (uintptr_t) &g);
     اگر (دیکھا_سیٹ.has (delta))
      واپس سچ;

    دیکھا_سیٹ.جوڑو (delta);
    واپس جھوٹ;
  }

  عوامی:
  ثابت OT::GSUBGPOS &g;
  hb_سیٹ_t *feature_indices;
  hb_سیٹ_t  feature_indices_filter;
  منطقی has_feature_filter;

  نجی:
  hb_سیٹ_t دیکھا_script;
  hb_سیٹ_t دیکھا_langsys;
  غیرعلامتی صحیح script_گنتی;
  غیرعلامتی صحیح langsys_گنتی;
  غیرعلامتی صحیح feature_اشاریہ_گنتی;
};

ساکن خالی
langsys_collect_features (hb_collect_features_context_t *c,
			  ثابت OT::LangSys  &l)
{
  اگر (c->دیکھا (l)) واپس;

  اگر (!c->has_feature_filter)
  {
    /* All features. */
    اگر (l.has_required_feature () && !c->دیکھا_feature_indices (1))
      c->feature_indices->جوڑو (l.get_required_feature_اشاریہ ());

    // TODO(garretrieger): filter out indices >= feature count?
    اگر (!c->دیکھا_feature_indices (l.feature_اشاریہ_2.len))
      l.جوڑو_feature_indexes_مقصد (c->feature_indices);
  }
  ورنہ
  {
    اگر (c->feature_indices_filter.is_empty()) واپس;
    غیرعلامتی صحیح عدد_features = l.get_feature_گنتی ();
    برائے (غیرعلامتی صحیح i = 0; i < عدد_features; i++)
    {
      غیرعلامتی صحیح feature_اشاریہ = l.get_feature_اشاریہ (i);
      اگر (!c->feature_indices_filter.has (feature_اشاریہ)) جاری;

      c->feature_indices->جوڑو (feature_اشاریہ);
      c->feature_indices_filter.del (feature_اشاریہ);
    }
  }
}

ساکن خالی
script_collect_features (hb_collect_features_context_t *c,
			 ثابت OT::Script   &s,
			 ثابت hb_لیبل_t *languages)
{
  اگر (c->دیکھا (s)) واپس;

  اگر (!languages)
  {
    /* All languages. */
    اگر (s.has_default_lang_sys ())
      langsys_collect_features (c,
				s.get_default_lang_sys ());


    غیرعلامتی صحیح گنتی = s.get_lang_sys_گنتی ();
    برائے (غیرعلامتی صحیح language_اشاریہ = 0; language_اشاریہ < گنتی; language_اشاریہ++)
      langsys_collect_features (c,
				s.get_lang_sys (language_اشاریہ));
  }
  ورنہ
  {
    برائے (; *languages; languages++)
    {
      غیرعلامتی صحیح language_اشاریہ;
      اگر (s.ڈھونڈو_lang_sys_اشاریہ (*languages, &language_اشاریہ))
	langsys_collect_features (c,
				  s.get_lang_sys (language_اشاریہ));

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
خالی
hb_ot_layout_collect_features (hb_face_t      *face,
			       hb_لیبل_t        جدول_لیبل_2,
			       ثابت hb_لیبل_t *scripts,
			       ثابت hb_لیبل_t *languages,
			       ثابت hb_لیبل_t *features,
			       hb_سیٹ_t       *feature_indexes /*برآمد*/)
{
  hb_collect_features_context_t c (face, جدول_لیبل_2, feature_indexes, features);
  اگر (!scripts)
  {
    /* All scripts. */
    غیرعلامتی صحیح گنتی = c.g.get_script_گنتی ();
    برائے (غیرعلامتی صحیح script_اشاریہ = 0; script_اشاریہ < گنتی; script_اشاریہ++)
      script_collect_features (&c,
			       c.g.get_script (script_اشاریہ),
			       languages);
  }
  ورنہ
  {
    برائے (; *scripts; scripts++)
    {
      غیرعلامتی صحیح script_اشاریہ;
      اگر (c.g.ڈھونڈو_script_اشاریہ (*scripts, &script_اشاریہ))
	script_collect_features (&c,
				 c.g.get_script (script_اشاریہ),
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
خالی
hb_ot_layout_collect_features_map (hb_face_t      *face,
				   hb_لیبل_t        جدول_لیبل_2,
				   غیرعلامتی        script_اشاریہ,
				   غیرعلامتی        language_اشاریہ,
				   hb_map_t       *feature_map /*برآمد*/)
{
  ثابت OT::GSUBGPOS &g = get_gsubgpos_جدول (face, جدول_لیبل_2);
  ثابت OT::LangSys &l = g.get_script (script_اشاریہ).get_lang_sys (language_اشاریہ);

  غیرعلامتی صحیح گنتی = l.get_feature_indexes (0, خالی_اشاریہ, خالی_اشاریہ);
  feature_map->alloc (گنتی);

  /* Loop in reverse, such that earlier entries win. That emulates
   * a linear search, which seems to be what other implementations do.
   * We found that with arialuni_t.ttf, the "ur" language system has
   * duplicate features, and the earlier ones work but not later ones.
   */
  برائے (غیرعلامتی صحیح i = گنتی; i; i--)
  {
    غیرعلامتی feature_اشاریہ = 0;
    غیرعلامتی feature_گنتی_2 = 1;
    l.get_feature_indexes (i - 1, &feature_گنتی_2, &feature_اشاریہ);
    اگر (!feature_گنتی_2)
      توڑو;
    hb_لیبل_t feature_لیبل = g.get_feature_لیبل (feature_اشاریہ);
    feature_map->سیٹ (feature_لیبل, feature_اشاریہ);
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
خالی
hb_ot_layout_collect_lookups (hb_face_t      *face,
			      hb_لیبل_t        جدول_لیبل_2,
			      ثابت hb_لیبل_t *scripts,
			      ثابت hb_لیبل_t *languages,
			      ثابت hb_لیبل_t *features,
			      hb_سیٹ_t       *lookup_indexes /*برآمد*/)
{
  ثابت OT::GSUBGPOS &g = get_gsubgpos_جدول (face, جدول_لیبل_2);

  hb_سیٹ_t feature_indexes;
  hb_ot_layout_collect_features (face, جدول_لیبل_2, scripts, languages, features, &feature_indexes);

  برائے (خودکار feature_اشاریہ : feature_indexes)
    g.get_feature (feature_اشاریہ).جوڑو_lookup_indexes_مقصد (lookup_indexes);

  g.feature_variation_collect_lookups (&feature_indexes, خالی_اشاریہ, lookup_indexes);
}


#اگر_غیر_تعریف_شدہ HB_NO_LAYOUT_COLLECT_GLYPHS
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
خالی
hb_ot_layout_lookup_collect_glyphs (hb_face_t    *face,
				    hb_لیبل_t      جدول_لیبل_2,
				    غیرعلامتی صحیح  lookup_اشاریہ,
				    hb_سیٹ_t     *glyphs_before, /* OUT.  May be NULL */
				    hb_سیٹ_t     *glyphs_داخلہ,  /* OUT.  May be NULL */
				    hb_سیٹ_t     *glyphs_after,  /* OUT.  May be NULL */
				    hb_سیٹ_t     *glyphs_output  /* OUT.  May be NULL */)
{
  OT::hb_collect_glyphs_context_t c (face,
				     glyphs_before,
				     glyphs_داخلہ,
				     glyphs_after,
				     glyphs_output);

  انتخاب (جدول_لیبل_2)
  {
    حالت HB_OT_لیبل_GSUB:
    {
      ثابت OT::SubstLookup& l = face->جدول.GSUB->جدول->get_lookup (lookup_اشاریہ);
      l.collect_glyphs (&c);
      واپس;
    }
    حالت HB_OT_لیبل_GPOS:
    {
      ثابت OT::PosLookup& l = face->جدول.GPOS->جدول->get_lookup (lookup_اشاریہ);
      l.collect_glyphs (&c);
      واپس;
    }
  }
}
#اگر_ختم


/* Variations support */

منطقی
hb_ot_layout_جدول_get_feature_variations_state (hb_face_t             *face,
						   hb_لیبل_t               جدول_لیبل_2,
						   ثابت صحیح             *coords,
						   غیرعلامتی صحیح           عدد_coords,
						   غیرعلامتی صحیح          *variations_اشاریہ,
						   hb_سمتیہ_t<غیرعلامتی> *lookup_conditions)
{
  ثابت OT::GSUBGPOS &g = get_gsubgpos_جدول (face, جدول_لیبل_2);
  ثابت OT::GDEF &gdef = *face->جدول.GDEF->جدول;

  خودکار instancer = OT::شے_Var_Store_Instancer (&gdef.get_var_store (), خالی_اشاریہ,
					      hb_array (coords, عدد_coords));

  واپس g.get_feature_variations_state (coords,
					 عدد_coords,
					 variations_اشاریہ,
					 &instancer,
					 lookup_conditions);
}

منطقی
_hb_ot_layout_feature_get_lookup_variations (hb_face_t *face,
                                             hb_لیبل_t جدول_لیبل_2,
                                             غیرعلامتی feature_اشاریہ,
                                             غیرعلامتی variations_اشاریہ,
                                             hb_sorted_array_t<ثابت غیرعلامتی> conditions,
                                             hb_سیٹ_t *lookup_indexes)
{
  واپس get_gsubgpos_جدول (face, جدول_لیبل_2).get_lookup_variations (
      feature_اشاریہ, variations_اشاریہ, conditions, lookup_indexes);
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
hb_ot_layout_جدول_ڈھونڈو_feature_variations (hb_face_t    *face,
					    hb_لیبل_t      جدول_لیبل_2,
					    ثابت صحیح    *coords,
					    غیرعلامتی صحیح  عدد_coords,
					    غیرعلامتی صحیح *variations_اشاریہ /* out */)
{
  ثابت OT::GSUBGPOS &g = get_gsubgpos_جدول (face, جدول_لیبل_2);
  ثابت OT::GDEF &gdef = *face->جدول.GDEF->جدول;

  خودکار instancer = OT::شے_Var_Store_Instancer(&gdef.get_var_store(), خالی_اشاریہ,
					     hb_array (coords, عدد_coords));

  واپس g.ڈھونڈو_variations_اشاریہ (coords, عدد_coords, variations_اشاریہ, &instancer);
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
غیرعلامتی صحیح
hb_ot_layout_feature_with_variations_get_lookups (hb_face_t    *face,
						  hb_لیبل_t      جدول_لیبل_2,
						  غیرعلامتی صحیح  feature_اشاریہ,
						  غیرعلامتی صحیح  variations_اشاریہ,
						  غیرعلامتی صحیح  آغاز_انحراف,
						  غیرعلامتی صحیح *lookup_گنتی /* IN/OUT */,
						  غیرعلامتی صحیح *lookup_indexes /*برآمد*/)
{
  static_assert ((OT::FeatureVariations::NOT_ملا_اشاریہ == HB_OT_LAYOUT_NO_VARIATIONS_اشاریہ), "");
  ثابت OT::GSUBGPOS &g = get_gsubgpos_جدول (face, جدول_لیبل_2);

  ثابت OT::Feature &f = g.get_feature_variation (feature_اشاریہ, variations_اشاریہ);

  واپس f.get_lookup_indexes (آغاز_انحراف, lookup_گنتی, lookup_indexes);
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
  واپس face->جدول.GSUB->جدول->has_ڈیٹا ();
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
				      غیرعلامتی صحیح          lookup_اشاریہ,
				      ثابت hb_codepoint_t *glyphs,
				      غیرعلامتی صحیح          glyphs_لمبائی,
				      hb_bool_t             zero_context)
{
  خودکار &gsub = face->جدول.GSUB;
  اگر (unlikely (lookup_اشاریہ >= gsub->lookup_گنتی)) واپس جھوٹ;
  OT::hb_would_apply_context_t c (face, glyphs, glyphs_لمبائی, (منطقی) zero_context);

  ثابت OT::SubstLookup& l = gsub->جدول->get_lookup (lookup_اشاریہ);
  خودکار *accel = gsub->get_accel (lookup_اشاریہ);
  واپس accel && l.would_apply (&c, accel);
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
خالی
hb_ot_layout_substitute_آغاز (hb_font_t    *font,
			       hb_بفر_t  *بفر)
{
  hb_ot_layout_سیٹ_glyph_props<جھوٹ> (font, بفر);
}

خالی
hb_ot_layout_substitute_آغاز_with_digest (hb_font_t    *font,
					   hb_بفر_t  *بفر)
{
  بفر->digest = hb_سیٹ_digest_t ();
  hb_ot_layout_سیٹ_glyph_props<سچ> (font, بفر);
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
خالی
hb_ot_layout_lookup_substitute_احاطہ (hb_face_t    *face,
					غیرعلامتی صحیح  lookup_اشاریہ,
					hb_سیٹ_t     *glyphs /*برآمد*/)
{
  hb_map_t مکمل_lookups_glyph_گنتی;
  hb_hashmap_t<غیرعلامتی, hb::unique_ptr<hb_سیٹ_t>> مکمل_lookups_glyph_سیٹ;
  OT::hb_احاطہ_context_t c (face, glyphs, &مکمل_lookups_glyph_گنتی, &مکمل_lookups_glyph_سیٹ);

  ثابت OT::SubstLookup& l = face->جدول.GSUB->جدول->get_lookup (lookup_اشاریہ);

  l.احاطہ (&c, lookup_اشاریہ);
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
خالی
hb_ot_layout_lookups_substitute_احاطہ (hb_face_t      *face,
					 ثابت hb_سیٹ_t *lookups,
					 hb_سیٹ_t       *glyphs /*برآمد*/)
{
  hb_map_t مکمل_lookups_glyph_گنتی;
  hb_hashmap_t<غیرعلامتی, hb::unique_ptr<hb_سیٹ_t>> مکمل_lookups_glyph_سیٹ;
  OT::hb_احاطہ_context_t c (face, glyphs, &مکمل_lookups_glyph_گنتی, &مکمل_lookups_glyph_سیٹ);
  ثابت GSUB& gsub = *face->جدول.GSUB->جدول;

  غیرعلامتی صحیح iteration_گنتی = 0;
  غیرعلامتی صحیح glyphs_لمبائی;
  کرو
  {
    c.reset_lookup_دورہ_گنتی ();
    glyphs_لمبائی = glyphs->get_population ();
    اگر (lookups)
    {
      برائے (خودکار lookup_اشاریہ : *lookups)
	gsub.get_lookup (lookup_اشاریہ).احاطہ (&c, lookup_اشاریہ);
    }
    ورنہ
    {
      برائے (غیرعلامتی صحیح i = 0; i < gsub.get_lookup_گنتی (); i++)
	gsub.get_lookup (i).احاطہ (&c, i);
    }
  } جب (iteration_گنتی++ <= HB_احاطہ_MAX_STAGES &&
	   glyphs_لمبائی != glyphs->get_population ());
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
  واپس face->جدول.GPOS->جدول->has_ڈیٹا ();
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
خالی
hb_ot_layout_position_آغاز (hb_font_t *font, hb_بفر_t *بفر)
{
  GPOS::position_آغاز (font, بفر);
}


/**
 * hb_ot_layout_position_finish_advances:
 * @font: #hb_font_t to use
 * @buffer: #hb_buffer_t buffer to work upon
 *
 * Called after positioning lookups are performed, to finish glyph advances.
 *
 **/
خالی
hb_ot_layout_position_اختتام_advances (hb_font_t *font, hb_بفر_t *بفر)
{
  GPOS::position_اختتام_advances (font, بفر);
}

/**
 * hb_ot_layout_position_finish_offsets:
 * @font: #hb_font_t to use
 * @buffer: #hb_buffer_t buffer to work upon
 *
 * Called after positioning lookups are performed, to finish glyph offsets.
 *
 **/
خالی
hb_ot_layout_position_اختتام_offsets (hb_font_t *font, hb_بفر_t *بفر)
{
  GPOS::position_اختتام_offsets (font, بفر);
}


#اگر_غیر_تعریف_شدہ HB_NO_LAYOUT_FEATURE_PARAMS
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
hb_ot_layout_get_سائز_params (hb_face_t       *face,
			      غیرعلامتی صحیح    *design_سائز_2,       /* OUT.  May be NULL */
			      غیرعلامتی صحیح    *subfamily_id,      /* OUT.  May be NULL */
			      hb_ot_name_id_t *subfamily_name_id, /* OUT.  May be NULL */
			      غیرعلامتی صحیح    *range_آغاز_2,       /* OUT.  May be NULL */
			      غیرعلامتی صحیح    *range_انتہا_2          /* OUT.  May be NULL */)
{
  ثابت GPOS &gpos = *face->جدول.GPOS->جدول;
  ثابت hb_لیبل_t لیبل_2 = HB_لیبل ('s','i','z','e');

  غیرعلامتی صحیح عدد_features = gpos.get_feature_گنتی ();
  برائے (غیرعلامتی صحیح i = 0; i < عدد_features; i++)
  {
    اگر (لیبل_2 == gpos.get_feature_لیبل (i))
    {
      ثابت OT::Feature &f = gpos.get_feature (i);
      ثابت OT::Feature_Params_سائز &params = f.get_feature_params ().get_سائز_params (لیبل_2);

      اگر (params.design_سائز)
      {
	اگر (design_سائز_2) *design_سائز_2 = params.design_سائز;
	اگر (subfamily_id) *subfamily_id = params.subfamilyID;
	اگر (subfamily_name_id) *subfamily_name_id = params.subfamilyNameID;
	اگر (range_آغاز_2) *range_آغاز_2 = params.range_آغاز;
	اگر (range_انتہا_2) *range_انتہا_2 = params.range_انتہا;

	واپس سچ;
      }
    }
  }

  اگر (design_سائز_2) *design_سائز_2 = 0;
  اگر (subfamily_id) *subfamily_id = 0;
  اگر (subfamily_name_id) *subfamily_name_id = HB_OT_NAME_ID_INVALID;
  اگر (range_آغاز_2) *range_آغاز_2 = 0;
  اگر (range_انتہا_2) *range_انتہا_2 = 0;

  واپس جھوٹ;
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
				   hb_لیبل_t         جدول_لیبل_2,
				   غیرعلامتی صحیح     feature_اشاریہ,
				   hb_ot_name_id_t *label_id,             /* OUT.  May be NULL */
				   hb_ot_name_id_t *tooltip_id,           /* OUT.  May be NULL */
				   hb_ot_name_id_t *sample_id,            /* OUT.  May be NULL */
				   غیرعلامتی صحیح    *عدد_named_parameters, /* OUT.  May be NULL */
				   hb_ot_name_id_t *پہلا_param_id        /* OUT.  May be NULL */)
{
  ثابت OT::GSUBGPOS &g = get_gsubgpos_جدول (face, جدول_لیبل_2);

  hb_لیبل_t feature_لیبل = g.get_feature_لیبل (feature_اشاریہ);
  ثابت OT::Feature &f = g.get_feature (feature_اشاریہ);

  ثابت OT::FeatureParams &feature_params = f.get_feature_params ();
  اگر (&feature_params != &Null (OT::FeatureParams))
  {
    ثابت OT::Feature_Params_Stylistic_سیٹ& ss_params =
      feature_params.get_stylistic_سیٹ_params (feature_لیبل);
    اگر (&ss_params != &Null (OT::Feature_Params_Stylistic_سیٹ)) /* ssXX */
    {
      اگر (label_id) *label_id = ss_params.uiNameID;
      // ssXX features don't have the rest
      اگر (tooltip_id) *tooltip_id = HB_OT_NAME_ID_INVALID;
      اگر (sample_id) *sample_id = HB_OT_NAME_ID_INVALID;
      اگر (عدد_named_parameters) *عدد_named_parameters = 0;
      اگر (پہلا_param_id) *پہلا_param_id = HB_OT_NAME_ID_INVALID;
      واپس سچ;
    }
    ثابت OT::FeatureParamsCharacterVariants& cv_params =
      feature_params.get_character_variants_params (feature_لیبل);
    اگر (&cv_params != &Null (OT::FeatureParamsCharacterVariants)) /* cvXX */
    {
      اگر (label_id) *label_id = cv_params.featUILableNameID;
      اگر (tooltip_id) *tooltip_id = cv_params.feat_UITooltip_متن_Name_ID;
      اگر (sample_id) *sample_id = cv_params.sample_متن_Name_ID;
      اگر (عدد_named_parameters) *عدد_named_parameters = cv_params.عدد_Named_Parameters;
      اگر (پہلا_param_id) *پہلا_param_id = cv_params.پہلا_Param_UILabel_Name_ID;
      واپس سچ;
    }
  }

  اگر (label_id) *label_id = HB_OT_NAME_ID_INVALID;
  اگر (tooltip_id) *tooltip_id = HB_OT_NAME_ID_INVALID;
  اگر (sample_id) *sample_id = HB_OT_NAME_ID_INVALID;
  اگر (عدد_named_parameters) *عدد_named_parameters = 0;
  اگر (پہلا_param_id) *پہلا_param_id = HB_OT_NAME_ID_INVALID;
  واپس جھوٹ;
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
غیرعلامتی صحیح
hb_ot_layout_feature_get_characters (hb_face_t      *face,
				     hb_لیبل_t        جدول_لیبل_2,
				     غیرعلامتی صحیح    feature_اشاریہ,
				     غیرعلامتی صحیح    آغاز_انحراف,
				     غیرعلامتی صحیح   *char_گنتی, /* IN/OUT.  May be NULL */
				     hb_codepoint_t *characters  /* OUT.     May be NULL */)
{
  ثابت OT::GSUBGPOS &g = get_gsubgpos_جدول (face, جدول_لیبل_2);
  واپس g.get_feature (feature_اشاریہ)
	  .get_feature_params ()
	  .get_character_variants_params(g.get_feature_لیبل (feature_اشاریہ))
	  .get_characters (آغاز_انحراف, char_گنتی, characters);
}
#اگر_ختم


/*
 * Parts of different types are implemented here such that they have direct
 * access to GSUB/GPOS lookups.
 */


ساخت GSUBProxy
{
  ساکن constexpr غیرعلامتی جدول_اشاریہ = 0u;
  ساکن constexpr منطقی always_inplace = جھوٹ;
  قسمتعریف OT::SubstLookup Lookup;

  GSUBProxy (hb_face_t *face) :
    accel (*face->جدول.GSUB) {}

  ثابت GSUB::accelerator_t &accel;
};

ساخت GPOSProxy
{
  ساکن constexpr غیرعلامتی جدول_اشاریہ = 1u;
  ساکن constexpr منطقی always_inplace = سچ;
  قسمتعریف OT::PosLookup Lookup;

  GPOSProxy (hb_face_t *face) :
    accel (*face->جدول.GPOS) {}

  ثابت GPOS::accelerator_t &accel;
};


ساکن اندرونی منطقی
apply_forward (OT::hb_ot_apply_context_t *c,
	       ثابت OT::hb_ot_layout_lookup_accelerator_t &accel)
{
  منطقی use_hot_subtable_cache = accel.cache_enter (c);

  منطقی ret = جھوٹ;
  hb_بفر_t *بفر = c->بفر;
  جب (بفر->successful)
  {
    hb_glyph_info_t *info = بفر->info;
    غیرعلامتی j = بفر->idx;
    جب (j < بفر->len &&
	   !(accel.digest.may_have (info[j].codepoint) &&
	     (info[j].نقاب & c->lookup_نقاب) &&
	     c->check_glyph_property (&info[j], c->lookup_props)))
      j++;
    اگر (unlikely (j > بفر->idx && !بفر->اگلا_glyphs (j - بفر->idx)))
      توڑو;
    اگر (بفر->idx >= بفر->len)
      توڑو;

    اگر (accel.apply (c, use_hot_subtable_cache))
      ret = سچ;
    ورنہ
      (خالی) بفر->اگلا_glyph ();
  }

  اگر (use_hot_subtable_cache)
    accel.cache_leave (c);

  واپس ret;
}

ساکن اندرونی منطقی
apply_backward (OT::hb_ot_apply_context_t *c,
	       ثابت OT::hb_ot_layout_lookup_accelerator_t &accel)
{
  منطقی ret = جھوٹ;
  hb_بفر_t *بفر = c->بفر;
  کرو
  {
    خودکار &cur = بفر->cur();
    اگر (accel.digest.may_have (cur.codepoint) &&
	(cur.نقاب & c->lookup_نقاب) &&
	c->check_glyph_property (&cur, c->lookup_props))
      ret |= accel.apply (c, جھوٹ);

    /* The reverse lookup doesn't "advance" cursor (for good reason). */
    بفر->idx--;
  }
  جب ((صحیح) بفر->idx >= 0);
  واپس ret;
}

سانچہ <قسم_نام Proxy>
ساکن اندرونی منطقی
apply_string (OT::hb_ot_apply_context_t *c,
	      ثابت قسم_نام Proxy::Lookup &lookup,
	      ثابت OT::hb_ot_layout_lookup_accelerator_t &accel)
{
  hb_بفر_t *بفر = c->بفر;

  اگر (unlikely (!بفر->len || !c->lookup_نقاب))
    واپس جھوٹ;

  منطقی ret = جھوٹ;

  c->سیٹ_lookup_props (lookup.get_props ());

  اگر (likely (!lookup.is_الٹ ()))
  {
    /* in/out forward substitution/positioning */
    اگر (!Proxy::always_inplace)
      بفر->clear_output ();

    بفر->idx = 0;
    ret = apply_forward (c, accel);

    اگر (!Proxy::always_inplace)
      بفر->ہمآہنگ ();
  }
  ورنہ
  {
    /* in-place backward substitution/positioning */
    أكد (!بفر->have_output);
    بفر->idx = بفر->len - 1;
    ret = apply_backward (c, accel);
  }

  واپس ret;
}

سانچہ <قسم_نام Proxy>
اندرونی خالی hb_ot_map_t::apply (ثابت Proxy &proxy,
				ثابت hb_ot_shape_plan_t *plan,
				hb_font_t *font,
				hb_بفر_t *بفر) ثابت
{
  ثابت غیرعلامتی صحیح جدول_اشاریہ = proxy.جدول_اشاریہ;
  غیرعلامتی صحیح i = 0;

  خودکار *font_ڈیٹا = font->ڈیٹا.ot.get ();
  خودکار *var_store_cache = (OT::hb_scalar_cache_t *) font_ڈیٹا;

  OT::hb_ot_apply_context_t c (جدول_اشاریہ, font, بفر, proxy.accel.get_blob (), var_store_cache);
  c.سیٹ_recurse_func (Proxy::Lookup::سانچہ dispatch_recurse_func<OT::hb_ot_apply_context_t>);

  برائے (غیرعلامتی صحیح stage_اشاریہ = 0; stage_اشاریہ < stages[جدول_اشاریہ].لمبائی; stage_اشاریہ++)
  {
    ثابت stage_map_t *stage = &stages[جدول_اشاریہ][stage_اشاریہ];
    برائے (; i < stage->آخری_lookup; i++)
    {
      خودکار &lookup = lookups[جدول_اشاریہ][i];

      غیرعلامتی صحیح lookup_اشاریہ = lookup.اشاریہ_2;

      خودکار *accel = proxy.accel.get_accel (lookup_اشاریہ);
      اگر (unlikely (!accel)) جاری;

      اگر (بفر->messaging () &&
	  !بفر->پیغام (font, "start lookup %u feature '%c%c%c%c'", lookup_اشاریہ, HB_UNTAG (lookup.feature_لیبل))) جاری;

      /* Only try applying the lookup if there is any overlap. */
      اگر (accel->digest.may_intersect (بفر->digest) &&
	  ((بفر->flags & HB_بفر_FLAG_PRODUCE_UNSAFE_مقصد_CONCAT) ||
	   accel->digest_دوسرا.may_intersect (بفر->digest)))
      {
	c.سیٹ_lookup_اشاریہ (lookup_اشاریہ);
	c.سیٹ_lookup_نقاب (lookup.نقاب, جھوٹ);
	c.سیٹ_auto_zwj (lookup.auto_zwj, جھوٹ);
	c.سیٹ_auto_zwnj (lookup.auto_zwnj, جھوٹ);
	c.سیٹ_random (lookup.random);
	c.سیٹ_فی_syllable (lookup.فی_syllable, جھوٹ);
	/* apply_string's set_lookup_props initializes the iterators. */

	apply_string<Proxy> (&c,
			     proxy.accel.جدول->get_lookup (lookup_اشاریہ),
			     *accel);
      }
      ورنہ اگر (بفر->messaging ())
	(خالی) بفر->پیغام (font, "skipped lookup %u feature '%c%c%c%c' because no glyph matches", lookup_اشاریہ, HB_UNTAG (lookup.feature_لیبل));

      اگر (بفر->messaging ())
	(خالی) بفر->پیغام (font, "end lookup %u feature '%c%c%c%c'", lookup_اشاریہ, HB_UNTAG (lookup.feature_لیبل));
    }

    اگر (stage->pause_func)
    {
      اگر (stage->pause_func (plan, font, بفر))
      {
	/* Refresh working buffer digest since buffer changed. */
	بفر->update_digest ();
      }
    }
  }
}

خالی hb_ot_map_t::substitute (ثابت hb_ot_shape_plan_t *plan, hb_font_t *font, hb_بفر_t *بفر) ثابت
{
  GSUBProxy proxy (font->face);
  حرف لیبل_2[5] = {0};
  hb_لیبل_مقصد_string (chosen_script[0], لیبل_2);
  اگر (بفر->messaging () &&
      !بفر->پیغام (font, "start table GSUB script tag '%s'", لیبل_2)) واپس;
  apply (proxy, plan, font, بفر);
  اگر (بفر->messaging ())
    (خالی) بفر->پیغام (font, "end table GSUB script tag '%s'", لیبل_2);
}

خالی hb_ot_map_t::position (ثابت hb_ot_shape_plan_t *plan, hb_font_t *font, hb_بفر_t *بفر) ثابت
{
  GPOSProxy proxy (font->face);
  حرف لیبل_2[5] = {0};
  hb_لیبل_مقصد_string (chosen_script[0], لیبل_2);
  اگر (بفر->messaging () &&
      !بفر->پیغام (font, "start table GPOS script tag '%s'", لیبل_2)) واپس;
  apply (proxy, plan, font, بفر);
  اگر (بفر->messaging ())
    (خالی) بفر->پیغام (font, "end table GPOS script tag '%s'", لیبل_2);
}

خالی
hb_ot_layout_substitute_lookup (OT::hb_ot_apply_context_t *c,
				ثابت OT::SubstLookup &lookup,
				ثابت OT::hb_ot_layout_lookup_accelerator_t &accel)
{
  apply_string<GSUBProxy> (c, lookup, accel);
}

#اگر_غیر_تعریف_شدہ HB_NO_بنیاد

ساکن خالی
انتخاب_بنیاد_لیبل_فہرست (hb_script_t    script,
		  hb_language_t  language,
		  hb_لیبل_t      *script_لیبل,
		  hb_لیبل_t      *language_لیبل)
{
  hb_لیبل_t script_لیبل_فہرست[HB_OT_MAX_لیبل_فہرست_فی_SCRIPT];
  غیرعلامتی script_گنتی = ARRAY_لمبائی (script_لیبل_فہرست);

  hb_لیبل_t language_لیبل_فہرست[HB_OT_MAX_لیبل_فہرست_فی_LANGUAGE];
  غیرعلامتی language_گنتی = ARRAY_لمبائی (language_لیبل_فہرست);

  hb_ot_لیبل_فہرست_from_script_and_language (script, language,
				       &script_گنتی, script_لیبل_فہرست,
				       &language_گنتی, language_لیبل_فہرست);

  *script_لیبل = script_گنتی ? script_لیبل_فہرست[script_گنتی - 1] : HB_OT_لیبل_DEFAULT_SCRIPT;
  *language_لیبل = language_گنتی ? language_لیبل_فہرست[language_گنتی - 1] : HB_OT_لیبل_DEFAULT_LANGUAGE;
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
			       hb_لیبل_t           script_لیبل,
			       hb_لیبل_t           language_لیبل,
			       hb_font_extents_t *extents)
{
  hb_position_t min = 0, max = 0;
  اگر (font->face->جدول.بنیاد_4->get_min_max (font, direction, script_لیبل, language_لیبل, HB_لیبل_NONE,
					   &min, &max))
  {
    اگر (extents)
    {
      extents->ascender  = max;
      extents->descender = min;
      extents->سطر_gap  = 0;
    }
    واپس سچ;
  }

  hb_font_get_extents_for_direction (font, direction, extents);
  واپس جھوٹ;
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
  hb_لیبل_t script_لیبل, language_لیبل;
  انتخاب_بنیاد_لیبل_فہرست (script, language, &script_لیبل, &language_لیبل);
  واپس hb_ot_layout_get_font_extents (font,
					direction,
					script_لیبل,
					language_لیبل,
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
hb_ot_layout_baseline_لیبل_t
hb_ot_layout_get_horizontal_baseline_لیبل_for_script (hb_script_t script)
{
  /* Keep in sync with hb_ot_layout_get_baseline_with_fallback */
  انتخاب ((صحیح) script)
  {
    /* Unicode-1.1 additions */
    حالت HB_SCRIPT_BENGALI:
    حالت HB_SCRIPT_DEVANAGARI:
    حالت HB_SCRIPT_GUJARATI:
    حالت HB_SCRIPT_GURMUKHI:
    /* Unicode-2.0 additions */
    حالت HB_SCRIPT_TIBETAN:
    /* Unicode-4.0 additions */
    حالت HB_SCRIPT_LIMBU:
    /* Unicode-4.1 additions */
    حالت HB_SCRIPT_SYLOTI_NAGRI:
    /* Unicode-5.0 additions */
    حالت HB_SCRIPT_PHAGS_PA:
    /* Unicode-5.2 additions */
    حالت HB_SCRIPT_MEETEI_MAYEK:
    /* Unicode-6.1 additions */
    حالت HB_SCRIPT_SHARADA:
    حالت HB_SCRIPT_TAKRI:
    /* Unicode-7.0 additions */
    حالت HB_SCRIPT_MODI:
    حالت HB_SCRIPT_SIDDHAM:
    حالت HB_SCRIPT_TIRHUTA:
    /* Unicode-9.0 additions */
    حالت HB_SCRIPT_MARCHEN:
    حالت HB_SCRIPT_NEWA:
    /* Unicode-10.0 additions */
    حالت HB_SCRIPT_SOYOMBO:
    حالت HB_SCRIPT_ZANABAZAR_مربع:
    /* Unicode-11.0 additions */
    حالت HB_SCRIPT_DOGRA:
    حالت HB_SCRIPT_GUNJALA_GONDI:
    /* Unicode-12.0 additions */
    حالت HB_SCRIPT_NANDINAGARI:
      واپس HB_OT_LAYOUT_BASELINE_لیبل_HANGING;

    /* Unicode-1.1 additions */
    حالت HB_SCRIPT_HANGUL:
    حالت HB_SCRIPT_HAN:
    حالت HB_SCRIPT_HIRAGANA:
    حالت HB_SCRIPT_KATAKANA:
    /* Unicode-3.0 additions */
    حالت HB_SCRIPT_BOPOMOFO:
    /* Unicode-9.0 additions */
    حالت HB_SCRIPT_TANGUT:
    /* Unicode-10.0 additions */
    حالت HB_SCRIPT_NUSHU:
    /* Unicode-13.0 additions */
    حالت HB_SCRIPT_KHITAN_SMALL_SCRIPT:
    /* Unicode-18.0 additions */
    حالت HB_SCRIPT_JURCHEN:
    حالت HB_SCRIPT_SEAL:
      واپس HB_OT_LAYOUT_BASELINE_لیبل_IDEO_FACE_BOTTOM_OR_بایاں;

    طےشدہ:
      واپس HB_OT_LAYOUT_BASELINE_لیبل_ROMAN;
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
			   hb_ot_layout_baseline_لیبل_t  baseline_لیبل,
			   hb_direction_t               direction,
			   hb_لیبل_t                     script_لیبل,
			   hb_لیبل_t                     language_لیبل,
			   hb_position_t               *coord        /* OUT.  May be NULL. */)
{
  واپس font->face->جدول.بنیاد_4->get_baseline (font, baseline_لیبل, direction, script_لیبل, language_لیبل, coord);
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
			    hb_ot_layout_baseline_لیبل_t  baseline_لیبل,
			    hb_direction_t               direction,
			    hb_script_t                  script,
			    hb_language_t                language,
			    hb_position_t               *coord        /* OUT.  May be NULL. */)
{
  hb_لیبل_t script_لیبل, language_لیبل;
  انتخاب_بنیاد_لیبل_فہرست (script, language, &script_لیبل, &language_لیبل);
  واپس hb_ot_layout_get_baseline (font,
				    baseline_لیبل,
				    direction,
				    script_لیبل,
				    language_لیبل,
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
خالی
hb_ot_layout_get_baseline_with_fallback (hb_font_t                   *font,
					 hb_ot_layout_baseline_لیبل_t  baseline_لیبل,
					 hb_direction_t               direction,
					 hb_لیبل_t                     script_لیبل,
					 hb_لیبل_t                     language_لیبل,
					 hb_position_t               *coord /*برآمد*/)
{
  اگر (hb_ot_layout_get_baseline (font,
				 baseline_لیبل,
				 direction,
				 script_لیبل,
				 language_لیبل,
				 coord))
    واپس;

  /* Synthesize missing baselines.
   * See https://www.w3.org/TR/css-inline-3/#baseline-synthesis-fonts
   */
  انتخاب (baseline_لیبل)
  {
  حالت HB_OT_LAYOUT_BASELINE_لیبل_ROMAN:
    *coord = 0; // FIXME origin ?
    توڑو;

  حالت HB_OT_LAYOUT_BASELINE_لیبل_MATH:
    {
      hb_codepoint_t glyph;
      hb_glyph_extents_t extents;
      اگر (HB_DIRECTION_IS_HORIZONTAL (direction) &&
	  (hb_font_get_nominal_glyph (font, 0x2212u, &glyph) ||
	   hb_font_get_nominal_glyph (font, '-', &glyph)) &&
	  hb_font_get_glyph_extents (font, glyph, &extents))
      {
	*coord = hb_saturate_جوڑو (extents.y_bearing, extents.اونچائی / 2);
      }
      ورنہ
      {
	hb_position_t x_اونچائی = font->y_scale / 2;
#اگر_غیر_تعریف_شدہ HB_NO_METRICS
	hb_ot_metrics_get_position_with_fallback (font, HB_OT_METRICS_لیبل_X_اونچائی, &x_اونچائی);
#اگر_ختم
	*coord = x_اونچائی / 2;
      }
    }
    توڑو;

  حالت HB_OT_LAYOUT_BASELINE_لیبل_IDEO_FACE_چوٹی_OR_دایاں:
  حالت HB_OT_LAYOUT_BASELINE_لیبل_IDEO_FACE_BOTTOM_OR_بایاں:
    {
      hb_position_t embox_چوٹی, embox_bottom;

      hb_ot_layout_get_baseline_with_fallback (font,
					       HB_OT_LAYOUT_BASELINE_لیبل_IDEO_EMBOX_چوٹی_OR_دایاں,
					       direction,
					       script_لیبل,
					       language_لیبل,
					       &embox_چوٹی);
      hb_ot_layout_get_baseline_with_fallback (font,
					       HB_OT_LAYOUT_BASELINE_لیبل_IDEO_EMBOX_BOTTOM_OR_بایاں,
					       direction,
					       script_لیبل,
					       language_لیبل,
					       &embox_bottom);

      اگر (baseline_لیبل == HB_OT_LAYOUT_BASELINE_لیبل_IDEO_FACE_چوٹی_OR_دایاں)
	*coord = hb_saturate_جوڑو (embox_چوٹی,
				  hb_clamp_مقصد<hb_position_t> (((int64_t) embox_bottom - embox_چوٹی) / 10));
      ورنہ
	*coord = hb_saturate_جوڑو (embox_bottom,
				  hb_clamp_مقصد<hb_position_t> (((int64_t) embox_چوٹی - embox_bottom) / 10));
    }
    توڑو;

  حالت HB_OT_LAYOUT_BASELINE_لیبل_IDEO_EMBOX_چوٹی_OR_دایاں:
    اگر (hb_ot_layout_get_baseline (font,
				   HB_OT_LAYOUT_BASELINE_لیبل_IDEO_EMBOX_BOTTOM_OR_بایاں,
				   direction,
				   script_لیبل,
				   language_لیبل,
				   coord))
      *coord = hb_saturate_جوڑو (*coord,
				HB_DIRECTION_IS_HORIZONTAL (direction) ? font->y_scale : font->x_scale);
    ورنہ
    {
      hb_font_extents_t font_extents;
      hb_font_get_extents_for_direction (font, direction, &font_extents);
      *coord = font_extents.ascender;
    }
    توڑو;

  حالت HB_OT_LAYOUT_BASELINE_لیبل_IDEO_EMBOX_BOTTOM_OR_بایاں:
    اگر (hb_ot_layout_get_baseline (font,
				   HB_OT_LAYOUT_BASELINE_لیبل_IDEO_EMBOX_چوٹی_OR_دایاں,
				   direction,
				   script_لیبل,
				   language_لیبل,
				   coord))
      *coord = hb_saturate_sub (*coord,
				HB_DIRECTION_IS_HORIZONTAL (direction) ? font->y_scale : font->x_scale);
    ورنہ
    {
      hb_font_extents_t font_extents;
      hb_font_get_extents_for_direction (font, direction, &font_extents);
      *coord = font_extents.descender;
    }
    توڑو;

  حالت HB_OT_LAYOUT_BASELINE_لیبل_HANGING:
    اگر (HB_DIRECTION_IS_HORIZONTAL (direction))
    {
      hb_codepoint_t ch;
      hb_codepoint_t glyph;
      hb_glyph_extents_t extents;

      /* Keep in sync with hb_ot_layout_get_horizontal_baseline_for_script */
      انتخاب ((صحیح) script_لیبل)
      {
      /* Unicode-1.1 additions */
      حالت HB_SCRIPT_BENGALI:          ch = 0x0995u; توڑو;
      حالت HB_SCRIPT_DEVANAGARI:       ch = 0x0915u; توڑو;
      حالت HB_SCRIPT_GUJARATI:         ch = 0x0a95u; توڑو;
      حالت HB_SCRIPT_GURMUKHI:         ch = 0x0a15u; توڑو;
      /* Unicode-2.0 additions */
      حالت HB_SCRIPT_TIBETAN:          ch = 0x0f40u; توڑو;
      /* Unicode-4.0 additions */
      حالت HB_SCRIPT_LIMBU:            ch = 0x1901u; توڑو;
      /* Unicode-4.1 additions */
      حالت HB_SCRIPT_SYLOTI_NAGRI:     ch = 0xa807u; توڑو;
      /* Unicode-5.0 additions */
      حالت HB_SCRIPT_PHAGS_PA:         ch = 0xa840u; توڑو;
      /* Unicode-5.2 additions */
      حالت HB_SCRIPT_MEETEI_MAYEK:     ch = 0xabc0u; توڑو;
      /* Unicode-6.1 additions */
      حالت HB_SCRIPT_SHARADA:          ch = 0x11191u; توڑو;
      حالت HB_SCRIPT_TAKRI:            ch = 0x1168cu; توڑو;
      /* Unicode-7.0 additions */
      حالت HB_SCRIPT_MODI:             ch = 0x1160eu;توڑو;
      حالت HB_SCRIPT_SIDDHAM:          ch = 0x11590u; توڑو;
      حالت HB_SCRIPT_TIRHUTA:          ch = 0x1148fu; توڑو;
      /* Unicode-9.0 additions */
      حالت HB_SCRIPT_MARCHEN:          ch = 0x11c72u; توڑو;
      حالت HB_SCRIPT_NEWA:             ch = 0x1140eu; توڑو;
      /* Unicode-10.0 additions */
      حالت HB_SCRIPT_SOYOMBO:          ch = 0x11a5cu; توڑو;
      حالت HB_SCRIPT_ZANABAZAR_مربع: ch = 0x11a0bu; توڑو;
      /* Unicode-11.0 additions */
      حالت HB_SCRIPT_DOGRA:            ch = 0x1180au; توڑو;
      حالت HB_SCRIPT_GUNJALA_GONDI:    ch = 0x11d6cu; توڑو;
      /* Unicode-12.0 additions */
      حالت HB_SCRIPT_NANDINAGARI:      ch = 0x119b0u; توڑو;
      طےشدہ:                         ch = 0;        توڑو;
      }

      اگر (ch &&
	  hb_font_get_nominal_glyph (font, ch, &glyph) &&
	  hb_font_get_glyph_extents (font, glyph, &extents))
	*coord = extents.y_bearing;
      ورنہ
	*coord = hb_clamp_مقصد<hb_position_t> ((int64_t) font->y_scale * 6 / 10); // FIXME makes assumptions about origin
    }
    ورنہ
      *coord = hb_clamp_مقصد<hb_position_t> ((int64_t) font->x_scale * 6 / 10); // FIXME makes assumptions about origin
    توڑو;

  حالت HB_OT_LAYOUT_BASELINE_لیبل_IDEO_EMBOX_CENTRAL:
    {
      hb_position_t چوٹی_2, bottom;
      hb_ot_layout_get_baseline_with_fallback (font,
					       HB_OT_LAYOUT_BASELINE_لیبل_IDEO_EMBOX_چوٹی_OR_دایاں,
					       direction,
					       script_لیبل,
					       language_لیبل,
					       &چوٹی_2);
      hb_ot_layout_get_baseline_with_fallback (font,
					       HB_OT_LAYOUT_BASELINE_لیبل_IDEO_EMBOX_BOTTOM_OR_بایاں,
					       direction,
					       script_لیبل,
					       language_لیبل,
					       &bottom);
      *coord = hb_clamp_مقصد<hb_position_t> (((int64_t) چوٹی_2 + bottom) / 2);

    }
    توڑو;

  حالت HB_OT_LAYOUT_BASELINE_لیبل_IDEO_FACE_CENTRAL:
    {
      hb_position_t چوٹی_2, bottom;
      hb_ot_layout_get_baseline_with_fallback (font,
					       HB_OT_LAYOUT_BASELINE_لیبل_IDEO_FACE_چوٹی_OR_دایاں,
					       direction,
					       script_لیبل,
					       language_لیبل,
					       &چوٹی_2);
      hb_ot_layout_get_baseline_with_fallback (font,
					       HB_OT_LAYOUT_BASELINE_لیبل_IDEO_FACE_BOTTOM_OR_بایاں,
					       direction,
					       script_لیبل,
					       language_لیبل,
					       &bottom);
      *coord = hb_clamp_مقصد<hb_position_t> (((int64_t) چوٹی_2 + bottom) / 2);

    }
    توڑو;

  حالت HB_OT_LAYOUT_BASELINE_لیبل_MAX_قدر:
  طےشدہ:
    *coord = 0;
    توڑو;
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
خالی
hb_ot_layout_get_baseline_with_fallback2 (hb_font_t                   *font,
					  hb_ot_layout_baseline_لیبل_t  baseline_لیبل,
					  hb_direction_t               direction,
					  hb_script_t                  script,
					  hb_language_t                language,
					  hb_position_t               *coord        /*برآمد*/)
{
  hb_لیبل_t script_لیبل, language_لیبل;
  انتخاب_بنیاد_لیبل_فہرست (script, language, &script_لیبل, &language_لیبل);
  hb_ot_layout_get_baseline_with_fallback (font,
					   baseline_لیبل,
					   direction,
					   script_لیبل,
					   language_لیبل,
					   coord);
}

#اگر_ختم


#اگر_غیر_تعریف_شدہ HB_NO_LAYOUT_RARELY_مستعمل
ساخت hb_get_glyph_alternates_dispatch_t :
       hb_dispatch_context_t<hb_get_glyph_alternates_dispatch_t, غیرعلامتی>
{
  ساکن return_t default_return_قدر () { واپس 0; }
  منطقی stop_sublookup_iteration (return_t r) ثابت { واپس r; }

  نجی:
  سانچہ <قسم_نام T, قسم_نام ...Ts> خودکار
  _dispatch (ثابت T &obj, hb_ترجیح<1>, Ts&&... ds) HB_AUTO_RETURN
  ( obj.get_glyph_alternates (std::forward<Ts> (ds)...) )
  سانچہ <قسم_نام T, قسم_نام ...Ts> خودکار
  _dispatch (ثابت T &obj, hb_ترجیح<0>, Ts&&... ds) HB_AUTO_RETURN
  ( default_return_قدر () )
  عوامی:
  سانچہ <قسم_نام T, قسم_نام ...Ts> خودکار
  dispatch (ثابت T &obj, Ts&&... ds) HB_AUTO_RETURN
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
HB_EXTERN غیرعلامتی
hb_ot_layout_lookup_get_glyph_alternates (hb_face_t      *face,
					  غیرعلامتی        lookup_اشاریہ,
					  hb_codepoint_t  glyph,
					  غیرعلامتی        آغاز_انحراف,
					  غیرعلامتی       *alternate_گنتی  /* IN/OUT.  May be NULL. */,
					  hb_codepoint_t *alternate_glyphs /* OUT.     May be NULL. */)
{
  hb_get_glyph_alternates_dispatch_t c;
  ثابت OT::SubstLookup &lookup = face->جدول.GSUB->جدول->get_lookup (lookup_اشاریہ);
  خودکار ret = lookup.dispatch (&c, glyph, آغاز_انحراف, alternate_گنتی, alternate_glyphs);
  اگر (!ret && alternate_گنتی) *alternate_گنتی = 0;
  واپس ret;
}

ساخت hb_collect_glyph_alternates_dispatch_t :
       hb_dispatch_context_t<hb_collect_glyph_alternates_dispatch_t, منطقی>
{
  ساکن return_t default_return_قدر () { واپس جھوٹ; }
  منطقی stop_sublookup_iteration (return_t r) ثابت { واپس جھوٹ; }

  نجی:
  سانچہ <قسم_نام T, قسم_نام ...Ts> خودکار
  _dispatch (ثابت T &obj, hb_ترجیح<1>, Ts&&... ds) HB_AUTO_RETURN
  ( (obj.collect_glyph_alternates (std::forward<Ts> (ds)...), سچ) )
  سانچہ <قسم_نام T, قسم_نام ...Ts> خودکار
  _dispatch (ثابت T &obj, hb_ترجیح<0>, Ts&&... ds) HB_AUTO_RETURN
  ( default_return_قدر () )
  عوامی:
  سانچہ <قسم_نام T, قسم_نام ...Ts> خودکار
  dispatch (ثابت T &obj, Ts&&... ds) HB_AUTO_RETURN
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
					      غیرعلامتی   lookup_اشاریہ,
					      hb_map_t  *alternate_گنتی /* IN/OUT */,
					      hb_map_t  *alternate_glyphs /* IN/OUT */)
{
  hb_collect_glyph_alternates_dispatch_t c;
  ثابت OT::SubstLookup &lookup = face->جدول.GSUB->جدول->get_lookup (lookup_اشاریہ);
  واپس lookup.dispatch (&c, alternate_گنتی, alternate_glyphs);
}

ساخت hb_position_single_dispatch_t :
       hb_dispatch_context_t<hb_position_single_dispatch_t, منطقی>
{
  ساکن return_t default_return_قدر () { واپس جھوٹ; }
  منطقی stop_sublookup_iteration (return_t r) ثابت { واپس r; }

  نجی:
  سانچہ <قسم_نام T, قسم_نام ...Ts> خودکار
  _dispatch (ثابت T &obj, hb_ترجیح<1>, Ts&&... ds) HB_AUTO_RETURN
  ( obj.position_single (std::forward<Ts> (ds)...) )
  سانچہ <قسم_نام T, قسم_نام ...Ts> خودکار
  _dispatch (ثابت T &obj, hb_ترجیح<0>, Ts&&... ds) HB_AUTO_RETURN
  ( default_return_قدر () )
  عوامی:
  سانچہ <قسم_نام T, قسم_نام ...Ts> خودکار
  dispatch (ثابت T &obj, Ts&&... ds) HB_AUTO_RETURN
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
				       غیرعلامتی        lookup_اشاریہ,
				       hb_direction_t  direction,
				       hb_codepoint_t  glyph)
{
  ثابت OT::PosLookup &lookup = font->face->جدول.GPOS->جدول->get_lookup (lookup_اشاریہ);
  hb_blob_t *blob = font->face->جدول.GPOS->get_blob ();
  hb_glyph_position_t pos = {0};
  hb_position_single_dispatch_t c;
  lookup.dispatch (&c, font, blob, direction, glyph, pos);
  hb_position_t ret = 0;
  انتخاب (direction)
  {
    حالت HB_DIRECTION_LTR:
      ret = pos.x_انحراف_2;
      توڑو;
    حالت HB_DIRECTION_RTL:
      ret = hb_saturate_sub (pos.x_advance, pos.x_انحراف_2);
      توڑو;
    حالت HB_DIRECTION_TTB:
      ret = pos.y_انحراف_2;
      توڑو;
    حالت HB_DIRECTION_BTT:
      ret = hb_saturate_sub (pos.y_advance, pos.y_انحراف_2);
      توڑو;
    حالت HB_DIRECTION_INVALID:
    طےشدہ:
      توڑو;
  }
  واپس ret;
}
#اگر_ختم


#اگر_ختم
