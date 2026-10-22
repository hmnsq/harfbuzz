/*
 * Copyright © 2009  Red Hat, Inc.
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

#ضمن "hb.hh"

#ضمن "hb-face.hh"

#ضمن "hb-map.hh"
#ضمن "hb-open-file.hh"
#ضمن "hb-serialize.hh"


/*
 * face-builder: A face that has add_table().
 */

بنية face_جدول_info_t
{
  hb_blob_t* داده;
  بیعلامت ترتیب;
};

بنية hb_face_builder_داده_t
{
  hb_hashmap_t<hb_برچسب_t, face_جدول_info_t> tables;
};

ساكن صحيح compare_entries (ثابت فراغ* pa, ثابت فراغ* pb)
{
  ثابت خودکار& a = * (ثابت hb_pair_t<hb_برچسب_t, face_جدول_info_t> *) pa;
  ثابت خودکار& b = * (ثابت hb_pair_t<hb_برچسب_t, face_جدول_info_t> *) pb;

  /* Order by blob size first (smallest to largest) and then table tag */

  إذا (a.second.ترتیب != b.second.ترتیب)
    أرجع a.second.ترتیب < b.second.ترتیب ? -1 : +1;

  إذا (a.second.داده->درازی != b.second.داده->درازی)
    أرجع a.second.داده->درازی < b.second.داده->درازی ? -1 : +1;

  أرجع a.اولی < b.اولی ? -1 : a.اولی == b.اولی ? 0 : +1;
}

ساكن hb_face_builder_داده_t *
hb_face_builder_داده_create ()
{
  hb_face_builder_داده_t *داده = (hb_face_builder_داده_t *) hb_calloc (1, حجم (hb_face_builder_داده_t));
  إذا (unlikely (!داده))
    أرجع اشاره_تهی;

  داده->tables.init ();

  أرجع داده;
}

ساكن فراغ
hb_face_builder_داده_destroy (فراغ *user_داده)
{
  hb_face_builder_داده_t *داده = (hb_face_builder_داده_t *) user_داده;

  لكل (خودکار info : داده->tables.values())
    hb_blob_destroy (info.داده);

  داده->tables.fini ();

  hb_free (داده);
}

ساكن hb_blob_t *
hb_face_builder_داده_reference_blob (hb_face_builder_داده_t *داده)
{

  بیعلامت صحيح جدول_شمار_2 = داده->tables.get_population ();
  بیعلامت صحيح face_درازی = جدول_شمار_2 * 16 + 12;

  لكل (خودکار info : داده->tables.values())
    face_درازی += hb_ceil_مقصد_4 (hb_blob_get_درازی (info.داده));

  حرف *buf = (حرف *) hb_malloc (face_درازی);
  إذا (unlikely (!buf))
    أرجع اشاره_تهی;

  hb_serialize_context_t c (buf, face_درازی);
  c.propagate_error (داده->tables);
  OT::OpenTypeFontFile *f = c.شروع_serialize<OT::OpenTypeFontFile> ();

  منطقي is_cff = (داده->tables.has (HB_برچسب ('C','F','F',' '))
                 || داده->tables.has (HB_برچسب ('C','F','F','2')));
  hb_برچسب_t sfnt_برچسب = is_cff ? OT::OpenTypeFontFile::CFFTag : OT::OpenTypeFontFile::True_Type_برچسب;

  // Sort the tags so that produced face is deterministic.
  hb_وکتور_t<hb_pair_t <hb_برچسب_t, face_جدول_info_t>> sorted_entries;
  داده->tables.iter () | hb_sink (sorted_entries);
  إذا (unlikely (sorted_entries.درون_error ()))
  {
    hb_free (buf);
    أرجع اشاره_تهی;
  }

  sorted_entries.qsort (compare_entries);

  منطقي ret = f->serialize_single (&c,
                                  sfnt_برچسب,
                                  + sorted_entries.iter()
                                  | hb_map ([&] (hb_pair_t<hb_برچسب_t, face_جدول_info_t> _) {
                                    أرجع hb_pair_t<hb_برچسب_t, hb_blob_t*> (_.اولی, _.second.داده);
                                  }));

  c.انتها_serialize ();

  إذا (unlikely (!ret))
  {
    hb_free (buf);
    أرجع اشاره_تهی;
  }

  أرجع hb_blob_create (buf, face_درازی, HB_حافظه_نما_WRITABLE, buf, hb_free);
}

ساكن hb_blob_t *
hb_face_builder_reference_جدول (hb_face_t *face HB_استفادهنشده, hb_برچسب_t برچسب_2, فراغ *user_داده)
{
  hb_face_builder_داده_t *داده = (hb_face_builder_داده_t *) user_داده;

  إذا (!برچسب_2)
    أرجع hb_face_builder_داده_reference_blob (داده);

  أرجع hb_blob_reference (داده->tables[برچسب_2].داده);
}

