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


प्रकारव्याख्या संरचना TableRecord
{
  पूर्णांक cmp (Tag t) स्थिरांक { परत -t.cmp (tag); }

  HB_INTERNAL स्थिर पूर्णांक cmp (स्थिरांक शून्य *pa, स्थिरांक शून्य *pb)
  {
    स्थिरांक TableRecord *a = (स्थिरांक TableRecord *) pa;
    स्थिरांक TableRecord *b = (स्थिरांक TableRecord *) pb;
    परत b->cmp (a->tag);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे));
  }

  Tag		tag;		/* 4-byte identifier. */
  CheckSum	checkSum;	/* CheckSum for this table. */
  Offset32	offset;		/* Offset from beginning of TrueType font
				 * file. */
  HBUINT32	length;		/* Length of this table. */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (16);
} OpenTypeTable;

प्रकारव्याख्या संरचना OpenTypeOffsetTable
{
  मित्र संरचना OpenTypeFontFile;

  अचिन्हित पूर्णांक get_table_मोजणी () स्थिरांक { परत tables.len; }
  स्थिरांक TableRecord& get_table (अचिन्हित पूर्णांक i) स्थिरांक
  { परत tables[i]; }
  अचिन्हित पूर्णांक get_table_tags (अचिन्हित पूर्णांक  start_offset,
			       अचिन्हित पूर्णांक *table_मोजणी_2, /* IN/OUT */
			       hb_tag_t     *table_tags /* OUT */) स्थिरांक
  {
    जर (table_मोजणी_2 && table_tags)
    {
      + tables.as_array ().sub_array (start_offset, table_मोजणी_2)
      | hb_map (&TableRecord::tag)
      | hb_sink (hb_array (table_tags, *table_मोजणी_2))
      ;
    }
    परत tables.len;
  }
  तार्किक find_table_index (hb_tag_t tag, अचिन्हित पूर्णांक *table_index) स्थिरांक
  {
    Tag t;
    t = tag;
    /* Use lfind for small fonts; there are fonts that have unsorted table entries;
     * those tend to work in other tools, so tolerate them.
     * https://github.com/harfbuzz/harfbuzz/issues/3065 */
    जर (tables.len < 16)
      परत tables.lfind (t, table_index, HB_NOT_FOUND_STORE, Index::NOT_FOUND_INDEX);
    अन्यथा
      परत tables.bfind (t, table_index, HB_NOT_FOUND_STORE, Index::NOT_FOUND_INDEX);
  }
  स्थिरांक TableRecord& get_table_by_tag (hb_tag_t tag) स्थिरांक
  {
    अचिन्हित पूर्णांक table_index;
    find_table_index (tag, &table_index);
    परत get_table (table_index);
  }

  सार्वजनिक:

  ढाँचा <typename Iterator,
	    hb_requires ((hb_is_source_of<Iterator, hb_pair_t<hb_tag_t, hb_blob_t *>>::value))>
  तार्किक serialize (hb_serialize_context_t *c,
		  hb_tag_t sfnt_tag,
		  Iterator it)
  {
    TRACE_SERIALIZE (हे);
    /* Alloc 12 for the OTHeader. */
    जर (unlikely (!c->extend_min (हे))) return_trace (असत्य);
    /* Write sfntVersion (bytes 0..3). */
    sfnt_version = sfnt_tag;
    /* Take space for numTables, searchRange, entrySelector, RangeShift
     * and the TableRecords themselves.  */
    अचिन्हित अंक_वस्तुसंच = hb_len (it);
    जर (unlikely (!tables.serialize (c, अंक_वस्तुसंच))) return_trace (असत्य);

    स्थिरांक अक्षर *dir_end = (स्थिरांक अक्षर *) c->head;
    HBUINT32 *checksum_adjustment = nullptr;

    /* Write OffsetTables, alloc for and write actual table blobs. */
    अचिन्हित i = 0;
    साठी (hb_pair_t<hb_tag_t, hb_blob_t*> entry : it)
    {
      hb_blob_t *blob = entry.second;
      अचिन्हित len = blob->length;

      /* Allocate room for the table and copy it. */
      अक्षर *start = (अक्षर *) c->allocate_size<शून्य> (len, असत्य);
      जर (unlikely (!start)) परत असत्य;

      TableRecord &rec = tables.arrayZ[i];
      rec.tag = entry.first;
      rec.length = len;
      rec.offset = 0;
      जर (unlikely (!c->check_assign (rec.offset,
				      (अचिन्हित) ((अक्षर *) start - (अक्षर *) हे),
				      HB_SERIALIZE_ERROR_OFFSET_OVERFLOW)))
        return_trace (असत्य);

      जर (likely (len))
	hb_memcpy (start, blob->data, len);

      /* 4-byte alignment. */
      c->align (4);
      स्थिरांक अक्षर *end = (स्थिरांक अक्षर *) c->head;

      जर (entry.first == HB_OT_TAG_head &&
	  (अचिन्हित) (end - start) >= head::static_size)
      {
	head *h = (head *) start;
	checksum_adjustment = &h->checkSumAdjustment;
	*checksum_adjustment = 0;
      }

      rec.checkSum.set_for_data (start, end - start);
      i++;
    }

    tables.क्विक ();

    जर (checksum_adjustment)
    {
      CheckSum checksum;

      /* The following line is a slower version of the following block. */
      //checksum.set_for_data (this, (const char *) c->head - (const char *) this);
      checksum.set_for_data (हे, dir_end - (स्थिरांक अक्षर *) हे);
      साठी (अचिन्हित पूर्णांक i = 0; i < अंक_वस्तुसंच; i++)
      {
	TableRecord &rec = tables.arrayZ[i];
	checksum = checksum + rec.checkSum;
      }

      *checksum_adjustment = 0xB1B0AFBAu - checksum;
    }

    return_trace (सत्य);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) && tables.sanitize (c));
  }

  संरक्षित:
  Tag		sfnt_version;	/* '\0\001\0\00' if TrueType / 'OTTO' if CFF */
  Bin_शोध_Array_Of<TableRecord>
		tables;
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (12, tables);
} OpenTypeFontFace;


