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

#ଅନ୍ତର୍ଭାଵ "hb-ot-os2-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-shaper-arabic-pua.hh"
#ଅନ୍ତର୍ଭାଵ "hb-open-type.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-dual.hh"
#ଅନ୍ତର୍ଭାଵ "hb-set.hh"
#ଅନ୍ତର୍ଭାଵ "hb-cache.hh"

/*
 * cmap -- Character to Glyph Index Mapping
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cmap
 */
#define HB_OT_TAG_cmap HB_TAG('c','m','a','p')
#define HB_OT_TAG_DMAP HB_TAG('D','M','A','P')

ନାମାକାଶ OT {

ସ୍ଥିର ଧାଡ଼ିଭିତରେ uint8_t unicode_to_macroman (hb_codepoint_t u)
{
  ସ୍ଥିର ଅଚଳ ସଂରଚନା unicode_to_macroman_t
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
  auto *c = hb_bsearch (u, mapping, ARRAY_LENGTH (mapping), ଆକାର (mapping[0]),
			_hb_cmp_operator<uint16_t, uint16_t>);
  ଫେରନ୍ତୁ c ? c->macroman : 0;
}

ସଂରଚନା CmapSubtableFormat0
{
  ତାର୍କିକ get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) ଅଚଳ
  {
    hb_codepoint_t gid = codepoint < 256 ? glyphIdArray[codepoint] : 0;
    ଯଦି (unlikely (!gid))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    *glyph = gid;
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  unsigned get_language () ଅଚଳ
  {
    ଫେରନ୍ତୁ language;
  }

  ଶୂନ୍ୟ collect_unicodes (hb_set_t *out) ଅଚଳ
  {
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < 256; i++)
      ଯଦି (glyphIdArray[i])
	out->add (i);
  }

  ଶୂନ୍ୟ collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping /* OUT */) ଅଚଳ
  {
    ପାଇଁ (unsigned i = 0; i < 256; i++)
      ଯଦି (glyphIdArray[i])
      {
	hb_codepoint_t glyph = glyphIdArray[i];
	unicodes->add (i);
	mapping->set (i, glyph);
      }
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି));
  }

  ସୁରକ୍ଷିତ:
  HBUINT16	format;		/* Format number is set to 0. */
  HBUINT16	length;		/* Byte length of this subtable. */
  HBUINT16	language;	/* Ignore. */
  HBUINT8	glyphIdArray[256];/* An array that maps character
				 * code to glyph index values. */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (6 + 256);
};

ସଂରଚନା CmapSubtableFormat4
{
  template<typename Iterator,
      typename Writer,
	   hb_requires (hb_is_iterator (Iterator))>
  ଶୂନ୍ୟ to_ranges (Iterator it, Writer& range_writer)
  {
    hb_codepoint_t start_cp = 0, prev_run_start_cp = 0, run_start_cp = 0, end_cp = 0, last_gid = 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା run_length = 0 , delta = 0, prev_delta = 0;

    ଗଣନା {
      FIRST_SUB_RANGE,
      FOLLOWING_SUB_RANGE,
    } mode;

    ଯେତେବେଳେ (it) {
      // Start a new range
      {
        ଅଚଳ auto& pair = *it;
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

      ଯେତେବେଳେ (it) {
        // Process range
        ଅଚଳ auto& pair = *it;
        hb_codepoint_t next_cp = pair.first;
        hb_codepoint_t next_gid = pair.second;
        ଯଦି (next_cp != end_cp + 1) {
          // Current range is over, stop processing.
          ଭାଙ୍ଗ;
        }

        ଯଦି (next_gid == last_gid + 1) {
          // The current run continues.
          end_cp = next_cp;
          run_length++;
          last_gid = next_gid;
          it++;
          ଜାରି;
        }

        // A new run is starting, decide if we want to commit the current run.
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା split_cost = (mode == FIRST_SUB_RANGE) ? 8 : 16;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା run_cost = run_length * 2;
        ଯଦି (run_cost >= split_cost) {
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

    ଯଦି (likely (end_cp != 0xFFFF)) {
      range_writer (0xFFFF, 0xFFFF, 1);
    }
  }

  /*
   * Writes the current range as either one or two ranges depending on what is most efficient.
   */
  template<typename Writer>
  ଶୂନ୍ୟ commit_current_range (hb_codepoint_t start,
                             hb_codepoint_t prev_run_start,
                             hb_codepoint_t run_start,
                             hb_codepoint_t end,
                             ପୂର୍ଣ୍ଣସଂଖ୍ୟା run_delta,
                             ପୂର୍ଣ୍ଣସଂଖ୍ୟା previous_run_delta,
                             ପୂର୍ଣ୍ଣସଂଖ୍ୟା split_cost,
                             Writer& range_writer) {
    ତାର୍କିକ should_split = ମିଥ୍ୟା;
    ଯଦି (start < run_start && run_start < end) {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା run_cost = (end - run_start + 1) * 2;
      ଯଦି (run_cost >= split_cost) {
        should_split = ସତ୍ୟ;
      }
    }

    // A zero delta is also the marker for ranges that need a
    // glyphIdArray; serialize_rangeoffset_glyid() tells the two cases
    // apart by re-checking the mapping.
    ଯଦି (should_split) {
      ଯଦି (start == prev_run_start)
        range_writer (start, run_start - 1, previous_run_delta);
      ଅନ୍ୟଥା
        range_writer (start, run_start - 1, 0);
      range_writer (run_start, end, run_delta);
      ଫେରନ୍ତୁ;
    }


    ଯଦି (start == run_start) {
      // Range is only a run
      range_writer (start, end, run_delta);
      ଫେରନ୍ତୁ;
    }

    // Write only a single non-run range.
    range_writer (start, end, 0);
  }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  unsigned serialize_find_segcount (Iterator it) {
    ସଂରଚନା Counter {
      unsigned segcount = 0;

      ଶୂନ୍ୟ operator() (hb_codepoint_t start,
                       hb_codepoint_t end,
                       ପୂର୍ଣ୍ଣସଂଖ୍ୟା delta) {
        segcount++;
      }
    } counter;

    to_ranges (+it, counter);
    ଫେରନ୍ତୁ counter.segcount;
  }


  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  ତାର୍କିକ serialize_start_end_delta_arrays (hb_serialize_context_t *c,
                                         Iterator it,
                                         ପୂର୍ଣ୍ଣସଂଖ୍ୟା segcount)
  {
    ସଂରଚନା Writer {
      hb_serialize_context_t *serializer_;
      HBUINT16* end_code_;
      HBUINT16* start_code_;
      HBINT16* id_delta_;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା index_;

      Writer(hb_serialize_context_t *serializer)
          : serializer_(serializer),
            end_code_(nullptr),
            start_code_(nullptr),
            id_delta_(nullptr),
            index_ (0) {}
      ଶୂନ୍ୟ operator() (hb_codepoint_t start,
                       hb_codepoint_t end,
                       ପୂର୍ଣ୍ଣସଂଖ୍ୟା delta) {
        start_code_[index_] = start;
        end_code_[index_] = end;
        id_delta_[index_] = delta;
        index_++;
      }
    } writer(c);

    writer.end_code_ = c->allocate_size<HBUINT16> (HBUINT16::static_size * segcount, ମିଥ୍ୟା);
    (ଶୂନ୍ୟ) c->allocate_size<HBUINT16> (2); // padding
    writer.start_code_ = c->allocate_size<HBUINT16> (HBUINT16::static_size * segcount, ମିଥ୍ୟା);
    writer.id_delta_ = c->allocate_size<HBINT16> (HBINT16::static_size * segcount, ମିଥ୍ୟା);

    ଯଦି (unlikely (!writer.end_code_ || !writer.start_code_ || !writer.id_delta_)) ଫେରନ୍ତୁ ମିଥ୍ୟା;

    to_ranges (+it, writer);
    ଫେରନ୍ତୁ ସତ୍ୟ;
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
    ଯଦି (unlikely (!c->check_success (idRangeOffset))) ଫେରନ୍ତୁ nullptr;
    ଯଦି (unlikely ((ଅକ୍ଷର *)idRangeOffset - (ଅକ୍ଷର *)idDelta != (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) segcount * (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) HBINT16::static_size)) ଫେରନ୍ତୁ nullptr;

    ପାଇଁ (unsigned i : hb_range (segcount))
    {
      /* A non-zero delta is always a real run. A zero delta is
       * ambiguous: identity mapping, or the marker for a range that
       * needs a glyphIdArray. Only zero deltas need a check. */
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା delta = idDelta[i];
      ଯଦି (delta)
	ଜାରି;
      ତାର୍କିକ use_delta = ସତ୍ୟ;
      ପାଇଁ (hb_codepoint_t cp = startCode[i]; cp <= endCode[i]; cp++)
      {
	hb_codepoint_t gid = cp_to_gid.get (cp);
	ଯଦି (gid == HB_MAP_ମୂଲ୍ୟ_INVALID)
	  gid = 0;
	ଯଦି (gid != cp)
	{
	  use_delta = ମିଥ୍ୟା;
	  ଭାଙ୍ଗ;
	}
      }
      ଯଦି (use_delta)
	ଜାରି;

      idRangeOffset[i] = 2 * (c->start_embed<HBUINT16> () - idRangeOffset - i);
      ପାଇଁ (hb_codepoint_t cp = startCode[i]; cp <= endCode[i]; cp++)
      {
        HBUINT16 gid;
        gid = cp_to_gid[cp];
        c->copy<HBUINT16> (gid);
      }
    }

    ଫେରନ୍ତୁ idRangeOffset;
  }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  ଶୂନ୍ୟ serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    auto format4_iter =
    + it
    | hb_filter ([&] (ଅଚଳ hb_codepoint_pair_t _)
		 { ଫେରନ୍ତୁ _.first <= 0xFFFF; })
    ;

    ଯଦି (!format4_iter) ଫେରନ୍ତୁ;

    unsigned table_initpos = c->length ();
    ଯଦି (unlikely (!c->extend_min (ଏହି))) ଫେରନ୍ତୁ;
    ଏହି->format = 4;

    hb_vector_t<hb_codepoint_pair_t> cp_to_gid {
      format4_iter
    };

    //serialize endCode[], startCode[], idDelta[]
    HBUINT16* endCode = c->start_embed<HBUINT16> ();
    unsigned segcount = serialize_find_segcount (cp_to_gid.iter());
    ଯଦି (unlikely (!serialize_start_end_delta_arrays (c, cp_to_gid.iter(), segcount)))
      ଫେରନ୍ତୁ;

    HBUINT16 *startCode = endCode + segcount + 1;
    HBINT16 *idDelta = ((HBINT16*)startCode) + segcount;

    HBUINT16 *idRangeOffset = serialize_rangeoffset_glyid (c,
                                                           cp_to_gid.iter (),
                                                           endCode,
                                                           startCode,
                                                           idDelta,
                                                           segcount);
    ଯଦି (unlikely (!c->check_success (idRangeOffset))) ଫେରନ୍ତୁ;

    ଏହି->length = c->length () - table_initpos;
    ଯଦି ((long long) ଏହି->length != (long long) c->length () - table_initpos)
    {
      c->err (HB_SERIALIZE_ERROR_INT_OVERFLOW);
      ଫେରନ୍ତୁ;
    }

    ଏହି->seg_ଗଣନା_X2 = segcount * 2;
    ଏହି->entrySelector = hb_max (1u, hb_bit_storage (segcount)) - 1;
    ଏହି->ସନ୍ଧାନ_Range = 2 * (1u << ଏହି->entrySelector);
    ଏହି->rangeShift = segcount * 2 > ଏହି->ସନ୍ଧାନ_Range
		       ? 2 * segcount - ଏହି->ସନ୍ଧାନ_Range
		       : 0;
  }

  unsigned get_language () ଅଚଳ
  {
    ଫେରନ୍ତୁ language;
  }

  ସଂରଚନା accelerator_t
  {
    accelerator_t () {}
    accelerator_t (ଅଚଳ CmapSubtableFormat4 *subtable) = delete;

    ଶୂନ୍ୟ init (ଅଚଳ CmapSubtableFormat4 *subtable,
	       unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା              subtable_data_size)
    {
      seg_ଗଣନା_2 = subtable->seg_ଗଣନା_X2 / 2;
      end_ଗଣନା = subtable->values.arrayZ;
      start_ଗଣନା = end_ଗଣନା + seg_ଗଣନା_2 + 1;
      idDelta = start_ଗଣନା + seg_ଗଣନା_2;
      idRangeOffset = idDelta + seg_ଗଣନା_2;
      glyphIdArray = idRangeOffset + seg_ଗଣନା_2;

      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ମୂଲ୍ୟଗୁଡ଼ିକ_offset = 16 + 8 * seg_ଗଣନା_2;
      glyphIdArrayLength = subtable_data_size > ମୂଲ୍ୟଗୁଡ଼ିକ_offset
			   ? (subtable_data_size - ମୂଲ୍ୟଗୁଡ଼ିକ_offset) / 2
			   : 0;
    }

    ତାର୍କିକ get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) ଅଚଳ
    {
      ସଂରଚନା CustomRange
      {
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp (hb_codepoint_t k,
		 unsigned distance) ଅଚଳ
	{
	  ଯଦି (k > last) ଫେରନ୍ତୁ +1;
	  ଯଦି (k < (&last)[distance]/*first*/) ଫେରନ୍ତୁ -1;
	  ଫେରନ୍ତୁ 0;
	}
	HBUINT16 last;
      };

      ଅଚଳ HBUINT16 *found = hb_bsearch (codepoint,
					  ଏହି->end_ଗଣନା,
					  ଏହି->seg_ଗଣନା_2,
					  ଆକାର (CustomRange),
					  _hb_cmp_method<hb_codepoint_t, CustomRange, unsigned>,
					  ଏହି->seg_ଗଣନା_2 + 1);
      ଯଦି (unlikely (!found))
	ଫେରନ୍ତୁ ମିଥ୍ୟା;
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = found - end_ଗଣନା;

      hb_codepoint_t gid;
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା rangeOffset = ଏହି->idRangeOffset[i];
      ଯଦି (rangeOffset == 0)
	gid = codepoint + ଏହି->idDelta[i];
      ଅନ୍ୟଥା
      {
	/* Somebody has been smoking... */
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = rangeOffset / 2 + (codepoint - ଏହି->start_ଗଣନା[i]) + i - ଏହି->seg_ଗଣନା_2;
	ଯଦି (unlikely (index >= ଏହି->glyphIdArrayLength))
	  ଫେରନ୍ତୁ ମିଥ୍ୟା;
	gid = ଏହି->glyphIdArray[index];
	ଯଦି (unlikely (!gid))
	  ଫେରନ୍ତୁ ମିଥ୍ୟା;
	gid += ଏହି->idDelta[i];
      }
      gid &= 0xFFFFu;
      ଯଦି (unlikely (!gid))
	ଫେରନ୍ତୁ ମିଥ୍ୟା;
      *glyph = gid;
      ଫେରନ୍ତୁ ସତ୍ୟ;
    }

    HB_INTERNAL ସ୍ଥିର ତାର୍କିକ get_glyph_func (ଅଚଳ ଶୂନ୍ୟ *obj, hb_codepoint_t codepoint, hb_codepoint_t *glyph)
    { ଫେରନ୍ତୁ ((ଅଚଳ accelerator_t *) obj)->get_glyph (codepoint, glyph); }

    ଶୂନ୍ୟ collect_unicodes (hb_set_t *out) ଅଚଳ
    {
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = ଏହି->seg_ଗଣନା_2;
      ଯଦି (ଗଣନା_2 && ଏହି->start_ଗଣନା[ଗଣନା_2 - 1] == 0xFFFFu)
	ଗଣନା_2--; /* Skip sentinel segment. */
      ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
      {
	hb_codepoint_t start = ଏହି->start_ଗଣନା[i];
	hb_codepoint_t end = ଏହି->end_ଗଣନା[i];
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା rangeOffset = ଏହି->idRangeOffset[i];
        out->add_range(start, end);
	ଯଦି (rangeOffset == 0)
	{
	  ପାଇଁ (hb_codepoint_t codepoint = start; codepoint <= end; codepoint++)
	  {
	    hb_codepoint_t gid = (codepoint + ଏହି->idDelta[i]) & 0xFFFFu;
	    ଯଦି (unlikely (!gid))
              out->del(codepoint);
	  }
	}
	ଅନ୍ୟଥା
	{
	  ପାଇଁ (hb_codepoint_t codepoint = start; codepoint <= end; codepoint++)
	  {
	    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = rangeOffset / 2 + (codepoint - ଏହି->start_ଗଣନା[i]) + i - ଏହି->seg_ଗଣନା_2;
	    ଯଦି (unlikely (index >= ଏହି->glyphIdArrayLength))
            {
              out->del_range (codepoint, end);
	      ଭାଙ୍ଗ;
            }
	    hb_codepoint_t gid = ଏହି->glyphIdArray[index];
	    ଯଦି (unlikely (!gid))
              out->del(codepoint);
	  }
	}
      }
    }

    ଶୂନ୍ୟ collect_mapping (hb_set_t *unicodes, /* OUT */
			  hb_map_t *mapping /* OUT */) ଅଚଳ
    {
      // TODO(grieger): optimize similar to collect_unicodes
      // (ie. use add_range())
      unsigned ଗଣନା_2 = ଏହି->seg_ଗଣନା_2;
      ଯଦି (ଗଣନା_2 && ଏହି->start_ଗଣନା[ଗଣନା_2 - 1] == 0xFFFFu)
	ଗଣନା_2--; /* Skip sentinel segment. */
      ପାଇଁ (unsigned i = 0; i < ଗଣନା_2; i++)
      {
	hb_codepoint_t start = ଏହି->start_ଗଣନା[i];
	hb_codepoint_t end = ଏହି->end_ଗଣନା[i];
	unsigned rangeOffset = ଏହି->idRangeOffset[i];
	ଯଦି (rangeOffset == 0)
	{
	  ପାଇଁ (hb_codepoint_t codepoint = start; codepoint <= end; codepoint++)
	  {
	    hb_codepoint_t gid = (codepoint + ଏହି->idDelta[i]) & 0xFFFFu;
	    ଯଦି (unlikely (!gid))
	      ଜାରି;
	    unicodes->add (codepoint);
	    mapping->set (codepoint, gid);
	  }
	}
	ଅନ୍ୟଥା
	{
	  ପାଇଁ (hb_codepoint_t codepoint = start; codepoint <= end; codepoint++)
	  {
	    unsigned index = rangeOffset / 2 + (codepoint - ଏହି->start_ଗଣନା[i]) + i - ଏହି->seg_ଗଣନା_2;
	    ଯଦି (unlikely (index >= ଏହି->glyphIdArrayLength))
	      ଭାଙ୍ଗ;
	    hb_codepoint_t gid = ଏହି->glyphIdArray[index];
	    ଯଦି (unlikely (!gid))
	      ଜାରି;
            gid = (gid + ଏହି->idDelta[i]) & 0xFFFFu;
	    unicodes->add (codepoint);
	    mapping->set (codepoint, gid);
	  }
	}
      }
    }

    ଅଚଳ HBUINT16 *end_ଗଣନା;
    ଅଚଳ HBUINT16 *start_ଗଣନା;
    ଅଚଳ HBUINT16 *idDelta;
    ଅଚଳ HBUINT16 *idRangeOffset;
    ଅଚଳ HBUINT16 *glyphIdArray;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା seg_ଗଣନା_2;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା glyphIdArrayLength;
  };

  ତାର୍କିକ get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph,
		  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା subtable_data_size) ଅଚଳ
  {
    accelerator_t accel;
    accel.init (ଏହି, subtable_data_size);
    ଫେରନ୍ତୁ accel.get_glyph_func (&accel, codepoint, glyph);
  }
  ତାର୍କିକ get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) ଅଚଳ
  { ଫେରନ୍ତୁ ମିଥ୍ୟା; }

  ଶୂନ୍ୟ collect_unicodes (hb_set_t *out, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା subtable_data_size) ଅଚଳ
  {
    accelerator_t accel;
    accel.init (ଏହି, subtable_data_size);
    accel.collect_unicodes (out);
  }
  ଶୂନ୍ୟ collect_unicodes (hb_set_t *out) ଅଚଳ
  { collect_unicodes (out, length); }

  ଶୂନ୍ୟ collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping, /* OUT */
			unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା subtable_data_size) ଅଚଳ
  {
    accelerator_t accel;
    accel.init (ଏହି, subtable_data_size);
    accel.collect_mapping (unicodes, mapping);
  }
  ଶୂନ୍ୟ collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping /* OUT */) ଅଚଳ
  { collect_mapping (unicodes, mapping, length); }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    ଯଦି (unlikely (!c->check_struct (ଏହି)))
      return_trace (ମିଥ୍ୟା);
    hb_barrier ();

    return_trace (c->check_range (values, 2 + 4 * seg_ଗଣନା_X2));
  }

  ସୁରକ୍ଷିତ:
  HBUINT16	format;		/* Format number is set to 4. */
  HBUINT16	length;		/* This is the length in bytes of the
				 * subtable. */
  HBUINT16	language;	/* Ignore. */
  HBUINT16	seg_ଗଣନା_X2;	/* 2 x segCount. */
  HBUINT16	ସନ୍ଧାନ_Range;	/* 2 * (2**floor(log2(segCount))) */
  HBUINT16	entrySelector;	/* log2(searchRange/2) */
  HBUINT16	rangeShift;	/* 2 x segCount - searchRange */

  UnsizedArrayOf<HBUINT16>
		values;
