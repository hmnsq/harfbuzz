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

#اگر_غیر_تعریف_شدہ HB_OPEN_FILE_HH
#تعریف_کرو HB_OPEN_FILE_HH

#شامل "hb-open-type.hh"
#شامل "hb-ot-head-table.hh"


نامخلا OT {

/*
 *
 * The OpenType Font File
 *
 */


/*
 * Organization of an OpenType Font
 */

ساخت OpenTypeFontFile;
ساخت Open_Type_انحراف_جدول;
ساخت TTCHeader;


قسمتعریف ساخت جدول_ریکارڈ
{
  صحیح cmp (لیبل t) ثابت { واپس -t.cmp (لیبل_2); }

  HB_اندرونی ساکن صحیح cmp (ثابت خالی *pa, ثابت خالی *pb)
  {
    ثابت جدول_ریکارڈ *a = (ثابت جدول_ریکارڈ *) pa;
    ثابت جدول_ریکارڈ *b = (ثابت جدول_ریکارڈ *) pb;
    واپس b->cmp (a->لیبل_2);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ));
  }

  لیبل		لیبل_2;		/* 4-byte identifier. */
  Check_مجموع	check_مجموع;	/* CheckSum for this table. */
  Offset32	انحراف;		/* Offset from beginning of TrueType font
				 * file. */
  HBUINT32	لمبائی;		/* Length of this table. */
  عوامی:
  DEFINE_سائز_STATIC (16);
} Open_Type_جدول;

