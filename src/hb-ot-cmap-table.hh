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

#अन्तर्भाव "hb-ot-os2-table.hh"
#अन्तर्भाव "hb-ot-shaper-arabic-pua.hh"
#अन्तर्भाव "hb-open-type.hh"
#अन्तर्भाव "hb-ot-dual.hh"
#अन्तर्भाव "hb-set.hh"
#अन्तर्भाव "hb-cache.hh"

/*
 * cmap -- Character to Glyph Index Mapping
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cmap
 */
#define HB_OT_TAG_cmap HB_TAG('c','m','a','p')
#define HB_OT_TAG_DMAP HB_TAG('D','M','A','P')

नामाकाश OT {

स्थिर अन्तःस्थ uint8_t unicode_to_macroman (hb_codepoint_t u)
{
  स्थिर ध्रुव संरचना unicode_to_macroman_t
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
  auto *c = hb_bsearch (u, mapping, ARRAY_LENGTH (mapping), परिमाण (mapping[0]),
			_hb_cmp_operator<uint16_t, uint16_t>);
  प्रत्यागम c ? c->macroman : 0;
}

संरचना CmapSubtableFormat0
{
  तार्किक get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) ध्रुव
  {
    hb_codepoint_t gid = codepoint < 256 ? glyphIdArray[codepoint] : 0;
    जइ (unlikely (!gid))
      प्रत्यागम मिच्छ;
    *glyph = gid;
    प्रत्यागम सच्च;
  }

  unsigned get_language () ध्रुव
  {
    प्रत्यागम language;
  }

  शून्य collect_unicodes (hb_set_t *out) ध्रुव
  {
    प्रति (unsigned पूर्णाङ्क i = 0; i < 256; i++)
      जइ (glyphIdArray[i])
	out->add (i);
  }

  शून्य collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping /* OUT */) ध्रुव
  {
    प्रति (unsigned i = 0; i < 256; i++)
      जइ (glyphIdArray[i])
      {
	hb_codepoint_t glyph = glyphIdArray[i];
	unicodes->add (i);
	mapping->set (i, glyph);
      }
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्));
  }

  रक्षित:
  HBUINT16	format;		/* Format number is set to 0. */
  HBUINT16	length;		/* Byte length of this subtable. */
  HBUINT16	language;	/* Ignore. */
  HBUINT8	glyphIdArray[256];/* An array that maps character
				 * code to glyph index values. */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (6 + 256);
};

संरचना CmapSubtableFormat4
{
  template<typename Iterator,
      typename Writer,
	   hb_requires (hb_is_iterator (Iterator))>
  शून्य to_ranges (Iterator it, Writer& range_writer)
  {
    hb_codepoint_t start_cp = 0, prev_run_start_cp = 0, run_start_cp = 0, end_cp = 0, last_gid = 0;
    पूर्णाङ्क run_length = 0 , delta = 0, prev_delta = 0;

    गणना {
      FIRST_SUB_RANGE,
      FOLLOWING_SUB_RANGE,
    } mode;

    जाव (it) {
      // Start a new range
      {
        ध्रुव auto& pair = *it;
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

      जाव (it) {
        // Process range
        ध्रुव auto& pair = *it;
        hb_codepoint_t next_cp = pair.first;
        hb_codepoint_t next_gid = pair.second;
        जइ (next_cp != end_cp + 1) {
          // Current range is over, stop processing.
          भङ्ग;
        }

        जइ (next_gid == last_gid + 1) {
          // The current run continues.
          end_cp = next_cp;
          run_length++;
          last_gid = next_gid;
          it++;
          अनुवर्त;
        }

        // A new run is starting, decide if we want to commit the current run.
        पूर्णाङ्क split_cost = (mode == FIRST_SUB_RANGE) ? 8 : 16;
        पूर्णाङ्क run_cost = run_length * 2;
        जइ (run_cost >= split_cost) {
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

    जइ (likely (end_cp != 0xFFFF)) {
      range_writer (0xFFFF, 0xFFFF, 1);
    }
  }

  /*
   * Writes the current range as either one or two ranges depending on what is most efficient.
   */
  template<typename Writer>
  शून्य commit_current_range (hb_codepoint_t start,
                             hb_codepoint_t prev_run_start,
                             hb_codepoint_t run_start,
                             hb_codepoint_t end,
                             पूर्णाङ्क run_delta,
                             पूर्णाङ्क previous_run_delta,
                             पूर्णाङ्क split_cost,
                             Writer& range_writer) {
    तार्किक should_split = मिच्छ;
    जइ (start < run_start && run_start < end) {
      पूर्णाङ्क run_cost = (end - run_start + 1) * 2;
      जइ (run_cost >= split_cost) {
        should_split = सच्च;
      }
    }

    // A zero delta is also the marker for ranges that need a
    // glyphIdArray; serialize_rangeoffset_glyid() tells the two cases
    // apart by re-checking the mapping.
    जइ (should_split) {
      जइ (start == prev_run_start)
        range_writer (start, run_start - 1, previous_run_delta);
      अण्णहा
        range_writer (start, run_start - 1, 0);
      range_writer (run_start, end, run_delta);
      प्रत्यागम;
    }


    जइ (start == run_start) {
      // Range is only a run
      range_writer (start, end, run_delta);
      प्रत्यागम;
    }

    // Write only a single non-run range.
    range_writer (start, end, 0);
  }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  unsigned serialize_find_segcount (Iterator it) {
    संरचना Counter {
      unsigned segcount = 0;

      शून्य operator() (hb_codepoint_t start,
                       hb_codepoint_t end,
                       पूर्णाङ्क delta) {
        segcount++;
      }
    } counter;

    to_ranges (+it, counter);
    प्रत्यागम counter.segcount;
  }


  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  तार्किक serialize_start_end_delta_arrays (hb_serialize_context_t *c,
                                         Iterator it,
                                         पूर्णाङ्क segcount)
  {
    संरचना Writer {
      hb_serialize_context_t *serializer_;
      HBUINT16* end_code_;
      HBUINT16* start_code_;
      HBINT16* id_delta_;
      पूर्णाङ्क index_;

      Writer(hb_serialize_context_t *serializer)
          : serializer_(serializer),
            end_code_(nullptr),
            start_code_(nullptr),
            id_delta_(nullptr),
            index_ (0) {}
      शून्य operator() (hb_codepoint_t start,
                       hb_codepoint_t end,
                       पूर्णाङ्क delta) {
        start_code_[index_] = start;
        end_code_[index_] = end;
        id_delta_[index_] = delta;
        index_++;
      }
    } writer(c);

    writer.end_code_ = c->allocate_size<HBUINT16> (HBUINT16::static_size * segcount, मिच्छ);
    (शून्य) c->allocate_size<HBUINT16> (2); // padding
    writer.start_code_ = c->allocate_size<HBUINT16> (HBUINT16::static_size * segcount, मिच्छ);
    writer.id_delta_ = c->allocate_size<HBINT16> (HBINT16::static_size * segcount, मिच्छ);

    जइ (unlikely (!writer.end_code_ || !writer.start_code_ || !writer.id_delta_)) प्रत्यागम मिच्छ;

    to_ranges (+it, writer);
    प्रत्यागम सच्च;
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
    जइ (unlikely (!c->check_success (idRangeOffset))) प्रत्यागम nullptr;
    जइ (unlikely ((वर्ण *)idRangeOffset - (वर्ण *)idDelta != (पूर्णाङ्क) segcount * (पूर्णाङ्क) HBINT16::static_size)) प्रत्यागम nullptr;

    प्रति (unsigned i : hb_range (segcount))
    {
      /* A non-zero delta is always a real run. A zero delta is
       * ambiguous: identity mapping, or the marker for a range that
       * needs a glyphIdArray. Only zero deltas need a check. */
      पूर्णाङ्क delta = idDelta[i];
      जइ (delta)
	अनुवर्त;
      तार्किक use_delta = सच्च;
      प्रति (hb_codepoint_t cp = startCode[i]; cp <= endCode[i]; cp++)
      {
	hb_codepoint_t gid = cp_to_gid.get (cp);
	जइ (gid == HB_MAP_मानम्_INVALID)
	  gid = 0;
	जइ (gid != cp)
	{
	  use_delta = मिच्छ;
	  भङ्ग;
	}
      }
      जइ (use_delta)
	अनुवर्त;

      idRangeOffset[i] = 2 * (c->start_embed<HBUINT16> () - idRangeOffset - i);
      प्रति (hb_codepoint_t cp = startCode[i]; cp <= endCode[i]; cp++)
      {
        HBUINT16 gid;
        gid = cp_to_gid[cp];
        c->copy<HBUINT16> (gid);
      }
    }

    प्रत्यागम idRangeOffset;
  }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  शून्य serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    auto format4_iter =
    + it
    | hb_filter ([&] (ध्रुव hb_codepoint_pair_t _)
		 { प्रत्यागम _.first <= 0xFFFF; })
    ;

    जइ (!format4_iter) प्रत्यागम;

    unsigned table_initpos = c->length ();
    जइ (unlikely (!c->extend_min (एतत्))) प्रत्यागम;
    एतत्->format = 4;

    hb_vector_t<hb_codepoint_pair_t> cp_to_gid {
      format4_iter
    };

    //serialize endCode[], startCode[], idDelta[]
    HBUINT16* endCode = c->start_embed<HBUINT16> ();
    unsigned segcount = serialize_find_segcount (cp_to_gid.iter());
    जइ (unlikely (!serialize_start_end_delta_arrays (c, cp_to_gid.iter(), segcount)))
      प्रत्यागम;

    HBUINT16 *startCode = endCode + segcount + 1;
    HBINT16 *idDelta = ((HBINT16*)startCode) + segcount;

    HBUINT16 *idRangeOffset = serialize_rangeoffset_glyid (c,
                                                           cp_to_gid.iter (),
                                                           endCode,
                                                           startCode,
                                                           idDelta,
                                                           segcount);
    जइ (unlikely (!c->check_success (idRangeOffset))) प्रत्यागम;

    एतत्->length = c->length () - table_initpos;
    जइ ((long long) एतत्->length != (long long) c->length () - table_initpos)
    {
      c->err (HB_SERIALIZE_ERROR_INT_OVERFLOW);
      प्रत्यागम;
    }

    एतत्->seg_गणनम्_X2 = segcount * 2;
    एतत्->entrySelector = hb_max (1u, hb_bit_storage (segcount)) - 1;
    एतत्->अन्वेषणम्_Range = 2 * (1u << एतत्->entrySelector);
    एतत्->rangeShift = segcount * 2 > एतत्->अन्वेषणम्_Range
		       ? 2 * segcount - एतत्->अन्वेषणम्_Range
		       : 0;
  }

  unsigned get_language () ध्रुव
  {
    प्रत्यागम language;
  }

  संरचना accelerator_t
  {
    accelerator_t () {}
    accelerator_t (ध्रुव CmapSubtableFormat4 *subtable) = delete;

    शून्य init (ध्रुव CmapSubtableFormat4 *subtable,
	       unsigned पूर्णाङ्क              subtable_data_size)
    {
      seg_गणनम्_2 = subtable->seg_गणनम्_X2 / 2;
      end_गणनम् = subtable->values.arrayZ;
      start_गणनम् = end_गणनम् + seg_गणनम्_2 + 1;
      idDelta = start_गणनम् + seg_गणनम्_2;
      idRangeOffset = idDelta + seg_गणनम्_2;
      glyphIdArray = idRangeOffset + seg_गणनम्_2;

      unsigned पूर्णाङ्क मानानि_offset = 16 + 8 * seg_गणनम्_2;
      glyphIdArrayLength = subtable_data_size > मानानि_offset
			   ? (subtable_data_size - मानानि_offset) / 2
			   : 0;
    }

    तार्किक get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) ध्रुव
    {
      संरचना CustomRange
      {
	पूर्णाङ्क cmp (hb_codepoint_t k,
		 unsigned distance) ध्रुव
	{
	  जइ (k > last) प्रत्यागम +1;
	  जइ (k < (&last)[distance]/*first*/) प्रत्यागम -1;
	  प्रत्यागम 0;
	}
	HBUINT16 last;
      };

      ध्रुव HBUINT16 *found = hb_bsearch (codepoint,
					  एतत्->end_गणनम्,
					  एतत्->seg_गणनम्_2,
					  परिमाण (CustomRange),
					  _hb_cmp_method<hb_codepoint_t, CustomRange, unsigned>,
					  एतत्->seg_गणनम्_2 + 1);
      जइ (unlikely (!found))
	प्रत्यागम मिच्छ;
      unsigned पूर्णाङ्क i = found - end_गणनम्;

      hb_codepoint_t gid;
      unsigned पूर्णाङ्क rangeOffset = एतत्->idRangeOffset[i];
      जइ (rangeOffset == 0)
	gid = codepoint + एतत्->idDelta[i];
      अण्णहा
      {
	/* Somebody has been smoking... */
	unsigned पूर्णाङ्क index = rangeOffset / 2 + (codepoint - एतत्->start_गणनम्[i]) + i - एतत्->seg_गणनम्_2;
	जइ (unlikely (index >= एतत्->glyphIdArrayLength))
	  प्रत्यागम मिच्छ;
	gid = एतत्->glyphIdArray[index];
	जइ (unlikely (!gid))
	  प्रत्यागम मिच्छ;
	gid += एतत्->idDelta[i];
      }
      gid &= 0xFFFFu;
      जइ (unlikely (!gid))
	प्रत्यागम मिच्छ;
      *glyph = gid;
      प्रत्यागम सच्च;
    }

    HB_INTERNAL स्थिर तार्किक get_glyph_func (ध्रुव शून्य *obj, hb_codepoint_t codepoint, hb_codepoint_t *glyph)
    { प्रत्यागम ((ध्रुव accelerator_t *) obj)->get_glyph (codepoint, glyph); }

    शून्य collect_unicodes (hb_set_t *out) ध्रुव
    {
      unsigned पूर्णाङ्क गणनम् = एतत्->seg_गणनम्_2;
      जइ (गणनम् && एतत्->start_गणनम्[गणनम् - 1] == 0xFFFFu)
	गणनम्--; /* Skip sentinel segment. */
      प्रति (unsigned पूर्णाङ्क i = 0; i < गणनम्; i++)
      {
	hb_codepoint_t start = एतत्->start_गणनम्[i];
	hb_codepoint_t end = एतत्->end_गणनम्[i];
	unsigned पूर्णाङ्क rangeOffset = एतत्->idRangeOffset[i];
        out->add_range(start, end);
	जइ (rangeOffset == 0)
	{
	  प्रति (hb_codepoint_t codepoint = start; codepoint <= end; codepoint++)
	  {
	    hb_codepoint_t gid = (codepoint + एतत्->idDelta[i]) & 0xFFFFu;
	    जइ (unlikely (!gid))
              out->del(codepoint);
	  }
	}
	अण्णहा
	{
	  प्रति (hb_codepoint_t codepoint = start; codepoint <= end; codepoint++)
	  {
	    unsigned पूर्णाङ्क index = rangeOffset / 2 + (codepoint - एतत्->start_गणनम्[i]) + i - एतत्->seg_गणनम्_2;
	    जइ (unlikely (index >= एतत्->glyphIdArrayLength))
            {
              out->del_range (codepoint, end);
	      भङ्ग;
            }
	    hb_codepoint_t gid = एतत्->glyphIdArray[index];
	    जइ (unlikely (!gid))
              out->del(codepoint);
	  }
	}
      }
    }

    शून्य collect_mapping (hb_set_t *unicodes, /* OUT */
			  hb_map_t *mapping /* OUT */) ध्रुव
    {
      // TODO(grieger): optimize similar to collect_unicodes
      // (ie. use add_range())
      unsigned गणनम् = एतत्->seg_गणनम्_2;
      जइ (गणनम् && एतत्->start_गणनम्[गणनम् - 1] == 0xFFFFu)
	गणनम्--; /* Skip sentinel segment. */
      प्रति (unsigned i = 0; i < गणनम्; i++)
      {
	hb_codepoint_t start = एतत्->start_गणनम्[i];
	hb_codepoint_t end = एतत्->end_गणनम्[i];
	unsigned rangeOffset = एतत्->idRangeOffset[i];
	जइ (rangeOffset == 0)
	{
	  प्रति (hb_codepoint_t codepoint = start; codepoint <= end; codepoint++)
	  {
	    hb_codepoint_t gid = (codepoint + एतत्->idDelta[i]) & 0xFFFFu;
	    जइ (unlikely (!gid))
	      अनुवर्त;
	    unicodes->add (codepoint);
	    mapping->set (codepoint, gid);
	  }
	}
	अण्णहा
	{
	  प्रति (hb_codepoint_t codepoint = start; codepoint <= end; codepoint++)
	  {
	    unsigned index = rangeOffset / 2 + (codepoint - एतत्->start_गणनम्[i]) + i - एतत्->seg_गणनम्_2;
	    जइ (unlikely (index >= एतत्->glyphIdArrayLength))
	      भङ्ग;
	    hb_codepoint_t gid = एतत्->glyphIdArray[index];
	    जइ (unlikely (!gid))
	      अनुवर्त;
            gid = (gid + एतत्->idDelta[i]) & 0xFFFFu;
	    unicodes->add (codepoint);
	    mapping->set (codepoint, gid);
	  }
	}
      }
    }

    ध्रुव HBUINT16 *end_गणनम्;
    ध्रुव HBUINT16 *start_गणनम्;
    ध्रुव HBUINT16 *idDelta;
    ध्रुव HBUINT16 *idRangeOffset;
    ध्रुव HBUINT16 *glyphIdArray;
    unsigned पूर्णाङ्क seg_गणनम्_2;
    unsigned पूर्णाङ्क glyphIdArrayLength;
  };

  तार्किक get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph,
		  unsigned पूर्णाङ्क subtable_data_size) ध्रुव
  {
    accelerator_t accel;
    accel.init (एतत्, subtable_data_size);
    प्रत्यागम accel.get_glyph_func (&accel, codepoint, glyph);
  }
  तार्किक get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) ध्रुव
  { प्रत्यागम मिच्छ; }

  शून्य collect_unicodes (hb_set_t *out, unsigned पूर्णाङ्क subtable_data_size) ध्रुव
  {
    accelerator_t accel;
    accel.init (एतत्, subtable_data_size);
    accel.collect_unicodes (out);
  }
  शून्य collect_unicodes (hb_set_t *out) ध्रुव
  { collect_unicodes (out, length); }

  शून्य collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping, /* OUT */
			unsigned पूर्णाङ्क subtable_data_size) ध्रुव
  {
    accelerator_t accel;
    accel.init (एतत्, subtable_data_size);
    accel.collect_mapping (unicodes, mapping);
  }
  शून्य collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping /* OUT */) ध्रुव
  { collect_mapping (unicodes, mapping, length); }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    जइ (unlikely (!c->check_struct (एतत्)))
      return_trace (मिच्छ);
    hb_barrier ();

    return_trace (c->check_range (values, 2 + 4 * seg_गणनम्_X2));
  }

  रक्षित:
  HBUINT16	format;		/* Format number is set to 4. */
  HBUINT16	length;		/* This is the length in bytes of the
				 * subtable. */
  HBUINT16	language;	/* Ignore. */
  HBUINT16	seg_गणनम्_X2;	/* 2 x segCount. */
  HBUINT16	अन्वेषणम्_Range;	/* 2 * (2**floor(log2(segCount))) */
  HBUINT16	entrySelector;	/* log2(searchRange/2) */
  HBUINT16	rangeShift;	/* 2 x segCount - searchRange */

  UnsizedArrayOf<HBUINT16>
		values;
