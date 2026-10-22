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

#ifndef HB_OT_CMAP_TABLE_HH
#define HB_OT_CMAP_TABLE_HH

#ಅನ್ತರ್ಭಾವ "hb-ot-os2-table.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-shaper-arabic-pua.hh"
#ಅನ್ತರ್ಭಾವ "hb-open-type.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-dual.hh"
#ಅನ್ತರ್ಭಾವ "hb-set.hh"
#ಅನ್ತರ್ಭಾವ "hb-cache.hh"

/*
 * cmap -- Character to Glyph Index Mapping
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cmap
 */
#define HB_OT_TAG_cmap HB_TAG('c','m','a','p')
#define HB_OT_TAG_DMAP HB_TAG('D','M','A','P')

ನಾಮಾಕಾಶ OT {

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ uint8_t unicode_to_macroman (hb_codepoint_t u)
{
  ಸ್ಥಿರ ಧ್ರುವ ಸಂರಚನಾ unicode_to_macroman_t
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
  auto *c = hb_bsearch (u, mapping, ARRAY_LENGTH (mapping), ಪರಿಮಾಣ (mapping[0]),
			_hb_cmp_operator<uint16_t, uint16_t>);
  ಪ್ರತ್ಯಾಗಮ c ? c->macroman : 0;
}

ಸಂರಚನಾ CmapSubtableFormat0
{
  ತಾರ್ಕಿಕ get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) ಧ್ರುವ
  {
    hb_codepoint_t gid = codepoint < 256 ? glyphIdArray[codepoint] : 0;
    ಯದಿ (unlikely (!gid))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    *glyph = gid;
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  unsigned get_language () ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ language;
  }

  ಶೂನ್ಯ collect_unicodes (hb_set_t *out) ಧ್ರುವ
  {
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < 256; i++)
      ಯದಿ (glyphIdArray[i])
	out->add (i);
  }

  ಶೂನ್ಯ collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping /* OUT */) ಧ್ರುವ
  {
    ಪ್ರತಿ (unsigned i = 0; i < 256; i++)
      ಯದಿ (glyphIdArray[i])
      {
	hb_codepoint_t glyph = glyphIdArray[i];
	unicodes->add (i);
	mapping->set (i, glyph);
      }
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್));
  }

  ರಕ್ಷಿತ:
  HBUINT16	format;		/* Format number is set to 0. */
  HBUINT16	length;		/* Byte length of this subtable. */
  HBUINT16	language;	/* Ignore. */
  HBUINT8	glyphIdArray[256];/* An array that maps character
				 * code to glyph index values. */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (6 + 256);
};

