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

#अन्तर्भाव "hb-open-type.hh"
#अन्तर्भाव "hb-ot-head-table.hh"


नामाकाश OT {

/*
 *
 * The OpenType Font File
 *
 */


/*
 * Organization of an OpenType Font
 */

संरचना OpenTypeFontFile;
संरचना OpenTypeOffsetTable;
संरचना TTCHeader;


प्रकारनिर्देश संरचना TableRecord
{
  पूर्णाङ्क cmp (Tag t) ध्रुव { प्रत्यागम -t.cmp (tag); }

  HB_INTERNAL स्थिर पूर्णाङ्क cmp (ध्रुव शून्य *pa, ध्रुव शून्य *pb)
  {
    ध्रुव TableRecord *a = (ध्रुव TableRecord *) pa;
    ध्रुव TableRecord *b = (ध्रुव TableRecord *) pb;
    प्रत्यागम b->cmp (a->tag);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्));
  }

  Tag		tag;		/* 4-byte identifier. */
  CheckSum	checkSum;	/* CheckSum for this table. */
  Offset32	offset;		/* Offset from beginning of TrueType font
				 * file. */
  HBUINT32	length;		/* Length of this table. */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (16);
} OpenTypeTable;

प्रकारनिर्देश संरचना OpenTypeOffsetTable
{
  friend संरचना OpenTypeFontFile;

  unsigned पूर्णाङ्क get_table_गिनती () ध्रुव { प्रत्यागम tables.len; }
  ध्रुव TableRecord& get_table (unsigned पूर्णाङ्क i) ध्रुव
  { प्रत्यागम tables[i]; }
  unsigned पूर्णाङ्क get_table_tags (unsigned पूर्णाङ्क  start_offset,
			       unsigned पूर्णाङ्क *table_गिनती_2, /* IN/OUT */
			       hb_tag_t     *table_tags /* OUT */) ध्रुव
  {
    यदि (table_गिनती_2 && table_tags)
    {
      + tables.as_array ().sub_array (start_offset, table_गिनती_2)
      | hb_map (&TableRecord::tag)
      | hb_sink (hb_array (table_tags, *table_गिनती_2))
      ;
    }
    प्रत्यागम tables.len;
  }
  तार्किक find_table_index (hb_tag_t tag, unsigned पूर्णाङ्क *table_index) ध्रुव
  {
    Tag t;
    t = tag;
    /* Use lfind for small fonts; there are fonts that have unsorted table entries;
     * those tend to work in other tools, so tolerate them.
     * https://github.com/harfbuzz/harfbuzz/issues/3065 */
    यदि (tables.len < 16)
      प्रत्यागम tables.lfind (t, table_index, HB_NOT_FOUND_STORE, Index::NOT_FOUND_INDEX);
    अन्यथा
      प्रत्यागम tables.bfind (t, table_index, HB_NOT_FOUND_STORE, Index::NOT_FOUND_INDEX);
  }
  ध्रुव TableRecord& get_table_by_tag (hb_tag_t tag) ध्रुव
  {
    unsigned पूर्णाङ्क table_index;
    find_table_index (tag, &table_index);
    प्रत्यागम get_table (table_index);
  }

  सार्वजनिक:

  template <typename Iterator,
	    hb_requires ((hb_is_source_of<Iterator, hb_pair_t<hb_tag_t, hb_blob_t *>>::value))>
  तार्किक serialize (hb_serialize_context_t *c,
		  hb_tag_t sfnt_tag,
		  Iterator it)
  {
    TRACE_SERIALIZE (एतत्);
    /* Alloc 12 for the OTHeader. */
    यदि (unlikely (!c->extend_min (एतत्))) return_trace (असत्य);
    /* Write sfntVersion (bytes 0..3). */
    sfnt_version = sfnt_tag;
    /* Take space for numTables, searchRange, entrySelector, RangeShift
     * and the TableRecords themselves.  */
    unsigned अंक_वस्तुएँ = hb_len (it);
    यदि (unlikely (!tables.serialize (c, अंक_वस्तुएँ))) return_trace (असत्य);

    ध्रुव वर्ण *dir_end = (ध्रुव वर्ण *) c->head;
    HBUINT32 *checksum_adjustment = nullptr;

    /* Write OffsetTables, alloc for and write actual table blobs. */
    unsigned i = 0;
    प्रति (hb_pair_t<hb_tag_t, hb_blob_t*> entry : it)
    {
      hb_blob_t *blob = entry.second;
      unsigned len = blob->length;

      /* Allocate room for the table and copy it. */
      वर्ण *start = (वर्ण *) c->allocate_size<शून्य> (len, असत्य);
      यदि (unlikely (!start)) प्रत्यागम असत्य;

      TableRecord &rec = tables.arrayZ[i];
      rec.tag = entry.first;
      rec.length = len;
      rec.offset = 0;
      यदि (unlikely (!c->check_assign (rec.offset,
				      (unsigned) ((वर्ण *) start - (वर्ण *) एतत्),
				      HB_SERIALIZE_ERROR_OFFSET_OVERFLOW)))
        return_trace (असत्य);

      यदि (likely (len))
	hb_memcpy (start, blob->data, len);

      /* 4-byte alignment. */
      c->align (4);
      ध्रुव वर्ण *end = (ध्रुव वर्ण *) c->head;

      यदि (entry.first == HB_OT_TAG_head &&
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

    यदि (checksum_adjustment)
    {
      CheckSum checksum;

      /* The following line is a slower version of the following block. */
      //checksum.set_for_data (this, (const char *) c->head - (const char *) this);
      checksum.set_for_data (एतत्, dir_end - (ध्रुव वर्ण *) एतत्);
      प्रति (unsigned पूर्णाङ्क i = 0; i < अंक_वस्तुएँ; i++)
      {
	TableRecord &rec = tables.arrayZ[i];
	checksum = checksum + rec.checkSum;
      }

      *checksum_adjustment = 0xB1B0AFBAu - checksum;
    }

    return_trace (सत्य);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) && tables.sanitize (c));
  }

  रक्षित:
  Tag		sfnt_version;	/* '\0\001\0\00' if TrueType / 'OTTO' if CFF */
  Bin_खोज_Array_Of<TableRecord>
		tables;
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (12, tables);
} OpenTypeFontFace;


