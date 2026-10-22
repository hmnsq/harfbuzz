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

#অন্তর্ভাৱ "hb-ot-os2-table.hh"
#অন্তর্ভাৱ "hb-ot-shaper-arabic-pua.hh"
#অন্তর্ভাৱ "hb-open-type.hh"
#অন্তর্ভাৱ "hb-ot-dual.hh"
#অন্তর্ভাৱ "hb-set.hh"
#অন্তর্ভাৱ "hb-cache.hh"

/*
 * cmap -- Character to Glyph Index Mapping
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cmap
 */
#define HB_OT_TAG_cmap HB_TAG('c','m','a','p')
#define HB_OT_TAG_DMAP HB_TAG('D','M','A','P')

নামাকাশ OT {

স্থির অন্তঃস্থ uint8_t unicode_to_macroman (hb_codepoint_t u)
{
  স্থির ধ্রুৱ সংরচনা unicode_to_macroman_t
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
  auto *c = hb_bsearch (u, mapping, ARRAY_LENGTH (mapping), পরিমাণ (mapping[0]),
			_hb_cmp_operator<uint16_t, uint16_t>);
  ঘূৰাই c ? c->macroman : 0;
}

সংরচনা CmapSubtableFormat0
{
  তার্কিক get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) ধ্রুৱ
  {
    hb_codepoint_t gid = codepoint < 256 ? glyphIdArray[codepoint] : 0;
    যদি (unlikely (!gid))
      ঘূৰাই মিছা;
    *glyph = gid;
    ঘূৰাই সঁচা;
  }

  unsigned get_language () ধ্রুৱ
  {
    ঘূৰাই language;
  }

  শূন্য collect_unicodes (hb_set_t *out) ধ্রুৱ
  {
    বাবে (unsigned পূর্ণাঙ্ক i = 0; i < 256; i++)
      যদি (glyphIdArray[i])
	out->add (i);
  }

  শূন্য collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping /* OUT */) ধ্রুৱ
  {
    বাবে (unsigned i = 0; i < 256; i++)
      যদি (glyphIdArray[i])
      {
	hb_codepoint_t glyph = glyphIdArray[i];
	unicodes->add (i);
	mapping->set (i, glyph);
      }
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্));
  }

  রক্ষিত:
  HBUINT16	format;		/* Format number is set to 0. */
  HBUINT16	length;		/* Byte length of this subtable. */
  HBUINT16	language;	/* Ignore. */
  HBUINT8	glyphIdArray[256];/* An array that maps character
				 * code to glyph index values. */
  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (6 + 256);
};

সংরচনা CmapSubtableFormat4
{
  template<typename Iterator,
      typename Writer,
	   hb_requires (hb_is_iterator (Iterator))>
  শূন্য to_ranges (Iterator it, Writer& range_writer)
  {
    hb_codepoint_t start_cp = 0, prev_run_start_cp = 0, run_start_cp = 0, end_cp = 0, last_gid = 0;
    পূর্ণাঙ্ক run_length = 0 , delta = 0, prev_delta = 0;

    গণনা {
      FIRST_SUB_RANGE,
      FOLLOWING_SUB_RANGE,
    } mode;

    যেতিয়ালৈ (it) {
      // Start a new range
      {
        ধ্রুৱ auto& pair = *it;
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

      যেতিয়ালৈ (it) {
        // Process range
        ধ্রুৱ auto& pair = *it;
        hb_codepoint_t next_cp = pair.first;
        hb_codepoint_t next_gid = pair.second;
        যদি (next_cp != end_cp + 1) {
          // Current range is over, stop processing.
          ভঙ্গ;
        }

        যদি (next_gid == last_gid + 1) {
          // The current run continues.
          end_cp = next_cp;
          run_length++;
          last_gid = next_gid;
          it++;
          অনুৱর্ত;
        }

        // A new run is starting, decide if we want to commit the current run.
        পূর্ণাঙ্ক split_cost = (mode == FIRST_SUB_RANGE) ? 8 : 16;
        পূর্ণাঙ্ক run_cost = run_length * 2;
        যদি (run_cost >= split_cost) {
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

    যদি (likely (end_cp != 0xFFFF)) {
      range_writer (0xFFFF, 0xFFFF, 1);
    }
  }

  /*
   * Writes the current range as either one or two ranges depending on what is most efficient.
   */
  template<typename Writer>
  শূন্য commit_current_range (hb_codepoint_t start,
                             hb_codepoint_t prev_run_start,
                             hb_codepoint_t run_start,
                             hb_codepoint_t end,
                             পূর্ণাঙ্ক run_delta,
                             পূর্ণাঙ্ক previous_run_delta,
                             পূর্ণাঙ্ক split_cost,
                             Writer& range_writer) {
    তার্কিক should_split = মিছা;
    যদি (start < run_start && run_start < end) {
      পূর্ণাঙ্ক run_cost = (end - run_start + 1) * 2;
      যদি (run_cost >= split_cost) {
        should_split = সঁচা;
      }
    }

    // A zero delta is also the marker for ranges that need a
    // glyphIdArray; serialize_rangeoffset_glyid() tells the two cases
    // apart by re-checking the mapping.
    যদি (should_split) {
      যদি (start == prev_run_start)
        range_writer (start, run_start - 1, previous_run_delta);
      অন্যথা
        range_writer (start, run_start - 1, 0);
      range_writer (run_start, end, run_delta);
      ঘূৰাই;
    }


    যদি (start == run_start) {
      // Range is only a run
      range_writer (start, end, run_delta);
      ঘূৰাই;
    }

    // Write only a single non-run range.
    range_writer (start, end, 0);
  }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  unsigned serialize_find_segcount (Iterator it) {
    সংরচনা Counter {
      unsigned segcount = 0;

      শূন্য operator() (hb_codepoint_t start,
                       hb_codepoint_t end,
                       পূর্ণাঙ্ক delta) {
        segcount++;
      }
    } counter;

    to_ranges (+it, counter);
    ঘূৰাই counter.segcount;
  }


  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  তার্কিক serialize_start_end_delta_arrays (hb_serialize_context_t *c,
                                         Iterator it,
                                         পূর্ণাঙ্ক segcount)
  {
    সংরচনা Writer {
      hb_serialize_context_t *serializer_;
      HBUINT16* end_code_;
      HBUINT16* start_code_;
      HBINT16* id_delta_;
      পূর্ণাঙ্ক index_;

      Writer(hb_serialize_context_t *serializer)
          : serializer_(serializer),
            end_code_(nullptr),
            start_code_(nullptr),
            id_delta_(nullptr),
            index_ (0) {}
      শূন্য operator() (hb_codepoint_t start,
                       hb_codepoint_t end,
                       পূর্ণাঙ্ক delta) {
        start_code_[index_] = start;
        end_code_[index_] = end;
        id_delta_[index_] = delta;
        index_++;
      }
    } writer(c);

    writer.end_code_ = c->allocate_size<HBUINT16> (HBUINT16::static_size * segcount, মিছা);
    (শূন্য) c->allocate_size<HBUINT16> (2); // padding
    writer.start_code_ = c->allocate_size<HBUINT16> (HBUINT16::static_size * segcount, মিছা);
    writer.id_delta_ = c->allocate_size<HBINT16> (HBINT16::static_size * segcount, মিছা);

    যদি (unlikely (!writer.end_code_ || !writer.start_code_ || !writer.id_delta_)) ঘূৰাই মিছা;

    to_ranges (+it, writer);
    ঘূৰাই সঁচা;
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
    যদি (unlikely (!c->check_success (idRangeOffset))) ঘূৰাই nullptr;
    যদি (unlikely ((ৱর্ণ *)idRangeOffset - (ৱর্ণ *)idDelta != (পূর্ণাঙ্ক) segcount * (পূর্ণাঙ্ক) HBINT16::static_size)) ঘূৰাই nullptr;

    বাবে (unsigned i : hb_range (segcount))
    {
      /* A non-zero delta is always a real run. A zero delta is
       * ambiguous: identity mapping, or the marker for a range that
       * needs a glyphIdArray. Only zero deltas need a check. */
      পূর্ণাঙ্ক delta = idDelta[i];
      যদি (delta)
	অনুৱর্ত;
      তার্কিক use_delta = সঁচা;
      বাবে (hb_codepoint_t cp = startCode[i]; cp <= endCode[i]; cp++)
      {
	hb_codepoint_t gid = cp_to_gid.get (cp);
	যদি (gid == HB_MAP_মান_INVALID)
	  gid = 0;
	যদি (gid != cp)
	{
	  use_delta = মিছা;
	  ভঙ্গ;
	}
      }
      যদি (use_delta)
	অনুৱর্ত;

      idRangeOffset[i] = 2 * (c->start_embed<HBUINT16> () - idRangeOffset - i);
      বাবে (hb_codepoint_t cp = startCode[i]; cp <= endCode[i]; cp++)
      {
        HBUINT16 gid;
        gid = cp_to_gid[cp];
        c->copy<HBUINT16> (gid);
      }
    }

    ঘূৰাই idRangeOffset;
  }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  শূন্য serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    auto format4_iter =
    + it
    | hb_filter ([&] (ধ্রুৱ hb_codepoint_pair_t _)
		 { ঘূৰাই _.first <= 0xFFFF; })
    ;

    যদি (!format4_iter) ঘূৰাই;

    unsigned table_initpos = c->length ();
    যদি (unlikely (!c->extend_min (এতত্))) ঘূৰাই;
    এতত্->format = 4;

    hb_vector_t<hb_codepoint_pair_t> cp_to_gid {
      format4_iter
    };

    //serialize endCode[], startCode[], idDelta[]
    HBUINT16* endCode = c->start_embed<HBUINT16> ();
    unsigned segcount = serialize_find_segcount (cp_to_gid.iter());
    যদি (unlikely (!serialize_start_end_delta_arrays (c, cp_to_gid.iter(), segcount)))
      ঘূৰাই;

    HBUINT16 *startCode = endCode + segcount + 1;
    HBINT16 *idDelta = ((HBINT16*)startCode) + segcount;

    HBUINT16 *idRangeOffset = serialize_rangeoffset_glyid (c,
                                                           cp_to_gid.iter (),
                                                           endCode,
                                                           startCode,
                                                           idDelta,
                                                           segcount);
    যদি (unlikely (!c->check_success (idRangeOffset))) ঘূৰাই;

    এতত্->length = c->length () - table_initpos;
    যদি ((long long) এতত্->length != (long long) c->length () - table_initpos)
    {
      c->err (HB_SERIALIZE_ERROR_INT_OVERFLOW);
      ঘূৰাই;
    }

    এতত্->seg_গুনতি_X2 = segcount * 2;
    এতত্->entrySelector = hb_max (1u, hb_bit_storage (segcount)) - 1;
    এতত্->অনুসন্ধান_Range = 2 * (1u << এতত্->entrySelector);
    এতত্->rangeShift = segcount * 2 > এতত্->অনুসন্ধান_Range
		       ? 2 * segcount - এতত্->অনুসন্ধান_Range
		       : 0;
  }

  unsigned get_language () ধ্রুৱ
  {
    ঘূৰাই language;
  }

  সংরচনা accelerator_t
  {
    accelerator_t () {}
    accelerator_t (ধ্রুৱ CmapSubtableFormat4 *subtable) = delete;

    শূন্য init (ধ্রুৱ CmapSubtableFormat4 *subtable,
	       unsigned পূর্ণাঙ্ক              subtable_data_size)
    {
      seg_গুনতি_2 = subtable->seg_গুনতি_X2 / 2;
      end_গুনতি = subtable->values.arrayZ;
      start_গুনতি = end_গুনতি + seg_গুনতি_2 + 1;
      idDelta = start_গুনতি + seg_গুনতি_2;
      idRangeOffset = idDelta + seg_গুনতি_2;
      glyphIdArray = idRangeOffset + seg_গুনতি_2;

      unsigned পূর্ণাঙ্ক মানসমূহ_offset = 16 + 8 * seg_গুনতি_2;
      glyphIdArrayLength = subtable_data_size > মানসমূহ_offset
			   ? (subtable_data_size - মানসমূহ_offset) / 2
			   : 0;
    }

    তার্কিক get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) ধ্রুৱ
    {
      সংরচনা CustomRange
      {
	পূর্ণাঙ্ক cmp (hb_codepoint_t k,
		 unsigned distance) ধ্রুৱ
	{
	  যদি (k > last) ঘূৰাই +1;
	  যদি (k < (&last)[distance]/*first*/) ঘূৰাই -1;
	  ঘূৰাই 0;
	}
	HBUINT16 last;
      };

      ধ্রুৱ HBUINT16 *found = hb_bsearch (codepoint,
					  এতত্->end_গুনতি,
					  এতত্->seg_গুনতি_2,
					  পরিমাণ (CustomRange),
					  _hb_cmp_method<hb_codepoint_t, CustomRange, unsigned>,
					  এতত্->seg_গুনতি_2 + 1);
      যদি (unlikely (!found))
	ঘূৰাই মিছা;
      unsigned পূর্ণাঙ্ক i = found - end_গুনতি;

      hb_codepoint_t gid;
      unsigned পূর্ণাঙ্ক rangeOffset = এতত্->idRangeOffset[i];
      যদি (rangeOffset == 0)
	gid = codepoint + এতত্->idDelta[i];
      অন্যথা
      {
	/* Somebody has been smoking... */
	unsigned পূর্ণাঙ্ক index = rangeOffset / 2 + (codepoint - এতত্->start_গুনতি[i]) + i - এতত্->seg_গুনতি_2;
	যদি (unlikely (index >= এতত্->glyphIdArrayLength))
	  ঘূৰাই মিছা;
	gid = এতত্->glyphIdArray[index];
	যদি (unlikely (!gid))
	  ঘূৰাই মিছা;
	gid += এতত্->idDelta[i];
      }
      gid &= 0xFFFFu;
      যদি (unlikely (!gid))
	ঘূৰাই মিছা;
      *glyph = gid;
      ঘূৰাই সঁচা;
    }

    HB_INTERNAL স্থির তার্কিক get_glyph_func (ধ্রুৱ শূন্য *obj, hb_codepoint_t codepoint, hb_codepoint_t *glyph)
    { ঘূৰাই ((ধ্রুৱ accelerator_t *) obj)->get_glyph (codepoint, glyph); }

    শূন্য collect_unicodes (hb_set_t *out) ধ্রুৱ
    {
      unsigned পূর্ণাঙ্ক গুনতি = এতত্->seg_গুনতি_2;
      যদি (গুনতি && এতত্->start_গুনতি[গুনতি - 1] == 0xFFFFu)
	গুনতি--; /* Skip sentinel segment. */
      বাবে (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
      {
	hb_codepoint_t start = এতত্->start_গুনতি[i];
	hb_codepoint_t end = এতত্->end_গুনতি[i];
	unsigned পূর্ণাঙ্ক rangeOffset = এতত্->idRangeOffset[i];
        out->add_range(start, end);
	যদি (rangeOffset == 0)
	{
	  বাবে (hb_codepoint_t codepoint = start; codepoint <= end; codepoint++)
	  {
	    hb_codepoint_t gid = (codepoint + এতত্->idDelta[i]) & 0xFFFFu;
	    যদি (unlikely (!gid))
              out->del(codepoint);
	  }
	}
	অন্যথা
	{
	  বাবে (hb_codepoint_t codepoint = start; codepoint <= end; codepoint++)
	  {
	    unsigned পূর্ণাঙ্ক index = rangeOffset / 2 + (codepoint - এতত্->start_গুনতি[i]) + i - এতত্->seg_গুনতি_2;
	    যদি (unlikely (index >= এতত্->glyphIdArrayLength))
            {
              out->del_range (codepoint, end);
	      ভঙ্গ;
            }
	    hb_codepoint_t gid = এতত্->glyphIdArray[index];
	    যদি (unlikely (!gid))
              out->del(codepoint);
	  }
	}
      }
    }

    শূন্য collect_mapping (hb_set_t *unicodes, /* OUT */
			  hb_map_t *mapping /* OUT */) ধ্রুৱ
    {
      // TODO(grieger): optimize similar to collect_unicodes
      // (ie. use add_range())
      unsigned গুনতি = এতত্->seg_গুনতি_2;
      যদি (গুনতি && এতত্->start_গুনতি[গুনতি - 1] == 0xFFFFu)
	গুনতি--; /* Skip sentinel segment. */
      বাবে (unsigned i = 0; i < গুনতি; i++)
      {
	hb_codepoint_t start = এতত্->start_গুনতি[i];
	hb_codepoint_t end = এতত্->end_গুনতি[i];
	unsigned rangeOffset = এতত্->idRangeOffset[i];
	যদি (rangeOffset == 0)
	{
	  বাবে (hb_codepoint_t codepoint = start; codepoint <= end; codepoint++)
	  {
	    hb_codepoint_t gid = (codepoint + এতত্->idDelta[i]) & 0xFFFFu;
	    যদি (unlikely (!gid))
	      অনুৱর্ত;
	    unicodes->add (codepoint);
	    mapping->set (codepoint, gid);
	  }
	}
	অন্যথা
	{
	  বাবে (hb_codepoint_t codepoint = start; codepoint <= end; codepoint++)
	  {
	    unsigned index = rangeOffset / 2 + (codepoint - এতত্->start_গুনতি[i]) + i - এতত্->seg_গুনতি_2;
	    যদি (unlikely (index >= এতত্->glyphIdArrayLength))
	      ভঙ্গ;
	    hb_codepoint_t gid = এতত্->glyphIdArray[index];
	    যদি (unlikely (!gid))
	      অনুৱর্ত;
            gid = (gid + এতত্->idDelta[i]) & 0xFFFFu;
	    unicodes->add (codepoint);
	    mapping->set (codepoint, gid);
	  }
	}
      }
    }

    ধ্রুৱ HBUINT16 *end_গুনতি;
    ধ্রুৱ HBUINT16 *start_গুনতি;
    ধ্রুৱ HBUINT16 *idDelta;
    ধ্রুৱ HBUINT16 *idRangeOffset;
    ধ্রুৱ HBUINT16 *glyphIdArray;
    unsigned পূর্ণাঙ্ক seg_গুনতি_2;
    unsigned পূর্ণাঙ্ক glyphIdArrayLength;
  };

  তার্কিক get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph,
		  unsigned পূর্ণাঙ্ক subtable_data_size) ধ্রুৱ
  {
    accelerator_t accel;
    accel.init (এতত্, subtable_data_size);
    ঘূৰাই accel.get_glyph_func (&accel, codepoint, glyph);
  }
  তার্কিক get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) ধ্রুৱ
  { ঘূৰাই মিছা; }

  শূন্য collect_unicodes (hb_set_t *out, unsigned পূর্ণাঙ্ক subtable_data_size) ধ্রুৱ
  {
    accelerator_t accel;
    accel.init (এতত্, subtable_data_size);
    accel.collect_unicodes (out);
  }
  শূন্য collect_unicodes (hb_set_t *out) ধ্রুৱ
  { collect_unicodes (out, length); }

  শূন্য collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping, /* OUT */
			unsigned পূর্ণাঙ্ক subtable_data_size) ধ্রুৱ
  {
    accelerator_t accel;
    accel.init (এতত্, subtable_data_size);
    accel.collect_mapping (unicodes, mapping);
  }
  শূন্য collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping /* OUT */) ধ্রুৱ
  { collect_mapping (unicodes, mapping, length); }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    যদি (unlikely (!c->check_struct (এতত্)))
      return_trace (মিছা);
    hb_barrier ();

    return_trace (c->check_range (values, 2 + 4 * seg_গুনতি_X2));
  }

  রক্ষিত:
  HBUINT16	format;		/* Format number is set to 4. */
  HBUINT16	length;		/* This is the length in bytes of the
				 * subtable. */
  HBUINT16	language;	/* Ignore. */
  HBUINT16	seg_গুনতি_X2;	/* 2 x segCount. */
  HBUINT16	অনুসন্ধান_Range;	/* 2 * (2**floor(log2(segCount))) */
  HBUINT16	entrySelector;	/* log2(searchRange/2) */
  HBUINT16	rangeShift;	/* 2 x segCount - searchRange */

  UnsizedArrayOf<HBUINT16>
		values;
