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

#ಅನ್ತರ್ಭಾವ "hb-open-type.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-head-table.hh"


ನಾಮಾಕಾಶ OT {

/*
 *
 * The OpenType Font File
 *
 */


/*
 * Organization of an OpenType Font
 */

ಸಂರಚನಾ OpenTypeFontFile;
ಸಂರಚನಾ OpenTypeOffsetTable;
ಸಂರಚನಾ TTCHeader;


ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ TableRecord
{
  ಪೂರ್ಣಾಙ್ಕ cmp (Tag t) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ -t.cmp (tag); }

  HB_INTERNAL ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cmp (ಧ್ರುವ ಶೂನ್ಯ *pa, ಧ್ರುವ ಶೂನ್ಯ *pb)
  {
    ಧ್ರುವ TableRecord *a = (ಧ್ರುವ TableRecord *) pa;
    ಧ್ರುವ TableRecord *b = (ಧ್ರುವ TableRecord *) pb;
    ಪ್ರತ್ಯಾಗಮ b->cmp (a->tag);
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್));
  }

  Tag		tag;		/* 4-byte identifier. */
  CheckSum	checkSum;	/* CheckSum for this table. */
  Offset32	offset;		/* Offset from beginning of TrueType font
				 * file. */
  HBUINT32	length;		/* Length of this table. */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (16);
} OpenTypeTable;

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ OpenTypeOffsetTable
{
  friend ಸಂರಚನಾ OpenTypeFontFile;

  unsigned ಪೂರ್ಣಾಙ್ಕ get_table_ಎಣಿಕೆ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ tables.len; }
  ಧ್ರುವ TableRecord& get_table (unsigned ಪೂರ್ಣಾಙ್ಕ i) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ tables[i]; }
  unsigned ಪೂರ್ಣಾಙ್ಕ get_table_tags (unsigned ಪೂರ್ಣಾಙ್ಕ  start_offset,
			       unsigned ಪೂರ್ಣಾಙ್ಕ *table_ಎಣಿಕೆ_2, /* IN/OUT */
			       hb_tag_t     *table_tags /* OUT */) ಧ್ರುವ
  {
    ಯದಿ (table_ಎಣಿಕೆ_2 && table_tags)
    {
      + tables.as_array ().sub_array (start_offset, table_ಎಣಿಕೆ_2)
      | hb_map (&TableRecord::tag)
      | hb_sink (hb_array (table_tags, *table_ಎಣಿಕೆ_2))
      ;
    }
    ಪ್ರತ್ಯಾಗಮ tables.len;
  }
  ತಾರ್ಕಿಕ find_table_index (hb_tag_t tag, unsigned ಪೂರ್ಣಾಙ್ಕ *table_index) ಧ್ರುವ
  {
    Tag t;
    t = tag;
    /* Use lfind for small fonts; there are fonts that have unsorted table entries;
     * those tend to work in other tools, so tolerate them.
     * https://github.com/harfbuzz/harfbuzz/issues/3065 */
    ಯದಿ (tables.len < 16)
      ಪ್ರತ್ಯಾಗಮ tables.lfind (t, table_index, HB_NOT_FOUND_STORE, Index::NOT_FOUND_INDEX);
    ಅನ್ಯಥಾ
      ಪ್ರತ್ಯಾಗಮ tables.bfind (t, table_index, HB_NOT_FOUND_STORE, Index::NOT_FOUND_INDEX);
  }
  ಧ್ರುವ TableRecord& get_table_by_tag (hb_tag_t tag) ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ table_index;
    find_table_index (tag, &table_index);
    ಪ್ರತ್ಯಾಗಮ get_table (table_index);
  }

  ಸಾರ್ವಜನಿಕ:

  template <typename Iterator,
	    hb_requires ((hb_is_source_of<Iterator, hb_pair_t<hb_tag_t, hb_blob_t *>>::value))>
  ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c,
		  hb_tag_t sfnt_tag,
		  Iterator it)
  {
    TRACE_SERIALIZE (ಏತತ್);
    /* Alloc 12 for the OTHeader. */
    ಯದಿ (unlikely (!c->extend_min (ಏತತ್))) return_trace (ಅಸತ್ಯ);
    /* Write sfntVersion (bytes 0..3). */
    sfnt_version = sfnt_tag;
    /* Take space for numTables, searchRange, entrySelector, RangeShift
     * and the TableRecords themselves.  */
    unsigned ಅಂಕೆ_ವಸ್ತುಗಳು = hb_len (it);
    ಯದಿ (unlikely (!tables.serialize (c, ಅಂಕೆ_ವಸ್ತುಗಳು))) return_trace (ಅಸತ್ಯ);

    ಧ್ರುವ ವರ್ಣ *dir_end = (ಧ್ರುವ ವರ್ಣ *) c->head;
    HBUINT32 *checksum_adjustment = nullptr;

    /* Write OffsetTables, alloc for and write actual table blobs. */
    unsigned i = 0;
    ಪ್ರತಿ (hb_pair_t<hb_tag_t, hb_blob_t*> entry : it)
    {
      hb_blob_t *blob = entry.second;
      unsigned len = blob->length;

      /* Allocate room for the table and copy it. */
      ವರ್ಣ *start = (ವರ್ಣ *) c->allocate_size<ಶೂನ್ಯ> (len, ಅಸತ್ಯ);
      ಯದಿ (unlikely (!start)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

      TableRecord &rec = tables.arrayZ[i];
      rec.tag = entry.first;
      rec.length = len;
      rec.offset = 0;
      ಯದಿ (unlikely (!c->check_assign (rec.offset,
				      (unsigned) ((ವರ್ಣ *) start - (ವರ್ಣ *) ಏತತ್),
				      HB_SERIALIZE_ERROR_OFFSET_OVERFLOW)))
        return_trace (ಅಸತ್ಯ);

      ಯದಿ (likely (len))
	hb_memcpy (start, blob->data, len);

      /* 4-byte alignment. */
      c->align (4);
      ಧ್ರುವ ವರ್ಣ *end = (ಧ್ರುವ ವರ್ಣ *) c->head;

      ಯದಿ (entry.first == HB_OT_TAG_head &&
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

    ಯದಿ (checksum_adjustment)
    {
      CheckSum checksum;

      /* The following line is a slower version of the following block. */
      //checksum.set_for_data (this, (const char *) c->head - (const char *) this);
      checksum.set_for_data (ಏತತ್, dir_end - (ಧ್ರುವ ವರ್ಣ *) ಏತತ್);
      ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಅಂಕೆ_ವಸ್ತುಗಳು; i++)
      {
	TableRecord &rec = tables.arrayZ[i];
	checksum = checksum + rec.checkSum;
      }

      *checksum_adjustment = 0xB1B0AFBAu - checksum;
    }

    return_trace (ಸತ್ಯ);
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) && tables.sanitize (c));
  }

  ರಕ್ಷಿತ:
  Tag		sfnt_version;	/* '\0\001\0\00' if TrueType / 'OTTO' if CFF */
  Bin_ಹುಡುಕು_Array_Of<TableRecord>
		tables;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (12, tables);
} OpenTypeFontFace;


