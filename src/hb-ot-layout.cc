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

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_LAYOUT

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_ਟੈਗ
#ਗਲਤੀ "Cannot compile hb-ot-layout.cc with HB_NO_OT_TAG."
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-open-type.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-layout.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-face.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-map.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-map.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-kern-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-layout-gdef-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-layout-gsub-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-layout-gpos-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-layout-base-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-layout-jstf-table.hh" // Just so we compile it; unused otherwise.
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-name-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-os2-table.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-aat-layout-morx-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-aat-layout-opbd-table.hh" // Just so we compile it; unused otherwise.

ਵਰਤੋਂ OT::Layout::GSUB;
ਵਰਤੋਂ OT::Layout::GPOS;

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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_KERN
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
ਤਾਰਕਿਕ
hb_ot_layout_has_kerning (hb_face_t *face)
{
  ਵਾਪਸ face->ਸਾਰਣੀ.kern->ਸਾਰਣੀ->has_ਅੰਕੜੇ ();
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
ਤਾਰਕਿਕ
hb_ot_layout_has_machine_kerning (hb_face_t *face)
{
  ਵਾਪਸ face->ਸਾਰਣੀ.kern->ਸਾਰਣੀ->has_state_machine ();
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
ਤਾਰਕਿਕ
hb_ot_layout_has_cross_kerning (hb_face_t *face)
{
  ਵਾਪਸ face->ਸਾਰਣੀ.kern->ਸਾਰਣੀ->has_cross_ਧਾਰਾ ();
}

ਖਾਲੀ
hb_ot_layout_kern (ਅਚਲ hb_ot_shape_plan_t *plan,
		   hb_font_t *font,
		   hb_ਬਫਰ_t  *ਬਫਰ)
{
  ਸਵੈਚਾਲਿਤ &accel = *font->face->ਸਾਰਣੀ.kern;
  hb_blob_t *blob = accel.get_blob ();

  AAT::hb_aat_apply_context_t c (plan, font, ਬਫਰ, blob);

  ਜੇ (!ਬਫਰ->ਸੁਨੇਹਾ (font, "start table kern")) ਵਾਪਸ;
  c.ਬਫਰ_glyph_ਸੈੱਟ = accel.scratch.create_ਬਫਰ_glyph_ਸੈੱਟ ();
  accel.apply (&c);
  accel.scratch.destroy_ਬਫਰ_glyph_ਸੈੱਟ (c.ਬਫਰ_glyph_ਸੈੱਟ);
  (ਖਾਲੀ) ਬਫਰ->ਸੁਨੇਹਾ (font, "end table kern");
}
#ਜੇ_ਅੰਤ


/*
 * GDEF
 */

ਤਾਰਕਿਕ
OT::GDEF::is_blocklisted (hb_blob_t *blob,
			  hb_face_t *face) ਅਚਲ
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_LAYOUT_BLOCKLIST
  ਵਾਪਸ ਝੂਠ;
#ਜੇ_ਅੰਤ
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
  ਚੋਣ HB_CODEPOINT_ENCODE3(blob->ਲੰਮਾਈ,
			      face->ਸਾਰਣੀ.GSUB->ਸਾਰਣੀ.get_ਲੰਮਾਈ (),
			      face->ਸਾਰਣੀ.GPOS->ਸਾਰਣੀ.get_ਲੰਮਾਈ ())
  {
    /* sha1sum:c5ee92f0bca4bfb7d06c4d03e8cf9f9cf75d2e8a Windows 7? timesi.ttf */
    ਹਾਲਤ HB_CODEPOINT_ENCODE3 (442, 2874, 42038):
    /* sha1sum:37fc8c16a0894ab7b749e35579856c73c840867b Windows 7? timesbi.ttf */
    ਹਾਲਤ HB_CODEPOINT_ENCODE3 (430, 2874, 40662):
    /* sha1sum:19fc45110ea6cd3cdd0a5faca256a3797a069a80 Windows 7 timesi.ttf */
    ਹਾਲਤ HB_CODEPOINT_ENCODE3 (442, 2874, 39116):
    /* sha1sum:6d2d3c9ed5b7de87bc84eae0df95ee5232ecde26 Windows 7 timesbi.ttf */
    ਹਾਲਤ HB_CODEPOINT_ENCODE3 (430, 2874, 39374):
    /* sha1sum:8583225a8b49667c077b3525333f84af08c6bcd8 OS X 10.11.3 Times New Roman Italic.ttf */
    ਹਾਲਤ HB_CODEPOINT_ENCODE3 (490, 3046, 41638):
    /* sha1sum:ec0f5a8751845355b7c3271d11f9918a966cb8c9 OS X 10.11.3 Times New Roman Bold Italic.ttf */
    ਹਾਲਤ HB_CODEPOINT_ENCODE3 (478, 3046, 41902):
    /* sha1sum:96eda93f7d33e79962451c6c39a6b51ee893ce8c  tahoma.ttf from Windows 8 */
    ਹਾਲਤ HB_CODEPOINT_ENCODE3 (898, 12554, 46470):
    /* sha1sum:20928dc06014e0cd120b6fc942d0c3b1a46ac2bc  tahomabd.ttf from Windows 8 */
    ਹਾਲਤ HB_CODEPOINT_ENCODE3 (910, 12566, 47732):
    /* sha1sum:4f95b7e4878f60fa3a39ca269618dfde9721a79e  tahoma.ttf from Windows 8.1 */
    ਹਾਲਤ HB_CODEPOINT_ENCODE3 (928, 23298, 59332):
    /* sha1sum:6d400781948517c3c0441ba42acb309584b73033  tahomabd.ttf from Windows 8.1 */
    ਹਾਲਤ HB_CODEPOINT_ENCODE3 (940, 23310, 60732):
    /* tahoma.ttf v6.04 from Windows 8.1 x64, see https://bugzilla.mozilla.org/show_bug.cgi?id=1279925 */
    ਹਾਲਤ HB_CODEPOINT_ENCODE3 (964, 23836, 60072):
    /* tahomabd.ttf v6.04 from Windows 8.1 x64, see https://bugzilla.mozilla.org/show_bug.cgi?id=1279925 */
    ਹਾਲਤ HB_CODEPOINT_ENCODE3 (976, 23832, 61456):
    /* sha1sum:e55fa2dfe957a9f7ec26be516a0e30b0c925f846  tahoma.ttf from Windows 10 */
    ਹਾਲਤ HB_CODEPOINT_ENCODE3 (994, 24474, 60336):
    /* sha1sum:7199385abb4c2cc81c83a151a7599b6368e92343  tahomabd.ttf from Windows 10 */
    ਹਾਲਤ HB_CODEPOINT_ENCODE3 (1006, 24470, 61740):
    /* tahoma.ttf v6.91 from Windows 10 x64, see https://bugzilla.mozilla.org/show_bug.cgi?id=1279925 */
    ਹਾਲਤ HB_CODEPOINT_ENCODE3 (1006, 24576, 61346):
    /* tahomabd.ttf v6.91 from Windows 10 x64, see https://bugzilla.mozilla.org/show_bug.cgi?id=1279925 */
    ਹਾਲਤ HB_CODEPOINT_ENCODE3 (1018, 24572, 62828):
    /* sha1sum:b9c84d820c49850d3d27ec498be93955b82772b5  tahoma.ttf from Windows 10 AU */
    ਹਾਲਤ HB_CODEPOINT_ENCODE3 (1006, 24576, 61352):
    /* sha1sum:2bdfaab28174bdadd2f3d4200a30a7ae31db79d2  tahomabd.ttf from Windows 10 AU */
    ਹਾਲਤ HB_CODEPOINT_ENCODE3 (1018, 24572, 62834):
    /* sha1sum:b0d36cf5a2fbe746a3dd277bffc6756a820807a7  Tahoma.ttf from Mac OS X 10.9 */
    ਹਾਲਤ HB_CODEPOINT_ENCODE3 (832, 7324, 47162):
    /* sha1sum:12fc4538e84d461771b30c18b5eb6bd434e30fba  Tahoma Bold.ttf from Mac OS X 10.9 */
    ਹਾਲਤ HB_CODEPOINT_ENCODE3 (844, 7302, 45474):
    /* sha1sum:eb8afadd28e9cf963e886b23a30b44ab4fd83acc  himalaya.ttf from Windows 7 */
    ਹਾਲਤ HB_CODEPOINT_ENCODE3 (180, 13054, 7254):
    /* sha1sum:73da7f025b238a3f737aa1fde22577a6370f77b0  himalaya.ttf from Windows 8 */
    ਹਾਲਤ HB_CODEPOINT_ENCODE3 (192, 12638, 7254):
    /* sha1sum:6e80fd1c0b059bbee49272401583160dc1e6a427  himalaya.ttf from Windows 8.1 */
    ਹਾਲਤ HB_CODEPOINT_ENCODE3 (192, 12690, 7254):
    /* 8d9267aea9cd2c852ecfb9f12a6e834bfaeafe44  cantarell-fonts-0.0.21/otf/Cantarell-Regular.otf */
    /* 983988ff7b47439ab79aeaf9a45bd4a2c5b9d371  cantarell-fonts-0.0.21/otf/Cantarell-Oblique.otf */
    ਹਾਲਤ HB_CODEPOINT_ENCODE3 (188, 248, 3852):
    /* 2c0c90c6f6087ffbfea76589c93113a9cbb0e75f  cantarell-fonts-0.0.21/otf/Cantarell-Bold.otf */
    /* 55461f5b853c6da88069ffcdf7f4dd3f8d7e3e6b  cantarell-fonts-0.0.21/otf/Cantarell-Bold-Oblique.otf */
    ਹਾਲਤ HB_CODEPOINT_ENCODE3 (188, 264, 3426):
    /* d125afa82a77a6475ac0e74e7c207914af84b37a padauk-2.80/Padauk.ttf RHEL 7.2 */
    ਹਾਲਤ HB_CODEPOINT_ENCODE3 (1058, 47032, 11818):
    /* 0f7b80437227b90a577cc078c0216160ae61b031 padauk-2.80/Padauk-Bold.ttf RHEL 7.2*/
    ਹਾਲਤ HB_CODEPOINT_ENCODE3 (1046, 47030, 12600):
    /* d3dde9aa0a6b7f8f6a89ef1002e9aaa11b882290 padauk-2.80/Padauk.ttf Ubuntu 16.04 */
    ਹਾਲਤ HB_CODEPOINT_ENCODE3 (1058, 71796, 16770):
    /* 5f3c98ccccae8a953be2d122c1b3a77fd805093f padauk-2.80/Padauk-Bold.ttf Ubuntu 16.04 */
    ਹਾਲਤ HB_CODEPOINT_ENCODE3 (1046, 71790, 17862):
    /* 6c93b63b64e8b2c93f5e824e78caca555dc887c7 padauk-2.80/Padauk-book.ttf */
    ਹਾਲਤ HB_CODEPOINT_ENCODE3 (1046, 71788, 17112):
    /* d89b1664058359b8ec82e35d3531931125991fb9 padauk-2.80/Padauk-bookbold.ttf */
    ਹਾਲਤ HB_CODEPOINT_ENCODE3 (1058, 71794, 17514):
    /* 824cfd193aaf6234b2b4dc0cf3c6ef576c0d00ef padauk-3.0/Padauk-book.ttf */
    ਹਾਲਤ HB_CODEPOINT_ENCODE3 (1330, 109904, 57938):
    /* 91fcc10cf15e012d27571e075b3b4dfe31754a8a padauk-3.0/Padauk-bookbold.ttf */
    ਹਾਲਤ HB_CODEPOINT_ENCODE3 (1330, 109904, 58972):
    /* sha1sum: c26e41d567ed821bed997e937bc0c41435689e85  Padauk.ttf
     *  "Padauk Regular" "Version 2.5", see https://crbug.com/681813 */
    ਹਾਲਤ HB_CODEPOINT_ENCODE3 (1004, 59092, 14836):
    /* 88d2006ca084f04af2df1954ed714a8c71e8400f  Courier New.ttf from macOS 15 */
    ਹਾਲਤ HB_CODEPOINT_ENCODE3 (588, 5078, 14418):
    /* 608e3ebb6dd1aee521cff08eb07d500a2c59df68  Courier New Bold.ttf from macOS 15 */
    ਹਾਲਤ HB_CODEPOINT_ENCODE3 (588, 5078, 14238):
    /* d13221044ff054efd78f1cd8631b853c3ce85676  cour.ttf from Windows 10 */
    ਹਾਲਤ HB_CODEPOINT_ENCODE3 (894, 17162, 33960):
    /* 68ed4a22d8067fcf1622ac6f6e2f4d3a2e3ec394  courbd.ttf from Windows 10 */
    ਹਾਲਤ HB_CODEPOINT_ENCODE3 (894, 17154, 34472):
    /* 4cdb0259c96b7fd7c103821bb8f08f7cc6b211d7  cour.ttf from Windows 8.1 */
    ਹਾਲਤ HB_CODEPOINT_ENCODE3 (816, 7868, 17052):
    /* 920483d8a8ed37f7f0afdabbe7f679aece7c75d8  courbd.ttf from Windows 8.1 */
    ਹਾਲਤ HB_CODEPOINT_ENCODE3 (816, 7868, 17138):
      ਵਾਪਸ ਸੱਚ;
  }
  ਵਾਪਸ ਝੂਠ;
}

ਸਾਂਚਾ <ਤਾਰਕਿਕ ਸੈੱਟ_Digest>
ਸਥਿਰ ਖਾਲੀ
hb_ot_layout_ਸੈੱਟ_glyph_props (hb_font_t *font,
			       hb_ਬਫਰ_t *ਬਫਰ)
{
  hb_ਬਫਰ_assert_gsubgpos_vars (ਬਫਰ);

  ਅਚਲ ਸਵੈਚਾਲਿਤ &gdef = *font->face->ਸਾਰਣੀ.GDEF;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = ਬਫਰ->len;
  hb_glyph_info_t *info = ਬਫਰ->info;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
  {
    ਜੇ (ਸੈੱਟ_Digest)
      ਬਫਰ->digest.ਜੋੜੋ (info[i].codepoint);
    hb_glyph_info_ਸੈੱਟ_glyph_props (&info[i], gdef.get_glyph_props (info[i].codepoint));
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
  ਵਾਪਸ face->ਸਾਰਣੀ.GDEF->ਸਾਰਣੀ->has_glyph_classes ();
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
  ਵਾਪਸ (hb_ot_layout_glyph_class_t) face->ਸਾਰਣੀ.GDEF->ਸਾਰਣੀ->get_glyph_class (glyph);
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
ਖਾਲੀ
hb_ot_layout_get_glyphs_ਵਿੱਚ_class (hb_face_t                  *face,
				  hb_ot_layout_glyph_class_t  klass,
				  hb_ਸੈੱਟ_t                   *glyphs /*ਬਾਹਰਲਾ*/)
{
  ਵਾਪਸ face->ਸਾਰਣੀ.GDEF->ਸਾਰਣੀ->get_glyphs_ਵਿੱਚ_class (klass, glyphs);
}

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_LAYOUT_ਅਣਵਰਤਿਆ
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
ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_ot_layout_get_attach_points (hb_face_t      *face,
				hb_codepoint_t  glyph,
				ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    ਸ਼ੁਰੂ_ਵਿੱਥ,
				ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ   *ਬਿੰਦੂ_ਗਿਣਤੀ /* IN/OUT */,
				ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ   *ਬਿੰਦੂ_array /*ਬਾਹਰਲਾ*/)
{
  ਵਾਪਸ face->ਸਾਰਣੀ.GDEF->ਸਾਰਣੀ->get_attach_points (glyph,
						     ਸ਼ੁਰੂ_ਵਿੱਥ,
						     ਬਿੰਦੂ_ਗਿਣਤੀ,
						     ਬਿੰਦੂ_array);
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
ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_ot_layout_get_ligature_carets (hb_font_t      *font,
				  hb_direction_t  direction,
				  hb_codepoint_t  glyph,
				  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    ਸ਼ੁਰੂ_ਵਿੱਥ,
				  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ   *caret_ਗਿਣਤੀ /* IN/OUT */,
				  hb_position_t  *caret_array /*ਬਾਹਰਲਾ*/)
{
  ਵਾਪਸ font->face->ਸਾਰਣੀ.GDEF->ਸਾਰਣੀ->get_lig_carets (font, direction, glyph, ਸ਼ੁਰੂ_ਵਿੱਥ, caret_ਗਿਣਤੀ, caret_array);
}
#ਜੇ_ਅੰਤ


/*
 * GSUB/GPOS
 */

ਤਾਰਕਿਕ
GSUB::is_blocklisted (hb_blob_t *blob HB_ਅਣਵਰਤਿਆ,
			  hb_face_t *face) ਅਚਲ
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_LAYOUT_BLOCKLIST
  ਵਾਪਸ ਝੂਠ;
#ਜੇ_ਅੰਤ
  ਵਾਪਸ ਝੂਠ;
}

ਤਾਰਕਿਕ
GPOS::is_blocklisted (hb_blob_t *blob HB_ਅਣਵਰਤਿਆ,
			  hb_face_t *face HB_ਅਣਵਰਤਿਆ) ਅਚਲ
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_LAYOUT_BLOCKLIST
  ਵਾਪਸ ਝੂਠ;
#ਜੇ_ਅੰਤ
  ਵਾਪਸ ਝੂਠ;
}

ਸਥਿਰ ਅਚਲ OT::GSUBGPOS&
get_gsubgpos_ਸਾਰਣੀ (hb_face_t *face,
		    hb_ਟੈਗ_t   ਸਾਰਣੀ_ਟੈਗ_2)
{
  ਚੋਣ (ਸਾਰਣੀ_ਟੈਗ_2) {
    ਹਾਲਤ HB_OT_ਟੈਗ_GSUB: ਵਾਪਸ *face->ਸਾਰਣੀ.GSUB->ਸਾਰਣੀ;
    ਹਾਲਤ HB_OT_ਟੈਗ_GPOS: ਵਾਪਸ *face->ਸਾਰਣੀ.GPOS->ਸਾਰਣੀ;
    ਮੂਲ:             ਵਾਪਸ Null (OT::GSUBGPOS);
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
ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_ot_layout_ਸਾਰਣੀ_get_script_ਟੈਗ_ਸੂਚੀ (hb_face_t    *face,
				    hb_ਟੈਗ_t      ਸਾਰਣੀ_ਟੈਗ_2,
				    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  ਸ਼ੁਰੂ_ਵਿੱਥ,
				    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *script_ਗਿਣਤੀ /* IN/OUT */,
				    hb_ਟੈਗ_t     *script_ਟੈਗ_ਸੂਚੀ  /*ਬਾਹਰਲਾ*/)
{
  ਅਚਲ OT::GSUBGPOS &g = get_gsubgpos_ਸਾਰਣੀ (face, ਸਾਰਣੀ_ਟੈਗ_2);

  ਵਾਪਸ g.get_script_ਟੈਗ_ਸੂਚੀ (ਸ਼ੁਰੂ_ਵਿੱਥ, script_ਗਿਣਤੀ, script_ਟੈਗ_ਸੂਚੀ);
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_ਟੈਗ_LATIN_SCRIPT		HB_ਟੈਗ ('l', 'a', 't', 'n')

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
hb_ot_layout_ਸਾਰਣੀ_ਲੱਭੋ_script (hb_face_t    *face,
				hb_ਟੈਗ_t      ਸਾਰਣੀ_ਟੈਗ_2,
				hb_ਟੈਗ_t      script_ਟੈਗ,
				ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *script_ਸੂਚਕਾਂਕ /*ਬਾਹਰਲਾ*/)
{
  static_assert ((OT::ਸੂਚਕਾਂਕ::NOT_ਮਿਲਿਆ_ਸੂਚਕਾਂਕ == HB_OT_LAYOUT_NO_SCRIPT_ਸੂਚਕਾਂਕ), "");
  ਅਚਲ OT::GSUBGPOS &g = get_gsubgpos_ਸਾਰਣੀ (face, ਸਾਰਣੀ_ਟੈਗ_2);

  ਜੇ (g.ਲੱਭੋ_script_ਸੂਚਕਾਂਕ (script_ਟੈਗ, script_ਸੂਚਕਾਂਕ))
    ਵਾਪਸ ਸੱਚ;

  /* try finding 'DFLT' */
  ਜੇ (g.ਲੱਭੋ_script_ਸੂਚਕਾਂਕ (HB_OT_ਟੈਗ_DEFAULT_SCRIPT, script_ਸੂਚਕਾਂਕ))
    ਵਾਪਸ ਝੂਠ;

  /* try with 'dflt'; MS site has had typos and many fonts use it now :(.
   * including many versions of DejaVu Sans Mono! */
  ਜੇ (g.ਲੱਭੋ_script_ਸੂਚਕਾਂਕ (HB_OT_ਟੈਗ_DEFAULT_LANGUAGE, script_ਸੂਚਕਾਂਕ))
    ਵਾਪਸ ਝੂਠ;

  /* try with 'latn'; some old fonts put their features there even though
     they're really trying to support Thai, for example :( */
  ਜੇ (g.ਲੱਭੋ_script_ਸੂਚਕਾਂਕ (HB_OT_ਟੈਗ_LATIN_SCRIPT, script_ਸੂਚਕਾਂਕ))
    ਵਾਪਸ ਝੂਠ;

  ਜੇ (script_ਸੂਚਕਾਂਕ) *script_ਸੂਚਕਾਂਕ = HB_OT_LAYOUT_NO_SCRIPT_ਸੂਚਕਾਂਕ;
  ਵਾਪਸ ਝੂਠ;
}

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_DISABLE_DEPRECATED
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
hb_ot_layout_ਸਾਰਣੀ_ਚੁਣੋ_script (hb_face_t      *face,
				  hb_ਟੈਗ_t        ਸਾਰਣੀ_ਟੈਗ_2,
				  ਅਚਲ hb_ਟੈਗ_t *script_ਟੈਗ_ਸੂਚੀ,
				  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ   *script_ਸੂਚਕਾਂਕ  /*ਬਾਹਰਲਾ*/,
				  hb_ਟੈਗ_t       *chosen_script /*ਬਾਹਰਲਾ*/)
{
  ਅਚਲ hb_ਟੈਗ_t *t;
  ਲਈ (t = script_ਟੈਗ_ਸੂਚੀ; *t; t++);
  ਵਾਪਸ hb_ot_layout_ਸਾਰਣੀ_select_script (face, ਸਾਰਣੀ_ਟੈਗ_2, t - script_ਟੈਗ_ਸੂਚੀ, script_ਟੈਗ_ਸੂਚੀ, script_ਸੂਚਕਾਂਕ, chosen_script);
}
#ਜੇ_ਅੰਤ

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
hb_ot_layout_ਸਾਰਣੀ_select_script (hb_face_t      *face,
				  hb_ਟੈਗ_t        ਸਾਰਣੀ_ਟੈਗ_2,
				  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    script_ਗਿਣਤੀ,
				  ਅਚਲ hb_ਟੈਗ_t *script_ਟੈਗ_ਸੂਚੀ,
				  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ   *script_ਸੂਚਕਾਂਕ  /*ਬਾਹਰਲਾ*/,
				  hb_ਟੈਗ_t       *chosen_script /*ਬਾਹਰਲਾ*/)
{
  static_assert ((OT::ਸੂਚਕਾਂਕ::NOT_ਮਿਲਿਆ_ਸੂਚਕਾਂਕ == HB_OT_LAYOUT_NO_SCRIPT_ਸੂਚਕਾਂਕ), "");
  ਅਚਲ OT::GSUBGPOS &g = get_gsubgpos_ਸਾਰਣੀ (face, ਸਾਰਣੀ_ਟੈਗ_2);
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i;

  ਲਈ (i = 0; i < script_ਗਿਣਤੀ; i++)
  {
    ਜੇ (g.ਲੱਭੋ_script_ਸੂਚਕਾਂਕ (script_ਟੈਗ_ਸੂਚੀ[i], script_ਸੂਚਕਾਂਕ))
    {
      ਜੇ (chosen_script)
	*chosen_script = script_ਟੈਗ_ਸੂਚੀ[i];
      ਵਾਪਸ ਸੱਚ;
    }
  }

  /* try finding 'DFLT' */
  ਜੇ (g.ਲੱਭੋ_script_ਸੂਚਕਾਂਕ (HB_OT_ਟੈਗ_DEFAULT_SCRIPT, script_ਸੂਚਕਾਂਕ)) {
    ਜੇ (chosen_script)
      *chosen_script = HB_OT_ਟੈਗ_DEFAULT_SCRIPT;
    ਵਾਪਸ ਝੂਠ;
  }

  /* try with 'dflt'; MS site has had typos and many fonts use it now :( */
  ਜੇ (g.ਲੱਭੋ_script_ਸੂਚਕਾਂਕ (HB_OT_ਟੈਗ_DEFAULT_LANGUAGE, script_ਸੂਚਕਾਂਕ)) {
    ਜੇ (chosen_script)
      *chosen_script = HB_OT_ਟੈਗ_DEFAULT_LANGUAGE;
    ਵਾਪਸ ਝੂਠ;
  }

  /* try with 'latn'; some old fonts put their features there even though
     they're really trying to support Thai, for example :( */
  ਜੇ (g.ਲੱਭੋ_script_ਸੂਚਕਾਂਕ (HB_OT_ਟੈਗ_LATIN_SCRIPT, script_ਸੂਚਕਾਂਕ)) {
    ਜੇ (chosen_script)
      *chosen_script = HB_OT_ਟੈਗ_LATIN_SCRIPT;
    ਵਾਪਸ ਝੂਠ;
  }

  ਜੇ (script_ਸੂਚਕਾਂਕ) *script_ਸੂਚਕਾਂਕ = HB_OT_LAYOUT_NO_SCRIPT_ਸੂਚਕਾਂਕ;
  ਜੇ (chosen_script)
    *chosen_script = HB_ਟੈਗ_NONE;
  ਵਾਪਸ ਝੂਠ;
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
ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_ot_layout_ਸਾਰਣੀ_get_feature_ਟੈਗ_ਸੂਚੀ (hb_face_t    *face,
				     hb_ਟੈਗ_t      ਸਾਰਣੀ_ਟੈਗ_2,
				     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  ਸ਼ੁਰੂ_ਵਿੱਥ,
				     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *feature_ਗਿਣਤੀ_2 /* IN/OUT */,
				     hb_ਟੈਗ_t     *feature_ਟੈਗ_ਸੂਚੀ  /*ਬਾਹਰਲਾ*/)
{
  ਅਚਲ OT::GSUBGPOS &g = get_gsubgpos_ਸਾਰਣੀ (face, ਸਾਰਣੀ_ਟੈਗ_2);

  ਵਾਪਸ g.get_feature_ਟੈਗ_ਸੂਚੀ (ਸ਼ੁਰੂ_ਵਿੱਥ, feature_ਗਿਣਤੀ_2, feature_ਟੈਗ_ਸੂਚੀ);
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
ਤਾਰਕਿਕ
hb_ot_layout_ਸਾਰਣੀ_ਲੱਭੋ_feature (hb_face_t    *face,
				 hb_ਟੈਗ_t      ਸਾਰਣੀ_ਟੈਗ_2,
				 hb_ਟੈਗ_t      feature_ਟੈਗ,
				 ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *feature_ਸੂਚਕਾਂਕ /*ਬਾਹਰਲਾ*/)
{
  static_assert ((OT::ਸੂਚਕਾਂਕ::NOT_ਮਿਲਿਆ_ਸੂਚਕਾਂਕ == HB_OT_LAYOUT_NO_FEATURE_ਸੂਚਕਾਂਕ), "");
  ਅਚਲ OT::GSUBGPOS &g = get_gsubgpos_ਸਾਰਣੀ (face, ਸਾਰਣੀ_ਟੈਗ_2);

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_features = g.get_feature_ਗਿਣਤੀ ();
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਅੰਕ_features; i++)
  {
    ਜੇ (feature_ਟੈਗ == g.get_feature_ਟੈਗ (i)) {
      ਜੇ (feature_ਸੂਚਕਾਂਕ) *feature_ਸੂਚਕਾਂਕ = i;
      ਵਾਪਸ ਸੱਚ;
    }
  }

  ਜੇ (feature_ਸੂਚਕਾਂਕ) *feature_ਸੂਚਕਾਂਕ = HB_OT_LAYOUT_NO_FEATURE_ਸੂਚਕਾਂਕ;
  ਵਾਪਸ ਝੂਠ;
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
ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_ot_layout_script_get_language_ਟੈਗ_ਸੂਚੀ (hb_face_t    *face,
				       hb_ਟੈਗ_t      ਸਾਰਣੀ_ਟੈਗ_2,
				       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  script_ਸੂਚਕਾਂਕ,
				       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  ਸ਼ੁਰੂ_ਵਿੱਥ,
				       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *language_ਗਿਣਤੀ /* IN/OUT */,
				       hb_ਟੈਗ_t     *language_ਟੈਗ_ਸੂਚੀ  /*ਬਾਹਰਲਾ*/)
{
  ਅਚਲ OT::Script &s = get_gsubgpos_ਸਾਰਣੀ (face, ਸਾਰਣੀ_ਟੈਗ_2).get_script (script_ਸੂਚਕਾਂਕ);

  ਵਾਪਸ s.get_lang_sys_ਟੈਗ_ਸੂਚੀ (ਸ਼ੁਰੂ_ਵਿੱਥ, language_ਗਿਣਤੀ, language_ਟੈਗ_ਸੂਚੀ);
}


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_DISABLE_DEPRECATED
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
hb_ot_layout_script_ਲੱਭੋ_language (hb_face_t    *face,
				   hb_ਟੈਗ_t      ਸਾਰਣੀ_ਟੈਗ_2,
				   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  script_ਸੂਚਕਾਂਕ,
				   hb_ਟੈਗ_t      language_ਟੈਗ,
				   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *language_ਸੂਚਕਾਂਕ)
{
  ਵਾਪਸ hb_ot_layout_script_select_language (face,
					      ਸਾਰਣੀ_ਟੈਗ_2,
					      script_ਸੂਚਕਾਂਕ,
					      1,
					      &language_ਟੈਗ,
					      language_ਸੂਚਕਾਂਕ);
}
#ਜੇ_ਅੰਤ


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
				     hb_ਟੈਗ_t        ਸਾਰਣੀ_ਟੈਗ_2,
				     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    script_ਸੂਚਕਾਂਕ,
				     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    language_ਗਿਣਤੀ,
				     ਅਚਲ hb_ਟੈਗ_t *language_ਟੈਗ_ਸੂਚੀ,
				     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ   *language_ਸੂਚਕਾਂਕ /*ਬਾਹਰਲਾ*/,
				     hb_ਟੈਗ_t       *chosen_language /*ਬਾਹਰਲਾ*/)
{
  static_assert ((OT::ਸੂਚਕਾਂਕ::NOT_ਮਿਲਿਆ_ਸੂਚਕਾਂਕ == HB_OT_LAYOUT_DEFAULT_LANGUAGE_ਸੂਚਕਾਂਕ), "");
  ਅਚਲ OT::Script &s = get_gsubgpos_ਸਾਰਣੀ (face, ਸਾਰਣੀ_ਟੈਗ_2).get_script (script_ਸੂਚਕਾਂਕ);
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i;

  ਲਈ (i = 0; i < language_ਗਿਣਤੀ; i++)
  {
    ਜੇ (s.ਲੱਭੋ_lang_sys_ਸੂਚਕਾਂਕ (language_ਟੈਗ_ਸੂਚੀ[i], language_ਸੂਚਕਾਂਕ))
    {
      ਜੇ (chosen_language)
        *chosen_language = language_ਟੈਗ_ਸੂਚੀ[i];
      ਵਾਪਸ ਸੱਚ;
    }
  }

  /* try finding 'dflt' */
  ਜੇ (s.ਲੱਭੋ_lang_sys_ਸੂਚਕਾਂਕ (HB_OT_ਟੈਗ_DEFAULT_LANGUAGE, language_ਸੂਚਕਾਂਕ))
  {
    ਜੇ (chosen_language)
      *chosen_language = HB_OT_ਟੈਗ_DEFAULT_LANGUAGE;
    ਵਾਪਸ ਝੂਠ;
  }

  ਜੇ (language_ਸੂਚਕਾਂਕ)
    *language_ਸੂਚਕਾਂਕ = HB_OT_LAYOUT_DEFAULT_LANGUAGE_ਸੂਚਕਾਂਕ;
  ਜੇ (chosen_language)
    *chosen_language = HB_ਟੈਗ_NONE;
  ਵਾਪਸ ਝੂਠ;
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
				     hb_ਟੈਗ_t        ਸਾਰਣੀ_ਟੈਗ_2,
				     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    script_ਸੂਚਕਾਂਕ,
				     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    language_ਗਿਣਤੀ,
				     ਅਚਲ hb_ਟੈਗ_t *language_ਟੈਗ_ਸੂਚੀ,
				     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ   *language_ਸੂਚਕਾਂਕ /*ਬਾਹਰਲਾ*/)
{
  ਵਾਪਸ hb_ot_layout_script_select_language2 (face, ਸਾਰਣੀ_ਟੈਗ_2,
					       script_ਸੂਚਕਾਂਕ,
					       language_ਗਿਣਤੀ, language_ਟੈਗ_ਸੂਚੀ,
					       language_ਸੂਚਕਾਂਕ, ਖਾਲੀ_ਸੂਚਕ);
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
hb_ot_layout_language_get_required_feature_ਸੂਚਕਾਂਕ (hb_face_t    *face,
						  hb_ਟੈਗ_t      ਸਾਰਣੀ_ਟੈਗ_2,
						  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  script_ਸੂਚਕਾਂਕ,
						  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  language_ਸੂਚਕਾਂਕ,
						  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *feature_ਸੂਚਕਾਂਕ /*ਬਾਹਰਲਾ*/)
{
  ਵਾਪਸ hb_ot_layout_language_get_required_feature (face,
						     ਸਾਰਣੀ_ਟੈਗ_2,
						     script_ਸੂਚਕਾਂਕ,
						     language_ਸੂਚਕਾਂਕ,
						     feature_ਸੂਚਕਾਂਕ,
						     ਖਾਲੀ_ਸੂਚਕ);
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
					    hb_ਟੈਗ_t      ਸਾਰਣੀ_ਟੈਗ_2,
					    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  script_ਸੂਚਕਾਂਕ,
					    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  language_ਸੂਚਕਾਂਕ,
					    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *feature_ਸੂਚਕਾਂਕ /*ਬਾਹਰਲਾ*/,
					    hb_ਟੈਗ_t     *feature_ਟੈਗ   /*ਬਾਹਰਲਾ*/)
{
  ਅਚਲ OT::GSUBGPOS &g = get_gsubgpos_ਸਾਰਣੀ (face, ਸਾਰਣੀ_ਟੈਗ_2);
  ਅਚਲ OT::LangSys &l = g.get_script (script_ਸੂਚਕਾਂਕ).get_lang_sys (language_ਸੂਚਕਾਂਕ);

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੂਚਕਾਂਕ_2 = l.get_required_feature_ਸੂਚਕਾਂਕ ();
  ਜੇ (feature_ਸੂਚਕਾਂਕ) *feature_ਸੂਚਕਾਂਕ = ਸੂਚਕਾਂਕ_2;
  ਜੇ (feature_ਟੈਗ) *feature_ਟੈਗ = g.get_feature_ਟੈਗ (ਸੂਚਕਾਂਕ_2);

  ਵਾਪਸ l.has_required_feature ();
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
ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_ot_layout_language_get_feature_indexes (hb_face_t    *face,
					   hb_ਟੈਗ_t      ਸਾਰਣੀ_ਟੈਗ_2,
					   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  script_ਸੂਚਕਾਂਕ,
					   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  language_ਸੂਚਕਾਂਕ,
					   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  ਸ਼ੁਰੂ_ਵਿੱਥ,
					   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *feature_ਗਿਣਤੀ_2   /* IN/OUT */,
					   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *feature_indexes /*ਬਾਹਰਲਾ*/)
{
  ਅਚਲ OT::GSUBGPOS &g = get_gsubgpos_ਸਾਰਣੀ (face, ਸਾਰਣੀ_ਟੈਗ_2);
  ਅਚਲ OT::LangSys &l = g.get_script (script_ਸੂਚਕਾਂਕ).get_lang_sys (language_ਸੂਚਕਾਂਕ);

  ਵਾਪਸ l.get_feature_indexes (ਸ਼ੁਰੂ_ਵਿੱਥ, feature_ਗਿਣਤੀ_2, feature_indexes);
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
ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_ot_layout_language_get_feature_ਟੈਗ_ਸੂਚੀ (hb_face_t    *face,
					hb_ਟੈਗ_t      ਸਾਰਣੀ_ਟੈਗ_2,
					ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  script_ਸੂਚਕਾਂਕ,
					ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  language_ਸੂਚਕਾਂਕ,
					ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  ਸ਼ੁਰੂ_ਵਿੱਥ,
					ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *feature_ਗਿਣਤੀ_2 /* IN/OUT */,
					hb_ਟੈਗ_t     *feature_ਟੈਗ_ਸੂਚੀ  /*ਬਾਹਰਲਾ*/)
{
  ਅਚਲ OT::GSUBGPOS &g = get_gsubgpos_ਸਾਰਣੀ (face, ਸਾਰਣੀ_ਟੈਗ_2);
  ਅਚਲ OT::LangSys &l = g.get_script (script_ਸੂਚਕਾਂਕ).get_lang_sys (language_ਸੂਚਕਾਂਕ);

  static_assert ((ਆਕਾਰ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) == ਆਕਾਰ (hb_ਟੈਗ_t)), "");
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ret = l.get_feature_indexes (ਸ਼ੁਰੂ_ਵਿੱਥ, feature_ਗਿਣਤੀ_2, (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *) feature_ਟੈਗ_ਸੂਚੀ);

  ਜੇ (feature_ਗਿਣਤੀ_2 && feature_ਟੈਗ_ਸੂਚੀ) {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = *feature_ਗਿਣਤੀ_2;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
      feature_ਟੈਗ_ਸੂਚੀ[i] = g.get_feature_ਟੈਗ ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) feature_ਟੈਗ_ਸੂਚੀ[i]);
  }

  ਵਾਪਸ ret;
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
hb_ot_layout_language_ਲੱਭੋ_feature (hb_face_t    *face,
				    hb_ਟੈਗ_t      ਸਾਰਣੀ_ਟੈਗ_2,
				    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  script_ਸੂਚਕਾਂਕ,
				    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  language_ਸੂਚਕਾਂਕ,
				    hb_ਟੈਗ_t      feature_ਟੈਗ,
				    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *feature_ਸੂਚਕਾਂਕ /*ਬਾਹਰਲਾ*/)
{
  static_assert ((OT::ਸੂਚਕਾਂਕ::NOT_ਮਿਲਿਆ_ਸੂਚਕਾਂਕ == HB_OT_LAYOUT_NO_FEATURE_ਸੂਚਕਾਂਕ), "");
  ਅਚਲ OT::GSUBGPOS &g = get_gsubgpos_ਸਾਰਣੀ (face, ਸਾਰਣੀ_ਟੈਗ_2);
  ਅਚਲ OT::LangSys &l = g.get_script (script_ਸੂਚਕਾਂਕ).get_lang_sys (language_ਸੂਚਕਾਂਕ);

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_features = l.get_feature_ਗਿਣਤੀ ();
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਅੰਕ_features; i++) {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ f_ਸੂਚਕਾਂਕ = l.get_feature_ਸੂਚਕਾਂਕ (i);

    ਜੇ (feature_ਟੈਗ == g.get_feature_ਟੈਗ (f_ਸੂਚਕਾਂਕ)) {
      ਜੇ (feature_ਸੂਚਕਾਂਕ) *feature_ਸੂਚਕਾਂਕ = f_ਸੂਚਕਾਂਕ;
      ਵਾਪਸ ਸੱਚ;
    }
  }

  ਜੇ (feature_ਸੂਚਕਾਂਕ) *feature_ਸੂਚਕਾਂਕ = HB_OT_LAYOUT_NO_FEATURE_ਸੂਚਕਾਂਕ;
  ਵਾਪਸ ਝੂਠ;
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
ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_ot_layout_feature_get_lookups (hb_face_t    *face,
				  hb_ਟੈਗ_t      ਸਾਰਣੀ_ਟੈਗ_2,
				  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  feature_ਸੂਚਕਾਂਕ,
				  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  ਸ਼ੁਰੂ_ਵਿੱਥ,
				  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *lookup_ਗਿਣਤੀ   /* IN/OUT */,
				  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *lookup_indexes /*ਬਾਹਰਲਾ*/)
{
  ਵਾਪਸ hb_ot_layout_feature_with_variations_get_lookups (face,
							   ਸਾਰਣੀ_ਟੈਗ_2,
							   feature_ਸੂਚਕਾਂਕ,
							   HB_OT_LAYOUT_NO_VARIATIONS_ਸੂਚਕਾਂਕ,
							   ਸ਼ੁਰੂ_ਵਿੱਥ,
							   lookup_ਗਿਣਤੀ,
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
ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_ot_layout_ਸਾਰਣੀ_get_lookup_ਗਿਣਤੀ (hb_face_t    *face,
				     hb_ਟੈਗ_t      ਸਾਰਣੀ_ਟੈਗ_2)
{
  ਵਾਪਸ get_gsubgpos_ਸਾਰਣੀ (face, ਸਾਰਣੀ_ਟੈਗ_2).get_lookup_ਗਿਣਤੀ ();
}


ਬਣਤਰ hb_collect_features_context_t
{
  hb_collect_features_context_t (hb_face_t *face,
				 hb_ਟੈਗ_t   ਸਾਰਣੀ_ਟੈਗ_2,
				 hb_ਸੈੱਟ_t  *feature_indices_,
				 ਅਚਲ hb_ਟੈਗ_t *features)

    : g (get_gsubgpos_ਸਾਰਣੀ (face, ਸਾਰਣੀ_ਟੈਗ_2)),
      feature_indices (feature_indices_),
      has_feature_filter (ਝੂਠ),
      script_ਗਿਣਤੀ (0),langsys_ਗਿਣਤੀ (0), feature_ਸੂਚਕਾਂਕ_ਗਿਣਤੀ (0)
  {
    compute_feature_filter (features);
  }

  ਖਾਲੀ compute_feature_filter (ਅਚਲ hb_ਟੈਗ_t *features)
  {
    ਜੇ (features == ਖਾਲੀ_ਸੂਚਕ)
    {
      has_feature_filter = ਝੂਠ;
      ਵਾਪਸ;
    }

    has_feature_filter = ਸੱਚ;
    hb_ਸੈੱਟ_t features_ਸੈੱਟ;
    ਲਈ (; *features; features++)
      features_ਸੈੱਟ.ਜੋੜੋ (*features);

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < g.get_feature_ਗਿਣਤੀ (); i++)
    {
      hb_ਟੈਗ_t ਟੈਗ_2 = g.get_feature_ਟੈਗ (i);
      ਜੇ (features_ਸੈੱਟ.has (ਟੈਗ_2))
	feature_indices_filter.ਜੋੜੋ(i);
    }
  }

  ਤਾਰਕਿਕ ਦੇਖਿਆ (ਅਚਲ OT::Script &s)
  {
    /* We might have Null() object here.  Don't want to involve
     * that in the memoize.  So, detect empty objects and return. */
    ਜੇ (unlikely (!s.has_default_lang_sys () &&
		  !s.get_lang_sys_ਗਿਣਤੀ ()))
      ਵਾਪਸ ਸੱਚ;

    ਜੇ (script_ਗਿਣਤੀ++ > HB_MAX_SCRIPTS)
      ਵਾਪਸ ਸੱਚ;

    ਵਾਪਸ ਦੇਖਿਆ (s, ਦੇਖਿਆ_script);
  }
  ਤਾਰਕਿਕ ਦੇਖਿਆ (ਅਚਲ OT::LangSys &l)
  {
    /* We might have Null() object here.  Don't want to involve
     * that in the memoize.  So, detect empty objects and return. */
    ਜੇ (unlikely (!l.has_required_feature () &&
		  !l.get_feature_ਗਿਣਤੀ ()))
      ਵਾਪਸ ਸੱਚ;

    ਜੇ (langsys_ਗਿਣਤੀ++ > HB_MAX_LANGSYS)
      ਵਾਪਸ ਸੱਚ;

    ਵਾਪਸ ਦੇਖਿਆ (l, ਦੇਖਿਆ_langsys);
  }

  ਤਾਰਕਿਕ ਦੇਖਿਆ_feature_indices (ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2)
  {
    feature_ਸੂਚਕਾਂਕ_ਗਿਣਤੀ += ਗਿਣਤੀ_2;
    ਵਾਪਸ feature_ਸੂਚਕਾਂਕ_ਗਿਣਤੀ > HB_MAX_FEATURE_INDICES;
  }

  ਨਿਜੀ:
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਤਾਰਕਿਕ ਦੇਖਿਆ (ਅਚਲ T &p, hb_ਸੈੱਟ_t &ਦੇਖਿਆ_ਸੈੱਟ)
  {
    hb_codepoint_t delta = (hb_codepoint_t) ((uintptr_t) &p - (uintptr_t) &g);
     ਜੇ (ਦੇਖਿਆ_ਸੈੱਟ.has (delta))
      ਵਾਪਸ ਸੱਚ;

    ਦੇਖਿਆ_ਸੈੱਟ.ਜੋੜੋ (delta);
    ਵਾਪਸ ਝੂਠ;
  }

  ਜਨਤਕ:
  ਅਚਲ OT::GSUBGPOS &g;
  hb_ਸੈੱਟ_t *feature_indices;
  hb_ਸੈੱਟ_t  feature_indices_filter;
  ਤਾਰਕਿਕ has_feature_filter;

  ਨਿਜੀ:
  hb_ਸੈੱਟ_t ਦੇਖਿਆ_script;
  hb_ਸੈੱਟ_t ਦੇਖਿਆ_langsys;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ script_ਗਿਣਤੀ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ langsys_ਗਿਣਤੀ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ feature_ਸੂਚਕਾਂਕ_ਗਿਣਤੀ;
};

ਸਥਿਰ ਖਾਲੀ
langsys_collect_features (hb_collect_features_context_t *c,
			  ਅਚਲ OT::LangSys  &l)
{
  ਜੇ (c->ਦੇਖਿਆ (l)) ਵਾਪਸ;

  ਜੇ (!c->has_feature_filter)
  {
    /* All features. */
    ਜੇ (l.has_required_feature () && !c->ਦੇਖਿਆ_feature_indices (1))
      c->feature_indices->ਜੋੜੋ (l.get_required_feature_ਸੂਚਕਾਂਕ ());

    // TODO(garretrieger): filter out indices >= feature count?
    ਜੇ (!c->ਦੇਖਿਆ_feature_indices (l.feature_ਸੂਚਕਾਂਕ_2.len))
      l.ਜੋੜੋ_feature_indexes_ਮੰਜ਼ਿਲ (c->feature_indices);
  }
  ਨਹੀਂਤਾਂ
  {
    ਜੇ (c->feature_indices_filter.is_empty()) ਵਾਪਸ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_features = l.get_feature_ਗਿਣਤੀ ();
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਅੰਕ_features; i++)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ feature_ਸੂਚਕਾਂਕ = l.get_feature_ਸੂਚਕਾਂਕ (i);
      ਜੇ (!c->feature_indices_filter.has (feature_ਸੂਚਕਾਂਕ)) ਜਾਰੀ;

      c->feature_indices->ਜੋੜੋ (feature_ਸੂਚਕਾਂਕ);
      c->feature_indices_filter.del (feature_ਸੂਚਕਾਂਕ);
    }
  }
}

ਸਥਿਰ ਖਾਲੀ
script_collect_features (hb_collect_features_context_t *c,
			 ਅਚਲ OT::Script   &s,
			 ਅਚਲ hb_ਟੈਗ_t *languages)
{
  ਜੇ (c->ਦੇਖਿਆ (s)) ਵਾਪਸ;

  ਜੇ (!languages)
  {
    /* All languages. */
    ਜੇ (s.has_default_lang_sys ())
      langsys_collect_features (c,
				s.get_default_lang_sys ());


    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = s.get_lang_sys_ਗਿਣਤੀ ();
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ language_ਸੂਚਕਾਂਕ = 0; language_ਸੂਚਕਾਂਕ < ਗਿਣਤੀ_2; language_ਸੂਚਕਾਂਕ++)
      langsys_collect_features (c,
				s.get_lang_sys (language_ਸੂਚਕਾਂਕ));
  }
  ਨਹੀਂਤਾਂ
  {
    ਲਈ (; *languages; languages++)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ language_ਸੂਚਕਾਂਕ;
      ਜੇ (s.ਲੱਭੋ_lang_sys_ਸੂਚਕਾਂਕ (*languages, &language_ਸੂਚਕਾਂਕ))
	langsys_collect_features (c,
				  s.get_lang_sys (language_ਸੂਚਕਾਂਕ));

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
ਖਾਲੀ
hb_ot_layout_collect_features (hb_face_t      *face,
			       hb_ਟੈਗ_t        ਸਾਰਣੀ_ਟੈਗ_2,
			       ਅਚਲ hb_ਟੈਗ_t *scripts,
			       ਅਚਲ hb_ਟੈਗ_t *languages,
			       ਅਚਲ hb_ਟੈਗ_t *features,
			       hb_ਸੈੱਟ_t       *feature_indexes /*ਬਾਹਰਲਾ*/)
{
  hb_collect_features_context_t c (face, ਸਾਰਣੀ_ਟੈਗ_2, feature_indexes, features);
  ਜੇ (!scripts)
  {
    /* All scripts. */
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = c.g.get_script_ਗਿਣਤੀ ();
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ script_ਸੂਚਕਾਂਕ = 0; script_ਸੂਚਕਾਂਕ < ਗਿਣਤੀ_2; script_ਸੂਚਕਾਂਕ++)
      script_collect_features (&c,
			       c.g.get_script (script_ਸੂਚਕਾਂਕ),
			       languages);
  }
  ਨਹੀਂਤਾਂ
  {
    ਲਈ (; *scripts; scripts++)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ script_ਸੂਚਕਾਂਕ;
      ਜੇ (c.g.ਲੱਭੋ_script_ਸੂਚਕਾਂਕ (*scripts, &script_ਸੂਚਕਾਂਕ))
	script_collect_features (&c,
				 c.g.get_script (script_ਸੂਚਕਾਂਕ),
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
ਖਾਲੀ
hb_ot_layout_collect_features_map (hb_face_t      *face,
				   hb_ਟੈਗ_t        ਸਾਰਣੀ_ਟੈਗ_2,
				   ਬਿਨਾਨਿਸ਼ਾਨੀ        script_ਸੂਚਕਾਂਕ,
				   ਬਿਨਾਨਿਸ਼ਾਨੀ        language_ਸੂਚਕਾਂਕ,
				   hb_map_t       *feature_map /*ਬਾਹਰਲਾ*/)
{
  ਅਚਲ OT::GSUBGPOS &g = get_gsubgpos_ਸਾਰਣੀ (face, ਸਾਰਣੀ_ਟੈਗ_2);
  ਅਚਲ OT::LangSys &l = g.get_script (script_ਸੂਚਕਾਂਕ).get_lang_sys (language_ਸੂਚਕਾਂਕ);

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = l.get_feature_indexes (0, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
  feature_map->alloc (ਗਿਣਤੀ_2);

  /* Loop in reverse, such that earlier entries win. That emulates
   * a linear search, which seems to be what other implementations do.
   * We found that with arialuni_t.ttf, the "ur" language system has
   * duplicate features, and the earlier ones work but not later ones.
   */
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = ਗਿਣਤੀ_2; i; i--)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ feature_ਸੂਚਕਾਂਕ = 0;
    ਬਿਨਾਨਿਸ਼ਾਨੀ feature_ਗਿਣਤੀ_2 = 1;
    l.get_feature_indexes (i - 1, &feature_ਗਿਣਤੀ_2, &feature_ਸੂਚਕਾਂਕ);
    ਜੇ (!feature_ਗਿਣਤੀ_2)
      ਰੋਕੋ;
    hb_ਟੈਗ_t feature_ਟੈਗ = g.get_feature_ਟੈਗ (feature_ਸੂਚਕਾਂਕ);
    feature_map->ਸੈੱਟ (feature_ਟੈਗ, feature_ਸੂਚਕਾਂਕ);
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
ਖਾਲੀ
hb_ot_layout_collect_lookups (hb_face_t      *face,
			      hb_ਟੈਗ_t        ਸਾਰਣੀ_ਟੈਗ_2,
			      ਅਚਲ hb_ਟੈਗ_t *scripts,
			      ਅਚਲ hb_ਟੈਗ_t *languages,
			      ਅਚਲ hb_ਟੈਗ_t *features,
			      hb_ਸੈੱਟ_t       *lookup_indexes /*ਬਾਹਰਲਾ*/)
{
  ਅਚਲ OT::GSUBGPOS &g = get_gsubgpos_ਸਾਰਣੀ (face, ਸਾਰਣੀ_ਟੈਗ_2);

  hb_ਸੈੱਟ_t feature_indexes;
  hb_ot_layout_collect_features (face, ਸਾਰਣੀ_ਟੈਗ_2, scripts, languages, features, &feature_indexes);

  ਲਈ (ਸਵੈਚਾਲਿਤ feature_ਸੂਚਕਾਂਕ : feature_indexes)
    g.get_feature (feature_ਸੂਚਕਾਂਕ).ਜੋੜੋ_lookup_indexes_ਮੰਜ਼ਿਲ (lookup_indexes);

  g.feature_variation_collect_lookups (&feature_indexes, ਖਾਲੀ_ਸੂਚਕ, lookup_indexes);
}


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_LAYOUT_COLLECT_GLYPHS
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
ਖਾਲੀ
hb_ot_layout_lookup_collect_glyphs (hb_face_t    *face,
				    hb_ਟੈਗ_t      ਸਾਰਣੀ_ਟੈਗ_2,
				    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  lookup_ਸੂਚਕਾਂਕ,
				    hb_ਸੈੱਟ_t     *glyphs_before, /* OUT.  May be NULL */
				    hb_ਸੈੱਟ_t     *glyphs_ਨਿਵੇਸ਼,  /* OUT.  May be NULL */
				    hb_ਸੈੱਟ_t     *glyphs_after,  /* OUT.  May be NULL */
				    hb_ਸੈੱਟ_t     *glyphs_output  /* OUT.  May be NULL */)
{
  OT::hb_collect_glyphs_context_t c (face,
				     glyphs_before,
				     glyphs_ਨਿਵੇਸ਼,
				     glyphs_after,
				     glyphs_output);

  ਚੋਣ (ਸਾਰਣੀ_ਟੈਗ_2)
  {
    ਹਾਲਤ HB_OT_ਟੈਗ_GSUB:
    {
      ਅਚਲ OT::SubstLookup& l = face->ਸਾਰਣੀ.GSUB->ਸਾਰਣੀ->get_lookup (lookup_ਸੂਚਕਾਂਕ);
      l.collect_glyphs (&c);
      ਵਾਪਸ;
    }
    ਹਾਲਤ HB_OT_ਟੈਗ_GPOS:
    {
      ਅਚਲ OT::PosLookup& l = face->ਸਾਰਣੀ.GPOS->ਸਾਰਣੀ->get_lookup (lookup_ਸੂਚਕਾਂਕ);
      l.collect_glyphs (&c);
      ਵਾਪਸ;
    }
  }
}
#ਜੇ_ਅੰਤ


/* Variations support */

ਤਾਰਕਿਕ
hb_ot_layout_ਸਾਰਣੀ_get_feature_variations_state (hb_face_t             *face,
						   hb_ਟੈਗ_t               ਸਾਰਣੀ_ਟੈਗ_2,
						   ਅਚਲ ਪੂਰਨ             *coords,
						   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ           ਅੰਕ_coords,
						   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ          *variations_ਸੂਚਕਾਂਕ,
						   hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> *lookup_conditions)
{
  ਅਚਲ OT::GSUBGPOS &g = get_gsubgpos_ਸਾਰਣੀ (face, ਸਾਰਣੀ_ਟੈਗ_2);
  ਅਚਲ OT::GDEF &gdef = *face->ਸਾਰਣੀ.GDEF->ਸਾਰਣੀ;

  ਸਵੈਚਾਲਿਤ instancer = OT::ਵਸਤੂ_Var_Store_Instancer (&gdef.get_var_store (), ਖਾਲੀ_ਸੂਚਕ,
					      hb_array (coords, ਅੰਕ_coords));

  ਵਾਪਸ g.get_feature_variations_state (coords,
					 ਅੰਕ_coords,
					 variations_ਸੂਚਕਾਂਕ,
					 &instancer,
					 lookup_conditions);
}

ਤਾਰਕਿਕ
_hb_ot_layout_feature_get_lookup_variations (hb_face_t *face,
                                             hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ_2,
                                             ਬਿਨਾਨਿਸ਼ਾਨੀ feature_ਸੂਚਕਾਂਕ,
                                             ਬਿਨਾਨਿਸ਼ਾਨੀ variations_ਸੂਚਕਾਂਕ,
                                             hb_sorted_array_t<ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ> conditions,
                                             hb_ਸੈੱਟ_t *lookup_indexes)
{
  ਵਾਪਸ get_gsubgpos_ਸਾਰਣੀ (face, ਸਾਰਣੀ_ਟੈਗ_2).get_lookup_variations (
      feature_ਸੂਚਕਾਂਕ, variations_ਸੂਚਕਾਂਕ, conditions, lookup_indexes);
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
hb_ot_layout_ਸਾਰਣੀ_ਲੱਭੋ_feature_variations (hb_face_t    *face,
					    hb_ਟੈਗ_t      ਸਾਰਣੀ_ਟੈਗ_2,
					    ਅਚਲ ਪੂਰਨ    *coords,
					    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  ਅੰਕ_coords,
					    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *variations_ਸੂਚਕਾਂਕ /* out */)
{
  ਅਚਲ OT::GSUBGPOS &g = get_gsubgpos_ਸਾਰਣੀ (face, ਸਾਰਣੀ_ਟੈਗ_2);
  ਅਚਲ OT::GDEF &gdef = *face->ਸਾਰਣੀ.GDEF->ਸਾਰਣੀ;

  ਸਵੈਚਾਲਿਤ instancer = OT::ਵਸਤੂ_Var_Store_Instancer(&gdef.get_var_store(), ਖਾਲੀ_ਸੂਚਕ,
					     hb_array (coords, ਅੰਕ_coords));

  ਵਾਪਸ g.ਲੱਭੋ_variations_ਸੂਚਕਾਂਕ (coords, ਅੰਕ_coords, variations_ਸੂਚਕਾਂਕ, &instancer);
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
ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_ot_layout_feature_with_variations_get_lookups (hb_face_t    *face,
						  hb_ਟੈਗ_t      ਸਾਰਣੀ_ਟੈਗ_2,
						  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  feature_ਸੂਚਕਾਂਕ,
						  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  variations_ਸੂਚਕਾਂਕ,
						  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  ਸ਼ੁਰੂ_ਵਿੱਥ,
						  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *lookup_ਗਿਣਤੀ /* IN/OUT */,
						  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *lookup_indexes /*ਬਾਹਰਲਾ*/)
{
  static_assert ((OT::FeatureVariations::NOT_ਮਿਲਿਆ_ਸੂਚਕਾਂਕ == HB_OT_LAYOUT_NO_VARIATIONS_ਸੂਚਕਾਂਕ), "");
  ਅਚਲ OT::GSUBGPOS &g = get_gsubgpos_ਸਾਰਣੀ (face, ਸਾਰਣੀ_ਟੈਗ_2);

  ਅਚਲ OT::Feature &f = g.get_feature_variation (feature_ਸੂਚਕਾਂਕ, variations_ਸੂਚਕਾਂਕ);

  ਵਾਪਸ f.get_lookup_indexes (ਸ਼ੁਰੂ_ਵਿੱਥ, lookup_ਗਿਣਤੀ, lookup_indexes);
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
  ਵਾਪਸ face->ਸਾਰਣੀ.GSUB->ਸਾਰਣੀ->has_ਅੰਕੜੇ ();
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
				      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ          lookup_ਸੂਚਕਾਂਕ,
				      ਅਚਲ hb_codepoint_t *glyphs,
				      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ          glyphs_ਲੰਮਾਈ,
				      hb_bool_t             zero_context)
{
  ਸਵੈਚਾਲਿਤ &gsub = face->ਸਾਰਣੀ.GSUB;
  ਜੇ (unlikely (lookup_ਸੂਚਕਾਂਕ >= gsub->lookup_ਗਿਣਤੀ)) ਵਾਪਸ ਝੂਠ;
  OT::hb_would_apply_context_t c (face, glyphs, glyphs_ਲੰਮਾਈ, (ਤਾਰਕਿਕ) zero_context);

  ਅਚਲ OT::SubstLookup& l = gsub->ਸਾਰਣੀ->get_lookup (lookup_ਸੂਚਕਾਂਕ);
  ਸਵੈਚਾਲਿਤ *accel = gsub->get_accel (lookup_ਸੂਚਕਾਂਕ);
  ਵਾਪਸ accel && l.would_apply (&c, accel);
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
ਖਾਲੀ
hb_ot_layout_substitute_ਸ਼ੁਰੂ (hb_font_t    *font,
			       hb_ਬਫਰ_t  *ਬਫਰ)
{
  hb_ot_layout_ਸੈੱਟ_glyph_props<ਝੂਠ> (font, ਬਫਰ);
}

ਖਾਲੀ
hb_ot_layout_substitute_ਸ਼ੁਰੂ_with_digest (hb_font_t    *font,
					   hb_ਬਫਰ_t  *ਬਫਰ)
{
  ਬਫਰ->digest = hb_ਸੈੱਟ_digest_t ();
  hb_ot_layout_ਸੈੱਟ_glyph_props<ਸੱਚ> (font, ਬਫਰ);
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
ਖਾਲੀ
hb_ot_layout_lookup_substitute_ਘੇਰਾ (hb_face_t    *face,
					ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  lookup_ਸੂਚਕਾਂਕ,
					hb_ਸੈੱਟ_t     *glyphs /*ਬਾਹਰਲਾ*/)
{
  hb_map_t ਪੂਰਾ_lookups_glyph_ਗਿਣਤੀ;
  hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, hb::unique_ptr<hb_ਸੈੱਟ_t>> ਪੂਰਾ_lookups_glyph_ਸੈੱਟ;
  OT::hb_ਘੇਰਾ_context_t c (face, glyphs, &ਪੂਰਾ_lookups_glyph_ਗਿਣਤੀ, &ਪੂਰਾ_lookups_glyph_ਸੈੱਟ);

  ਅਚਲ OT::SubstLookup& l = face->ਸਾਰਣੀ.GSUB->ਸਾਰਣੀ->get_lookup (lookup_ਸੂਚਕਾਂਕ);

  l.ਘੇਰਾ (&c, lookup_ਸੂਚਕਾਂਕ);
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
ਖਾਲੀ
hb_ot_layout_lookups_substitute_ਘੇਰਾ (hb_face_t      *face,
					 ਅਚਲ hb_ਸੈੱਟ_t *lookups,
					 hb_ਸੈੱਟ_t       *glyphs /*ਬਾਹਰਲਾ*/)
{
  hb_map_t ਪੂਰਾ_lookups_glyph_ਗਿਣਤੀ;
  hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, hb::unique_ptr<hb_ਸੈੱਟ_t>> ਪੂਰਾ_lookups_glyph_ਸੈੱਟ;
  OT::hb_ਘੇਰਾ_context_t c (face, glyphs, &ਪੂਰਾ_lookups_glyph_ਗਿਣਤੀ, &ਪੂਰਾ_lookups_glyph_ਸੈੱਟ);
  ਅਚਲ GSUB& gsub = *face->ਸਾਰਣੀ.GSUB->ਸਾਰਣੀ;

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ iteration_ਗਿਣਤੀ = 0;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ glyphs_ਲੰਮਾਈ;
  ਕਰੋ
  {
    c.reset_lookup_ਦੇਖੋ_ਗਿਣਤੀ ();
    glyphs_ਲੰਮਾਈ = glyphs->get_population ();
    ਜੇ (lookups)
    {
      ਲਈ (ਸਵੈਚਾਲਿਤ lookup_ਸੂਚਕਾਂਕ : *lookups)
	gsub.get_lookup (lookup_ਸੂਚਕਾਂਕ).ਘੇਰਾ (&c, lookup_ਸੂਚਕਾਂਕ);
    }
    ਨਹੀਂਤਾਂ
    {
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < gsub.get_lookup_ਗਿਣਤੀ (); i++)
	gsub.get_lookup (i).ਘੇਰਾ (&c, i);
    }
  } ਜਦੋਂ (iteration_ਗਿਣਤੀ++ <= HB_ਘੇਰਾ_MAX_STAGES &&
	   glyphs_ਲੰਮਾਈ != glyphs->get_population ());
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
  ਵਾਪਸ face->ਸਾਰਣੀ.GPOS->ਸਾਰਣੀ->has_ਅੰਕੜੇ ();
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
ਖਾਲੀ
hb_ot_layout_position_ਸ਼ੁਰੂ (hb_font_t *font, hb_ਬਫਰ_t *ਬਫਰ)
{
  GPOS::position_ਸ਼ੁਰੂ (font, ਬਫਰ);
}


/**
 * hb_ot_layout_position_finish_advances:
 * @font: #hb_font_t to use
 * @buffer: #hb_buffer_t buffer to work upon
 *
 * Called after positioning lookups are performed, to finish glyph advances.
 *
 **/
ਖਾਲੀ
hb_ot_layout_position_ਅੰਤ_advances (hb_font_t *font, hb_ਬਫਰ_t *ਬਫਰ)
{
  GPOS::position_ਅੰਤ_advances (font, ਬਫਰ);
}

/**
 * hb_ot_layout_position_finish_offsets:
 * @font: #hb_font_t to use
 * @buffer: #hb_buffer_t buffer to work upon
 *
 * Called after positioning lookups are performed, to finish glyph offsets.
 *
 **/
ਖਾਲੀ
hb_ot_layout_position_ਅੰਤ_offsets (hb_font_t *font, hb_ਬਫਰ_t *ਬਫਰ)
{
  GPOS::position_ਅੰਤ_offsets (font, ਬਫਰ);
}


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_LAYOUT_FEATURE_PARAMS
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
hb_ot_layout_get_ਮਾਪ_params (hb_face_t       *face,
			      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    *design_ਮਾਪ_2,       /* OUT.  May be NULL */
			      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    *subfamily_id,      /* OUT.  May be NULL */
			      hb_ot_name_id_t *subfamily_name_id, /* OUT.  May be NULL */
			      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    *range_ਸ਼ੁਰੂ_2,       /* OUT.  May be NULL */
			      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    *range_ਸਿਰਾ_2          /* OUT.  May be NULL */)
{
  ਅਚਲ GPOS &gpos = *face->ਸਾਰਣੀ.GPOS->ਸਾਰਣੀ;
  ਅਚਲ hb_ਟੈਗ_t ਟੈਗ_2 = HB_ਟੈਗ ('s','i','z','e');

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_features = gpos.get_feature_ਗਿਣਤੀ ();
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਅੰਕ_features; i++)
  {
    ਜੇ (ਟੈਗ_2 == gpos.get_feature_ਟੈਗ (i))
    {
      ਅਚਲ OT::Feature &f = gpos.get_feature (i);
      ਅਚਲ OT::Feature_Params_ਮਾਪ &params = f.get_feature_params ().get_ਮਾਪ_params (ਟੈਗ_2);

      ਜੇ (params.design_ਮਾਪ)
      {
	ਜੇ (design_ਮਾਪ_2) *design_ਮਾਪ_2 = params.design_ਮਾਪ;
	ਜੇ (subfamily_id) *subfamily_id = params.subfamilyID;
	ਜੇ (subfamily_name_id) *subfamily_name_id = params.subfamilyNameID;
	ਜੇ (range_ਸ਼ੁਰੂ_2) *range_ਸ਼ੁਰੂ_2 = params.range_ਸ਼ੁਰੂ;
	ਜੇ (range_ਸਿਰਾ_2) *range_ਸਿਰਾ_2 = params.range_ਸਿਰਾ;

	ਵਾਪਸ ਸੱਚ;
      }
    }
  }

  ਜੇ (design_ਮਾਪ_2) *design_ਮਾਪ_2 = 0;
  ਜੇ (subfamily_id) *subfamily_id = 0;
  ਜੇ (subfamily_name_id) *subfamily_name_id = HB_OT_NAME_ID_INVALID;
  ਜੇ (range_ਸ਼ੁਰੂ_2) *range_ਸ਼ੁਰੂ_2 = 0;
  ਜੇ (range_ਸਿਰਾ_2) *range_ਸਿਰਾ_2 = 0;

  ਵਾਪਸ ਝੂਠ;
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
				   hb_ਟੈਗ_t         ਸਾਰਣੀ_ਟੈਗ_2,
				   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ     feature_ਸੂਚਕਾਂਕ,
				   hb_ot_name_id_t *label_id,             /* OUT.  May be NULL */
				   hb_ot_name_id_t *tooltip_id,           /* OUT.  May be NULL */
				   hb_ot_name_id_t *sample_id,            /* OUT.  May be NULL */
				   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    *ਅੰਕ_named_parameters, /* OUT.  May be NULL */
				   hb_ot_name_id_t *ਪਹਿਲਾ_param_id        /* OUT.  May be NULL */)
{
  ਅਚਲ OT::GSUBGPOS &g = get_gsubgpos_ਸਾਰਣੀ (face, ਸਾਰਣੀ_ਟੈਗ_2);

  hb_ਟੈਗ_t feature_ਟੈਗ = g.get_feature_ਟੈਗ (feature_ਸੂਚਕਾਂਕ);
  ਅਚਲ OT::Feature &f = g.get_feature (feature_ਸੂਚਕਾਂਕ);

  ਅਚਲ OT::FeatureParams &feature_params = f.get_feature_params ();
  ਜੇ (&feature_params != &Null (OT::FeatureParams))
  {
    ਅਚਲ OT::Feature_Params_Stylistic_ਸੈੱਟ& ss_params =
      feature_params.get_stylistic_ਸੈੱਟ_params (feature_ਟੈਗ);
    ਜੇ (&ss_params != &Null (OT::Feature_Params_Stylistic_ਸੈੱਟ)) /* ssXX */
    {
      ਜੇ (label_id) *label_id = ss_params.uiNameID;
      // ssXX features don't have the rest
      ਜੇ (tooltip_id) *tooltip_id = HB_OT_NAME_ID_INVALID;
      ਜੇ (sample_id) *sample_id = HB_OT_NAME_ID_INVALID;
      ਜੇ (ਅੰਕ_named_parameters) *ਅੰਕ_named_parameters = 0;
      ਜੇ (ਪਹਿਲਾ_param_id) *ਪਹਿਲਾ_param_id = HB_OT_NAME_ID_INVALID;
      ਵਾਪਸ ਸੱਚ;
    }
    ਅਚਲ OT::FeatureParamsCharacterVariants& cv_params =
      feature_params.get_character_variants_params (feature_ਟੈਗ);
    ਜੇ (&cv_params != &Null (OT::FeatureParamsCharacterVariants)) /* cvXX */
    {
      ਜੇ (label_id) *label_id = cv_params.featUILableNameID;
      ਜੇ (tooltip_id) *tooltip_id = cv_params.feat_UITooltip_ਪਾਠ_Name_ID;
      ਜੇ (sample_id) *sample_id = cv_params.sample_ਪਾਠ_Name_ID;
      ਜੇ (ਅੰਕ_named_parameters) *ਅੰਕ_named_parameters = cv_params.ਅੰਕ_Named_Parameters;
      ਜੇ (ਪਹਿਲਾ_param_id) *ਪਹਿਲਾ_param_id = cv_params.ਪਹਿਲਾ_Param_UILabel_Name_ID;
      ਵਾਪਸ ਸੱਚ;
    }
  }

  ਜੇ (label_id) *label_id = HB_OT_NAME_ID_INVALID;
  ਜੇ (tooltip_id) *tooltip_id = HB_OT_NAME_ID_INVALID;
  ਜੇ (sample_id) *sample_id = HB_OT_NAME_ID_INVALID;
  ਜੇ (ਅੰਕ_named_parameters) *ਅੰਕ_named_parameters = 0;
  ਜੇ (ਪਹਿਲਾ_param_id) *ਪਹਿਲਾ_param_id = HB_OT_NAME_ID_INVALID;
  ਵਾਪਸ ਝੂਠ;
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
ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_ot_layout_feature_get_characters (hb_face_t      *face,
				     hb_ਟੈਗ_t        ਸਾਰਣੀ_ਟੈਗ_2,
				     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    feature_ਸੂਚਕਾਂਕ,
				     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    ਸ਼ੁਰੂ_ਵਿੱਥ,
				     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ   *char_ਗਿਣਤੀ, /* IN/OUT.  May be NULL */
				     hb_codepoint_t *characters  /* OUT.     May be NULL */)
{
  ਅਚਲ OT::GSUBGPOS &g = get_gsubgpos_ਸਾਰਣੀ (face, ਸਾਰਣੀ_ਟੈਗ_2);
  ਵਾਪਸ g.get_feature (feature_ਸੂਚਕਾਂਕ)
	  .get_feature_params ()
	  .get_character_variants_params(g.get_feature_ਟੈਗ (feature_ਸੂਚਕਾਂਕ))
	  .get_characters (ਸ਼ੁਰੂ_ਵਿੱਥ, char_ਗਿਣਤੀ, characters);
}
#ਜੇ_ਅੰਤ


/*
 * Parts of different types are implemented here such that they have direct
 * access to GSUB/GPOS lookups.
 */


ਬਣਤਰ GSUBProxy
{
  ਸਥਿਰ constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਾਰਣੀ_ਸੂਚਕਾਂਕ = 0u;
  ਸਥਿਰ constexpr ਤਾਰਕਿਕ always_inplace = ਝੂਠ;
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ OT::SubstLookup Lookup;

  GSUBProxy (hb_face_t *face) :
    accel (*face->ਸਾਰਣੀ.GSUB) {}

  ਅਚਲ GSUB::accelerator_t &accel;
};

ਬਣਤਰ GPOSProxy
{
  ਸਥਿਰ constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਾਰਣੀ_ਸੂਚਕਾਂਕ = 1u;
  ਸਥਿਰ constexpr ਤਾਰਕਿਕ always_inplace = ਸੱਚ;
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ OT::PosLookup Lookup;

  GPOSProxy (hb_face_t *face) :
    accel (*face->ਸਾਰਣੀ.GPOS) {}

  ਅਚਲ GPOS::accelerator_t &accel;
};


ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ
apply_forward (OT::hb_ot_apply_context_t *c,
	       ਅਚਲ OT::hb_ot_layout_lookup_accelerator_t &accel)
{
  ਤਾਰਕਿਕ use_hot_subtable_cache = accel.cache_enter (c);

  ਤਾਰਕਿਕ ret = ਝੂਠ;
  hb_ਬਫਰ_t *ਬਫਰ = c->ਬਫਰ;
  ਜਦੋਂ (ਬਫਰ->successful)
  {
    hb_glyph_info_t *info = ਬਫਰ->info;
    ਬਿਨਾਨਿਸ਼ਾਨੀ j = ਬਫਰ->idx;
    ਜਦੋਂ (j < ਬਫਰ->len &&
	   !(accel.digest.may_have (info[j].codepoint) &&
	     (info[j].ਨਕਾਬ & c->lookup_ਨਕਾਬ) &&
	     c->check_glyph_property (&info[j], c->lookup_props)))
      j++;
    ਜੇ (unlikely (j > ਬਫਰ->idx && !ਬਫਰ->ਅਗਲਾ_glyphs (j - ਬਫਰ->idx)))
      ਰੋਕੋ;
    ਜੇ (ਬਫਰ->idx >= ਬਫਰ->len)
      ਰੋਕੋ;

    ਜੇ (accel.apply (c, use_hot_subtable_cache))
      ret = ਸੱਚ;
    ਨਹੀਂਤਾਂ
      (ਖਾਲੀ) ਬਫਰ->ਅਗਲਾ_glyph ();
  }

  ਜੇ (use_hot_subtable_cache)
    accel.cache_leave (c);

  ਵਾਪਸ ret;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ
apply_backward (OT::hb_ot_apply_context_t *c,
	       ਅਚਲ OT::hb_ot_layout_lookup_accelerator_t &accel)
{
  ਤਾਰਕਿਕ ret = ਝੂਠ;
  hb_ਬਫਰ_t *ਬਫਰ = c->ਬਫਰ;
  ਕਰੋ
  {
    ਸਵੈਚਾਲਿਤ &cur = ਬਫਰ->cur();
    ਜੇ (accel.digest.may_have (cur.codepoint) &&
	(cur.ਨਕਾਬ & c->lookup_ਨਕਾਬ) &&
	c->check_glyph_property (&cur, c->lookup_props))
      ret |= accel.apply (c, ਝੂਠ);

    /* The reverse lookup doesn't "advance" cursor (for good reason). */
    ਬਫਰ->idx--;
  }
  ਜਦੋਂ ((ਪੂਰਨ) ਬਫਰ->idx >= 0);
  ਵਾਪਸ ret;
}

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Proxy>
ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ
apply_string (OT::hb_ot_apply_context_t *c,
	      ਅਚਲ ਕਿਸਮ_ਨਾਂ Proxy::Lookup &lookup,
	      ਅਚਲ OT::hb_ot_layout_lookup_accelerator_t &accel)
{
  hb_ਬਫਰ_t *ਬਫਰ = c->ਬਫਰ;

  ਜੇ (unlikely (!ਬਫਰ->len || !c->lookup_ਨਕਾਬ))
    ਵਾਪਸ ਝੂਠ;

  ਤਾਰਕਿਕ ret = ਝੂਠ;

  c->ਸੈੱਟ_lookup_props (lookup.get_props ());

  ਜੇ (likely (!lookup.is_ਉਲਟ ()))
  {
    /* in/out forward substitution/positioning */
    ਜੇ (!Proxy::always_inplace)
      ਬਫਰ->clear_output ();

    ਬਫਰ->idx = 0;
    ret = apply_forward (c, accel);

    ਜੇ (!Proxy::always_inplace)
      ਬਫਰ->ਸਮਕਾਲੀ ();
  }
  ਨਹੀਂਤਾਂ
  {
    /* in-place backward substitution/positioning */
    ਪ੍ਰਤਿਜ੍ਞਾ (!ਬਫਰ->have_output);
    ਬਫਰ->idx = ਬਫਰ->len - 1;
    ret = apply_backward (c, accel);
  }

  ਵਾਪਸ ret;
}

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Proxy>
ਸਤਰਅੰਦਰ ਖਾਲੀ hb_ot_map_t::apply (ਅਚਲ Proxy &proxy,
				ਅਚਲ hb_ot_shape_plan_t *plan,
				hb_font_t *font,
				hb_ਬਫਰ_t *ਬਫਰ) ਅਚਲ
{
  ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਾਰਣੀ_ਸੂਚਕਾਂਕ = proxy.ਸਾਰਣੀ_ਸੂਚਕਾਂਕ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0;

  ਸਵੈਚਾਲਿਤ *font_ਅੰਕੜੇ = font->ਅੰਕੜੇ.ot.get ();
  ਸਵੈਚਾਲਿਤ *var_store_cache = (OT::hb_scalar_cache_t *) font_ਅੰਕੜੇ;

  OT::hb_ot_apply_context_t c (ਸਾਰਣੀ_ਸੂਚਕਾਂਕ, font, ਬਫਰ, proxy.accel.get_blob (), var_store_cache);
  c.ਸੈੱਟ_recurse_func (Proxy::Lookup::ਸਾਂਚਾ dispatch_recurse_func<OT::hb_ot_apply_context_t>);

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ stage_ਸੂਚਕਾਂਕ = 0; stage_ਸੂਚਕਾਂਕ < stages[ਸਾਰਣੀ_ਸੂਚਕਾਂਕ].ਲੰਮਾਈ; stage_ਸੂਚਕਾਂਕ++)
  {
    ਅਚਲ stage_map_t *stage = &stages[ਸਾਰਣੀ_ਸੂਚਕਾਂਕ][stage_ਸੂਚਕਾਂਕ];
    ਲਈ (; i < stage->ਆਖਰੀ_lookup; i++)
    {
      ਸਵੈਚਾਲਿਤ &lookup = lookups[ਸਾਰਣੀ_ਸੂਚਕਾਂਕ][i];

      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ lookup_ਸੂਚਕਾਂਕ = lookup.ਸੂਚਕਾਂਕ_2;

      ਸਵੈਚਾਲਿਤ *accel = proxy.accel.get_accel (lookup_ਸੂਚਕਾਂਕ);
      ਜੇ (unlikely (!accel)) ਜਾਰੀ;

      ਜੇ (ਬਫਰ->messaging () &&
	  !ਬਫਰ->ਸੁਨੇਹਾ (font, "start lookup %u feature '%c%c%c%c'", lookup_ਸੂਚਕਾਂਕ, HB_UNTAG (lookup.feature_ਟੈਗ))) ਜਾਰੀ;

      /* Only try applying the lookup if there is any overlap. */
      ਜੇ (accel->digest.may_intersect (ਬਫਰ->digest) &&
	  ((ਬਫਰ->flags & HB_ਬਫਰ_FLAG_PRODUCE_UNSAFE_ਮੰਜ਼ਿਲ_CONCAT) ||
	   accel->digest_ਦੂਜਾ.may_intersect (ਬਫਰ->digest)))
      {
	c.ਸੈੱਟ_lookup_ਸੂਚਕਾਂਕ (lookup_ਸੂਚਕਾਂਕ);
	c.ਸੈੱਟ_lookup_ਨਕਾਬ (lookup.ਨਕਾਬ, ਝੂਠ);
	c.ਸੈੱਟ_auto_zwj (lookup.auto_zwj, ਝੂਠ);
	c.ਸੈੱਟ_auto_zwnj (lookup.auto_zwnj, ਝੂਠ);
	c.ਸੈੱਟ_random (lookup.random);
	c.ਸੈੱਟ_ਪ੍ਰਤੀ_syllable (lookup.ਪ੍ਰਤੀ_syllable, ਝੂਠ);
	/* apply_string's set_lookup_props initializes the iterators. */

	apply_string<Proxy> (&c,
			     proxy.accel.ਸਾਰਣੀ->get_lookup (lookup_ਸੂਚਕਾਂਕ),
			     *accel);
      }
      ਨਹੀਂਤਾਂ ਜੇ (ਬਫਰ->messaging ())
	(ਖਾਲੀ) ਬਫਰ->ਸੁਨੇਹਾ (font, "skipped lookup %u feature '%c%c%c%c' because no glyph matches", lookup_ਸੂਚਕਾਂਕ, HB_UNTAG (lookup.feature_ਟੈਗ));

      ਜੇ (ਬਫਰ->messaging ())
	(ਖਾਲੀ) ਬਫਰ->ਸੁਨੇਹਾ (font, "end lookup %u feature '%c%c%c%c'", lookup_ਸੂਚਕਾਂਕ, HB_UNTAG (lookup.feature_ਟੈਗ));
    }

    ਜੇ (stage->pause_func)
    {
      ਜੇ (stage->pause_func (plan, font, ਬਫਰ))
      {
	/* Refresh working buffer digest since buffer changed. */
	ਬਫਰ->update_digest ();
      }
    }
  }
}

ਖਾਲੀ hb_ot_map_t::substitute (ਅਚਲ hb_ot_shape_plan_t *plan, hb_font_t *font, hb_ਬਫਰ_t *ਬਫਰ) ਅਚਲ
{
  GSUBProxy proxy (font->face);
  ਅੱਖਰ ਟੈਗ_2[5] = {0};
  hb_ਟੈਗ_ਮੰਜ਼ਿਲ_string (chosen_script[0], ਟੈਗ_2);
  ਜੇ (ਬਫਰ->messaging () &&
      !ਬਫਰ->ਸੁਨੇਹਾ (font, "start table GSUB script tag '%s'", ਟੈਗ_2)) ਵਾਪਸ;
  apply (proxy, plan, font, ਬਫਰ);
  ਜੇ (ਬਫਰ->messaging ())
    (ਖਾਲੀ) ਬਫਰ->ਸੁਨੇਹਾ (font, "end table GSUB script tag '%s'", ਟੈਗ_2);
}

ਖਾਲੀ hb_ot_map_t::position (ਅਚਲ hb_ot_shape_plan_t *plan, hb_font_t *font, hb_ਬਫਰ_t *ਬਫਰ) ਅਚਲ
{
  GPOSProxy proxy (font->face);
  ਅੱਖਰ ਟੈਗ_2[5] = {0};
  hb_ਟੈਗ_ਮੰਜ਼ਿਲ_string (chosen_script[0], ਟੈਗ_2);
  ਜੇ (ਬਫਰ->messaging () &&
      !ਬਫਰ->ਸੁਨੇਹਾ (font, "start table GPOS script tag '%s'", ਟੈਗ_2)) ਵਾਪਸ;
  apply (proxy, plan, font, ਬਫਰ);
  ਜੇ (ਬਫਰ->messaging ())
    (ਖਾਲੀ) ਬਫਰ->ਸੁਨੇਹਾ (font, "end table GPOS script tag '%s'", ਟੈਗ_2);
}

ਖਾਲੀ
hb_ot_layout_substitute_lookup (OT::hb_ot_apply_context_t *c,
				ਅਚਲ OT::SubstLookup &lookup,
				ਅਚਲ OT::hb_ot_layout_lookup_accelerator_t &accel)
{
  apply_string<GSUBProxy> (c, lookup, accel);
}

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_ਅਧਾਰ

ਸਥਿਰ ਖਾਲੀ
ਚੁਣੋ_ਅਧਾਰ_ਟੈਗ_ਸੂਚੀ (hb_script_t    script,
		  hb_language_t  language,
		  hb_ਟੈਗ_t      *script_ਟੈਗ,
		  hb_ਟੈਗ_t      *language_ਟੈਗ)
{
  hb_ਟੈਗ_t script_ਟੈਗ_ਸੂਚੀ[HB_OT_MAX_ਟੈਗ_ਸੂਚੀ_ਪ੍ਰਤੀ_SCRIPT];
  ਬਿਨਾਨਿਸ਼ਾਨੀ script_ਗਿਣਤੀ = ARRAY_ਲੰਮਾਈ (script_ਟੈਗ_ਸੂਚੀ);

  hb_ਟੈਗ_t language_ਟੈਗ_ਸੂਚੀ[HB_OT_MAX_ਟੈਗ_ਸੂਚੀ_ਪ੍ਰਤੀ_LANGUAGE];
  ਬਿਨਾਨਿਸ਼ਾਨੀ language_ਗਿਣਤੀ = ARRAY_ਲੰਮਾਈ (language_ਟੈਗ_ਸੂਚੀ);

  hb_ot_ਟੈਗ_ਸੂਚੀ_from_script_and_language (script, language,
				       &script_ਗਿਣਤੀ, script_ਟੈਗ_ਸੂਚੀ,
				       &language_ਗਿਣਤੀ, language_ਟੈਗ_ਸੂਚੀ);

  *script_ਟੈਗ = script_ਗਿਣਤੀ ? script_ਟੈਗ_ਸੂਚੀ[script_ਗਿਣਤੀ - 1] : HB_OT_ਟੈਗ_DEFAULT_SCRIPT;
  *language_ਟੈਗ = language_ਗਿਣਤੀ ? language_ਟੈਗ_ਸੂਚੀ[language_ਗਿਣਤੀ - 1] : HB_OT_ਟੈਗ_DEFAULT_LANGUAGE;
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
			       hb_ਟੈਗ_t           script_ਟੈਗ,
			       hb_ਟੈਗ_t           language_ਟੈਗ,
			       hb_font_extents_t *extents)
{
  hb_position_t min = 0, max = 0;
  ਜੇ (font->face->ਸਾਰਣੀ.ਅਧਾਰ_4->get_min_max (font, direction, script_ਟੈਗ, language_ਟੈਗ, HB_ਟੈਗ_NONE,
					   &min, &max))
  {
    ਜੇ (extents)
    {
      extents->ascender  = max;
      extents->descender = min;
      extents->ਸਤਰ_gap  = 0;
    }
    ਵਾਪਸ ਸੱਚ;
  }

  hb_font_get_extents_for_direction (font, direction, extents);
  ਵਾਪਸ ਝੂਠ;
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
  hb_ਟੈਗ_t script_ਟੈਗ, language_ਟੈਗ;
  ਚੁਣੋ_ਅਧਾਰ_ਟੈਗ_ਸੂਚੀ (script, language, &script_ਟੈਗ, &language_ਟੈਗ);
  ਵਾਪਸ hb_ot_layout_get_font_extents (font,
					direction,
					script_ਟੈਗ,
					language_ਟੈਗ,
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
hb_ot_layout_baseline_ਟੈਗ_t
hb_ot_layout_get_horizontal_baseline_ਟੈਗ_for_script (hb_script_t script)
{
  /* Keep in sync with hb_ot_layout_get_baseline_with_fallback */
  ਚੋਣ ((ਪੂਰਨ) script)
  {
    /* Unicode-1.1 additions */
    ਹਾਲਤ HB_SCRIPT_BENGALI:
    ਹਾਲਤ HB_SCRIPT_DEVANAGARI:
    ਹਾਲਤ HB_SCRIPT_GUJARATI:
    ਹਾਲਤ HB_SCRIPT_GURMUKHI:
    /* Unicode-2.0 additions */
    ਹਾਲਤ HB_SCRIPT_TIBETAN:
    /* Unicode-4.0 additions */
    ਹਾਲਤ HB_SCRIPT_LIMBU:
    /* Unicode-4.1 additions */
    ਹਾਲਤ HB_SCRIPT_SYLOTI_NAGRI:
    /* Unicode-5.0 additions */
    ਹਾਲਤ HB_SCRIPT_PHAGS_PA:
    /* Unicode-5.2 additions */
    ਹਾਲਤ HB_SCRIPT_MEETEI_MAYEK:
    /* Unicode-6.1 additions */
    ਹਾਲਤ HB_SCRIPT_SHARADA:
    ਹਾਲਤ HB_SCRIPT_TAKRI:
    /* Unicode-7.0 additions */
    ਹਾਲਤ HB_SCRIPT_MODI:
    ਹਾਲਤ HB_SCRIPT_SIDDHAM:
    ਹਾਲਤ HB_SCRIPT_TIRHUTA:
    /* Unicode-9.0 additions */
    ਹਾਲਤ HB_SCRIPT_MARCHEN:
    ਹਾਲਤ HB_SCRIPT_NEWA:
    /* Unicode-10.0 additions */
    ਹਾਲਤ HB_SCRIPT_SOYOMBO:
    ਹਾਲਤ HB_SCRIPT_ZANABAZAR_ਵਰਗਫਲ:
    /* Unicode-11.0 additions */
    ਹਾਲਤ HB_SCRIPT_DOGRA:
    ਹਾਲਤ HB_SCRIPT_GUNJALA_GONDI:
    /* Unicode-12.0 additions */
    ਹਾਲਤ HB_SCRIPT_NANDINAGARI:
      ਵਾਪਸ HB_OT_LAYOUT_BASELINE_ਟੈਗ_HANGING;

    /* Unicode-1.1 additions */
    ਹਾਲਤ HB_SCRIPT_HANGUL:
    ਹਾਲਤ HB_SCRIPT_HAN:
    ਹਾਲਤ HB_SCRIPT_HIRAGANA:
    ਹਾਲਤ HB_SCRIPT_KATAKANA:
    /* Unicode-3.0 additions */
    ਹਾਲਤ HB_SCRIPT_BOPOMOFO:
    /* Unicode-9.0 additions */
    ਹਾਲਤ HB_SCRIPT_TANGUT:
    /* Unicode-10.0 additions */
    ਹਾਲਤ HB_SCRIPT_NUSHU:
    /* Unicode-13.0 additions */
    ਹਾਲਤ HB_SCRIPT_KHITAN_SMALL_SCRIPT:
    /* Unicode-18.0 additions */
    ਹਾਲਤ HB_SCRIPT_JURCHEN:
    ਹਾਲਤ HB_SCRIPT_SEAL:
      ਵਾਪਸ HB_OT_LAYOUT_BASELINE_ਟੈਗ_IDEO_FACE_BOTTOM_OR_ਖੱਬਾ;

    ਮੂਲ:
      ਵਾਪਸ HB_OT_LAYOUT_BASELINE_ਟੈਗ_ROMAN;
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
			   hb_ot_layout_baseline_ਟੈਗ_t  baseline_ਟੈਗ,
			   hb_direction_t               direction,
			   hb_ਟੈਗ_t                     script_ਟੈਗ,
			   hb_ਟੈਗ_t                     language_ਟੈਗ,
			   hb_position_t               *coord        /* OUT.  May be NULL. */)
{
  ਵਾਪਸ font->face->ਸਾਰਣੀ.ਅਧਾਰ_4->get_baseline (font, baseline_ਟੈਗ, direction, script_ਟੈਗ, language_ਟੈਗ, coord);
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
			    hb_ot_layout_baseline_ਟੈਗ_t  baseline_ਟੈਗ,
			    hb_direction_t               direction,
			    hb_script_t                  script,
			    hb_language_t                language,
			    hb_position_t               *coord        /* OUT.  May be NULL. */)
{
  hb_ਟੈਗ_t script_ਟੈਗ, language_ਟੈਗ;
  ਚੁਣੋ_ਅਧਾਰ_ਟੈਗ_ਸੂਚੀ (script, language, &script_ਟੈਗ, &language_ਟੈਗ);
  ਵਾਪਸ hb_ot_layout_get_baseline (font,
				    baseline_ਟੈਗ,
				    direction,
				    script_ਟੈਗ,
				    language_ਟੈਗ,
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
ਖਾਲੀ
hb_ot_layout_get_baseline_with_fallback (hb_font_t                   *font,
					 hb_ot_layout_baseline_ਟੈਗ_t  baseline_ਟੈਗ,
					 hb_direction_t               direction,
					 hb_ਟੈਗ_t                     script_ਟੈਗ,
					 hb_ਟੈਗ_t                     language_ਟੈਗ,
					 hb_position_t               *coord /*ਬਾਹਰਲਾ*/)
{
  ਜੇ (hb_ot_layout_get_baseline (font,
				 baseline_ਟੈਗ,
				 direction,
				 script_ਟੈਗ,
				 language_ਟੈਗ,
				 coord))
    ਵਾਪਸ;

  /* Synthesize missing baselines.
   * See https://www.w3.org/TR/css-inline-3/#baseline-synthesis-fonts
   */
  ਚੋਣ (baseline_ਟੈਗ)
  {
  ਹਾਲਤ HB_OT_LAYOUT_BASELINE_ਟੈਗ_ROMAN:
    *coord = 0; // FIXME origin ?
    ਰੋਕੋ;

  ਹਾਲਤ HB_OT_LAYOUT_BASELINE_ਟੈਗ_MATH:
    {
      hb_codepoint_t glyph;
      hb_glyph_extents_t extents;
      ਜੇ (HB_DIRECTION_IS_HORIZONTAL (direction) &&
	  (hb_font_get_nominal_glyph (font, 0x2212u, &glyph) ||
	   hb_font_get_nominal_glyph (font, '-', &glyph)) &&
	  hb_font_get_glyph_extents (font, glyph, &extents))
      {
	*coord = hb_saturate_ਜੋੜੋ (extents.y_bearing, extents.ਉਚਾਈ / 2);
      }
      ਨਹੀਂਤਾਂ
      {
	hb_position_t x_ਉਚਾਈ = font->y_scale / 2;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_METRICS
	hb_ot_metrics_get_position_with_fallback (font, HB_OT_METRICS_ਟੈਗ_X_ਉਚਾਈ, &x_ਉਚਾਈ);
#ਜੇ_ਅੰਤ
	*coord = x_ਉਚਾਈ / 2;
      }
    }
    ਰੋਕੋ;

  ਹਾਲਤ HB_OT_LAYOUT_BASELINE_ਟੈਗ_IDEO_FACE_ਸਿਖਰ_OR_ਸੱਜਾ:
  ਹਾਲਤ HB_OT_LAYOUT_BASELINE_ਟੈਗ_IDEO_FACE_BOTTOM_OR_ਖੱਬਾ:
    {
      hb_position_t embox_ਸਿਖਰ, embox_bottom;

      hb_ot_layout_get_baseline_with_fallback (font,
					       HB_OT_LAYOUT_BASELINE_ਟੈਗ_IDEO_EMBOX_ਸਿਖਰ_OR_ਸੱਜਾ,
					       direction,
					       script_ਟੈਗ,
					       language_ਟੈਗ,
					       &embox_ਸਿਖਰ);
      hb_ot_layout_get_baseline_with_fallback (font,
					       HB_OT_LAYOUT_BASELINE_ਟੈਗ_IDEO_EMBOX_BOTTOM_OR_ਖੱਬਾ,
					       direction,
					       script_ਟੈਗ,
					       language_ਟੈਗ,
					       &embox_bottom);

      ਜੇ (baseline_ਟੈਗ == HB_OT_LAYOUT_BASELINE_ਟੈਗ_IDEO_FACE_ਸਿਖਰ_OR_ਸੱਜਾ)
	*coord = hb_saturate_ਜੋੜੋ (embox_ਸਿਖਰ,
				  hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> (((int64_t) embox_bottom - embox_ਸਿਖਰ) / 10));
      ਨਹੀਂਤਾਂ
	*coord = hb_saturate_ਜੋੜੋ (embox_bottom,
				  hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> (((int64_t) embox_ਸਿਖਰ - embox_bottom) / 10));
    }
    ਰੋਕੋ;

  ਹਾਲਤ HB_OT_LAYOUT_BASELINE_ਟੈਗ_IDEO_EMBOX_ਸਿਖਰ_OR_ਸੱਜਾ:
    ਜੇ (hb_ot_layout_get_baseline (font,
				   HB_OT_LAYOUT_BASELINE_ਟੈਗ_IDEO_EMBOX_BOTTOM_OR_ਖੱਬਾ,
				   direction,
				   script_ਟੈਗ,
				   language_ਟੈਗ,
				   coord))
      *coord = hb_saturate_ਜੋੜੋ (*coord,
				HB_DIRECTION_IS_HORIZONTAL (direction) ? font->y_scale : font->x_scale);
    ਨਹੀਂਤਾਂ
    {
      hb_font_extents_t font_extents;
      hb_font_get_extents_for_direction (font, direction, &font_extents);
      *coord = font_extents.ascender;
    }
    ਰੋਕੋ;

  ਹਾਲਤ HB_OT_LAYOUT_BASELINE_ਟੈਗ_IDEO_EMBOX_BOTTOM_OR_ਖੱਬਾ:
    ਜੇ (hb_ot_layout_get_baseline (font,
				   HB_OT_LAYOUT_BASELINE_ਟੈਗ_IDEO_EMBOX_ਸਿਖਰ_OR_ਸੱਜਾ,
				   direction,
				   script_ਟੈਗ,
				   language_ਟੈਗ,
				   coord))
      *coord = hb_saturate_sub (*coord,
				HB_DIRECTION_IS_HORIZONTAL (direction) ? font->y_scale : font->x_scale);
    ਨਹੀਂਤਾਂ
    {
      hb_font_extents_t font_extents;
      hb_font_get_extents_for_direction (font, direction, &font_extents);
      *coord = font_extents.descender;
    }
    ਰੋਕੋ;

  ਹਾਲਤ HB_OT_LAYOUT_BASELINE_ਟੈਗ_HANGING:
    ਜੇ (HB_DIRECTION_IS_HORIZONTAL (direction))
    {
      hb_codepoint_t ch;
      hb_codepoint_t glyph;
      hb_glyph_extents_t extents;

      /* Keep in sync with hb_ot_layout_get_horizontal_baseline_for_script */
      ਚੋਣ ((ਪੂਰਨ) script_ਟੈਗ)
      {
      /* Unicode-1.1 additions */
      ਹਾਲਤ HB_SCRIPT_BENGALI:          ch = 0x0995u; ਰੋਕੋ;
      ਹਾਲਤ HB_SCRIPT_DEVANAGARI:       ch = 0x0915u; ਰੋਕੋ;
      ਹਾਲਤ HB_SCRIPT_GUJARATI:         ch = 0x0a95u; ਰੋਕੋ;
      ਹਾਲਤ HB_SCRIPT_GURMUKHI:         ch = 0x0a15u; ਰੋਕੋ;
      /* Unicode-2.0 additions */
      ਹਾਲਤ HB_SCRIPT_TIBETAN:          ch = 0x0f40u; ਰੋਕੋ;
      /* Unicode-4.0 additions */
      ਹਾਲਤ HB_SCRIPT_LIMBU:            ch = 0x1901u; ਰੋਕੋ;
      /* Unicode-4.1 additions */
      ਹਾਲਤ HB_SCRIPT_SYLOTI_NAGRI:     ch = 0xa807u; ਰੋਕੋ;
      /* Unicode-5.0 additions */
      ਹਾਲਤ HB_SCRIPT_PHAGS_PA:         ch = 0xa840u; ਰੋਕੋ;
      /* Unicode-5.2 additions */
      ਹਾਲਤ HB_SCRIPT_MEETEI_MAYEK:     ch = 0xabc0u; ਰੋਕੋ;
      /* Unicode-6.1 additions */
      ਹਾਲਤ HB_SCRIPT_SHARADA:          ch = 0x11191u; ਰੋਕੋ;
      ਹਾਲਤ HB_SCRIPT_TAKRI:            ch = 0x1168cu; ਰੋਕੋ;
      /* Unicode-7.0 additions */
      ਹਾਲਤ HB_SCRIPT_MODI:             ch = 0x1160eu;ਰੋਕੋ;
      ਹਾਲਤ HB_SCRIPT_SIDDHAM:          ch = 0x11590u; ਰੋਕੋ;
      ਹਾਲਤ HB_SCRIPT_TIRHUTA:          ch = 0x1148fu; ਰੋਕੋ;
      /* Unicode-9.0 additions */
      ਹਾਲਤ HB_SCRIPT_MARCHEN:          ch = 0x11c72u; ਰੋਕੋ;
      ਹਾਲਤ HB_SCRIPT_NEWA:             ch = 0x1140eu; ਰੋਕੋ;
      /* Unicode-10.0 additions */
      ਹਾਲਤ HB_SCRIPT_SOYOMBO:          ch = 0x11a5cu; ਰੋਕੋ;
      ਹਾਲਤ HB_SCRIPT_ZANABAZAR_ਵਰਗਫਲ: ch = 0x11a0bu; ਰੋਕੋ;
      /* Unicode-11.0 additions */
      ਹਾਲਤ HB_SCRIPT_DOGRA:            ch = 0x1180au; ਰੋਕੋ;
      ਹਾਲਤ HB_SCRIPT_GUNJALA_GONDI:    ch = 0x11d6cu; ਰੋਕੋ;
      /* Unicode-12.0 additions */
      ਹਾਲਤ HB_SCRIPT_NANDINAGARI:      ch = 0x119b0u; ਰੋਕੋ;
      ਮੂਲ:                         ch = 0;        ਰੋਕੋ;
      }

      ਜੇ (ch &&
	  hb_font_get_nominal_glyph (font, ch, &glyph) &&
	  hb_font_get_glyph_extents (font, glyph, &extents))
	*coord = extents.y_bearing;
      ਨਹੀਂਤਾਂ
	*coord = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> ((int64_t) font->y_scale * 6 / 10); // FIXME makes assumptions about origin
    }
    ਨਹੀਂਤਾਂ
      *coord = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> ((int64_t) font->x_scale * 6 / 10); // FIXME makes assumptions about origin
    ਰੋਕੋ;

  ਹਾਲਤ HB_OT_LAYOUT_BASELINE_ਟੈਗ_IDEO_EMBOX_CENTRAL:
    {
      hb_position_t ਸਿਖਰ_2, bottom;
      hb_ot_layout_get_baseline_with_fallback (font,
					       HB_OT_LAYOUT_BASELINE_ਟੈਗ_IDEO_EMBOX_ਸਿਖਰ_OR_ਸੱਜਾ,
					       direction,
					       script_ਟੈਗ,
					       language_ਟੈਗ,
					       &ਸਿਖਰ_2);
      hb_ot_layout_get_baseline_with_fallback (font,
					       HB_OT_LAYOUT_BASELINE_ਟੈਗ_IDEO_EMBOX_BOTTOM_OR_ਖੱਬਾ,
					       direction,
					       script_ਟੈਗ,
					       language_ਟੈਗ,
					       &bottom);
      *coord = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> (((int64_t) ਸਿਖਰ_2 + bottom) / 2);

    }
    ਰੋਕੋ;

  ਹਾਲਤ HB_OT_LAYOUT_BASELINE_ਟੈਗ_IDEO_FACE_CENTRAL:
    {
      hb_position_t ਸਿਖਰ_2, bottom;
      hb_ot_layout_get_baseline_with_fallback (font,
					       HB_OT_LAYOUT_BASELINE_ਟੈਗ_IDEO_FACE_ਸਿਖਰ_OR_ਸੱਜਾ,
					       direction,
					       script_ਟੈਗ,
					       language_ਟੈਗ,
					       &ਸਿਖਰ_2);
      hb_ot_layout_get_baseline_with_fallback (font,
					       HB_OT_LAYOUT_BASELINE_ਟੈਗ_IDEO_FACE_BOTTOM_OR_ਖੱਬਾ,
					       direction,
					       script_ਟੈਗ,
					       language_ਟੈਗ,
					       &bottom);
      *coord = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> (((int64_t) ਸਿਖਰ_2 + bottom) / 2);

    }
    ਰੋਕੋ;

  ਹਾਲਤ HB_OT_LAYOUT_BASELINE_ਟੈਗ_MAX_ਮੁੱਲ:
  ਮੂਲ:
    *coord = 0;
    ਰੋਕੋ;
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
ਖਾਲੀ
hb_ot_layout_get_baseline_with_fallback2 (hb_font_t                   *font,
					  hb_ot_layout_baseline_ਟੈਗ_t  baseline_ਟੈਗ,
					  hb_direction_t               direction,
					  hb_script_t                  script,
					  hb_language_t                language,
					  hb_position_t               *coord        /*ਬਾਹਰਲਾ*/)
{
  hb_ਟੈਗ_t script_ਟੈਗ, language_ਟੈਗ;
  ਚੁਣੋ_ਅਧਾਰ_ਟੈਗ_ਸੂਚੀ (script, language, &script_ਟੈਗ, &language_ਟੈਗ);
  hb_ot_layout_get_baseline_with_fallback (font,
					   baseline_ਟੈਗ,
					   direction,
					   script_ਟੈਗ,
					   language_ਟੈਗ,
					   coord);
}

#ਜੇ_ਅੰਤ


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_LAYOUT_RARELY_ਵਰਤਿਆ
ਬਣਤਰ hb_get_glyph_alternates_dispatch_t :
       hb_dispatch_context_t<hb_get_glyph_alternates_dispatch_t, ਬਿਨਾਨਿਸ਼ਾਨੀ>
{
  ਸਥਿਰ return_t default_return_ਮੁੱਲ () { ਵਾਪਸ 0; }
  ਤਾਰਕਿਕ stop_sublookup_iteration (return_t r) ਅਚਲ { ਵਾਪਸ r; }

  ਨਿਜੀ:
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ ...Ts> ਸਵੈਚਾਲਿਤ
  _dispatch (ਅਚਲ T &obj, hb_ਤਰਜੀਹ<1>, Ts&&... ds) HB_AUTO_RETURN
  ( obj.get_glyph_alternates (std::forward<Ts> (ds)...) )
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ ...Ts> ਸਵੈਚਾਲਿਤ
  _dispatch (ਅਚਲ T &obj, hb_ਤਰਜੀਹ<0>, Ts&&... ds) HB_AUTO_RETURN
  ( default_return_ਮੁੱਲ () )
  ਜਨਤਕ:
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ ...Ts> ਸਵੈਚਾਲਿਤ
  dispatch (ਅਚਲ T &obj, Ts&&... ds) HB_AUTO_RETURN
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
HB_EXTERN ਬਿਨਾਨਿਸ਼ਾਨੀ
hb_ot_layout_lookup_get_glyph_alternates (hb_face_t      *face,
					  ਬਿਨਾਨਿਸ਼ਾਨੀ        lookup_ਸੂਚਕਾਂਕ,
					  hb_codepoint_t  glyph,
					  ਬਿਨਾਨਿਸ਼ਾਨੀ        ਸ਼ੁਰੂ_ਵਿੱਥ,
					  ਬਿਨਾਨਿਸ਼ਾਨੀ       *alternate_ਗਿਣਤੀ  /* IN/OUT.  May be NULL. */,
					  hb_codepoint_t *alternate_glyphs /* OUT.     May be NULL. */)
{
  hb_get_glyph_alternates_dispatch_t c;
  ਅਚਲ OT::SubstLookup &lookup = face->ਸਾਰਣੀ.GSUB->ਸਾਰਣੀ->get_lookup (lookup_ਸੂਚਕਾਂਕ);
  ਸਵੈਚਾਲਿਤ ret = lookup.dispatch (&c, glyph, ਸ਼ੁਰੂ_ਵਿੱਥ, alternate_ਗਿਣਤੀ, alternate_glyphs);
  ਜੇ (!ret && alternate_ਗਿਣਤੀ) *alternate_ਗਿਣਤੀ = 0;
  ਵਾਪਸ ret;
}

ਬਣਤਰ hb_collect_glyph_alternates_dispatch_t :
       hb_dispatch_context_t<hb_collect_glyph_alternates_dispatch_t, ਤਾਰਕਿਕ>
{
  ਸਥਿਰ return_t default_return_ਮੁੱਲ () { ਵਾਪਸ ਝੂਠ; }
  ਤਾਰਕਿਕ stop_sublookup_iteration (return_t r) ਅਚਲ { ਵਾਪਸ ਝੂਠ; }

  ਨਿਜੀ:
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ ...Ts> ਸਵੈਚਾਲਿਤ
  _dispatch (ਅਚਲ T &obj, hb_ਤਰਜੀਹ<1>, Ts&&... ds) HB_AUTO_RETURN
  ( (obj.collect_glyph_alternates (std::forward<Ts> (ds)...), ਸੱਚ) )
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ ...Ts> ਸਵੈਚਾਲਿਤ
  _dispatch (ਅਚਲ T &obj, hb_ਤਰਜੀਹ<0>, Ts&&... ds) HB_AUTO_RETURN
  ( default_return_ਮੁੱਲ () )
  ਜਨਤਕ:
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ ...Ts> ਸਵੈਚਾਲਿਤ
  dispatch (ਅਚਲ T &obj, Ts&&... ds) HB_AUTO_RETURN
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
					      ਬਿਨਾਨਿਸ਼ਾਨੀ   lookup_ਸੂਚਕਾਂਕ,
					      hb_map_t  *alternate_ਗਿਣਤੀ /* IN/OUT */,
					      hb_map_t  *alternate_glyphs /* IN/OUT */)
{
  hb_collect_glyph_alternates_dispatch_t c;
  ਅਚਲ OT::SubstLookup &lookup = face->ਸਾਰਣੀ.GSUB->ਸਾਰਣੀ->get_lookup (lookup_ਸੂਚਕਾਂਕ);
  ਵਾਪਸ lookup.dispatch (&c, alternate_ਗਿਣਤੀ, alternate_glyphs);
}

ਬਣਤਰ hb_position_single_dispatch_t :
       hb_dispatch_context_t<hb_position_single_dispatch_t, ਤਾਰਕਿਕ>
{
  ਸਥਿਰ return_t default_return_ਮੁੱਲ () { ਵਾਪਸ ਝੂਠ; }
  ਤਾਰਕਿਕ stop_sublookup_iteration (return_t r) ਅਚਲ { ਵਾਪਸ r; }

  ਨਿਜੀ:
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ ...Ts> ਸਵੈਚਾਲਿਤ
  _dispatch (ਅਚਲ T &obj, hb_ਤਰਜੀਹ<1>, Ts&&... ds) HB_AUTO_RETURN
  ( obj.position_single (std::forward<Ts> (ds)...) )
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ ...Ts> ਸਵੈਚਾਲਿਤ
  _dispatch (ਅਚਲ T &obj, hb_ਤਰਜੀਹ<0>, Ts&&... ds) HB_AUTO_RETURN
  ( default_return_ਮੁੱਲ () )
  ਜਨਤਕ:
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ ...Ts> ਸਵੈਚਾਲਿਤ
  dispatch (ਅਚਲ T &obj, Ts&&... ds) HB_AUTO_RETURN
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
				       ਬਿਨਾਨਿਸ਼ਾਨੀ        lookup_ਸੂਚਕਾਂਕ,
				       hb_direction_t  direction,
				       hb_codepoint_t  glyph)
{
  ਅਚਲ OT::PosLookup &lookup = font->face->ਸਾਰਣੀ.GPOS->ਸਾਰਣੀ->get_lookup (lookup_ਸੂਚਕਾਂਕ);
  hb_blob_t *blob = font->face->ਸਾਰਣੀ.GPOS->get_blob ();
  hb_glyph_position_t pos = {0};
  hb_position_single_dispatch_t c;
  lookup.dispatch (&c, font, blob, direction, glyph, pos);
  hb_position_t ret = 0;
  ਚੋਣ (direction)
  {
    ਹਾਲਤ HB_DIRECTION_LTR:
      ret = pos.x_ਵਿੱਥ_2;
      ਰੋਕੋ;
    ਹਾਲਤ HB_DIRECTION_RTL:
      ret = hb_saturate_sub (pos.x_advance, pos.x_ਵਿੱਥ_2);
      ਰੋਕੋ;
    ਹਾਲਤ HB_DIRECTION_TTB:
      ret = pos.y_ਵਿੱਥ_2;
      ਰੋਕੋ;
    ਹਾਲਤ HB_DIRECTION_BTT:
      ret = hb_saturate_sub (pos.y_advance, pos.y_ਵਿੱਥ_2);
      ਰੋਕੋ;
    ਹਾਲਤ HB_DIRECTION_INVALID:
    ਮੂਲ:
      ਰੋਕੋ;
  }
  ਵਾਪਸ ret;
}
#ਜੇ_ਅੰਤ


#ਜੇ_ਅੰਤ
