/*
 * Copyright © 2007,2008,2009  Red Hat, Inc.
 * Copyright © 2012  Google, Inc.
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
 * Red Hat Author(s): Behdad Esfahbod
 * Google Author(s): Behdad Esfahbod
 */

#ifndef HB_OPEN_FILE_HH
#define HB_OPEN_FILE_HH

#অন্তর্ভাব "hb-open-type.hh"
#অন্তর্ভাব "hb-ot-head-table.hh"


নামাকাশ OT {

/*
 *
 * The OpenType Font File
 *
 */


/*
 * Organization of an OpenType Font
 */

সংরচনা OpenTypeFontFile;
সংরচনা OpenTypeOffsetTable;
সংরচনা TTCHeader;


প্রকারনির্দেশ সংরচনা TableRecord
{
  পূর্ণাঙ্ক cmp (Tag t) ধ্রুব { প্রত্যাগম -t.cmp (tag); }

  HB_INTERNAL স্থির পূর্ণাঙ্ক cmp (ধ্রুব শূন্য *pa, ধ্রুব শূন্য *pb)
  {
    ধ্রুব TableRecord *a = (ধ্রুব TableRecord *) pa;
    ধ্রুব TableRecord *b = (ধ্রুব TableRecord *) pb;
    প্রত্যাগম b->cmp (a->tag);
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্));
  }

  Tag		tag;		/* 4-byte identifier. */
  CheckSum	checkSum;	/* CheckSum for this table. */
  Offset32	offset;		/* Offset from beginning of TrueType font
				 * file. */
  HBUINT32	length;		/* Length of this table. */
  সার্বজনিক:
  DEFINE_SIZE_STATIC (16);
} OpenTypeTable;

