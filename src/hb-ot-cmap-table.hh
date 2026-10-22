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

#اگر_غیر_تعریف_شدہ HB_OT_CMAP_جدول_HH
#تعریف_کرو HB_OT_CMAP_جدول_HH

#شامل "hb-ot-os2-table.hh"
#شامل "hb-ot-shaper-arabic-pua.hh"
#شامل "hb-open-type.hh"
#شامل "hb-ot-dual.hh"
#شامل "hb-set.hh"
#شامل "hb-cache.hh"

/*
 * cmap -- Character to Glyph Index Mapping
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cmap
 */
#تعریف_کرو HB_OT_لیبل_cmap HB_لیبل('c','m','a','p')
#تعریف_کرو HB_OT_لیبل_DMAP HB_لیبل('D','M','A','P')

نامخلا OT {

ساکن اندرونی uint8_t unicode_مقصد_macroman (hb_codepoint_t u)
{
  ساکن ثابت ساخت unicode_مقصد_macroman_t
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
  خودکار *c = hb_bsearch (u, mapping, ARRAY_لمبائی (mapping), حجم (mapping[0]),
			_hb_cmp_operator<uint16_t, uint16_t>);
  واپس c ? c->macroman : 0;
}

ساخت CmapSubtableFormat0
{
  منطقی get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) ثابت
  {
    hb_codepoint_t gid = codepoint < 256 ? glyphIdArray[codepoint] : 0;
    اگر (unlikely (!gid))
      واپس جھوٹ;
    *glyph = gid;
    واپس سچ;
  }

  غیرعلامتی get_language () ثابت
  {
    واپس language;
  }

  خالی collect_unicodes (hb_سیٹ_t *برآمد) ثابت
  {
    برائے (غیرعلامتی صحیح i = 0; i < 256; i++)
      اگر (glyphIdArray[i])
	برآمد->جوڑو (i);
  }

  خالی collect_mapping (hb_سیٹ_t *unicodes, /*برآمد*/
			hb_map_t *mapping /*برآمد*/) ثابت
  {
    برائے (غیرعلامتی i = 0; i < 256; i++)
      اگر (glyphIdArray[i])
      {
	hb_codepoint_t glyph = glyphIdArray[i];
	unicodes->جوڑو (i);
	mapping->سیٹ (i, glyph);
      }
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ));
  }

  محفوظ:
  HBUINT16	format;		/* Format number is set to 0. */
  HBUINT16	لمبائی;		/* Byte length of this subtable. */
  HBUINT16	language;	/* Ignore. */
  HBUINT8	glyphIdArray[256];/* An array that maps character
				 * code to glyph index values. */
  عوامی:
  DEFINE_سائز_STATIC (6 + 256);
};

