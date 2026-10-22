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

#అన్తర్భావ "hb.hh"

#నిర్వచితమైతే HAVE_DIRECTWRITE

#అన్తర్భావ "hb-shaper-impl.hh"

#అన్తర్భావ "hb-directwrite.hh"

#అన్తర్భావ "hb-ms-feature-ranges.hh"


/*
* shaper face data
*/

hb_directwrite_face_దత్తాంశం_t *
hb_directwrite_shaper_face_దత్తాంశం_create (hb_face_t *face)
{
  hb_blob_t *blob = hb_face_reference_blob (face);

  hb_directwrite_face_దత్తాంశం_t *దత్తాంశం = (hb_directwrite_face_దత్తాంశం_t *) dw_face_create (blob, face->సూచీ_2);

  hb_blob_destroy (blob);

  తిరిగి దత్తాంశం;
}

శూన్య
hb_directwrite_shaper_face_దత్తాంశం_destroy (hb_directwrite_face_దత్తాంశం_t *దత్తాంశం)
{
  ((IDWriteFontFace *) దత్తాంశం)->Release ();
}


/*
 * shaper font data
 */

నిర్మాణం hb_directwrite_font_దత్తాంశం_t {};

hb_directwrite_font_దత్తాంశం_t *
hb_directwrite_shaper_font_దత్తాంశం_create (hb_font_t *font)
{
  IDWriteFontFace *fontFace = (IDWriteFontFace *) (స్థిరరాశి శూన్య *) font->face->దత్తాంశం.directwrite;

  /*
   * Set up variations.
   */
  IDWriteFontFace5 *fontFaceVariations = శూన్యసూచిక;
  {
    IDWriteFontFace5 *fontFace5;
    ఒకవేళ (SUCCEEDED (fontFace->QueryInterface (__uuidof (IDWriteFontFace5), (శూన్య **) &fontFace5)))
    {
      IDWriteFontResource *fontResource;
      ఒకవేళ (SUCCEEDED (fontFace5->GetFontResource (&fontResource)))
      {
	hb_సదిశ_t<DWRITE_FONT_AXIS_విలువ> axis_విలువలు;
	ఒకవేళ (likely (axis_విలువలు.resize_exact (font->అంకె_coords)))
	{
	  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < font->అంకె_coords; i++)
	  {
	    hb_ot_var_axis_info_t info;
	    గుర్తులేని పూర్ణాంకం c = 1;
	    hb_ot_var_get_axis_infos (font->face, i, &c, &info);
	    axis_విలువలు[i].axis_ట్యాగ్ = (DWRITE_FONT_AXIS_ట్యాగ్) hb_uint32_మార్పిడి (info.ట్యాగ్_2);
	    axis_విలువలు[i].value = i < font->అంకె_coords ?
				   hb_clamp (font->design_coords[i], info.min_విలువ, info.max_విలువ) :
				   info.default_విలువ;
	  }

	  fontResource->CreateFontFace (DWRITE_FONT_SIMULATIONS::DWRITE_FONT_SIMULATIONS_NONE,
					axis_విలువలు.arrayZ, axis_విలువలు.నిడివి, &fontFaceVariations);
	}
	fontResource->Release ();
      }
      fontFace5->Release ();
    }
  }

  తిరిగి (hb_directwrite_font_దత్తాంశం_t *) fontFaceVariations;
}

శూన్య
hb_directwrite_shaper_font_దత్తాంశం_destroy (hb_directwrite_font_దత్తాంశం_t *దత్తాంశం)
{
  ((IDWriteFontFace *) (స్థిరరాశి శూన్య *) దత్తాంశం)->Release ();
}


