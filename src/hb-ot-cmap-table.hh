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

#അന്തര്ഭാവ "hb-ot-os2-table.hh"
#അന്തര്ഭാവ "hb-ot-shaper-arabic-pua.hh"
#അന്തര്ഭാവ "hb-open-type.hh"
#അന്തര്ഭാവ "hb-ot-dual.hh"
#അന്തര്ഭാവ "hb-set.hh"
#അന്തര്ഭാവ "hb-cache.hh"

/*
 * cmap -- Character to Glyph Index Mapping
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cmap
 */
#define HB_OT_TAG_cmap HB_TAG('c','m','a','p')
#define HB_OT_TAG_DMAP HB_TAG('D','M','A','P')

നാമാകാശ OT {

സ്ഥിതം വരിയിൽ uint8_t unicode_to_macroman (hb_codepoint_t u)
{
  സ്ഥിതം സ്ഥിരം ഘടന unicode_to_macroman_t
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
  auto *c = hb_bsearch (u, mapping, ARRAY_LENGTH (mapping), വലുപ്പം (mapping[0]),
			_hb_cmp_operator<uint16_t, uint16_t>);
  തിരികെ c ? c->macroman : 0;
}

ഘടന CmapSubtableFormat0
{
  തർക്കം get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) സ്ഥിരം
  {
    hb_codepoint_t gid = codepoint < 256 ? glyphIdArray[codepoint] : 0;
    എങ്കിൽ (unlikely (!gid))
      തിരികെ അസത്യം;
    *glyph = gid;
    തിരികെ സത്യം;
  }

  unsigned get_language () സ്ഥിരം
  {
    തിരികെ language;
  }

  ശൂന്യം collect_unicodes (hb_set_t *out) സ്ഥിരം
  {
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < 256; i++)
      എങ്കിൽ (glyphIdArray[i])
	out->add (i);
  }

  ശൂന്യം collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping /* OUT */) സ്ഥിരം
  {
    വേണ്ടി (unsigned i = 0; i < 256; i++)
      എങ്കിൽ (glyphIdArray[i])
      {
	hb_codepoint_t glyph = glyphIdArray[i];
	unicodes->add (i);
	mapping->set (i, glyph);
      }
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്));
  }

  സംരക്ഷിത:
  HBUINT16	format;		/* Format number is set to 0. */
  HBUINT16	length;		/* Byte length of this subtable. */
  HBUINT16	language;	/* Ignore. */
  HBUINT8	glyphIdArray[256];/* An array that maps character
				 * code to glyph index values. */
  പൊതു:
  DEFINE_SIZE_STATIC (6 + 256);
};

ഘടന CmapSubtableFormat4
{
  template<typename Iterator,
      typename Writer,
	   hb_requires (hb_is_iterator (Iterator))>
  ശൂന്യം to_ranges (Iterator it, Writer& range_writer)
  {
    hb_codepoint_t start_cp = 0, prev_run_start_cp = 0, run_start_cp = 0, end_cp = 0, last_gid = 0;
    പൂർണ്ണസംഖ്യ run_length = 0 , delta = 0, prev_delta = 0;

    എണ്ണൽ {
      FIRST_SUB_RANGE,
      FOLLOWING_SUB_RANGE,
    } mode;

    ആകുമ്പോൾ (it) {
      // Start a new range
      {
        സ്ഥിരം auto& pair = *it;
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

      ആകുമ്പോൾ (it) {
        // Process range
        സ്ഥിരം auto& pair = *it;
        hb_codepoint_t next_cp = pair.first;
        hb_codepoint_t next_gid = pair.second;
        എങ്കിൽ (next_cp != end_cp + 1) {
          // Current range is over, stop processing.
          നിർത്തുക;
        }

        എങ്കിൽ (next_gid == last_gid + 1) {
          // The current run continues.
          end_cp = next_cp;
          run_length++;
          last_gid = next_gid;
          it++;
          തുടരുക;
        }

        // A new run is starting, decide if we want to commit the current run.
        പൂർണ്ണസംഖ്യ split_cost = (mode == FIRST_SUB_RANGE) ? 8 : 16;
        പൂർണ്ണസംഖ്യ run_cost = run_length * 2;
        എങ്കിൽ (run_cost >= split_cost) {
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

    എങ്കിൽ (likely (end_cp != 0xFFFF)) {
      range_writer (0xFFFF, 0xFFFF, 1);
    }
  }

  /*
   * Writes the current range as either one or two ranges depending on what is most efficient.
   */
  template<typename Writer>
  ശൂന്യം commit_current_range (hb_codepoint_t start,
                             hb_codepoint_t prev_run_start,
                             hb_codepoint_t run_start,
                             hb_codepoint_t end,
                             പൂർണ്ണസംഖ്യ run_delta,
                             പൂർണ്ണസംഖ്യ previous_run_delta,
                             പൂർണ്ണസംഖ്യ split_cost,
                             Writer& range_writer) {
    തർക്കം should_split = അസത്യം;
    എങ്കിൽ (start < run_start && run_start < end) {
      പൂർണ്ണസംഖ്യ run_cost = (end - run_start + 1) * 2;
      എങ്കിൽ (run_cost >= split_cost) {
        should_split = സത്യം;
      }
    }

    // A zero delta is also the marker for ranges that need a
    // glyphIdArray; serialize_rangeoffset_glyid() tells the two cases
    // apart by re-checking the mapping.
    എങ്കിൽ (should_split) {
      എങ്കിൽ (start == prev_run_start)
        range_writer (start, run_start - 1, previous_run_delta);
      അല്ലെങ്കിൽ
        range_writer (start, run_start - 1, 0);
      range_writer (run_start, end, run_delta);
      തിരികെ;
    }


    എങ്കിൽ (start == run_start) {
      // Range is only a run
      range_writer (start, end, run_delta);
      തിരികെ;
    }

    // Write only a single non-run range.
    range_writer (start, end, 0);
  }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  unsigned serialize_find_segcount (Iterator it) {
    ഘടന Counter {
      unsigned segcount = 0;

      ശൂന്യം operator() (hb_codepoint_t start,
                       hb_codepoint_t end,
                       പൂർണ്ണസംഖ്യ delta) {
        segcount++;
      }
    } counter;

    to_ranges (+it, counter);
    തിരികെ counter.segcount;
  }


  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  തർക്കം serialize_start_end_delta_arrays (hb_serialize_context_t *c,
                                         Iterator it,
                                         പൂർണ്ണസംഖ്യ segcount)
  {
    ഘടന Writer {
      hb_serialize_context_t *serializer_;
      HBUINT16* end_code_;
      HBUINT16* start_code_;
      HBINT16* id_delta_;
      പൂർണ്ണസംഖ്യ index_;

      Writer(hb_serialize_context_t *serializer)
          : serializer_(serializer),
            end_code_(nullptr),
            start_code_(nullptr),
            id_delta_(nullptr),
            index_ (0) {}
      ശൂന്യം operator() (hb_codepoint_t start,
                       hb_codepoint_t end,
                       പൂർണ്ണസംഖ്യ delta) {
        start_code_[index_] = start;
        end_code_[index_] = end;
        id_delta_[index_] = delta;
        index_++;
      }
    } writer(c);

    writer.end_code_ = c->allocate_size<HBUINT16> (HBUINT16::static_size * segcount, അസത്യം);
    (ശൂന്യം) c->allocate_size<HBUINT16> (2); // padding
    writer.start_code_ = c->allocate_size<HBUINT16> (HBUINT16::static_size * segcount, അസത്യം);
    writer.id_delta_ = c->allocate_size<HBINT16> (HBINT16::static_size * segcount, അസത്യം);

    എങ്കിൽ (unlikely (!writer.end_code_ || !writer.start_code_ || !writer.id_delta_)) തിരികെ അസത്യം;

    to_ranges (+it, writer);
    തിരികെ സത്യം;
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
    എങ്കിൽ (unlikely (!c->check_success (idRangeOffset))) തിരികെ nullptr;
    എങ്കിൽ (unlikely ((അക്ഷരം *)idRangeOffset - (അക്ഷരം *)idDelta != (പൂർണ്ണസംഖ്യ) segcount * (പൂർണ്ണസംഖ്യ) HBINT16::static_size)) തിരികെ nullptr;

    വേണ്ടി (unsigned i : hb_range (segcount))
    {
      /* A non-zero delta is always a real run. A zero delta is
       * ambiguous: identity mapping, or the marker for a range that
       * needs a glyphIdArray. Only zero deltas need a check. */
      പൂർണ്ണസംഖ്യ delta = idDelta[i];
      എങ്കിൽ (delta)
	തുടരുക;
      തർക്കം use_delta = സത്യം;
      വേണ്ടി (hb_codepoint_t cp = startCode[i]; cp <= endCode[i]; cp++)
      {
	hb_codepoint_t gid = cp_to_gid.get (cp);
	എങ്കിൽ (gid == HB_MAP_മൂല്യം_INVALID)
	  gid = 0;
	എങ്കിൽ (gid != cp)
	{
	  use_delta = അസത്യം;
	  നിർത്തുക;
	}
      }
      എങ്കിൽ (use_delta)
	തുടരുക;

      idRangeOffset[i] = 2 * (c->start_embed<HBUINT16> () - idRangeOffset - i);
      വേണ്ടി (hb_codepoint_t cp = startCode[i]; cp <= endCode[i]; cp++)
      {
        HBUINT16 gid;
        gid = cp_to_gid[cp];
        c->copy<HBUINT16> (gid);
      }
    }

    തിരികെ idRangeOffset;
  }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  ശൂന്യം serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    auto format4_iter =
    + it
    | hb_filter ([&] (സ്ഥിരം hb_codepoint_pair_t _)
		 { തിരികെ _.first <= 0xFFFF; })
    ;

    എങ്കിൽ (!format4_iter) തിരികെ;

    unsigned table_initpos = c->length ();
    എങ്കിൽ (unlikely (!c->extend_min (ഇത്))) തിരികെ;
    ഇത്->format = 4;

    hb_vector_t<hb_codepoint_pair_t> cp_to_gid {
      format4_iter
    };

    //serialize endCode[], startCode[], idDelta[]
    HBUINT16* endCode = c->start_embed<HBUINT16> ();
    unsigned segcount = serialize_find_segcount (cp_to_gid.iter());
    എങ്കിൽ (unlikely (!serialize_start_end_delta_arrays (c, cp_to_gid.iter(), segcount)))
      തിരികെ;

    HBUINT16 *startCode = endCode + segcount + 1;
    HBINT16 *idDelta = ((HBINT16*)startCode) + segcount;

    HBUINT16 *idRangeOffset = serialize_rangeoffset_glyid (c,
                                                           cp_to_gid.iter (),
                                                           endCode,
                                                           startCode,
                                                           idDelta,
                                                           segcount);
    എങ്കിൽ (unlikely (!c->check_success (idRangeOffset))) തിരികെ;

    ഇത്->length = c->length () - table_initpos;
    എങ്കിൽ ((long long) ഇത്->length != (long long) c->length () - table_initpos)
    {
      c->err (HB_SERIALIZE_ERROR_INT_OVERFLOW);
      തിരികെ;
    }

    ഇത്->seg_എണ്ണം_X2 = segcount * 2;
    ഇത്->entrySelector = hb_max (1u, hb_bit_storage (segcount)) - 1;
    ഇത്->തിരയൽ_Range = 2 * (1u << ഇത്->entrySelector);
    ഇത്->rangeShift = segcount * 2 > ഇത്->തിരയൽ_Range
		       ? 2 * segcount - ഇത്->തിരയൽ_Range
		       : 0;
  }

  unsigned get_language () സ്ഥിരം
  {
    തിരികെ language;
  }

  ഘടന accelerator_t
  {
    accelerator_t () {}
    accelerator_t (സ്ഥിരം CmapSubtableFormat4 *subtable) = delete;

    ശൂന്യം init (സ്ഥിരം CmapSubtableFormat4 *subtable,
	       unsigned പൂർണ്ണസംഖ്യ              subtable_data_size)
    {
      seg_എണ്ണം_2 = subtable->seg_എണ്ണം_X2 / 2;
      end_എണ്ണം = subtable->values.arrayZ;
      start_എണ്ണം = end_എണ്ണം + seg_എണ്ണം_2 + 1;
      idDelta = start_എണ്ണം + seg_എണ്ണം_2;
      idRangeOffset = idDelta + seg_എണ്ണം_2;
      glyphIdArray = idRangeOffset + seg_എണ്ണം_2;

      unsigned പൂർണ്ണസംഖ്യ മൂല്യങ്ങൾ_offset = 16 + 8 * seg_എണ്ണം_2;
      glyphIdArrayLength = subtable_data_size > മൂല്യങ്ങൾ_offset
			   ? (subtable_data_size - മൂല്യങ്ങൾ_offset) / 2
			   : 0;
    }

    തർക്കം get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) സ്ഥിരം
    {
      ഘടന CustomRange
      {
	പൂർണ്ണസംഖ്യ cmp (hb_codepoint_t k,
		 unsigned distance) സ്ഥിരം
	{
	  എങ്കിൽ (k > last) തിരികെ +1;
	  എങ്കിൽ (k < (&last)[distance]/*first*/) തിരികെ -1;
	  തിരികെ 0;
	}
	HBUINT16 last;
      };

      സ്ഥിരം HBUINT16 *found = hb_bsearch (codepoint,
					  ഇത്->end_എണ്ണം,
					  ഇത്->seg_എണ്ണം_2,
					  വലുപ്പം (CustomRange),
					  _hb_cmp_method<hb_codepoint_t, CustomRange, unsigned>,
					  ഇത്->seg_എണ്ണം_2 + 1);
      എങ്കിൽ (unlikely (!found))
	തിരികെ അസത്യം;
      unsigned പൂർണ്ണസംഖ്യ i = found - end_എണ്ണം;

      hb_codepoint_t gid;
      unsigned പൂർണ്ണസംഖ്യ rangeOffset = ഇത്->idRangeOffset[i];
      എങ്കിൽ (rangeOffset == 0)
	gid = codepoint + ഇത്->idDelta[i];
      അല്ലെങ്കിൽ
      {
	/* Somebody has been smoking... */
	unsigned പൂർണ്ണസംഖ്യ index = rangeOffset / 2 + (codepoint - ഇത്->start_എണ്ണം[i]) + i - ഇത്->seg_എണ്ണം_2;
	എങ്കിൽ (unlikely (index >= ഇത്->glyphIdArrayLength))
	  തിരികെ അസത്യം;
	gid = ഇത്->glyphIdArray[index];
	എങ്കിൽ (unlikely (!gid))
	  തിരികെ അസത്യം;
	gid += ഇത്->idDelta[i];
      }
      gid &= 0xFFFFu;
      എങ്കിൽ (unlikely (!gid))
	തിരികെ അസത്യം;
      *glyph = gid;
      തിരികെ സത്യം;
    }

    HB_INTERNAL സ്ഥിതം തർക്കം get_glyph_func (സ്ഥിരം ശൂന്യം *obj, hb_codepoint_t codepoint, hb_codepoint_t *glyph)
    { തിരികെ ((സ്ഥിരം accelerator_t *) obj)->get_glyph (codepoint, glyph); }

    ശൂന്യം collect_unicodes (hb_set_t *out) സ്ഥിരം
    {
      unsigned പൂർണ്ണസംഖ്യ എണ്ണം = ഇത്->seg_എണ്ണം_2;
      എങ്കിൽ (എണ്ണം && ഇത്->start_എണ്ണം[എണ്ണം - 1] == 0xFFFFu)
	എണ്ണം--; /* Skip sentinel segment. */
      വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
      {
	hb_codepoint_t start = ഇത്->start_എണ്ണം[i];
	hb_codepoint_t end = ഇത്->end_എണ്ണം[i];
	unsigned പൂർണ്ണസംഖ്യ rangeOffset = ഇത്->idRangeOffset[i];
        out->add_range(start, end);
	എങ്കിൽ (rangeOffset == 0)
	{
	  വേണ്ടി (hb_codepoint_t codepoint = start; codepoint <= end; codepoint++)
	  {
	    hb_codepoint_t gid = (codepoint + ഇത്->idDelta[i]) & 0xFFFFu;
	    എങ്കിൽ (unlikely (!gid))
              out->del(codepoint);
	  }
	}
	അല്ലെങ്കിൽ
	{
	  വേണ്ടി (hb_codepoint_t codepoint = start; codepoint <= end; codepoint++)
	  {
	    unsigned പൂർണ്ണസംഖ്യ index = rangeOffset / 2 + (codepoint - ഇത്->start_എണ്ണം[i]) + i - ഇത്->seg_എണ്ണം_2;
	    എങ്കിൽ (unlikely (index >= ഇത്->glyphIdArrayLength))
            {
              out->del_range (codepoint, end);
	      നിർത്തുക;
            }
	    hb_codepoint_t gid = ഇത്->glyphIdArray[index];
	    എങ്കിൽ (unlikely (!gid))
              out->del(codepoint);
	  }
	}
      }
    }

    ശൂന്യം collect_mapping (hb_set_t *unicodes, /* OUT */
			  hb_map_t *mapping /* OUT */) സ്ഥിരം
    {
      // TODO(grieger): optimize similar to collect_unicodes
      // (ie. use add_range())
      unsigned എണ്ണം = ഇത്->seg_എണ്ണം_2;
      എങ്കിൽ (എണ്ണം && ഇത്->start_എണ്ണം[എണ്ണം - 1] == 0xFFFFu)
	എണ്ണം--; /* Skip sentinel segment. */
      വേണ്ടി (unsigned i = 0; i < എണ്ണം; i++)
      {
	hb_codepoint_t start = ഇത്->start_എണ്ണം[i];
	hb_codepoint_t end = ഇത്->end_എണ്ണം[i];
	unsigned rangeOffset = ഇത്->idRangeOffset[i];
	എങ്കിൽ (rangeOffset == 0)
	{
	  വേണ്ടി (hb_codepoint_t codepoint = start; codepoint <= end; codepoint++)
	  {
	    hb_codepoint_t gid = (codepoint + ഇത്->idDelta[i]) & 0xFFFFu;
	    എങ്കിൽ (unlikely (!gid))
	      തുടരുക;
	    unicodes->add (codepoint);
	    mapping->set (codepoint, gid);
	  }
	}
	അല്ലെങ്കിൽ
	{
	  വേണ്ടി (hb_codepoint_t codepoint = start; codepoint <= end; codepoint++)
	  {
	    unsigned index = rangeOffset / 2 + (codepoint - ഇത്->start_എണ്ണം[i]) + i - ഇത്->seg_എണ്ണം_2;
	    എങ്കിൽ (unlikely (index >= ഇത്->glyphIdArrayLength))
	      നിർത്തുക;
	    hb_codepoint_t gid = ഇത്->glyphIdArray[index];
	    എങ്കിൽ (unlikely (!gid))
	      തുടരുക;
            gid = (gid + ഇത്->idDelta[i]) & 0xFFFFu;
	    unicodes->add (codepoint);
	    mapping->set (codepoint, gid);
	  }
	}
      }
    }

    സ്ഥിരം HBUINT16 *end_എണ്ണം;
    സ്ഥിരം HBUINT16 *start_എണ്ണം;
    സ്ഥിരം HBUINT16 *idDelta;
    സ്ഥിരം HBUINT16 *idRangeOffset;
    സ്ഥിരം HBUINT16 *glyphIdArray;
    unsigned പൂർണ്ണസംഖ്യ seg_എണ്ണം_2;
    unsigned പൂർണ്ണസംഖ്യ glyphIdArrayLength;
  };

  തർക്കം get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph,
		  unsigned പൂർണ്ണസംഖ്യ subtable_data_size) സ്ഥിരം
  {
    accelerator_t accel;
    accel.init (ഇത്, subtable_data_size);
    തിരികെ accel.get_glyph_func (&accel, codepoint, glyph);
  }
  തർക്കം get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) സ്ഥിരം
  { തിരികെ അസത്യം; }

  ശൂന്യം collect_unicodes (hb_set_t *out, unsigned പൂർണ്ണസംഖ്യ subtable_data_size) സ്ഥിരം
  {
    accelerator_t accel;
    accel.init (ഇത്, subtable_data_size);
    accel.collect_unicodes (out);
  }
  ശൂന്യം collect_unicodes (hb_set_t *out) സ്ഥിരം
  { collect_unicodes (out, length); }

  ശൂന്യം collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping, /* OUT */
			unsigned പൂർണ്ണസംഖ്യ subtable_data_size) സ്ഥിരം
  {
    accelerator_t accel;
    accel.init (ഇത്, subtable_data_size);
    accel.collect_mapping (unicodes, mapping);
  }
  ശൂന്യം collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping /* OUT */) സ്ഥിരം
  { collect_mapping (unicodes, mapping, length); }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    എങ്കിൽ (unlikely (!c->check_struct (ഇത്)))
      return_trace (അസത്യം);
    hb_barrier ();

    return_trace (c->check_range (values, 2 + 4 * seg_എണ്ണം_X2));
  }

  സംരക്ഷിത:
  HBUINT16	format;		/* Format number is set to 4. */
  HBUINT16	length;		/* This is the length in bytes of the
				 * subtable. */
  HBUINT16	language;	/* Ignore. */
  HBUINT16	seg_എണ്ണം_X2;	/* 2 x segCount. */
  HBUINT16	തിരയൽ_Range;	/* 2 * (2**floor(log2(segCount))) */
  HBUINT16	entrySelector;	/* log2(searchRange/2) */
  HBUINT16	rangeShift;	/* 2 x segCount - searchRange */

  UnsizedArrayOf<HBUINT16>
		values;