ساخت CmapSubtableFormat4
{
  سانچہ<قسم_نام Iterator,
      قسم_نام Writer,
	   hb_requires (hb_is_iterator (Iterator))>
  خالی مقصد_ranges (Iterator it, Writer& range_writer)
  {
    hb_codepoint_t آغاز_cp = 0, پچھلا_تسلسل_آغاز_cp = 0, تسلسل_آغاز_cp = 0, انتہا_cp = 0, آخری_gid = 0;
    صحیح تسلسل_لمبائی = 0 , delta = 0, پچھلا_delta = 0;

    شمار {
      پہلا_SUB_RANGE,
      FOLLOWING_SUB_RANGE,
    } منوال;

    جب (it) {
      // Start a new range
      {
        ثابت خودکار& pair = *it;
        آغاز_cp = pair.پہلا;
        پچھلا_تسلسل_آغاز_cp = آغاز_cp;
        تسلسل_آغاز_cp = آغاز_cp;
        انتہا_cp = آغاز_cp;
        آخری_gid = pair.second;
        تسلسل_لمبائی = 1;
        پچھلا_delta = 0;
      }

      delta = آخری_gid - آغاز_cp;
      منوال = پہلا_SUB_RANGE;
      it++;

      جب (it) {
        // Process range
        ثابت خودکار& pair = *it;
        hb_codepoint_t اگلا_cp = pair.پہلا;
        hb_codepoint_t اگلا_gid = pair.second;
        اگر (اگلا_cp != انتہا_cp + 1) {
          // Current range is over, stop processing.
          توڑو;
        }

        اگر (اگلا_gid == آخری_gid + 1) {
          // The current run continues.
          انتہا_cp = اگلا_cp;
          تسلسل_لمبائی++;
          آخری_gid = اگلا_gid;
          it++;
          جاری;
        }

        // A new run is starting, decide if we want to commit the current run.
        صحیح بانٹو_لاگت = (منوال == پہلا_SUB_RANGE) ? 8 : 16;
        صحیح تسلسل_لاگت = تسلسل_لمبائی * 2;
        اگر (تسلسل_لاگت >= بانٹو_لاگت) {
          commit_current_range(آغاز_cp,
                               پچھلا_تسلسل_آغاز_cp,
                               تسلسل_آغاز_cp,
                               انتہا_cp,
                               delta,
                               پچھلا_delta,
                               بانٹو_لاگت,
                               range_writer);
          آغاز_cp = اگلا_cp;
        }

        // Start the new run
        منوال = FOLLOWING_SUB_RANGE;
        پچھلا_تسلسل_آغاز_cp = تسلسل_آغاز_cp;
        تسلسل_آغاز_cp = اگلا_cp;
        انتہا_cp = اگلا_cp;
        پچھلا_delta = delta;
        delta = اگلا_gid - تسلسل_آغاز_cp;
        تسلسل_لمبائی = 1;
        آخری_gid = اگلا_gid;
        it++;
      }

      // Finalize range
      commit_current_range (آغاز_cp,
                            پچھلا_تسلسل_آغاز_cp,
                            تسلسل_آغاز_cp,
                            انتہا_cp,
                            delta,
                            پچھلا_delta,
                            8,
                            range_writer);
    }

    اگر (likely (انتہا_cp != 0xFFFF)) {
      range_writer (0xFFFF, 0xFFFF, 1);
    }
  }

  /*
   * Writes the current range as either one or two ranges depending on what is most efficient.
   */
  سانچہ<قسم_نام Writer>
  خالی commit_current_range (hb_codepoint_t آغاز,
                             hb_codepoint_t پچھلا_تسلسل_آغاز,
                             hb_codepoint_t تسلسل_آغاز,
                             hb_codepoint_t انتہا,
                             صحیح تسلسل_delta,
                             صحیح پچھلا_تسلسل_delta,
                             صحیح بانٹو_لاگت,
                             Writer& range_writer) {
    منطقی should_بانٹو = جھوٹ;
    اگر (آغاز < تسلسل_آغاز && تسلسل_آغاز < انتہا) {
      صحیح تسلسل_لاگت = (انتہا - تسلسل_آغاز + 1) * 2;
      اگر (تسلسل_لاگت >= بانٹو_لاگت) {
        should_بانٹو = سچ;
      }
    }

    // A zero delta is also the marker for ranges that need a
    // glyphIdArray; serialize_rangeoffset_glyid() tells the two cases
    // apart by re-checking the mapping.
    اگر (should_بانٹو) {
      اگر (آغاز == پچھلا_تسلسل_آغاز)
        range_writer (آغاز, تسلسل_آغاز - 1, پچھلا_تسلسل_delta);
      ورنہ
        range_writer (آغاز, تسلسل_آغاز - 1, 0);
      range_writer (تسلسل_آغاز, انتہا, تسلسل_delta);
      واپس;
    }


    اگر (آغاز == تسلسل_آغاز) {
      // Range is only a run
      range_writer (آغاز, انتہا, تسلسل_delta);
      واپس;
    }

    // Write only a single non-run range.
    range_writer (آغاز, انتہا, 0);
  }

  سانچہ<قسم_نام Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  غیرعلامتی serialize_ڈھونڈو_segcount (Iterator it) {
    ساخت شمار_2 {
      غیرعلامتی segcount = 0;

      خالی عامل() (hb_codepoint_t آغاز,
                       hb_codepoint_t انتہا,
                       صحیح delta) {
        segcount++;
      }
    } شمار_3;

    مقصد_ranges (+it, شمار_3);
    واپس شمار_3.segcount;
  }


  سانچہ<قسم_نام Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  منطقی serialize_آغاز_انتہا_delta_arrays (hb_serialize_context_t *c,
                                         Iterator it,
                                         صحیح segcount)
  {
    ساخت Writer {
      hb_serialize_context_t *serializer_;
      HBUINT16* انتہا_رمز;
      HBUINT16* آغاز_رمز;
      HBINT16* id_delta_;
      صحیح اشاریہ_3;

      Writer(hb_serialize_context_t *serializer)
          : serializer_(serializer),
            انتہا_رمز(خالی_اشاریہ),
            آغاز_رمز(خالی_اشاریہ),
            id_delta_(خالی_اشاریہ),
            اشاریہ_3 (0) {}
      خالی عامل() (hb_codepoint_t آغاز,
                       hb_codepoint_t انتہا,
                       صحیح delta) {
        آغاز_رمز[اشاریہ_3] = آغاز;
        انتہا_رمز[اشاریہ_3] = انتہا;
        id_delta_[اشاریہ_3] = delta;
        اشاریہ_3++;
      }
    } writer(c);

    writer.انتہا_رمز = c->allocate_سائز<HBUINT16> (HBUINT16::static_سائز * segcount, جھوٹ);
    (خالی) c->allocate_سائز<HBUINT16> (2); // padding
    writer.آغاز_رمز = c->allocate_سائز<HBUINT16> (HBUINT16::static_سائز * segcount, جھوٹ);
    writer.id_delta_ = c->allocate_سائز<HBINT16> (HBINT16::static_سائز * segcount, جھوٹ);

    اگر (unlikely (!writer.انتہا_رمز || !writer.آغاز_رمز || !writer.id_delta_)) واپس جھوٹ;

    مقصد_ranges (+it, writer);
    واپس سچ;
  }

  سانچہ<قسم_نام Iterator,
          hb_requires (hb_is_iterator (Iterator))>
  HBUINT16* serialize_rangeoffset_glyid (hb_serialize_context_t *c,
                                         Iterator it,
					 HBUINT16 *انتہا_رمز_2,
					 HBUINT16 *آغاز_رمز_2,
					 HBINT16 *idDelta,
					 غیرعلامتی segcount)
  {
    hb_map_t cp_مقصد_gid { it };

    HBUINT16 *id_Range_انحراف = c->allocate_سائز<HBUINT16> (HBUINT16::static_سائز * segcount);
    اگر (unlikely (!c->check_success (id_Range_انحراف))) واپس خالی_اشاریہ;
    اگر (unlikely ((حرف *)id_Range_انحراف - (حرف *)idDelta != (صحیح) segcount * (صحیح) HBINT16::static_سائز)) واپس خالی_اشاریہ;

    برائے (غیرعلامتی i : hb_range (segcount))
    {
      /* A non-zero delta is always a real run. A zero delta is
       * ambiguous: identity mapping, or the marker for a range that
       * needs a glyphIdArray. Only zero deltas need a check. */
      صحیح delta = idDelta[i];
      اگر (delta)
	جاری;
      منطقی use_delta = سچ;
      برائے (hb_codepoint_t cp = آغاز_رمز_2[i]; cp <= انتہا_رمز_2[i]; cp++)
      {
	hb_codepoint_t gid = cp_مقصد_gid.get (cp);
	اگر (gid == HB_MAP_قدر_INVALID)
	  gid = 0;
	اگر (gid != cp)
	{
	  use_delta = جھوٹ;
	  توڑو;
	}
      }
      اگر (use_delta)
	جاری;

      id_Range_انحراف[i] = 2 * (c->آغاز_embed<HBUINT16> () - id_Range_انحراف - i);
      برائے (hb_codepoint_t cp = آغاز_رمز_2[i]; cp <= انتہا_رمز_2[i]; cp++)
      {
        HBUINT16 gid;
        gid = cp_مقصد_gid[cp];
        c->copy<HBUINT16> (gid);
      }
    }

    واپس id_Range_انحراف;
  }

  سانچہ<قسم_نام Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  خالی serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    خودکار format4_iter =
    + it
    | hb_filter ([&] (ثابت hb_codepoint_pair_t _)
		 { واپس _.پہلا <= 0xFFFF; })
    ;

    اگر (!format4_iter) واپس;

    غیرعلامتی جدول_initpos = c->لمبائی ();
    اگر (unlikely (!c->extend_min (یہ))) واپس;
    یہ->format = 4;

    hb_سمتیہ_t<hb_codepoint_pair_t> cp_مقصد_gid {
      format4_iter
    };

    //serialize endCode[], startCode[], idDelta[]
    HBUINT16* انتہا_رمز_2 = c->آغاز_embed<HBUINT16> ();
    غیرعلامتی segcount = serialize_ڈھونڈو_segcount (cp_مقصد_gid.iter());
    اگر (unlikely (!serialize_آغاز_انتہا_delta_arrays (c, cp_مقصد_gid.iter(), segcount)))
      واپس;

    HBUINT16 *آغاز_رمز_2 = انتہا_رمز_2 + segcount + 1;
    HBINT16 *idDelta = ((HBINT16*)آغاز_رمز_2) + segcount;

    HBUINT16 *id_Range_انحراف = serialize_rangeoffset_glyid (c,
                                                           cp_مقصد_gid.iter (),
                                                           انتہا_رمز_2,
                                                           آغاز_رمز_2,
                                                           idDelta,
                                                           segcount);
    اگر (unlikely (!c->check_success (id_Range_انحراف))) واپس;

    یہ->لمبائی = c->لمبائی () - جدول_initpos;
    اگر ((طویل طویل) یہ->لمبائی != (طویل طویل) c->لمبائی () - جدول_initpos)
    {
      c->err (HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز);
      واپس;
    }

    یہ->seg_گنتی_X2 = segcount * 2;
    یہ->اندراج_Selector = hb_max (1u, hb_ثنائی_ہندسہ_storage (segcount)) - 1;
    یہ->تلاش_Range = 2 * (1u << یہ->اندراج_Selector);
    یہ->range_سرکاؤ = segcount * 2 > یہ->تلاش_Range
		       ? 2 * segcount - یہ->تلاش_Range
		       : 0;
  }

  غیرعلامتی get_language () ثابت
  {
    واپس language;
  }

  ساخت accelerator_t
  {
    accelerator_t () {}
    accelerator_t (ثابت CmapSubtableFormat4 *subtable) = حذف;

    خالی init (ثابت CmapSubtableFormat4 *subtable,
	       غیرعلامتی صحیح              subtable_ڈیٹا_سائز)
    {
      seg_گنتی_2 = subtable->seg_گنتی_X2 / 2;
      انتہا_گنتی = subtable->values.arrayZ;
      آغاز_گنتی = انتہا_گنتی + seg_گنتی_2 + 1;
      idDelta = آغاز_گنتی + seg_گنتی_2;
      id_Range_انحراف = idDelta + seg_گنتی_2;
      glyphIdArray = id_Range_انحراف + seg_گنتی_2;

      غیرعلامتی صحیح اقدار_انحراف = 16 + 8 * seg_گنتی_2;
      glyph_Id_Array_لمبائی = subtable_ڈیٹا_سائز > اقدار_انحراف
			   ? (subtable_ڈیٹا_سائز - اقدار_انحراف) / 2
			   : 0;
    }

    منطقی get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) ثابت
    {
      ساخت CustomRange
      {
	صحیح cmp (hb_codepoint_t k,
		 غیرعلامتی distance) ثابت
	{
	  اگر (k > آخری) واپس +1;
	  اگر (k < (&آخری)[distance]/*پہلا*/) واپس -1;
	  واپس 0;
	}
	HBUINT16 آخری;
      };

      ثابت HBUINT16 *ملا = hb_bsearch (codepoint,
					  یہ->انتہا_گنتی,
					  یہ->seg_گنتی_2,
					  حجم (CustomRange),
					  hb_cmp_طریقہ<hb_codepoint_t, CustomRange, غیرعلامتی>,
					  یہ->seg_گنتی_2 + 1);
      اگر (unlikely (!ملا))
	واپس جھوٹ;
      غیرعلامتی صحیح i = ملا - انتہا_گنتی;

      hb_codepoint_t gid;
      غیرعلامتی صحیح range_انحراف = یہ->id_Range_انحراف[i];
      اگر (range_انحراف == 0)
	gid = codepoint + یہ->idDelta[i];
      ورنہ
      {
	/* Somebody has been smoking... */
	غیرعلامتی صحیح اشاریہ_2 = range_انحراف / 2 + (codepoint - یہ->آغاز_گنتی[i]) + i - یہ->seg_گنتی_2;
	اگر (unlikely (اشاریہ_2 >= یہ->glyph_Id_Array_لمبائی))
	  واپس جھوٹ;
	gid = یہ->glyphIdArray[اشاریہ_2];
	اگر (unlikely (!gid))
	  واپس جھوٹ;
	gid += یہ->idDelta[i];
      }
      gid &= 0xFFFFu;
      اگر (unlikely (!gid))
	واپس جھوٹ;
      *glyph = gid;
      واپس سچ;
    }

    HB_اندرونی ساکن منطقی get_glyph_func (ثابت خالی *obj, hb_codepoint_t codepoint, hb_codepoint_t *glyph)
    { واپس ((ثابت accelerator_t *) obj)->get_glyph (codepoint, glyph); }

    خالی collect_unicodes (hb_سیٹ_t *برآمد) ثابت
    {
      غیرعلامتی صحیح گنتی = یہ->seg_گنتی_2;
      اگر (گنتی && یہ->آغاز_گنتی[گنتی - 1] == 0xFFFFu)
	گنتی--; /* Skip sentinel segment. */
      برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
      {
	hb_codepoint_t آغاز = یہ->آغاز_گنتی[i];
	hb_codepoint_t انتہا = یہ->انتہا_گنتی[i];
	غیرعلامتی صحیح range_انحراف = یہ->id_Range_انحراف[i];
        برآمد->جوڑو_range(آغاز, انتہا);
	اگر (range_انحراف == 0)
	{
	  برائے (hb_codepoint_t codepoint = آغاز; codepoint <= انتہا; codepoint++)
	  {
	    hb_codepoint_t gid = (codepoint + یہ->idDelta[i]) & 0xFFFFu;
	    اگر (unlikely (!gid))
              برآمد->del(codepoint);
	  }
	}
	ورنہ
	{
	  برائے (hb_codepoint_t codepoint = آغاز; codepoint <= انتہا; codepoint++)
	  {
	    غیرعلامتی صحیح اشاریہ_2 = range_انحراف / 2 + (codepoint - یہ->آغاز_گنتی[i]) + i - یہ->seg_گنتی_2;
	    اگر (unlikely (اشاریہ_2 >= یہ->glyph_Id_Array_لمبائی))
            {
              برآمد->del_range (codepoint, انتہا);
	      توڑو;
            }
	    hb_codepoint_t gid = یہ->glyphIdArray[اشاریہ_2];
	    اگر (unlikely (!gid))
              برآمد->del(codepoint);
	  }
	}
      }
    }

    خالی collect_mapping (hb_سیٹ_t *unicodes, /*برآمد*/
			  hb_map_t *mapping /*برآمد*/) ثابت
    {
      // TODO(grieger): optimize similar to collect_unicodes
      // (ie. use add_range())
      غیرعلامتی گنتی = یہ->seg_گنتی_2;
      اگر (گنتی && یہ->آغاز_گنتی[گنتی - 1] == 0xFFFFu)
	گنتی--; /* Skip sentinel segment. */
      برائے (غیرعلامتی i = 0; i < گنتی; i++)
      {
	hb_codepoint_t آغاز = یہ->آغاز_گنتی[i];
	hb_codepoint_t انتہا = یہ->انتہا_گنتی[i];
	غیرعلامتی range_انحراف = یہ->id_Range_انحراف[i];
	اگر (range_انحراف == 0)
	{
	  برائے (hb_codepoint_t codepoint = آغاز; codepoint <= انتہا; codepoint++)
	  {
	    hb_codepoint_t gid = (codepoint + یہ->idDelta[i]) & 0xFFFFu;
	    اگر (unlikely (!gid))
	      جاری;
	    unicodes->جوڑو (codepoint);
	    mapping->سیٹ (codepoint, gid);
	  }
	}
	ورنہ
	{
	  برائے (hb_codepoint_t codepoint = آغاز; codepoint <= انتہا; codepoint++)
	  {
	    غیرعلامتی اشاریہ_2 = range_انحراف / 2 + (codepoint - یہ->آغاز_گنتی[i]) + i - یہ->seg_گنتی_2;
	    اگر (unlikely (اشاریہ_2 >= یہ->glyph_Id_Array_لمبائی))
	      توڑو;
	    hb_codepoint_t gid = یہ->glyphIdArray[اشاریہ_2];
	    اگر (unlikely (!gid))
	      جاری;
            gid = (gid + یہ->idDelta[i]) & 0xFFFFu;
	    unicodes->جوڑو (codepoint);
	    mapping->سیٹ (codepoint, gid);
	  }
	}
      }
    }

    ثابت HBUINT16 *انتہا_گنتی;
    ثابت HBUINT16 *آغاز_گنتی;
    ثابت HBUINT16 *idDelta;
    ثابت HBUINT16 *id_Range_انحراف;
    ثابت HBUINT16 *glyphIdArray;
    غیرعلامتی صحیح seg_گنتی_2;
    غیرعلامتی صحیح glyph_Id_Array_لمبائی;
  };

  منطقی get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph,
		  غیرعلامتی صحیح subtable_ڈیٹا_سائز) ثابت
  {
    accelerator_t accel;
    accel.init (یہ, subtable_ڈیٹا_سائز);
    واپس accel.get_glyph_func (&accel, codepoint, glyph);
  }
  منطقی get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) ثابت
  { واپس جھوٹ; }

  خالی collect_unicodes (hb_سیٹ_t *برآمد, غیرعلامتی صحیح subtable_ڈیٹا_سائز) ثابت
  {
    accelerator_t accel;
    accel.init (یہ, subtable_ڈیٹا_سائز);
    accel.collect_unicodes (برآمد);
  }
  خالی collect_unicodes (hb_سیٹ_t *برآمد) ثابت
  { collect_unicodes (برآمد, لمبائی); }

  خالی collect_mapping (hb_سیٹ_t *unicodes, /*برآمد*/
			hb_map_t *mapping, /*برآمد*/
			غیرعلامتی صحیح subtable_ڈیٹا_سائز) ثابت
  {
    accelerator_t accel;
    accel.init (یہ, subtable_ڈیٹا_سائز);
    accel.collect_mapping (unicodes, mapping);
  }
  خالی collect_mapping (hb_سیٹ_t *unicodes, /*برآمد*/
			hb_map_t *mapping /*برآمد*/) ثابت
  { collect_mapping (unicodes, mapping, لمبائی); }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    اگر (unlikely (!c->check_struct (یہ)))
      return_trace (جھوٹ);
    hb_barrier ();

    return_trace (c->check_range (values, 2 + 4 * seg_گنتی_X2));
  }

  محفوظ:
  HBUINT16	format;		/* Format number is set to 4. */
  HBUINT16	لمبائی;		/* This is the length in bytes of the
				 * subtable. */
  HBUINT16	language;	/* Ignore. */
  HBUINT16	seg_گنتی_X2;	/* 2 x segCount. */
  HBUINT16	تلاش_Range;	/* 2 * (2**floor(log2(segCount))) */
  HBUINT16	اندراج_Selector;	/* log2(searchRange/2) */
  HBUINT16	range_سرکاؤ;	/* 2 x segCount - searchRange */

  Unsized_Array_کا<HBUINT16>
		values;
#اگر 0
  HBUINT16	انتہا_گنتی[seg_گنتی_2];	/* End characterCode for each segment,
					 * last=0xFFFFu. */
  HBUINT16	reservedPad;		/* Set to 0. */
  HBUINT16	آغاز_گنتی[seg_گنتی_2];	/* Start character code for each segment. */
  HBINT16		idDelta[seg_گنتی_2];	/* Delta for all character codes in segment. */
  HBUINT16	id_Range_انحراف[seg_گنتی_2];/* Offsets into glyphIdArray or 0 */
  Unsized_Array_کا<HBUINT16>
		glyphIdArray;	/* Glyph index array (arbitrary length) */
#اگر_ختم

  عوامی:
  DEFINE_سائز_ARRAY (14, values);
};

ساخت Cmap_Subtable_Long_گروہ
{
  دوست ساخت CmapSubtableFormat12;
  دوست ساخت CmapSubtableFormat13;
  سانچہ<قسم_نام U>
  دوست ساخت CmapSubtableLongSegmented;
  دوست ساخت cmap;

  صحیح cmp (hb_codepoint_t codepoint) ثابت
  {
    اگر (codepoint < آغاز_Char_رمز) واپس -1;
    اگر (codepoint > انتہا_Char_رمز)   واپس +1;
    واپس 0;
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ));
  }

  نجی:
  HBUINT32		آغاز_Char_رمز;	/* First character code in this group. */
  HBUINT32		انتہا_Char_رمز;	/* Last character code in this group. */
  HBUINT32		glyphID;	/* Glyph index; interpretation depends on
					 * subtable format. */
  عوامی:
  DEFINE_سائز_STATIC (12);
};
DECLARE_NULL_NAMESPACE_بائٹیں (OT, Cmap_Subtable_Long_گروہ);

