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

hb_directwrite_face_তথ্য_t *
hb_directwrite_shaper_face_তথ্য_create (hb_face_t *face)
{
  hb_blob_t *blob = hb_face_reference_blob (face);

  hb_directwrite_face_তথ্য_t *তথ্য = (hb_directwrite_face_তথ্য_t *) dw_face_create (blob, face->সূচক_2);

  hb_blob_destroy (blob);

  ফেরত তথ্য;
}

শূন্য
hb_directwrite_shaper_face_তথ্য_destroy (hb_directwrite_face_তথ্য_t *তথ্য)
{
  ((IDWriteFontFace *) তথ্য)->Release ();
}


/*
 * shaper font data
 */

কাঠামো hb_directwrite_font_তথ্য_t {};

hb_directwrite_font_তথ্য_t *
hb_directwrite_shaper_font_তথ্য_create (hb_font_t *font)
{
  IDWriteFontFace *fontFace = (IDWriteFontFace *) (ধ্রুবক শূন্য *) font->face->তথ্য.directwrite;

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
	  জন্য (unsigned পূর্ণসংখ্যা i = 0; i < font->অঙ্ক_coords; i++)
	  {
	    hb_ot_var_axis_info_t info;
	    unsigned পূর্ণসংখ্যা c = 1;
	    hb_ot_var_get_axis_infos (font->face, i, &c, &info);
	    axis_মানসমূহ[i].axisTag = (DWRITE_FONT_AXIS_TAG) hb_uint32_swap (info.tag);
	    axis_মানসমূহ[i].value = i < font->অঙ্ক_coords ?
				   hb_clamp (font->design_coords[i], info.min_মান, info.max_মান) :
				   info.default_মান;
	  }

	  fontResource->CreateFontFace (DWRITE_FONT_SIMULATIONS::DWRITE_FONT_SIMULATIONS_NONE,
					axis_মানসমূহ.arrayZ, axis_মানসমূহ.দৈর্ঘ্য, &fontFaceVariations);
	}
	fontResource->Release ();
      }
      fontFace5->Release ();
    }
  }

  ফেরত (hb_directwrite_font_তথ্য_t *) fontFaceVariations;
}

শূন্য
hb_directwrite_shaper_font_তথ্য_destroy (hb_directwrite_font_তথ্য_t *তথ্য)
{
  ((IDWriteFontFace *) (ধ্রুবক শূন্য *) তথ্য)->Release ();
}