প্রকারনির্দেশ সংরচনা OpenTypeOffsetTable
{
  friend সংরচনা OpenTypeFontFile;

  unsigned পূর্ণাঙ্ক get_table_গুনতি () ধ্রুব { প্রত্যাগম tables.len; }
  ধ্রুব TableRecord& get_table (unsigned পূর্ণাঙ্ক i) ধ্রুব
  { প্রত্যাগম tables[i]; }
  unsigned পূর্ণাঙ্ক get_table_tags (unsigned পূর্ণাঙ্ক  start_offset,
			       unsigned পূর্ণাঙ্ক *table_গুনতি_2, /* IN/OUT */
			       hb_tag_t     *table_tags /* OUT */) ধ্রুব
  {
    যদি (table_গুনতি_2 && table_tags)
    {
      + tables.as_array ().sub_array (start_offset, table_গুনতি_2)
      | hb_map (&TableRecord::tag)
      | hb_sink (hb_array (table_tags, *table_গুনতি_2))
      ;
    }
    প্রত্যাগম tables.len;
  }
  তার্কিক find_table_index (hb_tag_t tag, unsigned পূর্ণাঙ্ক *table_index) ধ্রুব
  {
    Tag t;
    t = tag;
    /* Use lfind for small fonts; there are fonts that have unsorted table entries;
     * those tend to work in other tools, so tolerate them.
     * https://github.com/harfbuzz/harfbuzz/issues/3065 */
    যদি (tables.len < 16)
      প্রত্যাগম tables.lfind (t, table_index, HB_NOT_FOUND_STORE, Index::NOT_FOUND_INDEX);
    অন্যথা
      প্রত্যাগম tables.bfind (t, table_index, HB_NOT_FOUND_STORE, Index::NOT_FOUND_INDEX);
  }
  ধ্রুব TableRecord& get_table_by_tag (hb_tag_t tag) ধ্রুব
  {
    unsigned পূর্ণাঙ্ক table_index;
    find_table_index (tag, &table_index);
    প্রত্যাগম get_table (table_index);
  }

  সার্বজনিক:

  template <typename Iterator,
	    hb_requires ((hb_is_source_of<Iterator, hb_pair_t<hb_tag_t, hb_blob_t *>>::value))>
  তার্কিক serialize (hb_serialize_context_t *c,
		  hb_tag_t sfnt_tag,
		  Iterator it)
  {
    TRACE_SERIALIZE (এতত্);
    /* Alloc 12 for the OTHeader. */
    যদি (unlikely (!c->extend_min (এতত্))) return_trace (অসত্য);
    /* Write sfntVersion (bytes 0..3). */
    sfnt_version = sfnt_tag;
    /* Take space for numTables, searchRange, entrySelector, RangeShift
     * and the TableRecords themselves.  */
    unsigned অঙ্ক_বস্তুসমূহ = hb_len (it);
    যদি (unlikely (!tables.serialize (c, অঙ্ক_বস্তুসমূহ))) return_trace (অসত্য);

    ধ্রুব বর্ণ *dir_end = (ধ্রুব বর্ণ *) c->head;
    HBUINT32 *checksum_adjustment = nullptr;

    /* Write OffsetTables, alloc for and write actual table blobs. */
    unsigned i = 0;
    প্রতি (hb_pair_t<hb_tag_t, hb_blob_t*> entry : it)
    {
      hb_blob_t *blob = entry.second;
      unsigned len = blob->length;

      /* Allocate room for the table and copy it. */
      বর্ণ *start = (বর্ণ *) c->allocate_size<শূন্য> (len, অসত্য);
      যদি (unlikely (!start)) প্রত্যাগম অসত্য;

      TableRecord &rec = tables.arrayZ[i];
      rec.tag = entry.first;
      rec.length = len;
      rec.offset = 0;
      যদি (unlikely (!c->check_assign (rec.offset,
				      (unsigned) ((বর্ণ *) start - (বর্ণ *) এতত্),
				      HB_SERIALIZE_ERROR_OFFSET_OVERFLOW)))
        return_trace (অসত্য);

      যদি (likely (len))
	hb_memcpy (start, blob->data, len);

      /* 4-byte alignment. */
      c->align (4);
      ধ্রুব বর্ণ *end = (ধ্রুব বর্ণ *) c->head;

      যদি (entry.first == HB_OT_TAG_head &&
	  (unsigned) (end - start) >= head::static_size)
      {
	head *h = (head *) start;
	checksum_adjustment = &h->checkSumAdjustment;
	*checksum_adjustment = 0;
      }

      rec.checkSum.set_for_data (start, end - start);
      i++;
    }

    tables.qsort ();

    যদি (checksum_adjustment)
    {
      CheckSum checksum;

      /* The following line is a slower version of the following block. */
      //checksum.set_for_data (this, (const char *) c->head - (const char *) this);
      checksum.set_for_data (এতত্, dir_end - (ধ্রুব বর্ণ *) এতত্);
      প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < অঙ্ক_বস্তুসমূহ; i++)
      {
	TableRecord &rec = tables.arrayZ[i];
	checksum = checksum + rec.checkSum;
      }

      *checksum_adjustment = 0xB1B0AFBAu - checksum;
    }

    return_trace (সত্য);
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) && tables.sanitize (c));
  }

  রক্ষিত:
  Tag		sfnt_version;	/* '\0\001\0\00' if TrueType / 'OTTO' if CFF */
  Bin_অনুসন্ধান_Array_Of<TableRecord>
		tables;
  সার্বজনিক:
  DEFINE_SIZE_ARRAY (12, tables);
} OpenTypeFontFace;


/*
 * TrueType Collections
 */

সংরচনা TTCHeaderVersion1
{
  friend সংরচনা TTCHeader;

  using DirectoryList = Array32Of<Offset32To<OpenTypeOffsetTable>>;

  unsigned পূর্ণাঙ্ক get_face_গুনতি () ধ্রুব { প্রত্যাগম get_directory_list ().len; }
  ধ্রুব OpenTypeFontFace& get_face (unsigned পূর্ণাঙ্ক i) ধ্রুব
  { প্রত্যাগম এতত্+get_directory_list ()[i]; }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
#ifndef HB_NO_BEYOND_64K
    যদি (version.minor >= 1)
    {
      যদি (unlikely (!table.sanitize_shallow (c)))
	return_trace (অসত্য);
      unsigned extra_size = version.major == 2 ? 12 : 0;
      যদি (unlikely (!c->check_range (&table, table.get_size () + extra_size)))
	return_trace (অসত্য);
      ধ্রুব DirectoryList &table2 = StructAtOffset<DirectoryList> (&table,
								   table.get_size () + extra_size);
      return_trace (table2.sanitize (c, এতত্));
    }
#endif
    return_trace (table.sanitize (c, এতত্));
  }

  গুপ্ত:
  ধ্রুব DirectoryList& get_directory_list () ধ্রুব
  {
#ifndef HB_NO_BEYOND_64K
    যদি (version.minor >= 1)
    {
      unsigned extra_size = version.major == 2 ? 12 : 0;
      প্রত্যাগম StructAtOffset<DirectoryList> (&table, table.get_size () + extra_size);
    }
#endif
    প্রত্যাগম table;
  }

  রক্ষিত:
  Tag		ttcTag;		/* TrueType Collection ID string: 'ttcf' */
  FixedVersion<>version;	/* Version of the TTC Header. */
  DirectoryList
		table;		/* Array of offsets to the OffsetTable for each font
				 * from the beginning of the file */
  সার্বজনিক:
  DEFINE_SIZE_ARRAY (12, table);
};

