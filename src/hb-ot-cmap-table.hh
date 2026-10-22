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

#include "hb-ot-os2-table.hh"
#include "hb-ot-shaper-arabic-pua.hh"
#include "hb-open-type.hh"
#include "hb-ot-dual.hh"
#include "hb-set.hh"
#include "hb-cache.hh"

/*
 * cmap -- Character to Glyph Index Mapping
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cmap
 */
#define HB_OT_TAG_cmap HB_TAG('c','m','a','p')
#define HB_OT_TAG_DMAP HB_TAG('D','M','A','P')

namespace OT {

static inline uint8_t unicode_to_macroman (hb_codepoint_t u)
{
  static const struct unicode_to_macroman_t
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
  auto *c = hb_bsearch (u, mapping, ARRAY_LENGTH (mapping), sizeof (mapping[0]),
			_hb_cmp_operator<uint16_t, uint16_t>);
  върни c ? c->macroman : 0;
}

struct CmapSubtableFormat0
{
  bool get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) const
  {
    hb_codepoint_t gid = codepoint < 256 ? glyphIdArray[codepoint] : 0;
    ако (unlikely (!gid))
      върни false;
    *glyph = gid;
    върни true;
  }

  unsigned get_language () const
  {
    върни language;
  }

  void collect_unicodes (hb_set_t *out) const
  {
    за (unsigned цяло i = 0; i < 256; i++)
      ако (glyphIdArray[i])
	out->add (i);
  }

  void collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping /* OUT */) const
  {
    за (unsigned i = 0; i < 256; i++)
      ако (glyphIdArray[i])
      {
	hb_codepoint_t glyph = glyphIdArray[i];
	unicodes->add (i);
	mapping->set (i, glyph);
      }
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this));
  }

  protected:
  HBUINT16	format;		/* Format number is set to 0. */
  HBUINT16	length;		/* Byte length of this subtable. */
  HBUINT16	language;	/* Ignore. */
  HBUINT8	glyphIdArray[256];/* An array that maps character
				 * code to glyph index values. */
  public:
  DEFINE_SIZE_STATIC (6 + 256);
};

struct CmapSubtableFormat4
{
  template<typename Iterator,
      typename Writer,
	   hb_requires (hb_is_iterator (Iterator))>
  void to_ranges (Iterator it, Writer& range_writer)
  {
    hb_codepoint_t start_cp = 0, prev_run_start_cp = 0, run_start_cp = 0, end_cp = 0, last_gid = 0;
    цяло run_length = 0 , delta = 0, prev_delta = 0;

    enum {
      FIRST_SUB_RANGE,
      FOLLOWING_SUB_RANGE,
    } mode;

    докато (it) {
      // Start a new range
      {
        const auto& pair = *it;
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

      докато (it) {
        // Process range
        const auto& pair = *it;
        hb_codepoint_t next_cp = pair.first;
        hb_codepoint_t next_gid = pair.second;
        ако (next_cp != end_cp + 1) {
          // Current range is over, stop processing.
          break;
        }

        ако (next_gid == last_gid + 1) {
          // The current run continues.
          end_cp = next_cp;
          run_length++;
          last_gid = next_gid;
          it++;
          continue;
        }

        // A new run is starting, decide if we want to commit the current run.
        цяло split_cost = (mode == FIRST_SUB_RANGE) ? 8 : 16;
        цяло run_cost = run_length * 2;
        ако (run_cost >= split_cost) {
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

    ако (likely (end_cp != 0xFFFF)) {
      range_writer (0xFFFF, 0xFFFF, 1);
    }
  }

  /*
   * Writes the current range as either one or two ranges depending on what is most efficient.
   */
  template<typename Writer>
  void commit_current_range (hb_codepoint_t start,
                             hb_codepoint_t prev_run_start,
                             hb_codepoint_t run_start,
                             hb_codepoint_t end,
                             цяло run_delta,
                             цяло previous_run_delta,
                             цяло split_cost,
                             Writer& range_writer) {
    bool should_split = false;
    ако (start < run_start && run_start < end) {
      цяло run_cost = (end - run_start + 1) * 2;
      ако (run_cost >= split_cost) {
        should_split = true;
      }
    }

    // A zero delta is also the marker for ranges that need a
    // glyphIdArray; serialize_rangeoffset_glyid() tells the two cases
    // apart by re-checking the mapping.
    ако (should_split) {
      ако (start == prev_run_start)
        range_writer (start, run_start - 1, previous_run_delta);
      иначе
        range_writer (start, run_start - 1, 0);
      range_writer (run_start, end, run_delta);
      върни;
    }


    ако (start == run_start) {
      // Range is only a run
      range_writer (start, end, run_delta);
      върни;
    }

    // Write only a single non-run range.
    range_writer (start, end, 0);
  }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  unsigned serialize_find_segcount (Iterator it) {
    struct Counter {
      unsigned segcount = 0;

      void operator() (hb_codepoint_t start,
                       hb_codepoint_t end,
                       цяло delta) {
        segcount++;
      }
    } counter;

    to_ranges (+it, counter);
    върни counter.segcount;
  }


  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  bool serialize_start_end_delta_arrays (hb_serialize_context_t *c,
                                         Iterator it,
                                         цяло segcount)
  {
    struct Writer {
      hb_serialize_context_t *serializer_;
      HBUINT16* end_code_;
      HBUINT16* start_code_;
      HBINT16* id_delta_;
      цяло index_;

      Writer(hb_serialize_context_t *serializer)
          : serializer_(serializer),
            end_code_(nullptr),
            start_code_(nullptr),
            id_delta_(nullptr),
            index_ (0) {}
      void operator() (hb_codepoint_t start,
                       hb_codepoint_t end,
                       цяло delta) {
        start_code_[index_] = start;
        end_code_[index_] = end;
        id_delta_[index_] = delta;
        index_++;
      }
    } writer(c);

    writer.end_code_ = c->allocate_size<HBUINT16> (HBUINT16::static_size * segcount, false);
    (void) c->allocate_size<HBUINT16> (2); // padding
    writer.start_code_ = c->allocate_size<HBUINT16> (HBUINT16::static_size * segcount, false);
    writer.id_delta_ = c->allocate_size<HBINT16> (HBINT16::static_size * segcount, false);

    ако (unlikely (!writer.end_code_ || !writer.start_code_ || !writer.id_delta_)) върни false;

    to_ranges (+it, writer);
    върни true;
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
    ако (unlikely (!c->check_success (idRangeOffset))) върни nullptr;
    ако (unlikely ((char *)idRangeOffset - (char *)idDelta != (цяло) segcount * (цяло) HBINT16::static_size)) върни nullptr;

    за (unsigned i : hb_range (segcount))
    {
      /* A non-zero delta is always a real run. A zero delta is
       * ambiguous: identity mapping, or the marker for a range that
       * needs a glyphIdArray. Only zero deltas need a check. */
      цяло delta = idDelta[i];
      ако (delta)
	continue;
      bool use_delta = true;
      за (hb_codepoint_t cp = startCode[i]; cp <= endCode[i]; cp++)
      {
	hb_codepoint_t gid = cp_to_gid.get (cp);
	ако (gid == HB_MAP_VALUE_INVALID)
	  gid = 0;
	ако (gid != cp)
	{
	  use_delta = false;
	  break;
	}
      }
      ако (use_delta)
	continue;

      idRangeOffset[i] = 2 * (c->start_embed<HBUINT16> () - idRangeOffset - i);
      за (hb_codepoint_t cp = startCode[i]; cp <= endCode[i]; cp++)
      {
        HBUINT16 gid;
        gid = cp_to_gid[cp];
        c->copy<HBUINT16> (gid);
      }
    }

    върни idRangeOffset;
  }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  void serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    auto format4_iter =
    + it
    | hb_filter ([&] (const hb_codepoint_pair_t _)
		 { върни _.first <= 0xFFFF; })
    ;

    ако (!format4_iter) върни;

    unsigned table_initpos = c->length ();
    ако (unlikely (!c->extend_min (this))) върни;
    this->format = 4;

    hb_vector_t<hb_codepoint_pair_t> cp_to_gid {
      format4_iter
    };

    //serialize endCode[], startCode[], idDelta[]
    HBUINT16* endCode = c->start_embed<HBUINT16> ();
    unsigned segcount = serialize_find_segcount (cp_to_gid.iter());
    ако (unlikely (!serialize_start_end_delta_arrays (c, cp_to_gid.iter(), segcount)))
      върни;

    HBUINT16 *startCode = endCode + segcount + 1;
    HBINT16 *idDelta = ((HBINT16*)startCode) + segcount;

    HBUINT16 *idRangeOffset = serialize_rangeoffset_glyid (c,
                                                           cp_to_gid.iter (),
                                                           endCode,
                                                           startCode,
                                                           idDelta,
                                                           segcount);
    ако (unlikely (!c->check_success (idRangeOffset))) върни;

    this->length = c->length () - table_initpos;
    ако ((long long) this->length != (long long) c->length () - table_initpos)
    {
      c->err (HB_SERIALIZE_ERROR_INT_OVERFLOW);
      върни;
    }

    this->segCountX2 = segcount * 2;
    this->entrySelector = hb_max (1u, hb_bit_storage (segcount)) - 1;
    this->searchRange = 2 * (1u << this->entrySelector);
    this->rangeShift = segcount * 2 > this->searchRange
		       ? 2 * segcount - this->searchRange
		       : 0;
  }

  unsigned get_language () const
  {
    върни language;
  }

  struct accelerator_t
  {
    accelerator_t () {}
    accelerator_t (const CmapSubtableFormat4 *subtable) = delete;

    void init (const CmapSubtableFormat4 *subtable,
	       unsigned цяло              subtable_data_size)
    {
      segCount = subtable->segCountX2 / 2;
      endCount = subtable->values.arrayZ;
      startCount = endCount + segCount + 1;
      idDelta = startCount + segCount;
      idRangeOffset = idDelta + segCount;
      glyphIdArray = idRangeOffset + segCount;

      unsigned цяло values_offset = 16 + 8 * segCount;
      glyphIdArrayLength = subtable_data_size > values_offset
			   ? (subtable_data_size - values_offset) / 2
			   : 0;
    }

    bool get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) const
    {
      struct CustomRange
      {
	цяло cmp (hb_codepoint_t k,
		 unsigned distance) const
	{
	  ако (k > last) върни +1;
	  ако (k < (&last)[distance]/*first*/) върни -1;
	  върни 0;
	}
	HBUINT16 last;
      };

      const HBUINT16 *found = hb_bsearch (codepoint,
					  this->endCount,
					  this->segCount,
					  sizeof (CustomRange),
					  _hb_cmp_method<hb_codepoint_t, CustomRange, unsigned>,
					  this->segCount + 1);
      ако (unlikely (!found))
	върни false;
      unsigned цяло i = found - endCount;

      hb_codepoint_t gid;
      unsigned цяло rangeOffset = this->idRangeOffset[i];
      ако (rangeOffset == 0)
	gid = codepoint + this->idDelta[i];
      иначе
      {
	/* Somebody has been smoking... */
	unsigned цяло index = rangeOffset / 2 + (codepoint - this->startCount[i]) + i - this->segCount;
	ако (unlikely (index >= this->glyphIdArrayLength))
	  върни false;
	gid = this->glyphIdArray[index];
	ако (unlikely (!gid))
	  върни false;
	gid += this->idDelta[i];
      }
      gid &= 0xFFFFu;
      ако (unlikely (!gid))
	върни false;
      *glyph = gid;
      върни true;
    }

    HB_INTERNAL static bool get_glyph_func (const void *obj, hb_codepoint_t codepoint, hb_codepoint_t *glyph)
    { върни ((const accelerator_t *) obj)->get_glyph (codepoint, glyph); }

    void collect_unicodes (hb_set_t *out) const
    {
      unsigned цяло count = this->segCount;
      ако (count && this->startCount[count - 1] == 0xFFFFu)
	count--; /* Skip sentinel segment. */
      за (unsigned цяло i = 0; i < count; i++)
      {
	hb_codepoint_t start = this->startCount[i];
	hb_codepoint_t end = this->endCount[i];
	unsigned цяло rangeOffset = this->idRangeOffset[i];
        out->add_range(start, end);
	ако (rangeOffset == 0)
	{
	  за (hb_codepoint_t codepoint = start; codepoint <= end; codepoint++)
	  {
	    hb_codepoint_t gid = (codepoint + this->idDelta[i]) & 0xFFFFu;
	    ако (unlikely (!gid))
              out->del(codepoint);
	  }
	}
	иначе
	{
	  за (hb_codepoint_t codepoint = start; codepoint <= end; codepoint++)
	  {
	    unsigned цяло index = rangeOffset / 2 + (codepoint - this->startCount[i]) + i - this->segCount;
	    ако (unlikely (index >= this->glyphIdArrayLength))
            {
              out->del_range (codepoint, end);
	      break;
            }
	    hb_codepoint_t gid = this->glyphIdArray[index];
	    ако (unlikely (!gid))
              out->del(codepoint);
	  }
	}
      }
    }

    void collect_mapping (hb_set_t *unicodes, /* OUT */
			  hb_map_t *mapping /* OUT */) const
    {
      // TODO(grieger): optimize similar to collect_unicodes
      // (ie. use add_range())
      unsigned count = this->segCount;
      ако (count && this->startCount[count - 1] == 0xFFFFu)
	count--; /* Skip sentinel segment. */
      за (unsigned i = 0; i < count; i++)
      {
	hb_codepoint_t start = this->startCount[i];
	hb_codepoint_t end = this->endCount[i];
	unsigned rangeOffset = this->idRangeOffset[i];
	ако (rangeOffset == 0)
	{
	  за (hb_codepoint_t codepoint = start; codepoint <= end; codepoint++)
	  {
	    hb_codepoint_t gid = (codepoint + this->idDelta[i]) & 0xFFFFu;
	    ако (unlikely (!gid))
	      continue;
	    unicodes->add (codepoint);
	    mapping->set (codepoint, gid);
	  }
	}
	иначе
	{
	  за (hb_codepoint_t codepoint = start; codepoint <= end; codepoint++)
	  {
	    unsigned index = rangeOffset / 2 + (codepoint - this->startCount[i]) + i - this->segCount;
	    ако (unlikely (index >= this->glyphIdArrayLength))
	      break;
	    hb_codepoint_t gid = this->glyphIdArray[index];
	    ако (unlikely (!gid))
	      continue;
            gid = (gid + this->idDelta[i]) & 0xFFFFu;
	    unicodes->add (codepoint);
	    mapping->set (codepoint, gid);
	  }
	}
      }
    }

    const HBUINT16 *endCount;
    const HBUINT16 *startCount;
    const HBUINT16 *idDelta;
    const HBUINT16 *idRangeOffset;
    const HBUINT16 *glyphIdArray;
    unsigned цяло segCount;
    unsigned цяло glyphIdArrayLength;
  };

  bool get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph,
		  unsigned цяло subtable_data_size) const
  {
    accelerator_t accel;
    accel.init (this, subtable_data_size);
    върни accel.get_glyph_func (&accel, codepoint, glyph);
  }
  bool get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) const
  { върни false; }

