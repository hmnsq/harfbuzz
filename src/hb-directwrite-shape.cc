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

#جے_تعریف_شدہ HAVE_DIRECTWRITE

#ضمن "hb-shaper-impl.hh"

#ضمن "hb-directwrite.hh"

#ضمن "hb-ms-feature-ranges.hh"


/*
* shaper face data
*/

hb_directwrite_face_انکڑے_t *
hb_directwrite_shaper_face_انکڑے_create (hb_face_t *face)
{
  hb_blob_t *blob = hb_face_reference_blob (face);

  hb_directwrite_face_انکڑے_t *انکڑے = (hb_directwrite_face_انکڑے_t *) dw_face_create (blob, face->اشاریہ_2);

  hb_blob_destroy (blob);

  أرجع انکڑے;
}

فراغ
hb_directwrite_shaper_face_انکڑے_destroy (hb_directwrite_face_انکڑے_t *انکڑے)
{
  ((IDWriteFontFace *) انکڑے)->Release ();
}


/*
 * shaper font data
 */

بنية hb_directwrite_font_انکڑے_t {};

hb_directwrite_font_انکڑے_t *
hb_directwrite_shaper_font_انکڑے_create (hb_font_t *font)
{
  IDWriteFontFace *fontFace = (IDWriteFontFace *) (ثابت فراغ *) font->face->انکڑے.directwrite;

  /*
   * Set up variations.
   */
  IDWriteFontFace5 *fontFaceVariations = خالی_سوچک;
  {
    IDWriteFontFace5 *fontFace5;
    إذا (SUCCEEDED (fontFace->QueryInterface (__uuidof (IDWriteFontFace5), (فراغ **) &fontFace5)))
    {
      IDWriteFontResource *fontResource;
      إذا (SUCCEEDED (fontFace5->GetFontResource (&fontResource)))
      {
	hb_سمتیہ_t<DWRITE_FONT_AXIS_قدر> axis_اقدار;
	إذا (likely (axis_اقدار.resize_exact (font->عدد_coords)))
	{
	  لكل (بنانشانی صحيح i = 0; i < font->عدد_coords; i++)
	  {
	    hb_ot_var_axis_info_t info;
	    بنانشانی صحيح c = 1;
	    hb_ot_var_get_axis_infos (font->face, i, &c, &info);
	    axis_اقدار[i].axis_ٹیگ = (DWRITE_FONT_AXIS_ٹیگ) hb_uint32_وٹاندرا (info.ٹیگ_2);
	    axis_اقدار[i].value = i < font->عدد_coords ?
				   hb_clamp (font->design_coords[i], info.min_قدر, info.max_قدر) :
				   info.default_قدر;
	  }

	  fontResource->CreateFontFace (DWRITE_FONT_SIMULATIONS::DWRITE_FONT_SIMULATIONS_NONE,
					axis_اقدار.arrayZ, axis_اقدار.لمائی, &fontFaceVariations);
	}
	fontResource->Release ();
      }
      fontFace5->Release ();
    }
  }

  أرجع (hb_directwrite_font_انکڑے_t *) fontFaceVariations;
}

فراغ
hb_directwrite_shaper_font_انکڑے_destroy (hb_directwrite_font_انکڑے_t *انکڑے)
{
  ((IDWriteFontFace *) (ثابت فراغ *) انکڑے)->Release ();
}


