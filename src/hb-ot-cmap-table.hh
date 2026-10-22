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

#جيڪڏهن_وصف_نه_ٿيل HB_OT_CMAP_جدول_HH
#وصف_ڪريو HB_OT_CMAP_جدول_HH

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
#وصف_ڪريو HB_OT_ٽيگ_cmap HB_ٽيگ('c','m','a','p')
#وصف_ڪريو HB_OT_ٽيگ_DMAP HB_ٽيگ('D','M','A','P')

نطاق OT {

ساكن مضمن uint8_t unicode_منزل_macroman (hb_codepoint_t u)
{
  ساكن ثابت بنية unicode_منزل_macroman_t
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
  خودڪار *c = hb_bsearch (u, mapping, ARRAY_ڊيگهائي (mapping), حجم (mapping[0]),
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
    أرجع سچ;
  }

  بنانشان get_language () ثابت
  {
    أرجع language;
  }

  فراغ collect_unicodes (hb_سيٽ_t *ٻاهريون) ثابت
  {
    لكل (بنانشان صحيح i = 0; i < 256; i++)
      إذا (glyphIdArray[i])
	ٻاهريون->جوڙيو (i);
  }

  فراغ collect_mapping (hb_سيٽ_t *unicodes, /*ٻاهريون*/
			hb_map_t *mapping /*ٻاهريون*/) ثابت
  {
    لكل (بنانشان i = 0; i < 256; i++)
      إذا (glyphIdArray[i])
      {
	hb_codepoint_t glyph = glyphIdArray[i];
	unicodes->جوڙيو (i);
	mapping->سيٽ (i, glyph);
      }
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  محمي:
  HBUINT16	format;		/* Format number is set to 0. */
  HBUINT16	ڊيگهائي;		/* Byte length of this subtable. */
  HBUINT16	language;	/* Ignore. */
  HBUINT8	glyphIdArray[256];/* An array that maps character
				 * code to glyph index values. */
  عام:
  DEFINE_سائيز_STATIC (6 + 256);
};

بنية CmapSubtableFormat4
{
  سانچو<قسم_نالو Iterator,
      قسم_نالو Writer,
	   hb_requires (hb_is_iterator (Iterator))>
  فراغ منزل_ranges (Iterator it, Writer& range_writer)
  {
    hb_codepoint_t شروعات_cp = 0, prev_لاڳيتو_شروعات_cp = 0, لاڳيتو_شروعات_cp = 0, پڇاڙي_cp = 0, آخري_gid = 0;
    صحيح لاڳيتو_ڊيگهائي = 0 , delta = 0, prev_delta = 0;

    تعداد {
      پهريون_SUB_RANGE,
      FOLLOWING_SUB_RANGE,
    } گهڻائي;

    طالما (it) {
      // Start a new range
      {
        ثابت خودڪار& pair = *it;
        شروعات_cp = pair.پهريون;
        prev_لاڳيتو_شروعات_cp = شروعات_cp;
        لاڳيتو_شروعات_cp = شروعات_cp;
        پڇاڙي_cp = شروعات_cp;
        آخري_gid = pair.second;
        لاڳيتو_ڊيگهائي = 1;
        prev_delta = 0;
      }

      delta = آخري_gid - شروعات_cp;
      گهڻائي = پهريون_SUB_RANGE;
      it++;

      طالما (it) {
        // Process range
        ثابت خودڪار& pair = *it;
        hb_codepoint_t ايندڙ_cp = pair.پهريون;
        hb_codepoint_t ايندڙ_gid = pair.second;
        إذا (ايندڙ_cp != پڇاڙي_cp + 1) {
          // Current range is over, stop processing.
          اكسر;
        }

        إذا (ايندڙ_gid == آخري_gid + 1) {
          // The current run continues.
          پڇاڙي_cp = ايندڙ_cp;
          لاڳيتو_ڊيگهائي++;
          آخري_gid = ايندڙ_gid;
          it++;
          استمر;
        }

        // A new run is starting, decide if we want to commit the current run.
        صحيح ورهايو_لاڳت = (گهڻائي == پهريون_SUB_RANGE) ? 8 : 16;
        صحيح لاڳيتو_لاڳت = لاڳيتو_ڊيگهائي * 2;
        إذا (لاڳيتو_لاڳت >= ورهايو_لاڳت) {
          commit_current_range(شروعات_cp,
                               prev_لاڳيتو_شروعات_cp,
                               لاڳيتو_شروعات_cp,
                               پڇاڙي_cp,
                               delta,
                               prev_delta,
                               ورهايو_لاڳت,
                               range_writer);
          شروعات_cp = ايندڙ_cp;
        }

        // Start the new run
        گهڻائي = FOLLOWING_SUB_RANGE;
        prev_لاڳيتو_شروعات_cp = لاڳيتو_شروعات_cp;
        لاڳيتو_شروعات_cp = ايندڙ_cp;
        پڇاڙي_cp = ايندڙ_cp;
        prev_delta = delta;
        delta = ايندڙ_gid - لاڳيتو_شروعات_cp;
        لاڳيتو_ڊيگهائي = 1;
        آخري_gid = ايندڙ_gid;
        it++;
      }

      // Finalize range
      commit_current_range (شروعات_cp,
                            prev_لاڳيتو_شروعات_cp,
                            لاڳيتو_شروعات_cp,
                            پڇاڙي_cp,
                            delta,
                            prev_delta,
                            8,
                            range_writer);
    }

    إذا (likely (پڇاڙي_cp != 0xFFFF)) {
      range_writer (0xFFFF, 0xFFFF, 1);
    }
  }

  /*
   * Writes the current range as either one or two ranges depending on what is most efficient.
   */
  سانچو<قسم_نالو Writer>
  فراغ commit_current_range (hb_codepoint_t شروعات,
                             hb_codepoint_t prev_لاڳيتو_شروعات,
                             hb_codepoint_t لاڳيتو_شروعات,
                             hb_codepoint_t پڇاڙي,
                             صحيح لاڳيتو_delta,
                             صحيح اڳيون_لاڳيتو_delta,
                             صحيح ورهايو_لاڳت,
                             Writer& range_writer) {
    منطقي should_ورهايو = خطأ;
    إذا (شروعات < لاڳيتو_شروعات && لاڳيتو_شروعات < پڇاڙي) {
      صحيح لاڳيتو_لاڳت = (پڇاڙي - لاڳيتو_شروعات + 1) * 2;
      إذا (لاڳيتو_لاڳت >= ورهايو_لاڳت) {
        should_ورهايو = سچ;
      }
    }

    // A zero delta is also the marker for ranges that need a
    // glyphIdArray; serialize_rangeoffset_glyid() tells the two cases
    // apart by re-checking the mapping.
    إذا (should_ورهايو) {
      إذا (شروعات == prev_لاڳيتو_شروعات)
        range_writer (شروعات, لاڳيتو_شروعات - 1, اڳيون_لاڳيتو_delta);
      وإلا
        range_writer (شروعات, لاڳيتو_شروعات - 1, 0);
      range_writer (لاڳيتو_شروعات, پڇاڙي, لاڳيتو_delta);
      أرجع;
    }


    إذا (شروعات == لاڳيتو_شروعات) {
      // Range is only a run
      range_writer (شروعات, پڇاڙي, لاڳيتو_delta);
      أرجع;
    }

    // Write only a single non-run range.
    range_writer (شروعات, پڇاڙي, 0);
  }

  سانچو<قسم_نالو Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  بنانشان serialize_ڳوليو_segcount (Iterator it) {
    بنية Counter {
      بنانشان segcount = 0;

      فراغ عامل() (hb_codepoint_t شروعات,
                       hb_codepoint_t پڇاڙي,
                       صحيح delta) {
        segcount++;
      }
    } counter;

    منزل_ranges (+it, counter);
    أرجع counter.segcount;
  }


  سانچو<قسم_نالو Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  منطقي serialize_شروعات_پڇاڙي_delta_arrays (hb_serialize_context_t *c,
                                         Iterator it,
                                         صحيح segcount)
  {
    بنية Writer {
      hb_serialize_context_t *serializer_;
      HBUINT16* پڇاڙي_ڪوڊ;
      HBUINT16* شروعات_ڪوڊ;
      HBINT16* id_delta_;
      صحيح اشاريو_3;

      Writer(hb_serialize_context_t *serializer)
          : serializer_(serializer),
            پڇاڙي_ڪوڊ(خالي_اشارو),
            شروعات_ڪوڊ(خالي_اشارو),
            id_delta_(خالي_اشارو),
            اشاريو_3 (0) {}
      فراغ عامل() (hb_codepoint_t شروعات,
                       hb_codepoint_t پڇاڙي,
                       صحيح delta) {
        شروعات_ڪوڊ[اشاريو_3] = شروعات;
        پڇاڙي_ڪوڊ[اشاريو_3] = پڇاڙي;
        id_delta_[اشاريو_3] = delta;
        اشاريو_3++;
      }
    } writer(c);

    writer.پڇاڙي_ڪوڊ = c->allocate_سائيز<HBUINT16> (HBUINT16::static_سائيز * segcount, خطأ);
    (فراغ) c->allocate_سائيز<HBUINT16> (2); // padding
    writer.شروعات_ڪوڊ = c->allocate_سائيز<HBUINT16> (HBUINT16::static_سائيز * segcount, خطأ);
    writer.id_delta_ = c->allocate_سائيز<HBINT16> (HBINT16::static_سائيز * segcount, خطأ);

    إذا (unlikely (!writer.پڇاڙي_ڪوڊ || !writer.شروعات_ڪوڊ || !writer.id_delta_)) أرجع خطأ;

    منزل_ranges (+it, writer);
    أرجع سچ;
  }

  سانچو<قسم_نالو Iterator,
          hb_requires (hb_is_iterator (Iterator))>
  HBUINT16* serialize_rangeoffset_glyid (hb_serialize_context_t *c,
                                         Iterator it,
					 HBUINT16 *پڇاڙي_ڪوڊ_2,
					 HBUINT16 *شروعات_ڪوڊ_2,
					 HBINT16 *idDelta,
					 بنانشان segcount)
  {
    hb_map_t cp_منزل_gid { it };

    HBUINT16 *id_Range_وٿي = c->allocate_سائيز<HBUINT16> (HBUINT16::static_سائيز * segcount);
    إذا (unlikely (!c->check_success (id_Range_وٿي))) أرجع خالي_اشارو;
    إذا (unlikely ((حرف *)id_Range_وٿي - (حرف *)idDelta != (صحيح) segcount * (صحيح) HBINT16::static_سائيز)) أرجع خالي_اشارو;

    لكل (بنانشان i : hb_range (segcount))
    {
      /* A non-zero delta is always a real run. A zero delta is
       * ambiguous: identity mapping, or the marker for a range that
       * needs a glyphIdArray. Only zero deltas need a check. */
      صحيح delta = idDelta[i];
      إذا (delta)
	استمر;
      منطقي use_delta = سچ;
      لكل (hb_codepoint_t cp = شروعات_ڪوڊ_2[i]; cp <= پڇاڙي_ڪوڊ_2[i]; cp++)
      {
	hb_codepoint_t gid = cp_منزل_gid.get (cp);
	إذا (gid == HB_MAP_قدر_INVALID)
	  gid = 0;
	إذا (gid != cp)
	{
	  use_delta = خطأ;
	  اكسر;
	}
      }
      إذا (use_delta)
	استمر;

      id_Range_وٿي[i] = 2 * (c->شروعات_embed<HBUINT16> () - id_Range_وٿي - i);
      لكل (hb_codepoint_t cp = شروعات_ڪوڊ_2[i]; cp <= پڇاڙي_ڪوڊ_2[i]; cp++)
      {
        HBUINT16 gid;
        gid = cp_منزل_gid[cp];
        c->copy<HBUINT16> (gid);
      }
    }

    أرجع id_Range_وٿي;
  }

  سانچو<قسم_نالو Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  فراغ serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    خودڪار format4_iter =
    + it
    | hb_filter ([&] (ثابت hb_codepoint_pair_t _)
		 { أرجع _.پهريون <= 0xFFFF; })
    ;

    إذا (!format4_iter) أرجع;

    بنانشان جدول_initpos = c->ڊيگهائي ();
    إذا (unlikely (!c->extend_min (هذا))) أرجع;
    هذا->format = 4;

    hb_ويڪٽر_t<hb_codepoint_pair_t> cp_منزل_gid {
      format4_iter
    };

    //serialize endCode[], startCode[], idDelta[]
    HBUINT16* پڇاڙي_ڪوڊ_2 = c->شروعات_embed<HBUINT16> ();
    بنانشان segcount = serialize_ڳوليو_segcount (cp_منزل_gid.iter());
    إذا (unlikely (!serialize_شروعات_پڇاڙي_delta_arrays (c, cp_منزل_gid.iter(), segcount)))
      أرجع;

    HBUINT16 *شروعات_ڪوڊ_2 = پڇاڙي_ڪوڊ_2 + segcount + 1;
    HBINT16 *idDelta = ((HBINT16*)شروعات_ڪوڊ_2) + segcount;

    HBUINT16 *id_Range_وٿي = serialize_rangeoffset_glyid (c,
                                                           cp_منزل_gid.iter (),
                                                           پڇاڙي_ڪوڊ_2,
                                                           شروعات_ڪوڊ_2,
                                                           idDelta,
                                                           segcount);
    إذا (unlikely (!c->check_success (id_Range_وٿي))) أرجع;

    هذا->ڊيگهائي = c->ڊيگهائي () - جدول_initpos;
    إذا ((ڊگهو ڊگهو) هذا->ڊيگهائي != (ڊگهو ڊگهو) c->ڊيگهائي () - جدول_initpos)
    {
      c->err (HB_SERIALIZE_ERROR_INT_حد_کان_مٿي);
      أرجع;
    }

    هذا->seg_ڳڻپ_X2 = segcount * 2;
    هذا->داخلا_Selector = hb_max (1u, hb_ٻٽو_انگ_storage (segcount)) - 1;
    هذا->ڳولا_Range = 2 * (1u << هذا->داخلا_Selector);
    هذا->range_سرڪايو = segcount * 2 > هذا->ڳولا_Range
		       ? 2 * segcount - هذا->ڳولا_Range
		       : 0;
  }

  بنانشان get_language () ثابت
  {
    أرجع language;
  }

  بنية accelerator_t
  {
    accelerator_t () {}
    accelerator_t (ثابت CmapSubtableFormat4 *subtable) = ميٽيو;

    فراغ init (ثابت CmapSubtableFormat4 *subtable,
	       بنانشان صحيح              subtable_معلومات_سائيز)
    {
      seg_ڳڻپ_2 = subtable->seg_ڳڻپ_X2 / 2;
      پڇاڙي_ڳڻپ = subtable->values.arrayZ;
      شروعات_ڳڻپ = پڇاڙي_ڳڻپ + seg_ڳڻپ_2 + 1;
      idDelta = شروعات_ڳڻپ + seg_ڳڻپ_2;
      id_Range_وٿي = idDelta + seg_ڳڻپ_2;
      glyphIdArray = id_Range_وٿي + seg_ڳڻپ_2;

      بنانشان صحيح قدرون_وٿي = 16 + 8 * seg_ڳڻپ_2;
      glyph_Id_Array_ڊيگهائي = subtable_معلومات_سائيز > قدرون_وٿي
			   ? (subtable_معلومات_سائيز - قدرون_وٿي) / 2
			   : 0;
    }

    منطقي get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) ثابت
    {
      بنية CustomRange
      {
	صحيح cmp (hb_codepoint_t k,
		 بنانشان distance) ثابت
	{
	  إذا (k > آخري) أرجع +1;
	  إذا (k < (&آخري)[distance]/*پهريون*/) أرجع -1;
	  أرجع 0;
	}
	HBUINT16 آخري;
      };

      ثابت HBUINT16 *مليو = hb_bsearch (codepoint,
					  هذا->پڇاڙي_ڳڻپ,
					  هذا->seg_ڳڻپ_2,
					  حجم (CustomRange),
					  hb_cmp_طريقو<hb_codepoint_t, CustomRange, بنانشان>,
					  هذا->seg_ڳڻپ_2 + 1);
      إذا (unlikely (!مليو))
	أرجع خطأ;
      بنانشان صحيح i = مليو - پڇاڙي_ڳڻپ;

      hb_codepoint_t gid;
      بنانشان صحيح range_وٿي = هذا->id_Range_وٿي[i];
      إذا (range_وٿي == 0)
	gid = codepoint + هذا->idDelta[i];
      وإلا
      {
	/* Somebody has been smoking... */
	بنانشان صحيح اشاريو_2 = range_وٿي / 2 + (codepoint - هذا->شروعات_ڳڻپ[i]) + i - هذا->seg_ڳڻپ_2;
	إذا (unlikely (اشاريو_2 >= هذا->glyph_Id_Array_ڊيگهائي))
	  أرجع خطأ;
	gid = هذا->glyphIdArray[اشاريو_2];
	إذا (unlikely (!gid))
	  أرجع خطأ;
	gid += هذا->idDelta[i];
      }
      gid &= 0xFFFFu;
      إذا (unlikely (!gid))
	أرجع خطأ;
      *glyph = gid;
      أرجع سچ;
    }

    HB_اندروني ساكن منطقي get_glyph_func (ثابت فراغ *obj, hb_codepoint_t codepoint, hb_codepoint_t *glyph)
    { أرجع ((ثابت accelerator_t *) obj)->get_glyph (codepoint, glyph); }

    فراغ collect_unicodes (hb_سيٽ_t *ٻاهريون) ثابت
    {
      بنانشان صحيح ڳڻپ = هذا->seg_ڳڻپ_2;
      إذا (ڳڻپ && هذا->شروعات_ڳڻپ[ڳڻپ - 1] == 0xFFFFu)
	ڳڻپ--; /* Skip sentinel segment. */
      لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
      {
	hb_codepoint_t شروعات = هذا->شروعات_ڳڻپ[i];
	hb_codepoint_t پڇاڙي = هذا->پڇاڙي_ڳڻپ[i];
	بنانشان صحيح range_وٿي = هذا->id_Range_وٿي[i];
        ٻاهريون->جوڙيو_range(شروعات, پڇاڙي);
	إذا (range_وٿي == 0)
	{
	  لكل (hb_codepoint_t codepoint = شروعات; codepoint <= پڇاڙي; codepoint++)
	  {
	    hb_codepoint_t gid = (codepoint + هذا->idDelta[i]) & 0xFFFFu;
	    إذا (unlikely (!gid))
              ٻاهريون->del(codepoint);
	  }
	}
	وإلا
	{
	  لكل (hb_codepoint_t codepoint = شروعات; codepoint <= پڇاڙي; codepoint++)
	  {
	    بنانشان صحيح اشاريو_2 = range_وٿي / 2 + (codepoint - هذا->شروعات_ڳڻپ[i]) + i - هذا->seg_ڳڻپ_2;
	    إذا (unlikely (اشاريو_2 >= هذا->glyph_Id_Array_ڊيگهائي))
            {
              ٻاهريون->del_range (codepoint, پڇاڙي);
	      اكسر;
            }
	    hb_codepoint_t gid = هذا->glyphIdArray[اشاريو_2];
	    إذا (unlikely (!gid))
              ٻاهريون->del(codepoint);
	  }
	}
      }
    }

    فراغ collect_mapping (hb_سيٽ_t *unicodes, /*ٻاهريون*/
			  hb_map_t *mapping /*ٻاهريون*/) ثابت
    {
      // TODO(grieger): optimize similar to collect_unicodes
      // (ie. use add_range())
      بنانشان ڳڻپ = هذا->seg_ڳڻپ_2;
      إذا (ڳڻپ && هذا->شروعات_ڳڻپ[ڳڻپ - 1] == 0xFFFFu)
	ڳڻپ--; /* Skip sentinel segment. */
      لكل (بنانشان i = 0; i < ڳڻپ; i++)
      {
	hb_codepoint_t شروعات = هذا->شروعات_ڳڻپ[i];
	hb_codepoint_t پڇاڙي = هذا->پڇاڙي_ڳڻپ[i];
	بنانشان range_وٿي = هذا->id_Range_وٿي[i];
	إذا (range_وٿي == 0)
	{
	  لكل (hb_codepoint_t codepoint = شروعات; codepoint <= پڇاڙي; codepoint++)
	  {
	    hb_codepoint_t gid = (codepoint + هذا->idDelta[i]) & 0xFFFFu;
	    إذا (unlikely (!gid))
	      استمر;
	    unicodes->جوڙيو (codepoint);
	    mapping->سيٽ (codepoint, gid);
	  }
	}
	وإلا
	{
	  لكل (hb_codepoint_t codepoint = شروعات; codepoint <= پڇاڙي; codepoint++)
	  {
	    بنانشان اشاريو_2 = range_وٿي / 2 + (codepoint - هذا->شروعات_ڳڻپ[i]) + i - هذا->seg_ڳڻپ_2;
	    إذا (unlikely (اشاريو_2 >= هذا->glyph_Id_Array_ڊيگهائي))
	      اكسر;
	    hb_codepoint_t gid = هذا->glyphIdArray[اشاريو_2];
	    إذا (unlikely (!gid))
	      استمر;
            gid = (gid + هذا->idDelta[i]) & 0xFFFFu;
	    unicodes->جوڙيو (codepoint);
	    mapping->سيٽ (codepoint, gid);
	  }
	}
      }
    }

    ثابت HBUINT16 *پڇاڙي_ڳڻپ;
    ثابت HBUINT16 *شروعات_ڳڻپ;
    ثابت HBUINT16 *idDelta;
    ثابت HBUINT16 *id_Range_وٿي;
    ثابت HBUINT16 *glyphIdArray;
    بنانشان صحيح seg_ڳڻپ_2;
    بنانشان صحيح glyph_Id_Array_ڊيگهائي;
  };

  منطقي get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph,
		  بنانشان صحيح subtable_معلومات_سائيز) ثابت
  {
    accelerator_t accel;
    accel.init (هذا, subtable_معلومات_سائيز);
    أرجع accel.get_glyph_func (&accel, codepoint, glyph);
  }
  منطقي get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) ثابت
  { أرجع خطأ; }

  فراغ collect_unicodes (hb_سيٽ_t *ٻاهريون, بنانشان صحيح subtable_معلومات_سائيز) ثابت
  {
    accelerator_t accel;
    accel.init (هذا, subtable_معلومات_سائيز);
    accel.collect_unicodes (ٻاهريون);
  }
  فراغ collect_unicodes (hb_سيٽ_t *ٻاهريون) ثابت
  { collect_unicodes (ٻاهريون, ڊيگهائي); }

  فراغ collect_mapping (hb_سيٽ_t *unicodes, /*ٻاهريون*/
			hb_map_t *mapping, /*ٻاهريون*/
			بنانشان صحيح subtable_معلومات_سائيز) ثابت
  {
    accelerator_t accel;
    accel.init (هذا, subtable_معلومات_سائيز);
    accel.collect_mapping (unicodes, mapping);
  }
  فراغ collect_mapping (hb_سيٽ_t *unicodes, /*ٻاهريون*/
			hb_map_t *mapping /*ٻاهريون*/) ثابت
  { collect_mapping (unicodes, mapping, ڊيگهائي); }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!c->check_struct (هذا)))
      return_trace (خطأ);
    hb_barrier ();

    return_trace (c->check_range (values, 2 + 4 * seg_ڳڻپ_X2));
  }

  محمي:
  HBUINT16	format;		/* Format number is set to 4. */
  HBUINT16	ڊيگهائي;		/* This is the length in bytes of the
				 * subtable. */
  HBUINT16	language;	/* Ignore. */
  HBUINT16	seg_ڳڻپ_X2;	/* 2 x segCount. */
  HBUINT16	ڳولا_Range;	/* 2 * (2**floor(log2(segCount))) */
  HBUINT16	داخلا_Selector;	/* log2(searchRange/2) */
  HBUINT16	range_سرڪايو;	/* 2 x segCount - searchRange */

  Unsized_Array_جو<HBUINT16>
		values;
