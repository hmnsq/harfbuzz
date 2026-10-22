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

#অন্তর্ভাব "hb-ot-os2-table.hh"
#অন্তর্ভাব "hb-ot-shaper-arabic-pua.hh"
#অন্তর্ভাব "hb-open-type.hh"
#অন্তর্ভাব "hb-ot-dual.hh"
#অন্তর্ভাব "hb-set.hh"
#অন্তর্ভাব "hb-cache.hh"

/*
 * cmap -- Character to Glyph Index Mapping
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cmap
 */
#define HB_OT_TAG_cmap HB_TAG('c','m','a','p')
#define HB_OT_TAG_DMAP HB_TAG('D','M','A','P')

নামাকাশ OT {

স্থির অন্তঃরেখ uint8_t unicode_to_macroman (hb_codepoint_t u)
{
  স্থির ধ্রুবক কাঠামো unicode_to_macroman_t
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
  auto *c = hb_bsearch (u, mapping, ARRAY_দৈর্ঘ্য (mapping), আকার (mapping[0]),
			_hb_cmp_operator<uint16_t, uint16_t>);
  ফেরত c ? c->macroman : 0;
}

কাঠামো CmapSubtableFormat0
{
  যৌক্তিক get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) ধ্রুবক
  {
    hb_codepoint_t gid = codepoint < 256 ? glyphIdArray[codepoint] : 0;
    যদি (unlikely (!gid))
      ফেরত মিথ্যা;
    *glyph = gid;
    ফেরত সত্য;
  }

  unsigned get_language () ধ্রুবক
  {
    ফেরত language;
  }

  শূন্য collect_unicodes (hb_set_t *out) ধ্রুবক
  {
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < 256; i++)
      যদি (glyphIdArray[i])
	out->add (i);
  }

  শূন্য collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping /* OUT */) ধ্রুবক
  {
    জন্য (unsigned i = 0; i < 256; i++)
      যদি (glyphIdArray[i])
      {
	hb_codepoint_t glyph = glyphIdArray[i];
	unicodes->add (i);
	mapping->set (i, glyph);
      }
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই));
  }

  সুরক্ষিত:
  HBUINT16	format;		/* Format number is set to 0. */
  HBUINT16	দৈর্ঘ্য;		/* Byte length of this subtable. */
  HBUINT16	language;	/* Ignore. */
  HBUINT8	glyphIdArray[256];/* An array that maps character
				 * code to glyph index values. */
  সর্বজনীন:
  DEFINE_আকার_STATIC (6 + 256);
};