#যদি 0
  HBUINT16	end_গুনতি[seg_গুনতি_2];	/* End characterCode for each segment,
					 * last=0xFFFFu. */
  HBUINT16	reservedPad;		/* Set to 0. */
  HBUINT16	start_গুনতি[seg_গুনতি_2];	/* Start character code for each segment. */
  HBINT16		idDelta[seg_গুনতি_2];	/* Delta for all character codes in segment. */
  HBUINT16	idRangeOffset[seg_গুনতি_2];/* Offsets into glyphIdArray or 0 */
  UnsizedArrayOf<HBUINT16>
		glyphIdArray;	/* Glyph index array (arbitrary length) */
#endif

  সার্ৱজনিক:
  DEFINE_SIZE_ARRAY (14, values);
};

সংরচনা CmapSubtableLongGroup
{
  friend সংরচনা CmapSubtableFormat12;
  friend সংরচনা CmapSubtableFormat13;
  template<typename U>
  friend সংরচনা CmapSubtableLongSegmented;
  friend সংরচনা cmap;

  পূর্ণাঙ্ক cmp (hb_codepoint_t codepoint) ধ্রুৱ
  {
    যদি (codepoint < startCharCode) ঘূৰাই -1;
    যদি (codepoint > endCharCode)   ঘূৰাই +1;
    ঘূৰাই 0;
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্));
  }

  গুপ্ত:
  HBUINT32		startCharCode;	/* First character code in this group. */
  HBUINT32		endCharCode;	/* Last character code in this group. */
  HBUINT32		glyphID;	/* Glyph index; interpretation depends on
					 * subtable format. */
  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (12);
};
DECLARE_NULL_NAMESPACE_BYTES (OT, CmapSubtableLongGroup);

template <typename UINT>
সংরচনা CmapSubtableTrimmed
{
  তার্কিক get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) ধ্রুৱ
  {
    /* Rely on our implicit array bound-checking. */
    hb_codepoint_t gid = glyphIdArray[codepoint - startCharCode];
    যদি (unlikely (!gid))
      ঘূৰাই মিছা;
    *glyph = gid;
    ঘূৰাই সঁচা;
  }

  unsigned get_language () ধ্রুৱ
  {
    ঘূৰাই language;
  }

  শূন্য collect_unicodes (hb_set_t *out) ধ্রুৱ
  {
    hb_codepoint_t start = startCharCode;
    unsigned পূর্ণাঙ্ক গুনতি = glyphIdArray.len;
    বাবে (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
      যদি (glyphIdArray[i])
	out->add (start + i);
  }

  শূন্য collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping /* OUT */) ধ্রুৱ
  {
    hb_codepoint_t start_cp = startCharCode;
    unsigned গুনতি = glyphIdArray.len;
    বাবে (unsigned i = 0; i < গুনতি; i++)
      যদি (glyphIdArray[i])
      {
	hb_codepoint_t unicode = start_cp + i;
	hb_codepoint_t glyphid = glyphIdArray[i];
	unicodes->add (unicode);
	mapping->set (unicode, glyphid);
      }
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) && glyphIdArray.sanitize (c));
  }

  রক্ষিত:
  UINT		formatReserved;	/* Subtable format and (maybe) padding. */
  UINT		length;		/* Byte length of this subtable. */
  UINT		language;	/* Ignore. */
  UINT		startCharCode;	/* First character code covered. */
  ArrayOf<HBGlyphID16, UINT>
		glyphIdArray;	/* Array of glyph index values for character
				 * codes in the range. */
  সার্ৱজনিক:
  DEFINE_SIZE_ARRAY (5 * পরিমাণ (UINT), glyphIdArray);
};

