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

#اگر_تعریف_نشده HB_OT_CMAP_جدول_HH
#تعریف_کن HB_OT_CMAP_جدول_HH

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
#تعریف_کن HB_OT_برچسب_cmap HB_برچسب('c','m','a','p')
#تعریف_کن HB_OT_برچسب_DMAP HB_برچسب('D','M','A','P')

نطاق OT {

ساكن مضمن uint8_t unicode_مقصد_macroman (hb_codepoint_t u)
{
  ساكن ثابت بنية unicode_مقصد_macroman_t
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
  خودکار *c = hb_bsearch (u, mapping, ARRAY_درازی (mapping), حجم (mapping[0]),
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
    أرجع درست;
  }

  بیعلامت get_language () ثابت
  {
    أرجع language;
  }

  فراغ collect_unicodes (hb_مجموعه_t *بیرون) ثابت
  {
    لكل (بیعلامت صحيح i = 0; i < 256; i++)
      إذا (glyphIdArray[i])
	بیرون->جمع_کن (i);
  }

  فراغ collect_mapping (hb_مجموعه_t *unicodes, /*بیرون*/
			hb_map_t *mapping /*بیرون*/) ثابت
  {
    لكل (بیعلامت i = 0; i < 256; i++)
      إذا (glyphIdArray[i])
      {
	hb_codepoint_t glyph = glyphIdArray[i];
	unicodes->جمع_کن (i);
	mapping->مجموعه (i, glyph);
      }
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  محمي:
  HBUINT16	format;		/* Format number is set to 0. */
  HBUINT16	درازی;		/* Byte length of this subtable. */
  HBUINT16	language;	/* Ignore. */
  HBUINT8	glyphIdArray[256];/* An array that maps character
				 * code to glyph index values. */
  عام:
  DEFINE_اندازه_STATIC (6 + 256);
};

بنية CmapSubtableFormat4
{
  قالب<نام_نوع Iterator,
      نام_نوع Writer,
	   hb_requires (hb_is_iterator (Iterator))>
  فراغ مقصد_ranges (Iterator it, Writer& range_writer)
  {
    hb_codepoint_t شروع_cp = 0, prev_پیاپی_شروع_cp = 0, پیاپی_شروع_cp = 0, انتها_cp = 0, آخرین_gid = 0;
    صحيح پیاپی_درازی = 0 , delta = 0, prev_delta = 0;

    تعداد {
      اولی_SUB_RANGE,
      FOLLOWING_SUB_RANGE,
    } نما;

    طالما (it) {
      // Start a new range
      {
        ثابت خودکار& pair = *it;
        شروع_cp = pair.اولی;
        prev_پیاپی_شروع_cp = شروع_cp;
        پیاپی_شروع_cp = شروع_cp;
        انتها_cp = شروع_cp;
        آخرین_gid = pair.second;
        پیاپی_درازی = 1;
        prev_delta = 0;
      }

      delta = آخرین_gid - شروع_cp;
      نما = اولی_SUB_RANGE;
      it++;

      طالما (it) {
        // Process range
        ثابت خودکار& pair = *it;
        hb_codepoint_t بعدی_cp = pair.اولی;
        hb_codepoint_t بعدی_gid = pair.second;
        إذا (بعدی_cp != انتها_cp + 1) {
          // Current range is over, stop processing.
          اكسر;
        }

        إذا (بعدی_gid == آخرین_gid + 1) {
          // The current run continues.
          انتها_cp = بعدی_cp;
          پیاپی_درازی++;
          آخرین_gid = بعدی_gid;
          it++;
          استمر;
        }

        // A new run is starting, decide if we want to commit the current run.
        صحيح تقسیم_هزینه = (نما == اولی_SUB_RANGE) ? 8 : 16;
        صحيح پیاپی_هزینه = پیاپی_درازی * 2;
        إذا (پیاپی_هزینه >= تقسیم_هزینه) {
          commit_current_range(شروع_cp,
                               prev_پیاپی_شروع_cp,
                               پیاپی_شروع_cp,
                               انتها_cp,
                               delta,
                               prev_delta,
                               تقسیم_هزینه,
                               range_writer);
          شروع_cp = بعدی_cp;
        }

        // Start the new run
        نما = FOLLOWING_SUB_RANGE;
        prev_پیاپی_شروع_cp = پیاپی_شروع_cp;
        پیاپی_شروع_cp = بعدی_cp;
        انتها_cp = بعدی_cp;
        prev_delta = delta;
        delta = بعدی_gid - پیاپی_شروع_cp;
        پیاپی_درازی = 1;
        آخرین_gid = بعدی_gid;
        it++;
      }

      // Finalize range
      commit_current_range (شروع_cp,
                            prev_پیاپی_شروع_cp,
                            پیاپی_شروع_cp,
                            انتها_cp,
                            delta,
                            prev_delta,
                            8,
                            range_writer);
    }

    إذا (likely (انتها_cp != 0xFFFF)) {
      range_writer (0xFFFF, 0xFFFF, 1);
    }
  }

  /*
   * Writes the current range as either one or two ranges depending on what is most efficient.
   */
  قالب<نام_نوع Writer>
  فراغ commit_current_range (hb_codepoint_t شروع,
                             hb_codepoint_t prev_پیاپی_شروع,
                             hb_codepoint_t پیاپی_شروع,
                             hb_codepoint_t انتها,
                             صحيح پیاپی_delta,
                             صحيح قبلی_پیاپی_delta,
                             صحيح تقسیم_هزینه,
                             Writer& range_writer) {
    منطقي should_تقسیم = خطأ;
    إذا (شروع < پیاپی_شروع && پیاپی_شروع < انتها) {
      صحيح پیاپی_هزینه = (انتها - پیاپی_شروع + 1) * 2;
      إذا (پیاپی_هزینه >= تقسیم_هزینه) {
        should_تقسیم = درست;
      }
    }

    // A zero delta is also the marker for ranges that need a
    // glyphIdArray; serialize_rangeoffset_glyid() tells the two cases
    // apart by re-checking the mapping.
    إذا (should_تقسیم) {
      إذا (شروع == prev_پیاپی_شروع)
        range_writer (شروع, پیاپی_شروع - 1, قبلی_پیاپی_delta);
      وإلا
        range_writer (شروع, پیاپی_شروع - 1, 0);
      range_writer (پیاپی_شروع, انتها, پیاپی_delta);
      أرجع;
    }


    إذا (شروع == پیاپی_شروع) {
      // Range is only a run
      range_writer (شروع, انتها, پیاپی_delta);
      أرجع;
    }

    // Write only a single non-run range.
    range_writer (شروع, انتها, 0);
  }

  قالب<نام_نوع Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  بیعلامت serialize_پیدا_کن_segcount (Iterator it) {
    بنية Counter {
      بیعلامت segcount = 0;

      فراغ عملگر() (hb_codepoint_t شروع,
                       hb_codepoint_t انتها,
                       صحيح delta) {
        segcount++;
      }
    } counter;

    مقصد_ranges (+it, counter);
    أرجع counter.segcount;
  }


  قالب<نام_نوع Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  منطقي serialize_شروع_انتها_delta_arrays (hb_serialize_context_t *c,
                                         Iterator it,
                                         صحيح segcount)
  {
    بنية Writer {
      hb_serialize_context_t *serializer_;
      HBUINT16* انتها_کد;
      HBUINT16* شروع_کد;
      HBINT16* id_delta_;
      صحيح شاخص_3;

      Writer(hb_serialize_context_t *serializer)
          : serializer_(serializer),
            انتها_کد(اشاره_تهی),
            شروع_کد(اشاره_تهی),
            id_delta_(اشاره_تهی),
            شاخص_3 (0) {}
      فراغ عملگر() (hb_codepoint_t شروع,
                       hb_codepoint_t انتها,
                       صحيح delta) {
        شروع_کد[شاخص_3] = شروع;
        انتها_کد[شاخص_3] = انتها;
        id_delta_[شاخص_3] = delta;
        شاخص_3++;
      }
    } writer(c);

    writer.انتها_کد = c->allocate_اندازه<HBUINT16> (HBUINT16::static_اندازه * segcount, خطأ);
    (فراغ) c->allocate_اندازه<HBUINT16> (2); // padding
    writer.شروع_کد = c->allocate_اندازه<HBUINT16> (HBUINT16::static_اندازه * segcount, خطأ);
    writer.id_delta_ = c->allocate_اندازه<HBINT16> (HBINT16::static_اندازه * segcount, خطأ);

    إذا (unlikely (!writer.انتها_کد || !writer.شروع_کد || !writer.id_delta_)) أرجع خطأ;

    مقصد_ranges (+it, writer);
    أرجع درست;
  }

  قالب<نام_نوع Iterator,
          hb_requires (hb_is_iterator (Iterator))>
  HBUINT16* serialize_rangeoffset_glyid (hb_serialize_context_t *c,
                                         Iterator it,
					 HBUINT16 *انتها_کد_2,
					 HBUINT16 *شروع_کد_2,
					 HBINT16 *idDelta,
					 بیعلامت segcount)
  {
    hb_map_t cp_مقصد_gid { it };

    HBUINT16 *id_Range_جابه_جایی = c->allocate_اندازه<HBUINT16> (HBUINT16::static_اندازه * segcount);
    إذا (unlikely (!c->check_success (id_Range_جابه_جایی))) أرجع اشاره_تهی;
    إذا (unlikely ((حرف *)id_Range_جابه_جایی - (حرف *)idDelta != (صحيح) segcount * (صحيح) HBINT16::static_اندازه)) أرجع اشاره_تهی;

    لكل (بیعلامت i : hb_range (segcount))
    {
      /* A non-zero delta is always a real run. A zero delta is
       * ambiguous: identity mapping, or the marker for a range that
       * needs a glyphIdArray. Only zero deltas need a check. */
      صحيح delta = idDelta[i];
      إذا (delta)
	استمر;
      منطقي use_delta = درست;
      لكل (hb_codepoint_t cp = شروع_کد_2[i]; cp <= انتها_کد_2[i]; cp++)
      {
	hb_codepoint_t gid = cp_مقصد_gid.get (cp);
	إذا (gid == HB_MAP_مقدار_INVALID)
	  gid = 0;
	إذا (gid != cp)
	{
	  use_delta = خطأ;
	  اكسر;
	}
      }
      إذا (use_delta)
	استمر;

      id_Range_جابه_جایی[i] = 2 * (c->شروع_embed<HBUINT16> () - id_Range_جابه_جایی - i);
      لكل (hb_codepoint_t cp = شروع_کد_2[i]; cp <= انتها_کد_2[i]; cp++)
      {
        HBUINT16 gid;
        gid = cp_مقصد_gid[cp];
        c->copy<HBUINT16> (gid);
      }
    }

    أرجع id_Range_جابه_جایی;
  }

  قالب<نام_نوع Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  فراغ serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    خودکار format4_iter =
    + it
    | hb_filter ([&] (ثابت hb_codepoint_pair_t _)
		 { أرجع _.اولی <= 0xFFFF; })
    ;

    إذا (!format4_iter) أرجع;

    بیعلامت جدول_initpos = c->درازی ();
    إذا (unlikely (!c->extend_min (هذا))) أرجع;
    هذا->format = 4;

    hb_وکتور_t<hb_codepoint_pair_t> cp_مقصد_gid {
      format4_iter
    };

    //serialize endCode[], startCode[], idDelta[]
    HBUINT16* انتها_کد_2 = c->شروع_embed<HBUINT16> ();
    بیعلامت segcount = serialize_پیدا_کن_segcount (cp_مقصد_gid.iter());
    إذا (unlikely (!serialize_شروع_انتها_delta_arrays (c, cp_مقصد_gid.iter(), segcount)))
      أرجع;

    HBUINT16 *شروع_کد_2 = انتها_کد_2 + segcount + 1;
    HBINT16 *idDelta = ((HBINT16*)شروع_کد_2) + segcount;

    HBUINT16 *id_Range_جابه_جایی = serialize_rangeoffset_glyid (c,
                                                           cp_مقصد_gid.iter (),
                                                           انتها_کد_2,
                                                           شروع_کد_2,
                                                           idDelta,
                                                           segcount);
    إذا (unlikely (!c->check_success (id_Range_جابه_جایی))) أرجع;

    هذا->درازی = c->درازی () - جدول_initpos;
    إذا ((دراز دراز) هذا->درازی != (دراز دراز) c->درازی () - جدول_initpos)
    {
      c->err (HB_SERIALIZE_ERROR_INT_سرریز);
      أرجع;
    }

    هذا->seg_شمار_X2 = segcount * 2;
    هذا->مدخل_Selector = hb_max (1u, hb_بیت_storage (segcount)) - 1;
    هذا->جستجو_Range = 2 * (1u << هذا->مدخل_Selector);
    هذا->range_لغزش = segcount * 2 > هذا->جستجو_Range
		       ? 2 * segcount - هذا->جستجو_Range
		       : 0;
  }

  بیعلامت get_language () ثابت
  {
    أرجع language;
  }

  بنية accelerator_t
  {
    accelerator_t () {}
    accelerator_t (ثابت CmapSubtableFormat4 *subtable) = حذف;

    فراغ init (ثابت CmapSubtableFormat4 *subtable,
	       بیعلامت صحيح              subtable_داده_اندازه)
    {
      seg_شمار_2 = subtable->seg_شمار_X2 / 2;
      انتها_شمار = subtable->values.arrayZ;
      شروع_شمار = انتها_شمار + seg_شمار_2 + 1;
      idDelta = شروع_شمار + seg_شمار_2;
      id_Range_جابه_جایی = idDelta + seg_شمار_2;
      glyphIdArray = id_Range_جابه_جایی + seg_شمار_2;

      بیعلامت صحيح مقادیر_جابه_جایی = 16 + 8 * seg_شمار_2;
      glyph_Id_Array_درازی = subtable_داده_اندازه > مقادیر_جابه_جایی
			   ? (subtable_داده_اندازه - مقادیر_جابه_جایی) / 2
			   : 0;
    }

    منطقي get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) ثابت
    {
      بنية CustomRange
      {
	صحيح cmp (hb_codepoint_t k,
		 بیعلامت distance) ثابت
	{
	  إذا (k > آخرین) أرجع +1;
	  إذا (k < (&آخرین)[distance]/*اولی*/) أرجع -1;
	  أرجع 0;
	}
	HBUINT16 آخرین;
      };

      ثابت HBUINT16 *یافت = hb_bsearch (codepoint,
					  هذا->انتها_شمار,
					  هذا->seg_شمار_2,
					  حجم (CustomRange),
					  hb_cmp_روش<hb_codepoint_t, CustomRange, بیعلامت>,
					  هذا->seg_شمار_2 + 1);
      إذا (unlikely (!یافت))
	أرجع خطأ;
      بیعلامت صحيح i = یافت - انتها_شمار;

      hb_codepoint_t gid;
      بیعلامت صحيح range_جابه_جایی = هذا->id_Range_جابه_جایی[i];
      إذا (range_جابه_جایی == 0)
	gid = codepoint + هذا->idDelta[i];
      وإلا
      {
	/* Somebody has been smoking... */
	بیعلامت صحيح شاخص_2 = range_جابه_جایی / 2 + (codepoint - هذا->شروع_شمار[i]) + i - هذا->seg_شمار_2;
	إذا (unlikely (شاخص_2 >= هذا->glyph_Id_Array_درازی))
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
      أرجع درست;
    }

    HB_داخلی ساكن منطقي get_glyph_func (ثابت فراغ *obj, hb_codepoint_t codepoint, hb_codepoint_t *glyph)
    { أرجع ((ثابت accelerator_t *) obj)->get_glyph (codepoint, glyph); }

    فراغ collect_unicodes (hb_مجموعه_t *بیرون) ثابت
    {
      بیعلامت صحيح شمار = هذا->seg_شمار_2;
      إذا (شمار && هذا->شروع_شمار[شمار - 1] == 0xFFFFu)
	شمار--; /* Skip sentinel segment. */
      لكل (بیعلامت صحيح i = 0; i < شمار; i++)
      {
	hb_codepoint_t شروع = هذا->شروع_شمار[i];
	hb_codepoint_t انتها = هذا->انتها_شمار[i];
	بیعلامت صحيح range_جابه_جایی = هذا->id_Range_جابه_جایی[i];
        بیرون->جمع_کن_range(شروع, انتها);
	إذا (range_جابه_جایی == 0)
	{
	  لكل (hb_codepoint_t codepoint = شروع; codepoint <= انتها; codepoint++)
	  {
	    hb_codepoint_t gid = (codepoint + هذا->idDelta[i]) & 0xFFFFu;
	    إذا (unlikely (!gid))
              بیرون->del(codepoint);
	  }
	}
	وإلا
	{
	  لكل (hb_codepoint_t codepoint = شروع; codepoint <= انتها; codepoint++)
	  {
	    بیعلامت صحيح شاخص_2 = range_جابه_جایی / 2 + (codepoint - هذا->شروع_شمار[i]) + i - هذا->seg_شمار_2;
	    إذا (unlikely (شاخص_2 >= هذا->glyph_Id_Array_درازی))
            {
              بیرون->del_range (codepoint, انتها);
	      اكسر;
            }
	    hb_codepoint_t gid = هذا->glyphIdArray[شاخص_2];
	    إذا (unlikely (!gid))
              بیرون->del(codepoint);
	  }
	}
      }
    }

    فراغ collect_mapping (hb_مجموعه_t *unicodes, /*بیرون*/
			  hb_map_t *mapping /*بیرون*/) ثابت
    {
      // TODO(grieger): optimize similar to collect_unicodes
      // (ie. use add_range())
      بیعلامت شمار = هذا->seg_شمار_2;
      إذا (شمار && هذا->شروع_شمار[شمار - 1] == 0xFFFFu)
	شمار--; /* Skip sentinel segment. */
      لكل (بیعلامت i = 0; i < شمار; i++)
      {
	hb_codepoint_t شروع = هذا->شروع_شمار[i];
	hb_codepoint_t انتها = هذا->انتها_شمار[i];
	بیعلامت range_جابه_جایی = هذا->id_Range_جابه_جایی[i];
	إذا (range_جابه_جایی == 0)
	{
	  لكل (hb_codepoint_t codepoint = شروع; codepoint <= انتها; codepoint++)
	  {
	    hb_codepoint_t gid = (codepoint + هذا->idDelta[i]) & 0xFFFFu;
	    إذا (unlikely (!gid))
	      استمر;
	    unicodes->جمع_کن (codepoint);
	    mapping->مجموعه (codepoint, gid);
	  }
	}
	وإلا
	{
	  لكل (hb_codepoint_t codepoint = شروع; codepoint <= انتها; codepoint++)
	  {
	    بیعلامت شاخص_2 = range_جابه_جایی / 2 + (codepoint - هذا->شروع_شمار[i]) + i - هذا->seg_شمار_2;
	    إذا (unlikely (شاخص_2 >= هذا->glyph_Id_Array_درازی))
	      اكسر;
	    hb_codepoint_t gid = هذا->glyphIdArray[شاخص_2];
	    إذا (unlikely (!gid))
	      استمر;
            gid = (gid + هذا->idDelta[i]) & 0xFFFFu;
	    unicodes->جمع_کن (codepoint);
	    mapping->مجموعه (codepoint, gid);
	  }
	}
      }
    }

    ثابت HBUINT16 *انتها_شمار;
    ثابت HBUINT16 *شروع_شمار;
    ثابت HBUINT16 *idDelta;
    ثابت HBUINT16 *id_Range_جابه_جایی;
    ثابت HBUINT16 *glyphIdArray;
    بیعلامت صحيح seg_شمار_2;
    بیعلامت صحيح glyph_Id_Array_درازی;
  };

  منطقي get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph,
		  بیعلامت صحيح subtable_داده_اندازه) ثابت
  {
    accelerator_t accel;
    accel.init (هذا, subtable_داده_اندازه);
    أرجع accel.get_glyph_func (&accel, codepoint, glyph);
  }
  منطقي get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) ثابت
  { أرجع خطأ; }

  فراغ collect_unicodes (hb_مجموعه_t *بیرون, بیعلامت صحيح subtable_داده_اندازه) ثابت
  {
    accelerator_t accel;
    accel.init (هذا, subtable_داده_اندازه);
    accel.collect_unicodes (بیرون);
  }
  فراغ collect_unicodes (hb_مجموعه_t *بیرون) ثابت
  { collect_unicodes (بیرون, درازی); }

  فراغ collect_mapping (hb_مجموعه_t *unicodes, /*بیرون*/
			hb_map_t *mapping, /*بیرون*/
			بیعلامت صحيح subtable_داده_اندازه) ثابت
  {
    accelerator_t accel;
    accel.init (هذا, subtable_داده_اندازه);
    accel.collect_mapping (unicodes, mapping);
  }
  فراغ collect_mapping (hb_مجموعه_t *unicodes, /*بیرون*/
			hb_map_t *mapping /*بیرون*/) ثابت
  { collect_mapping (unicodes, mapping, درازی); }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!c->check_struct (هذا)))
      return_trace (خطأ);
    hb_barrier ();

    return_trace (c->check_range (values, 2 + 4 * seg_شمار_X2));
  }

  محمي:
  HBUINT16	format;		/* Format number is set to 4. */
  HBUINT16	درازی;		/* This is the length in bytes of the
				 * subtable. */
  HBUINT16	language;	/* Ignore. */
  HBUINT16	seg_شمار_X2;	/* 2 x segCount. */
  HBUINT16	جستجو_Range;	/* 2 * (2**floor(log2(segCount))) */
  HBUINT16	مدخل_Selector;	/* log2(searchRange/2) */
  HBUINT16	range_لغزش;	/* 2 x segCount - searchRange */

  Unsized_Array_از<HBUINT16>
		values;
