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

#அந்தர்பாவ "hb-ot-os2-table.hh"
#அந்தர்பாவ "hb-ot-shaper-arabic-pua.hh"
#அந்தர்பாவ "hb-open-type.hh"
#அந்தர்பாவ "hb-ot-dual.hh"
#அந்தர்பாவ "hb-set.hh"
#அந்தர்பாவ "hb-cache.hh"

/*
 * cmap -- Character to Glyph Index Mapping
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cmap
 */
#define HB_OT_TAG_cmap HB_TAG('c','m','a','p')
#define HB_OT_TAG_DMAP HB_TAG('D','M','A','P')

நாமாகாஶ OT {

நிலையான உள்வரி uint8_t unicode_to_macroman (hb_codepoint_t u)
{
  நிலையான மாறிலி கட்டமைப்பு unicode_to_macroman_t
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
  auto *c = hb_bsearch (u, mapping, ARRAY_LENGTH (mapping), அளவு (mapping[0]),
			_hb_cmp_operator<uint16_t, uint16_t>);
  திரும்பு c ? c->macroman : 0;
}

கட்டமைப்பு CmapSubtableFormat0
{
  தருக்க get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) மாறிலி
  {
    hb_codepoint_t gid = codepoint < 256 ? glyphIdArray[codepoint] : 0;
    எனில் (unlikely (!gid))
      திரும்பு பொய்;
    *glyph = gid;
    திரும்பு உண்மை;
  }

  unsigned get_language () மாறிலி
  {
    திரும்பு language;
  }

  வெறுமை collect_unicodes (hb_set_t *out) மாறிலி
  {
    க்கு (unsigned முழு i = 0; i < 256; i++)
      எனில் (glyphIdArray[i])
	out->add (i);
  }

  வெறுமை collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping /* OUT */) மாறிலி
  {
    க்கு (unsigned i = 0; i < 256; i++)
      எனில் (glyphIdArray[i])
      {
	hb_codepoint_t glyph = glyphIdArray[i];
	unicodes->add (i);
	mapping->set (i, glyph);
      }
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது));
  }

  பாதுகாக்கப்பட்ட:
  HBUINT16	format;		/* Format number is set to 0. */
  HBUINT16	length;		/* Byte length of this subtable. */
  HBUINT16	language;	/* Ignore. */
  HBUINT8	glyphIdArray[256];/* An array that maps character
				 * code to glyph index values. */
  பொது:
  DEFINE_SIZE_STATIC (6 + 256);
};

கட்டமைப்பு CmapSubtableFormat4
{
  template<typename Iterator,
      typename Writer,
	   hb_requires (hb_is_iterator (Iterator))>
  வெறுமை to_ranges (Iterator it, Writer& range_writer)
  {
    hb_codepoint_t start_cp = 0, prev_run_start_cp = 0, run_start_cp = 0, end_cp = 0, last_gid = 0;
    முழு run_length = 0 , delta = 0, prev_delta = 0;

    எண்ணிக்கை {
      FIRST_SUB_RANGE,
      FOLLOWING_SUB_RANGE,
    } mode;

    போது (it) {
      // Start a new range
      {
        மாறிலி auto& pair = *it;
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

      போது (it) {
        // Process range
        மாறிலி auto& pair = *it;
        hb_codepoint_t next_cp = pair.first;
        hb_codepoint_t next_gid = pair.second;
        எனில் (next_cp != end_cp + 1) {
          // Current range is over, stop processing.
          நிறுத்து;
        }

        எனில் (next_gid == last_gid + 1) {
          // The current run continues.
          end_cp = next_cp;
          run_length++;
          last_gid = next_gid;
          it++;
          தொடர்;
        }

        // A new run is starting, decide if we want to commit the current run.
        முழு split_cost = (mode == FIRST_SUB_RANGE) ? 8 : 16;
        முழு run_cost = run_length * 2;
        எனில் (run_cost >= split_cost) {
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

    எனில் (likely (end_cp != 0xFFFF)) {
      range_writer (0xFFFF, 0xFFFF, 1);
    }
  }

  /*
   * Writes the current range as either one or two ranges depending on what is most efficient.
   */
  template<typename Writer>
  வெறுமை commit_current_range (hb_codepoint_t start,
                             hb_codepoint_t prev_run_start,
                             hb_codepoint_t run_start,
                             hb_codepoint_t end,
                             முழு run_delta,
                             முழு previous_run_delta,
                             முழு split_cost,
                             Writer& range_writer) {
    தருக்க should_split = பொய்;
    எனில் (start < run_start && run_start < end) {
      முழு run_cost = (end - run_start + 1) * 2;
      எனில் (run_cost >= split_cost) {
        should_split = உண்மை;
      }
    }

    // A zero delta is also the marker for ranges that need a
    // glyphIdArray; serialize_rangeoffset_glyid() tells the two cases
    // apart by re-checking the mapping.
    எனில் (should_split) {
      எனில் (start == prev_run_start)
        range_writer (start, run_start - 1, previous_run_delta);
      இல்லையெனில்
        range_writer (start, run_start - 1, 0);
      range_writer (run_start, end, run_delta);
      திரும்பு;
    }


    எனில் (start == run_start) {
      // Range is only a run
      range_writer (start, end, run_delta);
      திரும்பு;
    }

    // Write only a single non-run range.
    range_writer (start, end, 0);
  }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  unsigned serialize_find_segcount (Iterator it) {
    கட்டமைப்பு Counter {
      unsigned segcount = 0;

      வெறுமை operator() (hb_codepoint_t start,
                       hb_codepoint_t end,
                       முழு delta) {
        segcount++;
      }
    } counter;

    to_ranges (+it, counter);
    திரும்பு counter.segcount;
  }


  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  தருக்க serialize_start_end_delta_arrays (hb_serialize_context_t *c,
                                         Iterator it,
                                         முழு segcount)
  {
    கட்டமைப்பு Writer {
      hb_serialize_context_t *serializer_;
      HBUINT16* end_code_;
      HBUINT16* start_code_;
      HBINT16* id_delta_;
      முழு index_;

      Writer(hb_serialize_context_t *serializer)
          : serializer_(serializer),
            end_code_(nullptr),
            start_code_(nullptr),
            id_delta_(nullptr),
            index_ (0) {}
      வெறுமை operator() (hb_codepoint_t start,
                       hb_codepoint_t end,
                       முழு delta) {
        start_code_[index_] = start;
        end_code_[index_] = end;
        id_delta_[index_] = delta;
        index_++;
      }
    } writer(c);

    writer.end_code_ = c->allocate_size<HBUINT16> (HBUINT16::static_size * segcount, பொய்);
    (வெறுமை) c->allocate_size<HBUINT16> (2); // padding
    writer.start_code_ = c->allocate_size<HBUINT16> (HBUINT16::static_size * segcount, பொய்);
    writer.id_delta_ = c->allocate_size<HBINT16> (HBINT16::static_size * segcount, பொய்);

    எனில் (unlikely (!writer.end_code_ || !writer.start_code_ || !writer.id_delta_)) திரும்பு பொய்;

    to_ranges (+it, writer);
    திரும்பு உண்மை;
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
    எனில் (unlikely (!c->check_success (idRangeOffset))) திரும்பு nullptr;
    எனில் (unlikely ((எழுத்து *)idRangeOffset - (எழுத்து *)idDelta != (முழு) segcount * (முழு) HBINT16::static_size)) திரும்பு nullptr;

    க்கு (unsigned i : hb_range (segcount))
    {
      /* A non-zero delta is always a real run. A zero delta is
       * ambiguous: identity mapping, or the marker for a range that
       * needs a glyphIdArray. Only zero deltas need a check. */
      முழு delta = idDelta[i];
      எனில் (delta)
	தொடர்;
      தருக்க use_delta = உண்மை;
      க்கு (hb_codepoint_t cp = startCode[i]; cp <= endCode[i]; cp++)
      {
	hb_codepoint_t gid = cp_to_gid.get (cp);
	எனில் (gid == HB_MAP_மதிப்பு_INVALID)
	  gid = 0;
	எனில் (gid != cp)
	{
	  use_delta = பொய்;
	  நிறுத்து;
	}
      }
      எனில் (use_delta)
	தொடர்;

      idRangeOffset[i] = 2 * (c->start_embed<HBUINT16> () - idRangeOffset - i);
      க்கு (hb_codepoint_t cp = startCode[i]; cp <= endCode[i]; cp++)
      {
        HBUINT16 gid;
        gid = cp_to_gid[cp];
        c->copy<HBUINT16> (gid);
      }
    }

    திரும்பு idRangeOffset;
  }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  வெறுமை serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    auto format4_iter =
    + it
    | hb_filter ([&] (மாறிலி hb_codepoint_pair_t _)
		 { திரும்பு _.first <= 0xFFFF; })
    ;

    எனில் (!format4_iter) திரும்பு;

    unsigned table_initpos = c->length ();
    எனில் (unlikely (!c->extend_min (இது))) திரும்பு;
    இது->format = 4;

    hb_vector_t<hb_codepoint_pair_t> cp_to_gid {
      format4_iter
    };

    //serialize endCode[], startCode[], idDelta[]
    HBUINT16* endCode = c->start_embed<HBUINT16> ();
    unsigned segcount = serialize_find_segcount (cp_to_gid.iter());
    எனில் (unlikely (!serialize_start_end_delta_arrays (c, cp_to_gid.iter(), segcount)))
      திரும்பு;

    HBUINT16 *startCode = endCode + segcount + 1;
    HBINT16 *idDelta = ((HBINT16*)startCode) + segcount;

    HBUINT16 *idRangeOffset = serialize_rangeoffset_glyid (c,
                                                           cp_to_gid.iter (),
                                                           endCode,
                                                           startCode,
                                                           idDelta,
                                                           segcount);
    எனில் (unlikely (!c->check_success (idRangeOffset))) திரும்பு;

    இது->length = c->length () - table_initpos;
    எனில் ((long long) இது->length != (long long) c->length () - table_initpos)
    {
      c->err (HB_SERIALIZE_ERROR_INT_OVERFLOW);
      திரும்பு;
    }

    இது->seg_மொத்தம்_X2 = segcount * 2;
    இது->entrySelector = hb_max (1u, hb_bit_storage (segcount)) - 1;
    இது->தேடல்_Range = 2 * (1u << இது->entrySelector);
    இது->rangeShift = segcount * 2 > இது->தேடல்_Range
		       ? 2 * segcount - இது->தேடல்_Range
		       : 0;
  }

  unsigned get_language () மாறிலி
  {
    திரும்பு language;
  }

  கட்டமைப்பு accelerator_t
  {
    accelerator_t () {}
    accelerator_t (மாறிலி CmapSubtableFormat4 *subtable) = delete;

    வெறுமை init (மாறிலி CmapSubtableFormat4 *subtable,
	       unsigned முழு              subtable_data_size)
    {
      seg_மொத்தம்_2 = subtable->seg_மொத்தம்_X2 / 2;
      end_மொத்தம் = subtable->values.arrayZ;
      start_மொத்தம் = end_மொத்தம் + seg_மொத்தம்_2 + 1;
      idDelta = start_மொத்தம் + seg_மொத்தம்_2;
      idRangeOffset = idDelta + seg_மொத்தம்_2;
      glyphIdArray = idRangeOffset + seg_மொத்தம்_2;

      unsigned முழு மதிப்புகள்_offset = 16 + 8 * seg_மொத்தம்_2;
      glyphIdArrayLength = subtable_data_size > மதிப்புகள்_offset
			   ? (subtable_data_size - மதிப்புகள்_offset) / 2
			   : 0;
    }

    தருக்க get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) மாறிலி
    {
      கட்டமைப்பு CustomRange
      {
	முழு cmp (hb_codepoint_t k,
		 unsigned distance) மாறிலி
	{
	  எனில் (k > last) திரும்பு +1;
	  எனில் (k < (&last)[distance]/*first*/) திரும்பு -1;
	  திரும்பு 0;
	}
	HBUINT16 last;
      };

      மாறிலி HBUINT16 *found = hb_bsearch (codepoint,
					  இது->end_மொத்தம்,
					  இது->seg_மொத்தம்_2,
					  அளவு (CustomRange),
					  _hb_cmp_method<hb_codepoint_t, CustomRange, unsigned>,
					  இது->seg_மொத்தம்_2 + 1);
      எனில் (unlikely (!found))
	திரும்பு பொய்;
      unsigned முழு i = found - end_மொத்தம்;

      hb_codepoint_t gid;
      unsigned முழு rangeOffset = இது->idRangeOffset[i];
      எனில் (rangeOffset == 0)
	gid = codepoint + இது->idDelta[i];
      இல்லையெனில்
      {
	/* Somebody has been smoking... */
	unsigned முழு index = rangeOffset / 2 + (codepoint - இது->start_மொத்தம்[i]) + i - இது->seg_மொத்தம்_2;
	எனில் (unlikely (index >= இது->glyphIdArrayLength))
	  திரும்பு பொய்;
	gid = இது->glyphIdArray[index];
	எனில் (unlikely (!gid))
	  திரும்பு பொய்;
	gid += இது->idDelta[i];
      }
      gid &= 0xFFFFu;
      எனில் (unlikely (!gid))
	திரும்பு பொய்;
      *glyph = gid;
      திரும்பு உண்மை;
    }

    HB_INTERNAL நிலையான தருக்க get_glyph_func (மாறிலி வெறுமை *obj, hb_codepoint_t codepoint, hb_codepoint_t *glyph)
    { திரும்பு ((மாறிலி accelerator_t *) obj)->get_glyph (codepoint, glyph); }

    வெறுமை collect_unicodes (hb_set_t *out) மாறிலி
    {
      unsigned முழு மொத்தம் = இது->seg_மொத்தம்_2;
      எனில் (மொத்தம் && இது->start_மொத்தம்[மொத்தம் - 1] == 0xFFFFu)
	மொத்தம்--; /* Skip sentinel segment. */
      க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
      {
	hb_codepoint_t start = இது->start_மொத்தம்[i];
	hb_codepoint_t end = இது->end_மொத்தம்[i];
	unsigned முழு rangeOffset = இது->idRangeOffset[i];
        out->add_range(start, end);
	எனில் (rangeOffset == 0)
	{
	  க்கு (hb_codepoint_t codepoint = start; codepoint <= end; codepoint++)
	  {
	    hb_codepoint_t gid = (codepoint + இது->idDelta[i]) & 0xFFFFu;
	    எனில் (unlikely (!gid))
              out->del(codepoint);
	  }
	}
	இல்லையெனில்
	{
	  க்கு (hb_codepoint_t codepoint = start; codepoint <= end; codepoint++)
	  {
	    unsigned முழு index = rangeOffset / 2 + (codepoint - இது->start_மொத்தம்[i]) + i - இது->seg_மொத்தம்_2;
	    எனில் (unlikely (index >= இது->glyphIdArrayLength))
            {
              out->del_range (codepoint, end);
	      நிறுத்து;
            }
	    hb_codepoint_t gid = இது->glyphIdArray[index];
	    எனில் (unlikely (!gid))
              out->del(codepoint);
	  }
	}
      }
    }

    வெறுமை collect_mapping (hb_set_t *unicodes, /* OUT */
			  hb_map_t *mapping /* OUT */) மாறிலி
    {
      // TODO(grieger): optimize similar to collect_unicodes
      // (ie. use add_range())
      unsigned மொத்தம் = இது->seg_மொத்தம்_2;
      எனில் (மொத்தம் && இது->start_மொத்தம்[மொத்தம் - 1] == 0xFFFFu)
	மொத்தம்--; /* Skip sentinel segment. */
      க்கு (unsigned i = 0; i < மொத்தம்; i++)
      {
	hb_codepoint_t start = இது->start_மொத்தம்[i];
	hb_codepoint_t end = இது->end_மொத்தம்[i];
	unsigned rangeOffset = இது->idRangeOffset[i];
	எனில் (rangeOffset == 0)
	{
	  க்கு (hb_codepoint_t codepoint = start; codepoint <= end; codepoint++)
	  {
	    hb_codepoint_t gid = (codepoint + இது->idDelta[i]) & 0xFFFFu;
	    எனில் (unlikely (!gid))
	      தொடர்;
	    unicodes->add (codepoint);
	    mapping->set (codepoint, gid);
	  }
	}
	இல்லையெனில்
	{
	  க்கு (hb_codepoint_t codepoint = start; codepoint <= end; codepoint++)
	  {
	    unsigned index = rangeOffset / 2 + (codepoint - இது->start_மொத்தம்[i]) + i - இது->seg_மொத்தம்_2;
	    எனில் (unlikely (index >= இது->glyphIdArrayLength))
	      நிறுத்து;
	    hb_codepoint_t gid = இது->glyphIdArray[index];
	    எனில் (unlikely (!gid))
	      தொடர்;
            gid = (gid + இது->idDelta[i]) & 0xFFFFu;
	    unicodes->add (codepoint);
	    mapping->set (codepoint, gid);
	  }
	}
      }
    }

    மாறிலி HBUINT16 *end_மொத்தம்;
    மாறிலி HBUINT16 *start_மொத்தம்;
    மாறிலி HBUINT16 *idDelta;
    மாறிலி HBUINT16 *idRangeOffset;
    மாறிலி HBUINT16 *glyphIdArray;
    unsigned முழு seg_மொத்தம்_2;
    unsigned முழு glyphIdArrayLength;
  };

  தருக்க get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph,
		  unsigned முழு subtable_data_size) மாறிலி
  {
    accelerator_t accel;
    accel.init (இது, subtable_data_size);
    திரும்பு accel.get_glyph_func (&accel, codepoint, glyph);
  }
  தருக்க get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) மாறிலி
  { திரும்பு பொய்; }

  வெறுமை collect_unicodes (hb_set_t *out, unsigned முழு subtable_data_size) மாறிலி
  {
    accelerator_t accel;
    accel.init (இது, subtable_data_size);
    accel.collect_unicodes (out);
  }
  வெறுமை collect_unicodes (hb_set_t *out) மாறிலி
  { collect_unicodes (out, length); }

  வெறுமை collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping, /* OUT */
			unsigned முழு subtable_data_size) மாறிலி
  {
    accelerator_t accel;
    accel.init (இது, subtable_data_size);
    accel.collect_mapping (unicodes, mapping);
  }
  வெறுமை collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping /* OUT */) மாறிலி
  { collect_mapping (unicodes, mapping, length); }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    எனில் (unlikely (!c->check_struct (இது)))
      return_trace (பொய்);
    hb_barrier ();

    return_trace (c->check_range (values, 2 + 4 * seg_மொத்தம்_X2));
  }

  பாதுகாக்கப்பட்ட:
  HBUINT16	format;		/* Format number is set to 4. */
  HBUINT16	length;		/* This is the length in bytes of the
				 * subtable. */
  HBUINT16	language;	/* Ignore. */
  HBUINT16	seg_மொத்தம்_X2;	/* 2 x segCount. */
  HBUINT16	தேடல்_Range;	/* 2 * (2**floor(log2(segCount))) */
  HBUINT16	entrySelector;	/* log2(searchRange/2) */
  HBUINT16	rangeShift;	/* 2 x segCount - searchRange */

  UnsizedArrayOf<HBUINT16>
		values;