#ଯଦି 0
  HBUINT16	end_ଗଣନା[seg_ଗଣନା_2];	/* End characterCode for each segment,
					 * last=0xFFFFu. */
  HBUINT16	reservedPad;		/* Set to 0. */
  HBUINT16	start_ଗଣନା[seg_ଗଣନା_2];	/* Start character code for each segment. */
  HBINT16		idDelta[seg_ଗଣନା_2];	/* Delta for all character codes in segment. */
  HBUINT16	idRangeOffset[seg_ଗଣନା_2];/* Offsets into glyphIdArray or 0 */
  UnsizedArrayOf<HBUINT16>
		glyphIdArray;	/* Glyph index array (arbitrary length) */
#endif

  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (14, values);
};

ସଂରଚନା CmapSubtableLongGroup
{
  friend ସଂରଚନା CmapSubtableFormat12;
  friend ସଂରଚନା CmapSubtableFormat13;
  template<typename U>
  friend ସଂରଚନା CmapSubtableLongSegmented;
  friend ସଂରଚନା cmap;

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp (hb_codepoint_t codepoint) ଅଚଳ
  {
    ଯଦି (codepoint < startCharCode) ଫେରନ୍ତୁ -1;
    ଯଦି (codepoint > endCharCode)   ଫେରନ୍ତୁ +1;
    ଫେରନ୍ତୁ 0;
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି));
  }

  ବ୍ୟକ୍ତିଗତ:
  HBUINT32		startCharCode;	/* First character code in this group. */
  HBUINT32		endCharCode;	/* Last character code in this group. */
  HBUINT32		glyphID;	/* Glyph index; interpretation depends on
					 * subtable format. */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (12);
};
DECLARE_NULL_NAMESPACE_BYTES (OT, CmapSubtableLongGroup);

template <typename UINT>
ସଂରଚନା CmapSubtableTrimmed
{
  ତାର୍କିକ get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) ଅଚଳ
  {
    /* Rely on our implicit array bound-checking. */
    hb_codepoint_t gid = glyphIdArray[codepoint - startCharCode];
    ଯଦି (unlikely (!gid))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    *glyph = gid;
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  unsigned get_language () ଅଚଳ
  {
    ଫେରନ୍ତୁ language;
  }

  ଶୂନ୍ୟ collect_unicodes (hb_set_t *out) ଅଚଳ
  {
    hb_codepoint_t start = startCharCode;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = glyphIdArray.len;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
      ଯଦି (glyphIdArray[i])
	out->add (start + i);
  }

  ଶୂନ୍ୟ collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping /* OUT */) ଅଚଳ
  {
    hb_codepoint_t start_cp = startCharCode;
    unsigned ଗଣନା_2 = glyphIdArray.len;
    ପାଇଁ (unsigned i = 0; i < ଗଣନା_2; i++)
      ଯଦି (glyphIdArray[i])
      {
	hb_codepoint_t unicode = start_cp + i;
	hb_codepoint_t glyphid = glyphIdArray[i];
	unicodes->add (unicode);
	mapping->set (unicode, glyphid);
      }
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) && glyphIdArray.sanitize (c));
  }

  ସୁରକ୍ଷିତ:
  UINT		formatReserved;	/* Subtable format and (maybe) padding. */
  UINT		length;		/* Byte length of this subtable. */
  UINT		language;	/* Ignore. */
  UINT		startCharCode;	/* First character code covered. */
  ArrayOf<HBGlyphID16, UINT>
		glyphIdArray;	/* Array of glyph index values for character
				 * codes in the range. */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (5 * ଆକାର (UINT), glyphIdArray);
};