#എങ്കിൽ 0
  HBUINT16	end_എണ്ണം[seg_എണ്ണം_2];	/* End characterCode for each segment,
					 * last=0xFFFFu. */
  HBUINT16	reservedPad;		/* Set to 0. */
  HBUINT16	start_എണ്ണം[seg_എണ്ണം_2];	/* Start character code for each segment. */
  HBINT16		idDelta[seg_എണ്ണം_2];	/* Delta for all character codes in segment. */
  HBUINT16	idRangeOffset[seg_എണ്ണം_2];/* Offsets into glyphIdArray or 0 */
  UnsizedArrayOf<HBUINT16>
		glyphIdArray;	/* Glyph index array (arbitrary length) */
#endif

  പൊതു:
  DEFINE_SIZE_ARRAY (14, values);
};

ഘടന CmapSubtableLongGroup
{
  friend ഘടന CmapSubtableFormat12;
  friend ഘടന CmapSubtableFormat13;
  template<typename U>
  friend ഘടന CmapSubtableLongSegmented;
  friend ഘടന cmap;

  പൂർണ്ണസംഖ്യ cmp (hb_codepoint_t codepoint) സ്ഥിരം
  {
    എങ്കിൽ (codepoint < startCharCode) തിരികെ -1;
    എങ്കിൽ (codepoint > endCharCode)   തിരികെ +1;
    തിരികെ 0;
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്));
  }

  സ്വകാര്യ:
  HBUINT32		startCharCode;	/* First character code in this group. */
  HBUINT32		endCharCode;	/* Last character code in this group. */
  HBUINT32		glyphID;	/* Glyph index; interpretation depends on
					 * subtable format. */
  പൊതു:
  DEFINE_SIZE_STATIC (12);
};
DECLARE_NULL_NAMESPACE_BYTES (OT, CmapSubtableLongGroup);

template <typename UINT>
ഘടന CmapSubtableTrimmed
{
  തർക്കം get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) സ്ഥിരം
  {
    /* Rely on our implicit array bound-checking. */
    hb_codepoint_t gid = glyphIdArray[codepoint - startCharCode];
    എങ്കിൽ (unlikely (!gid))
      തിരികെ അസത്യം;
    *glyph = gid;
    തിരികെ സത്യം;
  }

  unsigned get_language () സ്ഥിരം
  {
    തിരികെ language;
  }

  ശൂന്യം collect_unicodes (hb_set_t *out) സ്ഥിരം
  {
    hb_codepoint_t start = startCharCode;
    unsigned പൂർണ്ണസംഖ്യ എണ്ണം = glyphIdArray.len;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
      എങ്കിൽ (glyphIdArray[i])
	out->add (start + i);
  }

  ശൂന്യം collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping /* OUT */) സ്ഥിരം
  {
    hb_codepoint_t start_cp = startCharCode;
    unsigned എണ്ണം = glyphIdArray.len;
    വേണ്ടി (unsigned i = 0; i < എണ്ണം; i++)
      എങ്കിൽ (glyphIdArray[i])
      {
	hb_codepoint_t unicode = start_cp + i;
	hb_codepoint_t glyphid = glyphIdArray[i];
	unicodes->add (unicode);
	mapping->set (unicode, glyphid);
      }
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്) && glyphIdArray.sanitize (c));
  }

  സംരക്ഷിത:
  UINT		formatReserved;	/* Subtable format and (maybe) padding. */
  UINT		length;		/* Byte length of this subtable. */
  UINT		language;	/* Ignore. */
  UINT		startCharCode;	/* First character code covered. */
  ArrayOf<HBGlyphID16, UINT>
		glyphIdArray;	/* Array of glyph index values for character
				 * codes in the range. */
  പൊതു:
  DEFINE_SIZE_ARRAY (5 * വലുപ്പം (UINT), glyphIdArray);
};

