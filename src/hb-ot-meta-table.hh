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

#که_نه_تعریف_شوی HB_OT_META_جدول_HH
#تعریف_کړه HB_OT_META_جدول_HH

#ضمن "hb-open-type.hh"

/*
 * meta -- Metadata Table
 * https://docs.microsoft.com/en-us/typography/opentype/spec/meta
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6meta.html
 */
#تعریف_کړه HB_OT_نښان_meta HB_نښان ('m','e','t','a')


نطاق OT {


بنية معلومات_Map
{
  صحيح cmp (hb_نښان_t a) ثابت { أرجع نښان_2.cmp (a); }

  hb_نښان_t get_نښان () ثابت { أرجع نښان_2; }

  hb_blob_t *reference_ثبت (hb_blob_t *meta_blob) ثابت
  { أرجع hb_blob_create_sub_blob (meta_blob, معلومات_Z, معلومات_اوږدتیا_2); }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *بنسټ) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  hb_barrier () &&
			  معلومات_Z.sanitize (c, بنسټ, معلومات_اوږدتیا_2)));
  }

  محمي:
  نښان		نښان_2;		/* A tag indicating the type of metadata. */
  NNOffset32_منزل<Unsized_Array_د<HBUINT8>>
		معلومات_Z;		/* Offset in bytes from the beginning of the
				 * metadata table to the data for this tag. */
  HBUINT32	معلومات_اوږدتیا_2;	/* Length of the data. The data is not required to
				 * be padded to any byte boundary. */
  عام:
  DEFINE_اندازه_STATIC (12);
};

بنية meta
{
  ساكن constexpr hb_نښان_t جدول_نښان = HB_OT_نښان_meta;

  بنية accelerator_t
  {
    accelerator_t (hb_face_t *face)
    { جدول = hb_sanitize_context_t ().reference_جدول<meta> (face); }
    ~accelerator_t () { جدول.destroy (); }

    hb_blob_t *reference_ثبت (hb_نښان_t نښان_2) ثابت
    { أرجع جدول->معلومات_Maps.lsearch (نښان_2).reference_ثبت (جدول.get_blob ()); }

    بینښه صحيح get_entries (بینښه صحيح      پیل_دننه_واټن,
			      بینښه صحيح     *شمېر,
			      hb_ot_meta_نښان_t *entries) ثابت
    {
      إذا (شمېر && entries)
      {
	+ جدول->معلومات_Maps.as_array ().sub_array (پیل_دننه_واټن, شمېر)
	| hb_map (&معلومات_Map::get_نښان)
	| hb_map ([](hb_نښان_t نښان_2) { أرجع (hb_ot_meta_نښان_t) نښان_2; })
	| hb_sink (hb_array (entries, *شمېر))
	;
      }
      أرجع جدول->معلومات_Maps.len;
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
			  معلومات_Maps.sanitize (c, هذا)));
  }

  محمي:
  HBUINT32	نسخه;	/* Version number of the metadata table — set to 1. */
  HBUINT32	flags;		/* Flags — currently unused; set to 0. */
  HBUINT32	معلومات_دننه_واټن;
				/* Per Apple specification:
				 * Offset from the beginning of the table to the data.
				 * Per OT specification:
				 * Reserved. Not used; should be set to 0. */
  Array32_د<معلومات_Map>
		معلومات_Maps;/* Array of data map records. */
  عام:
  DEFINE_اندازه_ARRAY (16, معلومات_Maps);
};

بنية meta_accelerator_t : meta::accelerator_t {
  meta_accelerator_t (hb_face_t *face) : meta::accelerator_t (face) {}
};

} /* namespace OT */


#د_که_پای /* HB_OT_META_TABLE_HH */
