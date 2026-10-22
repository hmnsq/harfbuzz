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

#include "hb.hh"

#ifdef HAVE_DIRECTWRITE

#include "hb-shaper-impl.hh"

#include "hb-directwrite.hh"

#include "hb-ms-feature-ranges.hh"


/*
* shaper face data
*/

hb_directwrite_face_data_t *
_hb_directwrite_shaper_face_data_create (hb_face_t *face)
{
  hb_blob_t *blob = hb_face_reference_blob (face);

  hb_directwrite_face_data_t *data = (hb_directwrite_face_data_t *) dw_face_create (blob, face->index);

  hb_blob_destroy (blob);

  返回 data;
}

void
_hb_directwrite_shaper_face_data_destroy (hb_directwrite_face_data_t *data)
{
  ((IDWriteFontFace *) data)->Release ();
}


/*
 * shaper font data
 */

struct hb_directwrite_font_data_t {};

hb_directwrite_font_data_t *
_hb_directwrite_shaper_font_data_create (hb_font_t *font)
{
  IDWriteFontFace *fontFace = (IDWriteFontFace *) (const void *) font->face->data.directwrite;

  /*
   * Set up variations.
   */
  IDWriteFontFace5 *fontFaceVariations = nullptr;
  {
    IDWriteFontFace5 *fontFace5;
    如果 (SUCCEEDED (fontFace->QueryInterface (__uuidof (IDWriteFontFace5), (void **) &fontFace5)))
    {
      IDWriteFontResource *fontResource;
      如果 (SUCCEEDED (fontFace5->GetFontResource (&fontResource)))
      {
	hb_vector_t<DWRITE_FONT_AXIS_VALUE> axis_values;
	如果 (likely (axis_values.resize_exact (font->num_coords)))
	{
	  为 (unsigned 整数 i = 0; i < font->num_coords; i++)
	  {
	    hb_ot_var_axis_info_t info;
	    unsigned 整数 c = 1;
	    hb_ot_var_get_axis_infos (font->face, i, &c, &info);
	    axis_values[i].axisTag = (DWRITE_FONT_AXIS_TAG) hb_uint32_swap (info.tag);
	    axis_values[i].value = i < font->num_coords ?
				   hb_clamp (font->design_coords[i], info.min_value, info.max_value) :
				   info.default_value;
	  }

	  fontResource->CreateFontFace (DWRITE_FONT_SIMULATIONS::DWRITE_FONT_SIMULATIONS_NONE,
					axis_values.arrayZ, axis_values.length, &fontFaceVariations);
	}
	fontResource->Release ();
      }
      fontFace5->Release ();
    }
  }

  返回 (hb_directwrite_font_data_t *) fontFaceVariations;
}

void
_hb_directwrite_shaper_font_data_destroy (hb_directwrite_font_data_t *data)
{
  ((IDWriteFontFace *) (const void *) data)->Release ();
}


// Most of TextAnalysis is originally written by Bas Schouten for Mozilla project
// but now is relicensed to MIT for HarfBuzz use
class TextAnalysis : public IDWriteTextAnalysisSource, public IDWriteTextAnalysisSink
{
private:
  hb_reference_count_t mRefCount;
public:
  IFACEMETHOD (QueryInterface) (IID const& iid, OUT void** ppObject)
  { 返回 S_OK; }
  IFACEMETHOD_ (ULONG, AddRef) ()
  {
    返回 mRefCount.inc () + 1;
  }
  IFACEMETHOD_ (ULONG, Release) ()
  {
    signed refCount = mRefCount.dec () - 1;
    assert (refCount >= 0);
    如果 (refCount)
      返回 refCount;
    返回 0;
  }

  // A single contiguous run of characters containing the same analysis
  // results.
  struct Run
  {
    uint32_t mTextStart;   // starting text position of this run
    uint32_t mTextLength;  // number of contiguous code units covered
    uint32_t mGlyphStart;  // starting glyph in the glyphs array
    uint32_t mGlyphCount;  // number of glyphs associated with this run
    // text
    DWRITE_SCRIPT_ANALYSIS mScript;
    uint8_t mBidiLevel;
    bool mIsSideways;

