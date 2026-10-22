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
  hb_blob_t* انکڑے;
  بنانشانی ترتیب;
};

بنية hb_face_builder_انکڑے_t
{
  hb_hashmap_t<hb_ٹیگ_t, face_جدول_info_t> tables;
};

ساكن صحيح compare_entries (ثابت فراغ* pa, ثابت فراغ* pb)
{
  ثابت خودکار& a = * (ثابت hb_pair_t<hb_ٹیگ_t, face_جدول_info_t> *) pa;
  ثابت خودکار& b = * (ثابت hb_pair_t<hb_ٹیگ_t, face_جدول_info_t> *) pb;

  /* Order by blob size first (smallest to largest) and then table tag */

  إذا (a.second.ترتیب != b.second.ترتیب)
    أرجع a.second.ترتیب < b.second.ترتیب ? -1 : +1;

  إذا (a.second.انکڑے->لمائی != b.second.انکڑے->لمائی)
    أرجع a.second.انکڑے->لمائی < b.second.انکڑے->لمائی ? -1 : +1;

  أرجع a.پہلا < b.پہلا ? -1 : a.پہلا == b.پہلا ? 0 : +1;
}

ساكن hb_face_builder_انکڑے_t *
hb_face_builder_انکڑے_create ()
{
  hb_face_builder_انکڑے_t *انکڑے = (hb_face_builder_انکڑے_t *) hb_calloc (1, حجم (hb_face_builder_انکڑے_t));
  إذا (unlikely (!انکڑے))
    أرجع خالی_سوچک;

  انکڑے->tables.init ();

  أرجع انکڑے;
}

ساكن فراغ
hb_face_builder_انکڑے_destroy (فراغ *user_انکڑے)
{
  hb_face_builder_انکڑے_t *انکڑے = (hb_face_builder_انکڑے_t *) user_انکڑے;

  لكل (خودکار info : انکڑے->tables.values())
    hb_blob_destroy (info.انکڑے);

  انکڑے->tables.fini ();

  hb_free (انکڑے);
}

ساكن hb_blob_t *
hb_face_builder_انکڑے_reference_blob (hb_face_builder_انکڑے_t *انکڑے)
{

  بنانشانی صحيح جدول_گنتی_2 = انکڑے->tables.get_population ();
  بنانشانی صحيح face_لمائی = جدول_گنتی_2 * 16 + 12;

  لكل (خودکار info : انکڑے->tables.values())
    face_لمائی += hb_ceil_منزل_4 (hb_blob_get_لمائی (info.انکڑے));

  حرف *buf = (حرف *) hb_malloc (face_لمائی);
  إذا (unlikely (!buf))
    أرجع خالی_سوچک;

  hb_serialize_context_t c (buf, face_لمائی);
  c.propagate_error (انکڑے->tables);
  OT::OpenTypeFontFile *f = c.شروع_serialize<OT::OpenTypeFontFile> ();

  منطقي is_cff = (انکڑے->tables.has (HB_ٹیگ ('C','F','F',' '))
                 || انکڑے->tables.has (HB_ٹیگ ('C','F','F','2')));
  hb_ٹیگ_t sfnt_ٹیگ = is_cff ? OT::OpenTypeFontFile::CFFTag : OT::OpenTypeFontFile::True_Type_ٹیگ;

  // Sort the tags so that produced face is deterministic.
  hb_سمتیہ_t<hb_pair_t <hb_ٹیگ_t, face_جدول_info_t>> sorted_entries;
  انکڑے->tables.iter () | hb_sink (sorted_entries);
  إذا (unlikely (sorted_entries.وچ_error ()))
  {
    hb_free (buf);
    أرجع خالی_سوچک;
  }

  sorted_entries.qsort (compare_entries);

  منطقي ret = f->serialize_single (&c,
                                  sfnt_ٹیگ,
                                  + sorted_entries.iter()
                                  | hb_map ([&] (hb_pair_t<hb_ٹیگ_t, face_جدول_info_t> _) {
                                    أرجع hb_pair_t<hb_ٹیگ_t, hb_blob_t*> (_.پہلا, _.second.انکڑے);
                                  }));

  c.سرا_serialize ();

  إذا (unlikely (!ret))
  {
    hb_free (buf);
    أرجع خالی_سوچک;
  }

  أرجع hb_blob_create (buf, face_لمائی, HB_حافظہ_منوال_WRITABLE, buf, hb_free);
}

