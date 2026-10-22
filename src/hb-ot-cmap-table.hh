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

स्थिर ओळीत uint8_t unicode_to_macroman (hb_codepoint_t u)
{
  स्थिर स्थिरांक संरचना unicode_to_macroman_t
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
  स्वतः *c = hb_bsearch (u, mapping, ARRAY_LENGTH (mapping), आकार (mapping[0]),
			_hb_cmp_operator<uint16_t, uint16_t>);
  परत c ? c->macroman : 0;
}

संरचना CmapSubtableFormat0
{
  तार्किक get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) स्थिरांक
  {
    hb_codepoint_t gid = codepoint < 256 ? glyphIdArray[codepoint] : 0;
    जर (unlikely (!gid))
      परत असत्य;
    *glyph = gid;
    परत सत्य;
  }

  अचिन्हित get_language () स्थिरांक
  {
    परत language;
  }

  शून्य collect_unicodes (hb_set_t *out) स्थिरांक
  {
    साठी (अचिन्हित पूर्णांक i = 0; i < 256; i++)
      जर (glyphIdArray[i])
	out->add (i);
  }

  शून्य collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping /* OUT */) स्थिरांक
  {
    साठी (अचिन्हित i = 0; i < 256; i++)
      जर (glyphIdArray[i])
      {
	hb_codepoint_t glyph = glyphIdArray[i];
	unicodes->add (i);
	mapping->set (i, glyph);
      }
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे));
  }

  संरक्षित:
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
    पूर्णांक run_length = 0 , delta = 0, prev_delta = 0;

    गणना {
      FIRST_SUB_RANGE,
      FOLLOWING_SUB_RANGE,
    } mode;

    पर्यंत (it) {
      // Start a new range
      {
        स्थिरांक स्वतः& pair = *it;
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

      पर्यंत (it) {
        // Process range
        स्थिरांक स्वतः& pair = *it;
        hb_codepoint_t next_cp = pair.first;
        hb_codepoint_t next_gid = pair.second;
        जर (next_cp != end_cp + 1) {
          // Current range is over, stop processing.
          थांबा;
        }

        जर (next_gid == last_gid + 1) {
          // The current run continues.
          end_cp = next_cp;
          run_length++;
          last_gid = next_gid;
          it++;
          सुरूठेवा;
        }

        // A new run is starting, decide if we want to commit the current run.
        पूर्णांक split_cost = (mode == FIRST_SUB_RANGE) ? 8 : 16;
        पूर्णांक run_cost = run_length * 2;
        जर (run_cost >= split_cost) {
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

    जर (likely (end_cp != 0xFFFF)) {
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
                             पूर्णांक run_delta,
                             पूर्णांक previous_run_delta,
                             पूर्णांक split_cost,
                             Writer& range_writer) {
    तार्किक should_split = असत्य;
    जर (start < run_start && run_start < end) {
      पूर्णांक run_cost = (end - run_start + 1) * 2;
      जर (run_cost >= split_cost) {
        should_split = सत्य;
      }
    }

    // A zero delta is also the marker for ranges that need a
    // glyphIdArray; serialize_rangeoffset_glyid() tells the two cases
    // apart by re-checking the mapping.
    जर (should_split) {
      जर (start == prev_run_start)
        range_writer (start, run_start - 1, previous_run_delta);
      अन्यथा
        range_writer (start, run_start - 1, 0);
      range_writer (run_start, end, run_delta);
      परत;
    }


    जर (start == run_start) {
      // Range is only a run
      range_writer (start, end, run_delta);
      परत;
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
                       पूर्णांक delta) {
        segcount++;
      }
    } counter;

    to_ranges (+it, counter);
    परत counter.segcount;
  }


  ढाँचा<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  तार्किक serialize_start_end_delta_arrays (hb_serialize_context_t *c,
                                         Iterator it,
                                         पूर्णांक segcount)
  {
    संरचना Writer {
      hb_serialize_context_t *serializer_;
      HBUINT16* end_code_;
      HBUINT16* start_code_;
      HBINT16* id_delta_;
      पूर्णांक index_;

      Writer(hb_serialize_context_t *serializer)
          : serializer_(serializer),
            end_code_(nullptr),
            start_code_(nullptr),
            id_delta_(nullptr),
            index_ (0) {}
      शून्य चालक() (hb_codepoint_t start,
                       hb_codepoint_t end,
                       पूर्णांक delta) {
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

    जर (unlikely (!writer.end_code_ || !writer.start_code_ || !writer.id_delta_)) परत असत्य;

    to_ranges (+it, writer);
    परत सत्य;
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
    जर (unlikely (!c->check_success (idRangeOffset))) परत nullptr;
    जर (unlikely ((अक्षर *)idRangeOffset - (अक्षर *)idDelta != (पूर्णांक) segcount * (पूर्णांक) HBINT16::static_size)) परत nullptr;

    साठी (अचिन्हित i : hb_range (segcount))
    {
      /* A non-zero delta is always a real run. A zero delta is
       * ambiguous: identity mapping, or the marker for a range that
       * needs a glyphIdArray. Only zero deltas need a check. */
      पूर्णांक delta = idDelta[i];
      जर (delta)
	सुरूठेवा;
      तार्किक use_delta = सत्य;
      साठी (hb_codepoint_t cp = startCode[i]; cp <= endCode[i]; cp++)
      {
	hb_codepoint_t gid = cp_to_gid.get (cp);
	जर (gid == HB_MAP_मूल्य_INVALID)
	  gid = 0;
	जर (gid != cp)
	{
	  use_delta = असत्य;
	  थांबा;
	}
      }
      जर (use_delta)
	सुरूठेवा;

      idRangeOffset[i] = 2 * (c->start_embed<HBUINT16> () - idRangeOffset - i);
      साठी (hb_codepoint_t cp = startCode[i]; cp <= endCode[i]; cp++)
      {
        HBUINT16 gid;
        gid = cp_to_gid[cp];
        c->copy<HBUINT16> (gid);
      }
    }

    परत idRangeOffset;
  }

  ढाँचा<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  शून्य serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    स्वतः format4_iter =
    + it
    | hb_filter ([&] (स्थिरांक hb_codepoint_pair_t _)
		 { परत _.first <= 0xFFFF; })
    ;

    जर (!format4_iter) परत;

    अचिन्हित table_initpos = c->length ();
    जर (unlikely (!c->extend_min (हे))) परत;
    हे->format = 4;

    hb_vector_t<hb_codepoint_pair_t> cp_to_gid {
      format4_iter
    };

    //serialize endCode[], startCode[], idDelta[]
    HBUINT16* endCode = c->start_embed<HBUINT16> ();
    अचिन्हित segcount = serialize_find_segcount (cp_to_gid.iter());
    जर (unlikely (!serialize_start_end_delta_arrays (c, cp_to_gid.iter(), segcount)))
      परत;

    HBUINT16 *startCode = endCode + segcount + 1;
    HBINT16 *idDelta = ((HBINT16*)startCode) + segcount;

    HBUINT16 *idRangeOffset = serialize_rangeoffset_glyid (c,
                                                           cp_to_gid.iter (),
                                                           endCode,
                                                           startCode,
                                                           idDelta,
                                                           segcount);
    जर (unlikely (!c->check_success (idRangeOffset))) परत;

    हे->length = c->length () - table_initpos;
    जर ((दीर्घ दीर्घ) हे->length != (दीर्घ दीर्घ) c->length () - table_initpos)
    {
      c->err (HB_SERIALIZE_ERROR_INT_OVERFLOW);
      परत;
    }

    हे->seg_मोजणी_X2 = segcount * 2;
    हे->entrySelector = hb_max (1u, hb_bit_storage (segcount)) - 1;
    हे->शोध_Range = 2 * (1u << हे->entrySelector);
    हे->rangeShift = segcount * 2 > हे->शोध_Range
		       ? 2 * segcount - हे->शोध_Range
		       : 0;
  }

  अचिन्हित get_language () स्थिरांक
  {
    परत language;
  }

  संरचना accelerator_t
  {
    accelerator_t () {}
    accelerator_t (स्थिरांक CmapSubtableFormat4 *subtable) = delete;

    शून्य init (स्थिरांक CmapSubtableFormat4 *subtable,
	       अचिन्हित पूर्णांक              subtable_data_size)
    {
      seg_मोजणी_2 = subtable->seg_मोजणी_X2 / 2;
      end_मोजणी = subtable->values.arrayZ;
      start_मोजणी = end_मोजणी + seg_मोजणी_2 + 1;
      idDelta = start_मोजणी + seg_मोजणी_2;
      idRangeOffset = idDelta + seg_मोजणी_2;
      glyphIdArray = idRangeOffset + seg_मोजणी_2;

      अचिन्हित पूर्णांक मूल्ये_offset = 16 + 8 * seg_मोजणी_2;
      glyphIdArrayLength = subtable_data_size > मूल्ये_offset
			   ? (subtable_data_size - मूल्ये_offset) / 2
			   : 0;
    }

    तार्किक get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) स्थिरांक
    {
      संरचना CustomRange
      {
	पूर्णांक cmp (hb_codepoint_t k,
		 अचिन्हित distance) स्थिरांक
	{
	  जर (k > last) परत +1;
	  जर (k < (&last)[distance]/*first*/) परत -1;
	  परत 0;
	}
	HBUINT16 last;
      };

      स्थिरांक HBUINT16 *found = hb_bsearch (codepoint,
					  हे->end_मोजणी,
					  हे->seg_मोजणी_2,
					  आकार (CustomRange),
					  _hb_cmp_method<hb_codepoint_t, CustomRange, अचिन्हित>,
					  हे->seg_मोजणी_2 + 1);
      जर (unlikely (!found))
	परत असत्य;
      अचिन्हित पूर्णांक i = found - end_मोजणी;

      hb_codepoint_t gid;
      अचिन्हित पूर्णांक rangeOffset = हे->idRangeOffset[i];
      जर (rangeOffset == 0)
	gid = codepoint + हे->idDelta[i];
      अन्यथा
      {
	/* Somebody has been smoking... */
	अचिन्हित पूर्णांक index = rangeOffset / 2 + (codepoint - हे->start_मोजणी[i]) + i - हे->seg_मोजणी_2;
	जर (unlikely (index >= हे->glyphIdArrayLength))
	  परत असत्य;
	gid = हे->glyphIdArray[index];
	जर (unlikely (!gid))
	  परत असत्य;
	gid += हे->idDelta[i];
      }
      gid &= 0xFFFFu;
      जर (unlikely (!gid))
	परत असत्य;
      *glyph = gid;
      परत सत्य;
    }

    HB_INTERNAL स्थिर तार्किक get_glyph_func (स्थिरांक शून्य *obj, hb_codepoint_t codepoint, hb_codepoint_t *glyph)
    { परत ((स्थिरांक accelerator_t *) obj)->get_glyph (codepoint, glyph); }

    शून्य collect_unicodes (hb_set_t *out) स्थिरांक
    {
      अचिन्हित पूर्णांक मोजणी = हे->seg_मोजणी_2;
      जर (मोजणी && हे->start_मोजणी[मोजणी - 1] == 0xFFFFu)
	मोजणी--; /* Skip sentinel segment. */
      साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
      {
	hb_codepoint_t start = हे->start_मोजणी[i];
	hb_codepoint_t end = हे->end_मोजणी[i];
	अचिन्हित पूर्णांक rangeOffset = हे->idRangeOffset[i];
        out->add_range(start, end);
	जर (rangeOffset == 0)
	{
	  साठी (hb_codepoint_t codepoint = start; codepoint <= end; codepoint++)
	  {
	    hb_codepoint_t gid = (codepoint + हे->idDelta[i]) & 0xFFFFu;
	    जर (unlikely (!gid))
              out->del(codepoint);
	  }
	}
	अन्यथा
	{
	  साठी (hb_codepoint_t codepoint = start; codepoint <= end; codepoint++)
	  {
	    अचिन्हित पूर्णांक index = rangeOffset / 2 + (codepoint - हे->start_मोजणी[i]) + i - हे->seg_मोजणी_2;
	    जर (unlikely (index >= हे->glyphIdArrayLength))
            {
              out->del_range (codepoint, end);
	      थांबा;
            }
	    hb_codepoint_t gid = हे->glyphIdArray[index];
	    जर (unlikely (!gid))
              out->del(codepoint);
	  }
	}
      }
    }

    शून्य collect_mapping (hb_set_t *unicodes, /* OUT */
			  hb_map_t *mapping /* OUT */) स्थिरांक
    {
      // TODO(grieger): optimize similar to collect_unicodes
      // (ie. use add_range())
      अचिन्हित मोजणी = हे->seg_मोजणी_2;
      जर (मोजणी && हे->start_मोजणी[मोजणी - 1] == 0xFFFFu)
	मोजणी--; /* Skip sentinel segment. */
      साठी (अचिन्हित i = 0; i < मोजणी; i++)
      {
	hb_codepoint_t start = हे->start_मोजणी[i];
	hb_codepoint_t end = हे->end_मोजणी[i];
	अचिन्हित rangeOffset = हे->idRangeOffset[i];
	जर (rangeOffset == 0)
	{
	  साठी (hb_codepoint_t codepoint = start; codepoint <= end; codepoint++)
	  {
	    hb_codepoint_t gid = (codepoint + हे->idDelta[i]) & 0xFFFFu;
	    जर (unlikely (!gid))
	      सुरूठेवा;
	    unicodes->add (codepoint);
	    mapping->set (codepoint, gid);
	  }
	}
	अन्यथा
	{
	  साठी (hb_codepoint_t codepoint = start; codepoint <= end; codepoint++)
	  {
	    अचिन्हित index = rangeOffset / 2 + (codepoint - हे->start_मोजणी[i]) + i - हे->seg_मोजणी_2;
	    जर (unlikely (index >= हे->glyphIdArrayLength))
	      थांबा;
	    hb_codepoint_t gid = हे->glyphIdArray[index];
	    जर (unlikely (!gid))
	      सुरूठेवा;
            gid = (gid + हे->idDelta[i]) & 0xFFFFu;
	    unicodes->add (codepoint);
	    mapping->set (codepoint, gid);
	  }
	}
      }
    }

    स्थिरांक HBUINT16 *end_मोजणी;
    स्थिरांक HBUINT16 *start_मोजणी;
    स्थिरांक HBUINT16 *idDelta;
    स्थिरांक HBUINT16 *idRangeOffset;
    स्थिरांक HBUINT16 *glyphIdArray;
    अचिन्हित पूर्णांक seg_मोजणी_2;
    अचिन्हित पूर्णांक glyphIdArrayLength;
  };

  तार्किक get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph,
		  अचिन्हित पूर्णांक subtable_data_size) स्थिरांक
  {
    accelerator_t accel;
    accel.init (हे, subtable_data_size);
    परत accel.get_glyph_func (&accel, codepoint, glyph);
  }
  तार्किक get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) स्थिरांक
  { परत असत्य; }

  शून्य collect_unicodes (hb_set_t *out, अचिन्हित पूर्णांक subtable_data_size) स्थिरांक
  {
    accelerator_t accel;
    accel.init (हे, subtable_data_size);
    accel.collect_unicodes (out);
  }
  शून्य collect_unicodes (hb_set_t *out) स्थिरांक
  { collect_unicodes (out, length); }

  शून्य collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping, /* OUT */
			अचिन्हित पूर्णांक subtable_data_size) स्थिरांक
  {
    accelerator_t accel;
    accel.init (हे, subtable_data_size);
    accel.collect_mapping (unicodes, mapping);
  }
  शून्य collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping /* OUT */) स्थिरांक
  { collect_mapping (unicodes, mapping, length); }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    जर (unlikely (!c->check_struct (हे)))
      return_trace (असत्य);
    hb_barrier ();

    return_trace (c->check_range (values, 2 + 4 * seg_मोजणी_X2));
  }

  संरक्षित:
  HBUINT16	format;		/* Format number is set to 4. */
  HBUINT16	length;		/* This is the length in bytes of the
				 * subtable. */
  HBUINT16	language;	/* Ignore. */
  HBUINT16	seg_मोजणी_X2;	/* 2 x segCount. */
  HBUINT16	शोध_Range;	/* 2 * (2**floor(log2(segCount))) */
  HBUINT16	entrySelector;	/* log2(searchRange/2) */
  HBUINT16	rangeShift;	/* 2 x segCount - searchRange */

  UnsizedArrayOf<HBUINT16>
		values;
