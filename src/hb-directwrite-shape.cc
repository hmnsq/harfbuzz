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

#અન્તર્ભાવ "hb.hh"

#ifdef HAVE_DIRECTWRITE

#અન્તર્ભાવ "hb-shaper-impl.hh"

#અન્તર્ભાવ "hb-directwrite.hh"

#અન્તર્ભાવ "hb-ms-feature-ranges.hh"


/*
* shaper face data
*/

hb_directwrite_face_data_t *
_hb_directwrite_shaper_face_data_create (hb_face_t *face)
{
  hb_blob_t *blob = hb_face_reference_blob (face);

  hb_directwrite_face_data_t *data = (hb_directwrite_face_data_t *) dw_face_create (blob, face->index);

  hb_blob_destroy (blob);

  પાછા data;
}

શૂન્ય
_hb_directwrite_shaper_face_data_destroy (hb_directwrite_face_data_t *data)
{
  ((IDWriteFontFace *) data)->Release ();
}


/*
 * shaper font data
 */

સંરચના hb_directwrite_font_data_t {};

hb_directwrite_font_data_t *
_hb_directwrite_shaper_font_data_create (hb_font_t *font)
{
  IDWriteFontFace *fontFace = (IDWriteFontFace *) (અચલ શૂન્ય *) font->face->data.directwrite;

  /*
   * Set up variations.
   */
  IDWriteFontFace5 *fontFaceVariations = nullptr;
  {
    IDWriteFontFace5 *fontFace5;
    જો (SUCCEEDED (fontFace->QueryInterface (__uuidof (IDWriteFontFace5), (શૂન્ય **) &fontFace5)))
    {
      IDWriteFontResource *fontResource;
      જો (SUCCEEDED (fontFace5->GetFontResource (&fontResource)))
      {
	hb_vector_t<DWRITE_FONT_AXIS_કિંમત> axis_કિંમતો;
	જો (likely (axis_કિંમતો.resize_exact (font->અંક_coords)))
	{
	  માટે (unsigned પૂર્ણાંક i = 0; i < font->અંક_coords; i++)
	  {
	    hb_ot_var_axis_info_t info;
	    unsigned પૂર્ણાંક c = 1;
	    hb_ot_var_get_axis_infos (font->face, i, &c, &info);
	    axis_કિંમતો[i].axisTag = (DWRITE_FONT_AXIS_TAG) hb_uint32_swap (info.tag);
	    axis_કિંમતો[i].value = i < font->અંક_coords ?
				   hb_clamp (font->design_coords[i], info.min_કિંમત, info.max_કિંમત) :
				   info.default_કિંમત;
	  }

	  fontResource->CreateFontFace (DWRITE_FONT_SIMULATIONS::DWRITE_FONT_SIMULATIONS_NONE,
					axis_કિંમતો.arrayZ, axis_કિંમતો.length, &fontFaceVariations);
	}
	fontResource->Release ();
      }
      fontFace5->Release ();
    }
  }

  પાછા (hb_directwrite_font_data_t *) fontFaceVariations;
}

શૂન્ય
_hb_directwrite_shaper_font_data_destroy (hb_directwrite_font_data_t *data)
{
  ((IDWriteFontFace *) (અચલ શૂન્ય *) data)->Release ();
}


