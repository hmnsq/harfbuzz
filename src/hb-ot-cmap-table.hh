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

#ضمن "hb-ot-os2-table.hh"
#ضمن "hb-ot-shaper-arabic-pua.hh"
#ضمن "hb-open-type.hh"
#ضمن "hb-ot-dual.hh"
#ضمن "hb-set.hh"
#ضمن "hb-cache.hh"

/*
 * cmap -- Character to Glyph Index Mapping
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cmap
 */
#define HB_OT_TAG_cmap HB_TAG('c','m','a','p')
#define HB_OT_TAG_DMAP HB_TAG('D','M','A','P')

نطاق OT {

ساكن مضمن uint8_t unicode_to_macroman (hb_codepoint_t u)
{
  ساكن ثابت بنية unicode_to_macroman_t
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
  auto *c = hb_bsearch (u, mapping, ARRAY_LENGTH (mapping), حجم (mapping[0]),
			_hb_cmp_operator<uint16_t, uint16_t>);
  أرجع c ? c->macroman : 0;
}

بنية CmapSubtableFormat0
{
  منطقي get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) ثابت
  {
    hb_codepoint_t gid = codepoint < 256 ? glyphIdArray[codepoint] : 0;
    إذا (unlikely (!gid))
      أرجع خطأ;
    *glyph = gid;
    أرجع true;
  }

  unsigned get_language () ثابت
  {
    أرجع language;
  }

  فراغ collect_unicodes (hb_set_t *out) ثابت
  {
    لكل (unsigned صحيح i = 0; i < 256; i++)
      إذا (glyphIdArray[i])
	out->add (i);
  }

  فراغ collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping /* OUT */) ثابت
  {
    لكل (unsigned i = 0; i < 256; i++)
      إذا (glyphIdArray[i])
      {
	hb_codepoint_t glyph = glyphIdArray[i];
	unicodes->add (i);
	mapping->set (i, glyph);
      }
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  محمي:
  HBUINT16	format;		/* Format number is set to 0. */
  HBUINT16	length;		/* Byte length of this subtable. */
  HBUINT16	language;	/* Ignore. */
  HBUINT8	glyphIdArray[256];/* An array that maps character
				 * code to glyph index values. */
  عام:
  DEFINE_SIZE_STATIC (6 + 256);
};

بنية CmapSubtableFormat4
{
  template<typename Iterator,
      typename Writer,
	   hb_requires (hb_is_iterator (Iterator))>
  فراغ to_ranges (Iterator it, Writer& range_writer)
  {
    hb_codepoint_t start_cp = 0, prev_run_start_cp = 0, run_start_cp = 0, end_cp = 0, last_gid = 0;
    صحيح run_length = 0 , delta = 0, prev_delta = 0;

    تعداد {
      FIRST_SUB_RANGE,
      FOLLOWING_SUB_RANGE,
    } mode;

    طالما (it) {
      // Start a new range
      {
        ثابت auto& pair = *it;
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

      طالما (it) {
        // Process range
        ثابت auto& pair = *it;
        hb_codepoint_t next_cp = pair.first;
        hb_codepoint_t next_gid = pair.second;
        إذا (next_cp != end_cp + 1) {
          // Current range is over, stop processing.
          اكسر;
        }

        إذا (next_gid == last_gid + 1) {
          // The current run continues.
          end_cp = next_cp;
          run_length++;
          last_gid = next_gid;
          it++;
          استمر;
        }

        // A new run is starting, decide if we want to commit the current run.
        صحيح split_cost = (mode == FIRST_SUB_RANGE) ? 8 : 16;
        صحيح run_cost = run_length * 2;
        إذا (run_cost >= split_cost) {
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

    إذا (likely (end_cp != 0xFFFF)) {
      range_writer (0xFFFF, 0xFFFF, 1);
    }
  }

  /*
   * Writes the current range as either one or two ranges depending on what is most efficient.
   */
  template<typename Writer>
  فراغ commit_current_range (hb_codepoint_t start,
                             hb_codepoint_t prev_run_start,
                             hb_codepoint_t run_start,
                             hb_codepoint_t end,
                             صحيح run_delta,
                             صحيح previous_run_delta,
                             صحيح split_cost,
                             Writer& range_writer) {
    منطقي should_split = خطأ;
    إذا (start < run_start && run_start < end) {
      صحيح run_cost = (end - run_start + 1) * 2;
      إذا (run_cost >= split_cost) {
        should_split = true;
      }
    }

    // A zero delta is also the marker for ranges that need a
    // glyphIdArray; serialize_rangeoffset_glyid() tells the two cases
    // apart by re-checking the mapping.
    إذا (should_split) {
      إذا (start == prev_run_start)
        range_writer (start, run_start - 1, previous_run_delta);
      وإلا
        range_writer (start, run_start - 1, 0);
      range_writer (run_start, end, run_delta);
      أرجع;
    }


    إذا (start == run_start) {
      // Range is only a run
      range_writer (start, end, run_delta);
      أرجع;
    }

    // Write only a single non-run range.
    range_writer (start, end, 0);
  }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  unsigned serialize_find_segcount (Iterator it) {
    بنية Counter {
      unsigned segcount = 0;

      فراغ operator() (hb_codepoint_t start,
                       hb_codepoint_t end,
                       صحيح delta) {
        segcount++;
      }
    } counter;

    to_ranges (+it, counter);
    أرجع counter.segcount;
  }


  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  منطقي serialize_start_end_delta_arrays (hb_serialize_context_t *c,
                                         Iterator it,
                                         صحيح segcount)
  {
    بنية Writer {
      hb_serialize_context_t *serializer_;
      HBUINT16* end_code_;
      HBUINT16* start_code_;
      HBINT16* id_delta_;
      صحيح index_;

      Writer(hb_serialize_context_t *serializer)
          : serializer_(serializer),
            end_code_(nullptr),
            start_code_(nullptr),
            id_delta_(nullptr),
            index_ (0) {}
      فراغ operator() (hb_codepoint_t start,
                       hb_codepoint_t end,
                       صحيح delta) {
        start_code_[index_] = start;
        end_code_[index_] = end;
        id_delta_[index_] = delta;
        index_++;
      }
    } writer(c);

    writer.end_code_ = c->allocate_size<HBUINT16> (HBUINT16::static_size * segcount, خطأ);
    (فراغ) c->allocate_size<HBUINT16> (2); // padding
    writer.start_code_ = c->allocate_size<HBUINT16> (HBUINT16::static_size * segcount, خطأ);
    writer.id_delta_ = c->allocate_size<HBINT16> (HBINT16::static_size * segcount, خطأ);

    إذا (unlikely (!writer.end_code_ || !writer.start_code_ || !writer.id_delta_)) أرجع خطأ;

    to_ranges (+it, writer);
    أرجع true;
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
    إذا (unlikely (!c->check_success (idRangeOffset))) أرجع nullptr;
    إذا (unlikely ((حرف *)idRangeOffset - (حرف *)idDelta != (صحيح) segcount * (صحيح) HBINT16::static_size)) أرجع nullptr;

    لكل (unsigned i : hb_range (segcount))
    {
      /* A non-zero delta is always a real run. A zero delta is
       * ambiguous: identity mapping, or the marker for a range that
       * needs a glyphIdArray. Only zero deltas need a check. */
      صحيح delta = idDelta[i];
      إذا (delta)
	استمر;
      منطقي use_delta = true;
      لكل (hb_codepoint_t cp = startCode[i]; cp <= endCode[i]; cp++)
      {
	hb_codepoint_t gid = cp_to_gid.get (cp);
	إذا (gid == HB_MAP_درج_INVALID)
	  gid = 0;
	إذا (gid != cp)
	{
	  use_delta = خطأ;
	  اكسر;
	}
      }
      إذا (use_delta)
	استمر;

      idRangeOffset[i] = 2 * (c->start_embed<HBUINT16> () - idRangeOffset - i);
      لكل (hb_codepoint_t cp = startCode[i]; cp <= endCode[i]; cp++)
      {
        HBUINT16 gid;
        gid = cp_to_gid[cp];
        c->copy<HBUINT16> (gid);
      }
    }

    أرجع idRangeOffset;
  }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  فراغ serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    auto format4_iter =
    + it
    | hb_filter ([&] (ثابت hb_codepoint_pair_t _)
		 { أرجع _.first <= 0xFFFF; })
    ;

    إذا (!format4_iter) أرجع;

    unsigned table_initpos = c->length ();
    إذا (unlikely (!c->extend_min (هذا))) أرجع;
    هذا->format = 4;

    hb_vector_t<hb_codepoint_pair_t> cp_to_gid {
      format4_iter
    };

    //serialize endCode[], startCode[], idDelta[]
    HBUINT16* endCode = c->start_embed<HBUINT16> ();
    unsigned segcount = serialize_find_segcount (cp_to_gid.iter());
    إذا (unlikely (!serialize_start_end_delta_arrays (c, cp_to_gid.iter(), segcount)))
      أرجع;

    HBUINT16 *startCode = endCode + segcount + 1;
    HBINT16 *idDelta = ((HBINT16*)startCode) + segcount;

    HBUINT16 *idRangeOffset = serialize_rangeoffset_glyid (c,
                                                           cp_to_gid.iter (),
                                                           endCode,
                                                           startCode,
                                                           idDelta,
                                                           segcount);
    إذا (unlikely (!c->check_success (idRangeOffset))) أرجع;

    هذا->length = c->length () - table_initpos;
    إذا ((long long) هذا->length != (long long) c->length () - table_initpos)
    {
      c->err (HB_SERIALIZE_ERROR_INT_OVERFLOW);
      أرجع;
    }

    هذا->seg_كدي_X2 = segcount * 2;
    هذا->entrySelector = hb_max (1u, hb_bit_storage (segcount)) - 1;
    هذا->نيما_Range = 2 * (1u << هذا->entrySelector);
    هذا->rangeShift = segcount * 2 > هذا->نيما_Range
		       ? 2 * segcount - هذا->نيما_Range
		       : 0;
  }

  unsigned get_language () ثابت
  {
    أرجع language;
  }

  بنية accelerator_t
  {
    accelerator_t () {}
    accelerator_t (ثابت CmapSubtableFormat4 *subtable) = delete;

    فراغ init (ثابت CmapSubtableFormat4 *subtable,
	       unsigned صحيح              subtable_data_size)
    {
      seg_كدي_2 = subtable->seg_كدي_X2 / 2;
      end_كدي = subtable->values.arrayZ;
      start_كدي = end_كدي + seg_كدي_2 + 1;
      idDelta = start_كدي + seg_كدي_2;
      idRangeOffset = idDelta + seg_كدي_2;
      glyphIdArray = idRangeOffset + seg_كدي_2;

      unsigned صحيح درجوجي_offset = 16 + 8 * seg_كدي_2;
      glyphIdArrayLength = subtable_data_size > درجوجي_offset
			   ? (subtable_data_size - درجوجي_offset) / 2
			   : 0;
    }

    منطقي get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) ثابت
    {
      بنية CustomRange
      {
	صحيح cmp (hb_codepoint_t k,
		 unsigned distance) ثابت
	{
	  إذا (k > last) أرجع +1;
	  إذا (k < (&last)[distance]/*first*/) أرجع -1;
	  أرجع 0;
	}
	HBUINT16 last;
      };

      ثابت HBUINT16 *found = hb_bsearch (codepoint,
					  هذا->end_كدي,
					  هذا->seg_كدي_2,
					  حجم (CustomRange),
					  _hb_cmp_method<hb_codepoint_t, CustomRange, unsigned>,
					  هذا->seg_كدي_2 + 1);
      إذا (unlikely (!found))
	أرجع خطأ;
      unsigned صحيح i = found - end_كدي;

      hb_codepoint_t gid;
      unsigned صحيح rangeOffset = هذا->idRangeOffset[i];
      إذا (rangeOffset == 0)
	gid = codepoint + هذا->idDelta[i];
      وإلا
      {
	/* Somebody has been smoking... */
	unsigned صحيح index = rangeOffset / 2 + (codepoint - هذا->start_كدي[i]) + i - هذا->seg_كدي_2;
	إذا (unlikely (index >= هذا->glyphIdArrayLength))
	  أرجع خطأ;
	gid = هذا->glyphIdArray[index];
	إذا (unlikely (!gid))
	  أرجع خطأ;
	gid += هذا->idDelta[i];
      }
      gid &= 0xFFFFu;
      إذا (unlikely (!gid))
	أرجع خطأ;
      *glyph = gid;
      أرجع true;
    }

    HB_INTERNAL ساكن منطقي get_glyph_func (ثابت فراغ *obj, hb_codepoint_t codepoint, hb_codepoint_t *glyph)
    { أرجع ((ثابت accelerator_t *) obj)->get_glyph (codepoint, glyph); }

    فراغ collect_unicodes (hb_set_t *out) ثابت
    {
      unsigned صحيح كدي = هذا->seg_كدي_2;
      إذا (كدي && هذا->start_كدي[كدي - 1] == 0xFFFFu)
	كدي--; /* Skip sentinel segment. */
      لكل (unsigned صحيح i = 0; i < كدي; i++)
      {
	hb_codepoint_t start = هذا->start_كدي[i];
	hb_codepoint_t end = هذا->end_كدي[i];
	unsigned صحيح rangeOffset = هذا->idRangeOffset[i];
        out->add_range(start, end);
	إذا (rangeOffset == 0)
	{
	  لكل (hb_codepoint_t codepoint = start; codepoint <= end; codepoint++)
	  {
	    hb_codepoint_t gid = (codepoint + هذا->idDelta[i]) & 0xFFFFu;
	    إذا (unlikely (!gid))
              out->del(codepoint);
	  }
	}
	وإلا
	{
	  لكل (hb_codepoint_t codepoint = start; codepoint <= end; codepoint++)
	  {
	    unsigned صحيح index = rangeOffset / 2 + (codepoint - هذا->start_كدي[i]) + i - هذا->seg_كدي_2;
	    إذا (unlikely (index >= هذا->glyphIdArrayLength))
            {
              out->del_range (codepoint, end);
	      اكسر;
            }
	    hb_codepoint_t gid = هذا->glyphIdArray[index];
	    إذا (unlikely (!gid))
              out->del(codepoint);
	  }
	}
      }
    }

    فراغ collect_mapping (hb_set_t *unicodes, /* OUT */
			  hb_map_t *mapping /* OUT */) ثابت
    {
      // TODO(grieger): optimize similar to collect_unicodes
      // (ie. use add_range())
      unsigned كدي = هذا->seg_كدي_2;
      إذا (كدي && هذا->start_كدي[كدي - 1] == 0xFFFFu)
	كدي--; /* Skip sentinel segment. */
      لكل (unsigned i = 0; i < كدي; i++)
      {
	hb_codepoint_t start = هذا->start_كدي[i];
	hb_codepoint_t end = هذا->end_كدي[i];
	unsigned rangeOffset = هذا->idRangeOffset[i];
	إذا (rangeOffset == 0)
	{
	  لكل (hb_codepoint_t codepoint = start; codepoint <= end; codepoint++)
	  {
	    hb_codepoint_t gid = (codepoint + هذا->idDelta[i]) & 0xFFFFu;
	    إذا (unlikely (!gid))
	      استمر;
	    unicodes->add (codepoint);
	    mapping->set (codepoint, gid);
	  }
	}
	وإلا
	{
	  لكل (hb_codepoint_t codepoint = start; codepoint <= end; codepoint++)
	  {
	    unsigned index = rangeOffset / 2 + (codepoint - هذا->start_كدي[i]) + i - هذا->seg_كدي_2;
	    إذا (unlikely (index >= هذا->glyphIdArrayLength))
	      اكسر;
	    hb_codepoint_t gid = هذا->glyphIdArray[index];
	    إذا (unlikely (!gid))
	      استمر;
            gid = (gid + هذا->idDelta[i]) & 0xFFFFu;
	    unicodes->add (codepoint);
	    mapping->set (codepoint, gid);
	  }
	}
      }
    }

    ثابت HBUINT16 *end_كدي;
    ثابت HBUINT16 *start_كدي;
    ثابت HBUINT16 *idDelta;
    ثابت HBUINT16 *idRangeOffset;
    ثابت HBUINT16 *glyphIdArray;
    unsigned صحيح seg_كدي_2;
    unsigned صحيح glyphIdArrayLength;
  };

  منطقي get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph,
		  unsigned صحيح subtable_data_size) ثابت
  {
    accelerator_t accel;
    accel.init (هذا, subtable_data_size);
    أرجع accel.get_glyph_func (&accel, codepoint, glyph);
  }
  منطقي get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) ثابت
  { أرجع خطأ; }

  فراغ collect_unicodes (hb_set_t *out, unsigned صحيح subtable_data_size) ثابت
  {
    accelerator_t accel;
    accel.init (هذا, subtable_data_size);
    accel.collect_unicodes (out);
  }
  فراغ collect_unicodes (hb_set_t *out) ثابت
  { collect_unicodes (out, length); }

  فراغ collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping, /* OUT */
			unsigned صحيح subtable_data_size) ثابت
  {
    accelerator_t accel;
    accel.init (هذا, subtable_data_size);
    accel.collect_mapping (unicodes, mapping);
  }
  فراغ collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping /* OUT */) ثابت
  { collect_mapping (unicodes, mapping, length); }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!c->check_struct (هذا)))
      return_trace (خطأ);
    hb_barrier ();

    return_trace (c->check_range (values, 2 + 4 * seg_كدي_X2));
  }

  محمي:
  HBUINT16	format;		/* Format number is set to 4. */
  HBUINT16	length;		/* This is the length in bytes of the
				 * subtable. */
  HBUINT16	language;	/* Ignore. */
  HBUINT16	seg_كدي_X2;	/* 2 x segCount. */
  HBUINT16	نيما_Range;	/* 2 * (2**floor(log2(segCount))) */
  HBUINT16	entrySelector;	/* log2(searchRange/2) */
  HBUINT16	rangeShift;	/* 2 x segCount - searchRange */

  UnsizedArrayOf<HBUINT16>
		values;