#जइ 0
  HBUINT16	end_गणनम्[seg_गणनम्_2];	/* End characterCode for each segment,
					 * last=0xFFFFu. */
  HBUINT16	reservedPad;		/* Set to 0. */
  HBUINT16	start_गणनम्[seg_गणनम्_2];	/* Start character code for each segment. */
  HBINT16		idDelta[seg_गणनम्_2];	/* Delta for all character codes in segment. */
  HBUINT16	idRangeOffset[seg_गणनम्_2];/* Offsets into glyphIdArray or 0 */
  UnsizedArrayOf<HBUINT16>
		glyphIdArray;	/* Glyph index array (arbitrary length) */
#endif

  सार्वजनिक:
  DEFINE_SIZE_ARRAY (14, values);
};

संरचना CmapSubtableLongGroup
{
  friend संरचना CmapSubtableFormat12;
  friend संरचना CmapSubtableFormat13;
  template<typename U>
  friend संरचना CmapSubtableLongSegmented;
  friend संरचना cmap;

  पूर्णाङ्क cmp (hb_codepoint_t codepoint) ध्रुव
  {
    जइ (codepoint < startCharCode) प्रत्यागम -1;
    जइ (codepoint > endCharCode)   प्रत्यागम +1;
    प्रत्यागम 0;
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्));
  }

  गुप्त:
  HBUINT32		startCharCode;	/* First character code in this group. */
  HBUINT32		endCharCode;	/* Last character code in this group. */
  HBUINT32		glyphID;	/* Glyph index; interpretation depends on
					 * subtable format. */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (12);
};
DECLARE_NULL_NAMESPACE_BYTES (OT, CmapSubtableLongGroup);

template <typename UINT>
संरचना CmapSubtableTrimmed
{
  तार्किक get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) ध्रुव
  {
    /* Rely on our implicit array bound-checking. */
    hb_codepoint_t gid = glyphIdArray[codepoint - startCharCode];
    जइ (unlikely (!gid))
      प्रत्यागम मिच्छ;
    *glyph = gid;
    प्रत्यागम सच्च;
  }

  unsigned get_language () ध्रुव
  {
    प्रत्यागम language;
  }

  शून्य collect_unicodes (hb_set_t *out) ध्रुव
  {
    hb_codepoint_t start = startCharCode;
    unsigned पूर्णाङ्क गणनम् = glyphIdArray.len;
    प्रति (unsigned पूर्णाङ्क i = 0; i < गणनम्; i++)
      जइ (glyphIdArray[i])
	out->add (start + i);
  }

  शून्य collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping /* OUT */) ध्रुव
  {
    hb_codepoint_t start_cp = startCharCode;
    unsigned गणनम् = glyphIdArray.len;
    प्रति (unsigned i = 0; i < गणनम्; i++)
      जइ (glyphIdArray[i])
      {
	hb_codepoint_t unicode = start_cp + i;
	hb_codepoint_t glyphid = glyphIdArray[i];
	unicodes->add (unicode);
	mapping->set (unicode, glyphid);
      }
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) && glyphIdArray.sanitize (c));
  }

  रक्षित:
  UINT		formatReserved;	/* Subtable format and (maybe) padding. */
  UINT		length;		/* Byte length of this subtable. */
  UINT		language;	/* Ignore. */
  UINT		startCharCode;	/* First character code covered. */
  ArrayOf<HBGlyphID16, UINT>
		glyphIdArray;	/* Array of glyph index values for character
				 * codes in the range. */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (5 * परिमाण (UINT), glyphIdArray);
};

