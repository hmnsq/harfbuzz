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

#جے_تعریف_شدہ HAVE_KBTS

#ضمن "hb-shaper-impl.hh"

#تعریف_کرو KB_متن_SHAPE_IMPLEMENTATION
#تعریف_کرو KB_متن_SHAPE_STATIC
#تعریف_کرو KB_متن_SHAPE_NO_CRT
#تعریف_کرو KBTS_MALLOC(a, b) hb_malloc(b)
#تعریف_کرو KBTS_FREE(a, b) hb_free(b)
#تعریف_کرو KBTS_MEMCPY hb_memcpy
#تعریف_کرو KBTS_MEMSET hb_memset

#ہدایت GCC diagnostic push
#ہدایت GCC diagnostic ignored "-Wcast-align"
#ہدایت GCC diagnostic ignored "-Wunused-function"
#ہدایت GCC diagnostic ignored "-Wextra-semi-stmt"
#ضمن "kb_text_shape.h"
#ہدایت GCC diagnostic نکالو

ساكن hb_user_انکڑے_کلید_t hb_kbts_shape_plan_انکڑے_کلید = {0};

بنية hb_kbts_shape_plan_انکڑے_t
{
  kbts_shape_config *config = خالی_سوچک;
  بدلنجوگ hb_atomic_t<kbts_shape_scratchpad *> scratchpad;
};

ساكن فراغ
hb_kbts_shape_plan_انکڑے_destroy (فراغ *انکڑے)
{
  خودکار *plan_انکڑے = (hb_kbts_shape_plan_انکڑے_t *) انکڑے;
  إذا (!plan_انکڑے)
    أرجع;

  خودکار *scratchpad = plan_انکڑے->scratchpad.get_acquire ();
  إذا (scratchpad && plan_انکڑے->scratchpad.cmpexch (scratchpad, خالی_سوچک))
    kbts_DestroyShapeScratchpad (scratchpad);

  إذا (plan_انکڑے->config)
    kbts_DestroyShapeConfig (plan_انکڑے->config);

  hb_free (plan_انکڑے);
}

ساكن فراغ
hb_kbts_plan_props_منزل_script_language (ثابت hb_segment_properties_t &props,
				       kbts_script *kb_script,
				       kbts_language *kb_language)
{
  hb_ٹیگ_t scripts[HB_OT_MAX_ٹیگ_سوچی_فی_SCRIPT];
  hb_ٹیگ_t language;
  بنانشانی صحيح script_گنتی = ARRAY_لمائی (scripts);
  بنانشانی صحيح language_گنتی = 1;

  *kb_script = KBTS_SCRIPT_DONT_KNOW;
  *kb_language = KBTS_LANGUAGE_DEFAULT;

  hb_ot_ٹیگ_سوچی_from_script_and_language (props.script, props.language,
				       &script_گنتی, scripts,
				       &language_گنتی, &language);

  لكل (بنانشانی صحيح i = 0; i < script_گنتی && scripts[i] != HB_ٹیگ_NONE; ++i)
  {
    *kb_script = kbts_Script_ٹیگ_منزل_Script (hb_uint32_وٹاندرا (scripts[i]));
    إذا (*kb_script != KBTS_SCRIPT_DONT_KNOW)
      اكسر;
  }

  إذا (language_گنتی)
    *kb_language = (kbts_language) hb_uint32_وٹاندرا (language);
}

ساكن hb_kbts_shape_plan_انکڑے_t *
hb_kbts_get_shape_plan_انکڑے (hb_shape_plan_t *shape_plan,
			     kbts_font *kb_font)
{
retry:
  خودکار *plan_انکڑے = (hb_kbts_shape_plan_انکڑے_t *)
    hb_shape_plan_get_user_انکڑے (shape_plan, &hb_kbts_shape_plan_انکڑے_کلید);
  إذا (plan_انکڑے)
    أرجع plan_انکڑے;

  kbts_script kb_script;
  kbts_language kb_language;
  hb_kbts_plan_props_منزل_script_language (shape_plan->key.props, &kb_script, &kb_language);

  kbts_shape_config *config = kbts_CreateShapeConfig (kb_font, kb_script, kb_language, خالی_سوچک, خالی_سوچک);
  إذا (unlikely (!config))
    أرجع خالی_سوچک;

  plan_انکڑے = (hb_kbts_shape_plan_انکڑے_t *) hb_calloc (1, حجم (*plan_انکڑے));
  إذا (unlikely (!plan_انکڑے))
  {
    kbts_DestroyShapeConfig (config);
    أرجع خالی_سوچک;
  }

  plan_انکڑے->config = config;
  plan_انکڑے->scratchpad.init (خالی_سوچک);

  إذا (!hb_shape_plan_سیٹ_user_انکڑے (shape_plan,
				    &hb_kbts_shape_plan_انکڑے_کلید,
				    plan_انکڑے,
				    hb_kbts_shape_plan_انکڑے_destroy,
				    خطأ))
  {
    hb_kbts_shape_plan_انکڑے_destroy (plan_انکڑے);
    اذهب retry;
  }

  أرجع plan_انکڑے;
}