// Most of TextAnalysis is originally written by Bas Schouten for Mozilla project
// but now is relicensed to MIT for HarfBuzz use
తరగతి పాఠ్యం_Analysis : బహిరంగ IDWrite_పాఠ్యం_Analysis_ఆధారం, బహిరంగ IDWrite_పాఠ్యం_Analysis_Sink
{
అంతరంగ:
  hb_reference_లెక్క_t m_Ref_లెక్క;
బహిరంగ:
  IFACEMETHOD (QueryInterface) (IID స్థిరరాశి& iid, బహిర్గతం_3 శూన్య** ppObject)
  { తిరిగి S_OK; }
  IFACEMETHOD_ (ULONG, కూడు_Ref) ()
  {
    తిరిగి m_Ref_లెక్క.inc () + 1;
  }
  IFACEMETHOD_ (ULONG, Release) ()
  {
    గుర్తుగల ref_లెక్క = m_Ref_లెక్క.dec () - 1;
    ప్రతిజ్ఞా (ref_లెక్క >= 0);
    ఒకవేళ (ref_లెక్క)
      తిరిగి ref_లెక్క;
    తిరిగి 0;
  }

  // A single contiguous run of characters containing the same analysis
  // results.
  నిర్మాణం నిరంతరం_2
  {
    uint32_t m_పాఠ్యం_ప్రారంభం;   // starting text position of this run
    uint32_t m_పాఠ్యం_నిడివి;  // number of contiguous code units covered
    uint32_t m_Glyph_ప్రారంభం;  // starting glyph in the glyphs array
    uint32_t m_Glyph_లెక్క;  // number of glyphs associated with this run
    //పాఠ్యం
    DWRITE_SCRIPT_ANALYSIS mScript;
    uint8_t mBidiLevel;
    తార్కిక mIsSideways;

    తార్కిక కలిగియుంది_పాఠ్యం_Position (uint32_t a_పాఠ్యం_Position) స్థిరరాశి
    {
      తిరిగి a_పాఠ్యం_Position >= m_పాఠ్యం_ప్రారంభం &&
	     a_పాఠ్యం_Position <  m_పాఠ్యం_ప్రారంభం + m_పాఠ్యం_నిడివి;
    }

    నిరంతరం_2 *తదుపరి_నిరంతరం;
  };

బహిరంగ:
  పాఠ్యం_Analysis (స్థిరరాశి wchar_t* పాఠ్యం, uint32_t పాఠ్యం_నిడివి_2,
		స్థిరరాశి wchar_t* localeName, DWRITE_READING_DIRECTION readingDirection)
	       : m_పాఠ్యం_నిడివి (పాఠ్యం_నిడివి_2), m_పాఠ్యం (పాఠ్యం), mLocaleName (localeName),
		 mReadingDirection (readingDirection), m_Current_నిరంతరం (శూన్యసూచిక)
  {
    m_Ref_లెక్క.init ();
  }
  ~పాఠ్యం_Analysis ()
  {
    // free runs, except mRunHead which is part of the TextAnalysis object
    కొరకు (నిరంతరం_2 *నిరంతరం = m_నిరంతరం_తల.తదుపరి_నిరంతరం; నిరంతరం;)
    {
      నిరంతరం_2 *orig_నిరంతరం = నిరంతరం;
      నిరంతరం = నిరంతరం->తదుపరి_నిరంతరం;
      hb_free (orig_నిరంతరం);
    }
  }

  STDMETHODIMP
  GenerateResults (IDWrite_పాఠ్యం_Analyzer* పాఠ్యం_Analyzer, నిరంతరం_2 **నిరంతరం_తల)
  {
    // Analyzes the text using the script analyzer and returns
    // the result as a series of runs.

    HRESULT hr = S_OK;

    // Initially start out with one result that covers the entire range.
    // This result will be subdivided by the analysis processes.
    m_నిరంతరం_తల.m_పాఠ్యం_ప్రారంభం = 0;
    m_నిరంతరం_తల.m_పాఠ్యం_నిడివి = m_పాఠ్యం_నిడివి;
    m_నిరంతరం_తల.mBidiLevel =
      (mReadingDirection == DWRITE_READING_DIRECTION_కుడి_గమ్యం_ఎడమ);
    m_నిరంతరం_తల.తదుపరి_నిరంతరం = శూన్యసూచిక;
    m_Current_నిరంతరం = &m_నిరంతరం_తల;

    // Call each of the analyzers in sequence, recording their results.
    ఒకవేళ (SUCCEEDED (hr = పాఠ్యం_Analyzer->AnalyzeScript (ఇది, 0, m_పాఠ్యం_నిడివి, ఇది)))
      *నిరంతరం_తల = &m_నిరంతరం_తల;

    తిరిగి hr;
  }

  // IDWriteTextAnalysisSource implementation

  IFACEMETHODIMP
  Get_పాఠ్యం_At_Position (uint32_t పాఠ్యం_Position,
		     బహిర్గతం_3 wchar_t స్థిరరాశి** పాఠ్యం_String,
		     బహిర్గతం_3 uint32_t* పాఠ్యం_నిడివి_2)
  {
    ఒకవేళ (పాఠ్యం_Position >= m_పాఠ్యం_నిడివి)
    {
      // No text at this position, valid query though.
      *పాఠ్యం_String = శూన్యసూచిక;
      *పాఠ్యం_నిడివి_2 = 0;
    }
    లేకపోతే
    {
      *పాఠ్యం_String = m_పాఠ్యం + పాఠ్యం_Position;
      *పాఠ్యం_నిడివి_2 = m_పాఠ్యం_నిడివి - పాఠ్యం_Position;
    }
    తిరిగి S_OK;
  }

  IFACEMETHODIMP
  Get_పాఠ్యం_Before_Position (uint32_t పాఠ్యం_Position,
			 బహిర్గతం_3 wchar_t స్థిరరాశి** పాఠ్యం_String,
			 బహిర్గతం_3 uint32_t* పాఠ్యం_నిడివి_2)
  {
    ఒకవేళ (పాఠ్యం_Position == 0 || పాఠ్యం_Position > m_పాఠ్యం_నిడివి)
    {
      // Either there is no text before here (== 0), or this
      // is an invalid position. The query is considered valid though.
      *పాఠ్యం_String = శూన్యసూచిక;
      *పాఠ్యం_నిడివి_2 = 0;
    }
    లేకపోతే
    {
      *పాఠ్యం_String = m_పాఠ్యం;
      *పాఠ్యం_నిడివి_2 = పాఠ్యం_Position;
    }
    తిరిగి S_OK;
  }

  IFACEMETHODIMP_ (DWRITE_READING_DIRECTION)
  GetParagraphReadingDirection () { తిరిగి mReadingDirection; }

  IFACEMETHODIMP GetLocaleName (uint32_t పాఠ్యం_Position, uint32_t* పాఠ్యం_నిడివి_2,
				wchar_t స్థిరరాశి** localeName)
  { తిరిగి S_OK; }

  IFACEMETHODIMP
  Get_సంఖ్య_Substitution (uint32_t పాఠ్యం_Position,
			 బహిర్గతం_3 uint32_t* పాఠ్యం_నిడివి_2,
			 బహిర్గతం_3 IDWrite_సంఖ్య_Substitution** సంఖ్య_Substitution)
  {
    // We do not support number substitution.
    *సంఖ్య_Substitution = శూన్యసూచిక;
    *పాఠ్యం_నిడివి_2 = m_పాఠ్యం_నిడివి - పాఠ్యం_Position;

    తిరిగి S_OK;
  }

  // IDWriteTextAnalysisSink implementation

  IFACEMETHODIMP
  సమితి_Script_Analysis (uint32_t పాఠ్యం_Position, uint32_t పాఠ్యం_నిడివి_2,
		     DWRITE_SCRIPT_ANALYSIS స్థిరరాశి* scriptAnalysis)
  {
    సమితి_Current_నిరంతరం (పాఠ్యం_Position);
    ఒకవేళ (unlikely (!విభజించు_Current_నిరంతరం (పాఠ్యం_Position)))
      తిరిగి E_OUTOFMEMORY;
    అయితే (పాఠ్యం_నిడివి_2 > 0)
    {
      నిరంతరం_2 *నిరంతరం = Fetch_తదుపరి_నిరంతరం (&పాఠ్యం_నిడివి_2);
      ఒకవేళ (unlikely (!నిరంతరం))
        తిరిగి E_OUTOFMEMORY;
      నిరంతరం->mScript = *scriptAnalysis;
    }

    తిరిగి S_OK;
  }

  IFACEMETHODIMP
  సమితి_పంక్తి_Breakpoints (uint32_t పాఠ్యం_Position,
		      uint32_t పాఠ్యం_నిడివి_2,
		      స్థిరరాశి DWRITE_పంక్తి_BREAKPOINT* పంక్తి_Breakpoints)
  { తిరిగి S_OK; }

  IFACEMETHODIMP సమితి_Bidi_Level (uint32_t పాఠ్యం_Position, uint32_t పాఠ్యం_నిడివి_2,
			       uint8_t explicitLevel, uint8_t resolvedLevel)
  { తిరిగి S_OK; }

  IFACEMETHODIMP
  సమితి_సంఖ్య_Substitution (uint32_t పాఠ్యం_Position, uint32_t పాఠ్యం_నిడివి_2,
			 IDWrite_సంఖ్య_Substitution* సంఖ్య_Substitution)
  { తిరిగి S_OK; }

రక్షిత:
  నిరంతరం_2 *Fetch_తదుపరి_నిరంతరం (లోపల_2 బహిర్గతం_3 uint32_t* పాఠ్యం_నిడివి_2)
  {
    // Used by the sink setters, this returns a reference to the next run.
    // Position and length are adjusted to now point after the current run
    // being returned.

    నిరంతరం_2 *orig_నిరంతరం = m_Current_నిరంతరం;
    // Split the tail if needed (the length remaining is less than the
    // current run's size).
    ఒకవేళ (*పాఠ్యం_నిడివి_2 < m_Current_నిరంతరం->m_పాఠ్యం_నిడివి)
    {
      ఒకవేళ (unlikely (!విభజించు_Current_నిరంతరం (m_Current_నిరంతరం->m_పాఠ్యం_ప్రారంభం + *పాఠ్యం_నిడివి_2)))
        తిరిగి శూన్యసూచిక;
    }
    లేకపోతే
      // Just advance the current run.
      m_Current_నిరంతరం = m_Current_నిరంతరం->తదుపరి_నిరంతరం;
    *పాఠ్యం_నిడివి_2 -= orig_నిరంతరం->m_పాఠ్యం_నిడివి;

    // Return a reference to the run that was just current.
    తిరిగి orig_నిరంతరం;
  }

  శూన్య సమితి_Current_నిరంతరం (uint32_t పాఠ్యం_Position)
  {
    // Move the current run to the given position.
    // Since the analyzers generally return results in a forward manner,
    // this will usually just return early. If not, find the
    // corresponding run for the text position.

    ఒకవేళ (m_Current_నిరంతరం && m_Current_నిరంతరం->కలిగియుంది_పాఠ్యం_Position (పాఠ్యం_Position))
      తిరిగి;

    కొరకు (నిరంతరం_2 *నిరంతరం = &m_నిరంతరం_తల; నిరంతరం; నిరంతరం = నిరంతరం->తదుపరి_నిరంతరం)
      ఒకవేళ (నిరంతరం->కలిగియుంది_పాఠ్యం_Position (పాఠ్యం_Position))
      {
	m_Current_నిరంతరం = నిరంతరం;
	తిరిగి;
      }
    ప్రతిజ్ఞా (0); // We should always be able to find the text position in one of our runs
  }

  తార్కిక విభజించు_Current_నిరంతరం (uint32_t విభజించు_Position)
  {
    ఒకవేళ (!m_Current_నిరంతరం)
    {
      ప్రతిజ్ఞా (0); // SplitCurrentRun called without current run
      // Shouldn't be calling this when no current run is set!
      తిరిగి అబద్ధం;
    }
    // Split the current run.
    ఒకవేళ (విభజించు_Position <= m_Current_నిరంతరం->m_పాఠ్యం_ప్రారంభం)
    {
      // No need to split, already the start of a run
      // or before it. Usually the first.
      తిరిగి నిజం;
    }
    నిరంతరం_2 *new_నిరంతరం = (నిరంతరం_2 *) hb_malloc (పరిమాణం (నిరంతరం_2));
    ఒకవేళ (unlikely (!new_నిరంతరం))
      తిరిగి అబద్ధం;

    *new_నిరంతరం = *m_Current_నిరంతరం;

    // Insert the new run in our linked list.
    new_నిరంతరం->తదుపరి_నిరంతరం = m_Current_నిరంతరం->తదుపరి_నిరంతరం;
    m_Current_నిరంతరం->తదుపరి_నిరంతరం = new_నిరంతరం;

    // Adjust runs' text positions and lengths.
    uint32_t విభజించు_బిందువు = విభజించు_Position - m_Current_నిరంతరం->m_పాఠ్యం_ప్రారంభం;
    new_నిరంతరం->m_పాఠ్యం_ప్రారంభం += విభజించు_బిందువు;
    new_నిరంతరం->m_పాఠ్యం_నిడివి -= విభజించు_బిందువు;
    m_Current_నిరంతరం->m_పాఠ్యం_నిడివి = విభజించు_బిందువు;
    m_Current_నిరంతరం = new_నిరంతరం;
    తిరిగి నిజం;
  }

రక్షిత:
  //ఆదానం
  // (weak references are fine here, since this class is a transient
  //  stack-based helper that doesn't need to copy data)
  uint32_t m_పాఠ్యం_నిడివి;
  స్థిరరాశి wchar_t* m_పాఠ్యం;
  స్థిరరాశి wchar_t* mLocaleName;
  DWRITE_READING_DIRECTION mReadingDirection;

  // Current processing state.
  నిరంతరం_2 *m_Current_నిరంతరం;

  // Output is a list of runs starting here
  నిరంతరం_2  m_నిరంతరం_తల;
};

