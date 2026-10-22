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

#অন্তর্ভাব "hb.hh"

#ifdef HAVE_DIRECTWRITE

#অন্তর্ভাব "hb-shaper-impl.hh"

#অন্তর্ভাব "hb-directwrite.hh"

#অন্তর্ভাব "hb-ms-feature-ranges.hh"


/*
* shaper face data
*/

hb_directwrite_face_data_t *
_hb_directwrite_shaper_face_data_create (hb_face_t *face)
{
  hb_blob_t *blob = hb_face_reference_blob (face);

  hb_directwrite_face_data_t *data = (hb_directwrite_face_data_t *) dw_face_create (blob, face->index);

  hb_blob_destroy (blob);

  প্রত্যাগম data;
}

শূন্য
_hb_directwrite_shaper_face_data_destroy (hb_directwrite_face_data_t *data)
{
  ((IDWriteFontFace *) data)->Release ();
}


/*
 * shaper font data
 */

সংরচনা hb_directwrite_font_data_t {};

hb_directwrite_font_data_t *
_hb_directwrite_shaper_font_data_create (hb_font_t *font)
{
  IDWriteFontFace *fontFace = (IDWriteFontFace *) (ধ্রুব শূন্য *) font->face->data.directwrite;

  /*
   * Set up variations.
   */
  IDWriteFontFace5 *fontFaceVariations = nullptr;
  {
    IDWriteFontFace5 *fontFace5;
    যদি (SUCCEEDED (fontFace->QueryInterface (__uuidof (IDWriteFontFace5), (শূন্য **) &fontFace5)))
    {
      IDWriteFontResource *fontResource;
      যদি (SUCCEEDED (fontFace5->GetFontResource (&fontResource)))
      {
	hb_vector_t<DWRITE_FONT_AXIS_মান> axis_মানসমূহ;
	যদি (likely (axis_মানসমূহ.resize_exact (font->অঙ্ক_coords)))
	{
	  প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < font->অঙ্ক_coords; i++)
	  {
	    hb_ot_var_axis_info_t info;
	    unsigned পূর্ণাঙ্ক c = 1;
	    hb_ot_var_get_axis_infos (font->face, i, &c, &info);
	    axis_মানসমূহ[i].axisTag = (DWRITE_FONT_AXIS_TAG) hb_uint32_swap (info.tag);
	    axis_মানসমূহ[i].value = i < font->অঙ্ক_coords ?
				   hb_clamp (font->design_coords[i], info.min_মান, info.max_মান) :
				   info.default_মান;
	  }

	  fontResource->CreateFontFace (DWRITE_FONT_SIMULATIONS::DWRITE_FONT_SIMULATIONS_NONE,
					axis_মানসমূহ.arrayZ, axis_মানসমূহ.length, &fontFaceVariations);
	}
	fontResource->Release ();
      }
      fontFace5->Release ();
    }
  }

  প্রত্যাগম (hb_directwrite_font_data_t *) fontFaceVariations;
}

শূন্য
_hb_directwrite_shaper_font_data_destroy (hb_directwrite_font_data_t *data)
{
  ((IDWriteFontFace *) (ধ্রুব শূন্য *) data)->Release ();
}