#إذا 0
  HBUINT16	end_كدي[seg_كدي_2];	/* End characterCode for each segment,
					 * last=0xFFFFu. */
  HBUINT16	reservedPad;		/* Set to 0. */
  HBUINT16	start_كدي[seg_كدي_2];	/* Start character code for each segment. */
  HBINT16		idDelta[seg_كدي_2];	/* Delta for all character codes in segment. */
  HBUINT16	idRangeOffset[seg_كدي_2];/* Offsets into glyphIdArray or 0 */
  UnsizedArrayOf<HBUINT16>
		glyphIdArray;	/* Glyph index array (arbitrary length) */
#endif

  عام:
  DEFINE_SIZE_ARRAY (14, values);
};

بنية CmapSubtableLongGroup
{
  friend بنية CmapSubtableFormat12;
  friend بنية CmapSubtableFormat13;
  template<typename U>
  friend بنية CmapSubtableLongSegmented;
  friend بنية cmap;

  صحيح cmp (hb_codepoint_t codepoint) ثابت
  {
    إذا (codepoint < startCharCode) أرجع -1;
    إذا (codepoint > endCharCode)   أرجع +1;
    أرجع 0;
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  خاص:
  HBUINT32		startCharCode;	/* First character code in this group. */
  HBUINT32		endCharCode;	/* Last character code in this group. */
  HBUINT32		glyphID;	/* Glyph index; interpretation depends on
					 * subtable format. */
  عام:
  DEFINE_SIZE_STATIC (12);
};
DECLARE_NULL_NAMESPACE_BYTES (OT, CmapSubtableLongGroup);

template <typename UINT>
بنية CmapSubtableTrimmed
{
  منطقي get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) ثابت
  {
    /* Rely on our implicit array bound-checking. */
    hb_codepoint_t gid = glyphIdArray[codepoint - startCharCode];
    إذا (unlikely (!gid))
      أرجع خطأ;
    *glyph = gid;
    أرجع true;
  }

  unsigned get_language () ثابت
  {
    أرجع language;
  }

  فراغ collect_unicodes (hb_set_t *out) ثابت
  {
    hb_codepoint_t start = startCharCode;
    unsigned صحيح كدي = glyphIdArray.len;
    لكل (unsigned صحيح i = 0; i < كدي; i++)
      إذا (glyphIdArray[i])
	out->add (start + i);
  }

  فراغ collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping /* OUT */) ثابت
  {
    hb_codepoint_t start_cp = startCharCode;
    unsigned كدي = glyphIdArray.len;
    لكل (unsigned i = 0; i < كدي; i++)
      إذا (glyphIdArray[i])
      {
	hb_codepoint_t unicode = start_cp + i;
	hb_codepoint_t glyphid = glyphIdArray[i];
	unicodes->add (unicode);
	mapping->set (unicode, glyphid);
      }
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && glyphIdArray.sanitize (c));
  }

  محمي:
  UINT		formatReserved;	/* Subtable format and (maybe) padding. */
  UINT		length;		/* Byte length of this subtable. */
  UINT		language;	/* Ignore. */
  UINT		startCharCode;	/* First character code covered. */
  ArrayOf<HBGlyphID16, UINT>
		glyphIdArray;	/* Array of glyph index values for character
				 * codes in the range. */
  عام:
  DEFINE_SIZE_ARRAY (5 * حجم (UINT), glyphIdArray);
};

