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

#അന്തര്ഭാവ "hb.hh"

#ifndef HB_NO_OT_LAYOUT

#ifdef HB_NO_OT_TAG
#error "Cannot compile hb-ot-layout.cc with HB_NO_OT_TAG."
#endif

#അന്തര്ഭാവ "hb-open-type.hh"
#അന്തര്ഭാവ "hb-ot-layout.hh"
#അന്തര്ഭാവ "hb-ot-face.hh"
#അന്തര്ഭാവ "hb-ot-map.hh"
#അന്തര്ഭാവ "hb-map.hh"

#അന്തര്ഭാവ "hb-ot-kern-table.hh"
#അന്തര്ഭാവ "hb-ot-layout-gdef-table.hh"
#അന്തര്ഭാവ "hb-ot-layout-gsub-table.hh"
#അന്തര്ഭാവ "hb-ot-layout-gpos-table.hh"
#അന്തര്ഭാവ "hb-ot-layout-base-table.hh"
#അന്തര്ഭാവ "hb-ot-layout-jstf-table.hh" // Just so we compile it; unused otherwise.
#അന്തര്ഭാവ "hb-ot-name-table.hh"
#അന്തര്ഭാവ "hb-ot-os2-table.hh"

#അന്തര്ഭാവ "hb-aat-layout-morx-table.hh"
#അന്തര്ഭാവ "hb-aat-layout-opbd-table.hh" // Just so we compile it; unused otherwise.

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