#जर 0
  HBUINT16	end_मोजणी[seg_मोजणी_2];	/* End characterCode for each segment,
					 * last=0xFFFFu. */
  HBUINT16	reservedPad;		/* Set to 0. */
  HBUINT16	start_मोजणी[seg_मोजणी_2];	/* Start character code for each segment. */
  HBINT16		idDelta[seg_मोजणी_2];	/* Delta for all character codes in segment. */
  HBUINT16	idRangeOffset[seg_मोजणी_2];/* Offsets into glyphIdArray or 0 */
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

  पूर्णांक cmp (hb_codepoint_t codepoint) स्थिरांक
  {
    जर (codepoint < startCharCode) परत -1;
    जर (codepoint > endCharCode)   परत +1;
    परत 0;
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे));
  }

  खाजगी:
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
  तार्किक get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) स्थिरांक
  {
    /* Rely on our implicit array bound-checking. */
    hb_codepoint_t gid = glyphIdArray[codepoint - startCharCode];
    जर (unlikely (!gid))
      परत असत्य;
    *glyph = gid;
    परत सत्य;
  }

  अचिन्हित get_language () स्थिरांक
  {
    परत language;
  }

  शून्य collect_unicodes (hb_set_t *out) स्थिरांक
  {
    hb_codepoint_t start = startCharCode;
    अचिन्हित पूर्णांक मोजणी = glyphIdArray.len;
    साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
      जर (glyphIdArray[i])
	out->add (start + i);
  }

  शून्य collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping /* OUT */) स्थिरांक
  {
    hb_codepoint_t start_cp = startCharCode;
    अचिन्हित मोजणी = glyphIdArray.len;
    साठी (अचिन्हित i = 0; i < मोजणी; i++)
      जर (glyphIdArray[i])
      {
	hb_codepoint_t unicode = start_cp + i;
	hb_codepoint_t glyphid = glyphIdArray[i];
	unicodes->add (unicode);
	mapping->set (unicode, glyphid);
      }
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) && glyphIdArray.sanitize (c));
  }

  संरक्षित:
  UINT		formatReserved;	/* Subtable format and (maybe) padding. */
  UINT		length;		/* Byte length of this subtable. */
  UINT		language;	/* Ignore. */
  UINT		startCharCode;	/* First character code covered. */
  ArrayOf<HBGlyphID16, UINT>
		glyphIdArray;	/* Array of glyph index values for character
				 * codes in the range. */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (5 * आकार (UINT), glyphIdArray);
};

संरचना CmapSubtableFormat6  : CmapSubtableTrimmed<HBUINT16> {};
संरचना CmapSubtableFormat10 : CmapSubtableTrimmed<HBUINT32> {};