ഘടന CmapSubtableFormat6  : CmapSubtableTrimmed<HBUINT16> {};
ഘടന CmapSubtableFormat10 : CmapSubtableTrimmed<HBUINT32> {};

template <typename T>
ഘടന CmapSubtableLongSegmented
{
  friend ഘടന cmap;

  തർക്കം get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) സ്ഥിരം
  {
    hb_codepoint_t gid = T::group_get_glyph (groups.bsearch (codepoint), codepoint);
    എങ്കിൽ (unlikely (!gid))
      തിരികെ അസത്യം;
    *glyph = gid;
    തിരികെ സത്യം;
  }

  unsigned get_language () സ്ഥിരം
  {
    തിരികെ language;
  }

  ശൂന്യം collect_unicodes (hb_set_t *out, unsigned പൂർണ്ണസംഖ്യ അക്കം_glyphs) സ്ഥിരം
  {
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < ഇത്->groups.len; i++)
    {
      hb_codepoint_t start = ഇത്->groups[i].startCharCode;
      hb_codepoint_t end = hb_min ((hb_codepoint_t) ഇത്->groups[i].endCharCode,
				   (hb_codepoint_t) HB_UNICODE_MAX);
      hb_codepoint_t gid = ഇത്->groups[i].glyphID;
      എങ്കിൽ (!gid)
      {
	/* Intention is: if (hb_is_same (T, CmapSubtableFormat13)) continue; */
	എങ്കിൽ (! T::group_get_glyph (ഇത്->groups[i], end)) തുടരുക;
	start++;
	gid++;
      }
      എങ്കിൽ (unlikely ((unsigned പൂർണ്ണസംഖ്യ) gid >= അക്കം_glyphs)) തുടരുക;
      എങ്കിൽ (unlikely ((unsigned പൂർണ്ണസംഖ്യ) (gid + end - start) >= അക്കം_glyphs))
	end = start + (hb_codepoint_t) അക്കം_glyphs - gid;

      out->add_range (start, hb_min (end, 0x10FFFFu));
    }
  }

  ശൂന്യം collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping, /* OUT */
			unsigned അക്കം_glyphs) സ്ഥിരം
  {
    hb_codepoint_t last_end = 0;
    unsigned എണ്ണം = ഇത്->groups.len;
    വേണ്ടി (unsigned i = 0; i < എണ്ണം; i++)
    {
      hb_codepoint_t start = ഇത്->groups.arrayZ[i].startCharCode;
      hb_codepoint_t end = hb_min ((hb_codepoint_t) ഇത്->groups.arrayZ[i].endCharCode,
				   (hb_codepoint_t) HB_UNICODE_MAX);
      എങ്കിൽ (unlikely (start > end || start < last_end)) {
        // Range is not in order and is invalid, skip it.
        തുടരുക;
      }
      last_end = end;


      hb_codepoint_t gid = ഇത്->groups.arrayZ[i].glyphID;
      എങ്കിൽ (!gid)
      {
        എങ്കിൽ (T::format_സംഖ്യ == 13) തുടരുക;
	start++;
	gid++;
      }
      എങ്കിൽ (unlikely ((unsigned പൂർണ്ണസംഖ്യ) gid >= അക്കം_glyphs)) തുടരുക;
      എങ്കിൽ (unlikely ((unsigned പൂർണ്ണസംഖ്യ) (gid + end - start) >= അക്കം_glyphs))
	end = start + (hb_codepoint_t) അക്കം_glyphs - gid;

      mapping->alloc (mapping->get_population () + end - start + 1);

      unicodes->add_range (start, end);
      വേണ്ടി (unsigned cp = start; cp <= end; cp++)
      {
	mapping->set (cp, gid);
        gid += T::increment;
      }
    }
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്) && groups.sanitize (c));
  }

  സംരക്ഷിത:
  HBUINT16	format;		/* Subtable format; set to 12. */
  HBUINT16	reserved;	/* Reserved; set to 0. */
  HBUINT32	length;		/* Byte length of this subtable. */
  HBUINT32	language;	/* Ignore. */
  SortedArray32Of<CmapSubtableLongGroup>
		groups;		/* Groupings. */
  പൊതു:
  DEFINE_SIZE_ARRAY (16, groups);
};

ഘടന CmapSubtableFormat12 : CmapSubtableLongSegmented<CmapSubtableFormat12>
{
  സ്ഥിതം constexpr പൂർണ്ണസംഖ്യ increment = 1;
  സ്ഥിതം constexpr പൂർണ്ണസംഖ്യ format_സംഖ്യ = 12;

  സ്ഥിതം hb_codepoint_t group_get_glyph (സ്ഥിരം CmapSubtableLongGroup &group,
					 hb_codepoint_t u)
  { തിരികെ likely (group.startCharCode <= group.endCharCode) ?
	   group.glyphID + (u - group.startCharCode) : 0; }


  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  ശൂന്യം serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    എങ്കിൽ (!it) തിരികെ;
    unsigned table_initpos = c->length ();
    എങ്കിൽ (unlikely (!c->extend_min (ഇത്))) തിരികെ;

    hb_codepoint_t startCharCode = (hb_codepoint_t) -1, endCharCode = (hb_codepoint_t) -1;
    hb_codepoint_t glyphID = 0;

    വേണ്ടി (സ്ഥിരം auto& _ : +it)
    {
      എങ്കിൽ (startCharCode == (hb_codepoint_t) -1)
      {
	startCharCode = _.first;
	endCharCode = _.first;
	glyphID = _.second;
      }
      അല്ലെങ്കിൽ എങ്കിൽ (!_is_gid_consecutive (endCharCode, startCharCode, glyphID, _.first, _.second))
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
      അല്ലെങ്കിൽ
	endCharCode = _.first;
    }

    CmapSubtableLongGroup record;
    record.startCharCode = startCharCode;
    record.endCharCode = endCharCode;
    record.glyphID = glyphID;
    c->copy<CmapSubtableLongGroup> (record);

    ഇത്->format = 12;
    ഇത്->reserved = 0;
    ഇത്->length = c->length () - table_initpos;
    ഇത്->groups.len = (ഇത്->length - min_size) / CmapSubtableLongGroup::static_size;
  }

  സ്ഥിതം size_t get_sub_table_size (സ്ഥിരം hb_sorted_vector_t<CmapSubtableLongGroup> &groups_data)
  { തിരികെ 16 + 12 * groups_data.length; }

  സ്വകാര്യ:
  സ്ഥിതം തർക്കം _is_gid_consecutive (hb_codepoint_t endCharCode,
				   hb_codepoint_t startCharCode,
				   hb_codepoint_t glyphID,
				   hb_codepoint_t cp,
				   hb_codepoint_t new_gid)
  {
    തിരികെ (cp - 1 == endCharCode) &&
	new_gid == glyphID + (cp - startCharCode);
  }

};

ഘടന CmapSubtableFormat13 : CmapSubtableLongSegmented<CmapSubtableFormat13>
{
  സ്ഥിതം constexpr പൂർണ്ണസംഖ്യ increment = 0;
  സ്ഥിതം constexpr പൂർണ്ണസംഖ്യ format_സംഖ്യ = 13;

  സ്ഥിതം hb_codepoint_t group_get_glyph (സ്ഥിരം CmapSubtableLongGroup &group,
					 hb_codepoint_t u HB_UNUSED)
  { തിരികെ group.glyphID; }
};

തരനിർവചനം എണ്ണൽ
{
  GLYPH_VARIANT_NOT_FOUND = 0,
  GLYPH_VARIANT_FOUND = 1,
  GLYPH_VARIANT_USE_DEFAULT = 2
} glyph_variant_t;

ഘടന Unicode_മൂല്യം_Range
{
  പൂർണ്ണസംഖ്യ cmp (സ്ഥിരം hb_codepoint_t &codepoint) സ്ഥിരം
  {
    എങ്കിൽ (codepoint < start_Unicode_മൂല്യം) തിരികെ -1;
    എങ്കിൽ (codepoint > start_Unicode_മൂല്യം + additional_എണ്ണം) തിരികെ +1;
    തിരികെ 0;
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്));
  }

  HBUINT24	start_Unicode_മൂല്യം;	/* First value in this range. */
  HBUINT8	additional_എണ്ണം;	/* Number of additional values in this
					 * range. */
  പൊതു:
  DEFINE_SIZE_STATIC (4);
};

ഘടന DefaultUVS : SortedArray32Of<Unicode_മൂല്യം_Range>
{
  ശൂന്യം collect_unicodes (hb_set_t *out) സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ എണ്ണം = len;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
    {
      hb_codepoint_t first = arrayZ[i].start_Unicode_മൂല്യം;
      hb_codepoint_t last = hb_min ((hb_codepoint_t) (first + arrayZ[i].additional_എണ്ണം),
				    (hb_codepoint_t) HB_UNICODE_MAX);
      out->add_range (first, last);
    }
  }

  DefaultUVS* copy (hb_serialize_context_t *c,
		    സ്ഥിരം hb_set_t *unicodes) സ്ഥിരം
  {
    auto *out = c->start_embed<DefaultUVS> ();
    auto snap = c->snapshot ();

    HBUINT32 len;
    len = 0;
    എങ്കിൽ (unlikely (!c->copy<HBUINT32> (len))) തിരികെ nullptr;
    unsigned init_len = c->length ();

    എങ്കിൽ (ഇത്->len > unicodes->get_population () * hb_bit_storage ((unsigned) ഇത്->len))
    {
      hb_codepoint_t start = HB_SET_മൂല്യം_INVALID;
      hb_codepoint_t end = HB_SET_മൂല്യം_INVALID;

      വേണ്ടി (auto u : *unicodes)
      {
        എങ്കിൽ (!as_array ().bsearch (u))
	  തുടരുക;
	എങ്കിൽ (start == HB_SET_മൂല്യം_INVALID)
	{
	  start = u;
	  end = start - 1;
	}
	എങ്കിൽ (end + 1 != u || end - start == 255)
        {
	  Unicode_മൂല്യം_Range rec;
	  rec.start_Unicode_മൂല്യം = start;
	  rec.additional_എണ്ണം = end - start;
	  c->copy<Unicode_മൂല്യം_Range> (rec);
	  start = u;
	}
	end = u;
      }
      എങ്കിൽ (start != HB_SET_മൂല്യം_INVALID)
      {
	Unicode_മൂല്യം_Range rec;
	rec.start_Unicode_മൂല്യം = start;
	rec.additional_എണ്ണം = end - start;
	c->copy<Unicode_മൂല്യം_Range> (rec);
      }

    }
    അല്ലെങ്കിൽ
    {
      hb_codepoint_t lastCode = HB_SET_മൂല്യം_INVALID;
      പൂർണ്ണസംഖ്യ എണ്ണം = -1;

      വേണ്ടി (സ്ഥിരം Unicode_മൂല്യം_Range& _ : *ഇത്)
      {
	hb_codepoint_t curEntry = (hb_codepoint_t) (_.start_Unicode_മൂല്യം - 1);
	hb_codepoint_t end = curEntry + _.additional_എണ്ണം + 2;

	വേണ്ടി (; unicodes->next (&curEntry) && curEntry < end;)
	{
	  എണ്ണം += 1;
	  എങ്കിൽ (lastCode == HB_SET_മൂല്യം_INVALID)
	    lastCode = curEntry;
	  അല്ലെങ്കിൽ എങ്കിൽ (lastCode + എണ്ണം != curEntry || എണ്ണം > 255)
	  {
	    Unicode_മൂല്യം_Range rec;
	    rec.start_Unicode_മൂല്യം = lastCode;
	    rec.additional_എണ്ണം = എണ്ണം - 1;
	    c->copy<Unicode_മൂല്യം_Range> (rec);

	    lastCode = curEntry;
	    എണ്ണം = 0;
	  }
	}
      }

      എങ്കിൽ (lastCode != HB_MAP_മൂല്യം_INVALID)
      {
	Unicode_മൂല്യം_Range rec;
	rec.start_Unicode_മൂല്യം = lastCode;
	rec.additional_എണ്ണം = എണ്ണം;
	c->copy<Unicode_മൂല്യം_Range> (rec);
      }
    }

    എങ്കിൽ (c->length () - init_len == 0)
    {
      c->revert (snap);
      തിരികെ nullptr;
    }
    അല്ലെങ്കിൽ
    {
      എങ്കിൽ (unlikely (!c->check_assign (out->len,
                                      (c->length () - init_len) / Unicode_മൂല്യം_Range::static_size,
                                      HB_SERIALIZE_ERROR_INT_OVERFLOW))) തിരികെ nullptr;
      തിരികെ out;
    }
  }

  പൊതു:
  DEFINE_SIZE_ARRAY (4, *ഇത്);
};

