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

#شامل "hb.hh"

#اگر_تعریف_شدہ HAVE_DIRECTWRITE

#شامل "hb-shaper-impl.hh"

#شامل "hb-directwrite.hh"

#شامل "hb-ms-feature-ranges.hh"


/*
* shaper face data
*/

hb_directwrite_face_ڈیٹا_t *
hb_directwrite_shaper_face_ڈیٹا_create (hb_face_t *face)
{
  hb_blob_t *blob = hb_face_reference_blob (face);

  hb_directwrite_face_ڈیٹا_t *ڈیٹا = (hb_directwrite_face_ڈیٹا_t *) dw_face_create (blob, face->اشاریہ_2);

  hb_blob_destroy (blob);

  واپس ڈیٹا;
}

خالی
hb_directwrite_shaper_face_ڈیٹا_destroy (hb_directwrite_face_ڈیٹا_t *ڈیٹا)
{
  ((IDWriteFontFace *) ڈیٹا)->Release ();
}


/*
 * shaper font data
 */

ساخت hb_directwrite_font_ڈیٹا_t {};

hb_directwrite_font_ڈیٹا_t *
hb_directwrite_shaper_font_ڈیٹا_create (hb_font_t *font)
{
  IDWriteFontFace *fontFace = (IDWriteFontFace *) (ثابت خالی *) font->face->ڈیٹا.directwrite;

  /*
   * Set up variations.
   */
  IDWriteFontFace5 *fontFaceVariations = خالی_اشاریہ;
  {
    IDWriteFontFace5 *fontFace5;
    اگر (SUCCEEDED (fontFace->QueryInterface (__uuidof (IDWriteFontFace5), (خالی **) &fontFace5)))
    {
      IDWriteFontResource *fontResource;
      اگر (SUCCEEDED (fontFace5->GetFontResource (&fontResource)))
      {
	hb_سمتیہ_t<DWRITE_FONT_AXIS_قدر> axis_اقدار;
	اگر (likely (axis_اقدار.resize_exact (font->عدد_coords)))
	{
	  برائے (غیرعلامتی صحیح i = 0; i < font->عدد_coords; i++)
	  {
	    hb_ot_var_axis_info_t info;
	    غیرعلامتی صحیح c = 1;
	    hb_ot_var_get_axis_infos (font->face, i, &c, &info);
	    axis_اقدار[i].axis_لیبل = (DWRITE_FONT_AXIS_لیبل) hb_uint32_تبادلہ (info.لیبل_2);
	    axis_اقدار[i].value = i < font->عدد_coords ?
				   hb_clamp (font->design_coords[i], info.min_قدر, info.max_قدر) :
				   info.default_قدر;
	  }

	  fontResource->CreateFontFace (DWRITE_FONT_SIMULATIONS::DWRITE_FONT_SIMULATIONS_NONE,
					axis_اقدار.arrayZ, axis_اقدار.لمبائی, &fontFaceVariations);
	}
	fontResource->Release ();
      }
      fontFace5->Release ();
    }
  }

  واپس (hb_directwrite_font_ڈیٹا_t *) fontFaceVariations;
}

خالی
hb_directwrite_shaper_font_ڈیٹا_destroy (hb_directwrite_font_ڈیٹا_t *ڈیٹا)
{
  ((IDWriteFontFace *) (ثابت خالی *) ڈیٹا)->Release ();
}