/*
 * shaper
 */

hb_bool_t
_hb_directwrite_shape (hb_shape_plan_t    *shape_plan,
		       hb_font_t          *font,
		       hb_బఫరు_t        *బఫరు,
		       స్థిరరాశి hb_feature_t *features,
		       గుర్తులేని పూర్ణాంకం        అంకె_features)
{
  IDWriteFontFace *fontFace = (IDWriteFontFace *) (స్థిరరాశి శూన్య *) font->దత్తాంశం.directwrite;
  స్వయంచాలక *global = get_directwrite_global ();
  ఒకవేళ (unlikely (!global))
    తిరిగి అబద్ధం;
  IDWriteFactory *dwriteFactory = global->dwriteFactory;

  IDWrite_పాఠ్యం_Analyzer* analyzer;
  dwriteFactory->Create_పాఠ్యం_Analyzer (&analyzer);

  గుర్తులేని పూర్ణాంకం scratch_కొలత;
  hb_బఫరు_t::scratch_బఫరు_t *scratch = బఫరు->get_scratch_బఫరు (&scratch_కొలత);
#నిర్వచించు ALLOCATE_ARRAY(Type, name, len) \
  Type *name = (Type *) scratch; \
  చేయి { \
    గుర్తులేని పూర్ణాంకం _consumed = DIV_CEIL ((len) * పరిమాణం (Type), పరిమాణం (*scratch)); \
    ప్రతిజ్ఞా (_consumed <= scratch_కొలత); \
    scratch += _consumed; \
    scratch_కొలత -= _consumed; \
  } అయితే (0)

#నిర్వచించు utf16_సూచీ() var1.u32

  ALLOCATE_ARRAY (wchar_t, పాఠ్యం_String, బఫరు->len * 2);

  గుర్తులేని పూర్ణాంకం chars_len = 0;
  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < బఫరు->len; i++)
  {
    hb_codepoint_t c = బఫరు->info[i].codepoint;
    బఫరు->info[i].utf16_సూచీ () = chars_len;
    ఒకవేళ (likely (c <= 0xFFFFu))
      పాఠ్యం_String[chars_len++] = c;
    లేకపోతే ఒకవేళ (unlikely (c > 0x10FFFFu))
      పాఠ్యం_String[chars_len++] = 0xFFFDu;
    లేకపోతే
    {
      పాఠ్యం_String[chars_len++] = 0xD800u + ((c - 0x10000u) >> 10);
      పాఠ్యం_String[chars_len++] = 0xDC00u + ((c - 0x10000u) & ((1u << 10) - 1));
    }
  }

  ALLOCATE_ARRAY (మాట, log_clusters, chars_len);
