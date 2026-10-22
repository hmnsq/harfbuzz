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

#அந்தர்பாவ "hb-open-type.hh"

/*
 * meta -- Metadata Table
 * https://docs.microsoft.com/en-us/typography/opentype/spec/meta
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6meta.html
 */
#define HB_OT_TAG_meta HB_TAG ('m','e','t','a')


நாமாகாஶ OT {


கட்டமைப்பு DataMap
{
  முழு cmp (hb_tag_t a) மாறிலி { திரும்பு tag.cmp (a); }

  hb_tag_t get_tag () மாறிலி { திரும்பு tag; }

  hb_blob_t *reference_entry (hb_blob_t *meta_blob) மாறிலி
  { திரும்பு hb_blob_create_sub_blob (meta_blob, dataZ, dataLength); }

  தருக்க sanitize (hb_sanitize_context_t *c, மாறிலி வெறுமை *base) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (likely (c->check_struct (இது) &&
			  hb_barrier () &&
			  dataZ.sanitize (c, base, dataLength)));
  }

  பாதுகாக்கப்பட்ட:
  Tag		tag;		/* A tag indicating the type of metadata. */
  NNOffset32To<UnsizedArrayOf<HBUINT8>>
		dataZ;		/* Offset in bytes from the beginning of the
				 * metadata table to the data for this tag. */
  HBUINT32	dataLength;	/* Length of the data. The data is not required to
				 * be padded to any byte boundary. */
  பொது:
  DEFINE_SIZE_STATIC (12);
};

கட்டமைப்பு meta
{
  நிலையான constexpr hb_tag_t tableTag = HB_OT_TAG_meta;

  கட்டமைப்பு accelerator_t
  {
    accelerator_t (hb_face_t *face)
    { table = hb_sanitize_context_t ().reference_table<meta> (face); }
    ~accelerator_t () { table.destroy (); }

    hb_blob_t *reference_entry (hb_tag_t tag) மாறிலி
    { திரும்பு table->dataMaps.lsearch (tag).reference_entry (table.get_blob ()); }

    unsigned முழு get_entries (unsigned முழு      start_offset,
			      unsigned முழு     *மொத்தம்,
			      hb_ot_meta_tag_t *entries) மாறிலி
    {
      எனில் (மொத்தம் && entries)
      {
	+ table->dataMaps.as_array ().sub_array (start_offset, மொத்தம்)
	| hb_map (&DataMap::get_tag)
	| hb_map ([](hb_tag_t tag) { திரும்பு (hb_ot_meta_tag_t) tag; })
	| hb_sink (hb_array (entries, *மொத்தம்))
	;
      }
      திரும்பு table->dataMaps.len;
    }

    தனிப்பட்ட:
    hb_blob_ptr_t<meta> table;
  };

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (likely (c->check_struct (இது) &&
			  hb_barrier () &&
			  version == 1 &&
			  dataMaps.sanitize (c, இது)));
  }

  பாதுகாக்கப்பட்ட:
  HBUINT32	version;	/* Version number of the metadata table — set to 1. */
  HBUINT32	flags;		/* Flags — currently unused; set to 0. */
  HBUINT32	dataOffset;
				/* Per Apple specification:
				 * Offset from the beginning of the table to the data.
				 * Per OT specification:
				 * Reserved. Not used; should be set to 0. */
  Array32Of<DataMap>
		dataMaps;/* Array of data map records. */
  பொது:
  DEFINE_SIZE_ARRAY (16, dataMaps);
};

கட்டமைப்பு meta_accelerator_t : meta::accelerator_t {
  meta_accelerator_t (hb_face_t *face) : meta::accelerator_t (face) {}
};

} /* namespace OT */


#endif /* HB_OT_META_TABLE_HH */
