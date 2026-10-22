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

#شامل "hb.hh"

#اگر_تعریف_شدہ HAVE_KBTS

#شامل "hb-shaper-impl.hh"

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
#شامل "kb_text_shape.h"
#ہدایت GCC diagnostic نکالو

ساکن hb_user_ڈیٹا_کلید_t hb_kbts_shape_plan_ڈیٹا_کلید = {0};

ساخت hb_kbts_shape_plan_ڈیٹا_t
{
  kbts_shape_config *config = خالی_اشاریہ;
  تغیرپذیر hb_atomic_t<kbts_shape_scratchpad *> scratchpad;
};

ساکن خالی
hb_kbts_shape_plan_ڈیٹا_destroy (خالی *ڈیٹا)
{
  خودکار *plan_ڈیٹا = (hb_kbts_shape_plan_ڈیٹا_t *) ڈیٹا;
  اگر (!plan_ڈیٹا)
    واپس;

  خودکار *scratchpad = plan_ڈیٹا->scratchpad.get_acquire ();
  اگر (scratchpad && plan_ڈیٹا->scratchpad.cmpexch (scratchpad, خالی_اشاریہ))
    kbts_DestroyShapeScratchpad (scratchpad);

  اگر (plan_ڈیٹا->config)
    kbts_DestroyShapeConfig (plan_ڈیٹا->config);

  hb_free (plan_ڈیٹا);
}

ساکن خالی
hb_kbts_plan_props_مقصد_script_language (ثابت hb_segment_properties_t &props,
				       kbts_script *kb_script,
				       kbts_language *kb_language)
{
  hb_لیبل_t scripts[HB_OT_MAX_لیبل_فہرست_فی_SCRIPT];
  hb_لیبل_t language;
  غیرعلامتی صحیح script_گنتی = ARRAY_لمبائی (scripts);
  غیرعلامتی صحیح language_گنتی = 1;

  *kb_script = KBTS_SCRIPT_DONT_KNOW;
  *kb_language = KBTS_LANGUAGE_DEFAULT;

  hb_ot_لیبل_فہرست_from_script_and_language (props.script, props.language,
				       &script_گنتی, scripts,
				       &language_گنتی, &language);

  برائے (غیرعلامتی صحیح i = 0; i < script_گنتی && scripts[i] != HB_لیبل_NONE; ++i)
  {
    *kb_script = kbts_Script_لیبل_مقصد_Script (hb_uint32_تبادلہ (scripts[i]));
    اگر (*kb_script != KBTS_SCRIPT_DONT_KNOW)
      توڑو;
  }

  اگر (language_گنتی)
    *kb_language = (kbts_language) hb_uint32_تبادلہ (language);
}

ساکن hb_kbts_shape_plan_ڈیٹا_t *
hb_kbts_get_shape_plan_ڈیٹا (hb_shape_plan_t *shape_plan,
			     kbts_font *kb_font)
{
retry:
  خودکار *plan_ڈیٹا = (hb_kbts_shape_plan_ڈیٹا_t *)
    hb_shape_plan_get_user_ڈیٹا (shape_plan, &hb_kbts_shape_plan_ڈیٹا_کلید);
  اگر (plan_ڈیٹا)
    واپس plan_ڈیٹا;

  kbts_script kb_script;
  kbts_language kb_language;
  hb_kbts_plan_props_مقصد_script_language (shape_plan->key.props, &kb_script, &kb_language);

  kbts_shape_config *config = kbts_CreateShapeConfig (kb_font, kb_script, kb_language, خالی_اشاریہ, خالی_اشاریہ);
  اگر (unlikely (!config))
    واپس خالی_اشاریہ;

  plan_ڈیٹا = (hb_kbts_shape_plan_ڈیٹا_t *) hb_calloc (1, حجم (*plan_ڈیٹا));
  اگر (unlikely (!plan_ڈیٹا))
  {
    kbts_DestroyShapeConfig (config);
    واپس خالی_اشاریہ;
  }

  plan_ڈیٹا->config = config;
  plan_ڈیٹا->scratchpad.init (خالی_اشاریہ);

  اگر (!hb_shape_plan_سیٹ_user_ڈیٹا (shape_plan,
				    &hb_kbts_shape_plan_ڈیٹا_کلید,
				    plan_ڈیٹا,
				    hb_kbts_shape_plan_ڈیٹا_destroy,
				    جھوٹ))
  {
    hb_kbts_shape_plan_ڈیٹا_destroy (plan_ڈیٹا);
    جاؤ retry;
  }

  واپس plan_ڈیٹا;
}

