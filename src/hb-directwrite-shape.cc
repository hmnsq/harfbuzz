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

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_DIRECTWRITE

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-shaper-impl.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-directwrite.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ms-feature-ranges.hh"


/*
* shaper face data
*/

hb_directwrite_face_ਅੰਕੜੇ_t *
hb_directwrite_shaper_face_ਅੰਕੜੇ_create (hb_face_t *face)
{
  hb_blob_t *blob = hb_face_reference_blob (face);

  hb_directwrite_face_ਅੰਕੜੇ_t *ਅੰਕੜੇ = (hb_directwrite_face_ਅੰਕੜੇ_t *) dw_face_create (blob, face->ਸੂਚਕਾਂਕ_2);

  hb_blob_destroy (blob);

  ਵਾਪਸ ਅੰਕੜੇ;
}

ਖਾਲੀ
hb_directwrite_shaper_face_ਅੰਕੜੇ_destroy (hb_directwrite_face_ਅੰਕੜੇ_t *ਅੰਕੜੇ)
{
  ((IDWriteFontFace *) ਅੰਕੜੇ)->Release ();
}


/*
 * shaper font data
 */

ਬਣਤਰ hb_directwrite_font_ਅੰਕੜੇ_t {};

hb_directwrite_font_ਅੰਕੜੇ_t *
hb_directwrite_shaper_font_ਅੰਕੜੇ_create (hb_font_t *font)
{
  IDWriteFontFace *fontFace = (IDWriteFontFace *) (ਅਚਲ ਖਾਲੀ *) font->face->ਅੰਕੜੇ.directwrite;

  /*
   * Set up variations.
   */
  IDWriteFontFace5 *fontFaceVariations = ਖਾਲੀ_ਸੂਚਕ;
  {
    IDWriteFontFace5 *fontFace5;
    ਜੇ (SUCCEEDED (fontFace->QueryInterface (__uuidof (IDWriteFontFace5), (ਖਾਲੀ **) &fontFace5)))
    {
      IDWriteFontResource *fontResource;
      ਜੇ (SUCCEEDED (fontFace5->GetFontResource (&fontResource)))
      {
	hb_ਸਦਿਸ਼_t<DWRITE_FONT_AXIS_ਮੁੱਲ> axis_ਮੁੱਲਾਂ;
	ਜੇ (likely (axis_ਮੁੱਲਾਂ.resize_exact (font->ਅੰਕ_coords)))
	{
	  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < font->ਅੰਕ_coords; i++)
	  {
	    hb_ot_var_axis_info_t info;
	    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ c = 1;
	    hb_ot_var_get_axis_infos (font->face, i, &c, &info);
	    axis_ਮੁੱਲਾਂ[i].axis_ਟੈਗ = (DWRITE_FONT_AXIS_ਟੈਗ) hb_uint32_ਅਦਲਾ_ਬਦਲੀ (info.ਟੈਗ_2);
	    axis_ਮੁੱਲਾਂ[i].value = i < font->ਅੰਕ_coords ?
				   hb_clamp (font->design_coords[i], info.min_ਮੁੱਲ, info.max_ਮੁੱਲ) :
				   info.default_ਮੁੱਲ;
	  }

	  fontResource->CreateFontFace (DWRITE_FONT_SIMULATIONS::DWRITE_FONT_SIMULATIONS_NONE,
					axis_ਮੁੱਲਾਂ.arrayZ, axis_ਮੁੱਲਾਂ.ਲੰਮਾਈ, &fontFaceVariations);
	}
	fontResource->Release ();
      }
      fontFace5->Release ();
    }
  }

  ਵਾਪਸ (hb_directwrite_font_ਅੰਕੜੇ_t *) fontFaceVariations;
}

ਖਾਲੀ
hb_directwrite_shaper_font_ਅੰਕੜੇ_destroy (hb_directwrite_font_ਅੰਕੜੇ_t *ਅੰਕੜੇ)
{
  ((IDWriteFontFace *) (ਅਚਲ ਖਾਲੀ *) ਅੰਕੜੇ)->Release ();
}


