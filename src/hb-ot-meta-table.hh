/*
 * Copyright © 2019  Ebrahim Byagowi
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
 */

#اگر_غیر_تعریف_شدہ HB_OT_META_جدول_HH
#تعریف_کرو HB_OT_META_جدول_HH

#شامل "hb-open-type.hh"

/*
 * meta -- Metadata Table
 * https://docs.microsoft.com/en-us/typography/opentype/spec/meta
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6meta.html
 */
#تعریف_کرو HB_OT_لیبل_meta HB_لیبل ('m','e','t','a')


نامخلا OT {


ساخت ڈیٹا_Map
{
  صحیح cmp (hb_لیبل_t a) ثابت { واپس لیبل_2.cmp (a); }

  hb_لیبل_t get_لیبل () ثابت { واپس لیبل_2; }

  hb_blob_t *reference_اندراج (hb_blob_t *meta_blob) ثابت
  { واپس hb_blob_create_sub_blob (meta_blob, ڈیٹا_Z, ڈیٹا_لمبائی_2); }

  منطقی sanitize (hb_sanitize_context_t *c, ثابت خالی *بنیاد) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (likely (c->check_struct (یہ) &&
			  hb_barrier () &&
			  ڈیٹا_Z.sanitize (c, بنیاد, ڈیٹا_لمبائی_2)));
  }

  محفوظ:
  لیبل		لیبل_2;		/* A tag indicating the type of metadata. */
  NNOffset32_مقصد<Unsized_Array_کا<HBUINT8>>
		ڈیٹا_Z;		/* Offset in bytes from the beginning of the
				 * metadata table to the data for this tag. */
  HBUINT32	ڈیٹا_لمبائی_2;	/* Length of the data. The data is not required to
				 * be padded to any byte boundary. */
  عوامی:
  DEFINE_سائز_STATIC (12);
};

ساخت meta
{
  ساکن constexpr hb_لیبل_t جدول_لیبل = HB_OT_لیبل_meta;

  ساخت accelerator_t
  {
    accelerator_t (hb_face_t *face)
    { جدول = hb_sanitize_context_t ().reference_جدول<meta> (face); }
    ~accelerator_t () { جدول.destroy (); }

    hb_blob_t *reference_اندراج (hb_لیبل_t لیبل_2) ثابت
    { واپس جدول->ڈیٹا_Maps.lsearch (لیبل_2).reference_اندراج (جدول.get_blob ()); }

    غیرعلامتی صحیح get_entries (غیرعلامتی صحیح      آغاز_انحراف,
			      غیرعلامتی صحیح     *گنتی,
			      hb_ot_meta_لیبل_t *entries) ثابت
    {
      اگر (گنتی && entries)
      {
	+ جدول->ڈیٹا_Maps.as_array ().sub_array (آغاز_انحراف, گنتی)
	| hb_map (&ڈیٹا_Map::get_لیبل)
	| hb_map ([](hb_لیبل_t لیبل_2) { واپس (hb_ot_meta_لیبل_t) لیبل_2; })
	| hb_sink (hb_array (entries, *گنتی))
	;
      }
      واپس جدول->ڈیٹا_Maps.len;
    }

    نجی:
    hb_blob_ptr_t<meta> جدول;
  };

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (likely (c->check_struct (یہ) &&
			  hb_barrier () &&
			  نسخہ == 1 &&
			  ڈیٹا_Maps.sanitize (c, یہ)));
  }

  محفوظ:
  HBUINT32	نسخہ;	/* Version number of the metadata table — set to 1. */
  HBUINT32	flags;		/* Flags — currently unused; set to 0. */
  HBUINT32	ڈیٹا_انحراف;
				/* Per Apple specification:
				 * Offset from the beginning of the table to the data.
				 * Per OT specification:
				 * Reserved. Not used; should be set to 0. */
  Array32_کا<ڈیٹا_Map>
		ڈیٹا_Maps;/* Array of data map records. */
  عوامی:
  DEFINE_سائز_ARRAY (16, ڈیٹا_Maps);
};

ساخت meta_accelerator_t : meta::accelerator_t {
  meta_accelerator_t (hb_face_t *face) : meta::accelerator_t (face) {}
};

} /* namespace OT */


#اگر_ختم /* HB_OT_META_TABLE_HH */
