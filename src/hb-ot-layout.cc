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

#समावेश "hb.hh"

#अगर_अघोषित HB_NO_OT_LAYOUT

#अगर_घोषित HB_NO_OT_TAG
#त्रुटि "Cannot compile hb-ot-layout.cc with HB_NO_OT_TAG."
#पूर्ण_अगर

#समावेश "hb-open-type.hh"
#समावेश "hb-ot-layout.hh"
#समावेश "hb-ot-face.hh"
#समावेश "hb-ot-map.hh"
#समावेश "hb-map.hh"

#समावेश "hb-ot-kern-table.hh"
#समावेश "hb-ot-layout-gdef-table.hh"
#समावेश "hb-ot-layout-gsub-table.hh"
#समावेश "hb-ot-layout-gpos-table.hh"
#समावेश "hb-ot-layout-base-table.hh"
#समावेश "hb-ot-layout-jstf-table.hh" // Just so we compile it; unused otherwise.
#समावेश "hb-ot-name-table.hh"
#समावेश "hb-ot-os2-table.hh"

#समावेश "hb-aat-layout-morx-table.hh"
#समावेश "hb-aat-layout-opbd-table.hh" // Just so we compile it; unused otherwise.

using OT::Layout::GSUB;
using OT::Layout::GPOS;

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

