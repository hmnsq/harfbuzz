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

#ଅନ୍ତର୍ଭାଵ "hb-open-type.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-head-table.hh"


ନାମାକାଶ OT {

/*
 *
 * The OpenType Font File
 *
 */


/*
 * Organization of an OpenType Font
 */

ସଂରଚନା OpenTypeFontFile;
ସଂରଚନା OpenTypeOffsetTable;
ସଂରଚନା TTCHeader;


ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା TableRecord
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp (Tag t) ଅଚଳ { ଫେରନ୍ତୁ -t.cmp (tag); }

  HB_INTERNAL ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp (ଅଚଳ ଶୂନ୍ୟ *pa, ଅଚଳ ଶୂନ୍ୟ *pb)
  {
    ଅଚଳ TableRecord *a = (ଅଚଳ TableRecord *) pa;
    ଅଚଳ TableRecord *b = (ଅଚଳ TableRecord *) pb;
    ଫେରନ୍ତୁ b->cmp (a->tag);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି));
  }

  Tag		tag;		/* 4-byte identifier. */
  CheckSum	checkSum;	/* CheckSum for this table. */
  Offset32	offset;		/* Offset from beginning of TrueType font
				 * file. */
  HBUINT32	length;		/* Length of this table. */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (16);
} OpenTypeTable;

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା OpenTypeOffsetTable
{
  friend ସଂରଚନା OpenTypeFontFile;

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_table_ଗଣନା () ଅଚଳ { ଫେରନ୍ତୁ tables.len; }
  ଅଚଳ TableRecord& get_table (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i) ଅଚଳ
  { ଫେରନ୍ତୁ tables[i]; }
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_table_tags (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା  start_offset,
			       unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *table_ଗଣନା_2, /* IN/OUT */
			       hb_tag_t     *table_tags /* OUT */) ଅଚଳ
  {
    ଯଦି (table_ଗଣନା_2 && table_tags)
    {
      + tables.as_array ().sub_array (start_offset, table_ଗଣନା_2)
      | hb_map (&TableRecord::tag)
      | hb_sink (hb_array (table_tags, *table_ଗଣନା_2))
      ;
    }
    ଫେରନ୍ତୁ tables.len;
  }
  ତାର୍କିକ find_table_index (hb_tag_t tag, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *table_index) ଅଚଳ
  {
    Tag t;
    t = tag;
    /* Use lfind for small fonts; there are fonts that have unsorted table entries;
     * those tend to work in other tools, so tolerate them.
     * https://github.com/harfbuzz/harfbuzz/issues/3065 */
    ଯଦି (tables.len < 16)
      ଫେରନ୍ତୁ tables.lfind (t, table_index, HB_NOT_FOUND_STORE, Index::NOT_FOUND_INDEX);
    ଅନ୍ୟଥା
      ଫେରନ୍ତୁ tables.bfind (t, table_index, HB_NOT_FOUND_STORE, Index::NOT_FOUND_INDEX);
  }
  ଅଚଳ TableRecord& get_table_by_tag (hb_tag_t tag) ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା table_index;
    find_table_index (tag, &table_index);
    ଫେରନ୍ତୁ get_table (table_index);
  }

  ସର୍ବସାଧାରଣ:

  template <typename Iterator,
	    hb_requires ((hb_is_source_of<Iterator, hb_pair_t<hb_tag_t, hb_blob_t *>>::value))>
  ତାର୍କିକ serialize (hb_serialize_context_t *c,
		  hb_tag_t sfnt_tag,
		  Iterator it)
  {
    TRACE_SERIALIZE (ଏହି);
    /* Alloc 12 for the OTHeader. */
    ଯଦି (unlikely (!c->extend_min (ଏହି))) return_trace (ମିଥ୍ୟା);
    /* Write sfntVersion (bytes 0..3). */
    sfnt_version = sfnt_tag;
    /* Take space for numTables, searchRange, entrySelector, RangeShift
     * and the TableRecords themselves.  */
    unsigned ଅଙ୍କ_ବସ୍ତୁଗୁଡ଼ିକ = hb_len (it);
    ଯଦି (unlikely (!tables.serialize (c, ଅଙ୍କ_ବସ୍ତୁଗୁଡ଼ିକ))) return_trace (ମିଥ୍ୟା);

    ଅଚଳ ଅକ୍ଷର *dir_end = (ଅଚଳ ଅକ୍ଷର *) c->head;
    HBUINT32 *checksum_adjustment = nullptr;

    /* Write OffsetTables, alloc for and write actual table blobs. */
    unsigned i = 0;
    ପାଇଁ (hb_pair_t<hb_tag_t, hb_blob_t*> entry : it)
    {
      hb_blob_t *blob = entry.second;
      unsigned len = blob->length;

      /* Allocate room for the table and copy it. */
      ଅକ୍ଷର *start = (ଅକ୍ଷର *) c->allocate_size<ଶୂନ୍ୟ> (len, ମିଥ୍ୟା);
      ଯଦି (unlikely (!start)) ଫେରନ୍ତୁ ମିଥ୍ୟା;

      TableRecord &rec = tables.arrayZ[i];
      rec.tag = entry.first;
      rec.length = len;
      rec.offset = 0;
      ଯଦି (unlikely (!c->check_assign (rec.offset,
				      (unsigned) ((ଅକ୍ଷର *) start - (ଅକ୍ଷର *) ଏହି),
				      HB_SERIALIZE_ERROR_OFFSET_OVERFLOW)))
        return_trace (ମିଥ୍ୟା);

      ଯଦି (likely (len))
	hb_memcpy (start, blob->data, len);

      /* 4-byte alignment. */
      c->align (4);
      ଅଚଳ ଅକ୍ଷର *end = (ଅଚଳ ଅକ୍ଷର *) c->head;

      ଯଦି (entry.first == HB_OT_TAG_head &&
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

    ଯଦି (checksum_adjustment)
    {
      CheckSum checksum;

      /* The following line is a slower version of the following block. */
      //checksum.set_for_data (this, (const char *) c->head - (const char *) this);
      checksum.set_for_data (ଏହି, dir_end - (ଅଚଳ ଅକ୍ଷର *) ଏହି);
      ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଅଙ୍କ_ବସ୍ତୁଗୁଡ଼ିକ; i++)
      {
	TableRecord &rec = tables.arrayZ[i];
	checksum = checksum + rec.checkSum;
      }

      *checksum_adjustment = 0xB1B0AFBAu - checksum;
    }

    return_trace (ସତ୍ୟ);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) && tables.sanitize (c));
  }

  ସୁରକ୍ଷିତ:
  Tag		sfnt_version;	/* '\0\001\0\00' if TrueType / 'OTTO' if CFF */
  Bin_ସନ୍ଧାନ_Array_Of<TableRecord>
		tables;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (12, tables);
} OpenTypeFontFace;


