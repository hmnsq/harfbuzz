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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OT_CMAP_ਸਾਰਣੀ_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_CMAP_ਸਾਰਣੀ_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-os2-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-shaper-arabic-pua.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-open-type.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-dual.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-set.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-cache.hh"

/*
 * cmap -- Character to Glyph Index Mapping
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cmap
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_ਟੈਗ_cmap HB_ਟੈਗ('c','m','a','p')
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_ਟੈਗ_DMAP HB_ਟੈਗ('D','M','A','P')

ਨਾਮਾਕਾਸ਼ OT {

ਸਥਿਰ ਸਤਰਅੰਦਰ uint8_t unicode_ਮੰਜ਼ਿਲ_macroman (hb_codepoint_t u)
{
  ਸਥਿਰ ਅਚਲ ਬਣਤਰ unicode_ਮੰਜ਼ਿਲ_macroman_t
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
  ਸਵੈਚਾਲਿਤ *c = hb_bsearch (u, mapping, ARRAY_ਲੰਮਾਈ (mapping), ਆਕਾਰ (mapping[0]),
			_hb_cmp_operator<uint16_t, uint16_t>);
  ਵਾਪਸ c ? c->macroman : 0;
}

ਬਣਤਰ CmapSubtableFormat0
{
  ਤਾਰਕਿਕ get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) ਅਚਲ
  {
    hb_codepoint_t gid = codepoint < 256 ? glyphIdArray[codepoint] : 0;
    ਜੇ (unlikely (!gid))
      ਵਾਪਸ ਝੂਠ;
    *glyph = gid;
    ਵਾਪਸ ਸੱਚ;
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ get_language () ਅਚਲ
  {
    ਵਾਪਸ language;
  }

  ਖਾਲੀ collect_unicodes (hb_ਸੈੱਟ_t *ਬਾਹਰਲਾ) ਅਚਲ
  {
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < 256; i++)
      ਜੇ (glyphIdArray[i])
	ਬਾਹਰਲਾ->ਜੋੜੋ (i);
  }

  ਖਾਲੀ collect_mapping (hb_ਸੈੱਟ_t *unicodes, /*ਬਾਹਰਲਾ*/
			hb_map_t *mapping /*ਬਾਹਰਲਾ*/) ਅਚਲ
  {
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < 256; i++)
      ਜੇ (glyphIdArray[i])
      {
	hb_codepoint_t glyph = glyphIdArray[i];
	unicodes->ਜੋੜੋ (i);
	mapping->ਸੈੱਟ (i, glyph);
      }
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ));
  }

  ਸੁਰੱਖਿਅਤ:
  HBUINT16	format;		/* Format number is set to 0. */
  HBUINT16	ਲੰਮਾਈ;		/* Byte length of this subtable. */
  HBUINT16	language;	/* Ignore. */
  HBUINT8	glyphIdArray[256];/* An array that maps character
				 * code to glyph index values. */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (6 + 256);
};