/*
 * TrueType Collections
 */

संरचना TTCHeaderVersion1
{
  friend संरचना TTCHeader;

  using DirectoryList = Array32Of<Offset32To<OpenTypeOffsetTable>>;

  unsigned पूर्णाङ्क get_face_गिनती () ध्रुव { प्रत्यागम get_directory_list ().len; }
  ध्रुव OpenTypeFontFace& get_face (unsigned पूर्णाङ्क i) ध्रुव
  { प्रत्यागम एतत्+get_directory_list ()[i]; }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
#ifndef HB_NO_BEYOND_64K
    यदि (version.minor >= 1)
    {
      यदि (unlikely (!table.sanitize_shallow (c)))
	return_trace (असत्य);
      unsigned extra_size = version.major == 2 ? 12 : 0;
      यदि (unlikely (!c->check_range (&table, table.get_size () + extra_size)))
	return_trace (असत्य);
      ध्रुव DirectoryList &table2 = StructAtOffset<DirectoryList> (&table,
								   table.get_size () + extra_size);
      return_trace (table2.sanitize (c, एतत्));
    }
#endif
    return_trace (table.sanitize (c, एतत्));
  }

  गुप्त:
  ध्रुव DirectoryList& get_directory_list () ध्रुव
  {
#ifndef HB_NO_BEYOND_64K
    यदि (version.minor >= 1)
    {
      unsigned extra_size = version.major == 2 ? 12 : 0;
      प्रत्यागम StructAtOffset<DirectoryList> (&table, table.get_size () + extra_size);
    }
#endif
    प्रत्यागम table;
  }

  रक्षित:
  Tag		ttcTag;		/* TrueType Collection ID string: 'ttcf' */
  FixedVersion<>version;	/* Version of the TTC Header. */
  DirectoryList
		table;		/* Array of offsets to the OffsetTable for each font
				 * from the beginning of the file */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (12, table);
};