ସଂରଚନା CmapSubtableFormat6  : CmapSubtableTrimmed<HBUINT16> {};
ସଂରଚନା CmapSubtableFormat10 : CmapSubtableTrimmed<HBUINT32> {};

template <typename T>
ସଂରଚନା CmapSubtableLongSegmented
{
  friend ସଂରଚନା cmap;

  ତାର୍କିକ get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) ଅଚଳ
  {
    hb_codepoint_t gid = T::group_get_glyph (groups.bsearch (codepoint), codepoint);
    ଯଦି (unlikely (!gid))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    *glyph = gid;
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  unsigned get_language () ଅଚଳ
  {
    ଫେରନ୍ତୁ language;
  }

  ଶୂନ୍ୟ collect_unicodes (hb_set_t *out, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_glyphs) ଅଚଳ
  {
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଏହି->groups.len; i++)
    {
      hb_codepoint_t start = ଏହି->groups[i].startCharCode;
      hb_codepoint_t end = hb_min ((hb_codepoint_t) ଏହି->groups[i].endCharCode,
				   (hb_codepoint_t) HB_UNICODE_MAX);
      hb_codepoint_t gid = ଏହି->groups[i].glyphID;
      ଯଦି (!gid)
      {
	/* Intention is: if (hb_is_same (T, CmapSubtableFormat13)) continue; */
	ଯଦି (! T::group_get_glyph (ଏହି->groups[i], end)) ଜାରି;
	start++;
	gid++;
      }
      ଯଦି (unlikely ((unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) gid >= ଅଙ୍କ_glyphs)) ଜାରି;
      ଯଦି (unlikely ((unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) (gid + end - start) >= ଅଙ୍କ_glyphs))
	end = start + (hb_codepoint_t) ଅଙ୍କ_glyphs - gid;

      out->add_range (start, hb_min (end, 0x10FFFFu));
    }
  }

  ଶୂନ୍ୟ collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping, /* OUT */
			unsigned ଅଙ୍କ_glyphs) ଅଚଳ
  {
    hb_codepoint_t last_end = 0;
    unsigned ଗଣନା_2 = ଏହି->groups.len;
    ପାଇଁ (unsigned i = 0; i < ଗଣନା_2; i++)
    {
      hb_codepoint_t start = ଏହି->groups.arrayZ[i].startCharCode;
      hb_codepoint_t end = hb_min ((hb_codepoint_t) ଏହି->groups.arrayZ[i].endCharCode,
				   (hb_codepoint_t) HB_UNICODE_MAX);
      ଯଦି (unlikely (start > end || start < last_end)) {
        // Range is not in order and is invalid, skip it.
        ଜାରି;
      }
      last_end = end;


      hb_codepoint_t gid = ଏହି->groups.arrayZ[i].glyphID;
      ଯଦି (!gid)
      {
        ଯଦି (T::format_ସଂଖ୍ୟା == 13) ଜାରି;
	start++;
	gid++;
      }
      ଯଦି (unlikely ((unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) gid >= ଅଙ୍କ_glyphs)) ଜାରି;
      ଯଦି (unlikely ((unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) (gid + end - start) >= ଅଙ୍କ_glyphs))
	end = start + (hb_codepoint_t) ଅଙ୍କ_glyphs - gid;

      mapping->alloc (mapping->get_population () + end - start + 1);

      unicodes->add_range (start, end);
      ପାଇଁ (unsigned cp = start; cp <= end; cp++)
      {
	mapping->set (cp, gid);
        gid += T::increment;
      }
    }
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) && groups.sanitize (c));
  }

  ସୁରକ୍ଷିତ:
  HBUINT16	format;		/* Subtable format; set to 12. */
  HBUINT16	reserved;	/* Reserved; set to 0. */
  HBUINT32	length;		/* Byte length of this subtable. */
  HBUINT32	language;	/* Ignore. */
  SortedArray32Of<CmapSubtableLongGroup>
		groups;		/* Groupings. */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (16, groups);
};

ସଂରଚନା CmapSubtableFormat12 : CmapSubtableLongSegmented<CmapSubtableFormat12>
{
  ସ୍ଥିର constexpr ପୂର୍ଣ୍ଣସଂଖ୍ୟା increment = 1;
  ସ୍ଥିର constexpr ପୂର୍ଣ୍ଣସଂଖ୍ୟା format_ସଂଖ୍ୟା = 12;

  ସ୍ଥିର hb_codepoint_t group_get_glyph (ଅଚଳ CmapSubtableLongGroup &group,
					 hb_codepoint_t u)
  { ଫେରନ୍ତୁ likely (group.startCharCode <= group.endCharCode) ?
	   group.glyphID + (u - group.startCharCode) : 0; }


  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  ଶୂନ୍ୟ serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    ଯଦି (!it) ଫେରନ୍ତୁ;
    unsigned table_initpos = c->length ();
    ଯଦି (unlikely (!c->extend_min (ଏହି))) ଫେରନ୍ତୁ;

    hb_codepoint_t startCharCode = (hb_codepoint_t) -1, endCharCode = (hb_codepoint_t) -1;
    hb_codepoint_t glyphID = 0;

    ପାଇଁ (ଅଚଳ auto& _ : +it)
    {
      ଯଦି (startCharCode == (hb_codepoint_t) -1)
      {
	startCharCode = _.first;
	endCharCode = _.first;
	glyphID = _.second;
      }
      ଅନ୍ୟଥା ଯଦି (!_is_gid_consecutive (endCharCode, startCharCode, glyphID, _.first, _.second))
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
      ଅନ୍ୟଥା
	endCharCode = _.first;
    }

    CmapSubtableLongGroup record;
    record.startCharCode = startCharCode;
    record.endCharCode = endCharCode;
    record.glyphID = glyphID;
    c->copy<CmapSubtableLongGroup> (record);

    ଏହି->format = 12;
    ଏହି->reserved = 0;
    ଏହି->length = c->length () - table_initpos;
    ଏହି->groups.len = (ଏହି->length - min_size) / CmapSubtableLongGroup::static_size;
  }

  ସ୍ଥିର size_t get_sub_table_size (ଅଚଳ hb_sorted_vector_t<CmapSubtableLongGroup> &groups_data)
  { ଫେରନ୍ତୁ 16 + 12 * groups_data.length; }

  ବ୍ୟକ୍ତିଗତ:
  ସ୍ଥିର ତାର୍କିକ _is_gid_consecutive (hb_codepoint_t endCharCode,
				   hb_codepoint_t startCharCode,
				   hb_codepoint_t glyphID,
				   hb_codepoint_t cp,
				   hb_codepoint_t new_gid)
  {
    ଫେରନ୍ତୁ (cp - 1 == endCharCode) &&
	new_gid == glyphID + (cp - startCharCode);
  }

};

ସଂରଚନା CmapSubtableFormat13 : CmapSubtableLongSegmented<CmapSubtableFormat13>
{
  ସ୍ଥିର constexpr ପୂର୍ଣ୍ଣସଂଖ୍ୟା increment = 0;
  ସ୍ଥିର constexpr ପୂର୍ଣ୍ଣସଂଖ୍ୟା format_ସଂଖ୍ୟା = 13;

  ସ୍ଥିର hb_codepoint_t group_get_glyph (ଅଚଳ CmapSubtableLongGroup &group,
					 hb_codepoint_t u HB_UNUSED)
  { ଫେରନ୍ତୁ group.glyphID; }
};

ପ୍ରକାରବ୍ୟାଖ୍ୟା ଗଣନା
{
  GLYPH_VARIANT_NOT_FOUND = 0,
  GLYPH_VARIANT_FOUND = 1,
  GLYPH_VARIANT_USE_DEFAULT = 2
} glyph_variant_t;

ସଂରଚନା Unicode_ମୂଲ୍ୟ_Range
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp (ଅଚଳ hb_codepoint_t &codepoint) ଅଚଳ
  {
    ଯଦି (codepoint < start_Unicode_ମୂଲ୍ୟ) ଫେରନ୍ତୁ -1;
    ଯଦି (codepoint > start_Unicode_ମୂଲ୍ୟ + additional_ଗଣନା) ଫେରନ୍ତୁ +1;
    ଫେରନ୍ତୁ 0;
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି));
  }

  HBUINT24	start_Unicode_ମୂଲ୍ୟ;	/* First value in this range. */
  HBUINT8	additional_ଗଣନା;	/* Number of additional values in this
					 * range. */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (4);
};

ସଂରଚନା DefaultUVS : SortedArray32Of<Unicode_ମୂଲ୍ୟ_Range>
{
  ଶୂନ୍ୟ collect_unicodes (hb_set_t *out) ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = len;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
    {
      hb_codepoint_t first = arrayZ[i].start_Unicode_ମୂଲ୍ୟ;
      hb_codepoint_t last = hb_min ((hb_codepoint_t) (first + arrayZ[i].additional_ଗଣନା),
				    (hb_codepoint_t) HB_UNICODE_MAX);
      out->add_range (first, last);
    }
  }

  DefaultUVS* copy (hb_serialize_context_t *c,
		    ଅଚଳ hb_set_t *unicodes) ଅଚଳ
  {
    auto *out = c->start_embed<DefaultUVS> ();
    auto snap = c->snapshot ();

    HBUINT32 len;
    len = 0;
    ଯଦି (unlikely (!c->copy<HBUINT32> (len))) ଫେରନ୍ତୁ nullptr;
    unsigned init_len = c->length ();

    ଯଦି (ଏହି->len > unicodes->get_population () * hb_bit_storage ((unsigned) ଏହି->len))
    {
      hb_codepoint_t start = HB_SET_ମୂଲ୍ୟ_INVALID;
      hb_codepoint_t end = HB_SET_ମୂଲ୍ୟ_INVALID;

      ପାଇଁ (auto u : *unicodes)
      {
        ଯଦି (!as_array ().bsearch (u))
	  ଜାରି;
	ଯଦି (start == HB_SET_ମୂଲ୍ୟ_INVALID)
	{
	  start = u;
	  end = start - 1;
	}
	ଯଦି (end + 1 != u || end - start == 255)
        {
	  Unicode_ମୂଲ୍ୟ_Range rec;
	  rec.start_Unicode_ମୂଲ୍ୟ = start;
	  rec.additional_ଗଣନା = end - start;
	  c->copy<Unicode_ମୂଲ୍ୟ_Range> (rec);
	  start = u;
	}
	end = u;
      }
      ଯଦି (start != HB_SET_ମୂଲ୍ୟ_INVALID)
      {
	Unicode_ମୂଲ୍ୟ_Range rec;
	rec.start_Unicode_ମୂଲ୍ୟ = start;
	rec.additional_ଗଣନା = end - start;
	c->copy<Unicode_ମୂଲ୍ୟ_Range> (rec);
      }

    }
    ଅନ୍ୟଥା
    {
      hb_codepoint_t lastCode = HB_SET_ମୂଲ୍ୟ_INVALID;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = -1;

      ପାଇଁ (ଅଚଳ Unicode_ମୂଲ୍ୟ_Range& _ : *ଏହି)
      {
	hb_codepoint_t curEntry = (hb_codepoint_t) (_.start_Unicode_ମୂଲ୍ୟ - 1);
	hb_codepoint_t end = curEntry + _.additional_ଗଣନା + 2;

	ପାଇଁ (; unicodes->next (&curEntry) && curEntry < end;)
	{
	  ଗଣନା_2 += 1;
	  ଯଦି (lastCode == HB_SET_ମୂଲ୍ୟ_INVALID)
	    lastCode = curEntry;
	  ଅନ୍ୟଥା ଯଦି (lastCode + ଗଣନା_2 != curEntry || ଗଣନା_2 > 255)
	  {
	    Unicode_ମୂଲ୍ୟ_Range rec;
	    rec.start_Unicode_ମୂଲ୍ୟ = lastCode;
	    rec.additional_ଗଣନା = ଗଣନା_2 - 1;
	    c->copy<Unicode_ମୂଲ୍ୟ_Range> (rec);

	    lastCode = curEntry;
	    ଗଣନା_2 = 0;
	  }
	}
      }

      ଯଦି (lastCode != HB_MAP_ମୂଲ୍ୟ_INVALID)
      {
	Unicode_ମୂଲ୍ୟ_Range rec;
	rec.start_Unicode_ମୂଲ୍ୟ = lastCode;
	rec.additional_ଗଣନା = ଗଣନା_2;
	c->copy<Unicode_ମୂଲ୍ୟ_Range> (rec);
      }
    }

    ଯଦି (c->length () - init_len == 0)
    {
      c->revert (snap);
      ଫେରନ୍ତୁ nullptr;
    }
    ଅନ୍ୟଥା
    {
      ଯଦି (unlikely (!c->check_assign (out->len,
                                      (c->length () - init_len) / Unicode_ମୂଲ୍ୟ_Range::static_size,
                                      HB_SERIALIZE_ERROR_INT_OVERFLOW))) ଫେରନ୍ତୁ nullptr;
      ଫେରନ୍ତୁ out;
    }
  }

  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (4, *ଏହି);
};