ਬਣਤਰ CmapSubtableFormat4
{
  ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ Iterator,
      ਕਿਸਮ_ਨਾਂ Writer,
	   hb_requires (hb_is_iterator (Iterator))>
  ਖਾਲੀ ਮੰਜ਼ਿਲ_ranges (Iterator it, Writer& range_writer)
  {
    hb_codepoint_t ਸ਼ੁਰੂ_cp = 0, prev_ਲੜੀਵਾਰ_ਸ਼ੁਰੂ_cp = 0, ਲੜੀਵਾਰ_ਸ਼ੁਰੂ_cp = 0, ਸਿਰਾ_cp = 0, ਆਖਰੀ_gid = 0;
    ਪੂਰਨ ਲੜੀਵਾਰ_ਲੰਮਾਈ = 0 , delta = 0, prev_delta = 0;

    ਗਿਣਤੀ {
      ਪਹਿਲਾ_SUB_RANGE,
      FOLLOWING_SUB_RANGE,
    } ਬਹੁਲਕ;

    ਜਦੋਂ (it) {
      // Start a new range
      {
        ਅਚਲ ਸਵੈਚਾਲਿਤ& pair = *it;
        ਸ਼ੁਰੂ_cp = pair.ਪਹਿਲਾ;
        prev_ਲੜੀਵਾਰ_ਸ਼ੁਰੂ_cp = ਸ਼ੁਰੂ_cp;
        ਲੜੀਵਾਰ_ਸ਼ੁਰੂ_cp = ਸ਼ੁਰੂ_cp;
        ਸਿਰਾ_cp = ਸ਼ੁਰੂ_cp;
        ਆਖਰੀ_gid = pair.second;
        ਲੜੀਵਾਰ_ਲੰਮਾਈ = 1;
        prev_delta = 0;
      }

      delta = ਆਖਰੀ_gid - ਸ਼ੁਰੂ_cp;
      ਬਹੁਲਕ = ਪਹਿਲਾ_SUB_RANGE;
      it++;

      ਜਦੋਂ (it) {
        // Process range
        ਅਚਲ ਸਵੈਚਾਲਿਤ& pair = *it;
        hb_codepoint_t ਅਗਲਾ_cp = pair.ਪਹਿਲਾ;
        hb_codepoint_t ਅਗਲਾ_gid = pair.second;
        ਜੇ (ਅਗਲਾ_cp != ਸਿਰਾ_cp + 1) {
          // Current range is over, stop processing.
          ਰੋਕੋ;
        }

        ਜੇ (ਅਗਲਾ_gid == ਆਖਰੀ_gid + 1) {
          // The current run continues.
          ਸਿਰਾ_cp = ਅਗਲਾ_cp;
          ਲੜੀਵਾਰ_ਲੰਮਾਈ++;
          ਆਖਰੀ_gid = ਅਗਲਾ_gid;
          it++;
          ਜਾਰੀ;
        }

        // A new run is starting, decide if we want to commit the current run.
        ਪੂਰਨ ਵੰਡੋ_ਲਾਗਤ = (ਬਹੁਲਕ == ਪਹਿਲਾ_SUB_RANGE) ? 8 : 16;
        ਪੂਰਨ ਲੜੀਵਾਰ_ਲਾਗਤ = ਲੜੀਵਾਰ_ਲੰਮਾਈ * 2;
        ਜੇ (ਲੜੀਵਾਰ_ਲਾਗਤ >= ਵੰਡੋ_ਲਾਗਤ) {
          commit_current_range(ਸ਼ੁਰੂ_cp,
                               prev_ਲੜੀਵਾਰ_ਸ਼ੁਰੂ_cp,
                               ਲੜੀਵਾਰ_ਸ਼ੁਰੂ_cp,
                               ਸਿਰਾ_cp,
                               delta,
                               prev_delta,
                               ਵੰਡੋ_ਲਾਗਤ,
                               range_writer);
          ਸ਼ੁਰੂ_cp = ਅਗਲਾ_cp;
        }

        // Start the new run
        ਬਹੁਲਕ = FOLLOWING_SUB_RANGE;
        prev_ਲੜੀਵਾਰ_ਸ਼ੁਰੂ_cp = ਲੜੀਵਾਰ_ਸ਼ੁਰੂ_cp;
        ਲੜੀਵਾਰ_ਸ਼ੁਰੂ_cp = ਅਗਲਾ_cp;
        ਸਿਰਾ_cp = ਅਗਲਾ_cp;
        prev_delta = delta;
        delta = ਅਗਲਾ_gid - ਲੜੀਵਾਰ_ਸ਼ੁਰੂ_cp;
        ਲੜੀਵਾਰ_ਲੰਮਾਈ = 1;
        ਆਖਰੀ_gid = ਅਗਲਾ_gid;
        it++;
      }

      // Finalize range
      commit_current_range (ਸ਼ੁਰੂ_cp,
                            prev_ਲੜੀਵਾਰ_ਸ਼ੁਰੂ_cp,
                            ਲੜੀਵਾਰ_ਸ਼ੁਰੂ_cp,
                            ਸਿਰਾ_cp,
                            delta,
                            prev_delta,
                            8,
                            range_writer);
    }

    ਜੇ (likely (ਸਿਰਾ_cp != 0xFFFF)) {
      range_writer (0xFFFF, 0xFFFF, 1);
    }
  }

  /*
   * Writes the current range as either one or two ranges depending on what is most efficient.
   */
  ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ Writer>
  ਖਾਲੀ commit_current_range (hb_codepoint_t ਸ਼ੁਰੂ,
                             hb_codepoint_t prev_ਲੜੀਵਾਰ_ਸ਼ੁਰੂ,
                             hb_codepoint_t ਲੜੀਵਾਰ_ਸ਼ੁਰੂ,
                             hb_codepoint_t ਸਿਰਾ,
                             ਪੂਰਨ ਲੜੀਵਾਰ_delta,
                             ਪੂਰਨ ਪਿਛਲਾ_ਲੜੀਵਾਰ_delta,
                             ਪੂਰਨ ਵੰਡੋ_ਲਾਗਤ,
                             Writer& range_writer) {
    ਤਾਰਕਿਕ should_ਵੰਡੋ = ਝੂਠ;
    ਜੇ (ਸ਼ੁਰੂ < ਲੜੀਵਾਰ_ਸ਼ੁਰੂ && ਲੜੀਵਾਰ_ਸ਼ੁਰੂ < ਸਿਰਾ) {
      ਪੂਰਨ ਲੜੀਵਾਰ_ਲਾਗਤ = (ਸਿਰਾ - ਲੜੀਵਾਰ_ਸ਼ੁਰੂ + 1) * 2;
      ਜੇ (ਲੜੀਵਾਰ_ਲਾਗਤ >= ਵੰਡੋ_ਲਾਗਤ) {
        should_ਵੰਡੋ = ਸੱਚ;
      }
    }

    // A zero delta is also the marker for ranges that need a
    // glyphIdArray; serialize_rangeoffset_glyid() tells the two cases
    // apart by re-checking the mapping.
    ਜੇ (should_ਵੰਡੋ) {
      ਜੇ (ਸ਼ੁਰੂ == prev_ਲੜੀਵਾਰ_ਸ਼ੁਰੂ)
        range_writer (ਸ਼ੁਰੂ, ਲੜੀਵਾਰ_ਸ਼ੁਰੂ - 1, ਪਿਛਲਾ_ਲੜੀਵਾਰ_delta);
      ਨਹੀਂਤਾਂ
        range_writer (ਸ਼ੁਰੂ, ਲੜੀਵਾਰ_ਸ਼ੁਰੂ - 1, 0);
      range_writer (ਲੜੀਵਾਰ_ਸ਼ੁਰੂ, ਸਿਰਾ, ਲੜੀਵਾਰ_delta);
      ਵਾਪਸ;
    }


    ਜੇ (ਸ਼ੁਰੂ == ਲੜੀਵਾਰ_ਸ਼ੁਰੂ) {
      // Range is only a run
      range_writer (ਸ਼ੁਰੂ, ਸਿਰਾ, ਲੜੀਵਾਰ_delta);
      ਵਾਪਸ;
    }

    // Write only a single non-run range.
    range_writer (ਸ਼ੁਰੂ, ਸਿਰਾ, 0);
  }

  ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  ਬਿਨਾਨਿਸ਼ਾਨੀ serialize_ਲੱਭੋ_segcount (Iterator it) {
    ਬਣਤਰ Counter {
      ਬਿਨਾਨਿਸ਼ਾਨੀ segcount = 0;

      ਖਾਲੀ ਸੰਚਾਲਕ() (hb_codepoint_t ਸ਼ੁਰੂ,
                       hb_codepoint_t ਸਿਰਾ,
                       ਪੂਰਨ delta) {
        segcount++;
      }
    } counter;

    ਮੰਜ਼ਿਲ_ranges (+it, counter);
    ਵਾਪਸ counter.segcount;
  }


  ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  ਤਾਰਕਿਕ serialize_ਸ਼ੁਰੂ_ਸਿਰਾ_delta_arrays (hb_serialize_context_t *c,
                                         Iterator it,
                                         ਪੂਰਨ segcount)
  {
    ਬਣਤਰ Writer {
      hb_serialize_context_t *serializer_;
      HBUINT16* ਸਿਰਾ_ਕੋਡ;
      HBUINT16* ਸ਼ੁਰੂ_ਕੋਡ;
      HBINT16* id_delta_;
      ਪੂਰਨ ਸੂਚਕਾਂਕ_3;

      Writer(hb_serialize_context_t *serializer)
          : serializer_(serializer),
            ਸਿਰਾ_ਕੋਡ(ਖਾਲੀ_ਸੂਚਕ),
            ਸ਼ੁਰੂ_ਕੋਡ(ਖਾਲੀ_ਸੂਚਕ),
            id_delta_(ਖਾਲੀ_ਸੂਚਕ),
            ਸੂਚਕਾਂਕ_3 (0) {}
      ਖਾਲੀ ਸੰਚਾਲਕ() (hb_codepoint_t ਸ਼ੁਰੂ,
                       hb_codepoint_t ਸਿਰਾ,
                       ਪੂਰਨ delta) {
        ਸ਼ੁਰੂ_ਕੋਡ[ਸੂਚਕਾਂਕ_3] = ਸ਼ੁਰੂ;
        ਸਿਰਾ_ਕੋਡ[ਸੂਚਕਾਂਕ_3] = ਸਿਰਾ;
        id_delta_[ਸੂਚਕਾਂਕ_3] = delta;
        ਸੂਚਕਾਂਕ_3++;
      }
    } writer(c);

    writer.ਸਿਰਾ_ਕੋਡ = c->allocate_ਮਾਪ<HBUINT16> (HBUINT16::static_ਮਾਪ * segcount, ਝੂਠ);
    (ਖਾਲੀ) c->allocate_ਮਾਪ<HBUINT16> (2); // padding
    writer.ਸ਼ੁਰੂ_ਕੋਡ = c->allocate_ਮਾਪ<HBUINT16> (HBUINT16::static_ਮਾਪ * segcount, ਝੂਠ);
    writer.id_delta_ = c->allocate_ਮਾਪ<HBINT16> (HBINT16::static_ਮਾਪ * segcount, ਝੂਠ);

    ਜੇ (unlikely (!writer.ਸਿਰਾ_ਕੋਡ || !writer.ਸ਼ੁਰੂ_ਕੋਡ || !writer.id_delta_)) ਵਾਪਸ ਝੂਠ;

    ਮੰਜ਼ਿਲ_ranges (+it, writer);
    ਵਾਪਸ ਸੱਚ;
  }

  ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ Iterator,
          hb_requires (hb_is_iterator (Iterator))>
  HBUINT16* serialize_rangeoffset_glyid (hb_serialize_context_t *c,
                                         Iterator it,
					 HBUINT16 *ਸਿਰਾ_ਕੋਡ_2,
					 HBUINT16 *ਸ਼ੁਰੂ_ਕੋਡ_2,
					 HBINT16 *idDelta,
					 ਬਿਨਾਨਿਸ਼ਾਨੀ segcount)
  {
    hb_map_t cp_ਮੰਜ਼ਿਲ_gid { it };

    HBUINT16 *id_Range_ਵਿੱਥ = c->allocate_ਮਾਪ<HBUINT16> (HBUINT16::static_ਮਾਪ * segcount);
    ਜੇ (unlikely (!c->check_success (id_Range_ਵਿੱਥ))) ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
    ਜੇ (unlikely ((ਅੱਖਰ *)id_Range_ਵਿੱਥ - (ਅੱਖਰ *)idDelta != (ਪੂਰਨ) segcount * (ਪੂਰਨ) HBINT16::static_ਮਾਪ)) ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i : hb_range (segcount))
    {
      /* A non-zero delta is always a real run. A zero delta is
       * ambiguous: identity mapping, or the marker for a range that
       * needs a glyphIdArray. Only zero deltas need a check. */
      ਪੂਰਨ delta = idDelta[i];
      ਜੇ (delta)
	ਜਾਰੀ;
      ਤਾਰਕਿਕ use_delta = ਸੱਚ;
      ਲਈ (hb_codepoint_t cp = ਸ਼ੁਰੂ_ਕੋਡ_2[i]; cp <= ਸਿਰਾ_ਕੋਡ_2[i]; cp++)
      {
	hb_codepoint_t gid = cp_ਮੰਜ਼ਿਲ_gid.get (cp);
	ਜੇ (gid == HB_MAP_ਮੁੱਲ_INVALID)
	  gid = 0;
	ਜੇ (gid != cp)
	{
	  use_delta = ਝੂਠ;
	  ਰੋਕੋ;
	}
      }
      ਜੇ (use_delta)
	ਜਾਰੀ;

      id_Range_ਵਿੱਥ[i] = 2 * (c->ਸ਼ੁਰੂ_embed<HBUINT16> () - id_Range_ਵਿੱਥ - i);
      ਲਈ (hb_codepoint_t cp = ਸ਼ੁਰੂ_ਕੋਡ_2[i]; cp <= ਸਿਰਾ_ਕੋਡ_2[i]; cp++)
      {
        HBUINT16 gid;
        gid = cp_ਮੰਜ਼ਿਲ_gid[cp];
        c->copy<HBUINT16> (gid);
      }
    }

    ਵਾਪਸ id_Range_ਵਿੱਥ;
  }

  ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  ਖਾਲੀ serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    ਸਵੈਚਾਲਿਤ format4_iter =
    + it
    | hb_filter ([&] (ਅਚਲ hb_codepoint_pair_t _)
		 { ਵਾਪਸ _.ਪਹਿਲਾ <= 0xFFFF; })
    ;

    ਜੇ (!format4_iter) ਵਾਪਸ;

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਾਰਣੀ_initpos = c->ਲੰਮਾਈ ();
    ਜੇ (unlikely (!c->extend_min (ਇਹ))) ਵਾਪਸ;
    ਇਹ->format = 4;

    hb_ਸਦਿਸ਼_t<hb_codepoint_pair_t> cp_ਮੰਜ਼ਿਲ_gid {
      format4_iter
    };

    //serialize endCode[], startCode[], idDelta[]
    HBUINT16* ਸਿਰਾ_ਕੋਡ_2 = c->ਸ਼ੁਰੂ_embed<HBUINT16> ();
    ਬਿਨਾਨਿਸ਼ਾਨੀ segcount = serialize_ਲੱਭੋ_segcount (cp_ਮੰਜ਼ਿਲ_gid.iter());
    ਜੇ (unlikely (!serialize_ਸ਼ੁਰੂ_ਸਿਰਾ_delta_arrays (c, cp_ਮੰਜ਼ਿਲ_gid.iter(), segcount)))
      ਵਾਪਸ;

    HBUINT16 *ਸ਼ੁਰੂ_ਕੋਡ_2 = ਸਿਰਾ_ਕੋਡ_2 + segcount + 1;
    HBINT16 *idDelta = ((HBINT16*)ਸ਼ੁਰੂ_ਕੋਡ_2) + segcount;

    HBUINT16 *id_Range_ਵਿੱਥ = serialize_rangeoffset_glyid (c,
                                                           cp_ਮੰਜ਼ਿਲ_gid.iter (),
                                                           ਸਿਰਾ_ਕੋਡ_2,
                                                           ਸ਼ੁਰੂ_ਕੋਡ_2,
                                                           idDelta,
                                                           segcount);
    ਜੇ (unlikely (!c->check_success (id_Range_ਵਿੱਥ))) ਵਾਪਸ;

    ਇਹ->ਲੰਮਾਈ = c->ਲੰਮਾਈ () - ਸਾਰਣੀ_initpos;
    ਜੇ ((ਲੰਮਾ ਲੰਮਾ) ਇਹ->ਲੰਮਾਈ != (ਲੰਮਾ ਲੰਮਾ) c->ਲੰਮਾਈ () - ਸਾਰਣੀ_initpos)
    {
      c->err (HB_SERIALIZE_ERROR_INT_ਉਛਾਲ);
      ਵਾਪਸ;
    }

    ਇਹ->seg_ਗਿਣਤੀ_X2 = segcount * 2;
    ਇਹ->ਇੰਦਰਾਜ_Selector = hb_max (1u, hb_ਦੋਆਧਾਰੀ_ਅੰਕ_storage (segcount)) - 1;
    ਇਹ->ਖੋਜ_Range = 2 * (1u << ਇਹ->ਇੰਦਰਾਜ_Selector);
    ਇਹ->range_ਖਿਸਕਾਓ = segcount * 2 > ਇਹ->ਖੋਜ_Range
		       ? 2 * segcount - ਇਹ->ਖੋਜ_Range
		       : 0;
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ get_language () ਅਚਲ
  {
    ਵਾਪਸ language;
  }

  ਬਣਤਰ accelerator_t
  {
    accelerator_t () {}
    accelerator_t (ਅਚਲ CmapSubtableFormat4 *subtable) = ਮਿਟਾਓ;

    ਖਾਲੀ init (ਅਚਲ CmapSubtableFormat4 *subtable,
	       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ              subtable_ਅੰਕੜੇ_ਮਾਪ)
    {
      seg_ਗਿਣਤੀ_2 = subtable->seg_ਗਿਣਤੀ_X2 / 2;
      ਸਿਰਾ_ਗਿਣਤੀ = subtable->values.arrayZ;
      ਸ਼ੁਰੂ_ਗਿਣਤੀ = ਸਿਰਾ_ਗਿਣਤੀ + seg_ਗਿਣਤੀ_2 + 1;
      idDelta = ਸ਼ੁਰੂ_ਗਿਣਤੀ + seg_ਗਿਣਤੀ_2;
      id_Range_ਵਿੱਥ = idDelta + seg_ਗਿਣਤੀ_2;
      glyphIdArray = id_Range_ਵਿੱਥ + seg_ਗਿਣਤੀ_2;

      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਮੁੱਲਾਂ_ਵਿੱਥ = 16 + 8 * seg_ਗਿਣਤੀ_2;
      glyph_Id_Array_ਲੰਮਾਈ = subtable_ਅੰਕੜੇ_ਮਾਪ > ਮੁੱਲਾਂ_ਵਿੱਥ
			   ? (subtable_ਅੰਕੜੇ_ਮਾਪ - ਮੁੱਲਾਂ_ਵਿੱਥ) / 2
			   : 0;
    }

    ਤਾਰਕਿਕ get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) ਅਚਲ
    {
      ਬਣਤਰ CustomRange
      {
	ਪੂਰਨ cmp (hb_codepoint_t k,
		 ਬਿਨਾਨਿਸ਼ਾਨੀ distance) ਅਚਲ
	{
	  ਜੇ (k > ਆਖਰੀ) ਵਾਪਸ +1;
	  ਜੇ (k < (&ਆਖਰੀ)[distance]/*ਪਹਿਲਾ*/) ਵਾਪਸ -1;
	  ਵਾਪਸ 0;
	}
	HBUINT16 ਆਖਰੀ;
      };

      ਅਚਲ HBUINT16 *ਮਿਲਿਆ = hb_bsearch (codepoint,
					  ਇਹ->ਸਿਰਾ_ਗਿਣਤੀ,
					  ਇਹ->seg_ਗਿਣਤੀ_2,
					  ਆਕਾਰ (CustomRange),
					  hb_cmp_ਵਿਧੀ<hb_codepoint_t, CustomRange, ਬਿਨਾਨਿਸ਼ਾਨੀ>,
					  ਇਹ->seg_ਗਿਣਤੀ_2 + 1);
      ਜੇ (unlikely (!ਮਿਲਿਆ))
	ਵਾਪਸ ਝੂਠ;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = ਮਿਲਿਆ - ਸਿਰਾ_ਗਿਣਤੀ;

      hb_codepoint_t gid;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ range_ਵਿੱਥ = ਇਹ->id_Range_ਵਿੱਥ[i];
      ਜੇ (range_ਵਿੱਥ == 0)
	gid = codepoint + ਇਹ->idDelta[i];
      ਨਹੀਂਤਾਂ
      {
	/* Somebody has been smoking... */
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੂਚਕਾਂਕ_2 = range_ਵਿੱਥ / 2 + (codepoint - ਇਹ->ਸ਼ੁਰੂ_ਗਿਣਤੀ[i]) + i - ਇਹ->seg_ਗਿਣਤੀ_2;
	ਜੇ (unlikely (ਸੂਚਕਾਂਕ_2 >= ਇਹ->glyph_Id_Array_ਲੰਮਾਈ))
	  ਵਾਪਸ ਝੂਠ;
	gid = ਇਹ->glyphIdArray[ਸੂਚਕਾਂਕ_2];
	ਜੇ (unlikely (!gid))
	  ਵਾਪਸ ਝੂਠ;
	gid += ਇਹ->idDelta[i];
      }
      gid &= 0xFFFFu;
      ਜੇ (unlikely (!gid))
	ਵਾਪਸ ਝੂਠ;
      *glyph = gid;
      ਵਾਪਸ ਸੱਚ;
    }

    HB_ਅੰਦਰੂਨੀ ਸਥਿਰ ਤਾਰਕਿਕ get_glyph_func (ਅਚਲ ਖਾਲੀ *obj, hb_codepoint_t codepoint, hb_codepoint_t *glyph)
    { ਵਾਪਸ ((ਅਚਲ accelerator_t *) obj)->get_glyph (codepoint, glyph); }

    ਖਾਲੀ collect_unicodes (hb_ਸੈੱਟ_t *ਬਾਹਰਲਾ) ਅਚਲ
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = ਇਹ->seg_ਗਿਣਤੀ_2;
      ਜੇ (ਗਿਣਤੀ_2 && ਇਹ->ਸ਼ੁਰੂ_ਗਿਣਤੀ[ਗਿਣਤੀ_2 - 1] == 0xFFFFu)
	ਗਿਣਤੀ_2--; /* Skip sentinel segment. */
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
      {
	hb_codepoint_t ਸ਼ੁਰੂ = ਇਹ->ਸ਼ੁਰੂ_ਗਿਣਤੀ[i];
	hb_codepoint_t ਸਿਰਾ = ਇਹ->ਸਿਰਾ_ਗਿਣਤੀ[i];
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ range_ਵਿੱਥ = ਇਹ->id_Range_ਵਿੱਥ[i];
        ਬਾਹਰਲਾ->ਜੋੜੋ_range(ਸ਼ੁਰੂ, ਸਿਰਾ);
	ਜੇ (range_ਵਿੱਥ == 0)
	{
	  ਲਈ (hb_codepoint_t codepoint = ਸ਼ੁਰੂ; codepoint <= ਸਿਰਾ; codepoint++)
	  {
	    hb_codepoint_t gid = (codepoint + ਇਹ->idDelta[i]) & 0xFFFFu;
	    ਜੇ (unlikely (!gid))
              ਬਾਹਰਲਾ->del(codepoint);
	  }
	}
	ਨਹੀਂਤਾਂ
	{
	  ਲਈ (hb_codepoint_t codepoint = ਸ਼ੁਰੂ; codepoint <= ਸਿਰਾ; codepoint++)
	  {
	    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੂਚਕਾਂਕ_2 = range_ਵਿੱਥ / 2 + (codepoint - ਇਹ->ਸ਼ੁਰੂ_ਗਿਣਤੀ[i]) + i - ਇਹ->seg_ਗਿਣਤੀ_2;
	    ਜੇ (unlikely (ਸੂਚਕਾਂਕ_2 >= ਇਹ->glyph_Id_Array_ਲੰਮਾਈ))
            {
              ਬਾਹਰਲਾ->del_range (codepoint, ਸਿਰਾ);
	      ਰੋਕੋ;
            }
	    hb_codepoint_t gid = ਇਹ->glyphIdArray[ਸੂਚਕਾਂਕ_2];
	    ਜੇ (unlikely (!gid))
              ਬਾਹਰਲਾ->del(codepoint);
	  }
	}
      }
    }

    ਖਾਲੀ collect_mapping (hb_ਸੈੱਟ_t *unicodes, /*ਬਾਹਰਲਾ*/
			  hb_map_t *mapping /*ਬਾਹਰਲਾ*/) ਅਚਲ
    {
      // TODO(grieger): optimize similar to collect_unicodes
      // (ie. use add_range())
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = ਇਹ->seg_ਗਿਣਤੀ_2;
      ਜੇ (ਗਿਣਤੀ_2 && ਇਹ->ਸ਼ੁਰੂ_ਗਿਣਤੀ[ਗਿਣਤੀ_2 - 1] == 0xFFFFu)
	ਗਿਣਤੀ_2--; /* Skip sentinel segment. */
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2; i++)
      {
	hb_codepoint_t ਸ਼ੁਰੂ = ਇਹ->ਸ਼ੁਰੂ_ਗਿਣਤੀ[i];
	hb_codepoint_t ਸਿਰਾ = ਇਹ->ਸਿਰਾ_ਗਿਣਤੀ[i];
	ਬਿਨਾਨਿਸ਼ਾਨੀ range_ਵਿੱਥ = ਇਹ->id_Range_ਵਿੱਥ[i];
	ਜੇ (range_ਵਿੱਥ == 0)
	{
	  ਲਈ (hb_codepoint_t codepoint = ਸ਼ੁਰੂ; codepoint <= ਸਿਰਾ; codepoint++)
	  {
	    hb_codepoint_t gid = (codepoint + ਇਹ->idDelta[i]) & 0xFFFFu;
	    ਜੇ (unlikely (!gid))
	      ਜਾਰੀ;
	    unicodes->ਜੋੜੋ (codepoint);
	    mapping->ਸੈੱਟ (codepoint, gid);
	  }
	}
	ਨਹੀਂਤਾਂ
	{
	  ਲਈ (hb_codepoint_t codepoint = ਸ਼ੁਰੂ; codepoint <= ਸਿਰਾ; codepoint++)
	  {
	    ਬਿਨਾਨਿਸ਼ਾਨੀ ਸੂਚਕਾਂਕ_2 = range_ਵਿੱਥ / 2 + (codepoint - ਇਹ->ਸ਼ੁਰੂ_ਗਿਣਤੀ[i]) + i - ਇਹ->seg_ਗਿਣਤੀ_2;
	    ਜੇ (unlikely (ਸੂਚਕਾਂਕ_2 >= ਇਹ->glyph_Id_Array_ਲੰਮਾਈ))
	      ਰੋਕੋ;
	    hb_codepoint_t gid = ਇਹ->glyphIdArray[ਸੂਚਕਾਂਕ_2];
	    ਜੇ (unlikely (!gid))
	      ਜਾਰੀ;
            gid = (gid + ਇਹ->idDelta[i]) & 0xFFFFu;
	    unicodes->ਜੋੜੋ (codepoint);
	    mapping->ਸੈੱਟ (codepoint, gid);
	  }
	}
      }
    }

    ਅਚਲ HBUINT16 *ਸਿਰਾ_ਗਿਣਤੀ;
    ਅਚਲ HBUINT16 *ਸ਼ੁਰੂ_ਗਿਣਤੀ;
    ਅਚਲ HBUINT16 *idDelta;
    ਅਚਲ HBUINT16 *id_Range_ਵਿੱਥ;
    ਅਚਲ HBUINT16 *glyphIdArray;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ seg_ਗਿਣਤੀ_2;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ glyph_Id_Array_ਲੰਮਾਈ;
  };

  ਤਾਰਕਿਕ get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph,
		  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ subtable_ਅੰਕੜੇ_ਮਾਪ) ਅਚਲ
  {
    accelerator_t accel;
    accel.init (ਇਹ, subtable_ਅੰਕੜੇ_ਮਾਪ);
    ਵਾਪਸ accel.get_glyph_func (&accel, codepoint, glyph);
  }
  ਤਾਰਕਿਕ get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) ਅਚਲ
  { ਵਾਪਸ ਝੂਠ; }

  ਖਾਲੀ collect_unicodes (hb_ਸੈੱਟ_t *ਬਾਹਰਲਾ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ subtable_ਅੰਕੜੇ_ਮਾਪ) ਅਚਲ
  {
    accelerator_t accel;
    accel.init (ਇਹ, subtable_ਅੰਕੜੇ_ਮਾਪ);
    accel.collect_unicodes (ਬਾਹਰਲਾ);
  }
  ਖਾਲੀ collect_unicodes (hb_ਸੈੱਟ_t *ਬਾਹਰਲਾ) ਅਚਲ
  { collect_unicodes (ਬਾਹਰਲਾ, ਲੰਮਾਈ); }

  ਖਾਲੀ collect_mapping (hb_ਸੈੱਟ_t *unicodes, /*ਬਾਹਰਲਾ*/
			hb_map_t *mapping, /*ਬਾਹਰਲਾ*/
			ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ subtable_ਅੰਕੜੇ_ਮਾਪ) ਅਚਲ
  {
    accelerator_t accel;
    accel.init (ਇਹ, subtable_ਅੰਕੜੇ_ਮਾਪ);
    accel.collect_mapping (unicodes, mapping);
  }
  ਖਾਲੀ collect_mapping (hb_ਸੈੱਟ_t *unicodes, /*ਬਾਹਰਲਾ*/
			hb_map_t *mapping /*ਬਾਹਰਲਾ*/) ਅਚਲ
  { collect_mapping (unicodes, mapping, ਲੰਮਾਈ); }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    ਜੇ (unlikely (!c->check_struct (ਇਹ)))
      return_trace (ਝੂਠ);
    hb_barrier ();

    return_trace (c->check_range (values, 2 + 4 * seg_ਗਿਣਤੀ_X2));
  }

  ਸੁਰੱਖਿਅਤ:
  HBUINT16	format;		/* Format number is set to 4. */
  HBUINT16	ਲੰਮਾਈ;		/* This is the length in bytes of the
				 * subtable. */
  HBUINT16	language;	/* Ignore. */
  HBUINT16	seg_ਗਿਣਤੀ_X2;	/* 2 x segCount. */
  HBUINT16	ਖੋਜ_Range;	/* 2 * (2**floor(log2(segCount))) */
  HBUINT16	ਇੰਦਰਾਜ_Selector;	/* log2(searchRange/2) */
  HBUINT16	range_ਖਿਸਕਾਓ;	/* 2 x segCount - searchRange */

  Unsized_Array_ਦਾ<HBUINT16>
		values;
