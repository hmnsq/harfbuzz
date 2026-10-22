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

#اگر_تعریف_شده HAVE_DIRECTWRITE

#ضمن "hb-shaper-impl.hh"

#ضمن "hb-directwrite.hh"

#ضمن "hb-ms-feature-ranges.hh"


/*
* shaper face data
*/

hb_directwrite_face_داده_t *
hb_directwrite_shaper_face_داده_create (hb_face_t *face)
{
  hb_blob_t *blob = hb_face_reference_blob (face);

  hb_directwrite_face_داده_t *داده = (hb_directwrite_face_داده_t *) dw_face_create (blob, face->شاخص_2);

  hb_blob_destroy (blob);

  أرجع داده;
}

فراغ
hb_directwrite_shaper_face_داده_destroy (hb_directwrite_face_داده_t *داده)
{
  ((IDWriteFontFace *) داده)->Release ();
}


/*
 * shaper font data
 */

بنية hb_directwrite_font_داده_t {};

hb_directwrite_font_داده_t *
hb_directwrite_shaper_font_داده_create (hb_font_t *font)
{
  IDWriteFontFace *fontFace = (IDWriteFontFace *) (ثابت فراغ *) font->face->داده.directwrite;

  /*
   * Set up variations.
   */
  IDWriteFontFace5 *fontFaceVariations = اشاره_تهی;
  {
    IDWriteFontFace5 *fontFace5;
    إذا (SUCCEEDED (fontFace->QueryInterface (__uuidof (IDWriteFontFace5), (فراغ **) &fontFace5)))
    {
      IDWriteFontResource *fontResource;
      إذا (SUCCEEDED (fontFace5->GetFontResource (&fontResource)))
      {
	hb_وکتور_t<DWRITE_FONT_AXIS_مقدار> axis_مقادیر;
	إذا (likely (axis_مقادیر.resize_exact (font->رقم_coords)))
	{
	  لكل (بیعلامت صحيح i = 0; i < font->رقم_coords; i++)
	  {
	    hb_ot_var_axis_info_t info;
	    بیعلامت صحيح c = 1;
	    hb_ot_var_get_axis_infos (font->face, i, &c, &info);
	    axis_مقادیر[i].axis_برچسب = (DWRITE_FONT_AXIS_برچسب) hb_uint32_جابجا (info.برچسب_2);
	    axis_مقادیر[i].value = i < font->رقم_coords ?
				   hb_clamp (font->design_coords[i], info.min_مقدار, info.max_مقدار) :
				   info.default_مقدار;
	  }

	  fontResource->CreateFontFace (DWRITE_FONT_SIMULATIONS::DWRITE_FONT_SIMULATIONS_NONE,
					axis_مقادیر.arrayZ, axis_مقادیر.درازی, &fontFaceVariations);
	}
	fontResource->Release ();
      }
      fontFace5->Release ();
    }
  }

  أرجع (hb_directwrite_font_داده_t *) fontFaceVariations;
}

فراغ
hb_directwrite_shaper_font_داده_destroy (hb_directwrite_font_داده_t *داده)
{
  ((IDWriteFontFace *) (ثابت فراغ *) داده)->Release ();
}


