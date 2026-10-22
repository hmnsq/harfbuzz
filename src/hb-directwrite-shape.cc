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
 * Author(s): Behdad Esfahbod
 */

#समावेश "hb.hh"

#अगर_घोषित HAVE_DIRECTWRITE

#समावेश "hb-shaper-impl.hh"

#समावेश "hb-directwrite.hh"

#समावेश "hb-ms-feature-ranges.hh"


/*
* shaper face data
*/

hb_directwrite_face_तथ्य_t *
hb_directwrite_shaper_face_तथ्य_create (hb_face_t *face)
{
  hb_blob_t *blob = hb_face_reference_blob (face);

  hb_directwrite_face_तथ्य_t *तथ्य = (hb_directwrite_face_तथ्य_t *) dw_face_create (blob, face->सूचकांक_2);

  hb_blob_destroy (blob);

  वापस तथ्य;
}

व्योम
hb_directwrite_shaper_face_तथ्य_destroy (hb_directwrite_face_तथ्य_t *तथ्य)
{
  ((IDWriteFontFace *) तथ्य)->Release ();
}


/*
 * shaper font data
 */

काष्ठा hb_directwrite_font_तथ्य_t {};

hb_directwrite_font_तथ्य_t *
hb_directwrite_shaper_font_तथ्य_create (hb_font_t *font)
{
  IDWriteFontFace *fontFace = (IDWriteFontFace *) (स्थिर व्योम *) font->face->तथ्य.directwrite;

  /*
   * Set up variations.
   */
  IDWriteFontFace5 *fontFaceVariations = nullptr;
  {
    IDWriteFontFace5 *fontFace5;
    अगर (SUCCEEDED (fontFace->QueryInterface (__uuidof (IDWriteFontFace5), (व्योम **) &fontFace5)))
    {
      IDWriteFontResource *fontResource;
      अगर (SUCCEEDED (fontFace5->GetFontResource (&fontResource)))
      {
	hb_vector_t<DWRITE_FONT_AXIS_मूल्य> axis_मूल्यावली;
	अगर (likely (axis_मूल्यावली.resize_exact (font->अंक_coords)))
	{
	  क्रम (अचिन्हित पूर्णांक i = 0; i < font->अंक_coords; i++)
	  {
	    hb_ot_var_axis_info_t info;
	    अचिन्हित पूर्णांक c = 1;
	    hb_ot_var_get_axis_infos (font->face, i, &c, &info);
	    axis_मूल्यावली[i].axisTag = (DWRITE_FONT_AXIS_TAG) hb_uint32_swap (info.tag);
	    axis_मूल्यावली[i].value = i < font->अंक_coords ?
				   hb_clamp (font->design_coords[i], info.min_मूल्य, info.max_मूल्य) :
				   info.default_मूल्य;
	  }

	  fontResource->CreateFontFace (DWRITE_FONT_SIMULATIONS::DWRITE_FONT_SIMULATIONS_NONE,
					axis_मूल्यावली.arrayZ, axis_मूल्यावली.लंबाई, &fontFaceVariations);
	}
	fontResource->Release ();
      }
      fontFace5->Release ();
    }
  }

  वापस (hb_directwrite_font_तथ्य_t *) fontFaceVariations;
}

व्योम
hb_directwrite_shaper_font_तथ्य_destroy (hb_directwrite_font_तथ्य_t *तथ्य)
{
  ((IDWriteFontFace *) (स्थिर व्योम *) तथ्य)->Release ();
}


