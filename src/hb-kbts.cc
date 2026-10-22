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

#అన్తర్భావ "hb.hh"

#నిర్వచితమైతే HAVE_KBTS

#అన్తర్భావ "hb-shaper-impl.hh"

#నిర్వచించు KB_పాఠ్యం_SHAPE_IMPLEMENTATION
#నిర్వచించు KB_పాఠ్యం_SHAPE_STATIC
#నిర్వచించు KB_పాఠ్యం_SHAPE_NO_CRT
#నిర్వచించు KBTS_MALLOC(a, b) hb_malloc(b)
#నిర్వచించు KBTS_FREE(a, b) hb_free(b)
#నిర్వచించు KBTS_MEMCPY hb_memcpy
#నిర్వచించు KBTS_MEMSET hb_memset

#సూచన GCC diagnostic push
#సూచన GCC diagnostic ignored "-Wcast-align"
#సూచన GCC diagnostic ignored "-Wunused-function"
#సూచన GCC diagnostic ignored "-Wextra-semi-stmt"
#అన్తర్భావ "kb_text_shape.h"
#సూచన GCC diagnostic తీయి

స్థిర hb_user_దత్తాంశం_కీలకం_t hb_kbts_shape_plan_దత్తాంశం_కీలకం = {0};

నిర్మాణం hb_kbts_shape_plan_దత్తాంశం_t
{
  kbts_shape_config *config = శూన్యసూచిక;
  మార్చదగిన hb_atomic_t<kbts_shape_scratchpad *> scratchpad;
};

స్థిర శూన్య
hb_kbts_shape_plan_దత్తాంశం_destroy (శూన్య *దత్తాంశం)
{
  స్వయంచాలక *plan_దత్తాంశం = (hb_kbts_shape_plan_దత్తాంశం_t *) దత్తాంశం;
  ఒకవేళ (!plan_దత్తాంశం)
    తిరిగి;

  స్వయంచాలక *scratchpad = plan_దత్తాంశం->scratchpad.get_acquire ();
  ఒకవేళ (scratchpad && plan_దత్తాంశం->scratchpad.cmpexch (scratchpad, శూన్యసూచిక))
    kbts_DestroyShapeScratchpad (scratchpad);

  ఒకవేళ (plan_దత్తాంశం->config)
    kbts_DestroyShapeConfig (plan_దత్తాంశం->config);

  hb_free (plan_దత్తాంశం);
}

స్థిర శూన్య
hb_kbts_plan_props_గమ్యం_script_language (స్థిరరాశి hb_segment_properties_t &props,
				       kbts_script *kb_script,
				       kbts_language *kb_language)
{
  hb_ట్యాగ్_t scripts[HB_OT_MAX_ట్యాగులు_చొప్పున_SCRIPT];
  hb_ట్యాగ్_t language;
  గుర్తులేని పూర్ణాంకం script_లెక్క = ARRAY_నిడివి (scripts);
  గుర్తులేని పూర్ణాంకం language_లెక్క = 1;

  *kb_script = KBTS_SCRIPT_DONT_KNOW;
  *kb_language = KBTS_LANGUAGE_DEFAULT;

  hb_ot_ట్యాగులు_from_script_and_language (props.script, props.language,
				       &script_లెక్క, scripts,
				       &language_లెక్క, &language);

  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < script_లెక్క && scripts[i] != HB_ట్యాగ్_NONE; ++i)
  {
    *kb_script = kbts_Script_ట్యాగ్_గమ్యం_Script (hb_uint32_మార్పిడి (scripts[i]));
    ఒకవేళ (*kb_script != KBTS_SCRIPT_DONT_KNOW)
      ఆపు;
  }

  ఒకవేళ (language_లెక్క)
    *kb_language = (kbts_language) hb_uint32_మార్పిడి (language);
}

స్థిర hb_kbts_shape_plan_దత్తాంశం_t *
hb_kbts_get_shape_plan_దత్తాంశం (hb_shape_plan_t *shape_plan,
			     kbts_font *kb_font)
{
retry:
  స్వయంచాలక *plan_దత్తాంశం = (hb_kbts_shape_plan_దత్తాంశం_t *)
    hb_shape_plan_get_user_దత్తాంశం (shape_plan, &hb_kbts_shape_plan_దత్తాంశం_కీలకం);
  ఒకవేళ (plan_దత్తాంశం)
    తిరిగి plan_దత్తాంశం;

  kbts_script kb_script;
  kbts_language kb_language;
  hb_kbts_plan_props_గమ్యం_script_language (shape_plan->key.props, &kb_script, &kb_language);

  kbts_shape_config *config = kbts_CreateShapeConfig (kb_font, kb_script, kb_language, శూన్యసూచిక, శూన్యసూచిక);
  ఒకవేళ (unlikely (!config))
    తిరిగి శూన్యసూచిక;

  plan_దత్తాంశం = (hb_kbts_shape_plan_దత్తాంశం_t *) hb_calloc (1, పరిమాణం (*plan_దత్తాంశం));
  ఒకవేళ (unlikely (!plan_దత్తాంశం))
  {
    kbts_DestroyShapeConfig (config);
    తిరిగి శూన్యసూచిక;
  }

  plan_దత్తాంశం->config = config;
  plan_దత్తాంశం->scratchpad.init (శూన్యసూచిక);

  ఒకవేళ (!hb_shape_plan_సమితి_user_దత్తాంశం (shape_plan,
				    &hb_kbts_shape_plan_దత్తాంశం_కీలకం,
				    plan_దత్తాంశం,
				    hb_kbts_shape_plan_దత్తాంశం_destroy,
				    అబద్ధం))
  {
    hb_kbts_shape_plan_దత్తాంశం_destroy (plan_దత్తాంశం);
    వెళ్ళు retry;
  }

  తిరిగి plan_దత్తాంశం;
}