संरचना CmapSubtableFormat6  : CmapSubtableTrimmed<HBUINT16> {};
संरचना CmapSubtableFormat10 : CmapSubtableTrimmed<HBUINT32> {};

template <typename T>
संरचना CmapSubtableLongSegmented
{
  friend संरचना cmap;

  तार्किक get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) ध्रुव
  {
    hb_codepoint_t gid = T::group_get_glyph (groups.bsearch (codepoint), codepoint);
    जइ (unlikely (!gid))
      प्रत्यागम मिच्छ;
    *glyph = gid;
    प्रत्यागम सच्च;
  }

  unsigned get_language () ध्रुव
  {
    प्रत्यागम language;
  }

  शून्य collect_unicodes (hb_set_t *out, unsigned पूर्णाङ्क अङ्कः_glyphs) ध्रुव
  {
    प्रति (unsigned पूर्णाङ्क i = 0; i < एतत्->groups.len; i++)
    {
      hb_codepoint_t start = एतत्->groups[i].startCharCode;
      hb_codepoint_t end = hb_min ((hb_codepoint_t) एतत्->groups[i].endCharCode,
				   (hb_codepoint_t) HB_UNICODE_MAX);
      hb_codepoint_t gid = एतत्->groups[i].glyphID;
      जइ (!gid)
      {
	/* Intention is: if (hb_is_same (T, CmapSubtableFormat13)) continue; */
	जइ (! T::group_get_glyph (एतत्->groups[i], end)) अनुवर्त;
	start++;
	gid++;
      }
      जइ (unlikely ((unsigned पूर्णाङ्क) gid >= अङ्कः_glyphs)) अनुवर्त;
      जइ (unlikely ((unsigned पूर्णाङ्क) (gid + end - start) >= अङ्कः_glyphs))
	end = start + (hb_codepoint_t) अङ्कः_glyphs - gid;

      out->add_range (start, hb_min (end, 0x10FFFFu));
    }
  }

  शून्य collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping, /* OUT */
			unsigned अङ्कः_glyphs) ध्रुव
  {
    hb_codepoint_t last_end = 0;
    unsigned गणनम् = एतत्->groups.len;
    प्रति (unsigned i = 0; i < गणनम्; i++)
    {
      hb_codepoint_t start = एतत्->groups.arrayZ[i].startCharCode;
      hb_codepoint_t end = hb_min ((hb_codepoint_t) एतत्->groups.arrayZ[i].endCharCode,
				   (hb_codepoint_t) HB_UNICODE_MAX);
      जइ (unlikely (start > end || start < last_end)) {
        // Range is not in order and is invalid, skip it.
        अनुवर्त;
      }
      last_end = end;


      hb_codepoint_t gid = एतत्->groups.arrayZ[i].glyphID;
      जइ (!gid)
      {
        जइ (T::format_सङ्ख्या == 13) अनुवर्त;
	start++;
	gid++;
      }
      जइ (unlikely ((unsigned पूर्णाङ्क) gid >= अङ्कः_glyphs)) अनुवर्त;
      जइ (unlikely ((unsigned पूर्णाङ्क) (gid + end - start) >= अङ्कः_glyphs))
	end = start + (hb_codepoint_t) अङ्कः_glyphs - gid;

      mapping->alloc (mapping->get_population () + end - start + 1);

      unicodes->add_range (start, end);
      प्रति (unsigned cp = start; cp <= end; cp++)
      {
	mapping->set (cp, gid);
        gid += T::increment;
      }
    }
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) && groups.sanitize (c));
  }

  रक्षित:
  HBUINT16	format;		/* Subtable format; set to 12. */
  HBUINT16	reserved;	/* Reserved; set to 0. */
  HBUINT32	length;		/* Byte length of this subtable. */
  HBUINT32	language;	/* Ignore. */
  SortedArray32Of<CmapSubtableLongGroup>
		groups;		/* Groupings. */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (16, groups);
};

संरचना CmapSubtableFormat12 : CmapSubtableLongSegmented<CmapSubtableFormat12>
{
  स्थिर constexpr पूर्णाङ्क increment = 1;
  स्थिर constexpr पूर्णाङ्क format_सङ्ख्या = 12;

  स्थिर hb_codepoint_t group_get_glyph (ध्रुव CmapSubtableLongGroup &group,
					 hb_codepoint_t u)
  { प्रत्यागम likely (group.startCharCode <= group.endCharCode) ?
	   group.glyphID + (u - group.startCharCode) : 0; }


  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  शून्य serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    जइ (!it) प्रत्यागम;
    unsigned table_initpos = c->length ();
    जइ (unlikely (!c->extend_min (एतत्))) प्रत्यागम;

    hb_codepoint_t startCharCode = (hb_codepoint_t) -1, endCharCode = (hb_codepoint_t) -1;
    hb_codepoint_t glyphID = 0;

    प्रति (ध्रुव auto& _ : +it)
    {
      जइ (startCharCode == (hb_codepoint_t) -1)
      {
	startCharCode = _.first;
	endCharCode = _.first;
	glyphID = _.second;
      }
      अण्णहा जइ (!_is_gid_consecutive (endCharCode, startCharCode, glyphID, _.first, _.second))
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
      अण्णहा
	endCharCode = _.first;
    }

    CmapSubtableLongGroup record;
    record.startCharCode = startCharCode;
    record.endCharCode = endCharCode;
    record.glyphID = glyphID;
    c->copy<CmapSubtableLongGroup> (record);

    एतत्->format = 12;
    एतत्->reserved = 0;
    एतत्->length = c->length () - table_initpos;
    एतत्->groups.len = (एतत्->length - min_size) / CmapSubtableLongGroup::static_size;
  }

  स्थिर size_t get_sub_table_size (ध्रुव hb_sorted_vector_t<CmapSubtableLongGroup> &groups_data)
  { प्रत्यागम 16 + 12 * groups_data.length; }

  गुप्त:
  स्थिर तार्किक _is_gid_consecutive (hb_codepoint_t endCharCode,
				   hb_codepoint_t startCharCode,
				   hb_codepoint_t glyphID,
				   hb_codepoint_t cp,
				   hb_codepoint_t new_gid)
  {
    प्रत्यागम (cp - 1 == endCharCode) &&
	new_gid == glyphID + (cp - startCharCode);
  }

};

संरचना CmapSubtableFormat13 : CmapSubtableLongSegmented<CmapSubtableFormat13>
{
  स्थिर constexpr पूर्णाङ्क increment = 0;
  स्थिर constexpr पूर्णाङ्क format_सङ्ख्या = 13;

  स्थिर hb_codepoint_t group_get_glyph (ध्रुव CmapSubtableLongGroup &group,
					 hb_codepoint_t u HB_UNUSED)
  { प्रत्यागम group.glyphID; }
};

प्रकारनिर्देश गणना
{
  GLYPH_VARIANT_NOT_FOUND = 0,
  GLYPH_VARIANT_FOUND = 1,
  GLYPH_VARIANT_USE_DEFAULT = 2
} glyph_variant_t;

संरचना Unicode_मानम्_Range
{
  पूर्णाङ्क cmp (ध्रुव hb_codepoint_t &codepoint) ध्रुव
  {
    जइ (codepoint < start_Unicode_मानम्) प्रत्यागम -1;
    जइ (codepoint > start_Unicode_मानम् + additional_गणनम्) प्रत्यागम +1;
    प्रत्यागम 0;
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्));
  }

  HBUINT24	start_Unicode_मानम्;	/* First value in this range. */
  HBUINT8	additional_गणनम्;	/* Number of additional values in this
					 * range. */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (4);
};

संरचना DefaultUVS : SortedArray32Of<Unicode_मानम्_Range>
{
  शून्य collect_unicodes (hb_set_t *out) ध्रुव
  {
    unsigned पूर्णाङ्क गणनम् = len;
    प्रति (unsigned पूर्णाङ्क i = 0; i < गणनम्; i++)
    {
      hb_codepoint_t first = arrayZ[i].start_Unicode_मानम्;
      hb_codepoint_t last = hb_min ((hb_codepoint_t) (first + arrayZ[i].additional_गणनम्),
				    (hb_codepoint_t) HB_UNICODE_MAX);
      out->add_range (first, last);
    }
  }

  DefaultUVS* copy (hb_serialize_context_t *c,
		    ध्रुव hb_set_t *unicodes) ध्रुव
  {
    auto *out = c->start_embed<DefaultUVS> ();
    auto snap = c->snapshot ();

    HBUINT32 len;
    len = 0;
    जइ (unlikely (!c->copy<HBUINT32> (len))) प्रत्यागम nullptr;
    unsigned init_len = c->length ();

    जइ (एतत्->len > unicodes->get_population () * hb_bit_storage ((unsigned) एतत्->len))
    {
      hb_codepoint_t start = HB_SET_मानम्_INVALID;
      hb_codepoint_t end = HB_SET_मानम्_INVALID;

      प्रति (auto u : *unicodes)
      {
        जइ (!as_array ().bsearch (u))
	  अनुवर्त;
	जइ (start == HB_SET_मानम्_INVALID)
	{
	  start = u;
	  end = start - 1;
	}
	जइ (end + 1 != u || end - start == 255)
        {
	  Unicode_मानम्_Range rec;
	  rec.start_Unicode_मानम् = start;
	  rec.additional_गणनम् = end - start;
	  c->copy<Unicode_मानम्_Range> (rec);
	  start = u;
	}
	end = u;
      }
      जइ (start != HB_SET_मानम्_INVALID)
      {
	Unicode_मानम्_Range rec;
	rec.start_Unicode_मानम् = start;
	rec.additional_गणनम् = end - start;
	c->copy<Unicode_मानम्_Range> (rec);
      }

    }
    अण्णहा
    {
      hb_codepoint_t lastCode = HB_SET_मानम्_INVALID;
      पूर्णाङ्क गणनम् = -1;

      प्रति (ध्रुव Unicode_मानम्_Range& _ : *एतत्)
      {
	hb_codepoint_t curEntry = (hb_codepoint_t) (_.start_Unicode_मानम् - 1);
	hb_codepoint_t end = curEntry + _.additional_गणनम् + 2;

	प्रति (; unicodes->next (&curEntry) && curEntry < end;)
	{
	  गणनम् += 1;
	  जइ (lastCode == HB_SET_मानम्_INVALID)
	    lastCode = curEntry;
	  अण्णहा जइ (lastCode + गणनम् != curEntry || गणनम् > 255)
	  {
	    Unicode_मानम्_Range rec;
	    rec.start_Unicode_मानम् = lastCode;
	    rec.additional_गणनम् = गणनम् - 1;
	    c->copy<Unicode_मानम्_Range> (rec);

	    lastCode = curEntry;
	    गणनम् = 0;
	  }
	}
      }

      जइ (lastCode != HB_MAP_मानम्_INVALID)
      {
	Unicode_मानम्_Range rec;
	rec.start_Unicode_मानम् = lastCode;
	rec.additional_गणनम् = गणनम्;
	c->copy<Unicode_मानम्_Range> (rec);
      }
    }

    जइ (c->length () - init_len == 0)
    {
      c->revert (snap);
      प्रत्यागम nullptr;
    }
    अण्णहा
    {
      जइ (unlikely (!c->check_assign (out->len,
                                      (c->length () - init_len) / Unicode_मानम्_Range::static_size,
                                      HB_SERIALIZE_ERROR_INT_OVERFLOW))) प्रत्यागम nullptr;
      प्रत्यागम out;
    }
  }

  सार्वजनिक:
  DEFINE_SIZE_ARRAY (4, *एतत्);
};