/*
 * TrueType Collections
 */

ಸಂರಚನಾ TTCHeaderVersion1
{
  friend ಸಂರಚನಾ TTCHeader;

  using DirectoryList = Array32Of<Offset32To<OpenTypeOffsetTable>>;

  unsigned ಪೂರ್ಣಾಙ್ಕ get_face_ಎಣಿಕೆ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ get_directory_list ().len; }
  ಧ್ರುವ OpenTypeFontFace& get_face (unsigned ಪೂರ್ಣಾಙ್ಕ i) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ ಏತತ್+get_directory_list ()[i]; }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
#ifndef HB_NO_BEYOND_64K
    ಯದಿ (version.minor >= 1)
    {
      ಯದಿ (unlikely (!table.sanitize_shallow (c)))
	return_trace (ಅಸತ್ಯ);
      unsigned extra_size = version.major == 2 ? 12 : 0;
      ಯದಿ (unlikely (!c->check_range (&table, table.get_size () + extra_size)))
	return_trace (ಅಸತ್ಯ);
      ಧ್ರುವ DirectoryList &table2 = StructAtOffset<DirectoryList> (&table,
								   table.get_size () + extra_size);
      return_trace (table2.sanitize (c, ಏತತ್));
    }
#endif
    return_trace (table.sanitize (c, ಏತತ್));
  }

  ಗುಪ್ತ:
  ಧ್ರುವ DirectoryList& get_directory_list () ಧ್ರುವ
  {
#ifndef HB_NO_BEYOND_64K
    ಯದಿ (version.minor >= 1)
    {
      unsigned extra_size = version.major == 2 ? 12 : 0;
      ಪ್ರತ್ಯಾಗಮ StructAtOffset<DirectoryList> (&table, table.get_size () + extra_size);
    }
#endif
    ಪ್ರತ್ಯಾಗಮ table;
  }

  ರಕ್ಷಿತ:
  Tag		ttcTag;		/* TrueType Collection ID string: 'ttcf' */
  FixedVersion<>version;	/* Version of the TTC Header. */
  DirectoryList
		table;		/* Array of offsets to the OffsetTable for each font
				 * from the beginning of the file */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (12, table);
};

