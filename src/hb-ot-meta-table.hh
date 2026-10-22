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

#اگر_تعریف_نشده HB_OT_META_جدول_HH
#تعریف_کن HB_OT_META_جدول_HH

#ضمن "hb-open-type.hh"

/*
 * meta -- Metadata Table
 * https://docs.microsoft.com/en-us/typography/opentype/spec/meta
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6meta.html
 */
#تعریف_کن HB_OT_برچسب_meta HB_برچسب ('m','e','t','a')


نطاق OT {


بنية داده_Map
{
  صحيح cmp (hb_برچسب_t a) ثابت { أرجع برچسب_2.cmp (a); }

  hb_برچسب_t get_برچسب () ثابت { أرجع برچسب_2; }

  hb_blob_t *reference_مدخل (hb_blob_t *meta_blob) ثابت
  { أرجع hb_blob_create_sub_blob (meta_blob, داده_Z, داده_درازی_2); }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *پایه) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  hb_barrier () &&
			  داده_Z.sanitize (c, پایه, داده_درازی_2)));
  }

  محمي:
  برچسب		برچسب_2;		/* A tag indicating the type of metadata. */
  NNOffset32_مقصد<Unsized_Array_از<HBUINT8>>
		داده_Z;		/* Offset in bytes from the beginning of the
				 * metadata table to the data for this tag. */
  HBUINT32	داده_درازی_2;	/* Length of the data. The data is not required to
				 * be padded to any byte boundary. */
  عام:
  DEFINE_اندازه_STATIC (12);
};

بنية meta
{
  ساكن constexpr hb_برچسب_t جدول_برچسب = HB_OT_برچسب_meta;

  بنية accelerator_t
  {
    accelerator_t (hb_face_t *face)
    { جدول = hb_sanitize_context_t ().reference_جدول<meta> (face); }
    ~accelerator_t () { جدول.destroy (); }

    hb_blob_t *reference_مدخل (hb_برچسب_t برچسب_2) ثابت
    { أرجع جدول->داده_Maps.lsearch (برچسب_2).reference_مدخل (جدول.get_blob ()); }

    بیعلامت صحيح get_entries (بیعلامت صحيح      شروع_جابه_جایی,
			      بیعلامت صحيح     *شمار,
			      hb_ot_meta_برچسب_t *entries) ثابت
    {
      إذا (شمار && entries)
      {
	+ جدول->داده_Maps.as_array ().sub_array (شروع_جابه_جایی, شمار)
	| hb_map (&داده_Map::get_برچسب)
	| hb_map ([](hb_برچسب_t برچسب_2) { أرجع (hb_ot_meta_برچسب_t) برچسب_2; })
	| hb_sink (hb_array (entries, *شمار))
	;
      }
      أرجع جدول->داده_Maps.len;
    }

    خاص:
    hb_blob_ptr_t<meta> جدول;
  };

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  hb_barrier () &&
			  نسخه == 1 &&
			  داده_Maps.sanitize (c, هذا)));
  }

  محمي:
  HBUINT32	نسخه;	/* Version number of the metadata table — set to 1. */
  HBUINT32	flags;		/* Flags — currently unused; set to 0. */
  HBUINT32	داده_جابه_جایی;
				/* Per Apple specification:
				 * Offset from the beginning of the table to the data.
				 * Per OT specification:
				 * Reserved. Not used; should be set to 0. */
  Array32_از<داده_Map>
		داده_Maps;/* Array of data map records. */
  عام:
  DEFINE_اندازه_ARRAY (16, داده_Maps);
};

بنية meta_accelerator_t : meta::accelerator_t {
  meta_accelerator_t (hb_face_t *face) : meta::accelerator_t (face) {}
};

} /* namespace OT */


#پایان_اگر /* HB_OT_META_TABLE_HH */