ساكن بیعلامت
hb_face_builder_get_جدول_برچسبها (ثابت hb_face_t *face HB_استفادهنشده,
				 بیعلامت صحيح شروع_جابه_جایی,
				 بیعلامت صحيح *جدول_شمار_2,
				 hb_برچسب_t *جدول_برچسبها,
				 فراغ *user_داده)
{
  hb_face_builder_داده_t *داده = (hb_face_builder_داده_t *) user_داده;

  بیعلامت population = داده->tables.get_population ();

  إذا (!جدول_شمار_2)
    أرجع population;

  إذا (unlikely (شروع_جابه_جایی >= population))
  {
    *جدول_شمار_2 = 0;
    أرجع population;
  }

  // Sort the tags.
  hb_وکتور_t<hb_برچسب_t> sorted_برچسبها;
  داده->tables.کلیدها () | hb_sink (sorted_برچسبها);
  إذا (unlikely (sorted_برچسبها.درون_error ()))
  {
    // Not much to do...
  }
  sorted_برچسبها.qsort ([] (ثابت hb_برچسب_t &a, ثابت hb_برچسب_t &b) {
    أرجع (a > b) - (a < b);
  });

  خودکار array = sorted_برچسبها.as_array ().sub_array (شروع_جابه_جایی, جدول_شمار_2);
  إذا (جدول_برچسبها)
  {
    خودکار بیرون = hb_array (جدول_برچسبها, *جدول_شمار_2);

    + array.iter ()
    | hb_sink (بیرون)
    ;
  }

  أرجع population;
}


/**
 * hb_face_builder_create:
 *
 * Creates a #hb_face_t that can be used with hb_face_builder_add_table().
 * After tables are added to the face, it can be compiled to a binary
 * font file by calling hb_face_reference_blob().
 *
 * Return value: (transfer full): New face.
 *
 * Since: 1.9.0
 **/
hb_face_t *
hb_face_builder_create ()
{
  hb_face_builder_داده_t *داده = hb_face_builder_داده_create ();
  إذا (unlikely (!داده)) أرجع hb_face_get_خالی ();

  hb_face_t *face = hb_face_create_for_tables (hb_face_builder_reference_جدول,
					       داده,
					       hb_face_builder_داده_destroy);

  hb_face_مجموعه_get_جدول_برچسبها_func (face,
				   hb_face_builder_get_جدول_برچسبها,
				   داده,
				   اشاره_تهی);

  أرجع face;
}

/**
 * hb_face_builder_add_table:
 * @face: A face object created with hb_face_builder_create()
 * @tag: The #hb_tag_t of the table to add
 * @blob: The blob containing the table data to add
 *
 * Add table for @tag with data provided by @blob to the face.  @face must
 * be created using hb_face_builder_create().
 *
 * Since: 1.9.0
 **/
hb_bool_t
hb_face_builder_جمع_کن_جدول (hb_face_t *face, hb_برچسب_t برچسب_2, hb_blob_t *blob)
{
  إذا (unlikely (face->destroy != (hb_destroy_func_t) hb_face_builder_داده_destroy))
    أرجع خطأ;

  إذا (برچسب_2 == HB_MAP_مقدار_INVALID)
    أرجع خطأ;

  hb_face_builder_داده_t *داده = (hb_face_builder_داده_t *) face->user_داده;

  hb_blob_t* قبلی = داده->tables.get (برچسب_2).داده;
  إذا (!داده->tables.مجموعه (برچسب_2, face_جدول_info_t {hb_blob_reference (blob), (بیعلامت) -1}))
  {
    hb_blob_destroy (blob);
    أرجع خطأ;
  }

  hb_blob_destroy (قبلی);
  أرجع درست;
}

/**
 * hb_face_builder_sort_tables:
 * @face: A face object created with hb_face_builder_create()
 * @tags: (array zero-terminated=1): ordered list of table tags terminated by
 *   %HB_TAG_NONE
 *
 * Set the ordering of tables for serialization. Any tables not
 * specified in the tags list will be ordered after the tables in
 * tags, ordered by the default sort ordering.
 *
 * Since: 5.3.0
 **/
فراغ
hb_face_builder_مرتب_سازی_tables (hb_face_t *face,
                             ثابت hb_برچسب_t  *برچسبها)
{
  إذا (unlikely (face->destroy != (hb_destroy_func_t) hb_face_builder_داده_destroy))
    أرجع;

  hb_face_builder_داده_t *داده = (hb_face_builder_داده_t *) face->user_داده;

  // Sort all unspecified tables after any specified tables.
  لكل (خودکار& info : داده->tables.مقادیر_ref())
    info.ترتیب = (بیعلامت) -1;

  علامتدار ترتیب = 0;
  لكل (ثابت hb_برچسب_t* برچسب_2 = برچسبها;
       *برچسب_2;
       برچسب_2++)
  {
    face_جدول_info_t* info;
    إذا (!داده->tables.has (*برچسب_2, &info)) استمر;
    info->ترتیب = ترتیب++;
  }
}