    bool ContainsTextPosition (uint32_t aTextPosition) const
    {
      返回 aTextPosition >= mTextStart &&
	     aTextPosition <  mTextStart + mTextLength;
    }

    Run *nextRun;
  };

public:
  TextAnalysis (const wchar_t* text, uint32_t textLength,
		const wchar_t* localeName, DWRITE_READING_DIRECTION readingDirection)
	       : mTextLength (textLength), mText (text), mLocaleName (localeName),
		 mReadingDirection (readingDirection), mCurrentRun (nullptr)
  {
    mRefCount.init ();
  }
  ~TextAnalysis ()
  {
    // free runs, except mRunHead which is part of the TextAnalysis object
    为 (Run *run = mRunHead.nextRun; run;)
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
      (mReadingDirection == DWRITE_READING_DIRECTION_RIGHT_TO_LEFT);
    mRunHead.nextRun = nullptr;
    mCurrentRun = &mRunHead;

    // Call each of the analyzers in sequence, recording their results.
    如果 (SUCCEEDED (hr = textAnalyzer->AnalyzeScript (this, 0, mTextLength, this)))
      *runHead = &mRunHead;

    返回 hr;
  }

  // IDWriteTextAnalysisSource implementation

  IFACEMETHODIMP
  GetTextAtPosition (uint32_t textPosition,
		     OUT wchar_t const** textString,
		     OUT uint32_t* textLength)
  {
    如果 (textPosition >= mTextLength)
    {
      // No text at this position, valid query though.
      *textString = nullptr;
      *textLength = 0;
    }
    否则
    {
      *textString = mText + textPosition;
      *textLength = mTextLength - textPosition;
    }
    返回 S_OK;
  }

  IFACEMETHODIMP
  GetTextBeforePosition (uint32_t textPosition,
			 OUT wchar_t const** textString,
			 OUT uint32_t* textLength)
  {
    如果 (textPosition == 0 || textPosition > mTextLength)
    {
      // Either there is no text before here (== 0), or this
      // is an invalid position. The query is considered valid though.
      *textString = nullptr;
      *textLength = 0;
    }
    否则
    {
      *textString = mText;
      *textLength = textPosition;
    }
    返回 S_OK;
  }

  IFACEMETHODIMP_ (DWRITE_READING_DIRECTION)
  GetParagraphReadingDirection () { 返回 mReadingDirection; }

  IFACEMETHODIMP GetLocaleName (uint32_t textPosition, uint32_t* textLength,
				wchar_t const** localeName)
  { 返回 S_OK; }

  IFACEMETHODIMP
  GetNumberSubstitution (uint32_t textPosition,
			 OUT uint32_t* textLength,
			 OUT IDWriteNumberSubstitution** numberSubstitution)
  {
    // We do not support number substitution.
    *numberSubstitution = nullptr;
    *textLength = mTextLength - textPosition;

    返回 S_OK;
  }

  // IDWriteTextAnalysisSink implementation

  IFACEMETHODIMP
  SetScriptAnalysis (uint32_t textPosition, uint32_t textLength,
		     DWRITE_SCRIPT_ANALYSIS const* scriptAnalysis)
  {
    SetCurrentRun (textPosition);
    如果 (unlikely (!SplitCurrentRun (textPosition)))
      返回 E_OUTOFMEMORY;
    当 (textLength > 0)
    {
      Run *run = FetchNextRun (&textLength);
      如果 (unlikely (!run))
        返回 E_OUTOFMEMORY;
      run->mScript = *scriptAnalysis;
    }

    返回 S_OK;
  }

  IFACEMETHODIMP
  SetLineBreakpoints (uint32_t textPosition,
		      uint32_t textLength,
		      const DWRITE_LINE_BREAKPOINT* lineBreakpoints)
  { 返回 S_OK; }