/*
 * TrueType Collections
 */

ସଂରଚନା TTCHeaderVersion1
{
  friend ସଂରଚନା TTCHeader;

  using DirectoryList = Array32Of<Offset32To<OpenTypeOffsetTable>>;

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_face_ଗଣନା () ଅଚଳ { ଫେରନ୍ତୁ get_directory_list ().len; }
  ଅଚଳ OpenTypeFontFace& get_face (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i) ଅଚଳ
  { ଫେରନ୍ତୁ ଏହି+get_directory_list ()[i]; }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
#ifndef HB_NO_BEYOND_64K
    ଯଦି (version.minor >= 1)
    {
      ଯଦି (unlikely (!table.sanitize_shallow (c)))
	return_trace (ମିଥ୍ୟା);
      unsigned extra_size = version.major == 2 ? 12 : 0;
      ଯଦି (unlikely (!c->check_range (&table, table.get_size () + extra_size)))
	return_trace (ମିଥ୍ୟା);
      ଅଚଳ DirectoryList &table2 = StructAtOffset<DirectoryList> (&table,
								   table.get_size () + extra_size);
      return_trace (table2.sanitize (c, ଏହି));
    }
#endif
    return_trace (table.sanitize (c, ଏହି));
  }

  ବ୍ୟକ୍ତିଗତ:
  ଅଚଳ DirectoryList& get_directory_list () ଅଚଳ
  {
#ifndef HB_NO_BEYOND_64K
    ଯଦି (version.minor >= 1)
    {
      unsigned extra_size = version.major == 2 ? 12 : 0;
      ଫେରନ୍ତୁ StructAtOffset<DirectoryList> (&table, table.get_size () + extra_size);
    }
#endif
    ଫେରନ୍ତୁ table;
  }

  ସୁରକ୍ଷିତ:
  Tag		ttcTag;		/* TrueType Collection ID string: 'ttcf' */
  FixedVersion<>version;	/* Version of the TTC Header. */
  DirectoryList
		table;		/* Array of offsets to the OffsetTable for each font
				 * from the beginning of the file */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (12, table);
};