template <typename Types>
ସଂରଚନା UVSMapping
{
  using GlyphID = typename Types::HBGlyphID;

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp (ଅଚଳ hb_codepoint_t &codepoint) ଅଚଳ
  { ଫେରନ୍ତୁ unicode_ମୂଲ୍ୟ.cmp (codepoint); }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି));
  }

  HBUINT24	unicode_ମୂଲ୍ୟ;	/* Base Unicode value of the UVS */
  GlyphID
		glyphID;	/* Glyph ID of the UVS. */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (3 + GlyphID::static_size);
};

template <typename Types>
ସଂରଚନା NonDefaultUVS : SortedArray32Of<UVSMapping<Types>>
{
  using Mapping = UVSMapping<Types>;

  ଶୂନ୍ୟ collect_unicodes (hb_set_t *out) ଅଚଳ
  {
    ପାଇଁ (ଅଚଳ auto& a : ଏହି->as_array ())
      out->add (a.unicode_ମୂଲ୍ୟ);
  }

  ଶୂନ୍ୟ collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping /* OUT */) ଅଚଳ
  {
    ପାଇଁ (ଅଚଳ auto& a : ଏହି->as_array ())
    {
      hb_codepoint_t unicode = a.unicode_ମୂଲ୍ୟ;
      hb_codepoint_t glyphid = a.glyphID;
      unicodes->add (unicode);
      mapping->set (unicode, glyphid);
    }
  }

  ଶୂନ୍ୟ closure_glyphs (ଅଚଳ hb_set_t      *unicodes,
		       hb_set_t            *glyphset) ଅଚଳ
  {
    + ଏହି->as_array ()
    | hb_filter (unicodes, &Mapping::unicode_ମୂଲ୍ୟ)
    | hb_map (&Mapping::glyphID)
    | hb_sink (glyphset)
    ;
  }

  NonDefaultUVS* copy (hb_serialize_context_t *c,
		       ଅଚଳ hb_set_t *unicodes,
		       ଅଚଳ hb_set_t *glyphs_requested,
		       ଅଚଳ hb_map_t *glyph_map) ଅଚଳ
  {
    auto *out = c->start_embed<NonDefaultUVS> ();
    auto it =
    + ଏହି->as_array ()
    | hb_filter ([&] (ଅଚଳ Mapping& _)
		 {
		   ଫେରନ୍ତୁ unicodes->has (_.unicode_ମୂଲ୍ୟ) || glyphs_requested->has (_.glyphID);
		 })
    ;

    ଯଦି (!it) ଫେରନ୍ତୁ nullptr;

    HBUINT32 len;
    len = it.len ();
    ଯଦି (unlikely (!c->copy<HBUINT32> (len))) ଫେରନ୍ତୁ nullptr;

    ପାଇଁ (ଅଚଳ Mapping& _ : it)
    {
      Mapping mapping;
      mapping.unicode_ମୂଲ୍ୟ = _.unicode_ମୂଲ୍ୟ;
      mapping.glyphID = glyph_map->get (_.glyphID);
      c->copy<Mapping> (mapping);
    }

    ଫେରନ୍ତୁ out;
  }

  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (4, *ଏହି);
};

template <typename Types>
ସଂରଚନା VariationSelectorRecord
{
  glyph_variant_t get_glyph (hb_codepoint_t codepoint,
			     hb_codepoint_t *glyph,
			     ଅଚଳ ଶୂନ୍ୟ *base) ଅଚଳ
  {
    ଯଦି ((base+defaultUVS).bfind (codepoint))
      ଫେରନ୍ତୁ GLYPH_VARIANT_USE_DEFAULT;
    ଅଚଳ UVSMapping<Types> &nonDefault = (base+nonDefaultUVS).bsearch (codepoint);
    ଯଦି (nonDefault.glyphID)
    {
      *glyph = nonDefault.glyphID;
       ଫେରନ୍ତୁ GLYPH_VARIANT_FOUND;
    }
    ଫେରନ୍ତୁ GLYPH_VARIANT_NOT_FOUND;
  }

  VariationSelectorRecord(ଅଚଳ VariationSelectorRecord& other)
  {
    *ଏହି = other;
  }

  ଶୂନ୍ୟ operator= (ଅଚଳ VariationSelectorRecord& other)
  {
    varSelector = other.varSelector;
    HBUINT32 offset = other.defaultUVS;
    defaultUVS = offset;
    offset = other.nonDefaultUVS;
    nonDefaultUVS = offset;
  }

  ଶୂନ୍ୟ collect_unicodes (hb_set_t *out, ଅଚଳ ଶୂନ୍ୟ *base) ଅଚଳ
  {
    (base+defaultUVS).collect_unicodes (out);
    (base+nonDefaultUVS).collect_unicodes (out);
  }

  ଶୂନ୍ୟ collect_mapping (ଅଚଳ ଶୂନ୍ୟ *base,
			hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping /* OUT */) ଅଚଳ
  {
    (base+defaultUVS).collect_unicodes (unicodes);
    (base+nonDefaultUVS).collect_mapping (unicodes, mapping);
  }

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp (ଅଚଳ hb_codepoint_t &variation_selector) ଅଚଳ
  { ଫେରନ୍ତୁ varSelector.cmp (variation_selector); }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c, ଅଚଳ ଶୂନ୍ୟ *base) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) &&
		  defaultUVS.sanitize (c, base) &&
		  nonDefaultUVS.sanitize (c, base));
  }

  hb_pair_t<unsigned, unsigned>
  copy (hb_serialize_context_t *c,
	ଅଚଳ hb_set_t *unicodes,
	ଅଚଳ hb_set_t *glyphs_requested,
	ଅଚଳ hb_map_t *glyph_map,
	ଅଚଳ ଶୂନ୍ୟ *base) ଅଚଳ
  {
    auto snap = c->snapshot ();
    auto *out = c->embed<VariationSelectorRecord> (*ଏହି);
    ଯଦି (unlikely (!out)) ଫେରନ୍ତୁ hb_pair (0, 0);

    out->defaultUVS = 0;
    out->nonDefaultUVS = 0;

    unsigned non_default_uvs_objidx = 0;
    ଯଦି (nonDefaultUVS != 0)
    {
      c->push ();
      ଯଦି (c->copy (base+nonDefaultUVS, unicodes, glyphs_requested, glyph_map))
	non_default_uvs_objidx = c->କାଢ଼_pack ();
      ଅନ୍ୟଥା c->କାଢ଼_discard ();
    }

    unsigned default_uvs_objidx = 0;
    ଯଦି (defaultUVS != 0)
    {
      c->push ();
      ଯଦି (c->copy (base+defaultUVS, unicodes))
	default_uvs_objidx = c->କାଢ଼_pack ();
      ଅନ୍ୟଥା c->କାଢ଼_discard ();
    }


    ଯଦି (!default_uvs_objidx && !non_default_uvs_objidx)
      c->revert (snap);

    ଫେରନ୍ତୁ hb_pair (default_uvs_objidx, non_default_uvs_objidx);
  }

  HBUINT24	varSelector;	/* Variation selector. */
  Offset32To<DefaultUVS>
		defaultUVS;	/* Offset to Default UVS Table.  May be 0. */
  Offset32To<NonDefaultUVS<Types>>
		nonDefaultUVS;	/* Offset to Non-Default UVS Table.  May be 0. */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (11);
};

template <typename Types>
ସଂରଚନା CmapSubtableFormat14_15
{
  ସ୍ଥିର constexpr unsigned format_ମୂଲ୍ୟ = Types::size == 2 ? 14 : 15;
  using Record = VariationSelectorRecord<Types>;
  using NonDefault = NonDefaultUVS<Types>;

  glyph_variant_t get_glyph_variant (hb_codepoint_t codepoint,
				     hb_codepoint_t variation_selector,
				     hb_codepoint_t *glyph) ଅଚଳ
  { ଫେରନ୍ତୁ record.bsearch (variation_selector).get_glyph (codepoint, glyph, ଏହି); }

  ଶୂନ୍ୟ collect_variation_selectors (hb_set_t *out) ଅଚଳ
  {
    ପାଇଁ (ଅଚଳ auto& a : record.as_array ())
      out->add (a.varSelector);
  }
  ଶୂନ୍ୟ collect_variation_unicodes (hb_codepoint_t variation_selector,
				   hb_set_t *out) ଅଚଳ
  { record.bsearch (variation_selector).collect_unicodes (out, ଏହି); }

  ଶୂନ୍ୟ serialize (hb_serialize_context_t *c,
		  ଅଚଳ hb_set_t *unicodes,
		  ଅଚଳ hb_set_t *glyphs_requested,
		  ଅଚଳ hb_map_t *glyph_map,
		  ଅଚଳ ଶୂନ୍ୟ *base)
  {
    auto snap = c->snapshot ();
    unsigned table_initpos = c->length ();
    ଅଚଳ ଅକ୍ଷର* init_tail = c->tail;

    ଯଦି (unlikely (!c->extend_min (ଏହି))) ଫେରନ୍ତୁ;
    ଏହି->format = format_ମୂଲ୍ୟ;

    auto src_tbl = reinterpret_cast<ଅଚଳ CmapSubtableFormat14_15*> (base);

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
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = src_tbl->record.len - 1; i >= 0; i--)
    {
      ଯଦି (!unicodes->has(src_tbl->record[i].varSelector))
        ଜାରି;

      hb_pair_t<unsigned, unsigned> result = src_tbl->record[i].copy (c, unicodes, glyphs_requested, glyph_map, base);
      ଯଦି (result.first || result.second)
	obj_indices.push (result);
    }

    ଯଦି (c->length () - table_initpos == CmapSubtableFormat14_15::min_size)
    {
      c->revert (snap);
      ଫେରନ୍ତୁ;
    }

    ଯଦି (unlikely (!c->check_success (!obj_indices.in_error ())))
      ଫେରନ୍ତୁ;

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା tail_len = init_tail - c->tail;
    c->check_assign (ଏହି->length, c->length () - table_initpos + tail_len,
                     HB_SERIALIZE_ERROR_INT_OVERFLOW);
    c->check_assign (ଏହି->record.len,
		     (c->length () - table_initpos - CmapSubtableFormat14_15::min_size) /
		     Record::static_size,
                     HB_SERIALIZE_ERROR_INT_OVERFLOW);

    /* Correct the incorrect write order by reversing the order of the variation
       records array. */
    _reverse_variation_records ();

    /* Now that records are in the right order, we can set up the offsets. */
    _add_links_to_variation_records (c, obj_indices);
  }

  ଶୂନ୍ୟ _reverse_variation_records ()
  {
    record.as_array ().reverse ();
  }

  ଶୂନ୍ୟ _add_links_to_variation_records (hb_serialize_context_t *c,
					ଅଚଳ hb_vector_t<hb_pair_t<unsigned, unsigned>>& obj_indices)
  {
    ପାଇଁ (unsigned i = 0; i < obj_indices.length; i++)
    {
      /*
       * Since the record array has been reversed (see comments in copy())
       * but obj_indices has not been, the indices at obj_indices[i]
       * are for the variation record at record[j].
       */
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = obj_indices.length - 1 - i;
      c->add_link (record[j].defaultUVS, obj_indices[i].first);
      c->add_link (record[j].nonDefaultUVS, obj_indices[i].second);
    }
  }

  ଶୂନ୍ୟ closure_glyphs (ଅଚଳ hb_set_t      *unicodes,
		       hb_set_t            *glyphset) ଅଚଳ
  {
    + hb_iter (record)
    | hb_filter (hb_bool, &Record::nonDefaultUVS)
    | hb_filter (unicodes, &Record::varSelector)
    | hb_map (&Record::nonDefaultUVS)
    | hb_map (hb_add (ଏହି))
    | hb_apply ([=] (ଅଚଳ NonDefault& _) { _.closure_glyphs (unicodes, glyphset); })
    ;
  }

  ଶୂନ୍ୟ collect_unicodes (hb_set_t *out) ଅଚଳ
  {
    ପାଇଁ (ଅଚଳ Record& _ : record)
      _.collect_unicodes (out, ଏହି);
  }

  ଶୂନ୍ୟ collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping /* OUT */) ଅଚଳ
  {
    ପାଇଁ (ଅଚଳ Record& _ : record)
      _.collect_mapping (ଏହି, unicodes, mapping);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) &&
		  record.sanitize (c, ଏହି));
  }

  ସୁରକ୍ଷିତ:
  HBUINT16	format;		/* Format number is set to 14 or 15. */
  HBUINT32	length;		/* Byte length of this subtable. */
  SortedArray32Of<Record>
		record;		/* Variation selector records; sorted
				 * in increasing order of `varSelector'. */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (10, record);
};