స్థిర kbts_shape_scratchpad *
hb_kbts_acquire_shape_scratchpad (hb_kbts_shape_plan_దత్తాంశం_t *plan_దత్తాంశం)
{
  స్వయంచాలక *scratchpad = plan_దత్తాంశం->scratchpad.get_acquire ();
  ఒకవేళ (!scratchpad || unlikely (!plan_దత్తాంశం->scratchpad.cmpexch (scratchpad, శూన్యసూచిక)))
    scratchpad = kbts_CreateShapeScratchpad (plan_దత్తాంశం->config, శూన్యసూచిక, శూన్యసూచిక);

  తిరిగి scratchpad;
}

స్థిర శూన్య
hb_kbts_release_shape_scratchpad (hb_kbts_shape_plan_దత్తాంశం_t *plan_దత్తాంశం,
				  kbts_shape_scratchpad *scratchpad)
{
  ఒకవేళ (!scratchpad)
    తిరిగి;

  ఒకవేళ (!plan_దత్తాంశం->scratchpad.cmpexch (శూన్యసూచిక, scratchpad))
    kbts_DestroyShapeScratchpad (scratchpad);
}


hb_kbts_face_దత్తాంశం_t *
hb_kbts_shaper_face_దత్తాంశం_create (hb_face_t *face)
{
  hb_blob_t *blob = hb_face_reference_blob (face);

  గుర్తులేని పూర్ణాంకం blob_నిడివి;
  స్థిరరాశి అక్షరం *blob_దత్తాంశం = hb_blob_get_దత్తాంశం (blob, &blob_నిడివి);
  ఒకవేళ (unlikely (!blob_నిడివి))
  {
    DEBUG_సందేశం (KBTS, blob, "Empty blob");
    hb_blob_destroy (blob);
    తిరిగి శూన్యసూచిక;
  }

  kbts_font *kb_font = (kbts_font *) hb_calloc (1, పరిమాణం (kbts_font));
  ఒకవేళ (unlikely (!kb_font))
  {
    hb_blob_destroy (blob);
    తిరిగి శూన్యసూచిక;
  }

  *kb_font = kbts_Font_From_జ్ఞాపకం((శూన్య *)blob_దత్తాంశం, blob_నిడివి, face->సూచీ_2, శూన్యసూచిక, శూన్యసూచిక);
  hb_blob_destroy (blob);
  blob = శూన్యసూచిక;

  ఒకవేళ (unlikely (!kbts_FontIsValid (kb_font)))
  {
    DEBUG_సందేశం (KBTS, face, "Failed create font from data");
    kbts_FreeFont (kb_font);
    hb_free (kb_font);
    తిరిగి శూన్యసూచిక;
  }

  తిరిగి (hb_kbts_face_దత్తాంశం_t *) kb_font;
}

శూన్య
hb_kbts_shaper_face_దత్తాంశం_destroy (hb_kbts_face_దత్తాంశం_t *దత్తాంశం)
{
  kbts_font *font = (kbts_font *) దత్తాంశం;

  ప్రతిజ్ఞా (kbts_FontIsValid (font));

  kbts_FreeFont (font);
  hb_free (font);
}

hb_kbts_font_దత్తాంశం_t *
hb_kbts_shaper_font_దత్తాంశం_create (hb_font_t *font)
{
  తిరిగి (hb_kbts_font_దత్తాంశం_t *) HB_SHAPER_దత్తాంశం_SUCCEEDED;
}

శూన్య
hb_kbts_shaper_font_దత్తాంశం_destroy (hb_kbts_font_దత్తాంశం_t *దత్తాంశం)
{
}