সংরচনা TTCHeader
{
  friend সংরচনা OpenTypeFontFile;

  গুপ্ত:

  unsigned পূর্ণাঙ্ক get_face_গুনতি () ধ্রুব
  {
    বিকল্প (u.header.version.major) {
    পক্ষ 2: /* version 2 is compatible with version 1 */
    পক্ষ 1: hb_barrier (); প্রত্যাগম u.version1.get_face_গুনতি ();
    স্বভাব:প্রত্যাগম 0;
    }
  }
  ধ্রুব OpenTypeFontFace& get_face (unsigned পূর্ণাঙ্ক i) ধ্রুব
  {
    বিকল্প (u.header.version.major) {
    পক্ষ 2: /* version 2 is compatible with version 1 */
    পক্ষ 1: hb_barrier (); প্রত্যাগম u.version1.get_face (i);
    স্বভাব:প্রত্যাগম Null (OpenTypeFontFace);
    }
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    যদি (unlikely (!u.header.version.sanitize (c))) return_trace (অসত্য);
    hb_barrier ();
    বিকল্প (u.header.version.major) {
    পক্ষ 2: /* version 2 is compatible with version 1 */
    পক্ষ 1: hb_barrier (); return_trace (u.version1.sanitize (c));
    স্বভাব:return_trace (সত্য);
    }
  }

  রক্ষিত:
  সঙ্ঘ {
  সংরচনা {
  Tag		ttcTag;		/* TrueType Collection ID string: 'ttcf' */
  FixedVersion<>version;	/* Version of the TTC Header (1.0 or 2.0),
				 * 0x00010000u or 0x00020000u */
  }			header;
  TTCHeaderVersion1	version1;
  } u;
};

/*
 * Mac Resource Fork
 *
 * http://mirror.informatimago.com/next/developer.apple.com/documentation/mac/MoreToolbox/MoreToolbox-99.html
 */

সংরচনা ResourceRecord
{
  ধ্রুব OpenTypeFontFace & get_face (ধ্রুব শূন্য *data_base) ধ্রুব
  { প্রত্যাগম * reinterpret_cast<ধ্রুব OpenTypeFontFace *> ((data_base+offset).arrayZ); }

  তার্কিক sanitize (hb_sanitize_context_t *c,
		 ধ্রুব শূন্য *data_base) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) &&
		  offset.sanitize (c, data_base) &&
		  hb_barrier () &&
		  get_face (data_base).sanitize (c));
  }

  রক্ষিত:
  HBUINT16	id;		/* Resource ID. */
  HBINT16	nameOffset;	/* Offset from beginning of resource name list
				 * to resource name, -1 means there is none. */
  HBUINT8	attrs;		/* Resource attributes */
  NNOffset24To<Array32Of<HBUINT8>>
		offset;		/* Offset from beginning of data block to
				 * data for this resource */
  HBUINT32	reserved;	/* Reserved for handle to resource */
  সার্বজনিক:
  DEFINE_SIZE_STATIC (12);
};

#define HB_TAG_sfnt HB_TAG ('s','f','n','t')

সংরচনা ResourceTypeRecord
{
  unsigned পূর্ণাঙ্ক get_resource_গুনতি () ধ্রুব
  { প্রত্যাগম tag == HB_TAG_sfnt ? res_গুনতি_M1 + 1 : 0; }

  তার্কিক is_sfnt () ধ্রুব { প্রত্যাগম tag == HB_TAG_sfnt; }

  ধ্রুব ResourceRecord& get_resource_record (unsigned পূর্ণাঙ্ক i,
					     ধ্রুব শূন্য *type_base) ধ্রুব
  { প্রত্যাগম (type_base+resourcesZ).as_array (get_resource_গুনতি ())[i]; }

  তার্কিক sanitize (hb_sanitize_context_t *c,
		 ধ্রুব শূন্য *type_base,
		 ধ্রুব শূন্য *data_base) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) &&
		  hb_barrier () &&
		  resourcesZ.sanitize (c, type_base,
				       get_resource_গুনতি (),
				       data_base));
  }

  রক্ষিত:
  Tag		tag;		/* Resource type. */
  HBUINT16	res_গুনতি_M1;	/* Number of resources minus 1. */
  NNOffset16To<UnsizedArrayOf<ResourceRecord>>
		resourcesZ;	/* Offset from beginning of resource type list
				 * to reference item list for this type. */
  সার্বজনিক:
  DEFINE_SIZE_STATIC (8);
};