ସଂରଚନା CmapSubtable
{
  /* Note: We intentionally do NOT implement subtable formats 2 and 8. */

  ତାର୍କିକ is_variation_selector () ଅଚଳ
  {
    ବାଛ (u.format.v) {
    ସ୍ଥିତି 14:
#ifndef HB_NO_BEYOND_64K
    ସ୍ଥିତି 15:
#endif
      ଫେରନ୍ତୁ ସତ୍ୟ;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
  }

  glyph_variant_t get_glyph_variant (hb_codepoint_t codepoint,
				     hb_codepoint_t variation_selector,
				     hb_codepoint_t *glyph) ଅଚଳ
  {
    ବାଛ (u.format.v) {
    ସ୍ଥିତି 14: hb_barrier (); ଫେରନ୍ତୁ u.format14.get_glyph_variant (codepoint, variation_selector, glyph);
#ifndef HB_NO_BEYOND_64K
    ସ୍ଥିତି 15: hb_barrier (); ଫେରନ୍ତୁ u.format15.get_glyph_variant (codepoint, variation_selector, glyph);
#endif
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ: ଫେରନ୍ତୁ GLYPH_VARIANT_NOT_FOUND;
    }
  }

  ଶୂନ୍ୟ collect_variation_selectors (hb_set_t *out) ଅଚଳ
  {
    ବାଛ (u.format.v) {
    ସ୍ଥିତି 14: hb_barrier (); u.format14.collect_variation_selectors (out); ଫେରନ୍ତୁ;
#ifndef HB_NO_BEYOND_64K
    ସ୍ଥିତି 15: hb_barrier (); u.format15.collect_variation_selectors (out); ଫେରନ୍ତୁ;
#endif
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ: ଫେରନ୍ତୁ;
    }
  }

  ଶୂନ୍ୟ collect_variation_unicodes (hb_codepoint_t variation_selector,
				    hb_set_t *out) ଅଚଳ
  {
    ବାଛ (u.format.v) {
    ସ୍ଥିତି 14: hb_barrier (); u.format14.collect_variation_unicodes (variation_selector, out); ଫେରନ୍ତୁ;
#ifndef HB_NO_BEYOND_64K
    ସ୍ଥିତି 15: hb_barrier (); u.format15.collect_variation_unicodes (variation_selector, out); ଫେରନ୍ତୁ;
#endif
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ: ଫେରନ୍ତୁ;
    }
  }

  ଶୂନ୍ୟ closure_glyphs (ଅଚଳ hb_set_t *unicodes, hb_set_t *glyphset) ଅଚଳ
  {
    ବାଛ (u.format.v) {
    ସ୍ଥିତି 14: hb_barrier (); u.format14.closure_glyphs (unicodes, glyphset); ଫେରନ୍ତୁ;
#ifndef HB_NO_BEYOND_64K
    ସ୍ଥିତି 15: hb_barrier (); u.format15.closure_glyphs (unicodes, glyphset); ଫେରନ୍ତୁ;
#endif
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ: ଫେରନ୍ତୁ;
    }
  }

  ତାର୍କିକ get_glyph (hb_codepoint_t codepoint,
		  hb_codepoint_t *glyph,
		  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା subtable_data_size = 0) ଅଚଳ
  {
    ବାଛ (u.format.v) {
    ସ୍ଥିତି  0: hb_barrier (); ଫେରନ୍ତୁ u.format0 .get_glyph (codepoint, glyph);
    ସ୍ଥିତି  4: hb_barrier (); ଫେରନ୍ତୁ subtable_data_size
				      ? u.format4.get_glyph (codepoint, glyph, subtable_data_size)
				      : ମିଥ୍ୟା;
    ସ୍ଥିତି  6: hb_barrier (); ଫେରନ୍ତୁ u.format6 .get_glyph (codepoint, glyph);
    ସ୍ଥିତି 10: hb_barrier (); ଫେରନ୍ତୁ u.format10.get_glyph (codepoint, glyph);
    ସ୍ଥିତି 12: hb_barrier (); ଫେରନ୍ତୁ u.format12.get_glyph (codepoint, glyph);
    ସ୍ଥିତି 13: hb_barrier (); ଫେରନ୍ତୁ u.format13.get_glyph (codepoint, glyph);
    ସ୍ଥିତି 14:
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ: ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
  }
  ଶୂନ୍ୟ collect_unicodes (hb_set_t *out,
			 unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_glyphs = UINT_MAX,
			 unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା subtable_data_size = 0) ଅଚଳ
  {
    ବାଛ (u.format.v) {
    ସ୍ଥିତି  0: hb_barrier (); u.format0 .collect_unicodes (out); ଫେରନ୍ତୁ;
    ସ୍ଥିତି  4: hb_barrier (); ଯଦି (subtable_data_size)
			      u.format4.collect_unicodes (out, subtable_data_size);
			    ଅନ୍ୟଥା
			      u.format4.collect_unicodes (out);
			    ଫେରନ୍ତୁ;
    ସ୍ଥିତି  6: hb_barrier (); u.format6 .collect_unicodes (out); ଫେରନ୍ତୁ;
    ସ୍ଥିତି 10: hb_barrier (); u.format10.collect_unicodes (out); ଫେରନ୍ତୁ;
    ସ୍ଥିତି 12: hb_barrier (); u.format12.collect_unicodes (out, ଅଙ୍କ_glyphs); ଫେରନ୍ତୁ;
    ସ୍ଥିତି 13: hb_barrier (); u.format13.collect_unicodes (out, ଅଙ୍କ_glyphs); ଫେରନ୍ତୁ;
    ସ୍ଥିତି 14:
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ: ଫେରନ୍ତୁ;
    }
  }

  ଶୂନ୍ୟ collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping, /* OUT */
			unsigned ଅଙ୍କ_glyphs = UINT_MAX,
			unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା subtable_data_size = 0) ଅଚଳ
  {
    ବାଛ (u.format.v) {
    ସ୍ଥିତି  0: hb_barrier (); u.format0 .collect_mapping (unicodes, mapping); ଫେରନ୍ତୁ;
    ସ୍ଥିତି  4: hb_barrier (); ଯଦି (subtable_data_size)
			      u.format4.collect_mapping (unicodes, mapping, subtable_data_size);
			    ଅନ୍ୟଥା
			      u.format4.collect_mapping (unicodes, mapping);
			    ଫେରନ୍ତୁ;
    ସ୍ଥିତି  6: hb_barrier (); u.format6 .collect_mapping (unicodes, mapping); ଫେରନ୍ତୁ;
    ସ୍ଥିତି 10: hb_barrier (); u.format10.collect_mapping (unicodes, mapping); ଫେରନ୍ତୁ;
    ସ୍ଥିତି 12: hb_barrier (); u.format12.collect_mapping (unicodes, mapping, ଅଙ୍କ_glyphs); ଫେରନ୍ତୁ;
    ସ୍ଥିତି 13: hb_barrier (); u.format13.collect_mapping (unicodes, mapping, ଅଙ୍କ_glyphs); ଫେରନ୍ତୁ;
    ସ୍ଥିତି 14:
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ: ଫେରନ୍ତୁ;
    }
  }

  unsigned get_language () ଅଚଳ
  {
    ବାଛ (u.format.v) {
    ସ୍ଥିତି  0: hb_barrier (); ଫେରନ୍ତୁ u.format0 .get_language ();
    ସ୍ଥିତି  4: hb_barrier (); ଫେରନ୍ତୁ u.format4 .get_language ();
    ସ୍ଥିତି  6: hb_barrier (); ଫେରନ୍ତୁ u.format6 .get_language ();
    ସ୍ଥିତି 10: hb_barrier (); ଫେରନ୍ତୁ u.format10.get_language ();
    ସ୍ଥିତି 12: hb_barrier (); ଫେରନ୍ତୁ u.format12.get_language ();
    ସ୍ଥିତି 13: hb_barrier (); ଫେରନ୍ତୁ u.format13.get_language ();
    ସ୍ଥିତି 14:
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ: ଫେରନ୍ତୁ 0;
    }
  }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  ଶୂନ୍ୟ serialize (hb_serialize_context_t *c,
		  Iterator it,
		  unsigned format,
		  ଅଚଳ hb_subset_plan_t *plan,
		  ଅଚଳ ଶୂନ୍ୟ *base)
  {
    ବାଛ (format) {
    ସ୍ଥିତି  4: hb_barrier (); ଫେରନ୍ତୁ u.format4.serialize (c, it);
    ସ୍ଥିତି 12: hb_barrier (); ଫେରନ୍ତୁ u.format12.serialize (c, it);
    ସ୍ଥିତି 14: hb_barrier (); ଫେରନ୍ତୁ u.format14.serialize (c, &plan->unicodes, &plan->glyphs_requested, plan->glyph_map, base);
#ifndef HB_NO_BEYOND_64K
    ସ୍ଥିତି 15: hb_barrier (); ଫେରନ୍ତୁ u.format15.serialize (c, &plan->unicodes, &plan->glyphs_requested, plan->glyph_map, base);
#endif
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ: ଫେରନ୍ତୁ;
    }
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    ଯଦି (!u.format.v.sanitize (c)) return_trace (ମିଥ୍ୟା);
    hb_barrier ();
    ବାଛ (u.format.v) {
    ସ୍ଥିତି  0: hb_barrier (); return_trace (u.format0 .sanitize (c));
    ସ୍ଥିତି  4: hb_barrier (); return_trace (u.format4 .sanitize (c));
    ସ୍ଥିତି  6: hb_barrier (); return_trace (u.format6 .sanitize (c));
    ସ୍ଥିତି 10: hb_barrier (); return_trace (u.format10.sanitize (c));
    ସ୍ଥିତି 12: hb_barrier (); return_trace (u.format12.sanitize (c));
    ସ୍ଥିତି 13: hb_barrier (); return_trace (u.format13.sanitize (c));
    ସ୍ଥିତି 14: hb_barrier (); return_trace (u.format14.sanitize (c));
#ifndef HB_NO_BEYOND_64K
    ସ୍ଥିତି 15: hb_barrier (); return_trace (u.format15.sanitize (c));
#endif
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:return_trace (ସତ୍ୟ);
    }
  }

  ସର୍ବସାଧାରଣ:
  ସଂଘ {
  ସଂରଚନା { HBUINT16 v; }	format;		/* Format identifier */
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
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_UNION (2, format.v);
};


ସଂରଚନା EncodingRecord
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp (ଅଚଳ EncodingRecord &other) ଅଚଳ
  {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
    ret = platformID.cmp (other.platformID);
    ଯଦି (ret) ଫେରନ୍ତୁ ret;
    ଯଦି (other.encodingID != 0xFFFF)
    {
      ret = encodingID.cmp (other.encodingID);
      ଯଦି (ret) ଫେରନ୍ତୁ ret;
    }
    ଫେରନ୍ତୁ 0;
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c, ଅଚଳ ଶୂନ୍ୟ *base) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) &&
		  subtable.sanitize (c, base));
  }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  EncodingRecord* copy (hb_serialize_context_t *c,
			Iterator it,
			unsigned format,
			ଅଚଳ ଶୂନ୍ୟ *base,
			ଅଚଳ hb_subset_plan_t *plan,
			/* INOUT */ unsigned *objidx) ଅଚଳ
  {
    TRACE_SERIALIZE (ଏହି);
    auto snap = c->snapshot ();
    auto *out = c->embed (ଏହି);
    ଯଦି (unlikely (!out)) return_trace (nullptr);
    out->subtable = 0;

    ଯଦି (*objidx == 0)
    {
      CmapSubtable *cmapsubtable = c->push<CmapSubtable> ();
      unsigned origin_length = c->length ();
      cmapsubtable->serialize (c, it, format, plan, &(base+subtable));
      ଯଦି (c->length () > origin_length && !c->in_error()) *objidx = c->କାଢ଼_pack ();
      ଅନ୍ୟଥା c->କାଢ଼_discard ();
    }

    ଯଦି (*objidx == 0)
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
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (8);
};

ସଂରଚନା cmap;