// Most of TextAnalysis is originally written by Bas Schouten for Mozilla project
// but now is relicensed to MIT for HarfBuzz use
વર્ગ TextAnalysis : જાહેર IDWriteTextAnalysisSource, જાહેર IDWriteTextAnalysisSink
{
ખાનગી:
  hb_reference_ગણતરી_t m_Ref_ગણતરી;
જાહેર:
  IFACEMETHOD (QueryInterface) (IID અચલ& iid, OUT શૂન્ય** ppObject)
  { પાછા S_OK; }
  IFACEMETHOD_ (ULONG, AddRef) ()
  {
    પાછા m_Ref_ગણતરી.inc () + 1;
  }
  IFACEMETHOD_ (ULONG, Release) ()
  {
    signed ref_ગણતરી = m_Ref_ગણતરી.dec () - 1;
    પ્રતિજ્ઞા (ref_ગણતરી >= 0);
    જો (ref_ગણતરી)
      પાછા ref_ગણતરી;
    પાછા 0;
  }

  // A single contiguous run of characters containing the same analysis
  // results.
  સંરચના Run
  {
    uint32_t mTextStart;   // starting text position of this run
    uint32_t mTextLength;  // number of contiguous code units covered
    uint32_t mGlyphStart;  // starting glyph in the glyphs array
    uint32_t m_Glyph_ગણતરી;  // number of glyphs associated with this run
    // text
    DWRITE_SCRIPT_ANALYSIS mScript;
    uint8_t mBidiLevel;
    તાર્કિક mIsSideways;

    તાર્કિક ContainsTextPosition (uint32_t aTextPosition) અચલ
    {
      પાછા aTextPosition >= mTextStart &&
	     aTextPosition <  mTextStart + mTextLength;
    }

    Run *nextRun;
  };

જાહેર:
  TextAnalysis (અચલ wchar_t* text, uint32_t textLength,
		અચલ wchar_t* localeName, DWRITE_READING_DIRECTION readingDirection)
	       : mTextLength (textLength), mText (text), mLocaleName (localeName),
		 mReadingDirection (readingDirection), mCurrentRun (nullptr)
  {
    m_Ref_ગણતરી.init ();
  }
  ~TextAnalysis ()
  {
    // free runs, except mRunHead which is part of the TextAnalysis object
    માટે (Run *run = mRunHead.nextRun; run;)
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
      (mReadingDirection == DWRITE_READING_DIRECTION_જમણું_TO_ડાબું);
    mRunHead.nextRun = nullptr;
    mCurrentRun = &mRunHead;

    // Call each of the analyzers in sequence, recording their results.
    જો (SUCCEEDED (hr = textAnalyzer->AnalyzeScript (આ, 0, mTextLength, આ)))
      *runHead = &mRunHead;

    પાછા hr;
  }

  // IDWriteTextAnalysisSource implementation

  IFACEMETHODIMP
  GetTextAtPosition (uint32_t textPosition,
		     OUT wchar_t અચલ** textString,
		     OUT uint32_t* textLength)
  {
    જો (textPosition >= mTextLength)
    {
      // No text at this position, valid query though.
      *textString = nullptr;
      *textLength = 0;
    }
    નહીંતર
    {
      *textString = mText + textPosition;
      *textLength = mTextLength - textPosition;
    }
    પાછા S_OK;
  }

  IFACEMETHODIMP
  GetTextBeforePosition (uint32_t textPosition,
			 OUT wchar_t અચલ** textString,
			 OUT uint32_t* textLength)
  {
    જો (textPosition == 0 || textPosition > mTextLength)
    {
      // Either there is no text before here (== 0), or this
      // is an invalid position. The query is considered valid though.
      *textString = nullptr;
      *textLength = 0;
    }
    નહીંતર
    {
      *textString = mText;
      *textLength = textPosition;
    }
    પાછા S_OK;
  }

  IFACEMETHODIMP_ (DWRITE_READING_DIRECTION)
  GetParagraphReadingDirection () { પાછા mReadingDirection; }

  IFACEMETHODIMP GetLocaleName (uint32_t textPosition, uint32_t* textLength,
				wchar_t અચલ** localeName)
  { પાછા S_OK; }

  IFACEMETHODIMP
  Get_સંખ્યા_Substitution (uint32_t textPosition,
			 OUT uint32_t* textLength,
			 OUT IDWrite_સંખ્યા_Substitution** સંખ્યા_Substitution)
  {
    // We do not support number substitution.
    *સંખ્યા_Substitution = nullptr;
    *textLength = mTextLength - textPosition;

    પાછા S_OK;
  }

  // IDWriteTextAnalysisSink implementation

  IFACEMETHODIMP
  SetScriptAnalysis (uint32_t textPosition, uint32_t textLength,
		     DWRITE_SCRIPT_ANALYSIS અચલ* scriptAnalysis)
  {
    SetCurrentRun (textPosition);
    જો (unlikely (!SplitCurrentRun (textPosition)))
      પાછા E_OUTOFMEMORY;
    જ્યારે (textLength > 0)
    {
      Run *run = FetchNextRun (&textLength);
      જો (unlikely (!run))
        પાછા E_OUTOFMEMORY;
      run->mScript = *scriptAnalysis;
    }

    પાછા S_OK;
  }

  IFACEMETHODIMP
  Set_લીટી_Breakpoints (uint32_t textPosition,
		      uint32_t textLength,
		      અચલ DWRITE_લીટી_BREAKPOINT* લીટી_Breakpoints)
  { પાછા S_OK; }

  IFACEMETHODIMP SetBidiLevel (uint32_t textPosition, uint32_t textLength,
			       uint8_t explicitLevel, uint8_t resolvedLevel)
  { પાછા S_OK; }

  IFACEMETHODIMP
  Set_સંખ્યા_Substitution (uint32_t textPosition, uint32_t textLength,
			 IDWrite_સંખ્યા_Substitution* સંખ્યા_Substitution)
  { પાછા S_OK; }

સુરક્ષિત:
  Run *FetchNextRun (IN OUT uint32_t* textLength)
  {
    // Used by the sink setters, this returns a reference to the next run.
    // Position and length are adjusted to now point after the current run
    // being returned.

    Run *origRun = mCurrentRun;
    // Split the tail if needed (the length remaining is less than the
    // current run's size).
    જો (*textLength < mCurrentRun->mTextLength)
    {
      જો (unlikely (!SplitCurrentRun (mCurrentRun->mTextStart + *textLength)))
        પાછા nullptr;
    }
    નહીંતર
      // Just advance the current run.
      mCurrentRun = mCurrentRun->nextRun;
    *textLength -= origRun->mTextLength;

    // Return a reference to the run that was just current.
    પાછા origRun;
  }

  શૂન્ય SetCurrentRun (uint32_t textPosition)
  {
    // Move the current run to the given position.
    // Since the analyzers generally return results in a forward manner,
    // this will usually just return early. If not, find the
    // corresponding run for the text position.

    જો (mCurrentRun && mCurrentRun->ContainsTextPosition (textPosition))
      પાછા;

    માટે (Run *run = &mRunHead; run; run = run->nextRun)
      જો (run->ContainsTextPosition (textPosition))
      {
	mCurrentRun = run;
	પાછા;
      }
    પ્રતિજ્ઞા (0); // We should always be able to find the text position in one of our runs
  }

  તાર્કિક SplitCurrentRun (uint32_t splitPosition)
  {
    જો (!mCurrentRun)
    {
      પ્રતિજ્ઞા (0); // SplitCurrentRun called without current run
      // Shouldn't be calling this when no current run is set!
      પાછા ખોટું;
    }
    // Split the current run.
    જો (splitPosition <= mCurrentRun->mTextStart)
    {
      // No need to split, already the start of a run
      // or before it. Usually the first.
      પાછા સાચું;
    }
    Run *newRun = (Run *) hb_malloc (કદ (Run));
    જો (unlikely (!newRun))
      પાછા ખોટું;

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
    પાછા સાચું;
  }

સુરક્ષિત:
  //આગત
  // (weak references are fine here, since this class is a transient
  //  stack-based helper that doesn't need to copy data)
  uint32_t mTextLength;
  અચલ wchar_t* mText;
  અચલ wchar_t* mLocaleName;
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
		       અચલ hb_feature_t *features,
		       unsigned પૂર્ણાંક        અંક_features)
{
  IDWriteFontFace *fontFace = (IDWriteFontFace *) (અચલ શૂન્ય *) font->data.directwrite;
  auto *global = get_directwrite_global ();
  જો (unlikely (!global))
    પાછા ખોટું;
  IDWriteFactory *dwriteFactory = global->dwriteFactory;

  IDWriteTextAnalyzer* analyzer;
  dwriteFactory->CreateTextAnalyzer (&analyzer);

  unsigned પૂર્ણાંક scratch_size;
  hb_buffer_t::scratch_buffer_t *scratch = buffer->get_scratch_buffer (&scratch_size);
#define ALLOCATE_ARRAY(Type, name, len) \
  Type *name = (Type *) scratch; \
  કરો { \
    unsigned પૂર્ણાંક _consumed = DIV_CEIL ((len) * કદ (Type), કદ (*scratch)); \
    પ્રતિજ્ઞા (_consumed <= scratch_size); \
    scratch += _consumed; \
    scratch_size -= _consumed; \
  } જ્યારે (0)

#define utf16_index() var1.u32

  ALLOCATE_ARRAY (wchar_t, textString, buffer->len * 2);

  unsigned પૂર્ણાંક chars_len = 0;
  માટે (unsigned પૂર્ણાંક i = 0; i < buffer->len; i++)
  {
    hb_codepoint_t c = buffer->info[i].codepoint;
    buffer->info[i].utf16_index () = chars_len;
    જો (likely (c <= 0xFFFFu))
      textString[chars_len++] = c;
    નહીંતર જો (unlikely (c > 0x10FFFFu))
      textString[chars_len++] = 0xFFFDu;
    નહીંતર
    {
      textString[chars_len++] = 0xD800u + ((c - 0x10000u) >> 10);
      textString[chars_len++] = 0xDC00u + ((c - 0x10000u) & ((1u << 10) - 1));
    }
  }

  ALLOCATE_ARRAY (WORD, log_clusters, chars_len);
#undef ALLOCATE_ARRAY
  /* Need log_clusters to assign features. */
  chars_len = 0;
  માટે (unsigned પૂર્ણાંક i = 0; i < buffer->len; i++)
  {
    hb_codepoint_t c = buffer->info[i].codepoint;
    unsigned પૂર્ણાંક cluster = buffer->info[i].cluster;
    log_clusters[chars_len++] = cluster;
    જો (hb_in_range (c, 0x10000u, 0x10FFFFu))
      log_clusters[chars_len++] = cluster; /* Surrogates. */
  }

  DWRITE_READING_DIRECTION readingDirection;
  readingDirection = buffer->props.direction ?
		     DWRITE_READING_DIRECTION_જમણું_TO_ડાબું :
		     DWRITE_READING_DIRECTION_ડાબું_TO_જમણું;

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
    DEBUG_સંદેશ (DIRECTWRITE, nullptr, __VA_ARGS__); \
    પાછા ખોટું; \
  } HB_STMT_END

  જો (FAILED (hr))
    FAIL ("Analyzer failed to generate results.");

  uint32_t max_Glyph_ગણતરી = 3 * textLength / 2 + 16;
  uint32_t glyph_ગણતરી_2;
  તાર્કિક is_જમણું_To_ડાબું = HB_DIRECTION_IS_BACKWARD (buffer->props.direction);

  અચલ wchar_t localeName[20] = {0};
  જો (buffer->props.language)
    mbstowcs ((wchar_t*) localeName,
	      hb_language_to_string (buffer->props.language), 20);

  /*
   * Set up features.
   */
  static_assert ((કદ (DWRITE_TYPOGRAPHIC_FEATURES) == કદ (hb_ms_features_t)), "");
  static_assert ((કદ (DWRITE_FONT_FEATURE) == કદ (hb_ms_feature_t)), "");
  hb_vector_t<hb_ms_features_t *> range_features;
  hb_vector_t<uint32_t> range_char_counts;

  // https://github.com/harfbuzz/harfbuzz/pull/5114
  // The data allocated by these two vectors are used by the above two, so they
  // should remain alive as long as the above two are.
  hb_vector_t<hb_ms_feature_t> feature_records;
  hb_vector_t<hb_ms_range_record_t> range_records;
  જો (અંક_features)
  {
    જો (hb_ms_setup_features (features, અંક_features, feature_records, range_records))
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
  જો (unlikely (!clusterMap.resize_exact (textLength) ||
                !textProperties.resize_exact (textLength)))
    FAIL ("Failed to allocate shaping text data.");

  hb_vector_t<uint16_t> glyphIndices;
  hb_vector_t<DWRITE_SHAPING_GLYPH_PROPERTIES> glyphProperties;

retry_getglyphs:
  જો (unlikely (!glyphIndices.resize_exact (max_Glyph_ગણતરી) ||
                !glyphProperties.resize_exact (max_Glyph_ગણતરી)))
    FAIL ("Failed to allocate shaping glyph data.");

  hr = analyzer->GetGlyphs (textString,
			    chars_len,
			    fontFace,
			    ખોટું,
			    is_જમણું_To_ડાબું,
			    &runHead->mScript,
			    localeName,
			    nullptr,
			    (અચલ DWRITE_TYPOGRAPHIC_FEATURES**) range_features.arrayZ,
			    range_char_counts.arrayZ,
			    range_features.length,
			    max_Glyph_ગણતરી,
			    clusterMap.arrayZ,
			    textProperties.arrayZ,
			    glyphIndices.arrayZ,
			    glyphProperties.arrayZ,
			    &glyph_ગણતરી_2);

  જો (unlikely (hr == HRESULT_FROM_WIN32 (ERROR_INSUFFICIENT_BUFFER)))
  {
    max_Glyph_ગણતરી *= 2;

    જાઓ retry_getglyphs;
  }
  જો (FAILED (hr))
    FAIL ("Analyzer failed to get glyphs.");

  hb_vector_t<દશાંશ> glyphAdvances;
  hb_vector_t<DWRITE_GLYPH_OFFSET> glyphOffsets;
  જો (unlikely (!glyphAdvances.resize_exact (max_Glyph_ગણતરી) ||
                !glyphOffsets.resize_exact (max_Glyph_ગણતરી)))
    FAIL ("Failed to allocate glyph positioning data.");


  unsigned fontEmSize = font->face->get_upem ();

  દશાંશ x_mult = font->x_multf;
  દશાંશ y_mult = font->y_multf;

  hr = analyzer->GetGlyphPlacements (textString,
				     clusterMap.arrayZ,
				     textProperties.arrayZ,
				     chars_len,
				     glyphIndices.arrayZ,
				     glyphProperties.arrayZ,
				     glyph_ગણતરી_2,
				     fontFace,
				     fontEmSize,
				     ખોટું,
				     is_જમણું_To_ડાબું,
				     &runHead->mScript,
				     localeName,
				     (અચલ DWRITE_TYPOGRAPHIC_FEATURES**) range_features.arrayZ,
				     range_char_counts.arrayZ,
				     range_features.length,
				     glyphAdvances.arrayZ,
				     glyphOffsets.arrayZ);

  જો (FAILED (hr))
    FAIL ("Analyzer failed to get glyph placements.");

  /* Ok, we've got everything we need, now compose output buffer,
   * very, *very*, carefully! */

  /* Calculate visual-clusters.  That's what we ship. */
  hb_vector_t<uint32_t> vis_clusters;
  જો (unlikely (!vis_clusters.resize_exact (glyph_ગણતરી_2)))
    FAIL ("Failed to allocate visual clusters.");

  માટે (unsigned પૂર્ણાંક i = 0; i < glyph_ગણતરી_2; i++)
    vis_clusters[i] = (uint32_t) -1;
  માટે (unsigned પૂર્ણાંક i = 0; i < buffer->len; i++)
  {
    unsigned પૂર્ણાંક c = clusterMap[buffer->info[i].utf16_index ()];
    જો (likely (c < glyph_ગણતરી_2))
      vis_clusters[c] = hb_min (vis_clusters[c], buffer->info[i].cluster);
  }
  માટે (unsigned પૂર્ણાંક i = 1; i < glyph_ગણતરી_2; i++)
    જો (vis_clusters[i] == (uint32_t) -1)
      vis_clusters[i] = vis_clusters[i - 1];
  માટે (પૂર્ણાંક i = (પૂર્ણાંક) glyph_ગણતરી_2 - 2; i >= 0; i--)
    જો (vis_clusters[i] == (uint32_t) -1)
      vis_clusters[i] = vis_clusters[i + 1];

#undef utf16_index

  જો (unlikely (!buffer->ensure (glyph_ગણતરી_2)))
    FAIL ("Buffer in error");

#undef FAIL

  /* Set glyph infos */
  buffer->len = 0;
  માટે (unsigned પૂર્ણાંક i = 0; i < glyph_ગણતરી_2; i++)
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
  માટે (unsigned પૂર્ણાંક i = 0; i < glyph_ગણતરી_2; i++)
  {
    hb_glyph_info_t *info = &buffer->info[i];
    hb_glyph_position_t *pos = &buffer->pos[i];

    /* TODO vertical */
    pos->x_advance = hb_clamp_to<hb_position_t> (round (x_mult * (int32_t) info->mask));
    દ્વિગુણ x_offset = is_જમણું_To_ડાબું ? -(દ્વિગુણ) info->var1.i32 : info->var1.i32;
    pos->x_offset = hb_clamp_to<hb_position_t> (round ((દ્વિગુણ) x_mult * x_offset));
    pos->y_offset = hb_clamp_to<hb_position_t> (round (y_mult * info->var2.i32));
  }

  જો (is_જમણું_To_ડાબું) hb_buffer_reverse (buffer);

  buffer->clear_glyph_flags ();
  buffer->unsafe_to_break ();

  /* Wow, done! */
  પાછા સાચું;
}


#endif
