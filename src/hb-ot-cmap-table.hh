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

#અન્તર્ભાવ "hb-ot-os2-table.hh"
#અન્તર્ભાવ "hb-ot-shaper-arabic-pua.hh"
#અન્તર્ભાવ "hb-open-type.hh"
#અન્તર્ભાવ "hb-ot-dual.hh"
#અન્તર્ભાવ "hb-set.hh"
#અન્તર્ભાવ "hb-cache.hh"

/*
 * cmap -- Character to Glyph Index Mapping
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cmap
 */
#define HB_OT_TAG_cmap HB_TAG('c','m','a','p')
#define HB_OT_TAG_DMAP HB_TAG('D','M','A','P')

નામાકાશ OT {

સ્થિર અંતઃરેખ uint8_t unicode_to_macroman (hb_codepoint_t u)
{
  સ્થિર અચલ સંરચના unicode_to_macroman_t
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
  auto *c = hb_bsearch (u, mapping, ARRAY_LENGTH (mapping), કદ (mapping[0]),
			_hb_cmp_operator<uint16_t, uint16_t>);
  પાછા c ? c->macroman : 0;
}

સંરચના CmapSubtableFormat0
{
  તાર્કિક get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) અચલ
  {
    hb_codepoint_t gid = codepoint < 256 ? glyphIdArray[codepoint] : 0;
    જો (unlikely (!gid))
      પાછા ખોટું;
    *glyph = gid;
    પાછા સાચું;
  }

  unsigned get_language () અચલ
  {
    પાછા language;
  }

  શૂન્ય collect_unicodes (hb_set_t *out) અચલ
  {
    માટે (unsigned પૂર્ણાંક i = 0; i < 256; i++)
      જો (glyphIdArray[i])
	out->add (i);
  }

  શૂન્ય collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping /* OUT */) અચલ
  {
    માટે (unsigned i = 0; i < 256; i++)
      જો (glyphIdArray[i])
      {
	hb_codepoint_t glyph = glyphIdArray[i];
	unicodes->add (i);
	mapping->set (i, glyph);
      }
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ));
  }

  સુરક્ષિત:
  HBUINT16	format;		/* Format number is set to 0. */
  HBUINT16	length;		/* Byte length of this subtable. */
  HBUINT16	language;	/* Ignore. */
  HBUINT8	glyphIdArray[256];/* An array that maps character
				 * code to glyph index values. */
  જાહેર:
  DEFINE_SIZE_STATIC (6 + 256);
};

સંરચના CmapSubtableFormat4
{
  template<typename Iterator,
      typename Writer,
	   hb_requires (hb_is_iterator (Iterator))>
  શૂન્ય to_ranges (Iterator it, Writer& range_writer)
  {
    hb_codepoint_t start_cp = 0, prev_run_start_cp = 0, run_start_cp = 0, end_cp = 0, last_gid = 0;
    પૂર્ણાંક run_length = 0 , delta = 0, prev_delta = 0;

    ગણના {
      FIRST_SUB_RANGE,
      FOLLOWING_SUB_RANGE,
    } mode;

    જ્યારે (it) {
      // Start a new range
      {
        અચલ auto& pair = *it;
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

      જ્યારે (it) {
        // Process range
        અચલ auto& pair = *it;
        hb_codepoint_t next_cp = pair.first;
        hb_codepoint_t next_gid = pair.second;
        જો (next_cp != end_cp + 1) {
          // Current range is over, stop processing.
          તોડો;
        }

        જો (next_gid == last_gid + 1) {
          // The current run continues.
          end_cp = next_cp;
          run_length++;
          last_gid = next_gid;
          it++;
          ચાલુ;
        }

        // A new run is starting, decide if we want to commit the current run.
        પૂર્ણાંક split_cost = (mode == FIRST_SUB_RANGE) ? 8 : 16;
        પૂર્ણાંક run_cost = run_length * 2;
        જો (run_cost >= split_cost) {
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

    જો (likely (end_cp != 0xFFFF)) {
      range_writer (0xFFFF, 0xFFFF, 1);
    }
  }

  /*
   * Writes the current range as either one or two ranges depending on what is most efficient.
   */
  template<typename Writer>
  શૂન્ય commit_current_range (hb_codepoint_t start,
                             hb_codepoint_t prev_run_start,
                             hb_codepoint_t run_start,
                             hb_codepoint_t end,
                             પૂર્ણાંક run_delta,
                             પૂર્ણાંક previous_run_delta,
                             પૂર્ણાંક split_cost,
                             Writer& range_writer) {
    તાર્કિક should_split = ખોટું;
    જો (start < run_start && run_start < end) {
      પૂર્ણાંક run_cost = (end - run_start + 1) * 2;
      જો (run_cost >= split_cost) {
        should_split = સાચું;
      }
    }

    // A zero delta is also the marker for ranges that need a
    // glyphIdArray; serialize_rangeoffset_glyid() tells the two cases
    // apart by re-checking the mapping.
    જો (should_split) {
      જો (start == prev_run_start)
        range_writer (start, run_start - 1, previous_run_delta);
      નહીંતર
        range_writer (start, run_start - 1, 0);
      range_writer (run_start, end, run_delta);
      પાછા;
    }


    જો (start == run_start) {
      // Range is only a run
      range_writer (start, end, run_delta);
      પાછા;
    }

    // Write only a single non-run range.
    range_writer (start, end, 0);
  }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  unsigned serialize_find_segcount (Iterator it) {
    સંરચના Counter {
      unsigned segcount = 0;

      શૂન્ય operator() (hb_codepoint_t start,
                       hb_codepoint_t end,
                       પૂર્ણાંક delta) {
        segcount++;
      }
    } counter;

    to_ranges (+it, counter);
    પાછા counter.segcount;
  }


  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  તાર્કિક serialize_start_end_delta_arrays (hb_serialize_context_t *c,
                                         Iterator it,
                                         પૂર્ણાંક segcount)
  {
    સંરચના Writer {
      hb_serialize_context_t *serializer_;
      HBUINT16* end_code_;
      HBUINT16* start_code_;
      HBINT16* id_delta_;
      પૂર્ણાંક index_;

      Writer(hb_serialize_context_t *serializer)
          : serializer_(serializer),
            end_code_(nullptr),
            start_code_(nullptr),
            id_delta_(nullptr),
            index_ (0) {}
      શૂન્ય operator() (hb_codepoint_t start,
                       hb_codepoint_t end,
                       પૂર્ણાંક delta) {
        start_code_[index_] = start;
        end_code_[index_] = end;
        id_delta_[index_] = delta;
        index_++;
      }
    } writer(c);

    writer.end_code_ = c->allocate_size<HBUINT16> (HBUINT16::static_size * segcount, ખોટું);
    (શૂન્ય) c->allocate_size<HBUINT16> (2); // padding
    writer.start_code_ = c->allocate_size<HBUINT16> (HBUINT16::static_size * segcount, ખોટું);
    writer.id_delta_ = c->allocate_size<HBINT16> (HBINT16::static_size * segcount, ખોટું);

    જો (unlikely (!writer.end_code_ || !writer.start_code_ || !writer.id_delta_)) પાછા ખોટું;

    to_ranges (+it, writer);
    પાછા સાચું;
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
    જો (unlikely (!c->check_success (idRangeOffset))) પાછા nullptr;
    જો (unlikely ((અક્ષર *)idRangeOffset - (અક્ષર *)idDelta != (પૂર્ણાંક) segcount * (પૂર્ણાંક) HBINT16::static_size)) પાછા nullptr;

    માટે (unsigned i : hb_range (segcount))
    {
      /* A non-zero delta is always a real run. A zero delta is
       * ambiguous: identity mapping, or the marker for a range that
       * needs a glyphIdArray. Only zero deltas need a check. */
      પૂર્ણાંક delta = idDelta[i];
      જો (delta)
	ચાલુ;
      તાર્કિક use_delta = સાચું;
      માટે (hb_codepoint_t cp = startCode[i]; cp <= endCode[i]; cp++)
      {
	hb_codepoint_t gid = cp_to_gid.get (cp);
	જો (gid == HB_MAP_કિંમત_INVALID)
	  gid = 0;
	જો (gid != cp)
	{
	  use_delta = ખોટું;
	  તોડો;
	}
      }
      જો (use_delta)
	ચાલુ;

      idRangeOffset[i] = 2 * (c->start_embed<HBUINT16> () - idRangeOffset - i);
      માટે (hb_codepoint_t cp = startCode[i]; cp <= endCode[i]; cp++)
      {
        HBUINT16 gid;
        gid = cp_to_gid[cp];
        c->copy<HBUINT16> (gid);
      }
    }

    પાછા idRangeOffset;
  }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  શૂન્ય serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    auto format4_iter =
    + it
    | hb_filter ([&] (અચલ hb_codepoint_pair_t _)
		 { પાછા _.first <= 0xFFFF; })
    ;

    જો (!format4_iter) પાછા;

    unsigned table_initpos = c->length ();
    જો (unlikely (!c->extend_min (આ))) પાછા;
    આ->format = 4;

    hb_vector_t<hb_codepoint_pair_t> cp_to_gid {
      format4_iter
    };

    //serialize endCode[], startCode[], idDelta[]
    HBUINT16* endCode = c->start_embed<HBUINT16> ();
    unsigned segcount = serialize_find_segcount (cp_to_gid.iter());
    જો (unlikely (!serialize_start_end_delta_arrays (c, cp_to_gid.iter(), segcount)))
      પાછા;

    HBUINT16 *startCode = endCode + segcount + 1;
    HBINT16 *idDelta = ((HBINT16*)startCode) + segcount;

    HBUINT16 *idRangeOffset = serialize_rangeoffset_glyid (c,
                                                           cp_to_gid.iter (),
                                                           endCode,
                                                           startCode,
                                                           idDelta,
                                                           segcount);
    જો (unlikely (!c->check_success (idRangeOffset))) પાછા;

    આ->length = c->length () - table_initpos;
    જો ((long long) આ->length != (long long) c->length () - table_initpos)
    {
      c->err (HB_SERIALIZE_ERROR_INT_OVERFLOW);
      પાછા;
    }

    આ->seg_ગણતરી_X2 = segcount * 2;
    આ->entrySelector = hb_max (1u, hb_bit_storage (segcount)) - 1;
    આ->શોધ_Range = 2 * (1u << આ->entrySelector);
    આ->rangeShift = segcount * 2 > આ->શોધ_Range
		       ? 2 * segcount - આ->શોધ_Range
		       : 0;
  }

  unsigned get_language () અચલ
  {
    પાછા language;
  }

  સંરચના accelerator_t
  {
    accelerator_t () {}
    accelerator_t (અચલ CmapSubtableFormat4 *subtable) = delete;

    શૂન્ય init (અચલ CmapSubtableFormat4 *subtable,
	       unsigned પૂર્ણાંક              subtable_data_size)
    {
      seg_ગણતરી_2 = subtable->seg_ગણતરી_X2 / 2;
      end_ગણતરી = subtable->values.arrayZ;
      start_ગણતરી = end_ગણતરી + seg_ગણતરી_2 + 1;
      idDelta = start_ગણતરી + seg_ગણતરી_2;
      idRangeOffset = idDelta + seg_ગણતરી_2;
      glyphIdArray = idRangeOffset + seg_ગણતરી_2;

      unsigned પૂર્ણાંક કિંમતો_offset = 16 + 8 * seg_ગણતરી_2;
      glyphIdArrayLength = subtable_data_size > કિંમતો_offset
			   ? (subtable_data_size - કિંમતો_offset) / 2
			   : 0;
    }

    તાર્કિક get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) અચલ
    {
      સંરચના CustomRange
      {
	પૂર્ણાંક cmp (hb_codepoint_t k,
		 unsigned distance) અચલ
	{
	  જો (k > last) પાછા +1;
	  જો (k < (&last)[distance]/*first*/) પાછા -1;
	  પાછા 0;
	}
	HBUINT16 last;
      };

      અચલ HBUINT16 *found = hb_bsearch (codepoint,
					  આ->end_ગણતરી,
					  આ->seg_ગણતરી_2,
					  કદ (CustomRange),
					  _hb_cmp_method<hb_codepoint_t, CustomRange, unsigned>,
					  આ->seg_ગણતરી_2 + 1);
      જો (unlikely (!found))
	પાછા ખોટું;
      unsigned પૂર્ણાંક i = found - end_ગણતરી;

      hb_codepoint_t gid;
      unsigned પૂર્ણાંક rangeOffset = આ->idRangeOffset[i];
      જો (rangeOffset == 0)
	gid = codepoint + આ->idDelta[i];
      નહીંતર
      {
	/* Somebody has been smoking... */
	unsigned પૂર્ણાંક index = rangeOffset / 2 + (codepoint - આ->start_ગણતરી[i]) + i - આ->seg_ગણતરી_2;
	જો (unlikely (index >= આ->glyphIdArrayLength))
	  પાછા ખોટું;
	gid = આ->glyphIdArray[index];
	જો (unlikely (!gid))
	  પાછા ખોટું;
	gid += આ->idDelta[i];
      }
      gid &= 0xFFFFu;
      જો (unlikely (!gid))
	પાછા ખોટું;
      *glyph = gid;
      પાછા સાચું;
    }

    HB_INTERNAL સ્થિર તાર્કિક get_glyph_func (અચલ શૂન્ય *obj, hb_codepoint_t codepoint, hb_codepoint_t *glyph)
    { પાછા ((અચલ accelerator_t *) obj)->get_glyph (codepoint, glyph); }

    શૂન્ય collect_unicodes (hb_set_t *out) અચલ
    {
      unsigned પૂર્ણાંક ગણતરી = આ->seg_ગણતરી_2;
      જો (ગણતરી && આ->start_ગણતરી[ગણતરી - 1] == 0xFFFFu)
	ગણતરી--; /* Skip sentinel segment. */
      માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
      {
	hb_codepoint_t start = આ->start_ગણતરી[i];
	hb_codepoint_t end = આ->end_ગણતરી[i];
	unsigned પૂર્ણાંક rangeOffset = આ->idRangeOffset[i];
        out->add_range(start, end);
	જો (rangeOffset == 0)
	{
	  માટે (hb_codepoint_t codepoint = start; codepoint <= end; codepoint++)
	  {
	    hb_codepoint_t gid = (codepoint + આ->idDelta[i]) & 0xFFFFu;
	    જો (unlikely (!gid))
              out->del(codepoint);
	  }
	}
	નહીંતર
	{
	  માટે (hb_codepoint_t codepoint = start; codepoint <= end; codepoint++)
	  {
	    unsigned પૂર્ણાંક index = rangeOffset / 2 + (codepoint - આ->start_ગણતરી[i]) + i - આ->seg_ગણતરી_2;
	    જો (unlikely (index >= આ->glyphIdArrayLength))
            {
              out->del_range (codepoint, end);
	      તોડો;
            }
	    hb_codepoint_t gid = આ->glyphIdArray[index];
	    જો (unlikely (!gid))
              out->del(codepoint);
	  }
	}
      }
    }

    શૂન્ય collect_mapping (hb_set_t *unicodes, /* OUT */
			  hb_map_t *mapping /* OUT */) અચલ
    {
      // TODO(grieger): optimize similar to collect_unicodes
      // (ie. use add_range())
      unsigned ગણતરી = આ->seg_ગણતરી_2;
      જો (ગણતરી && આ->start_ગણતરી[ગણતરી - 1] == 0xFFFFu)
	ગણતરી--; /* Skip sentinel segment. */
      માટે (unsigned i = 0; i < ગણતરી; i++)
      {
	hb_codepoint_t start = આ->start_ગણતરી[i];
	hb_codepoint_t end = આ->end_ગણતરી[i];
	unsigned rangeOffset = આ->idRangeOffset[i];
	જો (rangeOffset == 0)
	{
	  માટે (hb_codepoint_t codepoint = start; codepoint <= end; codepoint++)
	  {
	    hb_codepoint_t gid = (codepoint + આ->idDelta[i]) & 0xFFFFu;
	    જો (unlikely (!gid))
	      ચાલુ;
	    unicodes->add (codepoint);
	    mapping->set (codepoint, gid);
	  }
	}
	નહીંતર
	{
	  માટે (hb_codepoint_t codepoint = start; codepoint <= end; codepoint++)
	  {
	    unsigned index = rangeOffset / 2 + (codepoint - આ->start_ગણતરી[i]) + i - આ->seg_ગણતરી_2;
	    જો (unlikely (index >= આ->glyphIdArrayLength))
	      તોડો;
	    hb_codepoint_t gid = આ->glyphIdArray[index];
	    જો (unlikely (!gid))
	      ચાલુ;
            gid = (gid + આ->idDelta[i]) & 0xFFFFu;
	    unicodes->add (codepoint);
	    mapping->set (codepoint, gid);
	  }
	}
      }
    }

    અચલ HBUINT16 *end_ગણતરી;
    અચલ HBUINT16 *start_ગણતરી;
    અચલ HBUINT16 *idDelta;
    અચલ HBUINT16 *idRangeOffset;
    અચલ HBUINT16 *glyphIdArray;
    unsigned પૂર્ણાંક seg_ગણતરી_2;
    unsigned પૂર્ણાંક glyphIdArrayLength;
  };

  તાર્કિક get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph,
		  unsigned પૂર્ણાંક subtable_data_size) અચલ
  {
    accelerator_t accel;
    accel.init (આ, subtable_data_size);
    પાછા accel.get_glyph_func (&accel, codepoint, glyph);
  }
  તાર્કિક get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) અચલ
  { પાછા ખોટું; }

  શૂન્ય collect_unicodes (hb_set_t *out, unsigned પૂર્ણાંક subtable_data_size) અચલ
  {
    accelerator_t accel;
    accel.init (આ, subtable_data_size);
    accel.collect_unicodes (out);
  }
  શૂન્ય collect_unicodes (hb_set_t *out) અચલ
  { collect_unicodes (out, length); }

  શૂન્ય collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping, /* OUT */
			unsigned પૂર્ણાંક subtable_data_size) અચલ
  {
    accelerator_t accel;
    accel.init (આ, subtable_data_size);
    accel.collect_mapping (unicodes, mapping);
  }
  શૂન્ય collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping /* OUT */) અચલ
  { collect_mapping (unicodes, mapping, length); }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    જો (unlikely (!c->check_struct (આ)))
      return_trace (ખોટું);
    hb_barrier ();

    return_trace (c->check_range (values, 2 + 4 * seg_ગણતરી_X2));
  }

  સુરક્ષિત:
  HBUINT16	format;		/* Format number is set to 4. */
  HBUINT16	length;		/* This is the length in bytes of the
				 * subtable. */
  HBUINT16	language;	/* Ignore. */
  HBUINT16	seg_ગણતરી_X2;	/* 2 x segCount. */
  HBUINT16	શોધ_Range;	/* 2 * (2**floor(log2(segCount))) */
  HBUINT16	entrySelector;	/* log2(searchRange/2) */
  HBUINT16	rangeShift;	/* 2 x segCount - searchRange */

  UnsizedArrayOf<HBUINT16>
		values;