بنية CmapSubtableFormat6  : CmapSubtableTrimmed<HBUINT16> {};
بنية CmapSubtableFormat10 : CmapSubtableTrimmed<HBUINT32> {};

template <typename T>
بنية CmapSubtableLongSegmented
{
  friend بنية cmap;

  منطقي get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) ثابت
  {
    hb_codepoint_t gid = T::group_get_glyph (groups.bsearch (codepoint), codepoint);
    إذا (unlikely (!gid))
      أرجع خطأ;
    *glyph = gid;
    أرجع true;
  }

  unsigned get_language () ثابت
  {
    أرجع language;
  }

  فراغ collect_unicodes (hb_set_t *out, unsigned صحيح لمب_glyphs) ثابت
  {
    لكل (unsigned صحيح i = 0; i < هذا->groups.len; i++)
    {
      hb_codepoint_t start = هذا->groups[i].startCharCode;
      hb_codepoint_t end = hb_min ((hb_codepoint_t) هذا->groups[i].endCharCode,
				   (hb_codepoint_t) HB_UNICODE_MAX);
      hb_codepoint_t gid = هذا->groups[i].glyphID;
      إذا (!gid)
      {
	/* Intention is: if (hb_is_same (T, CmapSubtableFormat13)) continue; */
	إذا (! T::group_get_glyph (هذا->groups[i], end)) استمر;
	start++;
	gid++;
      }
      إذا (unlikely ((unsigned صحيح) gid >= لمب_glyphs)) استمر;
      إذا (unlikely ((unsigned صحيح) (gid + end - start) >= لمب_glyphs))
	end = start + (hb_codepoint_t) لمب_glyphs - gid;

      out->add_range (start, hb_min (end, 0x10FFFFu));
    }
  }

  فراغ collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping, /* OUT */
			unsigned لمب_glyphs) ثابت
  {
    hb_codepoint_t last_end = 0;
    unsigned كدي = هذا->groups.len;
    لكل (unsigned i = 0; i < كدي; i++)
    {
      hb_codepoint_t start = هذا->groups.arrayZ[i].startCharCode;
      hb_codepoint_t end = hb_min ((hb_codepoint_t) هذا->groups.arrayZ[i].endCharCode,
				   (hb_codepoint_t) HB_UNICODE_MAX);
      إذا (unlikely (start > end || start < last_end)) {
        // Range is not in order and is invalid, skip it.
        استمر;
      }
      last_end = end;


      hb_codepoint_t gid = هذا->groups.arrayZ[i].glyphID;
      إذا (!gid)
      {
        إذا (T::format_لمب == 13) استمر;
	start++;
	gid++;
      }
      إذا (unlikely ((unsigned صحيح) gid >= لمب_glyphs)) استمر;
      إذا (unlikely ((unsigned صحيح) (gid + end - start) >= لمب_glyphs))
	end = start + (hb_codepoint_t) لمب_glyphs - gid;

      mapping->alloc (mapping->get_population () + end - start + 1);

      unicodes->add_range (start, end);
      لكل (unsigned cp = start; cp <= end; cp++)
      {
	mapping->set (cp, gid);
        gid += T::increment;
      }
    }
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && groups.sanitize (c));
  }

  محمي:
  HBUINT16	format;		/* Subtable format; set to 12. */
  HBUINT16	reserved;	/* Reserved; set to 0. */
  HBUINT32	length;		/* Byte length of this subtable. */
  HBUINT32	language;	/* Ignore. */
  SortedArray32Of<CmapSubtableLongGroup>
		groups;		/* Groupings. */
  عام:
  DEFINE_SIZE_ARRAY (16, groups);
};

بنية CmapSubtableFormat12 : CmapSubtableLongSegmented<CmapSubtableFormat12>
{
  ساكن constexpr صحيح increment = 1;
  ساكن constexpr صحيح format_لمب = 12;

  ساكن hb_codepoint_t group_get_glyph (ثابت CmapSubtableLongGroup &group,
					 hb_codepoint_t u)
  { أرجع likely (group.startCharCode <= group.endCharCode) ?
	   group.glyphID + (u - group.startCharCode) : 0; }


  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  فراغ serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    إذا (!it) أرجع;
    unsigned table_initpos = c->length ();
    إذا (unlikely (!c->extend_min (هذا))) أرجع;

    hb_codepoint_t startCharCode = (hb_codepoint_t) -1, endCharCode = (hb_codepoint_t) -1;
    hb_codepoint_t glyphID = 0;

    لكل (ثابت auto& _ : +it)
    {
      إذا (startCharCode == (hb_codepoint_t) -1)
      {
	startCharCode = _.first;
	endCharCode = _.first;
	glyphID = _.second;
      }
      وإلا إذا (!_is_gid_consecutive (endCharCode, startCharCode, glyphID, _.first, _.second))
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
      وإلا
	endCharCode = _.first;
    }

    CmapSubtableLongGroup record;
    record.startCharCode = startCharCode;
    record.endCharCode = endCharCode;
    record.glyphID = glyphID;
    c->copy<CmapSubtableLongGroup> (record);

    هذا->format = 12;
    هذا->reserved = 0;
    هذا->length = c->length () - table_initpos;
    هذا->groups.len = (هذا->length - min_size) / CmapSubtableLongGroup::static_size;
  }

  ساكن size_t get_sub_table_size (ثابت hb_sorted_vector_t<CmapSubtableLongGroup> &groups_data)
  { أرجع 16 + 12 * groups_data.length; }

  خاص:
  ساكن منطقي _is_gid_consecutive (hb_codepoint_t endCharCode,
				   hb_codepoint_t startCharCode,
				   hb_codepoint_t glyphID,
				   hb_codepoint_t cp,
				   hb_codepoint_t new_gid)
  {
    أرجع (cp - 1 == endCharCode) &&
	new_gid == glyphID + (cp - startCharCode);
  }

};

بنية CmapSubtableFormat13 : CmapSubtableLongSegmented<CmapSubtableFormat13>
{
  ساكن constexpr صحيح increment = 0;
  ساكن constexpr صحيح format_لمب = 13;

  ساكن hb_codepoint_t group_get_glyph (ثابت CmapSubtableLongGroup &group,
					 hb_codepoint_t u HB_UNUSED)
  { أرجع group.glyphID; }
};

تعريفنوع تعداد
{
  GLYPH_VARIANT_NOT_FOUND = 0,
  GLYPH_VARIANT_FOUND = 1,
  GLYPH_VARIANT_USE_DEFAULT = 2
} glyph_variant_t;

بنية Unicode_درج_Range
{
  صحيح cmp (ثابت hb_codepoint_t &codepoint) ثابت
  {
    إذا (codepoint < start_Unicode_درج) أرجع -1;
    إذا (codepoint > start_Unicode_درج + additional_كدي) أرجع +1;
    أرجع 0;
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  HBUINT24	start_Unicode_درج;	/* First value in this range. */
  HBUINT8	additional_كدي;	/* Number of additional values in this
					 * range. */
  عام:
  DEFINE_SIZE_STATIC (4);
};

بنية DefaultUVS : SortedArray32Of<Unicode_درج_Range>
{
  فراغ collect_unicodes (hb_set_t *out) ثابت
  {
    unsigned صحيح كدي = len;
    لكل (unsigned صحيح i = 0; i < كدي; i++)
    {
      hb_codepoint_t first = arrayZ[i].start_Unicode_درج;
      hb_codepoint_t last = hb_min ((hb_codepoint_t) (first + arrayZ[i].additional_كدي),
				    (hb_codepoint_t) HB_UNICODE_MAX);
      out->add_range (first, last);
    }
  }

  DefaultUVS* copy (hb_serialize_context_t *c,
		    ثابت hb_set_t *unicodes) ثابت
  {
    auto *out = c->start_embed<DefaultUVS> ();
    auto snap = c->snapshot ();

    HBUINT32 len;
    len = 0;
    إذا (unlikely (!c->copy<HBUINT32> (len))) أرجع nullptr;
    unsigned init_len = c->length ();

    إذا (هذا->len > unicodes->get_population () * hb_bit_storage ((unsigned) هذا->len))
    {
      hb_codepoint_t start = HB_SET_درج_INVALID;
      hb_codepoint_t end = HB_SET_درج_INVALID;

      لكل (auto u : *unicodes)
      {
        إذا (!as_array ().bsearch (u))
	  استمر;
	إذا (start == HB_SET_درج_INVALID)
	{
	  start = u;
	  end = start - 1;
	}
	إذا (end + 1 != u || end - start == 255)
        {
	  Unicode_درج_Range rec;
	  rec.start_Unicode_درج = start;
	  rec.additional_كدي = end - start;
	  c->copy<Unicode_درج_Range> (rec);
	  start = u;
	}
	end = u;
      }
      إذا (start != HB_SET_درج_INVALID)
      {
	Unicode_درج_Range rec;
	rec.start_Unicode_درج = start;
	rec.additional_كدي = end - start;
	c->copy<Unicode_درج_Range> (rec);
      }

    }
    وإلا
    {
      hb_codepoint_t lastCode = HB_SET_درج_INVALID;
      صحيح كدي = -1;

      لكل (ثابت Unicode_درج_Range& _ : *هذا)
      {
	hb_codepoint_t curEntry = (hb_codepoint_t) (_.start_Unicode_درج - 1);
	hb_codepoint_t end = curEntry + _.additional_كدي + 2;

	لكل (; unicodes->next (&curEntry) && curEntry < end;)
	{
	  كدي += 1;
	  إذا (lastCode == HB_SET_درج_INVALID)
	    lastCode = curEntry;
	  وإلا إذا (lastCode + كدي != curEntry || كدي > 255)
	  {
	    Unicode_درج_Range rec;
	    rec.start_Unicode_درج = lastCode;
	    rec.additional_كدي = كدي - 1;
	    c->copy<Unicode_درج_Range> (rec);

	    lastCode = curEntry;
	    كدي = 0;
	  }
	}
      }

      إذا (lastCode != HB_MAP_درج_INVALID)
      {
	Unicode_درج_Range rec;
	rec.start_Unicode_درج = lastCode;
	rec.additional_كدي = كدي;
	c->copy<Unicode_درج_Range> (rec);
      }
    }

    إذا (c->length () - init_len == 0)
    {
      c->revert (snap);
      أرجع nullptr;
    }
    وإلا
    {
      إذا (unlikely (!c->check_assign (out->len,
                                      (c->length () - init_len) / Unicode_درج_Range::static_size,
                                      HB_SERIALIZE_ERROR_INT_OVERFLOW))) أرجع nullptr;
      أرجع out;
    }
  }

  عام:
  DEFINE_SIZE_ARRAY (4, *هذا);
};

template <typename Types>
بنية UVSMapping
{
  using GlyphID = typename Types::HBGlyphID;

  صحيح cmp (ثابت hb_codepoint_t &codepoint) ثابت
  { أرجع unicode_درج.cmp (codepoint); }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  HBUINT24	unicode_درج;	/* Base Unicode value of the UVS */
  GlyphID
		glyphID;	/* Glyph ID of the UVS. */
  عام:
  DEFINE_SIZE_STATIC (3 + GlyphID::static_size);
};

template <typename Types>
بنية NonDefaultUVS : SortedArray32Of<UVSMapping<Types>>
{
  using Mapping = UVSMapping<Types>;

  فراغ collect_unicodes (hb_set_t *out) ثابت
  {
    لكل (ثابت auto& a : هذا->as_array ())
      out->add (a.unicode_درج);
  }

  فراغ collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping /* OUT */) ثابت
  {
    لكل (ثابت auto& a : هذا->as_array ())
    {
      hb_codepoint_t unicode = a.unicode_درج;
      hb_codepoint_t glyphid = a.glyphID;
      unicodes->add (unicode);
      mapping->set (unicode, glyphid);
    }
  }

  فراغ closure_glyphs (ثابت hb_set_t      *unicodes,
		       hb_set_t            *glyphset) ثابت
  {
    + هذا->as_array ()
    | hb_filter (unicodes, &Mapping::unicode_درج)
    | hb_map (&Mapping::glyphID)
    | hb_sink (glyphset)
    ;
  }

  NonDefaultUVS* copy (hb_serialize_context_t *c,
		       ثابت hb_set_t *unicodes,
		       ثابت hb_set_t *glyphs_requested,
		       ثابت hb_map_t *glyph_map) ثابت
  {
    auto *out = c->start_embed<NonDefaultUVS> ();
    auto it =
    + هذا->as_array ()
    | hb_filter ([&] (ثابت Mapping& _)
		 {
		   أرجع unicodes->has (_.unicode_درج) || glyphs_requested->has (_.glyphID);
		 })
    ;

    إذا (!it) أرجع nullptr;

    HBUINT32 len;
    len = it.len ();
    إذا (unlikely (!c->copy<HBUINT32> (len))) أرجع nullptr;

    لكل (ثابت Mapping& _ : it)
    {
      Mapping mapping;
      mapping.unicode_درج = _.unicode_درج;
      mapping.glyphID = glyph_map->get (_.glyphID);
      c->copy<Mapping> (mapping);
    }

    أرجع out;
  }

  عام:
  DEFINE_SIZE_ARRAY (4, *هذا);
};

