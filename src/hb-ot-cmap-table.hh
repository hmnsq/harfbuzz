/*
 * Copyright © 2014  Google, Inc.
 *
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
 * Google Author(s): Behdad Esfahbod
 */

#నిర్వచితం_కాకపోతే HB_OT_CMAP_పట్టిక_HH
#నిర్వచించు HB_OT_CMAP_పట్టిక_HH

#అన్తర్భావ "hb-ot-os2-table.hh"
#అన్తర్భావ "hb-ot-shaper-arabic-pua.hh"
#అన్తర్భావ "hb-open-type.hh"
#అన్తర్భావ "hb-ot-dual.hh"
#అన్తర్భావ "hb-set.hh"
#అన్తర్భావ "hb-cache.hh"

/*
 * cmap -- Character to Glyph Index Mapping
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cmap
 */
#నిర్వచించు HB_OT_ట్యాగ్_cmap HB_ట్యాగ్('c','m','a','p')
#నిర్వచించు HB_OT_ట్యాగ్_DMAP HB_ట్యాగ్('D','M','A','P')

నామాకాశ OT {

స్థిర అంతఃవరుస uint8_t unicode_గమ్యం_macroman (hb_codepoint_t u)
{
  స్థిర స్థిరరాశి నిర్మాణం unicode_గమ్యం_macroman_t
  {
    uint16_t unicode;
    uint8_t macroman;
  }
  mapping[] =
  {
    { 0x00A0, 0xCA },
    { 0x00A1, 0xC1 },
    { 0x00A2, 0xA2 },
    { 0x00A3, 0xA3 },
    { 0x00A5, 0xB4 },
    { 0x00A7, 0xA4 },
    { 0x00A8, 0xAC },
    { 0x00A9, 0xA9 },
    { 0x00AA, 0xBB },
    { 0x00AB, 0xC7 },
    { 0x00AC, 0xC2 },
    { 0x00AE, 0xA8 },
    { 0x00AF, 0xF8 },
    { 0x00B0, 0xA1 },
    { 0x00B1, 0xB1 },
    { 0x00B4, 0xAB },
    { 0x00B5, 0xB5 },
    { 0x00B6, 0xA6 },
    { 0x00B7, 0xE1 },
    { 0x00B8, 0xFC },
    { 0x00BA, 0xBC },
    { 0x00BB, 0xC8 },
    { 0x00BF, 0xC0 },
    { 0x00C0, 0xCB },
    { 0x00C1, 0xE7 },
    { 0x00C2, 0xE5 },
    { 0x00C3, 0xCC },
    { 0x00C4, 0x80 },
    { 0x00C5, 0x81 },
    { 0x00C6, 0xAE },
    { 0x00C7, 0x82 },
    { 0x00C8, 0xE9 },
    { 0x00C9, 0x83 },
    { 0x00CA, 0xE6 },
    { 0x00CB, 0xE8 },
    { 0x00CC, 0xED },
    { 0x00CD, 0xEA },
    { 0x00CE, 0xEB },
    { 0x00CF, 0xEC },
    { 0x00D1, 0x84 },
    { 0x00D2, 0xF1 },
    { 0x00D3, 0xEE },
    { 0x00D4, 0xEF },
    { 0x00D5, 0xCD },
    { 0x00D6, 0x85 },
    { 0x00D8, 0xAF },
    { 0x00D9, 0xF4 },
    { 0x00DA, 0xF2 },
    { 0x00DB, 0xF3 },
    { 0x00DC, 0x86 },
    { 0x00DF, 0xA7 },
    { 0x00E0, 0x88 },
    { 0x00E1, 0x87 },
    { 0x00E2, 0x89 },
    { 0x00E3, 0x8B },
    { 0x00E4, 0x8A },
    { 0x00E5, 0x8C },
    { 0x00E6, 0xBE },
    { 0x00E7, 0x8D },
    { 0x00E8, 0x8F },
    { 0x00E9, 0x8E },
    { 0x00EA, 0x90 },
    { 0x00EB, 0x91 },
    { 0x00EC, 0x93 },
    { 0x00ED, 0x92 },
    { 0x00EE, 0x94 },
    { 0x00EF, 0x95 },
    { 0x00F1, 0x96 },
    { 0x00F2, 0x98 },
    { 0x00F3, 0x97 },
    { 0x00F4, 0x99 },
    { 0x00F5, 0x9B },
    { 0x00F6, 0x9A },
    { 0x00F7, 0xD6 },
    { 0x00F8, 0xBF },
    { 0x00F9, 0x9D },
    { 0x00FA, 0x9C },
    { 0x00FB, 0x9E },
    { 0x00FC, 0x9F },
    { 0x00FF, 0xD8 },
    { 0x0131, 0xF5 },
    { 0x0152, 0xCE },
    { 0x0153, 0xCF },
    { 0x0178, 0xD9 },
    { 0x0192, 0xC4 },
    { 0x02C6, 0xF6 },
    { 0x02C7, 0xFF },
    { 0x02D8, 0xF9 },
    { 0x02D9, 0xFA },
    { 0x02DA, 0xFB },
    { 0x02DB, 0xFE },
    { 0x02DC, 0xF7 },
    { 0x02DD, 0xFD },
    { 0x03A9, 0xBD },
    { 0x03C0, 0xB9 },
    { 0x2013, 0xD0 },
    { 0x2014, 0xD1 },
    { 0x2018, 0xD4 },
    { 0x2019, 0xD5 },
    { 0x201A, 0xE2 },
    { 0x201C, 0xD2 },
    { 0x201D, 0xD3 },
    { 0x201E, 0xE3 },
    { 0x2020, 0xA0 },
    { 0x2021, 0xE0 },
    { 0x2022, 0xA5 },
    { 0x2026, 0xC9 },
    { 0x2030, 0xE4 },
    { 0x2039, 0xDC },
    { 0x203A, 0xDD },
    { 0x2044, 0xDA },
    { 0x20AC, 0xDB },
    { 0x2122, 0xAA },
    { 0x2202, 0xB6 },
    { 0x2206, 0xC6 },
    { 0x220F, 0xB8 },
    { 0x2211, 0xB7 },
    { 0x221A, 0xC3 },
    { 0x221E, 0xB0 },
    { 0x222B, 0xBA },
    { 0x2248, 0xC5 },
    { 0x2260, 0xAD },
    { 0x2264, 0xB2 },
    { 0x2265, 0xB3 },
    { 0x25CA, 0xD7 },
    { 0xF8FF, 0xF0 },
    { 0xFB01, 0xDE },
    { 0xFB02, 0xDF },
  };
  స్వయంచాలక *c = hb_bsearch (u, mapping, ARRAY_నిడివి (mapping), పరిమాణం (mapping[0]),
			_hb_cmp_operator<uint16_t, uint16_t>);
  తిరిగి c ? c->macroman : 0;
}

నిర్మాణం CmapSubtableFormat0
{
  తార్కిక get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) స్థిరరాశి
  {
    hb_codepoint_t gid = codepoint < 256 ? glyphIdArray[codepoint] : 0;
    ఒకవేళ (unlikely (!gid))
      తిరిగి అబద్ధం;
    *glyph = gid;
    తిరిగి నిజం;
  }

  గుర్తులేని get_language () స్థిరరాశి
  {
    తిరిగి language;
  }

  శూన్య collect_unicodes (hb_సమితి_t *బహిర్గతం) స్థిరరాశి
  {
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < 256; i++)
      ఒకవేళ (glyphIdArray[i])
	బహిర్గతం->కూడు (i);
  }

  శూన్య collect_mapping (hb_సమితి_t *unicodes, /*బహిర్గతం*/
			hb_map_t *mapping /*బహిర్గతం*/) స్థిరరాశి
  {
    కొరకు (గుర్తులేని i = 0; i < 256; i++)
      ఒకవేళ (glyphIdArray[i])
      {
	hb_codepoint_t glyph = glyphIdArray[i];
	unicodes->కూడు (i);
	mapping->సమితి (i, glyph);
      }
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది));
  }

  రక్షిత:
  HBUINT16	format;		/* Format number is set to 0. */
  HBUINT16	నిడివి;		/* Byte length of this subtable. */
  HBUINT16	language;	/* Ignore. */
  HBUINT8	glyphIdArray[256];/* An array that maps character
				 * code to glyph index values. */
  బహిరంగ:
  DEFINE_కొలత_STATIC (6 + 256);
};

