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

#अगर_अघोषित HB_OT_CMAP_TABLE_HH
#घोषणा HB_OT_CMAP_TABLE_HH

#समावेश "hb-ot-os2-table.hh"
#समावेश "hb-ot-shaper-arabic-pua.hh"
#समावेश "hb-open-type.hh"
#समावेश "hb-ot-dual.hh"
#समावेश "hb-set.hh"
#समावेश "hb-cache.hh"

/*
 * cmap -- Character to Glyph Index Mapping
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cmap
 */
#घोषणा HB_OT_TAG_cmap HB_TAG('c','m','a','p')
#घोषणा HB_OT_TAG_DMAP HB_TAG('D','M','A','P')

namespace OT {

अटल अंतरभूत uint8_t unicode_to_macroman (hb_codepoint_t u)
{
  अटल स्थिर काष्ठा unicode_to_macroman_t
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
  स्वतः *c = hb_bsearch (u, mapping, ARRAY_लंबाई (mapping), माप (mapping[0]),
			_hb_cmp_operator<uint16_t, uint16_t>);
  वापस c ? c->macroman : 0;
}

काष्ठा CmapSubtableFormat0
{
  bool get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) स्थिर
  {
    hb_codepoint_t gid = codepoint < 256 ? glyphIdArray[codepoint] : 0;
    अगर (unlikely (!gid))
      वापस false;
    *glyph = gid;
    वापस true;
  }

  अचिन्हित get_language () स्थिर
  {
    वापस language;
  }

  व्योम collect_unicodes (hb_set_t *out) स्थिर
  {
    क्रम (अचिन्हित पूर्णांक i = 0; i < 256; i++)
      अगर (glyphIdArray[i])
	out->add (i);
  }

  व्योम collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping /* OUT */) स्थिर
  {
    क्रम (अचिन्हित i = 0; i < 256; i++)
      अगर (glyphIdArray[i])
      {
	hb_codepoint_t glyph = glyphIdArray[i];
	unicodes->add (i);
	mapping->set (i, glyph);
      }
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह));
  }

  रक्षित:
  HBUINT16	format;		/* Format number is set to 0. */
  HBUINT16	लंबाई;		/* Byte length of this subtable. */
  HBUINT16	language;	/* Ignore. */
  HBUINT8	glyphIdArray[256];/* An array that maps character
				 * code to glyph index values. */
  खुला:
  DEFINE_आकार_STATIC (6 + 256);
};

