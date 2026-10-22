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

#അന്തര്ഭാവ "hb.hh"

#ifdef HAVE_DIRECTWRITE

#അന്തര്ഭാവ "hb-shaper-impl.hh"

#അന്തര്ഭാവ "hb-directwrite.hh"

#അന്തര്ഭാവ "hb-ms-feature-ranges.hh"


/*
* shaper face data
*/

hb_directwrite_face_data_t *
_hb_directwrite_shaper_face_data_create (hb_face_t *face)
{
  hb_blob_t *blob = hb_face_reference_blob (face);

  hb_directwrite_face_data_t *data = (hb_directwrite_face_data_t *) dw_face_create (blob, face->index);

  hb_blob_destroy (blob);

  തിരികെ data;
}

ശൂന്യം
_hb_directwrite_shaper_face_data_destroy (hb_directwrite_face_data_t *data)
{
  ((IDWriteFontFace *) data)->Release ();
}


/*
 * shaper font data
 */

ഘടന hb_directwrite_font_data_t {};

hb_directwrite_font_data_t *
_hb_directwrite_shaper_font_data_create (hb_font_t *font)
{
  IDWriteFontFace *fontFace = (IDWriteFontFace *) (സ്ഥിരം ശൂന്യം *) font->face->data.directwrite;

  /*
   * Set up variations.
   */
  IDWriteFontFace5 *fontFaceVariations = nullptr;
  {
    IDWriteFontFace5 *fontFace5;
    എങ്കിൽ (SUCCEEDED (fontFace->QueryInterface (__uuidof (IDWriteFontFace5), (ശൂന്യം **) &fontFace5)))
    {
      IDWriteFontResource *fontResource;
      എങ്കിൽ (SUCCEEDED (fontFace5->GetFontResource (&fontResource)))
      {
	hb_vector_t<DWRITE_FONT_AXIS_മൂല്യം> axis_മൂല്യങ്ങൾ;
	എങ്കിൽ (likely (axis_മൂല്യങ്ങൾ.resize_exact (font->അക്കം_coords)))
	{
	  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < font->അക്കം_coords; i++)
	  {
	    hb_ot_var_axis_info_t info;
	    unsigned പൂർണ്ണസംഖ്യ c = 1;
	    hb_ot_var_get_axis_infos (font->face, i, &c, &info);
	    axis_മൂല്യങ്ങൾ[i].axisTag = (DWRITE_FONT_AXIS_TAG) hb_uint32_swap (info.tag);
	    axis_മൂല്യങ്ങൾ[i].value = i < font->അക്കം_coords ?
				   hb_clamp (font->design_coords[i], info.min_മൂല്യം, info.max_മൂല്യം) :
				   info.default_മൂല്യം;
	  }

	  fontResource->CreateFontFace (DWRITE_FONT_SIMULATIONS::DWRITE_FONT_SIMULATIONS_NONE,
					axis_മൂല്യങ്ങൾ.arrayZ, axis_മൂല്യങ്ങൾ.length, &fontFaceVariations);
	}
	fontResource->Release ();
      }
      fontFace5->Release ();
    }
  }

  തിരികെ (hb_directwrite_font_data_t *) fontFaceVariations;
}

ശൂന്യം
_hb_directwrite_shaper_font_data_destroy (hb_directwrite_font_data_t *data)
{
  ((IDWriteFontFace *) (സ്ഥിരം ശൂന്യം *) data)->Release ();
}