ساکن kbts_shape_scratchpad *
hb_kbts_acquire_shape_scratchpad (hb_kbts_shape_plan_ڈیٹا_t *plan_ڈیٹا)
{
  خودکار *scratchpad = plan_ڈیٹا->scratchpad.get_acquire ();
  اگر (!scratchpad || unlikely (!plan_ڈیٹا->scratchpad.cmpexch (scratchpad, خالی_اشاریہ)))
    scratchpad = kbts_CreateShapeScratchpad (plan_ڈیٹا->config, خالی_اشاریہ, خالی_اشاریہ);

  واپس scratchpad;
}

ساکن خالی
hb_kbts_release_shape_scratchpad (hb_kbts_shape_plan_ڈیٹا_t *plan_ڈیٹا,
				  kbts_shape_scratchpad *scratchpad)
{
  اگر (!scratchpad)
    واپس;

  اگر (!plan_ڈیٹا->scratchpad.cmpexch (خالی_اشاریہ, scratchpad))
    kbts_DestroyShapeScratchpad (scratchpad);
}


hb_kbts_face_ڈیٹا_t *
hb_kbts_shaper_face_ڈیٹا_create (hb_face_t *face)
{
  hb_blob_t *blob = hb_face_reference_blob (face);

  غیرعلامتی صحیح blob_لمبائی;
  ثابت حرف *blob_ڈیٹا = hb_blob_get_ڈیٹا (blob, &blob_لمبائی);
  اگر (unlikely (!blob_لمبائی))
  {
    DEBUG_پیغام (KBTS, blob, "Empty blob");
    hb_blob_destroy (blob);
    واپس خالی_اشاریہ;
  }

  kbts_font *kb_font = (kbts_font *) hb_calloc (1, حجم (kbts_font));
  اگر (unlikely (!kb_font))
  {
    hb_blob_destroy (blob);
    واپس خالی_اشاریہ;
  }

  *kb_font = kbts_Font_From_حافظہ((خالی *)blob_ڈیٹا, blob_لمبائی, face->اشاریہ_2, خالی_اشاریہ, خالی_اشاریہ);
  hb_blob_destroy (blob);
  blob = خالی_اشاریہ;

  اگر (unlikely (!kbts_FontIsValid (kb_font)))
  {
    DEBUG_پیغام (KBTS, face, "Failed create font from data");
    kbts_FreeFont (kb_font);
    hb_free (kb_font);
    واپس خالی_اشاریہ;
  }

  واپس (hb_kbts_face_ڈیٹا_t *) kb_font;
}

خالی
hb_kbts_shaper_face_ڈیٹا_destroy (hb_kbts_face_ڈیٹا_t *ڈیٹا)
{
  kbts_font *font = (kbts_font *) ڈیٹا;

  أكد (kbts_FontIsValid (font));

  kbts_FreeFont (font);
  hb_free (font);
}

hb_kbts_font_ڈیٹا_t *
hb_kbts_shaper_font_ڈیٹا_create (hb_font_t *font)
{
  واپس (hb_kbts_font_ڈیٹا_t *) HB_SHAPER_ڈیٹا_SUCCEEDED;
}

خالی
hb_kbts_shaper_font_ڈیٹا_destroy (hb_kbts_font_ڈیٹا_t *ڈیٹا)
{
}