काष्ठा CmapSubtableFormat4
{
  ढाँचा<typename Iterator,
      typename Writer,
	   hb_requires (hb_is_iterator (Iterator))>
  व्योम to_ranges (Iterator it, Writer& range_writer)
  {
    hb_codepoint_t start_cp = 0, पूर्व_run_start_cp = 0, run_start_cp = 0, end_cp = 0, last_gid = 0;
    पूर्णांक run_लंबाई = 0 , delta = 0, पूर्व_delta = 0;

    क्रमागत {
      FIRST_SUB_RANGE,
      FOLLOWING_SUB_RANGE,
    } mode;

    जबतक (it) {
      // Start a new range
      {
        स्थिर स्वतः& pair = *it;
        start_cp = pair.first;
        पूर्व_run_start_cp = start_cp;
        run_start_cp = start_cp;
        end_cp = start_cp;
        last_gid = pair.second;
        run_लंबाई = 1;
        पूर्व_delta = 0;
      }

      delta = last_gid - start_cp;
      mode = FIRST_SUB_RANGE;
      it++;

      जबतक (it) {
        // Process range
        स्थिर स्वतः& pair = *it;
        hb_codepoint_t अग्र_cp = pair.first;
        hb_codepoint_t अग्र_gid = pair.second;
        अगर (अग्र_cp != end_cp + 1) {
          // Current range is over, stop processing.
          अवरोध;
        }

        अगर (अग्र_gid == last_gid + 1) {
          // The current run continues.
          end_cp = अग्र_cp;
          run_लंबाई++;
          last_gid = अग्र_gid;
          it++;
          जारी;
        }

        // A new run is starting, decide if we want to commit the current run.
        पूर्णांक split_cost = (mode == FIRST_SUB_RANGE) ? 8 : 16;
        पूर्णांक run_cost = run_लंबाई * 2;
        अगर (run_cost >= split_cost) {
          commit_current_range(start_cp,
                               पूर्व_run_start_cp,
                               run_start_cp,
                               end_cp,
                               delta,
                               पूर्व_delta,
                               split_cost,
                               range_writer);
          start_cp = अग्र_cp;
        }

        // Start the new run
        mode = FOLLOWING_SUB_RANGE;
        पूर्व_run_start_cp = run_start_cp;
        run_start_cp = अग्र_cp;
        end_cp = अग्र_cp;
        पूर्व_delta = delta;
        delta = अग्र_gid - run_start_cp;
        run_लंबाई = 1;
        last_gid = अग्र_gid;
        it++;
      }

      // Finalize range
      commit_current_range (start_cp,
                            पूर्व_run_start_cp,
                            run_start_cp,
                            end_cp,
                            delta,
                            पूर्व_delta,
                            8,
                            range_writer);
    }

    अगर (likely (end_cp != 0xFFFF)) {
      range_writer (0xFFFF, 0xFFFF, 1);
    }
  }

  /*
   * Writes the current range as either one or two ranges depending on what is most efficient.
   */
  ढाँचा<typename Writer>
  व्योम commit_current_range (hb_codepoint_t start,
                             hb_codepoint_t पूर्व_run_start,
                             hb_codepoint_t run_start,
                             hb_codepoint_t end,
                             पूर्णांक run_delta,
                             पूर्णांक previous_run_delta,
                             पूर्णांक split_cost,
                             Writer& range_writer) {
    bool should_split = false;
    अगर (start < run_start && run_start < end) {
      पूर्णांक run_cost = (end - run_start + 1) * 2;
      अगर (run_cost >= split_cost) {
        should_split = true;
      }
    }

    // A zero delta is also the marker for ranges that need a
    // glyphIdArray; serialize_rangeoffset_glyid() tells the two cases
    // apart by re-checking the mapping.
    अगर (should_split) {
      अगर (start == पूर्व_run_start)
        range_writer (start, run_start - 1, previous_run_delta);
      अन्यथा
        range_writer (start, run_start - 1, 0);
      range_writer (run_start, end, run_delta);
      वापस;
    }


    अगर (start == run_start) {
      // Range is only a run
      range_writer (start, end, run_delta);
      वापस;
    }

    // Write only a single non-run range.
    range_writer (start, end, 0);
  }

  ढाँचा<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  अचिन्हित serialize_find_segcount (Iterator it) {
    काष्ठा गणक {
      अचिन्हित segcount = 0;

      व्योम चालक() (hb_codepoint_t start,
                       hb_codepoint_t end,
                       पूर्णांक delta) {
        segcount++;
      }
    } गणक_2;

    to_ranges (+it, गणक_2);
    वापस गणक_2.segcount;
  }


  ढाँचा<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  bool serialize_start_end_delta_arrays (hb_serialize_context_t *c,
                                         Iterator it,
                                         पूर्णांक segcount)
  {
    काष्ठा Writer {
      hb_serialize_context_t *serializer_;
      HBUINT16* end_code_;
      HBUINT16* start_code_;
      HBINT16* id_delta_;
      पूर्णांक सूचकांक_3;

      Writer(hb_serialize_context_t *serializer)
          : serializer_(serializer),
            end_code_(nullptr),
            start_code_(nullptr),
            id_delta_(nullptr),
            सूचकांक_3 (0) {}
      व्योम चालक() (hb_codepoint_t start,
                       hb_codepoint_t end,
                       पूर्णांक delta) {
        start_code_[सूचकांक_3] = start;
        end_code_[सूचकांक_3] = end;
        id_delta_[सूचकांक_3] = delta;
        सूचकांक_3++;
      }
    } writer(c);

    writer.end_code_ = c->allocate_आकार<HBUINT16> (HBUINT16::static_आकार * segcount, false);
    (व्योम) c->allocate_आकार<HBUINT16> (2); // padding
    writer.start_code_ = c->allocate_आकार<HBUINT16> (HBUINT16::static_आकार * segcount, false);
    writer.id_delta_ = c->allocate_आकार<HBINT16> (HBINT16::static_आकार * segcount, false);

    अगर (unlikely (!writer.end_code_ || !writer.start_code_ || !writer.id_delta_)) वापस false;

    to_ranges (+it, writer);
    वापस true;
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

    HBUINT16 *idRangeOffset = c->allocate_आकार<HBUINT16> (HBUINT16::static_आकार * segcount);
    अगर (unlikely (!c->check_success (idRangeOffset))) वापस nullptr;
    अगर (unlikely ((अक्षर *)idRangeOffset - (अक्षर *)idDelta != (पूर्णांक) segcount * (पूर्णांक) HBINT16::static_आकार)) वापस nullptr;

    क्रम (अचिन्हित i : hb_range (segcount))
    {
      /* A non-zero delta is always a real run. A zero delta is
       * ambiguous: identity mapping, or the marker for a range that
       * needs a glyphIdArray. Only zero deltas need a check. */
      पूर्णांक delta = idDelta[i];
      अगर (delta)
	जारी;
      bool use_delta = true;
      क्रम (hb_codepoint_t cp = startCode[i]; cp <= endCode[i]; cp++)
      {
	hb_codepoint_t gid = cp_to_gid.लो (cp);
	अगर (gid == HB_MAP_मूल्य_INVALID)
	  gid = 0;
	अगर (gid != cp)
	{
	  use_delta = false;
	  अवरोध;
	}
      }
      अगर (use_delta)
	जारी;

      idRangeOffset[i] = 2 * (c->start_embed<HBUINT16> () - idRangeOffset - i);
      क्रम (hb_codepoint_t cp = startCode[i]; cp <= endCode[i]; cp++)
      {
        HBUINT16 gid;
        gid = cp_to_gid[cp];
        c->copy<HBUINT16> (gid);
      }
    }

    वापस idRangeOffset;
  }

  ढाँचा<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  व्योम serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    स्वतः format4_iter =
    + it
    | hb_filter ([&] (स्थिर hb_codepoint_pair_t _)
		 { वापस _.first <= 0xFFFF; })
    ;

    अगर (!format4_iter) वापस;

    अचिन्हित table_initpos = c->लंबाई ();
    अगर (unlikely (!c->extend_min (यह))) वापस;
    यह->format = 4;

    hb_vector_t<hb_codepoint_pair_t> cp_to_gid {
      format4_iter
    };

    //serialize endCode[], startCode[], idDelta[]
    HBUINT16* endCode = c->start_embed<HBUINT16> ();
    अचिन्हित segcount = serialize_find_segcount (cp_to_gid.iter());
    अगर (unlikely (!serialize_start_end_delta_arrays (c, cp_to_gid.iter(), segcount)))
      वापस;

    HBUINT16 *startCode = endCode + segcount + 1;
    HBINT16 *idDelta = ((HBINT16*)startCode) + segcount;

    HBUINT16 *idRangeOffset = serialize_rangeoffset_glyid (c,
                                                           cp_to_gid.iter (),
                                                           endCode,
                                                           startCode,
                                                           idDelta,
                                                           segcount);
    अगर (unlikely (!c->check_success (idRangeOffset))) वापस;

    यह->लंबाई = c->लंबाई () - table_initpos;
    अगर ((दीर्घ दीर्घ) यह->लंबाई != (दीर्घ दीर्घ) c->लंबाई () - table_initpos)
    {
      c->err (HB_SERIALIZE_ERROR_INT_OVERFLOW);
      वापस;
    }

    यह->seg_गिनती_X2 = segcount * 2;
    यह->entrySelector = hb_max (1u, hb_bit_storage (segcount)) - 1;
    यह->खोज_Range = 2 * (1u << यह->entrySelector);
    यह->rangeShift = segcount * 2 > यह->खोज_Range
		       ? 2 * segcount - यह->खोज_Range
		       : 0;
  }

  अचिन्हित get_language () स्थिर
  {
    वापस language;
  }

  काष्ठा accelerator_t
  {
    accelerator_t () {}
    accelerator_t (स्थिर CmapSubtableFormat4 *subtable) = मिटाओ;

    व्योम init (स्थिर CmapSubtableFormat4 *subtable,
	       अचिन्हित पूर्णांक              subtable_तथ्य_आकार)
    {
      seg_गिनती_2 = subtable->seg_गिनती_X2 / 2;
      end_गिनती = subtable->values.arrayZ;
      start_गिनती = end_गिनती + seg_गिनती_2 + 1;
      idDelta = start_गिनती + seg_गिनती_2;
      idRangeOffset = idDelta + seg_गिनती_2;
      glyphIdArray = idRangeOffset + seg_गिनती_2;

      अचिन्हित पूर्णांक मूल्यावली_offset = 16 + 8 * seg_गिनती_2;
      glyph_Id_Array_लंबाई = subtable_तथ्य_आकार > मूल्यावली_offset
			   ? (subtable_तथ्य_आकार - मूल्यावली_offset) / 2
			   : 0;
    }

    bool get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) स्थिर
    {
      काष्ठा CustomRange
      {
	पूर्णांक cmp (hb_codepoint_t k,
		 अचिन्हित distance) स्थिर
	{
	  अगर (k > last) वापस +1;
	  अगर (k < (&last)[distance]/*first*/) वापस -1;
	  वापस 0;
	}
	HBUINT16 last;
      };

      स्थिर HBUINT16 *found = hb_bsearch (codepoint,
					  यह->end_गिनती,
					  यह->seg_गिनती_2,
					  माप (CustomRange),
					  _hb_cmp_method<hb_codepoint_t, CustomRange, अचिन्हित>,
					  यह->seg_गिनती_2 + 1);
      अगर (unlikely (!found))
	वापस false;
      अचिन्हित पूर्णांक i = found - end_गिनती;

      hb_codepoint_t gid;
      अचिन्हित पूर्णांक rangeOffset = यह->idRangeOffset[i];
      अगर (rangeOffset == 0)
	gid = codepoint + यह->idDelta[i];
      अन्यथा
      {
	/* Somebody has been smoking... */
	अचिन्हित पूर्णांक सूचकांक_2 = rangeOffset / 2 + (codepoint - यह->start_गिनती[i]) + i - यह->seg_गिनती_2;
	अगर (unlikely (सूचकांक_2 >= यह->glyph_Id_Array_लंबाई))
	  वापस false;
	gid = यह->glyphIdArray[सूचकांक_2];
	अगर (unlikely (!gid))
	  वापस false;
	gid += यह->idDelta[i];
      }
      gid &= 0xFFFFu;
      अगर (unlikely (!gid))
	वापस false;
      *glyph = gid;
      वापस true;
    }

    HB_INTERNAL अटल bool get_glyph_func (स्थिर व्योम *obj, hb_codepoint_t codepoint, hb_codepoint_t *glyph)
    { वापस ((स्थिर accelerator_t *) obj)->get_glyph (codepoint, glyph); }

    व्योम collect_unicodes (hb_set_t *out) स्थिर
    {
      अचिन्हित पूर्णांक गिनती = यह->seg_गिनती_2;
      अगर (गिनती && यह->start_गिनती[गिनती - 1] == 0xFFFFu)
	गिनती--; /* Skip sentinel segment. */
      क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
      {
	hb_codepoint_t start = यह->start_गिनती[i];
	hb_codepoint_t end = यह->end_गिनती[i];
	अचिन्हित पूर्णांक rangeOffset = यह->idRangeOffset[i];
        out->add_range(start, end);
	अगर (rangeOffset == 0)
	{
	  क्रम (hb_codepoint_t codepoint = start; codepoint <= end; codepoint++)
	  {
	    hb_codepoint_t gid = (codepoint + यह->idDelta[i]) & 0xFFFFu;
	    अगर (unlikely (!gid))
              out->del(codepoint);
	  }
	}
	अन्यथा
	{
	  क्रम (hb_codepoint_t codepoint = start; codepoint <= end; codepoint++)
	  {
	    अचिन्हित पूर्णांक सूचकांक_2 = rangeOffset / 2 + (codepoint - यह->start_गिनती[i]) + i - यह->seg_गिनती_2;
	    अगर (unlikely (सूचकांक_2 >= यह->glyph_Id_Array_लंबाई))
            {
              out->del_range (codepoint, end);
	      अवरोध;
            }
	    hb_codepoint_t gid = यह->glyphIdArray[सूचकांक_2];
	    अगर (unlikely (!gid))
              out->del(codepoint);
	  }
	}
      }
    }

    व्योम collect_mapping (hb_set_t *unicodes, /* OUT */
			  hb_map_t *mapping /* OUT */) स्थिर
    {
      // TODO(grieger): optimize similar to collect_unicodes
      // (ie. use add_range())
      अचिन्हित गिनती = यह->seg_गिनती_2;
      अगर (गिनती && यह->start_गिनती[गिनती - 1] == 0xFFFFu)
	गिनती--; /* Skip sentinel segment. */
      क्रम (अचिन्हित i = 0; i < गिनती; i++)
      {
	hb_codepoint_t start = यह->start_गिनती[i];
	hb_codepoint_t end = यह->end_गिनती[i];
	अचिन्हित rangeOffset = यह->idRangeOffset[i];
	अगर (rangeOffset == 0)
	{
	  क्रम (hb_codepoint_t codepoint = start; codepoint <= end; codepoint++)
	  {
	    hb_codepoint_t gid = (codepoint + यह->idDelta[i]) & 0xFFFFu;
	    अगर (unlikely (!gid))
	      जारी;
	    unicodes->add (codepoint);
	    mapping->set (codepoint, gid);
	  }
	}
	अन्यथा
	{
	  क्रम (hb_codepoint_t codepoint = start; codepoint <= end; codepoint++)
	  {
	    अचिन्हित सूचकांक_2 = rangeOffset / 2 + (codepoint - यह->start_गिनती[i]) + i - यह->seg_गिनती_2;
	    अगर (unlikely (सूचकांक_2 >= यह->glyph_Id_Array_लंबाई))
	      अवरोध;
	    hb_codepoint_t gid = यह->glyphIdArray[सूचकांक_2];
	    अगर (unlikely (!gid))
	      जारी;
            gid = (gid + यह->idDelta[i]) & 0xFFFFu;
	    unicodes->add (codepoint);
	    mapping->set (codepoint, gid);
	  }
	}
      }
    }

    स्थिर HBUINT16 *end_गिनती;
    स्थिर HBUINT16 *start_गिनती;
    स्थिर HBUINT16 *idDelta;
    स्थिर HBUINT16 *idRangeOffset;
    स्थिर HBUINT16 *glyphIdArray;
    अचिन्हित पूर्णांक seg_गिनती_2;
    अचिन्हित पूर्णांक glyph_Id_Array_लंबाई;
  };

  bool get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph,
		  अचिन्हित पूर्णांक subtable_तथ्य_आकार) स्थिर
  {
    accelerator_t accel;
    accel.init (यह, subtable_तथ्य_आकार);
    वापस accel.get_glyph_func (&accel, codepoint, glyph);
  }
  bool get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) स्थिर
  { वापस false; }

  व्योम collect_unicodes (hb_set_t *out, अचिन्हित पूर्णांक subtable_तथ्य_आकार) स्थिर
  {
    accelerator_t accel;
    accel.init (यह, subtable_तथ्य_आकार);
    accel.collect_unicodes (out);
  }
  व्योम collect_unicodes (hb_set_t *out) स्थिर
  { collect_unicodes (out, लंबाई); }

  व्योम collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping, /* OUT */
			अचिन्हित पूर्णांक subtable_तथ्य_आकार) स्थिर
  {
    accelerator_t accel;
    accel.init (यह, subtable_तथ्य_आकार);
    accel.collect_mapping (unicodes, mapping);
  }
  व्योम collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping /* OUT */) स्थिर
  { collect_mapping (unicodes, mapping, लंबाई); }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    अगर (unlikely (!c->check_struct (यह)))
      return_trace (false);
    hb_barrier ();

    return_trace (c->check_range (values, 2 + 4 * seg_गिनती_X2));
  }

  रक्षित:
  HBUINT16	format;		/* Format number is set to 4. */
  HBUINT16	लंबाई;		/* This is the length in bytes of the
				 * subtable. */
  HBUINT16	language;	/* Ignore. */
  HBUINT16	seg_गिनती_X2;	/* 2 x segCount. */
  HBUINT16	खोज_Range;	/* 2 * (2**floor(log2(segCount))) */
  HBUINT16	entrySelector;	/* log2(searchRange/2) */
  HBUINT16	rangeShift;	/* 2 x segCount - searchRange */

  UnsizedArrayOf<HBUINT16>
		values;
#अगर 0
  HBUINT16	end_गिनती[seg_गिनती_2];	/* End characterCode for each segment,
					 * last=0xFFFFu. */
  HBUINT16	reservedPad;		/* Set to 0. */
  HBUINT16	start_गिनती[seg_गिनती_2];	/* Start character code for each segment. */
  HBINT16		idDelta[seg_गिनती_2];	/* Delta for all character codes in segment. */
  HBUINT16	idRangeOffset[seg_गिनती_2];/* Offsets into glyphIdArray or 0 */
  UnsizedArrayOf<HBUINT16>
		glyphIdArray;	/* Glyph index array (arbitrary length) */
#पूर्ण_अगर

  खुला:
  DEFINE_आकार_ARRAY (14, values);
};

काष्ठा CmapSubtableLongGroup
{
  मित्र काष्ठा CmapSubtableFormat12;
  मित्र काष्ठा CmapSubtableFormat13;
  ढाँचा<typename U>
  मित्र काष्ठा CmapSubtableLongSegmented;
  मित्र काष्ठा cmap;

  पूर्णांक cmp (hb_codepoint_t codepoint) स्थिर
  {
    अगर (codepoint < startCharCode) वापस -1;
    अगर (codepoint > endCharCode)   वापस +1;
    वापस 0;
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह));
  }

  निजी:
  HBUINT32		startCharCode;	/* First character code in this group. */
  HBUINT32		endCharCode;	/* Last character code in this group. */
  HBUINT32		glyphID;	/* Glyph index; interpretation depends on
					 * subtable format. */
  खुला:
  DEFINE_आकार_STATIC (12);
};
DECLARE_NULL_NAMESPACE_BYTES (OT, CmapSubtableLongGroup);

