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

#جے_بنا_تعریف HB_OT_CMAP_جدول_HH
#تعریف_کرو HB_OT_CMAP_جدول_HH

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
#تعریف_کرو HB_OT_ٹیگ_cmap HB_ٹیگ('c','m','a','p')
#تعریف_کرو HB_OT_ٹیگ_DMAP HB_ٹیگ('D','M','A','P')

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
  خودکار *c = hb_bsearch (u, mapping, ARRAY_لمائی (mapping), حجم (mapping[0]),
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

  بنانشانی get_language () ثابت
  {
    أرجع language;
  }

  فراغ collect_unicodes (hb_سیٹ_t *باہرلا) ثابت
  {
    لكل (بنانشانی صحيح i = 0; i < 256; i++)
      إذا (glyphIdArray[i])
	باہرلا->جوڑو (i);
  }

  فراغ collect_mapping (hb_سیٹ_t *unicodes, /*باہرلا*/
			hb_map_t *mapping /*باہرلا*/) ثابت
  {
    لكل (بنانشانی i = 0; i < 256; i++)
      إذا (glyphIdArray[i])
      {
	hb_codepoint_t glyph = glyphIdArray[i];
	unicodes->جوڑو (i);
	mapping->سیٹ (i, glyph);
      }
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  محمي:
  HBUINT16	format;		/* Format number is set to 0. */
  HBUINT16	لمائی;		/* Byte length of this subtable. */
  HBUINT16	language;	/* Ignore. */
  HBUINT8	glyphIdArray[256];/* An array that maps character
				 * code to glyph index values. */
  عام:
  DEFINE_ناپ_STATIC (6 + 256);
};

بنية CmapSubtableFormat4
{
  سانچا<قسم_ناں Iterator,
      قسم_ناں Writer,
	   hb_requires (hb_is_iterator (Iterator))>
  فراغ منزل_ranges (Iterator it, Writer& range_writer)
  {
    hb_codepoint_t شروع_cp = 0, prev_لگاتار_شروع_cp = 0, لگاتار_شروع_cp = 0, سرا_cp = 0, آخری_gid = 0;
    صحيح لگاتار_لمائی = 0 , delta = 0, prev_delta = 0;

    تعداد {
      پہلا_SUB_RANGE,
      FOLLOWING_SUB_RANGE,
    } منوال;

    طالما (it) {
      // Start a new range
      {
        ثابت خودکار& pair = *it;
        شروع_cp = pair.پہلا;
        prev_لگاتار_شروع_cp = شروع_cp;
        لگاتار_شروع_cp = شروع_cp;
        سرا_cp = شروع_cp;
        آخری_gid = pair.second;
        لگاتار_لمائی = 1;
        prev_delta = 0;
      }

      delta = آخری_gid - شروع_cp;
      منوال = پہلا_SUB_RANGE;
      it++;

      طالما (it) {
        // Process range
        ثابت خودکار& pair = *it;
        hb_codepoint_t اگلا_cp = pair.پہلا;
        hb_codepoint_t اگلا_gid = pair.second;
        إذا (اگلا_cp != سرا_cp + 1) {
          // Current range is over, stop processing.
          اكسر;
        }

        إذا (اگلا_gid == آخری_gid + 1) {
          // The current run continues.
          سرا_cp = اگلا_cp;
          لگاتار_لمائی++;
          آخری_gid = اگلا_gid;
          it++;
          استمر;
        }

        // A new run is starting, decide if we want to commit the current run.
        صحيح ونڈو_لاگت = (منوال == پہلا_SUB_RANGE) ? 8 : 16;
        صحيح لگاتار_لاگت = لگاتار_لمائی * 2;
        إذا (لگاتار_لاگت >= ونڈو_لاگت) {
          commit_current_range(شروع_cp,
                               prev_لگاتار_شروع_cp,
                               لگاتار_شروع_cp,
                               سرا_cp,
                               delta,
                               prev_delta,
                               ونڈو_لاگت,
                               range_writer);
          شروع_cp = اگلا_cp;
        }

        // Start the new run
        منوال = FOLLOWING_SUB_RANGE;
        prev_لگاتار_شروع_cp = لگاتار_شروع_cp;
        لگاتار_شروع_cp = اگلا_cp;
        سرا_cp = اگلا_cp;
        prev_delta = delta;
        delta = اگلا_gid - لگاتار_شروع_cp;
        لگاتار_لمائی = 1;
        آخری_gid = اگلا_gid;
        it++;
      }

      // Finalize range
      commit_current_range (شروع_cp,
                            prev_لگاتار_شروع_cp,
                            لگاتار_شروع_cp,
                            سرا_cp,
                            delta,
                            prev_delta,
                            8,
                            range_writer);
    }

    إذا (likely (سرا_cp != 0xFFFF)) {
      range_writer (0xFFFF, 0xFFFF, 1);
    }
  }

  /*
   * Writes the current range as either one or two ranges depending on what is most efficient.
   */
  سانچا<قسم_ناں Writer>
  فراغ commit_current_range (hb_codepoint_t شروع,
                             hb_codepoint_t prev_لگاتار_شروع,
                             hb_codepoint_t لگاتار_شروع,
                             hb_codepoint_t سرا,
                             صحيح لگاتار_delta,
                             صحيح پچھلا_لگاتار_delta,
                             صحيح ونڈو_لاگت,
                             Writer& range_writer) {
    منطقي should_ونڈو = خطأ;
    إذا (شروع < لگاتار_شروع && لگاتار_شروع < سرا) {
      صحيح لگاتار_لاگت = (سرا - لگاتار_شروع + 1) * 2;
      إذا (لگاتار_لاگت >= ونڈو_لاگت) {
        should_ونڈو = سچ;
      }
    }

    // A zero delta is also the marker for ranges that need a
    // glyphIdArray; serialize_rangeoffset_glyid() tells the two cases
    // apart by re-checking the mapping.
    إذا (should_ونڈو) {
      إذا (شروع == prev_لگاتار_شروع)
        range_writer (شروع, لگاتار_شروع - 1, پچھلا_لگاتار_delta);
      وإلا
        range_writer (شروع, لگاتار_شروع - 1, 0);
      range_writer (لگاتار_شروع, سرا, لگاتار_delta);
      أرجع;
    }


    إذا (شروع == لگاتار_شروع) {
      // Range is only a run
      range_writer (شروع, سرا, لگاتار_delta);
      أرجع;
    }

    // Write only a single non-run range.
    range_writer (شروع, سرا, 0);
  }

  سانچا<قسم_ناں Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  بنانشانی serialize_لبھو_segcount (Iterator it) {
    بنية Counter {
      بنانشانی segcount = 0;

      فراغ عامل() (hb_codepoint_t شروع,
                       hb_codepoint_t سرا,
                       صحيح delta) {
        segcount++;
      }
    } counter;

    منزل_ranges (+it, counter);
    أرجع counter.segcount;
  }


  سانچا<قسم_ناں Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  منطقي serialize_شروع_سرا_delta_arrays (hb_serialize_context_t *c,
                                         Iterator it,
                                         صحيح segcount)
  {
    بنية Writer {
      hb_serialize_context_t *serializer_;
      HBUINT16* سرا_کوڈ;
      HBUINT16* شروع_کوڈ;
      HBINT16* id_delta_;
      صحيح اشاریہ_3;

      Writer(hb_serialize_context_t *serializer)
          : serializer_(serializer),
            سرا_کوڈ(خالی_سوچک),
            شروع_کوڈ(خالی_سوچک),
            id_delta_(خالی_سوچک),
            اشاریہ_3 (0) {}
      فراغ عامل() (hb_codepoint_t شروع,
                       hb_codepoint_t سرا,
                       صحيح delta) {
        شروع_کوڈ[اشاریہ_3] = شروع;
        سرا_کوڈ[اشاریہ_3] = سرا;
        id_delta_[اشاریہ_3] = delta;
        اشاریہ_3++;
      }
    } writer(c);

    writer.سرا_کوڈ = c->allocate_ناپ<HBUINT16> (HBUINT16::static_ناپ * segcount, خطأ);
    (فراغ) c->allocate_ناپ<HBUINT16> (2); // padding
    writer.شروع_کوڈ = c->allocate_ناپ<HBUINT16> (HBUINT16::static_ناپ * segcount, خطأ);
    writer.id_delta_ = c->allocate_ناپ<HBINT16> (HBINT16::static_ناپ * segcount, خطأ);

    إذا (unlikely (!writer.سرا_کوڈ || !writer.شروع_کوڈ || !writer.id_delta_)) أرجع خطأ;

    منزل_ranges (+it, writer);
    أرجع سچ;
  }

  سانچا<قسم_ناں Iterator,
          hb_requires (hb_is_iterator (Iterator))>
  HBUINT16* serialize_rangeoffset_glyid (hb_serialize_context_t *c,
                                         Iterator it,
					 HBUINT16 *سرا_کوڈ_2,
					 HBUINT16 *شروع_کوڈ_2,
					 HBINT16 *idDelta,
					 بنانشانی segcount)
  {
    hb_map_t cp_منزل_gid { it };

    HBUINT16 *id_Range_وتھ = c->allocate_ناپ<HBUINT16> (HBUINT16::static_ناپ * segcount);
    إذا (unlikely (!c->check_success (id_Range_وتھ))) أرجع خالی_سوچک;
    إذا (unlikely ((حرف *)id_Range_وتھ - (حرف *)idDelta != (صحيح) segcount * (صحيح) HBINT16::static_ناپ)) أرجع خالی_سوچک;

    لكل (بنانشانی i : hb_range (segcount))
    {
      /* A non-zero delta is always a real run. A zero delta is
       * ambiguous: identity mapping, or the marker for a range that
       * needs a glyphIdArray. Only zero deltas need a check. */
      صحيح delta = idDelta[i];
      إذا (delta)
	استمر;
      منطقي use_delta = سچ;
      لكل (hb_codepoint_t cp = شروع_کوڈ_2[i]; cp <= سرا_کوڈ_2[i]; cp++)
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

      id_Range_وتھ[i] = 2 * (c->شروع_embed<HBUINT16> () - id_Range_وتھ - i);
      لكل (hb_codepoint_t cp = شروع_کوڈ_2[i]; cp <= سرا_کوڈ_2[i]; cp++)
      {
        HBUINT16 gid;
        gid = cp_منزل_gid[cp];
        c->copy<HBUINT16> (gid);
      }
    }

    أرجع id_Range_وتھ;
  }

  سانچا<قسم_ناں Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  فراغ serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    خودکار format4_iter =
    + it
    | hb_filter ([&] (ثابت hb_codepoint_pair_t _)
		 { أرجع _.پہلا <= 0xFFFF; })
    ;

    إذا (!format4_iter) أرجع;

    بنانشانی جدول_initpos = c->لمائی ();
    إذا (unlikely (!c->extend_min (هذا))) أرجع;
    هذا->format = 4;

    hb_سمتیہ_t<hb_codepoint_pair_t> cp_منزل_gid {
      format4_iter
    };

    //serialize endCode[], startCode[], idDelta[]
    HBUINT16* سرا_کوڈ_2 = c->شروع_embed<HBUINT16> ();
    بنانشانی segcount = serialize_لبھو_segcount (cp_منزل_gid.iter());
    إذا (unlikely (!serialize_شروع_سرا_delta_arrays (c, cp_منزل_gid.iter(), segcount)))
      أرجع;

    HBUINT16 *شروع_کوڈ_2 = سرا_کوڈ_2 + segcount + 1;
    HBINT16 *idDelta = ((HBINT16*)شروع_کوڈ_2) + segcount;

    HBUINT16 *id_Range_وتھ = serialize_rangeoffset_glyid (c,
                                                           cp_منزل_gid.iter (),
                                                           سرا_کوڈ_2,
                                                           شروع_کوڈ_2,
                                                           idDelta,
                                                           segcount);
    إذا (unlikely (!c->check_success (id_Range_وتھ))) أرجع;

    هذا->لمائی = c->لمائی () - جدول_initpos;
    إذا ((لما لما) هذا->لمائی != (لما لما) c->لمائی () - جدول_initpos)
    {
      c->err (HB_SERIALIZE_ERROR_INT_حد_توں_ودھ);
      أرجع;
    }

    هذا->seg_گنتی_X2 = segcount * 2;
    هذا->اندراج_Selector = hb_max (1u, hb_ثنائی_ہندسہ_storage (segcount)) - 1;
    هذا->تلاش_Range = 2 * (1u << هذا->اندراج_Selector);
    هذا->range_کھسکاؤ = segcount * 2 > هذا->تلاش_Range
		       ? 2 * segcount - هذا->تلاش_Range
		       : 0;
  }

  بنانشانی get_language () ثابت
  {
    أرجع language;
  }

  بنية accelerator_t
  {
    accelerator_t () {}
    accelerator_t (ثابت CmapSubtableFormat4 *subtable) = مٹاؤ;

    فراغ init (ثابت CmapSubtableFormat4 *subtable,
	       بنانشانی صحيح              subtable_انکڑے_ناپ)
    {
      seg_گنتی_2 = subtable->seg_گنتی_X2 / 2;
      سرا_گنتی = subtable->values.arrayZ;
      شروع_گنتی = سرا_گنتی + seg_گنتی_2 + 1;
      idDelta = شروع_گنتی + seg_گنتی_2;
      id_Range_وتھ = idDelta + seg_گنتی_2;
      glyphIdArray = id_Range_وتھ + seg_گنتی_2;

      بنانشانی صحيح اقدار_وتھ = 16 + 8 * seg_گنتی_2;
      glyph_Id_Array_لمائی = subtable_انکڑے_ناپ > اقدار_وتھ
			   ? (subtable_انکڑے_ناپ - اقدار_وتھ) / 2
			   : 0;
    }

    منطقي get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) ثابت
    {
      بنية CustomRange
      {
	صحيح cmp (hb_codepoint_t k,
		 بنانشانی distance) ثابت
	{
	  إذا (k > آخری) أرجع +1;
	  إذا (k < (&آخری)[distance]/*پہلا*/) أرجع -1;
	  أرجع 0;
	}
	HBUINT16 آخری;
      };

      ثابت HBUINT16 *لبھیا = hb_bsearch (codepoint,
					  هذا->سرا_گنتی,
					  هذا->seg_گنتی_2,
					  حجم (CustomRange),
					  hb_cmp_طریقہ<hb_codepoint_t, CustomRange, بنانشانی>,
					  هذا->seg_گنتی_2 + 1);
      إذا (unlikely (!لبھیا))
	أرجع خطأ;
      بنانشانی صحيح i = لبھیا - سرا_گنتی;

      hb_codepoint_t gid;
      بنانشانی صحيح range_وتھ = هذا->id_Range_وتھ[i];
      إذا (range_وتھ == 0)
	gid = codepoint + هذا->idDelta[i];
      وإلا
      {
	/* Somebody has been smoking... */
	بنانشانی صحيح اشاریہ_2 = range_وتھ / 2 + (codepoint - هذا->شروع_گنتی[i]) + i - هذا->seg_گنتی_2;
	إذا (unlikely (اشاریہ_2 >= هذا->glyph_Id_Array_لمائی))
	  أرجع خطأ;
	gid = هذا->glyphIdArray[اشاریہ_2];
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

    HB_اندرونی ساكن منطقي get_glyph_func (ثابت فراغ *obj, hb_codepoint_t codepoint, hb_codepoint_t *glyph)
    { أرجع ((ثابت accelerator_t *) obj)->get_glyph (codepoint, glyph); }

    فراغ collect_unicodes (hb_سیٹ_t *باہرلا) ثابت
    {
      بنانشانی صحيح گنتی = هذا->seg_گنتی_2;
      إذا (گنتی && هذا->شروع_گنتی[گنتی - 1] == 0xFFFFu)
	گنتی--; /* Skip sentinel segment. */
      لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
      {
	hb_codepoint_t شروع = هذا->شروع_گنتی[i];
	hb_codepoint_t سرا = هذا->سرا_گنتی[i];
	بنانشانی صحيح range_وتھ = هذا->id_Range_وتھ[i];
        باہرلا->جوڑو_range(شروع, سرا);
	إذا (range_وتھ == 0)
	{
	  لكل (hb_codepoint_t codepoint = شروع; codepoint <= سرا; codepoint++)
	  {
	    hb_codepoint_t gid = (codepoint + هذا->idDelta[i]) & 0xFFFFu;
	    إذا (unlikely (!gid))
              باہرلا->del(codepoint);
	  }
	}
	وإلا
	{
	  لكل (hb_codepoint_t codepoint = شروع; codepoint <= سرا; codepoint++)
	  {
	    بنانشانی صحيح اشاریہ_2 = range_وتھ / 2 + (codepoint - هذا->شروع_گنتی[i]) + i - هذا->seg_گنتی_2;
	    إذا (unlikely (اشاریہ_2 >= هذا->glyph_Id_Array_لمائی))
            {
              باہرلا->del_range (codepoint, سرا);
	      اكسر;
            }
	    hb_codepoint_t gid = هذا->glyphIdArray[اشاریہ_2];
	    إذا (unlikely (!gid))
              باہرلا->del(codepoint);
	  }
	}
      }
    }

    فراغ collect_mapping (hb_سیٹ_t *unicodes, /*باہرلا*/
			  hb_map_t *mapping /*باہرلا*/) ثابت
    {
      // TODO(grieger): optimize similar to collect_unicodes
      // (ie. use add_range())
      بنانشانی گنتی = هذا->seg_گنتی_2;
      إذا (گنتی && هذا->شروع_گنتی[گنتی - 1] == 0xFFFFu)
	گنتی--; /* Skip sentinel segment. */
      لكل (بنانشانی i = 0; i < گنتی; i++)
      {
	hb_codepoint_t شروع = هذا->شروع_گنتی[i];
	hb_codepoint_t سرا = هذا->سرا_گنتی[i];
	بنانشانی range_وتھ = هذا->id_Range_وتھ[i];
	إذا (range_وتھ == 0)
	{
	  لكل (hb_codepoint_t codepoint = شروع; codepoint <= سرا; codepoint++)
	  {
	    hb_codepoint_t gid = (codepoint + هذا->idDelta[i]) & 0xFFFFu;
	    إذا (unlikely (!gid))
	      استمر;
	    unicodes->جوڑو (codepoint);
	    mapping->سیٹ (codepoint, gid);
	  }
	}
	وإلا
	{
	  لكل (hb_codepoint_t codepoint = شروع; codepoint <= سرا; codepoint++)
	  {
	    بنانشانی اشاریہ_2 = range_وتھ / 2 + (codepoint - هذا->شروع_گنتی[i]) + i - هذا->seg_گنتی_2;
	    إذا (unlikely (اشاریہ_2 >= هذا->glyph_Id_Array_لمائی))
	      اكسر;
	    hb_codepoint_t gid = هذا->glyphIdArray[اشاریہ_2];
	    إذا (unlikely (!gid))
	      استمر;
            gid = (gid + هذا->idDelta[i]) & 0xFFFFu;
	    unicodes->جوڑو (codepoint);
	    mapping->سیٹ (codepoint, gid);
	  }
	}
      }
    }

    ثابت HBUINT16 *سرا_گنتی;
    ثابت HBUINT16 *شروع_گنتی;
    ثابت HBUINT16 *idDelta;
    ثابت HBUINT16 *id_Range_وتھ;
    ثابت HBUINT16 *glyphIdArray;
    بنانشانی صحيح seg_گنتی_2;
    بنانشانی صحيح glyph_Id_Array_لمائی;
  };

  منطقي get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph,
		  بنانشانی صحيح subtable_انکڑے_ناپ) ثابت
  {
    accelerator_t accel;
    accel.init (هذا, subtable_انکڑے_ناپ);
    أرجع accel.get_glyph_func (&accel, codepoint, glyph);
  }
  منطقي get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) ثابت
  { أرجع خطأ; }

  فراغ collect_unicodes (hb_سیٹ_t *باہرلا, بنانشانی صحيح subtable_انکڑے_ناپ) ثابت
  {
    accelerator_t accel;
    accel.init (هذا, subtable_انکڑے_ناپ);
    accel.collect_unicodes (باہرلا);
  }
  فراغ collect_unicodes (hb_سیٹ_t *باہرلا) ثابت
  { collect_unicodes (باہرلا, لمائی); }

  فراغ collect_mapping (hb_سیٹ_t *unicodes, /*باہرلا*/
			hb_map_t *mapping, /*باہرلا*/
			بنانشانی صحيح subtable_انکڑے_ناپ) ثابت
  {
    accelerator_t accel;
    accel.init (هذا, subtable_انکڑے_ناپ);
    accel.collect_mapping (unicodes, mapping);
  }
  فراغ collect_mapping (hb_سیٹ_t *unicodes, /*باہرلا*/
			hb_map_t *mapping /*باہرلا*/) ثابت
  { collect_mapping (unicodes, mapping, لمائی); }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!c->check_struct (هذا)))
      return_trace (خطأ);
    hb_barrier ();

    return_trace (c->check_range (values, 2 + 4 * seg_گنتی_X2));
  }

  محمي:
  HBUINT16	format;		/* Format number is set to 4. */
  HBUINT16	لمائی;		/* This is the length in bytes of the
				 * subtable. */
  HBUINT16	language;	/* Ignore. */
  HBUINT16	seg_گنتی_X2;	/* 2 x segCount. */
  HBUINT16	تلاش_Range;	/* 2 * (2**floor(log2(segCount))) */
  HBUINT16	اندراج_Selector;	/* log2(searchRange/2) */
  HBUINT16	range_کھسکاؤ;	/* 2 x segCount - searchRange */

  Unsized_Array_دا<HBUINT16>
		values;
