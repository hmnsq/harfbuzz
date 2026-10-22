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

#ଅନ୍ତର୍ଭାଵ "hb.hh"

#ifdef HAVE_DIRECTWRITE

#ଅନ୍ତର୍ଭାଵ "hb-shaper-impl.hh"

#ଅନ୍ତର୍ଭାଵ "hb-directwrite.hh"

#ଅନ୍ତର୍ଭାଵ "hb-ms-feature-ranges.hh"


/*
* shaper face data
*/

hb_directwrite_face_data_t *
_hb_directwrite_shaper_face_data_create (hb_face_t *face)
{
  hb_blob_t *blob = hb_face_reference_blob (face);

  hb_directwrite_face_data_t *data = (hb_directwrite_face_data_t *) dw_face_create (blob, face->index);

  hb_blob_destroy (blob);

  ଫେରନ୍ତୁ data;
}

ଶୂନ୍ୟ
_hb_directwrite_shaper_face_data_destroy (hb_directwrite_face_data_t *data)
{
  ((IDWriteFontFace *) data)->Release ();
}


/*
 * shaper font data
 */

ସଂରଚନା hb_directwrite_font_data_t {};

hb_directwrite_font_data_t *
_hb_directwrite_shaper_font_data_create (hb_font_t *font)
{
  IDWriteFontFace *fontFace = (IDWriteFontFace *) (ଅଚଳ ଶୂନ୍ୟ *) font->face->data.directwrite;

  /*
   * Set up variations.
   */
  IDWriteFontFace5 *fontFaceVariations = nullptr;
  {
    IDWriteFontFace5 *fontFace5;
    ଯଦି (SUCCEEDED (fontFace->QueryInterface (__uuidof (IDWriteFontFace5), (ଶୂନ୍ୟ **) &fontFace5)))
    {
      IDWriteFontResource *fontResource;
      ଯଦି (SUCCEEDED (fontFace5->GetFontResource (&fontResource)))
      {
	hb_vector_t<DWRITE_FONT_AXIS_ମୂଲ୍ୟ> axis_ମୂଲ୍ୟଗୁଡ଼ିକ;
	ଯଦି (likely (axis_ମୂଲ୍ୟଗୁଡ଼ିକ.resize_exact (font->ଅଙ୍କ_coords)))
	{
	  ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < font->ଅଙ୍କ_coords; i++)
	  {
	    hb_ot_var_axis_info_t info;
	    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା c = 1;
	    hb_ot_var_get_axis_infos (font->face, i, &c, &info);
	    axis_ମୂଲ୍ୟଗୁଡ଼ିକ[i].axisTag = (DWRITE_FONT_AXIS_TAG) hb_uint32_swap (info.tag);
	    axis_ମୂଲ୍ୟଗୁଡ଼ିକ[i].value = i < font->ଅଙ୍କ_coords ?
				   hb_clamp (font->design_coords[i], info.min_ମୂଲ୍ୟ, info.max_ମୂଲ୍ୟ) :
				   info.default_ମୂଲ୍ୟ;
	  }

	  fontResource->CreateFontFace (DWRITE_FONT_SIMULATIONS::DWRITE_FONT_SIMULATIONS_NONE,
					axis_ମୂଲ୍ୟଗୁଡ଼ିକ.arrayZ, axis_ମୂଲ୍ୟଗୁଡ଼ିକ.length, &fontFaceVariations);
	}
	fontResource->Release ();
      }
      fontFace5->Release ();
    }
  }

  ଫେରନ୍ତୁ (hb_directwrite_font_data_t *) fontFaceVariations;
}

ଶୂନ୍ୟ
_hb_directwrite_shaper_font_data_destroy (hb_directwrite_font_data_t *data)
{
  ((IDWriteFontFace *) (ଅଚଳ ଶୂନ୍ୟ *) data)->Release ();
}