সংরচনা CmapSubtableFormat6  : CmapSubtableTrimmed<HBUINT16> {};
সংরচনা CmapSubtableFormat10 : CmapSubtableTrimmed<HBUINT32> {};

template <typename T>
সংরচনা CmapSubtableLongSegmented
{
  friend সংরচনা cmap;

  তার্কিক get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) ধ্রুৱ
  {
    hb_codepoint_t gid = T::group_get_glyph (groups.bsearch (codepoint), codepoint);
    যদি (unlikely (!gid))
      ঘূৰাই মিছা;
    *glyph = gid;
    ঘূৰাই সঁচা;
  }

  unsigned get_language () ধ্রুৱ
  {
    ঘূৰাই language;
  }

  শূন্য collect_unicodes (hb_set_t *out, unsigned পূর্ণাঙ্ক অঙ্ক_glyphs) ধ্রুৱ
  {
    বাবে (unsigned পূর্ণাঙ্ক i = 0; i < এতত্->groups.len; i++)
    {
      hb_codepoint_t start = এতত্->groups[i].startCharCode;
      hb_codepoint_t end = hb_min ((hb_codepoint_t) এতত্->groups[i].endCharCode,
				   (hb_codepoint_t) HB_UNICODE_MAX);
      hb_codepoint_t gid = এতত্->groups[i].glyphID;
      যদি (!gid)
      {
	/* Intention is: if (hb_is_same (T, CmapSubtableFormat13)) continue; */
	যদি (! T::group_get_glyph (এতত্->groups[i], end)) অনুৱর্ত;
	start++;
	gid++;
      }
      যদি (unlikely ((unsigned পূর্ণাঙ্ক) gid >= অঙ্ক_glyphs)) অনুৱর্ত;
      যদি (unlikely ((unsigned পূর্ণাঙ্ক) (gid + end - start) >= অঙ্ক_glyphs))
	end = start + (hb_codepoint_t) অঙ্ক_glyphs - gid;

      out->add_range (start, hb_min (end, 0x10FFFFu));
    }
  }

  শূন্য collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping, /* OUT */
			unsigned অঙ্ক_glyphs) ধ্রুৱ
  {
    hb_codepoint_t last_end = 0;
    unsigned গুনতি = এতত্->groups.len;
    বাবে (unsigned i = 0; i < গুনতি; i++)
    {
      hb_codepoint_t start = এতত্->groups.arrayZ[i].startCharCode;
      hb_codepoint_t end = hb_min ((hb_codepoint_t) এতত্->groups.arrayZ[i].endCharCode,
				   (hb_codepoint_t) HB_UNICODE_MAX);
      যদি (unlikely (start > end || start < last_end)) {
        // Range is not in order and is invalid, skip it.
        অনুৱর্ত;
      }
      last_end = end;


      hb_codepoint_t gid = এতত্->groups.arrayZ[i].glyphID;
      যদি (!gid)
      {
        যদি (T::format_সংখ্যা == 13) অনুৱর্ত;
	start++;
	gid++;
      }
      যদি (unlikely ((unsigned পূর্ণাঙ্ক) gid >= অঙ্ক_glyphs)) অনুৱর্ত;
      যদি (unlikely ((unsigned পূর্ণাঙ্ক) (gid + end - start) >= অঙ্ক_glyphs))
	end = start + (hb_codepoint_t) অঙ্ক_glyphs - gid;

      mapping->alloc (mapping->get_population () + end - start + 1);

      unicodes->add_range (start, end);
      বাবে (unsigned cp = start; cp <= end; cp++)
      {
	mapping->set (cp, gid);
        gid += T::increment;
      }
    }
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) && groups.sanitize (c));
  }

  রক্ষিত:
  HBUINT16	format;		/* Subtable format; set to 12. */
  HBUINT16	reserved;	/* Reserved; set to 0. */
  HBUINT32	length;		/* Byte length of this subtable. */
  HBUINT32	language;	/* Ignore. */
  SortedArray32Of<CmapSubtableLongGroup>
		groups;		/* Groupings. */
  সার্ৱজনিক:
  DEFINE_SIZE_ARRAY (16, groups);
};

সংরচনা CmapSubtableFormat12 : CmapSubtableLongSegmented<CmapSubtableFormat12>
{
  স্থির constexpr পূর্ণাঙ্ক increment = 1;
  স্থির constexpr পূর্ণাঙ্ক format_সংখ্যা = 12;

  স্থির hb_codepoint_t group_get_glyph (ধ্রুৱ CmapSubtableLongGroup &group,
					 hb_codepoint_t u)
  { ঘূৰাই likely (group.startCharCode <= group.endCharCode) ?
	   group.glyphID + (u - group.startCharCode) : 0; }


  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  শূন্য serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    যদি (!it) ঘূৰাই;
    unsigned table_initpos = c->length ();
    যদি (unlikely (!c->extend_min (এতত্))) ঘূৰাই;

    hb_codepoint_t startCharCode = (hb_codepoint_t) -1, endCharCode = (hb_codepoint_t) -1;
    hb_codepoint_t glyphID = 0;

    বাবে (ধ্রুৱ auto& _ : +it)
    {
      যদি (startCharCode == (hb_codepoint_t) -1)
      {
	startCharCode = _.first;
	endCharCode = _.first;
	glyphID = _.second;
      }
      অন্যথা যদি (!_is_gid_consecutive (endCharCode, startCharCode, glyphID, _.first, _.second))
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
      অন্যথা
	endCharCode = _.first;
    }

    CmapSubtableLongGroup record;
    record.startCharCode = startCharCode;
    record.endCharCode = endCharCode;
    record.glyphID = glyphID;
    c->copy<CmapSubtableLongGroup> (record);

    এতত্->format = 12;
    এতত্->reserved = 0;
    এতত্->length = c->length () - table_initpos;
    এতত্->groups.len = (এতত্->length - min_size) / CmapSubtableLongGroup::static_size;
  }

  স্থির size_t get_sub_table_size (ধ্রুৱ hb_sorted_vector_t<CmapSubtableLongGroup> &groups_data)
  { ঘূৰাই 16 + 12 * groups_data.length; }

  গুপ্ত:
  স্থির তার্কিক _is_gid_consecutive (hb_codepoint_t endCharCode,
				   hb_codepoint_t startCharCode,
				   hb_codepoint_t glyphID,
				   hb_codepoint_t cp,
				   hb_codepoint_t new_gid)
  {
    ঘূৰাই (cp - 1 == endCharCode) &&
	new_gid == glyphID + (cp - startCharCode);
  }

};

সংরচনা CmapSubtableFormat13 : CmapSubtableLongSegmented<CmapSubtableFormat13>
{
  স্থির constexpr পূর্ণাঙ্ক increment = 0;
  স্থির constexpr পূর্ণাঙ্ক format_সংখ্যা = 13;

  স্থির hb_codepoint_t group_get_glyph (ধ্রুৱ CmapSubtableLongGroup &group,
					 hb_codepoint_t u HB_UNUSED)
  { ঘূৰাই group.glyphID; }
};

প্রকারনির্দেশ গণনা
{
  GLYPH_VARIANT_NOT_FOUND = 0,
  GLYPH_VARIANT_FOUND = 1,
  GLYPH_VARIANT_USE_DEFAULT = 2
} glyph_variant_t;

সংরচনা Unicode_মান_Range
{
  পূর্ণাঙ্ক cmp (ধ্রুৱ hb_codepoint_t &codepoint) ধ্রুৱ
  {
    যদি (codepoint < start_Unicode_মান) ঘূৰাই -1;
    যদি (codepoint > start_Unicode_মান + additional_গুনতি) ঘূৰাই +1;
    ঘূৰাই 0;
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্));
  }

  HBUINT24	start_Unicode_মান;	/* First value in this range. */
  HBUINT8	additional_গুনতি;	/* Number of additional values in this
					 * range. */
  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (4);
};

সংরচনা DefaultUVS : SortedArray32Of<Unicode_মান_Range>
{
  শূন্য collect_unicodes (hb_set_t *out) ধ্রুৱ
  {
    unsigned পূর্ণাঙ্ক গুনতি = len;
    বাবে (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
    {
      hb_codepoint_t first = arrayZ[i].start_Unicode_মান;
      hb_codepoint_t last = hb_min ((hb_codepoint_t) (first + arrayZ[i].additional_গুনতি),
				    (hb_codepoint_t) HB_UNICODE_MAX);
      out->add_range (first, last);
    }
  }

  DefaultUVS* copy (hb_serialize_context_t *c,
		    ধ্রুৱ hb_set_t *unicodes) ধ্রুৱ
  {
    auto *out = c->start_embed<DefaultUVS> ();
    auto snap = c->snapshot ();

    HBUINT32 len;
    len = 0;
    যদি (unlikely (!c->copy<HBUINT32> (len))) ঘূৰাই nullptr;
    unsigned init_len = c->length ();

    যদি (এতত্->len > unicodes->get_population () * hb_bit_storage ((unsigned) এতত্->len))
    {
      hb_codepoint_t start = HB_SET_মান_INVALID;
      hb_codepoint_t end = HB_SET_মান_INVALID;

      বাবে (auto u : *unicodes)
      {
        যদি (!as_array ().bsearch (u))
	  অনুৱর্ত;
	যদি (start == HB_SET_মান_INVALID)
	{
	  start = u;
	  end = start - 1;
	}
	যদি (end + 1 != u || end - start == 255)
        {
	  Unicode_মান_Range rec;
	  rec.start_Unicode_মান = start;
	  rec.additional_গুনতি = end - start;
	  c->copy<Unicode_মান_Range> (rec);
	  start = u;
	}
	end = u;
      }
      যদি (start != HB_SET_মান_INVALID)
      {
	Unicode_মান_Range rec;
	rec.start_Unicode_মান = start;
	rec.additional_গুনতি = end - start;
	c->copy<Unicode_মান_Range> (rec);
      }

    }
    অন্যথা
    {
      hb_codepoint_t lastCode = HB_SET_মান_INVALID;
      পূর্ণাঙ্ক গুনতি = -1;

      বাবে (ধ্রুৱ Unicode_মান_Range& _ : *এতত্)
      {
	hb_codepoint_t curEntry = (hb_codepoint_t) (_.start_Unicode_মান - 1);
	hb_codepoint_t end = curEntry + _.additional_গুনতি + 2;

	বাবে (; unicodes->next (&curEntry) && curEntry < end;)
	{
	  গুনতি += 1;
	  যদি (lastCode == HB_SET_মান_INVALID)
	    lastCode = curEntry;
	  অন্যথা যদি (lastCode + গুনতি != curEntry || গুনতি > 255)
	  {
	    Unicode_মান_Range rec;
	    rec.start_Unicode_মান = lastCode;
	    rec.additional_গুনতি = গুনতি - 1;
	    c->copy<Unicode_মান_Range> (rec);

	    lastCode = curEntry;
	    গুনতি = 0;
	  }
	}
      }

      যদি (lastCode != HB_MAP_মান_INVALID)
      {
	Unicode_মান_Range rec;
	rec.start_Unicode_মান = lastCode;
	rec.additional_গুনতি = গুনতি;
	c->copy<Unicode_মান_Range> (rec);
      }
    }

    যদি (c->length () - init_len == 0)
    {
      c->revert (snap);
      ঘূৰাই nullptr;
    }
    অন্যথা
    {
      যদি (unlikely (!c->check_assign (out->len,
                                      (c->length () - init_len) / Unicode_মান_Range::static_size,
                                      HB_SERIALIZE_ERROR_INT_OVERFLOW))) ঘূৰাই nullptr;
      ঘূৰাই out;
    }
  }

  সার্ৱজনিক:
  DEFINE_SIZE_ARRAY (4, *এতত্);
};

template <typename Types>
সংরচনা UVSMapping
{
  using GlyphID = typename Types::HBGlyphID;

  পূর্ণাঙ্ক cmp (ধ্রুৱ hb_codepoint_t &codepoint) ধ্রুৱ
  { ঘূৰাই unicode_মান.cmp (codepoint); }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্));
  }

  HBUINT24	unicode_মান;	/* Base Unicode value of the UVS */
  GlyphID
		glyphID;	/* Glyph ID of the UVS. */
  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (3 + GlyphID::static_size);
};