ସଂରଚନା TTCHeader
{
  friend ସଂରଚନା OpenTypeFontFile;

  ବ୍ୟକ୍ତିଗତ:

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_face_ଗଣନା () ଅଚଳ
  {
    ବାଛ (u.header.version.major) {
    ସ୍ଥିତି 2: /* version 2 is compatible with version 1 */
    ସ୍ଥିତି 1: hb_barrier (); ଫେରନ୍ତୁ u.version1.get_face_ଗଣନା ();
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:ଫେରନ୍ତୁ 0;
    }
  }
  ଅଚଳ OpenTypeFontFace& get_face (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i) ଅଚଳ
  {
    ବାଛ (u.header.version.major) {
    ସ୍ଥିତି 2: /* version 2 is compatible with version 1 */
    ସ୍ଥିତି 1: hb_barrier (); ଫେରନ୍ତୁ u.version1.get_face (i);
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:ଫେରନ୍ତୁ Null (OpenTypeFontFace);
    }
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    ଯଦି (unlikely (!u.header.version.sanitize (c))) return_trace (ମିଥ୍ୟା);
    hb_barrier ();
    ବାଛ (u.header.version.major) {
    ସ୍ଥିତି 2: /* version 2 is compatible with version 1 */
    ସ୍ଥିତି 1: hb_barrier (); return_trace (u.version1.sanitize (c));
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:return_trace (ସତ୍ୟ);
    }
  }

  ସୁରକ୍ଷିତ:
  ସଂଘ {
  ସଂରଚନା {
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

ସଂରଚନା ResourceRecord
{
  ଅଚଳ OpenTypeFontFace & get_face (ଅଚଳ ଶୂନ୍ୟ *data_base) ଅଚଳ
  { ଫେରନ୍ତୁ * reinterpret_cast<ଅଚଳ OpenTypeFontFace *> ((data_base+offset).arrayZ); }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c,
		 ଅଚଳ ଶୂନ୍ୟ *data_base) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) &&
		  offset.sanitize (c, data_base) &&
		  hb_barrier () &&
		  get_face (data_base).sanitize (c));
  }

  ସୁରକ୍ଷିତ:
  HBUINT16	id;		/* Resource ID. */
  HBINT16	nameOffset;	/* Offset from beginning of resource name list
				 * to resource name, -1 means there is none. */
  HBUINT8	attrs;		/* Resource attributes */
  NNOffset24To<Array32Of<HBUINT8>>
		offset;		/* Offset from beginning of data block to
				 * data for this resource */
  HBUINT32	reserved;	/* Reserved for handle to resource */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (12);
};

#define HB_TAG_sfnt HB_TAG ('s','f','n','t')

ସଂରଚନା ResourceTypeRecord
{
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_resource_ଗଣନା () ଅଚଳ
  { ଫେରନ୍ତୁ tag == HB_TAG_sfnt ? res_ଗଣନା_M1 + 1 : 0; }

  ତାର୍କିକ is_sfnt () ଅଚଳ { ଫେରନ୍ତୁ tag == HB_TAG_sfnt; }

  ଅଚଳ ResourceRecord& get_resource_record (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i,
					     ଅଚଳ ଶୂନ୍ୟ *type_base) ଅଚଳ
  { ଫେରନ୍ତୁ (type_base+resourcesZ).as_array (get_resource_ଗଣନା ())[i]; }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c,
		 ଅଚଳ ଶୂନ୍ୟ *type_base,
		 ଅଚଳ ଶୂନ୍ୟ *data_base) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) &&
		  hb_barrier () &&
		  resourcesZ.sanitize (c, type_base,
				       get_resource_ଗଣନା (),
				       data_base));
  }

  ସୁରକ୍ଷିତ:
  Tag		tag;		/* Resource type. */
  HBUINT16	res_ଗଣନା_M1;	/* Number of resources minus 1. */
  NNOffset16To<UnsizedArrayOf<ResourceRecord>>
		resourcesZ;	/* Offset from beginning of resource type list
				 * to reference item list for this type. */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (8);
};