#నిర్వచనం_రద్దు ALLOCATE_ARRAY
  /* Need log_clusters to assign features. */
  chars_len = 0;
  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < బఫరు->len; i++)
  {
    hb_codepoint_t c = బఫరు->info[i].codepoint;
    గుర్తులేని పూర్ణాంకం cluster = బఫరు->info[i].cluster;
    log_clusters[chars_len++] = cluster;
    ఒకవేళ (hb_లోపల_range (c, 0x10000u, 0x10FFFFu))
      log_clusters[chars_len++] = cluster; /* Surrogates. */
  }

  DWRITE_READING_DIRECTION readingDirection;
  readingDirection = బఫరు->props.direction ?
		     DWRITE_READING_DIRECTION_కుడి_గమ్యం_ఎడమ :
		     DWRITE_READING_DIRECTION_ఎడమ_గమ్యం_కుడి;

  /*
  * There's an internal 16-bit limit on some things inside the analyzer,
  * but we never attempt to shape a word longer than 64K characters
  * in a single gfxShapedWord, so we cannot exceed that limit.
  */
  uint32_t పాఠ్యం_నిడివి_2 = chars_len;

  పాఠ్యం_Analysis analysis (పాఠ్యం_String, పాఠ్యం_నిడివి_2, శూన్యసూచిక, readingDirection);
  పాఠ్యం_Analysis::నిరంతరం_2 *నిరంతరం_తల;
  HRESULT hr;
  hr = analysis.GenerateResults (analyzer, &నిరంతరం_తల);