ساكن kbts_shape_scratchpad *
hb_kbts_acquire_shape_scratchpad (hb_kbts_shape_plan_انکڑے_t *plan_انکڑے)
{
  خودکار *scratchpad = plan_انکڑے->scratchpad.get_acquire ();
  إذا (!scratchpad || unlikely (!plan_انکڑے->scratchpad.cmpexch (scratchpad, خالی_سوچک)))
    scratchpad = kbts_CreateShapeScratchpad (plan_انکڑے->config, خالی_سوچک, خالی_سوچک);

  أرجع scratchpad;
}

ساكن فراغ
hb_kbts_release_shape_scratchpad (hb_kbts_shape_plan_انکڑے_t *plan_انکڑے,
				  kbts_shape_scratchpad *scratchpad)
{
  إذا (!scratchpad)
    أرجع;

  إذا (!plan_انکڑے->scratchpad.cmpexch (خالی_سوچک, scratchpad))
    kbts_DestroyShapeScratchpad (scratchpad);
}


hb_kbts_face_انکڑے_t *
hb_kbts_shaper_face_انکڑے_create (hb_face_t *face)
{
  hb_blob_t *blob = hb_face_reference_blob (face);

  بنانشانی صحيح blob_لمائی;
  ثابت حرف *blob_انکڑے = hb_blob_get_انکڑے (blob, &blob_لمائی);
  إذا (unlikely (!blob_لمائی))
  {
    DEBUG_پیغام (KBTS, blob, "Empty blob");
    hb_blob_destroy (blob);
    أرجع خالی_سوچک;
  }

  kbts_font *kb_font = (kbts_font *) hb_calloc (1, حجم (kbts_font));
  إذا (unlikely (!kb_font))
  {
    hb_blob_destroy (blob);
    أرجع خالی_سوچک;
  }

  *kb_font = kbts_Font_From_حافظہ((فراغ *)blob_انکڑے, blob_لمائی, face->اشاریہ_2, خالی_سوچک, خالی_سوچک);
  hb_blob_destroy (blob);
  blob = خالی_سوچک;

  إذا (unlikely (!kbts_FontIsValid (kb_font)))
  {
    DEBUG_پیغام (KBTS, face, "Failed create font from data");
    kbts_FreeFont (kb_font);
    hb_free (kb_font);
    أرجع خالی_سوچک;
  }

  أرجع (hb_kbts_face_انکڑے_t *) kb_font;
}

فراغ
hb_kbts_shaper_face_انکڑے_destroy (hb_kbts_face_انکڑے_t *انکڑے)
{
  kbts_font *font = (kbts_font *) انکڑے;

  أكد (kbts_FontIsValid (font));

  kbts_FreeFont (font);
  hb_free (font);
}

hb_kbts_font_انکڑے_t *
hb_kbts_shaper_font_انکڑے_create (hb_font_t *font)
{
  أرجع (hb_kbts_font_انکڑے_t *) HB_SHAPER_انکڑے_SUCCEEDED;
}

فراغ
hb_kbts_shaper_font_انکڑے_destroy (hb_kbts_font_انکڑے_t *انکڑے)
{
}

