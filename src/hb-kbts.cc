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

#समावेश "hb.hh"

#अगर_घोषित HAVE_KBTS

#समावेश "hb-shaper-impl.hh"

#घोषणा KB_TEXT_SHAPE_IMPLEMENTATION
#घोषणा KB_TEXT_SHAPE_STATIC
#घोषणा KB_TEXT_SHAPE_NO_CRT
#घोषणा KBTS_MALLOC(a, b) hb_malloc(b)
#घोषणा KBTS_FREE(a, b) hb_free(b)
#घोषणा KBTS_MEMCPY hb_memcpy
#घोषणा KBTS_MEMSET hb_memset

#आशय GCC diagnostic push
#आशय GCC diagnostic ignored "-Wcast-align"
#आशय GCC diagnostic ignored "-Wunused-function"
#आशय GCC diagnostic ignored "-Wextra-semi-stmt"
#समावेश "kb_text_shape.h"
#आशय GCC diagnostic निकालो

अटल hb_user_तथ्य_कुंजी_t hb_kbts_shape_plan_तथ्य_कुंजी = {0};

काष्ठा hb_kbts_shape_plan_तथ्य_t
{
  kbts_shape_config *config = nullptr;
  mutable hb_atomic_t<kbts_shape_scratchpad *> scratchpad;
};

अटल व्योम
hb_kbts_shape_plan_तथ्य_destroy (व्योम *तथ्य)
{
  स्वतः *plan_तथ्य = (hb_kbts_shape_plan_तथ्य_t *) तथ्य;
  अगर (!plan_तथ्य)
    वापस;

  स्वतः *scratchpad = plan_तथ्य->scratchpad.get_acquire ();
  अगर (scratchpad && plan_तथ्य->scratchpad.cmpexch (scratchpad, nullptr))
    kbts_DestroyShapeScratchpad (scratchpad);

  अगर (plan_तथ्य->config)
    kbts_DestroyShapeConfig (plan_तथ्य->config);

  hb_free (plan_तथ्य);
}

अटल व्योम
hb_kbts_plan_props_to_script_language (स्थिर hb_segment_properties_t &props,
				       kbts_script *kb_script,
				       kbts_language *kb_language)
{
  hb_tag_t scripts[HB_OT_MAX_TAGS_PER_SCRIPT];
  hb_tag_t language;
  अचिन्हित पूर्णांक script_गिनती = ARRAY_लंबाई (scripts);
  अचिन्हित पूर्णांक language_गिनती = 1;

  *kb_script = KBTS_SCRIPT_DONT_KNOW;
  *kb_language = KBTS_LANGUAGE_DEFAULT;

  hb_ot_tags_from_script_and_language (props.script, props.language,
				       &script_गिनती, scripts,
				       &language_गिनती, &language);

  क्रम (अचिन्हित पूर्णांक i = 0; i < script_गिनती && scripts[i] != HB_TAG_NONE; ++i)
  {
    *kb_script = kbts_ScriptTagToScript (hb_uint32_swap (scripts[i]));
    अगर (*kb_script != KBTS_SCRIPT_DONT_KNOW)
      अवरोध;
  }

  अगर (language_गिनती)
    *kb_language = (kbts_language) hb_uint32_swap (language);
}

अटल hb_kbts_shape_plan_तथ्य_t *
hb_kbts_get_shape_plan_तथ्य (hb_shape_plan_t *shape_plan,
			     kbts_font *kb_font)
{
retry:
  स्वतः *plan_तथ्य = (hb_kbts_shape_plan_तथ्य_t *)
    hb_shape_plan_get_user_तथ्य (shape_plan, &hb_kbts_shape_plan_तथ्य_कुंजी);
  अगर (plan_तथ्य)
    वापस plan_तथ्य;

  kbts_script kb_script;
  kbts_language kb_language;
  hb_kbts_plan_props_to_script_language (shape_plan->key.props, &kb_script, &kb_language);

  kbts_shape_config *config = kbts_CreateShapeConfig (kb_font, kb_script, kb_language, nullptr, nullptr);
  अगर (unlikely (!config))
    वापस nullptr;

  plan_तथ्य = (hb_kbts_shape_plan_तथ्य_t *) hb_calloc (1, माप (*plan_तथ्य));
  अगर (unlikely (!plan_तथ्य))
  {
    kbts_DestroyShapeConfig (config);
    वापस nullptr;
  }

  plan_तथ्य->config = config;
  plan_तथ्य->scratchpad.init (nullptr);

  अगर (!hb_shape_plan_set_user_तथ्य (shape_plan,
				    &hb_kbts_shape_plan_तथ्य_कुंजी,
				    plan_तथ्य,
				    hb_kbts_shape_plan_तथ्य_destroy,
				    false))
  {
    hb_kbts_shape_plan_तथ्य_destroy (plan_तथ्य);
    जाओ retry;
  }

  वापस plan_तथ्य;
}