#إذا 0
  HBUINT16	انتها_شمار[seg_شمار_2];	/* End characterCode for each segment,
					 * last=0xFFFFu. */
  HBUINT16	reservedPad;		/* Set to 0. */
  HBUINT16	شروع_شمار[seg_شمار_2];	/* Start character code for each segment. */
  HBINT16		idDelta[seg_شمار_2];	/* Delta for all character codes in segment. */
  HBUINT16	id_Range_جابه_جایی[seg_شمار_2];/* Offsets into glyphIdArray or 0 */
  Unsized_Array_از<HBUINT16>
		glyphIdArray;	/* Glyph index array (arbitrary length) */
#پایان_اگر

  عام:
  DEFINE_اندازه_ARRAY (14, values);
};

بنية Cmap_Subtable_Long_گروه
{
  دوست بنية CmapSubtableFormat12;
  دوست بنية CmapSubtableFormat13;
  قالب<نام_نوع U>
  دوست بنية CmapSubtableLongSegmented;
  دوست بنية cmap;

  صحيح cmp (hb_codepoint_t codepoint) ثابت
  {
    إذا (codepoint < شروع_Char_کد) أرجع -1;
    إذا (codepoint > انتها_Char_کد)   أرجع +1;
    أرجع 0;
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  خاص:
  HBUINT32		شروع_Char_کد;	/* First character code in this group. */
  HBUINT32		انتها_Char_کد;	/* Last character code in this group. */
  HBUINT32		glyphID;	/* Glyph index; interpretation depends on
					 * subtable format. */
  عام:
  DEFINE_اندازه_STATIC (12);
};
DECLARE_NULL_NAMESPACE_بایتها (OT, Cmap_Subtable_Long_گروه);

قالب <نام_نوع UINT>
بنية CmapSubtableTrimmed
{
  منطقي get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) ثابت
  {
    /* Rely on our implicit array bound-checking. */
    hb_codepoint_t gid = glyphIdArray[codepoint - شروع_Char_کد];
    إذا (unlikely (!gid))
      أرجع خطأ;
    *glyph = gid;
    أرجع درست;
  }

  بیعلامت get_language () ثابت
  {
    أرجع language;
  }

  فراغ collect_unicodes (hb_مجموعه_t *بیرون) ثابت
  {
    hb_codepoint_t شروع = شروع_Char_کد;
    بیعلامت صحيح شمار = glyphIdArray.len;
    لكل (بیعلامت صحيح i = 0; i < شمار; i++)
      إذا (glyphIdArray[i])
	بیرون->جمع_کن (شروع + i);
  }

  فراغ collect_mapping (hb_مجموعه_t *unicodes, /*بیرون*/
			hb_map_t *mapping /*بیرون*/) ثابت
  {
    hb_codepoint_t شروع_cp = شروع_Char_کد;
    بیعلامت شمار = glyphIdArray.len;
    لكل (بیعلامت i = 0; i < شمار; i++)
      إذا (glyphIdArray[i])
      {
	hb_codepoint_t unicode = شروع_cp + i;
	hb_codepoint_t glyphid = glyphIdArray[i];
	unicodes->جمع_کن (unicode);
	mapping->مجموعه (unicode, glyphid);
      }
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && glyphIdArray.sanitize (c));
  }

  محمي:
  UINT		formatReserved;	/* Subtable format and (maybe) padding. */
  UINT		درازی;		/* Byte length of this subtable. */
  UINT		language;	/* Ignore. */
  UINT		شروع_Char_کد;	/* First character code covered. */
  Array_از<HBGlyphID16, UINT>
		glyphIdArray;	/* Array of glyph index values for character
				 * codes in the range. */
  عام:
  DEFINE_اندازه_ARRAY (5 * حجم (UINT), glyphIdArray);
};