template <typename Types>
সংরচনা NonDefaultUVS : SortedArray32Of<UVSMapping<Types>>
{
  using Mapping = UVSMapping<Types>;

  শূন্য collect_unicodes (hb_set_t *out) ধ্রুৱ
  {
    বাবে (ধ্রুৱ auto& a : এতত্->as_array ())
      out->add (a.unicode_মান);
  }

  শূন্য collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping /* OUT */) ধ্রুৱ
  {
    বাবে (ধ্রুৱ auto& a : এতত্->as_array ())
    {
      hb_codepoint_t unicode = a.unicode_মান;
      hb_codepoint_t glyphid = a.glyphID;
      unicodes->add (unicode);
      mapping->set (unicode, glyphid);
    }
  }

  শূন্য closure_glyphs (ধ্রুৱ hb_set_t      *unicodes,
		       hb_set_t            *glyphset) ধ্রুৱ
  {
    + এতত্->as_array ()
    | hb_filter (unicodes, &Mapping::unicode_মান)
    | hb_map (&Mapping::glyphID)
    | hb_sink (glyphset)
    ;
  }

  NonDefaultUVS* copy (hb_serialize_context_t *c,
		       ধ্রুৱ hb_set_t *unicodes,
		       ধ্রুৱ hb_set_t *glyphs_requested,
		       ধ্রুৱ hb_map_t *glyph_map) ধ্রুৱ
  {
    auto *out = c->start_embed<NonDefaultUVS> ();
    auto it =
    + এতত্->as_array ()
    | hb_filter ([&] (ধ্রুৱ Mapping& _)
		 {
		   ঘূৰাই unicodes->has (_.unicode_মান) || glyphs_requested->has (_.glyphID);
		 })
    ;

    যদি (!it) ঘূৰাই nullptr;

    HBUINT32 len;
    len = it.len ();
    যদি (unlikely (!c->copy<HBUINT32> (len))) ঘূৰাই nullptr;

    বাবে (ধ্রুৱ Mapping& _ : it)
    {
      Mapping mapping;
      mapping.unicode_মান = _.unicode_মান;
      mapping.glyphID = glyph_map->get (_.glyphID);
      c->copy<Mapping> (mapping);
    }

    ঘূৰাই out;
  }

  সার্ৱজনিক:
  DEFINE_SIZE_ARRAY (4, *এতত্);
};

template <typename Types>
সংরচনা VariationSelectorRecord
{
  glyph_variant_t get_glyph (hb_codepoint_t codepoint,
			     hb_codepoint_t *glyph,
			     ধ্রুৱ শূন্য *base) ধ্রুৱ
  {
    যদি ((base+defaultUVS).bfind (codepoint))
      ঘূৰাই GLYPH_VARIANT_USE_DEFAULT;
    ধ্রুৱ UVSMapping<Types> &nonDefault = (base+nonDefaultUVS).bsearch (codepoint);
    যদি (nonDefault.glyphID)
    {
      *glyph = nonDefault.glyphID;
       ঘূৰাই GLYPH_VARIANT_FOUND;
    }
    ঘূৰাই GLYPH_VARIANT_NOT_FOUND;
  }

  VariationSelectorRecord(ধ্রুৱ VariationSelectorRecord& other)
  {
    *এতত্ = other;
  }

  শূন্য operator= (ধ্রুৱ VariationSelectorRecord& other)
  {
    varSelector = other.varSelector;
    HBUINT32 offset = other.defaultUVS;
    defaultUVS = offset;
    offset = other.nonDefaultUVS;
    nonDefaultUVS = offset;
  }

  শূন্য collect_unicodes (hb_set_t *out, ধ্রুৱ শূন্য *base) ধ্রুৱ
  {
    (base+defaultUVS).collect_unicodes (out);
    (base+nonDefaultUVS).collect_unicodes (out);
  }

  শূন্য collect_mapping (ধ্রুৱ শূন্য *base,
			hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping /* OUT */) ধ্রুৱ
  {
    (base+defaultUVS).collect_unicodes (unicodes);
    (base+nonDefaultUVS).collect_mapping (unicodes, mapping);
  }

  পূর্ণাঙ্ক cmp (ধ্রুৱ hb_codepoint_t &variation_selector) ধ্রুৱ
  { ঘূৰাই varSelector.cmp (variation_selector); }

  তার্কিক sanitize (hb_sanitize_context_t *c, ধ্রুৱ শূন্য *base) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) &&
		  defaultUVS.sanitize (c, base) &&
		  nonDefaultUVS.sanitize (c, base));
  }

  hb_pair_t<unsigned, unsigned>
  copy (hb_serialize_context_t *c,
	ধ্রুৱ hb_set_t *unicodes,
	ধ্রুৱ hb_set_t *glyphs_requested,
	ধ্রুৱ hb_map_t *glyph_map,
	ধ্রুৱ শূন্য *base) ধ্রুৱ
  {
    auto snap = c->snapshot ();
    auto *out = c->embed<VariationSelectorRecord> (*এতত্);
    যদি (unlikely (!out)) ঘূৰাই hb_pair (0, 0);

    out->defaultUVS = 0;
    out->nonDefaultUVS = 0;

    unsigned non_default_uvs_objidx = 0;
    যদি (nonDefaultUVS != 0)
    {
      c->push ();
      যদি (c->copy (base+nonDefaultUVS, unicodes, glyphs_requested, glyph_map))
	non_default_uvs_objidx = c->বেৰ_কৰো_pack ();
      অন্যথা c->বেৰ_কৰো_discard ();
    }

    unsigned default_uvs_objidx = 0;
    যদি (defaultUVS != 0)
    {
      c->push ();
      যদি (c->copy (base+defaultUVS, unicodes))
	default_uvs_objidx = c->বেৰ_কৰো_pack ();
      অন্যথা c->বেৰ_কৰো_discard ();
    }


    যদি (!default_uvs_objidx && !non_default_uvs_objidx)
      c->revert (snap);

    ঘূৰাই hb_pair (default_uvs_objidx, non_default_uvs_objidx);
  }

  HBUINT24	varSelector;	/* Variation selector. */
  Offset32To<DefaultUVS>
		defaultUVS;	/* Offset to Default UVS Table.  May be 0. */
  Offset32To<NonDefaultUVS<Types>>
		nonDefaultUVS;	/* Offset to Non-Default UVS Table.  May be 0. */
  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (11);
};

template <typename Types>
সংরচনা CmapSubtableFormat14_15
{
  স্থির constexpr unsigned format_মান = Types::size == 2 ? 14 : 15;
  using Record = VariationSelectorRecord<Types>;
  using NonDefault = NonDefaultUVS<Types>;

  glyph_variant_t get_glyph_variant (hb_codepoint_t codepoint,
				     hb_codepoint_t variation_selector,
				     hb_codepoint_t *glyph) ধ্রুৱ
  { ঘূৰাই record.bsearch (variation_selector).get_glyph (codepoint, glyph, এতত্); }

  শূন্য collect_variation_selectors (hb_set_t *out) ধ্রুৱ
  {
    বাবে (ধ্রুৱ auto& a : record.as_array ())
      out->add (a.varSelector);
  }
  শূন্য collect_variation_unicodes (hb_codepoint_t variation_selector,
				   hb_set_t *out) ধ্রুৱ
  { record.bsearch (variation_selector).collect_unicodes (out, এতত্); }

  শূন্য serialize (hb_serialize_context_t *c,
		  ধ্রুৱ hb_set_t *unicodes,
		  ধ্রুৱ hb_set_t *glyphs_requested,
		  ধ্রুৱ hb_map_t *glyph_map,
		  ধ্রুৱ শূন্য *base)
  {
    auto snap = c->snapshot ();
    unsigned table_initpos = c->length ();
    ধ্রুৱ ৱর্ণ* init_tail = c->tail;

    যদি (unlikely (!c->extend_min (এতত্))) ঘূৰাই;
    এতত্->format = format_মান;

    auto src_tbl = reinterpret_cast<ধ্রুৱ CmapSubtableFormat14_15*> (base);

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
    বাবে (পূর্ণাঙ্ক i = src_tbl->record.len - 1; i >= 0; i--)
    {
      যদি (!unicodes->has(src_tbl->record[i].varSelector))
        অনুৱর্ত;

      hb_pair_t<unsigned, unsigned> result = src_tbl->record[i].copy (c, unicodes, glyphs_requested, glyph_map, base);
      যদি (result.first || result.second)
	obj_indices.push (result);
    }

    যদি (c->length () - table_initpos == CmapSubtableFormat14_15::min_size)
    {
      c->revert (snap);
      ঘূৰাই;
    }

    যদি (unlikely (!c->check_success (!obj_indices.in_error ())))
      ঘূৰাই;

    পূর্ণাঙ্ক tail_len = init_tail - c->tail;
    c->check_assign (এতত্->length, c->length () - table_initpos + tail_len,
                     HB_SERIALIZE_ERROR_INT_OVERFLOW);
    c->check_assign (এতত্->record.len,
		     (c->length () - table_initpos - CmapSubtableFormat14_15::min_size) /
		     Record::static_size,
                     HB_SERIALIZE_ERROR_INT_OVERFLOW);

    /* Correct the incorrect write order by reversing the order of the variation
       records array. */
    _reverse_variation_records ();

    /* Now that records are in the right order, we can set up the offsets. */
    _add_links_to_variation_records (c, obj_indices);
  }

  শূন্য _reverse_variation_records ()
  {
    record.as_array ().reverse ();
  }

  শূন্য _add_links_to_variation_records (hb_serialize_context_t *c,
					ধ্রুৱ hb_vector_t<hb_pair_t<unsigned, unsigned>>& obj_indices)
  {
    বাবে (unsigned i = 0; i < obj_indices.length; i++)
    {
      /*
       * Since the record array has been reversed (see comments in copy())
       * but obj_indices has not been, the indices at obj_indices[i]
       * are for the variation record at record[j].
       */
      পূর্ণাঙ্ক j = obj_indices.length - 1 - i;
      c->add_link (record[j].defaultUVS, obj_indices[i].first);
      c->add_link (record[j].nonDefaultUVS, obj_indices[i].second);
    }
  }

  শূন্য closure_glyphs (ধ্রুৱ hb_set_t      *unicodes,
		       hb_set_t            *glyphset) ধ্রুৱ
  {
    + hb_iter (record)
    | hb_filter (hb_bool, &Record::nonDefaultUVS)
    | hb_filter (unicodes, &Record::varSelector)
    | hb_map (&Record::nonDefaultUVS)
    | hb_map (hb_add (এতত্))
    | hb_apply ([=] (ধ্রুৱ NonDefault& _) { _.closure_glyphs (unicodes, glyphset); })
    ;
  }

  শূন্য collect_unicodes (hb_set_t *out) ধ্রুৱ
  {
    বাবে (ধ্রুৱ Record& _ : record)
      _.collect_unicodes (out, এতত্);
  }

  শূন্য collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping /* OUT */) ধ্রুৱ
  {
    বাবে (ধ্রুৱ Record& _ : record)
      _.collect_mapping (এতত্, unicodes, mapping);
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) &&
		  record.sanitize (c, এতত্));
  }

  রক্ষিত:
  HBUINT16	format;		/* Format number is set to 14 or 15. */
  HBUINT32	length;		/* Byte length of this subtable. */
  SortedArray32Of<Record>
		record;		/* Variation selector records; sorted
				 * in increasing order of `varSelector'. */
  সার্ৱজনিক:
  DEFINE_SIZE_ARRAY (10, record);
};