template <typename Types>
संरचना UVSMapping
{
  using GlyphID = typename Types::HBGlyphID;

  पूर्णाङ्क cmp (ध्रुव hb_codepoint_t &codepoint) ध्रुव
  { प्रत्यागम unicode_मानम्.cmp (codepoint); }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्));
  }

  HBUINT24	unicode_मानम्;	/* Base Unicode value of the UVS */
  GlyphID
		glyphID;	/* Glyph ID of the UVS. */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (3 + GlyphID::static_size);
};

template <typename Types>
संरचना NonDefaultUVS : SortedArray32Of<UVSMapping<Types>>
{
  using Mapping = UVSMapping<Types>;

  शून्य collect_unicodes (hb_set_t *out) ध्रुव
  {
    प्रति (ध्रुव auto& a : एतत्->as_array ())
      out->add (a.unicode_मानम्);
  }

  शून्य collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping /* OUT */) ध्रुव
  {
    प्रति (ध्रुव auto& a : एतत्->as_array ())
    {
      hb_codepoint_t unicode = a.unicode_मानम्;
      hb_codepoint_t glyphid = a.glyphID;
      unicodes->add (unicode);
      mapping->set (unicode, glyphid);
    }
  }

  शून्य closure_glyphs (ध्रुव hb_set_t      *unicodes,
		       hb_set_t            *glyphset) ध्रुव
  {
    + एतत्->as_array ()
    | hb_filter (unicodes, &Mapping::unicode_मानम्)
    | hb_map (&Mapping::glyphID)
    | hb_sink (glyphset)
    ;
  }

  NonDefaultUVS* copy (hb_serialize_context_t *c,
		       ध्रुव hb_set_t *unicodes,
		       ध्रुव hb_set_t *glyphs_requested,
		       ध्रुव hb_map_t *glyph_map) ध्रुव
  {
    auto *out = c->start_embed<NonDefaultUVS> ();
    auto it =
    + एतत्->as_array ()
    | hb_filter ([&] (ध्रुव Mapping& _)
		 {
		   प्रत्यागम unicodes->has (_.unicode_मानम्) || glyphs_requested->has (_.glyphID);
		 })
    ;

    जइ (!it) प्रत्यागम nullptr;

    HBUINT32 len;
    len = it.len ();
    जइ (unlikely (!c->copy<HBUINT32> (len))) प्रत्यागम nullptr;

    प्रति (ध्रुव Mapping& _ : it)
    {
      Mapping mapping;
      mapping.unicode_मानम् = _.unicode_मानम्;
      mapping.glyphID = glyph_map->get (_.glyphID);
      c->copy<Mapping> (mapping);
    }

    प्रत्यागम out;
  }

  सार्वजनिक:
  DEFINE_SIZE_ARRAY (4, *एतत्);
};

template <typename Types>
संरचना VariationSelectorRecord
{
  glyph_variant_t get_glyph (hb_codepoint_t codepoint,
			     hb_codepoint_t *glyph,
			     ध्रुव शून्य *base) ध्रुव
  {
    जइ ((base+defaultUVS).bfind (codepoint))
      प्रत्यागम GLYPH_VARIANT_USE_DEFAULT;
    ध्रुव UVSMapping<Types> &nonDefault = (base+nonDefaultUVS).bsearch (codepoint);
    जइ (nonDefault.glyphID)
    {
      *glyph = nonDefault.glyphID;
       प्रत्यागम GLYPH_VARIANT_FOUND;
    }
    प्रत्यागम GLYPH_VARIANT_NOT_FOUND;
  }

  VariationSelectorRecord(ध्रुव VariationSelectorRecord& other)
  {
    *एतत् = other;
  }

  शून्य operator= (ध्रुव VariationSelectorRecord& other)
  {
    varSelector = other.varSelector;
    HBUINT32 offset = other.defaultUVS;
    defaultUVS = offset;
    offset = other.nonDefaultUVS;
    nonDefaultUVS = offset;
  }

  शून्य collect_unicodes (hb_set_t *out, ध्रुव शून्य *base) ध्रुव
  {
    (base+defaultUVS).collect_unicodes (out);
    (base+nonDefaultUVS).collect_unicodes (out);
  }

  शून्य collect_mapping (ध्रुव शून्य *base,
			hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping /* OUT */) ध्रुव
  {
    (base+defaultUVS).collect_unicodes (unicodes);
    (base+nonDefaultUVS).collect_mapping (unicodes, mapping);
  }

  पूर्णाङ्क cmp (ध्रुव hb_codepoint_t &variation_selector) ध्रुव
  { प्रत्यागम varSelector.cmp (variation_selector); }

  तार्किक sanitize (hb_sanitize_context_t *c, ध्रुव शून्य *base) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
		  defaultUVS.sanitize (c, base) &&
		  nonDefaultUVS.sanitize (c, base));
  }

  hb_pair_t<unsigned, unsigned>
  copy (hb_serialize_context_t *c,
	ध्रुव hb_set_t *unicodes,
	ध्रुव hb_set_t *glyphs_requested,
	ध्रुव hb_map_t *glyph_map,
	ध्रुव शून्य *base) ध्रुव
  {
    auto snap = c->snapshot ();
    auto *out = c->embed<VariationSelectorRecord> (*एतत्);
    जइ (unlikely (!out)) प्रत्यागम hb_pair (0, 0);

    out->defaultUVS = 0;
    out->nonDefaultUVS = 0;

    unsigned non_default_uvs_objidx = 0;
    जइ (nonDefaultUVS != 0)
    {
      c->push ();
      जइ (c->copy (base+nonDefaultUVS, unicodes, glyphs_requested, glyph_map))
	non_default_uvs_objidx = c->निष्कासय_pack ();
      अण्णहा c->निष्कासय_discard ();
    }

    unsigned default_uvs_objidx = 0;
    जइ (defaultUVS != 0)
    {
      c->push ();
      जइ (c->copy (base+defaultUVS, unicodes))
	default_uvs_objidx = c->निष्कासय_pack ();
      अण्णहा c->निष्कासय_discard ();
    }


    जइ (!default_uvs_objidx && !non_default_uvs_objidx)
      c->revert (snap);

    प्रत्यागम hb_pair (default_uvs_objidx, non_default_uvs_objidx);
  }

  HBUINT24	varSelector;	/* Variation selector. */
  Offset32To<DefaultUVS>
		defaultUVS;	/* Offset to Default UVS Table.  May be 0. */
  Offset32To<NonDefaultUVS<Types>>
		nonDefaultUVS;	/* Offset to Non-Default UVS Table.  May be 0. */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (11);
};

template <typename Types>
संरचना CmapSubtableFormat14_15
{
  स्थिर constexpr unsigned format_मानम् = Types::size == 2 ? 14 : 15;
  using Record = VariationSelectorRecord<Types>;
  using NonDefault = NonDefaultUVS<Types>;

  glyph_variant_t get_glyph_variant (hb_codepoint_t codepoint,
				     hb_codepoint_t variation_selector,
				     hb_codepoint_t *glyph) ध्रुव
  { प्रत्यागम record.bsearch (variation_selector).get_glyph (codepoint, glyph, एतत्); }

  शून्य collect_variation_selectors (hb_set_t *out) ध्रुव
  {
    प्रति (ध्रुव auto& a : record.as_array ())
      out->add (a.varSelector);
  }
  शून्य collect_variation_unicodes (hb_codepoint_t variation_selector,
				   hb_set_t *out) ध्रुव
  { record.bsearch (variation_selector).collect_unicodes (out, एतत्); }

  शून्य serialize (hb_serialize_context_t *c,
		  ध्रुव hb_set_t *unicodes,
		  ध्रुव hb_set_t *glyphs_requested,
		  ध्रुव hb_map_t *glyph_map,
		  ध्रुव शून्य *base)
  {
    auto snap = c->snapshot ();
    unsigned table_initpos = c->length ();
    ध्रुव वर्ण* init_tail = c->tail;

    जइ (unlikely (!c->extend_min (एतत्))) प्रत्यागम;
    एतत्->format = format_मानम्;

    auto src_tbl = reinterpret_cast<ध्रुव CmapSubtableFormat14_15*> (base);

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
    प्रति (पूर्णाङ्क i = src_tbl->record.len - 1; i >= 0; i--)
    {
      जइ (!unicodes->has(src_tbl->record[i].varSelector))
        अनुवर्त;

      hb_pair_t<unsigned, unsigned> result = src_tbl->record[i].copy (c, unicodes, glyphs_requested, glyph_map, base);
      जइ (result.first || result.second)
	obj_indices.push (result);
    }

    जइ (c->length () - table_initpos == CmapSubtableFormat14_15::min_size)
    {
      c->revert (snap);
      प्रत्यागम;
    }

    जइ (unlikely (!c->check_success (!obj_indices.in_error ())))
      प्रत्यागम;

    पूर्णाङ्क tail_len = init_tail - c->tail;
    c->check_assign (एतत्->length, c->length () - table_initpos + tail_len,
                     HB_SERIALIZE_ERROR_INT_OVERFLOW);
    c->check_assign (एतत्->record.len,
		     (c->length () - table_initpos - CmapSubtableFormat14_15::min_size) /
		     Record::static_size,
                     HB_SERIALIZE_ERROR_INT_OVERFLOW);

    /* Correct the incorrect write order by reversing the order of the variation
       records array. */
    _reverse_variation_records ();

    /* Now that records are in the right order, we can set up the offsets. */
    _add_links_to_variation_records (c, obj_indices);
  }

  शून्य _reverse_variation_records ()
  {
    record.as_array ().reverse ();
  }

  शून्य _add_links_to_variation_records (hb_serialize_context_t *c,
					ध्रुव hb_vector_t<hb_pair_t<unsigned, unsigned>>& obj_indices)
  {
    प्रति (unsigned i = 0; i < obj_indices.length; i++)
    {
      /*
       * Since the record array has been reversed (see comments in copy())
       * but obj_indices has not been, the indices at obj_indices[i]
       * are for the variation record at record[j].
       */
      पूर्णाङ्क j = obj_indices.length - 1 - i;
      c->add_link (record[j].defaultUVS, obj_indices[i].first);
      c->add_link (record[j].nonDefaultUVS, obj_indices[i].second);
    }
  }

  शून्य closure_glyphs (ध्रुव hb_set_t      *unicodes,
		       hb_set_t            *glyphset) ध्रुव
  {
    + hb_iter (record)
    | hb_filter (hb_bool, &Record::nonDefaultUVS)
    | hb_filter (unicodes, &Record::varSelector)
    | hb_map (&Record::nonDefaultUVS)
    | hb_map (hb_add (एतत्))
    | hb_apply ([=] (ध्रुव NonDefault& _) { _.closure_glyphs (unicodes, glyphset); })
    ;
  }

  शून्य collect_unicodes (hb_set_t *out) ध्रुव
  {
    प्रति (ध्रुव Record& _ : record)
      _.collect_unicodes (out, एतत्);
  }

  शून्य collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping /* OUT */) ध्रुव
  {
    प्रति (ध्रुव Record& _ : record)
      _.collect_mapping (एतत्, unicodes, mapping);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
		  record.sanitize (c, एतत्));
  }

  रक्षित:
  HBUINT16	format;		/* Format number is set to 14 or 15. */
  HBUINT32	length;		/* Byte length of this subtable. */
  SortedArray32Of<Record>
		record;		/* Variation selector records; sorted
				 * in increasing order of `varSelector'. */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (10, record);
};

