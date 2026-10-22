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

#अन्तर्भाव "hb.hh"

#ifdef HAVE_DIRECTWRITE

#अन्तर्भाव "hb-shaper-impl.hh"

#अन्तर्भाव "hb-directwrite.hh"

#अन्तर्भाव "hb-ms-feature-ranges.hh"


/*
* shaper face data
*/

hb_directwrite_face_data_t *
_hb_directwrite_shaper_face_data_create (hb_face_t *face)
{
  hb_blob_t *blob = hb_face_reference_blob (face);

  hb_directwrite_face_data_t *data = (hb_directwrite_face_data_t *) dw_face_create (blob, face->index);

  hb_blob_destroy (blob);

  पटिनिवत्त data;
}

शून्य
_hb_directwrite_shaper_face_data_destroy (hb_directwrite_face_data_t *data)
{
  ((IDWriteFontFace *) data)->Release ();
}


/*
 * shaper font data
 */

संरचना hb_directwrite_font_data_t {};

hb_directwrite_font_data_t *
_hb_directwrite_shaper_font_data_create (hb_font_t *font)
{
  IDWriteFontFace *fontFace = (IDWriteFontFace *) (ध्रुव शून्य *) font->face->data.directwrite;

  /*
   * Set up variations.
   */
  IDWriteFontFace5 *fontFaceVariations = nullptr;
  {
    IDWriteFontFace5 *fontFace5;
    सचे (SUCCEEDED (fontFace->QueryInterface (__uuidof (IDWriteFontFace5), (शून्य **) &fontFace5)))
    {
      IDWriteFontResource *fontResource;
      सचे (SUCCEEDED (fontFace5->GetFontResource (&fontResource)))
      {
	hb_vector_t<DWRITE_FONT_AXIS_मानम्> axis_मानानि;
	सचे (likely (axis_मानानि.resize_exact (font->अङ्कः_coords)))
	{
	  अत्थाय (unsigned पूर्णाङ्क i = 0; i < font->अङ्कः_coords; i++)
	  {
	    hb_ot_var_axis_info_t info;
	    unsigned पूर्णाङ्क c = 1;
	    hb_ot_var_get_axis_infos (font->face, i, &c, &info);
	    axis_मानानि[i].axisTag = (DWRITE_FONT_AXIS_TAG) hb_uint32_swap (info.tag);
	    axis_मानानि[i].value = i < font->अङ्कः_coords ?
				   hb_clamp (font->design_coords[i], info.min_मानम्, info.max_मानम्) :
				   info.default_मानम्;
	  }

	  fontResource->CreateFontFace (DWRITE_FONT_SIMULATIONS::DWRITE_FONT_SIMULATIONS_NONE,
					axis_मानानि.arrayZ, axis_मानानि.length, &fontFaceVariations);
	}
	fontResource->Release ();
      }
      fontFace5->Release ();
    }
  }

  पटिनिवत्त (hb_directwrite_font_data_t *) fontFaceVariations;
}

शून्य
_hb_directwrite_shaper_font_data_destroy (hb_directwrite_font_data_t *data)
{
  ((IDWriteFontFace *) (ध्रुव शून्य *) data)->Release ();
}


