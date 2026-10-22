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
  स्थिर अचर संरचना unicode_to_macroman_t
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
  स्वतः *c = hb_bsearch (u, mapping, ARRAY_LENGTH (mapping), परिमाण (mapping[0]),
			_hb_cmp_operator<uint16_t, uint16_t>);
  फर्काऊ c ? c->macroman : 0;
}

संरचना CmapSubtableFormat0
{
  तार्किक get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) अचर
  {
    hb_codepoint_t gid = codepoint < 256 ? glyphIdArray[codepoint] : 0;
    यदि (unlikely (!gid))
      फर्काऊ असत्य;
    *glyph = gid;
    फर्काऊ सत्य;
  }

  अचिन्हित get_language () अचर
  {
    फर्काऊ language;
  }

  शून्य collect_unicodes (hb_set_t *out) अचर
  {
    लागि (अचिन्हित पूर्णाङ्क i = 0; i < 256; i++)
      यदि (glyphIdArray[i])
	out->add (i);
  }

  शून्य collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping /* OUT */) अचर
  {
    लागि (अचिन्हित i = 0; i < 256; i++)
      यदि (glyphIdArray[i])
      {
	hb_codepoint_t glyph = glyphIdArray[i];
	unicodes->add (i);
	mapping->set (i, glyph);
      }
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
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
  ढाँचा<typename Iterator,
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

    जबसम्म (it) {
      // Start a new range
      {
        अचर स्वतः& pair = *it;
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

      जबसम्म (it) {
        // Process range
        अचर स्वतः& pair = *it;
        hb_codepoint_t next_cp = pair.first;
        hb_codepoint_t next_gid = pair.second;
        यदि (next_cp != end_cp + 1) {
          // Current range is over, stop processing.
          रोक;
        }

        यदि (next_gid == last_gid + 1) {
          // The current run continues.
          end_cp = next_cp;
          run_length++;
          last_gid = next_gid;
          it++;
          जारी;
        }

        // A new run is starting, decide if we want to commit the current run.
        पूर्णाङ्क split_cost = (mode == FIRST_SUB_RANGE) ? 8 : 16;
        पूर्णाङ्क run_cost = run_length * 2;
        यदि (run_cost >= split_cost) {
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

    यदि (likely (end_cp != 0xFFFF)) {
      range_writer (0xFFFF, 0xFFFF, 1);
    }
  }

  /*
   * Writes the current range as either one or two ranges depending on what is most efficient.
   */
  ढाँचा<typename Writer>
  शून्य commit_current_range (hb_codepoint_t start,
                             hb_codepoint_t prev_run_start,
                             hb_codepoint_t run_start,
                             hb_codepoint_t end,
                             पूर्णाङ्क run_delta,
                             पूर्णाङ्क previous_run_delta,
                             पूर्णाङ्क split_cost,
                             Writer& range_writer) {
    तार्किक should_split = असत्य;
    यदि (start < run_start && run_start < end) {
      पूर्णाङ्क run_cost = (end - run_start + 1) * 2;
      यदि (run_cost >= split_cost) {
        should_split = सत्य;
      }
    }

    // A zero delta is also the marker for ranges that need a
    // glyphIdArray; serialize_rangeoffset_glyid() tells the two cases
    // apart by re-checking the mapping.
    यदि (should_split) {
      यदि (start == prev_run_start)
        range_writer (start, run_start - 1, previous_run_delta);
      अन्यथा
        range_writer (start, run_start - 1, 0);
      range_writer (run_start, end, run_delta);
      फर्काऊ;
    }


    यदि (start == run_start) {
      // Range is only a run
      range_writer (start, end, run_delta);
      फर्काऊ;
    }

    // Write only a single non-run range.
    range_writer (start, end, 0);
  }

  ढाँचा<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  अचिन्हित serialize_find_segcount (Iterator it) {
    संरचना Counter {
      अचिन्हित segcount = 0;

      शून्य चालक() (hb_codepoint_t start,
                       hb_codepoint_t end,
                       पूर्णाङ्क delta) {
        segcount++;
      }
    } counter;

    to_ranges (+it, counter);
    फर्काऊ counter.segcount;
  }


  ढाँचा<typename Iterator,
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
      शून्य चालक() (hb_codepoint_t start,
                       hb_codepoint_t end,
                       पूर्णाङ्क delta) {
        start_code_[index_] = start;
        end_code_[index_] = end;
        id_delta_[index_] = delta;
        index_++;
      }
    } writer(c);

    writer.end_code_ = c->allocate_size<HBUINT16> (HBUINT16::static_size * segcount, असत्य);
    (शून्य) c->allocate_size<HBUINT16> (2); // padding
    writer.start_code_ = c->allocate_size<HBUINT16> (HBUINT16::static_size * segcount, असत्य);
    writer.id_delta_ = c->allocate_size<HBINT16> (HBINT16::static_size * segcount, असत्य);

    यदि (unlikely (!writer.end_code_ || !writer.start_code_ || !writer.id_delta_)) फर्काऊ असत्य;

    to_ranges (+it, writer);
    फर्काऊ सत्य;
  }

  ढाँचा<typename Iterator,
          hb_requires (hb_is_iterator (Iterator))>
  HBUINT16* serialize_rangeoffset_glyid (hb_serialize_context_t *c,
                                         Iterator it,
					 HBUINT16 *endCode,
					 HBUINT16 *startCode,
					 HBINT16 *idDelta,
					 अचिन्हित segcount)
  {
    hb_map_t cp_to_gid { it };

    HBUINT16 *idRangeOffset = c->allocate_size<HBUINT16> (HBUINT16::static_size * segcount);
    यदि (unlikely (!c->check_success (idRangeOffset))) फर्काऊ nullptr;
    यदि (unlikely ((वर्ण *)idRangeOffset - (वर्ण *)idDelta != (पूर्णाङ्क) segcount * (पूर्णाङ्क) HBINT16::static_size)) फर्काऊ nullptr;

    लागि (अचिन्हित i : hb_range (segcount))
    {
      /* A non-zero delta is always a real run. A zero delta is
       * ambiguous: identity mapping, or the marker for a range that
       * needs a glyphIdArray. Only zero deltas need a check. */
      पूर्णाङ्क delta = idDelta[i];
      यदि (delta)
	जारी;
      तार्किक use_delta = सत्य;
      लागि (hb_codepoint_t cp = startCode[i]; cp <= endCode[i]; cp++)
      {
	hb_codepoint_t gid = cp_to_gid.get (cp);
	यदि (gid == HB_MAP_मान_INVALID)
	  gid = 0;
	यदि (gid != cp)
	{
	  use_delta = असत्य;
	  रोक;
	}
      }
      यदि (use_delta)
	जारी;

      idRangeOffset[i] = 2 * (c->start_embed<HBUINT16> () - idRangeOffset - i);
      लागि (hb_codepoint_t cp = startCode[i]; cp <= endCode[i]; cp++)
      {
        HBUINT16 gid;
        gid = cp_to_gid[cp];
        c->copy<HBUINT16> (gid);
      }
    }

    फर्काऊ idRangeOffset;
  }

  ढाँचा<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  शून्य serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    स्वतः format4_iter =
    + it
    | hb_filter ([&] (अचर hb_codepoint_pair_t _)
		 { फर्काऊ _.first <= 0xFFFF; })
    ;

    यदि (!format4_iter) फर्काऊ;

    अचिन्हित table_initpos = c->length ();
    यदि (unlikely (!c->extend_min (एतत्))) फर्काऊ;
    एतत्->format = 4;

    hb_vector_t<hb_codepoint_pair_t> cp_to_gid {
      format4_iter
    };

    //serialize endCode[], startCode[], idDelta[]
    HBUINT16* endCode = c->start_embed<HBUINT16> ();
    अचिन्हित segcount = serialize_find_segcount (cp_to_gid.iter());
    यदि (unlikely (!serialize_start_end_delta_arrays (c, cp_to_gid.iter(), segcount)))
      फर्काऊ;

    HBUINT16 *startCode = endCode + segcount + 1;
    HBINT16 *idDelta = ((HBINT16*)startCode) + segcount;

    HBUINT16 *idRangeOffset = serialize_rangeoffset_glyid (c,
                                                           cp_to_gid.iter (),
                                                           endCode,
                                                           startCode,
                                                           idDelta,
                                                           segcount);
    यदि (unlikely (!c->check_success (idRangeOffset))) फर्काऊ;

    एतत्->length = c->length () - table_initpos;
    यदि ((दीर्घ दीर्घ) एतत्->length != (दीर्घ दीर्घ) c->length () - table_initpos)
    {
      c->err (HB_SERIALIZE_ERROR_INT_OVERFLOW);
      फर्काऊ;
    }

    एतत्->seg_गन्ती_X2 = segcount * 2;
    एतत्->entrySelector = hb_max (1u, hb_bit_storage (segcount)) - 1;
    एतत्->खोज_Range = 2 * (1u << एतत्->entrySelector);
    एतत्->rangeShift = segcount * 2 > एतत्->खोज_Range
		       ? 2 * segcount - एतत्->खोज_Range
		       : 0;
  }

  अचिन्हित get_language () अचर
  {
    फर्काऊ language;
  }

  संरचना accelerator_t
  {
    accelerator_t () {}
    accelerator_t (अचर CmapSubtableFormat4 *subtable) = delete;

    शून्य init (अचर CmapSubtableFormat4 *subtable,
	       अचिन्हित पूर्णाङ्क              subtable_data_size)
    {
      seg_गन्ती_2 = subtable->seg_गन्ती_X2 / 2;
      end_गन्ती = subtable->values.arrayZ;
      start_गन्ती = end_गन्ती + seg_गन्ती_2 + 1;
      idDelta = start_गन्ती + seg_गन्ती_2;
      idRangeOffset = idDelta + seg_गन्ती_2;
      glyphIdArray = idRangeOffset + seg_गन्ती_2;

      अचिन्हित पूर्णाङ्क मानहरू_offset = 16 + 8 * seg_गन्ती_2;
      glyphIdArrayLength = subtable_data_size > मानहरू_offset
			   ? (subtable_data_size - मानहरू_offset) / 2
			   : 0;
    }

    तार्किक get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) अचर
    {
      संरचना CustomRange
      {
	पूर्णाङ्क cmp (hb_codepoint_t k,
		 अचिन्हित distance) अचर
	{
	  यदि (k > last) फर्काऊ +1;
	  यदि (k < (&last)[distance]/*first*/) फर्काऊ -1;
	  फर्काऊ 0;
	}
	HBUINT16 last;
      };

      अचर HBUINT16 *found = hb_bsearch (codepoint,
					  एतत्->end_गन्ती,
					  एतत्->seg_गन्ती_2,
					  परिमाण (CustomRange),
					  _hb_cmp_method<hb_codepoint_t, CustomRange, अचिन्हित>,
					  एतत्->seg_गन्ती_2 + 1);
      यदि (unlikely (!found))
	फर्काऊ असत्य;
      अचिन्हित पूर्णाङ्क i = found - end_गन्ती;

      hb_codepoint_t gid;
      अचिन्हित पूर्णाङ्क rangeOffset = एतत्->idRangeOffset[i];
      यदि (rangeOffset == 0)
	gid = codepoint + एतत्->idDelta[i];
      अन्यथा
      {
	/* Somebody has been smoking... */
	अचिन्हित पूर्णाङ्क index = rangeOffset / 2 + (codepoint - एतत्->start_गन्ती[i]) + i - एतत्->seg_गन्ती_2;
	यदि (unlikely (index >= एतत्->glyphIdArrayLength))
	  फर्काऊ असत्य;
	gid = एतत्->glyphIdArray[index];
	यदि (unlikely (!gid))
	  फर्काऊ असत्य;
	gid += एतत्->idDelta[i];
      }
      gid &= 0xFFFFu;
      यदि (unlikely (!gid))
	फर्काऊ असत्य;
      *glyph = gid;
      फर्काऊ सत्य;
    }

    HB_INTERNAL स्थिर तार्किक get_glyph_func (अचर शून्य *obj, hb_codepoint_t codepoint, hb_codepoint_t *glyph)
    { फर्काऊ ((अचर accelerator_t *) obj)->get_glyph (codepoint, glyph); }

    शून्य collect_unicodes (hb_set_t *out) अचर
    {
      अचिन्हित पूर्णाङ्क गन्ती = एतत्->seg_गन्ती_2;
      यदि (गन्ती && एतत्->start_गन्ती[गन्ती - 1] == 0xFFFFu)
	गन्ती--; /* Skip sentinel segment. */
      लागि (अचिन्हित पूर्णाङ्क i = 0; i < गन्ती; i++)
      {
	hb_codepoint_t start = एतत्->start_गन्ती[i];
	hb_codepoint_t end = एतत्->end_गन्ती[i];
	अचिन्हित पूर्णाङ्क rangeOffset = एतत्->idRangeOffset[i];
        out->add_range(start, end);
	यदि (rangeOffset == 0)
	{
	  लागि (hb_codepoint_t codepoint = start; codepoint <= end; codepoint++)
	  {
	    hb_codepoint_t gid = (codepoint + एतत्->idDelta[i]) & 0xFFFFu;
	    यदि (unlikely (!gid))
              out->del(codepoint);
	  }
	}
	अन्यथा
	{
	  लागि (hb_codepoint_t codepoint = start; codepoint <= end; codepoint++)
	  {
	    अचिन्हित पूर्णाङ्क index = rangeOffset / 2 + (codepoint - एतत्->start_गन्ती[i]) + i - एतत्->seg_गन्ती_2;
	    यदि (unlikely (index >= एतत्->glyphIdArrayLength))
            {
              out->del_range (codepoint, end);
	      रोक;
            }
	    hb_codepoint_t gid = एतत्->glyphIdArray[index];
	    यदि (unlikely (!gid))
              out->del(codepoint);
	  }
	}
      }
    }

    शून्य collect_mapping (hb_set_t *unicodes, /* OUT */
			  hb_map_t *mapping /* OUT */) अचर
    {
      // TODO(grieger): optimize similar to collect_unicodes
      // (ie. use add_range())
      अचिन्हित गन्ती = एतत्->seg_गन्ती_2;
      यदि (गन्ती && एतत्->start_गन्ती[गन्ती - 1] == 0xFFFFu)
	गन्ती--; /* Skip sentinel segment. */
      लागि (अचिन्हित i = 0; i < गन्ती; i++)
      {
	hb_codepoint_t start = एतत्->start_गन्ती[i];
	hb_codepoint_t end = एतत्->end_गन्ती[i];
	अचिन्हित rangeOffset = एतत्->idRangeOffset[i];
	यदि (rangeOffset == 0)
	{
	  लागि (hb_codepoint_t codepoint = start; codepoint <= end; codepoint++)
	  {
	    hb_codepoint_t gid = (codepoint + एतत्->idDelta[i]) & 0xFFFFu;
	    यदि (unlikely (!gid))
	      जारी;
	    unicodes->add (codepoint);
	    mapping->set (codepoint, gid);
	  }
	}
	अन्यथा
	{
	  लागि (hb_codepoint_t codepoint = start; codepoint <= end; codepoint++)
	  {
	    अचिन्हित index = rangeOffset / 2 + (codepoint - एतत्->start_गन्ती[i]) + i - एतत्->seg_गन्ती_2;
	    यदि (unlikely (index >= एतत्->glyphIdArrayLength))
	      रोक;
	    hb_codepoint_t gid = एतत्->glyphIdArray[index];
	    यदि (unlikely (!gid))
	      जारी;
            gid = (gid + एतत्->idDelta[i]) & 0xFFFFu;
	    unicodes->add (codepoint);
	    mapping->set (codepoint, gid);
	  }
	}
      }
    }

    अचर HBUINT16 *end_गन्ती;
    अचर HBUINT16 *start_गन्ती;
    अचर HBUINT16 *idDelta;
    अचर HBUINT16 *idRangeOffset;
    अचर HBUINT16 *glyphIdArray;
    अचिन्हित पूर्णाङ्क seg_गन्ती_2;
    अचिन्हित पूर्णाङ्क glyphIdArrayLength;
  };

  तार्किक get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph,
		  अचिन्हित पूर्णाङ्क subtable_data_size) अचर
  {
    accelerator_t accel;
    accel.init (एतत्, subtable_data_size);
    फर्काऊ accel.get_glyph_func (&accel, codepoint, glyph);
  }
  तार्किक get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) अचर
  { फर्काऊ असत्य; }

  शून्य collect_unicodes (hb_set_t *out, अचिन्हित पूर्णाङ्क subtable_data_size) अचर
  {
    accelerator_t accel;
    accel.init (एतत्, subtable_data_size);
    accel.collect_unicodes (out);
  }
  शून्य collect_unicodes (hb_set_t *out) अचर
  { collect_unicodes (out, length); }

  शून्य collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping, /* OUT */
			अचिन्हित पूर्णाङ्क subtable_data_size) अचर
  {
    accelerator_t accel;
    accel.init (एतत्, subtable_data_size);
    accel.collect_mapping (unicodes, mapping);
  }
  शून्य collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping /* OUT */) अचर
  { collect_mapping (unicodes, mapping, length); }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    यदि (unlikely (!c->check_struct (एतत्)))
      return_trace (असत्य);
    hb_barrier ();

    return_trace (c->check_range (values, 2 + 4 * seg_गन्ती_X2));
  }

  रक्षित:
  HBUINT16	format;		/* Format number is set to 4. */
  HBUINT16	length;		/* This is the length in bytes of the
				 * subtable. */
  HBUINT16	language;	/* Ignore. */
  HBUINT16	seg_गन्ती_X2;	/* 2 x segCount. */
  HBUINT16	खोज_Range;	/* 2 * (2**floor(log2(segCount))) */
  HBUINT16	entrySelector;	/* log2(searchRange/2) */
  HBUINT16	rangeShift;	/* 2 x segCount - searchRange */

  UnsizedArrayOf<HBUINT16>
		values;
