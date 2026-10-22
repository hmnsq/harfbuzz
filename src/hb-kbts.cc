/*
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
 * Author(s): Khaled Hosny
 */

#ضمن "hb.hh"

#جيڪڏهن_وصف_ٿيل HAVE_KBTS

#ضمن "hb-shaper-impl.hh"

#وصف_ڪريو KB_متن_SHAPE_IMPLEMENTATION
#وصف_ڪريو KB_متن_SHAPE_STATIC
#وصف_ڪريو KB_متن_SHAPE_NO_CRT
#وصف_ڪريو KBTS_MALLOC(a, b) hb_malloc(b)
#وصف_ڪريو KBTS_FREE(a, b) hb_free(b)
#وصف_ڪريو KBTS_MEMCPY hb_memcpy
#وصف_ڪريو KBTS_MEMSET hb_memset

#هدايت GCC diagnostic push
#هدايت GCC diagnostic ignored "-Wcast-align"
#هدايت GCC diagnostic ignored "-Wunused-function"
#هدايت GCC diagnostic ignored "-Wextra-semi-stmt"
#ضمن "kb_text_shape.h"
#هدايت GCC diagnostic ڪڍو

ساكن hb_user_معلومات_ڪنجي_t hb_kbts_shape_plan_معلومات_ڪنجي = {0};

بنية hb_kbts_shape_plan_معلومات_t
{
  kbts_shape_config *config = خالي_اشارو;
  بدلجندڙ hb_atomic_t<kbts_shape_scratchpad *> scratchpad;
};

ساكن فراغ
hb_kbts_shape_plan_معلومات_destroy (فراغ *معلومات)
{
  خودڪار *plan_معلومات = (hb_kbts_shape_plan_معلومات_t *) معلومات;
  إذا (!plan_معلومات)
    أرجع;

  خودڪار *scratchpad = plan_معلومات->scratchpad.get_acquire ();
  إذا (scratchpad && plan_معلومات->scratchpad.cmpexch (scratchpad, خالي_اشارو))
    kbts_DestroyShapeScratchpad (scratchpad);

  إذا (plan_معلومات->config)
    kbts_DestroyShapeConfig (plan_معلومات->config);

  hb_free (plan_معلومات);
}

ساكن فراغ
hb_kbts_plan_props_منزل_script_language (ثابت hb_segment_properties_t &props,
				       kbts_script *kb_script,
				       kbts_language *kb_language)
{
  hb_ٽيگ_t scripts[HB_OT_MAX_ٽيگ_فهرست_في_SCRIPT];
  hb_ٽيگ_t language;
  بنانشان صحيح script_ڳڻپ = ARRAY_ڊيگهائي (scripts);
  بنانشان صحيح language_ڳڻپ = 1;

  *kb_script = KBTS_SCRIPT_DONT_KNOW;
  *kb_language = KBTS_LANGUAGE_DEFAULT;

  hb_ot_ٽيگ_فهرست_from_script_and_language (props.script, props.language,
				       &script_ڳڻپ, scripts,
				       &language_ڳڻپ, &language);

  لكل (بنانشان صحيح i = 0; i < script_ڳڻپ && scripts[i] != HB_ٽيگ_NONE; ++i)
  {
    *kb_script = kbts_Script_ٽيگ_منزل_Script (hb_uint32_مٽا_سٽا (scripts[i]));
    إذا (*kb_script != KBTS_SCRIPT_DONT_KNOW)
      اكسر;
  }

  إذا (language_ڳڻپ)
    *kb_language = (kbts_language) hb_uint32_مٽا_سٽا (language);
}

ساكن hb_kbts_shape_plan_معلومات_t *
hb_kbts_get_shape_plan_معلومات (hb_shape_plan_t *shape_plan,
			     kbts_font *kb_font)
{
retry:
  خودڪار *plan_معلومات = (hb_kbts_shape_plan_معلومات_t *)
    hb_shape_plan_get_user_معلومات (shape_plan, &hb_kbts_shape_plan_معلومات_ڪنجي);
  إذا (plan_معلومات)
    أرجع plan_معلومات;

  kbts_script kb_script;
  kbts_language kb_language;
  hb_kbts_plan_props_منزل_script_language (shape_plan->key.props, &kb_script, &kb_language);

  kbts_shape_config *config = kbts_CreateShapeConfig (kb_font, kb_script, kb_language, خالي_اشارو, خالي_اشارو);
  إذا (unlikely (!config))
    أرجع خالي_اشارو;

  plan_معلومات = (hb_kbts_shape_plan_معلومات_t *) hb_calloc (1, حجم (*plan_معلومات));
  إذا (unlikely (!plan_معلومات))
  {
    kbts_DestroyShapeConfig (config);
    أرجع خالي_اشارو;
  }

  plan_معلومات->config = config;
  plan_معلومات->scratchpad.init (خالي_اشارو);

  إذا (!hb_shape_plan_سيٽ_user_معلومات (shape_plan,
				    &hb_kbts_shape_plan_معلومات_ڪنجي,
				    plan_معلومات,
				    hb_kbts_shape_plan_معلومات_destroy,
				    خطأ))
  {
    hb_kbts_shape_plan_معلومات_destroy (plan_معلومات);
    اذهب retry;
  }

  أرجع plan_معلومات;
}