#જો 0
  HBUINT16	end_ગણતરી[seg_ગણતરી_2];	/* End characterCode for each segment,
					 * last=0xFFFFu. */
  HBUINT16	reservedPad;		/* Set to 0. */
  HBUINT16	start_ગણતરી[seg_ગણતરી_2];	/* Start character code for each segment. */
  HBINT16		idDelta[seg_ગણતરી_2];	/* Delta for all character codes in segment. */
  HBUINT16	idRangeOffset[seg_ગણતરી_2];/* Offsets into glyphIdArray or 0 */
  UnsizedArrayOf<HBUINT16>
		glyphIdArray;	/* Glyph index array (arbitrary length) */
#endif

  જાહેર:
  DEFINE_SIZE_ARRAY (14, values);
};

સંરચના CmapSubtableLongGroup
{
  friend સંરચના CmapSubtableFormat12;
  friend સંરચના CmapSubtableFormat13;
  template<typename U>
  friend સંરચના CmapSubtableLongSegmented;
  friend સંરચના cmap;

  પૂર્ણાંક cmp (hb_codepoint_t codepoint) અચલ
  {
    જો (codepoint < startCharCode) પાછા -1;
    જો (codepoint > endCharCode)   પાછા +1;
    પાછા 0;
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ));
  }

  ખાનગી:
  HBUINT32		startCharCode;	/* First character code in this group. */
  HBUINT32		endCharCode;	/* Last character code in this group. */
  HBUINT32		glyphID;	/* Glyph index; interpretation depends on
					 * subtable format. */
  જાહેર:
  DEFINE_SIZE_STATIC (12);
};
DECLARE_NULL_NAMESPACE_BYTES (OT, CmapSubtableLongGroup);

template <typename UINT>
સંરચના CmapSubtableTrimmed
{
  તાર્કિક get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) અચલ
  {
    /* Rely on our implicit array bound-checking. */
    hb_codepoint_t gid = glyphIdArray[codepoint - startCharCode];
    જો (unlikely (!gid))
      પાછા ખોટું;
    *glyph = gid;
    પાછા સાચું;
  }

  unsigned get_language () અચલ
  {
    પાછા language;
  }

  શૂન્ય collect_unicodes (hb_set_t *out) અચલ
  {
    hb_codepoint_t start = startCharCode;
    unsigned પૂર્ણાંક ગણતરી = glyphIdArray.len;
    માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
      જો (glyphIdArray[i])
	out->add (start + i);
  }

  શૂન્ય collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping /* OUT */) અચલ
  {
    hb_codepoint_t start_cp = startCharCode;
    unsigned ગણતરી = glyphIdArray.len;
    માટે (unsigned i = 0; i < ગણતરી; i++)
      જો (glyphIdArray[i])
      {
	hb_codepoint_t unicode = start_cp + i;
	hb_codepoint_t glyphid = glyphIdArray[i];
	unicodes->add (unicode);
	mapping->set (unicode, glyphid);
      }
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) && glyphIdArray.sanitize (c));
  }

  સુરક્ષિત:
  UINT		formatReserved;	/* Subtable format and (maybe) padding. */
  UINT		length;		/* Byte length of this subtable. */
  UINT		language;	/* Ignore. */
  UINT		startCharCode;	/* First character code covered. */
  ArrayOf<HBGlyphID16, UINT>
		glyphIdArray;	/* Array of glyph index values for character
				 * codes in the range. */
  જાહેર:
  DEFINE_SIZE_ARRAY (5 * કદ (UINT), glyphIdArray);
};

