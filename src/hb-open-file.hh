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

কাঠামো OpenTypeFontFile;
কাঠামো OpenTypeOffsetTable;
কাঠামো TTCHeader;


প্রকারলেখ কাঠামো TableRecord
{
  পূর্ণসংখ্যা cmp (Tag t) ধ্রুবক { ফেরত -t.cmp (tag); }

  HB_INTERNAL স্থির পূর্ণসংখ্যা cmp (ধ্রুবক শূন্য *pa, ধ্রুবক শূন্য *pb)
  {
    ধ্রুবক TableRecord *a = (ধ্রুবক TableRecord *) pa;
    ধ্রুবক TableRecord *b = (ধ্রুবক TableRecord *) pb;
    ফেরত b->cmp (a->tag);
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই));
  }

  Tag		tag;		/* 4-byte identifier. */
  Check_যোগ	check_যোগ;	/* CheckSum for this table. */
  Offset32	offset;		/* Offset from beginning of TrueType font
				 * file. */
  HBUINT32	দৈর্ঘ্য;		/* Length of this table. */
  সর্বজনীন:
  DEFINE_আকার_STATIC (16);
} OpenTypeTable;

প্রকারলেখ কাঠামো OpenTypeOffsetTable
{
  friend কাঠামো OpenTypeFontFile;

  unsigned পূর্ণসংখ্যা get_table_গুনতি () ধ্রুবক { ফেরত tables.len; }
  ধ্রুবক TableRecord& get_table (unsigned পূর্ণসংখ্যা i) ধ্রুবক
  { ফেরত tables[i]; }
  unsigned পূর্ণসংখ্যা get_table_tags (unsigned পূর্ণসংখ্যা  start_offset,
			       unsigned পূর্ণসংখ্যা *table_গুনতি_2, /* IN/OUT */
			       hb_tag_t     *table_tags /* OUT */) ধ্রুবক
  {
    যদি (table_গুনতি_2 && table_tags)
    {
      + tables.as_array ().sub_array (start_offset, table_গুনতি_2)
      | hb_map (&TableRecord::tag)
      | hb_sink (hb_array (table_tags, *table_গুনতি_2))
      ;
    }
    ফেরত tables.len;
  }
  যৌক্তিক find_table_সূচক (hb_tag_t tag, unsigned পূর্ণসংখ্যা *table_সূচক) ধ্রুবক
  {
    Tag t;
    t = tag;
    /* Use lfind for small fonts; there are fonts that have unsorted table entries;
     * those tend to work in other tools, so tolerate them.
     * https://github.com/harfbuzz/harfbuzz/issues/3065 */
    যদি (tables.len < 16)
      ফেরত tables.lfind (t, table_সূচক, HB_NOT_FOUND_STORE, সূচক::NOT_FOUND_সূচক);
    অন্যথা
      ফেরত tables.bfind (t, table_সূচক, HB_NOT_FOUND_STORE, সূচক::NOT_FOUND_সূচক);
  }
  ধ্রুবক TableRecord& get_table_by_tag (hb_tag_t tag) ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা table_সূচক;
    find_table_সূচক (tag, &table_সূচক);
    ফেরত get_table (table_সূচক);
  }

  সর্বজনীন:

  template <typename Iterator,
	    hb_requires ((hb_is_source_of<Iterator, hb_pair_t<hb_tag_t, hb_blob_t *>>::value))>
  যৌক্তিক serialize (hb_serialize_context_t *c,
		  hb_tag_t sfnt_tag,
		  Iterator it)
  {
    TRACE_SERIALIZE (এই);
    /* Alloc 12 for the OTHeader. */
    যদি (unlikely (!c->extend_min (এই))) return_trace (মিথ্যা);
    /* Write sfntVersion (bytes 0..3). */
    sfnt_version = sfnt_tag;
    /* Take space for numTables, searchRange, entrySelector, RangeShift
     * and the TableRecords themselves.  */
    unsigned অঙ্ক_বস্তুসমূহ = hb_len (it);
    যদি (unlikely (!tables.serialize (c, অঙ্ক_বস্তুসমূহ))) return_trace (মিথ্যা);

    ধ্রুবক অক্ষর *dir_end = (ধ্রুবক অক্ষর *) c->শীর্ষ_2;
    HBUINT32 *checksum_adjustment = nullptr;

    /* Write OffsetTables, alloc for and write actual table blobs. */
    unsigned i = 0;
    জন্য (hb_pair_t<hb_tag_t, hb_blob_t*> entry : it)
    {
      hb_blob_t *blob = entry.second;
      unsigned len = blob->দৈর্ঘ্য;

      /* Allocate room for the table and copy it. */
      অক্ষর *start = (অক্ষর *) c->allocate_আকার<শূন্য> (len, মিথ্যা);
      যদি (unlikely (!start)) ফেরত মিথ্যা;

      TableRecord &rec = tables.arrayZ[i];
      rec.tag = entry.first;
      rec.দৈর্ঘ্য = len;
      rec.offset = 0;
      যদি (unlikely (!c->check_assign (rec.offset,
				      (unsigned) ((অক্ষর *) start - (অক্ষর *) এই),
				      HB_SERIALIZE_ERROR_OFFSET_OVERFLOW)))
        return_trace (মিথ্যা);

      যদি (likely (len))
	hb_memcpy (start, blob->তথ্য, len);

      /* 4-byte alignment. */
      c->align (4);
      ধ্রুবক অক্ষর *end = (ধ্রুবক অক্ষর *) c->শীর্ষ_2;

      যদি (entry.first == HB_OT_TAG_শীর্ষ &&
	  (unsigned) (end - start) >= শীর্ষ_2::static_আকার)
      {
	শীর্ষ_2 *h = (শীর্ষ_2 *) start;
	checksum_adjustment = &h->check_যোগ_Adjustment;
	*checksum_adjustment = 0;
      }

      rec.check_যোগ.set_for_তথ্য (start, end - start);
      i++;
    }

    tables.qsort ();

    যদি (checksum_adjustment)
    {
      Check_যোগ checksum;

      /* The following line is a slower version of the following block. */
      //checksum.set_for_data (this, (const char *) c->head - (const char *) this);
      checksum.set_for_তথ্য (এই, dir_end - (ধ্রুবক অক্ষর *) এই);
      জন্য (unsigned পূর্ণসংখ্যা i = 0; i < অঙ্ক_বস্তুসমূহ; i++)
      {
	TableRecord &rec = tables.arrayZ[i];
	checksum = checksum + rec.check_যোগ;
      }

      *checksum_adjustment = 0xB1B0AFBAu - checksum;
    }

    return_trace (সত্য);
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) && tables.sanitize (c));
  }

  সুরক্ষিত:
  Tag		sfnt_version;	/* '\0\001\0\00' if TrueType / 'OTTO' if CFF */
  Bin_অনুসন্ধান_Array_Of<TableRecord>
		tables;
  সর্বজনীন:
  DEFINE_আকার_ARRAY (12, tables);
} OpenTypeFontFace;


