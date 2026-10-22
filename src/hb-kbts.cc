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

#ଅନ୍ତର୍ଭାଵ "hb.hh"

#ifdef HAVE_KBTS

#ଅନ୍ତର୍ଭାଵ "hb-shaper-impl.hh"

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
#ଅନ୍ତର୍ଭାଵ "kb_text_shape.h"
#pragma GCC diagnostic କାଢ଼

ସ୍ଥିର hb_user_data_ଚାବି_t hb_kbts_shape_plan_data_ଚାବି = {0};

ସଂରଚନା hb_kbts_shape_plan_data_t
{
  kbts_shape_config *config = nullptr;
  mutable hb_atomic_t<kbts_shape_scratchpad *> scratchpad;
};

ସ୍ଥିର ଶୂନ୍ୟ
hb_kbts_shape_plan_data_destroy (ଶୂନ୍ୟ *data)
{
  auto *plan_data = (hb_kbts_shape_plan_data_t *) data;
  ଯଦି (!plan_data)
    ଫେରନ୍ତୁ;

  auto *scratchpad = plan_data->scratchpad.get_acquire ();
  ଯଦି (scratchpad && plan_data->scratchpad.cmpexch (scratchpad, nullptr))
    kbts_DestroyShapeScratchpad (scratchpad);

  ଯଦି (plan_data->config)
    kbts_DestroyShapeConfig (plan_data->config);

  hb_free (plan_data);
}

ସ୍ଥିର ଶୂନ୍ୟ
hb_kbts_plan_props_to_script_language (ଅଚଳ hb_segment_properties_t &props,
				       kbts_script *kb_script,
				       kbts_language *kb_language)
{
  hb_tag_t scripts[HB_OT_MAX_TAGS_PER_SCRIPT];
  hb_tag_t language;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା script_ଗଣନା = ARRAY_LENGTH (scripts);
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା language_ଗଣନା = 1;

  *kb_script = KBTS_SCRIPT_DONT_KNOW;
  *kb_language = KBTS_LANGUAGE_DEFAULT;

  hb_ot_tags_from_script_and_language (props.script, props.language,
				       &script_ଗଣନା, scripts,
				       &language_ଗଣନା, &language);

  ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < script_ଗଣନା && scripts[i] != HB_TAG_NONE; ++i)
  {
    *kb_script = kbts_ScriptTagToScript (hb_uint32_swap (scripts[i]));
    ଯଦି (*kb_script != KBTS_SCRIPT_DONT_KNOW)
      ଭାଙ୍ଗ;
  }

  ଯଦି (language_ଗଣନା)
    *kb_language = (kbts_language) hb_uint32_swap (language);
}

ସ୍ଥିର hb_kbts_shape_plan_data_t *
hb_kbts_get_shape_plan_data (hb_shape_plan_t *shape_plan,
			     kbts_font *kb_font)
{
retry:
  auto *plan_data = (hb_kbts_shape_plan_data_t *)
    hb_shape_plan_get_user_data (shape_plan, &hb_kbts_shape_plan_data_ଚାବି);
  ଯଦି (plan_data)
    ଫେରନ୍ତୁ plan_data;

  kbts_script kb_script;
  kbts_language kb_language;
  hb_kbts_plan_props_to_script_language (shape_plan->key.props, &kb_script, &kb_language);

  kbts_shape_config *config = kbts_CreateShapeConfig (kb_font, kb_script, kb_language, nullptr, nullptr);
  ଯଦି (unlikely (!config))
    ଫେରନ୍ତୁ nullptr;

  plan_data = (hb_kbts_shape_plan_data_t *) hb_calloc (1, ଆକାର (*plan_data));
  ଯଦି (unlikely (!plan_data))
  {
    kbts_DestroyShapeConfig (config);
    ଫେରନ୍ତୁ nullptr;
  }

  plan_data->config = config;
  plan_data->scratchpad.init (nullptr);

  ଯଦି (!hb_shape_plan_set_user_data (shape_plan,
				    &hb_kbts_shape_plan_data_ଚାବି,
				    plan_data,
				    hb_kbts_shape_plan_data_destroy,
				    ମିଥ୍ୟା))
  {
    hb_kbts_shape_plan_data_destroy (plan_data);
    ଯାଅ retry;
  }

  ଫେରନ୍ତୁ plan_data;
}