بنية CmapSubtableFormat6  : CmapSubtableTrimmed<HBUINT16> {};
بنية CmapSubtableFormat10 : CmapSubtableTrimmed<HBUINT32> {};

قالب <نام_نوع T>
بنية CmapSubtableLongSegmented
{
  دوست بنية cmap;

  منطقي get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) ثابت
  {
    hb_codepoint_t gid = T::گروه_get_glyph (groups.bsearch (codepoint), codepoint);
    إذا (unlikely (!gid))
      أرجع خطأ;
    *glyph = gid;
    أرجع درست;
  }

  بیعلامت get_language () ثابت
  {
    أرجع language;
  }

  فراغ collect_unicodes (hb_مجموعه_t *بیرون, بیعلامت صحيح رقم_glyphs) ثابت
  {
    لكل (بیعلامت صحيح i = 0; i < هذا->groups.len; i++)
    {
      hb_codepoint_t شروع = هذا->groups[i].شروع_Char_کد;
      hb_codepoint_t انتها = hb_min ((hb_codepoint_t) هذا->groups[i].انتها_Char_کد,
				   (hb_codepoint_t) HB_UNICODE_MAX);
      hb_codepoint_t gid = هذا->groups[i].glyphID;
      إذا (!gid)
      {
	/* Intention is: if (hb_is_same (T, CmapSubtableFormat13)) continue; */
	إذا (! T::گروه_get_glyph (هذا->groups[i], انتها)) استمر;
	شروع++;
	gid++;
      }
      إذا (unlikely ((بیعلامت صحيح) gid >= رقم_glyphs)) استمر;
      إذا (unlikely ((بیعلامت صحيح) (gid + انتها - شروع) >= رقم_glyphs))
	انتها = شروع + (hb_codepoint_t) رقم_glyphs - gid;

      بیرون->جمع_کن_range (شروع, hb_min (انتها, 0x10FFFFu));
    }
  }

  فراغ collect_mapping (hb_مجموعه_t *unicodes, /*بیرون*/
			hb_map_t *mapping, /*بیرون*/
			بیعلامت رقم_glyphs) ثابت
  {
    hb_codepoint_t آخرین_انتها = 0;
    بیعلامت شمار = هذا->groups.len;
    لكل (بیعلامت i = 0; i < شمار; i++)
    {
      hb_codepoint_t شروع = هذا->groups.arrayZ[i].شروع_Char_کد;
      hb_codepoint_t انتها = hb_min ((hb_codepoint_t) هذا->groups.arrayZ[i].انتها_Char_کد,
				   (hb_codepoint_t) HB_UNICODE_MAX);
      إذا (unlikely (شروع > انتها || شروع < آخرین_انتها)) {
        // Range is not in order and is invalid, skip it.
        استمر;
      }
      آخرین_انتها = انتها;


      hb_codepoint_t gid = هذا->groups.arrayZ[i].glyphID;
      إذا (!gid)
      {
        إذا (T::format_عدد == 13) استمر;
	شروع++;
	gid++;
      }
      إذا (unlikely ((بیعلامت صحيح) gid >= رقم_glyphs)) استمر;
      إذا (unlikely ((بیعلامت صحيح) (gid + انتها - شروع) >= رقم_glyphs))
	انتها = شروع + (hb_codepoint_t) رقم_glyphs - gid;

      mapping->alloc (mapping->get_population () + انتها - شروع + 1);

      unicodes->جمع_کن_range (شروع, انتها);
      لكل (بیعلامت cp = شروع; cp <= انتها; cp++)
      {
	mapping->مجموعه (cp, gid);
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
  HBUINT32	درازی;		/* Byte length of this subtable. */
  HBUINT32	language;	/* Ignore. */
  Sorted_Array32_از<Cmap_Subtable_Long_گروه>
		groups;		/* Groupings. */
  عام:
  DEFINE_اندازه_ARRAY (16, groups);
};

بنية CmapSubtableFormat12 : CmapSubtableLongSegmented<CmapSubtableFormat12>
{
  ساكن constexpr صحيح increment = 1;
  ساكن constexpr صحيح format_عدد = 12;

  ساكن hb_codepoint_t گروه_get_glyph (ثابت Cmap_Subtable_Long_گروه &گروه,
					 hb_codepoint_t u)
  { أرجع likely (گروه.شروع_Char_کد <= گروه.انتها_Char_کد) ?
	   گروه.glyphID + (u - گروه.شروع_Char_کد) : 0; }


  قالب<نام_نوع Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  فراغ serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    إذا (!it) أرجع;
    بیعلامت جدول_initpos = c->درازی ();
    إذا (unlikely (!c->extend_min (هذا))) أرجع;

    hb_codepoint_t شروع_Char_کد = (hb_codepoint_t) -1, انتها_Char_کد = (hb_codepoint_t) -1;
    hb_codepoint_t glyphID = 0;

    لكل (ثابت خودکار& _ : +it)
    {
      إذا (شروع_Char_کد == (hb_codepoint_t) -1)
      {
	شروع_Char_کد = _.اولی;
	انتها_Char_کد = _.اولی;
	glyphID = _.second;
      }
      وإلا إذا (!_is_gid_consecutive (انتها_Char_کد, شروع_Char_کد, glyphID, _.اولی, _.second))
      {
	Cmap_Subtable_Long_گروه  grouprecord;
	grouprecord.شروع_Char_کد = شروع_Char_کد;
	grouprecord.انتها_Char_کد = انتها_Char_کد;
	grouprecord.glyphID = glyphID;
	c->copy<Cmap_Subtable_Long_گروه> (grouprecord);

	شروع_Char_کد = _.اولی;
	انتها_Char_کد = _.اولی;
	glyphID = _.second;
      }
      وإلا
	انتها_Char_کد = _.اولی;
    }

    Cmap_Subtable_Long_گروه رکورد;
    رکورد.شروع_Char_کد = شروع_Char_کد;
    رکورد.انتها_Char_کد = انتها_Char_کد;
    رکورد.glyphID = glyphID;
    c->copy<Cmap_Subtable_Long_گروه> (رکورد);

    هذا->format = 12;
    هذا->reserved = 0;
    هذا->درازی = c->درازی () - جدول_initpos;
    هذا->groups.len = (هذا->درازی - min_اندازه) / Cmap_Subtable_Long_گروه::static_اندازه;
  }

  ساكن size_t get_sub_جدول_اندازه (ثابت hb_sorted_وکتور_t<Cmap_Subtable_Long_گروه> &groups_داده)
  { أرجع 16 + 12 * groups_داده.درازی; }

  خاص:
  ساكن منطقي _is_gid_consecutive (hb_codepoint_t انتها_Char_کد,
				   hb_codepoint_t شروع_Char_کد,
				   hb_codepoint_t glyphID,
				   hb_codepoint_t cp,
				   hb_codepoint_t new_gid)
  {
    أرجع (cp - 1 == انتها_Char_کد) &&
	new_gid == glyphID + (cp - شروع_Char_کد);
  }

};

بنية CmapSubtableFormat13 : CmapSubtableLongSegmented<CmapSubtableFormat13>
{
  ساكن constexpr صحيح increment = 0;
  ساكن constexpr صحيح format_عدد = 13;

  ساكن hb_codepoint_t گروه_get_glyph (ثابت Cmap_Subtable_Long_گروه &گروه,
					 hb_codepoint_t u HB_استفادهنشده)
  { أرجع گروه.glyphID; }
};

تعريفنوع تعداد
{
  GLYPH_VARIANT_NOT_یافت = 0,
  GLYPH_VARIANT_یافت = 1,
  GLYPH_VARIANT_USE_DEFAULT = 2
} glyph_variant_t;

بنية Unicode_مقدار_Range
{
  صحيح cmp (ثابت hb_codepoint_t &codepoint) ثابت
  {
    إذا (codepoint < شروع_Unicode_مقدار) أرجع -1;
    إذا (codepoint > شروع_Unicode_مقدار + additional_شمار) أرجع +1;
    أرجع 0;
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  HBUINT24	شروع_Unicode_مقدار;	/* First value in this range. */
  HBUINT8	additional_شمار;	/* Number of additional values in this
					 * range. */
  عام:
  DEFINE_اندازه_STATIC (4);
};

بنية DefaultUVS : Sorted_Array32_از<Unicode_مقدار_Range>
{
  فراغ collect_unicodes (hb_مجموعه_t *بیرون) ثابت
  {
    بیعلامت صحيح شمار = len;
    لكل (بیعلامت صحيح i = 0; i < شمار; i++)
    {
      hb_codepoint_t اولی = arrayZ[i].شروع_Unicode_مقدار;
      hb_codepoint_t آخرین = hb_min ((hb_codepoint_t) (اولی + arrayZ[i].additional_شمار),
				    (hb_codepoint_t) HB_UNICODE_MAX);
      بیرون->جمع_کن_range (اولی, آخرین);
    }
  }

  DefaultUVS* copy (hb_serialize_context_t *c,
		    ثابت hb_مجموعه_t *unicodes) ثابت
  {
    خودکار *بیرون = c->شروع_embed<DefaultUVS> ();
    خودکار snap = c->snapshot ();

    HBUINT32 len;
    len = 0;
    إذا (unlikely (!c->copy<HBUINT32> (len))) أرجع اشاره_تهی;
    بیعلامت init_len = c->درازی ();

    إذا (هذا->len > unicodes->get_population () * hb_بیت_storage ((بیعلامت) هذا->len))
    {
      hb_codepoint_t شروع = HB_مجموعه_مقدار_INVALID;
      hb_codepoint_t انتها = HB_مجموعه_مقدار_INVALID;

      لكل (خودکار u : *unicodes)
      {
        إذا (!as_array ().bsearch (u))
	  استمر;
	إذا (شروع == HB_مجموعه_مقدار_INVALID)
	{
	  شروع = u;
	  انتها = شروع - 1;
	}
	إذا (انتها + 1 != u || انتها - شروع == 255)
        {
	  Unicode_مقدار_Range rec;
	  rec.شروع_Unicode_مقدار = شروع;
	  rec.additional_شمار = انتها - شروع;
	  c->copy<Unicode_مقدار_Range> (rec);
	  شروع = u;
	}
	انتها = u;
      }
      إذا (شروع != HB_مجموعه_مقدار_INVALID)
      {
	Unicode_مقدار_Range rec;
	rec.شروع_Unicode_مقدار = شروع;
	rec.additional_شمار = انتها - شروع;
	c->copy<Unicode_مقدار_Range> (rec);
      }

    }
    وإلا
    {
      hb_codepoint_t آخرین_کد = HB_مجموعه_مقدار_INVALID;
      صحيح شمار = -1;

      لكل (ثابت Unicode_مقدار_Range& _ : *هذا)
      {
	hb_codepoint_t cur_مدخل = (hb_codepoint_t) (_.شروع_Unicode_مقدار - 1);
	hb_codepoint_t انتها = cur_مدخل + _.additional_شمار + 2;

	لكل (; unicodes->next (&cur_مدخل) && cur_مدخل < انتها;)
	{
	  شمار += 1;
	  إذا (آخرین_کد == HB_مجموعه_مقدار_INVALID)
	    آخرین_کد = cur_مدخل;
	  وإلا إذا (آخرین_کد + شمار != cur_مدخل || شمار > 255)
	  {
	    Unicode_مقدار_Range rec;
	    rec.شروع_Unicode_مقدار = آخرین_کد;
	    rec.additional_شمار = شمار - 1;
	    c->copy<Unicode_مقدار_Range> (rec);

	    آخرین_کد = cur_مدخل;
	    شمار = 0;
	  }
	}
      }

      إذا (آخرین_کد != HB_MAP_مقدار_INVALID)
      {
	Unicode_مقدار_Range rec;
	rec.شروع_Unicode_مقدار = آخرین_کد;
	rec.additional_شمار = شمار;
	c->copy<Unicode_مقدار_Range> (rec);
      }
    }

    إذا (c->درازی () - init_len == 0)
    {
      c->revert (snap);
      أرجع اشاره_تهی;
    }
    وإلا
    {
      إذا (unlikely (!c->check_تخصیص_بده (بیرون->len,
                                      (c->درازی () - init_len) / Unicode_مقدار_Range::static_اندازه,
                                      HB_SERIALIZE_ERROR_INT_سرریز))) أرجع اشاره_تهی;
      أرجع بیرون;
    }
  }

  عام:
  DEFINE_اندازه_ARRAY (4, *هذا);
};

قالب <نام_نوع Types>
بنية UVSMapping
{
  استفاده GlyphID = نام_نوع Types::HBGlyphID;

  صحيح cmp (ثابت hb_codepoint_t &codepoint) ثابت
  { أرجع unicode_مقدار.cmp (codepoint); }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  HBUINT24	unicode_مقدار;	/* Base Unicode value of the UVS */
  GlyphID
		glyphID;	/* Glyph ID of the UVS. */
  عام:
  DEFINE_اندازه_STATIC (3 + GlyphID::static_اندازه);
};

قالب <نام_نوع Types>
بنية NonDefaultUVS : Sorted_Array32_از<UVSMapping<Types>>
{
  استفاده Mapping = UVSMapping<Types>;

  فراغ collect_unicodes (hb_مجموعه_t *بیرون) ثابت
  {
    لكل (ثابت خودکار& a : هذا->as_array ())
      بیرون->جمع_کن (a.unicode_مقدار);
  }

  فراغ collect_mapping (hb_مجموعه_t *unicodes, /*بیرون*/
			hb_map_t *mapping /*بیرون*/) ثابت
  {
    لكل (ثابت خودکار& a : هذا->as_array ())
    {
      hb_codepoint_t unicode = a.unicode_مقدار;
      hb_codepoint_t glyphid = a.glyphID;
      unicodes->جمع_کن (unicode);
      mapping->مجموعه (unicode, glyphid);
    }
  }

  فراغ بستگی_glyphs (ثابت hb_مجموعه_t      *unicodes,
		       hb_مجموعه_t            *glyphset) ثابت
  {
    + هذا->as_array ()
    | hb_filter (unicodes, &Mapping::unicode_مقدار)
    | hb_map (&Mapping::glyphID)
    | hb_sink (glyphset)
    ;
  }

  NonDefaultUVS* copy (hb_serialize_context_t *c,
		       ثابت hb_مجموعه_t *unicodes,
		       ثابت hb_مجموعه_t *glyphs_requested,
		       ثابت hb_map_t *glyph_map) ثابت
  {
    خودکار *بیرون = c->شروع_embed<NonDefaultUVS> ();
    خودکار it =
    + هذا->as_array ()
    | hb_filter ([&] (ثابت Mapping& _)
		 {
		   أرجع unicodes->has (_.unicode_مقدار) || glyphs_requested->has (_.glyphID);
		 })
    ;

    إذا (!it) أرجع اشاره_تهی;

    HBUINT32 len;
    len = it.len ();
    إذا (unlikely (!c->copy<HBUINT32> (len))) أرجع اشاره_تهی;

    لكل (ثابت Mapping& _ : it)
    {
      Mapping mapping;
      mapping.unicode_مقدار = _.unicode_مقدار;
      mapping.glyphID = glyph_map->get (_.glyphID);
      c->copy<Mapping> (mapping);
    }

    أرجع بیرون;
  }

  عام:
  DEFINE_اندازه_ARRAY (4, *هذا);
};

قالب <نام_نوع Types>
بنية Variation_Selector_رکورد
{
  glyph_variant_t get_glyph (hb_codepoint_t codepoint,
			     hb_codepoint_t *glyph,
			     ثابت فراغ *پایه) ثابت
  {
    إذا ((پایه+defaultUVS).bfind (codepoint))
      أرجع GLYPH_VARIANT_USE_DEFAULT;
    ثابت UVSMapping<Types> &nonDefault = (پایه+nonDefaultUVS).bsearch (codepoint);
    إذا (nonDefault.glyphID)
    {
      *glyph = nonDefault.glyphID;
       أرجع GLYPH_VARIANT_یافت;
    }
    أرجع GLYPH_VARIANT_NOT_یافت;
  }

  Variation_Selector_رکورد(ثابت Variation_Selector_رکورد& other)
  {
    *هذا = other;
  }

  فراغ عملگر= (ثابت Variation_Selector_رکورد& other)
  {
    varSelector = other.varSelector;
    HBUINT32 جابه_جایی = other.defaultUVS;
    defaultUVS = جابه_جایی;
    جابه_جایی = other.nonDefaultUVS;
    nonDefaultUVS = جابه_جایی;
  }

  فراغ collect_unicodes (hb_مجموعه_t *بیرون, ثابت فراغ *پایه) ثابت
  {
    (پایه+defaultUVS).collect_unicodes (بیرون);
    (پایه+nonDefaultUVS).collect_unicodes (بیرون);
  }

  فراغ collect_mapping (ثابت فراغ *پایه,
			hb_مجموعه_t *unicodes, /*بیرون*/
			hb_map_t *mapping /*بیرون*/) ثابت
  {
    (پایه+defaultUVS).collect_unicodes (unicodes);
    (پایه+nonDefaultUVS).collect_mapping (unicodes, mapping);
  }

  صحيح cmp (ثابت hb_codepoint_t &variation_selector) ثابت
  { أرجع varSelector.cmp (variation_selector); }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *پایه) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  defaultUVS.sanitize (c, پایه) &&
		  nonDefaultUVS.sanitize (c, پایه));
  }

  hb_pair_t<بیعلامت, بیعلامت>
  copy (hb_serialize_context_t *c,
	ثابت hb_مجموعه_t *unicodes,
	ثابت hb_مجموعه_t *glyphs_requested,
	ثابت hb_map_t *glyph_map,
	ثابت فراغ *پایه) ثابت
  {
    خودکار snap = c->snapshot ();
    خودکار *بیرون = c->embed<Variation_Selector_رکورد> (*هذا);
    إذا (unlikely (!بیرون)) أرجع hb_pair (0, 0);

    بیرون->defaultUVS = 0;
    بیرون->nonDefaultUVS = 0;

    بیعلامت non_default_uvs_objidx = 0;
    إذا (nonDefaultUVS != 0)
    {
      c->push ();
      إذا (c->copy (پایه+nonDefaultUVS, unicodes, glyphs_requested, glyph_map))
	non_default_uvs_objidx = c->برداشت_pack ();
      وإلا c->برداشت_discard ();
    }

    بیعلامت default_uvs_objidx = 0;
    إذا (defaultUVS != 0)
    {
      c->push ();
      إذا (c->copy (پایه+defaultUVS, unicodes))
	default_uvs_objidx = c->برداشت_pack ();
      وإلا c->برداشت_discard ();
    }


    إذا (!default_uvs_objidx && !non_default_uvs_objidx)
      c->revert (snap);

    أرجع hb_pair (default_uvs_objidx, non_default_uvs_objidx);
  }

  HBUINT24	varSelector;	/* Variation selector. */
  Offset32_مقصد<DefaultUVS>
		defaultUVS;	/* Offset to Default UVS Table.  May be 0. */
  Offset32_مقصد<NonDefaultUVS<Types>>
		nonDefaultUVS;	/* Offset to Non-Default UVS Table.  May be 0. */
  عام:
  DEFINE_اندازه_STATIC (11);
};