ಸಂರಚನಾ CmapSubtableFormat4
{
  template<typename Iterator,
      typename Writer,
	   hb_requires (hb_is_iterator (Iterator))>
  ಶೂನ್ಯ to_ranges (Iterator it, Writer& range_writer)
  {
    hb_codepoint_t start_cp = 0, prev_run_start_cp = 0, run_start_cp = 0, end_cp = 0, last_gid = 0;
    ಪೂರ್ಣಾಙ್ಕ run_length = 0 , delta = 0, prev_delta = 0;

    ಗಣನಾ {
      FIRST_SUB_RANGE,
      FOLLOWING_SUB_RANGE,
    } mode;

    ಯಾವತ್ (it) {
      // Start a new range
      {
        ಧ್ರುವ auto& pair = *it;
        start_cp = pair.first;
        prev_run_start_cp = start_cp;
        run_start_cp = start_cp;
        end_cp = start_cp;
        last_gid = pair.second;
        run_length = 1;
        prev_delta = 0;
      }

      delta = last_gid - start_cp;
      mode = FIRST_SUB_RANGE;
      it++;

      ಯಾವತ್ (it) {
        // Process range
        ಧ್ರುವ auto& pair = *it;
        hb_codepoint_t next_cp = pair.first;
        hb_codepoint_t next_gid = pair.second;
        ಯದಿ (next_cp != end_cp + 1) {
          // Current range is over, stop processing.
          ಭಙ್ಗ;
        }

        ಯದಿ (next_gid == last_gid + 1) {
          // The current run continues.
          end_cp = next_cp;
          run_length++;
          last_gid = next_gid;
          it++;
          ಅನುವರ್ತ;
        }

        // A new run is starting, decide if we want to commit the current run.
        ಪೂರ್ಣಾಙ್ಕ split_cost = (mode == FIRST_SUB_RANGE) ? 8 : 16;
        ಪೂರ್ಣಾಙ್ಕ run_cost = run_length * 2;
        ಯದಿ (run_cost >= split_cost) {
          commit_current_range(start_cp,
                               prev_run_start_cp,
                               run_start_cp,
                               end_cp,
                               delta,
                               prev_delta,
                               split_cost,
                               range_writer);
          start_cp = next_cp;
        }

        // Start the new run
        mode = FOLLOWING_SUB_RANGE;
        prev_run_start_cp = run_start_cp;
        run_start_cp = next_cp;
        end_cp = next_cp;
        prev_delta = delta;
        delta = next_gid - run_start_cp;
        run_length = 1;
        last_gid = next_gid;
        it++;
      }

      // Finalize range
      commit_current_range (start_cp,
                            prev_run_start_cp,
                            run_start_cp,
                            end_cp,
                            delta,
                            prev_delta,
                            8,
                            range_writer);
    }

    ಯದಿ (likely (end_cp != 0xFFFF)) {
      range_writer (0xFFFF, 0xFFFF, 1);
    }
  }

  /*
   * Writes the current range as either one or two ranges depending on what is most efficient.
   */
  template<typename Writer>
  ಶೂನ್ಯ commit_current_range (hb_codepoint_t start,
                             hb_codepoint_t prev_run_start,
                             hb_codepoint_t run_start,
                             hb_codepoint_t end,
                             ಪೂರ್ಣಾಙ್ಕ run_delta,
                             ಪೂರ್ಣಾಙ್ಕ previous_run_delta,
                             ಪೂರ್ಣಾಙ್ಕ split_cost,
                             Writer& range_writer) {
    ತಾರ್ಕಿಕ should_split = ಅಸತ್ಯ;
    ಯದಿ (start < run_start && run_start < end) {
      ಪೂರ್ಣಾಙ್ಕ run_cost = (end - run_start + 1) * 2;
      ಯದಿ (run_cost >= split_cost) {
        should_split = ಸತ್ಯ;
      }
    }

    // A zero delta is also the marker for ranges that need a
    // glyphIdArray; serialize_rangeoffset_glyid() tells the two cases
    // apart by re-checking the mapping.
    ಯದಿ (should_split) {
      ಯದಿ (start == prev_run_start)
        range_writer (start, run_start - 1, previous_run_delta);
      ಅನ್ಯಥಾ
        range_writer (start, run_start - 1, 0);
      range_writer (run_start, end, run_delta);
      ಪ್ರತ್ಯಾಗಮ;
    }


    ಯದಿ (start == run_start) {
      // Range is only a run
      range_writer (start, end, run_delta);
      ಪ್ರತ್ಯಾಗಮ;
    }

    // Write only a single non-run range.
    range_writer (start, end, 0);
  }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  unsigned serialize_find_segcount (Iterator it) {
    ಸಂರಚನಾ Counter {
      unsigned segcount = 0;

      ಶೂನ್ಯ operator() (hb_codepoint_t start,
                       hb_codepoint_t end,
                       ಪೂರ್ಣಾಙ್ಕ delta) {
        segcount++;
      }
    } counter;

    to_ranges (+it, counter);
    ಪ್ರತ್ಯಾಗಮ counter.segcount;
  }


  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  ತಾರ್ಕಿಕ serialize_start_end_delta_arrays (hb_serialize_context_t *c,
                                         Iterator it,
                                         ಪೂರ್ಣಾಙ್ಕ segcount)
  {
    ಸಂರಚನಾ Writer {
      hb_serialize_context_t *serializer_;
      HBUINT16* end_code_;
      HBUINT16* start_code_;
      HBINT16* id_delta_;
      ಪೂರ್ಣಾಙ್ಕ index_;

      Writer(hb_serialize_context_t *serializer)
          : serializer_(serializer),
            end_code_(nullptr),
            start_code_(nullptr),
            id_delta_(nullptr),
            index_ (0) {}
      ಶೂನ್ಯ operator() (hb_codepoint_t start,
                       hb_codepoint_t end,
                       ಪೂರ್ಣಾಙ್ಕ delta) {
        start_code_[index_] = start;
        end_code_[index_] = end;
        id_delta_[index_] = delta;
        index_++;
      }
    } writer(c);

    writer.end_code_ = c->allocate_size<HBUINT16> (HBUINT16::static_size * segcount, ಅಸತ್ಯ);
    (ಶೂನ್ಯ) c->allocate_size<HBUINT16> (2); // padding
    writer.start_code_ = c->allocate_size<HBUINT16> (HBUINT16::static_size * segcount, ಅಸತ್ಯ);
    writer.id_delta_ = c->allocate_size<HBINT16> (HBINT16::static_size * segcount, ಅಸತ್ಯ);

    ಯದಿ (unlikely (!writer.end_code_ || !writer.start_code_ || !writer.id_delta_)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    to_ranges (+it, writer);
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  template<typename Iterator,
          hb_requires (hb_is_iterator (Iterator))>
  HBUINT16* serialize_rangeoffset_glyid (hb_serialize_context_t *c,
                                         Iterator it,
					 HBUINT16 *endCode,
					 HBUINT16 *startCode,
					 HBINT16 *idDelta,
					 unsigned segcount)
  {
    hb_map_t cp_to_gid { it };

    HBUINT16 *idRangeOffset = c->allocate_size<HBUINT16> (HBUINT16::static_size * segcount);
    ಯದಿ (unlikely (!c->check_success (idRangeOffset))) ಪ್ರತ್ಯಾಗಮ nullptr;
    ಯದಿ (unlikely ((ವರ್ಣ *)idRangeOffset - (ವರ್ಣ *)idDelta != (ಪೂರ್ಣಾಙ್ಕ) segcount * (ಪೂರ್ಣಾಙ್ಕ) HBINT16::static_size)) ಪ್ರತ್ಯಾಗಮ nullptr;

    ಪ್ರತಿ (unsigned i : hb_range (segcount))
    {
      /* A non-zero delta is always a real run. A zero delta is
       * ambiguous: identity mapping, or the marker for a range that
       * needs a glyphIdArray. Only zero deltas need a check. */
      ಪೂರ್ಣಾಙ್ಕ delta = idDelta[i];
      ಯದಿ (delta)
	ಅನುವರ್ತ;
      ತಾರ್ಕಿಕ use_delta = ಸತ್ಯ;
      ಪ್ರತಿ (hb_codepoint_t cp = startCode[i]; cp <= endCode[i]; cp++)
      {
	hb_codepoint_t gid = cp_to_gid.get (cp);
	ಯದಿ (gid == HB_MAP_ಮೌಲ್ಯ_INVALID)
	  gid = 0;
	ಯದಿ (gid != cp)
	{
	  use_delta = ಅಸತ್ಯ;
	  ಭಙ್ಗ;
	}
      }
      ಯದಿ (use_delta)
	ಅನುವರ್ತ;

      idRangeOffset[i] = 2 * (c->start_embed<HBUINT16> () - idRangeOffset - i);
      ಪ್ರತಿ (hb_codepoint_t cp = startCode[i]; cp <= endCode[i]; cp++)
      {
        HBUINT16 gid;
        gid = cp_to_gid[cp];
        c->copy<HBUINT16> (gid);
      }
    }

    ಪ್ರತ್ಯಾಗಮ idRangeOffset;
  }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  ಶೂನ್ಯ serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    auto format4_iter =
    + it
    | hb_filter ([&] (ಧ್ರುವ hb_codepoint_pair_t _)
		 { ಪ್ರತ್ಯಾಗಮ _.first <= 0xFFFF; })
    ;

    ಯದಿ (!format4_iter) ಪ್ರತ್ಯಾಗಮ;

    unsigned table_initpos = c->length ();
    ಯದಿ (unlikely (!c->extend_min (ಏತತ್))) ಪ್ರತ್ಯಾಗಮ;
    ಏತತ್->format = 4;

    hb_vector_t<hb_codepoint_pair_t> cp_to_gid {
      format4_iter
    };

    //serialize endCode[], startCode[], idDelta[]
    HBUINT16* endCode = c->start_embed<HBUINT16> ();
    unsigned segcount = serialize_find_segcount (cp_to_gid.iter());
    ಯದಿ (unlikely (!serialize_start_end_delta_arrays (c, cp_to_gid.iter(), segcount)))
      ಪ್ರತ್ಯಾಗಮ;

    HBUINT16 *startCode = endCode + segcount + 1;
    HBINT16 *idDelta = ((HBINT16*)startCode) + segcount;

    HBUINT16 *idRangeOffset = serialize_rangeoffset_glyid (c,
                                                           cp_to_gid.iter (),
                                                           endCode,
                                                           startCode,
                                                           idDelta,
                                                           segcount);
    ಯದಿ (unlikely (!c->check_success (idRangeOffset))) ಪ್ರತ್ಯಾಗಮ;

    ಏತತ್->length = c->length () - table_initpos;
    ಯದಿ ((long long) ಏತತ್->length != (long long) c->length () - table_initpos)
    {
      c->err (HB_SERIALIZE_ERROR_INT_OVERFLOW);
      ಪ್ರತ್ಯಾಗಮ;
    }

    ಏತತ್->seg_ಎಣಿಕೆ_X2 = segcount * 2;
    ಏತತ್->entrySelector = hb_max (1u, hb_bit_storage (segcount)) - 1;
    ಏತತ್->ಹುಡುಕು_Range = 2 * (1u << ಏತತ್->entrySelector);
    ಏತತ್->rangeShift = segcount * 2 > ಏತತ್->ಹುಡುಕು_Range
		       ? 2 * segcount - ಏತತ್->ಹುಡುಕು_Range
		       : 0;
  }

  unsigned get_language () ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ language;
  }

  ಸಂರಚನಾ accelerator_t
  {
    accelerator_t () {}
    accelerator_t (ಧ್ರುವ CmapSubtableFormat4 *subtable) = delete;

    ಶೂನ್ಯ init (ಧ್ರುವ CmapSubtableFormat4 *subtable,
	       unsigned ಪೂರ್ಣಾಙ್ಕ              subtable_data_size)
    {
      seg_ಎಣಿಕೆ_2 = subtable->seg_ಎಣಿಕೆ_X2 / 2;
      end_ಎಣಿಕೆ = subtable->values.arrayZ;
      start_ಎಣಿಕೆ = end_ಎಣಿಕೆ + seg_ಎಣಿಕೆ_2 + 1;
      idDelta = start_ಎಣಿಕೆ + seg_ಎಣಿಕೆ_2;
      idRangeOffset = idDelta + seg_ಎಣಿಕೆ_2;
      glyphIdArray = idRangeOffset + seg_ಎಣಿಕೆ_2;

      unsigned ಪೂರ್ಣಾಙ್ಕ ಮೌಲ್ಯಗಳು_offset = 16 + 8 * seg_ಎಣಿಕೆ_2;
      glyphIdArrayLength = subtable_data_size > ಮೌಲ್ಯಗಳು_offset
			   ? (subtable_data_size - ಮೌಲ್ಯಗಳು_offset) / 2
			   : 0;
    }

    ತಾರ್ಕಿಕ get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) ಧ್ರುವ
    {
      ಸಂರಚನಾ CustomRange
      {
	ಪೂರ್ಣಾಙ್ಕ cmp (hb_codepoint_t k,
		 unsigned distance) ಧ್ರುವ
	{
	  ಯದಿ (k > last) ಪ್ರತ್ಯಾಗಮ +1;
	  ಯದಿ (k < (&last)[distance]/*first*/) ಪ್ರತ್ಯಾಗಮ -1;
	  ಪ್ರತ್ಯಾಗಮ 0;
	}
	HBUINT16 last;
      };

      ಧ್ರುವ HBUINT16 *found = hb_bsearch (codepoint,
					  ಏತತ್->end_ಎಣಿಕೆ,
					  ಏತತ್->seg_ಎಣಿಕೆ_2,
					  ಪರಿಮಾಣ (CustomRange),
					  _hb_cmp_method<hb_codepoint_t, CustomRange, unsigned>,
					  ಏತತ್->seg_ಎಣಿಕೆ_2 + 1);
      ಯದಿ (unlikely (!found))
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      unsigned ಪೂರ್ಣಾಙ್ಕ i = found - end_ಎಣಿಕೆ;

      hb_codepoint_t gid;
      unsigned ಪೂರ್ಣಾಙ್ಕ rangeOffset = ಏತತ್->idRangeOffset[i];
      ಯದಿ (rangeOffset == 0)
	gid = codepoint + ಏತತ್->idDelta[i];
      ಅನ್ಯಥಾ
      {
	/* Somebody has been smoking... */
	unsigned ಪೂರ್ಣಾಙ್ಕ index = rangeOffset / 2 + (codepoint - ಏತತ್->start_ಎಣಿಕೆ[i]) + i - ಏತತ್->seg_ಎಣಿಕೆ_2;
	ಯದಿ (unlikely (index >= ಏತತ್->glyphIdArrayLength))
	  ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
	gid = ಏತತ್->glyphIdArray[index];
	ಯದಿ (unlikely (!gid))
	  ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
	gid += ಏತತ್->idDelta[i];
      }
      gid &= 0xFFFFu;
      ಯದಿ (unlikely (!gid))
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      *glyph = gid;
      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }

    HB_INTERNAL ಸ್ಥಿರ ತಾರ್ಕಿಕ get_glyph_func (ಧ್ರುವ ಶೂನ್ಯ *obj, hb_codepoint_t codepoint, hb_codepoint_t *glyph)
    { ಪ್ರತ್ಯಾಗಮ ((ಧ್ರುವ accelerator_t *) obj)->get_glyph (codepoint, glyph); }

    ಶೂನ್ಯ collect_unicodes (hb_set_t *out) ಧ್ರುವ
    {
      unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = ಏತತ್->seg_ಎಣಿಕೆ_2;
      ಯದಿ (ಎಣಿಕೆ && ಏತತ್->start_ಎಣಿಕೆ[ಎಣಿಕೆ - 1] == 0xFFFFu)
	ಎಣಿಕೆ--; /* Skip sentinel segment. */
      ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
      {
	hb_codepoint_t start = ಏತತ್->start_ಎಣಿಕೆ[i];
	hb_codepoint_t end = ಏತತ್->end_ಎಣಿಕೆ[i];
	unsigned ಪೂರ್ಣಾಙ್ಕ rangeOffset = ಏತತ್->idRangeOffset[i];
        out->add_range(start, end);
	ಯದಿ (rangeOffset == 0)
	{
	  ಪ್ರತಿ (hb_codepoint_t codepoint = start; codepoint <= end; codepoint++)
	  {
	    hb_codepoint_t gid = (codepoint + ಏತತ್->idDelta[i]) & 0xFFFFu;
	    ಯದಿ (unlikely (!gid))
              out->del(codepoint);
	  }
	}
	ಅನ್ಯಥಾ
	{
	  ಪ್ರತಿ (hb_codepoint_t codepoint = start; codepoint <= end; codepoint++)
	  {
	    unsigned ಪೂರ್ಣಾಙ್ಕ index = rangeOffset / 2 + (codepoint - ಏತತ್->start_ಎಣಿಕೆ[i]) + i - ಏತತ್->seg_ಎಣಿಕೆ_2;
	    ಯದಿ (unlikely (index >= ಏತತ್->glyphIdArrayLength))
            {
              out->del_range (codepoint, end);
	      ಭಙ್ಗ;
            }
	    hb_codepoint_t gid = ಏತತ್->glyphIdArray[index];
	    ಯದಿ (unlikely (!gid))
              out->del(codepoint);
	  }
	}
      }
    }

    ಶೂನ್ಯ collect_mapping (hb_set_t *unicodes, /* OUT */
			  hb_map_t *mapping /* OUT */) ಧ್ರುವ
    {
      // TODO(grieger): optimize similar to collect_unicodes
      // (ie. use add_range())
      unsigned ಎಣಿಕೆ = ಏತತ್->seg_ಎಣಿಕೆ_2;
      ಯದಿ (ಎಣಿಕೆ && ಏತತ್->start_ಎಣಿಕೆ[ಎಣಿಕೆ - 1] == 0xFFFFu)
	ಎಣಿಕೆ--; /* Skip sentinel segment. */
      ಪ್ರತಿ (unsigned i = 0; i < ಎಣಿಕೆ; i++)
      {
	hb_codepoint_t start = ಏತತ್->start_ಎಣಿಕೆ[i];
	hb_codepoint_t end = ಏತತ್->end_ಎಣಿಕೆ[i];
	unsigned rangeOffset = ಏತತ್->idRangeOffset[i];
	ಯದಿ (rangeOffset == 0)
	{
	  ಪ್ರತಿ (hb_codepoint_t codepoint = start; codepoint <= end; codepoint++)
	  {
	    hb_codepoint_t gid = (codepoint + ಏತತ್->idDelta[i]) & 0xFFFFu;
	    ಯದಿ (unlikely (!gid))
	      ಅನುವರ್ತ;
	    unicodes->add (codepoint);
	    mapping->set (codepoint, gid);
	  }
	}
	ಅನ್ಯಥಾ
	{
	  ಪ್ರತಿ (hb_codepoint_t codepoint = start; codepoint <= end; codepoint++)
	  {
	    unsigned index = rangeOffset / 2 + (codepoint - ಏತತ್->start_ಎಣಿಕೆ[i]) + i - ಏತತ್->seg_ಎಣಿಕೆ_2;
	    ಯದಿ (unlikely (index >= ಏತತ್->glyphIdArrayLength))
	      ಭಙ್ಗ;
	    hb_codepoint_t gid = ಏತತ್->glyphIdArray[index];
	    ಯದಿ (unlikely (!gid))
	      ಅನುವರ್ತ;
            gid = (gid + ಏತತ್->idDelta[i]) & 0xFFFFu;
	    unicodes->add (codepoint);
	    mapping->set (codepoint, gid);
	  }
	}
      }
    }

    ಧ್ರುವ HBUINT16 *end_ಎಣಿಕೆ;
    ಧ್ರುವ HBUINT16 *start_ಎಣಿಕೆ;
    ಧ್ರುವ HBUINT16 *idDelta;
    ಧ್ರುವ HBUINT16 *idRangeOffset;
    ಧ್ರುವ HBUINT16 *glyphIdArray;
    unsigned ಪೂರ್ಣಾಙ್ಕ seg_ಎಣಿಕೆ_2;
    unsigned ಪೂರ್ಣಾಙ್ಕ glyphIdArrayLength;
  };

  ತಾರ್ಕಿಕ get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph,
		  unsigned ಪೂರ್ಣಾಙ್ಕ subtable_data_size) ಧ್ರುವ
  {
    accelerator_t accel;
    accel.init (ಏತತ್, subtable_data_size);
    ಪ್ರತ್ಯಾಗಮ accel.get_glyph_func (&accel, codepoint, glyph);
  }
  ತಾರ್ಕಿಕ get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ; }

  ಶೂನ್ಯ collect_unicodes (hb_set_t *out, unsigned ಪೂರ್ಣಾಙ್ಕ subtable_data_size) ಧ್ರುವ
  {
    accelerator_t accel;
    accel.init (ಏತತ್, subtable_data_size);
    accel.collect_unicodes (out);
  }
  ಶೂನ್ಯ collect_unicodes (hb_set_t *out) ಧ್ರುವ
  { collect_unicodes (out, length); }

  ಶೂನ್ಯ collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping, /* OUT */
			unsigned ಪೂರ್ಣಾಙ್ಕ subtable_data_size) ಧ್ರುವ
  {
    accelerator_t accel;
    accel.init (ಏತತ್, subtable_data_size);
    accel.collect_mapping (unicodes, mapping);
  }
  ಶೂನ್ಯ collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping /* OUT */) ಧ್ರುವ
  { collect_mapping (unicodes, mapping, length); }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    ಯದಿ (unlikely (!c->check_struct (ಏತತ್)))
      return_trace (ಅಸತ್ಯ);
    hb_barrier ();

    return_trace (c->check_range (values, 2 + 4 * seg_ಎಣಿಕೆ_X2));
  }

  ರಕ್ಷಿತ:
  HBUINT16	format;		/* Format number is set to 4. */
  HBUINT16	length;		/* This is the length in bytes of the
				 * subtable. */
  HBUINT16	language;	/* Ignore. */
  HBUINT16	seg_ಎಣಿಕೆ_X2;	/* 2 x segCount. */
  HBUINT16	ಹುಡುಕು_Range;	/* 2 * (2**floor(log2(segCount))) */
  HBUINT16	entrySelector;	/* log2(searchRange/2) */
  HBUINT16	rangeShift;	/* 2 x segCount - searchRange */

  UnsizedArrayOf<HBUINT16>
		values;
#ಯದಿ 0
  HBUINT16	end_ಎಣಿಕೆ[seg_ಎಣಿಕೆ_2];	/* End characterCode for each segment,
					 * last=0xFFFFu. */
  HBUINT16	reservedPad;		/* Set to 0. */
  HBUINT16	start_ಎಣಿಕೆ[seg_ಎಣಿಕೆ_2];	/* Start character code for each segment. */
  HBINT16		idDelta[seg_ಎಣಿಕೆ_2];	/* Delta for all character codes in segment. */
  HBUINT16	idRangeOffset[seg_ಎಣಿಕೆ_2];/* Offsets into glyphIdArray or 0 */
  UnsizedArrayOf<HBUINT16>
		glyphIdArray;	/* Glyph index array (arbitrary length) */
#endif

  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (14, values);
};

ಸಂರಚನಾ CmapSubtableLongGroup
{
  friend ಸಂರಚನಾ CmapSubtableFormat12;
  friend ಸಂರಚನಾ CmapSubtableFormat13;
  template<typename U>
  friend ಸಂರಚನಾ CmapSubtableLongSegmented;
  friend ಸಂರಚನಾ cmap;

  ಪೂರ್ಣಾಙ್ಕ cmp (hb_codepoint_t codepoint) ಧ್ರುವ
  {
    ಯದಿ (codepoint < startCharCode) ಪ್ರತ್ಯಾಗಮ -1;
    ಯದಿ (codepoint > endCharCode)   ಪ್ರತ್ಯಾಗಮ +1;
    ಪ್ರತ್ಯಾಗಮ 0;
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್));
  }

  ಗುಪ್ತ:
  HBUINT32		startCharCode;	/* First character code in this group. */
  HBUINT32		endCharCode;	/* Last character code in this group. */
  HBUINT32		glyphID;	/* Glyph index; interpretation depends on
					 * subtable format. */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (12);
};
DECLARE_NULL_NAMESPACE_BYTES (OT, CmapSubtableLongGroup);

