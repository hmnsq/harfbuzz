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

#ضمن "hb.hh"

#ifdef HAVE_DIRECTWRITE

#ضمن "hb-shaper-impl.hh"

#ضمن "hb-directwrite.hh"

#ضمن "hb-ms-feature-ranges.hh"


/*
* shaper face data
*/

hb_directwrite_face_data_t *
_hb_directwrite_shaper_face_data_create (hb_face_t *face)
{
  hb_blob_t *blob = hb_face_reference_blob (face);

  hb_directwrite_face_data_t *data = (hb_directwrite_face_data_t *) dw_face_create (blob, face->index);

  hb_blob_destroy (blob);

  أرجع data;
}

فراغ
_hb_directwrite_shaper_face_data_destroy (hb_directwrite_face_data_t *data)
{
  ((IDWriteFontFace *) data)->Release ();
}


/*
 * shaper font data
 */

بنية hb_directwrite_font_data_t {};

hb_directwrite_font_data_t *
_hb_directwrite_shaper_font_data_create (hb_font_t *font)
{
  IDWriteFontFace *fontFace = (IDWriteFontFace *) (ثابت فراغ *) font->face->data.directwrite;

  /*
   * Set up variations.
   */
  IDWriteFontFace5 *fontFaceVariations = nullptr;
  {
    IDWriteFontFace5 *fontFace5;
    إذا (SUCCEEDED (fontFace->QueryInterface (__uuidof (IDWriteFontFace5), (فراغ **) &fontFace5)))
    {
      IDWriteFontResource *fontResource;
      إذا (SUCCEEDED (fontFace5->GetFontResource (&fontResource)))
      {
	hb_vector_t<DWRITE_FONT_AXIS_قدر> axis_اقدار;
	إذا (likely (axis_اقدار.resize_exact (font->عدد_coords)))
	{
	  لكل (unsigned صحيح i = 0; i < font->عدد_coords; i++)
	  {
	    hb_ot_var_axis_info_t info;
	    unsigned صحيح c = 1;
	    hb_ot_var_get_axis_infos (font->face, i, &c, &info);
	    axis_اقدار[i].axisTag = (DWRITE_FONT_AXIS_TAG) hb_uint32_swap (info.tag);
	    axis_اقدار[i].value = i < font->عدد_coords ?
				   hb_clamp (font->design_coords[i], info.min_قدر, info.max_قدر) :
				   info.default_قدر;
	  }

	  fontResource->CreateFontFace (DWRITE_FONT_SIMULATIONS::DWRITE_FONT_SIMULATIONS_NONE,
					axis_اقدار.arrayZ, axis_اقدار.length, &fontFaceVariations);
	}
	fontResource->Release ();
      }
      fontFace5->Release ();
    }
  }

  أرجع (hb_directwrite_font_data_t *) fontFaceVariations;
}

فراغ
_hb_directwrite_shaper_font_data_destroy (hb_directwrite_font_data_t *data)
{
  ((IDWriteFontFace *) (ثابت فراغ *) data)->Release ();
}