#ਜੇ 0
  HBUINT16	ਸਿਰਾ_ਗਿਣਤੀ[seg_ਗਿਣਤੀ_2];	/* End characterCode for each segment,
					 * last=0xFFFFu. */
  HBUINT16	reservedPad;		/* Set to 0. */
  HBUINT16	ਸ਼ੁਰੂ_ਗਿਣਤੀ[seg_ਗਿਣਤੀ_2];	/* Start character code for each segment. */
  HBINT16		idDelta[seg_ਗਿਣਤੀ_2];	/* Delta for all character codes in segment. */
  HBUINT16	id_Range_ਵਿੱਥ[seg_ਗਿਣਤੀ_2];/* Offsets into glyphIdArray or 0 */
  Unsized_Array_ਦਾ<HBUINT16>
		glyphIdArray;	/* Glyph index array (arbitrary length) */
#ਜੇ_ਅੰਤ

  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (14, values);
};

ਬਣਤਰ Cmap_Subtable_Long_ਸਮੂਹ
{
  ਮਿੱਤਰ ਬਣਤਰ CmapSubtableFormat12;
  ਮਿੱਤਰ ਬਣਤਰ CmapSubtableFormat13;
  ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ U>
  ਮਿੱਤਰ ਬਣਤਰ CmapSubtableLongSegmented;
  ਮਿੱਤਰ ਬਣਤਰ cmap;

  ਪੂਰਨ cmp (hb_codepoint_t codepoint) ਅਚਲ
  {
    ਜੇ (codepoint < ਸ਼ੁਰੂ_Char_ਕੋਡ) ਵਾਪਸ -1;
    ਜੇ (codepoint > ਸਿਰਾ_Char_ਕੋਡ)   ਵਾਪਸ +1;
    ਵਾਪਸ 0;
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ));
  }

  ਨਿਜੀ:
  HBUINT32		ਸ਼ੁਰੂ_Char_ਕੋਡ;	/* First character code in this group. */
  HBUINT32		ਸਿਰਾ_Char_ਕੋਡ;	/* Last character code in this group. */
  HBUINT32		glyphID;	/* Glyph index; interpretation depends on
					 * subtable format. */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (12);
};
DECLARE_NULL_NAMESPACE_ਬਾਈਟਾਂ (OT, Cmap_Subtable_Long_ਸਮੂਹ);

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ UINT>
ਬਣਤਰ CmapSubtableTrimmed
{
  ਤਾਰਕਿਕ get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) ਅਚਲ
  {
    /* Rely on our implicit array bound-checking. */
    hb_codepoint_t gid = glyphIdArray[codepoint - ਸ਼ੁਰੂ_Char_ਕੋਡ];
    ਜੇ (unlikely (!gid))
      ਵਾਪਸ ਝੂਠ;
    *glyph = gid;
    ਵਾਪਸ ਸੱਚ;
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ get_language () ਅਚਲ
  {
    ਵਾਪਸ language;
  }

  ਖਾਲੀ collect_unicodes (hb_ਸੈੱਟ_t *ਬਾਹਰਲਾ) ਅਚਲ
  {
    hb_codepoint_t ਸ਼ੁਰੂ = ਸ਼ੁਰੂ_Char_ਕੋਡ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = glyphIdArray.len;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
      ਜੇ (glyphIdArray[i])
	ਬਾਹਰਲਾ->ਜੋੜੋ (ਸ਼ੁਰੂ + i);
  }

  ਖਾਲੀ collect_mapping (hb_ਸੈੱਟ_t *unicodes, /*ਬਾਹਰਲਾ*/
			hb_map_t *mapping /*ਬਾਹਰਲਾ*/) ਅਚਲ
  {
    hb_codepoint_t ਸ਼ੁਰੂ_cp = ਸ਼ੁਰੂ_Char_ਕੋਡ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = glyphIdArray.len;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2; i++)
      ਜੇ (glyphIdArray[i])
      {
	hb_codepoint_t unicode = ਸ਼ੁਰੂ_cp + i;
	hb_codepoint_t glyphid = glyphIdArray[i];
	unicodes->ਜੋੜੋ (unicode);
	mapping->ਸੈੱਟ (unicode, glyphid);
      }
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) && glyphIdArray.sanitize (c));
  }

  ਸੁਰੱਖਿਅਤ:
  UINT		formatReserved;	/* Subtable format and (maybe) padding. */
  UINT		ਲੰਮਾਈ;		/* Byte length of this subtable. */
  UINT		language;	/* Ignore. */
  UINT		ਸ਼ੁਰੂ_Char_ਕੋਡ;	/* First character code covered. */
  Array_ਦਾ<HBGlyphID16, UINT>
		glyphIdArray;	/* Array of glyph index values for character
				 * codes in the range. */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (5 * ਆਕਾਰ (UINT), glyphIdArray);
};

ਬਣਤਰ CmapSubtableFormat6  : CmapSubtableTrimmed<HBUINT16> {};
ਬਣਤਰ CmapSubtableFormat10 : CmapSubtableTrimmed<HBUINT32> {};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
ਬਣਤਰ CmapSubtableLongSegmented
{
  ਮਿੱਤਰ ਬਣਤਰ cmap;

  ਤਾਰਕਿਕ get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) ਅਚਲ
  {
    hb_codepoint_t gid = T::ਸਮੂਹ_get_glyph (groups.bsearch (codepoint), codepoint);
    ਜੇ (unlikely (!gid))
      ਵਾਪਸ ਝੂਠ;
    *glyph = gid;
    ਵਾਪਸ ਸੱਚ;
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ get_language () ਅਚਲ
  {
    ਵਾਪਸ language;
  }

  ਖਾਲੀ collect_unicodes (hb_ਸੈੱਟ_t *ਬਾਹਰਲਾ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_glyphs) ਅਚਲ
  {
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਇਹ->groups.len; i++)
    {
      hb_codepoint_t ਸ਼ੁਰੂ = ਇਹ->groups[i].ਸ਼ੁਰੂ_Char_ਕੋਡ;
      hb_codepoint_t ਸਿਰਾ = hb_min ((hb_codepoint_t) ਇਹ->groups[i].ਸਿਰਾ_Char_ਕੋਡ,
				   (hb_codepoint_t) HB_UNICODE_MAX);
      hb_codepoint_t gid = ਇਹ->groups[i].glyphID;
      ਜੇ (!gid)
      {
	/* Intention is: if (hb_is_same (T, CmapSubtableFormat13)) continue; */
	ਜੇ (! T::ਸਮੂਹ_get_glyph (ਇਹ->groups[i], ਸਿਰਾ)) ਜਾਰੀ;
	ਸ਼ੁਰੂ++;
	gid++;
      }
      ਜੇ (unlikely ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) gid >= ਅੰਕ_glyphs)) ਜਾਰੀ;
      ਜੇ (unlikely ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) (gid + ਸਿਰਾ - ਸ਼ੁਰੂ) >= ਅੰਕ_glyphs))
	ਸਿਰਾ = ਸ਼ੁਰੂ + (hb_codepoint_t) ਅੰਕ_glyphs - gid;

      ਬਾਹਰਲਾ->ਜੋੜੋ_range (ਸ਼ੁਰੂ, hb_min (ਸਿਰਾ, 0x10FFFFu));
    }
  }

  ਖਾਲੀ collect_mapping (hb_ਸੈੱਟ_t *unicodes, /*ਬਾਹਰਲਾ*/
			hb_map_t *mapping, /*ਬਾਹਰਲਾ*/
			ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_glyphs) ਅਚਲ
  {
    hb_codepoint_t ਆਖਰੀ_ਸਿਰਾ = 0;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = ਇਹ->groups.len;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      hb_codepoint_t ਸ਼ੁਰੂ = ਇਹ->groups.arrayZ[i].ਸ਼ੁਰੂ_Char_ਕੋਡ;
      hb_codepoint_t ਸਿਰਾ = hb_min ((hb_codepoint_t) ਇਹ->groups.arrayZ[i].ਸਿਰਾ_Char_ਕੋਡ,
				   (hb_codepoint_t) HB_UNICODE_MAX);
      ਜੇ (unlikely (ਸ਼ੁਰੂ > ਸਿਰਾ || ਸ਼ੁਰੂ < ਆਖਰੀ_ਸਿਰਾ)) {
        // Range is not in order and is invalid, skip it.
        ਜਾਰੀ;
      }
      ਆਖਰੀ_ਸਿਰਾ = ਸਿਰਾ;


      hb_codepoint_t gid = ਇਹ->groups.arrayZ[i].glyphID;
      ਜੇ (!gid)
      {
        ਜੇ (T::format_ਸੰਖਿਆ == 13) ਜਾਰੀ;
	ਸ਼ੁਰੂ++;
	gid++;
      }
      ਜੇ (unlikely ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) gid >= ਅੰਕ_glyphs)) ਜਾਰੀ;
      ਜੇ (unlikely ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) (gid + ਸਿਰਾ - ਸ਼ੁਰੂ) >= ਅੰਕ_glyphs))
	ਸਿਰਾ = ਸ਼ੁਰੂ + (hb_codepoint_t) ਅੰਕ_glyphs - gid;

      mapping->alloc (mapping->get_population () + ਸਿਰਾ - ਸ਼ੁਰੂ + 1);

      unicodes->ਜੋੜੋ_range (ਸ਼ੁਰੂ, ਸਿਰਾ);
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ cp = ਸ਼ੁਰੂ; cp <= ਸਿਰਾ; cp++)
      {
	mapping->ਸੈੱਟ (cp, gid);
        gid += T::increment;
      }
    }
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) && groups.sanitize (c));
  }

  ਸੁਰੱਖਿਅਤ:
  HBUINT16	format;		/* Subtable format; set to 12. */
  HBUINT16	reserved;	/* Reserved; set to 0. */
  HBUINT32	ਲੰਮਾਈ;		/* Byte length of this subtable. */
  HBUINT32	language;	/* Ignore. */
  Sorted_Array32_ਦਾ<Cmap_Subtable_Long_ਸਮੂਹ>
		groups;		/* Groupings. */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (16, groups);
};

ਬਣਤਰ CmapSubtableFormat12 : CmapSubtableLongSegmented<CmapSubtableFormat12>
{
  ਸਥਿਰ constexpr ਪੂਰਨ increment = 1;
  ਸਥਿਰ constexpr ਪੂਰਨ format_ਸੰਖਿਆ = 12;

  ਸਥਿਰ hb_codepoint_t ਸਮੂਹ_get_glyph (ਅਚਲ Cmap_Subtable_Long_ਸਮੂਹ &ਸਮੂਹ,
					 hb_codepoint_t u)
  { ਵਾਪਸ likely (ਸਮੂਹ.ਸ਼ੁਰੂ_Char_ਕੋਡ <= ਸਮੂਹ.ਸਿਰਾ_Char_ਕੋਡ) ?
	   ਸਮੂਹ.glyphID + (u - ਸਮੂਹ.ਸ਼ੁਰੂ_Char_ਕੋਡ) : 0; }


  ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  ਖਾਲੀ serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    ਜੇ (!it) ਵਾਪਸ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਾਰਣੀ_initpos = c->ਲੰਮਾਈ ();
    ਜੇ (unlikely (!c->extend_min (ਇਹ))) ਵਾਪਸ;

    hb_codepoint_t ਸ਼ੁਰੂ_Char_ਕੋਡ = (hb_codepoint_t) -1, ਸਿਰਾ_Char_ਕੋਡ = (hb_codepoint_t) -1;
    hb_codepoint_t glyphID = 0;

    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& _ : +it)
    {
      ਜੇ (ਸ਼ੁਰੂ_Char_ਕੋਡ == (hb_codepoint_t) -1)
      {
	ਸ਼ੁਰੂ_Char_ਕੋਡ = _.ਪਹਿਲਾ;
	ਸਿਰਾ_Char_ਕੋਡ = _.ਪਹਿਲਾ;
	glyphID = _.second;
      }
      ਨਹੀਂਤਾਂ ਜੇ (!_is_gid_consecutive (ਸਿਰਾ_Char_ਕੋਡ, ਸ਼ੁਰੂ_Char_ਕੋਡ, glyphID, _.ਪਹਿਲਾ, _.second))
      {
	Cmap_Subtable_Long_ਸਮੂਹ  grouprecord;
	grouprecord.ਸ਼ੁਰੂ_Char_ਕੋਡ = ਸ਼ੁਰੂ_Char_ਕੋਡ;
	grouprecord.ਸਿਰਾ_Char_ਕੋਡ = ਸਿਰਾ_Char_ਕੋਡ;
	grouprecord.glyphID = glyphID;
	c->copy<Cmap_Subtable_Long_ਸਮੂਹ> (grouprecord);

	ਸ਼ੁਰੂ_Char_ਕੋਡ = _.ਪਹਿਲਾ;
	ਸਿਰਾ_Char_ਕੋਡ = _.ਪਹਿਲਾ;
	glyphID = _.second;
      }
      ਨਹੀਂਤਾਂ
	ਸਿਰਾ_Char_ਕੋਡ = _.ਪਹਿਲਾ;
    }

    Cmap_Subtable_Long_ਸਮੂਹ ਰਿਕਾਰਡ;
    ਰਿਕਾਰਡ.ਸ਼ੁਰੂ_Char_ਕੋਡ = ਸ਼ੁਰੂ_Char_ਕੋਡ;
    ਰਿਕਾਰਡ.ਸਿਰਾ_Char_ਕੋਡ = ਸਿਰਾ_Char_ਕੋਡ;
    ਰਿਕਾਰਡ.glyphID = glyphID;
    c->copy<Cmap_Subtable_Long_ਸਮੂਹ> (ਰਿਕਾਰਡ);

    ਇਹ->format = 12;
    ਇਹ->reserved = 0;
    ਇਹ->ਲੰਮਾਈ = c->ਲੰਮਾਈ () - ਸਾਰਣੀ_initpos;
    ਇਹ->groups.len = (ਇਹ->ਲੰਮਾਈ - min_ਮਾਪ) / Cmap_Subtable_Long_ਸਮੂਹ::static_ਮਾਪ;
  }

  ਸਥਿਰ size_t get_sub_ਸਾਰਣੀ_ਮਾਪ (ਅਚਲ hb_sorted_ਸਦਿਸ਼_t<Cmap_Subtable_Long_ਸਮੂਹ> &groups_ਅੰਕੜੇ)
  { ਵਾਪਸ 16 + 12 * groups_ਅੰਕੜੇ.ਲੰਮਾਈ; }

  ਨਿਜੀ:
  ਸਥਿਰ ਤਾਰਕਿਕ _is_gid_consecutive (hb_codepoint_t ਸਿਰਾ_Char_ਕੋਡ,
				   hb_codepoint_t ਸ਼ੁਰੂ_Char_ਕੋਡ,
				   hb_codepoint_t glyphID,
				   hb_codepoint_t cp,
				   hb_codepoint_t new_gid)
  {
    ਵਾਪਸ (cp - 1 == ਸਿਰਾ_Char_ਕੋਡ) &&
	new_gid == glyphID + (cp - ਸ਼ੁਰੂ_Char_ਕੋਡ);
  }

};

ਬਣਤਰ CmapSubtableFormat13 : CmapSubtableLongSegmented<CmapSubtableFormat13>
{
  ਸਥਿਰ constexpr ਪੂਰਨ increment = 0;
  ਸਥਿਰ constexpr ਪੂਰਨ format_ਸੰਖਿਆ = 13;

  ਸਥਿਰ hb_codepoint_t ਸਮੂਹ_get_glyph (ਅਚਲ Cmap_Subtable_Long_ਸਮੂਹ &ਸਮੂਹ,
					 hb_codepoint_t u HB_ਅਣਵਰਤਿਆ)
  { ਵਾਪਸ ਸਮੂਹ.glyphID; }
};

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਗਿਣਤੀ
{
  GLYPH_VARIANT_NOT_ਮਿਲਿਆ = 0,
  GLYPH_VARIANT_ਮਿਲਿਆ = 1,
  GLYPH_VARIANT_USE_DEFAULT = 2
} glyph_variant_t;

ਬਣਤਰ Unicode_ਮੁੱਲ_Range
{
  ਪੂਰਨ cmp (ਅਚਲ hb_codepoint_t &codepoint) ਅਚਲ
  {
    ਜੇ (codepoint < ਸ਼ੁਰੂ_Unicode_ਮੁੱਲ) ਵਾਪਸ -1;
    ਜੇ (codepoint > ਸ਼ੁਰੂ_Unicode_ਮੁੱਲ + additional_ਗਿਣਤੀ) ਵਾਪਸ +1;
    ਵਾਪਸ 0;
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ));
  }

  HBUINT24	ਸ਼ੁਰੂ_Unicode_ਮੁੱਲ;	/* First value in this range. */
  HBUINT8	additional_ਗਿਣਤੀ;	/* Number of additional values in this
					 * range. */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (4);
};