#यदि 0
  HBUINT16	end_गन्ती[seg_गन्ती_2];	/* End characterCode for each segment,
					 * last=0xFFFFu. */
  HBUINT16	reservedPad;		/* Set to 0. */
  HBUINT16	start_गन्ती[seg_गन्ती_2];	/* Start character code for each segment. */
  HBINT16		idDelta[seg_गन्ती_2];	/* Delta for all character codes in segment. */
  HBUINT16	idRangeOffset[seg_गन्ती_2];/* Offsets into glyphIdArray or 0 */
  UnsizedArrayOf<HBUINT16>
		glyphIdArray;	/* Glyph index array (arbitrary length) */
#endif

  सार्वजनिक:
  DEFINE_SIZE_ARRAY (14, values);
};

संरचना CmapSubtableLongGroup
{
  मित्र संरचना CmapSubtableFormat12;
  मित्र संरचना CmapSubtableFormat13;
  ढाँचा<typename U>
  मित्र संरचना CmapSubtableLongSegmented;
  मित्र संरचना cmap;

  पूर्णाङ्क cmp (hb_codepoint_t codepoint) अचर
  {
    यदि (codepoint < startCharCode) फर्काऊ -1;
    यदि (codepoint > endCharCode)   फर्काऊ +1;
    फर्काऊ 0;
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
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

ढाँचा <typename UINT>
संरचना CmapSubtableTrimmed
{
  तार्किक get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) अचर
  {
    /* Rely on our implicit array bound-checking. */
    hb_codepoint_t gid = glyphIdArray[codepoint - startCharCode];
    यदि (unlikely (!gid))
      फर्काऊ असत्य;
    *glyph = gid;
    फर्काऊ सत्य;
  }

  अचिन्हित get_language () अचर
  {
    फर्काऊ language;
  }

  शून्य collect_unicodes (hb_set_t *out) अचर
  {
    hb_codepoint_t start = startCharCode;
    अचिन्हित पूर्णाङ्क गन्ती = glyphIdArray.len;
    लागि (अचिन्हित पूर्णाङ्क i = 0; i < गन्ती; i++)
      यदि (glyphIdArray[i])
	out->add (start + i);
  }

  शून्य collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping /* OUT */) अचर
  {
    hb_codepoint_t start_cp = startCharCode;
    अचिन्हित गन्ती = glyphIdArray.len;
    लागि (अचिन्हित i = 0; i < गन्ती; i++)
      यदि (glyphIdArray[i])
      {
	hb_codepoint_t unicode = start_cp + i;
	hb_codepoint_t glyphid = glyphIdArray[i];
	unicodes->add (unicode);
	mapping->set (unicode, glyphid);
      }
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
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

ढाँचा <typename T>
संरचना CmapSubtableLongSegmented
{
  मित्र संरचना cmap;

  तार्किक get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) अचर
  {
    hb_codepoint_t gid = T::group_get_glyph (groups.द्वा_खोज (codepoint), codepoint);
    यदि (unlikely (!gid))
      फर्काऊ असत्य;
    *glyph = gid;
    फर्काऊ सत्य;
  }

  अचिन्हित get_language () अचर
  {
    फर्काऊ language;
  }

  शून्य collect_unicodes (hb_set_t *out, अचिन्हित पूर्णाङ्क अङ्क_glyphs) अचर
  {
    लागि (अचिन्हित पूर्णाङ्क i = 0; i < एतत्->groups.len; i++)
    {
      hb_codepoint_t start = एतत्->groups[i].startCharCode;
      hb_codepoint_t end = hb_min ((hb_codepoint_t) एतत्->groups[i].endCharCode,
				   (hb_codepoint_t) HB_UNICODE_MAX);
      hb_codepoint_t gid = एतत्->groups[i].glyphID;
      यदि (!gid)
      {
	/* Intention is: if (hb_is_same (T, CmapSubtableFormat13)) continue; */
	यदि (! T::group_get_glyph (एतत्->groups[i], end)) जारी;
	start++;
	gid++;
      }
      यदि (unlikely ((अचिन्हित पूर्णाङ्क) gid >= अङ्क_glyphs)) जारी;
      यदि (unlikely ((अचिन्हित पूर्णाङ्क) (gid + end - start) >= अङ्क_glyphs))
	end = start + (hb_codepoint_t) अङ्क_glyphs - gid;

      out->add_range (start, hb_min (end, 0x10FFFFu));
    }
  }

  शून्य collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping, /* OUT */
			अचिन्हित अङ्क_glyphs) अचर
  {
    hb_codepoint_t last_end = 0;
    अचिन्हित गन्ती = एतत्->groups.len;
    लागि (अचिन्हित i = 0; i < गन्ती; i++)
    {
      hb_codepoint_t start = एतत्->groups.arrayZ[i].startCharCode;
      hb_codepoint_t end = hb_min ((hb_codepoint_t) एतत्->groups.arrayZ[i].endCharCode,
				   (hb_codepoint_t) HB_UNICODE_MAX);
      यदि (unlikely (start > end || start < last_end)) {
        // Range is not in order and is invalid, skip it.
        जारी;
      }
      last_end = end;


      hb_codepoint_t gid = एतत्->groups.arrayZ[i].glyphID;
      यदि (!gid)
      {
        यदि (T::format_सङ्ख्या == 13) जारी;
	start++;
	gid++;
      }
      यदि (unlikely ((अचिन्हित पूर्णाङ्क) gid >= अङ्क_glyphs)) जारी;
      यदि (unlikely ((अचिन्हित पूर्णाङ्क) (gid + end - start) >= अङ्क_glyphs))
	end = start + (hb_codepoint_t) अङ्क_glyphs - gid;

      mapping->alloc (mapping->get_population () + end - start + 1);

      unicodes->add_range (start, end);
      लागि (अचिन्हित cp = start; cp <= end; cp++)
      {
	mapping->set (cp, gid);
        gid += T::increment;
      }
    }
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
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

  स्थिर hb_codepoint_t group_get_glyph (अचर CmapSubtableLongGroup &group,
					 hb_codepoint_t u)
  { फर्काऊ likely (group.startCharCode <= group.endCharCode) ?
	   group.glyphID + (u - group.startCharCode) : 0; }


  ढाँचा<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  शून्य serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    यदि (!it) फर्काऊ;
    अचिन्हित table_initpos = c->length ();
    यदि (unlikely (!c->extend_min (एतत्))) फर्काऊ;

    hb_codepoint_t startCharCode = (hb_codepoint_t) -1, endCharCode = (hb_codepoint_t) -1;
    hb_codepoint_t glyphID = 0;

    लागि (अचर स्वतः& _ : +it)
    {
      यदि (startCharCode == (hb_codepoint_t) -1)
      {
	startCharCode = _.first;
	endCharCode = _.first;
	glyphID = _.second;
      }
      अन्यथा यदि (!_is_gid_consecutive (endCharCode, startCharCode, glyphID, _.first, _.second))
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
      अन्यथा
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

  स्थिर माप_प्रकार get_sub_table_size (अचर hb_sorted_vector_t<CmapSubtableLongGroup> &groups_data)
  { फर्काऊ 16 + 12 * groups_data.length; }

  गुप्त:
  स्थिर तार्किक _is_gid_consecutive (hb_codepoint_t endCharCode,
				   hb_codepoint_t startCharCode,
				   hb_codepoint_t glyphID,
				   hb_codepoint_t cp,
				   hb_codepoint_t new_gid)
  {
    फर्काऊ (cp - 1 == endCharCode) &&
	new_gid == glyphID + (cp - startCharCode);
  }

};

संरचना CmapSubtableFormat13 : CmapSubtableLongSegmented<CmapSubtableFormat13>
{
  स्थिर constexpr पूर्णाङ्क increment = 0;
  स्थिर constexpr पूर्णाङ्क format_सङ्ख्या = 13;

  स्थिर hb_codepoint_t group_get_glyph (अचर CmapSubtableLongGroup &group,
					 hb_codepoint_t u HB_UNUSED)
  { फर्काऊ group.glyphID; }
};

प्रकारनिर्देश गणना
{
  GLYPH_VARIANT_NOT_FOUND = 0,
  GLYPH_VARIANT_FOUND = 1,
  GLYPH_VARIANT_USE_DEFAULT = 2
} glyph_variant_t;

संरचना Unicode_मान_Range
{
  पूर्णाङ्क cmp (अचर hb_codepoint_t &codepoint) अचर
  {
    यदि (codepoint < start_Unicode_मान) फर्काऊ -1;
    यदि (codepoint > start_Unicode_मान + additional_गन्ती) फर्काऊ +1;
    फर्काऊ 0;
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्));
  }

  HBUINT24	start_Unicode_मान;	/* First value in this range. */
  HBUINT8	additional_गन्ती;	/* Number of additional values in this
					 * range. */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (4);
};