#إذا 0
  HBUINT16	پڇاڙي_ڳڻپ[seg_ڳڻپ_2];	/* End characterCode for each segment,
					 * last=0xFFFFu. */
  HBUINT16	reservedPad;		/* Set to 0. */
  HBUINT16	شروعات_ڳڻپ[seg_ڳڻپ_2];	/* Start character code for each segment. */
  HBINT16		idDelta[seg_ڳڻپ_2];	/* Delta for all character codes in segment. */
  HBUINT16	id_Range_وٿي[seg_ڳڻپ_2];/* Offsets into glyphIdArray or 0 */
  Unsized_Array_جو<HBUINT16>
		glyphIdArray;	/* Glyph index array (arbitrary length) */
#جيڪڏهن_پورو

  عام:
  DEFINE_سائيز_ARRAY (14, values);
};

بنية Cmap_Subtable_Long_گروهه
{
  دوست بنية CmapSubtableFormat12;
  دوست بنية CmapSubtableFormat13;
  سانچو<قسم_نالو U>
  دوست بنية CmapSubtableLongSegmented;
  دوست بنية cmap;

  صحيح cmp (hb_codepoint_t codepoint) ثابت
  {
    إذا (codepoint < شروعات_Char_ڪوڊ) أرجع -1;
    إذا (codepoint > پڇاڙي_Char_ڪوڊ)   أرجع +1;
    أرجع 0;
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  خاص:
  HBUINT32		شروعات_Char_ڪوڊ;	/* First character code in this group. */
  HBUINT32		پڇاڙي_Char_ڪوڊ;	/* Last character code in this group. */
  HBUINT32		glyphID;	/* Glyph index; interpretation depends on
					 * subtable format. */
  عام:
  DEFINE_سائيز_STATIC (12);
};
DECLARE_NULL_NAMESPACE_بائيٽون (OT, Cmap_Subtable_Long_گروهه);

سانچو <قسم_نالو UINT>
بنية CmapSubtableTrimmed
{
  منطقي get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) ثابت
  {
    /* Rely on our implicit array bound-checking. */
    hb_codepoint_t gid = glyphIdArray[codepoint - شروعات_Char_ڪوڊ];
    إذا (unlikely (!gid))
      أرجع خطأ;
    *glyph = gid;
    أرجع سچ;
  }

  بنانشان get_language () ثابت
  {
    أرجع language;
  }

  فراغ collect_unicodes (hb_سيٽ_t *ٻاهريون) ثابت
  {
    hb_codepoint_t شروعات = شروعات_Char_ڪوڊ;
    بنانشان صحيح ڳڻپ = glyphIdArray.len;
    لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
      إذا (glyphIdArray[i])
	ٻاهريون->جوڙيو (شروعات + i);
  }

  فراغ collect_mapping (hb_سيٽ_t *unicodes, /*ٻاهريون*/
			hb_map_t *mapping /*ٻاهريون*/) ثابت
  {
    hb_codepoint_t شروعات_cp = شروعات_Char_ڪوڊ;
    بنانشان ڳڻپ = glyphIdArray.len;
    لكل (بنانشان i = 0; i < ڳڻپ; i++)
      إذا (glyphIdArray[i])
      {
	hb_codepoint_t unicode = شروعات_cp + i;
	hb_codepoint_t glyphid = glyphIdArray[i];
	unicodes->جوڙيو (unicode);
	mapping->سيٽ (unicode, glyphid);
      }
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && glyphIdArray.sanitize (c));
  }

  محمي:
  UINT		formatReserved;	/* Subtable format and (maybe) padding. */
  UINT		ڊيگهائي;		/* Byte length of this subtable. */
  UINT		language;	/* Ignore. */
  UINT		شروعات_Char_ڪوڊ;	/* First character code covered. */
  Array_جو<HBGlyphID16, UINT>
		glyphIdArray;	/* Array of glyph index values for character
				 * codes in the range. */
  عام:
  DEFINE_سائيز_ARRAY (5 * حجم (UINT), glyphIdArray);
};

