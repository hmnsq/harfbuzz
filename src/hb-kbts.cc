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

#اگر_تعریف_شده HAVE_KBTS

#ضمن "hb-shaper-impl.hh"

#تعریف_کن KB_متن_SHAPE_IMPLEMENTATION
#تعریف_کن KB_متن_SHAPE_STATIC
#تعریف_کن KB_متن_SHAPE_NO_CRT
#تعریف_کن KBTS_MALLOC(a, b) hb_malloc(b)
#تعریف_کن KBTS_FREE(a, b) hb_free(b)
#تعریف_کن KBTS_MEMCPY hb_memcpy
#تعریف_کن KBTS_MEMSET hb_memset

#رهنمود GCC diagnostic push
#رهنمود GCC diagnostic ignored "-Wcast-align"
#رهنمود GCC diagnostic ignored "-Wunused-function"
#رهنمود GCC diagnostic ignored "-Wextra-semi-stmt"
#ضمن "kb_text_shape.h"
#رهنمود GCC diagnostic برداشت

ساكن hb_user_داده_کلید_t hb_kbts_shape_plan_داده_کلید = {0};

بنية hb_kbts_shape_plan_داده_t
{
  kbts_shape_config *config = اشاره_تهی;
  تغییرپذیر hb_atomic_t<kbts_shape_scratchpad *> scratchpad;
};

ساكن فراغ
hb_kbts_shape_plan_داده_destroy (فراغ *داده)
{
  خودکار *plan_داده = (hb_kbts_shape_plan_داده_t *) داده;
  إذا (!plan_داده)
    أرجع;

  خودکار *scratchpad = plan_داده->scratchpad.get_acquire ();
  إذا (scratchpad && plan_داده->scratchpad.cmpexch (scratchpad, اشاره_تهی))
    kbts_DestroyShapeScratchpad (scratchpad);

  إذا (plan_داده->config)
    kbts_DestroyShapeConfig (plan_داده->config);

  hb_free (plan_داده);
}

ساكن فراغ
hb_kbts_plan_props_مقصد_script_language (ثابت hb_segment_properties_t &props,
				       kbts_script *kb_script,
				       kbts_language *kb_language)
{
  hb_برچسب_t scripts[HB_OT_MAX_برچسبها_در_هر_SCRIPT];
  hb_برچسب_t language;
  بیعلامت صحيح script_شمار = ARRAY_درازی (scripts);
  بیعلامت صحيح language_شمار = 1;

  *kb_script = KBTS_SCRIPT_DONT_KNOW;
  *kb_language = KBTS_LANGUAGE_DEFAULT;

  hb_ot_برچسبها_from_script_and_language (props.script, props.language,
				       &script_شمار, scripts,
				       &language_شمار, &language);

  لكل (بیعلامت صحيح i = 0; i < script_شمار && scripts[i] != HB_برچسب_NONE; ++i)
  {
    *kb_script = kbts_Script_برچسب_مقصد_Script (hb_uint32_جابجا (scripts[i]));
    إذا (*kb_script != KBTS_SCRIPT_DONT_KNOW)
      اكسر;
  }

  إذا (language_شمار)
    *kb_language = (kbts_language) hb_uint32_جابجا (language);
}

ساكن hb_kbts_shape_plan_داده_t *
hb_kbts_get_shape_plan_داده (hb_shape_plan_t *shape_plan,
			     kbts_font *kb_font)
{
retry:
  خودکار *plan_داده = (hb_kbts_shape_plan_داده_t *)
    hb_shape_plan_get_user_داده (shape_plan, &hb_kbts_shape_plan_داده_کلید);
  إذا (plan_داده)
    أرجع plan_داده;

  kbts_script kb_script;
  kbts_language kb_language;
  hb_kbts_plan_props_مقصد_script_language (shape_plan->key.props, &kb_script, &kb_language);

  kbts_shape_config *config = kbts_CreateShapeConfig (kb_font, kb_script, kb_language, اشاره_تهی, اشاره_تهی);
  إذا (unlikely (!config))
    أرجع اشاره_تهی;

  plan_داده = (hb_kbts_shape_plan_داده_t *) hb_calloc (1, حجم (*plan_داده));
  إذا (unlikely (!plan_داده))
  {
    kbts_DestroyShapeConfig (config);
    أرجع اشاره_تهی;
  }

  plan_داده->config = config;
  plan_داده->scratchpad.init (اشاره_تهی);

  إذا (!hb_shape_plan_مجموعه_user_داده (shape_plan,
				    &hb_kbts_shape_plan_داده_کلید,
				    plan_داده,
				    hb_kbts_shape_plan_داده_destroy,
				    خطأ))
  {
    hb_kbts_shape_plan_داده_destroy (plan_داده);
    اذهب retry;
  }

  أرجع plan_داده;
}