সংরচনা ResourceMap
{
  unsigned পূর্ণাঙ্ক get_face_গুনতি () ধ্রুব
  {
    unsigned পূর্ণাঙ্ক গুনতি = get_type_গুনতি ();
    প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
    {
      ধ্রুব ResourceTypeRecord& type = get_type_record (i);
      যদি (type.is_sfnt ())
	প্রত্যাগম type.get_resource_গুনতি ();
    }
    প্রত্যাগম 0;
  }

  ধ্রুব OpenTypeFontFace& get_face (unsigned পূর্ণাঙ্ক idx,
				    ধ্রুব শূন্য *data_base) ধ্রুব
  {
    unsigned পূর্ণাঙ্ক গুনতি = get_type_গুনতি ();
    প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
    {
      ধ্রুব ResourceTypeRecord& type = get_type_record (i);
      /* The check for idx < count is here because ResourceRecord is NOT null-safe.
       * Because an offset of 0 there does NOT mean null. */
      যদি (type.is_sfnt () && idx < type.get_resource_গুনতি ())
	প্রত্যাগম type.get_resource_record (idx, &(এতত্+typeList)).get_face (data_base);
    }
    প্রত্যাগম Null (OpenTypeFontFace);
  }

  তার্কিক sanitize (hb_sanitize_context_t *c, ধ্রুব শূন্য *data_base) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) &&
		  hb_barrier () &&
		  typeList.sanitize (c, এতত্,
				     &(এতত্+typeList),
				     data_base));
  }

  গুপ্ত:
  unsigned পূর্ণাঙ্ক get_type_গুনতি () ধ্রুব { প্রত্যাগম (এতত্+typeList).lenM1 + 1; }

  ধ্রুব ResourceTypeRecord& get_type_record (unsigned পূর্ণাঙ্ক i) ধ্রুব
  { প্রত্যাগম (এতত্+typeList)[i]; }

  রক্ষিত:
  HBUINT8	reserved0[16];	/* Reserved for copy of resource header */
  HBUINT32	reserved1;	/* Reserved for handle to next resource map */
  HBUINT16	resreved2;	/* Reserved for file reference number */
  HBUINT16	attrs;		/* Resource fork attribute */
  NNOffset16To<ArrayOfM1<ResourceTypeRecord>>
		typeList;	/* Offset from beginning of map to
				 * resource type list */
  Offset16	nameList;	/* Offset from beginning of map to
				 * resource name list */
  সার্বজনিক:
  DEFINE_SIZE_STATIC (28);
};

সংরচনা ResourceForkHeader
{
  unsigned পূর্ণাঙ্ক get_face_গুনতি () ধ্রুব
  { প্রত্যাগম (এতত্+map).get_face_গুনতি (); }

  ধ্রুব OpenTypeFontFace& get_face (unsigned পূর্ণাঙ্ক idx,
				    unsigned পূর্ণাঙ্ক *base_offset = nullptr) ধ্রুব
  {
    ধ্রুব OpenTypeFontFace &face = (এতত্+map).get_face (idx, &(এতত্+data));
    যদি (base_offset)
      *base_offset = (ধ্রুব বর্ণ *) &face - (ধ্রুব বর্ণ *) এতত্;
    প্রত্যাগম face;
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) &&
		  hb_barrier () &&
		  data.sanitize (c, এতত্, dataLen) &&
		  map.sanitize (c, এতত্, &(এতত্+data)));
  }

  রক্ষিত:
  NNOffset32To<UnsizedArrayOf<HBUINT8>>
		data;		/* Offset from beginning of resource fork
				 * to resource data */
  NNOffset32To<ResourceMap >
		map;		/* Offset from beginning of resource fork
				 * to resource map */
  HBUINT32	dataLen;	/* Length of resource data */
  HBUINT32	mapLen;		/* Length of resource map */
  সার্বজনিক:
  DEFINE_SIZE_STATIC (16);
};

