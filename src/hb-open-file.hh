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

#شامل "hb-open-type.hh"
#شامل "hb-ot-head-table.hh"


فضاینام OT {

/*
 *
 * The OpenType Font File
 *
 */


/*
 * Organization of an OpenType Font
 */

ساختار OpenTypeFontFile;
ساختار OpenTypeOffsetTable;
ساختار TTCHeader;


تعریفنوع ساختار TableRecord
{
  صحیح cmp (Tag t) ثابت { بازگشت -t.cmp (tag); }

  HB_INTERNAL ایستا صحیح cmp (ثابت تهی *pa, ثابت تهی *pb)
  {
    ثابت TableRecord *a = (ثابت TableRecord *) pa;
    ثابت TableRecord *b = (ثابت TableRecord *) pb;
    بازگشت b->cmp (a->tag);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این));
  }

  Tag		tag;		/* 4-byte identifier. */
  CheckSum	checkSum;	/* CheckSum for this table. */
  Offset32	offset;		/* Offset from beginning of TrueType font
				 * file. */
  HBUINT32	length;		/* Length of this table. */
  عمومی:
  DEFINE_SIZE_STATIC (16);
} OpenTypeTable;

تعریفنوع ساختار OpenTypeOffsetTable
{
  friend ساختار OpenTypeFontFile;

  unsigned صحیح get_table_شمار () ثابت { بازگشت tables.len; }
  ثابت TableRecord& get_table (unsigned صحیح i) ثابت
  { بازگشت tables[i]; }
  unsigned صحیح get_table_tags (unsigned صحیح  start_offset,
			       unsigned صحیح *table_شمار_2, /* IN/OUT */
			       hb_tag_t     *table_tags /* OUT */) ثابت
  {
    اگر (table_شمار_2 && table_tags)
    {
      + tables.as_array ().sub_array (start_offset, table_شمار_2)
      | hb_map (&TableRecord::tag)
      | hb_sink (hb_array (table_tags, *table_شمار_2))
      ;
    }
    بازگشت tables.len;
  }
  منطقی find_table_index (hb_tag_t tag, unsigned صحیح *table_index) ثابت
  {
    Tag t;
    t = tag;
    /* Use lfind for small fonts; there are fonts that have unsorted table entries;
     * those tend to work in other tools, so tolerate them.
     * https://github.com/harfbuzz/harfbuzz/issues/3065 */
    اگر (tables.len < 16)
      بازگشت tables.lfind (t, table_index, HB_NOT_FOUND_STORE, Index::NOT_FOUND_INDEX);
    وگرنه
      بازگشت tables.bfind (t, table_index, HB_NOT_FOUND_STORE, Index::NOT_FOUND_INDEX);
  }
  ثابت TableRecord& get_table_by_tag (hb_tag_t tag) ثابت
  {
    unsigned صحیح table_index;
    find_table_index (tag, &table_index);
    بازگشت get_table (table_index);
  }

  عمومی:

  template <typename Iterator,
	    hb_requires ((hb_is_source_of<Iterator, hb_pair_t<hb_tag_t, hb_blob_t *>>::value))>
  منطقی serialize (hb_serialize_context_t *c,
		  hb_tag_t sfnt_tag,
		  Iterator it)
  {
    TRACE_SERIALIZE (این);
    /* Alloc 12 for the OTHeader. */
    اگر (unlikely (!c->extend_min (این))) return_trace (نادرست);
    /* Write sfntVersion (bytes 0..3). */
    sfnt_version = sfnt_tag;
    /* Take space for numTables, searchRange, entrySelector, RangeShift
     * and the TableRecords themselves.  */
    unsigned رقم_اقلام = hb_len (it);
    اگر (unlikely (!tables.serialize (c, رقم_اقلام))) return_trace (نادرست);

    ثابت نویسه *dir_end = (ثابت نویسه *) c->head;
    HBUINT32 *checksum_adjustment = nullptr;

    /* Write OffsetTables, alloc for and write actual table blobs. */
    unsigned i = 0;
    برای (hb_pair_t<hb_tag_t, hb_blob_t*> entry : it)
    {
      hb_blob_t *blob = entry.second;
      unsigned len = blob->length;

      /* Allocate room for the table and copy it. */
      نویسه *start = (نویسه *) c->allocate_size<تهی> (len, نادرست);
      اگر (unlikely (!start)) بازگشت نادرست;

      TableRecord &rec = tables.arrayZ[i];
      rec.tag = entry.first;
      rec.length = len;
      rec.offset = 0;
      اگر (unlikely (!c->check_assign (rec.offset,
				      (unsigned) ((نویسه *) start - (نویسه *) این),
				      HB_SERIALIZE_ERROR_OFFSET_OVERFLOW)))
        return_trace (نادرست);

      اگر (likely (len))
	hb_memcpy (start, blob->data, len);

      /* 4-byte alignment. */
      c->align (4);
      ثابت نویسه *end = (ثابت نویسه *) c->head;

      اگر (entry.first == HB_OT_TAG_head &&
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

    اگر (checksum_adjustment)
    {
      CheckSum checksum;

      /* The following line is a slower version of the following block. */
      //checksum.set_for_data (this, (const char *) c->head - (const char *) this);
      checksum.set_for_data (این, dir_end - (ثابت نویسه *) این);
      برای (unsigned صحیح i = 0; i < رقم_اقلام; i++)
      {
	TableRecord &rec = tables.arrayZ[i];
	checksum = checksum + rec.checkSum;
      }

      *checksum_adjustment = 0xB1B0AFBAu - checksum;
    }

    return_trace (درست);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) && tables.sanitize (c));
  }

  محافظتشده:
  Tag		sfnt_version;	/* '\0\001\0\00' if TrueType / 'OTTO' if CFF */
  Bin_جستجو_Array_Of<TableRecord>
		tables;
  عمومی:
  DEFINE_SIZE_ARRAY (12, tables);
} OpenTypeFontFace;


