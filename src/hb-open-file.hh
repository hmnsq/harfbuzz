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

#ضمن "hb-open-type.hh"
#ضمن "hb-ot-head-table.hh"


نطاق OT {

/*
 *
 * The OpenType Font File
 *
 */


/*
 * Organization of an OpenType Font
 */

بنية OpenTypeFontFile;
بنية OpenTypeOffsetTable;
بنية TTCHeader;


تعريفنوع بنية TableRecord
{
  صحيح cmp (Tag t) ثابت { أرجع -t.cmp (tag); }

  HB_INTERNAL ساكن صحيح cmp (ثابت فراغ *pa, ثابت فراغ *pb)
  {
    ثابت TableRecord *a = (ثابت TableRecord *) pa;
    ثابت TableRecord *b = (ثابت TableRecord *) pb;
    أرجع b->cmp (a->tag);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  Tag		tag;		/* 4-byte identifier. */
  CheckSum	checkSum;	/* CheckSum for this table. */
  Offset32	offset;		/* Offset from beginning of TrueType font
				 * file. */
  HBUINT32	length;		/* Length of this table. */
  عام:
  DEFINE_SIZE_STATIC (16);
} OpenTypeTable;

تعريفنوع بنية OpenTypeOffsetTable
{
  friend بنية OpenTypeFontFile;

  unsigned صحيح get_table_ژماردن () ثابت { أرجع tables.len; }
  ثابت TableRecord& get_table (unsigned صحيح i) ثابت
  { أرجع tables[i]; }
  unsigned صحيح get_table_tags (unsigned صحيح  start_offset,
			       unsigned صحيح *table_ژماردن_2, /* IN/OUT */
			       hb_tag_t     *table_tags /* OUT */) ثابت
  {
    إذا (table_ژماردن_2 && table_tags)
    {
      + tables.as_array ().sub_array (start_offset, table_ژماردن_2)
      | hb_map (&TableRecord::tag)
      | hb_sink (hb_array (table_tags, *table_ژماردن_2))
      ;
    }
    أرجع tables.len;
  }
  منطقي find_table_index (hb_tag_t tag, unsigned صحيح *table_index) ثابت
  {
    Tag t;
    t = tag;
    /* Use lfind for small fonts; there are fonts that have unsorted table entries;
     * those tend to work in other tools, so tolerate them.
     * https://github.com/harfbuzz/harfbuzz/issues/3065 */
    إذا (tables.len < 16)
      أرجع tables.lfind (t, table_index, HB_NOT_FOUND_STORE, Index::NOT_FOUND_INDEX);
    وإلا
      أرجع tables.bfind (t, table_index, HB_NOT_FOUND_STORE, Index::NOT_FOUND_INDEX);
  }
  ثابت TableRecord& get_table_by_tag (hb_tag_t tag) ثابت
  {
    unsigned صحيح table_index;
    find_table_index (tag, &table_index);
    أرجع get_table (table_index);
  }

  عام:

  template <typename Iterator,
	    hb_requires ((hb_is_source_of<Iterator, hb_pair_t<hb_tag_t, hb_blob_t *>>::value))>
  منطقي serialize (hb_serialize_context_t *c,
		  hb_tag_t sfnt_tag,
		  Iterator it)
  {
    TRACE_SERIALIZE (هذا);
    /* Alloc 12 for the OTHeader. */
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);
    /* Write sfntVersion (bytes 0..3). */
    sfnt_version = sfnt_tag;
    /* Take space for numTables, searchRange, entrySelector, RangeShift
     * and the TableRecords themselves.  */
    unsigned ژمارە_کاڵاکان = hb_len (it);
    إذا (unlikely (!tables.serialize (c, ژمارە_کاڵاکان))) return_trace (خطأ);

    ثابت حرف *dir_end = (ثابت حرف *) c->head;
    HBUINT32 *checksum_adjustment = nullptr;

    /* Write OffsetTables, alloc for and write actual table blobs. */
    unsigned i = 0;
    لكل (hb_pair_t<hb_tag_t, hb_blob_t*> entry : it)
    {
      hb_blob_t *blob = entry.second;
      unsigned len = blob->length;

      /* Allocate room for the table and copy it. */
      حرف *start = (حرف *) c->allocate_size<فراغ> (len, خطأ);
      إذا (unlikely (!start)) أرجع خطأ;

      TableRecord &rec = tables.arrayZ[i];
      rec.tag = entry.first;
      rec.length = len;
      rec.offset = 0;
      إذا (unlikely (!c->check_assign (rec.offset,
				      (unsigned) ((حرف *) start - (حرف *) هذا),
				      HB_SERIALIZE_ERROR_OFFSET_OVERFLOW)))
        return_trace (خطأ);

      إذا (likely (len))
	hb_memcpy (start, blob->data, len);

      /* 4-byte alignment. */
      c->align (4);
      ثابت حرف *end = (ثابت حرف *) c->head;

      إذا (entry.first == HB_OT_TAG_head &&
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

    إذا (checksum_adjustment)
    {
      CheckSum checksum;

      /* The following line is a slower version of the following block. */
      //checksum.set_for_data (this, (const char *) c->head - (const char *) this);
      checksum.set_for_data (هذا, dir_end - (ثابت حرف *) هذا);
      لكل (unsigned صحيح i = 0; i < ژمارە_کاڵاکان; i++)
      {
	TableRecord &rec = tables.arrayZ[i];
	checksum = checksum + rec.checkSum;
      }

      *checksum_adjustment = 0xB1B0AFBAu - checksum;
    }

    return_trace (true);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && tables.sanitize (c));
  }

  محمي:
  Tag		sfnt_version;	/* '\0\001\0\00' if TrueType / 'OTTO' if CFF */
  Bin_گەڕان_Array_Of<TableRecord>
		tables;
  عام:
  DEFINE_SIZE_ARRAY (12, tables);
} OpenTypeFontFace;


/*
 * TrueType Collections
 */

بنية TTCHeaderVersion1
{
  friend بنية TTCHeader;

  using DirectoryList = Array32Of<Offset32To<OpenTypeOffsetTable>>;

  unsigned صحيح get_face_ژماردن () ثابت { أرجع get_directory_list ().len; }
  ثابت OpenTypeFontFace& get_face (unsigned صحيح i) ثابت
  { أرجع هذا+get_directory_list ()[i]; }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
#ifndef HB_NO_BEYOND_64K
    إذا (version.minor >= 1)
    {
      إذا (unlikely (!table.sanitize_shallow (c)))
	return_trace (خطأ);
      unsigned extra_size = version.major == 2 ? 12 : 0;
      إذا (unlikely (!c->check_range (&table, table.get_size () + extra_size)))
	return_trace (خطأ);
      ثابت DirectoryList &table2 = StructAtOffset<DirectoryList> (&table,
								   table.get_size () + extra_size);
      return_trace (table2.sanitize (c, هذا));
    }
#endif
    return_trace (table.sanitize (c, هذا));
  }

  خاص:
  ثابت DirectoryList& get_directory_list () ثابت
  {
#ifndef HB_NO_BEYOND_64K
    إذا (version.minor >= 1)
    {
      unsigned extra_size = version.major == 2 ? 12 : 0;
      أرجع StructAtOffset<DirectoryList> (&table, table.get_size () + extra_size);
    }
#endif
    أرجع table;
  }

  محمي:
  Tag		ttcTag;		/* TrueType Collection ID string: 'ttcf' */
  FixedVersion<>version;	/* Version of the TTC Header. */
  DirectoryList
		table;		/* Array of offsets to the OffsetTable for each font
				 * from the beginning of the file */
  عام:
  DEFINE_SIZE_ARRAY (12, table);
};

بنية TTCHeader
{
  friend بنية OpenTypeFontFile;

  خاص:

  unsigned صحيح get_face_ژماردن () ثابت
  {
    بدل (u.header.version.major) {
    حالة 2: /* version 2 is compatible with version 1 */
    حالة 1: hb_barrier (); أرجع u.version1.get_face_ژماردن ();
    افتراضي:أرجع 0;
    }
  }
  ثابت OpenTypeFontFace& get_face (unsigned صحيح i) ثابت
  {
    بدل (u.header.version.major) {
    حالة 2: /* version 2 is compatible with version 1 */
    حالة 1: hb_barrier (); أرجع u.version1.get_face (i);
    افتراضي:أرجع Null (OpenTypeFontFace);
    }
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!u.header.version.sanitize (c))) return_trace (خطأ);
    hb_barrier ();
    بدل (u.header.version.major) {
    حالة 2: /* version 2 is compatible with version 1 */
    حالة 1: hb_barrier (); return_trace (u.version1.sanitize (c));
    افتراضي:return_trace (true);
    }
  }

  محمي:
  اتحاد {
  بنية {
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

بنية ResourceRecord
{
  ثابت OpenTypeFontFace & get_face (ثابت فراغ *data_base) ثابت
  { أرجع * reinterpret_cast<ثابت OpenTypeFontFace *> ((data_base+offset).arrayZ); }

  منطقي sanitize (hb_sanitize_context_t *c,
		 ثابت فراغ *data_base) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  offset.sanitize (c, data_base) &&
		  hb_barrier () &&
		  get_face (data_base).sanitize (c));
  }

  محمي:
  HBUINT16	id;		/* Resource ID. */
  HBINT16	nameOffset;	/* Offset from beginning of resource name list
				 * to resource name, -1 means there is none. */
  HBUINT8	attrs;		/* Resource attributes */
  NNOffset24To<Array32Of<HBUINT8>>
		offset;		/* Offset from beginning of data block to
				 * data for this resource */
  HBUINT32	reserved;	/* Reserved for handle to resource */
  عام:
  DEFINE_SIZE_STATIC (12);
};

