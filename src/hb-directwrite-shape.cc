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

#அந்தர்பாவ "hb.hh"

#ifdef HAVE_DIRECTWRITE

#அந்தர்பாவ "hb-shaper-impl.hh"

#அந்தர்பாவ "hb-directwrite.hh"

#அந்தர்பாவ "hb-ms-feature-ranges.hh"


/*
* shaper face data
*/

hb_directwrite_face_data_t *
_hb_directwrite_shaper_face_data_create (hb_face_t *face)
{
  hb_blob_t *blob = hb_face_reference_blob (face);

  hb_directwrite_face_data_t *data = (hb_directwrite_face_data_t *) dw_face_create (blob, face->index);

  hb_blob_destroy (blob);

  திரும்பு data;
}

வெறுமை
_hb_directwrite_shaper_face_data_destroy (hb_directwrite_face_data_t *data)
{
  ((IDWriteFontFace *) data)->Release ();
}


/*
 * shaper font data
 */

கட்டமைப்பு hb_directwrite_font_data_t {};

hb_directwrite_font_data_t *
_hb_directwrite_shaper_font_data_create (hb_font_t *font)
{
  IDWriteFontFace *fontFace = (IDWriteFontFace *) (மாறிலி வெறுமை *) font->face->data.directwrite;

  /*
   * Set up variations.
   */
  IDWriteFontFace5 *fontFaceVariations = nullptr;
  {
    IDWriteFontFace5 *fontFace5;
    எனில் (SUCCEEDED (fontFace->QueryInterface (__uuidof (IDWriteFontFace5), (வெறுமை **) &fontFace5)))
    {
      IDWriteFontResource *fontResource;
      எனில் (SUCCEEDED (fontFace5->GetFontResource (&fontResource)))
      {
	hb_vector_t<DWRITE_FONT_AXIS_மதிப்பு> axis_மதிப்புகள்;
	எனில் (likely (axis_மதிப்புகள்.resize_exact (font->இலக்கம்_coords)))
	{
	  க்கு (unsigned முழு i = 0; i < font->இலக்கம்_coords; i++)
	  {
	    hb_ot_var_axis_info_t info;
	    unsigned முழு c = 1;
	    hb_ot_var_get_axis_infos (font->face, i, &c, &info);
	    axis_மதிப்புகள்[i].axisTag = (DWRITE_FONT_AXIS_TAG) hb_uint32_swap (info.tag);
	    axis_மதிப்புகள்[i].value = i < font->இலக்கம்_coords ?
				   hb_clamp (font->design_coords[i], info.min_மதிப்பு, info.max_மதிப்பு) :
				   info.default_மதிப்பு;
	  }

	  fontResource->CreateFontFace (DWRITE_FONT_SIMULATIONS::DWRITE_FONT_SIMULATIONS_NONE,
					axis_மதிப்புகள்.arrayZ, axis_மதிப்புகள்.length, &fontFaceVariations);
	}
	fontResource->Release ();
      }
      fontFace5->Release ();
    }
  }

  திரும்பு (hb_directwrite_font_data_t *) fontFaceVariations;
}

வெறுமை
_hb_directwrite_shaper_font_data_destroy (hb_directwrite_font_data_t *data)
{
  ((IDWriteFontFace *) (மாறிலி வெறுமை *) data)->Release ();
}