সংরচনা CmapSubtable
{
  /* Note: We intentionally do NOT implement subtable formats 2 and 8. */

  তার্কিক is_variation_selector () ধ্রুৱ
  {
    ৱিকল্প (u.format.v) {
    পক্ষ 14:
#ifndef HB_NO_BEYOND_64K
    পক্ষ 15:
#endif
      ঘূৰাই সঁচা;
    স্ৱভাৱ:
      ঘূৰাই মিছা;
    }
  }

  glyph_variant_t get_glyph_variant (hb_codepoint_t codepoint,
				     hb_codepoint_t variation_selector,
				     hb_codepoint_t *glyph) ধ্রুৱ
  {
    ৱিকল্প (u.format.v) {
    পক্ষ 14: hb_barrier (); ঘূৰাই u.format14.get_glyph_variant (codepoint, variation_selector, glyph);
#ifndef HB_NO_BEYOND_64K
    পক্ষ 15: hb_barrier (); ঘূৰাই u.format15.get_glyph_variant (codepoint, variation_selector, glyph);
#endif
    স্ৱভাৱ: ঘূৰাই GLYPH_VARIANT_NOT_FOUND;
    }
  }

  শূন্য collect_variation_selectors (hb_set_t *out) ধ্রুৱ
  {
    ৱিকল্প (u.format.v) {
    পক্ষ 14: hb_barrier (); u.format14.collect_variation_selectors (out); ঘূৰাই;
#ifndef HB_NO_BEYOND_64K
    পক্ষ 15: hb_barrier (); u.format15.collect_variation_selectors (out); ঘূৰাই;
#endif
    স্ৱভাৱ: ঘূৰাই;
    }
  }

  শূন্য collect_variation_unicodes (hb_codepoint_t variation_selector,
				    hb_set_t *out) ধ্রুৱ
  {
    ৱিকল্প (u.format.v) {
    পক্ষ 14: hb_barrier (); u.format14.collect_variation_unicodes (variation_selector, out); ঘূৰাই;
#ifndef HB_NO_BEYOND_64K
    পক্ষ 15: hb_barrier (); u.format15.collect_variation_unicodes (variation_selector, out); ঘূৰাই;
#endif
    স্ৱভাৱ: ঘূৰাই;
    }
  }

  শূন্য closure_glyphs (ধ্রুৱ hb_set_t *unicodes, hb_set_t *glyphset) ধ্রুৱ
  {
    ৱিকল্প (u.format.v) {
    পক্ষ 14: hb_barrier (); u.format14.closure_glyphs (unicodes, glyphset); ঘূৰাই;
#ifndef HB_NO_BEYOND_64K
    পক্ষ 15: hb_barrier (); u.format15.closure_glyphs (unicodes, glyphset); ঘূৰাই;
#endif
    স্ৱভাৱ: ঘূৰাই;
    }
  }

  তার্কিক get_glyph (hb_codepoint_t codepoint,
		  hb_codepoint_t *glyph,
		  unsigned পূর্ণাঙ্ক subtable_data_size = 0) ধ্রুৱ
  {
    ৱিকল্প (u.format.v) {
    পক্ষ  0: hb_barrier (); ঘূৰাই u.format0 .get_glyph (codepoint, glyph);
    পক্ষ  4: hb_barrier (); ঘূৰাই subtable_data_size
				      ? u.format4.get_glyph (codepoint, glyph, subtable_data_size)
				      : মিছা;
    পক্ষ  6: hb_barrier (); ঘূৰাই u.format6 .get_glyph (codepoint, glyph);
    পক্ষ 10: hb_barrier (); ঘূৰাই u.format10.get_glyph (codepoint, glyph);
    পক্ষ 12: hb_barrier (); ঘূৰাই u.format12.get_glyph (codepoint, glyph);
    পক্ষ 13: hb_barrier (); ঘূৰাই u.format13.get_glyph (codepoint, glyph);
    পক্ষ 14:
    স্ৱভাৱ: ঘূৰাই মিছা;
    }
  }
  শূন্য collect_unicodes (hb_set_t *out,
			 unsigned পূর্ণাঙ্ক অঙ্ক_glyphs = UINT_MAX,
			 unsigned পূর্ণাঙ্ক subtable_data_size = 0) ধ্রুৱ
  {
    ৱিকল্প (u.format.v) {
    পক্ষ  0: hb_barrier (); u.format0 .collect_unicodes (out); ঘূৰাই;
    পক্ষ  4: hb_barrier (); যদি (subtable_data_size)
			      u.format4.collect_unicodes (out, subtable_data_size);
			    অন্যথা
			      u.format4.collect_unicodes (out);
			    ঘূৰাই;
    পক্ষ  6: hb_barrier (); u.format6 .collect_unicodes (out); ঘূৰাই;
    পক্ষ 10: hb_barrier (); u.format10.collect_unicodes (out); ঘূৰাই;
    পক্ষ 12: hb_barrier (); u.format12.collect_unicodes (out, অঙ্ক_glyphs); ঘূৰাই;
    পক্ষ 13: hb_barrier (); u.format13.collect_unicodes (out, অঙ্ক_glyphs); ঘূৰাই;
    পক্ষ 14:
    স্ৱভাৱ: ঘূৰাই;
    }
  }

  শূন্য collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping, /* OUT */
			unsigned অঙ্ক_glyphs = UINT_MAX,
			unsigned পূর্ণাঙ্ক subtable_data_size = 0) ধ্রুৱ
  {
    ৱিকল্প (u.format.v) {
    পক্ষ  0: hb_barrier (); u.format0 .collect_mapping (unicodes, mapping); ঘূৰাই;
    পক্ষ  4: hb_barrier (); যদি (subtable_data_size)
			      u.format4.collect_mapping (unicodes, mapping, subtable_data_size);
			    অন্যথা
			      u.format4.collect_mapping (unicodes, mapping);
			    ঘূৰাই;
    পক্ষ  6: hb_barrier (); u.format6 .collect_mapping (unicodes, mapping); ঘূৰাই;
    পক্ষ 10: hb_barrier (); u.format10.collect_mapping (unicodes, mapping); ঘূৰাই;
    পক্ষ 12: hb_barrier (); u.format12.collect_mapping (unicodes, mapping, অঙ্ক_glyphs); ঘূৰাই;
    পক্ষ 13: hb_barrier (); u.format13.collect_mapping (unicodes, mapping, অঙ্ক_glyphs); ঘূৰাই;
    পক্ষ 14:
    স্ৱভাৱ: ঘূৰাই;
    }
  }

  unsigned get_language () ধ্রুৱ
  {
    ৱিকল্প (u.format.v) {
    পক্ষ  0: hb_barrier (); ঘূৰাই u.format0 .get_language ();
    পক্ষ  4: hb_barrier (); ঘূৰাই u.format4 .get_language ();
    পক্ষ  6: hb_barrier (); ঘূৰাই u.format6 .get_language ();
    পক্ষ 10: hb_barrier (); ঘূৰাই u.format10.get_language ();
    পক্ষ 12: hb_barrier (); ঘূৰাই u.format12.get_language ();
    পক্ষ 13: hb_barrier (); ঘূৰাই u.format13.get_language ();
    পক্ষ 14:
    স্ৱভাৱ: ঘূৰাই 0;
    }
  }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  শূন্য serialize (hb_serialize_context_t *c,
		  Iterator it,
		  unsigned format,
		  ধ্রুৱ hb_subset_plan_t *plan,
		  ধ্রুৱ শূন্য *base)
  {
    ৱিকল্প (format) {
    পক্ষ  4: hb_barrier (); ঘূৰাই u.format4.serialize (c, it);
    পক্ষ 12: hb_barrier (); ঘূৰাই u.format12.serialize (c, it);
    পক্ষ 14: hb_barrier (); ঘূৰাই u.format14.serialize (c, &plan->unicodes, &plan->glyphs_requested, plan->glyph_map, base);
#ifndef HB_NO_BEYOND_64K
    পক্ষ 15: hb_barrier (); ঘূৰাই u.format15.serialize (c, &plan->unicodes, &plan->glyphs_requested, plan->glyph_map, base);
#endif
    স্ৱভাৱ: ঘূৰাই;
    }
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    যদি (!u.format.v.sanitize (c)) return_trace (মিছা);
    hb_barrier ();
    ৱিকল্প (u.format.v) {
    পক্ষ  0: hb_barrier (); return_trace (u.format0 .sanitize (c));
    পক্ষ  4: hb_barrier (); return_trace (u.format4 .sanitize (c));
    পক্ষ  6: hb_barrier (); return_trace (u.format6 .sanitize (c));
    পক্ষ 10: hb_barrier (); return_trace (u.format10.sanitize (c));
    পক্ষ 12: hb_barrier (); return_trace (u.format12.sanitize (c));
    পক্ষ 13: hb_barrier (); return_trace (u.format13.sanitize (c));
    পক্ষ 14: hb_barrier (); return_trace (u.format14.sanitize (c));
#ifndef HB_NO_BEYOND_64K
    পক্ষ 15: hb_barrier (); return_trace (u.format15.sanitize (c));
#endif
    স্ৱভাৱ:return_trace (সঁচা);
    }
  }

  সার্ৱজনিক:
  সঙ্ঘ {
  সংরচনা { HBUINT16 v; }	format;		/* Format identifier */
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
  সার্ৱজনিক:
  DEFINE_SIZE_UNION (2, format.v);
};


সংরচনা EncodingRecord
{
  পূর্ণাঙ্ক cmp (ধ্রুৱ EncodingRecord &other) ধ্রুৱ
  {
    পূর্ণাঙ্ক ret;
    ret = platformID.cmp (other.platformID);
    যদি (ret) ঘূৰাই ret;
    যদি (other.encodingID != 0xFFFF)
    {
      ret = encodingID.cmp (other.encodingID);
      যদি (ret) ঘূৰাই ret;
    }
    ঘূৰাই 0;
  }

  তার্কিক sanitize (hb_sanitize_context_t *c, ধ্রুৱ শূন্য *base) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) &&
		  subtable.sanitize (c, base));
  }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  EncodingRecord* copy (hb_serialize_context_t *c,
			Iterator it,
			unsigned format,
			ধ্রুৱ শূন্য *base,
			ধ্রুৱ hb_subset_plan_t *plan,
			/* INOUT */ unsigned *objidx) ধ্রুৱ
  {
    TRACE_SERIALIZE (এতত্);
    auto snap = c->snapshot ();
    auto *out = c->embed (এতত্);
    যদি (unlikely (!out)) return_trace (nullptr);
    out->subtable = 0;

    যদি (*objidx == 0)
    {
      CmapSubtable *cmapsubtable = c->push<CmapSubtable> ();
      unsigned origin_length = c->length ();
      cmapsubtable->serialize (c, it, format, plan, &(base+subtable));
      যদি (c->length () > origin_length && !c->in_error()) *objidx = c->বেৰ_কৰো_pack ();
      অন্যথা c->বেৰ_কৰো_discard ();
    }

    যদি (*objidx == 0)
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
  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (8);
};

সংরচনা cmap;