#எனில் 0
  HBUINT16	end_மொத்தம்[seg_மொத்தம்_2];	/* End characterCode for each segment,
					 * last=0xFFFFu. */
  HBUINT16	reservedPad;		/* Set to 0. */
  HBUINT16	start_மொத்தம்[seg_மொத்தம்_2];	/* Start character code for each segment. */
  HBINT16		idDelta[seg_மொத்தம்_2];	/* Delta for all character codes in segment. */
  HBUINT16	idRangeOffset[seg_மொத்தம்_2];/* Offsets into glyphIdArray or 0 */
  UnsizedArrayOf<HBUINT16>
		glyphIdArray;	/* Glyph index array (arbitrary length) */
#endif

  பொது:
  DEFINE_SIZE_ARRAY (14, values);
};

கட்டமைப்பு CmapSubtableLongGroup
{
  friend கட்டமைப்பு CmapSubtableFormat12;
  friend கட்டமைப்பு CmapSubtableFormat13;
  template<typename U>
  friend கட்டமைப்பு CmapSubtableLongSegmented;
  friend கட்டமைப்பு cmap;

  முழு cmp (hb_codepoint_t codepoint) மாறிலி
  {
    எனில் (codepoint < startCharCode) திரும்பு -1;
    எனில் (codepoint > endCharCode)   திரும்பு +1;
    திரும்பு 0;
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது));
  }

  தனிப்பட்ட:
  HBUINT32		startCharCode;	/* First character code in this group. */
  HBUINT32		endCharCode;	/* Last character code in this group. */
  HBUINT32		glyphID;	/* Glyph index; interpretation depends on
					 * subtable format. */
  பொது:
  DEFINE_SIZE_STATIC (12);
};
DECLARE_NULL_NAMESPACE_BYTES (OT, CmapSubtableLongGroup);

template <typename UINT>
கட்டமைப்பு CmapSubtableTrimmed
{
  தருக்க get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) மாறிலி
  {
    /* Rely on our implicit array bound-checking. */
    hb_codepoint_t gid = glyphIdArray[codepoint - startCharCode];
    எனில் (unlikely (!gid))
      திரும்பு பொய்;
    *glyph = gid;
    திரும்பு உண்மை;
  }

  unsigned get_language () மாறிலி
  {
    திரும்பு language;
  }

  வெறுமை collect_unicodes (hb_set_t *out) மாறிலி
  {
    hb_codepoint_t start = startCharCode;
    unsigned முழு மொத்தம் = glyphIdArray.len;
    க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
      எனில் (glyphIdArray[i])
	out->add (start + i);
  }

  வெறுமை collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping /* OUT */) மாறிலி
  {
    hb_codepoint_t start_cp = startCharCode;
    unsigned மொத்தம் = glyphIdArray.len;
    க்கு (unsigned i = 0; i < மொத்தம்; i++)
      எனில் (glyphIdArray[i])
      {
	hb_codepoint_t unicode = start_cp + i;
	hb_codepoint_t glyphid = glyphIdArray[i];
	unicodes->add (unicode);
	mapping->set (unicode, glyphid);
      }
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) && glyphIdArray.sanitize (c));
  }

  பாதுகாக்கப்பட்ட:
  UINT		formatReserved;	/* Subtable format and (maybe) padding. */
  UINT		length;		/* Byte length of this subtable. */
  UINT		language;	/* Ignore. */
  UINT		startCharCode;	/* First character code covered. */
  ArrayOf<HBGlyphID16, UINT>
		glyphIdArray;	/* Array of glyph index values for character
				 * codes in the range. */
  பொது:
  DEFINE_SIZE_ARRAY (5 * அளவு (UINT), glyphIdArray);
};