بنية CmapSubtableFormat6  : CmapSubtableTrimmed<HBUINT16> {};
بنية CmapSubtableFormat10 : CmapSubtableTrimmed<HBUINT32> {};

سانچو <قسم_نالو T>
بنية CmapSubtableLongSegmented
{
  دوست بنية cmap;

  منطقي get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) ثابت
  {
    hb_codepoint_t gid = T::گروهه_get_glyph (groups.bsearch (codepoint), codepoint);
    إذا (unlikely (!gid))
      أرجع خطأ;
    *glyph = gid;
    أرجع سچ;
  }

  بنانشان get_language () ثابت
  {
    أرجع language;
  }

  فراغ collect_unicodes (hb_سيٽ_t *ٻاهريون, بنانشان صحيح انگ_glyphs) ثابت
  {
    لكل (بنانشان صحيح i = 0; i < هذا->groups.len; i++)
    {
      hb_codepoint_t شروعات = هذا->groups[i].شروعات_Char_ڪوڊ;
      hb_codepoint_t پڇاڙي = hb_min ((hb_codepoint_t) هذا->groups[i].پڇاڙي_Char_ڪوڊ,
				   (hb_codepoint_t) HB_UNICODE_MAX);
      hb_codepoint_t gid = هذا->groups[i].glyphID;
      إذا (!gid)
      {
	/* Intention is: if (hb_is_same (T, CmapSubtableFormat13)) continue; */
	إذا (! T::گروهه_get_glyph (هذا->groups[i], پڇاڙي)) استمر;
	شروعات++;
	gid++;
      }
      إذا (unlikely ((بنانشان صحيح) gid >= انگ_glyphs)) استمر;
      إذا (unlikely ((بنانشان صحيح) (gid + پڇاڙي - شروعات) >= انگ_glyphs))
	پڇاڙي = شروعات + (hb_codepoint_t) انگ_glyphs - gid;

      ٻاهريون->جوڙيو_range (شروعات, hb_min (پڇاڙي, 0x10FFFFu));
    }
  }

  فراغ collect_mapping (hb_سيٽ_t *unicodes, /*ٻاهريون*/
			hb_map_t *mapping, /*ٻاهريون*/
			بنانشان انگ_glyphs) ثابت
  {
    hb_codepoint_t آخري_پڇاڙي = 0;
    بنانشان ڳڻپ = هذا->groups.len;
    لكل (بنانشان i = 0; i < ڳڻپ; i++)
    {
      hb_codepoint_t شروعات = هذا->groups.arrayZ[i].شروعات_Char_ڪوڊ;
      hb_codepoint_t پڇاڙي = hb_min ((hb_codepoint_t) هذا->groups.arrayZ[i].پڇاڙي_Char_ڪوڊ,
				   (hb_codepoint_t) HB_UNICODE_MAX);
      إذا (unlikely (شروعات > پڇاڙي || شروعات < آخري_پڇاڙي)) {
        // Range is not in order and is invalid, skip it.
        استمر;
      }
      آخري_پڇاڙي = پڇاڙي;


      hb_codepoint_t gid = هذا->groups.arrayZ[i].glyphID;
      إذا (!gid)
      {
        إذا (T::format_عدد == 13) استمر;
	شروعات++;
	gid++;
      }
      إذا (unlikely ((بنانشان صحيح) gid >= انگ_glyphs)) استمر;
      إذا (unlikely ((بنانشان صحيح) (gid + پڇاڙي - شروعات) >= انگ_glyphs))
	پڇاڙي = شروعات + (hb_codepoint_t) انگ_glyphs - gid;

      mapping->alloc (mapping->get_population () + پڇاڙي - شروعات + 1);

      unicodes->جوڙيو_range (شروعات, پڇاڙي);
      لكل (بنانشان cp = شروعات; cp <= پڇاڙي; cp++)
      {
	mapping->سيٽ (cp, gid);
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
  HBUINT32	ڊيگهائي;		/* Byte length of this subtable. */
  HBUINT32	language;	/* Ignore. */
  Sorted_Array32_جو<Cmap_Subtable_Long_گروهه>
		groups;		/* Groupings. */
  عام:
  DEFINE_سائيز_ARRAY (16, groups);
};

بنية CmapSubtableFormat12 : CmapSubtableLongSegmented<CmapSubtableFormat12>
{
  ساكن constexpr صحيح increment = 1;
  ساكن constexpr صحيح format_عدد = 12;

  ساكن hb_codepoint_t گروهه_get_glyph (ثابت Cmap_Subtable_Long_گروهه &گروهه,
					 hb_codepoint_t u)
  { أرجع likely (گروهه.شروعات_Char_ڪوڊ <= گروهه.پڇاڙي_Char_ڪوڊ) ?
	   گروهه.glyphID + (u - گروهه.شروعات_Char_ڪوڊ) : 0; }


  سانچو<قسم_نالو Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  فراغ serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    إذا (!it) أرجع;
    بنانشان جدول_initpos = c->ڊيگهائي ();
    إذا (unlikely (!c->extend_min (هذا))) أرجع;

    hb_codepoint_t شروعات_Char_ڪوڊ = (hb_codepoint_t) -1, پڇاڙي_Char_ڪوڊ = (hb_codepoint_t) -1;
    hb_codepoint_t glyphID = 0;

    لكل (ثابت خودڪار& _ : +it)
    {
      إذا (شروعات_Char_ڪوڊ == (hb_codepoint_t) -1)
      {
	شروعات_Char_ڪوڊ = _.پهريون;
	پڇاڙي_Char_ڪوڊ = _.پهريون;
	glyphID = _.second;
      }
      وإلا إذا (!_is_gid_consecutive (پڇاڙي_Char_ڪوڊ, شروعات_Char_ڪوڊ, glyphID, _.پهريون, _.second))
      {
	Cmap_Subtable_Long_گروهه  grouprecord;
	grouprecord.شروعات_Char_ڪوڊ = شروعات_Char_ڪوڊ;
	grouprecord.پڇاڙي_Char_ڪوڊ = پڇاڙي_Char_ڪوڊ;
	grouprecord.glyphID = glyphID;
	c->copy<Cmap_Subtable_Long_گروهه> (grouprecord);

	شروعات_Char_ڪوڊ = _.پهريون;
	پڇاڙي_Char_ڪوڊ = _.پهريون;
	glyphID = _.second;
      }
      وإلا
	پڇاڙي_Char_ڪوڊ = _.پهريون;
    }

    Cmap_Subtable_Long_گروهه رڪارڊ;
    رڪارڊ.شروعات_Char_ڪوڊ = شروعات_Char_ڪوڊ;
    رڪارڊ.پڇاڙي_Char_ڪوڊ = پڇاڙي_Char_ڪوڊ;
    رڪارڊ.glyphID = glyphID;
    c->copy<Cmap_Subtable_Long_گروهه> (رڪارڊ);

    هذا->format = 12;
    هذا->reserved = 0;
    هذا->ڊيگهائي = c->ڊيگهائي () - جدول_initpos;
    هذا->groups.len = (هذا->ڊيگهائي - min_سائيز) / Cmap_Subtable_Long_گروهه::static_سائيز;
  }

  ساكن size_t get_sub_جدول_سائيز (ثابت hb_sorted_ويڪٽر_t<Cmap_Subtable_Long_گروهه> &groups_معلومات)
  { أرجع 16 + 12 * groups_معلومات.ڊيگهائي; }

  خاص:
  ساكن منطقي _is_gid_consecutive (hb_codepoint_t پڇاڙي_Char_ڪوڊ,
				   hb_codepoint_t شروعات_Char_ڪوڊ,
				   hb_codepoint_t glyphID,
				   hb_codepoint_t cp,
				   hb_codepoint_t new_gid)
  {
    أرجع (cp - 1 == پڇاڙي_Char_ڪوڊ) &&
	new_gid == glyphID + (cp - شروعات_Char_ڪوڊ);
  }

};

بنية CmapSubtableFormat13 : CmapSubtableLongSegmented<CmapSubtableFormat13>
{
  ساكن constexpr صحيح increment = 0;
  ساكن constexpr صحيح format_عدد = 13;

  ساكن hb_codepoint_t گروهه_get_glyph (ثابت Cmap_Subtable_Long_گروهه &گروهه,
					 hb_codepoint_t u HB_اڻ_استعمال_ٿيل)
  { أرجع گروهه.glyphID; }
};

تعريفنوع تعداد
{
  GLYPH_VARIANT_NOT_مليو = 0,
  GLYPH_VARIANT_مليو = 1,
  GLYPH_VARIANT_USE_DEFAULT = 2
} glyph_variant_t;

بنية Unicode_قدر_Range
{
  صحيح cmp (ثابت hb_codepoint_t &codepoint) ثابت
  {
    إذا (codepoint < شروعات_Unicode_قدر) أرجع -1;
    إذا (codepoint > شروعات_Unicode_قدر + additional_ڳڻپ) أرجع +1;
    أرجع 0;
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  HBUINT24	شروعات_Unicode_قدر;	/* First value in this range. */
  HBUINT8	additional_ڳڻپ;	/* Number of additional values in this
					 * range. */
  عام:
  DEFINE_سائيز_STATIC (4);
};

بنية DefaultUVS : Sorted_Array32_جو<Unicode_قدر_Range>
{
  فراغ collect_unicodes (hb_سيٽ_t *ٻاهريون) ثابت
  {
    بنانشان صحيح ڳڻپ = len;
    لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
    {
      hb_codepoint_t پهريون = arrayZ[i].شروعات_Unicode_قدر;
      hb_codepoint_t آخري = hb_min ((hb_codepoint_t) (پهريون + arrayZ[i].additional_ڳڻپ),
				    (hb_codepoint_t) HB_UNICODE_MAX);
      ٻاهريون->جوڙيو_range (پهريون, آخري);
    }
  }

  DefaultUVS* copy (hb_serialize_context_t *c,
		    ثابت hb_سيٽ_t *unicodes) ثابت
  {
    خودڪار *ٻاهريون = c->شروعات_embed<DefaultUVS> ();
    خودڪار snap = c->snapshot ();

    HBUINT32 len;
    len = 0;
    إذا (unlikely (!c->copy<HBUINT32> (len))) أرجع خالي_اشارو;
    بنانشان init_len = c->ڊيگهائي ();

    إذا (هذا->len > unicodes->get_population () * hb_ٻٽو_انگ_storage ((بنانشان) هذا->len))
    {
      hb_codepoint_t شروعات = HB_سيٽ_قدر_INVALID;
      hb_codepoint_t پڇاڙي = HB_سيٽ_قدر_INVALID;

      لكل (خودڪار u : *unicodes)
      {
        إذا (!as_array ().bsearch (u))
	  استمر;
	إذا (شروعات == HB_سيٽ_قدر_INVALID)
	{
	  شروعات = u;
	  پڇاڙي = شروعات - 1;
	}
	إذا (پڇاڙي + 1 != u || پڇاڙي - شروعات == 255)
        {
	  Unicode_قدر_Range rec;
	  rec.شروعات_Unicode_قدر = شروعات;
	  rec.additional_ڳڻپ = پڇاڙي - شروعات;
	  c->copy<Unicode_قدر_Range> (rec);
	  شروعات = u;
	}
	پڇاڙي = u;
      }
      إذا (شروعات != HB_سيٽ_قدر_INVALID)
      {
	Unicode_قدر_Range rec;
	rec.شروعات_Unicode_قدر = شروعات;
	rec.additional_ڳڻپ = پڇاڙي - شروعات;
	c->copy<Unicode_قدر_Range> (rec);
      }

    }
    وإلا
    {
      hb_codepoint_t آخري_ڪوڊ = HB_سيٽ_قدر_INVALID;
      صحيح ڳڻپ = -1;

      لكل (ثابت Unicode_قدر_Range& _ : *هذا)
      {
	hb_codepoint_t cur_داخلا = (hb_codepoint_t) (_.شروعات_Unicode_قدر - 1);
	hb_codepoint_t پڇاڙي = cur_داخلا + _.additional_ڳڻپ + 2;

	لكل (; unicodes->next (&cur_داخلا) && cur_داخلا < پڇاڙي;)
	{
	  ڳڻپ += 1;
	  إذا (آخري_ڪوڊ == HB_سيٽ_قدر_INVALID)
	    آخري_ڪوڊ = cur_داخلا;
	  وإلا إذا (آخري_ڪوڊ + ڳڻپ != cur_داخلا || ڳڻپ > 255)
	  {
	    Unicode_قدر_Range rec;
	    rec.شروعات_Unicode_قدر = آخري_ڪوڊ;
	    rec.additional_ڳڻپ = ڳڻپ - 1;
	    c->copy<Unicode_قدر_Range> (rec);

	    آخري_ڪوڊ = cur_داخلا;
	    ڳڻپ = 0;
	  }
	}
      }

      إذا (آخري_ڪوڊ != HB_MAP_قدر_INVALID)
      {
	Unicode_قدر_Range rec;
	rec.شروعات_Unicode_قدر = آخري_ڪوڊ;
	rec.additional_ڳڻپ = ڳڻپ;
	c->copy<Unicode_قدر_Range> (rec);
      }
    }

    إذا (c->ڊيگهائي () - init_len == 0)
    {
      c->revert (snap);
      أرجع خالي_اشارو;
    }
    وإلا
    {
      إذا (unlikely (!c->check_سونپيو (ٻاهريون->len,
                                      (c->ڊيگهائي () - init_len) / Unicode_قدر_Range::static_سائيز,
                                      HB_SERIALIZE_ERROR_INT_حد_کان_مٿي))) أرجع خالي_اشارو;
      أرجع ٻاهريون;
    }
  }

  عام:
  DEFINE_سائيز_ARRAY (4, *هذا);
};

سانچو <قسم_نالو Types>
بنية UVSMapping
{
  استعمال GlyphID = قسم_نالو Types::HBGlyphID;

  صحيح cmp (ثابت hb_codepoint_t &codepoint) ثابت
  { أرجع unicode_قدر.cmp (codepoint); }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  HBUINT24	unicode_قدر;	/* Base Unicode value of the UVS */
  GlyphID
		glyphID;	/* Glyph ID of the UVS. */
  عام:
  DEFINE_سائيز_STATIC (3 + GlyphID::static_سائيز);
};

سانچو <قسم_نالو Types>
بنية NonDefaultUVS : Sorted_Array32_جو<UVSMapping<Types>>
{
  استعمال Mapping = UVSMapping<Types>;

  فراغ collect_unicodes (hb_سيٽ_t *ٻاهريون) ثابت
  {
    لكل (ثابت خودڪار& a : هذا->as_array ())
      ٻاهريون->جوڙيو (a.unicode_قدر);
  }

  فراغ collect_mapping (hb_سيٽ_t *unicodes, /*ٻاهريون*/
			hb_map_t *mapping /*ٻاهريون*/) ثابت
  {
    لكل (ثابت خودڪار& a : هذا->as_array ())
    {
      hb_codepoint_t unicode = a.unicode_قدر;
      hb_codepoint_t glyphid = a.glyphID;
      unicodes->جوڙيو (unicode);
      mapping->سيٽ (unicode, glyphid);
    }
  }

  فراغ گهيرو_glyphs (ثابت hb_سيٽ_t      *unicodes,
		       hb_سيٽ_t            *glyphset) ثابت
  {
    + هذا->as_array ()
    | hb_filter (unicodes, &Mapping::unicode_قدر)
    | hb_map (&Mapping::glyphID)
    | hb_sink (glyphset)
    ;
  }

  NonDefaultUVS* copy (hb_serialize_context_t *c,
		       ثابت hb_سيٽ_t *unicodes,
		       ثابت hb_سيٽ_t *glyphs_requested,
		       ثابت hb_map_t *glyph_map) ثابت
  {
    خودڪار *ٻاهريون = c->شروعات_embed<NonDefaultUVS> ();
    خودڪار it =
    + هذا->as_array ()
    | hb_filter ([&] (ثابت Mapping& _)
		 {
		   أرجع unicodes->has (_.unicode_قدر) || glyphs_requested->has (_.glyphID);
		 })
    ;

    إذا (!it) أرجع خالي_اشارو;

    HBUINT32 len;
    len = it.len ();
    إذا (unlikely (!c->copy<HBUINT32> (len))) أرجع خالي_اشارو;

    لكل (ثابت Mapping& _ : it)
    {
      Mapping mapping;
      mapping.unicode_قدر = _.unicode_قدر;
      mapping.glyphID = glyph_map->get (_.glyphID);
      c->copy<Mapping> (mapping);
    }

    أرجع ٻاهريون;
  }

  عام:
  DEFINE_سائيز_ARRAY (4, *هذا);
};

سانچو <قسم_نالو Types>
بنية Variation_Selector_رڪارڊ
{
  glyph_variant_t get_glyph (hb_codepoint_t codepoint,
			     hb_codepoint_t *glyph,
			     ثابت فراغ *بنياد) ثابت
  {
    إذا ((بنياد+defaultUVS).bfind (codepoint))
      أرجع GLYPH_VARIANT_USE_DEFAULT;
    ثابت UVSMapping<Types> &nonDefault = (بنياد+nonDefaultUVS).bsearch (codepoint);
    إذا (nonDefault.glyphID)
    {
      *glyph = nonDefault.glyphID;
       أرجع GLYPH_VARIANT_مليو;
    }
    أرجع GLYPH_VARIANT_NOT_مليو;
  }

  Variation_Selector_رڪارڊ(ثابت Variation_Selector_رڪارڊ& other)
  {
    *هذا = other;
  }

  فراغ عامل= (ثابت Variation_Selector_رڪارڊ& other)
  {
    varSelector = other.varSelector;
    HBUINT32 وٿي = other.defaultUVS;
    defaultUVS = وٿي;
    وٿي = other.nonDefaultUVS;
    nonDefaultUVS = وٿي;
  }

  فراغ collect_unicodes (hb_سيٽ_t *ٻاهريون, ثابت فراغ *بنياد) ثابت
  {
    (بنياد+defaultUVS).collect_unicodes (ٻاهريون);
    (بنياد+nonDefaultUVS).collect_unicodes (ٻاهريون);
  }

  فراغ collect_mapping (ثابت فراغ *بنياد,
			hb_سيٽ_t *unicodes, /*ٻاهريون*/
			hb_map_t *mapping /*ٻاهريون*/) ثابت
  {
    (بنياد+defaultUVS).collect_unicodes (unicodes);
    (بنياد+nonDefaultUVS).collect_mapping (unicodes, mapping);
  }

  صحيح cmp (ثابت hb_codepoint_t &variation_selector) ثابت
  { أرجع varSelector.cmp (variation_selector); }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *بنياد) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  defaultUVS.sanitize (c, بنياد) &&
		  nonDefaultUVS.sanitize (c, بنياد));
  }

  hb_pair_t<بنانشان, بنانشان>
  copy (hb_serialize_context_t *c,
	ثابت hb_سيٽ_t *unicodes,
	ثابت hb_سيٽ_t *glyphs_requested,
	ثابت hb_map_t *glyph_map,
	ثابت فراغ *بنياد) ثابت
  {
    خودڪار snap = c->snapshot ();
    خودڪار *ٻاهريون = c->embed<Variation_Selector_رڪارڊ> (*هذا);
    إذا (unlikely (!ٻاهريون)) أرجع hb_pair (0, 0);

    ٻاهريون->defaultUVS = 0;
    ٻاهريون->nonDefaultUVS = 0;

    بنانشان non_default_uvs_objidx = 0;
    إذا (nonDefaultUVS != 0)
    {
      c->push ();
      إذا (c->copy (بنياد+nonDefaultUVS, unicodes, glyphs_requested, glyph_map))
	non_default_uvs_objidx = c->ڪڍو_pack ();
      وإلا c->ڪڍو_discard ();
    }

    بنانشان default_uvs_objidx = 0;
    إذا (defaultUVS != 0)
    {
      c->push ();
      إذا (c->copy (بنياد+defaultUVS, unicodes))
	default_uvs_objidx = c->ڪڍو_pack ();
      وإلا c->ڪڍو_discard ();
    }


    إذا (!default_uvs_objidx && !non_default_uvs_objidx)
      c->revert (snap);

    أرجع hb_pair (default_uvs_objidx, non_default_uvs_objidx);
  }

  HBUINT24	varSelector;	/* Variation selector. */
  Offset32_منزل<DefaultUVS>
		defaultUVS;	/* Offset to Default UVS Table.  May be 0. */
  Offset32_منزل<NonDefaultUVS<Types>>
		nonDefaultUVS;	/* Offset to Non-Default UVS Table.  May be 0. */
  عام:
  DEFINE_سائيز_STATIC (11);
};

