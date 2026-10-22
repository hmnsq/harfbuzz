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

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_KBTS

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-shaper-impl.hh"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ KB_ਪਾਠ_SHAPE_IMPLEMENTATION
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ KB_ਪਾਠ_SHAPE_STATIC
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ KB_ਪਾਠ_SHAPE_NO_CRT
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ KBTS_MALLOC(a, b) hb_malloc(b)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ KBTS_FREE(a, b) hb_free(b)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ KBTS_MEMCPY hb_memcpy
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ KBTS_MEMSET hb_memset

#ਹਦਾਇਤ GCC diagnostic push
#ਹਦਾਇਤ GCC diagnostic ignored "-Wcast-align"
#ਹਦਾਇਤ GCC diagnostic ignored "-Wunused-function"
#ਹਦਾਇਤ GCC diagnostic ignored "-Wextra-semi-stmt"
#ਅਨ੍ਤਰ੍ਭਾਵ "kb_text_shape.h"
#ਹਦਾਇਤ GCC diagnostic ਕੱਢੋ

ਸਥਿਰ hb_user_ਅੰਕੜੇ_ਕੁੰਜੀ_t hb_kbts_shape_plan_ਅੰਕੜੇ_ਕੁੰਜੀ = {0};

ਬਣਤਰ hb_kbts_shape_plan_ਅੰਕੜੇ_t
{
  kbts_shape_config *config = ਖਾਲੀ_ਸੂਚਕ;
  ਬਦਲਣਯੋਗ hb_atomic_t<kbts_shape_scratchpad *> scratchpad;
};

ਸਥਿਰ ਖਾਲੀ
hb_kbts_shape_plan_ਅੰਕੜੇ_destroy (ਖਾਲੀ *ਅੰਕੜੇ)
{
  ਸਵੈਚਾਲਿਤ *plan_ਅੰਕੜੇ = (hb_kbts_shape_plan_ਅੰਕੜੇ_t *) ਅੰਕੜੇ;
  ਜੇ (!plan_ਅੰਕੜੇ)
    ਵਾਪਸ;

  ਸਵੈਚਾਲਿਤ *scratchpad = plan_ਅੰਕੜੇ->scratchpad.get_acquire ();
  ਜੇ (scratchpad && plan_ਅੰਕੜੇ->scratchpad.cmpexch (scratchpad, ਖਾਲੀ_ਸੂਚਕ))
    kbts_DestroyShapeScratchpad (scratchpad);

  ਜੇ (plan_ਅੰਕੜੇ->config)
    kbts_DestroyShapeConfig (plan_ਅੰਕੜੇ->config);

  hb_free (plan_ਅੰਕੜੇ);
}

ਸਥਿਰ ਖਾਲੀ
hb_kbts_plan_props_ਮੰਜ਼ਿਲ_script_language (ਅਚਲ hb_segment_properties_t &props,
				       kbts_script *kb_script,
				       kbts_language *kb_language)
{
  hb_ਟੈਗ_t scripts[HB_OT_MAX_ਟੈਗ_ਸੂਚੀ_ਪ੍ਰਤੀ_SCRIPT];
  hb_ਟੈਗ_t language;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ script_ਗਿਣਤੀ = ARRAY_ਲੰਮਾਈ (scripts);
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ language_ਗਿਣਤੀ = 1;

  *kb_script = KBTS_SCRIPT_DONT_KNOW;
  *kb_language = KBTS_LANGUAGE_DEFAULT;

  hb_ot_ਟੈਗ_ਸੂਚੀ_from_script_and_language (props.script, props.language,
				       &script_ਗਿਣਤੀ, scripts,
				       &language_ਗਿਣਤੀ, &language);

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < script_ਗਿਣਤੀ && scripts[i] != HB_ਟੈਗ_NONE; ++i)
  {
    *kb_script = kbts_Script_ਟੈਗ_ਮੰਜ਼ਿਲ_Script (hb_uint32_ਅਦਲਾ_ਬਦਲੀ (scripts[i]));
    ਜੇ (*kb_script != KBTS_SCRIPT_DONT_KNOW)
      ਰੋਕੋ;
  }

  ਜੇ (language_ਗਿਣਤੀ)
    *kb_language = (kbts_language) hb_uint32_ਅਦਲਾ_ਬਦਲੀ (language);
}