template <typename Types>
بنية VariationSelectorRecord
{
  glyph_variant_t get_glyph (hb_codepoint_t codepoint,
			     hb_codepoint_t *glyph,
			     ثابت فراغ *base) ثابت
  {
    إذا ((base+defaultUVS).bfind (codepoint))
      أرجع GLYPH_VARIANT_USE_DEFAULT;
    ثابت UVSMapping<Types> &nonDefault = (base+nonDefaultUVS).bsearch (codepoint);
    إذا (nonDefault.glyphID)
    {
      *glyph = nonDefault.glyphID;
       أرجع GLYPH_VARIANT_FOUND;
    }
    أرجع GLYPH_VARIANT_NOT_FOUND;
  }

  VariationSelectorRecord(ثابت VariationSelectorRecord& other)
  {
    *هذا = other;
  }

  فراغ operator= (ثابت VariationSelectorRecord& other)
  {
    varSelector = other.varSelector;
    HBUINT32 offset = other.defaultUVS;
    defaultUVS = offset;
    offset = other.nonDefaultUVS;
    nonDefaultUVS = offset;
  }

  فراغ collect_unicodes (hb_set_t *out, ثابت فراغ *base) ثابت
  {
    (base+defaultUVS).collect_unicodes (out);
    (base+nonDefaultUVS).collect_unicodes (out);
  }

  فراغ collect_mapping (ثابت فراغ *base,
			hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping /* OUT */) ثابت
  {
    (base+defaultUVS).collect_unicodes (unicodes);
    (base+nonDefaultUVS).collect_mapping (unicodes, mapping);
  }

  صحيح cmp (ثابت hb_codepoint_t &variation_selector) ثابت
  { أرجع varSelector.cmp (variation_selector); }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *base) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  defaultUVS.sanitize (c, base) &&
		  nonDefaultUVS.sanitize (c, base));
  }

  hb_pair_t<unsigned, unsigned>
  copy (hb_serialize_context_t *c,
	ثابت hb_set_t *unicodes,
	ثابت hb_set_t *glyphs_requested,
	ثابت hb_map_t *glyph_map,
	ثابت فراغ *base) ثابت
  {
    auto snap = c->snapshot ();
    auto *out = c->embed<VariationSelectorRecord> (*هذا);
    إذا (unlikely (!out)) أرجع hb_pair (0, 0);

    out->defaultUVS = 0;
    out->nonDefaultUVS = 0;

    unsigned non_default_uvs_objidx = 0;
    إذا (nonDefaultUVS != 0)
    {
      c->push ();
      إذا (c->copy (base+nonDefaultUVS, unicodes, glyphs_requested, glyph_map))
	non_default_uvs_objidx = c->چرو_pack ();
      وإلا c->چرو_discard ();
    }

    unsigned default_uvs_objidx = 0;
    إذا (defaultUVS != 0)
    {
      c->push ();
      إذا (c->copy (base+defaultUVS, unicodes))
	default_uvs_objidx = c->چرو_pack ();
      وإلا c->چرو_discard ();
    }


    إذا (!default_uvs_objidx && !non_default_uvs_objidx)
      c->revert (snap);

    أرجع hb_pair (default_uvs_objidx, non_default_uvs_objidx);
  }

  HBUINT24	varSelector;	/* Variation selector. */
  Offset32To<DefaultUVS>
		defaultUVS;	/* Offset to Default UVS Table.  May be 0. */
  Offset32To<NonDefaultUVS<Types>>
		nonDefaultUVS;	/* Offset to Non-Default UVS Table.  May be 0. */
  عام:
  DEFINE_SIZE_STATIC (11);
};

template <typename Types>
بنية CmapSubtableFormat14_15
{
  ساكن constexpr unsigned format_درج = Types::size == 2 ? 14 : 15;
  using Record = VariationSelectorRecord<Types>;
  using NonDefault = NonDefaultUVS<Types>;

  glyph_variant_t get_glyph_variant (hb_codepoint_t codepoint,
				     hb_codepoint_t variation_selector,
				     hb_codepoint_t *glyph) ثابت
  { أرجع record.bsearch (variation_selector).get_glyph (codepoint, glyph, هذا); }

  فراغ collect_variation_selectors (hb_set_t *out) ثابت
  {
    لكل (ثابت auto& a : record.as_array ())
      out->add (a.varSelector);
  }
  فراغ collect_variation_unicodes (hb_codepoint_t variation_selector,
				   hb_set_t *out) ثابت
  { record.bsearch (variation_selector).collect_unicodes (out, هذا); }

  فراغ serialize (hb_serialize_context_t *c,
		  ثابت hb_set_t *unicodes,
		  ثابت hb_set_t *glyphs_requested,
		  ثابت hb_map_t *glyph_map,
		  ثابت فراغ *base)
  {
    auto snap = c->snapshot ();
    unsigned table_initpos = c->length ();
    ثابت حرف* init_tail = c->tail;

    إذا (unlikely (!c->extend_min (هذا))) أرجع;
    هذا->format = format_درج;

    auto src_tbl = reinterpret_cast<ثابت CmapSubtableFormat14_15*> (base);

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
    لكل (صحيح i = src_tbl->record.len - 1; i >= 0; i--)
    {
      إذا (!unicodes->has(src_tbl->record[i].varSelector))
        استمر;

      hb_pair_t<unsigned, unsigned> result = src_tbl->record[i].copy (c, unicodes, glyphs_requested, glyph_map, base);
      إذا (result.first || result.second)
	obj_indices.push (result);
    }

    إذا (c->length () - table_initpos == CmapSubtableFormat14_15::min_size)
    {
      c->revert (snap);
      أرجع;
    }

    إذا (unlikely (!c->check_success (!obj_indices.in_error ())))
      أرجع;

    صحيح tail_len = init_tail - c->tail;
    c->check_assign (هذا->length, c->length () - table_initpos + tail_len,
                     HB_SERIALIZE_ERROR_INT_OVERFLOW);
    c->check_assign (هذا->record.len,
		     (c->length () - table_initpos - CmapSubtableFormat14_15::min_size) /
		     Record::static_size,
                     HB_SERIALIZE_ERROR_INT_OVERFLOW);

    /* Correct the incorrect write order by reversing the order of the variation
       records array. */
    _reverse_variation_records ();

    /* Now that records are in the right order, we can set up the offsets. */
    _add_links_to_variation_records (c, obj_indices);
  }

  فراغ _reverse_variation_records ()
  {
    record.as_array ().reverse ();
  }

  فراغ _add_links_to_variation_records (hb_serialize_context_t *c,
					ثابت hb_vector_t<hb_pair_t<unsigned, unsigned>>& obj_indices)
  {
    لكل (unsigned i = 0; i < obj_indices.length; i++)
    {
      /*
       * Since the record array has been reversed (see comments in copy())
       * but obj_indices has not been, the indices at obj_indices[i]
       * are for the variation record at record[j].
       */
      صحيح j = obj_indices.length - 1 - i;
      c->add_link (record[j].defaultUVS, obj_indices[i].first);
      c->add_link (record[j].nonDefaultUVS, obj_indices[i].second);
    }
  }

  فراغ closure_glyphs (ثابت hb_set_t      *unicodes,
		       hb_set_t            *glyphset) ثابت
  {
    + hb_iter (record)
    | hb_filter (hb_bool, &Record::nonDefaultUVS)
    | hb_filter (unicodes, &Record::varSelector)
    | hb_map (&Record::nonDefaultUVS)
    | hb_map (hb_add (هذا))
    | hb_apply ([=] (ثابت NonDefault& _) { _.closure_glyphs (unicodes, glyphset); })
    ;
  }

  فراغ collect_unicodes (hb_set_t *out) ثابت
  {
    لكل (ثابت Record& _ : record)
      _.collect_unicodes (out, هذا);
  }

  فراغ collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping /* OUT */) ثابت
  {
    لكل (ثابت Record& _ : record)
      _.collect_mapping (هذا, unicodes, mapping);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  record.sanitize (c, هذا));
  }

  محمي:
  HBUINT16	format;		/* Format number is set to 14 or 15. */
  HBUINT32	length;		/* Byte length of this subtable. */
  SortedArray32Of<Record>
		record;		/* Variation selector records; sorted
				 * in increasing order of `varSelector'. */
  عام:
  DEFINE_SIZE_ARRAY (10, record);
};