ਬਣਤਰ DefaultUVS : Sorted_Array32_ਦਾ<Unicode_ਮੁੱਲ_Range>
{
  ਖਾਲੀ collect_unicodes (hb_ਸੈੱਟ_t *ਬਾਹਰਲਾ) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = len;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      hb_codepoint_t ਪਹਿਲਾ = arrayZ[i].ਸ਼ੁਰੂ_Unicode_ਮੁੱਲ;
      hb_codepoint_t ਆਖਰੀ = hb_min ((hb_codepoint_t) (ਪਹਿਲਾ + arrayZ[i].additional_ਗਿਣਤੀ),
				    (hb_codepoint_t) HB_UNICODE_MAX);
      ਬਾਹਰਲਾ->ਜੋੜੋ_range (ਪਹਿਲਾ, ਆਖਰੀ);
    }
  }

  DefaultUVS* copy (hb_serialize_context_t *c,
		    ਅਚਲ hb_ਸੈੱਟ_t *unicodes) ਅਚਲ
  {
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->ਸ਼ੁਰੂ_embed<DefaultUVS> ();
    ਸਵੈਚਾਲਿਤ snap = c->snapshot ();

    HBUINT32 len;
    len = 0;
    ਜੇ (unlikely (!c->copy<HBUINT32> (len))) ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ init_len = c->ਲੰਮਾਈ ();

    ਜੇ (ਇਹ->len > unicodes->get_population () * hb_ਦੋਆਧਾਰੀ_ਅੰਕ_storage ((ਬਿਨਾਨਿਸ਼ਾਨੀ) ਇਹ->len))
    {
      hb_codepoint_t ਸ਼ੁਰੂ = HB_ਸੈੱਟ_ਮੁੱਲ_INVALID;
      hb_codepoint_t ਸਿਰਾ = HB_ਸੈੱਟ_ਮੁੱਲ_INVALID;

      ਲਈ (ਸਵੈਚਾਲਿਤ u : *unicodes)
      {
        ਜੇ (!as_array ().bsearch (u))
	  ਜਾਰੀ;
	ਜੇ (ਸ਼ੁਰੂ == HB_ਸੈੱਟ_ਮੁੱਲ_INVALID)
	{
	  ਸ਼ੁਰੂ = u;
	  ਸਿਰਾ = ਸ਼ੁਰੂ - 1;
	}
	ਜੇ (ਸਿਰਾ + 1 != u || ਸਿਰਾ - ਸ਼ੁਰੂ == 255)
        {
	  Unicode_ਮੁੱਲ_Range rec;
	  rec.ਸ਼ੁਰੂ_Unicode_ਮੁੱਲ = ਸ਼ੁਰੂ;
	  rec.additional_ਗਿਣਤੀ = ਸਿਰਾ - ਸ਼ੁਰੂ;
	  c->copy<Unicode_ਮੁੱਲ_Range> (rec);
	  ਸ਼ੁਰੂ = u;
	}
	ਸਿਰਾ = u;
      }
      ਜੇ (ਸ਼ੁਰੂ != HB_ਸੈੱਟ_ਮੁੱਲ_INVALID)
      {
	Unicode_ਮੁੱਲ_Range rec;
	rec.ਸ਼ੁਰੂ_Unicode_ਮੁੱਲ = ਸ਼ੁਰੂ;
	rec.additional_ਗਿਣਤੀ = ਸਿਰਾ - ਸ਼ੁਰੂ;
	c->copy<Unicode_ਮੁੱਲ_Range> (rec);
      }

    }
    ਨਹੀਂਤਾਂ
    {
      hb_codepoint_t ਆਖਰੀ_ਕੋਡ = HB_ਸੈੱਟ_ਮੁੱਲ_INVALID;
      ਪੂਰਨ ਗਿਣਤੀ_2 = -1;

      ਲਈ (ਅਚਲ Unicode_ਮੁੱਲ_Range& _ : *ਇਹ)
      {
	hb_codepoint_t cur_ਇੰਦਰਾਜ = (hb_codepoint_t) (_.ਸ਼ੁਰੂ_Unicode_ਮੁੱਲ - 1);
	hb_codepoint_t ਸਿਰਾ = cur_ਇੰਦਰਾਜ + _.additional_ਗਿਣਤੀ + 2;

	ਲਈ (; unicodes->next (&cur_ਇੰਦਰਾਜ) && cur_ਇੰਦਰਾਜ < ਸਿਰਾ;)
	{
	  ਗਿਣਤੀ_2 += 1;
	  ਜੇ (ਆਖਰੀ_ਕੋਡ == HB_ਸੈੱਟ_ਮੁੱਲ_INVALID)
	    ਆਖਰੀ_ਕੋਡ = cur_ਇੰਦਰਾਜ;
	  ਨਹੀਂਤਾਂ ਜੇ (ਆਖਰੀ_ਕੋਡ + ਗਿਣਤੀ_2 != cur_ਇੰਦਰਾਜ || ਗਿਣਤੀ_2 > 255)
	  {
	    Unicode_ਮੁੱਲ_Range rec;
	    rec.ਸ਼ੁਰੂ_Unicode_ਮੁੱਲ = ਆਖਰੀ_ਕੋਡ;
	    rec.additional_ਗਿਣਤੀ = ਗਿਣਤੀ_2 - 1;
	    c->copy<Unicode_ਮੁੱਲ_Range> (rec);

	    ਆਖਰੀ_ਕੋਡ = cur_ਇੰਦਰਾਜ;
	    ਗਿਣਤੀ_2 = 0;
	  }
	}
      }

      ਜੇ (ਆਖਰੀ_ਕੋਡ != HB_MAP_ਮੁੱਲ_INVALID)
      {
	Unicode_ਮੁੱਲ_Range rec;
	rec.ਸ਼ੁਰੂ_Unicode_ਮੁੱਲ = ਆਖਰੀ_ਕੋਡ;
	rec.additional_ਗਿਣਤੀ = ਗਿਣਤੀ_2;
	c->copy<Unicode_ਮੁੱਲ_Range> (rec);
      }
    }

    ਜੇ (c->ਲੰਮਾਈ () - init_len == 0)
    {
      c->revert (snap);
      ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
    }
    ਨਹੀਂਤਾਂ
    {
      ਜੇ (unlikely (!c->check_ਸੌਂਪੋ (ਬਾਹਰਲਾ->len,
                                      (c->ਲੰਮਾਈ () - init_len) / Unicode_ਮੁੱਲ_Range::static_ਮਾਪ,
                                      HB_SERIALIZE_ERROR_INT_ਉਛਾਲ))) ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
      ਵਾਪਸ ਬਾਹਰਲਾ;
    }
  }

  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (4, *ਇਹ);
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types>
ਬਣਤਰ UVSMapping
{
  ਵਰਤੋਂ GlyphID = ਕਿਸਮ_ਨਾਂ Types::HBGlyphID;

  ਪੂਰਨ cmp (ਅਚਲ hb_codepoint_t &codepoint) ਅਚਲ
  { ਵਾਪਸ unicode_ਮੁੱਲ.cmp (codepoint); }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ));
  }

  HBUINT24	unicode_ਮੁੱਲ;	/* Base Unicode value of the UVS */
  GlyphID
		glyphID;	/* Glyph ID of the UVS. */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (3 + GlyphID::static_ਮਾਪ);
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types>
ਬਣਤਰ NonDefaultUVS : Sorted_Array32_ਦਾ<UVSMapping<Types>>
{
  ਵਰਤੋਂ Mapping = UVSMapping<Types>;

  ਖਾਲੀ collect_unicodes (hb_ਸੈੱਟ_t *ਬਾਹਰਲਾ) ਅਚਲ
  {
    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& a : ਇਹ->as_array ())
      ਬਾਹਰਲਾ->ਜੋੜੋ (a.unicode_ਮੁੱਲ);
  }

  ਖਾਲੀ collect_mapping (hb_ਸੈੱਟ_t *unicodes, /*ਬਾਹਰਲਾ*/
			hb_map_t *mapping /*ਬਾਹਰਲਾ*/) ਅਚਲ
  {
    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& a : ਇਹ->as_array ())
    {
      hb_codepoint_t unicode = a.unicode_ਮੁੱਲ;
      hb_codepoint_t glyphid = a.glyphID;
      unicodes->ਜੋੜੋ (unicode);
      mapping->ਸੈੱਟ (unicode, glyphid);
    }
  }

  ਖਾਲੀ ਘੇਰਾ_glyphs (ਅਚਲ hb_ਸੈੱਟ_t      *unicodes,
		       hb_ਸੈੱਟ_t            *glyphset) ਅਚਲ
  {
    + ਇਹ->as_array ()
    | hb_filter (unicodes, &Mapping::unicode_ਮੁੱਲ)
    | hb_map (&Mapping::glyphID)
    | hb_sink (glyphset)
    ;
  }

  NonDefaultUVS* copy (hb_serialize_context_t *c,
		       ਅਚਲ hb_ਸੈੱਟ_t *unicodes,
		       ਅਚਲ hb_ਸੈੱਟ_t *glyphs_requested,
		       ਅਚਲ hb_map_t *glyph_map) ਅਚਲ
  {
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->ਸ਼ੁਰੂ_embed<NonDefaultUVS> ();
    ਸਵੈਚਾਲਿਤ it =
    + ਇਹ->as_array ()
    | hb_filter ([&] (ਅਚਲ Mapping& _)
		 {
		   ਵਾਪਸ unicodes->has (_.unicode_ਮੁੱਲ) || glyphs_requested->has (_.glyphID);
		 })
    ;

    ਜੇ (!it) ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

    HBUINT32 len;
    len = it.len ();
    ਜੇ (unlikely (!c->copy<HBUINT32> (len))) ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

    ਲਈ (ਅਚਲ Mapping& _ : it)
    {
      Mapping mapping;
      mapping.unicode_ਮੁੱਲ = _.unicode_ਮੁੱਲ;
      mapping.glyphID = glyph_map->get (_.glyphID);
      c->copy<Mapping> (mapping);
    }

    ਵਾਪਸ ਬਾਹਰਲਾ;
  }

  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (4, *ਇਹ);
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types>
ਬਣਤਰ Variation_Selector_ਰਿਕਾਰਡ
{
  glyph_variant_t get_glyph (hb_codepoint_t codepoint,
			     hb_codepoint_t *glyph,
			     ਅਚਲ ਖਾਲੀ *ਅਧਾਰ) ਅਚਲ
  {
    ਜੇ ((ਅਧਾਰ+defaultUVS).bfind (codepoint))
      ਵਾਪਸ GLYPH_VARIANT_USE_DEFAULT;
    ਅਚਲ UVSMapping<Types> &nonDefault = (ਅਧਾਰ+nonDefaultUVS).bsearch (codepoint);
    ਜੇ (nonDefault.glyphID)
    {
      *glyph = nonDefault.glyphID;
       ਵਾਪਸ GLYPH_VARIANT_ਮਿਲਿਆ;
    }
    ਵਾਪਸ GLYPH_VARIANT_NOT_ਮਿਲਿਆ;
  }

  Variation_Selector_ਰਿਕਾਰਡ(ਅਚਲ Variation_Selector_ਰਿਕਾਰਡ& other)
  {
    *ਇਹ = other;
  }

  ਖਾਲੀ ਸੰਚਾਲਕ= (ਅਚਲ Variation_Selector_ਰਿਕਾਰਡ& other)
  {
    varSelector = other.varSelector;
    HBUINT32 ਵਿੱਥ = other.defaultUVS;
    defaultUVS = ਵਿੱਥ;
    ਵਿੱਥ = other.nonDefaultUVS;
    nonDefaultUVS = ਵਿੱਥ;
  }

  ਖਾਲੀ collect_unicodes (hb_ਸੈੱਟ_t *ਬਾਹਰਲਾ, ਅਚਲ ਖਾਲੀ *ਅਧਾਰ) ਅਚਲ
  {
    (ਅਧਾਰ+defaultUVS).collect_unicodes (ਬਾਹਰਲਾ);
    (ਅਧਾਰ+nonDefaultUVS).collect_unicodes (ਬਾਹਰਲਾ);
  }

  ਖਾਲੀ collect_mapping (ਅਚਲ ਖਾਲੀ *ਅਧਾਰ,
			hb_ਸੈੱਟ_t *unicodes, /*ਬਾਹਰਲਾ*/
			hb_map_t *mapping /*ਬਾਹਰਲਾ*/) ਅਚਲ
  {
    (ਅਧਾਰ+defaultUVS).collect_unicodes (unicodes);
    (ਅਧਾਰ+nonDefaultUVS).collect_mapping (unicodes, mapping);
  }

  ਪੂਰਨ cmp (ਅਚਲ hb_codepoint_t &variation_selector) ਅਚਲ
  { ਵਾਪਸ varSelector.cmp (variation_selector); }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c, ਅਚਲ ਖਾਲੀ *ਅਧਾਰ) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) &&
		  defaultUVS.sanitize (c, ਅਧਾਰ) &&
		  nonDefaultUVS.sanitize (c, ਅਧਾਰ));
  }

  hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਬਿਨਾਨਿਸ਼ਾਨੀ>
  copy (hb_serialize_context_t *c,
	ਅਚਲ hb_ਸੈੱਟ_t *unicodes,
	ਅਚਲ hb_ਸੈੱਟ_t *glyphs_requested,
	ਅਚਲ hb_map_t *glyph_map,
	ਅਚਲ ਖਾਲੀ *ਅਧਾਰ) ਅਚਲ
  {
    ਸਵੈਚਾਲਿਤ snap = c->snapshot ();
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->embed<Variation_Selector_ਰਿਕਾਰਡ> (*ਇਹ);
    ਜੇ (unlikely (!ਬਾਹਰਲਾ)) ਵਾਪਸ hb_pair (0, 0);

    ਬਾਹਰਲਾ->defaultUVS = 0;
    ਬਾਹਰਲਾ->nonDefaultUVS = 0;

    ਬਿਨਾਨਿਸ਼ਾਨੀ non_default_uvs_objidx = 0;
    ਜੇ (nonDefaultUVS != 0)
    {
      c->push ();
      ਜੇ (c->copy (ਅਧਾਰ+nonDefaultUVS, unicodes, glyphs_requested, glyph_map))
	non_default_uvs_objidx = c->ਕੱਢੋ_pack ();
      ਨਹੀਂਤਾਂ c->ਕੱਢੋ_discard ();
    }

    ਬਿਨਾਨਿਸ਼ਾਨੀ default_uvs_objidx = 0;
    ਜੇ (defaultUVS != 0)
    {
      c->push ();
      ਜੇ (c->copy (ਅਧਾਰ+defaultUVS, unicodes))
	default_uvs_objidx = c->ਕੱਢੋ_pack ();
      ਨਹੀਂਤਾਂ c->ਕੱਢੋ_discard ();
    }


    ਜੇ (!default_uvs_objidx && !non_default_uvs_objidx)
      c->revert (snap);

    ਵਾਪਸ hb_pair (default_uvs_objidx, non_default_uvs_objidx);
  }

  HBUINT24	varSelector;	/* Variation selector. */
  Offset32_ਮੰਜ਼ਿਲ<DefaultUVS>
		defaultUVS;	/* Offset to Default UVS Table.  May be 0. */
  Offset32_ਮੰਜ਼ਿਲ<NonDefaultUVS<Types>>
		nonDefaultUVS;	/* Offset to Non-Default UVS Table.  May be 0. */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (11);
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types>
ਬਣਤਰ CmapSubtableFormat14_15
{
  ਸਥਿਰ constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ format_ਮੁੱਲ = Types::ਮਾਪ == 2 ? 14 : 15;
  ਵਰਤੋਂ ਰਿਕਾਰਡ_2 = Variation_Selector_ਰਿਕਾਰਡ<Types>;
  ਵਰਤੋਂ NonDefault = NonDefaultUVS<Types>;

  glyph_variant_t get_glyph_variant (hb_codepoint_t codepoint,
				     hb_codepoint_t variation_selector,
				     hb_codepoint_t *glyph) ਅਚਲ
  { ਵਾਪਸ ਰਿਕਾਰਡ.bsearch (variation_selector).get_glyph (codepoint, glyph, ਇਹ); }

  ਖਾਲੀ collect_variation_selectors (hb_ਸੈੱਟ_t *ਬਾਹਰਲਾ) ਅਚਲ
  {
    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& a : ਰਿਕਾਰਡ.as_array ())
      ਬਾਹਰਲਾ->ਜੋੜੋ (a.varSelector);
  }
  ਖਾਲੀ collect_variation_unicodes (hb_codepoint_t variation_selector,
				   hb_ਸੈੱਟ_t *ਬਾਹਰਲਾ) ਅਚਲ
  { ਰਿਕਾਰਡ.bsearch (variation_selector).collect_unicodes (ਬਾਹਰਲਾ, ਇਹ); }

  ਖਾਲੀ serialize (hb_serialize_context_t *c,
		  ਅਚਲ hb_ਸੈੱਟ_t *unicodes,
		  ਅਚਲ hb_ਸੈੱਟ_t *glyphs_requested,
		  ਅਚਲ hb_map_t *glyph_map,
		  ਅਚਲ ਖਾਲੀ *ਅਧਾਰ)
  {
    ਸਵੈਚਾਲਿਤ snap = c->snapshot ();
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਾਰਣੀ_initpos = c->ਲੰਮਾਈ ();
    ਅਚਲ ਅੱਖਰ* init_ਪੂਛ = c->ਪੂਛ;

    ਜੇ (unlikely (!c->extend_min (ਇਹ))) ਵਾਪਸ;
    ਇਹ->format = format_ਮੁੱਲ;

    ਸਵੈਚਾਲਿਤ src_tbl = reinterpret_cast<ਅਚਲ CmapSubtableFormat14_15*> (ਅਧਾਰ);

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
    hb_ਸਦਿਸ਼_t<hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਬਿਨਾਨਿਸ਼ਾਨੀ>> obj_indices;
    ਲਈ (ਪੂਰਨ i = src_tbl->ਰਿਕਾਰਡ.len - 1; i >= 0; i--)
    {
      ਜੇ (!unicodes->has(src_tbl->ਰਿਕਾਰਡ[i].varSelector))
        ਜਾਰੀ;

      hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਬਿਨਾਨਿਸ਼ਾਨੀ> ਨਤੀਜਾ = src_tbl->ਰਿਕਾਰਡ[i].copy (c, unicodes, glyphs_requested, glyph_map, ਅਧਾਰ);
      ਜੇ (ਨਤੀਜਾ.ਪਹਿਲਾ || ਨਤੀਜਾ.second)
	obj_indices.push (ਨਤੀਜਾ);
    }

    ਜੇ (c->ਲੰਮਾਈ () - ਸਾਰਣੀ_initpos == CmapSubtableFormat14_15::min_ਮਾਪ)
    {
      c->revert (snap);
      ਵਾਪਸ;
    }

    ਜੇ (unlikely (!c->check_success (!obj_indices.ਵਿੱਚ_error ())))
      ਵਾਪਸ;

    ਪੂਰਨ ਪੂਛ_len = init_ਪੂਛ - c->ਪੂਛ;
    c->check_ਸੌਂਪੋ (ਇਹ->ਲੰਮਾਈ, c->ਲੰਮਾਈ () - ਸਾਰਣੀ_initpos + ਪੂਛ_len,
                     HB_SERIALIZE_ERROR_INT_ਉਛਾਲ);
    c->check_ਸੌਂਪੋ (ਇਹ->ਰਿਕਾਰਡ.len,
		     (c->ਲੰਮਾਈ () - ਸਾਰਣੀ_initpos - CmapSubtableFormat14_15::min_ਮਾਪ) /
		     ਰਿਕਾਰਡ_2::static_ਮਾਪ,
                     HB_SERIALIZE_ERROR_INT_ਉਛਾਲ);

    /* Correct the incorrect write order by reversing the order of the variation
       records array. */
    ਉਲਟ_variation_ਰਿਕਾਰਡਾਂ ();

    /* Now that records are in the right order, we can set up the offsets. */
    ਜੋੜੋ_links_ਮੰਜ਼ਿਲ_variation_ਰਿਕਾਰਡਾਂ (c, obj_indices);
  }

  ਖਾਲੀ ਉਲਟ_variation_ਰਿਕਾਰਡਾਂ ()
  {
    ਰਿਕਾਰਡ.as_array ().reverse ();
  }

  ਖਾਲੀ ਜੋੜੋ_links_ਮੰਜ਼ਿਲ_variation_ਰਿਕਾਰਡਾਂ (hb_serialize_context_t *c,
					ਅਚਲ hb_ਸਦਿਸ਼_t<hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਬਿਨਾਨਿਸ਼ਾਨੀ>>& obj_indices)
  {
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < obj_indices.ਲੰਮਾਈ; i++)
    {
      /*
       * Since the record array has been reversed (see comments in copy())
       * but obj_indices has not been, the indices at obj_indices[i]
       * are for the variation record at record[j].
       */
      ਪੂਰਨ j = obj_indices.ਲੰਮਾਈ - 1 - i;
      c->ਜੋੜੋ_link (ਰਿਕਾਰਡ[j].defaultUVS, obj_indices[i].ਪਹਿਲਾ);
      c->ਜੋੜੋ_link (ਰਿਕਾਰਡ[j].nonDefaultUVS, obj_indices[i].second);
    }
  }

  ਖਾਲੀ ਘੇਰਾ_glyphs (ਅਚਲ hb_ਸੈੱਟ_t      *unicodes,
		       hb_ਸੈੱਟ_t            *glyphset) ਅਚਲ
  {
    + hb_iter (ਰਿਕਾਰਡ)
    | hb_filter (hb_bool, &ਰਿਕਾਰਡ_2::nonDefaultUVS)
    | hb_filter (unicodes, &ਰਿਕਾਰਡ_2::varSelector)
    | hb_map (&ਰਿਕਾਰਡ_2::nonDefaultUVS)
    | hb_map (hb_ਜੋੜੋ (ਇਹ))
    | hb_apply ([=] (ਅਚਲ NonDefault& _) { _.ਘੇਰਾ_glyphs (unicodes, glyphset); })
    ;
  }

  ਖਾਲੀ collect_unicodes (hb_ਸੈੱਟ_t *ਬਾਹਰਲਾ) ਅਚਲ
  {
    ਲਈ (ਅਚਲ ਰਿਕਾਰਡ_2& _ : ਰਿਕਾਰਡ)
      _.collect_unicodes (ਬਾਹਰਲਾ, ਇਹ);
  }

  ਖਾਲੀ collect_mapping (hb_ਸੈੱਟ_t *unicodes, /*ਬਾਹਰਲਾ*/
			hb_map_t *mapping /*ਬਾਹਰਲਾ*/) ਅਚਲ
  {
    ਲਈ (ਅਚਲ ਰਿਕਾਰਡ_2& _ : ਰਿਕਾਰਡ)
      _.collect_mapping (ਇਹ, unicodes, mapping);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) &&
		  ਰਿਕਾਰਡ.sanitize (c, ਇਹ));
  }

  ਸੁਰੱਖਿਅਤ:
  HBUINT16	format;		/* Format number is set to 14 or 15. */
  HBUINT32	ਲੰਮਾਈ;		/* Byte length of this subtable. */
  Sorted_Array32_ਦਾ<ਰਿਕਾਰਡ_2>
		ਰਿਕਾਰਡ;		/* Variation selector records; sorted
				 * in increasing order of `varSelector'. */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (10, ਰਿਕਾਰਡ);
};