/*
 * TrueType Collections
 */

संरचना TTCHeaderVersion1
{
  मित्र संरचना TTCHeader;

  using DirectoryList = Array32Of<Offset32To<OpenTypeOffsetTable>>;

  अचिन्हित पूर्णांक get_face_मोजणी () स्थिरांक { परत get_directory_list ().len; }
  स्थिरांक OpenTypeFontFace& get_face (अचिन्हित पूर्णांक i) स्थिरांक
  { परत हे+get_directory_list ()[i]; }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
#ifndef HB_NO_BEYOND_64K
    जर (version.minor >= 1)
    {
      जर (unlikely (!table.sanitize_shallow (c)))
	return_trace (असत्य);
      अचिन्हित extra_size = version.major == 2 ? 12 : 0;
      जर (unlikely (!c->check_range (&table, table.get_size () + extra_size)))
	return_trace (असत्य);
      स्थिरांक DirectoryList &table2 = StructAtOffset<DirectoryList> (&table,
								   table.get_size () + extra_size);
      return_trace (table2.sanitize (c, हे));
    }
#endif
    return_trace (table.sanitize (c, हे));
  }

  खाजगी:
  स्थिरांक DirectoryList& get_directory_list () स्थिरांक
  {
#ifndef HB_NO_BEYOND_64K
    जर (version.minor >= 1)
    {
      अचिन्हित extra_size = version.major == 2 ? 12 : 0;
      परत StructAtOffset<DirectoryList> (&table, table.get_size () + extra_size);
    }
#endif
    परत table;
  }

  संरक्षित:
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
  मित्र संरचना OpenTypeFontFile;

  खाजगी:

  अचिन्हित पूर्णांक get_face_मोजणी () स्थिरांक
  {
    निवड (u.header.version.major) {
    स्थिती 2: /* version 2 is compatible with version 1 */
    स्थिती 1: hb_barrier (); परत u.version1.get_face_मोजणी ();
    पूर्वनिर्धारित:परत 0;
    }
  }
  स्थिरांक OpenTypeFontFace& get_face (अचिन्हित पूर्णांक i) स्थिरांक
  {
    निवड (u.header.version.major) {
    स्थिती 2: /* version 2 is compatible with version 1 */
    स्थिती 1: hb_barrier (); परत u.version1.get_face (i);
    पूर्वनिर्धारित:परत Null (OpenTypeFontFace);
    }
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    जर (unlikely (!u.header.version.sanitize (c))) return_trace (असत्य);
    hb_barrier ();
    निवड (u.header.version.major) {
    स्थिती 2: /* version 2 is compatible with version 1 */
    स्थिती 1: hb_barrier (); return_trace (u.version1.sanitize (c));
    पूर्वनिर्धारित:return_trace (सत्य);
    }
  }

  संरक्षित:
  संघ {
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
  स्थिरांक OpenTypeFontFace & get_face (स्थिरांक शून्य *data_base) स्थिरांक
  { परत * reinterpret_cast<स्थिरांक OpenTypeFontFace *> ((data_base+offset).arrayZ); }

  तार्किक sanitize (hb_sanitize_context_t *c,
		 स्थिरांक शून्य *data_base) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) &&
		  offset.sanitize (c, data_base) &&
		  hb_barrier () &&
		  get_face (data_base).sanitize (c));
  }

  संरक्षित:
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
  अचिन्हित पूर्णांक get_resource_मोजणी () स्थिरांक
  { परत tag == HB_TAG_sfnt ? res_मोजणी_M1 + 1 : 0; }

  तार्किक is_sfnt () स्थिरांक { परत tag == HB_TAG_sfnt; }

  स्थिरांक ResourceRecord& get_resource_record (अचिन्हित पूर्णांक i,
					     स्थिरांक शून्य *type_base) स्थिरांक
  { परत (type_base+resourcesZ).as_array (get_resource_मोजणी ())[i]; }

  तार्किक sanitize (hb_sanitize_context_t *c,
		 स्थिरांक शून्य *type_base,
		 स्थिरांक शून्य *data_base) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) &&
		  hb_barrier () &&
		  resourcesZ.sanitize (c, type_base,
				       get_resource_मोजणी (),
				       data_base));
  }

  संरक्षित:
  Tag		tag;		/* Resource type. */
  HBUINT16	res_मोजणी_M1;	/* Number of resources minus 1. */
  NNOffset16To<UnsizedArrayOf<ResourceRecord>>
		resourcesZ;	/* Offset from beginning of resource type list
				 * to reference item list for this type. */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (8);
};