সংরচনা SubtableUnicodesCache {

 গুপ্ত:
  hb_blob_ptr_t<cmap> base_blob;
  ধ্রুৱ ৱর্ণ* base;
  unsigned পূর্ণাঙ্ক table_length;
  hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> cached_unicodes;

 সার্ৱজনিক:

  স্থির SubtableUnicodesCache* create (hb_blob_ptr_t<cmap> source_table)
  {
    SubtableUnicodesCache* cache =
        (SubtableUnicodesCache*) hb_malloc (পরিমাণ(SubtableUnicodesCache));

    যদি (unlikely (!cache))
      ঘূৰাই nullptr;

    নূতন (cache) SubtableUnicodesCache (source_table);
    ঘূৰাই cache;
  }

  স্থির শূন্য destroy (শূন্য* value) {
    যদি (!value) ঘূৰাই;

    SubtableUnicodesCache* cache = (SubtableUnicodesCache*) value;
    cache->~SubtableUnicodesCache ();
    hb_free (cache);
  }

  SubtableUnicodesCache(ধ্রুৱ শূন্য* cmap_base,
			unsigned পূর্ণাঙ্ক table_length_ = 0)
      : base_blob(),
        base ((ধ্রুৱ ৱর্ণ*) cmap_base),
        table_length (table_length_),
        cached_unicodes ()
  {}

  SubtableUnicodesCache(hb_blob_ptr_t<cmap> base_blob_)
      : base_blob(base_blob_),
        base ((ধ্রুৱ ৱর্ণ *) base_blob.get()),
        table_length (base_blob.get_length ()),
        cached_unicodes ()
  {}

  ~SubtableUnicodesCache()
  {
    base_blob.destroy ();
  }

  তার্কিক same_base(ধ্রুৱ শূন্য* other) ধ্রুৱ
  {
    ঘূৰাই other == (ধ্রুৱ শূন্য*) base;
  }

  ধ্রুৱ hb_set_t* set_for (ধ্রুৱ EncodingRecord* record,
                           SubtableUnicodesCache& mutable_cache) ধ্রুৱ
  {
    যদি (cached_unicodes.has ((unsigned) ((ধ্রুৱ ৱর্ণ *) record - base)))
      ঘূৰাই cached_unicodes.get ((unsigned) ((ধ্রুৱ ৱর্ণ *) record - base));

    ঘূৰাই mutable_cache.set_for (record);
  }

  ধ্রুৱ hb_set_t* set_for (ধ্রুৱ EncodingRecord* record)
  {
    যদি (!cached_unicodes.has ((unsigned) ((ধ্রুৱ ৱর্ণ *) record - base)))
    {
      hb_set_t *s = hb_set_create ();
      যদি (unlikely (s->in_error ()))
	ঘূৰাই hb_set_get_empty ();

      unsigned পূর্ণাঙ্ক subtable_data_size = record->subtable < table_length
					? table_length - (unsigned পূর্ণাঙ্ক) record->subtable
					: 0;
      (base+record->subtable).collect_unicodes (s, UINT_MAX, subtable_data_size);

      যদি (unlikely (!cached_unicodes.set ((unsigned) ((ধ্রুৱ ৱর্ণ *) record - base), hb::unique_ptr<hb_set_t> {s})))
        ঘূৰাই hb_set_get_empty ();

      ঘূৰাই s;
    }
    ঘূৰাই cached_unicodes.get ((unsigned) ((ধ্রুৱ ৱর্ণ *) record - base));
  }

};

স্থির অন্তঃস্থ uint16_t
hb_প্ৰতীক_pua_map (unsigned codepoint)
{
  যদি (codepoint <= 0x00FFu)
  {
    /* For symbol-encoded OpenType fonts, we duplicate the
     * U+F000..F0FF range at U+0000..U+00FF.  That's what
     * Windows seems to do, and that's hinted about at:
     * https://docs.microsoft.com/en-us/typography/opentype/spec/recom
     * under "Non-Standard (Symbol) Fonts". */
    ঘূৰাই 0xF000u + codepoint;
  }
  ঘূৰাই 0;
}