#إذا 0
  HBUINT16	سرا_گنتی[seg_گنتی_2];	/* End characterCode for each segment,
					 * last=0xFFFFu. */
  HBUINT16	reservedPad;		/* Set to 0. */
  HBUINT16	شروع_گنتی[seg_گنتی_2];	/* Start character code for each segment. */
  HBINT16		idDelta[seg_گنتی_2];	/* Delta for all character codes in segment. */
  HBUINT16	id_Range_وتھ[seg_گنتی_2];/* Offsets into glyphIdArray or 0 */
  Unsized_Array_دا<HBUINT16>
		glyphIdArray;	/* Glyph index array (arbitrary length) */
#جے_انت

  عام:
  DEFINE_ناپ_ARRAY (14, values);
};

بنية Cmap_Subtable_Long_گروہ
{
  یار بنية CmapSubtableFormat12;
  یار بنية CmapSubtableFormat13;
  سانچا<قسم_ناں U>
  یار بنية CmapSubtableLongSegmented;
  یار بنية cmap;

  صحيح cmp (hb_codepoint_t codepoint) ثابت
  {
    إذا (codepoint < شروع_Char_کوڈ) أرجع -1;
    إذا (codepoint > سرا_Char_کوڈ)   أرجع +1;
    أرجع 0;
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  خاص:
  HBUINT32		شروع_Char_کوڈ;	/* First character code in this group. */
  HBUINT32		سرا_Char_کوڈ;	/* Last character code in this group. */
  HBUINT32		glyphID;	/* Glyph index; interpretation depends on
					 * subtable format. */
  عام:
  DEFINE_ناپ_STATIC (12);
};
DECLARE_NULL_NAMESPACE_بائٹاں (OT, Cmap_Subtable_Long_گروہ);

سانچا <قسم_ناں UINT>
بنية CmapSubtableTrimmed
{
  منطقي get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) ثابت
  {
    /* Rely on our implicit array bound-checking. */
    hb_codepoint_t gid = glyphIdArray[codepoint - شروع_Char_کوڈ];
    إذا (unlikely (!gid))
      أرجع خطأ;
    *glyph = gid;
    أرجع سچ;
  }

  بنانشانی get_language () ثابت
  {
    أرجع language;
  }

  فراغ collect_unicodes (hb_سیٹ_t *باہرلا) ثابت
  {
    hb_codepoint_t شروع = شروع_Char_کوڈ;
    بنانشانی صحيح گنتی = glyphIdArray.len;
    لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
      إذا (glyphIdArray[i])
	باہرلا->جوڑو (شروع + i);
  }

  فراغ collect_mapping (hb_سیٹ_t *unicodes, /*باہرلا*/
			hb_map_t *mapping /*باہرلا*/) ثابت
  {
    hb_codepoint_t شروع_cp = شروع_Char_کوڈ;
    بنانشانی گنتی = glyphIdArray.len;
    لكل (بنانشانی i = 0; i < گنتی; i++)
      إذا (glyphIdArray[i])
      {
	hb_codepoint_t unicode = شروع_cp + i;
	hb_codepoint_t glyphid = glyphIdArray[i];
	unicodes->جوڑو (unicode);
	mapping->سیٹ (unicode, glyphid);
      }
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && glyphIdArray.sanitize (c));
  }

  محمي:
  UINT		formatReserved;	/* Subtable format and (maybe) padding. */
  UINT		لمائی;		/* Byte length of this subtable. */
  UINT		language;	/* Ignore. */
  UINT		شروع_Char_کوڈ;	/* First character code covered. */
  Array_دا<HBGlyphID16, UINT>
		glyphIdArray;	/* Array of glyph index values for character
				 * codes in the range. */
  عام:
  DEFINE_ناپ_ARRAY (5 * حجم (UINT), glyphIdArray);
};