قسمتعریف ساخت Open_Type_انحراف_جدول
{
  دوست ساخت OpenTypeFontFile;

  غیرعلامتی صحیح get_جدول_گنتی () ثابت { واپس tables.len; }
  ثابت جدول_ریکارڈ& get_جدول (غیرعلامتی صحیح i) ثابت
  { واپس tables[i]; }
  غیرعلامتی صحیح get_جدول_لیبل_فہرست (غیرعلامتی صحیح  آغاز_انحراف,
			       غیرعلامتی صحیح *جدول_گنتی_2, /* IN/OUT */
			       hb_لیبل_t     *جدول_لیبل_فہرست /*برآمد*/) ثابت
  {
    اگر (جدول_گنتی_2 && جدول_لیبل_فہرست)
    {
      + tables.as_array ().sub_array (آغاز_انحراف, جدول_گنتی_2)
      | hb_map (&جدول_ریکارڈ::لیبل_2)
      | hb_sink (hb_array (جدول_لیبل_فہرست, *جدول_گنتی_2))
      ;
    }
    واپس tables.len;
  }
  منطقی ڈھونڈو_جدول_اشاریہ (hb_لیبل_t لیبل_2, غیرعلامتی صحیح *جدول_اشاریہ) ثابت
  {
    لیبل t;
    t = لیبل_2;
    /* Use lfind for small fonts; there are fonts that have unsorted table entries;
     * those tend to work in other tools, so tolerate them.
     * https://github.com/harfbuzz/harfbuzz/issues/3065 */
    اگر (tables.len < 16)
      واپس tables.lfind (t, جدول_اشاریہ, HB_NOT_ملا_STORE, اشاریہ::NOT_ملا_اشاریہ);
    ورنہ
      واپس tables.bfind (t, جدول_اشاریہ, HB_NOT_ملا_STORE, اشاریہ::NOT_ملا_اشاریہ);
  }
  ثابت جدول_ریکارڈ& get_جدول_by_لیبل (hb_لیبل_t لیبل_2) ثابت
  {
    غیرعلامتی صحیح جدول_اشاریہ;
    ڈھونڈو_جدول_اشاریہ (لیبل_2, &جدول_اشاریہ);
    واپس get_جدول (جدول_اشاریہ);
  }

  عوامی:

  سانچہ <قسم_نام Iterator,
	    hb_requires ((hb_is_ماخذ_کا<Iterator, hb_pair_t<hb_لیبل_t, hb_blob_t *>>::value))>
  منطقی serialize (hb_serialize_context_t *c,
		  hb_لیبل_t sfnt_لیبل,
		  Iterator it)
  {
    TRACE_SERIALIZE (یہ);
    /* Alloc 12 for the OTHeader. */
    اگر (unlikely (!c->extend_min (یہ))) return_trace (جھوٹ);
    /* Write sfntVersion (bytes 0..3). */
    sfnt_نسخہ = sfnt_لیبل;
    /* Take space for numTables, searchRange, entrySelector, RangeShift
     * and the TableRecords themselves.  */
    غیرعلامتی عدد_اشیاء = hb_len (it);
    اگر (unlikely (!tables.serialize (c, عدد_اشیاء))) return_trace (جھوٹ);

    ثابت حرف *dir_انتہا = (ثابت حرف *) c->سر_2;
    HBUINT32 *checksum_adjustment = خالی_اشاریہ;

    /* Write OffsetTables, alloc for and write actual table blobs. */
    غیرعلامتی i = 0;
    برائے (hb_pair_t<hb_لیبل_t, hb_blob_t*> اندراج_2 : it)
    {
      hb_blob_t *blob = اندراج_2.second;
      غیرعلامتی len = blob->لمبائی;

      /* Allocate room for the table and copy it. */
      حرف *آغاز = (حرف *) c->allocate_سائز<خالی> (len, جھوٹ);
      اگر (unlikely (!آغاز)) واپس جھوٹ;

      جدول_ریکارڈ &rec = tables.arrayZ[i];
      rec.لیبل_2 = اندراج_2.پہلا;
      rec.لمبائی = len;
      rec.انحراف = 0;
      اگر (unlikely (!c->check_تفویض (rec.انحراف,
				      (غیرعلامتی) ((حرف *) آغاز - (حرف *) یہ),
				      HB_SERIALIZE_ERROR_انحراف_حد_سے_تجاوز)))
        return_trace (جھوٹ);

      اگر (likely (len))
	hb_memcpy (آغاز, blob->ڈیٹا, len);

      /* 4-byte alignment. */
      c->align (4);
      ثابت حرف *انتہا = (ثابت حرف *) c->سر_2;

      اگر (اندراج_2.پہلا == HB_OT_لیبل_سر &&
	  (غیرعلامتی) (انتہا - آغاز) >= سر_2::static_سائز)
      {
	سر_2 *h = (سر_2 *) آغاز;
	checksum_adjustment = &h->check_مجموع_Adjustment;
	*checksum_adjustment = 0;
      }

      rec.check_مجموع.سیٹ_for_ڈیٹا (آغاز, انتہا - آغاز);
      i++;
    }

    tables.qsort ();

    اگر (checksum_adjustment)
    {
      Check_مجموع checksum;

      /* The following line is a slower version of the following block. */
      //checksum.set_for_data (this, (const char *) c->head - (const char *) this);
      checksum.سیٹ_for_ڈیٹا (یہ, dir_انتہا - (ثابت حرف *) یہ);
      برائے (غیرعلامتی صحیح i = 0; i < عدد_اشیاء; i++)
      {
	جدول_ریکارڈ &rec = tables.arrayZ[i];
	checksum = checksum + rec.check_مجموع;
      }

      *checksum_adjustment = 0xB1B0AFBAu - checksum;
    }

    return_trace (سچ);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) && tables.sanitize (c));
  }

  محفوظ:
  لیبل		sfnt_نسخہ;	/* '\0\001\0\00' if TrueType / 'OTTO' if CFF */
  Bin_تلاش_Array_کا<جدول_ریکارڈ>
		tables;
  عوامی:
  DEFINE_سائز_ARRAY (12, tables);
} OpenTypeFontFace;


/*
 * TrueType Collections
 */

ساخت TTCHeaderVersion1
{
  دوست ساخت TTCHeader;

  استعمال DirectoryList = Array32_کا<Offset32_مقصد<Open_Type_انحراف_جدول>>;

  غیرعلامتی صحیح get_face_گنتی () ثابت { واپس get_directory_list ().len; }
  ثابت OpenTypeFontFace& get_face (غیرعلامتی صحیح i) ثابت
  { واپس یہ+get_directory_list ()[i]; }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    اگر (نسخہ.minor >= 1)
    {
      اگر (unlikely (!جدول.sanitize_shallow (c)))
	return_trace (جھوٹ);
      غیرعلامتی extra_سائز = نسخہ.major == 2 ? 12 : 0;
      اگر (unlikely (!c->check_range (&جدول, جدول.get_سائز () + extra_سائز)))
	return_trace (جھوٹ);
      ثابت DirectoryList &table2 = Struct_At_انحراف<DirectoryList> (&جدول,
								   جدول.get_سائز () + extra_سائز);
      return_trace (table2.sanitize (c, یہ));
    }
#اگر_ختم
    return_trace (جدول.sanitize (c, یہ));
  }

  نجی:
  ثابت DirectoryList& get_directory_list () ثابت
  {
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    اگر (نسخہ.minor >= 1)
    {
      غیرعلامتی extra_سائز = نسخہ.major == 2 ? 12 : 0;
      واپس Struct_At_انحراف<DirectoryList> (&جدول, جدول.get_سائز () + extra_سائز);
    }
#اگر_ختم
    واپس جدول;
  }

  محفوظ:
  لیبل		ttc_لیبل;		/* TrueType Collection ID string: 'ttcf' */
  Fixed_نسخہ<>نسخہ;	/* Version of the TTC Header. */
  DirectoryList
		جدول;		/* Array of offsets to the OffsetTable for each font
				 * from the beginning of the file */
  عوامی:
  DEFINE_سائز_ARRAY (12, جدول);
};