నిర్మాణం CmapSubtableFormat4
{
  మూస<రకం_పేరు Iterator,
      రకం_పేరు Writer,
	   hb_requires (hb_is_iterator (Iterator))>
  శూన్య గమ్యం_ranges (Iterator it, Writer& range_writer)
  {
    hb_codepoint_t ప్రారంభం_cp = 0, prev_నిరంతరం_ప్రారంభం_cp = 0, నిరంతరం_ప్రారంభం_cp = 0, చివర_cp = 0, చివరి_gid = 0;
    పూర్ణాంకం నిరంతరం_నిడివి = 0 , delta = 0, prev_delta = 0;

    గణన {
      మొదటి_SUB_RANGE,
      FOLLOWING_SUB_RANGE,
    } బహుళకం;

    అయితే (it) {
      // Start a new range
      {
        స్థిరరాశి స్వయంచాలక& pair = *it;
        ప్రారంభం_cp = pair.మొదటి;
        prev_నిరంతరం_ప్రారంభం_cp = ప్రారంభం_cp;
        నిరంతరం_ప్రారంభం_cp = ప్రారంభం_cp;
        చివర_cp = ప్రారంభం_cp;
        చివరి_gid = pair.second;
        నిరంతరం_నిడివి = 1;
        prev_delta = 0;
      }

      delta = చివరి_gid - ప్రారంభం_cp;
      బహుళకం = మొదటి_SUB_RANGE;
      it++;

      అయితే (it) {
        // Process range
        స్థిరరాశి స్వయంచాలక& pair = *it;
        hb_codepoint_t తదుపరి_cp = pair.మొదటి;
        hb_codepoint_t తదుపరి_gid = pair.second;
        ఒకవేళ (తదుపరి_cp != చివర_cp + 1) {
          // Current range is over, stop processing.
          ఆపు;
        }

        ఒకవేళ (తదుపరి_gid == చివరి_gid + 1) {
          // The current run continues.
          చివర_cp = తదుపరి_cp;
          నిరంతరం_నిడివి++;
          చివరి_gid = తదుపరి_gid;
          it++;
          కొనసాగు;
        }

        // A new run is starting, decide if we want to commit the current run.
        పూర్ణాంకం విభజించు_ఖర్చు = (బహుళకం == మొదటి_SUB_RANGE) ? 8 : 16;
        పూర్ణాంకం నిరంతరం_ఖర్చు = నిరంతరం_నిడివి * 2;
        ఒకవేళ (నిరంతరం_ఖర్చు >= విభజించు_ఖర్చు) {
          commit_current_range(ప్రారంభం_cp,
                               prev_నిరంతరం_ప్రారంభం_cp,
                               నిరంతరం_ప్రారంభం_cp,
                               చివర_cp,
                               delta,
                               prev_delta,
                               విభజించు_ఖర్చు,
                               range_writer);
          ప్రారంభం_cp = తదుపరి_cp;
        }

        // Start the new run
        బహుళకం = FOLLOWING_SUB_RANGE;
        prev_నిరంతరం_ప్రారంభం_cp = నిరంతరం_ప్రారంభం_cp;
        నిరంతరం_ప్రారంభం_cp = తదుపరి_cp;
        చివర_cp = తదుపరి_cp;
        prev_delta = delta;
        delta = తదుపరి_gid - నిరంతరం_ప్రారంభం_cp;
        నిరంతరం_నిడివి = 1;
        చివరి_gid = తదుపరి_gid;
        it++;
      }

      // Finalize range
      commit_current_range (ప్రారంభం_cp,
                            prev_నిరంతరం_ప్రారంభం_cp,
                            నిరంతరం_ప్రారంభం_cp,
                            చివర_cp,
                            delta,
                            prev_delta,
                            8,
                            range_writer);
    }

    ఒకవేళ (likely (చివర_cp != 0xFFFF)) {
      range_writer (0xFFFF, 0xFFFF, 1);
    }
  }

  /*
   * Writes the current range as either one or two ranges depending on what is most efficient.
   */
  మూస<రకం_పేరు Writer>
  శూన్య commit_current_range (hb_codepoint_t ప్రారంభం,
                             hb_codepoint_t prev_నిరంతరం_ప్రారంభం,
                             hb_codepoint_t నిరంతరం_ప్రారంభం,
                             hb_codepoint_t చివర,
                             పూర్ణాంకం నిరంతరం_delta,
                             పూర్ణాంకం మునుపటి_నిరంతరం_delta,
                             పూర్ణాంకం విభజించు_ఖర్చు,
                             Writer& range_writer) {
    తార్కిక should_విభజించు = అబద్ధం;
    ఒకవేళ (ప్రారంభం < నిరంతరం_ప్రారంభం && నిరంతరం_ప్రారంభం < చివర) {
      పూర్ణాంకం నిరంతరం_ఖర్చు = (చివర - నిరంతరం_ప్రారంభం + 1) * 2;
      ఒకవేళ (నిరంతరం_ఖర్చు >= విభజించు_ఖర్చు) {
        should_విభజించు = నిజం;
      }
    }

    // A zero delta is also the marker for ranges that need a
    // glyphIdArray; serialize_rangeoffset_glyid() tells the two cases
    // apart by re-checking the mapping.
    ఒకవేళ (should_విభజించు) {
      ఒకవేళ (ప్రారంభం == prev_నిరంతరం_ప్రారంభం)
        range_writer (ప్రారంభం, నిరంతరం_ప్రారంభం - 1, మునుపటి_నిరంతరం_delta);
      లేకపోతే
        range_writer (ప్రారంభం, నిరంతరం_ప్రారంభం - 1, 0);
      range_writer (నిరంతరం_ప్రారంభం, చివర, నిరంతరం_delta);
      తిరిగి;
    }


    ఒకవేళ (ప్రారంభం == నిరంతరం_ప్రారంభం) {
      // Range is only a run
      range_writer (ప్రారంభం, చివర, నిరంతరం_delta);
      తిరిగి;
    }

    // Write only a single non-run range.
    range_writer (ప్రారంభం, చివర, 0);
  }

  మూస<రకం_పేరు Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  గుర్తులేని serialize_కనుగొను_segcount (Iterator it) {
    నిర్మాణం Counter {
      గుర్తులేని segcount = 0;

      శూన్య క్రియాకారకం() (hb_codepoint_t ప్రారంభం,
                       hb_codepoint_t చివర,
                       పూర్ణాంకం delta) {
        segcount++;
      }
    } counter;

    గమ్యం_ranges (+it, counter);
    తిరిగి counter.segcount;
  }


  మూస<రకం_పేరు Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  తార్కిక serialize_ప్రారంభం_చివర_delta_arrays (hb_serialize_context_t *c,
                                         Iterator it,
                                         పూర్ణాంకం segcount)
  {
    నిర్మాణం Writer {
      hb_serialize_context_t *serializer_;
      HBUINT16* చివర_సంకేతం;
      HBUINT16* ప్రారంభం_సంకేతం;
      HBINT16* id_delta_;
      పూర్ణాంకం సూచీ_3;

      Writer(hb_serialize_context_t *serializer)
          : serializer_(serializer),
            చివర_సంకేతం(శూన్యసూచిక),
            ప్రారంభం_సంకేతం(శూన్యసూచిక),
            id_delta_(శూన్యసూచిక),
            సూచీ_3 (0) {}
      శూన్య క్రియాకారకం() (hb_codepoint_t ప్రారంభం,
                       hb_codepoint_t చివర,
                       పూర్ణాంకం delta) {
        ప్రారంభం_సంకేతం[సూచీ_3] = ప్రారంభం;
        చివర_సంకేతం[సూచీ_3] = చివర;
        id_delta_[సూచీ_3] = delta;
        సూచీ_3++;
      }
    } writer(c);

    writer.చివర_సంకేతం = c->allocate_కొలత<HBUINT16> (HBUINT16::static_కొలత * segcount, అబద్ధం);
    (శూన్య) c->allocate_కొలత<HBUINT16> (2); // padding
    writer.ప్రారంభం_సంకేతం = c->allocate_కొలత<HBUINT16> (HBUINT16::static_కొలత * segcount, అబద్ధం);
    writer.id_delta_ = c->allocate_కొలత<HBINT16> (HBINT16::static_కొలత * segcount, అబద్ధం);

    ఒకవేళ (unlikely (!writer.చివర_సంకేతం || !writer.ప్రారంభం_సంకేతం || !writer.id_delta_)) తిరిగి అబద్ధం;

    గమ్యం_ranges (+it, writer);
    తిరిగి నిజం;
  }

  మూస<రకం_పేరు Iterator,
          hb_requires (hb_is_iterator (Iterator))>
  HBUINT16* serialize_rangeoffset_glyid (hb_serialize_context_t *c,
                                         Iterator it,
					 HBUINT16 *చివర_సంకేతం_2,
					 HBUINT16 *ప్రారంభం_సంకేతం_2,
					 HBINT16 *idDelta,
					 గుర్తులేని segcount)
  {
    hb_map_t cp_గమ్యం_gid { it };

    HBUINT16 *id_Range_స్థానభ్రంశం = c->allocate_కొలత<HBUINT16> (HBUINT16::static_కొలత * segcount);
    ఒకవేళ (unlikely (!c->check_success (id_Range_స్థానభ్రంశం))) తిరిగి శూన్యసూచిక;
    ఒకవేళ (unlikely ((అక్షరం *)id_Range_స్థానభ్రంశం - (అక్షరం *)idDelta != (పూర్ణాంకం) segcount * (పూర్ణాంకం) HBINT16::static_కొలత)) తిరిగి శూన్యసూచిక;

    కొరకు (గుర్తులేని i : hb_range (segcount))
    {
      /* A non-zero delta is always a real run. A zero delta is
       * ambiguous: identity mapping, or the marker for a range that
       * needs a glyphIdArray. Only zero deltas need a check. */
      పూర్ణాంకం delta = idDelta[i];
      ఒకవేళ (delta)
	కొనసాగు;
      తార్కిక use_delta = నిజం;
      కొరకు (hb_codepoint_t cp = ప్రారంభం_సంకేతం_2[i]; cp <= చివర_సంకేతం_2[i]; cp++)
      {
	hb_codepoint_t gid = cp_గమ్యం_gid.get (cp);
	ఒకవేళ (gid == HB_MAP_విలువ_INVALID)
	  gid = 0;
	ఒకవేళ (gid != cp)
	{
	  use_delta = అబద్ధం;
	  ఆపు;
	}
      }
      ఒకవేళ (use_delta)
	కొనసాగు;

      id_Range_స్థానభ్రంశం[i] = 2 * (c->ప్రారంభం_embed<HBUINT16> () - id_Range_స్థానభ్రంశం - i);
      కొరకు (hb_codepoint_t cp = ప్రారంభం_సంకేతం_2[i]; cp <= చివర_సంకేతం_2[i]; cp++)
      {
        HBUINT16 gid;
        gid = cp_గమ్యం_gid[cp];
        c->copy<HBUINT16> (gid);
      }
    }

    తిరిగి id_Range_స్థానభ్రంశం;
  }

  మూస<రకం_పేరు Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  శూన్య serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    స్వయంచాలక format4_iter =
    + it
    | hb_filter ([&] (స్థిరరాశి hb_codepoint_pair_t _)
		 { తిరిగి _.మొదటి <= 0xFFFF; })
    ;

    ఒకవేళ (!format4_iter) తిరిగి;

    గుర్తులేని పట్టిక_initpos = c->నిడివి ();
    ఒకవేళ (unlikely (!c->extend_min (ఇది))) తిరిగి;
    ఇది->format = 4;

    hb_సదిశ_t<hb_codepoint_pair_t> cp_గమ్యం_gid {
      format4_iter
    };

    //serialize endCode[], startCode[], idDelta[]
    HBUINT16* చివర_సంకేతం_2 = c->ప్రారంభం_embed<HBUINT16> ();
    గుర్తులేని segcount = serialize_కనుగొను_segcount (cp_గమ్యం_gid.iter());
    ఒకవేళ (unlikely (!serialize_ప్రారంభం_చివర_delta_arrays (c, cp_గమ్యం_gid.iter(), segcount)))
      తిరిగి;

    HBUINT16 *ప్రారంభం_సంకేతం_2 = చివర_సంకేతం_2 + segcount + 1;
    HBINT16 *idDelta = ((HBINT16*)ప్రారంభం_సంకేతం_2) + segcount;

    HBUINT16 *id_Range_స్థానభ్రంశం = serialize_rangeoffset_glyid (c,
                                                           cp_గమ్యం_gid.iter (),
                                                           చివర_సంకేతం_2,
                                                           ప్రారంభం_సంకేతం_2,
                                                           idDelta,
                                                           segcount);
    ఒకవేళ (unlikely (!c->check_success (id_Range_స్థానభ్రంశం))) తిరిగి;

    ఇది->నిడివి = c->నిడివి () - పట్టిక_initpos;
    ఒకవేళ ((దీర్ఘ దీర్ఘ) ఇది->నిడివి != (దీర్ఘ దీర్ఘ) c->నిడివి () - పట్టిక_initpos)
    {
      c->err (HB_SERIALIZE_ERROR_INT_అధికప్రవాహం);
      తిరిగి;
    }

    ఇది->seg_లెక్క_X2 = segcount * 2;
    ఇది->నమోదు_Selector = hb_max (1u, hb_బిట్టు_storage (segcount)) - 1;
    ఇది->శోధన_Range = 2 * (1u << ఇది->నమోదు_Selector);
    ఇది->range_జరుపు = segcount * 2 > ఇది->శోధన_Range
		       ? 2 * segcount - ఇది->శోధన_Range
		       : 0;
  }

  గుర్తులేని get_language () స్థిరరాశి
  {
    తిరిగి language;
  }

  నిర్మాణం accelerator_t
  {
    accelerator_t () {}
    accelerator_t (స్థిరరాశి CmapSubtableFormat4 *subtable) = తొలగించు;

    శూన్య init (స్థిరరాశి CmapSubtableFormat4 *subtable,
	       గుర్తులేని పూర్ణాంకం              subtable_దత్తాంశం_కొలత)
    {
      seg_లెక్క_2 = subtable->seg_లెక్క_X2 / 2;
      చివర_లెక్క = subtable->values.arrayZ;
      ప్రారంభం_లెక్క = చివర_లెక్క + seg_లెక్క_2 + 1;
      idDelta = ప్రారంభం_లెక్క + seg_లెక్క_2;
      id_Range_స్థానభ్రంశం = idDelta + seg_లెక్క_2;
      glyphIdArray = id_Range_స్థానభ్రంశం + seg_లెక్క_2;

      గుర్తులేని పూర్ణాంకం విలువలు_స్థానభ్రంశం = 16 + 8 * seg_లెక్క_2;
      glyph_Id_Array_నిడివి = subtable_దత్తాంశం_కొలత > విలువలు_స్థానభ్రంశం
			   ? (subtable_దత్తాంశం_కొలత - విలువలు_స్థానభ్రంశం) / 2
			   : 0;
    }

    తార్కిక get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) స్థిరరాశి
    {
      నిర్మాణం CustomRange
      {
	పూర్ణాంకం cmp (hb_codepoint_t k,
		 గుర్తులేని distance) స్థిరరాశి
	{
	  ఒకవేళ (k > చివరి) తిరిగి +1;
	  ఒకవేళ (k < (&చివరి)[distance]/*మొదటి*/) తిరిగి -1;
	  తిరిగి 0;
	}
	HBUINT16 చివరి;
      };

      స్థిరరాశి HBUINT16 *దొరికింది = hb_bsearch (codepoint,
					  ఇది->చివర_లెక్క,
					  ఇది->seg_లెక్క_2,
					  పరిమాణం (CustomRange),
					  hb_cmp_పద్ధతి<hb_codepoint_t, CustomRange, గుర్తులేని>,
					  ఇది->seg_లెక్క_2 + 1);
      ఒకవేళ (unlikely (!దొరికింది))
	తిరిగి అబద్ధం;
      గుర్తులేని పూర్ణాంకం i = దొరికింది - చివర_లెక్క;

      hb_codepoint_t gid;
      గుర్తులేని పూర్ణాంకం range_స్థానభ్రంశం = ఇది->id_Range_స్థానభ్రంశం[i];
      ఒకవేళ (range_స్థానభ్రంశం == 0)
	gid = codepoint + ఇది->idDelta[i];
      లేకపోతే
      {
	/* Somebody has been smoking... */
	గుర్తులేని పూర్ణాంకం సూచీ_2 = range_స్థానభ్రంశం / 2 + (codepoint - ఇది->ప్రారంభం_లెక్క[i]) + i - ఇది->seg_లెక్క_2;
	ఒకవేళ (unlikely (సూచీ_2 >= ఇది->glyph_Id_Array_నిడివి))
	  తిరిగి అబద్ధం;
	gid = ఇది->glyphIdArray[సూచీ_2];
	ఒకవేళ (unlikely (!gid))
	  తిరిగి అబద్ధం;
	gid += ఇది->idDelta[i];
      }
      gid &= 0xFFFFu;
      ఒకవేళ (unlikely (!gid))
	తిరిగి అబద్ధం;
      *glyph = gid;
      తిరిగి నిజం;
    }

    HB_అంతర్గత స్థిర తార్కిక get_glyph_func (స్థిరరాశి శూన్య *obj, hb_codepoint_t codepoint, hb_codepoint_t *glyph)
    { తిరిగి ((స్థిరరాశి accelerator_t *) obj)->get_glyph (codepoint, glyph); }

    శూన్య collect_unicodes (hb_సమితి_t *బహిర్గతం) స్థిరరాశి
    {
      గుర్తులేని పూర్ణాంకం లెక్క = ఇది->seg_లెక్క_2;
      ఒకవేళ (లెక్క && ఇది->ప్రారంభం_లెక్క[లెక్క - 1] == 0xFFFFu)
	లెక్క--; /* Skip sentinel segment. */
      కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
      {
	hb_codepoint_t ప్రారంభం = ఇది->ప్రారంభం_లెక్క[i];
	hb_codepoint_t చివర = ఇది->చివర_లెక్క[i];
	గుర్తులేని పూర్ణాంకం range_స్థానభ్రంశం = ఇది->id_Range_స్థానభ్రంశం[i];
        బహిర్గతం->కూడు_range(ప్రారంభం, చివర);
	ఒకవేళ (range_స్థానభ్రంశం == 0)
	{
	  కొరకు (hb_codepoint_t codepoint = ప్రారంభం; codepoint <= చివర; codepoint++)
	  {
	    hb_codepoint_t gid = (codepoint + ఇది->idDelta[i]) & 0xFFFFu;
	    ఒకవేళ (unlikely (!gid))
              బహిర్గతం->del(codepoint);
	  }
	}
	లేకపోతే
	{
	  కొరకు (hb_codepoint_t codepoint = ప్రారంభం; codepoint <= చివర; codepoint++)
	  {
	    గుర్తులేని పూర్ణాంకం సూచీ_2 = range_స్థానభ్రంశం / 2 + (codepoint - ఇది->ప్రారంభం_లెక్క[i]) + i - ఇది->seg_లెక్క_2;
	    ఒకవేళ (unlikely (సూచీ_2 >= ఇది->glyph_Id_Array_నిడివి))
            {
              బహిర్గతం->del_range (codepoint, చివర);
	      ఆపు;
            }
	    hb_codepoint_t gid = ఇది->glyphIdArray[సూచీ_2];
	    ఒకవేళ (unlikely (!gid))
              బహిర్గతం->del(codepoint);
	  }
	}
      }
    }

    శూన్య collect_mapping (hb_సమితి_t *unicodes, /*బహిర్గతం*/
			  hb_map_t *mapping /*బహిర్గతం*/) స్థిరరాశి
    {
      // TODO(grieger): optimize similar to collect_unicodes
      // (ie. use add_range())
      గుర్తులేని లెక్క = ఇది->seg_లెక్క_2;
      ఒకవేళ (లెక్క && ఇది->ప్రారంభం_లెక్క[లెక్క - 1] == 0xFFFFu)
	లెక్క--; /* Skip sentinel segment. */
      కొరకు (గుర్తులేని i = 0; i < లెక్క; i++)
      {
	hb_codepoint_t ప్రారంభం = ఇది->ప్రారంభం_లెక్క[i];
	hb_codepoint_t చివర = ఇది->చివర_లెక్క[i];
	గుర్తులేని range_స్థానభ్రంశం = ఇది->id_Range_స్థానభ్రంశం[i];
	ఒకవేళ (range_స్థానభ్రంశం == 0)
	{
	  కొరకు (hb_codepoint_t codepoint = ప్రారంభం; codepoint <= చివర; codepoint++)
	  {
	    hb_codepoint_t gid = (codepoint + ఇది->idDelta[i]) & 0xFFFFu;
	    ఒకవేళ (unlikely (!gid))
	      కొనసాగు;
	    unicodes->కూడు (codepoint);
	    mapping->సమితి (codepoint, gid);
	  }
	}
	లేకపోతే
	{
	  కొరకు (hb_codepoint_t codepoint = ప్రారంభం; codepoint <= చివర; codepoint++)
	  {
	    గుర్తులేని సూచీ_2 = range_స్థానభ్రంశం / 2 + (codepoint - ఇది->ప్రారంభం_లెక్క[i]) + i - ఇది->seg_లెక్క_2;
	    ఒకవేళ (unlikely (సూచీ_2 >= ఇది->glyph_Id_Array_నిడివి))
	      ఆపు;
	    hb_codepoint_t gid = ఇది->glyphIdArray[సూచీ_2];
	    ఒకవేళ (unlikely (!gid))
	      కొనసాగు;
            gid = (gid + ఇది->idDelta[i]) & 0xFFFFu;
	    unicodes->కూడు (codepoint);
	    mapping->సమితి (codepoint, gid);
	  }
	}
      }
    }

    స్థిరరాశి HBUINT16 *చివర_లెక్క;
    స్థిరరాశి HBUINT16 *ప్రారంభం_లెక్క;
    స్థిరరాశి HBUINT16 *idDelta;
    స్థిరరాశి HBUINT16 *id_Range_స్థానభ్రంశం;
    స్థిరరాశి HBUINT16 *glyphIdArray;
    గుర్తులేని పూర్ణాంకం seg_లెక్క_2;
    గుర్తులేని పూర్ణాంకం glyph_Id_Array_నిడివి;
  };

  తార్కిక get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph,
		  గుర్తులేని పూర్ణాంకం subtable_దత్తాంశం_కొలత) స్థిరరాశి
  {
    accelerator_t accel;
    accel.init (ఇది, subtable_దత్తాంశం_కొలత);
    తిరిగి accel.get_glyph_func (&accel, codepoint, glyph);
  }
  తార్కిక get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) స్థిరరాశి
  { తిరిగి అబద్ధం; }

  శూన్య collect_unicodes (hb_సమితి_t *బహిర్గతం, గుర్తులేని పూర్ణాంకం subtable_దత్తాంశం_కొలత) స్థిరరాశి
  {
    accelerator_t accel;
    accel.init (ఇది, subtable_దత్తాంశం_కొలత);
    accel.collect_unicodes (బహిర్గతం);
  }
  శూన్య collect_unicodes (hb_సమితి_t *బహిర్గతం) స్థిరరాశి
  { collect_unicodes (బహిర్గతం, నిడివి); }

  శూన్య collect_mapping (hb_సమితి_t *unicodes, /*బహిర్గతం*/
			hb_map_t *mapping, /*బహిర్గతం*/
			గుర్తులేని పూర్ణాంకం subtable_దత్తాంశం_కొలత) స్థిరరాశి
  {
    accelerator_t accel;
    accel.init (ఇది, subtable_దత్తాంశం_కొలత);
    accel.collect_mapping (unicodes, mapping);
  }
  శూన్య collect_mapping (hb_సమితి_t *unicodes, /*బహిర్గతం*/
			hb_map_t *mapping /*బహిర్గతం*/) స్థిరరాశి
  { collect_mapping (unicodes, mapping, నిడివి); }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    ఒకవేళ (unlikely (!c->check_struct (ఇది)))
      return_trace (అబద్ధం);
    hb_barrier ();

    return_trace (c->check_range (values, 2 + 4 * seg_లెక్క_X2));
  }

  రక్షిత:
  HBUINT16	format;		/* Format number is set to 4. */
  HBUINT16	నిడివి;		/* This is the length in bytes of the
				 * subtable. */
  HBUINT16	language;	/* Ignore. */
  HBUINT16	seg_లెక్క_X2;	/* 2 x segCount. */
  HBUINT16	శోధన_Range;	/* 2 * (2**floor(log2(segCount))) */
  HBUINT16	నమోదు_Selector;	/* log2(searchRange/2) */
  HBUINT16	range_జరుపు;	/* 2 x segCount - searchRange */

  Unsized_Array_యొక్క<HBUINT16>
		values;