સંરચના CmapSubtableFormat6  : CmapSubtableTrimmed<HBUINT16> {};
સંરચના CmapSubtableFormat10 : CmapSubtableTrimmed<HBUINT32> {};

template <typename T>
સંરચના CmapSubtableLongSegmented
{
  friend સંરચના cmap;

  તાર્કિક get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) અચલ
  {
    hb_codepoint_t gid = T::group_get_glyph (groups.bsearch (codepoint), codepoint);
    જો (unlikely (!gid))
      પાછા ખોટું;
    *glyph = gid;
    પાછા સાચું;
  }

  unsigned get_language () અચલ
  {
    પાછા language;
  }

  શૂન્ય collect_unicodes (hb_set_t *out, unsigned પૂર્ણાંક અંક_glyphs) અચલ
  {
    માટે (unsigned પૂર્ણાંક i = 0; i < આ->groups.len; i++)
    {
      hb_codepoint_t start = આ->groups[i].startCharCode;
      hb_codepoint_t end = hb_min ((hb_codepoint_t) આ->groups[i].endCharCode,
				   (hb_codepoint_t) HB_UNICODE_MAX);
      hb_codepoint_t gid = આ->groups[i].glyphID;
      જો (!gid)
      {
	/* Intention is: if (hb_is_same (T, CmapSubtableFormat13)) continue; */
	જો (! T::group_get_glyph (આ->groups[i], end)) ચાલુ;
	start++;
	gid++;
      }
      જો (unlikely ((unsigned પૂર્ણાંક) gid >= અંક_glyphs)) ચાલુ;
      જો (unlikely ((unsigned પૂર્ણાંક) (gid + end - start) >= અંક_glyphs))
	end = start + (hb_codepoint_t) અંક_glyphs - gid;

      out->add_range (start, hb_min (end, 0x10FFFFu));
    }
  }

  શૂન્ય collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping, /* OUT */
			unsigned અંક_glyphs) અચલ
  {
    hb_codepoint_t last_end = 0;
    unsigned ગણતરી = આ->groups.len;
    માટે (unsigned i = 0; i < ગણતરી; i++)
    {
      hb_codepoint_t start = આ->groups.arrayZ[i].startCharCode;
      hb_codepoint_t end = hb_min ((hb_codepoint_t) આ->groups.arrayZ[i].endCharCode,
				   (hb_codepoint_t) HB_UNICODE_MAX);
      જો (unlikely (start > end || start < last_end)) {
        // Range is not in order and is invalid, skip it.
        ચાલુ;
      }
      last_end = end;


      hb_codepoint_t gid = આ->groups.arrayZ[i].glyphID;
      જો (!gid)
      {
        જો (T::format_સંખ્યા == 13) ચાલુ;
	start++;
	gid++;
      }
      જો (unlikely ((unsigned પૂર્ણાંક) gid >= અંક_glyphs)) ચાલુ;
      જો (unlikely ((unsigned પૂર્ણાંક) (gid + end - start) >= અંક_glyphs))
	end = start + (hb_codepoint_t) અંક_glyphs - gid;

      mapping->alloc (mapping->get_population () + end - start + 1);

      unicodes->add_range (start, end);
      માટે (unsigned cp = start; cp <= end; cp++)
      {
	mapping->set (cp, gid);
        gid += T::increment;
      }
    }
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) && groups.sanitize (c));
  }

  સુરક્ષિત:
  HBUINT16	format;		/* Subtable format; set to 12. */
  HBUINT16	reserved;	/* Reserved; set to 0. */
  HBUINT32	length;		/* Byte length of this subtable. */
  HBUINT32	language;	/* Ignore. */
  SortedArray32Of<CmapSubtableLongGroup>
		groups;		/* Groupings. */
  જાહેર:
  DEFINE_SIZE_ARRAY (16, groups);
};

સંરચના CmapSubtableFormat12 : CmapSubtableLongSegmented<CmapSubtableFormat12>
{
  સ્થિર constexpr પૂર્ણાંક increment = 1;
  સ્થિર constexpr પૂર્ણાંક format_સંખ્યા = 12;

  સ્થિર hb_codepoint_t group_get_glyph (અચલ CmapSubtableLongGroup &group,
					 hb_codepoint_t u)
  { પાછા likely (group.startCharCode <= group.endCharCode) ?
	   group.glyphID + (u - group.startCharCode) : 0; }


  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  શૂન્ય serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    જો (!it) પાછા;
    unsigned table_initpos = c->length ();
    જો (unlikely (!c->extend_min (આ))) પાછા;

    hb_codepoint_t startCharCode = (hb_codepoint_t) -1, endCharCode = (hb_codepoint_t) -1;
    hb_codepoint_t glyphID = 0;

    માટે (અચલ auto& _ : +it)
    {
      જો (startCharCode == (hb_codepoint_t) -1)
      {
	startCharCode = _.first;
	endCharCode = _.first;
	glyphID = _.second;
      }
      નહીંતર જો (!_is_gid_consecutive (endCharCode, startCharCode, glyphID, _.first, _.second))
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
      નહીંતર
	endCharCode = _.first;
    }

    CmapSubtableLongGroup record;
    record.startCharCode = startCharCode;
    record.endCharCode = endCharCode;
    record.glyphID = glyphID;
    c->copy<CmapSubtableLongGroup> (record);

    આ->format = 12;
    આ->reserved = 0;
    આ->length = c->length () - table_initpos;
    આ->groups.len = (આ->length - min_size) / CmapSubtableLongGroup::static_size;
  }

  સ્થિર size_t get_sub_table_size (અચલ hb_sorted_vector_t<CmapSubtableLongGroup> &groups_data)
  { પાછા 16 + 12 * groups_data.length; }

  ખાનગી:
  સ્થિર તાર્કિક _is_gid_consecutive (hb_codepoint_t endCharCode,
				   hb_codepoint_t startCharCode,
				   hb_codepoint_t glyphID,
				   hb_codepoint_t cp,
				   hb_codepoint_t new_gid)
  {
    પાછા (cp - 1 == endCharCode) &&
	new_gid == glyphID + (cp - startCharCode);
  }

};

સંરચના CmapSubtableFormat13 : CmapSubtableLongSegmented<CmapSubtableFormat13>
{
  સ્થિર constexpr પૂર્ણાંક increment = 0;
  સ્થિર constexpr પૂર્ણાંક format_સંખ્યા = 13;

  સ્થિર hb_codepoint_t group_get_glyph (અચલ CmapSubtableLongGroup &group,
					 hb_codepoint_t u HB_UNUSED)
  { પાછા group.glyphID; }
};

પ્રકારવ્યાખ્યા ગણના
{
  GLYPH_VARIANT_NOT_FOUND = 0,
  GLYPH_VARIANT_FOUND = 1,
  GLYPH_VARIANT_USE_DEFAULT = 2
} glyph_variant_t;

સંરચના Unicode_કિંમત_Range
{
  પૂર્ણાંક cmp (અચલ hb_codepoint_t &codepoint) અચલ
  {
    જો (codepoint < start_Unicode_કિંમત) પાછા -1;
    જો (codepoint > start_Unicode_કિંમત + additional_ગણતરી) પાછા +1;
    પાછા 0;
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ));
  }

  HBUINT24	start_Unicode_કિંમત;	/* First value in this range. */
  HBUINT8	additional_ગણતરી;	/* Number of additional values in this
					 * range. */
  જાહેર:
  DEFINE_SIZE_STATIC (4);
};

સંરચના DefaultUVS : SortedArray32Of<Unicode_કિંમત_Range>
{
  શૂન્ય collect_unicodes (hb_set_t *out) અચલ
  {
    unsigned પૂર્ણાંક ગણતરી = len;
    માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
    {
      hb_codepoint_t first = arrayZ[i].start_Unicode_કિંમત;
      hb_codepoint_t last = hb_min ((hb_codepoint_t) (first + arrayZ[i].additional_ગણતરી),
				    (hb_codepoint_t) HB_UNICODE_MAX);
      out->add_range (first, last);
    }
  }

  DefaultUVS* copy (hb_serialize_context_t *c,
		    અચલ hb_set_t *unicodes) અચલ
  {
    auto *out = c->start_embed<DefaultUVS> ();
    auto snap = c->snapshot ();

    HBUINT32 len;
    len = 0;
    જો (unlikely (!c->copy<HBUINT32> (len))) પાછા nullptr;
    unsigned init_len = c->length ();

    જો (આ->len > unicodes->get_population () * hb_bit_storage ((unsigned) આ->len))
    {
      hb_codepoint_t start = HB_SET_કિંમત_INVALID;
      hb_codepoint_t end = HB_SET_કિંમત_INVALID;

      માટે (auto u : *unicodes)
      {
        જો (!as_array ().bsearch (u))
	  ચાલુ;
	જો (start == HB_SET_કિંમત_INVALID)
	{
	  start = u;
	  end = start - 1;
	}
	જો (end + 1 != u || end - start == 255)
        {
	  Unicode_કિંમત_Range rec;
	  rec.start_Unicode_કિંમત = start;
	  rec.additional_ગણતરી = end - start;
	  c->copy<Unicode_કિંમત_Range> (rec);
	  start = u;
	}
	end = u;
      }
      જો (start != HB_SET_કિંમત_INVALID)
      {
	Unicode_કિંમત_Range rec;
	rec.start_Unicode_કિંમત = start;
	rec.additional_ગણતરી = end - start;
	c->copy<Unicode_કિંમત_Range> (rec);
      }

    }
    નહીંતર
    {
      hb_codepoint_t lastCode = HB_SET_કિંમત_INVALID;
      પૂર્ણાંક ગણતરી = -1;

      માટે (અચલ Unicode_કિંમત_Range& _ : *આ)
      {
	hb_codepoint_t curEntry = (hb_codepoint_t) (_.start_Unicode_કિંમત - 1);
	hb_codepoint_t end = curEntry + _.additional_ગણતરી + 2;

	માટે (; unicodes->next (&curEntry) && curEntry < end;)
	{
	  ગણતરી += 1;
	  જો (lastCode == HB_SET_કિંમત_INVALID)
	    lastCode = curEntry;
	  નહીંતર જો (lastCode + ગણતરી != curEntry || ગણતરી > 255)
	  {
	    Unicode_કિંમત_Range rec;
	    rec.start_Unicode_કિંમત = lastCode;
	    rec.additional_ગણતરી = ગણતરી - 1;
	    c->copy<Unicode_કિંમત_Range> (rec);

	    lastCode = curEntry;
	    ગણતરી = 0;
	  }
	}
      }

      જો (lastCode != HB_MAP_કિંમત_INVALID)
      {
	Unicode_કિંમત_Range rec;
	rec.start_Unicode_કિંમત = lastCode;
	rec.additional_ગણતરી = ગણતરી;
	c->copy<Unicode_કિંમત_Range> (rec);
      }
    }

    જો (c->length () - init_len == 0)
    {
      c->revert (snap);
      પાછા nullptr;
    }
    નહીંતર
    {
      જો (unlikely (!c->check_assign (out->len,
                                      (c->length () - init_len) / Unicode_કિંમત_Range::static_size,
                                      HB_SERIALIZE_ERROR_INT_OVERFLOW))) પાછા nullptr;
      પાછા out;
    }
  }

  જાહેર:
  DEFINE_SIZE_ARRAY (4, *આ);
};

