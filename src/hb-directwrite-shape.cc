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

#که_تعریف_شوی HAVE_DIRECTWRITE

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

  hb_directwrite_face_معلومات_t *معلومات = (hb_directwrite_face_معلومات_t *) dw_face_create (blob, face->شاخص_2);

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
  IDWriteFontFace5 *fontFaceVariations = تش_اشاره;
  {
    IDWriteFontFace5 *fontFace5;
    إذا (SUCCEEDED (fontFace->QueryInterface (__uuidof (IDWriteFontFace5), (فراغ **) &fontFace5)))
    {
      IDWriteFontResource *fontResource;
      إذا (SUCCEEDED (fontFace5->GetFontResource (&fontResource)))
      {
	hb_ویکټور_t<DWRITE_FONT_AXIS_ارزښت> axis_ارزښتونه;
	إذا (likely (axis_ارزښتونه.resize_exact (font->عدد_coords)))
	{
	  لكل (بینښه صحيح i = 0; i < font->عدد_coords; i++)
	  {
	    hb_ot_var_axis_info_t info;
	    بینښه صحيح c = 1;
	    hb_ot_var_get_axis_infos (font->face, i, &c, &info);
	    axis_ارزښتونه[i].axis_نښان = (DWRITE_FONT_AXIS_نښان) hb_uint32_بدلون (info.نښان_2);
	    axis_ارزښتونه[i].value = i < font->عدد_coords ?
				   hb_clamp (font->design_coords[i], info.min_ارزښت, info.max_ارزښت) :
				   info.default_ارزښت;
	  }

	  fontResource->CreateFontFace (DWRITE_FONT_SIMULATIONS::DWRITE_FONT_SIMULATIONS_NONE,
					axis_ارزښتونه.arrayZ, axis_ارزښتونه.اوږدتیا, &fontFaceVariations);
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
صنف متن_Analysis : عام IDWrite_متن_Analysis_سرچینه, عام IDWrite_متن_Analysis_Sink
{
خاص:
  hb_reference_شمېر_t m_Ref_شمېر;
عام:
  IFACEMETHOD (QueryInterface) (IID ثابت& iid, بهر_3 فراغ** ppObject)
  { أرجع S_OK; }
  IFACEMETHOD_ (ULONG, جمع_کړه_Ref) ()
  {
    أرجع m_Ref_شمېر.inc () + 1;
  }
  IFACEMETHOD_ (ULONG, Release) ()
  {
    نښهدار ref_شمېر = m_Ref_شمېر.dec () - 1;
    أكد (ref_شمېر >= 0);
    إذا (ref_شمېر)
      أرجع ref_شمېر;
    أرجع 0;
  }

  // A single contiguous run of characters containing the same analysis
  // results.
  بنية پرلپسې_2
  {
    uint32_t m_متن_پیل;   // starting text position of this run
    uint32_t m_متن_اوږدتیا;  // number of contiguous code units covered
    uint32_t m_Glyph_پیل;  // starting glyph in the glyphs array
    uint32_t m_Glyph_شمېر;  // number of glyphs associated with this run
    //متن
    DWRITE_SCRIPT_ANALYSIS mScript;
    uint8_t mBidiLevel;
    منطقي mIsSideways;

    منطقي لري_متن_Position (uint32_t a_متن_Position) ثابت
    {
      أرجع a_متن_Position >= m_متن_پیل &&
	     a_متن_Position <  m_متن_پیل + m_متن_اوږدتیا;
    }

    پرلپسې_2 *بل_پرلپسې;
  };

عام:
  متن_Analysis (ثابت wchar_t* متن, uint32_t متن_اوږدتیا_2,
		ثابت wchar_t* localeName, DWRITE_READING_DIRECTION readingDirection)
	       : m_متن_اوږدتیا (متن_اوږدتیا_2), m_متن (متن), mLocaleName (localeName),
		 mReadingDirection (readingDirection), m_Current_پرلپسې (تش_اشاره)
  {
    m_Ref_شمېر.init ();
  }
  ~متن_Analysis ()
  {
    // free runs, except mRunHead which is part of the TextAnalysis object
    لكل (پرلپسې_2 *پرلپسې = m_پرلپسې_سر.بل_پرلپسې; پرلپسې;)
    {
      پرلپسې_2 *orig_پرلپسې = پرلپسې;
      پرلپسې = پرلپسې->بل_پرلپسې;
      hb_free (orig_پرلپسې);
    }
  }

  STDMETHODIMP
  GenerateResults (IDWrite_متن_Analyzer* متن_Analyzer, پرلپسې_2 **پرلپسې_سر)
  {
    // Analyzes the text using the script analyzer and returns
    // the result as a series of runs.

    HRESULT hr = S_OK;

    // Initially start out with one result that covers the entire range.
    // This result will be subdivided by the analysis processes.
    m_پرلپسې_سر.m_متن_پیل = 0;
    m_پرلپسې_سر.m_متن_اوږدتیا = m_متن_اوږدتیا;
    m_پرلپسې_سر.mBidiLevel =
      (mReadingDirection == DWRITE_READING_DIRECTION_ښی_منزل_کیڼ);
    m_پرلپسې_سر.بل_پرلپسې = تش_اشاره;
    m_Current_پرلپسې = &m_پرلپسې_سر;

    // Call each of the analyzers in sequence, recording their results.
    إذا (SUCCEEDED (hr = متن_Analyzer->AnalyzeScript (هذا, 0, m_متن_اوږدتیا, هذا)))
      *پرلپسې_سر = &m_پرلپسې_سر;

    أرجع hr;
  }

  // IDWriteTextAnalysisSource implementation

  IFACEMETHODIMP
  Get_متن_At_Position (uint32_t متن_Position,
		     بهر_3 wchar_t ثابت** متن_String,
		     بهر_3 uint32_t* متن_اوږدتیا_2)
  {
    إذا (متن_Position >= m_متن_اوږدتیا)
    {
      // No text at this position, valid query though.
      *متن_String = تش_اشاره;
      *متن_اوږدتیا_2 = 0;
    }
    وإلا
    {
      *متن_String = m_متن + متن_Position;
      *متن_اوږدتیا_2 = m_متن_اوږدتیا - متن_Position;
    }
    أرجع S_OK;
  }

  IFACEMETHODIMP
  Get_متن_Before_Position (uint32_t متن_Position,
			 بهر_3 wchar_t ثابت** متن_String,
			 بهر_3 uint32_t* متن_اوږدتیا_2)
  {
    إذا (متن_Position == 0 || متن_Position > m_متن_اوږدتیا)
    {
      // Either there is no text before here (== 0), or this
      // is an invalid position. The query is considered valid though.
      *متن_String = تش_اشاره;
      *متن_اوږدتیا_2 = 0;
    }
    وإلا
    {
      *متن_String = m_متن;
      *متن_اوږدتیا_2 = متن_Position;
    }
    أرجع S_OK;
  }

  IFACEMETHODIMP_ (DWRITE_READING_DIRECTION)
  GetParagraphReadingDirection () { أرجع mReadingDirection; }

  IFACEMETHODIMP GetLocaleName (uint32_t متن_Position, uint32_t* متن_اوږدتیا_2,
				wchar_t ثابت** localeName)
  { أرجع S_OK; }

  IFACEMETHODIMP
  Get_شمېره_Substitution (uint32_t متن_Position,
			 بهر_3 uint32_t* متن_اوږدتیا_2,
			 بهر_3 IDWrite_شمېره_Substitution** شمېره_Substitution)
  {
    // We do not support number substitution.
    *شمېره_Substitution = تش_اشاره;
    *متن_اوږدتیا_2 = m_متن_اوږدتیا - متن_Position;

    أرجع S_OK;
  }

  // IDWriteTextAnalysisSink implementation

  IFACEMETHODIMP
  سېټ_Script_Analysis (uint32_t متن_Position, uint32_t متن_اوږدتیا_2,
		     DWRITE_SCRIPT_ANALYSIS ثابت* scriptAnalysis)
  {
    سېټ_Current_پرلپسې (متن_Position);
    إذا (unlikely (!وېشل_Current_پرلپسې (متن_Position)))
      أرجع E_OUTOFMEMORY;
    طالما (متن_اوږدتیا_2 > 0)
    {
      پرلپسې_2 *پرلپسې = Fetch_بل_پرلپسې (&متن_اوږدتیا_2);
      إذا (unlikely (!پرلپسې))
        أرجع E_OUTOFMEMORY;
      پرلپسې->mScript = *scriptAnalysis;
    }

    أرجع S_OK;
  }

  IFACEMETHODIMP
  سېټ_کرښه_Breakpoints (uint32_t متن_Position,
		      uint32_t متن_اوږدتیا_2,
		      ثابت DWRITE_کرښه_BREAKPOINT* کرښه_Breakpoints)
  { أرجع S_OK; }

  IFACEMETHODIMP سېټ_Bidi_Level (uint32_t متن_Position, uint32_t متن_اوږدتیا_2,
			       uint8_t explicitLevel, uint8_t resolvedLevel)
  { أرجع S_OK; }

  IFACEMETHODIMP
  سېټ_شمېره_Substitution (uint32_t متن_Position, uint32_t متن_اوږدتیا_2,
			 IDWrite_شمېره_Substitution* شمېره_Substitution)
  { أرجع S_OK; }

محمي:
  پرلپسې_2 *Fetch_بل_پرلپسې (دننه_2 بهر_3 uint32_t* متن_اوږدتیا_2)
  {
    // Used by the sink setters, this returns a reference to the next run.
    // Position and length are adjusted to now point after the current run
    // being returned.

    پرلپسې_2 *orig_پرلپسې = m_Current_پرلپسې;
    // Split the tail if needed (the length remaining is less than the
    // current run's size).
    إذا (*متن_اوږدتیا_2 < m_Current_پرلپسې->m_متن_اوږدتیا)
    {
      إذا (unlikely (!وېشل_Current_پرلپسې (m_Current_پرلپسې->m_متن_پیل + *متن_اوږدتیا_2)))
        أرجع تش_اشاره;
    }
    وإلا
      // Just advance the current run.
      m_Current_پرلپسې = m_Current_پرلپسې->بل_پرلپسې;
    *متن_اوږدتیا_2 -= orig_پرلپسې->m_متن_اوږدتیا;

    // Return a reference to the run that was just current.
    أرجع orig_پرلپسې;
  }

  فراغ سېټ_Current_پرلپسې (uint32_t متن_Position)
  {
    // Move the current run to the given position.
    // Since the analyzers generally return results in a forward manner,
    // this will usually just return early. If not, find the
    // corresponding run for the text position.

    إذا (m_Current_پرلپسې && m_Current_پرلپسې->لري_متن_Position (متن_Position))
      أرجع;

    لكل (پرلپسې_2 *پرلپسې = &m_پرلپسې_سر; پرلپسې; پرلپسې = پرلپسې->بل_پرلپسې)
      إذا (پرلپسې->لري_متن_Position (متن_Position))
      {
	m_Current_پرلپسې = پرلپسې;
	أرجع;
      }
    أكد (0); // We should always be able to find the text position in one of our runs
  }

  منطقي وېشل_Current_پرلپسې (uint32_t وېشل_Position)
  {
    إذا (!m_Current_پرلپسې)
    {
      أكد (0); // SplitCurrentRun called without current run
      // Shouldn't be calling this when no current run is set!
      أرجع خطأ;
    }
    // Split the current run.
    إذا (وېشل_Position <= m_Current_پرلپسې->m_متن_پیل)
    {
      // No need to split, already the start of a run
      // or before it. Usually the first.
      أرجع ریښتیا;
    }
    پرلپسې_2 *new_پرلپسې = (پرلپسې_2 *) hb_malloc (حجم (پرلپسې_2));
    إذا (unlikely (!new_پرلپسې))
      أرجع خطأ;

    *new_پرلپسې = *m_Current_پرلپسې;

    // Insert the new run in our linked list.
    new_پرلپسې->بل_پرلپسې = m_Current_پرلپسې->بل_پرلپسې;
    m_Current_پرلپسې->بل_پرلپسې = new_پرلپسې;

    // Adjust runs' text positions and lengths.
    uint32_t وېشل_ټکی = وېشل_Position - m_Current_پرلپسې->m_متن_پیل;
    new_پرلپسې->m_متن_پیل += وېشل_ټکی;
    new_پرلپسې->m_متن_اوږدتیا -= وېشل_ټکی;
    m_Current_پرلپسې->m_متن_اوږدتیا = وېشل_ټکی;
    m_Current_پرلپسې = new_پرلپسې;
    أرجع ریښتیا;
  }

محمي:
  //ننوت
  // (weak references are fine here, since this class is a transient
  //  stack-based helper that doesn't need to copy data)
  uint32_t m_متن_اوږدتیا;
  ثابت wchar_t* m_متن;
  ثابت wchar_t* mLocaleName;
  DWRITE_READING_DIRECTION mReadingDirection;

  // Current processing state.
  پرلپسې_2 *m_Current_پرلپسې;

  // Output is a list of runs starting here
  پرلپسې_2  m_پرلپسې_سر;
};

/*
 * shaper
 */

hb_bool_t
_hb_directwrite_shape (hb_shape_plan_t    *shape_plan,
		       hb_font_t          *font,
		       hb_بفر_t        *بفر,
		       ثابت hb_feature_t *features,
		       بینښه صحيح        عدد_features)
{
  IDWriteFontFace *fontFace = (IDWriteFontFace *) (ثابت فراغ *) font->معلومات.directwrite;
  خپلکاری *global = get_directwrite_global ();
  إذا (unlikely (!global))
    أرجع خطأ;
  IDWriteFactory *dwriteFactory = global->dwriteFactory;

  IDWrite_متن_Analyzer* analyzer;
  dwriteFactory->Create_متن_Analyzer (&analyzer);

  بینښه صحيح scratch_اندازه;
  hb_بفر_t::scratch_بفر_t *scratch = بفر->get_scratch_بفر (&scratch_اندازه);
#تعریف_کړه ALLOCATE_ARRAY(Type, name, len) \
  Type *name = (Type *) scratch; \
  افعل { \
    بینښه صحيح _consumed = DIV_CEIL ((len) * حجم (Type), حجم (*scratch)); \
    أكد (_consumed <= scratch_اندازه); \
    scratch += _consumed; \
    scratch_اندازه -= _consumed; \
  } طالما (0)

#تعریف_کړه utf16_شاخص() var1.u32

  ALLOCATE_ARRAY (wchar_t, متن_String, بفر->len * 2);

  بینښه صحيح chars_len = 0;
  لكل (بینښه صحيح i = 0; i < بفر->len; i++)
  {
    hb_codepoint_t c = بفر->info[i].codepoint;
    بفر->info[i].utf16_شاخص () = chars_len;
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
#تعریف_لغوه ALLOCATE_ARRAY
  /* Need log_clusters to assign features. */
  chars_len = 0;
  لكل (بینښه صحيح i = 0; i < بفر->len; i++)
  {
    hb_codepoint_t c = بفر->info[i].codepoint;
    بینښه صحيح cluster = بفر->info[i].cluster;
    log_clusters[chars_len++] = cluster;
    إذا (hb_دننه_range (c, 0x10000u, 0x10FFFFu))
      log_clusters[chars_len++] = cluster; /* Surrogates. */
  }

  DWRITE_READING_DIRECTION readingDirection;
  readingDirection = بفر->props.direction ?
		     DWRITE_READING_DIRECTION_ښی_منزل_کیڼ :
		     DWRITE_READING_DIRECTION_کیڼ_منزل_ښی;

  /*
  * There's an internal 16-bit limit on some things inside the analyzer,
  * but we never attempt to shape a word longer than 64K characters
  * in a single gfxShapedWord, so we cannot exceed that limit.
  */
  uint32_t متن_اوږدتیا_2 = chars_len;

  متن_Analysis analysis (متن_String, متن_اوږدتیا_2, تش_اشاره, readingDirection);
  متن_Analysis::پرلپسې_2 *پرلپسې_سر;
  HRESULT hr;
  hr = analysis.GenerateResults (analyzer, &پرلپسې_سر);

#تعریف_کړه FAIL(...) \
  HB_STMT_پیل { \
    DEBUG_پیغام (DIRECTWRITE, تش_اشاره, __VA_ARGS__); \
    أرجع خطأ; \
  } HB_STMT_پای_ټکی

  إذا (FAILED (hr))
    FAIL ("Analyzer failed to generate results.");

  uint32_t max_Glyph_شمېر = 3 * متن_اوږدتیا_2 / 2 + 16;
  uint32_t glyph_شمېر_2;
  منطقي is_ښی_منزل_کیڼ = HB_DIRECTION_IS_BACKWARD (بفر->props.direction);

  ثابت wchar_t localeName[20] = {0};
  إذا (بفر->props.language)
    mbstowcs ((wchar_t*) localeName,
	      hb_language_منزل_string (بفر->props.language), 20);

  /*
   * Set up features.
   */
  static_assert ((حجم (DWRITE_TYPOGRAPHIC_FEATURES) == حجم (hb_ms_features_t)), "");
  static_assert ((حجم (DWRITE_FONT_FEATURE) == حجم (hb_ms_feature_t)), "");
  hb_ویکټور_t<hb_ms_features_t *> range_features;
  hb_ویکټور_t<uint32_t> range_char_counts;

  // https://github.com/harfbuzz/harfbuzz/pull/5114
  // The data allocated by these two vectors are used by the above two, so they
  // should remain alive as long as the above two are.
  hb_ویکټور_t<hb_ms_feature_t> feature_ریکارډونه;
  hb_ویکټور_t<hb_ms_range_ریکارډ_t> range_ریکارډونه;
  إذا (عدد_features)
  {
    إذا (hb_ms_تیاری_features (features, عدد_features, feature_ریکارډونه, range_ریکارډونه))
    {
      hb_ms_make_feature_ranges (feature_ریکارډونه,
				 range_ریکارډونه,
				 0,
				 chars_len,
				 log_clusters,
				 range_features,
				 range_char_counts);
    }
  }

  hb_ویکټور_t<uint16_t> clusterMap;
  hb_ویکټور_t<DWRITE_SHAPING_متن_PROPERTIES> متن_Properties;
  إذا (unlikely (!clusterMap.resize_exact (متن_اوږدتیا_2) ||
                !متن_Properties.resize_exact (متن_اوږدتیا_2)))
    FAIL ("Failed to allocate shaping text data.");

  hb_ویکټور_t<uint16_t> glyphIndices;
  hb_ویکټور_t<DWRITE_SHAPING_GLYPH_PROPERTIES> glyphProperties;

retry_getglyphs:
  إذا (unlikely (!glyphIndices.resize_exact (max_Glyph_شمېر) ||
                !glyphProperties.resize_exact (max_Glyph_شمېر)))
    FAIL ("Failed to allocate shaping glyph data.");

  hr = analyzer->GetGlyphs (متن_String,
			    chars_len,
			    fontFace,
			    خطأ,
			    is_ښی_منزل_کیڼ,
			    &پرلپسې_سر->mScript,
			    localeName,
			    تش_اشاره,
			    (ثابت DWRITE_TYPOGRAPHIC_FEATURES**) range_features.arrayZ,
			    range_char_counts.arrayZ,
			    range_features.اوږدتیا,
			    max_Glyph_شمېر,
			    clusterMap.arrayZ,
			    متن_Properties.arrayZ,
			    glyphIndices.arrayZ,
			    glyphProperties.arrayZ,
			    &glyph_شمېر_2);

  إذا (unlikely (hr == HRESULT_FROM_WIN32 (ERROR_INSUFFICIENT_بفر)))
  {
    max_Glyph_شمېر *= 2;

    اذهب retry_getglyphs;
  }
  إذا (FAILED (hr))
    FAIL ("Analyzer failed to get glyphs.");

  hb_ویکټور_t<عائم> glyphAdvances;
  hb_ویکټور_t<DWRITE_GLYPH_دننه_واټن> glyphOffsets;
  إذا (unlikely (!glyphAdvances.resize_exact (max_Glyph_شمېر) ||
                !glyphOffsets.resize_exact (max_Glyph_شمېر)))
    FAIL ("Failed to allocate glyph positioning data.");


  بینښه font_Em_اندازه = font->face->get_upem ();

  عائم x_mult = font->x_multf;
  عائم y_mult = font->y_multf;

  hr = analyzer->GetGlyphPlacements (متن_String,
				     clusterMap.arrayZ,
				     متن_Properties.arrayZ,
				     chars_len,
				     glyphIndices.arrayZ,
				     glyphProperties.arrayZ,
				     glyph_شمېر_2,
				     fontFace,
				     font_Em_اندازه,
				     خطأ,
				     is_ښی_منزل_کیڼ,
				     &پرلپسې_سر->mScript,
				     localeName,
				     (ثابت DWRITE_TYPOGRAPHIC_FEATURES**) range_features.arrayZ,
				     range_char_counts.arrayZ,
				     range_features.اوږدتیا,
				     glyphAdvances.arrayZ,
				     glyphOffsets.arrayZ);

  إذا (FAILED (hr))
    FAIL ("Analyzer failed to get glyph placements.");

  /* Ok, we've got everything we need, now compose output buffer,
   * very, *very*, carefully! */

  /* Calculate visual-clusters.  That's what we ship. */
  hb_ویکټور_t<uint32_t> vis_clusters;
  إذا (unlikely (!vis_clusters.resize_exact (glyph_شمېر_2)))
    FAIL ("Failed to allocate visual clusters.");

  لكل (بینښه صحيح i = 0; i < glyph_شمېر_2; i++)
    vis_clusters[i] = (uint32_t) -1;
  لكل (بینښه صحيح i = 0; i < بفر->len; i++)
  {
    بینښه صحيح c = clusterMap[بفر->info[i].utf16_شاخص ()];
    إذا (likely (c < glyph_شمېر_2))
      vis_clusters[c] = hb_min (vis_clusters[c], بفر->info[i].cluster);
  }
  لكل (بینښه صحيح i = 1; i < glyph_شمېر_2; i++)
    إذا (vis_clusters[i] == (uint32_t) -1)
      vis_clusters[i] = vis_clusters[i - 1];
  لكل (صحيح i = (صحيح) glyph_شمېر_2 - 2; i >= 0; i--)
    إذا (vis_clusters[i] == (uint32_t) -1)
      vis_clusters[i] = vis_clusters[i + 1];

#تعریف_لغوه utf16_شاخص

  إذا (unlikely (!بفر->ensure (glyph_شمېر_2)))
    FAIL ("Buffer in error");

#تعریف_لغوه FAIL

  /* Set glyph infos */
  بفر->len = 0;
  لكل (بینښه صحيح i = 0; i < glyph_شمېر_2; i++)
  {
    hb_glyph_info_t *info = &بفر->info[بفر->len++];

    info->codepoint = glyphIndices[i];
    info->cluster = vis_clusters[i];

    /* The rest is crap.  Let's store position info there for now. */
    info->ماسک = (hb_ماسک_t) hb_clamp_منزل<int32_t> (glyphAdvances[i]);
    info->var1.i32 = hb_clamp_منزل<int32_t> (glyphOffsets[i].advance_دننه_واټن);
    info->var2.i32 = hb_clamp_منزل<int32_t> (glyphOffsets[i].ascender_دننه_واټن);
  }

  /* Set glyph positions */
  بفر->clear_positions ();
  لكل (بینښه صحيح i = 0; i < glyph_شمېر_2; i++)
  {
    hb_glyph_info_t *info = &بفر->info[i];
    hb_glyph_position_t *pos = &بفر->pos[i];

    /* TODO vertical */
    pos->x_advance = hb_clamp_منزل<hb_position_t> (round (x_mult * (int32_t) info->ماسک));
    مزدوج x_دننه_واټن_2 = is_ښی_منزل_کیڼ ? -(مزدوج) info->var1.i32 : info->var1.i32;
    pos->x_دننه_واټن_2 = hb_clamp_منزل<hb_position_t> (round ((مزدوج) x_mult * x_دننه_واټن_2));
    pos->y_دننه_واټن_2 = hb_clamp_منزل<hb_position_t> (round (y_mult * info->var2.i32));
  }

  إذا (is_ښی_منزل_کیڼ) hb_بفر_سرچپه (بفر);

  بفر->clear_glyph_flags ();
  بفر->unsafe_منزل_break ();

  /* Wow, done! */
  أرجع ریښتیا;
}


#د_که_پای