ਬਣਤਰ CmapSubtable
{
  /* Note: We intentionally do NOT implement subtable formats 2 and 8. */

  ਤਾਰਕਿਕ is_variation_selector () ਅਚਲ
  {
    ਚੋਣ (u.format.v) {
    ਹਾਲਤ 14:
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਹਾਲਤ 15:
#ਜੇ_ਅੰਤ
      ਵਾਪਸ ਸੱਚ;
    ਮੂਲ:
      ਵਾਪਸ ਝੂਠ;
    }
  }

  glyph_variant_t get_glyph_variant (hb_codepoint_t codepoint,
				     hb_codepoint_t variation_selector,
				     hb_codepoint_t *glyph) ਅਚਲ
  {
    ਚੋਣ (u.format.v) {
    ਹਾਲਤ 14: hb_barrier (); ਵਾਪਸ u.format14.get_glyph_variant (codepoint, variation_selector, glyph);
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਹਾਲਤ 15: hb_barrier (); ਵਾਪਸ u.format15.get_glyph_variant (codepoint, variation_selector, glyph);
#ਜੇ_ਅੰਤ
    ਮੂਲ: ਵਾਪਸ GLYPH_VARIANT_NOT_ਮਿਲਿਆ;
    }
  }

  ਖਾਲੀ collect_variation_selectors (hb_ਸੈੱਟ_t *ਬਾਹਰਲਾ) ਅਚਲ
  {
    ਚੋਣ (u.format.v) {
    ਹਾਲਤ 14: hb_barrier (); u.format14.collect_variation_selectors (ਬਾਹਰਲਾ); ਵਾਪਸ;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਹਾਲਤ 15: hb_barrier (); u.format15.collect_variation_selectors (ਬਾਹਰਲਾ); ਵਾਪਸ;
#ਜੇ_ਅੰਤ
    ਮੂਲ: ਵਾਪਸ;
    }
  }

  ਖਾਲੀ collect_variation_unicodes (hb_codepoint_t variation_selector,
				    hb_ਸੈੱਟ_t *ਬਾਹਰਲਾ) ਅਚਲ
  {
    ਚੋਣ (u.format.v) {
    ਹਾਲਤ 14: hb_barrier (); u.format14.collect_variation_unicodes (variation_selector, ਬਾਹਰਲਾ); ਵਾਪਸ;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਹਾਲਤ 15: hb_barrier (); u.format15.collect_variation_unicodes (variation_selector, ਬਾਹਰਲਾ); ਵਾਪਸ;
#ਜੇ_ਅੰਤ
    ਮੂਲ: ਵਾਪਸ;
    }
  }

  ਖਾਲੀ ਘੇਰਾ_glyphs (ਅਚਲ hb_ਸੈੱਟ_t *unicodes, hb_ਸੈੱਟ_t *glyphset) ਅਚਲ
  {
    ਚੋਣ (u.format.v) {
    ਹਾਲਤ 14: hb_barrier (); u.format14.ਘੇਰਾ_glyphs (unicodes, glyphset); ਵਾਪਸ;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਹਾਲਤ 15: hb_barrier (); u.format15.ਘੇਰਾ_glyphs (unicodes, glyphset); ਵਾਪਸ;
#ਜੇ_ਅੰਤ
    ਮੂਲ: ਵਾਪਸ;
    }
  }

  ਤਾਰਕਿਕ get_glyph (hb_codepoint_t codepoint,
		  hb_codepoint_t *glyph,
		  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ subtable_ਅੰਕੜੇ_ਮਾਪ = 0) ਅਚਲ
  {
    ਚੋਣ (u.format.v) {
    ਹਾਲਤ  0: hb_barrier (); ਵਾਪਸ u.format0 .get_glyph (codepoint, glyph);
    ਹਾਲਤ  4: hb_barrier (); ਵਾਪਸ subtable_ਅੰਕੜੇ_ਮਾਪ
				      ? u.format4.get_glyph (codepoint, glyph, subtable_ਅੰਕੜੇ_ਮਾਪ)
				      : ਝੂਠ;
    ਹਾਲਤ  6: hb_barrier (); ਵਾਪਸ u.format6 .get_glyph (codepoint, glyph);
    ਹਾਲਤ 10: hb_barrier (); ਵਾਪਸ u.format10.get_glyph (codepoint, glyph);
    ਹਾਲਤ 12: hb_barrier (); ਵਾਪਸ u.format12.get_glyph (codepoint, glyph);
    ਹਾਲਤ 13: hb_barrier (); ਵਾਪਸ u.format13.get_glyph (codepoint, glyph);
    ਹਾਲਤ 14:
    ਮੂਲ: ਵਾਪਸ ਝੂਠ;
    }
  }
  ਖਾਲੀ collect_unicodes (hb_ਸੈੱਟ_t *ਬਾਹਰਲਾ,
			 ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_glyphs = UINT_MAX,
			 ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ subtable_ਅੰਕੜੇ_ਮਾਪ = 0) ਅਚਲ
  {
    ਚੋਣ (u.format.v) {
    ਹਾਲਤ  0: hb_barrier (); u.format0 .collect_unicodes (ਬਾਹਰਲਾ); ਵਾਪਸ;
    ਹਾਲਤ  4: hb_barrier (); ਜੇ (subtable_ਅੰਕੜੇ_ਮਾਪ)
			      u.format4.collect_unicodes (ਬਾਹਰਲਾ, subtable_ਅੰਕੜੇ_ਮਾਪ);
			    ਨਹੀਂਤਾਂ
			      u.format4.collect_unicodes (ਬਾਹਰਲਾ);
			    ਵਾਪਸ;
    ਹਾਲਤ  6: hb_barrier (); u.format6 .collect_unicodes (ਬਾਹਰਲਾ); ਵਾਪਸ;
    ਹਾਲਤ 10: hb_barrier (); u.format10.collect_unicodes (ਬਾਹਰਲਾ); ਵਾਪਸ;
    ਹਾਲਤ 12: hb_barrier (); u.format12.collect_unicodes (ਬਾਹਰਲਾ, ਅੰਕ_glyphs); ਵਾਪਸ;
    ਹਾਲਤ 13: hb_barrier (); u.format13.collect_unicodes (ਬਾਹਰਲਾ, ਅੰਕ_glyphs); ਵਾਪਸ;
    ਹਾਲਤ 14:
    ਮੂਲ: ਵਾਪਸ;
    }
  }

  ਖਾਲੀ collect_mapping (hb_ਸੈੱਟ_t *unicodes, /*ਬਾਹਰਲਾ*/
			hb_map_t *mapping, /*ਬਾਹਰਲਾ*/
			ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_glyphs = UINT_MAX,
			ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ subtable_ਅੰਕੜੇ_ਮਾਪ = 0) ਅਚਲ
  {
    ਚੋਣ (u.format.v) {
    ਹਾਲਤ  0: hb_barrier (); u.format0 .collect_mapping (unicodes, mapping); ਵਾਪਸ;
    ਹਾਲਤ  4: hb_barrier (); ਜੇ (subtable_ਅੰਕੜੇ_ਮਾਪ)
			      u.format4.collect_mapping (unicodes, mapping, subtable_ਅੰਕੜੇ_ਮਾਪ);
			    ਨਹੀਂਤਾਂ
			      u.format4.collect_mapping (unicodes, mapping);
			    ਵਾਪਸ;
    ਹਾਲਤ  6: hb_barrier (); u.format6 .collect_mapping (unicodes, mapping); ਵਾਪਸ;
    ਹਾਲਤ 10: hb_barrier (); u.format10.collect_mapping (unicodes, mapping); ਵਾਪਸ;
    ਹਾਲਤ 12: hb_barrier (); u.format12.collect_mapping (unicodes, mapping, ਅੰਕ_glyphs); ਵਾਪਸ;
    ਹਾਲਤ 13: hb_barrier (); u.format13.collect_mapping (unicodes, mapping, ਅੰਕ_glyphs); ਵਾਪਸ;
    ਹਾਲਤ 14:
    ਮੂਲ: ਵਾਪਸ;
    }
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ get_language () ਅਚਲ
  {
    ਚੋਣ (u.format.v) {
    ਹਾਲਤ  0: hb_barrier (); ਵਾਪਸ u.format0 .get_language ();
    ਹਾਲਤ  4: hb_barrier (); ਵਾਪਸ u.format4 .get_language ();
    ਹਾਲਤ  6: hb_barrier (); ਵਾਪਸ u.format6 .get_language ();
    ਹਾਲਤ 10: hb_barrier (); ਵਾਪਸ u.format10.get_language ();
    ਹਾਲਤ 12: hb_barrier (); ਵਾਪਸ u.format12.get_language ();
    ਹਾਲਤ 13: hb_barrier (); ਵਾਪਸ u.format13.get_language ();
    ਹਾਲਤ 14:
    ਮੂਲ: ਵਾਪਸ 0;
    }
  }

  ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  ਖਾਲੀ serialize (hb_serialize_context_t *c,
		  Iterator it,
		  ਬਿਨਾਨਿਸ਼ਾਨੀ format,
		  ਅਚਲ hb_subset_plan_t *plan,
		  ਅਚਲ ਖਾਲੀ *ਅਧਾਰ)
  {
    ਚੋਣ (format) {
    ਹਾਲਤ  4: hb_barrier (); ਵਾਪਸ u.format4.serialize (c, it);
    ਹਾਲਤ 12: hb_barrier (); ਵਾਪਸ u.format12.serialize (c, it);
    ਹਾਲਤ 14: hb_barrier (); ਵਾਪਸ u.format14.serialize (c, &plan->unicodes, &plan->glyphs_requested, plan->glyph_map, ਅਧਾਰ);
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਹਾਲਤ 15: hb_barrier (); ਵਾਪਸ u.format15.serialize (c, &plan->unicodes, &plan->glyphs_requested, plan->glyph_map, ਅਧਾਰ);
#ਜੇ_ਅੰਤ
    ਮੂਲ: ਵਾਪਸ;
    }
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    ਜੇ (!u.format.v.sanitize (c)) return_trace (ਝੂਠ);
    hb_barrier ();
    ਚੋਣ (u.format.v) {
    ਹਾਲਤ  0: hb_barrier (); return_trace (u.format0 .sanitize (c));
    ਹਾਲਤ  4: hb_barrier (); return_trace (u.format4 .sanitize (c));
    ਹਾਲਤ  6: hb_barrier (); return_trace (u.format6 .sanitize (c));
    ਹਾਲਤ 10: hb_barrier (); return_trace (u.format10.sanitize (c));
    ਹਾਲਤ 12: hb_barrier (); return_trace (u.format12.sanitize (c));
    ਹਾਲਤ 13: hb_barrier (); return_trace (u.format13.sanitize (c));
    ਹਾਲਤ 14: hb_barrier (); return_trace (u.format14.sanitize (c));
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਹਾਲਤ 15: hb_barrier (); return_trace (u.format15.sanitize (c));
#ਜੇ_ਅੰਤ
    ਮੂਲ:return_trace (ਸੱਚ);
    }
  }

  ਜਨਤਕ:
  ਸੰਘ {
  ਬਣਤਰ { HBUINT16 v; }	format;		/* Format identifier */
  CmapSubtableFormat0	format0;
  CmapSubtableFormat4	format4;
  CmapSubtableFormat6	format6;
  CmapSubtableFormat10	format10;
  CmapSubtableFormat12	format12;
  CmapSubtableFormat13	format13;
  CmapSubtableFormat14_15<Layout::SmallTypes>
			format14;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
  CmapSubtableFormat14_15<Layout::MediumTypes>
			format15;
#ਜੇ_ਅੰਤ
  } u;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_UNION (2, format.v);
};


ਬਣਤਰ Encoding_ਰਿਕਾਰਡ
{
  ਪੂਰਨ cmp (ਅਚਲ Encoding_ਰਿਕਾਰਡ &other) ਅਚਲ
  {
    ਪੂਰਨ ret;
    ret = platformID.cmp (other.platformID);
    ਜੇ (ret) ਵਾਪਸ ret;
    ਜੇ (other.encodingID != 0xFFFF)
    {
      ret = encodingID.cmp (other.encodingID);
      ਜੇ (ret) ਵਾਪਸ ret;
    }
    ਵਾਪਸ 0;
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c, ਅਚਲ ਖਾਲੀ *ਅਧਾਰ) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) &&
		  subtable.sanitize (c, ਅਧਾਰ));
  }

  ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  Encoding_ਰਿਕਾਰਡ* copy (hb_serialize_context_t *c,
			Iterator it,
			ਬਿਨਾਨਿਸ਼ਾਨੀ format,
			ਅਚਲ ਖਾਲੀ *ਅਧਾਰ,
			ਅਚਲ hb_subset_plan_t *plan,
			/* INOUT */ ਬਿਨਾਨਿਸ਼ਾਨੀ *objidx) ਅਚਲ
  {
    TRACE_SERIALIZE (ਇਹ);
    ਸਵੈਚਾਲਿਤ snap = c->snapshot ();
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->embed (ਇਹ);
    ਜੇ (unlikely (!ਬਾਹਰਲਾ)) return_trace (ਖਾਲੀ_ਸੂਚਕ);
    ਬਾਹਰਲਾ->subtable = 0;

    ਜੇ (*objidx == 0)
    {
      CmapSubtable *cmapsubtable = c->push<CmapSubtable> ();
      ਬਿਨਾਨਿਸ਼ਾਨੀ origin_ਲੰਮਾਈ = c->ਲੰਮਾਈ ();
      cmapsubtable->serialize (c, it, format, plan, &(ਅਧਾਰ+subtable));
      ਜੇ (c->ਲੰਮਾਈ () > origin_ਲੰਮਾਈ && !c->ਵਿੱਚ_error()) *objidx = c->ਕੱਢੋ_pack ();
      ਨਹੀਂਤਾਂ c->ਕੱਢੋ_discard ();
    }

    ਜੇ (*objidx == 0)
    {
      c->revert (snap);
      return_trace (ਖਾਲੀ_ਸੂਚਕ);
    }

    c->ਜੋੜੋ_link (ਬਾਹਰਲਾ->subtable, *objidx);
    return_trace (ਬਾਹਰਲਾ);
  }

  HBUINT16	platformID;	/* Platform ID. */
  HBUINT16	encodingID;	/* Platform-specific encoding ID. */
  Offset32_ਮੰਜ਼ਿਲ<CmapSubtable>
		subtable;	/* Byte offset from beginning of table to the subtable for this encoding. */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (8);
};

