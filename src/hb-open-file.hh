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

#اگر_تعریف_نشده HB_OPEN_FILE_HH
#تعریف_کن HB_OPEN_FILE_HH

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
بنية Open_Type_جابه_جایی_جدول;
بنية TTCHeader;


تعريفنوع بنية جدول_رکورد
{
  صحيح cmp (برچسب t) ثابت { أرجع -t.cmp (برچسب_2); }

  HB_داخلی ساكن صحيح cmp (ثابت فراغ *pa, ثابت فراغ *pb)
  {
    ثابت جدول_رکورد *a = (ثابت جدول_رکورد *) pa;
    ثابت جدول_رکورد *b = (ثابت جدول_رکورد *) pb;
    أرجع b->cmp (a->برچسب_2);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  برچسب		برچسب_2;		/* 4-byte identifier. */
  Check_جمع	check_جمع;	/* CheckSum for this table. */
  Offset32	جابه_جایی;		/* Offset from beginning of TrueType font
				 * file. */
  HBUINT32	درازی;		/* Length of this table. */
  عام:
  DEFINE_اندازه_STATIC (16);
} Open_Type_جدول;

تعريفنوع بنية Open_Type_جابه_جایی_جدول
{
  دوست بنية OpenTypeFontFile;

  بیعلامت صحيح get_جدول_شمار () ثابت { أرجع tables.len; }
  ثابت جدول_رکورد& get_جدول (بیعلامت صحيح i) ثابت
  { أرجع tables[i]; }
  بیعلامت صحيح get_جدول_برچسبها (بیعلامت صحيح  شروع_جابه_جایی,
			       بیعلامت صحيح *جدول_شمار_2, /* IN/OUT */
			       hb_برچسب_t     *جدول_برچسبها /*بیرون*/) ثابت
  {
    إذا (جدول_شمار_2 && جدول_برچسبها)
    {
      + tables.as_array ().sub_array (شروع_جابه_جایی, جدول_شمار_2)
      | hb_map (&جدول_رکورد::برچسب_2)
      | hb_sink (hb_array (جدول_برچسبها, *جدول_شمار_2))
      ;
    }
    أرجع tables.len;
  }
  منطقي پیدا_کن_جدول_شاخص (hb_برچسب_t برچسب_2, بیعلامت صحيح *جدول_شاخص) ثابت
  {
    برچسب t;
    t = برچسب_2;
    /* Use lfind for small fonts; there are fonts that have unsorted table entries;
     * those tend to work in other tools, so tolerate them.
     * https://github.com/harfbuzz/harfbuzz/issues/3065 */
    إذا (tables.len < 16)
      أرجع tables.lfind (t, جدول_شاخص, HB_NOT_یافت_STORE, شاخص::NOT_یافت_شاخص);
    وإلا
      أرجع tables.bfind (t, جدول_شاخص, HB_NOT_یافت_STORE, شاخص::NOT_یافت_شاخص);
  }
  ثابت جدول_رکورد& get_جدول_by_برچسب (hb_برچسب_t برچسب_2) ثابت
  {
    بیعلامت صحيح جدول_شاخص;
    پیدا_کن_جدول_شاخص (برچسب_2, &جدول_شاخص);
    أرجع get_جدول (جدول_شاخص);
  }

  عام:

  قالب <نام_نوع Iterator,
	    hb_requires ((hb_is_منبع_از<Iterator, hb_pair_t<hb_برچسب_t, hb_blob_t *>>::value))>
  منطقي serialize (hb_serialize_context_t *c,
		  hb_برچسب_t sfnt_برچسب,
		  Iterator it)
  {
    TRACE_SERIALIZE (هذا);
    /* Alloc 12 for the OTHeader. */
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);
    /* Write sfntVersion (bytes 0..3). */
    sfnt_نسخه = sfnt_برچسب;
    /* Take space for numTables, searchRange, entrySelector, RangeShift
     * and the TableRecords themselves.  */
    بیعلامت رقم_اقلام = hb_len (it);
    إذا (unlikely (!tables.serialize (c, رقم_اقلام))) return_trace (خطأ);

    ثابت حرف *dir_انتها = (ثابت حرف *) c->سر_2;
    HBUINT32 *checksum_adjustment = اشاره_تهی;

    /* Write OffsetTables, alloc for and write actual table blobs. */
    بیعلامت i = 0;
    لكل (hb_pair_t<hb_برچسب_t, hb_blob_t*> مدخل : it)
    {
      hb_blob_t *blob = مدخل.second;
      بیعلامت len = blob->درازی;

      /* Allocate room for the table and copy it. */
      حرف *شروع = (حرف *) c->allocate_اندازه<فراغ> (len, خطأ);
      إذا (unlikely (!شروع)) أرجع خطأ;

      جدول_رکورد &rec = tables.arrayZ[i];
      rec.برچسب_2 = مدخل.اولی;
      rec.درازی = len;
      rec.جابه_جایی = 0;
      إذا (unlikely (!c->check_تخصیص_بده (rec.جابه_جایی,
				      (بیعلامت) ((حرف *) شروع - (حرف *) هذا),
				      HB_SERIALIZE_ERROR_جابه_جایی_سرریز)))
        return_trace (خطأ);

      إذا (likely (len))
	hb_memcpy (شروع, blob->داده, len);

      /* 4-byte alignment. */
      c->align (4);
      ثابت حرف *انتها = (ثابت حرف *) c->سر_2;

      إذا (مدخل.اولی == HB_OT_برچسب_سر &&
	  (بیعلامت) (انتها - شروع) >= سر_2::static_اندازه)
      {
	سر_2 *h = (سر_2 *) شروع;
	checksum_adjustment = &h->check_جمع_Adjustment;
	*checksum_adjustment = 0;
      }

      rec.check_جمع.مجموعه_for_داده (شروع, انتها - شروع);
      i++;
    }

    tables.qsort ();

    إذا (checksum_adjustment)
    {
      Check_جمع checksum;

      /* The following line is a slower version of the following block. */
      //checksum.set_for_data (this, (const char *) c->head - (const char *) this);
      checksum.مجموعه_for_داده (هذا, dir_انتها - (ثابت حرف *) هذا);
      لكل (بیعلامت صحيح i = 0; i < رقم_اقلام; i++)
      {
	جدول_رکورد &rec = tables.arrayZ[i];
	checksum = checksum + rec.check_جمع;
      }

      *checksum_adjustment = 0xB1B0AFBAu - checksum;
    }

    return_trace (درست);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && tables.sanitize (c));
  }

  محمي:
  برچسب		sfnt_نسخه;	/* '\0\001\0\00' if TrueType / 'OTTO' if CFF */
  Bin_جستجو_Array_از<جدول_رکورد>
		tables;
  عام:
  DEFINE_اندازه_ARRAY (12, tables);
} OpenTypeFontFace;