// Most of TextAnalysis is originally written by Bas Schouten for Mozilla project
// but now is relicensed to MIT for HarfBuzz use
വർഗ്ഗം TextAnalysis : പൊതു IDWriteTextAnalysisSource, പൊതു IDWriteTextAnalysisSink
{
സ്വകാര്യ:
  hb_reference_എണ്ണം_t m_Ref_എണ്ണം;
പൊതു:
  IFACEMETHOD (QueryInterface) (IID സ്ഥിരം& iid, OUT ശൂന്യം** ppObject)
  { തിരികെ S_OK; }
  IFACEMETHOD_ (ULONG, AddRef) ()
  {
    തിരികെ m_Ref_എണ്ണം.inc () + 1;
  }
  IFACEMETHOD_ (ULONG, Release) ()
  {
    signed ref_എണ്ണം = m_Ref_എണ്ണം.dec () - 1;
    പ്രതിജ്ഞാ (ref_എണ്ണം >= 0);
    എങ്കിൽ (ref_എണ്ണം)
      തിരികെ ref_എണ്ണം;
    തിരികെ 0;
  }

  // A single contiguous run of characters containing the same analysis
  // results.
  ഘടന Run
  {
    uint32_t mTextStart;   // starting text position of this run
    uint32_t mTextLength;  // number of contiguous code units covered
    uint32_t mGlyphStart;  // starting glyph in the glyphs array
    uint32_t m_Glyph_എണ്ണം;  // number of glyphs associated with this run
    // text
    DWRITE_SCRIPT_ANALYSIS mScript;
    uint8_t mBidiLevel;
    തർക്കം mIsSideways;

    തർക്കം ContainsTextPosition (uint32_t aTextPosition) സ്ഥിരം
    {
      തിരികെ aTextPosition >= mTextStart &&
	     aTextPosition <  mTextStart + mTextLength;
    }

    Run *nextRun;
  };

പൊതു:
  TextAnalysis (സ്ഥിരം wchar_t* text, uint32_t textLength,
		സ്ഥിരം wchar_t* localeName, DWRITE_READING_DIRECTION readingDirection)
	       : mTextLength (textLength), mText (text), mLocaleName (localeName),
		 mReadingDirection (readingDirection), mCurrentRun (nullptr)
  {
    m_Ref_എണ്ണം.init ();
  }
  ~TextAnalysis ()
  {
    // free runs, except mRunHead which is part of the TextAnalysis object
    വേണ്ടി (Run *run = mRunHead.nextRun; run;)
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
      (mReadingDirection == DWRITE_READING_DIRECTION_വലത്_TO_ഇടത്);
    mRunHead.nextRun = nullptr;
    mCurrentRun = &mRunHead;

    // Call each of the analyzers in sequence, recording their results.
    എങ്കിൽ (SUCCEEDED (hr = textAnalyzer->AnalyzeScript (ഇത്, 0, mTextLength, ഇത്)))
      *runHead = &mRunHead;

    തിരികെ hr;
  }

  // IDWriteTextAnalysisSource implementation

  IFACEMETHODIMP
  GetTextAtPosition (uint32_t textPosition,
		     OUT wchar_t സ്ഥിരം** textString,
		     OUT uint32_t* textLength)
  {
    എങ്കിൽ (textPosition >= mTextLength)
    {
      // No text at this position, valid query though.
      *textString = nullptr;
      *textLength = 0;
    }
    അല്ലെങ്കിൽ
    {
      *textString = mText + textPosition;
      *textLength = mTextLength - textPosition;
    }
    തിരികെ S_OK;
  }

  IFACEMETHODIMP
  GetTextBeforePosition (uint32_t textPosition,
			 OUT wchar_t സ്ഥിരം** textString,
			 OUT uint32_t* textLength)
  {
    എങ്കിൽ (textPosition == 0 || textPosition > mTextLength)
    {
      // Either there is no text before here (== 0), or this
      // is an invalid position. The query is considered valid though.
      *textString = nullptr;
      *textLength = 0;
    }
    അല്ലെങ്കിൽ
    {
      *textString = mText;
      *textLength = textPosition;
    }
    തിരികെ S_OK;
  }

  IFACEMETHODIMP_ (DWRITE_READING_DIRECTION)
  GetParagraphReadingDirection () { തിരികെ mReadingDirection; }

  IFACEMETHODIMP GetLocaleName (uint32_t textPosition, uint32_t* textLength,
				wchar_t സ്ഥിരം** localeName)
  { തിരികെ S_OK; }

  IFACEMETHODIMP
  Get_സംഖ്യ_Substitution (uint32_t textPosition,
			 OUT uint32_t* textLength,
			 OUT IDWrite_സംഖ്യ_Substitution** സംഖ്യ_Substitution)
  {
    // We do not support number substitution.
    *സംഖ്യ_Substitution = nullptr;
    *textLength = mTextLength - textPosition;

    തിരികെ S_OK;
  }

  // IDWriteTextAnalysisSink implementation

  IFACEMETHODIMP
  SetScriptAnalysis (uint32_t textPosition, uint32_t textLength,
		     DWRITE_SCRIPT_ANALYSIS സ്ഥിരം* scriptAnalysis)
  {
    SetCurrentRun (textPosition);
    എങ്കിൽ (unlikely (!SplitCurrentRun (textPosition)))
      തിരികെ E_OUTOFMEMORY;
    ആകുമ്പോൾ (textLength > 0)
    {
      Run *run = FetchNextRun (&textLength);
      എങ്കിൽ (unlikely (!run))
        തിരികെ E_OUTOFMEMORY;
      run->mScript = *scriptAnalysis;
    }

    തിരികെ S_OK;
  }

  IFACEMETHODIMP
  Set_വരി_Breakpoints (uint32_t textPosition,
		      uint32_t textLength,
		      സ്ഥിരം DWRITE_വരി_BREAKPOINT* വരി_Breakpoints)
  { തിരികെ S_OK; }

  IFACEMETHODIMP SetBidiLevel (uint32_t textPosition, uint32_t textLength,
			       uint8_t explicitLevel, uint8_t resolvedLevel)
  { തിരികെ S_OK; }

  IFACEMETHODIMP
  Set_സംഖ്യ_Substitution (uint32_t textPosition, uint32_t textLength,
			 IDWrite_സംഖ്യ_Substitution* സംഖ്യ_Substitution)
  { തിരികെ S_OK; }

സംരക്ഷിത:
  Run *FetchNextRun (IN OUT uint32_t* textLength)
  {
    // Used by the sink setters, this returns a reference to the next run.
    // Position and length are adjusted to now point after the current run
    // being returned.

    Run *origRun = mCurrentRun;
    // Split the tail if needed (the length remaining is less than the
    // current run's size).
    എങ്കിൽ (*textLength < mCurrentRun->mTextLength)
    {
      എങ്കിൽ (unlikely (!SplitCurrentRun (mCurrentRun->mTextStart + *textLength)))
        തിരികെ nullptr;
    }
    അല്ലെങ്കിൽ
      // Just advance the current run.
      mCurrentRun = mCurrentRun->nextRun;
    *textLength -= origRun->mTextLength;

    // Return a reference to the run that was just current.
    തിരികെ origRun;
  }

  ശൂന്യം SetCurrentRun (uint32_t textPosition)
  {
    // Move the current run to the given position.
    // Since the analyzers generally return results in a forward manner,
    // this will usually just return early. If not, find the
    // corresponding run for the text position.

    എങ്കിൽ (mCurrentRun && mCurrentRun->ContainsTextPosition (textPosition))
      തിരികെ;

    വേണ്ടി (Run *run = &mRunHead; run; run = run->nextRun)
      എങ്കിൽ (run->ContainsTextPosition (textPosition))
      {
	mCurrentRun = run;
	തിരികെ;
      }
    പ്രതിജ്ഞാ (0); // We should always be able to find the text position in one of our runs
  }

  തർക്കം SplitCurrentRun (uint32_t splitPosition)
  {
    എങ്കിൽ (!mCurrentRun)
    {
      പ്രതിജ്ഞാ (0); // SplitCurrentRun called without current run
      // Shouldn't be calling this when no current run is set!
      തിരികെ അസത്യം;
    }
    // Split the current run.
    എങ്കിൽ (splitPosition <= mCurrentRun->mTextStart)
    {
      // No need to split, already the start of a run
      // or before it. Usually the first.
      തിരികെ സത്യം;
    }
    Run *newRun = (Run *) hb_malloc (വലുപ്പം (Run));
    എങ്കിൽ (unlikely (!newRun))
      തിരികെ അസത്യം;

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
    തിരികെ സത്യം;
  }

സംരക്ഷിത:
  //നിവേശം
  // (weak references are fine here, since this class is a transient
  //  stack-based helper that doesn't need to copy data)
  uint32_t mTextLength;
  സ്ഥിരം wchar_t* mText;
  സ്ഥിരം wchar_t* mLocaleName;
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
		       സ്ഥിരം hb_feature_t *features,
		       unsigned പൂർണ്ണസംഖ്യ        അക്കം_features)
{
  IDWriteFontFace *fontFace = (IDWriteFontFace *) (സ്ഥിരം ശൂന്യം *) font->data.directwrite;
  auto *global = get_directwrite_global ();
  എങ്കിൽ (unlikely (!global))
    തിരികെ അസത്യം;
  IDWriteFactory *dwriteFactory = global->dwriteFactory;

  IDWriteTextAnalyzer* analyzer;
  dwriteFactory->CreateTextAnalyzer (&analyzer);

  unsigned പൂർണ്ണസംഖ്യ scratch_size;
  hb_buffer_t::scratch_buffer_t *scratch = buffer->get_scratch_buffer (&scratch_size);
#define ALLOCATE_ARRAY(Type, name, len) \
  Type *name = (Type *) scratch; \
  ചെയ്യുക { \
    unsigned പൂർണ്ണസംഖ്യ _consumed = DIV_CEIL ((len) * വലുപ്പം (Type), വലുപ്പം (*scratch)); \
    പ്രതിജ്ഞാ (_consumed <= scratch_size); \
    scratch += _consumed; \
    scratch_size -= _consumed; \
  } ആകുമ്പോൾ (0)

#define utf16_index() var1.u32

  ALLOCATE_ARRAY (wchar_t, textString, buffer->len * 2);

  unsigned പൂർണ്ണസംഖ്യ chars_len = 0;
  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < buffer->len; i++)
  {
    hb_codepoint_t c = buffer->info[i].codepoint;
    buffer->info[i].utf16_index () = chars_len;
    എങ്കിൽ (likely (c <= 0xFFFFu))
      textString[chars_len++] = c;
    അല്ലെങ്കിൽ എങ്കിൽ (unlikely (c > 0x10FFFFu))
      textString[chars_len++] = 0xFFFDu;
    അല്ലെങ്കിൽ
    {
      textString[chars_len++] = 0xD800u + ((c - 0x10000u) >> 10);
      textString[chars_len++] = 0xDC00u + ((c - 0x10000u) & ((1u << 10) - 1));
    }
  }

  ALLOCATE_ARRAY (WORD, log_clusters, chars_len);