// Most of TextAnalysis is originally written by Bas Schouten for Mozilla project
// but now is relicensed to MIT for HarfBuzz use
বর্গ TextAnalysis : সার্বজনিক IDWriteTextAnalysisSource, সার্বজনিক IDWriteTextAnalysisSink
{
গুপ্ত:
  hb_reference_গুনতি_t m_Ref_গুনতি;
সার্বজনিক:
  IFACEMETHOD (QueryInterface) (IID ধ্রুব& iid, OUT শূন্য** ppObject)
  { প্রত্যাগম S_OK; }
  IFACEMETHOD_ (ULONG, AddRef) ()
  {
    প্রত্যাগম m_Ref_গুনতি.inc () + 1;
  }
  IFACEMETHOD_ (ULONG, Release) ()
  {
    signed ref_গুনতি = m_Ref_গুনতি.dec () - 1;
    প্রতিজ্ঞা (ref_গুনতি >= 0);
    যদি (ref_গুনতি)
      প্রত্যাগম ref_গুনতি;
    প্রত্যাগম 0;
  }

  // A single contiguous run of characters containing the same analysis
  // results.
  সংরচনা Run
  {
    uint32_t mTextStart;   // starting text position of this run
    uint32_t mTextLength;  // number of contiguous code units covered
    uint32_t mGlyphStart;  // starting glyph in the glyphs array
    uint32_t m_Glyph_গুনতি;  // number of glyphs associated with this run
    // text
    DWRITE_SCRIPT_ANALYSIS mScript;
    uint8_t mBidiLevel;
    তার্কিক mIsSideways;

    তার্কিক ContainsTextPosition (uint32_t aTextPosition) ধ্রুব
    {
      প্রত্যাগম aTextPosition >= mTextStart &&
	     aTextPosition <  mTextStart + mTextLength;
    }

    Run *nextRun;
  };

সার্বজনিক:
  TextAnalysis (ধ্রুব wchar_t* text, uint32_t textLength,
		ধ্রুব wchar_t* localeName, DWRITE_READING_DIRECTION readingDirection)
	       : mTextLength (textLength), mText (text), mLocaleName (localeName),
		 mReadingDirection (readingDirection), mCurrentRun (nullptr)
  {
    m_Ref_গুনতি.init ();
  }
  ~TextAnalysis ()
  {
    // free runs, except mRunHead which is part of the TextAnalysis object
    প্রতি (Run *run = mRunHead.nextRun; run;)
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
      (mReadingDirection == DWRITE_READING_DIRECTION_ডান_TO_বাম);
    mRunHead.nextRun = nullptr;
    mCurrentRun = &mRunHead;

    // Call each of the analyzers in sequence, recording their results.
    যদি (SUCCEEDED (hr = textAnalyzer->AnalyzeScript (এতত্, 0, mTextLength, এতত্)))
      *runHead = &mRunHead;

    প্রত্যাগম hr;
  }

  // IDWriteTextAnalysisSource implementation

  IFACEMETHODIMP
  GetTextAtPosition (uint32_t textPosition,
		     OUT wchar_t ধ্রুব** textString,
		     OUT uint32_t* textLength)
  {
    যদি (textPosition >= mTextLength)
    {
      // No text at this position, valid query though.
      *textString = nullptr;
      *textLength = 0;
    }
    অন্যথা
    {
      *textString = mText + textPosition;
      *textLength = mTextLength - textPosition;
    }
    প্রত্যাগম S_OK;
  }

  IFACEMETHODIMP
  GetTextBeforePosition (uint32_t textPosition,
			 OUT wchar_t ধ্রুব** textString,
			 OUT uint32_t* textLength)
  {
    যদি (textPosition == 0 || textPosition > mTextLength)
    {
      // Either there is no text before here (== 0), or this
      // is an invalid position. The query is considered valid though.
      *textString = nullptr;
      *textLength = 0;
    }
    অন্যথা
    {
      *textString = mText;
      *textLength = textPosition;
    }
    প্রত্যাগম S_OK;
  }

  IFACEMETHODIMP_ (DWRITE_READING_DIRECTION)
  GetParagraphReadingDirection () { প্রত্যাগম mReadingDirection; }

  IFACEMETHODIMP GetLocaleName (uint32_t textPosition, uint32_t* textLength,
				wchar_t ধ্রুব** localeName)
  { প্রত্যাগম S_OK; }

  IFACEMETHODIMP
  Get_সংখ্যা_Substitution (uint32_t textPosition,
			 OUT uint32_t* textLength,
			 OUT IDWrite_সংখ্যা_Substitution** সংখ্যা_Substitution)
  {
    // We do not support number substitution.
    *সংখ্যা_Substitution = nullptr;
    *textLength = mTextLength - textPosition;

    প্রত্যাগম S_OK;
  }

  // IDWriteTextAnalysisSink implementation

  IFACEMETHODIMP
  SetScriptAnalysis (uint32_t textPosition, uint32_t textLength,
		     DWRITE_SCRIPT_ANALYSIS ধ্রুব* scriptAnalysis)
  {
    SetCurrentRun (textPosition);
    যদি (unlikely (!SplitCurrentRun (textPosition)))
      প্রত্যাগম E_OUTOFMEMORY;
    যাবত্ (textLength > 0)
    {
      Run *run = FetchNextRun (&textLength);
      যদি (unlikely (!run))
        প্রত্যাগম E_OUTOFMEMORY;
      run->mScript = *scriptAnalysis;
    }

    প্রত্যাগম S_OK;
  }

  IFACEMETHODIMP
  Set_সারি_Breakpoints (uint32_t textPosition,
		      uint32_t textLength,
		      ধ্রুব DWRITE_সারি_BREAKPOINT* সারি_Breakpoints)
  { প্রত্যাগম S_OK; }

  IFACEMETHODIMP SetBidiLevel (uint32_t textPosition, uint32_t textLength,
			       uint8_t explicitLevel, uint8_t resolvedLevel)
  { প্রত্যাগম S_OK; }

  IFACEMETHODIMP
  Set_সংখ্যা_Substitution (uint32_t textPosition, uint32_t textLength,
			 IDWrite_সংখ্যা_Substitution* সংখ্যা_Substitution)
  { প্রত্যাগম S_OK; }

রক্ষিত:
  Run *FetchNextRun (IN OUT uint32_t* textLength)
  {
    // Used by the sink setters, this returns a reference to the next run.
    // Position and length are adjusted to now point after the current run
    // being returned.

    Run *origRun = mCurrentRun;
    // Split the tail if needed (the length remaining is less than the
    // current run's size).
    যদি (*textLength < mCurrentRun->mTextLength)
    {
      যদি (unlikely (!SplitCurrentRun (mCurrentRun->mTextStart + *textLength)))
        প্রত্যাগম nullptr;
    }
    অন্যথা
      // Just advance the current run.
      mCurrentRun = mCurrentRun->nextRun;
    *textLength -= origRun->mTextLength;

    // Return a reference to the run that was just current.
    প্রত্যাগম origRun;
  }

  শূন্য SetCurrentRun (uint32_t textPosition)
  {
    // Move the current run to the given position.
    // Since the analyzers generally return results in a forward manner,
    // this will usually just return early. If not, find the
    // corresponding run for the text position.

    যদি (mCurrentRun && mCurrentRun->ContainsTextPosition (textPosition))
      প্রত্যাগম;

    প্রতি (Run *run = &mRunHead; run; run = run->nextRun)
      যদি (run->ContainsTextPosition (textPosition))
      {
	mCurrentRun = run;
	প্রত্যাগম;
      }
    প্রতিজ্ঞা (0); // We should always be able to find the text position in one of our runs
  }

  তার্কিক SplitCurrentRun (uint32_t splitPosition)
  {
    যদি (!mCurrentRun)
    {
      প্রতিজ্ঞা (0); // SplitCurrentRun called without current run
      // Shouldn't be calling this when no current run is set!
      প্রত্যাগম অসত্য;
    }
    // Split the current run.
    যদি (splitPosition <= mCurrentRun->mTextStart)
    {
      // No need to split, already the start of a run
      // or before it. Usually the first.
      প্রত্যাগম সত্য;
    }
    Run *newRun = (Run *) hb_malloc (পরিমাণ (Run));
    যদি (unlikely (!newRun))
      প্রত্যাগম অসত্য;

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
    প্রত্যাগম সত্য;
  }

রক্ষিত:
  //প্রবেশ
  // (weak references are fine here, since this class is a transient
  //  stack-based helper that doesn't need to copy data)
  uint32_t mTextLength;
  ধ্রুব wchar_t* mText;
  ধ্রুব wchar_t* mLocaleName;
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
		       ধ্রুব hb_feature_t *features,
		       unsigned পূর্ণাঙ্ক        অঙ্ক_features)
{
  IDWriteFontFace *fontFace = (IDWriteFontFace *) (ধ্রুব শূন্য *) font->data.directwrite;
  auto *global = get_directwrite_global ();
  যদি (unlikely (!global))
    প্রত্যাগম অসত্য;
  IDWriteFactory *dwriteFactory = global->dwriteFactory;

  IDWriteTextAnalyzer* analyzer;
  dwriteFactory->CreateTextAnalyzer (&analyzer);

  unsigned পূর্ণাঙ্ক scratch_size;
  hb_buffer_t::scratch_buffer_t *scratch = buffer->get_scratch_buffer (&scratch_size);
#define ALLOCATE_ARRAY(Type, name, len) \
  Type *name = (Type *) scratch; \
  কুরু { \
    unsigned পূর্ণাঙ্ক _consumed = DIV_CEIL ((len) * পরিমাণ (Type), পরিমাণ (*scratch)); \
    প্রতিজ্ঞা (_consumed <= scratch_size); \
    scratch += _consumed; \
    scratch_size -= _consumed; \
  } যাবত্ (0)

#define utf16_index() var1.u32

  ALLOCATE_ARRAY (wchar_t, textString, buffer->len * 2);

  unsigned পূর্ণাঙ্ক chars_len = 0;
  প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < buffer->len; i++)
  {
    hb_codepoint_t c = buffer->info[i].codepoint;
    buffer->info[i].utf16_index () = chars_len;
    যদি (likely (c <= 0xFFFFu))
      textString[chars_len++] = c;
    অন্যথা যদি (unlikely (c > 0x10FFFFu))
      textString[chars_len++] = 0xFFFDu;
    অন্যথা
    {
      textString[chars_len++] = 0xD800u + ((c - 0x10000u) >> 10);
      textString[chars_len++] = 0xDC00u + ((c - 0x10000u) & ((1u << 10) - 1));
    }
  }

  ALLOCATE_ARRAY (WORD, log_clusters, chars_len);