template <typename UINT>
ಸಂರಚನಾ CmapSubtableTrimmed
{
  ತಾರ್ಕಿಕ get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) ಧ್ರುವ
  {
    /* Rely on our implicit array bound-checking. */
    hb_codepoint_t gid = glyphIdArray[codepoint - startCharCode];
    ಯದಿ (unlikely (!gid))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    *glyph = gid;
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  unsigned get_language () ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ language;
  }

  ಶೂನ್ಯ collect_unicodes (hb_set_t *out) ಧ್ರುವ
  {
    hb_codepoint_t start = startCharCode;
    unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = glyphIdArray.len;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
      ಯದಿ (glyphIdArray[i])
	out->add (start + i);
  }

  ಶೂನ್ಯ collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping /* OUT */) ಧ್ರುವ
  {
    hb_codepoint_t start_cp = startCharCode;
    unsigned ಎಣಿಕೆ = glyphIdArray.len;
    ಪ್ರತಿ (unsigned i = 0; i < ಎಣಿಕೆ; i++)
      ಯದಿ (glyphIdArray[i])
      {
	hb_codepoint_t unicode = start_cp + i;
	hb_codepoint_t glyphid = glyphIdArray[i];
	unicodes->add (unicode);
	mapping->set (unicode, glyphid);
      }
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) && glyphIdArray.sanitize (c));
  }

  ರಕ್ಷಿತ:
  UINT		formatReserved;	/* Subtable format and (maybe) padding. */
  UINT		length;		/* Byte length of this subtable. */
  UINT		language;	/* Ignore. */
  UINT		startCharCode;	/* First character code covered. */
  ArrayOf<HBGlyphID16, UINT>
		glyphIdArray;	/* Array of glyph index values for character
				 * codes in the range. */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (5 * ಪರಿಮಾಣ (UINT), glyphIdArray);
};

ಸಂರಚನಾ CmapSubtableFormat6  : CmapSubtableTrimmed<HBUINT16> {};
ಸಂರಚನಾ CmapSubtableFormat10 : CmapSubtableTrimmed<HBUINT32> {};

template <typename T>
ಸಂರಚನಾ CmapSubtableLongSegmented
{
  friend ಸಂರಚನಾ cmap;

  ತಾರ್ಕಿಕ get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) ಧ್ರುವ
  {
    hb_codepoint_t gid = T::group_get_glyph (groups.bsearch (codepoint), codepoint);
    ಯದಿ (unlikely (!gid))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    *glyph = gid;
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  unsigned get_language () ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ language;
  }

  ಶೂನ್ಯ collect_unicodes (hb_set_t *out, unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_glyphs) ಧ್ರುವ
  {
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಏತತ್->groups.len; i++)
    {
      hb_codepoint_t start = ಏತತ್->groups[i].startCharCode;
      hb_codepoint_t end = hb_min ((hb_codepoint_t) ಏತತ್->groups[i].endCharCode,
				   (hb_codepoint_t) HB_UNICODE_MAX);
      hb_codepoint_t gid = ಏತತ್->groups[i].glyphID;
      ಯದಿ (!gid)
      {
	/* Intention is: if (hb_is_same (T, CmapSubtableFormat13)) continue; */
	ಯದಿ (! T::group_get_glyph (ಏತತ್->groups[i], end)) ಅನುವರ್ತ;
	start++;
	gid++;
      }
      ಯದಿ (unlikely ((unsigned ಪೂರ್ಣಾಙ್ಕ) gid >= ಅಂಕೆ_glyphs)) ಅನುವರ್ತ;
      ಯದಿ (unlikely ((unsigned ಪೂರ್ಣಾಙ್ಕ) (gid + end - start) >= ಅಂಕೆ_glyphs))
	end = start + (hb_codepoint_t) ಅಂಕೆ_glyphs - gid;

      out->add_range (start, hb_min (end, 0x10FFFFu));
    }
  }

  ಶೂನ್ಯ collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping, /* OUT */
			unsigned ಅಂಕೆ_glyphs) ಧ್ರುವ
  {
    hb_codepoint_t last_end = 0;
    unsigned ಎಣಿಕೆ = ಏತತ್->groups.len;
    ಪ್ರತಿ (unsigned i = 0; i < ಎಣಿಕೆ; i++)
    {
      hb_codepoint_t start = ಏತತ್->groups.arrayZ[i].startCharCode;
      hb_codepoint_t end = hb_min ((hb_codepoint_t) ಏತತ್->groups.arrayZ[i].endCharCode,
				   (hb_codepoint_t) HB_UNICODE_MAX);
      ಯದಿ (unlikely (start > end || start < last_end)) {
        // Range is not in order and is invalid, skip it.
        ಅನುವರ್ತ;
      }
      last_end = end;


      hb_codepoint_t gid = ಏತತ್->groups.arrayZ[i].glyphID;
      ಯದಿ (!gid)
      {
        ಯದಿ (T::format_ಸಂಖ್ಯೆ == 13) ಅನುವರ್ತ;
	start++;
	gid++;
      }
      ಯದಿ (unlikely ((unsigned ಪೂರ್ಣಾಙ್ಕ) gid >= ಅಂಕೆ_glyphs)) ಅನುವರ್ತ;
      ಯದಿ (unlikely ((unsigned ಪೂರ್ಣಾಙ್ಕ) (gid + end - start) >= ಅಂಕೆ_glyphs))
	end = start + (hb_codepoint_t) ಅಂಕೆ_glyphs - gid;

      mapping->alloc (mapping->get_population () + end - start + 1);

      unicodes->add_range (start, end);
      ಪ್ರತಿ (unsigned cp = start; cp <= end; cp++)
      {
	mapping->set (cp, gid);
        gid += T::increment;
      }
    }
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) && groups.sanitize (c));
  }

  ರಕ್ಷಿತ:
  HBUINT16	format;		/* Subtable format; set to 12. */
  HBUINT16	reserved;	/* Reserved; set to 0. */
  HBUINT32	length;		/* Byte length of this subtable. */
  HBUINT32	language;	/* Ignore. */
  SortedArray32Of<CmapSubtableLongGroup>
		groups;		/* Groupings. */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (16, groups);
};

ಸಂರಚನಾ CmapSubtableFormat12 : CmapSubtableLongSegmented<CmapSubtableFormat12>
{
  ಸ್ಥಿರ constexpr ಪೂರ್ಣಾಙ್ಕ increment = 1;
  ಸ್ಥಿರ constexpr ಪೂರ್ಣಾಙ್ಕ format_ಸಂಖ್ಯೆ = 12;

  ಸ್ಥಿರ hb_codepoint_t group_get_glyph (ಧ್ರುವ CmapSubtableLongGroup &group,
					 hb_codepoint_t u)
  { ಪ್ರತ್ಯಾಗಮ likely (group.startCharCode <= group.endCharCode) ?
	   group.glyphID + (u - group.startCharCode) : 0; }


  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  ಶೂನ್ಯ serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    ಯದಿ (!it) ಪ್ರತ್ಯಾಗಮ;
    unsigned table_initpos = c->length ();
    ಯದಿ (unlikely (!c->extend_min (ಏತತ್))) ಪ್ರತ್ಯಾಗಮ;

    hb_codepoint_t startCharCode = (hb_codepoint_t) -1, endCharCode = (hb_codepoint_t) -1;
    hb_codepoint_t glyphID = 0;

    ಪ್ರತಿ (ಧ್ರುವ auto& _ : +it)
    {
      ಯದಿ (startCharCode == (hb_codepoint_t) -1)
      {
	startCharCode = _.first;
	endCharCode = _.first;
	glyphID = _.second;
      }
      ಅನ್ಯಥಾ ಯದಿ (!_is_gid_consecutive (endCharCode, startCharCode, glyphID, _.first, _.second))
      {
	CmapSubtableLongGroup  grouprecord;
	grouprecord.startCharCode = startCharCode;
	grouprecord.endCharCode = endCharCode;
	grouprecord.glyphID = glyphID;
	c->copy<CmapSubtableLongGroup> (grouprecord);

	startCharCode = _.first;
	endCharCode = _.first;
	glyphID = _.second;
      }
      ಅನ್ಯಥಾ
	endCharCode = _.first;
    }

    CmapSubtableLongGroup record;
    record.startCharCode = startCharCode;
    record.endCharCode = endCharCode;
    record.glyphID = glyphID;
    c->copy<CmapSubtableLongGroup> (record);

    ಏತತ್->format = 12;
    ಏತತ್->reserved = 0;
    ಏತತ್->length = c->length () - table_initpos;
    ಏತತ್->groups.len = (ಏತತ್->length - min_size) / CmapSubtableLongGroup::static_size;
  }

  ಸ್ಥಿರ size_t get_sub_table_size (ಧ್ರುವ hb_sorted_vector_t<CmapSubtableLongGroup> &groups_data)
  { ಪ್ರತ್ಯಾಗಮ 16 + 12 * groups_data.length; }

  ಗುಪ್ತ:
  ಸ್ಥಿರ ತಾರ್ಕಿಕ _is_gid_consecutive (hb_codepoint_t endCharCode,
				   hb_codepoint_t startCharCode,
				   hb_codepoint_t glyphID,
				   hb_codepoint_t cp,
				   hb_codepoint_t new_gid)
  {
    ಪ್ರತ್ಯಾಗಮ (cp - 1 == endCharCode) &&
	new_gid == glyphID + (cp - startCharCode);
  }

};

ಸಂರಚನಾ CmapSubtableFormat13 : CmapSubtableLongSegmented<CmapSubtableFormat13>
{
  ಸ್ಥಿರ constexpr ಪೂರ್ಣಾಙ್ಕ increment = 0;
  ಸ್ಥಿರ constexpr ಪೂರ್ಣಾಙ್ಕ format_ಸಂಖ್ಯೆ = 13;

  ಸ್ಥಿರ hb_codepoint_t group_get_glyph (ಧ್ರುವ CmapSubtableLongGroup &group,
					 hb_codepoint_t u HB_UNUSED)
  { ಪ್ರತ್ಯಾಗಮ group.glyphID; }
};

ಪ್ರಕಾರನಿರ್ದೇಶ ಗಣನಾ
{
  GLYPH_VARIANT_NOT_FOUND = 0,
  GLYPH_VARIANT_FOUND = 1,
  GLYPH_VARIANT_USE_DEFAULT = 2
} glyph_variant_t;

ಸಂರಚನಾ Unicode_ಮೌಲ್ಯ_Range
{
  ಪೂರ್ಣಾಙ್ಕ cmp (ಧ್ರುವ hb_codepoint_t &codepoint) ಧ್ರುವ
  {
    ಯದಿ (codepoint < start_Unicode_ಮೌಲ್ಯ) ಪ್ರತ್ಯಾಗಮ -1;
    ಯದಿ (codepoint > start_Unicode_ಮೌಲ್ಯ + additional_ಎಣಿಕೆ) ಪ್ರತ್ಯಾಗಮ +1;
    ಪ್ರತ್ಯಾಗಮ 0;
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್));
  }

  HBUINT24	start_Unicode_ಮೌಲ್ಯ;	/* First value in this range. */
  HBUINT8	additional_ಎಣಿಕೆ;	/* Number of additional values in this
					 * range. */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (4);
};

ಸಂರಚನಾ DefaultUVS : SortedArray32Of<Unicode_ಮೌಲ್ಯ_Range>
{
  ಶೂನ್ಯ collect_unicodes (hb_set_t *out) ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = len;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
    {
      hb_codepoint_t first = arrayZ[i].start_Unicode_ಮೌಲ್ಯ;
      hb_codepoint_t last = hb_min ((hb_codepoint_t) (first + arrayZ[i].additional_ಎಣಿಕೆ),
				    (hb_codepoint_t) HB_UNICODE_MAX);
      out->add_range (first, last);
    }
  }

  DefaultUVS* copy (hb_serialize_context_t *c,
		    ಧ್ರುವ hb_set_t *unicodes) ಧ್ರುವ
  {
    auto *out = c->start_embed<DefaultUVS> ();
    auto snap = c->snapshot ();

    HBUINT32 len;
    len = 0;
    ಯದಿ (unlikely (!c->copy<HBUINT32> (len))) ಪ್ರತ್ಯಾಗಮ nullptr;
    unsigned init_len = c->length ();

    ಯದಿ (ಏತತ್->len > unicodes->get_population () * hb_bit_storage ((unsigned) ಏತತ್->len))
    {
      hb_codepoint_t start = HB_SET_ಮೌಲ್ಯ_INVALID;
      hb_codepoint_t end = HB_SET_ಮೌಲ್ಯ_INVALID;

      ಪ್ರತಿ (auto u : *unicodes)
      {
        ಯದಿ (!as_array ().bsearch (u))
	  ಅನುವರ್ತ;
	ಯದಿ (start == HB_SET_ಮೌಲ್ಯ_INVALID)
	{
	  start = u;
	  end = start - 1;
	}
	ಯದಿ (end + 1 != u || end - start == 255)
        {
	  Unicode_ಮೌಲ್ಯ_Range rec;
	  rec.start_Unicode_ಮೌಲ್ಯ = start;
	  rec.additional_ಎಣಿಕೆ = end - start;
	  c->copy<Unicode_ಮೌಲ್ಯ_Range> (rec);
	  start = u;
	}
	end = u;
      }
      ಯದಿ (start != HB_SET_ಮೌಲ್ಯ_INVALID)
      {
	Unicode_ಮೌಲ್ಯ_Range rec;
	rec.start_Unicode_ಮೌಲ್ಯ = start;
	rec.additional_ಎಣಿಕೆ = end - start;
	c->copy<Unicode_ಮೌಲ್ಯ_Range> (rec);
      }

    }
    ಅನ್ಯಥಾ
    {
      hb_codepoint_t lastCode = HB_SET_ಮೌಲ್ಯ_INVALID;
      ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = -1;

      ಪ್ರತಿ (ಧ್ರುವ Unicode_ಮೌಲ್ಯ_Range& _ : *ಏತತ್)
      {
	hb_codepoint_t curEntry = (hb_codepoint_t) (_.start_Unicode_ಮೌಲ್ಯ - 1);
	hb_codepoint_t end = curEntry + _.additional_ಎಣಿಕೆ + 2;

	ಪ್ರತಿ (; unicodes->next (&curEntry) && curEntry < end;)
	{
	  ಎಣಿಕೆ += 1;
	  ಯದಿ (lastCode == HB_SET_ಮೌಲ್ಯ_INVALID)
	    lastCode = curEntry;
	  ಅನ್ಯಥಾ ಯದಿ (lastCode + ಎಣಿಕೆ != curEntry || ಎಣಿಕೆ > 255)
	  {
	    Unicode_ಮೌಲ್ಯ_Range rec;
	    rec.start_Unicode_ಮೌಲ್ಯ = lastCode;
	    rec.additional_ಎಣಿಕೆ = ಎಣಿಕೆ - 1;
	    c->copy<Unicode_ಮೌಲ್ಯ_Range> (rec);

	    lastCode = curEntry;
	    ಎಣಿಕೆ = 0;
	  }
	}
      }

      ಯದಿ (lastCode != HB_MAP_ಮೌಲ್ಯ_INVALID)
      {
	Unicode_ಮೌಲ್ಯ_Range rec;
	rec.start_Unicode_ಮೌಲ್ಯ = lastCode;
	rec.additional_ಎಣಿಕೆ = ಎಣಿಕೆ;
	c->copy<Unicode_ಮೌಲ್ಯ_Range> (rec);
      }
    }

    ಯದಿ (c->length () - init_len == 0)
    {
      c->revert (snap);
      ಪ್ರತ್ಯಾಗಮ nullptr;
    }
    ಅನ್ಯಥಾ
    {
      ಯದಿ (unlikely (!c->check_assign (out->len,
                                      (c->length () - init_len) / Unicode_ಮೌಲ್ಯ_Range::static_size,
                                      HB_SERIALIZE_ERROR_INT_OVERFLOW))) ಪ್ರತ್ಯಾಗಮ nullptr;
      ಪ್ರತ್ಯಾಗಮ out;
    }
  }

  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (4, *ಏತತ್);
};