संरचना TTCHeader
{
  friend संरचना OpenTypeFontFile;

  गुप्त:

  unsigned पूर्णाङ्क get_face_गिनती () ध्रुव
  {
    विकल्प (u.header.version.major) {
    पक्ष 2: /* version 2 is compatible with version 1 */
    पक्ष 1: hb_barrier (); प्रत्यागम u.version1.get_face_गिनती ();
    स्वभाव:प्रत्यागम 0;
    }
  }
  ध्रुव OpenTypeFontFace& get_face (unsigned पूर्णाङ्क i) ध्रुव
  {
    विकल्प (u.header.version.major) {
    पक्ष 2: /* version 2 is compatible with version 1 */
    पक्ष 1: hb_barrier (); प्रत्यागम u.version1.get_face (i);
    स्वभाव:प्रत्यागम Null (OpenTypeFontFace);
    }
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    यदि (unlikely (!u.header.version.sanitize (c))) return_trace (असत्य);
    hb_barrier ();
    विकल्प (u.header.version.major) {
    पक्ष 2: /* version 2 is compatible with version 1 */
    पक्ष 1: hb_barrier (); return_trace (u.version1.sanitize (c));
    स्वभाव:return_trace (सत्य);
    }
  }

  रक्षित:
  सङ्घ {
  संरचना {
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

संरचना ResourceRecord
{
  ध्रुव OpenTypeFontFace & get_face (ध्रुव शून्य *data_base) ध्रुव
  { प्रत्यागम * reinterpret_cast<ध्रुव OpenTypeFontFace *> ((data_base+offset).arrayZ); }

  तार्किक sanitize (hb_sanitize_context_t *c,
		 ध्रुव शून्य *data_base) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
		  offset.sanitize (c, data_base) &&
		  hb_barrier () &&
		  get_face (data_base).sanitize (c));
  }

  रक्षित:
  HBUINT16	id;		/* Resource ID. */
  HBINT16	nameOffset;	/* Offset from beginning of resource name list
				 * to resource name, -1 means there is none. */
  HBUINT8	attrs;		/* Resource attributes */
  NNOffset24To<Array32Of<HBUINT8>>
		offset;		/* Offset from beginning of data block to
				 * data for this resource */
  HBUINT32	reserved;	/* Reserved for handle to resource */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (12);
};

#define HB_TAG_sfnt HB_TAG ('s','f','n','t')

संरचना ResourceTypeRecord
{
  unsigned पूर्णाङ्क get_resource_गिनती () ध्रुव
  { प्रत्यागम tag == HB_TAG_sfnt ? res_गिनती_M1 + 1 : 0; }

  तार्किक is_sfnt () ध्रुव { प्रत्यागम tag == HB_TAG_sfnt; }

  ध्रुव ResourceRecord& get_resource_record (unsigned पूर्णाङ्क i,
					     ध्रुव शून्य *type_base) ध्रुव
  { प्रत्यागम (type_base+resourcesZ).as_array (get_resource_गिनती ())[i]; }

  तार्किक sanitize (hb_sanitize_context_t *c,
		 ध्रुव शून्य *type_base,
		 ध्रुव शून्य *data_base) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
		  hb_barrier () &&
		  resourcesZ.sanitize (c, type_base,
				       get_resource_गिनती (),
				       data_base));
  }

  रक्षित:
  Tag		tag;		/* Resource type. */
  HBUINT16	res_गिनती_M1;	/* Number of resources minus 1. */
  NNOffset16To<UnsizedArrayOf<ResourceRecord>>
		resourcesZ;	/* Offset from beginning of resource type list
				 * to reference item list for this type. */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (8);
};

