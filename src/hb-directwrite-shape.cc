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

#ಅನ್ತರ್ಭಾವ "hb.hh"

#ifdef HAVE_DIRECTWRITE

#ಅನ್ತರ್ಭಾವ "hb-shaper-impl.hh"

#ಅನ್ತರ್ಭಾವ "hb-directwrite.hh"

#ಅನ್ತರ್ಭಾವ "hb-ms-feature-ranges.hh"


/*
* shaper face data
*/

hb_directwrite_face_data_t *
_hb_directwrite_shaper_face_data_create (hb_face_t *face)
{
  hb_blob_t *blob = hb_face_reference_blob (face);

  hb_directwrite_face_data_t *data = (hb_directwrite_face_data_t *) dw_face_create (blob, face->index);

  hb_blob_destroy (blob);

  ಪ್ರತ್ಯಾಗಮ data;
}

ಶೂನ್ಯ
_hb_directwrite_shaper_face_data_destroy (hb_directwrite_face_data_t *data)
{
  ((IDWriteFontFace *) data)->Release ();
}


/*
 * shaper font data
 */

ಸಂರಚನಾ hb_directwrite_font_data_t {};

hb_directwrite_font_data_t *
_hb_directwrite_shaper_font_data_create (hb_font_t *font)
{
  IDWriteFontFace *fontFace = (IDWriteFontFace *) (ಧ್ರುವ ಶೂನ್ಯ *) font->face->data.directwrite;

  /*
   * Set up variations.
   */
  IDWriteFontFace5 *fontFaceVariations = nullptr;
  {
    IDWriteFontFace5 *fontFace5;
    ಯದಿ (SUCCEEDED (fontFace->QueryInterface (__uuidof (IDWriteFontFace5), (ಶೂನ್ಯ **) &fontFace5)))
    {
      IDWriteFontResource *fontResource;
      ಯದಿ (SUCCEEDED (fontFace5->GetFontResource (&fontResource)))
      {
	hb_vector_t<DWRITE_FONT_AXIS_ಮೌಲ್ಯ> axis_ಮೌಲ್ಯಗಳು;
	ಯದಿ (likely (axis_ಮೌಲ್ಯಗಳು.resize_exact (font->ಅಂಕೆ_coords)))
	{
	  ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < font->ಅಂಕೆ_coords; i++)
	  {
	    hb_ot_var_axis_info_t info;
	    unsigned ಪೂರ್ಣಾಙ್ಕ c = 1;
	    hb_ot_var_get_axis_infos (font->face, i, &c, &info);
	    axis_ಮೌಲ್ಯಗಳು[i].axisTag = (DWRITE_FONT_AXIS_TAG) hb_uint32_swap (info.tag);
	    axis_ಮೌಲ್ಯಗಳು[i].value = i < font->ಅಂಕೆ_coords ?
				   hb_clamp (font->design_coords[i], info.min_ಮೌಲ್ಯ, info.max_ಮೌಲ್ಯ) :
				   info.default_ಮೌಲ್ಯ;
	  }

	  fontResource->CreateFontFace (DWRITE_FONT_SIMULATIONS::DWRITE_FONT_SIMULATIONS_NONE,
					axis_ಮೌಲ್ಯಗಳು.arrayZ, axis_ಮೌಲ್ಯಗಳು.length, &fontFaceVariations);
	}
	fontResource->Release ();
      }
      fontFace5->Release ();
    }
  }

  ಪ್ರತ್ಯಾಗಮ (hb_directwrite_font_data_t *) fontFaceVariations;
}

ಶೂನ್ಯ
_hb_directwrite_shaper_font_data_destroy (hb_directwrite_font_data_t *data)
{
  ((IDWriteFontFace *) (ಧ್ರುವ ಶೂನ್ಯ *) data)->Release ();
}