কাঠামো CmapSubtableFormat4
{
  template<typename Iterator,
      typename Writer,
	   hb_requires (hb_is_iterator (Iterator))>
  শূন্য to_ranges (Iterator it, Writer& range_writer)
  {
    hb_codepoint_t start_cp = 0, পূর্ব_run_start_cp = 0, run_start_cp = 0, end_cp = 0, last_gid = 0;
    পূর্ণসংখ্যা run_দৈর্ঘ্য = 0 , delta = 0, পূর্ব_delta = 0;

    গণনা {
      FIRST_SUB_RANGE,
      FOLLOWING_SUB_RANGE,
    } mode;

    যখন (it) {
      // Start a new range
      {
        ধ্রুবক auto& pair = *it;
        start_cp = pair.first;
        পূর্ব_run_start_cp = start_cp;
        run_start_cp = start_cp;
        end_cp = start_cp;
        last_gid = pair.second;
        run_দৈর্ঘ্য = 1;
        পূর্ব_delta = 0;
      }

      delta = last_gid - start_cp;
      mode = FIRST_SUB_RANGE;
      it++;

      যখন (it) {
        // Process range
        ধ্রুবক auto& pair = *it;
        hb_codepoint_t পরবর্তী_cp = pair.first;
        hb_codepoint_t পরবর্তী_gid = pair.second;
        যদি (পরবর্তী_cp != end_cp + 1) {
          // Current range is over, stop processing.
          বিরতি;
        }

        যদি (পরবর্তী_gid == last_gid + 1) {
          // The current run continues.
          end_cp = পরবর্তী_cp;
          run_দৈর্ঘ্য++;
          last_gid = পরবর্তী_gid;
          it++;
          চালু;
        }

        // A new run is starting, decide if we want to commit the current run.
        পূর্ণসংখ্যা split_cost = (mode == FIRST_SUB_RANGE) ? 8 : 16;
        পূর্ণসংখ্যা run_cost = run_দৈর্ঘ্য * 2;
        যদি (run_cost >= split_cost) {
          commit_current_range(start_cp,
                               পূর্ব_run_start_cp,
                               run_start_cp,
                               end_cp,
                               delta,
                               পূর্ব_delta,
                               split_cost,
                               range_writer);
          start_cp = পরবর্তী_cp;
        }

        // Start the new run
        mode = FOLLOWING_SUB_RANGE;
        পূর্ব_run_start_cp = run_start_cp;
        run_start_cp = পরবর্তী_cp;
        end_cp = পরবর্তী_cp;
        পূর্ব_delta = delta;
        delta = পরবর্তী_gid - run_start_cp;
        run_দৈর্ঘ্য = 1;
        last_gid = পরবর্তী_gid;
        it++;
      }

      // Finalize range
      commit_current_range (start_cp,
                            পূর্ব_run_start_cp,
                            run_start_cp,
                            end_cp,
                            delta,
                            পূর্ব_delta,
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
                             hb_codepoint_t পূর্ব_run_start,
                             hb_codepoint_t run_start,
                             hb_codepoint_t end,
                             পূর্ণসংখ্যা run_delta,
                             পূর্ণসংখ্যা previous_run_delta,
                             পূর্ণসংখ্যা split_cost,
                             Writer& range_writer) {
    যৌক্তিক should_split = মিথ্যা;
    যদি (start < run_start && run_start < end) {
      পূর্ণসংখ্যা run_cost = (end - run_start + 1) * 2;
      যদি (run_cost >= split_cost) {
        should_split = সত্য;
      }
    }

    // A zero delta is also the marker for ranges that need a
    // glyphIdArray; serialize_rangeoffset_glyid() tells the two cases
    // apart by re-checking the mapping.
    যদি (should_split) {
      যদি (start == পূর্ব_run_start)
        range_writer (start, run_start - 1, previous_run_delta);
      অন্যথা
        range_writer (start, run_start - 1, 0);
      range_writer (run_start, end, run_delta);
      ফেরত;
    }


    যদি (start == run_start) {
      // Range is only a run
      range_writer (start, end, run_delta);
      ফেরত;
    }

    // Write only a single non-run range.
    range_writer (start, end, 0);
  }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  unsigned serialize_find_segcount (Iterator it) {
    কাঠামো গণক {
      unsigned segcount = 0;

      শূন্য operator() (hb_codepoint_t start,
                       hb_codepoint_t end,
                       পূর্ণসংখ্যা delta) {
        segcount++;
      }
    } গণক_2;

    to_ranges (+it, গণক_2);
    ফেরত গণক_2.segcount;
  }


  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  যৌক্তিক serialize_start_end_delta_arrays (hb_serialize_context_t *c,
                                         Iterator it,
                                         পূর্ণসংখ্যা segcount)
  {
    কাঠামো Writer {
      hb_serialize_context_t *serializer_;
      HBUINT16* end_code_;
      HBUINT16* start_code_;
      HBINT16* id_delta_;
      পূর্ণসংখ্যা সূচক_3;

      Writer(hb_serialize_context_t *serializer)
          : serializer_(serializer),
            end_code_(nullptr),
            start_code_(nullptr),
            id_delta_(nullptr),
            সূচক_3 (0) {}
      শূন্য operator() (hb_codepoint_t start,
                       hb_codepoint_t end,
                       পূর্ণসংখ্যা delta) {
        start_code_[সূচক_3] = start;
        end_code_[সূচক_3] = end;
        id_delta_[সূচক_3] = delta;
        সূচক_3++;
      }
    } writer(c);

    writer.end_code_ = c->allocate_আকার<HBUINT16> (HBUINT16::static_আকার * segcount, মিথ্যা);
    (শূন্য) c->allocate_আকার<HBUINT16> (2); // padding
    writer.start_code_ = c->allocate_আকার<HBUINT16> (HBUINT16::static_আকার * segcount, মিথ্যা);
    writer.id_delta_ = c->allocate_আকার<HBINT16> (HBINT16::static_আকার * segcount, মিথ্যা);

    যদি (unlikely (!writer.end_code_ || !writer.start_code_ || !writer.id_delta_)) ফেরত মিথ্যা;

    to_ranges (+it, writer);
    ফেরত সত্য;
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

    HBUINT16 *idRangeOffset = c->allocate_আকার<HBUINT16> (HBUINT16::static_আকার * segcount);
    যদি (unlikely (!c->check_success (idRangeOffset))) ফেরত nullptr;
    যদি (unlikely ((অক্ষর *)idRangeOffset - (অক্ষর *)idDelta != (পূর্ণসংখ্যা) segcount * (পূর্ণসংখ্যা) HBINT16::static_আকার)) ফেরত nullptr;

    জন্য (unsigned i : hb_range (segcount))
    {
      /* A non-zero delta is always a real run. A zero delta is
       * ambiguous: identity mapping, or the marker for a range that
       * needs a glyphIdArray. Only zero deltas need a check. */
      পূর্ণসংখ্যা delta = idDelta[i];
      যদি (delta)
	চালু;
      যৌক্তিক use_delta = সত্য;
      জন্য (hb_codepoint_t cp = startCode[i]; cp <= endCode[i]; cp++)
      {
	hb_codepoint_t gid = cp_to_gid.get (cp);
	যদি (gid == HB_MAP_মান_INVALID)
	  gid = 0;
	যদি (gid != cp)
	{
	  use_delta = মিথ্যা;
	  বিরতি;
	}
      }
      যদি (use_delta)
	চালু;

      idRangeOffset[i] = 2 * (c->start_embed<HBUINT16> () - idRangeOffset - i);
      জন্য (hb_codepoint_t cp = startCode[i]; cp <= endCode[i]; cp++)
      {
        HBUINT16 gid;
        gid = cp_to_gid[cp];
        c->copy<HBUINT16> (gid);
      }
    }

    ফেরত idRangeOffset;
  }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  শূন্য serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    auto format4_iter =
    + it
    | hb_filter ([&] (ধ্রুবক hb_codepoint_pair_t _)
		 { ফেরত _.first <= 0xFFFF; })
    ;

    যদি (!format4_iter) ফেরত;

    unsigned table_initpos = c->দৈর্ঘ্য ();
    যদি (unlikely (!c->extend_min (এই))) ফেরত;
    এই->format = 4;

    hb_vector_t<hb_codepoint_pair_t> cp_to_gid {
      format4_iter
    };

    //serialize endCode[], startCode[], idDelta[]
    HBUINT16* endCode = c->start_embed<HBUINT16> ();
    unsigned segcount = serialize_find_segcount (cp_to_gid.iter());
    যদি (unlikely (!serialize_start_end_delta_arrays (c, cp_to_gid.iter(), segcount)))
      ফেরত;

    HBUINT16 *startCode = endCode + segcount + 1;
    HBINT16 *idDelta = ((HBINT16*)startCode) + segcount;

    HBUINT16 *idRangeOffset = serialize_rangeoffset_glyid (c,
                                                           cp_to_gid.iter (),
                                                           endCode,
                                                           startCode,
                                                           idDelta,
                                                           segcount);
    যদি (unlikely (!c->check_success (idRangeOffset))) ফেরত;

    এই->দৈর্ঘ্য = c->দৈর্ঘ্য () - table_initpos;
    যদি ((long long) এই->দৈর্ঘ্য != (long long) c->দৈর্ঘ্য () - table_initpos)
    {
      c->err (HB_SERIALIZE_ERROR_INT_OVERFLOW);
      ফেরত;
    }

    এই->seg_গুনতি_X2 = segcount * 2;
    এই->entrySelector = hb_max (1u, hb_bit_storage (segcount)) - 1;
    এই->অনুসন্ধান_Range = 2 * (1u << এই->entrySelector);
    এই->rangeShift = segcount * 2 > এই->অনুসন্ধান_Range
		       ? 2 * segcount - এই->অনুসন্ধান_Range
		       : 0;
  }

  unsigned get_language () ধ্রুবক
  {
    ফেরত language;
  }

  কাঠামো accelerator_t
  {
    accelerator_t () {}
    accelerator_t (ধ্রুবক CmapSubtableFormat4 *subtable) = delete;

    শূন্য init (ধ্রুবক CmapSubtableFormat4 *subtable,
	       unsigned পূর্ণসংখ্যা              subtable_তথ্য_আকার)
    {
      seg_গুনতি_2 = subtable->seg_গুনতি_X2 / 2;
      end_গুনতি = subtable->values.arrayZ;
      start_গুনতি = end_গুনতি + seg_গুনতি_2 + 1;
      idDelta = start_গুনতি + seg_গুনতি_2;
      idRangeOffset = idDelta + seg_গুনতি_2;
      glyphIdArray = idRangeOffset + seg_গুনতি_2;

      unsigned পূর্ণসংখ্যা মানসমূহ_offset = 16 + 8 * seg_গুনতি_2;
      glyph_Id_Array_দৈর্ঘ্য = subtable_তথ্য_আকার > মানসমূহ_offset
			   ? (subtable_তথ্য_আকার - মানসমূহ_offset) / 2
			   : 0;
    }

    যৌক্তিক get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) ধ্রুবক
    {
      কাঠামো CustomRange
      {
	পূর্ণসংখ্যা cmp (hb_codepoint_t k,
		 unsigned distance) ধ্রুবক
	{
	  যদি (k > last) ফেরত +1;
	  যদি (k < (&last)[distance]/*first*/) ফেরত -1;
	  ফেরত 0;
	}
	HBUINT16 last;
      };

      ধ্রুবক HBUINT16 *found = hb_bsearch (codepoint,
					  এই->end_গুনতি,
					  এই->seg_গুনতি_2,
					  আকার (CustomRange),
					  _hb_cmp_method<hb_codepoint_t, CustomRange, unsigned>,
					  এই->seg_গুনতি_2 + 1);
      যদি (unlikely (!found))
	ফেরত মিথ্যা;
      unsigned পূর্ণসংখ্যা i = found - end_গুনতি;

      hb_codepoint_t gid;
      unsigned পূর্ণসংখ্যা rangeOffset = এই->idRangeOffset[i];
      যদি (rangeOffset == 0)
	gid = codepoint + এই->idDelta[i];
      অন্যথা
      {
	/* Somebody has been smoking... */
	unsigned পূর্ণসংখ্যা সূচক_2 = rangeOffset / 2 + (codepoint - এই->start_গুনতি[i]) + i - এই->seg_গুনতি_2;
	যদি (unlikely (সূচক_2 >= এই->glyph_Id_Array_দৈর্ঘ্য))
	  ফেরত মিথ্যা;
	gid = এই->glyphIdArray[সূচক_2];
	যদি (unlikely (!gid))
	  ফেরত মিথ্যা;
	gid += এই->idDelta[i];
      }
      gid &= 0xFFFFu;
      যদি (unlikely (!gid))
	ফেরত মিথ্যা;
      *glyph = gid;
      ফেরত সত্য;
    }

    HB_INTERNAL স্থির যৌক্তিক get_glyph_func (ধ্রুবক শূন্য *obj, hb_codepoint_t codepoint, hb_codepoint_t *glyph)
    { ফেরত ((ধ্রুবক accelerator_t *) obj)->get_glyph (codepoint, glyph); }

    শূন্য collect_unicodes (hb_set_t *out) ধ্রুবক
    {
      unsigned পূর্ণসংখ্যা গুনতি = এই->seg_গুনতি_2;
      যদি (গুনতি && এই->start_গুনতি[গুনতি - 1] == 0xFFFFu)
	গুনতি--; /* Skip sentinel segment. */
      জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
      {
	hb_codepoint_t start = এই->start_গুনতি[i];
	hb_codepoint_t end = এই->end_গুনতি[i];
	unsigned পূর্ণসংখ্যা rangeOffset = এই->idRangeOffset[i];
        out->add_range(start, end);
	যদি (rangeOffset == 0)
	{
	  জন্য (hb_codepoint_t codepoint = start; codepoint <= end; codepoint++)
	  {
	    hb_codepoint_t gid = (codepoint + এই->idDelta[i]) & 0xFFFFu;
	    যদি (unlikely (!gid))
              out->del(codepoint);
	  }
	}
	অন্যথা
	{
	  জন্য (hb_codepoint_t codepoint = start; codepoint <= end; codepoint++)
	  {
	    unsigned পূর্ণসংখ্যা সূচক_2 = rangeOffset / 2 + (codepoint - এই->start_গুনতি[i]) + i - এই->seg_গুনতি_2;
	    যদি (unlikely (সূচক_2 >= এই->glyph_Id_Array_দৈর্ঘ্য))
            {
              out->del_range (codepoint, end);
	      বিরতি;
            }
	    hb_codepoint_t gid = এই->glyphIdArray[সূচক_2];
	    যদি (unlikely (!gid))
              out->del(codepoint);
	  }
	}
      }
    }

    শূন্য collect_mapping (hb_set_t *unicodes, /* OUT */
			  hb_map_t *mapping /* OUT */) ধ্রুবক
    {
      // TODO(grieger): optimize similar to collect_unicodes
      // (ie. use add_range())
      unsigned গুনতি = এই->seg_গুনতি_2;
      যদি (গুনতি && এই->start_গুনতি[গুনতি - 1] == 0xFFFFu)
	গুনতি--; /* Skip sentinel segment. */
      জন্য (unsigned i = 0; i < গুনতি; i++)
      {
	hb_codepoint_t start = এই->start_গুনতি[i];
	hb_codepoint_t end = এই->end_গুনতি[i];
	unsigned rangeOffset = এই->idRangeOffset[i];
	যদি (rangeOffset == 0)
	{
	  জন্য (hb_codepoint_t codepoint = start; codepoint <= end; codepoint++)
	  {
	    hb_codepoint_t gid = (codepoint + এই->idDelta[i]) & 0xFFFFu;
	    যদি (unlikely (!gid))
	      চালু;
	    unicodes->add (codepoint);
	    mapping->set (codepoint, gid);
	  }
	}
	অন্যথা
	{
	  জন্য (hb_codepoint_t codepoint = start; codepoint <= end; codepoint++)
	  {
	    unsigned সূচক_2 = rangeOffset / 2 + (codepoint - এই->start_গুনতি[i]) + i - এই->seg_গুনতি_2;
	    যদি (unlikely (সূচক_2 >= এই->glyph_Id_Array_দৈর্ঘ্য))
	      বিরতি;
	    hb_codepoint_t gid = এই->glyphIdArray[সূচক_2];
	    যদি (unlikely (!gid))
	      চালু;
            gid = (gid + এই->idDelta[i]) & 0xFFFFu;
	    unicodes->add (codepoint);
	    mapping->set (codepoint, gid);
	  }
	}
      }
    }

    ধ্রুবক HBUINT16 *end_গুনতি;
    ধ্রুবক HBUINT16 *start_গুনতি;
    ধ্রুবক HBUINT16 *idDelta;
    ধ্রুবক HBUINT16 *idRangeOffset;
    ধ্রুবক HBUINT16 *glyphIdArray;
    unsigned পূর্ণসংখ্যা seg_গুনতি_2;
    unsigned পূর্ণসংখ্যা glyph_Id_Array_দৈর্ঘ্য;
  };

  যৌক্তিক get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph,
		  unsigned পূর্ণসংখ্যা subtable_তথ্য_আকার) ধ্রুবক
  {
    accelerator_t accel;
    accel.init (এই, subtable_তথ্য_আকার);
    ফেরত accel.get_glyph_func (&accel, codepoint, glyph);
  }
  যৌক্তিক get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) ধ্রুবক
  { ফেরত মিথ্যা; }

  শূন্য collect_unicodes (hb_set_t *out, unsigned পূর্ণসংখ্যা subtable_তথ্য_আকার) ধ্রুবক
  {
    accelerator_t accel;
    accel.init (এই, subtable_তথ্য_আকার);
    accel.collect_unicodes (out);
  }
  শূন্য collect_unicodes (hb_set_t *out) ধ্রুবক
  { collect_unicodes (out, দৈর্ঘ্য); }

  শূন্য collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping, /* OUT */
			unsigned পূর্ণসংখ্যা subtable_তথ্য_আকার) ধ্রুবক
  {
    accelerator_t accel;
    accel.init (এই, subtable_তথ্য_আকার);
    accel.collect_mapping (unicodes, mapping);
  }
  শূন্য collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping /* OUT */) ধ্রুবক
  { collect_mapping (unicodes, mapping, দৈর্ঘ্য); }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    যদি (unlikely (!c->check_struct (এই)))
      return_trace (মিথ্যা);
    hb_barrier ();

    return_trace (c->check_range (values, 2 + 4 * seg_গুনতি_X2));
  }

  সুরক্ষিত:
  HBUINT16	format;		/* Format number is set to 4. */
  HBUINT16	দৈর্ঘ্য;		/* This is the length in bytes of the
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

  সর্বজনীন:
  DEFINE_আকার_ARRAY (14, values);
};

কাঠামো CmapSubtableLongGroup
{
  friend কাঠামো CmapSubtableFormat12;
  friend কাঠামো CmapSubtableFormat13;
  template<typename U>
  friend কাঠামো CmapSubtableLongSegmented;
  friend কাঠামো cmap;

  পূর্ণসংখ্যা cmp (hb_codepoint_t codepoint) ধ্রুবক
  {
    যদি (codepoint < startCharCode) ফেরত -1;
    যদি (codepoint > endCharCode)   ফেরত +1;
    ফেরত 0;
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই));
  }

  ব্যক্তিগত:
  HBUINT32		startCharCode;	/* First character code in this group. */
  HBUINT32		endCharCode;	/* Last character code in this group. */
  HBUINT32		glyphID;	/* Glyph index; interpretation depends on
					 * subtable format. */
  সর্বজনীন:
  DEFINE_আকার_STATIC (12);
};
DECLARE_NULL_NAMESPACE_BYTES (OT, CmapSubtableLongGroup);