سانچہ <قسم_نام UINT>
ساخت CmapSubtableTrimmed
{
  منطقی get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) ثابت
  {
    /* Rely on our implicit array bound-checking. */
    hb_codepoint_t gid = glyphIdArray[codepoint - آغاز_Char_رمز];
    اگر (unlikely (!gid))
      واپس جھوٹ;
    *glyph = gid;
    واپس سچ;
  }

  غیرعلامتی get_language () ثابت
  {
    واپس language;
  }

  خالی collect_unicodes (hb_سیٹ_t *برآمد) ثابت
  {
    hb_codepoint_t آغاز = آغاز_Char_رمز;
    غیرعلامتی صحیح گنتی = glyphIdArray.len;
    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
      اگر (glyphIdArray[i])
	برآمد->جوڑو (آغاز + i);
  }

  خالی collect_mapping (hb_سیٹ_t *unicodes, /*برآمد*/
			hb_map_t *mapping /*برآمد*/) ثابت
  {
    hb_codepoint_t آغاز_cp = آغاز_Char_رمز;
    غیرعلامتی گنتی = glyphIdArray.len;
    برائے (غیرعلامتی i = 0; i < گنتی; i++)
      اگر (glyphIdArray[i])
      {
	hb_codepoint_t unicode = آغاز_cp + i;
	hb_codepoint_t glyphid = glyphIdArray[i];
	unicodes->جوڑو (unicode);
	mapping->سیٹ (unicode, glyphid);
      }
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) && glyphIdArray.sanitize (c));
  }

  محفوظ:
  UINT		formatReserved;	/* Subtable format and (maybe) padding. */
  UINT		لمبائی;		/* Byte length of this subtable. */
  UINT		language;	/* Ignore. */
  UINT		آغاز_Char_رمز;	/* First character code covered. */
  Array_کا<HBGlyphID16, UINT>
		glyphIdArray;	/* Array of glyph index values for character
				 * codes in the range. */
  عوامی:
  DEFINE_سائز_ARRAY (5 * حجم (UINT), glyphIdArray);
};

ساخت CmapSubtableFormat6  : CmapSubtableTrimmed<HBUINT16> {};
ساخت CmapSubtableFormat10 : CmapSubtableTrimmed<HBUINT32> {};

سانچہ <قسم_نام T>
ساخت CmapSubtableLongSegmented
{
  دوست ساخت cmap;

  منطقی get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) ثابت
  {
    hb_codepoint_t gid = T::گروہ_get_glyph (groups.bsearch (codepoint), codepoint);
    اگر (unlikely (!gid))
      واپس جھوٹ;
    *glyph = gid;
    واپس سچ;
  }

  غیرعلامتی get_language () ثابت
  {
    واپس language;
  }

  خالی collect_unicodes (hb_سیٹ_t *برآمد, غیرعلامتی صحیح عدد_glyphs) ثابت
  {
    برائے (غیرعلامتی صحیح i = 0; i < یہ->groups.len; i++)
    {
      hb_codepoint_t آغاز = یہ->groups[i].آغاز_Char_رمز;
      hb_codepoint_t انتہا = hb_min ((hb_codepoint_t) یہ->groups[i].انتہا_Char_رمز,
				   (hb_codepoint_t) HB_UNICODE_MAX);
      hb_codepoint_t gid = یہ->groups[i].glyphID;
      اگر (!gid)
      {
	/* Intention is: if (hb_is_same (T, CmapSubtableFormat13)) continue; */
	اگر (! T::گروہ_get_glyph (یہ->groups[i], انتہا)) جاری;
	آغاز++;
	gid++;
      }
      اگر (unlikely ((غیرعلامتی صحیح) gid >= عدد_glyphs)) جاری;
      اگر (unlikely ((غیرعلامتی صحیح) (gid + انتہا - آغاز) >= عدد_glyphs))
	انتہا = آغاز + (hb_codepoint_t) عدد_glyphs - gid;

      برآمد->جوڑو_range (آغاز, hb_min (انتہا, 0x10FFFFu));
    }
  }

  خالی collect_mapping (hb_سیٹ_t *unicodes, /*برآمد*/
			hb_map_t *mapping, /*برآمد*/
			غیرعلامتی عدد_glyphs) ثابت
  {
    hb_codepoint_t آخری_انتہا = 0;
    غیرعلامتی گنتی = یہ->groups.len;
    برائے (غیرعلامتی i = 0; i < گنتی; i++)
    {
      hb_codepoint_t آغاز = یہ->groups.arrayZ[i].آغاز_Char_رمز;
      hb_codepoint_t انتہا = hb_min ((hb_codepoint_t) یہ->groups.arrayZ[i].انتہا_Char_رمز,
				   (hb_codepoint_t) HB_UNICODE_MAX);
      اگر (unlikely (آغاز > انتہا || آغاز < آخری_انتہا)) {
        // Range is not in order and is invalid, skip it.
        جاری;
      }
      آخری_انتہا = انتہا;


      hb_codepoint_t gid = یہ->groups.arrayZ[i].glyphID;
      اگر (!gid)
      {
        اگر (T::format_نمبر == 13) جاری;
	آغاز++;
	gid++;
      }
      اگر (unlikely ((غیرعلامتی صحیح) gid >= عدد_glyphs)) جاری;
      اگر (unlikely ((غیرعلامتی صحیح) (gid + انتہا - آغاز) >= عدد_glyphs))
	انتہا = آغاز + (hb_codepoint_t) عدد_glyphs - gid;

      mapping->alloc (mapping->get_population () + انتہا - آغاز + 1);

      unicodes->جوڑو_range (آغاز, انتہا);
      برائے (غیرعلامتی cp = آغاز; cp <= انتہا; cp++)
      {
	mapping->سیٹ (cp, gid);
        gid += T::increment;
      }
    }
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) && groups.sanitize (c));
  }

  محفوظ:
  HBUINT16	format;		/* Subtable format; set to 12. */
  HBUINT16	reserved;	/* Reserved; set to 0. */
  HBUINT32	لمبائی;		/* Byte length of this subtable. */
  HBUINT32	language;	/* Ignore. */
  Sorted_Array32_کا<Cmap_Subtable_Long_گروہ>
		groups;		/* Groupings. */
  عوامی:
  DEFINE_سائز_ARRAY (16, groups);
};

ساخت CmapSubtableFormat12 : CmapSubtableLongSegmented<CmapSubtableFormat12>
{
  ساکن constexpr صحیح increment = 1;
  ساکن constexpr صحیح format_نمبر = 12;

  ساکن hb_codepoint_t گروہ_get_glyph (ثابت Cmap_Subtable_Long_گروہ &گروہ,
					 hb_codepoint_t u)
  { واپس likely (گروہ.آغاز_Char_رمز <= گروہ.انتہا_Char_رمز) ?
	   گروہ.glyphID + (u - گروہ.آغاز_Char_رمز) : 0; }


  سانچہ<قسم_نام Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  خالی serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    اگر (!it) واپس;
    غیرعلامتی جدول_initpos = c->لمبائی ();
    اگر (unlikely (!c->extend_min (یہ))) واپس;

    hb_codepoint_t آغاز_Char_رمز = (hb_codepoint_t) -1, انتہا_Char_رمز = (hb_codepoint_t) -1;
    hb_codepoint_t glyphID = 0;

    برائے (ثابت خودکار& _ : +it)
    {
      اگر (آغاز_Char_رمز == (hb_codepoint_t) -1)
      {
	آغاز_Char_رمز = _.پہلا;
	انتہا_Char_رمز = _.پہلا;
	glyphID = _.second;
      }
      ورنہ اگر (!_is_gid_consecutive (انتہا_Char_رمز, آغاز_Char_رمز, glyphID, _.پہلا, _.second))
      {
	Cmap_Subtable_Long_گروہ  grouprecord;
	grouprecord.آغاز_Char_رمز = آغاز_Char_رمز;
	grouprecord.انتہا_Char_رمز = انتہا_Char_رمز;
	grouprecord.glyphID = glyphID;
	c->copy<Cmap_Subtable_Long_گروہ> (grouprecord);

	آغاز_Char_رمز = _.پہلا;
	انتہا_Char_رمز = _.پہلا;
	glyphID = _.second;
      }
      ورنہ
	انتہا_Char_رمز = _.پہلا;
    }

    Cmap_Subtable_Long_گروہ ریکارڈ;
    ریکارڈ.آغاز_Char_رمز = آغاز_Char_رمز;
    ریکارڈ.انتہا_Char_رمز = انتہا_Char_رمز;
    ریکارڈ.glyphID = glyphID;
    c->copy<Cmap_Subtable_Long_گروہ> (ریکارڈ);

    یہ->format = 12;
    یہ->reserved = 0;
    یہ->لمبائی = c->لمبائی () - جدول_initpos;
    یہ->groups.len = (یہ->لمبائی - min_سائز) / Cmap_Subtable_Long_گروہ::static_سائز;
  }

  ساکن size_t get_sub_جدول_سائز (ثابت hb_sorted_سمتیہ_t<Cmap_Subtable_Long_گروہ> &groups_ڈیٹا)
  { واپس 16 + 12 * groups_ڈیٹا.لمبائی; }

  نجی:
  ساکن منطقی _is_gid_consecutive (hb_codepoint_t انتہا_Char_رمز,
				   hb_codepoint_t آغاز_Char_رمز,
				   hb_codepoint_t glyphID,
				   hb_codepoint_t cp,
				   hb_codepoint_t new_gid)
  {
    واپس (cp - 1 == انتہا_Char_رمز) &&
	new_gid == glyphID + (cp - آغاز_Char_رمز);
  }

};

ساخت CmapSubtableFormat13 : CmapSubtableLongSegmented<CmapSubtableFormat13>
{
  ساکن constexpr صحیح increment = 0;
  ساکن constexpr صحیح format_نمبر = 13;

  ساکن hb_codepoint_t گروہ_get_glyph (ثابت Cmap_Subtable_Long_گروہ &گروہ,
					 hb_codepoint_t u HB_غیر_مستعمل)
  { واپس گروہ.glyphID; }
};

قسمتعریف شمار
{
  GLYPH_VARIANT_NOT_ملا = 0,
  GLYPH_VARIANT_ملا = 1,
  GLYPH_VARIANT_USE_DEFAULT = 2
} glyph_variant_t;

ساخت Unicode_قدر_Range
{
  صحیح cmp (ثابت hb_codepoint_t &codepoint) ثابت
  {
    اگر (codepoint < آغاز_Unicode_قدر) واپس -1;
    اگر (codepoint > آغاز_Unicode_قدر + additional_گنتی) واپس +1;
    واپس 0;
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ));
  }

  HBUINT24	آغاز_Unicode_قدر;	/* First value in this range. */
  HBUINT8	additional_گنتی;	/* Number of additional values in this
					 * range. */
  عوامی:
  DEFINE_سائز_STATIC (4);
};