#నిర్వచించు FAIL(...) \
  HB_STMT_ప్రారంభం { \
    DEBUG_సందేశం (DIRECTWRITE, శూన్యసూచిక, __VA_ARGS__); \
    తిరిగి అబద్ధం; \
  } HB_STMT_చివర

  ఒకవేళ (FAILED (hr))
    FAIL ("Analyzer failed to generate results.");

  uint32_t max_Glyph_లెక్క = 3 * పాఠ్యం_నిడివి_2 / 2 + 16;
  uint32_t glyph_లెక్క_2;
  తార్కిక is_కుడి_గమ్యం_ఎడమ = HB_DIRECTION_IS_BACKWARD (బఫరు->props.direction);

  స్థిరరాశి wchar_t localeName[20] = {0};
  ఒకవేళ (బఫరు->props.language)
    mbstowcs ((wchar_t*) localeName,
	      hb_language_గమ్యం_string (బఫరు->props.language), 20);

  /*
   * Set up features.
   */
  static_assert ((పరిమాణం (DWRITE_TYPOGRAPHIC_FEATURES) == పరిమాణం (hb_ms_features_t)), "");
  static_assert ((పరిమాణం (DWRITE_FONT_FEATURE) == పరిమాణం (hb_ms_feature_t)), "");
  hb_సదిశ_t<hb_ms_features_t *> range_features;
  hb_సదిశ_t<uint32_t> range_char_counts;

  // https://github.com/harfbuzz/harfbuzz/pull/5114
  // The data allocated by these two vectors are used by the above two, so they
  // should remain alive as long as the above two are.
  hb_సదిశ_t<hb_ms_feature_t> feature_రికార్డులు;
  hb_సదిశ_t<hb_ms_range_రికార్డు_t> range_రికార్డులు;
  ఒకవేళ (అంకె_features)
  {
    ఒకవేళ (hb_ms_అమరిక_features (features, అంకె_features, feature_రికార్డులు, range_రికార్డులు))
    {
      hb_ms_make_feature_ranges (feature_రికార్డులు,
				 range_రికార్డులు,
				 0,
				 chars_len,
				 log_clusters,
				 range_features,
				 range_char_counts);
    }
  }

  hb_సదిశ_t<uint16_t> clusterMap;
  hb_సదిశ_t<DWRITE_SHAPING_పాఠ్యం_PROPERTIES> పాఠ్యం_Properties;
  ఒకవేళ (unlikely (!clusterMap.resize_exact (పాఠ్యం_నిడివి_2) ||
                !పాఠ్యం_Properties.resize_exact (పాఠ్యం_నిడివి_2)))
    FAIL ("Failed to allocate shaping text data.");

  hb_సదిశ_t<uint16_t> glyphIndices;
  hb_సదిశ_t<DWRITE_SHAPING_GLYPH_PROPERTIES> glyphProperties;