  void collect_unicodes (hb_set_t *out, unsigned цяло subtable_data_size) const
  {
    accelerator_t accel;
    accel.init (this, subtable_data_size);
    accel.collect_unicodes (out);
  }
  void collect_unicodes (hb_set_t *out) const
  { collect_unicodes (out, length); }

  void collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping, /* OUT */
			unsigned цяло subtable_data_size) const
  {
    accelerator_t accel;
    accel.init (this, subtable_data_size);
    accel.collect_mapping (unicodes, mapping);
  }
  void collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping /* OUT */) const
  { collect_mapping (unicodes, mapping, length); }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    ако (unlikely (!c->check_struct (this)))
      return_trace (false);
    hb_barrier ();

    return_trace (c->check_range (values, 2 + 4 * segCountX2));
  }

  protected:
  HBUINT16	format;		/* Format number is set to 4. */
  HBUINT16	length;		/* This is the length in bytes of the
				 * subtable. */
  HBUINT16	language;	/* Ignore. */
  HBUINT16	segCountX2;	/* 2 x segCount. */
  HBUINT16	searchRange;	/* 2 * (2**floor(log2(segCount))) */
  HBUINT16	entrySelector;	/* log2(searchRange/2) */
  HBUINT16	rangeShift;	/* 2 x segCount - searchRange */

  UnsizedArrayOf<HBUINT16>
		values;
#ако 0
  HBUINT16	endCount[segCount];	/* End characterCode for each segment,
					 * last=0xFFFFu. */
  HBUINT16	reservedPad;		/* Set to 0. */
  HBUINT16	startCount[segCount];	/* Start character code for each segment. */
  HBINT16		idDelta[segCount];	/* Delta for all character codes in segment. */
  HBUINT16	idRangeOffset[segCount];/* Offsets into glyphIdArray or 0 */
  UnsizedArrayOf<HBUINT16>
		glyphIdArray;	/* Glyph index array (arbitrary length) */
#endif

  public:
  DEFINE_SIZE_ARRAY (14, values);
};

struct CmapSubtableLongGroup
{
  friend struct CmapSubtableFormat12;
  friend struct CmapSubtableFormat13;
  template<typename U>
  friend struct CmapSubtableLongSegmented;
  friend struct cmap;

  цяло cmp (hb_codepoint_t codepoint) const
  {
    ако (codepoint < startCharCode) върни -1;
    ако (codepoint > endCharCode)   върни +1;
    върни 0;
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this));
  }

  private:
  HBUINT32		startCharCode;	/* First character code in this group. */
  HBUINT32		endCharCode;	/* Last character code in this group. */
  HBUINT32		glyphID;	/* Glyph index; interpretation depends on
					 * subtable format. */
  public:
  DEFINE_SIZE_STATIC (12);
};
DECLARE_NULL_NAMESPACE_BYTES (OT, CmapSubtableLongGroup);

template <typename UINT>
struct CmapSubtableTrimmed
{
  bool get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) const
  {
    /* Rely on our implicit array bound-checking. */
    hb_codepoint_t gid = glyphIdArray[codepoint - startCharCode];
    ако (unlikely (!gid))
      върни false;
    *glyph = gid;
    върни true;
  }

  unsigned get_language () const
  {
    върни language;
  }

  void collect_unicodes (hb_set_t *out) const
  {
    hb_codepoint_t start = startCharCode;
    unsigned цяло count = glyphIdArray.len;
    за (unsigned цяло i = 0; i < count; i++)
      ако (glyphIdArray[i])
	out->add (start + i);
  }

  void collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping /* OUT */) const
  {
    hb_codepoint_t start_cp = startCharCode;
    unsigned count = glyphIdArray.len;
    за (unsigned i = 0; i < count; i++)
      ако (glyphIdArray[i])
      {
	hb_codepoint_t unicode = start_cp + i;
	hb_codepoint_t glyphid = glyphIdArray[i];
	unicodes->add (unicode);
	mapping->set (unicode, glyphid);
      }
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this) && glyphIdArray.sanitize (c));
  }

  protected:
  UINT		formatReserved;	/* Subtable format and (maybe) padding. */
  UINT		length;		/* Byte length of this subtable. */
  UINT		language;	/* Ignore. */
  UINT		startCharCode;	/* First character code covered. */
  ArrayOf<HBGlyphID16, UINT>
		glyphIdArray;	/* Array of glyph index values for character
				 * codes in the range. */
  public:
  DEFINE_SIZE_ARRAY (5 * sizeof (UINT), glyphIdArray);
};