ساخت TTCHeader
{
  دوست ساخت OpenTypeFontFile;

  نجی:

  غیرعلامتی صحیح get_face_گنتی () ثابت
  {
    انتخاب (u.سرخی.نسخہ.major) {
    حالت 2: /* version 2 is compatible with version 1 */
    حالت 1: hb_barrier (); واپس u.version1.get_face_گنتی ();
    طےشدہ:واپس 0;
    }
  }
  ثابت OpenTypeFontFace& get_face (غیرعلامتی صحیح i) ثابت
  {
    انتخاب (u.سرخی.نسخہ.major) {
    حالت 2: /* version 2 is compatible with version 1 */
    حالت 1: hb_barrier (); واپس u.version1.get_face (i);
    طےشدہ:واپس Null (OpenTypeFontFace);
    }
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    اگر (unlikely (!u.سرخی.نسخہ.sanitize (c))) return_trace (جھوٹ);
    hb_barrier ();
    انتخاب (u.سرخی.نسخہ.major) {
    حالت 2: /* version 2 is compatible with version 1 */
    حالت 1: hb_barrier (); return_trace (u.version1.sanitize (c));
    طےشدہ:return_trace (سچ);
    }
  }

  محفوظ:
  اتحاد {
  ساخت {
  لیبل		ttc_لیبل;		/* TrueType Collection ID string: 'ttcf' */
  Fixed_نسخہ<>نسخہ;	/* Version of the TTC Header (1.0 or 2.0),
				 * 0x00010000u or 0x00020000u */
  }			سرخی;
  TTCHeaderVersion1	version1;
  } u;
};

/*
 * Mac Resource Fork
 *
 * http://mirror.informatimago.com/next/developer.apple.com/documentation/mac/MoreToolbox/MoreToolbox-99.html
 */

ساخت Resource_ریکارڈ
{
  ثابت OpenTypeFontFace & get_face (ثابت خالی *ڈیٹا_بنیاد) ثابت
  { واپس * reinterpret_cast<ثابت OpenTypeFontFace *> ((ڈیٹا_بنیاد+انحراف).arrayZ); }

  منطقی sanitize (hb_sanitize_context_t *c,
		 ثابت خالی *ڈیٹا_بنیاد) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) &&
		  انحراف.sanitize (c, ڈیٹا_بنیاد) &&
		  hb_barrier () &&
		  get_face (ڈیٹا_بنیاد).sanitize (c));
  }

  محفوظ:
  HBUINT16	id;		/* Resource ID. */
  HBINT16	name_انحراف;	/* Offset from beginning of resource name list
				 * to resource name, -1 means there is none. */
  HBUINT8	attrs;		/* Resource attributes */
  NNOffset24_مقصد<Array32_کا<HBUINT8>>
		انحراف;		/* Offset from beginning of data block to
				 * data for this resource */
  HBUINT32	reserved;	/* Reserved for handle to resource */
  عوامی:
  DEFINE_سائز_STATIC (12);
};

#تعریف_کرو HB_لیبل_sfnt HB_لیبل ('s','f','n','t')