سانچو <قسم_نالو Types>
بنية CmapSubtableFormat14_15
{
  ساكن constexpr بنانشان format_قدر = Types::سائيز == 2 ? 14 : 15;
  استعمال رڪارڊ_2 = Variation_Selector_رڪارڊ<Types>;
  استعمال NonDefault = NonDefaultUVS<Types>;

  glyph_variant_t get_glyph_variant (hb_codepoint_t codepoint,
				     hb_codepoint_t variation_selector,
				     hb_codepoint_t *glyph) ثابت
  { أرجع رڪارڊ.bsearch (variation_selector).get_glyph (codepoint, glyph, هذا); }

  فراغ collect_variation_selectors (hb_سيٽ_t *ٻاهريون) ثابت
  {
    لكل (ثابت خودڪار& a : رڪارڊ.as_array ())
      ٻاهريون->جوڙيو (a.varSelector);
  }
  فراغ collect_variation_unicodes (hb_codepoint_t variation_selector,
				   hb_سيٽ_t *ٻاهريون) ثابت
  { رڪارڊ.bsearch (variation_selector).collect_unicodes (ٻاهريون, هذا); }

  فراغ serialize (hb_serialize_context_t *c,
		  ثابت hb_سيٽ_t *unicodes,
		  ثابت hb_سيٽ_t *glyphs_requested,
		  ثابت hb_map_t *glyph_map,
		  ثابت فراغ *بنياد)
  {
    خودڪار snap = c->snapshot ();
    بنانشان جدول_initpos = c->ڊيگهائي ();
    ثابت حرف* init_پڇ = c->پڇ;

    إذا (unlikely (!c->extend_min (هذا))) أرجع;
    هذا->format = format_قدر;

    خودڪار src_tbl = reinterpret_cast<ثابت CmapSubtableFormat14_15*> (بنياد);

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
    hb_ويڪٽر_t<hb_pair_t<بنانشان, بنانشان>> obj_indices;
    لكل (صحيح i = src_tbl->رڪارڊ.len - 1; i >= 0; i--)
    {
      إذا (!unicodes->has(src_tbl->رڪارڊ[i].varSelector))
        استمر;

      hb_pair_t<بنانشان, بنانشان> نتيجو = src_tbl->رڪارڊ[i].copy (c, unicodes, glyphs_requested, glyph_map, بنياد);
      إذا (نتيجو.پهريون || نتيجو.second)
	obj_indices.push (نتيجو);
    }

    إذا (c->ڊيگهائي () - جدول_initpos == CmapSubtableFormat14_15::min_سائيز)
    {
      c->revert (snap);
      أرجع;
    }

    إذا (unlikely (!c->check_success (!obj_indices.اندريون_error ())))
      أرجع;

    صحيح پڇ_len = init_پڇ - c->پڇ;
    c->check_سونپيو (هذا->ڊيگهائي, c->ڊيگهائي () - جدول_initpos + پڇ_len,
                     HB_SERIALIZE_ERROR_INT_حد_کان_مٿي);
    c->check_سونپيو (هذا->رڪارڊ.len,
		     (c->ڊيگهائي () - جدول_initpos - CmapSubtableFormat14_15::min_سائيز) /
		     رڪارڊ_2::static_سائيز,
                     HB_SERIALIZE_ERROR_INT_حد_کان_مٿي);

    /* Correct the incorrect write order by reversing the order of the variation
       records array. */
    ابتو_variation_رڪارڊون ();

    /* Now that records are in the right order, we can set up the offsets. */
    جوڙيو_links_منزل_variation_رڪارڊون (c, obj_indices);
  }

  فراغ ابتو_variation_رڪارڊون ()
  {
    رڪارڊ.as_array ().reverse ();
  }

  فراغ جوڙيو_links_منزل_variation_رڪارڊون (hb_serialize_context_t *c,
					ثابت hb_ويڪٽر_t<hb_pair_t<بنانشان, بنانشان>>& obj_indices)
  {
    لكل (بنانشان i = 0; i < obj_indices.ڊيگهائي; i++)
    {
      /*
       * Since the record array has been reversed (see comments in copy())
       * but obj_indices has not been, the indices at obj_indices[i]
       * are for the variation record at record[j].
       */
      صحيح j = obj_indices.ڊيگهائي - 1 - i;
      c->جوڙيو_link (رڪارڊ[j].defaultUVS, obj_indices[i].پهريون);
      c->جوڙيو_link (رڪارڊ[j].nonDefaultUVS, obj_indices[i].second);
    }
  }

  فراغ گهيرو_glyphs (ثابت hb_سيٽ_t      *unicodes,
		       hb_سيٽ_t            *glyphset) ثابت
  {
    + hb_iter (رڪارڊ)
    | hb_filter (hb_bool, &رڪارڊ_2::nonDefaultUVS)
    | hb_filter (unicodes, &رڪارڊ_2::varSelector)
    | hb_map (&رڪارڊ_2::nonDefaultUVS)
    | hb_map (hb_جوڙيو (هذا))
    | hb_apply ([=] (ثابت NonDefault& _) { _.گهيرو_glyphs (unicodes, glyphset); })
    ;
  }

  فراغ collect_unicodes (hb_سيٽ_t *ٻاهريون) ثابت
  {
    لكل (ثابت رڪارڊ_2& _ : رڪارڊ)
      _.collect_unicodes (ٻاهريون, هذا);
  }

  فراغ collect_mapping (hb_سيٽ_t *unicodes, /*ٻاهريون*/
			hb_map_t *mapping /*ٻاهريون*/) ثابت
  {
    لكل (ثابت رڪارڊ_2& _ : رڪارڊ)
      _.collect_mapping (هذا, unicodes, mapping);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  رڪارڊ.sanitize (c, هذا));
  }

  محمي:
  HBUINT16	format;		/* Format number is set to 14 or 15. */
  HBUINT32	ڊيگهائي;		/* Byte length of this subtable. */
  Sorted_Array32_جو<رڪارڊ_2>
		رڪارڊ;		/* Variation selector records; sorted
				 * in increasing order of `varSelector'. */
  عام:
  DEFINE_سائيز_ARRAY (10, رڪارڊ);
};