بنية CmapSubtableFormat6  : CmapSubtableTrimmed<HBUINT16> {};
بنية CmapSubtableFormat10 : CmapSubtableTrimmed<HBUINT32> {};

سانچا <قسم_ناں T>
بنية CmapSubtableLongSegmented
{
  یار بنية cmap;

  منطقي get_glyph (hb_codepoint_t codepoint, hb_codepoint_t *glyph) ثابت
  {
    hb_codepoint_t gid = T::گروہ_get_glyph (groups.bsearch (codepoint), codepoint);
    إذا (unlikely (!gid))
      أرجع خطأ;
    *glyph = gid;
    أرجع سچ;
  }

  بنانشانی get_language () ثابت
  {
    أرجع language;
  }

  فراغ collect_unicodes (hb_سیٹ_t *باہرلا, بنانشانی صحيح عدد_glyphs) ثابت
  {
    لكل (بنانشانی صحيح i = 0; i < هذا->groups.len; i++)
    {
      hb_codepoint_t شروع = هذا->groups[i].شروع_Char_کوڈ;
      hb_codepoint_t سرا = hb_min ((hb_codepoint_t) هذا->groups[i].سرا_Char_کوڈ,
				   (hb_codepoint_t) HB_UNICODE_MAX);
      hb_codepoint_t gid = هذا->groups[i].glyphID;
      إذا (!gid)
      {
	/* Intention is: if (hb_is_same (T, CmapSubtableFormat13)) continue; */
	إذا (! T::گروہ_get_glyph (هذا->groups[i], سرا)) استمر;
	شروع++;
	gid++;
      }
      إذا (unlikely ((بنانشانی صحيح) gid >= عدد_glyphs)) استمر;
      إذا (unlikely ((بنانشانی صحيح) (gid + سرا - شروع) >= عدد_glyphs))
	سرا = شروع + (hb_codepoint_t) عدد_glyphs - gid;

      باہرلا->جوڑو_range (شروع, hb_min (سرا, 0x10FFFFu));
    }
  }

  فراغ collect_mapping (hb_سیٹ_t *unicodes, /*باہرلا*/
			hb_map_t *mapping, /*باہرلا*/
			بنانشانی عدد_glyphs) ثابت
  {
    hb_codepoint_t آخری_سرا = 0;
    بنانشانی گنتی = هذا->groups.len;
    لكل (بنانشانی i = 0; i < گنتی; i++)
    {
      hb_codepoint_t شروع = هذا->groups.arrayZ[i].شروع_Char_کوڈ;
      hb_codepoint_t سرا = hb_min ((hb_codepoint_t) هذا->groups.arrayZ[i].سرا_Char_کوڈ,
				   (hb_codepoint_t) HB_UNICODE_MAX);
      إذا (unlikely (شروع > سرا || شروع < آخری_سرا)) {
        // Range is not in order and is invalid, skip it.
        استمر;
      }
      آخری_سرا = سرا;


      hb_codepoint_t gid = هذا->groups.arrayZ[i].glyphID;
      إذا (!gid)
      {
        إذا (T::format_نمبر == 13) استمر;
	شروع++;
	gid++;
      }
      إذا (unlikely ((بنانشانی صحيح) gid >= عدد_glyphs)) استمر;
      إذا (unlikely ((بنانشانی صحيح) (gid + سرا - شروع) >= عدد_glyphs))
	سرا = شروع + (hb_codepoint_t) عدد_glyphs - gid;

      mapping->alloc (mapping->get_population () + سرا - شروع + 1);

      unicodes->جوڑو_range (شروع, سرا);
      لكل (بنانشانی cp = شروع; cp <= سرا; cp++)
      {
	mapping->سیٹ (cp, gid);
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
  HBUINT32	لمائی;		/* Byte length of this subtable. */
  HBUINT32	language;	/* Ignore. */
  Sorted_Array32_دا<Cmap_Subtable_Long_گروہ>
		groups;		/* Groupings. */
  عام:
  DEFINE_ناپ_ARRAY (16, groups);
};

بنية CmapSubtableFormat12 : CmapSubtableLongSegmented<CmapSubtableFormat12>
{
  ساكن constexpr صحيح increment = 1;
  ساكن constexpr صحيح format_نمبر = 12;

  ساكن hb_codepoint_t گروہ_get_glyph (ثابت Cmap_Subtable_Long_گروہ &گروہ,
					 hb_codepoint_t u)
  { أرجع likely (گروہ.شروع_Char_کوڈ <= گروہ.سرا_Char_کوڈ) ?
	   گروہ.glyphID + (u - گروہ.شروع_Char_کوڈ) : 0; }


  سانچا<قسم_ناں Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  فراغ serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    إذا (!it) أرجع;
    بنانشانی جدول_initpos = c->لمائی ();
    إذا (unlikely (!c->extend_min (هذا))) أرجع;

    hb_codepoint_t شروع_Char_کوڈ = (hb_codepoint_t) -1, سرا_Char_کوڈ = (hb_codepoint_t) -1;
    hb_codepoint_t glyphID = 0;

    لكل (ثابت خودکار& _ : +it)
    {
      إذا (شروع_Char_کوڈ == (hb_codepoint_t) -1)
      {
	شروع_Char_کوڈ = _.پہلا;
	سرا_Char_کوڈ = _.پہلا;
	glyphID = _.second;
      }
      وإلا إذا (!_is_gid_consecutive (سرا_Char_کوڈ, شروع_Char_کوڈ, glyphID, _.پہلا, _.second))
      {
	Cmap_Subtable_Long_گروہ  grouprecord;
	grouprecord.شروع_Char_کوڈ = شروع_Char_کوڈ;
	grouprecord.سرا_Char_کوڈ = سرا_Char_کوڈ;
	grouprecord.glyphID = glyphID;
	c->copy<Cmap_Subtable_Long_گروہ> (grouprecord);

	شروع_Char_کوڈ = _.پہلا;
	سرا_Char_کوڈ = _.پہلا;
	glyphID = _.second;
      }
      وإلا
	سرا_Char_کوڈ = _.پہلا;
    }

    Cmap_Subtable_Long_گروہ ریکارڈ;
    ریکارڈ.شروع_Char_کوڈ = شروع_Char_کوڈ;
    ریکارڈ.سرا_Char_کوڈ = سرا_Char_کوڈ;
    ریکارڈ.glyphID = glyphID;
    c->copy<Cmap_Subtable_Long_گروہ> (ریکارڈ);

    هذا->format = 12;
    هذا->reserved = 0;
    هذا->لمائی = c->لمائی () - جدول_initpos;
    هذا->groups.len = (هذا->لمائی - min_ناپ) / Cmap_Subtable_Long_گروہ::static_ناپ;
  }

  ساكن size_t get_sub_جدول_ناپ (ثابت hb_sorted_سمتیہ_t<Cmap_Subtable_Long_گروہ> &groups_انکڑے)
  { أرجع 16 + 12 * groups_انکڑے.لمائی; }

  خاص:
  ساكن منطقي _is_gid_consecutive (hb_codepoint_t سرا_Char_کوڈ,
				   hb_codepoint_t شروع_Char_کوڈ,
				   hb_codepoint_t glyphID,
				   hb_codepoint_t cp,
				   hb_codepoint_t new_gid)
  {
    أرجع (cp - 1 == سرا_Char_کوڈ) &&
	new_gid == glyphID + (cp - شروع_Char_کوڈ);
  }

};

بنية CmapSubtableFormat13 : CmapSubtableLongSegmented<CmapSubtableFormat13>
{
  ساكن constexpr صحيح increment = 0;
  ساكن constexpr صحيح format_نمبر = 13;

  ساكن hb_codepoint_t گروہ_get_glyph (ثابت Cmap_Subtable_Long_گروہ &گروہ,
					 hb_codepoint_t u HB_بن_ورتیا)
  { أرجع گروہ.glyphID; }
};

تعريفنوع تعداد
{
  GLYPH_VARIANT_NOT_لبھیا = 0,
  GLYPH_VARIANT_لبھیا = 1,
  GLYPH_VARIANT_USE_DEFAULT = 2
} glyph_variant_t;

بنية Unicode_قدر_Range
{
  صحيح cmp (ثابت hb_codepoint_t &codepoint) ثابت
  {
    إذا (codepoint < شروع_Unicode_قدر) أرجع -1;
    إذا (codepoint > شروع_Unicode_قدر + additional_گنتی) أرجع +1;
    أرجع 0;
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  HBUINT24	شروع_Unicode_قدر;	/* First value in this range. */
  HBUINT8	additional_گنتی;	/* Number of additional values in this
					 * range. */
  عام:
  DEFINE_ناپ_STATIC (4);
};

بنية DefaultUVS : Sorted_Array32_دا<Unicode_قدر_Range>
{
  فراغ collect_unicodes (hb_سیٹ_t *باہرلا) ثابت
  {
    بنانشانی صحيح گنتی = len;
    لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
    {
      hb_codepoint_t پہلا = arrayZ[i].شروع_Unicode_قدر;
      hb_codepoint_t آخری = hb_min ((hb_codepoint_t) (پہلا + arrayZ[i].additional_گنتی),
				    (hb_codepoint_t) HB_UNICODE_MAX);
      باہرلا->جوڑو_range (پہلا, آخری);
    }
  }

  DefaultUVS* copy (hb_serialize_context_t *c,
		    ثابت hb_سیٹ_t *unicodes) ثابت
  {
    خودکار *باہرلا = c->شروع_embed<DefaultUVS> ();
    خودکار snap = c->snapshot ();

    HBUINT32 len;
    len = 0;
    إذا (unlikely (!c->copy<HBUINT32> (len))) أرجع خالی_سوچک;
    بنانشانی init_len = c->لمائی ();

    إذا (هذا->len > unicodes->get_population () * hb_ثنائی_ہندسہ_storage ((بنانشانی) هذا->len))
    {
      hb_codepoint_t شروع = HB_سیٹ_قدر_INVALID;
      hb_codepoint_t سرا = HB_سیٹ_قدر_INVALID;

      لكل (خودکار u : *unicodes)
      {
        إذا (!as_array ().bsearch (u))
	  استمر;
	إذا (شروع == HB_سیٹ_قدر_INVALID)
	{
	  شروع = u;
	  سرا = شروع - 1;
	}
	إذا (سرا + 1 != u || سرا - شروع == 255)
        {
	  Unicode_قدر_Range rec;
	  rec.شروع_Unicode_قدر = شروع;
	  rec.additional_گنتی = سرا - شروع;
	  c->copy<Unicode_قدر_Range> (rec);
	  شروع = u;
	}
	سرا = u;
      }
      إذا (شروع != HB_سیٹ_قدر_INVALID)
      {
	Unicode_قدر_Range rec;
	rec.شروع_Unicode_قدر = شروع;
	rec.additional_گنتی = سرا - شروع;
	c->copy<Unicode_قدر_Range> (rec);
      }

    }
    وإلا
    {
      hb_codepoint_t آخری_کوڈ = HB_سیٹ_قدر_INVALID;
      صحيح گنتی = -1;

      لكل (ثابت Unicode_قدر_Range& _ : *هذا)
      {
	hb_codepoint_t cur_اندراج = (hb_codepoint_t) (_.شروع_Unicode_قدر - 1);
	hb_codepoint_t سرا = cur_اندراج + _.additional_گنتی + 2;

	لكل (; unicodes->next (&cur_اندراج) && cur_اندراج < سرا;)
	{
	  گنتی += 1;
	  إذا (آخری_کوڈ == HB_سیٹ_قدر_INVALID)
	    آخری_کوڈ = cur_اندراج;
	  وإلا إذا (آخری_کوڈ + گنتی != cur_اندراج || گنتی > 255)
	  {
	    Unicode_قدر_Range rec;
	    rec.شروع_Unicode_قدر = آخری_کوڈ;
	    rec.additional_گنتی = گنتی - 1;
	    c->copy<Unicode_قدر_Range> (rec);

	    آخری_کوڈ = cur_اندراج;
	    گنتی = 0;
	  }
	}
      }

      إذا (آخری_کوڈ != HB_MAP_قدر_INVALID)
      {
	Unicode_قدر_Range rec;
	rec.شروع_Unicode_قدر = آخری_کوڈ;
	rec.additional_گنتی = گنتی;
	c->copy<Unicode_قدر_Range> (rec);
      }
    }

    إذا (c->لمائی () - init_len == 0)
    {
      c->revert (snap);
      أرجع خالی_سوچک;
    }
    وإلا
    {
      إذا (unlikely (!c->check_سونپو (باہرلا->len,
                                      (c->لمائی () - init_len) / Unicode_قدر_Range::static_ناپ,
                                      HB_SERIALIZE_ERROR_INT_حد_توں_ودھ))) أرجع خالی_سوچک;
      أرجع باہرلا;
    }
  }

  عام:
  DEFINE_ناپ_ARRAY (4, *هذا);
};

سانچا <قسم_ناں Types>
بنية UVSMapping
{
  ورتوں GlyphID = قسم_ناں Types::HBGlyphID;

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
  DEFINE_ناپ_STATIC (3 + GlyphID::static_ناپ);
};

سانچا <قسم_ناں Types>
بنية NonDefaultUVS : Sorted_Array32_دا<UVSMapping<Types>>
{
  ورتوں Mapping = UVSMapping<Types>;

  فراغ collect_unicodes (hb_سیٹ_t *باہرلا) ثابت
  {
    لكل (ثابت خودکار& a : هذا->as_array ())
      باہرلا->جوڑو (a.unicode_قدر);
  }

  فراغ collect_mapping (hb_سیٹ_t *unicodes, /*باہرلا*/
			hb_map_t *mapping /*باہرلا*/) ثابت
  {
    لكل (ثابت خودکار& a : هذا->as_array ())
    {
      hb_codepoint_t unicode = a.unicode_قدر;
      hb_codepoint_t glyphid = a.glyphID;
      unicodes->جوڑو (unicode);
      mapping->سیٹ (unicode, glyphid);
    }
  }

  فراغ گھیرا_glyphs (ثابت hb_سیٹ_t      *unicodes,
		       hb_سیٹ_t            *glyphset) ثابت
  {
    + هذا->as_array ()
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
    خودکار *باہرلا = c->شروع_embed<NonDefaultUVS> ();
    خودکار it =
    + هذا->as_array ()
    | hb_filter ([&] (ثابت Mapping& _)
		 {
		   أرجع unicodes->has (_.unicode_قدر) || glyphs_requested->has (_.glyphID);
		 })
    ;

    إذا (!it) أرجع خالی_سوچک;

    HBUINT32 len;
    len = it.len ();
    إذا (unlikely (!c->copy<HBUINT32> (len))) أرجع خالی_سوچک;

    لكل (ثابت Mapping& _ : it)
    {
      Mapping mapping;
      mapping.unicode_قدر = _.unicode_قدر;
      mapping.glyphID = glyph_map->get (_.glyphID);
      c->copy<Mapping> (mapping);
    }

    أرجع باہرلا;
  }

  عام:
  DEFINE_ناپ_ARRAY (4, *هذا);
};

سانچا <قسم_ناں Types>
بنية Variation_Selector_ریکارڈ
{
  glyph_variant_t get_glyph (hb_codepoint_t codepoint,
			     hb_codepoint_t *glyph,
			     ثابت فراغ *ادھار) ثابت
  {
    إذا ((ادھار+defaultUVS).bfind (codepoint))
      أرجع GLYPH_VARIANT_USE_DEFAULT;
    ثابت UVSMapping<Types> &nonDefault = (ادھار+nonDefaultUVS).bsearch (codepoint);
    إذا (nonDefault.glyphID)
    {
      *glyph = nonDefault.glyphID;
       أرجع GLYPH_VARIANT_لبھیا;
    }
    أرجع GLYPH_VARIANT_NOT_لبھیا;
  }

  Variation_Selector_ریکارڈ(ثابت Variation_Selector_ریکارڈ& other)
  {
    *هذا = other;
  }

  فراغ عامل= (ثابت Variation_Selector_ریکارڈ& other)
  {
    varSelector = other.varSelector;
    HBUINT32 وتھ = other.defaultUVS;
    defaultUVS = وتھ;
    وتھ = other.nonDefaultUVS;
    nonDefaultUVS = وتھ;
  }

  فراغ collect_unicodes (hb_سیٹ_t *باہرلا, ثابت فراغ *ادھار) ثابت
  {
    (ادھار+defaultUVS).collect_unicodes (باہرلا);
    (ادھار+nonDefaultUVS).collect_unicodes (باہرلا);
  }

  فراغ collect_mapping (ثابت فراغ *ادھار,
			hb_سیٹ_t *unicodes, /*باہرلا*/
			hb_map_t *mapping /*باہرلا*/) ثابت
  {
    (ادھار+defaultUVS).collect_unicodes (unicodes);
    (ادھار+nonDefaultUVS).collect_mapping (unicodes, mapping);
  }

  صحيح cmp (ثابت hb_codepoint_t &variation_selector) ثابت
  { أرجع varSelector.cmp (variation_selector); }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *ادھار) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  defaultUVS.sanitize (c, ادھار) &&
		  nonDefaultUVS.sanitize (c, ادھار));
  }

  hb_pair_t<بنانشانی, بنانشانی>
  copy (hb_serialize_context_t *c,
	ثابت hb_سیٹ_t *unicodes,
	ثابت hb_سیٹ_t *glyphs_requested,
	ثابت hb_map_t *glyph_map,
	ثابت فراغ *ادھار) ثابت
  {
    خودکار snap = c->snapshot ();
    خودکار *باہرلا = c->embed<Variation_Selector_ریکارڈ> (*هذا);
    إذا (unlikely (!باہرلا)) أرجع hb_pair (0, 0);

    باہرلا->defaultUVS = 0;
    باہرلا->nonDefaultUVS = 0;

    بنانشانی non_default_uvs_objidx = 0;
    إذا (nonDefaultUVS != 0)
    {
      c->push ();
      إذا (c->copy (ادھار+nonDefaultUVS, unicodes, glyphs_requested, glyph_map))
	non_default_uvs_objidx = c->نکالو_pack ();
      وإلا c->نکالو_discard ();
    }

    بنانشانی default_uvs_objidx = 0;
    إذا (defaultUVS != 0)
    {
      c->push ();
      إذا (c->copy (ادھار+defaultUVS, unicodes))
	default_uvs_objidx = c->نکالو_pack ();
      وإلا c->نکالو_discard ();
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
  DEFINE_ناپ_STATIC (11);
};

سانچا <قسم_ناں Types>
بنية CmapSubtableFormat14_15
{
  ساكن constexpr بنانشانی format_قدر = Types::ناپ == 2 ? 14 : 15;
  ورتوں ریکارڈ_2 = Variation_Selector_ریکارڈ<Types>;
  ورتوں NonDefault = NonDefaultUVS<Types>;

  glyph_variant_t get_glyph_variant (hb_codepoint_t codepoint,
				     hb_codepoint_t variation_selector,
				     hb_codepoint_t *glyph) ثابت
  { أرجع ریکارڈ.bsearch (variation_selector).get_glyph (codepoint, glyph, هذا); }

  فراغ collect_variation_selectors (hb_سیٹ_t *باہرلا) ثابت
  {
    لكل (ثابت خودکار& a : ریکارڈ.as_array ())
      باہرلا->جوڑو (a.varSelector);
  }
  فراغ collect_variation_unicodes (hb_codepoint_t variation_selector,
				   hb_سیٹ_t *باہرلا) ثابت
  { ریکارڈ.bsearch (variation_selector).collect_unicodes (باہرلا, هذا); }

  فراغ serialize (hb_serialize_context_t *c,
		  ثابت hb_سیٹ_t *unicodes,
		  ثابت hb_سیٹ_t *glyphs_requested,
		  ثابت hb_map_t *glyph_map,
		  ثابت فراغ *ادھار)
  {
    خودکار snap = c->snapshot ();
    بنانشانی جدول_initpos = c->لمائی ();
    ثابت حرف* init_پوچھ = c->پوچھ;

    إذا (unlikely (!c->extend_min (هذا))) أرجع;
    هذا->format = format_قدر;

    خودکار src_tbl = reinterpret_cast<ثابت CmapSubtableFormat14_15*> (ادھار);

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
    hb_سمتیہ_t<hb_pair_t<بنانشانی, بنانشانی>> obj_indices;
    لكل (صحيح i = src_tbl->ریکارڈ.len - 1; i >= 0; i--)
    {
      إذا (!unicodes->has(src_tbl->ریکارڈ[i].varSelector))
        استمر;

      hb_pair_t<بنانشانی, بنانشانی> نتیجہ = src_tbl->ریکارڈ[i].copy (c, unicodes, glyphs_requested, glyph_map, ادھار);
      إذا (نتیجہ.پہلا || نتیجہ.second)
	obj_indices.push (نتیجہ);
    }

    إذا (c->لمائی () - جدول_initpos == CmapSubtableFormat14_15::min_ناپ)
    {
      c->revert (snap);
      أرجع;
    }

    إذا (unlikely (!c->check_success (!obj_indices.وچ_error ())))
      أرجع;

    صحيح پوچھ_len = init_پوچھ - c->پوچھ;
    c->check_سونپو (هذا->لمائی, c->لمائی () - جدول_initpos + پوچھ_len,
                     HB_SERIALIZE_ERROR_INT_حد_توں_ودھ);
    c->check_سونپو (هذا->ریکارڈ.len,
		     (c->لمائی () - جدول_initpos - CmapSubtableFormat14_15::min_ناپ) /
		     ریکارڈ_2::static_ناپ,
                     HB_SERIALIZE_ERROR_INT_حد_توں_ودھ);

    /* Correct the incorrect write order by reversing the order of the variation
       records array. */
    الٹ_variation_ریکارڈاں ();

    /* Now that records are in the right order, we can set up the offsets. */
    جوڑو_links_منزل_variation_ریکارڈاں (c, obj_indices);
  }

  فراغ الٹ_variation_ریکارڈاں ()
  {
    ریکارڈ.as_array ().reverse ();
  }

  فراغ جوڑو_links_منزل_variation_ریکارڈاں (hb_serialize_context_t *c,
					ثابت hb_سمتیہ_t<hb_pair_t<بنانشانی, بنانشانی>>& obj_indices)
  {
    لكل (بنانشانی i = 0; i < obj_indices.لمائی; i++)
    {
      /*
       * Since the record array has been reversed (see comments in copy())
       * but obj_indices has not been, the indices at obj_indices[i]
       * are for the variation record at record[j].
       */
      صحيح j = obj_indices.لمائی - 1 - i;
      c->جوڑو_link (ریکارڈ[j].defaultUVS, obj_indices[i].پہلا);
      c->جوڑو_link (ریکارڈ[j].nonDefaultUVS, obj_indices[i].second);
    }
  }

  فراغ گھیرا_glyphs (ثابت hb_سیٹ_t      *unicodes,
		       hb_سیٹ_t            *glyphset) ثابت
  {
    + hb_iter (ریکارڈ)
    | hb_filter (hb_bool, &ریکارڈ_2::nonDefaultUVS)
    | hb_filter (unicodes, &ریکارڈ_2::varSelector)
    | hb_map (&ریکارڈ_2::nonDefaultUVS)
    | hb_map (hb_جوڑو (هذا))
    | hb_apply ([=] (ثابت NonDefault& _) { _.گھیرا_glyphs (unicodes, glyphset); })
    ;
  }

  فراغ collect_unicodes (hb_سیٹ_t *باہرلا) ثابت
  {
    لكل (ثابت ریکارڈ_2& _ : ریکارڈ)
      _.collect_unicodes (باہرلا, هذا);
  }

  فراغ collect_mapping (hb_سیٹ_t *unicodes, /*باہرلا*/
			hb_map_t *mapping /*باہرلا*/) ثابت
  {
    لكل (ثابت ریکارڈ_2& _ : ریکارڈ)
      _.collect_mapping (هذا, unicodes, mapping);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  ریکارڈ.sanitize (c, هذا));
  }

  محمي:
  HBUINT16	format;		/* Format number is set to 14 or 15. */
  HBUINT32	لمائی;		/* Byte length of this subtable. */
  Sorted_Array32_دا<ریکارڈ_2>
		ریکارڈ;		/* Variation selector records; sorted
				 * in increasing order of `varSelector'. */
  عام:
  DEFINE_ناپ_ARRAY (10, ریکارڈ);
};

بنية CmapSubtable
{
  /* Note: We intentionally do NOT implement subtable formats 2 and 8. */

  منطقي is_variation_selector () ثابت
  {
    بدل (u.format.v) {
    حالة 14:
#جے_بنا_تعریف HB_NO_BEYOND_64K
    حالة 15:
#جے_انت
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
#جے_بنا_تعریف HB_NO_BEYOND_64K
    حالة 15: hb_barrier (); أرجع u.format15.get_glyph_variant (codepoint, variation_selector, glyph);
#جے_انت
    افتراضي: أرجع GLYPH_VARIANT_NOT_لبھیا;
    }
  }

  فراغ collect_variation_selectors (hb_سیٹ_t *باہرلا) ثابت
  {
    بدل (u.format.v) {
    حالة 14: hb_barrier (); u.format14.collect_variation_selectors (باہرلا); أرجع;
#جے_بنا_تعریف HB_NO_BEYOND_64K
    حالة 15: hb_barrier (); u.format15.collect_variation_selectors (باہرلا); أرجع;
#جے_انت
    افتراضي: أرجع;
    }
  }

  فراغ collect_variation_unicodes (hb_codepoint_t variation_selector,
				    hb_سیٹ_t *باہرلا) ثابت
  {
    بدل (u.format.v) {
    حالة 14: hb_barrier (); u.format14.collect_variation_unicodes (variation_selector, باہرلا); أرجع;
#جے_بنا_تعریف HB_NO_BEYOND_64K
    حالة 15: hb_barrier (); u.format15.collect_variation_unicodes (variation_selector, باہرلا); أرجع;
#جے_انت
    افتراضي: أرجع;
    }
  }

  فراغ گھیرا_glyphs (ثابت hb_سیٹ_t *unicodes, hb_سیٹ_t *glyphset) ثابت
  {
    بدل (u.format.v) {
    حالة 14: hb_barrier (); u.format14.گھیرا_glyphs (unicodes, glyphset); أرجع;
#جے_بنا_تعریف HB_NO_BEYOND_64K
    حالة 15: hb_barrier (); u.format15.گھیرا_glyphs (unicodes, glyphset); أرجع;
#جے_انت
    افتراضي: أرجع;
    }
  }

  منطقي get_glyph (hb_codepoint_t codepoint,
		  hb_codepoint_t *glyph,
		  بنانشانی صحيح subtable_انکڑے_ناپ = 0) ثابت
  {
    بدل (u.format.v) {
    حالة  0: hb_barrier (); أرجع u.format0 .get_glyph (codepoint, glyph);
    حالة  4: hb_barrier (); أرجع subtable_انکڑے_ناپ
				      ? u.format4.get_glyph (codepoint, glyph, subtable_انکڑے_ناپ)
				      : خطأ;
    حالة  6: hb_barrier (); أرجع u.format6 .get_glyph (codepoint, glyph);
    حالة 10: hb_barrier (); أرجع u.format10.get_glyph (codepoint, glyph);
    حالة 12: hb_barrier (); أرجع u.format12.get_glyph (codepoint, glyph);
    حالة 13: hb_barrier (); أرجع u.format13.get_glyph (codepoint, glyph);
    حالة 14:
    افتراضي: أرجع خطأ;
    }
  }
  فراغ collect_unicodes (hb_سیٹ_t *باہرلا,
			 بنانشانی صحيح عدد_glyphs = UINT_MAX,
			 بنانشانی صحيح subtable_انکڑے_ناپ = 0) ثابت
  {
    بدل (u.format.v) {
    حالة  0: hb_barrier (); u.format0 .collect_unicodes (باہرلا); أرجع;
    حالة  4: hb_barrier (); إذا (subtable_انکڑے_ناپ)
			      u.format4.collect_unicodes (باہرلا, subtable_انکڑے_ناپ);
			    وإلا
			      u.format4.collect_unicodes (باہرلا);
			    أرجع;
    حالة  6: hb_barrier (); u.format6 .collect_unicodes (باہرلا); أرجع;
    حالة 10: hb_barrier (); u.format10.collect_unicodes (باہرلا); أرجع;
    حالة 12: hb_barrier (); u.format12.collect_unicodes (باہرلا, عدد_glyphs); أرجع;
    حالة 13: hb_barrier (); u.format13.collect_unicodes (باہرلا, عدد_glyphs); أرجع;
    حالة 14:
    افتراضي: أرجع;
    }
  }

  فراغ collect_mapping (hb_سیٹ_t *unicodes, /*باہرلا*/
			hb_map_t *mapping, /*باہرلا*/
			بنانشانی عدد_glyphs = UINT_MAX,
			بنانشانی صحيح subtable_انکڑے_ناپ = 0) ثابت
  {
    بدل (u.format.v) {
    حالة  0: hb_barrier (); u.format0 .collect_mapping (unicodes, mapping); أرجع;
    حالة  4: hb_barrier (); إذا (subtable_انکڑے_ناپ)
			      u.format4.collect_mapping (unicodes, mapping, subtable_انکڑے_ناپ);
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

  بنانشانی get_language () ثابت
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

  سانچا<قسم_ناں Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  فراغ serialize (hb_serialize_context_t *c,
		  Iterator it,
		  بنانشانی format,
		  ثابت hb_subset_plan_t *plan,
		  ثابت فراغ *ادھار)
  {
    بدل (format) {
    حالة  4: hb_barrier (); أرجع u.format4.serialize (c, it);
    حالة 12: hb_barrier (); أرجع u.format12.serialize (c, it);
    حالة 14: hb_barrier (); أرجع u.format14.serialize (c, &plan->unicodes, &plan->glyphs_requested, plan->glyph_map, ادھار);
#جے_بنا_تعریف HB_NO_BEYOND_64K
    حالة 15: hb_barrier (); أرجع u.format15.serialize (c, &plan->unicodes, &plan->glyphs_requested, plan->glyph_map, ادھار);
#جے_انت
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
#جے_بنا_تعریف HB_NO_BEYOND_64K
    حالة 15: hb_barrier (); return_trace (u.format15.sanitize (c));
#جے_انت
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
#جے_بنا_تعریف HB_NO_BEYOND_64K
  CmapSubtableFormat14_15<Layout::MediumTypes>
			format15;
#جے_انت
  } u;
  عام:
  DEFINE_ناپ_UNION (2, format.v);
};


بنية Encoding_ریکارڈ
{
  صحيح cmp (ثابت Encoding_ریکارڈ &other) ثابت
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

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *ادھار) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  subtable.sanitize (c, ادھار));
  }

  سانچا<قسم_ناں Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  Encoding_ریکارڈ* copy (hb_serialize_context_t *c,
			Iterator it,
			بنانشانی format,
			ثابت فراغ *ادھار,
			ثابت hb_subset_plan_t *plan,
			/* INOUT */ بنانشانی *objidx) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خودکار snap = c->snapshot ();
    خودکار *باہرلا = c->embed (هذا);
    إذا (unlikely (!باہرلا)) return_trace (خالی_سوچک);
    باہرلا->subtable = 0;

    إذا (*objidx == 0)
    {
      CmapSubtable *cmapsubtable = c->push<CmapSubtable> ();
      بنانشانی origin_لمائی = c->لمائی ();
      cmapsubtable->serialize (c, it, format, plan, &(ادھار+subtable));
      إذا (c->لمائی () > origin_لمائی && !c->وچ_error()) *objidx = c->نکالو_pack ();
      وإلا c->نکالو_discard ();
    }

    إذا (*objidx == 0)
    {
      c->revert (snap);
      return_trace (خالی_سوچک);
    }

    c->جوڑو_link (باہرلا->subtable, *objidx);
    return_trace (باہرلا);
  }

  HBUINT16	platformID;	/* Platform ID. */
  HBUINT16	encodingID;	/* Platform-specific encoding ID. */
  Offset32_منزل<CmapSubtable>
		subtable;	/* Byte offset from beginning of table to the subtable for this encoding. */
  عام:
  DEFINE_ناپ_STATIC (8);
};

