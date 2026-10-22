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

#അന്തര്ഭാവ "hb.hh"

#ifdef HAVE_KBTS

#അന്തര്ഭാവ "hb-shaper-impl.hh"

#define KB_TEXT_SHAPE_IMPLEMENTATION
#define KB_TEXT_SHAPE_STATIC
#define KB_TEXT_SHAPE_NO_CRT
#define KBTS_MALLOC(a, b) hb_malloc(b)
#define KBTS_FREE(a, b) hb_free(b)
#define KBTS_MEMCPY hb_memcpy
#define KBTS_MEMSET hb_memset

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
#pragma GCC diagnostic ignored "-Wunused-function"
#pragma GCC diagnostic ignored "-Wextra-semi-stmt"
#അന്തര്ഭാവ "kb_text_shape.h"
#pragma GCC diagnostic എടുക്കുക

സ്ഥിതം hb_user_data_താക്കോൽ_t hb_kbts_shape_plan_data_താക്കോൽ = {0};

ഘടന hb_kbts_shape_plan_data_t
{
  kbts_shape_config *config = nullptr;
  mutable hb_atomic_t<kbts_shape_scratchpad *> scratchpad;
};

സ്ഥിതം ശൂന്യം
hb_kbts_shape_plan_data_destroy (ശൂന്യം *data)
{
  auto *plan_data = (hb_kbts_shape_plan_data_t *) data;
  എങ്കിൽ (!plan_data)
    തിരികെ;

  auto *scratchpad = plan_data->scratchpad.get_acquire ();
  എങ്കിൽ (scratchpad && plan_data->scratchpad.cmpexch (scratchpad, nullptr))
    kbts_DestroyShapeScratchpad (scratchpad);

  എങ്കിൽ (plan_data->config)
    kbts_DestroyShapeConfig (plan_data->config);

  hb_free (plan_data);
}

സ്ഥിതം ശൂന്യം
hb_kbts_plan_props_to_script_language (സ്ഥിരം hb_segment_properties_t &props,
				       kbts_script *kb_script,
				       kbts_language *kb_language)
{
  hb_tag_t scripts[HB_OT_MAX_TAGS_PER_SCRIPT];
  hb_tag_t language;
  unsigned പൂർണ്ണസംഖ്യ script_എണ്ണം = ARRAY_LENGTH (scripts);
  unsigned പൂർണ്ണസംഖ്യ language_എണ്ണം = 1;

  *kb_script = KBTS_SCRIPT_DONT_KNOW;
  *kb_language = KBTS_LANGUAGE_DEFAULT;

  hb_ot_tags_from_script_and_language (props.script, props.language,
				       &script_എണ്ണം, scripts,
				       &language_എണ്ണം, &language);

  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < script_എണ്ണം && scripts[i] != HB_TAG_NONE; ++i)
  {
    *kb_script = kbts_ScriptTagToScript (hb_uint32_swap (scripts[i]));
    എങ്കിൽ (*kb_script != KBTS_SCRIPT_DONT_KNOW)
      നിർത്തുക;
  }

  എങ്കിൽ (language_എണ്ണം)
    *kb_language = (kbts_language) hb_uint32_swap (language);
}

സ്ഥിതം hb_kbts_shape_plan_data_t *
hb_kbts_get_shape_plan_data (hb_shape_plan_t *shape_plan,
			     kbts_font *kb_font)
{
retry:
  auto *plan_data = (hb_kbts_shape_plan_data_t *)
    hb_shape_plan_get_user_data (shape_plan, &hb_kbts_shape_plan_data_താക്കോൽ);
  എങ്കിൽ (plan_data)
    തിരികെ plan_data;

  kbts_script kb_script;
  kbts_language kb_language;
  hb_kbts_plan_props_to_script_language (shape_plan->key.props, &kb_script, &kb_language);

  kbts_shape_config *config = kbts_CreateShapeConfig (kb_font, kb_script, kb_language, nullptr, nullptr);
  എങ്കിൽ (unlikely (!config))
    തിരികെ nullptr;

  plan_data = (hb_kbts_shape_plan_data_t *) hb_calloc (1, വലുപ്പം (*plan_data));
  എങ്കിൽ (unlikely (!plan_data))
  {
    kbts_DestroyShapeConfig (config);
    തിരികെ nullptr;
  }

  plan_data->config = config;
  plan_data->scratchpad.init (nullptr);

  എങ്കിൽ (!hb_shape_plan_set_user_data (shape_plan,
				    &hb_kbts_shape_plan_data_താക്കോൽ,
				    plan_data,
				    hb_kbts_shape_plan_data_destroy,
				    അസത്യം))
  {
    hb_kbts_shape_plan_data_destroy (plan_data);
    പോകുക retry;
  }

  തിരികെ plan_data;
}