ढाँचा <typename UINT>
काष्ठा CmapSubtableTrimmed
{
  bool get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) स्थिर
  {
    /* Rely on our implicit array bound-checking. */
    hb_codepoint_t gid = glyphIdArray[codepoint - startCharCode];
    अगर (unlikely (!gid))
      वापस false;
    *glyph = gid;
    वापस true;
  }

  अचिन्हित get_language () स्थिर
  {
    वापस language;
  }

  व्योम collect_unicodes (hb_set_t *out) स्थिर
  {
    hb_codepoint_t start = startCharCode;
    अचिन्हित पूर्णांक गिनती = glyphIdArray.len;
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
      अगर (glyphIdArray[i])
	out->add (start + i);
  }

  व्योम collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping /* OUT */) स्थिर
  {
    hb_codepoint_t start_cp = startCharCode;
    अचिन्हित गिनती = glyphIdArray.len;
    क्रम (अचिन्हित i = 0; i < गिनती; i++)
      अगर (glyphIdArray[i])
      {
	hb_codepoint_t unicode = start_cp + i;
	hb_codepoint_t glyphid = glyphIdArray[i];
	unicodes->add (unicode);
	mapping->set (unicode, glyphid);
      }
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) && glyphIdArray.sanitize (c));
  }

  रक्षित:
  UINT		formatReserved;	/* Subtable format and (maybe) padding. */
  UINT		लंबाई;		/* Byte length of this subtable. */
  UINT		language;	/* Ignore. */
  UINT		startCharCode;	/* First character code covered. */
  ArrayOf<HBGlyphID16, UINT>
		glyphIdArray;	/* Array of glyph index values for character
				 * codes in the range. */
  खुला:
  DEFINE_आकार_ARRAY (5 * माप (UINT), glyphIdArray);
};

काष्ठा CmapSubtableFormat6  : CmapSubtableTrimmed<HBUINT16> {};
काष्ठा CmapSubtableFormat10 : CmapSubtableTrimmed<HBUINT32> {};

ढाँचा <typename T>
काष्ठा CmapSubtableLongSegmented
{
  मित्र काष्ठा cmap;

  bool get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) स्थिर
  {
    hb_codepoint_t gid = T::group_get_glyph (groups.द्वा_खोज (codepoint), codepoint);
    अगर (unlikely (!gid))
      वापस false;
    *glyph = gid;
    वापस true;
  }

  अचिन्हित get_language () स्थिर
  {
    वापस language;
  }

  व्योम collect_unicodes (hb_set_t *out, अचिन्हित पूर्णांक अंक_glyphs) स्थिर
  {
    क्रम (अचिन्हित पूर्णांक i = 0; i < यह->groups.len; i++)
    {
      hb_codepoint_t start = यह->groups[i].startCharCode;
      hb_codepoint_t end = hb_min ((hb_codepoint_t) यह->groups[i].endCharCode,
				   (hb_codepoint_t) HB_UNICODE_MAX);
      hb_codepoint_t gid = यह->groups[i].glyphID;
      अगर (!gid)
      {
	/* Intention is: if (hb_is_same (T, CmapSubtableFormat13)) continue; */
	अगर (! T::group_get_glyph (यह->groups[i], end)) जारी;
	start++;
	gid++;
      }
      अगर (unlikely ((अचिन्हित पूर्णांक) gid >= अंक_glyphs)) जारी;
      अगर (unlikely ((अचिन्हित पूर्णांक) (gid + end - start) >= अंक_glyphs))
	end = start + (hb_codepoint_t) अंक_glyphs - gid;

      out->add_range (start, hb_min (end, 0x10FFFFu));
    }
  }

  व्योम collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping, /* OUT */
			अचिन्हित अंक_glyphs) स्थिर
  {
    hb_codepoint_t last_end = 0;
    अचिन्हित गिनती = यह->groups.len;
    क्रम (अचिन्हित i = 0; i < गिनती; i++)
    {
      hb_codepoint_t start = यह->groups.arrayZ[i].startCharCode;
      hb_codepoint_t end = hb_min ((hb_codepoint_t) यह->groups.arrayZ[i].endCharCode,
				   (hb_codepoint_t) HB_UNICODE_MAX);
      अगर (unlikely (start > end || start < last_end)) {
        // Range is not in order and is invalid, skip it.
        जारी;
      }
      last_end = end;


      hb_codepoint_t gid = यह->groups.arrayZ[i].glyphID;
      अगर (!gid)
      {
        अगर (T::format_संख्या == 13) जारी;
	start++;
	gid++;
      }
      अगर (unlikely ((अचिन्हित पूर्णांक) gid >= अंक_glyphs)) जारी;
      अगर (unlikely ((अचिन्हित पूर्णांक) (gid + end - start) >= अंक_glyphs))
	end = start + (hb_codepoint_t) अंक_glyphs - gid;

      mapping->alloc (mapping->get_population () + end - start + 1);

      unicodes->add_range (start, end);
      क्रम (अचिन्हित cp = start; cp <= end; cp++)
      {
	mapping->set (cp, gid);
        gid += T::increment;
      }
    }
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) && groups.sanitize (c));
  }

  रक्षित:
  HBUINT16	format;		/* Subtable format; set to 12. */
  HBUINT16	reserved;	/* Reserved; set to 0. */
  HBUINT32	लंबाई;		/* Byte length of this subtable. */
  HBUINT32	language;	/* Ignore. */
  SortedArray32Of<CmapSubtableLongGroup>
		groups;		/* Groupings. */
  खुला:
  DEFINE_आकार_ARRAY (16, groups);
};

काष्ठा CmapSubtableFormat12 : CmapSubtableLongSegmented<CmapSubtableFormat12>
{
  अटल constexpr पूर्णांक increment = 1;
  अटल constexpr पूर्णांक format_संख्या = 12;

  अटल hb_codepoint_t group_get_glyph (स्थिर CmapSubtableLongGroup &group,
					 hb_codepoint_t u)
  { वापस likely (group.startCharCode <= group.endCharCode) ?
	   group.glyphID + (u - group.startCharCode) : 0; }


  ढाँचा<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  व्योम serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    अगर (!it) वापस;
    अचिन्हित table_initpos = c->लंबाई ();
    अगर (unlikely (!c->extend_min (यह))) वापस;

    hb_codepoint_t startCharCode = (hb_codepoint_t) -1, endCharCode = (hb_codepoint_t) -1;
    hb_codepoint_t glyphID = 0;

    क्रम (स्थिर स्वतः& _ : +it)
    {
      अगर (startCharCode == (hb_codepoint_t) -1)
      {
	startCharCode = _.first;
	endCharCode = _.first;
	glyphID = _.second;
      }
      अन्यथा अगर (!_is_gid_consecutive (endCharCode, startCharCode, glyphID, _.first, _.second))
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

    यह->format = 12;
    यह->reserved = 0;
    यह->लंबाई = c->लंबाई () - table_initpos;
    यह->groups.len = (यह->लंबाई - min_आकार) / CmapSubtableLongGroup::static_आकार;
  }

  अटल माप_प्रकार get_sub_table_आकार (स्थिर hb_sorted_vector_t<CmapSubtableLongGroup> &groups_तथ्य)
  { वापस 16 + 12 * groups_तथ्य.लंबाई; }

  निजी:
  अटल bool _is_gid_consecutive (hb_codepoint_t endCharCode,
				   hb_codepoint_t startCharCode,
				   hb_codepoint_t glyphID,
				   hb_codepoint_t cp,
				   hb_codepoint_t new_gid)
  {
    वापस (cp - 1 == endCharCode) &&
	new_gid == glyphID + (cp - startCharCode);
  }

};

काष्ठा CmapSubtableFormat13 : CmapSubtableLongSegmented<CmapSubtableFormat13>
{
  अटल constexpr पूर्णांक increment = 0;
  अटल constexpr पूर्णांक format_संख्या = 13;

  अटल hb_codepoint_t group_get_glyph (स्थिर CmapSubtableLongGroup &group,
					 hb_codepoint_t u HB_UNUSED)
  { वापस group.glyphID; }
};

प्रकार क्रमागत
{
  GLYPH_VARIANT_NOT_FOUND = 0,
  GLYPH_VARIANT_FOUND = 1,
  GLYPH_VARIANT_USE_DEFAULT = 2
} glyph_variant_t;

काष्ठा Unicode_मूल्य_Range
{
  पूर्णांक cmp (स्थिर hb_codepoint_t &codepoint) स्थिर
  {
    अगर (codepoint < start_Unicode_मूल्य) वापस -1;
    अगर (codepoint > start_Unicode_मूल्य + additional_गिनती) वापस +1;
    वापस 0;
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह));
  }

  HBUINT24	start_Unicode_मूल्य;	/* First value in this range. */
  HBUINT8	additional_गिनती;	/* Number of additional values in this
					 * range. */
  खुला:
  DEFINE_आकार_STATIC (4);
};

काष्ठा DefaultUVS : SortedArray32Of<Unicode_मूल्य_Range>
{
  व्योम collect_unicodes (hb_set_t *out) स्थिर
  {
    अचिन्हित पूर्णांक गिनती = len;
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
    {
      hb_codepoint_t first = arrayZ[i].start_Unicode_मूल्य;
      hb_codepoint_t last = hb_min ((hb_codepoint_t) (first + arrayZ[i].additional_गिनती),
				    (hb_codepoint_t) HB_UNICODE_MAX);
      out->add_range (first, last);
    }
  }

  DefaultUVS* copy (hb_serialize_context_t *c,
		    स्थिर hb_set_t *unicodes) स्थिर
  {
    स्वतः *out = c->start_embed<DefaultUVS> ();
    स्वतः snap = c->snapshot ();

    HBUINT32 len;
    len = 0;
    अगर (unlikely (!c->copy<HBUINT32> (len))) वापस nullptr;
    अचिन्हित init_len = c->लंबाई ();

    अगर (यह->len > unicodes->get_population () * hb_bit_storage ((अचिन्हित) यह->len))
    {
      hb_codepoint_t start = HB_SET_मूल्य_INVALID;
      hb_codepoint_t end = HB_SET_मूल्य_INVALID;

      क्रम (स्वतः u : *unicodes)
      {
        अगर (!as_array ().द्वा_खोज (u))
	  जारी;
	अगर (start == HB_SET_मूल्य_INVALID)
	{
	  start = u;
	  end = start - 1;
	}
	अगर (end + 1 != u || end - start == 255)
        {
	  Unicode_मूल्य_Range rec;
	  rec.start_Unicode_मूल्य = start;
	  rec.additional_गिनती = end - start;
	  c->copy<Unicode_मूल्य_Range> (rec);
	  start = u;
	}
	end = u;
      }
      अगर (start != HB_SET_मूल्य_INVALID)
      {
	Unicode_मूल्य_Range rec;
	rec.start_Unicode_मूल्य = start;
	rec.additional_गिनती = end - start;
	c->copy<Unicode_मूल्य_Range> (rec);
      }

    }
    अन्यथा
    {
      hb_codepoint_t lastCode = HB_SET_मूल्य_INVALID;
      पूर्णांक गिनती = -1;

      क्रम (स्थिर Unicode_मूल्य_Range& _ : *यह)
      {
	hb_codepoint_t curEntry = (hb_codepoint_t) (_.start_Unicode_मूल्य - 1);
	hb_codepoint_t end = curEntry + _.additional_गिनती + 2;

	क्रम (; unicodes->next (&curEntry) && curEntry < end;)
	{
	  गिनती += 1;
	  अगर (lastCode == HB_SET_मूल्य_INVALID)
	    lastCode = curEntry;
	  अन्यथा अगर (lastCode + गिनती != curEntry || गिनती > 255)
	  {
	    Unicode_मूल्य_Range rec;
	    rec.start_Unicode_मूल्य = lastCode;
	    rec.additional_गिनती = गिनती - 1;
	    c->copy<Unicode_मूल्य_Range> (rec);

	    lastCode = curEntry;
	    गिनती = 0;
	  }
	}
      }

      अगर (lastCode != HB_MAP_मूल्य_INVALID)
      {
	Unicode_मूल्य_Range rec;
	rec.start_Unicode_मूल्य = lastCode;
	rec.additional_गिनती = गिनती;
	c->copy<Unicode_मूल्य_Range> (rec);
      }
    }

    अगर (c->लंबाई () - init_len == 0)
    {
      c->revert (snap);
      वापस nullptr;
    }
    अन्यथा
    {
      अगर (unlikely (!c->check_assign (out->len,
                                      (c->लंबाई () - init_len) / Unicode_मूल्य_Range::static_आकार,
                                      HB_SERIALIZE_ERROR_INT_OVERFLOW))) वापस nullptr;
      वापस out;
    }
  }

  खुला:
  DEFINE_आकार_ARRAY (4, *यह);
};