ढाँचा <typename T>
संरचना CmapSubtableLongSegmented
{
  मित्र संरचना cmap;

  तार्किक get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) स्थिरांक
  {
    hb_codepoint_t gid = T::group_get_glyph (groups.द्वा_खोज (codepoint), codepoint);
    जर (unlikely (!gid))
      परत असत्य;
    *glyph = gid;
    परत सत्य;
  }

  अचिन्हित get_language () स्थिरांक
  {
    परत language;
  }

  शून्य collect_unicodes (hb_set_t *out, अचिन्हित पूर्णांक अंक_glyphs) स्थिरांक
  {
    साठी (अचिन्हित पूर्णांक i = 0; i < हे->groups.len; i++)
    {
      hb_codepoint_t start = हे->groups[i].startCharCode;
      hb_codepoint_t end = hb_min ((hb_codepoint_t) हे->groups[i].endCharCode,
				   (hb_codepoint_t) HB_UNICODE_MAX);
      hb_codepoint_t gid = हे->groups[i].glyphID;
      जर (!gid)
      {
	/* Intention is: if (hb_is_same (T, CmapSubtableFormat13)) continue; */
	जर (! T::group_get_glyph (हे->groups[i], end)) सुरूठेवा;
	start++;
	gid++;
      }
      जर (unlikely ((अचिन्हित पूर्णांक) gid >= अंक_glyphs)) सुरूठेवा;
      जर (unlikely ((अचिन्हित पूर्णांक) (gid + end - start) >= अंक_glyphs))
	end = start + (hb_codepoint_t) अंक_glyphs - gid;

      out->add_range (start, hb_min (end, 0x10FFFFu));
    }
  }

  शून्य collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping, /* OUT */
			अचिन्हित अंक_glyphs) स्थिरांक
  {
    hb_codepoint_t last_end = 0;
    अचिन्हित मोजणी = हे->groups.len;
    साठी (अचिन्हित i = 0; i < मोजणी; i++)
    {
      hb_codepoint_t start = हे->groups.arrayZ[i].startCharCode;
      hb_codepoint_t end = hb_min ((hb_codepoint_t) हे->groups.arrayZ[i].endCharCode,
				   (hb_codepoint_t) HB_UNICODE_MAX);
      जर (unlikely (start > end || start < last_end)) {
        // Range is not in order and is invalid, skip it.
        सुरूठेवा;
      }
      last_end = end;


      hb_codepoint_t gid = हे->groups.arrayZ[i].glyphID;
      जर (!gid)
      {
        जर (T::format_संख्या == 13) सुरूठेवा;
	start++;
	gid++;
      }
      जर (unlikely ((अचिन्हित पूर्णांक) gid >= अंक_glyphs)) सुरूठेवा;
      जर (unlikely ((अचिन्हित पूर्णांक) (gid + end - start) >= अंक_glyphs))
	end = start + (hb_codepoint_t) अंक_glyphs - gid;

      mapping->alloc (mapping->get_population () + end - start + 1);

      unicodes->add_range (start, end);
      साठी (अचिन्हित cp = start; cp <= end; cp++)
      {
	mapping->set (cp, gid);
        gid += T::increment;
      }
    }
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) && groups.sanitize (c));
  }

  संरक्षित:
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
  स्थिर constexpr पूर्णांक increment = 1;
  स्थिर constexpr पूर्णांक format_संख्या = 12;

  स्थिर hb_codepoint_t group_get_glyph (स्थिरांक CmapSubtableLongGroup &group,
					 hb_codepoint_t u)
  { परत likely (group.startCharCode <= group.endCharCode) ?
	   group.glyphID + (u - group.startCharCode) : 0; }


  ढाँचा<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  शून्य serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    जर (!it) परत;
    अचिन्हित table_initpos = c->length ();
    जर (unlikely (!c->extend_min (हे))) परत;

    hb_codepoint_t startCharCode = (hb_codepoint_t) -1, endCharCode = (hb_codepoint_t) -1;
    hb_codepoint_t glyphID = 0;

    साठी (स्थिरांक स्वतः& _ : +it)
    {
      जर (startCharCode == (hb_codepoint_t) -1)
      {
	startCharCode = _.first;
	endCharCode = _.first;
	glyphID = _.second;
      }
      अन्यथा जर (!_is_gid_consecutive (endCharCode, startCharCode, glyphID, _.first, _.second))
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

    हे->format = 12;
    हे->reserved = 0;
    हे->length = c->length () - table_initpos;
    हे->groups.len = (हे->length - min_size) / CmapSubtableLongGroup::static_size;
  }

  स्थिर माप_प्रकार get_sub_table_size (स्थिरांक hb_sorted_vector_t<CmapSubtableLongGroup> &groups_data)
  { परत 16 + 12 * groups_data.length; }

  खाजगी:
  स्थिर तार्किक _is_gid_consecutive (hb_codepoint_t endCharCode,
				   hb_codepoint_t startCharCode,
				   hb_codepoint_t glyphID,
				   hb_codepoint_t cp,
				   hb_codepoint_t new_gid)
  {
    परत (cp - 1 == endCharCode) &&
	new_gid == glyphID + (cp - startCharCode);
  }

};

संरचना CmapSubtableFormat13 : CmapSubtableLongSegmented<CmapSubtableFormat13>
{
  स्थिर constexpr पूर्णांक increment = 0;
  स्थिर constexpr पूर्णांक format_संख्या = 13;

  स्थिर hb_codepoint_t group_get_glyph (स्थिरांक CmapSubtableLongGroup &group,
					 hb_codepoint_t u HB_UNUSED)
  { परत group.glyphID; }
};

प्रकारव्याख्या गणना
{
  GLYPH_VARIANT_NOT_FOUND = 0,
  GLYPH_VARIANT_FOUND = 1,
  GLYPH_VARIANT_USE_DEFAULT = 2
} glyph_variant_t;

संरचना Unicode_मूल्य_Range
{
  पूर्णांक cmp (स्थिरांक hb_codepoint_t &codepoint) स्थिरांक
  {
    जर (codepoint < start_Unicode_मूल्य) परत -1;
    जर (codepoint > start_Unicode_मूल्य + additional_मोजणी) परत +1;
    परत 0;
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे));
  }

  HBUINT24	start_Unicode_मूल्य;	/* First value in this range. */
  HBUINT8	additional_मोजणी;	/* Number of additional values in this
					 * range. */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (4);
};

संरचना DefaultUVS : SortedArray32Of<Unicode_मूल्य_Range>
{
  शून्य collect_unicodes (hb_set_t *out) स्थिरांक
  {
    अचिन्हित पूर्णांक मोजणी = len;
    साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
    {
      hb_codepoint_t first = arrayZ[i].start_Unicode_मूल्य;
      hb_codepoint_t last = hb_min ((hb_codepoint_t) (first + arrayZ[i].additional_मोजणी),
				    (hb_codepoint_t) HB_UNICODE_MAX);
      out->add_range (first, last);
    }
  }

  DefaultUVS* copy (hb_serialize_context_t *c,
		    स्थिरांक hb_set_t *unicodes) स्थिरांक
  {
    स्वतः *out = c->start_embed<DefaultUVS> ();
    स्वतः snap = c->snapshot ();

    HBUINT32 len;
    len = 0;
    जर (unlikely (!c->copy<HBUINT32> (len))) परत nullptr;
    अचिन्हित init_len = c->length ();

    जर (हे->len > unicodes->get_population () * hb_bit_storage ((अचिन्हित) हे->len))
    {
      hb_codepoint_t start = HB_SET_मूल्य_INVALID;
      hb_codepoint_t end = HB_SET_मूल्य_INVALID;

      साठी (स्वतः u : *unicodes)
      {
        जर (!as_array ().द्वा_खोज (u))
	  सुरूठेवा;
	जर (start == HB_SET_मूल्य_INVALID)
	{
	  start = u;
	  end = start - 1;
	}
	जर (end + 1 != u || end - start == 255)
        {
	  Unicode_मूल्य_Range rec;
	  rec.start_Unicode_मूल्य = start;
	  rec.additional_मोजणी = end - start;
	  c->copy<Unicode_मूल्य_Range> (rec);
	  start = u;
	}
	end = u;
      }
      जर (start != HB_SET_मूल्य_INVALID)
      {
	Unicode_मूल्य_Range rec;
	rec.start_Unicode_मूल्य = start;
	rec.additional_मोजणी = end - start;
	c->copy<Unicode_मूल्य_Range> (rec);
      }

    }
    अन्यथा
    {
      hb_codepoint_t lastCode = HB_SET_मूल्य_INVALID;
      पूर्णांक मोजणी = -1;

      साठी (स्थिरांक Unicode_मूल्य_Range& _ : *हे)
      {
	hb_codepoint_t curEntry = (hb_codepoint_t) (_.start_Unicode_मूल्य - 1);
	hb_codepoint_t end = curEntry + _.additional_मोजणी + 2;

	साठी (; unicodes->next (&curEntry) && curEntry < end;)
	{
	  मोजणी += 1;
	  जर (lastCode == HB_SET_मूल्य_INVALID)
	    lastCode = curEntry;
	  अन्यथा जर (lastCode + मोजणी != curEntry || मोजणी > 255)
	  {
	    Unicode_मूल्य_Range rec;
	    rec.start_Unicode_मूल्य = lastCode;
	    rec.additional_मोजणी = मोजणी - 1;
	    c->copy<Unicode_मूल्य_Range> (rec);

	    lastCode = curEntry;
	    मोजणी = 0;
	  }
	}
      }

      जर (lastCode != HB_MAP_मूल्य_INVALID)
      {
	Unicode_मूल्य_Range rec;
	rec.start_Unicode_मूल्य = lastCode;
	rec.additional_मोजणी = मोजणी;
	c->copy<Unicode_मूल्य_Range> (rec);
      }
    }

    जर (c->length () - init_len == 0)
    {
      c->revert (snap);
      परत nullptr;
    }
    अन्यथा
    {
      जर (unlikely (!c->check_assign (out->len,
                                      (c->length () - init_len) / Unicode_मूल्य_Range::static_size,
                                      HB_SERIALIZE_ERROR_INT_OVERFLOW))) परत nullptr;
      परत out;
    }
  }

  सार्वजनिक:
  DEFINE_SIZE_ARRAY (4, *हे);
};

ढाँचा <typename Types>
संरचना UVSMapping
{
  using GlyphID = typename Types::HBGlyphID;

  पूर्णांक cmp (स्थिरांक hb_codepoint_t &codepoint) स्थिरांक
  { परत unicode_मूल्य.cmp (codepoint); }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे));
  }

  HBUINT24	unicode_मूल्य;	/* Base Unicode value of the UVS */
  GlyphID
		glyphID;	/* Glyph ID of the UVS. */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (3 + GlyphID::static_size);
};