संरचना CmapSubtable
{
  /* Note: We intentionally do NOT implement subtable formats 2 and 8. */

  तार्किक is_variation_selector () ध्रुव
  {
    विकल्प (u.format.v) {
    पक्ष 14:
#ifndef HB_NO_BEYOND_64K
    पक्ष 15:
#endif
      प्रत्यागम सच्च;
    स्वभाव:
      प्रत्यागम मिच्छ;
    }
  }

  glyph_variant_t get_glyph_variant (hb_codepoint_t codepoint,
				     hb_codepoint_t variation_selector,
				     hb_codepoint_t *glyph) ध्रुव
  {
    विकल्प (u.format.v) {
    पक्ष 14: hb_barrier (); प्रत्यागम u.format14.get_glyph_variant (codepoint, variation_selector, glyph);
#ifndef HB_NO_BEYOND_64K
    पक्ष 15: hb_barrier (); प्रत्यागम u.format15.get_glyph_variant (codepoint, variation_selector, glyph);
#endif
    स्वभाव: प्रत्यागम GLYPH_VARIANT_NOT_FOUND;
    }
  }

  शून्य collect_variation_selectors (hb_set_t *out) ध्रुव
  {
    विकल्प (u.format.v) {
    पक्ष 14: hb_barrier (); u.format14.collect_variation_selectors (out); प्रत्यागम;
#ifndef HB_NO_BEYOND_64K
    पक्ष 15: hb_barrier (); u.format15.collect_variation_selectors (out); प्रत्यागम;
#endif
    स्वभाव: प्रत्यागम;
    }
  }

  शून्य collect_variation_unicodes (hb_codepoint_t variation_selector,
				    hb_set_t *out) ध्रुव
  {
    विकल्प (u.format.v) {
    पक्ष 14: hb_barrier (); u.format14.collect_variation_unicodes (variation_selector, out); प्रत्यागम;
#ifndef HB_NO_BEYOND_64K
    पक्ष 15: hb_barrier (); u.format15.collect_variation_unicodes (variation_selector, out); प्रत्यागम;
#endif
    स्वभाव: प्रत्यागम;
    }
  }

  शून्य closure_glyphs (ध्रुव hb_set_t *unicodes, hb_set_t *glyphset) ध्रुव
  {
    विकल्प (u.format.v) {
    पक्ष 14: hb_barrier (); u.format14.closure_glyphs (unicodes, glyphset); प्रत्यागम;
#ifndef HB_NO_BEYOND_64K
    पक्ष 15: hb_barrier (); u.format15.closure_glyphs (unicodes, glyphset); प्रत्यागम;
#endif
    स्वभाव: प्रत्यागम;
    }
  }

  तार्किक get_glyph (hb_codepoint_t codepoint,
		  hb_codepoint_t *glyph,
		  unsigned पूर्णाङ्क subtable_data_size = 0) ध्रुव
  {
    विकल्प (u.format.v) {
    पक्ष  0: hb_barrier (); प्रत्यागम u.format0 .get_glyph (codepoint, glyph);
    पक्ष  4: hb_barrier (); प्रत्यागम subtable_data_size
				      ? u.format4.get_glyph (codepoint, glyph, subtable_data_size)
				      : मिच्छ;
    पक्ष  6: hb_barrier (); प्रत्यागम u.format6 .get_glyph (codepoint, glyph);
    पक्ष 10: hb_barrier (); प्रत्यागम u.format10.get_glyph (codepoint, glyph);
    पक्ष 12: hb_barrier (); प्रत्यागम u.format12.get_glyph (codepoint, glyph);
    पक्ष 13: hb_barrier (); प्रत्यागम u.format13.get_glyph (codepoint, glyph);
    पक्ष 14:
    स्वभाव: प्रत्यागम मिच्छ;
    }
  }
  शून्य collect_unicodes (hb_set_t *out,
			 unsigned पूर्णाङ्क अङ्कः_glyphs = UINT_MAX,
			 unsigned पूर्णाङ्क subtable_data_size = 0) ध्रुव
  {
    विकल्प (u.format.v) {
    पक्ष  0: hb_barrier (); u.format0 .collect_unicodes (out); प्रत्यागम;
    पक्ष  4: hb_barrier (); जइ (subtable_data_size)
			      u.format4.collect_unicodes (out, subtable_data_size);
			    अण्णहा
			      u.format4.collect_unicodes (out);
			    प्रत्यागम;
    पक्ष  6: hb_barrier (); u.format6 .collect_unicodes (out); प्रत्यागम;
    पक्ष 10: hb_barrier (); u.format10.collect_unicodes (out); प्रत्यागम;
    पक्ष 12: hb_barrier (); u.format12.collect_unicodes (out, अङ्कः_glyphs); प्रत्यागम;
    पक्ष 13: hb_barrier (); u.format13.collect_unicodes (out, अङ्कः_glyphs); प्रत्यागम;
    पक्ष 14:
    स्वभाव: प्रत्यागम;
    }
  }

  शून्य collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping, /* OUT */
			unsigned अङ्कः_glyphs = UINT_MAX,
			unsigned पूर्णाङ्क subtable_data_size = 0) ध्रुव
  {
    विकल्प (u.format.v) {
    पक्ष  0: hb_barrier (); u.format0 .collect_mapping (unicodes, mapping); प्रत्यागम;
    पक्ष  4: hb_barrier (); जइ (subtable_data_size)
			      u.format4.collect_mapping (unicodes, mapping, subtable_data_size);
			    अण्णहा
			      u.format4.collect_mapping (unicodes, mapping);
			    प्रत्यागम;
    पक्ष  6: hb_barrier (); u.format6 .collect_mapping (unicodes, mapping); प्रत्यागम;
    पक्ष 10: hb_barrier (); u.format10.collect_mapping (unicodes, mapping); प्रत्यागम;
    पक्ष 12: hb_barrier (); u.format12.collect_mapping (unicodes, mapping, अङ्कः_glyphs); प्रत्यागम;
    पक्ष 13: hb_barrier (); u.format13.collect_mapping (unicodes, mapping, अङ्कः_glyphs); प्रत्यागम;
    पक्ष 14:
    स्वभाव: प्रत्यागम;
    }
  }

  unsigned get_language () ध्रुव
  {
    विकल्प (u.format.v) {
    पक्ष  0: hb_barrier (); प्रत्यागम u.format0 .get_language ();
    पक्ष  4: hb_barrier (); प्रत्यागम u.format4 .get_language ();
    पक्ष  6: hb_barrier (); प्रत्यागम u.format6 .get_language ();
    पक्ष 10: hb_barrier (); प्रत्यागम u.format10.get_language ();
    पक्ष 12: hb_barrier (); प्रत्यागम u.format12.get_language ();
    पक्ष 13: hb_barrier (); प्रत्यागम u.format13.get_language ();
    पक्ष 14:
    स्वभाव: प्रत्यागम 0;
    }
  }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  शून्य serialize (hb_serialize_context_t *c,
		  Iterator it,
		  unsigned format,
		  ध्रुव hb_subset_plan_t *plan,
		  ध्रुव शून्य *base)
  {
    विकल्प (format) {
    पक्ष  4: hb_barrier (); प्रत्यागम u.format4.serialize (c, it);
    पक्ष 12: hb_barrier (); प्रत्यागम u.format12.serialize (c, it);
    पक्ष 14: hb_barrier (); प्रत्यागम u.format14.serialize (c, &plan->unicodes, &plan->glyphs_requested, plan->glyph_map, base);
#ifndef HB_NO_BEYOND_64K
    पक्ष 15: hb_barrier (); प्रत्यागम u.format15.serialize (c, &plan->unicodes, &plan->glyphs_requested, plan->glyph_map, base);
#endif
    स्वभाव: प्रत्यागम;
    }
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    जइ (!u.format.v.sanitize (c)) return_trace (मिच्छ);
    hb_barrier ();
    विकल्प (u.format.v) {
    पक्ष  0: hb_barrier (); return_trace (u.format0 .sanitize (c));
    पक्ष  4: hb_barrier (); return_trace (u.format4 .sanitize (c));
    पक्ष  6: hb_barrier (); return_trace (u.format6 .sanitize (c));
    पक्ष 10: hb_barrier (); return_trace (u.format10.sanitize (c));
    पक्ष 12: hb_barrier (); return_trace (u.format12.sanitize (c));
    पक्ष 13: hb_barrier (); return_trace (u.format13.sanitize (c));
    पक्ष 14: hb_barrier (); return_trace (u.format14.sanitize (c));
#ifndef HB_NO_BEYOND_64K
    पक्ष 15: hb_barrier (); return_trace (u.format15.sanitize (c));
#endif
    स्वभाव:return_trace (सच्च);
    }
  }

  सार्वजनिक:
  सङ्घ {
  संरचना { HBUINT16 v; }	format;		/* Format identifier */
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
  सार्वजनिक:
  DEFINE_SIZE_UNION (2, format.v);
};


संरचना EncodingRecord
{
  पूर्णाङ्क cmp (ध्रुव EncodingRecord &other) ध्रुव
  {
    पूर्णाङ्क ret;
    ret = platformID.cmp (other.platformID);
    जइ (ret) प्रत्यागम ret;
    जइ (other.encodingID != 0xFFFF)
    {
      ret = encodingID.cmp (other.encodingID);
      जइ (ret) प्रत्यागम ret;
    }
    प्रत्यागम 0;
  }

  तार्किक sanitize (hb_sanitize_context_t *c, ध्रुव शून्य *base) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
		  subtable.sanitize (c, base));
  }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  EncodingRecord* copy (hb_serialize_context_t *c,
			Iterator it,
			unsigned format,
			ध्रुव शून्य *base,
			ध्रुव hb_subset_plan_t *plan,
			/* INOUT */ unsigned *objidx) ध्रुव
  {
    TRACE_SERIALIZE (एतत्);
    auto snap = c->snapshot ();
    auto *out = c->embed (एतत्);
    जइ (unlikely (!out)) return_trace (nullptr);
    out->subtable = 0;

    जइ (*objidx == 0)
    {
      CmapSubtable *cmapsubtable = c->push<CmapSubtable> ();
      unsigned origin_length = c->length ();
      cmapsubtable->serialize (c, it, format, plan, &(base+subtable));
      जइ (c->length () > origin_length && !c->in_error()) *objidx = c->निष्कासय_pack ();
      अण्णहा c->निष्कासय_discard ();
    }

    जइ (*objidx == 0)
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
  सार्वजनिक:
  DEFINE_SIZE_STATIC (8);
};

संरचना cmap;