ساكن kbts_shape_scratchpad *
hb_kbts_acquire_shape_scratchpad (hb_kbts_shape_plan_داده_t *plan_داده)
{
  خودکار *scratchpad = plan_داده->scratchpad.get_acquire ();
  إذا (!scratchpad || unlikely (!plan_داده->scratchpad.cmpexch (scratchpad, اشاره_تهی)))
    scratchpad = kbts_CreateShapeScratchpad (plan_داده->config, اشاره_تهی, اشاره_تهی);

  أرجع scratchpad;
}

ساكن فراغ
hb_kbts_release_shape_scratchpad (hb_kbts_shape_plan_داده_t *plan_داده,
				  kbts_shape_scratchpad *scratchpad)
{
  إذا (!scratchpad)
    أرجع;

  إذا (!plan_داده->scratchpad.cmpexch (اشاره_تهی, scratchpad))
    kbts_DestroyShapeScratchpad (scratchpad);
}


hb_kbts_face_داده_t *
hb_kbts_shaper_face_داده_create (hb_face_t *face)
{
  hb_blob_t *blob = hb_face_reference_blob (face);

  بیعلامت صحيح blob_درازی;
  ثابت حرف *blob_داده = hb_blob_get_داده (blob, &blob_درازی);
  إذا (unlikely (!blob_درازی))
  {
    DEBUG_پیام (KBTS, blob, "Empty blob");
    hb_blob_destroy (blob);
    أرجع اشاره_تهی;
  }

  kbts_font *kb_font = (kbts_font *) hb_calloc (1, حجم (kbts_font));
  إذا (unlikely (!kb_font))
  {
    hb_blob_destroy (blob);
    أرجع اشاره_تهی;
  }

  *kb_font = kbts_Font_From_حافظه((فراغ *)blob_داده, blob_درازی, face->شاخص_2, اشاره_تهی, اشاره_تهی);
  hb_blob_destroy (blob);
  blob = اشاره_تهی;

  إذا (unlikely (!kbts_FontIsValid (kb_font)))
  {
    DEBUG_پیام (KBTS, face, "Failed create font from data");
    kbts_FreeFont (kb_font);
    hb_free (kb_font);
    أرجع اشاره_تهی;
  }

  أرجع (hb_kbts_face_داده_t *) kb_font;
}

فراغ
hb_kbts_shaper_face_داده_destroy (hb_kbts_face_داده_t *داده)
{
  kbts_font *font = (kbts_font *) داده;

  أكد (kbts_FontIsValid (font));

  kbts_FreeFont (font);
  hb_free (font);
}

hb_kbts_font_داده_t *
hb_kbts_shaper_font_داده_create (hb_font_t *font)
{
  أرجع (hb_kbts_font_داده_t *) HB_SHAPER_داده_SUCCEEDED;
}

فراغ
hb_kbts_shaper_font_داده_destroy (hb_kbts_font_داده_t *داده)
{
}