സ്ഥിതം kbts_shape_scratchpad *
hb_kbts_acquire_shape_scratchpad (hb_kbts_shape_plan_data_t *plan_data)
{
  auto *scratchpad = plan_data->scratchpad.get_acquire ();
  എങ്കിൽ (!scratchpad || unlikely (!plan_data->scratchpad.cmpexch (scratchpad, nullptr)))
    scratchpad = kbts_CreateShapeScratchpad (plan_data->config, nullptr, nullptr);

  തിരികെ scratchpad;
}

സ്ഥിതം ശൂന്യം
hb_kbts_release_shape_scratchpad (hb_kbts_shape_plan_data_t *plan_data,
				  kbts_shape_scratchpad *scratchpad)
{
  എങ്കിൽ (!scratchpad)
    തിരികെ;

  എങ്കിൽ (!plan_data->scratchpad.cmpexch (nullptr, scratchpad))
    kbts_DestroyShapeScratchpad (scratchpad);
}


hb_kbts_face_data_t *
_hb_kbts_shaper_face_data_create (hb_face_t *face)
{
  hb_blob_t *blob = hb_face_reference_blob (face);

  unsigned പൂർണ്ണസംഖ്യ blob_length;
  സ്ഥിരം അക്ഷരം *blob_data = hb_blob_get_data (blob, &blob_length);
  എങ്കിൽ (unlikely (!blob_length))
  {
    DEBUG_സന്ദേശം (KBTS, blob, "Empty blob");
    hb_blob_destroy (blob);
    തിരികെ nullptr;
  }

  kbts_font *kb_font = (kbts_font *) hb_calloc (1, വലുപ്പം (kbts_font));
  എങ്കിൽ (unlikely (!kb_font))
  {
    hb_blob_destroy (blob);
    തിരികെ nullptr;
  }

  *kb_font = kbts_FontFromMemory((ശൂന്യം *)blob_data, blob_length, face->index, nullptr, nullptr);
  hb_blob_destroy (blob);
  blob = nullptr;

  എങ്കിൽ (unlikely (!kbts_FontIsValid (kb_font)))
  {
    DEBUG_സന്ദേശം (KBTS, face, "Failed create font from data");
    kbts_FreeFont (kb_font);
    hb_free (kb_font);
    തിരികെ nullptr;
  }

  തിരികെ (hb_kbts_face_data_t *) kb_font;
}

ശൂന്യം
_hb_kbts_shaper_face_data_destroy (hb_kbts_face_data_t *data)
{
  kbts_font *font = (kbts_font *) data;

  പ്രതിജ്ഞാ (kbts_FontIsValid (font));

  kbts_FreeFont (font);
  hb_free (font);
}

hb_kbts_font_data_t *
_hb_kbts_shaper_font_data_create (hb_font_t *font)
{
  തിരികെ (hb_kbts_font_data_t *) HB_SHAPER_DATA_SUCCEEDED;
}

ശൂന്യം
_hb_kbts_shaper_font_data_destroy (hb_kbts_font_data_t *data)
{
}