#ఒకవేళ 0
  HBUINT16	చివర_లెక్క[seg_లెక్క_2];	/* End characterCode for each segment,
					 * last=0xFFFFu. */
  HBUINT16	reservedPad;		/* Set to 0. */
  HBUINT16	ప్రారంభం_లెక్క[seg_లెక్క_2];	/* Start character code for each segment. */
  HBINT16		idDelta[seg_లెక్క_2];	/* Delta for all character codes in segment. */
  HBUINT16	id_Range_స్థానభ్రంశం[seg_లెక్క_2];/* Offsets into glyphIdArray or 0 */
  Unsized_Array_యొక్క<HBUINT16>
		glyphIdArray;	/* Glyph index array (arbitrary length) */
#అయితే_ముగింపు

  బహిరంగ:
  DEFINE_కొలత_ARRAY (14, values);
};

నిర్మాణం Cmap_Subtable_Long_సమూహం
{
  మిత్ర నిర్మాణం CmapSubtableFormat12;
  మిత్ర నిర్మాణం CmapSubtableFormat13;
  మూస<రకం_పేరు U>
  మిత్ర నిర్మాణం CmapSubtableLongSegmented;
  మిత్ర నిర్మాణం cmap;

  పూర్ణాంకం cmp (hb_codepoint_t codepoint) స్థిరరాశి
  {
    ఒకవేళ (codepoint < ప్రారంభం_Char_సంకేతం) తిరిగి -1;
    ఒకవేళ (codepoint > చివర_Char_సంకేతం)   తిరిగి +1;
    తిరిగి 0;
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది));
  }

  అంతరంగ:
  HBUINT32		ప్రారంభం_Char_సంకేతం;	/* First character code in this group. */
  HBUINT32		చివర_Char_సంకేతం;	/* Last character code in this group. */
  HBUINT32		glyphID;	/* Glyph index; interpretation depends on
					 * subtable format. */
  బహిరంగ:
  DEFINE_కొలత_STATIC (12);
};
DECLARE_NULL_NAMESPACE_బైట్లు (OT, Cmap_Subtable_Long_సమూహం);

మూస <రకం_పేరు UINT>
నిర్మాణం CmapSubtableTrimmed
{
  తార్కిక get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) స్థిరరాశి
  {
    /* Rely on our implicit array bound-checking. */
    hb_codepoint_t gid = glyphIdArray[codepoint - ప్రారంభం_Char_సంకేతం];
    ఒకవేళ (unlikely (!gid))
      తిరిగి అబద్ధం;
    *glyph = gid;
    తిరిగి నిజం;
  }

  గుర్తులేని get_language () స్థిరరాశి
  {
    తిరిగి language;
  }

  శూన్య collect_unicodes (hb_సమితి_t *బహిర్గతం) స్థిరరాశి
  {
    hb_codepoint_t ప్రారంభం = ప్రారంభం_Char_సంకేతం;
    గుర్తులేని పూర్ణాంకం లెక్క = glyphIdArray.len;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
      ఒకవేళ (glyphIdArray[i])
	బహిర్గతం->కూడు (ప్రారంభం + i);
  }

  శూన్య collect_mapping (hb_సమితి_t *unicodes, /*బహిర్గతం*/
			hb_map_t *mapping /*బహిర్గతం*/) స్థిరరాశి
  {
    hb_codepoint_t ప్రారంభం_cp = ప్రారంభం_Char_సంకేతం;
    గుర్తులేని లెక్క = glyphIdArray.len;
    కొరకు (గుర్తులేని i = 0; i < లెక్క; i++)
      ఒకవేళ (glyphIdArray[i])
      {
	hb_codepoint_t unicode = ప్రారంభం_cp + i;
	hb_codepoint_t glyphid = glyphIdArray[i];
	unicodes->కూడు (unicode);
	mapping->సమితి (unicode, glyphid);
      }
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) && glyphIdArray.sanitize (c));
  }

  రక్షిత:
  UINT		formatReserved;	/* Subtable format and (maybe) padding. */
  UINT		నిడివి;		/* Byte length of this subtable. */
  UINT		language;	/* Ignore. */
  UINT		ప్రారంభం_Char_సంకేతం;	/* First character code covered. */
  Array_యొక్క<HBGlyphID16, UINT>
		glyphIdArray;	/* Array of glyph index values for character
				 * codes in the range. */
  బహిరంగ:
  DEFINE_కొలత_ARRAY (5 * పరిమాణం (UINT), glyphIdArray);
};

నిర్మాణం CmapSubtableFormat6  : CmapSubtableTrimmed<HBUINT16> {};
నిర్మాణం CmapSubtableFormat10 : CmapSubtableTrimmed<HBUINT32> {};

మూస <రకం_పేరు T>
నిర్మాణం CmapSubtableLongSegmented
{
  మిత్ర నిర్మాణం cmap;

  తార్కిక get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) స్థిరరాశి
  {
    hb_codepoint_t gid = T::సమూహం_get_glyph (groups.bsearch (codepoint), codepoint);
    ఒకవేళ (unlikely (!gid))
      తిరిగి అబద్ధం;
    *glyph = gid;
    తిరిగి నిజం;
  }

  గుర్తులేని get_language () స్థిరరాశి
  {
    తిరిగి language;
  }

  శూన్య collect_unicodes (hb_సమితి_t *బహిర్గతం, గుర్తులేని పూర్ణాంకం అంకె_glyphs) స్థిరరాశి
  {
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < ఇది->groups.len; i++)
    {
      hb_codepoint_t ప్రారంభం = ఇది->groups[i].ప్రారంభం_Char_సంకేతం;
      hb_codepoint_t చివర = hb_min ((hb_codepoint_t) ఇది->groups[i].చివర_Char_సంకేతం,
				   (hb_codepoint_t) HB_UNICODE_MAX);
      hb_codepoint_t gid = ఇది->groups[i].glyphID;
      ఒకవేళ (!gid)
      {
	/* Intention is: if (hb_is_same (T, CmapSubtableFormat13)) continue; */
	ఒకవేళ (! T::సమూహం_get_glyph (ఇది->groups[i], చివర)) కొనసాగు;
	ప్రారంభం++;
	gid++;
      }
      ఒకవేళ (unlikely ((గుర్తులేని పూర్ణాంకం) gid >= అంకె_glyphs)) కొనసాగు;
      ఒకవేళ (unlikely ((గుర్తులేని పూర్ణాంకం) (gid + చివర - ప్రారంభం) >= అంకె_glyphs))
	చివర = ప్రారంభం + (hb_codepoint_t) అంకె_glyphs - gid;

      బహిర్గతం->కూడు_range (ప్రారంభం, hb_min (చివర, 0x10FFFFu));
    }
  }

  శూన్య collect_mapping (hb_సమితి_t *unicodes, /*బహిర్గతం*/
			hb_map_t *mapping, /*బహిర్గతం*/
			గుర్తులేని అంకె_glyphs) స్థిరరాశి
  {
    hb_codepoint_t చివరి_చివర = 0;
    గుర్తులేని లెక్క = ఇది->groups.len;
    కొరకు (గుర్తులేని i = 0; i < లెక్క; i++)
    {
      hb_codepoint_t ప్రారంభం = ఇది->groups.arrayZ[i].ప్రారంభం_Char_సంకేతం;
      hb_codepoint_t చివర = hb_min ((hb_codepoint_t) ఇది->groups.arrayZ[i].చివర_Char_సంకేతం,
				   (hb_codepoint_t) HB_UNICODE_MAX);
      ఒకవేళ (unlikely (ప్రారంభం > చివర || ప్రారంభం < చివరి_చివర)) {
        // Range is not in order and is invalid, skip it.
        కొనసాగు;
      }
      చివరి_చివర = చివర;


      hb_codepoint_t gid = ఇది->groups.arrayZ[i].glyphID;
      ఒకవేళ (!gid)
      {
        ఒకవేళ (T::format_సంఖ్య == 13) కొనసాగు;
	ప్రారంభం++;
	gid++;
      }
      ఒకవేళ (unlikely ((గుర్తులేని పూర్ణాంకం) gid >= అంకె_glyphs)) కొనసాగు;
      ఒకవేళ (unlikely ((గుర్తులేని పూర్ణాంకం) (gid + చివర - ప్రారంభం) >= అంకె_glyphs))
	చివర = ప్రారంభం + (hb_codepoint_t) అంకె_glyphs - gid;

      mapping->alloc (mapping->get_population () + చివర - ప్రారంభం + 1);

      unicodes->కూడు_range (ప్రారంభం, చివర);
      కొరకు (గుర్తులేని cp = ప్రారంభం; cp <= చివర; cp++)
      {
	mapping->సమితి (cp, gid);
        gid += T::increment;
      }
    }
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) && groups.sanitize (c));
  }

  రక్షిత:
  HBUINT16	format;		/* Subtable format; set to 12. */
  HBUINT16	reserved;	/* Reserved; set to 0. */
  HBUINT32	నిడివి;		/* Byte length of this subtable. */
  HBUINT32	language;	/* Ignore. */
  Sorted_Array32_యొక్క<Cmap_Subtable_Long_సమూహం>
		groups;		/* Groupings. */
  బహిరంగ:
  DEFINE_కొలత_ARRAY (16, groups);
};

నిర్మాణం CmapSubtableFormat12 : CmapSubtableLongSegmented<CmapSubtableFormat12>
{
  స్థిర constexpr పూర్ణాంకం increment = 1;
  స్థిర constexpr పూర్ణాంకం format_సంఖ్య = 12;

  స్థిర hb_codepoint_t సమూహం_get_glyph (స్థిరరాశి Cmap_Subtable_Long_సమూహం &సమూహం,
					 hb_codepoint_t u)
  { తిరిగి likely (సమూహం.ప్రారంభం_Char_సంకేతం <= సమూహం.చివర_Char_సంకేతం) ?
	   సమూహం.glyphID + (u - సమూహం.ప్రారంభం_Char_సంకేతం) : 0; }


  మూస<రకం_పేరు Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  శూన్య serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    ఒకవేళ (!it) తిరిగి;
    గుర్తులేని పట్టిక_initpos = c->నిడివి ();
    ఒకవేళ (unlikely (!c->extend_min (ఇది))) తిరిగి;

    hb_codepoint_t ప్రారంభం_Char_సంకేతం = (hb_codepoint_t) -1, చివర_Char_సంకేతం = (hb_codepoint_t) -1;
    hb_codepoint_t glyphID = 0;

    కొరకు (స్థిరరాశి స్వయంచాలక& _ : +it)
    {
      ఒకవేళ (ప్రారంభం_Char_సంకేతం == (hb_codepoint_t) -1)
      {
	ప్రారంభం_Char_సంకేతం = _.మొదటి;
	చివర_Char_సంకేతం = _.మొదటి;
	glyphID = _.second;
      }
      లేకపోతే ఒకవేళ (!_is_gid_consecutive (చివర_Char_సంకేతం, ప్రారంభం_Char_సంకేతం, glyphID, _.మొదటి, _.second))
      {
	Cmap_Subtable_Long_సమూహం  grouprecord;
	grouprecord.ప్రారంభం_Char_సంకేతం = ప్రారంభం_Char_సంకేతం;
	grouprecord.చివర_Char_సంకేతం = చివర_Char_సంకేతం;
	grouprecord.glyphID = glyphID;
	c->copy<Cmap_Subtable_Long_సమూహం> (grouprecord);

	ప్రారంభం_Char_సంకేతం = _.మొదటి;
	చివర_Char_సంకేతం = _.మొదటి;
	glyphID = _.second;
      }
      లేకపోతే
	చివర_Char_సంకేతం = _.మొదటి;
    }

    Cmap_Subtable_Long_సమూహం రికార్డు;
    రికార్డు.ప్రారంభం_Char_సంకేతం = ప్రారంభం_Char_సంకేతం;
    రికార్డు.చివర_Char_సంకేతం = చివర_Char_సంకేతం;
    రికార్డు.glyphID = glyphID;
    c->copy<Cmap_Subtable_Long_సమూహం> (రికార్డు);

    ఇది->format = 12;
    ఇది->reserved = 0;
    ఇది->నిడివి = c->నిడివి () - పట్టిక_initpos;
    ఇది->groups.len = (ఇది->నిడివి - min_కొలత) / Cmap_Subtable_Long_సమూహం::static_కొలత;
  }

  స్థిర size_t get_sub_పట్టిక_కొలత (స్థిరరాశి hb_sorted_సదిశ_t<Cmap_Subtable_Long_సమూహం> &groups_దత్తాంశం)
  { తిరిగి 16 + 12 * groups_దత్తాంశం.నిడివి; }

  అంతరంగ:
  స్థిర తార్కిక _is_gid_consecutive (hb_codepoint_t చివర_Char_సంకేతం,
				   hb_codepoint_t ప్రారంభం_Char_సంకేతం,
				   hb_codepoint_t glyphID,
				   hb_codepoint_t cp,
				   hb_codepoint_t new_gid)
  {
    తిరిగి (cp - 1 == చివర_Char_సంకేతం) &&
	new_gid == glyphID + (cp - ప్రారంభం_Char_సంకేతం);
  }

};