ढाँचा <typename Types>
संरचना NonDefaultUVS : SortedArray32Of<UVSMapping<Types>>
{
  using Mapping = UVSMapping<Types>;

  शून्य collect_unicodes (hb_set_t *out) स्थिरांक
  {
    साठी (स्थिरांक स्वतः& a : हे->as_array ())
      out->add (a.unicode_मूल्य);
  }

  शून्य collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping /* OUT */) स्थिरांक
  {
    साठी (स्थिरांक स्वतः& a : हे->as_array ())
    {
      hb_codepoint_t unicode = a.unicode_मूल्य;
      hb_codepoint_t glyphid = a.glyphID;
      unicodes->add (unicode);
      mapping->set (unicode, glyphid);
    }
  }

  शून्य closure_glyphs (स्थिरांक hb_set_t      *unicodes,
		       hb_set_t            *glyphset) स्थिरांक
  {
    + हे->as_array ()
    | hb_filter (unicodes, &Mapping::unicode_मूल्य)
    | hb_map (&Mapping::glyphID)
    | hb_sink (glyphset)
    ;
  }

  NonDefaultUVS* copy (hb_serialize_context_t *c,
		       स्थिरांक hb_set_t *unicodes,
		       स्थिरांक hb_set_t *glyphs_requested,
		       स्थिरांक hb_map_t *glyph_map) स्थिरांक
  {
    स्वतः *out = c->start_embed<NonDefaultUVS> ();
    स्वतः it =
    + हे->as_array ()
    | hb_filter ([&] (स्थिरांक Mapping& _)
		 {
		   परत unicodes->has (_.unicode_मूल्य) || glyphs_requested->has (_.glyphID);
		 })
    ;

    जर (!it) परत nullptr;

    HBUINT32 len;
    len = it.len ();
    जर (unlikely (!c->copy<HBUINT32> (len))) परत nullptr;

    साठी (स्थिरांक Mapping& _ : it)
    {
      Mapping mapping;
      mapping.unicode_मूल्य = _.unicode_मूल्य;
      mapping.glyphID = glyph_map->get (_.glyphID);
      c->copy<Mapping> (mapping);
    }

    परत out;
  }

  सार्वजनिक:
  DEFINE_SIZE_ARRAY (4, *हे);
};

