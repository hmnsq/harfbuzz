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

#که_نه_تعریف_شوی HB_OT_CMAP_جدول_HH
#تعریف_کړه HB_OT_CMAP_جدول_HH

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
#تعریف_کړه HB_OT_نښان_cmap HB_نښان('c','m','a','p')
#تعریف_کړه HB_OT_نښان_DMAP HB_نښان('D','M','A','P')

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
  خپلکاری *c = hb_bsearch (u, mapping, ARRAY_اوږدتیا (mapping), حجم (mapping[0]),
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
    أرجع ریښتیا;
  }

  بینښه get_language () ثابت
  {
    أرجع language;
  }

  فراغ collect_unicodes (hb_سېټ_t *بهر) ثابت
  {
    لكل (بینښه صحيح i = 0; i < 256; i++)
      إذا (glyphIdArray[i])
	بهر->جمع_کړه (i);
  }

  فراغ collect_mapping (hb_سېټ_t *unicodes, /*بهر*/
			hb_map_t *mapping /*بهر*/) ثابت
  {
    لكل (بینښه i = 0; i < 256; i++)
      إذا (glyphIdArray[i])
      {
	hb_codepoint_t glyph = glyphIdArray[i];
	unicodes->جمع_کړه (i);
	mapping->سېټ (i, glyph);
      }
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  محمي:
  HBUINT16	format;		/* Format number is set to 0. */
  HBUINT16	اوږدتیا;		/* Byte length of this subtable. */
  HBUINT16	language;	/* Ignore. */
  HBUINT8	glyphIdArray[256];/* An array that maps character
				 * code to glyph index values. */
  عام:
  DEFINE_اندازه_STATIC (6 + 256);
};