// Most of TextAnalysis is originally written by Bas Schouten for Mozilla project
// but now is relicensed to MIT for HarfBuzz use
ଶ୍ରେଣୀ TextAnalysis : ସର୍ବସାଧାରଣ IDWriteTextAnalysisSource, ସର୍ବସାଧାରଣ IDWriteTextAnalysisSink
{
ବ୍ୟକ୍ତିଗତ:
  hb_reference_ଗଣନା_t m_Ref_ଗଣନା;
ସର୍ବସାଧାରଣ:
  IFACEMETHOD (QueryInterface) (IID ଅଚଳ& iid, OUT ଶୂନ୍ୟ** ppObject)
  { ଫେରନ୍ତୁ S_OK; }
  IFACEMETHOD_ (ULONG, AddRef) ()
  {
    ଫେରନ୍ତୁ m_Ref_ଗଣନା.inc () + 1;
  }
  IFACEMETHOD_ (ULONG, Release) ()
  {
    signed ref_ଗଣନା = m_Ref_ଗଣନା.dec () - 1;
    ପ୍ରତିଜ୍ଞା (ref_ଗଣନା >= 0);
    ଯଦି (ref_ଗଣନା)
      ଫେରନ୍ତୁ ref_ଗଣନା;
    ଫେରନ୍ତୁ 0;
  }

  // A single contiguous run of characters containing the same analysis
  // results.
  ସଂରଚନା Run
  {
    uint32_t mTextStart;   // starting text position of this run
    uint32_t mTextLength;  // number of contiguous code units covered
    uint32_t mGlyphStart;  // starting glyph in the glyphs array
    uint32_t m_Glyph_ଗଣନା;  // number of glyphs associated with this run
    // text
    DWRITE_SCRIPT_ANALYSIS mScript;
    uint8_t mBidiLevel;
    ତାର୍କିକ mIsSideways;

    ତାର୍କିକ ContainsTextPosition (uint32_t aTextPosition) ଅଚଳ
    {
      ଫେରନ୍ତୁ aTextPosition >= mTextStart &&
	     aTextPosition <  mTextStart + mTextLength;
    }

    Run *nextRun;
  };

ସର୍ବସାଧାରଣ:
  TextAnalysis (ଅଚଳ wchar_t* text, uint32_t textLength,
		ଅଚଳ wchar_t* localeName, DWRITE_READING_DIRECTION readingDirection)
	       : mTextLength (textLength), mText (text), mLocaleName (localeName),
		 mReadingDirection (readingDirection), mCurrentRun (nullptr)
  {
    m_Ref_ଗଣନା.init ();
  }
  ~TextAnalysis ()
  {
    // free runs, except mRunHead which is part of the TextAnalysis object
    ପାଇଁ (Run *run = mRunHead.nextRun; run;)
    {
      Run *origRun = run;
      run = run->nextRun;
      hb_free (origRun);
    }
  }

  STDMETHODIMP
  GenerateResults (IDWriteTextAnalyzer* textAnalyzer, Run **runHead)
  {
    // Analyzes the text using the script analyzer and returns
    // the result as a series of runs.

    HRESULT hr = S_OK;

    // Initially start out with one result that covers the entire range.
    // This result will be subdivided by the analysis processes.
    mRunHead.mTextStart = 0;
    mRunHead.mTextLength = mTextLength;
    mRunHead.mBidiLevel =
      (mReadingDirection == DWRITE_READING_DIRECTION_ଡାହାଣ_TO_ବାମ);
    mRunHead.nextRun = nullptr;
    mCurrentRun = &mRunHead;

    // Call each of the analyzers in sequence, recording their results.
    ଯଦି (SUCCEEDED (hr = textAnalyzer->AnalyzeScript (ଏହି, 0, mTextLength, ଏହି)))
      *runHead = &mRunHead;

    ଫେରନ୍ତୁ hr;
  }

  // IDWriteTextAnalysisSource implementation

  IFACEMETHODIMP
  GetTextAtPosition (uint32_t textPosition,
		     OUT wchar_t ଅଚଳ** textString,
		     OUT uint32_t* textLength)
  {
    ଯଦି (textPosition >= mTextLength)
    {
      // No text at this position, valid query though.
      *textString = nullptr;
      *textLength = 0;
    }
    ଅନ୍ୟଥା
    {
      *textString = mText + textPosition;
      *textLength = mTextLength - textPosition;
    }
    ଫେରନ୍ତୁ S_OK;
  }

  IFACEMETHODIMP
  GetTextBeforePosition (uint32_t textPosition,
			 OUT wchar_t ଅଚଳ** textString,
			 OUT uint32_t* textLength)
  {
    ଯଦି (textPosition == 0 || textPosition > mTextLength)
    {
      // Either there is no text before here (== 0), or this
      // is an invalid position. The query is considered valid though.
      *textString = nullptr;
      *textLength = 0;
    }
    ଅନ୍ୟଥା
    {
      *textString = mText;
      *textLength = textPosition;
    }
    ଫେରନ୍ତୁ S_OK;
  }

  IFACEMETHODIMP_ (DWRITE_READING_DIRECTION)
  GetParagraphReadingDirection () { ଫେରନ୍ତୁ mReadingDirection; }

  IFACEMETHODIMP GetLocaleName (uint32_t textPosition, uint32_t* textLength,
				wchar_t ଅଚଳ** localeName)
  { ଫେରନ୍ତୁ S_OK; }

  IFACEMETHODIMP
  Get_ସଂଖ୍ୟା_Substitution (uint32_t textPosition,
			 OUT uint32_t* textLength,
			 OUT IDWrite_ସଂଖ୍ୟା_Substitution** ସଂଖ୍ୟା_Substitution)
  {
    // We do not support number substitution.
    *ସଂଖ୍ୟା_Substitution = nullptr;
    *textLength = mTextLength - textPosition;

    ଫେରନ୍ତୁ S_OK;
  }

  // IDWriteTextAnalysisSink implementation

  IFACEMETHODIMP
  SetScriptAnalysis (uint32_t textPosition, uint32_t textLength,
		     DWRITE_SCRIPT_ANALYSIS ଅଚଳ* scriptAnalysis)
  {
    SetCurrentRun (textPosition);
    ଯଦି (unlikely (!SplitCurrentRun (textPosition)))
      ଫେରନ୍ତୁ E_OUTOFMEMORY;
    ଯେତେବେଳେ (textLength > 0)
    {
      Run *run = FetchNextRun (&textLength);
      ଯଦି (unlikely (!run))
        ଫେରନ୍ତୁ E_OUTOFMEMORY;
      run->mScript = *scriptAnalysis;
    }

    ଫେରନ୍ତୁ S_OK;
  }

  IFACEMETHODIMP
  Set_ଧାଡ଼ି_Breakpoints (uint32_t textPosition,
		      uint32_t textLength,
		      ଅଚଳ DWRITE_ଧାଡ଼ି_BREAKPOINT* ଧାଡ଼ି_Breakpoints)
  { ଫେରନ୍ତୁ S_OK; }

  IFACEMETHODIMP SetBidiLevel (uint32_t textPosition, uint32_t textLength,
			       uint8_t explicitLevel, uint8_t resolvedLevel)
  { ଫେରନ୍ତୁ S_OK; }

  IFACEMETHODIMP
  Set_ସଂଖ୍ୟା_Substitution (uint32_t textPosition, uint32_t textLength,
			 IDWrite_ସଂଖ୍ୟା_Substitution* ସଂଖ୍ୟା_Substitution)
  { ଫେରନ୍ତୁ S_OK; }

ସୁରକ୍ଷିତ:
  Run *FetchNextRun (IN OUT uint32_t* textLength)
  {
    // Used by the sink setters, this returns a reference to the next run.
    // Position and length are adjusted to now point after the current run
    // being returned.

    Run *origRun = mCurrentRun;
    // Split the tail if needed (the length remaining is less than the
    // current run's size).
    ଯଦି (*textLength < mCurrentRun->mTextLength)
    {
      ଯଦି (unlikely (!SplitCurrentRun (mCurrentRun->mTextStart + *textLength)))
        ଫେରନ୍ତୁ nullptr;
    }
    ଅନ୍ୟଥା
      // Just advance the current run.
      mCurrentRun = mCurrentRun->nextRun;
    *textLength -= origRun->mTextLength;

    // Return a reference to the run that was just current.
    ଫେରନ୍ତୁ origRun;
  }

  ଶୂନ୍ୟ SetCurrentRun (uint32_t textPosition)
  {
    // Move the current run to the given position.
    // Since the analyzers generally return results in a forward manner,
    // this will usually just return early. If not, find the
    // corresponding run for the text position.

    ଯଦି (mCurrentRun && mCurrentRun->ContainsTextPosition (textPosition))
      ଫେରନ୍ତୁ;

    ପାଇଁ (Run *run = &mRunHead; run; run = run->nextRun)
      ଯଦି (run->ContainsTextPosition (textPosition))
      {
	mCurrentRun = run;
	ଫେରନ୍ତୁ;
      }
    ପ୍ରତିଜ୍ଞା (0); // We should always be able to find the text position in one of our runs
  }

  ତାର୍କିକ SplitCurrentRun (uint32_t splitPosition)
  {
    ଯଦି (!mCurrentRun)
    {
      ପ୍ରତିଜ୍ଞା (0); // SplitCurrentRun called without current run
      // Shouldn't be calling this when no current run is set!
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
    // Split the current run.
    ଯଦି (splitPosition <= mCurrentRun->mTextStart)
    {
      // No need to split, already the start of a run
      // or before it. Usually the first.
      ଫେରନ୍ତୁ ସତ୍ୟ;
    }
    Run *newRun = (Run *) hb_malloc (ଆକାର (Run));
    ଯଦି (unlikely (!newRun))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    *newRun = *mCurrentRun;

    // Insert the new run in our linked list.
    newRun->nextRun = mCurrentRun->nextRun;
    mCurrentRun->nextRun = newRun;

    // Adjust runs' text positions and lengths.
    uint32_t splitPoint = splitPosition - mCurrentRun->mTextStart;
    newRun->mTextStart += splitPoint;
    newRun->mTextLength -= splitPoint;
    mCurrentRun->mTextLength = splitPoint;
    mCurrentRun = newRun;
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

ସୁରକ୍ଷିତ:
  //ନିବେଶ
  // (weak references are fine here, since this class is a transient
  //  stack-based helper that doesn't need to copy data)
  uint32_t mTextLength;
  ଅଚଳ wchar_t* mText;
  ଅଚଳ wchar_t* mLocaleName;
  DWRITE_READING_DIRECTION mReadingDirection;

  // Current processing state.
  Run *mCurrentRun;

  // Output is a list of runs starting here
  Run  mRunHead;
};

/*
 * shaper
 */

hb_bool_t
_hb_directwrite_shape (hb_shape_plan_t    *shape_plan,
		       hb_font_t          *font,
		       hb_buffer_t        *buffer,
		       ଅଚଳ hb_feature_t *features,
		       unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା        ଅଙ୍କ_features)
{
  IDWriteFontFace *fontFace = (IDWriteFontFace *) (ଅଚଳ ଶୂନ୍ୟ *) font->data.directwrite;
  auto *global = get_directwrite_global ();
  ଯଦି (unlikely (!global))
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  IDWriteFactory *dwriteFactory = global->dwriteFactory;

  IDWriteTextAnalyzer* analyzer;
  dwriteFactory->CreateTextAnalyzer (&analyzer);

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା scratch_size;
  hb_buffer_t::scratch_buffer_t *scratch = buffer->get_scratch_buffer (&scratch_size);
#define ALLOCATE_ARRAY(Type, name, len) \
  Type *name = (Type *) scratch; \
  କର { \
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା _consumed = DIV_CEIL ((len) * ଆକାର (Type), ଆକାର (*scratch)); \
    ପ୍ରତିଜ୍ଞା (_consumed <= scratch_size); \
    scratch += _consumed; \
    scratch_size -= _consumed; \
  } ଯେତେବେଳେ (0)

#define utf16_index() var1.u32

  ALLOCATE_ARRAY (wchar_t, textString, buffer->len * 2);

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା chars_len = 0;
  ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < buffer->len; i++)
  {
    hb_codepoint_t c = buffer->info[i].codepoint;
    buffer->info[i].utf16_index () = chars_len;
    ଯଦି (likely (c <= 0xFFFFu))
      textString[chars_len++] = c;
    ଅନ୍ୟଥା ଯଦି (unlikely (c > 0x10FFFFu))
      textString[chars_len++] = 0xFFFDu;
    ଅନ୍ୟଥା
    {
      textString[chars_len++] = 0xD800u + ((c - 0x10000u) >> 10);
      textString[chars_len++] = 0xDC00u + ((c - 0x10000u) & ((1u << 10) - 1));
    }
  }

  ALLOCATE_ARRAY (WORD, log_clusters, chars_len);
#undef ALLOCATE_ARRAY
  /* Need log_clusters to assign features. */
  chars_len = 0;
  ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < buffer->len; i++)
  {
    hb_codepoint_t c = buffer->info[i].codepoint;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cluster = buffer->info[i].cluster;
    log_clusters[chars_len++] = cluster;
    ଯଦି (hb_in_range (c, 0x10000u, 0x10FFFFu))
      log_clusters[chars_len++] = cluster; /* Surrogates. */
  }

  DWRITE_READING_DIRECTION readingDirection;
  readingDirection = buffer->props.direction ?
		     DWRITE_READING_DIRECTION_ଡାହାଣ_TO_ବାମ :
		     DWRITE_READING_DIRECTION_ବାମ_TO_ଡାହାଣ;

  /*
  * There's an internal 16-bit limit on some things inside the analyzer,
  * but we never attempt to shape a word longer than 64K characters
  * in a single gfxShapedWord, so we cannot exceed that limit.
  */
  uint32_t textLength = chars_len;

  TextAnalysis analysis (textString, textLength, nullptr, readingDirection);
  TextAnalysis::Run *runHead;
  HRESULT hr;
  hr = analysis.GenerateResults (analyzer, &runHead);

#define FAIL(...) \
  HB_STMT_START { \
    DEBUG_ସନ୍ଦେଶ (DIRECTWRITE, nullptr, __VA_ARGS__); \
    ଫେରନ୍ତୁ ମିଥ୍ୟା; \
  } HB_STMT_END

  ଯଦି (FAILED (hr))
    FAIL ("Analyzer failed to generate results.");

  uint32_t max_Glyph_ଗଣନା = 3 * textLength / 2 + 16;
  uint32_t glyph_ଗଣନା_2;
  ତାର୍କିକ is_ଡାହାଣ_To_ବାମ = HB_DIRECTION_IS_BACKWARD (buffer->props.direction);

  ଅଚଳ wchar_t localeName[20] = {0};
  ଯଦି (buffer->props.language)
    mbstowcs ((wchar_t*) localeName,
	      hb_language_to_string (buffer->props.language), 20);

  /*
   * Set up features.
   */
  static_assert ((ଆକାର (DWRITE_TYPOGRAPHIC_FEATURES) == ଆକାର (hb_ms_features_t)), "");
  static_assert ((ଆକାର (DWRITE_FONT_FEATURE) == ଆକାର (hb_ms_feature_t)), "");
  hb_vector_t<hb_ms_features_t *> range_features;
  hb_vector_t<uint32_t> range_char_counts;

  // https://github.com/harfbuzz/harfbuzz/pull/5114
  // The data allocated by these two vectors are used by the above two, so they
  // should remain alive as long as the above two are.
  hb_vector_t<hb_ms_feature_t> feature_records;
  hb_vector_t<hb_ms_range_record_t> range_records;
  ଯଦି (ଅଙ୍କ_features)
  {
    ଯଦି (hb_ms_setup_features (features, ଅଙ୍କ_features, feature_records, range_records))
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
  ଯଦି (unlikely (!clusterMap.resize_exact (textLength) ||
                !textProperties.resize_exact (textLength)))
    FAIL ("Failed to allocate shaping text data.");

  hb_vector_t<uint16_t> glyphIndices;
  hb_vector_t<DWRITE_SHAPING_GLYPH_PROPERTIES> glyphProperties;

retry_getglyphs:
  ଯଦି (unlikely (!glyphIndices.resize_exact (max_Glyph_ଗଣନା) ||
                !glyphProperties.resize_exact (max_Glyph_ଗଣନା)))
    FAIL ("Failed to allocate shaping glyph data.");

  hr = analyzer->GetGlyphs (textString,
			    chars_len,
			    fontFace,
			    ମିଥ୍ୟା,
			    is_ଡାହାଣ_To_ବାମ,
			    &runHead->mScript,
			    localeName,
			    nullptr,
			    (ଅଚଳ DWRITE_TYPOGRAPHIC_FEATURES**) range_features.arrayZ,
			    range_char_counts.arrayZ,
			    range_features.length,
			    max_Glyph_ଗଣନା,
			    clusterMap.arrayZ,
			    textProperties.arrayZ,
			    glyphIndices.arrayZ,
			    glyphProperties.arrayZ,
			    &glyph_ଗଣନା_2);

  ଯଦି (unlikely (hr == HRESULT_FROM_WIN32 (ERROR_INSUFFICIENT_BUFFER)))
  {
    max_Glyph_ଗଣନା *= 2;

    ଯାଅ retry_getglyphs;
  }
  ଯଦି (FAILED (hr))
    FAIL ("Analyzer failed to get glyphs.");

  hb_vector_t<ଦଶମିକ> glyphAdvances;
  hb_vector_t<DWRITE_GLYPH_OFFSET> glyphOffsets;
  ଯଦି (unlikely (!glyphAdvances.resize_exact (max_Glyph_ଗଣନା) ||
                !glyphOffsets.resize_exact (max_Glyph_ଗଣନା)))
    FAIL ("Failed to allocate glyph positioning data.");


  unsigned fontEmSize = font->face->get_upem ();

  ଦଶମିକ x_mult = font->x_multf;
  ଦଶମିକ y_mult = font->y_multf;

  hr = analyzer->GetGlyphPlacements (textString,
				     clusterMap.arrayZ,
				     textProperties.arrayZ,
				     chars_len,
				     glyphIndices.arrayZ,
				     glyphProperties.arrayZ,
				     glyph_ଗଣନା_2,
				     fontFace,
				     fontEmSize,
				     ମିଥ୍ୟା,
				     is_ଡାହାଣ_To_ବାମ,
				     &runHead->mScript,
				     localeName,
				     (ଅଚଳ DWRITE_TYPOGRAPHIC_FEATURES**) range_features.arrayZ,
				     range_char_counts.arrayZ,
				     range_features.length,
				     glyphAdvances.arrayZ,
				     glyphOffsets.arrayZ);

  ଯଦି (FAILED (hr))
    FAIL ("Analyzer failed to get glyph placements.");

  /* Ok, we've got everything we need, now compose output buffer,
   * very, *very*, carefully! */

  /* Calculate visual-clusters.  That's what we ship. */
  hb_vector_t<uint32_t> vis_clusters;
  ଯଦି (unlikely (!vis_clusters.resize_exact (glyph_ଗଣନା_2)))
    FAIL ("Failed to allocate visual clusters.");

  ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < glyph_ଗଣନା_2; i++)
    vis_clusters[i] = (uint32_t) -1;
  ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < buffer->len; i++)
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା c = clusterMap[buffer->info[i].utf16_index ()];
    ଯଦି (likely (c < glyph_ଗଣନା_2))
      vis_clusters[c] = hb_min (vis_clusters[c], buffer->info[i].cluster);
  }
  ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 1; i < glyph_ଗଣନା_2; i++)
    ଯଦି (vis_clusters[i] == (uint32_t) -1)
      vis_clusters[i] = vis_clusters[i - 1];
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) glyph_ଗଣନା_2 - 2; i >= 0; i--)
    ଯଦି (vis_clusters[i] == (uint32_t) -1)
      vis_clusters[i] = vis_clusters[i + 1];