నిర్మాణం CmapSubtableFormat13 : CmapSubtableLongSegmented<CmapSubtableFormat13>
{
  స్థిర constexpr పూర్ణాంకం increment = 0;
  స్థిర constexpr పూర్ణాంకం format_సంఖ్య = 13;

  స్థిర hb_codepoint_t సమూహం_get_glyph (స్థిరరాశి Cmap_Subtable_Long_సమూహం &సమూహం,
					 hb_codepoint_t u HB_వాడని)
  { తిరిగి సమూహం.glyphID; }
};

రకనిర్వచనం గణన
{
  GLYPH_VARIANT_NOT_దొరికింది = 0,
  GLYPH_VARIANT_దొరికింది = 1,
  GLYPH_VARIANT_USE_DEFAULT = 2
} glyph_variant_t;

నిర్మాణం Unicode_విలువ_Range
{
  పూర్ణాంకం cmp (స్థిరరాశి hb_codepoint_t &codepoint) స్థిరరాశి
  {
    ఒకవేళ (codepoint < ప్రారంభం_Unicode_విలువ) తిరిగి -1;
    ఒకవేళ (codepoint > ప్రారంభం_Unicode_విలువ + additional_లెక్క) తిరిగి +1;
    తిరిగి 0;
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది));
  }

  HBUINT24	ప్రారంభం_Unicode_విలువ;	/* First value in this range. */
  HBUINT8	additional_లెక్క;	/* Number of additional values in this
					 * range. */
  బహిరంగ:
  DEFINE_కొలత_STATIC (4);
};

నిర్మాణం DefaultUVS : Sorted_Array32_యొక్క<Unicode_విలువ_Range>
{
  శూన్య collect_unicodes (hb_సమితి_t *బహిర్గతం) స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం లెక్క = len;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
    {
      hb_codepoint_t మొదటి = arrayZ[i].ప్రారంభం_Unicode_విలువ;
      hb_codepoint_t చివరి = hb_min ((hb_codepoint_t) (మొదటి + arrayZ[i].additional_లెక్క),
				    (hb_codepoint_t) HB_UNICODE_MAX);
      బహిర్గతం->కూడు_range (మొదటి, చివరి);
    }
  }

  DefaultUVS* copy (hb_serialize_context_t *c,
		    స్థిరరాశి hb_సమితి_t *unicodes) స్థిరరాశి
  {
    స్వయంచాలక *బహిర్గతం = c->ప్రారంభం_embed<DefaultUVS> ();
    స్వయంచాలక snap = c->snapshot ();

    HBUINT32 len;
    len = 0;
    ఒకవేళ (unlikely (!c->copy<HBUINT32> (len))) తిరిగి శూన్యసూచిక;
    గుర్తులేని init_len = c->నిడివి ();

    ఒకవేళ (ఇది->len > unicodes->get_population () * hb_బిట్టు_storage ((గుర్తులేని) ఇది->len))
    {
      hb_codepoint_t ప్రారంభం = HB_సమితి_విలువ_INVALID;
      hb_codepoint_t చివర = HB_సమితి_విలువ_INVALID;

      కొరకు (స్వయంచాలక u : *unicodes)
      {
        ఒకవేళ (!as_array ().bsearch (u))
	  కొనసాగు;
	ఒకవేళ (ప్రారంభం == HB_సమితి_విలువ_INVALID)
	{
	  ప్రారంభం = u;
	  చివర = ప్రారంభం - 1;
	}
	ఒకవేళ (చివర + 1 != u || చివర - ప్రారంభం == 255)
        {
	  Unicode_విలువ_Range rec;
	  rec.ప్రారంభం_Unicode_విలువ = ప్రారంభం;
	  rec.additional_లెక్క = చివర - ప్రారంభం;
	  c->copy<Unicode_విలువ_Range> (rec);
	  ప్రారంభం = u;
	}
	చివర = u;
      }
      ఒకవేళ (ప్రారంభం != HB_సమితి_విలువ_INVALID)
      {
	Unicode_విలువ_Range rec;
	rec.ప్రారంభం_Unicode_విలువ = ప్రారంభం;
	rec.additional_లెక్క = చివర - ప్రారంభం;
	c->copy<Unicode_విలువ_Range> (rec);
      }

    }
    లేకపోతే
    {
      hb_codepoint_t చివరి_సంకేతం = HB_సమితి_విలువ_INVALID;
      పూర్ణాంకం లెక్క = -1;

      కొరకు (స్థిరరాశి Unicode_విలువ_Range& _ : *ఇది)
      {
	hb_codepoint_t cur_నమోదు = (hb_codepoint_t) (_.ప్రారంభం_Unicode_విలువ - 1);
	hb_codepoint_t చివర = cur_నమోదు + _.additional_లెక్క + 2;

	కొరకు (; unicodes->next (&cur_నమోదు) && cur_నమోదు < చివర;)
	{
	  లెక్క += 1;
	  ఒకవేళ (చివరి_సంకేతం == HB_సమితి_విలువ_INVALID)
	    చివరి_సంకేతం = cur_నమోదు;
	  లేకపోతే ఒకవేళ (చివరి_సంకేతం + లెక్క != cur_నమోదు || లెక్క > 255)
	  {
	    Unicode_విలువ_Range rec;
	    rec.ప్రారంభం_Unicode_విలువ = చివరి_సంకేతం;
	    rec.additional_లెక్క = లెక్క - 1;
	    c->copy<Unicode_విలువ_Range> (rec);

	    చివరి_సంకేతం = cur_నమోదు;
	    లెక్క = 0;
	  }
	}
      }

      ఒకవేళ (చివరి_సంకేతం != HB_MAP_విలువ_INVALID)
      {
	Unicode_విలువ_Range rec;
	rec.ప్రారంభం_Unicode_విలువ = చివరి_సంకేతం;
	rec.additional_లెక్క = లెక్క;
	c->copy<Unicode_విలువ_Range> (rec);
      }
    }

    ఒకవేళ (c->నిడివి () - init_len == 0)
    {
      c->revert (snap);
      తిరిగి శూన్యసూచిక;
    }
    లేకపోతే
    {
      ఒకవేళ (unlikely (!c->check_కేటాయించు (బహిర్గతం->len,
                                      (c->నిడివి () - init_len) / Unicode_విలువ_Range::static_కొలత,
                                      HB_SERIALIZE_ERROR_INT_అధికప్రవాహం))) తిరిగి శూన్యసూచిక;
      తిరిగి బహిర్గతం;
    }
  }

  బహిరంగ:
  DEFINE_కొలత_ARRAY (4, *ఇది);
};

మూస <రకం_పేరు Types>
నిర్మాణం UVSMapping
{
  ఉపయోగించి GlyphID = రకం_పేరు Types::HBGlyphID;

  పూర్ణాంకం cmp (స్థిరరాశి hb_codepoint_t &codepoint) స్థిరరాశి
  { తిరిగి unicode_విలువ.cmp (codepoint); }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది));
  }

  HBUINT24	unicode_విలువ;	/* Base Unicode value of the UVS */
  GlyphID
		glyphID;	/* Glyph ID of the UVS. */
  బహిరంగ:
  DEFINE_కొలత_STATIC (3 + GlyphID::static_కొలత);
};

మూస <రకం_పేరు Types>
నిర్మాణం NonDefaultUVS : Sorted_Array32_యొక్క<UVSMapping<Types>>
{
  ఉపయోగించి Mapping = UVSMapping<Types>;

  శూన్య collect_unicodes (hb_సమితి_t *బహిర్గతం) స్థిరరాశి
  {
    కొరకు (స్థిరరాశి స్వయంచాలక& a : ఇది->as_array ())
      బహిర్గతం->కూడు (a.unicode_విలువ);
  }

  శూన్య collect_mapping (hb_సమితి_t *unicodes, /*బహిర్గతం*/
			hb_map_t *mapping /*బహిర్గతం*/) స్థిరరాశి
  {
    కొరకు (స్థిరరాశి స్వయంచాలక& a : ఇది->as_array ())
    {
      hb_codepoint_t unicode = a.unicode_విలువ;
      hb_codepoint_t glyphid = a.glyphID;
      unicodes->కూడు (unicode);
      mapping->సమితి (unicode, glyphid);
    }
  }

  శూన్య సంవృతం_glyphs (స్థిరరాశి hb_సమితి_t      *unicodes,
		       hb_సమితి_t            *glyphset) స్థిరరాశి
  {
    + ఇది->as_array ()
    | hb_filter (unicodes, &Mapping::unicode_విలువ)
    | hb_map (&Mapping::glyphID)
    | hb_sink (glyphset)
    ;
  }

  NonDefaultUVS* copy (hb_serialize_context_t *c,
		       స్థిరరాశి hb_సమితి_t *unicodes,
		       స్థిరరాశి hb_సమితి_t *glyphs_requested,
		       స్థిరరాశి hb_map_t *glyph_map) స్థిరరాశి
  {
    స్వయంచాలక *బహిర్గతం = c->ప్రారంభం_embed<NonDefaultUVS> ();
    స్వయంచాలక it =
    + ఇది->as_array ()
    | hb_filter ([&] (స్థిరరాశి Mapping& _)
		 {
		   తిరిగి unicodes->has (_.unicode_విలువ) || glyphs_requested->has (_.glyphID);
		 })
    ;

    ఒకవేళ (!it) తిరిగి శూన్యసూచిక;

    HBUINT32 len;
    len = it.len ();
    ఒకవేళ (unlikely (!c->copy<HBUINT32> (len))) తిరిగి శూన్యసూచిక;

    కొరకు (స్థిరరాశి Mapping& _ : it)
    {
      Mapping mapping;
      mapping.unicode_విలువ = _.unicode_విలువ;
      mapping.glyphID = glyph_map->get (_.glyphID);
      c->copy<Mapping> (mapping);
    }

    తిరిగి బహిర్గతం;
  }

  బహిరంగ:
  DEFINE_కొలత_ARRAY (4, *ఇది);
};

మూస <రకం_పేరు Types>
నిర్మాణం Variation_Selector_రికార్డు
{
  glyph_variant_t get_glyph (hb_codepoint_t codepoint,
			     hb_codepoint_t *glyph,
			     స్థిరరాశి శూన్య *పునాది) స్థిరరాశి
  {
    ఒకవేళ ((పునాది+defaultUVS).bfind (codepoint))
      తిరిగి GLYPH_VARIANT_USE_DEFAULT;
    స్థిరరాశి UVSMapping<Types> &nonDefault = (పునాది+nonDefaultUVS).bsearch (codepoint);
    ఒకవేళ (nonDefault.glyphID)
    {
      *glyph = nonDefault.glyphID;
       తిరిగి GLYPH_VARIANT_దొరికింది;
    }
    తిరిగి GLYPH_VARIANT_NOT_దొరికింది;
  }

  Variation_Selector_రికార్డు(స్థిరరాశి Variation_Selector_రికార్డు& other)
  {
    *ఇది = other;
  }

  శూన్య క్రియాకారకం= (స్థిరరాశి Variation_Selector_రికార్డు& other)
  {
    varSelector = other.varSelector;
    HBUINT32 స్థానభ్రంశం = other.defaultUVS;
    defaultUVS = స్థానభ్రంశం;
    స్థానభ్రంశం = other.nonDefaultUVS;
    nonDefaultUVS = స్థానభ్రంశం;
  }

  శూన్య collect_unicodes (hb_సమితి_t *బహిర్గతం, స్థిరరాశి శూన్య *పునాది) స్థిరరాశి
  {
    (పునాది+defaultUVS).collect_unicodes (బహిర్గతం);
    (పునాది+nonDefaultUVS).collect_unicodes (బహిర్గతం);
  }

  శూన్య collect_mapping (స్థిరరాశి శూన్య *పునాది,
			hb_సమితి_t *unicodes, /*బహిర్గతం*/
			hb_map_t *mapping /*బహిర్గతం*/) స్థిరరాశి
  {
    (పునాది+defaultUVS).collect_unicodes (unicodes);
    (పునాది+nonDefaultUVS).collect_mapping (unicodes, mapping);
  }

  పూర్ణాంకం cmp (స్థిరరాశి hb_codepoint_t &variation_selector) స్థిరరాశి
  { తిరిగి varSelector.cmp (variation_selector); }

  తార్కిక sanitize (hb_sanitize_context_t *c, స్థిరరాశి శూన్య *పునాది) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) &&
		  defaultUVS.sanitize (c, పునాది) &&
		  nonDefaultUVS.sanitize (c, పునాది));
  }

  hb_pair_t<గుర్తులేని, గుర్తులేని>
  copy (hb_serialize_context_t *c,
	స్థిరరాశి hb_సమితి_t *unicodes,
	స్థిరరాశి hb_సమితి_t *glyphs_requested,
	స్థిరరాశి hb_map_t *glyph_map,
	స్థిరరాశి శూన్య *పునాది) స్థిరరాశి
  {
    స్వయంచాలక snap = c->snapshot ();
    స్వయంచాలక *బహిర్గతం = c->embed<Variation_Selector_రికార్డు> (*ఇది);
    ఒకవేళ (unlikely (!బహిర్గతం)) తిరిగి hb_pair (0, 0);

    బహిర్గతం->defaultUVS = 0;
    బహిర్గతం->nonDefaultUVS = 0;

    గుర్తులేని non_default_uvs_objidx = 0;
    ఒకవేళ (nonDefaultUVS != 0)
    {
      c->push ();
      ఒకవేళ (c->copy (పునాది+nonDefaultUVS, unicodes, glyphs_requested, glyph_map))
	non_default_uvs_objidx = c->తీయి_pack ();
      లేకపోతే c->తీయి_discard ();
    }

    గుర్తులేని default_uvs_objidx = 0;
    ఒకవేళ (defaultUVS != 0)
    {
      c->push ();
      ఒకవేళ (c->copy (పునాది+defaultUVS, unicodes))
	default_uvs_objidx = c->తీయి_pack ();
      లేకపోతే c->తీయి_discard ();
    }


    ఒకవేళ (!default_uvs_objidx && !non_default_uvs_objidx)
      c->revert (snap);

    తిరిగి hb_pair (default_uvs_objidx, non_default_uvs_objidx);
  }

  HBUINT24	varSelector;	/* Variation selector. */
  Offset32_గమ్యం<DefaultUVS>
		defaultUVS;	/* Offset to Default UVS Table.  May be 0. */
  Offset32_గమ్యం<NonDefaultUVS<Types>>
		nonDefaultUVS;	/* Offset to Non-Default UVS Table.  May be 0. */
  బహిరంగ:
  DEFINE_కొలత_STATIC (11);
};