struct CmapSubtableFormat6  : CmapSubtableTrimmed<HBUINT16> {};
struct CmapSubtableFormat10 : CmapSubtableTrimmed<HBUINT32> {};

template <typename T>
struct CmapSubtableLongSegmented
{
  friend struct cmap;

  bool get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) const
  {
    hb_codepoint_t gid = T::group_get_glyph (groups.bsearch (codepoint), codepoint);
    ако (unlikely (!gid))
      върни false;
    *glyph = gid;
    върни true;
  }

  unsigned get_language () const
  {
    върни language;
  }

  void collect_unicodes (hb_set_t *out, unsigned цяло num_glyphs) const
  {
    за (unsigned цяло i = 0; i < this->groups.len; i++)
    {
      hb_codepoint_t start = this->groups[i].startCharCode;
      hb_codepoint_t end = hb_min ((hb_codepoint_t) this->groups[i].endCharCode,
				   (hb_codepoint_t) HB_UNICODE_MAX);
      hb_codepoint_t gid = this->groups[i].glyphID;
      ако (!gid)
      {
	/* Intention is: if (hb_is_same (T, CmapSubtableFormat13)) continue; */
	ако (! T::group_get_glyph (this->groups[i], end)) continue;
	start++;
	gid++;
      }
      ако (unlikely ((unsigned цяло) gid >= num_glyphs)) continue;
      ако (unlikely ((unsigned цяло) (gid + end - start) >= num_glyphs))
	end = start + (hb_codepoint_t) num_glyphs - gid;

      out->add_range (start, hb_min (end, 0x10FFFFu));
    }
  }

  void collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping, /* OUT */
			unsigned num_glyphs) const
  {
    hb_codepoint_t last_end = 0;
    unsigned count = this->groups.len;
    за (unsigned i = 0; i < count; i++)
    {
      hb_codepoint_t start = this->groups.arrayZ[i].startCharCode;
      hb_codepoint_t end = hb_min ((hb_codepoint_t) this->groups.arrayZ[i].endCharCode,
				   (hb_codepoint_t) HB_UNICODE_MAX);
      ако (unlikely (start > end || start < last_end)) {
        // Range is not in order and is invalid, skip it.
        continue;
      }
      last_end = end;


      hb_codepoint_t gid = this->groups.arrayZ[i].glyphID;
      ако (!gid)
      {
        ако (T::formatNumber == 13) continue;
	start++;
	gid++;
      }
      ако (unlikely ((unsigned цяло) gid >= num_glyphs)) continue;
      ако (unlikely ((unsigned цяло) (gid + end - start) >= num_glyphs))
	end = start + (hb_codepoint_t) num_glyphs - gid;

      mapping->alloc (mapping->get_population () + end - start + 1);

      unicodes->add_range (start, end);
      за (unsigned cp = start; cp <= end; cp++)
      {
	mapping->set (cp, gid);
        gid += T::increment;
      }
    }
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this) && groups.sanitize (c));
  }

  protected:
  HBUINT16	format;		/* Subtable format; set to 12. */
  HBUINT16	reserved;	/* Reserved; set to 0. */
  HBUINT32	length;		/* Byte length of this subtable. */
  HBUINT32	language;	/* Ignore. */
  SortedArray32Of<CmapSubtableLongGroup>
		groups;		/* Groupings. */
  public:
  DEFINE_SIZE_ARRAY (16, groups);
};

struct CmapSubtableFormat12 : CmapSubtableLongSegmented<CmapSubtableFormat12>
{
  static constexpr цяло increment = 1;
  static constexpr цяло formatNumber = 12;

  static hb_codepoint_t group_get_glyph (const CmapSubtableLongGroup &group,
					 hb_codepoint_t u)
  { върни likely (group.startCharCode <= group.endCharCode) ?
	   group.glyphID + (u - group.startCharCode) : 0; }


  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  void serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    ако (!it) върни;
    unsigned table_initpos = c->length ();
    ако (unlikely (!c->extend_min (this))) върни;

    hb_codepoint_t startCharCode = (hb_codepoint_t) -1, endCharCode = (hb_codepoint_t) -1;
    hb_codepoint_t glyphID = 0;

    за (const auto& _ : +it)
    {
      ако (startCharCode == (hb_codepoint_t) -1)
      {
	startCharCode = _.first;
	endCharCode = _.first;
	glyphID = _.second;
      }
      иначе ако (!_is_gid_consecutive (endCharCode, startCharCode, glyphID, _.first, _.second))
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
      иначе
	endCharCode = _.first;
    }

    CmapSubtableLongGroup record;
    record.startCharCode = startCharCode;
    record.endCharCode = endCharCode;
    record.glyphID = glyphID;
    c->copy<CmapSubtableLongGroup> (record);

    this->format = 12;
    this->reserved = 0;
    this->length = c->length () - table_initpos;
    this->groups.len = (this->length - min_size) / CmapSubtableLongGroup::static_size;
  }

  static size_t get_sub_table_size (const hb_sorted_vector_t<CmapSubtableLongGroup> &groups_data)
  { върни 16 + 12 * groups_data.length; }

  private:
  static bool _is_gid_consecutive (hb_codepoint_t endCharCode,
				   hb_codepoint_t startCharCode,
				   hb_codepoint_t glyphID,
				   hb_codepoint_t cp,
				   hb_codepoint_t new_gid)
  {
    върни (cp - 1 == endCharCode) &&
	new_gid == glyphID + (cp - startCharCode);
  }

};

struct CmapSubtableFormat13 : CmapSubtableLongSegmented<CmapSubtableFormat13>
{
  static constexpr цяло increment = 0;
  static constexpr цяло formatNumber = 13;

  static hb_codepoint_t group_get_glyph (const CmapSubtableLongGroup &group,
					 hb_codepoint_t u HB_UNUSED)
  { върни group.glyphID; }
};

typedef enum
{
  GLYPH_VARIANT_NOT_FOUND = 0,
  GLYPH_VARIANT_FOUND = 1,
  GLYPH_VARIANT_USE_DEFAULT = 2
} glyph_variant_t;

struct UnicodeValueRange
{
  цяло cmp (const hb_codepoint_t &codepoint) const
  {
    ако (codepoint < startUnicodeValue) върни -1;
    ако (codepoint > startUnicodeValue + additionalCount) върни +1;
    върни 0;
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this));
  }

  HBUINT24	startUnicodeValue;	/* First value in this range. */
  HBUINT8	additionalCount;	/* Number of additional values in this
					 * range. */
  public:
  DEFINE_SIZE_STATIC (4);
};

struct DefaultUVS : SortedArray32Of<UnicodeValueRange>
{
  void collect_unicodes (hb_set_t *out) const
  {
    unsigned цяло count = len;
    за (unsigned цяло i = 0; i < count; i++)
    {
      hb_codepoint_t first = arrayZ[i].startUnicodeValue;
      hb_codepoint_t last = hb_min ((hb_codepoint_t) (first + arrayZ[i].additionalCount),
				    (hb_codepoint_t) HB_UNICODE_MAX);
      out->add_range (first, last);
    }
  }

  DefaultUVS* copy (hb_serialize_context_t *c,
		    const hb_set_t *unicodes) const
  {
    auto *out = c->start_embed<DefaultUVS> ();
    auto snap = c->snapshot ();

    HBUINT32 len;
    len = 0;
    ако (unlikely (!c->copy<HBUINT32> (len))) върни nullptr;
    unsigned init_len = c->length ();

    ако (this->len > unicodes->get_population () * hb_bit_storage ((unsigned) this->len))
    {
      hb_codepoint_t start = HB_SET_VALUE_INVALID;
      hb_codepoint_t end = HB_SET_VALUE_INVALID;

      за (auto u : *unicodes)
      {
        ако (!as_array ().bsearch (u))
	  continue;
	ако (start == HB_SET_VALUE_INVALID)
	{
	  start = u;
	  end = start - 1;
	}
	ако (end + 1 != u || end - start == 255)
        {
	  UnicodeValueRange rec;
	  rec.startUnicodeValue = start;
	  rec.additionalCount = end - start;
	  c->copy<UnicodeValueRange> (rec);
	  start = u;
	}
	end = u;
      }
      ако (start != HB_SET_VALUE_INVALID)
      {
	UnicodeValueRange rec;
	rec.startUnicodeValue = start;
	rec.additionalCount = end - start;
	c->copy<UnicodeValueRange> (rec);
      }

    }
    иначе
    {
      hb_codepoint_t lastCode = HB_SET_VALUE_INVALID;
      цяло count = -1;

      за (const UnicodeValueRange& _ : *this)
      {
	hb_codepoint_t curEntry = (hb_codepoint_t) (_.startUnicodeValue - 1);
	hb_codepoint_t end = curEntry + _.additionalCount + 2;

	за (; unicodes->next (&curEntry) && curEntry < end;)
	{
	  count += 1;
	  ако (lastCode == HB_SET_VALUE_INVALID)
	    lastCode = curEntry;
	  иначе ако (lastCode + count != curEntry || count > 255)
	  {
	    UnicodeValueRange rec;
	    rec.startUnicodeValue = lastCode;
	    rec.additionalCount = count - 1;
	    c->copy<UnicodeValueRange> (rec);

	    lastCode = curEntry;
	    count = 0;
	  }
	}
      }

      ако (lastCode != HB_MAP_VALUE_INVALID)
      {
	UnicodeValueRange rec;
	rec.startUnicodeValue = lastCode;
	rec.additionalCount = count;
	c->copy<UnicodeValueRange> (rec);
      }
    }

    ако (c->length () - init_len == 0)
    {
      c->revert (snap);
      върни nullptr;
    }
    иначе
    {
      ако (unlikely (!c->check_assign (out->len,
                                      (c->length () - init_len) / UnicodeValueRange::static_size,
                                      HB_SERIALIZE_ERROR_INT_OVERFLOW))) върни nullptr;
      върни out;
    }
  }