#undef utf16_index

  ଯଦି (unlikely (!buffer->ensure (glyph_ଗଣନା_2)))
    FAIL ("Buffer in error");

#undef FAIL

  /* Set glyph infos */
  buffer->len = 0;
  ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < glyph_ଗଣନା_2; i++)
  {
    hb_glyph_info_t *info = &buffer->info[buffer->len++];

    info->codepoint = glyphIndices[i];
    info->cluster = vis_clusters[i];

    /* The rest is crap.  Let's store position info there for now. */
    info->mask = (hb_mask_t) hb_clamp_to<int32_t> (glyphAdvances[i]);
    info->var1.i32 = hb_clamp_to<int32_t> (glyphOffsets[i].advanceOffset);
    info->var2.i32 = hb_clamp_to<int32_t> (glyphOffsets[i].ascenderOffset);
  }

  /* Set glyph positions */
  buffer->clear_positions ();
  ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < glyph_ଗଣନା_2; i++)
  {
    hb_glyph_info_t *info = &buffer->info[i];
    hb_glyph_position_t *pos = &buffer->pos[i];

    /* TODO vertical */
    pos->x_advance = hb_clamp_to<hb_position_t> (round (x_mult * (int32_t) info->mask));
    ଦ୍ୱିଗୁଣ x_offset = is_ଡାହାଣ_To_ବାମ ? -(ଦ୍ୱିଗୁଣ) info->var1.i32 : info->var1.i32;
    pos->x_offset = hb_clamp_to<hb_position_t> (round ((ଦ୍ୱିଗୁଣ) x_mult * x_offset));
    pos->y_offset = hb_clamp_to<hb_position_t> (round (y_mult * info->var2.i32));
  }

  ଯଦି (is_ଡାହାଣ_To_ବାମ) hb_buffer_reverse (buffer);

  buffer->clear_glyph_flags ();
  buffer->unsafe_to_break ();

  /* Wow, done! */
  ଫେରନ୍ତୁ ସତ୍ୟ;
}


#endif