// Most of TextAnalysis is originally written by Bas Schouten for Mozilla project
// but now is relicensed to MIT for HarfBuzz use
جماعت متن_Analysis : عوامی IDWrite_متن_Analysis_ماخذ, عوامی IDWrite_متن_Analysis_Sink
{
نجی:
  hb_reference_گنتی_t m_Ref_گنتی;
عوامی:
  IFACEMETHOD (QueryInterface) (IID ثابت& iid, برآمد_3 خالی** ppObject)
  { واپس S_OK; }
  IFACEMETHOD_ (ULONG, جوڑو_Ref) ()
  {
    واپس m_Ref_گنتی.inc () + 1;
  }
  IFACEMETHOD_ (ULONG, Release) ()
  {
    علامتی ref_گنتی = m_Ref_گنتی.dec () - 1;
    أكد (ref_گنتی >= 0);
    اگر (ref_گنتی)
      واپس ref_گنتی;
    واپس 0;
  }

  // A single contiguous run of characters containing the same analysis
  // results.
  ساخت تسلسل_2
  {
    uint32_t m_متن_آغاز;   // starting text position of this run
    uint32_t m_متن_لمبائی;  // number of contiguous code units covered
    uint32_t m_Glyph_آغاز;  // starting glyph in the glyphs array
    uint32_t m_Glyph_گنتی;  // number of glyphs associated with this run
    //متن
    DWRITE_SCRIPT_ANALYSIS mScript;
    uint8_t mBidiLevel;
    منطقی mIsSideways;

    منطقی شامل_ہے_متن_Position (uint32_t a_متن_Position) ثابت
    {
      واپس a_متن_Position >= m_متن_آغاز &&
	     a_متن_Position <  m_متن_آغاز + m_متن_لمبائی;
    }

    تسلسل_2 *اگلا_تسلسل;
  };

عوامی:
  متن_Analysis (ثابت wchar_t* متن, uint32_t متن_لمبائی_2,
		ثابت wchar_t* localeName, DWRITE_READING_DIRECTION readingDirection)
	       : m_متن_لمبائی (متن_لمبائی_2), m_متن (متن), mLocaleName (localeName),
		 mReadingDirection (readingDirection), m_Current_تسلسل (خالی_اشاریہ)
  {
    m_Ref_گنتی.init ();
  }
  ~متن_Analysis ()
  {
    // free runs, except mRunHead which is part of the TextAnalysis object
    برائے (تسلسل_2 *تسلسل = m_تسلسل_سر.اگلا_تسلسل; تسلسل;)
    {
      تسلسل_2 *orig_تسلسل = تسلسل;
      تسلسل = تسلسل->اگلا_تسلسل;
      hb_free (orig_تسلسل);
    }
  }

  STDMETHODIMP
  GenerateResults (IDWrite_متن_Analyzer* متن_Analyzer, تسلسل_2 **تسلسل_سر)
  {
    // Analyzes the text using the script analyzer and returns
    // the result as a series of runs.

    HRESULT hr = S_OK;

    // Initially start out with one result that covers the entire range.
    // This result will be subdivided by the analysis processes.
    m_تسلسل_سر.m_متن_آغاز = 0;
    m_تسلسل_سر.m_متن_لمبائی = m_متن_لمبائی;
    m_تسلسل_سر.mBidiLevel =
      (mReadingDirection == DWRITE_READING_DIRECTION_دایاں_مقصد_بایاں);
    m_تسلسل_سر.اگلا_تسلسل = خالی_اشاریہ;
    m_Current_تسلسل = &m_تسلسل_سر;

    // Call each of the analyzers in sequence, recording their results.
    اگر (SUCCEEDED (hr = متن_Analyzer->AnalyzeScript (یہ, 0, m_متن_لمبائی, یہ)))
      *تسلسل_سر = &m_تسلسل_سر;

    واپس hr;
  }

  // IDWriteTextAnalysisSource implementation

  IFACEMETHODIMP
  Get_متن_At_Position (uint32_t متن_Position,
		     برآمد_3 wchar_t ثابت** متن_String,
		     برآمد_3 uint32_t* متن_لمبائی_2)
  {
    اگر (متن_Position >= m_متن_لمبائی)
    {
      // No text at this position, valid query though.
      *متن_String = خالی_اشاریہ;
      *متن_لمبائی_2 = 0;
    }
    ورنہ
    {
      *متن_String = m_متن + متن_Position;
      *متن_لمبائی_2 = m_متن_لمبائی - متن_Position;
    }
    واپس S_OK;
  }

  IFACEMETHODIMP
  Get_متن_Before_Position (uint32_t متن_Position,
			 برآمد_3 wchar_t ثابت** متن_String,
			 برآمد_3 uint32_t* متن_لمبائی_2)
  {
    اگر (متن_Position == 0 || متن_Position > m_متن_لمبائی)
    {
      // Either there is no text before here (== 0), or this
      // is an invalid position. The query is considered valid though.
      *متن_String = خالی_اشاریہ;
      *متن_لمبائی_2 = 0;
    }
    ورنہ
    {
      *متن_String = m_متن;
      *متن_لمبائی_2 = متن_Position;
    }
    واپس S_OK;
  }

  IFACEMETHODIMP_ (DWRITE_READING_DIRECTION)
  GetParagraphReadingDirection () { واپس mReadingDirection; }

  IFACEMETHODIMP GetLocaleName (uint32_t متن_Position, uint32_t* متن_لمبائی_2,
				wchar_t ثابت** localeName)
  { واپس S_OK; }

  IFACEMETHODIMP
  Get_نمبر_Substitution (uint32_t متن_Position,
			 برآمد_3 uint32_t* متن_لمبائی_2,
			 برآمد_3 IDWrite_نمبر_Substitution** نمبر_Substitution)
  {
    // We do not support number substitution.
    *نمبر_Substitution = خالی_اشاریہ;
    *متن_لمبائی_2 = m_متن_لمبائی - متن_Position;

    واپس S_OK;
  }

  // IDWriteTextAnalysisSink implementation

  IFACEMETHODIMP
  سیٹ_Script_Analysis (uint32_t متن_Position, uint32_t متن_لمبائی_2,
		     DWRITE_SCRIPT_ANALYSIS ثابت* scriptAnalysis)
  {
    سیٹ_Current_تسلسل (متن_Position);
    اگر (unlikely (!بانٹو_Current_تسلسل (متن_Position)))
      واپس E_OUTOFMEMORY;
    جب (متن_لمبائی_2 > 0)
    {
      تسلسل_2 *تسلسل = Fetch_اگلا_تسلسل (&متن_لمبائی_2);
      اگر (unlikely (!تسلسل))
        واپس E_OUTOFMEMORY;
      تسلسل->mScript = *scriptAnalysis;
    }

    واپس S_OK;
  }

  IFACEMETHODIMP
  سیٹ_سطر_Breakpoints (uint32_t متن_Position,
		      uint32_t متن_لمبائی_2,
		      ثابت DWRITE_سطر_BREAKPOINT* سطر_Breakpoints)
  { واپس S_OK; }

  IFACEMETHODIMP سیٹ_Bidi_Level (uint32_t متن_Position, uint32_t متن_لمبائی_2,
			       uint8_t explicitLevel, uint8_t resolvedLevel)
  { واپس S_OK; }

  IFACEMETHODIMP
  سیٹ_نمبر_Substitution (uint32_t متن_Position, uint32_t متن_لمبائی_2,
			 IDWrite_نمبر_Substitution* نمبر_Substitution)
  { واپس S_OK; }

محفوظ:
  تسلسل_2 *Fetch_اگلا_تسلسل (اندر_2 برآمد_3 uint32_t* متن_لمبائی_2)
  {
    // Used by the sink setters, this returns a reference to the next run.
    // Position and length are adjusted to now point after the current run
    // being returned.

    تسلسل_2 *orig_تسلسل = m_Current_تسلسل;
    // Split the tail if needed (the length remaining is less than the
    // current run's size).
    اگر (*متن_لمبائی_2 < m_Current_تسلسل->m_متن_لمبائی)
    {
      اگر (unlikely (!بانٹو_Current_تسلسل (m_Current_تسلسل->m_متن_آغاز + *متن_لمبائی_2)))
        واپس خالی_اشاریہ;
    }
    ورنہ
      // Just advance the current run.
      m_Current_تسلسل = m_Current_تسلسل->اگلا_تسلسل;
    *متن_لمبائی_2 -= orig_تسلسل->m_متن_لمبائی;

    // Return a reference to the run that was just current.
    واپس orig_تسلسل;
  }

  خالی سیٹ_Current_تسلسل (uint32_t متن_Position)
  {
    // Move the current run to the given position.
    // Since the analyzers generally return results in a forward manner,
    // this will usually just return early. If not, find the
    // corresponding run for the text position.

    اگر (m_Current_تسلسل && m_Current_تسلسل->شامل_ہے_متن_Position (متن_Position))
      واپس;

    برائے (تسلسل_2 *تسلسل = &m_تسلسل_سر; تسلسل; تسلسل = تسلسل->اگلا_تسلسل)
      اگر (تسلسل->شامل_ہے_متن_Position (متن_Position))
      {
	m_Current_تسلسل = تسلسل;
	واپس;
      }
    أكد (0); // We should always be able to find the text position in one of our runs
  }

  منطقی بانٹو_Current_تسلسل (uint32_t بانٹو_Position)
  {
    اگر (!m_Current_تسلسل)
    {
      أكد (0); // SplitCurrentRun called without current run
      // Shouldn't be calling this when no current run is set!
      واپس جھوٹ;
    }
    // Split the current run.
    اگر (بانٹو_Position <= m_Current_تسلسل->m_متن_آغاز)
    {
      // No need to split, already the start of a run
      // or before it. Usually the first.
      واپس سچ;
    }
    تسلسل_2 *new_تسلسل = (تسلسل_2 *) hb_malloc (حجم (تسلسل_2));
    اگر (unlikely (!new_تسلسل))
      واپس جھوٹ;

    *new_تسلسل = *m_Current_تسلسل;

    // Insert the new run in our linked list.
    new_تسلسل->اگلا_تسلسل = m_Current_تسلسل->اگلا_تسلسل;
    m_Current_تسلسل->اگلا_تسلسل = new_تسلسل;

    // Adjust runs' text positions and lengths.
    uint32_t بانٹو_نقطہ = بانٹو_Position - m_Current_تسلسل->m_متن_آغاز;
    new_تسلسل->m_متن_آغاز += بانٹو_نقطہ;
    new_تسلسل->m_متن_لمبائی -= بانٹو_نقطہ;
    m_Current_تسلسل->m_متن_لمبائی = بانٹو_نقطہ;
    m_Current_تسلسل = new_تسلسل;
    واپس سچ;
  }

محفوظ:
  //داخلہ
  // (weak references are fine here, since this class is a transient
  //  stack-based helper that doesn't need to copy data)
  uint32_t m_متن_لمبائی;
  ثابت wchar_t* m_متن;
  ثابت wchar_t* mLocaleName;
  DWRITE_READING_DIRECTION mReadingDirection;

  // Current processing state.
  تسلسل_2 *m_Current_تسلسل;

  // Output is a list of runs starting here
  تسلسل_2  m_تسلسل_سر;
};