ಸಂರಚನಾ TTCHeader
{
  friend ಸಂರಚನಾ OpenTypeFontFile;

  ಗುಪ್ತ:

  unsigned ಪೂರ್ಣಾಙ್ಕ get_face_ಎಣಿಕೆ () ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (u.header.version.major) {
    ಪಕ್ಷ 2: /* version 2 is compatible with version 1 */
    ಪಕ್ಷ 1: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.version1.get_face_ಎಣಿಕೆ ();
    ಸ್ವಭಾವ:ಪ್ರತ್ಯಾಗಮ 0;
    }
  }
  ಧ್ರುವ OpenTypeFontFace& get_face (unsigned ಪೂರ್ಣಾಙ್ಕ i) ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (u.header.version.major) {
    ಪಕ್ಷ 2: /* version 2 is compatible with version 1 */
    ಪಕ್ಷ 1: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.version1.get_face (i);
    ಸ್ವಭಾವ:ಪ್ರತ್ಯಾಗಮ Null (OpenTypeFontFace);
    }
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    ಯದಿ (unlikely (!u.header.version.sanitize (c))) return_trace (ಅಸತ್ಯ);
    hb_barrier ();
    ವಿಕಲ್ಪ (u.header.version.major) {
    ಪಕ್ಷ 2: /* version 2 is compatible with version 1 */
    ಪಕ್ಷ 1: hb_barrier (); return_trace (u.version1.sanitize (c));
    ಸ್ವಭಾವ:return_trace (ಸತ್ಯ);
    }
  }

  ರಕ್ಷಿತ:
  ಸಙ್ಘ {
  ಸಂರಚನಾ {
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

ಸಂರಚನಾ ResourceRecord
{
  ಧ್ರುವ OpenTypeFontFace & get_face (ಧ್ರುವ ಶೂನ್ಯ *data_base) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ * reinterpret_cast<ಧ್ರುವ OpenTypeFontFace *> ((data_base+offset).arrayZ); }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c,
		 ಧ್ರುವ ಶೂನ್ಯ *data_base) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) &&
		  offset.sanitize (c, data_base) &&
		  hb_barrier () &&
		  get_face (data_base).sanitize (c));
  }

  ರಕ್ಷಿತ:
  HBUINT16	id;		/* Resource ID. */
  HBINT16	nameOffset;	/* Offset from beginning of resource name list
				 * to resource name, -1 means there is none. */
  HBUINT8	attrs;		/* Resource attributes */
  NNOffset24To<Array32Of<HBUINT8>>
		offset;		/* Offset from beginning of data block to
				 * data for this resource */
  HBUINT32	reserved;	/* Reserved for handle to resource */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (12);
};

#define HB_TAG_sfnt HB_TAG ('s','f','n','t')

ಸಂರಚನಾ ResourceTypeRecord
{
  unsigned ಪೂರ್ಣಾಙ್ಕ get_resource_ಎಣಿಕೆ () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ tag == HB_TAG_sfnt ? res_ಎಣಿಕೆ_M1 + 1 : 0; }

  ತಾರ್ಕಿಕ is_sfnt () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ tag == HB_TAG_sfnt; }

  ಧ್ರುವ ResourceRecord& get_resource_record (unsigned ಪೂರ್ಣಾಙ್ಕ i,
					     ಧ್ರುವ ಶೂನ್ಯ *type_base) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ (type_base+resourcesZ).as_array (get_resource_ಎಣಿಕೆ ())[i]; }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c,
		 ಧ್ರುವ ಶೂನ್ಯ *type_base,
		 ಧ್ರುವ ಶೂನ್ಯ *data_base) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) &&
		  hb_barrier () &&
		  resourcesZ.sanitize (c, type_base,
				       get_resource_ಎಣಿಕೆ (),
				       data_base));
  }

  ರಕ್ಷಿತ:
  Tag		tag;		/* Resource type. */
  HBUINT16	res_ಎಣಿಕೆ_M1;	/* Number of resources minus 1. */
  NNOffset16To<UnsizedArrayOf<ResourceRecord>>
		resourcesZ;	/* Offset from beginning of resource type list
				 * to reference item list for this type. */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (8);
};