// Most of TextAnalysis is originally written by Bas Schouten for Mozilla project
// but now is relicensed to MIT for HarfBuzz use
صنف متن_Analysis : عام IDWrite_متن_Analysis_منبع, عام IDWrite_متن_Analysis_Sink
{
خاص:
  hb_reference_شمار_t m_Ref_شمار;
عام:
  IFACEMETHOD (QueryInterface) (IID ثابت& iid, بیرون_3 فراغ** ppObject)
  { أرجع S_OK; }
  IFACEMETHOD_ (ULONG, جمع_کن_Ref) ()
  {
    أرجع m_Ref_شمار.inc () + 1;
  }
  IFACEMETHOD_ (ULONG, Release) ()
  {
    علامتدار ref_شمار = m_Ref_شمار.dec () - 1;
    أكد (ref_شمار >= 0);
    إذا (ref_شمار)
      أرجع ref_شمار;
    أرجع 0;
  }

  // A single contiguous run of characters containing the same analysis
  // results.
  بنية پیاپی_2
  {
    uint32_t m_متن_شروع;   // starting text position of this run
    uint32_t m_متن_درازی;  // number of contiguous code units covered
    uint32_t m_Glyph_شروع;  // starting glyph in the glyphs array
    uint32_t m_Glyph_شمار;  // number of glyphs associated with this run
    //متن
    DWRITE_SCRIPT_ANALYSIS mScript;
    uint8_t mBidiLevel;
    منطقي mIsSideways;

    منطقي شامل_است_متن_Position (uint32_t a_متن_Position) ثابت
    {
      أرجع a_متن_Position >= m_متن_شروع &&
	     a_متن_Position <  m_متن_شروع + m_متن_درازی;
    }

    پیاپی_2 *بعدی_پیاپی;
  };

عام:
  متن_Analysis (ثابت wchar_t* متن, uint32_t متن_درازی_2,
		ثابت wchar_t* localeName, DWRITE_READING_DIRECTION readingDirection)
	       : m_متن_درازی (متن_درازی_2), m_متن (متن), mLocaleName (localeName),
		 mReadingDirection (readingDirection), m_Current_پیاپی (اشاره_تهی)
  {
    m_Ref_شمار.init ();
  }
  ~متن_Analysis ()
  {
    // free runs, except mRunHead which is part of the TextAnalysis object
    لكل (پیاپی_2 *پیاپی = m_پیاپی_سر.بعدی_پیاپی; پیاپی;)
    {
      پیاپی_2 *orig_پیاپی = پیاپی;
      پیاپی = پیاپی->بعدی_پیاپی;
      hb_free (orig_پیاپی);
    }
  }

  STDMETHODIMP
  GenerateResults (IDWrite_متن_Analyzer* متن_Analyzer, پیاپی_2 **پیاپی_سر)
  {
    // Analyzes the text using the script analyzer and returns
    // the result as a series of runs.

    HRESULT hr = S_OK;

    // Initially start out with one result that covers the entire range.
    // This result will be subdivided by the analysis processes.
    m_پیاپی_سر.m_متن_شروع = 0;
    m_پیاپی_سر.m_متن_درازی = m_متن_درازی;
    m_پیاپی_سر.mBidiLevel =
      (mReadingDirection == DWRITE_READING_DIRECTION_راست_مقصد_چپ);
    m_پیاپی_سر.بعدی_پیاپی = اشاره_تهی;
    m_Current_پیاپی = &m_پیاپی_سر;

    // Call each of the analyzers in sequence, recording their results.
    إذا (SUCCEEDED (hr = متن_Analyzer->AnalyzeScript (هذا, 0, m_متن_درازی, هذا)))
      *پیاپی_سر = &m_پیاپی_سر;

    أرجع hr;
  }

  // IDWriteTextAnalysisSource implementation

  IFACEMETHODIMP
  Get_متن_At_Position (uint32_t متن_Position,
		     بیرون_3 wchar_t ثابت** متن_String,
		     بیرون_3 uint32_t* متن_درازی_2)
  {
    إذا (متن_Position >= m_متن_درازی)
    {
      // No text at this position, valid query though.
      *متن_String = اشاره_تهی;
      *متن_درازی_2 = 0;
    }
    وإلا
    {
      *متن_String = m_متن + متن_Position;
      *متن_درازی_2 = m_متن_درازی - متن_Position;
    }
    أرجع S_OK;
  }

  IFACEMETHODIMP
  Get_متن_Before_Position (uint32_t متن_Position,
			 بیرون_3 wchar_t ثابت** متن_String,
			 بیرون_3 uint32_t* متن_درازی_2)
  {
    إذا (متن_Position == 0 || متن_Position > m_متن_درازی)
    {
      // Either there is no text before here (== 0), or this
      // is an invalid position. The query is considered valid though.
      *متن_String = اشاره_تهی;
      *متن_درازی_2 = 0;
    }
    وإلا
    {
      *متن_String = m_متن;
      *متن_درازی_2 = متن_Position;
    }
    أرجع S_OK;
  }

  IFACEMETHODIMP_ (DWRITE_READING_DIRECTION)
  GetParagraphReadingDirection () { أرجع mReadingDirection; }

  IFACEMETHODIMP GetLocaleName (uint32_t متن_Position, uint32_t* متن_درازی_2,
				wchar_t ثابت** localeName)
  { أرجع S_OK; }

  IFACEMETHODIMP
  Get_عدد_Substitution (uint32_t متن_Position,
			 بیرون_3 uint32_t* متن_درازی_2,
			 بیرون_3 IDWrite_عدد_Substitution** عدد_Substitution)
  {
    // We do not support number substitution.
    *عدد_Substitution = اشاره_تهی;
    *متن_درازی_2 = m_متن_درازی - متن_Position;

    أرجع S_OK;
  }

  // IDWriteTextAnalysisSink implementation

  IFACEMETHODIMP
  مجموعه_Script_Analysis (uint32_t متن_Position, uint32_t متن_درازی_2,
		     DWRITE_SCRIPT_ANALYSIS ثابت* scriptAnalysis)
  {
    مجموعه_Current_پیاپی (متن_Position);
    إذا (unlikely (!تقسیم_Current_پیاپی (متن_Position)))
      أرجع E_OUTOFMEMORY;
    طالما (متن_درازی_2 > 0)
    {
      پیاپی_2 *پیاپی = Fetch_بعدی_پیاپی (&متن_درازی_2);
      إذا (unlikely (!پیاپی))
        أرجع E_OUTOFMEMORY;
      پیاپی->mScript = *scriptAnalysis;
    }

    أرجع S_OK;
  }

  IFACEMETHODIMP
  مجموعه_سطر_Breakpoints (uint32_t متن_Position,
		      uint32_t متن_درازی_2,
		      ثابت DWRITE_سطر_BREAKPOINT* سطر_Breakpoints)
  { أرجع S_OK; }

  IFACEMETHODIMP مجموعه_Bidi_Level (uint32_t متن_Position, uint32_t متن_درازی_2,
			       uint8_t explicitLevel, uint8_t resolvedLevel)
  { أرجع S_OK; }

  IFACEMETHODIMP
  مجموعه_عدد_Substitution (uint32_t متن_Position, uint32_t متن_درازی_2,
			 IDWrite_عدد_Substitution* عدد_Substitution)
  { أرجع S_OK; }

محمي:
  پیاپی_2 *Fetch_بعدی_پیاپی (درون_2 بیرون_3 uint32_t* متن_درازی_2)
  {
    // Used by the sink setters, this returns a reference to the next run.
    // Position and length are adjusted to now point after the current run
    // being returned.

    پیاپی_2 *orig_پیاپی = m_Current_پیاپی;
    // Split the tail if needed (the length remaining is less than the
    // current run's size).
    إذا (*متن_درازی_2 < m_Current_پیاپی->m_متن_درازی)
    {
      إذا (unlikely (!تقسیم_Current_پیاپی (m_Current_پیاپی->m_متن_شروع + *متن_درازی_2)))
        أرجع اشاره_تهی;
    }
    وإلا
      // Just advance the current run.
      m_Current_پیاپی = m_Current_پیاپی->بعدی_پیاپی;
    *متن_درازی_2 -= orig_پیاپی->m_متن_درازی;

    // Return a reference to the run that was just current.
    أرجع orig_پیاپی;
  }

  فراغ مجموعه_Current_پیاپی (uint32_t متن_Position)
  {
    // Move the current run to the given position.
    // Since the analyzers generally return results in a forward manner,
    // this will usually just return early. If not, find the
    // corresponding run for the text position.

    إذا (m_Current_پیاپی && m_Current_پیاپی->شامل_است_متن_Position (متن_Position))
      أرجع;

    لكل (پیاپی_2 *پیاپی = &m_پیاپی_سر; پیاپی; پیاپی = پیاپی->بعدی_پیاپی)
      إذا (پیاپی->شامل_است_متن_Position (متن_Position))
      {
	m_Current_پیاپی = پیاپی;
	أرجع;
      }
    أكد (0); // We should always be able to find the text position in one of our runs
  }

  منطقي تقسیم_Current_پیاپی (uint32_t تقسیم_Position)
  {
    إذا (!m_Current_پیاپی)
    {
      أكد (0); // SplitCurrentRun called without current run
      // Shouldn't be calling this when no current run is set!
      أرجع خطأ;
    }
    // Split the current run.
    إذا (تقسیم_Position <= m_Current_پیاپی->m_متن_شروع)
    {
      // No need to split, already the start of a run
      // or before it. Usually the first.
      أرجع درست;
    }
    پیاپی_2 *new_پیاپی = (پیاپی_2 *) hb_malloc (حجم (پیاپی_2));
    إذا (unlikely (!new_پیاپی))
      أرجع خطأ;

    *new_پیاپی = *m_Current_پیاپی;

    // Insert the new run in our linked list.
    new_پیاپی->بعدی_پیاپی = m_Current_پیاپی->بعدی_پیاپی;
    m_Current_پیاپی->بعدی_پیاپی = new_پیاپی;

    // Adjust runs' text positions and lengths.
    uint32_t تقسیم_نقطه = تقسیم_Position - m_Current_پیاپی->m_متن_شروع;
    new_پیاپی->m_متن_شروع += تقسیم_نقطه;
    new_پیاپی->m_متن_درازی -= تقسیم_نقطه;
    m_Current_پیاپی->m_متن_درازی = تقسیم_نقطه;
    m_Current_پیاپی = new_پیاپی;
    أرجع درست;
  }

محمي:
  //درون داد
  // (weak references are fine here, since this class is a transient
  //  stack-based helper that doesn't need to copy data)
  uint32_t m_متن_درازی;
  ثابت wchar_t* m_متن;
  ثابت wchar_t* mLocaleName;
  DWRITE_READING_DIRECTION mReadingDirection;

  // Current processing state.
  پیاپی_2 *m_Current_پیاپی;

  // Output is a list of runs starting here
  پیاپی_2  m_پیاپی_سر;
};