கட்டமைப்பு CmapSubtableFormat6  : CmapSubtableTrimmed<HBUINT16> {};
கட்டமைப்பு CmapSubtableFormat10 : CmapSubtableTrimmed<HBUINT32> {};

template <typename T>
கட்டமைப்பு CmapSubtableLongSegmented
{
  friend கட்டமைப்பு cmap;

  தருக்க get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) மாறிலி
  {
    hb_codepoint_t gid = T::group_get_glyph (groups.bsearch (codepoint), codepoint);
    எனில் (unlikely (!gid))
      திரும்பு பொய்;
    *glyph = gid;
    திரும்பு உண்மை;
  }

  unsigned get_language () மாறிலி
  {
    திரும்பு language;
  }

  வெறுமை collect_unicodes (hb_set_t *out, unsigned முழு இலக்கம்_glyphs) மாறிலி
  {
    க்கு (unsigned முழு i = 0; i < இது->groups.len; i++)
    {
      hb_codepoint_t start = இது->groups[i].startCharCode;
      hb_codepoint_t end = hb_min ((hb_codepoint_t) இது->groups[i].endCharCode,
				   (hb_codepoint_t) HB_UNICODE_MAX);
      hb_codepoint_t gid = இது->groups[i].glyphID;
      எனில் (!gid)
      {
	/* Intention is: if (hb_is_same (T, CmapSubtableFormat13)) continue; */
	எனில் (! T::group_get_glyph (இது->groups[i], end)) தொடர்;
	start++;
	gid++;
      }
      எனில் (unlikely ((unsigned முழு) gid >= இலக்கம்_glyphs)) தொடர்;
      எனில் (unlikely ((unsigned முழு) (gid + end - start) >= இலக்கம்_glyphs))
	end = start + (hb_codepoint_t) இலக்கம்_glyphs - gid;

      out->add_range (start, hb_min (end, 0x10FFFFu));
    }
  }

  வெறுமை collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping, /* OUT */
			unsigned இலக்கம்_glyphs) மாறிலி
  {
    hb_codepoint_t last_end = 0;
    unsigned மொத்தம் = இது->groups.len;
    க்கு (unsigned i = 0; i < மொத்தம்; i++)
    {
      hb_codepoint_t start = இது->groups.arrayZ[i].startCharCode;
      hb_codepoint_t end = hb_min ((hb_codepoint_t) இது->groups.arrayZ[i].endCharCode,
				   (hb_codepoint_t) HB_UNICODE_MAX);
      எனில் (unlikely (start > end || start < last_end)) {
        // Range is not in order and is invalid, skip it.
        தொடர்;
      }
      last_end = end;


      hb_codepoint_t gid = இது->groups.arrayZ[i].glyphID;
      எனில் (!gid)
      {
        எனில் (T::format_எண் == 13) தொடர்;
	start++;
	gid++;
      }
      எனில் (unlikely ((unsigned முழு) gid >= இலக்கம்_glyphs)) தொடர்;
      எனில் (unlikely ((unsigned முழு) (gid + end - start) >= இலக்கம்_glyphs))
	end = start + (hb_codepoint_t) இலக்கம்_glyphs - gid;

      mapping->alloc (mapping->get_population () + end - start + 1);

      unicodes->add_range (start, end);
      க்கு (unsigned cp = start; cp <= end; cp++)
      {
	mapping->set (cp, gid);
        gid += T::increment;
      }
    }
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) && groups.sanitize (c));
  }

  பாதுகாக்கப்பட்ட:
  HBUINT16	format;		/* Subtable format; set to 12. */
  HBUINT16	reserved;	/* Reserved; set to 0. */
  HBUINT32	length;		/* Byte length of this subtable. */
  HBUINT32	language;	/* Ignore. */
  SortedArray32Of<CmapSubtableLongGroup>
		groups;		/* Groupings. */
  பொது:
  DEFINE_SIZE_ARRAY (16, groups);
};

கட்டமைப்பு CmapSubtableFormat12 : CmapSubtableLongSegmented<CmapSubtableFormat12>
{
  நிலையான constexpr முழு increment = 1;
  நிலையான constexpr முழு format_எண் = 12;

  நிலையான hb_codepoint_t group_get_glyph (மாறிலி CmapSubtableLongGroup &group,
					 hb_codepoint_t u)
  { திரும்பு likely (group.startCharCode <= group.endCharCode) ?
	   group.glyphID + (u - group.startCharCode) : 0; }


  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  வெறுமை serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    எனில் (!it) திரும்பு;
    unsigned table_initpos = c->length ();
    எனில் (unlikely (!c->extend_min (இது))) திரும்பு;

    hb_codepoint_t startCharCode = (hb_codepoint_t) -1, endCharCode = (hb_codepoint_t) -1;
    hb_codepoint_t glyphID = 0;

    க்கு (மாறிலி auto& _ : +it)
    {
      எனில் (startCharCode == (hb_codepoint_t) -1)
      {
	startCharCode = _.first;
	endCharCode = _.first;
	glyphID = _.second;
      }
      இல்லையெனில் எனில் (!_is_gid_consecutive (endCharCode, startCharCode, glyphID, _.first, _.second))
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
      இல்லையெனில்
	endCharCode = _.first;
    }

    CmapSubtableLongGroup record;
    record.startCharCode = startCharCode;
    record.endCharCode = endCharCode;
    record.glyphID = glyphID;
    c->copy<CmapSubtableLongGroup> (record);

    இது->format = 12;
    இது->reserved = 0;
    இது->length = c->length () - table_initpos;
    இது->groups.len = (இது->length - min_size) / CmapSubtableLongGroup::static_size;
  }

  நிலையான size_t get_sub_table_size (மாறிலி hb_sorted_vector_t<CmapSubtableLongGroup> &groups_data)
  { திரும்பு 16 + 12 * groups_data.length; }

  தனிப்பட்ட:
  நிலையான தருக்க _is_gid_consecutive (hb_codepoint_t endCharCode,
				   hb_codepoint_t startCharCode,
				   hb_codepoint_t glyphID,
				   hb_codepoint_t cp,
				   hb_codepoint_t new_gid)
  {
    திரும்பு (cp - 1 == endCharCode) &&
	new_gid == glyphID + (cp - startCharCode);
  }

};

கட்டமைப்பு CmapSubtableFormat13 : CmapSubtableLongSegmented<CmapSubtableFormat13>
{
  நிலையான constexpr முழு increment = 0;
  நிலையான constexpr முழு format_எண் = 13;

  நிலையான hb_codepoint_t group_get_glyph (மாறிலி CmapSubtableLongGroup &group,
					 hb_codepoint_t u HB_UNUSED)
  { திரும்பு group.glyphID; }
};

வகைவரை எண்ணிக்கை
{
  GLYPH_VARIANT_NOT_FOUND = 0,
  GLYPH_VARIANT_FOUND = 1,
  GLYPH_VARIANT_USE_DEFAULT = 2
} glyph_variant_t;

கட்டமைப்பு Unicode_மதிப்பு_Range
{
  முழு cmp (மாறிலி hb_codepoint_t &codepoint) மாறிலி
  {
    எனில் (codepoint < start_Unicode_மதிப்பு) திரும்பு -1;
    எனில் (codepoint > start_Unicode_மதிப்பு + additional_மொத்தம்) திரும்பு +1;
    திரும்பு 0;
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது));
  }

  HBUINT24	start_Unicode_மதிப்பு;	/* First value in this range. */
  HBUINT8	additional_மொத்தம்;	/* Number of additional values in this
					 * range. */
  பொது:
  DEFINE_SIZE_STATIC (4);
};

கட்டமைப்பு DefaultUVS : SortedArray32Of<Unicode_மதிப்பு_Range>
{
  வெறுமை collect_unicodes (hb_set_t *out) மாறிலி
  {
    unsigned முழு மொத்தம் = len;
    க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
    {
      hb_codepoint_t first = arrayZ[i].start_Unicode_மதிப்பு;
      hb_codepoint_t last = hb_min ((hb_codepoint_t) (first + arrayZ[i].additional_மொத்தம்),
				    (hb_codepoint_t) HB_UNICODE_MAX);
      out->add_range (first, last);
    }
  }

  DefaultUVS* copy (hb_serialize_context_t *c,
		    மாறிலி hb_set_t *unicodes) மாறிலி
  {
    auto *out = c->start_embed<DefaultUVS> ();
    auto snap = c->snapshot ();

    HBUINT32 len;
    len = 0;
    எனில் (unlikely (!c->copy<HBUINT32> (len))) திரும்பு nullptr;
    unsigned init_len = c->length ();

    எனில் (இது->len > unicodes->get_population () * hb_bit_storage ((unsigned) இது->len))
    {
      hb_codepoint_t start = HB_SET_மதிப்பு_INVALID;
      hb_codepoint_t end = HB_SET_மதிப்பு_INVALID;

      க்கு (auto u : *unicodes)
      {
        எனில் (!as_array ().bsearch (u))
	  தொடர்;
	எனில் (start == HB_SET_மதிப்பு_INVALID)
	{
	  start = u;
	  end = start - 1;
	}
	எனில் (end + 1 != u || end - start == 255)
        {
	  Unicode_மதிப்பு_Range rec;
	  rec.start_Unicode_மதிப்பு = start;
	  rec.additional_மொத்தம் = end - start;
	  c->copy<Unicode_மதிப்பு_Range> (rec);
	  start = u;
	}
	end = u;
      }
      எனில் (start != HB_SET_மதிப்பு_INVALID)
      {
	Unicode_மதிப்பு_Range rec;
	rec.start_Unicode_மதிப்பு = start;
	rec.additional_மொத்தம் = end - start;
	c->copy<Unicode_மதிப்பு_Range> (rec);
      }

    }
    இல்லையெனில்
    {
      hb_codepoint_t lastCode = HB_SET_மதிப்பு_INVALID;
      முழு மொத்தம் = -1;

      க்கு (மாறிலி Unicode_மதிப்பு_Range& _ : *இது)
      {
	hb_codepoint_t curEntry = (hb_codepoint_t) (_.start_Unicode_மதிப்பு - 1);
	hb_codepoint_t end = curEntry + _.additional_மொத்தம் + 2;

	க்கு (; unicodes->next (&curEntry) && curEntry < end;)
	{
	  மொத்தம் += 1;
	  எனில் (lastCode == HB_SET_மதிப்பு_INVALID)
	    lastCode = curEntry;
	  இல்லையெனில் எனில் (lastCode + மொத்தம் != curEntry || மொத்தம் > 255)
	  {
	    Unicode_மதிப்பு_Range rec;
	    rec.start_Unicode_மதிப்பு = lastCode;
	    rec.additional_மொத்தம் = மொத்தம் - 1;
	    c->copy<Unicode_மதிப்பு_Range> (rec);

	    lastCode = curEntry;
	    மொத்தம் = 0;
	  }
	}
      }

      எனில் (lastCode != HB_MAP_மதிப்பு_INVALID)
      {
	Unicode_மதிப்பு_Range rec;
	rec.start_Unicode_மதிப்பு = lastCode;
	rec.additional_மொத்தம் = மொத்தம்;
	c->copy<Unicode_மதிப்பு_Range> (rec);
      }
    }

    எனில் (c->length () - init_len == 0)
    {
      c->revert (snap);
      திரும்பு nullptr;
    }
    இல்லையெனில்
    {
      எனில் (unlikely (!c->check_assign (out->len,
                                      (c->length () - init_len) / Unicode_மதிப்பு_Range::static_size,
                                      HB_SERIALIZE_ERROR_INT_OVERFLOW))) திரும்பு nullptr;
      திரும்பு out;
    }
  }

  பொது:
  DEFINE_SIZE_ARRAY (4, *இது);
};