بنية cmap;

بنية SubtableUnicodesCache {

 خاص:
  hb_blob_ptr_t<cmap> ادھار_blob;
  ثابت حرف* ادھار;
  بنانشانی صحيح جدول_لمائی;
  hb_hashmap_t<بنانشانی, hb::unique_ptr<hb_سیٹ_t>> cached_unicodes;

 عام:

  ساكن SubtableUnicodesCache* create (hb_blob_ptr_t<cmap> سروت_جدول)
  {
    SubtableUnicodesCache* cache =
        (SubtableUnicodesCache*) hb_malloc (حجم(SubtableUnicodesCache));

    إذا (unlikely (!cache))
      أرجع خالی_سوچک;

    جديد (cache) SubtableUnicodesCache (سروت_جدول);
    أرجع cache;
  }

  ساكن فراغ destroy (فراغ* value) {
    إذا (!value) أرجع;

    SubtableUnicodesCache* cache = (SubtableUnicodesCache*) value;
    cache->~SubtableUnicodesCache ();
    hb_free (cache);
  }

  SubtableUnicodesCache(ثابت فراغ* cmap_ادھار,
			بنانشانی صحيح جدول_لمائی_2 = 0)
      : ادھار_blob(),
        ادھار ((ثابت حرف*) cmap_ادھار),
        جدول_لمائی (جدول_لمائی_2),
        cached_unicodes ()
  {}

  SubtableUnicodesCache(hb_blob_ptr_t<cmap> ادھار_blob_2)
      : ادھار_blob(ادھار_blob_2),
        ادھار ((ثابت حرف *) ادھار_blob.get()),
        جدول_لمائی (ادھار_blob.get_لمائی ()),
        cached_unicodes ()
  {}

  ~SubtableUnicodesCache()
  {
    ادھار_blob.destroy ();
  }

  منطقي same_ادھار(ثابت فراغ* other) ثابت
  {
    أرجع other == (ثابت فراغ*) ادھار;
  }

  ثابت hb_سیٹ_t* سیٹ_for (ثابت Encoding_ریکارڈ* ریکارڈ,
                           SubtableUnicodesCache& mutable_cache) ثابت
  {
    إذا (cached_unicodes.has ((بنانشانی) ((ثابت حرف *) ریکارڈ - ادھار)))
      أرجع cached_unicodes.get ((بنانشانی) ((ثابت حرف *) ریکارڈ - ادھار));

    أرجع mutable_cache.سیٹ_for (ریکارڈ);
  }

  ثابت hb_سیٹ_t* سیٹ_for (ثابت Encoding_ریکارڈ* ریکارڈ)
  {
    إذا (!cached_unicodes.has ((بنانشانی) ((ثابت حرف *) ریکارڈ - ادھار)))
    {
      hb_سیٹ_t *s = hb_سیٹ_create ();
      إذا (unlikely (s->وچ_error ()))
	أرجع hb_سیٹ_get_خالی_تھاواں ();

      بنانشانی صحيح subtable_انکڑے_ناپ = ریکارڈ->subtable < جدول_لمائی
					? جدول_لمائی - (بنانشانی صحيح) ریکارڈ->subtable
					: 0;
      (ادھار+ریکارڈ->subtable).collect_unicodes (s, UINT_MAX, subtable_انکڑے_ناپ);

      إذا (unlikely (!cached_unicodes.سیٹ ((بنانشانی) ((ثابت حرف *) ریکارڈ - ادھار), hb::unique_ptr<hb_سیٹ_t> {s})))
        أرجع hb_سیٹ_get_خالی_تھاواں ();

      أرجع s;
    }
    أرجع cached_unicodes.get ((بنانشانی) ((ثابت حرف *) ریکارڈ - ادھار));
  }

};

