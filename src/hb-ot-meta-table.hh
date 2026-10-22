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

#جيڪڏهن_وصف_نه_ٿيل HB_OT_META_جدول_HH
#وصف_ڪريو HB_OT_META_جدول_HH

#ضمن "hb-open-type.hh"

/*
 * meta -- Metadata Table
 * https://docs.microsoft.com/en-us/typography/opentype/spec/meta
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6meta.html
 */
#وصف_ڪريو HB_OT_ٽيگ_meta HB_ٽيگ ('m','e','t','a')


نطاق OT {


بنية معلومات_Map
{
  صحيح cmp (hb_ٽيگ_t a) ثابت { أرجع ٽيگ_2.cmp (a); }

  hb_ٽيگ_t get_ٽيگ () ثابت { أرجع ٽيگ_2; }

  hb_blob_t *reference_داخلا (hb_blob_t *meta_blob) ثابت
  { أرجع hb_blob_create_sub_blob (meta_blob, معلومات_Z, معلومات_ڊيگهائي_2); }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *بنياد) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  hb_barrier () &&
			  معلومات_Z.sanitize (c, بنياد, معلومات_ڊيگهائي_2)));
  }

  محمي:
  ٽيگ		ٽيگ_2;		/* A tag indicating the type of metadata. */
  NNOffset32_منزل<Unsized_Array_جو<HBUINT8>>
		معلومات_Z;		/* Offset in bytes from the beginning of the
				 * metadata table to the data for this tag. */
  HBUINT32	معلومات_ڊيگهائي_2;	/* Length of the data. The data is not required to
				 * be padded to any byte boundary. */
  عام:
  DEFINE_سائيز_STATIC (12);
};

بنية meta
{
  ساكن constexpr hb_ٽيگ_t جدول_ٽيگ = HB_OT_ٽيگ_meta;

  بنية accelerator_t
  {
    accelerator_t (hb_face_t *face)
    { جدول = hb_sanitize_context_t ().reference_جدول<meta> (face); }
    ~accelerator_t () { جدول.destroy (); }

    hb_blob_t *reference_داخلا (hb_ٽيگ_t ٽيگ_2) ثابت
    { أرجع جدول->معلومات_Maps.lsearch (ٽيگ_2).reference_داخلا (جدول.get_blob ()); }

    بنانشان صحيح get_entries (بنانشان صحيح      شروعات_وٿي,
			      بنانشان صحيح     *ڳڻپ,
			      hb_ot_meta_ٽيگ_t *entries) ثابت
    {
      إذا (ڳڻپ && entries)
      {
	+ جدول->معلومات_Maps.as_array ().sub_array (شروعات_وٿي, ڳڻپ)
	| hb_map (&معلومات_Map::get_ٽيگ)
	| hb_map ([](hb_ٽيگ_t ٽيگ_2) { أرجع (hb_ot_meta_ٽيگ_t) ٽيگ_2; })
	| hb_sink (hb_array (entries, *ڳڻپ))
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
			  نسخو == 1 &&
			  معلومات_Maps.sanitize (c, هذا)));
  }

  محمي:
  HBUINT32	نسخو;	/* Version number of the metadata table — set to 1. */
  HBUINT32	flags;		/* Flags — currently unused; set to 0. */
  HBUINT32	معلومات_وٿي;
				/* Per Apple specification:
				 * Offset from the beginning of the table to the data.
				 * Per OT specification:
				 * Reserved. Not used; should be set to 0. */
  Array32_جو<معلومات_Map>
		معلومات_Maps;/* Array of data map records. */
  عام:
  DEFINE_سائيز_ARRAY (16, معلومات_Maps);
};

بنية meta_accelerator_t : meta::accelerator_t {
  meta_accelerator_t (hb_face_t *face) : meta::accelerator_t (face) {}
};

} /* namespace OT */


#جيڪڏهن_پورو /* HB_OT_META_TABLE_HH */