بنية CmapSubtable
{
  /* Note: We intentionally do NOT implement subtable formats 2 and 8. */

  منطقي is_variation_selector () ثابت
  {
    بدل (u.format.v) {
    حالة 14:
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    حالة 15:
#جيڪڏهن_پورو
      أرجع سچ;
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
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    حالة 15: hb_barrier (); أرجع u.format15.get_glyph_variant (codepoint, variation_selector, glyph);
#جيڪڏهن_پورو
    افتراضي: أرجع GLYPH_VARIANT_NOT_مليو;
    }
  }

  فراغ collect_variation_selectors (hb_سيٽ_t *ٻاهريون) ثابت
  {
    بدل (u.format.v) {
    حالة 14: hb_barrier (); u.format14.collect_variation_selectors (ٻاهريون); أرجع;
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    حالة 15: hb_barrier (); u.format15.collect_variation_selectors (ٻاهريون); أرجع;
#جيڪڏهن_پورو
    افتراضي: أرجع;
    }
  }

  فراغ collect_variation_unicodes (hb_codepoint_t variation_selector,
				    hb_سيٽ_t *ٻاهريون) ثابت
  {
    بدل (u.format.v) {
    حالة 14: hb_barrier (); u.format14.collect_variation_unicodes (variation_selector, ٻاهريون); أرجع;
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    حالة 15: hb_barrier (); u.format15.collect_variation_unicodes (variation_selector, ٻاهريون); أرجع;
#جيڪڏهن_پورو
    افتراضي: أرجع;
    }
  }

  فراغ گهيرو_glyphs (ثابت hb_سيٽ_t *unicodes, hb_سيٽ_t *glyphset) ثابت
  {
    بدل (u.format.v) {
    حالة 14: hb_barrier (); u.format14.گهيرو_glyphs (unicodes, glyphset); أرجع;
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    حالة 15: hb_barrier (); u.format15.گهيرو_glyphs (unicodes, glyphset); أرجع;
#جيڪڏهن_پورو
    افتراضي: أرجع;
    }
  }

  منطقي get_glyph (hb_codepoint_t codepoint,
		  hb_codepoint_t *glyph,
		  بنانشان صحيح subtable_معلومات_سائيز = 0) ثابت
  {
    بدل (u.format.v) {
    حالة  0: hb_barrier (); أرجع u.format0 .get_glyph (codepoint, glyph);
    حالة  4: hb_barrier (); أرجع subtable_معلومات_سائيز
				      ? u.format4.get_glyph (codepoint, glyph, subtable_معلومات_سائيز)
				      : خطأ;
    حالة  6: hb_barrier (); أرجع u.format6 .get_glyph (codepoint, glyph);
    حالة 10: hb_barrier (); أرجع u.format10.get_glyph (codepoint, glyph);
    حالة 12: hb_barrier (); أرجع u.format12.get_glyph (codepoint, glyph);
    حالة 13: hb_barrier (); أرجع u.format13.get_glyph (codepoint, glyph);
    حالة 14:
    افتراضي: أرجع خطأ;
    }
  }
  فراغ collect_unicodes (hb_سيٽ_t *ٻاهريون,
			 بنانشان صحيح انگ_glyphs = UINT_MAX,
			 بنانشان صحيح subtable_معلومات_سائيز = 0) ثابت
  {
    بدل (u.format.v) {
    حالة  0: hb_barrier (); u.format0 .collect_unicodes (ٻاهريون); أرجع;
    حالة  4: hb_barrier (); إذا (subtable_معلومات_سائيز)
			      u.format4.collect_unicodes (ٻاهريون, subtable_معلومات_سائيز);
			    وإلا
			      u.format4.collect_unicodes (ٻاهريون);
			    أرجع;
    حالة  6: hb_barrier (); u.format6 .collect_unicodes (ٻاهريون); أرجع;
    حالة 10: hb_barrier (); u.format10.collect_unicodes (ٻاهريون); أرجع;
    حالة 12: hb_barrier (); u.format12.collect_unicodes (ٻاهريون, انگ_glyphs); أرجع;
    حالة 13: hb_barrier (); u.format13.collect_unicodes (ٻاهريون, انگ_glyphs); أرجع;
    حالة 14:
    افتراضي: أرجع;
    }
  }

  فراغ collect_mapping (hb_سيٽ_t *unicodes, /*ٻاهريون*/
			hb_map_t *mapping, /*ٻاهريون*/
			بنانشان انگ_glyphs = UINT_MAX,
			بنانشان صحيح subtable_معلومات_سائيز = 0) ثابت
  {
    بدل (u.format.v) {
    حالة  0: hb_barrier (); u.format0 .collect_mapping (unicodes, mapping); أرجع;
    حالة  4: hb_barrier (); إذا (subtable_معلومات_سائيز)
			      u.format4.collect_mapping (unicodes, mapping, subtable_معلومات_سائيز);
			    وإلا
			      u.format4.collect_mapping (unicodes, mapping);
			    أرجع;
    حالة  6: hb_barrier (); u.format6 .collect_mapping (unicodes, mapping); أرجع;
    حالة 10: hb_barrier (); u.format10.collect_mapping (unicodes, mapping); أرجع;
    حالة 12: hb_barrier (); u.format12.collect_mapping (unicodes, mapping, انگ_glyphs); أرجع;
    حالة 13: hb_barrier (); u.format13.collect_mapping (unicodes, mapping, انگ_glyphs); أرجع;
    حالة 14:
    افتراضي: أرجع;
    }
  }

  بنانشان get_language () ثابت
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

  سانچو<قسم_نالو Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  فراغ serialize (hb_serialize_context_t *c,
		  Iterator it,
		  بنانشان format,
		  ثابت hb_subset_plan_t *plan,
		  ثابت فراغ *بنياد)
  {
    بدل (format) {
    حالة  4: hb_barrier (); أرجع u.format4.serialize (c, it);
    حالة 12: hb_barrier (); أرجع u.format12.serialize (c, it);
    حالة 14: hb_barrier (); أرجع u.format14.serialize (c, &plan->unicodes, &plan->glyphs_requested, plan->glyph_map, بنياد);
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    حالة 15: hb_barrier (); أرجع u.format15.serialize (c, &plan->unicodes, &plan->glyphs_requested, plan->glyph_map, بنياد);
#جيڪڏهن_پورو
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
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    حالة 15: hb_barrier (); return_trace (u.format15.sanitize (c));
#جيڪڏهن_پورو
    افتراضي:return_trace (سچ);
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
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
  CmapSubtableFormat14_15<Layout::MediumTypes>
			format15;
#جيڪڏهن_پورو
  } u;
  عام:
  DEFINE_سائيز_UNION (2, format.v);
};


بنية Encoding_رڪارڊ
{
  صحيح cmp (ثابت Encoding_رڪارڊ &other) ثابت
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

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *بنياد) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  subtable.sanitize (c, بنياد));
  }

  سانچو<قسم_نالو Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  Encoding_رڪارڊ* copy (hb_serialize_context_t *c,
			Iterator it,
			بنانشان format,
			ثابت فراغ *بنياد,
			ثابت hb_subset_plan_t *plan,
			/* INOUT */ بنانشان *objidx) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خودڪار snap = c->snapshot ();
    خودڪار *ٻاهريون = c->embed (هذا);
    إذا (unlikely (!ٻاهريون)) return_trace (خالي_اشارو);
    ٻاهريون->subtable = 0;

    إذا (*objidx == 0)
    {
      CmapSubtable *cmapsubtable = c->push<CmapSubtable> ();
      بنانشان origin_ڊيگهائي = c->ڊيگهائي ();
      cmapsubtable->serialize (c, it, format, plan, &(بنياد+subtable));
      إذا (c->ڊيگهائي () > origin_ڊيگهائي && !c->اندريون_error()) *objidx = c->ڪڍو_pack ();
      وإلا c->ڪڍو_discard ();
    }

    إذا (*objidx == 0)
    {
      c->revert (snap);
      return_trace (خالي_اشارو);
    }

    c->جوڙيو_link (ٻاهريون->subtable, *objidx);
    return_trace (ٻاهريون);
  }

  HBUINT16	platformID;	/* Platform ID. */
  HBUINT16	encodingID;	/* Platform-specific encoding ID. */
  Offset32_منزل<CmapSubtable>
		subtable;	/* Byte offset from beginning of table to the subtable for this encoding. */
  عام:
  DEFINE_سائيز_STATIC (8);
};