بنية CmapSubtableFormat4
{
  کينډۍ<ډول_نوم Iterator,
      ډول_نوم Writer,
	   hb_requires (hb_is_iterator (Iterator))>
  فراغ منزل_ranges (Iterator it, Writer& range_writer)
  {
    hb_codepoint_t پیل_cp = 0, prev_پرلپسې_پیل_cp = 0, پرلپسې_پیل_cp = 0, پای_ټکی_cp = 0, وروستی_gid = 0;
    صحيح پرلپسې_اوږدتیا = 0 , delta = 0, prev_delta = 0;

    تعداد {
      لومړی_SUB_RANGE,
      FOLLOWING_SUB_RANGE,
    } ډېرپېښ;

    طالما (it) {
      // Start a new range
      {
        ثابت خپلکاری& pair = *it;
        پیل_cp = pair.لومړی;
        prev_پرلپسې_پیل_cp = پیل_cp;
        پرلپسې_پیل_cp = پیل_cp;
        پای_ټکی_cp = پیل_cp;
        وروستی_gid = pair.second;
        پرلپسې_اوږدتیا = 1;
        prev_delta = 0;
      }

      delta = وروستی_gid - پیل_cp;
      ډېرپېښ = لومړی_SUB_RANGE;
      it++;

      طالما (it) {
        // Process range
        ثابت خپلکاری& pair = *it;
        hb_codepoint_t بل_cp = pair.لومړی;
        hb_codepoint_t بل_gid = pair.second;
        إذا (بل_cp != پای_ټکی_cp + 1) {
          // Current range is over, stop processing.
          اكسر;
        }

        إذا (بل_gid == وروستی_gid + 1) {
          // The current run continues.
          پای_ټکی_cp = بل_cp;
          پرلپسې_اوږدتیا++;
          وروستی_gid = بل_gid;
          it++;
          استمر;
        }

        // A new run is starting, decide if we want to commit the current run.
        صحيح وېشل_لګښت = (ډېرپېښ == لومړی_SUB_RANGE) ? 8 : 16;
        صحيح پرلپسې_لګښت = پرلپسې_اوږدتیا * 2;
        إذا (پرلپسې_لګښت >= وېشل_لګښت) {
          commit_current_range(پیل_cp,
                               prev_پرلپسې_پیل_cp,
                               پرلپسې_پیل_cp,
                               پای_ټکی_cp,
                               delta,
                               prev_delta,
                               وېشل_لګښت,
                               range_writer);
          پیل_cp = بل_cp;
        }

        // Start the new run
        ډېرپېښ = FOLLOWING_SUB_RANGE;
        prev_پرلپسې_پیل_cp = پرلپسې_پیل_cp;
        پرلپسې_پیل_cp = بل_cp;
        پای_ټکی_cp = بل_cp;
        prev_delta = delta;
        delta = بل_gid - پرلپسې_پیل_cp;
        پرلپسې_اوږدتیا = 1;
        وروستی_gid = بل_gid;
        it++;
      }

      // Finalize range
      commit_current_range (پیل_cp,
                            prev_پرلپسې_پیل_cp,
                            پرلپسې_پیل_cp,
                            پای_ټکی_cp,
                            delta,
                            prev_delta,
                            8,
                            range_writer);
    }

    إذا (likely (پای_ټکی_cp != 0xFFFF)) {
      range_writer (0xFFFF, 0xFFFF, 1);
    }
  }

  /*
   * Writes the current range as either one or two ranges depending on what is most efficient.
   */
  کينډۍ<ډول_نوم Writer>
  فراغ commit_current_range (hb_codepoint_t پیل,
                             hb_codepoint_t prev_پرلپسې_پیل,
                             hb_codepoint_t پرلپسې_پیل,
                             hb_codepoint_t پای_ټکی,
                             صحيح پرلپسې_delta,
                             صحيح پخوانی_پرلپسې_delta,
                             صحيح وېشل_لګښت,
                             Writer& range_writer) {
    منطقي should_وېشل = خطأ;
    إذا (پیل < پرلپسې_پیل && پرلپسې_پیل < پای_ټکی) {
      صحيح پرلپسې_لګښت = (پای_ټکی - پرلپسې_پیل + 1) * 2;
      إذا (پرلپسې_لګښت >= وېشل_لګښت) {
        should_وېشل = ریښتیا;
      }
    }

    // A zero delta is also the marker for ranges that need a
    // glyphIdArray; serialize_rangeoffset_glyid() tells the two cases
    // apart by re-checking the mapping.
    إذا (should_وېشل) {
      إذا (پیل == prev_پرلپسې_پیل)
        range_writer (پیل, پرلپسې_پیل - 1, پخوانی_پرلپسې_delta);
      وإلا
        range_writer (پیل, پرلپسې_پیل - 1, 0);
      range_writer (پرلپسې_پیل, پای_ټکی, پرلپسې_delta);
      أرجع;
    }


    إذا (پیل == پرلپسې_پیل) {
      // Range is only a run
      range_writer (پیل, پای_ټکی, پرلپسې_delta);
      أرجع;
    }

    // Write only a single non-run range.
    range_writer (پیل, پای_ټکی, 0);
  }

  کينډۍ<ډول_نوم Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  بینښه serialize_پیدا_کړه_segcount (Iterator it) {
    بنية Counter {
      بینښه segcount = 0;

      فراغ چلوونکی() (hb_codepoint_t پیل,
                       hb_codepoint_t پای_ټکی,
                       صحيح delta) {
        segcount++;
      }
    } counter;

    منزل_ranges (+it, counter);
    أرجع counter.segcount;
  }


  کينډۍ<ډول_نوم Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  منطقي serialize_پیل_پای_ټکی_delta_arrays (hb_serialize_context_t *c,
                                         Iterator it,
                                         صحيح segcount)
  {
    بنية Writer {
      hb_serialize_context_t *serializer_;
      HBUINT16* پای_ټکی_کوډ;
      HBUINT16* پیل_کوډ;
      HBINT16* id_delta_;
      صحيح شاخص_3;

      Writer(hb_serialize_context_t *serializer)
          : serializer_(serializer),
            پای_ټکی_کوډ(تش_اشاره),
            پیل_کوډ(تش_اشاره),
            id_delta_(تش_اشاره),
            شاخص_3 (0) {}
      فراغ چلوونکی() (hb_codepoint_t پیل,
                       hb_codepoint_t پای_ټکی,
                       صحيح delta) {
        پیل_کوډ[شاخص_3] = پیل;
        پای_ټکی_کوډ[شاخص_3] = پای_ټکی;
        id_delta_[شاخص_3] = delta;
        شاخص_3++;
      }
    } writer(c);

    writer.پای_ټکی_کوډ = c->allocate_اندازه<HBUINT16> (HBUINT16::static_اندازه * segcount, خطأ);
    (فراغ) c->allocate_اندازه<HBUINT16> (2); // padding
    writer.پیل_کوډ = c->allocate_اندازه<HBUINT16> (HBUINT16::static_اندازه * segcount, خطأ);
    writer.id_delta_ = c->allocate_اندازه<HBINT16> (HBINT16::static_اندازه * segcount, خطأ);

    إذا (unlikely (!writer.پای_ټکی_کوډ || !writer.پیل_کوډ || !writer.id_delta_)) أرجع خطأ;

    منزل_ranges (+it, writer);
    أرجع ریښتیا;
  }

  کينډۍ<ډول_نوم Iterator,
          hb_requires (hb_is_iterator (Iterator))>
  HBUINT16* serialize_rangeoffset_glyid (hb_serialize_context_t *c,
                                         Iterator it,
					 HBUINT16 *پای_ټکی_کوډ_2,
					 HBUINT16 *پیل_کوډ_2,
					 HBINT16 *idDelta,
					 بینښه segcount)
  {
    hb_map_t cp_منزل_gid { it };

    HBUINT16 *id_Range_دننه_واټن = c->allocate_اندازه<HBUINT16> (HBUINT16::static_اندازه * segcount);
    إذا (unlikely (!c->check_success (id_Range_دننه_واټن))) أرجع تش_اشاره;
    إذا (unlikely ((حرف *)id_Range_دننه_واټن - (حرف *)idDelta != (صحيح) segcount * (صحيح) HBINT16::static_اندازه)) أرجع تش_اشاره;

    لكل (بینښه i : hb_range (segcount))
    {
      /* A non-zero delta is always a real run. A zero delta is
       * ambiguous: identity mapping, or the marker for a range that
       * needs a glyphIdArray. Only zero deltas need a check. */
      صحيح delta = idDelta[i];
      إذا (delta)
	استمر;
      منطقي use_delta = ریښتیا;
      لكل (hb_codepoint_t cp = پیل_کوډ_2[i]; cp <= پای_ټکی_کوډ_2[i]; cp++)
      {
	hb_codepoint_t gid = cp_منزل_gid.get (cp);
	إذا (gid == HB_MAP_ارزښت_INVALID)
	  gid = 0;
	إذا (gid != cp)
	{
	  use_delta = خطأ;
	  اكسر;
	}
      }
      إذا (use_delta)
	استمر;

      id_Range_دننه_واټن[i] = 2 * (c->پیل_embed<HBUINT16> () - id_Range_دننه_واټن - i);
      لكل (hb_codepoint_t cp = پیل_کوډ_2[i]; cp <= پای_ټکی_کوډ_2[i]; cp++)
      {
        HBUINT16 gid;
        gid = cp_منزل_gid[cp];
        c->copy<HBUINT16> (gid);
      }
    }

    أرجع id_Range_دننه_واټن;
  }

  کينډۍ<ډول_نوم Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  فراغ serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    خپلکاری format4_iter =
    + it
    | hb_filter ([&] (ثابت hb_codepoint_pair_t _)
		 { أرجع _.لومړی <= 0xFFFF; })
    ;

    إذا (!format4_iter) أرجع;

    بینښه جدول_initpos = c->اوږدتیا ();
    إذا (unlikely (!c->extend_min (هذا))) أرجع;
    هذا->format = 4;

    hb_ویکټور_t<hb_codepoint_pair_t> cp_منزل_gid {
      format4_iter
    };

    //serialize endCode[], startCode[], idDelta[]
    HBUINT16* پای_ټکی_کوډ_2 = c->پیل_embed<HBUINT16> ();
    بینښه segcount = serialize_پیدا_کړه_segcount (cp_منزل_gid.iter());
    إذا (unlikely (!serialize_پیل_پای_ټکی_delta_arrays (c, cp_منزل_gid.iter(), segcount)))
      أرجع;

    HBUINT16 *پیل_کوډ_2 = پای_ټکی_کوډ_2 + segcount + 1;
    HBINT16 *idDelta = ((HBINT16*)پیل_کوډ_2) + segcount;

    HBUINT16 *id_Range_دننه_واټن = serialize_rangeoffset_glyid (c,
                                                           cp_منزل_gid.iter (),
                                                           پای_ټکی_کوډ_2,
                                                           پیل_کوډ_2,
                                                           idDelta,
                                                           segcount);
    إذا (unlikely (!c->check_success (id_Range_دننه_واټن))) أرجع;

    هذا->اوږدتیا = c->اوږدتیا () - جدول_initpos;
    إذا ((اوږد اوږد) هذا->اوږدتیا != (اوږد اوږد) c->اوږدتیا () - جدول_initpos)
    {
      c->err (HB_SERIALIZE_ERROR_INT_ډکېدنه);
      أرجع;
    }

    هذا->seg_شمېر_X2 = segcount * 2;
    هذا->ثبت_Selector = hb_max (1u, hb_بټ_storage (segcount)) - 1;
    هذا->پلټنه_Range = 2 * (1u << هذا->ثبت_Selector);
    هذا->range_ښويدنه = segcount * 2 > هذا->پلټنه_Range
		       ? 2 * segcount - هذا->پلټنه_Range
		       : 0;
  }

  بینښه get_language () ثابت
  {
    أرجع language;
  }

  بنية accelerator_t
  {
    accelerator_t () {}
    accelerator_t (ثابت CmapSubtableFormat4 *subtable) = ړنګول;

    فراغ init (ثابت CmapSubtableFormat4 *subtable,
	       بینښه صحيح              subtable_معلومات_اندازه)
    {
      seg_شمېر_2 = subtable->seg_شمېر_X2 / 2;
      پای_ټکی_شمېر = subtable->values.arrayZ;
      پیل_شمېر = پای_ټکی_شمېر + seg_شمېر_2 + 1;
      idDelta = پیل_شمېر + seg_شمېر_2;
      id_Range_دننه_واټن = idDelta + seg_شمېر_2;
      glyphIdArray = id_Range_دننه_واټن + seg_شمېر_2;

      بینښه صحيح ارزښتونه_دننه_واټن = 16 + 8 * seg_شمېر_2;
      glyph_Id_Array_اوږدتیا = subtable_معلومات_اندازه > ارزښتونه_دننه_واټن
			   ? (subtable_معلومات_اندازه - ارزښتونه_دننه_واټن) / 2
			   : 0;
    }

    منطقي get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) ثابت
    {
      بنية CustomRange
      {
	صحيح cmp (hb_codepoint_t k,
		 بینښه distance) ثابت
	{
	  إذا (k > وروستی) أرجع +1;
	  إذا (k < (&وروستی)[distance]/*لومړی*/) أرجع -1;
	  أرجع 0;
	}
	HBUINT16 وروستی;
      };

      ثابت HBUINT16 *وموندل = hb_bsearch (codepoint,
					  هذا->پای_ټکی_شمېر,
					  هذا->seg_شمېر_2,
					  حجم (CustomRange),
					  hb_cmp_لاره<hb_codepoint_t, CustomRange, بینښه>,
					  هذا->seg_شمېر_2 + 1);
      إذا (unlikely (!وموندل))
	أرجع خطأ;
      بینښه صحيح i = وموندل - پای_ټکی_شمېر;

      hb_codepoint_t gid;
      بینښه صحيح range_دننه_واټن = هذا->id_Range_دننه_واټن[i];
      إذا (range_دننه_واټن == 0)
	gid = codepoint + هذا->idDelta[i];
      وإلا
      {
	/* Somebody has been smoking... */
	بینښه صحيح شاخص_2 = range_دننه_واټن / 2 + (codepoint - هذا->پیل_شمېر[i]) + i - هذا->seg_شمېر_2;
	إذا (unlikely (شاخص_2 >= هذا->glyph_Id_Array_اوږدتیا))
	  أرجع خطأ;
	gid = هذا->glyphIdArray[شاخص_2];
	إذا (unlikely (!gid))
	  أرجع خطأ;
	gid += هذا->idDelta[i];
      }
      gid &= 0xFFFFu;
      إذا (unlikely (!gid))
	أرجع خطأ;
      *glyph = gid;
      أرجع ریښتیا;
    }

    HB_دننی ساكن منطقي get_glyph_func (ثابت فراغ *obj, hb_codepoint_t codepoint, hb_codepoint_t *glyph)
    { أرجع ((ثابت accelerator_t *) obj)->get_glyph (codepoint, glyph); }

    فراغ collect_unicodes (hb_سېټ_t *بهر) ثابت
    {
      بینښه صحيح شمېر = هذا->seg_شمېر_2;
      إذا (شمېر && هذا->پیل_شمېر[شمېر - 1] == 0xFFFFu)
	شمېر--; /* Skip sentinel segment. */
      لكل (بینښه صحيح i = 0; i < شمېر; i++)
      {
	hb_codepoint_t پیل = هذا->پیل_شمېر[i];
	hb_codepoint_t پای_ټکی = هذا->پای_ټکی_شمېر[i];
	بینښه صحيح range_دننه_واټن = هذا->id_Range_دننه_واټن[i];
        بهر->جمع_کړه_range(پیل, پای_ټکی);
	إذا (range_دننه_واټن == 0)
	{
	  لكل (hb_codepoint_t codepoint = پیل; codepoint <= پای_ټکی; codepoint++)
	  {
	    hb_codepoint_t gid = (codepoint + هذا->idDelta[i]) & 0xFFFFu;
	    إذا (unlikely (!gid))
              بهر->del(codepoint);
	  }
	}
	وإلا
	{
	  لكل (hb_codepoint_t codepoint = پیل; codepoint <= پای_ټکی; codepoint++)
	  {
	    بینښه صحيح شاخص_2 = range_دننه_واټن / 2 + (codepoint - هذا->پیل_شمېر[i]) + i - هذا->seg_شمېر_2;
	    إذا (unlikely (شاخص_2 >= هذا->glyph_Id_Array_اوږدتیا))
            {
              بهر->del_range (codepoint, پای_ټکی);
	      اكسر;
            }
	    hb_codepoint_t gid = هذا->glyphIdArray[شاخص_2];
	    إذا (unlikely (!gid))
              بهر->del(codepoint);
	  }
	}
      }
    }

    فراغ collect_mapping (hb_سېټ_t *unicodes, /*بهر*/
			  hb_map_t *mapping /*بهر*/) ثابت
    {
      // TODO(grieger): optimize similar to collect_unicodes
      // (ie. use add_range())
      بینښه شمېر = هذا->seg_شمېر_2;
      إذا (شمېر && هذا->پیل_شمېر[شمېر - 1] == 0xFFFFu)
	شمېر--; /* Skip sentinel segment. */
      لكل (بینښه i = 0; i < شمېر; i++)
      {
	hb_codepoint_t پیل = هذا->پیل_شمېر[i];
	hb_codepoint_t پای_ټکی = هذا->پای_ټکی_شمېر[i];
	بینښه range_دننه_واټن = هذا->id_Range_دننه_واټن[i];
	إذا (range_دننه_واټن == 0)
	{
	  لكل (hb_codepoint_t codepoint = پیل; codepoint <= پای_ټکی; codepoint++)
	  {
	    hb_codepoint_t gid = (codepoint + هذا->idDelta[i]) & 0xFFFFu;
	    إذا (unlikely (!gid))
	      استمر;
	    unicodes->جمع_کړه (codepoint);
	    mapping->سېټ (codepoint, gid);
	  }
	}
	وإلا
	{
	  لكل (hb_codepoint_t codepoint = پیل; codepoint <= پای_ټکی; codepoint++)
	  {
	    بینښه شاخص_2 = range_دننه_واټن / 2 + (codepoint - هذا->پیل_شمېر[i]) + i - هذا->seg_شمېر_2;
	    إذا (unlikely (شاخص_2 >= هذا->glyph_Id_Array_اوږدتیا))
	      اكسر;
	    hb_codepoint_t gid = هذا->glyphIdArray[شاخص_2];
	    إذا (unlikely (!gid))
	      استمر;
            gid = (gid + هذا->idDelta[i]) & 0xFFFFu;
	    unicodes->جمع_کړه (codepoint);
	    mapping->سېټ (codepoint, gid);
	  }
	}
      }
    }

    ثابت HBUINT16 *پای_ټکی_شمېر;
    ثابت HBUINT16 *پیل_شمېر;
    ثابت HBUINT16 *idDelta;
    ثابت HBUINT16 *id_Range_دننه_واټن;
    ثابت HBUINT16 *glyphIdArray;
    بینښه صحيح seg_شمېر_2;
    بینښه صحيح glyph_Id_Array_اوږدتیا;
  };

  منطقي get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph,
		  بینښه صحيح subtable_معلومات_اندازه) ثابت
  {
    accelerator_t accel;
    accel.init (هذا, subtable_معلومات_اندازه);
    أرجع accel.get_glyph_func (&accel, codepoint, glyph);
  }
  منطقي get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) ثابت
  { أرجع خطأ; }

  فراغ collect_unicodes (hb_سېټ_t *بهر, بینښه صحيح subtable_معلومات_اندازه) ثابت
  {
    accelerator_t accel;
    accel.init (هذا, subtable_معلومات_اندازه);
    accel.collect_unicodes (بهر);
  }
  فراغ collect_unicodes (hb_سېټ_t *بهر) ثابت
  { collect_unicodes (بهر, اوږدتیا); }

  فراغ collect_mapping (hb_سېټ_t *unicodes, /*بهر*/
			hb_map_t *mapping, /*بهر*/
			بینښه صحيح subtable_معلومات_اندازه) ثابت
  {
    accelerator_t accel;
    accel.init (هذا, subtable_معلومات_اندازه);
    accel.collect_mapping (unicodes, mapping);
  }
  فراغ collect_mapping (hb_سېټ_t *unicodes, /*بهر*/
			hb_map_t *mapping /*بهر*/) ثابت
  { collect_mapping (unicodes, mapping, اوږدتیا); }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!c->check_struct (هذا)))
      return_trace (خطأ);
    hb_barrier ();

    return_trace (c->check_range (values, 2 + 4 * seg_شمېر_X2));
  }

  محمي:
  HBUINT16	format;		/* Format number is set to 4. */
  HBUINT16	اوږدتیا;		/* This is the length in bytes of the
				 * subtable. */
  HBUINT16	language;	/* Ignore. */
  HBUINT16	seg_شمېر_X2;	/* 2 x segCount. */
  HBUINT16	پلټنه_Range;	/* 2 * (2**floor(log2(segCount))) */
  HBUINT16	ثبت_Selector;	/* log2(searchRange/2) */
  HBUINT16	range_ښويدنه;	/* 2 x segCount - searchRange */

  Unsized_Array_د<HBUINT16>
		values;
#إذا 0
  HBUINT16	پای_ټکی_شمېر[seg_شمېر_2];	/* End characterCode for each segment,
					 * last=0xFFFFu. */
  HBUINT16	reservedPad;		/* Set to 0. */
  HBUINT16	پیل_شمېر[seg_شمېر_2];	/* Start character code for each segment. */
  HBINT16		idDelta[seg_شمېر_2];	/* Delta for all character codes in segment. */
  HBUINT16	id_Range_دننه_واټن[seg_شمېر_2];/* Offsets into glyphIdArray or 0 */
  Unsized_Array_د<HBUINT16>
		glyphIdArray;	/* Glyph index array (arbitrary length) */