#define HB_TAG_sfnt HB_TAG ('s','f','n','t')

بنية ResourceTypeRecord
{
  unsigned صحيح get_resource_ژماردن () ثابت
  { أرجع tag == HB_TAG_sfnt ? res_ژماردن_M1 + 1 : 0; }

  منطقي is_sfnt () ثابت { أرجع tag == HB_TAG_sfnt; }

  ثابت ResourceRecord& get_resource_record (unsigned صحيح i,
					     ثابت فراغ *type_base) ثابت
  { أرجع (type_base+resourcesZ).as_array (get_resource_ژماردن ())[i]; }

  منطقي sanitize (hb_sanitize_context_t *c,
		 ثابت فراغ *type_base,
		 ثابت فراغ *data_base) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  resourcesZ.sanitize (c, type_base,
				       get_resource_ژماردن (),
				       data_base));
  }

  محمي:
  Tag		tag;		/* Resource type. */
  HBUINT16	res_ژماردن_M1;	/* Number of resources minus 1. */
  NNOffset16To<UnsizedArrayOf<ResourceRecord>>
		resourcesZ;	/* Offset from beginning of resource type list
				 * to reference item list for this type. */
  عام:
  DEFINE_SIZE_STATIC (8);
};

بنية ResourceMap
{
  unsigned صحيح get_face_ژماردن () ثابت
  {
    unsigned صحيح ژماردن = get_type_ژماردن ();
    لكل (unsigned صحيح i = 0; i < ژماردن; i++)
    {
      ثابت ResourceTypeRecord& type = get_type_record (i);
      إذا (type.is_sfnt ())
	أرجع type.get_resource_ژماردن ();
    }
    أرجع 0;
  }

  ثابت OpenTypeFontFace& get_face (unsigned صحيح idx,
				    ثابت فراغ *data_base) ثابت
  {
    unsigned صحيح ژماردن = get_type_ژماردن ();
    لكل (unsigned صحيح i = 0; i < ژماردن; i++)
    {
      ثابت ResourceTypeRecord& type = get_type_record (i);
      /* The check for idx < count is here because ResourceRecord is NOT null-safe.
       * Because an offset of 0 there does NOT mean null. */
      إذا (type.is_sfnt () && idx < type.get_resource_ژماردن ())
	أرجع type.get_resource_record (idx, &(هذا+typeList)).get_face (data_base);
    }
    أرجع Null (OpenTypeFontFace);
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *data_base) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  typeList.sanitize (c, هذا,
				     &(هذا+typeList),
				     data_base));
  }

  خاص:
  unsigned صحيح get_type_ژماردن () ثابت { أرجع (هذا+typeList).lenM1 + 1; }

  ثابت ResourceTypeRecord& get_type_record (unsigned صحيح i) ثابت
  { أرجع (هذا+typeList)[i]; }

  محمي:
  HBUINT8	reserved0[16];	/* Reserved for copy of resource header */
  HBUINT32	reserved1;	/* Reserved for handle to next resource map */
  HBUINT16	resreved2;	/* Reserved for file reference number */
  HBUINT16	attrs;		/* Resource fork attribute */
  NNOffset16To<ArrayOfM1<ResourceTypeRecord>>
		typeList;	/* Offset from beginning of map to
				 * resource type list */
  Offset16	nameList;	/* Offset from beginning of map to
				 * resource name list */
  عام:
  DEFINE_SIZE_STATIC (28);
};