संरचना SubtableUnicodesCache {

 गुप्त:
  hb_blob_ptr_t<cmap> base_blob;
  ध्रुव वर्ण* base;
  unsigned पूर्णाङ्क table_length;
  hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> cached_unicodes;

 सार्वजनिक:

  स्थिर SubtableUnicodesCache* create (hb_blob_ptr_t<cmap> source_table)
  {
    SubtableUnicodesCache* cache =
        (SubtableUnicodesCache*) hb_malloc (परिमाण(SubtableUnicodesCache));

    जइ (unlikely (!cache))
      प्रत्यागम nullptr;

    नूतन (cache) SubtableUnicodesCache (source_table);
    प्रत्यागम cache;
  }

  स्थिर शून्य destroy (शून्य* value) {
    जइ (!value) प्रत्यागम;

    SubtableUnicodesCache* cache = (SubtableUnicodesCache*) value;
    cache->~SubtableUnicodesCache ();
    hb_free (cache);
  }

  SubtableUnicodesCache(ध्रुव शून्य* cmap_base,
			unsigned पूर्णाङ्क table_length_ = 0)
      : base_blob(),
        base ((ध्रुव वर्ण*) cmap_base),
        table_length (table_length_),
        cached_unicodes ()
  {}

  SubtableUnicodesCache(hb_blob_ptr_t<cmap> base_blob_)
      : base_blob(base_blob_),
        base ((ध्रुव वर्ण *) base_blob.get()),
        table_length (base_blob.get_length ()),
        cached_unicodes ()
  {}

  ~SubtableUnicodesCache()
  {
    base_blob.destroy ();
  }

  तार्किक same_base(ध्रुव शून्य* other) ध्रुव
  {
    प्रत्यागम other == (ध्रुव शून्य*) base;
  }

  ध्रुव hb_set_t* set_for (ध्रुव EncodingRecord* record,
                           SubtableUnicodesCache& mutable_cache) ध्रुव
  {
    जइ (cached_unicodes.has ((unsigned) ((ध्रुव वर्ण *) record - base)))
      प्रत्यागम cached_unicodes.get ((unsigned) ((ध्रुव वर्ण *) record - base));

    प्रत्यागम mutable_cache.set_for (record);
  }

  ध्रुव hb_set_t* set_for (ध्रुव EncodingRecord* record)
  {
    जइ (!cached_unicodes.has ((unsigned) ((ध्रुव वर्ण *) record - base)))
    {
      hb_set_t *s = hb_set_create ();
      जइ (unlikely (s->in_error ()))
	प्रत्यागम hb_set_get_empty ();

      unsigned पूर्णाङ्क subtable_data_size = record->subtable < table_length
					? table_length - (unsigned पूर्णाङ्क) record->subtable
					: 0;
      (base+record->subtable).collect_unicodes (s, UINT_MAX, subtable_data_size);

      जइ (unlikely (!cached_unicodes.set ((unsigned) ((ध्रुव वर्ण *) record - base), hb::unique_ptr<hb_set_t> {s})))
        प्रत्यागम hb_set_get_empty ();

      प्रत्यागम s;
    }
    प्रत्यागम cached_unicodes.get ((unsigned) ((ध्रुव वर्ण *) record - base));
  }

};

स्थिर अन्तःस्थ uint16_t
hb_चिह्नम्_pua_map (unsigned codepoint)
{
  जइ (codepoint <= 0x00FFu)
  {
    /* For symbol-encoded OpenType fonts, we duplicate the
     * U+F000..F0FF range at U+0000..U+00FF.  That's what
     * Windows seems to do, and that's hinted about at:
     * https://docs.microsoft.com/en-us/typography/opentype/spec/recom
     * under "Non-Standard (Symbol) Fonts". */
    प्रत्यागम 0xF000u + codepoint;
  }
  प्रत्यागम 0;
}