ਸਥਿਰ hb_kbts_shape_plan_ਅੰਕੜੇ_t *
hb_kbts_get_shape_plan_ਅੰਕੜੇ (hb_shape_plan_t *shape_plan,
			     kbts_font *kb_font)
{
retry:
  ਸਵੈਚਾਲਿਤ *plan_ਅੰਕੜੇ = (hb_kbts_shape_plan_ਅੰਕੜੇ_t *)
    hb_shape_plan_get_user_ਅੰਕੜੇ (shape_plan, &hb_kbts_shape_plan_ਅੰਕੜੇ_ਕੁੰਜੀ);
  ਜੇ (plan_ਅੰਕੜੇ)
    ਵਾਪਸ plan_ਅੰਕੜੇ;

  kbts_script kb_script;
  kbts_language kb_language;
  hb_kbts_plan_props_ਮੰਜ਼ਿਲ_script_language (shape_plan->key.props, &kb_script, &kb_language);

  kbts_shape_config *config = kbts_CreateShapeConfig (kb_font, kb_script, kb_language, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
  ਜੇ (unlikely (!config))
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  plan_ਅੰਕੜੇ = (hb_kbts_shape_plan_ਅੰਕੜੇ_t *) hb_calloc (1, ਆਕਾਰ (*plan_ਅੰਕੜੇ));
  ਜੇ (unlikely (!plan_ਅੰਕੜੇ))
  {
    kbts_DestroyShapeConfig (config);
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }

  plan_ਅੰਕੜੇ->config = config;
  plan_ਅੰਕੜੇ->scratchpad.init (ਖਾਲੀ_ਸੂਚਕ);

  ਜੇ (!hb_shape_plan_ਸੈੱਟ_user_ਅੰਕੜੇ (shape_plan,
				    &hb_kbts_shape_plan_ਅੰਕੜੇ_ਕੁੰਜੀ,
				    plan_ਅੰਕੜੇ,
				    hb_kbts_shape_plan_ਅੰਕੜੇ_destroy,
				    ਝੂਠ))
  {
    hb_kbts_shape_plan_ਅੰਕੜੇ_destroy (plan_ਅੰਕੜੇ);
    ਜਾਓ retry;
  }

  ਵਾਪਸ plan_ਅੰਕੜੇ;
}

ਸਥਿਰ kbts_shape_scratchpad *
hb_kbts_acquire_shape_scratchpad (hb_kbts_shape_plan_ਅੰਕੜੇ_t *plan_ਅੰਕੜੇ)
{
  ਸਵੈਚਾਲਿਤ *scratchpad = plan_ਅੰਕੜੇ->scratchpad.get_acquire ();
  ਜੇ (!scratchpad || unlikely (!plan_ਅੰਕੜੇ->scratchpad.cmpexch (scratchpad, ਖਾਲੀ_ਸੂਚਕ)))
    scratchpad = kbts_CreateShapeScratchpad (plan_ਅੰਕੜੇ->config, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);

  ਵਾਪਸ scratchpad;
}

ਸਥਿਰ ਖਾਲੀ
hb_kbts_release_shape_scratchpad (hb_kbts_shape_plan_ਅੰਕੜੇ_t *plan_ਅੰਕੜੇ,
				  kbts_shape_scratchpad *scratchpad)
{
  ਜੇ (!scratchpad)
    ਵਾਪਸ;

  ਜੇ (!plan_ਅੰਕੜੇ->scratchpad.cmpexch (ਖਾਲੀ_ਸੂਚਕ, scratchpad))
    kbts_DestroyShapeScratchpad (scratchpad);
}


hb_kbts_face_ਅੰਕੜੇ_t *
hb_kbts_shaper_face_ਅੰਕੜੇ_create (hb_face_t *face)
{
  hb_blob_t *blob = hb_face_reference_blob (face);

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ blob_ਲੰਮਾਈ;
  ਅਚਲ ਅੱਖਰ *blob_ਅੰਕੜੇ = hb_blob_get_ਅੰਕੜੇ (blob, &blob_ਲੰਮਾਈ);
  ਜੇ (unlikely (!blob_ਲੰਮਾਈ))
  {
    DEBUG_ਸੁਨੇਹਾ (KBTS, blob, "Empty blob");
    hb_blob_destroy (blob);
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }

  kbts_font *kb_font = (kbts_font *) hb_calloc (1, ਆਕਾਰ (kbts_font));
  ਜੇ (unlikely (!kb_font))
  {
    hb_blob_destroy (blob);
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }

  *kb_font = kbts_Font_From_ਸਮ੍ਰਿਤੀ((ਖਾਲੀ *)blob_ਅੰਕੜੇ, blob_ਲੰਮਾਈ, face->ਸੂਚਕਾਂਕ_2, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
  hb_blob_destroy (blob);
  blob = ਖਾਲੀ_ਸੂਚਕ;

  ਜੇ (unlikely (!kbts_FontIsValid (kb_font)))
  {
    DEBUG_ਸੁਨੇਹਾ (KBTS, face, "Failed create font from data");
    kbts_FreeFont (kb_font);
    hb_free (kb_font);
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }

  ਵਾਪਸ (hb_kbts_face_ਅੰਕੜੇ_t *) kb_font;
}

ਖਾਲੀ
hb_kbts_shaper_face_ਅੰਕੜੇ_destroy (hb_kbts_face_ਅੰਕੜੇ_t *ਅੰਕੜੇ)
{
  kbts_font *font = (kbts_font *) ਅੰਕੜੇ;

  ਪ੍ਰਤਿਜ੍ਞਾ (kbts_FontIsValid (font));

  kbts_FreeFont (font);
  hb_free (font);
}

hb_kbts_font_ਅੰਕੜੇ_t *
hb_kbts_shaper_font_ਅੰਕੜੇ_create (hb_font_t *font)
{
  ਵਾਪਸ (hb_kbts_font_ਅੰਕੜੇ_t *) HB_SHAPER_ਅੰਕੜੇ_SUCCEEDED;
}

ਖਾਲੀ
hb_kbts_shaper_font_ਅੰਕੜੇ_destroy (hb_kbts_font_ਅੰਕੜੇ_t *ਅੰਕੜੇ)
{
}

hb_bool_t
_hb_kbts_shape (hb_shape_plan_t    *shape_plan,
		hb_font_t          *font,
		hb_ਬਫਰ_t        *ਬਫਰ,
		ਅਚਲ hb_feature_t *features,
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ        ਅੰਕ_features)
{
  hb_face_t *face = font->face;
  kbts_font *kb_font = (kbts_font *) (ਅਚਲ ਖਾਲੀ *) face->ਅੰਕੜੇ.kbts;

  kbts_direction kb_direction;
  ਚੋਣ (ਬਫਰ->props.direction)
  {
    ਹਾਲਤ HB_DIRECTION_LTR: kb_direction = KBTS_DIRECTION_LTR; ਰੋਕੋ;
    ਹਾਲਤ HB_DIRECTION_RTL: kb_direction = KBTS_DIRECTION_RTL; ਰੋਕੋ;
    ਹਾਲਤ HB_DIRECTION_TTB:
    ਹਾਲਤ HB_DIRECTION_BTT:
      DEBUG_ਸੁਨੇਹਾ (KBTS, face, "Vertical direction is not supported");
      ਵਾਪਸ ਝੂਠ;
    ਮੂਲ:
    ਹਾਲਤ HB_DIRECTION_INVALID:
      DEBUG_ਸੁਨੇਹਾ (KBTS, face, "Invalid direction");
      ਵਾਪਸ ਝੂਠ;
  }

  kbts_glyph_storage kb_glyph_storage;
  ਜੇ (unlikely (!kbts_InitializeGlyphStorage (&kb_glyph_storage, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ)))
    ਵਾਪਸ ਝੂਠ;

  hb_kbts_shape_plan_ਅੰਕੜੇ_t *plan_ਅੰਕੜੇ = hb_kbts_get_shape_plan_ਅੰਕੜੇ (shape_plan, kb_font);
  ਜੇ (unlikely (!plan_ਅੰਕੜੇ))
  {
    kbts_FreeAllGlyphs (&kb_glyph_storage);
    ਵਾਪਸ ਝੂਠ;
  }

  kbts_shape_config *kb_shape_config = ਖਾਲੀ_ਸੂਚਕ;
  kbts_shape_scratchpad *kb_shape_scratchpad = ਖਾਲੀ_ਸੂਚਕ;
  uint32_t glyph_ਗਿਣਤੀ = 0;
  kbts_glyph *kb_glyph = ਖਾਲੀ_ਸੂਚਕ;
  kbts_glyph_iterator kb_output;
  hb_glyph_info_t *info;
  hb_glyph_position_t *pos;
  hb_bool_t res = ਝੂਠ;

  kb_shape_config = plan_ਅੰਕੜੇ->config;
  kb_shape_scratchpad = hb_kbts_acquire_shape_scratchpad (plan_ਅੰਕੜੇ);
  ਜੇ (unlikely (!kb_shape_scratchpad))
    ਜਾਓ ਪੂਰਾ;

  ਲਈ (size_t i = 0; i < ਬਫਰ->len; ++i)
  {
    kbts_glyph_config *kb_config = ਖਾਲੀ_ਸੂਚਕ;
    ਜੇ (ਅੰਕ_features)
    {
      hb_ਸਦਿਸ਼_t<kbts_feature_override> kb_features;
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ j = 0; j < ਅੰਕ_features; ++j)
      {
        ਜੇ (hb_ਵਿੱਚ_range<size_t> (i, features[j].ਸ਼ੁਰੂ, features[j].ਸਿਰਾ))
	  kb_features.push<kbts_feature_override>({ hb_uint32_ਅਦਲਾ_ਬਦਲੀ (features[j].ਟੈਗ_2), (ਪੂਰਨ)features[j].value });
      }
      ਜੇ (kb_features)
        kb_config = kbts_CreateGlyphConfig (kb_shape_config, kb_features.arrayZ, kb_features.ਲੰਮਾਈ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    }
    kbts_ਧੱਕੋ_Glyph (&kb_glyph_storage, kb_font, ਬਫਰ->info[i].codepoint, kb_config, ਬਫਰ->info[i].cluster);
  }

  res = kbts_Shape_ਸਿੱਧਾ (kb_shape_scratchpad, &kb_glyph_storage, kb_direction, &kb_output) == KBTS_SHAPE_ERROR_NONE;
  ਜੇ (unlikely (!res))
    ਜਾਓ ਪੂਰਾ;

  ਲਈ (ਸਵੈਚਾਲਿਤ it = kb_output; kbts_Glyph_Iterator_ਅਗਲਾ (&it, &kb_glyph); )
    glyph_ਗਿਣਤੀ += 1;

  hb_ਬਫਰ_ਸੈੱਟ_content_type (ਬਫਰ, HB_ਬਫਰ_CONTENT_TYPE_GLYPHS);
  hb_ਬਫਰ_ਸੈੱਟ_ਲੰਮਾਈ (ਬਫਰ, glyph_ਗਿਣਤੀ);

  ਬਫਰ->clear_positions ();

  info = ਬਫਰ->info;
  pos = ਬਫਰ->pos;

  ਲਈ (ਸਵੈਚਾਲਿਤ it = kb_output; kbts_Glyph_Iterator_ਅਗਲਾ (&it, &kb_glyph); info++, pos++)
  {
    info->codepoint = kb_glyph->Id;
    info->cluster = kb_glyph->User_Id_Or_Codepoint_ਸੂਚਕਾਂਕ;
    pos->x_advance = font->em_scalef_x (kb_glyph->AdvanceX);
    pos->y_advance = font->em_scalef_y (kb_glyph->AdvanceY);
    pos->x_ਵਿੱਥ_2 = font->em_scalef_x (kb_glyph->ਵਿੱਥ_X);
    pos->y_ਵਿੱਥ_2 = font->em_scalef_y (kb_glyph->ਵਿੱਥ_Y);
  }

ਪੂਰਾ:
  ਜੇ (likely (kb_shape_scratchpad))
    hb_kbts_release_shape_scratchpad (plan_ਅੰਕੜੇ, kb_shape_scratchpad);
  ਜਦੋਂ (kbts_Glyph_Iterator_ਅਗਲਾ (&kb_output, &kb_glyph))
    kbts_DestroyGlyphConfig (kb_glyph->Config);
  kbts_FreeAllGlyphs (&kb_glyph_storage);

  ਬਫਰ->clear_glyph_flags ();
  ਬਫਰ->unsafe_ਮੰਜ਼ਿਲ_break ();

  ਵਾਪਸ res;
}

#ਜੇ_ਅੰਤ