ساخت DefaultUVS : Sorted_Array32_کا<Unicode_قدر_Range>
{
  خالی collect_unicodes (hb_سیٹ_t *برآمد) ثابت
  {
    غیرعلامتی صحیح گنتی = len;
    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
    {
      hb_codepoint_t پہلا = arrayZ[i].آغاز_Unicode_قدر;
      hb_codepoint_t آخری = hb_min ((hb_codepoint_t) (پہلا + arrayZ[i].additional_گنتی),
				    (hb_codepoint_t) HB_UNICODE_MAX);
      برآمد->جوڑو_range (پہلا, آخری);
    }
  }

  DefaultUVS* copy (hb_serialize_context_t *c,
		    ثابت hb_سیٹ_t *unicodes) ثابت
  {
    خودکار *برآمد = c->آغاز_embed<DefaultUVS> ();
    خودکار snap = c->snapshot ();

    HBUINT32 len;
    len = 0;
    اگر (unlikely (!c->copy<HBUINT32> (len))) واپس خالی_اشاریہ;
    غیرعلامتی init_len = c->لمبائی ();

    اگر (یہ->len > unicodes->get_population () * hb_ثنائی_ہندسہ_storage ((غیرعلامتی) یہ->len))
    {
      hb_codepoint_t آغاز = HB_سیٹ_قدر_INVALID;
      hb_codepoint_t انتہا = HB_سیٹ_قدر_INVALID;

      برائے (خودکار u : *unicodes)
      {
        اگر (!as_array ().bsearch (u))
	  جاری;
	اگر (آغاز == HB_سیٹ_قدر_INVALID)
	{
	  آغاز = u;
	  انتہا = آغاز - 1;
	}
	اگر (انتہا + 1 != u || انتہا - آغاز == 255)
        {
	  Unicode_قدر_Range rec;
	  rec.آغاز_Unicode_قدر = آغاز;
	  rec.additional_گنتی = انتہا - آغاز;
	  c->copy<Unicode_قدر_Range> (rec);
	  آغاز = u;
	}
	انتہا = u;
      }
      اگر (آغاز != HB_سیٹ_قدر_INVALID)
      {
	Unicode_قدر_Range rec;
	rec.آغاز_Unicode_قدر = آغاز;
	rec.additional_گنتی = انتہا - آغاز;
	c->copy<Unicode_قدر_Range> (rec);
      }

    }
    ورنہ
    {
      hb_codepoint_t آخری_رمز = HB_سیٹ_قدر_INVALID;
      صحیح گنتی = -1;

      برائے (ثابت Unicode_قدر_Range& _ : *یہ)
      {
	hb_codepoint_t cur_اندراج = (hb_codepoint_t) (_.آغاز_Unicode_قدر - 1);
	hb_codepoint_t انتہا = cur_اندراج + _.additional_گنتی + 2;

	برائے (; unicodes->next (&cur_اندراج) && cur_اندراج < انتہا;)
	{
	  گنتی += 1;
	  اگر (آخری_رمز == HB_سیٹ_قدر_INVALID)
	    آخری_رمز = cur_اندراج;
	  ورنہ اگر (آخری_رمز + گنتی != cur_اندراج || گنتی > 255)
	  {
	    Unicode_قدر_Range rec;
	    rec.آغاز_Unicode_قدر = آخری_رمز;
	    rec.additional_گنتی = گنتی - 1;
	    c->copy<Unicode_قدر_Range> (rec);

	    آخری_رمز = cur_اندراج;
	    گنتی = 0;
	  }
	}
      }

      اگر (آخری_رمز != HB_MAP_قدر_INVALID)
      {
	Unicode_قدر_Range rec;
	rec.آغاز_Unicode_قدر = آخری_رمز;
	rec.additional_گنتی = گنتی;
	c->copy<Unicode_قدر_Range> (rec);
      }
    }

    اگر (c->لمبائی () - init_len == 0)
    {
      c->revert (snap);
      واپس خالی_اشاریہ;
    }
    ورنہ
    {
      اگر (unlikely (!c->check_تفویض (برآمد->len,
                                      (c->لمبائی () - init_len) / Unicode_قدر_Range::static_سائز,
                                      HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز))) واپس خالی_اشاریہ;
      واپس برآمد;
    }
  }

  عوامی:
  DEFINE_سائز_ARRAY (4, *یہ);
};

سانچہ <قسم_نام Types>
ساخت UVSMapping
{
  استعمال GlyphID = قسم_نام Types::HBGlyphID;

  صحیح cmp (ثابت hb_codepoint_t &codepoint) ثابت
  { واپس unicode_قدر.cmp (codepoint); }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ));
  }

  HBUINT24	unicode_قدر;	/* Base Unicode value of the UVS */
  GlyphID
		glyphID;	/* Glyph ID of the UVS. */
  عوامی:
  DEFINE_سائز_STATIC (3 + GlyphID::static_سائز);
};

سانچہ <قسم_نام Types>
ساخت NonDefaultUVS : Sorted_Array32_کا<UVSMapping<Types>>
{
  استعمال Mapping = UVSMapping<Types>;

  خالی collect_unicodes (hb_سیٹ_t *برآمد) ثابت
  {
    برائے (ثابت خودکار& a : یہ->as_array ())
      برآمد->جوڑو (a.unicode_قدر);
  }

  خالی collect_mapping (hb_سیٹ_t *unicodes, /*برآمد*/
			hb_map_t *mapping /*برآمد*/) ثابت
  {
    برائے (ثابت خودکار& a : یہ->as_array ())
    {
      hb_codepoint_t unicode = a.unicode_قدر;
      hb_codepoint_t glyphid = a.glyphID;
      unicodes->جوڑو (unicode);
      mapping->سیٹ (unicode, glyphid);
    }
  }

  خالی احاطہ_glyphs (ثابت hb_سیٹ_t      *unicodes,
		       hb_سیٹ_t            *glyphset) ثابت
  {
    + یہ->as_array ()
    | hb_filter (unicodes, &Mapping::unicode_قدر)
    | hb_map (&Mapping::glyphID)
    | hb_sink (glyphset)
    ;
  }

  NonDefaultUVS* copy (hb_serialize_context_t *c,
		       ثابت hb_سیٹ_t *unicodes,
		       ثابت hb_سیٹ_t *glyphs_requested,
		       ثابت hb_map_t *glyph_map) ثابت
  {
    خودکار *برآمد = c->آغاز_embed<NonDefaultUVS> ();
    خودکار it =
    + یہ->as_array ()
    | hb_filter ([&] (ثابت Mapping& _)
		 {
		   واپس unicodes->has (_.unicode_قدر) || glyphs_requested->has (_.glyphID);
		 })
    ;

    اگر (!it) واپس خالی_اشاریہ;

    HBUINT32 len;
    len = it.len ();
    اگر (unlikely (!c->copy<HBUINT32> (len))) واپس خالی_اشاریہ;

    برائے (ثابت Mapping& _ : it)
    {
      Mapping mapping;
      mapping.unicode_قدر = _.unicode_قدر;
      mapping.glyphID = glyph_map->get (_.glyphID);
      c->copy<Mapping> (mapping);
    }

    واپس برآمد;
  }

  عوامی:
  DEFINE_سائز_ARRAY (4, *یہ);
};

سانچہ <قسم_نام Types>
ساخت Variation_Selector_ریکارڈ
{
  glyph_variant_t get_glyph (hb_codepoint_t codepoint,
			     hb_codepoint_t *glyph,
			     ثابت خالی *بنیاد) ثابت
  {
    اگر ((بنیاد+defaultUVS).bfind (codepoint))
      واپس GLYPH_VARIANT_USE_DEFAULT;
    ثابت UVSMapping<Types> &nonDefault = (بنیاد+nonDefaultUVS).bsearch (codepoint);
    اگر (nonDefault.glyphID)
    {
      *glyph = nonDefault.glyphID;
       واپس GLYPH_VARIANT_ملا;
    }
    واپس GLYPH_VARIANT_NOT_ملا;
  }

  Variation_Selector_ریکارڈ(ثابت Variation_Selector_ریکارڈ& other)
  {
    *یہ = other;
  }

  خالی عامل= (ثابت Variation_Selector_ریکارڈ& other)
  {
    varSelector = other.varSelector;
    HBUINT32 انحراف = other.defaultUVS;
    defaultUVS = انحراف;
    انحراف = other.nonDefaultUVS;
    nonDefaultUVS = انحراف;
  }

  خالی collect_unicodes (hb_سیٹ_t *برآمد, ثابت خالی *بنیاد) ثابت
  {
    (بنیاد+defaultUVS).collect_unicodes (برآمد);
    (بنیاد+nonDefaultUVS).collect_unicodes (برآمد);
  }

  خالی collect_mapping (ثابت خالی *بنیاد,
			hb_سیٹ_t *unicodes, /*برآمد*/
			hb_map_t *mapping /*برآمد*/) ثابت
  {
    (بنیاد+defaultUVS).collect_unicodes (unicodes);
    (بنیاد+nonDefaultUVS).collect_mapping (unicodes, mapping);
  }

  صحیح cmp (ثابت hb_codepoint_t &variation_selector) ثابت
  { واپس varSelector.cmp (variation_selector); }

  منطقی sanitize (hb_sanitize_context_t *c, ثابت خالی *بنیاد) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) &&
		  defaultUVS.sanitize (c, بنیاد) &&
		  nonDefaultUVS.sanitize (c, بنیاد));
  }

  hb_pair_t<غیرعلامتی, غیرعلامتی>
  copy (hb_serialize_context_t *c,
	ثابت hb_سیٹ_t *unicodes,
	ثابت hb_سیٹ_t *glyphs_requested,
	ثابت hb_map_t *glyph_map,
	ثابت خالی *بنیاد) ثابت
  {
    خودکار snap = c->snapshot ();
    خودکار *برآمد = c->embed<Variation_Selector_ریکارڈ> (*یہ);
    اگر (unlikely (!برآمد)) واپس hb_pair (0, 0);

    برآمد->defaultUVS = 0;
    برآمد->nonDefaultUVS = 0;

    غیرعلامتی non_default_uvs_objidx = 0;
    اگر (nonDefaultUVS != 0)
    {
      c->push ();
      اگر (c->copy (بنیاد+nonDefaultUVS, unicodes, glyphs_requested, glyph_map))
	non_default_uvs_objidx = c->نکالو_pack ();
      ورنہ c->نکالو_discard ();
    }

    غیرعلامتی default_uvs_objidx = 0;
    اگر (defaultUVS != 0)
    {
      c->push ();
      اگر (c->copy (بنیاد+defaultUVS, unicodes))
	default_uvs_objidx = c->نکالو_pack ();
      ورنہ c->نکالو_discard ();
    }


    اگر (!default_uvs_objidx && !non_default_uvs_objidx)
      c->revert (snap);

    واپس hb_pair (default_uvs_objidx, non_default_uvs_objidx);
  }

  HBUINT24	varSelector;	/* Variation selector. */
  Offset32_مقصد<DefaultUVS>
		defaultUVS;	/* Offset to Default UVS Table.  May be 0. */
  Offset32_مقصد<NonDefaultUVS<Types>>
		nonDefaultUVS;	/* Offset to Non-Default UVS Table.  May be 0. */
  عوامی:
  DEFINE_سائز_STATIC (11);
};