// Most of TextAnalysis is originally written by Bas Schouten for Mozilla project
// but now is relicensed to MIT for HarfBuzz use
वग्ग TextAnalysis : सार्वजनिक IDWriteTextAnalysisSource, सार्वजनिक IDWriteTextAnalysisSink
{
गुप्त:
  hb_reference_गणनम्_t m_Ref_गणनम्;
सार्वजनिक:
  IFACEMETHOD (QueryInterface) (IID ध्रुव& iid, OUT शून्य** ppObject)
  { पटिनिवत्त S_OK; }
  IFACEMETHOD_ (ULONG, AddRef) ()
  {
    पटिनिवत्त m_Ref_गणनम्.inc () + 1;
  }
  IFACEMETHOD_ (ULONG, Release) ()
  {
    signed ref_गणनम् = m_Ref_गणनम्.dec () - 1;
    प्रतिज्ञा (ref_गणनम् >= 0);
    सचे (ref_गणनम्)
      पटिनिवत्त ref_गणनम्;
    पटिनिवत्त 0;
  }

  // A single contiguous run of characters containing the same analysis
  // results.
  संरचना Run
  {
    uint32_t mTextStart;   // starting text position of this run
    uint32_t mTextLength;  // number of contiguous code units covered
    uint32_t mGlyphStart;  // starting glyph in the glyphs array
    uint32_t m_Glyph_गणनम्;  // number of glyphs associated with this run
    // text
    DWRITE_SCRIPT_ANALYSIS mScript;
    uint8_t mBidiLevel;
    तार्किक mIsSideways;

    तार्किक ContainsTextPosition (uint32_t aTextPosition) ध्रुव
    {
      पटिनिवत्त aTextPosition >= mTextStart &&
	     aTextPosition <  mTextStart + mTextLength;
    }

    Run *nextRun;
  };

सार्वजनिक:
  TextAnalysis (ध्रुव wchar_t* text, uint32_t textLength,
		ध्रुव wchar_t* localeName, DWRITE_READING_DIRECTION readingDirection)
	       : mTextLength (textLength), mText (text), mLocaleName (localeName),
		 mReadingDirection (readingDirection), mCurrentRun (nullptr)
  {
    m_Ref_गणनम्.init ();
  }
  ~TextAnalysis ()
  {
    // free runs, except mRunHead which is part of the TextAnalysis object
    अत्थाय (Run *run = mRunHead.nextRun; run;)
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
      (mReadingDirection == DWRITE_READING_DIRECTION_दक्षिणः_TO_वामः);
    mRunHead.nextRun = nullptr;
    mCurrentRun = &mRunHead;

    // Call each of the analyzers in sequence, recording their results.
    सचे (SUCCEEDED (hr = textAnalyzer->AnalyzeScript (एतत्, 0, mTextLength, एतत्)))
      *runHead = &mRunHead;

    पटिनिवत्त hr;
  }

  // IDWriteTextAnalysisSource implementation

  IFACEMETHODIMP
  GetTextAtPosition (uint32_t textPosition,
		     OUT wchar_t ध्रुव** textString,
		     OUT uint32_t* textLength)
  {
    सचे (textPosition >= mTextLength)
    {
      // No text at this position, valid query though.
      *textString = nullptr;
      *textLength = 0;
    }
    अञ्ञथा
    {
      *textString = mText + textPosition;
      *textLength = mTextLength - textPosition;
    }
    पटिनिवत्त S_OK;
  }

  IFACEMETHODIMP
  GetTextBeforePosition (uint32_t textPosition,
			 OUT wchar_t ध्रुव** textString,
			 OUT uint32_t* textLength)
  {
    सचे (textPosition == 0 || textPosition > mTextLength)
    {
      // Either there is no text before here (== 0), or this
      // is an invalid position. The query is considered valid though.
      *textString = nullptr;
      *textLength = 0;
    }
    अञ्ञथा
    {
      *textString = mText;
      *textLength = textPosition;
    }
    पटिनिवत्त S_OK;
  }

  IFACEMETHODIMP_ (DWRITE_READING_DIRECTION)
  GetParagraphReadingDirection () { पटिनिवत्त mReadingDirection; }

  IFACEMETHODIMP GetLocaleName (uint32_t textPosition, uint32_t* textLength,
				wchar_t ध्रुव** localeName)
  { पटिनिवत्त S_OK; }

  IFACEMETHODIMP
  Get_सङ्ख्या_Substitution (uint32_t textPosition,
			 OUT uint32_t* textLength,
			 OUT IDWrite_सङ्ख्या_Substitution** सङ्ख्या_Substitution)
  {
    // We do not support number substitution.
    *सङ्ख्या_Substitution = nullptr;
    *textLength = mTextLength - textPosition;

    पटिनिवत्त S_OK;
  }

  // IDWriteTextAnalysisSink implementation

  IFACEMETHODIMP
  SetScriptAnalysis (uint32_t textPosition, uint32_t textLength,
		     DWRITE_SCRIPT_ANALYSIS ध्रुव* scriptAnalysis)
  {
    SetCurrentRun (textPosition);
    सचे (unlikely (!SplitCurrentRun (textPosition)))
      पटिनिवत्त E_OUTOFMEMORY;
    याव (textLength > 0)
    {
      Run *run = FetchNextRun (&textLength);
      सचे (unlikely (!run))
        पटिनिवत्त E_OUTOFMEMORY;
      run->mScript = *scriptAnalysis;
    }

    पटिनिवत्त S_OK;
  }

  IFACEMETHODIMP
  Set_पङ्क्तिः_Breakpoints (uint32_t textPosition,
		      uint32_t textLength,
		      ध्रुव DWRITE_पङ्क्तिः_BREAKPOINT* पङ्क्तिः_Breakpoints)
  { पटिनिवत्त S_OK; }

  IFACEMETHODIMP SetBidiLevel (uint32_t textPosition, uint32_t textLength,
			       uint8_t explicitLevel, uint8_t resolvedLevel)
  { पटिनिवत्त S_OK; }

  IFACEMETHODIMP
  Set_सङ्ख्या_Substitution (uint32_t textPosition, uint32_t textLength,
			 IDWrite_सङ्ख्या_Substitution* सङ्ख्या_Substitution)
  { पटिनिवत्त S_OK; }

रक्षित:
  Run *FetchNextRun (IN OUT uint32_t* textLength)
  {
    // Used by the sink setters, this returns a reference to the next run.
    // Position and length are adjusted to now point after the current run
    // being returned.

    Run *origRun = mCurrentRun;
    // Split the tail if needed (the length remaining is less than the
    // current run's size).
    सचे (*textLength < mCurrentRun->mTextLength)
    {
      सचे (unlikely (!SplitCurrentRun (mCurrentRun->mTextStart + *textLength)))
        पटिनिवत्त nullptr;
    }
    अञ्ञथा
      // Just advance the current run.
      mCurrentRun = mCurrentRun->nextRun;
    *textLength -= origRun->mTextLength;

    // Return a reference to the run that was just current.
    पटिनिवत्त origRun;
  }

  शून्य SetCurrentRun (uint32_t textPosition)
  {
    // Move the current run to the given position.
    // Since the analyzers generally return results in a forward manner,
    // this will usually just return early. If not, find the
    // corresponding run for the text position.

    सचे (mCurrentRun && mCurrentRun->ContainsTextPosition (textPosition))
      पटिनिवत्त;

    अत्थाय (Run *run = &mRunHead; run; run = run->nextRun)
      सचे (run->ContainsTextPosition (textPosition))
      {
	mCurrentRun = run;
	पटिनिवत्त;
      }
    प्रतिज्ञा (0); // We should always be able to find the text position in one of our runs
  }

  तार्किक SplitCurrentRun (uint32_t splitPosition)
  {
    सचे (!mCurrentRun)
    {
      प्रतिज्ञा (0); // SplitCurrentRun called without current run
      // Shouldn't be calling this when no current run is set!
      पटिनिवत्त मिच्छा;
    }
    // Split the current run.
    सचे (splitPosition <= mCurrentRun->mTextStart)
    {
      // No need to split, already the start of a run
      // or before it. Usually the first.
      पटिनिवत्त सच्च;
    }
    Run *newRun = (Run *) hb_malloc (परिमाण (Run));
    सचे (unlikely (!newRun))
      पटिनिवत्त मिच्छा;

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
    पटिनिवत्त सच्च;
  }

रक्षित:
  //निवेशः
  // (weak references are fine here, since this class is a transient
  //  stack-based helper that doesn't need to copy data)
  uint32_t mTextLength;
  ध्रुव wchar_t* mText;
  ध्रुव wchar_t* mLocaleName;
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
		       ध्रुव hb_feature_t *features,
		       unsigned पूर्णाङ्क        अङ्कः_features)
{
  IDWriteFontFace *fontFace = (IDWriteFontFace *) (ध्रुव शून्य *) font->data.directwrite;
  auto *global = get_directwrite_global ();
  सचे (unlikely (!global))
    पटिनिवत्त मिच्छा;
  IDWriteFactory *dwriteFactory = global->dwriteFactory;

  IDWriteTextAnalyzer* analyzer;
  dwriteFactory->CreateTextAnalyzer (&analyzer);

  unsigned पूर्णाङ्क scratch_size;
  hb_buffer_t::scratch_buffer_t *scratch = buffer->get_scratch_buffer (&scratch_size);
#define ALLOCATE_ARRAY(Type, name, len) \
  Type *name = (Type *) scratch; \
  करोहि { \
    unsigned पूर्णाङ्क _consumed = DIV_CEIL ((len) * परिमाण (Type), परिमाण (*scratch)); \
    प्रतिज्ञा (_consumed <= scratch_size); \
    scratch += _consumed; \
    scratch_size -= _consumed; \
  } याव (0)

#define utf16_index() var1.u32

  ALLOCATE_ARRAY (wchar_t, textString, buffer->len * 2);

  unsigned पूर्णाङ्क chars_len = 0;
  अत्थाय (unsigned पूर्णाङ्क i = 0; i < buffer->len; i++)
  {
    hb_codepoint_t c = buffer->info[i].codepoint;
    buffer->info[i].utf16_index () = chars_len;
    सचे (likely (c <= 0xFFFFu))
      textString[chars_len++] = c;
    अञ्ञथा सचे (unlikely (c > 0x10FFFFu))
      textString[chars_len++] = 0xFFFDu;
    अञ्ञथा
    {
      textString[chars_len++] = 0xD800u + ((c - 0x10000u) >> 10);
      textString[chars_len++] = 0xDC00u + ((c - 0x10000u) & ((1u << 10) - 1));
    }
  }

  ALLOCATE_ARRAY (WORD, log_clusters, chars_len);
#undef ALLOCATE_ARRAY
  /* Need log_clusters to assign features. */
  chars_len = 0;
  अत्थाय (unsigned पूर्णाङ्क i = 0; i < buffer->len; i++)
  {
    hb_codepoint_t c = buffer->info[i].codepoint;
    unsigned पूर्णाङ्क cluster = buffer->info[i].cluster;
    log_clusters[chars_len++] = cluster;
    सचे (hb_in_range (c, 0x10000u, 0x10FFFFu))
      log_clusters[chars_len++] = cluster; /* Surrogates. */
  }

  DWRITE_READING_DIRECTION readingDirection;
  readingDirection = buffer->props.direction ?
		     DWRITE_READING_DIRECTION_दक्षिणः_TO_वामः :
		     DWRITE_READING_DIRECTION_वामः_TO_दक्षिणः;

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
    DEBUG_सन्देशः (DIRECTWRITE, nullptr, __VA_ARGS__); \
    पटिनिवत्त मिच्छा; \
  } HB_STMT_END

  सचे (FAILED (hr))
    FAIL ("Analyzer failed to generate results.");

  uint32_t max_Glyph_गणनम् = 3 * textLength / 2 + 16;
  uint32_t glyph_गणनम्_2;
  तार्किक is_दक्षिणः_To_वामः = HB_DIRECTION_IS_BACKWARD (buffer->props.direction);

  ध्रुव wchar_t localeName[20] = {0};
  सचे (buffer->props.language)
    mbstowcs ((wchar_t*) localeName,
	      hb_language_to_string (buffer->props.language), 20);

  /*
   * Set up features.
   */
  static_assert ((परिमाण (DWRITE_TYPOGRAPHIC_FEATURES) == परिमाण (hb_ms_features_t)), "");
  static_assert ((परिमाण (DWRITE_FONT_FEATURE) == परिमाण (hb_ms_feature_t)), "");
  hb_vector_t<hb_ms_features_t *> range_features;
  hb_vector_t<uint32_t> range_char_counts;

  // https://github.com/harfbuzz/harfbuzz/pull/5114
  // The data allocated by these two vectors are used by the above two, so they
  // should remain alive as long as the above two are.
  hb_vector_t<hb_ms_feature_t> feature_records;
  hb_vector_t<hb_ms_range_record_t> range_records;
  सचे (अङ्कः_features)
  {
    सचे (hb_ms_setup_features (features, अङ्कः_features, feature_records, range_records))
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
  सचे (unlikely (!clusterMap.resize_exact (textLength) ||
                !textProperties.resize_exact (textLength)))
    FAIL ("Failed to allocate shaping text data.");

  hb_vector_t<uint16_t> glyphIndices;
  hb_vector_t<DWRITE_SHAPING_GLYPH_PROPERTIES> glyphProperties;

retry_getglyphs:
  सचे (unlikely (!glyphIndices.resize_exact (max_Glyph_गणनम्) ||
                !glyphProperties.resize_exact (max_Glyph_गणनम्)))
    FAIL ("Failed to allocate shaping glyph data.");

  hr = analyzer->GetGlyphs (textString,
			    chars_len,
			    fontFace,
			    मिच्छा,
			    is_दक्षिणः_To_वामः,
			    &runHead->mScript,
			    localeName,
			    nullptr,
			    (ध्रुव DWRITE_TYPOGRAPHIC_FEATURES**) range_features.arrayZ,
			    range_char_counts.arrayZ,
			    range_features.length,
			    max_Glyph_गणनम्,
			    clusterMap.arrayZ,
			    textProperties.arrayZ,
			    glyphIndices.arrayZ,
			    glyphProperties.arrayZ,
			    &glyph_गणनम्_2);

  सचे (unlikely (hr == HRESULT_FROM_WIN32 (ERROR_INSUFFICIENT_BUFFER)))
  {
    max_Glyph_गणनम् *= 2;

    गच्छ retry_getglyphs;
  }
  सचे (FAILED (hr))
    FAIL ("Analyzer failed to get glyphs.");

  hb_vector_t<भिन्न> glyphAdvances;
  hb_vector_t<DWRITE_GLYPH_OFFSET> glyphOffsets;
  सचे (unlikely (!glyphAdvances.resize_exact (max_Glyph_गणनम्) ||
                !glyphOffsets.resize_exact (max_Glyph_गणनम्)))
    FAIL ("Failed to allocate glyph positioning data.");


  unsigned fontEmSize = font->face->get_upem ();

  भिन्न x_mult = font->x_multf;
  भिन्न y_mult = font->y_multf;

  hr = analyzer->GetGlyphPlacements (textString,
				     clusterMap.arrayZ,
				     textProperties.arrayZ,
				     chars_len,
				     glyphIndices.arrayZ,
				     glyphProperties.arrayZ,
				     glyph_गणनम्_2,
				     fontFace,
				     fontEmSize,
				     मिच्छा,
				     is_दक्षिणः_To_वामः,
				     &runHead->mScript,
				     localeName,
				     (ध्रुव DWRITE_TYPOGRAPHIC_FEATURES**) range_features.arrayZ,
				     range_char_counts.arrayZ,
				     range_features.length,
				     glyphAdvances.arrayZ,
				     glyphOffsets.arrayZ);

  सचे (FAILED (hr))
    FAIL ("Analyzer failed to get glyph placements.");

  /* Ok, we've got everything we need, now compose output buffer,
   * very, *very*, carefully! */

  /* Calculate visual-clusters.  That's what we ship. */
  hb_vector_t<uint32_t> vis_clusters;
  सचे (unlikely (!vis_clusters.resize_exact (glyph_गणनम्_2)))
    FAIL ("Failed to allocate visual clusters.");

  अत्थाय (unsigned पूर्णाङ्क i = 0; i < glyph_गणनम्_2; i++)
    vis_clusters[i] = (uint32_t) -1;
  अत्थाय (unsigned पूर्णाङ्क i = 0; i < buffer->len; i++)
  {
    unsigned पूर्णाङ्क c = clusterMap[buffer->info[i].utf16_index ()];
    सचे (likely (c < glyph_गणनम्_2))
      vis_clusters[c] = hb_min (vis_clusters[c], buffer->info[i].cluster);
  }
  अत्थाय (unsigned पूर्णाङ्क i = 1; i < glyph_गणनम्_2; i++)
    सचे (vis_clusters[i] == (uint32_t) -1)
      vis_clusters[i] = vis_clusters[i - 1];
  अत्थाय (पूर्णाङ्क i = (पूर्णाङ्क) glyph_गणनम्_2 - 2; i >= 0; i--)
    सचे (vis_clusters[i] == (uint32_t) -1)
      vis_clusters[i] = vis_clusters[i + 1];

#undef utf16_index

  सचे (unlikely (!buffer->ensure (glyph_गणनम्_2)))
    FAIL ("Buffer in error");

#undef FAIL

  /* Set glyph infos */
  buffer->len = 0;
  अत्थाय (unsigned पूर्णाङ्क i = 0; i < glyph_गणनम्_2; i++)
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
  अत्थाय (unsigned पूर्णाङ्क i = 0; i < glyph_गणनम्_2; i++)
  {
    hb_glyph_info_t *info = &buffer->info[i];
    hb_glyph_position_t *pos = &buffer->pos[i];

    /* TODO vertical */
    pos->x_advance = hb_clamp_to<hb_position_t> (round (x_mult * (int32_t) info->mask));
    द्विगुण x_offset = is_दक्षिणः_To_वामः ? -(द्विगुण) info->var1.i32 : info->var1.i32;
    pos->x_offset = hb_clamp_to<hb_position_t> (round ((द्विगुण) x_mult * x_offset));
    pos->y_offset = hb_clamp_to<hb_position_t> (round (y_mult * info->var2.i32));
  }

  सचे (is_दक्षिणः_To_वामः) hb_buffer_reverse (buffer);

  buffer->clear_glyph_flags ();
  buffer->unsafe_to_break ();

  /* Wow, done! */
  पटिनिवत्त सच्च;
}


#endif