بنية CmapSubtable
{
  /* Note: We intentionally do NOT implement subtable formats 2 and 8. */

  منطقي is_variation_selector () ثابت
  {
    بدل (u.format.v) {
    حالة 14:
#ifndef HB_NO_BEYOND_64K
    حالة 15:
#endif
      أرجع true;
    افتراضي:
      أرجع خطأ;
    }
  }

  glyph_variant_t get_glyph_variant (hb_codepoint_t codepoint,
				     hb_codepoint_t variation_selector,
				     hb_codepoint_t *glyph) ثابت
  {
    بدل (u.format.v) {
    حالة 14: hb_barrier (); أرجع u.format14.get_glyph_variant (codepoint, variation_selector, glyph);
#ifndef HB_NO_BEYOND_64K
    حالة 15: hb_barrier (); أرجع u.format15.get_glyph_variant (codepoint, variation_selector, glyph);
#endif
    افتراضي: أرجع GLYPH_VARIANT_NOT_FOUND;
    }
  }

  فراغ collect_variation_selectors (hb_set_t *out) ثابت
  {
    بدل (u.format.v) {
    حالة 14: hb_barrier (); u.format14.collect_variation_selectors (out); أرجع;
#ifndef HB_NO_BEYOND_64K
    حالة 15: hb_barrier (); u.format15.collect_variation_selectors (out); أرجع;
#endif
    افتراضي: أرجع;
    }
  }

  فراغ collect_variation_unicodes (hb_codepoint_t variation_selector,
				    hb_set_t *out) ثابت
  {
    بدل (u.format.v) {
    حالة 14: hb_barrier (); u.format14.collect_variation_unicodes (variation_selector, out); أرجع;
#ifndef HB_NO_BEYOND_64K
    حالة 15: hb_barrier (); u.format15.collect_variation_unicodes (variation_selector, out); أرجع;
#endif
    افتراضي: أرجع;
    }
  }

  فراغ closure_glyphs (ثابت hb_set_t *unicodes, hb_set_t *glyphset) ثابت
  {
    بدل (u.format.v) {
    حالة 14: hb_barrier (); u.format14.closure_glyphs (unicodes, glyphset); أرجع;
#ifndef HB_NO_BEYOND_64K
    حالة 15: hb_barrier (); u.format15.closure_glyphs (unicodes, glyphset); أرجع;
#endif
    افتراضي: أرجع;
    }
  }

  منطقي get_glyph (hb_codepoint_t codepoint,
		  hb_codepoint_t *glyph,
		  unsigned صحيح subtable_data_size = 0) ثابت
  {
    بدل (u.format.v) {
    حالة  0: hb_barrier (); أرجع u.format0 .get_glyph (codepoint, glyph);
    حالة  4: hb_barrier (); أرجع subtable_data_size
				      ? u.format4.get_glyph (codepoint, glyph, subtable_data_size)
				      : خطأ;
    حالة  6: hb_barrier (); أرجع u.format6 .get_glyph (codepoint, glyph);
    حالة 10: hb_barrier (); أرجع u.format10.get_glyph (codepoint, glyph);
    حالة 12: hb_barrier (); أرجع u.format12.get_glyph (codepoint, glyph);
    حالة 13: hb_barrier (); أرجع u.format13.get_glyph (codepoint, glyph);
    حالة 14:
    افتراضي: أرجع خطأ;
    }
  }
  فراغ collect_unicodes (hb_set_t *out,
			 unsigned صحيح لمب_glyphs = UINT_MAX,
			 unsigned صحيح subtable_data_size = 0) ثابت
  {
    بدل (u.format.v) {
    حالة  0: hb_barrier (); u.format0 .collect_unicodes (out); أرجع;
    حالة  4: hb_barrier (); إذا (subtable_data_size)
			      u.format4.collect_unicodes (out, subtable_data_size);
			    وإلا
			      u.format4.collect_unicodes (out);
			    أرجع;
    حالة  6: hb_barrier (); u.format6 .collect_unicodes (out); أرجع;
    حالة 10: hb_barrier (); u.format10.collect_unicodes (out); أرجع;
    حالة 12: hb_barrier (); u.format12.collect_unicodes (out, لمب_glyphs); أرجع;
    حالة 13: hb_barrier (); u.format13.collect_unicodes (out, لمب_glyphs); أرجع;
    حالة 14:
    افتراضي: أرجع;
    }
  }

  فراغ collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping, /* OUT */
			unsigned لمب_glyphs = UINT_MAX,
			unsigned صحيح subtable_data_size = 0) ثابت
  {
    بدل (u.format.v) {
    حالة  0: hb_barrier (); u.format0 .collect_mapping (unicodes, mapping); أرجع;
    حالة  4: hb_barrier (); إذا (subtable_data_size)
			      u.format4.collect_mapping (unicodes, mapping, subtable_data_size);
			    وإلا
			      u.format4.collect_mapping (unicodes, mapping);
			    أرجع;
    حالة  6: hb_barrier (); u.format6 .collect_mapping (unicodes, mapping); أرجع;
    حالة 10: hb_barrier (); u.format10.collect_mapping (unicodes, mapping); أرجع;
    حالة 12: hb_barrier (); u.format12.collect_mapping (unicodes, mapping, لمب_glyphs); أرجع;
    حالة 13: hb_barrier (); u.format13.collect_mapping (unicodes, mapping, لمب_glyphs); أرجع;
    حالة 14:
    افتراضي: أرجع;
    }
  }

  unsigned get_language () ثابت
  {
    بدل (u.format.v) {
    حالة  0: hb_barrier (); أرجع u.format0 .get_language ();
    حالة  4: hb_barrier (); أرجع u.format4 .get_language ();
    حالة  6: hb_barrier (); أرجع u.format6 .get_language ();
    حالة 10: hb_barrier (); أرجع u.format10.get_language ();
    حالة 12: hb_barrier (); أرجع u.format12.get_language ();
    حالة 13: hb_barrier (); أرجع u.format13.get_language ();
    حالة 14:
    افتراضي: أرجع 0;
    }
  }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  فراغ serialize (hb_serialize_context_t *c,
		  Iterator it,
		  unsigned format,
		  ثابت hb_subset_plan_t *plan,
		  ثابت فراغ *base)
  {
    بدل (format) {
    حالة  4: hb_barrier (); أرجع u.format4.serialize (c, it);
    حالة 12: hb_barrier (); أرجع u.format12.serialize (c, it);
    حالة 14: hb_barrier (); أرجع u.format14.serialize (c, &plan->unicodes, &plan->glyphs_requested, plan->glyph_map, base);
#ifndef HB_NO_BEYOND_64K
    حالة 15: hb_barrier (); أرجع u.format15.serialize (c, &plan->unicodes, &plan->glyphs_requested, plan->glyph_map, base);
#endif
    افتراضي: أرجع;
    }
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (!u.format.v.sanitize (c)) return_trace (خطأ);
    hb_barrier ();
    بدل (u.format.v) {
    حالة  0: hb_barrier (); return_trace (u.format0 .sanitize (c));
    حالة  4: hb_barrier (); return_trace (u.format4 .sanitize (c));
    حالة  6: hb_barrier (); return_trace (u.format6 .sanitize (c));
    حالة 10: hb_barrier (); return_trace (u.format10.sanitize (c));
    حالة 12: hb_barrier (); return_trace (u.format12.sanitize (c));
    حالة 13: hb_barrier (); return_trace (u.format13.sanitize (c));
    حالة 14: hb_barrier (); return_trace (u.format14.sanitize (c));
#ifndef HB_NO_BEYOND_64K
    حالة 15: hb_barrier (); return_trace (u.format15.sanitize (c));
#endif
    افتراضي:return_trace (true);
    }
  }

  عام:
  اتحاد {
  بنية { HBUINT16 v; }	format;		/* Format identifier */
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
  عام:
  DEFINE_SIZE_UNION (2, format.v);
};


بنية EncodingRecord
{
  صحيح cmp (ثابت EncodingRecord &other) ثابت
  {
    صحيح ret;
    ret = platformID.cmp (other.platformID);
    إذا (ret) أرجع ret;
    إذا (other.encodingID != 0xFFFF)
    {
      ret = encodingID.cmp (other.encodingID);
      إذا (ret) أرجع ret;
    }
    أرجع 0;
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *base) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  subtable.sanitize (c, base));
  }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  EncodingRecord* copy (hb_serialize_context_t *c,
			Iterator it,
			unsigned format,
			ثابت فراغ *base,
			ثابت hb_subset_plan_t *plan,
			/* INOUT */ unsigned *objidx) ثابت
  {
    TRACE_SERIALIZE (هذا);
    auto snap = c->snapshot ();
    auto *out = c->embed (هذا);
    إذا (unlikely (!out)) return_trace (nullptr);
    out->subtable = 0;

    إذا (*objidx == 0)
    {
      CmapSubtable *cmapsubtable = c->push<CmapSubtable> ();
      unsigned origin_length = c->length ();
      cmapsubtable->serialize (c, it, format, plan, &(base+subtable));
      إذا (c->length () > origin_length && !c->in_error()) *objidx = c->چرو_pack ();
      وإلا c->چرو_discard ();
    }

    إذا (*objidx == 0)
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
  عام:
  DEFINE_SIZE_STATIC (8);
};

بنية cmap;