template <typename Types>
கட்டமைப்பு UVSMapping
{
  using GlyphID = typename Types::HBGlyphID;

  முழு cmp (மாறிலி hb_codepoint_t &codepoint) மாறிலி
  { திரும்பு unicode_மதிப்பு.cmp (codepoint); }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது));
  }

  HBUINT24	unicode_மதிப்பு;	/* Base Unicode value of the UVS */
  GlyphID
		glyphID;	/* Glyph ID of the UVS. */
  பொது:
  DEFINE_SIZE_STATIC (3 + GlyphID::static_size);
};

template <typename Types>
கட்டமைப்பு NonDefaultUVS : SortedArray32Of<UVSMapping<Types>>
{
  using Mapping = UVSMapping<Types>;

  வெறுமை collect_unicodes (hb_set_t *out) மாறிலி
  {
    க்கு (மாறிலி auto& a : இது->as_array ())
      out->add (a.unicode_மதிப்பு);
  }

  வெறுமை collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping /* OUT */) மாறிலி
  {
    க்கு (மாறிலி auto& a : இது->as_array ())
    {
      hb_codepoint_t unicode = a.unicode_மதிப்பு;
      hb_codepoint_t glyphid = a.glyphID;
      unicodes->add (unicode);
      mapping->set (unicode, glyphid);
    }
  }

  வெறுமை closure_glyphs (மாறிலி hb_set_t      *unicodes,
		       hb_set_t            *glyphset) மாறிலி
  {
    + இது->as_array ()
    | hb_filter (unicodes, &Mapping::unicode_மதிப்பு)
    | hb_map (&Mapping::glyphID)
    | hb_sink (glyphset)
    ;
  }

  NonDefaultUVS* copy (hb_serialize_context_t *c,
		       மாறிலி hb_set_t *unicodes,
		       மாறிலி hb_set_t *glyphs_requested,
		       மாறிலி hb_map_t *glyph_map) மாறிலி
  {
    auto *out = c->start_embed<NonDefaultUVS> ();
    auto it =
    + இது->as_array ()
    | hb_filter ([&] (மாறிலி Mapping& _)
		 {
		   திரும்பு unicodes->has (_.unicode_மதிப்பு) || glyphs_requested->has (_.glyphID);
		 })
    ;

    எனில் (!it) திரும்பு nullptr;

    HBUINT32 len;
    len = it.len ();
    எனில் (unlikely (!c->copy<HBUINT32> (len))) திரும்பு nullptr;

    க்கு (மாறிலி Mapping& _ : it)
    {
      Mapping mapping;
      mapping.unicode_மதிப்பு = _.unicode_மதிப்பு;
      mapping.glyphID = glyph_map->get (_.glyphID);
      c->copy<Mapping> (mapping);
    }

    திரும்பு out;
  }

  பொது:
  DEFINE_SIZE_ARRAY (4, *இது);
};

template <typename Types>
கட்டமைப்பு VariationSelectorRecord
{
  glyph_variant_t get_glyph (hb_codepoint_t codepoint,
			     hb_codepoint_t *glyph,
			     மாறிலி வெறுமை *base) மாறிலி
  {
    எனில் ((base+defaultUVS).bfind (codepoint))
      திரும்பு GLYPH_VARIANT_USE_DEFAULT;
    மாறிலி UVSMapping<Types> &nonDefault = (base+nonDefaultUVS).bsearch (codepoint);
    எனில் (nonDefault.glyphID)
    {
      *glyph = nonDefault.glyphID;
       திரும்பு GLYPH_VARIANT_FOUND;
    }
    திரும்பு GLYPH_VARIANT_NOT_FOUND;
  }

  VariationSelectorRecord(மாறிலி VariationSelectorRecord& other)
  {
    *இது = other;
  }

  வெறுமை operator= (மாறிலி VariationSelectorRecord& other)
  {
    varSelector = other.varSelector;
    HBUINT32 offset = other.defaultUVS;
    defaultUVS = offset;
    offset = other.nonDefaultUVS;
    nonDefaultUVS = offset;
  }

  வெறுமை collect_unicodes (hb_set_t *out, மாறிலி வெறுமை *base) மாறிலி
  {
    (base+defaultUVS).collect_unicodes (out);
    (base+nonDefaultUVS).collect_unicodes (out);
  }

  வெறுமை collect_mapping (மாறிலி வெறுமை *base,
			hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping /* OUT */) மாறிலி
  {
    (base+defaultUVS).collect_unicodes (unicodes);
    (base+nonDefaultUVS).collect_mapping (unicodes, mapping);
  }

  முழு cmp (மாறிலி hb_codepoint_t &variation_selector) மாறிலி
  { திரும்பு varSelector.cmp (variation_selector); }

  தருக்க sanitize (hb_sanitize_context_t *c, மாறிலி வெறுமை *base) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) &&
		  defaultUVS.sanitize (c, base) &&
		  nonDefaultUVS.sanitize (c, base));
  }

  hb_pair_t<unsigned, unsigned>
  copy (hb_serialize_context_t *c,
	மாறிலி hb_set_t *unicodes,
	மாறிலி hb_set_t *glyphs_requested,
	மாறிலி hb_map_t *glyph_map,
	மாறிலி வெறுமை *base) மாறிலி
  {
    auto snap = c->snapshot ();
    auto *out = c->embed<VariationSelectorRecord> (*இது);
    எனில் (unlikely (!out)) திரும்பு hb_pair (0, 0);

    out->defaultUVS = 0;
    out->nonDefaultUVS = 0;

    unsigned non_default_uvs_objidx = 0;
    எனில் (nonDefaultUVS != 0)
    {
      c->push ();
      எனில் (c->copy (base+nonDefaultUVS, unicodes, glyphs_requested, glyph_map))
	non_default_uvs_objidx = c->எடு_pack ();
      இல்லையெனில் c->எடு_discard ();
    }

    unsigned default_uvs_objidx = 0;
    எனில் (defaultUVS != 0)
    {
      c->push ();
      எனில் (c->copy (base+defaultUVS, unicodes))
	default_uvs_objidx = c->எடு_pack ();
      இல்லையெனில் c->எடு_discard ();
    }


    எனில் (!default_uvs_objidx && !non_default_uvs_objidx)
      c->revert (snap);

    திரும்பு hb_pair (default_uvs_objidx, non_default_uvs_objidx);
  }

  HBUINT24	varSelector;	/* Variation selector. */
  Offset32To<DefaultUVS>
		defaultUVS;	/* Offset to Default UVS Table.  May be 0. */
  Offset32To<NonDefaultUVS<Types>>
		nonDefaultUVS;	/* Offset to Non-Default UVS Table.  May be 0. */
  பொது:
  DEFINE_SIZE_STATIC (11);
};

template <typename Types>
கட்டமைப்பு CmapSubtableFormat14_15
{
  நிலையான constexpr unsigned format_மதிப்பு = Types::size == 2 ? 14 : 15;
  using Record = VariationSelectorRecord<Types>;
  using NonDefault = NonDefaultUVS<Types>;

  glyph_variant_t get_glyph_variant (hb_codepoint_t codepoint,
				     hb_codepoint_t variation_selector,
				     hb_codepoint_t *glyph) மாறிலி
  { திரும்பு record.bsearch (variation_selector).get_glyph (codepoint, glyph, இது); }

  வெறுமை collect_variation_selectors (hb_set_t *out) மாறிலி
  {
    க்கு (மாறிலி auto& a : record.as_array ())
      out->add (a.varSelector);
  }
  வெறுமை collect_variation_unicodes (hb_codepoint_t variation_selector,
				   hb_set_t *out) மாறிலி
  { record.bsearch (variation_selector).collect_unicodes (out, இது); }

  வெறுமை serialize (hb_serialize_context_t *c,
		  மாறிலி hb_set_t *unicodes,
		  மாறிலி hb_set_t *glyphs_requested,
		  மாறிலி hb_map_t *glyph_map,
		  மாறிலி வெறுமை *base)
  {
    auto snap = c->snapshot ();
    unsigned table_initpos = c->length ();
    மாறிலி எழுத்து* init_tail = c->tail;

    எனில் (unlikely (!c->extend_min (இது))) திரும்பு;
    இது->format = format_மதிப்பு;

    auto src_tbl = reinterpret_cast<மாறிலி CmapSubtableFormat14_15*> (base);

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
    க்கு (முழு i = src_tbl->record.len - 1; i >= 0; i--)
    {
      எனில் (!unicodes->has(src_tbl->record[i].varSelector))
        தொடர்;

      hb_pair_t<unsigned, unsigned> result = src_tbl->record[i].copy (c, unicodes, glyphs_requested, glyph_map, base);
      எனில் (result.first || result.second)
	obj_indices.push (result);
    }

    எனில் (c->length () - table_initpos == CmapSubtableFormat14_15::min_size)
    {
      c->revert (snap);
      திரும்பு;
    }

    எனில் (unlikely (!c->check_success (!obj_indices.in_error ())))
      திரும்பு;

    முழு tail_len = init_tail - c->tail;
    c->check_assign (இது->length, c->length () - table_initpos + tail_len,
                     HB_SERIALIZE_ERROR_INT_OVERFLOW);
    c->check_assign (இது->record.len,
		     (c->length () - table_initpos - CmapSubtableFormat14_15::min_size) /
		     Record::static_size,
                     HB_SERIALIZE_ERROR_INT_OVERFLOW);

    /* Correct the incorrect write order by reversing the order of the variation
       records array. */
    _reverse_variation_records ();

    /* Now that records are in the right order, we can set up the offsets. */
    _add_links_to_variation_records (c, obj_indices);
  }

  வெறுமை _reverse_variation_records ()
  {
    record.as_array ().reverse ();
  }

  வெறுமை _add_links_to_variation_records (hb_serialize_context_t *c,
					மாறிலி hb_vector_t<hb_pair_t<unsigned, unsigned>>& obj_indices)
  {
    க்கு (unsigned i = 0; i < obj_indices.length; i++)
    {
      /*
       * Since the record array has been reversed (see comments in copy())
       * but obj_indices has not been, the indices at obj_indices[i]
       * are for the variation record at record[j].
       */
      முழு j = obj_indices.length - 1 - i;
      c->add_link (record[j].defaultUVS, obj_indices[i].first);
      c->add_link (record[j].nonDefaultUVS, obj_indices[i].second);
    }
  }

  வெறுமை closure_glyphs (மாறிலி hb_set_t      *unicodes,
		       hb_set_t            *glyphset) மாறிலி
  {
    + hb_iter (record)
    | hb_filter (hb_bool, &Record::nonDefaultUVS)
    | hb_filter (unicodes, &Record::varSelector)
    | hb_map (&Record::nonDefaultUVS)
    | hb_map (hb_add (இது))
    | hb_apply ([=] (மாறிலி NonDefault& _) { _.closure_glyphs (unicodes, glyphset); })
    ;
  }

  வெறுமை collect_unicodes (hb_set_t *out) மாறிலி
  {
    க்கு (மாறிலி Record& _ : record)
      _.collect_unicodes (out, இது);
  }

  வெறுமை collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping /* OUT */) மாறிலி
  {
    க்கு (மாறிலி Record& _ : record)
      _.collect_mapping (இது, unicodes, mapping);
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) &&
		  record.sanitize (c, இது));
  }

  பாதுகாக்கப்பட்ட:
  HBUINT16	format;		/* Format number is set to 14 or 15. */
  HBUINT32	length;		/* Byte length of this subtable. */
  SortedArray32Of<Record>
		record;		/* Variation selector records; sorted
				 * in increasing order of `varSelector'. */
  பொது:
  DEFINE_SIZE_ARRAY (10, record);
};