संरचना DefaultUVS : SortedArray32Of<Unicode_मान_Range>
{
  शून्य collect_unicodes (hb_set_t *out) अचर
  {
    अचिन्हित पूर्णाङ्क गन्ती = len;
    लागि (अचिन्हित पूर्णाङ्क i = 0; i < गन्ती; i++)
    {
      hb_codepoint_t first = arrayZ[i].start_Unicode_मान;
      hb_codepoint_t last = hb_min ((hb_codepoint_t) (first + arrayZ[i].additional_गन्ती),
				    (hb_codepoint_t) HB_UNICODE_MAX);
      out->add_range (first, last);
    }
  }

  DefaultUVS* copy (hb_serialize_context_t *c,
		    अचर hb_set_t *unicodes) अचर
  {
    स्वतः *out = c->start_embed<DefaultUVS> ();
    स्वतः snap = c->snapshot ();

    HBUINT32 len;
    len = 0;
    यदि (unlikely (!c->copy<HBUINT32> (len))) फर्काऊ nullptr;
    अचिन्हित init_len = c->length ();

    यदि (एतत्->len > unicodes->get_population () * hb_bit_storage ((अचिन्हित) एतत्->len))
    {
      hb_codepoint_t start = HB_SET_मान_INVALID;
      hb_codepoint_t end = HB_SET_मान_INVALID;

      लागि (स्वतः u : *unicodes)
      {
        यदि (!as_array ().द्वा_खोज (u))
	  जारी;
	यदि (start == HB_SET_मान_INVALID)
	{
	  start = u;
	  end = start - 1;
	}
	यदि (end + 1 != u || end - start == 255)
        {
	  Unicode_मान_Range rec;
	  rec.start_Unicode_मान = start;
	  rec.additional_गन्ती = end - start;
	  c->copy<Unicode_मान_Range> (rec);
	  start = u;
	}
	end = u;
      }
      यदि (start != HB_SET_मान_INVALID)
      {
	Unicode_मान_Range rec;
	rec.start_Unicode_मान = start;
	rec.additional_गन्ती = end - start;
	c->copy<Unicode_मान_Range> (rec);
      }

    }
    अन्यथा
    {
      hb_codepoint_t lastCode = HB_SET_मान_INVALID;
      पूर्णाङ्क गन्ती = -1;

      लागि (अचर Unicode_मान_Range& _ : *एतत्)
      {
	hb_codepoint_t curEntry = (hb_codepoint_t) (_.start_Unicode_मान - 1);
	hb_codepoint_t end = curEntry + _.additional_गन्ती + 2;

	लागि (; unicodes->next (&curEntry) && curEntry < end;)
	{
	  गन्ती += 1;
	  यदि (lastCode == HB_SET_मान_INVALID)
	    lastCode = curEntry;
	  अन्यथा यदि (lastCode + गन्ती != curEntry || गन्ती > 255)
	  {
	    Unicode_मान_Range rec;
	    rec.start_Unicode_मान = lastCode;
	    rec.additional_गन्ती = गन्ती - 1;
	    c->copy<Unicode_मान_Range> (rec);

	    lastCode = curEntry;
	    गन्ती = 0;
	  }
	}
      }

      यदि (lastCode != HB_MAP_मान_INVALID)
      {
	Unicode_मान_Range rec;
	rec.start_Unicode_मान = lastCode;
	rec.additional_गन्ती = गन्ती;
	c->copy<Unicode_मान_Range> (rec);
      }
    }

    यदि (c->length () - init_len == 0)
    {
      c->revert (snap);
      फर्काऊ nullptr;
    }
    अन्यथा
    {
      यदि (unlikely (!c->check_assign (out->len,
                                      (c->length () - init_len) / Unicode_मान_Range::static_size,
                                      HB_SERIALIZE_ERROR_INT_OVERFLOW))) फर्काऊ nullptr;
      फर्काऊ out;
    }
  }

  सार्वजनिक:
  DEFINE_SIZE_ARRAY (4, *एतत्);
};