بنية SubtableUnicodesCache {

 خاص:
  hb_blob_ptr_t<cmap> base_blob;
  ثابت حرف* base;
  unsigned صحيح table_length;
  hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> cached_unicodes;

 عام:

  ساكن SubtableUnicodesCache* create (hb_blob_ptr_t<cmap> source_table)
  {
    SubtableUnicodesCache* cache =
        (SubtableUnicodesCache*) hb_malloc (حجم(SubtableUnicodesCache));

    إذا (unlikely (!cache))
      أرجع nullptr;

    جديد (cache) SubtableUnicodesCache (source_table);
    أرجع cache;
  }

  ساكن فراغ destroy (فراغ* value) {
    إذا (!value) أرجع;

    SubtableUnicodesCache* cache = (SubtableUnicodesCache*) value;
    cache->~SubtableUnicodesCache ();
    hb_free (cache);
  }

  SubtableUnicodesCache(ثابت فراغ* cmap_base,
			unsigned صحيح table_length_ = 0)
      : base_blob(),
        base ((ثابت حرف*) cmap_base),
        table_length (table_length_),
        cached_unicodes ()
  {}

  SubtableUnicodesCache(hb_blob_ptr_t<cmap> base_blob_)
      : base_blob(base_blob_),
        base ((ثابت حرف *) base_blob.get()),
        table_length (base_blob.get_length ()),
        cached_unicodes ()
  {}

  ~SubtableUnicodesCache()
  {
    base_blob.destroy ();
  }

  منطقي same_base(ثابت فراغ* other) ثابت
  {
    أرجع other == (ثابت فراغ*) base;
  }

  ثابت hb_set_t* set_for (ثابت EncodingRecord* record,
                           SubtableUnicodesCache& mutable_cache) ثابت
  {
    إذا (cached_unicodes.has ((unsigned) ((ثابت حرف *) record - base)))
      أرجع cached_unicodes.get ((unsigned) ((ثابت حرف *) record - base));

    أرجع mutable_cache.set_for (record);
  }

  ثابت hb_set_t* set_for (ثابت EncodingRecord* record)
  {
    إذا (!cached_unicodes.has ((unsigned) ((ثابت حرف *) record - base)))
    {
      hb_set_t *s = hb_set_create ();
      إذا (unlikely (s->in_error ()))
	أرجع hb_set_get_empty ();

      unsigned صحيح subtable_data_size = record->subtable < table_length
					? table_length - (unsigned صحيح) record->subtable
					: 0;
      (base+record->subtable).collect_unicodes (s, UINT_MAX, subtable_data_size);

      إذا (unlikely (!cached_unicodes.set ((unsigned) ((ثابت حرف *) record - base), hb::unique_ptr<hb_set_t> {s})))
        أرجع hb_set_get_empty ();

      أرجع s;
    }
    أرجع cached_unicodes.get ((unsigned) ((ثابت حرف *) record - base));
  }

};

ساكن مضمن uint16_t
hb_علام_pua_map (unsigned codepoint)
{
  إذا (codepoint <= 0x00FFu)
  {
    /* For symbol-encoded OpenType fonts, we duplicate the
     * U+F000..F0FF range at U+0000..U+00FF.  That's what
     * Windows seems to do, and that's hinted about at:
     * https://docs.microsoft.com/en-us/typography/opentype/spec/recom
     * under "Non-Standard (Symbol) Fonts". */
    أرجع 0xF000u + codepoint;
  }
  أرجع 0;
}