#د_که_پای

  عام:
  DEFINE_اندازه_ARRAY (14, values);
};

بنية Cmap_Subtable_Long_ډله
{
  ملګری بنية CmapSubtableFormat12;
  ملګری بنية CmapSubtableFormat13;
  کينډۍ<ډول_نوم U>
  ملګری بنية CmapSubtableLongSegmented;
  ملګری بنية cmap;

  صحيح cmp (hb_codepoint_t codepoint) ثابت
  {
    إذا (codepoint < پیل_Char_کوډ) أرجع -1;
    إذا (codepoint > پای_ټکی_Char_کوډ)   أرجع +1;
    أرجع 0;
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  خاص:
  HBUINT32		پیل_Char_کوډ;	/* First character code in this group. */
  HBUINT32		پای_ټکی_Char_کوډ;	/* Last character code in this group. */
  HBUINT32		glyphID;	/* Glyph index; interpretation depends on
					 * subtable format. */
  عام:
  DEFINE_اندازه_STATIC (12);
};
DECLARE_NULL_NAMESPACE_بایټونه (OT, Cmap_Subtable_Long_ډله);

کينډۍ <ډول_نوم UINT>
بنية CmapSubtableTrimmed
{
  منطقي get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) ثابت
  {
    /* Rely on our implicit array bound-checking. */
    hb_codepoint_t gid = glyphIdArray[codepoint - پیل_Char_کوډ];
    إذا (unlikely (!gid))
      أرجع خطأ;
    *glyph = gid;
    أرجع ریښتیا;
  }

  بینښه get_language () ثابت
  {
    أرجع language;
  }

  فراغ collect_unicodes (hb_سېټ_t *بهر) ثابت
  {
    hb_codepoint_t پیل = پیل_Char_کوډ;
    بینښه صحيح شمېر = glyphIdArray.len;
    لكل (بینښه صحيح i = 0; i < شمېر; i++)
      إذا (glyphIdArray[i])
	بهر->جمع_کړه (پیل + i);
  }

  فراغ collect_mapping (hb_سېټ_t *unicodes, /*بهر*/
			hb_map_t *mapping /*بهر*/) ثابت
  {
    hb_codepoint_t پیل_cp = پیل_Char_کوډ;
    بینښه شمېر = glyphIdArray.len;
    لكل (بینښه i = 0; i < شمېر; i++)
      إذا (glyphIdArray[i])
      {
	hb_codepoint_t unicode = پیل_cp + i;
	hb_codepoint_t glyphid = glyphIdArray[i];
	unicodes->جمع_کړه (unicode);
	mapping->سېټ (unicode, glyphid);
      }
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && glyphIdArray.sanitize (c));
  }

  محمي:
  UINT		formatReserved;	/* Subtable format and (maybe) padding. */
  UINT		اوږدتیا;		/* Byte length of this subtable. */
  UINT		language;	/* Ignore. */
  UINT		پیل_Char_کوډ;	/* First character code covered. */
  Array_د<HBGlyphID16, UINT>
		glyphIdArray;	/* Array of glyph index values for character
				 * codes in the range. */
  عام:
  DEFINE_اندازه_ARRAY (5 * حجم (UINT), glyphIdArray);
};

بنية CmapSubtableFormat6  : CmapSubtableTrimmed<HBUINT16> {};
بنية CmapSubtableFormat10 : CmapSubtableTrimmed<HBUINT32> {};