/*
 * OpenType Font File
 */

সংরচনা OpenTypeFontFile
{
  গণনা {
    CFFTag		= HB_TAG ('O','T','T','O'), /* OpenType with Postscript outlines */
    TrueTypeTag		= HB_TAG ( 0 , 1 , 0 , 0 ), /* OpenType with TrueType outlines */
    TTCTag		= HB_TAG ('t','t','c','f'), /* TrueType Collection */
    DFontTag		= HB_TAG ( 0 , 0 , 1 , 0 ), /* DFont Mac Resource Fork */
    TrueTag		= HB_TAG ('t','r','u','e'), /* Obsolete Apple TrueType */
    Typ1Tag		= HB_TAG ('t','y','p','1')  /* Obsolete Apple Type1 font in SFNT container */
  };

  hb_tag_t get_tag () ধ্রুব { প্রত্যাগম u.tag.v; }

  unsigned পূর্ণাঙ্ক get_face_গুনতি () ধ্রুব
  {
    বিকল্প (u.tag.v) {
    পক্ষ CFFTag:	/* All the non-collection tags */
    পক্ষ TrueTag:
    পক্ষ Typ1Tag:
    পক্ষ TrueTypeTag:	প্রত্যাগম 1;
    পক্ষ TTCTag:	hb_barrier (); প্রত্যাগম u.ttcHeader.get_face_গুনতি ();
    পক্ষ DFontTag:	hb_barrier (); প্রত্যাগম u.rfHeader.get_face_গুনতি ();
    স্বভাব:		প্রত্যাগম 0;
    }
  }
  ধ্রুব OpenTypeFontFace& get_face (unsigned পূর্ণাঙ্ক i, unsigned পূর্ণাঙ্ক *base_offset = nullptr) ধ্রুব
  {
    যদি (base_offset)
      *base_offset = 0;
    বিকল্প (u.tag.v) {
    /* Note: for non-collection SFNT data we ignore index.  This is because
     * Apple dfont container is a container of SFNT's.  So each SFNT is a
     * non-TTC, but the index is more than zero. */
    পক্ষ CFFTag:	/* All the non-collection tags */
    পক্ষ TrueTag:
    পক্ষ Typ1Tag:
    পক্ষ TrueTypeTag:	hb_barrier (); প্রত্যাগম u.fontFace;
    পক্ষ TTCTag:	hb_barrier (); প্রত্যাগম u.ttcHeader.get_face (i);
    পক্ষ DFontTag:	hb_barrier (); প্রত্যাগম u.rfHeader.get_face (i, base_offset);
    স্বভাব:		প্রত্যাগম Null (OpenTypeFontFace);
    }
  }

  template <typename Iterator,
	    hb_requires ((hb_is_source_of<Iterator, hb_pair_t<hb_tag_t, hb_blob_t *>>::value))>
  তার্কিক serialize_single (hb_serialize_context_t *c,
			 hb_tag_t sfnt_tag,
			 Iterator বস্তুসমূহ)
  {
    TRACE_SERIALIZE (এতত্);
    প্রতিজ্ঞা (sfnt_tag != TTCTag);
    যদি (unlikely (!c->extend_min (এতত্))) return_trace (অসত্য);
    return_trace (u.fontFace.serialize (c, sfnt_tag, বস্তুসমূহ));
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    যদি (unlikely (!u.tag.v.sanitize (c))) return_trace (অসত্য);
    hb_barrier ();
    বিকল্প (u.tag.v) {
    পক্ষ CFFTag:	/* All the non-collection tags */
    পক্ষ TrueTag:
    পক্ষ Typ1Tag:
    পক্ষ TrueTypeTag:	hb_barrier (); return_trace (u.fontFace.sanitize (c));
    পক্ষ TTCTag:	hb_barrier (); return_trace (u.ttcHeader.sanitize (c));
    পক্ষ DFontTag:	hb_barrier (); return_trace (u.rfHeader.sanitize (c));
    স্বভাব:		return_trace (সত্য);
    }
  }

  রক্ষিত:
  সঙ্ঘ {
  সংরচনা { Tag v; }	tag;		/* 4-byte identifier. */
  OpenTypeFontFace	fontFace;
  TTCHeader		ttcHeader;
  ResourceForkHeader	rfHeader;
  } u;
  সার্বজনিক:
  DEFINE_SIZE_UNION (4, tag.v);
};


} /* namespace OT */


#endif /* HB_OPEN_FILE_HH */