سانچہ <قسم_نام Types>
ساخت CmapSubtableFormat14_15
{
  ساکن constexpr غیرعلامتی format_قدر = Types::سائز == 2 ? 14 : 15;
  استعمال ریکارڈ_2 = Variation_Selector_ریکارڈ<Types>;
  استعمال NonDefault = NonDefaultUVS<Types>;

  glyph_variant_t get_glyph_variant (hb_codepoint_t codepoint,
				     hb_codepoint_t variation_selector,
				     hb_codepoint_t *glyph) ثابت
  { واپس ریکارڈ.bsearch (variation_selector).get_glyph (codepoint, glyph, یہ); }

  خالی collect_variation_selectors (hb_سیٹ_t *برآمد) ثابت
  {
    برائے (ثابت خودکار& a : ریکارڈ.as_array ())
      برآمد->جوڑو (a.varSelector);
  }
  خالی collect_variation_unicodes (hb_codepoint_t variation_selector,
				   hb_سیٹ_t *برآمد) ثابت
  { ریکارڈ.bsearch (variation_selector).collect_unicodes (برآمد, یہ); }

  خالی serialize (hb_serialize_context_t *c,
		  ثابت hb_سیٹ_t *unicodes,
		  ثابت hb_سیٹ_t *glyphs_requested,
		  ثابت hb_map_t *glyph_map,
		  ثابت خالی *بنیاد)
  {
    خودکار snap = c->snapshot ();
    غیرعلامتی جدول_initpos = c->لمبائی ();
    ثابت حرف* init_دم = c->دم;

    اگر (unlikely (!c->extend_min (یہ))) واپس;
    یہ->format = format_قدر;

    خودکار src_tbl = reinterpret_cast<ثابت CmapSubtableFormat14_15*> (بنیاد);

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
    hb_سمتیہ_t<hb_pair_t<غیرعلامتی, غیرعلامتی>> obj_indices;
    برائے (صحیح i = src_tbl->ریکارڈ.len - 1; i >= 0; i--)
    {
      اگر (!unicodes->has(src_tbl->ریکارڈ[i].varSelector))
        جاری;

      hb_pair_t<غیرعلامتی, غیرعلامتی> نتیجہ = src_tbl->ریکارڈ[i].copy (c, unicodes, glyphs_requested, glyph_map, بنیاد);
      اگر (نتیجہ.پہلا || نتیجہ.second)
	obj_indices.push (نتیجہ);
    }

    اگر (c->لمبائی () - جدول_initpos == CmapSubtableFormat14_15::min_سائز)
    {
      c->revert (snap);
      واپس;
    }

    اگر (unlikely (!c->check_success (!obj_indices.اندر_error ())))
      واپس;

    صحیح دم_len = init_دم - c->دم;
    c->check_تفویض (یہ->لمبائی, c->لمبائی () - جدول_initpos + دم_len,
                     HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز);
    c->check_تفویض (یہ->ریکارڈ.len,
		     (c->لمبائی () - جدول_initpos - CmapSubtableFormat14_15::min_سائز) /
		     ریکارڈ_2::static_سائز,
                     HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز);

    /* Correct the incorrect write order by reversing the order of the variation
       records array. */
    الٹ_variation_اندراجات ();

    /* Now that records are in the right order, we can set up the offsets. */
    جوڑو_links_مقصد_variation_اندراجات (c, obj_indices);
  }

  خالی الٹ_variation_اندراجات ()
  {
    ریکارڈ.as_array ().reverse ();
  }

  خالی جوڑو_links_مقصد_variation_اندراجات (hb_serialize_context_t *c,
					ثابت hb_سمتیہ_t<hb_pair_t<غیرعلامتی, غیرعلامتی>>& obj_indices)
  {
    برائے (غیرعلامتی i = 0; i < obj_indices.لمبائی; i++)
    {
      /*
       * Since the record array has been reversed (see comments in copy())
       * but obj_indices has not been, the indices at obj_indices[i]
       * are for the variation record at record[j].
       */
      صحیح j = obj_indices.لمبائی - 1 - i;
      c->جوڑو_link (ریکارڈ[j].defaultUVS, obj_indices[i].پہلا);
      c->جوڑو_link (ریکارڈ[j].nonDefaultUVS, obj_indices[i].second);
    }
  }

  خالی احاطہ_glyphs (ثابت hb_سیٹ_t      *unicodes,
		       hb_سیٹ_t            *glyphset) ثابت
  {
    + hb_iter (ریکارڈ)
    | hb_filter (hb_bool, &ریکارڈ_2::nonDefaultUVS)
    | hb_filter (unicodes, &ریکارڈ_2::varSelector)
    | hb_map (&ریکارڈ_2::nonDefaultUVS)
    | hb_map (hb_جوڑو (یہ))
    | hb_apply ([=] (ثابت NonDefault& _) { _.احاطہ_glyphs (unicodes, glyphset); })
    ;
  }

  خالی collect_unicodes (hb_سیٹ_t *برآمد) ثابت
  {
    برائے (ثابت ریکارڈ_2& _ : ریکارڈ)
      _.collect_unicodes (برآمد, یہ);
  }

  خالی collect_mapping (hb_سیٹ_t *unicodes, /*برآمد*/
			hb_map_t *mapping /*برآمد*/) ثابت
  {
    برائے (ثابت ریکارڈ_2& _ : ریکارڈ)
      _.collect_mapping (یہ, unicodes, mapping);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) &&
		  ریکارڈ.sanitize (c, یہ));
  }

  محفوظ:
  HBUINT16	format;		/* Format number is set to 14 or 15. */
  HBUINT32	لمبائی;		/* Byte length of this subtable. */
  Sorted_Array32_کا<ریکارڈ_2>
		ریکارڈ;		/* Variation selector records; sorted
				 * in increasing order of `varSelector'. */
  عوامی:
  DEFINE_سائز_ARRAY (10, ریکارڈ);
};

ساخت CmapSubtable
{
  /* Note: We intentionally do NOT implement subtable formats 2 and 8. */

  منطقی is_variation_selector () ثابت
  {
    انتخاب (u.format.v) {
    حالت 14:
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    حالت 15:
#اگر_ختم
      واپس سچ;
    طےشدہ:
      واپس جھوٹ;
    }
  }

  glyph_variant_t get_glyph_variant (hb_codepoint_t codepoint,
				     hb_codepoint_t variation_selector,
				     hb_codepoint_t *glyph) ثابت
  {
    انتخاب (u.format.v) {
    حالت 14: hb_barrier (); واپس u.format14.get_glyph_variant (codepoint, variation_selector, glyph);
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    حالت 15: hb_barrier (); واپس u.format15.get_glyph_variant (codepoint, variation_selector, glyph);
#اگر_ختم
    طےشدہ: واپس GLYPH_VARIANT_NOT_ملا;
    }
  }

  خالی collect_variation_selectors (hb_سیٹ_t *برآمد) ثابت
  {
    انتخاب (u.format.v) {
    حالت 14: hb_barrier (); u.format14.collect_variation_selectors (برآمد); واپس;
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    حالت 15: hb_barrier (); u.format15.collect_variation_selectors (برآمد); واپس;
#اگر_ختم
    طےشدہ: واپس;
    }
  }

  خالی collect_variation_unicodes (hb_codepoint_t variation_selector,
				    hb_سیٹ_t *برآمد) ثابت
  {
    انتخاب (u.format.v) {
    حالت 14: hb_barrier (); u.format14.collect_variation_unicodes (variation_selector, برآمد); واپس;
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    حالت 15: hb_barrier (); u.format15.collect_variation_unicodes (variation_selector, برآمد); واپس;
#اگر_ختم
    طےشدہ: واپس;
    }
  }

  خالی احاطہ_glyphs (ثابت hb_سیٹ_t *unicodes, hb_سیٹ_t *glyphset) ثابت
  {
    انتخاب (u.format.v) {
    حالت 14: hb_barrier (); u.format14.احاطہ_glyphs (unicodes, glyphset); واپس;
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    حالت 15: hb_barrier (); u.format15.احاطہ_glyphs (unicodes, glyphset); واپس;
#اگر_ختم
    طےشدہ: واپس;
    }
  }

  منطقی get_glyph (hb_codepoint_t codepoint,
		  hb_codepoint_t *glyph,
		  غیرعلامتی صحیح subtable_ڈیٹا_سائز = 0) ثابت
  {
    انتخاب (u.format.v) {
    حالت  0: hb_barrier (); واپس u.format0 .get_glyph (codepoint, glyph);
    حالت  4: hb_barrier (); واپس subtable_ڈیٹا_سائز
				      ? u.format4.get_glyph (codepoint, glyph, subtable_ڈیٹا_سائز)
				      : جھوٹ;
    حالت  6: hb_barrier (); واپس u.format6 .get_glyph (codepoint, glyph);
    حالت 10: hb_barrier (); واپس u.format10.get_glyph (codepoint, glyph);
    حالت 12: hb_barrier (); واپس u.format12.get_glyph (codepoint, glyph);
    حالت 13: hb_barrier (); واپس u.format13.get_glyph (codepoint, glyph);
    حالت 14:
    طےشدہ: واپس جھوٹ;
    }
  }
  خالی collect_unicodes (hb_سیٹ_t *برآمد,
			 غیرعلامتی صحیح عدد_glyphs = UINT_MAX,
			 غیرعلامتی صحیح subtable_ڈیٹا_سائز = 0) ثابت
  {
    انتخاب (u.format.v) {
    حالت  0: hb_barrier (); u.format0 .collect_unicodes (برآمد); واپس;
    حالت  4: hb_barrier (); اگر (subtable_ڈیٹا_سائز)
			      u.format4.collect_unicodes (برآمد, subtable_ڈیٹا_سائز);
			    ورنہ
			      u.format4.collect_unicodes (برآمد);
			    واپس;
    حالت  6: hb_barrier (); u.format6 .collect_unicodes (برآمد); واپس;
    حالت 10: hb_barrier (); u.format10.collect_unicodes (برآمد); واپس;
    حالت 12: hb_barrier (); u.format12.collect_unicodes (برآمد, عدد_glyphs); واپس;
    حالت 13: hb_barrier (); u.format13.collect_unicodes (برآمد, عدد_glyphs); واپس;
    حالت 14:
    طےشدہ: واپس;
    }
  }

  خالی collect_mapping (hb_سیٹ_t *unicodes, /*برآمد*/
			hb_map_t *mapping, /*برآمد*/
			غیرعلامتی عدد_glyphs = UINT_MAX,
			غیرعلامتی صحیح subtable_ڈیٹا_سائز = 0) ثابت
  {
    انتخاب (u.format.v) {
    حالت  0: hb_barrier (); u.format0 .collect_mapping (unicodes, mapping); واپس;
    حالت  4: hb_barrier (); اگر (subtable_ڈیٹا_سائز)
			      u.format4.collect_mapping (unicodes, mapping, subtable_ڈیٹا_سائز);
			    ورنہ
			      u.format4.collect_mapping (unicodes, mapping);
			    واپس;
    حالت  6: hb_barrier (); u.format6 .collect_mapping (unicodes, mapping); واپس;
    حالت 10: hb_barrier (); u.format10.collect_mapping (unicodes, mapping); واپس;
    حالت 12: hb_barrier (); u.format12.collect_mapping (unicodes, mapping, عدد_glyphs); واپس;
    حالت 13: hb_barrier (); u.format13.collect_mapping (unicodes, mapping, عدد_glyphs); واپس;
    حالت 14:
    طےشدہ: واپس;
    }
  }

  غیرعلامتی get_language () ثابت
  {
    انتخاب (u.format.v) {
    حالت  0: hb_barrier (); واپس u.format0 .get_language ();
    حالت  4: hb_barrier (); واپس u.format4 .get_language ();
    حالت  6: hb_barrier (); واپس u.format6 .get_language ();
    حالت 10: hb_barrier (); واپس u.format10.get_language ();
    حالت 12: hb_barrier (); واپس u.format12.get_language ();
    حالت 13: hb_barrier (); واپس u.format13.get_language ();
    حالت 14:
    طےشدہ: واپس 0;
    }
  }

  سانچہ<قسم_نام Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  خالی serialize (hb_serialize_context_t *c,
		  Iterator it,
		  غیرعلامتی format,
		  ثابت hb_subset_plan_t *plan,
		  ثابت خالی *بنیاد)
  {
    انتخاب (format) {
    حالت  4: hb_barrier (); واپس u.format4.serialize (c, it);
    حالت 12: hb_barrier (); واپس u.format12.serialize (c, it);
    حالت 14: hb_barrier (); واپس u.format14.serialize (c, &plan->unicodes, &plan->glyphs_requested, plan->glyph_map, بنیاد);
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    حالت 15: hb_barrier (); واپس u.format15.serialize (c, &plan->unicodes, &plan->glyphs_requested, plan->glyph_map, بنیاد);
#اگر_ختم
    طےشدہ: واپس;
    }
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    اگر (!u.format.v.sanitize (c)) return_trace (جھوٹ);
    hb_barrier ();
    انتخاب (u.format.v) {
    حالت  0: hb_barrier (); return_trace (u.format0 .sanitize (c));
    حالت  4: hb_barrier (); return_trace (u.format4 .sanitize (c));
    حالت  6: hb_barrier (); return_trace (u.format6 .sanitize (c));
    حالت 10: hb_barrier (); return_trace (u.format10.sanitize (c));
    حالت 12: hb_barrier (); return_trace (u.format12.sanitize (c));
    حالت 13: hb_barrier (); return_trace (u.format13.sanitize (c));
    حالت 14: hb_barrier (); return_trace (u.format14.sanitize (c));
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    حالت 15: hb_barrier (); return_trace (u.format15.sanitize (c));
#اگر_ختم
    طےشدہ:return_trace (سچ);
    }
  }

  عوامی:
  اتحاد {
  ساخت { HBUINT16 v; }	format;		/* Format identifier */
  CmapSubtableFormat0	format0;
  CmapSubtableFormat4	format4;
  CmapSubtableFormat6	format6;
  CmapSubtableFormat10	format10;
  CmapSubtableFormat12	format12;
  CmapSubtableFormat13	format13;
  CmapSubtableFormat14_15<Layout::SmallTypes>
			format14;
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
  CmapSubtableFormat14_15<Layout::MediumTypes>
			format15;
#اگر_ختم
  } u;
  عوامی:
  DEFINE_سائز_UNION (2, format.v);
};