/*
 * shaper
 */

hb_bool_t
_hb_directwrite_shape (hb_shape_plan_t    *shape_plan,
		       hb_font_t          *font,
		       hb_میانگیر_t        *میانگیر,
		       ثابت hb_feature_t *features,
		       بیعلامت صحيح        رقم_features)
{
  IDWriteFontFace *fontFace = (IDWriteFontFace *) (ثابت فراغ *) font->داده.directwrite;
  خودکار *global = get_directwrite_global ();
  إذا (unlikely (!global))
    أرجع خطأ;
  IDWriteFactory *dwriteFactory = global->dwriteFactory;

  IDWrite_متن_Analyzer* analyzer;
  dwriteFactory->Create_متن_Analyzer (&analyzer);

  بیعلامت صحيح scratch_اندازه;
  hb_میانگیر_t::scratch_میانگیر_t *scratch = میانگیر->get_scratch_میانگیر (&scratch_اندازه);
#تعریف_کن ALLOCATE_ARRAY(Type, name, len) \
  Type *name = (Type *) scratch; \
  افعل { \
    بیعلامت صحيح _consumed = DIV_CEIL ((len) * حجم (Type), حجم (*scratch)); \
    أكد (_consumed <= scratch_اندازه); \
    scratch += _consumed; \
    scratch_اندازه -= _consumed; \
  } طالما (0)

#تعریف_کن utf16_شاخص() var1.u32

  ALLOCATE_ARRAY (wchar_t, متن_String, میانگیر->len * 2);

  بیعلامت صحيح chars_len = 0;
  لكل (بیعلامت صحيح i = 0; i < میانگیر->len; i++)
  {
    hb_codepoint_t c = میانگیر->info[i].codepoint;
    میانگیر->info[i].utf16_شاخص () = chars_len;
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

  ALLOCATE_ARRAY (کلمه, log_clusters, chars_len);
#لغو_تعریف ALLOCATE_ARRAY
  /* Need log_clusters to assign features. */
  chars_len = 0;
  لكل (بیعلامت صحيح i = 0; i < میانگیر->len; i++)
  {
    hb_codepoint_t c = میانگیر->info[i].codepoint;
    بیعلامت صحيح cluster = میانگیر->info[i].cluster;
    log_clusters[chars_len++] = cluster;
    إذا (hb_درون_range (c, 0x10000u, 0x10FFFFu))
      log_clusters[chars_len++] = cluster; /* Surrogates. */
  }

  DWRITE_READING_DIRECTION readingDirection;
  readingDirection = میانگیر->props.direction ?
		     DWRITE_READING_DIRECTION_راست_مقصد_چپ :
		     DWRITE_READING_DIRECTION_چپ_مقصد_راست;

  /*
  * There's an internal 16-bit limit on some things inside the analyzer,
  * but we never attempt to shape a word longer than 64K characters
  * in a single gfxShapedWord, so we cannot exceed that limit.
  */
  uint32_t متن_درازی_2 = chars_len;

  متن_Analysis analysis (متن_String, متن_درازی_2, اشاره_تهی, readingDirection);
  متن_Analysis::پیاپی_2 *پیاپی_سر;
  HRESULT hr;
  hr = analysis.GenerateResults (analyzer, &پیاپی_سر);

#تعریف_کن FAIL(...) \
  HB_STMT_شروع { \
    DEBUG_پیام (DIRECTWRITE, اشاره_تهی, __VA_ARGS__); \
    أرجع خطأ; \
  } HB_STMT_انتها

  إذا (FAILED (hr))
    FAIL ("Analyzer failed to generate results.");

  uint32_t max_Glyph_شمار = 3 * متن_درازی_2 / 2 + 16;
  uint32_t glyph_شمار_2;
  منطقي is_راست_مقصد_چپ = HB_DIRECTION_IS_BACKWARD (میانگیر->props.direction);

  ثابت wchar_t localeName[20] = {0};
  إذا (میانگیر->props.language)
    mbstowcs ((wchar_t*) localeName,
	      hb_language_مقصد_string (میانگیر->props.language), 20);

  /*
   * Set up features.
   */
  static_assert ((حجم (DWRITE_TYPOGRAPHIC_FEATURES) == حجم (hb_ms_features_t)), "");
  static_assert ((حجم (DWRITE_FONT_FEATURE) == حجم (hb_ms_feature_t)), "");
  hb_وکتور_t<hb_ms_features_t *> range_features;
  hb_وکتور_t<uint32_t> range_char_counts;

  // https://github.com/harfbuzz/harfbuzz/pull/5114
  // The data allocated by these two vectors are used by the above two, so they
  // should remain alive as long as the above two are.
  hb_وکتور_t<hb_ms_feature_t> feature_رکوردها;
  hb_وکتور_t<hb_ms_range_رکورد_t> range_رکوردها;
  إذا (رقم_features)
  {
    إذا (hb_ms_راهاندازی_features (features, رقم_features, feature_رکوردها, range_رکوردها))
    {
      hb_ms_make_feature_ranges (feature_رکوردها,
				 range_رکوردها,
				 0,
				 chars_len,
				 log_clusters,
				 range_features,
				 range_char_counts);
    }
  }

  hb_وکتور_t<uint16_t> clusterMap;
  hb_وکتور_t<DWRITE_SHAPING_متن_PROPERTIES> متن_Properties;
  إذا (unlikely (!clusterMap.resize_exact (متن_درازی_2) ||
                !متن_Properties.resize_exact (متن_درازی_2)))
    FAIL ("Failed to allocate shaping text data.");

  hb_وکتور_t<uint16_t> glyphIndices;
  hb_وکتور_t<DWRITE_SHAPING_GLYPH_PROPERTIES> glyphProperties;

retry_getglyphs:
  إذا (unlikely (!glyphIndices.resize_exact (max_Glyph_شمار) ||
                !glyphProperties.resize_exact (max_Glyph_شمار)))
    FAIL ("Failed to allocate shaping glyph data.");

  hr = analyzer->GetGlyphs (متن_String,
			    chars_len,
			    fontFace,
			    خطأ,
			    is_راست_مقصد_چپ,
			    &پیاپی_سر->mScript,
			    localeName,
			    اشاره_تهی,
			    (ثابت DWRITE_TYPOGRAPHIC_FEATURES**) range_features.arrayZ,
			    range_char_counts.arrayZ,
			    range_features.درازی,
			    max_Glyph_شمار,
			    clusterMap.arrayZ,
			    متن_Properties.arrayZ,
			    glyphIndices.arrayZ,
			    glyphProperties.arrayZ,
			    &glyph_شمار_2);

  إذا (unlikely (hr == HRESULT_FROM_WIN32 (ERROR_INSUFFICIENT_میانگیر)))
  {
    max_Glyph_شمار *= 2;

    اذهب retry_getglyphs;
  }
  إذا (FAILED (hr))
    FAIL ("Analyzer failed to get glyphs.");

  hb_وکتور_t<عائم> glyphAdvances;
  hb_وکتور_t<DWRITE_GLYPH_جابه_جایی> glyphOffsets;
  إذا (unlikely (!glyphAdvances.resize_exact (max_Glyph_شمار) ||
                !glyphOffsets.resize_exact (max_Glyph_شمار)))
    FAIL ("Failed to allocate glyph positioning data.");


  بیعلامت font_Em_اندازه = font->face->get_upem ();

  عائم x_mult = font->x_multf;
  عائم y_mult = font->y_multf;

  hr = analyzer->GetGlyphPlacements (متن_String,
				     clusterMap.arrayZ,
				     متن_Properties.arrayZ,
				     chars_len,
				     glyphIndices.arrayZ,
				     glyphProperties.arrayZ,
				     glyph_شمار_2,
				     fontFace,
				     font_Em_اندازه,
				     خطأ,
				     is_راست_مقصد_چپ,
				     &پیاپی_سر->mScript,
				     localeName,
				     (ثابت DWRITE_TYPOGRAPHIC_FEATURES**) range_features.arrayZ,
				     range_char_counts.arrayZ,
				     range_features.درازی,
				     glyphAdvances.arrayZ,
				     glyphOffsets.arrayZ);

  إذا (FAILED (hr))
    FAIL ("Analyzer failed to get glyph placements.");

  /* Ok, we've got everything we need, now compose output buffer,
   * very, *very*, carefully! */

  /* Calculate visual-clusters.  That's what we ship. */
  hb_وکتور_t<uint32_t> vis_clusters;
  إذا (unlikely (!vis_clusters.resize_exact (glyph_شمار_2)))
    FAIL ("Failed to allocate visual clusters.");

  لكل (بیعلامت صحيح i = 0; i < glyph_شمار_2; i++)
    vis_clusters[i] = (uint32_t) -1;
  لكل (بیعلامت صحيح i = 0; i < میانگیر->len; i++)
  {
    بیعلامت صحيح c = clusterMap[میانگیر->info[i].utf16_شاخص ()];
    إذا (likely (c < glyph_شمار_2))
      vis_clusters[c] = hb_min (vis_clusters[c], میانگیر->info[i].cluster);
  }
  لكل (بیعلامت صحيح i = 1; i < glyph_شمار_2; i++)
    إذا (vis_clusters[i] == (uint32_t) -1)
      vis_clusters[i] = vis_clusters[i - 1];
  لكل (صحيح i = (صحيح) glyph_شمار_2 - 2; i >= 0; i--)
    إذا (vis_clusters[i] == (uint32_t) -1)
      vis_clusters[i] = vis_clusters[i + 1];

#لغو_تعریف utf16_شاخص

  إذا (unlikely (!میانگیر->ensure (glyph_شمار_2)))
    FAIL ("Buffer in error");

#لغو_تعریف FAIL

  /* Set glyph infos */
  میانگیر->len = 0;
  لكل (بیعلامت صحيح i = 0; i < glyph_شمار_2; i++)
  {
    hb_glyph_info_t *info = &میانگیر->info[میانگیر->len++];

    info->codepoint = glyphIndices[i];
    info->cluster = vis_clusters[i];

    /* The rest is crap.  Let's store position info there for now. */
    info->ماسک = (hb_ماسک_t) hb_clamp_مقصد<int32_t> (glyphAdvances[i]);
    info->var1.i32 = hb_clamp_مقصد<int32_t> (glyphOffsets[i].advance_جابه_جایی);
    info->var2.i32 = hb_clamp_مقصد<int32_t> (glyphOffsets[i].ascender_جابه_جایی);
  }

  /* Set glyph positions */
  میانگیر->clear_positions ();
  لكل (بیعلامت صحيح i = 0; i < glyph_شمار_2; i++)
  {
    hb_glyph_info_t *info = &میانگیر->info[i];
    hb_glyph_position_t *pos = &میانگیر->pos[i];

    /* TODO vertical */
    pos->x_advance = hb_clamp_مقصد<hb_position_t> (round (x_mult * (int32_t) info->ماسک));
    مزدوج x_جابه_جایی_2 = is_راست_مقصد_چپ ? -(مزدوج) info->var1.i32 : info->var1.i32;
    pos->x_جابه_جایی_2 = hb_clamp_مقصد<hb_position_t> (round ((مزدوج) x_mult * x_جابه_جایی_2));
    pos->y_جابه_جایی_2 = hb_clamp_مقصد<hb_position_t> (round (y_mult * info->var2.i32));
  }

  إذا (is_راست_مقصد_چپ) hb_میانگیر_معکوس (میانگیر);

  میانگیر->clear_glyph_flags ();
  میانگیر->unsafe_مقصد_break ();

  /* Wow, done! */
  أرجع درست;
}


#پایان_اگر