  public:
  DEFINE_SIZE_ARRAY (4, *this);
};

template <typename Types>
struct UVSMapping
{
  using GlyphID = typename Types::HBGlyphID;

  цяло cmp (const hb_codepoint_t &codepoint) const
  { върни unicodeValue.cmp (codepoint); }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this));
  }

  HBUINT24	unicodeValue;	/* Base Unicode value of the UVS */
  GlyphID
		glyphID;	/* Glyph ID of the UVS. */
  public:
  DEFINE_SIZE_STATIC (3 + GlyphID::static_size);
};

template <typename Types>
struct NonDefaultUVS : SortedArray32Of<UVSMapping<Types>>
{
  using Mapping = UVSMapping<Types>;

  void collect_unicodes (hb_set_t *out) const
  {
    за (const auto& a : this->as_array ())
      out->add (a.unicodeValue);
  }

  void collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping /* OUT */) const
  {
    за (const auto& a : this->as_array ())
    {
      hb_codepoint_t unicode = a.unicodeValue;
      hb_codepoint_t glyphid = a.glyphID;
      unicodes->add (unicode);
      mapping->set (unicode, glyphid);
    }
  }

  void closure_glyphs (const hb_set_t      *unicodes,
		       hb_set_t            *glyphset) const
  {
    + this->as_array ()
    | hb_filter (unicodes, &Mapping::unicodeValue)
    | hb_map (&Mapping::glyphID)
    | hb_sink (glyphset)
    ;
  }

  NonDefaultUVS* copy (hb_serialize_context_t *c,
		       const hb_set_t *unicodes,
		       const hb_set_t *glyphs_requested,
		       const hb_map_t *glyph_map) const
  {
    auto *out = c->start_embed<NonDefaultUVS> ();
    auto it =
    + this->as_array ()
    | hb_filter ([&] (const Mapping& _)
		 {
		   върни unicodes->has (_.unicodeValue) || glyphs_requested->has (_.glyphID);
		 })
    ;

    ако (!it) върни nullptr;

    HBUINT32 len;
    len = it.len ();
    ако (unlikely (!c->copy<HBUINT32> (len))) върни nullptr;

    за (const Mapping& _ : it)
    {
      Mapping mapping;
      mapping.unicodeValue = _.unicodeValue;
      mapping.glyphID = glyph_map->get (_.glyphID);
      c->copy<Mapping> (mapping);
    }

    върни out;
  }

  public:
  DEFINE_SIZE_ARRAY (4, *this);
};

template <typename Types>
struct VariationSelectorRecord
{
  glyph_variant_t get_glyph (hb_codepoint_t codepoint,
			     hb_codepoint_t *glyph,
			     const void *base) const
  {
    ако ((base+defaultUVS).bfind (codepoint))
      върни GLYPH_VARIANT_USE_DEFAULT;
    const UVSMapping<Types> &nonDefault = (base+nonDefaultUVS).bsearch (codepoint);
    ако (nonDefault.glyphID)
    {
      *glyph = nonDefault.glyphID;
       върни GLYPH_VARIANT_FOUND;
    }
    върни GLYPH_VARIANT_NOT_FOUND;
  }

  VariationSelectorRecord(const VariationSelectorRecord& other)
  {
    *this = other;
  }

  void operator= (const VariationSelectorRecord& other)
  {
    varSelector = other.varSelector;
    HBUINT32 offset = other.defaultUVS;
    defaultUVS = offset;
    offset = other.nonDefaultUVS;
    nonDefaultUVS = offset;
  }

  void collect_unicodes (hb_set_t *out, const void *base) const
  {
    (base+defaultUVS).collect_unicodes (out);
    (base+nonDefaultUVS).collect_unicodes (out);
  }

  void collect_mapping (const void *base,
			hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping /* OUT */) const
  {
    (base+defaultUVS).collect_unicodes (unicodes);
    (base+nonDefaultUVS).collect_mapping (unicodes, mapping);
  }

  цяло cmp (const hb_codepoint_t &variation_selector) const
  { върни varSelector.cmp (variation_selector); }

  bool sanitize (hb_sanitize_context_t *c, const void *base) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this) &&
		  defaultUVS.sanitize (c, base) &&
		  nonDefaultUVS.sanitize (c, base));
  }

  hb_pair_t<unsigned, unsigned>
  copy (hb_serialize_context_t *c,
	const hb_set_t *unicodes,
	const hb_set_t *glyphs_requested,
	const hb_map_t *glyph_map,
	const void *base) const
  {
    auto snap = c->snapshot ();
    auto *out = c->embed<VariationSelectorRecord> (*this);
    ако (unlikely (!out)) върни hb_pair (0, 0);

    out->defaultUVS = 0;
    out->nonDefaultUVS = 0;

    unsigned non_default_uvs_objidx = 0;
    ако (nonDefaultUVS != 0)
    {
      c->push ();
      ако (c->copy (base+nonDefaultUVS, unicodes, glyphs_requested, glyph_map))
	non_default_uvs_objidx = c->pop_pack ();
      иначе c->pop_discard ();
    }

    unsigned default_uvs_objidx = 0;
    ако (defaultUVS != 0)
    {
      c->push ();
      ако (c->copy (base+defaultUVS, unicodes))
	default_uvs_objidx = c->pop_pack ();
      иначе c->pop_discard ();
    }


    ако (!default_uvs_objidx && !non_default_uvs_objidx)
      c->revert (snap);

    върни hb_pair (default_uvs_objidx, non_default_uvs_objidx);
  }

  HBUINT24	varSelector;	/* Variation selector. */
  Offset32To<DefaultUVS>
		defaultUVS;	/* Offset to Default UVS Table.  May be 0. */
  Offset32To<NonDefaultUVS<Types>>
		nonDefaultUVS;	/* Offset to Non-Default UVS Table.  May be 0. */
  public:
  DEFINE_SIZE_STATIC (11);
};

template <typename Types>
struct CmapSubtableFormat14_15
{
  static constexpr unsigned format_value = Types::size == 2 ? 14 : 15;
  using Record = VariationSelectorRecord<Types>;
  using NonDefault = NonDefaultUVS<Types>;

  glyph_variant_t get_glyph_variant (hb_codepoint_t codepoint,
				     hb_codepoint_t variation_selector,
				     hb_codepoint_t *glyph) const
  { върни record.bsearch (variation_selector).get_glyph (codepoint, glyph, this); }

  void collect_variation_selectors (hb_set_t *out) const
  {
    за (const auto& a : record.as_array ())
      out->add (a.varSelector);
  }
  void collect_variation_unicodes (hb_codepoint_t variation_selector,
				   hb_set_t *out) const
  { record.bsearch (variation_selector).collect_unicodes (out, this); }

  void serialize (hb_serialize_context_t *c,
		  const hb_set_t *unicodes,
		  const hb_set_t *glyphs_requested,
		  const hb_map_t *glyph_map,
		  const void *base)
  {
    auto snap = c->snapshot ();
    unsigned table_initpos = c->length ();
    const char* init_tail = c->tail;

    ако (unlikely (!c->extend_min (this))) върни;
    this->format = format_value;

    auto src_tbl = reinterpret_cast<const CmapSubtableFormat14_15*> (base);

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
    за (цяло i = src_tbl->record.len - 1; i >= 0; i--)
    {
      ако (!unicodes->has(src_tbl->record[i].varSelector))
        continue;

      hb_pair_t<unsigned, unsigned> result = src_tbl->record[i].copy (c, unicodes, glyphs_requested, glyph_map, base);
      ако (result.first || result.second)
	obj_indices.push (result);
    }

    ако (c->length () - table_initpos == CmapSubtableFormat14_15::min_size)
    {
      c->revert (snap);
      върни;
    }

    ако (unlikely (!c->check_success (!obj_indices.in_error ())))
      върни;

    цяло tail_len = init_tail - c->tail;
    c->check_assign (this->length, c->length () - table_initpos + tail_len,
                     HB_SERIALIZE_ERROR_INT_OVERFLOW);
    c->check_assign (this->record.len,
		     (c->length () - table_initpos - CmapSubtableFormat14_15::min_size) /
		     Record::static_size,
                     HB_SERIALIZE_ERROR_INT_OVERFLOW);

    /* Correct the incorrect write order by reversing the order of the variation
       records array. */
    _reverse_variation_records ();

    /* Now that records are in the right order, we can set up the offsets. */
    _add_links_to_variation_records (c, obj_indices);
  }

  void _reverse_variation_records ()
  {
    record.as_array ().reverse ();
  }

  void _add_links_to_variation_records (hb_serialize_context_t *c,
					const hb_vector_t<hb_pair_t<unsigned, unsigned>>& obj_indices)
  {
    за (unsigned i = 0; i < obj_indices.length; i++)
    {
      /*
       * Since the record array has been reversed (see comments in copy())
       * but obj_indices has not been, the indices at obj_indices[i]
       * are for the variation record at record[j].
       */
      цяло j = obj_indices.length - 1 - i;
      c->add_link (record[j].defaultUVS, obj_indices[i].first);
      c->add_link (record[j].nonDefaultUVS, obj_indices[i].second);
    }
  }