మూస <రకం_పేరు Types>
నిర్మాణం CmapSubtableFormat14_15
{
  స్థిర constexpr గుర్తులేని format_విలువ = Types::కొలత == 2 ? 14 : 15;
  ఉపయోగించి రికార్డు_2 = Variation_Selector_రికార్డు<Types>;
  ఉపయోగించి NonDefault = NonDefaultUVS<Types>;

  glyph_variant_t get_glyph_variant (hb_codepoint_t codepoint,
				     hb_codepoint_t variation_selector,
				     hb_codepoint_t *glyph) స్థిరరాశి
  { తిరిగి రికార్డు.bsearch (variation_selector).get_glyph (codepoint, glyph, ఇది); }

  శూన్య collect_variation_selectors (hb_సమితి_t *బహిర్గతం) స్థిరరాశి
  {
    కొరకు (స్థిరరాశి స్వయంచాలక& a : రికార్డు.as_array ())
      బహిర్గతం->కూడు (a.varSelector);
  }
  శూన్య collect_variation_unicodes (hb_codepoint_t variation_selector,
				   hb_సమితి_t *బహిర్గతం) స్థిరరాశి
  { రికార్డు.bsearch (variation_selector).collect_unicodes (బహిర్గతం, ఇది); }

  శూన్య serialize (hb_serialize_context_t *c,
		  స్థిరరాశి hb_సమితి_t *unicodes,
		  స్థిరరాశి hb_సమితి_t *glyphs_requested,
		  స్థిరరాశి hb_map_t *glyph_map,
		  స్థిరరాశి శూన్య *పునాది)
  {
    స్వయంచాలక snap = c->snapshot ();
    గుర్తులేని పట్టిక_initpos = c->నిడివి ();
    స్థిరరాశి అక్షరం* init_తోక = c->తోక;

    ఒకవేళ (unlikely (!c->extend_min (ఇది))) తిరిగి;
    ఇది->format = format_విలువ;

    స్వయంచాలక src_tbl = reinterpret_cast<స్థిరరాశి CmapSubtableFormat14_15*> (పునాది);

    /*
     * Some versions of OTS require that offsets are in order. Due to the use
     * of push()/pop_pack() serializing the variation records in order results
     * in the offsets being in reverse order (first record has the largest
     * offset). While this is perfectly valid, it will cause some versions of
     * OTS to consider this table bad.
     *
     * So to prevent this issue we serialize the variation records in reverse
     * order, so that the offsets are ordered from small to large. Since
     * variation records are supposed to be in increasing order of varSelector
     * we then have to reverse the order of the written variation selector
     * records after everything is finalized.
     */
    hb_సదిశ_t<hb_pair_t<గుర్తులేని, గుర్తులేని>> obj_indices;
    కొరకు (పూర్ణాంకం i = src_tbl->రికార్డు.len - 1; i >= 0; i--)
    {
      ఒకవేళ (!unicodes->has(src_tbl->రికార్డు[i].varSelector))
        కొనసాగు;

      hb_pair_t<గుర్తులేని, గుర్తులేని> ఫలితం = src_tbl->రికార్డు[i].copy (c, unicodes, glyphs_requested, glyph_map, పునాది);
      ఒకవేళ (ఫలితం.మొదటి || ఫలితం.second)
	obj_indices.push (ఫలితం);
    }

    ఒకవేళ (c->నిడివి () - పట్టిక_initpos == CmapSubtableFormat14_15::min_కొలత)
    {
      c->revert (snap);
      తిరిగి;
    }

    ఒకవేళ (unlikely (!c->check_success (!obj_indices.లోపల_error ())))
      తిరిగి;

    పూర్ణాంకం తోక_len = init_తోక - c->తోక;
    c->check_కేటాయించు (ఇది->నిడివి, c->నిడివి () - పట్టిక_initpos + తోక_len,
                     HB_SERIALIZE_ERROR_INT_అధికప్రవాహం);
    c->check_కేటాయించు (ఇది->రికార్డు.len,
		     (c->నిడివి () - పట్టిక_initpos - CmapSubtableFormat14_15::min_కొలత) /
		     రికార్డు_2::static_కొలత,
                     HB_SERIALIZE_ERROR_INT_అధికప్రవాహం);

    /* Correct the incorrect write order by reversing the order of the variation
       records array. */
    తిరగేయి_variation_రికార్డులు ();

    /* Now that records are in the right order, we can set up the offsets. */
    కూడు_links_గమ్యం_variation_రికార్డులు (c, obj_indices);
  }

  శూన్య తిరగేయి_variation_రికార్డులు ()
  {
    రికార్డు.as_array ().reverse ();
  }

  శూన్య కూడు_links_గమ్యం_variation_రికార్డులు (hb_serialize_context_t *c,
					స్థిరరాశి hb_సదిశ_t<hb_pair_t<గుర్తులేని, గుర్తులేని>>& obj_indices)
  {
    కొరకు (గుర్తులేని i = 0; i < obj_indices.నిడివి; i++)
    {
      /*
       * Since the record array has been reversed (see comments in copy())
       * but obj_indices has not been, the indices at obj_indices[i]
       * are for the variation record at record[j].
       */
      పూర్ణాంకం j = obj_indices.నిడివి - 1 - i;
      c->కూడు_link (రికార్డు[j].defaultUVS, obj_indices[i].మొదటి);
      c->కూడు_link (రికార్డు[j].nonDefaultUVS, obj_indices[i].second);
    }
  }

  శూన్య సంవృతం_glyphs (స్థిరరాశి hb_సమితి_t      *unicodes,
		       hb_సమితి_t            *glyphset) స్థిరరాశి
  {
    + hb_iter (రికార్డు)
    | hb_filter (hb_bool, &రికార్డు_2::nonDefaultUVS)
    | hb_filter (unicodes, &రికార్డు_2::varSelector)
    | hb_map (&రికార్డు_2::nonDefaultUVS)
    | hb_map (hb_కూడు (ఇది))
    | hb_apply ([=] (స్థిరరాశి NonDefault& _) { _.సంవృతం_glyphs (unicodes, glyphset); })
    ;
  }

  శూన్య collect_unicodes (hb_సమితి_t *బహిర్గతం) స్థిరరాశి
  {
    కొరకు (స్థిరరాశి రికార్డు_2& _ : రికార్డు)
      _.collect_unicodes (బహిర్గతం, ఇది);
  }

  శూన్య collect_mapping (hb_సమితి_t *unicodes, /*బహిర్గతం*/
			hb_map_t *mapping /*బహిర్గతం*/) స్థిరరాశి
  {
    కొరకు (స్థిరరాశి రికార్డు_2& _ : రికార్డు)
      _.collect_mapping (ఇది, unicodes, mapping);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) &&
		  రికార్డు.sanitize (c, ఇది));
  }

  రక్షిత:
  HBUINT16	format;		/* Format number is set to 14 or 15. */
  HBUINT32	నిడివి;		/* Byte length of this subtable. */
  Sorted_Array32_యొక్క<రికార్డు_2>
		రికార్డు;		/* Variation selector records; sorted
				 * in increasing order of `varSelector'. */
  బహిరంగ:
  DEFINE_కొలత_ARRAY (10, రికార్డు);
};

నిర్మాణం CmapSubtable
{
  /* Note: We intentionally do NOT implement subtable formats 2 and 8. */

  తార్కిక is_variation_selector () స్థిరరాశి
  {
    ఎంపిక (u.format.v) {
    స్థితి 14:
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    స్థితి 15:
#అయితే_ముగింపు
      తిరిగి నిజం;
    అప్రమేయ:
      తిరిగి అబద్ధం;
    }
  }

  glyph_variant_t get_glyph_variant (hb_codepoint_t codepoint,
				     hb_codepoint_t variation_selector,
				     hb_codepoint_t *glyph) స్థిరరాశి
  {
    ఎంపిక (u.format.v) {
    స్థితి 14: hb_barrier (); తిరిగి u.format14.get_glyph_variant (codepoint, variation_selector, glyph);
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    స్థితి 15: hb_barrier (); తిరిగి u.format15.get_glyph_variant (codepoint, variation_selector, glyph);
#అయితే_ముగింపు
    అప్రమేయ: తిరిగి GLYPH_VARIANT_NOT_దొరికింది;
    }
  }

  శూన్య collect_variation_selectors (hb_సమితి_t *బహిర్గతం) స్థిరరాశి
  {
    ఎంపిక (u.format.v) {
    స్థితి 14: hb_barrier (); u.format14.collect_variation_selectors (బహిర్గతం); తిరిగి;
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    స్థితి 15: hb_barrier (); u.format15.collect_variation_selectors (బహిర్గతం); తిరిగి;
#అయితే_ముగింపు
    అప్రమేయ: తిరిగి;
    }
  }

  శూన్య collect_variation_unicodes (hb_codepoint_t variation_selector,
				    hb_సమితి_t *బహిర్గతం) స్థిరరాశి
  {
    ఎంపిక (u.format.v) {
    స్థితి 14: hb_barrier (); u.format14.collect_variation_unicodes (variation_selector, బహిర్గతం); తిరిగి;
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    స్థితి 15: hb_barrier (); u.format15.collect_variation_unicodes (variation_selector, బహిర్గతం); తిరిగి;
#అయితే_ముగింపు
    అప్రమేయ: తిరిగి;
    }
  }

  శూన్య సంవృతం_glyphs (స్థిరరాశి hb_సమితి_t *unicodes, hb_సమితి_t *glyphset) స్థిరరాశి
  {
    ఎంపిక (u.format.v) {
    స్థితి 14: hb_barrier (); u.format14.సంవృతం_glyphs (unicodes, glyphset); తిరిగి;
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    స్థితి 15: hb_barrier (); u.format15.సంవృతం_glyphs (unicodes, glyphset); తిరిగి;
#అయితే_ముగింపు
    అప్రమేయ: తిరిగి;
    }
  }

  తార్కిక get_glyph (hb_codepoint_t codepoint,
		  hb_codepoint_t *glyph,
		  గుర్తులేని పూర్ణాంకం subtable_దత్తాంశం_కొలత = 0) స్థిరరాశి
  {
    ఎంపిక (u.format.v) {
    స్థితి  0: hb_barrier (); తిరిగి u.format0 .get_glyph (codepoint, glyph);
    స్థితి  4: hb_barrier (); తిరిగి subtable_దత్తాంశం_కొలత
				      ? u.format4.get_glyph (codepoint, glyph, subtable_దత్తాంశం_కొలత)
				      : అబద్ధం;
    స్థితి  6: hb_barrier (); తిరిగి u.format6 .get_glyph (codepoint, glyph);
    స్థితి 10: hb_barrier (); తిరిగి u.format10.get_glyph (codepoint, glyph);
    స్థితి 12: hb_barrier (); తిరిగి u.format12.get_glyph (codepoint, glyph);
    స్థితి 13: hb_barrier (); తిరిగి u.format13.get_glyph (codepoint, glyph);
    స్థితి 14:
    అప్రమేయ: తిరిగి అబద్ధం;
    }
  }
  శూన్య collect_unicodes (hb_సమితి_t *బహిర్గతం,
			 గుర్తులేని పూర్ణాంకం అంకె_glyphs = UINT_MAX,
			 గుర్తులేని పూర్ణాంకం subtable_దత్తాంశం_కొలత = 0) స్థిరరాశి
  {
    ఎంపిక (u.format.v) {
    స్థితి  0: hb_barrier (); u.format0 .collect_unicodes (బహిర్గతం); తిరిగి;
    స్థితి  4: hb_barrier (); ఒకవేళ (subtable_దత్తాంశం_కొలత)
			      u.format4.collect_unicodes (బహిర్గతం, subtable_దత్తాంశం_కొలత);
			    లేకపోతే
			      u.format4.collect_unicodes (బహిర్గతం);
			    తిరిగి;
    స్థితి  6: hb_barrier (); u.format6 .collect_unicodes (బహిర్గతం); తిరిగి;
    స్థితి 10: hb_barrier (); u.format10.collect_unicodes (బహిర్గతం); తిరిగి;
    స్థితి 12: hb_barrier (); u.format12.collect_unicodes (బహిర్గతం, అంకె_glyphs); తిరిగి;
    స్థితి 13: hb_barrier (); u.format13.collect_unicodes (బహిర్గతం, అంకె_glyphs); తిరిగి;
    స్థితి 14:
    అప్రమేయ: తిరిగి;
    }
  }

  శూన్య collect_mapping (hb_సమితి_t *unicodes, /*బహిర్గతం*/
			hb_map_t *mapping, /*బహిర్గతం*/
			గుర్తులేని అంకె_glyphs = UINT_MAX,
			గుర్తులేని పూర్ణాంకం subtable_దత్తాంశం_కొలత = 0) స్థిరరాశి
  {
    ఎంపిక (u.format.v) {
    స్థితి  0: hb_barrier (); u.format0 .collect_mapping (unicodes, mapping); తిరిగి;
    స్థితి  4: hb_barrier (); ఒకవేళ (subtable_దత్తాంశం_కొలత)
			      u.format4.collect_mapping (unicodes, mapping, subtable_దత్తాంశం_కొలత);
			    లేకపోతే
			      u.format4.collect_mapping (unicodes, mapping);
			    తిరిగి;
    స్థితి  6: hb_barrier (); u.format6 .collect_mapping (unicodes, mapping); తిరిగి;
    స్థితి 10: hb_barrier (); u.format10.collect_mapping (unicodes, mapping); తిరిగి;
    స్థితి 12: hb_barrier (); u.format12.collect_mapping (unicodes, mapping, అంకె_glyphs); తిరిగి;
    స్థితి 13: hb_barrier (); u.format13.collect_mapping (unicodes, mapping, అంకె_glyphs); తిరిగి;
    స్థితి 14:
    అప్రమేయ: తిరిగి;
    }
  }

  గుర్తులేని get_language () స్థిరరాశి
  {
    ఎంపిక (u.format.v) {
    స్థితి  0: hb_barrier (); తిరిగి u.format0 .get_language ();
    స్థితి  4: hb_barrier (); తిరిగి u.format4 .get_language ();
    స్థితి  6: hb_barrier (); తిరిగి u.format6 .get_language ();
    స్థితి 10: hb_barrier (); తిరిగి u.format10.get_language ();
    స్థితి 12: hb_barrier (); తిరిగి u.format12.get_language ();
    స్థితి 13: hb_barrier (); తిరిగి u.format13.get_language ();
    స్థితి 14:
    అప్రమేయ: తిరిగి 0;
    }
  }

  మూస<రకం_పేరు Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  శూన్య serialize (hb_serialize_context_t *c,
		  Iterator it,
		  గుర్తులేని format,
		  స్థిరరాశి hb_subset_plan_t *plan,
		  స్థిరరాశి శూన్య *పునాది)
  {
    ఎంపిక (format) {
    స్థితి  4: hb_barrier (); తిరిగి u.format4.serialize (c, it);
    స్థితి 12: hb_barrier (); తిరిగి u.format12.serialize (c, it);
    స్థితి 14: hb_barrier (); తిరిగి u.format14.serialize (c, &plan->unicodes, &plan->glyphs_requested, plan->glyph_map, పునాది);
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    స్థితి 15: hb_barrier (); తిరిగి u.format15.serialize (c, &plan->unicodes, &plan->glyphs_requested, plan->glyph_map, పునాది);
#అయితే_ముగింపు
    అప్రమేయ: తిరిగి;
    }
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    ఒకవేళ (!u.format.v.sanitize (c)) return_trace (అబద్ధం);
    hb_barrier ();
    ఎంపిక (u.format.v) {
    స్థితి  0: hb_barrier (); return_trace (u.format0 .sanitize (c));
    స్థితి  4: hb_barrier (); return_trace (u.format4 .sanitize (c));
    స్థితి  6: hb_barrier (); return_trace (u.format6 .sanitize (c));
    స్థితి 10: hb_barrier (); return_trace (u.format10.sanitize (c));
    స్థితి 12: hb_barrier (); return_trace (u.format12.sanitize (c));
    స్థితి 13: hb_barrier (); return_trace (u.format13.sanitize (c));
    స్థితి 14: hb_barrier (); return_trace (u.format14.sanitize (c));
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    స్థితి 15: hb_barrier (); return_trace (u.format15.sanitize (c));
#అయితే_ముగింపు
    అప్రమేయ:return_trace (నిజం);
    }
  }

  బహిరంగ:
  సంఘం {
  నిర్మాణం { HBUINT16 v; }	format;		/* Format identifier */
  CmapSubtableFormat0	format0;
  CmapSubtableFormat4	format4;
  CmapSubtableFormat6	format6;
  CmapSubtableFormat10	format10;
  CmapSubtableFormat12	format12;
  CmapSubtableFormat13	format13;
  CmapSubtableFormat14_15<Layout::SmallTypes>
			format14;
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
  CmapSubtableFormat14_15<Layout::MediumTypes>
			format15;
#అయితే_ముగింపు
  } u;
  బహిరంగ:
  DEFINE_కొలత_UNION (2, format.v);
};