بنية ResourceForkHeader
{
  unsigned صحيح get_face_ژماردن () ثابت
  { أرجع (هذا+map).get_face_ژماردن (); }

  ثابت OpenTypeFontFace& get_face (unsigned صحيح idx,
				    unsigned صحيح *base_offset = nullptr) ثابت
  {
    ثابت OpenTypeFontFace &face = (هذا+map).get_face (idx, &(هذا+data));
    إذا (base_offset)
      *base_offset = (ثابت حرف *) &face - (ثابت حرف *) هذا;
    أرجع face;
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  data.sanitize (c, هذا, dataLen) &&
		  map.sanitize (c, هذا, &(هذا+data)));
  }

  محمي:
  NNOffset32To<UnsizedArrayOf<HBUINT8>>
		data;		/* Offset from beginning of resource fork
				 * to resource data */
  NNOffset32To<ResourceMap >
		map;		/* Offset from beginning of resource fork
				 * to resource map */
  HBUINT32	dataLen;	/* Length of resource data */
  HBUINT32	mapLen;		/* Length of resource map */
  عام:
  DEFINE_SIZE_STATIC (16);
};

/*
 * OpenType Font File
 */

بنية OpenTypeFontFile
{
  تعداد {
    CFFTag		= HB_TAG ('O','T','T','O'), /* OpenType with Postscript outlines */
    TrueTypeTag		= HB_TAG ( 0 , 1 , 0 , 0 ), /* OpenType with TrueType outlines */
    TTCTag		= HB_TAG ('t','t','c','f'), /* TrueType Collection */
    DFontTag		= HB_TAG ( 0 , 0 , 1 , 0 ), /* DFont Mac Resource Fork */
    TrueTag		= HB_TAG ('t','r','u','e'), /* Obsolete Apple TrueType */
    Typ1Tag		= HB_TAG ('t','y','p','1')  /* Obsolete Apple Type1 font in SFNT container */
  };

  hb_tag_t get_tag () ثابت { أرجع u.tag.v; }

  unsigned صحيح get_face_ژماردن () ثابت
  {
    بدل (u.tag.v) {
    حالة CFFTag:	/* All the non-collection tags */
    حالة TrueTag:
    حالة Typ1Tag:
    حالة TrueTypeTag:	أرجع 1;
    حالة TTCTag:	hb_barrier (); أرجع u.ttcHeader.get_face_ژماردن ();
    حالة DFontTag:	hb_barrier (); أرجع u.rfHeader.get_face_ژماردن ();
    افتراضي:		أرجع 0;
    }
  }
  ثابت OpenTypeFontFace& get_face (unsigned صحيح i, unsigned صحيح *base_offset = nullptr) ثابت
  {
    إذا (base_offset)
      *base_offset = 0;
    بدل (u.tag.v) {
    /* Note: for non-collection SFNT data we ignore index.  This is because
     * Apple dfont container is a container of SFNT's.  So each SFNT is a
     * non-TTC, but the index is more than zero. */
    حالة CFFTag:	/* All the non-collection tags */
    حالة TrueTag:
    حالة Typ1Tag:
    حالة TrueTypeTag:	hb_barrier (); أرجع u.fontFace;
    حالة TTCTag:	hb_barrier (); أرجع u.ttcHeader.get_face (i);
    حالة DFontTag:	hb_barrier (); أرجع u.rfHeader.get_face (i, base_offset);
    افتراضي:		أرجع Null (OpenTypeFontFace);
    }
  }

  template <typename Iterator,
	    hb_requires ((hb_is_source_of<Iterator, hb_pair_t<hb_tag_t, hb_blob_t *>>::value))>
  منطقي serialize_single (hb_serialize_context_t *c,
			 hb_tag_t sfnt_tag,
			 Iterator کاڵاکان)
  {
    TRACE_SERIALIZE (هذا);
    أكد (sfnt_tag != TTCTag);
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);
    return_trace (u.fontFace.serialize (c, sfnt_tag, کاڵاکان));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!u.tag.v.sanitize (c))) return_trace (خطأ);
    hb_barrier ();
    بدل (u.tag.v) {
    حالة CFFTag:	/* All the non-collection tags */
    حالة TrueTag:
    حالة Typ1Tag:
    حالة TrueTypeTag:	hb_barrier (); return_trace (u.fontFace.sanitize (c));
    حالة TTCTag:	hb_barrier (); return_trace (u.ttcHeader.sanitize (c));
    حالة DFontTag:	hb_barrier (); return_trace (u.rfHeader.sanitize (c));
    افتراضي:		return_trace (true);
    }
  }

  محمي:
  اتحاد {
  بنية { Tag v; }	tag;		/* 4-byte identifier. */
  OpenTypeFontFace	fontFace;
  TTCHeader		ttcHeader;
  ResourceForkHeader	rfHeader;
  } u;
  عام:
  DEFINE_SIZE_UNION (4, tag.v);
};


} /* namespace OT */


#endif /* HB_OPEN_FILE_HH */
