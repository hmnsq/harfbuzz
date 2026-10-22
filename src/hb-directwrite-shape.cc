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

#جيڪڏهن_وصف_ٿيل HAVE_DIRECTWRITE

#ضمن "hb-shaper-impl.hh"

#ضمن "hb-directwrite.hh"

#ضمن "hb-ms-feature-ranges.hh"


/*
* shaper face data
*/

hb_directwrite_face_معلومات_t *
hb_directwrite_shaper_face_معلومات_create (hb_face_t *face)
{
  hb_blob_t *blob = hb_face_reference_blob (face);

  hb_directwrite_face_معلومات_t *معلومات = (hb_directwrite_face_معلومات_t *) dw_face_create (blob, face->اشاريو_2);

  hb_blob_destroy (blob);

  أرجع معلومات;
}

فراغ
hb_directwrite_shaper_face_معلومات_destroy (hb_directwrite_face_معلومات_t *معلومات)
{
  ((IDWriteFontFace *) معلومات)->Release ();
}


/*
 * shaper font data
 */

بنية hb_directwrite_font_معلومات_t {};

hb_directwrite_font_معلومات_t *
hb_directwrite_shaper_font_معلومات_create (hb_font_t *font)
{
  IDWriteFontFace *fontFace = (IDWriteFontFace *) (ثابت فراغ *) font->face->معلومات.directwrite;

  /*
   * Set up variations.
   */
  IDWriteFontFace5 *fontFaceVariations = خالي_اشارو;
  {
    IDWriteFontFace5 *fontFace5;
    إذا (SUCCEEDED (fontFace->QueryInterface (__uuidof (IDWriteFontFace5), (فراغ **) &fontFace5)))
    {
      IDWriteFontResource *fontResource;
      إذا (SUCCEEDED (fontFace5->GetFontResource (&fontResource)))
      {
	hb_ويڪٽر_t<DWRITE_FONT_AXIS_قدر> axis_قدرون;
	إذا (likely (axis_قدرون.resize_exact (font->انگ_coords)))
	{
	  لكل (بنانشان صحيح i = 0; i < font->انگ_coords; i++)
	  {
	    hb_ot_var_axis_info_t info;
	    بنانشان صحيح c = 1;
	    hb_ot_var_get_axis_infos (font->face, i, &c, &info);
	    axis_قدرون[i].axis_ٽيگ = (DWRITE_FONT_AXIS_ٽيگ) hb_uint32_مٽا_سٽا (info.ٽيگ_2);
	    axis_قدرون[i].value = i < font->انگ_coords ?
				   hb_clamp (font->design_coords[i], info.min_قدر, info.max_قدر) :
				   info.default_قدر;
	  }

	  fontResource->CreateFontFace (DWRITE_FONT_SIMULATIONS::DWRITE_FONT_SIMULATIONS_NONE,
					axis_قدرون.arrayZ, axis_قدرون.ڊيگهائي, &fontFaceVariations);
	}
	fontResource->Release ();
      }
      fontFace5->Release ();
    }
  }

  أرجع (hb_directwrite_font_معلومات_t *) fontFaceVariations;
}

فراغ
hb_directwrite_shaper_font_معلومات_destroy (hb_directwrite_font_معلومات_t *معلومات)
{
  ((IDWriteFontFace *) (ثابت فراغ *) معلومات)->Release ();
}