నిర్మాణం Encoding_రికార్డు
{
  పూర్ణాంకం cmp (స్థిరరాశి Encoding_రికార్డు &other) స్థిరరాశి
  {
    పూర్ణాంకం ret;
    ret = platformID.cmp (other.platformID);
    ఒకవేళ (ret) తిరిగి ret;
    ఒకవేళ (other.encodingID != 0xFFFF)
    {
      ret = encodingID.cmp (other.encodingID);
      ఒకవేళ (ret) తిరిగి ret;
    }
    తిరిగి 0;
  }

  తార్కిక sanitize (hb_sanitize_context_t *c, స్థిరరాశి శూన్య *పునాది) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) &&
		  subtable.sanitize (c, పునాది));
  }

  మూస<రకం_పేరు Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  Encoding_రికార్డు* copy (hb_serialize_context_t *c,
			Iterator it,
			గుర్తులేని format,
			స్థిరరాశి శూన్య *పునాది,
			స్థిరరాశి hb_subset_plan_t *plan,
			/* INOUT */ గుర్తులేని *objidx) స్థిరరాశి
  {
    TRACE_SERIALIZE (ఇది);
    స్వయంచాలక snap = c->snapshot ();
    స్వయంచాలక *బహిర్గతం = c->embed (ఇది);
    ఒకవేళ (unlikely (!బహిర్గతం)) return_trace (శూన్యసూచిక);
    బహిర్గతం->subtable = 0;

    ఒకవేళ (*objidx == 0)
    {
      CmapSubtable *cmapsubtable = c->push<CmapSubtable> ();
      గుర్తులేని origin_నిడివి = c->నిడివి ();
      cmapsubtable->serialize (c, it, format, plan, &(పునాది+subtable));
      ఒకవేళ (c->నిడివి () > origin_నిడివి && !c->లోపల_error()) *objidx = c->తీయి_pack ();
      లేకపోతే c->తీయి_discard ();
    }

    ఒకవేళ (*objidx == 0)
    {
      c->revert (snap);
      return_trace (శూన్యసూచిక);
    }

    c->కూడు_link (బహిర్గతం->subtable, *objidx);
    return_trace (బహిర్గతం);
  }

  HBUINT16	platformID;	/* Platform ID. */
  HBUINT16	encodingID;	/* Platform-specific encoding ID. */
  Offset32_గమ్యం<CmapSubtable>
		subtable;	/* Byte offset from beginning of table to the subtable for this encoding. */
  బహిరంగ:
  DEFINE_కొలత_STATIC (8);
};

నిర్మాణం cmap;

నిర్మాణం SubtableUnicodesCache {

 అంతరంగ:
  hb_blob_ptr_t<cmap> పునాది_blob;
  స్థిరరాశి అక్షరం* పునాది;
  గుర్తులేని పూర్ణాంకం పట్టిక_నిడివి;
  hb_hashmap_t<గుర్తులేని, hb::unique_ptr<hb_సమితి_t>> cached_unicodes;

 బహిరంగ:

  స్థిర SubtableUnicodesCache* create (hb_blob_ptr_t<cmap> ఆధారం_పట్టిక)
  {
    SubtableUnicodesCache* cache =
        (SubtableUnicodesCache*) hb_malloc (పరిమాణం(SubtableUnicodesCache));

    ఒకవేళ (unlikely (!cache))
      తిరిగి శూన్యసూచిక;

    కొత్త (cache) SubtableUnicodesCache (ఆధారం_పట్టిక);
    తిరిగి cache;
  }

  స్థిర శూన్య destroy (శూన్య* value) {
    ఒకవేళ (!value) తిరిగి;

    SubtableUnicodesCache* cache = (SubtableUnicodesCache*) value;
    cache->~SubtableUnicodesCache ();
    hb_free (cache);
  }

  SubtableUnicodesCache(స్థిరరాశి శూన్య* cmap_పునాది,
			గుర్తులేని పూర్ణాంకం పట్టిక_నిడివి_2 = 0)
      : పునాది_blob(),
        పునాది ((స్థిరరాశి అక్షరం*) cmap_పునాది),
        పట్టిక_నిడివి (పట్టిక_నిడివి_2),
        cached_unicodes ()
  {}

  SubtableUnicodesCache(hb_blob_ptr_t<cmap> పునాది_blob_2)
      : పునాది_blob(పునాది_blob_2),
        పునాది ((స్థిరరాశి అక్షరం *) పునాది_blob.get()),
        పట్టిక_నిడివి (పునాది_blob.get_నిడివి ()),
        cached_unicodes ()
  {}

  ~SubtableUnicodesCache()
  {
    పునాది_blob.destroy ();
  }

  తార్కిక same_పునాది(స్థిరరాశి శూన్య* other) స్థిరరాశి
  {
    తిరిగి other == (స్థిరరాశి శూన్య*) పునాది;
  }

  స్థిరరాశి hb_సమితి_t* సమితి_for (స్థిరరాశి Encoding_రికార్డు* రికార్డు,
                           SubtableUnicodesCache& mutable_cache) స్థిరరాశి
  {
    ఒకవేళ (cached_unicodes.has ((గుర్తులేని) ((స్థిరరాశి అక్షరం *) రికార్డు - పునాది)))
      తిరిగి cached_unicodes.get ((గుర్తులేని) ((స్థిరరాశి అక్షరం *) రికార్డు - పునాది));

    తిరిగి mutable_cache.సమితి_for (రికార్డు);
  }

  స్థిరరాశి hb_సమితి_t* సమితి_for (స్థిరరాశి Encoding_రికార్డు* రికార్డు)
  {
    ఒకవేళ (!cached_unicodes.has ((గుర్తులేని) ((స్థిరరాశి అక్షరం *) రికార్డు - పునాది)))
    {
      hb_సమితి_t *s = hb_సమితి_create ();
      ఒకవేళ (unlikely (s->లోపల_error ()))
	తిరిగి hb_సమితి_get_ఖాళీలు ();

      గుర్తులేని పూర్ణాంకం subtable_దత్తాంశం_కొలత = రికార్డు->subtable < పట్టిక_నిడివి
					? పట్టిక_నిడివి - (గుర్తులేని పూర్ణాంకం) రికార్డు->subtable
					: 0;
      (పునాది+రికార్డు->subtable).collect_unicodes (s, UINT_MAX, subtable_దత్తాంశం_కొలత);

      ఒకవేళ (unlikely (!cached_unicodes.సమితి ((గుర్తులేని) ((స్థిరరాశి అక్షరం *) రికార్డు - పునాది), hb::unique_ptr<hb_సమితి_t> {s})))
        తిరిగి hb_సమితి_get_ఖాళీలు ();

      తిరిగి s;
    }
    తిరిగి cached_unicodes.get ((గుర్తులేని) ((స్థిరరాశి అక్షరం *) రికార్డు - పునాది));
  }

};

స్థిర అంతఃవరుస uint16_t
hb_చిహ్నం_pua_map (గుర్తులేని codepoint)
{
  ఒకవేళ (codepoint <= 0x00FFu)
  {
    /* For symbol-encoded OpenType fonts, we duplicate the
     * U+F000..F0FF range at U+0000..U+00FF.  That's what
     * Windows seems to do, and that's hinted about at:
     * https://docs.microsoft.com/en-us/typography/opentype/spec/recom
     * under "Non-Standard (Symbol) Fonts". */
    తిరిగి 0xF000u + codepoint;
  }
  తిరిగి 0;
}