ਬਣਤਰ cmap;

ਬਣਤਰ SubtableUnicodesCache {

 ਨਿਜੀ:
  hb_blob_ptr_t<cmap> ਅਧਾਰ_blob;
  ਅਚਲ ਅੱਖਰ* ਅਧਾਰ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਾਰਣੀ_ਲੰਮਾਈ;
  hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, hb::unique_ptr<hb_ਸੈੱਟ_t>> cached_unicodes;

 ਜਨਤਕ:

  ਸਥਿਰ SubtableUnicodesCache* create (hb_blob_ptr_t<cmap> ਸਰੋਤ_ਸਾਰਣੀ)
  {
    SubtableUnicodesCache* cache =
        (SubtableUnicodesCache*) hb_malloc (ਆਕਾਰ(SubtableUnicodesCache));

    ਜੇ (unlikely (!cache))
      ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

    ਨਵਾਂ (cache) SubtableUnicodesCache (ਸਰੋਤ_ਸਾਰਣੀ);
    ਵਾਪਸ cache;
  }

  ਸਥਿਰ ਖਾਲੀ destroy (ਖਾਲੀ* value) {
    ਜੇ (!value) ਵਾਪਸ;

    SubtableUnicodesCache* cache = (SubtableUnicodesCache*) value;
    cache->~SubtableUnicodesCache ();
    hb_free (cache);
  }

  SubtableUnicodesCache(ਅਚਲ ਖਾਲੀ* cmap_ਅਧਾਰ,
			ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਾਰਣੀ_ਲੰਮਾਈ_2 = 0)
      : ਅਧਾਰ_blob(),
        ਅਧਾਰ ((ਅਚਲ ਅੱਖਰ*) cmap_ਅਧਾਰ),
        ਸਾਰਣੀ_ਲੰਮਾਈ (ਸਾਰਣੀ_ਲੰਮਾਈ_2),
        cached_unicodes ()
  {}

  SubtableUnicodesCache(hb_blob_ptr_t<cmap> ਅਧਾਰ_blob_2)
      : ਅਧਾਰ_blob(ਅਧਾਰ_blob_2),
        ਅਧਾਰ ((ਅਚਲ ਅੱਖਰ *) ਅਧਾਰ_blob.get()),
        ਸਾਰਣੀ_ਲੰਮਾਈ (ਅਧਾਰ_blob.get_ਲੰਮਾਈ ()),
        cached_unicodes ()
  {}

  ~SubtableUnicodesCache()
  {
    ਅਧਾਰ_blob.destroy ();
  }

  ਤਾਰਕਿਕ same_ਅਧਾਰ(ਅਚਲ ਖਾਲੀ* other) ਅਚਲ
  {
    ਵਾਪਸ other == (ਅਚਲ ਖਾਲੀ*) ਅਧਾਰ;
  }

  ਅਚਲ hb_ਸੈੱਟ_t* ਸੈੱਟ_for (ਅਚਲ Encoding_ਰਿਕਾਰਡ* ਰਿਕਾਰਡ,
                           SubtableUnicodesCache& mutable_cache) ਅਚਲ
  {
    ਜੇ (cached_unicodes.has ((ਬਿਨਾਨਿਸ਼ਾਨੀ) ((ਅਚਲ ਅੱਖਰ *) ਰਿਕਾਰਡ - ਅਧਾਰ)))
      ਵਾਪਸ cached_unicodes.get ((ਬਿਨਾਨਿਸ਼ਾਨੀ) ((ਅਚਲ ਅੱਖਰ *) ਰਿਕਾਰਡ - ਅਧਾਰ));

    ਵਾਪਸ mutable_cache.ਸੈੱਟ_for (ਰਿਕਾਰਡ);
  }

  ਅਚਲ hb_ਸੈੱਟ_t* ਸੈੱਟ_for (ਅਚਲ Encoding_ਰਿਕਾਰਡ* ਰਿਕਾਰਡ)
  {
    ਜੇ (!cached_unicodes.has ((ਬਿਨਾਨਿਸ਼ਾਨੀ) ((ਅਚਲ ਅੱਖਰ *) ਰਿਕਾਰਡ - ਅਧਾਰ)))
    {
      hb_ਸੈੱਟ_t *s = hb_ਸੈੱਟ_create ();
      ਜੇ (unlikely (s->ਵਿੱਚ_error ()))
	ਵਾਪਸ hb_ਸੈੱਟ_get_ਖਾਲੀ_ਥਾਵਾਂ ();

      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ subtable_ਅੰਕੜੇ_ਮਾਪ = ਰਿਕਾਰਡ->subtable < ਸਾਰਣੀ_ਲੰਮਾਈ
					? ਸਾਰਣੀ_ਲੰਮਾਈ - (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) ਰਿਕਾਰਡ->subtable
					: 0;
      (ਅਧਾਰ+ਰਿਕਾਰਡ->subtable).collect_unicodes (s, UINT_MAX, subtable_ਅੰਕੜੇ_ਮਾਪ);

      ਜੇ (unlikely (!cached_unicodes.ਸੈੱਟ ((ਬਿਨਾਨਿਸ਼ਾਨੀ) ((ਅਚਲ ਅੱਖਰ *) ਰਿਕਾਰਡ - ਅਧਾਰ), hb::unique_ptr<hb_ਸੈੱਟ_t> {s})))
        ਵਾਪਸ hb_ਸੈੱਟ_get_ਖਾਲੀ_ਥਾਵਾਂ ();

      ਵਾਪਸ s;
    }
    ਵਾਪਸ cached_unicodes.get ((ਬਿਨਾਨਿਸ਼ਾਨੀ) ((ਅਚਲ ਅੱਖਰ *) ਰਿਕਾਰਡ - ਅਧਾਰ));
  }

};

ਸਥਿਰ ਸਤਰਅੰਦਰ uint16_t
hb_ਚਿੰਨ੍ਹ_pua_map (ਬਿਨਾਨਿਸ਼ਾਨੀ codepoint)
{
  ਜੇ (codepoint <= 0x00FFu)
  {
    /* For symbol-encoded OpenType fonts, we duplicate the
     * U+F000..F0FF range at U+0000..U+00FF.  That's what
     * Windows seems to do, and that's hinted about at:
     * https://docs.microsoft.com/en-us/typography/opentype/spec/recom
     * under "Non-Standard (Symbol) Fonts". */
    ਵਾਪਸ 0xF000u + codepoint;
  }
  ਵਾਪਸ 0;
}