// Most of TextAnalysis is originally written by Bas Schouten for Mozilla project
// but now is relicensed to MIT for HarfBuzz use
صنف متن_Analysis : عام IDWrite_متن_Analysis_ذريعو, عام IDWrite_متن_Analysis_Sink
{
خاص:
  hb_reference_ڳڻپ_t m_Ref_ڳڻپ;
عام:
  IFACEMETHOD (QueryInterface) (IID ثابت& iid, ٻاهريون_3 فراغ** ppObject)
  { أرجع S_OK; }
  IFACEMETHOD_ (ULONG, جوڙيو_Ref) ()
  {
    أرجع m_Ref_ڳڻپ.inc () + 1;
  }
  IFACEMETHOD_ (ULONG, Release) ()
  {
    نشانوارو ref_ڳڻپ = m_Ref_ڳڻپ.dec () - 1;
    أكد (ref_ڳڻپ >= 0);
    إذا (ref_ڳڻپ)
      أرجع ref_ڳڻپ;
    أرجع 0;
  }

  // A single contiguous run of characters containing the same analysis
  // results.
  بنية لاڳيتو_2
  {
    uint32_t m_متن_شروعات;   // starting text position of this run
    uint32_t m_متن_ڊيگهائي;  // number of contiguous code units covered
    uint32_t m_Glyph_شروعات;  // starting glyph in the glyphs array
    uint32_t m_Glyph_ڳڻپ;  // number of glyphs associated with this run
    //متن
    DWRITE_SCRIPT_ANALYSIS mScript;
    uint8_t mBidiLevel;
    منطقي mIsSideways;

    منطقي شامل_آهي_متن_Position (uint32_t a_متن_Position) ثابت
    {
      أرجع a_متن_Position >= m_متن_شروعات &&
	     a_متن_Position <  m_متن_شروعات + m_متن_ڊيگهائي;
    }

    لاڳيتو_2 *ايندڙ_لاڳيتو;
  };

عام:
  متن_Analysis (ثابت wchar_t* متن, uint32_t متن_ڊيگهائي_2,
		ثابت wchar_t* localeName, DWRITE_READING_DIRECTION readingDirection)
	       : m_متن_ڊيگهائي (متن_ڊيگهائي_2), m_متن (متن), mLocaleName (localeName),
		 mReadingDirection (readingDirection), m_Current_لاڳيتو (خالي_اشارو)
  {
    m_Ref_ڳڻپ.init ();
  }
  ~متن_Analysis ()
  {
    // free runs, except mRunHead which is part of the TextAnalysis object
    لكل (لاڳيتو_2 *لاڳيتو = m_لاڳيتو_سر.ايندڙ_لاڳيتو; لاڳيتو;)
    {
      لاڳيتو_2 *orig_لاڳيتو = لاڳيتو;
      لاڳيتو = لاڳيتو->ايندڙ_لاڳيتو;
      hb_free (orig_لاڳيتو);
    }
  }

  STDMETHODIMP
  GenerateResults (IDWrite_متن_Analyzer* متن_Analyzer, لاڳيتو_2 **لاڳيتو_سر)
  {
    // Analyzes the text using the script analyzer and returns
    // the result as a series of runs.

    HRESULT hr = S_OK;

    // Initially start out with one result that covers the entire range.
    // This result will be subdivided by the analysis processes.
    m_لاڳيتو_سر.m_متن_شروعات = 0;
    m_لاڳيتو_سر.m_متن_ڊيگهائي = m_متن_ڊيگهائي;
    m_لاڳيتو_سر.mBidiLevel =
      (mReadingDirection == DWRITE_READING_DIRECTION_ساڄو_منزل_کاٻو);
    m_لاڳيتو_سر.ايندڙ_لاڳيتو = خالي_اشارو;
    m_Current_لاڳيتو = &m_لاڳيتو_سر;

    // Call each of the analyzers in sequence, recording their results.
    إذا (SUCCEEDED (hr = متن_Analyzer->AnalyzeScript (هذا, 0, m_متن_ڊيگهائي, هذا)))
      *لاڳيتو_سر = &m_لاڳيتو_سر;

    أرجع hr;
  }

  // IDWriteTextAnalysisSource implementation

  IFACEMETHODIMP
  Get_متن_At_Position (uint32_t متن_Position,
		     ٻاهريون_3 wchar_t ثابت** متن_String,
		     ٻاهريون_3 uint32_t* متن_ڊيگهائي_2)
  {
    إذا (متن_Position >= m_متن_ڊيگهائي)
    {
      // No text at this position, valid query though.
      *متن_String = خالي_اشارو;
      *متن_ڊيگهائي_2 = 0;
    }
    وإلا
    {
      *متن_String = m_متن + متن_Position;
      *متن_ڊيگهائي_2 = m_متن_ڊيگهائي - متن_Position;
    }
    أرجع S_OK;
  }

  IFACEMETHODIMP
  Get_متن_Before_Position (uint32_t متن_Position,
			 ٻاهريون_3 wchar_t ثابت** متن_String,
			 ٻاهريون_3 uint32_t* متن_ڊيگهائي_2)
  {
    إذا (متن_Position == 0 || متن_Position > m_متن_ڊيگهائي)
    {
      // Either there is no text before here (== 0), or this
      // is an invalid position. The query is considered valid though.
      *متن_String = خالي_اشارو;
      *متن_ڊيگهائي_2 = 0;
    }
    وإلا
    {
      *متن_String = m_متن;
      *متن_ڊيگهائي_2 = متن_Position;
    }
    أرجع S_OK;
  }

  IFACEMETHODIMP_ (DWRITE_READING_DIRECTION)
  GetParagraphReadingDirection () { أرجع mReadingDirection; }

  IFACEMETHODIMP GetLocaleName (uint32_t متن_Position, uint32_t* متن_ڊيگهائي_2,
				wchar_t ثابت** localeName)
  { أرجع S_OK; }

  IFACEMETHODIMP
  Get_عدد_Substitution (uint32_t متن_Position,
			 ٻاهريون_3 uint32_t* متن_ڊيگهائي_2,
			 ٻاهريون_3 IDWrite_عدد_Substitution** عدد_Substitution)
  {
    // We do not support number substitution.
    *عدد_Substitution = خالي_اشارو;
    *متن_ڊيگهائي_2 = m_متن_ڊيگهائي - متن_Position;

    أرجع S_OK;
  }

  // IDWriteTextAnalysisSink implementation

  IFACEMETHODIMP
  سيٽ_Script_Analysis (uint32_t متن_Position, uint32_t متن_ڊيگهائي_2,
		     DWRITE_SCRIPT_ANALYSIS ثابت* scriptAnalysis)
  {
    سيٽ_Current_لاڳيتو (متن_Position);
    إذا (unlikely (!ورهايو_Current_لاڳيتو (متن_Position)))
      أرجع E_OUTOFMEMORY;
    طالما (متن_ڊيگهائي_2 > 0)
    {
      لاڳيتو_2 *لاڳيتو = Fetch_ايندڙ_لاڳيتو (&متن_ڊيگهائي_2);
      إذا (unlikely (!لاڳيتو))
        أرجع E_OUTOFMEMORY;
      لاڳيتو->mScript = *scriptAnalysis;
    }

    أرجع S_OK;
  }

  IFACEMETHODIMP
  سيٽ_سٽ_Breakpoints (uint32_t متن_Position,
		      uint32_t متن_ڊيگهائي_2,
		      ثابت DWRITE_سٽ_BREAKPOINT* سٽ_Breakpoints)
  { أرجع S_OK; }

  IFACEMETHODIMP سيٽ_Bidi_Level (uint32_t متن_Position, uint32_t متن_ڊيگهائي_2,
			       uint8_t explicitLevel, uint8_t resolvedLevel)
  { أرجع S_OK; }

  IFACEMETHODIMP
  سيٽ_عدد_Substitution (uint32_t متن_Position, uint32_t متن_ڊيگهائي_2,
			 IDWrite_عدد_Substitution* عدد_Substitution)
  { أرجع S_OK; }

محمي:
  لاڳيتو_2 *Fetch_ايندڙ_لاڳيتو (اندريون_2 ٻاهريون_3 uint32_t* متن_ڊيگهائي_2)
  {
    // Used by the sink setters, this returns a reference to the next run.
    // Position and length are adjusted to now point after the current run
    // being returned.

    لاڳيتو_2 *orig_لاڳيتو = m_Current_لاڳيتو;
    // Split the tail if needed (the length remaining is less than the
    // current run's size).
    إذا (*متن_ڊيگهائي_2 < m_Current_لاڳيتو->m_متن_ڊيگهائي)
    {
      إذا (unlikely (!ورهايو_Current_لاڳيتو (m_Current_لاڳيتو->m_متن_شروعات + *متن_ڊيگهائي_2)))
        أرجع خالي_اشارو;
    }
    وإلا
      // Just advance the current run.
      m_Current_لاڳيتو = m_Current_لاڳيتو->ايندڙ_لاڳيتو;
    *متن_ڊيگهائي_2 -= orig_لاڳيتو->m_متن_ڊيگهائي;

    // Return a reference to the run that was just current.
    أرجع orig_لاڳيتو;
  }

  فراغ سيٽ_Current_لاڳيتو (uint32_t متن_Position)
  {
    // Move the current run to the given position.
    // Since the analyzers generally return results in a forward manner,
    // this will usually just return early. If not, find the
    // corresponding run for the text position.

    إذا (m_Current_لاڳيتو && m_Current_لاڳيتو->شامل_آهي_متن_Position (متن_Position))
      أرجع;

    لكل (لاڳيتو_2 *لاڳيتو = &m_لاڳيتو_سر; لاڳيتو; لاڳيتو = لاڳيتو->ايندڙ_لاڳيتو)
      إذا (لاڳيتو->شامل_آهي_متن_Position (متن_Position))
      {
	m_Current_لاڳيتو = لاڳيتو;
	أرجع;
      }
    أكد (0); // We should always be able to find the text position in one of our runs
  }

  منطقي ورهايو_Current_لاڳيتو (uint32_t ورهايو_Position)
  {
    إذا (!m_Current_لاڳيتو)
    {
      أكد (0); // SplitCurrentRun called without current run
      // Shouldn't be calling this when no current run is set!
      أرجع خطأ;
    }
    // Split the current run.
    إذا (ورهايو_Position <= m_Current_لاڳيتو->m_متن_شروعات)
    {
      // No need to split, already the start of a run
      // or before it. Usually the first.
      أرجع سچ;
    }
    لاڳيتو_2 *new_لاڳيتو = (لاڳيتو_2 *) hb_malloc (حجم (لاڳيتو_2));
    إذا (unlikely (!new_لاڳيتو))
      أرجع خطأ;

    *new_لاڳيتو = *m_Current_لاڳيتو;

    // Insert the new run in our linked list.
    new_لاڳيتو->ايندڙ_لاڳيتو = m_Current_لاڳيتو->ايندڙ_لاڳيتو;
    m_Current_لاڳيتو->ايندڙ_لاڳيتو = new_لاڳيتو;

    // Adjust runs' text positions and lengths.
    uint32_t ورهايو_نقطو = ورهايو_Position - m_Current_لاڳيتو->m_متن_شروعات;
    new_لاڳيتو->m_متن_شروعات += ورهايو_نقطو;
    new_لاڳيتو->m_متن_ڊيگهائي -= ورهايو_نقطو;
    m_Current_لاڳيتو->m_متن_ڊيگهائي = ورهايو_نقطو;
    m_Current_لاڳيتو = new_لاڳيتو;
    أرجع سچ;
  }

محمي:
  //داخلا
  // (weak references are fine here, since this class is a transient
  //  stack-based helper that doesn't need to copy data)
  uint32_t m_متن_ڊيگهائي;
  ثابت wchar_t* m_متن;
  ثابت wchar_t* mLocaleName;
  DWRITE_READING_DIRECTION mReadingDirection;

  // Current processing state.
  لاڳيتو_2 *m_Current_لاڳيتو;

  // Output is a list of runs starting here
  لاڳيتو_2  m_لاڳيتو_سر;
};