retry_getglyphs:
  ఒకవేళ (unlikely (!glyphIndices.resize_exact (max_Glyph_లెక్క) ||
                !glyphProperties.resize_exact (max_Glyph_లెక్క)))
    FAIL ("Failed to allocate shaping glyph data.");

  hr = analyzer->GetGlyphs (పాఠ్యం_String,
			    chars_len,
			    fontFace,
			    అబద్ధం,
			    is_కుడి_గమ్యం_ఎడమ,
			    &నిరంతరం_తల->mScript,
			    localeName,
			    శూన్యసూచిక,
			    (స్థిరరాశి DWRITE_TYPOGRAPHIC_FEATURES**) range_features.arrayZ,
			    range_char_counts.arrayZ,
			    range_features.నిడివి,
			    max_Glyph_లెక్క,
			    clusterMap.arrayZ,
			    పాఠ్యం_Properties.arrayZ,
			    glyphIndices.arrayZ,
			    glyphProperties.arrayZ,
			    &glyph_లెక్క_2);

  ఒకవేళ (unlikely (hr == HRESULT_FROM_WIN32 (ERROR_INSUFFICIENT_బఫరు)))
  {
    max_Glyph_లెక్క *= 2;

    వెళ్ళు retry_getglyphs;
  }
  ఒకవేళ (FAILED (hr))
    FAIL ("Analyzer failed to get glyphs.");

  hb_సదిశ_t<దశాంశం> glyphAdvances;
  hb_సదిశ_t<DWRITE_GLYPH_స్థానభ్రంశం> glyphOffsets;
  ఒకవేళ (unlikely (!glyphAdvances.resize_exact (max_Glyph_లెక్క) ||
                !glyphOffsets.resize_exact (max_Glyph_లెక్క)))
    FAIL ("Failed to allocate glyph positioning data.");


  గుర్తులేని font_Em_కొలత = font->face->get_upem ();

  దశాంశం x_mult = font->x_multf;
  దశాంశం y_mult = font->y_multf;

  hr = analyzer->GetGlyphPlacements (పాఠ్యం_String,
				     clusterMap.arrayZ,
				     పాఠ్యం_Properties.arrayZ,
				     chars_len,
				     glyphIndices.arrayZ,
				     glyphProperties.arrayZ,
				     glyph_లెక్క_2,
				     fontFace,
				     font_Em_కొలత,
				     అబద్ధం,
				     is_కుడి_గమ్యం_ఎడమ,
				     &నిరంతరం_తల->mScript,
				     localeName,
				     (స్థిరరాశి DWRITE_TYPOGRAPHIC_FEATURES**) range_features.arrayZ,
				     range_char_counts.arrayZ,
				     range_features.నిడివి,
				     glyphAdvances.arrayZ,
				     glyphOffsets.arrayZ);

  ఒకవేళ (FAILED (hr))
    FAIL ("Analyzer failed to get glyph placements.");

  /* Ok, we've got everything we need, now compose output buffer,
   * very, *very*, carefully! */

  /* Calculate visual-clusters.  That's what we ship. */
  hb_సదిశ_t<uint32_t> vis_clusters;
  ఒకవేళ (unlikely (!vis_clusters.resize_exact (glyph_లెక్క_2)))
    FAIL ("Failed to allocate visual clusters.");

  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < glyph_లెక్క_2; i++)
    vis_clusters[i] = (uint32_t) -1;
  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < బఫరు->len; i++)
  {
    గుర్తులేని పూర్ణాంకం c = clusterMap[బఫరు->info[i].utf16_సూచీ ()];
    ఒకవేళ (likely (c < glyph_లెక్క_2))
      vis_clusters[c] = hb_min (vis_clusters[c], బఫరు->info[i].cluster);
  }
  కొరకు (గుర్తులేని పూర్ణాంకం i = 1; i < glyph_లెక్క_2; i++)
    ఒకవేళ (vis_clusters[i] == (uint32_t) -1)
      vis_clusters[i] = vis_clusters[i - 1];
  కొరకు (పూర్ణాంకం i = (పూర్ణాంకం) glyph_లెక్క_2 - 2; i >= 0; i--)
    ఒకవేళ (vis_clusters[i] == (uint32_t) -1)
      vis_clusters[i] = vis_clusters[i + 1];