// Most of TextAnalysis is originally written by Bas Schouten for Mozilla project
// but now is relicensed to MIT for HarfBuzz use
صنف متن_Analysis : عام IDWrite_متن_Analysis_سروت, عام IDWrite_متن_Analysis_Sink
{
خاص:
  hb_reference_گنتی_t m_Ref_گنتی;
عام:
  IFACEMETHOD (QueryInterface) (IID ثابت& iid, باہرلا_3 فراغ** ppObject)
  { أرجع S_OK; }
  IFACEMETHOD_ (ULONG, جوڑو_Ref) ()
  {
    أرجع m_Ref_گنتی.inc () + 1;
  }
  IFACEMETHOD_ (ULONG, Release) ()
  {
    نشانیوالا ref_گنتی = m_Ref_گنتی.dec () - 1;
    أكد (ref_گنتی >= 0);
    إذا (ref_گنتی)
      أرجع ref_گنتی;
    أرجع 0;
  }

  // A single contiguous run of characters containing the same analysis
  // results.
  بنية لگاتار_2
  {
    uint32_t m_متن_شروع;   // starting text position of this run
    uint32_t m_متن_لمائی;  // number of contiguous code units covered
    uint32_t m_Glyph_شروع;  // starting glyph in the glyphs array
    uint32_t m_Glyph_گنتی;  // number of glyphs associated with this run
    //متن
    DWRITE_SCRIPT_ANALYSIS mScript;
    uint8_t mBidiLevel;
    منطقي mIsSideways;

    منطقي شامل_اے_متن_Position (uint32_t a_متن_Position) ثابت
    {
      أرجع a_متن_Position >= m_متن_شروع &&
	     a_متن_Position <  m_متن_شروع + m_متن_لمائی;
    }

    لگاتار_2 *اگلا_لگاتار;
  };

عام:
  متن_Analysis (ثابت wchar_t* متن, uint32_t متن_لمائی_2,
		ثابت wchar_t* localeName, DWRITE_READING_DIRECTION readingDirection)
	       : m_متن_لمائی (متن_لمائی_2), m_متن (متن), mLocaleName (localeName),
		 mReadingDirection (readingDirection), m_Current_لگاتار (خالی_سوچک)
  {
    m_Ref_گنتی.init ();
  }
  ~متن_Analysis ()
  {
    // free runs, except mRunHead which is part of the TextAnalysis object
    لكل (لگاتار_2 *لگاتار = m_لگاتار_سر.اگلا_لگاتار; لگاتار;)
    {
      لگاتار_2 *orig_لگاتار = لگاتار;
      لگاتار = لگاتار->اگلا_لگاتار;
      hb_free (orig_لگاتار);
    }
  }

  STDMETHODIMP
  GenerateResults (IDWrite_متن_Analyzer* متن_Analyzer, لگاتار_2 **لگاتار_سر)
  {
    // Analyzes the text using the script analyzer and returns
    // the result as a series of runs.

    HRESULT hr = S_OK;

    // Initially start out with one result that covers the entire range.
    // This result will be subdivided by the analysis processes.
    m_لگاتار_سر.m_متن_شروع = 0;
    m_لگاتار_سر.m_متن_لمائی = m_متن_لمائی;
    m_لگاتار_سر.mBidiLevel =
      (mReadingDirection == DWRITE_READING_DIRECTION_دایاں_منزل_بایاں);
    m_لگاتار_سر.اگلا_لگاتار = خالی_سوچک;
    m_Current_لگاتار = &m_لگاتار_سر;

    // Call each of the analyzers in sequence, recording their results.
    إذا (SUCCEEDED (hr = متن_Analyzer->AnalyzeScript (هذا, 0, m_متن_لمائی, هذا)))
      *لگاتار_سر = &m_لگاتار_سر;

    أرجع hr;
  }

  // IDWriteTextAnalysisSource implementation

  IFACEMETHODIMP
  Get_متن_At_Position (uint32_t متن_Position,
		     باہرلا_3 wchar_t ثابت** متن_String,
		     باہرلا_3 uint32_t* متن_لمائی_2)
  {
    إذا (متن_Position >= m_متن_لمائی)
    {
      // No text at this position, valid query though.
      *متن_String = خالی_سوچک;
      *متن_لمائی_2 = 0;
    }
    وإلا
    {
      *متن_String = m_متن + متن_Position;
      *متن_لمائی_2 = m_متن_لمائی - متن_Position;
    }
    أرجع S_OK;
  }

  IFACEMETHODIMP
  Get_متن_Before_Position (uint32_t متن_Position,
			 باہرلا_3 wchar_t ثابت** متن_String,
			 باہرلا_3 uint32_t* متن_لمائی_2)
  {
    إذا (متن_Position == 0 || متن_Position > m_متن_لمائی)
    {
      // Either there is no text before here (== 0), or this
      // is an invalid position. The query is considered valid though.
      *متن_String = خالی_سوچک;
      *متن_لمائی_2 = 0;
    }
    وإلا
    {
      *متن_String = m_متن;
      *متن_لمائی_2 = متن_Position;
    }
    أرجع S_OK;
  }

  IFACEMETHODIMP_ (DWRITE_READING_DIRECTION)
  GetParagraphReadingDirection () { أرجع mReadingDirection; }

  IFACEMETHODIMP GetLocaleName (uint32_t متن_Position, uint32_t* متن_لمائی_2,
				wchar_t ثابت** localeName)
  { أرجع S_OK; }

  IFACEMETHODIMP
  Get_نمبر_Substitution (uint32_t متن_Position,
			 باہرلا_3 uint32_t* متن_لمائی_2,
			 باہرلا_3 IDWrite_نمبر_Substitution** نمبر_Substitution)
  {
    // We do not support number substitution.
    *نمبر_Substitution = خالی_سوچک;
    *متن_لمائی_2 = m_متن_لمائی - متن_Position;

    أرجع S_OK;
  }

  // IDWriteTextAnalysisSink implementation

  IFACEMETHODIMP
  سیٹ_Script_Analysis (uint32_t متن_Position, uint32_t متن_لمائی_2,
		     DWRITE_SCRIPT_ANALYSIS ثابت* scriptAnalysis)
  {
    سیٹ_Current_لگاتار (متن_Position);
    إذا (unlikely (!ونڈو_Current_لگاتار (متن_Position)))
      أرجع E_OUTOFMEMORY;
    طالما (متن_لمائی_2 > 0)
    {
      لگاتار_2 *لگاتار = Fetch_اگلا_لگاتار (&متن_لمائی_2);
      إذا (unlikely (!لگاتار))
        أرجع E_OUTOFMEMORY;
      لگاتار->mScript = *scriptAnalysis;
    }

    أرجع S_OK;
  }

  IFACEMETHODIMP
  سیٹ_سطر_Breakpoints (uint32_t متن_Position,
		      uint32_t متن_لمائی_2,
		      ثابت DWRITE_سطر_BREAKPOINT* سطر_Breakpoints)
  { أرجع S_OK; }

  IFACEMETHODIMP سیٹ_Bidi_Level (uint32_t متن_Position, uint32_t متن_لمائی_2,
			       uint8_t explicitLevel, uint8_t resolvedLevel)
  { أرجع S_OK; }

  IFACEMETHODIMP
  سیٹ_نمبر_Substitution (uint32_t متن_Position, uint32_t متن_لمائی_2,
			 IDWrite_نمبر_Substitution* نمبر_Substitution)
  { أرجع S_OK; }

محمي:
  لگاتار_2 *Fetch_اگلا_لگاتار (وچ_2 باہرلا_3 uint32_t* متن_لمائی_2)
  {
    // Used by the sink setters, this returns a reference to the next run.
    // Position and length are adjusted to now point after the current run
    // being returned.

    لگاتار_2 *orig_لگاتار = m_Current_لگاتار;
    // Split the tail if needed (the length remaining is less than the
    // current run's size).
    إذا (*متن_لمائی_2 < m_Current_لگاتار->m_متن_لمائی)
    {
      إذا (unlikely (!ونڈو_Current_لگاتار (m_Current_لگاتار->m_متن_شروع + *متن_لمائی_2)))
        أرجع خالی_سوچک;
    }
    وإلا
      // Just advance the current run.
      m_Current_لگاتار = m_Current_لگاتار->اگلا_لگاتار;
    *متن_لمائی_2 -= orig_لگاتار->m_متن_لمائی;

    // Return a reference to the run that was just current.
    أرجع orig_لگاتار;
  }

  فراغ سیٹ_Current_لگاتار (uint32_t متن_Position)
  {
    // Move the current run to the given position.
    // Since the analyzers generally return results in a forward manner,
    // this will usually just return early. If not, find the
    // corresponding run for the text position.

    إذا (m_Current_لگاتار && m_Current_لگاتار->شامل_اے_متن_Position (متن_Position))
      أرجع;

    لكل (لگاتار_2 *لگاتار = &m_لگاتار_سر; لگاتار; لگاتار = لگاتار->اگلا_لگاتار)
      إذا (لگاتار->شامل_اے_متن_Position (متن_Position))
      {
	m_Current_لگاتار = لگاتار;
	أرجع;
      }
    أكد (0); // We should always be able to find the text position in one of our runs
  }

  منطقي ونڈو_Current_لگاتار (uint32_t ونڈو_Position)
  {
    إذا (!m_Current_لگاتار)
    {
      أكد (0); // SplitCurrentRun called without current run
      // Shouldn't be calling this when no current run is set!
      أرجع خطأ;
    }
    // Split the current run.
    إذا (ونڈو_Position <= m_Current_لگاتار->m_متن_شروع)
    {
      // No need to split, already the start of a run
      // or before it. Usually the first.
      أرجع سچ;
    }
    لگاتار_2 *new_لگاتار = (لگاتار_2 *) hb_malloc (حجم (لگاتار_2));
    إذا (unlikely (!new_لگاتار))
      أرجع خطأ;

    *new_لگاتار = *m_Current_لگاتار;

    // Insert the new run in our linked list.
    new_لگاتار->اگلا_لگاتار = m_Current_لگاتار->اگلا_لگاتار;
    m_Current_لگاتار->اگلا_لگاتار = new_لگاتار;

    // Adjust runs' text positions and lengths.
    uint32_t ونڈو_نقطہ = ونڈو_Position - m_Current_لگاتار->m_متن_شروع;
    new_لگاتار->m_متن_شروع += ونڈو_نقطہ;
    new_لگاتار->m_متن_لمائی -= ونڈو_نقطہ;
    m_Current_لگاتار->m_متن_لمائی = ونڈو_نقطہ;
    m_Current_لگاتار = new_لگاتار;
    أرجع سچ;
  }

محمي:
  //داخلہ
  // (weak references are fine here, since this class is a transient
  //  stack-based helper that doesn't need to copy data)
  uint32_t m_متن_لمائی;
  ثابت wchar_t* m_متن;
  ثابت wchar_t* mLocaleName;
  DWRITE_READING_DIRECTION mReadingDirection;

  // Current processing state.
  لگاتار_2 *m_Current_لگاتار;

  // Output is a list of runs starting here
  لگاتار_2  m_لگاتار_سر;
};