template <typename Types>
ഘടന UVSMapping
{
  using GlyphID = typename Types::HBGlyphID;

  പൂർണ്ണസംഖ്യ cmp (സ്ഥിരം hb_codepoint_t &codepoint) സ്ഥിരം
  { തിരികെ unicode_മൂല്യം.cmp (codepoint); }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്));
  }

  HBUINT24	unicode_മൂല്യം;	/* Base Unicode value of the UVS */
  GlyphID
		glyphID;	/* Glyph ID of the UVS. */
  പൊതു:
  DEFINE_SIZE_STATIC (3 + GlyphID::static_size);
};

template <typename Types>
ഘടന NonDefaultUVS : SortedArray32Of<UVSMapping<Types>>
{
  using Mapping = UVSMapping<Types>;

  ശൂന്യം collect_unicodes (hb_set_t *out) സ്ഥിരം
  {
    വേണ്ടി (സ്ഥിരം auto& a : ഇത്->as_array ())
      out->add (a.unicode_മൂല്യം);
  }

  ശൂന്യം collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping /* OUT */) സ്ഥിരം
  {
    വേണ്ടി (സ്ഥിരം auto& a : ഇത്->as_array ())
    {
      hb_codepoint_t unicode = a.unicode_മൂല്യം;
      hb_codepoint_t glyphid = a.glyphID;
      unicodes->add (unicode);
      mapping->set (unicode, glyphid);
    }
  }

  ശൂന്യം closure_glyphs (സ്ഥിരം hb_set_t      *unicodes,
		       hb_set_t            *glyphset) സ്ഥിരം
  {
    + ഇത്->as_array ()
    | hb_filter (unicodes, &Mapping::unicode_മൂല്യം)
    | hb_map (&Mapping::glyphID)
    | hb_sink (glyphset)
    ;
  }

  NonDefaultUVS* copy (hb_serialize_context_t *c,
		       സ്ഥിരം hb_set_t *unicodes,
		       സ്ഥിരം hb_set_t *glyphs_requested,
		       സ്ഥിരം hb_map_t *glyph_map) സ്ഥിരം
  {
    auto *out = c->start_embed<NonDefaultUVS> ();
    auto it =
    + ഇത്->as_array ()
    | hb_filter ([&] (സ്ഥിരം Mapping& _)
		 {
		   തിരികെ unicodes->has (_.unicode_മൂല്യം) || glyphs_requested->has (_.glyphID);
		 })
    ;

    എങ്കിൽ (!it) തിരികെ nullptr;

    HBUINT32 len;
    len = it.len ();
    എങ്കിൽ (unlikely (!c->copy<HBUINT32> (len))) തിരികെ nullptr;

    വേണ്ടി (സ്ഥിരം Mapping& _ : it)
    {
      Mapping mapping;
      mapping.unicode_മൂല്യം = _.unicode_മൂല്യം;
      mapping.glyphID = glyph_map->get (_.glyphID);
      c->copy<Mapping> (mapping);
    }

    തിരികെ out;
  }

  പൊതു:
  DEFINE_SIZE_ARRAY (4, *ഇത്);
};

template <typename Types>
ഘടന VariationSelectorRecord
{
  glyph_variant_t get_glyph (hb_codepoint_t codepoint,
			     hb_codepoint_t *glyph,
			     സ്ഥിരം ശൂന്യം *base) സ്ഥിരം
  {
    എങ്കിൽ ((base+defaultUVS).bfind (codepoint))
      തിരികെ GLYPH_VARIANT_USE_DEFAULT;
    സ്ഥിരം UVSMapping<Types> &nonDefault = (base+nonDefaultUVS).bsearch (codepoint);
    എങ്കിൽ (nonDefault.glyphID)
    {
      *glyph = nonDefault.glyphID;
       തിരികെ GLYPH_VARIANT_FOUND;
    }
    തിരികെ GLYPH_VARIANT_NOT_FOUND;
  }

  VariationSelectorRecord(സ്ഥിരം VariationSelectorRecord& other)
  {
    *ഇത് = other;
  }

  ശൂന്യം operator= (സ്ഥിരം VariationSelectorRecord& other)
  {
    varSelector = other.varSelector;
    HBUINT32 offset = other.defaultUVS;
    defaultUVS = offset;
    offset = other.nonDefaultUVS;
    nonDefaultUVS = offset;
  }

  ശൂന്യം collect_unicodes (hb_set_t *out, സ്ഥിരം ശൂന്യം *base) സ്ഥിരം
  {
    (base+defaultUVS).collect_unicodes (out);
    (base+nonDefaultUVS).collect_unicodes (out);
  }

  ശൂന്യം collect_mapping (സ്ഥിരം ശൂന്യം *base,
			hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping /* OUT */) സ്ഥിരം
  {
    (base+defaultUVS).collect_unicodes (unicodes);
    (base+nonDefaultUVS).collect_mapping (unicodes, mapping);
  }

  പൂർണ്ണസംഖ്യ cmp (സ്ഥിരം hb_codepoint_t &variation_selector) സ്ഥിരം
  { തിരികെ varSelector.cmp (variation_selector); }

  തർക്കം sanitize (hb_sanitize_context_t *c, സ്ഥിരം ശൂന്യം *base) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്) &&
		  defaultUVS.sanitize (c, base) &&
		  nonDefaultUVS.sanitize (c, base));
  }

  hb_pair_t<unsigned, unsigned>
  copy (hb_serialize_context_t *c,
	സ്ഥിരം hb_set_t *unicodes,
	സ്ഥിരം hb_set_t *glyphs_requested,
	സ്ഥിരം hb_map_t *glyph_map,
	സ്ഥിരം ശൂന്യം *base) സ്ഥിരം
  {
    auto snap = c->snapshot ();
    auto *out = c->embed<VariationSelectorRecord> (*ഇത്);
    എങ്കിൽ (unlikely (!out)) തിരികെ hb_pair (0, 0);

    out->defaultUVS = 0;
    out->nonDefaultUVS = 0;

    unsigned non_default_uvs_objidx = 0;
    എങ്കിൽ (nonDefaultUVS != 0)
    {
      c->push ();
      എങ്കിൽ (c->copy (base+nonDefaultUVS, unicodes, glyphs_requested, glyph_map))
	non_default_uvs_objidx = c->എടുക്കുക_pack ();
      അല്ലെങ്കിൽ c->എടുക്കുക_discard ();
    }

    unsigned default_uvs_objidx = 0;
    എങ്കിൽ (defaultUVS != 0)
    {
      c->push ();
      എങ്കിൽ (c->copy (base+defaultUVS, unicodes))
	default_uvs_objidx = c->എടുക്കുക_pack ();
      അല്ലെങ്കിൽ c->എടുക്കുക_discard ();
    }


    എങ്കിൽ (!default_uvs_objidx && !non_default_uvs_objidx)
      c->revert (snap);

    തിരികെ hb_pair (default_uvs_objidx, non_default_uvs_objidx);
  }

  HBUINT24	varSelector;	/* Variation selector. */
  Offset32To<DefaultUVS>
		defaultUVS;	/* Offset to Default UVS Table.  May be 0. */
  Offset32To<NonDefaultUVS<Types>>
		nonDefaultUVS;	/* Offset to Non-Default UVS Table.  May be 0. */
  പൊതു:
  DEFINE_SIZE_STATIC (11);
};

template <typename Types>
ഘടന CmapSubtableFormat14_15
{
  സ്ഥിതം constexpr unsigned format_മൂല്യം = Types::size == 2 ? 14 : 15;
  using Record = VariationSelectorRecord<Types>;
  using NonDefault = NonDefaultUVS<Types>;

  glyph_variant_t get_glyph_variant (hb_codepoint_t codepoint,
				     hb_codepoint_t variation_selector,
				     hb_codepoint_t *glyph) സ്ഥിരം
  { തിരികെ record.bsearch (variation_selector).get_glyph (codepoint, glyph, ഇത്); }

  ശൂന്യം collect_variation_selectors (hb_set_t *out) സ്ഥിരം
  {
    വേണ്ടി (സ്ഥിരം auto& a : record.as_array ())
      out->add (a.varSelector);
  }
  ശൂന്യം collect_variation_unicodes (hb_codepoint_t variation_selector,
				   hb_set_t *out) സ്ഥിരം
  { record.bsearch (variation_selector).collect_unicodes (out, ഇത്); }

  ശൂന്യം serialize (hb_serialize_context_t *c,
		  സ്ഥിരം hb_set_t *unicodes,
		  സ്ഥിരം hb_set_t *glyphs_requested,
		  സ്ഥിരം hb_map_t *glyph_map,
		  സ്ഥിരം ശൂന്യം *base)
  {
    auto snap = c->snapshot ();
    unsigned table_initpos = c->length ();
    സ്ഥിരം അക്ഷരം* init_tail = c->tail;

    എങ്കിൽ (unlikely (!c->extend_min (ഇത്))) തിരികെ;
    ഇത്->format = format_മൂല്യം;

    auto src_tbl = reinterpret_cast<സ്ഥിരം CmapSubtableFormat14_15*> (base);

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
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = src_tbl->record.len - 1; i >= 0; i--)
    {
      എങ്കിൽ (!unicodes->has(src_tbl->record[i].varSelector))
        തുടരുക;

      hb_pair_t<unsigned, unsigned> result = src_tbl->record[i].copy (c, unicodes, glyphs_requested, glyph_map, base);
      എങ്കിൽ (result.first || result.second)
	obj_indices.push (result);
    }

    എങ്കിൽ (c->length () - table_initpos == CmapSubtableFormat14_15::min_size)
    {
      c->revert (snap);
      തിരികെ;
    }

    എങ്കിൽ (unlikely (!c->check_success (!obj_indices.in_error ())))
      തിരികെ;

    പൂർണ്ണസംഖ്യ tail_len = init_tail - c->tail;
    c->check_assign (ഇത്->length, c->length () - table_initpos + tail_len,
                     HB_SERIALIZE_ERROR_INT_OVERFLOW);
    c->check_assign (ഇത്->record.len,
		     (c->length () - table_initpos - CmapSubtableFormat14_15::min_size) /
		     Record::static_size,
                     HB_SERIALIZE_ERROR_INT_OVERFLOW);

    /* Correct the incorrect write order by reversing the order of the variation
       records array. */
    _reverse_variation_records ();

    /* Now that records are in the right order, we can set up the offsets. */
    _add_links_to_variation_records (c, obj_indices);
  }

  ശൂന്യം _reverse_variation_records ()
  {
    record.as_array ().reverse ();
  }

  ശൂന്യം _add_links_to_variation_records (hb_serialize_context_t *c,
					സ്ഥിരം hb_vector_t<hb_pair_t<unsigned, unsigned>>& obj_indices)
  {
    വേണ്ടി (unsigned i = 0; i < obj_indices.length; i++)
    {
      /*
       * Since the record array has been reversed (see comments in copy())
       * but obj_indices has not been, the indices at obj_indices[i]
       * are for the variation record at record[j].
       */
      പൂർണ്ണസംഖ്യ j = obj_indices.length - 1 - i;
      c->add_link (record[j].defaultUVS, obj_indices[i].first);
      c->add_link (record[j].nonDefaultUVS, obj_indices[i].second);
    }
  }

  ശൂന്യം closure_glyphs (സ്ഥിരം hb_set_t      *unicodes,
		       hb_set_t            *glyphset) സ്ഥിരം
  {
    + hb_iter (record)
    | hb_filter (hb_bool, &Record::nonDefaultUVS)
    | hb_filter (unicodes, &Record::varSelector)
    | hb_map (&Record::nonDefaultUVS)
    | hb_map (hb_add (ഇത്))
    | hb_apply ([=] (സ്ഥിരം NonDefault& _) { _.closure_glyphs (unicodes, glyphset); })
    ;
  }

  ശൂന്യം collect_unicodes (hb_set_t *out) സ്ഥിരം
  {
    വേണ്ടി (സ്ഥിരം Record& _ : record)
      _.collect_unicodes (out, ഇത്);
  }

  ശൂന്യം collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping /* OUT */) സ്ഥിരം
  {
    വേണ്ടി (സ്ഥിരം Record& _ : record)
      _.collect_mapping (ഇത്, unicodes, mapping);
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്) &&
		  record.sanitize (c, ഇത്));
  }

  സംരക്ഷിത:
  HBUINT16	format;		/* Format number is set to 14 or 15. */
  HBUINT32	length;		/* Byte length of this subtable. */
  SortedArray32Of<Record>
		record;		/* Variation selector records; sorted
				 * in increasing order of `varSelector'. */
  പൊതു:
  DEFINE_SIZE_ARRAY (10, record);
};