// Most of TextAnalysis is originally written by Bas Schouten for Mozilla project
// but now is relicensed to MIT for HarfBuzz use
শ্রেণী TextAnalysis : সর্বজনীন IDWriteTextAnalysisSource, সর্বজনীন IDWriteTextAnalysisSink
{
ব্যক্তিগত:
  hb_reference_গুনতি_t m_Ref_গুনতি;
সর্বজনীন:
  IFACEMETHOD (QueryInterface) (IID ধ্রুবক& iid, OUT শূন্য** ppObject)
  { ফেরত S_OK; }
  IFACEMETHOD_ (ULONG, AddRef) ()
  {
    ফেরত m_Ref_গুনতি.inc () + 1;
  }
  IFACEMETHOD_ (ULONG, Release) ()
  {
    signed ref_গুনতি = m_Ref_গুনতি.dec () - 1;
    প্রতিজ্ঞা (ref_গুনতি >= 0);
    যদি (ref_গুনতি)
      ফেরত ref_গুনতি;
    ফেরত 0;
  }

  // A single contiguous run of characters containing the same analysis
  // results.
  কাঠামো Run
  {
    uint32_t mTextStart;   // starting text position of this run
    uint32_t m_Text_দৈর্ঘ্য;  // number of contiguous code units covered
    uint32_t mGlyphStart;  // starting glyph in the glyphs array
    uint32_t m_Glyph_গুনতি;  // number of glyphs associated with this run
    // text
    DWRITE_SCRIPT_ANALYSIS mScript;
    uint8_t mBidiLevel;
    যৌক্তিক mIsSideways;

    যৌক্তিক ContainsTextPosition (uint32_t aTextPosition) ধ্রুবক
    {
      ফেরত aTextPosition >= mTextStart &&
	     aTextPosition <  mTextStart + m_Text_দৈর্ঘ্য;
    }

    Run *পরবর্তী_Run;
  };

সর্বজনীন:
  TextAnalysis (ধ্রুবক wchar_t* text, uint32_t text_দৈর্ঘ্য_2,
		ধ্রুবক wchar_t* localeName, DWRITE_READING_DIRECTION readingDirection)
	       : m_Text_দৈর্ঘ্য (text_দৈর্ঘ্য_2), mText (text), mLocaleName (localeName),
		 mReadingDirection (readingDirection), mCurrentRun (nullptr)
  {
    m_Ref_গুনতি.init ();
  }
  ~TextAnalysis ()
  {
    // free runs, except mRunHead which is part of the TextAnalysis object
    জন্য (Run *run = m_Run_শীর্ষ.পরবর্তী_Run; run;)
    {
      Run *origRun = run;
      run = run->পরবর্তী_Run;
      hb_free (origRun);
    }
  }

  STDMETHODIMP
  GenerateResults (IDWriteTextAnalyzer* textAnalyzer, Run **run_শীর্ষ)
  {
    // Analyzes the text using the script analyzer and returns
    // the result as a series of runs.

    HRESULT hr = S_OK;

    // Initially start out with one result that covers the entire range.
    // This result will be subdivided by the analysis processes.
    m_Run_শীর্ষ.mTextStart = 0;
    m_Run_শীর্ষ.m_Text_দৈর্ঘ্য = m_Text_দৈর্ঘ্য;
    m_Run_শীর্ষ.mBidiLevel =
      (mReadingDirection == DWRITE_READING_DIRECTION_ডান_TO_বাম);
    m_Run_শীর্ষ.পরবর্তী_Run = nullptr;
    mCurrentRun = &m_Run_শীর্ষ;

    // Call each of the analyzers in sequence, recording their results.
    যদি (SUCCEEDED (hr = textAnalyzer->AnalyzeScript (এই, 0, m_Text_দৈর্ঘ্য, এই)))
      *run_শীর্ষ = &m_Run_শীর্ষ;

    ফেরত hr;
  }

  // IDWriteTextAnalysisSource implementation

  IFACEMETHODIMP
  GetTextAtPosition (uint32_t textPosition,
		     OUT wchar_t ধ্রুবক** textString,
		     OUT uint32_t* text_দৈর্ঘ্য_2)
  {
    যদি (textPosition >= m_Text_দৈর্ঘ্য)
    {
      // No text at this position, valid query though.
      *textString = nullptr;
      *text_দৈর্ঘ্য_2 = 0;
    }
    অন্যথা
    {
      *textString = mText + textPosition;
      *text_দৈর্ঘ্য_2 = m_Text_দৈর্ঘ্য - textPosition;
    }
    ফেরত S_OK;
  }

  IFACEMETHODIMP
  GetTextBeforePosition (uint32_t textPosition,
			 OUT wchar_t ধ্রুবক** textString,
			 OUT uint32_t* text_দৈর্ঘ্য_2)
  {
    যদি (textPosition == 0 || textPosition > m_Text_দৈর্ঘ্য)
    {
      // Either there is no text before here (== 0), or this
      // is an invalid position. The query is considered valid though.
      *textString = nullptr;
      *text_দৈর্ঘ্য_2 = 0;
    }
    অন্যথা
    {
      *textString = mText;
      *text_দৈর্ঘ্য_2 = textPosition;
    }
    ফেরত S_OK;
  }

  IFACEMETHODIMP_ (DWRITE_READING_DIRECTION)
  GetParagraphReadingDirection () { ফেরত mReadingDirection; }

  IFACEMETHODIMP GetLocaleName (uint32_t textPosition, uint32_t* text_দৈর্ঘ্য_2,
				wchar_t ধ্রুবক** localeName)
  { ফেরত S_OK; }

  IFACEMETHODIMP
  Get_সংখ্যা_Substitution (uint32_t textPosition,
			 OUT uint32_t* text_দৈর্ঘ্য_2,
			 OUT IDWrite_সংখ্যা_Substitution** সংখ্যা_Substitution)
  {
    // We do not support number substitution.
    *সংখ্যা_Substitution = nullptr;
    *text_দৈর্ঘ্য_2 = m_Text_দৈর্ঘ্য - textPosition;

    ফেরত S_OK;
  }

  // IDWriteTextAnalysisSink implementation

  IFACEMETHODIMP
  SetScriptAnalysis (uint32_t textPosition, uint32_t text_দৈর্ঘ্য_2,
		     DWRITE_SCRIPT_ANALYSIS ধ্রুবক* scriptAnalysis)
  {
    SetCurrentRun (textPosition);
    যদি (unlikely (!SplitCurrentRun (textPosition)))
      ফেরত E_OUTOFMEMORY;
    যখন (text_দৈর্ঘ্য_2 > 0)
    {
      Run *run = Fetch_পরবর্তী_Run (&text_দৈর্ঘ্য_2);
      যদি (unlikely (!run))
        ফেরত E_OUTOFMEMORY;
      run->mScript = *scriptAnalysis;
    }

    ফেরত S_OK;
  }

  IFACEMETHODIMP
  Set_সারি_Breakpoints (uint32_t textPosition,
		      uint32_t text_দৈর্ঘ্য_2,
		      ধ্রুবক DWRITE_সারি_BREAKPOINT* সারি_Breakpoints)
  { ফেরত S_OK; }

  IFACEMETHODIMP SetBidiLevel (uint32_t textPosition, uint32_t text_দৈর্ঘ্য_2,
			       uint8_t explicitLevel, uint8_t resolvedLevel)
  { ফেরত S_OK; }

  IFACEMETHODIMP
  Set_সংখ্যা_Substitution (uint32_t textPosition, uint32_t text_দৈর্ঘ্য_2,
			 IDWrite_সংখ্যা_Substitution* সংখ্যা_Substitution)
  { ফেরত S_OK; }

সুরক্ষিত:
  Run *Fetch_পরবর্তী_Run (IN OUT uint32_t* text_দৈর্ঘ্য_2)
  {
    // Used by the sink setters, this returns a reference to the next run.
    // Position and length are adjusted to now point after the current run
    // being returned.

    Run *origRun = mCurrentRun;
    // Split the tail if needed (the length remaining is less than the
    // current run's size).
    যদি (*text_দৈর্ঘ্য_2 < mCurrentRun->m_Text_দৈর্ঘ্য)
    {
      যদি (unlikely (!SplitCurrentRun (mCurrentRun->mTextStart + *text_দৈর্ঘ্য_2)))
        ফেরত nullptr;
    }
    অন্যথা
      // Just advance the current run.
      mCurrentRun = mCurrentRun->পরবর্তী_Run;
    *text_দৈর্ঘ্য_2 -= origRun->m_Text_দৈর্ঘ্য;

    // Return a reference to the run that was just current.
    ফেরত origRun;
  }

  শূন্য SetCurrentRun (uint32_t textPosition)
  {
    // Move the current run to the given position.
    // Since the analyzers generally return results in a forward manner,
    // this will usually just return early. If not, find the
    // corresponding run for the text position.

    যদি (mCurrentRun && mCurrentRun->ContainsTextPosition (textPosition))
      ফেরত;

    জন্য (Run *run = &m_Run_শীর্ষ; run; run = run->পরবর্তী_Run)
      যদি (run->ContainsTextPosition (textPosition))
      {
	mCurrentRun = run;
	ফেরত;
      }
    প্রতিজ্ঞা (0); // We should always be able to find the text position in one of our runs
  }

  যৌক্তিক SplitCurrentRun (uint32_t splitPosition)
  {
    যদি (!mCurrentRun)
    {
      প্রতিজ্ঞা (0); // SplitCurrentRun called without current run
      // Shouldn't be calling this when no current run is set!
      ফেরত মিথ্যা;
    }
    // Split the current run.
    যদি (splitPosition <= mCurrentRun->mTextStart)
    {
      // No need to split, already the start of a run
      // or before it. Usually the first.
      ফেরত সত্য;
    }
    Run *newRun = (Run *) hb_malloc (আকার (Run));
    যদি (unlikely (!newRun))
      ফেরত মিথ্যা;

    *newRun = *mCurrentRun;

    // Insert the new run in our linked list.
    newRun->পরবর্তী_Run = mCurrentRun->পরবর্তী_Run;
    mCurrentRun->পরবর্তী_Run = newRun;

    // Adjust runs' text positions and lengths.
    uint32_t splitPoint = splitPosition - mCurrentRun->mTextStart;
    newRun->mTextStart += splitPoint;
    newRun->m_Text_দৈর্ঘ্য -= splitPoint;
    mCurrentRun->m_Text_দৈর্ঘ্য = splitPoint;
    mCurrentRun = newRun;
    ফেরত সত্য;
  }

সুরক্ষিত:
  //প্রবেশ
  // (weak references are fine here, since this class is a transient
  //  stack-based helper that doesn't need to copy data)
  uint32_t m_Text_দৈর্ঘ্য;
  ধ্রুবক wchar_t* mText;
  ধ্রুবক wchar_t* mLocaleName;
  DWRITE_READING_DIRECTION mReadingDirection;

  // Current processing state.
  Run *mCurrentRun;

  // Output is a list of runs starting here
  Run  m_Run_শীর্ষ;
};