अटल kbts_shape_scratchpad *
hb_kbts_acquire_shape_scratchpad (hb_kbts_shape_plan_तथ्य_t *plan_तथ्य)
{
  स्वतः *scratchpad = plan_तथ्य->scratchpad.get_acquire ();
  अगर (!scratchpad || unlikely (!plan_तथ्य->scratchpad.cmpexch (scratchpad, nullptr)))
    scratchpad = kbts_CreateShapeScratchpad (plan_तथ्य->config, nullptr, nullptr);

  वापस scratchpad;
}

अटल व्योम
hb_kbts_release_shape_scratchpad (hb_kbts_shape_plan_तथ्य_t *plan_तथ्य,
				  kbts_shape_scratchpad *scratchpad)
{
  अगर (!scratchpad)
    वापस;

  अगर (!plan_तथ्य->scratchpad.cmpexch (nullptr, scratchpad))
    kbts_DestroyShapeScratchpad (scratchpad);
}


hb_kbts_face_तथ्य_t *
hb_kbts_shaper_face_तथ्य_create (hb_face_t *face)
{
  hb_blob_t *blob = hb_face_reference_blob (face);

  अचिन्हित पूर्णांक blob_लंबाई;
  स्थिर अक्षर *blob_तथ्य = hb_blob_get_तथ्य (blob, &blob_लंबाई);
  अगर (unlikely (!blob_लंबाई))
  {
    DEBUG_संदेश (KBTS, blob, "Empty blob");
    hb_blob_destroy (blob);
    वापस nullptr;
  }

  kbts_font *kb_font = (kbts_font *) hb_calloc (1, माप (kbts_font));
  अगर (unlikely (!kb_font))
  {
    hb_blob_destroy (blob);
    वापस nullptr;
  }

  *kb_font = kbts_FontFromMemory((व्योम *)blob_तथ्य, blob_लंबाई, face->सूचकांक_2, nullptr, nullptr);
  hb_blob_destroy (blob);
  blob = nullptr;

  अगर (unlikely (!kbts_FontIsValid (kb_font)))
  {
    DEBUG_संदेश (KBTS, face, "Failed create font from data");
    kbts_FreeFont (kb_font);
    hb_free (kb_font);
    वापस nullptr;
  }

  वापस (hb_kbts_face_तथ्य_t *) kb_font;
}

व्योम
hb_kbts_shaper_face_तथ्य_destroy (hb_kbts_face_तथ्य_t *तथ्य)
{
  kbts_font *font = (kbts_font *) तथ्य;

  निश्चित (kbts_FontIsValid (font));

  kbts_FreeFont (font);
  hb_free (font);
}

hb_kbts_font_तथ्य_t *
hb_kbts_shaper_font_तथ्य_create (hb_font_t *font)
{
  वापस (hb_kbts_font_तथ्य_t *) HB_SHAPER_तथ्य_SUCCEEDED;
}

व्योम
hb_kbts_shaper_font_तथ्य_destroy (hb_kbts_font_तथ्य_t *तथ्य)
{
}