ساخت Resource_Type_ریکارڈ
{
  غیرعلامتی صحیح get_resource_گنتی () ثابت
  { واپس لیبل_2 == HB_لیبل_sfnt ? res_گنتی_M1 + 1 : 0; }

  منطقی is_sfnt () ثابت { واپس لیبل_2 == HB_لیبل_sfnt; }

  ثابت Resource_ریکارڈ& get_resource_ریکارڈ (غیرعلامتی صحیح i,
					     ثابت خالی *type_بنیاد) ثابت
  { واپس (type_بنیاد+resourcesZ).as_array (get_resource_گنتی ())[i]; }

  منطقی sanitize (hb_sanitize_context_t *c,
		 ثابت خالی *type_بنیاد,
		 ثابت خالی *ڈیٹا_بنیاد) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) &&
		  hb_barrier () &&
		  resourcesZ.sanitize (c, type_بنیاد,
				       get_resource_گنتی (),
				       ڈیٹا_بنیاد));
  }

  محفوظ:
  لیبل		لیبل_2;		/* Resource type. */
  HBUINT16	res_گنتی_M1;	/* Number of resources minus 1. */
  NNOffset16_مقصد<Unsized_Array_کا<Resource_ریکارڈ>>
		resourcesZ;	/* Offset from beginning of resource type list
				 * to reference item list for this type. */
  عوامی:
  DEFINE_سائز_STATIC (8);
};

ساخت ResourceMap
{
  غیرعلامتی صحیح get_face_گنتی () ثابت
  {
    غیرعلامتی صحیح گنتی = get_type_گنتی ();
    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
    {
      ثابت Resource_Type_ریکارڈ& type = get_type_ریکارڈ (i);
      اگر (type.is_sfnt ())
	واپس type.get_resource_گنتی ();
    }
    واپس 0;
  }

  ثابت OpenTypeFontFace& get_face (غیرعلامتی صحیح idx,
				    ثابت خالی *ڈیٹا_بنیاد) ثابت
  {
    غیرعلامتی صحیح گنتی = get_type_گنتی ();
    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
    {
      ثابت Resource_Type_ریکارڈ& type = get_type_ریکارڈ (i);
      /* The check for idx < count is here because ResourceRecord is NOT null-safe.
       * Because an offset of 0 there does NOT mean null. */
      اگر (type.is_sfnt () && idx < type.get_resource_گنتی ())
	واپس type.get_resource_ریکارڈ (idx, &(یہ+typeList)).get_face (ڈیٹا_بنیاد);
    }
    واپس Null (OpenTypeFontFace);
  }

  منطقی sanitize (hb_sanitize_context_t *c, ثابت خالی *ڈیٹا_بنیاد) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) &&
		  hb_barrier () &&
		  typeList.sanitize (c, یہ,
				     &(یہ+typeList),
				     ڈیٹا_بنیاد));
  }

  نجی:
  غیرعلامتی صحیح get_type_گنتی () ثابت { واپس (یہ+typeList).lenM1 + 1; }

  ثابت Resource_Type_ریکارڈ& get_type_ریکارڈ (غیرعلامتی صحیح i) ثابت
  { واپس (یہ+typeList)[i]; }

  محفوظ:
  HBUINT8	reserved0[16];	/* Reserved for copy of resource header */
  HBUINT32	reserved1;	/* Reserved for handle to next resource map */
  HBUINT16	resreved2;	/* Reserved for file reference number */
  HBUINT16	attrs;		/* Resource fork attribute */
  NNOffset16_مقصد<Array_کا_M1<Resource_Type_ریکارڈ>>
		typeList;	/* Offset from beginning of map to
				 * resource type list */
  Offset16	nameList;	/* Offset from beginning of map to
				 * resource name list */
  عوامی:
  DEFINE_سائز_STATIC (28);
};

ساخت Resource_Fork_سرخی
{
  غیرعلامتی صحیح get_face_گنتی () ثابت
  { واپس (یہ+map).get_face_گنتی (); }

  ثابت OpenTypeFontFace& get_face (غیرعلامتی صحیح idx,
				    غیرعلامتی صحیح *بنیاد_انحراف = خالی_اشاریہ) ثابت
  {
    ثابت OpenTypeFontFace &face = (یہ+map).get_face (idx, &(یہ+ڈیٹا));
    اگر (بنیاد_انحراف)
      *بنیاد_انحراف = (ثابت حرف *) &face - (ثابت حرف *) یہ;
    واپس face;
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) &&
		  hb_barrier () &&
		  ڈیٹا.sanitize (c, یہ, ڈیٹا_Len) &&
		  map.sanitize (c, یہ, &(یہ+ڈیٹا)));
  }

  محفوظ:
  NNOffset32_مقصد<Unsized_Array_کا<HBUINT8>>
		ڈیٹا;		/* Offset from beginning of resource fork
				 * to resource data */
  NNOffset32_مقصد<ResourceMap >
		map;		/* Offset from beginning of resource fork
				 * to resource map */
  HBUINT32	ڈیٹا_Len;	/* Length of resource data */
  HBUINT32	mapLen;		/* Length of resource map */
  عوامی:
  DEFINE_سائز_STATIC (16);
};