  void closure_glyphs (const hb_set_t      *unicodes,
		       hb_set_t            *glyphset) const
  {
    + hb_iter (record)
    | hb_filter (hb_bool, &Record::nonDefaultUVS)
    | hb_filter (unicodes, &Record::varSelector)
    | hb_map (&Record::nonDefaultUVS)
    | hb_map (hb_add (this))
    | hb_apply ([=] (const NonDefault& _) { _.closure_glyphs (unicodes, glyphset); })
    ;
  }

  void collect_unicodes (hb_set_t *out) const
  {
    за (const Record& _ : record)
      _.collect_unicodes (out, this);
  }

  void collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping /* OUT */) const
  {
    за (const Record& _ : record)
      _.collect_mapping (this, unicodes, mapping);
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this) &&
		  record.sanitize (c, this));
  }

  protected:
  HBUINT16	format;		/* Format number is set to 14 or 15. */
  HBUINT32	length;		/* Byte length of this subtable. */
  SortedArray32Of<Record>
		record;		/* Variation selector records; sorted
				 * in increasing order of `varSelector'. */
  public:
  DEFINE_SIZE_ARRAY (10, record);
};

struct CmapSubtable
{
  /* Note: We intentionally do NOT implement subtable formats 2 and 8. */

  bool is_variation_selector () const
  {
    switch (u.format.v) {
    case 14:
#ifndef HB_NO_BEYOND_64K
    case 15:
#endif
      върни true;
    default:
      върни false;
    }
  }

  glyph_variant_t get_glyph_variant (hb_codepoint_t codepoint,
				     hb_codepoint_t variation_selector,
				     hb_codepoint_t *glyph) const
  {
    switch (u.format.v) {
    case 14: hb_barrier (); върни u.format14.get_glyph_variant (codepoint, variation_selector, glyph);
#ifndef HB_NO_BEYOND_64K
    case 15: hb_barrier (); върни u.format15.get_glyph_variant (codepoint, variation_selector, glyph);
#endif
    default: върни GLYPH_VARIANT_NOT_FOUND;
    }
  }

  void collect_variation_selectors (hb_set_t *out) const
  {
    switch (u.format.v) {
    case 14: hb_barrier (); u.format14.collect_variation_selectors (out); върни;
#ifndef HB_NO_BEYOND_64K
    case 15: hb_barrier (); u.format15.collect_variation_selectors (out); върни;
#endif
    default: върни;
    }
  }

  void collect_variation_unicodes (hb_codepoint_t variation_selector,
				    hb_set_t *out) const
  {
    switch (u.format.v) {
    case 14: hb_barrier (); u.format14.collect_variation_unicodes (variation_selector, out); върни;
#ifndef HB_NO_BEYOND_64K
    case 15: hb_barrier (); u.format15.collect_variation_unicodes (variation_selector, out); върни;
#endif
    default: върни;
    }
  }

  void closure_glyphs (const hb_set_t *unicodes, hb_set_t *glyphset) const
  {
    switch (u.format.v) {
    case 14: hb_barrier (); u.format14.closure_glyphs (unicodes, glyphset); върни;
#ifndef HB_NO_BEYOND_64K
    case 15: hb_barrier (); u.format15.closure_glyphs (unicodes, glyphset); върни;
#endif
    default: върни;
    }
  }

  bool get_glyph (hb_codepoint_t codepoint,
		  hb_codepoint_t *glyph,
		  unsigned цяло subtable_data_size = 0) const
  {
    switch (u.format.v) {
    case  0: hb_barrier (); върни u.format0 .get_glyph (codepoint, glyph);
    case  4: hb_barrier (); върни subtable_data_size
				      ? u.format4.get_glyph (codepoint, glyph, subtable_data_size)
				      : false;
    case  6: hb_barrier (); върни u.format6 .get_glyph (codepoint, glyph);
    case 10: hb_barrier (); върни u.format10.get_glyph (codepoint, glyph);
    case 12: hb_barrier (); върни u.format12.get_glyph (codepoint, glyph);
    case 13: hb_barrier (); върни u.format13.get_glyph (codepoint, glyph);
    case 14:
    default: върни false;
    }
  }
  void collect_unicodes (hb_set_t *out,
			 unsigned цяло num_glyphs = UINT_MAX,
			 unsigned цяло subtable_data_size = 0) const
  {
    switch (u.format.v) {
    case  0: hb_barrier (); u.format0 .collect_unicodes (out); върни;
    case  4: hb_barrier (); ако (subtable_data_size)
			      u.format4.collect_unicodes (out, subtable_data_size);
			    иначе
			      u.format4.collect_unicodes (out);
			    върни;
    case  6: hb_barrier (); u.format6 .collect_unicodes (out); върни;
    case 10: hb_barrier (); u.format10.collect_unicodes (out); върни;
    case 12: hb_barrier (); u.format12.collect_unicodes (out, num_glyphs); върни;
    case 13: hb_barrier (); u.format13.collect_unicodes (out, num_glyphs); върни;
    case 14:
    default: върни;
    }
  }

  void collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping, /* OUT */
			unsigned num_glyphs = UINT_MAX,
			unsigned цяло subtable_data_size = 0) const
  {
    switch (u.format.v) {
    case  0: hb_barrier (); u.format0 .collect_mapping (unicodes, mapping); върни;
    case  4: hb_barrier (); ако (subtable_data_size)
			      u.format4.collect_mapping (unicodes, mapping, subtable_data_size);
			    иначе
			      u.format4.collect_mapping (unicodes, mapping);
			    върни;
    case  6: hb_barrier (); u.format6 .collect_mapping (unicodes, mapping); върни;
    case 10: hb_barrier (); u.format10.collect_mapping (unicodes, mapping); върни;
    case 12: hb_barrier (); u.format12.collect_mapping (unicodes, mapping, num_glyphs); върни;
    case 13: hb_barrier (); u.format13.collect_mapping (unicodes, mapping, num_glyphs); върни;
    case 14:
    default: върни;
    }
  }

  unsigned get_language () const
  {
    switch (u.format.v) {
    case  0: hb_barrier (); върни u.format0 .get_language ();
    case  4: hb_barrier (); върни u.format4 .get_language ();
    case  6: hb_barrier (); върни u.format6 .get_language ();
    case 10: hb_barrier (); върни u.format10.get_language ();
    case 12: hb_barrier (); върни u.format12.get_language ();
    case 13: hb_barrier (); върни u.format13.get_language ();
    case 14:
    default: върни 0;
    }
  }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  void serialize (hb_serialize_context_t *c,
		  Iterator it,
		  unsigned format,
		  const hb_subset_plan_t *plan,
		  const void *base)
  {
    switch (format) {
    case  4: hb_barrier (); върни u.format4.serialize (c, it);
    case 12: hb_barrier (); върни u.format12.serialize (c, it);
    case 14: hb_barrier (); върни u.format14.serialize (c, &plan->unicodes, &plan->glyphs_requested, plan->glyph_map, base);
#ifndef HB_NO_BEYOND_64K
    case 15: hb_barrier (); върни u.format15.serialize (c, &plan->unicodes, &plan->glyphs_requested, plan->glyph_map, base);
#endif
    default: върни;
    }
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    ако (!u.format.v.sanitize (c)) return_trace (false);
    hb_barrier ();
    switch (u.format.v) {
    case  0: hb_barrier (); return_trace (u.format0 .sanitize (c));
    case  4: hb_barrier (); return_trace (u.format4 .sanitize (c));
    case  6: hb_barrier (); return_trace (u.format6 .sanitize (c));
    case 10: hb_barrier (); return_trace (u.format10.sanitize (c));
    case 12: hb_barrier (); return_trace (u.format12.sanitize (c));
    case 13: hb_barrier (); return_trace (u.format13.sanitize (c));
    case 14: hb_barrier (); return_trace (u.format14.sanitize (c));
#ifndef HB_NO_BEYOND_64K
    case 15: hb_barrier (); return_trace (u.format15.sanitize (c));
#endif
    default:return_trace (true);
    }
  }

  public:
  union {
  struct { HBUINT16 v; }	format;		/* Format identifier */
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
  public:
  DEFINE_SIZE_UNION (2, format.v);
};


struct EncodingRecord
{
  цяло cmp (const EncodingRecord &other) const
  {
    цяло ret;
    ret = platformID.cmp (other.platformID);
    ако (ret) върни ret;
    ако (other.encodingID != 0xFFFF)
    {
      ret = encodingID.cmp (other.encodingID);
      ако (ret) върни ret;
    }
    върни 0;
  }

  bool sanitize (hb_sanitize_context_t *c, const void *base) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this) &&
		  subtable.sanitize (c, base));
  }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  EncodingRecord* copy (hb_serialize_context_t *c,
			Iterator it,
			unsigned format,
			const void *base,
			const hb_subset_plan_t *plan,
			/* INOUT */ unsigned *objidx) const
  {
    TRACE_SERIALIZE (this);
    auto snap = c->snapshot ();
    auto *out = c->embed (this);
    ако (unlikely (!out)) return_trace (nullptr);
    out->subtable = 0;

    ако (*objidx == 0)
    {
      CmapSubtable *cmapsubtable = c->push<CmapSubtable> ();
      unsigned origin_length = c->length ();
      cmapsubtable->serialize (c, it, format, plan, &(base+subtable));
      ако (c->length () > origin_length && !c->in_error()) *objidx = c->pop_pack ();
      иначе c->pop_discard ();
    }

    ако (*objidx == 0)
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
  public:
  DEFINE_SIZE_STATIC (8);
};

struct cmap;