template <typename Types>
ಸಂರಚನಾ UVSMapping
{
  using GlyphID = typename Types::HBGlyphID;

  ಪೂರ್ಣಾಙ್ಕ cmp (ಧ್ರುವ hb_codepoint_t &codepoint) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ unicode_ಮೌಲ್ಯ.cmp (codepoint); }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್));
  }

  HBUINT24	unicode_ಮೌಲ್ಯ;	/* Base Unicode value of the UVS */
  GlyphID
		glyphID;	/* Glyph ID of the UVS. */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (3 + GlyphID::static_size);
};

template <typename Types>
ಸಂರಚನಾ NonDefaultUVS : SortedArray32Of<UVSMapping<Types>>
{
  using Mapping = UVSMapping<Types>;

  ಶೂನ್ಯ collect_unicodes (hb_set_t *out) ಧ್ರುವ
  {
    ಪ್ರತಿ (ಧ್ರುವ auto& a : ಏತತ್->as_array ())
      out->add (a.unicode_ಮೌಲ್ಯ);
  }

  ಶೂನ್ಯ collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping /* OUT */) ಧ್ರುವ
  {
    ಪ್ರತಿ (ಧ್ರುವ auto& a : ಏತತ್->as_array ())
    {
      hb_codepoint_t unicode = a.unicode_ಮೌಲ್ಯ;
      hb_codepoint_t glyphid = a.glyphID;
      unicodes->add (unicode);
      mapping->set (unicode, glyphid);
    }
  }

  ಶೂನ್ಯ closure_glyphs (ಧ್ರುವ hb_set_t      *unicodes,
		       hb_set_t            *glyphset) ಧ್ರುವ
  {
    + ಏತತ್->as_array ()
    | hb_filter (unicodes, &Mapping::unicode_ಮೌಲ್ಯ)
    | hb_map (&Mapping::glyphID)
    | hb_sink (glyphset)
    ;
  }

  NonDefaultUVS* copy (hb_serialize_context_t *c,
		       ಧ್ರುವ hb_set_t *unicodes,
		       ಧ್ರುವ hb_set_t *glyphs_requested,
		       ಧ್ರುವ hb_map_t *glyph_map) ಧ್ರುವ
  {
    auto *out = c->start_embed<NonDefaultUVS> ();
    auto it =
    + ಏತತ್->as_array ()
    | hb_filter ([&] (ಧ್ರುವ Mapping& _)
		 {
		   ಪ್ರತ್ಯಾಗಮ unicodes->has (_.unicode_ಮೌಲ್ಯ) || glyphs_requested->has (_.glyphID);
		 })
    ;

    ಯದಿ (!it) ಪ್ರತ್ಯಾಗಮ nullptr;

    HBUINT32 len;
    len = it.len ();
    ಯದಿ (unlikely (!c->copy<HBUINT32> (len))) ಪ್ರತ್ಯಾಗಮ nullptr;

    ಪ್ರತಿ (ಧ್ರುವ Mapping& _ : it)
    {
      Mapping mapping;
      mapping.unicode_ಮೌಲ್ಯ = _.unicode_ಮೌಲ್ಯ;
      mapping.glyphID = glyph_map->get (_.glyphID);
      c->copy<Mapping> (mapping);
    }

    ಪ್ರತ್ಯಾಗಮ out;
  }

  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (4, *ಏತತ್);
};

template <typename Types>
ಸಂರಚನಾ VariationSelectorRecord
{
  glyph_variant_t get_glyph (hb_codepoint_t codepoint,
			     hb_codepoint_t *glyph,
			     ಧ್ರುವ ಶೂನ್ಯ *base) ಧ್ರುವ
  {
    ಯದಿ ((base+defaultUVS).bfind (codepoint))
      ಪ್ರತ್ಯಾಗಮ GLYPH_VARIANT_USE_DEFAULT;
    ಧ್ರುವ UVSMapping<Types> &nonDefault = (base+nonDefaultUVS).bsearch (codepoint);
    ಯದಿ (nonDefault.glyphID)
    {
      *glyph = nonDefault.glyphID;
       ಪ್ರತ್ಯಾಗಮ GLYPH_VARIANT_FOUND;
    }
    ಪ್ರತ್ಯಾಗಮ GLYPH_VARIANT_NOT_FOUND;
  }

  VariationSelectorRecord(ಧ್ರುವ VariationSelectorRecord& other)
  {
    *ಏತತ್ = other;
  }

  ಶೂನ್ಯ operator= (ಧ್ರುವ VariationSelectorRecord& other)
  {
    varSelector = other.varSelector;
    HBUINT32 offset = other.defaultUVS;
    defaultUVS = offset;
    offset = other.nonDefaultUVS;
    nonDefaultUVS = offset;
  }

  ಶೂನ್ಯ collect_unicodes (hb_set_t *out, ಧ್ರುವ ಶೂನ್ಯ *base) ಧ್ರುವ
  {
    (base+defaultUVS).collect_unicodes (out);
    (base+nonDefaultUVS).collect_unicodes (out);
  }

  ಶೂನ್ಯ collect_mapping (ಧ್ರುವ ಶೂನ್ಯ *base,
			hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping /* OUT */) ಧ್ರುವ
  {
    (base+defaultUVS).collect_unicodes (unicodes);
    (base+nonDefaultUVS).collect_mapping (unicodes, mapping);
  }

  ಪೂರ್ಣಾಙ್ಕ cmp (ಧ್ರುವ hb_codepoint_t &variation_selector) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ varSelector.cmp (variation_selector); }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c, ಧ್ರುವ ಶೂನ್ಯ *base) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) &&
		  defaultUVS.sanitize (c, base) &&
		  nonDefaultUVS.sanitize (c, base));
  }

  hb_pair_t<unsigned, unsigned>
  copy (hb_serialize_context_t *c,
	ಧ್ರುವ hb_set_t *unicodes,
	ಧ್ರುವ hb_set_t *glyphs_requested,
	ಧ್ರುವ hb_map_t *glyph_map,
	ಧ್ರುವ ಶೂನ್ಯ *base) ಧ್ರುವ
  {
    auto snap = c->snapshot ();
    auto *out = c->embed<VariationSelectorRecord> (*ಏತತ್);
    ಯದಿ (unlikely (!out)) ಪ್ರತ್ಯಾಗಮ hb_pair (0, 0);

    out->defaultUVS = 0;
    out->nonDefaultUVS = 0;

    unsigned non_default_uvs_objidx = 0;
    ಯದಿ (nonDefaultUVS != 0)
    {
      c->push ();
      ಯದಿ (c->copy (base+nonDefaultUVS, unicodes, glyphs_requested, glyph_map))
	non_default_uvs_objidx = c->ತೆಗೆ_pack ();
      ಅನ್ಯಥಾ c->ತೆಗೆ_discard ();
    }

    unsigned default_uvs_objidx = 0;
    ಯದಿ (defaultUVS != 0)
    {
      c->push ();
      ಯದಿ (c->copy (base+defaultUVS, unicodes))
	default_uvs_objidx = c->ತೆಗೆ_pack ();
      ಅನ್ಯಥಾ c->ತೆಗೆ_discard ();
    }


    ಯದಿ (!default_uvs_objidx && !non_default_uvs_objidx)
      c->revert (snap);

    ಪ್ರತ್ಯಾಗಮ hb_pair (default_uvs_objidx, non_default_uvs_objidx);
  }

  HBUINT24	varSelector;	/* Variation selector. */
  Offset32To<DefaultUVS>
		defaultUVS;	/* Offset to Default UVS Table.  May be 0. */
  Offset32To<NonDefaultUVS<Types>>
		nonDefaultUVS;	/* Offset to Non-Default UVS Table.  May be 0. */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (11);
};

template <typename Types>
ಸಂರಚನಾ CmapSubtableFormat14_15
{
  ಸ್ಥಿರ constexpr unsigned format_ಮೌಲ್ಯ = Types::size == 2 ? 14 : 15;
  using Record = VariationSelectorRecord<Types>;
  using NonDefault = NonDefaultUVS<Types>;

  glyph_variant_t get_glyph_variant (hb_codepoint_t codepoint,
				     hb_codepoint_t variation_selector,
				     hb_codepoint_t *glyph) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ record.bsearch (variation_selector).get_glyph (codepoint, glyph, ಏತತ್); }

  ಶೂನ್ಯ collect_variation_selectors (hb_set_t *out) ಧ್ರುವ
  {
    ಪ್ರತಿ (ಧ್ರುವ auto& a : record.as_array ())
      out->add (a.varSelector);
  }
  ಶೂನ್ಯ collect_variation_unicodes (hb_codepoint_t variation_selector,
				   hb_set_t *out) ಧ್ರುವ
  { record.bsearch (variation_selector).collect_unicodes (out, ಏತತ್); }

  ಶೂನ್ಯ serialize (hb_serialize_context_t *c,
		  ಧ್ರುವ hb_set_t *unicodes,
		  ಧ್ರುವ hb_set_t *glyphs_requested,
		  ಧ್ರುವ hb_map_t *glyph_map,
		  ಧ್ರುವ ಶೂನ್ಯ *base)
  {
    auto snap = c->snapshot ();
    unsigned table_initpos = c->length ();
    ಧ್ರುವ ವರ್ಣ* init_tail = c->tail;

    ಯದಿ (unlikely (!c->extend_min (ಏತತ್))) ಪ್ರತ್ಯಾಗಮ;
    ಏತತ್->format = format_ಮೌಲ್ಯ;

    auto src_tbl = reinterpret_cast<ಧ್ರುವ CmapSubtableFormat14_15*> (base);

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
    hb_vector_t<hb_pair_t<unsigned, unsigned>> obj_indices;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = src_tbl->record.len - 1; i >= 0; i--)
    {
      ಯದಿ (!unicodes->has(src_tbl->record[i].varSelector))
        ಅನುವರ್ತ;

      hb_pair_t<unsigned, unsigned> result = src_tbl->record[i].copy (c, unicodes, glyphs_requested, glyph_map, base);
      ಯದಿ (result.first || result.second)
	obj_indices.push (result);
    }

    ಯದಿ (c->length () - table_initpos == CmapSubtableFormat14_15::min_size)
    {
      c->revert (snap);
      ಪ್ರತ್ಯಾಗಮ;
    }

    ಯದಿ (unlikely (!c->check_success (!obj_indices.in_error ())))
      ಪ್ರತ್ಯಾಗಮ;

    ಪೂರ್ಣಾಙ್ಕ tail_len = init_tail - c->tail;
    c->check_assign (ಏತತ್->length, c->length () - table_initpos + tail_len,
                     HB_SERIALIZE_ERROR_INT_OVERFLOW);
    c->check_assign (ಏತತ್->record.len,
		     (c->length () - table_initpos - CmapSubtableFormat14_15::min_size) /
		     Record::static_size,
                     HB_SERIALIZE_ERROR_INT_OVERFLOW);

    /* Correct the incorrect write order by reversing the order of the variation
       records array. */
    _reverse_variation_records ();

    /* Now that records are in the right order, we can set up the offsets. */
    _add_links_to_variation_records (c, obj_indices);
  }

  ಶೂನ್ಯ _reverse_variation_records ()
  {
    record.as_array ().reverse ();
  }

  ಶೂನ್ಯ _add_links_to_variation_records (hb_serialize_context_t *c,
					ಧ್ರುವ hb_vector_t<hb_pair_t<unsigned, unsigned>>& obj_indices)
  {
    ಪ್ರತಿ (unsigned i = 0; i < obj_indices.length; i++)
    {
      /*
       * Since the record array has been reversed (see comments in copy())
       * but obj_indices has not been, the indices at obj_indices[i]
       * are for the variation record at record[j].
       */
      ಪೂರ್ಣಾಙ್ಕ j = obj_indices.length - 1 - i;
      c->add_link (record[j].defaultUVS, obj_indices[i].first);
      c->add_link (record[j].nonDefaultUVS, obj_indices[i].second);
    }
  }

  ಶೂನ್ಯ closure_glyphs (ಧ್ರುವ hb_set_t      *unicodes,
		       hb_set_t            *glyphset) ಧ್ರುವ
  {
    + hb_iter (record)
    | hb_filter (hb_bool, &Record::nonDefaultUVS)
    | hb_filter (unicodes, &Record::varSelector)
    | hb_map (&Record::nonDefaultUVS)
    | hb_map (hb_add (ಏತತ್))
    | hb_apply ([=] (ಧ್ರುವ NonDefault& _) { _.closure_glyphs (unicodes, glyphset); })
    ;
  }

  ಶೂನ್ಯ collect_unicodes (hb_set_t *out) ಧ್ರುವ
  {
    ಪ್ರತಿ (ಧ್ರುವ Record& _ : record)
      _.collect_unicodes (out, ಏತತ್);
  }

  ಶೂನ್ಯ collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping /* OUT */) ಧ್ರುವ
  {
    ಪ್ರತಿ (ಧ್ರುವ Record& _ : record)
      _.collect_mapping (ಏತತ್, unicodes, mapping);
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) &&
		  record.sanitize (c, ಏತತ್));
  }

  ರಕ್ಷಿತ:
  HBUINT16	format;		/* Format number is set to 14 or 15. */
  HBUINT32	length;		/* Byte length of this subtable. */
  SortedArray32Of<Record>
		record;		/* Variation selector records; sorted
				 * in increasing order of `varSelector'. */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (10, record);
};