ഘടന CmapSubtable
{
  /* Note: We intentionally do NOT implement subtable formats 2 and 8. */

  തർക്കം is_variation_selector () സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (u.format.v) {
    അവസ്ഥ 14:
#ifndef HB_NO_BEYOND_64K
    അവസ്ഥ 15:
#endif
      തിരികെ സത്യം;
    സ്ഥിരസ്ഥിതി:
      തിരികെ അസത്യം;
    }
  }

  glyph_variant_t get_glyph_variant (hb_codepoint_t codepoint,
				     hb_codepoint_t variation_selector,
				     hb_codepoint_t *glyph) സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (u.format.v) {
    അവസ്ഥ 14: hb_barrier (); തിരികെ u.format14.get_glyph_variant (codepoint, variation_selector, glyph);
#ifndef HB_NO_BEYOND_64K
    അവസ്ഥ 15: hb_barrier (); തിരികെ u.format15.get_glyph_variant (codepoint, variation_selector, glyph);
#endif
    സ്ഥിരസ്ഥിതി: തിരികെ GLYPH_VARIANT_NOT_FOUND;
    }
  }

  ശൂന്യം collect_variation_selectors (hb_set_t *out) സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (u.format.v) {
    അവസ്ഥ 14: hb_barrier (); u.format14.collect_variation_selectors (out); തിരികെ;
#ifndef HB_NO_BEYOND_64K
    അവസ്ഥ 15: hb_barrier (); u.format15.collect_variation_selectors (out); തിരികെ;
#endif
    സ്ഥിരസ്ഥിതി: തിരികെ;
    }
  }

  ശൂന്യം collect_variation_unicodes (hb_codepoint_t variation_selector,
				    hb_set_t *out) സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (u.format.v) {
    അവസ്ഥ 14: hb_barrier (); u.format14.collect_variation_unicodes (variation_selector, out); തിരികെ;
#ifndef HB_NO_BEYOND_64K
    അവസ്ഥ 15: hb_barrier (); u.format15.collect_variation_unicodes (variation_selector, out); തിരികെ;
#endif
    സ്ഥിരസ്ഥിതി: തിരികെ;
    }
  }

  ശൂന്യം closure_glyphs (സ്ഥിരം hb_set_t *unicodes, hb_set_t *glyphset) സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (u.format.v) {
    അവസ്ഥ 14: hb_barrier (); u.format14.closure_glyphs (unicodes, glyphset); തിരികെ;
#ifndef HB_NO_BEYOND_64K
    അവസ്ഥ 15: hb_barrier (); u.format15.closure_glyphs (unicodes, glyphset); തിരികെ;
#endif
    സ്ഥിരസ്ഥിതി: തിരികെ;
    }
  }

  തർക്കം get_glyph (hb_codepoint_t codepoint,
		  hb_codepoint_t *glyph,
		  unsigned പൂർണ്ണസംഖ്യ subtable_data_size = 0) സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (u.format.v) {
    അവസ്ഥ  0: hb_barrier (); തിരികെ u.format0 .get_glyph (codepoint, glyph);
    അവസ്ഥ  4: hb_barrier (); തിരികെ subtable_data_size
				      ? u.format4.get_glyph (codepoint, glyph, subtable_data_size)
				      : അസത്യം;
    അവസ്ഥ  6: hb_barrier (); തിരികെ u.format6 .get_glyph (codepoint, glyph);
    അവസ്ഥ 10: hb_barrier (); തിരികെ u.format10.get_glyph (codepoint, glyph);
    അവസ്ഥ 12: hb_barrier (); തിരികെ u.format12.get_glyph (codepoint, glyph);
    അവസ്ഥ 13: hb_barrier (); തിരികെ u.format13.get_glyph (codepoint, glyph);
    അവസ്ഥ 14:
    സ്ഥിരസ്ഥിതി: തിരികെ അസത്യം;
    }
  }
  ശൂന്യം collect_unicodes (hb_set_t *out,
			 unsigned പൂർണ്ണസംഖ്യ അക്കം_glyphs = UINT_MAX,
			 unsigned പൂർണ്ണസംഖ്യ subtable_data_size = 0) സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (u.format.v) {
    അവസ്ഥ  0: hb_barrier (); u.format0 .collect_unicodes (out); തിരികെ;
    അവസ്ഥ  4: hb_barrier (); എങ്കിൽ (subtable_data_size)
			      u.format4.collect_unicodes (out, subtable_data_size);
			    അല്ലെങ്കിൽ
			      u.format4.collect_unicodes (out);
			    തിരികെ;
    അവസ്ഥ  6: hb_barrier (); u.format6 .collect_unicodes (out); തിരികെ;
    അവസ്ഥ 10: hb_barrier (); u.format10.collect_unicodes (out); തിരികെ;
    അവസ്ഥ 12: hb_barrier (); u.format12.collect_unicodes (out, അക്കം_glyphs); തിരികെ;
    അവസ്ഥ 13: hb_barrier (); u.format13.collect_unicodes (out, അക്കം_glyphs); തിരികെ;
    അവസ്ഥ 14:
    സ്ഥിരസ്ഥിതി: തിരികെ;
    }
  }

  ശൂന്യം collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping, /* OUT */
			unsigned അക്കം_glyphs = UINT_MAX,
			unsigned പൂർണ്ണസംഖ്യ subtable_data_size = 0) സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (u.format.v) {
    അവസ്ഥ  0: hb_barrier (); u.format0 .collect_mapping (unicodes, mapping); തിരികെ;
    അവസ്ഥ  4: hb_barrier (); എങ്കിൽ (subtable_data_size)
			      u.format4.collect_mapping (unicodes, mapping, subtable_data_size);
			    അല്ലെങ്കിൽ
			      u.format4.collect_mapping (unicodes, mapping);
			    തിരികെ;
    അവസ്ഥ  6: hb_barrier (); u.format6 .collect_mapping (unicodes, mapping); തിരികെ;
    അവസ്ഥ 10: hb_barrier (); u.format10.collect_mapping (unicodes, mapping); തിരികെ;
    അവസ്ഥ 12: hb_barrier (); u.format12.collect_mapping (unicodes, mapping, അക്കം_glyphs); തിരികെ;
    അവസ്ഥ 13: hb_barrier (); u.format13.collect_mapping (unicodes, mapping, അക്കം_glyphs); തിരികെ;
    അവസ്ഥ 14:
    സ്ഥിരസ്ഥിതി: തിരികെ;
    }
  }

  unsigned get_language () സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (u.format.v) {
    അവസ്ഥ  0: hb_barrier (); തിരികെ u.format0 .get_language ();
    അവസ്ഥ  4: hb_barrier (); തിരികെ u.format4 .get_language ();
    അവസ്ഥ  6: hb_barrier (); തിരികെ u.format6 .get_language ();
    അവസ്ഥ 10: hb_barrier (); തിരികെ u.format10.get_language ();
    അവസ്ഥ 12: hb_barrier (); തിരികെ u.format12.get_language ();
    അവസ്ഥ 13: hb_barrier (); തിരികെ u.format13.get_language ();
    അവസ്ഥ 14:
    സ്ഥിരസ്ഥിതി: തിരികെ 0;
    }
  }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  ശൂന്യം serialize (hb_serialize_context_t *c,
		  Iterator it,
		  unsigned format,
		  സ്ഥിരം hb_subset_plan_t *plan,
		  സ്ഥിരം ശൂന്യം *base)
  {
    തിരഞ്ഞെടുപ്പ് (format) {
    അവസ്ഥ  4: hb_barrier (); തിരികെ u.format4.serialize (c, it);
    അവസ്ഥ 12: hb_barrier (); തിരികെ u.format12.serialize (c, it);
    അവസ്ഥ 14: hb_barrier (); തിരികെ u.format14.serialize (c, &plan->unicodes, &plan->glyphs_requested, plan->glyph_map, base);
#ifndef HB_NO_BEYOND_64K
    അവസ്ഥ 15: hb_barrier (); തിരികെ u.format15.serialize (c, &plan->unicodes, &plan->glyphs_requested, plan->glyph_map, base);
#endif
    സ്ഥിരസ്ഥിതി: തിരികെ;
    }
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    എങ്കിൽ (!u.format.v.sanitize (c)) return_trace (അസത്യം);
    hb_barrier ();
    തിരഞ്ഞെടുപ്പ് (u.format.v) {
    അവസ്ഥ  0: hb_barrier (); return_trace (u.format0 .sanitize (c));
    അവസ്ഥ  4: hb_barrier (); return_trace (u.format4 .sanitize (c));
    അവസ്ഥ  6: hb_barrier (); return_trace (u.format6 .sanitize (c));
    അവസ്ഥ 10: hb_barrier (); return_trace (u.format10.sanitize (c));
    അവസ്ഥ 12: hb_barrier (); return_trace (u.format12.sanitize (c));
    അവസ്ഥ 13: hb_barrier (); return_trace (u.format13.sanitize (c));
    അവസ്ഥ 14: hb_barrier (); return_trace (u.format14.sanitize (c));
#ifndef HB_NO_BEYOND_64K
    അവസ്ഥ 15: hb_barrier (); return_trace (u.format15.sanitize (c));
#endif
    സ്ഥിരസ്ഥിതി:return_trace (സത്യം);
    }
  }

  പൊതു:
  സംഘം {
  ഘടന { HBUINT16 v; }	format;		/* Format identifier */
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
  പൊതു:
  DEFINE_SIZE_UNION (2, format.v);
};


ഘടന EncodingRecord
{
  പൂർണ്ണസംഖ്യ cmp (സ്ഥിരം EncodingRecord &other) സ്ഥിരം
  {
    പൂർണ്ണസംഖ്യ ret;
    ret = platformID.cmp (other.platformID);
    എങ്കിൽ (ret) തിരികെ ret;
    എങ്കിൽ (other.encodingID != 0xFFFF)
    {
      ret = encodingID.cmp (other.encodingID);
      എങ്കിൽ (ret) തിരികെ ret;
    }
    തിരികെ 0;
  }

  തർക്കം sanitize (hb_sanitize_context_t *c, സ്ഥിരം ശൂന്യം *base) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്) &&
		  subtable.sanitize (c, base));
  }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  EncodingRecord* copy (hb_serialize_context_t *c,
			Iterator it,
			unsigned format,
			സ്ഥിരം ശൂന്യം *base,
			സ്ഥിരം hb_subset_plan_t *plan,
			/* INOUT */ unsigned *objidx) സ്ഥിരം
  {
    TRACE_SERIALIZE (ഇത്);
    auto snap = c->snapshot ();
    auto *out = c->embed (ഇത്);
    എങ്കിൽ (unlikely (!out)) return_trace (nullptr);
    out->subtable = 0;

    എങ്കിൽ (*objidx == 0)
    {
      CmapSubtable *cmapsubtable = c->push<CmapSubtable> ();
      unsigned origin_length = c->length ();
      cmapsubtable->serialize (c, it, format, plan, &(base+subtable));
      എങ്കിൽ (c->length () > origin_length && !c->in_error()) *objidx = c->എടുക്കുക_pack ();
      അല്ലെങ്കിൽ c->എടുക്കുക_discard ();
    }

    എങ്കിൽ (*objidx == 0)
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
  പൊതു:
  DEFINE_SIZE_STATIC (8);
};

ഘടന cmap;