/*
 * shaper
 */

hb_bool_t
_hb_directwrite_shape (hb_shape_plan_t    *shape_plan,
		       hb_font_t          *font,
		       hb_বাফার_t        *বাফার,
		       ধ্রুবক hb_feature_t *features,
		       unsigned পূর্ণসংখ্যা        অঙ্ক_features)
{
  IDWriteFontFace *fontFace = (IDWriteFontFace *) (ধ্রুবক শূন্য *) font->তথ্য.directwrite;
  auto *global = get_directwrite_global ();
  যদি (unlikely (!global))
    ফেরত মিথ্যা;
  IDWriteFactory *dwriteFactory = global->dwriteFactory;

  IDWriteTextAnalyzer* analyzer;
  dwriteFactory->CreateTextAnalyzer (&analyzer);

  unsigned পূর্ণসংখ্যা scratch_আকার;
  hb_বাফার_t::scratch_বাফার_t *scratch = বাফার->get_scratch_বাফার (&scratch_আকার);
#define ALLOCATE_ARRAY(Type, name, len) \
  Type *name = (Type *) scratch; \
  করো { \
    unsigned পূর্ণসংখ্যা _consumed = DIV_CEIL ((len) * আকার (Type), আকার (*scratch)); \
    প্রতিজ্ঞা (_consumed <= scratch_আকার); \
    scratch += _consumed; \
    scratch_আকার -= _consumed; \
  } যখন (0)

#define utf16_সূচক() var1.u32

  ALLOCATE_ARRAY (wchar_t, textString, বাফার->len * 2);

  unsigned পূর্ণসংখ্যা chars_len = 0;
  জন্য (unsigned পূর্ণসংখ্যা i = 0; i < বাফার->len; i++)
  {
    hb_codepoint_t c = বাফার->info[i].codepoint;
    বাফার->info[i].utf16_সূচক () = chars_len;
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
  জন্য (unsigned পূর্ণসংখ্যা i = 0; i < বাফার->len; i++)
  {
    hb_codepoint_t c = বাফার->info[i].codepoint;
    unsigned পূর্ণসংখ্যা cluster = বাফার->info[i].cluster;
    log_clusters[chars_len++] = cluster;
    যদি (hb_in_range (c, 0x10000u, 0x10FFFFu))
      log_clusters[chars_len++] = cluster; /* Surrogates. */
  }

  DWRITE_READING_DIRECTION readingDirection;
  readingDirection = বাফার->props.direction ?
		     DWRITE_READING_DIRECTION_ডান_TO_বাম :
		     DWRITE_READING_DIRECTION_বাম_TO_ডান;

  /*
  * There's an internal 16-bit limit on some things inside the analyzer,
  * but we never attempt to shape a word longer than 64K characters
  * in a single gfxShapedWord, so we cannot exceed that limit.
  */
  uint32_t text_দৈর্ঘ্য_2 = chars_len;

  TextAnalysis analysis (textString, text_দৈর্ঘ্য_2, nullptr, readingDirection);
  TextAnalysis::Run *run_শীর্ষ;
  HRESULT hr;
  hr = analysis.GenerateResults (analyzer, &run_শীর্ষ);

#define FAIL(...) \
  HB_STMT_START { \
    DEBUG_বার্তা (DIRECTWRITE, nullptr, __VA_ARGS__); \
    ফেরত মিথ্যা; \
  } HB_STMT_END

  যদি (FAILED (hr))
    FAIL ("Analyzer failed to generate results.");

  uint32_t max_Glyph_গুনতি = 3 * text_দৈর্ঘ্য_2 / 2 + 16;
  uint32_t glyph_গুনতি_2;
  যৌক্তিক is_ডান_To_বাম = HB_DIRECTION_IS_BACKWARD (বাফার->props.direction);

  ধ্রুবক wchar_t localeName[20] = {0};
  যদি (বাফার->props.language)
    mbstowcs ((wchar_t*) localeName,
	      hb_language_to_string (বাফার->props.language), 20);

  /*
   * Set up features.
   */
  static_assert ((আকার (DWRITE_TYPOGRAPHIC_FEATURES) == আকার (hb_ms_features_t)), "");
  static_assert ((আকার (DWRITE_FONT_FEATURE) == আকার (hb_ms_feature_t)), "");
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
  যদি (unlikely (!clusterMap.resize_exact (text_দৈর্ঘ্য_2) ||
                !textProperties.resize_exact (text_দৈর্ঘ্য_2)))
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
			    মিথ্যা,
			    is_ডান_To_বাম,
			    &run_শীর্ষ->mScript,
			    localeName,
			    nullptr,
			    (ধ্রুবক DWRITE_TYPOGRAPHIC_FEATURES**) range_features.arrayZ,
			    range_char_counts.arrayZ,
			    range_features.দৈর্ঘ্য,
			    max_Glyph_গুনতি,
			    clusterMap.arrayZ,
			    textProperties.arrayZ,
			    glyphIndices.arrayZ,
			    glyphProperties.arrayZ,
			    &glyph_গুনতি_2);

  যদি (unlikely (hr == HRESULT_FROM_WIN32 (ERROR_INSUFFICIENT_বাফার)))
  {
    max_Glyph_গুনতি *= 2;

    যাও retry_getglyphs;
  }
  যদি (FAILED (hr))
    FAIL ("Analyzer failed to get glyphs.");

  hb_vector_t<দশমিক> glyphAdvances;
  hb_vector_t<DWRITE_GLYPH_OFFSET> glyphOffsets;
  যদি (unlikely (!glyphAdvances.resize_exact (max_Glyph_গুনতি) ||
                !glyphOffsets.resize_exact (max_Glyph_গুনতি)))
    FAIL ("Failed to allocate glyph positioning data.");


  unsigned font_Em_আকার = font->face->get_upem ();

  দশমিক x_mult = font->x_multf;
  দশমিক y_mult = font->y_multf;

  hr = analyzer->GetGlyphPlacements (textString,
				     clusterMap.arrayZ,
				     textProperties.arrayZ,
				     chars_len,
				     glyphIndices.arrayZ,
				     glyphProperties.arrayZ,
				     glyph_গুনতি_2,
				     fontFace,
				     font_Em_আকার,
				     মিথ্যা,
				     is_ডান_To_বাম,
				     &run_শীর্ষ->mScript,
				     localeName,
				     (ধ্রুবক DWRITE_TYPOGRAPHIC_FEATURES**) range_features.arrayZ,
				     range_char_counts.arrayZ,
				     range_features.দৈর্ঘ্য,
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

  জন্য (unsigned পূর্ণসংখ্যা i = 0; i < glyph_গুনতি_2; i++)
    vis_clusters[i] = (uint32_t) -1;
  জন্য (unsigned পূর্ণসংখ্যা i = 0; i < বাফার->len; i++)
  {
    unsigned পূর্ণসংখ্যা c = clusterMap[বাফার->info[i].utf16_সূচক ()];
    যদি (likely (c < glyph_গুনতি_2))
      vis_clusters[c] = hb_min (vis_clusters[c], বাফার->info[i].cluster);
  }
  জন্য (unsigned পূর্ণসংখ্যা i = 1; i < glyph_গুনতি_2; i++)
    যদি (vis_clusters[i] == (uint32_t) -1)
      vis_clusters[i] = vis_clusters[i - 1];
  জন্য (পূর্ণসংখ্যা i = (পূর্ণসংখ্যা) glyph_গুনতি_2 - 2; i >= 0; i--)
    যদি (vis_clusters[i] == (uint32_t) -1)
      vis_clusters[i] = vis_clusters[i + 1];

#undef utf16_সূচক

  যদি (unlikely (!বাফার->ensure (glyph_গুনতি_2)))
    FAIL ("Buffer in error");

#undef FAIL

  /* Set glyph infos */
  বাফার->len = 0;
  জন্য (unsigned পূর্ণসংখ্যা i = 0; i < glyph_গুনতি_2; i++)
  {
    hb_glyph_info_t *info = &বাফার->info[বাফার->len++];

    info->codepoint = glyphIndices[i];
    info->cluster = vis_clusters[i];

    /* The rest is crap.  Let's store position info there for now. */
    info->mask = (hb_mask_t) hb_clamp_to<int32_t> (glyphAdvances[i]);
    info->var1.i32 = hb_clamp_to<int32_t> (glyphOffsets[i].advanceOffset);
    info->var2.i32 = hb_clamp_to<int32_t> (glyphOffsets[i].ascenderOffset);
  }

  /* Set glyph positions */
  বাফার->clear_positions ();
  জন্য (unsigned পূর্ণসংখ্যা i = 0; i < glyph_গুনতি_2; i++)
  {
    hb_glyph_info_t *info = &বাফার->info[i];
    hb_glyph_position_t *pos = &বাফার->pos[i];

    /* TODO vertical */
    pos->x_advance = hb_clamp_to<hb_position_t> (round (x_mult * (int32_t) info->mask));
    দ্বিগুণ x_offset = is_ডান_To_বাম ? -(দ্বিগুণ) info->var1.i32 : info->var1.i32;
    pos->x_offset = hb_clamp_to<hb_position_t> (round ((দ্বিগুণ) x_mult * x_offset));
    pos->y_offset = hb_clamp_to<hb_position_t> (round (y_mult * info->var2.i32));
  }

  যদি (is_ডান_To_বাম) hb_বাফার_reverse (বাফার);

  বাফার->clear_glyph_flags ();
  বাফার->unsafe_to_break ();

  /* Wow, done! */
  ফেরত সত্য;
}


#endif