template <typename UINT>
কাঠামো CmapSubtableTrimmed
{
  যৌক্তিক get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) ধ্রুবক
  {
    /* Rely on our implicit array bound-checking. */
    hb_codepoint_t gid = glyphIdArray[codepoint - startCharCode];
    যদি (unlikely (!gid))
      ফেরত মিথ্যা;
    *glyph = gid;
    ফেরত সত্য;
  }

  unsigned get_language () ধ্রুবক
  {
    ফেরত language;
  }

  শূন্য collect_unicodes (hb_set_t *out) ধ্রুবক
  {
    hb_codepoint_t start = startCharCode;
    unsigned পূর্ণসংখ্যা গুনতি = glyphIdArray.len;
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
      যদি (glyphIdArray[i])
	out->add (start + i);
  }

  শূন্য collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping /* OUT */) ধ্রুবক
  {
    hb_codepoint_t start_cp = startCharCode;
    unsigned গুনতি = glyphIdArray.len;
    জন্য (unsigned i = 0; i < গুনতি; i++)
      যদি (glyphIdArray[i])
      {
	hb_codepoint_t unicode = start_cp + i;
	hb_codepoint_t glyphid = glyphIdArray[i];
	unicodes->add (unicode);
	mapping->set (unicode, glyphid);
      }
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) && glyphIdArray.sanitize (c));
  }

  সুরক্ষিত:
  UINT		formatReserved;	/* Subtable format and (maybe) padding. */
  UINT		দৈর্ঘ্য;		/* Byte length of this subtable. */
  UINT		language;	/* Ignore. */
  UINT		startCharCode;	/* First character code covered. */
  ArrayOf<HBGlyphID16, UINT>
		glyphIdArray;	/* Array of glyph index values for character
				 * codes in the range. */
  সর্বজনীন:
  DEFINE_আকার_ARRAY (5 * আকার (UINT), glyphIdArray);
};

কাঠামো CmapSubtableFormat6  : CmapSubtableTrimmed<HBUINT16> {};
কাঠামো CmapSubtableFormat10 : CmapSubtableTrimmed<HBUINT32> {};

template <typename T>
কাঠামো CmapSubtableLongSegmented
{
  friend কাঠামো cmap;

  যৌক্তিক get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) ধ্রুবক
  {
    hb_codepoint_t gid = T::group_get_glyph (groups.bsearch (codepoint), codepoint);
    যদি (unlikely (!gid))
      ফেরত মিথ্যা;
    *glyph = gid;
    ফেরত সত্য;
  }

  unsigned get_language () ধ্রুবক
  {
    ফেরত language;
  }

  শূন্য collect_unicodes (hb_set_t *out, unsigned পূর্ণসংখ্যা অঙ্ক_glyphs) ধ্রুবক
  {
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < এই->groups.len; i++)
    {
      hb_codepoint_t start = এই->groups[i].startCharCode;
      hb_codepoint_t end = hb_min ((hb_codepoint_t) এই->groups[i].endCharCode,
				   (hb_codepoint_t) HB_UNICODE_MAX);
      hb_codepoint_t gid = এই->groups[i].glyphID;
      যদি (!gid)
      {
	/* Intention is: if (hb_is_same (T, CmapSubtableFormat13)) continue; */
	যদি (! T::group_get_glyph (এই->groups[i], end)) চালু;
	start++;
	gid++;
      }
      যদি (unlikely ((unsigned পূর্ণসংখ্যা) gid >= অঙ্ক_glyphs)) চালু;
      যদি (unlikely ((unsigned পূর্ণসংখ্যা) (gid + end - start) >= অঙ্ক_glyphs))
	end = start + (hb_codepoint_t) অঙ্ক_glyphs - gid;

      out->add_range (start, hb_min (end, 0x10FFFFu));
    }
  }

  শূন্য collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping, /* OUT */
			unsigned অঙ্ক_glyphs) ধ্রুবক
  {
    hb_codepoint_t last_end = 0;
    unsigned গুনতি = এই->groups.len;
    জন্য (unsigned i = 0; i < গুনতি; i++)
    {
      hb_codepoint_t start = এই->groups.arrayZ[i].startCharCode;
      hb_codepoint_t end = hb_min ((hb_codepoint_t) এই->groups.arrayZ[i].endCharCode,
				   (hb_codepoint_t) HB_UNICODE_MAX);
      যদি (unlikely (start > end || start < last_end)) {
        // Range is not in order and is invalid, skip it.
        চালু;
      }
      last_end = end;


      hb_codepoint_t gid = এই->groups.arrayZ[i].glyphID;
      যদি (!gid)
      {
        যদি (T::format_সংখ্যা == 13) চালু;
	start++;
	gid++;
      }
      যদি (unlikely ((unsigned পূর্ণসংখ্যা) gid >= অঙ্ক_glyphs)) চালু;
      যদি (unlikely ((unsigned পূর্ণসংখ্যা) (gid + end - start) >= অঙ্ক_glyphs))
	end = start + (hb_codepoint_t) অঙ্ক_glyphs - gid;

      mapping->alloc (mapping->get_population () + end - start + 1);

      unicodes->add_range (start, end);
      জন্য (unsigned cp = start; cp <= end; cp++)
      {
	mapping->set (cp, gid);
        gid += T::increment;
      }
    }
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) && groups.sanitize (c));
  }

  সুরক্ষিত:
  HBUINT16	format;		/* Subtable format; set to 12. */
  HBUINT16	reserved;	/* Reserved; set to 0. */
  HBUINT32	দৈর্ঘ্য;		/* Byte length of this subtable. */
  HBUINT32	language;	/* Ignore. */
  SortedArray32Of<CmapSubtableLongGroup>
		groups;		/* Groupings. */
  সর্বজনীন:
  DEFINE_আকার_ARRAY (16, groups);
};

কাঠামো CmapSubtableFormat12 : CmapSubtableLongSegmented<CmapSubtableFormat12>
{
  স্থির constexpr পূর্ণসংখ্যা increment = 1;
  স্থির constexpr পূর্ণসংখ্যা format_সংখ্যা = 12;

  স্থির hb_codepoint_t group_get_glyph (ধ্রুবক CmapSubtableLongGroup &group,
					 hb_codepoint_t u)
  { ফেরত likely (group.startCharCode <= group.endCharCode) ?
	   group.glyphID + (u - group.startCharCode) : 0; }


  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  শূন্য serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    যদি (!it) ফেরত;
    unsigned table_initpos = c->দৈর্ঘ্য ();
    যদি (unlikely (!c->extend_min (এই))) ফেরত;

    hb_codepoint_t startCharCode = (hb_codepoint_t) -1, endCharCode = (hb_codepoint_t) -1;
    hb_codepoint_t glyphID = 0;

    জন্য (ধ্রুবক auto& _ : +it)
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

    এই->format = 12;
    এই->reserved = 0;
    এই->দৈর্ঘ্য = c->দৈর্ঘ্য () - table_initpos;
    এই->groups.len = (এই->দৈর্ঘ্য - min_আকার) / CmapSubtableLongGroup::static_আকার;
  }

  স্থির size_t get_sub_table_আকার (ধ্রুবক hb_sorted_vector_t<CmapSubtableLongGroup> &groups_তথ্য)
  { ফেরত 16 + 12 * groups_তথ্য.দৈর্ঘ্য; }

  ব্যক্তিগত:
  স্থির যৌক্তিক _is_gid_consecutive (hb_codepoint_t endCharCode,
				   hb_codepoint_t startCharCode,
				   hb_codepoint_t glyphID,
				   hb_codepoint_t cp,
				   hb_codepoint_t new_gid)
  {
    ফেরত (cp - 1 == endCharCode) &&
	new_gid == glyphID + (cp - startCharCode);
  }

};

কাঠামো CmapSubtableFormat13 : CmapSubtableLongSegmented<CmapSubtableFormat13>
{
  স্থির constexpr পূর্ণসংখ্যা increment = 0;
  স্থির constexpr পূর্ণসংখ্যা format_সংখ্যা = 13;

  স্থির hb_codepoint_t group_get_glyph (ধ্রুবক CmapSubtableLongGroup &group,
					 hb_codepoint_t u HB_UNUSED)
  { ফেরত group.glyphID; }
};

প্রকারলেখ গণনা
{
  GLYPH_VARIANT_NOT_FOUND = 0,
  GLYPH_VARIANT_FOUND = 1,
  GLYPH_VARIANT_USE_DEFAULT = 2
} glyph_variant_t;

কাঠামো Unicode_মান_Range
{
  পূর্ণসংখ্যা cmp (ধ্রুবক hb_codepoint_t &codepoint) ধ্রুবক
  {
    যদি (codepoint < start_Unicode_মান) ফেরত -1;
    যদি (codepoint > start_Unicode_মান + additional_গুনতি) ফেরত +1;
    ফেরত 0;
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই));
  }

  HBUINT24	start_Unicode_মান;	/* First value in this range. */
  HBUINT8	additional_গুনতি;	/* Number of additional values in this
					 * range. */
  সর্বজনীন:
  DEFINE_আকার_STATIC (4);
};

কাঠামো DefaultUVS : SortedArray32Of<Unicode_মান_Range>
{
  শূন্য collect_unicodes (hb_set_t *out) ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা গুনতি = len;
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
    {
      hb_codepoint_t first = arrayZ[i].start_Unicode_মান;
      hb_codepoint_t last = hb_min ((hb_codepoint_t) (first + arrayZ[i].additional_গুনতি),
				    (hb_codepoint_t) HB_UNICODE_MAX);
      out->add_range (first, last);
    }
  }

  DefaultUVS* copy (hb_serialize_context_t *c,
		    ধ্রুবক hb_set_t *unicodes) ধ্রুবক
  {
    auto *out = c->start_embed<DefaultUVS> ();
    auto snap = c->snapshot ();

    HBUINT32 len;
    len = 0;
    যদি (unlikely (!c->copy<HBUINT32> (len))) ফেরত nullptr;
    unsigned init_len = c->দৈর্ঘ্য ();

    যদি (এই->len > unicodes->get_population () * hb_bit_storage ((unsigned) এই->len))
    {
      hb_codepoint_t start = HB_SET_মান_INVALID;
      hb_codepoint_t end = HB_SET_মান_INVALID;

      জন্য (auto u : *unicodes)
      {
        যদি (!as_array ().bsearch (u))
	  চালু;
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
      পূর্ণসংখ্যা গুনতি = -1;

      জন্য (ধ্রুবক Unicode_মান_Range& _ : *এই)
      {
	hb_codepoint_t curEntry = (hb_codepoint_t) (_.start_Unicode_মান - 1);
	hb_codepoint_t end = curEntry + _.additional_গুনতি + 2;

	জন্য (; unicodes->next (&curEntry) && curEntry < end;)
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

    যদি (c->দৈর্ঘ্য () - init_len == 0)
    {
      c->revert (snap);
      ফেরত nullptr;
    }
    অন্যথা
    {
      যদি (unlikely (!c->check_assign (out->len,
                                      (c->দৈর্ঘ্য () - init_len) / Unicode_মান_Range::static_আকার,
                                      HB_SERIALIZE_ERROR_INT_OVERFLOW))) ফেরত nullptr;
      ফেরত out;
    }
  }

  সর্বজনীন:
  DEFINE_আকার_ARRAY (4, *এই);
};

template <typename Types>
কাঠামো UVSMapping
{
  using GlyphID = typename Types::HBGlyphID;

  পূর্ণসংখ্যা cmp (ধ্রুবক hb_codepoint_t &codepoint) ধ্রুবক
  { ফেরত unicode_মান.cmp (codepoint); }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই));
  }

  HBUINT24	unicode_মান;	/* Base Unicode value of the UVS */
  GlyphID
		glyphID;	/* Glyph ID of the UVS. */
  সর্বজনীন:
  DEFINE_আকার_STATIC (3 + GlyphID::static_আকার);
};