/*
 * TrueType Collections
 */

بنية TTCHeaderVersion1
{
  دوست بنية TTCHeader;

  استفاده DirectoryList = Array32_از<Offset32_مقصد<Open_Type_جابه_جایی_جدول>>;

  بیعلامت صحيح get_face_شمار () ثابت { أرجع get_directory_list ().len; }
  ثابت OpenTypeFontFace& get_face (بیعلامت صحيح i) ثابت
  { أرجع هذا+get_directory_list ()[i]; }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    إذا (نسخه.minor >= 1)
    {
      إذا (unlikely (!جدول.sanitize_shallow (c)))
	return_trace (خطأ);
      بیعلامت extra_اندازه = نسخه.major == 2 ? 12 : 0;
      إذا (unlikely (!c->check_range (&جدول, جدول.get_اندازه () + extra_اندازه)))
	return_trace (خطأ);
      ثابت DirectoryList &table2 = Struct_At_جابه_جایی<DirectoryList> (&جدول,
								   جدول.get_اندازه () + extra_اندازه);
      return_trace (table2.sanitize (c, هذا));
    }
#پایان_اگر
    return_trace (جدول.sanitize (c, هذا));
  }

  خاص:
  ثابت DirectoryList& get_directory_list () ثابت
  {
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    إذا (نسخه.minor >= 1)
    {
      بیعلامت extra_اندازه = نسخه.major == 2 ? 12 : 0;
      أرجع Struct_At_جابه_جایی<DirectoryList> (&جدول, جدول.get_اندازه () + extra_اندازه);
    }
#پایان_اگر
    أرجع جدول;
  }

  محمي:
  برچسب		ttc_برچسب;		/* TrueType Collection ID string: 'ttcf' */
  Fixed_نسخه<>نسخه;	/* Version of the TTC Header. */
  DirectoryList
		جدول;		/* Array of offsets to the OffsetTable for each font
				 * from the beginning of the file */
  عام:
  DEFINE_اندازه_ARRAY (12, جدول);
};