ഘടന SubtableUnicodesCache {

 സ്വകാര്യ:
  hb_blob_ptr_t<cmap> base_blob;
  സ്ഥിരം അക്ഷരം* base;
  unsigned പൂർണ്ണസംഖ്യ table_length;
  hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> cached_unicodes;

 പൊതു:

  സ്ഥിതം SubtableUnicodesCache* create (hb_blob_ptr_t<cmap> source_table)
  {
    SubtableUnicodesCache* cache =
        (SubtableUnicodesCache*) hb_malloc (വലുപ്പം(SubtableUnicodesCache));

    എങ്കിൽ (unlikely (!cache))
      തിരികെ nullptr;

    പുതിയ (cache) SubtableUnicodesCache (source_table);
    തിരികെ cache;
  }

  സ്ഥിതം ശൂന്യം destroy (ശൂന്യം* value) {
    എങ്കിൽ (!value) തിരികെ;

    SubtableUnicodesCache* cache = (SubtableUnicodesCache*) value;
    cache->~SubtableUnicodesCache ();
    hb_free (cache);
  }

  SubtableUnicodesCache(സ്ഥിരം ശൂന്യം* cmap_base,
			unsigned പൂർണ്ണസംഖ്യ table_length_ = 0)
      : base_blob(),
        base ((സ്ഥിരം അക്ഷരം*) cmap_base),
        table_length (table_length_),
        cached_unicodes ()
  {}

  SubtableUnicodesCache(hb_blob_ptr_t<cmap> base_blob_)
      : base_blob(base_blob_),
        base ((സ്ഥിരം അക്ഷരം *) base_blob.get()),
        table_length (base_blob.get_length ()),
        cached_unicodes ()
  {}

  ~SubtableUnicodesCache()
  {
    base_blob.destroy ();
  }

  തർക്കം same_base(സ്ഥിരം ശൂന്യം* other) സ്ഥിരം
  {
    തിരികെ other == (സ്ഥിരം ശൂന്യം*) base;
  }

  സ്ഥിരം hb_set_t* set_for (സ്ഥിരം EncodingRecord* record,
                           SubtableUnicodesCache& mutable_cache) സ്ഥിരം
  {
    എങ്കിൽ (cached_unicodes.has ((unsigned) ((സ്ഥിരം അക്ഷരം *) record - base)))
      തിരികെ cached_unicodes.get ((unsigned) ((സ്ഥിരം അക്ഷരം *) record - base));

    തിരികെ mutable_cache.set_for (record);
  }

  സ്ഥിരം hb_set_t* set_for (സ്ഥിരം EncodingRecord* record)
  {
    എങ്കിൽ (!cached_unicodes.has ((unsigned) ((സ്ഥിരം അക്ഷരം *) record - base)))
    {
      hb_set_t *s = hb_set_create ();
      എങ്കിൽ (unlikely (s->in_error ()))
	തിരികെ hb_set_get_empty ();

      unsigned പൂർണ്ണസംഖ്യ subtable_data_size = record->subtable < table_length
					? table_length - (unsigned പൂർണ്ണസംഖ്യ) record->subtable
					: 0;
      (base+record->subtable).collect_unicodes (s, UINT_MAX, subtable_data_size);

      എങ്കിൽ (unlikely (!cached_unicodes.set ((unsigned) ((സ്ഥിരം അക്ഷരം *) record - base), hb::unique_ptr<hb_set_t> {s})))
        തിരികെ hb_set_get_empty ();

      തിരികെ s;
    }
    തിരികെ cached_unicodes.get ((unsigned) ((സ്ഥിരം അക്ഷരം *) record - base));
  }

};

സ്ഥിതം വരിയിൽ uint16_t
hb_ചിഹ്നം_pua_map (unsigned codepoint)
{
  എങ്കിൽ (codepoint <= 0x00FFu)
  {
    /* For symbol-encoded OpenType fonts, we duplicate the
     * U+F000..F0FF range at U+0000..U+00FF.  That's what
     * Windows seems to do, and that's hinted about at:
     * https://docs.microsoft.com/en-us/typography/opentype/spec/recom
     * under "Non-Standard (Symbol) Fonts". */
    തിരികെ 0xF000u + codepoint;
  }
  തിരികെ 0;
}