// Most of TextAnalysis is originally written by Bas Schouten for Mozilla project
// but now is relicensed to MIT for HarfBuzz use
श्रेणी TextAnalysis : खुला IDWriteTextAnalysisSource, खुला IDWriteTextAnalysisSink
{
निजी:
  hb_reference_गिनती_t m_Ref_गिनती;
खुला:
  IFACEMETHOD (QueryInterface) (IID स्थिर& iid, OUT व्योम** ppObject)
  { वापस S_OK; }
  IFACEMETHOD_ (ULONG, AddRef) ()
  {
    वापस m_Ref_गिनती.inc () + 1;
  }
  IFACEMETHOD_ (ULONG, Release) ()
  {
    चिन्हित ref_गिनती = m_Ref_गिनती.डेसी () - 1;
    निश्चित (ref_गिनती >= 0);
    अगर (ref_गिनती)
      वापस ref_गिनती;
    वापस 0;
  }

  // A single contiguous run of characters containing the same analysis
  // results.
  काष्ठा Run
  {
    uint32_t mTextStart;   // starting text position of this run
    uint32_t m_Text_लंबाई;  // number of contiguous code units covered
    uint32_t mGlyphStart;  // starting glyph in the glyphs array
    uint32_t m_Glyph_गिनती;  // number of glyphs associated with this run
    // text
    DWRITE_SCRIPT_ANALYSIS mScript;
    uint8_t mBidiLevel;
    bool mIsSideways;

    bool ContainsTextPosition (uint32_t aTextPosition) स्थिर
    {
      वापस aTextPosition >= mTextStart &&
	     aTextPosition <  mTextStart + m_Text_लंबाई;
    }

    Run *अग्र_Run;
  };

खुला:
  TextAnalysis (स्थिर ब_अक्षर_प्रकार* text, uint32_t text_लंबाई_2,
		स्थिर ब_अक्षर_प्रकार* localeName, DWRITE_READING_DIRECTION readingDirection)
	       : m_Text_लंबाई (text_लंबाई_2), mText (text), mLocaleName (localeName),
		 mReadingDirection (readingDirection), mCurrentRun (nullptr)
  {
    m_Ref_गिनती.init ();
  }
  ~TextAnalysis ()
  {
    // free runs, except mRunHead which is part of the TextAnalysis object
    क्रम (Run *run = m_Run_शीर्ष.अग्र_Run; run;)
    {
      Run *origRun = run;
      run = run->अग्र_Run;
      hb_free (origRun);
    }
  }

  STDMETHODIMP
  GenerateResults (IDWriteTextAnalyzer* textAnalyzer, Run **run_शीर्ष)
  {
    // Analyzes the text using the script analyzer and returns
    // the result as a series of runs.

    HRESULT hr = S_OK;

    // Initially start out with one result that covers the entire range.
    // This result will be subdivided by the analysis processes.
    m_Run_शीर्ष.mTextStart = 0;
    m_Run_शीर्ष.m_Text_लंबाई = m_Text_लंबाई;
    m_Run_शीर्ष.mBidiLevel =
      (mReadingDirection == DWRITE_READING_DIRECTION_दायाँ_TO_बायाँ);
    m_Run_शीर्ष.अग्र_Run = nullptr;
    mCurrentRun = &m_Run_शीर्ष;

    // Call each of the analyzers in sequence, recording their results.
    अगर (SUCCEEDED (hr = textAnalyzer->AnalyzeScript (यह, 0, m_Text_लंबाई, यह)))
      *run_शीर्ष = &m_Run_शीर्ष;

    वापस hr;
  }

  // IDWriteTextAnalysisSource implementation

  IFACEMETHODIMP
  GetTextAtPosition (uint32_t textPosition,
		     OUT ब_अक्षर_प्रकार स्थिर** textString,
		     OUT uint32_t* text_लंबाई_2)
  {
    अगर (textPosition >= m_Text_लंबाई)
    {
      // No text at this position, valid query though.
      *textString = nullptr;
      *text_लंबाई_2 = 0;
    }
    अन्यथा
    {
      *textString = mText + textPosition;
      *text_लंबाई_2 = m_Text_लंबाई - textPosition;
    }
    वापस S_OK;
  }

  IFACEMETHODIMP
  GetTextBeforePosition (uint32_t textPosition,
			 OUT ब_अक्षर_प्रकार स्थिर** textString,
			 OUT uint32_t* text_लंबाई_2)
  {
    अगर (textPosition == 0 || textPosition > m_Text_लंबाई)
    {
      // Either there is no text before here (== 0), or this
      // is an invalid position. The query is considered valid though.
      *textString = nullptr;
      *text_लंबाई_2 = 0;
    }
    अन्यथा
    {
      *textString = mText;
      *text_लंबाई_2 = textPosition;
    }
    वापस S_OK;
  }

  IFACEMETHODIMP_ (DWRITE_READING_DIRECTION)
  GetParagraphReadingDirection () { वापस mReadingDirection; }

  IFACEMETHODIMP GetLocaleName (uint32_t textPosition, uint32_t* text_लंबाई_2,
				ब_अक्षर_प्रकार स्थिर** localeName)
  { वापस S_OK; }

  IFACEMETHODIMP
  Get_संख्या_Substitution (uint32_t textPosition,
			 OUT uint32_t* text_लंबाई_2,
			 OUT IDWrite_संख्या_Substitution** संख्या_Substitution)
  {
    // We do not support number substitution.
    *संख्या_Substitution = nullptr;
    *text_लंबाई_2 = m_Text_लंबाई - textPosition;

    वापस S_OK;
  }

  // IDWriteTextAnalysisSink implementation

  IFACEMETHODIMP
  SetScriptAnalysis (uint32_t textPosition, uint32_t text_लंबाई_2,
		     DWRITE_SCRIPT_ANALYSIS स्थिर* scriptAnalysis)
  {
    SetCurrentRun (textPosition);
    अगर (unlikely (!SplitCurrentRun (textPosition)))
      वापस E_OUTOFMEMORY;
    जबतक (text_लंबाई_2 > 0)
    {
      Run *run = Fetch_अग्र_Run (&text_लंबाई_2);
      अगर (unlikely (!run))
        वापस E_OUTOFMEMORY;
      run->mScript = *scriptAnalysis;
    }

    वापस S_OK;
  }

  IFACEMETHODIMP
  Set_पंक्ति_Breakpoints (uint32_t textPosition,
		      uint32_t text_लंबाई_2,
		      स्थिर DWRITE_पंक्ति_BREAKPOINT* पंक्ति_Breakpoints)
  { वापस S_OK; }

  IFACEMETHODIMP SetBidiLevel (uint32_t textPosition, uint32_t text_लंबाई_2,
			       uint8_t explicitLevel, uint8_t resolvedLevel)
  { वापस S_OK; }

  IFACEMETHODIMP
  Set_संख्या_Substitution (uint32_t textPosition, uint32_t text_लंबाई_2,
			 IDWrite_संख्या_Substitution* संख्या_Substitution)
  { वापस S_OK; }

रक्षित:
  Run *Fetch_अग्र_Run (IN OUT uint32_t* text_लंबाई_2)
  {
    // Used by the sink setters, this returns a reference to the next run.
    // Position and length are adjusted to now point after the current run
    // being returned.

    Run *origRun = mCurrentRun;
    // Split the tail if needed (the length remaining is less than the
    // current run's size).
    अगर (*text_लंबाई_2 < mCurrentRun->m_Text_लंबाई)
    {
      अगर (unlikely (!SplitCurrentRun (mCurrentRun->mTextStart + *text_लंबाई_2)))
        वापस nullptr;
    }
    अन्यथा
      // Just advance the current run.
      mCurrentRun = mCurrentRun->अग्र_Run;
    *text_लंबाई_2 -= origRun->m_Text_लंबाई;

    // Return a reference to the run that was just current.
    वापस origRun;
  }

  व्योम SetCurrentRun (uint32_t textPosition)
  {
    // Move the current run to the given position.
    // Since the analyzers generally return results in a forward manner,
    // this will usually just return early. If not, find the
    // corresponding run for the text position.

    अगर (mCurrentRun && mCurrentRun->ContainsTextPosition (textPosition))
      वापस;

    क्रम (Run *run = &m_Run_शीर्ष; run; run = run->अग्र_Run)
      अगर (run->ContainsTextPosition (textPosition))
      {
	mCurrentRun = run;
	वापस;
      }
    निश्चित (0); // We should always be able to find the text position in one of our runs
  }

  bool SplitCurrentRun (uint32_t splitPosition)
  {
    अगर (!mCurrentRun)
    {
      निश्चित (0); // SplitCurrentRun called without current run
      // Shouldn't be calling this when no current run is set!
      वापस false;
    }
    // Split the current run.
    अगर (splitPosition <= mCurrentRun->mTextStart)
    {
      // No need to split, already the start of a run
      // or before it. Usually the first.
      वापस true;
    }
    Run *newRun = (Run *) hb_malloc (माप (Run));
    अगर (unlikely (!newRun))
      वापस false;

    *newRun = *mCurrentRun;

    // Insert the new run in our linked list.
    newRun->अग्र_Run = mCurrentRun->अग्र_Run;
    mCurrentRun->अग्र_Run = newRun;

    // Adjust runs' text positions and lengths.
    uint32_t splitPoint = splitPosition - mCurrentRun->mTextStart;
    newRun->mTextStart += splitPoint;
    newRun->m_Text_लंबाई -= splitPoint;
    mCurrentRun->m_Text_लंबाई = splitPoint;
    mCurrentRun = newRun;
    वापस true;
  }

रक्षित:
  //निवेश
  // (weak references are fine here, since this class is a transient
  //  stack-based helper that doesn't need to copy data)
  uint32_t m_Text_लंबाई;
  स्थिर ब_अक्षर_प्रकार* mText;
  स्थिर ब_अक्षर_प्रकार* mLocaleName;
  DWRITE_READING_DIRECTION mReadingDirection;

  // Current processing state.
  Run *mCurrentRun;

  // Output is a list of runs starting here
  Run  m_Run_शीर्ष;
};