সংরচনা cmap
{
  স্থির constexpr hb_tag_t tableTag = HB_OT_TAG_cmap;


  স্থির SubtableUnicodesCache* create_filled_cache(hb_blob_ptr_t<cmap> source_table) {
    ধ্রুৱ cmap* cmap = source_table.get();
    auto it =
    + hb_iter (cmap->encodingRecord)
    | hb_filter ([&](ধ্রুৱ EncodingRecord& _) {
      ঘূৰাই cmap::filter_encoding_records_for_subset (cmap, _);
    })
    ;

    SubtableUnicodesCache* cache = SubtableUnicodesCache::create(source_table);

    যদি (unlikely (!cache))
      ঘূৰাই nullptr;

    বাবে (ধ্রুৱ EncodingRecord& _ : it)
      cache->set_for(&_); // populate the cache for this encoding record.

    ঘূৰাই cache;
  }

  template<typename Iterator, typename EncodingRecIter,
	   hb_requires (hb_is_iterator (EncodingRecIter))>
  তার্কিক serialize (hb_serialize_context_t *c,
		  Iterator it,
		  EncodingRecIter encodingrec_iter,
		  ধ্রুৱ শূন্য *base,
		  hb_subset_plan_t *plan,
	                  তার্কিক drop_format_4 = মিছা,
			  unsigned পূর্ণাঙ্ক source_table_length = 0)
  {
    যদি (unlikely (!c->extend_min ((*এতত্))))  ঘূৰাই মিছা;
    এতত্->version = 0;

    unsigned format4objidx = 0, format12objidx = 0, format14objidx = 0;
#ifndef HB_NO_BEYOND_64K
    unsigned format15objidx = 0;
#endif
    auto snap = c->snapshot ();

    SubtableUnicodesCache local_unicodes_cache (base, source_table_length);
    ধ্রুৱ SubtableUnicodesCache* unicodes_cache = &local_unicodes_cache;

    যদি (plan->accelerator &&
        plan->accelerator->cmap_cache &&
        plan->accelerator->cmap_cache->same_base (base))
      unicodes_cache = plan->accelerator->cmap_cache;

    বাবে (ধ্রুৱ EncodingRecord& _ : encodingrec_iter)
    {
      যদি (c->in_error ())
        ঘূৰাই মিছা;

      unsigned format = (base+_.subtable).u.format.v;
      যদি (format != 4 && format != 12 && format != 14
#ifndef HB_NO_BEYOND_64K
	  && format != 15
#endif
	 ) অনুৱর্ত;

      ধ্রুৱ hb_set_t* unicodes_set = unicodes_cache->set_for (&_, local_unicodes_cache);

      যদি (!drop_format_4 && format == 4)
      {
        c->copy (_, + it | hb_filter (*unicodes_set, hb_first), 4u, base, plan, &format4objidx);
        যদি (c->in_error () && c->only_overflow ())
        {
          // cmap4 overflowed, reset and retry serialization without format 4 subtables.
          c->revert (snap);
          ঘূৰাই serialize (c, it,
                            encodingrec_iter,
                            base,
                            plan,
                            সঁচা,
			    source_table_length);
        }
      }

      অন্যথা যদি (format == 12)
      {
        যদি (_can_drop (_,
                       *unicodes_set,
                       base,
                       *unicodes_cache,
                       local_unicodes_cache,
                       + it | hb_map (hb_first), encodingrec_iter))
          অনুৱর্ত;
        c->copy (_, + it | hb_filter (*unicodes_set, hb_first), 12u, base, plan, &format12objidx);
      }
      অন্যথা যদি (format == 14) c->copy (_, it, 14u, base, plan, &format14objidx);
#ifndef HB_NO_BEYOND_64K
      অন্যথা যদি (format == 15) c->copy (_, it, 15u, base, plan, &format15objidx);
#endif
    }
        unsigned length = c->length ();
        unsigned available = length > cmap::min_size ? length - cmap::min_size : 0;
        c->check_assign(এতত্->encodingRecord.len,
                        available / EncodingRecord::static_size,
                        HB_SERIALIZE_ERROR_INT_OVERFLOW);

    // Fail if format 4 was dropped and there is no cmap12.
    ঘূৰাই !drop_format_4 || format12objidx;
  }

  template<typename Iterator, typename EncodingRecordIterator,
      hb_requires (hb_is_iterator (Iterator)),
      hb_requires (hb_is_iterator (EncodingRecordIterator))>
  তার্কিক _can_drop (ধ্রুৱ EncodingRecord& cmap12,
                  ধ্রুৱ hb_set_t& cmap12_unicodes,
                  ধ্রুৱ শূন্য* base,
                  ধ্রুৱ SubtableUnicodesCache& unicodes_cache,
                  SubtableUnicodesCache& local_unicodes_cache,
                  Iterator subset_unicodes,
                  EncodingRecordIterator encoding_records)
  {
    বাবে (auto cp : + subset_unicodes | hb_filter (cmap12_unicodes))
    {
      যদি (cp >= 0x10000) ঘূৰাই মিছা;
    }

    unsigned target_platform;
    unsigned target_encoding;
    unsigned target_language = (base+cmap12.subtable).get_language ();

    যদি (cmap12.platformID == 0 && cmap12.encodingID == 4)
    {
      target_platform = 0;
      target_encoding = 3;
    } অন্যথা যদি (cmap12.platformID == 3 && cmap12.encodingID == 10) {
      target_platform = 3;
      target_encoding = 1;
    } অন্যথা {
      ঘূৰাই মিছা;
    }

    বাবে (ধ্রুৱ auto& _ : encoding_records)
    {
      যদি (_.platformID != target_platform
          || _.encodingID != target_encoding
          || (base+_.subtable).get_language() != target_language)
        অনুৱর্ত;

      ধ্রুৱ hb_set_t* sibling_unicodes = unicodes_cache.set_for (&_, local_unicodes_cache);

      auto cmap12 = + subset_unicodes | hb_filter (cmap12_unicodes);
      auto sibling = + subset_unicodes | hb_filter (*sibling_unicodes);
      বাবে (; cmap12 && sibling; cmap12++, sibling++)
      {
        unsigned a = *cmap12;
        unsigned b = *sibling;
        যদি (a != b) ঘূৰাই মিছা;
      }

      ঘূৰাই !cmap12 && !sibling;
    }

    ঘূৰাই মিছা;
  }

  শূন্য closure_glyphs (ধ্রুৱ hb_set_t      *unicodes,
		       hb_set_t            *glyphset) ধ্রুৱ
  {
    + hb_iter (encodingRecord)
    | hb_map (&EncodingRecord::subtable)
    | hb_map (hb_add (এতত্))
    | hb_filter (&CmapSubtable::is_variation_selector)
    | hb_apply ([=] (ধ্রুৱ CmapSubtable& _) { _.closure_glyphs (unicodes, glyphset); })
    ;
  }

  তার্কিক subset (hb_subset_context_t *c) ধ্রুৱ
  { ঘূৰাই subset (c, মিছা); }

  তার্কিক subset (hb_subset_context_t *c, তার্কিক is_dmap) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);

    cmap *cmap_মৌলিক = c->serializer->start_embed<cmap> ();

    auto encodingrec_iter =
    + hb_iter (encodingRecord)
    | hb_filter ([&](ধ্রুৱ EncodingRecord& _) {
      ঘূৰাই cmap::filter_encoding_records_for_subset (এতত্, _);
    })
    ;

    যদি (unlikely (!encodingrec_iter.len ())) return_trace (মিছা);

    ধ্রুৱ EncodingRecord *unicode_bmp= nullptr, *unicode_ucs4 = nullptr, *ms_bmp = nullptr, *ms_ucs4 = nullptr;
    তার্কিক has_format12 = মিছা;

    বাবে (ধ্রুৱ EncodingRecord& _ : encodingrec_iter)
    {
      unsigned format = (এতত্ + _.subtable).u.format.v;
      যদি (format == 12) has_format12 = সঁচা;

      ধ্রুৱ EncodingRecord *table = std::addressof (_);
      যদি      (_.platformID == 0 && _.encodingID ==  3) unicode_bmp = table;
      অন্যথা যদি (_.platformID == 0 && _.encodingID ==  4) unicode_ucs4 = table;
      অন্যথা যদি (_.platformID == 3 && _.encodingID ==  1) ms_bmp = table;
      অন্যথা যদি (_.platformID == 3 && _.encodingID == 10) ms_ucs4 = table;
    }

    যদি (unlikely (!is_dmap && !has_format12 && !unicode_bmp && !ms_bmp)) return_trace (মিছা);
    যদি (unlikely (!is_dmap && has_format12 && (!unicode_ucs4 && !ms_ucs4))) return_trace (মিছা);

    auto it =
    + c->plan->unicode_to_new_gid_list.iter ()
    | hb_filter ([&] (ধ্রুৱ hb_codepoint_pair_t _)
		 { ঘূৰাই (_.second != HB_MAP_মান_INVALID); })
    ;

    return_trace (cmap_মৌলিক->serialize (c->serializer,
                                         it,
                                         encodingrec_iter,
                                         এতত্,
                                         c->plan,
					 মিছা,
					 c->source_blob->length));
  }

  ধ্রুৱ CmapSubtable *find_best_subtable (তার্কিক *প্ৰতীক = nullptr,
					  তার্কিক *mac = nullptr,
					  তার্কিক *macroman = nullptr) ধ্রুৱ
  {
    যদি (প্ৰতীক) *প্ৰতীক = মিছা;
    যদি (mac) *mac = মিছা;
    যদি (macroman) *macroman = মিছা;

    ধ্রুৱ CmapSubtable *subtable;

    /* Symbol subtable.
     * Prefer symbol if available.
     * https://github.com/harfbuzz/harfbuzz/issues/1918 */
    যদি ((subtable = এতত্->find_subtable (3, 0)))
    {
      যদি (প্ৰতীক) *প্ৰতীক = সঁচা;
      ঘূৰাই subtable;
    }

    /* 32-bit subtables. */
    যদি ((subtable = এতত্->find_subtable (3, 10))) ঘূৰাই subtable;
    যদি ((subtable = এতত্->find_subtable (0, 6))) ঘূৰাই subtable;
    যদি ((subtable = এতত্->find_subtable (0, 4))) ঘূৰাই subtable;

    /* 16-bit subtables. */
    যদি ((subtable = এতত্->find_subtable (3, 1))) ঘূৰাই subtable;
    যদি ((subtable = এতত্->find_subtable (0, 3))) ঘূৰাই subtable;
    যদি ((subtable = এতত্->find_subtable (0, 2))) ঘূৰাই subtable;
    যদি ((subtable = এতত্->find_subtable (0, 1))) ঘূৰাই subtable;
    যদি ((subtable = এতত্->find_subtable (0, 0))) ঘূৰাই subtable;

    /* MacRoman subtable. */
    যদি ((subtable = এতত্->find_subtable (1, 0)))
    {
      যদি (mac) *mac = সঁচা;
      যদি (macroman) *macroman = সঁচা;
      ঘূৰাই subtable;
    }
    /* Any other Mac subtable; we just map ASCII for these. */
    যদি ((subtable = এতত্->find_subtable (1, 0xFFFF)))
    {
      যদি (mac) *mac = সঁচা;
      ঘূৰাই subtable;
    }

    /* Meh. */
    ঘূৰাই &Null (CmapSubtable);
  }

  সংরচনা accelerator_t
  {
    using cache_t = hb_cache_t<21, 19>;
    static_assert (পরিমাণ (cache_t) == 1024, "");

    accelerator_t (hb_face_t *face, hb_tag_t table_tag = cmap::tableTag)
    {
      এতত্->table = hb_sanitize_context_t ().reference_table<cmap> (face, table_tag);
      যদি (!এতত্->table.get_length ())
	ঘূৰাই;

      তার্কিক প্ৰতীক, mac, macroman;
      এতত্->subtable = table->find_best_subtable (&প্ৰতীক, &mac, &macroman);
      এতত্->subtable_uvs = &Null (CmapSubtable);
      {
	ধ্রুৱ CmapSubtable *st = table->find_variation_selector_subtable ();
	যদি (st)
	  subtable_uvs = st;
      }

#ifndef HB_NO_OT_FONT_CMAP_CACHE
      cache = (cache_t *) hb_malloc (পরিমাণ (cache_t));
      যদি (cache)
	নূতন (cache) cache_t ();
      অন্যথা
        ঘূৰাই; // Such that get_glyph_funcZ remains null.
#endif

      এতত্->get_glyph_data = subtable;
#ifndef HB_NO_CMAP_LEGACY_SUBTABLES
      তার্কিক is_format4 = subtable->u.format.v == 4;
      auto set_format4_getter = [এতত্] (তার্কিক (*func) (ধ্রুৱ শূন্য *,
						      hb_codepoint_t,
						      hb_codepoint_t *))
      {
	এতত্->format4_accel.init (&এতত্->subtable->u.format4,
				  get_subtable_data_size (এতত্->subtable));
	এতত্->get_glyph_data = &এতত্->format4_accel;
	এতত্->get_glyph_funcZ = func;
      };
      যদি (unlikely (প্ৰতীক))
      {
	ৱিকল্প ((unsigned) face->table.OS2->get_font_page ()) {
	পক্ষ OS2::font_page_t::FONT_PAGE_NONE:
	  যদি (is_format4)
	    set_format4_getter (get_glyph_from_প্ৰতীক<CmapSubtableFormat4::accelerator_t, hb_প্ৰতীক_pua_map>);
	  অন্যথা
	    এতত্->get_glyph_funcZ = get_glyph_from_প্ৰতীক<CmapSubtable, hb_প্ৰতীক_pua_map>;
	  ভঙ্গ;
#ifndef HB_NO_OT_SHAPER_ARABIC_FALLBACK
	পক্ষ OS2::font_page_t::FONT_PAGE_SIMP_ARABIC:
	  যদি (is_format4)
	    set_format4_getter (get_glyph_from_প্ৰতীক<CmapSubtableFormat4::accelerator_t, _hb_arabic_pua_simp_map>);
	  অন্যথা
	    এতত্->get_glyph_funcZ = get_glyph_from_প্ৰতীক<CmapSubtable, _hb_arabic_pua_simp_map>;
	  ভঙ্গ;
	পক্ষ OS2::font_page_t::FONT_PAGE_TRAD_ARABIC:
	  যদি (is_format4)
	    set_format4_getter (get_glyph_from_প্ৰতীক<CmapSubtableFormat4::accelerator_t, _hb_arabic_pua_trad_map>);
	  অন্যথা
	    এতত্->get_glyph_funcZ = get_glyph_from_প্ৰতীক<CmapSubtable, _hb_arabic_pua_trad_map>;
	  ভঙ্গ;
#endif
	স্ৱভাৱ:
	  যদি (is_format4)
	    set_format4_getter (get_glyph_from<CmapSubtableFormat4::accelerator_t>);
	  অন্যথা
	    এতত্->get_glyph_funcZ = get_glyph_from<CmapSubtable>;
	  ভঙ্গ;
	}
      }
      অন্যথা যদি (unlikely (macroman))
      {
	যদি (is_format4)
	  set_format4_getter (get_glyph_from_macroman<CmapSubtableFormat4::accelerator_t>);
	অন্যথা
	  এতত্->get_glyph_funcZ = get_glyph_from_macroman<CmapSubtable>;
      }
      অন্যথা যদি (unlikely (mac))
      {
	যদি (is_format4)
	  set_format4_getter (get_glyph_from_ascii<CmapSubtableFormat4::accelerator_t>);
	অন্যথা
	  এতত্->get_glyph_funcZ = get_glyph_from_ascii<CmapSubtable>;
      }
      অন্যথা
#endif
      {
	ৱিকল্প (subtable->u.format.v) {
	  /* Accelerate format 4 and format 12. */
	  স্ৱভাৱ:
	    এতত্->get_glyph_funcZ = get_glyph_from<CmapSubtable>;
	    ভঙ্গ;
	  পক্ষ 12:
	    এতত্->get_glyph_funcZ = get_glyph_from<CmapSubtableFormat12>;
	    ভঙ্গ;
	  পক্ষ  4:
	  {
	    hb_barrier ();
	    এতত্->format4_accel.init (&subtable->u.format4,
				      get_subtable_data_size (subtable));
	    এতত্->get_glyph_data = &এতত্->format4_accel;
	    এতত্->get_glyph_funcZ = এতত্->format4_accel.get_glyph_func;
	    ভঙ্গ;
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

    অন্তঃস্থ তার্কিক _cached_get (hb_codepoint_t unicode,
			     hb_codepoint_t *glyph) ধ্রুৱ
    {
#ifndef HB_NO_OT_FONT_CMAP_CACHE
      // cache is always non-null if we have a get_glyph_funcZ
      unsigned v;
      যদি (cache->get (unicode, &v))
      {
        *glyph = v;
	ঘূৰাই সঁচা;
      }
#endif
      তার্কিক ret  = এতত্->get_glyph_funcZ (এতত্->get_glyph_data, unicode, glyph);

#ifndef HB_NO_OT_FONT_CMAP_CACHE
      যদি (ret)
        cache->set (unicode, *glyph);
#endif

      ঘূৰাই ret;
    }

    তার্কিক get_nominal_glyph (hb_codepoint_t  unicode,
			    hb_codepoint_t *glyph) ধ্রুৱ
    {
      যদি (unlikely (!এতত্->get_glyph_funcZ)) ঘূৰাই মিছা;
      ঘূৰাই _cached_get (unicode, glyph);
    }

    unsigned পূর্ণাঙ্ক get_nominal_glyphs (unsigned পূর্ণাঙ্ক গুনতি,
				     ধ্রুৱ hb_codepoint_t *first_unicode,
				     unsigned পূর্ণাঙ্ক unicode_stride,
				     hb_codepoint_t *first_glyph,
				     unsigned পূর্ণাঙ্ক glyph_stride) ধ্রুৱ
    {
      যদি (unlikely (!এতত্->get_glyph_funcZ)) ঘূৰাই 0;

      unsigned পূর্ণাঙ্ক done;
      বাবে (done = 0;
	   done < গুনতি && _cached_get (*first_unicode, first_glyph);
	   done++)
      {
	first_unicode = &StructAtOffsetUnaligned<hb_codepoint_t> (first_unicode, unicode_stride);
	first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      }
      ঘূৰাই done;
    }

    তার্কিক get_variation_glyph (hb_codepoint_t  unicode,
			      hb_codepoint_t  variation_selector,
			      hb_codepoint_t *glyph) ধ্রুৱ
    {
      ৱিকল্প (get_glyph_variant (unicode, variation_selector, glyph))
      {
	পক্ষ GLYPH_VARIANT_NOT_FOUND:	ঘূৰাই মিছা;
	পক্ষ GLYPH_VARIANT_FOUND:	ঘূৰাই সঁচা;
	পক্ষ GLYPH_VARIANT_USE_DEFAULT:	ভঙ্গ;
      }

      ঘূৰাই get_nominal_glyph (unicode, glyph);
    }

    glyph_variant_t get_glyph_variant (hb_codepoint_t  unicode,
				       hb_codepoint_t  variation_selector,
				       hb_codepoint_t *glyph) ধ্রুৱ
    {
      ঘূৰাই এতত্->subtable_uvs->get_glyph_variant (unicode,
						    variation_selector,
						    glyph);
    }

    তার্কিক has_data () ধ্রুৱ { ঘূৰাই table.get_length (); }

    শূন্য collect_unicodes (hb_set_t *out, unsigned পূর্ণাঙ্ক অঙ্ক_glyphs) ধ্রুৱ
    { subtable->collect_unicodes (out, অঙ্ক_glyphs, get_subtable_data_size (subtable)); }
    শূন্য collect_mapping (hb_set_t *unicodes, hb_map_t *mapping,
			  unsigned অঙ্ক_glyphs = UINT_MAX) ধ্রুৱ
    { subtable->collect_mapping (unicodes, mapping, অঙ্ক_glyphs, get_subtable_data_size (subtable)); }
    শূন্য collect_variation_selectors (hb_set_t *out) ধ্রুৱ
    { subtable_uvs->collect_variation_selectors (out); }
    শূন্য collect_variation_unicodes (hb_codepoint_t variation_selector,
				     hb_set_t *out) ধ্রুৱ
    { subtable_uvs->collect_variation_unicodes (variation_selector, out); }

    রক্ষিত:
    প্রকারনির্দেশ তার্কিক (*hb_cmap_get_glyph_func_t) (ধ্রুৱ শূন্য *obj,
					      hb_codepoint_t codepoint,
					      hb_codepoint_t *glyph);
    প্রকারনির্দেশ uint16_t (*hb_pua_remap_func_t) (unsigned);

    template <typename Type>
    HB_INTERNAL স্থির তার্কিক get_glyph_from (ধ্রুৱ শূন্য *obj,
					    hb_codepoint_t codepoint,
					    hb_codepoint_t *glyph)
    {
      ধ্রুৱ Type *typed_obj = (ধ্রুৱ Type *) obj;
      ঘূৰাই typed_obj->get_glyph (codepoint, glyph);
    }

    template <typename Type, hb_pua_remap_func_t remap>
    HB_INTERNAL স্থির তার্কিক get_glyph_from_প্ৰতীক (ধ্রুৱ শূন্য *obj,
						   hb_codepoint_t codepoint,
						   hb_codepoint_t *glyph)
    {
      ধ্রুৱ Type *typed_obj = (ধ্রুৱ Type *) obj;
      যদি (likely (typed_obj->get_glyph (codepoint, glyph)))
	ঘূৰাই সঁচা;

      যদি (hb_codepoint_t c = remap (codepoint))
	ঘূৰাই typed_obj->get_glyph (c, glyph);

      ঘূৰাই মিছা;
    }

    template <typename Type>
    HB_INTERNAL স্থির তার্কিক get_glyph_from_ascii (ধ্রুৱ শূন্য *obj,
						  hb_codepoint_t codepoint,
						  hb_codepoint_t *glyph)
    {
      ধ্রুৱ Type *typed_obj = (ধ্রুৱ Type *) obj;
      ঘূৰাই codepoint < 0x80 && typed_obj->get_glyph (codepoint, glyph);
    }

    template <typename Type>
    HB_INTERNAL স্থির তার্কিক get_glyph_from_macroman (ধ্রুৱ শূন্য *obj,
						     hb_codepoint_t codepoint,
						     hb_codepoint_t *glyph)
    {
      যদি (get_glyph_from_ascii<Type> (obj, codepoint, glyph))
	ঘূৰাই সঁচা;

      ধ্রুৱ Type *typed_obj = (ধ্রুৱ Type *) obj;
      unsigned c = unicode_to_macroman (codepoint);
      ঘূৰাই c && typed_obj->get_glyph (c, glyph);
    }

    unsigned পূর্ণাঙ্ক get_subtable_data_size (ধ্রুৱ CmapSubtable *subtable) ধ্রুৱ
    {
      unsigned পূর্ণাঙ্ক table_length = এতত্->table.get_length ();
      uintptr_t table_start = (uintptr_t) (ধ্রুৱ শূন্য *) এতত্->table.get ();
      uintptr_t subtable_addr = (uintptr_t) (ধ্রুৱ শূন্য *) subtable;
      যদি (unlikely (subtable_addr < table_start))
	ঘূৰাই 0;

      uintptr_t subtable_offset = subtable_addr - table_start;
      যদি (unlikely (subtable_offset >= table_length))
	ঘূৰাই 0;

      ঘূৰাই table_length - (unsigned পূর্ণাঙ্ক) subtable_offset;
    }

    গুপ্ত:
    hb_nonnull_ptr_t<ধ্রুৱ CmapSubtable> subtable;
    hb_nonnull_ptr_t<ধ্রুৱ CmapSubtable> subtable_uvs;

    hb_cmap_get_glyph_func_t get_glyph_funcZ = nullptr;
    ধ্রুৱ শূন্য *get_glyph_data = nullptr;

    CmapSubtableFormat4::accelerator_t format4_accel;

#ifndef HB_NO_OT_FONT_CMAP_CACHE
    cache_t *cache = nullptr;
#endif

    সার্ৱজনিক:
    hb_blob_ptr_t<cmap> table;
  };

  রক্ষিত:

  ধ্রুৱ CmapSubtable *find_subtable (unsigned পূর্ণাঙ্ক platform_id,
				     unsigned পূর্ণাঙ্ক encoding_id) ধ্রুৱ
  {
    EncodingRecord key;
    key.platformID = platform_id;
    key.encodingID = encoding_id;

    ধ্রুৱ EncodingRecord &result = encodingRecord.bsearch (key);
    যদি (!result.subtable)
      ঘূৰাই nullptr;

    ঘূৰাই &(এতত্+result.subtable);
  }

  ধ্রুৱ CmapSubtable *find_variation_selector_subtable () ধ্রুৱ
  {
    ধ্রুৱ CmapSubtable *format14 = nullptr;
    বাবে (ধ্রুৱ EncodingRecord& record : encodingRecord)
    {
      যদি (record.platformID != 0 || record.encodingID != 5)
	অনুৱর্ত;

      ধ্রুৱ CmapSubtable *subtable = &(এতত্+record.subtable);
#ifndef HB_NO_BEYOND_64K
      যদি (subtable->u.format.v == 15)
	ঘূৰাই subtable;
#endif
      যদি (subtable->u.format.v == 14)
	format14 = subtable;
    }
    ঘূৰাই format14;
  }

  সার্ৱজনিক:

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) &&
		  hb_barrier () &&
		  likely (version == 0) &&
		  encodingRecord.sanitize (c, এতত্));
  }

 গুপ্ত:

  স্থির তার্কিক filter_encoding_records_for_subset(ধ্রুৱ cmap* cmap,
                                                 ধ্রুৱ EncodingRecord& _)
  {
    ঘূৰাই
        (_.platformID == 0 && _.encodingID == 3) ||
        (_.platformID == 0 && _.encodingID == 4) ||
        (_.platformID == 3 && _.encodingID == 1) ||
        (_.platformID == 3 && _.encodingID == 10) ||
        (cmap + _.subtable).is_variation_selector ();
  }

  রক্ষিত:
  HBUINT16	version;	/* Table version number (0). */
  SortedArray16Of<EncodingRecord>
		encodingRecord;	/* Encoding tables. */
  সার্ৱজনিক:
  DEFINE_SIZE_ARRAY (4, encodingRecord);
};