کينډۍ <ډول_نوم T>
بنية CmapSubtableLongSegmented
{
  ملګری بنية cmap;

  منطقي get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) ثابت
  {
    hb_codepoint_t gid = T::ډله_get_glyph (groups.bsearch (codepoint), codepoint);
    إذا (unlikely (!gid))
      أرجع خطأ;
    *glyph = gid;
    أرجع ریښتیا;
  }

  بینښه get_language () ثابت
  {
    أرجع language;
  }

  فراغ collect_unicodes (hb_سېټ_t *بهر, بینښه صحيح عدد_glyphs) ثابت
  {
    لكل (بینښه صحيح i = 0; i < هذا->groups.len; i++)
    {
      hb_codepoint_t پیل = هذا->groups[i].پیل_Char_کوډ;
      hb_codepoint_t پای_ټکی = hb_min ((hb_codepoint_t) هذا->groups[i].پای_ټکی_Char_کوډ,
				   (hb_codepoint_t) HB_UNICODE_MAX);
      hb_codepoint_t gid = هذا->groups[i].glyphID;
      إذا (!gid)
      {
	/* Intention is: if (hb_is_same (T, CmapSubtableFormat13)) continue; */
	إذا (! T::ډله_get_glyph (هذا->groups[i], پای_ټکی)) استمر;
	پیل++;
	gid++;
      }
      إذا (unlikely ((بینښه صحيح) gid >= عدد_glyphs)) استمر;
      إذا (unlikely ((بینښه صحيح) (gid + پای_ټکی - پیل) >= عدد_glyphs))
	پای_ټکی = پیل + (hb_codepoint_t) عدد_glyphs - gid;

      بهر->جمع_کړه_range (پیل, hb_min (پای_ټکی, 0x10FFFFu));
    }
  }

  فراغ collect_mapping (hb_سېټ_t *unicodes, /*بهر*/
			hb_map_t *mapping, /*بهر*/
			بینښه عدد_glyphs) ثابت
  {
    hb_codepoint_t وروستی_پای_ټکی = 0;
    بینښه شمېر = هذا->groups.len;
    لكل (بینښه i = 0; i < شمېر; i++)
    {
      hb_codepoint_t پیل = هذا->groups.arrayZ[i].پیل_Char_کوډ;
      hb_codepoint_t پای_ټکی = hb_min ((hb_codepoint_t) هذا->groups.arrayZ[i].پای_ټکی_Char_کوډ,
				   (hb_codepoint_t) HB_UNICODE_MAX);
      إذا (unlikely (پیل > پای_ټکی || پیل < وروستی_پای_ټکی)) {
        // Range is not in order and is invalid, skip it.
        استمر;
      }
      وروستی_پای_ټکی = پای_ټکی;


      hb_codepoint_t gid = هذا->groups.arrayZ[i].glyphID;
      إذا (!gid)
      {
        إذا (T::format_شمېره == 13) استمر;
	پیل++;
	gid++;
      }
      إذا (unlikely ((بینښه صحيح) gid >= عدد_glyphs)) استمر;
      إذا (unlikely ((بینښه صحيح) (gid + پای_ټکی - پیل) >= عدد_glyphs))
	پای_ټکی = پیل + (hb_codepoint_t) عدد_glyphs - gid;

      mapping->alloc (mapping->get_population () + پای_ټکی - پیل + 1);

      unicodes->جمع_کړه_range (پیل, پای_ټکی);
      لكل (بینښه cp = پیل; cp <= پای_ټکی; cp++)
      {
	mapping->سېټ (cp, gid);
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
  HBUINT32	اوږدتیا;		/* Byte length of this subtable. */
  HBUINT32	language;	/* Ignore. */
  Sorted_Array32_د<Cmap_Subtable_Long_ډله>
		groups;		/* Groupings. */
  عام:
  DEFINE_اندازه_ARRAY (16, groups);
};

بنية CmapSubtableFormat12 : CmapSubtableLongSegmented<CmapSubtableFormat12>
{
  ساكن constexpr صحيح increment = 1;
  ساكن constexpr صحيح format_شمېره = 12;

  ساكن hb_codepoint_t ډله_get_glyph (ثابت Cmap_Subtable_Long_ډله &ډله,
					 hb_codepoint_t u)
  { أرجع likely (ډله.پیل_Char_کوډ <= ډله.پای_ټکی_Char_کوډ) ?
	   ډله.glyphID + (u - ډله.پیل_Char_کوډ) : 0; }


  کينډۍ<ډول_نوم Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  فراغ serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    إذا (!it) أرجع;
    بینښه جدول_initpos = c->اوږدتیا ();
    إذا (unlikely (!c->extend_min (هذا))) أرجع;

    hb_codepoint_t پیل_Char_کوډ = (hb_codepoint_t) -1, پای_ټکی_Char_کوډ = (hb_codepoint_t) -1;
    hb_codepoint_t glyphID = 0;

    لكل (ثابت خپلکاری& _ : +it)
    {
      إذا (پیل_Char_کوډ == (hb_codepoint_t) -1)
      {
	پیل_Char_کوډ = _.لومړی;
	پای_ټکی_Char_کوډ = _.لومړی;
	glyphID = _.second;
      }
      وإلا إذا (!_is_gid_consecutive (پای_ټکی_Char_کوډ, پیل_Char_کوډ, glyphID, _.لومړی, _.second))
      {
	Cmap_Subtable_Long_ډله  grouprecord;
	grouprecord.پیل_Char_کوډ = پیل_Char_کوډ;
	grouprecord.پای_ټکی_Char_کوډ = پای_ټکی_Char_کوډ;
	grouprecord.glyphID = glyphID;
	c->copy<Cmap_Subtable_Long_ډله> (grouprecord);

	پیل_Char_کوډ = _.لومړی;
	پای_ټکی_Char_کوډ = _.لومړی;
	glyphID = _.second;
      }
      وإلا
	پای_ټکی_Char_کوډ = _.لومړی;
    }

    Cmap_Subtable_Long_ډله ریکارډ;
    ریکارډ.پیل_Char_کوډ = پیل_Char_کوډ;
    ریکارډ.پای_ټکی_Char_کوډ = پای_ټکی_Char_کوډ;
    ریکارډ.glyphID = glyphID;
    c->copy<Cmap_Subtable_Long_ډله> (ریکارډ);

    هذا->format = 12;
    هذا->reserved = 0;
    هذا->اوږدتیا = c->اوږدتیا () - جدول_initpos;
    هذا->groups.len = (هذا->اوږدتیا - min_اندازه) / Cmap_Subtable_Long_ډله::static_اندازه;
  }

  ساكن size_t get_sub_جدول_اندازه (ثابت hb_sorted_ویکټور_t<Cmap_Subtable_Long_ډله> &groups_معلومات)
  { أرجع 16 + 12 * groups_معلومات.اوږدتیا; }

  خاص:
  ساكن منطقي _is_gid_consecutive (hb_codepoint_t پای_ټکی_Char_کوډ,
				   hb_codepoint_t پیل_Char_کوډ,
				   hb_codepoint_t glyphID,
				   hb_codepoint_t cp,
				   hb_codepoint_t new_gid)
  {
    أرجع (cp - 1 == پای_ټکی_Char_کوډ) &&
	new_gid == glyphID + (cp - پیل_Char_کوډ);
  }

};

بنية CmapSubtableFormat13 : CmapSubtableLongSegmented<CmapSubtableFormat13>
{
  ساكن constexpr صحيح increment = 0;
  ساكن constexpr صحيح format_شمېره = 13;

  ساكن hb_codepoint_t ډله_get_glyph (ثابت Cmap_Subtable_Long_ډله &ډله,
					 hb_codepoint_t u HB_ناکارول_شوی)
  { أرجع ډله.glyphID; }
};

تعريفنوع تعداد
{
  GLYPH_VARIANT_NOT_وموندل = 0,
  GLYPH_VARIANT_وموندل = 1,
  GLYPH_VARIANT_USE_DEFAULT = 2
} glyph_variant_t;

بنية Unicode_ارزښت_Range
{
  صحيح cmp (ثابت hb_codepoint_t &codepoint) ثابت
  {
    إذا (codepoint < پیل_Unicode_ارزښت) أرجع -1;
    إذا (codepoint > پیل_Unicode_ارزښت + additional_شمېر) أرجع +1;
    أرجع 0;
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  HBUINT24	پیل_Unicode_ارزښت;	/* First value in this range. */
  HBUINT8	additional_شمېر;	/* Number of additional values in this
					 * range. */
  عام:
  DEFINE_اندازه_STATIC (4);
};

بنية DefaultUVS : Sorted_Array32_د<Unicode_ارزښت_Range>
{
  فراغ collect_unicodes (hb_سېټ_t *بهر) ثابت
  {
    بینښه صحيح شمېر = len;
    لكل (بینښه صحيح i = 0; i < شمېر; i++)
    {
      hb_codepoint_t لومړی = arrayZ[i].پیل_Unicode_ارزښت;
      hb_codepoint_t وروستی = hb_min ((hb_codepoint_t) (لومړی + arrayZ[i].additional_شمېر),
				    (hb_codepoint_t) HB_UNICODE_MAX);
      بهر->جمع_کړه_range (لومړی, وروستی);
    }
  }

  DefaultUVS* copy (hb_serialize_context_t *c,
		    ثابت hb_سېټ_t *unicodes) ثابت
  {
    خپلکاری *بهر = c->پیل_embed<DefaultUVS> ();
    خپلکاری snap = c->snapshot ();

    HBUINT32 len;
    len = 0;
    إذا (unlikely (!c->copy<HBUINT32> (len))) أرجع تش_اشاره;
    بینښه init_len = c->اوږدتیا ();

    إذا (هذا->len > unicodes->get_population () * hb_بټ_storage ((بینښه) هذا->len))
    {
      hb_codepoint_t پیل = HB_سېټ_ارزښت_INVALID;
      hb_codepoint_t پای_ټکی = HB_سېټ_ارزښت_INVALID;

      لكل (خپلکاری u : *unicodes)
      {
        إذا (!as_array ().bsearch (u))
	  استمر;
	إذا (پیل == HB_سېټ_ارزښت_INVALID)
	{
	  پیل = u;
	  پای_ټکی = پیل - 1;
	}
	إذا (پای_ټکی + 1 != u || پای_ټکی - پیل == 255)
        {
	  Unicode_ارزښت_Range rec;
	  rec.پیل_Unicode_ارزښت = پیل;
	  rec.additional_شمېر = پای_ټکی - پیل;
	  c->copy<Unicode_ارزښت_Range> (rec);
	  پیل = u;
	}
	پای_ټکی = u;
      }
      إذا (پیل != HB_سېټ_ارزښت_INVALID)
      {
	Unicode_ارزښت_Range rec;
	rec.پیل_Unicode_ارزښت = پیل;
	rec.additional_شمېر = پای_ټکی - پیل;
	c->copy<Unicode_ارزښت_Range> (rec);
      }

    }
    وإلا
    {
      hb_codepoint_t وروستی_کوډ = HB_سېټ_ارزښت_INVALID;
      صحيح شمېر = -1;

      لكل (ثابت Unicode_ارزښت_Range& _ : *هذا)
      {
	hb_codepoint_t cur_ثبت = (hb_codepoint_t) (_.پیل_Unicode_ارزښت - 1);
	hb_codepoint_t پای_ټکی = cur_ثبت + _.additional_شمېر + 2;

	لكل (; unicodes->next (&cur_ثبت) && cur_ثبت < پای_ټکی;)
	{
	  شمېر += 1;
	  إذا (وروستی_کوډ == HB_سېټ_ارزښت_INVALID)
	    وروستی_کوډ = cur_ثبت;
	  وإلا إذا (وروستی_کوډ + شمېر != cur_ثبت || شمېر > 255)
	  {
	    Unicode_ارزښت_Range rec;
	    rec.پیل_Unicode_ارزښت = وروستی_کوډ;
	    rec.additional_شمېر = شمېر - 1;
	    c->copy<Unicode_ارزښت_Range> (rec);

	    وروستی_کوډ = cur_ثبت;
	    شمېر = 0;
	  }
	}
      }

      إذا (وروستی_کوډ != HB_MAP_ارزښت_INVALID)
      {
	Unicode_ارزښت_Range rec;
	rec.پیل_Unicode_ارزښت = وروستی_کوډ;
	rec.additional_شمېر = شمېر;
	c->copy<Unicode_ارزښت_Range> (rec);
      }
    }

    إذا (c->اوږدتیا () - init_len == 0)
    {
      c->revert (snap);
      أرجع تش_اشاره;
    }
    وإلا
    {
      إذا (unlikely (!c->check_ټاکل (بهر->len,
                                      (c->اوږدتیا () - init_len) / Unicode_ارزښت_Range::static_اندازه,
                                      HB_SERIALIZE_ERROR_INT_ډکېدنه))) أرجع تش_اشاره;
      أرجع بهر;
    }
  }

  عام:
  DEFINE_اندازه_ARRAY (4, *هذا);
};

کينډۍ <ډول_نوم Types>
بنية UVSMapping
{
  کارول GlyphID = ډول_نوم Types::HBGlyphID;

  صحيح cmp (ثابت hb_codepoint_t &codepoint) ثابت
  { أرجع unicode_ارزښت.cmp (codepoint); }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  HBUINT24	unicode_ارزښت;	/* Base Unicode value of the UVS */
  GlyphID
		glyphID;	/* Glyph ID of the UVS. */
  عام:
  DEFINE_اندازه_STATIC (3 + GlyphID::static_اندازه);
};

کينډۍ <ډول_نوم Types>
بنية NonDefaultUVS : Sorted_Array32_د<UVSMapping<Types>>
{
  کارول Mapping = UVSMapping<Types>;

  فراغ collect_unicodes (hb_سېټ_t *بهر) ثابت
  {
    لكل (ثابت خپلکاری& a : هذا->as_array ())
      بهر->جمع_کړه (a.unicode_ارزښت);
  }

  فراغ collect_mapping (hb_سېټ_t *unicodes, /*بهر*/
			hb_map_t *mapping /*بهر*/) ثابت
  {
    لكل (ثابت خپلکاری& a : هذا->as_array ())
    {
      hb_codepoint_t unicode = a.unicode_ارزښت;
      hb_codepoint_t glyphid = a.glyphID;
      unicodes->جمع_کړه (unicode);
      mapping->سېټ (unicode, glyphid);
    }
  }

  فراغ تړون_glyphs (ثابت hb_سېټ_t      *unicodes,
		       hb_سېټ_t            *glyphset) ثابت
  {
    + هذا->as_array ()
    | hb_filter (unicodes, &Mapping::unicode_ارزښت)
    | hb_map (&Mapping::glyphID)
    | hb_sink (glyphset)
    ;
  }

  NonDefaultUVS* copy (hb_serialize_context_t *c,
		       ثابت hb_سېټ_t *unicodes,
		       ثابت hb_سېټ_t *glyphs_requested,
		       ثابت hb_map_t *glyph_map) ثابت
  {
    خپلکاری *بهر = c->پیل_embed<NonDefaultUVS> ();
    خپلکاری it =
    + هذا->as_array ()
    | hb_filter ([&] (ثابت Mapping& _)
		 {
		   أرجع unicodes->has (_.unicode_ارزښت) || glyphs_requested->has (_.glyphID);
		 })
    ;

    إذا (!it) أرجع تش_اشاره;

    HBUINT32 len;
    len = it.len ();
    إذا (unlikely (!c->copy<HBUINT32> (len))) أرجع تش_اشاره;

    لكل (ثابت Mapping& _ : it)
    {
      Mapping mapping;
      mapping.unicode_ارزښت = _.unicode_ارزښت;
      mapping.glyphID = glyph_map->get (_.glyphID);
      c->copy<Mapping> (mapping);
    }

    أرجع بهر;
  }

  عام:
  DEFINE_اندازه_ARRAY (4, *هذا);
};

کينډۍ <ډول_نوم Types>
بنية Variation_Selector_ریکارډ
{
  glyph_variant_t get_glyph (hb_codepoint_t codepoint,
			     hb_codepoint_t *glyph,
			     ثابت فراغ *بنسټ) ثابت
  {
    إذا ((بنسټ+defaultUVS).bfind (codepoint))
      أرجع GLYPH_VARIANT_USE_DEFAULT;
    ثابت UVSMapping<Types> &nonDefault = (بنسټ+nonDefaultUVS).bsearch (codepoint);
    إذا (nonDefault.glyphID)
    {
      *glyph = nonDefault.glyphID;
       أرجع GLYPH_VARIANT_وموندل;
    }
    أرجع GLYPH_VARIANT_NOT_وموندل;
  }

  Variation_Selector_ریکارډ(ثابت Variation_Selector_ریکارډ& other)
  {
    *هذا = other;
  }

  فراغ چلوونکی= (ثابت Variation_Selector_ریکارډ& other)
  {
    varSelector = other.varSelector;
    HBUINT32 دننه_واټن = other.defaultUVS;
    defaultUVS = دننه_واټن;
    دننه_واټن = other.nonDefaultUVS;
    nonDefaultUVS = دننه_واټن;
  }

  فراغ collect_unicodes (hb_سېټ_t *بهر, ثابت فراغ *بنسټ) ثابت
  {
    (بنسټ+defaultUVS).collect_unicodes (بهر);
    (بنسټ+nonDefaultUVS).collect_unicodes (بهر);
  }

  فراغ collect_mapping (ثابت فراغ *بنسټ,
			hb_سېټ_t *unicodes, /*بهر*/
			hb_map_t *mapping /*بهر*/) ثابت
  {
    (بنسټ+defaultUVS).collect_unicodes (unicodes);
    (بنسټ+nonDefaultUVS).collect_mapping (unicodes, mapping);
  }

  صحيح cmp (ثابت hb_codepoint_t &variation_selector) ثابت
  { أرجع varSelector.cmp (variation_selector); }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *بنسټ) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  defaultUVS.sanitize (c, بنسټ) &&
		  nonDefaultUVS.sanitize (c, بنسټ));
  }

  hb_pair_t<بینښه, بینښه>
  copy (hb_serialize_context_t *c,
	ثابت hb_سېټ_t *unicodes,
	ثابت hb_سېټ_t *glyphs_requested,
	ثابت hb_map_t *glyph_map,
	ثابت فراغ *بنسټ) ثابت
  {
    خپلکاری snap = c->snapshot ();
    خپلکاری *بهر = c->embed<Variation_Selector_ریکارډ> (*هذا);
    إذا (unlikely (!بهر)) أرجع hb_pair (0, 0);

    بهر->defaultUVS = 0;
    بهر->nonDefaultUVS = 0;

    بینښه non_default_uvs_objidx = 0;
    إذا (nonDefaultUVS != 0)
    {
      c->push ();
      إذا (c->copy (بنسټ+nonDefaultUVS, unicodes, glyphs_requested, glyph_map))
	non_default_uvs_objidx = c->راوباسه_pack ();
      وإلا c->راوباسه_discard ();
    }

    بینښه default_uvs_objidx = 0;
    إذا (defaultUVS != 0)
    {
      c->push ();
      إذا (c->copy (بنسټ+defaultUVS, unicodes))
	default_uvs_objidx = c->راوباسه_pack ();
      وإلا c->راوباسه_discard ();
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
  DEFINE_اندازه_STATIC (11);
};

کينډۍ <ډول_نوم Types>
بنية CmapSubtableFormat14_15
{
  ساكن constexpr بینښه format_ارزښت = Types::اندازه == 2 ? 14 : 15;
  کارول ریکارډ_2 = Variation_Selector_ریکارډ<Types>;
  کارول NonDefault = NonDefaultUVS<Types>;

  glyph_variant_t get_glyph_variant (hb_codepoint_t codepoint,
				     hb_codepoint_t variation_selector,
				     hb_codepoint_t *glyph) ثابت
  { أرجع ریکارډ.bsearch (variation_selector).get_glyph (codepoint, glyph, هذا); }

  فراغ collect_variation_selectors (hb_سېټ_t *بهر) ثابت
  {
    لكل (ثابت خپلکاری& a : ریکارډ.as_array ())
      بهر->جمع_کړه (a.varSelector);
  }
  فراغ collect_variation_unicodes (hb_codepoint_t variation_selector,
				   hb_سېټ_t *بهر) ثابت
  { ریکارډ.bsearch (variation_selector).collect_unicodes (بهر, هذا); }

  فراغ serialize (hb_serialize_context_t *c,
		  ثابت hb_سېټ_t *unicodes,
		  ثابت hb_سېټ_t *glyphs_requested,
		  ثابت hb_map_t *glyph_map,
		  ثابت فراغ *بنسټ)
  {
    خپلکاری snap = c->snapshot ();
    بینښه جدول_initpos = c->اوږدتیا ();
    ثابت حرف* init_لکۍ = c->لکۍ;

    إذا (unlikely (!c->extend_min (هذا))) أرجع;
    هذا->format = format_ارزښت;

    خپلکاری src_tbl = reinterpret_cast<ثابت CmapSubtableFormat14_15*> (بنسټ);

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
    hb_ویکټور_t<hb_pair_t<بینښه, بینښه>> obj_indices;
    لكل (صحيح i = src_tbl->ریکارډ.len - 1; i >= 0; i--)
    {
      إذا (!unicodes->has(src_tbl->ریکارډ[i].varSelector))
        استمر;

      hb_pair_t<بینښه, بینښه> پایله = src_tbl->ریکارډ[i].copy (c, unicodes, glyphs_requested, glyph_map, بنسټ);
      إذا (پایله.لومړی || پایله.second)
	obj_indices.push (پایله);
    }

    إذا (c->اوږدتیا () - جدول_initpos == CmapSubtableFormat14_15::min_اندازه)
    {
      c->revert (snap);
      أرجع;
    }

    إذا (unlikely (!c->check_success (!obj_indices.دننه_error ())))
      أرجع;

    صحيح لکۍ_len = init_لکۍ - c->لکۍ;
    c->check_ټاکل (هذا->اوږدتیا, c->اوږدتیا () - جدول_initpos + لکۍ_len,
                     HB_SERIALIZE_ERROR_INT_ډکېدنه);
    c->check_ټاکل (هذا->ریکارډ.len,
		     (c->اوږدتیا () - جدول_initpos - CmapSubtableFormat14_15::min_اندازه) /
		     ریکارډ_2::static_اندازه,
                     HB_SERIALIZE_ERROR_INT_ډکېدنه);

    /* Correct the incorrect write order by reversing the order of the variation
       records array. */
    سرچپه_variation_ریکارډونه ();

    /* Now that records are in the right order, we can set up the offsets. */
    جمع_کړه_links_منزل_variation_ریکارډونه (c, obj_indices);
  }

  فراغ سرچپه_variation_ریکارډونه ()
  {
    ریکارډ.as_array ().reverse ();
  }

  فراغ جمع_کړه_links_منزل_variation_ریکارډونه (hb_serialize_context_t *c,
					ثابت hb_ویکټور_t<hb_pair_t<بینښه, بینښه>>& obj_indices)
  {
    لكل (بینښه i = 0; i < obj_indices.اوږدتیا; i++)
    {
      /*
       * Since the record array has been reversed (see comments in copy())
       * but obj_indices has not been, the indices at obj_indices[i]
       * are for the variation record at record[j].
       */
      صحيح j = obj_indices.اوږدتیا - 1 - i;
      c->جمع_کړه_link (ریکارډ[j].defaultUVS, obj_indices[i].لومړی);
      c->جمع_کړه_link (ریکارډ[j].nonDefaultUVS, obj_indices[i].second);
    }
  }

  فراغ تړون_glyphs (ثابت hb_سېټ_t      *unicodes,
		       hb_سېټ_t            *glyphset) ثابت
  {
    + hb_iter (ریکارډ)
    | hb_filter (hb_bool, &ریکارډ_2::nonDefaultUVS)
    | hb_filter (unicodes, &ریکارډ_2::varSelector)
    | hb_map (&ریکارډ_2::nonDefaultUVS)
    | hb_map (hb_جمع_کړه (هذا))
    | hb_apply ([=] (ثابت NonDefault& _) { _.تړون_glyphs (unicodes, glyphset); })
    ;
  }

  فراغ collect_unicodes (hb_سېټ_t *بهر) ثابت
  {
    لكل (ثابت ریکارډ_2& _ : ریکارډ)
      _.collect_unicodes (بهر, هذا);
  }

  فراغ collect_mapping (hb_سېټ_t *unicodes, /*بهر*/
			hb_map_t *mapping /*بهر*/) ثابت
  {
    لكل (ثابت ریکارډ_2& _ : ریکارډ)
      _.collect_mapping (هذا, unicodes, mapping);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  ریکارډ.sanitize (c, هذا));
  }

  محمي:
  HBUINT16	format;		/* Format number is set to 14 or 15. */
  HBUINT32	اوږدتیا;		/* Byte length of this subtable. */
  Sorted_Array32_د<ریکارډ_2>
		ریکارډ;		/* Variation selector records; sorted
				 * in increasing order of `varSelector'. */
  عام:
  DEFINE_اندازه_ARRAY (10, ریکارډ);
};