ढाँचा <typename Types>
संरचना VariationSelectorRecord
{
  glyph_variant_t get_glyph (hb_codepoint_t codepoint,
			     hb_codepoint_t *glyph,
			     स्थिरांक शून्य *base) स्थिरांक
  {
    जर ((base+defaultUVS).bfind (codepoint))
      परत GLYPH_VARIANT_USE_DEFAULT;
    स्थिरांक UVSMapping<Types> &nonDefault = (base+nonDefaultUVS).द्वा_खोज (codepoint);
    जर (nonDefault.glyphID)
    {
      *glyph = nonDefault.glyphID;
       परत GLYPH_VARIANT_FOUND;
    }
    परत GLYPH_VARIANT_NOT_FOUND;
  }

  VariationSelectorRecord(स्थिरांक VariationSelectorRecord& other)
  {
    *हे = other;
  }

  शून्य चालक= (स्थिरांक VariationSelectorRecord& other)
  {
    varSelector = other.varSelector;
    HBUINT32 offset = other.defaultUVS;
    defaultUVS = offset;
    offset = other.nonDefaultUVS;
    nonDefaultUVS = offset;
  }

  शून्य collect_unicodes (hb_set_t *out, स्थिरांक शून्य *base) स्थिरांक
  {
    (base+defaultUVS).collect_unicodes (out);
    (base+nonDefaultUVS).collect_unicodes (out);
  }

  शून्य collect_mapping (स्थिरांक शून्य *base,
			hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping /* OUT */) स्थिरांक
  {
    (base+defaultUVS).collect_unicodes (unicodes);
    (base+nonDefaultUVS).collect_mapping (unicodes, mapping);
  }

  पूर्णांक cmp (स्थिरांक hb_codepoint_t &variation_selector) स्थिरांक
  { परत varSelector.cmp (variation_selector); }

  तार्किक sanitize (hb_sanitize_context_t *c, स्थिरांक शून्य *base) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) &&
		  defaultUVS.sanitize (c, base) &&
		  nonDefaultUVS.sanitize (c, base));
  }

  hb_pair_t<अचिन्हित, अचिन्हित>
  copy (hb_serialize_context_t *c,
	स्थिरांक hb_set_t *unicodes,
	स्थिरांक hb_set_t *glyphs_requested,
	स्थिरांक hb_map_t *glyph_map,
	स्थिरांक शून्य *base) स्थिरांक
  {
    स्वतः snap = c->snapshot ();
    स्वतः *out = c->embed<VariationSelectorRecord> (*हे);
    जर (unlikely (!out)) परत hb_pair (0, 0);

    out->defaultUVS = 0;
    out->nonDefaultUVS = 0;

    अचिन्हित non_default_uvs_objidx = 0;
    जर (nonDefaultUVS != 0)
    {
      c->push ();
      जर (c->copy (base+nonDefaultUVS, unicodes, glyphs_requested, glyph_map))
	non_default_uvs_objidx = c->काढा_pack ();
      अन्यथा c->काढा_discard ();
    }

    अचिन्हित default_uvs_objidx = 0;
    जर (defaultUVS != 0)
    {
      c->push ();
      जर (c->copy (base+defaultUVS, unicodes))
	default_uvs_objidx = c->काढा_pack ();
      अन्यथा c->काढा_discard ();
    }


    जर (!default_uvs_objidx && !non_default_uvs_objidx)
      c->revert (snap);

    परत hb_pair (default_uvs_objidx, non_default_uvs_objidx);
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
  स्थिर constexpr अचिन्हित format_मूल्य = Types::size == 2 ? 14 : 15;
  using Record = VariationSelectorRecord<Types>;
  using NonDefault = NonDefaultUVS<Types>;

  glyph_variant_t get_glyph_variant (hb_codepoint_t codepoint,
				     hb_codepoint_t variation_selector,
				     hb_codepoint_t *glyph) स्थिरांक
  { परत record.द्वा_खोज (variation_selector).get_glyph (codepoint, glyph, हे); }

  शून्य collect_variation_selectors (hb_set_t *out) स्थिरांक
  {
    साठी (स्थिरांक स्वतः& a : record.as_array ())
      out->add (a.varSelector);
  }
  शून्य collect_variation_unicodes (hb_codepoint_t variation_selector,
				   hb_set_t *out) स्थिरांक
  { record.द्वा_खोज (variation_selector).collect_unicodes (out, हे); }

  शून्य serialize (hb_serialize_context_t *c,
		  स्थिरांक hb_set_t *unicodes,
		  स्थिरांक hb_set_t *glyphs_requested,
		  स्थिरांक hb_map_t *glyph_map,
		  स्थिरांक शून्य *base)
  {
    स्वतः snap = c->snapshot ();
    अचिन्हित table_initpos = c->length ();
    स्थिरांक अक्षर* init_tail = c->tail;

    जर (unlikely (!c->extend_min (हे))) परत;
    हे->format = format_मूल्य;

    स्वतः src_tbl = reinterpret_cast<स्थिरांक CmapSubtableFormat14_15*> (base);

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
    साठी (पूर्णांक i = src_tbl->record.len - 1; i >= 0; i--)
    {
      जर (!unicodes->has(src_tbl->record[i].varSelector))
        सुरूठेवा;

      hb_pair_t<अचिन्हित, अचिन्हित> result = src_tbl->record[i].copy (c, unicodes, glyphs_requested, glyph_map, base);
      जर (result.first || result.second)
	obj_indices.push (result);
    }

    जर (c->length () - table_initpos == CmapSubtableFormat14_15::min_size)
    {
      c->revert (snap);
      परत;
    }

    जर (unlikely (!c->check_success (!obj_indices.in_error ())))
      परत;

    पूर्णांक tail_len = init_tail - c->tail;
    c->check_assign (हे->length, c->length () - table_initpos + tail_len,
                     HB_SERIALIZE_ERROR_INT_OVERFLOW);
    c->check_assign (हे->record.len,
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
					स्थिरांक hb_vector_t<hb_pair_t<अचिन्हित, अचिन्हित>>& obj_indices)
  {
    साठी (अचिन्हित i = 0; i < obj_indices.length; i++)
    {
      /*
       * Since the record array has been reversed (see comments in copy())
       * but obj_indices has not been, the indices at obj_indices[i]
       * are for the variation record at record[j].
       */
      पूर्णांक j = obj_indices.length - 1 - i;
      c->add_link (record[j].defaultUVS, obj_indices[i].first);
      c->add_link (record[j].nonDefaultUVS, obj_indices[i].second);
    }
  }

  शून्य closure_glyphs (स्थिरांक hb_set_t      *unicodes,
		       hb_set_t            *glyphset) स्थिरांक
  {
    + hb_iter (record)
    | hb_filter (hb_bool, &Record::nonDefaultUVS)
    | hb_filter (unicodes, &Record::varSelector)
    | hb_map (&Record::nonDefaultUVS)
    | hb_map (hb_add (हे))
    | hb_apply ([=] (स्थिरांक NonDefault& _) { _.closure_glyphs (unicodes, glyphset); })
    ;
  }

  शून्य collect_unicodes (hb_set_t *out) स्थिरांक
  {
    साठी (स्थिरांक Record& _ : record)
      _.collect_unicodes (out, हे);
  }

  शून्य collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping /* OUT */) स्थिरांक
  {
    साठी (स्थिरांक Record& _ : record)
      _.collect_mapping (हे, unicodes, mapping);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) &&
		  record.sanitize (c, हे));
  }

  संरक्षित:
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

  तार्किक is_variation_selector () स्थिरांक
  {
    निवड (u.format.v) {
    स्थिती 14:
#ifndef HB_NO_BEYOND_64K
    स्थिती 15:
#endif
      परत सत्य;
    पूर्वनिर्धारित:
      परत असत्य;
    }
  }

  glyph_variant_t get_glyph_variant (hb_codepoint_t codepoint,
				     hb_codepoint_t variation_selector,
				     hb_codepoint_t *glyph) स्थिरांक
  {
    निवड (u.format.v) {
    स्थिती 14: hb_barrier (); परत u.format14.get_glyph_variant (codepoint, variation_selector, glyph);
#ifndef HB_NO_BEYOND_64K
    स्थिती 15: hb_barrier (); परत u.format15.get_glyph_variant (codepoint, variation_selector, glyph);
#endif
    पूर्वनिर्धारित: परत GLYPH_VARIANT_NOT_FOUND;
    }
  }

  शून्य collect_variation_selectors (hb_set_t *out) स्थिरांक
  {
    निवड (u.format.v) {
    स्थिती 14: hb_barrier (); u.format14.collect_variation_selectors (out); परत;
#ifndef HB_NO_BEYOND_64K
    स्थिती 15: hb_barrier (); u.format15.collect_variation_selectors (out); परत;
#endif
    पूर्वनिर्धारित: परत;
    }
  }

  शून्य collect_variation_unicodes (hb_codepoint_t variation_selector,
				    hb_set_t *out) स्थिरांक
  {
    निवड (u.format.v) {
    स्थिती 14: hb_barrier (); u.format14.collect_variation_unicodes (variation_selector, out); परत;
#ifndef HB_NO_BEYOND_64K
    स्थिती 15: hb_barrier (); u.format15.collect_variation_unicodes (variation_selector, out); परत;
#endif
    पूर्वनिर्धारित: परत;
    }
  }

  शून्य closure_glyphs (स्थिरांक hb_set_t *unicodes, hb_set_t *glyphset) स्थिरांक
  {
    निवड (u.format.v) {
    स्थिती 14: hb_barrier (); u.format14.closure_glyphs (unicodes, glyphset); परत;
#ifndef HB_NO_BEYOND_64K
    स्थिती 15: hb_barrier (); u.format15.closure_glyphs (unicodes, glyphset); परत;
#endif
    पूर्वनिर्धारित: परत;
    }
  }

  तार्किक get_glyph (hb_codepoint_t codepoint,
		  hb_codepoint_t *glyph,
		  अचिन्हित पूर्णांक subtable_data_size = 0) स्थिरांक
  {
    निवड (u.format.v) {
    स्थिती  0: hb_barrier (); परत u.format0 .get_glyph (codepoint, glyph);
    स्थिती  4: hb_barrier (); परत subtable_data_size
				      ? u.format4.get_glyph (codepoint, glyph, subtable_data_size)
				      : असत्य;
    स्थिती  6: hb_barrier (); परत u.format6 .get_glyph (codepoint, glyph);
    स्थिती 10: hb_barrier (); परत u.format10.get_glyph (codepoint, glyph);
    स्थिती 12: hb_barrier (); परत u.format12.get_glyph (codepoint, glyph);
    स्थिती 13: hb_barrier (); परत u.format13.get_glyph (codepoint, glyph);
    स्थिती 14:
    पूर्वनिर्धारित: परत असत्य;
    }
  }
  शून्य collect_unicodes (hb_set_t *out,
			 अचिन्हित पूर्णांक अंक_glyphs = अच_पूर्णांक_उच्च,
			 अचिन्हित पूर्णांक subtable_data_size = 0) स्थिरांक
  {
    निवड (u.format.v) {
    स्थिती  0: hb_barrier (); u.format0 .collect_unicodes (out); परत;
    स्थिती  4: hb_barrier (); जर (subtable_data_size)
			      u.format4.collect_unicodes (out, subtable_data_size);
			    अन्यथा
			      u.format4.collect_unicodes (out);
			    परत;
    स्थिती  6: hb_barrier (); u.format6 .collect_unicodes (out); परत;
    स्थिती 10: hb_barrier (); u.format10.collect_unicodes (out); परत;
    स्थिती 12: hb_barrier (); u.format12.collect_unicodes (out, अंक_glyphs); परत;
    स्थिती 13: hb_barrier (); u.format13.collect_unicodes (out, अंक_glyphs); परत;
    स्थिती 14:
    पूर्वनिर्धारित: परत;
    }
  }

  शून्य collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping, /* OUT */
			अचिन्हित अंक_glyphs = अच_पूर्णांक_उच्च,
			अचिन्हित पूर्णांक subtable_data_size = 0) स्थिरांक
  {
    निवड (u.format.v) {
    स्थिती  0: hb_barrier (); u.format0 .collect_mapping (unicodes, mapping); परत;
    स्थिती  4: hb_barrier (); जर (subtable_data_size)
			      u.format4.collect_mapping (unicodes, mapping, subtable_data_size);
			    अन्यथा
			      u.format4.collect_mapping (unicodes, mapping);
			    परत;
    स्थिती  6: hb_barrier (); u.format6 .collect_mapping (unicodes, mapping); परत;
    स्थिती 10: hb_barrier (); u.format10.collect_mapping (unicodes, mapping); परत;
    स्थिती 12: hb_barrier (); u.format12.collect_mapping (unicodes, mapping, अंक_glyphs); परत;
    स्थिती 13: hb_barrier (); u.format13.collect_mapping (unicodes, mapping, अंक_glyphs); परत;
    स्थिती 14:
    पूर्वनिर्धारित: परत;
    }
  }

  अचिन्हित get_language () स्थिरांक
  {
    निवड (u.format.v) {
    स्थिती  0: hb_barrier (); परत u.format0 .get_language ();
    स्थिती  4: hb_barrier (); परत u.format4 .get_language ();
    स्थिती  6: hb_barrier (); परत u.format6 .get_language ();
    स्थिती 10: hb_barrier (); परत u.format10.get_language ();
    स्थिती 12: hb_barrier (); परत u.format12.get_language ();
    स्थिती 13: hb_barrier (); परत u.format13.get_language ();
    स्थिती 14:
    पूर्वनिर्धारित: परत 0;
    }
  }

  ढाँचा<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  शून्य serialize (hb_serialize_context_t *c,
		  Iterator it,
		  अचिन्हित format,
		  स्थिरांक hb_subset_plan_t *plan,
		  स्थिरांक शून्य *base)
  {
    निवड (format) {
    स्थिती  4: hb_barrier (); परत u.format4.serialize (c, it);
    स्थिती 12: hb_barrier (); परत u.format12.serialize (c, it);
    स्थिती 14: hb_barrier (); परत u.format14.serialize (c, &plan->unicodes, &plan->glyphs_requested, plan->glyph_map, base);
#ifndef HB_NO_BEYOND_64K
    स्थिती 15: hb_barrier (); परत u.format15.serialize (c, &plan->unicodes, &plan->glyphs_requested, plan->glyph_map, base);
#endif
    पूर्वनिर्धारित: परत;
    }
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    जर (!u.format.v.sanitize (c)) return_trace (असत्य);
    hb_barrier ();
    निवड (u.format.v) {
    स्थिती  0: hb_barrier (); return_trace (u.format0 .sanitize (c));
    स्थिती  4: hb_barrier (); return_trace (u.format4 .sanitize (c));
    स्थिती  6: hb_barrier (); return_trace (u.format6 .sanitize (c));
    स्थिती 10: hb_barrier (); return_trace (u.format10.sanitize (c));
    स्थिती 12: hb_barrier (); return_trace (u.format12.sanitize (c));
    स्थिती 13: hb_barrier (); return_trace (u.format13.sanitize (c));
    स्थिती 14: hb_barrier (); return_trace (u.format14.sanitize (c));
#ifndef HB_NO_BEYOND_64K
    स्थिती 15: hb_barrier (); return_trace (u.format15.sanitize (c));
#endif
    पूर्वनिर्धारित:return_trace (सत्य);
    }
  }

  सार्वजनिक:
  संघ {
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
  पूर्णांक cmp (स्थिरांक EncodingRecord &other) स्थिरांक
  {
    पूर्णांक ret;
    ret = platformID.cmp (other.platformID);
    जर (ret) परत ret;
    जर (other.encodingID != 0xFFFF)
    {
      ret = encodingID.cmp (other.encodingID);
      जर (ret) परत ret;
    }
    परत 0;
  }

  तार्किक sanitize (hb_sanitize_context_t *c, स्थिरांक शून्य *base) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) &&
		  subtable.sanitize (c, base));
  }

  ढाँचा<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  EncodingRecord* copy (hb_serialize_context_t *c,
			Iterator it,
			अचिन्हित format,
			स्थिरांक शून्य *base,
			स्थिरांक hb_subset_plan_t *plan,
			/* INOUT */ अचिन्हित *objidx) स्थिरांक
  {
    TRACE_SERIALIZE (हे);
    स्वतः snap = c->snapshot ();
    स्वतः *out = c->embed (हे);
    जर (unlikely (!out)) return_trace (nullptr);
    out->subtable = 0;

    जर (*objidx == 0)
    {
      CmapSubtable *cmapsubtable = c->push<CmapSubtable> ();
      अचिन्हित origin_length = c->length ();
      cmapsubtable->serialize (c, it, format, plan, &(base+subtable));
      जर (c->length () > origin_length && !c->in_error()) *objidx = c->काढा_pack ();
      अन्यथा c->काढा_discard ();
    }

    जर (*objidx == 0)
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

 खाजगी:
  hb_blob_ptr_t<cmap> base_blob;
  स्थिरांक अक्षर* base;
  अचिन्हित पूर्णांक table_length;
  hb_hashmap_t<अचिन्हित, hb::unique_ptr<hb_set_t>> cached_unicodes;

 सार्वजनिक:

  स्थिर SubtableUnicodesCache* create (hb_blob_ptr_t<cmap> source_table)
  {
    SubtableUnicodesCache* cache =
        (SubtableUnicodesCache*) hb_malloc (आकार(SubtableUnicodesCache));

    जर (unlikely (!cache))
      परत nullptr;

    नवीन (cache) SubtableUnicodesCache (source_table);
    परत cache;
  }

  स्थिर शून्य destroy (शून्य* value) {
    जर (!value) परत;

    SubtableUnicodesCache* cache = (SubtableUnicodesCache*) value;
    cache->~SubtableUnicodesCache ();
    hb_free (cache);
  }

  SubtableUnicodesCache(स्थिरांक शून्य* cmap_base,
			अचिन्हित पूर्णांक table_length_ = 0)
      : base_blob(),
        base ((स्थिरांक अक्षर*) cmap_base),
        table_length (table_length_),
        cached_unicodes ()
  {}

  SubtableUnicodesCache(hb_blob_ptr_t<cmap> base_blob_)
      : base_blob(base_blob_),
        base ((स्थिरांक अक्षर *) base_blob.get()),
        table_length (base_blob.get_length ()),
        cached_unicodes ()
  {}

  ~SubtableUnicodesCache()
  {
    base_blob.destroy ();
  }

  तार्किक same_base(स्थिरांक शून्य* other) स्थिरांक
  {
    परत other == (स्थिरांक शून्य*) base;
  }

  स्थिरांक hb_set_t* set_for (स्थिरांक EncodingRecord* record,
                           SubtableUnicodesCache& mutable_cache) स्थिरांक
  {
    जर (cached_unicodes.has ((अचिन्हित) ((स्थिरांक अक्षर *) record - base)))
      परत cached_unicodes.get ((अचिन्हित) ((स्थिरांक अक्षर *) record - base));

    परत mutable_cache.set_for (record);
  }

  स्थिरांक hb_set_t* set_for (स्थिरांक EncodingRecord* record)
  {
    जर (!cached_unicodes.has ((अचिन्हित) ((स्थिरांक अक्षर *) record - base)))
    {
      hb_set_t *s = hb_set_create ();
      जर (unlikely (s->in_error ()))
	परत hb_set_get_empty ();

      अचिन्हित पूर्णांक subtable_data_size = record->subtable < table_length
					? table_length - (अचिन्हित पूर्णांक) record->subtable
					: 0;
      (base+record->subtable).collect_unicodes (s, अच_पूर्णांक_उच्च, subtable_data_size);

      जर (unlikely (!cached_unicodes.set ((अचिन्हित) ((स्थिरांक अक्षर *) record - base), hb::unique_ptr<hb_set_t> {s})))
        परत hb_set_get_empty ();

      परत s;
    }
    परत cached_unicodes.get ((अचिन्हित) ((स्थिरांक अक्षर *) record - base));
  }

};