ഘടന cmap
{
  സ്ഥിതം constexpr hb_tag_t tableTag = HB_OT_TAG_cmap;


  സ്ഥിതം SubtableUnicodesCache* create_filled_cache(hb_blob_ptr_t<cmap> source_table) {
    സ്ഥിരം cmap* cmap = source_table.get();
    auto it =
    + hb_iter (cmap->encodingRecord)
    | hb_filter ([&](സ്ഥിരം EncodingRecord& _) {
      തിരികെ cmap::filter_encoding_records_for_subset (cmap, _);
    })
    ;

    SubtableUnicodesCache* cache = SubtableUnicodesCache::create(source_table);

    എങ്കിൽ (unlikely (!cache))
      തിരികെ nullptr;

    വേണ്ടി (സ്ഥിരം EncodingRecord& _ : it)
      cache->set_for(&_); // populate the cache for this encoding record.

    തിരികെ cache;
  }

  template<typename Iterator, typename EncodingRecIter,
	   hb_requires (hb_is_iterator (EncodingRecIter))>
  തർക്കം serialize (hb_serialize_context_t *c,
		  Iterator it,
		  EncodingRecIter encodingrec_iter,
		  സ്ഥിരം ശൂന്യം *base,
		  hb_subset_plan_t *plan,
	                  തർക്കം drop_format_4 = അസത്യം,
			  unsigned പൂർണ്ണസംഖ്യ source_table_length = 0)
  {
    എങ്കിൽ (unlikely (!c->extend_min ((*ഇത്))))  തിരികെ അസത്യം;
    ഇത്->version = 0;

    unsigned format4objidx = 0, format12objidx = 0, format14objidx = 0;
#ifndef HB_NO_BEYOND_64K
    unsigned format15objidx = 0;
#endif
    auto snap = c->snapshot ();

    SubtableUnicodesCache local_unicodes_cache (base, source_table_length);
    സ്ഥിരം SubtableUnicodesCache* unicodes_cache = &local_unicodes_cache;

    എങ്കിൽ (plan->accelerator &&
        plan->accelerator->cmap_cache &&
        plan->accelerator->cmap_cache->same_base (base))
      unicodes_cache = plan->accelerator->cmap_cache;

    വേണ്ടി (സ്ഥിരം EncodingRecord& _ : encodingrec_iter)
    {
      എങ്കിൽ (c->in_error ())
        തിരികെ അസത്യം;

      unsigned format = (base+_.subtable).u.format.v;
      എങ്കിൽ (format != 4 && format != 12 && format != 14
#ifndef HB_NO_BEYOND_64K
	  && format != 15
#endif
	 ) തുടരുക;

      സ്ഥിരം hb_set_t* unicodes_set = unicodes_cache->set_for (&_, local_unicodes_cache);

      എങ്കിൽ (!drop_format_4 && format == 4)
      {
        c->copy (_, + it | hb_filter (*unicodes_set, hb_first), 4u, base, plan, &format4objidx);
        എങ്കിൽ (c->in_error () && c->only_overflow ())
        {
          // cmap4 overflowed, reset and retry serialization without format 4 subtables.
          c->revert (snap);
          തിരികെ serialize (c, it,
                            encodingrec_iter,
                            base,
                            plan,
                            സത്യം,
			    source_table_length);
        }
      }

      അല്ലെങ്കിൽ എങ്കിൽ (format == 12)
      {
        എങ്കിൽ (_can_drop (_,
                       *unicodes_set,
                       base,
                       *unicodes_cache,
                       local_unicodes_cache,
                       + it | hb_map (hb_first), encodingrec_iter))
          തുടരുക;
        c->copy (_, + it | hb_filter (*unicodes_set, hb_first), 12u, base, plan, &format12objidx);
      }
      അല്ലെങ്കിൽ എങ്കിൽ (format == 14) c->copy (_, it, 14u, base, plan, &format14objidx);
#ifndef HB_NO_BEYOND_64K
      അല്ലെങ്കിൽ എങ്കിൽ (format == 15) c->copy (_, it, 15u, base, plan, &format15objidx);
#endif
    }
        unsigned length = c->length ();
        unsigned available = length > cmap::min_size ? length - cmap::min_size : 0;
        c->check_assign(ഇത്->encodingRecord.len,
                        available / EncodingRecord::static_size,
                        HB_SERIALIZE_ERROR_INT_OVERFLOW);

    // Fail if format 4 was dropped and there is no cmap12.
    തിരികെ !drop_format_4 || format12objidx;
  }

  template<typename Iterator, typename EncodingRecordIterator,
      hb_requires (hb_is_iterator (Iterator)),
      hb_requires (hb_is_iterator (EncodingRecordIterator))>
  തർക്കം _can_drop (സ്ഥിരം EncodingRecord& cmap12,
                  സ്ഥിരം hb_set_t& cmap12_unicodes,
                  സ്ഥിരം ശൂന്യം* base,
                  സ്ഥിരം SubtableUnicodesCache& unicodes_cache,
                  SubtableUnicodesCache& local_unicodes_cache,
                  Iterator subset_unicodes,
                  EncodingRecordIterator encoding_records)
  {
    വേണ്ടി (auto cp : + subset_unicodes | hb_filter (cmap12_unicodes))
    {
      എങ്കിൽ (cp >= 0x10000) തിരികെ അസത്യം;
    }

    unsigned target_platform;
    unsigned target_encoding;
    unsigned target_language = (base+cmap12.subtable).get_language ();

    എങ്കിൽ (cmap12.platformID == 0 && cmap12.encodingID == 4)
    {
      target_platform = 0;
      target_encoding = 3;
    } അല്ലെങ്കിൽ എങ്കിൽ (cmap12.platformID == 3 && cmap12.encodingID == 10) {
      target_platform = 3;
      target_encoding = 1;
    } അല്ലെങ്കിൽ {
      തിരികെ അസത്യം;
    }

    വേണ്ടി (സ്ഥിരം auto& _ : encoding_records)
    {
      എങ്കിൽ (_.platformID != target_platform
          || _.encodingID != target_encoding
          || (base+_.subtable).get_language() != target_language)
        തുടരുക;

      സ്ഥിരം hb_set_t* sibling_unicodes = unicodes_cache.set_for (&_, local_unicodes_cache);

      auto cmap12 = + subset_unicodes | hb_filter (cmap12_unicodes);
      auto sibling = + subset_unicodes | hb_filter (*sibling_unicodes);
      വേണ്ടി (; cmap12 && sibling; cmap12++, sibling++)
      {
        unsigned a = *cmap12;
        unsigned b = *sibling;
        എങ്കിൽ (a != b) തിരികെ അസത്യം;
      }

      തിരികെ !cmap12 && !sibling;
    }

    തിരികെ അസത്യം;
  }

  ശൂന്യം closure_glyphs (സ്ഥിരം hb_set_t      *unicodes,
		       hb_set_t            *glyphset) സ്ഥിരം
  {
    + hb_iter (encodingRecord)
    | hb_map (&EncodingRecord::subtable)
    | hb_map (hb_add (ഇത്))
    | hb_filter (&CmapSubtable::is_variation_selector)
    | hb_apply ([=] (സ്ഥിരം CmapSubtable& _) { _.closure_glyphs (unicodes, glyphset); })
    ;
  }

  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  { തിരികെ subset (c, അസത്യം); }

  തർക്കം subset (hb_subset_context_t *c, തർക്കം is_dmap) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);

    cmap *cmap_അഭാജ്യം = c->serializer->start_embed<cmap> ();

    auto encodingrec_iter =
    + hb_iter (encodingRecord)
    | hb_filter ([&](സ്ഥിരം EncodingRecord& _) {
      തിരികെ cmap::filter_encoding_records_for_subset (ഇത്, _);
    })
    ;

    എങ്കിൽ (unlikely (!encodingrec_iter.len ())) return_trace (അസത്യം);

    സ്ഥിരം EncodingRecord *unicode_bmp= nullptr, *unicode_ucs4 = nullptr, *ms_bmp = nullptr, *ms_ucs4 = nullptr;
    തർക്കം has_format12 = അസത്യം;

    വേണ്ടി (സ്ഥിരം EncodingRecord& _ : encodingrec_iter)
    {
      unsigned format = (ഇത് + _.subtable).u.format.v;
      എങ്കിൽ (format == 12) has_format12 = സത്യം;

      സ്ഥിരം EncodingRecord *table = std::addressof (_);
      എങ്കിൽ      (_.platformID == 0 && _.encodingID ==  3) unicode_bmp = table;
      അല്ലെങ്കിൽ എങ്കിൽ (_.platformID == 0 && _.encodingID ==  4) unicode_ucs4 = table;
      അല്ലെങ്കിൽ എങ്കിൽ (_.platformID == 3 && _.encodingID ==  1) ms_bmp = table;
      അല്ലെങ്കിൽ എങ്കിൽ (_.platformID == 3 && _.encodingID == 10) ms_ucs4 = table;
    }

    എങ്കിൽ (unlikely (!is_dmap && !has_format12 && !unicode_bmp && !ms_bmp)) return_trace (അസത്യം);
    എങ്കിൽ (unlikely (!is_dmap && has_format12 && (!unicode_ucs4 && !ms_ucs4))) return_trace (അസത്യം);

    auto it =
    + c->plan->unicode_to_new_gid_list.iter ()
    | hb_filter ([&] (സ്ഥിരം hb_codepoint_pair_t _)
		 { തിരികെ (_.second != HB_MAP_മൂല്യം_INVALID); })
    ;

    return_trace (cmap_അഭാജ്യം->serialize (c->serializer,
                                         it,
                                         encodingrec_iter,
                                         ഇത്,
                                         c->plan,
					 അസത്യം,
					 c->source_blob->length));
  }

  സ്ഥിരം CmapSubtable *find_best_subtable (തർക്കം *ചിഹ്നം = nullptr,
					  തർക്കം *mac = nullptr,
					  തർക്കം *macroman = nullptr) സ്ഥിരം
  {
    എങ്കിൽ (ചിഹ്നം) *ചിഹ്നം = അസത്യം;
    എങ്കിൽ (mac) *mac = അസത്യം;
    എങ്കിൽ (macroman) *macroman = അസത്യം;

    സ്ഥിരം CmapSubtable *subtable;

    /* Symbol subtable.
     * Prefer symbol if available.
     * https://github.com/harfbuzz/harfbuzz/issues/1918 */
    എങ്കിൽ ((subtable = ഇത്->find_subtable (3, 0)))
    {
      എങ്കിൽ (ചിഹ്നം) *ചിഹ്നം = സത്യം;
      തിരികെ subtable;
    }

    /* 32-bit subtables. */
    എങ്കിൽ ((subtable = ഇത്->find_subtable (3, 10))) തിരികെ subtable;
    എങ്കിൽ ((subtable = ഇത്->find_subtable (0, 6))) തിരികെ subtable;
    എങ്കിൽ ((subtable = ഇത്->find_subtable (0, 4))) തിരികെ subtable;

    /* 16-bit subtables. */
    എങ്കിൽ ((subtable = ഇത്->find_subtable (3, 1))) തിരികെ subtable;
    എങ്കിൽ ((subtable = ഇത്->find_subtable (0, 3))) തിരികെ subtable;
    എങ്കിൽ ((subtable = ഇത്->find_subtable (0, 2))) തിരികെ subtable;
    എങ്കിൽ ((subtable = ഇത്->find_subtable (0, 1))) തിരികെ subtable;
    എങ്കിൽ ((subtable = ഇത്->find_subtable (0, 0))) തിരികെ subtable;

    /* MacRoman subtable. */
    എങ്കിൽ ((subtable = ഇത്->find_subtable (1, 0)))
    {
      എങ്കിൽ (mac) *mac = സത്യം;
      എങ്കിൽ (macroman) *macroman = സത്യം;
      തിരികെ subtable;
    }
    /* Any other Mac subtable; we just map ASCII for these. */
    എങ്കിൽ ((subtable = ഇത്->find_subtable (1, 0xFFFF)))
    {
      എങ്കിൽ (mac) *mac = സത്യം;
      തിരികെ subtable;
    }

    /* Meh. */
    തിരികെ &Null (CmapSubtable);
  }

  ഘടന accelerator_t
  {
    using cache_t = hb_cache_t<21, 19>;
    static_assert (വലുപ്പം (cache_t) == 1024, "");

    accelerator_t (hb_face_t *face, hb_tag_t table_tag = cmap::tableTag)
    {
      ഇത്->table = hb_sanitize_context_t ().reference_table<cmap> (face, table_tag);
      എങ്കിൽ (!ഇത്->table.get_length ())
	തിരികെ;

      തർക്കം ചിഹ്നം, mac, macroman;
      ഇത്->subtable = table->find_best_subtable (&ചിഹ്നം, &mac, &macroman);
      ഇത്->subtable_uvs = &Null (CmapSubtable);
      {
	സ്ഥിരം CmapSubtable *st = table->find_variation_selector_subtable ();
	എങ്കിൽ (st)
	  subtable_uvs = st;
      }

#ifndef HB_NO_OT_FONT_CMAP_CACHE
      cache = (cache_t *) hb_malloc (വലുപ്പം (cache_t));
      എങ്കിൽ (cache)
	പുതിയ (cache) cache_t ();
      അല്ലെങ്കിൽ
        തിരികെ; // Such that get_glyph_funcZ remains null.
#endif

      ഇത്->get_glyph_data = subtable;
#ifndef HB_NO_CMAP_LEGACY_SUBTABLES
      തർക്കം is_format4 = subtable->u.format.v == 4;
      auto set_format4_getter = [ഇത്] (തർക്കം (*func) (സ്ഥിരം ശൂന്യം *,
						      hb_codepoint_t,
						      hb_codepoint_t *))
      {
	ഇത്->format4_accel.init (&ഇത്->subtable->u.format4,
				  get_subtable_data_size (ഇത്->subtable));
	ഇത്->get_glyph_data = &ഇത്->format4_accel;
	ഇത്->get_glyph_funcZ = func;
      };
      എങ്കിൽ (unlikely (ചിഹ്നം))
      {
	തിരഞ്ഞെടുപ്പ് ((unsigned) face->table.OS2->get_font_page ()) {
	അവസ്ഥ OS2::font_page_t::FONT_PAGE_NONE:
	  എങ്കിൽ (is_format4)
	    set_format4_getter (get_glyph_from_ചിഹ്നം<CmapSubtableFormat4::accelerator_t, hb_ചിഹ്നം_pua_map>);
	  അല്ലെങ്കിൽ
	    ഇത്->get_glyph_funcZ = get_glyph_from_ചിഹ്നം<CmapSubtable, hb_ചിഹ്നം_pua_map>;
	  നിർത്തുക;
#ifndef HB_NO_OT_SHAPER_ARABIC_FALLBACK
	അവസ്ഥ OS2::font_page_t::FONT_PAGE_SIMP_ARABIC:
	  എങ്കിൽ (is_format4)
	    set_format4_getter (get_glyph_from_ചിഹ്നം<CmapSubtableFormat4::accelerator_t, _hb_arabic_pua_simp_map>);
	  അല്ലെങ്കിൽ
	    ഇത്->get_glyph_funcZ = get_glyph_from_ചിഹ്നം<CmapSubtable, _hb_arabic_pua_simp_map>;
	  നിർത്തുക;
	അവസ്ഥ OS2::font_page_t::FONT_PAGE_TRAD_ARABIC:
	  എങ്കിൽ (is_format4)
	    set_format4_getter (get_glyph_from_ചിഹ്നം<CmapSubtableFormat4::accelerator_t, _hb_arabic_pua_trad_map>);
	  അല്ലെങ്കിൽ
	    ഇത്->get_glyph_funcZ = get_glyph_from_ചിഹ്നം<CmapSubtable, _hb_arabic_pua_trad_map>;
	  നിർത്തുക;
#endif
	സ്ഥിരസ്ഥിതി:
	  എങ്കിൽ (is_format4)
	    set_format4_getter (get_glyph_from<CmapSubtableFormat4::accelerator_t>);
	  അല്ലെങ്കിൽ
	    ഇത്->get_glyph_funcZ = get_glyph_from<CmapSubtable>;
	  നിർത്തുക;
	}
      }
      അല്ലെങ്കിൽ എങ്കിൽ (unlikely (macroman))
      {
	എങ്കിൽ (is_format4)
	  set_format4_getter (get_glyph_from_macroman<CmapSubtableFormat4::accelerator_t>);
	അല്ലെങ്കിൽ
	  ഇത്->get_glyph_funcZ = get_glyph_from_macroman<CmapSubtable>;
      }
      അല്ലെങ്കിൽ എങ്കിൽ (unlikely (mac))
      {
	എങ്കിൽ (is_format4)
	  set_format4_getter (get_glyph_from_ascii<CmapSubtableFormat4::accelerator_t>);
	അല്ലെങ്കിൽ
	  ഇത്->get_glyph_funcZ = get_glyph_from_ascii<CmapSubtable>;
      }
      അല്ലെങ്കിൽ
#endif
      {
	തിരഞ്ഞെടുപ്പ് (subtable->u.format.v) {
	  /* Accelerate format 4 and format 12. */
	  സ്ഥിരസ്ഥിതി:
	    ഇത്->get_glyph_funcZ = get_glyph_from<CmapSubtable>;
	    നിർത്തുക;
	  അവസ്ഥ 12:
	    ഇത്->get_glyph_funcZ = get_glyph_from<CmapSubtableFormat12>;
	    നിർത്തുക;
	  അവസ്ഥ  4:
	  {
	    hb_barrier ();
	    ഇത്->format4_accel.init (&subtable->u.format4,
				      get_subtable_data_size (subtable));
	    ഇത്->get_glyph_data = &ഇത്->format4_accel;
	    ഇത്->get_glyph_funcZ = ഇത്->format4_accel.get_glyph_func;
	    നിർത്തുക;
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

    വരിയിൽ തർക്കം _cached_get (hb_codepoint_t unicode,
			     hb_codepoint_t *glyph) സ്ഥിരം
    {
#ifndef HB_NO_OT_FONT_CMAP_CACHE
      // cache is always non-null if we have a get_glyph_funcZ
      unsigned v;
      എങ്കിൽ (cache->get (unicode, &v))
      {
        *glyph = v;
	തിരികെ സത്യം;
      }
#endif
      തർക്കം ret  = ഇത്->get_glyph_funcZ (ഇത്->get_glyph_data, unicode, glyph);

#ifndef HB_NO_OT_FONT_CMAP_CACHE
      എങ്കിൽ (ret)
        cache->set (unicode, *glyph);
#endif

      തിരികെ ret;
    }

    തർക്കം get_nominal_glyph (hb_codepoint_t  unicode,
			    hb_codepoint_t *glyph) സ്ഥിരം
    {
      എങ്കിൽ (unlikely (!ഇത്->get_glyph_funcZ)) തിരികെ അസത്യം;
      തിരികെ _cached_get (unicode, glyph);
    }

    unsigned പൂർണ്ണസംഖ്യ get_nominal_glyphs (unsigned പൂർണ്ണസംഖ്യ എണ്ണം,
				     സ്ഥിരം hb_codepoint_t *first_unicode,
				     unsigned പൂർണ്ണസംഖ്യ unicode_stride,
				     hb_codepoint_t *first_glyph,
				     unsigned പൂർണ്ണസംഖ്യ glyph_stride) സ്ഥിരം
    {
      എങ്കിൽ (unlikely (!ഇത്->get_glyph_funcZ)) തിരികെ 0;

      unsigned പൂർണ്ണസംഖ്യ done;
      വേണ്ടി (done = 0;
	   done < എണ്ണം && _cached_get (*first_unicode, first_glyph);
	   done++)
      {
	first_unicode = &StructAtOffsetUnaligned<hb_codepoint_t> (first_unicode, unicode_stride);
	first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      }
      തിരികെ done;
    }

    തർക്കം get_variation_glyph (hb_codepoint_t  unicode,
			      hb_codepoint_t  variation_selector,
			      hb_codepoint_t *glyph) സ്ഥിരം
    {
      തിരഞ്ഞെടുപ്പ് (get_glyph_variant (unicode, variation_selector, glyph))
      {
	അവസ്ഥ GLYPH_VARIANT_NOT_FOUND:	തിരികെ അസത്യം;
	അവസ്ഥ GLYPH_VARIANT_FOUND:	തിരികെ സത്യം;
	അവസ്ഥ GLYPH_VARIANT_USE_DEFAULT:	നിർത്തുക;
      }

      തിരികെ get_nominal_glyph (unicode, glyph);
    }

    glyph_variant_t get_glyph_variant (hb_codepoint_t  unicode,
				       hb_codepoint_t  variation_selector,
				       hb_codepoint_t *glyph) സ്ഥിരം
    {
      തിരികെ ഇത്->subtable_uvs->get_glyph_variant (unicode,
						    variation_selector,
						    glyph);
    }

    തർക്കം has_data () സ്ഥിരം { തിരികെ table.get_length (); }

    ശൂന്യം collect_unicodes (hb_set_t *out, unsigned പൂർണ്ണസംഖ്യ അക്കം_glyphs) സ്ഥിരം
    { subtable->collect_unicodes (out, അക്കം_glyphs, get_subtable_data_size (subtable)); }
    ശൂന്യം collect_mapping (hb_set_t *unicodes, hb_map_t *mapping,
			  unsigned അക്കം_glyphs = UINT_MAX) സ്ഥിരം
    { subtable->collect_mapping (unicodes, mapping, അക്കം_glyphs, get_subtable_data_size (subtable)); }
    ശൂന്യം collect_variation_selectors (hb_set_t *out) സ്ഥിരം
    { subtable_uvs->collect_variation_selectors (out); }
    ശൂന്യം collect_variation_unicodes (hb_codepoint_t variation_selector,
				     hb_set_t *out) സ്ഥിരം
    { subtable_uvs->collect_variation_unicodes (variation_selector, out); }

    സംരക്ഷിത:
    തരനിർവചനം തർക്കം (*hb_cmap_get_glyph_func_t) (സ്ഥിരം ശൂന്യം *obj,
					      hb_codepoint_t codepoint,
					      hb_codepoint_t *glyph);
    തരനിർവചനം uint16_t (*hb_pua_remap_func_t) (unsigned);

    template <typename Type>
    HB_INTERNAL സ്ഥിതം തർക്കം get_glyph_from (സ്ഥിരം ശൂന്യം *obj,
					    hb_codepoint_t codepoint,
					    hb_codepoint_t *glyph)
    {
      സ്ഥിരം Type *typed_obj = (സ്ഥിരം Type *) obj;
      തിരികെ typed_obj->get_glyph (codepoint, glyph);
    }

    template <typename Type, hb_pua_remap_func_t remap>
    HB_INTERNAL സ്ഥിതം തർക്കം get_glyph_from_ചിഹ്നം (സ്ഥിരം ശൂന്യം *obj,
						   hb_codepoint_t codepoint,
						   hb_codepoint_t *glyph)
    {
      സ്ഥിരം Type *typed_obj = (സ്ഥിരം Type *) obj;
      എങ്കിൽ (likely (typed_obj->get_glyph (codepoint, glyph)))
	തിരികെ സത്യം;

      എങ്കിൽ (hb_codepoint_t c = remap (codepoint))
	തിരികെ typed_obj->get_glyph (c, glyph);

      തിരികെ അസത്യം;
    }

    template <typename Type>
    HB_INTERNAL സ്ഥിതം തർക്കം get_glyph_from_ascii (സ്ഥിരം ശൂന്യം *obj,
						  hb_codepoint_t codepoint,
						  hb_codepoint_t *glyph)
    {
      സ്ഥിരം Type *typed_obj = (സ്ഥിരം Type *) obj;
      തിരികെ codepoint < 0x80 && typed_obj->get_glyph (codepoint, glyph);
    }

    template <typename Type>
    HB_INTERNAL സ്ഥിതം തർക്കം get_glyph_from_macroman (സ്ഥിരം ശൂന്യം *obj,
						     hb_codepoint_t codepoint,
						     hb_codepoint_t *glyph)
    {
      എങ്കിൽ (get_glyph_from_ascii<Type> (obj, codepoint, glyph))
	തിരികെ സത്യം;

      സ്ഥിരം Type *typed_obj = (സ്ഥിരം Type *) obj;
      unsigned c = unicode_to_macroman (codepoint);
      തിരികെ c && typed_obj->get_glyph (c, glyph);
    }

    unsigned പൂർണ്ണസംഖ്യ get_subtable_data_size (സ്ഥിരം CmapSubtable *subtable) സ്ഥിരം
    {
      unsigned പൂർണ്ണസംഖ്യ table_length = ഇത്->table.get_length ();
      uintptr_t table_start = (uintptr_t) (സ്ഥിരം ശൂന്യം *) ഇത്->table.get ();
      uintptr_t subtable_addr = (uintptr_t) (സ്ഥിരം ശൂന്യം *) subtable;
      എങ്കിൽ (unlikely (subtable_addr < table_start))
	തിരികെ 0;

      uintptr_t subtable_offset = subtable_addr - table_start;
      എങ്കിൽ (unlikely (subtable_offset >= table_length))
	തിരികെ 0;

      തിരികെ table_length - (unsigned പൂർണ്ണസംഖ്യ) subtable_offset;
    }

    സ്വകാര്യ:
    hb_nonnull_ptr_t<സ്ഥിരം CmapSubtable> subtable;
    hb_nonnull_ptr_t<സ്ഥിരം CmapSubtable> subtable_uvs;

    hb_cmap_get_glyph_func_t get_glyph_funcZ = nullptr;
    സ്ഥിരം ശൂന്യം *get_glyph_data = nullptr;

    CmapSubtableFormat4::accelerator_t format4_accel;

#ifndef HB_NO_OT_FONT_CMAP_CACHE
    cache_t *cache = nullptr;
#endif

    പൊതു:
    hb_blob_ptr_t<cmap> table;
  };

  സംരക്ഷിത:

  സ്ഥിരം CmapSubtable *find_subtable (unsigned പൂർണ്ണസംഖ്യ platform_id,
				     unsigned പൂർണ്ണസംഖ്യ encoding_id) സ്ഥിരം
  {
    EncodingRecord key;
    key.platformID = platform_id;
    key.encodingID = encoding_id;

    സ്ഥിരം EncodingRecord &result = encodingRecord.bsearch (key);
    എങ്കിൽ (!result.subtable)
      തിരികെ nullptr;

    തിരികെ &(ഇത്+result.subtable);
  }

  സ്ഥിരം CmapSubtable *find_variation_selector_subtable () സ്ഥിരം
  {
    സ്ഥിരം CmapSubtable *format14 = nullptr;
    വേണ്ടി (സ്ഥിരം EncodingRecord& record : encodingRecord)
    {
      എങ്കിൽ (record.platformID != 0 || record.encodingID != 5)
	തുടരുക;

      സ്ഥിരം CmapSubtable *subtable = &(ഇത്+record.subtable);
#ifndef HB_NO_BEYOND_64K
      എങ്കിൽ (subtable->u.format.v == 15)
	തിരികെ subtable;
#endif
      എങ്കിൽ (subtable->u.format.v == 14)
	format14 = subtable;
    }
    തിരികെ format14;
  }

  പൊതു:

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്) &&
		  hb_barrier () &&
		  likely (version == 0) &&
		  encodingRecord.sanitize (c, ഇത്));
  }

 സ്വകാര്യ:

  സ്ഥിതം തർക്കം filter_encoding_records_for_subset(സ്ഥിരം cmap* cmap,
                                                 സ്ഥിരം EncodingRecord& _)
  {
    തിരികെ
        (_.platformID == 0 && _.encodingID == 3) ||
        (_.platformID == 0 && _.encodingID == 4) ||
        (_.platformID == 3 && _.encodingID == 1) ||
        (_.platformID == 3 && _.encodingID == 10) ||
        (cmap + _.subtable).is_variation_selector ();
  }

  സംരക്ഷിത:
  HBUINT16	version;	/* Table version number (0). */
  SortedArray16Of<EncodingRecord>
		encodingRecord;	/* Encoding tables. */
  പൊതു:
  DEFINE_SIZE_ARRAY (4, encodingRecord);
};