ढाँचा <typename Types>
काष्ठा UVSMapping
{
  using GlyphID = typename Types::HBGlyphID;

  पूर्णांक cmp (स्थिर hb_codepoint_t &codepoint) स्थिर
  { वापस unicode_मूल्य.cmp (codepoint); }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह));
  }

  HBUINT24	unicode_मूल्य;	/* Base Unicode value of the UVS */
  GlyphID
		glyphID;	/* Glyph ID of the UVS. */
  खुला:
  DEFINE_आकार_STATIC (3 + GlyphID::static_आकार);
};

ढाँचा <typename Types>
काष्ठा NonDefaultUVS : SortedArray32Of<UVSMapping<Types>>
{
  using Mapping = UVSMapping<Types>;

  व्योम collect_unicodes (hb_set_t *out) स्थिर
  {
    क्रम (स्थिर स्वतः& a : यह->as_array ())
      out->add (a.unicode_मूल्य);
  }

  व्योम collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping /* OUT */) स्थिर
  {
    क्रम (स्थिर स्वतः& a : यह->as_array ())
    {
      hb_codepoint_t unicode = a.unicode_मूल्य;
      hb_codepoint_t glyphid = a.glyphID;
      unicodes->add (unicode);
      mapping->set (unicode, glyphid);
    }
  }

  व्योम closure_glyphs (स्थिर hb_set_t      *unicodes,
		       hb_set_t            *glyphset) स्थिर
  {
    + यह->as_array ()
    | hb_filter (unicodes, &Mapping::unicode_मूल्य)
    | hb_map (&Mapping::glyphID)
    | hb_sink (glyphset)
    ;
  }

  NonDefaultUVS* copy (hb_serialize_context_t *c,
		       स्थिर hb_set_t *unicodes,
		       स्थिर hb_set_t *glyphs_requested,
		       स्थिर hb_map_t *glyph_map) स्थिर
  {
    स्वतः *out = c->start_embed<NonDefaultUVS> ();
    स्वतः it =
    + यह->as_array ()
    | hb_filter ([&] (स्थिर Mapping& _)
		 {
		   वापस unicodes->has (_.unicode_मूल्य) || glyphs_requested->has (_.glyphID);
		 })
    ;

    अगर (!it) वापस nullptr;

    HBUINT32 len;
    len = it.len ();
    अगर (unlikely (!c->copy<HBUINT32> (len))) वापस nullptr;

    क्रम (स्थिर Mapping& _ : it)
    {
      Mapping mapping;
      mapping.unicode_मूल्य = _.unicode_मूल्य;
      mapping.glyphID = glyph_map->लो (_.glyphID);
      c->copy<Mapping> (mapping);
    }

    वापस out;
  }

  खुला:
  DEFINE_आकार_ARRAY (4, *यह);
};

ढाँचा <typename Types>
काष्ठा VariationSelectorRecord
{
  glyph_variant_t get_glyph (hb_codepoint_t codepoint,
			     hb_codepoint_t *glyph,
			     स्थिर व्योम *base) स्थिर
  {
    अगर ((base+defaultUVS).bfind (codepoint))
      वापस GLYPH_VARIANT_USE_DEFAULT;
    स्थिर UVSMapping<Types> &nonDefault = (base+nonDefaultUVS).द्वा_खोज (codepoint);
    अगर (nonDefault.glyphID)
    {
      *glyph = nonDefault.glyphID;
       वापस GLYPH_VARIANT_FOUND;
    }
    वापस GLYPH_VARIANT_NOT_FOUND;
  }

  VariationSelectorRecord(स्थिर VariationSelectorRecord& other)
  {
    *यह = other;
  }

  व्योम चालक= (स्थिर VariationSelectorRecord& other)
  {
    varSelector = other.varSelector;
    HBUINT32 offset = other.defaultUVS;
    defaultUVS = offset;
    offset = other.nonDefaultUVS;
    nonDefaultUVS = offset;
  }

  व्योम collect_unicodes (hb_set_t *out, स्थिर व्योम *base) स्थिर
  {
    (base+defaultUVS).collect_unicodes (out);
    (base+nonDefaultUVS).collect_unicodes (out);
  }

  व्योम collect_mapping (स्थिर व्योम *base,
			hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping /* OUT */) स्थिर
  {
    (base+defaultUVS).collect_unicodes (unicodes);
    (base+nonDefaultUVS).collect_mapping (unicodes, mapping);
  }

  पूर्णांक cmp (स्थिर hb_codepoint_t &variation_selector) स्थिर
  { वापस varSelector.cmp (variation_selector); }

  bool sanitize (hb_sanitize_context_t *c, स्थिर व्योम *base) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) &&
		  defaultUVS.sanitize (c, base) &&
		  nonDefaultUVS.sanitize (c, base));
  }

  hb_pair_t<अचिन्हित, अचिन्हित>
  copy (hb_serialize_context_t *c,
	स्थिर hb_set_t *unicodes,
	स्थिर hb_set_t *glyphs_requested,
	स्थिर hb_map_t *glyph_map,
	स्थिर व्योम *base) स्थिर
  {
    स्वतः snap = c->snapshot ();
    स्वतः *out = c->embed<VariationSelectorRecord> (*यह);
    अगर (unlikely (!out)) वापस hb_pair (0, 0);

    out->defaultUVS = 0;
    out->nonDefaultUVS = 0;

    अचिन्हित non_default_uvs_objidx = 0;
    अगर (nonDefaultUVS != 0)
    {
      c->push ();
      अगर (c->copy (base+nonDefaultUVS, unicodes, glyphs_requested, glyph_map))
	non_default_uvs_objidx = c->निकालो_pack ();
      अन्यथा c->निकालो_discard ();
    }

    अचिन्हित default_uvs_objidx = 0;
    अगर (defaultUVS != 0)
    {
      c->push ();
      अगर (c->copy (base+defaultUVS, unicodes))
	default_uvs_objidx = c->निकालो_pack ();
      अन्यथा c->निकालो_discard ();
    }


    अगर (!default_uvs_objidx && !non_default_uvs_objidx)
      c->revert (snap);

    वापस hb_pair (default_uvs_objidx, non_default_uvs_objidx);
  }

  HBUINT24	varSelector;	/* Variation selector. */
  Offset32To<DefaultUVS>
		defaultUVS;	/* Offset to Default UVS Table.  May be 0. */
  Offset32To<NonDefaultUVS<Types>>
		nonDefaultUVS;	/* Offset to Non-Default UVS Table.  May be 0. */
  खुला:
  DEFINE_आकार_STATIC (11);
};

ढाँचा <typename Types>
काष्ठा CmapSubtableFormat14_15
{
  अटल constexpr अचिन्हित format_मूल्य = Types::आकार == 2 ? 14 : 15;
  using Record = VariationSelectorRecord<Types>;
  using NonDefault = NonDefaultUVS<Types>;

  glyph_variant_t get_glyph_variant (hb_codepoint_t codepoint,
				     hb_codepoint_t variation_selector,
				     hb_codepoint_t *glyph) स्थिर
  { वापस record.द्वा_खोज (variation_selector).get_glyph (codepoint, glyph, यह); }

  व्योम collect_variation_selectors (hb_set_t *out) स्थिर
  {
    क्रम (स्थिर स्वतः& a : record.as_array ())
      out->add (a.varSelector);
  }
  व्योम collect_variation_unicodes (hb_codepoint_t variation_selector,
				   hb_set_t *out) स्थिर
  { record.द्वा_खोज (variation_selector).collect_unicodes (out, यह); }

  व्योम serialize (hb_serialize_context_t *c,
		  स्थिर hb_set_t *unicodes,
		  स्थिर hb_set_t *glyphs_requested,
		  स्थिर hb_map_t *glyph_map,
		  स्थिर व्योम *base)
  {
    स्वतः snap = c->snapshot ();
    अचिन्हित table_initpos = c->लंबाई ();
    स्थिर अक्षर* init_पुच्छ = c->पुच्छ;

    अगर (unlikely (!c->extend_min (यह))) वापस;
    यह->format = format_मूल्य;

    स्वतः src_tbl = reinterpret_cast<स्थिर CmapSubtableFormat14_15*> (base);

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
    क्रम (पूर्णांक i = src_tbl->record.len - 1; i >= 0; i--)
    {
      अगर (!unicodes->has(src_tbl->record[i].varSelector))
        जारी;

      hb_pair_t<अचिन्हित, अचिन्हित> परिणाम = src_tbl->record[i].copy (c, unicodes, glyphs_requested, glyph_map, base);
      अगर (परिणाम.first || परिणाम.second)
	obj_indices.push (परिणाम);
    }

    अगर (c->लंबाई () - table_initpos == CmapSubtableFormat14_15::min_आकार)
    {
      c->revert (snap);
      वापस;
    }

    अगर (unlikely (!c->check_success (!obj_indices.in_error ())))
      वापस;

    पूर्णांक पुच्छ_len = init_पुच्छ - c->पुच्छ;
    c->check_assign (यह->लंबाई, c->लंबाई () - table_initpos + पुच्छ_len,
                     HB_SERIALIZE_ERROR_INT_OVERFLOW);
    c->check_assign (यह->record.len,
		     (c->लंबाई () - table_initpos - CmapSubtableFormat14_15::min_आकार) /
		     Record::static_आकार,
                     HB_SERIALIZE_ERROR_INT_OVERFLOW);

    /* Correct the incorrect write order by reversing the order of the variation
       records array. */
    _reverse_variation_records ();

    /* Now that records are in the right order, we can set up the offsets. */
    _add_links_to_variation_records (c, obj_indices);
  }

  व्योम _reverse_variation_records ()
  {
    record.as_array ().reverse ();
  }

  व्योम _add_links_to_variation_records (hb_serialize_context_t *c,
					स्थिर hb_vector_t<hb_pair_t<अचिन्हित, अचिन्हित>>& obj_indices)
  {
    क्रम (अचिन्हित i = 0; i < obj_indices.लंबाई; i++)
    {
      /*
       * Since the record array has been reversed (see comments in copy())
       * but obj_indices has not been, the indices at obj_indices[i]
       * are for the variation record at record[j].
       */
      पूर्णांक j = obj_indices.लंबाई - 1 - i;
      c->add_link (record[j].defaultUVS, obj_indices[i].first);
      c->add_link (record[j].nonDefaultUVS, obj_indices[i].second);
    }
  }

  व्योम closure_glyphs (स्थिर hb_set_t      *unicodes,
		       hb_set_t            *glyphset) स्थिर
  {
    + hb_iter (record)
    | hb_filter (hb_bool, &Record::nonDefaultUVS)
    | hb_filter (unicodes, &Record::varSelector)
    | hb_map (&Record::nonDefaultUVS)
    | hb_map (hb_add (यह))
    | hb_apply ([=] (स्थिर NonDefault& _) { _.closure_glyphs (unicodes, glyphset); })
    ;
  }

  व्योम collect_unicodes (hb_set_t *out) स्थिर
  {
    क्रम (स्थिर Record& _ : record)
      _.collect_unicodes (out, यह);
  }

  व्योम collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping /* OUT */) स्थिर
  {
    क्रम (स्थिर Record& _ : record)
      _.collect_mapping (यह, unicodes, mapping);
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) &&
		  record.sanitize (c, यह));
  }

  रक्षित:
  HBUINT16	format;		/* Format number is set to 14 or 15. */
  HBUINT32	लंबाई;		/* Byte length of this subtable. */
  SortedArray32Of<Record>
		record;		/* Variation selector records; sorted
				 * in increasing order of `varSelector'. */
  खुला:
  DEFINE_आकार_ARRAY (10, record);
};