स्थिर ओळीत uint16_t
hb_चिन्ह_pua_map (अचिन्हित codepoint)
{
  जर (codepoint <= 0x00FFu)
  {
    /* For symbol-encoded OpenType fonts, we duplicate the
     * U+F000..F0FF range at U+0000..U+00FF.  That's what
     * Windows seems to do, and that's hinted about at:
     * https://docs.microsoft.com/en-us/typography/opentype/spec/recom
     * under "Non-Standard (Symbol) Fonts". */
    परत 0xF000u + codepoint;
  }
  परत 0;
}

संरचना cmap
{
  स्थिर constexpr hb_tag_t tableTag = HB_OT_TAG_cmap;


  स्थिर SubtableUnicodesCache* create_filled_cache(hb_blob_ptr_t<cmap> source_table) {
    स्थिरांक cmap* cmap = source_table.get();
    स्वतः it =
    + hb_iter (cmap->encodingRecord)
    | hb_filter ([&](स्थिरांक EncodingRecord& _) {
      परत cmap::filter_encoding_records_for_subset (cmap, _);
    })
    ;

    SubtableUnicodesCache* cache = SubtableUnicodesCache::create(source_table);

    जर (unlikely (!cache))
      परत nullptr;

    साठी (स्थिरांक EncodingRecord& _ : it)
      cache->set_for(&_); // populate the cache for this encoding record.

    परत cache;
  }

  ढाँचा<typename Iterator, typename EncodingRecIter,
	   hb_requires (hb_is_iterator (EncodingRecIter))>
  तार्किक serialize (hb_serialize_context_t *c,
		  Iterator it,
		  EncodingRecIter encodingrec_iter,
		  स्थिरांक शून्य *base,
		  hb_subset_plan_t *plan,
	                  तार्किक drop_format_4 = असत्य,
			  अचिन्हित पूर्णांक source_table_length = 0)
  {
    जर (unlikely (!c->extend_min ((*हे))))  परत असत्य;
    हे->version = 0;

    अचिन्हित format4objidx = 0, format12objidx = 0, format14objidx = 0;
#ifndef HB_NO_BEYOND_64K
    अचिन्हित format15objidx = 0;
#endif
    स्वतः snap = c->snapshot ();

    SubtableUnicodesCache local_unicodes_cache (base, source_table_length);
    स्थिरांक SubtableUnicodesCache* unicodes_cache = &local_unicodes_cache;

    जर (plan->accelerator &&
        plan->accelerator->cmap_cache &&
        plan->accelerator->cmap_cache->same_base (base))
      unicodes_cache = plan->accelerator->cmap_cache;

    साठी (स्थिरांक EncodingRecord& _ : encodingrec_iter)
    {
      जर (c->in_error ())
        परत असत्य;

      अचिन्हित format = (base+_.subtable).u.format.v;
      जर (format != 4 && format != 12 && format != 14
#ifndef HB_NO_BEYOND_64K
	  && format != 15
#endif
	 ) सुरूठेवा;

      स्थिरांक hb_set_t* unicodes_set = unicodes_cache->set_for (&_, local_unicodes_cache);

      जर (!drop_format_4 && format == 4)
      {
        c->copy (_, + it | hb_filter (*unicodes_set, hb_first), 4u, base, plan, &format4objidx);
        जर (c->in_error () && c->only_overflow ())
        {
          // cmap4 overflowed, reset and retry serialization without format 4 subtables.
          c->revert (snap);
          परत serialize (c, it,
                            encodingrec_iter,
                            base,
                            plan,
                            सत्य,
			    source_table_length);
        }
      }

      अन्यथा जर (format == 12)
      {
        जर (_can_drop (_,
                       *unicodes_set,
                       base,
                       *unicodes_cache,
                       local_unicodes_cache,
                       + it | hb_map (hb_first), encodingrec_iter))
          सुरूठेवा;
        c->copy (_, + it | hb_filter (*unicodes_set, hb_first), 12u, base, plan, &format12objidx);
      }
      अन्यथा जर (format == 14) c->copy (_, it, 14u, base, plan, &format14objidx);
#ifndef HB_NO_BEYOND_64K
      अन्यथा जर (format == 15) c->copy (_, it, 15u, base, plan, &format15objidx);
#endif
    }
        अचिन्हित length = c->length ();
        अचिन्हित available = length > cmap::min_size ? length - cmap::min_size : 0;
        c->check_assign(हे->encodingRecord.len,
                        available / EncodingRecord::static_size,
                        HB_SERIALIZE_ERROR_INT_OVERFLOW);

    // Fail if format 4 was dropped and there is no cmap12.
    परत !drop_format_4 || format12objidx;
  }

  ढाँचा<typename Iterator, typename EncodingRecordIterator,
      hb_requires (hb_is_iterator (Iterator)),
      hb_requires (hb_is_iterator (EncodingRecordIterator))>
  तार्किक _can_drop (स्थिरांक EncodingRecord& cmap12,
                  स्थिरांक hb_set_t& cmap12_unicodes,
                  स्थिरांक शून्य* base,
                  स्थिरांक SubtableUnicodesCache& unicodes_cache,
                  SubtableUnicodesCache& local_unicodes_cache,
                  Iterator subset_unicodes,
                  EncodingRecordIterator encoding_records)
  {
    साठी (स्वतः cp : + subset_unicodes | hb_filter (cmap12_unicodes))
    {
      जर (cp >= 0x10000) परत असत्य;
    }

    अचिन्हित target_platform;
    अचिन्हित target_encoding;
    अचिन्हित target_language = (base+cmap12.subtable).get_language ();

    जर (cmap12.platformID == 0 && cmap12.encodingID == 4)
    {
      target_platform = 0;
      target_encoding = 3;
    } अन्यथा जर (cmap12.platformID == 3 && cmap12.encodingID == 10) {
      target_platform = 3;
      target_encoding = 1;
    } अन्यथा {
      परत असत्य;
    }

    साठी (स्थिरांक स्वतः& _ : encoding_records)
    {
      जर (_.platformID != target_platform
          || _.encodingID != target_encoding
          || (base+_.subtable).get_language() != target_language)
        सुरूठेवा;

      स्थिरांक hb_set_t* sibling_unicodes = unicodes_cache.set_for (&_, local_unicodes_cache);

      स्वतः cmap12 = + subset_unicodes | hb_filter (cmap12_unicodes);
      स्वतः sibling = + subset_unicodes | hb_filter (*sibling_unicodes);
      साठी (; cmap12 && sibling; cmap12++, sibling++)
      {
        अचिन्हित a = *cmap12;
        अचिन्हित b = *sibling;
        जर (a != b) परत असत्य;
      }

      परत !cmap12 && !sibling;
    }

    परत असत्य;
  }

  शून्य closure_glyphs (स्थिरांक hb_set_t      *unicodes,
		       hb_set_t            *glyphset) स्थिरांक
  {
    + hb_iter (encodingRecord)
    | hb_map (&EncodingRecord::subtable)
    | hb_map (hb_add (हे))
    | hb_filter (&CmapSubtable::is_variation_selector)
    | hb_apply ([=] (स्थिरांक CmapSubtable& _) { _.closure_glyphs (unicodes, glyphset); })
    ;
  }

  तार्किक subset (hb_subset_context_t *c) स्थिरांक
  { परत subset (c, असत्य); }

  तार्किक subset (hb_subset_context_t *c, तार्किक is_dmap) स्थिरांक
  {
    TRACE_SUBSET (हे);

    cmap *cmap_मूळसंख्या = c->serializer->start_embed<cmap> ();

    स्वतः encodingrec_iter =
    + hb_iter (encodingRecord)
    | hb_filter ([&](स्थिरांक EncodingRecord& _) {
      परत cmap::filter_encoding_records_for_subset (हे, _);
    })
    ;

    जर (unlikely (!encodingrec_iter.len ())) return_trace (असत्य);

    स्थिरांक EncodingRecord *unicode_bmp= nullptr, *unicode_ucs4 = nullptr, *ms_bmp = nullptr, *ms_ucs4 = nullptr;
    तार्किक has_format12 = असत्य;

    साठी (स्थिरांक EncodingRecord& _ : encodingrec_iter)
    {
      अचिन्हित format = (हे + _.subtable).u.format.v;
      जर (format == 12) has_format12 = सत्य;

      स्थिरांक EncodingRecord *table = std::addressof (_);
      जर      (_.platformID == 0 && _.encodingID ==  3) unicode_bmp = table;
      अन्यथा जर (_.platformID == 0 && _.encodingID ==  4) unicode_ucs4 = table;
      अन्यथा जर (_.platformID == 3 && _.encodingID ==  1) ms_bmp = table;
      अन्यथा जर (_.platformID == 3 && _.encodingID == 10) ms_ucs4 = table;
    }

    जर (unlikely (!is_dmap && !has_format12 && !unicode_bmp && !ms_bmp)) return_trace (असत्य);
    जर (unlikely (!is_dmap && has_format12 && (!unicode_ucs4 && !ms_ucs4))) return_trace (असत्य);

    स्वतः it =
    + c->plan->unicode_to_new_gid_list.iter ()
    | hb_filter ([&] (स्थिरांक hb_codepoint_pair_t _)
		 { परत (_.second != HB_MAP_मूल्य_INVALID); })
    ;

    return_trace (cmap_मूळसंख्या->serialize (c->serializer,
                                         it,
                                         encodingrec_iter,
                                         हे,
                                         c->plan,
					 असत्य,
					 c->source_blob->length));
  }

  स्थिरांक CmapSubtable *find_best_subtable (तार्किक *चिन्ह = nullptr,
					  तार्किक *mac = nullptr,
					  तार्किक *macroman = nullptr) स्थिरांक
  {
    जर (चिन्ह) *चिन्ह = असत्य;
    जर (mac) *mac = असत्य;
    जर (macroman) *macroman = असत्य;

    स्थिरांक CmapSubtable *subtable;

    /* Symbol subtable.
     * Prefer symbol if available.
     * https://github.com/harfbuzz/harfbuzz/issues/1918 */
    जर ((subtable = हे->find_subtable (3, 0)))
    {
      जर (चिन्ह) *चिन्ह = सत्य;
      परत subtable;
    }

    /* 32-bit subtables. */
    जर ((subtable = हे->find_subtable (3, 10))) परत subtable;
    जर ((subtable = हे->find_subtable (0, 6))) परत subtable;
    जर ((subtable = हे->find_subtable (0, 4))) परत subtable;

    /* 16-bit subtables. */
    जर ((subtable = हे->find_subtable (3, 1))) परत subtable;
    जर ((subtable = हे->find_subtable (0, 3))) परत subtable;
    जर ((subtable = हे->find_subtable (0, 2))) परत subtable;
    जर ((subtable = हे->find_subtable (0, 1))) परत subtable;
    जर ((subtable = हे->find_subtable (0, 0))) परत subtable;

    /* MacRoman subtable. */
    जर ((subtable = हे->find_subtable (1, 0)))
    {
      जर (mac) *mac = सत्य;
      जर (macroman) *macroman = सत्य;
      परत subtable;
    }
    /* Any other Mac subtable; we just map ASCII for these. */
    जर ((subtable = हे->find_subtable (1, 0xFFFF)))
    {
      जर (mac) *mac = सत्य;
      परत subtable;
    }

    /* Meh. */
    परत &Null (CmapSubtable);
  }

  संरचना accelerator_t
  {
    using cache_t = hb_cache_t<21, 19>;
    static_assert (आकार (cache_t) == 1024, "");

    accelerator_t (hb_face_t *face, hb_tag_t table_tag = cmap::tableTag)
    {
      हे->table = hb_sanitize_context_t ().reference_table<cmap> (face, table_tag);
      जर (!हे->table.get_length ())
	परत;

      तार्किक चिन्ह, mac, macroman;
      हे->subtable = table->find_best_subtable (&चिन्ह, &mac, &macroman);
      हे->subtable_uvs = &Null (CmapSubtable);
      {
	स्थिरांक CmapSubtable *st = table->find_variation_selector_subtable ();
	जर (st)
	  subtable_uvs = st;
      }

#ifndef HB_NO_OT_FONT_CMAP_CACHE
      cache = (cache_t *) hb_malloc (आकार (cache_t));
      जर (cache)
	नवीन (cache) cache_t ();
      अन्यथा
        परत; // Such that get_glyph_funcZ remains null.
#endif

      हे->get_glyph_data = subtable;
#ifndef HB_NO_CMAP_LEGACY_SUBTABLES
      तार्किक is_format4 = subtable->u.format.v == 4;
      स्वतः set_format4_getter = [हे] (तार्किक (*func) (स्थिरांक शून्य *,
						      hb_codepoint_t,
						      hb_codepoint_t *))
      {
	हे->format4_accel.init (&हे->subtable->u.format4,
				  get_subtable_data_size (हे->subtable));
	हे->get_glyph_data = &हे->format4_accel;
	हे->get_glyph_funcZ = func;
      };
      जर (unlikely (चिन्ह))
      {
	निवड ((अचिन्हित) face->table.OS2->get_font_page ()) {
	स्थिती OS2::font_page_t::FONT_PAGE_NONE:
	  जर (is_format4)
	    set_format4_getter (get_glyph_from_चिन्ह<CmapSubtableFormat4::accelerator_t, hb_चिन्ह_pua_map>);
	  अन्यथा
	    हे->get_glyph_funcZ = get_glyph_from_चिन्ह<CmapSubtable, hb_चिन्ह_pua_map>;
	  थांबा;
#ifndef HB_NO_OT_SHAPER_ARABIC_FALLBACK
	स्थिती OS2::font_page_t::FONT_PAGE_SIMP_ARABIC:
	  जर (is_format4)
	    set_format4_getter (get_glyph_from_चिन्ह<CmapSubtableFormat4::accelerator_t, _hb_arabic_pua_simp_map>);
	  अन्यथा
	    हे->get_glyph_funcZ = get_glyph_from_चिन्ह<CmapSubtable, _hb_arabic_pua_simp_map>;
	  थांबा;
	स्थिती OS2::font_page_t::FONT_PAGE_TRAD_ARABIC:
	  जर (is_format4)
	    set_format4_getter (get_glyph_from_चिन्ह<CmapSubtableFormat4::accelerator_t, _hb_arabic_pua_trad_map>);
	  अन्यथा
	    हे->get_glyph_funcZ = get_glyph_from_चिन्ह<CmapSubtable, _hb_arabic_pua_trad_map>;
	  थांबा;
#endif
	पूर्वनिर्धारित:
	  जर (is_format4)
	    set_format4_getter (get_glyph_from<CmapSubtableFormat4::accelerator_t>);
	  अन्यथा
	    हे->get_glyph_funcZ = get_glyph_from<CmapSubtable>;
	  थांबा;
	}
      }
      अन्यथा जर (unlikely (macroman))
      {
	जर (is_format4)
	  set_format4_getter (get_glyph_from_macroman<CmapSubtableFormat4::accelerator_t>);
	अन्यथा
	  हे->get_glyph_funcZ = get_glyph_from_macroman<CmapSubtable>;
      }
      अन्यथा जर (unlikely (mac))
      {
	जर (is_format4)
	  set_format4_getter (get_glyph_from_ascii<CmapSubtableFormat4::accelerator_t>);
	अन्यथा
	  हे->get_glyph_funcZ = get_glyph_from_ascii<CmapSubtable>;
      }
      अन्यथा
#endif
      {
	निवड (subtable->u.format.v) {
	  /* Accelerate format 4 and format 12. */
	  पूर्वनिर्धारित:
	    हे->get_glyph_funcZ = get_glyph_from<CmapSubtable>;
	    थांबा;
	  स्थिती 12:
	    हे->get_glyph_funcZ = get_glyph_from<CmapSubtableFormat12>;
	    थांबा;
	  स्थिती  4:
	  {
	    hb_barrier ();
	    हे->format4_accel.init (&subtable->u.format4,
				      get_subtable_data_size (subtable));
	    हे->get_glyph_data = &हे->format4_accel;
	    हे->get_glyph_funcZ = हे->format4_accel.get_glyph_func;
	    थांबा;
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

    ओळीत तार्किक _cached_get (hb_codepoint_t unicode,
			     hb_codepoint_t *glyph) स्थिरांक
    {
#ifndef HB_NO_OT_FONT_CMAP_CACHE
      // cache is always non-null if we have a get_glyph_funcZ
      अचिन्हित v;
      जर (cache->get (unicode, &v))
      {
        *glyph = v;
	परत सत्य;
      }
#endif
      तार्किक ret  = हे->get_glyph_funcZ (हे->get_glyph_data, unicode, glyph);

#ifndef HB_NO_OT_FONT_CMAP_CACHE
      जर (ret)
        cache->set (unicode, *glyph);
#endif

      परत ret;
    }

    तार्किक get_nominal_glyph (hb_codepoint_t  unicode,
			    hb_codepoint_t *glyph) स्थिरांक
    {
      जर (unlikely (!हे->get_glyph_funcZ)) परत असत्य;
      परत _cached_get (unicode, glyph);
    }

    अचिन्हित पूर्णांक get_nominal_glyphs (अचिन्हित पूर्णांक मोजणी,
				     स्थिरांक hb_codepoint_t *first_unicode,
				     अचिन्हित पूर्णांक unicode_stride,
				     hb_codepoint_t *first_glyph,
				     अचिन्हित पूर्णांक glyph_stride) स्थिरांक
    {
      जर (unlikely (!हे->get_glyph_funcZ)) परत 0;

      अचिन्हित पूर्णांक done;
      साठी (done = 0;
	   done < मोजणी && _cached_get (*first_unicode, first_glyph);
	   done++)
      {
	first_unicode = &StructAtOffsetUnaligned<hb_codepoint_t> (first_unicode, unicode_stride);
	first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      }
      परत done;
    }

    तार्किक get_variation_glyph (hb_codepoint_t  unicode,
			      hb_codepoint_t  variation_selector,
			      hb_codepoint_t *glyph) स्थिरांक
    {
      निवड (get_glyph_variant (unicode, variation_selector, glyph))
      {
	स्थिती GLYPH_VARIANT_NOT_FOUND:	परत असत्य;
	स्थिती GLYPH_VARIANT_FOUND:	परत सत्य;
	स्थिती GLYPH_VARIANT_USE_DEFAULT:	थांबा;
      }

      परत get_nominal_glyph (unicode, glyph);
    }

    glyph_variant_t get_glyph_variant (hb_codepoint_t  unicode,
				       hb_codepoint_t  variation_selector,
				       hb_codepoint_t *glyph) स्थिरांक
    {
      परत हे->subtable_uvs->get_glyph_variant (unicode,
						    variation_selector,
						    glyph);
    }

    तार्किक has_data () स्थिरांक { परत table.get_length (); }

    शून्य collect_unicodes (hb_set_t *out, अचिन्हित पूर्णांक अंक_glyphs) स्थिरांक
    { subtable->collect_unicodes (out, अंक_glyphs, get_subtable_data_size (subtable)); }
    शून्य collect_mapping (hb_set_t *unicodes, hb_map_t *mapping,
			  अचिन्हित अंक_glyphs = अच_पूर्णांक_उच्च) स्थिरांक
    { subtable->collect_mapping (unicodes, mapping, अंक_glyphs, get_subtable_data_size (subtable)); }
    शून्य collect_variation_selectors (hb_set_t *out) स्थिरांक
    { subtable_uvs->collect_variation_selectors (out); }
    शून्य collect_variation_unicodes (hb_codepoint_t variation_selector,
				     hb_set_t *out) स्थिरांक
    { subtable_uvs->collect_variation_unicodes (variation_selector, out); }

    संरक्षित:
    प्रकारव्याख्या तार्किक (*hb_cmap_get_glyph_func_t) (स्थिरांक शून्य *obj,
					      hb_codepoint_t codepoint,
					      hb_codepoint_t *glyph);
    प्रकारव्याख्या uint16_t (*hb_pua_remap_func_t) (अचिन्हित);

    ढाँचा <typename Type>
    HB_INTERNAL स्थिर तार्किक get_glyph_from (स्थिरांक शून्य *obj,
					    hb_codepoint_t codepoint,
					    hb_codepoint_t *glyph)
    {
      स्थिरांक Type *typed_obj = (स्थिरांक Type *) obj;
      परत typed_obj->get_glyph (codepoint, glyph);
    }

    ढाँचा <typename Type, hb_pua_remap_func_t remap>
    HB_INTERNAL स्थिर तार्किक get_glyph_from_चिन्ह (स्थिरांक शून्य *obj,
						   hb_codepoint_t codepoint,
						   hb_codepoint_t *glyph)
    {
      स्थिरांक Type *typed_obj = (स्थिरांक Type *) obj;
      जर (likely (typed_obj->get_glyph (codepoint, glyph)))
	परत सत्य;

      जर (hb_codepoint_t c = remap (codepoint))
	परत typed_obj->get_glyph (c, glyph);

      परत असत्य;
    }

    ढाँचा <typename Type>
    HB_INTERNAL स्थिर तार्किक get_glyph_from_ascii (स्थिरांक शून्य *obj,
						  hb_codepoint_t codepoint,
						  hb_codepoint_t *glyph)
    {
      स्थिरांक Type *typed_obj = (स्थिरांक Type *) obj;
      परत codepoint < 0x80 && typed_obj->get_glyph (codepoint, glyph);
    }

    ढाँचा <typename Type>
    HB_INTERNAL स्थिर तार्किक get_glyph_from_macroman (स्थिरांक शून्य *obj,
						     hb_codepoint_t codepoint,
						     hb_codepoint_t *glyph)
    {
      जर (get_glyph_from_ascii<Type> (obj, codepoint, glyph))
	परत सत्य;

      स्थिरांक Type *typed_obj = (स्थिरांक Type *) obj;
      अचिन्हित c = unicode_to_macroman (codepoint);
      परत c && typed_obj->get_glyph (c, glyph);
    }

    अचिन्हित पूर्णांक get_subtable_data_size (स्थिरांक CmapSubtable *subtable) स्थिरांक
    {
      अचिन्हित पूर्णांक table_length = हे->table.get_length ();
      uintptr_t table_start = (uintptr_t) (स्थिरांक शून्य *) हे->table.get ();
      uintptr_t subtable_addr = (uintptr_t) (स्थिरांक शून्य *) subtable;
      जर (unlikely (subtable_addr < table_start))
	परत 0;

      uintptr_t subtable_offset = subtable_addr - table_start;
      जर (unlikely (subtable_offset >= table_length))
	परत 0;

      परत table_length - (अचिन्हित पूर्णांक) subtable_offset;
    }

    खाजगी:
    hb_nonnull_ptr_t<स्थिरांक CmapSubtable> subtable;
    hb_nonnull_ptr_t<स्थिरांक CmapSubtable> subtable_uvs;

    hb_cmap_get_glyph_func_t get_glyph_funcZ = nullptr;
    स्थिरांक शून्य *get_glyph_data = nullptr;

    CmapSubtableFormat4::accelerator_t format4_accel;

#ifndef HB_NO_OT_FONT_CMAP_CACHE
    cache_t *cache = nullptr;
#endif

    सार्वजनिक:
    hb_blob_ptr_t<cmap> table;
  };

  संरक्षित:

  स्थिरांक CmapSubtable *find_subtable (अचिन्हित पूर्णांक platform_id,
				     अचिन्हित पूर्णांक encoding_id) स्थिरांक
  {
    EncodingRecord key;
    key.platformID = platform_id;
    key.encodingID = encoding_id;

    स्थिरांक EncodingRecord &result = encodingRecord.द्वा_खोज (key);
    जर (!result.subtable)
      परत nullptr;

    परत &(हे+result.subtable);
  }

  स्थिरांक CmapSubtable *find_variation_selector_subtable () स्थिरांक
  {
    स्थिरांक CmapSubtable *format14 = nullptr;
    साठी (स्थिरांक EncodingRecord& record : encodingRecord)
    {
      जर (record.platformID != 0 || record.encodingID != 5)
	सुरूठेवा;

      स्थिरांक CmapSubtable *subtable = &(हे+record.subtable);
#ifndef HB_NO_BEYOND_64K
      जर (subtable->u.format.v == 15)
	परत subtable;
#endif
      जर (subtable->u.format.v == 14)
	format14 = subtable;
    }
    परत format14;
  }

  सार्वजनिक:

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) &&
		  hb_barrier () &&
		  likely (version == 0) &&
		  encodingRecord.sanitize (c, हे));
  }

 खाजगी:

  स्थिर तार्किक filter_encoding_records_for_subset(स्थिरांक cmap* cmap,
                                                 स्थिरांक EncodingRecord& _)
  {
    परत
        (_.platformID == 0 && _.encodingID == 3) ||
        (_.platformID == 0 && _.encodingID == 4) ||
        (_.platformID == 3 && _.encodingID == 1) ||
        (_.platformID == 3 && _.encodingID == 10) ||
        (cmap + _.subtable).is_variation_selector ();
  }

  संरक्षित:
  HBUINT16	version;	/* Table version number (0). */
  SortedArray16Of<EncodingRecord>
		encodingRecord;	/* Encoding tables. */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (4, encodingRecord);
};

संरचना DMAP : cmap
{
  स्थिर constexpr hb_tag_t tableTag = HB_OT_TAG_DMAP;

  तार्किक subset (hb_subset_context_t *c) स्थिरांक
  { परत cmap::subset (c, सत्य); }
};

संरचना cmap_accelerator_t
{
  cmap_accelerator_t (hb_face_t *face) :
    dmap (face, HB_OT_TAG_DMAP),
    base (face)
  {}

  तार्किक get_nominal_glyph (hb_codepoint_t  unicode,
			  hb_codepoint_t *glyph) स्थिरांक
  {
    परत dmap.get_nominal_glyph (unicode, glyph) ||
	   base.get_nominal_glyph (unicode, glyph);
  }

  अचिन्हित पूर्णांक get_nominal_glyphs (अचिन्हित पूर्णांक मोजणी,
				   स्थिरांक hb_codepoint_t *first_unicode,
				   अचिन्हित पूर्णांक unicode_stride,
				   hb_codepoint_t *first_glyph,
				   अचिन्हित पूर्णांक glyph_stride) स्थिरांक
  {
    जर (likely (!dmap.has_data ()))
      परत base.get_nominal_glyphs (मोजणी,
				      first_unicode, unicode_stride,
				      first_glyph, glyph_stride);

    अचिन्हित पूर्णांक done;
    साठी (done = 0;
	 done < मोजणी && get_nominal_glyph (*first_unicode, first_glyph);
	 done++)
    {
      first_unicode = &StructAtOffsetUnaligned<hb_codepoint_t> (first_unicode, unicode_stride);
      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
    }
    परत done;
  }

  तार्किक get_variation_glyph (hb_codepoint_t  unicode,
			    hb_codepoint_t  variation_selector,
			    hb_codepoint_t *glyph) स्थिरांक
  {
    glyph_variant_t result = dmap.get_glyph_variant (unicode,
						     variation_selector,
						     glyph);
    जर (result == GLYPH_VARIANT_NOT_FOUND)
      result = base.get_glyph_variant (unicode, variation_selector, glyph);

    निवड (result)
    {
      स्थिती GLYPH_VARIANT_NOT_FOUND: परत असत्य;
      स्थिती GLYPH_VARIANT_FOUND: परत सत्य;
      स्थिती GLYPH_VARIANT_USE_DEFAULT: थांबा;
    }

    परत get_nominal_glyph (unicode, glyph);
  }

  शून्य collect_unicodes (hb_set_t *out, अचिन्हित पूर्णांक अंक_glyphs) स्थिरांक
  {
    base.collect_unicodes (out, अंक_glyphs);
    dmap.collect_unicodes (out, अंक_glyphs);
  }

  शून्य collect_mapping (hb_set_t *unicodes, hb_map_t *mapping,
			अचिन्हित अंक_glyphs = अच_पूर्णांक_उच्च) स्थिरांक
  {
    base.collect_mapping (unicodes, mapping, अंक_glyphs);
    dmap.collect_mapping (unicodes, mapping, अंक_glyphs);
  }

  शून्य collect_variation_selectors (hb_set_t *out) स्थिरांक
  {
    base.collect_variation_selectors (out);
    dmap.collect_variation_selectors (out);
  }

  शून्य collect_variation_unicodes (hb_codepoint_t variation_selector,
				   hb_set_t *out) स्थिरांक
  {
    base.collect_variation_unicodes (variation_selector, out);
    dmap.collect_variation_unicodes (variation_selector, out);
  }

  शून्य closure_glyphs (स्थिरांक hb_set_t *unicodes,
		       hb_set_t       *glyphset) स्थिरांक
  {
    जर (likely (!dmap.has_data ()))
    {
      base.table->closure_glyphs (unicodes, glyphset);
      परत;
    }

    hb_set_t variation_selectors;
    collect_variation_selectors (&variation_selectors);
    साठी (hb_codepoint_t variation_selector : variation_selectors)
    {
      जर (!unicodes->has (variation_selector))
	सुरूठेवा;

      hb_set_t variation_unicodes;
      collect_variation_unicodes (variation_selector, &variation_unicodes);
      साठी (hb_codepoint_t unicode : variation_unicodes)
      {
	जर (!unicodes->has (unicode))
	  सुरूठेवा;

	hb_codepoint_t glyph;
	जर (get_variation_glyph (unicode, variation_selector, &glyph))
	  glyphset->add (glyph);
      }
    }
  }

  cmap::accelerator_t dmap;
  cmap::accelerator_t base;
};

} /* namespace OT */


#endif /* HB_OT_CMAP_TABLE_HH */