ಸಂರಚನಾ ResourceMap
{
  unsigned ಪೂರ್ಣಾಙ್ಕ get_face_ಎಣಿಕೆ () ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = get_type_ಎಣಿಕೆ ();
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
    {
      ಧ್ರುವ ResourceTypeRecord& type = get_type_record (i);
      ಯದಿ (type.is_sfnt ())
	ಪ್ರತ್ಯಾಗಮ type.get_resource_ಎಣಿಕೆ ();
    }
    ಪ್ರತ್ಯಾಗಮ 0;
  }

  ಧ್ರುವ OpenTypeFontFace& get_face (unsigned ಪೂರ್ಣಾಙ್ಕ idx,
				    ಧ್ರುವ ಶೂನ್ಯ *data_base) ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = get_type_ಎಣಿಕೆ ();
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
    {
      ಧ್ರುವ ResourceTypeRecord& type = get_type_record (i);
      /* The check for idx < count is here because ResourceRecord is NOT null-safe.
       * Because an offset of 0 there does NOT mean null. */
      ಯದಿ (type.is_sfnt () && idx < type.get_resource_ಎಣಿಕೆ ())
	ಪ್ರತ್ಯಾಗಮ type.get_resource_record (idx, &(ಏತತ್+typeList)).get_face (data_base);
    }
    ಪ್ರತ್ಯಾಗಮ Null (OpenTypeFontFace);
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c, ಧ್ರುವ ಶೂನ್ಯ *data_base) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) &&
		  hb_barrier () &&
		  typeList.sanitize (c, ಏತತ್,
				     &(ಏತತ್+typeList),
				     data_base));
  }

  ಗುಪ್ತ:
  unsigned ಪೂರ್ಣಾಙ್ಕ get_type_ಎಣಿಕೆ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ (ಏತತ್+typeList).lenM1 + 1; }

  ಧ್ರುವ ResourceTypeRecord& get_type_record (unsigned ಪೂರ್ಣಾಙ್ಕ i) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ (ಏತತ್+typeList)[i]; }

  ರಕ್ಷಿತ:
  HBUINT8	reserved0[16];	/* Reserved for copy of resource header */
  HBUINT32	reserved1;	/* Reserved for handle to next resource map */
  HBUINT16	resreved2;	/* Reserved for file reference number */
  HBUINT16	attrs;		/* Resource fork attribute */
  NNOffset16To<ArrayOfM1<ResourceTypeRecord>>
		typeList;	/* Offset from beginning of map to
				 * resource type list */
  Offset16	nameList;	/* Offset from beginning of map to
				 * resource name list */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (28);
};

ಸಂರಚನಾ ResourceForkHeader
{
  unsigned ಪೂರ್ಣಾಙ್ಕ get_face_ಎಣಿಕೆ () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ (ಏತತ್+map).get_face_ಎಣಿಕೆ (); }

  ಧ್ರುವ OpenTypeFontFace& get_face (unsigned ಪೂರ್ಣಾಙ್ಕ idx,
				    unsigned ಪೂರ್ಣಾಙ್ಕ *base_offset = nullptr) ಧ್ರುವ
  {
    ಧ್ರುವ OpenTypeFontFace &face = (ಏತತ್+map).get_face (idx, &(ಏತತ್+data));
    ಯದಿ (base_offset)
      *base_offset = (ಧ್ರುವ ವರ್ಣ *) &face - (ಧ್ರುವ ವರ್ಣ *) ಏತತ್;
    ಪ್ರತ್ಯಾಗಮ face;
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) &&
		  hb_barrier () &&
		  data.sanitize (c, ಏತತ್, dataLen) &&
		  map.sanitize (c, ಏತತ್, &(ಏತತ್+data)));
  }

  ರಕ್ಷಿತ:
  NNOffset32To<UnsizedArrayOf<HBUINT8>>
		data;		/* Offset from beginning of resource fork
				 * to resource data */
  NNOffset32To<ResourceMap >
		map;		/* Offset from beginning of resource fork
				 * to resource map */
  HBUINT32	dataLen;	/* Length of resource data */
  HBUINT32	mapLen;		/* Length of resource map */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (16);
};

/*
 * OpenType Font File
 */