بنية cmap;

بنية SubtableUnicodesCache {

 خاص:
  hb_blob_ptr_t<cmap> بنياد_blob;
  ثابت حرف* بنياد;
  بنانشان صحيح جدول_ڊيگهائي;
  hb_hashmap_t<بنانشان, hb::unique_ptr<hb_سيٽ_t>> cached_unicodes;

 عام:

  ساكن SubtableUnicodesCache* create (hb_blob_ptr_t<cmap> ذريعو_جدول)
  {
    SubtableUnicodesCache* cache =
        (SubtableUnicodesCache*) hb_malloc (حجم(SubtableUnicodesCache));

    إذا (unlikely (!cache))
      أرجع خالي_اشارو;

    جديد (cache) SubtableUnicodesCache (ذريعو_جدول);
    أرجع cache;
  }

  ساكن فراغ destroy (فراغ* value) {
    إذا (!value) أرجع;

    SubtableUnicodesCache* cache = (SubtableUnicodesCache*) value;
    cache->~SubtableUnicodesCache ();
    hb_free (cache);
  }

  SubtableUnicodesCache(ثابت فراغ* cmap_بنياد,
			بنانشان صحيح جدول_ڊيگهائي_2 = 0)
      : بنياد_blob(),
        بنياد ((ثابت حرف*) cmap_بنياد),
        جدول_ڊيگهائي (جدول_ڊيگهائي_2),
        cached_unicodes ()
  {}

  SubtableUnicodesCache(hb_blob_ptr_t<cmap> بنياد_blob_2)
      : بنياد_blob(بنياد_blob_2),
        بنياد ((ثابت حرف *) بنياد_blob.get()),
        جدول_ڊيگهائي (بنياد_blob.get_ڊيگهائي ()),
        cached_unicodes ()
  {}

  ~SubtableUnicodesCache()
  {
    بنياد_blob.destroy ();
  }

  منطقي same_بنياد(ثابت فراغ* other) ثابت
  {
    أرجع other == (ثابت فراغ*) بنياد;
  }

  ثابت hb_سيٽ_t* سيٽ_for (ثابت Encoding_رڪارڊ* رڪارڊ,
                           SubtableUnicodesCache& mutable_cache) ثابت
  {
    إذا (cached_unicodes.has ((بنانشان) ((ثابت حرف *) رڪارڊ - بنياد)))
      أرجع cached_unicodes.get ((بنانشان) ((ثابت حرف *) رڪارڊ - بنياد));

    أرجع mutable_cache.سيٽ_for (رڪارڊ);
  }

  ثابت hb_سيٽ_t* سيٽ_for (ثابت Encoding_رڪارڊ* رڪارڊ)
  {
    إذا (!cached_unicodes.has ((بنانشان) ((ثابت حرف *) رڪارڊ - بنياد)))
    {
      hb_سيٽ_t *s = hb_سيٽ_create ();
      إذا (unlikely (s->اندريون_error ()))
	أرجع hb_سيٽ_get_خالي_جايون ();

      بنانشان صحيح subtable_معلومات_سائيز = رڪارڊ->subtable < جدول_ڊيگهائي
					? جدول_ڊيگهائي - (بنانشان صحيح) رڪارڊ->subtable
					: 0;
      (بنياد+رڪارڊ->subtable).collect_unicodes (s, UINT_MAX, subtable_معلومات_سائيز);

      إذا (unlikely (!cached_unicodes.سيٽ ((بنانشان) ((ثابت حرف *) رڪارڊ - بنياد), hb::unique_ptr<hb_سيٽ_t> {s})))
        أرجع hb_سيٽ_get_خالي_جايون ();

      أرجع s;
    }
    أرجع cached_unicodes.get ((بنانشان) ((ثابت حرف *) رڪارڊ - بنياد));
  }

};