  IFACEMETHODIMP SetBidiLevel (uint32_t textPosition, uint32_t textLength,
			       uint8_t explicitLevel, uint8_t resolvedLevel)
  { 返回 S_OK; }

  IFACEMETHODIMP
  SetNumberSubstitution (uint32_t textPosition, uint32_t textLength,
			 IDWriteNumberSubstitution* numberSubstitution)
  { 返回 S_OK; }

protected:
  Run *FetchNextRun (IN OUT uint32_t* textLength)
  {
    // Used by the sink setters, this returns a reference to the next run.
    // Position and length are adjusted to now point after the current run
    // being returned.

    Run *origRun = mCurrentRun;
    // Split the tail if needed (the length remaining is less than the
    // current run's size).
    如果 (*textLength < mCurrentRun->mTextLength)
    {
      如果 (unlikely (!SplitCurrentRun (mCurrentRun->mTextStart + *textLength)))
        返回 nullptr;
    }
    否则
      // Just advance the current run.
      mCurrentRun = mCurrentRun->nextRun;
    *textLength -= origRun->mTextLength;

    // Return a reference to the run that was just current.
    返回 origRun;
  }

  void SetCurrentRun (uint32_t textPosition)
  {
    // Move the current run to the given position.
    // Since the analyzers generally return results in a forward manner,
    // this will usually just return early. If not, find the
    // corresponding run for the text position.

    如果 (mCurrentRun && mCurrentRun->ContainsTextPosition (textPosition))
      返回;

    为 (Run *run = &mRunHead; run; run = run->nextRun)
      如果 (run->ContainsTextPosition (textPosition))
      {
	mCurrentRun = run;
	返回;
      }
    assert (0); // We should always be able to find the text position in one of our runs
  }