بنية CmapSubtable
{
  /* Note: We intentionally do NOT implement subtable formats 2 and 8. */

  منطقي is_variation_selector () ثابت
  {
    بدل (u.format.v) {
    حالة 14:
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    حالة 15:
#د_که_پای
      أرجع ریښتیا;
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
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    حالة 15: hb_barrier (); أرجع u.format15.get_glyph_variant (codepoint, variation_selector, glyph);
#د_که_پای
    افتراضي: أرجع GLYPH_VARIANT_NOT_وموندل;
    }
  }

  فراغ collect_variation_selectors (hb_سېټ_t *بهر) ثابت
  {
    بدل (u.format.v) {
    حالة 14: hb_barrier (); u.format14.collect_variation_selectors (بهر); أرجع;
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    حالة 15: hb_barrier (); u.format15.collect_variation_selectors (بهر); أرجع;
#د_که_پای
    افتراضي: أرجع;
    }
  }

  فراغ collect_variation_unicodes (hb_codepoint_t variation_selector,
				    hb_سېټ_t *بهر) ثابت
  {
    بدل (u.format.v) {
    حالة 14: hb_barrier (); u.format14.collect_variation_unicodes (variation_selector, بهر); أرجع;
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    حالة 15: hb_barrier (); u.format15.collect_variation_unicodes (variation_selector, بهر); أرجع;
#د_که_پای
    افتراضي: أرجع;
    }
  }

  فراغ تړون_glyphs (ثابت hb_سېټ_t *unicodes, hb_سېټ_t *glyphset) ثابت
  {
    بدل (u.format.v) {
    حالة 14: hb_barrier (); u.format14.تړون_glyphs (unicodes, glyphset); أرجع;
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    حالة 15: hb_barrier (); u.format15.تړون_glyphs (unicodes, glyphset); أرجع;
#د_که_پای
    افتراضي: أرجع;
    }
  }

  منطقي get_glyph (hb_codepoint_t codepoint,
		  hb_codepoint_t *glyph,
		  بینښه صحيح subtable_معلومات_اندازه = 0) ثابت
  {
    بدل (u.format.v) {
    حالة  0: hb_barrier (); أرجع u.format0 .get_glyph (codepoint, glyph);
    حالة  4: hb_barrier (); أرجع subtable_معلومات_اندازه
				      ? u.format4.get_glyph (codepoint, glyph, subtable_معلومات_اندازه)
				      : خطأ;
    حالة  6: hb_barrier (); أرجع u.format6 .get_glyph (codepoint, glyph);
    حالة 10: hb_barrier (); أرجع u.format10.get_glyph (codepoint, glyph);
    حالة 12: hb_barrier (); أرجع u.format12.get_glyph (codepoint, glyph);
    حالة 13: hb_barrier (); أرجع u.format13.get_glyph (codepoint, glyph);
    حالة 14:
    افتراضي: أرجع خطأ;
    }
  }
  فراغ collect_unicodes (hb_سېټ_t *بهر,
			 بینښه صحيح عدد_glyphs = UINT_MAX,
			 بینښه صحيح subtable_معلومات_اندازه = 0) ثابت
  {
    بدل (u.format.v) {
    حالة  0: hb_barrier (); u.format0 .collect_unicodes (بهر); أرجع;
    حالة  4: hb_barrier (); إذا (subtable_معلومات_اندازه)
			      u.format4.collect_unicodes (بهر, subtable_معلومات_اندازه);
			    وإلا
			      u.format4.collect_unicodes (بهر);
			    أرجع;
    حالة  6: hb_barrier (); u.format6 .collect_unicodes (بهر); أرجع;
    حالة 10: hb_barrier (); u.format10.collect_unicodes (بهر); أرجع;
    حالة 12: hb_barrier (); u.format12.collect_unicodes (بهر, عدد_glyphs); أرجع;
    حالة 13: hb_barrier (); u.format13.collect_unicodes (بهر, عدد_glyphs); أرجع;
    حالة 14:
    افتراضي: أرجع;
    }
  }

  فراغ collect_mapping (hb_سېټ_t *unicodes, /*بهر*/
			hb_map_t *mapping, /*بهر*/
			بینښه عدد_glyphs = UINT_MAX,
			بینښه صحيح subtable_معلومات_اندازه = 0) ثابت
  {
    بدل (u.format.v) {
    حالة  0: hb_barrier (); u.format0 .collect_mapping (unicodes, mapping); أرجع;
    حالة  4: hb_barrier (); إذا (subtable_معلومات_اندازه)
			      u.format4.collect_mapping (unicodes, mapping, subtable_معلومات_اندازه);
			    وإلا
			      u.format4.collect_mapping (unicodes, mapping);
			    أرجع;
    حالة  6: hb_barrier (); u.format6 .collect_mapping (unicodes, mapping); أرجع;
    حالة 10: hb_barrier (); u.format10.collect_mapping (unicodes, mapping); أرجع;
    حالة 12: hb_barrier (); u.format12.collect_mapping (unicodes, mapping, عدد_glyphs); أرجع;
    حالة 13: hb_barrier (); u.format13.collect_mapping (unicodes, mapping, عدد_glyphs); أرجع;
    حالة 14:
    افتراضي: أرجع;
    }
  }

  بینښه get_language () ثابت
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

  کينډۍ<ډول_نوم Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  فراغ serialize (hb_serialize_context_t *c,
		  Iterator it,
		  بینښه format,
		  ثابت hb_subset_plan_t *plan,
		  ثابت فراغ *بنسټ)
  {
    بدل (format) {
    حالة  4: hb_barrier (); أرجع u.format4.serialize (c, it);
    حالة 12: hb_barrier (); أرجع u.format12.serialize (c, it);
    حالة 14: hb_barrier (); أرجع u.format14.serialize (c, &plan->unicodes, &plan->glyphs_requested, plan->glyph_map, بنسټ);
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    حالة 15: hb_barrier (); أرجع u.format15.serialize (c, &plan->unicodes, &plan->glyphs_requested, plan->glyph_map, بنسټ);
#د_که_پای
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
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    حالة 15: hb_barrier (); return_trace (u.format15.sanitize (c));
#د_که_پای
    افتراضي:return_trace (ریښتیا);
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
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
  CmapSubtableFormat14_15<Layout::MediumTypes>
			format15;
#د_که_پای
  } u;
  عام:
  DEFINE_اندازه_UNION (2, format.v);
};


بنية Encoding_ریکارډ
{
  صحيح cmp (ثابت Encoding_ریکارډ &other) ثابت
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

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *بنسټ) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  subtable.sanitize (c, بنسټ));
  }

  کينډۍ<ډول_نوم Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  Encoding_ریکارډ* copy (hb_serialize_context_t *c,
			Iterator it,
			بینښه format,
			ثابت فراغ *بنسټ,
			ثابت hb_subset_plan_t *plan,
			/* INOUT */ بینښه *objidx) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خپلکاری snap = c->snapshot ();
    خپلکاری *بهر = c->embed (هذا);
    إذا (unlikely (!بهر)) return_trace (تش_اشاره);
    بهر->subtable = 0;

    إذا (*objidx == 0)
    {
      CmapSubtable *cmapsubtable = c->push<CmapSubtable> ();
      بینښه origin_اوږدتیا = c->اوږدتیا ();
      cmapsubtable->serialize (c, it, format, plan, &(بنسټ+subtable));
      إذا (c->اوږدتیا () > origin_اوږدتیا && !c->دننه_error()) *objidx = c->راوباسه_pack ();
      وإلا c->راوباسه_discard ();
    }

    إذا (*objidx == 0)
    {
      c->revert (snap);
      return_trace (تش_اشاره);
    }

    c->جمع_کړه_link (بهر->subtable, *objidx);
    return_trace (بهر);
  }

  HBUINT16	platformID;	/* Platform ID. */
  HBUINT16	encodingID;	/* Platform-specific encoding ID. */
  Offset32_منزل<CmapSubtable>
		subtable;	/* Byte offset from beginning of table to the subtable for this encoding. */
  عام:
  DEFINE_اندازه_STATIC (8);
};