ساكن مضمن uint16_t
hb_نشان_pua_map (بنانشان codepoint)
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
  ساكن constexpr hb_ٽيگ_t جدول_ٽيگ = HB_OT_ٽيگ_cmap;


  ساكن SubtableUnicodesCache* create_filled_cache(hb_blob_ptr_t<cmap> ذريعو_جدول) {
    ثابت cmap* cmap = ذريعو_جدول.get();
    خودڪار it =
    + hb_iter (cmap->encoding_رڪارڊ)
    | hb_filter ([&](ثابت Encoding_رڪارڊ& _) {
      أرجع cmap::filter_encoding_رڪارڊون_for_subset (cmap, _);
    })
    ;

    SubtableUnicodesCache* cache = SubtableUnicodesCache::create(ذريعو_جدول);

    إذا (unlikely (!cache))
      أرجع خالي_اشارو;

    لكل (ثابت Encoding_رڪارڊ& _ : it)
      cache->سيٽ_for(&_); // populate the cache for this encoding record.

    أرجع cache;
  }

  سانچو<قسم_نالو Iterator, قسم_نالو EncodingRecIter,
	   hb_requires (hb_is_iterator (EncodingRecIter))>
  منطقي serialize (hb_serialize_context_t *c,
		  Iterator it,
		  EncodingRecIter encodingrec_iter,
		  ثابت فراغ *بنياد,
		  hb_subset_plan_t *plan,
	                  منطقي drop_format_4 = خطأ,
			  بنانشان صحيح ذريعو_جدول_ڊيگهائي = 0)
  {
    إذا (unlikely (!c->extend_min ((*هذا))))  أرجع خطأ;
    هذا->نسخو = 0;

    بنانشان format4objidx = 0, format12objidx = 0, format14objidx = 0;
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    بنانشان format15objidx = 0;
#جيڪڏهن_پورو
    خودڪار snap = c->snapshot ();

    SubtableUnicodesCache local_unicodes_cache (بنياد, ذريعو_جدول_ڊيگهائي);
    ثابت SubtableUnicodesCache* unicodes_cache = &local_unicodes_cache;

    إذا (plan->accelerator &&
        plan->accelerator->cmap_cache &&
        plan->accelerator->cmap_cache->same_بنياد (بنياد))
      unicodes_cache = plan->accelerator->cmap_cache;

    لكل (ثابت Encoding_رڪارڊ& _ : encodingrec_iter)
    {
      إذا (c->اندريون_error ())
        أرجع خطأ;

      بنانشان format = (بنياد+_.subtable).u.format.v;
      إذا (format != 4 && format != 12 && format != 14
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
	  && format != 15
#جيڪڏهن_پورو
	 ) استمر;

      ثابت hb_سيٽ_t* unicodes_سيٽ = unicodes_cache->سيٽ_for (&_, local_unicodes_cache);

      إذا (!drop_format_4 && format == 4)
      {
        c->copy (_, + it | hb_filter (*unicodes_سيٽ, hb_پهريون), 4u, بنياد, plan, &format4objidx);
        إذا (c->اندريون_error () && c->only_حد_کان_مٿي ())
        {
          // cmap4 overflowed, reset and retry serialization without format 4 subtables.
          c->revert (snap);
          أرجع serialize (c, it,
                            encodingrec_iter,
                            بنياد,
                            plan,
                            سچ,
			    ذريعو_جدول_ڊيگهائي);
        }
      }

      وإلا إذا (format == 12)
      {
        إذا (_can_drop (_,
                       *unicodes_سيٽ,
                       بنياد,
                       *unicodes_cache,
                       local_unicodes_cache,
                       + it | hb_map (hb_پهريون), encodingrec_iter))
          استمر;
        c->copy (_, + it | hb_filter (*unicodes_سيٽ, hb_پهريون), 12u, بنياد, plan, &format12objidx);
      }
      وإلا إذا (format == 14) c->copy (_, it, 14u, بنياد, plan, &format14objidx);
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
      وإلا إذا (format == 15) c->copy (_, it, 15u, بنياد, plan, &format15objidx);
#جيڪڏهن_پورو
    }
        بنانشان ڊيگهائي = c->ڊيگهائي ();
        بنانشان موجود = ڊيگهائي > cmap::min_سائيز ? ڊيگهائي - cmap::min_سائيز : 0;
        c->check_سونپيو(هذا->encoding_رڪارڊ.len,
                        موجود / Encoding_رڪارڊ::static_سائيز,
                        HB_SERIALIZE_ERROR_INT_حد_کان_مٿي);

    // Fail if format 4 was dropped and there is no cmap12.
    أرجع !drop_format_4 || format12objidx;
  }

  سانچو<قسم_نالو Iterator, قسم_نالو Encoding_رڪارڊ_Iterator,
      hb_requires (hb_is_iterator (Iterator)),
      hb_requires (hb_is_iterator (Encoding_رڪارڊ_Iterator))>
  منطقي _can_drop (ثابت Encoding_رڪارڊ& cmap12,
                  ثابت hb_سيٽ_t& cmap12_unicodes,
                  ثابت فراغ* بنياد,
                  ثابت SubtableUnicodesCache& unicodes_cache,
                  SubtableUnicodesCache& local_unicodes_cache,
                  Iterator subset_unicodes,
                  Encoding_رڪارڊ_Iterator encoding_رڪارڊون)
  {
    لكل (خودڪار cp : + subset_unicodes | hb_filter (cmap12_unicodes))
    {
      إذا (cp >= 0x10000) أرجع خطأ;
    }

    بنانشان هدف_platform;
    بنانشان هدف_encoding;
    بنانشان هدف_language = (بنياد+cmap12.subtable).get_language ();

    إذا (cmap12.platformID == 0 && cmap12.encodingID == 4)
    {
      هدف_platform = 0;
      هدف_encoding = 3;
    } وإلا إذا (cmap12.platformID == 3 && cmap12.encodingID == 10) {
      هدف_platform = 3;
      هدف_encoding = 1;
    } وإلا {
      أرجع خطأ;
    }

    لكل (ثابت خودڪار& _ : encoding_رڪارڊون)
    {
      إذا (_.platformID != هدف_platform
          || _.encodingID != هدف_encoding
          || (بنياد+_.subtable).get_language() != هدف_language)
        استمر;

      ثابت hb_سيٽ_t* sibling_unicodes = unicodes_cache.سيٽ_for (&_, local_unicodes_cache);

      خودڪار cmap12 = + subset_unicodes | hb_filter (cmap12_unicodes);
      خودڪار sibling = + subset_unicodes | hb_filter (*sibling_unicodes);
      لكل (; cmap12 && sibling; cmap12++, sibling++)
      {
        بنانشان a = *cmap12;
        بنانشان b = *sibling;
        إذا (a != b) أرجع خطأ;
      }

      أرجع !cmap12 && !sibling;
    }

    أرجع خطأ;
  }

  فراغ گهيرو_glyphs (ثابت hb_سيٽ_t      *unicodes,
		       hb_سيٽ_t            *glyphset) ثابت
  {
    + hb_iter (encoding_رڪارڊ)
    | hb_map (&Encoding_رڪارڊ::subtable)
    | hb_map (hb_جوڙيو (هذا))
    | hb_filter (&CmapSubtable::is_variation_selector)
    | hb_apply ([=] (ثابت CmapSubtable& _) { _.گهيرو_glyphs (unicodes, glyphset); })
    ;
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  { أرجع subset (c, خطأ); }

  منطقي subset (hb_subset_context_t *c, منطقي is_dmap) ثابت
  {
    TRACE_SUBSET (هذا);

    cmap *cmap_مفرد = c->serializer->شروعات_embed<cmap> ();

    خودڪار encodingrec_iter =
    + hb_iter (encoding_رڪارڊ)
    | hb_filter ([&](ثابت Encoding_رڪارڊ& _) {
      أرجع cmap::filter_encoding_رڪارڊون_for_subset (هذا, _);
    })
    ;

    إذا (unlikely (!encodingrec_iter.len ())) return_trace (خطأ);

    ثابت Encoding_رڪارڊ *unicode_bmp= خالي_اشارو, *unicode_ucs4 = خالي_اشارو, *ms_bmp = خالي_اشارو, *ms_ucs4 = خالي_اشارو;
    منطقي has_format12 = خطأ;

    لكل (ثابت Encoding_رڪارڊ& _ : encodingrec_iter)
    {
      بنانشان format = (هذا + _.subtable).u.format.v;
      إذا (format == 12) has_format12 = سچ;

      ثابت Encoding_رڪارڊ *جدول = std::addressof (_);
      إذا      (_.platformID == 0 && _.encodingID ==  3) unicode_bmp = جدول;
      وإلا إذا (_.platformID == 0 && _.encodingID ==  4) unicode_ucs4 = جدول;
      وإلا إذا (_.platformID == 3 && _.encodingID ==  1) ms_bmp = جدول;
      وإلا إذا (_.platformID == 3 && _.encodingID == 10) ms_ucs4 = جدول;
    }

    إذا (unlikely (!is_dmap && !has_format12 && !unicode_bmp && !ms_bmp)) return_trace (خطأ);
    إذا (unlikely (!is_dmap && has_format12 && (!unicode_ucs4 && !ms_ucs4))) return_trace (خطأ);

    خودڪار it =
    + c->plan->unicode_منزل_new_gid_list.iter ()
    | hb_filter ([&] (ثابت hb_codepoint_pair_t _)
		 { أرجع (_.second != HB_MAP_قدر_INVALID); })
    ;

    return_trace (cmap_مفرد->serialize (c->serializer,
                                         it,
                                         encodingrec_iter,
                                         هذا,
                                         c->plan,
					 خطأ,
					 c->ذريعو_blob->ڊيگهائي));
  }

  ثابت CmapSubtable *ڳوليو_بهترين_subtable (منطقي *نشان = خالي_اشارو,
					  منطقي *mac = خالي_اشارو,
					  منطقي *macroman = خالي_اشارو) ثابت
  {
    إذا (نشان) *نشان = خطأ;
    إذا (mac) *mac = خطأ;
    إذا (macroman) *macroman = خطأ;

    ثابت CmapSubtable *subtable;

    /* Symbol subtable.
     * Prefer symbol if available.
     * https://github.com/harfbuzz/harfbuzz/issues/1918 */
    إذا ((subtable = هذا->ڳوليو_subtable (3, 0)))
    {
      إذا (نشان) *نشان = سچ;
      أرجع subtable;
    }

    /* 32-bit subtables. */
    إذا ((subtable = هذا->ڳوليو_subtable (3, 10))) أرجع subtable;
    إذا ((subtable = هذا->ڳوليو_subtable (0, 6))) أرجع subtable;
    إذا ((subtable = هذا->ڳوليو_subtable (0, 4))) أرجع subtable;

    /* 16-bit subtables. */
    إذا ((subtable = هذا->ڳوليو_subtable (3, 1))) أرجع subtable;
    إذا ((subtable = هذا->ڳوليو_subtable (0, 3))) أرجع subtable;
    إذا ((subtable = هذا->ڳوليو_subtable (0, 2))) أرجع subtable;
    إذا ((subtable = هذا->ڳوليو_subtable (0, 1))) أرجع subtable;
    إذا ((subtable = هذا->ڳوليو_subtable (0, 0))) أرجع subtable;

    /* MacRoman subtable. */
    إذا ((subtable = هذا->ڳوليو_subtable (1, 0)))
    {
      إذا (mac) *mac = سچ;
      إذا (macroman) *macroman = سچ;
      أرجع subtable;
    }
    /* Any other Mac subtable; we just map ASCII for these. */
    إذا ((subtable = هذا->ڳوليو_subtable (1, 0xFFFF)))
    {
      إذا (mac) *mac = سچ;
      أرجع subtable;
    }

    /* Meh. */
    أرجع &Null (CmapSubtable);
  }

  بنية accelerator_t
  {
    استعمال cache_t = hb_cache_t<21, 19>;
    static_assert (حجم (cache_t) == 1024, "");

    accelerator_t (hb_face_t *face, hb_ٽيگ_t جدول_ٽيگ_2 = cmap::جدول_ٽيگ)
    {
      هذا->جدول = hb_sanitize_context_t ().reference_جدول<cmap> (face, جدول_ٽيگ_2);
      إذا (!هذا->جدول.get_ڊيگهائي ())
	أرجع;

      منطقي نشان, mac, macroman;
      هذا->subtable = جدول->ڳوليو_بهترين_subtable (&نشان, &mac, &macroman);
      هذا->subtable_uvs = &Null (CmapSubtable);
      {
	ثابت CmapSubtable *st = جدول->ڳوليو_variation_selector_subtable ();
	إذا (st)
	  subtable_uvs = st;
      }

#جيڪڏهن_وصف_نه_ٿيل HB_NO_OT_FONT_CMAP_CACHE
      cache = (cache_t *) hb_malloc (حجم (cache_t));
      إذا (cache)
	جديد (cache) cache_t ();
      وإلا
        أرجع; // Such that get_glyph_funcZ remains null.
#جيڪڏهن_پورو

      هذا->get_glyph_معلومات = subtable;
#جيڪڏهن_وصف_نه_ٿيل HB_NO_CMAP_LEGACY_SUBTABLES
      منطقي is_format4 = subtable->u.format.v == 4;
      خودڪار سيٽ_format4_getter = [هذا] (منطقي (*func) (ثابت فراغ *,
						      hb_codepoint_t,
						      hb_codepoint_t *))
      {
	هذا->format4_accel.init (&هذا->subtable->u.format4,
				  get_subtable_معلومات_سائيز (هذا->subtable));
	هذا->get_glyph_معلومات = &هذا->format4_accel;
	هذا->get_glyph_funcZ = func;
      };
      إذا (unlikely (نشان))
      {
	بدل ((بنانشان) face->جدول.OS2->get_font_صفحو ()) {
	حالة OS2::font_صفحو_t::FONT_صفحو_NONE:
	  إذا (is_format4)
	    سيٽ_format4_getter (get_glyph_from_نشان<CmapSubtableFormat4::accelerator_t, hb_نشان_pua_map>);
	  وإلا
	    هذا->get_glyph_funcZ = get_glyph_from_نشان<CmapSubtable, hb_نشان_pua_map>;
	  اكسر;
#جيڪڏهن_وصف_نه_ٿيل HB_NO_OT_SHAPER_ARABIC_FALLBACK
	حالة OS2::font_صفحو_t::FONT_صفحو_SIMP_ARABIC:
	  إذا (is_format4)
	    سيٽ_format4_getter (get_glyph_from_نشان<CmapSubtableFormat4::accelerator_t, _hb_arabic_pua_simp_map>);
	  وإلا
	    هذا->get_glyph_funcZ = get_glyph_from_نشان<CmapSubtable, _hb_arabic_pua_simp_map>;
	  اكسر;
	حالة OS2::font_صفحو_t::FONT_صفحو_TRAD_ARABIC:
	  إذا (is_format4)
	    سيٽ_format4_getter (get_glyph_from_نشان<CmapSubtableFormat4::accelerator_t, _hb_arabic_pua_trad_map>);
	  وإلا
	    هذا->get_glyph_funcZ = get_glyph_from_نشان<CmapSubtable, _hb_arabic_pua_trad_map>;
	  اكسر;
#جيڪڏهن_پورو
	افتراضي:
	  إذا (is_format4)
	    سيٽ_format4_getter (get_glyph_from<CmapSubtableFormat4::accelerator_t>);
	  وإلا
	    هذا->get_glyph_funcZ = get_glyph_from<CmapSubtable>;
	  اكسر;
	}
      }
      وإلا إذا (unlikely (macroman))
      {
	إذا (is_format4)
	  سيٽ_format4_getter (get_glyph_from_macroman<CmapSubtableFormat4::accelerator_t>);
	وإلا
	  هذا->get_glyph_funcZ = get_glyph_from_macroman<CmapSubtable>;
      }
      وإلا إذا (unlikely (mac))
      {
	إذا (is_format4)
	  سيٽ_format4_getter (get_glyph_from_ascii<CmapSubtableFormat4::accelerator_t>);
	وإلا
	  هذا->get_glyph_funcZ = get_glyph_from_ascii<CmapSubtable>;
      }
      وإلا
#جيڪڏهن_پورو
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
				      get_subtable_معلومات_سائيز (subtable));
	    هذا->get_glyph_معلومات = &هذا->format4_accel;
	    هذا->get_glyph_funcZ = هذا->format4_accel.get_glyph_func;
	    اكسر;
	  }
	}
      }
    }
    ~accelerator_t ()
    {
#جيڪڏهن_وصف_نه_ٿيل HB_NO_OT_FONT_CMAP_CACHE
      hb_free (cache);
#جيڪڏهن_پورو
      جدول.destroy ();
    }

    مضمن منطقي _cached_get (hb_codepoint_t unicode,
			     hb_codepoint_t *glyph) ثابت
    {
#جيڪڏهن_وصف_نه_ٿيل HB_NO_OT_FONT_CMAP_CACHE
      // cache is always non-null if we have a get_glyph_funcZ
      بنانشان v;
      إذا (cache->get (unicode, &v))
      {
        *glyph = v;
	أرجع سچ;
      }
#جيڪڏهن_پورو
      منطقي ret  = هذا->get_glyph_funcZ (هذا->get_glyph_معلومات, unicode, glyph);

#جيڪڏهن_وصف_نه_ٿيل HB_NO_OT_FONT_CMAP_CACHE
      إذا (ret)
        cache->سيٽ (unicode, *glyph);
#جيڪڏهن_پورو

      أرجع ret;
    }

    منطقي get_nominal_glyph (hb_codepoint_t  unicode,
			    hb_codepoint_t *glyph) ثابت
    {
      إذا (unlikely (!هذا->get_glyph_funcZ)) أرجع خطأ;
      أرجع _cached_get (unicode, glyph);
    }

    بنانشان صحيح get_nominal_glyphs (بنانشان صحيح ڳڻپ,
				     ثابت hb_codepoint_t *پهريون_unicode,
				     بنانشان صحيح unicode_stride,
				     hb_codepoint_t *پهريون_glyph,
				     بنانشان صحيح glyph_stride) ثابت
    {
      إذا (unlikely (!هذا->get_glyph_funcZ)) أرجع 0;

      بنانشان صحيح پورو;
      لكل (پورو = 0;
	   پورو < ڳڻپ && _cached_get (*پهريون_unicode, پهريون_glyph);
	   پورو++)
      {
	پهريون_unicode = &Struct_At_وٿي_Unaligned<hb_codepoint_t> (پهريون_unicode, unicode_stride);
	پهريون_glyph = &Struct_At_وٿي_Unaligned<hb_codepoint_t> (پهريون_glyph, glyph_stride);
      }
      أرجع پورو;
    }

    منطقي get_variation_glyph (hb_codepoint_t  unicode,
			      hb_codepoint_t  variation_selector,
			      hb_codepoint_t *glyph) ثابت
    {
      بدل (get_glyph_variant (unicode, variation_selector, glyph))
      {
	حالة GLYPH_VARIANT_NOT_مليو:	أرجع خطأ;
	حالة GLYPH_VARIANT_مليو:	أرجع سچ;
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

    منطقي has_معلومات () ثابت { أرجع جدول.get_ڊيگهائي (); }

    فراغ collect_unicodes (hb_سيٽ_t *ٻاهريون, بنانشان صحيح انگ_glyphs) ثابت
    { subtable->collect_unicodes (ٻاهريون, انگ_glyphs, get_subtable_معلومات_سائيز (subtable)); }
    فراغ collect_mapping (hb_سيٽ_t *unicodes, hb_map_t *mapping,
			  بنانشان انگ_glyphs = UINT_MAX) ثابت
    { subtable->collect_mapping (unicodes, mapping, انگ_glyphs, get_subtable_معلومات_سائيز (subtable)); }
    فراغ collect_variation_selectors (hb_سيٽ_t *ٻاهريون) ثابت
    { subtable_uvs->collect_variation_selectors (ٻاهريون); }
    فراغ collect_variation_unicodes (hb_codepoint_t variation_selector,
				     hb_سيٽ_t *ٻاهريون) ثابت
    { subtable_uvs->collect_variation_unicodes (variation_selector, ٻاهريون); }

    محمي:
    تعريفنوع منطقي (*hb_cmap_get_glyph_func_t) (ثابت فراغ *obj,
					      hb_codepoint_t codepoint,
					      hb_codepoint_t *glyph);
    تعريفنوع uint16_t (*hb_pua_remap_func_t) (بنانشان);

    سانچو <قسم_نالو Type>
    HB_اندروني ساكن منطقي get_glyph_from (ثابت فراغ *obj,
					    hb_codepoint_t codepoint,
					    hb_codepoint_t *glyph)
    {
      ثابت Type *typed_obj = (ثابت Type *) obj;
      أرجع typed_obj->get_glyph (codepoint, glyph);
    }

    سانچو <قسم_نالو Type, hb_pua_remap_func_t remap>
    HB_اندروني ساكن منطقي get_glyph_from_نشان (ثابت فراغ *obj,
						   hb_codepoint_t codepoint,
						   hb_codepoint_t *glyph)
    {
      ثابت Type *typed_obj = (ثابت Type *) obj;
      إذا (likely (typed_obj->get_glyph (codepoint, glyph)))
	أرجع سچ;

      إذا (hb_codepoint_t c = remap (codepoint))
	أرجع typed_obj->get_glyph (c, glyph);

      أرجع خطأ;
    }

    سانچو <قسم_نالو Type>
    HB_اندروني ساكن منطقي get_glyph_from_ascii (ثابت فراغ *obj,
						  hb_codepoint_t codepoint,
						  hb_codepoint_t *glyph)
    {
      ثابت Type *typed_obj = (ثابت Type *) obj;
      أرجع codepoint < 0x80 && typed_obj->get_glyph (codepoint, glyph);
    }

    سانچو <قسم_نالو Type>
    HB_اندروني ساكن منطقي get_glyph_from_macroman (ثابت فراغ *obj,
						     hb_codepoint_t codepoint,
						     hb_codepoint_t *glyph)
    {
      إذا (get_glyph_from_ascii<Type> (obj, codepoint, glyph))
	أرجع سچ;

      ثابت Type *typed_obj = (ثابت Type *) obj;
      بنانشان c = unicode_منزل_macroman (codepoint);
      أرجع c && typed_obj->get_glyph (c, glyph);
    }

    بنانشان صحيح get_subtable_معلومات_سائيز (ثابت CmapSubtable *subtable) ثابت
    {
      بنانشان صحيح جدول_ڊيگهائي = هذا->جدول.get_ڊيگهائي ();
      uintptr_t جدول_شروعات = (uintptr_t) (ثابت فراغ *) هذا->جدول.get ();
      uintptr_t subtable_addr = (uintptr_t) (ثابت فراغ *) subtable;
      إذا (unlikely (subtable_addr < جدول_شروعات))
	أرجع 0;

      uintptr_t subtable_وٿي = subtable_addr - جدول_شروعات;
      إذا (unlikely (subtable_وٿي >= جدول_ڊيگهائي))
	أرجع 0;

      أرجع جدول_ڊيگهائي - (بنانشان صحيح) subtable_وٿي;
    }

    خاص:
    hb_nonnull_ptr_t<ثابت CmapSubtable> subtable;
    hb_nonnull_ptr_t<ثابت CmapSubtable> subtable_uvs;

    hb_cmap_get_glyph_func_t get_glyph_funcZ = خالي_اشارو;
    ثابت فراغ *get_glyph_معلومات = خالي_اشارو;

    CmapSubtableFormat4::accelerator_t format4_accel;

#جيڪڏهن_وصف_نه_ٿيل HB_NO_OT_FONT_CMAP_CACHE
    cache_t *cache = خالي_اشارو;
#جيڪڏهن_پورو

    عام:
    hb_blob_ptr_t<cmap> جدول;
  };

  محمي:

  ثابت CmapSubtable *ڳوليو_subtable (بنانشان صحيح platform_id,
				     بنانشان صحيح encoding_id) ثابت
  {
    Encoding_رڪارڊ key;
    key.platformID = platform_id;
    key.encodingID = encoding_id;

    ثابت Encoding_رڪارڊ &نتيجو = encoding_رڪارڊ.bsearch (key);
    إذا (!نتيجو.subtable)
      أرجع خالي_اشارو;

    أرجع &(هذا+نتيجو.subtable);
  }

  ثابت CmapSubtable *ڳوليو_variation_selector_subtable () ثابت
  {
    ثابت CmapSubtable *format14 = خالي_اشارو;
    لكل (ثابت Encoding_رڪارڊ& رڪارڊ : encoding_رڪارڊ)
    {
      إذا (رڪارڊ.platformID != 0 || رڪارڊ.encodingID != 5)
	استمر;

      ثابت CmapSubtable *subtable = &(هذا+رڪارڊ.subtable);
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
      إذا (subtable->u.format.v == 15)
	أرجع subtable;
#جيڪڏهن_پورو
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
		  likely (نسخو == 0) &&
		  encoding_رڪارڊ.sanitize (c, هذا));
  }

 خاص:

  ساكن منطقي filter_encoding_رڪارڊون_for_subset(ثابت cmap* cmap,
                                                 ثابت Encoding_رڪارڊ& _)
  {
    أرجع
        (_.platformID == 0 && _.encodingID == 3) ||
        (_.platformID == 0 && _.encodingID == 4) ||
        (_.platformID == 3 && _.encodingID == 1) ||
        (_.platformID == 3 && _.encodingID == 10) ||
        (cmap + _.subtable).is_variation_selector ();
  }

  محمي:
  HBUINT16	نسخو;	/* Table version number (0). */
  Sorted_Array16_جو<Encoding_رڪارڊ>
		encoding_رڪارڊ;	/* Encoding tables. */
  عام:
  DEFINE_سائيز_ARRAY (4, encoding_رڪارڊ);
};