hb_bool_t
_hb_kbts_shape (hb_shape_plan_t    *shape_plan,
		hb_font_t          *font,
		hb_बफर_t        *बफर,
		स्थिर hb_feature_t *features,
		अचिन्हित पूर्णांक        अंक_features)
{
  hb_face_t *face = font->face;
  kbts_font *kb_font = (kbts_font *) (स्थिर व्योम *) face->तथ्य.kbts;

  kbts_direction kb_direction;
  चयन (बफर->props.direction)
  {
    हाल HB_DIRECTION_LTR: kb_direction = KBTS_DIRECTION_LTR; अवरोध;
    हाल HB_DIRECTION_RTL: kb_direction = KBTS_DIRECTION_RTL; अवरोध;
    हाल HB_DIRECTION_TTB:
    हाल HB_DIRECTION_BTT:
      DEBUG_संदेश (KBTS, face, "Vertical direction is not supported");
      वापस false;
    शेष:
    हाल HB_DIRECTION_INVALID:
      DEBUG_संदेश (KBTS, face, "Invalid direction");
      वापस false;
  }

  kbts_glyph_storage kb_glyph_storage;
  अगर (unlikely (!kbts_InitializeGlyphStorage (&kb_glyph_storage, nullptr, nullptr)))
    वापस false;

  hb_kbts_shape_plan_तथ्य_t *plan_तथ्य = hb_kbts_get_shape_plan_तथ्य (shape_plan, kb_font);
  अगर (unlikely (!plan_तथ्य))
  {
    kbts_FreeAllGlyphs (&kb_glyph_storage);
    वापस false;
  }

  kbts_shape_config *kb_shape_config = nullptr;
  kbts_shape_scratchpad *kb_shape_scratchpad = nullptr;
  uint32_t glyph_गिनती = 0;
  kbts_glyph *kb_glyph = nullptr;
  kbts_glyph_iterator kb_output;
  hb_glyph_info_t *info;
  hb_glyph_position_t *pos;
  hb_bool_t res = false;

  kb_shape_config = plan_तथ्य->config;
  kb_shape_scratchpad = hb_kbts_acquire_shape_scratchpad (plan_तथ्य);
  अगर (unlikely (!kb_shape_scratchpad))
    जाओ done;

  क्रम (माप_प्रकार i = 0; i < बफर->len; ++i)
  {
    kbts_glyph_config *kb_config = nullptr;
    अगर (अंक_features)
    {
      hb_vector_t<kbts_feature_override> kb_features;
      क्रम (अचिन्हित पूर्णांक j = 0; j < अंक_features; ++j)
      {
        अगर (hb_in_range<माप_प्रकार> (i, features[j].start, features[j].end))
	  kb_features.push<kbts_feature_override>({ hb_uint32_swap (features[j].tag), (पूर्णांक)features[j].value });
      }
      अगर (kb_features)
        kb_config = kbts_CreateGlyphConfig (kb_shape_config, kb_features.arrayZ, kb_features.लंबाई, nullptr, nullptr);
    }
    kbts_धकेलो_Glyph (&kb_glyph_storage, kb_font, बफर->info[i].codepoint, kb_config, बफर->info[i].cluster);
  }

  res = kbts_ShapeDirect (kb_shape_scratchpad, &kb_glyph_storage, kb_direction, &kb_output) == KBTS_SHAPE_ERROR_NONE;
  अगर (unlikely (!res))
    जाओ done;

  क्रम (स्वतः it = kb_output; kbts_Glyph_Iterator_अग्र (&it, &kb_glyph); )
    glyph_गिनती += 1;

  hb_बफर_set_content_type (बफर, HB_बफर_CONTENT_TYPE_GLYPHS);
  hb_बफर_set_लंबाई (बफर, glyph_गिनती);

  बफर->clear_positions ();

  info = बफर->info;
  pos = बफर->pos;

  क्रम (स्वतः it = kb_output; kbts_Glyph_Iterator_अग्र (&it, &kb_glyph); info++, pos++)
  {
    info->codepoint = kb_glyph->Id;
    info->cluster = kb_glyph->User_Id_Or_Codepoint_सूचकांक;
    pos->x_advance = font->em_scalef_x (kb_glyph->AdvanceX);
    pos->y_advance = font->em_scalef_y (kb_glyph->AdvanceY);
    pos->x_offset = font->em_scalef_x (kb_glyph->OffsetX);
    pos->y_offset = font->em_scalef_y (kb_glyph->OffsetY);
  }

done:
  अगर (likely (kb_shape_scratchpad))
    hb_kbts_release_shape_scratchpad (plan_तथ्य, kb_shape_scratchpad);
  जबतक (kbts_Glyph_Iterator_अग्र (&kb_output, &kb_glyph))
    kbts_DestroyGlyphConfig (kb_glyph->Config);
  kbts_FreeAllGlyphs (&kb_glyph_storage);

  बफर->clear_glyph_flags ();
  बफर->unsafe_to_break ();

  वापस res;
}

#पूर्ण_अगर
