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

#شامل "hb.hh"

#شامل "hb-face.hh"

#شامل "hb-map.hh"
#شامل "hb-open-file.hh"
#شامل "hb-serialize.hh"


/*
 * face-builder: A face that has add_table().
 */

ساخت face_جدول_info_t
{
  hb_blob_t* ڈیٹا;
  غیرعلامتی ترتیب;
};

ساخت hb_face_builder_ڈیٹا_t
{
  hb_hashmap_t<hb_لیبل_t, face_جدول_info_t> tables;
};

ساکن صحیح compare_entries (ثابت خالی* pa, ثابت خالی* pb)
{
  ثابت خودکار& a = * (ثابت hb_pair_t<hb_لیبل_t, face_جدول_info_t> *) pa;
  ثابت خودکار& b = * (ثابت hb_pair_t<hb_لیبل_t, face_جدول_info_t> *) pb;

  /* Order by blob size first (smallest to largest) and then table tag */

  اگر (a.second.ترتیب != b.second.ترتیب)
    واپس a.second.ترتیب < b.second.ترتیب ? -1 : +1;

  اگر (a.second.ڈیٹا->لمبائی != b.second.ڈیٹا->لمبائی)
    واپس a.second.ڈیٹا->لمبائی < b.second.ڈیٹا->لمبائی ? -1 : +1;

  واپس a.پہلا < b.پہلا ? -1 : a.پہلا == b.پہلا ? 0 : +1;
}

ساکن hb_face_builder_ڈیٹا_t *
hb_face_builder_ڈیٹا_create ()
{
  hb_face_builder_ڈیٹا_t *ڈیٹا = (hb_face_builder_ڈیٹا_t *) hb_calloc (1, حجم (hb_face_builder_ڈیٹا_t));
  اگر (unlikely (!ڈیٹا))
    واپس خالی_اشاریہ;

  ڈیٹا->tables.init ();

  واپس ڈیٹا;
}

ساکن خالی
hb_face_builder_ڈیٹا_destroy (خالی *user_ڈیٹا)
{
  hb_face_builder_ڈیٹا_t *ڈیٹا = (hb_face_builder_ڈیٹا_t *) user_ڈیٹا;

  برائے (خودکار info : ڈیٹا->tables.values())
    hb_blob_destroy (info.ڈیٹا);

  ڈیٹا->tables.fini ();

  hb_free (ڈیٹا);
}

ساکن hb_blob_t *
hb_face_builder_ڈیٹا_reference_blob (hb_face_builder_ڈیٹا_t *ڈیٹا)
{

  غیرعلامتی صحیح جدول_گنتی_2 = ڈیٹا->tables.get_population ();
  غیرعلامتی صحیح face_لمبائی = جدول_گنتی_2 * 16 + 12;

  برائے (خودکار info : ڈیٹا->tables.values())
    face_لمبائی += hb_ceil_مقصد_4 (hb_blob_get_لمبائی (info.ڈیٹا));

  حرف *buf = (حرف *) hb_malloc (face_لمبائی);
  اگر (unlikely (!buf))
    واپس خالی_اشاریہ;

  hb_serialize_context_t c (buf, face_لمبائی);
  c.propagate_error (ڈیٹا->tables);
  OT::OpenTypeFontFile *f = c.آغاز_serialize<OT::OpenTypeFontFile> ();

  منطقی is_cff = (ڈیٹا->tables.has (HB_لیبل ('C','F','F',' '))
                 || ڈیٹا->tables.has (HB_لیبل ('C','F','F','2')));
  hb_لیبل_t sfnt_لیبل = is_cff ? OT::OpenTypeFontFile::CFFTag : OT::OpenTypeFontFile::True_Type_لیبل;

  // Sort the tags so that produced face is deterministic.
  hb_سمتیہ_t<hb_pair_t <hb_لیبل_t, face_جدول_info_t>> sorted_entries;
  ڈیٹا->tables.iter () | hb_sink (sorted_entries);
  اگر (unlikely (sorted_entries.اندر_error ()))
  {
    hb_free (buf);
    واپس خالی_اشاریہ;
  }

  sorted_entries.qsort (compare_entries);

  منطقی ret = f->serialize_single (&c,
                                  sfnt_لیبل,
                                  + sorted_entries.iter()
                                  | hb_map ([&] (hb_pair_t<hb_لیبل_t, face_جدول_info_t> _) {
                                    واپس hb_pair_t<hb_لیبل_t, hb_blob_t*> (_.پہلا, _.second.ڈیٹا);
                                  }));

  c.انتہا_serialize ();

  اگر (unlikely (!ret))
  {
    hb_free (buf);
    واپس خالی_اشاریہ;
  }

  واپس hb_blob_create (buf, face_لمبائی, HB_حافظہ_منوال_WRITABLE, buf, hb_free);
}