// Most of TextAnalysis is originally written by Bas Schouten for Mozilla project
// but now is relicensed to MIT for HarfBuzz use
வகுப்பு TextAnalysis : பொது IDWriteTextAnalysisSource, பொது IDWriteTextAnalysisSink
{
தனிப்பட்ட:
  hb_reference_மொத்தம்_t m_Ref_மொத்தம்;
பொது:
  IFACEMETHOD (QueryInterface) (IID மாறிலி& iid, OUT வெறுமை** ppObject)
  { திரும்பு S_OK; }
  IFACEMETHOD_ (ULONG, AddRef) ()
  {
    திரும்பு m_Ref_மொத்தம்.inc () + 1;
  }
  IFACEMETHOD_ (ULONG, Release) ()
  {
    signed ref_மொத்தம் = m_Ref_மொத்தம்.dec () - 1;
    ப்ரதிஜ்ஞா (ref_மொத்தம் >= 0);
    எனில் (ref_மொத்தம்)
      திரும்பு ref_மொத்தம்;
    திரும்பு 0;
  }

  // A single contiguous run of characters containing the same analysis
  // results.
  கட்டமைப்பு Run
  {
    uint32_t mTextStart;   // starting text position of this run
    uint32_t mTextLength;  // number of contiguous code units covered
    uint32_t mGlyphStart;  // starting glyph in the glyphs array
    uint32_t m_Glyph_மொத்தம்;  // number of glyphs associated with this run
    // text
    DWRITE_SCRIPT_ANALYSIS mScript;
    uint8_t mBidiLevel;
    தருக்க mIsSideways;

    தருக்க ContainsTextPosition (uint32_t aTextPosition) மாறிலி
    {
      திரும்பு aTextPosition >= mTextStart &&
	     aTextPosition <  mTextStart + mTextLength;
    }

    Run *nextRun;
  };

பொது:
  TextAnalysis (மாறிலி wchar_t* text, uint32_t textLength,
		மாறிலி wchar_t* localeName, DWRITE_READING_DIRECTION readingDirection)
	       : mTextLength (textLength), mText (text), mLocaleName (localeName),
		 mReadingDirection (readingDirection), mCurrentRun (nullptr)
  {
    m_Ref_மொத்தம்.init ();
  }
  ~TextAnalysis ()
  {
    // free runs, except mRunHead which is part of the TextAnalysis object
    க்கு (Run *run = mRunHead.nextRun; run;)
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
      (mReadingDirection == DWRITE_READING_DIRECTION_வலது_TO_இடது);
    mRunHead.nextRun = nullptr;
    mCurrentRun = &mRunHead;

    // Call each of the analyzers in sequence, recording their results.
    எனில் (SUCCEEDED (hr = textAnalyzer->AnalyzeScript (இது, 0, mTextLength, இது)))
      *runHead = &mRunHead;

    திரும்பு hr;
  }

  // IDWriteTextAnalysisSource implementation

  IFACEMETHODIMP
  GetTextAtPosition (uint32_t textPosition,
		     OUT wchar_t மாறிலி** textString,
		     OUT uint32_t* textLength)
  {
    எனில் (textPosition >= mTextLength)
    {
      // No text at this position, valid query though.
      *textString = nullptr;
      *textLength = 0;
    }
    இல்லையெனில்
    {
      *textString = mText + textPosition;
      *textLength = mTextLength - textPosition;
    }
    திரும்பு S_OK;
  }

  IFACEMETHODIMP
  GetTextBeforePosition (uint32_t textPosition,
			 OUT wchar_t மாறிலி** textString,
			 OUT uint32_t* textLength)
  {
    எனில் (textPosition == 0 || textPosition > mTextLength)
    {
      // Either there is no text before here (== 0), or this
      // is an invalid position. The query is considered valid though.
      *textString = nullptr;
      *textLength = 0;
    }
    இல்லையெனில்
    {
      *textString = mText;
      *textLength = textPosition;
    }
    திரும்பு S_OK;
  }

  IFACEMETHODIMP_ (DWRITE_READING_DIRECTION)
  GetParagraphReadingDirection () { திரும்பு mReadingDirection; }

  IFACEMETHODIMP GetLocaleName (uint32_t textPosition, uint32_t* textLength,
				wchar_t மாறிலி** localeName)
  { திரும்பு S_OK; }

  IFACEMETHODIMP
  Get_எண்_Substitution (uint32_t textPosition,
			 OUT uint32_t* textLength,
			 OUT IDWrite_எண்_Substitution** எண்_Substitution)
  {
    // We do not support number substitution.
    *எண்_Substitution = nullptr;
    *textLength = mTextLength - textPosition;

    திரும்பு S_OK;
  }

  // IDWriteTextAnalysisSink implementation

  IFACEMETHODIMP
  SetScriptAnalysis (uint32_t textPosition, uint32_t textLength,
		     DWRITE_SCRIPT_ANALYSIS மாறிலி* scriptAnalysis)
  {
    SetCurrentRun (textPosition);
    எனில் (unlikely (!SplitCurrentRun (textPosition)))
      திரும்பு E_OUTOFMEMORY;
    போது (textLength > 0)
    {
      Run *run = FetchNextRun (&textLength);
      எனில் (unlikely (!run))
        திரும்பு E_OUTOFMEMORY;
      run->mScript = *scriptAnalysis;
    }

    திரும்பு S_OK;
  }

  IFACEMETHODIMP
  Set_வரி_Breakpoints (uint32_t textPosition,
		      uint32_t textLength,
		      மாறிலி DWRITE_வரி_BREAKPOINT* வரி_Breakpoints)
  { திரும்பு S_OK; }

  IFACEMETHODIMP SetBidiLevel (uint32_t textPosition, uint32_t textLength,
			       uint8_t explicitLevel, uint8_t resolvedLevel)
  { திரும்பு S_OK; }

  IFACEMETHODIMP
  Set_எண்_Substitution (uint32_t textPosition, uint32_t textLength,
			 IDWrite_எண்_Substitution* எண்_Substitution)
  { திரும்பு S_OK; }

பாதுகாக்கப்பட்ட:
  Run *FetchNextRun (IN OUT uint32_t* textLength)
  {
    // Used by the sink setters, this returns a reference to the next run.
    // Position and length are adjusted to now point after the current run
    // being returned.

    Run *origRun = mCurrentRun;
    // Split the tail if needed (the length remaining is less than the
    // current run's size).
    எனில் (*textLength < mCurrentRun->mTextLength)
    {
      எனில் (unlikely (!SplitCurrentRun (mCurrentRun->mTextStart + *textLength)))
        திரும்பு nullptr;
    }
    இல்லையெனில்
      // Just advance the current run.
      mCurrentRun = mCurrentRun->nextRun;
    *textLength -= origRun->mTextLength;

    // Return a reference to the run that was just current.
    திரும்பு origRun;
  }

  வெறுமை SetCurrentRun (uint32_t textPosition)
  {
    // Move the current run to the given position.
    // Since the analyzers generally return results in a forward manner,
    // this will usually just return early. If not, find the
    // corresponding run for the text position.

    எனில் (mCurrentRun && mCurrentRun->ContainsTextPosition (textPosition))
      திரும்பு;

    க்கு (Run *run = &mRunHead; run; run = run->nextRun)
      எனில் (run->ContainsTextPosition (textPosition))
      {
	mCurrentRun = run;
	திரும்பு;
      }
    ப்ரதிஜ்ஞா (0); // We should always be able to find the text position in one of our runs
  }

  தருக்க SplitCurrentRun (uint32_t splitPosition)
  {
    எனில் (!mCurrentRun)
    {
      ப்ரதிஜ்ஞா (0); // SplitCurrentRun called without current run
      // Shouldn't be calling this when no current run is set!
      திரும்பு பொய்;
    }
    // Split the current run.
    எனில் (splitPosition <= mCurrentRun->mTextStart)
    {
      // No need to split, already the start of a run
      // or before it. Usually the first.
      திரும்பு உண்மை;
    }
    Run *newRun = (Run *) hb_malloc (அளவு (Run));
    எனில் (unlikely (!newRun))
      திரும்பு பொய்;

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
    திரும்பு உண்மை;
  }

பாதுகாக்கப்பட்ட:
  //உள்ளீடு
  // (weak references are fine here, since this class is a transient
  //  stack-based helper that doesn't need to copy data)
  uint32_t mTextLength;
  மாறிலி wchar_t* mText;
  மாறிலி wchar_t* mLocaleName;
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
		       மாறிலி hb_feature_t *features,
		       unsigned முழு        இலக்கம்_features)
{
  IDWriteFontFace *fontFace = (IDWriteFontFace *) (மாறிலி வெறுமை *) font->data.directwrite;
  auto *global = get_directwrite_global ();
  எனில் (unlikely (!global))
    திரும்பு பொய்;
  IDWriteFactory *dwriteFactory = global->dwriteFactory;

  IDWriteTextAnalyzer* analyzer;
  dwriteFactory->CreateTextAnalyzer (&analyzer);

  unsigned முழு scratch_size;
  hb_buffer_t::scratch_buffer_t *scratch = buffer->get_scratch_buffer (&scratch_size);
#define ALLOCATE_ARRAY(Type, name, len) \
  Type *name = (Type *) scratch; \
  செய் { \
    unsigned முழு _consumed = DIV_CEIL ((len) * அளவு (Type), அளவு (*scratch)); \
    ப்ரதிஜ்ஞா (_consumed <= scratch_size); \
    scratch += _consumed; \
    scratch_size -= _consumed; \
  } போது (0)

#define utf16_index() var1.u32

  ALLOCATE_ARRAY (wchar_t, textString, buffer->len * 2);

  unsigned முழு chars_len = 0;
  க்கு (unsigned முழு i = 0; i < buffer->len; i++)
  {
    hb_codepoint_t c = buffer->info[i].codepoint;
    buffer->info[i].utf16_index () = chars_len;
    எனில் (likely (c <= 0xFFFFu))
      textString[chars_len++] = c;
    இல்லையெனில் எனில் (unlikely (c > 0x10FFFFu))
      textString[chars_len++] = 0xFFFDu;
    இல்லையெனில்
    {
      textString[chars_len++] = 0xD800u + ((c - 0x10000u) >> 10);
      textString[chars_len++] = 0xDC00u + ((c - 0x10000u) & ((1u << 10) - 1));
    }
  }

  ALLOCATE_ARRAY (WORD, log_clusters, chars_len);
#undef ALLOCATE_ARRAY
  /* Need log_clusters to assign features. */
  chars_len = 0;
  க்கு (unsigned முழு i = 0; i < buffer->len; i++)
  {
    hb_codepoint_t c = buffer->info[i].codepoint;
    unsigned முழு cluster = buffer->info[i].cluster;
    log_clusters[chars_len++] = cluster;
    எனில் (hb_in_range (c, 0x10000u, 0x10FFFFu))
      log_clusters[chars_len++] = cluster; /* Surrogates. */
  }

  DWRITE_READING_DIRECTION readingDirection;
  readingDirection = buffer->props.direction ?
		     DWRITE_READING_DIRECTION_வலது_TO_இடது :
		     DWRITE_READING_DIRECTION_இடது_TO_வலது;

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
    DEBUG_செய்தி (DIRECTWRITE, nullptr, __VA_ARGS__); \
    திரும்பு பொய்; \
  } HB_STMT_END

  எனில் (FAILED (hr))
    FAIL ("Analyzer failed to generate results.");

  uint32_t max_Glyph_மொத்தம் = 3 * textLength / 2 + 16;
  uint32_t glyph_மொத்தம்_2;
  தருக்க is_வலது_To_இடது = HB_DIRECTION_IS_BACKWARD (buffer->props.direction);

  மாறிலி wchar_t localeName[20] = {0};
  எனில் (buffer->props.language)
    mbstowcs ((wchar_t*) localeName,
	      hb_language_to_string (buffer->props.language), 20);

  /*
   * Set up features.
   */
  static_assert ((அளவு (DWRITE_TYPOGRAPHIC_FEATURES) == அளவு (hb_ms_features_t)), "");
  static_assert ((அளவு (DWRITE_FONT_FEATURE) == அளவு (hb_ms_feature_t)), "");
  hb_vector_t<hb_ms_features_t *> range_features;
  hb_vector_t<uint32_t> range_char_counts;

  // https://github.com/harfbuzz/harfbuzz/pull/5114
  // The data allocated by these two vectors are used by the above two, so they
  // should remain alive as long as the above two are.
  hb_vector_t<hb_ms_feature_t> feature_records;
  hb_vector_t<hb_ms_range_record_t> range_records;
  எனில் (இலக்கம்_features)
  {
    எனில் (hb_ms_setup_features (features, இலக்கம்_features, feature_records, range_records))
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
  எனில் (unlikely (!clusterMap.resize_exact (textLength) ||
                !textProperties.resize_exact (textLength)))
    FAIL ("Failed to allocate shaping text data.");

  hb_vector_t<uint16_t> glyphIndices;
  hb_vector_t<DWRITE_SHAPING_GLYPH_PROPERTIES> glyphProperties;

retry_getglyphs:
  எனில் (unlikely (!glyphIndices.resize_exact (max_Glyph_மொத்தம்) ||
                !glyphProperties.resize_exact (max_Glyph_மொத்தம்)))
    FAIL ("Failed to allocate shaping glyph data.");

  hr = analyzer->GetGlyphs (textString,
			    chars_len,
			    fontFace,
			    பொய்,
			    is_வலது_To_இடது,
			    &runHead->mScript,
			    localeName,
			    nullptr,
			    (மாறிலி DWRITE_TYPOGRAPHIC_FEATURES**) range_features.arrayZ,
			    range_char_counts.arrayZ,
			    range_features.length,
			    max_Glyph_மொத்தம்,
			    clusterMap.arrayZ,
			    textProperties.arrayZ,
			    glyphIndices.arrayZ,
			    glyphProperties.arrayZ,
			    &glyph_மொத்தம்_2);

  எனில் (unlikely (hr == HRESULT_FROM_WIN32 (ERROR_INSUFFICIENT_BUFFER)))
  {
    max_Glyph_மொத்தம் *= 2;

    செல் retry_getglyphs;
  }
  எனில் (FAILED (hr))
    FAIL ("Analyzer failed to get glyphs.");

  hb_vector_t<தசம> glyphAdvances;
  hb_vector_t<DWRITE_GLYPH_OFFSET> glyphOffsets;
  எனில் (unlikely (!glyphAdvances.resize_exact (max_Glyph_மொத்தம்) ||
                !glyphOffsets.resize_exact (max_Glyph_மொத்தம்)))
    FAIL ("Failed to allocate glyph positioning data.");


  unsigned fontEmSize = font->face->get_upem ();

  தசம x_mult = font->x_multf;
  தசம y_mult = font->y_multf;

  hr = analyzer->GetGlyphPlacements (textString,
				     clusterMap.arrayZ,
				     textProperties.arrayZ,
				     chars_len,
				     glyphIndices.arrayZ,
				     glyphProperties.arrayZ,
				     glyph_மொத்தம்_2,
				     fontFace,
				     fontEmSize,
				     பொய்,
				     is_வலது_To_இடது,
				     &runHead->mScript,
				     localeName,
				     (மாறிலி DWRITE_TYPOGRAPHIC_FEATURES**) range_features.arrayZ,
				     range_char_counts.arrayZ,
				     range_features.length,
				     glyphAdvances.arrayZ,
				     glyphOffsets.arrayZ);

  எனில் (FAILED (hr))
    FAIL ("Analyzer failed to get glyph placements.");

  /* Ok, we've got everything we need, now compose output buffer,
   * very, *very*, carefully! */

  /* Calculate visual-clusters.  That's what we ship. */
  hb_vector_t<uint32_t> vis_clusters;
  எனில் (unlikely (!vis_clusters.resize_exact (glyph_மொத்தம்_2)))
    FAIL ("Failed to allocate visual clusters.");

  க்கு (unsigned முழு i = 0; i < glyph_மொத்தம்_2; i++)
    vis_clusters[i] = (uint32_t) -1;
  க்கு (unsigned முழு i = 0; i < buffer->len; i++)
  {
    unsigned முழு c = clusterMap[buffer->info[i].utf16_index ()];
    எனில் (likely (c < glyph_மொத்தம்_2))
      vis_clusters[c] = hb_min (vis_clusters[c], buffer->info[i].cluster);
  }
  க்கு (unsigned முழு i = 1; i < glyph_மொத்தம்_2; i++)
    எனில் (vis_clusters[i] == (uint32_t) -1)
      vis_clusters[i] = vis_clusters[i - 1];
  க்கு (முழு i = (முழு) glyph_மொத்தம்_2 - 2; i >= 0; i--)
    எனில் (vis_clusters[i] == (uint32_t) -1)
      vis_clusters[i] = vis_clusters[i + 1];

#undef utf16_index

  எனில் (unlikely (!buffer->ensure (glyph_மொத்தம்_2)))
    FAIL ("Buffer in error");

#undef FAIL

  /* Set glyph infos */
  buffer->len = 0;
  க்கு (unsigned முழு i = 0; i < glyph_மொத்தம்_2; i++)
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
  க்கு (unsigned முழு i = 0; i < glyph_மொத்தம்_2; i++)
  {
    hb_glyph_info_t *info = &buffer->info[i];
    hb_glyph_position_t *pos = &buffer->pos[i];

    /* TODO vertical */
    pos->x_advance = hb_clamp_to<hb_position_t> (round (x_mult * (int32_t) info->mask));
    இரட்டை x_offset = is_வலது_To_இடது ? -(இரட்டை) info->var1.i32 : info->var1.i32;
    pos->x_offset = hb_clamp_to<hb_position_t> (round ((இரட்டை) x_mult * x_offset));
    pos->y_offset = hb_clamp_to<hb_position_t> (round (y_mult * info->var2.i32));
  }

  எனில் (is_வலது_To_இடது) hb_buffer_reverse (buffer);

  buffer->clear_glyph_flags ();
  buffer->unsafe_to_break ();

  /* Wow, done! */
  திரும்பு உண்மை;
}


#endif