/*
 * shaper
 */

hb_bool_t
_hb_directwrite_shape (hb_shape_plan_t    *shape_plan,
		       hb_font_t          *font,
		       hb_बफर_t        *बफर,
		       स्थिर hb_feature_t *features,
		       अचिन्हित पूर्णांक        अंक_features)
{
  IDWriteFontFace *fontFace = (IDWriteFontFace *) (स्थिर व्योम *) font->तथ्य.directwrite;
  स्वतः *global = get_directwrite_global ();
  अगर (unlikely (!global))
    वापस false;
  IDWriteFactory *dwriteFactory = global->dwriteFactory;

  IDWriteTextAnalyzer* analyzer;
  dwriteFactory->CreateTextAnalyzer (&analyzer);

  अचिन्हित पूर्णांक scratch_आकार;
  hb_बफर_t::scratch_बफर_t *scratch = बफर->get_scratch_बफर (&scratch_आकार);
#घोषणा ALLOCATE_ARRAY(Type, name, len) \
  Type *name = (Type *) scratch; \
  करो { \
    अचिन्हित पूर्णांक _consumed = DIV_CEIL ((len) * माप (Type), माप (*scratch)); \
    निश्चित (_consumed <= scratch_आकार); \
    scratch += _consumed; \
    scratch_आकार -= _consumed; \
  } जबतक (0)

#घोषणा utf16_सूचकांक() var1.u32

  ALLOCATE_ARRAY (ब_अक्षर_प्रकार, textString, बफर->len * 2);

  अचिन्हित पूर्णांक chars_len = 0;
  क्रम (अचिन्हित पूर्णांक i = 0; i < बफर->len; i++)
  {
    hb_codepoint_t c = बफर->info[i].codepoint;
    बफर->info[i].utf16_सूचकांक () = chars_len;
    अगर (likely (c <= 0xFFFFu))
      textString[chars_len++] = c;
    अन्यथा अगर (unlikely (c > 0x10FFFFu))
      textString[chars_len++] = 0xFFFDu;
    अन्यथा
    {
      textString[chars_len++] = 0xD800u + ((c - 0x10000u) >> 10);
      textString[chars_len++] = 0xDC00u + ((c - 0x10000u) & ((1u << 10) - 1));
    }
  }

  ALLOCATE_ARRAY (WORD, log_clusters, chars_len);
#अघोषित ALLOCATE_ARRAY
  /* Need log_clusters to assign features. */
  chars_len = 0;
  क्रम (अचिन्हित पूर्णांक i = 0; i < बफर->len; i++)
  {
    hb_codepoint_t c = बफर->info[i].codepoint;
    अचिन्हित पूर्णांक cluster = बफर->info[i].cluster;
    log_clusters[chars_len++] = cluster;
    अगर (hb_in_range (c, 0x10000u, 0x10FFFFu))
      log_clusters[chars_len++] = cluster; /* Surrogates. */
  }

  DWRITE_READING_DIRECTION readingDirection;
  readingDirection = बफर->props.direction ?
		     DWRITE_READING_DIRECTION_दायाँ_TO_बायाँ :
		     DWRITE_READING_DIRECTION_बायाँ_TO_दायाँ;

  /*
  * There's an internal 16-bit limit on some things inside the analyzer,
  * but we never attempt to shape a word longer than 64K characters
  * in a single gfxShapedWord, so we cannot exceed that limit.
  */
  uint32_t text_लंबाई_2 = chars_len;

  TextAnalysis analysis (textString, text_लंबाई_2, nullptr, readingDirection);
  TextAnalysis::Run *run_शीर्ष;
  HRESULT hr;
  hr = analysis.GenerateResults (analyzer, &run_शीर्ष);

#घोषणा FAIL(...) \
  HB_STMT_START { \
    DEBUG_संदेश (DIRECTWRITE, nullptr, __VA_ARGS__); \
    वापस false; \
  } HB_STMT_END

  अगर (FAILED (hr))
    FAIL ("Analyzer failed to generate results.");

  uint32_t max_Glyph_गिनती = 3 * text_लंबाई_2 / 2 + 16;
  uint32_t glyph_गिनती_2;
  bool is_दायाँ_To_बायाँ = HB_DIRECTION_IS_BACKWARD (बफर->props.direction);

  स्थिर ब_अक्षर_प्रकार localeName[20] = {0};
  अगर (बफर->props.language)
    mbstowcs ((ब_अक्षर_प्रकार*) localeName,
	      hb_language_to_string (बफर->props.language), 20);

  /*
   * Set up features.
   */
  static_assert ((माप (DWRITE_TYPOGRAPHIC_FEATURES) == माप (hb_ms_features_t)), "");
  static_assert ((माप (DWRITE_FONT_FEATURE) == माप (hb_ms_feature_t)), "");
  hb_vector_t<hb_ms_features_t *> range_features;
  hb_vector_t<uint32_t> range_char_counts;

  // https://github.com/harfbuzz/harfbuzz/pull/5114
  // The data allocated by these two vectors are used by the above two, so they
  // should remain alive as long as the above two are.
  hb_vector_t<hb_ms_feature_t> feature_records;
  hb_vector_t<hb_ms_range_record_t> range_records;
  अगर (अंक_features)
  {
    अगर (hb_ms_setup_features (features, अंक_features, feature_records, range_records))
    {
      hb_ms_make_feature_ranges (feature_records,
				 range_records,
				 0,
				 chars_len,
				 log_clusters,
				 range_features,
				 range_char_counts);
    }
  }

  hb_vector_t<uint16_t> clusterMap;
  hb_vector_t<DWRITE_SHAPING_TEXT_PROPERTIES> textProperties;
  अगर (unlikely (!clusterMap.resize_exact (text_लंबाई_2) ||
                !textProperties.resize_exact (text_लंबाई_2)))
    FAIL ("Failed to allocate shaping text data.");

  hb_vector_t<uint16_t> glyphIndices;
  hb_vector_t<DWRITE_SHAPING_GLYPH_PROPERTIES> glyphProperties;

retry_getglyphs:
  अगर (unlikely (!glyphIndices.resize_exact (max_Glyph_गिनती) ||
                !glyphProperties.resize_exact (max_Glyph_गिनती)))
    FAIL ("Failed to allocate shaping glyph data.");

  hr = analyzer->GetGlyphs (textString,
			    chars_len,
			    fontFace,
			    false,
			    is_दायाँ_To_बायाँ,
			    &run_शीर्ष->mScript,
			    localeName,
			    nullptr,
			    (स्थिर DWRITE_TYPOGRAPHIC_FEATURES**) range_features.arrayZ,
			    range_char_counts.arrayZ,
			    range_features.लंबाई,
			    max_Glyph_गिनती,
			    clusterMap.arrayZ,
			    textProperties.arrayZ,
			    glyphIndices.arrayZ,
			    glyphProperties.arrayZ,
			    &glyph_गिनती_2);

  अगर (unlikely (hr == HRESULT_FROM_WIN32 (ERROR_INSUFFICIENT_बफर)))
  {
    max_Glyph_गिनती *= 2;

    जाओ retry_getglyphs;
  }
  अगर (FAILED (hr))
    FAIL ("Analyzer failed to get glyphs.");

  hb_vector_t<भग्न> glyphAdvances;
  hb_vector_t<DWRITE_GLYPH_OFFSET> glyphOffsets;
  अगर (unlikely (!glyphAdvances.resize_exact (max_Glyph_गिनती) ||
                !glyphOffsets.resize_exact (max_Glyph_गिनती)))
    FAIL ("Failed to allocate glyph positioning data.");


  अचिन्हित font_Em_आकार = font->face->get_upem ();

  भग्न x_mult = font->x_multf;
  भग्न y_mult = font->y_multf;

  hr = analyzer->GetGlyphPlacements (textString,
				     clusterMap.arrayZ,
				     textProperties.arrayZ,
				     chars_len,
				     glyphIndices.arrayZ,
				     glyphProperties.arrayZ,
				     glyph_गिनती_2,
				     fontFace,
				     font_Em_आकार,
				     false,
				     is_दायाँ_To_बायाँ,
				     &run_शीर्ष->mScript,
				     localeName,
				     (स्थिर DWRITE_TYPOGRAPHIC_FEATURES**) range_features.arrayZ,
				     range_char_counts.arrayZ,
				     range_features.लंबाई,
				     glyphAdvances.arrayZ,
				     glyphOffsets.arrayZ);

  अगर (FAILED (hr))
    FAIL ("Analyzer failed to get glyph placements.");

  /* Ok, we've got everything we need, now compose output buffer,
   * very, *very*, carefully! */

  /* Calculate visual-clusters.  That's what we ship. */
  hb_vector_t<uint32_t> vis_clusters;
  अगर (unlikely (!vis_clusters.resize_exact (glyph_गिनती_2)))
    FAIL ("Failed to allocate visual clusters.");

  क्रम (अचिन्हित पूर्णांक i = 0; i < glyph_गिनती_2; i++)
    vis_clusters[i] = (uint32_t) -1;
  क्रम (अचिन्हित पूर्णांक i = 0; i < बफर->len; i++)
  {
    अचिन्हित पूर्णांक c = clusterMap[बफर->info[i].utf16_सूचकांक ()];
    अगर (likely (c < glyph_गिनती_2))
      vis_clusters[c] = hb_min (vis_clusters[c], बफर->info[i].cluster);
  }
  क्रम (अचिन्हित पूर्णांक i = 1; i < glyph_गिनती_2; i++)
    अगर (vis_clusters[i] == (uint32_t) -1)
      vis_clusters[i] = vis_clusters[i - 1];
  क्रम (पूर्णांक i = (पूर्णांक) glyph_गिनती_2 - 2; i >= 0; i--)
    अगर (vis_clusters[i] == (uint32_t) -1)
      vis_clusters[i] = vis_clusters[i + 1];

#अघोषित utf16_सूचकांक

  अगर (unlikely (!बफर->ensure (glyph_गिनती_2)))
    FAIL ("Buffer in error");

#अघोषित FAIL

  /* Set glyph infos */
  बफर->len = 0;
  क्रम (अचिन्हित पूर्णांक i = 0; i < glyph_गिनती_2; i++)
  {
    hb_glyph_info_t *info = &बफर->info[बफर->len++];

    info->codepoint = glyphIndices[i];
    info->cluster = vis_clusters[i];

    /* The rest is crap.  Let's store position info there for now. */
    info->mask = (hb_mask_t) hb_clamp_to<int32_t> (glyphAdvances[i]);
    info->var1.i32 = hb_clamp_to<int32_t> (glyphOffsets[i].advanceOffset);
    info->var2.i32 = hb_clamp_to<int32_t> (glyphOffsets[i].ascenderOffset);
  }

  /* Set glyph positions */
  बफर->clear_positions ();
  क्रम (अचिन्हित पूर्णांक i = 0; i < glyph_गिनती_2; i++)
  {
    hb_glyph_info_t *info = &बफर->info[i];
    hb_glyph_position_t *pos = &बफर->pos[i];

    /* TODO vertical */
    pos->x_advance = hb_clamp_to<hb_position_t> (round (x_mult * (int32_t) info->mask));
    द्विगुन x_offset = is_दायाँ_To_बायाँ ? -(द्विगुन) info->var1.i32 : info->var1.i32;
    pos->x_offset = hb_clamp_to<hb_position_t> (round ((द्विगुन) x_mult * x_offset));
    pos->y_offset = hb_clamp_to<hb_position_t> (round (y_mult * info->var2.i32));
  }

  अगर (is_दायाँ_To_बायाँ) hb_बफर_reverse (बफर);

  बफर->clear_glyph_flags ();
  बफर->unsafe_to_break ();

  /* Wow, done! */
  वापस true;
}


#पूर्ण_अगर