template <typename Types>
কাঠামো NonDefaultUVS : SortedArray32Of<UVSMapping<Types>>
{
  using Mapping = UVSMapping<Types>;

  শূন্য collect_unicodes (hb_set_t *out) ধ্রুবক
  {
    জন্য (ধ্রুবক auto& a : এই->as_array ())
      out->add (a.unicode_মান);
  }

  শূন্য collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping /* OUT */) ধ্রুবক
  {
    জন্য (ধ্রুবক auto& a : এই->as_array ())
    {
      hb_codepoint_t unicode = a.unicode_মান;
      hb_codepoint_t glyphid = a.glyphID;
      unicodes->add (unicode);
      mapping->set (unicode, glyphid);
    }
  }

  শূন্য closure_glyphs (ধ্রুবক hb_set_t      *unicodes,
		       hb_set_t            *glyphset) ধ্রুবক
  {
    + এই->as_array ()
    | hb_filter (unicodes, &Mapping::unicode_মান)
    | hb_map (&Mapping::glyphID)
    | hb_sink (glyphset)
    ;
  }

  NonDefaultUVS* copy (hb_serialize_context_t *c,
		       ধ্রুবক hb_set_t *unicodes,
		       ধ্রুবক hb_set_t *glyphs_requested,
		       ধ্রুবক hb_map_t *glyph_map) ধ্রুবক
  {
    auto *out = c->start_embed<NonDefaultUVS> ();
    auto it =
    + এই->as_array ()
    | hb_filter ([&] (ধ্রুবক Mapping& _)
		 {
		   ফেরত unicodes->has (_.unicode_মান) || glyphs_requested->has (_.glyphID);
		 })
    ;

    যদি (!it) ফেরত nullptr;

    HBUINT32 len;
    len = it.len ();
    যদি (unlikely (!c->copy<HBUINT32> (len))) ফেরত nullptr;

    জন্য (ধ্রুবক Mapping& _ : it)
    {
      Mapping mapping;
      mapping.unicode_মান = _.unicode_মান;
      mapping.glyphID = glyph_map->get (_.glyphID);
      c->copy<Mapping> (mapping);
    }

    ফেরত out;
  }

  সর্বজনীন:
  DEFINE_আকার_ARRAY (4, *এই);
};

template <typename Types>
কাঠামো VariationSelectorRecord
{
  glyph_variant_t get_glyph (hb_codepoint_t codepoint,
			     hb_codepoint_t *glyph,
			     ধ্রুবক শূন্য *base) ধ্রুবক
  {
    যদি ((base+defaultUVS).bfind (codepoint))
      ফেরত GLYPH_VARIANT_USE_DEFAULT;
    ধ্রুবক UVSMapping<Types> &nonDefault = (base+nonDefaultUVS).bsearch (codepoint);
    যদি (nonDefault.glyphID)
    {
      *glyph = nonDefault.glyphID;
       ফেরত GLYPH_VARIANT_FOUND;
    }
    ফেরত GLYPH_VARIANT_NOT_FOUND;
  }

  VariationSelectorRecord(ধ্রুবক VariationSelectorRecord& other)
  {
    *এই = other;
  }

  শূন্য operator= (ধ্রুবক VariationSelectorRecord& other)
  {
    varSelector = other.varSelector;
    HBUINT32 offset = other.defaultUVS;
    defaultUVS = offset;
    offset = other.nonDefaultUVS;
    nonDefaultUVS = offset;
  }

  শূন্য collect_unicodes (hb_set_t *out, ধ্রুবক শূন্য *base) ধ্রুবক
  {
    (base+defaultUVS).collect_unicodes (out);
    (base+nonDefaultUVS).collect_unicodes (out);
  }

  শূন্য collect_mapping (ধ্রুবক শূন্য *base,
			hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping /* OUT */) ধ্রুবক
  {
    (base+defaultUVS).collect_unicodes (unicodes);
    (base+nonDefaultUVS).collect_mapping (unicodes, mapping);
  }

  পূর্ণসংখ্যা cmp (ধ্রুবক hb_codepoint_t &variation_selector) ধ্রুবক
  { ফেরত varSelector.cmp (variation_selector); }

  যৌক্তিক sanitize (hb_sanitize_context_t *c, ধ্রুবক শূন্য *base) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) &&
		  defaultUVS.sanitize (c, base) &&
		  nonDefaultUVS.sanitize (c, base));
  }

  hb_pair_t<unsigned, unsigned>
  copy (hb_serialize_context_t *c,
	ধ্রুবক hb_set_t *unicodes,
	ধ্রুবক hb_set_t *glyphs_requested,
	ধ্রুবক hb_map_t *glyph_map,
	ধ্রুবক শূন্য *base) ধ্রুবক
  {
    auto snap = c->snapshot ();
    auto *out = c->embed<VariationSelectorRecord> (*এই);
    যদি (unlikely (!out)) ফেরত hb_pair (0, 0);

    out->defaultUVS = 0;
    out->nonDefaultUVS = 0;

    unsigned non_default_uvs_objidx = 0;
    যদি (nonDefaultUVS != 0)
    {
      c->push ();
      যদি (c->copy (base+nonDefaultUVS, unicodes, glyphs_requested, glyph_map))
	non_default_uvs_objidx = c->বের_করো_pack ();
      অন্যথা c->বের_করো_discard ();
    }

    unsigned default_uvs_objidx = 0;
    যদি (defaultUVS != 0)
    {
      c->push ();
      যদি (c->copy (base+defaultUVS, unicodes))
	default_uvs_objidx = c->বের_করো_pack ();
      অন্যথা c->বের_করো_discard ();
    }


    যদি (!default_uvs_objidx && !non_default_uvs_objidx)
      c->revert (snap);

    ফেরত hb_pair (default_uvs_objidx, non_default_uvs_objidx);
  }

  HBUINT24	varSelector;	/* Variation selector. */
  Offset32To<DefaultUVS>
		defaultUVS;	/* Offset to Default UVS Table.  May be 0. */
  Offset32To<NonDefaultUVS<Types>>
		nonDefaultUVS;	/* Offset to Non-Default UVS Table.  May be 0. */
  সর্বজনীন:
  DEFINE_আকার_STATIC (11);
};

template <typename Types>
কাঠামো CmapSubtableFormat14_15
{
  স্থির constexpr unsigned format_মান = Types::আকার_2 == 2 ? 14 : 15;
  using Record = VariationSelectorRecord<Types>;
  using NonDefault = NonDefaultUVS<Types>;

  glyph_variant_t get_glyph_variant (hb_codepoint_t codepoint,
				     hb_codepoint_t variation_selector,
				     hb_codepoint_t *glyph) ধ্রুবক
  { ফেরত record.bsearch (variation_selector).get_glyph (codepoint, glyph, এই); }

  শূন্য collect_variation_selectors (hb_set_t *out) ধ্রুবক
  {
    জন্য (ধ্রুবক auto& a : record.as_array ())
      out->add (a.varSelector);
  }
  শূন্য collect_variation_unicodes (hb_codepoint_t variation_selector,
				   hb_set_t *out) ধ্রুবক
  { record.bsearch (variation_selector).collect_unicodes (out, এই); }

  শূন্য serialize (hb_serialize_context_t *c,
		  ধ্রুবক hb_set_t *unicodes,
		  ধ্রুবক hb_set_t *glyphs_requested,
		  ধ্রুবক hb_map_t *glyph_map,
		  ধ্রুবক শূন্য *base)
  {
    auto snap = c->snapshot ();
    unsigned table_initpos = c->দৈর্ঘ্য ();
    ধ্রুবক অক্ষর* init_লেজ = c->লেজ;

    যদি (unlikely (!c->extend_min (এই))) ফেরত;
    এই->format = format_মান;

    auto src_tbl = reinterpret_cast<ধ্রুবক CmapSubtableFormat14_15*> (base);

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
    জন্য (পূর্ণসংখ্যা i = src_tbl->record.len - 1; i >= 0; i--)
    {
      যদি (!unicodes->has(src_tbl->record[i].varSelector))
        চালু;

      hb_pair_t<unsigned, unsigned> ফলাফল = src_tbl->record[i].copy (c, unicodes, glyphs_requested, glyph_map, base);
      যদি (ফলাফল.first || ফলাফল.second)
	obj_indices.push (ফলাফল);
    }

    যদি (c->দৈর্ঘ্য () - table_initpos == CmapSubtableFormat14_15::min_আকার)
    {
      c->revert (snap);
      ফেরত;
    }

    যদি (unlikely (!c->check_success (!obj_indices.in_error ())))
      ফেরত;

    পূর্ণসংখ্যা লেজ_len = init_লেজ - c->লেজ;
    c->check_assign (এই->দৈর্ঘ্য, c->দৈর্ঘ্য () - table_initpos + লেজ_len,
                     HB_SERIALIZE_ERROR_INT_OVERFLOW);
    c->check_assign (এই->record.len,
		     (c->দৈর্ঘ্য () - table_initpos - CmapSubtableFormat14_15::min_আকার) /
		     Record::static_আকার,
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
					ধ্রুবক hb_vector_t<hb_pair_t<unsigned, unsigned>>& obj_indices)
  {
    জন্য (unsigned i = 0; i < obj_indices.দৈর্ঘ্য; i++)
    {
      /*
       * Since the record array has been reversed (see comments in copy())
       * but obj_indices has not been, the indices at obj_indices[i]
       * are for the variation record at record[j].
       */
      পূর্ণসংখ্যা j = obj_indices.দৈর্ঘ্য - 1 - i;
      c->add_link (record[j].defaultUVS, obj_indices[i].first);
      c->add_link (record[j].nonDefaultUVS, obj_indices[i].second);
    }
  }

  শূন্য closure_glyphs (ধ্রুবক hb_set_t      *unicodes,
		       hb_set_t            *glyphset) ধ্রুবক
  {
    + hb_iter (record)
    | hb_filter (hb_bool, &Record::nonDefaultUVS)
    | hb_filter (unicodes, &Record::varSelector)
    | hb_map (&Record::nonDefaultUVS)
    | hb_map (hb_add (এই))
    | hb_apply ([=] (ধ্রুবক NonDefault& _) { _.closure_glyphs (unicodes, glyphset); })
    ;
  }

  শূন্য collect_unicodes (hb_set_t *out) ধ্রুবক
  {
    জন্য (ধ্রুবক Record& _ : record)
      _.collect_unicodes (out, এই);
  }

  শূন্য collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping /* OUT */) ধ্রুবক
  {
    জন্য (ধ্রুবক Record& _ : record)
      _.collect_mapping (এই, unicodes, mapping);
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) &&
		  record.sanitize (c, এই));
  }

  সুরক্ষিত:
  HBUINT16	format;		/* Format number is set to 14 or 15. */
  HBUINT32	দৈর্ঘ্য;		/* Byte length of this subtable. */
  SortedArray32Of<Record>
		record;		/* Variation selector records; sorted
				 * in increasing order of `varSelector'. */
  সর্বজনীন:
  DEFINE_আকার_ARRAY (10, record);
};