  bool SplitCurrentRun (uint32_t splitPosition)
  {
    如果 (!mCurrentRun)
    {
      assert (0); // SplitCurrentRun called without current run
      // Shouldn't be calling this when no current run is set!
      返回 false;
    }
    // Split the current run.
    如果 (splitPosition <= mCurrentRun->mTextStart)
    {
      // No need to split, already the start of a run
      // or before it. Usually the first.
      返回 true;
    }
    Run *newRun = (Run *) hb_malloc (sizeof (Run));
    如果 (unlikely (!newRun))
      返回 false;

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
    返回 true;
  }

protected:
  // Input
  // (weak references are fine here, since this class is a transient
  //  stack-based helper that doesn't need to copy data)
  uint32_t mTextLength;
  const wchar_t* mText;
  const wchar_t* mLocaleName;
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
		       const hb_feature_t *features,
		       unsigned 整数        num_features)
{
  IDWriteFontFace *fontFace = (IDWriteFontFace *) (const void *) font->data.directwrite;
  auto *global = get_directwrite_global ();
  如果 (unlikely (!global))
    返回 false;
  IDWriteFactory *dwriteFactory = global->dwriteFactory;

  IDWriteTextAnalyzer* analyzer;
  dwriteFactory->CreateTextAnalyzer (&analyzer);

  unsigned 整数 scratch_size;
  hb_buffer_t::scratch_buffer_t *scratch = buffer->get_scratch_buffer (&scratch_size);
#define ALLOCATE_ARRAY(Type, name, len) \
  Type *name = (Type *) scratch; \
  do { \
    unsigned 整数 _consumed = DIV_CEIL ((len) * sizeof (Type), sizeof (*scratch)); \
    assert (_consumed <= scratch_size); \
    scratch += _consumed; \
    scratch_size -= _consumed; \
  } 当 (0)

#define utf16_index() var1.u32

  ALLOCATE_ARRAY (wchar_t, textString, buffer->len * 2);

  unsigned 整数 chars_len = 0;
  为 (unsigned 整数 i = 0; i < buffer->len; i++)
  {
    hb_codepoint_t c = buffer->info[i].codepoint;
    buffer->info[i].utf16_index () = chars_len;
    如果 (likely (c <= 0xFFFFu))
      textString[chars_len++] = c;
    否则 如果 (unlikely (c > 0x10FFFFu))
      textString[chars_len++] = 0xFFFDu;
    否则
    {
      textString[chars_len++] = 0xD800u + ((c - 0x10000u) >> 10);
      textString[chars_len++] = 0xDC00u + ((c - 0x10000u) & ((1u << 10) - 1));
    }
  }

  ALLOCATE_ARRAY (WORD, log_clusters, chars_len);
#undef ALLOCATE_ARRAY
  /* Need log_clusters to assign features. */
  chars_len = 0;
  为 (unsigned 整数 i = 0; i < buffer->len; i++)
  {
    hb_codepoint_t c = buffer->info[i].codepoint;
    unsigned 整数 cluster = buffer->info[i].cluster;
    log_clusters[chars_len++] = cluster;
    如果 (hb_in_range (c, 0x10000u, 0x10FFFFu))
      log_clusters[chars_len++] = cluster; /* Surrogates. */
  }

  DWRITE_READING_DIRECTION readingDirection;
  readingDirection = buffer->props.direction ?
		     DWRITE_READING_DIRECTION_RIGHT_TO_LEFT :
		     DWRITE_READING_DIRECTION_LEFT_TO_RIGHT;

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
    DEBUG_MSG (DIRECTWRITE, nullptr, __VA_ARGS__); \
    返回 false; \
  } HB_STMT_END

  如果 (FAILED (hr))
    FAIL ("Analyzer failed to generate results.");

  uint32_t maxGlyphCount = 3 * textLength / 2 + 16;
  uint32_t glyphCount;
  bool isRightToLeft = HB_DIRECTION_IS_BACKWARD (buffer->props.direction);

  const wchar_t localeName[20] = {0};
  如果 (buffer->props.language)
    mbstowcs ((wchar_t*) localeName,
	      hb_language_to_string (buffer->props.language), 20);

  /*
   * Set up features.
   */
  static_assert ((sizeof (DWRITE_TYPOGRAPHIC_FEATURES) == sizeof (hb_ms_features_t)), "");
  static_assert ((sizeof (DWRITE_FONT_FEATURE) == sizeof (hb_ms_feature_t)), "");
  hb_vector_t<hb_ms_features_t *> range_features;
  hb_vector_t<uint32_t> range_char_counts;

  // https://github.com/harfbuzz/harfbuzz/pull/5114
  // The data allocated by these two vectors are used by the above two, so they
  // should remain alive as long as the above two are.
  hb_vector_t<hb_ms_feature_t> feature_records;
  hb_vector_t<hb_ms_range_record_t> range_records;
  如果 (num_features)
  {
    如果 (hb_ms_setup_features (features, num_features, feature_records, range_records))
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
  如果 (unlikely (!clusterMap.resize_exact (textLength) ||
                !textProperties.resize_exact (textLength)))
    FAIL ("Failed to allocate shaping text data.");

  hb_vector_t<uint16_t> glyphIndices;
  hb_vector_t<DWRITE_SHAPING_GLYPH_PROPERTIES> glyphProperties;

retry_getglyphs:
  如果 (unlikely (!glyphIndices.resize_exact (maxGlyphCount) ||
                !glyphProperties.resize_exact (maxGlyphCount)))
    FAIL ("Failed to allocate shaping glyph data.");

  hr = analyzer->GetGlyphs (textString,
			    chars_len,
			    fontFace,
			    false,
			    isRightToLeft,
			    &runHead->mScript,
			    localeName,
			    nullptr,
			    (const DWRITE_TYPOGRAPHIC_FEATURES**) range_features.arrayZ,
			    range_char_counts.arrayZ,
			    range_features.length,
			    maxGlyphCount,
			    clusterMap.arrayZ,
			    textProperties.arrayZ,
			    glyphIndices.arrayZ,
			    glyphProperties.arrayZ,
			    &glyphCount);

  如果 (unlikely (hr == HRESULT_FROM_WIN32 (ERROR_INSUFFICIENT_BUFFER)))
  {
    maxGlyphCount *= 2;

    goto retry_getglyphs;
  }
  如果 (FAILED (hr))
    FAIL ("Analyzer failed to get glyphs.");

  hb_vector_t<float> glyphAdvances;
  hb_vector_t<DWRITE_GLYPH_OFFSET> glyphOffsets;
  如果 (unlikely (!glyphAdvances.resize_exact (maxGlyphCount) ||
                !glyphOffsets.resize_exact (maxGlyphCount)))
    FAIL ("Failed to allocate glyph positioning data.");


  unsigned fontEmSize = font->face->get_upem ();

  float x_mult = font->x_multf;
  float y_mult = font->y_multf;

  hr = analyzer->GetGlyphPlacements (textString,
				     clusterMap.arrayZ,
				     textProperties.arrayZ,
				     chars_len,
				     glyphIndices.arrayZ,
				     glyphProperties.arrayZ,
				     glyphCount,
				     fontFace,
				     fontEmSize,
				     false,
				     isRightToLeft,
				     &runHead->mScript,
				     localeName,
				     (const DWRITE_TYPOGRAPHIC_FEATURES**) range_features.arrayZ,
				     range_char_counts.arrayZ,
				     range_features.length,
				     glyphAdvances.arrayZ,
				     glyphOffsets.arrayZ);

  如果 (FAILED (hr))
    FAIL ("Analyzer failed to get glyph placements.");

  /* Ok, we've got everything we need, now compose output buffer,
   * very, *very*, carefully! */

  /* Calculate visual-clusters.  That's what we ship. */
  hb_vector_t<uint32_t> vis_clusters;
  如果 (unlikely (!vis_clusters.resize_exact (glyphCount)))
    FAIL ("Failed to allocate visual clusters.");

  为 (unsigned 整数 i = 0; i < glyphCount; i++)
    vis_clusters[i] = (uint32_t) -1;
  为 (unsigned 整数 i = 0; i < buffer->len; i++)
  {
    unsigned 整数 c = clusterMap[buffer->info[i].utf16_index ()];
    如果 (likely (c < glyphCount))
      vis_clusters[c] = hb_min (vis_clusters[c], buffer->info[i].cluster);
  }
  为 (unsigned 整数 i = 1; i < glyphCount; i++)
    如果 (vis_clusters[i] == (uint32_t) -1)
      vis_clusters[i] = vis_clusters[i - 1];
  为 (整数 i = (整数) glyphCount - 2; i >= 0; i--)
    如果 (vis_clusters[i] == (uint32_t) -1)
      vis_clusters[i] = vis_clusters[i + 1];

#undef utf16_index

  如果 (unlikely (!buffer->ensure (glyphCount)))
    FAIL ("Buffer in error");

#undef FAIL

  /* Set glyph infos */
  buffer->len = 0;
  为 (unsigned 整数 i = 0; i < glyphCount; i++)
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
  为 (unsigned 整数 i = 0; i < glyphCount; i++)
  {
    hb_glyph_info_t *info = &buffer->info[i];
    hb_glyph_position_t *pos = &buffer->pos[i];

    /* TODO vertical */
    pos->x_advance = hb_clamp_to<hb_position_t> (round (x_mult * (int32_t) info->mask));
    double x_offset = isRightToLeft ? -(double) info->var1.i32 : info->var1.i32;
    pos->x_offset = hb_clamp_to<hb_position_t> (round ((double) x_mult * x_offset));
    pos->y_offset = hb_clamp_to<hb_position_t> (round (y_mult * info->var2.i32));
  }

  如果 (isRightToLeft) hb_buffer_reverse (buffer);

  buffer->clear_glyph_flags ();
  buffer->unsafe_to_break ();

  /* Wow, done! */
  返回 true;
}


#endif
