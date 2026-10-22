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

#جے_بنا_تعریف HB_OT_META_جدول_HH
#تعریف_کرو HB_OT_META_جدول_HH

#ضمن "hb-open-type.hh"

/*
 * meta -- Metadata Table
 * https://docs.microsoft.com/en-us/typography/opentype/spec/meta
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6meta.html
 */
#تعریف_کرو HB_OT_ٹیگ_meta HB_ٹیگ ('m','e','t','a')


نطاق OT {


بنية انکڑے_Map
{
  صحيح cmp (hb_ٹیگ_t a) ثابت { أرجع ٹیگ_2.cmp (a); }

  hb_ٹیگ_t get_ٹیگ () ثابت { أرجع ٹیگ_2; }

  hb_blob_t *reference_اندراج (hb_blob_t *meta_blob) ثابت
  { أرجع hb_blob_create_sub_blob (meta_blob, انکڑے_Z, انکڑے_لمائی_2); }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *ادھار) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  hb_barrier () &&
			  انکڑے_Z.sanitize (c, ادھار, انکڑے_لمائی_2)));
  }

  محمي:
  ٹیگ		ٹیگ_2;		/* A tag indicating the type of metadata. */
  NNOffset32_منزل<Unsized_Array_دا<HBUINT8>>
		انکڑے_Z;		/* Offset in bytes from the beginning of the
				 * metadata table to the data for this tag. */
  HBUINT32	انکڑے_لمائی_2;	/* Length of the data. The data is not required to
				 * be padded to any byte boundary. */
  عام:
  DEFINE_ناپ_STATIC (12);
};

بنية meta
{
  ساكن constexpr hb_ٹیگ_t جدول_ٹیگ = HB_OT_ٹیگ_meta;

  بنية accelerator_t
  {
    accelerator_t (hb_face_t *face)
    { جدول = hb_sanitize_context_t ().reference_جدول<meta> (face); }
    ~accelerator_t () { جدول.destroy (); }

    hb_blob_t *reference_اندراج (hb_ٹیگ_t ٹیگ_2) ثابت
    { أرجع جدول->انکڑے_Maps.lsearch (ٹیگ_2).reference_اندراج (جدول.get_blob ()); }

    بنانشانی صحيح get_entries (بنانشانی صحيح      شروع_وتھ,
			      بنانشانی صحيح     *گنتی,
			      hb_ot_meta_ٹیگ_t *entries) ثابت
    {
      إذا (گنتی && entries)
      {
	+ جدول->انکڑے_Maps.as_array ().sub_array (شروع_وتھ, گنتی)
	| hb_map (&انکڑے_Map::get_ٹیگ)
	| hb_map ([](hb_ٹیگ_t ٹیگ_2) { أرجع (hb_ot_meta_ٹیگ_t) ٹیگ_2; })
	| hb_sink (hb_array (entries, *گنتی))
	;
      }
      أرجع جدول->انکڑے_Maps.len;
    }

    خاص:
    hb_blob_ptr_t<meta> جدول;
  };

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  hb_barrier () &&
			  نسخہ == 1 &&
			  انکڑے_Maps.sanitize (c, هذا)));
  }

  محمي:
  HBUINT32	نسخہ;	/* Version number of the metadata table — set to 1. */
  HBUINT32	flags;		/* Flags — currently unused; set to 0. */
  HBUINT32	انکڑے_وتھ;
				/* Per Apple specification:
				 * Offset from the beginning of the table to the data.
				 * Per OT specification:
				 * Reserved. Not used; should be set to 0. */
  Array32_دا<انکڑے_Map>
		انکڑے_Maps;/* Array of data map records. */
  عام:
  DEFINE_ناپ_ARRAY (16, انکڑے_Maps);
};

بنية meta_accelerator_t : meta::accelerator_t {
  meta_accelerator_t (hb_face_t *face) : meta::accelerator_t (face) {}
};

} /* namespace OT */


#جے_انت /* HB_OT_META_TABLE_HH */