hb_bool_t
_hb_kbts_shape (hb_shape_plan_t    *shape_plan,
		hb_font_t          *font,
		hb_بفر_t        *بفر,
		ثابت hb_feature_t *features,
		بنانشانی صحيح        عدد_features)
{
  hb_face_t *face = font->face;
  kbts_font *kb_font = (kbts_font *) (ثابت فراغ *) face->انکڑے.kbts;

  kbts_direction kb_direction;
  بدل (بفر->props.direction)
  {
    حالة HB_DIRECTION_LTR: kb_direction = KBTS_DIRECTION_LTR; اكسر;
    حالة HB_DIRECTION_RTL: kb_direction = KBTS_DIRECTION_RTL; اكسر;
    حالة HB_DIRECTION_TTB:
    حالة HB_DIRECTION_BTT:
      DEBUG_پیغام (KBTS, face, "Vertical direction is not supported");
      أرجع خطأ;
    افتراضي:
    حالة HB_DIRECTION_INVALID:
      DEBUG_پیغام (KBTS, face, "Invalid direction");
      أرجع خطأ;
  }

  kbts_glyph_storage kb_glyph_storage;
  إذا (unlikely (!kbts_InitializeGlyphStorage (&kb_glyph_storage, خالی_سوچک, خالی_سوچک)))
    أرجع خطأ;

  hb_kbts_shape_plan_انکڑے_t *plan_انکڑے = hb_kbts_get_shape_plan_انکڑے (shape_plan, kb_font);
  إذا (unlikely (!plan_انکڑے))
  {
    kbts_FreeAllGlyphs (&kb_glyph_storage);
    أرجع خطأ;
  }

  kbts_shape_config *kb_shape_config = خالی_سوچک;
  kbts_shape_scratchpad *kb_shape_scratchpad = خالی_سوچک;
  uint32_t glyph_گنتی = 0;
  kbts_glyph *kb_glyph = خالی_سوچک;
  kbts_glyph_iterator kb_output;
  hb_glyph_info_t *info;
  hb_glyph_position_t *pos;
  hb_bool_t res = خطأ;

  kb_shape_config = plan_انکڑے->config;
  kb_shape_scratchpad = hb_kbts_acquire_shape_scratchpad (plan_انکڑے);
  إذا (unlikely (!kb_shape_scratchpad))
    اذهب پورا;

  لكل (size_t i = 0; i < بفر->len; ++i)
  {
    kbts_glyph_config *kb_config = خالی_سوچک;
    إذا (عدد_features)
    {
      hb_سمتیہ_t<kbts_feature_override> kb_features;
      لكل (بنانشانی صحيح j = 0; j < عدد_features; ++j)
      {
        إذا (hb_وچ_range<size_t> (i, features[j].شروع, features[j].سرا))
	  kb_features.push<kbts_feature_override>({ hb_uint32_وٹاندرا (features[j].ٹیگ_2), (صحيح)features[j].value });
      }
      إذا (kb_features)
        kb_config = kbts_CreateGlyphConfig (kb_shape_config, kb_features.arrayZ, kb_features.لمائی, خالی_سوچک, خالی_سوچک);
    }
    kbts_ڈالو_Glyph (&kb_glyph_storage, kb_font, بفر->info[i].codepoint, kb_config, بفر->info[i].cluster);
  }

  res = kbts_Shape_سدھا (kb_shape_scratchpad, &kb_glyph_storage, kb_direction, &kb_output) == KBTS_SHAPE_ERROR_NONE;
  إذا (unlikely (!res))
    اذهب پورا;

  لكل (خودکار it = kb_output; kbts_Glyph_Iterator_اگلا (&it, &kb_glyph); )
    glyph_گنتی += 1;

  hb_بفر_سیٹ_content_type (بفر, HB_بفر_CONTENT_TYPE_GLYPHS);
  hb_بفر_سیٹ_لمائی (بفر, glyph_گنتی);

  بفر->clear_positions ();

  info = بفر->info;
  pos = بفر->pos;

  لكل (خودکار it = kb_output; kbts_Glyph_Iterator_اگلا (&it, &kb_glyph); info++, pos++)
  {
    info->codepoint = kb_glyph->Id;
    info->cluster = kb_glyph->User_Id_Or_Codepoint_اشاریہ;
    pos->x_advance = font->em_scalef_x (kb_glyph->AdvanceX);
    pos->y_advance = font->em_scalef_y (kb_glyph->AdvanceY);
    pos->x_وتھ_2 = font->em_scalef_x (kb_glyph->وتھ_X);
    pos->y_وتھ_2 = font->em_scalef_y (kb_glyph->وتھ_Y);
  }

پورا:
  إذا (likely (kb_shape_scratchpad))
    hb_kbts_release_shape_scratchpad (plan_انکڑے, kb_shape_scratchpad);
  طالما (kbts_Glyph_Iterator_اگلا (&kb_output, &kb_glyph))
    kbts_DestroyGlyphConfig (kb_glyph->Config);
  kbts_FreeAllGlyphs (&kb_glyph_storage);

  بفر->clear_glyph_flags ();
  بفر->unsafe_منزل_break ();

  أرجع res;
}

#جے_انت