संरचना cmap
{
  स्थिर constexpr hb_tag_t tableTag = HB_OT_TAG_cmap;


  स्थिर SubtableUnicodesCache* create_filled_cache(hb_blob_ptr_t<cmap> source_table) {
    ध्रुव cmap* cmap = source_table.get();
    auto it =
    + hb_iter (cmap->encodingRecord)
    | hb_filter ([&](ध्रुव EncodingRecord& _) {
      प्रत्यागम cmap::filter_encoding_records_for_subset (cmap, _);
    })
    ;

    SubtableUnicodesCache* cache = SubtableUnicodesCache::create(source_table);

    जइ (unlikely (!cache))
      प्रत्यागम nullptr;

    प्रति (ध्रुव EncodingRecord& _ : it)
      cache->set_for(&_); // populate the cache for this encoding record.

    प्रत्यागम cache;
  }

  template<typename Iterator, typename EncodingRecIter,
	   hb_requires (hb_is_iterator (EncodingRecIter))>
  तार्किक serialize (hb_serialize_context_t *c,
		  Iterator it,
		  EncodingRecIter encodingrec_iter,
		  ध्रुव शून्य *base,
		  hb_subset_plan_t *plan,
	                  तार्किक drop_format_4 = मिच्छ,
			  unsigned पूर्णाङ्क source_table_length = 0)
  {
    जइ (unlikely (!c->extend_min ((*एतत्))))  प्रत्यागम मिच्छ;
    एतत्->version = 0;

    unsigned format4objidx = 0, format12objidx = 0, format14objidx = 0;
#ifndef HB_NO_BEYOND_64K
    unsigned format15objidx = 0;
#endif
    auto snap = c->snapshot ();

    SubtableUnicodesCache local_unicodes_cache (base, source_table_length);
    ध्रुव SubtableUnicodesCache* unicodes_cache = &local_unicodes_cache;

    जइ (plan->accelerator &&
        plan->accelerator->cmap_cache &&
        plan->accelerator->cmap_cache->same_base (base))
      unicodes_cache = plan->accelerator->cmap_cache;

    प्रति (ध्रुव EncodingRecord& _ : encodingrec_iter)
    {
      जइ (c->in_error ())
        प्रत्यागम मिच्छ;

      unsigned format = (base+_.subtable).u.format.v;
      जइ (format != 4 && format != 12 && format != 14
#ifndef HB_NO_BEYOND_64K
	  && format != 15
#endif
	 ) अनुवर्त;

      ध्रुव hb_set_t* unicodes_set = unicodes_cache->set_for (&_, local_unicodes_cache);

      जइ (!drop_format_4 && format == 4)
      {
        c->copy (_, + it | hb_filter (*unicodes_set, hb_first), 4u, base, plan, &format4objidx);
        जइ (c->in_error () && c->only_overflow ())
        {
          // cmap4 overflowed, reset and retry serialization without format 4 subtables.
          c->revert (snap);
          प्रत्यागम serialize (c, it,
                            encodingrec_iter,
                            base,
                            plan,
                            सच्च,
			    source_table_length);
        }
      }

      अण्णहा जइ (format == 12)
      {
        जइ (_can_drop (_,
                       *unicodes_set,
                       base,
                       *unicodes_cache,
                       local_unicodes_cache,
                       + it | hb_map (hb_first), encodingrec_iter))
          अनुवर्त;
        c->copy (_, + it | hb_filter (*unicodes_set, hb_first), 12u, base, plan, &format12objidx);
      }
      अण्णहा जइ (format == 14) c->copy (_, it, 14u, base, plan, &format14objidx);
#ifndef HB_NO_BEYOND_64K
      अण्णहा जइ (format == 15) c->copy (_, it, 15u, base, plan, &format15objidx);
#endif
    }
        unsigned length = c->length ();
        unsigned available = length > cmap::min_size ? length - cmap::min_size : 0;
        c->check_assign(एतत्->encodingRecord.len,
                        available / EncodingRecord::static_size,
                        HB_SERIALIZE_ERROR_INT_OVERFLOW);

    // Fail if format 4 was dropped and there is no cmap12.
    प्रत्यागम !drop_format_4 || format12objidx;
  }

  template<typename Iterator, typename EncodingRecordIterator,
      hb_requires (hb_is_iterator (Iterator)),
      hb_requires (hb_is_iterator (EncodingRecordIterator))>
  तार्किक _can_drop (ध्रुव EncodingRecord& cmap12,
                  ध्रुव hb_set_t& cmap12_unicodes,
                  ध्रुव शून्य* base,
                  ध्रुव SubtableUnicodesCache& unicodes_cache,
                  SubtableUnicodesCache& local_unicodes_cache,
                  Iterator subset_unicodes,
                  EncodingRecordIterator encoding_records)
  {
    प्रति (auto cp : + subset_unicodes | hb_filter (cmap12_unicodes))
    {
      जइ (cp >= 0x10000) प्रत्यागम मिच्छ;
    }

    unsigned target_platform;
    unsigned target_encoding;
    unsigned target_language = (base+cmap12.subtable).get_language ();

    जइ (cmap12.platformID == 0 && cmap12.encodingID == 4)
    {
      target_platform = 0;
      target_encoding = 3;
    } अण्णहा जइ (cmap12.platformID == 3 && cmap12.encodingID == 10) {
      target_platform = 3;
      target_encoding = 1;
    } अण्णहा {
      प्रत्यागम मिच्छ;
    }

    प्रति (ध्रुव auto& _ : encoding_records)
    {
      जइ (_.platformID != target_platform
          || _.encodingID != target_encoding
          || (base+_.subtable).get_language() != target_language)
        अनुवर्त;

      ध्रुव hb_set_t* sibling_unicodes = unicodes_cache.set_for (&_, local_unicodes_cache);

      auto cmap12 = + subset_unicodes | hb_filter (cmap12_unicodes);
      auto sibling = + subset_unicodes | hb_filter (*sibling_unicodes);
      प्रति (; cmap12 && sibling; cmap12++, sibling++)
      {
        unsigned a = *cmap12;
        unsigned b = *sibling;
        जइ (a != b) प्रत्यागम मिच्छ;
      }

      प्रत्यागम !cmap12 && !sibling;
    }

    प्रत्यागम मिच्छ;
  }

  शून्य closure_glyphs (ध्रुव hb_set_t      *unicodes,
		       hb_set_t            *glyphset) ध्रुव
  {
    + hb_iter (encodingRecord)
    | hb_map (&EncodingRecord::subtable)
    | hb_map (hb_add (एतत्))
    | hb_filter (&CmapSubtable::is_variation_selector)
    | hb_apply ([=] (ध्रुव CmapSubtable& _) { _.closure_glyphs (unicodes, glyphset); })
    ;
  }

  तार्किक subset (hb_subset_context_t *c) ध्रुव
  { प्रत्यागम subset (c, मिच्छ); }

  तार्किक subset (hb_subset_context_t *c, तार्किक is_dmap) ध्रुव
  {
    TRACE_SUBSET (एतत्);

    cmap *cmap_अविभाज्यः = c->serializer->start_embed<cmap> ();

    auto encodingrec_iter =
    + hb_iter (encodingRecord)
    | hb_filter ([&](ध्रुव EncodingRecord& _) {
      प्रत्यागम cmap::filter_encoding_records_for_subset (एतत्, _);
    })
    ;

    जइ (unlikely (!encodingrec_iter.len ())) return_trace (मिच्छ);

    ध्रुव EncodingRecord *unicode_bmp= nullptr, *unicode_ucs4 = nullptr, *ms_bmp = nullptr, *ms_ucs4 = nullptr;
    तार्किक has_format12 = मिच्छ;

    प्रति (ध्रुव EncodingRecord& _ : encodingrec_iter)
    {
      unsigned format = (एतत् + _.subtable).u.format.v;
      जइ (format == 12) has_format12 = सच्च;

      ध्रुव EncodingRecord *table = std::addressof (_);
      जइ      (_.platformID == 0 && _.encodingID ==  3) unicode_bmp = table;
      अण्णहा जइ (_.platformID == 0 && _.encodingID ==  4) unicode_ucs4 = table;
      अण्णहा जइ (_.platformID == 3 && _.encodingID ==  1) ms_bmp = table;
      अण्णहा जइ (_.platformID == 3 && _.encodingID == 10) ms_ucs4 = table;
    }

    जइ (unlikely (!is_dmap && !has_format12 && !unicode_bmp && !ms_bmp)) return_trace (मिच्छ);
    जइ (unlikely (!is_dmap && has_format12 && (!unicode_ucs4 && !ms_ucs4))) return_trace (मिच्छ);

    auto it =
    + c->plan->unicode_to_new_gid_list.iter ()
    | hb_filter ([&] (ध्रुव hb_codepoint_pair_t _)
		 { प्रत्यागम (_.second != HB_MAP_मानम्_INVALID); })
    ;

    return_trace (cmap_अविभाज्यः->serialize (c->serializer,
                                         it,
                                         encodingrec_iter,
                                         एतत्,
                                         c->plan,
					 मिच्छ,
					 c->source_blob->length));
  }

  ध्रुव CmapSubtable *find_best_subtable (तार्किक *चिह्नम् = nullptr,
					  तार्किक *mac = nullptr,
					  तार्किक *macroman = nullptr) ध्रुव
  {
    जइ (चिह्नम्) *चिह्नम् = मिच्छ;
    जइ (mac) *mac = मिच्छ;
    जइ (macroman) *macroman = मिच्छ;

    ध्रुव CmapSubtable *subtable;

    /* Symbol subtable.
     * Prefer symbol if available.
     * https://github.com/harfbuzz/harfbuzz/issues/1918 */
    जइ ((subtable = एतत्->find_subtable (3, 0)))
    {
      जइ (चिह्नम्) *चिह्नम् = सच्च;
      प्रत्यागम subtable;
    }

    /* 32-bit subtables. */
    जइ ((subtable = एतत्->find_subtable (3, 10))) प्रत्यागम subtable;
    जइ ((subtable = एतत्->find_subtable (0, 6))) प्रत्यागम subtable;
    जइ ((subtable = एतत्->find_subtable (0, 4))) प्रत्यागम subtable;

    /* 16-bit subtables. */
    जइ ((subtable = एतत्->find_subtable (3, 1))) प्रत्यागम subtable;
    जइ ((subtable = एतत्->find_subtable (0, 3))) प्रत्यागम subtable;
    जइ ((subtable = एतत्->find_subtable (0, 2))) प्रत्यागम subtable;
    जइ ((subtable = एतत्->find_subtable (0, 1))) प्रत्यागम subtable;
    जइ ((subtable = एतत्->find_subtable (0, 0))) प्रत्यागम subtable;

    /* MacRoman subtable. */
    जइ ((subtable = एतत्->find_subtable (1, 0)))
    {
      जइ (mac) *mac = सच्च;
      जइ (macroman) *macroman = सच्च;
      प्रत्यागम subtable;
    }
    /* Any other Mac subtable; we just map ASCII for these. */
    जइ ((subtable = एतत्->find_subtable (1, 0xFFFF)))
    {
      जइ (mac) *mac = सच्च;
      प्रत्यागम subtable;
    }

    /* Meh. */
    प्रत्यागम &Null (CmapSubtable);
  }

  संरचना accelerator_t
  {
    using cache_t = hb_cache_t<21, 19>;
    static_assert (परिमाण (cache_t) == 1024, "");

    accelerator_t (hb_face_t *face, hb_tag_t table_tag = cmap::tableTag)
    {
      एतत्->table = hb_sanitize_context_t ().reference_table<cmap> (face, table_tag);
      जइ (!एतत्->table.get_length ())
	प्रत्यागम;

      तार्किक चिह्नम्, mac, macroman;
      एतत्->subtable = table->find_best_subtable (&चिह्नम्, &mac, &macroman);
      एतत्->subtable_uvs = &Null (CmapSubtable);
      {
	ध्रुव CmapSubtable *st = table->find_variation_selector_subtable ();
	जइ (st)
	  subtable_uvs = st;
      }

#ifndef HB_NO_OT_FONT_CMAP_CACHE
      cache = (cache_t *) hb_malloc (परिमाण (cache_t));
      जइ (cache)
	नूतन (cache) cache_t ();
      अण्णहा
        प्रत्यागम; // Such that get_glyph_funcZ remains null.
#endif

      एतत्->get_glyph_data = subtable;
#ifndef HB_NO_CMAP_LEGACY_SUBTABLES
      तार्किक is_format4 = subtable->u.format.v == 4;
      auto set_format4_getter = [एतत्] (तार्किक (*func) (ध्रुव शून्य *,
						      hb_codepoint_t,
						      hb_codepoint_t *))
      {
	एतत्->format4_accel.init (&एतत्->subtable->u.format4,
				  get_subtable_data_size (एतत्->subtable));
	एतत्->get_glyph_data = &एतत्->format4_accel;
	एतत्->get_glyph_funcZ = func;
      };
      जइ (unlikely (चिह्नम्))
      {
	विकल्प ((unsigned) face->table.OS2->get_font_page ()) {
	पक्ष OS2::font_page_t::FONT_PAGE_NONE:
	  जइ (is_format4)
	    set_format4_getter (get_glyph_from_चिह्नम्<CmapSubtableFormat4::accelerator_t, hb_चिह्नम्_pua_map>);
	  अण्णहा
	    एतत्->get_glyph_funcZ = get_glyph_from_चिह्नम्<CmapSubtable, hb_चिह्नम्_pua_map>;
	  भङ्ग;
#ifndef HB_NO_OT_SHAPER_ARABIC_FALLBACK
	पक्ष OS2::font_page_t::FONT_PAGE_SIMP_ARABIC:
	  जइ (is_format4)
	    set_format4_getter (get_glyph_from_चिह्नम्<CmapSubtableFormat4::accelerator_t, _hb_arabic_pua_simp_map>);
	  अण्णहा
	    एतत्->get_glyph_funcZ = get_glyph_from_चिह्नम्<CmapSubtable, _hb_arabic_pua_simp_map>;
	  भङ्ग;
	पक्ष OS2::font_page_t::FONT_PAGE_TRAD_ARABIC:
	  जइ (is_format4)
	    set_format4_getter (get_glyph_from_चिह्नम्<CmapSubtableFormat4::accelerator_t, _hb_arabic_pua_trad_map>);
	  अण्णहा
	    एतत्->get_glyph_funcZ = get_glyph_from_चिह्नम्<CmapSubtable, _hb_arabic_pua_trad_map>;
	  भङ्ग;
#endif
	स्वभाव:
	  जइ (is_format4)
	    set_format4_getter (get_glyph_from<CmapSubtableFormat4::accelerator_t>);
	  अण्णहा
	    एतत्->get_glyph_funcZ = get_glyph_from<CmapSubtable>;
	  भङ्ग;
	}
      }
      अण्णहा जइ (unlikely (macroman))
      {
	जइ (is_format4)
	  set_format4_getter (get_glyph_from_macroman<CmapSubtableFormat4::accelerator_t>);
	अण्णहा
	  एतत्->get_glyph_funcZ = get_glyph_from_macroman<CmapSubtable>;
      }
      अण्णहा जइ (unlikely (mac))
      {
	जइ (is_format4)
	  set_format4_getter (get_glyph_from_ascii<CmapSubtableFormat4::accelerator_t>);
	अण्णहा
	  एतत्->get_glyph_funcZ = get_glyph_from_ascii<CmapSubtable>;
      }
      अण्णहा
#endif
      {
	विकल्प (subtable->u.format.v) {
	  /* Accelerate format 4 and format 12. */
	  स्वभाव:
	    एतत्->get_glyph_funcZ = get_glyph_from<CmapSubtable>;
	    भङ्ग;
	  पक्ष 12:
	    एतत्->get_glyph_funcZ = get_glyph_from<CmapSubtableFormat12>;
	    भङ्ग;
	  पक्ष  4:
	  {
	    hb_barrier ();
	    एतत्->format4_accel.init (&subtable->u.format4,
				      get_subtable_data_size (subtable));
	    एतत्->get_glyph_data = &एतत्->format4_accel;
	    एतत्->get_glyph_funcZ = एतत्->format4_accel.get_glyph_func;
	    भङ्ग;
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

    अन्तःस्थ तार्किक _cached_get (hb_codepoint_t unicode,
			     hb_codepoint_t *glyph) ध्रुव
    {
#ifndef HB_NO_OT_FONT_CMAP_CACHE
      // cache is always non-null if we have a get_glyph_funcZ
      unsigned v;
      जइ (cache->get (unicode, &v))
      {
        *glyph = v;
	प्रत्यागम सच्च;
      }
#endif
      तार्किक ret  = एतत्->get_glyph_funcZ (एतत्->get_glyph_data, unicode, glyph);

#ifndef HB_NO_OT_FONT_CMAP_CACHE
      जइ (ret)
        cache->set (unicode, *glyph);
#endif

      प्रत्यागम ret;
    }

    तार्किक get_nominal_glyph (hb_codepoint_t  unicode,
			    hb_codepoint_t *glyph) ध्रुव
    {
      जइ (unlikely (!एतत्->get_glyph_funcZ)) प्रत्यागम मिच्छ;
      प्रत्यागम _cached_get (unicode, glyph);
    }

    unsigned पूर्णाङ्क get_nominal_glyphs (unsigned पूर्णाङ्क गणनम्,
				     ध्रुव hb_codepoint_t *first_unicode,
				     unsigned पूर्णाङ्क unicode_stride,
				     hb_codepoint_t *first_glyph,
				     unsigned पूर्णाङ्क glyph_stride) ध्रुव
    {
      जइ (unlikely (!एतत्->get_glyph_funcZ)) प्रत्यागम 0;

      unsigned पूर्णाङ्क done;
      प्रति (done = 0;
	   done < गणनम् && _cached_get (*first_unicode, first_glyph);
	   done++)
      {
	first_unicode = &StructAtOffsetUnaligned<hb_codepoint_t> (first_unicode, unicode_stride);
	first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      }
      प्रत्यागम done;
    }

    तार्किक get_variation_glyph (hb_codepoint_t  unicode,
			      hb_codepoint_t  variation_selector,
			      hb_codepoint_t *glyph) ध्रुव
    {
      विकल्प (get_glyph_variant (unicode, variation_selector, glyph))
      {
	पक्ष GLYPH_VARIANT_NOT_FOUND:	प्रत्यागम मिच्छ;
	पक्ष GLYPH_VARIANT_FOUND:	प्रत्यागम सच्च;
	पक्ष GLYPH_VARIANT_USE_DEFAULT:	भङ्ग;
      }

      प्रत्यागम get_nominal_glyph (unicode, glyph);
    }

    glyph_variant_t get_glyph_variant (hb_codepoint_t  unicode,
				       hb_codepoint_t  variation_selector,
				       hb_codepoint_t *glyph) ध्रुव
    {
      प्रत्यागम एतत्->subtable_uvs->get_glyph_variant (unicode,
						    variation_selector,
						    glyph);
    }

    तार्किक has_data () ध्रुव { प्रत्यागम table.get_length (); }

    शून्य collect_unicodes (hb_set_t *out, unsigned पूर्णाङ्क अङ्कः_glyphs) ध्रुव
    { subtable->collect_unicodes (out, अङ्कः_glyphs, get_subtable_data_size (subtable)); }
    शून्य collect_mapping (hb_set_t *unicodes, hb_map_t *mapping,
			  unsigned अङ्कः_glyphs = UINT_MAX) ध्रुव
    { subtable->collect_mapping (unicodes, mapping, अङ्कः_glyphs, get_subtable_data_size (subtable)); }
    शून्य collect_variation_selectors (hb_set_t *out) ध्रुव
    { subtable_uvs->collect_variation_selectors (out); }
    शून्य collect_variation_unicodes (hb_codepoint_t variation_selector,
				     hb_set_t *out) ध्रुव
    { subtable_uvs->collect_variation_unicodes (variation_selector, out); }

    रक्षित:
    प्रकारनिर्देश तार्किक (*hb_cmap_get_glyph_func_t) (ध्रुव शून्य *obj,
					      hb_codepoint_t codepoint,
					      hb_codepoint_t *glyph);
    प्रकारनिर्देश uint16_t (*hb_pua_remap_func_t) (unsigned);

    template <typename Type>
    HB_INTERNAL स्थिर तार्किक get_glyph_from (ध्रुव शून्य *obj,
					    hb_codepoint_t codepoint,
					    hb_codepoint_t *glyph)
    {
      ध्रुव Type *typed_obj = (ध्रुव Type *) obj;
      प्रत्यागम typed_obj->get_glyph (codepoint, glyph);
    }

    template <typename Type, hb_pua_remap_func_t remap>
    HB_INTERNAL स्थिर तार्किक get_glyph_from_चिह्नम् (ध्रुव शून्य *obj,
						   hb_codepoint_t codepoint,
						   hb_codepoint_t *glyph)
    {
      ध्रुव Type *typed_obj = (ध्रुव Type *) obj;
      जइ (likely (typed_obj->get_glyph (codepoint, glyph)))
	प्रत्यागम सच्च;

      जइ (hb_codepoint_t c = remap (codepoint))
	प्रत्यागम typed_obj->get_glyph (c, glyph);

      प्रत्यागम मिच्छ;
    }

    template <typename Type>
    HB_INTERNAL स्थिर तार्किक get_glyph_from_ascii (ध्रुव शून्य *obj,
						  hb_codepoint_t codepoint,
						  hb_codepoint_t *glyph)
    {
      ध्रुव Type *typed_obj = (ध्रुव Type *) obj;
      प्रत्यागम codepoint < 0x80 && typed_obj->get_glyph (codepoint, glyph);
    }

    template <typename Type>
    HB_INTERNAL स्थिर तार्किक get_glyph_from_macroman (ध्रुव शून्य *obj,
						     hb_codepoint_t codepoint,
						     hb_codepoint_t *glyph)
    {
      जइ (get_glyph_from_ascii<Type> (obj, codepoint, glyph))
	प्रत्यागम सच्च;

      ध्रुव Type *typed_obj = (ध्रुव Type *) obj;
      unsigned c = unicode_to_macroman (codepoint);
      प्रत्यागम c && typed_obj->get_glyph (c, glyph);
    }

    unsigned पूर्णाङ्क get_subtable_data_size (ध्रुव CmapSubtable *subtable) ध्रुव
    {
      unsigned पूर्णाङ्क table_length = एतत्->table.get_length ();
      uintptr_t table_start = (uintptr_t) (ध्रुव शून्य *) एतत्->table.get ();
      uintptr_t subtable_addr = (uintptr_t) (ध्रुव शून्य *) subtable;
      जइ (unlikely (subtable_addr < table_start))
	प्रत्यागम 0;

      uintptr_t subtable_offset = subtable_addr - table_start;
      जइ (unlikely (subtable_offset >= table_length))
	प्रत्यागम 0;

      प्रत्यागम table_length - (unsigned पूर्णाङ्क) subtable_offset;
    }

    गुप्त:
    hb_nonnull_ptr_t<ध्रुव CmapSubtable> subtable;
    hb_nonnull_ptr_t<ध्रुव CmapSubtable> subtable_uvs;

    hb_cmap_get_glyph_func_t get_glyph_funcZ = nullptr;
    ध्रुव शून्य *get_glyph_data = nullptr;

    CmapSubtableFormat4::accelerator_t format4_accel;

#ifndef HB_NO_OT_FONT_CMAP_CACHE
    cache_t *cache = nullptr;
#endif

    सार्वजनिक:
    hb_blob_ptr_t<cmap> table;
  };

  रक्षित:

  ध्रुव CmapSubtable *find_subtable (unsigned पूर्णाङ्क platform_id,
				     unsigned पूर्णाङ्क encoding_id) ध्रुव
  {
    EncodingRecord key;
    key.platformID = platform_id;
    key.encodingID = encoding_id;

    ध्रुव EncodingRecord &result = encodingRecord.bsearch (key);
    जइ (!result.subtable)
      प्रत्यागम nullptr;

    प्रत्यागम &(एतत्+result.subtable);
  }

  ध्रुव CmapSubtable *find_variation_selector_subtable () ध्रुव
  {
    ध्रुव CmapSubtable *format14 = nullptr;
    प्रति (ध्रुव EncodingRecord& record : encodingRecord)
    {
      जइ (record.platformID != 0 || record.encodingID != 5)
	अनुवर्त;

      ध्रुव CmapSubtable *subtable = &(एतत्+record.subtable);
#ifndef HB_NO_BEYOND_64K
      जइ (subtable->u.format.v == 15)
	प्रत्यागम subtable;
#endif
      जइ (subtable->u.format.v == 14)
	format14 = subtable;
    }
    प्रत्यागम format14;
  }

  सार्वजनिक:

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
		  hb_barrier () &&
		  likely (version == 0) &&
		  encodingRecord.sanitize (c, एतत्));
  }

 गुप्त:

  स्थिर तार्किक filter_encoding_records_for_subset(ध्रुव cmap* cmap,
                                                 ध्रुव EncodingRecord& _)
  {
    प्रत्यागम
        (_.platformID == 0 && _.encodingID == 3) ||
        (_.platformID == 0 && _.encodingID == 4) ||
        (_.platformID == 3 && _.encodingID == 1) ||
        (_.platformID == 3 && _.encodingID == 10) ||
        (cmap + _.subtable).is_variation_selector ();
  }

  रक्षित:
  HBUINT16	version;	/* Table version number (0). */
  SortedArray16Of<EncodingRecord>
		encodingRecord;	/* Encoding tables. */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (4, encodingRecord);
};

संरचना DMAP : cmap
{
  स्थिर constexpr hb_tag_t tableTag = HB_OT_TAG_DMAP;

  तार्किक subset (hb_subset_context_t *c) ध्रुव
  { प्रत्यागम cmap::subset (c, सच्च); }
};

संरचना cmap_accelerator_t
{
  cmap_accelerator_t (hb_face_t *face) :
    dmap (face, HB_OT_TAG_DMAP),
    base (face)
  {}

  तार्किक get_nominal_glyph (hb_codepoint_t  unicode,
			  hb_codepoint_t *glyph) ध्रुव
  {
    प्रत्यागम dmap.get_nominal_glyph (unicode, glyph) ||
	   base.get_nominal_glyph (unicode, glyph);
  }

  unsigned पूर्णाङ्क get_nominal_glyphs (unsigned पूर्णाङ्क गणनम्,
				   ध्रुव hb_codepoint_t *first_unicode,
				   unsigned पूर्णाङ्क unicode_stride,
				   hb_codepoint_t *first_glyph,
				   unsigned पूर्णाङ्क glyph_stride) ध्रुव
  {
    जइ (likely (!dmap.has_data ()))
      प्रत्यागम base.get_nominal_glyphs (गणनम्,
				      first_unicode, unicode_stride,
				      first_glyph, glyph_stride);

    unsigned पूर्णाङ्क done;
    प्रति (done = 0;
	 done < गणनम् && get_nominal_glyph (*first_unicode, first_glyph);
	 done++)
    {
      first_unicode = &StructAtOffsetUnaligned<hb_codepoint_t> (first_unicode, unicode_stride);
      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
    }
    प्रत्यागम done;
  }

  तार्किक get_variation_glyph (hb_codepoint_t  unicode,
			    hb_codepoint_t  variation_selector,
			    hb_codepoint_t *glyph) ध्रुव
  {
    glyph_variant_t result = dmap.get_glyph_variant (unicode,
						     variation_selector,
						     glyph);
    जइ (result == GLYPH_VARIANT_NOT_FOUND)
      result = base.get_glyph_variant (unicode, variation_selector, glyph);

    विकल्प (result)
    {
      पक्ष GLYPH_VARIANT_NOT_FOUND: प्रत्यागम मिच्छ;
      पक्ष GLYPH_VARIANT_FOUND: प्रत्यागम सच्च;
      पक्ष GLYPH_VARIANT_USE_DEFAULT: भङ्ग;
    }

    प्रत्यागम get_nominal_glyph (unicode, glyph);
  }

  शून्य collect_unicodes (hb_set_t *out, unsigned पूर्णाङ्क अङ्कः_glyphs) ध्रुव
  {
    base.collect_unicodes (out, अङ्कः_glyphs);
    dmap.collect_unicodes (out, अङ्कः_glyphs);
  }

  शून्य collect_mapping (hb_set_t *unicodes, hb_map_t *mapping,
			unsigned अङ्कः_glyphs = UINT_MAX) ध्रुव
  {
    base.collect_mapping (unicodes, mapping, अङ्कः_glyphs);
    dmap.collect_mapping (unicodes, mapping, अङ्कः_glyphs);
  }

  शून्य collect_variation_selectors (hb_set_t *out) ध्रुव
  {
    base.collect_variation_selectors (out);
    dmap.collect_variation_selectors (out);
  }

  शून्य collect_variation_unicodes (hb_codepoint_t variation_selector,
				   hb_set_t *out) ध्रुव
  {
    base.collect_variation_unicodes (variation_selector, out);
    dmap.collect_variation_unicodes (variation_selector, out);
  }

  शून्य closure_glyphs (ध्रुव hb_set_t *unicodes,
		       hb_set_t       *glyphset) ध्रुव
  {
    जइ (likely (!dmap.has_data ()))
    {
      base.table->closure_glyphs (unicodes, glyphset);
      प्रत्यागम;
    }

    hb_set_t variation_selectors;
    collect_variation_selectors (&variation_selectors);
    प्रति (hb_codepoint_t variation_selector : variation_selectors)
    {
      जइ (!unicodes->has (variation_selector))
	अनुवर्त;

      hb_set_t variation_unicodes;
      collect_variation_unicodes (variation_selector, &variation_unicodes);
      प्रति (hb_codepoint_t unicode : variation_unicodes)
      {
	जइ (!unicodes->has (unicode))
	  अनुवर्त;

	hb_codepoint_t glyph;
	जइ (get_variation_glyph (unicode, variation_selector, &glyph))
	  glyphset->add (glyph);
      }
    }
  }

  cmap::accelerator_t dmap;
  cmap::accelerator_t base;
};

} /* namespace OT */


#endif /* HB_OT_CMAP_TABLE_HH */