ಸಂರಚನಾ CmapSubtable
{
  /* Note: We intentionally do NOT implement subtable formats 2 and 8. */

  ತಾರ್ಕಿಕ is_variation_selector () ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ 14:
#ifndef HB_NO_BEYOND_64K
    ಪಕ್ಷ 15:
#endif
      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    ಸ್ವಭಾವ:
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
  }

  glyph_variant_t get_glyph_variant (hb_codepoint_t codepoint,
				     hb_codepoint_t variation_selector,
				     hb_codepoint_t *glyph) ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ 14: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format14.get_glyph_variant (codepoint, variation_selector, glyph);
#ifndef HB_NO_BEYOND_64K
    ಪಕ್ಷ 15: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format15.get_glyph_variant (codepoint, variation_selector, glyph);
#endif
    ಸ್ವಭಾವ: ಪ್ರತ್ಯಾಗಮ GLYPH_VARIANT_NOT_FOUND;
    }
  }

  ಶೂನ್ಯ collect_variation_selectors (hb_set_t *out) ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ 14: hb_barrier (); u.format14.collect_variation_selectors (out); ಪ್ರತ್ಯಾಗಮ;
#ifndef HB_NO_BEYOND_64K
    ಪಕ್ಷ 15: hb_barrier (); u.format15.collect_variation_selectors (out); ಪ್ರತ್ಯಾಗಮ;
#endif
    ಸ್ವಭಾವ: ಪ್ರತ್ಯಾಗಮ;
    }
  }

  ಶೂನ್ಯ collect_variation_unicodes (hb_codepoint_t variation_selector,
				    hb_set_t *out) ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ 14: hb_barrier (); u.format14.collect_variation_unicodes (variation_selector, out); ಪ್ರತ್ಯಾಗಮ;
#ifndef HB_NO_BEYOND_64K
    ಪಕ್ಷ 15: hb_barrier (); u.format15.collect_variation_unicodes (variation_selector, out); ಪ್ರತ್ಯಾಗಮ;
#endif
    ಸ್ವಭಾವ: ಪ್ರತ್ಯಾಗಮ;
    }
  }

  ಶೂನ್ಯ closure_glyphs (ಧ್ರುವ hb_set_t *unicodes, hb_set_t *glyphset) ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ 14: hb_barrier (); u.format14.closure_glyphs (unicodes, glyphset); ಪ್ರತ್ಯಾಗಮ;
#ifndef HB_NO_BEYOND_64K
    ಪಕ್ಷ 15: hb_barrier (); u.format15.closure_glyphs (unicodes, glyphset); ಪ್ರತ್ಯಾಗಮ;
#endif
    ಸ್ವಭಾವ: ಪ್ರತ್ಯಾಗಮ;
    }
  }

  ತಾರ್ಕಿಕ get_glyph (hb_codepoint_t codepoint,
		  hb_codepoint_t *glyph,
		  unsigned ಪೂರ್ಣಾಙ್ಕ subtable_data_size = 0) ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ  0: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format0 .get_glyph (codepoint, glyph);
    ಪಕ್ಷ  4: hb_barrier (); ಪ್ರತ್ಯಾಗಮ subtable_data_size
				      ? u.format4.get_glyph (codepoint, glyph, subtable_data_size)
				      : ಅಸತ್ಯ;
    ಪಕ್ಷ  6: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format6 .get_glyph (codepoint, glyph);
    ಪಕ್ಷ 10: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format10.get_glyph (codepoint, glyph);
    ಪಕ್ಷ 12: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format12.get_glyph (codepoint, glyph);
    ಪಕ್ಷ 13: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format13.get_glyph (codepoint, glyph);
    ಪಕ್ಷ 14:
    ಸ್ವಭಾವ: ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
  }
  ಶೂನ್ಯ collect_unicodes (hb_set_t *out,
			 unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_glyphs = UINT_MAX,
			 unsigned ಪೂರ್ಣಾಙ್ಕ subtable_data_size = 0) ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ  0: hb_barrier (); u.format0 .collect_unicodes (out); ಪ್ರತ್ಯಾಗಮ;
    ಪಕ್ಷ  4: hb_barrier (); ಯದಿ (subtable_data_size)
			      u.format4.collect_unicodes (out, subtable_data_size);
			    ಅನ್ಯಥಾ
			      u.format4.collect_unicodes (out);
			    ಪ್ರತ್ಯಾಗಮ;
    ಪಕ್ಷ  6: hb_barrier (); u.format6 .collect_unicodes (out); ಪ್ರತ್ಯಾಗಮ;
    ಪಕ್ಷ 10: hb_barrier (); u.format10.collect_unicodes (out); ಪ್ರತ್ಯಾಗಮ;
    ಪಕ್ಷ 12: hb_barrier (); u.format12.collect_unicodes (out, ಅಂಕೆ_glyphs); ಪ್ರತ್ಯಾಗಮ;
    ಪಕ್ಷ 13: hb_barrier (); u.format13.collect_unicodes (out, ಅಂಕೆ_glyphs); ಪ್ರತ್ಯಾಗಮ;
    ಪಕ್ಷ 14:
    ಸ್ವಭಾವ: ಪ್ರತ್ಯಾಗಮ;
    }
  }

  ಶೂನ್ಯ collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping, /* OUT */
			unsigned ಅಂಕೆ_glyphs = UINT_MAX,
			unsigned ಪೂರ್ಣಾಙ್ಕ subtable_data_size = 0) ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ  0: hb_barrier (); u.format0 .collect_mapping (unicodes, mapping); ಪ್ರತ್ಯಾಗಮ;
    ಪಕ್ಷ  4: hb_barrier (); ಯದಿ (subtable_data_size)
			      u.format4.collect_mapping (unicodes, mapping, subtable_data_size);
			    ಅನ್ಯಥಾ
			      u.format4.collect_mapping (unicodes, mapping);
			    ಪ್ರತ್ಯಾಗಮ;
    ಪಕ್ಷ  6: hb_barrier (); u.format6 .collect_mapping (unicodes, mapping); ಪ್ರತ್ಯಾಗಮ;
    ಪಕ್ಷ 10: hb_barrier (); u.format10.collect_mapping (unicodes, mapping); ಪ್ರತ್ಯಾಗಮ;
    ಪಕ್ಷ 12: hb_barrier (); u.format12.collect_mapping (unicodes, mapping, ಅಂಕೆ_glyphs); ಪ್ರತ್ಯಾಗಮ;
    ಪಕ್ಷ 13: hb_barrier (); u.format13.collect_mapping (unicodes, mapping, ಅಂಕೆ_glyphs); ಪ್ರತ್ಯಾಗಮ;
    ಪಕ್ಷ 14:
    ಸ್ವಭಾವ: ಪ್ರತ್ಯಾಗಮ;
    }
  }

  unsigned get_language () ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ  0: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format0 .get_language ();
    ಪಕ್ಷ  4: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format4 .get_language ();
    ಪಕ್ಷ  6: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format6 .get_language ();
    ಪಕ್ಷ 10: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format10.get_language ();
    ಪಕ್ಷ 12: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format12.get_language ();
    ಪಕ್ಷ 13: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format13.get_language ();
    ಪಕ್ಷ 14:
    ಸ್ವಭಾವ: ಪ್ರತ್ಯಾಗಮ 0;
    }
  }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  ಶೂನ್ಯ serialize (hb_serialize_context_t *c,
		  Iterator it,
		  unsigned format,
		  ಧ್ರುವ hb_subset_plan_t *plan,
		  ಧ್ರುವ ಶೂನ್ಯ *base)
  {
    ವಿಕಲ್ಪ (format) {
    ಪಕ್ಷ  4: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format4.serialize (c, it);
    ಪಕ್ಷ 12: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format12.serialize (c, it);
    ಪಕ್ಷ 14: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format14.serialize (c, &plan->unicodes, &plan->glyphs_requested, plan->glyph_map, base);
#ifndef HB_NO_BEYOND_64K
    ಪಕ್ಷ 15: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format15.serialize (c, &plan->unicodes, &plan->glyphs_requested, plan->glyph_map, base);
#endif
    ಸ್ವಭಾವ: ಪ್ರತ್ಯಾಗಮ;
    }
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    ಯದಿ (!u.format.v.sanitize (c)) return_trace (ಅಸತ್ಯ);
    hb_barrier ();
    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ  0: hb_barrier (); return_trace (u.format0 .sanitize (c));
    ಪಕ್ಷ  4: hb_barrier (); return_trace (u.format4 .sanitize (c));
    ಪಕ್ಷ  6: hb_barrier (); return_trace (u.format6 .sanitize (c));
    ಪಕ್ಷ 10: hb_barrier (); return_trace (u.format10.sanitize (c));
    ಪಕ್ಷ 12: hb_barrier (); return_trace (u.format12.sanitize (c));
    ಪಕ್ಷ 13: hb_barrier (); return_trace (u.format13.sanitize (c));
    ಪಕ್ಷ 14: hb_barrier (); return_trace (u.format14.sanitize (c));
#ifndef HB_NO_BEYOND_64K
    ಪಕ್ಷ 15: hb_barrier (); return_trace (u.format15.sanitize (c));
#endif
    ಸ್ವಭಾವ:return_trace (ಸತ್ಯ);
    }
  }

  ಸಾರ್ವಜನಿಕ:
  ಸಙ್ಘ {
  ಸಂರಚನಾ { HBUINT16 v; }	format;		/* Format identifier */
  CmapSubtableFormat0	format0;
  CmapSubtableFormat4	format4;
  CmapSubtableFormat6	format6;
  CmapSubtableFormat10	format10;
  CmapSubtableFormat12	format12;
  CmapSubtableFormat13	format13;
  CmapSubtableFormat14_15<Layout::SmallTypes>
			format14;
#ifndef HB_NO_BEYOND_64K
  CmapSubtableFormat14_15<Layout::MediumTypes>
			format15;
#endif
  } u;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_UNION (2, format.v);
};


ಸಂರಚನಾ EncodingRecord
{
  ಪೂರ್ಣಾಙ್ಕ cmp (ಧ್ರುವ EncodingRecord &other) ಧ್ರುವ
  {
    ಪೂರ್ಣಾಙ್ಕ ret;
    ret = platformID.cmp (other.platformID);
    ಯದಿ (ret) ಪ್ರತ್ಯಾಗಮ ret;
    ಯದಿ (other.encodingID != 0xFFFF)
    {
      ret = encodingID.cmp (other.encodingID);
      ಯದಿ (ret) ಪ್ರತ್ಯಾಗಮ ret;
    }
    ಪ್ರತ್ಯಾಗಮ 0;
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c, ಧ್ರುವ ಶೂನ್ಯ *base) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) &&
		  subtable.sanitize (c, base));
  }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  EncodingRecord* copy (hb_serialize_context_t *c,
			Iterator it,
			unsigned format,
			ಧ್ರುವ ಶೂನ್ಯ *base,
			ಧ್ರುವ hb_subset_plan_t *plan,
			/* INOUT */ unsigned *objidx) ಧ್ರುವ
  {
    TRACE_SERIALIZE (ಏತತ್);
    auto snap = c->snapshot ();
    auto *out = c->embed (ಏತತ್);
    ಯದಿ (unlikely (!out)) return_trace (nullptr);
    out->subtable = 0;

    ಯದಿ (*objidx == 0)
    {
      CmapSubtable *cmapsubtable = c->push<CmapSubtable> ();
      unsigned origin_length = c->length ();
      cmapsubtable->serialize (c, it, format, plan, &(base+subtable));
      ಯದಿ (c->length () > origin_length && !c->in_error()) *objidx = c->ತೆಗೆ_pack ();
      ಅನ್ಯಥಾ c->ತೆಗೆ_discard ();
    }

    ಯದಿ (*objidx == 0)
    {
      c->revert (snap);
      return_trace (nullptr);
    }

    c->add_link (out->subtable, *objidx);
    return_trace (out);
  }

  HBUINT16	platformID;	/* Platform ID. */
  HBUINT16	encodingID;	/* Platform-specific encoding ID. */
  Offset32To<CmapSubtable>
		subtable;	/* Byte offset from beginning of table to the subtable for this encoding. */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (8);
};