काष्ठा CmapSubtable
{
  /* Note: We intentionally do NOT implement subtable formats 2 and 8. */

  bool is_variation_selector () स्थिर
  {
    चयन (u.format.v) {
    हाल 14:
#अगर_अघोषित HB_NO_BEYOND_64K
    हाल 15:
#पूर्ण_अगर
      वापस true;
    शेष:
      वापस false;
    }
  }

  glyph_variant_t get_glyph_variant (hb_codepoint_t codepoint,
				     hb_codepoint_t variation_selector,
				     hb_codepoint_t *glyph) स्थिर
  {
    चयन (u.format.v) {
    हाल 14: hb_barrier (); वापस u.format14.get_glyph_variant (codepoint, variation_selector, glyph);
#अगर_अघोषित HB_NO_BEYOND_64K
    हाल 15: hb_barrier (); वापस u.format15.get_glyph_variant (codepoint, variation_selector, glyph);
#पूर्ण_अगर
    शेष: वापस GLYPH_VARIANT_NOT_FOUND;
    }
  }

  व्योम collect_variation_selectors (hb_set_t *out) स्थिर
  {
    चयन (u.format.v) {
    हाल 14: hb_barrier (); u.format14.collect_variation_selectors (out); वापस;
#अगर_अघोषित HB_NO_BEYOND_64K
    हाल 15: hb_barrier (); u.format15.collect_variation_selectors (out); वापस;
#पूर्ण_अगर
    शेष: वापस;
    }
  }

  व्योम collect_variation_unicodes (hb_codepoint_t variation_selector,
				    hb_set_t *out) स्थिर
  {
    चयन (u.format.v) {
    हाल 14: hb_barrier (); u.format14.collect_variation_unicodes (variation_selector, out); वापस;
#अगर_अघोषित HB_NO_BEYOND_64K
    हाल 15: hb_barrier (); u.format15.collect_variation_unicodes (variation_selector, out); वापस;
#पूर्ण_अगर
    शेष: वापस;
    }
  }

  व्योम closure_glyphs (स्थिर hb_set_t *unicodes, hb_set_t *glyphset) स्थिर
  {
    चयन (u.format.v) {
    हाल 14: hb_barrier (); u.format14.closure_glyphs (unicodes, glyphset); वापस;
#अगर_अघोषित HB_NO_BEYOND_64K
    हाल 15: hb_barrier (); u.format15.closure_glyphs (unicodes, glyphset); वापस;
#पूर्ण_अगर
    शेष: वापस;
    }
  }

  bool get_glyph (hb_codepoint_t codepoint,
		  hb_codepoint_t *glyph,
		  अचिन्हित पूर्णांक subtable_तथ्य_आकार = 0) स्थिर
  {
    चयन (u.format.v) {
    हाल  0: hb_barrier (); वापस u.format0 .get_glyph (codepoint, glyph);
    हाल  4: hb_barrier (); वापस subtable_तथ्य_आकार
				      ? u.format4.get_glyph (codepoint, glyph, subtable_तथ्य_आकार)
				      : false;
    हाल  6: hb_barrier (); वापस u.format6 .get_glyph (codepoint, glyph);
    हाल 10: hb_barrier (); वापस u.format10.get_glyph (codepoint, glyph);
    हाल 12: hb_barrier (); वापस u.format12.get_glyph (codepoint, glyph);
    हाल 13: hb_barrier (); वापस u.format13.get_glyph (codepoint, glyph);
    हाल 14:
    शेष: वापस false;
    }
  }
  व्योम collect_unicodes (hb_set_t *out,
			 अचिन्हित पूर्णांक अंक_glyphs = अच_पूर्णांक_उच्च,
			 अचिन्हित पूर्णांक subtable_तथ्य_आकार = 0) स्थिर
  {
    चयन (u.format.v) {
    हाल  0: hb_barrier (); u.format0 .collect_unicodes (out); वापस;
    हाल  4: hb_barrier (); अगर (subtable_तथ्य_आकार)
			      u.format4.collect_unicodes (out, subtable_तथ्य_आकार);
			    अन्यथा
			      u.format4.collect_unicodes (out);
			    वापस;
    हाल  6: hb_barrier (); u.format6 .collect_unicodes (out); वापस;
    हाल 10: hb_barrier (); u.format10.collect_unicodes (out); वापस;
    हाल 12: hb_barrier (); u.format12.collect_unicodes (out, अंक_glyphs); वापस;
    हाल 13: hb_barrier (); u.format13.collect_unicodes (out, अंक_glyphs); वापस;
    हाल 14:
    शेष: वापस;
    }
  }

  व्योम collect_mapping (hb_set_t *unicodes, /* OUT */
			hb_map_t *mapping, /* OUT */
			अचिन्हित अंक_glyphs = अच_पूर्णांक_उच्च,
			अचिन्हित पूर्णांक subtable_तथ्य_आकार = 0) स्थिर
  {
    चयन (u.format.v) {
    हाल  0: hb_barrier (); u.format0 .collect_mapping (unicodes, mapping); वापस;
    हाल  4: hb_barrier (); अगर (subtable_तथ्य_आकार)
			      u.format4.collect_mapping (unicodes, mapping, subtable_तथ्य_आकार);
			    अन्यथा
			      u.format4.collect_mapping (unicodes, mapping);
			    वापस;
    हाल  6: hb_barrier (); u.format6 .collect_mapping (unicodes, mapping); वापस;
    हाल 10: hb_barrier (); u.format10.collect_mapping (unicodes, mapping); वापस;
    हाल 12: hb_barrier (); u.format12.collect_mapping (unicodes, mapping, अंक_glyphs); वापस;
    हाल 13: hb_barrier (); u.format13.collect_mapping (unicodes, mapping, अंक_glyphs); वापस;
    हाल 14:
    शेष: वापस;
    }
  }

  अचिन्हित get_language () स्थिर
  {
    चयन (u.format.v) {
    हाल  0: hb_barrier (); वापस u.format0 .get_language ();
    हाल  4: hb_barrier (); वापस u.format4 .get_language ();
    हाल  6: hb_barrier (); वापस u.format6 .get_language ();
    हाल 10: hb_barrier (); वापस u.format10.get_language ();
    हाल 12: hb_barrier (); वापस u.format12.get_language ();
    हाल 13: hb_barrier (); वापस u.format13.get_language ();
    हाल 14:
    शेष: वापस 0;
    }
  }

  ढाँचा<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  व्योम serialize (hb_serialize_context_t *c,
		  Iterator it,
		  अचिन्हित format,
		  स्थिर hb_subset_plan_t *plan,
		  स्थिर व्योम *base)
  {
    चयन (format) {
    हाल  4: hb_barrier (); वापस u.format4.serialize (c, it);
    हाल 12: hb_barrier (); वापस u.format12.serialize (c, it);
    हाल 14: hb_barrier (); वापस u.format14.serialize (c, &plan->unicodes, &plan->glyphs_requested, plan->glyph_map, base);
#अगर_अघोषित HB_NO_BEYOND_64K
    हाल 15: hb_barrier (); वापस u.format15.serialize (c, &plan->unicodes, &plan->glyphs_requested, plan->glyph_map, base);
#पूर्ण_अगर
    शेष: वापस;
    }
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    अगर (!u.format.v.sanitize (c)) return_trace (false);
    hb_barrier ();
    चयन (u.format.v) {
    हाल  0: hb_barrier (); return_trace (u.format0 .sanitize (c));
    हाल  4: hb_barrier (); return_trace (u.format4 .sanitize (c));
    हाल  6: hb_barrier (); return_trace (u.format6 .sanitize (c));
    हाल 10: hb_barrier (); return_trace (u.format10.sanitize (c));
    हाल 12: hb_barrier (); return_trace (u.format12.sanitize (c));
    हाल 13: hb_barrier (); return_trace (u.format13.sanitize (c));
    हाल 14: hb_barrier (); return_trace (u.format14.sanitize (c));
#अगर_अघोषित HB_NO_BEYOND_64K
    हाल 15: hb_barrier (); return_trace (u.format15.sanitize (c));
#पूर्ण_अगर
    शेष:return_trace (true);
    }
  }

  खुला:
  जोड़ {
  काष्ठा { HBUINT16 v; }	format;		/* Format identifier */
  CmapSubtableFormat0	format0;
  CmapSubtableFormat4	format4;
  CmapSubtableFormat6	format6;
  CmapSubtableFormat10	format10;
  CmapSubtableFormat12	format12;
  CmapSubtableFormat13	format13;
  CmapSubtableFormat14_15<Layout::SmallTypes>
			format14;
#अगर_अघोषित HB_NO_BEYOND_64K
  CmapSubtableFormat14_15<Layout::MediumTypes>
			format15;
#पूर्ण_अगर
  } u;
  खुला:
  DEFINE_आकार_UNION (2, format.v);
};