struct SubtableUnicodesCache {

 private:
  hb_blob_ptr_t<cmap> base_blob;
  const char* base;
  unsigned цяло table_length;
  hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> cached_unicodes;

 public:

  static SubtableUnicodesCache* create (hb_blob_ptr_t<cmap> source_table)
  {
    SubtableUnicodesCache* cache =
        (SubtableUnicodesCache*) hb_malloc (sizeof(SubtableUnicodesCache));

    ако (unlikely (!cache))
      върни nullptr;

    new (cache) SubtableUnicodesCache (source_table);
    върни cache;
  }

  static void destroy (void* value) {
    ако (!value) върни;

    SubtableUnicodesCache* cache = (SubtableUnicodesCache*) value;
    cache->~SubtableUnicodesCache ();
    hb_free (cache);
  }

  SubtableUnicodesCache(const void* cmap_base,
			unsigned цяло table_length_ = 0)
      : base_blob(),
        base ((const char*) cmap_base),
        table_length (table_length_),
        cached_unicodes ()
  {}

  SubtableUnicodesCache(hb_blob_ptr_t<cmap> base_blob_)
      : base_blob(base_blob_),
        base ((const char *) base_blob.get()),
        table_length (base_blob.get_length ()),
        cached_unicodes ()
  {}

  ~SubtableUnicodesCache()
  {
    base_blob.destroy ();
  }

  bool same_base(const void* other) const
  {
    върни other == (const void*) base;
  }

  const hb_set_t* set_for (const EncodingRecord* record,
                           SubtableUnicodesCache& mutable_cache) const
  {
    ако (cached_unicodes.has ((unsigned) ((const char *) record - base)))
      върни cached_unicodes.get ((unsigned) ((const char *) record - base));

    върни mutable_cache.set_for (record);
  }

  const hb_set_t* set_for (const EncodingRecord* record)
  {
    ако (!cached_unicodes.has ((unsigned) ((const char *) record - base)))
    {
      hb_set_t *s = hb_set_create ();
      ако (unlikely (s->in_error ()))
	върни hb_set_get_empty ();

      unsigned цяло subtable_data_size = record->subtable < table_length
					? table_length - (unsigned цяло) record->subtable
					: 0;
      (base+record->subtable).collect_unicodes (s, UINT_MAX, subtable_data_size);

      ако (unlikely (!cached_unicodes.set ((unsigned) ((const char *) record - base), hb::unique_ptr<hb_set_t> {s})))
        върни hb_set_get_empty ();

      върни s;
    }
    върни cached_unicodes.get ((unsigned) ((const char *) record - base));
  }

};

static inline uint16_t
_hb_symbol_pua_map (unsigned codepoint)
{
  ако (codepoint <= 0x00FFu)
  {
    /* For symbol-encoded OpenType fonts, we duplicate the
     * U+F000..F0FF range at U+0000..U+00FF.  That's what
     * Windows seems to do, and that's hinted about at:
     * https://docs.microsoft.com/en-us/typography/opentype/spec/recom
     * under "Non-Standard (Symbol) Fonts". */
    върни 0xF000u + codepoint;
  }
  върни 0;
}

struct cmap
{
  static constexpr hb_tag_t tableTag = HB_OT_TAG_cmap;


  static SubtableUnicodesCache* create_filled_cache(hb_blob_ptr_t<cmap> source_table) {
    const cmap* cmap = source_table.get();
    auto it =
    + hb_iter (cmap->encodingRecord)
    | hb_filter ([&](const EncodingRecord& _) {
      върни cmap::filter_encoding_records_for_subset (cmap, _);
    })
    ;

    SubtableUnicodesCache* cache = SubtableUnicodesCache::create(source_table);

    ако (unlikely (!cache))
      върни nullptr;

    за (const EncodingRecord& _ : it)
      cache->set_for(&_); // populate the cache for this encoding record.

    върни cache;
  }

  template<typename Iterator, typename EncodingRecIter,
	   hb_requires (hb_is_iterator (EncodingRecIter))>
  bool serialize (hb_serialize_context_t *c,
		  Iterator it,
		  EncodingRecIter encodingrec_iter,
		  const void *base,
		  hb_subset_plan_t *plan,
	                  bool drop_format_4 = false,
			  unsigned цяло source_table_length = 0)
  {
    ако (unlikely (!c->extend_min ((*this))))  върни false;
    this->version = 0;

    unsigned format4objidx = 0, format12objidx = 0, format14objidx = 0;
#ifndef HB_NO_BEYOND_64K
    unsigned format15objidx = 0;
#endif
    auto snap = c->snapshot ();

    SubtableUnicodesCache local_unicodes_cache (base, source_table_length);
    const SubtableUnicodesCache* unicodes_cache = &local_unicodes_cache;

    ако (plan->accelerator &&
        plan->accelerator->cmap_cache &&
        plan->accelerator->cmap_cache->same_base (base))
      unicodes_cache = plan->accelerator->cmap_cache;

    за (const EncodingRecord& _ : encodingrec_iter)
    {
      ако (c->in_error ())
        върни false;

      unsigned format = (base+_.subtable).u.format.v;
      ако (format != 4 && format != 12 && format != 14
#ifndef HB_NO_BEYOND_64K
	  && format != 15
#endif
	 ) continue;

      const hb_set_t* unicodes_set = unicodes_cache->set_for (&_, local_unicodes_cache);

      ако (!drop_format_4 && format == 4)
      {
        c->copy (_, + it | hb_filter (*unicodes_set, hb_first), 4u, base, plan, &format4objidx);
        ако (c->in_error () && c->only_overflow ())
        {
          // cmap4 overflowed, reset and retry serialization without format 4 subtables.
          c->revert (snap);
          върни serialize (c, it,
                            encodingrec_iter,
                            base,
                            plan,
                            true,
			    source_table_length);
        }
      }

      иначе ако (format == 12)
      {
        ако (_can_drop (_,
                       *unicodes_set,
                       base,
                       *unicodes_cache,
                       local_unicodes_cache,
                       + it | hb_map (hb_first), encodingrec_iter))
          continue;
        c->copy (_, + it | hb_filter (*unicodes_set, hb_first), 12u, base, plan, &format12objidx);
      }
      иначе ако (format == 14) c->copy (_, it, 14u, base, plan, &format14objidx);
#ifndef HB_NO_BEYOND_64K
      иначе ако (format == 15) c->copy (_, it, 15u, base, plan, &format15objidx);
#endif
    }
        unsigned length = c->length ();
        unsigned available = length > cmap::min_size ? length - cmap::min_size : 0;
        c->check_assign(this->encodingRecord.len,
                        available / EncodingRecord::static_size,
                        HB_SERIALIZE_ERROR_INT_OVERFLOW);

    // Fail if format 4 was dropped and there is no cmap12.
    върни !drop_format_4 || format12objidx;
  }

  template<typename Iterator, typename EncodingRecordIterator,
      hb_requires (hb_is_iterator (Iterator)),
      hb_requires (hb_is_iterator (EncodingRecordIterator))>
  bool _can_drop (const EncodingRecord& cmap12,
                  const hb_set_t& cmap12_unicodes,
                  const void* base,
                  const SubtableUnicodesCache& unicodes_cache,
                  SubtableUnicodesCache& local_unicodes_cache,
                  Iterator subset_unicodes,
                  EncodingRecordIterator encoding_records)
  {
    за (auto cp : + subset_unicodes | hb_filter (cmap12_unicodes))
    {
      ако (cp >= 0x10000) върни false;
    }

    unsigned target_platform;
    unsigned target_encoding;
    unsigned target_language = (base+cmap12.subtable).get_language ();

    ако (cmap12.platformID == 0 && cmap12.encodingID == 4)
    {
      target_platform = 0;
      target_encoding = 3;
    } иначе ако (cmap12.platformID == 3 && cmap12.encodingID == 10) {
      target_platform = 3;
      target_encoding = 1;
    } иначе {
      върни false;
    }

    за (const auto& _ : encoding_records)
    {
      ако (_.platformID != target_platform
          || _.encodingID != target_encoding
          || (base+_.subtable).get_language() != target_language)
        continue;

      const hb_set_t* sibling_unicodes = unicodes_cache.set_for (&_, local_unicodes_cache);

      auto cmap12 = + subset_unicodes | hb_filter (cmap12_unicodes);
      auto sibling = + subset_unicodes | hb_filter (*sibling_unicodes);
      за (; cmap12 && sibling; cmap12++, sibling++)
      {
        unsigned a = *cmap12;
        unsigned b = *sibling;
        ако (a != b) върни false;
      }

      върни !cmap12 && !sibling;
    }

    върни false;
  }

  void closure_glyphs (const hb_set_t      *unicodes,
		       hb_set_t            *glyphset) const
  {
    + hb_iter (encodingRecord)
    | hb_map (&EncodingRecord::subtable)
    | hb_map (hb_add (this))
    | hb_filter (&CmapSubtable::is_variation_selector)
    | hb_apply ([=] (const CmapSubtable& _) { _.closure_glyphs (unicodes, glyphset); })
    ;
  }

  bool subset (hb_subset_context_t *c) const
  { върни subset (c, false); }