ساكن kbts_shape_scratchpad *
hb_kbts_acquire_shape_scratchpad (hb_kbts_shape_plan_معلومات_t *plan_معلومات)
{
  خودڪار *scratchpad = plan_معلومات->scratchpad.get_acquire ();
  إذا (!scratchpad || unlikely (!plan_معلومات->scratchpad.cmpexch (scratchpad, خالي_اشارو)))
    scratchpad = kbts_CreateShapeScratchpad (plan_معلومات->config, خالي_اشارو, خالي_اشارو);

  أرجع scratchpad;
}

ساكن فراغ
hb_kbts_release_shape_scratchpad (hb_kbts_shape_plan_معلومات_t *plan_معلومات,
				  kbts_shape_scratchpad *scratchpad)
{
  إذا (!scratchpad)
    أرجع;

  إذا (!plan_معلومات->scratchpad.cmpexch (خالي_اشارو, scratchpad))
    kbts_DestroyShapeScratchpad (scratchpad);
}


hb_kbts_face_معلومات_t *
hb_kbts_shaper_face_معلومات_create (hb_face_t *face)
{
  hb_blob_t *blob = hb_face_reference_blob (face);

  بنانشان صحيح blob_ڊيگهائي;
  ثابت حرف *blob_معلومات = hb_blob_get_معلومات (blob, &blob_ڊيگهائي);
  إذا (unlikely (!blob_ڊيگهائي))
  {
    DEBUG_نياپو (KBTS, blob, "Empty blob");
    hb_blob_destroy (blob);
    أرجع خالي_اشارو;
  }

  kbts_font *kb_font = (kbts_font *) hb_calloc (1, حجم (kbts_font));
  إذا (unlikely (!kb_font))
  {
    hb_blob_destroy (blob);
    أرجع خالي_اشارو;
  }

  *kb_font = kbts_Font_From_يادگيري((فراغ *)blob_معلومات, blob_ڊيگهائي, face->اشاريو_2, خالي_اشارو, خالي_اشارو);
  hb_blob_destroy (blob);
  blob = خالي_اشارو;

  إذا (unlikely (!kbts_FontIsValid (kb_font)))
  {
    DEBUG_نياپو (KBTS, face, "Failed create font from data");
    kbts_FreeFont (kb_font);
    hb_free (kb_font);
    أرجع خالي_اشارو;
  }

  أرجع (hb_kbts_face_معلومات_t *) kb_font;
}

فراغ
hb_kbts_shaper_face_معلومات_destroy (hb_kbts_face_معلومات_t *معلومات)
{
  kbts_font *font = (kbts_font *) معلومات;

  أكد (kbts_FontIsValid (font));

  kbts_FreeFont (font);
  hb_free (font);
}

hb_kbts_font_معلومات_t *
hb_kbts_shaper_font_معلومات_create (hb_font_t *font)
{
  أرجع (hb_kbts_font_معلومات_t *) HB_SHAPER_معلومات_SUCCEEDED;
}

فراغ
hb_kbts_shaper_font_معلومات_destroy (hb_kbts_font_معلومات_t *معلومات)
{
}