ସଂରଚନା ResourceMap
{
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_face_ଗଣନା () ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = get_type_ଗଣନା ();
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
    {
      ଅଚଳ ResourceTypeRecord& type = get_type_record (i);
      ଯଦି (type.is_sfnt ())
	ଫେରନ୍ତୁ type.get_resource_ଗଣନା ();
    }
    ଫେରନ୍ତୁ 0;
  }

  ଅଚଳ OpenTypeFontFace& get_face (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା idx,
				    ଅଚଳ ଶୂନ୍ୟ *data_base) ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = get_type_ଗଣନା ();
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
    {
      ଅଚଳ ResourceTypeRecord& type = get_type_record (i);
      /* The check for idx < count is here because ResourceRecord is NOT null-safe.
       * Because an offset of 0 there does NOT mean null. */
      ଯଦି (type.is_sfnt () && idx < type.get_resource_ଗଣନା ())
	ଫେରନ୍ତୁ type.get_resource_record (idx, &(ଏହି+typeList)).get_face (data_base);
    }
    ଫେରନ୍ତୁ Null (OpenTypeFontFace);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c, ଅଚଳ ଶୂନ୍ୟ *data_base) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) &&
		  hb_barrier () &&
		  typeList.sanitize (c, ଏହି,
				     &(ଏହି+typeList),
				     data_base));
  }

  ବ୍ୟକ୍ତିଗତ:
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_type_ଗଣନା () ଅଚଳ { ଫେରନ୍ତୁ (ଏହି+typeList).lenM1 + 1; }

  ଅଚଳ ResourceTypeRecord& get_type_record (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i) ଅଚଳ
  { ଫେରନ୍ତୁ (ଏହି+typeList)[i]; }

  ସୁରକ୍ଷିତ:
  HBUINT8	reserved0[16];	/* Reserved for copy of resource header */
  HBUINT32	reserved1;	/* Reserved for handle to next resource map */
  HBUINT16	resreved2;	/* Reserved for file reference number */
  HBUINT16	attrs;		/* Resource fork attribute */
  NNOffset16To<ArrayOfM1<ResourceTypeRecord>>
		typeList;	/* Offset from beginning of map to
				 * resource type list */
  Offset16	nameList;	/* Offset from beginning of map to
				 * resource name list */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (28);
};

ସଂରଚନା ResourceForkHeader
{
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_face_ଗଣନା () ଅଚଳ
  { ଫେରନ୍ତୁ (ଏହି+map).get_face_ଗଣନା (); }

  ଅଚଳ OpenTypeFontFace& get_face (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା idx,
				    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *base_offset = nullptr) ଅଚଳ
  {
    ଅଚଳ OpenTypeFontFace &face = (ଏହି+map).get_face (idx, &(ଏହି+data));
    ଯଦି (base_offset)
      *base_offset = (ଅଚଳ ଅକ୍ଷର *) &face - (ଅଚଳ ଅକ୍ଷର *) ଏହି;
    ଫେରନ୍ତୁ face;
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) &&
		  hb_barrier () &&
		  data.sanitize (c, ଏହି, dataLen) &&
		  map.sanitize (c, ଏହି, &(ଏହି+data)));
  }

  ସୁରକ୍ଷିତ:
  NNOffset32To<UnsizedArrayOf<HBUINT8>>
		data;		/* Offset from beginning of resource fork
				 * to resource data */
  NNOffset32To<ResourceMap >
		map;		/* Offset from beginning of resource fork
				 * to resource map */
  HBUINT32	dataLen;	/* Length of resource data */
  HBUINT32	mapLen;		/* Length of resource map */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (16);
};

/*
 * OpenType Font File
 */