// Most of TextAnalysis is originally written by Bas Schouten for Mozilla project
// but now is relicensed to MIT for HarfBuzz use
صنف TextAnalysis : عام IDWriteTextAnalysisSource, عام IDWriteTextAnalysisSink
{
خاص:
  hb_reference_گنتی_t m_Ref_گنتی;
عام:
  IFACEMETHOD (QueryInterface) (IID ثابت& iid, OUT فراغ** ppObject)
  { أرجع S_OK; }
  IFACEMETHOD_ (ULONG, AddRef) ()
  {
    أرجع m_Ref_گنتی.inc () + 1;
  }
  IFACEMETHOD_ (ULONG, Release) ()
  {
    signed ref_گنتی = m_Ref_گنتی.dec () - 1;
    أكد (ref_گنتی >= 0);
    إذا (ref_گنتی)
      أرجع ref_گنتی;
    أرجع 0;
  }

  // A single contiguous run of characters containing the same analysis
  // results.
  بنية Run
  {
    uint32_t mTextStart;   // starting text position of this run
    uint32_t mTextLength;  // number of contiguous code units covered
    uint32_t mGlyphStart;  // starting glyph in the glyphs array
    uint32_t m_Glyph_گنتی;  // number of glyphs associated with this run
    // text
    DWRITE_SCRIPT_ANALYSIS mScript;
    uint8_t mBidiLevel;
    منطقي mIsSideways;

    منطقي ContainsTextPosition (uint32_t aTextPosition) ثابت
    {
      أرجع aTextPosition >= mTextStart &&
	     aTextPosition <  mTextStart + mTextLength;
    }

    Run *nextRun;
  };

عام:
  TextAnalysis (ثابت wchar_t* text, uint32_t textLength,
		ثابت wchar_t* localeName, DWRITE_READING_DIRECTION readingDirection)
	       : mTextLength (textLength), mText (text), mLocaleName (localeName),
		 mReadingDirection (readingDirection), mCurrentRun (nullptr)
  {
    m_Ref_گنتی.init ();
  }
  ~TextAnalysis ()
  {
    // free runs, except mRunHead which is part of the TextAnalysis object
    لكل (Run *run = mRunHead.nextRun; run;)
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
      (mReadingDirection == DWRITE_READING_DIRECTION_دایاں_TO_بایاں);
    mRunHead.nextRun = nullptr;
    mCurrentRun = &mRunHead;

    // Call each of the analyzers in sequence, recording their results.
    إذا (SUCCEEDED (hr = textAnalyzer->AnalyzeScript (هذا, 0, mTextLength, هذا)))
      *runHead = &mRunHead;

    أرجع hr;
  }

  // IDWriteTextAnalysisSource implementation

  IFACEMETHODIMP
  GetTextAtPosition (uint32_t textPosition,
		     OUT wchar_t ثابت** textString,
		     OUT uint32_t* textLength)
  {
    إذا (textPosition >= mTextLength)
    {
      // No text at this position, valid query though.
      *textString = nullptr;
      *textLength = 0;
    }
    وإلا
    {
      *textString = mText + textPosition;
      *textLength = mTextLength - textPosition;
    }
    أرجع S_OK;
  }

  IFACEMETHODIMP
  GetTextBeforePosition (uint32_t textPosition,
			 OUT wchar_t ثابت** textString,
			 OUT uint32_t* textLength)
  {
    إذا (textPosition == 0 || textPosition > mTextLength)
    {
      // Either there is no text before here (== 0), or this
      // is an invalid position. The query is considered valid though.
      *textString = nullptr;
      *textLength = 0;
    }
    وإلا
    {
      *textString = mText;
      *textLength = textPosition;
    }
    أرجع S_OK;
  }

  IFACEMETHODIMP_ (DWRITE_READING_DIRECTION)
  GetParagraphReadingDirection () { أرجع mReadingDirection; }

  IFACEMETHODIMP GetLocaleName (uint32_t textPosition, uint32_t* textLength,
				wchar_t ثابت** localeName)
  { أرجع S_OK; }

  IFACEMETHODIMP
  Get_نمبر_Substitution (uint32_t textPosition,
			 OUT uint32_t* textLength,
			 OUT IDWrite_نمبر_Substitution** نمبر_Substitution)
  {
    // We do not support number substitution.
    *نمبر_Substitution = nullptr;
    *textLength = mTextLength - textPosition;

    أرجع S_OK;
  }

  // IDWriteTextAnalysisSink implementation

  IFACEMETHODIMP
  SetScriptAnalysis (uint32_t textPosition, uint32_t textLength,
		     DWRITE_SCRIPT_ANALYSIS ثابت* scriptAnalysis)
  {
    SetCurrentRun (textPosition);
    إذا (unlikely (!SplitCurrentRun (textPosition)))
      أرجع E_OUTOFMEMORY;
    طالما (textLength > 0)
    {
      Run *run = FetchNextRun (&textLength);
      إذا (unlikely (!run))
        أرجع E_OUTOFMEMORY;
      run->mScript = *scriptAnalysis;
    }

    أرجع S_OK;
  }

  IFACEMETHODIMP
  Set_سطر_Breakpoints (uint32_t textPosition,
		      uint32_t textLength,
		      ثابت DWRITE_سطر_BREAKPOINT* سطر_Breakpoints)
  { أرجع S_OK; }

  IFACEMETHODIMP SetBidiLevel (uint32_t textPosition, uint32_t textLength,
			       uint8_t explicitLevel, uint8_t resolvedLevel)
  { أرجع S_OK; }

  IFACEMETHODIMP
  Set_نمبر_Substitution (uint32_t textPosition, uint32_t textLength,
			 IDWrite_نمبر_Substitution* نمبر_Substitution)
  { أرجع S_OK; }

محمي:
  Run *FetchNextRun (IN OUT uint32_t* textLength)
  {
    // Used by the sink setters, this returns a reference to the next run.
    // Position and length are adjusted to now point after the current run
    // being returned.

    Run *origRun = mCurrentRun;
    // Split the tail if needed (the length remaining is less than the
    // current run's size).
    إذا (*textLength < mCurrentRun->mTextLength)
    {
      إذا (unlikely (!SplitCurrentRun (mCurrentRun->mTextStart + *textLength)))
        أرجع nullptr;
    }
    وإلا
      // Just advance the current run.
      mCurrentRun = mCurrentRun->nextRun;
    *textLength -= origRun->mTextLength;

    // Return a reference to the run that was just current.
    أرجع origRun;
  }

  فراغ SetCurrentRun (uint32_t textPosition)
  {
    // Move the current run to the given position.
    // Since the analyzers generally return results in a forward manner,
    // this will usually just return early. If not, find the
    // corresponding run for the text position.

    إذا (mCurrentRun && mCurrentRun->ContainsTextPosition (textPosition))
      أرجع;

    لكل (Run *run = &mRunHead; run; run = run->nextRun)
      إذا (run->ContainsTextPosition (textPosition))
      {
	mCurrentRun = run;
	أرجع;
      }
    أكد (0); // We should always be able to find the text position in one of our runs
  }

  منطقي SplitCurrentRun (uint32_t splitPosition)
  {
    إذا (!mCurrentRun)
    {
      أكد (0); // SplitCurrentRun called without current run
      // Shouldn't be calling this when no current run is set!
      أرجع خطأ;
    }
    // Split the current run.
    إذا (splitPosition <= mCurrentRun->mTextStart)
    {
      // No need to split, already the start of a run
      // or before it. Usually the first.
      أرجع true;
    }
    Run *newRun = (Run *) hb_malloc (حجم (Run));
    إذا (unlikely (!newRun))
      أرجع خطأ;

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
    أرجع true;
  }

محمي:
  //داخلہ
  // (weak references are fine here, since this class is a transient
  //  stack-based helper that doesn't need to copy data)
  uint32_t mTextLength;
  ثابت wchar_t* mText;
  ثابت wchar_t* mLocaleName;
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
		       ثابت hb_feature_t *features,
		       unsigned صحيح        عدد_features)
{
  IDWriteFontFace *fontFace = (IDWriteFontFace *) (ثابت فراغ *) font->data.directwrite;
  auto *global = get_directwrite_global ();
  إذا (unlikely (!global))
    أرجع خطأ;
  IDWriteFactory *dwriteFactory = global->dwriteFactory;

  IDWriteTextAnalyzer* analyzer;
  dwriteFactory->CreateTextAnalyzer (&analyzer);

  unsigned صحيح scratch_size;
  hb_buffer_t::scratch_buffer_t *scratch = buffer->get_scratch_buffer (&scratch_size);
#define ALLOCATE_ARRAY(Type, name, len) \
  Type *name = (Type *) scratch; \
  افعل { \
    unsigned صحيح _consumed = DIV_CEIL ((len) * حجم (Type), حجم (*scratch)); \
    أكد (_consumed <= scratch_size); \
    scratch += _consumed; \
    scratch_size -= _consumed; \
  } طالما (0)

#define utf16_index() var1.u32

  ALLOCATE_ARRAY (wchar_t, textString, buffer->len * 2);

  unsigned صحيح chars_len = 0;
  لكل (unsigned صحيح i = 0; i < buffer->len; i++)
  {
    hb_codepoint_t c = buffer->info[i].codepoint;
    buffer->info[i].utf16_index () = chars_len;
    إذا (likely (c <= 0xFFFFu))
      textString[chars_len++] = c;
    وإلا إذا (unlikely (c > 0x10FFFFu))
      textString[chars_len++] = 0xFFFDu;
    وإلا
    {
      textString[chars_len++] = 0xD800u + ((c - 0x10000u) >> 10);
      textString[chars_len++] = 0xDC00u + ((c - 0x10000u) & ((1u << 10) - 1));
    }
  }

  ALLOCATE_ARRAY (WORD, log_clusters, chars_len);
#undef ALLOCATE_ARRAY
  /* Need log_clusters to assign features. */
  chars_len = 0;
  لكل (unsigned صحيح i = 0; i < buffer->len; i++)
  {
    hb_codepoint_t c = buffer->info[i].codepoint;
    unsigned صحيح cluster = buffer->info[i].cluster;
    log_clusters[chars_len++] = cluster;
    إذا (hb_in_range (c, 0x10000u, 0x10FFFFu))
      log_clusters[chars_len++] = cluster; /* Surrogates. */
  }

  DWRITE_READING_DIRECTION readingDirection;
  readingDirection = buffer->props.direction ?
		     DWRITE_READING_DIRECTION_دایاں_TO_بایاں :
		     DWRITE_READING_DIRECTION_بایاں_TO_دایاں;

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
    DEBUG_پیغام (DIRECTWRITE, nullptr, __VA_ARGS__); \
    أرجع خطأ; \
  } HB_STMT_END

  إذا (FAILED (hr))
    FAIL ("Analyzer failed to generate results.");

  uint32_t max_Glyph_گنتی = 3 * textLength / 2 + 16;
  uint32_t glyph_گنتی_2;
  منطقي is_دایاں_To_بایاں = HB_DIRECTION_IS_BACKWARD (buffer->props.direction);

  ثابت wchar_t localeName[20] = {0};
  إذا (buffer->props.language)
    mbstowcs ((wchar_t*) localeName,
	      hb_language_to_string (buffer->props.language), 20);

  /*
   * Set up features.
   */
  static_assert ((حجم (DWRITE_TYPOGRAPHIC_FEATURES) == حجم (hb_ms_features_t)), "");
  static_assert ((حجم (DWRITE_FONT_FEATURE) == حجم (hb_ms_feature_t)), "");
  hb_vector_t<hb_ms_features_t *> range_features;
  hb_vector_t<uint32_t> range_char_counts;

  // https://github.com/harfbuzz/harfbuzz/pull/5114
  // The data allocated by these two vectors are used by the above two, so they
  // should remain alive as long as the above two are.
  hb_vector_t<hb_ms_feature_t> feature_records;
  hb_vector_t<hb_ms_range_record_t> range_records;
  إذا (عدد_features)
  {
    إذا (hb_ms_setup_features (features, عدد_features, feature_records, range_records))
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
  إذا (unlikely (!clusterMap.resize_exact (textLength) ||
                !textProperties.resize_exact (textLength)))
    FAIL ("Failed to allocate shaping text data.");

  hb_vector_t<uint16_t> glyphIndices;
  hb_vector_t<DWRITE_SHAPING_GLYPH_PROPERTIES> glyphProperties;

retry_getglyphs:
  إذا (unlikely (!glyphIndices.resize_exact (max_Glyph_گنتی) ||
                !glyphProperties.resize_exact (max_Glyph_گنتی)))
    FAIL ("Failed to allocate shaping glyph data.");

  hr = analyzer->GetGlyphs (textString,
			    chars_len,
			    fontFace,
			    خطأ,
			    is_دایاں_To_بایاں,
			    &runHead->mScript,
			    localeName,
			    nullptr,
			    (ثابت DWRITE_TYPOGRAPHIC_FEATURES**) range_features.arrayZ,
			    range_char_counts.arrayZ,
			    range_features.length,
			    max_Glyph_گنتی,
			    clusterMap.arrayZ,
			    textProperties.arrayZ,
			    glyphIndices.arrayZ,
			    glyphProperties.arrayZ,
			    &glyph_گنتی_2);

  إذا (unlikely (hr == HRESULT_FROM_WIN32 (ERROR_INSUFFICIENT_BUFFER)))
  {
    max_Glyph_گنتی *= 2;

    اذهب retry_getglyphs;
  }
  إذا (FAILED (hr))
    FAIL ("Analyzer failed to get glyphs.");

  hb_vector_t<عائم> glyphAdvances;
  hb_vector_t<DWRITE_GLYPH_OFFSET> glyphOffsets;
  إذا (unlikely (!glyphAdvances.resize_exact (max_Glyph_گنتی) ||
                !glyphOffsets.resize_exact (max_Glyph_گنتی)))
    FAIL ("Failed to allocate glyph positioning data.");


  unsigned fontEmSize = font->face->get_upem ();

  عائم x_mult = font->x_multf;
  عائم y_mult = font->y_multf;

  hr = analyzer->GetGlyphPlacements (textString,
				     clusterMap.arrayZ,
				     textProperties.arrayZ,
				     chars_len,
				     glyphIndices.arrayZ,
				     glyphProperties.arrayZ,
				     glyph_گنتی_2,
				     fontFace,
				     fontEmSize,
				     خطأ,
				     is_دایاں_To_بایاں,
				     &runHead->mScript,
				     localeName,
				     (ثابت DWRITE_TYPOGRAPHIC_FEATURES**) range_features.arrayZ,
				     range_char_counts.arrayZ,
				     range_features.length,
				     glyphAdvances.arrayZ,
				     glyphOffsets.arrayZ);

  إذا (FAILED (hr))
    FAIL ("Analyzer failed to get glyph placements.");

  /* Ok, we've got everything we need, now compose output buffer,
   * very, *very*, carefully! */

  /* Calculate visual-clusters.  That's what we ship. */
  hb_vector_t<uint32_t> vis_clusters;
  إذا (unlikely (!vis_clusters.resize_exact (glyph_گنتی_2)))
    FAIL ("Failed to allocate visual clusters.");

  لكل (unsigned صحيح i = 0; i < glyph_گنتی_2; i++)
    vis_clusters[i] = (uint32_t) -1;
  لكل (unsigned صحيح i = 0; i < buffer->len; i++)
  {
    unsigned صحيح c = clusterMap[buffer->info[i].utf16_index ()];
    إذا (likely (c < glyph_گنتی_2))
      vis_clusters[c] = hb_min (vis_clusters[c], buffer->info[i].cluster);
  }
  لكل (unsigned صحيح i = 1; i < glyph_گنتی_2; i++)
    إذا (vis_clusters[i] == (uint32_t) -1)
      vis_clusters[i] = vis_clusters[i - 1];
  لكل (صحيح i = (صحيح) glyph_گنتی_2 - 2; i >= 0; i--)
    إذا (vis_clusters[i] == (uint32_t) -1)
      vis_clusters[i] = vis_clusters[i + 1];

#undef utf16_index

  إذا (unlikely (!buffer->ensure (glyph_گنتی_2)))
    FAIL ("Buffer in error");

#undef FAIL

  /* Set glyph infos */
  buffer->len = 0;
  لكل (unsigned صحيح i = 0; i < glyph_گنتی_2; i++)
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
  لكل (unsigned صحيح i = 0; i < glyph_گنتی_2; i++)
  {
    hb_glyph_info_t *info = &buffer->info[i];
    hb_glyph_position_t *pos = &buffer->pos[i];

    /* TODO vertical */
    pos->x_advance = hb_clamp_to<hb_position_t> (round (x_mult * (int32_t) info->mask));
    مزدوج x_offset = is_دایاں_To_بایاں ? -(مزدوج) info->var1.i32 : info->var1.i32;
    pos->x_offset = hb_clamp_to<hb_position_t> (round ((مزدوج) x_mult * x_offset));
    pos->y_offset = hb_clamp_to<hb_position_t> (round (y_mult * info->var2.i32));
  }

  إذا (is_دایاں_To_بایاں) hb_buffer_reverse (buffer);

  buffer->clear_glyph_flags ();
  buffer->unsafe_to_break ();

  /* Wow, done! */
  أرجع true;
}


#endif