काष्ठा EncodingRecord
{
  पूर्णांक cmp (स्थिर EncodingRecord &other) स्थिर
  {
    पूर्णांक ret;
    ret = platformID.cmp (other.platformID);
    अगर (ret) वापस ret;
    अगर (other.encodingID != 0xFFFF)
    {
      ret = encodingID.cmp (other.encodingID);
      अगर (ret) वापस ret;
    }
    वापस 0;
  }

  bool sanitize (hb_sanitize_context_t *c, स्थिर व्योम *base) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) &&
		  subtable.sanitize (c, base));
  }

  ढाँचा<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  EncodingRecord* copy (hb_serialize_context_t *c,
			Iterator it,
			अचिन्हित format,
			स्थिर व्योम *base,
			स्थिर hb_subset_plan_t *plan,
			/* INOUT */ अचिन्हित *objidx) स्थिर
  {
    TRACE_SERIALIZE (यह);
    स्वतः snap = c->snapshot ();
    स्वतः *out = c->embed (यह);
    अगर (unlikely (!out)) return_trace (nullptr);
    out->subtable = 0;

    अगर (*objidx == 0)
    {
      CmapSubtable *cmapsubtable = c->push<CmapSubtable> ();
      अचिन्हित origin_लंबाई = c->लंबाई ();
      cmapsubtable->serialize (c, it, format, plan, &(base+subtable));
      अगर (c->लंबाई () > origin_लंबाई && !c->in_error()) *objidx = c->निकालो_pack ();
      अन्यथा c->निकालो_discard ();
    }

    अगर (*objidx == 0)
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
  खुला:
  DEFINE_आकार_STATIC (8);
};

काष्ठा cmap;

काष्ठा SubtableUnicodesCache {

 निजी:
  hb_blob_ptr_t<cmap> base_blob;
  स्थिर अक्षर* base;
  अचिन्हित पूर्णांक table_लंबाई;
  hb_hashmap_t<अचिन्हित, hb::unique_ptr<hb_set_t>> cached_unicodes;

 खुला:

  अटल SubtableUnicodesCache* create (hb_blob_ptr_t<cmap> source_table)
  {
    SubtableUnicodesCache* cache =
        (SubtableUnicodesCache*) hb_malloc (माप(SubtableUnicodesCache));

    अगर (unlikely (!cache))
      वापस nullptr;

    नया (cache) SubtableUnicodesCache (source_table);
    वापस cache;
  }

  अटल व्योम destroy (व्योम* value) {
    अगर (!value) वापस;

    SubtableUnicodesCache* cache = (SubtableUnicodesCache*) value;
    cache->~SubtableUnicodesCache ();
    hb_free (cache);
  }

  SubtableUnicodesCache(स्थिर व्योम* cmap_base,
			अचिन्हित पूर्णांक table_लंबाई_2 = 0)
      : base_blob(),
        base ((स्थिर अक्षर*) cmap_base),
        table_लंबाई (table_लंबाई_2),
        cached_unicodes ()
  {}

  SubtableUnicodesCache(hb_blob_ptr_t<cmap> base_blob_)
      : base_blob(base_blob_),
        base ((स्थिर अक्षर *) base_blob.लो()),
        table_लंबाई (base_blob.get_लंबाई ()),
        cached_unicodes ()
  {}

  ~SubtableUnicodesCache()
  {
    base_blob.destroy ();
  }

  bool same_base(स्थिर व्योम* other) स्थिर
  {
    वापस other == (स्थिर व्योम*) base;
  }

  स्थिर hb_set_t* set_for (स्थिर EncodingRecord* record,
                           SubtableUnicodesCache& mutable_cache) स्थिर
  {
    अगर (cached_unicodes.has ((अचिन्हित) ((स्थिर अक्षर *) record - base)))
      वापस cached_unicodes.लो ((अचिन्हित) ((स्थिर अक्षर *) record - base));

    वापस mutable_cache.set_for (record);
  }

  स्थिर hb_set_t* set_for (स्थिर EncodingRecord* record)
  {
    अगर (!cached_unicodes.has ((अचिन्हित) ((स्थिर अक्षर *) record - base)))
    {
      hb_set_t *s = hb_set_create ();
      अगर (unlikely (s->in_error ()))
	वापस hb_set_get_empty ();

      अचिन्हित पूर्णांक subtable_तथ्य_आकार = record->subtable < table_लंबाई
					? table_लंबाई - (अचिन्हित पूर्णांक) record->subtable
					: 0;
      (base+record->subtable).collect_unicodes (s, अच_पूर्णांक_उच्च, subtable_तथ्य_आकार);

      अगर (unlikely (!cached_unicodes.set ((अचिन्हित) ((स्थिर अक्षर *) record - base), hb::unique_ptr<hb_set_t> {s})))
        वापस hb_set_get_empty ();

      वापस s;
    }
    वापस cached_unicodes.लो ((अचिन्हित) ((स्थिर अक्षर *) record - base));
  }

};

अटल अंतरभूत uint16_t
hb_प्रतीक_pua_map (अचिन्हित codepoint)
{
  अगर (codepoint <= 0x00FFu)
  {
    /* For symbol-encoded OpenType fonts, we duplicate the
     * U+F000..F0FF range at U+0000..U+00FF.  That's what
     * Windows seems to do, and that's hinted about at:
     * https://docs.microsoft.com/en-us/typography/opentype/spec/recom
     * under "Non-Standard (Symbol) Fonts". */
    वापस 0xF000u + codepoint;
  }
  वापस 0;
}