ସଂରଚନା SubtableUnicodesCache {

 ବ୍ୟକ୍ତିଗତ:
  hb_blob_ptr_t<cmap> base_blob;
  ଅଚଳ ଅକ୍ଷର* base;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା table_length;
  hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> cached_unicodes;

 ସର୍ବସାଧାରଣ:

  ସ୍ଥିର SubtableUnicodesCache* create (hb_blob_ptr_t<cmap> source_table)
  {
    SubtableUnicodesCache* cache =
        (SubtableUnicodesCache*) hb_malloc (ଆକାର(SubtableUnicodesCache));

    ଯଦି (unlikely (!cache))
      ଫେରନ୍ତୁ nullptr;

    ନୂତନ (cache) SubtableUnicodesCache (source_table);
    ଫେରନ୍ତୁ cache;
  }

  ସ୍ଥିର ଶୂନ୍ୟ destroy (ଶୂନ୍ୟ* value) {
    ଯଦି (!value) ଫେରନ୍ତୁ;

    SubtableUnicodesCache* cache = (SubtableUnicodesCache*) value;
    cache->~SubtableUnicodesCache ();
    hb_free (cache);
  }

  SubtableUnicodesCache(ଅଚଳ ଶୂନ୍ୟ* cmap_base,
			unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା table_length_ = 0)
      : base_blob(),
        base ((ଅଚଳ ଅକ୍ଷର*) cmap_base),
        table_length (table_length_),
        cached_unicodes ()
  {}

  SubtableUnicodesCache(hb_blob_ptr_t<cmap> base_blob_)
      : base_blob(base_blob_),
        base ((ଅଚଳ ଅକ୍ଷର *) base_blob.get()),
        table_length (base_blob.get_length ()),
        cached_unicodes ()
  {}

  ~SubtableUnicodesCache()
  {
    base_blob.destroy ();
  }

  ତାର୍କିକ same_base(ଅଚଳ ଶୂନ୍ୟ* other) ଅଚଳ
  {
    ଫେରନ୍ତୁ other == (ଅଚଳ ଶୂନ୍ୟ*) base;
  }

  ଅଚଳ hb_set_t* set_for (ଅଚଳ EncodingRecord* record,
                           SubtableUnicodesCache& mutable_cache) ଅଚଳ
  {
    ଯଦି (cached_unicodes.has ((unsigned) ((ଅଚଳ ଅକ୍ଷର *) record - base)))
      ଫେରନ୍ତୁ cached_unicodes.get ((unsigned) ((ଅଚଳ ଅକ୍ଷର *) record - base));

    ଫେରନ୍ତୁ mutable_cache.set_for (record);
  }

  ଅଚଳ hb_set_t* set_for (ଅଚଳ EncodingRecord* record)
  {
    ଯଦି (!cached_unicodes.has ((unsigned) ((ଅଚଳ ଅକ୍ଷର *) record - base)))
    {
      hb_set_t *s = hb_set_create ();
      ଯଦି (unlikely (s->in_error ()))
	ଫେରନ୍ତୁ hb_set_get_empty ();

      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା subtable_data_size = record->subtable < table_length
					? table_length - (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) record->subtable
					: 0;
      (base+record->subtable).collect_unicodes (s, UINT_MAX, subtable_data_size);

      ଯଦି (unlikely (!cached_unicodes.set ((unsigned) ((ଅଚଳ ଅକ୍ଷର *) record - base), hb::unique_ptr<hb_set_t> {s})))
        ଫେରନ୍ତୁ hb_set_get_empty ();

      ଫେରନ୍ତୁ s;
    }
    ଫେରନ୍ତୁ cached_unicodes.get ((unsigned) ((ଅଚଳ ଅକ୍ଷର *) record - base));
  }

};

ସ୍ଥିର ଧାଡ଼ିଭିତରେ uint16_t
hb_ପ୍ରତୀକ_pua_map (unsigned codepoint)
{
  ଯଦି (codepoint <= 0x00FFu)
  {
    /* For symbol-encoded OpenType fonts, we duplicate the
     * U+F000..F0FF range at U+0000..U+00FF.  That's what
     * Windows seems to do, and that's hinted about at:
     * https://docs.microsoft.com/en-us/typography/opentype/spec/recom
     * under "Non-Standard (Symbol) Fonts". */
    ଫେରନ୍ତୁ 0xF000u + codepoint;
  }
  ଫେରନ୍ତୁ 0;
}