قالب <نام_نوع Types>
بنية CmapSubtableFormat14_15
{
  ساكن constexpr بیعلامت format_مقدار = Types::اندازه == 2 ? 14 : 15;
  استفاده رکورد_2 = Variation_Selector_رکورد<Types>;
  استفاده NonDefault = NonDefaultUVS<Types>;

  glyph_variant_t get_glyph_variant (hb_codepoint_t codepoint,
				     hb_codepoint_t variation_selector,
				     hb_codepoint_t *glyph) ثابت
  { أرجع رکورد.bsearch (variation_selector).get_glyph (codepoint, glyph, هذا); }

  فراغ collect_variation_selectors (hb_مجموعه_t *بیرون) ثابت
  {
    لكل (ثابت خودکار& a : رکورد.as_array ())
      بیرون->جمع_کن (a.varSelector);
  }
  فراغ collect_variation_unicodes (hb_codepoint_t variation_selector,
				   hb_مجموعه_t *بیرون) ثابت
  { رکورد.bsearch (variation_selector).collect_unicodes (بیرون, هذا); }

  فراغ serialize (hb_serialize_context_t *c,
		  ثابت hb_مجموعه_t *unicodes,
		  ثابت hb_مجموعه_t *glyphs_requested,
		  ثابت hb_map_t *glyph_map,
		  ثابت فراغ *پایه)
  {
    خودکار snap = c->snapshot ();
    بیعلامت جدول_initpos = c->درازی ();
    ثابت حرف* init_دم = c->دم;

    إذا (unlikely (!c->extend_min (هذا))) أرجع;
    هذا->format = format_مقدار;

    خودکار src_tbl = reinterpret_cast<ثابت CmapSubtableFormat14_15*> (پایه);

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
    hb_وکتور_t<hb_pair_t<بیعلامت, بیعلامت>> obj_indices;
    لكل (صحيح i = src_tbl->رکورد.len - 1; i >= 0; i--)
    {
      إذا (!unicodes->has(src_tbl->رکورد[i].varSelector))
        استمر;

      hb_pair_t<بیعلامت, بیعلامت> نتیجه = src_tbl->رکورد[i].copy (c, unicodes, glyphs_requested, glyph_map, پایه);
      إذا (نتیجه.اولی || نتیجه.second)
	obj_indices.push (نتیجه);
    }

    إذا (c->درازی () - جدول_initpos == CmapSubtableFormat14_15::min_اندازه)
    {
      c->revert (snap);
      أرجع;
    }

    إذا (unlikely (!c->check_success (!obj_indices.درون_error ())))
      أرجع;

    صحيح دم_len = init_دم - c->دم;
    c->check_تخصیص_بده (هذا->درازی, c->درازی () - جدول_initpos + دم_len,
                     HB_SERIALIZE_ERROR_INT_سرریز);
    c->check_تخصیص_بده (هذا->رکورد.len,
		     (c->درازی () - جدول_initpos - CmapSubtableFormat14_15::min_اندازه) /
		     رکورد_2::static_اندازه,
                     HB_SERIALIZE_ERROR_INT_سرریز);

    /* Correct the incorrect write order by reversing the order of the variation
       records array. */
    معکوس_variation_رکوردها ();

    /* Now that records are in the right order, we can set up the offsets. */
    جمع_کن_links_مقصد_variation_رکوردها (c, obj_indices);
  }

  فراغ معکوس_variation_رکوردها ()
  {
    رکورد.as_array ().reverse ();
  }

  فراغ جمع_کن_links_مقصد_variation_رکوردها (hb_serialize_context_t *c,
					ثابت hb_وکتور_t<hb_pair_t<بیعلامت, بیعلامت>>& obj_indices)
  {
    لكل (بیعلامت i = 0; i < obj_indices.درازی; i++)
    {
      /*
       * Since the record array has been reversed (see comments in copy())
       * but obj_indices has not been, the indices at obj_indices[i]
       * are for the variation record at record[j].
       */
      صحيح j = obj_indices.درازی - 1 - i;
      c->جمع_کن_link (رکورد[j].defaultUVS, obj_indices[i].اولی);
      c->جمع_کن_link (رکورد[j].nonDefaultUVS, obj_indices[i].second);
    }
  }

  فراغ بستگی_glyphs (ثابت hb_مجموعه_t      *unicodes,
		       hb_مجموعه_t            *glyphset) ثابت
  {
    + hb_iter (رکورد)
    | hb_filter (hb_bool, &رکورد_2::nonDefaultUVS)
    | hb_filter (unicodes, &رکورد_2::varSelector)
    | hb_map (&رکورد_2::nonDefaultUVS)
    | hb_map (hb_جمع_کن (هذا))
    | hb_apply ([=] (ثابت NonDefault& _) { _.بستگی_glyphs (unicodes, glyphset); })
    ;
  }

  فراغ collect_unicodes (hb_مجموعه_t *بیرون) ثابت
  {
    لكل (ثابت رکورد_2& _ : رکورد)
      _.collect_unicodes (بیرون, هذا);
  }

  فراغ collect_mapping (hb_مجموعه_t *unicodes, /*بیرون*/
			hb_map_t *mapping /*بیرون*/) ثابت
  {
    لكل (ثابت رکورد_2& _ : رکورد)
      _.collect_mapping (هذا, unicodes, mapping);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  رکورد.sanitize (c, هذا));
  }

  محمي:
  HBUINT16	format;		/* Format number is set to 14 or 15. */
  HBUINT32	درازی;		/* Byte length of this subtable. */
  Sorted_Array32_از<رکورد_2>
		رکورد;		/* Variation selector records; sorted
				 * in increasing order of `varSelector'. */
  عام:
  DEFINE_اندازه_ARRAY (10, رکورد);
};