بنية DMAP : cmap
{
  ساكن constexpr hb_ٽيگ_t جدول_ٽيگ = HB_OT_ٽيگ_DMAP;

  منطقي subset (hb_subset_context_t *c) ثابت
  { أرجع cmap::subset (c, سچ); }
};

بنية cmap_accelerator_t
{
  cmap_accelerator_t (hb_face_t *face) :
    dmap (face, HB_OT_ٽيگ_DMAP),
    بنياد (face)
  {}

  منطقي get_nominal_glyph (hb_codepoint_t  unicode,
			  hb_codepoint_t *glyph) ثابت
  {
    أرجع dmap.get_nominal_glyph (unicode, glyph) ||
	   بنياد.get_nominal_glyph (unicode, glyph);
  }

  بنانشان صحيح get_nominal_glyphs (بنانشان صحيح ڳڻپ,
				   ثابت hb_codepoint_t *پهريون_unicode,
				   بنانشان صحيح unicode_stride,
				   hb_codepoint_t *پهريون_glyph,
				   بنانشان صحيح glyph_stride) ثابت
  {
    إذا (likely (!dmap.has_معلومات ()))
      أرجع بنياد.get_nominal_glyphs (ڳڻپ,
				      پهريون_unicode, unicode_stride,
				      پهريون_glyph, glyph_stride);

    بنانشان صحيح پورو;
    لكل (پورو = 0;
	 پورو < ڳڻپ && get_nominal_glyph (*پهريون_unicode, پهريون_glyph);
	 پورو++)
    {
      پهريون_unicode = &Struct_At_وٿي_Unaligned<hb_codepoint_t> (پهريون_unicode, unicode_stride);
      پهريون_glyph = &Struct_At_وٿي_Unaligned<hb_codepoint_t> (پهريون_glyph, glyph_stride);
    }
    أرجع پورو;
  }

  منطقي get_variation_glyph (hb_codepoint_t  unicode,
			    hb_codepoint_t  variation_selector,
			    hb_codepoint_t *glyph) ثابت
  {
    glyph_variant_t نتيجو = dmap.get_glyph_variant (unicode,
						     variation_selector,
						     glyph);
    إذا (نتيجو == GLYPH_VARIANT_NOT_مليو)
      نتيجو = بنياد.get_glyph_variant (unicode, variation_selector, glyph);

    بدل (نتيجو)
    {
      حالة GLYPH_VARIANT_NOT_مليو: أرجع خطأ;
      حالة GLYPH_VARIANT_مليو: أرجع سچ;
      حالة GLYPH_VARIANT_USE_DEFAULT: اكسر;
    }

    أرجع get_nominal_glyph (unicode, glyph);
  }

  فراغ collect_unicodes (hb_سيٽ_t *ٻاهريون, بنانشان صحيح انگ_glyphs) ثابت
  {
    بنياد.collect_unicodes (ٻاهريون, انگ_glyphs);
    dmap.collect_unicodes (ٻاهريون, انگ_glyphs);
  }

  فراغ collect_mapping (hb_سيٽ_t *unicodes, hb_map_t *mapping,
			بنانشان انگ_glyphs = UINT_MAX) ثابت
  {
    بنياد.collect_mapping (unicodes, mapping, انگ_glyphs);
    dmap.collect_mapping (unicodes, mapping, انگ_glyphs);
  }

  فراغ collect_variation_selectors (hb_سيٽ_t *ٻاهريون) ثابت
  {
    بنياد.collect_variation_selectors (ٻاهريون);
    dmap.collect_variation_selectors (ٻاهريون);
  }

  فراغ collect_variation_unicodes (hb_codepoint_t variation_selector,
				   hb_سيٽ_t *ٻاهريون) ثابت
  {
    بنياد.collect_variation_unicodes (variation_selector, ٻاهريون);
    dmap.collect_variation_unicodes (variation_selector, ٻاهريون);
  }

  فراغ گهيرو_glyphs (ثابت hb_سيٽ_t *unicodes,
		       hb_سيٽ_t       *glyphset) ثابت
  {
    إذا (likely (!dmap.has_معلومات ()))
    {
      بنياد.جدول->گهيرو_glyphs (unicodes, glyphset);
      أرجع;
    }

    hb_سيٽ_t variation_selectors;
    collect_variation_selectors (&variation_selectors);
    لكل (hb_codepoint_t variation_selector : variation_selectors)
    {
      إذا (!unicodes->has (variation_selector))
	استمر;

      hb_سيٽ_t variation_unicodes;
      collect_variation_unicodes (variation_selector, &variation_unicodes);
      لكل (hb_codepoint_t unicode : variation_unicodes)
      {
	إذا (!unicodes->has (unicode))
	  استمر;

	hb_codepoint_t glyph;
	إذا (get_variation_glyph (unicode, variation_selector, &glyph))
	  glyphset->جوڙيو (glyph);
      }
    }
  }

  cmap::accelerator_t dmap;
  cmap::accelerator_t بنياد;
};

} /* namespace OT */


#جيڪڏهن_پورو /* HB_OT_CMAP_TABLE_HH */