#undef ALLOCATE_ARRAY
  /* Need log_clusters to assign features. */
  chars_len = 0;
  প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < buffer->len; i++)
  {
    hb_codepoint_t c = buffer->info[i].codepoint;
    unsigned পূর্ণাঙ্ক cluster = buffer->info[i].cluster;
    log_clusters[chars_len++] = cluster;
    যদি (hb_in_range (c, 0x10000u, 0x10FFFFu))
      log_clusters[chars_len++] = cluster; /* Surrogates. */
  }

  DWRITE_READING_DIRECTION readingDirection;
  readingDirection = buffer->props.direction ?
		     DWRITE_READING_DIRECTION_ডান_TO_বাম :
		     DWRITE_READING_DIRECTION_বাম_TO_ডান;

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
    DEBUG_বার্তা (DIRECTWRITE, nullptr, __VA_ARGS__); \
    প্রত্যাগম অসত্য; \
  } HB_STMT_END

  যদি (FAILED (hr))
    FAIL ("Analyzer failed to generate results.");

  uint32_t max_Glyph_গুনতি = 3 * textLength / 2 + 16;
  uint32_t glyph_গুনতি_2;
  তার্কিক is_ডান_To_বাম = HB_DIRECTION_IS_BACKWARD (buffer->props.direction);

  ধ্রুব wchar_t localeName[20] = {0};
  যদি (buffer->props.language)
    mbstowcs ((wchar_t*) localeName,
	      hb_language_to_string (buffer->props.language), 20);

  /*
   * Set up features.
   */
  static_assert ((পরিমাণ (DWRITE_TYPOGRAPHIC_FEATURES) == পরিমাণ (hb_ms_features_t)), "");
  static_assert ((পরিমাণ (DWRITE_FONT_FEATURE) == পরিমাণ (hb_ms_feature_t)), "");
  hb_vector_t<hb_ms_features_t *> range_features;
  hb_vector_t<uint32_t> range_char_counts;

  // https://github.com/harfbuzz/harfbuzz/pull/5114
  // The data allocated by these two vectors are used by the above two, so they
  // should remain alive as long as the above two are.
  hb_vector_t<hb_ms_feature_t> feature_records;
  hb_vector_t<hb_ms_range_record_t> range_records;
  যদি (অঙ্ক_features)
  {
    যদি (hb_ms_setup_features (features, অঙ্ক_features, feature_records, range_records))
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
  যদি (unlikely (!clusterMap.resize_exact (textLength) ||
                !textProperties.resize_exact (textLength)))
    FAIL ("Failed to allocate shaping text data.");

  hb_vector_t<uint16_t> glyphIndices;
  hb_vector_t<DWRITE_SHAPING_GLYPH_PROPERTIES> glyphProperties;

retry_getglyphs:
  যদি (unlikely (!glyphIndices.resize_exact (max_Glyph_গুনতি) ||
                !glyphProperties.resize_exact (max_Glyph_গুনতি)))
    FAIL ("Failed to allocate shaping glyph data.");

  hr = analyzer->GetGlyphs (textString,
			    chars_len,
			    fontFace,
			    অসত্য,
			    is_ডান_To_বাম,
			    &runHead->mScript,
			    localeName,
			    nullptr,
			    (ধ্রুব DWRITE_TYPOGRAPHIC_FEATURES**) range_features.arrayZ,
			    range_char_counts.arrayZ,
			    range_features.length,
			    max_Glyph_গুনতি,
			    clusterMap.arrayZ,
			    textProperties.arrayZ,
			    glyphIndices.arrayZ,
			    glyphProperties.arrayZ,
			    &glyph_গুনতি_2);

  যদি (unlikely (hr == HRESULT_FROM_WIN32 (ERROR_INSUFFICIENT_BUFFER)))
  {
    max_Glyph_গুনতি *= 2;

    গচ্ছ retry_getglyphs;
  }
  যদি (FAILED (hr))
    FAIL ("Analyzer failed to get glyphs.");

  hb_vector_t<ভিন্ন> glyphAdvances;
  hb_vector_t<DWRITE_GLYPH_OFFSET> glyphOffsets;
  যদি (unlikely (!glyphAdvances.resize_exact (max_Glyph_গুনতি) ||
                !glyphOffsets.resize_exact (max_Glyph_গুনতি)))
    FAIL ("Failed to allocate glyph positioning data.");


  unsigned fontEmSize = font->face->get_upem ();

  ভিন্ন x_mult = font->x_multf;
  ভিন্ন y_mult = font->y_multf;

  hr = analyzer->GetGlyphPlacements (textString,
				     clusterMap.arrayZ,
				     textProperties.arrayZ,
				     chars_len,
				     glyphIndices.arrayZ,
				     glyphProperties.arrayZ,
				     glyph_গুনতি_2,
				     fontFace,
				     fontEmSize,
				     অসত্য,
				     is_ডান_To_বাম,
				     &runHead->mScript,
				     localeName,
				     (ধ্রুব DWRITE_TYPOGRAPHIC_FEATURES**) range_features.arrayZ,
				     range_char_counts.arrayZ,
				     range_features.length,
				     glyphAdvances.arrayZ,
				     glyphOffsets.arrayZ);

  যদি (FAILED (hr))
    FAIL ("Analyzer failed to get glyph placements.");

  /* Ok, we've got everything we need, now compose output buffer,
   * very, *very*, carefully! */

  /* Calculate visual-clusters.  That's what we ship. */
  hb_vector_t<uint32_t> vis_clusters;
  যদি (unlikely (!vis_clusters.resize_exact (glyph_গুনতি_2)))
    FAIL ("Failed to allocate visual clusters.");

  প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < glyph_গুনতি_2; i++)
    vis_clusters[i] = (uint32_t) -1;
  প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < buffer->len; i++)
  {
    unsigned পূর্ণাঙ্ক c = clusterMap[buffer->info[i].utf16_index ()];
    যদি (likely (c < glyph_গুনতি_2))
      vis_clusters[c] = hb_min (vis_clusters[c], buffer->info[i].cluster);
  }
  প্রতি (unsigned পূর্ণাঙ্ক i = 1; i < glyph_গুনতি_2; i++)
    যদি (vis_clusters[i] == (uint32_t) -1)
      vis_clusters[i] = vis_clusters[i - 1];
  প্রতি (পূর্ণাঙ্ক i = (পূর্ণাঙ্ক) glyph_গুনতি_2 - 2; i >= 0; i--)
    যদি (vis_clusters[i] == (uint32_t) -1)
      vis_clusters[i] = vis_clusters[i + 1];

#undef utf16_index

  যদি (unlikely (!buffer->ensure (glyph_গুনতি_2)))
    FAIL ("Buffer in error");

#undef FAIL

  /* Set glyph infos */
  buffer->len = 0;
  প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < glyph_গুনতি_2; i++)
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
  প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < glyph_গুনতি_2; i++)
  {
    hb_glyph_info_t *info = &buffer->info[i];
    hb_glyph_position_t *pos = &buffer->pos[i];

    /* TODO vertical */
    pos->x_advance = hb_clamp_to<hb_position_t> (round (x_mult * (int32_t) info->mask));
    দ্বিগুণ x_offset = is_ডান_To_বাম ? -(দ্বিগুণ) info->var1.i32 : info->var1.i32;
    pos->x_offset = hb_clamp_to<hb_position_t> (round ((দ্বিগুণ) x_mult * x_offset));
    pos->y_offset = hb_clamp_to<hb_position_t> (round (y_mult * info->var2.i32));
  }

  যদি (is_ডান_To_বাম) hb_buffer_reverse (buffer);

  buffer->clear_glyph_flags ();
  buffer->unsafe_to_break ();

  /* Wow, done! */
  প্রত্যাগম সত্য;
}


#endif