template <typename Types>
સંરચના UVSMapping
{
  using GlyphID = typename Types::HBGlyphID;

  પૂર્ણાંક cmp (અચલ hb_codepoint_t &codepoint) અચલ
  { પાછા unicode_કિંમત.cmp (codepoint); }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ));
  }

  HBUINT24	unicode_કિંમત;	/* Base Unicode value of the UVS */
  GlyphID
		glyphID;	/* Glyph ID of the UVS. */
  જાહેર:
  DEFINE_SIZE_STATIC (3 + GlyphID::static_size);
};

template <typename Types>
સંરચના NonDefaultUVS : SortedArray32Of<UVSMapping<Types>>
{
  using Mapping = UVSMapping<Types>;

  શૂન્ય collect_unicodes (hb_set_t *out) અચલ
  {
    માટે (અચલ auto& a : આ->as_array ())
      out->add (a.unicode_કિંમત);
  }

  શૂન્ય collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping /* OUT */) અચલ
  {
    માટે (અચલ auto& a : આ->as_array ())
    {
      hb_codepoint_t unicode = a.unicode_કિંમત;
      hb_codepoint_t glyphid = a.glyphID;
      unicodes->add (unicode);
      mapping->set (unicode, glyphid);
    }
  }

  શૂન્ય closure_glyphs (અચલ hb_set_t      *unicodes,
		       hb_set_t            *glyphset) અચલ
  {
    + આ->as_array ()
    | hb_filter (unicodes, &Mapping::unicode_કિંમત)
    | hb_map (&Mapping::glyphID)
    | hb_sink (glyphset)
    ;
  }

  NonDefaultUVS* copy (hb_serialize_context_t *c,
		       અચલ hb_set_t *unicodes,
		       અચલ hb_set_t *glyphs_requested,
		       અચલ hb_map_t *glyph_map) અચલ
  {
    auto *out = c->start_embed<NonDefaultUVS> ();
    auto it =
    + આ->as_array ()
    | hb_filter ([&] (અચલ Mapping& _)
		 {
		   પાછા unicodes->has (_.unicode_કિંમત) || glyphs_requested->has (_.glyphID);
		 })
    ;

    જો (!it) પાછા nullptr;

    HBUINT32 len;
    len = it.len ();
    જો (unlikely (!c->copy<HBUINT32> (len))) પાછા nullptr;

    માટે (અચલ Mapping& _ : it)
    {
      Mapping mapping;
      mapping.unicode_કિંમત = _.unicode_કિંમત;
      mapping.glyphID = glyph_map->get (_.glyphID);
      c->copy<Mapping> (mapping);
    }

    પાછા out;
  }

  જાહેર:
  DEFINE_SIZE_ARRAY (4, *આ);
};

template <typename Types>
સંરચના VariationSelectorRecord
{
  glyph_variant_t get_glyph (hb_codepoint_t codepoint,
			     hb_codepoint_t *glyph,
			     અચલ શૂન્ય *base) અચલ
  {
    જો ((base+defaultUVS).bfind (codepoint))
      પાછા GLYPH_VARIANT_USE_DEFAULT;
    અચલ UVSMapping<Types> &nonDefault = (base+nonDefaultUVS).bsearch (codepoint);
    જો (nonDefault.glyphID)
    {
      *glyph = nonDefault.glyphID;
       પાછા GLYPH_VARIANT_FOUND;
    }
    પાછા GLYPH_VARIANT_NOT_FOUND;
  }

  VariationSelectorRecord(અચલ VariationSelectorRecord& other)
  {
    *આ = other;
  }

  શૂન્ય operator= (અચલ VariationSelectorRecord& other)
  {
    varSelector = other.varSelector;
    HBUINT32 offset = other.defaultUVS;
    defaultUVS = offset;
    offset = other.nonDefaultUVS;
    nonDefaultUVS = offset;
  }

  શૂન્ય collect_unicodes (hb_set_t *out, અચલ શૂન્ય *base) અચલ
  {
    (base+defaultUVS).collect_unicodes (out);
    (base+nonDefaultUVS).collect_unicodes (out);
  }

  શૂન્ય collect_mapping (અચલ શૂન્ય *base,
			hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping /* OUT */) અચલ
  {
    (base+defaultUVS).collect_unicodes (unicodes);
    (base+nonDefaultUVS).collect_mapping (unicodes, mapping);
  }

  પૂર્ણાંક cmp (અચલ hb_codepoint_t &variation_selector) અચલ
  { પાછા varSelector.cmp (variation_selector); }

  તાર્કિક sanitize (hb_sanitize_context_t *c, અચલ શૂન્ય *base) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) &&
		  defaultUVS.sanitize (c, base) &&
		  nonDefaultUVS.sanitize (c, base));
  }

  hb_pair_t<unsigned, unsigned>
  copy (hb_serialize_context_t *c,
	અચલ hb_set_t *unicodes,
	અચલ hb_set_t *glyphs_requested,
	અચલ hb_map_t *glyph_map,
	અચલ શૂન્ય *base) અચલ
  {
    auto snap = c->snapshot ();
    auto *out = c->embed<VariationSelectorRecord> (*આ);
    જો (unlikely (!out)) પાછા hb_pair (0, 0);

    out->defaultUVS = 0;
    out->nonDefaultUVS = 0;

    unsigned non_default_uvs_objidx = 0;
    જો (nonDefaultUVS != 0)
    {
      c->push ();
      જો (c->copy (base+nonDefaultUVS, unicodes, glyphs_requested, glyph_map))
	non_default_uvs_objidx = c->કાઢો_pack ();
      નહીંતર c->કાઢો_discard ();
    }

    unsigned default_uvs_objidx = 0;
    જો (defaultUVS != 0)
    {
      c->push ();
      જો (c->copy (base+defaultUVS, unicodes))
	default_uvs_objidx = c->કાઢો_pack ();
      નહીંતર c->કાઢો_discard ();
    }


    જો (!default_uvs_objidx && !non_default_uvs_objidx)
      c->revert (snap);

    પાછા hb_pair (default_uvs_objidx, non_default_uvs_objidx);
  }

  HBUINT24	varSelector;	/* Variation selector. */
  Offset32To<DefaultUVS>
		defaultUVS;	/* Offset to Default UVS Table.  May be 0. */
  Offset32To<NonDefaultUVS<Types>>
		nonDefaultUVS;	/* Offset to Non-Default UVS Table.  May be 0. */
  જાહેર:
  DEFINE_SIZE_STATIC (11);
};

template <typename Types>
સંરચના CmapSubtableFormat14_15
{
  સ્થિર constexpr unsigned format_કિંમત = Types::size == 2 ? 14 : 15;
  using Record = VariationSelectorRecord<Types>;
  using NonDefault = NonDefaultUVS<Types>;

  glyph_variant_t get_glyph_variant (hb_codepoint_t codepoint,
				     hb_codepoint_t variation_selector,
				     hb_codepoint_t *glyph) અચલ
  { પાછા record.bsearch (variation_selector).get_glyph (codepoint, glyph, આ); }

  શૂન્ય collect_variation_selectors (hb_set_t *out) અચલ
  {
    માટે (અચલ auto& a : record.as_array ())
      out->add (a.varSelector);
  }
  શૂન્ય collect_variation_unicodes (hb_codepoint_t variation_selector,
				   hb_set_t *out) અચલ
  { record.bsearch (variation_selector).collect_unicodes (out, આ); }

  શૂન્ય serialize (hb_serialize_context_t *c,
		  અચલ hb_set_t *unicodes,
		  અચલ hb_set_t *glyphs_requested,
		  અચલ hb_map_t *glyph_map,
		  અચલ શૂન્ય *base)
  {
    auto snap = c->snapshot ();
    unsigned table_initpos = c->length ();
    અચલ અક્ષર* init_tail = c->tail;

    જો (unlikely (!c->extend_min (આ))) પાછા;
    આ->format = format_કિંમત;

    auto src_tbl = reinterpret_cast<અચલ CmapSubtableFormat14_15*> (base);

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
    માટે (પૂર્ણાંક i = src_tbl->record.len - 1; i >= 0; i--)
    {
      જો (!unicodes->has(src_tbl->record[i].varSelector))
        ચાલુ;

      hb_pair_t<unsigned, unsigned> result = src_tbl->record[i].copy (c, unicodes, glyphs_requested, glyph_map, base);
      જો (result.first || result.second)
	obj_indices.push (result);
    }

    જો (c->length () - table_initpos == CmapSubtableFormat14_15::min_size)
    {
      c->revert (snap);
      પાછા;
    }

    જો (unlikely (!c->check_success (!obj_indices.in_error ())))
      પાછા;

    પૂર્ણાંક tail_len = init_tail - c->tail;
    c->check_assign (આ->length, c->length () - table_initpos + tail_len,
                     HB_SERIALIZE_ERROR_INT_OVERFLOW);
    c->check_assign (આ->record.len,
		     (c->length () - table_initpos - CmapSubtableFormat14_15::min_size) /
		     Record::static_size,
                     HB_SERIALIZE_ERROR_INT_OVERFLOW);

    /* Correct the incorrect write order by reversing the order of the variation
       records array. */
    _reverse_variation_records ();

    /* Now that records are in the right order, we can set up the offsets. */
    _add_links_to_variation_records (c, obj_indices);
  }

  શૂન્ય _reverse_variation_records ()
  {
    record.as_array ().reverse ();
  }

  શૂન્ય _add_links_to_variation_records (hb_serialize_context_t *c,
					અચલ hb_vector_t<hb_pair_t<unsigned, unsigned>>& obj_indices)
  {
    માટે (unsigned i = 0; i < obj_indices.length; i++)
    {
      /*
       * Since the record array has been reversed (see comments in copy())
       * but obj_indices has not been, the indices at obj_indices[i]
       * are for the variation record at record[j].
       */
      પૂર્ણાંક j = obj_indices.length - 1 - i;
      c->add_link (record[j].defaultUVS, obj_indices[i].first);
      c->add_link (record[j].nonDefaultUVS, obj_indices[i].second);
    }
  }

  શૂન્ય closure_glyphs (અચલ hb_set_t      *unicodes,
		       hb_set_t            *glyphset) અચલ
  {
    + hb_iter (record)
    | hb_filter (hb_bool, &Record::nonDefaultUVS)
    | hb_filter (unicodes, &Record::varSelector)
    | hb_map (&Record::nonDefaultUVS)
    | hb_map (hb_add (આ))
    | hb_apply ([=] (અચલ NonDefault& _) { _.closure_glyphs (unicodes, glyphset); })
    ;
  }

  શૂન્ય collect_unicodes (hb_set_t *out) અચલ
  {
    માટે (અચલ Record& _ : record)
      _.collect_unicodes (out, આ);
  }

  શૂન્ય collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping /* OUT */) અચલ
  {
    માટે (અચલ Record& _ : record)
      _.collect_mapping (આ, unicodes, mapping);
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) &&
		  record.sanitize (c, આ));
  }

  સુરક્ષિત:
  HBUINT16	format;		/* Format number is set to 14 or 15. */
  HBUINT32	length;		/* Byte length of this subtable. */
  SortedArray32Of<Record>
		record;		/* Variation selector records; sorted
				 * in increasing order of `varSelector'. */
  જાહેર:
  DEFINE_SIZE_ARRAY (10, record);
};

