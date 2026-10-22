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

#अगर_अघोषित HB_OT_META_TABLE_HH
#घोषणा HB_OT_META_TABLE_HH

#समावेश "hb-open-type.hh"

/*
 * meta -- Metadata Table
 * https://docs.microsoft.com/en-us/typography/opentype/spec/meta
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6meta.html
 */
#घोषणा HB_OT_TAG_meta HB_TAG ('m','e','t','a')


namespace OT {


काष्ठा तथ्य_Map
{
  पूर्णांक cmp (hb_tag_t a) स्थिर { वापस tag.cmp (a); }

  hb_tag_t get_tag () स्थिर { वापस tag; }

  hb_blob_t *reference_entry (hb_blob_t *meta_blob) स्थिर
  { वापस hb_blob_create_sub_blob (meta_blob, तथ्य_Z, तथ्य_लंबाई_2); }

  bool sanitize (hb_sanitize_context_t *c, स्थिर व्योम *base) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (likely (c->check_struct (यह) &&
			  hb_barrier () &&
			  तथ्य_Z.sanitize (c, base, तथ्य_लंबाई_2)));
  }

  रक्षित:
  Tag		tag;		/* A tag indicating the type of metadata. */
  NNOffset32To<UnsizedArrayOf<HBUINT8>>
		तथ्य_Z;		/* Offset in bytes from the beginning of the
				 * metadata table to the data for this tag. */
  HBUINT32	तथ्य_लंबाई_2;	/* Length of the data. The data is not required to
				 * be padded to any byte boundary. */
  खुला:
  DEFINE_आकार_STATIC (12);
};

काष्ठा meta
{
  अटल constexpr hb_tag_t tableTag = HB_OT_TAG_meta;

  काष्ठा accelerator_t
  {
    accelerator_t (hb_face_t *face)
    { table = hb_sanitize_context_t ().reference_table<meta> (face); }
    ~accelerator_t () { table.destroy (); }

    hb_blob_t *reference_entry (hb_tag_t tag) स्थिर
    { वापस table->तथ्य_Maps.lsearch (tag).reference_entry (table.get_blob ()); }

    अचिन्हित पूर्णांक get_entries (अचिन्हित पूर्णांक      start_offset,
			      अचिन्हित पूर्णांक     *गिनती,
			      hb_ot_meta_tag_t *entries) स्थिर
    {
      अगर (गिनती && entries)
      {
	+ table->तथ्य_Maps.as_array ().sub_array (start_offset, गिनती)
	| hb_map (&तथ्य_Map::get_tag)
	| hb_map ([](hb_tag_t tag) { वापस (hb_ot_meta_tag_t) tag; })
	| hb_sink (hb_array (entries, *गिनती))
	;
      }
      वापस table->तथ्य_Maps.len;
    }

    निजी:
    hb_blob_ptr_t<meta> table;
  };

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (likely (c->check_struct (यह) &&
			  hb_barrier () &&
			  version == 1 &&
			  तथ्य_Maps.sanitize (c, यह)));
  }

  रक्षित:
  HBUINT32	version;	/* Version number of the metadata table — set to 1. */
  HBUINT32	flags;		/* Flags — currently unused; set to 0. */
  HBUINT32	तथ्य_Offset;
				/* Per Apple specification:
				 * Offset from the beginning of the table to the data.
				 * Per OT specification:
				 * Reserved. Not used; should be set to 0. */
  Array32Of<तथ्य_Map>
		तथ्य_Maps;/* Array of data map records. */
  खुला:
  DEFINE_आकार_ARRAY (16, तथ्य_Maps);
};

काष्ठा meta_accelerator_t : meta::accelerator_t {
  meta_accelerator_t (hb_face_t *face) : meta::accelerator_t (face) {}
};

} /* namespace OT */


#पूर्ण_अगर /* HB_OT_META_TABLE_HH */