काष्ठा cmap
{
  अटल constexpr hb_tag_t tableTag = HB_OT_TAG_cmap;


  अटल SubtableUnicodesCache* create_filled_cache(hb_blob_ptr_t<cmap> source_table) {
    स्थिर cmap* cmap = source_table.लो();
    स्वतः it =
    + hb_iter (cmap->encodingRecord)
    | hb_filter ([&](स्थिर EncodingRecord& _) {
      वापस cmap::filter_encoding_records_for_subset (cmap, _);
    })
    ;

    SubtableUnicodesCache* cache = SubtableUnicodesCache::create(source_table);

    अगर (unlikely (!cache))
      वापस nullptr;

    क्रम (स्थिर EncodingRecord& _ : it)
      cache->set_for(&_); // populate the cache for this encoding record.

    वापस cache;
  }

  ढाँचा<typename Iterator, typename EncodingRecIter,
	   hb_requires (hb_is_iterator (EncodingRecIter))>
  bool serialize (hb_serialize_context_t *c,
		  Iterator it,
		  EncodingRecIter encodingrec_iter,
		  स्थिर व्योम *base,
		  hb_subset_plan_t *plan,
	                  bool drop_format_4 = false,
			  अचिन्हित पूर्णांक source_table_लंबाई = 0)
  {
    अगर (unlikely (!c->extend_min ((*यह))))  वापस false;
    यह->version = 0;

    अचिन्हित format4objidx = 0, format12objidx = 0, format14objidx = 0;
#अगर_अघोषित HB_NO_BEYOND_64K
    अचिन्हित format15objidx = 0;
#पूर्ण_अगर
    स्वतः snap = c->snapshot ();

    SubtableUnicodesCache local_unicodes_cache (base, source_table_लंबाई);
    स्थिर SubtableUnicodesCache* unicodes_cache = &local_unicodes_cache;

    अगर (plan->accelerator &&
        plan->accelerator->cmap_cache &&
        plan->accelerator->cmap_cache->same_base (base))
      unicodes_cache = plan->accelerator->cmap_cache;

    क्रम (स्थिर EncodingRecord& _ : encodingrec_iter)
    {
      अगर (c->in_error ())
        वापस false;

      अचिन्हित format = (base+_.subtable).u.format.v;
      अगर (format != 4 && format != 12 && format != 14
#अगर_अघोषित HB_NO_BEYOND_64K
	  && format != 15
#पूर्ण_अगर
	 ) जारी;

      स्थिर hb_set_t* unicodes_set = unicodes_cache->set_for (&_, local_unicodes_cache);

      अगर (!drop_format_4 && format == 4)
      {
        c->copy (_, + it | hb_filter (*unicodes_set, hb_first), 4u, base, plan, &format4objidx);
        अगर (c->in_error () && c->only_overflow ())
        {
          // cmap4 overflowed, reset and retry serialization without format 4 subtables.
          c->revert (snap);
          वापस serialize (c, it,
                            encodingrec_iter,
                            base,
                            plan,
                            true,
			    source_table_लंबाई);
        }
      }

      अन्यथा अगर (format == 12)
      {
        अगर (_can_drop (_,
                       *unicodes_set,
                       base,
                       *unicodes_cache,
                       local_unicodes_cache,
                       + it | hb_map (hb_first), encodingrec_iter))
          जारी;
        c->copy (_, + it | hb_filter (*unicodes_set, hb_first), 12u, base, plan, &format12objidx);
      }
      अन्यथा अगर (format == 14) c->copy (_, it, 14u, base, plan, &format14objidx);
#अगर_अघोषित HB_NO_BEYOND_64K
      अन्यथा अगर (format == 15) c->copy (_, it, 15u, base, plan, &format15objidx);
#पूर्ण_अगर
    }
        अचिन्हित लंबाई = c->लंबाई ();
        अचिन्हित available = लंबाई > cmap::min_आकार ? लंबाई - cmap::min_आकार : 0;
        c->check_assign(यह->encodingRecord.len,
                        available / EncodingRecord::static_आकार,
                        HB_SERIALIZE_ERROR_INT_OVERFLOW);

    // Fail if format 4 was dropped and there is no cmap12.
    वापस !drop_format_4 || format12objidx;
  }

  ढाँचा<typename Iterator, typename EncodingRecordIterator,
      hb_requires (hb_is_iterator (Iterator)),
      hb_requires (hb_is_iterator (EncodingRecordIterator))>
  bool _can_drop (स्थिर EncodingRecord& cmap12,
                  स्थिर hb_set_t& cmap12_unicodes,
                  स्थिर व्योम* base,
                  स्थिर SubtableUnicodesCache& unicodes_cache,
                  SubtableUnicodesCache& local_unicodes_cache,
                  Iterator subset_unicodes,
                  EncodingRecordIterator encoding_records)
  {
    क्रम (स्वतः cp : + subset_unicodes | hb_filter (cmap12_unicodes))
    {
      अगर (cp >= 0x10000) वापस false;
    }

    अचिन्हित target_platform;
    अचिन्हित target_encoding;
    अचिन्हित target_language = (base+cmap12.subtable).get_language ();

    अगर (cmap12.platformID == 0 && cmap12.encodingID == 4)
    {
      target_platform = 0;
      target_encoding = 3;
    } अन्यथा अगर (cmap12.platformID == 3 && cmap12.encodingID == 10) {
      target_platform = 3;
      target_encoding = 1;
    } अन्यथा {
      वापस false;
    }

    क्रम (स्थिर स्वतः& _ : encoding_records)
    {
      अगर (_.platformID != target_platform
          || _.encodingID != target_encoding
          || (base+_.subtable).get_language() != target_language)
        जारी;

      स्थिर hb_set_t* sibling_unicodes = unicodes_cache.set_for (&_, local_unicodes_cache);

      स्वतः cmap12 = + subset_unicodes | hb_filter (cmap12_unicodes);
      स्वतः sibling = + subset_unicodes | hb_filter (*sibling_unicodes);
      क्रम (; cmap12 && sibling; cmap12++, sibling++)
      {
        अचिन्हित a = *cmap12;
        अचिन्हित b = *sibling;
        अगर (a != b) वापस false;
      }

      वापस !cmap12 && !sibling;
    }

    वापस false;
  }

  व्योम closure_glyphs (स्थिर hb_set_t      *unicodes,
		       hb_set_t            *glyphset) स्थिर
  {
    + hb_iter (encodingRecord)
    | hb_map (&EncodingRecord::subtable)
    | hb_map (hb_add (यह))
    | hb_filter (&CmapSubtable::is_variation_selector)
    | hb_apply ([=] (स्थिर CmapSubtable& _) { _.closure_glyphs (unicodes, glyphset); })
    ;
  }

  bool subset (hb_subset_context_t *c) स्थिर
  { वापस subset (c, false); }

  bool subset (hb_subset_context_t *c, bool is_dmap) स्थिर
  {
    TRACE_SUBSET (यह);

    cmap *cmap_अभाज्य = c->serializer->start_embed<cmap> ();

    स्वतः encodingrec_iter =
    + hb_iter (encodingRecord)
    | hb_filter ([&](स्थिर EncodingRecord& _) {
      वापस cmap::filter_encoding_records_for_subset (यह, _);
    })
    ;

    अगर (unlikely (!encodingrec_iter.len ())) return_trace (false);

    स्थिर EncodingRecord *unicode_bmp= nullptr, *unicode_ucs4 = nullptr, *ms_bmp = nullptr, *ms_ucs4 = nullptr;
    bool has_format12 = false;

    क्रम (स्थिर EncodingRecord& _ : encodingrec_iter)
    {
      अचिन्हित format = (यह + _.subtable).u.format.v;
      अगर (format == 12) has_format12 = true;

      स्थिर EncodingRecord *table = std::addressof (_);
      अगर      (_.platformID == 0 && _.encodingID ==  3) unicode_bmp = table;
      अन्यथा अगर (_.platformID == 0 && _.encodingID ==  4) unicode_ucs4 = table;
      अन्यथा अगर (_.platformID == 3 && _.encodingID ==  1) ms_bmp = table;
      अन्यथा अगर (_.platformID == 3 && _.encodingID == 10) ms_ucs4 = table;
    }

    अगर (unlikely (!is_dmap && !has_format12 && !unicode_bmp && !ms_bmp)) return_trace (false);
    अगर (unlikely (!is_dmap && has_format12 && (!unicode_ucs4 && !ms_ucs4))) return_trace (false);

    स्वतः it =
    + c->plan->unicode_to_new_gid_list.iter ()
    | hb_filter ([&] (स्थिर hb_codepoint_pair_t _)
		 { वापस (_.second != HB_MAP_मूल्य_INVALID); })
    ;

    return_trace (cmap_अभाज्य->serialize (c->serializer,
                                         it,
                                         encodingrec_iter,
                                         यह,
                                         c->plan,
					 false,
					 c->source_blob->लंबाई));
  }

  स्थिर CmapSubtable *find_best_subtable (bool *प्रतीक = nullptr,
					  bool *mac = nullptr,
					  bool *macroman = nullptr) स्थिर
  {
    अगर (प्रतीक) *प्रतीक = false;
    अगर (mac) *mac = false;
    अगर (macroman) *macroman = false;

    स्थिर CmapSubtable *subtable;

    /* Symbol subtable.
     * Prefer symbol if available.
     * https://github.com/harfbuzz/harfbuzz/issues/1918 */
    अगर ((subtable = यह->find_subtable (3, 0)))
    {
      अगर (प्रतीक) *प्रतीक = true;
      वापस subtable;
    }

    /* 32-bit subtables. */
    अगर ((subtable = यह->find_subtable (3, 10))) वापस subtable;
    अगर ((subtable = यह->find_subtable (0, 6))) वापस subtable;
    अगर ((subtable = यह->find_subtable (0, 4))) वापस subtable;

    /* 16-bit subtables. */
    अगर ((subtable = यह->find_subtable (3, 1))) वापस subtable;
    अगर ((subtable = यह->find_subtable (0, 3))) वापस subtable;
    अगर ((subtable = यह->find_subtable (0, 2))) वापस subtable;
    अगर ((subtable = यह->find_subtable (0, 1))) वापस subtable;
    अगर ((subtable = यह->find_subtable (0, 0))) वापस subtable;

    /* MacRoman subtable. */
    अगर ((subtable = यह->find_subtable (1, 0)))
    {
      अगर (mac) *mac = true;
      अगर (macroman) *macroman = true;
      वापस subtable;
    }
    /* Any other Mac subtable; we just map ASCII for these. */
    अगर ((subtable = यह->find_subtable (1, 0xFFFF)))
    {
      अगर (mac) *mac = true;
      वापस subtable;
    }

    /* Meh. */
    वापस &Null (CmapSubtable);
  }

  काष्ठा accelerator_t
  {
    using cache_t = hb_cache_t<21, 19>;
    static_assert (माप (cache_t) == 1024, "");

    accelerator_t (hb_face_t *face, hb_tag_t table_tag = cmap::tableTag)
    {
      यह->table = hb_sanitize_context_t ().reference_table<cmap> (face, table_tag);
      अगर (!यह->table.get_लंबाई ())
	वापस;

      bool प्रतीक, mac, macroman;
      यह->subtable = table->find_best_subtable (&प्रतीक, &mac, &macroman);
      यह->subtable_uvs = &Null (CmapSubtable);
      {
	स्थिर CmapSubtable *st = table->find_variation_selector_subtable ();
	अगर (st)
	  subtable_uvs = st;
      }

#अगर_अघोषित HB_NO_OT_FONT_CMAP_CACHE
      cache = (cache_t *) hb_malloc (माप (cache_t));
      अगर (cache)
	नया (cache) cache_t ();
      अन्यथा
        वापस; // Such that get_glyph_funcZ remains null.
#पूर्ण_अगर

      यह->get_glyph_तथ्य = subtable;
#अगर_अघोषित HB_NO_CMAP_LEGACY_SUBTABLES
      bool is_format4 = subtable->u.format.v == 4;
      स्वतः set_format4_getter = [यह] (bool (*func) (स्थिर व्योम *,
						      hb_codepoint_t,
						      hb_codepoint_t *))
      {
	यह->format4_accel.init (&यह->subtable->u.format4,
				  get_subtable_तथ्य_आकार (यह->subtable));
	यह->get_glyph_तथ्य = &यह->format4_accel;
	यह->get_glyph_funcZ = func;
      };
      अगर (unlikely (प्रतीक))
      {
	चयन ((अचिन्हित) face->table.OS2->get_font_page ()) {
	हाल OS2::font_page_t::FONT_PAGE_NONE:
	  अगर (is_format4)
	    set_format4_getter (get_glyph_from_प्रतीक<CmapSubtableFormat4::accelerator_t, hb_प्रतीक_pua_map>);
	  अन्यथा
	    यह->get_glyph_funcZ = get_glyph_from_प्रतीक<CmapSubtable, hb_प्रतीक_pua_map>;
	  अवरोध;
#अगर_अघोषित HB_NO_OT_SHAPER_ARABIC_FALLBACK
	हाल OS2::font_page_t::FONT_PAGE_SIMP_ARABIC:
	  अगर (is_format4)
	    set_format4_getter (get_glyph_from_प्रतीक<CmapSubtableFormat4::accelerator_t, _hb_arabic_pua_simp_map>);
	  अन्यथा
	    यह->get_glyph_funcZ = get_glyph_from_प्रतीक<CmapSubtable, _hb_arabic_pua_simp_map>;
	  अवरोध;
	हाल OS2::font_page_t::FONT_PAGE_TRAD_ARABIC:
	  अगर (is_format4)
	    set_format4_getter (get_glyph_from_प्रतीक<CmapSubtableFormat4::accelerator_t, _hb_arabic_pua_trad_map>);
	  अन्यथा
	    यह->get_glyph_funcZ = get_glyph_from_प्रतीक<CmapSubtable, _hb_arabic_pua_trad_map>;
	  अवरोध;
#पूर्ण_अगर
	शेष:
	  अगर (is_format4)
	    set_format4_getter (get_glyph_from<CmapSubtableFormat4::accelerator_t>);
	  अन्यथा
	    यह->get_glyph_funcZ = get_glyph_from<CmapSubtable>;
	  अवरोध;
	}
      }
      अन्यथा अगर (unlikely (macroman))
      {
	अगर (is_format4)
	  set_format4_getter (get_glyph_from_macroman<CmapSubtableFormat4::accelerator_t>);
	अन्यथा
	  यह->get_glyph_funcZ = get_glyph_from_macroman<CmapSubtable>;
      }
      अन्यथा अगर (unlikely (mac))
      {
	अगर (is_format4)
	  set_format4_getter (get_glyph_from_ascii<CmapSubtableFormat4::accelerator_t>);
	अन्यथा
	  यह->get_glyph_funcZ = get_glyph_from_ascii<CmapSubtable>;
      }
      अन्यथा
#पूर्ण_अगर
      {
	चयन (subtable->u.format.v) {
	  /* Accelerate format 4 and format 12. */
	  शेष:
	    यह->get_glyph_funcZ = get_glyph_from<CmapSubtable>;
	    अवरोध;
	  हाल 12:
	    यह->get_glyph_funcZ = get_glyph_from<CmapSubtableFormat12>;
	    अवरोध;
	  हाल  4:
	  {
	    hb_barrier ();
	    यह->format4_accel.init (&subtable->u.format4,
				      get_subtable_तथ्य_आकार (subtable));
	    यह->get_glyph_तथ्य = &यह->format4_accel;
	    यह->get_glyph_funcZ = यह->format4_accel.get_glyph_func;
	    अवरोध;
	  }
	}
      }
    }
    ~accelerator_t ()
    {
#अगर_अघोषित HB_NO_OT_FONT_CMAP_CACHE
      hb_free (cache);
#पूर्ण_अगर
      table.destroy ();
    }

    अंतरभूत bool _cached_get (hb_codepoint_t unicode,
			     hb_codepoint_t *glyph) स्थिर
    {
#अगर_अघोषित HB_NO_OT_FONT_CMAP_CACHE
      // cache is always non-null if we have a get_glyph_funcZ
      अचिन्हित v;
      अगर (cache->लो (unicode, &v))
      {
        *glyph = v;
	वापस true;
      }
#पूर्ण_अगर
      bool ret  = यह->get_glyph_funcZ (यह->get_glyph_तथ्य, unicode, glyph);

#अगर_अघोषित HB_NO_OT_FONT_CMAP_CACHE
      अगर (ret)
        cache->set (unicode, *glyph);
#पूर्ण_अगर

      वापस ret;
    }

    bool get_nominal_glyph (hb_codepoint_t  unicode,
			    hb_codepoint_t *glyph) स्थिर
    {
      अगर (unlikely (!यह->get_glyph_funcZ)) वापस false;
      वापस _cached_get (unicode, glyph);
    }

    अचिन्हित पूर्णांक get_nominal_glyphs (अचिन्हित पूर्णांक गिनती,
				     स्थिर hb_codepoint_t *first_unicode,
				     अचिन्हित पूर्णांक unicode_stride,
				     hb_codepoint_t *first_glyph,
				     अचिन्हित पूर्णांक glyph_stride) स्थिर
    {
      अगर (unlikely (!यह->get_glyph_funcZ)) वापस 0;

      अचिन्हित पूर्णांक done;
      क्रम (done = 0;
	   done < गिनती && _cached_get (*first_unicode, first_glyph);
	   done++)
      {
	first_unicode = &StructAtOffsetUnaligned<hb_codepoint_t> (first_unicode, unicode_stride);
	first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      }
      वापस done;
    }

    bool get_variation_glyph (hb_codepoint_t  unicode,
			      hb_codepoint_t  variation_selector,
			      hb_codepoint_t *glyph) स्थिर
    {
      चयन (get_glyph_variant (unicode, variation_selector, glyph))
      {
	हाल GLYPH_VARIANT_NOT_FOUND:	वापस false;
	हाल GLYPH_VARIANT_FOUND:	वापस true;
	हाल GLYPH_VARIANT_USE_DEFAULT:	अवरोध;
      }

      वापस get_nominal_glyph (unicode, glyph);
    }

    glyph_variant_t get_glyph_variant (hb_codepoint_t  unicode,
				       hb_codepoint_t  variation_selector,
				       hb_codepoint_t *glyph) स्थिर
    {
      वापस यह->subtable_uvs->get_glyph_variant (unicode,
						    variation_selector,
						    glyph);
    }

    bool has_तथ्य () स्थिर { वापस table.get_लंबाई (); }

    व्योम collect_unicodes (hb_set_t *out, अचिन्हित पूर्णांक अंक_glyphs) स्थिर
    { subtable->collect_unicodes (out, अंक_glyphs, get_subtable_तथ्य_आकार (subtable)); }
    व्योम collect_mapping (hb_set_t *unicodes, hb_map_t *mapping,
			  अचिन्हित अंक_glyphs = अच_पूर्णांक_उच्च) स्थिर
    { subtable->collect_mapping (unicodes, mapping, अंक_glyphs, get_subtable_तथ्य_आकार (subtable)); }
    व्योम collect_variation_selectors (hb_set_t *out) स्थिर
    { subtable_uvs->collect_variation_selectors (out); }
    व्योम collect_variation_unicodes (hb_codepoint_t variation_selector,
				     hb_set_t *out) स्थिर
    { subtable_uvs->collect_variation_unicodes (variation_selector, out); }

    रक्षित:
    प्रकार bool (*hb_cmap_get_glyph_func_t) (स्थिर व्योम *obj,
					      hb_codepoint_t codepoint,
					      hb_codepoint_t *glyph);
    प्रकार uint16_t (*hb_pua_remap_func_t) (अचिन्हित);

    ढाँचा <typename Type>
    HB_INTERNAL अटल bool get_glyph_from (स्थिर व्योम *obj,
					    hb_codepoint_t codepoint,
					    hb_codepoint_t *glyph)
    {
      स्थिर Type *typed_obj = (स्थिर Type *) obj;
      वापस typed_obj->get_glyph (codepoint, glyph);
    }

    ढाँचा <typename Type, hb_pua_remap_func_t remap>
    HB_INTERNAL अटल bool get_glyph_from_प्रतीक (स्थिर व्योम *obj,
						   hb_codepoint_t codepoint,
						   hb_codepoint_t *glyph)
    {
      स्थिर Type *typed_obj = (स्थिर Type *) obj;
      अगर (likely (typed_obj->get_glyph (codepoint, glyph)))
	वापस true;

      अगर (hb_codepoint_t c = remap (codepoint))
	वापस typed_obj->get_glyph (c, glyph);

      वापस false;
    }

    ढाँचा <typename Type>
    HB_INTERNAL अटल bool get_glyph_from_ascii (स्थिर व्योम *obj,
						  hb_codepoint_t codepoint,
						  hb_codepoint_t *glyph)
    {
      स्थिर Type *typed_obj = (स्थिर Type *) obj;
      वापस codepoint < 0x80 && typed_obj->get_glyph (codepoint, glyph);
    }

    ढाँचा <typename Type>
    HB_INTERNAL अटल bool get_glyph_from_macroman (स्थिर व्योम *obj,
						     hb_codepoint_t codepoint,
						     hb_codepoint_t *glyph)
    {
      अगर (get_glyph_from_ascii<Type> (obj, codepoint, glyph))
	वापस true;

      स्थिर Type *typed_obj = (स्थिर Type *) obj;
      अचिन्हित c = unicode_to_macroman (codepoint);
      वापस c && typed_obj->get_glyph (c, glyph);
    }

    अचिन्हित पूर्णांक get_subtable_तथ्य_आकार (स्थिर CmapSubtable *subtable) स्थिर
    {
      अचिन्हित पूर्णांक table_लंबाई = यह->table.get_लंबाई ();
      uintptr_t table_start = (uintptr_t) (स्थिर व्योम *) यह->table.लो ();
      uintptr_t subtable_addr = (uintptr_t) (स्थिर व्योम *) subtable;
      अगर (unlikely (subtable_addr < table_start))
	वापस 0;

      uintptr_t subtable_offset = subtable_addr - table_start;
      अगर (unlikely (subtable_offset >= table_लंबाई))
	वापस 0;

      वापस table_लंबाई - (अचिन्हित पूर्णांक) subtable_offset;
    }

    निजी:
    hb_nonnull_ptr_t<स्थिर CmapSubtable> subtable;
    hb_nonnull_ptr_t<स्थिर CmapSubtable> subtable_uvs;

    hb_cmap_get_glyph_func_t get_glyph_funcZ = nullptr;
    स्थिर व्योम *get_glyph_तथ्य = nullptr;

    CmapSubtableFormat4::accelerator_t format4_accel;

#अगर_अघोषित HB_NO_OT_FONT_CMAP_CACHE
    cache_t *cache = nullptr;
#पूर्ण_अगर

    खुला:
    hb_blob_ptr_t<cmap> table;
  };

  रक्षित:

  स्थिर CmapSubtable *find_subtable (अचिन्हित पूर्णांक platform_id,
				     अचिन्हित पूर्णांक encoding_id) स्थिर
  {
    EncodingRecord key;
    key.platformID = platform_id;
    key.encodingID = encoding_id;

    स्थिर EncodingRecord &परिणाम = encodingRecord.द्वा_खोज (key);
    अगर (!परिणाम.subtable)
      वापस nullptr;

    वापस &(यह+परिणाम.subtable);
  }

  स्थिर CmapSubtable *find_variation_selector_subtable () स्थिर
  {
    स्थिर CmapSubtable *format14 = nullptr;
    क्रम (स्थिर EncodingRecord& record : encodingRecord)
    {
      अगर (record.platformID != 0 || record.encodingID != 5)
	जारी;

      स्थिर CmapSubtable *subtable = &(यह+record.subtable);
#अगर_अघोषित HB_NO_BEYOND_64K
      अगर (subtable->u.format.v == 15)
	वापस subtable;
#पूर्ण_अगर
      अगर (subtable->u.format.v == 14)
	format14 = subtable;
    }
    वापस format14;
  }

  खुला:

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) &&
		  hb_barrier () &&
		  likely (version == 0) &&
		  encodingRecord.sanitize (c, यह));
  }

 निजी:

  अटल bool filter_encoding_records_for_subset(स्थिर cmap* cmap,
                                                 स्थिर EncodingRecord& _)
  {
    वापस
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
  खुला:
  DEFINE_आकार_ARRAY (4, encodingRecord);
};