/*
 * TrueType Collections
 */

কাঠামো TTCHeaderVersion1
{
  friend কাঠামো TTCHeader;

  using DirectoryList = Array32Of<Offset32To<OpenTypeOffsetTable>>;

  unsigned পূর্ণসংখ্যা get_face_গুনতি () ধ্রুবক { ফেরত get_directory_list ().len; }
  ধ্রুবক OpenTypeFontFace& get_face (unsigned পূর্ণসংখ্যা i) ধ্রুবক
  { ফেরত এই+get_directory_list ()[i]; }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
#ifndef HB_NO_BEYOND_64K
    যদি (version.minor >= 1)
    {
      যদি (unlikely (!table.sanitize_shallow (c)))
	return_trace (মিথ্যা);
      unsigned extra_আকার = version.major == 2 ? 12 : 0;
      যদি (unlikely (!c->check_range (&table, table.get_আকার () + extra_আকার)))
	return_trace (মিথ্যা);
      ধ্রুবক DirectoryList &table2 = StructAtOffset<DirectoryList> (&table,
								   table.get_আকার () + extra_আকার);
      return_trace (table2.sanitize (c, এই));
    }
#endif
    return_trace (table.sanitize (c, এই));
  }

  ব্যক্তিগত:
  ধ্রুবক DirectoryList& get_directory_list () ধ্রুবক
  {
#ifndef HB_NO_BEYOND_64K
    যদি (version.minor >= 1)
    {
      unsigned extra_আকার = version.major == 2 ? 12 : 0;
      ফেরত StructAtOffset<DirectoryList> (&table, table.get_আকার () + extra_আকার);
    }
#endif
    ফেরত table;
  }

  সুরক্ষিত:
  Tag		ttcTag;		/* TrueType Collection ID string: 'ttcf' */
  FixedVersion<>version;	/* Version of the TTC Header. */
  DirectoryList
		table;		/* Array of offsets to the OffsetTable for each font
				 * from the beginning of the file */
  সর্বজনীন:
  DEFINE_আকার_ARRAY (12, table);
};