ढाँचा <typename Types>
संरचना UVSMapping
{
  using GlyphID = typename Types::HBGlyphID;

  पूर्णाङ्क cmp (अचर hb_codepoint_t &codepoint) अचर
  { फर्काऊ unicode_मान.cmp (codepoint); }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्));
  }

  HBUINT24	unicode_मान;	/* Base Unicode value of the UVS */
  GlyphID
		glyphID;	/* Glyph ID of the UVS. */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (3 + GlyphID::static_size);
};

ढाँचा <typename Types>
संरचना NonDefaultUVS : SortedArray32Of<UVSMapping<Types>>
{
  using Mapping = UVSMapping<Types>;

  शून्य collect_unicodes (hb_set_t *out) अचर
  {
    लागि (अचर स्वतः& a : एतत्->as_array ())
      out->add (a.unicode_मान);
  }

  शून्य collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping /* OUT */) अचर
  {
    लागि (अचर स्वतः& a : एतत्->as_array ())
    {
      hb_codepoint_t unicode = a.unicode_मान;
      hb_codepoint_t glyphid = a.glyphID;
      unicodes->add (unicode);
      mapping->set (unicode, glyphid);
    }
  }

  शून्य closure_glyphs (अचर hb_set_t      *unicodes,
		       hb_set_t            *glyphset) अचर
  {
    + एतत्->as_array ()
    | hb_filter (unicodes, &Mapping::unicode_मान)
    | hb_map (&Mapping::glyphID)
    | hb_sink (glyphset)
    ;
  }

  NonDefaultUVS* copy (hb_serialize_context_t *c,
		       अचर hb_set_t *unicodes,
		       अचर hb_set_t *glyphs_requested,
		       अचर hb_map_t *glyph_map) अचर
  {
    स्वतः *out = c->start_embed<NonDefaultUVS> ();
    स्वतः it =
    + एतत्->as_array ()
    | hb_filter ([&] (अचर Mapping& _)
		 {
		   फर्काऊ unicodes->has (_.unicode_मान) || glyphs_requested->has (_.glyphID);
		 })
    ;

    यदि (!it) फर्काऊ nullptr;

    HBUINT32 len;
    len = it.len ();
    यदि (unlikely (!c->copy<HBUINT32> (len))) फर्काऊ nullptr;

    लागि (अचर Mapping& _ : it)
    {
      Mapping mapping;
      mapping.unicode_मान = _.unicode_मान;
      mapping.glyphID = glyph_map->get (_.glyphID);
      c->copy<Mapping> (mapping);
    }

    फर्काऊ out;
  }

  सार्वजनिक:
  DEFINE_SIZE_ARRAY (4, *एतत्);
};

ढाँचा <typename Types>
संरचना VariationSelectorRecord
{
  glyph_variant_t get_glyph (hb_codepoint_t codepoint,
			     hb_codepoint_t *glyph,
			     अचर शून्य *base) अचर
  {
    यदि ((base+defaultUVS).bfind (codepoint))
      फर्काऊ GLYPH_VARIANT_USE_DEFAULT;
    अचर UVSMapping<Types> &nonDefault = (base+nonDefaultUVS).द्वा_खोज (codepoint);
    यदि (nonDefault.glyphID)
    {
      *glyph = nonDefault.glyphID;
       फर्काऊ GLYPH_VARIANT_FOUND;
    }
    फर्काऊ GLYPH_VARIANT_NOT_FOUND;
  }

  VariationSelectorRecord(अचर VariationSelectorRecord& other)
  {
    *एतत् = other;
  }

  शून्य चालक= (अचर VariationSelectorRecord& other)
  {
    varSelector = other.varSelector;
    HBUINT32 offset = other.defaultUVS;
    defaultUVS = offset;
    offset = other.nonDefaultUVS;
    nonDefaultUVS = offset;
  }

  शून्य collect_unicodes (hb_set_t *out, अचर शून्य *base) अचर
  {
    (base+defaultUVS).collect_unicodes (out);
    (base+nonDefaultUVS).collect_unicodes (out);
  }

  शून्य collect_mapping (अचर शून्य *base,
			hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping /* OUT */) अचर
  {
    (base+defaultUVS).collect_unicodes (unicodes);
    (base+nonDefaultUVS).collect_mapping (unicodes, mapping);
  }

  पूर्णाङ्क cmp (अचर hb_codepoint_t &variation_selector) अचर
  { फर्काऊ varSelector.cmp (variation_selector); }

  तार्किक sanitize (hb_sanitize_context_t *c, अचर शून्य *base) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
		  defaultUVS.sanitize (c, base) &&
		  nonDefaultUVS.sanitize (c, base));
  }

  hb_pair_t<अचिन्हित, अचिन्हित>
  copy (hb_serialize_context_t *c,
	अचर hb_set_t *unicodes,
	अचर hb_set_t *glyphs_requested,
	अचर hb_map_t *glyph_map,
	अचर शून्य *base) अचर
  {
    स्वतः snap = c->snapshot ();
    स्वतः *out = c->embed<VariationSelectorRecord> (*एतत्);
    यदि (unlikely (!out)) फर्काऊ hb_pair (0, 0);

    out->defaultUVS = 0;
    out->nonDefaultUVS = 0;

    अचिन्हित non_default_uvs_objidx = 0;
    यदि (nonDefaultUVS != 0)
    {
      c->push ();
      यदि (c->copy (base+nonDefaultUVS, unicodes, glyphs_requested, glyph_map))
	non_default_uvs_objidx = c->झिक_pack ();
      अन्यथा c->झिक_discard ();
    }

    अचिन्हित default_uvs_objidx = 0;
    यदि (defaultUVS != 0)
    {
      c->push ();
      यदि (c->copy (base+defaultUVS, unicodes))
	default_uvs_objidx = c->झिक_pack ();
      अन्यथा c->झिक_discard ();
    }


    यदि (!default_uvs_objidx && !non_default_uvs_objidx)
      c->revert (snap);

    फर्काऊ hb_pair (default_uvs_objidx, non_default_uvs_objidx);
  }

  HBUINT24	varSelector;	/* Variation selector. */
  Offset32To<DefaultUVS>
		defaultUVS;	/* Offset to Default UVS Table.  May be 0. */
  Offset32To<NonDefaultUVS<Types>>
		nonDefaultUVS;	/* Offset to Non-Default UVS Table.  May be 0. */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (11);
};