#ifndef HB_NO_OT_KERN
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
തർക്കം
hb_ot_layout_has_kerning (hb_face_t *face)
{
  തിരികെ face->table.kern->table->has_data ();
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
തർക്കം
hb_ot_layout_has_machine_kerning (hb_face_t *face)
{
  തിരികെ face->table.kern->table->has_state_machine ();
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
തർക്കം
hb_ot_layout_has_cross_kerning (hb_face_t *face)
{
  തിരികെ face->table.kern->table->has_cross_stream ();
}

ശൂന്യം
hb_ot_layout_kern (സ്ഥിരം hb_ot_shape_plan_t *plan,
		   hb_font_t *font,
		   hb_buffer_t  *buffer)
{
  auto &accel = *font->face->table.kern;
  hb_blob_t *blob = accel.get_blob ();

  AAT::hb_aat_apply_context_t c (plan, font, buffer, blob);

  എങ്കിൽ (!buffer->message (font, "start table kern")) തിരികെ;
  c.buffer_glyph_set = accel.scratch.create_buffer_glyph_set ();
  accel.apply (&c);
  accel.scratch.destroy_buffer_glyph_set (c.buffer_glyph_set);
  (ശൂന്യം) buffer->message (font, "end table kern");
}
#endif


/*
 * GDEF
 */

തർക്കം
OT::GDEF::is_blocklisted (hb_blob_t *blob,
			  hb_face_t *face) സ്ഥിരം
{
#ifdef HB_NO_OT_LAYOUT_BLOCKLIST
  തിരികെ അസത്യം;
#endif
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
  തിരഞ്ഞെടുപ്പ് HB_CODEPOINT_ENCODE3(blob->length,
			      face->table.GSUB->table.get_length (),
			      face->table.GPOS->table.get_length ())
  {
    /* sha1sum:c5ee92f0bca4bfb7d06c4d03e8cf9f9cf75d2e8a Windows 7? timesi.ttf */
    അവസ്ഥ HB_CODEPOINT_ENCODE3 (442, 2874, 42038):
    /* sha1sum:37fc8c16a0894ab7b749e35579856c73c840867b Windows 7? timesbi.ttf */
    അവസ്ഥ HB_CODEPOINT_ENCODE3 (430, 2874, 40662):
    /* sha1sum:19fc45110ea6cd3cdd0a5faca256a3797a069a80 Windows 7 timesi.ttf */
    അവസ്ഥ HB_CODEPOINT_ENCODE3 (442, 2874, 39116):
    /* sha1sum:6d2d3c9ed5b7de87bc84eae0df95ee5232ecde26 Windows 7 timesbi.ttf */
    അവസ്ഥ HB_CODEPOINT_ENCODE3 (430, 2874, 39374):
    /* sha1sum:8583225a8b49667c077b3525333f84af08c6bcd8 OS X 10.11.3 Times New Roman Italic.ttf */
    അവസ്ഥ HB_CODEPOINT_ENCODE3 (490, 3046, 41638):
    /* sha1sum:ec0f5a8751845355b7c3271d11f9918a966cb8c9 OS X 10.11.3 Times New Roman Bold Italic.ttf */
    അവസ്ഥ HB_CODEPOINT_ENCODE3 (478, 3046, 41902):
    /* sha1sum:96eda93f7d33e79962451c6c39a6b51ee893ce8c  tahoma.ttf from Windows 8 */
    അവസ്ഥ HB_CODEPOINT_ENCODE3 (898, 12554, 46470):
    /* sha1sum:20928dc06014e0cd120b6fc942d0c3b1a46ac2bc  tahomabd.ttf from Windows 8 */
    അവസ്ഥ HB_CODEPOINT_ENCODE3 (910, 12566, 47732):
    /* sha1sum:4f95b7e4878f60fa3a39ca269618dfde9721a79e  tahoma.ttf from Windows 8.1 */
    അവസ്ഥ HB_CODEPOINT_ENCODE3 (928, 23298, 59332):
    /* sha1sum:6d400781948517c3c0441ba42acb309584b73033  tahomabd.ttf from Windows 8.1 */
    അവസ്ഥ HB_CODEPOINT_ENCODE3 (940, 23310, 60732):
    /* tahoma.ttf v6.04 from Windows 8.1 x64, see https://bugzilla.mozilla.org/show_bug.cgi?id=1279925 */
    അവസ്ഥ HB_CODEPOINT_ENCODE3 (964, 23836, 60072):
    /* tahomabd.ttf v6.04 from Windows 8.1 x64, see https://bugzilla.mozilla.org/show_bug.cgi?id=1279925 */
    അവസ്ഥ HB_CODEPOINT_ENCODE3 (976, 23832, 61456):
    /* sha1sum:e55fa2dfe957a9f7ec26be516a0e30b0c925f846  tahoma.ttf from Windows 10 */
    അവസ്ഥ HB_CODEPOINT_ENCODE3 (994, 24474, 60336):
    /* sha1sum:7199385abb4c2cc81c83a151a7599b6368e92343  tahomabd.ttf from Windows 10 */
    അവസ്ഥ HB_CODEPOINT_ENCODE3 (1006, 24470, 61740):
    /* tahoma.ttf v6.91 from Windows 10 x64, see https://bugzilla.mozilla.org/show_bug.cgi?id=1279925 */
    അവസ്ഥ HB_CODEPOINT_ENCODE3 (1006, 24576, 61346):
    /* tahomabd.ttf v6.91 from Windows 10 x64, see https://bugzilla.mozilla.org/show_bug.cgi?id=1279925 */
    അവസ്ഥ HB_CODEPOINT_ENCODE3 (1018, 24572, 62828):
    /* sha1sum:b9c84d820c49850d3d27ec498be93955b82772b5  tahoma.ttf from Windows 10 AU */
    അവസ്ഥ HB_CODEPOINT_ENCODE3 (1006, 24576, 61352):
    /* sha1sum:2bdfaab28174bdadd2f3d4200a30a7ae31db79d2  tahomabd.ttf from Windows 10 AU */
    അവസ്ഥ HB_CODEPOINT_ENCODE3 (1018, 24572, 62834):
    /* sha1sum:b0d36cf5a2fbe746a3dd277bffc6756a820807a7  Tahoma.ttf from Mac OS X 10.9 */
    അവസ്ഥ HB_CODEPOINT_ENCODE3 (832, 7324, 47162):
    /* sha1sum:12fc4538e84d461771b30c18b5eb6bd434e30fba  Tahoma Bold.ttf from Mac OS X 10.9 */
    അവസ്ഥ HB_CODEPOINT_ENCODE3 (844, 7302, 45474):
    /* sha1sum:eb8afadd28e9cf963e886b23a30b44ab4fd83acc  himalaya.ttf from Windows 7 */
    അവസ്ഥ HB_CODEPOINT_ENCODE3 (180, 13054, 7254):
    /* sha1sum:73da7f025b238a3f737aa1fde22577a6370f77b0  himalaya.ttf from Windows 8 */
    അവസ്ഥ HB_CODEPOINT_ENCODE3 (192, 12638, 7254):
    /* sha1sum:6e80fd1c0b059bbee49272401583160dc1e6a427  himalaya.ttf from Windows 8.1 */
    അവസ്ഥ HB_CODEPOINT_ENCODE3 (192, 12690, 7254):
    /* 8d9267aea9cd2c852ecfb9f12a6e834bfaeafe44  cantarell-fonts-0.0.21/otf/Cantarell-Regular.otf */
    /* 983988ff7b47439ab79aeaf9a45bd4a2c5b9d371  cantarell-fonts-0.0.21/otf/Cantarell-Oblique.otf */
    അവസ്ഥ HB_CODEPOINT_ENCODE3 (188, 248, 3852):
    /* 2c0c90c6f6087ffbfea76589c93113a9cbb0e75f  cantarell-fonts-0.0.21/otf/Cantarell-Bold.otf */
    /* 55461f5b853c6da88069ffcdf7f4dd3f8d7e3e6b  cantarell-fonts-0.0.21/otf/Cantarell-Bold-Oblique.otf */
    അവസ്ഥ HB_CODEPOINT_ENCODE3 (188, 264, 3426):
    /* d125afa82a77a6475ac0e74e7c207914af84b37a padauk-2.80/Padauk.ttf RHEL 7.2 */
    അവസ്ഥ HB_CODEPOINT_ENCODE3 (1058, 47032, 11818):
    /* 0f7b80437227b90a577cc078c0216160ae61b031 padauk-2.80/Padauk-Bold.ttf RHEL 7.2*/
    അവസ്ഥ HB_CODEPOINT_ENCODE3 (1046, 47030, 12600):
    /* d3dde9aa0a6b7f8f6a89ef1002e9aaa11b882290 padauk-2.80/Padauk.ttf Ubuntu 16.04 */
    അവസ്ഥ HB_CODEPOINT_ENCODE3 (1058, 71796, 16770):
    /* 5f3c98ccccae8a953be2d122c1b3a77fd805093f padauk-2.80/Padauk-Bold.ttf Ubuntu 16.04 */
    അവസ്ഥ HB_CODEPOINT_ENCODE3 (1046, 71790, 17862):
    /* 6c93b63b64e8b2c93f5e824e78caca555dc887c7 padauk-2.80/Padauk-book.ttf */
    അവസ്ഥ HB_CODEPOINT_ENCODE3 (1046, 71788, 17112):
    /* d89b1664058359b8ec82e35d3531931125991fb9 padauk-2.80/Padauk-bookbold.ttf */
    അവസ്ഥ HB_CODEPOINT_ENCODE3 (1058, 71794, 17514):
    /* 824cfd193aaf6234b2b4dc0cf3c6ef576c0d00ef padauk-3.0/Padauk-book.ttf */
    അവസ്ഥ HB_CODEPOINT_ENCODE3 (1330, 109904, 57938):
    /* 91fcc10cf15e012d27571e075b3b4dfe31754a8a padauk-3.0/Padauk-bookbold.ttf */
    അവസ്ഥ HB_CODEPOINT_ENCODE3 (1330, 109904, 58972):
    /* sha1sum: c26e41d567ed821bed997e937bc0c41435689e85  Padauk.ttf
     *  "Padauk Regular" "Version 2.5", see https://crbug.com/681813 */
    അവസ്ഥ HB_CODEPOINT_ENCODE3 (1004, 59092, 14836):
    /* 88d2006ca084f04af2df1954ed714a8c71e8400f  Courier New.ttf from macOS 15 */
    അവസ്ഥ HB_CODEPOINT_ENCODE3 (588, 5078, 14418):
    /* 608e3ebb6dd1aee521cff08eb07d500a2c59df68  Courier New Bold.ttf from macOS 15 */
    അവസ്ഥ HB_CODEPOINT_ENCODE3 (588, 5078, 14238):
    /* d13221044ff054efd78f1cd8631b853c3ce85676  cour.ttf from Windows 10 */
    അവസ്ഥ HB_CODEPOINT_ENCODE3 (894, 17162, 33960):
    /* 68ed4a22d8067fcf1622ac6f6e2f4d3a2e3ec394  courbd.ttf from Windows 10 */
    അവസ്ഥ HB_CODEPOINT_ENCODE3 (894, 17154, 34472):
    /* 4cdb0259c96b7fd7c103821bb8f08f7cc6b211d7  cour.ttf from Windows 8.1 */
    അവസ്ഥ HB_CODEPOINT_ENCODE3 (816, 7868, 17052):
    /* 920483d8a8ed37f7f0afdabbe7f679aece7c75d8  courbd.ttf from Windows 8.1 */
    അവസ്ഥ HB_CODEPOINT_ENCODE3 (816, 7868, 17138):
      തിരികെ സത്യം;
  }
  തിരികെ അസത്യം;
}

template <തർക്കം SetDigest>
സ്ഥിതം ശൂന്യം
_hb_ot_layout_set_glyph_props (hb_font_t *font,
			       hb_buffer_t *buffer)
{
  _hb_buffer_assert_gsubgpos_vars (buffer);

  സ്ഥിരം auto &gdef = *font->face->table.GDEF;
  unsigned പൂർണ്ണസംഖ്യ എണ്ണം = buffer->len;
  hb_glyph_info_t *info = buffer->info;
  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
  {
    എങ്കിൽ (SetDigest)
      buffer->digest.add (info[i].codepoint);
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
  തിരികെ face->table.GDEF->table->has_glyph_classes ();
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
  തിരികെ (hb_ot_layout_glyph_class_t) face->table.GDEF->table->get_glyph_class (glyph);
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
ശൂന്യം
hb_ot_layout_get_glyphs_in_class (hb_face_t                  *face,
				  hb_ot_layout_glyph_class_t  klass,
				  hb_set_t                   *glyphs /* OUT */)
{
  തിരികെ face->table.GDEF->table->get_glyphs_in_class (klass, glyphs);
}

#ifndef HB_NO_LAYOUT_UNUSED
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
unsigned പൂർണ്ണസംഖ്യ
hb_ot_layout_get_attach_points (hb_face_t      *face,
				hb_codepoint_t  glyph,
				unsigned പൂർണ്ണസംഖ്യ    start_offset,
				unsigned പൂർണ്ണസംഖ്യ   *point_എണ്ണം /* IN/OUT */,
				unsigned പൂർണ്ണസംഖ്യ   *point_array /* OUT */)
{
  തിരികെ face->table.GDEF->table->get_attach_points (glyph,
						     start_offset,
						     point_എണ്ണം,
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
unsigned പൂർണ്ണസംഖ്യ
hb_ot_layout_get_ligature_carets (hb_font_t      *font,
				  hb_direction_t  direction,
				  hb_codepoint_t  glyph,
				  unsigned പൂർണ്ണസംഖ്യ    start_offset,
				  unsigned പൂർണ്ണസംഖ്യ   *caret_എണ്ണം /* IN/OUT */,
				  hb_position_t  *caret_array /* OUT */)
{
  തിരികെ font->face->table.GDEF->table->get_lig_carets (font, direction, glyph, start_offset, caret_എണ്ണം, caret_array);
}
#endif


/*
 * GSUB/GPOS
 */

തർക്കം
GSUB::is_blocklisted (hb_blob_t *blob HB_UNUSED,
			  hb_face_t *face) സ്ഥിരം
{
#ifdef HB_NO_OT_LAYOUT_BLOCKLIST
  തിരികെ അസത്യം;
#endif
  തിരികെ അസത്യം;
}

തർക്കം
GPOS::is_blocklisted (hb_blob_t *blob HB_UNUSED,
			  hb_face_t *face HB_UNUSED) സ്ഥിരം
{
#ifdef HB_NO_OT_LAYOUT_BLOCKLIST
  തിരികെ അസത്യം;
#endif
  തിരികെ അസത്യം;
}

സ്ഥിതം സ്ഥിരം OT::GSUBGPOS&
get_gsubgpos_table (hb_face_t *face,
		    hb_tag_t   table_tag)
{
  തിരഞ്ഞെടുപ്പ് (table_tag) {
    അവസ്ഥ HB_OT_TAG_GSUB: തിരികെ *face->table.GSUB->table;
    അവസ്ഥ HB_OT_TAG_GPOS: തിരികെ *face->table.GPOS->table;
    സ്ഥിരസ്ഥിതി:             തിരികെ Null (OT::GSUBGPOS);
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
unsigned പൂർണ്ണസംഖ്യ
hb_ot_layout_table_get_script_tags (hb_face_t    *face,
				    hb_tag_t      table_tag,
				    unsigned പൂർണ്ണസംഖ്യ  start_offset,
				    unsigned പൂർണ്ണസംഖ്യ *script_എണ്ണം /* IN/OUT */,
				    hb_tag_t     *script_tags  /* OUT */)
{
  സ്ഥിരം OT::GSUBGPOS &g = get_gsubgpos_table (face, table_tag);

  തിരികെ g.get_script_tags (start_offset, script_എണ്ണം, script_tags);
}

#define HB_OT_TAG_LATIN_SCRIPT		HB_TAG ('l', 'a', 't', 'n')

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
				unsigned പൂർണ്ണസംഖ്യ *script_index /* OUT */)
{
  static_assert ((OT::Index::NOT_FOUND_INDEX == HB_OT_LAYOUT_NO_SCRIPT_INDEX), "");
  സ്ഥിരം OT::GSUBGPOS &g = get_gsubgpos_table (face, table_tag);

  എങ്കിൽ (g.find_script_index (script_tag, script_index))
    തിരികെ സത്യം;

  /* try finding 'DFLT' */
  എങ്കിൽ (g.find_script_index (HB_OT_TAG_DEFAULT_SCRIPT, script_index))
    തിരികെ അസത്യം;

  /* try with 'dflt'; MS site has had typos and many fonts use it now :(.
   * including many versions of DejaVu Sans Mono! */
  എങ്കിൽ (g.find_script_index (HB_OT_TAG_DEFAULT_LANGUAGE, script_index))
    തിരികെ അസത്യം;

  /* try with 'latn'; some old fonts put their features there even though
     they're really trying to support Thai, for example :( */
  എങ്കിൽ (g.find_script_index (HB_OT_TAG_LATIN_SCRIPT, script_index))
    തിരികെ അസത്യം;

  എങ്കിൽ (script_index) *script_index = HB_OT_LAYOUT_NO_SCRIPT_INDEX;
  തിരികെ അസത്യം;
}

#ifndef HB_DISABLE_DEPRECATED
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
				  സ്ഥിരം hb_tag_t *script_tags,
				  unsigned പൂർണ്ണസംഖ്യ   *script_index  /* OUT */,
				  hb_tag_t       *chosen_script /* OUT */)
{
  സ്ഥിരം hb_tag_t *t;
  വേണ്ടി (t = script_tags; *t; t++);
  തിരികെ hb_ot_layout_table_select_script (face, table_tag, t - script_tags, script_tags, script_index, chosen_script);
}
#endif

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
				  unsigned പൂർണ്ണസംഖ്യ    script_എണ്ണം,
				  സ്ഥിരം hb_tag_t *script_tags,
				  unsigned പൂർണ്ണസംഖ്യ   *script_index  /* OUT */,
				  hb_tag_t       *chosen_script /* OUT */)
{
  static_assert ((OT::Index::NOT_FOUND_INDEX == HB_OT_LAYOUT_NO_SCRIPT_INDEX), "");
  സ്ഥിരം OT::GSUBGPOS &g = get_gsubgpos_table (face, table_tag);
  unsigned പൂർണ്ണസംഖ്യ i;

  വേണ്ടി (i = 0; i < script_എണ്ണം; i++)
  {
    എങ്കിൽ (g.find_script_index (script_tags[i], script_index))
    {
      എങ്കിൽ (chosen_script)
	*chosen_script = script_tags[i];
      തിരികെ സത്യം;
    }
  }

  /* try finding 'DFLT' */
  എങ്കിൽ (g.find_script_index (HB_OT_TAG_DEFAULT_SCRIPT, script_index)) {
    എങ്കിൽ (chosen_script)
      *chosen_script = HB_OT_TAG_DEFAULT_SCRIPT;
    തിരികെ അസത്യം;
  }

  /* try with 'dflt'; MS site has had typos and many fonts use it now :( */
  എങ്കിൽ (g.find_script_index (HB_OT_TAG_DEFAULT_LANGUAGE, script_index)) {
    എങ്കിൽ (chosen_script)
      *chosen_script = HB_OT_TAG_DEFAULT_LANGUAGE;
    തിരികെ അസത്യം;
  }

  /* try with 'latn'; some old fonts put their features there even though
     they're really trying to support Thai, for example :( */
  എങ്കിൽ (g.find_script_index (HB_OT_TAG_LATIN_SCRIPT, script_index)) {
    എങ്കിൽ (chosen_script)
      *chosen_script = HB_OT_TAG_LATIN_SCRIPT;
    തിരികെ അസത്യം;
  }

  എങ്കിൽ (script_index) *script_index = HB_OT_LAYOUT_NO_SCRIPT_INDEX;
  എങ്കിൽ (chosen_script)
    *chosen_script = HB_TAG_NONE;
  തിരികെ അസത്യം;
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
unsigned പൂർണ്ണസംഖ്യ
hb_ot_layout_table_get_feature_tags (hb_face_t    *face,
				     hb_tag_t      table_tag,
				     unsigned പൂർണ്ണസംഖ്യ  start_offset,
				     unsigned പൂർണ്ണസംഖ്യ *feature_എണ്ണം_2 /* IN/OUT */,
				     hb_tag_t     *feature_tags  /* OUT */)
{
  സ്ഥിരം OT::GSUBGPOS &g = get_gsubgpos_table (face, table_tag);

  തിരികെ g.get_feature_tags (start_offset, feature_എണ്ണം_2, feature_tags);
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
തർക്കം
hb_ot_layout_table_find_feature (hb_face_t    *face,
				 hb_tag_t      table_tag,
				 hb_tag_t      feature_tag,
				 unsigned പൂർണ്ണസംഖ്യ *feature_index /* OUT */)
{
  static_assert ((OT::Index::NOT_FOUND_INDEX == HB_OT_LAYOUT_NO_FEATURE_INDEX), "");
  സ്ഥിരം OT::GSUBGPOS &g = get_gsubgpos_table (face, table_tag);

  unsigned പൂർണ്ണസംഖ്യ അക്കം_features = g.get_feature_എണ്ണം ();
  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < അക്കം_features; i++)
  {
    എങ്കിൽ (feature_tag == g.get_feature_tag (i)) {
      എങ്കിൽ (feature_index) *feature_index = i;
      തിരികെ സത്യം;
    }
  }

  എങ്കിൽ (feature_index) *feature_index = HB_OT_LAYOUT_NO_FEATURE_INDEX;
  തിരികെ അസത്യം;
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
unsigned പൂർണ്ണസംഖ്യ
hb_ot_layout_script_get_language_tags (hb_face_t    *face,
				       hb_tag_t      table_tag,
				       unsigned പൂർണ്ണസംഖ്യ  script_index,
				       unsigned പൂർണ്ണസംഖ്യ  start_offset,
				       unsigned പൂർണ്ണസംഖ്യ *language_എണ്ണം /* IN/OUT */,
				       hb_tag_t     *language_tags  /* OUT */)
{
  സ്ഥിരം OT::Script &s = get_gsubgpos_table (face, table_tag).get_script (script_index);

  തിരികെ s.get_lang_sys_tags (start_offset, language_എണ്ണം, language_tags);
}


#ifndef HB_DISABLE_DEPRECATED
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
				   unsigned പൂർണ്ണസംഖ്യ  script_index,
				   hb_tag_t      language_tag,
				   unsigned പൂർണ്ണസംഖ്യ *language_index)
{
  തിരികെ hb_ot_layout_script_select_language (face,
					      table_tag,
					      script_index,
					      1,
					      &language_tag,
					      language_index);
}
#endif


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
				     unsigned പൂർണ്ണസംഖ്യ    script_index,
				     unsigned പൂർണ്ണസംഖ്യ    language_എണ്ണം,
				     സ്ഥിരം hb_tag_t *language_tags,
				     unsigned പൂർണ്ണസംഖ്യ   *language_index /* OUT */,
				     hb_tag_t       *chosen_language /* OUT */)
{
  static_assert ((OT::Index::NOT_FOUND_INDEX == HB_OT_LAYOUT_DEFAULT_LANGUAGE_INDEX), "");
  സ്ഥിരം OT::Script &s = get_gsubgpos_table (face, table_tag).get_script (script_index);
  unsigned പൂർണ്ണസംഖ്യ i;

  വേണ്ടി (i = 0; i < language_എണ്ണം; i++)
  {
    എങ്കിൽ (s.find_lang_sys_index (language_tags[i], language_index))
    {
      എങ്കിൽ (chosen_language)
        *chosen_language = language_tags[i];
      തിരികെ സത്യം;
    }
  }

  /* try finding 'dflt' */
  എങ്കിൽ (s.find_lang_sys_index (HB_OT_TAG_DEFAULT_LANGUAGE, language_index))
  {
    എങ്കിൽ (chosen_language)
      *chosen_language = HB_OT_TAG_DEFAULT_LANGUAGE;
    തിരികെ അസത്യം;
  }

  എങ്കിൽ (language_index)
    *language_index = HB_OT_LAYOUT_DEFAULT_LANGUAGE_INDEX;
  എങ്കിൽ (chosen_language)
    *chosen_language = HB_TAG_NONE;
  തിരികെ അസത്യം;
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
				     unsigned പൂർണ്ണസംഖ്യ    script_index,
				     unsigned പൂർണ്ണസംഖ്യ    language_എണ്ണം,
				     സ്ഥിരം hb_tag_t *language_tags,
				     unsigned പൂർണ്ണസംഖ്യ   *language_index /* OUT */)
{
  തിരികെ hb_ot_layout_script_select_language2 (face, table_tag,
					       script_index,
					       language_എണ്ണം, language_tags,
					       language_index, nullptr);
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
hb_ot_layout_language_get_required_feature_index (hb_face_t    *face,
						  hb_tag_t      table_tag,
						  unsigned പൂർണ്ണസംഖ്യ  script_index,
						  unsigned പൂർണ്ണസംഖ്യ  language_index,
						  unsigned പൂർണ്ണസംഖ്യ *feature_index /* OUT */)
{
  തിരികെ hb_ot_layout_language_get_required_feature (face,
						     table_tag,
						     script_index,
						     language_index,
						     feature_index,
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
					    unsigned പൂർണ്ണസംഖ്യ  script_index,
					    unsigned പൂർണ്ണസംഖ്യ  language_index,
					    unsigned പൂർണ്ണസംഖ്യ *feature_index /* OUT */,
					    hb_tag_t     *feature_tag   /* OUT */)
{
  സ്ഥിരം OT::GSUBGPOS &g = get_gsubgpos_table (face, table_tag);
  സ്ഥിരം OT::LangSys &l = g.get_script (script_index).get_lang_sys (language_index);

  unsigned പൂർണ്ണസംഖ്യ index = l.get_required_feature_index ();
  എങ്കിൽ (feature_index) *feature_index = index;
  എങ്കിൽ (feature_tag) *feature_tag = g.get_feature_tag (index);

  തിരികെ l.has_required_feature ();
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
unsigned പൂർണ്ണസംഖ്യ
hb_ot_layout_language_get_feature_indexes (hb_face_t    *face,
					   hb_tag_t      table_tag,
					   unsigned പൂർണ്ണസംഖ്യ  script_index,
					   unsigned പൂർണ്ണസംഖ്യ  language_index,
					   unsigned പൂർണ്ണസംഖ്യ  start_offset,
					   unsigned പൂർണ്ണസംഖ്യ *feature_എണ്ണം_2   /* IN/OUT */,
					   unsigned പൂർണ്ണസംഖ്യ *feature_indexes /* OUT */)
{
  സ്ഥിരം OT::GSUBGPOS &g = get_gsubgpos_table (face, table_tag);
  സ്ഥിരം OT::LangSys &l = g.get_script (script_index).get_lang_sys (language_index);

  തിരികെ l.get_feature_indexes (start_offset, feature_എണ്ണം_2, feature_indexes);
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
unsigned പൂർണ്ണസംഖ്യ
hb_ot_layout_language_get_feature_tags (hb_face_t    *face,
					hb_tag_t      table_tag,
					unsigned പൂർണ്ണസംഖ്യ  script_index,
					unsigned പൂർണ്ണസംഖ്യ  language_index,
					unsigned പൂർണ്ണസംഖ്യ  start_offset,
					unsigned പൂർണ്ണസംഖ്യ *feature_എണ്ണം_2 /* IN/OUT */,
					hb_tag_t     *feature_tags  /* OUT */)
{
  സ്ഥിരം OT::GSUBGPOS &g = get_gsubgpos_table (face, table_tag);
  സ്ഥിരം OT::LangSys &l = g.get_script (script_index).get_lang_sys (language_index);

  static_assert ((വലുപ്പം (unsigned പൂർണ്ണസംഖ്യ) == വലുപ്പം (hb_tag_t)), "");
  unsigned പൂർണ്ണസംഖ്യ ret = l.get_feature_indexes (start_offset, feature_എണ്ണം_2, (unsigned പൂർണ്ണസംഖ്യ *) feature_tags);

  എങ്കിൽ (feature_എണ്ണം_2 && feature_tags) {
    unsigned പൂർണ്ണസംഖ്യ എണ്ണം = *feature_എണ്ണം_2;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
      feature_tags[i] = g.get_feature_tag ((unsigned പൂർണ്ണസംഖ്യ) feature_tags[i]);
  }

  തിരികെ ret;
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
				    unsigned പൂർണ്ണസംഖ്യ  script_index,
				    unsigned പൂർണ്ണസംഖ്യ  language_index,
				    hb_tag_t      feature_tag,
				    unsigned പൂർണ്ണസംഖ്യ *feature_index /* OUT */)
{
  static_assert ((OT::Index::NOT_FOUND_INDEX == HB_OT_LAYOUT_NO_FEATURE_INDEX), "");
  സ്ഥിരം OT::GSUBGPOS &g = get_gsubgpos_table (face, table_tag);
  സ്ഥിരം OT::LangSys &l = g.get_script (script_index).get_lang_sys (language_index);

  unsigned പൂർണ്ണസംഖ്യ അക്കം_features = l.get_feature_എണ്ണം ();
  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < അക്കം_features; i++) {
    unsigned പൂർണ്ണസംഖ്യ f_index = l.get_feature_index (i);

    എങ്കിൽ (feature_tag == g.get_feature_tag (f_index)) {
      എങ്കിൽ (feature_index) *feature_index = f_index;
      തിരികെ സത്യം;
    }
  }

  എങ്കിൽ (feature_index) *feature_index = HB_OT_LAYOUT_NO_FEATURE_INDEX;
  തിരികെ അസത്യം;
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
unsigned പൂർണ്ണസംഖ്യ
hb_ot_layout_feature_get_lookups (hb_face_t    *face,
				  hb_tag_t      table_tag,
				  unsigned പൂർണ്ണസംഖ്യ  feature_index,
				  unsigned പൂർണ്ണസംഖ്യ  start_offset,
				  unsigned പൂർണ്ണസംഖ്യ *lookup_എണ്ണം   /* IN/OUT */,
				  unsigned പൂർണ്ണസംഖ്യ *lookup_indexes /* OUT */)
{
  തിരികെ hb_ot_layout_feature_with_variations_get_lookups (face,
							   table_tag,
							   feature_index,
							   HB_OT_LAYOUT_NO_VARIATIONS_INDEX,
							   start_offset,
							   lookup_എണ്ണം,
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
unsigned പൂർണ്ണസംഖ്യ
hb_ot_layout_table_get_lookup_എണ്ണം (hb_face_t    *face,
				     hb_tag_t      table_tag)
{
  തിരികെ get_gsubgpos_table (face, table_tag).get_lookup_എണ്ണം ();
}


ഘടന hb_collect_features_context_t
{
  hb_collect_features_context_t (hb_face_t *face,
				 hb_tag_t   table_tag,
				 hb_set_t  *feature_indices_,
				 സ്ഥിരം hb_tag_t *features)

    : g (get_gsubgpos_table (face, table_tag)),
      feature_indices (feature_indices_),
      has_feature_filter (അസത്യം),
      script_എണ്ണം (0),langsys_എണ്ണം (0), feature_index_എണ്ണം (0)
  {
    compute_feature_filter (features);
  }

  ശൂന്യം compute_feature_filter (സ്ഥിരം hb_tag_t *features)
  {
    എങ്കിൽ (features == nullptr)
    {
      has_feature_filter = അസത്യം;
      തിരികെ;
    }

    has_feature_filter = സത്യം;
    hb_set_t features_set;
    വേണ്ടി (; *features; features++)
      features_set.add (*features);

    വേണ്ടി (unsigned i = 0; i < g.get_feature_എണ്ണം (); i++)
    {
      hb_tag_t tag = g.get_feature_tag (i);
      എങ്കിൽ (features_set.has (tag))
	feature_indices_filter.add(i);
    }
  }

  തർക്കം visited (സ്ഥിരം OT::Script &s)
  {
    /* We might have Null() object here.  Don't want to involve
     * that in the memoize.  So, detect empty objects and return. */
    എങ്കിൽ (unlikely (!s.has_default_lang_sys () &&
		  !s.get_lang_sys_എണ്ണം ()))
      തിരികെ സത്യം;

    എങ്കിൽ (script_എണ്ണം++ > HB_MAX_SCRIPTS)
      തിരികെ സത്യം;

    തിരികെ visited (s, visited_script);
  }
  തർക്കം visited (സ്ഥിരം OT::LangSys &l)
  {
    /* We might have Null() object here.  Don't want to involve
     * that in the memoize.  So, detect empty objects and return. */
    എങ്കിൽ (unlikely (!l.has_required_feature () &&
		  !l.get_feature_എണ്ണം ()))
      തിരികെ സത്യം;

    എങ്കിൽ (langsys_എണ്ണം++ > HB_MAX_LANGSYS)
      തിരികെ സത്യം;

    തിരികെ visited (l, visited_langsys);
  }

  തർക്കം visited_feature_indices (unsigned എണ്ണം)
  {
    feature_index_എണ്ണം += എണ്ണം;
    തിരികെ feature_index_എണ്ണം > HB_MAX_FEATURE_INDICES;
  }

  സ്വകാര്യ:
  template <typename T>
  തർക്കം visited (സ്ഥിരം T &p, hb_set_t &visited_set)
  {
    hb_codepoint_t delta = (hb_codepoint_t) ((uintptr_t) &p - (uintptr_t) &g);
     എങ്കിൽ (visited_set.has (delta))
      തിരികെ സത്യം;

    visited_set.add (delta);
    തിരികെ അസത്യം;
  }

  പൊതു:
  സ്ഥിരം OT::GSUBGPOS &g;
  hb_set_t *feature_indices;
  hb_set_t  feature_indices_filter;
  തർക്കം has_feature_filter;

  സ്വകാര്യ:
  hb_set_t visited_script;
  hb_set_t visited_langsys;
  unsigned പൂർണ്ണസംഖ്യ script_എണ്ണം;
  unsigned പൂർണ്ണസംഖ്യ langsys_എണ്ണം;
  unsigned പൂർണ്ണസംഖ്യ feature_index_എണ്ണം;
};

സ്ഥിതം ശൂന്യം
langsys_collect_features (hb_collect_features_context_t *c,
			  സ്ഥിരം OT::LangSys  &l)
{
  എങ്കിൽ (c->visited (l)) തിരികെ;

  എങ്കിൽ (!c->has_feature_filter)
  {
    /* All features. */
    എങ്കിൽ (l.has_required_feature () && !c->visited_feature_indices (1))
      c->feature_indices->add (l.get_required_feature_index ());

    // TODO(garretrieger): filter out indices >= feature count?
    എങ്കിൽ (!c->visited_feature_indices (l.featureIndex.len))
      l.add_feature_indexes_to (c->feature_indices);
  }
  അല്ലെങ്കിൽ
  {
    എങ്കിൽ (c->feature_indices_filter.is_empty()) തിരികെ;
    unsigned പൂർണ്ണസംഖ്യ അക്കം_features = l.get_feature_എണ്ണം ();
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < അക്കം_features; i++)
    {
      unsigned പൂർണ്ണസംഖ്യ feature_index = l.get_feature_index (i);
      എങ്കിൽ (!c->feature_indices_filter.has (feature_index)) തുടരുക;

      c->feature_indices->add (feature_index);
      c->feature_indices_filter.del (feature_index);
    }
  }
}

സ്ഥിതം ശൂന്യം
script_collect_features (hb_collect_features_context_t *c,
			 സ്ഥിരം OT::Script   &s,
			 സ്ഥിരം hb_tag_t *languages)
{
  എങ്കിൽ (c->visited (s)) തിരികെ;

  എങ്കിൽ (!languages)
  {
    /* All languages. */
    എങ്കിൽ (s.has_default_lang_sys ())
      langsys_collect_features (c,
				s.get_default_lang_sys ());


    unsigned പൂർണ്ണസംഖ്യ എണ്ണം = s.get_lang_sys_എണ്ണം ();
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ language_index = 0; language_index < എണ്ണം; language_index++)
      langsys_collect_features (c,
				s.get_lang_sys (language_index));
  }
  അല്ലെങ്കിൽ
  {
    വേണ്ടി (; *languages; languages++)
    {
      unsigned പൂർണ്ണസംഖ്യ language_index;
      എങ്കിൽ (s.find_lang_sys_index (*languages, &language_index))
	langsys_collect_features (c,
				  s.get_lang_sys (language_index));

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
ശൂന്യം
hb_ot_layout_collect_features (hb_face_t      *face,
			       hb_tag_t        table_tag,
			       സ്ഥിരം hb_tag_t *scripts,
			       സ്ഥിരം hb_tag_t *languages,
			       സ്ഥിരം hb_tag_t *features,
			       hb_set_t       *feature_indexes /* OUT */)
{
  hb_collect_features_context_t c (face, table_tag, feature_indexes, features);
  എങ്കിൽ (!scripts)
  {
    /* All scripts. */
    unsigned പൂർണ്ണസംഖ്യ എണ്ണം = c.g.get_script_എണ്ണം ();
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ script_index = 0; script_index < എണ്ണം; script_index++)
      script_collect_features (&c,
			       c.g.get_script (script_index),
			       languages);
  }
  അല്ലെങ്കിൽ
  {
    വേണ്ടി (; *scripts; scripts++)
    {
      unsigned പൂർണ്ണസംഖ്യ script_index;
      എങ്കിൽ (c.g.find_script_index (*scripts, &script_index))
	script_collect_features (&c,
				 c.g.get_script (script_index),
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
ശൂന്യം
hb_ot_layout_collect_features_map (hb_face_t      *face,
				   hb_tag_t        table_tag,
				   unsigned        script_index,
				   unsigned        language_index,
				   hb_map_t       *feature_map /* OUT */)
{
  സ്ഥിരം OT::GSUBGPOS &g = get_gsubgpos_table (face, table_tag);
  സ്ഥിരം OT::LangSys &l = g.get_script (script_index).get_lang_sys (language_index);

  unsigned പൂർണ്ണസംഖ്യ എണ്ണം = l.get_feature_indexes (0, nullptr, nullptr);
  feature_map->alloc (എണ്ണം);

  /* Loop in reverse, such that earlier entries win. That emulates
   * a linear search, which seems to be what other implementations do.
   * We found that with arialuni_t.ttf, the "ur" language system has
   * duplicate features, and the earlier ones work but not later ones.
   */
  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = എണ്ണം; i; i--)
  {
    unsigned feature_index = 0;
    unsigned feature_എണ്ണം_2 = 1;
    l.get_feature_indexes (i - 1, &feature_എണ്ണം_2, &feature_index);
    എങ്കിൽ (!feature_എണ്ണം_2)
      നിർത്തുക;
    hb_tag_t feature_tag = g.get_feature_tag (feature_index);
    feature_map->set (feature_tag, feature_index);
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
ശൂന്യം
hb_ot_layout_collect_lookups (hb_face_t      *face,
			      hb_tag_t        table_tag,
			      സ്ഥിരം hb_tag_t *scripts,
			      സ്ഥിരം hb_tag_t *languages,
			      സ്ഥിരം hb_tag_t *features,
			      hb_set_t       *lookup_indexes /* OUT */)
{
  സ്ഥിരം OT::GSUBGPOS &g = get_gsubgpos_table (face, table_tag);

  hb_set_t feature_indexes;
  hb_ot_layout_collect_features (face, table_tag, scripts, languages, features, &feature_indexes);

  വേണ്ടി (auto feature_index : feature_indexes)
    g.get_feature (feature_index).add_lookup_indexes_to (lookup_indexes);

  g.feature_variation_collect_lookups (&feature_indexes, nullptr, lookup_indexes);
}


#ifndef HB_NO_LAYOUT_COLLECT_GLYPHS
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
ശൂന്യം
hb_ot_layout_lookup_collect_glyphs (hb_face_t    *face,
				    hb_tag_t      table_tag,
				    unsigned പൂർണ്ണസംഖ്യ  lookup_index,
				    hb_set_t     *glyphs_before, /* OUT.  May be NULL */
				    hb_set_t     *glyphs_നിവേശം,  /* OUT.  May be NULL */
				    hb_set_t     *glyphs_after,  /* OUT.  May be NULL */
				    hb_set_t     *glyphs_output  /* OUT.  May be NULL */)
{
  OT::hb_collect_glyphs_context_t c (face,
				     glyphs_before,
				     glyphs_നിവേശം,
				     glyphs_after,
				     glyphs_output);

  തിരഞ്ഞെടുപ്പ് (table_tag)
  {
    അവസ്ഥ HB_OT_TAG_GSUB:
    {
      സ്ഥിരം OT::SubstLookup& l = face->table.GSUB->table->get_lookup (lookup_index);
      l.collect_glyphs (&c);
      തിരികെ;
    }
    അവസ്ഥ HB_OT_TAG_GPOS:
    {
      സ്ഥിരം OT::PosLookup& l = face->table.GPOS->table->get_lookup (lookup_index);
      l.collect_glyphs (&c);
      തിരികെ;
    }
  }
}
#endif


/* Variations support */

തർക്കം
_hb_ot_layout_table_get_feature_variations_state (hb_face_t             *face,
						   hb_tag_t               table_tag,
						   സ്ഥിരം പൂർണ്ണസംഖ്യ             *coords,
						   unsigned പൂർണ്ണസംഖ്യ           അക്കം_coords,
						   unsigned പൂർണ്ണസംഖ്യ          *variations_index,
						   hb_vector_t<unsigned> *lookup_conditions)
{
  സ്ഥിരം OT::GSUBGPOS &g = get_gsubgpos_table (face, table_tag);
  സ്ഥിരം OT::GDEF &gdef = *face->table.GDEF->table;

  auto instancer = OT::ItemVarStoreInstancer (&gdef.get_var_store (), nullptr,
					      hb_array (coords, അക്കം_coords));

  തിരികെ g.get_feature_variations_state (coords,
					 അക്കം_coords,
					 variations_index,
					 &instancer,
					 lookup_conditions);
}

തർക്കം
_hb_ot_layout_feature_get_lookup_variations (hb_face_t *face,
                                             hb_tag_t table_tag,
                                             unsigned feature_index,
                                             unsigned variations_index,
                                             hb_sorted_array_t<സ്ഥിരം unsigned> conditions,
                                             hb_set_t *lookup_indexes)
{
  തിരികെ get_gsubgpos_table (face, table_tag).get_lookup_variations (
      feature_index, variations_index, conditions, lookup_indexes);
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
					    സ്ഥിരം പൂർണ്ണസംഖ്യ    *coords,
					    unsigned പൂർണ്ണസംഖ്യ  അക്കം_coords,
					    unsigned പൂർണ്ണസംഖ്യ *variations_index /* out */)
{
  സ്ഥിരം OT::GSUBGPOS &g = get_gsubgpos_table (face, table_tag);
  സ്ഥിരം OT::GDEF &gdef = *face->table.GDEF->table;

  auto instancer = OT::ItemVarStoreInstancer(&gdef.get_var_store(), nullptr,
					     hb_array (coords, അക്കം_coords));

  തിരികെ g.find_variations_index (coords, അക്കം_coords, variations_index, &instancer);
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
unsigned പൂർണ്ണസംഖ്യ
hb_ot_layout_feature_with_variations_get_lookups (hb_face_t    *face,
						  hb_tag_t      table_tag,
						  unsigned പൂർണ്ണസംഖ്യ  feature_index,
						  unsigned പൂർണ്ണസംഖ്യ  variations_index,
						  unsigned പൂർണ്ണസംഖ്യ  start_offset,
						  unsigned പൂർണ്ണസംഖ്യ *lookup_എണ്ണം /* IN/OUT */,
						  unsigned പൂർണ്ണസംഖ്യ *lookup_indexes /* OUT */)
{
  static_assert ((OT::FeatureVariations::NOT_FOUND_INDEX == HB_OT_LAYOUT_NO_VARIATIONS_INDEX), "");
  സ്ഥിരം OT::GSUBGPOS &g = get_gsubgpos_table (face, table_tag);

  സ്ഥിരം OT::Feature &f = g.get_feature_variation (feature_index, variations_index);

  തിരികെ f.get_lookup_indexes (start_offset, lookup_എണ്ണം, lookup_indexes);
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
  തിരികെ face->table.GSUB->table->has_data ();
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
				      unsigned പൂർണ്ണസംഖ്യ          lookup_index,
				      സ്ഥിരം hb_codepoint_t *glyphs,
				      unsigned പൂർണ്ണസംഖ്യ          glyphs_length,
				      hb_bool_t             zero_context)
{
  auto &gsub = face->table.GSUB;
  എങ്കിൽ (unlikely (lookup_index >= gsub->lookup_എണ്ണം)) തിരികെ അസത്യം;
  OT::hb_would_apply_context_t c (face, glyphs, glyphs_length, (തർക്കം) zero_context);

  സ്ഥിരം OT::SubstLookup& l = gsub->table->get_lookup (lookup_index);
  auto *accel = gsub->get_accel (lookup_index);
  തിരികെ accel && l.would_apply (&c, accel);
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
ശൂന്യം
hb_ot_layout_substitute_start (hb_font_t    *font,
			       hb_buffer_t  *buffer)
{
  _hb_ot_layout_set_glyph_props<അസത്യം> (font, buffer);
}

ശൂന്യം
hb_ot_layout_substitute_start_with_digest (hb_font_t    *font,
					   hb_buffer_t  *buffer)
{
  buffer->digest = hb_set_digest_t ();
  _hb_ot_layout_set_glyph_props<സത്യം> (font, buffer);
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
ശൂന്യം
hb_ot_layout_lookup_substitute_closure (hb_face_t    *face,
					unsigned പൂർണ്ണസംഖ്യ  lookup_index,
					hb_set_t     *glyphs /* OUT */)
{
  hb_map_t done_lookups_glyph_എണ്ണം;
  hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> done_lookups_glyph_set;
  OT::hb_closure_context_t c (face, glyphs, &done_lookups_glyph_എണ്ണം, &done_lookups_glyph_set);

  സ്ഥിരം OT::SubstLookup& l = face->table.GSUB->table->get_lookup (lookup_index);

  l.closure (&c, lookup_index);
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
ശൂന്യം
hb_ot_layout_lookups_substitute_closure (hb_face_t      *face,
					 സ്ഥിരം hb_set_t *lookups,
					 hb_set_t       *glyphs /* OUT */)
{
  hb_map_t done_lookups_glyph_എണ്ണം;
  hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> done_lookups_glyph_set;
  OT::hb_closure_context_t c (face, glyphs, &done_lookups_glyph_എണ്ണം, &done_lookups_glyph_set);
  സ്ഥിരം GSUB& gsub = *face->table.GSUB->table;

  unsigned പൂർണ്ണസംഖ്യ iteration_എണ്ണം = 0;
  unsigned പൂർണ്ണസംഖ്യ glyphs_length;
  ചെയ്യുക
  {
    c.reset_lookup_visit_എണ്ണം ();
    glyphs_length = glyphs->get_population ();
    എങ്കിൽ (lookups)
    {
      വേണ്ടി (auto lookup_index : *lookups)
	gsub.get_lookup (lookup_index).closure (&c, lookup_index);
    }
    അല്ലെങ്കിൽ
    {
      വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < gsub.get_lookup_എണ്ണം (); i++)
	gsub.get_lookup (i).closure (&c, i);
    }
  } ആകുമ്പോൾ (iteration_എണ്ണം++ <= HB_CLOSURE_MAX_STAGES &&
	   glyphs_length != glyphs->get_population ());
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
  തിരികെ face->table.GPOS->table->has_data ();
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
ശൂന്യം
hb_ot_layout_position_start (hb_font_t *font, hb_buffer_t *buffer)
{
  GPOS::position_start (font, buffer);
}


/**
 * hb_ot_layout_position_finish_advances:
 * @font: #hb_font_t to use
 * @buffer: #hb_buffer_t buffer to work upon
 *
 * Called after positioning lookups are performed, to finish glyph advances.
 *
 **/
ശൂന്യം
hb_ot_layout_position_finish_advances (hb_font_t *font, hb_buffer_t *buffer)
{
  GPOS::position_finish_advances (font, buffer);
}

/**
 * hb_ot_layout_position_finish_offsets:
 * @font: #hb_font_t to use
 * @buffer: #hb_buffer_t buffer to work upon
 *
 * Called after positioning lookups are performed, to finish glyph offsets.
 *
 **/
ശൂന്യം
hb_ot_layout_position_finish_offsets (hb_font_t *font, hb_buffer_t *buffer)
{
  GPOS::position_finish_offsets (font, buffer);
}


#ifndef HB_NO_LAYOUT_FEATURE_PARAMS
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
hb_ot_layout_get_size_params (hb_face_t       *face,
			      unsigned പൂർണ്ണസംഖ്യ    *design_size,       /* OUT.  May be NULL */
			      unsigned പൂർണ്ണസംഖ്യ    *subfamily_id,      /* OUT.  May be NULL */
			      hb_ot_name_id_t *subfamily_name_id, /* OUT.  May be NULL */
			      unsigned പൂർണ്ണസംഖ്യ    *range_start,       /* OUT.  May be NULL */
			      unsigned പൂർണ്ണസംഖ്യ    *range_end          /* OUT.  May be NULL */)
{
  സ്ഥിരം GPOS &gpos = *face->table.GPOS->table;
  സ്ഥിരം hb_tag_t tag = HB_TAG ('s','i','z','e');

  unsigned പൂർണ്ണസംഖ്യ അക്കം_features = gpos.get_feature_എണ്ണം ();
  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < അക്കം_features; i++)
  {
    എങ്കിൽ (tag == gpos.get_feature_tag (i))
    {
      സ്ഥിരം OT::Feature &f = gpos.get_feature (i);
      സ്ഥിരം OT::FeatureParamsSize &params = f.get_feature_params ().get_size_params (tag);

      എങ്കിൽ (params.designSize)
      {
	എങ്കിൽ (design_size) *design_size = params.designSize;
	എങ്കിൽ (subfamily_id) *subfamily_id = params.subfamilyID;
	എങ്കിൽ (subfamily_name_id) *subfamily_name_id = params.subfamilyNameID;
	എങ്കിൽ (range_start) *range_start = params.rangeStart;
	എങ്കിൽ (range_end) *range_end = params.rangeEnd;

	തിരികെ സത്യം;
      }
    }
  }

  എങ്കിൽ (design_size) *design_size = 0;
  എങ്കിൽ (subfamily_id) *subfamily_id = 0;
  എങ്കിൽ (subfamily_name_id) *subfamily_name_id = HB_OT_NAME_ID_INVALID;
  എങ്കിൽ (range_start) *range_start = 0;
  എങ്കിൽ (range_end) *range_end = 0;

  തിരികെ അസത്യം;
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
				   unsigned പൂർണ്ണസംഖ്യ     feature_index,
				   hb_ot_name_id_t *label_id,             /* OUT.  May be NULL */
				   hb_ot_name_id_t *tooltip_id,           /* OUT.  May be NULL */
				   hb_ot_name_id_t *sample_id,            /* OUT.  May be NULL */
				   unsigned പൂർണ്ണസംഖ്യ    *അക്കം_named_parameters, /* OUT.  May be NULL */
				   hb_ot_name_id_t *first_param_id        /* OUT.  May be NULL */)
{
  സ്ഥിരം OT::GSUBGPOS &g = get_gsubgpos_table (face, table_tag);

  hb_tag_t feature_tag = g.get_feature_tag (feature_index);
  സ്ഥിരം OT::Feature &f = g.get_feature (feature_index);

  സ്ഥിരം OT::FeatureParams &feature_params = f.get_feature_params ();
  എങ്കിൽ (&feature_params != &Null (OT::FeatureParams))
  {
    സ്ഥിരം OT::FeatureParamsStylisticSet& ss_params =
      feature_params.get_stylistic_set_params (feature_tag);
    എങ്കിൽ (&ss_params != &Null (OT::FeatureParamsStylisticSet)) /* ssXX */
    {
      എങ്കിൽ (label_id) *label_id = ss_params.uiNameID;
      // ssXX features don't have the rest
      എങ്കിൽ (tooltip_id) *tooltip_id = HB_OT_NAME_ID_INVALID;
      എങ്കിൽ (sample_id) *sample_id = HB_OT_NAME_ID_INVALID;
      എങ്കിൽ (അക്കം_named_parameters) *അക്കം_named_parameters = 0;
      എങ്കിൽ (first_param_id) *first_param_id = HB_OT_NAME_ID_INVALID;
      തിരികെ സത്യം;
    }
    സ്ഥിരം OT::FeatureParamsCharacterVariants& cv_params =
      feature_params.get_character_variants_params (feature_tag);
    എങ്കിൽ (&cv_params != &Null (OT::FeatureParamsCharacterVariants)) /* cvXX */
    {
      എങ്കിൽ (label_id) *label_id = cv_params.featUILableNameID;
      എങ്കിൽ (tooltip_id) *tooltip_id = cv_params.featUITooltipTextNameID;
      എങ്കിൽ (sample_id) *sample_id = cv_params.sampleTextNameID;
      എങ്കിൽ (അക്കം_named_parameters) *അക്കം_named_parameters = cv_params.അക്കം_Named_Parameters;
      എങ്കിൽ (first_param_id) *first_param_id = cv_params.firstParamUILabelNameID;
      തിരികെ സത്യം;
    }
  }

  എങ്കിൽ (label_id) *label_id = HB_OT_NAME_ID_INVALID;
  എങ്കിൽ (tooltip_id) *tooltip_id = HB_OT_NAME_ID_INVALID;
  എങ്കിൽ (sample_id) *sample_id = HB_OT_NAME_ID_INVALID;
  എങ്കിൽ (അക്കം_named_parameters) *അക്കം_named_parameters = 0;
  എങ്കിൽ (first_param_id) *first_param_id = HB_OT_NAME_ID_INVALID;
  തിരികെ അസത്യം;
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
unsigned പൂർണ്ണസംഖ്യ
hb_ot_layout_feature_get_characters (hb_face_t      *face,
				     hb_tag_t        table_tag,
				     unsigned പൂർണ്ണസംഖ്യ    feature_index,
				     unsigned പൂർണ്ണസംഖ്യ    start_offset,
				     unsigned പൂർണ്ണസംഖ്യ   *char_എണ്ണം, /* IN/OUT.  May be NULL */
				     hb_codepoint_t *characters  /* OUT.     May be NULL */)
{
  സ്ഥിരം OT::GSUBGPOS &g = get_gsubgpos_table (face, table_tag);
  തിരികെ g.get_feature (feature_index)
	  .get_feature_params ()
	  .get_character_variants_params(g.get_feature_tag (feature_index))
	  .get_characters (start_offset, char_എണ്ണം, characters);
}
#endif


/*
 * Parts of different types are implemented here such that they have direct
 * access to GSUB/GPOS lookups.
 */


ഘടന GSUBProxy
{
  സ്ഥിതം constexpr unsigned table_index = 0u;
  സ്ഥിതം constexpr തർക്കം always_inplace = അസത്യം;
  തരനിർവചനം OT::SubstLookup Lookup;

  GSUBProxy (hb_face_t *face) :
    accel (*face->table.GSUB) {}

  സ്ഥിരം GSUB::accelerator_t &accel;
};

ഘടന GPOSProxy
{
  സ്ഥിതം constexpr unsigned table_index = 1u;
  സ്ഥിതം constexpr തർക്കം always_inplace = സത്യം;
  തരനിർവചനം OT::PosLookup Lookup;

  GPOSProxy (hb_face_t *face) :
    accel (*face->table.GPOS) {}

  സ്ഥിരം GPOS::accelerator_t &accel;
};


സ്ഥിതം വരിയിൽ തർക്കം
apply_forward (OT::hb_ot_apply_context_t *c,
	       സ്ഥിരം OT::hb_ot_layout_lookup_accelerator_t &accel)
{
  തർക്കം use_hot_subtable_cache = accel.cache_enter (c);

  തർക്കം ret = അസത്യം;
  hb_buffer_t *buffer = c->buffer;
  ആകുമ്പോൾ (buffer->successful)
  {
    hb_glyph_info_t *info = buffer->info;
    unsigned j = buffer->idx;
    ആകുമ്പോൾ (j < buffer->len &&
	   !(accel.digest.may_have (info[j].codepoint) &&
	     (info[j].mask & c->lookup_mask) &&
	     c->check_glyph_property (&info[j], c->lookup_props)))
      j++;
    എങ്കിൽ (unlikely (j > buffer->idx && !buffer->next_glyphs (j - buffer->idx)))
      നിർത്തുക;
    എങ്കിൽ (buffer->idx >= buffer->len)
      നിർത്തുക;

    എങ്കിൽ (accel.apply (c, use_hot_subtable_cache))
      ret = സത്യം;
    അല്ലെങ്കിൽ
      (ശൂന്യം) buffer->next_glyph ();
  }

  എങ്കിൽ (use_hot_subtable_cache)
    accel.cache_leave (c);

  തിരികെ ret;
}

സ്ഥിതം വരിയിൽ തർക്കം
apply_backward (OT::hb_ot_apply_context_t *c,
	       സ്ഥിരം OT::hb_ot_layout_lookup_accelerator_t &accel)
{
  തർക്കം ret = അസത്യം;
  hb_buffer_t *buffer = c->buffer;
  ചെയ്യുക
  {
    auto &cur = buffer->cur();
    എങ്കിൽ (accel.digest.may_have (cur.codepoint) &&
	(cur.mask & c->lookup_mask) &&
	c->check_glyph_property (&cur, c->lookup_props))
      ret |= accel.apply (c, അസത്യം);

    /* The reverse lookup doesn't "advance" cursor (for good reason). */
    buffer->idx--;
  }
  ആകുമ്പോൾ ((പൂർണ്ണസംഖ്യ) buffer->idx >= 0);
  തിരികെ ret;
}

template <typename Proxy>
സ്ഥിതം വരിയിൽ തർക്കം
apply_string (OT::hb_ot_apply_context_t *c,
	      സ്ഥിരം typename Proxy::Lookup &lookup,
	      സ്ഥിരം OT::hb_ot_layout_lookup_accelerator_t &accel)
{
  hb_buffer_t *buffer = c->buffer;

  എങ്കിൽ (unlikely (!buffer->len || !c->lookup_mask))
    തിരികെ അസത്യം;

  തർക്കം ret = അസത്യം;

  c->set_lookup_props (lookup.get_props ());

  എങ്കിൽ (likely (!lookup.is_reverse ()))
  {
    /* in/out forward substitution/positioning */
    എങ്കിൽ (!Proxy::always_inplace)
      buffer->clear_output ();

    buffer->idx = 0;
    ret = apply_forward (c, accel);

    എങ്കിൽ (!Proxy::always_inplace)
      buffer->സമന്വയം ();
  }
  അല്ലെങ്കിൽ
  {
    /* in-place backward substitution/positioning */
    പ്രതിജ്ഞാ (!buffer->have_output);
    buffer->idx = buffer->len - 1;
    ret = apply_backward (c, accel);
  }

  തിരികെ ret;
}

template <typename Proxy>
വരിയിൽ ശൂന്യം hb_ot_map_t::apply (സ്ഥിരം Proxy &proxy,
				സ്ഥിരം hb_ot_shape_plan_t *plan,
				hb_font_t *font,
				hb_buffer_t *buffer) സ്ഥിരം
{
  സ്ഥിരം unsigned പൂർണ്ണസംഖ്യ table_index = proxy.table_index;
  unsigned പൂർണ്ണസംഖ്യ i = 0;

  auto *font_data = font->data.ot.get ();
  auto *var_store_cache = (OT::hb_scalar_cache_t *) font_data;

  OT::hb_ot_apply_context_t c (table_index, font, buffer, proxy.accel.get_blob (), var_store_cache);
  c.set_recurse_func (Proxy::Lookup::template dispatch_recurse_func<OT::hb_ot_apply_context_t>);

  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ stage_index = 0; stage_index < stages[table_index].length; stage_index++)
  {
    സ്ഥിരം stage_map_t *stage = &stages[table_index][stage_index];
    വേണ്ടി (; i < stage->last_lookup; i++)
    {
      auto &lookup = lookups[table_index][i];

      unsigned പൂർണ്ണസംഖ്യ lookup_index = lookup.index;

      auto *accel = proxy.accel.get_accel (lookup_index);
      എങ്കിൽ (unlikely (!accel)) തുടരുക;

      എങ്കിൽ (buffer->messaging () &&
	  !buffer->message (font, "start lookup %u feature '%c%c%c%c'", lookup_index, HB_UNTAG (lookup.feature_tag))) തുടരുക;

      /* Only try applying the lookup if there is any overlap. */
      എങ്കിൽ (accel->digest.may_intersect (buffer->digest) &&
	  ((buffer->flags & HB_BUFFER_FLAG_PRODUCE_UNSAFE_TO_CONCAT) ||
	   accel->digest_second.may_intersect (buffer->digest)))
      {
	c.set_lookup_index (lookup_index);
	c.set_lookup_mask (lookup.mask, അസത്യം);
	c.set_auto_zwj (lookup.auto_zwj, അസത്യം);
	c.set_auto_zwnj (lookup.auto_zwnj, അസത്യം);
	c.set_random (lookup.random);
	c.set_per_syllable (lookup.per_syllable, അസത്യം);
	/* apply_string's set_lookup_props initializes the iterators. */

	apply_string<Proxy> (&c,
			     proxy.accel.table->get_lookup (lookup_index),
			     *accel);
      }
      അല്ലെങ്കിൽ എങ്കിൽ (buffer->messaging ())
	(ശൂന്യം) buffer->message (font, "skipped lookup %u feature '%c%c%c%c' because no glyph matches", lookup_index, HB_UNTAG (lookup.feature_tag));

      എങ്കിൽ (buffer->messaging ())
	(ശൂന്യം) buffer->message (font, "end lookup %u feature '%c%c%c%c'", lookup_index, HB_UNTAG (lookup.feature_tag));
    }

    എങ്കിൽ (stage->pause_func)
    {
      എങ്കിൽ (stage->pause_func (plan, font, buffer))
      {
	/* Refresh working buffer digest since buffer changed. */
	buffer->update_digest ();
      }
    }
  }
}

ശൂന്യം hb_ot_map_t::substitute (സ്ഥിരം hb_ot_shape_plan_t *plan, hb_font_t *font, hb_buffer_t *buffer) സ്ഥിരം
{
  GSUBProxy proxy (font->face);
  അക്ഷരം tag[5] = {0};
  hb_tag_to_string (chosen_script[0], tag);
  എങ്കിൽ (buffer->messaging () &&
      !buffer->message (font, "start table GSUB script tag '%s'", tag)) തിരികെ;
  apply (proxy, plan, font, buffer);
  എങ്കിൽ (buffer->messaging ())
    (ശൂന്യം) buffer->message (font, "end table GSUB script tag '%s'", tag);
}

ശൂന്യം hb_ot_map_t::position (സ്ഥിരം hb_ot_shape_plan_t *plan, hb_font_t *font, hb_buffer_t *buffer) സ്ഥിരം
{
  GPOSProxy proxy (font->face);
  അക്ഷരം tag[5] = {0};
  hb_tag_to_string (chosen_script[0], tag);
  എങ്കിൽ (buffer->messaging () &&
      !buffer->message (font, "start table GPOS script tag '%s'", tag)) തിരികെ;
  apply (proxy, plan, font, buffer);
  എങ്കിൽ (buffer->messaging ())
    (ശൂന്യം) buffer->message (font, "end table GPOS script tag '%s'", tag);
}

ശൂന്യം
hb_ot_layout_substitute_lookup (OT::hb_ot_apply_context_t *c,
				സ്ഥിരം OT::SubstLookup &lookup,
				സ്ഥിരം OT::hb_ot_layout_lookup_accelerator_t &accel)
{
  apply_string<GSUBProxy> (c, lookup, accel);
}

#ifndef HB_NO_BASE

സ്ഥിതം ശൂന്യം
choose_base_tags (hb_script_t    script,
		  hb_language_t  language,
		  hb_tag_t      *script_tag,
		  hb_tag_t      *language_tag)
{
  hb_tag_t script_tags[HB_OT_MAX_TAGS_PER_SCRIPT];
  unsigned script_എണ്ണം = ARRAY_LENGTH (script_tags);

  hb_tag_t language_tags[HB_OT_MAX_TAGS_PER_LANGUAGE];
  unsigned language_എണ്ണം = ARRAY_LENGTH (language_tags);

  hb_ot_tags_from_script_and_language (script, language,
				       &script_എണ്ണം, script_tags,
				       &language_എണ്ണം, language_tags);

  *script_tag = script_എണ്ണം ? script_tags[script_എണ്ണം - 1] : HB_OT_TAG_DEFAULT_SCRIPT;
  *language_tag = language_എണ്ണം ? language_tags[language_എണ്ണം - 1] : HB_OT_TAG_DEFAULT_LANGUAGE;
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
  എങ്കിൽ (font->face->table.BASE->get_min_max (font, direction, script_tag, language_tag, HB_TAG_NONE,
					   &min, &max))
  {
    എങ്കിൽ (extents)
    {
      extents->ascender  = max;
      extents->descender = min;
      extents->വരി_gap  = 0;
    }
    തിരികെ സത്യം;
  }

  hb_font_get_extents_for_direction (font, direction, extents);
  തിരികെ അസത്യം;
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
  തിരികെ hb_ot_layout_get_font_extents (font,
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
  തിരഞ്ഞെടുപ്പ് ((പൂർണ്ണസംഖ്യ) script)
  {
    /* Unicode-1.1 additions */
    അവസ്ഥ HB_SCRIPT_BENGALI:
    അവസ്ഥ HB_SCRIPT_DEVANAGARI:
    അവസ്ഥ HB_SCRIPT_GUJARATI:
    അവസ്ഥ HB_SCRIPT_GURMUKHI:
    /* Unicode-2.0 additions */
    അവസ്ഥ HB_SCRIPT_TIBETAN:
    /* Unicode-4.0 additions */
    അവസ്ഥ HB_SCRIPT_LIMBU:
    /* Unicode-4.1 additions */
    അവസ്ഥ HB_SCRIPT_SYLOTI_NAGRI:
    /* Unicode-5.0 additions */
    അവസ്ഥ HB_SCRIPT_PHAGS_PA:
    /* Unicode-5.2 additions */
    അവസ്ഥ HB_SCRIPT_MEETEI_MAYEK:
    /* Unicode-6.1 additions */
    അവസ്ഥ HB_SCRIPT_SHARADA:
    അവസ്ഥ HB_SCRIPT_TAKRI:
    /* Unicode-7.0 additions */
    അവസ്ഥ HB_SCRIPT_MODI:
    അവസ്ഥ HB_SCRIPT_SIDDHAM:
    അവസ്ഥ HB_SCRIPT_TIRHUTA:
    /* Unicode-9.0 additions */
    അവസ്ഥ HB_SCRIPT_MARCHEN:
    അവസ്ഥ HB_SCRIPT_NEWA:
    /* Unicode-10.0 additions */
    അവസ്ഥ HB_SCRIPT_SOYOMBO:
    അവസ്ഥ HB_SCRIPT_ZANABAZAR_SQUARE:
    /* Unicode-11.0 additions */
    അവസ്ഥ HB_SCRIPT_DOGRA:
    അവസ്ഥ HB_SCRIPT_GUNJALA_GONDI:
    /* Unicode-12.0 additions */
    അവസ്ഥ HB_SCRIPT_NANDINAGARI:
      തിരികെ HB_OT_LAYOUT_BASELINE_TAG_HANGING;

    /* Unicode-1.1 additions */
    അവസ്ഥ HB_SCRIPT_HANGUL:
    അവസ്ഥ HB_SCRIPT_HAN:
    അവസ്ഥ HB_SCRIPT_HIRAGANA:
    അവസ്ഥ HB_SCRIPT_KATAKANA:
    /* Unicode-3.0 additions */
    അവസ്ഥ HB_SCRIPT_BOPOMOFO:
    /* Unicode-9.0 additions */
    അവസ്ഥ HB_SCRIPT_TANGUT:
    /* Unicode-10.0 additions */
    അവസ്ഥ HB_SCRIPT_NUSHU:
    /* Unicode-13.0 additions */
    അവസ്ഥ HB_SCRIPT_KHITAN_SMALL_SCRIPT:
    /* Unicode-18.0 additions */
    അവസ്ഥ HB_SCRIPT_JURCHEN:
    അവസ്ഥ HB_SCRIPT_SEAL:
      തിരികെ HB_OT_LAYOUT_BASELINE_TAG_IDEO_FACE_BOTTOM_OR_ഇടത്;

    സ്ഥിരസ്ഥിതി:
      തിരികെ HB_OT_LAYOUT_BASELINE_TAG_ROMAN;
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
  തിരികെ font->face->table.BASE->get_baseline (font, baseline_tag, direction, script_tag, language_tag, coord);
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
  തിരികെ hb_ot_layout_get_baseline (font,
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
ശൂന്യം
hb_ot_layout_get_baseline_with_fallback (hb_font_t                   *font,
					 hb_ot_layout_baseline_tag_t  baseline_tag,
					 hb_direction_t               direction,
					 hb_tag_t                     script_tag,
					 hb_tag_t                     language_tag,
					 hb_position_t               *coord /* OUT */)
{
  എങ്കിൽ (hb_ot_layout_get_baseline (font,
				 baseline_tag,
				 direction,
				 script_tag,
				 language_tag,
				 coord))
    തിരികെ;

  /* Synthesize missing baselines.
   * See https://www.w3.org/TR/css-inline-3/#baseline-synthesis-fonts
   */
  തിരഞ്ഞെടുപ്പ് (baseline_tag)
  {
  അവസ്ഥ HB_OT_LAYOUT_BASELINE_TAG_ROMAN:
    *coord = 0; // FIXME origin ?
    നിർത്തുക;

  അവസ്ഥ HB_OT_LAYOUT_BASELINE_TAG_MATH:
    {
      hb_codepoint_t glyph;
      hb_glyph_extents_t extents;
      എങ്കിൽ (HB_DIRECTION_IS_HORIZONTAL (direction) &&
	  (hb_font_get_nominal_glyph (font, 0x2212u, &glyph) ||
	   hb_font_get_nominal_glyph (font, '-', &glyph)) &&
	  hb_font_get_glyph_extents (font, glyph, &extents))
      {
	*coord = hb_saturate_add (extents.y_bearing, extents.height / 2);
      }
      അല്ലെങ്കിൽ
      {
	hb_position_t x_height = font->y_scale / 2;
#ifndef HB_NO_METRICS
	hb_ot_metrics_get_position_with_fallback (font, HB_OT_METRICS_TAG_X_HEIGHT, &x_height);
#endif
	*coord = x_height / 2;
      }
    }
    നിർത്തുക;

  അവസ്ഥ HB_OT_LAYOUT_BASELINE_TAG_IDEO_FACE_മുകൾ_OR_വലത്:
  അവസ്ഥ HB_OT_LAYOUT_BASELINE_TAG_IDEO_FACE_BOTTOM_OR_ഇടത്:
    {
      hb_position_t embox_മുകൾ, embox_bottom;

      hb_ot_layout_get_baseline_with_fallback (font,
					       HB_OT_LAYOUT_BASELINE_TAG_IDEO_EMBOX_മുകൾ_OR_വലത്,
					       direction,
					       script_tag,
					       language_tag,
					       &embox_മുകൾ);
      hb_ot_layout_get_baseline_with_fallback (font,
					       HB_OT_LAYOUT_BASELINE_TAG_IDEO_EMBOX_BOTTOM_OR_ഇടത്,
					       direction,
					       script_tag,
					       language_tag,
					       &embox_bottom);

      എങ്കിൽ (baseline_tag == HB_OT_LAYOUT_BASELINE_TAG_IDEO_FACE_മുകൾ_OR_വലത്)
	*coord = hb_saturate_add (embox_മുകൾ,
				  hb_clamp_to<hb_position_t> (((int64_t) embox_bottom - embox_മുകൾ) / 10));
      അല്ലെങ്കിൽ
	*coord = hb_saturate_add (embox_bottom,
				  hb_clamp_to<hb_position_t> (((int64_t) embox_മുകൾ - embox_bottom) / 10));
    }
    നിർത്തുക;

  അവസ്ഥ HB_OT_LAYOUT_BASELINE_TAG_IDEO_EMBOX_മുകൾ_OR_വലത്:
    എങ്കിൽ (hb_ot_layout_get_baseline (font,
				   HB_OT_LAYOUT_BASELINE_TAG_IDEO_EMBOX_BOTTOM_OR_ഇടത്,
				   direction,
				   script_tag,
				   language_tag,
				   coord))
      *coord = hb_saturate_add (*coord,
				HB_DIRECTION_IS_HORIZONTAL (direction) ? font->y_scale : font->x_scale);
    അല്ലെങ്കിൽ
    {
      hb_font_extents_t font_extents;
      hb_font_get_extents_for_direction (font, direction, &font_extents);
      *coord = font_extents.ascender;
    }
    നിർത്തുക;

  അവസ്ഥ HB_OT_LAYOUT_BASELINE_TAG_IDEO_EMBOX_BOTTOM_OR_ഇടത്:
    എങ്കിൽ (hb_ot_layout_get_baseline (font,
				   HB_OT_LAYOUT_BASELINE_TAG_IDEO_EMBOX_മുകൾ_OR_വലത്,
				   direction,
				   script_tag,
				   language_tag,
				   coord))
      *coord = hb_saturate_sub (*coord,
				HB_DIRECTION_IS_HORIZONTAL (direction) ? font->y_scale : font->x_scale);
    അല്ലെങ്കിൽ
    {
      hb_font_extents_t font_extents;
      hb_font_get_extents_for_direction (font, direction, &font_extents);
      *coord = font_extents.descender;
    }
    നിർത്തുക;

  അവസ്ഥ HB_OT_LAYOUT_BASELINE_TAG_HANGING:
    എങ്കിൽ (HB_DIRECTION_IS_HORIZONTAL (direction))
    {
      hb_codepoint_t ch;
      hb_codepoint_t glyph;
      hb_glyph_extents_t extents;

      /* Keep in sync with hb_ot_layout_get_horizontal_baseline_for_script */
      തിരഞ്ഞെടുപ്പ് ((പൂർണ്ണസംഖ്യ) script_tag)
      {
      /* Unicode-1.1 additions */
      അവസ്ഥ HB_SCRIPT_BENGALI:          ch = 0x0995u; നിർത്തുക;
      അവസ്ഥ HB_SCRIPT_DEVANAGARI:       ch = 0x0915u; നിർത്തുക;
      അവസ്ഥ HB_SCRIPT_GUJARATI:         ch = 0x0a95u; നിർത്തുക;
      അവസ്ഥ HB_SCRIPT_GURMUKHI:         ch = 0x0a15u; നിർത്തുക;
      /* Unicode-2.0 additions */
      അവസ്ഥ HB_SCRIPT_TIBETAN:          ch = 0x0f40u; നിർത്തുക;
      /* Unicode-4.0 additions */
      അവസ്ഥ HB_SCRIPT_LIMBU:            ch = 0x1901u; നിർത്തുക;
      /* Unicode-4.1 additions */
      അവസ്ഥ HB_SCRIPT_SYLOTI_NAGRI:     ch = 0xa807u; നിർത്തുക;
      /* Unicode-5.0 additions */
      അവസ്ഥ HB_SCRIPT_PHAGS_PA:         ch = 0xa840u; നിർത്തുക;
      /* Unicode-5.2 additions */
      അവസ്ഥ HB_SCRIPT_MEETEI_MAYEK:     ch = 0xabc0u; നിർത്തുക;
      /* Unicode-6.1 additions */
      അവസ്ഥ HB_SCRIPT_SHARADA:          ch = 0x11191u; നിർത്തുക;
      അവസ്ഥ HB_SCRIPT_TAKRI:            ch = 0x1168cu; നിർത്തുക;
      /* Unicode-7.0 additions */
      അവസ്ഥ HB_SCRIPT_MODI:             ch = 0x1160eu;നിർത്തുക;
      അവസ്ഥ HB_SCRIPT_SIDDHAM:          ch = 0x11590u; നിർത്തുക;
      അവസ്ഥ HB_SCRIPT_TIRHUTA:          ch = 0x1148fu; നിർത്തുക;
      /* Unicode-9.0 additions */
      അവസ്ഥ HB_SCRIPT_MARCHEN:          ch = 0x11c72u; നിർത്തുക;
      അവസ്ഥ HB_SCRIPT_NEWA:             ch = 0x1140eu; നിർത്തുക;
      /* Unicode-10.0 additions */
      അവസ്ഥ HB_SCRIPT_SOYOMBO:          ch = 0x11a5cu; നിർത്തുക;
      അവസ്ഥ HB_SCRIPT_ZANABAZAR_SQUARE: ch = 0x11a0bu; നിർത്തുക;
      /* Unicode-11.0 additions */
      അവസ്ഥ HB_SCRIPT_DOGRA:            ch = 0x1180au; നിർത്തുക;
      അവസ്ഥ HB_SCRIPT_GUNJALA_GONDI:    ch = 0x11d6cu; നിർത്തുക;
      /* Unicode-12.0 additions */
      അവസ്ഥ HB_SCRIPT_NANDINAGARI:      ch = 0x119b0u; നിർത്തുക;
      സ്ഥിരസ്ഥിതി:                         ch = 0;        നിർത്തുക;
      }

      എങ്കിൽ (ch &&
	  hb_font_get_nominal_glyph (font, ch, &glyph) &&
	  hb_font_get_glyph_extents (font, glyph, &extents))
	*coord = extents.y_bearing;
      അല്ലെങ്കിൽ
	*coord = hb_clamp_to<hb_position_t> ((int64_t) font->y_scale * 6 / 10); // FIXME makes assumptions about origin
    }
    അല്ലെങ്കിൽ
      *coord = hb_clamp_to<hb_position_t> ((int64_t) font->x_scale * 6 / 10); // FIXME makes assumptions about origin
    നിർത്തുക;

  അവസ്ഥ HB_OT_LAYOUT_BASELINE_TAG_IDEO_EMBOX_CENTRAL:
    {
      hb_position_t മുകൾ_2, bottom;
      hb_ot_layout_get_baseline_with_fallback (font,
					       HB_OT_LAYOUT_BASELINE_TAG_IDEO_EMBOX_മുകൾ_OR_വലത്,
					       direction,
					       script_tag,
					       language_tag,
					       &മുകൾ_2);
      hb_ot_layout_get_baseline_with_fallback (font,
					       HB_OT_LAYOUT_BASELINE_TAG_IDEO_EMBOX_BOTTOM_OR_ഇടത്,
					       direction,
					       script_tag,
					       language_tag,
					       &bottom);
      *coord = hb_clamp_to<hb_position_t> (((int64_t) മുകൾ_2 + bottom) / 2);

    }
    നിർത്തുക;

  അവസ്ഥ HB_OT_LAYOUT_BASELINE_TAG_IDEO_FACE_CENTRAL:
    {
      hb_position_t മുകൾ_2, bottom;
      hb_ot_layout_get_baseline_with_fallback (font,
					       HB_OT_LAYOUT_BASELINE_TAG_IDEO_FACE_മുകൾ_OR_വലത്,
					       direction,
					       script_tag,
					       language_tag,
					       &മുകൾ_2);
      hb_ot_layout_get_baseline_with_fallback (font,
					       HB_OT_LAYOUT_BASELINE_TAG_IDEO_FACE_BOTTOM_OR_ഇടത്,
					       direction,
					       script_tag,
					       language_tag,
					       &bottom);
      *coord = hb_clamp_to<hb_position_t> (((int64_t) മുകൾ_2 + bottom) / 2);

    }
    നിർത്തുക;

  അവസ്ഥ HB_OT_LAYOUT_BASELINE_TAG_MAX_മൂല്യം:
  സ്ഥിരസ്ഥിതി:
    *coord = 0;
    നിർത്തുക;
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
ശൂന്യം
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

#endif


#ifndef HB_NO_LAYOUT_RARELY_USED
ഘടന hb_get_glyph_alternates_dispatch_t :
       hb_dispatch_context_t<hb_get_glyph_alternates_dispatch_t, unsigned>
{
  സ്ഥിതം return_t default_return_മൂല്യം () { തിരികെ 0; }
  തർക്കം stop_sublookup_iteration (return_t r) സ്ഥിരം { തിരികെ r; }

  സ്വകാര്യ:
  template <typename T, typename ...Ts> auto
  _dispatch (സ്ഥിരം T &obj, hb_priority<1>, Ts&&... ds) HB_AUTO_RETURN
  ( obj.get_glyph_alternates (std::forward<Ts> (ds)...) )
  template <typename T, typename ...Ts> auto
  _dispatch (സ്ഥിരം T &obj, hb_priority<0>, Ts&&... ds) HB_AUTO_RETURN
  ( default_return_മൂല്യം () )
  പൊതു:
  template <typename T, typename ...Ts> auto
  dispatch (സ്ഥിരം T &obj, Ts&&... ds) HB_AUTO_RETURN
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
HB_EXTERN unsigned
hb_ot_layout_lookup_get_glyph_alternates (hb_face_t      *face,
					  unsigned        lookup_index,
					  hb_codepoint_t  glyph,
					  unsigned        start_offset,
					  unsigned       *alternate_എണ്ണം  /* IN/OUT.  May be NULL. */,
					  hb_codepoint_t *alternate_glyphs /* OUT.     May be NULL. */)
{
  hb_get_glyph_alternates_dispatch_t c;
  സ്ഥിരം OT::SubstLookup &lookup = face->table.GSUB->table->get_lookup (lookup_index);
  auto ret = lookup.dispatch (&c, glyph, start_offset, alternate_എണ്ണം, alternate_glyphs);
  എങ്കിൽ (!ret && alternate_എണ്ണം) *alternate_എണ്ണം = 0;
  തിരികെ ret;
}

ഘടന hb_collect_glyph_alternates_dispatch_t :
       hb_dispatch_context_t<hb_collect_glyph_alternates_dispatch_t, തർക്കം>
{
  സ്ഥിതം return_t default_return_മൂല്യം () { തിരികെ അസത്യം; }
  തർക്കം stop_sublookup_iteration (return_t r) സ്ഥിരം { തിരികെ അസത്യം; }

  സ്വകാര്യ:
  template <typename T, typename ...Ts> auto
  _dispatch (സ്ഥിരം T &obj, hb_priority<1>, Ts&&... ds) HB_AUTO_RETURN
  ( (obj.collect_glyph_alternates (std::forward<Ts> (ds)...), സത്യം) )
  template <typename T, typename ...Ts> auto
  _dispatch (സ്ഥിരം T &obj, hb_priority<0>, Ts&&... ds) HB_AUTO_RETURN
  ( default_return_മൂല്യം () )
  പൊതു:
  template <typename T, typename ...Ts> auto
  dispatch (സ്ഥിരം T &obj, Ts&&... ds) HB_AUTO_RETURN
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
					      unsigned   lookup_index,
					      hb_map_t  *alternate_എണ്ണം /* IN/OUT */,
					      hb_map_t  *alternate_glyphs /* IN/OUT */)
{
  hb_collect_glyph_alternates_dispatch_t c;
  സ്ഥിരം OT::SubstLookup &lookup = face->table.GSUB->table->get_lookup (lookup_index);
  തിരികെ lookup.dispatch (&c, alternate_എണ്ണം, alternate_glyphs);
}

ഘടന hb_position_single_dispatch_t :
       hb_dispatch_context_t<hb_position_single_dispatch_t, തർക്കം>
{
  സ്ഥിതം return_t default_return_മൂല്യം () { തിരികെ അസത്യം; }
  തർക്കം stop_sublookup_iteration (return_t r) സ്ഥിരം { തിരികെ r; }

  സ്വകാര്യ:
  template <typename T, typename ...Ts> auto
  _dispatch (സ്ഥിരം T &obj, hb_priority<1>, Ts&&... ds) HB_AUTO_RETURN
  ( obj.position_single (std::forward<Ts> (ds)...) )
  template <typename T, typename ...Ts> auto
  _dispatch (സ്ഥിരം T &obj, hb_priority<0>, Ts&&... ds) HB_AUTO_RETURN
  ( default_return_മൂല്യം () )
  പൊതു:
  template <typename T, typename ...Ts> auto
  dispatch (സ്ഥിരം T &obj, Ts&&... ds) HB_AUTO_RETURN
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
				       unsigned        lookup_index,
				       hb_direction_t  direction,
				       hb_codepoint_t  glyph)
{
  സ്ഥിരം OT::PosLookup &lookup = font->face->table.GPOS->table->get_lookup (lookup_index);
  hb_blob_t *blob = font->face->table.GPOS->get_blob ();
  hb_glyph_position_t pos = {0};
  hb_position_single_dispatch_t c;
  lookup.dispatch (&c, font, blob, direction, glyph, pos);
  hb_position_t ret = 0;
  തിരഞ്ഞെടുപ്പ് (direction)
  {
    അവസ്ഥ HB_DIRECTION_LTR:
      ret = pos.x_offset;
      നിർത്തുക;
    അവസ്ഥ HB_DIRECTION_RTL:
      ret = hb_saturate_sub (pos.x_advance, pos.x_offset);
      നിർത്തുക;
    അവസ്ഥ HB_DIRECTION_TTB:
      ret = pos.y_offset;
      നിർത്തുക;
    അവസ്ഥ HB_DIRECTION_BTT:
      ret = hb_saturate_sub (pos.y_advance, pos.y_offset);
      നിർത്തുക;
    അവസ്ഥ HB_DIRECTION_INVALID:
    സ്ഥിരസ്ഥിതി:
      നിർത്തുക;
  }
  തിരികെ ret;
}
#endif


#endif