संरचना ResourceMap
{
  unsigned पूर्णाङ्क get_face_गिनती () ध्रुव
  {
    unsigned पूर्णाङ्क गिनती = get_type_गिनती ();
    प्रति (unsigned पूर्णाङ्क i = 0; i < गिनती; i++)
    {
      ध्रुव ResourceTypeRecord& type = get_type_record (i);
      यदि (type.is_sfnt ())
	प्रत्यागम type.get_resource_गिनती ();
    }
    प्रत्यागम 0;
  }

  ध्रुव OpenTypeFontFace& get_face (unsigned पूर्णाङ्क idx,
				    ध्रुव शून्य *data_base) ध्रुव
  {
    unsigned पूर्णाङ्क गिनती = get_type_गिनती ();
    प्रति (unsigned पूर्णाङ्क i = 0; i < गिनती; i++)
    {
      ध्रुव ResourceTypeRecord& type = get_type_record (i);
      /* The check for idx < count is here because ResourceRecord is NOT null-safe.
       * Because an offset of 0 there does NOT mean null. */
      यदि (type.is_sfnt () && idx < type.get_resource_गिनती ())
	प्रत्यागम type.get_resource_record (idx, &(एतत्+typeList)).get_face (data_base);
    }
    प्रत्यागम Null (OpenTypeFontFace);
  }

  तार्किक sanitize (hb_sanitize_context_t *c, ध्रुव शून्य *data_base) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
		  hb_barrier () &&
		  typeList.sanitize (c, एतत्,
				     &(एतत्+typeList),
				     data_base));
  }

  गुप्त:
  unsigned पूर्णाङ्क get_type_गिनती () ध्रुव { प्रत्यागम (एतत्+typeList).lenM1 + 1; }

  ध्रुव ResourceTypeRecord& get_type_record (unsigned पूर्णाङ्क i) ध्रुव
  { प्रत्यागम (एतत्+typeList)[i]; }

  रक्षित:
  HBUINT8	reserved0[16];	/* Reserved for copy of resource header */
  HBUINT32	reserved1;	/* Reserved for handle to next resource map */
  HBUINT16	resreved2;	/* Reserved for file reference number */
  HBUINT16	attrs;		/* Resource fork attribute */
  NNOffset16To<ArrayOfM1<ResourceTypeRecord>>
		typeList;	/* Offset from beginning of map to
				 * resource type list */
  Offset16	nameList;	/* Offset from beginning of map to
				 * resource name list */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (28);
};

संरचना ResourceForkHeader
{
  unsigned पूर्णाङ्क get_face_गिनती () ध्रुव
  { प्रत्यागम (एतत्+map).get_face_गिनती (); }

  ध्रुव OpenTypeFontFace& get_face (unsigned पूर्णाङ्क idx,
				    unsigned पूर्णाङ्क *base_offset = nullptr) ध्रुव
  {
    ध्रुव OpenTypeFontFace &face = (एतत्+map).get_face (idx, &(एतत्+data));
    यदि (base_offset)
      *base_offset = (ध्रुव वर्ण *) &face - (ध्रुव वर्ण *) एतत्;
    प्रत्यागम face;
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
		  hb_barrier () &&
		  data.sanitize (c, एतत्, dataLen) &&
		  map.sanitize (c, एतत्, &(एतत्+data)));
  }

  रक्षित:
  NNOffset32To<UnsizedArrayOf<HBUINT8>>
		data;		/* Offset from beginning of resource fork
				 * to resource data */
  NNOffset32To<ResourceMap >
		map;		/* Offset from beginning of resource fork
				 * to resource map */
  HBUINT32	dataLen;	/* Length of resource data */
  HBUINT32	mapLen;		/* Length of resource map */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (16);
};

/*
 * OpenType Font File
 */