ساخت Encoding_ریکارڈ
{
  صحیح cmp (ثابت Encoding_ریکارڈ &other) ثابت
  {
    صحیح ret;
    ret = platformID.cmp (other.platformID);
    اگر (ret) واپس ret;
    اگر (other.encodingID != 0xFFFF)
    {
      ret = encodingID.cmp (other.encodingID);
      اگر (ret) واپس ret;
    }
    واپس 0;
  }

  منطقی sanitize (hb_sanitize_context_t *c, ثابت خالی *بنیاد) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) &&
		  subtable.sanitize (c, بنیاد));
  }

  سانچہ<قسم_نام Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  Encoding_ریکارڈ* copy (hb_serialize_context_t *c,
			Iterator it,
			غیرعلامتی format,
			ثابت خالی *بنیاد,
			ثابت hb_subset_plan_t *plan,
			/* INOUT */ غیرعلامتی *objidx) ثابت
  {
    TRACE_SERIALIZE (یہ);
    خودکار snap = c->snapshot ();
    خودکار *برآمد = c->embed (یہ);
    اگر (unlikely (!برآمد)) return_trace (خالی_اشاریہ);
    برآمد->subtable = 0;

    اگر (*objidx == 0)
    {
      CmapSubtable *cmapsubtable = c->push<CmapSubtable> ();
      غیرعلامتی origin_لمبائی = c->لمبائی ();
      cmapsubtable->serialize (c, it, format, plan, &(بنیاد+subtable));
      اگر (c->لمبائی () > origin_لمبائی && !c->اندر_error()) *objidx = c->نکالو_pack ();
      ورنہ c->نکالو_discard ();
    }

    اگر (*objidx == 0)
    {
      c->revert (snap);
      return_trace (خالی_اشاریہ);
    }

    c->جوڑو_link (برآمد->subtable, *objidx);
    return_trace (برآمد);
  }

  HBUINT16	platformID;	/* Platform ID. */
  HBUINT16	encodingID;	/* Platform-specific encoding ID. */
  Offset32_مقصد<CmapSubtable>
		subtable;	/* Byte offset from beginning of table to the subtable for this encoding. */
  عوامی:
  DEFINE_سائز_STATIC (8);
};

ساخت cmap;

ساخت SubtableUnicodesCache {

 نجی:
  hb_blob_ptr_t<cmap> بنیاد_blob;
  ثابت حرف* بنیاد;
  غیرعلامتی صحیح جدول_لمبائی;
  hb_hashmap_t<غیرعلامتی, hb::unique_ptr<hb_سیٹ_t>> cached_unicodes;

 عوامی:

  ساکن SubtableUnicodesCache* create (hb_blob_ptr_t<cmap> ماخذ_جدول)
  {
    SubtableUnicodesCache* cache =
        (SubtableUnicodesCache*) hb_malloc (حجم(SubtableUnicodesCache));

    اگر (unlikely (!cache))
      واپس خالی_اشاریہ;

    نیا (cache) SubtableUnicodesCache (ماخذ_جدول);
    واپس cache;
  }

  ساکن خالی destroy (خالی* value) {
    اگر (!value) واپس;

    SubtableUnicodesCache* cache = (SubtableUnicodesCache*) value;
    cache->~SubtableUnicodesCache ();
    hb_free (cache);
  }

  SubtableUnicodesCache(ثابت خالی* cmap_بنیاد,
			غیرعلامتی صحیح جدول_لمبائی_2 = 0)
      : بنیاد_blob(),
        بنیاد ((ثابت حرف*) cmap_بنیاد),
        جدول_لمبائی (جدول_لمبائی_2),
        cached_unicodes ()
  {}

  SubtableUnicodesCache(hb_blob_ptr_t<cmap> بنیاد_blob_2)
      : بنیاد_blob(بنیاد_blob_2),
        بنیاد ((ثابت حرف *) بنیاد_blob.get()),
        جدول_لمبائی (بنیاد_blob.get_لمبائی ()),
        cached_unicodes ()
  {}

  ~SubtableUnicodesCache()
  {
    بنیاد_blob.destroy ();
  }

  منطقی same_بنیاد(ثابت خالی* other) ثابت
  {
    واپس other == (ثابت خالی*) بنیاد;
  }

  ثابت hb_سیٹ_t* سیٹ_for (ثابت Encoding_ریکارڈ* ریکارڈ,
                           SubtableUnicodesCache& mutable_cache) ثابت
  {
    اگر (cached_unicodes.has ((غیرعلامتی) ((ثابت حرف *) ریکارڈ - بنیاد)))
      واپس cached_unicodes.get ((غیرعلامتی) ((ثابت حرف *) ریکارڈ - بنیاد));

    واپس mutable_cache.سیٹ_for (ریکارڈ);
  }

  ثابت hb_سیٹ_t* سیٹ_for (ثابت Encoding_ریکارڈ* ریکارڈ)
  {
    اگر (!cached_unicodes.has ((غیرعلامتی) ((ثابت حرف *) ریکارڈ - بنیاد)))
    {
      hb_سیٹ_t *s = hb_سیٹ_create ();
      اگر (unlikely (s->اندر_error ()))
	واپس hb_سیٹ_get_خالی_خانے ();

      غیرعلامتی صحیح subtable_ڈیٹا_سائز = ریکارڈ->subtable < جدول_لمبائی
					? جدول_لمبائی - (غیرعلامتی صحیح) ریکارڈ->subtable
					: 0;
      (بنیاد+ریکارڈ->subtable).collect_unicodes (s, UINT_MAX, subtable_ڈیٹا_سائز);

      اگر (unlikely (!cached_unicodes.سیٹ ((غیرعلامتی) ((ثابت حرف *) ریکارڈ - بنیاد), hb::unique_ptr<hb_سیٹ_t> {s})))
        واپس hb_سیٹ_get_خالی_خانے ();

      واپس s;
    }
    واپس cached_unicodes.get ((غیرعلامتی) ((ثابت حرف *) ریکارڈ - بنیاد));
  }

};

ساکن اندرونی uint16_t
hb_علامت_pua_map (غیرعلامتی codepoint)
{
  اگر (codepoint <= 0x00FFu)
  {
    /* For symbol-encoded OpenType fonts, we duplicate the
     * U+F000..F0FF range at U+0000..U+00FF.  That's what
     * Windows seems to do, and that's hinted about at:
     * https://docs.microsoft.com/en-us/typography/opentype/spec/recom
     * under "Non-Standard (Symbol) Fonts". */
    واپس 0xF000u + codepoint;
  }
  واپس 0;
}