સંરચના CmapSubtable
{
  /* Note: We intentionally do NOT implement subtable formats 2 and 8. */

  તાર્કિક is_variation_selector () અચલ
  {
    પસંદગી (u.format.v) {
    સ્થિતિ 14:
#ifndef HB_NO_BEYOND_64K
    સ્થિતિ 15:
#endif
      પાછા સાચું;
    મૂળભૂત:
      પાછા ખોટું;
    }
  }

  glyph_variant_t get_glyph_variant (hb_codepoint_t codepoint,
				     hb_codepoint_t variation_selector,
				     hb_codepoint_t *glyph) અચલ
  {
    પસંદગી (u.format.v) {
    સ્થિતિ 14: hb_barrier (); પાછા u.format14.get_glyph_variant (codepoint, variation_selector, glyph);
#ifndef HB_NO_BEYOND_64K
    સ્થિતિ 15: hb_barrier (); પાછા u.format15.get_glyph_variant (codepoint, variation_selector, glyph);
#endif
    મૂળભૂત: પાછા GLYPH_VARIANT_NOT_FOUND;
    }
  }

  શૂન્ય collect_variation_selectors (hb_set_t *out) અચલ
  {
    પસંદગી (u.format.v) {
    સ્થિતિ 14: hb_barrier (); u.format14.collect_variation_selectors (out); પાછા;
#ifndef HB_NO_BEYOND_64K
    સ્થિતિ 15: hb_barrier (); u.format15.collect_variation_selectors (out); પાછા;
#endif
    મૂળભૂત: પાછા;
    }
  }

  શૂન્ય collect_variation_unicodes (hb_codepoint_t variation_selector,
				    hb_set_t *out) અચલ
  {
    પસંદગી (u.format.v) {
    સ્થિતિ 14: hb_barrier (); u.format14.collect_variation_unicodes (variation_selector, out); પાછા;
#ifndef HB_NO_BEYOND_64K
    સ્થિતિ 15: hb_barrier (); u.format15.collect_variation_unicodes (variation_selector, out); પાછા;
#endif
    મૂળભૂત: પાછા;
    }
  }

  શૂન્ય closure_glyphs (અચલ hb_set_t *unicodes, hb_set_t *glyphset) અચલ
  {
    પસંદગી (u.format.v) {
    સ્થિતિ 14: hb_barrier (); u.format14.closure_glyphs (unicodes, glyphset); પાછા;
#ifndef HB_NO_BEYOND_64K
    સ્થિતિ 15: hb_barrier (); u.format15.closure_glyphs (unicodes, glyphset); પાછા;
#endif
    મૂળભૂત: પાછા;
    }
  }

  તાર્કિક get_glyph (hb_codepoint_t codepoint,
		  hb_codepoint_t *glyph,
		  unsigned પૂર્ણાંક subtable_data_size = 0) અચલ
  {
    પસંદગી (u.format.v) {
    સ્થિતિ  0: hb_barrier (); પાછા u.format0 .get_glyph (codepoint, glyph);
    સ્થિતિ  4: hb_barrier (); પાછા subtable_data_size
				      ? u.format4.get_glyph (codepoint, glyph, subtable_data_size)
				      : ખોટું;
    સ્થિતિ  6: hb_barrier (); પાછા u.format6 .get_glyph (codepoint, glyph);
    સ્થિતિ 10: hb_barrier (); પાછા u.format10.get_glyph (codepoint, glyph);
    સ્થિતિ 12: hb_barrier (); પાછા u.format12.get_glyph (codepoint, glyph);
    સ્થિતિ 13: hb_barrier (); પાછા u.format13.get_glyph (codepoint, glyph);
    સ્થિતિ 14:
    મૂળભૂત: પાછા ખોટું;
    }
  }
  શૂન્ય collect_unicodes (hb_set_t *out,
			 unsigned પૂર્ણાંક અંક_glyphs = UINT_MAX,
			 unsigned પૂર્ણાંક subtable_data_size = 0) અચલ
  {
    પસંદગી (u.format.v) {
    સ્થિતિ  0: hb_barrier (); u.format0 .collect_unicodes (out); પાછા;
    સ્થિતિ  4: hb_barrier (); જો (subtable_data_size)
			      u.format4.collect_unicodes (out, subtable_data_size);
			    નહીંતર
			      u.format4.collect_unicodes (out);
			    પાછા;
    સ્થિતિ  6: hb_barrier (); u.format6 .collect_unicodes (out); પાછા;
    સ્થિતિ 10: hb_barrier (); u.format10.collect_unicodes (out); પાછા;
    સ્થિતિ 12: hb_barrier (); u.format12.collect_unicodes (out, અંક_glyphs); પાછા;
    સ્થિતિ 13: hb_barrier (); u.format13.collect_unicodes (out, અંક_glyphs); પાછા;
    સ્થિતિ 14:
    મૂળભૂત: પાછા;
    }
  }

  શૂન્ય collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping, /* OUT */
			unsigned અંક_glyphs = UINT_MAX,
			unsigned પૂર્ણાંક subtable_data_size = 0) અચલ
  {
    પસંદગી (u.format.v) {
    સ્થિતિ  0: hb_barrier (); u.format0 .collect_mapping (unicodes, mapping); પાછા;
    સ્થિતિ  4: hb_barrier (); જો (subtable_data_size)
			      u.format4.collect_mapping (unicodes, mapping, subtable_data_size);
			    નહીંતર
			      u.format4.collect_mapping (unicodes, mapping);
			    પાછા;
    સ્થિતિ  6: hb_barrier (); u.format6 .collect_mapping (unicodes, mapping); પાછા;
    સ્થિતિ 10: hb_barrier (); u.format10.collect_mapping (unicodes, mapping); પાછા;
    સ્થિતિ 12: hb_barrier (); u.format12.collect_mapping (unicodes, mapping, અંક_glyphs); પાછા;
    સ્થિતિ 13: hb_barrier (); u.format13.collect_mapping (unicodes, mapping, અંક_glyphs); પાછા;
    સ્થિતિ 14:
    મૂળભૂત: પાછા;
    }
  }

  unsigned get_language () અચલ
  {
    પસંદગી (u.format.v) {
    સ્થિતિ  0: hb_barrier (); પાછા u.format0 .get_language ();
    સ્થિતિ  4: hb_barrier (); પાછા u.format4 .get_language ();
    સ્થિતિ  6: hb_barrier (); પાછા u.format6 .get_language ();
    સ્થિતિ 10: hb_barrier (); પાછા u.format10.get_language ();
    સ્થિતિ 12: hb_barrier (); પાછા u.format12.get_language ();
    સ્થિતિ 13: hb_barrier (); પાછા u.format13.get_language ();
    સ્થિતિ 14:
    મૂળભૂત: પાછા 0;
    }
  }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  શૂન્ય serialize (hb_serialize_context_t *c,
		  Iterator it,
		  unsigned format,
		  અચલ hb_subset_plan_t *plan,
		  અચલ શૂન્ય *base)
  {
    પસંદગી (format) {
    સ્થિતિ  4: hb_barrier (); પાછા u.format4.serialize (c, it);
    સ્થિતિ 12: hb_barrier (); પાછા u.format12.serialize (c, it);
    સ્થિતિ 14: hb_barrier (); પાછા u.format14.serialize (c, &plan->unicodes, &plan->glyphs_requested, plan->glyph_map, base);
#ifndef HB_NO_BEYOND_64K
    સ્થિતિ 15: hb_barrier (); પાછા u.format15.serialize (c, &plan->unicodes, &plan->glyphs_requested, plan->glyph_map, base);
#endif
    મૂળભૂત: પાછા;
    }
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    જો (!u.format.v.sanitize (c)) return_trace (ખોટું);
    hb_barrier ();
    પસંદગી (u.format.v) {
    સ્થિતિ  0: hb_barrier (); return_trace (u.format0 .sanitize (c));
    સ્થિતિ  4: hb_barrier (); return_trace (u.format4 .sanitize (c));
    સ્થિતિ  6: hb_barrier (); return_trace (u.format6 .sanitize (c));
    સ્થિતિ 10: hb_barrier (); return_trace (u.format10.sanitize (c));
    સ્થિતિ 12: hb_barrier (); return_trace (u.format12.sanitize (c));
    સ્થિતિ 13: hb_barrier (); return_trace (u.format13.sanitize (c));
    સ્થિતિ 14: hb_barrier (); return_trace (u.format14.sanitize (c));
#ifndef HB_NO_BEYOND_64K
    સ્થિતિ 15: hb_barrier (); return_trace (u.format15.sanitize (c));
#endif
    મૂળભૂત:return_trace (સાચું);
    }
  }

  જાહેર:
  સંઘ {
  સંરચના { HBUINT16 v; }	format;		/* Format identifier */
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
  જાહેર:
  DEFINE_SIZE_UNION (2, format.v);
};


સંરચના EncodingRecord
{
  પૂર્ણાંક cmp (અચલ EncodingRecord &other) અચલ
  {
    પૂર્ણાંક ret;
    ret = platformID.cmp (other.platformID);
    જો (ret) પાછા ret;
    જો (other.encodingID != 0xFFFF)
    {
      ret = encodingID.cmp (other.encodingID);
      જો (ret) પાછા ret;
    }
    પાછા 0;
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c, અચલ શૂન્ય *base) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) &&
		  subtable.sanitize (c, base));
  }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  EncodingRecord* copy (hb_serialize_context_t *c,
			Iterator it,
			unsigned format,
			અચલ શૂન્ય *base,
			અચલ hb_subset_plan_t *plan,
			/* INOUT */ unsigned *objidx) અચલ
  {
    TRACE_SERIALIZE (આ);
    auto snap = c->snapshot ();
    auto *out = c->embed (આ);
    જો (unlikely (!out)) return_trace (nullptr);
    out->subtable = 0;

    જો (*objidx == 0)
    {
      CmapSubtable *cmapsubtable = c->push<CmapSubtable> ();
      unsigned origin_length = c->length ();
      cmapsubtable->serialize (c, it, format, plan, &(base+subtable));
      જો (c->length () > origin_length && !c->in_error()) *objidx = c->કાઢો_pack ();
      નહીંતર c->કાઢો_discard ();
    }

    જો (*objidx == 0)
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
  જાહેર:
  DEFINE_SIZE_STATIC (8);
};