/*
 * shaper
 */

hb_bool_t
_hb_directwrite_shape (hb_shape_plan_t    *shape_plan,
		       hb_font_t          *font,
		       hb_بفر_t        *بفر,
		       ثابت hb_feature_t *features,
		       غیرعلامتی صحیح        عدد_features)
{
  IDWriteFontFace *fontFace = (IDWriteFontFace *) (ثابت خالی *) font->ڈیٹا.directwrite;
  خودکار *global = get_directwrite_global ();
  اگر (unlikely (!global))
    واپس جھوٹ;
  IDWriteFactory *dwriteFactory = global->dwriteFactory;

  IDWrite_متن_Analyzer* analyzer;
  dwriteFactory->Create_متن_Analyzer (&analyzer);

  غیرعلامتی صحیح scratch_سائز;
  hb_بفر_t::scratch_بفر_t *scratch = بفر->get_scratch_بفر (&scratch_سائز);
#تعریف_کرو ALLOCATE_ARRAY(Type, name, len) \
  Type *name = (Type *) scratch; \
  کرو { \
    غیرعلامتی صحیح _consumed = DIV_CEIL ((len) * حجم (Type), حجم (*scratch)); \
    أكد (_consumed <= scratch_سائز); \
    scratch += _consumed; \
    scratch_سائز -= _consumed; \
  } جب (0)

#تعریف_کرو utf16_اشاریہ() var1.u32

  ALLOCATE_ARRAY (wchar_t, متن_String, بفر->len * 2);

  غیرعلامتی صحیح chars_len = 0;
  برائے (غیرعلامتی صحیح i = 0; i < بفر->len; i++)
  {
    hb_codepoint_t c = بفر->info[i].codepoint;
    بفر->info[i].utf16_اشاریہ () = chars_len;
    اگر (likely (c <= 0xFFFFu))
      متن_String[chars_len++] = c;
    ورنہ اگر (unlikely (c > 0x10FFFFu))
      متن_String[chars_len++] = 0xFFFDu;
    ورنہ
    {
      متن_String[chars_len++] = 0xD800u + ((c - 0x10000u) >> 10);
      متن_String[chars_len++] = 0xDC00u + ((c - 0x10000u) & ((1u << 10) - 1));
    }
  }

  ALLOCATE_ARRAY (لفظ, log_clusters, chars_len);
#تعریف_منسوخ ALLOCATE_ARRAY
  /* Need log_clusters to assign features. */
  chars_len = 0;
  برائے (غیرعلامتی صحیح i = 0; i < بفر->len; i++)
  {
    hb_codepoint_t c = بفر->info[i].codepoint;
    غیرعلامتی صحیح cluster = بفر->info[i].cluster;
    log_clusters[chars_len++] = cluster;
    اگر (hb_اندر_range (c, 0x10000u, 0x10FFFFu))
      log_clusters[chars_len++] = cluster; /* Surrogates. */
  }

  DWRITE_READING_DIRECTION readingDirection;
  readingDirection = بفر->props.direction ?
		     DWRITE_READING_DIRECTION_دایاں_مقصد_بایاں :
		     DWRITE_READING_DIRECTION_بایاں_مقصد_دایاں;

  /*
  * There's an internal 16-bit limit on some things inside the analyzer,
  * but we never attempt to shape a word longer than 64K characters
  * in a single gfxShapedWord, so we cannot exceed that limit.
  */
  uint32_t متن_لمبائی_2 = chars_len;

  متن_Analysis analysis (متن_String, متن_لمبائی_2, خالی_اشاریہ, readingDirection);
  متن_Analysis::تسلسل_2 *تسلسل_سر;
  HRESULT hr;
  hr = analysis.GenerateResults (analyzer, &تسلسل_سر);

#تعریف_کرو FAIL(...) \
  HB_STMT_آغاز { \
    DEBUG_پیغام (DIRECTWRITE, خالی_اشاریہ, __VA_ARGS__); \
    واپس جھوٹ; \
  } HB_STMT_انتہا

  اگر (FAILED (hr))
    FAIL ("Analyzer failed to generate results.");

  uint32_t max_Glyph_گنتی = 3 * متن_لمبائی_2 / 2 + 16;
  uint32_t glyph_گنتی_2;
  منطقی is_دایاں_مقصد_بایاں = HB_DIRECTION_IS_BACKWARD (بفر->props.direction);

  ثابت wchar_t localeName[20] = {0};
  اگر (بفر->props.language)
    mbstowcs ((wchar_t*) localeName,
	      hb_language_مقصد_string (بفر->props.language), 20);

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
  hb_سمتیہ_t<hb_ms_feature_t> feature_اندراجات;
  hb_سمتیہ_t<hb_ms_range_ریکارڈ_t> range_اندراجات;
  اگر (عدد_features)
  {
    اگر (hb_ms_تیاری_features (features, عدد_features, feature_اندراجات, range_اندراجات))
    {
      hb_ms_make_feature_ranges (feature_اندراجات,
				 range_اندراجات,
				 0,
				 chars_len,
				 log_clusters,
				 range_features,
				 range_char_counts);
    }
  }

  hb_سمتیہ_t<uint16_t> clusterMap;
  hb_سمتیہ_t<DWRITE_SHAPING_متن_PROPERTIES> متن_Properties;
  اگر (unlikely (!clusterMap.resize_exact (متن_لمبائی_2) ||
                !متن_Properties.resize_exact (متن_لمبائی_2)))
    FAIL ("Failed to allocate shaping text data.");

  hb_سمتیہ_t<uint16_t> glyphIndices;
  hb_سمتیہ_t<DWRITE_SHAPING_GLYPH_PROPERTIES> glyphProperties;

retry_getglyphs:
  اگر (unlikely (!glyphIndices.resize_exact (max_Glyph_گنتی) ||
                !glyphProperties.resize_exact (max_Glyph_گنتی)))
    FAIL ("Failed to allocate shaping glyph data.");

  hr = analyzer->GetGlyphs (متن_String,
			    chars_len,
			    fontFace,
			    جھوٹ,
			    is_دایاں_مقصد_بایاں,
			    &تسلسل_سر->mScript,
			    localeName,
			    خالی_اشاریہ,
			    (ثابت DWRITE_TYPOGRAPHIC_FEATURES**) range_features.arrayZ,
			    range_char_counts.arrayZ,
			    range_features.لمبائی,
			    max_Glyph_گنتی,
			    clusterMap.arrayZ,
			    متن_Properties.arrayZ,
			    glyphIndices.arrayZ,
			    glyphProperties.arrayZ,
			    &glyph_گنتی_2);

  اگر (unlikely (hr == HRESULT_FROM_WIN32 (ERROR_INSUFFICIENT_بفر)))
  {
    max_Glyph_گنتی *= 2;

    جاؤ retry_getglyphs;
  }
  اگر (FAILED (hr))
    FAIL ("Analyzer failed to get glyphs.");

  hb_سمتیہ_t<اعشاری> glyphAdvances;
  hb_سمتیہ_t<DWRITE_GLYPH_انحراف> glyphOffsets;
  اگر (unlikely (!glyphAdvances.resize_exact (max_Glyph_گنتی) ||
                !glyphOffsets.resize_exact (max_Glyph_گنتی)))
    FAIL ("Failed to allocate glyph positioning data.");


  غیرعلامتی font_Em_سائز = font->face->get_upem ();

  اعشاری x_mult = font->x_multf;
  اعشاری y_mult = font->y_multf;

  hr = analyzer->GetGlyphPlacements (متن_String,
				     clusterMap.arrayZ,
				     متن_Properties.arrayZ,
				     chars_len,
				     glyphIndices.arrayZ,
				     glyphProperties.arrayZ,
				     glyph_گنتی_2,
				     fontFace,
				     font_Em_سائز,
				     جھوٹ,
				     is_دایاں_مقصد_بایاں,
				     &تسلسل_سر->mScript,
				     localeName,
				     (ثابت DWRITE_TYPOGRAPHIC_FEATURES**) range_features.arrayZ,
				     range_char_counts.arrayZ,
				     range_features.لمبائی,
				     glyphAdvances.arrayZ,
				     glyphOffsets.arrayZ);

  اگر (FAILED (hr))
    FAIL ("Analyzer failed to get glyph placements.");

  /* Ok, we've got everything we need, now compose output buffer,
   * very, *very*, carefully! */

  /* Calculate visual-clusters.  That's what we ship. */
  hb_سمتیہ_t<uint32_t> vis_clusters;
  اگر (unlikely (!vis_clusters.resize_exact (glyph_گنتی_2)))
    FAIL ("Failed to allocate visual clusters.");

  برائے (غیرعلامتی صحیح i = 0; i < glyph_گنتی_2; i++)
    vis_clusters[i] = (uint32_t) -1;
  برائے (غیرعلامتی صحیح i = 0; i < بفر->len; i++)
  {
    غیرعلامتی صحیح c = clusterMap[بفر->info[i].utf16_اشاریہ ()];
    اگر (likely (c < glyph_گنتی_2))
      vis_clusters[c] = hb_min (vis_clusters[c], بفر->info[i].cluster);
  }
  برائے (غیرعلامتی صحیح i = 1; i < glyph_گنتی_2; i++)
    اگر (vis_clusters[i] == (uint32_t) -1)
      vis_clusters[i] = vis_clusters[i - 1];
  برائے (صحیح i = (صحیح) glyph_گنتی_2 - 2; i >= 0; i--)
    اگر (vis_clusters[i] == (uint32_t) -1)
      vis_clusters[i] = vis_clusters[i + 1];

#تعریف_منسوخ utf16_اشاریہ

  اگر (unlikely (!بفر->ensure (glyph_گنتی_2)))
    FAIL ("Buffer in error");

#تعریف_منسوخ FAIL

  /* Set glyph infos */
  بفر->len = 0;
  برائے (غیرعلامتی صحیح i = 0; i < glyph_گنتی_2; i++)
  {
    hb_glyph_info_t *info = &بفر->info[بفر->len++];

    info->codepoint = glyphIndices[i];
    info->cluster = vis_clusters[i];

    /* The rest is crap.  Let's store position info there for now. */
    info->نقاب = (hb_نقاب_t) hb_clamp_مقصد<int32_t> (glyphAdvances[i]);
    info->var1.i32 = hb_clamp_مقصد<int32_t> (glyphOffsets[i].advance_انحراف);
    info->var2.i32 = hb_clamp_مقصد<int32_t> (glyphOffsets[i].ascender_انحراف);
  }

  /* Set glyph positions */
  بفر->clear_positions ();
  برائے (غیرعلامتی صحیح i = 0; i < glyph_گنتی_2; i++)
  {
    hb_glyph_info_t *info = &بفر->info[i];
    hb_glyph_position_t *pos = &بفر->pos[i];

    /* TODO vertical */
    pos->x_advance = hb_clamp_مقصد<hb_position_t> (round (x_mult * (int32_t) info->نقاب));
    دوہرا x_انحراف_2 = is_دایاں_مقصد_بایاں ? -(دوہرا) info->var1.i32 : info->var1.i32;
    pos->x_انحراف_2 = hb_clamp_مقصد<hb_position_t> (round ((دوہرا) x_mult * x_انحراف_2));
    pos->y_انحراف_2 = hb_clamp_مقصد<hb_position_t> (round (y_mult * info->var2.i32));
  }

  اگر (is_دایاں_مقصد_بایاں) hb_بفر_الٹ (بفر);

  بفر->clear_glyph_flags ();
  بفر->unsafe_مقصد_break ();

  /* Wow, done! */
  واپس سچ;
}


#اگر_ختم