بنية TTCHeader
{
  دوست بنية OpenTypeFontFile;

  خاص:

  بیعلامت صحيح get_face_شمار () ثابت
  {
    بدل (u.سرآیند.نسخه.major) {
    حالة 2: /* version 2 is compatible with version 1 */
    حالة 1: hb_barrier (); أرجع u.version1.get_face_شمار ();
    افتراضي:أرجع 0;
    }
  }
  ثابت OpenTypeFontFace& get_face (بیعلامت صحيح i) ثابت
  {
    بدل (u.سرآیند.نسخه.major) {
    حالة 2: /* version 2 is compatible with version 1 */
    حالة 1: hb_barrier (); أرجع u.version1.get_face (i);
    افتراضي:أرجع Null (OpenTypeFontFace);
    }
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!u.سرآیند.نسخه.sanitize (c))) return_trace (خطأ);
    hb_barrier ();
    بدل (u.سرآیند.نسخه.major) {
    حالة 2: /* version 2 is compatible with version 1 */
    حالة 1: hb_barrier (); return_trace (u.version1.sanitize (c));
    افتراضي:return_trace (درست);
    }
  }

  محمي:
  اتحاد {
  بنية {
  برچسب		ttc_برچسب;		/* TrueType Collection ID string: 'ttcf' */
  Fixed_نسخه<>نسخه;	/* Version of the TTC Header (1.0 or 2.0),
				 * 0x00010000u or 0x00020000u */
  }			سرآیند;
  TTCHeaderVersion1	version1;
  } u;
};

/*
 * Mac Resource Fork
 *
 * http://mirror.informatimago.com/next/developer.apple.com/documentation/mac/MoreToolbox/MoreToolbox-99.html
 */

بنية Resource_رکورد
{
  ثابت OpenTypeFontFace & get_face (ثابت فراغ *داده_پایه) ثابت
  { أرجع * reinterpret_cast<ثابت OpenTypeFontFace *> ((داده_پایه+جابه_جایی).arrayZ); }

  منطقي sanitize (hb_sanitize_context_t *c,
		 ثابت فراغ *داده_پایه) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  جابه_جایی.sanitize (c, داده_پایه) &&
		  hb_barrier () &&
		  get_face (داده_پایه).sanitize (c));
  }

  محمي:
  HBUINT16	id;		/* Resource ID. */
  HBINT16	name_جابه_جایی;	/* Offset from beginning of resource name list
				 * to resource name, -1 means there is none. */
  HBUINT8	attrs;		/* Resource attributes */
  NNOffset24_مقصد<Array32_از<HBUINT8>>
		جابه_جایی;		/* Offset from beginning of data block to
				 * data for this resource */
  HBUINT32	reserved;	/* Reserved for handle to resource */
  عام:
  DEFINE_اندازه_STATIC (12);
};

#تعریف_کن HB_برچسب_sfnt HB_برچسب ('s','f','n','t')

بنية Resource_Type_رکورد
{
  بیعلامت صحيح get_resource_شمار () ثابت
  { أرجع برچسب_2 == HB_برچسب_sfnt ? res_شمار_M1 + 1 : 0; }

  منطقي is_sfnt () ثابت { أرجع برچسب_2 == HB_برچسب_sfnt; }

  ثابت Resource_رکورد& get_resource_رکورد (بیعلامت صحيح i,
					     ثابت فراغ *type_پایه) ثابت
  { أرجع (type_پایه+resourcesZ).as_array (get_resource_شمار ())[i]; }

  منطقي sanitize (hb_sanitize_context_t *c,
		 ثابت فراغ *type_پایه,
		 ثابت فراغ *داده_پایه) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  resourcesZ.sanitize (c, type_پایه,
				       get_resource_شمار (),
				       داده_پایه));
  }

  محمي:
  برچسب		برچسب_2;		/* Resource type. */
  HBUINT16	res_شمار_M1;	/* Number of resources minus 1. */
  NNOffset16_مقصد<Unsized_Array_از<Resource_رکورد>>
		resourcesZ;	/* Offset from beginning of resource type list
				 * to reference item list for this type. */
  عام:
  DEFINE_اندازه_STATIC (8);
};