/*
 * TrueType Collections
 */

ساختار TTCHeaderVersion1
{
  friend ساختار TTCHeader;

  using DirectoryList = Array32Of<Offset32To<OpenTypeOffsetTable>>;

  unsigned صحیح get_face_شمار () ثابت { بازگشت get_directory_list ().len; }
  ثابت OpenTypeFontFace& get_face (unsigned صحیح i) ثابت
  { بازگشت این+get_directory_list ()[i]; }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
#ifndef HB_NO_BEYOND_64K
    اگر (version.minor >= 1)
    {
      اگر (unlikely (!table.sanitize_shallow (c)))
	return_trace (نادرست);
      unsigned extra_size = version.major == 2 ? 12 : 0;
      اگر (unlikely (!c->check_range (&table, table.get_size () + extra_size)))
	return_trace (نادرست);
      ثابت DirectoryList &table2 = StructAtOffset<DirectoryList> (&table,
								   table.get_size () + extra_size);
      return_trace (table2.sanitize (c, این));
    }
#endif
    return_trace (table.sanitize (c, این));
  }

  خصوصی:
  ثابت DirectoryList& get_directory_list () ثابت
  {
#ifndef HB_NO_BEYOND_64K
    اگر (version.minor >= 1)
    {
      unsigned extra_size = version.major == 2 ? 12 : 0;
      بازگشت StructAtOffset<DirectoryList> (&table, table.get_size () + extra_size);
    }
#endif
    بازگشت table;
  }

  محافظتشده:
  Tag		ttcTag;		/* TrueType Collection ID string: 'ttcf' */
  FixedVersion<>version;	/* Version of the TTC Header. */
  DirectoryList
		table;		/* Array of offsets to the OffsetTable for each font
				 * from the beginning of the file */
  عمومی:
  DEFINE_SIZE_ARRAY (12, table);
};

ساختار TTCHeader
{
  friend ساختار OpenTypeFontFile;

  خصوصی:

  unsigned صحیح get_face_شمار () ثابت
  {
    انتخاب (u.header.version.major) {
    حالت 2: /* version 2 is compatible with version 1 */
    حالت 1: hb_barrier (); بازگشت u.version1.get_face_شمار ();
    پیشفرض:بازگشت 0;
    }
  }
  ثابت OpenTypeFontFace& get_face (unsigned صحیح i) ثابت
  {
    انتخاب (u.header.version.major) {
    حالت 2: /* version 2 is compatible with version 1 */
    حالت 1: hb_barrier (); بازگشت u.version1.get_face (i);
    پیشفرض:بازگشت Null (OpenTypeFontFace);
    }
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    اگر (unlikely (!u.header.version.sanitize (c))) return_trace (نادرست);
    hb_barrier ();
    انتخاب (u.header.version.major) {
    حالت 2: /* version 2 is compatible with version 1 */
    حالت 1: hb_barrier (); return_trace (u.version1.sanitize (c));
    پیشفرض:return_trace (درست);
    }
  }

  محافظتشده:
  اجتماع {
  ساختار {
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

ساختار ResourceRecord
{
  ثابت OpenTypeFontFace & get_face (ثابت تهی *data_base) ثابت
  { بازگشت * reinterpret_cast<ثابت OpenTypeFontFace *> ((data_base+offset).arrayZ); }

  منطقی sanitize (hb_sanitize_context_t *c,
		 ثابت تهی *data_base) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) &&
		  offset.sanitize (c, data_base) &&
		  hb_barrier () &&
		  get_face (data_base).sanitize (c));
  }

  محافظتشده:
  HBUINT16	id;		/* Resource ID. */
  HBINT16	nameOffset;	/* Offset from beginning of resource name list
				 * to resource name, -1 means there is none. */
  HBUINT8	attrs;		/* Resource attributes */
  NNOffset24To<Array32Of<HBUINT8>>
		offset;		/* Offset from beginning of data block to
				 * data for this resource */
  HBUINT32	reserved;	/* Reserved for handle to resource */
  عمومی:
  DEFINE_SIZE_STATIC (12);
};