// Most of TextAnalysis is originally written by Bas Schouten for Mozilla project
// but now is relicensed to MIT for HarfBuzz use
ವರ್ಗ TextAnalysis : ಸಾರ್ವಜನಿಕ IDWriteTextAnalysisSource, ಸಾರ್ವಜನಿಕ IDWriteTextAnalysisSink
{
ಗುಪ್ತ:
  hb_reference_ಎಣಿಕೆ_t m_Ref_ಎಣಿಕೆ;
ಸಾರ್ವಜನಿಕ:
  IFACEMETHOD (QueryInterface) (IID ಧ್ರುವ& iid, OUT ಶೂನ್ಯ** ppObject)
  { ಪ್ರತ್ಯಾಗಮ S_OK; }
  IFACEMETHOD_ (ULONG, AddRef) ()
  {
    ಪ್ರತ್ಯಾಗಮ m_Ref_ಎಣಿಕೆ.inc () + 1;
  }
  IFACEMETHOD_ (ULONG, Release) ()
  {
    signed ref_ಎಣಿಕೆ = m_Ref_ಎಣಿಕೆ.dec () - 1;
    ಪ್ರತಿಜ್ಞಾ (ref_ಎಣಿಕೆ >= 0);
    ಯದಿ (ref_ಎಣಿಕೆ)
      ಪ್ರತ್ಯಾಗಮ ref_ಎಣಿಕೆ;
    ಪ್ರತ್ಯಾಗಮ 0;
  }

  // A single contiguous run of characters containing the same analysis
  // results.
  ಸಂರಚನಾ Run
  {
    uint32_t mTextStart;   // starting text position of this run
    uint32_t mTextLength;  // number of contiguous code units covered
    uint32_t mGlyphStart;  // starting glyph in the glyphs array
    uint32_t m_Glyph_ಎಣಿಕೆ;  // number of glyphs associated with this run
    // text
    DWRITE_SCRIPT_ANALYSIS mScript;
    uint8_t mBidiLevel;
    ತಾರ್ಕಿಕ mIsSideways;

    ತಾರ್ಕಿಕ ContainsTextPosition (uint32_t aTextPosition) ಧ್ರುವ
    {
      ಪ್ರತ್ಯಾಗಮ aTextPosition >= mTextStart &&
	     aTextPosition <  mTextStart + mTextLength;
    }

    Run *nextRun;
  };

ಸಾರ್ವಜನಿಕ:
  TextAnalysis (ಧ್ರುವ wchar_t* text, uint32_t textLength,
		ಧ್ರುವ wchar_t* localeName, DWRITE_READING_DIRECTION readingDirection)
	       : mTextLength (textLength), mText (text), mLocaleName (localeName),
		 mReadingDirection (readingDirection), mCurrentRun (nullptr)
  {
    m_Ref_ಎಣಿಕೆ.init ();
  }
  ~TextAnalysis ()
  {
    // free runs, except mRunHead which is part of the TextAnalysis object
    ಪ್ರತಿ (Run *run = mRunHead.nextRun; run;)
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
      (mReadingDirection == DWRITE_READING_DIRECTION_ಬಲ_TO_ಎಡ);
    mRunHead.nextRun = nullptr;
    mCurrentRun = &mRunHead;

    // Call each of the analyzers in sequence, recording their results.
    ಯದಿ (SUCCEEDED (hr = textAnalyzer->AnalyzeScript (ಏತತ್, 0, mTextLength, ಏತತ್)))
      *runHead = &mRunHead;

    ಪ್ರತ್ಯಾಗಮ hr;
  }

  // IDWriteTextAnalysisSource implementation

  IFACEMETHODIMP
  GetTextAtPosition (uint32_t textPosition,
		     OUT wchar_t ಧ್ರುವ** textString,
		     OUT uint32_t* textLength)
  {
    ಯದಿ (textPosition >= mTextLength)
    {
      // No text at this position, valid query though.
      *textString = nullptr;
      *textLength = 0;
    }
    ಅನ್ಯಥಾ
    {
      *textString = mText + textPosition;
      *textLength = mTextLength - textPosition;
    }
    ಪ್ರತ್ಯಾಗಮ S_OK;
  }

  IFACEMETHODIMP
  GetTextBeforePosition (uint32_t textPosition,
			 OUT wchar_t ಧ್ರುವ** textString,
			 OUT uint32_t* textLength)
  {
    ಯದಿ (textPosition == 0 || textPosition > mTextLength)
    {
      // Either there is no text before here (== 0), or this
      // is an invalid position. The query is considered valid though.
      *textString = nullptr;
      *textLength = 0;
    }
    ಅನ್ಯಥಾ
    {
      *textString = mText;
      *textLength = textPosition;
    }
    ಪ್ರತ್ಯಾಗಮ S_OK;
  }

  IFACEMETHODIMP_ (DWRITE_READING_DIRECTION)
  GetParagraphReadingDirection () { ಪ್ರತ್ಯಾಗಮ mReadingDirection; }

  IFACEMETHODIMP GetLocaleName (uint32_t textPosition, uint32_t* textLength,
				wchar_t ಧ್ರುವ** localeName)
  { ಪ್ರತ್ಯಾಗಮ S_OK; }

  IFACEMETHODIMP
  Get_ಸಂಖ್ಯೆ_Substitution (uint32_t textPosition,
			 OUT uint32_t* textLength,
			 OUT IDWrite_ಸಂಖ್ಯೆ_Substitution** ಸಂಖ್ಯೆ_Substitution)
  {
    // We do not support number substitution.
    *ಸಂಖ್ಯೆ_Substitution = nullptr;
    *textLength = mTextLength - textPosition;

    ಪ್ರತ್ಯಾಗಮ S_OK;
  }

  // IDWriteTextAnalysisSink implementation

  IFACEMETHODIMP
  SetScriptAnalysis (uint32_t textPosition, uint32_t textLength,
		     DWRITE_SCRIPT_ANALYSIS ಧ್ರುವ* scriptAnalysis)
  {
    SetCurrentRun (textPosition);
    ಯದಿ (unlikely (!SplitCurrentRun (textPosition)))
      ಪ್ರತ್ಯಾಗಮ E_OUTOFMEMORY;
    ಯಾವತ್ (textLength > 0)
    {
      Run *run = FetchNextRun (&textLength);
      ಯದಿ (unlikely (!run))
        ಪ್ರತ್ಯಾಗಮ E_OUTOFMEMORY;
      run->mScript = *scriptAnalysis;
    }

    ಪ್ರತ್ಯಾಗಮ S_OK;
  }

  IFACEMETHODIMP
  Set_ಸಾಲು_Breakpoints (uint32_t textPosition,
		      uint32_t textLength,
		      ಧ್ರುವ DWRITE_ಸಾಲು_BREAKPOINT* ಸಾಲು_Breakpoints)
  { ಪ್ರತ್ಯಾಗಮ S_OK; }

  IFACEMETHODIMP SetBidiLevel (uint32_t textPosition, uint32_t textLength,
			       uint8_t explicitLevel, uint8_t resolvedLevel)
  { ಪ್ರತ್ಯಾಗಮ S_OK; }

  IFACEMETHODIMP
  Set_ಸಂಖ್ಯೆ_Substitution (uint32_t textPosition, uint32_t textLength,
			 IDWrite_ಸಂಖ್ಯೆ_Substitution* ಸಂಖ್ಯೆ_Substitution)
  { ಪ್ರತ್ಯಾಗಮ S_OK; }

ರಕ್ಷಿತ:
  Run *FetchNextRun (IN OUT uint32_t* textLength)
  {
    // Used by the sink setters, this returns a reference to the next run.
    // Position and length are adjusted to now point after the current run
    // being returned.

    Run *origRun = mCurrentRun;
    // Split the tail if needed (the length remaining is less than the
    // current run's size).
    ಯದಿ (*textLength < mCurrentRun->mTextLength)
    {
      ಯದಿ (unlikely (!SplitCurrentRun (mCurrentRun->mTextStart + *textLength)))
        ಪ್ರತ್ಯಾಗಮ nullptr;
    }
    ಅನ್ಯಥಾ
      // Just advance the current run.
      mCurrentRun = mCurrentRun->nextRun;
    *textLength -= origRun->mTextLength;

    // Return a reference to the run that was just current.
    ಪ್ರತ್ಯಾಗಮ origRun;
  }

  ಶೂನ್ಯ SetCurrentRun (uint32_t textPosition)
  {
    // Move the current run to the given position.
    // Since the analyzers generally return results in a forward manner,
    // this will usually just return early. If not, find the
    // corresponding run for the text position.

    ಯದಿ (mCurrentRun && mCurrentRun->ContainsTextPosition (textPosition))
      ಪ್ರತ್ಯಾಗಮ;

    ಪ್ರತಿ (Run *run = &mRunHead; run; run = run->nextRun)
      ಯದಿ (run->ContainsTextPosition (textPosition))
      {
	mCurrentRun = run;
	ಪ್ರತ್ಯಾಗಮ;
      }
    ಪ್ರತಿಜ್ಞಾ (0); // We should always be able to find the text position in one of our runs
  }

  ತಾರ್ಕಿಕ SplitCurrentRun (uint32_t splitPosition)
  {
    ಯದಿ (!mCurrentRun)
    {
      ಪ್ರತಿಜ್ಞಾ (0); // SplitCurrentRun called without current run
      // Shouldn't be calling this when no current run is set!
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
    // Split the current run.
    ಯದಿ (splitPosition <= mCurrentRun->mTextStart)
    {
      // No need to split, already the start of a run
      // or before it. Usually the first.
      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }
    Run *newRun = (Run *) hb_malloc (ಪರಿಮಾಣ (Run));
    ಯದಿ (unlikely (!newRun))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

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
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

ರಕ್ಷಿತ:
  //ಒಳಹರಿವು
  // (weak references are fine here, since this class is a transient
  //  stack-based helper that doesn't need to copy data)
  uint32_t mTextLength;
  ಧ್ರುವ wchar_t* mText;
  ಧ್ರುವ wchar_t* mLocaleName;
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
		       ಧ್ರುವ hb_feature_t *features,
		       unsigned ಪೂರ್ಣಾಙ್ಕ        ಅಂಕೆ_features)
{
  IDWriteFontFace *fontFace = (IDWriteFontFace *) (ಧ್ರುವ ಶೂನ್ಯ *) font->data.directwrite;
  auto *global = get_directwrite_global ();
  ಯದಿ (unlikely (!global))
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  IDWriteFactory *dwriteFactory = global->dwriteFactory;

  IDWriteTextAnalyzer* analyzer;
  dwriteFactory->CreateTextAnalyzer (&analyzer);

  unsigned ಪೂರ್ಣಾಙ್ಕ scratch_size;
  hb_buffer_t::scratch_buffer_t *scratch = buffer->get_scratch_buffer (&scratch_size);
#define ALLOCATE_ARRAY(Type, name, len) \
  Type *name = (Type *) scratch; \
  ಕುರು { \
    unsigned ಪೂರ್ಣಾಙ್ಕ _consumed = DIV_CEIL ((len) * ಪರಿಮಾಣ (Type), ಪರಿಮಾಣ (*scratch)); \
    ಪ್ರತಿಜ್ಞಾ (_consumed <= scratch_size); \
    scratch += _consumed; \
    scratch_size -= _consumed; \
  } ಯಾವತ್ (0)

#define utf16_index() var1.u32

  ALLOCATE_ARRAY (wchar_t, textString, buffer->len * 2);

  unsigned ಪೂರ್ಣಾಙ್ಕ chars_len = 0;
  ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < buffer->len; i++)
  {
    hb_codepoint_t c = buffer->info[i].codepoint;
    buffer->info[i].utf16_index () = chars_len;
    ಯದಿ (likely (c <= 0xFFFFu))
      textString[chars_len++] = c;
    ಅನ್ಯಥಾ ಯದಿ (unlikely (c > 0x10FFFFu))
      textString[chars_len++] = 0xFFFDu;
    ಅನ್ಯಥಾ
    {
      textString[chars_len++] = 0xD800u + ((c - 0x10000u) >> 10);
      textString[chars_len++] = 0xDC00u + ((c - 0x10000u) & ((1u << 10) - 1));
    }
  }

  ALLOCATE_ARRAY (WORD, log_clusters, chars_len);
#undef ALLOCATE_ARRAY
  /* Need log_clusters to assign features. */
  chars_len = 0;
  ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < buffer->len; i++)
  {
    hb_codepoint_t c = buffer->info[i].codepoint;
    unsigned ಪೂರ್ಣಾಙ್ಕ cluster = buffer->info[i].cluster;
    log_clusters[chars_len++] = cluster;
    ಯದಿ (hb_in_range (c, 0x10000u, 0x10FFFFu))
      log_clusters[chars_len++] = cluster; /* Surrogates. */
  }

  DWRITE_READING_DIRECTION readingDirection;
  readingDirection = buffer->props.direction ?
		     DWRITE_READING_DIRECTION_ಬಲ_TO_ಎಡ :
		     DWRITE_READING_DIRECTION_ಎಡ_TO_ಬಲ;

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
    DEBUG_ಸಂದೇಶ (DIRECTWRITE, nullptr, __VA_ARGS__); \
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ; \
  } HB_STMT_END

  ಯದಿ (FAILED (hr))
    FAIL ("Analyzer failed to generate results.");

  uint32_t max_Glyph_ಎಣಿಕೆ = 3 * textLength / 2 + 16;
  uint32_t glyph_ಎಣಿಕೆ_2;
  ತಾರ್ಕಿಕ is_ಬಲ_To_ಎಡ = HB_DIRECTION_IS_BACKWARD (buffer->props.direction);

  ಧ್ರುವ wchar_t localeName[20] = {0};
  ಯದಿ (buffer->props.language)
    mbstowcs ((wchar_t*) localeName,
	      hb_language_to_string (buffer->props.language), 20);

  /*
   * Set up features.
   */
  static_assert ((ಪರಿಮಾಣ (DWRITE_TYPOGRAPHIC_FEATURES) == ಪರಿಮಾಣ (hb_ms_features_t)), "");
  static_assert ((ಪರಿಮಾಣ (DWRITE_FONT_FEATURE) == ಪರಿಮಾಣ (hb_ms_feature_t)), "");
  hb_vector_t<hb_ms_features_t *> range_features;
  hb_vector_t<uint32_t> range_char_counts;

  // https://github.com/harfbuzz/harfbuzz/pull/5114
  // The data allocated by these two vectors are used by the above two, so they
  // should remain alive as long as the above two are.
  hb_vector_t<hb_ms_feature_t> feature_records;
  hb_vector_t<hb_ms_range_record_t> range_records;
  ಯದಿ (ಅಂಕೆ_features)
  {
    ಯದಿ (hb_ms_setup_features (features, ಅಂಕೆ_features, feature_records, range_records))
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
  ಯದಿ (unlikely (!clusterMap.resize_exact (textLength) ||
                !textProperties.resize_exact (textLength)))
    FAIL ("Failed to allocate shaping text data.");

  hb_vector_t<uint16_t> glyphIndices;
  hb_vector_t<DWRITE_SHAPING_GLYPH_PROPERTIES> glyphProperties;

retry_getglyphs:
  ಯದಿ (unlikely (!glyphIndices.resize_exact (max_Glyph_ಎಣಿಕೆ) ||
                !glyphProperties.resize_exact (max_Glyph_ಎಣಿಕೆ)))
    FAIL ("Failed to allocate shaping glyph data.");

  hr = analyzer->GetGlyphs (textString,
			    chars_len,
			    fontFace,
			    ಅಸತ್ಯ,
			    is_ಬಲ_To_ಎಡ,
			    &runHead->mScript,
			    localeName,
			    nullptr,
			    (ಧ್ರುವ DWRITE_TYPOGRAPHIC_FEATURES**) range_features.arrayZ,
			    range_char_counts.arrayZ,
			    range_features.length,
			    max_Glyph_ಎಣಿಕೆ,
			    clusterMap.arrayZ,
			    textProperties.arrayZ,
			    glyphIndices.arrayZ,
			    glyphProperties.arrayZ,
			    &glyph_ಎಣಿಕೆ_2);

  ಯದಿ (unlikely (hr == HRESULT_FROM_WIN32 (ERROR_INSUFFICIENT_BUFFER)))
  {
    max_Glyph_ಎಣಿಕೆ *= 2;

    ಗಚ್ಛ retry_getglyphs;
  }
  ಯದಿ (FAILED (hr))
    FAIL ("Analyzer failed to get glyphs.");

  hb_vector_t<ಭಿನ್ನ> glyphAdvances;
  hb_vector_t<DWRITE_GLYPH_OFFSET> glyphOffsets;
  ಯದಿ (unlikely (!glyphAdvances.resize_exact (max_Glyph_ಎಣಿಕೆ) ||
                !glyphOffsets.resize_exact (max_Glyph_ಎಣಿಕೆ)))
    FAIL ("Failed to allocate glyph positioning data.");


  unsigned fontEmSize = font->face->get_upem ();

  ಭಿನ್ನ x_mult = font->x_multf;
  ಭಿನ್ನ y_mult = font->y_multf;

  hr = analyzer->GetGlyphPlacements (textString,
				     clusterMap.arrayZ,
				     textProperties.arrayZ,
				     chars_len,
				     glyphIndices.arrayZ,
				     glyphProperties.arrayZ,
				     glyph_ಎಣಿಕೆ_2,
				     fontFace,
				     fontEmSize,
				     ಅಸತ್ಯ,
				     is_ಬಲ_To_ಎಡ,
				     &runHead->mScript,
				     localeName,
				     (ಧ್ರುವ DWRITE_TYPOGRAPHIC_FEATURES**) range_features.arrayZ,
				     range_char_counts.arrayZ,
				     range_features.length,
				     glyphAdvances.arrayZ,
				     glyphOffsets.arrayZ);

  ಯದಿ (FAILED (hr))
    FAIL ("Analyzer failed to get glyph placements.");

  /* Ok, we've got everything we need, now compose output buffer,
   * very, *very*, carefully! */

  /* Calculate visual-clusters.  That's what we ship. */
  hb_vector_t<uint32_t> vis_clusters;
  ಯದಿ (unlikely (!vis_clusters.resize_exact (glyph_ಎಣಿಕೆ_2)))
    FAIL ("Failed to allocate visual clusters.");

  ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < glyph_ಎಣಿಕೆ_2; i++)
    vis_clusters[i] = (uint32_t) -1;
  ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < buffer->len; i++)
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ c = clusterMap[buffer->info[i].utf16_index ()];
    ಯದಿ (likely (c < glyph_ಎಣಿಕೆ_2))
      vis_clusters[c] = hb_min (vis_clusters[c], buffer->info[i].cluster);
  }
  ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 1; i < glyph_ಎಣಿಕೆ_2; i++)
    ಯದಿ (vis_clusters[i] == (uint32_t) -1)
      vis_clusters[i] = vis_clusters[i - 1];
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = (ಪೂರ್ಣಾಙ್ಕ) glyph_ಎಣಿಕೆ_2 - 2; i >= 0; i--)
    ಯದಿ (vis_clusters[i] == (uint32_t) -1)
      vis_clusters[i] = vis_clusters[i + 1];