সংরচনা DMAP : cmap
{
  স্থির constexpr hb_tag_t tableTag = HB_OT_TAG_DMAP;

  তার্কিক subset (hb_subset_context_t *c) ধ্রুৱ
  { ঘূৰাই cmap::subset (c, সঁচা); }
};

সংরচনা cmap_accelerator_t
{
  cmap_accelerator_t (hb_face_t *face) :
    dmap (face, HB_OT_TAG_DMAP),
    base (face)
  {}

  তার্কিক get_nominal_glyph (hb_codepoint_t  unicode,
			  hb_codepoint_t *glyph) ধ্রুৱ
  {
    ঘূৰাই dmap.get_nominal_glyph (unicode, glyph) ||
	   base.get_nominal_glyph (unicode, glyph);
  }

  unsigned পূর্ণাঙ্ক get_nominal_glyphs (unsigned পূর্ণাঙ্ক গুনতি,
				   ধ্রুৱ hb_codepoint_t *first_unicode,
				   unsigned পূর্ণাঙ্ক unicode_stride,
				   hb_codepoint_t *first_glyph,
				   unsigned পূর্ণাঙ্ক glyph_stride) ধ্রুৱ
  {
    যদি (likely (!dmap.has_data ()))
      ঘূৰাই base.get_nominal_glyphs (গুনতি,
				      first_unicode, unicode_stride,
				      first_glyph, glyph_stride);

    unsigned পূর্ণাঙ্ক done;
    বাবে (done = 0;
	 done < গুনতি && get_nominal_glyph (*first_unicode, first_glyph);
	 done++)
    {
      first_unicode = &StructAtOffsetUnaligned<hb_codepoint_t> (first_unicode, unicode_stride);
      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
    }
    ঘূৰাই done;
  }

  তার্কিক get_variation_glyph (hb_codepoint_t  unicode,
			    hb_codepoint_t  variation_selector,
			    hb_codepoint_t *glyph) ধ্রুৱ
  {
    glyph_variant_t result = dmap.get_glyph_variant (unicode,
						     variation_selector,
						     glyph);
    যদি (result == GLYPH_VARIANT_NOT_FOUND)
      result = base.get_glyph_variant (unicode, variation_selector, glyph);

    ৱিকল্প (result)
    {
      পক্ষ GLYPH_VARIANT_NOT_FOUND: ঘূৰাই মিছা;
      পক্ষ GLYPH_VARIANT_FOUND: ঘূৰাই সঁচা;
      পক্ষ GLYPH_VARIANT_USE_DEFAULT: ভঙ্গ;
    }

    ঘূৰাই get_nominal_glyph (unicode, glyph);
  }

  শূন্য collect_unicodes (hb_set_t *out, unsigned পূর্ণাঙ্ক অঙ্ক_glyphs) ধ্রুৱ
  {
    base.collect_unicodes (out, অঙ্ক_glyphs);
    dmap.collect_unicodes (out, অঙ্ক_glyphs);
  }

  শূন্য collect_mapping (hb_set_t *unicodes, hb_map_t *mapping,
			unsigned অঙ্ক_glyphs = UINT_MAX) ধ্রুৱ
  {
    base.collect_mapping (unicodes, mapping, অঙ্ক_glyphs);
    dmap.collect_mapping (unicodes, mapping, অঙ্ক_glyphs);
  }

  শূন্য collect_variation_selectors (hb_set_t *out) ধ্রুৱ
  {
    base.collect_variation_selectors (out);
    dmap.collect_variation_selectors (out);
  }

  শূন্য collect_variation_unicodes (hb_codepoint_t variation_selector,
				   hb_set_t *out) ধ্রুৱ
  {
    base.collect_variation_unicodes (variation_selector, out);
    dmap.collect_variation_unicodes (variation_selector, out);
  }

  শূন্য closure_glyphs (ধ্রুৱ hb_set_t *unicodes,
		       hb_set_t       *glyphset) ধ্রুৱ
  {
    যদি (likely (!dmap.has_data ()))
    {
      base.table->closure_glyphs (unicodes, glyphset);
      ঘূৰাই;
    }

    hb_set_t variation_selectors;
    collect_variation_selectors (&variation_selectors);
    বাবে (hb_codepoint_t variation_selector : variation_selectors)
    {
      যদি (!unicodes->has (variation_selector))
	অনুৱর্ত;

      hb_set_t variation_unicodes;
      collect_variation_unicodes (variation_selector, &variation_unicodes);
      বাবে (hb_codepoint_t unicode : variation_unicodes)
      {
	যদি (!unicodes->has (unicode))
	  অনুৱর্ত;

	hb_codepoint_t glyph;
	যদি (get_variation_glyph (unicode, variation_selector, &glyph))
	  glyphset->add (glyph);
      }
    }
  }

  cmap::accelerator_t dmap;
  cmap::accelerator_t base;
};

} /* namespace OT */


#endif /* HB_OT_CMAP_TABLE_HH */