#undef ALLOCATE_ARRAY
  /* Need log_clusters to assign features. */
  chars_len = 0;
  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < buffer->len; i++)
  {
    hb_codepoint_t c = buffer->info[i].codepoint;
    unsigned പൂർണ്ണസംഖ്യ cluster = buffer->info[i].cluster;
    log_clusters[chars_len++] = cluster;
    എങ്കിൽ (hb_in_range (c, 0x10000u, 0x10FFFFu))
      log_clusters[chars_len++] = cluster; /* Surrogates. */
  }

  DWRITE_READING_DIRECTION readingDirection;
  readingDirection = buffer->props.direction ?
		     DWRITE_READING_DIRECTION_വലത്_TO_ഇടത് :
		     DWRITE_READING_DIRECTION_ഇടത്_TO_വലത്;

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
    DEBUG_സന്ദേശം (DIRECTWRITE, nullptr, __VA_ARGS__); \
    തിരികെ അസത്യം; \
  } HB_STMT_END

  എങ്കിൽ (FAILED (hr))
    FAIL ("Analyzer failed to generate results.");

  uint32_t max_Glyph_എണ്ണം = 3 * textLength / 2 + 16;
  uint32_t glyph_എണ്ണം_2;
  തർക്കം is_വലത്_To_ഇടത് = HB_DIRECTION_IS_BACKWARD (buffer->props.direction);

  സ്ഥിരം wchar_t localeName[20] = {0};
  എങ്കിൽ (buffer->props.language)
    mbstowcs ((wchar_t*) localeName,
	      hb_language_to_string (buffer->props.language), 20);

  /*
   * Set up features.
   */
  static_assert ((വലുപ്പം (DWRITE_TYPOGRAPHIC_FEATURES) == വലുപ്പം (hb_ms_features_t)), "");
  static_assert ((വലുപ്പം (DWRITE_FONT_FEATURE) == വലുപ്പം (hb_ms_feature_t)), "");
  hb_vector_t<hb_ms_features_t *> range_features;
  hb_vector_t<uint32_t> range_char_counts;

  // https://github.com/harfbuzz/harfbuzz/pull/5114
  // The data allocated by these two vectors are used by the above two, so they
  // should remain alive as long as the above two are.
  hb_vector_t<hb_ms_feature_t> feature_records;
  hb_vector_t<hb_ms_range_record_t> range_records;
  എങ്കിൽ (അക്കം_features)
  {
    എങ്കിൽ (hb_ms_setup_features (features, അക്കം_features, feature_records, range_records))
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
  എങ്കിൽ (unlikely (!clusterMap.resize_exact (textLength) ||
                !textProperties.resize_exact (textLength)))
    FAIL ("Failed to allocate shaping text data.");

  hb_vector_t<uint16_t> glyphIndices;
  hb_vector_t<DWRITE_SHAPING_GLYPH_PROPERTIES> glyphProperties;

retry_getglyphs:
  എങ്കിൽ (unlikely (!glyphIndices.resize_exact (max_Glyph_എണ്ണം) ||
                !glyphProperties.resize_exact (max_Glyph_എണ്ണം)))
    FAIL ("Failed to allocate shaping glyph data.");

  hr = analyzer->GetGlyphs (textString,
			    chars_len,
			    fontFace,
			    അസത്യം,
			    is_വലത്_To_ഇടത്,
			    &runHead->mScript,
			    localeName,
			    nullptr,
			    (സ്ഥിരം DWRITE_TYPOGRAPHIC_FEATURES**) range_features.arrayZ,
			    range_char_counts.arrayZ,
			    range_features.length,
			    max_Glyph_എണ്ണം,
			    clusterMap.arrayZ,
			    textProperties.arrayZ,
			    glyphIndices.arrayZ,
			    glyphProperties.arrayZ,
			    &glyph_എണ്ണം_2);

  എങ്കിൽ (unlikely (hr == HRESULT_FROM_WIN32 (ERROR_INSUFFICIENT_BUFFER)))
  {
    max_Glyph_എണ്ണം *= 2;

    പോകുക retry_getglyphs;
  }
  എങ്കിൽ (FAILED (hr))
    FAIL ("Analyzer failed to get glyphs.");

  hb_vector_t<ദശാംശം> glyphAdvances;
  hb_vector_t<DWRITE_GLYPH_OFFSET> glyphOffsets;
  എങ്കിൽ (unlikely (!glyphAdvances.resize_exact (max_Glyph_എണ്ണം) ||
                !glyphOffsets.resize_exact (max_Glyph_എണ്ണം)))
    FAIL ("Failed to allocate glyph positioning data.");


  unsigned fontEmSize = font->face->get_upem ();

  ദശാംശം x_mult = font->x_multf;
  ദശാംശം y_mult = font->y_multf;

  hr = analyzer->GetGlyphPlacements (textString,
				     clusterMap.arrayZ,
				     textProperties.arrayZ,
				     chars_len,
				     glyphIndices.arrayZ,
				     glyphProperties.arrayZ,
				     glyph_എണ്ണം_2,
				     fontFace,
				     fontEmSize,
				     അസത്യം,
				     is_വലത്_To_ഇടത്,
				     &runHead->mScript,
				     localeName,
				     (സ്ഥിരം DWRITE_TYPOGRAPHIC_FEATURES**) range_features.arrayZ,
				     range_char_counts.arrayZ,
				     range_features.length,
				     glyphAdvances.arrayZ,
				     glyphOffsets.arrayZ);

  എങ്കിൽ (FAILED (hr))
    FAIL ("Analyzer failed to get glyph placements.");

  /* Ok, we've got everything we need, now compose output buffer,
   * very, *very*, carefully! */

  /* Calculate visual-clusters.  That's what we ship. */
  hb_vector_t<uint32_t> vis_clusters;
  എങ്കിൽ (unlikely (!vis_clusters.resize_exact (glyph_എണ്ണം_2)))
    FAIL ("Failed to allocate visual clusters.");

  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < glyph_എണ്ണം_2; i++)
    vis_clusters[i] = (uint32_t) -1;
  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < buffer->len; i++)
  {
    unsigned പൂർണ്ണസംഖ്യ c = clusterMap[buffer->info[i].utf16_index ()];
    എങ്കിൽ (likely (c < glyph_എണ്ണം_2))
      vis_clusters[c] = hb_min (vis_clusters[c], buffer->info[i].cluster);
  }
  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 1; i < glyph_എണ്ണം_2; i++)
    എങ്കിൽ (vis_clusters[i] == (uint32_t) -1)
      vis_clusters[i] = vis_clusters[i - 1];
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = (പൂർണ്ണസംഖ്യ) glyph_എണ്ണം_2 - 2; i >= 0; i--)
    എങ്കിൽ (vis_clusters[i] == (uint32_t) -1)
      vis_clusters[i] = vis_clusters[i + 1];

#undef utf16_index

  എങ്കിൽ (unlikely (!buffer->ensure (glyph_എണ്ണം_2)))
    FAIL ("Buffer in error");

#undef FAIL

  /* Set glyph infos */
  buffer->len = 0;
  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < glyph_എണ്ണം_2; i++)
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
  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < glyph_എണ്ണം_2; i++)
  {
    hb_glyph_info_t *info = &buffer->info[i];
    hb_glyph_position_t *pos = &buffer->pos[i];

    /* TODO vertical */
    pos->x_advance = hb_clamp_to<hb_position_t> (round (x_mult * (int32_t) info->mask));
    ഇരട്ട x_offset = is_വലത്_To_ഇടത് ? -(ഇരട്ട) info->var1.i32 : info->var1.i32;
    pos->x_offset = hb_clamp_to<hb_position_t> (round ((ഇരട്ട) x_mult * x_offset));
    pos->y_offset = hb_clamp_to<hb_position_t> (round (y_mult * info->var2.i32));
  }

  എങ്കിൽ (is_വലത്_To_ഇടത്) hb_buffer_reverse (buffer);

  buffer->clear_glyph_flags ();
  buffer->unsafe_to_break ();

  /* Wow, done! */
  തിരികെ സത്യം;
}


#endif