কাঠামো CmapSubtable
{
  /* Note: We intentionally do NOT implement subtable formats 2 and 8. */

  যৌক্তিক is_variation_selector () ধ্রুবক
  {
    নির্বাচন (u.format.v) {
    ক্ষেত্র 14:
#ifndef HB_NO_BEYOND_64K
    ক্ষেত্র 15:
#endif
      ফেরত সত্য;
    পূর্বনির্ধারিত:
      ফেরত মিথ্যা;
    }
  }

  glyph_variant_t get_glyph_variant (hb_codepoint_t codepoint,
				     hb_codepoint_t variation_selector,
				     hb_codepoint_t *glyph) ধ্রুবক
  {
    নির্বাচন (u.format.v) {
    ক্ষেত্র 14: hb_barrier (); ফেরত u.format14.get_glyph_variant (codepoint, variation_selector, glyph);
#ifndef HB_NO_BEYOND_64K
    ক্ষেত্র 15: hb_barrier (); ফেরত u.format15.get_glyph_variant (codepoint, variation_selector, glyph);
#endif
    পূর্বনির্ধারিত: ফেরত GLYPH_VARIANT_NOT_FOUND;
    }
  }

  শূন্য collect_variation_selectors (hb_set_t *out) ধ্রুবক
  {
    নির্বাচন (u.format.v) {
    ক্ষেত্র 14: hb_barrier (); u.format14.collect_variation_selectors (out); ফেরত;
#ifndef HB_NO_BEYOND_64K
    ক্ষেত্র 15: hb_barrier (); u.format15.collect_variation_selectors (out); ফেরত;
#endif
    পূর্বনির্ধারিত: ফেরত;
    }
  }

  শূন্য collect_variation_unicodes (hb_codepoint_t variation_selector,
				    hb_set_t *out) ধ্রুবক
  {
    নির্বাচন (u.format.v) {
    ক্ষেত্র 14: hb_barrier (); u.format14.collect_variation_unicodes (variation_selector, out); ফেরত;
#ifndef HB_NO_BEYOND_64K
    ক্ষেত্র 15: hb_barrier (); u.format15.collect_variation_unicodes (variation_selector, out); ফেরত;
#endif
    পূর্বনির্ধারিত: ফেরত;
    }
  }

  শূন্য closure_glyphs (ধ্রুবক hb_set_t *unicodes, hb_set_t *glyphset) ধ্রুবক
  {
    নির্বাচন (u.format.v) {
    ক্ষেত্র 14: hb_barrier (); u.format14.closure_glyphs (unicodes, glyphset); ফেরত;
#ifndef HB_NO_BEYOND_64K
    ক্ষেত্র 15: hb_barrier (); u.format15.closure_glyphs (unicodes, glyphset); ফেরত;
#endif
    পূর্বনির্ধারিত: ফেরত;
    }
  }

  যৌক্তিক get_glyph (hb_codepoint_t codepoint,
		  hb_codepoint_t *glyph,
		  unsigned পূর্ণসংখ্যা subtable_তথ্য_আকার = 0) ধ্রুবক
  {
    নির্বাচন (u.format.v) {
    ক্ষেত্র  0: hb_barrier (); ফেরত u.format0 .get_glyph (codepoint, glyph);
    ক্ষেত্র  4: hb_barrier (); ফেরত subtable_তথ্য_আকার
				      ? u.format4.get_glyph (codepoint, glyph, subtable_তথ্য_আকার)
				      : মিথ্যা;
    ক্ষেত্র  6: hb_barrier (); ফেরত u.format6 .get_glyph (codepoint, glyph);
    ক্ষেত্র 10: hb_barrier (); ফেরত u.format10.get_glyph (codepoint, glyph);
    ক্ষেত্র 12: hb_barrier (); ফেরত u.format12.get_glyph (codepoint, glyph);
    ক্ষেত্র 13: hb_barrier (); ফেরত u.format13.get_glyph (codepoint, glyph);
    ক্ষেত্র 14:
    পূর্বনির্ধারিত: ফেরত মিথ্যা;
    }
  }
  শূন্য collect_unicodes (hb_set_t *out,
			 unsigned পূর্ণসংখ্যা অঙ্ক_glyphs = UINT_MAX,
			 unsigned পূর্ণসংখ্যা subtable_তথ্য_আকার = 0) ধ্রুবক
  {
    নির্বাচন (u.format.v) {
    ক্ষেত্র  0: hb_barrier (); u.format0 .collect_unicodes (out); ফেরত;
    ক্ষেত্র  4: hb_barrier (); যদি (subtable_তথ্য_আকার)
			      u.format4.collect_unicodes (out, subtable_তথ্য_আকার);
			    অন্যথা
			      u.format4.collect_unicodes (out);
			    ফেরত;
    ক্ষেত্র  6: hb_barrier (); u.format6 .collect_unicodes (out); ফেরত;
    ক্ষেত্র 10: hb_barrier (); u.format10.collect_unicodes (out); ফেরত;
    ক্ষেত্র 12: hb_barrier (); u.format12.collect_unicodes (out, অঙ্ক_glyphs); ফেরত;
    ক্ষেত্র 13: hb_barrier (); u.format13.collect_unicodes (out, অঙ্ক_glyphs); ফেরত;
    ক্ষেত্র 14:
    পূর্বনির্ধারিত: ফেরত;
    }
  }

  শূন্য collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping, /* OUT */
			unsigned অঙ্ক_glyphs = UINT_MAX,
			unsigned পূর্ণসংখ্যা subtable_তথ্য_আকার = 0) ধ্রুবক
  {
    নির্বাচন (u.format.v) {
    ক্ষেত্র  0: hb_barrier (); u.format0 .collect_mapping (unicodes, mapping); ফেরত;
    ক্ষেত্র  4: hb_barrier (); যদি (subtable_তথ্য_আকার)
			      u.format4.collect_mapping (unicodes, mapping, subtable_তথ্য_আকার);
			    অন্যথা
			      u.format4.collect_mapping (unicodes, mapping);
			    ফেরত;
    ক্ষেত্র  6: hb_barrier (); u.format6 .collect_mapping (unicodes, mapping); ফেরত;
    ক্ষেত্র 10: hb_barrier (); u.format10.collect_mapping (unicodes, mapping); ফেরত;
    ক্ষেত্র 12: hb_barrier (); u.format12.collect_mapping (unicodes, mapping, অঙ্ক_glyphs); ফেরত;
    ক্ষেত্র 13: hb_barrier (); u.format13.collect_mapping (unicodes, mapping, অঙ্ক_glyphs); ফেরত;
    ক্ষেত্র 14:
    পূর্বনির্ধারিত: ফেরত;
    }
  }

  unsigned get_language () ধ্রুবক
  {
    নির্বাচন (u.format.v) {
    ক্ষেত্র  0: hb_barrier (); ফেরত u.format0 .get_language ();
    ক্ষেত্র  4: hb_barrier (); ফেরত u.format4 .get_language ();
    ক্ষেত্র  6: hb_barrier (); ফেরত u.format6 .get_language ();
    ক্ষেত্র 10: hb_barrier (); ফেরত u.format10.get_language ();
    ক্ষেত্র 12: hb_barrier (); ফেরত u.format12.get_language ();
    ক্ষেত্র 13: hb_barrier (); ফেরত u.format13.get_language ();
    ক্ষেত্র 14:
    পূর্বনির্ধারিত: ফেরত 0;
    }
  }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  শূন্য serialize (hb_serialize_context_t *c,
		  Iterator it,
		  unsigned format,
		  ধ্রুবক hb_subset_plan_t *plan,
		  ধ্রুবক শূন্য *base)
  {
    নির্বাচন (format) {
    ক্ষেত্র  4: hb_barrier (); ফেরত u.format4.serialize (c, it);
    ক্ষেত্র 12: hb_barrier (); ফেরত u.format12.serialize (c, it);
    ক্ষেত্র 14: hb_barrier (); ফেরত u.format14.serialize (c, &plan->unicodes, &plan->glyphs_requested, plan->glyph_map, base);
#ifndef HB_NO_BEYOND_64K
    ক্ষেত্র 15: hb_barrier (); ফেরত u.format15.serialize (c, &plan->unicodes, &plan->glyphs_requested, plan->glyph_map, base);
#endif
    পূর্বনির্ধারিত: ফেরত;
    }
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    যদি (!u.format.v.sanitize (c)) return_trace (মিথ্যা);
    hb_barrier ();
    নির্বাচন (u.format.v) {
    ক্ষেত্র  0: hb_barrier (); return_trace (u.format0 .sanitize (c));
    ক্ষেত্র  4: hb_barrier (); return_trace (u.format4 .sanitize (c));
    ক্ষেত্র  6: hb_barrier (); return_trace (u.format6 .sanitize (c));
    ক্ষেত্র 10: hb_barrier (); return_trace (u.format10.sanitize (c));
    ক্ষেত্র 12: hb_barrier (); return_trace (u.format12.sanitize (c));
    ক্ষেত্র 13: hb_barrier (); return_trace (u.format13.sanitize (c));
    ক্ষেত্র 14: hb_barrier (); return_trace (u.format14.sanitize (c));
#ifndef HB_NO_BEYOND_64K
    ক্ষেত্র 15: hb_barrier (); return_trace (u.format15.sanitize (c));
#endif
    পূর্বনির্ধারিত:return_trace (সত্য);
    }
  }

  সর্বজনীন:
  সংঘ {
  কাঠামো { HBUINT16 v; }	format;		/* Format identifier */
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
  সর্বজনীন:
  DEFINE_আকার_UNION (2, format.v);
};