بنية CmapSubtable
{
  /* Note: We intentionally do NOT implement subtable formats 2 and 8. */

  منطقي is_variation_selector () ثابت
  {
    بدل (u.format.v) {
    حالة 14:
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    حالة 15:
#پایان_اگر
      أرجع درست;
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
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    حالة 15: hb_barrier (); أرجع u.format15.get_glyph_variant (codepoint, variation_selector, glyph);
#پایان_اگر
    افتراضي: أرجع GLYPH_VARIANT_NOT_یافت;
    }
  }

  فراغ collect_variation_selectors (hb_مجموعه_t *بیرون) ثابت
  {
    بدل (u.format.v) {
    حالة 14: hb_barrier (); u.format14.collect_variation_selectors (بیرون); أرجع;
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    حالة 15: hb_barrier (); u.format15.collect_variation_selectors (بیرون); أرجع;
#پایان_اگر
    افتراضي: أرجع;
    }
  }

  فراغ collect_variation_unicodes (hb_codepoint_t variation_selector,
				    hb_مجموعه_t *بیرون) ثابت
  {
    بدل (u.format.v) {
    حالة 14: hb_barrier (); u.format14.collect_variation_unicodes (variation_selector, بیرون); أرجع;
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    حالة 15: hb_barrier (); u.format15.collect_variation_unicodes (variation_selector, بیرون); أرجع;
#پایان_اگر
    افتراضي: أرجع;
    }
  }

  فراغ بستگی_glyphs (ثابت hb_مجموعه_t *unicodes, hb_مجموعه_t *glyphset) ثابت
  {
    بدل (u.format.v) {
    حالة 14: hb_barrier (); u.format14.بستگی_glyphs (unicodes, glyphset); أرجع;
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    حالة 15: hb_barrier (); u.format15.بستگی_glyphs (unicodes, glyphset); أرجع;
#پایان_اگر
    افتراضي: أرجع;
    }
  }

  منطقي get_glyph (hb_codepoint_t codepoint,
		  hb_codepoint_t *glyph,
		  بیعلامت صحيح subtable_داده_اندازه = 0) ثابت
  {
    بدل (u.format.v) {
    حالة  0: hb_barrier (); أرجع u.format0 .get_glyph (codepoint, glyph);
    حالة  4: hb_barrier (); أرجع subtable_داده_اندازه
				      ? u.format4.get_glyph (codepoint, glyph, subtable_داده_اندازه)
				      : خطأ;
    حالة  6: hb_barrier (); أرجع u.format6 .get_glyph (codepoint, glyph);
    حالة 10: hb_barrier (); أرجع u.format10.get_glyph (codepoint, glyph);
    حالة 12: hb_barrier (); أرجع u.format12.get_glyph (codepoint, glyph);
    حالة 13: hb_barrier (); أرجع u.format13.get_glyph (codepoint, glyph);
    حالة 14:
    افتراضي: أرجع خطأ;
    }
  }
  فراغ collect_unicodes (hb_مجموعه_t *بیرون,
			 بیعلامت صحيح رقم_glyphs = UINT_MAX,
			 بیعلامت صحيح subtable_داده_اندازه = 0) ثابت
  {
    بدل (u.format.v) {
    حالة  0: hb_barrier (); u.format0 .collect_unicodes (بیرون); أرجع;
    حالة  4: hb_barrier (); إذا (subtable_داده_اندازه)
			      u.format4.collect_unicodes (بیرون, subtable_داده_اندازه);
			    وإلا
			      u.format4.collect_unicodes (بیرون);
			    أرجع;
    حالة  6: hb_barrier (); u.format6 .collect_unicodes (بیرون); أرجع;
    حالة 10: hb_barrier (); u.format10.collect_unicodes (بیرون); أرجع;
    حالة 12: hb_barrier (); u.format12.collect_unicodes (بیرون, رقم_glyphs); أرجع;
    حالة 13: hb_barrier (); u.format13.collect_unicodes (بیرون, رقم_glyphs); أرجع;
    حالة 14:
    افتراضي: أرجع;
    }
  }

  فراغ collect_mapping (hb_مجموعه_t *unicodes, /*بیرون*/
			hb_map_t *mapping, /*بیرون*/
			بیعلامت رقم_glyphs = UINT_MAX,
			بیعلامت صحيح subtable_داده_اندازه = 0) ثابت
  {
    بدل (u.format.v) {
    حالة  0: hb_barrier (); u.format0 .collect_mapping (unicodes, mapping); أرجع;
    حالة  4: hb_barrier (); إذا (subtable_داده_اندازه)
			      u.format4.collect_mapping (unicodes, mapping, subtable_داده_اندازه);
			    وإلا
			      u.format4.collect_mapping (unicodes, mapping);
			    أرجع;
    حالة  6: hb_barrier (); u.format6 .collect_mapping (unicodes, mapping); أرجع;
    حالة 10: hb_barrier (); u.format10.collect_mapping (unicodes, mapping); أرجع;
    حالة 12: hb_barrier (); u.format12.collect_mapping (unicodes, mapping, رقم_glyphs); أرجع;
    حالة 13: hb_barrier (); u.format13.collect_mapping (unicodes, mapping, رقم_glyphs); أرجع;
    حالة 14:
    افتراضي: أرجع;
    }
  }

  بیعلامت get_language () ثابت
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

  قالب<نام_نوع Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  فراغ serialize (hb_serialize_context_t *c,
		  Iterator it,
		  بیعلامت format,
		  ثابت hb_subset_plan_t *plan,
		  ثابت فراغ *پایه)
  {
    بدل (format) {
    حالة  4: hb_barrier (); أرجع u.format4.serialize (c, it);
    حالة 12: hb_barrier (); أرجع u.format12.serialize (c, it);
    حالة 14: hb_barrier (); أرجع u.format14.serialize (c, &plan->unicodes, &plan->glyphs_requested, plan->glyph_map, پایه);
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    حالة 15: hb_barrier (); أرجع u.format15.serialize (c, &plan->unicodes, &plan->glyphs_requested, plan->glyph_map, پایه);
#پایان_اگر
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
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    حالة 15: hb_barrier (); return_trace (u.format15.sanitize (c));
#پایان_اگر
    افتراضي:return_trace (درست);
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
#اگر_تعریف_نشده HB_NO_BEYOND_64K
  CmapSubtableFormat14_15<Layout::MediumTypes>
			format15;
#پایان_اگر
  } u;
  عام:
  DEFINE_اندازه_UNION (2, format.v);
};


بنية Encoding_رکورد
{
  صحيح cmp (ثابت Encoding_رکورد &other) ثابت
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

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *پایه) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  subtable.sanitize (c, پایه));
  }

  قالب<نام_نوع Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  Encoding_رکورد* copy (hb_serialize_context_t *c,
			Iterator it,
			بیعلامت format,
			ثابت فراغ *پایه,
			ثابت hb_subset_plan_t *plan,
			/* INOUT */ بیعلامت *objidx) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خودکار snap = c->snapshot ();
    خودکار *بیرون = c->embed (هذا);
    إذا (unlikely (!بیرون)) return_trace (اشاره_تهی);
    بیرون->subtable = 0;

    إذا (*objidx == 0)
    {
      CmapSubtable *cmapsubtable = c->push<CmapSubtable> ();
      بیعلامت origin_درازی = c->درازی ();
      cmapsubtable->serialize (c, it, format, plan, &(پایه+subtable));
      إذا (c->درازی () > origin_درازی && !c->درون_error()) *objidx = c->برداشت_pack ();
      وإلا c->برداشت_discard ();
    }

    إذا (*objidx == 0)
    {
      c->revert (snap);
      return_trace (اشاره_تهی);
    }

    c->جمع_کن_link (بیرون->subtable, *objidx);
    return_trace (بیرون);
  }

  HBUINT16	platformID;	/* Platform ID. */
  HBUINT16	encodingID;	/* Platform-specific encoding ID. */
  Offset32_مقصد<CmapSubtable>
		subtable;	/* Byte offset from beginning of table to the subtable for this encoding. */
  عام:
  DEFINE_اندازه_STATIC (8);
};