ସ୍ଥିର kbts_shape_scratchpad *
hb_kbts_acquire_shape_scratchpad (hb_kbts_shape_plan_data_t *plan_data)
{
  auto *scratchpad = plan_data->scratchpad.get_acquire ();
  ଯଦି (!scratchpad || unlikely (!plan_data->scratchpad.cmpexch (scratchpad, nullptr)))
    scratchpad = kbts_CreateShapeScratchpad (plan_data->config, nullptr, nullptr);

  ଫେରନ୍ତୁ scratchpad;
}

ସ୍ଥିର ଶୂନ୍ୟ
hb_kbts_release_shape_scratchpad (hb_kbts_shape_plan_data_t *plan_data,
				  kbts_shape_scratchpad *scratchpad)
{
  ଯଦି (!scratchpad)
    ଫେରନ୍ତୁ;

  ଯଦି (!plan_data->scratchpad.cmpexch (nullptr, scratchpad))
    kbts_DestroyShapeScratchpad (scratchpad);
}


hb_kbts_face_data_t *
_hb_kbts_shaper_face_data_create (hb_face_t *face)
{
  hb_blob_t *blob = hb_face_reference_blob (face);

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା blob_length;
  ଅଚଳ ଅକ୍ଷର *blob_data = hb_blob_get_data (blob, &blob_length);
  ଯଦି (unlikely (!blob_length))
  {
    DEBUG_ସନ୍ଦେଶ (KBTS, blob, "Empty blob");
    hb_blob_destroy (blob);
    ଫେରନ୍ତୁ nullptr;
  }

  kbts_font *kb_font = (kbts_font *) hb_calloc (1, ଆକାର (kbts_font));
  ଯଦି (unlikely (!kb_font))
  {
    hb_blob_destroy (blob);
    ଫେରନ୍ତୁ nullptr;
  }

  *kb_font = kbts_FontFromMemory((ଶୂନ୍ୟ *)blob_data, blob_length, face->index, nullptr, nullptr);
  hb_blob_destroy (blob);
  blob = nullptr;

  ଯଦି (unlikely (!kbts_FontIsValid (kb_font)))
  {
    DEBUG_ସନ୍ଦେଶ (KBTS, face, "Failed create font from data");
    kbts_FreeFont (kb_font);
    hb_free (kb_font);
    ଫେରନ୍ତୁ nullptr;
  }

  ଫେରନ୍ତୁ (hb_kbts_face_data_t *) kb_font;
}

ଶୂନ୍ୟ
_hb_kbts_shaper_face_data_destroy (hb_kbts_face_data_t *data)
{
  kbts_font *font = (kbts_font *) data;

  ପ୍ରତିଜ୍ଞା (kbts_FontIsValid (font));

  kbts_FreeFont (font);
  hb_free (font);
}

hb_kbts_font_data_t *
_hb_kbts_shaper_font_data_create (hb_font_t *font)
{
  ଫେରନ୍ତୁ (hb_kbts_font_data_t *) HB_SHAPER_DATA_SUCCEEDED;
}

ଶୂନ୍ୟ
_hb_kbts_shaper_font_data_destroy (hb_kbts_font_data_t *data)
{
}