#अगर_अघोषित HB_NO_OT_KERN
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
bool
hb_ot_layout_has_kerning (hb_face_t *face)
{
  वापस face->table.kern->table->has_तथ्य ();
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
bool
hb_ot_layout_has_machine_kerning (hb_face_t *face)
{
  वापस face->table.kern->table->has_state_machine ();
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
bool
hb_ot_layout_has_cross_kerning (hb_face_t *face)
{
  वापस face->table.kern->table->has_cross_stream ();
}

व्योम
hb_ot_layout_kern (स्थिर hb_ot_shape_plan_t *plan,
		   hb_font_t *font,
		   hb_बफर_t  *बफर)
{
  स्वतः &accel = *font->face->table.kern;
  hb_blob_t *blob = accel.get_blob ();

  AAT::hb_aat_apply_context_t c (plan, font, बफर, blob);

  अगर (!बफर->message (font, "start table kern")) वापस;
  c.बफर_glyph_set = accel.scratch.create_बफर_glyph_set ();
  accel.apply (&c);
  accel.scratch.destroy_बफर_glyph_set (c.बफर_glyph_set);
  (व्योम) बफर->message (font, "end table kern");
}
#पूर्ण_अगर


/*
 * GDEF
 */

bool
OT::GDEF::is_blocklisted (hb_blob_t *blob,
			  hb_face_t *face) स्थिर
{
#अगर_घोषित HB_NO_OT_LAYOUT_BLOCKLIST
  वापस false;
#पूर्ण_अगर
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
  चयन HB_CODEPOINT_ENCODE3(blob->लंबाई,
			      face->table.GSUB->table.get_लंबाई (),
			      face->table.GPOS->table.get_लंबाई ())
  {
    /* sha1sum:c5ee92f0bca4bfb7d06c4d03e8cf9f9cf75d2e8a Windows 7? timesi.ttf */
    हाल HB_CODEPOINT_ENCODE3 (442, 2874, 42038):
    /* sha1sum:37fc8c16a0894ab7b749e35579856c73c840867b Windows 7? timesbi.ttf */
    हाल HB_CODEPOINT_ENCODE3 (430, 2874, 40662):
    /* sha1sum:19fc45110ea6cd3cdd0a5faca256a3797a069a80 Windows 7 timesi.ttf */
    हाल HB_CODEPOINT_ENCODE3 (442, 2874, 39116):
    /* sha1sum:6d2d3c9ed5b7de87bc84eae0df95ee5232ecde26 Windows 7 timesbi.ttf */
    हाल HB_CODEPOINT_ENCODE3 (430, 2874, 39374):
    /* sha1sum:8583225a8b49667c077b3525333f84af08c6bcd8 OS X 10.11.3 Times New Roman Italic.ttf */
    हाल HB_CODEPOINT_ENCODE3 (490, 3046, 41638):
    /* sha1sum:ec0f5a8751845355b7c3271d11f9918a966cb8c9 OS X 10.11.3 Times New Roman Bold Italic.ttf */
    हाल HB_CODEPOINT_ENCODE3 (478, 3046, 41902):
    /* sha1sum:96eda93f7d33e79962451c6c39a6b51ee893ce8c  tahoma.ttf from Windows 8 */
    हाल HB_CODEPOINT_ENCODE3 (898, 12554, 46470):
    /* sha1sum:20928dc06014e0cd120b6fc942d0c3b1a46ac2bc  tahomabd.ttf from Windows 8 */
    हाल HB_CODEPOINT_ENCODE3 (910, 12566, 47732):
    /* sha1sum:4f95b7e4878f60fa3a39ca269618dfde9721a79e  tahoma.ttf from Windows 8.1 */
    हाल HB_CODEPOINT_ENCODE3 (928, 23298, 59332):
    /* sha1sum:6d400781948517c3c0441ba42acb309584b73033  tahomabd.ttf from Windows 8.1 */
    हाल HB_CODEPOINT_ENCODE3 (940, 23310, 60732):
    /* tahoma.ttf v6.04 from Windows 8.1 x64, see https://bugzilla.mozilla.org/show_bug.cgi?id=1279925 */
    हाल HB_CODEPOINT_ENCODE3 (964, 23836, 60072):
    /* tahomabd.ttf v6.04 from Windows 8.1 x64, see https://bugzilla.mozilla.org/show_bug.cgi?id=1279925 */
    हाल HB_CODEPOINT_ENCODE3 (976, 23832, 61456):
    /* sha1sum:e55fa2dfe957a9f7ec26be516a0e30b0c925f846  tahoma.ttf from Windows 10 */
    हाल HB_CODEPOINT_ENCODE3 (994, 24474, 60336):
    /* sha1sum:7199385abb4c2cc81c83a151a7599b6368e92343  tahomabd.ttf from Windows 10 */
    हाल HB_CODEPOINT_ENCODE3 (1006, 24470, 61740):
    /* tahoma.ttf v6.91 from Windows 10 x64, see https://bugzilla.mozilla.org/show_bug.cgi?id=1279925 */
    हाल HB_CODEPOINT_ENCODE3 (1006, 24576, 61346):
    /* tahomabd.ttf v6.91 from Windows 10 x64, see https://bugzilla.mozilla.org/show_bug.cgi?id=1279925 */
    हाल HB_CODEPOINT_ENCODE3 (1018, 24572, 62828):
    /* sha1sum:b9c84d820c49850d3d27ec498be93955b82772b5  tahoma.ttf from Windows 10 AU */
    हाल HB_CODEPOINT_ENCODE3 (1006, 24576, 61352):
    /* sha1sum:2bdfaab28174bdadd2f3d4200a30a7ae31db79d2  tahomabd.ttf from Windows 10 AU */
    हाल HB_CODEPOINT_ENCODE3 (1018, 24572, 62834):
    /* sha1sum:b0d36cf5a2fbe746a3dd277bffc6756a820807a7  Tahoma.ttf from Mac OS X 10.9 */
    हाल HB_CODEPOINT_ENCODE3 (832, 7324, 47162):
    /* sha1sum:12fc4538e84d461771b30c18b5eb6bd434e30fba  Tahoma Bold.ttf from Mac OS X 10.9 */
    हाल HB_CODEPOINT_ENCODE3 (844, 7302, 45474):
    /* sha1sum:eb8afadd28e9cf963e886b23a30b44ab4fd83acc  himalaya.ttf from Windows 7 */
    हाल HB_CODEPOINT_ENCODE3 (180, 13054, 7254):
    /* sha1sum:73da7f025b238a3f737aa1fde22577a6370f77b0  himalaya.ttf from Windows 8 */
    हाल HB_CODEPOINT_ENCODE3 (192, 12638, 7254):
    /* sha1sum:6e80fd1c0b059bbee49272401583160dc1e6a427  himalaya.ttf from Windows 8.1 */
    हाल HB_CODEPOINT_ENCODE3 (192, 12690, 7254):
    /* 8d9267aea9cd2c852ecfb9f12a6e834bfaeafe44  cantarell-fonts-0.0.21/otf/Cantarell-Regular.otf */
    /* 983988ff7b47439ab79aeaf9a45bd4a2c5b9d371  cantarell-fonts-0.0.21/otf/Cantarell-Oblique.otf */
    हाल HB_CODEPOINT_ENCODE3 (188, 248, 3852):
    /* 2c0c90c6f6087ffbfea76589c93113a9cbb0e75f  cantarell-fonts-0.0.21/otf/Cantarell-Bold.otf */
    /* 55461f5b853c6da88069ffcdf7f4dd3f8d7e3e6b  cantarell-fonts-0.0.21/otf/Cantarell-Bold-Oblique.otf */
    हाल HB_CODEPOINT_ENCODE3 (188, 264, 3426):
    /* d125afa82a77a6475ac0e74e7c207914af84b37a padauk-2.80/Padauk.ttf RHEL 7.2 */
    हाल HB_CODEPOINT_ENCODE3 (1058, 47032, 11818):
    /* 0f7b80437227b90a577cc078c0216160ae61b031 padauk-2.80/Padauk-Bold.ttf RHEL 7.2*/
    हाल HB_CODEPOINT_ENCODE3 (1046, 47030, 12600):
    /* d3dde9aa0a6b7f8f6a89ef1002e9aaa11b882290 padauk-2.80/Padauk.ttf Ubuntu 16.04 */
    हाल HB_CODEPOINT_ENCODE3 (1058, 71796, 16770):
    /* 5f3c98ccccae8a953be2d122c1b3a77fd805093f padauk-2.80/Padauk-Bold.ttf Ubuntu 16.04 */
    हाल HB_CODEPOINT_ENCODE3 (1046, 71790, 17862):
    /* 6c93b63b64e8b2c93f5e824e78caca555dc887c7 padauk-2.80/Padauk-book.ttf */
    हाल HB_CODEPOINT_ENCODE3 (1046, 71788, 17112):
    /* d89b1664058359b8ec82e35d3531931125991fb9 padauk-2.80/Padauk-bookbold.ttf */
    हाल HB_CODEPOINT_ENCODE3 (1058, 71794, 17514):
    /* 824cfd193aaf6234b2b4dc0cf3c6ef576c0d00ef padauk-3.0/Padauk-book.ttf */
    हाल HB_CODEPOINT_ENCODE3 (1330, 109904, 57938):
    /* 91fcc10cf15e012d27571e075b3b4dfe31754a8a padauk-3.0/Padauk-bookbold.ttf */
    हाल HB_CODEPOINT_ENCODE3 (1330, 109904, 58972):
    /* sha1sum: c26e41d567ed821bed997e937bc0c41435689e85  Padauk.ttf
     *  "Padauk Regular" "Version 2.5", see https://crbug.com/681813 */
    हाल HB_CODEPOINT_ENCODE3 (1004, 59092, 14836):
    /* 88d2006ca084f04af2df1954ed714a8c71e8400f  Courier New.ttf from macOS 15 */
    हाल HB_CODEPOINT_ENCODE3 (588, 5078, 14418):
    /* 608e3ebb6dd1aee521cff08eb07d500a2c59df68  Courier New Bold.ttf from macOS 15 */
    हाल HB_CODEPOINT_ENCODE3 (588, 5078, 14238):
    /* d13221044ff054efd78f1cd8631b853c3ce85676  cour.ttf from Windows 10 */
    हाल HB_CODEPOINT_ENCODE3 (894, 17162, 33960):
    /* 68ed4a22d8067fcf1622ac6f6e2f4d3a2e3ec394  courbd.ttf from Windows 10 */
    हाल HB_CODEPOINT_ENCODE3 (894, 17154, 34472):
    /* 4cdb0259c96b7fd7c103821bb8f08f7cc6b211d7  cour.ttf from Windows 8.1 */
    हाल HB_CODEPOINT_ENCODE3 (816, 7868, 17052):
    /* 920483d8a8ed37f7f0afdabbe7f679aece7c75d8  courbd.ttf from Windows 8.1 */
    हाल HB_CODEPOINT_ENCODE3 (816, 7868, 17138):
      वापस true;
  }
  वापस false;
}

ढाँचा <bool SetDigest>
अटल व्योम
_hb_ot_layout_set_glyph_props (hb_font_t *font,
			       hb_बफर_t *बफर)
{
  hb_बफर_assert_gsubgpos_vars (बफर);

  स्थिर स्वतः &gdef = *font->face->table.GDEF;
  अचिन्हित पूर्णांक गिनती = बफर->len;
  hb_glyph_info_t *info = बफर->info;
  क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
  {
    अगर (SetDigest)
      बफर->digest.add (info[i].codepoint);
    _hb_glyph_info_set_glyph_props (&info[i], gdef.get_glyph_props (info[i].codepoint));
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
  वापस face->table.GDEF->table->has_glyph_classes ();
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
  वापस (hb_ot_layout_glyph_class_t) face->table.GDEF->table->get_glyph_class (glyph);
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
व्योम
hb_ot_layout_get_glyphs_in_class (hb_face_t                  *face,
				  hb_ot_layout_glyph_class_t  klass,
				  hb_set_t                   *glyphs /* OUT */)
{
  वापस face->table.GDEF->table->get_glyphs_in_class (klass, glyphs);
}

#अगर_अघोषित HB_NO_LAYOUT_UNUSED
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
अचिन्हित पूर्णांक
hb_ot_layout_get_attach_points (hb_face_t      *face,
				hb_codepoint_t  glyph,
				अचिन्हित पूर्णांक    start_offset,
				अचिन्हित पूर्णांक   *point_गिनती /* IN/OUT */,
				अचिन्हित पूर्णांक   *point_array /* OUT */)
{
  वापस face->table.GDEF->table->get_attach_points (glyph,
						     start_offset,
						     point_गिनती,
						     point_array);
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
अचिन्हित पूर्णांक
hb_ot_layout_get_ligature_carets (hb_font_t      *font,
				  hb_direction_t  direction,
				  hb_codepoint_t  glyph,
				  अचिन्हित पूर्णांक    start_offset,
				  अचिन्हित पूर्णांक   *caret_गिनती /* IN/OUT */,
				  hb_position_t  *caret_array /* OUT */)
{
  वापस font->face->table.GDEF->table->get_lig_carets (font, direction, glyph, start_offset, caret_गिनती, caret_array);
}
#पूर्ण_अगर


/*
 * GSUB/GPOS
 */

bool
GSUB::is_blocklisted (hb_blob_t *blob HB_UNUSED,
			  hb_face_t *face) स्थिर
{
#अगर_घोषित HB_NO_OT_LAYOUT_BLOCKLIST
  वापस false;
#पूर्ण_अगर
  वापस false;
}

bool
GPOS::is_blocklisted (hb_blob_t *blob HB_UNUSED,
			  hb_face_t *face HB_UNUSED) स्थिर
{
#अगर_घोषित HB_NO_OT_LAYOUT_BLOCKLIST
  वापस false;
#पूर्ण_अगर
  वापस false;
}

अटल स्थिर OT::GSUBGPOS&
get_gsubgpos_table (hb_face_t *face,
		    hb_tag_t   table_tag)
{
  चयन (table_tag) {
    हाल HB_OT_TAG_GSUB: वापस *face->table.GSUB->table;
    हाल HB_OT_TAG_GPOS: वापस *face->table.GPOS->table;
    शेष:             वापस Null (OT::GSUBGPOS);
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
अचिन्हित पूर्णांक
hb_ot_layout_table_get_script_tags (hb_face_t    *face,
				    hb_tag_t      table_tag,
				    अचिन्हित पूर्णांक  start_offset,
				    अचिन्हित पूर्णांक *script_गिनती /* IN/OUT */,
				    hb_tag_t     *script_tags  /* OUT */)
{
  स्थिर OT::GSUBGPOS &g = get_gsubgpos_table (face, table_tag);

  वापस g.get_script_tags (start_offset, script_गिनती, script_tags);
}

#घोषणा HB_OT_TAG_LATIN_SCRIPT		HB_TAG ('l', 'a', 't', 'n')

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
hb_ot_layout_table_find_script (hb_face_t    *face,
				hb_tag_t      table_tag,
				hb_tag_t      script_tag,
				अचिन्हित पूर्णांक *script_सूचकांक /* OUT */)
{
  static_assert ((OT::सूचकांक::NOT_FOUND_सूचकांक == HB_OT_LAYOUT_NO_SCRIPT_सूचकांक), "");
  स्थिर OT::GSUBGPOS &g = get_gsubgpos_table (face, table_tag);

  अगर (g.find_script_सूचकांक (script_tag, script_सूचकांक))
    वापस true;

  /* try finding 'DFLT' */
  अगर (g.find_script_सूचकांक (HB_OT_TAG_DEFAULT_SCRIPT, script_सूचकांक))
    वापस false;

  /* try with 'dflt'; MS site has had typos and many fonts use it now :(.
   * including many versions of DejaVu Sans Mono! */
  अगर (g.find_script_सूचकांक (HB_OT_TAG_DEFAULT_LANGUAGE, script_सूचकांक))
    वापस false;

  /* try with 'latn'; some old fonts put their features there even though
     they're really trying to support Thai, for example :( */
  अगर (g.find_script_सूचकांक (HB_OT_TAG_LATIN_SCRIPT, script_सूचकांक))
    वापस false;

  अगर (script_सूचकांक) *script_सूचकांक = HB_OT_LAYOUT_NO_SCRIPT_सूचकांक;
  वापस false;
}

#अगर_अघोषित HB_DISABLE_DEPRECATED
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
hb_ot_layout_table_choose_script (hb_face_t      *face,
				  hb_tag_t        table_tag,
				  स्थिर hb_tag_t *script_tags,
				  अचिन्हित पूर्णांक   *script_सूचकांक  /* OUT */,
				  hb_tag_t       *chosen_script /* OUT */)
{
  स्थिर hb_tag_t *t;
  क्रम (t = script_tags; *t; t++);
  वापस hb_ot_layout_table_select_script (face, table_tag, t - script_tags, script_tags, script_सूचकांक, chosen_script);
}
#पूर्ण_अगर

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
hb_ot_layout_table_select_script (hb_face_t      *face,
				  hb_tag_t        table_tag,
				  अचिन्हित पूर्णांक    script_गिनती,
				  स्थिर hb_tag_t *script_tags,
				  अचिन्हित पूर्णांक   *script_सूचकांक  /* OUT */,
				  hb_tag_t       *chosen_script /* OUT */)
{
  static_assert ((OT::सूचकांक::NOT_FOUND_सूचकांक == HB_OT_LAYOUT_NO_SCRIPT_सूचकांक), "");
  स्थिर OT::GSUBGPOS &g = get_gsubgpos_table (face, table_tag);
  अचिन्हित पूर्णांक i;

  क्रम (i = 0; i < script_गिनती; i++)
  {
    अगर (g.find_script_सूचकांक (script_tags[i], script_सूचकांक))
    {
      अगर (chosen_script)
	*chosen_script = script_tags[i];
      वापस true;
    }
  }

  /* try finding 'DFLT' */
  अगर (g.find_script_सूचकांक (HB_OT_TAG_DEFAULT_SCRIPT, script_सूचकांक)) {
    अगर (chosen_script)
      *chosen_script = HB_OT_TAG_DEFAULT_SCRIPT;
    वापस false;
  }

  /* try with 'dflt'; MS site has had typos and many fonts use it now :( */
  अगर (g.find_script_सूचकांक (HB_OT_TAG_DEFAULT_LANGUAGE, script_सूचकांक)) {
    अगर (chosen_script)
      *chosen_script = HB_OT_TAG_DEFAULT_LANGUAGE;
    वापस false;
  }

  /* try with 'latn'; some old fonts put their features there even though
     they're really trying to support Thai, for example :( */
  अगर (g.find_script_सूचकांक (HB_OT_TAG_LATIN_SCRIPT, script_सूचकांक)) {
    अगर (chosen_script)
      *chosen_script = HB_OT_TAG_LATIN_SCRIPT;
    वापस false;
  }

  अगर (script_सूचकांक) *script_सूचकांक = HB_OT_LAYOUT_NO_SCRIPT_सूचकांक;
  अगर (chosen_script)
    *chosen_script = HB_TAG_NONE;
  वापस false;
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
अचिन्हित पूर्णांक
hb_ot_layout_table_get_feature_tags (hb_face_t    *face,
				     hb_tag_t      table_tag,
				     अचिन्हित पूर्णांक  start_offset,
				     अचिन्हित पूर्णांक *feature_गिनती_2 /* IN/OUT */,
				     hb_tag_t     *feature_tags  /* OUT */)
{
  स्थिर OT::GSUBGPOS &g = get_gsubgpos_table (face, table_tag);

  वापस g.get_feature_tags (start_offset, feature_गिनती_2, feature_tags);
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
bool
hb_ot_layout_table_find_feature (hb_face_t    *face,
				 hb_tag_t      table_tag,
				 hb_tag_t      feature_tag,
				 अचिन्हित पूर्णांक *feature_सूचकांक /* OUT */)
{
  static_assert ((OT::सूचकांक::NOT_FOUND_सूचकांक == HB_OT_LAYOUT_NO_FEATURE_सूचकांक), "");
  स्थिर OT::GSUBGPOS &g = get_gsubgpos_table (face, table_tag);

  अचिन्हित पूर्णांक अंक_features = g.get_feature_गिनती ();
  क्रम (अचिन्हित पूर्णांक i = 0; i < अंक_features; i++)
  {
    अगर (feature_tag == g.get_feature_tag (i)) {
      अगर (feature_सूचकांक) *feature_सूचकांक = i;
      वापस true;
    }
  }

  अगर (feature_सूचकांक) *feature_सूचकांक = HB_OT_LAYOUT_NO_FEATURE_सूचकांक;
  वापस false;
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
अचिन्हित पूर्णांक
hb_ot_layout_script_get_language_tags (hb_face_t    *face,
				       hb_tag_t      table_tag,
				       अचिन्हित पूर्णांक  script_सूचकांक,
				       अचिन्हित पूर्णांक  start_offset,
				       अचिन्हित पूर्णांक *language_गिनती /* IN/OUT */,
				       hb_tag_t     *language_tags  /* OUT */)
{
  स्थिर OT::Script &s = get_gsubgpos_table (face, table_tag).get_script (script_सूचकांक);

  वापस s.get_lang_sys_tags (start_offset, language_गिनती, language_tags);
}


#अगर_अघोषित HB_DISABLE_DEPRECATED
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
hb_ot_layout_script_find_language (hb_face_t    *face,
				   hb_tag_t      table_tag,
				   अचिन्हित पूर्णांक  script_सूचकांक,
				   hb_tag_t      language_tag,
				   अचिन्हित पूर्णांक *language_सूचकांक)
{
  वापस hb_ot_layout_script_select_language (face,
					      table_tag,
					      script_सूचकांक,
					      1,
					      &language_tag,
					      language_सूचकांक);
}
#पूर्ण_अगर


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
				     hb_tag_t        table_tag,
				     अचिन्हित पूर्णांक    script_सूचकांक,
				     अचिन्हित पूर्णांक    language_गिनती,
				     स्थिर hb_tag_t *language_tags,
				     अचिन्हित पूर्णांक   *language_सूचकांक /* OUT */,
				     hb_tag_t       *chosen_language /* OUT */)
{
  static_assert ((OT::सूचकांक::NOT_FOUND_सूचकांक == HB_OT_LAYOUT_DEFAULT_LANGUAGE_सूचकांक), "");
  स्थिर OT::Script &s = get_gsubgpos_table (face, table_tag).get_script (script_सूचकांक);
  अचिन्हित पूर्णांक i;

  क्रम (i = 0; i < language_गिनती; i++)
  {
    अगर (s.find_lang_sys_सूचकांक (language_tags[i], language_सूचकांक))
    {
      अगर (chosen_language)
        *chosen_language = language_tags[i];
      वापस true;
    }
  }

  /* try finding 'dflt' */
  अगर (s.find_lang_sys_सूचकांक (HB_OT_TAG_DEFAULT_LANGUAGE, language_सूचकांक))
  {
    अगर (chosen_language)
      *chosen_language = HB_OT_TAG_DEFAULT_LANGUAGE;
    वापस false;
  }

  अगर (language_सूचकांक)
    *language_सूचकांक = HB_OT_LAYOUT_DEFAULT_LANGUAGE_सूचकांक;
  अगर (chosen_language)
    *chosen_language = HB_TAG_NONE;
  वापस false;
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
				     hb_tag_t        table_tag,
				     अचिन्हित पूर्णांक    script_सूचकांक,
				     अचिन्हित पूर्णांक    language_गिनती,
				     स्थिर hb_tag_t *language_tags,
				     अचिन्हित पूर्णांक   *language_सूचकांक /* OUT */)
{
  वापस hb_ot_layout_script_select_language2 (face, table_tag,
					       script_सूचकांक,
					       language_गिनती, language_tags,
					       language_सूचकांक, nullptr);
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
hb_ot_layout_language_get_required_feature_सूचकांक (hb_face_t    *face,
						  hb_tag_t      table_tag,
						  अचिन्हित पूर्णांक  script_सूचकांक,
						  अचिन्हित पूर्णांक  language_सूचकांक,
						  अचिन्हित पूर्णांक *feature_सूचकांक /* OUT */)
{
  वापस hb_ot_layout_language_get_required_feature (face,
						     table_tag,
						     script_सूचकांक,
						     language_सूचकांक,
						     feature_सूचकांक,
						     nullptr);
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
					    hb_tag_t      table_tag,
					    अचिन्हित पूर्णांक  script_सूचकांक,
					    अचिन्हित पूर्णांक  language_सूचकांक,
					    अचिन्हित पूर्णांक *feature_सूचकांक /* OUT */,
					    hb_tag_t     *feature_tag   /* OUT */)
{
  स्थिर OT::GSUBGPOS &g = get_gsubgpos_table (face, table_tag);
  स्थिर OT::LangSys &l = g.get_script (script_सूचकांक).get_lang_sys (language_सूचकांक);

  अचिन्हित पूर्णांक सूचकांक_2 = l.get_required_feature_सूचकांक ();
  अगर (feature_सूचकांक) *feature_सूचकांक = सूचकांक_2;
  अगर (feature_tag) *feature_tag = g.get_feature_tag (सूचकांक_2);

  वापस l.has_required_feature ();
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
अचिन्हित पूर्णांक
hb_ot_layout_language_get_feature_indexes (hb_face_t    *face,
					   hb_tag_t      table_tag,
					   अचिन्हित पूर्णांक  script_सूचकांक,
					   अचिन्हित पूर्णांक  language_सूचकांक,
					   अचिन्हित पूर्णांक  start_offset,
					   अचिन्हित पूर्णांक *feature_गिनती_2   /* IN/OUT */,
					   अचिन्हित पूर्णांक *feature_indexes /* OUT */)
{
  स्थिर OT::GSUBGPOS &g = get_gsubgpos_table (face, table_tag);
  स्थिर OT::LangSys &l = g.get_script (script_सूचकांक).get_lang_sys (language_सूचकांक);

  वापस l.get_feature_indexes (start_offset, feature_गिनती_2, feature_indexes);
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
अचिन्हित पूर्णांक
hb_ot_layout_language_get_feature_tags (hb_face_t    *face,
					hb_tag_t      table_tag,
					अचिन्हित पूर्णांक  script_सूचकांक,
					अचिन्हित पूर्णांक  language_सूचकांक,
					अचिन्हित पूर्णांक  start_offset,
					अचिन्हित पूर्णांक *feature_गिनती_2 /* IN/OUT */,
					hb_tag_t     *feature_tags  /* OUT */)
{
  स्थिर OT::GSUBGPOS &g = get_gsubgpos_table (face, table_tag);
  स्थिर OT::LangSys &l = g.get_script (script_सूचकांक).get_lang_sys (language_सूचकांक);

  static_assert ((माप (अचिन्हित पूर्णांक) == माप (hb_tag_t)), "");
  अचिन्हित पूर्णांक ret = l.get_feature_indexes (start_offset, feature_गिनती_2, (अचिन्हित पूर्णांक *) feature_tags);

  अगर (feature_गिनती_2 && feature_tags) {
    अचिन्हित पूर्णांक गिनती = *feature_गिनती_2;
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
      feature_tags[i] = g.get_feature_tag ((अचिन्हित पूर्णांक) feature_tags[i]);
  }

  वापस ret;
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
hb_ot_layout_language_find_feature (hb_face_t    *face,
				    hb_tag_t      table_tag,
				    अचिन्हित पूर्णांक  script_सूचकांक,
				    अचिन्हित पूर्णांक  language_सूचकांक,
				    hb_tag_t      feature_tag,
				    अचिन्हित पूर्णांक *feature_सूचकांक /* OUT */)
{
  static_assert ((OT::सूचकांक::NOT_FOUND_सूचकांक == HB_OT_LAYOUT_NO_FEATURE_सूचकांक), "");
  स्थिर OT::GSUBGPOS &g = get_gsubgpos_table (face, table_tag);
  स्थिर OT::LangSys &l = g.get_script (script_सूचकांक).get_lang_sys (language_सूचकांक);

  अचिन्हित पूर्णांक अंक_features = l.get_feature_गिनती ();
  क्रम (अचिन्हित पूर्णांक i = 0; i < अंक_features; i++) {
    अचिन्हित पूर्णांक f_सूचकांक = l.get_feature_सूचकांक (i);

    अगर (feature_tag == g.get_feature_tag (f_सूचकांक)) {
      अगर (feature_सूचकांक) *feature_सूचकांक = f_सूचकांक;
      वापस true;
    }
  }

  अगर (feature_सूचकांक) *feature_सूचकांक = HB_OT_LAYOUT_NO_FEATURE_सूचकांक;
  वापस false;
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
अचिन्हित पूर्णांक
hb_ot_layout_feature_get_lookups (hb_face_t    *face,
				  hb_tag_t      table_tag,
				  अचिन्हित पूर्णांक  feature_सूचकांक,
				  अचिन्हित पूर्णांक  start_offset,
				  अचिन्हित पूर्णांक *lookup_गिनती   /* IN/OUT */,
				  अचिन्हित पूर्णांक *lookup_indexes /* OUT */)
{
  वापस hb_ot_layout_feature_with_variations_get_lookups (face,
							   table_tag,
							   feature_सूचकांक,
							   HB_OT_LAYOUT_NO_VARIATIONS_सूचकांक,
							   start_offset,
							   lookup_गिनती,
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
अचिन्हित पूर्णांक
hb_ot_layout_table_get_lookup_गिनती (hb_face_t    *face,
				     hb_tag_t      table_tag)
{
  वापस get_gsubgpos_table (face, table_tag).get_lookup_गिनती ();
}


काष्ठा hb_collect_features_context_t
{
  hb_collect_features_context_t (hb_face_t *face,
				 hb_tag_t   table_tag,
				 hb_set_t  *feature_indices_,
				 स्थिर hb_tag_t *features)

    : g (get_gsubgpos_table (face, table_tag)),
      feature_indices (feature_indices_),
      has_feature_filter (false),
      script_गिनती (0),langsys_गिनती (0), feature_सूचकांक_गिनती (0)
  {
    compute_feature_filter (features);
  }

  व्योम compute_feature_filter (स्थिर hb_tag_t *features)
  {
    अगर (features == nullptr)
    {
      has_feature_filter = false;
      वापस;
    }

    has_feature_filter = true;
    hb_set_t features_set;
    क्रम (; *features; features++)
      features_set.add (*features);

    क्रम (अचिन्हित i = 0; i < g.get_feature_गिनती (); i++)
    {
      hb_tag_t tag = g.get_feature_tag (i);
      अगर (features_set.has (tag))
	feature_indices_filter.add(i);
    }
  }

  bool visited (स्थिर OT::Script &s)
  {
    /* We might have Null() object here.  Don't want to involve
     * that in the memoize.  So, detect empty objects and return. */
    अगर (unlikely (!s.has_default_lang_sys () &&
		  !s.get_lang_sys_गिनती ()))
      वापस true;

    अगर (script_गिनती++ > HB_MAX_SCRIPTS)
      वापस true;

    वापस visited (s, visited_script);
  }
  bool visited (स्थिर OT::LangSys &l)
  {
    /* We might have Null() object here.  Don't want to involve
     * that in the memoize.  So, detect empty objects and return. */
    अगर (unlikely (!l.has_required_feature () &&
		  !l.get_feature_गिनती ()))
      वापस true;

    अगर (langsys_गिनती++ > HB_MAX_LANGSYS)
      वापस true;

    वापस visited (l, visited_langsys);
  }

  bool visited_feature_indices (अचिन्हित गिनती)
  {
    feature_सूचकांक_गिनती += गिनती;
    वापस feature_सूचकांक_गिनती > HB_MAX_FEATURE_INDICES;
  }

  निजी:
  ढाँचा <typename T>
  bool visited (स्थिर T &p, hb_set_t &visited_set)
  {
    hb_codepoint_t delta = (hb_codepoint_t) ((uintptr_t) &p - (uintptr_t) &g);
     अगर (visited_set.has (delta))
      वापस true;

    visited_set.add (delta);
    वापस false;
  }

  खुला:
  स्थिर OT::GSUBGPOS &g;
  hb_set_t *feature_indices;
  hb_set_t  feature_indices_filter;
  bool has_feature_filter;

  निजी:
  hb_set_t visited_script;
  hb_set_t visited_langsys;
  अचिन्हित पूर्णांक script_गिनती;
  अचिन्हित पूर्णांक langsys_गिनती;
  अचिन्हित पूर्णांक feature_सूचकांक_गिनती;
};

अटल व्योम
langsys_collect_features (hb_collect_features_context_t *c,
			  स्थिर OT::LangSys  &l)
{
  अगर (c->visited (l)) वापस;

  अगर (!c->has_feature_filter)
  {
    /* All features. */
    अगर (l.has_required_feature () && !c->visited_feature_indices (1))
      c->feature_indices->add (l.get_required_feature_सूचकांक ());

    // TODO(garretrieger): filter out indices >= feature count?
    अगर (!c->visited_feature_indices (l.feature_सूचकांक_2.len))
      l.add_feature_indexes_to (c->feature_indices);
  }
  अन्यथा
  {
    अगर (c->feature_indices_filter.is_empty()) वापस;
    अचिन्हित पूर्णांक अंक_features = l.get_feature_गिनती ();
    क्रम (अचिन्हित पूर्णांक i = 0; i < अंक_features; i++)
    {
      अचिन्हित पूर्णांक feature_सूचकांक = l.get_feature_सूचकांक (i);
      अगर (!c->feature_indices_filter.has (feature_सूचकांक)) जारी;

      c->feature_indices->add (feature_सूचकांक);
      c->feature_indices_filter.del (feature_सूचकांक);
    }
  }
}

अटल व्योम
script_collect_features (hb_collect_features_context_t *c,
			 स्थिर OT::Script   &s,
			 स्थिर hb_tag_t *languages)
{
  अगर (c->visited (s)) वापस;

  अगर (!languages)
  {
    /* All languages. */
    अगर (s.has_default_lang_sys ())
      langsys_collect_features (c,
				s.get_default_lang_sys ());


    अचिन्हित पूर्णांक गिनती = s.get_lang_sys_गिनती ();
    क्रम (अचिन्हित पूर्णांक language_सूचकांक = 0; language_सूचकांक < गिनती; language_सूचकांक++)
      langsys_collect_features (c,
				s.get_lang_sys (language_सूचकांक));
  }
  अन्यथा
  {
    क्रम (; *languages; languages++)
    {
      अचिन्हित पूर्णांक language_सूचकांक;
      अगर (s.find_lang_sys_सूचकांक (*languages, &language_सूचकांक))
	langsys_collect_features (c,
				  s.get_lang_sys (language_सूचकांक));

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
व्योम
hb_ot_layout_collect_features (hb_face_t      *face,
			       hb_tag_t        table_tag,
			       स्थिर hb_tag_t *scripts,
			       स्थिर hb_tag_t *languages,
			       स्थिर hb_tag_t *features,
			       hb_set_t       *feature_indexes /* OUT */)
{
  hb_collect_features_context_t c (face, table_tag, feature_indexes, features);
  अगर (!scripts)
  {
    /* All scripts. */
    अचिन्हित पूर्णांक गिनती = c.g.get_script_गिनती ();
    क्रम (अचिन्हित पूर्णांक script_सूचकांक = 0; script_सूचकांक < गिनती; script_सूचकांक++)
      script_collect_features (&c,
			       c.g.get_script (script_सूचकांक),
			       languages);
  }
  अन्यथा
  {
    क्रम (; *scripts; scripts++)
    {
      अचिन्हित पूर्णांक script_सूचकांक;
      अगर (c.g.find_script_सूचकांक (*scripts, &script_सूचकांक))
	script_collect_features (&c,
				 c.g.get_script (script_सूचकांक),
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
व्योम
hb_ot_layout_collect_features_map (hb_face_t      *face,
				   hb_tag_t        table_tag,
				   अचिन्हित        script_सूचकांक,
				   अचिन्हित        language_सूचकांक,
				   hb_map_t       *feature_map /* OUT */)
{
  स्थिर OT::GSUBGPOS &g = get_gsubgpos_table (face, table_tag);
  स्थिर OT::LangSys &l = g.get_script (script_सूचकांक).get_lang_sys (language_सूचकांक);

  अचिन्हित पूर्णांक गिनती = l.get_feature_indexes (0, nullptr, nullptr);
  feature_map->alloc (गिनती);

  /* Loop in reverse, such that earlier entries win. That emulates
   * a linear search, which seems to be what other implementations do.
   * We found that with arialuni_t.ttf, the "ur" language system has
   * duplicate features, and the earlier ones work but not later ones.
   */
  क्रम (अचिन्हित पूर्णांक i = गिनती; i; i--)
  {
    अचिन्हित feature_सूचकांक = 0;
    अचिन्हित feature_गिनती_2 = 1;
    l.get_feature_indexes (i - 1, &feature_गिनती_2, &feature_सूचकांक);
    अगर (!feature_गिनती_2)
      अवरोध;
    hb_tag_t feature_tag = g.get_feature_tag (feature_सूचकांक);
    feature_map->set (feature_tag, feature_सूचकांक);
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
व्योम
hb_ot_layout_collect_lookups (hb_face_t      *face,
			      hb_tag_t        table_tag,
			      स्थिर hb_tag_t *scripts,
			      स्थिर hb_tag_t *languages,
			      स्थिर hb_tag_t *features,
			      hb_set_t       *lookup_indexes /* OUT */)
{
  स्थिर OT::GSUBGPOS &g = get_gsubgpos_table (face, table_tag);

  hb_set_t feature_indexes;
  hb_ot_layout_collect_features (face, table_tag, scripts, languages, features, &feature_indexes);

  क्रम (स्वतः feature_सूचकांक : feature_indexes)
    g.get_feature (feature_सूचकांक).add_lookup_indexes_to (lookup_indexes);

  g.feature_variation_collect_lookups (&feature_indexes, nullptr, lookup_indexes);
}


#अगर_अघोषित HB_NO_LAYOUT_COLLECT_GLYPHS
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
व्योम
hb_ot_layout_lookup_collect_glyphs (hb_face_t    *face,
				    hb_tag_t      table_tag,
				    अचिन्हित पूर्णांक  lookup_सूचकांक,
				    hb_set_t     *glyphs_before, /* OUT.  May be NULL */
				    hb_set_t     *glyphs_निवेश,  /* OUT.  May be NULL */
				    hb_set_t     *glyphs_after,  /* OUT.  May be NULL */
				    hb_set_t     *glyphs_output  /* OUT.  May be NULL */)
{
  OT::hb_collect_glyphs_context_t c (face,
				     glyphs_before,
				     glyphs_निवेश,
				     glyphs_after,
				     glyphs_output);

  चयन (table_tag)
  {
    हाल HB_OT_TAG_GSUB:
    {
      स्थिर OT::SubstLookup& l = face->table.GSUB->table->get_lookup (lookup_सूचकांक);
      l.collect_glyphs (&c);
      वापस;
    }
    हाल HB_OT_TAG_GPOS:
    {
      स्थिर OT::PosLookup& l = face->table.GPOS->table->get_lookup (lookup_सूचकांक);
      l.collect_glyphs (&c);
      वापस;
    }
  }
}
#पूर्ण_अगर


/* Variations support */

bool
_hb_ot_layout_table_get_feature_variations_state (hb_face_t             *face,
						   hb_tag_t               table_tag,
						   स्थिर पूर्णांक             *coords,
						   अचिन्हित पूर्णांक           अंक_coords,
						   अचिन्हित पूर्णांक          *variations_सूचकांक,
						   hb_vector_t<अचिन्हित> *lookup_conditions)
{
  स्थिर OT::GSUBGPOS &g = get_gsubgpos_table (face, table_tag);
  स्थिर OT::GDEF &gdef = *face->table.GDEF->table;

  स्वतः instancer = OT::ItemVarStoreInstancer (&gdef.get_var_store (), nullptr,
					      hb_array (coords, अंक_coords));

  वापस g.get_feature_variations_state (coords,
					 अंक_coords,
					 variations_सूचकांक,
					 &instancer,
					 lookup_conditions);
}

bool
_hb_ot_layout_feature_get_lookup_variations (hb_face_t *face,
                                             hb_tag_t table_tag,
                                             अचिन्हित feature_सूचकांक,
                                             अचिन्हित variations_सूचकांक,
                                             hb_sorted_array_t<स्थिर अचिन्हित> conditions,
                                             hb_set_t *lookup_indexes)
{
  वापस get_gsubgpos_table (face, table_tag).get_lookup_variations (
      feature_सूचकांक, variations_सूचकांक, conditions, lookup_indexes);
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
hb_ot_layout_table_find_feature_variations (hb_face_t    *face,
					    hb_tag_t      table_tag,
					    स्थिर पूर्णांक    *coords,
					    अचिन्हित पूर्णांक  अंक_coords,
					    अचिन्हित पूर्णांक *variations_सूचकांक /* out */)
{
  स्थिर OT::GSUBGPOS &g = get_gsubgpos_table (face, table_tag);
  स्थिर OT::GDEF &gdef = *face->table.GDEF->table;

  स्वतः instancer = OT::ItemVarStoreInstancer(&gdef.get_var_store(), nullptr,
					     hb_array (coords, अंक_coords));

  वापस g.find_variations_सूचकांक (coords, अंक_coords, variations_सूचकांक, &instancer);
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
अचिन्हित पूर्णांक
hb_ot_layout_feature_with_variations_get_lookups (hb_face_t    *face,
						  hb_tag_t      table_tag,
						  अचिन्हित पूर्णांक  feature_सूचकांक,
						  अचिन्हित पूर्णांक  variations_सूचकांक,
						  अचिन्हित पूर्णांक  start_offset,
						  अचिन्हित पूर्णांक *lookup_गिनती /* IN/OUT */,
						  अचिन्हित पूर्णांक *lookup_indexes /* OUT */)
{
  static_assert ((OT::FeatureVariations::NOT_FOUND_सूचकांक == HB_OT_LAYOUT_NO_VARIATIONS_सूचकांक), "");
  स्थिर OT::GSUBGPOS &g = get_gsubgpos_table (face, table_tag);

  स्थिर OT::Feature &f = g.get_feature_variation (feature_सूचकांक, variations_सूचकांक);

  वापस f.get_lookup_indexes (start_offset, lookup_गिनती, lookup_indexes);
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
  वापस face->table.GSUB->table->has_तथ्य ();
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
				      अचिन्हित पूर्णांक          lookup_सूचकांक,
				      स्थिर hb_codepoint_t *glyphs,
				      अचिन्हित पूर्णांक          glyphs_लंबाई,
				      hb_bool_t             zero_context)
{
  स्वतः &gsub = face->table.GSUB;
  अगर (unlikely (lookup_सूचकांक >= gsub->lookup_गिनती)) वापस false;
  OT::hb_would_apply_context_t c (face, glyphs, glyphs_लंबाई, (bool) zero_context);

  स्थिर OT::SubstLookup& l = gsub->table->get_lookup (lookup_सूचकांक);
  स्वतः *accel = gsub->get_accel (lookup_सूचकांक);
  वापस accel && l.would_apply (&c, accel);
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
व्योम
hb_ot_layout_substitute_start (hb_font_t    *font,
			       hb_बफर_t  *बफर)
{
  _hb_ot_layout_set_glyph_props<false> (font, बफर);
}

व्योम
hb_ot_layout_substitute_start_with_digest (hb_font_t    *font,
					   hb_बफर_t  *बफर)
{
  बफर->digest = hb_set_digest_t ();
  _hb_ot_layout_set_glyph_props<true> (font, बफर);
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
व्योम
hb_ot_layout_lookup_substitute_closure (hb_face_t    *face,
					अचिन्हित पूर्णांक  lookup_सूचकांक,
					hb_set_t     *glyphs /* OUT */)
{
  hb_map_t done_lookups_glyph_गिनती;
  hb_hashmap_t<अचिन्हित, hb::unique_ptr<hb_set_t>> done_lookups_glyph_set;
  OT::hb_closure_context_t c (face, glyphs, &done_lookups_glyph_गिनती, &done_lookups_glyph_set);

  स्थिर OT::SubstLookup& l = face->table.GSUB->table->get_lookup (lookup_सूचकांक);

  l.closure (&c, lookup_सूचकांक);
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
व्योम
hb_ot_layout_lookups_substitute_closure (hb_face_t      *face,
					 स्थिर hb_set_t *lookups,
					 hb_set_t       *glyphs /* OUT */)
{
  hb_map_t done_lookups_glyph_गिनती;
  hb_hashmap_t<अचिन्हित, hb::unique_ptr<hb_set_t>> done_lookups_glyph_set;
  OT::hb_closure_context_t c (face, glyphs, &done_lookups_glyph_गिनती, &done_lookups_glyph_set);
  स्थिर GSUB& gsub = *face->table.GSUB->table;

  अचिन्हित पूर्णांक iteration_गिनती = 0;
  अचिन्हित पूर्णांक glyphs_लंबाई;
  करो
  {
    c.reset_lookup_visit_गिनती ();
    glyphs_लंबाई = glyphs->get_population ();
    अगर (lookups)
    {
      क्रम (स्वतः lookup_सूचकांक : *lookups)
	gsub.get_lookup (lookup_सूचकांक).closure (&c, lookup_सूचकांक);
    }
    अन्यथा
    {
      क्रम (अचिन्हित पूर्णांक i = 0; i < gsub.get_lookup_गिनती (); i++)
	gsub.get_lookup (i).closure (&c, i);
    }
  } जबतक (iteration_गिनती++ <= HB_CLOSURE_MAX_STAGES &&
	   glyphs_लंबाई != glyphs->get_population ());
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
  वापस face->table.GPOS->table->has_तथ्य ();
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
व्योम
hb_ot_layout_position_start (hb_font_t *font, hb_बफर_t *बफर)
{
  GPOS::position_start (font, बफर);
}


/**
 * hb_ot_layout_position_finish_advances:
 * @font: #hb_font_t to use
 * @buffer: #hb_buffer_t buffer to work upon
 *
 * Called after positioning lookups are performed, to finish glyph advances.
 *
 **/
व्योम
hb_ot_layout_position_finish_advances (hb_font_t *font, hb_बफर_t *बफर)
{
  GPOS::position_finish_advances (font, बफर);
}

/**
 * hb_ot_layout_position_finish_offsets:
 * @font: #hb_font_t to use
 * @buffer: #hb_buffer_t buffer to work upon
 *
 * Called after positioning lookups are performed, to finish glyph offsets.
 *
 **/
व्योम
hb_ot_layout_position_finish_offsets (hb_font_t *font, hb_बफर_t *बफर)
{
  GPOS::position_finish_offsets (font, बफर);
}


#अगर_अघोषित HB_NO_LAYOUT_FEATURE_PARAMS
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
hb_ot_layout_get_आकार_params (hb_face_t       *face,
			      अचिन्हित पूर्णांक    *design_आकार_2,       /* OUT.  May be NULL */
			      अचिन्हित पूर्णांक    *subfamily_id,      /* OUT.  May be NULL */
			      hb_ot_name_id_t *subfamily_name_id, /* OUT.  May be NULL */
			      अचिन्हित पूर्णांक    *range_start,       /* OUT.  May be NULL */
			      अचिन्हित पूर्णांक    *range_end          /* OUT.  May be NULL */)
{
  स्थिर GPOS &gpos = *face->table.GPOS->table;
  स्थिर hb_tag_t tag = HB_TAG ('s','i','z','e');

  अचिन्हित पूर्णांक अंक_features = gpos.get_feature_गिनती ();
  क्रम (अचिन्हित पूर्णांक i = 0; i < अंक_features; i++)
  {
    अगर (tag == gpos.get_feature_tag (i))
    {
      स्थिर OT::Feature &f = gpos.get_feature (i);
      स्थिर OT::Feature_Params_आकार &params = f.get_feature_params ().get_आकार_params (tag);

      अगर (params.design_आकार)
      {
	अगर (design_आकार_2) *design_आकार_2 = params.design_आकार;
	अगर (subfamily_id) *subfamily_id = params.subfamilyID;
	अगर (subfamily_name_id) *subfamily_name_id = params.subfamilyNameID;
	अगर (range_start) *range_start = params.rangeStart;
	अगर (range_end) *range_end = params.rangeEnd;

	वापस true;
      }
    }
  }

  अगर (design_आकार_2) *design_आकार_2 = 0;
  अगर (subfamily_id) *subfamily_id = 0;
  अगर (subfamily_name_id) *subfamily_name_id = HB_OT_NAME_ID_INVALID;
  अगर (range_start) *range_start = 0;
  अगर (range_end) *range_end = 0;

  वापस false;
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
				   hb_tag_t         table_tag,
				   अचिन्हित पूर्णांक     feature_सूचकांक,
				   hb_ot_name_id_t *label_id,             /* OUT.  May be NULL */
				   hb_ot_name_id_t *tooltip_id,           /* OUT.  May be NULL */
				   hb_ot_name_id_t *sample_id,            /* OUT.  May be NULL */
				   अचिन्हित पूर्णांक    *अंक_named_parameters, /* OUT.  May be NULL */
				   hb_ot_name_id_t *first_param_id        /* OUT.  May be NULL */)
{
  स्थिर OT::GSUBGPOS &g = get_gsubgpos_table (face, table_tag);

  hb_tag_t feature_tag = g.get_feature_tag (feature_सूचकांक);
  स्थिर OT::Feature &f = g.get_feature (feature_सूचकांक);

  स्थिर OT::FeatureParams &feature_params = f.get_feature_params ();
  अगर (&feature_params != &Null (OT::FeatureParams))
  {
    स्थिर OT::FeatureParamsStylisticSet& ss_params =
      feature_params.get_stylistic_set_params (feature_tag);
    अगर (&ss_params != &Null (OT::FeatureParamsStylisticSet)) /* ssXX */
    {
      अगर (label_id) *label_id = ss_params.uiNameID;
      // ssXX features don't have the rest
      अगर (tooltip_id) *tooltip_id = HB_OT_NAME_ID_INVALID;
      अगर (sample_id) *sample_id = HB_OT_NAME_ID_INVALID;
      अगर (अंक_named_parameters) *अंक_named_parameters = 0;
      अगर (first_param_id) *first_param_id = HB_OT_NAME_ID_INVALID;
      वापस true;
    }
    स्थिर OT::FeatureParamsCharacterVariants& cv_params =
      feature_params.get_character_variants_params (feature_tag);
    अगर (&cv_params != &Null (OT::FeatureParamsCharacterVariants)) /* cvXX */
    {
      अगर (label_id) *label_id = cv_params.featUILableNameID;
      अगर (tooltip_id) *tooltip_id = cv_params.featUITooltipTextNameID;
      अगर (sample_id) *sample_id = cv_params.sampleTextNameID;
      अगर (अंक_named_parameters) *अंक_named_parameters = cv_params.अंक_Named_Parameters;
      अगर (first_param_id) *first_param_id = cv_params.firstParamUILabelNameID;
      वापस true;
    }
  }

  अगर (label_id) *label_id = HB_OT_NAME_ID_INVALID;
  अगर (tooltip_id) *tooltip_id = HB_OT_NAME_ID_INVALID;
  अगर (sample_id) *sample_id = HB_OT_NAME_ID_INVALID;
  अगर (अंक_named_parameters) *अंक_named_parameters = 0;
  अगर (first_param_id) *first_param_id = HB_OT_NAME_ID_INVALID;
  वापस false;
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
अचिन्हित पूर्णांक
hb_ot_layout_feature_get_characters (hb_face_t      *face,
				     hb_tag_t        table_tag,
				     अचिन्हित पूर्णांक    feature_सूचकांक,
				     अचिन्हित पूर्णांक    start_offset,
				     अचिन्हित पूर्णांक   *char_गिनती, /* IN/OUT.  May be NULL */
				     hb_codepoint_t *characters  /* OUT.     May be NULL */)
{
  स्थिर OT::GSUBGPOS &g = get_gsubgpos_table (face, table_tag);
  वापस g.get_feature (feature_सूचकांक)
	  .get_feature_params ()
	  .get_character_variants_params(g.get_feature_tag (feature_सूचकांक))
	  .get_characters (start_offset, char_गिनती, characters);
}
#पूर्ण_अगर


/*
 * Parts of different types are implemented here such that they have direct
 * access to GSUB/GPOS lookups.
 */


काष्ठा GSUBProxy
{
  अटल constexpr अचिन्हित table_सूचकांक = 0u;
  अटल constexpr bool always_inplace = false;
  प्रकार OT::SubstLookup Lookup;

  GSUBProxy (hb_face_t *face) :
    accel (*face->table.GSUB) {}

  स्थिर GSUB::accelerator_t &accel;
};

काष्ठा GPOSProxy
{
  अटल constexpr अचिन्हित table_सूचकांक = 1u;
  अटल constexpr bool always_inplace = true;
  प्रकार OT::PosLookup Lookup;

  GPOSProxy (hb_face_t *face) :
    accel (*face->table.GPOS) {}

  स्थिर GPOS::accelerator_t &accel;
};


अटल अंतरभूत bool
apply_forward (OT::hb_ot_apply_context_t *c,
	       स्थिर OT::hb_ot_layout_lookup_accelerator_t &accel)
{
  bool use_hot_subtable_cache = accel.cache_enter (c);

  bool ret = false;
  hb_बफर_t *बफर = c->बफर;
  जबतक (बफर->successful)
  {
    hb_glyph_info_t *info = बफर->info;
    अचिन्हित j = बफर->idx;
    जबतक (j < बफर->len &&
	   !(accel.digest.may_have (info[j].codepoint) &&
	     (info[j].mask & c->lookup_mask) &&
	     c->check_glyph_property (&info[j], c->lookup_props)))
      j++;
    अगर (unlikely (j > बफर->idx && !बफर->अग्र_glyphs (j - बफर->idx)))
      अवरोध;
    अगर (बफर->idx >= बफर->len)
      अवरोध;

    अगर (accel.apply (c, use_hot_subtable_cache))
      ret = true;
    अन्यथा
      (व्योम) बफर->अग्र_glyph ();
  }

  अगर (use_hot_subtable_cache)
    accel.cache_leave (c);

  वापस ret;
}

अटल अंतरभूत bool
apply_backward (OT::hb_ot_apply_context_t *c,
	       स्थिर OT::hb_ot_layout_lookup_accelerator_t &accel)
{
  bool ret = false;
  hb_बफर_t *बफर = c->बफर;
  करो
  {
    स्वतः &cur = बफर->cur();
    अगर (accel.digest.may_have (cur.codepoint) &&
	(cur.mask & c->lookup_mask) &&
	c->check_glyph_property (&cur, c->lookup_props))
      ret |= accel.apply (c, false);

    /* The reverse lookup doesn't "advance" cursor (for good reason). */
    बफर->idx--;
  }
  जबतक ((पूर्णांक) बफर->idx >= 0);
  वापस ret;
}

ढाँचा <typename Proxy>
अटल अंतरभूत bool
apply_string (OT::hb_ot_apply_context_t *c,
	      स्थिर typename Proxy::Lookup &lookup,
	      स्थिर OT::hb_ot_layout_lookup_accelerator_t &accel)
{
  hb_बफर_t *बफर = c->बफर;

  अगर (unlikely (!बफर->len || !c->lookup_mask))
    वापस false;

  bool ret = false;

  c->set_lookup_props (lookup.get_props ());

  अगर (likely (!lookup.is_reverse ()))
  {
    /* in/out forward substitution/positioning */
    अगर (!Proxy::always_inplace)
      बफर->clear_output ();

    बफर->idx = 0;
    ret = apply_forward (c, accel);

    अगर (!Proxy::always_inplace)
      बफर->sync ();
  }
  अन्यथा
  {
    /* in-place backward substitution/positioning */
    निश्चित (!बफर->have_output);
    बफर->idx = बफर->len - 1;
    ret = apply_backward (c, accel);
  }

  वापस ret;
}

ढाँचा <typename Proxy>
अंतरभूत व्योम hb_ot_map_t::apply (स्थिर Proxy &proxy,
				स्थिर hb_ot_shape_plan_t *plan,
				hb_font_t *font,
				hb_बफर_t *बफर) स्थिर
{
  स्थिर अचिन्हित पूर्णांक table_सूचकांक = proxy.table_सूचकांक;
  अचिन्हित पूर्णांक i = 0;

  स्वतः *font_तथ्य = font->तथ्य.ot.लो ();
  स्वतः *var_store_cache = (OT::hb_scalar_cache_t *) font_तथ्य;

  OT::hb_ot_apply_context_t c (table_सूचकांक, font, बफर, proxy.accel.get_blob (), var_store_cache);
  c.set_recurse_func (Proxy::Lookup::ढाँचा dispatch_recurse_func<OT::hb_ot_apply_context_t>);

  क्रम (अचिन्हित पूर्णांक stage_सूचकांक = 0; stage_सूचकांक < stages[table_सूचकांक].लंबाई; stage_सूचकांक++)
  {
    स्थिर stage_map_t *stage = &stages[table_सूचकांक][stage_सूचकांक];
    क्रम (; i < stage->last_lookup; i++)
    {
      स्वतः &lookup = lookups[table_सूचकांक][i];

      अचिन्हित पूर्णांक lookup_सूचकांक = lookup.सूचकांक_2;

      स्वतः *accel = proxy.accel.get_accel (lookup_सूचकांक);
      अगर (unlikely (!accel)) जारी;

      अगर (बफर->messaging () &&
	  !बफर->message (font, "start lookup %u feature '%c%c%c%c'", lookup_सूचकांक, HB_UNTAG (lookup.feature_tag))) जारी;

      /* Only try applying the lookup if there is any overlap. */
      अगर (accel->digest.may_intersect (बफर->digest) &&
	  ((बफर->flags & HB_बफर_FLAG_PRODUCE_UNSAFE_TO_CONCAT) ||
	   accel->digest_second.may_intersect (बफर->digest)))
      {
	c.set_lookup_सूचकांक (lookup_सूचकांक);
	c.set_lookup_mask (lookup.mask, false);
	c.set_auto_zwj (lookup.auto_zwj, false);
	c.set_auto_zwnj (lookup.auto_zwnj, false);
	c.set_random (lookup.random);
	c.set_per_syllable (lookup.per_syllable, false);
	/* apply_string's set_lookup_props initializes the iterators. */

	apply_string<Proxy> (&c,
			     proxy.accel.table->get_lookup (lookup_सूचकांक),
			     *accel);
      }
      अन्यथा अगर (बफर->messaging ())
	(व्योम) बफर->message (font, "skipped lookup %u feature '%c%c%c%c' because no glyph matches", lookup_सूचकांक, HB_UNTAG (lookup.feature_tag));

      अगर (बफर->messaging ())
	(व्योम) बफर->message (font, "end lookup %u feature '%c%c%c%c'", lookup_सूचकांक, HB_UNTAG (lookup.feature_tag));
    }

    अगर (stage->pause_func)
    {
      अगर (stage->pause_func (plan, font, बफर))
      {
	/* Refresh working buffer digest since buffer changed. */
	बफर->update_digest ();
      }
    }
  }
}

व्योम hb_ot_map_t::substitute (स्थिर hb_ot_shape_plan_t *plan, hb_font_t *font, hb_बफर_t *बफर) स्थिर
{
  GSUBProxy proxy (font->face);
  अक्षर tag[5] = {0};
  hb_tag_to_string (chosen_script[0], tag);
  अगर (बफर->messaging () &&
      !बफर->message (font, "start table GSUB script tag '%s'", tag)) वापस;
  apply (proxy, plan, font, बफर);
  अगर (बफर->messaging ())
    (व्योम) बफर->message (font, "end table GSUB script tag '%s'", tag);
}

व्योम hb_ot_map_t::position (स्थिर hb_ot_shape_plan_t *plan, hb_font_t *font, hb_बफर_t *बफर) स्थिर
{
  GPOSProxy proxy (font->face);
  अक्षर tag[5] = {0};
  hb_tag_to_string (chosen_script[0], tag);
  अगर (बफर->messaging () &&
      !बफर->message (font, "start table GPOS script tag '%s'", tag)) वापस;
  apply (proxy, plan, font, बफर);
  अगर (बफर->messaging ())
    (व्योम) बफर->message (font, "end table GPOS script tag '%s'", tag);
}

व्योम
hb_ot_layout_substitute_lookup (OT::hb_ot_apply_context_t *c,
				स्थिर OT::SubstLookup &lookup,
				स्थिर OT::hb_ot_layout_lookup_accelerator_t &accel)
{
  apply_string<GSUBProxy> (c, lookup, accel);
}

#अगर_अघोषित HB_NO_BASE

अटल व्योम
choose_base_tags (hb_script_t    script,
		  hb_language_t  language,
		  hb_tag_t      *script_tag,
		  hb_tag_t      *language_tag)
{
  hb_tag_t script_tags[HB_OT_MAX_TAGS_PER_SCRIPT];
  अचिन्हित script_गिनती = ARRAY_लंबाई (script_tags);

  hb_tag_t language_tags[HB_OT_MAX_TAGS_PER_LANGUAGE];
  अचिन्हित language_गिनती = ARRAY_लंबाई (language_tags);

  hb_ot_tags_from_script_and_language (script, language,
				       &script_गिनती, script_tags,
				       &language_गिनती, language_tags);

  *script_tag = script_गिनती ? script_tags[script_गिनती - 1] : HB_OT_TAG_DEFAULT_SCRIPT;
  *language_tag = language_गिनती ? language_tags[language_गिनती - 1] : HB_OT_TAG_DEFAULT_LANGUAGE;
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
			       hb_tag_t           script_tag,
			       hb_tag_t           language_tag,
			       hb_font_extents_t *extents)
{
  hb_position_t min = 0, max = 0;
  अगर (font->face->table.BASE->get_min_max (font, direction, script_tag, language_tag, HB_TAG_NONE,
					   &min, &max))
  {
    अगर (extents)
    {
      extents->ascender  = max;
      extents->descender = min;
      extents->पंक्ति_gap  = 0;
    }
    वापस true;
  }

  hb_font_get_extents_for_direction (font, direction, extents);
  वापस false;
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
  hb_tag_t script_tag, language_tag;
  choose_base_tags (script, language, &script_tag, &language_tag);
  वापस hb_ot_layout_get_font_extents (font,
					direction,
					script_tag,
					language_tag,
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
hb_ot_layout_baseline_tag_t
hb_ot_layout_get_horizontal_baseline_tag_for_script (hb_script_t script)
{
  /* Keep in sync with hb_ot_layout_get_baseline_with_fallback */
  चयन ((पूर्णांक) script)
  {
    /* Unicode-1.1 additions */
    हाल HB_SCRIPT_BENGALI:
    हाल HB_SCRIPT_DEVANAGARI:
    हाल HB_SCRIPT_GUJARATI:
    हाल HB_SCRIPT_GURMUKHI:
    /* Unicode-2.0 additions */
    हाल HB_SCRIPT_TIBETAN:
    /* Unicode-4.0 additions */
    हाल HB_SCRIPT_LIMBU:
    /* Unicode-4.1 additions */
    हाल HB_SCRIPT_SYLOTI_NAGRI:
    /* Unicode-5.0 additions */
    हाल HB_SCRIPT_PHAGS_PA:
    /* Unicode-5.2 additions */
    हाल HB_SCRIPT_MEETEI_MAYEK:
    /* Unicode-6.1 additions */
    हाल HB_SCRIPT_SHARADA:
    हाल HB_SCRIPT_TAKRI:
    /* Unicode-7.0 additions */
    हाल HB_SCRIPT_MODI:
    हाल HB_SCRIPT_SIDDHAM:
    हाल HB_SCRIPT_TIRHUTA:
    /* Unicode-9.0 additions */
    हाल HB_SCRIPT_MARCHEN:
    हाल HB_SCRIPT_NEWA:
    /* Unicode-10.0 additions */
    हाल HB_SCRIPT_SOYOMBO:
    हाल HB_SCRIPT_ZANABAZAR_SQUARE:
    /* Unicode-11.0 additions */
    हाल HB_SCRIPT_DOGRA:
    हाल HB_SCRIPT_GUNJALA_GONDI:
    /* Unicode-12.0 additions */
    हाल HB_SCRIPT_NANDINAGARI:
      वापस HB_OT_LAYOUT_BASELINE_TAG_HANGING;

    /* Unicode-1.1 additions */
    हाल HB_SCRIPT_HANGUL:
    हाल HB_SCRIPT_HAN:
    हाल HB_SCRIPT_HIRAGANA:
    हाल HB_SCRIPT_KATAKANA:
    /* Unicode-3.0 additions */
    हाल HB_SCRIPT_BOPOMOFO:
    /* Unicode-9.0 additions */
    हाल HB_SCRIPT_TANGUT:
    /* Unicode-10.0 additions */
    हाल HB_SCRIPT_NUSHU:
    /* Unicode-13.0 additions */
    हाल HB_SCRIPT_KHITAN_SMALL_SCRIPT:
    /* Unicode-18.0 additions */
    हाल HB_SCRIPT_JURCHEN:
    हाल HB_SCRIPT_SEAL:
      वापस HB_OT_LAYOUT_BASELINE_TAG_IDEO_FACE_BOTTOM_OR_बायाँ;

    शेष:
      वापस HB_OT_LAYOUT_BASELINE_TAG_ROMAN;
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
			   hb_ot_layout_baseline_tag_t  baseline_tag,
			   hb_direction_t               direction,
			   hb_tag_t                     script_tag,
			   hb_tag_t                     language_tag,
			   hb_position_t               *coord        /* OUT.  May be NULL. */)
{
  वापस font->face->table.BASE->get_baseline (font, baseline_tag, direction, script_tag, language_tag, coord);
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
			    hb_ot_layout_baseline_tag_t  baseline_tag,
			    hb_direction_t               direction,
			    hb_script_t                  script,
			    hb_language_t                language,
			    hb_position_t               *coord        /* OUT.  May be NULL. */)
{
  hb_tag_t script_tag, language_tag;
  choose_base_tags (script, language, &script_tag, &language_tag);
  वापस hb_ot_layout_get_baseline (font,
				    baseline_tag,
				    direction,
				    script_tag,
				    language_tag,
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
व्योम
hb_ot_layout_get_baseline_with_fallback (hb_font_t                   *font,
					 hb_ot_layout_baseline_tag_t  baseline_tag,
					 hb_direction_t               direction,
					 hb_tag_t                     script_tag,
					 hb_tag_t                     language_tag,
					 hb_position_t               *coord /* OUT */)
{
  अगर (hb_ot_layout_get_baseline (font,
				 baseline_tag,
				 direction,
				 script_tag,
				 language_tag,
				 coord))
    वापस;

  /* Synthesize missing baselines.
   * See https://www.w3.org/TR/css-inline-3/#baseline-synthesis-fonts
   */
  चयन (baseline_tag)
  {
  हाल HB_OT_LAYOUT_BASELINE_TAG_ROMAN:
    *coord = 0; // FIXME origin ?
    अवरोध;

  हाल HB_OT_LAYOUT_BASELINE_TAG_MATH:
    {
      hb_codepoint_t glyph;
      hb_glyph_extents_t extents;
      अगर (HB_DIRECTION_IS_HORIZONTAL (direction) &&
	  (hb_font_get_nominal_glyph (font, 0x2212u, &glyph) ||
	   hb_font_get_nominal_glyph (font, '-', &glyph)) &&
	  hb_font_get_glyph_extents (font, glyph, &extents))
      {
	*coord = hb_saturate_add (extents.y_bearing, extents.height / 2);
      }
      अन्यथा
      {
	hb_position_t x_height = font->y_scale / 2;
#अगर_अघोषित HB_NO_METRICS
	hb_ot_metrics_get_position_with_fallback (font, HB_OT_METRICS_TAG_X_HEIGHT, &x_height);
#पूर्ण_अगर
	*coord = x_height / 2;
      }
    }
    अवरोध;

  हाल HB_OT_LAYOUT_BASELINE_TAG_IDEO_FACE_शीर्ष_OR_दायाँ:
  हाल HB_OT_LAYOUT_BASELINE_TAG_IDEO_FACE_BOTTOM_OR_बायाँ:
    {
      hb_position_t embox_शीर्ष, embox_bottom;

      hb_ot_layout_get_baseline_with_fallback (font,
					       HB_OT_LAYOUT_BASELINE_TAG_IDEO_EMBOX_शीर्ष_OR_दायाँ,
					       direction,
					       script_tag,
					       language_tag,
					       &embox_शीर्ष);
      hb_ot_layout_get_baseline_with_fallback (font,
					       HB_OT_LAYOUT_BASELINE_TAG_IDEO_EMBOX_BOTTOM_OR_बायाँ,
					       direction,
					       script_tag,
					       language_tag,
					       &embox_bottom);

      अगर (baseline_tag == HB_OT_LAYOUT_BASELINE_TAG_IDEO_FACE_शीर्ष_OR_दायाँ)
	*coord = hb_saturate_add (embox_शीर्ष,
				  hb_clamp_to<hb_position_t> (((int64_t) embox_bottom - embox_शीर्ष) / 10));
      अन्यथा
	*coord = hb_saturate_add (embox_bottom,
				  hb_clamp_to<hb_position_t> (((int64_t) embox_शीर्ष - embox_bottom) / 10));
    }
    अवरोध;

  हाल HB_OT_LAYOUT_BASELINE_TAG_IDEO_EMBOX_शीर्ष_OR_दायाँ:
    अगर (hb_ot_layout_get_baseline (font,
				   HB_OT_LAYOUT_BASELINE_TAG_IDEO_EMBOX_BOTTOM_OR_बायाँ,
				   direction,
				   script_tag,
				   language_tag,
				   coord))
      *coord = hb_saturate_add (*coord,
				HB_DIRECTION_IS_HORIZONTAL (direction) ? font->y_scale : font->x_scale);
    अन्यथा
    {
      hb_font_extents_t font_extents;
      hb_font_get_extents_for_direction (font, direction, &font_extents);
      *coord = font_extents.ascender;
    }
    अवरोध;

  हाल HB_OT_LAYOUT_BASELINE_TAG_IDEO_EMBOX_BOTTOM_OR_बायाँ:
    अगर (hb_ot_layout_get_baseline (font,
				   HB_OT_LAYOUT_BASELINE_TAG_IDEO_EMBOX_शीर्ष_OR_दायाँ,
				   direction,
				   script_tag,
				   language_tag,
				   coord))
      *coord = hb_saturate_sub (*coord,
				HB_DIRECTION_IS_HORIZONTAL (direction) ? font->y_scale : font->x_scale);
    अन्यथा
    {
      hb_font_extents_t font_extents;
      hb_font_get_extents_for_direction (font, direction, &font_extents);
      *coord = font_extents.descender;
    }
    अवरोध;

  हाल HB_OT_LAYOUT_BASELINE_TAG_HANGING:
    अगर (HB_DIRECTION_IS_HORIZONTAL (direction))
    {
      hb_codepoint_t ch;
      hb_codepoint_t glyph;
      hb_glyph_extents_t extents;

      /* Keep in sync with hb_ot_layout_get_horizontal_baseline_for_script */
      चयन ((पूर्णांक) script_tag)
      {
      /* Unicode-1.1 additions */
      हाल HB_SCRIPT_BENGALI:          ch = 0x0995u; अवरोध;
      हाल HB_SCRIPT_DEVANAGARI:       ch = 0x0915u; अवरोध;
      हाल HB_SCRIPT_GUJARATI:         ch = 0x0a95u; अवरोध;
      हाल HB_SCRIPT_GURMUKHI:         ch = 0x0a15u; अवरोध;
      /* Unicode-2.0 additions */
      हाल HB_SCRIPT_TIBETAN:          ch = 0x0f40u; अवरोध;
      /* Unicode-4.0 additions */
      हाल HB_SCRIPT_LIMBU:            ch = 0x1901u; अवरोध;
      /* Unicode-4.1 additions */
      हाल HB_SCRIPT_SYLOTI_NAGRI:     ch = 0xa807u; अवरोध;
      /* Unicode-5.0 additions */
      हाल HB_SCRIPT_PHAGS_PA:         ch = 0xa840u; अवरोध;
      /* Unicode-5.2 additions */
      हाल HB_SCRIPT_MEETEI_MAYEK:     ch = 0xabc0u; अवरोध;
      /* Unicode-6.1 additions */
      हाल HB_SCRIPT_SHARADA:          ch = 0x11191u; अवरोध;
      हाल HB_SCRIPT_TAKRI:            ch = 0x1168cu; अवरोध;
      /* Unicode-7.0 additions */
      हाल HB_SCRIPT_MODI:             ch = 0x1160eu;अवरोध;
      हाल HB_SCRIPT_SIDDHAM:          ch = 0x11590u; अवरोध;
      हाल HB_SCRIPT_TIRHUTA:          ch = 0x1148fu; अवरोध;
      /* Unicode-9.0 additions */
      हाल HB_SCRIPT_MARCHEN:          ch = 0x11c72u; अवरोध;
      हाल HB_SCRIPT_NEWA:             ch = 0x1140eu; अवरोध;
      /* Unicode-10.0 additions */
      हाल HB_SCRIPT_SOYOMBO:          ch = 0x11a5cu; अवरोध;
      हाल HB_SCRIPT_ZANABAZAR_SQUARE: ch = 0x11a0bu; अवरोध;
      /* Unicode-11.0 additions */
      हाल HB_SCRIPT_DOGRA:            ch = 0x1180au; अवरोध;
      हाल HB_SCRIPT_GUNJALA_GONDI:    ch = 0x11d6cu; अवरोध;
      /* Unicode-12.0 additions */
      हाल HB_SCRIPT_NANDINAGARI:      ch = 0x119b0u; अवरोध;
      शेष:                         ch = 0;        अवरोध;
      }

      अगर (ch &&
	  hb_font_get_nominal_glyph (font, ch, &glyph) &&
	  hb_font_get_glyph_extents (font, glyph, &extents))
	*coord = extents.y_bearing;
      अन्यथा
	*coord = hb_clamp_to<hb_position_t> ((int64_t) font->y_scale * 6 / 10); // FIXME makes assumptions about origin
    }
    अन्यथा
      *coord = hb_clamp_to<hb_position_t> ((int64_t) font->x_scale * 6 / 10); // FIXME makes assumptions about origin
    अवरोध;

  हाल HB_OT_LAYOUT_BASELINE_TAG_IDEO_EMBOX_CENTRAL:
    {
      hb_position_t शीर्ष_4, bottom;
      hb_ot_layout_get_baseline_with_fallback (font,
					       HB_OT_LAYOUT_BASELINE_TAG_IDEO_EMBOX_शीर्ष_OR_दायाँ,
					       direction,
					       script_tag,
					       language_tag,
					       &शीर्ष_4);
      hb_ot_layout_get_baseline_with_fallback (font,
					       HB_OT_LAYOUT_BASELINE_TAG_IDEO_EMBOX_BOTTOM_OR_बायाँ,
					       direction,
					       script_tag,
					       language_tag,
					       &bottom);
      *coord = hb_clamp_to<hb_position_t> (((int64_t) शीर्ष_4 + bottom) / 2);

    }
    अवरोध;

  हाल HB_OT_LAYOUT_BASELINE_TAG_IDEO_FACE_CENTRAL:
    {
      hb_position_t शीर्ष_4, bottom;
      hb_ot_layout_get_baseline_with_fallback (font,
					       HB_OT_LAYOUT_BASELINE_TAG_IDEO_FACE_शीर्ष_OR_दायाँ,
					       direction,
					       script_tag,
					       language_tag,
					       &शीर्ष_4);
      hb_ot_layout_get_baseline_with_fallback (font,
					       HB_OT_LAYOUT_BASELINE_TAG_IDEO_FACE_BOTTOM_OR_बायाँ,
					       direction,
					       script_tag,
					       language_tag,
					       &bottom);
      *coord = hb_clamp_to<hb_position_t> (((int64_t) शीर्ष_4 + bottom) / 2);

    }
    अवरोध;

  हाल HB_OT_LAYOUT_BASELINE_TAG_MAX_मूल्य:
  शेष:
    *coord = 0;
    अवरोध;
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
व्योम
hb_ot_layout_get_baseline_with_fallback2 (hb_font_t                   *font,
					  hb_ot_layout_baseline_tag_t  baseline_tag,
					  hb_direction_t               direction,
					  hb_script_t                  script,
					  hb_language_t                language,
					  hb_position_t               *coord        /* OUT */)
{
  hb_tag_t script_tag, language_tag;
  choose_base_tags (script, language, &script_tag, &language_tag);
  hb_ot_layout_get_baseline_with_fallback (font,
					   baseline_tag,
					   direction,
					   script_tag,
					   language_tag,
					   coord);
}

#पूर्ण_अगर


#अगर_अघोषित HB_NO_LAYOUT_RARELY_USED
काष्ठा hb_get_glyph_alternates_dispatch_t :
       hb_dispatch_context_t<hb_get_glyph_alternates_dispatch_t, अचिन्हित>
{
  अटल return_t default_return_मूल्य () { वापस 0; }
  bool stop_sublookup_iteration (return_t r) स्थिर { वापस r; }

  निजी:
  ढाँचा <typename T, typename ...Ts> स्वतः
  _dispatch (स्थिर T &obj, hb_priority<1>, Ts&&... ds) HB_AUTO_RETURN
  ( obj.get_glyph_alternates (std::forward<Ts> (ds)...) )
  ढाँचा <typename T, typename ...Ts> स्वतः
  _dispatch (स्थिर T &obj, hb_priority<0>, Ts&&... ds) HB_AUTO_RETURN
  ( default_return_मूल्य () )
  खुला:
  ढाँचा <typename T, typename ...Ts> स्वतः
  dispatch (स्थिर T &obj, Ts&&... ds) HB_AUTO_RETURN
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
HB_EXTERN अचिन्हित
hb_ot_layout_lookup_get_glyph_alternates (hb_face_t      *face,
					  अचिन्हित        lookup_सूचकांक,
					  hb_codepoint_t  glyph,
					  अचिन्हित        start_offset,
					  अचिन्हित       *alternate_गिनती  /* IN/OUT.  May be NULL. */,
					  hb_codepoint_t *alternate_glyphs /* OUT.     May be NULL. */)
{
  hb_get_glyph_alternates_dispatch_t c;
  स्थिर OT::SubstLookup &lookup = face->table.GSUB->table->get_lookup (lookup_सूचकांक);
  स्वतः ret = lookup.dispatch (&c, glyph, start_offset, alternate_गिनती, alternate_glyphs);
  अगर (!ret && alternate_गिनती) *alternate_गिनती = 0;
  वापस ret;
}

काष्ठा hb_collect_glyph_alternates_dispatch_t :
       hb_dispatch_context_t<hb_collect_glyph_alternates_dispatch_t, bool>
{
  अटल return_t default_return_मूल्य () { वापस false; }
  bool stop_sublookup_iteration (return_t r) स्थिर { वापस false; }

  निजी:
  ढाँचा <typename T, typename ...Ts> स्वतः
  _dispatch (स्थिर T &obj, hb_priority<1>, Ts&&... ds) HB_AUTO_RETURN
  ( (obj.collect_glyph_alternates (std::forward<Ts> (ds)...), true) )
  ढाँचा <typename T, typename ...Ts> स्वतः
  _dispatch (स्थिर T &obj, hb_priority<0>, Ts&&... ds) HB_AUTO_RETURN
  ( default_return_मूल्य () )
  खुला:
  ढाँचा <typename T, typename ...Ts> स्वतः
  dispatch (स्थिर T &obj, Ts&&... ds) HB_AUTO_RETURN
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
					      अचिन्हित   lookup_सूचकांक,
					      hb_map_t  *alternate_गिनती /* IN/OUT */,
					      hb_map_t  *alternate_glyphs /* IN/OUT */)
{
  hb_collect_glyph_alternates_dispatch_t c;
  स्थिर OT::SubstLookup &lookup = face->table.GSUB->table->get_lookup (lookup_सूचकांक);
  वापस lookup.dispatch (&c, alternate_गिनती, alternate_glyphs);
}

काष्ठा hb_position_single_dispatch_t :
       hb_dispatch_context_t<hb_position_single_dispatch_t, bool>
{
  अटल return_t default_return_मूल्य () { वापस false; }
  bool stop_sublookup_iteration (return_t r) स्थिर { वापस r; }

  निजी:
  ढाँचा <typename T, typename ...Ts> स्वतः
  _dispatch (स्थिर T &obj, hb_priority<1>, Ts&&... ds) HB_AUTO_RETURN
  ( obj.position_single (std::forward<Ts> (ds)...) )
  ढाँचा <typename T, typename ...Ts> स्वतः
  _dispatch (स्थिर T &obj, hb_priority<0>, Ts&&... ds) HB_AUTO_RETURN
  ( default_return_मूल्य () )
  खुला:
  ढाँचा <typename T, typename ...Ts> स्वतः
  dispatch (स्थिर T &obj, Ts&&... ds) HB_AUTO_RETURN
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
				       अचिन्हित        lookup_सूचकांक,
				       hb_direction_t  direction,
				       hb_codepoint_t  glyph)
{
  स्थिर OT::PosLookup &lookup = font->face->table.GPOS->table->get_lookup (lookup_सूचकांक);
  hb_blob_t *blob = font->face->table.GPOS->get_blob ();
  hb_glyph_position_t pos = {0};
  hb_position_single_dispatch_t c;
  lookup.dispatch (&c, font, blob, direction, glyph, pos);
  hb_position_t ret = 0;
  चयन (direction)
  {
    हाल HB_DIRECTION_LTR:
      ret = pos.x_offset;
      अवरोध;
    हाल HB_DIRECTION_RTL:
      ret = hb_saturate_sub (pos.x_advance, pos.x_offset);
      अवरोध;
    हाल HB_DIRECTION_TTB:
      ret = pos.y_offset;
      अवरोध;
    हाल HB_DIRECTION_BTT:
      ret = hb_saturate_sub (pos.y_advance, pos.y_offset);
      अवरोध;
    हाल HB_DIRECTION_INVALID:
    शेष:
      अवरोध;
  }
  वापस ret;
}
#पूर्ण_अगर


#पूर्ण_अगर