ਬਣਤਰ cmap
{
  ਸਥਿਰ constexpr hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ = HB_OT_ਟੈਗ_cmap;


  ਸਥਿਰ SubtableUnicodesCache* create_filled_cache(hb_blob_ptr_t<cmap> ਸਰੋਤ_ਸਾਰਣੀ) {
    ਅਚਲ cmap* cmap = ਸਰੋਤ_ਸਾਰਣੀ.get();
    ਸਵੈਚਾਲਿਤ it =
    + hb_iter (cmap->encoding_ਰਿਕਾਰਡ)
    | hb_filter ([&](ਅਚਲ Encoding_ਰਿਕਾਰਡ& _) {
      ਵਾਪਸ cmap::filter_encoding_ਰਿਕਾਰਡਾਂ_for_subset (cmap, _);
    })
    ;

    SubtableUnicodesCache* cache = SubtableUnicodesCache::create(ਸਰੋਤ_ਸਾਰਣੀ);

    ਜੇ (unlikely (!cache))
      ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

    ਲਈ (ਅਚਲ Encoding_ਰਿਕਾਰਡ& _ : it)
      cache->ਸੈੱਟ_for(&_); // populate the cache for this encoding record.

    ਵਾਪਸ cache;
  }

  ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ Iterator, ਕਿਸਮ_ਨਾਂ EncodingRecIter,
	   hb_requires (hb_is_iterator (EncodingRecIter))>
  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c,
		  Iterator it,
		  EncodingRecIter encodingrec_iter,
		  ਅਚਲ ਖਾਲੀ *ਅਧਾਰ,
		  hb_subset_plan_t *plan,
	                  ਤਾਰਕਿਕ drop_format_4 = ਝੂਠ,
			  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਰੋਤ_ਸਾਰਣੀ_ਲੰਮਾਈ = 0)
  {
    ਜੇ (unlikely (!c->extend_min ((*ਇਹ))))  ਵਾਪਸ ਝੂਠ;
    ਇਹ->ਸੰਸਕਰਣ = 0;

    ਬਿਨਾਨਿਸ਼ਾਨੀ format4objidx = 0, format12objidx = 0, format14objidx = 0;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਬਿਨਾਨਿਸ਼ਾਨੀ format15objidx = 0;
#ਜੇ_ਅੰਤ
    ਸਵੈਚਾਲਿਤ snap = c->snapshot ();

    SubtableUnicodesCache local_unicodes_cache (ਅਧਾਰ, ਸਰੋਤ_ਸਾਰਣੀ_ਲੰਮਾਈ);
    ਅਚਲ SubtableUnicodesCache* unicodes_cache = &local_unicodes_cache;

    ਜੇ (plan->accelerator &&
        plan->accelerator->cmap_cache &&
        plan->accelerator->cmap_cache->same_ਅਧਾਰ (ਅਧਾਰ))
      unicodes_cache = plan->accelerator->cmap_cache;

    ਲਈ (ਅਚਲ Encoding_ਰਿਕਾਰਡ& _ : encodingrec_iter)
    {
      ਜੇ (c->ਵਿੱਚ_error ())
        ਵਾਪਸ ਝੂਠ;

      ਬਿਨਾਨਿਸ਼ਾਨੀ format = (ਅਧਾਰ+_.subtable).u.format.v;
      ਜੇ (format != 4 && format != 12 && format != 14
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
	  && format != 15
#ਜੇ_ਅੰਤ
	 ) ਜਾਰੀ;

      ਅਚਲ hb_ਸੈੱਟ_t* unicodes_ਸੈੱਟ = unicodes_cache->ਸੈੱਟ_for (&_, local_unicodes_cache);

      ਜੇ (!drop_format_4 && format == 4)
      {
        c->copy (_, + it | hb_filter (*unicodes_ਸੈੱਟ, hb_ਪਹਿਲਾ), 4u, ਅਧਾਰ, plan, &format4objidx);
        ਜੇ (c->ਵਿੱਚ_error () && c->only_ਉਛਾਲ ())
        {
          // cmap4 overflowed, reset and retry serialization without format 4 subtables.
          c->revert (snap);
          ਵਾਪਸ serialize (c, it,
                            encodingrec_iter,
                            ਅਧਾਰ,
                            plan,
                            ਸੱਚ,
			    ਸਰੋਤ_ਸਾਰਣੀ_ਲੰਮਾਈ);
        }
      }

      ਨਹੀਂਤਾਂ ਜੇ (format == 12)
      {
        ਜੇ (_can_drop (_,
                       *unicodes_ਸੈੱਟ,
                       ਅਧਾਰ,
                       *unicodes_cache,
                       local_unicodes_cache,
                       + it | hb_map (hb_ਪਹਿਲਾ), encodingrec_iter))
          ਜਾਰੀ;
        c->copy (_, + it | hb_filter (*unicodes_ਸੈੱਟ, hb_ਪਹਿਲਾ), 12u, ਅਧਾਰ, plan, &format12objidx);
      }
      ਨਹੀਂਤਾਂ ਜੇ (format == 14) c->copy (_, it, 14u, ਅਧਾਰ, plan, &format14objidx);
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
      ਨਹੀਂਤਾਂ ਜੇ (format == 15) c->copy (_, it, 15u, ਅਧਾਰ, plan, &format15objidx);
#ਜੇ_ਅੰਤ
    }
        ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾਈ = c->ਲੰਮਾਈ ();
        ਬਿਨਾਨਿਸ਼ਾਨੀ ਉਪਲਬਧ = ਲੰਮਾਈ > cmap::min_ਮਾਪ ? ਲੰਮਾਈ - cmap::min_ਮਾਪ : 0;
        c->check_ਸੌਂਪੋ(ਇਹ->encoding_ਰਿਕਾਰਡ.len,
                        ਉਪਲਬਧ / Encoding_ਰਿਕਾਰਡ::static_ਮਾਪ,
                        HB_SERIALIZE_ERROR_INT_ਉਛਾਲ);

    // Fail if format 4 was dropped and there is no cmap12.
    ਵਾਪਸ !drop_format_4 || format12objidx;
  }

  ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ Iterator, ਕਿਸਮ_ਨਾਂ Encoding_ਰਿਕਾਰਡ_Iterator,
      hb_requires (hb_is_iterator (Iterator)),
      hb_requires (hb_is_iterator (Encoding_ਰਿਕਾਰਡ_Iterator))>
  ਤਾਰਕਿਕ _can_drop (ਅਚਲ Encoding_ਰਿਕਾਰਡ& cmap12,
                  ਅਚਲ hb_ਸੈੱਟ_t& cmap12_unicodes,
                  ਅਚਲ ਖਾਲੀ* ਅਧਾਰ,
                  ਅਚਲ SubtableUnicodesCache& unicodes_cache,
                  SubtableUnicodesCache& local_unicodes_cache,
                  Iterator subset_unicodes,
                  Encoding_ਰਿਕਾਰਡ_Iterator encoding_ਰਿਕਾਰਡਾਂ)
  {
    ਲਈ (ਸਵੈਚਾਲਿਤ cp : + subset_unicodes | hb_filter (cmap12_unicodes))
    {
      ਜੇ (cp >= 0x10000) ਵਾਪਸ ਝੂਠ;
    }

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਨਿਸ਼ਾਨਾ_platform;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਨਿਸ਼ਾਨਾ_encoding;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਨਿਸ਼ਾਨਾ_language = (ਅਧਾਰ+cmap12.subtable).get_language ();

    ਜੇ (cmap12.platformID == 0 && cmap12.encodingID == 4)
    {
      ਨਿਸ਼ਾਨਾ_platform = 0;
      ਨਿਸ਼ਾਨਾ_encoding = 3;
    } ਨਹੀਂਤਾਂ ਜੇ (cmap12.platformID == 3 && cmap12.encodingID == 10) {
      ਨਿਸ਼ਾਨਾ_platform = 3;
      ਨਿਸ਼ਾਨਾ_encoding = 1;
    } ਨਹੀਂਤਾਂ {
      ਵਾਪਸ ਝੂਠ;
    }

    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& _ : encoding_ਰਿਕਾਰਡਾਂ)
    {
      ਜੇ (_.platformID != ਨਿਸ਼ਾਨਾ_platform
          || _.encodingID != ਨਿਸ਼ਾਨਾ_encoding
          || (ਅਧਾਰ+_.subtable).get_language() != ਨਿਸ਼ਾਨਾ_language)
        ਜਾਰੀ;

      ਅਚਲ hb_ਸੈੱਟ_t* sibling_unicodes = unicodes_cache.ਸੈੱਟ_for (&_, local_unicodes_cache);

      ਸਵੈਚਾਲਿਤ cmap12 = + subset_unicodes | hb_filter (cmap12_unicodes);
      ਸਵੈਚਾਲਿਤ sibling = + subset_unicodes | hb_filter (*sibling_unicodes);
      ਲਈ (; cmap12 && sibling; cmap12++, sibling++)
      {
        ਬਿਨਾਨਿਸ਼ਾਨੀ a = *cmap12;
        ਬਿਨਾਨਿਸ਼ਾਨੀ b = *sibling;
        ਜੇ (a != b) ਵਾਪਸ ਝੂਠ;
      }

      ਵਾਪਸ !cmap12 && !sibling;
    }

    ਵਾਪਸ ਝੂਠ;
  }

  ਖਾਲੀ ਘੇਰਾ_glyphs (ਅਚਲ hb_ਸੈੱਟ_t      *unicodes,
		       hb_ਸੈੱਟ_t            *glyphset) ਅਚਲ
  {
    + hb_iter (encoding_ਰਿਕਾਰਡ)
    | hb_map (&Encoding_ਰਿਕਾਰਡ::subtable)
    | hb_map (hb_ਜੋੜੋ (ਇਹ))
    | hb_filter (&CmapSubtable::is_variation_selector)
    | hb_apply ([=] (ਅਚਲ CmapSubtable& _) { _.ਘੇਰਾ_glyphs (unicodes, glyphset); })
    ;
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  { ਵਾਪਸ subset (c, ਝੂਠ); }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c, ਤਾਰਕਿਕ is_dmap) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);

    cmap *cmap_ਅਭਾਜ = c->serializer->ਸ਼ੁਰੂ_embed<cmap> ();

    ਸਵੈਚਾਲਿਤ encodingrec_iter =
    + hb_iter (encoding_ਰਿਕਾਰਡ)
    | hb_filter ([&](ਅਚਲ Encoding_ਰਿਕਾਰਡ& _) {
      ਵਾਪਸ cmap::filter_encoding_ਰਿਕਾਰਡਾਂ_for_subset (ਇਹ, _);
    })
    ;

    ਜੇ (unlikely (!encodingrec_iter.len ())) return_trace (ਝੂਠ);

    ਅਚਲ Encoding_ਰਿਕਾਰਡ *unicode_bmp= ਖਾਲੀ_ਸੂਚਕ, *unicode_ucs4 = ਖਾਲੀ_ਸੂਚਕ, *ms_bmp = ਖਾਲੀ_ਸੂਚਕ, *ms_ucs4 = ਖਾਲੀ_ਸੂਚਕ;
    ਤਾਰਕਿਕ has_format12 = ਝੂਠ;

    ਲਈ (ਅਚਲ Encoding_ਰਿਕਾਰਡ& _ : encodingrec_iter)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ format = (ਇਹ + _.subtable).u.format.v;
      ਜੇ (format == 12) has_format12 = ਸੱਚ;

      ਅਚਲ Encoding_ਰਿਕਾਰਡ *ਸਾਰਣੀ = std::addressof (_);
      ਜੇ      (_.platformID == 0 && _.encodingID ==  3) unicode_bmp = ਸਾਰਣੀ;
      ਨਹੀਂਤਾਂ ਜੇ (_.platformID == 0 && _.encodingID ==  4) unicode_ucs4 = ਸਾਰਣੀ;
      ਨਹੀਂਤਾਂ ਜੇ (_.platformID == 3 && _.encodingID ==  1) ms_bmp = ਸਾਰਣੀ;
      ਨਹੀਂਤਾਂ ਜੇ (_.platformID == 3 && _.encodingID == 10) ms_ucs4 = ਸਾਰਣੀ;
    }

    ਜੇ (unlikely (!is_dmap && !has_format12 && !unicode_bmp && !ms_bmp)) return_trace (ਝੂਠ);
    ਜੇ (unlikely (!is_dmap && has_format12 && (!unicode_ucs4 && !ms_ucs4))) return_trace (ਝੂਠ);

    ਸਵੈਚਾਲਿਤ it =
    + c->plan->unicode_ਮੰਜ਼ਿਲ_new_gid_list.iter ()
    | hb_filter ([&] (ਅਚਲ hb_codepoint_pair_t _)
		 { ਵਾਪਸ (_.second != HB_MAP_ਮੁੱਲ_INVALID); })
    ;

    return_trace (cmap_ਅਭਾਜ->serialize (c->serializer,
                                         it,
                                         encodingrec_iter,
                                         ਇਹ,
                                         c->plan,
					 ਝੂਠ,
					 c->ਸਰੋਤ_blob->ਲੰਮਾਈ));
  }

  ਅਚਲ CmapSubtable *ਲੱਭੋ_ਉੱਤਮ_subtable (ਤਾਰਕਿਕ *ਚਿੰਨ੍ਹ = ਖਾਲੀ_ਸੂਚਕ,
					  ਤਾਰਕਿਕ *mac = ਖਾਲੀ_ਸੂਚਕ,
					  ਤਾਰਕਿਕ *macroman = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
  {
    ਜੇ (ਚਿੰਨ੍ਹ) *ਚਿੰਨ੍ਹ = ਝੂਠ;
    ਜੇ (mac) *mac = ਝੂਠ;
    ਜੇ (macroman) *macroman = ਝੂਠ;

    ਅਚਲ CmapSubtable *subtable;

    /* Symbol subtable.
     * Prefer symbol if available.
     * https://github.com/harfbuzz/harfbuzz/issues/1918 */
    ਜੇ ((subtable = ਇਹ->ਲੱਭੋ_subtable (3, 0)))
    {
      ਜੇ (ਚਿੰਨ੍ਹ) *ਚਿੰਨ੍ਹ = ਸੱਚ;
      ਵਾਪਸ subtable;
    }

    /* 32-bit subtables. */
    ਜੇ ((subtable = ਇਹ->ਲੱਭੋ_subtable (3, 10))) ਵਾਪਸ subtable;
    ਜੇ ((subtable = ਇਹ->ਲੱਭੋ_subtable (0, 6))) ਵਾਪਸ subtable;
    ਜੇ ((subtable = ਇਹ->ਲੱਭੋ_subtable (0, 4))) ਵਾਪਸ subtable;

    /* 16-bit subtables. */
    ਜੇ ((subtable = ਇਹ->ਲੱਭੋ_subtable (3, 1))) ਵਾਪਸ subtable;
    ਜੇ ((subtable = ਇਹ->ਲੱਭੋ_subtable (0, 3))) ਵਾਪਸ subtable;
    ਜੇ ((subtable = ਇਹ->ਲੱਭੋ_subtable (0, 2))) ਵਾਪਸ subtable;
    ਜੇ ((subtable = ਇਹ->ਲੱਭੋ_subtable (0, 1))) ਵਾਪਸ subtable;
    ਜੇ ((subtable = ਇਹ->ਲੱਭੋ_subtable (0, 0))) ਵਾਪਸ subtable;

    /* MacRoman subtable. */
    ਜੇ ((subtable = ਇਹ->ਲੱਭੋ_subtable (1, 0)))
    {
      ਜੇ (mac) *mac = ਸੱਚ;
      ਜੇ (macroman) *macroman = ਸੱਚ;
      ਵਾਪਸ subtable;
    }
    /* Any other Mac subtable; we just map ASCII for these. */
    ਜੇ ((subtable = ਇਹ->ਲੱਭੋ_subtable (1, 0xFFFF)))
    {
      ਜੇ (mac) *mac = ਸੱਚ;
      ਵਾਪਸ subtable;
    }

    /* Meh. */
    ਵਾਪਸ &Null (CmapSubtable);
  }

  ਬਣਤਰ accelerator_t
  {
    ਵਰਤੋਂ cache_t = hb_cache_t<21, 19>;
    static_assert (ਆਕਾਰ (cache_t) == 1024, "");

    accelerator_t (hb_face_t *face, hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ_2 = cmap::ਸਾਰਣੀ_ਟੈਗ)
    {
      ਇਹ->ਸਾਰਣੀ = hb_sanitize_context_t ().reference_ਸਾਰਣੀ<cmap> (face, ਸਾਰਣੀ_ਟੈਗ_2);
      ਜੇ (!ਇਹ->ਸਾਰਣੀ.get_ਲੰਮਾਈ ())
	ਵਾਪਸ;

      ਤਾਰਕਿਕ ਚਿੰਨ੍ਹ, mac, macroman;
      ਇਹ->subtable = ਸਾਰਣੀ->ਲੱਭੋ_ਉੱਤਮ_subtable (&ਚਿੰਨ੍ਹ, &mac, &macroman);
      ਇਹ->subtable_uvs = &Null (CmapSubtable);
      {
	ਅਚਲ CmapSubtable *st = ਸਾਰਣੀ->ਲੱਭੋ_variation_selector_subtable ();
	ਜੇ (st)
	  subtable_uvs = st;
      }

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_FONT_CMAP_CACHE
      cache = (cache_t *) hb_malloc (ਆਕਾਰ (cache_t));
      ਜੇ (cache)
	ਨਵਾਂ (cache) cache_t ();
      ਨਹੀਂਤਾਂ
        ਵਾਪਸ; // Such that get_glyph_funcZ remains null.
#ਜੇ_ਅੰਤ

      ਇਹ->get_glyph_ਅੰਕੜੇ = subtable;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_CMAP_LEGACY_SUBTABLES
      ਤਾਰਕਿਕ is_format4 = subtable->u.format.v == 4;
      ਸਵੈਚਾਲਿਤ ਸੈੱਟ_format4_getter = [ਇਹ] (ਤਾਰਕਿਕ (*func) (ਅਚਲ ਖਾਲੀ *,
						      hb_codepoint_t,
						      hb_codepoint_t *))
      {
	ਇਹ->format4_accel.init (&ਇਹ->subtable->u.format4,
				  get_subtable_ਅੰਕੜੇ_ਮਾਪ (ਇਹ->subtable));
	ਇਹ->get_glyph_ਅੰਕੜੇ = &ਇਹ->format4_accel;
	ਇਹ->get_glyph_funcZ = func;
      };
      ਜੇ (unlikely (ਚਿੰਨ੍ਹ))
      {
	ਚੋਣ ((ਬਿਨਾਨਿਸ਼ਾਨੀ) face->ਸਾਰਣੀ.OS2->get_font_ਪੰਨਾ ()) {
	ਹਾਲਤ OS2::font_ਪੰਨਾ_t::FONT_ਪੰਨਾ_NONE:
	  ਜੇ (is_format4)
	    ਸੈੱਟ_format4_getter (get_glyph_from_ਚਿੰਨ੍ਹ<CmapSubtableFormat4::accelerator_t, hb_ਚਿੰਨ੍ਹ_pua_map>);
	  ਨਹੀਂਤਾਂ
	    ਇਹ->get_glyph_funcZ = get_glyph_from_ਚਿੰਨ੍ਹ<CmapSubtable, hb_ਚਿੰਨ੍ਹ_pua_map>;
	  ਰੋਕੋ;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_SHAPER_ARABIC_FALLBACK
	ਹਾਲਤ OS2::font_ਪੰਨਾ_t::FONT_ਪੰਨਾ_SIMP_ARABIC:
	  ਜੇ (is_format4)
	    ਸੈੱਟ_format4_getter (get_glyph_from_ਚਿੰਨ੍ਹ<CmapSubtableFormat4::accelerator_t, _hb_arabic_pua_simp_map>);
	  ਨਹੀਂਤਾਂ
	    ਇਹ->get_glyph_funcZ = get_glyph_from_ਚਿੰਨ੍ਹ<CmapSubtable, _hb_arabic_pua_simp_map>;
	  ਰੋਕੋ;
	ਹਾਲਤ OS2::font_ਪੰਨਾ_t::FONT_ਪੰਨਾ_TRAD_ARABIC:
	  ਜੇ (is_format4)
	    ਸੈੱਟ_format4_getter (get_glyph_from_ਚਿੰਨ੍ਹ<CmapSubtableFormat4::accelerator_t, _hb_arabic_pua_trad_map>);
	  ਨਹੀਂਤਾਂ
	    ਇਹ->get_glyph_funcZ = get_glyph_from_ਚਿੰਨ੍ਹ<CmapSubtable, _hb_arabic_pua_trad_map>;
	  ਰੋਕੋ;
#ਜੇ_ਅੰਤ
	ਮੂਲ:
	  ਜੇ (is_format4)
	    ਸੈੱਟ_format4_getter (get_glyph_from<CmapSubtableFormat4::accelerator_t>);
	  ਨਹੀਂਤਾਂ
	    ਇਹ->get_glyph_funcZ = get_glyph_from<CmapSubtable>;
	  ਰੋਕੋ;
	}
      }
      ਨਹੀਂਤਾਂ ਜੇ (unlikely (macroman))
      {
	ਜੇ (is_format4)
	  ਸੈੱਟ_format4_getter (get_glyph_from_macroman<CmapSubtableFormat4::accelerator_t>);
	ਨਹੀਂਤਾਂ
	  ਇਹ->get_glyph_funcZ = get_glyph_from_macroman<CmapSubtable>;
      }
      ਨਹੀਂਤਾਂ ਜੇ (unlikely (mac))
      {
	ਜੇ (is_format4)
	  ਸੈੱਟ_format4_getter (get_glyph_from_ascii<CmapSubtableFormat4::accelerator_t>);
	ਨਹੀਂਤਾਂ
	  ਇਹ->get_glyph_funcZ = get_glyph_from_ascii<CmapSubtable>;
      }
      ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ
      {
	ਚੋਣ (subtable->u.format.v) {
	  /* Accelerate format 4 and format 12. */
	  ਮੂਲ:
	    ਇਹ->get_glyph_funcZ = get_glyph_from<CmapSubtable>;
	    ਰੋਕੋ;
	  ਹਾਲਤ 12:
	    ਇਹ->get_glyph_funcZ = get_glyph_from<CmapSubtableFormat12>;
	    ਰੋਕੋ;
	  ਹਾਲਤ  4:
	  {
	    hb_barrier ();
	    ਇਹ->format4_accel.init (&subtable->u.format4,
				      get_subtable_ਅੰਕੜੇ_ਮਾਪ (subtable));
	    ਇਹ->get_glyph_ਅੰਕੜੇ = &ਇਹ->format4_accel;
	    ਇਹ->get_glyph_funcZ = ਇਹ->format4_accel.get_glyph_func;
	    ਰੋਕੋ;
	  }
	}
      }
    }
    ~accelerator_t ()
    {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_FONT_CMAP_CACHE
      hb_free (cache);
#ਜੇ_ਅੰਤ
      ਸਾਰਣੀ.destroy ();
    }

    ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ _cached_get (hb_codepoint_t unicode,
			     hb_codepoint_t *glyph) ਅਚਲ
    {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_FONT_CMAP_CACHE
      // cache is always non-null if we have a get_glyph_funcZ
      ਬਿਨਾਨਿਸ਼ਾਨੀ v;
      ਜੇ (cache->get (unicode, &v))
      {
        *glyph = v;
	ਵਾਪਸ ਸੱਚ;
      }
#ਜੇ_ਅੰਤ
      ਤਾਰਕਿਕ ret  = ਇਹ->get_glyph_funcZ (ਇਹ->get_glyph_ਅੰਕੜੇ, unicode, glyph);

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_FONT_CMAP_CACHE
      ਜੇ (ret)
        cache->ਸੈੱਟ (unicode, *glyph);
#ਜੇ_ਅੰਤ

      ਵਾਪਸ ret;
    }

    ਤਾਰਕਿਕ get_nominal_glyph (hb_codepoint_t  unicode,
			    hb_codepoint_t *glyph) ਅਚਲ
    {
      ਜੇ (unlikely (!ਇਹ->get_glyph_funcZ)) ਵਾਪਸ ਝੂਠ;
      ਵਾਪਸ _cached_get (unicode, glyph);
    }

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_nominal_glyphs (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2,
				     ਅਚਲ hb_codepoint_t *ਪਹਿਲਾ_unicode,
				     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ unicode_stride,
				     hb_codepoint_t *ਪਹਿਲਾ_glyph,
				     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ glyph_stride) ਅਚਲ
    {
      ਜੇ (unlikely (!ਇਹ->get_glyph_funcZ)) ਵਾਪਸ 0;

      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਪੂਰਾ;
      ਲਈ (ਪੂਰਾ = 0;
	   ਪੂਰਾ < ਗਿਣਤੀ_2 && _cached_get (*ਪਹਿਲਾ_unicode, ਪਹਿਲਾ_glyph);
	   ਪੂਰਾ++)
      {
	ਪਹਿਲਾ_unicode = &Struct_At_ਵਿੱਥ_Unaligned<hb_codepoint_t> (ਪਹਿਲਾ_unicode, unicode_stride);
	ਪਹਿਲਾ_glyph = &Struct_At_ਵਿੱਥ_Unaligned<hb_codepoint_t> (ਪਹਿਲਾ_glyph, glyph_stride);
      }
      ਵਾਪਸ ਪੂਰਾ;
    }

    ਤਾਰਕਿਕ get_variation_glyph (hb_codepoint_t  unicode,
			      hb_codepoint_t  variation_selector,
			      hb_codepoint_t *glyph) ਅਚਲ
    {
      ਚੋਣ (get_glyph_variant (unicode, variation_selector, glyph))
      {
	ਹਾਲਤ GLYPH_VARIANT_NOT_ਮਿਲਿਆ:	ਵਾਪਸ ਝੂਠ;
	ਹਾਲਤ GLYPH_VARIANT_ਮਿਲਿਆ:	ਵਾਪਸ ਸੱਚ;
	ਹਾਲਤ GLYPH_VARIANT_USE_DEFAULT:	ਰੋਕੋ;
      }

      ਵਾਪਸ get_nominal_glyph (unicode, glyph);
    }

    glyph_variant_t get_glyph_variant (hb_codepoint_t  unicode,
				       hb_codepoint_t  variation_selector,
				       hb_codepoint_t *glyph) ਅਚਲ
    {
      ਵਾਪਸ ਇਹ->subtable_uvs->get_glyph_variant (unicode,
						    variation_selector,
						    glyph);
    }

    ਤਾਰਕਿਕ has_ਅੰਕੜੇ () ਅਚਲ { ਵਾਪਸ ਸਾਰਣੀ.get_ਲੰਮਾਈ (); }

    ਖਾਲੀ collect_unicodes (hb_ਸੈੱਟ_t *ਬਾਹਰਲਾ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_glyphs) ਅਚਲ
    { subtable->collect_unicodes (ਬਾਹਰਲਾ, ਅੰਕ_glyphs, get_subtable_ਅੰਕੜੇ_ਮਾਪ (subtable)); }
    ਖਾਲੀ collect_mapping (hb_ਸੈੱਟ_t *unicodes, hb_map_t *mapping,
			  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_glyphs = UINT_MAX) ਅਚਲ
    { subtable->collect_mapping (unicodes, mapping, ਅੰਕ_glyphs, get_subtable_ਅੰਕੜੇ_ਮਾਪ (subtable)); }
    ਖਾਲੀ collect_variation_selectors (hb_ਸੈੱਟ_t *ਬਾਹਰਲਾ) ਅਚਲ
    { subtable_uvs->collect_variation_selectors (ਬਾਹਰਲਾ); }
    ਖਾਲੀ collect_variation_unicodes (hb_codepoint_t variation_selector,
				     hb_ਸੈੱਟ_t *ਬਾਹਰਲਾ) ਅਚਲ
    { subtable_uvs->collect_variation_unicodes (variation_selector, ਬਾਹਰਲਾ); }

    ਸੁਰੱਖਿਅਤ:
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਤਾਰਕਿਕ (*hb_cmap_get_glyph_func_t) (ਅਚਲ ਖਾਲੀ *obj,
					      hb_codepoint_t codepoint,
					      hb_codepoint_t *glyph);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ uint16_t (*hb_pua_remap_func_t) (ਬਿਨਾਨਿਸ਼ਾਨੀ);

    ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type>
    HB_ਅੰਦਰੂਨੀ ਸਥਿਰ ਤਾਰਕਿਕ get_glyph_from (ਅਚਲ ਖਾਲੀ *obj,
					    hb_codepoint_t codepoint,
					    hb_codepoint_t *glyph)
    {
      ਅਚਲ Type *typed_obj = (ਅਚਲ Type *) obj;
      ਵਾਪਸ typed_obj->get_glyph (codepoint, glyph);
    }

    ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type, hb_pua_remap_func_t remap>
    HB_ਅੰਦਰੂਨੀ ਸਥਿਰ ਤਾਰਕਿਕ get_glyph_from_ਚਿੰਨ੍ਹ (ਅਚਲ ਖਾਲੀ *obj,
						   hb_codepoint_t codepoint,
						   hb_codepoint_t *glyph)
    {
      ਅਚਲ Type *typed_obj = (ਅਚਲ Type *) obj;
      ਜੇ (likely (typed_obj->get_glyph (codepoint, glyph)))
	ਵਾਪਸ ਸੱਚ;

      ਜੇ (hb_codepoint_t c = remap (codepoint))
	ਵਾਪਸ typed_obj->get_glyph (c, glyph);

      ਵਾਪਸ ਝੂਠ;
    }

    ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type>
    HB_ਅੰਦਰੂਨੀ ਸਥਿਰ ਤਾਰਕਿਕ get_glyph_from_ascii (ਅਚਲ ਖਾਲੀ *obj,
						  hb_codepoint_t codepoint,
						  hb_codepoint_t *glyph)
    {
      ਅਚਲ Type *typed_obj = (ਅਚਲ Type *) obj;
      ਵਾਪਸ codepoint < 0x80 && typed_obj->get_glyph (codepoint, glyph);
    }

    ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type>
    HB_ਅੰਦਰੂਨੀ ਸਥਿਰ ਤਾਰਕਿਕ get_glyph_from_macroman (ਅਚਲ ਖਾਲੀ *obj,
						     hb_codepoint_t codepoint,
						     hb_codepoint_t *glyph)
    {
      ਜੇ (get_glyph_from_ascii<Type> (obj, codepoint, glyph))
	ਵਾਪਸ ਸੱਚ;

      ਅਚਲ Type *typed_obj = (ਅਚਲ Type *) obj;
      ਬਿਨਾਨਿਸ਼ਾਨੀ c = unicode_ਮੰਜ਼ਿਲ_macroman (codepoint);
      ਵਾਪਸ c && typed_obj->get_glyph (c, glyph);
    }

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_subtable_ਅੰਕੜੇ_ਮਾਪ (ਅਚਲ CmapSubtable *subtable) ਅਚਲ
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਾਰਣੀ_ਲੰਮਾਈ = ਇਹ->ਸਾਰਣੀ.get_ਲੰਮਾਈ ();
      uintptr_t ਸਾਰਣੀ_ਸ਼ੁਰੂ = (uintptr_t) (ਅਚਲ ਖਾਲੀ *) ਇਹ->ਸਾਰਣੀ.get ();
      uintptr_t subtable_addr = (uintptr_t) (ਅਚਲ ਖਾਲੀ *) subtable;
      ਜੇ (unlikely (subtable_addr < ਸਾਰਣੀ_ਸ਼ੁਰੂ))
	ਵਾਪਸ 0;

      uintptr_t subtable_ਵਿੱਥ = subtable_addr - ਸਾਰਣੀ_ਸ਼ੁਰੂ;
      ਜੇ (unlikely (subtable_ਵਿੱਥ >= ਸਾਰਣੀ_ਲੰਮਾਈ))
	ਵਾਪਸ 0;

      ਵਾਪਸ ਸਾਰਣੀ_ਲੰਮਾਈ - (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) subtable_ਵਿੱਥ;
    }

    ਨਿਜੀ:
    hb_nonnull_ptr_t<ਅਚਲ CmapSubtable> subtable;
    hb_nonnull_ptr_t<ਅਚਲ CmapSubtable> subtable_uvs;

    hb_cmap_get_glyph_func_t get_glyph_funcZ = ਖਾਲੀ_ਸੂਚਕ;
    ਅਚਲ ਖਾਲੀ *get_glyph_ਅੰਕੜੇ = ਖਾਲੀ_ਸੂਚਕ;

    CmapSubtableFormat4::accelerator_t format4_accel;

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_FONT_CMAP_CACHE
    cache_t *cache = ਖਾਲੀ_ਸੂਚਕ;
#ਜੇ_ਅੰਤ

    ਜਨਤਕ:
    hb_blob_ptr_t<cmap> ਸਾਰਣੀ;
  };

  ਸੁਰੱਖਿਅਤ:

  ਅਚਲ CmapSubtable *ਲੱਭੋ_subtable (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ platform_id,
				     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ encoding_id) ਅਚਲ
  {
    Encoding_ਰਿਕਾਰਡ key;
    key.platformID = platform_id;
    key.encodingID = encoding_id;

    ਅਚਲ Encoding_ਰਿਕਾਰਡ &ਨਤੀਜਾ = encoding_ਰਿਕਾਰਡ.bsearch (key);
    ਜੇ (!ਨਤੀਜਾ.subtable)
      ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

    ਵਾਪਸ &(ਇਹ+ਨਤੀਜਾ.subtable);
  }

  ਅਚਲ CmapSubtable *ਲੱਭੋ_variation_selector_subtable () ਅਚਲ
  {
    ਅਚਲ CmapSubtable *format14 = ਖਾਲੀ_ਸੂਚਕ;
    ਲਈ (ਅਚਲ Encoding_ਰਿਕਾਰਡ& ਰਿਕਾਰਡ : encoding_ਰਿਕਾਰਡ)
    {
      ਜੇ (ਰਿਕਾਰਡ.platformID != 0 || ਰਿਕਾਰਡ.encodingID != 5)
	ਜਾਰੀ;

      ਅਚਲ CmapSubtable *subtable = &(ਇਹ+ਰਿਕਾਰਡ.subtable);
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
      ਜੇ (subtable->u.format.v == 15)
	ਵਾਪਸ subtable;
#ਜੇ_ਅੰਤ
      ਜੇ (subtable->u.format.v == 14)
	format14 = subtable;
    }
    ਵਾਪਸ format14;
  }

  ਜਨਤਕ:

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) &&
		  hb_barrier () &&
		  likely (ਸੰਸਕਰਣ == 0) &&
		  encoding_ਰਿਕਾਰਡ.sanitize (c, ਇਹ));
  }

 ਨਿਜੀ:

  ਸਥਿਰ ਤਾਰਕਿਕ filter_encoding_ਰਿਕਾਰਡਾਂ_for_subset(ਅਚਲ cmap* cmap,
                                                 ਅਚਲ Encoding_ਰਿਕਾਰਡ& _)
  {
    ਵਾਪਸ
        (_.platformID == 0 && _.encodingID == 3) ||
        (_.platformID == 0 && _.encodingID == 4) ||
        (_.platformID == 3 && _.encodingID == 1) ||
        (_.platformID == 3 && _.encodingID == 10) ||
        (cmap + _.subtable).is_variation_selector ();
  }

  ਸੁਰੱਖਿਅਤ:
  HBUINT16	ਸੰਸਕਰਣ;	/* Table version number (0). */
  Sorted_Array16_ਦਾ<Encoding_ਰਿਕਾਰਡ>
		encoding_ਰਿਕਾਰਡ;	/* Encoding tables. */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (4, encoding_ਰਿਕਾਰਡ);
};