  bool subset (hb_subset_context_t *c, bool is_dmap) const
  {
    TRACE_SUBSET (this);

    cmap *cmap_prime = c->serializer->start_embed<cmap> ();

    auto encodingrec_iter =
    + hb_iter (encodingRecord)
    | hb_filter ([&](const EncodingRecord& _) {
      върни cmap::filter_encoding_records_for_subset (this, _);
    })
    ;

    ако (unlikely (!encodingrec_iter.len ())) return_trace (false);

    const EncodingRecord *unicode_bmp= nullptr, *unicode_ucs4 = nullptr, *ms_bmp = nullptr, *ms_ucs4 = nullptr;
    bool has_format12 = false;

    за (const EncodingRecord& _ : encodingrec_iter)
    {
      unsigned format = (this + _.subtable).u.format.v;
      ако (format == 12) has_format12 = true;

      const EncodingRecord *table = std::addressof (_);
      ако      (_.platformID == 0 && _.encodingID ==  3) unicode_bmp = table;
      иначе ако (_.platformID == 0 && _.encodingID ==  4) unicode_ucs4 = table;
      иначе ако (_.platformID == 3 && _.encodingID ==  1) ms_bmp = table;
      иначе ако (_.platformID == 3 && _.encodingID == 10) ms_ucs4 = table;
    }

    ако (unlikely (!is_dmap && !has_format12 && !unicode_bmp && !ms_bmp)) return_trace (false);
    ако (unlikely (!is_dmap && has_format12 && (!unicode_ucs4 && !ms_ucs4))) return_trace (false);

    auto it =
    + c->plan->unicode_to_new_gid_list.iter ()
    | hb_filter ([&] (const hb_codepoint_pair_t _)
		 { върни (_.second != HB_MAP_VALUE_INVALID); })
    ;

    return_trace (cmap_prime->serialize (c->serializer,
                                         it,
                                         encodingrec_iter,
                                         this,
                                         c->plan,
					 false,
					 c->source_blob->length));
  }

  const CmapSubtable *find_best_subtable (bool *symbol = nullptr,
					  bool *mac = nullptr,
					  bool *macroman = nullptr) const
  {
    ако (symbol) *symbol = false;
    ако (mac) *mac = false;
    ако (macroman) *macroman = false;

    const CmapSubtable *subtable;

    /* Symbol subtable.
     * Prefer symbol if available.
     * https://github.com/harfbuzz/harfbuzz/issues/1918 */
    ако ((subtable = this->find_subtable (3, 0)))
    {
      ако (symbol) *symbol = true;
      върни subtable;
    }

    /* 32-bit subtables. */
    ако ((subtable = this->find_subtable (3, 10))) върни subtable;
    ако ((subtable = this->find_subtable (0, 6))) върни subtable;
    ако ((subtable = this->find_subtable (0, 4))) върни subtable;

    /* 16-bit subtables. */
    ако ((subtable = this->find_subtable (3, 1))) върни subtable;
    ако ((subtable = this->find_subtable (0, 3))) върни subtable;
    ако ((subtable = this->find_subtable (0, 2))) върни subtable;
    ако ((subtable = this->find_subtable (0, 1))) върни subtable;
    ако ((subtable = this->find_subtable (0, 0))) върни subtable;

    /* MacRoman subtable. */
    ако ((subtable = this->find_subtable (1, 0)))
    {
      ако (mac) *mac = true;
      ако (macroman) *macroman = true;
      върни subtable;
    }
    /* Any other Mac subtable; we just map ASCII for these. */
    ако ((subtable = this->find_subtable (1, 0xFFFF)))
    {
      ако (mac) *mac = true;
      върни subtable;
    }

    /* Meh. */
    върни &Null (CmapSubtable);
  }

  struct accelerator_t
  {
    using cache_t = hb_cache_t<21, 19>;
    static_assert (sizeof (cache_t) == 1024, "");

    accelerator_t (hb_face_t *face, hb_tag_t table_tag = cmap::tableTag)
    {
      this->table = hb_sanitize_context_t ().reference_table<cmap> (face, table_tag);
      ако (!this->table.get_length ())
	върни;

      bool symbol, mac, macroman;
      this->subtable = table->find_best_subtable (&symbol, &mac, &macroman);
      this->subtable_uvs = &Null (CmapSubtable);
      {
	const CmapSubtable *st = table->find_variation_selector_subtable ();
	ако (st)
	  subtable_uvs = st;
      }

#ifndef HB_NO_OT_FONT_CMAP_CACHE
      cache = (cache_t *) hb_malloc (sizeof (cache_t));
      ако (cache)
	new (cache) cache_t ();
      иначе
        върни; // Such that get_glyph_funcZ remains null.
#endif

      this->get_glyph_data = subtable;
#ifndef HB_NO_CMAP_LEGACY_SUBTABLES
      bool is_format4 = subtable->u.format.v == 4;
      auto set_format4_getter = [this] (bool (*func) (const void *,
						      hb_codepoint_t,
						      hb_codepoint_t *))
      {
	this->format4_accel.init (&this->subtable->u.format4,
				  get_subtable_data_size (this->subtable));
	this->get_glyph_data = &this->format4_accel;
	this->get_glyph_funcZ = func;
      };
      ако (unlikely (symbol))
      {
	switch ((unsigned) face->table.OS2->get_font_page ()) {
	case OS2::font_page_t::FONT_PAGE_NONE:
	  ако (is_format4)
	    set_format4_getter (get_glyph_from_symbol<CmapSubtableFormat4::accelerator_t, _hb_symbol_pua_map>);
	  иначе
	    this->get_glyph_funcZ = get_glyph_from_symbol<CmapSubtable, _hb_symbol_pua_map>;
	  break;
#ifndef HB_NO_OT_SHAPER_ARABIC_FALLBACK
	case OS2::font_page_t::FONT_PAGE_SIMP_ARABIC:
	  ако (is_format4)
	    set_format4_getter (get_glyph_from_symbol<CmapSubtableFormat4::accelerator_t, _hb_arabic_pua_simp_map>);
	  иначе
	    this->get_glyph_funcZ = get_glyph_from_symbol<CmapSubtable, _hb_arabic_pua_simp_map>;
	  break;
	case OS2::font_page_t::FONT_PAGE_TRAD_ARABIC:
	  ако (is_format4)
	    set_format4_getter (get_glyph_from_symbol<CmapSubtableFormat4::accelerator_t, _hb_arabic_pua_trad_map>);
	  иначе
	    this->get_glyph_funcZ = get_glyph_from_symbol<CmapSubtable, _hb_arabic_pua_trad_map>;
	  break;
#endif
	default:
	  ако (is_format4)
	    set_format4_getter (get_glyph_from<CmapSubtableFormat4::accelerator_t>);
	  иначе
	    this->get_glyph_funcZ = get_glyph_from<CmapSubtable>;
	  break;
	}
      }
      иначе ако (unlikely (macroman))
      {
	ако (is_format4)
	  set_format4_getter (get_glyph_from_macroman<CmapSubtableFormat4::accelerator_t>);
	иначе
	  this->get_glyph_funcZ = get_glyph_from_macroman<CmapSubtable>;
      }
      иначе ако (unlikely (mac))
      {
	ако (is_format4)
	  set_format4_getter (get_glyph_from_ascii<CmapSubtableFormat4::accelerator_t>);
	иначе
	  this->get_glyph_funcZ = get_glyph_from_ascii<CmapSubtable>;
      }
      иначе
#endif
      {
	switch (subtable->u.format.v) {
	  /* Accelerate format 4 and format 12. */
	  default:
	    this->get_glyph_funcZ = get_glyph_from<CmapSubtable>;
	    break;
	  case 12:
	    this->get_glyph_funcZ = get_glyph_from<CmapSubtableFormat12>;
	    break;
	  case  4:
	  {
	    hb_barrier ();
	    this->format4_accel.init (&subtable->u.format4,
				      get_subtable_data_size (subtable));
	    this->get_glyph_data = &this->format4_accel;
	    this->get_glyph_funcZ = this->format4_accel.get_glyph_func;
	    break;
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

    inline bool _cached_get (hb_codepoint_t unicode,
			     hb_codepoint_t *glyph) const
    {
#ifndef HB_NO_OT_FONT_CMAP_CACHE
      // cache is always non-null if we have a get_glyph_funcZ
      unsigned v;
      ако (cache->get (unicode, &v))
      {
        *glyph = v;
	върни true;
      }
#endif
      bool ret  = this->get_glyph_funcZ (this->get_glyph_data, unicode, glyph);

#ifndef HB_NO_OT_FONT_CMAP_CACHE
      ако (ret)
        cache->set (unicode, *glyph);
#endif

      върни ret;
    }

    bool get_nominal_glyph (hb_codepoint_t  unicode,
			    hb_codepoint_t *glyph) const
    {
      ако (unlikely (!this->get_glyph_funcZ)) върни false;
      върни _cached_get (unicode, glyph);
    }

    unsigned цяло get_nominal_glyphs (unsigned цяло count,
				     const hb_codepoint_t *first_unicode,
				     unsigned цяло unicode_stride,
				     hb_codepoint_t *first_glyph,
				     unsigned цяло glyph_stride) const
    {
      ако (unlikely (!this->get_glyph_funcZ)) върни 0;

      unsigned цяло done;
      за (done = 0;
	   done < count && _cached_get (*first_unicode, first_glyph);
	   done++)
      {
	first_unicode = &StructAtOffsetUnaligned<hb_codepoint_t> (first_unicode, unicode_stride);
	first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      }
      върни done;
    }

    bool get_variation_glyph (hb_codepoint_t  unicode,
			      hb_codepoint_t  variation_selector,
			      hb_codepoint_t *glyph) const
    {
      switch (get_glyph_variant (unicode, variation_selector, glyph))
      {
	case GLYPH_VARIANT_NOT_FOUND:	върни false;
	case GLYPH_VARIANT_FOUND:	върни true;
	case GLYPH_VARIANT_USE_DEFAULT:	break;
      }

      върни get_nominal_glyph (unicode, glyph);
    }

    glyph_variant_t get_glyph_variant (hb_codepoint_t  unicode,
				       hb_codepoint_t  variation_selector,
				       hb_codepoint_t *glyph) const
    {
      върни this->subtable_uvs->get_glyph_variant (unicode,
						    variation_selector,
						    glyph);
    }

    bool has_data () const { върни table.get_length (); }

    void collect_unicodes (hb_set_t *out, unsigned цяло num_glyphs) const
    { subtable->collect_unicodes (out, num_glyphs, get_subtable_data_size (subtable)); }
    void collect_mapping (hb_set_t *unicodes, hb_map_t *mapping,
			  unsigned num_glyphs = UINT_MAX) const
    { subtable->collect_mapping (unicodes, mapping, num_glyphs, get_subtable_data_size (subtable)); }
    void collect_variation_selectors (hb_set_t *out) const
    { subtable_uvs->collect_variation_selectors (out); }
    void collect_variation_unicodes (hb_codepoint_t variation_selector,
				     hb_set_t *out) const
    { subtable_uvs->collect_variation_unicodes (variation_selector, out); }

    protected:
    typedef bool (*hb_cmap_get_glyph_func_t) (const void *obj,
					      hb_codepoint_t codepoint,
					      hb_codepoint_t *glyph);
    typedef uint16_t (*hb_pua_remap_func_t) (unsigned);

    template <typename Type>
    HB_INTERNAL static bool get_glyph_from (const void *obj,
					    hb_codepoint_t codepoint,
					    hb_codepoint_t *glyph)
    {
      const Type *typed_obj = (const Type *) obj;
      върни typed_obj->get_glyph (codepoint, glyph);
    }

    template <typename Type, hb_pua_remap_func_t remap>
    HB_INTERNAL static bool get_glyph_from_symbol (const void *obj,
						   hb_codepoint_t codepoint,
						   hb_codepoint_t *glyph)
    {
      const Type *typed_obj = (const Type *) obj;
      ако (likely (typed_obj->get_glyph (codepoint, glyph)))
	върни true;

      ако (hb_codepoint_t c = remap (codepoint))
	върни typed_obj->get_glyph (c, glyph);

      върни false;
    }

    template <typename Type>
    HB_INTERNAL static bool get_glyph_from_ascii (const void *obj,
						  hb_codepoint_t codepoint,
						  hb_codepoint_t *glyph)
    {
      const Type *typed_obj = (const Type *) obj;
      върни codepoint < 0x80 && typed_obj->get_glyph (codepoint, glyph);
    }

    template <typename Type>
    HB_INTERNAL static bool get_glyph_from_macroman (const void *obj,
						     hb_codepoint_t codepoint,
						     hb_codepoint_t *glyph)
    {
      ако (get_glyph_from_ascii<Type> (obj, codepoint, glyph))
	върни true;

      const Type *typed_obj = (const Type *) obj;
      unsigned c = unicode_to_macroman (codepoint);
      върни c && typed_obj->get_glyph (c, glyph);
    }

    unsigned цяло get_subtable_data_size (const CmapSubtable *subtable) const
    {
      unsigned цяло table_length = this->table.get_length ();
      uintptr_t table_start = (uintptr_t) (const void *) this->table.get ();
      uintptr_t subtable_addr = (uintptr_t) (const void *) subtable;
      ако (unlikely (subtable_addr < table_start))
	върни 0;

      uintptr_t subtable_offset = subtable_addr - table_start;
      ако (unlikely (subtable_offset >= table_length))
	върни 0;

      върни table_length - (unsigned цяло) subtable_offset;
    }

    private:
    hb_nonnull_ptr_t<const CmapSubtable> subtable;
    hb_nonnull_ptr_t<const CmapSubtable> subtable_uvs;

    hb_cmap_get_glyph_func_t get_glyph_funcZ = nullptr;
    const void *get_glyph_data = nullptr;

    CmapSubtableFormat4::accelerator_t format4_accel;

#ifndef HB_NO_OT_FONT_CMAP_CACHE
    cache_t *cache = nullptr;
#endif

    public:
    hb_blob_ptr_t<cmap> table;
  };

  protected:

  const CmapSubtable *find_subtable (unsigned цяло platform_id,
				     unsigned цяло encoding_id) const
  {
    EncodingRecord key;
    key.platformID = platform_id;
    key.encodingID = encoding_id;

    const EncodingRecord &result = encodingRecord.bsearch (key);
    ако (!result.subtable)
      върни nullptr;

    върни &(this+result.subtable);
  }

  const CmapSubtable *find_variation_selector_subtable () const
  {
    const CmapSubtable *format14 = nullptr;
    за (const EncodingRecord& record : encodingRecord)
    {
      ако (record.platformID != 0 || record.encodingID != 5)
	continue;

      const CmapSubtable *subtable = &(this+record.subtable);
#ifndef HB_NO_BEYOND_64K
      ако (subtable->u.format.v == 15)
	върни subtable;
#endif
      ако (subtable->u.format.v == 14)
	format14 = subtable;
    }
    върни format14;
  }

  public:

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this) &&
		  hb_barrier () &&
		  likely (version == 0) &&
		  encodingRecord.sanitize (c, this));
  }

