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

#अन्तर्भाव "hb.hh"

#ifdef HAVE_KBTS

#अन्तर्भाव "hb-shaper-impl.hh"

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
#अन्तर्भाव "kb_text_shape.h"
#pragma GCC diagnostic काढा

स्थिर hb_user_data_किल्ली_t hb_kbts_shape_plan_data_किल्ली = {0};

संरचना hb_kbts_shape_plan_data_t
{
  kbts_shape_config *config = nullptr;
  mutable hb_atomic_t<kbts_shape_scratchpad *> scratchpad;
};

स्थिर शून्य
hb_kbts_shape_plan_data_destroy (शून्य *data)
{
  स्वतः *plan_data = (hb_kbts_shape_plan_data_t *) data;
  जर (!plan_data)
    परत;

  स्वतः *scratchpad = plan_data->scratchpad.get_acquire ();
  जर (scratchpad && plan_data->scratchpad.cmpexch (scratchpad, nullptr))
    kbts_DestroyShapeScratchpad (scratchpad);

  जर (plan_data->config)
    kbts_DestroyShapeConfig (plan_data->config);

  hb_free (plan_data);
}

स्थिर शून्य
hb_kbts_plan_props_to_script_language (स्थिरांक hb_segment_properties_t &props,
				       kbts_script *kb_script,
				       kbts_language *kb_language)
{
  hb_tag_t scripts[HB_OT_MAX_TAGS_PER_SCRIPT];
  hb_tag_t language;
  अचिन्हित पूर्णांक script_मोजणी = ARRAY_LENGTH (scripts);
  अचिन्हित पूर्णांक language_मोजणी = 1;

  *kb_script = KBTS_SCRIPT_DONT_KNOW;
  *kb_language = KBTS_LANGUAGE_DEFAULT;

  hb_ot_tags_from_script_and_language (props.script, props.language,
				       &script_मोजणी, scripts,
				       &language_मोजणी, &language);

  साठी (अचिन्हित पूर्णांक i = 0; i < script_मोजणी && scripts[i] != HB_TAG_NONE; ++i)
  {
    *kb_script = kbts_ScriptTagToScript (hb_uint32_swap (scripts[i]));
    जर (*kb_script != KBTS_SCRIPT_DONT_KNOW)
      थांबा;
  }

  जर (language_मोजणी)
    *kb_language = (kbts_language) hb_uint32_swap (language);
}

स्थिर hb_kbts_shape_plan_data_t *
hb_kbts_get_shape_plan_data (hb_shape_plan_t *shape_plan,
			     kbts_font *kb_font)
{
retry:
  स्वतः *plan_data = (hb_kbts_shape_plan_data_t *)
    hb_shape_plan_get_user_data (shape_plan, &hb_kbts_shape_plan_data_किल्ली);
  जर (plan_data)
    परत plan_data;

  kbts_script kb_script;
  kbts_language kb_language;
  hb_kbts_plan_props_to_script_language (shape_plan->key.props, &kb_script, &kb_language);

  kbts_shape_config *config = kbts_CreateShapeConfig (kb_font, kb_script, kb_language, nullptr, nullptr);
  जर (unlikely (!config))
    परत nullptr;

  plan_data = (hb_kbts_shape_plan_data_t *) hb_calloc (1, आकार (*plan_data));
  जर (unlikely (!plan_data))
  {
    kbts_DestroyShapeConfig (config);
    परत nullptr;
  }

  plan_data->config = config;
  plan_data->scratchpad.init (nullptr);

  जर (!hb_shape_plan_set_user_data (shape_plan,
				    &hb_kbts_shape_plan_data_किल्ली,
				    plan_data,
				    hb_kbts_shape_plan_data_destroy,
				    असत्य))
  {
    hb_kbts_shape_plan_data_destroy (plan_data);
    जा retry;
  }

  परत plan_data;
}

स्थिर kbts_shape_scratchpad *
hb_kbts_acquire_shape_scratchpad (hb_kbts_shape_plan_data_t *plan_data)
{
  स्वतः *scratchpad = plan_data->scratchpad.get_acquire ();
  जर (!scratchpad || unlikely (!plan_data->scratchpad.cmpexch (scratchpad, nullptr)))
    scratchpad = kbts_CreateShapeScratchpad (plan_data->config, nullptr, nullptr);

  परत scratchpad;
}

स्थिर शून्य
hb_kbts_release_shape_scratchpad (hb_kbts_shape_plan_data_t *plan_data,
				  kbts_shape_scratchpad *scratchpad)
{
  जर (!scratchpad)
    परत;

  जर (!plan_data->scratchpad.cmpexch (nullptr, scratchpad))
    kbts_DestroyShapeScratchpad (scratchpad);
}


hb_kbts_face_data_t *
_hb_kbts_shaper_face_data_create (hb_face_t *face)
{
  hb_blob_t *blob = hb_face_reference_blob (face);

  अचिन्हित पूर्णांक blob_length;
  स्थिरांक अक्षर *blob_data = hb_blob_get_data (blob, &blob_length);
  जर (unlikely (!blob_length))
  {
    DEBUG_संदेश (KBTS, blob, "Empty blob");
    hb_blob_destroy (blob);
    परत nullptr;
  }

  kbts_font *kb_font = (kbts_font *) hb_calloc (1, आकार (kbts_font));
  जर (unlikely (!kb_font))
  {
    hb_blob_destroy (blob);
    परत nullptr;
  }

  *kb_font = kbts_FontFromMemory((शून्य *)blob_data, blob_length, face->index, nullptr, nullptr);
  hb_blob_destroy (blob);
  blob = nullptr;

  जर (unlikely (!kbts_FontIsValid (kb_font)))
  {
    DEBUG_संदेश (KBTS, face, "Failed create font from data");
    kbts_FreeFont (kb_font);
    hb_free (kb_font);
    परत nullptr;
  }

  परत (hb_kbts_face_data_t *) kb_font;
}