#define HB_TAG_sfnt HB_TAG ('s','f','n','t')

ساختار ResourceTypeRecord
{
  unsigned صحیح get_resource_شمار () ثابت
  { بازگشت tag == HB_TAG_sfnt ? res_شمار_M1 + 1 : 0; }

  منطقی is_sfnt () ثابت { بازگشت tag == HB_TAG_sfnt; }

  ثابت ResourceRecord& get_resource_record (unsigned صحیح i,
					     ثابت تهی *type_base) ثابت
  { بازگشت (type_base+resourcesZ).as_array (get_resource_شمار ())[i]; }

  منطقی sanitize (hb_sanitize_context_t *c,
		 ثابت تهی *type_base,
		 ثابت تهی *data_base) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) &&
		  hb_barrier () &&
		  resourcesZ.sanitize (c, type_base,
				       get_resource_شمار (),
				       data_base));
  }

  محافظتشده:
  Tag		tag;		/* Resource type. */
  HBUINT16	res_شمار_M1;	/* Number of resources minus 1. */
  NNOffset16To<UnsizedArrayOf<ResourceRecord>>
		resourcesZ;	/* Offset from beginning of resource type list
				 * to reference item list for this type. */
  عمومی:
  DEFINE_SIZE_STATIC (8);
};

ساختار ResourceMap
{
  unsigned صحیح get_face_شمار () ثابت
  {
    unsigned صحیح شمار = get_type_شمار ();
    برای (unsigned صحیح i = 0; i < شمار; i++)
    {
      ثابت ResourceTypeRecord& type = get_type_record (i);
      اگر (type.is_sfnt ())
	بازگشت type.get_resource_شمار ();
    }
    بازگشت 0;
  }

  ثابت OpenTypeFontFace& get_face (unsigned صحیح idx,
				    ثابت تهی *data_base) ثابت
  {
    unsigned صحیح شمار = get_type_شمار ();
    برای (unsigned صحیح i = 0; i < شمار; i++)
    {
      ثابت ResourceTypeRecord& type = get_type_record (i);
      /* The check for idx < count is here because ResourceRecord is NOT null-safe.
       * Because an offset of 0 there does NOT mean null. */
      اگر (type.is_sfnt () && idx < type.get_resource_شمار ())
	بازگشت type.get_resource_record (idx, &(این+typeList)).get_face (data_base);
    }
    بازگشت Null (OpenTypeFontFace);
  }

  منطقی sanitize (hb_sanitize_context_t *c, ثابت تهی *data_base) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) &&
		  hb_barrier () &&
		  typeList.sanitize (c, این,
				     &(این+typeList),
				     data_base));
  }

  خصوصی:
  unsigned صحیح get_type_شمار () ثابت { بازگشت (این+typeList).lenM1 + 1; }

  ثابت ResourceTypeRecord& get_type_record (unsigned صحیح i) ثابت
  { بازگشت (این+typeList)[i]; }

  محافظتشده:
  HBUINT8	reserved0[16];	/* Reserved for copy of resource header */
  HBUINT32	reserved1;	/* Reserved for handle to next resource map */
  HBUINT16	resreved2;	/* Reserved for file reference number */
  HBUINT16	attrs;		/* Resource fork attribute */
  NNOffset16To<ArrayOfM1<ResourceTypeRecord>>
		typeList;	/* Offset from beginning of map to
				 * resource type list */
  Offset16	nameList;	/* Offset from beginning of map to
				 * resource name list */
  عمومی:
  DEFINE_SIZE_STATIC (28);
};