hb_bool_t
_hb_kbts_shape (hb_shape_plan_t    *shape_plan,
		hb_font_t          *font,
		hb_بفر_t        *بفر,
		ثابت hb_feature_t *features,
		غیرعلامتی صحیح        عدد_features)
{
  hb_face_t *face = font->face;
  kbts_font *kb_font = (kbts_font *) (ثابت خالی *) face->ڈیٹا.kbts;

  kbts_direction kb_direction;
  انتخاب (بفر->props.direction)
  {
    حالت HB_DIRECTION_LTR: kb_direction = KBTS_DIRECTION_LTR; توڑو;
    حالت HB_DIRECTION_RTL: kb_direction = KBTS_DIRECTION_RTL; توڑو;
    حالت HB_DIRECTION_TTB:
    حالت HB_DIRECTION_BTT:
      DEBUG_پیغام (KBTS, face, "Vertical direction is not supported");
      واپس جھوٹ;
    طےشدہ:
    حالت HB_DIRECTION_INVALID:
      DEBUG_پیغام (KBTS, face, "Invalid direction");
      واپس جھوٹ;
  }

  kbts_glyph_storage kb_glyph_storage;
  اگر (unlikely (!kbts_InitializeGlyphStorage (&kb_glyph_storage, خالی_اشاریہ, خالی_اشاریہ)))
    واپس جھوٹ;

  hb_kbts_shape_plan_ڈیٹا_t *plan_ڈیٹا = hb_kbts_get_shape_plan_ڈیٹا (shape_plan, kb_font);
  اگر (unlikely (!plan_ڈیٹا))
  {
    kbts_FreeAllGlyphs (&kb_glyph_storage);
    واپس جھوٹ;
  }

  kbts_shape_config *kb_shape_config = خالی_اشاریہ;
  kbts_shape_scratchpad *kb_shape_scratchpad = خالی_اشاریہ;
  uint32_t glyph_گنتی = 0;
  kbts_glyph *kb_glyph = خالی_اشاریہ;
  kbts_glyph_iterator kb_output;
  hb_glyph_info_t *info;
  hb_glyph_position_t *pos;
  hb_bool_t res = جھوٹ;

  kb_shape_config = plan_ڈیٹا->config;
  kb_shape_scratchpad = hb_kbts_acquire_shape_scratchpad (plan_ڈیٹا);
  اگر (unlikely (!kb_shape_scratchpad))
    جاؤ مکمل;

  برائے (size_t i = 0; i < بفر->len; ++i)
  {
    kbts_glyph_config *kb_config = خالی_اشاریہ;
    اگر (عدد_features)
    {
      hb_سمتیہ_t<kbts_feature_override> kb_features;
      برائے (غیرعلامتی صحیح j = 0; j < عدد_features; ++j)
      {
        اگر (hb_اندر_range<size_t> (i, features[j].آغاز, features[j].انتہا))
	  kb_features.push<kbts_feature_override>({ hb_uint32_تبادلہ (features[j].لیبل_2), (صحیح)features[j].value });
      }
      اگر (kb_features)
        kb_config = kbts_CreateGlyphConfig (kb_shape_config, kb_features.arrayZ, kb_features.لمبائی, خالی_اشاریہ, خالی_اشاریہ);
    }
    kbts_ڈالو_Glyph (&kb_glyph_storage, kb_font, بفر->info[i].codepoint, kb_config, بفر->info[i].cluster);
  }

  res = kbts_Shape_براہ_راست (kb_shape_scratchpad, &kb_glyph_storage, kb_direction, &kb_output) == KBTS_SHAPE_ERROR_NONE;
  اگر (unlikely (!res))
    جاؤ مکمل;

  برائے (خودکار it = kb_output; kbts_Glyph_Iterator_اگلا (&it, &kb_glyph); )
    glyph_گنتی += 1;

  hb_بفر_سیٹ_content_type (بفر, HB_بفر_CONTENT_TYPE_GLYPHS);
  hb_بفر_سیٹ_لمبائی (بفر, glyph_گنتی);

  بفر->clear_positions ();

  info = بفر->info;
  pos = بفر->pos;

  برائے (خودکار it = kb_output; kbts_Glyph_Iterator_اگلا (&it, &kb_glyph); info++, pos++)
  {
    info->codepoint = kb_glyph->Id;
    info->cluster = kb_glyph->User_Id_Or_Codepoint_اشاریہ;
    pos->x_advance = font->em_scalef_x (kb_glyph->AdvanceX);
    pos->y_advance = font->em_scalef_y (kb_glyph->AdvanceY);
    pos->x_انحراف_2 = font->em_scalef_x (kb_glyph->انحراف_X);
    pos->y_انحراف_2 = font->em_scalef_y (kb_glyph->انحراف_Y);
  }

مکمل:
  اگر (likely (kb_shape_scratchpad))
    hb_kbts_release_shape_scratchpad (plan_ڈیٹا, kb_shape_scratchpad);
  جب (kbts_Glyph_Iterator_اگلا (&kb_output, &kb_glyph))
    kbts_DestroyGlyphConfig (kb_glyph->Config);
  kbts_FreeAllGlyphs (&kb_glyph_storage);

  بفر->clear_glyph_flags ();
  بفر->unsafe_مقصد_break ();

  واپس res;
}

#اگر_ختم