ಸಂರಚನಾ cmap;

ಸಂರಚನಾ SubtableUnicodesCache {

 ಗುಪ್ತ:
  hb_blob_ptr_t<cmap> base_blob;
  ಧ್ರುವ ವರ್ಣ* base;
  unsigned ಪೂರ್ಣಾಙ್ಕ table_length;
  hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> cached_unicodes;

 ಸಾರ್ವಜನಿಕ:

  ಸ್ಥಿರ SubtableUnicodesCache* create (hb_blob_ptr_t<cmap> source_table)
  {
    SubtableUnicodesCache* cache =
        (SubtableUnicodesCache*) hb_malloc (ಪರಿಮಾಣ(SubtableUnicodesCache));

    ಯದಿ (unlikely (!cache))
      ಪ್ರತ್ಯಾಗಮ nullptr;

    ನೂತನ (cache) SubtableUnicodesCache (source_table);
    ಪ್ರತ್ಯಾಗಮ cache;
  }

  ಸ್ಥಿರ ಶೂನ್ಯ destroy (ಶೂನ್ಯ* value) {
    ಯದಿ (!value) ಪ್ರತ್ಯಾಗಮ;

    SubtableUnicodesCache* cache = (SubtableUnicodesCache*) value;
    cache->~SubtableUnicodesCache ();
    hb_free (cache);
  }

  SubtableUnicodesCache(ಧ್ರುವ ಶೂನ್ಯ* cmap_base,
			unsigned ಪೂರ್ಣಾಙ್ಕ table_length_ = 0)
      : base_blob(),
        base ((ಧ್ರುವ ವರ್ಣ*) cmap_base),
        table_length (table_length_),
        cached_unicodes ()
  {}

  SubtableUnicodesCache(hb_blob_ptr_t<cmap> base_blob_)
      : base_blob(base_blob_),
        base ((ಧ್ರುವ ವರ್ಣ *) base_blob.get()),
        table_length (base_blob.get_length ()),
        cached_unicodes ()
  {}

  ~SubtableUnicodesCache()
  {
    base_blob.destroy ();
  }

  ತಾರ್ಕಿಕ same_base(ಧ್ರುವ ಶೂನ್ಯ* other) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ other == (ಧ್ರುವ ಶೂನ್ಯ*) base;
  }

  ಧ್ರುವ hb_set_t* set_for (ಧ್ರುವ EncodingRecord* record,
                           SubtableUnicodesCache& mutable_cache) ಧ್ರುವ
  {
    ಯದಿ (cached_unicodes.has ((unsigned) ((ಧ್ರುವ ವರ್ಣ *) record - base)))
      ಪ್ರತ್ಯಾಗಮ cached_unicodes.get ((unsigned) ((ಧ್ರುವ ವರ್ಣ *) record - base));

    ಪ್ರತ್ಯಾಗಮ mutable_cache.set_for (record);
  }

  ಧ್ರುವ hb_set_t* set_for (ಧ್ರುವ EncodingRecord* record)
  {
    ಯದಿ (!cached_unicodes.has ((unsigned) ((ಧ್ರುವ ವರ್ಣ *) record - base)))
    {
      hb_set_t *s = hb_set_create ();
      ಯದಿ (unlikely (s->in_error ()))
	ಪ್ರತ್ಯಾಗಮ hb_set_get_empty ();

      unsigned ಪೂರ್ಣಾಙ್ಕ subtable_data_size = record->subtable < table_length
					? table_length - (unsigned ಪೂರ್ಣಾಙ್ಕ) record->subtable
					: 0;
      (base+record->subtable).collect_unicodes (s, UINT_MAX, subtable_data_size);

      ಯದಿ (unlikely (!cached_unicodes.set ((unsigned) ((ಧ್ರುವ ವರ್ಣ *) record - base), hb::unique_ptr<hb_set_t> {s})))
        ಪ್ರತ್ಯಾಗಮ hb_set_get_empty ();

      ಪ್ರತ್ಯಾಗಮ s;
    }
    ಪ್ರತ್ಯಾಗಮ cached_unicodes.get ((unsigned) ((ಧ್ರುವ ವರ್ಣ *) record - base));
  }

};

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ uint16_t
hb_ಚಿಹ್ನೆ_pua_map (unsigned codepoint)
{
  ಯದಿ (codepoint <= 0x00FFu)
  {
    /* For symbol-encoded OpenType fonts, we duplicate the
     * U+F000..F0FF range at U+0000..U+00FF.  That's what
     * Windows seems to do, and that's hinted about at:
     * https://docs.microsoft.com/en-us/typography/opentype/spec/recom
     * under "Non-Standard (Symbol) Fonts". */
    ಪ್ರತ್ಯಾಗಮ 0xF000u + codepoint;
  }
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸಂರಚನಾ cmap
{
  ಸ್ಥಿರ constexpr hb_tag_t tableTag = HB_OT_TAG_cmap;


  ಸ್ಥಿರ SubtableUnicodesCache* create_filled_cache(hb_blob_ptr_t<cmap> source_table) {
    ಧ್ರುವ cmap* cmap = source_table.get();
    auto it =
    + hb_iter (cmap->encodingRecord)
    | hb_filter ([&](ಧ್ರುವ EncodingRecord& _) {
      ಪ್ರತ್ಯಾಗಮ cmap::filter_encoding_records_for_subset (cmap, _);
    })
    ;

    SubtableUnicodesCache* cache = SubtableUnicodesCache::create(source_table);

    ಯದಿ (unlikely (!cache))
      ಪ್ರತ್ಯಾಗಮ nullptr;

    ಪ್ರತಿ (ಧ್ರುವ EncodingRecord& _ : it)
      cache->set_for(&_); // populate the cache for this encoding record.

    ಪ್ರತ್ಯಾಗಮ cache;
  }

  template<typename Iterator, typename EncodingRecIter,
	   hb_requires (hb_is_iterator (EncodingRecIter))>
  ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c,
		  Iterator it,
		  EncodingRecIter encodingrec_iter,
		  ಧ್ರುವ ಶೂನ್ಯ *base,
		  hb_subset_plan_t *plan,
	                  ತಾರ್ಕಿಕ drop_format_4 = ಅಸತ್ಯ,
			  unsigned ಪೂರ್ಣಾಙ್ಕ source_table_length = 0)
  {
    ಯದಿ (unlikely (!c->extend_min ((*ಏತತ್))))  ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಏತತ್->version = 0;

    unsigned format4objidx = 0, format12objidx = 0, format14objidx = 0;
#ifndef HB_NO_BEYOND_64K
    unsigned format15objidx = 0;
#endif
    auto snap = c->snapshot ();

    SubtableUnicodesCache local_unicodes_cache (base, source_table_length);
    ಧ್ರುವ SubtableUnicodesCache* unicodes_cache = &local_unicodes_cache;

    ಯದಿ (plan->accelerator &&
        plan->accelerator->cmap_cache &&
        plan->accelerator->cmap_cache->same_base (base))
      unicodes_cache = plan->accelerator->cmap_cache;

    ಪ್ರತಿ (ಧ್ರುವ EncodingRecord& _ : encodingrec_iter)
    {
      ಯದಿ (c->in_error ())
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

      unsigned format = (base+_.subtable).u.format.v;
      ಯದಿ (format != 4 && format != 12 && format != 14
#ifndef HB_NO_BEYOND_64K
	  && format != 15
#endif
	 ) ಅನುವರ್ತ;

      ಧ್ರುವ hb_set_t* unicodes_set = unicodes_cache->set_for (&_, local_unicodes_cache);

      ಯದಿ (!drop_format_4 && format == 4)
      {
        c->copy (_, + it | hb_filter (*unicodes_set, hb_first), 4u, base, plan, &format4objidx);
        ಯದಿ (c->in_error () && c->only_overflow ())
        {
          // cmap4 overflowed, reset and retry serialization without format 4 subtables.
          c->revert (snap);
          ಪ್ರತ್ಯಾಗಮ serialize (c, it,
                            encodingrec_iter,
                            base,
                            plan,
                            ಸತ್ಯ,
			    source_table_length);
        }
      }

      ಅನ್ಯಥಾ ಯದಿ (format == 12)
      {
        ಯದಿ (_can_drop (_,
                       *unicodes_set,
                       base,
                       *unicodes_cache,
                       local_unicodes_cache,
                       + it | hb_map (hb_first), encodingrec_iter))
          ಅನುವರ್ತ;
        c->copy (_, + it | hb_filter (*unicodes_set, hb_first), 12u, base, plan, &format12objidx);
      }
      ಅನ್ಯಥಾ ಯದಿ (format == 14) c->copy (_, it, 14u, base, plan, &format14objidx);
#ifndef HB_NO_BEYOND_64K
      ಅನ್ಯಥಾ ಯದಿ (format == 15) c->copy (_, it, 15u, base, plan, &format15objidx);
#endif
    }
        unsigned length = c->length ();
        unsigned available = length > cmap::min_size ? length - cmap::min_size : 0;
        c->check_assign(ಏತತ್->encodingRecord.len,
                        available / EncodingRecord::static_size,
                        HB_SERIALIZE_ERROR_INT_OVERFLOW);

    // Fail if format 4 was dropped and there is no cmap12.
    ಪ್ರತ್ಯಾಗಮ !drop_format_4 || format12objidx;
  }

  template<typename Iterator, typename EncodingRecordIterator,
      hb_requires (hb_is_iterator (Iterator)),
      hb_requires (hb_is_iterator (EncodingRecordIterator))>
  ತಾರ್ಕಿಕ _can_drop (ಧ್ರುವ EncodingRecord& cmap12,
                  ಧ್ರುವ hb_set_t& cmap12_unicodes,
                  ಧ್ರುವ ಶೂನ್ಯ* base,
                  ಧ್ರುವ SubtableUnicodesCache& unicodes_cache,
                  SubtableUnicodesCache& local_unicodes_cache,
                  Iterator subset_unicodes,
                  EncodingRecordIterator encoding_records)
  {
    ಪ್ರತಿ (auto cp : + subset_unicodes | hb_filter (cmap12_unicodes))
    {
      ಯದಿ (cp >= 0x10000) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }

    unsigned target_platform;
    unsigned target_encoding;
    unsigned target_language = (base+cmap12.subtable).get_language ();

    ಯದಿ (cmap12.platformID == 0 && cmap12.encodingID == 4)
    {
      target_platform = 0;
      target_encoding = 3;
    } ಅನ್ಯಥಾ ಯದಿ (cmap12.platformID == 3 && cmap12.encodingID == 10) {
      target_platform = 3;
      target_encoding = 1;
    } ಅನ್ಯಥಾ {
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }

    ಪ್ರತಿ (ಧ್ರುವ auto& _ : encoding_records)
    {
      ಯದಿ (_.platformID != target_platform
          || _.encodingID != target_encoding
          || (base+_.subtable).get_language() != target_language)
        ಅನುವರ್ತ;

      ಧ್ರುವ hb_set_t* sibling_unicodes = unicodes_cache.set_for (&_, local_unicodes_cache);

      auto cmap12 = + subset_unicodes | hb_filter (cmap12_unicodes);
      auto sibling = + subset_unicodes | hb_filter (*sibling_unicodes);
      ಪ್ರತಿ (; cmap12 && sibling; cmap12++, sibling++)
      {
        unsigned a = *cmap12;
        unsigned b = *sibling;
        ಯದಿ (a != b) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      }

      ಪ್ರತ್ಯಾಗಮ !cmap12 && !sibling;
    }

    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }

  ಶೂನ್ಯ closure_glyphs (ಧ್ರುವ hb_set_t      *unicodes,
		       hb_set_t            *glyphset) ಧ್ರುವ
  {
    + hb_iter (encodingRecord)
    | hb_map (&EncodingRecord::subtable)
    | hb_map (hb_add (ಏತತ್))
    | hb_filter (&CmapSubtable::is_variation_selector)
    | hb_apply ([=] (ಧ್ರುವ CmapSubtable& _) { _.closure_glyphs (unicodes, glyphset); })
    ;
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ subset (c, ಅಸತ್ಯ); }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c, ತಾರ್ಕಿಕ is_dmap) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);

    cmap *cmap_ಅವಿಭಾಜ್ಯ = c->serializer->start_embed<cmap> ();

    auto encodingrec_iter =
    + hb_iter (encodingRecord)
    | hb_filter ([&](ಧ್ರುವ EncodingRecord& _) {
      ಪ್ರತ್ಯಾಗಮ cmap::filter_encoding_records_for_subset (ಏತತ್, _);
    })
    ;

    ಯದಿ (unlikely (!encodingrec_iter.len ())) return_trace (ಅಸತ್ಯ);

    ಧ್ರುವ EncodingRecord *unicode_bmp= nullptr, *unicode_ucs4 = nullptr, *ms_bmp = nullptr, *ms_ucs4 = nullptr;
    ತಾರ್ಕಿಕ has_format12 = ಅಸತ್ಯ;

    ಪ್ರತಿ (ಧ್ರುವ EncodingRecord& _ : encodingrec_iter)
    {
      unsigned format = (ಏತತ್ + _.subtable).u.format.v;
      ಯದಿ (format == 12) has_format12 = ಸತ್ಯ;

      ಧ್ರುವ EncodingRecord *table = std::addressof (_);
      ಯದಿ      (_.platformID == 0 && _.encodingID ==  3) unicode_bmp = table;
      ಅನ್ಯಥಾ ಯದಿ (_.platformID == 0 && _.encodingID ==  4) unicode_ucs4 = table;
      ಅನ್ಯಥಾ ಯದಿ (_.platformID == 3 && _.encodingID ==  1) ms_bmp = table;
      ಅನ್ಯಥಾ ಯದಿ (_.platformID == 3 && _.encodingID == 10) ms_ucs4 = table;
    }

    ಯದಿ (unlikely (!is_dmap && !has_format12 && !unicode_bmp && !ms_bmp)) return_trace (ಅಸತ್ಯ);
    ಯದಿ (unlikely (!is_dmap && has_format12 && (!unicode_ucs4 && !ms_ucs4))) return_trace (ಅಸತ್ಯ);

    auto it =
    + c->plan->unicode_to_new_gid_list.iter ()
    | hb_filter ([&] (ಧ್ರುವ hb_codepoint_pair_t _)
		 { ಪ್ರತ್ಯಾಗಮ (_.second != HB_MAP_ಮೌಲ್ಯ_INVALID); })
    ;

    return_trace (cmap_ಅವಿಭಾಜ್ಯ->serialize (c->serializer,
                                         it,
                                         encodingrec_iter,
                                         ಏತತ್,
                                         c->plan,
					 ಅಸತ್ಯ,
					 c->source_blob->length));
  }

  ಧ್ರುವ CmapSubtable *find_best_subtable (ತಾರ್ಕಿಕ *ಚಿಹ್ನೆ = nullptr,
					  ತಾರ್ಕಿಕ *mac = nullptr,
					  ತಾರ್ಕಿಕ *macroman = nullptr) ಧ್ರುವ
  {
    ಯದಿ (ಚಿಹ್ನೆ) *ಚಿಹ್ನೆ = ಅಸತ್ಯ;
    ಯದಿ (mac) *mac = ಅಸತ್ಯ;
    ಯದಿ (macroman) *macroman = ಅಸತ್ಯ;

    ಧ್ರುವ CmapSubtable *subtable;

    /* Symbol subtable.
     * Prefer symbol if available.
     * https://github.com/harfbuzz/harfbuzz/issues/1918 */
    ಯದಿ ((subtable = ಏತತ್->find_subtable (3, 0)))
    {
      ಯದಿ (ಚಿಹ್ನೆ) *ಚಿಹ್ನೆ = ಸತ್ಯ;
      ಪ್ರತ್ಯಾಗಮ subtable;
    }

    /* 32-bit subtables. */
    ಯದಿ ((subtable = ಏತತ್->find_subtable (3, 10))) ಪ್ರತ್ಯಾಗಮ subtable;
    ಯದಿ ((subtable = ಏತತ್->find_subtable (0, 6))) ಪ್ರತ್ಯಾಗಮ subtable;
    ಯದಿ ((subtable = ಏತತ್->find_subtable (0, 4))) ಪ್ರತ್ಯಾಗಮ subtable;

    /* 16-bit subtables. */
    ಯದಿ ((subtable = ಏತತ್->find_subtable (3, 1))) ಪ್ರತ್ಯಾಗಮ subtable;
    ಯದಿ ((subtable = ಏತತ್->find_subtable (0, 3))) ಪ್ರತ್ಯಾಗಮ subtable;
    ಯದಿ ((subtable = ಏತತ್->find_subtable (0, 2))) ಪ್ರತ್ಯಾಗಮ subtable;
    ಯದಿ ((subtable = ಏತತ್->find_subtable (0, 1))) ಪ್ರತ್ಯಾಗಮ subtable;
    ಯದಿ ((subtable = ಏತತ್->find_subtable (0, 0))) ಪ್ರತ್ಯಾಗಮ subtable;

    /* MacRoman subtable. */
    ಯದಿ ((subtable = ಏತತ್->find_subtable (1, 0)))
    {
      ಯದಿ (mac) *mac = ಸತ್ಯ;
      ಯದಿ (macroman) *macroman = ಸತ್ಯ;
      ಪ್ರತ್ಯಾಗಮ subtable;
    }
    /* Any other Mac subtable; we just map ASCII for these. */
    ಯದಿ ((subtable = ಏತತ್->find_subtable (1, 0xFFFF)))
    {
      ಯದಿ (mac) *mac = ಸತ್ಯ;
      ಪ್ರತ್ಯಾಗಮ subtable;
    }

    /* Meh. */
    ಪ್ರತ್ಯಾಗಮ &Null (CmapSubtable);
  }

  ಸಂರಚನಾ accelerator_t
  {
    using cache_t = hb_cache_t<21, 19>;
    static_assert (ಪರಿಮಾಣ (cache_t) == 1024, "");

    accelerator_t (hb_face_t *face, hb_tag_t table_tag = cmap::tableTag)
    {
      ಏತತ್->table = hb_sanitize_context_t ().reference_table<cmap> (face, table_tag);
      ಯದಿ (!ಏತತ್->table.get_length ())
	ಪ್ರತ್ಯಾಗಮ;

      ತಾರ್ಕಿಕ ಚಿಹ್ನೆ, mac, macroman;
      ಏತತ್->subtable = table->find_best_subtable (&ಚಿಹ್ನೆ, &mac, &macroman);
      ಏತತ್->subtable_uvs = &Null (CmapSubtable);
      {
	ಧ್ರುವ CmapSubtable *st = table->find_variation_selector_subtable ();
	ಯದಿ (st)
	  subtable_uvs = st;
      }

#ifndef HB_NO_OT_FONT_CMAP_CACHE
      cache = (cache_t *) hb_malloc (ಪರಿಮಾಣ (cache_t));
      ಯದಿ (cache)
	ನೂತನ (cache) cache_t ();
      ಅನ್ಯಥಾ
        ಪ್ರತ್ಯಾಗಮ; // Such that get_glyph_funcZ remains null.
#endif

      ಏತತ್->get_glyph_data = subtable;
#ifndef HB_NO_CMAP_LEGACY_SUBTABLES
      ತಾರ್ಕಿಕ is_format4 = subtable->u.format.v == 4;
      auto set_format4_getter = [ಏತತ್] (ತಾರ್ಕಿಕ (*func) (ಧ್ರುವ ಶೂನ್ಯ *,
						      hb_codepoint_t,
						      hb_codepoint_t *))
      {
	ಏತತ್->format4_accel.init (&ಏತತ್->subtable->u.format4,
				  get_subtable_data_size (ಏತತ್->subtable));
	ಏತತ್->get_glyph_data = &ಏತತ್->format4_accel;
	ಏತತ್->get_glyph_funcZ = func;
      };
      ಯದಿ (unlikely (ಚಿಹ್ನೆ))
      {
	ವಿಕಲ್ಪ ((unsigned) face->table.OS2->get_font_page ()) {
	ಪಕ್ಷ OS2::font_page_t::FONT_PAGE_NONE:
	  ಯದಿ (is_format4)
	    set_format4_getter (get_glyph_from_ಚಿಹ್ನೆ<CmapSubtableFormat4::accelerator_t, hb_ಚಿಹ್ನೆ_pua_map>);
	  ಅನ್ಯಥಾ
	    ಏತತ್->get_glyph_funcZ = get_glyph_from_ಚಿಹ್ನೆ<CmapSubtable, hb_ಚಿಹ್ನೆ_pua_map>;
	  ಭಙ್ಗ;
#ifndef HB_NO_OT_SHAPER_ARABIC_FALLBACK
	ಪಕ್ಷ OS2::font_page_t::FONT_PAGE_SIMP_ARABIC:
	  ಯದಿ (is_format4)
	    set_format4_getter (get_glyph_from_ಚಿಹ್ನೆ<CmapSubtableFormat4::accelerator_t, _hb_arabic_pua_simp_map>);
	  ಅನ್ಯಥಾ
	    ಏತತ್->get_glyph_funcZ = get_glyph_from_ಚಿಹ್ನೆ<CmapSubtable, _hb_arabic_pua_simp_map>;
	  ಭಙ್ಗ;
	ಪಕ್ಷ OS2::font_page_t::FONT_PAGE_TRAD_ARABIC:
	  ಯದಿ (is_format4)
	    set_format4_getter (get_glyph_from_ಚಿಹ್ನೆ<CmapSubtableFormat4::accelerator_t, _hb_arabic_pua_trad_map>);
	  ಅನ್ಯಥಾ
	    ಏತತ್->get_glyph_funcZ = get_glyph_from_ಚಿಹ್ನೆ<CmapSubtable, _hb_arabic_pua_trad_map>;
	  ಭಙ್ಗ;
#endif
	ಸ್ವಭಾವ:
	  ಯದಿ (is_format4)
	    set_format4_getter (get_glyph_from<CmapSubtableFormat4::accelerator_t>);
	  ಅನ್ಯಥಾ
	    ಏತತ್->get_glyph_funcZ = get_glyph_from<CmapSubtable>;
	  ಭಙ್ಗ;
	}
      }
      ಅನ್ಯಥಾ ಯದಿ (unlikely (macroman))
      {
	ಯದಿ (is_format4)
	  set_format4_getter (get_glyph_from_macroman<CmapSubtableFormat4::accelerator_t>);
	ಅನ್ಯಥಾ
	  ಏತತ್->get_glyph_funcZ = get_glyph_from_macroman<CmapSubtable>;
      }
      ಅನ್ಯಥಾ ಯದಿ (unlikely (mac))
      {
	ಯದಿ (is_format4)
	  set_format4_getter (get_glyph_from_ascii<CmapSubtableFormat4::accelerator_t>);
	ಅನ್ಯಥಾ
	  ಏತತ್->get_glyph_funcZ = get_glyph_from_ascii<CmapSubtable>;
      }
      ಅನ್ಯಥಾ
#endif
      {
	ವಿಕಲ್ಪ (subtable->u.format.v) {
	  /* Accelerate format 4 and format 12. */
	  ಸ್ವಭಾವ:
	    ಏತತ್->get_glyph_funcZ = get_glyph_from<CmapSubtable>;
	    ಭಙ್ಗ;
	  ಪಕ್ಷ 12:
	    ಏತತ್->get_glyph_funcZ = get_glyph_from<CmapSubtableFormat12>;
	    ಭಙ್ಗ;
	  ಪಕ್ಷ  4:
	  {
	    hb_barrier ();
	    ಏತತ್->format4_accel.init (&subtable->u.format4,
				      get_subtable_data_size (subtable));
	    ಏತತ್->get_glyph_data = &ಏತತ್->format4_accel;
	    ಏತತ್->get_glyph_funcZ = ಏತತ್->format4_accel.get_glyph_func;
	    ಭಙ್ಗ;
	  }
	}
      }
    }
    ~accelerator_t ()
    {
#ifndef HB_NO_OT_FONT_CMAP_CACHE
      hb_free (cache);
#endif
      table.destroy ();
    }

    ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ _cached_get (hb_codepoint_t unicode,
			     hb_codepoint_t *glyph) ಧ್ರುವ
    {
#ifndef HB_NO_OT_FONT_CMAP_CACHE
      // cache is always non-null if we have a get_glyph_funcZ
      unsigned v;
      ಯದಿ (cache->get (unicode, &v))
      {
        *glyph = v;
	ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
      }
#endif
      ತಾರ್ಕಿಕ ret  = ಏತತ್->get_glyph_funcZ (ಏತತ್->get_glyph_data, unicode, glyph);

#ifndef HB_NO_OT_FONT_CMAP_CACHE
      ಯದಿ (ret)
        cache->set (unicode, *glyph);
#endif

      ಪ್ರತ್ಯಾಗಮ ret;
    }

    ತಾರ್ಕಿಕ get_nominal_glyph (hb_codepoint_t  unicode,
			    hb_codepoint_t *glyph) ಧ್ರುವ
    {
      ಯದಿ (unlikely (!ಏತತ್->get_glyph_funcZ)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      ಪ್ರತ್ಯಾಗಮ _cached_get (unicode, glyph);
    }

    unsigned ಪೂರ್ಣಾಙ್ಕ get_nominal_glyphs (unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ,
				     ಧ್ರುವ hb_codepoint_t *first_unicode,
				     unsigned ಪೂರ್ಣಾಙ್ಕ unicode_stride,
				     hb_codepoint_t *first_glyph,
				     unsigned ಪೂರ್ಣಾಙ್ಕ glyph_stride) ಧ್ರುವ
    {
      ಯದಿ (unlikely (!ಏತತ್->get_glyph_funcZ)) ಪ್ರತ್ಯಾಗಮ 0;

      unsigned ಪೂರ್ಣಾಙ್ಕ done;
      ಪ್ರತಿ (done = 0;
	   done < ಎಣಿಕೆ && _cached_get (*first_unicode, first_glyph);
	   done++)
      {
	first_unicode = &StructAtOffsetUnaligned<hb_codepoint_t> (first_unicode, unicode_stride);
	first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      }
      ಪ್ರತ್ಯಾಗಮ done;
    }

    ತಾರ್ಕಿಕ get_variation_glyph (hb_codepoint_t  unicode,
			      hb_codepoint_t  variation_selector,
			      hb_codepoint_t *glyph) ಧ್ರುವ
    {
      ವಿಕಲ್ಪ (get_glyph_variant (unicode, variation_selector, glyph))
      {
	ಪಕ್ಷ GLYPH_VARIANT_NOT_FOUND:	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
	ಪಕ್ಷ GLYPH_VARIANT_FOUND:	ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
	ಪಕ್ಷ GLYPH_VARIANT_USE_DEFAULT:	ಭಙ್ಗ;
      }

      ಪ್ರತ್ಯಾಗಮ get_nominal_glyph (unicode, glyph);
    }

    glyph_variant_t get_glyph_variant (hb_codepoint_t  unicode,
				       hb_codepoint_t  variation_selector,
				       hb_codepoint_t *glyph) ಧ್ರುವ
    {
      ಪ್ರತ್ಯಾಗಮ ಏತತ್->subtable_uvs->get_glyph_variant (unicode,
						    variation_selector,
						    glyph);
    }

    ತಾರ್ಕಿಕ has_data () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ table.get_length (); }

    ಶೂನ್ಯ collect_unicodes (hb_set_t *out, unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_glyphs) ಧ್ರುವ
    { subtable->collect_unicodes (out, ಅಂಕೆ_glyphs, get_subtable_data_size (subtable)); }
    ಶೂನ್ಯ collect_mapping (hb_set_t *unicodes, hb_map_t *mapping,
			  unsigned ಅಂಕೆ_glyphs = UINT_MAX) ಧ್ರುವ
    { subtable->collect_mapping (unicodes, mapping, ಅಂಕೆ_glyphs, get_subtable_data_size (subtable)); }
    ಶೂನ್ಯ collect_variation_selectors (hb_set_t *out) ಧ್ರುವ
    { subtable_uvs->collect_variation_selectors (out); }
    ಶೂನ್ಯ collect_variation_unicodes (hb_codepoint_t variation_selector,
				     hb_set_t *out) ಧ್ರುವ
    { subtable_uvs->collect_variation_unicodes (variation_selector, out); }

    ರಕ್ಷಿತ:
    ಪ್ರಕಾರನಿರ್ದೇಶ ತಾರ್ಕಿಕ (*hb_cmap_get_glyph_func_t) (ಧ್ರುವ ಶೂನ್ಯ *obj,
					      hb_codepoint_t codepoint,
					      hb_codepoint_t *glyph);
    ಪ್ರಕಾರನಿರ್ದೇಶ uint16_t (*hb_pua_remap_func_t) (unsigned);

    template <typename Type>
    HB_INTERNAL ಸ್ಥಿರ ತಾರ್ಕಿಕ get_glyph_from (ಧ್ರುವ ಶೂನ್ಯ *obj,
					    hb_codepoint_t codepoint,
					    hb_codepoint_t *glyph)
    {
      ಧ್ರುವ Type *typed_obj = (ಧ್ರುವ Type *) obj;
      ಪ್ರತ್ಯಾಗಮ typed_obj->get_glyph (codepoint, glyph);
    }

    template <typename Type, hb_pua_remap_func_t remap>
    HB_INTERNAL ಸ್ಥಿರ ತಾರ್ಕಿಕ get_glyph_from_ಚಿಹ್ನೆ (ಧ್ರುವ ಶೂನ್ಯ *obj,
						   hb_codepoint_t codepoint,
						   hb_codepoint_t *glyph)
    {
      ಧ್ರುವ Type *typed_obj = (ಧ್ರುವ Type *) obj;
      ಯದಿ (likely (typed_obj->get_glyph (codepoint, glyph)))
	ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;

      ಯದಿ (hb_codepoint_t c = remap (codepoint))
	ಪ್ರತ್ಯಾಗಮ typed_obj->get_glyph (c, glyph);

      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }

    template <typename Type>
    HB_INTERNAL ಸ್ಥಿರ ತಾರ್ಕಿಕ get_glyph_from_ascii (ಧ್ರುವ ಶೂನ್ಯ *obj,
						  hb_codepoint_t codepoint,
						  hb_codepoint_t *glyph)
    {
      ಧ್ರುವ Type *typed_obj = (ಧ್ರುವ Type *) obj;
      ಪ್ರತ್ಯಾಗಮ codepoint < 0x80 && typed_obj->get_glyph (codepoint, glyph);
    }

    template <typename Type>
    HB_INTERNAL ಸ್ಥಿರ ತಾರ್ಕಿಕ get_glyph_from_macroman (ಧ್ರುವ ಶೂನ್ಯ *obj,
						     hb_codepoint_t codepoint,
						     hb_codepoint_t *glyph)
    {
      ಯದಿ (get_glyph_from_ascii<Type> (obj, codepoint, glyph))
	ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;

      ಧ್ರುವ Type *typed_obj = (ಧ್ರುವ Type *) obj;
      unsigned c = unicode_to_macroman (codepoint);
      ಪ್ರತ್ಯಾಗಮ c && typed_obj->get_glyph (c, glyph);
    }

    unsigned ಪೂರ್ಣಾಙ್ಕ get_subtable_data_size (ಧ್ರುವ CmapSubtable *subtable) ಧ್ರುವ
    {
      unsigned ಪೂರ್ಣಾಙ್ಕ table_length = ಏತತ್->table.get_length ();
      uintptr_t table_start = (uintptr_t) (ಧ್ರುವ ಶೂನ್ಯ *) ಏತತ್->table.get ();
      uintptr_t subtable_addr = (uintptr_t) (ಧ್ರುವ ಶೂನ್ಯ *) subtable;
      ಯದಿ (unlikely (subtable_addr < table_start))
	ಪ್ರತ್ಯಾಗಮ 0;

      uintptr_t subtable_offset = subtable_addr - table_start;
      ಯದಿ (unlikely (subtable_offset >= table_length))
	ಪ್ರತ್ಯಾಗಮ 0;

      ಪ್ರತ್ಯಾಗಮ table_length - (unsigned ಪೂರ್ಣಾಙ್ಕ) subtable_offset;
    }

    ಗುಪ್ತ:
    hb_nonnull_ptr_t<ಧ್ರುವ CmapSubtable> subtable;
    hb_nonnull_ptr_t<ಧ್ರುವ CmapSubtable> subtable_uvs;

    hb_cmap_get_glyph_func_t get_glyph_funcZ = nullptr;
    ಧ್ರುವ ಶೂನ್ಯ *get_glyph_data = nullptr;

    CmapSubtableFormat4::accelerator_t format4_accel;

#ifndef HB_NO_OT_FONT_CMAP_CACHE
    cache_t *cache = nullptr;
#endif

    ಸಾರ್ವಜನಿಕ:
    hb_blob_ptr_t<cmap> table;
  };

  ರಕ್ಷಿತ:

  ಧ್ರುವ CmapSubtable *find_subtable (unsigned ಪೂರ್ಣಾಙ್ಕ platform_id,
				     unsigned ಪೂರ್ಣಾಙ್ಕ encoding_id) ಧ್ರುವ
  {
    EncodingRecord key;
    key.platformID = platform_id;
    key.encodingID = encoding_id;

    ಧ್ರುವ EncodingRecord &result = encodingRecord.bsearch (key);
    ಯದಿ (!result.subtable)
      ಪ್ರತ್ಯಾಗಮ nullptr;

    ಪ್ರತ್ಯಾಗಮ &(ಏತತ್+result.subtable);
  }

  ಧ್ರುವ CmapSubtable *find_variation_selector_subtable () ಧ್ರುವ
  {
    ಧ್ರುವ CmapSubtable *format14 = nullptr;
    ಪ್ರತಿ (ಧ್ರುವ EncodingRecord& record : encodingRecord)
    {
      ಯದಿ (record.platformID != 0 || record.encodingID != 5)
	ಅನುವರ್ತ;

      ಧ್ರುವ CmapSubtable *subtable = &(ಏತತ್+record.subtable);
#ifndef HB_NO_BEYOND_64K
      ಯದಿ (subtable->u.format.v == 15)
	ಪ್ರತ್ಯಾಗಮ subtable;
#endif
      ಯದಿ (subtable->u.format.v == 14)
	format14 = subtable;
    }
    ಪ್ರತ್ಯಾಗಮ format14;
  }

  ಸಾರ್ವಜನಿಕ:

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) &&
		  hb_barrier () &&
		  likely (version == 0) &&
		  encodingRecord.sanitize (c, ಏತತ್));
  }

 ಗುಪ್ತ:

  ಸ್ಥಿರ ತಾರ್ಕಿಕ filter_encoding_records_for_subset(ಧ್ರುವ cmap* cmap,
                                                 ಧ್ರುವ EncodingRecord& _)
  {
    ಪ್ರತ್ಯಾಗಮ
        (_.platformID == 0 && _.encodingID == 3) ||
        (_.platformID == 0 && _.encodingID == 4) ||
        (_.platformID == 3 && _.encodingID == 1) ||
        (_.platformID == 3 && _.encodingID == 10) ||
        (cmap + _.subtable).is_variation_selector ();
  }

  ರಕ್ಷಿತ:
  HBUINT16	version;	/* Table version number (0). */
  SortedArray16Of<EncodingRecord>
		encodingRecord;	/* Encoding tables. */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (4, encodingRecord);
};