સંરચના cmap;

સંરચના SubtableUnicodesCache {

 ખાનગી:
  hb_blob_ptr_t<cmap> base_blob;
  અચલ અક્ષર* base;
  unsigned પૂર્ણાંક table_length;
  hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> cached_unicodes;

 જાહેર:

  સ્થિર SubtableUnicodesCache* create (hb_blob_ptr_t<cmap> source_table)
  {
    SubtableUnicodesCache* cache =
        (SubtableUnicodesCache*) hb_malloc (કદ(SubtableUnicodesCache));

    જો (unlikely (!cache))
      પાછા nullptr;

    નવું (cache) SubtableUnicodesCache (source_table);
    પાછા cache;
  }

  સ્થિર શૂન્ય destroy (શૂન્ય* value) {
    જો (!value) પાછા;

    SubtableUnicodesCache* cache = (SubtableUnicodesCache*) value;
    cache->~SubtableUnicodesCache ();
    hb_free (cache);
  }

  SubtableUnicodesCache(અચલ શૂન્ય* cmap_base,
			unsigned પૂર્ણાંક table_length_ = 0)
      : base_blob(),
        base ((અચલ અક્ષર*) cmap_base),
        table_length (table_length_),
        cached_unicodes ()
  {}

  SubtableUnicodesCache(hb_blob_ptr_t<cmap> base_blob_)
      : base_blob(base_blob_),
        base ((અચલ અક્ષર *) base_blob.get()),
        table_length (base_blob.get_length ()),
        cached_unicodes ()
  {}

  ~SubtableUnicodesCache()
  {
    base_blob.destroy ();
  }

  તાર્કિક same_base(અચલ શૂન્ય* other) અચલ
  {
    પાછા other == (અચલ શૂન્ય*) base;
  }

  અચલ hb_set_t* set_for (અચલ EncodingRecord* record,
                           SubtableUnicodesCache& mutable_cache) અચલ
  {
    જો (cached_unicodes.has ((unsigned) ((અચલ અક્ષર *) record - base)))
      પાછા cached_unicodes.get ((unsigned) ((અચલ અક્ષર *) record - base));

    પાછા mutable_cache.set_for (record);
  }

  અચલ hb_set_t* set_for (અચલ EncodingRecord* record)
  {
    જો (!cached_unicodes.has ((unsigned) ((અચલ અક્ષર *) record - base)))
    {
      hb_set_t *s = hb_set_create ();
      જો (unlikely (s->in_error ()))
	પાછા hb_set_get_empty ();

      unsigned પૂર્ણાંક subtable_data_size = record->subtable < table_length
					? table_length - (unsigned પૂર્ણાંક) record->subtable
					: 0;
      (base+record->subtable).collect_unicodes (s, UINT_MAX, subtable_data_size);

      જો (unlikely (!cached_unicodes.set ((unsigned) ((અચલ અક્ષર *) record - base), hb::unique_ptr<hb_set_t> {s})))
        પાછા hb_set_get_empty ();

      પાછા s;
    }
    પાછા cached_unicodes.get ((unsigned) ((અચલ અક્ષર *) record - base));
  }

};

સ્થિર અંતઃરેખ uint16_t
hb_પ્રતીક_pua_map (unsigned codepoint)
{
  જો (codepoint <= 0x00FFu)
  {
    /* For symbol-encoded OpenType fonts, we duplicate the
     * U+F000..F0FF range at U+0000..U+00FF.  That's what
     * Windows seems to do, and that's hinted about at:
     * https://docs.microsoft.com/en-us/typography/opentype/spec/recom
     * under "Non-Standard (Symbol) Fonts". */
    પાછા 0xF000u + codepoint;
  }
  પાછા 0;
}