بنية ResourceMap
{
  بیعلامت صحيح get_face_شمار () ثابت
  {
    بیعلامت صحيح شمار = get_type_شمار ();
    لكل (بیعلامت صحيح i = 0; i < شمار; i++)
    {
      ثابت Resource_Type_رکورد& type = get_type_رکورد (i);
      إذا (type.is_sfnt ())
	أرجع type.get_resource_شمار ();
    }
    أرجع 0;
  }

  ثابت OpenTypeFontFace& get_face (بیعلامت صحيح idx,
				    ثابت فراغ *داده_پایه) ثابت
  {
    بیعلامت صحيح شمار = get_type_شمار ();
    لكل (بیعلامت صحيح i = 0; i < شمار; i++)
    {
      ثابت Resource_Type_رکورد& type = get_type_رکورد (i);
      /* The check for idx < count is here because ResourceRecord is NOT null-safe.
       * Because an offset of 0 there does NOT mean null. */
      إذا (type.is_sfnt () && idx < type.get_resource_شمار ())
	أرجع type.get_resource_رکورد (idx, &(هذا+typeList)).get_face (داده_پایه);
    }
    أرجع Null (OpenTypeFontFace);
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *داده_پایه) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  typeList.sanitize (c, هذا,
				     &(هذا+typeList),
				     داده_پایه));
  }

  خاص:
  بیعلامت صحيح get_type_شمار () ثابت { أرجع (هذا+typeList).lenM1 + 1; }

  ثابت Resource_Type_رکورد& get_type_رکورد (بیعلامت صحيح i) ثابت
  { أرجع (هذا+typeList)[i]; }

  محمي:
  HBUINT8	reserved0[16];	/* Reserved for copy of resource header */
  HBUINT32	reserved1;	/* Reserved for handle to next resource map */
  HBUINT16	resreved2;	/* Reserved for file reference number */
  HBUINT16	attrs;		/* Resource fork attribute */
  NNOffset16_مقصد<Array_از_M1<Resource_Type_رکورد>>
		typeList;	/* Offset from beginning of map to
				 * resource type list */
  Offset16	nameList;	/* Offset from beginning of map to
				 * resource name list */
  عام:
  DEFINE_اندازه_STATIC (28);
};

بنية Resource_Fork_سرآیند
{
  بیعلامت صحيح get_face_شمار () ثابت
  { أرجع (هذا+map).get_face_شمار (); }

  ثابت OpenTypeFontFace& get_face (بیعلامت صحيح idx,
				    بیعلامت صحيح *پایه_جابه_جایی = اشاره_تهی) ثابت
  {
    ثابت OpenTypeFontFace &face = (هذا+map).get_face (idx, &(هذا+داده));
    إذا (پایه_جابه_جایی)
      *پایه_جابه_جایی = (ثابت حرف *) &face - (ثابت حرف *) هذا;
    أرجع face;
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  داده.sanitize (c, هذا, داده_Len) &&
		  map.sanitize (c, هذا, &(هذا+داده)));
  }

  محمي:
  NNOffset32_مقصد<Unsized_Array_از<HBUINT8>>
		داده;		/* Offset from beginning of resource fork
				 * to resource data */
  NNOffset32_مقصد<ResourceMap >
		map;		/* Offset from beginning of resource fork
				 * to resource map */
  HBUINT32	داده_Len;	/* Length of resource data */
  HBUINT32	mapLen;		/* Length of resource map */
  عام:
  DEFINE_اندازه_STATIC (16);
};

/*
 * OpenType Font File
 */