/*
 * shaper
 */

hb_bool_t
_hb_directwrite_shape (hb_shape_plan_t    *shape_plan,
		       hb_font_t          *font,
		       hb_بفر_t        *بفر,
		       ثابت hb_feature_t *features,
		       بنانشانی صحيح        عدد_features)
{
  IDWriteFontFace *fontFace = (IDWriteFontFace *) (ثابت فراغ *) font->انکڑے.directwrite;
  خودکار *global = get_directwrite_global ();
  إذا (unlikely (!global))
    أرجع خطأ;
  IDWriteFactory *dwriteFactory = global->dwriteFactory;

  IDWrite_متن_Analyzer* analyzer;
  dwriteFactory->Create_متن_Analyzer (&analyzer);

  بنانشانی صحيح scratch_ناپ;
  hb_بفر_t::scratch_بفر_t *scratch = بفر->get_scratch_بفر (&scratch_ناپ);
#تعریف_کرو ALLOCATE_ARRAY(Type, name, len) \
  Type *name = (Type *) scratch; \
  افعل { \
    بنانشانی صحيح _consumed = DIV_CEIL ((len) * حجم (Type), حجم (*scratch)); \
    أكد (_consumed <= scratch_ناپ); \
    scratch += _consumed; \
    scratch_ناپ -= _consumed; \
  } طالما (0)

#تعریف_کرو utf16_اشاریہ() var1.u32

  ALLOCATE_ARRAY (wchar_t, متن_String, بفر->len * 2);

  بنانشانی صحيح chars_len = 0;
  لكل (بنانشانی صحيح i = 0; i < بفر->len; i++)
  {
    hb_codepoint_t c = بفر->info[i].codepoint;
    بفر->info[i].utf16_اشاریہ () = chars_len;
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
#تعریف_رد ALLOCATE_ARRAY
  /* Need log_clusters to assign features. */
  chars_len = 0;
  لكل (بنانشانی صحيح i = 0; i < بفر->len; i++)
  {
    hb_codepoint_t c = بفر->info[i].codepoint;
    بنانشانی صحيح cluster = بفر->info[i].cluster;
    log_clusters[chars_len++] = cluster;
    إذا (hb_وچ_range (c, 0x10000u, 0x10FFFFu))
      log_clusters[chars_len++] = cluster; /* Surrogates. */
  }

  DWRITE_READING_DIRECTION readingDirection;
  readingDirection = بفر->props.direction ?
		     DWRITE_READING_DIRECTION_دایاں_منزل_بایاں :
		     DWRITE_READING_DIRECTION_بایاں_منزل_دایاں;

  /*
  * There's an internal 16-bit limit on some things inside the analyzer,
  * but we never attempt to shape a word longer than 64K characters
  * in a single gfxShapedWord, so we cannot exceed that limit.
  */
  uint32_t متن_لمائی_2 = chars_len;

  متن_Analysis analysis (متن_String, متن_لمائی_2, خالی_سوچک, readingDirection);
  متن_Analysis::لگاتار_2 *لگاتار_سر;
  HRESULT hr;
  hr = analysis.GenerateResults (analyzer, &لگاتار_سر);

#تعریف_کرو FAIL(...) \
  HB_STMT_شروع { \
    DEBUG_پیغام (DIRECTWRITE, خالی_سوچک, __VA_ARGS__); \
    أرجع خطأ; \
  } HB_STMT_سرا

  إذا (FAILED (hr))
    FAIL ("Analyzer failed to generate results.");

  uint32_t max_Glyph_گنتی = 3 * متن_لمائی_2 / 2 + 16;
  uint32_t glyph_گنتی_2;
  منطقي is_دایاں_منزل_بایاں = HB_DIRECTION_IS_BACKWARD (بفر->props.direction);

  ثابت wchar_t localeName[20] = {0};
  إذا (بفر->props.language)
    mbstowcs ((wchar_t*) localeName,
	      hb_language_منزل_string (بفر->props.language), 20);

  /*
   * Set up features.
   */
  static_assert ((حجم (DWRITE_TYPOGRAPHIC_FEATURES) == حجم (hb_ms_features_t)), "");
  static_assert ((حجم (DWRITE_FONT_FEATURE) == حجم (hb_ms_feature_t)), "");
  hb_سمتیہ_t<hb_ms_features_t *> range_features;
  hb_سمتیہ_t<uint32_t> range_char_counts;

  // https://github.com/harfbuzz/harfbuzz/pull/5114
  // The data allocated by these two vectors are used by the above two, so they
  // should remain alive as long as the above two are.
  hb_سمتیہ_t<hb_ms_feature_t> feature_ریکارڈاں;
  hb_سمتیہ_t<hb_ms_range_ریکارڈ_t> range_ریکارڈاں;
  إذا (عدد_features)
  {
    إذا (hb_ms_تیاری_features (features, عدد_features, feature_ریکارڈاں, range_ریکارڈاں))
    {
      hb_ms_make_feature_ranges (feature_ریکارڈاں,
				 range_ریکارڈاں,
				 0,
				 chars_len,
				 log_clusters,
				 range_features,
				 range_char_counts);
    }
  }

  hb_سمتیہ_t<uint16_t> clusterMap;
  hb_سمتیہ_t<DWRITE_SHAPING_متن_PROPERTIES> متن_Properties;
  إذا (unlikely (!clusterMap.resize_exact (متن_لمائی_2) ||
                !متن_Properties.resize_exact (متن_لمائی_2)))
    FAIL ("Failed to allocate shaping text data.");

  hb_سمتیہ_t<uint16_t> glyphIndices;
  hb_سمتیہ_t<DWRITE_SHAPING_GLYPH_PROPERTIES> glyphProperties;

retry_getglyphs:
  إذا (unlikely (!glyphIndices.resize_exact (max_Glyph_گنتی) ||
                !glyphProperties.resize_exact (max_Glyph_گنتی)))
    FAIL ("Failed to allocate shaping glyph data.");

  hr = analyzer->GetGlyphs (متن_String,
			    chars_len,
			    fontFace,
			    خطأ,
			    is_دایاں_منزل_بایاں,
			    &لگاتار_سر->mScript,
			    localeName,
			    خالی_سوچک,
			    (ثابت DWRITE_TYPOGRAPHIC_FEATURES**) range_features.arrayZ,
			    range_char_counts.arrayZ,
			    range_features.لمائی,
			    max_Glyph_گنتی,
			    clusterMap.arrayZ,
			    متن_Properties.arrayZ,
			    glyphIndices.arrayZ,
			    glyphProperties.arrayZ,
			    &glyph_گنتی_2);

  إذا (unlikely (hr == HRESULT_FROM_WIN32 (ERROR_INSUFFICIENT_بفر)))
  {
    max_Glyph_گنتی *= 2;

    اذهب retry_getglyphs;
  }
  إذا (FAILED (hr))
    FAIL ("Analyzer failed to get glyphs.");

  hb_سمتیہ_t<عائم> glyphAdvances;
  hb_سمتیہ_t<DWRITE_GLYPH_وتھ> glyphOffsets;
  إذا (unlikely (!glyphAdvances.resize_exact (max_Glyph_گنتی) ||
                !glyphOffsets.resize_exact (max_Glyph_گنتی)))
    FAIL ("Failed to allocate glyph positioning data.");


  بنانشانی font_Em_ناپ = font->face->get_upem ();

  عائم x_mult = font->x_multf;
  عائم y_mult = font->y_multf;

  hr = analyzer->GetGlyphPlacements (متن_String,
				     clusterMap.arrayZ,
				     متن_Properties.arrayZ,
				     chars_len,
				     glyphIndices.arrayZ,
				     glyphProperties.arrayZ,
				     glyph_گنتی_2,
				     fontFace,
				     font_Em_ناپ,
				     خطأ,
				     is_دایاں_منزل_بایاں,
				     &لگاتار_سر->mScript,
				     localeName,
				     (ثابت DWRITE_TYPOGRAPHIC_FEATURES**) range_features.arrayZ,
				     range_char_counts.arrayZ,
				     range_features.لمائی,
				     glyphAdvances.arrayZ,
				     glyphOffsets.arrayZ);

  إذا (FAILED (hr))
    FAIL ("Analyzer failed to get glyph placements.");

  /* Ok, we've got everything we need, now compose output buffer,
   * very, *very*, carefully! */

  /* Calculate visual-clusters.  That's what we ship. */
  hb_سمتیہ_t<uint32_t> vis_clusters;
  إذا (unlikely (!vis_clusters.resize_exact (glyph_گنتی_2)))
    FAIL ("Failed to allocate visual clusters.");

  لكل (بنانشانی صحيح i = 0; i < glyph_گنتی_2; i++)
    vis_clusters[i] = (uint32_t) -1;
  لكل (بنانشانی صحيح i = 0; i < بفر->len; i++)
  {
    بنانشانی صحيح c = clusterMap[بفر->info[i].utf16_اشاریہ ()];
    إذا (likely (c < glyph_گنتی_2))
      vis_clusters[c] = hb_min (vis_clusters[c], بفر->info[i].cluster);
  }
  لكل (بنانشانی صحيح i = 1; i < glyph_گنتی_2; i++)
    إذا (vis_clusters[i] == (uint32_t) -1)
      vis_clusters[i] = vis_clusters[i - 1];
  لكل (صحيح i = (صحيح) glyph_گنتی_2 - 2; i >= 0; i--)
    إذا (vis_clusters[i] == (uint32_t) -1)
      vis_clusters[i] = vis_clusters[i + 1];

#تعریف_رد utf16_اشاریہ

  إذا (unlikely (!بفر->ensure (glyph_گنتی_2)))
    FAIL ("Buffer in error");

#تعریف_رد FAIL

  /* Set glyph infos */
  بفر->len = 0;
  لكل (بنانشانی صحيح i = 0; i < glyph_گنتی_2; i++)
  {
    hb_glyph_info_t *info = &بفر->info[بفر->len++];

    info->codepoint = glyphIndices[i];
    info->cluster = vis_clusters[i];

    /* The rest is crap.  Let's store position info there for now. */
    info->نقاب = (hb_نقاب_t) hb_clamp_منزل<int32_t> (glyphAdvances[i]);
    info->var1.i32 = hb_clamp_منزل<int32_t> (glyphOffsets[i].advance_وتھ);
    info->var2.i32 = hb_clamp_منزل<int32_t> (glyphOffsets[i].ascender_وتھ);
  }

  /* Set glyph positions */
  بفر->clear_positions ();
  لكل (بنانشانی صحيح i = 0; i < glyph_گنتی_2; i++)
  {
    hb_glyph_info_t *info = &بفر->info[i];
    hb_glyph_position_t *pos = &بفر->pos[i];

    /* TODO vertical */
    pos->x_advance = hb_clamp_منزل<hb_position_t> (round (x_mult * (int32_t) info->نقاب));
    مزدوج x_وتھ_2 = is_دایاں_منزل_بایاں ? -(مزدوج) info->var1.i32 : info->var1.i32;
    pos->x_وتھ_2 = hb_clamp_منزل<hb_position_t> (round ((مزدوج) x_mult * x_وتھ_2));
    pos->y_وتھ_2 = hb_clamp_منزل<hb_position_t> (round (y_mult * info->var2.i32));
  }

  إذا (is_دایاں_منزل_بایاں) hb_بفر_الٹ (بفر);

  بفر->clear_glyph_flags ();
  بفر->unsafe_منزل_break ();

  /* Wow, done! */
  أرجع سچ;
}


#جے_انت