#undef utf16_index

  ಯದಿ (unlikely (!buffer->ensure (glyph_ಎಣಿಕೆ_2)))
    FAIL ("Buffer in error");

#undef FAIL

  /* Set glyph infos */
  buffer->len = 0;
  ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < glyph_ಎಣಿಕೆ_2; i++)
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
  ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < glyph_ಎಣಿಕೆ_2; i++)
  {
    hb_glyph_info_t *info = &buffer->info[i];
    hb_glyph_position_t *pos = &buffer->pos[i];

    /* TODO vertical */
    pos->x_advance = hb_clamp_to<hb_position_t> (round (x_mult * (int32_t) info->mask));
    ದ್ವಿಗುಣ x_offset = is_ಬಲ_To_ಎಡ ? -(ದ್ವಿಗುಣ) info->var1.i32 : info->var1.i32;
    pos->x_offset = hb_clamp_to<hb_position_t> (round ((ದ್ವಿಗುಣ) x_mult * x_offset));
    pos->y_offset = hb_clamp_to<hb_position_t> (round (y_mult * info->var2.i32));
  }

  ಯದಿ (is_ಬಲ_To_ಎಡ) hb_buffer_reverse (buffer);

  buffer->clear_glyph_flags ();
  buffer->unsafe_to_break ();

  /* Wow, done! */
  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}


#endif