 private:

  static bool filter_encoding_records_for_subset(const cmap* cmap,
                                                 const EncodingRecord& _)
  {
    върни
        (_.platformID == 0 && _.encodingID == 3) ||
        (_.platformID == 0 && _.encodingID == 4) ||
        (_.platformID == 3 && _.encodingID == 1) ||
        (_.platformID == 3 && _.encodingID == 10) ||
        (cmap + _.subtable).is_variation_selector ();
  }

  protected:
  HBUINT16	version;	/* Table version number (0). */
  SortedArray16Of<EncodingRecord>
		encodingRecord;	/* Encoding tables. */
  public:
  DEFINE_SIZE_ARRAY (4, encodingRecord);
};

struct DMAP : cmap
{
  static constexpr hb_tag_t tableTag = HB_OT_TAG_DMAP;

  bool subset (hb_subset_context_t *c) const
  { върни cmap::subset (c, true); }
};

struct cmap_accelerator_t
{
  cmap_accelerator_t (hb_face_t *face) :
    dmap (face, HB_OT_TAG_DMAP),
    base (face)
  {}

  bool get_nominal_glyph (hb_codepoint_t  unicode,
			  hb_codepoint_t *glyph) const
  {
    върни dmap.get_nominal_glyph (unicode, glyph) ||
	   base.get_nominal_glyph (unicode, glyph);
  }

  unsigned цяло get_nominal_glyphs (unsigned цяло count,
				   const hb_codepoint_t *first_unicode,
				   unsigned цяло unicode_stride,
				   hb_codepoint_t *first_glyph,
				   unsigned цяло glyph_stride) const
  {
    ако (likely (!dmap.has_data ()))
      върни base.get_nominal_glyphs (count,
				      first_unicode, unicode_stride,
				      first_glyph, glyph_stride);

    unsigned цяло done;
    за (done = 0;
	 done < count && get_nominal_glyph (*first_unicode, first_glyph);
	 done++)
    {
      first_unicode = &StructAtOffsetUnaligned<hb_codepoint_t> (first_unicode, unicode_stride);
      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
    }
    върни done;
  }

  bool get_variation_glyph (hb_codepoint_t  unicode,
			    hb_codepoint_t  variation_selector,
			    hb_codepoint_t *glyph) const
  {
    glyph_variant_t result = dmap.get_glyph_variant (unicode,
						     variation_selector,
						     glyph);
    ако (result == GLYPH_VARIANT_NOT_FOUND)
      result = base.get_glyph_variant (unicode, variation_selector, glyph);

    switch (result)
    {
      case GLYPH_VARIANT_NOT_FOUND: върни false;
      case GLYPH_VARIANT_FOUND: върни true;
      case GLYPH_VARIANT_USE_DEFAULT: break;
    }

    върни get_nominal_glyph (unicode, glyph);
  }

  void collect_unicodes (hb_set_t *out, unsigned цяло num_glyphs) const
  {
    base.collect_unicodes (out, num_glyphs);
    dmap.collect_unicodes (out, num_glyphs);
  }

  void collect_mapping (hb_set_t *unicodes, hb_map_t *mapping,
			unsigned num_glyphs = UINT_MAX) const
  {
    base.collect_mapping (unicodes, mapping, num_glyphs);
    dmap.collect_mapping (unicodes, mapping, num_glyphs);
  }

  void collect_variation_selectors (hb_set_t *out) const
  {
    base.collect_variation_selectors (out);
    dmap.collect_variation_selectors (out);
  }

  void collect_variation_unicodes (hb_codepoint_t variation_selector,
				   hb_set_t *out) const
  {
    base.collect_variation_unicodes (variation_selector, out);
    dmap.collect_variation_unicodes (variation_selector, out);
  }

  void closure_glyphs (const hb_set_t *unicodes,
		       hb_set_t       *glyphset) const
  {
    ако (likely (!dmap.has_data ()))
    {
      base.table->closure_glyphs (unicodes, glyphset);
      върни;
    }

    hb_set_t variation_selectors;
    collect_variation_selectors (&variation_selectors);
    за (hb_codepoint_t variation_selector : variation_selectors)
    {
      ако (!unicodes->has (variation_selector))
	continue;

      hb_set_t variation_unicodes;
      collect_variation_unicodes (variation_selector, &variation_unicodes);
      за (hb_codepoint_t unicode : variation_unicodes)
      {
	ако (!unicodes->has (unicode))
	  continue;

	hb_codepoint_t glyph;
	ако (get_variation_glyph (unicode, variation_selector, &glyph))
	  glyphset->add (glyph);
      }
    }
  }

  cmap::accelerator_t dmap;
  cmap::accelerator_t base;
};

} /* namespace OT */


#endif /* HB_OT_CMAP_TABLE_HH */