بنية cmap;

بنية SubtableUnicodesCache {

 خاص:
  hb_blob_ptr_t<cmap> پایه_blob;
  ثابت حرف* پایه;
  بیعلامت صحيح جدول_درازی;
  hb_hashmap_t<بیعلامت, hb::unique_ptr<hb_مجموعه_t>> cached_unicodes;

 عام:

  ساكن SubtableUnicodesCache* create (hb_blob_ptr_t<cmap> منبع_جدول)
  {
    SubtableUnicodesCache* cache =
        (SubtableUnicodesCache*) hb_malloc (حجم(SubtableUnicodesCache));

    إذا (unlikely (!cache))
      أرجع اشاره_تهی;

    جديد (cache) SubtableUnicodesCache (منبع_جدول);
    أرجع cache;
  }

  ساكن فراغ destroy (فراغ* value) {
    إذا (!value) أرجع;

    SubtableUnicodesCache* cache = (SubtableUnicodesCache*) value;
    cache->~SubtableUnicodesCache ();
    hb_free (cache);
  }

  SubtableUnicodesCache(ثابت فراغ* cmap_پایه,
			بیعلامت صحيح جدول_درازی_2 = 0)
      : پایه_blob(),
        پایه ((ثابت حرف*) cmap_پایه),
        جدول_درازی (جدول_درازی_2),
        cached_unicodes ()
  {}

  SubtableUnicodesCache(hb_blob_ptr_t<cmap> پایه_blob_2)
      : پایه_blob(پایه_blob_2),
        پایه ((ثابت حرف *) پایه_blob.get()),
        جدول_درازی (پایه_blob.get_درازی ()),
        cached_unicodes ()
  {}

  ~SubtableUnicodesCache()
  {
    پایه_blob.destroy ();
  }

  منطقي same_پایه(ثابت فراغ* other) ثابت
  {
    أرجع other == (ثابت فراغ*) پایه;
  }

  ثابت hb_مجموعه_t* مجموعه_for (ثابت Encoding_رکورد* رکورد,
                           SubtableUnicodesCache& mutable_cache) ثابت
  {
    إذا (cached_unicodes.has ((بیعلامت) ((ثابت حرف *) رکورد - پایه)))
      أرجع cached_unicodes.get ((بیعلامت) ((ثابت حرف *) رکورد - پایه));

    أرجع mutable_cache.مجموعه_for (رکورد);
  }

  ثابت hb_مجموعه_t* مجموعه_for (ثابت Encoding_رکورد* رکورد)
  {
    إذا (!cached_unicodes.has ((بیعلامت) ((ثابت حرف *) رکورد - پایه)))
    {
      hb_مجموعه_t *s = hb_مجموعه_create ();
      إذا (unlikely (s->درون_error ()))
	أرجع hb_مجموعه_get_خالی ();

      بیعلامت صحيح subtable_داده_اندازه = رکورد->subtable < جدول_درازی
					? جدول_درازی - (بیعلامت صحيح) رکورد->subtable
					: 0;
      (پایه+رکورد->subtable).collect_unicodes (s, UINT_MAX, subtable_داده_اندازه);

      إذا (unlikely (!cached_unicodes.مجموعه ((بیعلامت) ((ثابت حرف *) رکورد - پایه), hb::unique_ptr<hb_مجموعه_t> {s})))
        أرجع hb_مجموعه_get_خالی ();

      أرجع s;
    }
    أرجع cached_unicodes.get ((بیعلامت) ((ثابت حرف *) رکورد - پایه));
  }

};