સંરચના cmap
{
  સ્થિર constexpr hb_tag_t tableTag = HB_OT_TAG_cmap;


  સ્થિર SubtableUnicodesCache* create_filled_cache(hb_blob_ptr_t<cmap> source_table) {
    અચલ cmap* cmap = source_table.get();
    auto it =
    + hb_iter (cmap->encodingRecord)
    | hb_filter ([&](અચલ EncodingRecord& _) {
      પાછા cmap::filter_encoding_records_for_subset (cmap, _);
    })
    ;

    SubtableUnicodesCache* cache = SubtableUnicodesCache::create(source_table);

    જો (unlikely (!cache))
      પાછા nullptr;

    માટે (અચલ EncodingRecord& _ : it)
      cache->set_for(&_); // populate the cache for this encoding record.

    પાછા cache;
  }

  template<typename Iterator, typename EncodingRecIter,
	   hb_requires (hb_is_iterator (EncodingRecIter))>
  તાર્કિક serialize (hb_serialize_context_t *c,
		  Iterator it,
		  EncodingRecIter encodingrec_iter,
		  અચલ શૂન્ય *base,
		  hb_subset_plan_t *plan,
	                  તાર્કિક drop_format_4 = ખોટું,
			  unsigned પૂર્ણાંક source_table_length = 0)
  {
    જો (unlikely (!c->extend_min ((*આ))))  પાછા ખોટું;
    આ->version = 0;

    unsigned format4objidx = 0, format12objidx = 0, format14objidx = 0;
#ifndef HB_NO_BEYOND_64K
    unsigned format15objidx = 0;
#endif
    auto snap = c->snapshot ();

    SubtableUnicodesCache local_unicodes_cache (base, source_table_length);
    અચલ SubtableUnicodesCache* unicodes_cache = &local_unicodes_cache;

    જો (plan->accelerator &&
        plan->accelerator->cmap_cache &&
        plan->accelerator->cmap_cache->same_base (base))
      unicodes_cache = plan->accelerator->cmap_cache;

    માટે (અચલ EncodingRecord& _ : encodingrec_iter)
    {
      જો (c->in_error ())
        પાછા ખોટું;

      unsigned format = (base+_.subtable).u.format.v;
      જો (format != 4 && format != 12 && format != 14
#ifndef HB_NO_BEYOND_64K
	  && format != 15
#endif
	 ) ચાલુ;

      અચલ hb_set_t* unicodes_set = unicodes_cache->set_for (&_, local_unicodes_cache);

      જો (!drop_format_4 && format == 4)
      {
        c->copy (_, + it | hb_filter (*unicodes_set, hb_first), 4u, base, plan, &format4objidx);
        જો (c->in_error () && c->only_overflow ())
        {
          // cmap4 overflowed, reset and retry serialization without format 4 subtables.
          c->revert (snap);
          પાછા serialize (c, it,
                            encodingrec_iter,
                            base,
                            plan,
                            સાચું,
			    source_table_length);
        }
      }

      નહીંતર જો (format == 12)
      {
        જો (_can_drop (_,
                       *unicodes_set,
                       base,
                       *unicodes_cache,
                       local_unicodes_cache,
                       + it | hb_map (hb_first), encodingrec_iter))
          ચાલુ;
        c->copy (_, + it | hb_filter (*unicodes_set, hb_first), 12u, base, plan, &format12objidx);
      }
      નહીંતર જો (format == 14) c->copy (_, it, 14u, base, plan, &format14objidx);
#ifndef HB_NO_BEYOND_64K
      નહીંતર જો (format == 15) c->copy (_, it, 15u, base, plan, &format15objidx);
#endif
    }
        unsigned length = c->length ();
        unsigned available = length > cmap::min_size ? length - cmap::min_size : 0;
        c->check_assign(આ->encodingRecord.len,
                        available / EncodingRecord::static_size,
                        HB_SERIALIZE_ERROR_INT_OVERFLOW);

    // Fail if format 4 was dropped and there is no cmap12.
    પાછા !drop_format_4 || format12objidx;
  }

  template<typename Iterator, typename EncodingRecordIterator,
      hb_requires (hb_is_iterator (Iterator)),
      hb_requires (hb_is_iterator (EncodingRecordIterator))>
  તાર્કિક _can_drop (અચલ EncodingRecord& cmap12,
                  અચલ hb_set_t& cmap12_unicodes,
                  અચલ શૂન્ય* base,
                  અચલ SubtableUnicodesCache& unicodes_cache,
                  SubtableUnicodesCache& local_unicodes_cache,
                  Iterator subset_unicodes,
                  EncodingRecordIterator encoding_records)
  {
    માટે (auto cp : + subset_unicodes | hb_filter (cmap12_unicodes))
    {
      જો (cp >= 0x10000) પાછા ખોટું;
    }

    unsigned target_platform;
    unsigned target_encoding;
    unsigned target_language = (base+cmap12.subtable).get_language ();

    જો (cmap12.platformID == 0 && cmap12.encodingID == 4)
    {
      target_platform = 0;
      target_encoding = 3;
    } નહીંતર જો (cmap12.platformID == 3 && cmap12.encodingID == 10) {
      target_platform = 3;
      target_encoding = 1;
    } નહીંતર {
      પાછા ખોટું;
    }

    માટે (અચલ auto& _ : encoding_records)
    {
      જો (_.platformID != target_platform
          || _.encodingID != target_encoding
          || (base+_.subtable).get_language() != target_language)
        ચાલુ;

      અચલ hb_set_t* sibling_unicodes = unicodes_cache.set_for (&_, local_unicodes_cache);

      auto cmap12 = + subset_unicodes | hb_filter (cmap12_unicodes);
      auto sibling = + subset_unicodes | hb_filter (*sibling_unicodes);
      માટે (; cmap12 && sibling; cmap12++, sibling++)
      {
        unsigned a = *cmap12;
        unsigned b = *sibling;
        જો (a != b) પાછા ખોટું;
      }

      પાછા !cmap12 && !sibling;
    }

    પાછા ખોટું;
  }

  શૂન્ય closure_glyphs (અચલ hb_set_t      *unicodes,
		       hb_set_t            *glyphset) અચલ
  {
    + hb_iter (encodingRecord)
    | hb_map (&EncodingRecord::subtable)
    | hb_map (hb_add (આ))
    | hb_filter (&CmapSubtable::is_variation_selector)
    | hb_apply ([=] (અચલ CmapSubtable& _) { _.closure_glyphs (unicodes, glyphset); })
    ;
  }

  તાર્કિક subset (hb_subset_context_t *c) અચલ
  { પાછા subset (c, ખોટું); }

  તાર્કિક subset (hb_subset_context_t *c, તાર્કિક is_dmap) અચલ
  {
    TRACE_SUBSET (આ);

    cmap *cmap_અવિભાજ્ય = c->serializer->start_embed<cmap> ();

    auto encodingrec_iter =
    + hb_iter (encodingRecord)
    | hb_filter ([&](અચલ EncodingRecord& _) {
      પાછા cmap::filter_encoding_records_for_subset (આ, _);
    })
    ;

    જો (unlikely (!encodingrec_iter.len ())) return_trace (ખોટું);

    અચલ EncodingRecord *unicode_bmp= nullptr, *unicode_ucs4 = nullptr, *ms_bmp = nullptr, *ms_ucs4 = nullptr;
    તાર્કિક has_format12 = ખોટું;

    માટે (અચલ EncodingRecord& _ : encodingrec_iter)
    {
      unsigned format = (આ + _.subtable).u.format.v;
      જો (format == 12) has_format12 = સાચું;

      અચલ EncodingRecord *table = std::addressof (_);
      જો      (_.platformID == 0 && _.encodingID ==  3) unicode_bmp = table;
      નહીંતર જો (_.platformID == 0 && _.encodingID ==  4) unicode_ucs4 = table;
      નહીંતર જો (_.platformID == 3 && _.encodingID ==  1) ms_bmp = table;
      નહીંતર જો (_.platformID == 3 && _.encodingID == 10) ms_ucs4 = table;
    }

    જો (unlikely (!is_dmap && !has_format12 && !unicode_bmp && !ms_bmp)) return_trace (ખોટું);
    જો (unlikely (!is_dmap && has_format12 && (!unicode_ucs4 && !ms_ucs4))) return_trace (ખોટું);

    auto it =
    + c->plan->unicode_to_new_gid_list.iter ()
    | hb_filter ([&] (અચલ hb_codepoint_pair_t _)
		 { પાછા (_.second != HB_MAP_કિંમત_INVALID); })
    ;

    return_trace (cmap_અવિભાજ્ય->serialize (c->serializer,
                                         it,
                                         encodingrec_iter,
                                         આ,
                                         c->plan,
					 ખોટું,
					 c->source_blob->length));
  }

  અચલ CmapSubtable *find_best_subtable (તાર્કિક *પ્રતીક = nullptr,
					  તાર્કિક *mac = nullptr,
					  તાર્કિક *macroman = nullptr) અચલ
  {
    જો (પ્રતીક) *પ્રતીક = ખોટું;
    જો (mac) *mac = ખોટું;
    જો (macroman) *macroman = ખોટું;

    અચલ CmapSubtable *subtable;

    /* Symbol subtable.
     * Prefer symbol if available.
     * https://github.com/harfbuzz/harfbuzz/issues/1918 */
    જો ((subtable = આ->find_subtable (3, 0)))
    {
      જો (પ્રતીક) *પ્રતીક = સાચું;
      પાછા subtable;
    }

    /* 32-bit subtables. */
    જો ((subtable = આ->find_subtable (3, 10))) પાછા subtable;
    જો ((subtable = આ->find_subtable (0, 6))) પાછા subtable;
    જો ((subtable = આ->find_subtable (0, 4))) પાછા subtable;

    /* 16-bit subtables. */
    જો ((subtable = આ->find_subtable (3, 1))) પાછા subtable;
    જો ((subtable = આ->find_subtable (0, 3))) પાછા subtable;
    જો ((subtable = આ->find_subtable (0, 2))) પાછા subtable;
    જો ((subtable = આ->find_subtable (0, 1))) પાછા subtable;
    જો ((subtable = આ->find_subtable (0, 0))) પાછા subtable;

    /* MacRoman subtable. */
    જો ((subtable = આ->find_subtable (1, 0)))
    {
      જો (mac) *mac = સાચું;
      જો (macroman) *macroman = સાચું;
      પાછા subtable;
    }
    /* Any other Mac subtable; we just map ASCII for these. */
    જો ((subtable = આ->find_subtable (1, 0xFFFF)))
    {
      જો (mac) *mac = સાચું;
      પાછા subtable;
    }

    /* Meh. */
    પાછા &Null (CmapSubtable);
  }

  સંરચના accelerator_t
  {
    using cache_t = hb_cache_t<21, 19>;
    static_assert (કદ (cache_t) == 1024, "");

    accelerator_t (hb_face_t *face, hb_tag_t table_tag = cmap::tableTag)
    {
      આ->table = hb_sanitize_context_t ().reference_table<cmap> (face, table_tag);
      જો (!આ->table.get_length ())
	પાછા;

      તાર્કિક પ્રતીક, mac, macroman;
      આ->subtable = table->find_best_subtable (&પ્રતીક, &mac, &macroman);
      આ->subtable_uvs = &Null (CmapSubtable);
      {
	અચલ CmapSubtable *st = table->find_variation_selector_subtable ();
	જો (st)
	  subtable_uvs = st;
      }

#ifndef HB_NO_OT_FONT_CMAP_CACHE
      cache = (cache_t *) hb_malloc (કદ (cache_t));
      જો (cache)
	નવું (cache) cache_t ();
      નહીંતર
        પાછા; // Such that get_glyph_funcZ remains null.
#endif

      આ->get_glyph_data = subtable;
#ifndef HB_NO_CMAP_LEGACY_SUBTABLES
      તાર્કિક is_format4 = subtable->u.format.v == 4;
      auto set_format4_getter = [આ] (તાર્કિક (*func) (અચલ શૂન્ય *,
						      hb_codepoint_t,
						      hb_codepoint_t *))
      {
	આ->format4_accel.init (&આ->subtable->u.format4,
				  get_subtable_data_size (આ->subtable));
	આ->get_glyph_data = &આ->format4_accel;
	આ->get_glyph_funcZ = func;
      };
      જો (unlikely (પ્રતીક))
      {
	પસંદગી ((unsigned) face->table.OS2->get_font_page ()) {
	સ્થિતિ OS2::font_page_t::FONT_PAGE_NONE:
	  જો (is_format4)
	    set_format4_getter (get_glyph_from_પ્રતીક<CmapSubtableFormat4::accelerator_t, hb_પ્રતીક_pua_map>);
	  નહીંતર
	    આ->get_glyph_funcZ = get_glyph_from_પ્રતીક<CmapSubtable, hb_પ્રતીક_pua_map>;
	  તોડો;
#ifndef HB_NO_OT_SHAPER_ARABIC_FALLBACK
	સ્થિતિ OS2::font_page_t::FONT_PAGE_SIMP_ARABIC:
	  જો (is_format4)
	    set_format4_getter (get_glyph_from_પ્રતીક<CmapSubtableFormat4::accelerator_t, _hb_arabic_pua_simp_map>);
	  નહીંતર
	    આ->get_glyph_funcZ = get_glyph_from_પ્રતીક<CmapSubtable, _hb_arabic_pua_simp_map>;
	  તોડો;
	સ્થિતિ OS2::font_page_t::FONT_PAGE_TRAD_ARABIC:
	  જો (is_format4)
	    set_format4_getter (get_glyph_from_પ્રતીક<CmapSubtableFormat4::accelerator_t, _hb_arabic_pua_trad_map>);
	  નહીંતર
	    આ->get_glyph_funcZ = get_glyph_from_પ્રતીક<CmapSubtable, _hb_arabic_pua_trad_map>;
	  તોડો;
#endif
	મૂળભૂત:
	  જો (is_format4)
	    set_format4_getter (get_glyph_from<CmapSubtableFormat4::accelerator_t>);
	  નહીંતર
	    આ->get_glyph_funcZ = get_glyph_from<CmapSubtable>;
	  તોડો;
	}
      }
      નહીંતર જો (unlikely (macroman))
      {
	જો (is_format4)
	  set_format4_getter (get_glyph_from_macroman<CmapSubtableFormat4::accelerator_t>);
	નહીંતર
	  આ->get_glyph_funcZ = get_glyph_from_macroman<CmapSubtable>;
      }
      નહીંતર જો (unlikely (mac))
      {
	જો (is_format4)
	  set_format4_getter (get_glyph_from_ascii<CmapSubtableFormat4::accelerator_t>);
	નહીંતર
	  આ->get_glyph_funcZ = get_glyph_from_ascii<CmapSubtable>;
      }
      નહીંતર
#endif
      {
	પસંદગી (subtable->u.format.v) {
	  /* Accelerate format 4 and format 12. */
	  મૂળભૂત:
	    આ->get_glyph_funcZ = get_glyph_from<CmapSubtable>;
	    તોડો;
	  સ્થિતિ 12:
	    આ->get_glyph_funcZ = get_glyph_from<CmapSubtableFormat12>;
	    તોડો;
	  સ્થિતિ  4:
	  {
	    hb_barrier ();
	    આ->format4_accel.init (&subtable->u.format4,
				      get_subtable_data_size (subtable));
	    આ->get_glyph_data = &આ->format4_accel;
	    આ->get_glyph_funcZ = આ->format4_accel.get_glyph_func;
	    તોડો;
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

    અંતઃરેખ તાર્કિક _cached_get (hb_codepoint_t unicode,
			     hb_codepoint_t *glyph) અચલ
    {
#ifndef HB_NO_OT_FONT_CMAP_CACHE
      // cache is always non-null if we have a get_glyph_funcZ
      unsigned v;
      જો (cache->get (unicode, &v))
      {
        *glyph = v;
	પાછા સાચું;
      }
#endif
      તાર્કિક ret  = આ->get_glyph_funcZ (આ->get_glyph_data, unicode, glyph);

#ifndef HB_NO_OT_FONT_CMAP_CACHE
      જો (ret)
        cache->set (unicode, *glyph);
#endif

      પાછા ret;
    }

    તાર્કિક get_nominal_glyph (hb_codepoint_t  unicode,
			    hb_codepoint_t *glyph) અચલ
    {
      જો (unlikely (!આ->get_glyph_funcZ)) પાછા ખોટું;
      પાછા _cached_get (unicode, glyph);
    }

    unsigned પૂર્ણાંક get_nominal_glyphs (unsigned પૂર્ણાંક ગણતરી,
				     અચલ hb_codepoint_t *first_unicode,
				     unsigned પૂર્ણાંક unicode_stride,
				     hb_codepoint_t *first_glyph,
				     unsigned પૂર્ણાંક glyph_stride) અચલ
    {
      જો (unlikely (!આ->get_glyph_funcZ)) પાછા 0;

      unsigned પૂર્ણાંક done;
      માટે (done = 0;
	   done < ગણતરી && _cached_get (*first_unicode, first_glyph);
	   done++)
      {
	first_unicode = &StructAtOffsetUnaligned<hb_codepoint_t> (first_unicode, unicode_stride);
	first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      }
      પાછા done;
    }

    તાર્કિક get_variation_glyph (hb_codepoint_t  unicode,
			      hb_codepoint_t  variation_selector,
			      hb_codepoint_t *glyph) અચલ
    {
      પસંદગી (get_glyph_variant (unicode, variation_selector, glyph))
      {
	સ્થિતિ GLYPH_VARIANT_NOT_FOUND:	પાછા ખોટું;
	સ્થિતિ GLYPH_VARIANT_FOUND:	પાછા સાચું;
	સ્થિતિ GLYPH_VARIANT_USE_DEFAULT:	તોડો;
      }

      પાછા get_nominal_glyph (unicode, glyph);
    }

    glyph_variant_t get_glyph_variant (hb_codepoint_t  unicode,
				       hb_codepoint_t  variation_selector,
				       hb_codepoint_t *glyph) અચલ
    {
      પાછા આ->subtable_uvs->get_glyph_variant (unicode,
						    variation_selector,
						    glyph);
    }

    તાર્કિક has_data () અચલ { પાછા table.get_length (); }

    શૂન્ય collect_unicodes (hb_set_t *out, unsigned પૂર્ણાંક અંક_glyphs) અચલ
    { subtable->collect_unicodes (out, અંક_glyphs, get_subtable_data_size (subtable)); }
    શૂન્ય collect_mapping (hb_set_t *unicodes, hb_map_t *mapping,
			  unsigned અંક_glyphs = UINT_MAX) અચલ
    { subtable->collect_mapping (unicodes, mapping, અંક_glyphs, get_subtable_data_size (subtable)); }
    શૂન્ય collect_variation_selectors (hb_set_t *out) અચલ
    { subtable_uvs->collect_variation_selectors (out); }
    શૂન્ય collect_variation_unicodes (hb_codepoint_t variation_selector,
				     hb_set_t *out) અચલ
    { subtable_uvs->collect_variation_unicodes (variation_selector, out); }

    સુરક્ષિત:
    પ્રકારવ્યાખ્યા તાર્કિક (*hb_cmap_get_glyph_func_t) (અચલ શૂન્ય *obj,
					      hb_codepoint_t codepoint,
					      hb_codepoint_t *glyph);
    પ્રકારવ્યાખ્યા uint16_t (*hb_pua_remap_func_t) (unsigned);

    template <typename Type>
    HB_INTERNAL સ્થિર તાર્કિક get_glyph_from (અચલ શૂન્ય *obj,
					    hb_codepoint_t codepoint,
					    hb_codepoint_t *glyph)
    {
      અચલ Type *typed_obj = (અચલ Type *) obj;
      પાછા typed_obj->get_glyph (codepoint, glyph);
    }

    template <typename Type, hb_pua_remap_func_t remap>
    HB_INTERNAL સ્થિર તાર્કિક get_glyph_from_પ્રતીક (અચલ શૂન્ય *obj,
						   hb_codepoint_t codepoint,
						   hb_codepoint_t *glyph)
    {
      અચલ Type *typed_obj = (અચલ Type *) obj;
      જો (likely (typed_obj->get_glyph (codepoint, glyph)))
	પાછા સાચું;

      જો (hb_codepoint_t c = remap (codepoint))
	પાછા typed_obj->get_glyph (c, glyph);

      પાછા ખોટું;
    }

    template <typename Type>
    HB_INTERNAL સ્થિર તાર્કિક get_glyph_from_ascii (અચલ શૂન્ય *obj,
						  hb_codepoint_t codepoint,
						  hb_codepoint_t *glyph)
    {
      અચલ Type *typed_obj = (અચલ Type *) obj;
      પાછા codepoint < 0x80 && typed_obj->get_glyph (codepoint, glyph);
    }

    template <typename Type>
    HB_INTERNAL સ્થિર તાર્કિક get_glyph_from_macroman (અચલ શૂન્ય *obj,
						     hb_codepoint_t codepoint,
						     hb_codepoint_t *glyph)
    {
      જો (get_glyph_from_ascii<Type> (obj, codepoint, glyph))
	પાછા સાચું;

      અચલ Type *typed_obj = (અચલ Type *) obj;
      unsigned c = unicode_to_macroman (codepoint);
      પાછા c && typed_obj->get_glyph (c, glyph);
    }

    unsigned પૂર્ણાંક get_subtable_data_size (અચલ CmapSubtable *subtable) અચલ
    {
      unsigned પૂર્ણાંક table_length = આ->table.get_length ();
      uintptr_t table_start = (uintptr_t) (અચલ શૂન્ય *) આ->table.get ();
      uintptr_t subtable_addr = (uintptr_t) (અચલ શૂન્ય *) subtable;
      જો (unlikely (subtable_addr < table_start))
	પાછા 0;

      uintptr_t subtable_offset = subtable_addr - table_start;
      જો (unlikely (subtable_offset >= table_length))
	પાછા 0;

      પાછા table_length - (unsigned પૂર્ણાંક) subtable_offset;
    }

    ખાનગી:
    hb_nonnull_ptr_t<અચલ CmapSubtable> subtable;
    hb_nonnull_ptr_t<અચલ CmapSubtable> subtable_uvs;

    hb_cmap_get_glyph_func_t get_glyph_funcZ = nullptr;
    અચલ શૂન્ય *get_glyph_data = nullptr;

    CmapSubtableFormat4::accelerator_t format4_accel;

#ifndef HB_NO_OT_FONT_CMAP_CACHE
    cache_t *cache = nullptr;
#endif

    જાહેર:
    hb_blob_ptr_t<cmap> table;
  };

  સુરક્ષિત:

  અચલ CmapSubtable *find_subtable (unsigned પૂર્ણાંક platform_id,
				     unsigned પૂર્ણાંક encoding_id) અચલ
  {
    EncodingRecord key;
    key.platformID = platform_id;
    key.encodingID = encoding_id;

    અચલ EncodingRecord &result = encodingRecord.bsearch (key);
    જો (!result.subtable)
      પાછા nullptr;

    પાછા &(આ+result.subtable);
  }

  અચલ CmapSubtable *find_variation_selector_subtable () અચલ
  {
    અચલ CmapSubtable *format14 = nullptr;
    માટે (અચલ EncodingRecord& record : encodingRecord)
    {
      જો (record.platformID != 0 || record.encodingID != 5)
	ચાલુ;

      અચલ CmapSubtable *subtable = &(આ+record.subtable);
#ifndef HB_NO_BEYOND_64K
      જો (subtable->u.format.v == 15)
	પાછા subtable;
#endif
      જો (subtable->u.format.v == 14)
	format14 = subtable;
    }
    પાછા format14;
  }

  જાહેર:

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) &&
		  hb_barrier () &&
		  likely (version == 0) &&
		  encodingRecord.sanitize (c, આ));
  }

 ખાનગી:

  સ્થિર તાર્કિક filter_encoding_records_for_subset(અચલ cmap* cmap,
                                                 અચલ EncodingRecord& _)
  {
    પાછા
        (_.platformID == 0 && _.encodingID == 3) ||
        (_.platformID == 0 && _.encodingID == 4) ||
        (_.platformID == 3 && _.encodingID == 1) ||
        (_.platformID == 3 && _.encodingID == 10) ||
        (cmap + _.subtable).is_variation_selector ();
  }

  સુરક્ષિત:
  HBUINT16	version;	/* Table version number (0). */
  SortedArray16Of<EncodingRecord>
		encodingRecord;	/* Encoding tables. */
  જાહેર:
  DEFINE_SIZE_ARRAY (4, encodingRecord);
};

સંરચના DMAP : cmap
{
  સ્થિર constexpr hb_tag_t tableTag = HB_OT_TAG_DMAP;

  તાર્કિક subset (hb_subset_context_t *c) અચલ
  { પાછા cmap::subset (c, સાચું); }
};

સંરચના cmap_accelerator_t
{
  cmap_accelerator_t (hb_face_t *face) :
    dmap (face, HB_OT_TAG_DMAP),
    base (face)
  {}

  તાર્કિક get_nominal_glyph (hb_codepoint_t  unicode,
			  hb_codepoint_t *glyph) અચલ
  {
    પાછા dmap.get_nominal_glyph (unicode, glyph) ||
	   base.get_nominal_glyph (unicode, glyph);
  }

  unsigned પૂર્ણાંક get_nominal_glyphs (unsigned પૂર્ણાંક ગણતરી,
				   અચલ hb_codepoint_t *first_unicode,
				   unsigned પૂર્ણાંક unicode_stride,
				   hb_codepoint_t *first_glyph,
				   unsigned પૂર્ણાંક glyph_stride) અચલ
  {
    જો (likely (!dmap.has_data ()))
      પાછા base.get_nominal_glyphs (ગણતરી,
				      first_unicode, unicode_stride,
				      first_glyph, glyph_stride);

    unsigned પૂર્ણાંક done;
    માટે (done = 0;
	 done < ગણતરી && get_nominal_glyph (*first_unicode, first_glyph);
	 done++)
    {
      first_unicode = &StructAtOffsetUnaligned<hb_codepoint_t> (first_unicode, unicode_stride);
      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
    }
    પાછા done;
  }

  તાર્કિક get_variation_glyph (hb_codepoint_t  unicode,
			    hb_codepoint_t  variation_selector,
			    hb_codepoint_t *glyph) અચલ
  {
    glyph_variant_t result = dmap.get_glyph_variant (unicode,
						     variation_selector,
						     glyph);
    જો (result == GLYPH_VARIANT_NOT_FOUND)
      result = base.get_glyph_variant (unicode, variation_selector, glyph);

    પસંદગી (result)
    {
      સ્થિતિ GLYPH_VARIANT_NOT_FOUND: પાછા ખોટું;
      સ્થિતિ GLYPH_VARIANT_FOUND: પાછા સાચું;
      સ્થિતિ GLYPH_VARIANT_USE_DEFAULT: તોડો;
    }

    પાછા get_nominal_glyph (unicode, glyph);
  }

  શૂન્ય collect_unicodes (hb_set_t *out, unsigned પૂર્ણાંક અંક_glyphs) અચલ
  {
    base.collect_unicodes (out, અંક_glyphs);
    dmap.collect_unicodes (out, અંક_glyphs);
  }

  શૂન્ય collect_mapping (hb_set_t *unicodes, hb_map_t *mapping,
			unsigned અંક_glyphs = UINT_MAX) અચલ
  {
    base.collect_mapping (unicodes, mapping, અંક_glyphs);
    dmap.collect_mapping (unicodes, mapping, અંક_glyphs);
  }

  શૂન્ય collect_variation_selectors (hb_set_t *out) અચલ
  {
    base.collect_variation_selectors (out);
    dmap.collect_variation_selectors (out);
  }

  શૂન્ય collect_variation_unicodes (hb_codepoint_t variation_selector,
				   hb_set_t *out) અચલ
  {
    base.collect_variation_unicodes (variation_selector, out);
    dmap.collect_variation_unicodes (variation_selector, out);
  }

  શૂન્ય closure_glyphs (અચલ hb_set_t *unicodes,
		       hb_set_t       *glyphset) અચલ
  {
    જો (likely (!dmap.has_data ()))
    {
      base.table->closure_glyphs (unicodes, glyphset);
      પાછા;
    }

    hb_set_t variation_selectors;
    collect_variation_selectors (&variation_selectors);
    માટે (hb_codepoint_t variation_selector : variation_selectors)
    {
      જો (!unicodes->has (variation_selector))
	ચાલુ;

      hb_set_t variation_unicodes;
      collect_variation_unicodes (variation_selector, &variation_unicodes);
      માટે (hb_codepoint_t unicode : variation_unicodes)
      {
	જો (!unicodes->has (unicode))
	  ચાલુ;

	hb_codepoint_t glyph;
	જો (get_variation_glyph (unicode, variation_selector, &glyph))
	  glyphset->add (glyph);
      }
    }
  }

  cmap::accelerator_t dmap;
  cmap::accelerator_t base;
};

} /* namespace OT */


#endif /* HB_OT_CMAP_TABLE_HH */