ढाँचा <typename Types>
संरचना CmapSubtableFormat14_15
{
  स्थिर constexpr अचिन्हित format_मान = Types::size == 2 ? 14 : 15;
  using Record = VariationSelectorRecord<Types>;
  using NonDefault = NonDefaultUVS<Types>;

  glyph_variant_t get_glyph_variant (hb_codepoint_t codepoint,
				     hb_codepoint_t variation_selector,
				     hb_codepoint_t *glyph) अचर
  { फर्काऊ record.द्वा_खोज (variation_selector).get_glyph (codepoint, glyph, एतत्); }

  शून्य collect_variation_selectors (hb_set_t *out) अचर
  {
    लागि (अचर स्वतः& a : record.as_array ())
      out->add (a.varSelector);
  }
  शून्य collect_variation_unicodes (hb_codepoint_t variation_selector,
				   hb_set_t *out) अचर
  { record.द्वा_खोज (variation_selector).collect_unicodes (out, एतत्); }

  शून्य serialize (hb_serialize_context_t *c,
		  अचर hb_set_t *unicodes,
		  अचर hb_set_t *glyphs_requested,
		  अचर hb_map_t *glyph_map,
		  अचर शून्य *base)
  {
    स्वतः snap = c->snapshot ();
    अचिन्हित table_initpos = c->length ();
    अचर वर्ण* init_tail = c->tail;

    यदि (unlikely (!c->extend_min (एतत्))) फर्काऊ;
    एतत्->format = format_मान;

    स्वतः src_tbl = reinterpret_cast<अचर CmapSubtableFormat14_15*> (base);

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
    hb_vector_t<hb_pair_t<अचिन्हित, अचिन्हित>> obj_indices;
    लागि (पूर्णाङ्क i = src_tbl->record.len - 1; i >= 0; i--)
    {
      यदि (!unicodes->has(src_tbl->record[i].varSelector))
        जारी;

      hb_pair_t<अचिन्हित, अचिन्हित> result = src_tbl->record[i].copy (c, unicodes, glyphs_requested, glyph_map, base);
      यदि (result.first || result.second)
	obj_indices.push (result);
    }

    यदि (c->length () - table_initpos == CmapSubtableFormat14_15::min_size)
    {
      c->revert (snap);
      फर्काऊ;
    }

    यदि (unlikely (!c->check_success (!obj_indices.in_error ())))
      फर्काऊ;

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
					अचर hb_vector_t<hb_pair_t<अचिन्हित, अचिन्हित>>& obj_indices)
  {
    लागि (अचिन्हित i = 0; i < obj_indices.length; i++)
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

  शून्य closure_glyphs (अचर hb_set_t      *unicodes,
		       hb_set_t            *glyphset) अचर
  {
    + hb_iter (record)
    | hb_filter (hb_bool, &Record::nonDefaultUVS)
    | hb_filter (unicodes, &Record::varSelector)
    | hb_map (&Record::nonDefaultUVS)
    | hb_map (hb_add (एतत्))
    | hb_apply ([=] (अचर NonDefault& _) { _.closure_glyphs (unicodes, glyphset); })
    ;
  }

  शून्य collect_unicodes (hb_set_t *out) अचर
  {
    लागि (अचर Record& _ : record)
      _.collect_unicodes (out, एतत्);
  }

  शून्य collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping /* OUT */) अचर
  {
    लागि (अचर Record& _ : record)
      _.collect_mapping (एतत्, unicodes, mapping);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
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

  तार्किक is_variation_selector () अचर
  {
    विकल्प (u.format.v) {
    पक्ष 14:
#ifndef HB_NO_BEYOND_64K
    पक्ष 15:
#endif
      फर्काऊ सत्य;
    स्वभाव:
      फर्काऊ असत्य;
    }
  }

  glyph_variant_t get_glyph_variant (hb_codepoint_t codepoint,
				     hb_codepoint_t variation_selector,
				     hb_codepoint_t *glyph) अचर
  {
    विकल्प (u.format.v) {
    पक्ष 14: hb_barrier (); फर्काऊ u.format14.get_glyph_variant (codepoint, variation_selector, glyph);
#ifndef HB_NO_BEYOND_64K
    पक्ष 15: hb_barrier (); फर्काऊ u.format15.get_glyph_variant (codepoint, variation_selector, glyph);
#endif
    स्वभाव: फर्काऊ GLYPH_VARIANT_NOT_FOUND;
    }
  }

  शून्य collect_variation_selectors (hb_set_t *out) अचर
  {
    विकल्प (u.format.v) {
    पक्ष 14: hb_barrier (); u.format14.collect_variation_selectors (out); फर्काऊ;
#ifndef HB_NO_BEYOND_64K
    पक्ष 15: hb_barrier (); u.format15.collect_variation_selectors (out); फर्काऊ;
#endif
    स्वभाव: फर्काऊ;
    }
  }

  शून्य collect_variation_unicodes (hb_codepoint_t variation_selector,
				    hb_set_t *out) अचर
  {
    विकल्प (u.format.v) {
    पक्ष 14: hb_barrier (); u.format14.collect_variation_unicodes (variation_selector, out); फर्काऊ;
#ifndef HB_NO_BEYOND_64K
    पक्ष 15: hb_barrier (); u.format15.collect_variation_unicodes (variation_selector, out); फर्काऊ;
#endif
    स्वभाव: फर्काऊ;
    }
  }

  शून्य closure_glyphs (अचर hb_set_t *unicodes, hb_set_t *glyphset) अचर
  {
    विकल्प (u.format.v) {
    पक्ष 14: hb_barrier (); u.format14.closure_glyphs (unicodes, glyphset); फर्काऊ;
#ifndef HB_NO_BEYOND_64K
    पक्ष 15: hb_barrier (); u.format15.closure_glyphs (unicodes, glyphset); फर्काऊ;
#endif
    स्वभाव: फर्काऊ;
    }
  }

  तार्किक get_glyph (hb_codepoint_t codepoint,
		  hb_codepoint_t *glyph,
		  अचिन्हित पूर्णाङ्क subtable_data_size = 0) अचर
  {
    विकल्प (u.format.v) {
    पक्ष  0: hb_barrier (); फर्काऊ u.format0 .get_glyph (codepoint, glyph);
    पक्ष  4: hb_barrier (); फर्काऊ subtable_data_size
				      ? u.format4.get_glyph (codepoint, glyph, subtable_data_size)
				      : असत्य;
    पक्ष  6: hb_barrier (); फर्काऊ u.format6 .get_glyph (codepoint, glyph);
    पक्ष 10: hb_barrier (); फर्काऊ u.format10.get_glyph (codepoint, glyph);
    पक्ष 12: hb_barrier (); फर्काऊ u.format12.get_glyph (codepoint, glyph);
    पक्ष 13: hb_barrier (); फर्काऊ u.format13.get_glyph (codepoint, glyph);
    पक्ष 14:
    स्वभाव: फर्काऊ असत्य;
    }
  }
  शून्य collect_unicodes (hb_set_t *out,
			 अचिन्हित पूर्णाङ्क अङ्क_glyphs = अच_पूर्णांक_उच्च,
			 अचिन्हित पूर्णाङ्क subtable_data_size = 0) अचर
  {
    विकल्प (u.format.v) {
    पक्ष  0: hb_barrier (); u.format0 .collect_unicodes (out); फर्काऊ;
    पक्ष  4: hb_barrier (); यदि (subtable_data_size)
			      u.format4.collect_unicodes (out, subtable_data_size);
			    अन्यथा
			      u.format4.collect_unicodes (out);
			    फर्काऊ;
    पक्ष  6: hb_barrier (); u.format6 .collect_unicodes (out); फर्काऊ;
    पक्ष 10: hb_barrier (); u.format10.collect_unicodes (out); फर्काऊ;
    पक्ष 12: hb_barrier (); u.format12.collect_unicodes (out, अङ्क_glyphs); फर्काऊ;
    पक्ष 13: hb_barrier (); u.format13.collect_unicodes (out, अङ्क_glyphs); फर्काऊ;
    पक्ष 14:
    स्वभाव: फर्काऊ;
    }
  }

  शून्य collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping, /* OUT */
			अचिन्हित अङ्क_glyphs = अच_पूर्णांक_उच्च,
			अचिन्हित पूर्णाङ्क subtable_data_size = 0) अचर
  {
    विकल्प (u.format.v) {
    पक्ष  0: hb_barrier (); u.format0 .collect_mapping (unicodes, mapping); फर्काऊ;
    पक्ष  4: hb_barrier (); यदि (subtable_data_size)
			      u.format4.collect_mapping (unicodes, mapping, subtable_data_size);
			    अन्यथा
			      u.format4.collect_mapping (unicodes, mapping);
			    फर्काऊ;
    पक्ष  6: hb_barrier (); u.format6 .collect_mapping (unicodes, mapping); फर्काऊ;
    पक्ष 10: hb_barrier (); u.format10.collect_mapping (unicodes, mapping); फर्काऊ;
    पक्ष 12: hb_barrier (); u.format12.collect_mapping (unicodes, mapping, अङ्क_glyphs); फर्काऊ;
    पक्ष 13: hb_barrier (); u.format13.collect_mapping (unicodes, mapping, अङ्क_glyphs); फर्काऊ;
    पक्ष 14:
    स्वभाव: फर्काऊ;
    }
  }

  अचिन्हित get_language () अचर
  {
    विकल्प (u.format.v) {
    पक्ष  0: hb_barrier (); फर्काऊ u.format0 .get_language ();
    पक्ष  4: hb_barrier (); फर्काऊ u.format4 .get_language ();
    पक्ष  6: hb_barrier (); फर्काऊ u.format6 .get_language ();
    पक्ष 10: hb_barrier (); फर्काऊ u.format10.get_language ();
    पक्ष 12: hb_barrier (); फर्काऊ u.format12.get_language ();
    पक्ष 13: hb_barrier (); फर्काऊ u.format13.get_language ();
    पक्ष 14:
    स्वभाव: फर्काऊ 0;
    }
  }

  ढाँचा<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  शून्य serialize (hb_serialize_context_t *c,
		  Iterator it,
		  अचिन्हित format,
		  अचर hb_subset_plan_t *plan,
		  अचर शून्य *base)
  {
    विकल्प (format) {
    पक्ष  4: hb_barrier (); फर्काऊ u.format4.serialize (c, it);
    पक्ष 12: hb_barrier (); फर्काऊ u.format12.serialize (c, it);
    पक्ष 14: hb_barrier (); फर्काऊ u.format14.serialize (c, &plan->unicodes, &plan->glyphs_requested, plan->glyph_map, base);
#ifndef HB_NO_BEYOND_64K
    पक्ष 15: hb_barrier (); फर्काऊ u.format15.serialize (c, &plan->unicodes, &plan->glyphs_requested, plan->glyph_map, base);
#endif
    स्वभाव: फर्काऊ;
    }
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    यदि (!u.format.v.sanitize (c)) return_trace (असत्य);
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
    स्वभाव:return_trace (सत्य);
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
  पूर्णाङ्क cmp (अचर EncodingRecord &other) अचर
  {
    पूर्णाङ्क ret;
    ret = platformID.cmp (other.platformID);
    यदि (ret) फर्काऊ ret;
    यदि (other.encodingID != 0xFFFF)
    {
      ret = encodingID.cmp (other.encodingID);
      यदि (ret) फर्काऊ ret;
    }
    फर्काऊ 0;
  }

  तार्किक sanitize (hb_sanitize_context_t *c, अचर शून्य *base) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
		  subtable.sanitize (c, base));
  }

  ढाँचा<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  EncodingRecord* copy (hb_serialize_context_t *c,
			Iterator it,
			अचिन्हित format,
			अचर शून्य *base,
			अचर hb_subset_plan_t *plan,
			/* INOUT */ अचिन्हित *objidx) अचर
  {
    TRACE_SERIALIZE (एतत्);
    स्वतः snap = c->snapshot ();
    स्वतः *out = c->embed (एतत्);
    यदि (unlikely (!out)) return_trace (nullptr);
    out->subtable = 0;

    यदि (*objidx == 0)
    {
      CmapSubtable *cmapsubtable = c->push<CmapSubtable> ();
      अचिन्हित origin_length = c->length ();
      cmapsubtable->serialize (c, it, format, plan, &(base+subtable));
      यदि (c->length () > origin_length && !c->in_error()) *objidx = c->झिक_pack ();
      अन्यथा c->झिक_discard ();
    }

    यदि (*objidx == 0)
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
  अचर वर्ण* base;
  अचिन्हित पूर्णाङ्क table_length;
  hb_hashmap_t<अचिन्हित, hb::unique_ptr<hb_set_t>> cached_unicodes;

 सार्वजनिक:

  स्थिर SubtableUnicodesCache* create (hb_blob_ptr_t<cmap> source_table)
  {
    SubtableUnicodesCache* cache =
        (SubtableUnicodesCache*) hb_malloc (परिमाण(SubtableUnicodesCache));

    यदि (unlikely (!cache))
      फर्काऊ nullptr;

    नूतन (cache) SubtableUnicodesCache (source_table);
    फर्काऊ cache;
  }

  स्थिर शून्य destroy (शून्य* value) {
    यदि (!value) फर्काऊ;

    SubtableUnicodesCache* cache = (SubtableUnicodesCache*) value;
    cache->~SubtableUnicodesCache ();
    hb_free (cache);
  }

  SubtableUnicodesCache(अचर शून्य* cmap_base,
			अचिन्हित पूर्णाङ्क table_length_ = 0)
      : base_blob(),
        base ((अचर वर्ण*) cmap_base),
        table_length (table_length_),
        cached_unicodes ()
  {}

  SubtableUnicodesCache(hb_blob_ptr_t<cmap> base_blob_)
      : base_blob(base_blob_),
        base ((अचर वर्ण *) base_blob.get()),
        table_length (base_blob.get_length ()),
        cached_unicodes ()
  {}

  ~SubtableUnicodesCache()
  {
    base_blob.destroy ();
  }

  तार्किक same_base(अचर शून्य* other) अचर
  {
    फर्काऊ other == (अचर शून्य*) base;
  }

  अचर hb_set_t* set_for (अचर EncodingRecord* record,
                           SubtableUnicodesCache& mutable_cache) अचर
  {
    यदि (cached_unicodes.has ((अचिन्हित) ((अचर वर्ण *) record - base)))
      फर्काऊ cached_unicodes.get ((अचिन्हित) ((अचर वर्ण *) record - base));

    फर्काऊ mutable_cache.set_for (record);
  }

  अचर hb_set_t* set_for (अचर EncodingRecord* record)
  {
    यदि (!cached_unicodes.has ((अचिन्हित) ((अचर वर्ण *) record - base)))
    {
      hb_set_t *s = hb_set_create ();
      यदि (unlikely (s->in_error ()))
	फर्काऊ hb_set_get_empty ();

      अचिन्हित पूर्णाङ्क subtable_data_size = record->subtable < table_length
					? table_length - (अचिन्हित पूर्णाङ्क) record->subtable
					: 0;
      (base+record->subtable).collect_unicodes (s, अच_पूर्णांक_उच्च, subtable_data_size);

      यदि (unlikely (!cached_unicodes.set ((अचिन्हित) ((अचर वर्ण *) record - base), hb::unique_ptr<hb_set_t> {s})))
        फर्काऊ hb_set_get_empty ();

      फर्काऊ s;
    }
    फर्काऊ cached_unicodes.get ((अचिन्हित) ((अचर वर्ण *) record - base));
  }

};