/*
 * shaper
 */

hb_bool_t
_hb_directwrite_shape (hb_shape_plan_t    *shape_plan,
		       hb_font_t          *font,
		       hb_بفر_t        *بفر,
		       ثابت hb_feature_t *features,
		       بنانشان صحيح        انگ_features)
{
  IDWriteFontFace *fontFace = (IDWriteFontFace *) (ثابت فراغ *) font->معلومات.directwrite;
  خودڪار *global = get_directwrite_global ();
  إذا (unlikely (!global))
    أرجع خطأ;
  IDWriteFactory *dwriteFactory = global->dwriteFactory;

  IDWrite_متن_Analyzer* analyzer;
  dwriteFactory->Create_متن_Analyzer (&analyzer);

  بنانشان صحيح scratch_سائيز;
  hb_بفر_t::scratch_بفر_t *scratch = بفر->get_scratch_بفر (&scratch_سائيز);
#وصف_ڪريو ALLOCATE_ARRAY(Type, name, len) \
  Type *name = (Type *) scratch; \
  افعل { \
    بنانشان صحيح _consumed = DIV_CEIL ((len) * حجم (Type), حجم (*scratch)); \
    أكد (_consumed <= scratch_سائيز); \
    scratch += _consumed; \
    scratch_سائيز -= _consumed; \
  } طالما (0)

#وصف_ڪريو utf16_اشاريو() var1.u32

  ALLOCATE_ARRAY (wchar_t, متن_String, بفر->len * 2);

  بنانشان صحيح chars_len = 0;
  لكل (بنانشان صحيح i = 0; i < بفر->len; i++)
  {
    hb_codepoint_t c = بفر->info[i].codepoint;
    بفر->info[i].utf16_اشاريو () = chars_len;
    إذا (likely (c <= 0xFFFFu))
      متن_String[chars_len++] = c;
    وإلا إذا (unlikely (c > 0x10FFFFu))
      متن_String[chars_len++] = 0xFFFDu;
    وإلا
    {
      متن_String[chars_len++] = 0xD800u + ((c - 0x10000u) >> 10);
      متن_String[chars_len++] = 0xDC00u + ((c - 0x10000u) & ((1u << 10) - 1));
    }
  }

  ALLOCATE_ARRAY (لفظ, log_clusters, chars_len);
#وصف_رد ALLOCATE_ARRAY
  /* Need log_clusters to assign features. */
  chars_len = 0;
  لكل (بنانشان صحيح i = 0; i < بفر->len; i++)
  {
    hb_codepoint_t c = بفر->info[i].codepoint;
    بنانشان صحيح cluster = بفر->info[i].cluster;
    log_clusters[chars_len++] = cluster;
    إذا (hb_اندريون_range (c, 0x10000u, 0x10FFFFu))
      log_clusters[chars_len++] = cluster; /* Surrogates. */
  }

  DWRITE_READING_DIRECTION readingDirection;
  readingDirection = بفر->props.direction ?
		     DWRITE_READING_DIRECTION_ساڄو_منزل_کاٻو :
		     DWRITE_READING_DIRECTION_کاٻو_منزل_ساڄو;

  /*
  * There's an internal 16-bit limit on some things inside the analyzer,
  * but we never attempt to shape a word longer than 64K characters
  * in a single gfxShapedWord, so we cannot exceed that limit.
  */
  uint32_t متن_ڊيگهائي_2 = chars_len;

  متن_Analysis analysis (متن_String, متن_ڊيگهائي_2, خالي_اشارو, readingDirection);
  متن_Analysis::لاڳيتو_2 *لاڳيتو_سر;
  HRESULT hr;
  hr = analysis.GenerateResults (analyzer, &لاڳيتو_سر);

#وصف_ڪريو FAIL(...) \
  HB_STMT_شروعات { \
    DEBUG_نياپو (DIRECTWRITE, خالي_اشارو, __VA_ARGS__); \
    أرجع خطأ; \
  } HB_STMT_پڇاڙي

  إذا (FAILED (hr))
    FAIL ("Analyzer failed to generate results.");

  uint32_t max_Glyph_ڳڻپ = 3 * متن_ڊيگهائي_2 / 2 + 16;
  uint32_t glyph_ڳڻپ_2;
  منطقي is_ساڄو_منزل_کاٻو = HB_DIRECTION_IS_BACKWARD (بفر->props.direction);

  ثابت wchar_t localeName[20] = {0};
  إذا (بفر->props.language)
    mbstowcs ((wchar_t*) localeName,
	      hb_language_منزل_string (بفر->props.language), 20);

  /*
   * Set up features.
   */
  static_assert ((حجم (DWRITE_TYPOGRAPHIC_FEATURES) == حجم (hb_ms_features_t)), "");
  static_assert ((حجم (DWRITE_FONT_FEATURE) == حجم (hb_ms_feature_t)), "");
  hb_ويڪٽر_t<hb_ms_features_t *> range_features;
  hb_ويڪٽر_t<uint32_t> range_char_counts;

  // https://github.com/harfbuzz/harfbuzz/pull/5114
  // The data allocated by these two vectors are used by the above two, so they
  // should remain alive as long as the above two are.
  hb_ويڪٽر_t<hb_ms_feature_t> feature_رڪارڊون;
  hb_ويڪٽر_t<hb_ms_range_رڪارڊ_t> range_رڪارڊون;
  إذا (انگ_features)
  {
    إذا (hb_ms_تياري_features (features, انگ_features, feature_رڪارڊون, range_رڪارڊون))
    {
      hb_ms_make_feature_ranges (feature_رڪارڊون,
				 range_رڪارڊون,
				 0,
				 chars_len,
				 log_clusters,
				 range_features,
				 range_char_counts);
    }
  }

  hb_ويڪٽر_t<uint16_t> clusterMap;
  hb_ويڪٽر_t<DWRITE_SHAPING_متن_PROPERTIES> متن_Properties;
  إذا (unlikely (!clusterMap.resize_exact (متن_ڊيگهائي_2) ||
                !متن_Properties.resize_exact (متن_ڊيگهائي_2)))
    FAIL ("Failed to allocate shaping text data.");

  hb_ويڪٽر_t<uint16_t> glyphIndices;
  hb_ويڪٽر_t<DWRITE_SHAPING_GLYPH_PROPERTIES> glyphProperties;

retry_getglyphs:
  إذا (unlikely (!glyphIndices.resize_exact (max_Glyph_ڳڻپ) ||
                !glyphProperties.resize_exact (max_Glyph_ڳڻپ)))
    FAIL ("Failed to allocate shaping glyph data.");

  hr = analyzer->GetGlyphs (متن_String,
			    chars_len,
			    fontFace,
			    خطأ,
			    is_ساڄو_منزل_کاٻو,
			    &لاڳيتو_سر->mScript,
			    localeName,
			    خالي_اشارو,
			    (ثابت DWRITE_TYPOGRAPHIC_FEATURES**) range_features.arrayZ,
			    range_char_counts.arrayZ,
			    range_features.ڊيگهائي,
			    max_Glyph_ڳڻپ,
			    clusterMap.arrayZ,
			    متن_Properties.arrayZ,
			    glyphIndices.arrayZ,
			    glyphProperties.arrayZ,
			    &glyph_ڳڻپ_2);

  إذا (unlikely (hr == HRESULT_FROM_WIN32 (ERROR_INSUFFICIENT_بفر)))
  {
    max_Glyph_ڳڻپ *= 2;

    اذهب retry_getglyphs;
  }
  إذا (FAILED (hr))
    FAIL ("Analyzer failed to get glyphs.");

  hb_ويڪٽر_t<عائم> glyphAdvances;
  hb_ويڪٽر_t<DWRITE_GLYPH_وٿي> glyphOffsets;
  إذا (unlikely (!glyphAdvances.resize_exact (max_Glyph_ڳڻپ) ||
                !glyphOffsets.resize_exact (max_Glyph_ڳڻپ)))
    FAIL ("Failed to allocate glyph positioning data.");


  بنانشان font_Em_سائيز = font->face->get_upem ();

  عائم x_mult = font->x_multf;
  عائم y_mult = font->y_multf;

  hr = analyzer->GetGlyphPlacements (متن_String,
				     clusterMap.arrayZ,
				     متن_Properties.arrayZ,
				     chars_len,
				     glyphIndices.arrayZ,
				     glyphProperties.arrayZ,
				     glyph_ڳڻپ_2,
				     fontFace,
				     font_Em_سائيز,
				     خطأ,
				     is_ساڄو_منزل_کاٻو,
				     &لاڳيتو_سر->mScript,
				     localeName,
				     (ثابت DWRITE_TYPOGRAPHIC_FEATURES**) range_features.arrayZ,
				     range_char_counts.arrayZ,
				     range_features.ڊيگهائي,
				     glyphAdvances.arrayZ,
				     glyphOffsets.arrayZ);

  إذا (FAILED (hr))
    FAIL ("Analyzer failed to get glyph placements.");

  /* Ok, we've got everything we need, now compose output buffer,
   * very, *very*, carefully! */

  /* Calculate visual-clusters.  That's what we ship. */
  hb_ويڪٽر_t<uint32_t> vis_clusters;
  إذا (unlikely (!vis_clusters.resize_exact (glyph_ڳڻپ_2)))
    FAIL ("Failed to allocate visual clusters.");

  لكل (بنانشان صحيح i = 0; i < glyph_ڳڻپ_2; i++)
    vis_clusters[i] = (uint32_t) -1;
  لكل (بنانشان صحيح i = 0; i < بفر->len; i++)
  {
    بنانشان صحيح c = clusterMap[بفر->info[i].utf16_اشاريو ()];
    إذا (likely (c < glyph_ڳڻپ_2))
      vis_clusters[c] = hb_min (vis_clusters[c], بفر->info[i].cluster);
  }
  لكل (بنانشان صحيح i = 1; i < glyph_ڳڻپ_2; i++)
    إذا (vis_clusters[i] == (uint32_t) -1)
      vis_clusters[i] = vis_clusters[i - 1];
  لكل (صحيح i = (صحيح) glyph_ڳڻپ_2 - 2; i >= 0; i--)
    إذا (vis_clusters[i] == (uint32_t) -1)
      vis_clusters[i] = vis_clusters[i + 1];

#وصف_رد utf16_اشاريو

  إذا (unlikely (!بفر->ensure (glyph_ڳڻپ_2)))
    FAIL ("Buffer in error");

#وصف_رد FAIL

  /* Set glyph infos */
  بفر->len = 0;
  لكل (بنانشان صحيح i = 0; i < glyph_ڳڻپ_2; i++)
  {
    hb_glyph_info_t *info = &بفر->info[بفر->len++];

    info->codepoint = glyphIndices[i];
    info->cluster = vis_clusters[i];

    /* The rest is crap.  Let's store position info there for now. */
    info->نقاب = (hb_نقاب_t) hb_clamp_منزل<int32_t> (glyphAdvances[i]);
    info->var1.i32 = hb_clamp_منزل<int32_t> (glyphOffsets[i].advance_وٿي);
    info->var2.i32 = hb_clamp_منزل<int32_t> (glyphOffsets[i].ascender_وٿي);
  }

  /* Set glyph positions */
  بفر->clear_positions ();
  لكل (بنانشان صحيح i = 0; i < glyph_ڳڻپ_2; i++)
  {
    hb_glyph_info_t *info = &بفر->info[i];
    hb_glyph_position_t *pos = &بفر->pos[i];

    /* TODO vertical */
    pos->x_advance = hb_clamp_منزل<hb_position_t> (round (x_mult * (int32_t) info->نقاب));
    مزدوج x_وٿي_2 = is_ساڄو_منزل_کاٻو ? -(مزدوج) info->var1.i32 : info->var1.i32;
    pos->x_وٿي_2 = hb_clamp_منزل<hb_position_t> (round ((مزدوج) x_mult * x_وٿي_2));
    pos->y_وٿي_2 = hb_clamp_منزل<hb_position_t> (round (y_mult * info->var2.i32));
  }

  إذا (is_ساڄو_منزل_کاٻو) hb_بفر_ابتو (بفر);

  بفر->clear_glyph_flags ();
  بفر->unsafe_منزل_break ();

  /* Wow, done! */
  أرجع سچ;
}


#جيڪڏهن_پورو