கட்டமைப்பு CmapSubtable
{
  /* Note: We intentionally do NOT implement subtable formats 2 and 8. */

  தருக்க is_variation_selector () மாறிலி
  {
    தேர்வு (u.format.v) {
    நிலை 14:
#ifndef HB_NO_BEYOND_64K
    நிலை 15:
#endif
      திரும்பு உண்மை;
    இயல்பு:
      திரும்பு பொய்;
    }
  }

  glyph_variant_t get_glyph_variant (hb_codepoint_t codepoint,
				     hb_codepoint_t variation_selector,
				     hb_codepoint_t *glyph) மாறிலி
  {
    தேர்வு (u.format.v) {
    நிலை 14: hb_barrier (); திரும்பு u.format14.get_glyph_variant (codepoint, variation_selector, glyph);
#ifndef HB_NO_BEYOND_64K
    நிலை 15: hb_barrier (); திரும்பு u.format15.get_glyph_variant (codepoint, variation_selector, glyph);
#endif
    இயல்பு: திரும்பு GLYPH_VARIANT_NOT_FOUND;
    }
  }

  வெறுமை collect_variation_selectors (hb_set_t *out) மாறிலி
  {
    தேர்வு (u.format.v) {
    நிலை 14: hb_barrier (); u.format14.collect_variation_selectors (out); திரும்பு;
#ifndef HB_NO_BEYOND_64K
    நிலை 15: hb_barrier (); u.format15.collect_variation_selectors (out); திரும்பு;
#endif
    இயல்பு: திரும்பு;
    }
  }

  வெறுமை collect_variation_unicodes (hb_codepoint_t variation_selector,
				    hb_set_t *out) மாறிலி
  {
    தேர்வு (u.format.v) {
    நிலை 14: hb_barrier (); u.format14.collect_variation_unicodes (variation_selector, out); திரும்பு;
#ifndef HB_NO_BEYOND_64K
    நிலை 15: hb_barrier (); u.format15.collect_variation_unicodes (variation_selector, out); திரும்பு;
#endif
    இயல்பு: திரும்பு;
    }
  }

  வெறுமை closure_glyphs (மாறிலி hb_set_t *unicodes, hb_set_t *glyphset) மாறிலி
  {
    தேர்வு (u.format.v) {
    நிலை 14: hb_barrier (); u.format14.closure_glyphs (unicodes, glyphset); திரும்பு;
#ifndef HB_NO_BEYOND_64K
    நிலை 15: hb_barrier (); u.format15.closure_glyphs (unicodes, glyphset); திரும்பு;
#endif
    இயல்பு: திரும்பு;
    }
  }

  தருக்க get_glyph (hb_codepoint_t codepoint,
		  hb_codepoint_t *glyph,
		  unsigned முழு subtable_data_size = 0) மாறிலி
  {
    தேர்வு (u.format.v) {
    நிலை  0: hb_barrier (); திரும்பு u.format0 .get_glyph (codepoint, glyph);
    நிலை  4: hb_barrier (); திரும்பு subtable_data_size
				      ? u.format4.get_glyph (codepoint, glyph, subtable_data_size)
				      : பொய்;
    நிலை  6: hb_barrier (); திரும்பு u.format6 .get_glyph (codepoint, glyph);
    நிலை 10: hb_barrier (); திரும்பு u.format10.get_glyph (codepoint, glyph);
    நிலை 12: hb_barrier (); திரும்பு u.format12.get_glyph (codepoint, glyph);
    நிலை 13: hb_barrier (); திரும்பு u.format13.get_glyph (codepoint, glyph);
    நிலை 14:
    இயல்பு: திரும்பு பொய்;
    }
  }
  வெறுமை collect_unicodes (hb_set_t *out,
			 unsigned முழு இலக்கம்_glyphs = UINT_MAX,
			 unsigned முழு subtable_data_size = 0) மாறிலி
  {
    தேர்வு (u.format.v) {
    நிலை  0: hb_barrier (); u.format0 .collect_unicodes (out); திரும்பு;
    நிலை  4: hb_barrier (); எனில் (subtable_data_size)
			      u.format4.collect_unicodes (out, subtable_data_size);
			    இல்லையெனில்
			      u.format4.collect_unicodes (out);
			    திரும்பு;
    நிலை  6: hb_barrier (); u.format6 .collect_unicodes (out); திரும்பு;
    நிலை 10: hb_barrier (); u.format10.collect_unicodes (out); திரும்பு;
    நிலை 12: hb_barrier (); u.format12.collect_unicodes (out, இலக்கம்_glyphs); திரும்பு;
    நிலை 13: hb_barrier (); u.format13.collect_unicodes (out, இலக்கம்_glyphs); திரும்பு;
    நிலை 14:
    இயல்பு: திரும்பு;
    }
  }

  வெறுமை collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping, /* OUT */
			unsigned இலக்கம்_glyphs = UINT_MAX,
			unsigned முழு subtable_data_size = 0) மாறிலி
  {
    தேர்வு (u.format.v) {
    நிலை  0: hb_barrier (); u.format0 .collect_mapping (unicodes, mapping); திரும்பு;
    நிலை  4: hb_barrier (); எனில் (subtable_data_size)
			      u.format4.collect_mapping (unicodes, mapping, subtable_data_size);
			    இல்லையெனில்
			      u.format4.collect_mapping (unicodes, mapping);
			    திரும்பு;
    நிலை  6: hb_barrier (); u.format6 .collect_mapping (unicodes, mapping); திரும்பு;
    நிலை 10: hb_barrier (); u.format10.collect_mapping (unicodes, mapping); திரும்பு;
    நிலை 12: hb_barrier (); u.format12.collect_mapping (unicodes, mapping, இலக்கம்_glyphs); திரும்பு;
    நிலை 13: hb_barrier (); u.format13.collect_mapping (unicodes, mapping, இலக்கம்_glyphs); திரும்பு;
    நிலை 14:
    இயல்பு: திரும்பு;
    }
  }

  unsigned get_language () மாறிலி
  {
    தேர்வு (u.format.v) {
    நிலை  0: hb_barrier (); திரும்பு u.format0 .get_language ();
    நிலை  4: hb_barrier (); திரும்பு u.format4 .get_language ();
    நிலை  6: hb_barrier (); திரும்பு u.format6 .get_language ();
    நிலை 10: hb_barrier (); திரும்பு u.format10.get_language ();
    நிலை 12: hb_barrier (); திரும்பு u.format12.get_language ();
    நிலை 13: hb_barrier (); திரும்பு u.format13.get_language ();
    நிலை 14:
    இயல்பு: திரும்பு 0;
    }
  }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  வெறுமை serialize (hb_serialize_context_t *c,
		  Iterator it,
		  unsigned format,
		  மாறிலி hb_subset_plan_t *plan,
		  மாறிலி வெறுமை *base)
  {
    தேர்வு (format) {
    நிலை  4: hb_barrier (); திரும்பு u.format4.serialize (c, it);
    நிலை 12: hb_barrier (); திரும்பு u.format12.serialize (c, it);
    நிலை 14: hb_barrier (); திரும்பு u.format14.serialize (c, &plan->unicodes, &plan->glyphs_requested, plan->glyph_map, base);
#ifndef HB_NO_BEYOND_64K
    நிலை 15: hb_barrier (); திரும்பு u.format15.serialize (c, &plan->unicodes, &plan->glyphs_requested, plan->glyph_map, base);
#endif
    இயல்பு: திரும்பு;
    }
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    எனில் (!u.format.v.sanitize (c)) return_trace (பொய்);
    hb_barrier ();
    தேர்வு (u.format.v) {
    நிலை  0: hb_barrier (); return_trace (u.format0 .sanitize (c));
    நிலை  4: hb_barrier (); return_trace (u.format4 .sanitize (c));
    நிலை  6: hb_barrier (); return_trace (u.format6 .sanitize (c));
    நிலை 10: hb_barrier (); return_trace (u.format10.sanitize (c));
    நிலை 12: hb_barrier (); return_trace (u.format12.sanitize (c));
    நிலை 13: hb_barrier (); return_trace (u.format13.sanitize (c));
    நிலை 14: hb_barrier (); return_trace (u.format14.sanitize (c));
#ifndef HB_NO_BEYOND_64K
    நிலை 15: hb_barrier (); return_trace (u.format15.sanitize (c));
#endif
    இயல்பு:return_trace (உண்மை);
    }
  }

  பொது:
  ஒன்றிப்பு {
  கட்டமைப்பு { HBUINT16 v; }	format;		/* Format identifier */
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
  பொது:
  DEFINE_SIZE_UNION (2, format.v);
};


கட்டமைப்பு EncodingRecord
{
  முழு cmp (மாறிலி EncodingRecord &other) மாறிலி
  {
    முழு ret;
    ret = platformID.cmp (other.platformID);
    எனில் (ret) திரும்பு ret;
    எனில் (other.encodingID != 0xFFFF)
    {
      ret = encodingID.cmp (other.encodingID);
      எனில் (ret) திரும்பு ret;
    }
    திரும்பு 0;
  }

  தருக்க sanitize (hb_sanitize_context_t *c, மாறிலி வெறுமை *base) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) &&
		  subtable.sanitize (c, base));
  }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  EncodingRecord* copy (hb_serialize_context_t *c,
			Iterator it,
			unsigned format,
			மாறிலி வெறுமை *base,
			மாறிலி hb_subset_plan_t *plan,
			/* INOUT */ unsigned *objidx) மாறிலி
  {
    TRACE_SERIALIZE (இது);
    auto snap = c->snapshot ();
    auto *out = c->embed (இது);
    எனில் (unlikely (!out)) return_trace (nullptr);
    out->subtable = 0;

    எனில் (*objidx == 0)
    {
      CmapSubtable *cmapsubtable = c->push<CmapSubtable> ();
      unsigned origin_length = c->length ();
      cmapsubtable->serialize (c, it, format, plan, &(base+subtable));
      எனில் (c->length () > origin_length && !c->in_error()) *objidx = c->எடு_pack ();
      இல்லையெனில் c->எடு_discard ();
    }

    எனில் (*objidx == 0)
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
  பொது:
  DEFINE_SIZE_STATIC (8);
};