ਬਣਤਰ DMAP : cmap
{
  ਸਥਿਰ constexpr hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ = HB_OT_ਟੈਗ_DMAP;

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  { ਵਾਪਸ cmap::subset (c, ਸੱਚ); }
};

ਬਣਤਰ cmap_accelerator_t
{
  cmap_accelerator_t (hb_face_t *face) :
    dmap (face, HB_OT_ਟੈਗ_DMAP),
    ਅਧਾਰ (face)
  {}

  ਤਾਰਕਿਕ get_nominal_glyph (hb_codepoint_t  unicode,
			  hb_codepoint_t *glyph) ਅਚਲ
  {
    ਵਾਪਸ dmap.get_nominal_glyph (unicode, glyph) ||
	   ਅਧਾਰ.get_nominal_glyph (unicode, glyph);
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_nominal_glyphs (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2,
				   ਅਚਲ hb_codepoint_t *ਪਹਿਲਾ_unicode,
				   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ unicode_stride,
				   hb_codepoint_t *ਪਹਿਲਾ_glyph,
				   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ glyph_stride) ਅਚਲ
  {
    ਜੇ (likely (!dmap.has_ਅੰਕੜੇ ()))
      ਵਾਪਸ ਅਧਾਰ.get_nominal_glyphs (ਗਿਣਤੀ_2,
				      ਪਹਿਲਾ_unicode, unicode_stride,
				      ਪਹਿਲਾ_glyph, glyph_stride);

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਪੂਰਾ;
    ਲਈ (ਪੂਰਾ = 0;
	 ਪੂਰਾ < ਗਿਣਤੀ_2 && get_nominal_glyph (*ਪਹਿਲਾ_unicode, ਪਹਿਲਾ_glyph);
	 ਪੂਰਾ++)
    {
      ਪਹਿਲਾ_unicode = &Struct_At_ਵਿੱਥ_Unaligned<hb_codepoint_t> (ਪਹਿਲਾ_unicode, unicode_stride);
      ਪਹਿਲਾ_glyph = &Struct_At_ਵਿੱਥ_Unaligned<hb_codepoint_t> (ਪਹਿਲਾ_glyph, glyph_stride);
    }
    ਵਾਪਸ ਪੂਰਾ;
  }

  ਤਾਰਕਿਕ get_variation_glyph (hb_codepoint_t  unicode,
			    hb_codepoint_t  variation_selector,
			    hb_codepoint_t *glyph) ਅਚਲ
  {
    glyph_variant_t ਨਤੀਜਾ = dmap.get_glyph_variant (unicode,
						     variation_selector,
						     glyph);
    ਜੇ (ਨਤੀਜਾ == GLYPH_VARIANT_NOT_ਮਿਲਿਆ)
      ਨਤੀਜਾ = ਅਧਾਰ.get_glyph_variant (unicode, variation_selector, glyph);

    ਚੋਣ (ਨਤੀਜਾ)
    {
      ਹਾਲਤ GLYPH_VARIANT_NOT_ਮਿਲਿਆ: ਵਾਪਸ ਝੂਠ;
      ਹਾਲਤ GLYPH_VARIANT_ਮਿਲਿਆ: ਵਾਪਸ ਸੱਚ;
      ਹਾਲਤ GLYPH_VARIANT_USE_DEFAULT: ਰੋਕੋ;
    }

    ਵਾਪਸ get_nominal_glyph (unicode, glyph);
  }

  ਖਾਲੀ collect_unicodes (hb_ਸੈੱਟ_t *ਬਾਹਰਲਾ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_glyphs) ਅਚਲ
  {
    ਅਧਾਰ.collect_unicodes (ਬਾਹਰਲਾ, ਅੰਕ_glyphs);
    dmap.collect_unicodes (ਬਾਹਰਲਾ, ਅੰਕ_glyphs);
  }

  ਖਾਲੀ collect_mapping (hb_ਸੈੱਟ_t *unicodes, hb_map_t *mapping,
			ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_glyphs = UINT_MAX) ਅਚਲ
  {
    ਅਧਾਰ.collect_mapping (unicodes, mapping, ਅੰਕ_glyphs);
    dmap.collect_mapping (unicodes, mapping, ਅੰਕ_glyphs);
  }

  ਖਾਲੀ collect_variation_selectors (hb_ਸੈੱਟ_t *ਬਾਹਰਲਾ) ਅਚਲ
  {
    ਅਧਾਰ.collect_variation_selectors (ਬਾਹਰਲਾ);
    dmap.collect_variation_selectors (ਬਾਹਰਲਾ);
  }

  ਖਾਲੀ collect_variation_unicodes (hb_codepoint_t variation_selector,
				   hb_ਸੈੱਟ_t *ਬਾਹਰਲਾ) ਅਚਲ
  {
    ਅਧਾਰ.collect_variation_unicodes (variation_selector, ਬਾਹਰਲਾ);
    dmap.collect_variation_unicodes (variation_selector, ਬਾਹਰਲਾ);
  }

  ਖਾਲੀ ਘੇਰਾ_glyphs (ਅਚਲ hb_ਸੈੱਟ_t *unicodes,
		       hb_ਸੈੱਟ_t       *glyphset) ਅਚਲ
  {
    ਜੇ (likely (!dmap.has_ਅੰਕੜੇ ()))
    {
      ਅਧਾਰ.ਸਾਰਣੀ->ਘੇਰਾ_glyphs (unicodes, glyphset);
      ਵਾਪਸ;
    }

    hb_ਸੈੱਟ_t variation_selectors;
    collect_variation_selectors (&variation_selectors);
    ਲਈ (hb_codepoint_t variation_selector : variation_selectors)
    {
      ਜੇ (!unicodes->has (variation_selector))
	ਜਾਰੀ;

      hb_ਸੈੱਟ_t variation_unicodes;
      collect_variation_unicodes (variation_selector, &variation_unicodes);
      ਲਈ (hb_codepoint_t unicode : variation_unicodes)
      {
	ਜੇ (!unicodes->has (unicode))
	  ਜਾਰੀ;

	hb_codepoint_t glyph;
	ਜੇ (get_variation_glyph (unicode, variation_selector, &glyph))
	  glyphset->ਜੋੜੋ (glyph);
      }
    }
  }

  cmap::accelerator_t dmap;
  cmap::accelerator_t ਅਧਾਰ;
};

} /* namespace OT */


#ਜੇ_ਅੰਤ /* HB_OT_CMAP_TABLE_HH */