بنية cmap;

بنية SubtableUnicodesCache {

 خاص:
  hb_blob_ptr_t<cmap> بنسټ_blob;
  ثابت حرف* بنسټ;
  بینښه صحيح جدول_اوږدتیا;
  hb_hashmap_t<بینښه, hb::unique_ptr<hb_سېټ_t>> cached_unicodes;

 عام:

  ساكن SubtableUnicodesCache* create (hb_blob_ptr_t<cmap> سرچینه_جدول)
  {
    SubtableUnicodesCache* cache =
        (SubtableUnicodesCache*) hb_malloc (حجم(SubtableUnicodesCache));

    إذا (unlikely (!cache))
      أرجع تش_اشاره;

    جديد (cache) SubtableUnicodesCache (سرچینه_جدول);
    أرجع cache;
  }

  ساكن فراغ destroy (فراغ* value) {
    إذا (!value) أرجع;

    SubtableUnicodesCache* cache = (SubtableUnicodesCache*) value;
    cache->~SubtableUnicodesCache ();
    hb_free (cache);
  }

  SubtableUnicodesCache(ثابت فراغ* cmap_بنسټ,
			بینښه صحيح جدول_اوږدتیا_2 = 0)
      : بنسټ_blob(),
        بنسټ ((ثابت حرف*) cmap_بنسټ),
        جدول_اوږدتیا (جدول_اوږدتیا_2),
        cached_unicodes ()
  {}

  SubtableUnicodesCache(hb_blob_ptr_t<cmap> بنسټ_blob_2)
      : بنسټ_blob(بنسټ_blob_2),
        بنسټ ((ثابت حرف *) بنسټ_blob.get()),
        جدول_اوږدتیا (بنسټ_blob.get_اوږدتیا ()),
        cached_unicodes ()
  {}

  ~SubtableUnicodesCache()
  {
    بنسټ_blob.destroy ();
  }

  منطقي same_بنسټ(ثابت فراغ* other) ثابت
  {
    أرجع other == (ثابت فراغ*) بنسټ;
  }

  ثابت hb_سېټ_t* سېټ_for (ثابت Encoding_ریکارډ* ریکارډ,
                           SubtableUnicodesCache& mutable_cache) ثابت
  {
    إذا (cached_unicodes.has ((بینښه) ((ثابت حرف *) ریکارډ - بنسټ)))
      أرجع cached_unicodes.get ((بینښه) ((ثابت حرف *) ریکارډ - بنسټ));

    أرجع mutable_cache.سېټ_for (ریکارډ);
  }

  ثابت hb_سېټ_t* سېټ_for (ثابت Encoding_ریکارډ* ریکارډ)
  {
    إذا (!cached_unicodes.has ((بینښه) ((ثابت حرف *) ریکارډ - بنسټ)))
    {
      hb_سېټ_t *s = hb_سېټ_create ();
      إذا (unlikely (s->دننه_error ()))
	أرجع hb_سېټ_get_تشه ();

      بینښه صحيح subtable_معلومات_اندازه = ریکارډ->subtable < جدول_اوږدتیا
					? جدول_اوږدتیا - (بینښه صحيح) ریکارډ->subtable
					: 0;
      (بنسټ+ریکارډ->subtable).collect_unicodes (s, UINT_MAX, subtable_معلومات_اندازه);

      إذا (unlikely (!cached_unicodes.سېټ ((بینښه) ((ثابت حرف *) ریکارډ - بنسټ), hb::unique_ptr<hb_سېټ_t> {s})))
        أرجع hb_سېټ_get_تشه ();

      أرجع s;
    }
    أرجع cached_unicodes.get ((بینښه) ((ثابت حرف *) ریکارډ - بنسټ));
  }

};