بنية OpenTypeFontFile
{
  تعداد {
    CFFTag		= HB_برچسب ('O','T','T','O'), /* OpenType with Postscript outlines */
    True_Type_برچسب		= HB_برچسب ( 0 , 1 , 0 , 0 ), /* OpenType with TrueType outlines */
    TTCTag		= HB_برچسب ('t','t','c','f'), /* TrueType Collection */
    DFont_برچسب		= HB_برچسب ( 0 , 0 , 1 , 0 ), /* DFont Mac Resource Fork */
    True_برچسب		= HB_برچسب ('t','r','u','e'), /* Obsolete Apple TrueType */
    Typ1_برچسب		= HB_برچسب ('t','y','p','1')  /* Obsolete Apple Type1 font in SFNT container */
  };

  hb_برچسب_t get_برچسب () ثابت { أرجع u.برچسب_2.v; }

  بیعلامت صحيح get_face_شمار () ثابت
  {
    بدل (u.برچسب_2.v) {
    حالة CFFTag:	/* All the non-collection tags */
    حالة True_برچسب:
    حالة Typ1_برچسب:
    حالة True_Type_برچسب:	أرجع 1;
    حالة TTCTag:	hb_barrier (); أرجع u.ttc_سرآیند.get_face_شمار ();
    حالة DFont_برچسب:	hb_barrier (); أرجع u.rf_سرآیند.get_face_شمار ();
    افتراضي:		أرجع 0;
    }
  }
  ثابت OpenTypeFontFace& get_face (بیعلامت صحيح i, بیعلامت صحيح *پایه_جابه_جایی = اشاره_تهی) ثابت
  {
    إذا (پایه_جابه_جایی)
      *پایه_جابه_جایی = 0;
    بدل (u.برچسب_2.v) {
    /* Note: for non-collection SFNT data we ignore index.  This is because
     * Apple dfont container is a container of SFNT's.  So each SFNT is a
     * non-TTC, but the index is more than zero. */
    حالة CFFTag:	/* All the non-collection tags */
    حالة True_برچسب:
    حالة Typ1_برچسب:
    حالة True_Type_برچسب:	hb_barrier (); أرجع u.fontFace;
    حالة TTCTag:	hb_barrier (); أرجع u.ttc_سرآیند.get_face (i);
    حالة DFont_برچسب:	hb_barrier (); أرجع u.rf_سرآیند.get_face (i, پایه_جابه_جایی);
    افتراضي:		أرجع Null (OpenTypeFontFace);
    }
  }

  قالب <نام_نوع Iterator,
	    hb_requires ((hb_is_منبع_از<Iterator, hb_pair_t<hb_برچسب_t, hb_blob_t *>>::value))>
  منطقي serialize_single (hb_serialize_context_t *c,
			 hb_برچسب_t sfnt_برچسب,
			 Iterator اقلام)
  {
    TRACE_SERIALIZE (هذا);
    أكد (sfnt_برچسب != TTCTag);
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);
    return_trace (u.fontFace.serialize (c, sfnt_برچسب, اقلام));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!u.برچسب_2.v.sanitize (c))) return_trace (خطأ);
    hb_barrier ();
    بدل (u.برچسب_2.v) {
    حالة CFFTag:	/* All the non-collection tags */
    حالة True_برچسب:
    حالة Typ1_برچسب:
    حالة True_Type_برچسب:	hb_barrier (); return_trace (u.fontFace.sanitize (c));
    حالة TTCTag:	hb_barrier (); return_trace (u.ttc_سرآیند.sanitize (c));
    حالة DFont_برچسب:	hb_barrier (); return_trace (u.rf_سرآیند.sanitize (c));
    افتراضي:		return_trace (درست);
    }
  }

  محمي:
  اتحاد {
  بنية { برچسب v; }	برچسب_2;		/* 4-byte identifier. */
  OpenTypeFontFace	fontFace;
  TTCHeader		ttc_سرآیند;
  Resource_Fork_سرآیند	rf_سرآیند;
  } u;
  عام:
  DEFINE_اندازه_UNION (4, برچسب_2.v);
};


} /* namespace OT */


#پایان_اگر /* HB_OPEN_FILE_HH */