ساکن hb_blob_t *
hb_face_builder_reference_جدول (hb_face_t *face HB_غیر_مستعمل, hb_لیبل_t لیبل_2, خالی *user_ڈیٹا)
{
  hb_face_builder_ڈیٹا_t *ڈیٹا = (hb_face_builder_ڈیٹا_t *) user_ڈیٹا;

  اگر (!لیبل_2)
    واپس hb_face_builder_ڈیٹا_reference_blob (ڈیٹا);

  واپس hb_blob_reference (ڈیٹا->tables[لیبل_2].ڈیٹا);
}

ساکن غیرعلامتی
hb_face_builder_get_جدول_لیبل_فہرست (ثابت hb_face_t *face HB_غیر_مستعمل,
				 غیرعلامتی صحیح آغاز_انحراف,
				 غیرعلامتی صحیح *جدول_گنتی_2,
				 hb_لیبل_t *جدول_لیبل_فہرست,
				 خالی *user_ڈیٹا)
{
  hb_face_builder_ڈیٹا_t *ڈیٹا = (hb_face_builder_ڈیٹا_t *) user_ڈیٹا;

  غیرعلامتی population = ڈیٹا->tables.get_population ();

  اگر (!جدول_گنتی_2)
    واپس population;

  اگر (unlikely (آغاز_انحراف >= population))
  {
    *جدول_گنتی_2 = 0;
    واپس population;
  }

  // Sort the tags.
  hb_سمتیہ_t<hb_لیبل_t> sorted_لیبل_فہرست;
  ڈیٹا->tables.کلیدیں () | hb_sink (sorted_لیبل_فہرست);
  اگر (unlikely (sorted_لیبل_فہرست.اندر_error ()))
  {
    // Not much to do...
  }
  sorted_لیبل_فہرست.qsort ([] (ثابت hb_لیبل_t &a, ثابت hb_لیبل_t &b) {
    واپس (a > b) - (a < b);
  });

  خودکار array = sorted_لیبل_فہرست.as_array ().sub_array (آغاز_انحراف, جدول_گنتی_2);
  اگر (جدول_لیبل_فہرست)
  {
    خودکار برآمد = hb_array (جدول_لیبل_فہرست, *جدول_گنتی_2);

    + array.iter ()
    | hb_sink (برآمد)
    ;
  }

  واپس population;
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
  hb_face_builder_ڈیٹا_t *ڈیٹا = hb_face_builder_ڈیٹا_create ();
  اگر (unlikely (!ڈیٹا)) واپس hb_face_get_خالی_خانے ();

  hb_face_t *face = hb_face_create_for_tables (hb_face_builder_reference_جدول,
					       ڈیٹا,
					       hb_face_builder_ڈیٹا_destroy);

  hb_face_سیٹ_get_جدول_لیبل_فہرست_func (face,
				   hb_face_builder_get_جدول_لیبل_فہرست,
				   ڈیٹا,
				   خالی_اشاریہ);

  واپس face;
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
hb_face_builder_جوڑو_جدول (hb_face_t *face, hb_لیبل_t لیبل_2, hb_blob_t *blob)
{
  اگر (unlikely (face->destroy != (hb_destroy_func_t) hb_face_builder_ڈیٹا_destroy))
    واپس جھوٹ;

  اگر (لیبل_2 == HB_MAP_قدر_INVALID)
    واپس جھوٹ;

  hb_face_builder_ڈیٹا_t *ڈیٹا = (hb_face_builder_ڈیٹا_t *) face->user_ڈیٹا;

  hb_blob_t* پچھلا = ڈیٹا->tables.get (لیبل_2).ڈیٹا;
  اگر (!ڈیٹا->tables.سیٹ (لیبل_2, face_جدول_info_t {hb_blob_reference (blob), (غیرعلامتی) -1}))
  {
    hb_blob_destroy (blob);
    واپس جھوٹ;
  }

  hb_blob_destroy (پچھلا);
  واپس سچ;
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
خالی
hb_face_builder_ترتیب_دو_tables (hb_face_t *face,
                             ثابت hb_لیبل_t  *لیبل_فہرست)
{
  اگر (unlikely (face->destroy != (hb_destroy_func_t) hb_face_builder_ڈیٹا_destroy))
    واپس;

  hb_face_builder_ڈیٹا_t *ڈیٹا = (hb_face_builder_ڈیٹا_t *) face->user_ڈیٹا;

  // Sort all unspecified tables after any specified tables.
  برائے (خودکار& info : ڈیٹا->tables.اقدار_ref())
    info.ترتیب = (غیرعلامتی) -1;

  علامتی ترتیب = 0;
  برائے (ثابت hb_لیبل_t* لیبل_2 = لیبل_فہرست;
       *لیبل_2;
       لیبل_2++)
  {
    face_جدول_info_t* info;
    اگر (!ڈیٹا->tables.has (*لیبل_2, &info)) جاری;
    info->ترتیب = ترتیب++;
  }
}