ಸಂರಚನಾ DMAP : cmap
{
  ಸ್ಥಿರ constexpr hb_tag_t tableTag = HB_OT_TAG_DMAP;

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ cmap::subset (c, ಸತ್ಯ); }
};

ಸಂರಚನಾ cmap_accelerator_t
{
  cmap_accelerator_t (hb_face_t *face) :
    dmap (face, HB_OT_TAG_DMAP),
    base (face)
  {}

  ತಾರ್ಕಿಕ get_nominal_glyph (hb_codepoint_t  unicode,
			  hb_codepoint_t *glyph) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ dmap.get_nominal_glyph (unicode, glyph) ||
	   base.get_nominal_glyph (unicode, glyph);
  }

  unsigned ಪೂರ್ಣಾಙ್ಕ get_nominal_glyphs (unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ,
				   ಧ್ರುವ hb_codepoint_t *first_unicode,
				   unsigned ಪೂರ್ಣಾಙ್ಕ unicode_stride,
				   hb_codepoint_t *first_glyph,
				   unsigned ಪೂರ್ಣಾಙ್ಕ glyph_stride) ಧ್ರುವ
  {
    ಯದಿ (likely (!dmap.has_data ()))
      ಪ್ರತ್ಯಾಗಮ base.get_nominal_glyphs (ಎಣಿಕೆ,
				      first_unicode, unicode_stride,
				      first_glyph, glyph_stride);

    unsigned ಪೂರ್ಣಾಙ್ಕ done;
    ಪ್ರತಿ (done = 0;
	 done < ಎಣಿಕೆ && get_nominal_glyph (*first_unicode, first_glyph);
	 done++)
    {
      first_unicode = &StructAtOffsetUnaligned<hb_codepoint_t> (first_unicode, unicode_stride);
      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
    }
    ಪ್ರತ್ಯಾಗಮ done;
  }

  ತಾರ್ಕಿಕ get_variation_glyph (hb_codepoint_t  unicode,
			    hb_codepoint_t  variation_selector,
			    hb_codepoint_t *glyph) ಧ್ರುವ
  {
    glyph_variant_t result = dmap.get_glyph_variant (unicode,
						     variation_selector,
						     glyph);
    ಯದಿ (result == GLYPH_VARIANT_NOT_FOUND)
      result = base.get_glyph_variant (unicode, variation_selector, glyph);

    ವಿಕಲ್ಪ (result)
    {
      ಪಕ್ಷ GLYPH_VARIANT_NOT_FOUND: ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      ಪಕ್ಷ GLYPH_VARIANT_FOUND: ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
      ಪಕ್ಷ GLYPH_VARIANT_USE_DEFAULT: ಭಙ್ಗ;
    }

    ಪ್ರತ್ಯಾಗಮ get_nominal_glyph (unicode, glyph);
  }

  ಶೂನ್ಯ collect_unicodes (hb_set_t *out, unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_glyphs) ಧ್ರುವ
  {
    base.collect_unicodes (out, ಅಂಕೆ_glyphs);
    dmap.collect_unicodes (out, ಅಂಕೆ_glyphs);
  }

  ಶೂನ್ಯ collect_mapping (hb_set_t *unicodes, hb_map_t *mapping,
			unsigned ಅಂಕೆ_glyphs = UINT_MAX) ಧ್ರುವ
  {
    base.collect_mapping (unicodes, mapping, ಅಂಕೆ_glyphs);
    dmap.collect_mapping (unicodes, mapping, ಅಂಕೆ_glyphs);
  }

  ಶೂನ್ಯ collect_variation_selectors (hb_set_t *out) ಧ್ರುವ
  {
    base.collect_variation_selectors (out);
    dmap.collect_variation_selectors (out);
  }

  ಶೂನ್ಯ collect_variation_unicodes (hb_codepoint_t variation_selector,
				   hb_set_t *out) ಧ್ರುವ
  {
    base.collect_variation_unicodes (variation_selector, out);
    dmap.collect_variation_unicodes (variation_selector, out);
  }

  ಶೂನ್ಯ closure_glyphs (ಧ್ರುವ hb_set_t *unicodes,
		       hb_set_t       *glyphset) ಧ್ರುವ
  {
    ಯದಿ (likely (!dmap.has_data ()))
    {
      base.table->closure_glyphs (unicodes, glyphset);
      ಪ್ರತ್ಯಾಗಮ;
    }

    hb_set_t variation_selectors;
    collect_variation_selectors (&variation_selectors);
    ಪ್ರತಿ (hb_codepoint_t variation_selector : variation_selectors)
    {
      ಯದಿ (!unicodes->has (variation_selector))
	ಅನುವರ್ತ;

      hb_set_t variation_unicodes;
      collect_variation_unicodes (variation_selector, &variation_unicodes);
      ಪ್ರತಿ (hb_codepoint_t unicode : variation_unicodes)
      {
	ಯದಿ (!unicodes->has (unicode))
	  ಅನುವರ್ತ;

	hb_codepoint_t glyph;
	ಯದಿ (get_variation_glyph (unicode, variation_selector, &glyph))
	  glyphset->add (glyph);
      }
    }
  }

  cmap::accelerator_t dmap;
  cmap::accelerator_t base;
};

} /* namespace OT */


#endif /* HB_OT_CMAP_TABLE_HH */