ಸಂರಚನಾ OpenTypeFontFile
{
  ಗಣನಾ {
    CFFTag		= HB_TAG ('O','T','T','O'), /* OpenType with Postscript outlines */
    TrueTypeTag		= HB_TAG ( 0 , 1 , 0 , 0 ), /* OpenType with TrueType outlines */
    TTCTag		= HB_TAG ('t','t','c','f'), /* TrueType Collection */
    DFontTag		= HB_TAG ( 0 , 0 , 1 , 0 ), /* DFont Mac Resource Fork */
    TrueTag		= HB_TAG ('t','r','u','e'), /* Obsolete Apple TrueType */
    Typ1Tag		= HB_TAG ('t','y','p','1')  /* Obsolete Apple Type1 font in SFNT container */
  };

  hb_tag_t get_tag () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ u.tag.v; }

  unsigned ಪೂರ್ಣಾಙ್ಕ get_face_ಎಣಿಕೆ () ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (u.tag.v) {
    ಪಕ್ಷ CFFTag:	/* All the non-collection tags */
    ಪಕ್ಷ TrueTag:
    ಪಕ್ಷ Typ1Tag:
    ಪಕ್ಷ TrueTypeTag:	ಪ್ರತ್ಯಾಗಮ 1;
    ಪಕ್ಷ TTCTag:	hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.ttcHeader.get_face_ಎಣಿಕೆ ();
    ಪಕ್ಷ DFontTag:	hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.rfHeader.get_face_ಎಣಿಕೆ ();
    ಸ್ವಭಾವ:		ಪ್ರತ್ಯಾಗಮ 0;
    }
  }
  ಧ್ರುವ OpenTypeFontFace& get_face (unsigned ಪೂರ್ಣಾಙ್ಕ i, unsigned ಪೂರ್ಣಾಙ್ಕ *base_offset = nullptr) ಧ್ರುವ
  {
    ಯದಿ (base_offset)
      *base_offset = 0;
    ವಿಕಲ್ಪ (u.tag.v) {
    /* Note: for non-collection SFNT data we ignore index.  This is because
     * Apple dfont container is a container of SFNT's.  So each SFNT is a
     * non-TTC, but the index is more than zero. */
    ಪಕ್ಷ CFFTag:	/* All the non-collection tags */
    ಪಕ್ಷ TrueTag:
    ಪಕ್ಷ Typ1Tag:
    ಪಕ್ಷ TrueTypeTag:	hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.fontFace;
    ಪಕ್ಷ TTCTag:	hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.ttcHeader.get_face (i);
    ಪಕ್ಷ DFontTag:	hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.rfHeader.get_face (i, base_offset);
    ಸ್ವಭಾವ:		ಪ್ರತ್ಯಾಗಮ Null (OpenTypeFontFace);
    }
  }

  template <typename Iterator,
	    hb_requires ((hb_is_source_of<Iterator, hb_pair_t<hb_tag_t, hb_blob_t *>>::value))>
  ತಾರ್ಕಿಕ serialize_single (hb_serialize_context_t *c,
			 hb_tag_t sfnt_tag,
			 Iterator ವಸ್ತುಗಳು)
  {
    TRACE_SERIALIZE (ಏತತ್);
    ಪ್ರತಿಜ್ಞಾ (sfnt_tag != TTCTag);
    ಯದಿ (unlikely (!c->extend_min (ಏತತ್))) return_trace (ಅಸತ್ಯ);
    return_trace (u.fontFace.serialize (c, sfnt_tag, ವಸ್ತುಗಳು));
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    ಯದಿ (unlikely (!u.tag.v.sanitize (c))) return_trace (ಅಸತ್ಯ);
    hb_barrier ();
    ವಿಕಲ್ಪ (u.tag.v) {
    ಪಕ್ಷ CFFTag:	/* All the non-collection tags */
    ಪಕ್ಷ TrueTag:
    ಪಕ್ಷ Typ1Tag:
    ಪಕ್ಷ TrueTypeTag:	hb_barrier (); return_trace (u.fontFace.sanitize (c));
    ಪಕ್ಷ TTCTag:	hb_barrier (); return_trace (u.ttcHeader.sanitize (c));
    ಪಕ್ಷ DFontTag:	hb_barrier (); return_trace (u.rfHeader.sanitize (c));
    ಸ್ವಭಾವ:		return_trace (ಸತ್ಯ);
    }
  }

  ರಕ್ಷಿತ:
  ಸಙ್ಘ {
  ಸಂರಚನಾ { Tag v; }	tag;		/* 4-byte identifier. */
  OpenTypeFontFace	fontFace;
  TTCHeader		ttcHeader;
  ResourceForkHeader	rfHeader;
  } u;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_UNION (4, tag.v);
};


} /* namespace OT */


#endif /* HB_OPEN_FILE_HH */
