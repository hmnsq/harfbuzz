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

#ضمن "hb.hh"

#جيڪڏهن_وصف_نه_ٿيل HB_NO_OT_LAYOUT

#جيڪڏهن_وصف_ٿيل HB_NO_OT_ٽيگ
#غلطي "Cannot compile hb-ot-layout.cc with HB_NO_OT_TAG."
#جيڪڏهن_پورو

#ضمن "hb-open-type.hh"
#ضمن "hb-ot-layout.hh"
#ضمن "hb-ot-face.hh"
#ضمن "hb-ot-map.hh"
#ضمن "hb-map.hh"

#ضمن "hb-ot-kern-table.hh"
#ضمن "hb-ot-layout-gdef-table.hh"
#ضمن "hb-ot-layout-gsub-table.hh"
#ضمن "hb-ot-layout-gpos-table.hh"
#ضمن "hb-ot-layout-base-table.hh"
#ضمن "hb-ot-layout-jstf-table.hh" // Just so we compile it; unused otherwise.
#ضمن "hb-ot-name-table.hh"
#ضمن "hb-ot-os2-table.hh"

#ضمن "hb-aat-layout-morx-table.hh"
#ضمن "hb-aat-layout-opbd-table.hh" // Just so we compile it; unused otherwise.

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

#جيڪڏهن_وصف_نه_ٿيل HB_NO_OT_KERN
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
منطقي
hb_ot_layout_has_kerning (hb_face_t *face)
{
  أرجع face->جدول.kern->جدول->has_معلومات ();
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
منطقي
hb_ot_layout_has_machine_kerning (hb_face_t *face)
{
  أرجع face->جدول.kern->جدول->has_state_machine ();
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
منطقي
hb_ot_layout_has_cross_kerning (hb_face_t *face)
{
  أرجع face->جدول.kern->جدول->has_cross_وهڪرو ();
}

فراغ
hb_ot_layout_kern (ثابت hb_ot_shape_plan_t *plan,
		   hb_font_t *font,
		   hb_بفر_t  *بفر)
{
  خودڪار &accel = *font->face->جدول.kern;
  hb_blob_t *blob = accel.get_blob ();

  AAT::hb_aat_apply_context_t c (plan, font, بفر, blob);

  إذا (!بفر->پيغام (font, "start table kern")) أرجع;
  c.بفر_glyph_سيٽ = accel.scratch.create_بفر_glyph_سيٽ ();
  accel.apply (&c);
  accel.scratch.destroy_بفر_glyph_سيٽ (c.بفر_glyph_سيٽ);
  (فراغ) بفر->پيغام (font, "end table kern");
}
#جيڪڏهن_پورو


/*
 * GDEF
 */

منطقي
OT::GDEF::is_blocklisted (hb_blob_t *blob,
			  hb_face_t *face) ثابت
{
#جيڪڏهن_وصف_ٿيل HB_NO_OT_LAYOUT_BLOCKLIST
  أرجع خطأ;
#جيڪڏهن_پورو
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
  بدل HB_CODEPOINT_ENCODE3(blob->ڊيگهائي,
			      face->جدول.GSUB->جدول.get_ڊيگهائي (),
			      face->جدول.GPOS->جدول.get_ڊيگهائي ())
  {
    /* sha1sum:c5ee92f0bca4bfb7d06c4d03e8cf9f9cf75d2e8a Windows 7? timesi.ttf */
    حالة HB_CODEPOINT_ENCODE3 (442, 2874, 42038):
    /* sha1sum:37fc8c16a0894ab7b749e35579856c73c840867b Windows 7? timesbi.ttf */
    حالة HB_CODEPOINT_ENCODE3 (430, 2874, 40662):
    /* sha1sum:19fc45110ea6cd3cdd0a5faca256a3797a069a80 Windows 7 timesi.ttf */
    حالة HB_CODEPOINT_ENCODE3 (442, 2874, 39116):
    /* sha1sum:6d2d3c9ed5b7de87bc84eae0df95ee5232ecde26 Windows 7 timesbi.ttf */
    حالة HB_CODEPOINT_ENCODE3 (430, 2874, 39374):
    /* sha1sum:8583225a8b49667c077b3525333f84af08c6bcd8 OS X 10.11.3 Times New Roman Italic.ttf */
    حالة HB_CODEPOINT_ENCODE3 (490, 3046, 41638):
    /* sha1sum:ec0f5a8751845355b7c3271d11f9918a966cb8c9 OS X 10.11.3 Times New Roman Bold Italic.ttf */
    حالة HB_CODEPOINT_ENCODE3 (478, 3046, 41902):
    /* sha1sum:96eda93f7d33e79962451c6c39a6b51ee893ce8c  tahoma.ttf from Windows 8 */
    حالة HB_CODEPOINT_ENCODE3 (898, 12554, 46470):
    /* sha1sum:20928dc06014e0cd120b6fc942d0c3b1a46ac2bc  tahomabd.ttf from Windows 8 */
    حالة HB_CODEPOINT_ENCODE3 (910, 12566, 47732):
    /* sha1sum:4f95b7e4878f60fa3a39ca269618dfde9721a79e  tahoma.ttf from Windows 8.1 */
    حالة HB_CODEPOINT_ENCODE3 (928, 23298, 59332):
    /* sha1sum:6d400781948517c3c0441ba42acb309584b73033  tahomabd.ttf from Windows 8.1 */
    حالة HB_CODEPOINT_ENCODE3 (940, 23310, 60732):
    /* tahoma.ttf v6.04 from Windows 8.1 x64, see https://bugzilla.mozilla.org/show_bug.cgi?id=1279925 */
    حالة HB_CODEPOINT_ENCODE3 (964, 23836, 60072):
    /* tahomabd.ttf v6.04 from Windows 8.1 x64, see https://bugzilla.mozilla.org/show_bug.cgi?id=1279925 */
    حالة HB_CODEPOINT_ENCODE3 (976, 23832, 61456):
    /* sha1sum:e55fa2dfe957a9f7ec26be516a0e30b0c925f846  tahoma.ttf from Windows 10 */
    حالة HB_CODEPOINT_ENCODE3 (994, 24474, 60336):
    /* sha1sum:7199385abb4c2cc81c83a151a7599b6368e92343  tahomabd.ttf from Windows 10 */
    حالة HB_CODEPOINT_ENCODE3 (1006, 24470, 61740):
    /* tahoma.ttf v6.91 from Windows 10 x64, see https://bugzilla.mozilla.org/show_bug.cgi?id=1279925 */
    حالة HB_CODEPOINT_ENCODE3 (1006, 24576, 61346):
    /* tahomabd.ttf v6.91 from Windows 10 x64, see https://bugzilla.mozilla.org/show_bug.cgi?id=1279925 */
    حالة HB_CODEPOINT_ENCODE3 (1018, 24572, 62828):
    /* sha1sum:b9c84d820c49850d3d27ec498be93955b82772b5  tahoma.ttf from Windows 10 AU */
    حالة HB_CODEPOINT_ENCODE3 (1006, 24576, 61352):
    /* sha1sum:2bdfaab28174bdadd2f3d4200a30a7ae31db79d2  tahomabd.ttf from Windows 10 AU */
    حالة HB_CODEPOINT_ENCODE3 (1018, 24572, 62834):
    /* sha1sum:b0d36cf5a2fbe746a3dd277bffc6756a820807a7  Tahoma.ttf from Mac OS X 10.9 */
    حالة HB_CODEPOINT_ENCODE3 (832, 7324, 47162):
    /* sha1sum:12fc4538e84d461771b30c18b5eb6bd434e30fba  Tahoma Bold.ttf from Mac OS X 10.9 */
    حالة HB_CODEPOINT_ENCODE3 (844, 7302, 45474):
    /* sha1sum:eb8afadd28e9cf963e886b23a30b44ab4fd83acc  himalaya.ttf from Windows 7 */
    حالة HB_CODEPOINT_ENCODE3 (180, 13054, 7254):
    /* sha1sum:73da7f025b238a3f737aa1fde22577a6370f77b0  himalaya.ttf from Windows 8 */
    حالة HB_CODEPOINT_ENCODE3 (192, 12638, 7254):
    /* sha1sum:6e80fd1c0b059bbee49272401583160dc1e6a427  himalaya.ttf from Windows 8.1 */
    حالة HB_CODEPOINT_ENCODE3 (192, 12690, 7254):
    /* 8d9267aea9cd2c852ecfb9f12a6e834bfaeafe44  cantarell-fonts-0.0.21/otf/Cantarell-Regular.otf */
    /* 983988ff7b47439ab79aeaf9a45bd4a2c5b9d371  cantarell-fonts-0.0.21/otf/Cantarell-Oblique.otf */
    حالة HB_CODEPOINT_ENCODE3 (188, 248, 3852):
    /* 2c0c90c6f6087ffbfea76589c93113a9cbb0e75f  cantarell-fonts-0.0.21/otf/Cantarell-Bold.otf */
    /* 55461f5b853c6da88069ffcdf7f4dd3f8d7e3e6b  cantarell-fonts-0.0.21/otf/Cantarell-Bold-Oblique.otf */
    حالة HB_CODEPOINT_ENCODE3 (188, 264, 3426):
    /* d125afa82a77a6475ac0e74e7c207914af84b37a padauk-2.80/Padauk.ttf RHEL 7.2 */
    حالة HB_CODEPOINT_ENCODE3 (1058, 47032, 11818):
    /* 0f7b80437227b90a577cc078c0216160ae61b031 padauk-2.80/Padauk-Bold.ttf RHEL 7.2*/
    حالة HB_CODEPOINT_ENCODE3 (1046, 47030, 12600):
    /* d3dde9aa0a6b7f8f6a89ef1002e9aaa11b882290 padauk-2.80/Padauk.ttf Ubuntu 16.04 */
    حالة HB_CODEPOINT_ENCODE3 (1058, 71796, 16770):
    /* 5f3c98ccccae8a953be2d122c1b3a77fd805093f padauk-2.80/Padauk-Bold.ttf Ubuntu 16.04 */
    حالة HB_CODEPOINT_ENCODE3 (1046, 71790, 17862):
    /* 6c93b63b64e8b2c93f5e824e78caca555dc887c7 padauk-2.80/Padauk-book.ttf */
    حالة HB_CODEPOINT_ENCODE3 (1046, 71788, 17112):
    /* d89b1664058359b8ec82e35d3531931125991fb9 padauk-2.80/Padauk-bookbold.ttf */
    حالة HB_CODEPOINT_ENCODE3 (1058, 71794, 17514):
    /* 824cfd193aaf6234b2b4dc0cf3c6ef576c0d00ef padauk-3.0/Padauk-book.ttf */
    حالة HB_CODEPOINT_ENCODE3 (1330, 109904, 57938):
    /* 91fcc10cf15e012d27571e075b3b4dfe31754a8a padauk-3.0/Padauk-bookbold.ttf */
    حالة HB_CODEPOINT_ENCODE3 (1330, 109904, 58972):
    /* sha1sum: c26e41d567ed821bed997e937bc0c41435689e85  Padauk.ttf
     *  "Padauk Regular" "Version 2.5", see https://crbug.com/681813 */
    حالة HB_CODEPOINT_ENCODE3 (1004, 59092, 14836):
    /* 88d2006ca084f04af2df1954ed714a8c71e8400f  Courier New.ttf from macOS 15 */
    حالة HB_CODEPOINT_ENCODE3 (588, 5078, 14418):
    /* 608e3ebb6dd1aee521cff08eb07d500a2c59df68  Courier New Bold.ttf from macOS 15 */
    حالة HB_CODEPOINT_ENCODE3 (588, 5078, 14238):
    /* d13221044ff054efd78f1cd8631b853c3ce85676  cour.ttf from Windows 10 */
    حالة HB_CODEPOINT_ENCODE3 (894, 17162, 33960):
    /* 68ed4a22d8067fcf1622ac6f6e2f4d3a2e3ec394  courbd.ttf from Windows 10 */
    حالة HB_CODEPOINT_ENCODE3 (894, 17154, 34472):
    /* 4cdb0259c96b7fd7c103821bb8f08f7cc6b211d7  cour.ttf from Windows 8.1 */
    حالة HB_CODEPOINT_ENCODE3 (816, 7868, 17052):
    /* 920483d8a8ed37f7f0afdabbe7f679aece7c75d8  courbd.ttf from Windows 8.1 */
    حالة HB_CODEPOINT_ENCODE3 (816, 7868, 17138):
      أرجع سچ;
  }
  أرجع خطأ;
}

سانچو <منطقي سيٽ_Digest>
ساكن فراغ
hb_ot_layout_سيٽ_glyph_props (hb_font_t *font,
			       hb_بفر_t *بفر)
{
  hb_بفر_assert_gsubgpos_vars (بفر);

  ثابت خودڪار &gdef = *font->face->جدول.GDEF;
  بنانشان صحيح ڳڻپ = بفر->len;
  hb_glyph_info_t *info = بفر->info;
  لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
  {
    إذا (سيٽ_Digest)
      بفر->digest.جوڙيو (info[i].codepoint);
    hb_glyph_info_سيٽ_glyph_props (&info[i], gdef.get_glyph_props (info[i].codepoint));
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
  أرجع face->جدول.GDEF->جدول->has_glyph_classes ();
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
  أرجع (hb_ot_layout_glyph_class_t) face->جدول.GDEF->جدول->get_glyph_class (glyph);
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
فراغ
hb_ot_layout_get_glyphs_اندريون_class (hb_face_t                  *face,
				  hb_ot_layout_glyph_class_t  klass,
				  hb_سيٽ_t                   *glyphs /*ٻاهريون*/)
{
  أرجع face->جدول.GDEF->جدول->get_glyphs_اندريون_class (klass, glyphs);
}

#جيڪڏهن_وصف_نه_ٿيل HB_NO_LAYOUT_اڻ_استعمال_ٿيل
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
بنانشان صحيح
hb_ot_layout_get_attach_points (hb_face_t      *face,
				hb_codepoint_t  glyph,
				بنانشان صحيح    شروعات_وٿي,
				بنانشان صحيح   *نقطو_ڳڻپ /* IN/OUT */,
				بنانشان صحيح   *نقطو_array /*ٻاهريون*/)
{
  أرجع face->جدول.GDEF->جدول->get_attach_points (glyph,
						     شروعات_وٿي,
						     نقطو_ڳڻپ,
						     نقطو_array);
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
بنانشان صحيح
hb_ot_layout_get_ligature_carets (hb_font_t      *font,
				  hb_direction_t  direction,
				  hb_codepoint_t  glyph,
				  بنانشان صحيح    شروعات_وٿي,
				  بنانشان صحيح   *caret_ڳڻپ /* IN/OUT */,
				  hb_position_t  *caret_array /*ٻاهريون*/)
{
  أرجع font->face->جدول.GDEF->جدول->get_lig_carets (font, direction, glyph, شروعات_وٿي, caret_ڳڻپ, caret_array);
}
#جيڪڏهن_پورو


/*
 * GSUB/GPOS
 */

منطقي
GSUB::is_blocklisted (hb_blob_t *blob HB_اڻ_استعمال_ٿيل,
			  hb_face_t *face) ثابت
{
#جيڪڏهن_وصف_ٿيل HB_NO_OT_LAYOUT_BLOCKLIST
  أرجع خطأ;
#جيڪڏهن_پورو
  أرجع خطأ;
}

منطقي
GPOS::is_blocklisted (hb_blob_t *blob HB_اڻ_استعمال_ٿيل,
			  hb_face_t *face HB_اڻ_استعمال_ٿيل) ثابت
{
#جيڪڏهن_وصف_ٿيل HB_NO_OT_LAYOUT_BLOCKLIST
  أرجع خطأ;
#جيڪڏهن_پورو
  أرجع خطأ;
}

ساكن ثابت OT::GSUBGPOS&
get_gsubgpos_جدول (hb_face_t *face,
		    hb_ٽيگ_t   جدول_ٽيگ_2)
{
  بدل (جدول_ٽيگ_2) {
    حالة HB_OT_ٽيگ_GSUB: أرجع *face->جدول.GSUB->جدول;
    حالة HB_OT_ٽيگ_GPOS: أرجع *face->جدول.GPOS->جدول;
    افتراضي:             أرجع Null (OT::GSUBGPOS);
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
بنانشان صحيح
hb_ot_layout_جدول_get_script_ٽيگ_فهرست (hb_face_t    *face,
				    hb_ٽيگ_t      جدول_ٽيگ_2,
				    بنانشان صحيح  شروعات_وٿي,
				    بنانشان صحيح *script_ڳڻپ /* IN/OUT */,
				    hb_ٽيگ_t     *script_ٽيگ_فهرست  /*ٻاهريون*/)
{
  ثابت OT::GSUBGPOS &g = get_gsubgpos_جدول (face, جدول_ٽيگ_2);

  أرجع g.get_script_ٽيگ_فهرست (شروعات_وٿي, script_ڳڻپ, script_ٽيگ_فهرست);
}

#وصف_ڪريو HB_OT_ٽيگ_LATIN_SCRIPT		HB_ٽيگ ('l', 'a', 't', 'n')

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
hb_ot_layout_جدول_ڳوليو_script (hb_face_t    *face,
				hb_ٽيگ_t      جدول_ٽيگ_2,
				hb_ٽيگ_t      script_ٽيگ,
				بنانشان صحيح *script_اشاريو /*ٻاهريون*/)
{
  static_assert ((OT::اشاريو::NOT_مليو_اشاريو == HB_OT_LAYOUT_NO_SCRIPT_اشاريو), "");
  ثابت OT::GSUBGPOS &g = get_gsubgpos_جدول (face, جدول_ٽيگ_2);

  إذا (g.ڳوليو_script_اشاريو (script_ٽيگ, script_اشاريو))
    أرجع سچ;

  /* try finding 'DFLT' */
  إذا (g.ڳوليو_script_اشاريو (HB_OT_ٽيگ_DEFAULT_SCRIPT, script_اشاريو))
    أرجع خطأ;

  /* try with 'dflt'; MS site has had typos and many fonts use it now :(.
   * including many versions of DejaVu Sans Mono! */
  إذا (g.ڳوليو_script_اشاريو (HB_OT_ٽيگ_DEFAULT_LANGUAGE, script_اشاريو))
    أرجع خطأ;

  /* try with 'latn'; some old fonts put their features there even though
     they're really trying to support Thai, for example :( */
  إذا (g.ڳوليو_script_اشاريو (HB_OT_ٽيگ_LATIN_SCRIPT, script_اشاريو))
    أرجع خطأ;

  إذا (script_اشاريو) *script_اشاريو = HB_OT_LAYOUT_NO_SCRIPT_اشاريو;
  أرجع خطأ;
}

#جيڪڏهن_وصف_نه_ٿيل HB_DISABLE_DEPRECATED
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
hb_ot_layout_جدول_چونڊيو_script (hb_face_t      *face,
				  hb_ٽيگ_t        جدول_ٽيگ_2,
				  ثابت hb_ٽيگ_t *script_ٽيگ_فهرست,
				  بنانشان صحيح   *script_اشاريو  /*ٻاهريون*/,
				  hb_ٽيگ_t       *chosen_script /*ٻاهريون*/)
{
  ثابت hb_ٽيگ_t *t;
  لكل (t = script_ٽيگ_فهرست; *t; t++);
  أرجع hb_ot_layout_جدول_select_script (face, جدول_ٽيگ_2, t - script_ٽيگ_فهرست, script_ٽيگ_فهرست, script_اشاريو, chosen_script);
}
#جيڪڏهن_پورو

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
				  hb_ٽيگ_t        جدول_ٽيگ_2,
				  بنانشان صحيح    script_ڳڻپ,
				  ثابت hb_ٽيگ_t *script_ٽيگ_فهرست,
				  بنانشان صحيح   *script_اشاريو  /*ٻاهريون*/,
				  hb_ٽيگ_t       *chosen_script /*ٻاهريون*/)
{
  static_assert ((OT::اشاريو::NOT_مليو_اشاريو == HB_OT_LAYOUT_NO_SCRIPT_اشاريو), "");
  ثابت OT::GSUBGPOS &g = get_gsubgpos_جدول (face, جدول_ٽيگ_2);
  بنانشان صحيح i;

  لكل (i = 0; i < script_ڳڻپ; i++)
  {
    إذا (g.ڳوليو_script_اشاريو (script_ٽيگ_فهرست[i], script_اشاريو))
    {
      إذا (chosen_script)
	*chosen_script = script_ٽيگ_فهرست[i];
      أرجع سچ;
    }
  }

  /* try finding 'DFLT' */
  إذا (g.ڳوليو_script_اشاريو (HB_OT_ٽيگ_DEFAULT_SCRIPT, script_اشاريو)) {
    إذا (chosen_script)
      *chosen_script = HB_OT_ٽيگ_DEFAULT_SCRIPT;
    أرجع خطأ;
  }

  /* try with 'dflt'; MS site has had typos and many fonts use it now :( */
  إذا (g.ڳوليو_script_اشاريو (HB_OT_ٽيگ_DEFAULT_LANGUAGE, script_اشاريو)) {
    إذا (chosen_script)
      *chosen_script = HB_OT_ٽيگ_DEFAULT_LANGUAGE;
    أرجع خطأ;
  }

  /* try with 'latn'; some old fonts put their features there even though
     they're really trying to support Thai, for example :( */
  إذا (g.ڳوليو_script_اشاريو (HB_OT_ٽيگ_LATIN_SCRIPT, script_اشاريو)) {
    إذا (chosen_script)
      *chosen_script = HB_OT_ٽيگ_LATIN_SCRIPT;
    أرجع خطأ;
  }

  إذا (script_اشاريو) *script_اشاريو = HB_OT_LAYOUT_NO_SCRIPT_اشاريو;
  إذا (chosen_script)
    *chosen_script = HB_ٽيگ_NONE;
  أرجع خطأ;
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
بنانشان صحيح
hb_ot_layout_جدول_get_feature_ٽيگ_فهرست (hb_face_t    *face,
				     hb_ٽيگ_t      جدول_ٽيگ_2,
				     بنانشان صحيح  شروعات_وٿي,
				     بنانشان صحيح *feature_ڳڻپ_2 /* IN/OUT */,
				     hb_ٽيگ_t     *feature_ٽيگ_فهرست  /*ٻاهريون*/)
{
  ثابت OT::GSUBGPOS &g = get_gsubgpos_جدول (face, جدول_ٽيگ_2);

  أرجع g.get_feature_ٽيگ_فهرست (شروعات_وٿي, feature_ڳڻپ_2, feature_ٽيگ_فهرست);
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
منطقي
hb_ot_layout_جدول_ڳوليو_feature (hb_face_t    *face,
				 hb_ٽيگ_t      جدول_ٽيگ_2,
				 hb_ٽيگ_t      feature_ٽيگ,
				 بنانشان صحيح *feature_اشاريو /*ٻاهريون*/)
{
  static_assert ((OT::اشاريو::NOT_مليو_اشاريو == HB_OT_LAYOUT_NO_FEATURE_اشاريو), "");
  ثابت OT::GSUBGPOS &g = get_gsubgpos_جدول (face, جدول_ٽيگ_2);

  بنانشان صحيح انگ_features = g.get_feature_ڳڻپ ();
  لكل (بنانشان صحيح i = 0; i < انگ_features; i++)
  {
    إذا (feature_ٽيگ == g.get_feature_ٽيگ (i)) {
      إذا (feature_اشاريو) *feature_اشاريو = i;
      أرجع سچ;
    }
  }

  إذا (feature_اشاريو) *feature_اشاريو = HB_OT_LAYOUT_NO_FEATURE_اشاريو;
  أرجع خطأ;
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
بنانشان صحيح
hb_ot_layout_script_get_language_ٽيگ_فهرست (hb_face_t    *face,
				       hb_ٽيگ_t      جدول_ٽيگ_2,
				       بنانشان صحيح  script_اشاريو,
				       بنانشان صحيح  شروعات_وٿي,
				       بنانشان صحيح *language_ڳڻپ /* IN/OUT */,
				       hb_ٽيگ_t     *language_ٽيگ_فهرست  /*ٻاهريون*/)
{
  ثابت OT::Script &s = get_gsubgpos_جدول (face, جدول_ٽيگ_2).get_script (script_اشاريو);

  أرجع s.get_lang_sys_ٽيگ_فهرست (شروعات_وٿي, language_ڳڻپ, language_ٽيگ_فهرست);
}


#جيڪڏهن_وصف_نه_ٿيل HB_DISABLE_DEPRECATED
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
hb_ot_layout_script_ڳوليو_language (hb_face_t    *face,
				   hb_ٽيگ_t      جدول_ٽيگ_2,
				   بنانشان صحيح  script_اشاريو,
				   hb_ٽيگ_t      language_ٽيگ,
				   بنانشان صحيح *language_اشاريو)
{
  أرجع hb_ot_layout_script_select_language (face,
					      جدول_ٽيگ_2,
					      script_اشاريو,
					      1,
					      &language_ٽيگ,
					      language_اشاريو);
}
#جيڪڏهن_پورو


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
				     hb_ٽيگ_t        جدول_ٽيگ_2,
				     بنانشان صحيح    script_اشاريو,
				     بنانشان صحيح    language_ڳڻپ,
				     ثابت hb_ٽيگ_t *language_ٽيگ_فهرست,
				     بنانشان صحيح   *language_اشاريو /*ٻاهريون*/,
				     hb_ٽيگ_t       *chosen_language /*ٻاهريون*/)
{
  static_assert ((OT::اشاريو::NOT_مليو_اشاريو == HB_OT_LAYOUT_DEFAULT_LANGUAGE_اشاريو), "");
  ثابت OT::Script &s = get_gsubgpos_جدول (face, جدول_ٽيگ_2).get_script (script_اشاريو);
  بنانشان صحيح i;

  لكل (i = 0; i < language_ڳڻپ; i++)
  {
    إذا (s.ڳوليو_lang_sys_اشاريو (language_ٽيگ_فهرست[i], language_اشاريو))
    {
      إذا (chosen_language)
        *chosen_language = language_ٽيگ_فهرست[i];
      أرجع سچ;
    }
  }

  /* try finding 'dflt' */
  إذا (s.ڳوليو_lang_sys_اشاريو (HB_OT_ٽيگ_DEFAULT_LANGUAGE, language_اشاريو))
  {
    إذا (chosen_language)
      *chosen_language = HB_OT_ٽيگ_DEFAULT_LANGUAGE;
    أرجع خطأ;
  }

  إذا (language_اشاريو)
    *language_اشاريو = HB_OT_LAYOUT_DEFAULT_LANGUAGE_اشاريو;
  إذا (chosen_language)
    *chosen_language = HB_ٽيگ_NONE;
  أرجع خطأ;
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
				     hb_ٽيگ_t        جدول_ٽيگ_2,
				     بنانشان صحيح    script_اشاريو,
				     بنانشان صحيح    language_ڳڻپ,
				     ثابت hb_ٽيگ_t *language_ٽيگ_فهرست,
				     بنانشان صحيح   *language_اشاريو /*ٻاهريون*/)
{
  أرجع hb_ot_layout_script_select_language2 (face, جدول_ٽيگ_2,
					       script_اشاريو,
					       language_ڳڻپ, language_ٽيگ_فهرست,
					       language_اشاريو, خالي_اشارو);
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
hb_ot_layout_language_get_required_feature_اشاريو (hb_face_t    *face,
						  hb_ٽيگ_t      جدول_ٽيگ_2,
						  بنانشان صحيح  script_اشاريو,
						  بنانشان صحيح  language_اشاريو,
						  بنانشان صحيح *feature_اشاريو /*ٻاهريون*/)
{
  أرجع hb_ot_layout_language_get_required_feature (face,
						     جدول_ٽيگ_2,
						     script_اشاريو,
						     language_اشاريو,
						     feature_اشاريو,
						     خالي_اشارو);
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
					    hb_ٽيگ_t      جدول_ٽيگ_2,
					    بنانشان صحيح  script_اشاريو,
					    بنانشان صحيح  language_اشاريو,
					    بنانشان صحيح *feature_اشاريو /*ٻاهريون*/,
					    hb_ٽيگ_t     *feature_ٽيگ   /*ٻاهريون*/)
{
  ثابت OT::GSUBGPOS &g = get_gsubgpos_جدول (face, جدول_ٽيگ_2);
  ثابت OT::LangSys &l = g.get_script (script_اشاريو).get_lang_sys (language_اشاريو);

  بنانشان صحيح اشاريو_2 = l.get_required_feature_اشاريو ();
  إذا (feature_اشاريو) *feature_اشاريو = اشاريو_2;
  إذا (feature_ٽيگ) *feature_ٽيگ = g.get_feature_ٽيگ (اشاريو_2);

  أرجع l.has_required_feature ();
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
بنانشان صحيح
hb_ot_layout_language_get_feature_indexes (hb_face_t    *face,
					   hb_ٽيگ_t      جدول_ٽيگ_2,
					   بنانشان صحيح  script_اشاريو,
					   بنانشان صحيح  language_اشاريو,
					   بنانشان صحيح  شروعات_وٿي,
					   بنانشان صحيح *feature_ڳڻپ_2   /* IN/OUT */,
					   بنانشان صحيح *feature_indexes /*ٻاهريون*/)
{
  ثابت OT::GSUBGPOS &g = get_gsubgpos_جدول (face, جدول_ٽيگ_2);
  ثابت OT::LangSys &l = g.get_script (script_اشاريو).get_lang_sys (language_اشاريو);

  أرجع l.get_feature_indexes (شروعات_وٿي, feature_ڳڻپ_2, feature_indexes);
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
بنانشان صحيح
hb_ot_layout_language_get_feature_ٽيگ_فهرست (hb_face_t    *face,
					hb_ٽيگ_t      جدول_ٽيگ_2,
					بنانشان صحيح  script_اشاريو,
					بنانشان صحيح  language_اشاريو,
					بنانشان صحيح  شروعات_وٿي,
					بنانشان صحيح *feature_ڳڻپ_2 /* IN/OUT */,
					hb_ٽيگ_t     *feature_ٽيگ_فهرست  /*ٻاهريون*/)
{
  ثابت OT::GSUBGPOS &g = get_gsubgpos_جدول (face, جدول_ٽيگ_2);
  ثابت OT::LangSys &l = g.get_script (script_اشاريو).get_lang_sys (language_اشاريو);

  static_assert ((حجم (بنانشان صحيح) == حجم (hb_ٽيگ_t)), "");
  بنانشان صحيح ret = l.get_feature_indexes (شروعات_وٿي, feature_ڳڻپ_2, (بنانشان صحيح *) feature_ٽيگ_فهرست);

  إذا (feature_ڳڻپ_2 && feature_ٽيگ_فهرست) {
    بنانشان صحيح ڳڻپ = *feature_ڳڻپ_2;
    لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
      feature_ٽيگ_فهرست[i] = g.get_feature_ٽيگ ((بنانشان صحيح) feature_ٽيگ_فهرست[i]);
  }

  أرجع ret;
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
hb_ot_layout_language_ڳوليو_feature (hb_face_t    *face,
				    hb_ٽيگ_t      جدول_ٽيگ_2,
				    بنانشان صحيح  script_اشاريو,
				    بنانشان صحيح  language_اشاريو,
				    hb_ٽيگ_t      feature_ٽيگ,
				    بنانشان صحيح *feature_اشاريو /*ٻاهريون*/)
{
  static_assert ((OT::اشاريو::NOT_مليو_اشاريو == HB_OT_LAYOUT_NO_FEATURE_اشاريو), "");
  ثابت OT::GSUBGPOS &g = get_gsubgpos_جدول (face, جدول_ٽيگ_2);
  ثابت OT::LangSys &l = g.get_script (script_اشاريو).get_lang_sys (language_اشاريو);

  بنانشان صحيح انگ_features = l.get_feature_ڳڻپ ();
  لكل (بنانشان صحيح i = 0; i < انگ_features; i++) {
    بنانشان صحيح f_اشاريو = l.get_feature_اشاريو (i);

    إذا (feature_ٽيگ == g.get_feature_ٽيگ (f_اشاريو)) {
      إذا (feature_اشاريو) *feature_اشاريو = f_اشاريو;
      أرجع سچ;
    }
  }

  إذا (feature_اشاريو) *feature_اشاريو = HB_OT_LAYOUT_NO_FEATURE_اشاريو;
  أرجع خطأ;
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
بنانشان صحيح
hb_ot_layout_feature_get_lookups (hb_face_t    *face,
				  hb_ٽيگ_t      جدول_ٽيگ_2,
				  بنانشان صحيح  feature_اشاريو,
				  بنانشان صحيح  شروعات_وٿي,
				  بنانشان صحيح *lookup_ڳڻپ   /* IN/OUT */,
				  بنانشان صحيح *lookup_indexes /*ٻاهريون*/)
{
  أرجع hb_ot_layout_feature_with_variations_get_lookups (face,
							   جدول_ٽيگ_2,
							   feature_اشاريو,
							   HB_OT_LAYOUT_NO_VARIATIONS_اشاريو,
							   شروعات_وٿي,
							   lookup_ڳڻپ,
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
بنانشان صحيح
hb_ot_layout_جدول_get_lookup_ڳڻپ (hb_face_t    *face,
				     hb_ٽيگ_t      جدول_ٽيگ_2)
{
  أرجع get_gsubgpos_جدول (face, جدول_ٽيگ_2).get_lookup_ڳڻپ ();
}


بنية hb_collect_features_context_t
{
  hb_collect_features_context_t (hb_face_t *face,
				 hb_ٽيگ_t   جدول_ٽيگ_2,
				 hb_سيٽ_t  *feature_indices_,
				 ثابت hb_ٽيگ_t *features)

    : g (get_gsubgpos_جدول (face, جدول_ٽيگ_2)),
      feature_indices (feature_indices_),
      has_feature_filter (خطأ),
      script_ڳڻپ (0),langsys_ڳڻپ (0), feature_اشاريو_ڳڻپ (0)
  {
    compute_feature_filter (features);
  }

  فراغ compute_feature_filter (ثابت hb_ٽيگ_t *features)
  {
    إذا (features == خالي_اشارو)
    {
      has_feature_filter = خطأ;
      أرجع;
    }

    has_feature_filter = سچ;
    hb_سيٽ_t features_سيٽ;
    لكل (; *features; features++)
      features_سيٽ.جوڙيو (*features);

    لكل (بنانشان i = 0; i < g.get_feature_ڳڻپ (); i++)
    {
      hb_ٽيگ_t ٽيگ_2 = g.get_feature_ٽيگ (i);
      إذا (features_سيٽ.has (ٽيگ_2))
	feature_indices_filter.جوڙيو(i);
    }
  }

  منطقي گهميل (ثابت OT::Script &s)
  {
    /* We might have Null() object here.  Don't want to involve
     * that in the memoize.  So, detect empty objects and return. */
    إذا (unlikely (!s.has_default_lang_sys () &&
		  !s.get_lang_sys_ڳڻپ ()))
      أرجع سچ;

    إذا (script_ڳڻپ++ > HB_MAX_SCRIPTS)
      أرجع سچ;

    أرجع گهميل (s, گهميل_script);
  }
  منطقي گهميل (ثابت OT::LangSys &l)
  {
    /* We might have Null() object here.  Don't want to involve
     * that in the memoize.  So, detect empty objects and return. */
    إذا (unlikely (!l.has_required_feature () &&
		  !l.get_feature_ڳڻپ ()))
      أرجع سچ;

    إذا (langsys_ڳڻپ++ > HB_MAX_LANGSYS)
      أرجع سچ;

    أرجع گهميل (l, گهميل_langsys);
  }

  منطقي گهميل_feature_indices (بنانشان ڳڻپ)
  {
    feature_اشاريو_ڳڻپ += ڳڻپ;
    أرجع feature_اشاريو_ڳڻپ > HB_MAX_FEATURE_INDICES;
  }

  خاص:
  سانچو <قسم_نالو T>
  منطقي گهميل (ثابت T &p, hb_سيٽ_t &گهميل_سيٽ)
  {
    hb_codepoint_t delta = (hb_codepoint_t) ((uintptr_t) &p - (uintptr_t) &g);
     إذا (گهميل_سيٽ.has (delta))
      أرجع سچ;

    گهميل_سيٽ.جوڙيو (delta);
    أرجع خطأ;
  }

  عام:
  ثابت OT::GSUBGPOS &g;
  hb_سيٽ_t *feature_indices;
  hb_سيٽ_t  feature_indices_filter;
  منطقي has_feature_filter;

  خاص:
  hb_سيٽ_t گهميل_script;
  hb_سيٽ_t گهميل_langsys;
  بنانشان صحيح script_ڳڻپ;
  بنانشان صحيح langsys_ڳڻپ;
  بنانشان صحيح feature_اشاريو_ڳڻپ;
};

ساكن فراغ
langsys_collect_features (hb_collect_features_context_t *c,
			  ثابت OT::LangSys  &l)
{
  إذا (c->گهميل (l)) أرجع;

  إذا (!c->has_feature_filter)
  {
    /* All features. */
    إذا (l.has_required_feature () && !c->گهميل_feature_indices (1))
      c->feature_indices->جوڙيو (l.get_required_feature_اشاريو ());

    // TODO(garretrieger): filter out indices >= feature count?
    إذا (!c->گهميل_feature_indices (l.feature_اشاريو_2.len))
      l.جوڙيو_feature_indexes_منزل (c->feature_indices);
  }
  وإلا
  {
    إذا (c->feature_indices_filter.is_empty()) أرجع;
    بنانشان صحيح انگ_features = l.get_feature_ڳڻپ ();
    لكل (بنانشان صحيح i = 0; i < انگ_features; i++)
    {
      بنانشان صحيح feature_اشاريو = l.get_feature_اشاريو (i);
      إذا (!c->feature_indices_filter.has (feature_اشاريو)) استمر;

      c->feature_indices->جوڙيو (feature_اشاريو);
      c->feature_indices_filter.del (feature_اشاريو);
    }
  }
}

ساكن فراغ
script_collect_features (hb_collect_features_context_t *c,
			 ثابت OT::Script   &s,
			 ثابت hb_ٽيگ_t *languages)
{
  إذا (c->گهميل (s)) أرجع;

  إذا (!languages)
  {
    /* All languages. */
    إذا (s.has_default_lang_sys ())
      langsys_collect_features (c,
				s.get_default_lang_sys ());


    بنانشان صحيح ڳڻپ = s.get_lang_sys_ڳڻپ ();
    لكل (بنانشان صحيح language_اشاريو = 0; language_اشاريو < ڳڻپ; language_اشاريو++)
      langsys_collect_features (c,
				s.get_lang_sys (language_اشاريو));
  }
  وإلا
  {
    لكل (; *languages; languages++)
    {
      بنانشان صحيح language_اشاريو;
      إذا (s.ڳوليو_lang_sys_اشاريو (*languages, &language_اشاريو))
	langsys_collect_features (c,
				  s.get_lang_sys (language_اشاريو));

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
فراغ
hb_ot_layout_collect_features (hb_face_t      *face,
			       hb_ٽيگ_t        جدول_ٽيگ_2,
			       ثابت hb_ٽيگ_t *scripts,
			       ثابت hb_ٽيگ_t *languages,
			       ثابت hb_ٽيگ_t *features,
			       hb_سيٽ_t       *feature_indexes /*ٻاهريون*/)
{
  hb_collect_features_context_t c (face, جدول_ٽيگ_2, feature_indexes, features);
  إذا (!scripts)
  {
    /* All scripts. */
    بنانشان صحيح ڳڻپ = c.g.get_script_ڳڻپ ();
    لكل (بنانشان صحيح script_اشاريو = 0; script_اشاريو < ڳڻپ; script_اشاريو++)
      script_collect_features (&c,
			       c.g.get_script (script_اشاريو),
			       languages);
  }
  وإلا
  {
    لكل (; *scripts; scripts++)
    {
      بنانشان صحيح script_اشاريو;
      إذا (c.g.ڳوليو_script_اشاريو (*scripts, &script_اشاريو))
	script_collect_features (&c,
				 c.g.get_script (script_اشاريو),
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
فراغ
hb_ot_layout_collect_features_map (hb_face_t      *face,
				   hb_ٽيگ_t        جدول_ٽيگ_2,
				   بنانشان        script_اشاريو,
				   بنانشان        language_اشاريو,
				   hb_map_t       *feature_map /*ٻاهريون*/)
{
  ثابت OT::GSUBGPOS &g = get_gsubgpos_جدول (face, جدول_ٽيگ_2);
  ثابت OT::LangSys &l = g.get_script (script_اشاريو).get_lang_sys (language_اشاريو);

  بنانشان صحيح ڳڻپ = l.get_feature_indexes (0, خالي_اشارو, خالي_اشارو);
  feature_map->alloc (ڳڻپ);

  /* Loop in reverse, such that earlier entries win. That emulates
   * a linear search, which seems to be what other implementations do.
   * We found that with arialuni_t.ttf, the "ur" language system has
   * duplicate features, and the earlier ones work but not later ones.
   */
  لكل (بنانشان صحيح i = ڳڻپ; i; i--)
  {
    بنانشان feature_اشاريو = 0;
    بنانشان feature_ڳڻپ_2 = 1;
    l.get_feature_indexes (i - 1, &feature_ڳڻپ_2, &feature_اشاريو);
    إذا (!feature_ڳڻپ_2)
      اكسر;
    hb_ٽيگ_t feature_ٽيگ = g.get_feature_ٽيگ (feature_اشاريو);
    feature_map->سيٽ (feature_ٽيگ, feature_اشاريو);
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
فراغ
hb_ot_layout_collect_lookups (hb_face_t      *face,
			      hb_ٽيگ_t        جدول_ٽيگ_2,
			      ثابت hb_ٽيگ_t *scripts,
			      ثابت hb_ٽيگ_t *languages,
			      ثابت hb_ٽيگ_t *features,
			      hb_سيٽ_t       *lookup_indexes /*ٻاهريون*/)
{
  ثابت OT::GSUBGPOS &g = get_gsubgpos_جدول (face, جدول_ٽيگ_2);

  hb_سيٽ_t feature_indexes;
  hb_ot_layout_collect_features (face, جدول_ٽيگ_2, scripts, languages, features, &feature_indexes);

  لكل (خودڪار feature_اشاريو : feature_indexes)
    g.get_feature (feature_اشاريو).جوڙيو_lookup_indexes_منزل (lookup_indexes);

  g.feature_variation_collect_lookups (&feature_indexes, خالي_اشارو, lookup_indexes);
}


#جيڪڏهن_وصف_نه_ٿيل HB_NO_LAYOUT_COLLECT_GLYPHS
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
فراغ
hb_ot_layout_lookup_collect_glyphs (hb_face_t    *face,
				    hb_ٽيگ_t      جدول_ٽيگ_2,
				    بنانشان صحيح  lookup_اشاريو,
				    hb_سيٽ_t     *glyphs_before, /* OUT.  May be NULL */
				    hb_سيٽ_t     *glyphs_داخلا,  /* OUT.  May be NULL */
				    hb_سيٽ_t     *glyphs_after,  /* OUT.  May be NULL */
				    hb_سيٽ_t     *glyphs_output  /* OUT.  May be NULL */)
{
  OT::hb_collect_glyphs_context_t c (face,
				     glyphs_before,
				     glyphs_داخلا,
				     glyphs_after,
				     glyphs_output);

  بدل (جدول_ٽيگ_2)
  {
    حالة HB_OT_ٽيگ_GSUB:
    {
      ثابت OT::SubstLookup& l = face->جدول.GSUB->جدول->get_lookup (lookup_اشاريو);
      l.collect_glyphs (&c);
      أرجع;
    }
    حالة HB_OT_ٽيگ_GPOS:
    {
      ثابت OT::PosLookup& l = face->جدول.GPOS->جدول->get_lookup (lookup_اشاريو);
      l.collect_glyphs (&c);
      أرجع;
    }
  }
}
#جيڪڏهن_پورو


/* Variations support */

منطقي
hb_ot_layout_جدول_get_feature_variations_state (hb_face_t             *face,
						   hb_ٽيگ_t               جدول_ٽيگ_2,
						   ثابت صحيح             *coords,
						   بنانشان صحيح           انگ_coords,
						   بنانشان صحيح          *variations_اشاريو,
						   hb_ويڪٽر_t<بنانشان> *lookup_conditions)
{
  ثابت OT::GSUBGPOS &g = get_gsubgpos_جدول (face, جدول_ٽيگ_2);
  ثابت OT::GDEF &gdef = *face->جدول.GDEF->جدول;

  خودڪار instancer = OT::شي_Var_Store_Instancer (&gdef.get_var_store (), خالي_اشارو,
					      hb_array (coords, انگ_coords));

  أرجع g.get_feature_variations_state (coords,
					 انگ_coords,
					 variations_اشاريو,
					 &instancer,
					 lookup_conditions);
}

منطقي
_hb_ot_layout_feature_get_lookup_variations (hb_face_t *face,
                                             hb_ٽيگ_t جدول_ٽيگ_2,
                                             بنانشان feature_اشاريو,
                                             بنانشان variations_اشاريو,
                                             hb_sorted_array_t<ثابت بنانشان> conditions,
                                             hb_سيٽ_t *lookup_indexes)
{
  أرجع get_gsubgpos_جدول (face, جدول_ٽيگ_2).get_lookup_variations (
      feature_اشاريو, variations_اشاريو, conditions, lookup_indexes);
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
hb_ot_layout_جدول_ڳوليو_feature_variations (hb_face_t    *face,
					    hb_ٽيگ_t      جدول_ٽيگ_2,
					    ثابت صحيح    *coords,
					    بنانشان صحيح  انگ_coords,
					    بنانشان صحيح *variations_اشاريو /* out */)
{
  ثابت OT::GSUBGPOS &g = get_gsubgpos_جدول (face, جدول_ٽيگ_2);
  ثابت OT::GDEF &gdef = *face->جدول.GDEF->جدول;

  خودڪار instancer = OT::شي_Var_Store_Instancer(&gdef.get_var_store(), خالي_اشارو,
					     hb_array (coords, انگ_coords));

  أرجع g.ڳوليو_variations_اشاريو (coords, انگ_coords, variations_اشاريو, &instancer);
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
بنانشان صحيح
hb_ot_layout_feature_with_variations_get_lookups (hb_face_t    *face,
						  hb_ٽيگ_t      جدول_ٽيگ_2,
						  بنانشان صحيح  feature_اشاريو,
						  بنانشان صحيح  variations_اشاريو,
						  بنانشان صحيح  شروعات_وٿي,
						  بنانشان صحيح *lookup_ڳڻپ /* IN/OUT */,
						  بنانشان صحيح *lookup_indexes /*ٻاهريون*/)
{
  static_assert ((OT::FeatureVariations::NOT_مليو_اشاريو == HB_OT_LAYOUT_NO_VARIATIONS_اشاريو), "");
  ثابت OT::GSUBGPOS &g = get_gsubgpos_جدول (face, جدول_ٽيگ_2);

  ثابت OT::Feature &f = g.get_feature_variation (feature_اشاريو, variations_اشاريو);

  أرجع f.get_lookup_indexes (شروعات_وٿي, lookup_ڳڻپ, lookup_indexes);
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
  أرجع face->جدول.GSUB->جدول->has_معلومات ();
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
				      بنانشان صحيح          lookup_اشاريو,
				      ثابت hb_codepoint_t *glyphs,
				      بنانشان صحيح          glyphs_ڊيگهائي,
				      hb_bool_t             zero_context)
{
  خودڪار &gsub = face->جدول.GSUB;
  إذا (unlikely (lookup_اشاريو >= gsub->lookup_ڳڻپ)) أرجع خطأ;
  OT::hb_would_apply_context_t c (face, glyphs, glyphs_ڊيگهائي, (منطقي) zero_context);

  ثابت OT::SubstLookup& l = gsub->جدول->get_lookup (lookup_اشاريو);
  خودڪار *accel = gsub->get_accel (lookup_اشاريو);
  أرجع accel && l.would_apply (&c, accel);
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
فراغ
hb_ot_layout_substitute_شروعات (hb_font_t    *font,
			       hb_بفر_t  *بفر)
{
  hb_ot_layout_سيٽ_glyph_props<خطأ> (font, بفر);
}

فراغ
hb_ot_layout_substitute_شروعات_with_digest (hb_font_t    *font,
					   hb_بفر_t  *بفر)
{
  بفر->digest = hb_سيٽ_digest_t ();
  hb_ot_layout_سيٽ_glyph_props<سچ> (font, بفر);
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
فراغ
hb_ot_layout_lookup_substitute_گهيرو (hb_face_t    *face,
					بنانشان صحيح  lookup_اشاريو,
					hb_سيٽ_t     *glyphs /*ٻاهريون*/)
{
  hb_map_t پورو_lookups_glyph_ڳڻپ;
  hb_hashmap_t<بنانشان, hb::unique_ptr<hb_سيٽ_t>> پورو_lookups_glyph_سيٽ;
  OT::hb_گهيرو_context_t c (face, glyphs, &پورو_lookups_glyph_ڳڻپ, &پورو_lookups_glyph_سيٽ);

  ثابت OT::SubstLookup& l = face->جدول.GSUB->جدول->get_lookup (lookup_اشاريو);

  l.گهيرو (&c, lookup_اشاريو);
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
فراغ
hb_ot_layout_lookups_substitute_گهيرو (hb_face_t      *face,
					 ثابت hb_سيٽ_t *lookups,
					 hb_سيٽ_t       *glyphs /*ٻاهريون*/)
{
  hb_map_t پورو_lookups_glyph_ڳڻپ;
  hb_hashmap_t<بنانشان, hb::unique_ptr<hb_سيٽ_t>> پورو_lookups_glyph_سيٽ;
  OT::hb_گهيرو_context_t c (face, glyphs, &پورو_lookups_glyph_ڳڻپ, &پورو_lookups_glyph_سيٽ);
  ثابت GSUB& gsub = *face->جدول.GSUB->جدول;

  بنانشان صحيح iteration_ڳڻپ = 0;
  بنانشان صحيح glyphs_ڊيگهائي;
  افعل
  {
    c.reset_lookup_گهمو_ڳڻپ ();
    glyphs_ڊيگهائي = glyphs->get_population ();
    إذا (lookups)
    {
      لكل (خودڪار lookup_اشاريو : *lookups)
	gsub.get_lookup (lookup_اشاريو).گهيرو (&c, lookup_اشاريو);
    }
    وإلا
    {
      لكل (بنانشان صحيح i = 0; i < gsub.get_lookup_ڳڻپ (); i++)
	gsub.get_lookup (i).گهيرو (&c, i);
    }
  } طالما (iteration_ڳڻپ++ <= HB_گهيرو_MAX_STAGES &&
	   glyphs_ڊيگهائي != glyphs->get_population ());
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
  أرجع face->جدول.GPOS->جدول->has_معلومات ();
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
فراغ
hb_ot_layout_position_شروعات (hb_font_t *font, hb_بفر_t *بفر)
{
  GPOS::position_شروعات (font, بفر);
}


/**
 * hb_ot_layout_position_finish_advances:
 * @font: #hb_font_t to use
 * @buffer: #hb_buffer_t buffer to work upon
 *
 * Called after positioning lookups are performed, to finish glyph advances.
 *
 **/
فراغ
hb_ot_layout_position_پڄاڻي_advances (hb_font_t *font, hb_بفر_t *بفر)
{
  GPOS::position_پڄاڻي_advances (font, بفر);
}

/**
 * hb_ot_layout_position_finish_offsets:
 * @font: #hb_font_t to use
 * @buffer: #hb_buffer_t buffer to work upon
 *
 * Called after positioning lookups are performed, to finish glyph offsets.
 *
 **/
فراغ
hb_ot_layout_position_پڄاڻي_offsets (hb_font_t *font, hb_بفر_t *بفر)
{
  GPOS::position_پڄاڻي_offsets (font, بفر);
}


#جيڪڏهن_وصف_نه_ٿيل HB_NO_LAYOUT_FEATURE_PARAMS
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
hb_ot_layout_get_سائيز_params (hb_face_t       *face,
			      بنانشان صحيح    *design_سائيز_2,       /* OUT.  May be NULL */
			      بنانشان صحيح    *subfamily_id,      /* OUT.  May be NULL */
			      hb_ot_name_id_t *subfamily_name_id, /* OUT.  May be NULL */
			      بنانشان صحيح    *range_شروعات_2,       /* OUT.  May be NULL */
			      بنانشان صحيح    *range_پڇاڙي_2          /* OUT.  May be NULL */)
{
  ثابت GPOS &gpos = *face->جدول.GPOS->جدول;
  ثابت hb_ٽيگ_t ٽيگ_2 = HB_ٽيگ ('s','i','z','e');

  بنانشان صحيح انگ_features = gpos.get_feature_ڳڻپ ();
  لكل (بنانشان صحيح i = 0; i < انگ_features; i++)
  {
    إذا (ٽيگ_2 == gpos.get_feature_ٽيگ (i))
    {
      ثابت OT::Feature &f = gpos.get_feature (i);
      ثابت OT::Feature_Params_سائيز &params = f.get_feature_params ().get_سائيز_params (ٽيگ_2);

      إذا (params.design_سائيز)
      {
	إذا (design_سائيز_2) *design_سائيز_2 = params.design_سائيز;
	إذا (subfamily_id) *subfamily_id = params.subfamilyID;
	إذا (subfamily_name_id) *subfamily_name_id = params.subfamilyNameID;
	إذا (range_شروعات_2) *range_شروعات_2 = params.range_شروعات;
	إذا (range_پڇاڙي_2) *range_پڇاڙي_2 = params.range_پڇاڙي;

	أرجع سچ;
      }
    }
  }

  إذا (design_سائيز_2) *design_سائيز_2 = 0;
  إذا (subfamily_id) *subfamily_id = 0;
  إذا (subfamily_name_id) *subfamily_name_id = HB_OT_NAME_ID_INVALID;
  إذا (range_شروعات_2) *range_شروعات_2 = 0;
  إذا (range_پڇاڙي_2) *range_پڇاڙي_2 = 0;

  أرجع خطأ;
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
				   hb_ٽيگ_t         جدول_ٽيگ_2,
				   بنانشان صحيح     feature_اشاريو,
				   hb_ot_name_id_t *label_id,             /* OUT.  May be NULL */
				   hb_ot_name_id_t *tooltip_id,           /* OUT.  May be NULL */
				   hb_ot_name_id_t *sample_id,            /* OUT.  May be NULL */
				   بنانشان صحيح    *انگ_named_parameters, /* OUT.  May be NULL */
				   hb_ot_name_id_t *پهريون_param_id        /* OUT.  May be NULL */)
{
  ثابت OT::GSUBGPOS &g = get_gsubgpos_جدول (face, جدول_ٽيگ_2);

  hb_ٽيگ_t feature_ٽيگ = g.get_feature_ٽيگ (feature_اشاريو);
  ثابت OT::Feature &f = g.get_feature (feature_اشاريو);

  ثابت OT::FeatureParams &feature_params = f.get_feature_params ();
  إذا (&feature_params != &Null (OT::FeatureParams))
  {
    ثابت OT::Feature_Params_Stylistic_سيٽ& ss_params =
      feature_params.get_stylistic_سيٽ_params (feature_ٽيگ);
    إذا (&ss_params != &Null (OT::Feature_Params_Stylistic_سيٽ)) /* ssXX */
    {
      إذا (label_id) *label_id = ss_params.uiNameID;
      // ssXX features don't have the rest
      إذا (tooltip_id) *tooltip_id = HB_OT_NAME_ID_INVALID;
      إذا (sample_id) *sample_id = HB_OT_NAME_ID_INVALID;
      إذا (انگ_named_parameters) *انگ_named_parameters = 0;
      إذا (پهريون_param_id) *پهريون_param_id = HB_OT_NAME_ID_INVALID;
      أرجع سچ;
    }
    ثابت OT::FeatureParamsCharacterVariants& cv_params =
      feature_params.get_character_variants_params (feature_ٽيگ);
    إذا (&cv_params != &Null (OT::FeatureParamsCharacterVariants)) /* cvXX */
    {
      إذا (label_id) *label_id = cv_params.featUILableNameID;
      إذا (tooltip_id) *tooltip_id = cv_params.feat_UITooltip_متن_Name_ID;
      إذا (sample_id) *sample_id = cv_params.sample_متن_Name_ID;
      إذا (انگ_named_parameters) *انگ_named_parameters = cv_params.انگ_Named_Parameters;
      إذا (پهريون_param_id) *پهريون_param_id = cv_params.پهريون_Param_UILabel_Name_ID;
      أرجع سچ;
    }
  }

  إذا (label_id) *label_id = HB_OT_NAME_ID_INVALID;
  إذا (tooltip_id) *tooltip_id = HB_OT_NAME_ID_INVALID;
  إذا (sample_id) *sample_id = HB_OT_NAME_ID_INVALID;
  إذا (انگ_named_parameters) *انگ_named_parameters = 0;
  إذا (پهريون_param_id) *پهريون_param_id = HB_OT_NAME_ID_INVALID;
  أرجع خطأ;
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
بنانشان صحيح
hb_ot_layout_feature_get_characters (hb_face_t      *face,
				     hb_ٽيگ_t        جدول_ٽيگ_2,
				     بنانشان صحيح    feature_اشاريو,
				     بنانشان صحيح    شروعات_وٿي,
				     بنانشان صحيح   *char_ڳڻپ, /* IN/OUT.  May be NULL */
				     hb_codepoint_t *characters  /* OUT.     May be NULL */)
{
  ثابت OT::GSUBGPOS &g = get_gsubgpos_جدول (face, جدول_ٽيگ_2);
  أرجع g.get_feature (feature_اشاريو)
	  .get_feature_params ()
	  .get_character_variants_params(g.get_feature_ٽيگ (feature_اشاريو))
	  .get_characters (شروعات_وٿي, char_ڳڻپ, characters);
}
#جيڪڏهن_پورو


/*
 * Parts of different types are implemented here such that they have direct
 * access to GSUB/GPOS lookups.
 */


بنية GSUBProxy
{
  ساكن constexpr بنانشان جدول_اشاريو = 0u;
  ساكن constexpr منطقي always_inplace = خطأ;
  تعريفنوع OT::SubstLookup Lookup;

  GSUBProxy (hb_face_t *face) :
    accel (*face->جدول.GSUB) {}

  ثابت GSUB::accelerator_t &accel;
};

بنية GPOSProxy
{
  ساكن constexpr بنانشان جدول_اشاريو = 1u;
  ساكن constexpr منطقي always_inplace = سچ;
  تعريفنوع OT::PosLookup Lookup;

  GPOSProxy (hb_face_t *face) :
    accel (*face->جدول.GPOS) {}

  ثابت GPOS::accelerator_t &accel;
};


ساكن مضمن منطقي
apply_forward (OT::hb_ot_apply_context_t *c,
	       ثابت OT::hb_ot_layout_lookup_accelerator_t &accel)
{
  منطقي use_hot_subtable_cache = accel.cache_enter (c);

  منطقي ret = خطأ;
  hb_بفر_t *بفر = c->بفر;
  طالما (بفر->successful)
  {
    hb_glyph_info_t *info = بفر->info;
    بنانشان j = بفر->idx;
    طالما (j < بفر->len &&
	   !(accel.digest.may_have (info[j].codepoint) &&
	     (info[j].نقاب & c->lookup_نقاب) &&
	     c->check_glyph_property (&info[j], c->lookup_props)))
      j++;
    إذا (unlikely (j > بفر->idx && !بفر->ايندڙ_glyphs (j - بفر->idx)))
      اكسر;
    إذا (بفر->idx >= بفر->len)
      اكسر;

    إذا (accel.apply (c, use_hot_subtable_cache))
      ret = سچ;
    وإلا
      (فراغ) بفر->ايندڙ_glyph ();
  }

  إذا (use_hot_subtable_cache)
    accel.cache_leave (c);

  أرجع ret;
}

ساكن مضمن منطقي
apply_backward (OT::hb_ot_apply_context_t *c,
	       ثابت OT::hb_ot_layout_lookup_accelerator_t &accel)
{
  منطقي ret = خطأ;
  hb_بفر_t *بفر = c->بفر;
  افعل
  {
    خودڪار &cur = بفر->cur();
    إذا (accel.digest.may_have (cur.codepoint) &&
	(cur.نقاب & c->lookup_نقاب) &&
	c->check_glyph_property (&cur, c->lookup_props))
      ret |= accel.apply (c, خطأ);

    /* The reverse lookup doesn't "advance" cursor (for good reason). */
    بفر->idx--;
  }
  طالما ((صحيح) بفر->idx >= 0);
  أرجع ret;
}

سانچو <قسم_نالو Proxy>
ساكن مضمن منطقي
apply_string (OT::hb_ot_apply_context_t *c,
	      ثابت قسم_نالو Proxy::Lookup &lookup,
	      ثابت OT::hb_ot_layout_lookup_accelerator_t &accel)
{
  hb_بفر_t *بفر = c->بفر;

  إذا (unlikely (!بفر->len || !c->lookup_نقاب))
    أرجع خطأ;

  منطقي ret = خطأ;

  c->سيٽ_lookup_props (lookup.get_props ());

  إذا (likely (!lookup.is_ابتو ()))
  {
    /* in/out forward substitution/positioning */
    إذا (!Proxy::always_inplace)
      بفر->clear_output ();

    بفر->idx = 0;
    ret = apply_forward (c, accel);

    إذا (!Proxy::always_inplace)
      بفر->متزامن ();
  }
  وإلا
  {
    /* in-place backward substitution/positioning */
    أكد (!بفر->have_output);
    بفر->idx = بفر->len - 1;
    ret = apply_backward (c, accel);
  }

  أرجع ret;
}

سانچو <قسم_نالو Proxy>
مضمن فراغ hb_ot_map_t::apply (ثابت Proxy &proxy,
				ثابت hb_ot_shape_plan_t *plan,
				hb_font_t *font,
				hb_بفر_t *بفر) ثابت
{
  ثابت بنانشان صحيح جدول_اشاريو = proxy.جدول_اشاريو;
  بنانشان صحيح i = 0;

  خودڪار *font_معلومات = font->معلومات.ot.get ();
  خودڪار *var_store_cache = (OT::hb_scalar_cache_t *) font_معلومات;

  OT::hb_ot_apply_context_t c (جدول_اشاريو, font, بفر, proxy.accel.get_blob (), var_store_cache);
  c.سيٽ_recurse_func (Proxy::Lookup::سانچو dispatch_recurse_func<OT::hb_ot_apply_context_t>);

  لكل (بنانشان صحيح stage_اشاريو = 0; stage_اشاريو < stages[جدول_اشاريو].ڊيگهائي; stage_اشاريو++)
  {
    ثابت stage_map_t *stage = &stages[جدول_اشاريو][stage_اشاريو];
    لكل (; i < stage->آخري_lookup; i++)
    {
      خودڪار &lookup = lookups[جدول_اشاريو][i];

      بنانشان صحيح lookup_اشاريو = lookup.اشاريو_2;

      خودڪار *accel = proxy.accel.get_accel (lookup_اشاريو);
      إذا (unlikely (!accel)) استمر;

      إذا (بفر->messaging () &&
	  !بفر->پيغام (font, "start lookup %u feature '%c%c%c%c'", lookup_اشاريو, HB_UNTAG (lookup.feature_ٽيگ))) استمر;

      /* Only try applying the lookup if there is any overlap. */
      إذا (accel->digest.may_intersect (بفر->digest) &&
	  ((بفر->flags & HB_بفر_FLAG_PRODUCE_UNSAFE_منزل_CONCAT) ||
	   accel->digest_ٻيو.may_intersect (بفر->digest)))
      {
	c.سيٽ_lookup_اشاريو (lookup_اشاريو);
	c.سيٽ_lookup_نقاب (lookup.نقاب, خطأ);
	c.سيٽ_auto_zwj (lookup.auto_zwj, خطأ);
	c.سيٽ_auto_zwnj (lookup.auto_zwnj, خطأ);
	c.سيٽ_random (lookup.random);
	c.سيٽ_في_syllable (lookup.في_syllable, خطأ);
	/* apply_string's set_lookup_props initializes the iterators. */

	apply_string<Proxy> (&c,
			     proxy.accel.جدول->get_lookup (lookup_اشاريو),
			     *accel);
      }
      وإلا إذا (بفر->messaging ())
	(فراغ) بفر->پيغام (font, "skipped lookup %u feature '%c%c%c%c' because no glyph matches", lookup_اشاريو, HB_UNTAG (lookup.feature_ٽيگ));

      إذا (بفر->messaging ())
	(فراغ) بفر->پيغام (font, "end lookup %u feature '%c%c%c%c'", lookup_اشاريو, HB_UNTAG (lookup.feature_ٽيگ));
    }

    إذا (stage->pause_func)
    {
      إذا (stage->pause_func (plan, font, بفر))
      {
	/* Refresh working buffer digest since buffer changed. */
	بفر->update_digest ();
      }
    }
  }
}

فراغ hb_ot_map_t::substitute (ثابت hb_ot_shape_plan_t *plan, hb_font_t *font, hb_بفر_t *بفر) ثابت
{
  GSUBProxy proxy (font->face);
  حرف ٽيگ_2[5] = {0};
  hb_ٽيگ_منزل_string (chosen_script[0], ٽيگ_2);
  إذا (بفر->messaging () &&
      !بفر->پيغام (font, "start table GSUB script tag '%s'", ٽيگ_2)) أرجع;
  apply (proxy, plan, font, بفر);
  إذا (بفر->messaging ())
    (فراغ) بفر->پيغام (font, "end table GSUB script tag '%s'", ٽيگ_2);
}

فراغ hb_ot_map_t::position (ثابت hb_ot_shape_plan_t *plan, hb_font_t *font, hb_بفر_t *بفر) ثابت
{
  GPOSProxy proxy (font->face);
  حرف ٽيگ_2[5] = {0};
  hb_ٽيگ_منزل_string (chosen_script[0], ٽيگ_2);
  إذا (بفر->messaging () &&
      !بفر->پيغام (font, "start table GPOS script tag '%s'", ٽيگ_2)) أرجع;
  apply (proxy, plan, font, بفر);
  إذا (بفر->messaging ())
    (فراغ) بفر->پيغام (font, "end table GPOS script tag '%s'", ٽيگ_2);
}

فراغ
hb_ot_layout_substitute_lookup (OT::hb_ot_apply_context_t *c,
				ثابت OT::SubstLookup &lookup,
				ثابت OT::hb_ot_layout_lookup_accelerator_t &accel)
{
  apply_string<GSUBProxy> (c, lookup, accel);
}

#جيڪڏهن_وصف_نه_ٿيل HB_NO_بنياد

ساكن فراغ
چونڊيو_بنياد_ٽيگ_فهرست (hb_script_t    script,
		  hb_language_t  language,
		  hb_ٽيگ_t      *script_ٽيگ,
		  hb_ٽيگ_t      *language_ٽيگ)
{
  hb_ٽيگ_t script_ٽيگ_فهرست[HB_OT_MAX_ٽيگ_فهرست_في_SCRIPT];
  بنانشان script_ڳڻپ = ARRAY_ڊيگهائي (script_ٽيگ_فهرست);

  hb_ٽيگ_t language_ٽيگ_فهرست[HB_OT_MAX_ٽيگ_فهرست_في_LANGUAGE];
  بنانشان language_ڳڻپ = ARRAY_ڊيگهائي (language_ٽيگ_فهرست);

  hb_ot_ٽيگ_فهرست_from_script_and_language (script, language,
				       &script_ڳڻپ, script_ٽيگ_فهرست,
				       &language_ڳڻپ, language_ٽيگ_فهرست);

  *script_ٽيگ = script_ڳڻپ ? script_ٽيگ_فهرست[script_ڳڻپ - 1] : HB_OT_ٽيگ_DEFAULT_SCRIPT;
  *language_ٽيگ = language_ڳڻپ ? language_ٽيگ_فهرست[language_ڳڻپ - 1] : HB_OT_ٽيگ_DEFAULT_LANGUAGE;
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
			       hb_ٽيگ_t           script_ٽيگ,
			       hb_ٽيگ_t           language_ٽيگ,
			       hb_font_extents_t *extents)
{
  hb_position_t min = 0, max = 0;
  إذا (font->face->جدول.بنياد_4->get_min_max (font, direction, script_ٽيگ, language_ٽيگ, HB_ٽيگ_NONE,
					   &min, &max))
  {
    إذا (extents)
    {
      extents->ascender  = max;
      extents->descender = min;
      extents->سٽ_gap  = 0;
    }
    أرجع سچ;
  }

  hb_font_get_extents_for_direction (font, direction, extents);
  أرجع خطأ;
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
  hb_ٽيگ_t script_ٽيگ, language_ٽيگ;
  چونڊيو_بنياد_ٽيگ_فهرست (script, language, &script_ٽيگ, &language_ٽيگ);
  أرجع hb_ot_layout_get_font_extents (font,
					direction,
					script_ٽيگ,
					language_ٽيگ,
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
hb_ot_layout_baseline_ٽيگ_t
hb_ot_layout_get_horizontal_baseline_ٽيگ_for_script (hb_script_t script)
{
  /* Keep in sync with hb_ot_layout_get_baseline_with_fallback */
  بدل ((صحيح) script)
  {
    /* Unicode-1.1 additions */
    حالة HB_SCRIPT_BENGALI:
    حالة HB_SCRIPT_DEVANAGARI:
    حالة HB_SCRIPT_GUJARATI:
    حالة HB_SCRIPT_GURMUKHI:
    /* Unicode-2.0 additions */
    حالة HB_SCRIPT_TIBETAN:
    /* Unicode-4.0 additions */
    حالة HB_SCRIPT_LIMBU:
    /* Unicode-4.1 additions */
    حالة HB_SCRIPT_SYLOTI_NAGRI:
    /* Unicode-5.0 additions */
    حالة HB_SCRIPT_PHAGS_PA:
    /* Unicode-5.2 additions */
    حالة HB_SCRIPT_MEETEI_MAYEK:
    /* Unicode-6.1 additions */
    حالة HB_SCRIPT_SHARADA:
    حالة HB_SCRIPT_TAKRI:
    /* Unicode-7.0 additions */
    حالة HB_SCRIPT_MODI:
    حالة HB_SCRIPT_SIDDHAM:
    حالة HB_SCRIPT_TIRHUTA:
    /* Unicode-9.0 additions */
    حالة HB_SCRIPT_MARCHEN:
    حالة HB_SCRIPT_NEWA:
    /* Unicode-10.0 additions */
    حالة HB_SCRIPT_SOYOMBO:
    حالة HB_SCRIPT_ZANABAZAR_چورس:
    /* Unicode-11.0 additions */
    حالة HB_SCRIPT_DOGRA:
    حالة HB_SCRIPT_GUNJALA_GONDI:
    /* Unicode-12.0 additions */
    حالة HB_SCRIPT_NANDINAGARI:
      أرجع HB_OT_LAYOUT_BASELINE_ٽيگ_HANGING;

    /* Unicode-1.1 additions */
    حالة HB_SCRIPT_HANGUL:
    حالة HB_SCRIPT_HAN:
    حالة HB_SCRIPT_HIRAGANA:
    حالة HB_SCRIPT_KATAKANA:
    /* Unicode-3.0 additions */
    حالة HB_SCRIPT_BOPOMOFO:
    /* Unicode-9.0 additions */
    حالة HB_SCRIPT_TANGUT:
    /* Unicode-10.0 additions */
    حالة HB_SCRIPT_NUSHU:
    /* Unicode-13.0 additions */
    حالة HB_SCRIPT_KHITAN_SMALL_SCRIPT:
    /* Unicode-18.0 additions */
    حالة HB_SCRIPT_JURCHEN:
    حالة HB_SCRIPT_SEAL:
      أرجع HB_OT_LAYOUT_BASELINE_ٽيگ_IDEO_FACE_BOTTOM_OR_کاٻو;

    افتراضي:
      أرجع HB_OT_LAYOUT_BASELINE_ٽيگ_ROMAN;
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
			   hb_ot_layout_baseline_ٽيگ_t  baseline_ٽيگ,
			   hb_direction_t               direction,
			   hb_ٽيگ_t                     script_ٽيگ,
			   hb_ٽيگ_t                     language_ٽيگ,
			   hb_position_t               *coord        /* OUT.  May be NULL. */)
{
  أرجع font->face->جدول.بنياد_4->get_baseline (font, baseline_ٽيگ, direction, script_ٽيگ, language_ٽيگ, coord);
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
			    hb_ot_layout_baseline_ٽيگ_t  baseline_ٽيگ,
			    hb_direction_t               direction,
			    hb_script_t                  script,
			    hb_language_t                language,
			    hb_position_t               *coord        /* OUT.  May be NULL. */)
{
  hb_ٽيگ_t script_ٽيگ, language_ٽيگ;
  چونڊيو_بنياد_ٽيگ_فهرست (script, language, &script_ٽيگ, &language_ٽيگ);
  أرجع hb_ot_layout_get_baseline (font,
				    baseline_ٽيگ,
				    direction,
				    script_ٽيگ,
				    language_ٽيگ,
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
فراغ
hb_ot_layout_get_baseline_with_fallback (hb_font_t                   *font,
					 hb_ot_layout_baseline_ٽيگ_t  baseline_ٽيگ,
					 hb_direction_t               direction,
					 hb_ٽيگ_t                     script_ٽيگ,
					 hb_ٽيگ_t                     language_ٽيگ,
					 hb_position_t               *coord /*ٻاهريون*/)
{
  إذا (hb_ot_layout_get_baseline (font,
				 baseline_ٽيگ,
				 direction,
				 script_ٽيگ,
				 language_ٽيگ,
				 coord))
    أرجع;

  /* Synthesize missing baselines.
   * See https://www.w3.org/TR/css-inline-3/#baseline-synthesis-fonts
   */
  بدل (baseline_ٽيگ)
  {
  حالة HB_OT_LAYOUT_BASELINE_ٽيگ_ROMAN:
    *coord = 0; // FIXME origin ?
    اكسر;

  حالة HB_OT_LAYOUT_BASELINE_ٽيگ_MATH:
    {
      hb_codepoint_t glyph;
      hb_glyph_extents_t extents;
      إذا (HB_DIRECTION_IS_HORIZONTAL (direction) &&
	  (hb_font_get_nominal_glyph (font, 0x2212u, &glyph) ||
	   hb_font_get_nominal_glyph (font, '-', &glyph)) &&
	  hb_font_get_glyph_extents (font, glyph, &extents))
      {
	*coord = hb_saturate_جوڙيو (extents.y_bearing, extents.اوچائي / 2);
      }
      وإلا
      {
	hb_position_t x_اوچائي = font->y_scale / 2;
#جيڪڏهن_وصف_نه_ٿيل HB_NO_METRICS
	hb_ot_metrics_get_position_with_fallback (font, HB_OT_METRICS_ٽيگ_X_اوچائي, &x_اوچائي);
#جيڪڏهن_پورو
	*coord = x_اوچائي / 2;
      }
    }
    اكسر;

  حالة HB_OT_LAYOUT_BASELINE_ٽيگ_IDEO_FACE_چوٽي_OR_ساڄو:
  حالة HB_OT_LAYOUT_BASELINE_ٽيگ_IDEO_FACE_BOTTOM_OR_کاٻو:
    {
      hb_position_t embox_چوٽي, embox_bottom;

      hb_ot_layout_get_baseline_with_fallback (font,
					       HB_OT_LAYOUT_BASELINE_ٽيگ_IDEO_EMBOX_چوٽي_OR_ساڄو,
					       direction,
					       script_ٽيگ,
					       language_ٽيگ,
					       &embox_چوٽي);
      hb_ot_layout_get_baseline_with_fallback (font,
					       HB_OT_LAYOUT_BASELINE_ٽيگ_IDEO_EMBOX_BOTTOM_OR_کاٻو,
					       direction,
					       script_ٽيگ,
					       language_ٽيگ,
					       &embox_bottom);

      إذا (baseline_ٽيگ == HB_OT_LAYOUT_BASELINE_ٽيگ_IDEO_FACE_چوٽي_OR_ساڄو)
	*coord = hb_saturate_جوڙيو (embox_چوٽي,
				  hb_clamp_منزل<hb_position_t> (((int64_t) embox_bottom - embox_چوٽي) / 10));
      وإلا
	*coord = hb_saturate_جوڙيو (embox_bottom,
				  hb_clamp_منزل<hb_position_t> (((int64_t) embox_چوٽي - embox_bottom) / 10));
    }
    اكسر;

  حالة HB_OT_LAYOUT_BASELINE_ٽيگ_IDEO_EMBOX_چوٽي_OR_ساڄو:
    إذا (hb_ot_layout_get_baseline (font,
				   HB_OT_LAYOUT_BASELINE_ٽيگ_IDEO_EMBOX_BOTTOM_OR_کاٻو,
				   direction,
				   script_ٽيگ,
				   language_ٽيگ,
				   coord))
      *coord = hb_saturate_جوڙيو (*coord,
				HB_DIRECTION_IS_HORIZONTAL (direction) ? font->y_scale : font->x_scale);
    وإلا
    {
      hb_font_extents_t font_extents;
      hb_font_get_extents_for_direction (font, direction, &font_extents);
      *coord = font_extents.ascender;
    }
    اكسر;

  حالة HB_OT_LAYOUT_BASELINE_ٽيگ_IDEO_EMBOX_BOTTOM_OR_کاٻو:
    إذا (hb_ot_layout_get_baseline (font,
				   HB_OT_LAYOUT_BASELINE_ٽيگ_IDEO_EMBOX_چوٽي_OR_ساڄو,
				   direction,
				   script_ٽيگ,
				   language_ٽيگ,
				   coord))
      *coord = hb_saturate_sub (*coord,
				HB_DIRECTION_IS_HORIZONTAL (direction) ? font->y_scale : font->x_scale);
    وإلا
    {
      hb_font_extents_t font_extents;
      hb_font_get_extents_for_direction (font, direction, &font_extents);
      *coord = font_extents.descender;
    }
    اكسر;

  حالة HB_OT_LAYOUT_BASELINE_ٽيگ_HANGING:
    إذا (HB_DIRECTION_IS_HORIZONTAL (direction))
    {
      hb_codepoint_t ch;
      hb_codepoint_t glyph;
      hb_glyph_extents_t extents;

      /* Keep in sync with hb_ot_layout_get_horizontal_baseline_for_script */
      بدل ((صحيح) script_ٽيگ)
      {
      /* Unicode-1.1 additions */
      حالة HB_SCRIPT_BENGALI:          ch = 0x0995u; اكسر;
      حالة HB_SCRIPT_DEVANAGARI:       ch = 0x0915u; اكسر;
      حالة HB_SCRIPT_GUJARATI:         ch = 0x0a95u; اكسر;
      حالة HB_SCRIPT_GURMUKHI:         ch = 0x0a15u; اكسر;
      /* Unicode-2.0 additions */
      حالة HB_SCRIPT_TIBETAN:          ch = 0x0f40u; اكسر;
      /* Unicode-4.0 additions */
      حالة HB_SCRIPT_LIMBU:            ch = 0x1901u; اكسر;
      /* Unicode-4.1 additions */
      حالة HB_SCRIPT_SYLOTI_NAGRI:     ch = 0xa807u; اكسر;
      /* Unicode-5.0 additions */
      حالة HB_SCRIPT_PHAGS_PA:         ch = 0xa840u; اكسر;
      /* Unicode-5.2 additions */
      حالة HB_SCRIPT_MEETEI_MAYEK:     ch = 0xabc0u; اكسر;
      /* Unicode-6.1 additions */
      حالة HB_SCRIPT_SHARADA:          ch = 0x11191u; اكسر;
      حالة HB_SCRIPT_TAKRI:            ch = 0x1168cu; اكسر;
      /* Unicode-7.0 additions */
      حالة HB_SCRIPT_MODI:             ch = 0x1160eu;اكسر;
      حالة HB_SCRIPT_SIDDHAM:          ch = 0x11590u; اكسر;
      حالة HB_SCRIPT_TIRHUTA:          ch = 0x1148fu; اكسر;
      /* Unicode-9.0 additions */
      حالة HB_SCRIPT_MARCHEN:          ch = 0x11c72u; اكسر;
      حالة HB_SCRIPT_NEWA:             ch = 0x1140eu; اكسر;
      /* Unicode-10.0 additions */
      حالة HB_SCRIPT_SOYOMBO:          ch = 0x11a5cu; اكسر;
      حالة HB_SCRIPT_ZANABAZAR_چورس: ch = 0x11a0bu; اكسر;
      /* Unicode-11.0 additions */
      حالة HB_SCRIPT_DOGRA:            ch = 0x1180au; اكسر;
      حالة HB_SCRIPT_GUNJALA_GONDI:    ch = 0x11d6cu; اكسر;
      /* Unicode-12.0 additions */
      حالة HB_SCRIPT_NANDINAGARI:      ch = 0x119b0u; اكسر;
      افتراضي:                         ch = 0;        اكسر;
      }

      إذا (ch &&
	  hb_font_get_nominal_glyph (font, ch, &glyph) &&
	  hb_font_get_glyph_extents (font, glyph, &extents))
	*coord = extents.y_bearing;
      وإلا
	*coord = hb_clamp_منزل<hb_position_t> ((int64_t) font->y_scale * 6 / 10); // FIXME makes assumptions about origin
    }
    وإلا
      *coord = hb_clamp_منزل<hb_position_t> ((int64_t) font->x_scale * 6 / 10); // FIXME makes assumptions about origin
    اكسر;

  حالة HB_OT_LAYOUT_BASELINE_ٽيگ_IDEO_EMBOX_CENTRAL:
    {
      hb_position_t چوٽي_2, bottom;
      hb_ot_layout_get_baseline_with_fallback (font,
					       HB_OT_LAYOUT_BASELINE_ٽيگ_IDEO_EMBOX_چوٽي_OR_ساڄو,
					       direction,
					       script_ٽيگ,
					       language_ٽيگ,
					       &چوٽي_2);
      hb_ot_layout_get_baseline_with_fallback (font,
					       HB_OT_LAYOUT_BASELINE_ٽيگ_IDEO_EMBOX_BOTTOM_OR_کاٻو,
					       direction,
					       script_ٽيگ,
					       language_ٽيگ,
					       &bottom);
      *coord = hb_clamp_منزل<hb_position_t> (((int64_t) چوٽي_2 + bottom) / 2);

    }
    اكسر;

  حالة HB_OT_LAYOUT_BASELINE_ٽيگ_IDEO_FACE_CENTRAL:
    {
      hb_position_t چوٽي_2, bottom;
      hb_ot_layout_get_baseline_with_fallback (font,
					       HB_OT_LAYOUT_BASELINE_ٽيگ_IDEO_FACE_چوٽي_OR_ساڄو,
					       direction,
					       script_ٽيگ,
					       language_ٽيگ,
					       &چوٽي_2);
      hb_ot_layout_get_baseline_with_fallback (font,
					       HB_OT_LAYOUT_BASELINE_ٽيگ_IDEO_FACE_BOTTOM_OR_کاٻو,
					       direction,
					       script_ٽيگ,
					       language_ٽيگ,
					       &bottom);
      *coord = hb_clamp_منزل<hb_position_t> (((int64_t) چوٽي_2 + bottom) / 2);

    }
    اكسر;

  حالة HB_OT_LAYOUT_BASELINE_ٽيگ_MAX_قدر:
  افتراضي:
    *coord = 0;
    اكسر;
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
فراغ
hb_ot_layout_get_baseline_with_fallback2 (hb_font_t                   *font,
					  hb_ot_layout_baseline_ٽيگ_t  baseline_ٽيگ,
					  hb_direction_t               direction,
					  hb_script_t                  script,
					  hb_language_t                language,
					  hb_position_t               *coord        /*ٻاهريون*/)
{
  hb_ٽيگ_t script_ٽيگ, language_ٽيگ;
  چونڊيو_بنياد_ٽيگ_فهرست (script, language, &script_ٽيگ, &language_ٽيگ);
  hb_ot_layout_get_baseline_with_fallback (font,
					   baseline_ٽيگ,
					   direction,
					   script_ٽيگ,
					   language_ٽيگ,
					   coord);
}

#جيڪڏهن_پورو


#جيڪڏهن_وصف_نه_ٿيل HB_NO_LAYOUT_RARELY_استعمال_ٿيل
بنية hb_get_glyph_alternates_dispatch_t :
       hb_dispatch_context_t<hb_get_glyph_alternates_dispatch_t, بنانشان>
{
  ساكن return_t default_return_قدر () { أرجع 0; }
  منطقي stop_sublookup_iteration (return_t r) ثابت { أرجع r; }

  خاص:
  سانچو <قسم_نالو T, قسم_نالو ...Ts> خودڪار
  _dispatch (ثابت T &obj, hb_ترجيح<1>, Ts&&... ds) HB_AUTO_RETURN
  ( obj.get_glyph_alternates (std::forward<Ts> (ds)...) )
  سانچو <قسم_نالو T, قسم_نالو ...Ts> خودڪار
  _dispatch (ثابت T &obj, hb_ترجيح<0>, Ts&&... ds) HB_AUTO_RETURN
  ( default_return_قدر () )
  عام:
  سانچو <قسم_نالو T, قسم_نالو ...Ts> خودڪار
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
HB_EXTERN بنانشان
hb_ot_layout_lookup_get_glyph_alternates (hb_face_t      *face,
					  بنانشان        lookup_اشاريو,
					  hb_codepoint_t  glyph,
					  بنانشان        شروعات_وٿي,
					  بنانشان       *alternate_ڳڻپ  /* IN/OUT.  May be NULL. */,
					  hb_codepoint_t *alternate_glyphs /* OUT.     May be NULL. */)
{
  hb_get_glyph_alternates_dispatch_t c;
  ثابت OT::SubstLookup &lookup = face->جدول.GSUB->جدول->get_lookup (lookup_اشاريو);
  خودڪار ret = lookup.dispatch (&c, glyph, شروعات_وٿي, alternate_ڳڻپ, alternate_glyphs);
  إذا (!ret && alternate_ڳڻپ) *alternate_ڳڻپ = 0;
  أرجع ret;
}

بنية hb_collect_glyph_alternates_dispatch_t :
       hb_dispatch_context_t<hb_collect_glyph_alternates_dispatch_t, منطقي>
{
  ساكن return_t default_return_قدر () { أرجع خطأ; }
  منطقي stop_sublookup_iteration (return_t r) ثابت { أرجع خطأ; }

  خاص:
  سانچو <قسم_نالو T, قسم_نالو ...Ts> خودڪار
  _dispatch (ثابت T &obj, hb_ترجيح<1>, Ts&&... ds) HB_AUTO_RETURN
  ( (obj.collect_glyph_alternates (std::forward<Ts> (ds)...), سچ) )
  سانچو <قسم_نالو T, قسم_نالو ...Ts> خودڪار
  _dispatch (ثابت T &obj, hb_ترجيح<0>, Ts&&... ds) HB_AUTO_RETURN
  ( default_return_قدر () )
  عام:
  سانچو <قسم_نالو T, قسم_نالو ...Ts> خودڪار
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
					      بنانشان   lookup_اشاريو,
					      hb_map_t  *alternate_ڳڻپ /* IN/OUT */,
					      hb_map_t  *alternate_glyphs /* IN/OUT */)
{
  hb_collect_glyph_alternates_dispatch_t c;
  ثابت OT::SubstLookup &lookup = face->جدول.GSUB->جدول->get_lookup (lookup_اشاريو);
  أرجع lookup.dispatch (&c, alternate_ڳڻپ, alternate_glyphs);
}

بنية hb_position_single_dispatch_t :
       hb_dispatch_context_t<hb_position_single_dispatch_t, منطقي>
{
  ساكن return_t default_return_قدر () { أرجع خطأ; }
  منطقي stop_sublookup_iteration (return_t r) ثابت { أرجع r; }

  خاص:
  سانچو <قسم_نالو T, قسم_نالو ...Ts> خودڪار
  _dispatch (ثابت T &obj, hb_ترجيح<1>, Ts&&... ds) HB_AUTO_RETURN
  ( obj.position_single (std::forward<Ts> (ds)...) )
  سانچو <قسم_نالو T, قسم_نالو ...Ts> خودڪار
  _dispatch (ثابت T &obj, hb_ترجيح<0>, Ts&&... ds) HB_AUTO_RETURN
  ( default_return_قدر () )
  عام:
  سانچو <قسم_نالو T, قسم_نالو ...Ts> خودڪار
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
				       بنانشان        lookup_اشاريو,
				       hb_direction_t  direction,
				       hb_codepoint_t  glyph)
{
  ثابت OT::PosLookup &lookup = font->face->جدول.GPOS->جدول->get_lookup (lookup_اشاريو);
  hb_blob_t *blob = font->face->جدول.GPOS->get_blob ();
  hb_glyph_position_t pos = {0};
  hb_position_single_dispatch_t c;
  lookup.dispatch (&c, font, blob, direction, glyph, pos);
  hb_position_t ret = 0;
  بدل (direction)
  {
    حالة HB_DIRECTION_LTR:
      ret = pos.x_وٿي_2;
      اكسر;
    حالة HB_DIRECTION_RTL:
      ret = hb_saturate_sub (pos.x_advance, pos.x_وٿي_2);
      اكسر;
    حالة HB_DIRECTION_TTB:
      ret = pos.y_وٿي_2;
      اكسر;
    حالة HB_DIRECTION_BTT:
      ret = hb_saturate_sub (pos.y_advance, pos.y_وٿي_2);
      اكسر;
    حالة HB_DIRECTION_INVALID:
    افتراضي:
      اكسر;
  }
  أرجع ret;
}
#جيڪڏهن_پورو


#جيڪڏهن_پورو