hb_bool_t
_hb_kbts_shape (hb_shape_plan_t    *shape_plan,
		hb_font_t          *font,
		hb_buffer_t        *buffer,
		സ്ഥിരം hb_feature_t *features,
		unsigned പൂർണ്ണസംഖ്യ        അക്കം_features)
{
  hb_face_t *face = font->face;
  kbts_font *kb_font = (kbts_font *) (സ്ഥിരം ശൂന്യം *) face->data.kbts;

  kbts_direction kb_direction;
  തിരഞ്ഞെടുപ്പ് (buffer->props.direction)
  {
    അവസ്ഥ HB_DIRECTION_LTR: kb_direction = KBTS_DIRECTION_LTR; നിർത്തുക;
    അവസ്ഥ HB_DIRECTION_RTL: kb_direction = KBTS_DIRECTION_RTL; നിർത്തുക;
    അവസ്ഥ HB_DIRECTION_TTB:
    അവസ്ഥ HB_DIRECTION_BTT:
      DEBUG_സന്ദേശം (KBTS, face, "Vertical direction is not supported");
      തിരികെ അസത്യം;
    സ്ഥിരസ്ഥിതി:
    അവസ്ഥ HB_DIRECTION_INVALID:
      DEBUG_സന്ദേശം (KBTS, face, "Invalid direction");
      തിരികെ അസത്യം;
  }

  kbts_glyph_storage kb_glyph_storage;
  എങ്കിൽ (unlikely (!kbts_InitializeGlyphStorage (&kb_glyph_storage, nullptr, nullptr)))
    തിരികെ അസത്യം;

  hb_kbts_shape_plan_data_t *plan_data = hb_kbts_get_shape_plan_data (shape_plan, kb_font);
  എങ്കിൽ (unlikely (!plan_data))
  {
    kbts_FreeAllGlyphs (&kb_glyph_storage);
    തിരികെ അസത്യം;
  }

  kbts_shape_config *kb_shape_config = nullptr;
  kbts_shape_scratchpad *kb_shape_scratchpad = nullptr;
  uint32_t glyph_എണ്ണം = 0;
  kbts_glyph *kb_glyph = nullptr;
  kbts_glyph_iterator kb_output;
  hb_glyph_info_t *info;
  hb_glyph_position_t *pos;
  hb_bool_t res = അസത്യം;

  kb_shape_config = plan_data->config;
  kb_shape_scratchpad = hb_kbts_acquire_shape_scratchpad (plan_data);
  എങ്കിൽ (unlikely (!kb_shape_scratchpad))
    പോകുക done;

  വേണ്ടി (size_t i = 0; i < buffer->len; ++i)
  {
    kbts_glyph_config *kb_config = nullptr;
    എങ്കിൽ (അക്കം_features)
    {
      hb_vector_t<kbts_feature_override> kb_features;
      വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ j = 0; j < അക്കം_features; ++j)
      {
        എങ്കിൽ (hb_in_range<size_t> (i, features[j].start, features[j].end))
	  kb_features.push<kbts_feature_override>({ hb_uint32_swap (features[j].tag), (പൂർണ്ണസംഖ്യ)features[j].value });
      }
      എങ്കിൽ (kb_features)
        kb_config = kbts_CreateGlyphConfig (kb_shape_config, kb_features.arrayZ, kb_features.length, nullptr, nullptr);
    }
    kbts_തള്ളുക_Glyph (&kb_glyph_storage, kb_font, buffer->info[i].codepoint, kb_config, buffer->info[i].cluster);
  }

  res = kbts_ShapeDirect (kb_shape_scratchpad, &kb_glyph_storage, kb_direction, &kb_output) == KBTS_SHAPE_ERROR_NONE;
  എങ്കിൽ (unlikely (!res))
    പോകുക done;

  വേണ്ടി (auto it = kb_output; kbts_GlyphIteratorNext (&it, &kb_glyph); )
    glyph_എണ്ണം += 1;

  hb_buffer_set_content_type (buffer, HB_BUFFER_CONTENT_TYPE_GLYPHS);
  hb_buffer_set_length (buffer, glyph_എണ്ണം);

  buffer->clear_positions ();

  info = buffer->info;
  pos = buffer->pos;

  വേണ്ടി (auto it = kb_output; kbts_GlyphIteratorNext (&it, &kb_glyph); info++, pos++)
  {
    info->codepoint = kb_glyph->Id;
    info->cluster = kb_glyph->UserIdOrCodepointIndex;
    pos->x_advance = font->em_scalef_x (kb_glyph->AdvanceX);
    pos->y_advance = font->em_scalef_y (kb_glyph->AdvanceY);
    pos->x_offset = font->em_scalef_x (kb_glyph->OffsetX);
    pos->y_offset = font->em_scalef_y (kb_glyph->OffsetY);
  }

done:
  എങ്കിൽ (likely (kb_shape_scratchpad))
    hb_kbts_release_shape_scratchpad (plan_data, kb_shape_scratchpad);
  ആകുമ്പോൾ (kbts_GlyphIteratorNext (&kb_output, &kb_glyph))
    kbts_DestroyGlyphConfig (kb_glyph->Config);
  kbts_FreeAllGlyphs (&kb_glyph_storage);

  buffer->clear_glyph_flags ();
  buffer->unsafe_to_break ();

  തിരികെ res;
}

#endif