// Most of TextAnalysis is originally written by Bas Schouten for Mozilla project
// but now is relicensed to MIT for HarfBuzz use
ਵਰਗ ਪਾਠ_Analysis : ਜਨਤਕ IDWrite_ਪਾਠ_Analysis_ਸਰੋਤ, ਜਨਤਕ IDWrite_ਪਾਠ_Analysis_Sink
{
ਨਿਜੀ:
  hb_reference_ਗਿਣਤੀ_t m_Ref_ਗਿਣਤੀ;
ਜਨਤਕ:
  IFACEMETHOD (QueryInterface) (IID ਅਚਲ& iid, ਬਾਹਰਲਾ_3 ਖਾਲੀ** ppObject)
  { ਵਾਪਸ S_OK; }
  IFACEMETHOD_ (ULONG, ਜੋੜੋ_Ref) ()
  {
    ਵਾਪਸ m_Ref_ਗਿਣਤੀ.inc () + 1;
  }
  IFACEMETHOD_ (ULONG, Release) ()
  {
    ਨਿਸ਼ਾਨੀਵਾਲਾ ref_ਗਿਣਤੀ = m_Ref_ਗਿਣਤੀ.dec () - 1;
    ਪ੍ਰਤਿਜ੍ਞਾ (ref_ਗਿਣਤੀ >= 0);
    ਜੇ (ref_ਗਿਣਤੀ)
      ਵਾਪਸ ref_ਗਿਣਤੀ;
    ਵਾਪਸ 0;
  }

  // A single contiguous run of characters containing the same analysis
  // results.
  ਬਣਤਰ ਲੜੀਵਾਰ_2
  {
    uint32_t m_ਪਾਠ_ਸ਼ੁਰੂ;   // starting text position of this run
    uint32_t m_ਪਾਠ_ਲੰਮਾਈ;  // number of contiguous code units covered
    uint32_t m_Glyph_ਸ਼ੁਰੂ;  // starting glyph in the glyphs array
    uint32_t m_Glyph_ਗਿਣਤੀ;  // number of glyphs associated with this run
    //ਪਾਠ
    DWRITE_SCRIPT_ANALYSIS mScript;
    uint8_t mBidiLevel;
    ਤਾਰਕਿਕ mIsSideways;

    ਤਾਰਕਿਕ ਸ਼ਾਮਲ_ਹੈ_ਪਾਠ_Position (uint32_t a_ਪਾਠ_Position) ਅਚਲ
    {
      ਵਾਪਸ a_ਪਾਠ_Position >= m_ਪਾਠ_ਸ਼ੁਰੂ &&
	     a_ਪਾਠ_Position <  m_ਪਾਠ_ਸ਼ੁਰੂ + m_ਪਾਠ_ਲੰਮਾਈ;
    }

    ਲੜੀਵਾਰ_2 *ਅਗਲਾ_ਲੜੀਵਾਰ;
  };

ਜਨਤਕ:
  ਪਾਠ_Analysis (ਅਚਲ wchar_t* ਪਾਠ, uint32_t ਪਾਠ_ਲੰਮਾਈ_2,
		ਅਚਲ wchar_t* localeName, DWRITE_READING_DIRECTION readingDirection)
	       : m_ਪਾਠ_ਲੰਮਾਈ (ਪਾਠ_ਲੰਮਾਈ_2), m_ਪਾਠ (ਪਾਠ), mLocaleName (localeName),
		 mReadingDirection (readingDirection), m_Current_ਲੜੀਵਾਰ (ਖਾਲੀ_ਸੂਚਕ)
  {
    m_Ref_ਗਿਣਤੀ.init ();
  }
  ~ਪਾਠ_Analysis ()
  {
    // free runs, except mRunHead which is part of the TextAnalysis object
    ਲਈ (ਲੜੀਵਾਰ_2 *ਲੜੀਵਾਰ = m_ਲੜੀਵਾਰ_ਸਿਰ.ਅਗਲਾ_ਲੜੀਵਾਰ; ਲੜੀਵਾਰ;)
    {
      ਲੜੀਵਾਰ_2 *orig_ਲੜੀਵਾਰ = ਲੜੀਵਾਰ;
      ਲੜੀਵਾਰ = ਲੜੀਵਾਰ->ਅਗਲਾ_ਲੜੀਵਾਰ;
      hb_free (orig_ਲੜੀਵਾਰ);
    }
  }

  STDMETHODIMP
  GenerateResults (IDWrite_ਪਾਠ_Analyzer* ਪਾਠ_Analyzer, ਲੜੀਵਾਰ_2 **ਲੜੀਵਾਰ_ਸਿਰ)
  {
    // Analyzes the text using the script analyzer and returns
    // the result as a series of runs.

    HRESULT hr = S_OK;

    // Initially start out with one result that covers the entire range.
    // This result will be subdivided by the analysis processes.
    m_ਲੜੀਵਾਰ_ਸਿਰ.m_ਪਾਠ_ਸ਼ੁਰੂ = 0;
    m_ਲੜੀਵਾਰ_ਸਿਰ.m_ਪਾਠ_ਲੰਮਾਈ = m_ਪਾਠ_ਲੰਮਾਈ;
    m_ਲੜੀਵਾਰ_ਸਿਰ.mBidiLevel =
      (mReadingDirection == DWRITE_READING_DIRECTION_ਸੱਜਾ_ਮੰਜ਼ਿਲ_ਖੱਬਾ);
    m_ਲੜੀਵਾਰ_ਸਿਰ.ਅਗਲਾ_ਲੜੀਵਾਰ = ਖਾਲੀ_ਸੂਚਕ;
    m_Current_ਲੜੀਵਾਰ = &m_ਲੜੀਵਾਰ_ਸਿਰ;

    // Call each of the analyzers in sequence, recording their results.
    ਜੇ (SUCCEEDED (hr = ਪਾਠ_Analyzer->AnalyzeScript (ਇਹ, 0, m_ਪਾਠ_ਲੰਮਾਈ, ਇਹ)))
      *ਲੜੀਵਾਰ_ਸਿਰ = &m_ਲੜੀਵਾਰ_ਸਿਰ;

    ਵਾਪਸ hr;
  }

  // IDWriteTextAnalysisSource implementation

  IFACEMETHODIMP
  Get_ਪਾਠ_At_Position (uint32_t ਪਾਠ_Position,
		     ਬਾਹਰਲਾ_3 wchar_t ਅਚਲ** ਪਾਠ_String,
		     ਬਾਹਰਲਾ_3 uint32_t* ਪਾਠ_ਲੰਮਾਈ_2)
  {
    ਜੇ (ਪਾਠ_Position >= m_ਪਾਠ_ਲੰਮਾਈ)
    {
      // No text at this position, valid query though.
      *ਪਾਠ_String = ਖਾਲੀ_ਸੂਚਕ;
      *ਪਾਠ_ਲੰਮਾਈ_2 = 0;
    }
    ਨਹੀਂਤਾਂ
    {
      *ਪਾਠ_String = m_ਪਾਠ + ਪਾਠ_Position;
      *ਪਾਠ_ਲੰਮਾਈ_2 = m_ਪਾਠ_ਲੰਮਾਈ - ਪਾਠ_Position;
    }
    ਵਾਪਸ S_OK;
  }

  IFACEMETHODIMP
  Get_ਪਾਠ_Before_Position (uint32_t ਪਾਠ_Position,
			 ਬਾਹਰਲਾ_3 wchar_t ਅਚਲ** ਪਾਠ_String,
			 ਬਾਹਰਲਾ_3 uint32_t* ਪਾਠ_ਲੰਮਾਈ_2)
  {
    ਜੇ (ਪਾਠ_Position == 0 || ਪਾਠ_Position > m_ਪਾਠ_ਲੰਮਾਈ)
    {
      // Either there is no text before here (== 0), or this
      // is an invalid position. The query is considered valid though.
      *ਪਾਠ_String = ਖਾਲੀ_ਸੂਚਕ;
      *ਪਾਠ_ਲੰਮਾਈ_2 = 0;
    }
    ਨਹੀਂਤਾਂ
    {
      *ਪਾਠ_String = m_ਪਾਠ;
      *ਪਾਠ_ਲੰਮਾਈ_2 = ਪਾਠ_Position;
    }
    ਵਾਪਸ S_OK;
  }

  IFACEMETHODIMP_ (DWRITE_READING_DIRECTION)
  GetParagraphReadingDirection () { ਵਾਪਸ mReadingDirection; }

  IFACEMETHODIMP GetLocaleName (uint32_t ਪਾਠ_Position, uint32_t* ਪਾਠ_ਲੰਮਾਈ_2,
				wchar_t ਅਚਲ** localeName)
  { ਵਾਪਸ S_OK; }

  IFACEMETHODIMP
  Get_ਸੰਖਿਆ_Substitution (uint32_t ਪਾਠ_Position,
			 ਬਾਹਰਲਾ_3 uint32_t* ਪਾਠ_ਲੰਮਾਈ_2,
			 ਬਾਹਰਲਾ_3 IDWrite_ਸੰਖਿਆ_Substitution** ਸੰਖਿਆ_Substitution)
  {
    // We do not support number substitution.
    *ਸੰਖਿਆ_Substitution = ਖਾਲੀ_ਸੂਚਕ;
    *ਪਾਠ_ਲੰਮਾਈ_2 = m_ਪਾਠ_ਲੰਮਾਈ - ਪਾਠ_Position;

    ਵਾਪਸ S_OK;
  }

  // IDWriteTextAnalysisSink implementation

  IFACEMETHODIMP
  ਸੈੱਟ_Script_Analysis (uint32_t ਪਾਠ_Position, uint32_t ਪਾਠ_ਲੰਮਾਈ_2,
		     DWRITE_SCRIPT_ANALYSIS ਅਚਲ* scriptAnalysis)
  {
    ਸੈੱਟ_Current_ਲੜੀਵਾਰ (ਪਾਠ_Position);
    ਜੇ (unlikely (!ਵੰਡੋ_Current_ਲੜੀਵਾਰ (ਪਾਠ_Position)))
      ਵਾਪਸ E_OUTOFMEMORY;
    ਜਦੋਂ (ਪਾਠ_ਲੰਮਾਈ_2 > 0)
    {
      ਲੜੀਵਾਰ_2 *ਲੜੀਵਾਰ = Fetch_ਅਗਲਾ_ਲੜੀਵਾਰ (&ਪਾਠ_ਲੰਮਾਈ_2);
      ਜੇ (unlikely (!ਲੜੀਵਾਰ))
        ਵਾਪਸ E_OUTOFMEMORY;
      ਲੜੀਵਾਰ->mScript = *scriptAnalysis;
    }

    ਵਾਪਸ S_OK;
  }

  IFACEMETHODIMP
  ਸੈੱਟ_ਸਤਰ_Breakpoints (uint32_t ਪਾਠ_Position,
		      uint32_t ਪਾਠ_ਲੰਮਾਈ_2,
		      ਅਚਲ DWRITE_ਸਤਰ_BREAKPOINT* ਸਤਰ_Breakpoints)
  { ਵਾਪਸ S_OK; }

  IFACEMETHODIMP ਸੈੱਟ_Bidi_Level (uint32_t ਪਾਠ_Position, uint32_t ਪਾਠ_ਲੰਮਾਈ_2,
			       uint8_t explicitLevel, uint8_t resolvedLevel)
  { ਵਾਪਸ S_OK; }

  IFACEMETHODIMP
  ਸੈੱਟ_ਸੰਖਿਆ_Substitution (uint32_t ਪਾਠ_Position, uint32_t ਪਾਠ_ਲੰਮਾਈ_2,
			 IDWrite_ਸੰਖਿਆ_Substitution* ਸੰਖਿਆ_Substitution)
  { ਵਾਪਸ S_OK; }

ਸੁਰੱਖਿਅਤ:
  ਲੜੀਵਾਰ_2 *Fetch_ਅਗਲਾ_ਲੜੀਵਾਰ (ਵਿੱਚ_2 ਬਾਹਰਲਾ_3 uint32_t* ਪਾਠ_ਲੰਮਾਈ_2)
  {
    // Used by the sink setters, this returns a reference to the next run.
    // Position and length are adjusted to now point after the current run
    // being returned.

    ਲੜੀਵਾਰ_2 *orig_ਲੜੀਵਾਰ = m_Current_ਲੜੀਵਾਰ;
    // Split the tail if needed (the length remaining is less than the
    // current run's size).
    ਜੇ (*ਪਾਠ_ਲੰਮਾਈ_2 < m_Current_ਲੜੀਵਾਰ->m_ਪਾਠ_ਲੰਮਾਈ)
    {
      ਜੇ (unlikely (!ਵੰਡੋ_Current_ਲੜੀਵਾਰ (m_Current_ਲੜੀਵਾਰ->m_ਪਾਠ_ਸ਼ੁਰੂ + *ਪਾਠ_ਲੰਮਾਈ_2)))
        ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
    }
    ਨਹੀਂਤਾਂ
      // Just advance the current run.
      m_Current_ਲੜੀਵਾਰ = m_Current_ਲੜੀਵਾਰ->ਅਗਲਾ_ਲੜੀਵਾਰ;
    *ਪਾਠ_ਲੰਮਾਈ_2 -= orig_ਲੜੀਵਾਰ->m_ਪਾਠ_ਲੰਮਾਈ;

    // Return a reference to the run that was just current.
    ਵਾਪਸ orig_ਲੜੀਵਾਰ;
  }

  ਖਾਲੀ ਸੈੱਟ_Current_ਲੜੀਵਾਰ (uint32_t ਪਾਠ_Position)
  {
    // Move the current run to the given position.
    // Since the analyzers generally return results in a forward manner,
    // this will usually just return early. If not, find the
    // corresponding run for the text position.

    ਜੇ (m_Current_ਲੜੀਵਾਰ && m_Current_ਲੜੀਵਾਰ->ਸ਼ਾਮਲ_ਹੈ_ਪਾਠ_Position (ਪਾਠ_Position))
      ਵਾਪਸ;

    ਲਈ (ਲੜੀਵਾਰ_2 *ਲੜੀਵਾਰ = &m_ਲੜੀਵਾਰ_ਸਿਰ; ਲੜੀਵਾਰ; ਲੜੀਵਾਰ = ਲੜੀਵਾਰ->ਅਗਲਾ_ਲੜੀਵਾਰ)
      ਜੇ (ਲੜੀਵਾਰ->ਸ਼ਾਮਲ_ਹੈ_ਪਾਠ_Position (ਪਾਠ_Position))
      {
	m_Current_ਲੜੀਵਾਰ = ਲੜੀਵਾਰ;
	ਵਾਪਸ;
      }
    ਪ੍ਰਤਿਜ੍ਞਾ (0); // We should always be able to find the text position in one of our runs
  }

  ਤਾਰਕਿਕ ਵੰਡੋ_Current_ਲੜੀਵਾਰ (uint32_t ਵੰਡੋ_Position)
  {
    ਜੇ (!m_Current_ਲੜੀਵਾਰ)
    {
      ਪ੍ਰਤਿਜ੍ਞਾ (0); // SplitCurrentRun called without current run
      // Shouldn't be calling this when no current run is set!
      ਵਾਪਸ ਝੂਠ;
    }
    // Split the current run.
    ਜੇ (ਵੰਡੋ_Position <= m_Current_ਲੜੀਵਾਰ->m_ਪਾਠ_ਸ਼ੁਰੂ)
    {
      // No need to split, already the start of a run
      // or before it. Usually the first.
      ਵਾਪਸ ਸੱਚ;
    }
    ਲੜੀਵਾਰ_2 *new_ਲੜੀਵਾਰ = (ਲੜੀਵਾਰ_2 *) hb_malloc (ਆਕਾਰ (ਲੜੀਵਾਰ_2));
    ਜੇ (unlikely (!new_ਲੜੀਵਾਰ))
      ਵਾਪਸ ਝੂਠ;

    *new_ਲੜੀਵਾਰ = *m_Current_ਲੜੀਵਾਰ;

    // Insert the new run in our linked list.
    new_ਲੜੀਵਾਰ->ਅਗਲਾ_ਲੜੀਵਾਰ = m_Current_ਲੜੀਵਾਰ->ਅਗਲਾ_ਲੜੀਵਾਰ;
    m_Current_ਲੜੀਵਾਰ->ਅਗਲਾ_ਲੜੀਵਾਰ = new_ਲੜੀਵਾਰ;

    // Adjust runs' text positions and lengths.
    uint32_t ਵੰਡੋ_ਬਿੰਦੂ = ਵੰਡੋ_Position - m_Current_ਲੜੀਵਾਰ->m_ਪਾਠ_ਸ਼ੁਰੂ;
    new_ਲੜੀਵਾਰ->m_ਪਾਠ_ਸ਼ੁਰੂ += ਵੰਡੋ_ਬਿੰਦੂ;
    new_ਲੜੀਵਾਰ->m_ਪਾਠ_ਲੰਮਾਈ -= ਵੰਡੋ_ਬਿੰਦੂ;
    m_Current_ਲੜੀਵਾਰ->m_ਪਾਠ_ਲੰਮਾਈ = ਵੰਡੋ_ਬਿੰਦੂ;
    m_Current_ਲੜੀਵਾਰ = new_ਲੜੀਵਾਰ;
    ਵਾਪਸ ਸੱਚ;
  }

ਸੁਰੱਖਿਅਤ:
  //ਨਿਵੇਸ਼
  // (weak references are fine here, since this class is a transient
  //  stack-based helper that doesn't need to copy data)
  uint32_t m_ਪਾਠ_ਲੰਮਾਈ;
  ਅਚਲ wchar_t* m_ਪਾਠ;
  ਅਚਲ wchar_t* mLocaleName;
  DWRITE_READING_DIRECTION mReadingDirection;

  // Current processing state.
  ਲੜੀਵਾਰ_2 *m_Current_ਲੜੀਵਾਰ;

  // Output is a list of runs starting here
  ਲੜੀਵਾਰ_2  m_ਲੜੀਵਾਰ_ਸਿਰ;
};