ساكن مضمن uint16_t
hb_نښه_pua_map (بینښه codepoint)
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
  ساكن constexpr hb_نښان_t جدول_نښان = HB_OT_نښان_cmap;


  ساكن SubtableUnicodesCache* create_filled_cache(hb_blob_ptr_t<cmap> سرچینه_جدول) {
    ثابت cmap* cmap = سرچینه_جدول.get();
    خپلکاری it =
    + hb_iter (cmap->encoding_ریکارډ)
    | hb_filter ([&](ثابت Encoding_ریکارډ& _) {
      أرجع cmap::filter_encoding_ریکارډونه_for_subset (cmap, _);
    })
    ;

    SubtableUnicodesCache* cache = SubtableUnicodesCache::create(سرچینه_جدول);

    إذا (unlikely (!cache))
      أرجع تش_اشاره;

    لكل (ثابت Encoding_ریکارډ& _ : it)
      cache->سېټ_for(&_); // populate the cache for this encoding record.

    أرجع cache;
  }

  کينډۍ<ډول_نوم Iterator, ډول_نوم EncodingRecIter,
	   hb_requires (hb_is_iterator (EncodingRecIter))>
  منطقي serialize (hb_serialize_context_t *c,
		  Iterator it,
		  EncodingRecIter encodingrec_iter,
		  ثابت فراغ *بنسټ,
		  hb_subset_plan_t *plan,
	                  منطقي drop_format_4 = خطأ,
			  بینښه صحيح سرچینه_جدول_اوږدتیا = 0)
  {
    إذا (unlikely (!c->extend_min ((*هذا))))  أرجع خطأ;
    هذا->نسخه = 0;

    بینښه format4objidx = 0, format12objidx = 0, format14objidx = 0;
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    بینښه format15objidx = 0;
#د_که_پای
    خپلکاری snap = c->snapshot ();

    SubtableUnicodesCache local_unicodes_cache (بنسټ, سرچینه_جدول_اوږدتیا);
    ثابت SubtableUnicodesCache* unicodes_cache = &local_unicodes_cache;

    إذا (plan->accelerator &&
        plan->accelerator->cmap_cache &&
        plan->accelerator->cmap_cache->same_بنسټ (بنسټ))
      unicodes_cache = plan->accelerator->cmap_cache;

    لكل (ثابت Encoding_ریکارډ& _ : encodingrec_iter)
    {
      إذا (c->دننه_error ())
        أرجع خطأ;

      بینښه format = (بنسټ+_.subtable).u.format.v;
      إذا (format != 4 && format != 12 && format != 14
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
	  && format != 15
#د_که_پای
	 ) استمر;

      ثابت hb_سېټ_t* unicodes_سېټ = unicodes_cache->سېټ_for (&_, local_unicodes_cache);

      إذا (!drop_format_4 && format == 4)
      {
        c->copy (_, + it | hb_filter (*unicodes_سېټ, hb_لومړی), 4u, بنسټ, plan, &format4objidx);
        إذا (c->دننه_error () && c->only_ډکېدنه ())
        {
          // cmap4 overflowed, reset and retry serialization without format 4 subtables.
          c->revert (snap);
          أرجع serialize (c, it,
                            encodingrec_iter,
                            بنسټ,
                            plan,
                            ریښتیا,
			    سرچینه_جدول_اوږدتیا);
        }
      }

      وإلا إذا (format == 12)
      {
        إذا (_can_drop (_,
                       *unicodes_سېټ,
                       بنسټ,
                       *unicodes_cache,
                       local_unicodes_cache,
                       + it | hb_map (hb_لومړی), encodingrec_iter))
          استمر;
        c->copy (_, + it | hb_filter (*unicodes_سېټ, hb_لومړی), 12u, بنسټ, plan, &format12objidx);
      }
      وإلا إذا (format == 14) c->copy (_, it, 14u, بنسټ, plan, &format14objidx);
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
      وإلا إذا (format == 15) c->copy (_, it, 15u, بنسټ, plan, &format15objidx);
#د_که_پای
    }
        بینښه اوږدتیا = c->اوږدتیا ();
        بینښه شته = اوږدتیا > cmap::min_اندازه ? اوږدتیا - cmap::min_اندازه : 0;
        c->check_ټاکل(هذا->encoding_ریکارډ.len,
                        شته / Encoding_ریکارډ::static_اندازه,
                        HB_SERIALIZE_ERROR_INT_ډکېدنه);

    // Fail if format 4 was dropped and there is no cmap12.
    أرجع !drop_format_4 || format12objidx;
  }

  کينډۍ<ډول_نوم Iterator, ډول_نوم Encoding_ریکارډ_Iterator,
      hb_requires (hb_is_iterator (Iterator)),
      hb_requires (hb_is_iterator (Encoding_ریکارډ_Iterator))>
  منطقي _can_drop (ثابت Encoding_ریکارډ& cmap12,
                  ثابت hb_سېټ_t& cmap12_unicodes,
                  ثابت فراغ* بنسټ,
                  ثابت SubtableUnicodesCache& unicodes_cache,
                  SubtableUnicodesCache& local_unicodes_cache,
                  Iterator subset_unicodes,
                  Encoding_ریکارډ_Iterator encoding_ریکارډونه)
  {
    لكل (خپلکاری cp : + subset_unicodes | hb_filter (cmap12_unicodes))
    {
      إذا (cp >= 0x10000) أرجع خطأ;
    }

    بینښه موخه_platform;
    بینښه موخه_encoding;
    بینښه موخه_language = (بنسټ+cmap12.subtable).get_language ();

    إذا (cmap12.platformID == 0 && cmap12.encodingID == 4)
    {
      موخه_platform = 0;
      موخه_encoding = 3;
    } وإلا إذا (cmap12.platformID == 3 && cmap12.encodingID == 10) {
      موخه_platform = 3;
      موخه_encoding = 1;
    } وإلا {
      أرجع خطأ;
    }

    لكل (ثابت خپلکاری& _ : encoding_ریکارډونه)
    {
      إذا (_.platformID != موخه_platform
          || _.encodingID != موخه_encoding
          || (بنسټ+_.subtable).get_language() != موخه_language)
        استمر;

      ثابت hb_سېټ_t* sibling_unicodes = unicodes_cache.سېټ_for (&_, local_unicodes_cache);

      خپلکاری cmap12 = + subset_unicodes | hb_filter (cmap12_unicodes);
      خپلکاری sibling = + subset_unicodes | hb_filter (*sibling_unicodes);
      لكل (; cmap12 && sibling; cmap12++, sibling++)
      {
        بینښه a = *cmap12;
        بینښه b = *sibling;
        إذا (a != b) أرجع خطأ;
      }

      أرجع !cmap12 && !sibling;
    }

    أرجع خطأ;
  }

  فراغ تړون_glyphs (ثابت hb_سېټ_t      *unicodes,
		       hb_سېټ_t            *glyphset) ثابت
  {
    + hb_iter (encoding_ریکارډ)
    | hb_map (&Encoding_ریکارډ::subtable)
    | hb_map (hb_جمع_کړه (هذا))
    | hb_filter (&CmapSubtable::is_variation_selector)
    | hb_apply ([=] (ثابت CmapSubtable& _) { _.تړون_glyphs (unicodes, glyphset); })
    ;
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  { أرجع subset (c, خطأ); }

  منطقي subset (hb_subset_context_t *c, منطقي is_dmap) ثابت
  {
    TRACE_SUBSET (هذا);

    cmap *cmap_اولي = c->serializer->پیل_embed<cmap> ();

    خپلکاری encodingrec_iter =
    + hb_iter (encoding_ریکارډ)
    | hb_filter ([&](ثابت Encoding_ریکارډ& _) {
      أرجع cmap::filter_encoding_ریکارډونه_for_subset (هذا, _);
    })
    ;

    إذا (unlikely (!encodingrec_iter.len ())) return_trace (خطأ);

    ثابت Encoding_ریکارډ *unicode_bmp= تش_اشاره, *unicode_ucs4 = تش_اشاره, *ms_bmp = تش_اشاره, *ms_ucs4 = تش_اشاره;
    منطقي has_format12 = خطأ;

    لكل (ثابت Encoding_ریکارډ& _ : encodingrec_iter)
    {
      بینښه format = (هذا + _.subtable).u.format.v;
      إذا (format == 12) has_format12 = ریښتیا;

      ثابت Encoding_ریکارډ *جدول = std::addressof (_);
      إذا      (_.platformID == 0 && _.encodingID ==  3) unicode_bmp = جدول;
      وإلا إذا (_.platformID == 0 && _.encodingID ==  4) unicode_ucs4 = جدول;
      وإلا إذا (_.platformID == 3 && _.encodingID ==  1) ms_bmp = جدول;
      وإلا إذا (_.platformID == 3 && _.encodingID == 10) ms_ucs4 = جدول;
    }

    إذا (unlikely (!is_dmap && !has_format12 && !unicode_bmp && !ms_bmp)) return_trace (خطأ);
    إذا (unlikely (!is_dmap && has_format12 && (!unicode_ucs4 && !ms_ucs4))) return_trace (خطأ);

    خپلکاری it =
    + c->plan->unicode_منزل_new_gid_list.iter ()
    | hb_filter ([&] (ثابت hb_codepoint_pair_t _)
		 { أرجع (_.second != HB_MAP_ارزښت_INVALID); })
    ;

    return_trace (cmap_اولي->serialize (c->serializer,
                                         it,
                                         encodingrec_iter,
                                         هذا,
                                         c->plan,
					 خطأ,
					 c->سرچینه_blob->اوږدتیا));
  }

  ثابت CmapSubtable *پیدا_کړه_غوره_subtable (منطقي *نښه = تش_اشاره,
					  منطقي *mac = تش_اشاره,
					  منطقي *macroman = تش_اشاره) ثابت
  {
    إذا (نښه) *نښه = خطأ;
    إذا (mac) *mac = خطأ;
    إذا (macroman) *macroman = خطأ;

    ثابت CmapSubtable *subtable;

    /* Symbol subtable.
     * Prefer symbol if available.
     * https://github.com/harfbuzz/harfbuzz/issues/1918 */
    إذا ((subtable = هذا->پیدا_کړه_subtable (3, 0)))
    {
      إذا (نښه) *نښه = ریښتیا;
      أرجع subtable;
    }

    /* 32-bit subtables. */
    إذا ((subtable = هذا->پیدا_کړه_subtable (3, 10))) أرجع subtable;
    إذا ((subtable = هذا->پیدا_کړه_subtable (0, 6))) أرجع subtable;
    إذا ((subtable = هذا->پیدا_کړه_subtable (0, 4))) أرجع subtable;

    /* 16-bit subtables. */
    إذا ((subtable = هذا->پیدا_کړه_subtable (3, 1))) أرجع subtable;
    إذا ((subtable = هذا->پیدا_کړه_subtable (0, 3))) أرجع subtable;
    إذا ((subtable = هذا->پیدا_کړه_subtable (0, 2))) أرجع subtable;
    إذا ((subtable = هذا->پیدا_کړه_subtable (0, 1))) أرجع subtable;
    إذا ((subtable = هذا->پیدا_کړه_subtable (0, 0))) أرجع subtable;

    /* MacRoman subtable. */
    إذا ((subtable = هذا->پیدا_کړه_subtable (1, 0)))
    {
      إذا (mac) *mac = ریښتیا;
      إذا (macroman) *macroman = ریښتیا;
      أرجع subtable;
    }
    /* Any other Mac subtable; we just map ASCII for these. */
    إذا ((subtable = هذا->پیدا_کړه_subtable (1, 0xFFFF)))
    {
      إذا (mac) *mac = ریښتیا;
      أرجع subtable;
    }

    /* Meh. */
    أرجع &Null (CmapSubtable);
  }

  بنية accelerator_t
  {
    کارول cache_t = hb_cache_t<21, 19>;
    static_assert (حجم (cache_t) == 1024, "");

    accelerator_t (hb_face_t *face, hb_نښان_t جدول_نښان_2 = cmap::جدول_نښان)
    {
      هذا->جدول = hb_sanitize_context_t ().reference_جدول<cmap> (face, جدول_نښان_2);
      إذا (!هذا->جدول.get_اوږدتیا ())
	أرجع;

      منطقي نښه, mac, macroman;
      هذا->subtable = جدول->پیدا_کړه_غوره_subtable (&نښه, &mac, &macroman);
      هذا->subtable_uvs = &Null (CmapSubtable);
      {
	ثابت CmapSubtable *st = جدول->پیدا_کړه_variation_selector_subtable ();
	إذا (st)
	  subtable_uvs = st;
      }

#که_نه_تعریف_شوی HB_NO_OT_FONT_CMAP_CACHE
      cache = (cache_t *) hb_malloc (حجم (cache_t));
      إذا (cache)
	جديد (cache) cache_t ();
      وإلا
        أرجع; // Such that get_glyph_funcZ remains null.
#د_که_پای

      هذا->get_glyph_معلومات = subtable;
#که_نه_تعریف_شوی HB_NO_CMAP_LEGACY_SUBTABLES
      منطقي is_format4 = subtable->u.format.v == 4;
      خپلکاری سېټ_format4_getter = [هذا] (منطقي (*func) (ثابت فراغ *,
						      hb_codepoint_t,
						      hb_codepoint_t *))
      {
	هذا->format4_accel.init (&هذا->subtable->u.format4,
				  get_subtable_معلومات_اندازه (هذا->subtable));
	هذا->get_glyph_معلومات = &هذا->format4_accel;
	هذا->get_glyph_funcZ = func;
      };
      إذا (unlikely (نښه))
      {
	بدل ((بینښه) face->جدول.OS2->get_font_پاڼه ()) {
	حالة OS2::font_پاڼه_t::FONT_پاڼه_NONE:
	  إذا (is_format4)
	    سېټ_format4_getter (get_glyph_from_نښه<CmapSubtableFormat4::accelerator_t, hb_نښه_pua_map>);
	  وإلا
	    هذا->get_glyph_funcZ = get_glyph_from_نښه<CmapSubtable, hb_نښه_pua_map>;
	  اكسر;
#که_نه_تعریف_شوی HB_NO_OT_SHAPER_ARABIC_FALLBACK
	حالة OS2::font_پاڼه_t::FONT_پاڼه_SIMP_ARABIC:
	  إذا (is_format4)
	    سېټ_format4_getter (get_glyph_from_نښه<CmapSubtableFormat4::accelerator_t, _hb_arabic_pua_simp_map>);
	  وإلا
	    هذا->get_glyph_funcZ = get_glyph_from_نښه<CmapSubtable, _hb_arabic_pua_simp_map>;
	  اكسر;
	حالة OS2::font_پاڼه_t::FONT_پاڼه_TRAD_ARABIC:
	  إذا (is_format4)
	    سېټ_format4_getter (get_glyph_from_نښه<CmapSubtableFormat4::accelerator_t, _hb_arabic_pua_trad_map>);
	  وإلا
	    هذا->get_glyph_funcZ = get_glyph_from_نښه<CmapSubtable, _hb_arabic_pua_trad_map>;
	  اكسر;
#د_که_پای
	افتراضي:
	  إذا (is_format4)
	    سېټ_format4_getter (get_glyph_from<CmapSubtableFormat4::accelerator_t>);
	  وإلا
	    هذا->get_glyph_funcZ = get_glyph_from<CmapSubtable>;
	  اكسر;
	}
      }
      وإلا إذا (unlikely (macroman))
      {
	إذا (is_format4)
	  سېټ_format4_getter (get_glyph_from_macroman<CmapSubtableFormat4::accelerator_t>);
	وإلا
	  هذا->get_glyph_funcZ = get_glyph_from_macroman<CmapSubtable>;
      }
      وإلا إذا (unlikely (mac))
      {
	إذا (is_format4)
	  سېټ_format4_getter (get_glyph_from_ascii<CmapSubtableFormat4::accelerator_t>);
	وإلا
	  هذا->get_glyph_funcZ = get_glyph_from_ascii<CmapSubtable>;
      }
      وإلا
#د_که_پای
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
				      get_subtable_معلومات_اندازه (subtable));
	    هذا->get_glyph_معلومات = &هذا->format4_accel;
	    هذا->get_glyph_funcZ = هذا->format4_accel.get_glyph_func;
	    اكسر;
	  }
	}
      }
    }
    ~accelerator_t ()
    {
#که_نه_تعریف_شوی HB_NO_OT_FONT_CMAP_CACHE
      hb_free (cache);
#د_که_پای
      جدول.destroy ();
    }

    مضمن منطقي _cached_get (hb_codepoint_t unicode,
			     hb_codepoint_t *glyph) ثابت
    {
#که_نه_تعریف_شوی HB_NO_OT_FONT_CMAP_CACHE
      // cache is always non-null if we have a get_glyph_funcZ
      بینښه v;
      إذا (cache->get (unicode, &v))
      {
        *glyph = v;
	أرجع ریښتیا;
      }
#د_که_پای
      منطقي ret  = هذا->get_glyph_funcZ (هذا->get_glyph_معلومات, unicode, glyph);

#که_نه_تعریف_شوی HB_NO_OT_FONT_CMAP_CACHE
      إذا (ret)
        cache->سېټ (unicode, *glyph);
#د_که_پای

      أرجع ret;
    }

    منطقي get_nominal_glyph (hb_codepoint_t  unicode,
			    hb_codepoint_t *glyph) ثابت
    {
      إذا (unlikely (!هذا->get_glyph_funcZ)) أرجع خطأ;
      أرجع _cached_get (unicode, glyph);
    }

    بینښه صحيح get_nominal_glyphs (بینښه صحيح شمېر,
				     ثابت hb_codepoint_t *لومړی_unicode,
				     بینښه صحيح unicode_stride,
				     hb_codepoint_t *لومړی_glyph,
				     بینښه صحيح glyph_stride) ثابت
    {
      إذا (unlikely (!هذا->get_glyph_funcZ)) أرجع 0;

      بینښه صحيح بشپړ;
      لكل (بشپړ = 0;
	   بشپړ < شمېر && _cached_get (*لومړی_unicode, لومړی_glyph);
	   بشپړ++)
      {
	لومړی_unicode = &Struct_At_دننه_واټن_Unaligned<hb_codepoint_t> (لومړی_unicode, unicode_stride);
	لومړی_glyph = &Struct_At_دننه_واټن_Unaligned<hb_codepoint_t> (لومړی_glyph, glyph_stride);
      }
      أرجع بشپړ;
    }

    منطقي get_variation_glyph (hb_codepoint_t  unicode,
			      hb_codepoint_t  variation_selector,
			      hb_codepoint_t *glyph) ثابت
    {
      بدل (get_glyph_variant (unicode, variation_selector, glyph))
      {
	حالة GLYPH_VARIANT_NOT_وموندل:	أرجع خطأ;
	حالة GLYPH_VARIANT_وموندل:	أرجع ریښتیا;
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

    منطقي has_معلومات () ثابت { أرجع جدول.get_اوږدتیا (); }

    فراغ collect_unicodes (hb_سېټ_t *بهر, بینښه صحيح عدد_glyphs) ثابت
    { subtable->collect_unicodes (بهر, عدد_glyphs, get_subtable_معلومات_اندازه (subtable)); }
    فراغ collect_mapping (hb_سېټ_t *unicodes, hb_map_t *mapping,
			  بینښه عدد_glyphs = UINT_MAX) ثابت
    { subtable->collect_mapping (unicodes, mapping, عدد_glyphs, get_subtable_معلومات_اندازه (subtable)); }
    فراغ collect_variation_selectors (hb_سېټ_t *بهر) ثابت
    { subtable_uvs->collect_variation_selectors (بهر); }
    فراغ collect_variation_unicodes (hb_codepoint_t variation_selector,
				     hb_سېټ_t *بهر) ثابت
    { subtable_uvs->collect_variation_unicodes (variation_selector, بهر); }

    محمي:
    تعريفنوع منطقي (*hb_cmap_get_glyph_func_t) (ثابت فراغ *obj,
					      hb_codepoint_t codepoint,
					      hb_codepoint_t *glyph);
    تعريفنوع uint16_t (*hb_pua_remap_func_t) (بینښه);

    کينډۍ <ډول_نوم Type>
    HB_دننی ساكن منطقي get_glyph_from (ثابت فراغ *obj,
					    hb_codepoint_t codepoint,
					    hb_codepoint_t *glyph)
    {
      ثابت Type *typed_obj = (ثابت Type *) obj;
      أرجع typed_obj->get_glyph (codepoint, glyph);
    }

    کينډۍ <ډول_نوم Type, hb_pua_remap_func_t remap>
    HB_دننی ساكن منطقي get_glyph_from_نښه (ثابت فراغ *obj,
						   hb_codepoint_t codepoint,
						   hb_codepoint_t *glyph)
    {
      ثابت Type *typed_obj = (ثابت Type *) obj;
      إذا (likely (typed_obj->get_glyph (codepoint, glyph)))
	أرجع ریښتیا;

      إذا (hb_codepoint_t c = remap (codepoint))
	أرجع typed_obj->get_glyph (c, glyph);

      أرجع خطأ;
    }

    کينډۍ <ډول_نوم Type>
    HB_دننی ساكن منطقي get_glyph_from_ascii (ثابت فراغ *obj,
						  hb_codepoint_t codepoint,
						  hb_codepoint_t *glyph)
    {
      ثابت Type *typed_obj = (ثابت Type *) obj;
      أرجع codepoint < 0x80 && typed_obj->get_glyph (codepoint, glyph);
    }

    کينډۍ <ډول_نوم Type>
    HB_دننی ساكن منطقي get_glyph_from_macroman (ثابت فراغ *obj,
						     hb_codepoint_t codepoint,
						     hb_codepoint_t *glyph)
    {
      إذا (get_glyph_from_ascii<Type> (obj, codepoint, glyph))
	أرجع ریښتیا;

      ثابت Type *typed_obj = (ثابت Type *) obj;
      بینښه c = unicode_منزل_macroman (codepoint);
      أرجع c && typed_obj->get_glyph (c, glyph);
    }

    بینښه صحيح get_subtable_معلومات_اندازه (ثابت CmapSubtable *subtable) ثابت
    {
      بینښه صحيح جدول_اوږدتیا = هذا->جدول.get_اوږدتیا ();
      uintptr_t جدول_پیل = (uintptr_t) (ثابت فراغ *) هذا->جدول.get ();
      uintptr_t subtable_addr = (uintptr_t) (ثابت فراغ *) subtable;
      إذا (unlikely (subtable_addr < جدول_پیل))
	أرجع 0;

      uintptr_t subtable_دننه_واټن = subtable_addr - جدول_پیل;
      إذا (unlikely (subtable_دننه_واټن >= جدول_اوږدتیا))
	أرجع 0;

      أرجع جدول_اوږدتیا - (بینښه صحيح) subtable_دننه_واټن;
    }

    خاص:
    hb_nonnull_ptr_t<ثابت CmapSubtable> subtable;
    hb_nonnull_ptr_t<ثابت CmapSubtable> subtable_uvs;

    hb_cmap_get_glyph_func_t get_glyph_funcZ = تش_اشاره;
    ثابت فراغ *get_glyph_معلومات = تش_اشاره;

    CmapSubtableFormat4::accelerator_t format4_accel;

#که_نه_تعریف_شوی HB_NO_OT_FONT_CMAP_CACHE
    cache_t *cache = تش_اشاره;
#د_که_پای

    عام:
    hb_blob_ptr_t<cmap> جدول;
  };

  محمي:

  ثابت CmapSubtable *پیدا_کړه_subtable (بینښه صحيح platform_id,
				     بینښه صحيح encoding_id) ثابت
  {
    Encoding_ریکارډ key;
    key.platformID = platform_id;
    key.encodingID = encoding_id;

    ثابت Encoding_ریکارډ &پایله = encoding_ریکارډ.bsearch (key);
    إذا (!پایله.subtable)
      أرجع تش_اشاره;

    أرجع &(هذا+پایله.subtable);
  }

  ثابت CmapSubtable *پیدا_کړه_variation_selector_subtable () ثابت
  {
    ثابت CmapSubtable *format14 = تش_اشاره;
    لكل (ثابت Encoding_ریکارډ& ریکارډ : encoding_ریکارډ)
    {
      إذا (ریکارډ.platformID != 0 || ریکارډ.encodingID != 5)
	استمر;

      ثابت CmapSubtable *subtable = &(هذا+ریکارډ.subtable);
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
      إذا (subtable->u.format.v == 15)
	أرجع subtable;
#د_که_پای
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
		  likely (نسخه == 0) &&
		  encoding_ریکارډ.sanitize (c, هذا));
  }

 خاص:

  ساكن منطقي filter_encoding_ریکارډونه_for_subset(ثابت cmap* cmap,
                                                 ثابت Encoding_ریکارډ& _)
  {
    أرجع
        (_.platformID == 0 && _.encodingID == 3) ||
        (_.platformID == 0 && _.encodingID == 4) ||
        (_.platformID == 3 && _.encodingID == 1) ||
        (_.platformID == 3 && _.encodingID == 10) ||
        (cmap + _.subtable).is_variation_selector ();
  }

  محمي:
  HBUINT16	نسخه;	/* Table version number (0). */
  Sorted_Array16_د<Encoding_ریکارډ>
		encoding_ریکارډ;	/* Encoding tables. */
  عام:
  DEFINE_اندازه_ARRAY (4, encoding_ریکارډ);
};