నిర్మాణం cmap
{
  స్థిర constexpr hb_ట్యాగ్_t పట్టిక_ట్యాగ్ = HB_OT_ట్యాగ్_cmap;


  స్థిర SubtableUnicodesCache* create_filled_cache(hb_blob_ptr_t<cmap> ఆధారం_పట్టిక) {
    స్థిరరాశి cmap* cmap = ఆధారం_పట్టిక.get();
    స్వయంచాలక it =
    + hb_iter (cmap->encoding_రికార్డు)
    | hb_filter ([&](స్థిరరాశి Encoding_రికార్డు& _) {
      తిరిగి cmap::filter_encoding_రికార్డులు_for_subset (cmap, _);
    })
    ;

    SubtableUnicodesCache* cache = SubtableUnicodesCache::create(ఆధారం_పట్టిక);

    ఒకవేళ (unlikely (!cache))
      తిరిగి శూన్యసూచిక;

    కొరకు (స్థిరరాశి Encoding_రికార్డు& _ : it)
      cache->సమితి_for(&_); // populate the cache for this encoding record.

    తిరిగి cache;
  }

  మూస<రకం_పేరు Iterator, రకం_పేరు EncodingRecIter,
	   hb_requires (hb_is_iterator (EncodingRecIter))>
  తార్కిక serialize (hb_serialize_context_t *c,
		  Iterator it,
		  EncodingRecIter encodingrec_iter,
		  స్థిరరాశి శూన్య *పునాది,
		  hb_subset_plan_t *plan,
	                  తార్కిక drop_format_4 = అబద్ధం,
			  గుర్తులేని పూర్ణాంకం ఆధారం_పట్టిక_నిడివి = 0)
  {
    ఒకవేళ (unlikely (!c->extend_min ((*ఇది))))  తిరిగి అబద్ధం;
    ఇది->సంస్కరణ = 0;

    గుర్తులేని format4objidx = 0, format12objidx = 0, format14objidx = 0;
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    గుర్తులేని format15objidx = 0;
#అయితే_ముగింపు
    స్వయంచాలక snap = c->snapshot ();

    SubtableUnicodesCache local_unicodes_cache (పునాది, ఆధారం_పట్టిక_నిడివి);
    స్థిరరాశి SubtableUnicodesCache* unicodes_cache = &local_unicodes_cache;

    ఒకవేళ (plan->accelerator &&
        plan->accelerator->cmap_cache &&
        plan->accelerator->cmap_cache->same_పునాది (పునాది))
      unicodes_cache = plan->accelerator->cmap_cache;

    కొరకు (స్థిరరాశి Encoding_రికార్డు& _ : encodingrec_iter)
    {
      ఒకవేళ (c->లోపల_error ())
        తిరిగి అబద్ధం;

      గుర్తులేని format = (పునాది+_.subtable).u.format.v;
      ఒకవేళ (format != 4 && format != 12 && format != 14
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
	  && format != 15
#అయితే_ముగింపు
	 ) కొనసాగు;

      స్థిరరాశి hb_సమితి_t* unicodes_సమితి = unicodes_cache->సమితి_for (&_, local_unicodes_cache);

      ఒకవేళ (!drop_format_4 && format == 4)
      {
        c->copy (_, + it | hb_filter (*unicodes_సమితి, hb_మొదటి), 4u, పునాది, plan, &format4objidx);
        ఒకవేళ (c->లోపల_error () && c->only_అధికప్రవాహం ())
        {
          // cmap4 overflowed, reset and retry serialization without format 4 subtables.
          c->revert (snap);
          తిరిగి serialize (c, it,
                            encodingrec_iter,
                            పునాది,
                            plan,
                            నిజం,
			    ఆధారం_పట్టిక_నిడివి);
        }
      }

      లేకపోతే ఒకవేళ (format == 12)
      {
        ఒకవేళ (_can_drop (_,
                       *unicodes_సమితి,
                       పునాది,
                       *unicodes_cache,
                       local_unicodes_cache,
                       + it | hb_map (hb_మొదటి), encodingrec_iter))
          కొనసాగు;
        c->copy (_, + it | hb_filter (*unicodes_సమితి, hb_మొదటి), 12u, పునాది, plan, &format12objidx);
      }
      లేకపోతే ఒకవేళ (format == 14) c->copy (_, it, 14u, పునాది, plan, &format14objidx);
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
      లేకపోతే ఒకవేళ (format == 15) c->copy (_, it, 15u, పునాది, plan, &format15objidx);
#అయితే_ముగింపు
    }
        గుర్తులేని నిడివి = c->నిడివి ();
        గుర్తులేని అందుబాటు = నిడివి > cmap::min_కొలత ? నిడివి - cmap::min_కొలత : 0;
        c->check_కేటాయించు(ఇది->encoding_రికార్డు.len,
                        అందుబాటు / Encoding_రికార్డు::static_కొలత,
                        HB_SERIALIZE_ERROR_INT_అధికప్రవాహం);

    // Fail if format 4 was dropped and there is no cmap12.
    తిరిగి !drop_format_4 || format12objidx;
  }

  మూస<రకం_పేరు Iterator, రకం_పేరు Encoding_రికార్డు_Iterator,
      hb_requires (hb_is_iterator (Iterator)),
      hb_requires (hb_is_iterator (Encoding_రికార్డు_Iterator))>
  తార్కిక _can_drop (స్థిరరాశి Encoding_రికార్డు& cmap12,
                  స్థిరరాశి hb_సమితి_t& cmap12_unicodes,
                  స్థిరరాశి శూన్య* పునాది,
                  స్థిరరాశి SubtableUnicodesCache& unicodes_cache,
                  SubtableUnicodesCache& local_unicodes_cache,
                  Iterator subset_unicodes,
                  Encoding_రికార్డు_Iterator encoding_రికార్డులు)
  {
    కొరకు (స్వయంచాలక cp : + subset_unicodes | hb_filter (cmap12_unicodes))
    {
      ఒకవేళ (cp >= 0x10000) తిరిగి అబద్ధం;
    }

    గుర్తులేని లక్ష్యం_platform;
    గుర్తులేని లక్ష్యం_encoding;
    గుర్తులేని లక్ష్యం_language = (పునాది+cmap12.subtable).get_language ();

    ఒకవేళ (cmap12.platformID == 0 && cmap12.encodingID == 4)
    {
      లక్ష్యం_platform = 0;
      లక్ష్యం_encoding = 3;
    } లేకపోతే ఒకవేళ (cmap12.platformID == 3 && cmap12.encodingID == 10) {
      లక్ష్యం_platform = 3;
      లక్ష్యం_encoding = 1;
    } లేకపోతే {
      తిరిగి అబద్ధం;
    }

    కొరకు (స్థిరరాశి స్వయంచాలక& _ : encoding_రికార్డులు)
    {
      ఒకవేళ (_.platformID != లక్ష్యం_platform
          || _.encodingID != లక్ష్యం_encoding
          || (పునాది+_.subtable).get_language() != లక్ష్యం_language)
        కొనసాగు;

      స్థిరరాశి hb_సమితి_t* sibling_unicodes = unicodes_cache.సమితి_for (&_, local_unicodes_cache);

      స్వయంచాలక cmap12 = + subset_unicodes | hb_filter (cmap12_unicodes);
      స్వయంచాలక sibling = + subset_unicodes | hb_filter (*sibling_unicodes);
      కొరకు (; cmap12 && sibling; cmap12++, sibling++)
      {
        గుర్తులేని a = *cmap12;
        గుర్తులేని b = *sibling;
        ఒకవేళ (a != b) తిరిగి అబద్ధం;
      }

      తిరిగి !cmap12 && !sibling;
    }

    తిరిగి అబద్ధం;
  }

  శూన్య సంవృతం_glyphs (స్థిరరాశి hb_సమితి_t      *unicodes,
		       hb_సమితి_t            *glyphset) స్థిరరాశి
  {
    + hb_iter (encoding_రికార్డు)
    | hb_map (&Encoding_రికార్డు::subtable)
    | hb_map (hb_కూడు (ఇది))
    | hb_filter (&CmapSubtable::is_variation_selector)
    | hb_apply ([=] (స్థిరరాశి CmapSubtable& _) { _.సంవృతం_glyphs (unicodes, glyphset); })
    ;
  }

  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  { తిరిగి subset (c, అబద్ధం); }

  తార్కిక subset (hb_subset_context_t *c, తార్కిక is_dmap) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);

    cmap *cmap_ప్రధానసంఖ్య = c->serializer->ప్రారంభం_embed<cmap> ();

    స్వయంచాలక encodingrec_iter =
    + hb_iter (encoding_రికార్డు)
    | hb_filter ([&](స్థిరరాశి Encoding_రికార్డు& _) {
      తిరిగి cmap::filter_encoding_రికార్డులు_for_subset (ఇది, _);
    })
    ;

    ఒకవేళ (unlikely (!encodingrec_iter.len ())) return_trace (అబద్ధం);

    స్థిరరాశి Encoding_రికార్డు *unicode_bmp= శూన్యసూచిక, *unicode_ucs4 = శూన్యసూచిక, *ms_bmp = శూన్యసూచిక, *ms_ucs4 = శూన్యసూచిక;
    తార్కిక has_format12 = అబద్ధం;

    కొరకు (స్థిరరాశి Encoding_రికార్డు& _ : encodingrec_iter)
    {
      గుర్తులేని format = (ఇది + _.subtable).u.format.v;
      ఒకవేళ (format == 12) has_format12 = నిజం;

      స్థిరరాశి Encoding_రికార్డు *పట్టిక = std::addressof (_);
      ఒకవేళ      (_.platformID == 0 && _.encodingID ==  3) unicode_bmp = పట్టిక;
      లేకపోతే ఒకవేళ (_.platformID == 0 && _.encodingID ==  4) unicode_ucs4 = పట్టిక;
      లేకపోతే ఒకవేళ (_.platformID == 3 && _.encodingID ==  1) ms_bmp = పట్టిక;
      లేకపోతే ఒకవేళ (_.platformID == 3 && _.encodingID == 10) ms_ucs4 = పట్టిక;
    }

    ఒకవేళ (unlikely (!is_dmap && !has_format12 && !unicode_bmp && !ms_bmp)) return_trace (అబద్ధం);
    ఒకవేళ (unlikely (!is_dmap && has_format12 && (!unicode_ucs4 && !ms_ucs4))) return_trace (అబద్ధం);

    స్వయంచాలక it =
    + c->plan->unicode_గమ్యం_new_gid_list.iter ()
    | hb_filter ([&] (స్థిరరాశి hb_codepoint_pair_t _)
		 { తిరిగి (_.second != HB_MAP_విలువ_INVALID); })
    ;

    return_trace (cmap_ప్రధానసంఖ్య->serialize (c->serializer,
                                         it,
                                         encodingrec_iter,
                                         ఇది,
                                         c->plan,
					 అబద్ధం,
					 c->ఆధారం_blob->నిడివి));
  }

  స్థిరరాశి CmapSubtable *కనుగొను_ఉత్తమ_subtable (తార్కిక *చిహ్నం = శూన్యసూచిక,
					  తార్కిక *mac = శూన్యసూచిక,
					  తార్కిక *macroman = శూన్యసూచిక) స్థిరరాశి
  {
    ఒకవేళ (చిహ్నం) *చిహ్నం = అబద్ధం;
    ఒకవేళ (mac) *mac = అబద్ధం;
    ఒకవేళ (macroman) *macroman = అబద్ధం;

    స్థిరరాశి CmapSubtable *subtable;

    /* Symbol subtable.
     * Prefer symbol if available.
     * https://github.com/harfbuzz/harfbuzz/issues/1918 */
    ఒకవేళ ((subtable = ఇది->కనుగొను_subtable (3, 0)))
    {
      ఒకవేళ (చిహ్నం) *చిహ్నం = నిజం;
      తిరిగి subtable;
    }

    /* 32-bit subtables. */
    ఒకవేళ ((subtable = ఇది->కనుగొను_subtable (3, 10))) తిరిగి subtable;
    ఒకవేళ ((subtable = ఇది->కనుగొను_subtable (0, 6))) తిరిగి subtable;
    ఒకవేళ ((subtable = ఇది->కనుగొను_subtable (0, 4))) తిరిగి subtable;

    /* 16-bit subtables. */
    ఒకవేళ ((subtable = ఇది->కనుగొను_subtable (3, 1))) తిరిగి subtable;
    ఒకవేళ ((subtable = ఇది->కనుగొను_subtable (0, 3))) తిరిగి subtable;
    ఒకవేళ ((subtable = ఇది->కనుగొను_subtable (0, 2))) తిరిగి subtable;
    ఒకవేళ ((subtable = ఇది->కనుగొను_subtable (0, 1))) తిరిగి subtable;
    ఒకవేళ ((subtable = ఇది->కనుగొను_subtable (0, 0))) తిరిగి subtable;

    /* MacRoman subtable. */
    ఒకవేళ ((subtable = ఇది->కనుగొను_subtable (1, 0)))
    {
      ఒకవేళ (mac) *mac = నిజం;
      ఒకవేళ (macroman) *macroman = నిజం;
      తిరిగి subtable;
    }
    /* Any other Mac subtable; we just map ASCII for these. */
    ఒకవేళ ((subtable = ఇది->కనుగొను_subtable (1, 0xFFFF)))
    {
      ఒకవేళ (mac) *mac = నిజం;
      తిరిగి subtable;
    }

    /* Meh. */
    తిరిగి &Null (CmapSubtable);
  }

  నిర్మాణం accelerator_t
  {
    ఉపయోగించి cache_t = hb_cache_t<21, 19>;
    static_assert (పరిమాణం (cache_t) == 1024, "");

    accelerator_t (hb_face_t *face, hb_ట్యాగ్_t పట్టిక_ట్యాగ్_2 = cmap::పట్టిక_ట్యాగ్)
    {
      ఇది->పట్టిక = hb_sanitize_context_t ().reference_పట్టిక<cmap> (face, పట్టిక_ట్యాగ్_2);
      ఒకవేళ (!ఇది->పట్టిక.get_నిడివి ())
	తిరిగి;

      తార్కిక చిహ్నం, mac, macroman;
      ఇది->subtable = పట్టిక->కనుగొను_ఉత్తమ_subtable (&చిహ్నం, &mac, &macroman);
      ఇది->subtable_uvs = &Null (CmapSubtable);
      {
	స్థిరరాశి CmapSubtable *st = పట్టిక->కనుగొను_variation_selector_subtable ();
	ఒకవేళ (st)
	  subtable_uvs = st;
      }

#నిర్వచితం_కాకపోతే HB_NO_OT_FONT_CMAP_CACHE
      cache = (cache_t *) hb_malloc (పరిమాణం (cache_t));
      ఒకవేళ (cache)
	కొత్త (cache) cache_t ();
      లేకపోతే
        తిరిగి; // Such that get_glyph_funcZ remains null.
#అయితే_ముగింపు

      ఇది->get_glyph_దత్తాంశం = subtable;
#నిర్వచితం_కాకపోతే HB_NO_CMAP_LEGACY_SUBTABLES
      తార్కిక is_format4 = subtable->u.format.v == 4;
      స్వయంచాలక సమితి_format4_getter = [ఇది] (తార్కిక (*func) (స్థిరరాశి శూన్య *,
						      hb_codepoint_t,
						      hb_codepoint_t *))
      {
	ఇది->format4_accel.init (&ఇది->subtable->u.format4,
				  get_subtable_దత్తాంశం_కొలత (ఇది->subtable));
	ఇది->get_glyph_దత్తాంశం = &ఇది->format4_accel;
	ఇది->get_glyph_funcZ = func;
      };
      ఒకవేళ (unlikely (చిహ్నం))
      {
	ఎంపిక ((గుర్తులేని) face->పట్టిక.OS2->get_font_పుట ()) {
	స్థితి OS2::font_పుట_t::FONT_పుట_NONE:
	  ఒకవేళ (is_format4)
	    సమితి_format4_getter (get_glyph_from_చిహ్నం<CmapSubtableFormat4::accelerator_t, hb_చిహ్నం_pua_map>);
	  లేకపోతే
	    ఇది->get_glyph_funcZ = get_glyph_from_చిహ్నం<CmapSubtable, hb_చిహ్నం_pua_map>;
	  ఆపు;
#నిర్వచితం_కాకపోతే HB_NO_OT_SHAPER_ARABIC_FALLBACK
	స్థితి OS2::font_పుట_t::FONT_పుట_SIMP_ARABIC:
	  ఒకవేళ (is_format4)
	    సమితి_format4_getter (get_glyph_from_చిహ్నం<CmapSubtableFormat4::accelerator_t, _hb_arabic_pua_simp_map>);
	  లేకపోతే
	    ఇది->get_glyph_funcZ = get_glyph_from_చిహ్నం<CmapSubtable, _hb_arabic_pua_simp_map>;
	  ఆపు;
	స్థితి OS2::font_పుట_t::FONT_పుట_TRAD_ARABIC:
	  ఒకవేళ (is_format4)
	    సమితి_format4_getter (get_glyph_from_చిహ్నం<CmapSubtableFormat4::accelerator_t, _hb_arabic_pua_trad_map>);
	  లేకపోతే
	    ఇది->get_glyph_funcZ = get_glyph_from_చిహ్నం<CmapSubtable, _hb_arabic_pua_trad_map>;
	  ఆపు;
#అయితే_ముగింపు
	అప్రమేయ:
	  ఒకవేళ (is_format4)
	    సమితి_format4_getter (get_glyph_from<CmapSubtableFormat4::accelerator_t>);
	  లేకపోతే
	    ఇది->get_glyph_funcZ = get_glyph_from<CmapSubtable>;
	  ఆపు;
	}
      }
      లేకపోతే ఒకవేళ (unlikely (macroman))
      {
	ఒకవేళ (is_format4)
	  సమితి_format4_getter (get_glyph_from_macroman<CmapSubtableFormat4::accelerator_t>);
	లేకపోతే
	  ఇది->get_glyph_funcZ = get_glyph_from_macroman<CmapSubtable>;
      }
      లేకపోతే ఒకవేళ (unlikely (mac))
      {
	ఒకవేళ (is_format4)
	  సమితి_format4_getter (get_glyph_from_ascii<CmapSubtableFormat4::accelerator_t>);
	లేకపోతే
	  ఇది->get_glyph_funcZ = get_glyph_from_ascii<CmapSubtable>;
      }
      లేకపోతే
#అయితే_ముగింపు
      {
	ఎంపిక (subtable->u.format.v) {
	  /* Accelerate format 4 and format 12. */
	  అప్రమేయ:
	    ఇది->get_glyph_funcZ = get_glyph_from<CmapSubtable>;
	    ఆపు;
	  స్థితి 12:
	    ఇది->get_glyph_funcZ = get_glyph_from<CmapSubtableFormat12>;
	    ఆపు;
	  స్థితి  4:
	  {
	    hb_barrier ();
	    ఇది->format4_accel.init (&subtable->u.format4,
				      get_subtable_దత్తాంశం_కొలత (subtable));
	    ఇది->get_glyph_దత్తాంశం = &ఇది->format4_accel;
	    ఇది->get_glyph_funcZ = ఇది->format4_accel.get_glyph_func;
	    ఆపు;
	  }
	}
      }
    }
    ~accelerator_t ()
    {
#నిర్వచితం_కాకపోతే HB_NO_OT_FONT_CMAP_CACHE
      hb_free (cache);
#అయితే_ముగింపు
      పట్టిక.destroy ();
    }

    అంతఃవరుస తార్కిక _cached_get (hb_codepoint_t unicode,
			     hb_codepoint_t *glyph) స్థిరరాశి
    {
#నిర్వచితం_కాకపోతే HB_NO_OT_FONT_CMAP_CACHE
      // cache is always non-null if we have a get_glyph_funcZ
      గుర్తులేని v;
      ఒకవేళ (cache->get (unicode, &v))
      {
        *glyph = v;
	తిరిగి నిజం;
      }
#అయితే_ముగింపు
      తార్కిక ret  = ఇది->get_glyph_funcZ (ఇది->get_glyph_దత్తాంశం, unicode, glyph);

#నిర్వచితం_కాకపోతే HB_NO_OT_FONT_CMAP_CACHE
      ఒకవేళ (ret)
        cache->సమితి (unicode, *glyph);
#అయితే_ముగింపు

      తిరిగి ret;
    }

    తార్కిక get_nominal_glyph (hb_codepoint_t  unicode,
			    hb_codepoint_t *glyph) స్థిరరాశి
    {
      ఒకవేళ (unlikely (!ఇది->get_glyph_funcZ)) తిరిగి అబద్ధం;
      తిరిగి _cached_get (unicode, glyph);
    }

    గుర్తులేని పూర్ణాంకం get_nominal_glyphs (గుర్తులేని పూర్ణాంకం లెక్క,
				     స్థిరరాశి hb_codepoint_t *మొదటి_unicode,
				     గుర్తులేని పూర్ణాంకం unicode_stride,
				     hb_codepoint_t *మొదటి_glyph,
				     గుర్తులేని పూర్ణాంకం glyph_stride) స్థిరరాశి
    {
      ఒకవేళ (unlikely (!ఇది->get_glyph_funcZ)) తిరిగి 0;

      గుర్తులేని పూర్ణాంకం పూర్తి;
      కొరకు (పూర్తి = 0;
	   పూర్తి < లెక్క && _cached_get (*మొదటి_unicode, మొదటి_glyph);
	   పూర్తి++)
      {
	మొదటి_unicode = &Struct_At_స్థానభ్రంశం_Unaligned<hb_codepoint_t> (మొదటి_unicode, unicode_stride);
	మొదటి_glyph = &Struct_At_స్థానభ్రంశం_Unaligned<hb_codepoint_t> (మొదటి_glyph, glyph_stride);
      }
      తిరిగి పూర్తి;
    }

    తార్కిక get_variation_glyph (hb_codepoint_t  unicode,
			      hb_codepoint_t  variation_selector,
			      hb_codepoint_t *glyph) స్థిరరాశి
    {
      ఎంపిక (get_glyph_variant (unicode, variation_selector, glyph))
      {
	స్థితి GLYPH_VARIANT_NOT_దొరికింది:	తిరిగి అబద్ధం;
	స్థితి GLYPH_VARIANT_దొరికింది:	తిరిగి నిజం;
	స్థితి GLYPH_VARIANT_USE_DEFAULT:	ఆపు;
      }

      తిరిగి get_nominal_glyph (unicode, glyph);
    }

    glyph_variant_t get_glyph_variant (hb_codepoint_t  unicode,
				       hb_codepoint_t  variation_selector,
				       hb_codepoint_t *glyph) స్థిరరాశి
    {
      తిరిగి ఇది->subtable_uvs->get_glyph_variant (unicode,
						    variation_selector,
						    glyph);
    }

    తార్కిక has_దత్తాంశం () స్థిరరాశి { తిరిగి పట్టిక.get_నిడివి (); }

    శూన్య collect_unicodes (hb_సమితి_t *బహిర్గతం, గుర్తులేని పూర్ణాంకం అంకె_glyphs) స్థిరరాశి
    { subtable->collect_unicodes (బహిర్గతం, అంకె_glyphs, get_subtable_దత్తాంశం_కొలత (subtable)); }
    శూన్య collect_mapping (hb_సమితి_t *unicodes, hb_map_t *mapping,
			  గుర్తులేని అంకె_glyphs = UINT_MAX) స్థిరరాశి
    { subtable->collect_mapping (unicodes, mapping, అంకె_glyphs, get_subtable_దత్తాంశం_కొలత (subtable)); }
    శూన్య collect_variation_selectors (hb_సమితి_t *బహిర్గతం) స్థిరరాశి
    { subtable_uvs->collect_variation_selectors (బహిర్గతం); }
    శూన్య collect_variation_unicodes (hb_codepoint_t variation_selector,
				     hb_సమితి_t *బహిర్గతం) స్థిరరాశి
    { subtable_uvs->collect_variation_unicodes (variation_selector, బహిర్గతం); }

    రక్షిత:
    రకనిర్వచనం తార్కిక (*hb_cmap_get_glyph_func_t) (స్థిరరాశి శూన్య *obj,
					      hb_codepoint_t codepoint,
					      hb_codepoint_t *glyph);
    రకనిర్వచనం uint16_t (*hb_pua_remap_func_t) (గుర్తులేని);

    మూస <రకం_పేరు Type>
    HB_అంతర్గత స్థిర తార్కిక get_glyph_from (స్థిరరాశి శూన్య *obj,
					    hb_codepoint_t codepoint,
					    hb_codepoint_t *glyph)
    {
      స్థిరరాశి Type *typed_obj = (స్థిరరాశి Type *) obj;
      తిరిగి typed_obj->get_glyph (codepoint, glyph);
    }

    మూస <రకం_పేరు Type, hb_pua_remap_func_t remap>
    HB_అంతర్గత స్థిర తార్కిక get_glyph_from_చిహ్నం (స్థిరరాశి శూన్య *obj,
						   hb_codepoint_t codepoint,
						   hb_codepoint_t *glyph)
    {
      స్థిరరాశి Type *typed_obj = (స్థిరరాశి Type *) obj;
      ఒకవేళ (likely (typed_obj->get_glyph (codepoint, glyph)))
	తిరిగి నిజం;

      ఒకవేళ (hb_codepoint_t c = remap (codepoint))
	తిరిగి typed_obj->get_glyph (c, glyph);

      తిరిగి అబద్ధం;
    }

    మూస <రకం_పేరు Type>
    HB_అంతర్గత స్థిర తార్కిక get_glyph_from_ascii (స్థిరరాశి శూన్య *obj,
						  hb_codepoint_t codepoint,
						  hb_codepoint_t *glyph)
    {
      స్థిరరాశి Type *typed_obj = (స్థిరరాశి Type *) obj;
      తిరిగి codepoint < 0x80 && typed_obj->get_glyph (codepoint, glyph);
    }

    మూస <రకం_పేరు Type>
    HB_అంతర్గత స్థిర తార్కిక get_glyph_from_macroman (స్థిరరాశి శూన్య *obj,
						     hb_codepoint_t codepoint,
						     hb_codepoint_t *glyph)
    {
      ఒకవేళ (get_glyph_from_ascii<Type> (obj, codepoint, glyph))
	తిరిగి నిజం;

      స్థిరరాశి Type *typed_obj = (స్థిరరాశి Type *) obj;
      గుర్తులేని c = unicode_గమ్యం_macroman (codepoint);
      తిరిగి c && typed_obj->get_glyph (c, glyph);
    }

    గుర్తులేని పూర్ణాంకం get_subtable_దత్తాంశం_కొలత (స్థిరరాశి CmapSubtable *subtable) స్థిరరాశి
    {
      గుర్తులేని పూర్ణాంకం పట్టిక_నిడివి = ఇది->పట్టిక.get_నిడివి ();
      uintptr_t పట్టిక_ప్రారంభం = (uintptr_t) (స్థిరరాశి శూన్య *) ఇది->పట్టిక.get ();
      uintptr_t subtable_addr = (uintptr_t) (స్థిరరాశి శూన్య *) subtable;
      ఒకవేళ (unlikely (subtable_addr < పట్టిక_ప్రారంభం))
	తిరిగి 0;

      uintptr_t subtable_స్థానభ్రంశం = subtable_addr - పట్టిక_ప్రారంభం;
      ఒకవేళ (unlikely (subtable_స్థానభ్రంశం >= పట్టిక_నిడివి))
	తిరిగి 0;

      తిరిగి పట్టిక_నిడివి - (గుర్తులేని పూర్ణాంకం) subtable_స్థానభ్రంశం;
    }

    అంతరంగ:
    hb_nonnull_ptr_t<స్థిరరాశి CmapSubtable> subtable;
    hb_nonnull_ptr_t<స్థిరరాశి CmapSubtable> subtable_uvs;

    hb_cmap_get_glyph_func_t get_glyph_funcZ = శూన్యసూచిక;
    స్థిరరాశి శూన్య *get_glyph_దత్తాంశం = శూన్యసూచిక;

    CmapSubtableFormat4::accelerator_t format4_accel;

#నిర్వచితం_కాకపోతే HB_NO_OT_FONT_CMAP_CACHE
    cache_t *cache = శూన్యసూచిక;
#అయితే_ముగింపు

    బహిరంగ:
    hb_blob_ptr_t<cmap> పట్టిక;
  };

  రక్షిత:

  స్థిరరాశి CmapSubtable *కనుగొను_subtable (గుర్తులేని పూర్ణాంకం platform_id,
				     గుర్తులేని పూర్ణాంకం encoding_id) స్థిరరాశి
  {
    Encoding_రికార్డు key;
    key.platformID = platform_id;
    key.encodingID = encoding_id;

    స్థిరరాశి Encoding_రికార్డు &ఫలితం = encoding_రికార్డు.bsearch (key);
    ఒకవేళ (!ఫలితం.subtable)
      తిరిగి శూన్యసూచిక;

    తిరిగి &(ఇది+ఫలితం.subtable);
  }

  స్థిరరాశి CmapSubtable *కనుగొను_variation_selector_subtable () స్థిరరాశి
  {
    స్థిరరాశి CmapSubtable *format14 = శూన్యసూచిక;
    కొరకు (స్థిరరాశి Encoding_రికార్డు& రికార్డు : encoding_రికార్డు)
    {
      ఒకవేళ (రికార్డు.platformID != 0 || రికార్డు.encodingID != 5)
	కొనసాగు;

      స్థిరరాశి CmapSubtable *subtable = &(ఇది+రికార్డు.subtable);
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
      ఒకవేళ (subtable->u.format.v == 15)
	తిరిగి subtable;
#అయితే_ముగింపు
      ఒకవేళ (subtable->u.format.v == 14)
	format14 = subtable;
    }
    తిరిగి format14;
  }

  బహిరంగ:

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) &&
		  hb_barrier () &&
		  likely (సంస్కరణ == 0) &&
		  encoding_రికార్డు.sanitize (c, ఇది));
  }

 అంతరంగ:

  స్థిర తార్కిక filter_encoding_రికార్డులు_for_subset(స్థిరరాశి cmap* cmap,
                                                 స్థిరరాశి Encoding_రికార్డు& _)
  {
    తిరిగి
        (_.platformID == 0 && _.encodingID == 3) ||
        (_.platformID == 0 && _.encodingID == 4) ||
        (_.platformID == 3 && _.encodingID == 1) ||
        (_.platformID == 3 && _.encodingID == 10) ||
        (cmap + _.subtable).is_variation_selector ();
  }

  రక్షిత:
  HBUINT16	సంస్కరణ;	/* Table version number (0). */
  Sorted_Array16_యొక్క<Encoding_రికార్డు>
		encoding_రికార్డు;	/* Encoding tables. */
  బహిరంగ:
  DEFINE_కొలత_ARRAY (4, encoding_రికార్డు);
};