கட்டமைப்பு cmap;

கட்டமைப்பு SubtableUnicodesCache {

 தனிப்பட்ட:
  hb_blob_ptr_t<cmap> base_blob;
  மாறிலி எழுத்து* base;
  unsigned முழு table_length;
  hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> cached_unicodes;

 பொது:

  நிலையான SubtableUnicodesCache* create (hb_blob_ptr_t<cmap> source_table)
  {
    SubtableUnicodesCache* cache =
        (SubtableUnicodesCache*) hb_malloc (அளவு(SubtableUnicodesCache));

    எனில் (unlikely (!cache))
      திரும்பு nullptr;

    புதிய (cache) SubtableUnicodesCache (source_table);
    திரும்பு cache;
  }

  நிலையான வெறுமை destroy (வெறுமை* value) {
    எனில் (!value) திரும்பு;

    SubtableUnicodesCache* cache = (SubtableUnicodesCache*) value;
    cache->~SubtableUnicodesCache ();
    hb_free (cache);
  }

  SubtableUnicodesCache(மாறிலி வெறுமை* cmap_base,
			unsigned முழு table_length_ = 0)
      : base_blob(),
        base ((மாறிலி எழுத்து*) cmap_base),
        table_length (table_length_),
        cached_unicodes ()
  {}

  SubtableUnicodesCache(hb_blob_ptr_t<cmap> base_blob_)
      : base_blob(base_blob_),
        base ((மாறிலி எழுத்து *) base_blob.get()),
        table_length (base_blob.get_length ()),
        cached_unicodes ()
  {}

  ~SubtableUnicodesCache()
  {
    base_blob.destroy ();
  }

  தருக்க same_base(மாறிலி வெறுமை* other) மாறிலி
  {
    திரும்பு other == (மாறிலி வெறுமை*) base;
  }

  மாறிலி hb_set_t* set_for (மாறிலி EncodingRecord* record,
                           SubtableUnicodesCache& mutable_cache) மாறிலி
  {
    எனில் (cached_unicodes.has ((unsigned) ((மாறிலி எழுத்து *) record - base)))
      திரும்பு cached_unicodes.get ((unsigned) ((மாறிலி எழுத்து *) record - base));

    திரும்பு mutable_cache.set_for (record);
  }

  மாறிலி hb_set_t* set_for (மாறிலி EncodingRecord* record)
  {
    எனில் (!cached_unicodes.has ((unsigned) ((மாறிலி எழுத்து *) record - base)))
    {
      hb_set_t *s = hb_set_create ();
      எனில் (unlikely (s->in_error ()))
	திரும்பு hb_set_get_empty ();

      unsigned முழு subtable_data_size = record->subtable < table_length
					? table_length - (unsigned முழு) record->subtable
					: 0;
      (base+record->subtable).collect_unicodes (s, UINT_MAX, subtable_data_size);

      எனில் (unlikely (!cached_unicodes.set ((unsigned) ((மாறிலி எழுத்து *) record - base), hb::unique_ptr<hb_set_t> {s})))
        திரும்பு hb_set_get_empty ();

      திரும்பு s;
    }
    திரும்பு cached_unicodes.get ((unsigned) ((மாறிலி எழுத்து *) record - base));
  }

};

நிலையான உள்வரி uint16_t
hb_குறியீடு_pua_map (unsigned codepoint)
{
  எனில் (codepoint <= 0x00FFu)
  {
    /* For symbol-encoded OpenType fonts, we duplicate the
     * U+F000..F0FF range at U+0000..U+00FF.  That's what
     * Windows seems to do, and that's hinted about at:
     * https://docs.microsoft.com/en-us/typography/opentype/spec/recom
     * under "Non-Standard (Symbol) Fonts". */
    திரும்பு 0xF000u + codepoint;
  }
  திரும்பு 0;
}