ساكن مضمن uint16_t
hb_علامت_pua_map (بنانشانی codepoint)
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
  ساكن constexpr hb_ٹیگ_t جدول_ٹیگ = HB_OT_ٹیگ_cmap;


  ساكن SubtableUnicodesCache* create_filled_cache(hb_blob_ptr_t<cmap> سروت_جدول) {
    ثابت cmap* cmap = سروت_جدول.get();
    خودکار it =
    + hb_iter (cmap->encoding_ریکارڈ)
    | hb_filter ([&](ثابت Encoding_ریکارڈ& _) {
      أرجع cmap::filter_encoding_ریکارڈاں_for_subset (cmap, _);
    })
    ;

    SubtableUnicodesCache* cache = SubtableUnicodesCache::create(سروت_جدول);

    إذا (unlikely (!cache))
      أرجع خالی_سوچک;

    لكل (ثابت Encoding_ریکارڈ& _ : it)
      cache->سیٹ_for(&_); // populate the cache for this encoding record.

    أرجع cache;
  }

  سانچا<قسم_ناں Iterator, قسم_ناں EncodingRecIter,
	   hb_requires (hb_is_iterator (EncodingRecIter))>
  منطقي serialize (hb_serialize_context_t *c,
		  Iterator it,
		  EncodingRecIter encodingrec_iter,
		  ثابت فراغ *ادھار,
		  hb_subset_plan_t *plan,
	                  منطقي drop_format_4 = خطأ,
			  بنانشانی صحيح سروت_جدول_لمائی = 0)
  {
    إذا (unlikely (!c->extend_min ((*هذا))))  أرجع خطأ;
    هذا->نسخہ = 0;

    بنانشانی format4objidx = 0, format12objidx = 0, format14objidx = 0;
#جے_بنا_تعریف HB_NO_BEYOND_64K
    بنانشانی format15objidx = 0;
#جے_انت
    خودکار snap = c->snapshot ();

    SubtableUnicodesCache local_unicodes_cache (ادھار, سروت_جدول_لمائی);
    ثابت SubtableUnicodesCache* unicodes_cache = &local_unicodes_cache;

    إذا (plan->accelerator &&
        plan->accelerator->cmap_cache &&
        plan->accelerator->cmap_cache->same_ادھار (ادھار))
      unicodes_cache = plan->accelerator->cmap_cache;

    لكل (ثابت Encoding_ریکارڈ& _ : encodingrec_iter)
    {
      إذا (c->وچ_error ())
        أرجع خطأ;

      بنانشانی format = (ادھار+_.subtable).u.format.v;
      إذا (format != 4 && format != 12 && format != 14
#جے_بنا_تعریف HB_NO_BEYOND_64K
	  && format != 15
#جے_انت
	 ) استمر;

      ثابت hb_سیٹ_t* unicodes_سیٹ = unicodes_cache->سیٹ_for (&_, local_unicodes_cache);

      إذا (!drop_format_4 && format == 4)
      {
        c->copy (_, + it | hb_filter (*unicodes_سیٹ, hb_پہلا), 4u, ادھار, plan, &format4objidx);
        إذا (c->وچ_error () && c->only_حد_توں_ودھ ())
        {
          // cmap4 overflowed, reset and retry serialization without format 4 subtables.
          c->revert (snap);
          أرجع serialize (c, it,
                            encodingrec_iter,
                            ادھار,
                            plan,
                            سچ,
			    سروت_جدول_لمائی);
        }
      }

      وإلا إذا (format == 12)
      {
        إذا (_can_drop (_,
                       *unicodes_سیٹ,
                       ادھار,
                       *unicodes_cache,
                       local_unicodes_cache,
                       + it | hb_map (hb_پہلا), encodingrec_iter))
          استمر;
        c->copy (_, + it | hb_filter (*unicodes_سیٹ, hb_پہلا), 12u, ادھار, plan, &format12objidx);
      }
      وإلا إذا (format == 14) c->copy (_, it, 14u, ادھار, plan, &format14objidx);
#جے_بنا_تعریف HB_NO_BEYOND_64K
      وإلا إذا (format == 15) c->copy (_, it, 15u, ادھار, plan, &format15objidx);
#جے_انت
    }
        بنانشانی لمائی = c->لمائی ();
        بنانشانی دستیاب = لمائی > cmap::min_ناپ ? لمائی - cmap::min_ناپ : 0;
        c->check_سونپو(هذا->encoding_ریکارڈ.len,
                        دستیاب / Encoding_ریکارڈ::static_ناپ,
                        HB_SERIALIZE_ERROR_INT_حد_توں_ودھ);

    // Fail if format 4 was dropped and there is no cmap12.
    أرجع !drop_format_4 || format12objidx;
  }

  سانچا<قسم_ناں Iterator, قسم_ناں Encoding_ریکارڈ_Iterator,
      hb_requires (hb_is_iterator (Iterator)),
      hb_requires (hb_is_iterator (Encoding_ریکارڈ_Iterator))>
  منطقي _can_drop (ثابت Encoding_ریکارڈ& cmap12,
                  ثابت hb_سیٹ_t& cmap12_unicodes,
                  ثابت فراغ* ادھار,
                  ثابت SubtableUnicodesCache& unicodes_cache,
                  SubtableUnicodesCache& local_unicodes_cache,
                  Iterator subset_unicodes,
                  Encoding_ریکارڈ_Iterator encoding_ریکارڈاں)
  {
    لكل (خودکار cp : + subset_unicodes | hb_filter (cmap12_unicodes))
    {
      إذا (cp >= 0x10000) أرجع خطأ;
    }

    بنانشانی نشانہ_platform;
    بنانشانی نشانہ_encoding;
    بنانشانی نشانہ_language = (ادھار+cmap12.subtable).get_language ();

    إذا (cmap12.platformID == 0 && cmap12.encodingID == 4)
    {
      نشانہ_platform = 0;
      نشانہ_encoding = 3;
    } وإلا إذا (cmap12.platformID == 3 && cmap12.encodingID == 10) {
      نشانہ_platform = 3;
      نشانہ_encoding = 1;
    } وإلا {
      أرجع خطأ;
    }

    لكل (ثابت خودکار& _ : encoding_ریکارڈاں)
    {
      إذا (_.platformID != نشانہ_platform
          || _.encodingID != نشانہ_encoding
          || (ادھار+_.subtable).get_language() != نشانہ_language)
        استمر;

      ثابت hb_سیٹ_t* sibling_unicodes = unicodes_cache.سیٹ_for (&_, local_unicodes_cache);

      خودکار cmap12 = + subset_unicodes | hb_filter (cmap12_unicodes);
      خودکار sibling = + subset_unicodes | hb_filter (*sibling_unicodes);
      لكل (; cmap12 && sibling; cmap12++, sibling++)
      {
        بنانشانی a = *cmap12;
        بنانشانی b = *sibling;
        إذا (a != b) أرجع خطأ;
      }

      أرجع !cmap12 && !sibling;
    }

    أرجع خطأ;
  }

  فراغ گھیرا_glyphs (ثابت hb_سیٹ_t      *unicodes,
		       hb_سیٹ_t            *glyphset) ثابت
  {
    + hb_iter (encoding_ریکارڈ)
    | hb_map (&Encoding_ریکارڈ::subtable)
    | hb_map (hb_جوڑو (هذا))
    | hb_filter (&CmapSubtable::is_variation_selector)
    | hb_apply ([=] (ثابت CmapSubtable& _) { _.گھیرا_glyphs (unicodes, glyphset); })
    ;
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  { أرجع subset (c, خطأ); }

  منطقي subset (hb_subset_context_t *c, منطقي is_dmap) ثابت
  {
    TRACE_SUBSET (هذا);

    cmap *cmap_مفرد = c->serializer->شروع_embed<cmap> ();

    خودکار encodingrec_iter =
    + hb_iter (encoding_ریکارڈ)
    | hb_filter ([&](ثابت Encoding_ریکارڈ& _) {
      أرجع cmap::filter_encoding_ریکارڈاں_for_subset (هذا, _);
    })
    ;

    إذا (unlikely (!encodingrec_iter.len ())) return_trace (خطأ);

    ثابت Encoding_ریکارڈ *unicode_bmp= خالی_سوچک, *unicode_ucs4 = خالی_سوچک, *ms_bmp = خالی_سوچک, *ms_ucs4 = خالی_سوچک;
    منطقي has_format12 = خطأ;

    لكل (ثابت Encoding_ریکارڈ& _ : encodingrec_iter)
    {
      بنانشانی format = (هذا + _.subtable).u.format.v;
      إذا (format == 12) has_format12 = سچ;

      ثابت Encoding_ریکارڈ *جدول = std::addressof (_);
      إذا      (_.platformID == 0 && _.encodingID ==  3) unicode_bmp = جدول;
      وإلا إذا (_.platformID == 0 && _.encodingID ==  4) unicode_ucs4 = جدول;
      وإلا إذا (_.platformID == 3 && _.encodingID ==  1) ms_bmp = جدول;
      وإلا إذا (_.platformID == 3 && _.encodingID == 10) ms_ucs4 = جدول;
    }

    إذا (unlikely (!is_dmap && !has_format12 && !unicode_bmp && !ms_bmp)) return_trace (خطأ);
    إذا (unlikely (!is_dmap && has_format12 && (!unicode_ucs4 && !ms_ucs4))) return_trace (خطأ);

    خودکار it =
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
					 c->سروت_blob->لمائی));
  }

  ثابت CmapSubtable *لبھو_بہترین_subtable (منطقي *علامت = خالی_سوچک,
					  منطقي *mac = خالی_سوچک,
					  منطقي *macroman = خالی_سوچک) ثابت
  {
    إذا (علامت) *علامت = خطأ;
    إذا (mac) *mac = خطأ;
    إذا (macroman) *macroman = خطأ;

    ثابت CmapSubtable *subtable;

    /* Symbol subtable.
     * Prefer symbol if available.
     * https://github.com/harfbuzz/harfbuzz/issues/1918 */
    إذا ((subtable = هذا->لبھو_subtable (3, 0)))
    {
      إذا (علامت) *علامت = سچ;
      أرجع subtable;
    }

    /* 32-bit subtables. */
    إذا ((subtable = هذا->لبھو_subtable (3, 10))) أرجع subtable;
    إذا ((subtable = هذا->لبھو_subtable (0, 6))) أرجع subtable;
    إذا ((subtable = هذا->لبھو_subtable (0, 4))) أرجع subtable;

    /* 16-bit subtables. */
    إذا ((subtable = هذا->لبھو_subtable (3, 1))) أرجع subtable;
    إذا ((subtable = هذا->لبھو_subtable (0, 3))) أرجع subtable;
    إذا ((subtable = هذا->لبھو_subtable (0, 2))) أرجع subtable;
    إذا ((subtable = هذا->لبھو_subtable (0, 1))) أرجع subtable;
    إذا ((subtable = هذا->لبھو_subtable (0, 0))) أرجع subtable;

    /* MacRoman subtable. */
    إذا ((subtable = هذا->لبھو_subtable (1, 0)))
    {
      إذا (mac) *mac = سچ;
      إذا (macroman) *macroman = سچ;
      أرجع subtable;
    }
    /* Any other Mac subtable; we just map ASCII for these. */
    إذا ((subtable = هذا->لبھو_subtable (1, 0xFFFF)))
    {
      إذا (mac) *mac = سچ;
      أرجع subtable;
    }

    /* Meh. */
    أرجع &Null (CmapSubtable);
  }

  بنية accelerator_t
  {
    ورتوں cache_t = hb_cache_t<21, 19>;
    static_assert (حجم (cache_t) == 1024, "");

    accelerator_t (hb_face_t *face, hb_ٹیگ_t جدول_ٹیگ_2 = cmap::جدول_ٹیگ)
    {
      هذا->جدول = hb_sanitize_context_t ().reference_جدول<cmap> (face, جدول_ٹیگ_2);
      إذا (!هذا->جدول.get_لمائی ())
	أرجع;

      منطقي علامت, mac, macroman;
      هذا->subtable = جدول->لبھو_بہترین_subtable (&علامت, &mac, &macroman);
      هذا->subtable_uvs = &Null (CmapSubtable);
      {
	ثابت CmapSubtable *st = جدول->لبھو_variation_selector_subtable ();
	إذا (st)
	  subtable_uvs = st;
      }

#جے_بنا_تعریف HB_NO_OT_FONT_CMAP_CACHE
      cache = (cache_t *) hb_malloc (حجم (cache_t));
      إذا (cache)
	جديد (cache) cache_t ();
      وإلا
        أرجع; // Such that get_glyph_funcZ remains null.
#جے_انت

      هذا->get_glyph_انکڑے = subtable;
#جے_بنا_تعریف HB_NO_CMAP_LEGACY_SUBTABLES
      منطقي is_format4 = subtable->u.format.v == 4;
      خودکار سیٹ_format4_getter = [هذا] (منطقي (*func) (ثابت فراغ *,
						      hb_codepoint_t,
						      hb_codepoint_t *))
      {
	هذا->format4_accel.init (&هذا->subtable->u.format4,
				  get_subtable_انکڑے_ناپ (هذا->subtable));
	هذا->get_glyph_انکڑے = &هذا->format4_accel;
	هذا->get_glyph_funcZ = func;
      };
      إذا (unlikely (علامت))
      {
	بدل ((بنانشانی) face->جدول.OS2->get_font_ورقہ ()) {
	حالة OS2::font_ورقہ_t::FONT_ورقہ_NONE:
	  إذا (is_format4)
	    سیٹ_format4_getter (get_glyph_from_علامت<CmapSubtableFormat4::accelerator_t, hb_علامت_pua_map>);
	  وإلا
	    هذا->get_glyph_funcZ = get_glyph_from_علامت<CmapSubtable, hb_علامت_pua_map>;
	  اكسر;
#جے_بنا_تعریف HB_NO_OT_SHAPER_ARABIC_FALLBACK
	حالة OS2::font_ورقہ_t::FONT_ورقہ_SIMP_ARABIC:
	  إذا (is_format4)
	    سیٹ_format4_getter (get_glyph_from_علامت<CmapSubtableFormat4::accelerator_t, _hb_arabic_pua_simp_map>);
	  وإلا
	    هذا->get_glyph_funcZ = get_glyph_from_علامت<CmapSubtable, _hb_arabic_pua_simp_map>;
	  اكسر;
	حالة OS2::font_ورقہ_t::FONT_ورقہ_TRAD_ARABIC:
	  إذا (is_format4)
	    سیٹ_format4_getter (get_glyph_from_علامت<CmapSubtableFormat4::accelerator_t, _hb_arabic_pua_trad_map>);
	  وإلا
	    هذا->get_glyph_funcZ = get_glyph_from_علامت<CmapSubtable, _hb_arabic_pua_trad_map>;
	  اكسر;
#جے_انت
	افتراضي:
	  إذا (is_format4)
	    سیٹ_format4_getter (get_glyph_from<CmapSubtableFormat4::accelerator_t>);
	  وإلا
	    هذا->get_glyph_funcZ = get_glyph_from<CmapSubtable>;
	  اكسر;
	}
      }
      وإلا إذا (unlikely (macroman))
      {
	إذا (is_format4)
	  سیٹ_format4_getter (get_glyph_from_macroman<CmapSubtableFormat4::accelerator_t>);
	وإلا
	  هذا->get_glyph_funcZ = get_glyph_from_macroman<CmapSubtable>;
      }
      وإلا إذا (unlikely (mac))
      {
	إذا (is_format4)
	  سیٹ_format4_getter (get_glyph_from_ascii<CmapSubtableFormat4::accelerator_t>);
	وإلا
	  هذا->get_glyph_funcZ = get_glyph_from_ascii<CmapSubtable>;
      }
      وإلا
#جے_انت
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
				      get_subtable_انکڑے_ناپ (subtable));
	    هذا->get_glyph_انکڑے = &هذا->format4_accel;
	    هذا->get_glyph_funcZ = هذا->format4_accel.get_glyph_func;
	    اكسر;
	  }
	}
      }
    }
    ~accelerator_t ()
    {
#جے_بنا_تعریف HB_NO_OT_FONT_CMAP_CACHE
      hb_free (cache);
#جے_انت
      جدول.destroy ();
    }

    مضمن منطقي _cached_get (hb_codepoint_t unicode,
			     hb_codepoint_t *glyph) ثابت
    {
#جے_بنا_تعریف HB_NO_OT_FONT_CMAP_CACHE
      // cache is always non-null if we have a get_glyph_funcZ
      بنانشانی v;
      إذا (cache->get (unicode, &v))
      {
        *glyph = v;
	أرجع سچ;
      }
#جے_انت
      منطقي ret  = هذا->get_glyph_funcZ (هذا->get_glyph_انکڑے, unicode, glyph);

#جے_بنا_تعریف HB_NO_OT_FONT_CMAP_CACHE
      إذا (ret)
        cache->سیٹ (unicode, *glyph);
#جے_انت

      أرجع ret;
    }

    منطقي get_nominal_glyph (hb_codepoint_t  unicode,
			    hb_codepoint_t *glyph) ثابت
    {
      إذا (unlikely (!هذا->get_glyph_funcZ)) أرجع خطأ;
      أرجع _cached_get (unicode, glyph);
    }

    بنانشانی صحيح get_nominal_glyphs (بنانشانی صحيح گنتی,
				     ثابت hb_codepoint_t *پہلا_unicode,
				     بنانشانی صحيح unicode_stride,
				     hb_codepoint_t *پہلا_glyph,
				     بنانشانی صحيح glyph_stride) ثابت
    {
      إذا (unlikely (!هذا->get_glyph_funcZ)) أرجع 0;

      بنانشانی صحيح پورا;
      لكل (پورا = 0;
	   پورا < گنتی && _cached_get (*پہلا_unicode, پہلا_glyph);
	   پورا++)
      {
	پہلا_unicode = &Struct_At_وتھ_Unaligned<hb_codepoint_t> (پہلا_unicode, unicode_stride);
	پہلا_glyph = &Struct_At_وتھ_Unaligned<hb_codepoint_t> (پہلا_glyph, glyph_stride);
      }
      أرجع پورا;
    }

    منطقي get_variation_glyph (hb_codepoint_t  unicode,
			      hb_codepoint_t  variation_selector,
			      hb_codepoint_t *glyph) ثابت
    {
      بدل (get_glyph_variant (unicode, variation_selector, glyph))
      {
	حالة GLYPH_VARIANT_NOT_لبھیا:	أرجع خطأ;
	حالة GLYPH_VARIANT_لبھیا:	أرجع سچ;
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

    منطقي has_انکڑے () ثابت { أرجع جدول.get_لمائی (); }

    فراغ collect_unicodes (hb_سیٹ_t *باہرلا, بنانشانی صحيح عدد_glyphs) ثابت
    { subtable->collect_unicodes (باہرلا, عدد_glyphs, get_subtable_انکڑے_ناپ (subtable)); }
    فراغ collect_mapping (hb_سیٹ_t *unicodes, hb_map_t *mapping,
			  بنانشانی عدد_glyphs = UINT_MAX) ثابت
    { subtable->collect_mapping (unicodes, mapping, عدد_glyphs, get_subtable_انکڑے_ناپ (subtable)); }
    فراغ collect_variation_selectors (hb_سیٹ_t *باہرلا) ثابت
    { subtable_uvs->collect_variation_selectors (باہرلا); }
    فراغ collect_variation_unicodes (hb_codepoint_t variation_selector,
				     hb_سیٹ_t *باہرلا) ثابت
    { subtable_uvs->collect_variation_unicodes (variation_selector, باہرلا); }

    محمي:
    تعريفنوع منطقي (*hb_cmap_get_glyph_func_t) (ثابت فراغ *obj,
					      hb_codepoint_t codepoint,
					      hb_codepoint_t *glyph);
    تعريفنوع uint16_t (*hb_pua_remap_func_t) (بنانشانی);

    سانچا <قسم_ناں Type>
    HB_اندرونی ساكن منطقي get_glyph_from (ثابت فراغ *obj,
					    hb_codepoint_t codepoint,
					    hb_codepoint_t *glyph)
    {
      ثابت Type *typed_obj = (ثابت Type *) obj;
      أرجع typed_obj->get_glyph (codepoint, glyph);
    }

    سانچا <قسم_ناں Type, hb_pua_remap_func_t remap>
    HB_اندرونی ساكن منطقي get_glyph_from_علامت (ثابت فراغ *obj,
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

    سانچا <قسم_ناں Type>
    HB_اندرونی ساكن منطقي get_glyph_from_ascii (ثابت فراغ *obj,
						  hb_codepoint_t codepoint,
						  hb_codepoint_t *glyph)
    {
      ثابت Type *typed_obj = (ثابت Type *) obj;
      أرجع codepoint < 0x80 && typed_obj->get_glyph (codepoint, glyph);
    }

    سانچا <قسم_ناں Type>
    HB_اندرونی ساكن منطقي get_glyph_from_macroman (ثابت فراغ *obj,
						     hb_codepoint_t codepoint,
						     hb_codepoint_t *glyph)
    {
      إذا (get_glyph_from_ascii<Type> (obj, codepoint, glyph))
	أرجع سچ;

      ثابت Type *typed_obj = (ثابت Type *) obj;
      بنانشانی c = unicode_منزل_macroman (codepoint);
      أرجع c && typed_obj->get_glyph (c, glyph);
    }

    بنانشانی صحيح get_subtable_انکڑے_ناپ (ثابت CmapSubtable *subtable) ثابت
    {
      بنانشانی صحيح جدول_لمائی = هذا->جدول.get_لمائی ();
      uintptr_t جدول_شروع = (uintptr_t) (ثابت فراغ *) هذا->جدول.get ();
      uintptr_t subtable_addr = (uintptr_t) (ثابت فراغ *) subtable;
      إذا (unlikely (subtable_addr < جدول_شروع))
	أرجع 0;

      uintptr_t subtable_وتھ = subtable_addr - جدول_شروع;
      إذا (unlikely (subtable_وتھ >= جدول_لمائی))
	أرجع 0;

      أرجع جدول_لمائی - (بنانشانی صحيح) subtable_وتھ;
    }

    خاص:
    hb_nonnull_ptr_t<ثابت CmapSubtable> subtable;
    hb_nonnull_ptr_t<ثابت CmapSubtable> subtable_uvs;

    hb_cmap_get_glyph_func_t get_glyph_funcZ = خالی_سوچک;
    ثابت فراغ *get_glyph_انکڑے = خالی_سوچک;

    CmapSubtableFormat4::accelerator_t format4_accel;

#جے_بنا_تعریف HB_NO_OT_FONT_CMAP_CACHE
    cache_t *cache = خالی_سوچک;
#جے_انت

    عام:
    hb_blob_ptr_t<cmap> جدول;
  };

  محمي:

  ثابت CmapSubtable *لبھو_subtable (بنانشانی صحيح platform_id,
				     بنانشانی صحيح encoding_id) ثابت
  {
    Encoding_ریکارڈ key;
    key.platformID = platform_id;
    key.encodingID = encoding_id;

    ثابت Encoding_ریکارڈ &نتیجہ = encoding_ریکارڈ.bsearch (key);
    إذا (!نتیجہ.subtable)
      أرجع خالی_سوچک;

    أرجع &(هذا+نتیجہ.subtable);
  }

  ثابت CmapSubtable *لبھو_variation_selector_subtable () ثابت
  {
    ثابت CmapSubtable *format14 = خالی_سوچک;
    لكل (ثابت Encoding_ریکارڈ& ریکارڈ : encoding_ریکارڈ)
    {
      إذا (ریکارڈ.platformID != 0 || ریکارڈ.encodingID != 5)
	استمر;

      ثابت CmapSubtable *subtable = &(هذا+ریکارڈ.subtable);
#جے_بنا_تعریف HB_NO_BEYOND_64K
      إذا (subtable->u.format.v == 15)
	أرجع subtable;
#جے_انت
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
		  likely (نسخہ == 0) &&
		  encoding_ریکارڈ.sanitize (c, هذا));
  }

 خاص:

  ساكن منطقي filter_encoding_ریکارڈاں_for_subset(ثابت cmap* cmap,
                                                 ثابت Encoding_ریکارڈ& _)
  {
    أرجع
        (_.platformID == 0 && _.encodingID == 3) ||
        (_.platformID == 0 && _.encodingID == 4) ||
        (_.platformID == 3 && _.encodingID == 1) ||
        (_.platformID == 3 && _.encodingID == 10) ||
        (cmap + _.subtable).is_variation_selector ();
  }

  محمي:
  HBUINT16	نسخہ;	/* Table version number (0). */
  Sorted_Array16_دا<Encoding_ریکارڈ>
		encoding_ریکارڈ;	/* Encoding tables. */
  عام:
  DEFINE_ناپ_ARRAY (4, encoding_ریکارڈ);
};

بنية DMAP : cmap
{
  ساكن constexpr hb_ٹیگ_t جدول_ٹیگ = HB_OT_ٹیگ_DMAP;

  منطقي subset (hb_subset_context_t *c) ثابت
  { أرجع cmap::subset (c, سچ); }
};

بنية cmap_accelerator_t
{
  cmap_accelerator_t (hb_face_t *face) :
    dmap (face, HB_OT_ٹیگ_DMAP),
    ادھار (face)
  {}

  منطقي get_nominal_glyph (hb_codepoint_t  unicode,
			  hb_codepoint_t *glyph) ثابت
  {
    أرجع dmap.get_nominal_glyph (unicode, glyph) ||
	   ادھار.get_nominal_glyph (unicode, glyph);
  }

  بنانشانی صحيح get_nominal_glyphs (بنانشانی صحيح گنتی,
				   ثابت hb_codepoint_t *پہلا_unicode,
				   بنانشانی صحيح unicode_stride,
				   hb_codepoint_t *پہلا_glyph,
				   بنانشانی صحيح glyph_stride) ثابت
  {
    إذا (likely (!dmap.has_انکڑے ()))
      أرجع ادھار.get_nominal_glyphs (گنتی,
				      پہلا_unicode, unicode_stride,
				      پہلا_glyph, glyph_stride);

    بنانشانی صحيح پورا;
    لكل (پورا = 0;
	 پورا < گنتی && get_nominal_glyph (*پہلا_unicode, پہلا_glyph);
	 پورا++)
    {
      پہلا_unicode = &Struct_At_وتھ_Unaligned<hb_codepoint_t> (پہلا_unicode, unicode_stride);
      پہلا_glyph = &Struct_At_وتھ_Unaligned<hb_codepoint_t> (پہلا_glyph, glyph_stride);
    }
    أرجع پورا;
  }

  منطقي get_variation_glyph (hb_codepoint_t  unicode,
			    hb_codepoint_t  variation_selector,
			    hb_codepoint_t *glyph) ثابت
  {
    glyph_variant_t نتیجہ = dmap.get_glyph_variant (unicode,
						     variation_selector,
						     glyph);
    إذا (نتیجہ == GLYPH_VARIANT_NOT_لبھیا)
      نتیجہ = ادھار.get_glyph_variant (unicode, variation_selector, glyph);

    بدل (نتیجہ)
    {
      حالة GLYPH_VARIANT_NOT_لبھیا: أرجع خطأ;
      حالة GLYPH_VARIANT_لبھیا: أرجع سچ;
      حالة GLYPH_VARIANT_USE_DEFAULT: اكسر;
    }

    أرجع get_nominal_glyph (unicode, glyph);
  }

  فراغ collect_unicodes (hb_سیٹ_t *باہرلا, بنانشانی صحيح عدد_glyphs) ثابت
  {
    ادھار.collect_unicodes (باہرلا, عدد_glyphs);
    dmap.collect_unicodes (باہرلا, عدد_glyphs);
  }

  فراغ collect_mapping (hb_سیٹ_t *unicodes, hb_map_t *mapping,
			بنانشانی عدد_glyphs = UINT_MAX) ثابت
  {
    ادھار.collect_mapping (unicodes, mapping, عدد_glyphs);
    dmap.collect_mapping (unicodes, mapping, عدد_glyphs);
  }

  فراغ collect_variation_selectors (hb_سیٹ_t *باہرلا) ثابت
  {
    ادھار.collect_variation_selectors (باہرلا);
    dmap.collect_variation_selectors (باہرلا);
  }

  فراغ collect_variation_unicodes (hb_codepoint_t variation_selector,
				   hb_سیٹ_t *باہرلا) ثابت
  {
    ادھار.collect_variation_unicodes (variation_selector, باہرلا);
    dmap.collect_variation_unicodes (variation_selector, باہرلا);
  }

  فراغ گھیرا_glyphs (ثابت hb_سیٹ_t *unicodes,
		       hb_سیٹ_t       *glyphset) ثابت
  {
    إذا (likely (!dmap.has_انکڑے ()))
    {
      ادھار.جدول->گھیرا_glyphs (unicodes, glyphset);
      أرجع;
    }

    hb_سیٹ_t variation_selectors;
    collect_variation_selectors (&variation_selectors);
    لكل (hb_codepoint_t variation_selector : variation_selectors)
    {
      إذا (!unicodes->has (variation_selector))
	استمر;

      hb_سیٹ_t variation_unicodes;
      collect_variation_unicodes (variation_selector, &variation_unicodes);
      لكل (hb_codepoint_t unicode : variation_unicodes)
      {
	إذا (!unicodes->has (unicode))
	  استمر;

	hb_codepoint_t glyph;
	إذا (get_variation_glyph (unicode, variation_selector, &glyph))
	  glyphset->جوڑو (glyph);
      }
    }
  }

  cmap::accelerator_t dmap;
  cmap::accelerator_t ادھار;
};

} /* namespace OT */


#جے_انت /* HB_OT_CMAP_TABLE_HH */