#నిర్వచనం_రద్దు utf16_సూచీ

  ఒకవేళ (unlikely (!బఫరు->ensure (glyph_లెక్క_2)))
    FAIL ("Buffer in error");

#నిర్వచనం_రద్దు FAIL

  /* Set glyph infos */
  బఫరు->len = 0;
  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < glyph_లెక్క_2; i++)
  {
    hb_glyph_info_t *info = &బఫరు->info[బఫరు->len++];

    info->codepoint = glyphIndices[i];
    info->cluster = vis_clusters[i];

    /* The rest is crap.  Let's store position info there for now. */
    info->ముసుగు = (hb_ముసుగు_t) hb_clamp_గమ్యం<int32_t> (glyphAdvances[i]);
    info->var1.i32 = hb_clamp_గమ్యం<int32_t> (glyphOffsets[i].advance_స్థానభ్రంశం);
    info->var2.i32 = hb_clamp_గమ్యం<int32_t> (glyphOffsets[i].ascender_స్థానభ్రంశం);
  }

  /* Set glyph positions */
  బఫరు->clear_positions ();
  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < glyph_లెక్క_2; i++)
  {
    hb_glyph_info_t *info = &బఫరు->info[i];
    hb_glyph_position_t *pos = &బఫరు->pos[i];

    /* TODO vertical */
    pos->x_advance = hb_clamp_గమ్యం<hb_position_t> (round (x_mult * (int32_t) info->ముసుగు));
    ద్విగుణ x_స్థానభ్రంశం_2 = is_కుడి_గమ్యం_ఎడమ ? -(ద్విగుణ) info->var1.i32 : info->var1.i32;
    pos->x_స్థానభ్రంశం_2 = hb_clamp_గమ్యం<hb_position_t> (round ((ద్విగుణ) x_mult * x_స్థానభ్రంశం_2));
    pos->y_స్థానభ్రంశం_2 = hb_clamp_గమ్యం<hb_position_t> (round (y_mult * info->var2.i32));
  }

  ఒకవేళ (is_కుడి_గమ్యం_ఎడమ) hb_బఫరు_తిరగేయి (బఫరు);

  బఫరు->clear_glyph_flags ();
  బఫరు->unsafe_గమ్యం_break ();

  /* Wow, done! */
  తిరిగి నిజం;
}


#అయితే_ముగింపు