संरचना ResourceMap
{
  अचिन्हित पूर्णांक get_face_मोजणी () स्थिरांक
  {
    अचिन्हित पूर्णांक मोजणी = get_type_मोजणी ();
    साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
    {
      स्थिरांक ResourceTypeRecord& type = get_type_record (i);
      जर (type.is_sfnt ())
	परत type.get_resource_मोजणी ();
    }
    परत 0;
  }

  स्थिरांक OpenTypeFontFace& get_face (अचिन्हित पूर्णांक idx,
				    स्थिरांक शून्य *data_base) स्थिरांक
  {
    अचिन्हित पूर्णांक मोजणी = get_type_मोजणी ();
    साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
    {
      स्थिरांक ResourceTypeRecord& type = get_type_record (i);
      /* The check for idx < count is here because ResourceRecord is NOT null-safe.
       * Because an offset of 0 there does NOT mean null. */
      जर (type.is_sfnt () && idx < type.get_resource_मोजणी ())
	परत type.get_resource_record (idx, &(हे+typeList)).get_face (data_base);
    }
    परत Null (OpenTypeFontFace);
  }

  तार्किक sanitize (hb_sanitize_context_t *c, स्थिरांक शून्य *data_base) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) &&
		  hb_barrier () &&
		  typeList.sanitize (c, हे,
				     &(हे+typeList),
				     data_base));
  }

  खाजगी:
  अचिन्हित पूर्णांक get_type_मोजणी () स्थिरांक { परत (हे+typeList).lenM1 + 1; }

  स्थिरांक ResourceTypeRecord& get_type_record (अचिन्हित पूर्णांक i) स्थिरांक
  { परत (हे+typeList)[i]; }

  संरक्षित:
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
  अचिन्हित पूर्णांक get_face_मोजणी () स्थिरांक
  { परत (हे+map).get_face_मोजणी (); }

  स्थिरांक OpenTypeFontFace& get_face (अचिन्हित पूर्णांक idx,
				    अचिन्हित पूर्णांक *base_offset = nullptr) स्थिरांक
  {
    स्थिरांक OpenTypeFontFace &face = (हे+map).get_face (idx, &(हे+data));
    जर (base_offset)
      *base_offset = (स्थिरांक अक्षर *) &face - (स्थिरांक अक्षर *) हे;
    परत face;
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) &&
		  hb_barrier () &&
		  data.sanitize (c, हे, dataLen) &&
		  map.sanitize (c, हे, &(हे+data)));
  }

  संरक्षित:
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

  hb_tag_t get_tag () स्थिरांक { परत u.tag.v; }

  अचिन्हित पूर्णांक get_face_मोजणी () स्थिरांक
  {
    निवड (u.tag.v) {
    स्थिती CFFTag:	/* All the non-collection tags */
    स्थिती TrueTag:
    स्थिती Typ1Tag:
    स्थिती TrueTypeTag:	परत 1;
    स्थिती TTCTag:	hb_barrier (); परत u.ttcHeader.get_face_मोजणी ();
    स्थिती DFontTag:	hb_barrier (); परत u.rfHeader.get_face_मोजणी ();
    पूर्वनिर्धारित:		परत 0;
    }
  }
  स्थिरांक OpenTypeFontFace& get_face (अचिन्हित पूर्णांक i, अचिन्हित पूर्णांक *base_offset = nullptr) स्थिरांक
  {
    जर (base_offset)
      *base_offset = 0;
    निवड (u.tag.v) {
    /* Note: for non-collection SFNT data we ignore index.  This is because
     * Apple dfont container is a container of SFNT's.  So each SFNT is a
     * non-TTC, but the index is more than zero. */
    स्थिती CFFTag:	/* All the non-collection tags */
    स्थिती TrueTag:
    स्थिती Typ1Tag:
    स्थिती TrueTypeTag:	hb_barrier (); परत u.fontFace;
    स्थिती TTCTag:	hb_barrier (); परत u.ttcHeader.get_face (i);
    स्थिती DFontTag:	hb_barrier (); परत u.rfHeader.get_face (i, base_offset);
    पूर्वनिर्धारित:		परत Null (OpenTypeFontFace);
    }
  }

  ढाँचा <typename Iterator,
	    hb_requires ((hb_is_source_of<Iterator, hb_pair_t<hb_tag_t, hb_blob_t *>>::value))>
  तार्किक serialize_single (hb_serialize_context_t *c,
			 hb_tag_t sfnt_tag,
			 Iterator वस्तुसंच)
  {
    TRACE_SERIALIZE (हे);
    प्रतिज्ञा (sfnt_tag != TTCTag);
    जर (unlikely (!c->extend_min (हे))) return_trace (असत्य);
    return_trace (u.fontFace.serialize (c, sfnt_tag, वस्तुसंच));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    जर (unlikely (!u.tag.v.sanitize (c))) return_trace (असत्य);
    hb_barrier ();
    निवड (u.tag.v) {
    स्थिती CFFTag:	/* All the non-collection tags */
    स्थिती TrueTag:
    स्थिती Typ1Tag:
    स्थिती TrueTypeTag:	hb_barrier (); return_trace (u.fontFace.sanitize (c));
    स्थिती TTCTag:	hb_barrier (); return_trace (u.ttcHeader.sanitize (c));
    स्थिती DFontTag:	hb_barrier (); return_trace (u.rfHeader.sanitize (c));
    पूर्वनिर्धारित:		return_trace (सत्य);
    }
  }

  संरक्षित:
  संघ {
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