/*
 * shaper
 */

hb_bool_t
_hb_directwrite_shape (hb_shape_plan_t    *shape_plan,
		       hb_font_t          *font,
		       hb_ਬਫਰ_t        *ਬਫਰ,
		       ਅਚਲ hb_feature_t *features,
		       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ        ਅੰਕ_features)
{
  IDWriteFontFace *fontFace = (IDWriteFontFace *) (ਅਚਲ ਖਾਲੀ *) font->ਅੰਕੜੇ.directwrite;
  ਸਵੈਚਾਲਿਤ *global = get_directwrite_global ();
  ਜੇ (unlikely (!global))
    ਵਾਪਸ ਝੂਠ;
  IDWriteFactory *dwriteFactory = global->dwriteFactory;

  IDWrite_ਪਾਠ_Analyzer* analyzer;
  dwriteFactory->Create_ਪਾਠ_Analyzer (&analyzer);

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ scratch_ਮਾਪ;
  hb_ਬਫਰ_t::scratch_ਬਫਰ_t *scratch = ਬਫਰ->get_scratch_ਬਫਰ (&scratch_ਮਾਪ);
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ALLOCATE_ARRAY(Type, name, len) \
  Type *name = (Type *) scratch; \
  ਕਰੋ { \
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ _consumed = DIV_CEIL ((len) * ਆਕਾਰ (Type), ਆਕਾਰ (*scratch)); \
    ਪ੍ਰਤਿਜ੍ਞਾ (_consumed <= scratch_ਮਾਪ); \
    scratch += _consumed; \
    scratch_ਮਾਪ -= _consumed; \
  } ਜਦੋਂ (0)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ utf16_ਸੂਚਕਾਂਕ() var1.u32

  ALLOCATE_ARRAY (wchar_t, ਪਾਠ_String, ਬਫਰ->len * 2);

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ chars_len = 0;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਬਫਰ->len; i++)
  {
    hb_codepoint_t c = ਬਫਰ->info[i].codepoint;
    ਬਫਰ->info[i].utf16_ਸੂਚਕਾਂਕ () = chars_len;
    ਜੇ (likely (c <= 0xFFFFu))
      ਪਾਠ_String[chars_len++] = c;
    ਨਹੀਂਤਾਂ ਜੇ (unlikely (c > 0x10FFFFu))
      ਪਾਠ_String[chars_len++] = 0xFFFDu;
    ਨਹੀਂਤਾਂ
    {
      ਪਾਠ_String[chars_len++] = 0xD800u + ((c - 0x10000u) >> 10);
      ਪਾਠ_String[chars_len++] = 0xDC00u + ((c - 0x10000u) & ((1u << 10) - 1));
    }
  }

  ALLOCATE_ARRAY (ਸ਼ਬਦ, log_clusters, chars_len);
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ ALLOCATE_ARRAY
  /* Need log_clusters to assign features. */
  chars_len = 0;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਬਫਰ->len; i++)
  {
    hb_codepoint_t c = ਬਫਰ->info[i].codepoint;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cluster = ਬਫਰ->info[i].cluster;
    log_clusters[chars_len++] = cluster;
    ਜੇ (hb_ਵਿੱਚ_range (c, 0x10000u, 0x10FFFFu))
      log_clusters[chars_len++] = cluster; /* Surrogates. */
  }

  DWRITE_READING_DIRECTION readingDirection;
  readingDirection = ਬਫਰ->props.direction ?
		     DWRITE_READING_DIRECTION_ਸੱਜਾ_ਮੰਜ਼ਿਲ_ਖੱਬਾ :
		     DWRITE_READING_DIRECTION_ਖੱਬਾ_ਮੰਜ਼ਿਲ_ਸੱਜਾ;

  /*
  * There's an internal 16-bit limit on some things inside the analyzer,
  * but we never attempt to shape a word longer than 64K characters
  * in a single gfxShapedWord, so we cannot exceed that limit.
  */
  uint32_t ਪਾਠ_ਲੰਮਾਈ_2 = chars_len;

  ਪਾਠ_Analysis analysis (ਪਾਠ_String, ਪਾਠ_ਲੰਮਾਈ_2, ਖਾਲੀ_ਸੂਚਕ, readingDirection);
  ਪਾਠ_Analysis::ਲੜੀਵਾਰ_2 *ਲੜੀਵਾਰ_ਸਿਰ;
  HRESULT hr;
  hr = analysis.GenerateResults (analyzer, &ਲੜੀਵਾਰ_ਸਿਰ);

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FAIL(...) \
  HB_STMT_ਸ਼ੁਰੂ { \
    DEBUG_ਸੁਨੇਹਾ (DIRECTWRITE, ਖਾਲੀ_ਸੂਚਕ, __VA_ARGS__); \
    ਵਾਪਸ ਝੂਠ; \
  } HB_STMT_ਸਿਰਾ

  ਜੇ (FAILED (hr))
    FAIL ("Analyzer failed to generate results.");

  uint32_t max_Glyph_ਗਿਣਤੀ = 3 * ਪਾਠ_ਲੰਮਾਈ_2 / 2 + 16;
  uint32_t glyph_ਗਿਣਤੀ_2;
  ਤਾਰਕਿਕ is_ਸੱਜਾ_ਮੰਜ਼ਿਲ_ਖੱਬਾ = HB_DIRECTION_IS_BACKWARD (ਬਫਰ->props.direction);

  ਅਚਲ wchar_t localeName[20] = {0};
  ਜੇ (ਬਫਰ->props.language)
    mbstowcs ((wchar_t*) localeName,
	      hb_language_ਮੰਜ਼ਿਲ_string (ਬਫਰ->props.language), 20);

  /*
   * Set up features.
   */
  static_assert ((ਆਕਾਰ (DWRITE_TYPOGRAPHIC_FEATURES) == ਆਕਾਰ (hb_ms_features_t)), "");
  static_assert ((ਆਕਾਰ (DWRITE_FONT_FEATURE) == ਆਕਾਰ (hb_ms_feature_t)), "");
  hb_ਸਦਿਸ਼_t<hb_ms_features_t *> range_features;
  hb_ਸਦਿਸ਼_t<uint32_t> range_char_counts;

  // https://github.com/harfbuzz/harfbuzz/pull/5114
  // The data allocated by these two vectors are used by the above two, so they
  // should remain alive as long as the above two are.
  hb_ਸਦਿਸ਼_t<hb_ms_feature_t> feature_ਰਿਕਾਰਡਾਂ;
  hb_ਸਦਿਸ਼_t<hb_ms_range_ਰਿਕਾਰਡ_t> range_ਰਿਕਾਰਡਾਂ;
  ਜੇ (ਅੰਕ_features)
  {
    ਜੇ (hb_ms_ਤਿਆਰੀ_features (features, ਅੰਕ_features, feature_ਰਿਕਾਰਡਾਂ, range_ਰਿਕਾਰਡਾਂ))
    {
      hb_ms_make_feature_ranges (feature_ਰਿਕਾਰਡਾਂ,
				 range_ਰਿਕਾਰਡਾਂ,
				 0,
				 chars_len,
				 log_clusters,
				 range_features,
				 range_char_counts);
    }
  }

  hb_ਸਦਿਸ਼_t<uint16_t> clusterMap;
  hb_ਸਦਿਸ਼_t<DWRITE_SHAPING_ਪਾਠ_PROPERTIES> ਪਾਠ_Properties;
  ਜੇ (unlikely (!clusterMap.resize_exact (ਪਾਠ_ਲੰਮਾਈ_2) ||
                !ਪਾਠ_Properties.resize_exact (ਪਾਠ_ਲੰਮਾਈ_2)))
    FAIL ("Failed to allocate shaping text data.");

  hb_ਸਦਿਸ਼_t<uint16_t> glyphIndices;
  hb_ਸਦਿਸ਼_t<DWRITE_SHAPING_GLYPH_PROPERTIES> glyphProperties;