शून्य
_hb_kbts_shaper_face_data_destroy (hb_kbts_face_data_t *data)
{
  kbts_font *font = (kbts_font *) data;

  प्रतिज्ञा (kbts_FontIsValid (font));

  kbts_FreeFont (font);
  hb_free (font);
}

hb_kbts_font_data_t *
_hb_kbts_shaper_font_data_create (hb_font_t *font)
{
  परत (hb_kbts_font_data_t *) HB_SHAPER_DATA_SUCCEEDED;
}

शून्य
_hb_kbts_shaper_font_data_destroy (hb_kbts_font_data_t *data)
{
}

hb_bool_t
_hb_kbts_shape (hb_shape_plan_t    *shape_plan,
		hb_font_t          *font,
		hb_buffer_t        *buffer,
		स्थिरांक hb_feature_t *features,
		अचिन्हित पूर्णांक        अंक_features)
{
  hb_face_t *face = font->face;
  kbts_font *kb_font = (kbts_font *) (स्थिरांक शून्य *) face->data.kbts;

  kbts_direction kb_direction;
  निवड (buffer->props.direction)
  {
    स्थिती HB_DIRECTION_LTR: kb_direction = KBTS_DIRECTION_LTR; थांबा;
    स्थिती HB_DIRECTION_RTL: kb_direction = KBTS_DIRECTION_RTL; थांबा;
    स्थिती HB_DIRECTION_TTB:
    स्थिती HB_DIRECTION_BTT:
      DEBUG_संदेश (KBTS, face, "Vertical direction is not supported");
      परत असत्य;
    पूर्वनिर्धारित:
    स्थिती HB_DIRECTION_INVALID:
      DEBUG_संदेश (KBTS, face, "Invalid direction");
      परत असत्य;
  }

  kbts_glyph_storage kb_glyph_storage;
  जर (unlikely (!kbts_InitializeGlyphStorage (&kb_glyph_storage, nullptr, nullptr)))
    परत असत्य;

  hb_kbts_shape_plan_data_t *plan_data = hb_kbts_get_shape_plan_data (shape_plan, kb_font);
  जर (unlikely (!plan_data))
  {
    kbts_FreeAllGlyphs (&kb_glyph_storage);
    परत असत्य;
  }

  kbts_shape_config *kb_shape_config = nullptr;
  kbts_shape_scratchpad *kb_shape_scratchpad = nullptr;
  uint32_t glyph_मोजणी = 0;
  kbts_glyph *kb_glyph = nullptr;
  kbts_glyph_iterator kb_output;
  hb_glyph_info_t *info;
  hb_glyph_position_t *pos;
  hb_bool_t res = असत्य;

  kb_shape_config = plan_data->config;
  kb_shape_scratchpad = hb_kbts_acquire_shape_scratchpad (plan_data);
  जर (unlikely (!kb_shape_scratchpad))
    जा done;

  साठी (माप_प्रकार i = 0; i < buffer->len; ++i)
  {
    kbts_glyph_config *kb_config = nullptr;
    जर (अंक_features)
    {
      hb_vector_t<kbts_feature_override> kb_features;
      साठी (अचिन्हित पूर्णांक j = 0; j < अंक_features; ++j)
      {
        जर (hb_in_range<माप_प्रकार> (i, features[j].start, features[j].end))
	  kb_features.push<kbts_feature_override>({ hb_uint32_swap (features[j].tag), (पूर्णांक)features[j].value });
      }
      जर (kb_features)
        kb_config = kbts_CreateGlyphConfig (kb_shape_config, kb_features.arrayZ, kb_features.length, nullptr, nullptr);
    }
    kbts_ढकला_Glyph (&kb_glyph_storage, kb_font, buffer->info[i].codepoint, kb_config, buffer->info[i].cluster);
  }

  res = kbts_ShapeDirect (kb_shape_scratchpad, &kb_glyph_storage, kb_direction, &kb_output) == KBTS_SHAPE_ERROR_NONE;
  जर (unlikely (!res))
    जा done;

  साठी (स्वतः it = kb_output; kbts_GlyphIteratorNext (&it, &kb_glyph); )
    glyph_मोजणी += 1;

  hb_buffer_set_content_type (buffer, HB_BUFFER_CONTENT_TYPE_GLYPHS);
  hb_buffer_set_length (buffer, glyph_मोजणी);

  buffer->clear_positions ();

  info = buffer->info;
  pos = buffer->pos;

  साठी (स्वतः it = kb_output; kbts_GlyphIteratorNext (&it, &kb_glyph); info++, pos++)
  {
    info->codepoint = kb_glyph->Id;
    info->cluster = kb_glyph->UserIdOrCodepointIndex;
    pos->x_advance = font->em_scalef_x (kb_glyph->AdvanceX);
    pos->y_advance = font->em_scalef_y (kb_glyph->AdvanceY);
    pos->x_offset = font->em_scalef_x (kb_glyph->OffsetX);
    pos->y_offset = font->em_scalef_y (kb_glyph->OffsetY);
  }

done:
  जर (likely (kb_shape_scratchpad))
    hb_kbts_release_shape_scratchpad (plan_data, kb_shape_scratchpad);
  पर्यंत (kbts_GlyphIteratorNext (&kb_output, &kb_glyph))
    kbts_DestroyGlyphConfig (kb_glyph->Config);
  kbts_FreeAllGlyphs (&kb_glyph_storage);

  buffer->clear_glyph_flags ();
  buffer->unsafe_to_break ();

  परत res;
}

#endif