কাঠামো EncodingRecord
{
  পূর্ণসংখ্যা cmp (ধ্রুবক EncodingRecord &other) ধ্রুবক
  {
    পূর্ণসংখ্যা ret;
    ret = platformID.cmp (other.platformID);
    যদি (ret) ফেরত ret;
    যদি (other.encodingID != 0xFFFF)
    {
      ret = encodingID.cmp (other.encodingID);
      যদি (ret) ফেরত ret;
    }
    ফেরত 0;
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c, ধ্রুবক শূন্য *base) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) &&
		  subtable.sanitize (c, base));
  }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  EncodingRecord* copy (hb_serialize_context_t *c,
			Iterator it,
			unsigned format,
			ধ্রুবক শূন্য *base,
			ধ্রুবক hb_subset_plan_t *plan,
			/* INOUT */ unsigned *objidx) ধ্রুবক
  {
    TRACE_SERIALIZE (এই);
    auto snap = c->snapshot ();
    auto *out = c->embed (এই);
    যদি (unlikely (!out)) return_trace (nullptr);
    out->subtable = 0;

    যদি (*objidx == 0)
    {
      CmapSubtable *cmapsubtable = c->push<CmapSubtable> ();
      unsigned origin_দৈর্ঘ্য = c->দৈর্ঘ্য ();
      cmapsubtable->serialize (c, it, format, plan, &(base+subtable));
      যদি (c->দৈর্ঘ্য () > origin_দৈর্ঘ্য && !c->in_error()) *objidx = c->বের_করো_pack ();
      অন্যথা c->বের_করো_discard ();
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
  সর্বজনীন:
  DEFINE_আকার_STATIC (8);
};

কাঠামো cmap;

কাঠামো SubtableUnicodesCache {

 ব্যক্তিগত:
  hb_blob_ptr_t<cmap> base_blob;
  ধ্রুবক অক্ষর* base;
  unsigned পূর্ণসংখ্যা table_দৈর্ঘ্য;
  hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> cached_unicodes;

 সর্বজনীন:

  স্থির SubtableUnicodesCache* create (hb_blob_ptr_t<cmap> source_table)
  {
    SubtableUnicodesCache* cache =
        (SubtableUnicodesCache*) hb_malloc (আকার(SubtableUnicodesCache));

    যদি (unlikely (!cache))
      ফেরত nullptr;

    নতুন (cache) SubtableUnicodesCache (source_table);
    ফেরত cache;
  }

  স্থির শূন্য destroy (শূন্য* value) {
    যদি (!value) ফেরত;

    SubtableUnicodesCache* cache = (SubtableUnicodesCache*) value;
    cache->~SubtableUnicodesCache ();
    hb_free (cache);
  }

  SubtableUnicodesCache(ধ্রুবক শূন্য* cmap_base,
			unsigned পূর্ণসংখ্যা table_দৈর্ঘ্য_2 = 0)
      : base_blob(),
        base ((ধ্রুবক অক্ষর*) cmap_base),
        table_দৈর্ঘ্য (table_দৈর্ঘ্য_2),
        cached_unicodes ()
  {}

  SubtableUnicodesCache(hb_blob_ptr_t<cmap> base_blob_)
      : base_blob(base_blob_),
        base ((ধ্রুবক অক্ষর *) base_blob.get()),
        table_দৈর্ঘ্য (base_blob.get_দৈর্ঘ্য ()),
        cached_unicodes ()
  {}

  ~SubtableUnicodesCache()
  {
    base_blob.destroy ();
  }

  যৌক্তিক same_base(ধ্রুবক শূন্য* other) ধ্রুবক
  {
    ফেরত other == (ধ্রুবক শূন্য*) base;
  }

  ধ্রুবক hb_set_t* set_for (ধ্রুবক EncodingRecord* record,
                           SubtableUnicodesCache& mutable_cache) ধ্রুবক
  {
    যদি (cached_unicodes.has ((unsigned) ((ধ্রুবক অক্ষর *) record - base)))
      ফেরত cached_unicodes.get ((unsigned) ((ধ্রুবক অক্ষর *) record - base));

    ফেরত mutable_cache.set_for (record);
  }

  ধ্রুবক hb_set_t* set_for (ধ্রুবক EncodingRecord* record)
  {
    যদি (!cached_unicodes.has ((unsigned) ((ধ্রুবক অক্ষর *) record - base)))
    {
      hb_set_t *s = hb_set_create ();
      যদি (unlikely (s->in_error ()))
	ফেরত hb_set_get_empty ();

      unsigned পূর্ণসংখ্যা subtable_তথ্য_আকার = record->subtable < table_দৈর্ঘ্য
					? table_দৈর্ঘ্য - (unsigned পূর্ণসংখ্যা) record->subtable
					: 0;
      (base+record->subtable).collect_unicodes (s, UINT_MAX, subtable_তথ্য_আকার);

      যদি (unlikely (!cached_unicodes.set ((unsigned) ((ধ্রুবক অক্ষর *) record - base), hb::unique_ptr<hb_set_t> {s})))
        ফেরত hb_set_get_empty ();

      ফেরত s;
    }
    ফেরত cached_unicodes.get ((unsigned) ((ধ্রুবক অক্ষর *) record - base));
  }

};

স্থির অন্তঃরেখ uint16_t
hb_প্রতীক_pua_map (unsigned codepoint)
{
  যদি (codepoint <= 0x00FFu)
  {
    /* For symbol-encoded OpenType fonts, we duplicate the
     * U+F000..F0FF range at U+0000..U+00FF.  That's what
     * Windows seems to do, and that's hinted about at:
     * https://docs.microsoft.com/en-us/typography/opentype/spec/recom
     * under "Non-Standard (Symbol) Fonts". */
    ফেরত 0xF000u + codepoint;
  }
  ফেরত 0;
}

কাঠামো cmap
{
  স্থির constexpr hb_tag_t tableTag = HB_OT_TAG_cmap;


  স্থির SubtableUnicodesCache* create_filled_cache(hb_blob_ptr_t<cmap> source_table) {
    ধ্রুবক cmap* cmap = source_table.get();
    auto it =
    + hb_iter (cmap->encodingRecord)
    | hb_filter ([&](ধ্রুবক EncodingRecord& _) {
      ফেরত cmap::filter_encoding_records_for_subset (cmap, _);
    })
    ;

    SubtableUnicodesCache* cache = SubtableUnicodesCache::create(source_table);

    যদি (unlikely (!cache))
      ফেরত nullptr;

    জন্য (ধ্রুবক EncodingRecord& _ : it)
      cache->set_for(&_); // populate the cache for this encoding record.

    ফেরত cache;
  }

  template<typename Iterator, typename EncodingRecIter,
	   hb_requires (hb_is_iterator (EncodingRecIter))>
  যৌক্তিক serialize (hb_serialize_context_t *c,
		  Iterator it,
		  EncodingRecIter encodingrec_iter,
		  ধ্রুবক শূন্য *base,
		  hb_subset_plan_t *plan,
	                  যৌক্তিক drop_format_4 = মিথ্যা,
			  unsigned পূর্ণসংখ্যা source_table_দৈর্ঘ্য = 0)
  {
    যদি (unlikely (!c->extend_min ((*এই))))  ফেরত মিথ্যা;
    এই->version = 0;

    unsigned format4objidx = 0, format12objidx = 0, format14objidx = 0;
#ifndef HB_NO_BEYOND_64K
    unsigned format15objidx = 0;
#endif
    auto snap = c->snapshot ();

    SubtableUnicodesCache local_unicodes_cache (base, source_table_দৈর্ঘ্য);
    ধ্রুবক SubtableUnicodesCache* unicodes_cache = &local_unicodes_cache;

    যদি (plan->accelerator &&
        plan->accelerator->cmap_cache &&
        plan->accelerator->cmap_cache->same_base (base))
      unicodes_cache = plan->accelerator->cmap_cache;

    জন্য (ধ্রুবক EncodingRecord& _ : encodingrec_iter)
    {
      যদি (c->in_error ())
        ফেরত মিথ্যা;

      unsigned format = (base+_.subtable).u.format.v;
      যদি (format != 4 && format != 12 && format != 14
#ifndef HB_NO_BEYOND_64K
	  && format != 15
#endif
	 ) চালু;

      ধ্রুবক hb_set_t* unicodes_set = unicodes_cache->set_for (&_, local_unicodes_cache);

      যদি (!drop_format_4 && format == 4)
      {
        c->copy (_, + it | hb_filter (*unicodes_set, hb_first), 4u, base, plan, &format4objidx);
        যদি (c->in_error () && c->only_overflow ())
        {
          // cmap4 overflowed, reset and retry serialization without format 4 subtables.
          c->revert (snap);
          ফেরত serialize (c, it,
                            encodingrec_iter,
                            base,
                            plan,
                            সত্য,
			    source_table_দৈর্ঘ্য);
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
          চালু;
        c->copy (_, + it | hb_filter (*unicodes_set, hb_first), 12u, base, plan, &format12objidx);
      }
      অন্যথা যদি (format == 14) c->copy (_, it, 14u, base, plan, &format14objidx);
#ifndef HB_NO_BEYOND_64K
      অন্যথা যদি (format == 15) c->copy (_, it, 15u, base, plan, &format15objidx);
#endif
    }
        unsigned দৈর্ঘ্য = c->দৈর্ঘ্য ();
        unsigned available = দৈর্ঘ্য > cmap::min_আকার ? দৈর্ঘ্য - cmap::min_আকার : 0;
        c->check_assign(এই->encodingRecord.len,
                        available / EncodingRecord::static_আকার,
                        HB_SERIALIZE_ERROR_INT_OVERFLOW);

    // Fail if format 4 was dropped and there is no cmap12.
    ফেরত !drop_format_4 || format12objidx;
  }

  template<typename Iterator, typename EncodingRecordIterator,
      hb_requires (hb_is_iterator (Iterator)),
      hb_requires (hb_is_iterator (EncodingRecordIterator))>
  যৌক্তিক _can_drop (ধ্রুবক EncodingRecord& cmap12,
                  ধ্রুবক hb_set_t& cmap12_unicodes,
                  ধ্রুবক শূন্য* base,
                  ধ্রুবক SubtableUnicodesCache& unicodes_cache,
                  SubtableUnicodesCache& local_unicodes_cache,
                  Iterator subset_unicodes,
                  EncodingRecordIterator encoding_records)
  {
    জন্য (auto cp : + subset_unicodes | hb_filter (cmap12_unicodes))
    {
      যদি (cp >= 0x10000) ফেরত মিথ্যা;
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
      ফেরত মিথ্যা;
    }

    জন্য (ধ্রুবক auto& _ : encoding_records)
    {
      যদি (_.platformID != target_platform
          || _.encodingID != target_encoding
          || (base+_.subtable).get_language() != target_language)
        চালু;

      ধ্রুবক hb_set_t* sibling_unicodes = unicodes_cache.set_for (&_, local_unicodes_cache);

      auto cmap12 = + subset_unicodes | hb_filter (cmap12_unicodes);
      auto sibling = + subset_unicodes | hb_filter (*sibling_unicodes);
      জন্য (; cmap12 && sibling; cmap12++, sibling++)
      {
        unsigned a = *cmap12;
        unsigned b = *sibling;
        যদি (a != b) ফেরত মিথ্যা;
      }

      ফেরত !cmap12 && !sibling;
    }

    ফেরত মিথ্যা;
  }

  শূন্য closure_glyphs (ধ্রুবক hb_set_t      *unicodes,
		       hb_set_t            *glyphset) ধ্রুবক
  {
    + hb_iter (encodingRecord)
    | hb_map (&EncodingRecord::subtable)
    | hb_map (hb_add (এই))
    | hb_filter (&CmapSubtable::is_variation_selector)
    | hb_apply ([=] (ধ্রুবক CmapSubtable& _) { _.closure_glyphs (unicodes, glyphset); })
    ;
  }

  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  { ফেরত subset (c, মিথ্যা); }

  যৌক্তিক subset (hb_subset_context_t *c, যৌক্তিক is_dmap) ধ্রুবক
  {
    TRACE_SUBSET (এই);

    cmap *cmap_মৌলিক = c->serializer->start_embed<cmap> ();

    auto encodingrec_iter =
    + hb_iter (encodingRecord)
    | hb_filter ([&](ধ্রুবক EncodingRecord& _) {
      ফেরত cmap::filter_encoding_records_for_subset (এই, _);
    })
    ;

    যদি (unlikely (!encodingrec_iter.len ())) return_trace (মিথ্যা);

    ধ্রুবক EncodingRecord *unicode_bmp= nullptr, *unicode_ucs4 = nullptr, *ms_bmp = nullptr, *ms_ucs4 = nullptr;
    যৌক্তিক has_format12 = মিথ্যা;

    জন্য (ধ্রুবক EncodingRecord& _ : encodingrec_iter)
    {
      unsigned format = (এই + _.subtable).u.format.v;
      যদি (format == 12) has_format12 = সত্য;

      ধ্রুবক EncodingRecord *table = std::addressof (_);
      যদি      (_.platformID == 0 && _.encodingID ==  3) unicode_bmp = table;
      অন্যথা যদি (_.platformID == 0 && _.encodingID ==  4) unicode_ucs4 = table;
      অন্যথা যদি (_.platformID == 3 && _.encodingID ==  1) ms_bmp = table;
      অন্যথা যদি (_.platformID == 3 && _.encodingID == 10) ms_ucs4 = table;
    }

    যদি (unlikely (!is_dmap && !has_format12 && !unicode_bmp && !ms_bmp)) return_trace (মিথ্যা);
    যদি (unlikely (!is_dmap && has_format12 && (!unicode_ucs4 && !ms_ucs4))) return_trace (মিথ্যা);

    auto it =
    + c->plan->unicode_to_new_gid_list.iter ()
    | hb_filter ([&] (ধ্রুবক hb_codepoint_pair_t _)
		 { ফেরত (_.second != HB_MAP_মান_INVALID); })
    ;

    return_trace (cmap_মৌলিক->serialize (c->serializer,
                                         it,
                                         encodingrec_iter,
                                         এই,
                                         c->plan,
					 মিথ্যা,
					 c->source_blob->দৈর্ঘ্য));
  }

  ধ্রুবক CmapSubtable *find_best_subtable (যৌক্তিক *প্রতীক = nullptr,
					  যৌক্তিক *mac = nullptr,
					  যৌক্তিক *macroman = nullptr) ধ্রুবক
  {
    যদি (প্রতীক) *প্রতীক = মিথ্যা;
    যদি (mac) *mac = মিথ্যা;
    যদি (macroman) *macroman = মিথ্যা;

    ধ্রুবক CmapSubtable *subtable;

    /* Symbol subtable.
     * Prefer symbol if available.
     * https://github.com/harfbuzz/harfbuzz/issues/1918 */
    যদি ((subtable = এই->find_subtable (3, 0)))
    {
      যদি (প্রতীক) *প্রতীক = সত্য;
      ফেরত subtable;
    }

    /* 32-bit subtables. */
    যদি ((subtable = এই->find_subtable (3, 10))) ফেরত subtable;
    যদি ((subtable = এই->find_subtable (0, 6))) ফেরত subtable;
    যদি ((subtable = এই->find_subtable (0, 4))) ফেরত subtable;

    /* 16-bit subtables. */
    যদি ((subtable = এই->find_subtable (3, 1))) ফেরত subtable;
    যদি ((subtable = এই->find_subtable (0, 3))) ফেরত subtable;
    যদি ((subtable = এই->find_subtable (0, 2))) ফেরত subtable;
    যদি ((subtable = এই->find_subtable (0, 1))) ফেরত subtable;
    যদি ((subtable = এই->find_subtable (0, 0))) ফেরত subtable;

    /* MacRoman subtable. */
    যদি ((subtable = এই->find_subtable (1, 0)))
    {
      যদি (mac) *mac = সত্য;
      যদি (macroman) *macroman = সত্য;
      ফেরত subtable;
    }
    /* Any other Mac subtable; we just map ASCII for these. */
    যদি ((subtable = এই->find_subtable (1, 0xFFFF)))
    {
      যদি (mac) *mac = সত্য;
      ফেরত subtable;
    }

    /* Meh. */
    ফেরত &Null (CmapSubtable);
  }

  কাঠামো accelerator_t
  {
    using cache_t = hb_cache_t<21, 19>;
    static_assert (আকার (cache_t) == 1024, "");

    accelerator_t (hb_face_t *face, hb_tag_t table_tag = cmap::tableTag)
    {
      এই->table = hb_sanitize_context_t ().reference_table<cmap> (face, table_tag);
      যদি (!এই->table.get_দৈর্ঘ্য ())
	ফেরত;

      যৌক্তিক প্রতীক, mac, macroman;
      এই->subtable = table->find_best_subtable (&প্রতীক, &mac, &macroman);
      এই->subtable_uvs = &Null (CmapSubtable);
      {
	ধ্রুবক CmapSubtable *st = table->find_variation_selector_subtable ();
	যদি (st)
	  subtable_uvs = st;
      }

#ifndef HB_NO_OT_FONT_CMAP_CACHE
      cache = (cache_t *) hb_malloc (আকার (cache_t));
      যদি (cache)
	নতুন (cache) cache_t ();
      অন্যথা
        ফেরত; // Such that get_glyph_funcZ remains null.
#endif

      এই->get_glyph_তথ্য = subtable;
#ifndef HB_NO_CMAP_LEGACY_SUBTABLES
      যৌক্তিক is_format4 = subtable->u.format.v == 4;
      auto set_format4_getter = [এই] (যৌক্তিক (*func) (ধ্রুবক শূন্য *,
						      hb_codepoint_t,
						      hb_codepoint_t *))
      {
	এই->format4_accel.init (&এই->subtable->u.format4,
				  get_subtable_তথ্য_আকার (এই->subtable));
	এই->get_glyph_তথ্য = &এই->format4_accel;
	এই->get_glyph_funcZ = func;
      };
      যদি (unlikely (প্রতীক))
      {
	নির্বাচন ((unsigned) face->table.OS2->get_font_page ()) {
	ক্ষেত্র OS2::font_page_t::FONT_PAGE_NONE:
	  যদি (is_format4)
	    set_format4_getter (get_glyph_from_প্রতীক<CmapSubtableFormat4::accelerator_t, hb_প্রতীক_pua_map>);
	  অন্যথা
	    এই->get_glyph_funcZ = get_glyph_from_প্রতীক<CmapSubtable, hb_প্রতীক_pua_map>;
	  বিরতি;
#ifndef HB_NO_OT_SHAPER_ARABIC_FALLBACK
	ক্ষেত্র OS2::font_page_t::FONT_PAGE_SIMP_ARABIC:
	  যদি (is_format4)
	    set_format4_getter (get_glyph_from_প্রতীক<CmapSubtableFormat4::accelerator_t, _hb_arabic_pua_simp_map>);
	  অন্যথা
	    এই->get_glyph_funcZ = get_glyph_from_প্রতীক<CmapSubtable, _hb_arabic_pua_simp_map>;
	  বিরতি;
	ক্ষেত্র OS2::font_page_t::FONT_PAGE_TRAD_ARABIC:
	  যদি (is_format4)
	    set_format4_getter (get_glyph_from_প্রতীক<CmapSubtableFormat4::accelerator_t, _hb_arabic_pua_trad_map>);
	  অন্যথা
	    এই->get_glyph_funcZ = get_glyph_from_প্রতীক<CmapSubtable, _hb_arabic_pua_trad_map>;
	  বিরতি;
#endif
	পূর্বনির্ধারিত:
	  যদি (is_format4)
	    set_format4_getter (get_glyph_from<CmapSubtableFormat4::accelerator_t>);
	  অন্যথা
	    এই->get_glyph_funcZ = get_glyph_from<CmapSubtable>;
	  বিরতি;
	}
      }
      অন্যথা যদি (unlikely (macroman))
      {
	যদি (is_format4)
	  set_format4_getter (get_glyph_from_macroman<CmapSubtableFormat4::accelerator_t>);
	অন্যথা
	  এই->get_glyph_funcZ = get_glyph_from_macroman<CmapSubtable>;
      }
      অন্যথা যদি (unlikely (mac))
      {
	যদি (is_format4)
	  set_format4_getter (get_glyph_from_ascii<CmapSubtableFormat4::accelerator_t>);
	অন্যথা
	  এই->get_glyph_funcZ = get_glyph_from_ascii<CmapSubtable>;
      }
      অন্যথা
#endif
      {
	নির্বাচন (subtable->u.format.v) {
	  /* Accelerate format 4 and format 12. */
	  পূর্বনির্ধারিত:
	    এই->get_glyph_funcZ = get_glyph_from<CmapSubtable>;
	    বিরতি;
	  ক্ষেত্র 12:
	    এই->get_glyph_funcZ = get_glyph_from<CmapSubtableFormat12>;
	    বিরতি;
	  ক্ষেত্র  4:
	  {
	    hb_barrier ();
	    এই->format4_accel.init (&subtable->u.format4,
				      get_subtable_তথ্য_আকার (subtable));
	    এই->get_glyph_তথ্য = &এই->format4_accel;
	    এই->get_glyph_funcZ = এই->format4_accel.get_glyph_func;
	    বিরতি;
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

    অন্তঃরেখ যৌক্তিক _cached_get (hb_codepoint_t unicode,
			     hb_codepoint_t *glyph) ধ্রুবক
    {
#ifndef HB_NO_OT_FONT_CMAP_CACHE
      // cache is always non-null if we have a get_glyph_funcZ
      unsigned v;
      যদি (cache->get (unicode, &v))
      {
        *glyph = v;
	ফেরত সত্য;
      }
#endif
      যৌক্তিক ret  = এই->get_glyph_funcZ (এই->get_glyph_তথ্য, unicode, glyph);

#ifndef HB_NO_OT_FONT_CMAP_CACHE
      যদি (ret)
        cache->set (unicode, *glyph);
#endif

      ফেরত ret;
    }

    যৌক্তিক get_nominal_glyph (hb_codepoint_t  unicode,
			    hb_codepoint_t *glyph) ধ্রুবক
    {
      যদি (unlikely (!এই->get_glyph_funcZ)) ফেরত মিথ্যা;
      ফেরত _cached_get (unicode, glyph);
    }

    unsigned পূর্ণসংখ্যা get_nominal_glyphs (unsigned পূর্ণসংখ্যা গুনতি,
				     ধ্রুবক hb_codepoint_t *first_unicode,
				     unsigned পূর্ণসংখ্যা unicode_stride,
				     hb_codepoint_t *first_glyph,
				     unsigned পূর্ণসংখ্যা glyph_stride) ধ্রুবক
    {
      যদি (unlikely (!এই->get_glyph_funcZ)) ফেরত 0;

      unsigned পূর্ণসংখ্যা done;
      জন্য (done = 0;
	   done < গুনতি && _cached_get (*first_unicode, first_glyph);
	   done++)
      {
	first_unicode = &StructAtOffsetUnaligned<hb_codepoint_t> (first_unicode, unicode_stride);
	first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      }
      ফেরত done;
    }

    যৌক্তিক get_variation_glyph (hb_codepoint_t  unicode,
			      hb_codepoint_t  variation_selector,
			      hb_codepoint_t *glyph) ধ্রুবক
    {
      নির্বাচন (get_glyph_variant (unicode, variation_selector, glyph))
      {
	ক্ষেত্র GLYPH_VARIANT_NOT_FOUND:	ফেরত মিথ্যা;
	ক্ষেত্র GLYPH_VARIANT_FOUND:	ফেরত সত্য;
	ক্ষেত্র GLYPH_VARIANT_USE_DEFAULT:	বিরতি;
      }

      ফেরত get_nominal_glyph (unicode, glyph);
    }

    glyph_variant_t get_glyph_variant (hb_codepoint_t  unicode,
				       hb_codepoint_t  variation_selector,
				       hb_codepoint_t *glyph) ধ্রুবক
    {
      ফেরত এই->subtable_uvs->get_glyph_variant (unicode,
						    variation_selector,
						    glyph);
    }

    যৌক্তিক has_তথ্য () ধ্রুবক { ফেরত table.get_দৈর্ঘ্য (); }

    শূন্য collect_unicodes (hb_set_t *out, unsigned পূর্ণসংখ্যা অঙ্ক_glyphs) ধ্রুবক
    { subtable->collect_unicodes (out, অঙ্ক_glyphs, get_subtable_তথ্য_আকার (subtable)); }
    শূন্য collect_mapping (hb_set_t *unicodes, hb_map_t *mapping,
			  unsigned অঙ্ক_glyphs = UINT_MAX) ধ্রুবক
    { subtable->collect_mapping (unicodes, mapping, অঙ্ক_glyphs, get_subtable_তথ্য_আকার (subtable)); }
    শূন্য collect_variation_selectors (hb_set_t *out) ধ্রুবক
    { subtable_uvs->collect_variation_selectors (out); }
    শূন্য collect_variation_unicodes (hb_codepoint_t variation_selector,
				     hb_set_t *out) ধ্রুবক
    { subtable_uvs->collect_variation_unicodes (variation_selector, out); }

    সুরক্ষিত:
    প্রকারলেখ যৌক্তিক (*hb_cmap_get_glyph_func_t) (ধ্রুবক শূন্য *obj,
					      hb_codepoint_t codepoint,
					      hb_codepoint_t *glyph);
    প্রকারলেখ uint16_t (*hb_pua_remap_func_t) (unsigned);

    template <typename Type>
    HB_INTERNAL স্থির যৌক্তিক get_glyph_from (ধ্রুবক শূন্য *obj,
					    hb_codepoint_t codepoint,
					    hb_codepoint_t *glyph)
    {
      ধ্রুবক Type *typed_obj = (ধ্রুবক Type *) obj;
      ফেরত typed_obj->get_glyph (codepoint, glyph);
    }

    template <typename Type, hb_pua_remap_func_t remap>
    HB_INTERNAL স্থির যৌক্তিক get_glyph_from_প্রতীক (ধ্রুবক শূন্য *obj,
						   hb_codepoint_t codepoint,
						   hb_codepoint_t *glyph)
    {
      ধ্রুবক Type *typed_obj = (ধ্রুবক Type *) obj;
      যদি (likely (typed_obj->get_glyph (codepoint, glyph)))
	ফেরত সত্য;

      যদি (hb_codepoint_t c = remap (codepoint))
	ফেরত typed_obj->get_glyph (c, glyph);

      ফেরত মিথ্যা;
    }

    template <typename Type>
    HB_INTERNAL স্থির যৌক্তিক get_glyph_from_ascii (ধ্রুবক শূন্য *obj,
						  hb_codepoint_t codepoint,
						  hb_codepoint_t *glyph)
    {
      ধ্রুবক Type *typed_obj = (ধ্রুবক Type *) obj;
      ফেরত codepoint < 0x80 && typed_obj->get_glyph (codepoint, glyph);
    }

    template <typename Type>
    HB_INTERNAL স্থির যৌক্তিক get_glyph_from_macroman (ধ্রুবক শূন্য *obj,
						     hb_codepoint_t codepoint,
						     hb_codepoint_t *glyph)
    {
      যদি (get_glyph_from_ascii<Type> (obj, codepoint, glyph))
	ফেরত সত্য;

      ধ্রুবক Type *typed_obj = (ধ্রুবক Type *) obj;
      unsigned c = unicode_to_macroman (codepoint);
      ফেরত c && typed_obj->get_glyph (c, glyph);
    }

    unsigned পূর্ণসংখ্যা get_subtable_তথ্য_আকার (ধ্রুবক CmapSubtable *subtable) ধ্রুবক
    {
      unsigned পূর্ণসংখ্যা table_দৈর্ঘ্য = এই->table.get_দৈর্ঘ্য ();
      uintptr_t table_start = (uintptr_t) (ধ্রুবক শূন্য *) এই->table.get ();
      uintptr_t subtable_addr = (uintptr_t) (ধ্রুবক শূন্য *) subtable;
      যদি (unlikely (subtable_addr < table_start))
	ফেরত 0;

      uintptr_t subtable_offset = subtable_addr - table_start;
      যদি (unlikely (subtable_offset >= table_দৈর্ঘ্য))
	ফেরত 0;

      ফেরত table_দৈর্ঘ্য - (unsigned পূর্ণসংখ্যা) subtable_offset;
    }

    ব্যক্তিগত:
    hb_nonnull_ptr_t<ধ্রুবক CmapSubtable> subtable;
    hb_nonnull_ptr_t<ধ্রুবক CmapSubtable> subtable_uvs;

    hb_cmap_get_glyph_func_t get_glyph_funcZ = nullptr;
    ধ্রুবক শূন্য *get_glyph_তথ্য = nullptr;

    CmapSubtableFormat4::accelerator_t format4_accel;

#ifndef HB_NO_OT_FONT_CMAP_CACHE
    cache_t *cache = nullptr;
#endif

    সর্বজনীন:
    hb_blob_ptr_t<cmap> table;
  };

  সুরক্ষিত:

  ধ্রুবক CmapSubtable *find_subtable (unsigned পূর্ণসংখ্যা platform_id,
				     unsigned পূর্ণসংখ্যা encoding_id) ধ্রুবক
  {
    EncodingRecord key;
    key.platformID = platform_id;
    key.encodingID = encoding_id;

    ধ্রুবক EncodingRecord &ফলাফল = encodingRecord.bsearch (key);
    যদি (!ফলাফল.subtable)
      ফেরত nullptr;

    ফেরত &(এই+ফলাফল.subtable);
  }

  ধ্রুবক CmapSubtable *find_variation_selector_subtable () ধ্রুবক
  {
    ধ্রুবক CmapSubtable *format14 = nullptr;
    জন্য (ধ্রুবক EncodingRecord& record : encodingRecord)
    {
      যদি (record.platformID != 0 || record.encodingID != 5)
	চালু;

      ধ্রুবক CmapSubtable *subtable = &(এই+record.subtable);
#ifndef HB_NO_BEYOND_64K
      যদি (subtable->u.format.v == 15)
	ফেরত subtable;
#endif
      যদি (subtable->u.format.v == 14)
	format14 = subtable;
    }
    ফেরত format14;
  }

  সর্বজনীন:

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) &&
		  hb_barrier () &&
		  likely (version == 0) &&
		  encodingRecord.sanitize (c, এই));
  }

 ব্যক্তিগত:

  স্থির যৌক্তিক filter_encoding_records_for_subset(ধ্রুবক cmap* cmap,
                                                 ধ্রুবক EncodingRecord& _)
  {
    ফেরত
        (_.platformID == 0 && _.encodingID == 3) ||
        (_.platformID == 0 && _.encodingID == 4) ||
        (_.platformID == 3 && _.encodingID == 1) ||
        (_.platformID == 3 && _.encodingID == 10) ||
        (cmap + _.subtable).is_variation_selector ();
  }

  সুরক্ষিত:
  HBUINT16	version;	/* Table version number (0). */
  SortedArray16Of<EncodingRecord>
		encodingRecord;	/* Encoding tables. */
  সর্বজনীন:
  DEFINE_আকার_ARRAY (4, encodingRecord);
};

কাঠামো DMAP : cmap
{
  স্থির constexpr hb_tag_t tableTag = HB_OT_TAG_DMAP;

  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  { ফেরত cmap::subset (c, সত্য); }
};

কাঠামো cmap_accelerator_t
{
  cmap_accelerator_t (hb_face_t *face) :
    dmap (face, HB_OT_TAG_DMAP),
    base (face)
  {}

  যৌক্তিক get_nominal_glyph (hb_codepoint_t  unicode,
			  hb_codepoint_t *glyph) ধ্রুবক
  {
    ফেরত dmap.get_nominal_glyph (unicode, glyph) ||
	   base.get_nominal_glyph (unicode, glyph);
  }

  unsigned পূর্ণসংখ্যা get_nominal_glyphs (unsigned পূর্ণসংখ্যা গুনতি,
				   ধ্রুবক hb_codepoint_t *first_unicode,
				   unsigned পূর্ণসংখ্যা unicode_stride,
				   hb_codepoint_t *first_glyph,
				   unsigned পূর্ণসংখ্যা glyph_stride) ধ্রুবক
  {
    যদি (likely (!dmap.has_তথ্য ()))
      ফেরত base.get_nominal_glyphs (গুনতি,
				      first_unicode, unicode_stride,
				      first_glyph, glyph_stride);

    unsigned পূর্ণসংখ্যা done;
    জন্য (done = 0;
	 done < গুনতি && get_nominal_glyph (*first_unicode, first_glyph);
	 done++)
    {
      first_unicode = &StructAtOffsetUnaligned<hb_codepoint_t> (first_unicode, unicode_stride);
      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
    }
    ফেরত done;
  }

  যৌক্তিক get_variation_glyph (hb_codepoint_t  unicode,
			    hb_codepoint_t  variation_selector,
			    hb_codepoint_t *glyph) ধ্রুবক
  {
    glyph_variant_t ফলাফল = dmap.get_glyph_variant (unicode,
						     variation_selector,
						     glyph);
    যদি (ফলাফল == GLYPH_VARIANT_NOT_FOUND)
      ফলাফল = base.get_glyph_variant (unicode, variation_selector, glyph);

    নির্বাচন (ফলাফল)
    {
      ক্ষেত্র GLYPH_VARIANT_NOT_FOUND: ফেরত মিথ্যা;
      ক্ষেত্র GLYPH_VARIANT_FOUND: ফেরত সত্য;
      ক্ষেত্র GLYPH_VARIANT_USE_DEFAULT: বিরতি;
    }

    ফেরত get_nominal_glyph (unicode, glyph);
  }

  শূন্য collect_unicodes (hb_set_t *out, unsigned পূর্ণসংখ্যা অঙ্ক_glyphs) ধ্রুবক
  {
    base.collect_unicodes (out, অঙ্ক_glyphs);
    dmap.collect_unicodes (out, অঙ্ক_glyphs);
  }

  শূন্য collect_mapping (hb_set_t *unicodes, hb_map_t *mapping,
			unsigned অঙ্ক_glyphs = UINT_MAX) ধ্রুবক
  {
    base.collect_mapping (unicodes, mapping, অঙ্ক_glyphs);
    dmap.collect_mapping (unicodes, mapping, অঙ্ক_glyphs);
  }

  শূন্য collect_variation_selectors (hb_set_t *out) ধ্রুবক
  {
    base.collect_variation_selectors (out);
    dmap.collect_variation_selectors (out);
  }

  শূন্য collect_variation_unicodes (hb_codepoint_t variation_selector,
				   hb_set_t *out) ধ্রুবক
  {
    base.collect_variation_unicodes (variation_selector, out);
    dmap.collect_variation_unicodes (variation_selector, out);
  }

  শূন্য closure_glyphs (ধ্রুবক hb_set_t *unicodes,
		       hb_set_t       *glyphset) ধ্রুবক
  {
    যদি (likely (!dmap.has_তথ্য ()))
    {
      base.table->closure_glyphs (unicodes, glyphset);
      ফেরত;
    }

    hb_set_t variation_selectors;
    collect_variation_selectors (&variation_selectors);
    জন্য (hb_codepoint_t variation_selector : variation_selectors)
    {
      যদি (!unicodes->has (variation_selector))
	চালু;

      hb_set_t variation_unicodes;
      collect_variation_unicodes (variation_selector, &variation_unicodes);
      জন্য (hb_codepoint_t unicode : variation_unicodes)
      {
	যদি (!unicodes->has (unicode))
	  চালু;

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