কাঠামো TTCHeader
{
  friend কাঠামো OpenTypeFontFile;

  ব্যক্তিগত:

  unsigned পূর্ণসংখ্যা get_face_গুনতি () ধ্রুবক
  {
    নির্বাচন (u.header.version.major) {
    ক্ষেত্র 2: /* version 2 is compatible with version 1 */
    ক্ষেত্র 1: hb_barrier (); ফেরত u.version1.get_face_গুনতি ();
    পূর্বনির্ধারিত:ফেরত 0;
    }
  }
  ধ্রুবক OpenTypeFontFace& get_face (unsigned পূর্ণসংখ্যা i) ধ্রুবক
  {
    নির্বাচন (u.header.version.major) {
    ক্ষেত্র 2: /* version 2 is compatible with version 1 */
    ক্ষেত্র 1: hb_barrier (); ফেরত u.version1.get_face (i);
    পূর্বনির্ধারিত:ফেরত Null (OpenTypeFontFace);
    }
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    যদি (unlikely (!u.header.version.sanitize (c))) return_trace (মিথ্যা);
    hb_barrier ();
    নির্বাচন (u.header.version.major) {
    ক্ষেত্র 2: /* version 2 is compatible with version 1 */
    ক্ষেত্র 1: hb_barrier (); return_trace (u.version1.sanitize (c));
    পূর্বনির্ধারিত:return_trace (সত্য);
    }
  }

  সুরক্ষিত:
  সংঘ {
  কাঠামো {
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

কাঠামো ResourceRecord
{
  ধ্রুবক OpenTypeFontFace & get_face (ধ্রুবক শূন্য *তথ্য_base) ধ্রুবক
  { ফেরত * reinterpret_cast<ধ্রুবক OpenTypeFontFace *> ((তথ্য_base+offset).arrayZ); }

  যৌক্তিক sanitize (hb_sanitize_context_t *c,
		 ধ্রুবক শূন্য *তথ্য_base) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) &&
		  offset.sanitize (c, তথ্য_base) &&
		  hb_barrier () &&
		  get_face (তথ্য_base).sanitize (c));
  }

  সুরক্ষিত:
  HBUINT16	id;		/* Resource ID. */
  HBINT16	nameOffset;	/* Offset from beginning of resource name list
				 * to resource name, -1 means there is none. */
  HBUINT8	attrs;		/* Resource attributes */
  NNOffset24To<Array32Of<HBUINT8>>
		offset;		/* Offset from beginning of data block to
				 * data for this resource */
  HBUINT32	reserved;	/* Reserved for handle to resource */
  সর্বজনীন:
  DEFINE_আকার_STATIC (12);
};

#define HB_TAG_sfnt HB_TAG ('s','f','n','t')

কাঠামো ResourceTypeRecord
{
  unsigned পূর্ণসংখ্যা get_resource_গুনতি () ধ্রুবক
  { ফেরত tag == HB_TAG_sfnt ? res_গুনতি_M1 + 1 : 0; }

  যৌক্তিক is_sfnt () ধ্রুবক { ফেরত tag == HB_TAG_sfnt; }

  ধ্রুবক ResourceRecord& get_resource_record (unsigned পূর্ণসংখ্যা i,
					     ধ্রুবক শূন্য *type_base) ধ্রুবক
  { ফেরত (type_base+resourcesZ).as_array (get_resource_গুনতি ())[i]; }

  যৌক্তিক sanitize (hb_sanitize_context_t *c,
		 ধ্রুবক শূন্য *type_base,
		 ধ্রুবক শূন্য *তথ্য_base) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) &&
		  hb_barrier () &&
		  resourcesZ.sanitize (c, type_base,
				       get_resource_গুনতি (),
				       তথ্য_base));
  }

  সুরক্ষিত:
  Tag		tag;		/* Resource type. */
  HBUINT16	res_গুনতি_M1;	/* Number of resources minus 1. */
  NNOffset16To<UnsizedArrayOf<ResourceRecord>>
		resourcesZ;	/* Offset from beginning of resource type list
				 * to reference item list for this type. */
  সর্বজনীন:
  DEFINE_আকার_STATIC (8);
};