ساخت cmap
{
  ساکن constexpr hb_لیبل_t جدول_لیبل = HB_OT_لیبل_cmap;


  ساکن SubtableUnicodesCache* create_filled_cache(hb_blob_ptr_t<cmap> ماخذ_جدول) {
    ثابت cmap* cmap = ماخذ_جدول.get();
    خودکار it =
    + hb_iter (cmap->encoding_ریکارڈ)
    | hb_filter ([&](ثابت Encoding_ریکارڈ& _) {
      واپس cmap::filter_encoding_اندراجات_for_subset (cmap, _);
    })
    ;

    SubtableUnicodesCache* cache = SubtableUnicodesCache::create(ماخذ_جدول);

    اگر (unlikely (!cache))
      واپس خالی_اشاریہ;

    برائے (ثابت Encoding_ریکارڈ& _ : it)
      cache->سیٹ_for(&_); // populate the cache for this encoding record.

    واپس cache;
  }

  سانچہ<قسم_نام Iterator, قسم_نام EncodingRecIter,
	   hb_requires (hb_is_iterator (EncodingRecIter))>
  منطقی serialize (hb_serialize_context_t *c,
		  Iterator it,
		  EncodingRecIter encodingrec_iter,
		  ثابت خالی *بنیاد,
		  hb_subset_plan_t *plan,
	                  منطقی drop_format_4 = جھوٹ,
			  غیرعلامتی صحیح ماخذ_جدول_لمبائی = 0)
  {
    اگر (unlikely (!c->extend_min ((*یہ))))  واپس جھوٹ;
    یہ->نسخہ = 0;

    غیرعلامتی format4objidx = 0, format12objidx = 0, format14objidx = 0;
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    غیرعلامتی format15objidx = 0;
#اگر_ختم
    خودکار snap = c->snapshot ();

    SubtableUnicodesCache local_unicodes_cache (بنیاد, ماخذ_جدول_لمبائی);
    ثابت SubtableUnicodesCache* unicodes_cache = &local_unicodes_cache;

    اگر (plan->accelerator &&
        plan->accelerator->cmap_cache &&
        plan->accelerator->cmap_cache->same_بنیاد (بنیاد))
      unicodes_cache = plan->accelerator->cmap_cache;

    برائے (ثابت Encoding_ریکارڈ& _ : encodingrec_iter)
    {
      اگر (c->اندر_error ())
        واپس جھوٹ;

      غیرعلامتی format = (بنیاد+_.subtable).u.format.v;
      اگر (format != 4 && format != 12 && format != 14
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
	  && format != 15
#اگر_ختم
	 ) جاری;

      ثابت hb_سیٹ_t* unicodes_سیٹ = unicodes_cache->سیٹ_for (&_, local_unicodes_cache);

      اگر (!drop_format_4 && format == 4)
      {
        c->copy (_, + it | hb_filter (*unicodes_سیٹ, hb_پہلا), 4u, بنیاد, plan, &format4objidx);
        اگر (c->اندر_error () && c->only_حد_سے_تجاوز ())
        {
          // cmap4 overflowed, reset and retry serialization without format 4 subtables.
          c->revert (snap);
          واپس serialize (c, it,
                            encodingrec_iter,
                            بنیاد,
                            plan,
                            سچ,
			    ماخذ_جدول_لمبائی);
        }
      }

      ورنہ اگر (format == 12)
      {
        اگر (_can_drop (_,
                       *unicodes_سیٹ,
                       بنیاد,
                       *unicodes_cache,
                       local_unicodes_cache,
                       + it | hb_map (hb_پہلا), encodingrec_iter))
          جاری;
        c->copy (_, + it | hb_filter (*unicodes_سیٹ, hb_پہلا), 12u, بنیاد, plan, &format12objidx);
      }
      ورنہ اگر (format == 14) c->copy (_, it, 14u, بنیاد, plan, &format14objidx);
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
      ورنہ اگر (format == 15) c->copy (_, it, 15u, بنیاد, plan, &format15objidx);
#اگر_ختم
    }
        غیرعلامتی لمبائی = c->لمبائی ();
        غیرعلامتی دستیاب = لمبائی > cmap::min_سائز ? لمبائی - cmap::min_سائز : 0;
        c->check_تفویض(یہ->encoding_ریکارڈ.len,
                        دستیاب / Encoding_ریکارڈ::static_سائز,
                        HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز);

    // Fail if format 4 was dropped and there is no cmap12.
    واپس !drop_format_4 || format12objidx;
  }

  سانچہ<قسم_نام Iterator, قسم_نام Encoding_ریکارڈ_Iterator,
      hb_requires (hb_is_iterator (Iterator)),
      hb_requires (hb_is_iterator (Encoding_ریکارڈ_Iterator))>
  منطقی _can_drop (ثابت Encoding_ریکارڈ& cmap12,
                  ثابت hb_سیٹ_t& cmap12_unicodes,
                  ثابت خالی* بنیاد,
                  ثابت SubtableUnicodesCache& unicodes_cache,
                  SubtableUnicodesCache& local_unicodes_cache,
                  Iterator subset_unicodes,
                  Encoding_ریکارڈ_Iterator encoding_اندراجات)
  {
    برائے (خودکار cp : + subset_unicodes | hb_filter (cmap12_unicodes))
    {
      اگر (cp >= 0x10000) واپس جھوٹ;
    }

    غیرعلامتی ہدف_platform;
    غیرعلامتی ہدف_encoding;
    غیرعلامتی ہدف_language = (بنیاد+cmap12.subtable).get_language ();

    اگر (cmap12.platformID == 0 && cmap12.encodingID == 4)
    {
      ہدف_platform = 0;
      ہدف_encoding = 3;
    } ورنہ اگر (cmap12.platformID == 3 && cmap12.encodingID == 10) {
      ہدف_platform = 3;
      ہدف_encoding = 1;
    } ورنہ {
      واپس جھوٹ;
    }

    برائے (ثابت خودکار& _ : encoding_اندراجات)
    {
      اگر (_.platformID != ہدف_platform
          || _.encodingID != ہدف_encoding
          || (بنیاد+_.subtable).get_language() != ہدف_language)
        جاری;

      ثابت hb_سیٹ_t* sibling_unicodes = unicodes_cache.سیٹ_for (&_, local_unicodes_cache);

      خودکار cmap12 = + subset_unicodes | hb_filter (cmap12_unicodes);
      خودکار sibling = + subset_unicodes | hb_filter (*sibling_unicodes);
      برائے (; cmap12 && sibling; cmap12++, sibling++)
      {
        غیرعلامتی a = *cmap12;
        غیرعلامتی b = *sibling;
        اگر (a != b) واپس جھوٹ;
      }

      واپس !cmap12 && !sibling;
    }

    واپس جھوٹ;
  }

  خالی احاطہ_glyphs (ثابت hb_سیٹ_t      *unicodes,
		       hb_سیٹ_t            *glyphset) ثابت
  {
    + hb_iter (encoding_ریکارڈ)
    | hb_map (&Encoding_ریکارڈ::subtable)
    | hb_map (hb_جوڑو (یہ))
    | hb_filter (&CmapSubtable::is_variation_selector)
    | hb_apply ([=] (ثابت CmapSubtable& _) { _.احاطہ_glyphs (unicodes, glyphset); })
    ;
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  { واپس subset (c, جھوٹ); }

  منطقی subset (hb_subset_context_t *c, منطقی is_dmap) ثابت
  {
    TRACE_SUBSET (یہ);

    cmap *cmap_مفرد = c->serializer->آغاز_embed<cmap> ();

    خودکار encodingrec_iter =
    + hb_iter (encoding_ریکارڈ)
    | hb_filter ([&](ثابت Encoding_ریکارڈ& _) {
      واپس cmap::filter_encoding_اندراجات_for_subset (یہ, _);
    })
    ;

    اگر (unlikely (!encodingrec_iter.len ())) return_trace (جھوٹ);

    ثابت Encoding_ریکارڈ *unicode_bmp= خالی_اشاریہ, *unicode_ucs4 = خالی_اشاریہ, *ms_bmp = خالی_اشاریہ, *ms_ucs4 = خالی_اشاریہ;
    منطقی has_format12 = جھوٹ;

    برائے (ثابت Encoding_ریکارڈ& _ : encodingrec_iter)
    {
      غیرعلامتی format = (یہ + _.subtable).u.format.v;
      اگر (format == 12) has_format12 = سچ;

      ثابت Encoding_ریکارڈ *جدول = std::addressof (_);
      اگر      (_.platformID == 0 && _.encodingID ==  3) unicode_bmp = جدول;
      ورنہ اگر (_.platformID == 0 && _.encodingID ==  4) unicode_ucs4 = جدول;
      ورنہ اگر (_.platformID == 3 && _.encodingID ==  1) ms_bmp = جدول;
      ورنہ اگر (_.platformID == 3 && _.encodingID == 10) ms_ucs4 = جدول;
    }

    اگر (unlikely (!is_dmap && !has_format12 && !unicode_bmp && !ms_bmp)) return_trace (جھوٹ);
    اگر (unlikely (!is_dmap && has_format12 && (!unicode_ucs4 && !ms_ucs4))) return_trace (جھوٹ);

    خودکار it =
    + c->plan->unicode_مقصد_new_gid_list.iter ()
    | hb_filter ([&] (ثابت hb_codepoint_pair_t _)
		 { واپس (_.second != HB_MAP_قدر_INVALID); })
    ;

    return_trace (cmap_مفرد->serialize (c->serializer,
                                         it,
                                         encodingrec_iter,
                                         یہ,
                                         c->plan,
					 جھوٹ,
					 c->ماخذ_blob->لمبائی));
  }

  ثابت CmapSubtable *ڈھونڈو_بہترین_subtable (منطقی *علامت = خالی_اشاریہ,
					  منطقی *mac = خالی_اشاریہ,
					  منطقی *macroman = خالی_اشاریہ) ثابت
  {
    اگر (علامت) *علامت = جھوٹ;
    اگر (mac) *mac = جھوٹ;
    اگر (macroman) *macroman = جھوٹ;

    ثابت CmapSubtable *subtable;

    /* Symbol subtable.
     * Prefer symbol if available.
     * https://github.com/harfbuzz/harfbuzz/issues/1918 */
    اگر ((subtable = یہ->ڈھونڈو_subtable (3, 0)))
    {
      اگر (علامت) *علامت = سچ;
      واپس subtable;
    }

    /* 32-bit subtables. */
    اگر ((subtable = یہ->ڈھونڈو_subtable (3, 10))) واپس subtable;
    اگر ((subtable = یہ->ڈھونڈو_subtable (0, 6))) واپس subtable;
    اگر ((subtable = یہ->ڈھونڈو_subtable (0, 4))) واپس subtable;

    /* 16-bit subtables. */
    اگر ((subtable = یہ->ڈھونڈو_subtable (3, 1))) واپس subtable;
    اگر ((subtable = یہ->ڈھونڈو_subtable (0, 3))) واپس subtable;
    اگر ((subtable = یہ->ڈھونڈو_subtable (0, 2))) واپس subtable;
    اگر ((subtable = یہ->ڈھونڈو_subtable (0, 1))) واپس subtable;
    اگر ((subtable = یہ->ڈھونڈو_subtable (0, 0))) واپس subtable;

    /* MacRoman subtable. */
    اگر ((subtable = یہ->ڈھونڈو_subtable (1, 0)))
    {
      اگر (mac) *mac = سچ;
      اگر (macroman) *macroman = سچ;
      واپس subtable;
    }
    /* Any other Mac subtable; we just map ASCII for these. */
    اگر ((subtable = یہ->ڈھونڈو_subtable (1, 0xFFFF)))
    {
      اگر (mac) *mac = سچ;
      واپس subtable;
    }

    /* Meh. */
    واپس &Null (CmapSubtable);
  }

  ساخت accelerator_t
  {
    استعمال cache_t = hb_cache_t<21, 19>;
    static_assert (حجم (cache_t) == 1024, "");

    accelerator_t (hb_face_t *face, hb_لیبل_t جدول_لیبل_2 = cmap::جدول_لیبل)
    {
      یہ->جدول = hb_sanitize_context_t ().reference_جدول<cmap> (face, جدول_لیبل_2);
      اگر (!یہ->جدول.get_لمبائی ())
	واپس;

      منطقی علامت, mac, macroman;
      یہ->subtable = جدول->ڈھونڈو_بہترین_subtable (&علامت, &mac, &macroman);
      یہ->subtable_uvs = &Null (CmapSubtable);
      {
	ثابت CmapSubtable *st = جدول->ڈھونڈو_variation_selector_subtable ();
	اگر (st)
	  subtable_uvs = st;
      }

#اگر_غیر_تعریف_شدہ HB_NO_OT_FONT_CMAP_CACHE
      cache = (cache_t *) hb_malloc (حجم (cache_t));
      اگر (cache)
	نیا (cache) cache_t ();
      ورنہ
        واپس; // Such that get_glyph_funcZ remains null.
#اگر_ختم

      یہ->get_glyph_ڈیٹا = subtable;
#اگر_غیر_تعریف_شدہ HB_NO_CMAP_LEGACY_SUBTABLES
      منطقی is_format4 = subtable->u.format.v == 4;
      خودکار سیٹ_format4_getter = [یہ] (منطقی (*func) (ثابت خالی *,
						      hb_codepoint_t,
						      hb_codepoint_t *))
      {
	یہ->format4_accel.init (&یہ->subtable->u.format4,
				  get_subtable_ڈیٹا_سائز (یہ->subtable));
	یہ->get_glyph_ڈیٹا = &یہ->format4_accel;
	یہ->get_glyph_funcZ = func;
      };
      اگر (unlikely (علامت))
      {
	انتخاب ((غیرعلامتی) face->جدول.OS2->get_font_صفحہ ()) {
	حالت OS2::font_صفحہ_t::FONT_صفحہ_NONE:
	  اگر (is_format4)
	    سیٹ_format4_getter (get_glyph_from_علامت<CmapSubtableFormat4::accelerator_t, hb_علامت_pua_map>);
	  ورنہ
	    یہ->get_glyph_funcZ = get_glyph_from_علامت<CmapSubtable, hb_علامت_pua_map>;
	  توڑو;
#اگر_غیر_تعریف_شدہ HB_NO_OT_SHAPER_ARABIC_FALLBACK
	حالت OS2::font_صفحہ_t::FONT_صفحہ_SIMP_ARABIC:
	  اگر (is_format4)
	    سیٹ_format4_getter (get_glyph_from_علامت<CmapSubtableFormat4::accelerator_t, _hb_arabic_pua_simp_map>);
	  ورنہ
	    یہ->get_glyph_funcZ = get_glyph_from_علامت<CmapSubtable, _hb_arabic_pua_simp_map>;
	  توڑو;
	حالت OS2::font_صفحہ_t::FONT_صفحہ_TRAD_ARABIC:
	  اگر (is_format4)
	    سیٹ_format4_getter (get_glyph_from_علامت<CmapSubtableFormat4::accelerator_t, _hb_arabic_pua_trad_map>);
	  ورنہ
	    یہ->get_glyph_funcZ = get_glyph_from_علامت<CmapSubtable, _hb_arabic_pua_trad_map>;
	  توڑو;
#اگر_ختم
	طےشدہ:
	  اگر (is_format4)
	    سیٹ_format4_getter (get_glyph_from<CmapSubtableFormat4::accelerator_t>);
	  ورنہ
	    یہ->get_glyph_funcZ = get_glyph_from<CmapSubtable>;
	  توڑو;
	}
      }
      ورنہ اگر (unlikely (macroman))
      {
	اگر (is_format4)
	  سیٹ_format4_getter (get_glyph_from_macroman<CmapSubtableFormat4::accelerator_t>);
	ورنہ
	  یہ->get_glyph_funcZ = get_glyph_from_macroman<CmapSubtable>;
      }
      ورنہ اگر (unlikely (mac))
      {
	اگر (is_format4)
	  سیٹ_format4_getter (get_glyph_from_ascii<CmapSubtableFormat4::accelerator_t>);
	ورنہ
	  یہ->get_glyph_funcZ = get_glyph_from_ascii<CmapSubtable>;
      }
      ورنہ
#اگر_ختم
      {
	انتخاب (subtable->u.format.v) {
	  /* Accelerate format 4 and format 12. */
	  طےشدہ:
	    یہ->get_glyph_funcZ = get_glyph_from<CmapSubtable>;
	    توڑو;
	  حالت 12:
	    یہ->get_glyph_funcZ = get_glyph_from<CmapSubtableFormat12>;
	    توڑو;
	  حالت  4:
	  {
	    hb_barrier ();
	    یہ->format4_accel.init (&subtable->u.format4,
				      get_subtable_ڈیٹا_سائز (subtable));
	    یہ->get_glyph_ڈیٹا = &یہ->format4_accel;
	    یہ->get_glyph_funcZ = یہ->format4_accel.get_glyph_func;
	    توڑو;
	  }
	}
      }
    }
    ~accelerator_t ()
    {
#اگر_غیر_تعریف_شدہ HB_NO_OT_FONT_CMAP_CACHE
      hb_free (cache);
#اگر_ختم
      جدول.destroy ();
    }

    اندرونی منطقی _cached_get (hb_codepoint_t unicode,
			     hb_codepoint_t *glyph) ثابت
    {
#اگر_غیر_تعریف_شدہ HB_NO_OT_FONT_CMAP_CACHE
      // cache is always non-null if we have a get_glyph_funcZ
      غیرعلامتی v;
      اگر (cache->get (unicode, &v))
      {
        *glyph = v;
	واپس سچ;
      }
#اگر_ختم
      منطقی ret  = یہ->get_glyph_funcZ (یہ->get_glyph_ڈیٹا, unicode, glyph);

#اگر_غیر_تعریف_شدہ HB_NO_OT_FONT_CMAP_CACHE
      اگر (ret)
        cache->سیٹ (unicode, *glyph);
#اگر_ختم

      واپس ret;
    }

    منطقی get_nominal_glyph (hb_codepoint_t  unicode,
			    hb_codepoint_t *glyph) ثابت
    {
      اگر (unlikely (!یہ->get_glyph_funcZ)) واپس جھوٹ;
      واپس _cached_get (unicode, glyph);
    }

    غیرعلامتی صحیح get_nominal_glyphs (غیرعلامتی صحیح گنتی,
				     ثابت hb_codepoint_t *پہلا_unicode,
				     غیرعلامتی صحیح unicode_stride,
				     hb_codepoint_t *پہلا_glyph,
				     غیرعلامتی صحیح glyph_stride) ثابت
    {
      اگر (unlikely (!یہ->get_glyph_funcZ)) واپس 0;

      غیرعلامتی صحیح مکمل;
      برائے (مکمل = 0;
	   مکمل < گنتی && _cached_get (*پہلا_unicode, پہلا_glyph);
	   مکمل++)
      {
	پہلا_unicode = &Struct_At_انحراف_Unaligned<hb_codepoint_t> (پہلا_unicode, unicode_stride);
	پہلا_glyph = &Struct_At_انحراف_Unaligned<hb_codepoint_t> (پہلا_glyph, glyph_stride);
      }
      واپس مکمل;
    }

    منطقی get_variation_glyph (hb_codepoint_t  unicode,
			      hb_codepoint_t  variation_selector,
			      hb_codepoint_t *glyph) ثابت
    {
      انتخاب (get_glyph_variant (unicode, variation_selector, glyph))
      {
	حالت GLYPH_VARIANT_NOT_ملا:	واپس جھوٹ;
	حالت GLYPH_VARIANT_ملا:	واپس سچ;
	حالت GLYPH_VARIANT_USE_DEFAULT:	توڑو;
      }

      واپس get_nominal_glyph (unicode, glyph);
    }

    glyph_variant_t get_glyph_variant (hb_codepoint_t  unicode,
				       hb_codepoint_t  variation_selector,
				       hb_codepoint_t *glyph) ثابت
    {
      واپس یہ->subtable_uvs->get_glyph_variant (unicode,
						    variation_selector,
						    glyph);
    }

    منطقی has_ڈیٹا () ثابت { واپس جدول.get_لمبائی (); }

    خالی collect_unicodes (hb_سیٹ_t *برآمد, غیرعلامتی صحیح عدد_glyphs) ثابت
    { subtable->collect_unicodes (برآمد, عدد_glyphs, get_subtable_ڈیٹا_سائز (subtable)); }
    خالی collect_mapping (hb_سیٹ_t *unicodes, hb_map_t *mapping,
			  غیرعلامتی عدد_glyphs = UINT_MAX) ثابت
    { subtable->collect_mapping (unicodes, mapping, عدد_glyphs, get_subtable_ڈیٹا_سائز (subtable)); }
    خالی collect_variation_selectors (hb_سیٹ_t *برآمد) ثابت
    { subtable_uvs->collect_variation_selectors (برآمد); }
    خالی collect_variation_unicodes (hb_codepoint_t variation_selector,
				     hb_سیٹ_t *برآمد) ثابت
    { subtable_uvs->collect_variation_unicodes (variation_selector, برآمد); }

    محفوظ:
    قسمتعریف منطقی (*hb_cmap_get_glyph_func_t) (ثابت خالی *obj,
					      hb_codepoint_t codepoint,
					      hb_codepoint_t *glyph);
    قسمتعریف uint16_t (*hb_pua_remap_func_t) (غیرعلامتی);

    سانچہ <قسم_نام Type>
    HB_اندرونی ساکن منطقی get_glyph_from (ثابت خالی *obj,
					    hb_codepoint_t codepoint,
					    hb_codepoint_t *glyph)
    {
      ثابت Type *typed_obj = (ثابت Type *) obj;
      واپس typed_obj->get_glyph (codepoint, glyph);
    }

    سانچہ <قسم_نام Type, hb_pua_remap_func_t remap>
    HB_اندرونی ساکن منطقی get_glyph_from_علامت (ثابت خالی *obj,
						   hb_codepoint_t codepoint,
						   hb_codepoint_t *glyph)
    {
      ثابت Type *typed_obj = (ثابت Type *) obj;
      اگر (likely (typed_obj->get_glyph (codepoint, glyph)))
	واپس سچ;

      اگر (hb_codepoint_t c = remap (codepoint))
	واپس typed_obj->get_glyph (c, glyph);

      واپس جھوٹ;
    }

    سانچہ <قسم_نام Type>
    HB_اندرونی ساکن منطقی get_glyph_from_ascii (ثابت خالی *obj,
						  hb_codepoint_t codepoint,
						  hb_codepoint_t *glyph)
    {
      ثابت Type *typed_obj = (ثابت Type *) obj;
      واپس codepoint < 0x80 && typed_obj->get_glyph (codepoint, glyph);
    }

    سانچہ <قسم_نام Type>
    HB_اندرونی ساکن منطقی get_glyph_from_macroman (ثابت خالی *obj,
						     hb_codepoint_t codepoint,
						     hb_codepoint_t *glyph)
    {
      اگر (get_glyph_from_ascii<Type> (obj, codepoint, glyph))
	واپس سچ;

      ثابت Type *typed_obj = (ثابت Type *) obj;
      غیرعلامتی c = unicode_مقصد_macroman (codepoint);
      واپس c && typed_obj->get_glyph (c, glyph);
    }

    غیرعلامتی صحیح get_subtable_ڈیٹا_سائز (ثابت CmapSubtable *subtable) ثابت
    {
      غیرعلامتی صحیح جدول_لمبائی = یہ->جدول.get_لمبائی ();
      uintptr_t جدول_آغاز = (uintptr_t) (ثابت خالی *) یہ->جدول.get ();
      uintptr_t subtable_addr = (uintptr_t) (ثابت خالی *) subtable;
      اگر (unlikely (subtable_addr < جدول_آغاز))
	واپس 0;

      uintptr_t subtable_انحراف = subtable_addr - جدول_آغاز;
      اگر (unlikely (subtable_انحراف >= جدول_لمبائی))
	واپس 0;

      واپس جدول_لمبائی - (غیرعلامتی صحیح) subtable_انحراف;
    }

    نجی:
    hb_nonnull_ptr_t<ثابت CmapSubtable> subtable;
    hb_nonnull_ptr_t<ثابت CmapSubtable> subtable_uvs;

    hb_cmap_get_glyph_func_t get_glyph_funcZ = خالی_اشاریہ;
    ثابت خالی *get_glyph_ڈیٹا = خالی_اشاریہ;

    CmapSubtableFormat4::accelerator_t format4_accel;

#اگر_غیر_تعریف_شدہ HB_NO_OT_FONT_CMAP_CACHE
    cache_t *cache = خالی_اشاریہ;
#اگر_ختم

    عوامی:
    hb_blob_ptr_t<cmap> جدول;
  };

  محفوظ:

  ثابت CmapSubtable *ڈھونڈو_subtable (غیرعلامتی صحیح platform_id,
				     غیرعلامتی صحیح encoding_id) ثابت
  {
    Encoding_ریکارڈ key;
    key.platformID = platform_id;
    key.encodingID = encoding_id;

    ثابت Encoding_ریکارڈ &نتیجہ = encoding_ریکارڈ.bsearch (key);
    اگر (!نتیجہ.subtable)
      واپس خالی_اشاریہ;

    واپس &(یہ+نتیجہ.subtable);
  }

  ثابت CmapSubtable *ڈھونڈو_variation_selector_subtable () ثابت
  {
    ثابت CmapSubtable *format14 = خالی_اشاریہ;
    برائے (ثابت Encoding_ریکارڈ& ریکارڈ : encoding_ریکارڈ)
    {
      اگر (ریکارڈ.platformID != 0 || ریکارڈ.encodingID != 5)
	جاری;

      ثابت CmapSubtable *subtable = &(یہ+ریکارڈ.subtable);
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
      اگر (subtable->u.format.v == 15)
	واپس subtable;
#اگر_ختم
      اگر (subtable->u.format.v == 14)
	format14 = subtable;
    }
    واپس format14;
  }

  عوامی:

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) &&
		  hb_barrier () &&
		  likely (نسخہ == 0) &&
		  encoding_ریکارڈ.sanitize (c, یہ));
  }

 نجی:

  ساکن منطقی filter_encoding_اندراجات_for_subset(ثابت cmap* cmap,
                                                 ثابت Encoding_ریکارڈ& _)
  {
    واپس
        (_.platformID == 0 && _.encodingID == 3) ||
        (_.platformID == 0 && _.encodingID == 4) ||
        (_.platformID == 3 && _.encodingID == 1) ||
        (_.platformID == 3 && _.encodingID == 10) ||
        (cmap + _.subtable).is_variation_selector ();
  }

  محفوظ:
  HBUINT16	نسخہ;	/* Table version number (0). */
  Sorted_Array16_کا<Encoding_ریکارڈ>
		encoding_ریکارڈ;	/* Encoding tables. */
  عوامی:
  DEFINE_سائز_ARRAY (4, encoding_ریکارڈ);
};