ساكن مضمن uint16_t
hb_نماد_pua_map (بیعلامت codepoint)
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
  ساكن constexpr hb_برچسب_t جدول_برچسب = HB_OT_برچسب_cmap;


  ساكن SubtableUnicodesCache* create_filled_cache(hb_blob_ptr_t<cmap> منبع_جدول) {
    ثابت cmap* cmap = منبع_جدول.get();
    خودکار it =
    + hb_iter (cmap->encoding_رکورد)
    | hb_filter ([&](ثابت Encoding_رکورد& _) {
      أرجع cmap::filter_encoding_رکوردها_for_subset (cmap, _);
    })
    ;

    SubtableUnicodesCache* cache = SubtableUnicodesCache::create(منبع_جدول);

    إذا (unlikely (!cache))
      أرجع اشاره_تهی;

    لكل (ثابت Encoding_رکورد& _ : it)
      cache->مجموعه_for(&_); // populate the cache for this encoding record.

    أرجع cache;
  }

  قالب<نام_نوع Iterator, نام_نوع EncodingRecIter,
	   hb_requires (hb_is_iterator (EncodingRecIter))>
  منطقي serialize (hb_serialize_context_t *c,
		  Iterator it,
		  EncodingRecIter encodingrec_iter,
		  ثابت فراغ *پایه,
		  hb_subset_plan_t *plan,
	                  منطقي drop_format_4 = خطأ,
			  بیعلامت صحيح منبع_جدول_درازی = 0)
  {
    إذا (unlikely (!c->extend_min ((*هذا))))  أرجع خطأ;
    هذا->نسخه = 0;

    بیعلامت format4objidx = 0, format12objidx = 0, format14objidx = 0;
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    بیعلامت format15objidx = 0;
#پایان_اگر
    خودکار snap = c->snapshot ();

    SubtableUnicodesCache local_unicodes_cache (پایه, منبع_جدول_درازی);
    ثابت SubtableUnicodesCache* unicodes_cache = &local_unicodes_cache;

    إذا (plan->accelerator &&
        plan->accelerator->cmap_cache &&
        plan->accelerator->cmap_cache->same_پایه (پایه))
      unicodes_cache = plan->accelerator->cmap_cache;

    لكل (ثابت Encoding_رکورد& _ : encodingrec_iter)
    {
      إذا (c->درون_error ())
        أرجع خطأ;

      بیعلامت format = (پایه+_.subtable).u.format.v;
      إذا (format != 4 && format != 12 && format != 14
#اگر_تعریف_نشده HB_NO_BEYOND_64K
	  && format != 15
#پایان_اگر
	 ) استمر;

      ثابت hb_مجموعه_t* unicodes_مجموعه = unicodes_cache->مجموعه_for (&_, local_unicodes_cache);

      إذا (!drop_format_4 && format == 4)
      {
        c->copy (_, + it | hb_filter (*unicodes_مجموعه, hb_اولی), 4u, پایه, plan, &format4objidx);
        إذا (c->درون_error () && c->only_سرریز ())
        {
          // cmap4 overflowed, reset and retry serialization without format 4 subtables.
          c->revert (snap);
          أرجع serialize (c, it,
                            encodingrec_iter,
                            پایه,
                            plan,
                            درست,
			    منبع_جدول_درازی);
        }
      }

      وإلا إذا (format == 12)
      {
        إذا (_can_drop (_,
                       *unicodes_مجموعه,
                       پایه,
                       *unicodes_cache,
                       local_unicodes_cache,
                       + it | hb_map (hb_اولی), encodingrec_iter))
          استمر;
        c->copy (_, + it | hb_filter (*unicodes_مجموعه, hb_اولی), 12u, پایه, plan, &format12objidx);
      }
      وإلا إذا (format == 14) c->copy (_, it, 14u, پایه, plan, &format14objidx);
#اگر_تعریف_نشده HB_NO_BEYOND_64K
      وإلا إذا (format == 15) c->copy (_, it, 15u, پایه, plan, &format15objidx);
#پایان_اگر
    }
        بیعلامت درازی = c->درازی ();
        بیعلامت موجود = درازی > cmap::min_اندازه ? درازی - cmap::min_اندازه : 0;
        c->check_تخصیص_بده(هذا->encoding_رکورد.len,
                        موجود / Encoding_رکورد::static_اندازه,
                        HB_SERIALIZE_ERROR_INT_سرریز);

    // Fail if format 4 was dropped and there is no cmap12.
    أرجع !drop_format_4 || format12objidx;
  }

  قالب<نام_نوع Iterator, نام_نوع Encoding_رکورد_Iterator,
      hb_requires (hb_is_iterator (Iterator)),
      hb_requires (hb_is_iterator (Encoding_رکورد_Iterator))>
  منطقي _can_drop (ثابت Encoding_رکورد& cmap12,
                  ثابت hb_مجموعه_t& cmap12_unicodes,
                  ثابت فراغ* پایه,
                  ثابت SubtableUnicodesCache& unicodes_cache,
                  SubtableUnicodesCache& local_unicodes_cache,
                  Iterator subset_unicodes,
                  Encoding_رکورد_Iterator encoding_رکوردها)
  {
    لكل (خودکار cp : + subset_unicodes | hb_filter (cmap12_unicodes))
    {
      إذا (cp >= 0x10000) أرجع خطأ;
    }

    بیعلامت هدف_platform;
    بیعلامت هدف_encoding;
    بیعلامت هدف_language = (پایه+cmap12.subtable).get_language ();

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

    لكل (ثابت خودکار& _ : encoding_رکوردها)
    {
      إذا (_.platformID != هدف_platform
          || _.encodingID != هدف_encoding
          || (پایه+_.subtable).get_language() != هدف_language)
        استمر;

      ثابت hb_مجموعه_t* sibling_unicodes = unicodes_cache.مجموعه_for (&_, local_unicodes_cache);

      خودکار cmap12 = + subset_unicodes | hb_filter (cmap12_unicodes);
      خودکار sibling = + subset_unicodes | hb_filter (*sibling_unicodes);
      لكل (; cmap12 && sibling; cmap12++, sibling++)
      {
        بیعلامت a = *cmap12;
        بیعلامت b = *sibling;
        إذا (a != b) أرجع خطأ;
      }

      أرجع !cmap12 && !sibling;
    }

    أرجع خطأ;
  }

  فراغ بستگی_glyphs (ثابت hb_مجموعه_t      *unicodes,
		       hb_مجموعه_t            *glyphset) ثابت
  {
    + hb_iter (encoding_رکورد)
    | hb_map (&Encoding_رکورد::subtable)
    | hb_map (hb_جمع_کن (هذا))
    | hb_filter (&CmapSubtable::is_variation_selector)
    | hb_apply ([=] (ثابت CmapSubtable& _) { _.بستگی_glyphs (unicodes, glyphset); })
    ;
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  { أرجع subset (c, خطأ); }

  منطقي subset (hb_subset_context_t *c, منطقي is_dmap) ثابت
  {
    TRACE_SUBSET (هذا);

    cmap *cmap_اول = c->serializer->شروع_embed<cmap> ();

    خودکار encodingrec_iter =
    + hb_iter (encoding_رکورد)
    | hb_filter ([&](ثابت Encoding_رکورد& _) {
      أرجع cmap::filter_encoding_رکوردها_for_subset (هذا, _);
    })
    ;

    إذا (unlikely (!encodingrec_iter.len ())) return_trace (خطأ);

    ثابت Encoding_رکورد *unicode_bmp= اشاره_تهی, *unicode_ucs4 = اشاره_تهی, *ms_bmp = اشاره_تهی, *ms_ucs4 = اشاره_تهی;
    منطقي has_format12 = خطأ;

    لكل (ثابت Encoding_رکورد& _ : encodingrec_iter)
    {
      بیعلامت format = (هذا + _.subtable).u.format.v;
      إذا (format == 12) has_format12 = درست;

      ثابت Encoding_رکورد *جدول = std::addressof (_);
      إذا      (_.platformID == 0 && _.encodingID ==  3) unicode_bmp = جدول;
      وإلا إذا (_.platformID == 0 && _.encodingID ==  4) unicode_ucs4 = جدول;
      وإلا إذا (_.platformID == 3 && _.encodingID ==  1) ms_bmp = جدول;
      وإلا إذا (_.platformID == 3 && _.encodingID == 10) ms_ucs4 = جدول;
    }

    إذا (unlikely (!is_dmap && !has_format12 && !unicode_bmp && !ms_bmp)) return_trace (خطأ);
    إذا (unlikely (!is_dmap && has_format12 && (!unicode_ucs4 && !ms_ucs4))) return_trace (خطأ);

    خودکار it =
    + c->plan->unicode_مقصد_new_gid_list.iter ()
    | hb_filter ([&] (ثابت hb_codepoint_pair_t _)
		 { أرجع (_.second != HB_MAP_مقدار_INVALID); })
    ;

    return_trace (cmap_اول->serialize (c->serializer,
                                         it,
                                         encodingrec_iter,
                                         هذا,
                                         c->plan,
					 خطأ,
					 c->منبع_blob->درازی));
  }

  ثابت CmapSubtable *پیدا_کن_بهترین_subtable (منطقي *نماد = اشاره_تهی,
					  منطقي *mac = اشاره_تهی,
					  منطقي *macroman = اشاره_تهی) ثابت
  {
    إذا (نماد) *نماد = خطأ;
    إذا (mac) *mac = خطأ;
    إذا (macroman) *macroman = خطأ;

    ثابت CmapSubtable *subtable;

    /* Symbol subtable.
     * Prefer symbol if available.
     * https://github.com/harfbuzz/harfbuzz/issues/1918 */
    إذا ((subtable = هذا->پیدا_کن_subtable (3, 0)))
    {
      إذا (نماد) *نماد = درست;
      أرجع subtable;
    }

    /* 32-bit subtables. */
    إذا ((subtable = هذا->پیدا_کن_subtable (3, 10))) أرجع subtable;
    إذا ((subtable = هذا->پیدا_کن_subtable (0, 6))) أرجع subtable;
    إذا ((subtable = هذا->پیدا_کن_subtable (0, 4))) أرجع subtable;

    /* 16-bit subtables. */
    إذا ((subtable = هذا->پیدا_کن_subtable (3, 1))) أرجع subtable;
    إذا ((subtable = هذا->پیدا_کن_subtable (0, 3))) أرجع subtable;
    إذا ((subtable = هذا->پیدا_کن_subtable (0, 2))) أرجع subtable;
    إذا ((subtable = هذا->پیدا_کن_subtable (0, 1))) أرجع subtable;
    إذا ((subtable = هذا->پیدا_کن_subtable (0, 0))) أرجع subtable;

    /* MacRoman subtable. */
    إذا ((subtable = هذا->پیدا_کن_subtable (1, 0)))
    {
      إذا (mac) *mac = درست;
      إذا (macroman) *macroman = درست;
      أرجع subtable;
    }
    /* Any other Mac subtable; we just map ASCII for these. */
    إذا ((subtable = هذا->پیدا_کن_subtable (1, 0xFFFF)))
    {
      إذا (mac) *mac = درست;
      أرجع subtable;
    }

    /* Meh. */
    أرجع &Null (CmapSubtable);
  }

  بنية accelerator_t
  {
    استفاده cache_t = hb_cache_t<21, 19>;
    static_assert (حجم (cache_t) == 1024, "");

    accelerator_t (hb_face_t *face, hb_برچسب_t جدول_برچسب_2 = cmap::جدول_برچسب)
    {
      هذا->جدول = hb_sanitize_context_t ().reference_جدول<cmap> (face, جدول_برچسب_2);
      إذا (!هذا->جدول.get_درازی ())
	أرجع;

      منطقي نماد, mac, macroman;
      هذا->subtable = جدول->پیدا_کن_بهترین_subtable (&نماد, &mac, &macroman);
      هذا->subtable_uvs = &Null (CmapSubtable);
      {
	ثابت CmapSubtable *st = جدول->پیدا_کن_variation_selector_subtable ();
	إذا (st)
	  subtable_uvs = st;
      }

#اگر_تعریف_نشده HB_NO_OT_FONT_CMAP_CACHE
      cache = (cache_t *) hb_malloc (حجم (cache_t));
      إذا (cache)
	جديد (cache) cache_t ();
      وإلا
        أرجع; // Such that get_glyph_funcZ remains null.
#پایان_اگر

      هذا->get_glyph_داده = subtable;
#اگر_تعریف_نشده HB_NO_CMAP_LEGACY_SUBTABLES
      منطقي is_format4 = subtable->u.format.v == 4;
      خودکار مجموعه_format4_getter = [هذا] (منطقي (*func) (ثابت فراغ *,
						      hb_codepoint_t,
						      hb_codepoint_t *))
      {
	هذا->format4_accel.init (&هذا->subtable->u.format4,
				  get_subtable_داده_اندازه (هذا->subtable));
	هذا->get_glyph_داده = &هذا->format4_accel;
	هذا->get_glyph_funcZ = func;
      };
      إذا (unlikely (نماد))
      {
	بدل ((بیعلامت) face->جدول.OS2->get_font_صفحه ()) {
	حالة OS2::font_صفحه_t::FONT_صفحه_NONE:
	  إذا (is_format4)
	    مجموعه_format4_getter (get_glyph_from_نماد<CmapSubtableFormat4::accelerator_t, hb_نماد_pua_map>);
	  وإلا
	    هذا->get_glyph_funcZ = get_glyph_from_نماد<CmapSubtable, hb_نماد_pua_map>;
	  اكسر;
#اگر_تعریف_نشده HB_NO_OT_SHAPER_ARABIC_FALLBACK
	حالة OS2::font_صفحه_t::FONT_صفحه_SIMP_ARABIC:
	  إذا (is_format4)
	    مجموعه_format4_getter (get_glyph_from_نماد<CmapSubtableFormat4::accelerator_t, _hb_arabic_pua_simp_map>);
	  وإلا
	    هذا->get_glyph_funcZ = get_glyph_from_نماد<CmapSubtable, _hb_arabic_pua_simp_map>;
	  اكسر;
	حالة OS2::font_صفحه_t::FONT_صفحه_TRAD_ARABIC:
	  إذا (is_format4)
	    مجموعه_format4_getter (get_glyph_from_نماد<CmapSubtableFormat4::accelerator_t, _hb_arabic_pua_trad_map>);
	  وإلا
	    هذا->get_glyph_funcZ = get_glyph_from_نماد<CmapSubtable, _hb_arabic_pua_trad_map>;
	  اكسر;
#پایان_اگر
	افتراضي:
	  إذا (is_format4)
	    مجموعه_format4_getter (get_glyph_from<CmapSubtableFormat4::accelerator_t>);
	  وإلا
	    هذا->get_glyph_funcZ = get_glyph_from<CmapSubtable>;
	  اكسر;
	}
      }
      وإلا إذا (unlikely (macroman))
      {
	إذا (is_format4)
	  مجموعه_format4_getter (get_glyph_from_macroman<CmapSubtableFormat4::accelerator_t>);
	وإلا
	  هذا->get_glyph_funcZ = get_glyph_from_macroman<CmapSubtable>;
      }
      وإلا إذا (unlikely (mac))
      {
	إذا (is_format4)
	  مجموعه_format4_getter (get_glyph_from_ascii<CmapSubtableFormat4::accelerator_t>);
	وإلا
	  هذا->get_glyph_funcZ = get_glyph_from_ascii<CmapSubtable>;
      }
      وإلا
#پایان_اگر
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
				      get_subtable_داده_اندازه (subtable));
	    هذا->get_glyph_داده = &هذا->format4_accel;
	    هذا->get_glyph_funcZ = هذا->format4_accel.get_glyph_func;
	    اكسر;
	  }
	}
      }
    }
    ~accelerator_t ()
    {
#اگر_تعریف_نشده HB_NO_OT_FONT_CMAP_CACHE
      hb_free (cache);
#پایان_اگر
      جدول.destroy ();
    }

    مضمن منطقي _cached_get (hb_codepoint_t unicode,
			     hb_codepoint_t *glyph) ثابت
    {
#اگر_تعریف_نشده HB_NO_OT_FONT_CMAP_CACHE
      // cache is always non-null if we have a get_glyph_funcZ
      بیعلامت v;
      إذا (cache->get (unicode, &v))
      {
        *glyph = v;
	أرجع درست;
      }
#پایان_اگر
      منطقي ret  = هذا->get_glyph_funcZ (هذا->get_glyph_داده, unicode, glyph);

#اگر_تعریف_نشده HB_NO_OT_FONT_CMAP_CACHE
      إذا (ret)
        cache->مجموعه (unicode, *glyph);
#پایان_اگر

      أرجع ret;
    }

    منطقي get_nominal_glyph (hb_codepoint_t  unicode,
			    hb_codepoint_t *glyph) ثابت
    {
      إذا (unlikely (!هذا->get_glyph_funcZ)) أرجع خطأ;
      أرجع _cached_get (unicode, glyph);
    }

    بیعلامت صحيح get_nominal_glyphs (بیعلامت صحيح شمار,
				     ثابت hb_codepoint_t *اولی_unicode,
				     بیعلامت صحيح unicode_stride,
				     hb_codepoint_t *اولی_glyph,
				     بیعلامت صحيح glyph_stride) ثابت
    {
      إذا (unlikely (!هذا->get_glyph_funcZ)) أرجع 0;

      بیعلامت صحيح انجامشده;
      لكل (انجامشده = 0;
	   انجامشده < شمار && _cached_get (*اولی_unicode, اولی_glyph);
	   انجامشده++)
      {
	اولی_unicode = &Struct_At_جابه_جایی_Unaligned<hb_codepoint_t> (اولی_unicode, unicode_stride);
	اولی_glyph = &Struct_At_جابه_جایی_Unaligned<hb_codepoint_t> (اولی_glyph, glyph_stride);
      }
      أرجع انجامشده;
    }

    منطقي get_variation_glyph (hb_codepoint_t  unicode,
			      hb_codepoint_t  variation_selector,
			      hb_codepoint_t *glyph) ثابت
    {
      بدل (get_glyph_variant (unicode, variation_selector, glyph))
      {
	حالة GLYPH_VARIANT_NOT_یافت:	أرجع خطأ;
	حالة GLYPH_VARIANT_یافت:	أرجع درست;
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

    منطقي has_داده () ثابت { أرجع جدول.get_درازی (); }

    فراغ collect_unicodes (hb_مجموعه_t *بیرون, بیعلامت صحيح رقم_glyphs) ثابت
    { subtable->collect_unicodes (بیرون, رقم_glyphs, get_subtable_داده_اندازه (subtable)); }
    فراغ collect_mapping (hb_مجموعه_t *unicodes, hb_map_t *mapping,
			  بیعلامت رقم_glyphs = UINT_MAX) ثابت
    { subtable->collect_mapping (unicodes, mapping, رقم_glyphs, get_subtable_داده_اندازه (subtable)); }
    فراغ collect_variation_selectors (hb_مجموعه_t *بیرون) ثابت
    { subtable_uvs->collect_variation_selectors (بیرون); }
    فراغ collect_variation_unicodes (hb_codepoint_t variation_selector,
				     hb_مجموعه_t *بیرون) ثابت
    { subtable_uvs->collect_variation_unicodes (variation_selector, بیرون); }

    محمي:
    تعريفنوع منطقي (*hb_cmap_get_glyph_func_t) (ثابت فراغ *obj,
					      hb_codepoint_t codepoint,
					      hb_codepoint_t *glyph);
    تعريفنوع uint16_t (*hb_pua_remap_func_t) (بیعلامت);

    قالب <نام_نوع Type>
    HB_داخلی ساكن منطقي get_glyph_from (ثابت فراغ *obj,
					    hb_codepoint_t codepoint,
					    hb_codepoint_t *glyph)
    {
      ثابت Type *typed_obj = (ثابت Type *) obj;
      أرجع typed_obj->get_glyph (codepoint, glyph);
    }

    قالب <نام_نوع Type, hb_pua_remap_func_t remap>
    HB_داخلی ساكن منطقي get_glyph_from_نماد (ثابت فراغ *obj,
						   hb_codepoint_t codepoint,
						   hb_codepoint_t *glyph)
    {
      ثابت Type *typed_obj = (ثابت Type *) obj;
      إذا (likely (typed_obj->get_glyph (codepoint, glyph)))
	أرجع درست;

      إذا (hb_codepoint_t c = remap (codepoint))
	أرجع typed_obj->get_glyph (c, glyph);

      أرجع خطأ;
    }

    قالب <نام_نوع Type>
    HB_داخلی ساكن منطقي get_glyph_from_ascii (ثابت فراغ *obj,
						  hb_codepoint_t codepoint,
						  hb_codepoint_t *glyph)
    {
      ثابت Type *typed_obj = (ثابت Type *) obj;
      أرجع codepoint < 0x80 && typed_obj->get_glyph (codepoint, glyph);
    }

    قالب <نام_نوع Type>
    HB_داخلی ساكن منطقي get_glyph_from_macroman (ثابت فراغ *obj,
						     hb_codepoint_t codepoint,
						     hb_codepoint_t *glyph)
    {
      إذا (get_glyph_from_ascii<Type> (obj, codepoint, glyph))
	أرجع درست;

      ثابت Type *typed_obj = (ثابت Type *) obj;
      بیعلامت c = unicode_مقصد_macroman (codepoint);
      أرجع c && typed_obj->get_glyph (c, glyph);
    }

    بیعلامت صحيح get_subtable_داده_اندازه (ثابت CmapSubtable *subtable) ثابت
    {
      بیعلامت صحيح جدول_درازی = هذا->جدول.get_درازی ();
      uintptr_t جدول_شروع = (uintptr_t) (ثابت فراغ *) هذا->جدول.get ();
      uintptr_t subtable_addr = (uintptr_t) (ثابت فراغ *) subtable;
      إذا (unlikely (subtable_addr < جدول_شروع))
	أرجع 0;

      uintptr_t subtable_جابه_جایی = subtable_addr - جدول_شروع;
      إذا (unlikely (subtable_جابه_جایی >= جدول_درازی))
	أرجع 0;

      أرجع جدول_درازی - (بیعلامت صحيح) subtable_جابه_جایی;
    }

    خاص:
    hb_nonnull_ptr_t<ثابت CmapSubtable> subtable;
    hb_nonnull_ptr_t<ثابت CmapSubtable> subtable_uvs;

    hb_cmap_get_glyph_func_t get_glyph_funcZ = اشاره_تهی;
    ثابت فراغ *get_glyph_داده = اشاره_تهی;

    CmapSubtableFormat4::accelerator_t format4_accel;

#اگر_تعریف_نشده HB_NO_OT_FONT_CMAP_CACHE
    cache_t *cache = اشاره_تهی;
#پایان_اگر

    عام:
    hb_blob_ptr_t<cmap> جدول;
  };

  محمي:

  ثابت CmapSubtable *پیدا_کن_subtable (بیعلامت صحيح platform_id,
				     بیعلامت صحيح encoding_id) ثابت
  {
    Encoding_رکورد key;
    key.platformID = platform_id;
    key.encodingID = encoding_id;

    ثابت Encoding_رکورد &نتیجه = encoding_رکورد.bsearch (key);
    إذا (!نتیجه.subtable)
      أرجع اشاره_تهی;

    أرجع &(هذا+نتیجه.subtable);
  }

  ثابت CmapSubtable *پیدا_کن_variation_selector_subtable () ثابت
  {
    ثابت CmapSubtable *format14 = اشاره_تهی;
    لكل (ثابت Encoding_رکورد& رکورد : encoding_رکورد)
    {
      إذا (رکورد.platformID != 0 || رکورد.encodingID != 5)
	استمر;

      ثابت CmapSubtable *subtable = &(هذا+رکورد.subtable);
#اگر_تعریف_نشده HB_NO_BEYOND_64K
      إذا (subtable->u.format.v == 15)
	أرجع subtable;
#پایان_اگر
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
		  encoding_رکورد.sanitize (c, هذا));
  }

 خاص:

  ساكن منطقي filter_encoding_رکوردها_for_subset(ثابت cmap* cmap,
                                                 ثابت Encoding_رکورد& _)
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
  Sorted_Array16_از<Encoding_رکورد>
		encoding_رکورد;	/* Encoding tables. */
  عام:
  DEFINE_اندازه_ARRAY (4, encoding_رکورد);
};