ഘടന DMAP : cmap
{
  സ്ഥിതം constexpr hb_tag_t tableTag = HB_OT_TAG_DMAP;

  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  { തിരികെ cmap::subset (c, സത്യം); }
};

ഘടന cmap_accelerator_t
{
  cmap_accelerator_t (hb_face_t *face) :
    dmap (face, HB_OT_TAG_DMAP),
    base (face)
  {}

  തർക്കം get_nominal_glyph (hb_codepoint_t  unicode,
			  hb_codepoint_t *glyph) സ്ഥിരം
  {
    തിരികെ dmap.get_nominal_glyph (unicode, glyph) ||
	   base.get_nominal_glyph (unicode, glyph);
  }

  unsigned പൂർണ്ണസംഖ്യ get_nominal_glyphs (unsigned പൂർണ്ണസംഖ്യ എണ്ണം,
				   സ്ഥിരം hb_codepoint_t *first_unicode,
				   unsigned പൂർണ്ണസംഖ്യ unicode_stride,
				   hb_codepoint_t *first_glyph,
				   unsigned പൂർണ്ണസംഖ്യ glyph_stride) സ്ഥിരം
  {
    എങ്കിൽ (likely (!dmap.has_data ()))
      തിരികെ base.get_nominal_glyphs (എണ്ണം,
				      first_unicode, unicode_stride,
				      first_glyph, glyph_stride);

    unsigned പൂർണ്ണസംഖ്യ done;
    വേണ്ടി (done = 0;
	 done < എണ്ണം && get_nominal_glyph (*first_unicode, first_glyph);
	 done++)
    {
      first_unicode = &StructAtOffsetUnaligned<hb_codepoint_t> (first_unicode, unicode_stride);
      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
    }
    തിരികെ done;
  }

  തർക്കം get_variation_glyph (hb_codepoint_t  unicode,
			    hb_codepoint_t  variation_selector,
			    hb_codepoint_t *glyph) സ്ഥിരം
  {
    glyph_variant_t result = dmap.get_glyph_variant (unicode,
						     variation_selector,
						     glyph);
    എങ്കിൽ (result == GLYPH_VARIANT_NOT_FOUND)
      result = base.get_glyph_variant (unicode, variation_selector, glyph);

    തിരഞ്ഞെടുപ്പ് (result)
    {
      അവസ്ഥ GLYPH_VARIANT_NOT_FOUND: തിരികെ അസത്യം;
      അവസ്ഥ GLYPH_VARIANT_FOUND: തിരികെ സത്യം;
      അവസ്ഥ GLYPH_VARIANT_USE_DEFAULT: നിർത്തുക;
    }

    തിരികെ get_nominal_glyph (unicode, glyph);
  }

  ശൂന്യം collect_unicodes (hb_set_t *out, unsigned പൂർണ്ണസംഖ്യ അക്കം_glyphs) സ്ഥിരം
  {
    base.collect_unicodes (out, അക്കം_glyphs);
    dmap.collect_unicodes (out, അക്കം_glyphs);
  }

  ശൂന്യം collect_mapping (hb_set_t *unicodes, hb_map_t *mapping,
			unsigned അക്കം_glyphs = UINT_MAX) സ്ഥിരം
  {
    base.collect_mapping (unicodes, mapping, അക്കം_glyphs);
    dmap.collect_mapping (unicodes, mapping, അക്കം_glyphs);
  }

  ശൂന്യം collect_variation_selectors (hb_set_t *out) സ്ഥിരം
  {
    base.collect_variation_selectors (out);
    dmap.collect_variation_selectors (out);
  }

  ശൂന്യം collect_variation_unicodes (hb_codepoint_t variation_selector,
				   hb_set_t *out) സ്ഥിരം
  {
    base.collect_variation_unicodes (variation_selector, out);
    dmap.collect_variation_unicodes (variation_selector, out);
  }

  ശൂന്യം closure_glyphs (സ്ഥിരം hb_set_t *unicodes,
		       hb_set_t       *glyphset) സ്ഥിരം
  {
    എങ്കിൽ (likely (!dmap.has_data ()))
    {
      base.table->closure_glyphs (unicodes, glyphset);
      തിരികെ;
    }

    hb_set_t variation_selectors;
    collect_variation_selectors (&variation_selectors);
    വേണ്ടി (hb_codepoint_t variation_selector : variation_selectors)
    {
      എങ്കിൽ (!unicodes->has (variation_selector))
	തുടരുക;

      hb_set_t variation_unicodes;
      collect_variation_unicodes (variation_selector, &variation_unicodes);
      വേണ്ടി (hb_codepoint_t unicode : variation_unicodes)
      {
	എങ്കിൽ (!unicodes->has (unicode))
	  തുടരുക;

	hb_codepoint_t glyph;
	എങ്കിൽ (get_variation_glyph (unicode, variation_selector, &glyph))
	  glyphset->add (glyph);
      }
    }
  }

  cmap::accelerator_t dmap;
  cmap::accelerator_t base;
};

} /* namespace OT */


#endif /* HB_OT_CMAP_TABLE_HH */