hb_bool_t
_hb_kbts_shape (hb_shape_plan_t    *shape_plan,
		hb_font_t          *font,
		hb_میانگیر_t        *میانگیر,
		ثابت hb_feature_t *features,
		بیعلامت صحيح        رقم_features)
{
  hb_face_t *face = font->face;
  kbts_font *kb_font = (kbts_font *) (ثابت فراغ *) face->داده.kbts;

  kbts_direction kb_direction;
  بدل (میانگیر->props.direction)
  {
    حالة HB_DIRECTION_LTR: kb_direction = KBTS_DIRECTION_LTR; اكسر;
    حالة HB_DIRECTION_RTL: kb_direction = KBTS_DIRECTION_RTL; اكسر;
    حالة HB_DIRECTION_TTB:
    حالة HB_DIRECTION_BTT:
      DEBUG_پیام (KBTS, face, "Vertical direction is not supported");
      أرجع خطأ;
    افتراضي:
    حالة HB_DIRECTION_INVALID:
      DEBUG_پیام (KBTS, face, "Invalid direction");
      أرجع خطأ;
  }

  kbts_glyph_storage kb_glyph_storage;
  إذا (unlikely (!kbts_InitializeGlyphStorage (&kb_glyph_storage, اشاره_تهی, اشاره_تهی)))
    أرجع خطأ;

  hb_kbts_shape_plan_داده_t *plan_داده = hb_kbts_get_shape_plan_داده (shape_plan, kb_font);
  إذا (unlikely (!plan_داده))
  {
    kbts_FreeAllGlyphs (&kb_glyph_storage);
    أرجع خطأ;
  }

  kbts_shape_config *kb_shape_config = اشاره_تهی;
  kbts_shape_scratchpad *kb_shape_scratchpad = اشاره_تهی;
  uint32_t glyph_شمار = 0;
  kbts_glyph *kb_glyph = اشاره_تهی;
  kbts_glyph_iterator kb_output;
  hb_glyph_info_t *info;
  hb_glyph_position_t *pos;
  hb_bool_t res = خطأ;

  kb_shape_config = plan_داده->config;
  kb_shape_scratchpad = hb_kbts_acquire_shape_scratchpad (plan_داده);
  إذا (unlikely (!kb_shape_scratchpad))
    اذهب انجامشده;

  لكل (size_t i = 0; i < میانگیر->len; ++i)
  {
    kbts_glyph_config *kb_config = اشاره_تهی;
    إذا (رقم_features)
    {
      hb_وکتور_t<kbts_feature_override> kb_features;
      لكل (بیعلامت صحيح j = 0; j < رقم_features; ++j)
      {
        إذا (hb_درون_range<size_t> (i, features[j].شروع, features[j].انتها))
	  kb_features.push<kbts_feature_override>({ hb_uint32_جابجا (features[j].برچسب_2), (صحيح)features[j].value });
      }
      إذا (kb_features)
        kb_config = kbts_CreateGlyphConfig (kb_shape_config, kb_features.arrayZ, kb_features.درازی, اشاره_تهی, اشاره_تهی);
    }
    kbts_افزودن_Glyph (&kb_glyph_storage, kb_font, میانگیر->info[i].codepoint, kb_config, میانگیر->info[i].cluster);
  }

  res = kbts_Shape_مستقیم (kb_shape_scratchpad, &kb_glyph_storage, kb_direction, &kb_output) == KBTS_SHAPE_ERROR_NONE;
  إذا (unlikely (!res))
    اذهب انجامشده;

  لكل (خودکار it = kb_output; kbts_Glyph_Iterator_بعدی (&it, &kb_glyph); )
    glyph_شمار += 1;

  hb_میانگیر_مجموعه_content_type (میانگیر, HB_میانگیر_CONTENT_TYPE_GLYPHS);
  hb_میانگیر_مجموعه_درازی (میانگیر, glyph_شمار);

  میانگیر->clear_positions ();

  info = میانگیر->info;
  pos = میانگیر->pos;

  لكل (خودکار it = kb_output; kbts_Glyph_Iterator_بعدی (&it, &kb_glyph); info++, pos++)
  {
    info->codepoint = kb_glyph->Id;
    info->cluster = kb_glyph->User_Id_Or_Codepoint_شاخص;
    pos->x_advance = font->em_scalef_x (kb_glyph->AdvanceX);
    pos->y_advance = font->em_scalef_y (kb_glyph->AdvanceY);
    pos->x_جابه_جایی_2 = font->em_scalef_x (kb_glyph->جابه_جایی_X);
    pos->y_جابه_جایی_2 = font->em_scalef_y (kb_glyph->جابه_جایی_Y);
  }

انجامشده:
  إذا (likely (kb_shape_scratchpad))
    hb_kbts_release_shape_scratchpad (plan_داده, kb_shape_scratchpad);
  طالما (kbts_Glyph_Iterator_بعدی (&kb_output, &kb_glyph))
    kbts_DestroyGlyphConfig (kb_glyph->Config);
  kbts_FreeAllGlyphs (&kb_glyph_storage);

  میانگیر->clear_glyph_flags ();
  میانگیر->unsafe_مقصد_break ();

  أرجع res;
}

#پایان_اگر