ସଂରଚନା cmap
{
  ସ୍ଥିର constexpr hb_tag_t tableTag = HB_OT_TAG_cmap;


  ସ୍ଥିର SubtableUnicodesCache* create_filled_cache(hb_blob_ptr_t<cmap> source_table) {
    ଅଚଳ cmap* cmap = source_table.get();
    auto it =
    + hb_iter (cmap->encodingRecord)
    | hb_filter ([&](ଅଚଳ EncodingRecord& _) {
      ଫେରନ୍ତୁ cmap::filter_encoding_records_for_subset (cmap, _);
    })
    ;

    SubtableUnicodesCache* cache = SubtableUnicodesCache::create(source_table);

    ଯଦି (unlikely (!cache))
      ଫେରନ୍ତୁ nullptr;

    ପାଇଁ (ଅଚଳ EncodingRecord& _ : it)
      cache->set_for(&_); // populate the cache for this encoding record.

    ଫେରନ୍ତୁ cache;
  }

  template<typename Iterator, typename EncodingRecIter,
	   hb_requires (hb_is_iterator (EncodingRecIter))>
  ତାର୍କିକ serialize (hb_serialize_context_t *c,
		  Iterator it,
		  EncodingRecIter encodingrec_iter,
		  ଅଚଳ ଶୂନ୍ୟ *base,
		  hb_subset_plan_t *plan,
	                  ତାର୍କିକ drop_format_4 = ମିଥ୍ୟା,
			  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା source_table_length = 0)
  {
    ଯଦି (unlikely (!c->extend_min ((*ଏହି))))  ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଏହି->version = 0;

    unsigned format4objidx = 0, format12objidx = 0, format14objidx = 0;
#ifndef HB_NO_BEYOND_64K
    unsigned format15objidx = 0;
#endif
    auto snap = c->snapshot ();

    SubtableUnicodesCache local_unicodes_cache (base, source_table_length);
    ଅଚଳ SubtableUnicodesCache* unicodes_cache = &local_unicodes_cache;

    ଯଦି (plan->accelerator &&
        plan->accelerator->cmap_cache &&
        plan->accelerator->cmap_cache->same_base (base))
      unicodes_cache = plan->accelerator->cmap_cache;

    ପାଇଁ (ଅଚଳ EncodingRecord& _ : encodingrec_iter)
    {
      ଯଦି (c->in_error ())
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

      unsigned format = (base+_.subtable).u.format.v;
      ଯଦି (format != 4 && format != 12 && format != 14
#ifndef HB_NO_BEYOND_64K
	  && format != 15
#endif
	 ) ଜାରି;

      ଅଚଳ hb_set_t* unicodes_set = unicodes_cache->set_for (&_, local_unicodes_cache);

      ଯଦି (!drop_format_4 && format == 4)
      {
        c->copy (_, + it | hb_filter (*unicodes_set, hb_first), 4u, base, plan, &format4objidx);
        ଯଦି (c->in_error () && c->only_overflow ())
        {
          // cmap4 overflowed, reset and retry serialization without format 4 subtables.
          c->revert (snap);
          ଫେରନ୍ତୁ serialize (c, it,
                            encodingrec_iter,
                            base,
                            plan,
                            ସତ୍ୟ,
			    source_table_length);
        }
      }

      ଅନ୍ୟଥା ଯଦି (format == 12)
      {
        ଯଦି (_can_drop (_,
                       *unicodes_set,
                       base,
                       *unicodes_cache,
                       local_unicodes_cache,
                       + it | hb_map (hb_first), encodingrec_iter))
          ଜାରି;
        c->copy (_, + it | hb_filter (*unicodes_set, hb_first), 12u, base, plan, &format12objidx);
      }
      ଅନ୍ୟଥା ଯଦି (format == 14) c->copy (_, it, 14u, base, plan, &format14objidx);
#ifndef HB_NO_BEYOND_64K
      ଅନ୍ୟଥା ଯଦି (format == 15) c->copy (_, it, 15u, base, plan, &format15objidx);
#endif
    }
        unsigned length = c->length ();
        unsigned available = length > cmap::min_size ? length - cmap::min_size : 0;
        c->check_assign(ଏହି->encodingRecord.len,
                        available / EncodingRecord::static_size,
                        HB_SERIALIZE_ERROR_INT_OVERFLOW);

    // Fail if format 4 was dropped and there is no cmap12.
    ଫେରନ୍ତୁ !drop_format_4 || format12objidx;
  }

  template<typename Iterator, typename EncodingRecordIterator,
      hb_requires (hb_is_iterator (Iterator)),
      hb_requires (hb_is_iterator (EncodingRecordIterator))>
  ତାର୍କିକ _can_drop (ଅଚଳ EncodingRecord& cmap12,
                  ଅଚଳ hb_set_t& cmap12_unicodes,
                  ଅଚଳ ଶୂନ୍ୟ* base,
                  ଅଚଳ SubtableUnicodesCache& unicodes_cache,
                  SubtableUnicodesCache& local_unicodes_cache,
                  Iterator subset_unicodes,
                  EncodingRecordIterator encoding_records)
  {
    ପାଇଁ (auto cp : + subset_unicodes | hb_filter (cmap12_unicodes))
    {
      ଯଦି (cp >= 0x10000) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }

    unsigned target_platform;
    unsigned target_encoding;
    unsigned target_language = (base+cmap12.subtable).get_language ();

    ଯଦି (cmap12.platformID == 0 && cmap12.encodingID == 4)
    {
      target_platform = 0;
      target_encoding = 3;
    } ଅନ୍ୟଥା ଯଦି (cmap12.platformID == 3 && cmap12.encodingID == 10) {
      target_platform = 3;
      target_encoding = 1;
    } ଅନ୍ୟଥା {
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }

    ପାଇଁ (ଅଚଳ auto& _ : encoding_records)
    {
      ଯଦି (_.platformID != target_platform
          || _.encodingID != target_encoding
          || (base+_.subtable).get_language() != target_language)
        ଜାରି;

      ଅଚଳ hb_set_t* sibling_unicodes = unicodes_cache.set_for (&_, local_unicodes_cache);

      auto cmap12 = + subset_unicodes | hb_filter (cmap12_unicodes);
      auto sibling = + subset_unicodes | hb_filter (*sibling_unicodes);
      ପାଇଁ (; cmap12 && sibling; cmap12++, sibling++)
      {
        unsigned a = *cmap12;
        unsigned b = *sibling;
        ଯଦି (a != b) ଫେରନ୍ତୁ ମିଥ୍ୟା;
      }

      ଫେରନ୍ତୁ !cmap12 && !sibling;
    }

    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }

  ଶୂନ୍ୟ closure_glyphs (ଅଚଳ hb_set_t      *unicodes,
		       hb_set_t            *glyphset) ଅଚଳ
  {
    + hb_iter (encodingRecord)
    | hb_map (&EncodingRecord::subtable)
    | hb_map (hb_add (ଏହି))
    | hb_filter (&CmapSubtable::is_variation_selector)
    | hb_apply ([=] (ଅଚଳ CmapSubtable& _) { _.closure_glyphs (unicodes, glyphset); })
    ;
  }

  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  { ଫେରନ୍ତୁ subset (c, ମିଥ୍ୟା); }

  ତାର୍କିକ subset (hb_subset_context_t *c, ତାର୍କିକ is_dmap) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);

    cmap *cmap_ମୌଳିକ = c->serializer->start_embed<cmap> ();

    auto encodingrec_iter =
    + hb_iter (encodingRecord)
    | hb_filter ([&](ଅଚଳ EncodingRecord& _) {
      ଫେରନ୍ତୁ cmap::filter_encoding_records_for_subset (ଏହି, _);
    })
    ;

    ଯଦି (unlikely (!encodingrec_iter.len ())) return_trace (ମିଥ୍ୟା);

    ଅଚଳ EncodingRecord *unicode_bmp= nullptr, *unicode_ucs4 = nullptr, *ms_bmp = nullptr, *ms_ucs4 = nullptr;
    ତାର୍କିକ has_format12 = ମିଥ୍ୟା;

    ପାଇଁ (ଅଚଳ EncodingRecord& _ : encodingrec_iter)
    {
      unsigned format = (ଏହି + _.subtable).u.format.v;
      ଯଦି (format == 12) has_format12 = ସତ୍ୟ;

      ଅଚଳ EncodingRecord *table = std::addressof (_);
      ଯଦି      (_.platformID == 0 && _.encodingID ==  3) unicode_bmp = table;
      ଅନ୍ୟଥା ଯଦି (_.platformID == 0 && _.encodingID ==  4) unicode_ucs4 = table;
      ଅନ୍ୟଥା ଯଦି (_.platformID == 3 && _.encodingID ==  1) ms_bmp = table;
      ଅନ୍ୟଥା ଯଦି (_.platformID == 3 && _.encodingID == 10) ms_ucs4 = table;
    }

    ଯଦି (unlikely (!is_dmap && !has_format12 && !unicode_bmp && !ms_bmp)) return_trace (ମିଥ୍ୟା);
    ଯଦି (unlikely (!is_dmap && has_format12 && (!unicode_ucs4 && !ms_ucs4))) return_trace (ମିଥ୍ୟା);

    auto it =
    + c->plan->unicode_to_new_gid_list.iter ()
    | hb_filter ([&] (ଅଚଳ hb_codepoint_pair_t _)
		 { ଫେରନ୍ତୁ (_.second != HB_MAP_ମୂଲ୍ୟ_INVALID); })
    ;

    return_trace (cmap_ମୌଳିକ->serialize (c->serializer,
                                         it,
                                         encodingrec_iter,
                                         ଏହି,
                                         c->plan,
					 ମିଥ୍ୟା,
					 c->source_blob->length));
  }

  ଅଚଳ CmapSubtable *find_best_subtable (ତାର୍କିକ *ପ୍ରତୀକ = nullptr,
					  ତାର୍କିକ *mac = nullptr,
					  ତାର୍କିକ *macroman = nullptr) ଅଚଳ
  {
    ଯଦି (ପ୍ରତୀକ) *ପ୍ରତୀକ = ମିଥ୍ୟା;
    ଯଦି (mac) *mac = ମିଥ୍ୟା;
    ଯଦି (macroman) *macroman = ମିଥ୍ୟା;

    ଅଚଳ CmapSubtable *subtable;

    /* Symbol subtable.
     * Prefer symbol if available.
     * https://github.com/harfbuzz/harfbuzz/issues/1918 */
    ଯଦି ((subtable = ଏହି->find_subtable (3, 0)))
    {
      ଯଦି (ପ୍ରତୀକ) *ପ୍ରତୀକ = ସତ୍ୟ;
      ଫେରନ୍ତୁ subtable;
    }

    /* 32-bit subtables. */
    ଯଦି ((subtable = ଏହି->find_subtable (3, 10))) ଫେରନ୍ତୁ subtable;
    ଯଦି ((subtable = ଏହି->find_subtable (0, 6))) ଫେରନ୍ତୁ subtable;
    ଯଦି ((subtable = ଏହି->find_subtable (0, 4))) ଫେରନ୍ତୁ subtable;

    /* 16-bit subtables. */
    ଯଦି ((subtable = ଏହି->find_subtable (3, 1))) ଫେରନ୍ତୁ subtable;
    ଯଦି ((subtable = ଏହି->find_subtable (0, 3))) ଫେରନ୍ତୁ subtable;
    ଯଦି ((subtable = ଏହି->find_subtable (0, 2))) ଫେରନ୍ତୁ subtable;
    ଯଦି ((subtable = ଏହି->find_subtable (0, 1))) ଫେରନ୍ତୁ subtable;
    ଯଦି ((subtable = ଏହି->find_subtable (0, 0))) ଫେରନ୍ତୁ subtable;

    /* MacRoman subtable. */
    ଯଦି ((subtable = ଏହି->find_subtable (1, 0)))
    {
      ଯଦି (mac) *mac = ସତ୍ୟ;
      ଯଦି (macroman) *macroman = ସତ୍ୟ;
      ଫେରନ୍ତୁ subtable;
    }
    /* Any other Mac subtable; we just map ASCII for these. */
    ଯଦି ((subtable = ଏହି->find_subtable (1, 0xFFFF)))
    {
      ଯଦି (mac) *mac = ସତ୍ୟ;
      ଫେରନ୍ତୁ subtable;
    }

    /* Meh. */
    ଫେରନ୍ତୁ &Null (CmapSubtable);
  }

  ସଂରଚନା accelerator_t
  {
    using cache_t = hb_cache_t<21, 19>;
    static_assert (ଆକାର (cache_t) == 1024, "");

    accelerator_t (hb_face_t *face, hb_tag_t table_tag = cmap::tableTag)
    {
      ଏହି->table = hb_sanitize_context_t ().reference_table<cmap> (face, table_tag);
      ଯଦି (!ଏହି->table.get_length ())
	ଫେରନ୍ତୁ;

      ତାର୍କିକ ପ୍ରତୀକ, mac, macroman;
      ଏହି->subtable = table->find_best_subtable (&ପ୍ରତୀକ, &mac, &macroman);
      ଏହି->subtable_uvs = &Null (CmapSubtable);
      {
	ଅଚଳ CmapSubtable *st = table->find_variation_selector_subtable ();
	ଯଦି (st)
	  subtable_uvs = st;
      }

#ifndef HB_NO_OT_FONT_CMAP_CACHE
      cache = (cache_t *) hb_malloc (ଆକାର (cache_t));
      ଯଦି (cache)
	ନୂତନ (cache) cache_t ();
      ଅନ୍ୟଥା
        ଫେରନ୍ତୁ; // Such that get_glyph_funcZ remains null.
#endif

      ଏହି->get_glyph_data = subtable;
#ifndef HB_NO_CMAP_LEGACY_SUBTABLES
      ତାର୍କିକ is_format4 = subtable->u.format.v == 4;
      auto set_format4_getter = [ଏହି] (ତାର୍କିକ (*func) (ଅଚଳ ଶୂନ୍ୟ *,
						      hb_codepoint_t,
						      hb_codepoint_t *))
      {
	ଏହି->format4_accel.init (&ଏହି->subtable->u.format4,
				  get_subtable_data_size (ଏହି->subtable));
	ଏହି->get_glyph_data = &ଏହି->format4_accel;
	ଏହି->get_glyph_funcZ = func;
      };
      ଯଦି (unlikely (ପ୍ରତୀକ))
      {
	ବାଛ ((unsigned) face->table.OS2->get_font_page ()) {
	ସ୍ଥିତି OS2::font_page_t::FONT_PAGE_NONE:
	  ଯଦି (is_format4)
	    set_format4_getter (get_glyph_from_ପ୍ରତୀକ<CmapSubtableFormat4::accelerator_t, hb_ପ୍ରତୀକ_pua_map>);
	  ଅନ୍ୟଥା
	    ଏହି->get_glyph_funcZ = get_glyph_from_ପ୍ରତୀକ<CmapSubtable, hb_ପ୍ରତୀକ_pua_map>;
	  ଭାଙ୍ଗ;
#ifndef HB_NO_OT_SHAPER_ARABIC_FALLBACK
	ସ୍ଥିତି OS2::font_page_t::FONT_PAGE_SIMP_ARABIC:
	  ଯଦି (is_format4)
	    set_format4_getter (get_glyph_from_ପ୍ରତୀକ<CmapSubtableFormat4::accelerator_t, _hb_arabic_pua_simp_map>);
	  ଅନ୍ୟଥା
	    ଏହି->get_glyph_funcZ = get_glyph_from_ପ୍ରତୀକ<CmapSubtable, _hb_arabic_pua_simp_map>;
	  ଭାଙ୍ଗ;
	ସ୍ଥିତି OS2::font_page_t::FONT_PAGE_TRAD_ARABIC:
	  ଯଦି (is_format4)
	    set_format4_getter (get_glyph_from_ପ୍ରତୀକ<CmapSubtableFormat4::accelerator_t, _hb_arabic_pua_trad_map>);
	  ଅନ୍ୟଥା
	    ଏହି->get_glyph_funcZ = get_glyph_from_ପ୍ରତୀକ<CmapSubtable, _hb_arabic_pua_trad_map>;
	  ଭାଙ୍ଗ;
#endif
	ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
	  ଯଦି (is_format4)
	    set_format4_getter (get_glyph_from<CmapSubtableFormat4::accelerator_t>);
	  ଅନ୍ୟଥା
	    ଏହି->get_glyph_funcZ = get_glyph_from<CmapSubtable>;
	  ଭାଙ୍ଗ;
	}
      }
      ଅନ୍ୟଥା ଯଦି (unlikely (macroman))
      {
	ଯଦି (is_format4)
	  set_format4_getter (get_glyph_from_macroman<CmapSubtableFormat4::accelerator_t>);
	ଅନ୍ୟଥା
	  ଏହି->get_glyph_funcZ = get_glyph_from_macroman<CmapSubtable>;
      }
      ଅନ୍ୟଥା ଯଦି (unlikely (mac))
      {
	ଯଦି (is_format4)
	  set_format4_getter (get_glyph_from_ascii<CmapSubtableFormat4::accelerator_t>);
	ଅନ୍ୟଥା
	  ଏହି->get_glyph_funcZ = get_glyph_from_ascii<CmapSubtable>;
      }
      ଅନ୍ୟଥା
#endif
      {
	ବାଛ (subtable->u.format.v) {
	  /* Accelerate format 4 and format 12. */
	  ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
	    ଏହି->get_glyph_funcZ = get_glyph_from<CmapSubtable>;
	    ଭାଙ୍ଗ;
	  ସ୍ଥିତି 12:
	    ଏହି->get_glyph_funcZ = get_glyph_from<CmapSubtableFormat12>;
	    ଭାଙ୍ଗ;
	  ସ୍ଥିତି  4:
	  {
	    hb_barrier ();
	    ଏହି->format4_accel.init (&subtable->u.format4,
				      get_subtable_data_size (subtable));
	    ଏହି->get_glyph_data = &ଏହି->format4_accel;
	    ଏହି->get_glyph_funcZ = ଏହି->format4_accel.get_glyph_func;
	    ଭାଙ୍ଗ;
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

    ଧାଡ଼ିଭିତରେ ତାର୍କିକ _cached_get (hb_codepoint_t unicode,
			     hb_codepoint_t *glyph) ଅଚଳ
    {
#ifndef HB_NO_OT_FONT_CMAP_CACHE
      // cache is always non-null if we have a get_glyph_funcZ
      unsigned v;
      ଯଦି (cache->get (unicode, &v))
      {
        *glyph = v;
	ଫେରନ୍ତୁ ସତ୍ୟ;
      }
#endif
      ତାର୍କିକ ret  = ଏହି->get_glyph_funcZ (ଏହି->get_glyph_data, unicode, glyph);

#ifndef HB_NO_OT_FONT_CMAP_CACHE
      ଯଦି (ret)
        cache->set (unicode, *glyph);
#endif

      ଫେରନ୍ତୁ ret;
    }

    ତାର୍କିକ get_nominal_glyph (hb_codepoint_t  unicode,
			    hb_codepoint_t *glyph) ଅଚଳ
    {
      ଯଦି (unlikely (!ଏହି->get_glyph_funcZ)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
      ଫେରନ୍ତୁ _cached_get (unicode, glyph);
    }

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_nominal_glyphs (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2,
				     ଅଚଳ hb_codepoint_t *first_unicode,
				     unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା unicode_stride,
				     hb_codepoint_t *first_glyph,
				     unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା glyph_stride) ଅଚଳ
    {
      ଯଦି (unlikely (!ଏହି->get_glyph_funcZ)) ଫେରନ୍ତୁ 0;

      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା done;
      ପାଇଁ (done = 0;
	   done < ଗଣନା_2 && _cached_get (*first_unicode, first_glyph);
	   done++)
      {
	first_unicode = &StructAtOffsetUnaligned<hb_codepoint_t> (first_unicode, unicode_stride);
	first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      }
      ଫେରନ୍ତୁ done;
    }

    ତାର୍କିକ get_variation_glyph (hb_codepoint_t  unicode,
			      hb_codepoint_t  variation_selector,
			      hb_codepoint_t *glyph) ଅଚଳ
    {
      ବାଛ (get_glyph_variant (unicode, variation_selector, glyph))
      {
	ସ୍ଥିତି GLYPH_VARIANT_NOT_FOUND:	ଫେରନ୍ତୁ ମିଥ୍ୟା;
	ସ୍ଥିତି GLYPH_VARIANT_FOUND:	ଫେରନ୍ତୁ ସତ୍ୟ;
	ସ୍ଥିତି GLYPH_VARIANT_USE_DEFAULT:	ଭାଙ୍ଗ;
      }

      ଫେରନ୍ତୁ get_nominal_glyph (unicode, glyph);
    }

    glyph_variant_t get_glyph_variant (hb_codepoint_t  unicode,
				       hb_codepoint_t  variation_selector,
				       hb_codepoint_t *glyph) ଅଚଳ
    {
      ଫେରନ୍ତୁ ଏହି->subtable_uvs->get_glyph_variant (unicode,
						    variation_selector,
						    glyph);
    }

    ତାର୍କିକ has_data () ଅଚଳ { ଫେରନ୍ତୁ table.get_length (); }

    ଶୂନ୍ୟ collect_unicodes (hb_set_t *out, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_glyphs) ଅଚଳ
    { subtable->collect_unicodes (out, ଅଙ୍କ_glyphs, get_subtable_data_size (subtable)); }
    ଶୂନ୍ୟ collect_mapping (hb_set_t *unicodes, hb_map_t *mapping,
			  unsigned ଅଙ୍କ_glyphs = UINT_MAX) ଅଚଳ
    { subtable->collect_mapping (unicodes, mapping, ଅଙ୍କ_glyphs, get_subtable_data_size (subtable)); }
    ଶୂନ୍ୟ collect_variation_selectors (hb_set_t *out) ଅଚଳ
    { subtable_uvs->collect_variation_selectors (out); }
    ଶୂନ୍ୟ collect_variation_unicodes (hb_codepoint_t variation_selector,
				     hb_set_t *out) ଅଚଳ
    { subtable_uvs->collect_variation_unicodes (variation_selector, out); }

    ସୁରକ୍ଷିତ:
    ପ୍ରକାରବ୍ୟାଖ୍ୟା ତାର୍କିକ (*hb_cmap_get_glyph_func_t) (ଅଚଳ ଶୂନ୍ୟ *obj,
					      hb_codepoint_t codepoint,
					      hb_codepoint_t *glyph);
    ପ୍ରକାରବ୍ୟାଖ୍ୟା uint16_t (*hb_pua_remap_func_t) (unsigned);

    template <typename Type>
    HB_INTERNAL ସ୍ଥିର ତାର୍କିକ get_glyph_from (ଅଚଳ ଶୂନ୍ୟ *obj,
					    hb_codepoint_t codepoint,
					    hb_codepoint_t *glyph)
    {
      ଅଚଳ Type *typed_obj = (ଅଚଳ Type *) obj;
      ଫେରନ୍ତୁ typed_obj->get_glyph (codepoint, glyph);
    }

    template <typename Type, hb_pua_remap_func_t remap>
    HB_INTERNAL ସ୍ଥିର ତାର୍କିକ get_glyph_from_ପ୍ରତୀକ (ଅଚଳ ଶୂନ୍ୟ *obj,
						   hb_codepoint_t codepoint,
						   hb_codepoint_t *glyph)
    {
      ଅଚଳ Type *typed_obj = (ଅଚଳ Type *) obj;
      ଯଦି (likely (typed_obj->get_glyph (codepoint, glyph)))
	ଫେରନ୍ତୁ ସତ୍ୟ;

      ଯଦି (hb_codepoint_t c = remap (codepoint))
	ଫେରନ୍ତୁ typed_obj->get_glyph (c, glyph);

      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }

    template <typename Type>
    HB_INTERNAL ସ୍ଥିର ତାର୍କିକ get_glyph_from_ascii (ଅଚଳ ଶୂନ୍ୟ *obj,
						  hb_codepoint_t codepoint,
						  hb_codepoint_t *glyph)
    {
      ଅଚଳ Type *typed_obj = (ଅଚଳ Type *) obj;
      ଫେରନ୍ତୁ codepoint < 0x80 && typed_obj->get_glyph (codepoint, glyph);
    }

    template <typename Type>
    HB_INTERNAL ସ୍ଥିର ତାର୍କିକ get_glyph_from_macroman (ଅଚଳ ଶୂନ୍ୟ *obj,
						     hb_codepoint_t codepoint,
						     hb_codepoint_t *glyph)
    {
      ଯଦି (get_glyph_from_ascii<Type> (obj, codepoint, glyph))
	ଫେରନ୍ତୁ ସତ୍ୟ;

      ଅଚଳ Type *typed_obj = (ଅଚଳ Type *) obj;
      unsigned c = unicode_to_macroman (codepoint);
      ଫେରନ୍ତୁ c && typed_obj->get_glyph (c, glyph);
    }

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_subtable_data_size (ଅଚଳ CmapSubtable *subtable) ଅଚଳ
    {
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା table_length = ଏହି->table.get_length ();
      uintptr_t table_start = (uintptr_t) (ଅଚଳ ଶୂନ୍ୟ *) ଏହି->table.get ();
      uintptr_t subtable_addr = (uintptr_t) (ଅଚଳ ଶୂନ୍ୟ *) subtable;
      ଯଦି (unlikely (subtable_addr < table_start))
	ଫେରନ୍ତୁ 0;

      uintptr_t subtable_offset = subtable_addr - table_start;
      ଯଦି (unlikely (subtable_offset >= table_length))
	ଫେରନ୍ତୁ 0;

      ଫେରନ୍ତୁ table_length - (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) subtable_offset;
    }

    ବ୍ୟକ୍ତିଗତ:
    hb_nonnull_ptr_t<ଅଚଳ CmapSubtable> subtable;
    hb_nonnull_ptr_t<ଅଚଳ CmapSubtable> subtable_uvs;

    hb_cmap_get_glyph_func_t get_glyph_funcZ = nullptr;
    ଅଚଳ ଶୂନ୍ୟ *get_glyph_data = nullptr;

    CmapSubtableFormat4::accelerator_t format4_accel;

#ifndef HB_NO_OT_FONT_CMAP_CACHE
    cache_t *cache = nullptr;
#endif

    ସର୍ବସାଧାରଣ:
    hb_blob_ptr_t<cmap> table;
  };

  ସୁରକ୍ଷିତ:

  ଅଚଳ CmapSubtable *find_subtable (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା platform_id,
				     unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା encoding_id) ଅଚଳ
  {
    EncodingRecord key;
    key.platformID = platform_id;
    key.encodingID = encoding_id;

    ଅଚଳ EncodingRecord &result = encodingRecord.bsearch (key);
    ଯଦି (!result.subtable)
      ଫେରନ୍ତୁ nullptr;

    ଫେରନ୍ତୁ &(ଏହି+result.subtable);
  }

  ଅଚଳ CmapSubtable *find_variation_selector_subtable () ଅଚଳ
  {
    ଅଚଳ CmapSubtable *format14 = nullptr;
    ପାଇଁ (ଅଚଳ EncodingRecord& record : encodingRecord)
    {
      ଯଦି (record.platformID != 0 || record.encodingID != 5)
	ଜାରି;

      ଅଚଳ CmapSubtable *subtable = &(ଏହି+record.subtable);
#ifndef HB_NO_BEYOND_64K
      ଯଦି (subtable->u.format.v == 15)
	ଫେରନ୍ତୁ subtable;
#endif
      ଯଦି (subtable->u.format.v == 14)
	format14 = subtable;
    }
    ଫେରନ୍ତୁ format14;
  }

  ସର୍ବସାଧାରଣ:

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) &&
		  hb_barrier () &&
		  likely (version == 0) &&
		  encodingRecord.sanitize (c, ଏହି));
  }

 ବ୍ୟକ୍ତିଗତ:

  ସ୍ଥିର ତାର୍କିକ filter_encoding_records_for_subset(ଅଚଳ cmap* cmap,
                                                 ଅଚଳ EncodingRecord& _)
  {
    ଫେରନ୍ତୁ
        (_.platformID == 0 && _.encodingID == 3) ||
        (_.platformID == 0 && _.encodingID == 4) ||
        (_.platformID == 3 && _.encodingID == 1) ||
        (_.platformID == 3 && _.encodingID == 10) ||
        (cmap + _.subtable).is_variation_selector ();
  }

  ସୁରକ୍ଷିତ:
  HBUINT16	version;	/* Table version number (0). */
  SortedArray16Of<EncodingRecord>
		encodingRecord;	/* Encoding tables. */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (4, encodingRecord);
};

ସଂରଚନା DMAP : cmap
{
  ସ୍ଥିର constexpr hb_tag_t tableTag = HB_OT_TAG_DMAP;

  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  { ଫେରନ୍ତୁ cmap::subset (c, ସତ୍ୟ); }
};

ସଂରଚନା cmap_accelerator_t
{
  cmap_accelerator_t (hb_face_t *face) :
    dmap (face, HB_OT_TAG_DMAP),
    base (face)
  {}

  ତାର୍କିକ get_nominal_glyph (hb_codepoint_t  unicode,
			  hb_codepoint_t *glyph) ଅଚଳ
  {
    ଫେରନ୍ତୁ dmap.get_nominal_glyph (unicode, glyph) ||
	   base.get_nominal_glyph (unicode, glyph);
  }

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_nominal_glyphs (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2,
				   ଅଚଳ hb_codepoint_t *first_unicode,
				   unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା unicode_stride,
				   hb_codepoint_t *first_glyph,
				   unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା glyph_stride) ଅଚଳ
  {
    ଯଦି (likely (!dmap.has_data ()))
      ଫେରନ୍ତୁ base.get_nominal_glyphs (ଗଣନା_2,
				      first_unicode, unicode_stride,
				      first_glyph, glyph_stride);

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା done;
    ପାଇଁ (done = 0;
	 done < ଗଣନା_2 && get_nominal_glyph (*first_unicode, first_glyph);
	 done++)
    {
      first_unicode = &StructAtOffsetUnaligned<hb_codepoint_t> (first_unicode, unicode_stride);
      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
    }
    ଫେରନ୍ତୁ done;
  }

  ତାର୍କିକ get_variation_glyph (hb_codepoint_t  unicode,
			    hb_codepoint_t  variation_selector,
			    hb_codepoint_t *glyph) ଅଚଳ
  {
    glyph_variant_t result = dmap.get_glyph_variant (unicode,
						     variation_selector,
						     glyph);
    ଯଦି (result == GLYPH_VARIANT_NOT_FOUND)
      result = base.get_glyph_variant (unicode, variation_selector, glyph);

    ବାଛ (result)
    {
      ସ୍ଥିତି GLYPH_VARIANT_NOT_FOUND: ଫେରନ୍ତୁ ମିଥ୍ୟା;
      ସ୍ଥିତି GLYPH_VARIANT_FOUND: ଫେରନ୍ତୁ ସତ୍ୟ;
      ସ୍ଥିତି GLYPH_VARIANT_USE_DEFAULT: ଭାଙ୍ଗ;
    }

    ଫେରନ୍ତୁ get_nominal_glyph (unicode, glyph);
  }

  ଶୂନ୍ୟ collect_unicodes (hb_set_t *out, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_glyphs) ଅଚଳ
  {
    base.collect_unicodes (out, ଅଙ୍କ_glyphs);
    dmap.collect_unicodes (out, ଅଙ୍କ_glyphs);
  }

  ଶୂନ୍ୟ collect_mapping (hb_set_t *unicodes, hb_map_t *mapping,
			unsigned ଅଙ୍କ_glyphs = UINT_MAX) ଅଚଳ
  {
    base.collect_mapping (unicodes, mapping, ଅଙ୍କ_glyphs);
    dmap.collect_mapping (unicodes, mapping, ଅଙ୍କ_glyphs);
  }

  ଶୂନ୍ୟ collect_variation_selectors (hb_set_t *out) ଅଚଳ
  {
    base.collect_variation_selectors (out);
    dmap.collect_variation_selectors (out);
  }

  ଶୂନ୍ୟ collect_variation_unicodes (hb_codepoint_t variation_selector,
				   hb_set_t *out) ଅଚଳ
  {
    base.collect_variation_unicodes (variation_selector, out);
    dmap.collect_variation_unicodes (variation_selector, out);
  }

  ଶୂନ୍ୟ closure_glyphs (ଅଚଳ hb_set_t *unicodes,
		       hb_set_t       *glyphset) ଅଚଳ
  {
    ଯଦି (likely (!dmap.has_data ()))
    {
      base.table->closure_glyphs (unicodes, glyphset);
      ଫେରନ୍ତୁ;
    }

    hb_set_t variation_selectors;
    collect_variation_selectors (&variation_selectors);
    ପାଇଁ (hb_codepoint_t variation_selector : variation_selectors)
    {
      ଯଦି (!unicodes->has (variation_selector))
	ଜାରି;

      hb_set_t variation_unicodes;
      collect_variation_unicodes (variation_selector, &variation_unicodes);
      ପାଇଁ (hb_codepoint_t unicode : variation_unicodes)
      {
	ଯଦି (!unicodes->has (unicode))
	  ଜାରି;

	hb_codepoint_t glyph;
	ଯଦି (get_variation_glyph (unicode, variation_selector, &glyph))
	  glyphset->add (glyph);
      }
    }
  }

  cmap::accelerator_t dmap;
  cmap::accelerator_t base;
};

} /* namespace OT */


#endif /* HB_OT_CMAP_TABLE_HH */