কাঠামো ResourceMap
{
  unsigned পূর্ণসংখ্যা get_face_গুনতি () ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা গুনতি = get_type_গুনতি ();
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
    {
      ধ্রুবক ResourceTypeRecord& type = get_type_record (i);
      যদি (type.is_sfnt ())
	ফেরত type.get_resource_গুনতি ();
    }
    ফেরত 0;
  }

  ধ্রুবক OpenTypeFontFace& get_face (unsigned পূর্ণসংখ্যা idx,
				    ধ্রুবক শূন্য *তথ্য_base) ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা গুনতি = get_type_গুনতি ();
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
    {
      ধ্রুবক ResourceTypeRecord& type = get_type_record (i);
      /* The check for idx < count is here because ResourceRecord is NOT null-safe.
       * Because an offset of 0 there does NOT mean null. */
      যদি (type.is_sfnt () && idx < type.get_resource_গুনতি ())
	ফেরত type.get_resource_record (idx, &(এই+typeList)).get_face (তথ্য_base);
    }
    ফেরত Null (OpenTypeFontFace);
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c, ধ্রুবক শূন্য *তথ্য_base) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) &&
		  hb_barrier () &&
		  typeList.sanitize (c, এই,
				     &(এই+typeList),
				     তথ্য_base));
  }

  ব্যক্তিগত:
  unsigned পূর্ণসংখ্যা get_type_গুনতি () ধ্রুবক { ফেরত (এই+typeList).lenM1 + 1; }

  ধ্রুবক ResourceTypeRecord& get_type_record (unsigned পূর্ণসংখ্যা i) ধ্রুবক
  { ফেরত (এই+typeList)[i]; }

  সুরক্ষিত:
  HBUINT8	reserved0[16];	/* Reserved for copy of resource header */
  HBUINT32	reserved1;	/* Reserved for handle to next resource map */
  HBUINT16	resreved2;	/* Reserved for file reference number */
  HBUINT16	attrs;		/* Resource fork attribute */
  NNOffset16To<ArrayOfM1<ResourceTypeRecord>>
		typeList;	/* Offset from beginning of map to
				 * resource type list */
  Offset16	nameList;	/* Offset from beginning of map to
				 * resource name list */
  সর্বজনীন:
  DEFINE_আকার_STATIC (28);
};

কাঠামো ResourceForkHeader
{
  unsigned পূর্ণসংখ্যা get_face_গুনতি () ধ্রুবক
  { ফেরত (এই+map).get_face_গুনতি (); }

  ধ্রুবক OpenTypeFontFace& get_face (unsigned পূর্ণসংখ্যা idx,
				    unsigned পূর্ণসংখ্যা *base_offset = nullptr) ধ্রুবক
  {
    ধ্রুবক OpenTypeFontFace &face = (এই+map).get_face (idx, &(এই+তথ্য));
    যদি (base_offset)
      *base_offset = (ধ্রুবক অক্ষর *) &face - (ধ্রুবক অক্ষর *) এই;
    ফেরত face;
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) &&
		  hb_barrier () &&
		  তথ্য.sanitize (c, এই, তথ্য_Len) &&
		  map.sanitize (c, এই, &(এই+তথ্য)));
  }

  সুরক্ষিত:
  NNOffset32To<UnsizedArrayOf<HBUINT8>>
		তথ্য;		/* Offset from beginning of resource fork
				 * to resource data */
  NNOffset32To<ResourceMap >
		map;		/* Offset from beginning of resource fork
				 * to resource map */
  HBUINT32	তথ্য_Len;	/* Length of resource data */
  HBUINT32	mapLen;		/* Length of resource map */
  সর্বজনীন:
  DEFINE_আকার_STATIC (16);
};

/*
 * OpenType Font File
 */