ساختار ResourceForkHeader
{
  unsigned صحیح get_face_شمار () ثابت
  { بازگشت (این+map).get_face_شمار (); }

  ثابت OpenTypeFontFace& get_face (unsigned صحیح idx,
				    unsigned صحیح *base_offset = nullptr) ثابت
  {
    ثابت OpenTypeFontFace &face = (این+map).get_face (idx, &(این+data));
    اگر (base_offset)
      *base_offset = (ثابت نویسه *) &face - (ثابت نویسه *) این;
    بازگشت face;
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) &&
		  hb_barrier () &&
		  data.sanitize (c, این, dataLen) &&
		  map.sanitize (c, این, &(این+data)));
  }

  محافظتشده:
  NNOffset32To<UnsizedArrayOf<HBUINT8>>
		data;		/* Offset from beginning of resource fork
				 * to resource data */
  NNOffset32To<ResourceMap >
		map;		/* Offset from beginning of resource fork
				 * to resource map */
  HBUINT32	dataLen;	/* Length of resource data */
  HBUINT32	mapLen;		/* Length of resource map */
  عمومی:
  DEFINE_SIZE_STATIC (16);
};

/*
 * OpenType Font File
 */

ساختار OpenTypeFontFile
{
  شمارش {
    CFFTag		= HB_TAG ('O','T','T','O'), /* OpenType with Postscript outlines */
    TrueTypeTag		= HB_TAG ( 0 , 1 , 0 , 0 ), /* OpenType with TrueType outlines */
    TTCTag		= HB_TAG ('t','t','c','f'), /* TrueType Collection */
    DFontTag		= HB_TAG ( 0 , 0 , 1 , 0 ), /* DFont Mac Resource Fork */
    TrueTag		= HB_TAG ('t','r','u','e'), /* Obsolete Apple TrueType */
    Typ1Tag		= HB_TAG ('t','y','p','1')  /* Obsolete Apple Type1 font in SFNT container */
  };

  hb_tag_t get_tag () ثابت { بازگشت u.tag.v; }

  unsigned صحیح get_face_شمار () ثابت
  {
    انتخاب (u.tag.v) {
    حالت CFFTag:	/* All the non-collection tags */
    حالت TrueTag:
    حالت Typ1Tag:
    حالت TrueTypeTag:	بازگشت 1;
    حالت TTCTag:	hb_barrier (); بازگشت u.ttcHeader.get_face_شمار ();
    حالت DFontTag:	hb_barrier (); بازگشت u.rfHeader.get_face_شمار ();
    پیشفرض:		بازگشت 0;
    }
  }
  ثابت OpenTypeFontFace& get_face (unsigned صحیح i, unsigned صحیح *base_offset = nullptr) ثابت
  {
    اگر (base_offset)
      *base_offset = 0;
    انتخاب (u.tag.v) {
    /* Note: for non-collection SFNT data we ignore index.  This is because
     * Apple dfont container is a container of SFNT's.  So each SFNT is a
     * non-TTC, but the index is more than zero. */
    حالت CFFTag:	/* All the non-collection tags */
    حالت TrueTag:
    حالت Typ1Tag:
    حالت TrueTypeTag:	hb_barrier (); بازگشت u.fontFace;
    حالت TTCTag:	hb_barrier (); بازگشت u.ttcHeader.get_face (i);
    حالت DFontTag:	hb_barrier (); بازگشت u.rfHeader.get_face (i, base_offset);
    پیشفرض:		بازگشت Null (OpenTypeFontFace);
    }
  }

  template <typename Iterator,
	    hb_requires ((hb_is_source_of<Iterator, hb_pair_t<hb_tag_t, hb_blob_t *>>::value))>
  منطقی serialize_single (hb_serialize_context_t *c,
			 hb_tag_t sfnt_tag,
			 Iterator اقلام)
  {
    TRACE_SERIALIZE (این);
    أكد (sfnt_tag != TTCTag);
    اگر (unlikely (!c->extend_min (این))) return_trace (نادرست);
    return_trace (u.fontFace.serialize (c, sfnt_tag, اقلام));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    اگر (unlikely (!u.tag.v.sanitize (c))) return_trace (نادرست);
    hb_barrier ();
    انتخاب (u.tag.v) {
    حالت CFFTag:	/* All the non-collection tags */
    حالت TrueTag:
    حالت Typ1Tag:
    حالت TrueTypeTag:	hb_barrier (); return_trace (u.fontFace.sanitize (c));
    حالت TTCTag:	hb_barrier (); return_trace (u.ttcHeader.sanitize (c));
    حالت DFontTag:	hb_barrier (); return_trace (u.rfHeader.sanitize (c));
    پیشفرض:		return_trace (درست);
    }
  }

  محافظتشده:
  اجتماع {
  ساختار { Tag v; }	tag;		/* 4-byte identifier. */
  OpenTypeFontFace	fontFace;
  TTCHeader		ttcHeader;
  ResourceForkHeader	rfHeader;
  } u;
  عمومی:
  DEFINE_SIZE_UNION (4, tag.v);
};


} /* namespace OT */


#endif /* HB_OPEN_FILE_HH */