بنية DMAP : cmap
{
  ساكن constexpr hb_نښان_t جدول_نښان = HB_OT_نښان_DMAP;

  منطقي subset (hb_subset_context_t *c) ثابت
  { أرجع cmap::subset (c, ریښتیا); }
};

بنية cmap_accelerator_t
{
  cmap_accelerator_t (hb_face_t *face) :
    dmap (face, HB_OT_نښان_DMAP),
    بنسټ (face)
  {}

  منطقي get_nominal_glyph (hb_codepoint_t  unicode,
			  hb_codepoint_t *glyph) ثابت
  {
    أرجع dmap.get_nominal_glyph (unicode, glyph) ||
	   بنسټ.get_nominal_glyph (unicode, glyph);
  }

  بینښه صحيح get_nominal_glyphs (بینښه صحيح شمېر,
				   ثابت hb_codepoint_t *لومړی_unicode,
				   بینښه صحيح unicode_stride,
				   hb_codepoint_t *لومړی_glyph,
				   بینښه صحيح glyph_stride) ثابت
  {
    إذا (likely (!dmap.has_معلومات ()))
      أرجع بنسټ.get_nominal_glyphs (شمېر,
				      لومړی_unicode, unicode_stride,
				      لومړی_glyph, glyph_stride);

    بینښه صحيح بشپړ;
    لكل (بشپړ = 0;
	 بشپړ < شمېر && get_nominal_glyph (*لومړی_unicode, لومړی_glyph);
	 بشپړ++)
    {
      لومړی_unicode = &Struct_At_دننه_واټن_Unaligned<hb_codepoint_t> (لومړی_unicode, unicode_stride);
      لومړی_glyph = &Struct_At_دننه_واټن_Unaligned<hb_codepoint_t> (لومړی_glyph, glyph_stride);
    }
    أرجع بشپړ;
  }

  منطقي get_variation_glyph (hb_codepoint_t  unicode,
			    hb_codepoint_t  variation_selector,
			    hb_codepoint_t *glyph) ثابت
  {
    glyph_variant_t پایله = dmap.get_glyph_variant (unicode,
						     variation_selector,
						     glyph);
    إذا (پایله == GLYPH_VARIANT_NOT_وموندل)
      پایله = بنسټ.get_glyph_variant (unicode, variation_selector, glyph);

    بدل (پایله)
    {
      حالة GLYPH_VARIANT_NOT_وموندل: أرجع خطأ;
      حالة GLYPH_VARIANT_وموندل: أرجع ریښتیا;
      حالة GLYPH_VARIANT_USE_DEFAULT: اكسر;
    }

    أرجع get_nominal_glyph (unicode, glyph);
  }

  فراغ collect_unicodes (hb_سېټ_t *بهر, بینښه صحيح عدد_glyphs) ثابت
  {
    بنسټ.collect_unicodes (بهر, عدد_glyphs);
    dmap.collect_unicodes (بهر, عدد_glyphs);
  }

  فراغ collect_mapping (hb_سېټ_t *unicodes, hb_map_t *mapping,
			بینښه عدد_glyphs = UINT_MAX) ثابت
  {
    بنسټ.collect_mapping (unicodes, mapping, عدد_glyphs);
    dmap.collect_mapping (unicodes, mapping, عدد_glyphs);
  }

  فراغ collect_variation_selectors (hb_سېټ_t *بهر) ثابت
  {
    بنسټ.collect_variation_selectors (بهر);
    dmap.collect_variation_selectors (بهر);
  }

  فراغ collect_variation_unicodes (hb_codepoint_t variation_selector,
				   hb_سېټ_t *بهر) ثابت
  {
    بنسټ.collect_variation_unicodes (variation_selector, بهر);
    dmap.collect_variation_unicodes (variation_selector, بهر);
  }

  فراغ تړون_glyphs (ثابت hb_سېټ_t *unicodes,
		       hb_سېټ_t       *glyphset) ثابت
  {
    إذا (likely (!dmap.has_معلومات ()))
    {
      بنسټ.جدول->تړون_glyphs (unicodes, glyphset);
      أرجع;
    }

    hb_سېټ_t variation_selectors;
    collect_variation_selectors (&variation_selectors);
    لكل (hb_codepoint_t variation_selector : variation_selectors)
    {
      إذا (!unicodes->has (variation_selector))
	استمر;

      hb_سېټ_t variation_unicodes;
      collect_variation_unicodes (variation_selector, &variation_unicodes);
      لكل (hb_codepoint_t unicode : variation_unicodes)
      {
	إذا (!unicodes->has (unicode))
	  استمر;

	hb_codepoint_t glyph;
	إذا (get_variation_glyph (unicode, variation_selector, &glyph))
	  glyphset->جمع_کړه (glyph);
      }
    }
  }

  cmap::accelerator_t dmap;
  cmap::accelerator_t بنسټ;
};

} /* namespace OT */


#د_که_پای /* HB_OT_CMAP_TABLE_HH */
