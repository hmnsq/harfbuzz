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

#ifndef HB_OT_META_TABLE_HH
#define HB_OT_META_TABLE_HH

#অন্তর্ভাব "hb-open-type.hh"

/*
 * meta -- Metadata Table
 * https://docs.microsoft.com/en-us/typography/opentype/spec/meta
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6meta.html
 */
#define HB_OT_TAG_meta HB_TAG ('m','e','t','a')


নামাকাশ OT {


কাঠামো তথ্য_Map
{
  পূর্ণসংখ্যা cmp (hb_tag_t a) ধ্রুবক { ফেরত tag.cmp (a); }

  hb_tag_t get_tag () ধ্রুবক { ফেরত tag; }

  hb_blob_t *reference_entry (hb_blob_t *meta_blob) ধ্রুবক
  { ফেরত hb_blob_create_sub_blob (meta_blob, তথ্য_Z, তথ্য_দৈর্ঘ্য_2); }

  যৌক্তিক sanitize (hb_sanitize_context_t *c, ধ্রুবক শূন্য *base) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (likely (c->check_struct (এই) &&
			  hb_barrier () &&
			  তথ্য_Z.sanitize (c, base, তথ্য_দৈর্ঘ্য_2)));
  }

  সুরক্ষিত:
  Tag		tag;		/* A tag indicating the type of metadata. */
  NNOffset32To<UnsizedArrayOf<HBUINT8>>
		তথ্য_Z;		/* Offset in bytes from the beginning of the
				 * metadata table to the data for this tag. */
  HBUINT32	তথ্য_দৈর্ঘ্য_2;	/* Length of the data. The data is not required to
				 * be padded to any byte boundary. */
  সর্বজনীন:
  DEFINE_আকার_STATIC (12);
};

কাঠামো meta
{
  স্থির constexpr hb_tag_t tableTag = HB_OT_TAG_meta;

  কাঠামো accelerator_t
  {
    accelerator_t (hb_face_t *face)
    { table = hb_sanitize_context_t ().reference_table<meta> (face); }
    ~accelerator_t () { table.destroy (); }

    hb_blob_t *reference_entry (hb_tag_t tag) ধ্রুবক
    { ফেরত table->তথ্য_Maps.lsearch (tag).reference_entry (table.get_blob ()); }

    unsigned পূর্ণসংখ্যা get_entries (unsigned পূর্ণসংখ্যা      start_offset,
			      unsigned পূর্ণসংখ্যা     *গুনতি,
			      hb_ot_meta_tag_t *entries) ধ্রুবক
    {
      যদি (গুনতি && entries)
      {
	+ table->তথ্য_Maps.as_array ().sub_array (start_offset, গুনতি)
	| hb_map (&তথ্য_Map::get_tag)
	| hb_map ([](hb_tag_t tag) { ফেরত (hb_ot_meta_tag_t) tag; })
	| hb_sink (hb_array (entries, *গুনতি))
	;
      }
      ফেরত table->তথ্য_Maps.len;
    }

    ব্যক্তিগত:
    hb_blob_ptr_t<meta> table;
  };

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (likely (c->check_struct (এই) &&
			  hb_barrier () &&
			  version == 1 &&
			  তথ্য_Maps.sanitize (c, এই)));
  }

  সুরক্ষিত:
  HBUINT32	version;	/* Version number of the metadata table — set to 1. */
  HBUINT32	flags;		/* Flags — currently unused; set to 0. */
  HBUINT32	তথ্য_Offset;
				/* Per Apple specification:
				 * Offset from the beginning of the table to the data.
				 * Per OT specification:
				 * Reserved. Not used; should be set to 0. */
  Array32Of<তথ্য_Map>
		তথ্য_Maps;/* Array of data map records. */
  সর্বজনীন:
  DEFINE_আকার_ARRAY (16, তথ্য_Maps);
};

কাঠামো meta_accelerator_t : meta::accelerator_t {
  meta_accelerator_t (hb_face_t *face) : meta::accelerator_t (face) {}
};

} /* namespace OT */


#endif /* HB_OT_META_TABLE_HH */