काष्ठा DMAP : cmap
{
  अटल constexpr hb_tag_t tableTag = HB_OT_TAG_DMAP;

  bool subset (hb_subset_context_t *c) स्थिर
  { वापस cmap::subset (c, true); }
};

काष्ठा cmap_accelerator_t
{
  cmap_accelerator_t (hb_face_t *face) :
    dmap (face, HB_OT_TAG_DMAP),
    base (face)
  {}

  bool get_nominal_glyph (hb_codepoint_t  unicode,
			  hb_codepoint_t *glyph) स्थिर
  {
    वापस dmap.get_nominal_glyph (unicode, glyph) ||
	   base.get_nominal_glyph (unicode, glyph);
  }

  अचिन्हित पूर्णांक get_nominal_glyphs (अचिन्हित पूर्णांक गिनती,
				   स्थिर hb_codepoint_t *first_unicode,
				   अचिन्हित पूर्णांक unicode_stride,
				   hb_codepoint_t *first_glyph,
				   अचिन्हित पूर्णांक glyph_stride) स्थिर
  {
    अगर (likely (!dmap.has_तथ्य ()))
      वापस base.get_nominal_glyphs (गिनती,
				      first_unicode, unicode_stride,
				      first_glyph, glyph_stride);

    अचिन्हित पूर्णांक done;
    क्रम (done = 0;
	 done < गिनती && get_nominal_glyph (*first_unicode, first_glyph);
	 done++)
    {
      first_unicode = &StructAtOffsetUnaligned<hb_codepoint_t> (first_unicode, unicode_stride);
      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
    }
    वापस done;
  }

  bool get_variation_glyph (hb_codepoint_t  unicode,
			    hb_codepoint_t  variation_selector,
			    hb_codepoint_t *glyph) स्थिर
  {
    glyph_variant_t परिणाम = dmap.get_glyph_variant (unicode,
						     variation_selector,
						     glyph);
    अगर (परिणाम == GLYPH_VARIANT_NOT_FOUND)
      परिणाम = base.get_glyph_variant (unicode, variation_selector, glyph);

    चयन (परिणाम)
    {
      हाल GLYPH_VARIANT_NOT_FOUND: वापस false;
      हाल GLYPH_VARIANT_FOUND: वापस true;
      हाल GLYPH_VARIANT_USE_DEFAULT: अवरोध;
    }

    वापस get_nominal_glyph (unicode, glyph);
  }

  व्योम collect_unicodes (hb_set_t *out, अचिन्हित पूर्णांक अंक_glyphs) स्थिर
  {
    base.collect_unicodes (out, अंक_glyphs);
    dmap.collect_unicodes (out, अंक_glyphs);
  }

  व्योम collect_mapping (hb_set_t *unicodes, hb_map_t *mapping,
			अचिन्हित अंक_glyphs = अच_पूर्णांक_उच्च) स्थिर
  {
    base.collect_mapping (unicodes, mapping, अंक_glyphs);
    dmap.collect_mapping (unicodes, mapping, अंक_glyphs);
  }

  व्योम collect_variation_selectors (hb_set_t *out) स्थिर
  {
    base.collect_variation_selectors (out);
    dmap.collect_variation_selectors (out);
  }

  व्योम collect_variation_unicodes (hb_codepoint_t variation_selector,
				   hb_set_t *out) स्थिर
  {
    base.collect_variation_unicodes (variation_selector, out);
    dmap.collect_variation_unicodes (variation_selector, out);
  }

  व्योम closure_glyphs (स्थिर hb_set_t *unicodes,
		       hb_set_t       *glyphset) स्थिर
  {
    अगर (likely (!dmap.has_तथ्य ()))
    {
      base.table->closure_glyphs (unicodes, glyphset);
      वापस;
    }

    hb_set_t variation_selectors;
    collect_variation_selectors (&variation_selectors);
    क्रम (hb_codepoint_t variation_selector : variation_selectors)
    {
      अगर (!unicodes->has (variation_selector))
	जारी;

      hb_set_t variation_unicodes;
      collect_variation_unicodes (variation_selector, &variation_unicodes);
      क्रम (hb_codepoint_t unicode : variation_unicodes)
      {
	अगर (!unicodes->has (unicode))
	  जारी;

	hb_codepoint_t glyph;
	अगर (get_variation_glyph (unicode, variation_selector, &glyph))
	  glyphset->add (glyph);
      }
    }
  }

  cmap::accelerator_t dmap;
  cmap::accelerator_t base;
};

} /* namespace OT */


#पूर्ण_अगर /* HB_OT_CMAP_TABLE_HH */