నిర్మాణం DMAP : cmap
{
  స్థిర constexpr hb_ట్యాగ్_t పట్టిక_ట్యాగ్ = HB_OT_ట్యాగ్_DMAP;

  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  { తిరిగి cmap::subset (c, నిజం); }
};

నిర్మాణం cmap_accelerator_t
{
  cmap_accelerator_t (hb_face_t *face) :
    dmap (face, HB_OT_ట్యాగ్_DMAP),
    పునాది (face)
  {}

  తార్కిక get_nominal_glyph (hb_codepoint_t  unicode,
			  hb_codepoint_t *glyph) స్థిరరాశి
  {
    తిరిగి dmap.get_nominal_glyph (unicode, glyph) ||
	   పునాది.get_nominal_glyph (unicode, glyph);
  }

  గుర్తులేని పూర్ణాంకం get_nominal_glyphs (గుర్తులేని పూర్ణాంకం లెక్క,
				   స్థిరరాశి hb_codepoint_t *మొదటి_unicode,
				   గుర్తులేని పూర్ణాంకం unicode_stride,
				   hb_codepoint_t *మొదటి_glyph,
				   గుర్తులేని పూర్ణాంకం glyph_stride) స్థిరరాశి
  {
    ఒకవేళ (likely (!dmap.has_దత్తాంశం ()))
      తిరిగి పునాది.get_nominal_glyphs (లెక్క,
				      మొదటి_unicode, unicode_stride,
				      మొదటి_glyph, glyph_stride);

    గుర్తులేని పూర్ణాంకం పూర్తి;
    కొరకు (పూర్తి = 0;
	 పూర్తి < లెక్క && get_nominal_glyph (*మొదటి_unicode, మొదటి_glyph);
	 పూర్తి++)
    {
      మొదటి_unicode = &Struct_At_స్థానభ్రంశం_Unaligned<hb_codepoint_t> (మొదటి_unicode, unicode_stride);
      మొదటి_glyph = &Struct_At_స్థానభ్రంశం_Unaligned<hb_codepoint_t> (మొదటి_glyph, glyph_stride);
    }
    తిరిగి పూర్తి;
  }

  తార్కిక get_variation_glyph (hb_codepoint_t  unicode,
			    hb_codepoint_t  variation_selector,
			    hb_codepoint_t *glyph) స్థిరరాశి
  {
    glyph_variant_t ఫలితం = dmap.get_glyph_variant (unicode,
						     variation_selector,
						     glyph);
    ఒకవేళ (ఫలితం == GLYPH_VARIANT_NOT_దొరికింది)
      ఫలితం = పునాది.get_glyph_variant (unicode, variation_selector, glyph);

    ఎంపిక (ఫలితం)
    {
      స్థితి GLYPH_VARIANT_NOT_దొరికింది: తిరిగి అబద్ధం;
      స్థితి GLYPH_VARIANT_దొరికింది: తిరిగి నిజం;
      స్థితి GLYPH_VARIANT_USE_DEFAULT: ఆపు;
    }

    తిరిగి get_nominal_glyph (unicode, glyph);
  }

  శూన్య collect_unicodes (hb_సమితి_t *బహిర్గతం, గుర్తులేని పూర్ణాంకం అంకె_glyphs) స్థిరరాశి
  {
    పునాది.collect_unicodes (బహిర్గతం, అంకె_glyphs);
    dmap.collect_unicodes (బహిర్గతం, అంకె_glyphs);
  }

  శూన్య collect_mapping (hb_సమితి_t *unicodes, hb_map_t *mapping,
			గుర్తులేని అంకె_glyphs = UINT_MAX) స్థిరరాశి
  {
    పునాది.collect_mapping (unicodes, mapping, అంకె_glyphs);
    dmap.collect_mapping (unicodes, mapping, అంకె_glyphs);
  }

  శూన్య collect_variation_selectors (hb_సమితి_t *బహిర్గతం) స్థిరరాశి
  {
    పునాది.collect_variation_selectors (బహిర్గతం);
    dmap.collect_variation_selectors (బహిర్గతం);
  }

  శూన్య collect_variation_unicodes (hb_codepoint_t variation_selector,
				   hb_సమితి_t *బహిర్గతం) స్థిరరాశి
  {
    పునాది.collect_variation_unicodes (variation_selector, బహిర్గతం);
    dmap.collect_variation_unicodes (variation_selector, బహిర్గతం);
  }

  శూన్య సంవృతం_glyphs (స్థిరరాశి hb_సమితి_t *unicodes,
		       hb_సమితి_t       *glyphset) స్థిరరాశి
  {
    ఒకవేళ (likely (!dmap.has_దత్తాంశం ()))
    {
      పునాది.పట్టిక->సంవృతం_glyphs (unicodes, glyphset);
      తిరిగి;
    }

    hb_సమితి_t variation_selectors;
    collect_variation_selectors (&variation_selectors);
    కొరకు (hb_codepoint_t variation_selector : variation_selectors)
    {
      ఒకవేళ (!unicodes->has (variation_selector))
	కొనసాగు;

      hb_సమితి_t variation_unicodes;
      collect_variation_unicodes (variation_selector, &variation_unicodes);
      కొరకు (hb_codepoint_t unicode : variation_unicodes)
      {
	ఒకవేళ (!unicodes->has (unicode))
	  కొనసాగు;

	hb_codepoint_t glyph;
	ఒకవేళ (get_variation_glyph (unicode, variation_selector, &glyph))
	  glyphset->కూడు (glyph);
      }
    }
  }

  cmap::accelerator_t dmap;
  cmap::accelerator_t పునాది;
};

} /* namespace OT */


#అయితే_ముగింపు /* HB_OT_CMAP_TABLE_HH */