retry_getglyphs:
  ਜੇ (unlikely (!glyphIndices.resize_exact (max_Glyph_ਗਿਣਤੀ) ||
                !glyphProperties.resize_exact (max_Glyph_ਗਿਣਤੀ)))
    FAIL ("Failed to allocate shaping glyph data.");

  hr = analyzer->GetGlyphs (ਪਾਠ_String,
			    chars_len,
			    fontFace,
			    ਝੂਠ,
			    is_ਸੱਜਾ_ਮੰਜ਼ਿਲ_ਖੱਬਾ,
			    &ਲੜੀਵਾਰ_ਸਿਰ->mScript,
			    localeName,
			    ਖਾਲੀ_ਸੂਚਕ,
			    (ਅਚਲ DWRITE_TYPOGRAPHIC_FEATURES**) range_features.arrayZ,
			    range_char_counts.arrayZ,
			    range_features.ਲੰਮਾਈ,
			    max_Glyph_ਗਿਣਤੀ,
			    clusterMap.arrayZ,
			    ਪਾਠ_Properties.arrayZ,
			    glyphIndices.arrayZ,
			    glyphProperties.arrayZ,
			    &glyph_ਗਿਣਤੀ_2);

  ਜੇ (unlikely (hr == HRESULT_FROM_WIN32 (ERROR_INSUFFICIENT_ਬਫਰ)))
  {
    max_Glyph_ਗਿਣਤੀ *= 2;

    ਜਾਓ retry_getglyphs;
  }
  ਜੇ (FAILED (hr))
    FAIL ("Analyzer failed to get glyphs.");

  hb_ਸਦਿਸ਼_t<ਦਸ਼ਮਲਵ> glyphAdvances;
  hb_ਸਦਿਸ਼_t<DWRITE_GLYPH_ਵਿੱਥ> glyphOffsets;
  ਜੇ (unlikely (!glyphAdvances.resize_exact (max_Glyph_ਗਿਣਤੀ) ||
                !glyphOffsets.resize_exact (max_Glyph_ਗਿਣਤੀ)))
    FAIL ("Failed to allocate glyph positioning data.");


  ਬਿਨਾਨਿਸ਼ਾਨੀ font_Em_ਮਾਪ = font->face->get_upem ();

  ਦਸ਼ਮਲਵ x_mult = font->x_multf;
  ਦਸ਼ਮਲਵ y_mult = font->y_multf;

  hr = analyzer->GetGlyphPlacements (ਪਾਠ_String,
				     clusterMap.arrayZ,
				     ਪਾਠ_Properties.arrayZ,
				     chars_len,
				     glyphIndices.arrayZ,
				     glyphProperties.arrayZ,
				     glyph_ਗਿਣਤੀ_2,
				     fontFace,
				     font_Em_ਮਾਪ,
				     ਝੂਠ,
				     is_ਸੱਜਾ_ਮੰਜ਼ਿਲ_ਖੱਬਾ,
				     &ਲੜੀਵਾਰ_ਸਿਰ->mScript,
				     localeName,
				     (ਅਚਲ DWRITE_TYPOGRAPHIC_FEATURES**) range_features.arrayZ,
				     range_char_counts.arrayZ,
				     range_features.ਲੰਮਾਈ,
				     glyphAdvances.arrayZ,
				     glyphOffsets.arrayZ);

  ਜੇ (FAILED (hr))
    FAIL ("Analyzer failed to get glyph placements.");

  /* Ok, we've got everything we need, now compose output buffer,
   * very, *very*, carefully! */

  /* Calculate visual-clusters.  That's what we ship. */
  hb_ਸਦਿਸ਼_t<uint32_t> vis_clusters;
  ਜੇ (unlikely (!vis_clusters.resize_exact (glyph_ਗਿਣਤੀ_2)))
    FAIL ("Failed to allocate visual clusters.");

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < glyph_ਗਿਣਤੀ_2; i++)
    vis_clusters[i] = (uint32_t) -1;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਬਫਰ->len; i++)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ c = clusterMap[ਬਫਰ->info[i].utf16_ਸੂਚਕਾਂਕ ()];
    ਜੇ (likely (c < glyph_ਗਿਣਤੀ_2))
      vis_clusters[c] = hb_min (vis_clusters[c], ਬਫਰ->info[i].cluster);
  }
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 1; i < glyph_ਗਿਣਤੀ_2; i++)
    ਜੇ (vis_clusters[i] == (uint32_t) -1)
      vis_clusters[i] = vis_clusters[i - 1];
  ਲਈ (ਪੂਰਨ i = (ਪੂਰਨ) glyph_ਗਿਣਤੀ_2 - 2; i >= 0; i--)
    ਜੇ (vis_clusters[i] == (uint32_t) -1)
      vis_clusters[i] = vis_clusters[i + 1];

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ utf16_ਸੂਚਕਾਂਕ

  ਜੇ (unlikely (!ਬਫਰ->ensure (glyph_ਗਿਣਤੀ_2)))
    FAIL ("Buffer in error");

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ FAIL

  /* Set glyph infos */
  ਬਫਰ->len = 0;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < glyph_ਗਿਣਤੀ_2; i++)
  {
    hb_glyph_info_t *info = &ਬਫਰ->info[ਬਫਰ->len++];

    info->codepoint = glyphIndices[i];
    info->cluster = vis_clusters[i];

    /* The rest is crap.  Let's store position info there for now. */
    info->ਨਕਾਬ = (hb_ਨਕਾਬ_t) hb_clamp_ਮੰਜ਼ਿਲ<int32_t> (glyphAdvances[i]);
    info->var1.i32 = hb_clamp_ਮੰਜ਼ਿਲ<int32_t> (glyphOffsets[i].advance_ਵਿੱਥ);
    info->var2.i32 = hb_clamp_ਮੰਜ਼ਿਲ<int32_t> (glyphOffsets[i].ascender_ਵਿੱਥ);
  }

  /* Set glyph positions */
  ਬਫਰ->clear_positions ();
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < glyph_ਗਿਣਤੀ_2; i++)
  {
    hb_glyph_info_t *info = &ਬਫਰ->info[i];
    hb_glyph_position_t *pos = &ਬਫਰ->pos[i];

    /* TODO vertical */
    pos->x_advance = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> (round (x_mult * (int32_t) info->ਨਕਾਬ));
    ਦੁੱਗਣਾ x_ਵਿੱਥ_2 = is_ਸੱਜਾ_ਮੰਜ਼ਿਲ_ਖੱਬਾ ? -(ਦੁੱਗਣਾ) info->var1.i32 : info->var1.i32;
    pos->x_ਵਿੱਥ_2 = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> (round ((ਦੁੱਗਣਾ) x_mult * x_ਵਿੱਥ_2));
    pos->y_ਵਿੱਥ_2 = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> (round (y_mult * info->var2.i32));
  }

  ਜੇ (is_ਸੱਜਾ_ਮੰਜ਼ਿਲ_ਖੱਬਾ) hb_ਬਫਰ_ਉਲਟ (ਬਫਰ);

  ਬਫਰ->clear_glyph_flags ();
  ਬਫਰ->unsafe_ਮੰਜ਼ਿਲ_break ();

  /* Wow, done! */
  ਵਾਪਸ ਸੱਚ;
}


#ਜੇ_ਅੰਤ