ساخت DMAP : cmap
{
  ساکن constexpr hb_لیبل_t جدول_لیبل = HB_OT_لیبل_DMAP;

  منطقی subset (hb_subset_context_t *c) ثابت
  { واپس cmap::subset (c, سچ); }
};

ساخت cmap_accelerator_t
{
  cmap_accelerator_t (hb_face_t *face) :
    dmap (face, HB_OT_لیبل_DMAP),
    بنیاد (face)
  {}

  منطقی get_nominal_glyph (hb_codepoint_t  unicode,
			  hb_codepoint_t *glyph) ثابت
  {
    واپس dmap.get_nominal_glyph (unicode, glyph) ||
	   بنیاد.get_nominal_glyph (unicode, glyph);
  }

  غیرعلامتی صحیح get_nominal_glyphs (غیرعلامتی صحیح گنتی,
				   ثابت hb_codepoint_t *پہلا_unicode,
				   غیرعلامتی صحیح unicode_stride,
				   hb_codepoint_t *پہلا_glyph,
				   غیرعلامتی صحیح glyph_stride) ثابت
  {
    اگر (likely (!dmap.has_ڈیٹا ()))
      واپس بنیاد.get_nominal_glyphs (گنتی,
				      پہلا_unicode, unicode_stride,
				      پہلا_glyph, glyph_stride);

    غیرعلامتی صحیح مکمل;
    برائے (مکمل = 0;
	 مکمل < گنتی && get_nominal_glyph (*پہلا_unicode, پہلا_glyph);
	 مکمل++)
    {
      پہلا_unicode = &Struct_At_انحراف_Unaligned<hb_codepoint_t> (پہلا_unicode, unicode_stride);
      پہلا_glyph = &Struct_At_انحراف_Unaligned<hb_codepoint_t> (پہلا_glyph, glyph_stride);
    }
    واپس مکمل;
  }

  منطقی get_variation_glyph (hb_codepoint_t  unicode,
			    hb_codepoint_t  variation_selector,
			    hb_codepoint_t *glyph) ثابت
  {
    glyph_variant_t نتیجہ = dmap.get_glyph_variant (unicode,
						     variation_selector,
						     glyph);
    اگر (نتیجہ == GLYPH_VARIANT_NOT_ملا)
      نتیجہ = بنیاد.get_glyph_variant (unicode, variation_selector, glyph);

    انتخاب (نتیجہ)
    {
      حالت GLYPH_VARIANT_NOT_ملا: واپس جھوٹ;
      حالت GLYPH_VARIANT_ملا: واپس سچ;
      حالت GLYPH_VARIANT_USE_DEFAULT: توڑو;
    }

    واپس get_nominal_glyph (unicode, glyph);
  }

  خالی collect_unicodes (hb_سیٹ_t *برآمد, غیرعلامتی صحیح عدد_glyphs) ثابت
  {
    بنیاد.collect_unicodes (برآمد, عدد_glyphs);
    dmap.collect_unicodes (برآمد, عدد_glyphs);
  }

  خالی collect_mapping (hb_سیٹ_t *unicodes, hb_map_t *mapping,
			غیرعلامتی عدد_glyphs = UINT_MAX) ثابت
  {
    بنیاد.collect_mapping (unicodes, mapping, عدد_glyphs);
    dmap.collect_mapping (unicodes, mapping, عدد_glyphs);
  }

  خالی collect_variation_selectors (hb_سیٹ_t *برآمد) ثابت
  {
    بنیاد.collect_variation_selectors (برآمد);
    dmap.collect_variation_selectors (برآمد);
  }

  خالی collect_variation_unicodes (hb_codepoint_t variation_selector,
				   hb_سیٹ_t *برآمد) ثابت
  {
    بنیاد.collect_variation_unicodes (variation_selector, برآمد);
    dmap.collect_variation_unicodes (variation_selector, برآمد);
  }

  خالی احاطہ_glyphs (ثابت hb_سیٹ_t *unicodes,
		       hb_سیٹ_t       *glyphset) ثابت
  {
    اگر (likely (!dmap.has_ڈیٹا ()))
    {
      بنیاد.جدول->احاطہ_glyphs (unicodes, glyphset);
      واپس;
    }

    hb_سیٹ_t variation_selectors;
    collect_variation_selectors (&variation_selectors);
    برائے (hb_codepoint_t variation_selector : variation_selectors)
    {
      اگر (!unicodes->has (variation_selector))
	جاری;

      hb_سیٹ_t variation_unicodes;
      collect_variation_unicodes (variation_selector, &variation_unicodes);
      برائے (hb_codepoint_t unicode : variation_unicodes)
      {
	اگر (!unicodes->has (unicode))
	  جاری;

	hb_codepoint_t glyph;
	اگر (get_variation_glyph (unicode, variation_selector, &glyph))
	  glyphset->جوڑو (glyph);
      }
    }
  }

  cmap::accelerator_t dmap;
  cmap::accelerator_t بنیاد;
};

} /* namespace OT */


#اگر_ختم /* HB_OT_CMAP_TABLE_HH */