ساكن hb_blob_t *
hb_face_builder_reference_جدول (hb_face_t *face HB_بن_ورتیا, hb_ٹیگ_t ٹیگ_2, فراغ *user_انکڑے)
{
  hb_face_builder_انکڑے_t *انکڑے = (hb_face_builder_انکڑے_t *) user_انکڑے;

  إذا (!ٹیگ_2)
    أرجع hb_face_builder_انکڑے_reference_blob (انکڑے);

  أرجع hb_blob_reference (انکڑے->tables[ٹیگ_2].انکڑے);
}

ساكن بنانشانی
hb_face_builder_get_جدول_ٹیگ_سوچی (ثابت hb_face_t *face HB_بن_ورتیا,
				 بنانشانی صحيح شروع_وتھ,
				 بنانشانی صحيح *جدول_گنتی_2,
				 hb_ٹیگ_t *جدول_ٹیگ_سوچی,
				 فراغ *user_انکڑے)
{
  hb_face_builder_انکڑے_t *انکڑے = (hb_face_builder_انکڑے_t *) user_انکڑے;

  بنانشانی population = انکڑے->tables.get_population ();

  إذا (!جدول_گنتی_2)
    أرجع population;

  إذا (unlikely (شروع_وتھ >= population))
  {
    *جدول_گنتی_2 = 0;
    أرجع population;
  }

  // Sort the tags.
  hb_سمتیہ_t<hb_ٹیگ_t> sorted_ٹیگ_سوچی;
  انکڑے->tables.کلیدیں () | hb_sink (sorted_ٹیگ_سوچی);
  إذا (unlikely (sorted_ٹیگ_سوچی.وچ_error ()))
  {
    // Not much to do...
  }
  sorted_ٹیگ_سوچی.qsort ([] (ثابت hb_ٹیگ_t &a, ثابت hb_ٹیگ_t &b) {
    أرجع (a > b) - (a < b);
  });

  خودکار array = sorted_ٹیگ_سوچی.as_array ().sub_array (شروع_وتھ, جدول_گنتی_2);
  إذا (جدول_ٹیگ_سوچی)
  {
    خودکار باہرلا = hb_array (جدول_ٹیگ_سوچی, *جدول_گنتی_2);

    + array.iter ()
    | hb_sink (باہرلا)
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
  hb_face_builder_انکڑے_t *انکڑے = hb_face_builder_انکڑے_create ();
  إذا (unlikely (!انکڑے)) أرجع hb_face_get_خالی_تھاواں ();

  hb_face_t *face = hb_face_create_for_tables (hb_face_builder_reference_جدول,
					       انکڑے,
					       hb_face_builder_انکڑے_destroy);

  hb_face_سیٹ_get_جدول_ٹیگ_سوچی_func (face,
				   hb_face_builder_get_جدول_ٹیگ_سوچی,
				   انکڑے,
				   خالی_سوچک);

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
hb_face_builder_جوڑو_جدول (hb_face_t *face, hb_ٹیگ_t ٹیگ_2, hb_blob_t *blob)
{
  إذا (unlikely (face->destroy != (hb_destroy_func_t) hb_face_builder_انکڑے_destroy))
    أرجع خطأ;

  إذا (ٹیگ_2 == HB_MAP_قدر_INVALID)
    أرجع خطأ;

  hb_face_builder_انکڑے_t *انکڑے = (hb_face_builder_انکڑے_t *) face->user_انکڑے;

  hb_blob_t* پچھلا = انکڑے->tables.get (ٹیگ_2).انکڑے;
  إذا (!انکڑے->tables.سیٹ (ٹیگ_2, face_جدول_info_t {hb_blob_reference (blob), (بنانشانی) -1}))
  {
    hb_blob_destroy (blob);
    أرجع خطأ;
  }

  hb_blob_destroy (پچھلا);
  أرجع سچ;
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
hb_face_builder_ترتیب_دیو_tables (hb_face_t *face,
                             ثابت hb_ٹیگ_t  *ٹیگ_سوچی)
{
  إذا (unlikely (face->destroy != (hb_destroy_func_t) hb_face_builder_انکڑے_destroy))
    أرجع;

  hb_face_builder_انکڑے_t *انکڑے = (hb_face_builder_انکڑے_t *) face->user_انکڑے;

  // Sort all unspecified tables after any specified tables.
  لكل (خودکار& info : انکڑے->tables.اقدار_ref())
    info.ترتیب = (بنانشانی) -1;

  نشانیوالا ترتیب = 0;
  لكل (ثابت hb_ٹیگ_t* ٹیگ_2 = ٹیگ_سوچی;
       *ٹیگ_2;
       ٹیگ_2++)
  {
    face_جدول_info_t* info;
    إذا (!انکڑے->tables.has (*ٹیگ_2, &info)) استمر;
    info->ترتیب = ترتیب++;
  }
}