स्थिर अन्तःस्थ uint16_t
hb_चिह्न_pua_map (अचिन्हित codepoint)
{
  यदि (codepoint <= 0x00FFu)
  {
    /* For symbol-encoded OpenType fonts, we duplicate the
     * U+F000..F0FF range at U+0000..U+00FF.  That's what
     * Windows seems to do, and that's hinted about at:
     * https://docs.microsoft.com/en-us/typography/opentype/spec/recom
     * under "Non-Standard (Symbol) Fonts". */
    फर्काऊ 0xF000u + codepoint;
  }
  फर्काऊ 0;
}

संरचना cmap
{
  स्थिर constexpr hb_tag_t tableTag = HB_OT_TAG_cmap;


  स्थिर SubtableUnicodesCache* create_filled_cache(hb_blob_ptr_t<cmap> source_table) {
    अचर cmap* cmap = source_table.get();
    स्वतः it =
    + hb_iter (cmap->encodingRecord)
    | hb_filter ([&](अचर EncodingRecord& _) {
      फर्काऊ cmap::filter_encoding_records_for_subset (cmap, _);
    })
    ;

    SubtableUnicodesCache* cache = SubtableUnicodesCache::create(source_table);

    यदि (unlikely (!cache))
      फर्काऊ nullptr;

    लागि (अचर EncodingRecord& _ : it)
      cache->set_for(&_); // populate the cache for this encoding record.

    फर्काऊ cache;
  }

  ढाँचा<typename Iterator, typename EncodingRecIter,
	   hb_requires (hb_is_iterator (EncodingRecIter))>
  तार्किक serialize (hb_serialize_context_t *c,
		  Iterator it,
		  EncodingRecIter encodingrec_iter,
		  अचर शून्य *base,
		  hb_subset_plan_t *plan,
	                  तार्किक drop_format_4 = असत्य,
			  अचिन्हित पूर्णाङ्क source_table_length = 0)
  {
    यदि (unlikely (!c->extend_min ((*एतत्))))  फर्काऊ असत्य;
    एतत्->version = 0;

    अचिन्हित format4objidx = 0, format12objidx = 0, format14objidx = 0;
#ifndef HB_NO_BEYOND_64K
    अचिन्हित format15objidx = 0;
#endif
    स्वतः snap = c->snapshot ();

    SubtableUnicodesCache local_unicodes_cache (base, source_table_length);
    अचर SubtableUnicodesCache* unicodes_cache = &local_unicodes_cache;

    यदि (plan->accelerator &&
        plan->accelerator->cmap_cache &&
        plan->accelerator->cmap_cache->same_base (base))
      unicodes_cache = plan->accelerator->cmap_cache;

    लागि (अचर EncodingRecord& _ : encodingrec_iter)
    {
      यदि (c->in_error ())
        फर्काऊ असत्य;

      अचिन्हित format = (base+_.subtable).u.format.v;
      यदि (format != 4 && format != 12 && format != 14
#ifndef HB_NO_BEYOND_64K
	  && format != 15
#endif
	 ) जारी;

      अचर hb_set_t* unicodes_set = unicodes_cache->set_for (&_, local_unicodes_cache);

      यदि (!drop_format_4 && format == 4)
      {
        c->copy (_, + it | hb_filter (*unicodes_set, hb_first), 4u, base, plan, &format4objidx);
        यदि (c->in_error () && c->only_overflow ())
        {
          // cmap4 overflowed, reset and retry serialization without format 4 subtables.
          c->revert (snap);
          फर्काऊ serialize (c, it,
                            encodingrec_iter,
                            base,
                            plan,
                            सत्य,
			    source_table_length);
        }
      }

      अन्यथा यदि (format == 12)
      {
        यदि (_can_drop (_,
                       *unicodes_set,
                       base,
                       *unicodes_cache,
                       local_unicodes_cache,
                       + it | hb_map (hb_first), encodingrec_iter))
          जारी;
        c->copy (_, + it | hb_filter (*unicodes_set, hb_first), 12u, base, plan, &format12objidx);
      }
      अन्यथा यदि (format == 14) c->copy (_, it, 14u, base, plan, &format14objidx);
#ifndef HB_NO_BEYOND_64K
      अन्यथा यदि (format == 15) c->copy (_, it, 15u, base, plan, &format15objidx);
#endif
    }
        अचिन्हित length = c->length ();
        अचिन्हित available = length > cmap::min_size ? length - cmap::min_size : 0;
        c->check_assign(एतत्->encodingRecord.len,
                        available / EncodingRecord::static_size,
                        HB_SERIALIZE_ERROR_INT_OVERFLOW);

    // Fail if format 4 was dropped and there is no cmap12.
    फर्काऊ !drop_format_4 || format12objidx;
  }

  ढाँचा<typename Iterator, typename EncodingRecordIterator,
      hb_requires (hb_is_iterator (Iterator)),
      hb_requires (hb_is_iterator (EncodingRecordIterator))>
  तार्किक _can_drop (अचर EncodingRecord& cmap12,
                  अचर hb_set_t& cmap12_unicodes,
                  अचर शून्य* base,
                  अचर SubtableUnicodesCache& unicodes_cache,
                  SubtableUnicodesCache& local_unicodes_cache,
                  Iterator subset_unicodes,
                  EncodingRecordIterator encoding_records)
  {
    लागि (स्वतः cp : + subset_unicodes | hb_filter (cmap12_unicodes))
    {
      यदि (cp >= 0x10000) फर्काऊ असत्य;
    }

    अचिन्हित target_platform;
    अचिन्हित target_encoding;
    अचिन्हित target_language = (base+cmap12.subtable).get_language ();

    यदि (cmap12.platformID == 0 && cmap12.encodingID == 4)
    {
      target_platform = 0;
      target_encoding = 3;
    } अन्यथा यदि (cmap12.platformID == 3 && cmap12.encodingID == 10) {
      target_platform = 3;
      target_encoding = 1;
    } अन्यथा {
      फर्काऊ असत्य;
    }

    लागि (अचर स्वतः& _ : encoding_records)
    {
      यदि (_.platformID != target_platform
          || _.encodingID != target_encoding
          || (base+_.subtable).get_language() != target_language)
        जारी;

      अचर hb_set_t* sibling_unicodes = unicodes_cache.set_for (&_, local_unicodes_cache);

      स्वतः cmap12 = + subset_unicodes | hb_filter (cmap12_unicodes);
      स्वतः sibling = + subset_unicodes | hb_filter (*sibling_unicodes);
      लागि (; cmap12 && sibling; cmap12++, sibling++)
      {
        अचिन्हित a = *cmap12;
        अचिन्हित b = *sibling;
        यदि (a != b) फर्काऊ असत्य;
      }

      फर्काऊ !cmap12 && !sibling;
    }

    फर्काऊ असत्य;
  }

  शून्य closure_glyphs (अचर hb_set_t      *unicodes,
		       hb_set_t            *glyphset) अचर
  {
    + hb_iter (encodingRecord)
    | hb_map (&EncodingRecord::subtable)
    | hb_map (hb_add (एतत्))
    | hb_filter (&CmapSubtable::is_variation_selector)
    | hb_apply ([=] (अचर CmapSubtable& _) { _.closure_glyphs (unicodes, glyphset); })
    ;
  }

  तार्किक subset (hb_subset_context_t *c) अचर
  { फर्काऊ subset (c, असत्य); }

  तार्किक subset (hb_subset_context_t *c, तार्किक is_dmap) अचर
  {
    TRACE_SUBSET (एतत्);

    cmap *cmap_अविभाज्य = c->serializer->start_embed<cmap> ();

    स्वतः encodingrec_iter =
    + hb_iter (encodingRecord)
    | hb_filter ([&](अचर EncodingRecord& _) {
      फर्काऊ cmap::filter_encoding_records_for_subset (एतत्, _);
    })
    ;

    यदि (unlikely (!encodingrec_iter.len ())) return_trace (असत्य);

    अचर EncodingRecord *unicode_bmp= nullptr, *unicode_ucs4 = nullptr, *ms_bmp = nullptr, *ms_ucs4 = nullptr;
    तार्किक has_format12 = असत्य;

    लागि (अचर EncodingRecord& _ : encodingrec_iter)
    {
      अचिन्हित format = (एतत् + _.subtable).u.format.v;
      यदि (format == 12) has_format12 = सत्य;

      अचर EncodingRecord *table = std::addressof (_);
      यदि      (_.platformID == 0 && _.encodingID ==  3) unicode_bmp = table;
      अन्यथा यदि (_.platformID == 0 && _.encodingID ==  4) unicode_ucs4 = table;
      अन्यथा यदि (_.platformID == 3 && _.encodingID ==  1) ms_bmp = table;
      अन्यथा यदि (_.platformID == 3 && _.encodingID == 10) ms_ucs4 = table;
    }

    यदि (unlikely (!is_dmap && !has_format12 && !unicode_bmp && !ms_bmp)) return_trace (असत्य);
    यदि (unlikely (!is_dmap && has_format12 && (!unicode_ucs4 && !ms_ucs4))) return_trace (असत्य);

    स्वतः it =
    + c->plan->unicode_to_new_gid_list.iter ()
    | hb_filter ([&] (अचर hb_codepoint_pair_t _)
		 { फर्काऊ (_.second != HB_MAP_मान_INVALID); })
    ;

    return_trace (cmap_अविभाज्य->serialize (c->serializer,
                                         it,
                                         encodingrec_iter,
                                         एतत्,
                                         c->plan,
					 असत्य,
					 c->source_blob->length));
  }

  अचर CmapSubtable *find_best_subtable (तार्किक *चिह्न = nullptr,
					  तार्किक *mac = nullptr,
					  तार्किक *macroman = nullptr) अचर
  {
    यदि (चिह्न) *चिह्न = असत्य;
    यदि (mac) *mac = असत्य;
    यदि (macroman) *macroman = असत्य;

    अचर CmapSubtable *subtable;

    /* Symbol subtable.
     * Prefer symbol if available.
     * https://github.com/harfbuzz/harfbuzz/issues/1918 */
    यदि ((subtable = एतत्->find_subtable (3, 0)))
    {
      यदि (चिह्न) *चिह्न = सत्य;
      फर्काऊ subtable;
    }

    /* 32-bit subtables. */
    यदि ((subtable = एतत्->find_subtable (3, 10))) फर्काऊ subtable;
    यदि ((subtable = एतत्->find_subtable (0, 6))) फर्काऊ subtable;
    यदि ((subtable = एतत्->find_subtable (0, 4))) फर्काऊ subtable;

    /* 16-bit subtables. */
    यदि ((subtable = एतत्->find_subtable (3, 1))) फर्काऊ subtable;
    यदि ((subtable = एतत्->find_subtable (0, 3))) फर्काऊ subtable;
    यदि ((subtable = एतत्->find_subtable (0, 2))) फर्काऊ subtable;
    यदि ((subtable = एतत्->find_subtable (0, 1))) फर्काऊ subtable;
    यदि ((subtable = एतत्->find_subtable (0, 0))) फर्काऊ subtable;

    /* MacRoman subtable. */
    यदि ((subtable = एतत्->find_subtable (1, 0)))
    {
      यदि (mac) *mac = सत्य;
      यदि (macroman) *macroman = सत्य;
      फर्काऊ subtable;
    }
    /* Any other Mac subtable; we just map ASCII for these. */
    यदि ((subtable = एतत्->find_subtable (1, 0xFFFF)))
    {
      यदि (mac) *mac = सत्य;
      फर्काऊ subtable;
    }

    /* Meh. */
    फर्काऊ &Null (CmapSubtable);
  }

  संरचना accelerator_t
  {
    using cache_t = hb_cache_t<21, 19>;
    static_assert (परिमाण (cache_t) == 1024, "");

    accelerator_t (hb_face_t *face, hb_tag_t table_tag = cmap::tableTag)
    {
      एतत्->table = hb_sanitize_context_t ().reference_table<cmap> (face, table_tag);
      यदि (!एतत्->table.get_length ())
	फर्काऊ;

      तार्किक चिह्न, mac, macroman;
      एतत्->subtable = table->find_best_subtable (&चिह्न, &mac, &macroman);
      एतत्->subtable_uvs = &Null (CmapSubtable);
      {
	अचर CmapSubtable *st = table->find_variation_selector_subtable ();
	यदि (st)
	  subtable_uvs = st;
      }

#ifndef HB_NO_OT_FONT_CMAP_CACHE
      cache = (cache_t *) hb_malloc (परिमाण (cache_t));
      यदि (cache)
	नूतन (cache) cache_t ();
      अन्यथा
        फर्काऊ; // Such that get_glyph_funcZ remains null.
#endif

      एतत्->get_glyph_data = subtable;
#ifndef HB_NO_CMAP_LEGACY_SUBTABLES
      तार्किक is_format4 = subtable->u.format.v == 4;
      स्वतः set_format4_getter = [एतत्] (तार्किक (*func) (अचर शून्य *,
						      hb_codepoint_t,
						      hb_codepoint_t *))
      {
	एतत्->format4_accel.init (&एतत्->subtable->u.format4,
				  get_subtable_data_size (एतत्->subtable));
	एतत्->get_glyph_data = &एतत्->format4_accel;
	एतत्->get_glyph_funcZ = func;
      };
      यदि (unlikely (चिह्न))
      {
	विकल्प ((अचिन्हित) face->table.OS2->get_font_page ()) {
	पक्ष OS2::font_page_t::FONT_PAGE_NONE:
	  यदि (is_format4)
	    set_format4_getter (get_glyph_from_चिह्न<CmapSubtableFormat4::accelerator_t, hb_चिह्न_pua_map>);
	  अन्यथा
	    एतत्->get_glyph_funcZ = get_glyph_from_चिह्न<CmapSubtable, hb_चिह्न_pua_map>;
	  रोक;
#ifndef HB_NO_OT_SHAPER_ARABIC_FALLBACK
	पक्ष OS2::font_page_t::FONT_PAGE_SIMP_ARABIC:
	  यदि (is_format4)
	    set_format4_getter (get_glyph_from_चिह्न<CmapSubtableFormat4::accelerator_t, _hb_arabic_pua_simp_map>);
	  अन्यथा
	    एतत्->get_glyph_funcZ = get_glyph_from_चिह्न<CmapSubtable, _hb_arabic_pua_simp_map>;
	  रोक;
	पक्ष OS2::font_page_t::FONT_PAGE_TRAD_ARABIC:
	  यदि (is_format4)
	    set_format4_getter (get_glyph_from_चिह्न<CmapSubtableFormat4::accelerator_t, _hb_arabic_pua_trad_map>);
	  अन्यथा
	    एतत्->get_glyph_funcZ = get_glyph_from_चिह्न<CmapSubtable, _hb_arabic_pua_trad_map>;
	  रोक;
#endif
	स्वभाव:
	  यदि (is_format4)
	    set_format4_getter (get_glyph_from<CmapSubtableFormat4::accelerator_t>);
	  अन्यथा
	    एतत्->get_glyph_funcZ = get_glyph_from<CmapSubtable>;
	  रोक;
	}
      }
      अन्यथा यदि (unlikely (macroman))
      {
	यदि (is_format4)
	  set_format4_getter (get_glyph_from_macroman<CmapSubtableFormat4::accelerator_t>);
	अन्यथा
	  एतत्->get_glyph_funcZ = get_glyph_from_macroman<CmapSubtable>;
      }
      अन्यथा यदि (unlikely (mac))
      {
	यदि (is_format4)
	  set_format4_getter (get_glyph_from_ascii<CmapSubtableFormat4::accelerator_t>);
	अन्यथा
	  एतत्->get_glyph_funcZ = get_glyph_from_ascii<CmapSubtable>;
      }
      अन्यथा
#endif
      {
	विकल्प (subtable->u.format.v) {
	  /* Accelerate format 4 and format 12. */
	  स्वभाव:
	    एतत्->get_glyph_funcZ = get_glyph_from<CmapSubtable>;
	    रोक;
	  पक्ष 12:
	    एतत्->get_glyph_funcZ = get_glyph_from<CmapSubtableFormat12>;
	    रोक;
	  पक्ष  4:
	  {
	    hb_barrier ();
	    एतत्->format4_accel.init (&subtable->u.format4,
				      get_subtable_data_size (subtable));
	    एतत्->get_glyph_data = &एतत्->format4_accel;
	    एतत्->get_glyph_funcZ = एतत्->format4_accel.get_glyph_func;
	    रोक;
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
			     hb_codepoint_t *glyph) अचर
    {
#ifndef HB_NO_OT_FONT_CMAP_CACHE
      // cache is always non-null if we have a get_glyph_funcZ
      अचिन्हित v;
      यदि (cache->get (unicode, &v))
      {
        *glyph = v;
	फर्काऊ सत्य;
      }
#endif
      तार्किक ret  = एतत्->get_glyph_funcZ (एतत्->get_glyph_data, unicode, glyph);

#ifndef HB_NO_OT_FONT_CMAP_CACHE
      यदि (ret)
        cache->set (unicode, *glyph);
#endif

      फर्काऊ ret;
    }

    तार्किक get_nominal_glyph (hb_codepoint_t  unicode,
			    hb_codepoint_t *glyph) अचर
    {
      यदि (unlikely (!एतत्->get_glyph_funcZ)) फर्काऊ असत्य;
      फर्काऊ _cached_get (unicode, glyph);
    }

    अचिन्हित पूर्णाङ्क get_nominal_glyphs (अचिन्हित पूर्णाङ्क गन्ती,
				     अचर hb_codepoint_t *first_unicode,
				     अचिन्हित पूर्णाङ्क unicode_stride,
				     hb_codepoint_t *first_glyph,
				     अचिन्हित पूर्णाङ्क glyph_stride) अचर
    {
      यदि (unlikely (!एतत्->get_glyph_funcZ)) फर्काऊ 0;

      अचिन्हित पूर्णाङ्क done;
      लागि (done = 0;
	   done < गन्ती && _cached_get (*first_unicode, first_glyph);
	   done++)
      {
	first_unicode = &StructAtOffsetUnaligned<hb_codepoint_t> (first_unicode, unicode_stride);
	first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      }
      फर्काऊ done;
    }

    तार्किक get_variation_glyph (hb_codepoint_t  unicode,
			      hb_codepoint_t  variation_selector,
			      hb_codepoint_t *glyph) अचर
    {
      विकल्प (get_glyph_variant (unicode, variation_selector, glyph))
      {
	पक्ष GLYPH_VARIANT_NOT_FOUND:	फर्काऊ असत्य;
	पक्ष GLYPH_VARIANT_FOUND:	फर्काऊ सत्य;
	पक्ष GLYPH_VARIANT_USE_DEFAULT:	रोक;
      }

      फर्काऊ get_nominal_glyph (unicode, glyph);
    }

    glyph_variant_t get_glyph_variant (hb_codepoint_t  unicode,
				       hb_codepoint_t  variation_selector,
				       hb_codepoint_t *glyph) अचर
    {
      फर्काऊ एतत्->subtable_uvs->get_glyph_variant (unicode,
						    variation_selector,
						    glyph);
    }

    तार्किक has_data () अचर { फर्काऊ table.get_length (); }

    शून्य collect_unicodes (hb_set_t *out, अचिन्हित पूर्णाङ्क अङ्क_glyphs) अचर
    { subtable->collect_unicodes (out, अङ्क_glyphs, get_subtable_data_size (subtable)); }
    शून्य collect_mapping (hb_set_t *unicodes, hb_map_t *mapping,
			  अचिन्हित अङ्क_glyphs = अच_पूर्णांक_उच्च) अचर
    { subtable->collect_mapping (unicodes, mapping, अङ्क_glyphs, get_subtable_data_size (subtable)); }
    शून्य collect_variation_selectors (hb_set_t *out) अचर
    { subtable_uvs->collect_variation_selectors (out); }
    शून्य collect_variation_unicodes (hb_codepoint_t variation_selector,
				     hb_set_t *out) अचर
    { subtable_uvs->collect_variation_unicodes (variation_selector, out); }

    रक्षित:
    प्रकारनिर्देश तार्किक (*hb_cmap_get_glyph_func_t) (अचर शून्य *obj,
					      hb_codepoint_t codepoint,
					      hb_codepoint_t *glyph);
    प्रकारनिर्देश uint16_t (*hb_pua_remap_func_t) (अचिन्हित);

    ढाँचा <typename Type>
    HB_INTERNAL स्थिर तार्किक get_glyph_from (अचर शून्य *obj,
					    hb_codepoint_t codepoint,
					    hb_codepoint_t *glyph)
    {
      अचर Type *typed_obj = (अचर Type *) obj;
      फर्काऊ typed_obj->get_glyph (codepoint, glyph);
    }

    ढाँचा <typename Type, hb_pua_remap_func_t remap>
    HB_INTERNAL स्थिर तार्किक get_glyph_from_चिह्न (अचर शून्य *obj,
						   hb_codepoint_t codepoint,
						   hb_codepoint_t *glyph)
    {
      अचर Type *typed_obj = (अचर Type *) obj;
      यदि (likely (typed_obj->get_glyph (codepoint, glyph)))
	फर्काऊ सत्य;

      यदि (hb_codepoint_t c = remap (codepoint))
	फर्काऊ typed_obj->get_glyph (c, glyph);

      फर्काऊ असत्य;
    }

    ढाँचा <typename Type>
    HB_INTERNAL स्थिर तार्किक get_glyph_from_ascii (अचर शून्य *obj,
						  hb_codepoint_t codepoint,
						  hb_codepoint_t *glyph)
    {
      अचर Type *typed_obj = (अचर Type *) obj;
      फर्काऊ codepoint < 0x80 && typed_obj->get_glyph (codepoint, glyph);
    }

    ढाँचा <typename Type>
    HB_INTERNAL स्थिर तार्किक get_glyph_from_macroman (अचर शून्य *obj,
						     hb_codepoint_t codepoint,
						     hb_codepoint_t *glyph)
    {
      यदि (get_glyph_from_ascii<Type> (obj, codepoint, glyph))
	फर्काऊ सत्य;

      अचर Type *typed_obj = (अचर Type *) obj;
      अचिन्हित c = unicode_to_macroman (codepoint);
      फर्काऊ c && typed_obj->get_glyph (c, glyph);
    }

    अचिन्हित पूर्णाङ्क get_subtable_data_size (अचर CmapSubtable *subtable) अचर
    {
      अचिन्हित पूर्णाङ्क table_length = एतत्->table.get_length ();
      uintptr_t table_start = (uintptr_t) (अचर शून्य *) एतत्->table.get ();
      uintptr_t subtable_addr = (uintptr_t) (अचर शून्य *) subtable;
      यदि (unlikely (subtable_addr < table_start))
	फर्काऊ 0;

      uintptr_t subtable_offset = subtable_addr - table_start;
      यदि (unlikely (subtable_offset >= table_length))
	फर्काऊ 0;

      फर्काऊ table_length - (अचिन्हित पूर्णाङ्क) subtable_offset;
    }

    गुप्त:
    hb_nonnull_ptr_t<अचर CmapSubtable> subtable;
    hb_nonnull_ptr_t<अचर CmapSubtable> subtable_uvs;

    hb_cmap_get_glyph_func_t get_glyph_funcZ = nullptr;
    अचर शून्य *get_glyph_data = nullptr;

    CmapSubtableFormat4::accelerator_t format4_accel;

#ifndef HB_NO_OT_FONT_CMAP_CACHE
    cache_t *cache = nullptr;
#endif

    सार्वजनिक:
    hb_blob_ptr_t<cmap> table;
  };

  रक्षित:

  अचर CmapSubtable *find_subtable (अचिन्हित पूर्णाङ्क platform_id,
				     अचिन्हित पूर्णाङ्क encoding_id) अचर
  {
    EncodingRecord key;
    key.platformID = platform_id;
    key.encodingID = encoding_id;

    अचर EncodingRecord &result = encodingRecord.द्वा_खोज (key);
    यदि (!result.subtable)
      फर्काऊ nullptr;

    फर्काऊ &(एतत्+result.subtable);
  }

  अचर CmapSubtable *find_variation_selector_subtable () अचर
  {
    अचर CmapSubtable *format14 = nullptr;
    लागि (अचर EncodingRecord& record : encodingRecord)
    {
      यदि (record.platformID != 0 || record.encodingID != 5)
	जारी;

      अचर CmapSubtable *subtable = &(एतत्+record.subtable);
#ifndef HB_NO_BEYOND_64K
      यदि (subtable->u.format.v == 15)
	फर्काऊ subtable;
#endif
      यदि (subtable->u.format.v == 14)
	format14 = subtable;
    }
    फर्काऊ format14;
  }

  सार्वजनिक:

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
		  hb_barrier () &&
		  likely (version == 0) &&
		  encodingRecord.sanitize (c, एतत्));
  }

 गुप्त:

  स्थिर तार्किक filter_encoding_records_for_subset(अचर cmap* cmap,
                                                 अचर EncodingRecord& _)
  {
    फर्काऊ
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

  तार्किक subset (hb_subset_context_t *c) अचर
  { फर्काऊ cmap::subset (c, सत्य); }
};

संरचना cmap_accelerator_t
{
  cmap_accelerator_t (hb_face_t *face) :
    dmap (face, HB_OT_TAG_DMAP),
    base (face)
  {}

  तार्किक get_nominal_glyph (hb_codepoint_t  unicode,
			  hb_codepoint_t *glyph) अचर
  {
    फर्काऊ dmap.get_nominal_glyph (unicode, glyph) ||
	   base.get_nominal_glyph (unicode, glyph);
  }

  अचिन्हित पूर्णाङ्क get_nominal_glyphs (अचिन्हित पूर्णाङ्क गन्ती,
				   अचर hb_codepoint_t *first_unicode,
				   अचिन्हित पूर्णाङ्क unicode_stride,
				   hb_codepoint_t *first_glyph,
				   अचिन्हित पूर्णाङ्क glyph_stride) अचर
  {
    यदि (likely (!dmap.has_data ()))
      फर्काऊ base.get_nominal_glyphs (गन्ती,
				      first_unicode, unicode_stride,
				      first_glyph, glyph_stride);

    अचिन्हित पूर्णाङ्क done;
    लागि (done = 0;
	 done < गन्ती && get_nominal_glyph (*first_unicode, first_glyph);
	 done++)
    {
      first_unicode = &StructAtOffsetUnaligned<hb_codepoint_t> (first_unicode, unicode_stride);
      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
    }
    फर्काऊ done;
  }

  तार्किक get_variation_glyph (hb_codepoint_t  unicode,
			    hb_codepoint_t  variation_selector,
			    hb_codepoint_t *glyph) अचर
  {
    glyph_variant_t result = dmap.get_glyph_variant (unicode,
						     variation_selector,
						     glyph);
    यदि (result == GLYPH_VARIANT_NOT_FOUND)
      result = base.get_glyph_variant (unicode, variation_selector, glyph);

    विकल्प (result)
    {
      पक्ष GLYPH_VARIANT_NOT_FOUND: फर्काऊ असत्य;
      पक्ष GLYPH_VARIANT_FOUND: फर्काऊ सत्य;
      पक्ष GLYPH_VARIANT_USE_DEFAULT: रोक;
    }

    फर्काऊ get_nominal_glyph (unicode, glyph);
  }

  शून्य collect_unicodes (hb_set_t *out, अचिन्हित पूर्णाङ्क अङ्क_glyphs) अचर
  {
    base.collect_unicodes (out, अङ्क_glyphs);
    dmap.collect_unicodes (out, अङ्क_glyphs);
  }

  शून्य collect_mapping (hb_set_t *unicodes, hb_map_t *mapping,
			अचिन्हित अङ्क_glyphs = अच_पूर्णांक_उच्च) अचर
  {
    base.collect_mapping (unicodes, mapping, अङ्क_glyphs);
    dmap.collect_mapping (unicodes, mapping, अङ्क_glyphs);
  }

  शून्य collect_variation_selectors (hb_set_t *out) अचर
  {
    base.collect_variation_selectors (out);
    dmap.collect_variation_selectors (out);
  }

  शून्य collect_variation_unicodes (hb_codepoint_t variation_selector,
				   hb_set_t *out) अचर
  {
    base.collect_variation_unicodes (variation_selector, out);
    dmap.collect_variation_unicodes (variation_selector, out);
  }

  शून्य closure_glyphs (अचर hb_set_t *unicodes,
		       hb_set_t       *glyphset) अचर
  {
    यदि (likely (!dmap.has_data ()))
    {
      base.table->closure_glyphs (unicodes, glyphset);
      फर्काऊ;
    }

    hb_set_t variation_selectors;
    collect_variation_selectors (&variation_selectors);
    लागि (hb_codepoint_t variation_selector : variation_selectors)
    {
      यदि (!unicodes->has (variation_selector))
	जारी;

      hb_set_t variation_unicodes;
      collect_variation_unicodes (variation_selector, &variation_unicodes);
      लागि (hb_codepoint_t unicode : variation_unicodes)
      {
	यदि (!unicodes->has (unicode))
	  जारी;

	hb_codepoint_t glyph;
	यदि (get_variation_glyph (unicode, variation_selector, &glyph))
	  glyphset->add (glyph);
      }
    }
  }

  cmap::accelerator_t dmap;
  cmap::accelerator_t base;
};

} /* namespace OT */


#endif /* HB_OT_CMAP_TABLE_HH */