hb_bool_t
_hb_kbts_shape (hb_shape_plan_t    *shape_plan,
		hb_font_t          *font,
		hb_buffer_t        *buffer,
		ଅଚଳ hb_feature_t *features,
		unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା        ଅଙ୍କ_features)
{
  hb_face_t *face = font->face;
  kbts_font *kb_font = (kbts_font *) (ଅଚଳ ଶୂନ୍ୟ *) face->data.kbts;

  kbts_direction kb_direction;
  ବାଛ (buffer->props.direction)
  {
    ସ୍ଥିତି HB_DIRECTION_LTR: kb_direction = KBTS_DIRECTION_LTR; ଭାଙ୍ଗ;
    ସ୍ଥିତି HB_DIRECTION_RTL: kb_direction = KBTS_DIRECTION_RTL; ଭାଙ୍ଗ;
    ସ୍ଥିତି HB_DIRECTION_TTB:
    ସ୍ଥିତି HB_DIRECTION_BTT:
      DEBUG_ସନ୍ଦେଶ (KBTS, face, "Vertical direction is not supported");
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
    ସ୍ଥିତି HB_DIRECTION_INVALID:
      DEBUG_ସନ୍ଦେଶ (KBTS, face, "Invalid direction");
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }

  kbts_glyph_storage kb_glyph_storage;
  ଯଦି (unlikely (!kbts_InitializeGlyphStorage (&kb_glyph_storage, nullptr, nullptr)))
    ଫେରନ୍ତୁ ମିଥ୍ୟା;

  hb_kbts_shape_plan_data_t *plan_data = hb_kbts_get_shape_plan_data (shape_plan, kb_font);
  ଯଦି (unlikely (!plan_data))
  {
    kbts_FreeAllGlyphs (&kb_glyph_storage);
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }

  kbts_shape_config *kb_shape_config = nullptr;
  kbts_shape_scratchpad *kb_shape_scratchpad = nullptr;
  uint32_t glyph_ଗଣନା = 0;
  kbts_glyph *kb_glyph = nullptr;
  kbts_glyph_iterator kb_output;
  hb_glyph_info_t *info;
  hb_glyph_position_t *pos;
  hb_bool_t res = ମିଥ୍ୟା;

  kb_shape_config = plan_data->config;
  kb_shape_scratchpad = hb_kbts_acquire_shape_scratchpad (plan_data);
  ଯଦି (unlikely (!kb_shape_scratchpad))
    ଯାଅ done;

  ପାଇଁ (size_t i = 0; i < buffer->len; ++i)
  {
    kbts_glyph_config *kb_config = nullptr;
    ଯଦି (ଅଙ୍କ_features)
    {
      hb_vector_t<kbts_feature_override> kb_features;
      ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < ଅଙ୍କ_features; ++j)
      {
        ଯଦି (hb_in_range<size_t> (i, features[j].start, features[j].end))
	  kb_features.push<kbts_feature_override>({ hb_uint32_swap (features[j].tag), (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)features[j].value });
      }
      ଯଦି (kb_features)
        kb_config = kbts_CreateGlyphConfig (kb_shape_config, kb_features.arrayZ, kb_features.length, nullptr, nullptr);
    }
    kbts_ଠେଲ_Glyph (&kb_glyph_storage, kb_font, buffer->info[i].codepoint, kb_config, buffer->info[i].cluster);
  }

  res = kbts_ShapeDirect (kb_shape_scratchpad, &kb_glyph_storage, kb_direction, &kb_output) == KBTS_SHAPE_ERROR_NONE;
  ଯଦି (unlikely (!res))
    ଯାଅ done;

  ପାଇଁ (auto it = kb_output; kbts_GlyphIteratorNext (&it, &kb_glyph); )
    glyph_ଗଣନା += 1;

  hb_buffer_set_content_type (buffer, HB_BUFFER_CONTENT_TYPE_GLYPHS);
  hb_buffer_set_length (buffer, glyph_ଗଣନା);

  buffer->clear_positions ();

  info = buffer->info;
  pos = buffer->pos;

  ପାଇଁ (auto it = kb_output; kbts_GlyphIteratorNext (&it, &kb_glyph); info++, pos++)
  {
    info->codepoint = kb_glyph->Id;
    info->cluster = kb_glyph->UserIdOrCodepointIndex;
    pos->x_advance = font->em_scalef_x (kb_glyph->AdvanceX);
    pos->y_advance = font->em_scalef_y (kb_glyph->AdvanceY);
    pos->x_offset = font->em_scalef_x (kb_glyph->OffsetX);
    pos->y_offset = font->em_scalef_y (kb_glyph->OffsetY);
  }

done:
  ଯଦି (likely (kb_shape_scratchpad))
    hb_kbts_release_shape_scratchpad (plan_data, kb_shape_scratchpad);
  ଯେତେବେଳେ (kbts_GlyphIteratorNext (&kb_output, &kb_glyph))
    kbts_DestroyGlyphConfig (kb_glyph->Config);
  kbts_FreeAllGlyphs (&kb_glyph_storage);

  buffer->clear_glyph_flags ();
  buffer->unsafe_to_break ();

  ଫେରନ୍ତୁ res;
}

#endif