بنية cmap
{
  ساكن constexpr hb_tag_t tableTag = HB_OT_TAG_cmap;


  ساكن SubtableUnicodesCache* create_filled_cache(hb_blob_ptr_t<cmap> source_table) {
    ثابت cmap* cmap = source_table.get();
    auto it =
    + hb_iter (cmap->encodingRecord)
    | hb_filter ([&](ثابت EncodingRecord& _) {
      أرجع cmap::filter_encoding_records_for_subset (cmap, _);
    })
    ;

    SubtableUnicodesCache* cache = SubtableUnicodesCache::create(source_table);

    إذا (unlikely (!cache))
      أرجع nullptr;

    لكل (ثابت EncodingRecord& _ : it)
      cache->set_for(&_); // populate the cache for this encoding record.

    أرجع cache;
  }

  template<typename Iterator, typename EncodingRecIter,
	   hb_requires (hb_is_iterator (EncodingRecIter))>
  منطقي serialize (hb_serialize_context_t *c,
		  Iterator it,
		  EncodingRecIter encodingrec_iter,
		  ثابت فراغ *base,
		  hb_subset_plan_t *plan,
	                  منطقي drop_format_4 = خطأ,
			  unsigned صحيح source_table_length = 0)
  {
    إذا (unlikely (!c->extend_min ((*هذا))))  أرجع خطأ;
    هذا->version = 0;

    unsigned format4objidx = 0, format12objidx = 0, format14objidx = 0;
#ifndef HB_NO_BEYOND_64K
    unsigned format15objidx = 0;
#endif
    auto snap = c->snapshot ();

    SubtableUnicodesCache local_unicodes_cache (base, source_table_length);
    ثابت SubtableUnicodesCache* unicodes_cache = &local_unicodes_cache;

    إذا (plan->accelerator &&
        plan->accelerator->cmap_cache &&
        plan->accelerator->cmap_cache->same_base (base))
      unicodes_cache = plan->accelerator->cmap_cache;

    لكل (ثابت EncodingRecord& _ : encodingrec_iter)
    {
      إذا (c->in_error ())
        أرجع خطأ;

      unsigned format = (base+_.subtable).u.format.v;
      إذا (format != 4 && format != 12 && format != 14
#ifndef HB_NO_BEYOND_64K
	  && format != 15
#endif
	 ) استمر;

      ثابت hb_set_t* unicodes_set = unicodes_cache->set_for (&_, local_unicodes_cache);

      إذا (!drop_format_4 && format == 4)
      {
        c->copy (_, + it | hb_filter (*unicodes_set, hb_first), 4u, base, plan, &format4objidx);
        إذا (c->in_error () && c->only_overflow ())
        {
          // cmap4 overflowed, reset and retry serialization without format 4 subtables.
          c->revert (snap);
          أرجع serialize (c, it,
                            encodingrec_iter,
                            base,
                            plan,
                            true,
			    source_table_length);
        }
      }

      وإلا إذا (format == 12)
      {
        إذا (_can_drop (_,
                       *unicodes_set,
                       base,
                       *unicodes_cache,
                       local_unicodes_cache,
                       + it | hb_map (hb_first), encodingrec_iter))
          استمر;
        c->copy (_, + it | hb_filter (*unicodes_set, hb_first), 12u, base, plan, &format12objidx);
      }
      وإلا إذا (format == 14) c->copy (_, it, 14u, base, plan, &format14objidx);
#ifndef HB_NO_BEYOND_64K
      وإلا إذا (format == 15) c->copy (_, it, 15u, base, plan, &format15objidx);
#endif
    }
        unsigned length = c->length ();
        unsigned available = length > cmap::min_size ? length - cmap::min_size : 0;
        c->check_assign(هذا->encodingRecord.len,
                        available / EncodingRecord::static_size,
                        HB_SERIALIZE_ERROR_INT_OVERFLOW);

    // Fail if format 4 was dropped and there is no cmap12.
    أرجع !drop_format_4 || format12objidx;
  }

  template<typename Iterator, typename EncodingRecordIterator,
      hb_requires (hb_is_iterator (Iterator)),
      hb_requires (hb_is_iterator (EncodingRecordIterator))>
  منطقي _can_drop (ثابت EncodingRecord& cmap12,
                  ثابت hb_set_t& cmap12_unicodes,
                  ثابت فراغ* base,
                  ثابت SubtableUnicodesCache& unicodes_cache,
                  SubtableUnicodesCache& local_unicodes_cache,
                  Iterator subset_unicodes,
                  EncodingRecordIterator encoding_records)
  {
    لكل (auto cp : + subset_unicodes | hb_filter (cmap12_unicodes))
    {
      إذا (cp >= 0x10000) أرجع خطأ;
    }

    unsigned target_platform;
    unsigned target_encoding;
    unsigned target_language = (base+cmap12.subtable).get_language ();

    إذا (cmap12.platformID == 0 && cmap12.encodingID == 4)
    {
      target_platform = 0;
      target_encoding = 3;
    } وإلا إذا (cmap12.platformID == 3 && cmap12.encodingID == 10) {
      target_platform = 3;
      target_encoding = 1;
    } وإلا {
      أرجع خطأ;
    }

    لكل (ثابت auto& _ : encoding_records)
    {
      إذا (_.platformID != target_platform
          || _.encodingID != target_encoding
          || (base+_.subtable).get_language() != target_language)
        استمر;

      ثابت hb_set_t* sibling_unicodes = unicodes_cache.set_for (&_, local_unicodes_cache);

      auto cmap12 = + subset_unicodes | hb_filter (cmap12_unicodes);
      auto sibling = + subset_unicodes | hb_filter (*sibling_unicodes);
      لكل (; cmap12 && sibling; cmap12++, sibling++)
      {
        unsigned a = *cmap12;
        unsigned b = *sibling;
        إذا (a != b) أرجع خطأ;
      }

      أرجع !cmap12 && !sibling;
    }

    أرجع خطأ;
  }

  فراغ closure_glyphs (ثابت hb_set_t      *unicodes,
		       hb_set_t            *glyphset) ثابت
  {
    + hb_iter (encodingRecord)
    | hb_map (&EncodingRecord::subtable)
    | hb_map (hb_add (هذا))
    | hb_filter (&CmapSubtable::is_variation_selector)
    | hb_apply ([=] (ثابت CmapSubtable& _) { _.closure_glyphs (unicodes, glyphset); })
    ;
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  { أرجع subset (c, خطأ); }

  منطقي subset (hb_subset_context_t *c, منطقي is_dmap) ثابت
  {
    TRACE_SUBSET (هذا);

    cmap *cmap_فرك = c->serializer->start_embed<cmap> ();

    auto encodingrec_iter =
    + hb_iter (encodingRecord)
    | hb_filter ([&](ثابت EncodingRecord& _) {
      أرجع cmap::filter_encoding_records_for_subset (هذا, _);
    })
    ;

    إذا (unlikely (!encodingrec_iter.len ())) return_trace (خطأ);

    ثابت EncodingRecord *unicode_bmp= nullptr, *unicode_ucs4 = nullptr, *ms_bmp = nullptr, *ms_ucs4 = nullptr;
    منطقي has_format12 = خطأ;

    لكل (ثابت EncodingRecord& _ : encodingrec_iter)
    {
      unsigned format = (هذا + _.subtable).u.format.v;
      إذا (format == 12) has_format12 = true;

      ثابت EncodingRecord *table = std::addressof (_);
      إذا      (_.platformID == 0 && _.encodingID ==  3) unicode_bmp = table;
      وإلا إذا (_.platformID == 0 && _.encodingID ==  4) unicode_ucs4 = table;
      وإلا إذا (_.platformID == 3 && _.encodingID ==  1) ms_bmp = table;
      وإلا إذا (_.platformID == 3 && _.encodingID == 10) ms_ucs4 = table;
    }

    إذا (unlikely (!is_dmap && !has_format12 && !unicode_bmp && !ms_bmp)) return_trace (خطأ);
    إذا (unlikely (!is_dmap && has_format12 && (!unicode_ucs4 && !ms_ucs4))) return_trace (خطأ);

    auto it =
    + c->plan->unicode_to_new_gid_list.iter ()
    | hb_filter ([&] (ثابت hb_codepoint_pair_t _)
		 { أرجع (_.second != HB_MAP_درج_INVALID); })
    ;

    return_trace (cmap_فرك->serialize (c->serializer,
                                         it,
                                         encodingrec_iter,
                                         هذا,
                                         c->plan,
					 خطأ,
					 c->source_blob->length));
  }

  ثابت CmapSubtable *find_best_subtable (منطقي *علام = nullptr,
					  منطقي *mac = nullptr,
					  منطقي *macroman = nullptr) ثابت
  {
    إذا (علام) *علام = خطأ;
    إذا (mac) *mac = خطأ;
    إذا (macroman) *macroman = خطأ;

    ثابت CmapSubtable *subtable;

    /* Symbol subtable.
     * Prefer symbol if available.
     * https://github.com/harfbuzz/harfbuzz/issues/1918 */
    إذا ((subtable = هذا->find_subtable (3, 0)))
    {
      إذا (علام) *علام = true;
      أرجع subtable;
    }

    /* 32-bit subtables. */
    إذا ((subtable = هذا->find_subtable (3, 10))) أرجع subtable;
    إذا ((subtable = هذا->find_subtable (0, 6))) أرجع subtable;
    إذا ((subtable = هذا->find_subtable (0, 4))) أرجع subtable;

    /* 16-bit subtables. */
    إذا ((subtable = هذا->find_subtable (3, 1))) أرجع subtable;
    إذا ((subtable = هذا->find_subtable (0, 3))) أرجع subtable;
    إذا ((subtable = هذا->find_subtable (0, 2))) أرجع subtable;
    إذا ((subtable = هذا->find_subtable (0, 1))) أرجع subtable;
    إذا ((subtable = هذا->find_subtable (0, 0))) أرجع subtable;

    /* MacRoman subtable. */
    إذا ((subtable = هذا->find_subtable (1, 0)))
    {
      إذا (mac) *mac = true;
      إذا (macroman) *macroman = true;
      أرجع subtable;
    }
    /* Any other Mac subtable; we just map ASCII for these. */
    إذا ((subtable = هذا->find_subtable (1, 0xFFFF)))
    {
      إذا (mac) *mac = true;
      أرجع subtable;
    }

    /* Meh. */
    أرجع &Null (CmapSubtable);
  }

  بنية accelerator_t
  {
    using cache_t = hb_cache_t<21, 19>;
    static_assert (حجم (cache_t) == 1024, "");

    accelerator_t (hb_face_t *face, hb_tag_t table_tag = cmap::tableTag)
    {
      هذا->table = hb_sanitize_context_t ().reference_table<cmap> (face, table_tag);
      إذا (!هذا->table.get_length ())
	أرجع;

      منطقي علام, mac, macroman;
      هذا->subtable = table->find_best_subtable (&علام, &mac, &macroman);
      هذا->subtable_uvs = &Null (CmapSubtable);
      {
	ثابت CmapSubtable *st = table->find_variation_selector_subtable ();
	إذا (st)
	  subtable_uvs = st;
      }

#ifndef HB_NO_OT_FONT_CMAP_CACHE
      cache = (cache_t *) hb_malloc (حجم (cache_t));
      إذا (cache)
	جديد (cache) cache_t ();
      وإلا
        أرجع; // Such that get_glyph_funcZ remains null.
#endif

      هذا->get_glyph_data = subtable;
#ifndef HB_NO_CMAP_LEGACY_SUBTABLES
      منطقي is_format4 = subtable->u.format.v == 4;
      auto set_format4_getter = [هذا] (منطقي (*func) (ثابت فراغ *,
						      hb_codepoint_t,
						      hb_codepoint_t *))
      {
	هذا->format4_accel.init (&هذا->subtable->u.format4,
				  get_subtable_data_size (هذا->subtable));
	هذا->get_glyph_data = &هذا->format4_accel;
	هذا->get_glyph_funcZ = func;
      };
      إذا (unlikely (علام))
      {
	بدل ((unsigned) face->table.OS2->get_font_page ()) {
	حالة OS2::font_page_t::FONT_PAGE_NONE:
	  إذا (is_format4)
	    set_format4_getter (get_glyph_from_علام<CmapSubtableFormat4::accelerator_t, hb_علام_pua_map>);
	  وإلا
	    هذا->get_glyph_funcZ = get_glyph_from_علام<CmapSubtable, hb_علام_pua_map>;
	  اكسر;
#ifndef HB_NO_OT_SHAPER_ARABIC_FALLBACK
	حالة OS2::font_page_t::FONT_PAGE_SIMP_ARABIC:
	  إذا (is_format4)
	    set_format4_getter (get_glyph_from_علام<CmapSubtableFormat4::accelerator_t, _hb_arabic_pua_simp_map>);
	  وإلا
	    هذا->get_glyph_funcZ = get_glyph_from_علام<CmapSubtable, _hb_arabic_pua_simp_map>;
	  اكسر;
	حالة OS2::font_page_t::FONT_PAGE_TRAD_ARABIC:
	  إذا (is_format4)
	    set_format4_getter (get_glyph_from_علام<CmapSubtableFormat4::accelerator_t, _hb_arabic_pua_trad_map>);
	  وإلا
	    هذا->get_glyph_funcZ = get_glyph_from_علام<CmapSubtable, _hb_arabic_pua_trad_map>;
	  اكسر;
#endif
	افتراضي:
	  إذا (is_format4)
	    set_format4_getter (get_glyph_from<CmapSubtableFormat4::accelerator_t>);
	  وإلا
	    هذا->get_glyph_funcZ = get_glyph_from<CmapSubtable>;
	  اكسر;
	}
      }
      وإلا إذا (unlikely (macroman))
      {
	إذا (is_format4)
	  set_format4_getter (get_glyph_from_macroman<CmapSubtableFormat4::accelerator_t>);
	وإلا
	  هذا->get_glyph_funcZ = get_glyph_from_macroman<CmapSubtable>;
      }
      وإلا إذا (unlikely (mac))
      {
	إذا (is_format4)
	  set_format4_getter (get_glyph_from_ascii<CmapSubtableFormat4::accelerator_t>);
	وإلا
	  هذا->get_glyph_funcZ = get_glyph_from_ascii<CmapSubtable>;
      }
      وإلا
#endif
      {
	بدل (subtable->u.format.v) {
	  /* Accelerate format 4 and format 12. */
	  افتراضي:
	    هذا->get_glyph_funcZ = get_glyph_from<CmapSubtable>;
	    اكسر;
	  حالة 12:
	    هذا->get_glyph_funcZ = get_glyph_from<CmapSubtableFormat12>;
	    اكسر;
	  حالة  4:
	  {
	    hb_barrier ();
	    هذا->format4_accel.init (&subtable->u.format4,
				      get_subtable_data_size (subtable));
	    هذا->get_glyph_data = &هذا->format4_accel;
	    هذا->get_glyph_funcZ = هذا->format4_accel.get_glyph_func;
	    اكسر;
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

    مضمن منطقي _cached_get (hb_codepoint_t unicode,
			     hb_codepoint_t *glyph) ثابت
    {
#ifndef HB_NO_OT_FONT_CMAP_CACHE
      // cache is always non-null if we have a get_glyph_funcZ
      unsigned v;
      إذا (cache->get (unicode, &v))
      {
        *glyph = v;
	أرجع true;
      }
#endif
      منطقي ret  = هذا->get_glyph_funcZ (هذا->get_glyph_data, unicode, glyph);

#ifndef HB_NO_OT_FONT_CMAP_CACHE
      إذا (ret)
        cache->set (unicode, *glyph);
#endif

      أرجع ret;
    }

    منطقي get_nominal_glyph (hb_codepoint_t  unicode,
			    hb_codepoint_t *glyph) ثابت
    {
      إذا (unlikely (!هذا->get_glyph_funcZ)) أرجع خطأ;
      أرجع _cached_get (unicode, glyph);
    }

    unsigned صحيح get_nominal_glyphs (unsigned صحيح كدي,
				     ثابت hb_codepoint_t *first_unicode,
				     unsigned صحيح unicode_stride,
				     hb_codepoint_t *first_glyph,
				     unsigned صحيح glyph_stride) ثابت
    {
      إذا (unlikely (!هذا->get_glyph_funcZ)) أرجع 0;

      unsigned صحيح done;
      لكل (done = 0;
	   done < كدي && _cached_get (*first_unicode, first_glyph);
	   done++)
      {
	first_unicode = &StructAtOffsetUnaligned<hb_codepoint_t> (first_unicode, unicode_stride);
	first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      }
      أرجع done;
    }

    منطقي get_variation_glyph (hb_codepoint_t  unicode,
			      hb_codepoint_t  variation_selector,
			      hb_codepoint_t *glyph) ثابت
    {
      بدل (get_glyph_variant (unicode, variation_selector, glyph))
      {
	حالة GLYPH_VARIANT_NOT_FOUND:	أرجع خطأ;
	حالة GLYPH_VARIANT_FOUND:	أرجع true;
	حالة GLYPH_VARIANT_USE_DEFAULT:	اكسر;
      }

      أرجع get_nominal_glyph (unicode, glyph);
    }

    glyph_variant_t get_glyph_variant (hb_codepoint_t  unicode,
				       hb_codepoint_t  variation_selector,
				       hb_codepoint_t *glyph) ثابت
    {
      أرجع هذا->subtable_uvs->get_glyph_variant (unicode,
						    variation_selector,
						    glyph);
    }

    منطقي has_data () ثابت { أرجع table.get_length (); }

    فراغ collect_unicodes (hb_set_t *out, unsigned صحيح لمب_glyphs) ثابت
    { subtable->collect_unicodes (out, لمب_glyphs, get_subtable_data_size (subtable)); }
    فراغ collect_mapping (hb_set_t *unicodes, hb_map_t *mapping,
			  unsigned لمب_glyphs = UINT_MAX) ثابت
    { subtable->collect_mapping (unicodes, mapping, لمب_glyphs, get_subtable_data_size (subtable)); }
    فراغ collect_variation_selectors (hb_set_t *out) ثابت
    { subtable_uvs->collect_variation_selectors (out); }
    فراغ collect_variation_unicodes (hb_codepoint_t variation_selector,
				     hb_set_t *out) ثابت
    { subtable_uvs->collect_variation_unicodes (variation_selector, out); }

    محمي:
    تعريفنوع منطقي (*hb_cmap_get_glyph_func_t) (ثابت فراغ *obj,
					      hb_codepoint_t codepoint,
					      hb_codepoint_t *glyph);
    تعريفنوع uint16_t (*hb_pua_remap_func_t) (unsigned);

    template <typename Type>
    HB_INTERNAL ساكن منطقي get_glyph_from (ثابت فراغ *obj,
					    hb_codepoint_t codepoint,
					    hb_codepoint_t *glyph)
    {
      ثابت Type *typed_obj = (ثابت Type *) obj;
      أرجع typed_obj->get_glyph (codepoint, glyph);
    }

    template <typename Type, hb_pua_remap_func_t remap>
    HB_INTERNAL ساكن منطقي get_glyph_from_علام (ثابت فراغ *obj,
						   hb_codepoint_t codepoint,
						   hb_codepoint_t *glyph)
    {
      ثابت Type *typed_obj = (ثابت Type *) obj;
      إذا (likely (typed_obj->get_glyph (codepoint, glyph)))
	أرجع true;

      إذا (hb_codepoint_t c = remap (codepoint))
	أرجع typed_obj->get_glyph (c, glyph);

      أرجع خطأ;
    }

    template <typename Type>
    HB_INTERNAL ساكن منطقي get_glyph_from_ascii (ثابت فراغ *obj,
						  hb_codepoint_t codepoint,
						  hb_codepoint_t *glyph)
    {
      ثابت Type *typed_obj = (ثابت Type *) obj;
      أرجع codepoint < 0x80 && typed_obj->get_glyph (codepoint, glyph);
    }

    template <typename Type>
    HB_INTERNAL ساكن منطقي get_glyph_from_macroman (ثابت فراغ *obj,
						     hb_codepoint_t codepoint,
						     hb_codepoint_t *glyph)
    {
      إذا (get_glyph_from_ascii<Type> (obj, codepoint, glyph))
	أرجع true;

      ثابت Type *typed_obj = (ثابت Type *) obj;
      unsigned c = unicode_to_macroman (codepoint);
      أرجع c && typed_obj->get_glyph (c, glyph);
    }

    unsigned صحيح get_subtable_data_size (ثابت CmapSubtable *subtable) ثابت
    {
      unsigned صحيح table_length = هذا->table.get_length ();
      uintptr_t table_start = (uintptr_t) (ثابت فراغ *) هذا->table.get ();
      uintptr_t subtable_addr = (uintptr_t) (ثابت فراغ *) subtable;
      إذا (unlikely (subtable_addr < table_start))
	أرجع 0;

      uintptr_t subtable_offset = subtable_addr - table_start;
      إذا (unlikely (subtable_offset >= table_length))
	أرجع 0;

      أرجع table_length - (unsigned صحيح) subtable_offset;
    }

    خاص:
    hb_nonnull_ptr_t<ثابت CmapSubtable> subtable;
    hb_nonnull_ptr_t<ثابت CmapSubtable> subtable_uvs;

    hb_cmap_get_glyph_func_t get_glyph_funcZ = nullptr;
    ثابت فراغ *get_glyph_data = nullptr;

    CmapSubtableFormat4::accelerator_t format4_accel;

#ifndef HB_NO_OT_FONT_CMAP_CACHE
    cache_t *cache = nullptr;
#endif

    عام:
    hb_blob_ptr_t<cmap> table;
  };

  محمي:

  ثابت CmapSubtable *find_subtable (unsigned صحيح platform_id,
				     unsigned صحيح encoding_id) ثابت
  {
    EncodingRecord key;
    key.platformID = platform_id;
    key.encodingID = encoding_id;

    ثابت EncodingRecord &result = encodingRecord.bsearch (key);
    إذا (!result.subtable)
      أرجع nullptr;

    أرجع &(هذا+result.subtable);
  }

  ثابت CmapSubtable *find_variation_selector_subtable () ثابت
  {
    ثابت CmapSubtable *format14 = nullptr;
    لكل (ثابت EncodingRecord& record : encodingRecord)
    {
      إذا (record.platformID != 0 || record.encodingID != 5)
	استمر;

      ثابت CmapSubtable *subtable = &(هذا+record.subtable);
#ifndef HB_NO_BEYOND_64K
      إذا (subtable->u.format.v == 15)
	أرجع subtable;
#endif
      إذا (subtable->u.format.v == 14)
	format14 = subtable;
    }
    أرجع format14;
  }

  عام:

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  likely (version == 0) &&
		  encodingRecord.sanitize (c, هذا));
  }

 خاص:

  ساكن منطقي filter_encoding_records_for_subset(ثابت cmap* cmap,
                                                 ثابت EncodingRecord& _)
  {
    أرجع
        (_.platformID == 0 && _.encodingID == 3) ||
        (_.platformID == 0 && _.encodingID == 4) ||
        (_.platformID == 3 && _.encodingID == 1) ||
        (_.platformID == 3 && _.encodingID == 10) ||
        (cmap + _.subtable).is_variation_selector ();
  }

  محمي:
  HBUINT16	version;	/* Table version number (0). */
  SortedArray16Of<EncodingRecord>
		encodingRecord;	/* Encoding tables. */
  عام:
  DEFINE_SIZE_ARRAY (4, encodingRecord);
};

بنية DMAP : cmap
{
  ساكن constexpr hb_tag_t tableTag = HB_OT_TAG_DMAP;

  منطقي subset (hb_subset_context_t *c) ثابت
  { أرجع cmap::subset (c, true); }
};

بنية cmap_accelerator_t
{
  cmap_accelerator_t (hb_face_t *face) :
    dmap (face, HB_OT_TAG_DMAP),
    base (face)
  {}

  منطقي get_nominal_glyph (hb_codepoint_t  unicode,
			  hb_codepoint_t *glyph) ثابت
  {
    أرجع dmap.get_nominal_glyph (unicode, glyph) ||
	   base.get_nominal_glyph (unicode, glyph);
  }

  unsigned صحيح get_nominal_glyphs (unsigned صحيح كدي,
				   ثابت hb_codepoint_t *first_unicode,
				   unsigned صحيح unicode_stride,
				   hb_codepoint_t *first_glyph,
				   unsigned صحيح glyph_stride) ثابت
  {
    إذا (likely (!dmap.has_data ()))
      أرجع base.get_nominal_glyphs (كدي,
				      first_unicode, unicode_stride,
				      first_glyph, glyph_stride);

    unsigned صحيح done;
    لكل (done = 0;
	 done < كدي && get_nominal_glyph (*first_unicode, first_glyph);
	 done++)
    {
      first_unicode = &StructAtOffsetUnaligned<hb_codepoint_t> (first_unicode, unicode_stride);
      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
    }
    أرجع done;
  }

  منطقي get_variation_glyph (hb_codepoint_t  unicode,
			    hb_codepoint_t  variation_selector,
			    hb_codepoint_t *glyph) ثابت
  {
    glyph_variant_t result = dmap.get_glyph_variant (unicode,
						     variation_selector,
						     glyph);
    إذا (result == GLYPH_VARIANT_NOT_FOUND)
      result = base.get_glyph_variant (unicode, variation_selector, glyph);

    بدل (result)
    {
      حالة GLYPH_VARIANT_NOT_FOUND: أرجع خطأ;
      حالة GLYPH_VARIANT_FOUND: أرجع true;
      حالة GLYPH_VARIANT_USE_DEFAULT: اكسر;
    }

    أرجع get_nominal_glyph (unicode, glyph);
  }

  فراغ collect_unicodes (hb_set_t *out, unsigned صحيح لمب_glyphs) ثابت
  {
    base.collect_unicodes (out, لمب_glyphs);
    dmap.collect_unicodes (out, لمب_glyphs);
  }

  فراغ collect_mapping (hb_set_t *unicodes, hb_map_t *mapping,
			unsigned لمب_glyphs = UINT_MAX) ثابت
  {
    base.collect_mapping (unicodes, mapping, لمب_glyphs);
    dmap.collect_mapping (unicodes, mapping, لمب_glyphs);
  }

  فراغ collect_variation_selectors (hb_set_t *out) ثابت
  {
    base.collect_variation_selectors (out);
    dmap.collect_variation_selectors (out);
  }

  فراغ collect_variation_unicodes (hb_codepoint_t variation_selector,
				   hb_set_t *out) ثابت
  {
    base.collect_variation_unicodes (variation_selector, out);
    dmap.collect_variation_unicodes (variation_selector, out);
  }

  فراغ closure_glyphs (ثابت hb_set_t *unicodes,
		       hb_set_t       *glyphset) ثابت
  {
    إذا (likely (!dmap.has_data ()))
    {
      base.table->closure_glyphs (unicodes, glyphset);
      أرجع;
    }

    hb_set_t variation_selectors;
    collect_variation_selectors (&variation_selectors);
    لكل (hb_codepoint_t variation_selector : variation_selectors)
    {
      إذا (!unicodes->has (variation_selector))
	استمر;

      hb_set_t variation_unicodes;
      collect_variation_unicodes (variation_selector, &variation_unicodes);
      لكل (hb_codepoint_t unicode : variation_unicodes)
      {
	إذا (!unicodes->has (unicode))
	  استمر;

	hb_codepoint_t glyph;
	إذا (get_variation_glyph (unicode, variation_selector, &glyph))
	  glyphset->add (glyph);
      }
    }
  }

  cmap::accelerator_t dmap;
  cmap::accelerator_t base;
};

} /* namespace OT */


#endif /* HB_OT_CMAP_TABLE_HH */