கட்டமைப்பு cmap
{
  நிலையான constexpr hb_tag_t tableTag = HB_OT_TAG_cmap;


  நிலையான SubtableUnicodesCache* create_filled_cache(hb_blob_ptr_t<cmap> source_table) {
    மாறிலி cmap* cmap = source_table.get();
    auto it =
    + hb_iter (cmap->encodingRecord)
    | hb_filter ([&](மாறிலி EncodingRecord& _) {
      திரும்பு cmap::filter_encoding_records_for_subset (cmap, _);
    })
    ;

    SubtableUnicodesCache* cache = SubtableUnicodesCache::create(source_table);

    எனில் (unlikely (!cache))
      திரும்பு nullptr;

    க்கு (மாறிலி EncodingRecord& _ : it)
      cache->set_for(&_); // populate the cache for this encoding record.

    திரும்பு cache;
  }

  template<typename Iterator, typename EncodingRecIter,
	   hb_requires (hb_is_iterator (EncodingRecIter))>
  தருக்க serialize (hb_serialize_context_t *c,
		  Iterator it,
		  EncodingRecIter encodingrec_iter,
		  மாறிலி வெறுமை *base,
		  hb_subset_plan_t *plan,
	                  தருக்க drop_format_4 = பொய்,
			  unsigned முழு source_table_length = 0)
  {
    எனில் (unlikely (!c->extend_min ((*இது))))  திரும்பு பொய்;
    இது->version = 0;

    unsigned format4objidx = 0, format12objidx = 0, format14objidx = 0;
#ifndef HB_NO_BEYOND_64K
    unsigned format15objidx = 0;
#endif
    auto snap = c->snapshot ();

    SubtableUnicodesCache local_unicodes_cache (base, source_table_length);
    மாறிலி SubtableUnicodesCache* unicodes_cache = &local_unicodes_cache;

    எனில் (plan->accelerator &&
        plan->accelerator->cmap_cache &&
        plan->accelerator->cmap_cache->same_base (base))
      unicodes_cache = plan->accelerator->cmap_cache;

    க்கு (மாறிலி EncodingRecord& _ : encodingrec_iter)
    {
      எனில் (c->in_error ())
        திரும்பு பொய்;

      unsigned format = (base+_.subtable).u.format.v;
      எனில் (format != 4 && format != 12 && format != 14
#ifndef HB_NO_BEYOND_64K
	  && format != 15
#endif
	 ) தொடர்;

      மாறிலி hb_set_t* unicodes_set = unicodes_cache->set_for (&_, local_unicodes_cache);

      எனில் (!drop_format_4 && format == 4)
      {
        c->copy (_, + it | hb_filter (*unicodes_set, hb_first), 4u, base, plan, &format4objidx);
        எனில் (c->in_error () && c->only_overflow ())
        {
          // cmap4 overflowed, reset and retry serialization without format 4 subtables.
          c->revert (snap);
          திரும்பு serialize (c, it,
                            encodingrec_iter,
                            base,
                            plan,
                            உண்மை,
			    source_table_length);
        }
      }

      இல்லையெனில் எனில் (format == 12)
      {
        எனில் (_can_drop (_,
                       *unicodes_set,
                       base,
                       *unicodes_cache,
                       local_unicodes_cache,
                       + it | hb_map (hb_first), encodingrec_iter))
          தொடர்;
        c->copy (_, + it | hb_filter (*unicodes_set, hb_first), 12u, base, plan, &format12objidx);
      }
      இல்லையெனில் எனில் (format == 14) c->copy (_, it, 14u, base, plan, &format14objidx);
#ifndef HB_NO_BEYOND_64K
      இல்லையெனில் எனில் (format == 15) c->copy (_, it, 15u, base, plan, &format15objidx);
#endif
    }
        unsigned length = c->length ();
        unsigned available = length > cmap::min_size ? length - cmap::min_size : 0;
        c->check_assign(இது->encodingRecord.len,
                        available / EncodingRecord::static_size,
                        HB_SERIALIZE_ERROR_INT_OVERFLOW);

    // Fail if format 4 was dropped and there is no cmap12.
    திரும்பு !drop_format_4 || format12objidx;
  }

  template<typename Iterator, typename EncodingRecordIterator,
      hb_requires (hb_is_iterator (Iterator)),
      hb_requires (hb_is_iterator (EncodingRecordIterator))>
  தருக்க _can_drop (மாறிலி EncodingRecord& cmap12,
                  மாறிலி hb_set_t& cmap12_unicodes,
                  மாறிலி வெறுமை* base,
                  மாறிலி SubtableUnicodesCache& unicodes_cache,
                  SubtableUnicodesCache& local_unicodes_cache,
                  Iterator subset_unicodes,
                  EncodingRecordIterator encoding_records)
  {
    க்கு (auto cp : + subset_unicodes | hb_filter (cmap12_unicodes))
    {
      எனில் (cp >= 0x10000) திரும்பு பொய்;
    }

    unsigned target_platform;
    unsigned target_encoding;
    unsigned target_language = (base+cmap12.subtable).get_language ();

    எனில் (cmap12.platformID == 0 && cmap12.encodingID == 4)
    {
      target_platform = 0;
      target_encoding = 3;
    } இல்லையெனில் எனில் (cmap12.platformID == 3 && cmap12.encodingID == 10) {
      target_platform = 3;
      target_encoding = 1;
    } இல்லையெனில் {
      திரும்பு பொய்;
    }

    க்கு (மாறிலி auto& _ : encoding_records)
    {
      எனில் (_.platformID != target_platform
          || _.encodingID != target_encoding
          || (base+_.subtable).get_language() != target_language)
        தொடர்;

      மாறிலி hb_set_t* sibling_unicodes = unicodes_cache.set_for (&_, local_unicodes_cache);

      auto cmap12 = + subset_unicodes | hb_filter (cmap12_unicodes);
      auto sibling = + subset_unicodes | hb_filter (*sibling_unicodes);
      க்கு (; cmap12 && sibling; cmap12++, sibling++)
      {
        unsigned a = *cmap12;
        unsigned b = *sibling;
        எனில் (a != b) திரும்பு பொய்;
      }

      திரும்பு !cmap12 && !sibling;
    }

    திரும்பு பொய்;
  }

  வெறுமை closure_glyphs (மாறிலி hb_set_t      *unicodes,
		       hb_set_t            *glyphset) மாறிலி
  {
    + hb_iter (encodingRecord)
    | hb_map (&EncodingRecord::subtable)
    | hb_map (hb_add (இது))
    | hb_filter (&CmapSubtable::is_variation_selector)
    | hb_apply ([=] (மாறிலி CmapSubtable& _) { _.closure_glyphs (unicodes, glyphset); })
    ;
  }

  தருக்க subset (hb_subset_context_t *c) மாறிலி
  { திரும்பு subset (c, பொய்); }

  தருக்க subset (hb_subset_context_t *c, தருக்க is_dmap) மாறிலி
  {
    TRACE_SUBSET (இது);

    cmap *cmap_பகாஎண் = c->serializer->start_embed<cmap> ();

    auto encodingrec_iter =
    + hb_iter (encodingRecord)
    | hb_filter ([&](மாறிலி EncodingRecord& _) {
      திரும்பு cmap::filter_encoding_records_for_subset (இது, _);
    })
    ;

    எனில் (unlikely (!encodingrec_iter.len ())) return_trace (பொய்);

    மாறிலி EncodingRecord *unicode_bmp= nullptr, *unicode_ucs4 = nullptr, *ms_bmp = nullptr, *ms_ucs4 = nullptr;
    தருக்க has_format12 = பொய்;

    க்கு (மாறிலி EncodingRecord& _ : encodingrec_iter)
    {
      unsigned format = (இது + _.subtable).u.format.v;
      எனில் (format == 12) has_format12 = உண்மை;

      மாறிலி EncodingRecord *table = std::addressof (_);
      எனில்      (_.platformID == 0 && _.encodingID ==  3) unicode_bmp = table;
      இல்லையெனில் எனில் (_.platformID == 0 && _.encodingID ==  4) unicode_ucs4 = table;
      இல்லையெனில் எனில் (_.platformID == 3 && _.encodingID ==  1) ms_bmp = table;
      இல்லையெனில் எனில் (_.platformID == 3 && _.encodingID == 10) ms_ucs4 = table;
    }

    எனில் (unlikely (!is_dmap && !has_format12 && !unicode_bmp && !ms_bmp)) return_trace (பொய்);
    எனில் (unlikely (!is_dmap && has_format12 && (!unicode_ucs4 && !ms_ucs4))) return_trace (பொய்);

    auto it =
    + c->plan->unicode_to_new_gid_list.iter ()
    | hb_filter ([&] (மாறிலி hb_codepoint_pair_t _)
		 { திரும்பு (_.second != HB_MAP_மதிப்பு_INVALID); })
    ;

    return_trace (cmap_பகாஎண்->serialize (c->serializer,
                                         it,
                                         encodingrec_iter,
                                         இது,
                                         c->plan,
					 பொய்,
					 c->source_blob->length));
  }

  மாறிலி CmapSubtable *find_best_subtable (தருக்க *குறியீடு = nullptr,
					  தருக்க *mac = nullptr,
					  தருக்க *macroman = nullptr) மாறிலி
  {
    எனில் (குறியீடு) *குறியீடு = பொய்;
    எனில் (mac) *mac = பொய்;
    எனில் (macroman) *macroman = பொய்;

    மாறிலி CmapSubtable *subtable;

    /* Symbol subtable.
     * Prefer symbol if available.
     * https://github.com/harfbuzz/harfbuzz/issues/1918 */
    எனில் ((subtable = இது->find_subtable (3, 0)))
    {
      எனில் (குறியீடு) *குறியீடு = உண்மை;
      திரும்பு subtable;
    }

    /* 32-bit subtables. */
    எனில் ((subtable = இது->find_subtable (3, 10))) திரும்பு subtable;
    எனில் ((subtable = இது->find_subtable (0, 6))) திரும்பு subtable;
    எனில் ((subtable = இது->find_subtable (0, 4))) திரும்பு subtable;

    /* 16-bit subtables. */
    எனில் ((subtable = இது->find_subtable (3, 1))) திரும்பு subtable;
    எனில் ((subtable = இது->find_subtable (0, 3))) திரும்பு subtable;
    எனில் ((subtable = இது->find_subtable (0, 2))) திரும்பு subtable;
    எனில் ((subtable = இது->find_subtable (0, 1))) திரும்பு subtable;
    எனில் ((subtable = இது->find_subtable (0, 0))) திரும்பு subtable;

    /* MacRoman subtable. */
    எனில் ((subtable = இது->find_subtable (1, 0)))
    {
      எனில் (mac) *mac = உண்மை;
      எனில் (macroman) *macroman = உண்மை;
      திரும்பு subtable;
    }
    /* Any other Mac subtable; we just map ASCII for these. */
    எனில் ((subtable = இது->find_subtable (1, 0xFFFF)))
    {
      எனில் (mac) *mac = உண்மை;
      திரும்பு subtable;
    }

    /* Meh. */
    திரும்பு &Null (CmapSubtable);
  }

  கட்டமைப்பு accelerator_t
  {
    using cache_t = hb_cache_t<21, 19>;
    static_assert (அளவு (cache_t) == 1024, "");

    accelerator_t (hb_face_t *face, hb_tag_t table_tag = cmap::tableTag)
    {
      இது->table = hb_sanitize_context_t ().reference_table<cmap> (face, table_tag);
      எனில் (!இது->table.get_length ())
	திரும்பு;

      தருக்க குறியீடு, mac, macroman;
      இது->subtable = table->find_best_subtable (&குறியீடு, &mac, &macroman);
      இது->subtable_uvs = &Null (CmapSubtable);
      {
	மாறிலி CmapSubtable *st = table->find_variation_selector_subtable ();
	எனில் (st)
	  subtable_uvs = st;
      }

#ifndef HB_NO_OT_FONT_CMAP_CACHE
      cache = (cache_t *) hb_malloc (அளவு (cache_t));
      எனில் (cache)
	புதிய (cache) cache_t ();
      இல்லையெனில்
        திரும்பு; // Such that get_glyph_funcZ remains null.
#endif

      இது->get_glyph_data = subtable;
#ifndef HB_NO_CMAP_LEGACY_SUBTABLES
      தருக்க is_format4 = subtable->u.format.v == 4;
      auto set_format4_getter = [இது] (தருக்க (*func) (மாறிலி வெறுமை *,
						      hb_codepoint_t,
						      hb_codepoint_t *))
      {
	இது->format4_accel.init (&இது->subtable->u.format4,
				  get_subtable_data_size (இது->subtable));
	இது->get_glyph_data = &இது->format4_accel;
	இது->get_glyph_funcZ = func;
      };
      எனில் (unlikely (குறியீடு))
      {
	தேர்வு ((unsigned) face->table.OS2->get_font_page ()) {
	நிலை OS2::font_page_t::FONT_PAGE_NONE:
	  எனில் (is_format4)
	    set_format4_getter (get_glyph_from_குறியீடு<CmapSubtableFormat4::accelerator_t, hb_குறியீடு_pua_map>);
	  இல்லையெனில்
	    இது->get_glyph_funcZ = get_glyph_from_குறியீடு<CmapSubtable, hb_குறியீடு_pua_map>;
	  நிறுத்து;
#ifndef HB_NO_OT_SHAPER_ARABIC_FALLBACK
	நிலை OS2::font_page_t::FONT_PAGE_SIMP_ARABIC:
	  எனில் (is_format4)
	    set_format4_getter (get_glyph_from_குறியீடு<CmapSubtableFormat4::accelerator_t, _hb_arabic_pua_simp_map>);
	  இல்லையெனில்
	    இது->get_glyph_funcZ = get_glyph_from_குறியீடு<CmapSubtable, _hb_arabic_pua_simp_map>;
	  நிறுத்து;
	நிலை OS2::font_page_t::FONT_PAGE_TRAD_ARABIC:
	  எனில் (is_format4)
	    set_format4_getter (get_glyph_from_குறியீடு<CmapSubtableFormat4::accelerator_t, _hb_arabic_pua_trad_map>);
	  இல்லையெனில்
	    இது->get_glyph_funcZ = get_glyph_from_குறியீடு<CmapSubtable, _hb_arabic_pua_trad_map>;
	  நிறுத்து;
#endif
	இயல்பு:
	  எனில் (is_format4)
	    set_format4_getter (get_glyph_from<CmapSubtableFormat4::accelerator_t>);
	  இல்லையெனில்
	    இது->get_glyph_funcZ = get_glyph_from<CmapSubtable>;
	  நிறுத்து;
	}
      }
      இல்லையெனில் எனில் (unlikely (macroman))
      {
	எனில் (is_format4)
	  set_format4_getter (get_glyph_from_macroman<CmapSubtableFormat4::accelerator_t>);
	இல்லையெனில்
	  இது->get_glyph_funcZ = get_glyph_from_macroman<CmapSubtable>;
      }
      இல்லையெனில் எனில் (unlikely (mac))
      {
	எனில் (is_format4)
	  set_format4_getter (get_glyph_from_ascii<CmapSubtableFormat4::accelerator_t>);
	இல்லையெனில்
	  இது->get_glyph_funcZ = get_glyph_from_ascii<CmapSubtable>;
      }
      இல்லையெனில்
#endif
      {
	தேர்வு (subtable->u.format.v) {
	  /* Accelerate format 4 and format 12. */
	  இயல்பு:
	    இது->get_glyph_funcZ = get_glyph_from<CmapSubtable>;
	    நிறுத்து;
	  நிலை 12:
	    இது->get_glyph_funcZ = get_glyph_from<CmapSubtableFormat12>;
	    நிறுத்து;
	  நிலை  4:
	  {
	    hb_barrier ();
	    இது->format4_accel.init (&subtable->u.format4,
				      get_subtable_data_size (subtable));
	    இது->get_glyph_data = &இது->format4_accel;
	    இது->get_glyph_funcZ = இது->format4_accel.get_glyph_func;
	    நிறுத்து;
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

    உள்வரி தருக்க _cached_get (hb_codepoint_t unicode,
			     hb_codepoint_t *glyph) மாறிலி
    {
#ifndef HB_NO_OT_FONT_CMAP_CACHE
      // cache is always non-null if we have a get_glyph_funcZ
      unsigned v;
      எனில் (cache->get (unicode, &v))
      {
        *glyph = v;
	திரும்பு உண்மை;
      }
#endif
      தருக்க ret  = இது->get_glyph_funcZ (இது->get_glyph_data, unicode, glyph);

#ifndef HB_NO_OT_FONT_CMAP_CACHE
      எனில் (ret)
        cache->set (unicode, *glyph);
#endif

      திரும்பு ret;
    }

    தருக்க get_nominal_glyph (hb_codepoint_t  unicode,
			    hb_codepoint_t *glyph) மாறிலி
    {
      எனில் (unlikely (!இது->get_glyph_funcZ)) திரும்பு பொய்;
      திரும்பு _cached_get (unicode, glyph);
    }

    unsigned முழு get_nominal_glyphs (unsigned முழு மொத்தம்,
				     மாறிலி hb_codepoint_t *first_unicode,
				     unsigned முழு unicode_stride,
				     hb_codepoint_t *first_glyph,
				     unsigned முழு glyph_stride) மாறிலி
    {
      எனில் (unlikely (!இது->get_glyph_funcZ)) திரும்பு 0;

      unsigned முழு done;
      க்கு (done = 0;
	   done < மொத்தம் && _cached_get (*first_unicode, first_glyph);
	   done++)
      {
	first_unicode = &StructAtOffsetUnaligned<hb_codepoint_t> (first_unicode, unicode_stride);
	first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      }
      திரும்பு done;
    }

    தருக்க get_variation_glyph (hb_codepoint_t  unicode,
			      hb_codepoint_t  variation_selector,
			      hb_codepoint_t *glyph) மாறிலி
    {
      தேர்வு (get_glyph_variant (unicode, variation_selector, glyph))
      {
	நிலை GLYPH_VARIANT_NOT_FOUND:	திரும்பு பொய்;
	நிலை GLYPH_VARIANT_FOUND:	திரும்பு உண்மை;
	நிலை GLYPH_VARIANT_USE_DEFAULT:	நிறுத்து;
      }

      திரும்பு get_nominal_glyph (unicode, glyph);
    }

    glyph_variant_t get_glyph_variant (hb_codepoint_t  unicode,
				       hb_codepoint_t  variation_selector,
				       hb_codepoint_t *glyph) மாறிலி
    {
      திரும்பு இது->subtable_uvs->get_glyph_variant (unicode,
						    variation_selector,
						    glyph);
    }

    தருக்க has_data () மாறிலி { திரும்பு table.get_length (); }

    வெறுமை collect_unicodes (hb_set_t *out, unsigned முழு இலக்கம்_glyphs) மாறிலி
    { subtable->collect_unicodes (out, இலக்கம்_glyphs, get_subtable_data_size (subtable)); }
    வெறுமை collect_mapping (hb_set_t *unicodes, hb_map_t *mapping,
			  unsigned இலக்கம்_glyphs = UINT_MAX) மாறிலி
    { subtable->collect_mapping (unicodes, mapping, இலக்கம்_glyphs, get_subtable_data_size (subtable)); }
    வெறுமை collect_variation_selectors (hb_set_t *out) மாறிலி
    { subtable_uvs->collect_variation_selectors (out); }
    வெறுமை collect_variation_unicodes (hb_codepoint_t variation_selector,
				     hb_set_t *out) மாறிலி
    { subtable_uvs->collect_variation_unicodes (variation_selector, out); }

    பாதுகாக்கப்பட்ட:
    வகைவரை தருக்க (*hb_cmap_get_glyph_func_t) (மாறிலி வெறுமை *obj,
					      hb_codepoint_t codepoint,
					      hb_codepoint_t *glyph);
    வகைவரை uint16_t (*hb_pua_remap_func_t) (unsigned);

    template <typename Type>
    HB_INTERNAL நிலையான தருக்க get_glyph_from (மாறிலி வெறுமை *obj,
					    hb_codepoint_t codepoint,
					    hb_codepoint_t *glyph)
    {
      மாறிலி Type *typed_obj = (மாறிலி Type *) obj;
      திரும்பு typed_obj->get_glyph (codepoint, glyph);
    }

    template <typename Type, hb_pua_remap_func_t remap>
    HB_INTERNAL நிலையான தருக்க get_glyph_from_குறியீடு (மாறிலி வெறுமை *obj,
						   hb_codepoint_t codepoint,
						   hb_codepoint_t *glyph)
    {
      மாறிலி Type *typed_obj = (மாறிலி Type *) obj;
      எனில் (likely (typed_obj->get_glyph (codepoint, glyph)))
	திரும்பு உண்மை;

      எனில் (hb_codepoint_t c = remap (codepoint))
	திரும்பு typed_obj->get_glyph (c, glyph);

      திரும்பு பொய்;
    }

    template <typename Type>
    HB_INTERNAL நிலையான தருக்க get_glyph_from_ascii (மாறிலி வெறுமை *obj,
						  hb_codepoint_t codepoint,
						  hb_codepoint_t *glyph)
    {
      மாறிலி Type *typed_obj = (மாறிலி Type *) obj;
      திரும்பு codepoint < 0x80 && typed_obj->get_glyph (codepoint, glyph);
    }

    template <typename Type>
    HB_INTERNAL நிலையான தருக்க get_glyph_from_macroman (மாறிலி வெறுமை *obj,
						     hb_codepoint_t codepoint,
						     hb_codepoint_t *glyph)
    {
      எனில் (get_glyph_from_ascii<Type> (obj, codepoint, glyph))
	திரும்பு உண்மை;

      மாறிலி Type *typed_obj = (மாறிலி Type *) obj;
      unsigned c = unicode_to_macroman (codepoint);
      திரும்பு c && typed_obj->get_glyph (c, glyph);
    }

    unsigned முழு get_subtable_data_size (மாறிலி CmapSubtable *subtable) மாறிலி
    {
      unsigned முழு table_length = இது->table.get_length ();
      uintptr_t table_start = (uintptr_t) (மாறிலி வெறுமை *) இது->table.get ();
      uintptr_t subtable_addr = (uintptr_t) (மாறிலி வெறுமை *) subtable;
      எனில் (unlikely (subtable_addr < table_start))
	திரும்பு 0;

      uintptr_t subtable_offset = subtable_addr - table_start;
      எனில் (unlikely (subtable_offset >= table_length))
	திரும்பு 0;

      திரும்பு table_length - (unsigned முழு) subtable_offset;
    }

    தனிப்பட்ட:
    hb_nonnull_ptr_t<மாறிலி CmapSubtable> subtable;
    hb_nonnull_ptr_t<மாறிலி CmapSubtable> subtable_uvs;

    hb_cmap_get_glyph_func_t get_glyph_funcZ = nullptr;
    மாறிலி வெறுமை *get_glyph_data = nullptr;

    CmapSubtableFormat4::accelerator_t format4_accel;

#ifndef HB_NO_OT_FONT_CMAP_CACHE
    cache_t *cache = nullptr;
#endif

    பொது:
    hb_blob_ptr_t<cmap> table;
  };

  பாதுகாக்கப்பட்ட:

  மாறிலி CmapSubtable *find_subtable (unsigned முழு platform_id,
				     unsigned முழு encoding_id) மாறிலி
  {
    EncodingRecord key;
    key.platformID = platform_id;
    key.encodingID = encoding_id;

    மாறிலி EncodingRecord &result = encodingRecord.bsearch (key);
    எனில் (!result.subtable)
      திரும்பு nullptr;

    திரும்பு &(இது+result.subtable);
  }

  மாறிலி CmapSubtable *find_variation_selector_subtable () மாறிலி
  {
    மாறிலி CmapSubtable *format14 = nullptr;
    க்கு (மாறிலி EncodingRecord& record : encodingRecord)
    {
      எனில் (record.platformID != 0 || record.encodingID != 5)
	தொடர்;

      மாறிலி CmapSubtable *subtable = &(இது+record.subtable);
#ifndef HB_NO_BEYOND_64K
      எனில் (subtable->u.format.v == 15)
	திரும்பு subtable;
#endif
      எனில் (subtable->u.format.v == 14)
	format14 = subtable;
    }
    திரும்பு format14;
  }

  பொது:

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) &&
		  hb_barrier () &&
		  likely (version == 0) &&
		  encodingRecord.sanitize (c, இது));
  }

 தனிப்பட்ட:

  நிலையான தருக்க filter_encoding_records_for_subset(மாறிலி cmap* cmap,
                                                 மாறிலி EncodingRecord& _)
  {
    திரும்பு
        (_.platformID == 0 && _.encodingID == 3) ||
        (_.platformID == 0 && _.encodingID == 4) ||
        (_.platformID == 3 && _.encodingID == 1) ||
        (_.platformID == 3 && _.encodingID == 10) ||
        (cmap + _.subtable).is_variation_selector ();
  }

  பாதுகாக்கப்பட்ட:
  HBUINT16	version;	/* Table version number (0). */
  SortedArray16Of<EncodingRecord>
		encodingRecord;	/* Encoding tables. */
  பொது:
  DEFINE_SIZE_ARRAY (4, encodingRecord);
};

கட்டமைப்பு DMAP : cmap
{
  நிலையான constexpr hb_tag_t tableTag = HB_OT_TAG_DMAP;

  தருக்க subset (hb_subset_context_t *c) மாறிலி
  { திரும்பு cmap::subset (c, உண்மை); }
};

கட்டமைப்பு cmap_accelerator_t
{
  cmap_accelerator_t (hb_face_t *face) :
    dmap (face, HB_OT_TAG_DMAP),
    base (face)
  {}

  தருக்க get_nominal_glyph (hb_codepoint_t  unicode,
			  hb_codepoint_t *glyph) மாறிலி
  {
    திரும்பு dmap.get_nominal_glyph (unicode, glyph) ||
	   base.get_nominal_glyph (unicode, glyph);
  }

  unsigned முழு get_nominal_glyphs (unsigned முழு மொத்தம்,
				   மாறிலி hb_codepoint_t *first_unicode,
				   unsigned முழு unicode_stride,
				   hb_codepoint_t *first_glyph,
				   unsigned முழு glyph_stride) மாறிலி
  {
    எனில் (likely (!dmap.has_data ()))
      திரும்பு base.get_nominal_glyphs (மொத்தம்,
				      first_unicode, unicode_stride,
				      first_glyph, glyph_stride);

    unsigned முழு done;
    க்கு (done = 0;
	 done < மொத்தம் && get_nominal_glyph (*first_unicode, first_glyph);
	 done++)
    {
      first_unicode = &StructAtOffsetUnaligned<hb_codepoint_t> (first_unicode, unicode_stride);
      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
    }
    திரும்பு done;
  }

  தருக்க get_variation_glyph (hb_codepoint_t  unicode,
			    hb_codepoint_t  variation_selector,
			    hb_codepoint_t *glyph) மாறிலி
  {
    glyph_variant_t result = dmap.get_glyph_variant (unicode,
						     variation_selector,
						     glyph);
    எனில் (result == GLYPH_VARIANT_NOT_FOUND)
      result = base.get_glyph_variant (unicode, variation_selector, glyph);

    தேர்வு (result)
    {
      நிலை GLYPH_VARIANT_NOT_FOUND: திரும்பு பொய்;
      நிலை GLYPH_VARIANT_FOUND: திரும்பு உண்மை;
      நிலை GLYPH_VARIANT_USE_DEFAULT: நிறுத்து;
    }

    திரும்பு get_nominal_glyph (unicode, glyph);
  }

  வெறுமை collect_unicodes (hb_set_t *out, unsigned முழு இலக்கம்_glyphs) மாறிலி
  {
    base.collect_unicodes (out, இலக்கம்_glyphs);
    dmap.collect_unicodes (out, இலக்கம்_glyphs);
  }

  வெறுமை collect_mapping (hb_set_t *unicodes, hb_map_t *mapping,
			unsigned இலக்கம்_glyphs = UINT_MAX) மாறிலி
  {
    base.collect_mapping (unicodes, mapping, இலக்கம்_glyphs);
    dmap.collect_mapping (unicodes, mapping, இலக்கம்_glyphs);
  }

  வெறுமை collect_variation_selectors (hb_set_t *out) மாறிலி
  {
    base.collect_variation_selectors (out);
    dmap.collect_variation_selectors (out);
  }

  வெறுமை collect_variation_unicodes (hb_codepoint_t variation_selector,
				   hb_set_t *out) மாறிலி
  {
    base.collect_variation_unicodes (variation_selector, out);
    dmap.collect_variation_unicodes (variation_selector, out);
  }

  வெறுமை closure_glyphs (மாறிலி hb_set_t *unicodes,
		       hb_set_t       *glyphset) மாறிலி
  {
    எனில் (likely (!dmap.has_data ()))
    {
      base.table->closure_glyphs (unicodes, glyphset);
      திரும்பு;
    }

    hb_set_t variation_selectors;
    collect_variation_selectors (&variation_selectors);
    க்கு (hb_codepoint_t variation_selector : variation_selectors)
    {
      எனில் (!unicodes->has (variation_selector))
	தொடர்;

      hb_set_t variation_unicodes;
      collect_variation_unicodes (variation_selector, &variation_unicodes);
      க்கு (hb_codepoint_t unicode : variation_unicodes)
      {
	எனில் (!unicodes->has (unicode))
	  தொடர்;

	hb_codepoint_t glyph;
	எனில் (get_variation_glyph (unicode, variation_selector, &glyph))
	  glyphset->add (glyph);
      }
    }
  }

  cmap::accelerator_t dmap;
  cmap::accelerator_t base;
};

} /* namespace OT */


#endif /* HB_OT_CMAP_TABLE_HH */