ସଂରଚନା OpenTypeFontFile
{
  ଗଣନା {
    CFFTag		= HB_TAG ('O','T','T','O'), /* OpenType with Postscript outlines */
    TrueTypeTag		= HB_TAG ( 0 , 1 , 0 , 0 ), /* OpenType with TrueType outlines */
    TTCTag		= HB_TAG ('t','t','c','f'), /* TrueType Collection */
    DFontTag		= HB_TAG ( 0 , 0 , 1 , 0 ), /* DFont Mac Resource Fork */
    TrueTag		= HB_TAG ('t','r','u','e'), /* Obsolete Apple TrueType */
    Typ1Tag		= HB_TAG ('t','y','p','1')  /* Obsolete Apple Type1 font in SFNT container */
  };

  hb_tag_t get_tag () ଅଚଳ { ଫେରନ୍ତୁ u.tag.v; }

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_face_ଗଣନା () ଅଚଳ
  {
    ବାଛ (u.tag.v) {
    ସ୍ଥିତି CFFTag:	/* All the non-collection tags */
    ସ୍ଥିତି TrueTag:
    ସ୍ଥିତି Typ1Tag:
    ସ୍ଥିତି TrueTypeTag:	ଫେରନ୍ତୁ 1;
    ସ୍ଥିତି TTCTag:	hb_barrier (); ଫେରନ୍ତୁ u.ttcHeader.get_face_ଗଣନା ();
    ସ୍ଥିତି DFontTag:	hb_barrier (); ଫେରନ୍ତୁ u.rfHeader.get_face_ଗଣନା ();
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:		ଫେରନ୍ତୁ 0;
    }
  }
  ଅଚଳ OpenTypeFontFace& get_face (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *base_offset = nullptr) ଅଚଳ
  {
    ଯଦି (base_offset)
      *base_offset = 0;
    ବାଛ (u.tag.v) {
    /* Note: for non-collection SFNT data we ignore index.  This is because
     * Apple dfont container is a container of SFNT's.  So each SFNT is a
     * non-TTC, but the index is more than zero. */
    ସ୍ଥିତି CFFTag:	/* All the non-collection tags */
    ସ୍ଥିତି TrueTag:
    ସ୍ଥିତି Typ1Tag:
    ସ୍ଥିତି TrueTypeTag:	hb_barrier (); ଫେରନ୍ତୁ u.fontFace;
    ସ୍ଥିତି TTCTag:	hb_barrier (); ଫେରନ୍ତୁ u.ttcHeader.get_face (i);
    ସ୍ଥିତି DFontTag:	hb_barrier (); ଫେରନ୍ତୁ u.rfHeader.get_face (i, base_offset);
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:		ଫେରନ୍ତୁ Null (OpenTypeFontFace);
    }
  }

  template <typename Iterator,
	    hb_requires ((hb_is_source_of<Iterator, hb_pair_t<hb_tag_t, hb_blob_t *>>::value))>
  ତାର୍କିକ serialize_single (hb_serialize_context_t *c,
			 hb_tag_t sfnt_tag,
			 Iterator ବସ୍ତୁଗୁଡ଼ିକ)
  {
    TRACE_SERIALIZE (ଏହି);
    ପ୍ରତିଜ୍ଞା (sfnt_tag != TTCTag);
    ଯଦି (unlikely (!c->extend_min (ଏହି))) return_trace (ମିଥ୍ୟା);
    return_trace (u.fontFace.serialize (c, sfnt_tag, ବସ୍ତୁଗୁଡ଼ିକ));
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    ଯଦି (unlikely (!u.tag.v.sanitize (c))) return_trace (ମିଥ୍ୟା);
    hb_barrier ();
    ବାଛ (u.tag.v) {
    ସ୍ଥିତି CFFTag:	/* All the non-collection tags */
    ସ୍ଥିତି TrueTag:
    ସ୍ଥିତି Typ1Tag:
    ସ୍ଥିତି TrueTypeTag:	hb_barrier (); return_trace (u.fontFace.sanitize (c));
    ସ୍ଥିତି TTCTag:	hb_barrier (); return_trace (u.ttcHeader.sanitize (c));
    ସ୍ଥିତି DFontTag:	hb_barrier (); return_trace (u.rfHeader.sanitize (c));
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:		return_trace (ସତ୍ୟ);
    }
  }

  ସୁରକ୍ଷିତ:
  ସଂଘ {
  ସଂରଚନା { Tag v; }	tag;		/* 4-byte identifier. */
  OpenTypeFontFace	fontFace;
  TTCHeader		ttcHeader;
  ResourceForkHeader	rfHeader;
  } u;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_UNION (4, tag.v);
};


} /* namespace OT */


#endif /* HB_OPEN_FILE_HH */