संरचना OpenTypeFontFile
{
  गणना {
    CFFTag		= HB_TAG ('O','T','T','O'), /* OpenType with Postscript outlines */
    TrueTypeTag		= HB_TAG ( 0 , 1 , 0 , 0 ), /* OpenType with TrueType outlines */
    TTCTag		= HB_TAG ('t','t','c','f'), /* TrueType Collection */
    DFontTag		= HB_TAG ( 0 , 0 , 1 , 0 ), /* DFont Mac Resource Fork */
    TrueTag		= HB_TAG ('t','r','u','e'), /* Obsolete Apple TrueType */
    Typ1Tag		= HB_TAG ('t','y','p','1')  /* Obsolete Apple Type1 font in SFNT container */
  };

  hb_tag_t get_tag () ध्रुव { प्रत्यागम u.tag.v; }

  unsigned पूर्णाङ्क get_face_गिनती () ध्रुव
  {
    विकल्प (u.tag.v) {
    पक्ष CFFTag:	/* All the non-collection tags */
    पक्ष TrueTag:
    पक्ष Typ1Tag:
    पक्ष TrueTypeTag:	प्रत्यागम 1;
    पक्ष TTCTag:	hb_barrier (); प्रत्यागम u.ttcHeader.get_face_गिनती ();
    पक्ष DFontTag:	hb_barrier (); प्रत्यागम u.rfHeader.get_face_गिनती ();
    स्वभाव:		प्रत्यागम 0;
    }
  }
  ध्रुव OpenTypeFontFace& get_face (unsigned पूर्णाङ्क i, unsigned पूर्णाङ्क *base_offset = nullptr) ध्रुव
  {
    यदि (base_offset)
      *base_offset = 0;
    विकल्प (u.tag.v) {
    /* Note: for non-collection SFNT data we ignore index.  This is because
     * Apple dfont container is a container of SFNT's.  So each SFNT is a
     * non-TTC, but the index is more than zero. */
    पक्ष CFFTag:	/* All the non-collection tags */
    पक्ष TrueTag:
    पक्ष Typ1Tag:
    पक्ष TrueTypeTag:	hb_barrier (); प्रत्यागम u.fontFace;
    पक्ष TTCTag:	hb_barrier (); प्रत्यागम u.ttcHeader.get_face (i);
    पक्ष DFontTag:	hb_barrier (); प्रत्यागम u.rfHeader.get_face (i, base_offset);
    स्वभाव:		प्रत्यागम Null (OpenTypeFontFace);
    }
  }

  template <typename Iterator,
	    hb_requires ((hb_is_source_of<Iterator, hb_pair_t<hb_tag_t, hb_blob_t *>>::value))>
  तार्किक serialize_single (hb_serialize_context_t *c,
			 hb_tag_t sfnt_tag,
			 Iterator वस्तुएँ)
  {
    TRACE_SERIALIZE (एतत्);
    प्रतिज्ञा (sfnt_tag != TTCTag);
    यदि (unlikely (!c->extend_min (एतत्))) return_trace (असत्य);
    return_trace (u.fontFace.serialize (c, sfnt_tag, वस्तुएँ));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    यदि (unlikely (!u.tag.v.sanitize (c))) return_trace (असत्य);
    hb_barrier ();
    विकल्प (u.tag.v) {
    पक्ष CFFTag:	/* All the non-collection tags */
    पक्ष TrueTag:
    पक्ष Typ1Tag:
    पक्ष TrueTypeTag:	hb_barrier (); return_trace (u.fontFace.sanitize (c));
    पक्ष TTCTag:	hb_barrier (); return_trace (u.ttcHeader.sanitize (c));
    पक्ष DFontTag:	hb_barrier (); return_trace (u.rfHeader.sanitize (c));
    स्वभाव:		return_trace (सत्य);
    }
  }

  रक्षित:
  सङ्घ {
  संरचना { Tag v; }	tag;		/* 4-byte identifier. */
  OpenTypeFontFace	fontFace;
  TTCHeader		ttcHeader;
  ResourceForkHeader	rfHeader;
  } u;
  सार्वजनिक:
  DEFINE_SIZE_UNION (4, tag.v);
};


} /* namespace OT */


#endif /* HB_OPEN_FILE_HH */