কাঠামো OpenTypeFontFile
{
  গণনা {
    CFFTag		= HB_TAG ('O','T','T','O'), /* OpenType with Postscript outlines */
    TrueTypeTag		= HB_TAG ( 0 , 1 , 0 , 0 ), /* OpenType with TrueType outlines */
    TTCTag		= HB_TAG ('t','t','c','f'), /* TrueType Collection */
    DFontTag		= HB_TAG ( 0 , 0 , 1 , 0 ), /* DFont Mac Resource Fork */
    TrueTag		= HB_TAG ('t','r','u','e'), /* Obsolete Apple TrueType */
    Typ1Tag		= HB_TAG ('t','y','p','1')  /* Obsolete Apple Type1 font in SFNT container */
  };

  hb_tag_t get_tag () ধ্রুবক { ফেরত u.tag.v; }

  unsigned পূর্ণসংখ্যা get_face_গুনতি () ধ্রুবক
  {
    নির্বাচন (u.tag.v) {
    ক্ষেত্র CFFTag:	/* All the non-collection tags */
    ক্ষেত্র TrueTag:
    ক্ষেত্র Typ1Tag:
    ক্ষেত্র TrueTypeTag:	ফেরত 1;
    ক্ষেত্র TTCTag:	hb_barrier (); ফেরত u.ttcHeader.get_face_গুনতি ();
    ক্ষেত্র DFontTag:	hb_barrier (); ফেরত u.rfHeader.get_face_গুনতি ();
    পূর্বনির্ধারিত:		ফেরত 0;
    }
  }
  ধ্রুবক OpenTypeFontFace& get_face (unsigned পূর্ণসংখ্যা i, unsigned পূর্ণসংখ্যা *base_offset = nullptr) ধ্রুবক
  {
    যদি (base_offset)
      *base_offset = 0;
    নির্বাচন (u.tag.v) {
    /* Note: for non-collection SFNT data we ignore index.  This is because
     * Apple dfont container is a container of SFNT's.  So each SFNT is a
     * non-TTC, but the index is more than zero. */
    ক্ষেত্র CFFTag:	/* All the non-collection tags */
    ক্ষেত্র TrueTag:
    ক্ষেত্র Typ1Tag:
    ক্ষেত্র TrueTypeTag:	hb_barrier (); ফেরত u.fontFace;
    ক্ষেত্র TTCTag:	hb_barrier (); ফেরত u.ttcHeader.get_face (i);
    ক্ষেত্র DFontTag:	hb_barrier (); ফেরত u.rfHeader.get_face (i, base_offset);
    পূর্বনির্ধারিত:		ফেরত Null (OpenTypeFontFace);
    }
  }

  template <typename Iterator,
	    hb_requires ((hb_is_source_of<Iterator, hb_pair_t<hb_tag_t, hb_blob_t *>>::value))>
  যৌক্তিক serialize_single (hb_serialize_context_t *c,
			 hb_tag_t sfnt_tag,
			 Iterator বস্তুসমূহ)
  {
    TRACE_SERIALIZE (এই);
    প্রতিজ্ঞা (sfnt_tag != TTCTag);
    যদি (unlikely (!c->extend_min (এই))) return_trace (মিথ্যা);
    return_trace (u.fontFace.serialize (c, sfnt_tag, বস্তুসমূহ));
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    যদি (unlikely (!u.tag.v.sanitize (c))) return_trace (মিথ্যা);
    hb_barrier ();
    নির্বাচন (u.tag.v) {
    ক্ষেত্র CFFTag:	/* All the non-collection tags */
    ক্ষেত্র TrueTag:
    ক্ষেত্র Typ1Tag:
    ক্ষেত্র TrueTypeTag:	hb_barrier (); return_trace (u.fontFace.sanitize (c));
    ক্ষেত্র TTCTag:	hb_barrier (); return_trace (u.ttcHeader.sanitize (c));
    ক্ষেত্র DFontTag:	hb_barrier (); return_trace (u.rfHeader.sanitize (c));
    পূর্বনির্ধারিত:		return_trace (সত্য);
    }
  }

  সুরক্ষিত:
  সংঘ {
  কাঠামো { Tag v; }	tag;		/* 4-byte identifier. */
  OpenTypeFontFace	fontFace;
  TTCHeader		ttcHeader;
  ResourceForkHeader	rfHeader;
  } u;
  সর্বজনীন:
  DEFINE_আকার_UNION (4, tag.v);
};


} /* namespace OT */


#endif /* HB_OPEN_FILE_HH */