hb_bool_t
_hb_kbts_shape (hb_shape_plan_t    *shape_plan,
		hb_font_t          *font,
		hb_బఫరు_t        *బఫరు,
		స్థిరరాశి hb_feature_t *features,
		గుర్తులేని పూర్ణాంకం        అంకె_features)
{
  hb_face_t *face = font->face;
  kbts_font *kb_font = (kbts_font *) (స్థిరరాశి శూన్య *) face->దత్తాంశం.kbts;

  kbts_direction kb_direction;
  ఎంపిక (బఫరు->props.direction)
  {
    స్థితి HB_DIRECTION_LTR: kb_direction = KBTS_DIRECTION_LTR; ఆపు;
    స్థితి HB_DIRECTION_RTL: kb_direction = KBTS_DIRECTION_RTL; ఆపు;
    స్థితి HB_DIRECTION_TTB:
    స్థితి HB_DIRECTION_BTT:
      DEBUG_సందేశం (KBTS, face, "Vertical direction is not supported");
      తిరిగి అబద్ధం;
    అప్రమేయ:
    స్థితి HB_DIRECTION_INVALID:
      DEBUG_సందేశం (KBTS, face, "Invalid direction");
      తిరిగి అబద్ధం;
  }

  kbts_glyph_storage kb_glyph_storage;
  ఒకవేళ (unlikely (!kbts_InitializeGlyphStorage (&kb_glyph_storage, శూన్యసూచిక, శూన్యసూచిక)))
    తిరిగి అబద్ధం;

  hb_kbts_shape_plan_దత్తాంశం_t *plan_దత్తాంశం = hb_kbts_get_shape_plan_దత్తాంశం (shape_plan, kb_font);
  ఒకవేళ (unlikely (!plan_దత్తాంశం))
  {
    kbts_FreeAllGlyphs (&kb_glyph_storage);
    తిరిగి అబద్ధం;
  }

  kbts_shape_config *kb_shape_config = శూన్యసూచిక;
  kbts_shape_scratchpad *kb_shape_scratchpad = శూన్యసూచిక;
  uint32_t glyph_లెక్క = 0;
  kbts_glyph *kb_glyph = శూన్యసూచిక;
  kbts_glyph_iterator kb_output;
  hb_glyph_info_t *info;
  hb_glyph_position_t *pos;
  hb_bool_t res = అబద్ధం;

  kb_shape_config = plan_దత్తాంశం->config;
  kb_shape_scratchpad = hb_kbts_acquire_shape_scratchpad (plan_దత్తాంశం);
  ఒకవేళ (unlikely (!kb_shape_scratchpad))
    వెళ్ళు పూర్తి;

  కొరకు (size_t i = 0; i < బఫరు->len; ++i)
  {
    kbts_glyph_config *kb_config = శూన్యసూచిక;
    ఒకవేళ (అంకె_features)
    {
      hb_సదిశ_t<kbts_feature_override> kb_features;
      కొరకు (గుర్తులేని పూర్ణాంకం j = 0; j < అంకె_features; ++j)
      {
        ఒకవేళ (hb_లోపల_range<size_t> (i, features[j].ప్రారంభం, features[j].చివర))
	  kb_features.push<kbts_feature_override>({ hb_uint32_మార్పిడి (features[j].ట్యాగ్_2), (పూర్ణాంకం)features[j].value });
      }
      ఒకవేళ (kb_features)
        kb_config = kbts_CreateGlyphConfig (kb_shape_config, kb_features.arrayZ, kb_features.నిడివి, శూన్యసూచిక, శూన్యసూచిక);
    }
    kbts_నెట్టు_Glyph (&kb_glyph_storage, kb_font, బఫరు->info[i].codepoint, kb_config, బఫరు->info[i].cluster);
  }

  res = kbts_Shape_ప్రత్యక్ష (kb_shape_scratchpad, &kb_glyph_storage, kb_direction, &kb_output) == KBTS_SHAPE_ERROR_NONE;
  ఒకవేళ (unlikely (!res))
    వెళ్ళు పూర్తి;

  కొరకు (స్వయంచాలక it = kb_output; kbts_Glyph_Iterator_తదుపరి (&it, &kb_glyph); )
    glyph_లెక్క += 1;

  hb_బఫరు_సమితి_content_type (బఫరు, HB_బఫరు_CONTENT_TYPE_GLYPHS);
  hb_బఫరు_సమితి_నిడివి (బఫరు, glyph_లెక్క);

  బఫరు->clear_positions ();

  info = బఫరు->info;
  pos = బఫరు->pos;

  కొరకు (స్వయంచాలక it = kb_output; kbts_Glyph_Iterator_తదుపరి (&it, &kb_glyph); info++, pos++)
  {
    info->codepoint = kb_glyph->Id;
    info->cluster = kb_glyph->User_Id_Or_Codepoint_సూచీ;
    pos->x_advance = font->em_scalef_x (kb_glyph->AdvanceX);
    pos->y_advance = font->em_scalef_y (kb_glyph->AdvanceY);
    pos->x_స్థానభ్రంశం_2 = font->em_scalef_x (kb_glyph->స్థానభ్రంశం_X);
    pos->y_స్థానభ్రంశం_2 = font->em_scalef_y (kb_glyph->స్థానభ్రంశం_Y);
  }

పూర్తి:
  ఒకవేళ (likely (kb_shape_scratchpad))
    hb_kbts_release_shape_scratchpad (plan_దత్తాంశం, kb_shape_scratchpad);
  అయితే (kbts_Glyph_Iterator_తదుపరి (&kb_output, &kb_glyph))
    kbts_DestroyGlyphConfig (kb_glyph->Config);
  kbts_FreeAllGlyphs (&kb_glyph_storage);

  బఫరు->clear_glyph_flags ();
  బఫరు->unsafe_గమ్యం_break ();

  తిరిగి res;
}

#అయితే_ముగింపు