/*
 * OpenType Font File
 */

ساخت OpenTypeFontFile
{
  شمار {
    CFFTag		= HB_لیبل ('O','T','T','O'), /* OpenType with Postscript outlines */
    True_Type_لیبل		= HB_لیبل ( 0 , 1 , 0 , 0 ), /* OpenType with TrueType outlines */
    TTCTag		= HB_لیبل ('t','t','c','f'), /* TrueType Collection */
    DFont_لیبل		= HB_لیبل ( 0 , 0 , 1 , 0 ), /* DFont Mac Resource Fork */
    True_لیبل		= HB_لیبل ('t','r','u','e'), /* Obsolete Apple TrueType */
    Typ1_لیبل		= HB_لیبل ('t','y','p','1')  /* Obsolete Apple Type1 font in SFNT container */
  };

  hb_لیبل_t get_لیبل () ثابت { واپس u.لیبل_2.v; }

  غیرعلامتی صحیح get_face_گنتی () ثابت
  {
    انتخاب (u.لیبل_2.v) {
    حالت CFFTag:	/* All the non-collection tags */
    حالت True_لیبل:
    حالت Typ1_لیبل:
    حالت True_Type_لیبل:	واپس 1;
    حالت TTCTag:	hb_barrier (); واپس u.ttc_سرخی.get_face_گنتی ();
    حالت DFont_لیبل:	hb_barrier (); واپس u.rf_سرخی.get_face_گنتی ();
    طےشدہ:		واپس 0;
    }
  }
  ثابت OpenTypeFontFace& get_face (غیرعلامتی صحیح i, غیرعلامتی صحیح *بنیاد_انحراف = خالی_اشاریہ) ثابت
  {
    اگر (بنیاد_انحراف)
      *بنیاد_انحراف = 0;
    انتخاب (u.لیبل_2.v) {
    /* Note: for non-collection SFNT data we ignore index.  This is because
     * Apple dfont container is a container of SFNT's.  So each SFNT is a
     * non-TTC, but the index is more than zero. */
    حالت CFFTag:	/* All the non-collection tags */
    حالت True_لیبل:
    حالت Typ1_لیبل:
    حالت True_Type_لیبل:	hb_barrier (); واپس u.fontFace;
    حالت TTCTag:	hb_barrier (); واپس u.ttc_سرخی.get_face (i);
    حالت DFont_لیبل:	hb_barrier (); واپس u.rf_سرخی.get_face (i, بنیاد_انحراف);
    طےشدہ:		واپس Null (OpenTypeFontFace);
    }
  }

  سانچہ <قسم_نام Iterator,
	    hb_requires ((hb_is_ماخذ_کا<Iterator, hb_pair_t<hb_لیبل_t, hb_blob_t *>>::value))>
  منطقی serialize_single (hb_serialize_context_t *c,
			 hb_لیبل_t sfnt_لیبل,
			 Iterator اشیاء)
  {
    TRACE_SERIALIZE (یہ);
    أكد (sfnt_لیبل != TTCTag);
    اگر (unlikely (!c->extend_min (یہ))) return_trace (جھوٹ);
    return_trace (u.fontFace.serialize (c, sfnt_لیبل, اشیاء));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    اگر (unlikely (!u.لیبل_2.v.sanitize (c))) return_trace (جھوٹ);
    hb_barrier ();
    انتخاب (u.لیبل_2.v) {
    حالت CFFTag:	/* All the non-collection tags */
    حالت True_لیبل:
    حالت Typ1_لیبل:
    حالت True_Type_لیبل:	hb_barrier (); return_trace (u.fontFace.sanitize (c));
    حالت TTCTag:	hb_barrier (); return_trace (u.ttc_سرخی.sanitize (c));
    حالت DFont_لیبل:	hb_barrier (); return_trace (u.rf_سرخی.sanitize (c));
    طےشدہ:		return_trace (سچ);
    }
  }

  محفوظ:
  اتحاد {
  ساخت { لیبل v; }	لیبل_2;		/* 4-byte identifier. */
  OpenTypeFontFace	fontFace;
  TTCHeader		ttc_سرخی;
  Resource_Fork_سرخی	rf_سرخی;
  } u;
  عوامی:
  DEFINE_سائز_UNION (4, لیبل_2.v);
};


} /* namespace OT */


#اگر_ختم /* HB_OPEN_FILE_HH */