بنية DMAP : cmap
{
  ساكن constexpr hb_برچسب_t جدول_برچسب = HB_OT_برچسب_DMAP;

  منطقي subset (hb_subset_context_t *c) ثابت
  { أرجع cmap::subset (c, درست); }
};

بنية cmap_accelerator_t
{
  cmap_accelerator_t (hb_face_t *face) :
    dmap (face, HB_OT_برچسب_DMAP),
    پایه (face)
  {}

  منطقي get_nominal_glyph (hb_codepoint_t  unicode,
			  hb_codepoint_t *glyph) ثابت
  {
    أرجع dmap.get_nominal_glyph (unicode, glyph) ||
	   پایه.get_nominal_glyph (unicode, glyph);
  }

  بیعلامت صحيح get_nominal_glyphs (بیعلامت صحيح شمار,
				   ثابت hb_codepoint_t *اولی_unicode,
				   بیعلامت صحيح unicode_stride,
				   hb_codepoint_t *اولی_glyph,
				   بیعلامت صحيح glyph_stride) ثابت
  {
    إذا (likely (!dmap.has_داده ()))
      أرجع پایه.get_nominal_glyphs (شمار,
				      اولی_unicode, unicode_stride,
				      اولی_glyph, glyph_stride);

    بیعلامت صحيح انجامشده;
    لكل (انجامشده = 0;
	 انجامشده < شمار && get_nominal_glyph (*اولی_unicode, اولی_glyph);
	 انجامشده++)
    {
      اولی_unicode = &Struct_At_جابه_جایی_Unaligned<hb_codepoint_t> (اولی_unicode, unicode_stride);
      اولی_glyph = &Struct_At_جابه_جایی_Unaligned<hb_codepoint_t> (اولی_glyph, glyph_stride);
    }
    أرجع انجامشده;
  }

  منطقي get_variation_glyph (hb_codepoint_t  unicode,
			    hb_codepoint_t  variation_selector,
			    hb_codepoint_t *glyph) ثابت
  {
    glyph_variant_t نتیجه = dmap.get_glyph_variant (unicode,
						     variation_selector,
						     glyph);
    إذا (نتیجه == GLYPH_VARIANT_NOT_یافت)
      نتیجه = پایه.get_glyph_variant (unicode, variation_selector, glyph);

    بدل (نتیجه)
    {
      حالة GLYPH_VARIANT_NOT_یافت: أرجع خطأ;
      حالة GLYPH_VARIANT_یافت: أرجع درست;
      حالة GLYPH_VARIANT_USE_DEFAULT: اكسر;
    }

    أرجع get_nominal_glyph (unicode, glyph);
  }

  فراغ collect_unicodes (hb_مجموعه_t *بیرون, بیعلامت صحيح رقم_glyphs) ثابت
  {
    پایه.collect_unicodes (بیرون, رقم_glyphs);
    dmap.collect_unicodes (بیرون, رقم_glyphs);
  }

  فراغ collect_mapping (hb_مجموعه_t *unicodes, hb_map_t *mapping,
			بیعلامت رقم_glyphs = UINT_MAX) ثابت
  {
    پایه.collect_mapping (unicodes, mapping, رقم_glyphs);
    dmap.collect_mapping (unicodes, mapping, رقم_glyphs);
  }

  فراغ collect_variation_selectors (hb_مجموعه_t *بیرون) ثابت
  {
    پایه.collect_variation_selectors (بیرون);
    dmap.collect_variation_selectors (بیرون);
  }

  فراغ collect_variation_unicodes (hb_codepoint_t variation_selector,
				   hb_مجموعه_t *بیرون) ثابت
  {
    پایه.collect_variation_unicodes (variation_selector, بیرون);
    dmap.collect_variation_unicodes (variation_selector, بیرون);
  }

  فراغ بستگی_glyphs (ثابت hb_مجموعه_t *unicodes,
		       hb_مجموعه_t       *glyphset) ثابت
  {
    إذا (likely (!dmap.has_داده ()))
    {
      پایه.جدول->بستگی_glyphs (unicodes, glyphset);
      أرجع;
    }

    hb_مجموعه_t variation_selectors;
    collect_variation_selectors (&variation_selectors);
    لكل (hb_codepoint_t variation_selector : variation_selectors)
    {
      إذا (!unicodes->has (variation_selector))
	استمر;

      hb_مجموعه_t variation_unicodes;
      collect_variation_unicodes (variation_selector, &variation_unicodes);
      لكل (hb_codepoint_t unicode : variation_unicodes)
      {
	إذا (!unicodes->has (unicode))
	  استمر;

	hb_codepoint_t glyph;
	إذا (get_variation_glyph (unicode, variation_selector, &glyph))
	  glyphset->جمع_کن (glyph);
      }
    }
  }

  cmap::accelerator_t dmap;
  cmap::accelerator_t پایه;
};

} /* namespace OT */


#پایان_اگر /* HB_OT_CMAP_TABLE_HH */