hb_bool_t
_hb_kbts_shape (hb_shape_plan_t    *shape_plan,
		hb_font_t          *font,
		hb_بفر_t        *بفر,
		ثابت hb_feature_t *features,
		بنانشان صحيح        انگ_features)
{
  hb_face_t *face = font->face;
  kbts_font *kb_font = (kbts_font *) (ثابت فراغ *) face->معلومات.kbts;

  kbts_direction kb_direction;
  بدل (بفر->props.direction)
  {
    حالة HB_DIRECTION_LTR: kb_direction = KBTS_DIRECTION_LTR; اكسر;
    حالة HB_DIRECTION_RTL: kb_direction = KBTS_DIRECTION_RTL; اكسر;
    حالة HB_DIRECTION_TTB:
    حالة HB_DIRECTION_BTT:
      DEBUG_نياپو (KBTS, face, "Vertical direction is not supported");
      أرجع خطأ;
    افتراضي:
    حالة HB_DIRECTION_INVALID:
      DEBUG_نياپو (KBTS, face, "Invalid direction");
      أرجع خطأ;
  }

  kbts_glyph_storage kb_glyph_storage;
  إذا (unlikely (!kbts_InitializeGlyphStorage (&kb_glyph_storage, خالي_اشارو, خالي_اشارو)))
    أرجع خطأ;

  hb_kbts_shape_plan_معلومات_t *plan_معلومات = hb_kbts_get_shape_plan_معلومات (shape_plan, kb_font);
  إذا (unlikely (!plan_معلومات))
  {
    kbts_FreeAllGlyphs (&kb_glyph_storage);
    أرجع خطأ;
  }

  kbts_shape_config *kb_shape_config = خالي_اشارو;
  kbts_shape_scratchpad *kb_shape_scratchpad = خالي_اشارو;
  uint32_t glyph_ڳڻپ = 0;
  kbts_glyph *kb_glyph = خالي_اشارو;
  kbts_glyph_iterator kb_output;
  hb_glyph_info_t *info;
  hb_glyph_position_t *pos;
  hb_bool_t res = خطأ;

  kb_shape_config = plan_معلومات->config;
  kb_shape_scratchpad = hb_kbts_acquire_shape_scratchpad (plan_معلومات);
  إذا (unlikely (!kb_shape_scratchpad))
    اذهب پورو;

  لكل (size_t i = 0; i < بفر->len; ++i)
  {
    kbts_glyph_config *kb_config = خالي_اشارو;
    إذا (انگ_features)
    {
      hb_ويڪٽر_t<kbts_feature_override> kb_features;
      لكل (بنانشان صحيح j = 0; j < انگ_features; ++j)
      {
        إذا (hb_اندريون_range<size_t> (i, features[j].شروعات, features[j].پڇاڙي))
	  kb_features.push<kbts_feature_override>({ hb_uint32_مٽا_سٽا (features[j].ٽيگ_2), (صحيح)features[j].value });
      }
      إذا (kb_features)
        kb_config = kbts_CreateGlyphConfig (kb_shape_config, kb_features.arrayZ, kb_features.ڊيگهائي, خالي_اشارو, خالي_اشارو);
    }
    kbts_ڌڪو_Glyph (&kb_glyph_storage, kb_font, بفر->info[i].codepoint, kb_config, بفر->info[i].cluster);
  }

  res = kbts_Shape_سڌو (kb_shape_scratchpad, &kb_glyph_storage, kb_direction, &kb_output) == KBTS_SHAPE_ERROR_NONE;
  إذا (unlikely (!res))
    اذهب پورو;

  لكل (خودڪار it = kb_output; kbts_Glyph_Iterator_ايندڙ (&it, &kb_glyph); )
    glyph_ڳڻپ += 1;

  hb_بفر_سيٽ_content_type (بفر, HB_بفر_CONTENT_TYPE_GLYPHS);
  hb_بفر_سيٽ_ڊيگهائي (بفر, glyph_ڳڻپ);

  بفر->clear_positions ();

  info = بفر->info;
  pos = بفر->pos;

  لكل (خودڪار it = kb_output; kbts_Glyph_Iterator_ايندڙ (&it, &kb_glyph); info++, pos++)
  {
    info->codepoint = kb_glyph->Id;
    info->cluster = kb_glyph->User_Id_Or_Codepoint_اشاريو;
    pos->x_advance = font->em_scalef_x (kb_glyph->AdvanceX);
    pos->y_advance = font->em_scalef_y (kb_glyph->AdvanceY);
    pos->x_وٿي_2 = font->em_scalef_x (kb_glyph->وٿي_X);
    pos->y_وٿي_2 = font->em_scalef_y (kb_glyph->وٿي_Y);
  }

پورو:
  إذا (likely (kb_shape_scratchpad))
    hb_kbts_release_shape_scratchpad (plan_معلومات, kb_shape_scratchpad);
  طالما (kbts_Glyph_Iterator_ايندڙ (&kb_output, &kb_glyph))
    kbts_DestroyGlyphConfig (kb_glyph->Config);
  kbts_FreeAllGlyphs (&kb_glyph_storage);

  بفر->clear_glyph_flags ();
  بفر->unsafe_منزل_break ();

  أرجع res;
}

#جيڪڏهن_پورو
