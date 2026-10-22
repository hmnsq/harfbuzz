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
  hb_blob_t* معلومات;
  بینښه ترتیب;
};

بنية hb_face_builder_معلومات_t
{
  hb_hashmap_t<hb_نښان_t, face_جدول_info_t> tables;
};

ساكن صحيح compare_entries (ثابت فراغ* pa, ثابت فراغ* pb)
{
  ثابت خپلکاری& a = * (ثابت hb_pair_t<hb_نښان_t, face_جدول_info_t> *) pa;
  ثابت خپلکاری& b = * (ثابت hb_pair_t<hb_نښان_t, face_جدول_info_t> *) pb;

  /* Order by blob size first (smallest to largest) and then table tag */

  إذا (a.second.ترتیب != b.second.ترتیب)
    أرجع a.second.ترتیب < b.second.ترتیب ? -1 : +1;

  إذا (a.second.معلومات->اوږدتیا != b.second.معلومات->اوږدتیا)
    أرجع a.second.معلومات->اوږدتیا < b.second.معلومات->اوږدتیا ? -1 : +1;

  أرجع a.لومړی < b.لومړی ? -1 : a.لومړی == b.لومړی ? 0 : +1;
}

ساكن hb_face_builder_معلومات_t *
hb_face_builder_معلومات_create ()
{
  hb_face_builder_معلومات_t *معلومات = (hb_face_builder_معلومات_t *) hb_calloc (1, حجم (hb_face_builder_معلومات_t));
  إذا (unlikely (!معلومات))
    أرجع تش_اشاره;

  معلومات->tables.init ();

  أرجع معلومات;
}

ساكن فراغ
hb_face_builder_معلومات_destroy (فراغ *user_معلومات)
{
  hb_face_builder_معلومات_t *معلومات = (hb_face_builder_معلومات_t *) user_معلومات;

  لكل (خپلکاری info : معلومات->tables.values())
    hb_blob_destroy (info.معلومات);

  معلومات->tables.fini ();

  hb_free (معلومات);
}

ساكن hb_blob_t *
hb_face_builder_معلومات_reference_blob (hb_face_builder_معلومات_t *معلومات)
{

  بینښه صحيح جدول_شمېر_2 = معلومات->tables.get_population ();
  بینښه صحيح face_اوږدتیا = جدول_شمېر_2 * 16 + 12;

  لكل (خپلکاری info : معلومات->tables.values())
    face_اوږدتیا += hb_ceil_منزل_4 (hb_blob_get_اوږدتیا (info.معلومات));

  حرف *buf = (حرف *) hb_malloc (face_اوږدتیا);
  إذا (unlikely (!buf))
    أرجع تش_اشاره;

  hb_serialize_context_t c (buf, face_اوږدتیا);
  c.propagate_error (معلومات->tables);
  OT::OpenTypeFontFile *f = c.پیل_serialize<OT::OpenTypeFontFile> ();

  منطقي is_cff = (معلومات->tables.has (HB_نښان ('C','F','F',' '))
                 || معلومات->tables.has (HB_نښان ('C','F','F','2')));
  hb_نښان_t sfnt_نښان = is_cff ? OT::OpenTypeFontFile::CFFTag : OT::OpenTypeFontFile::True_Type_نښان;

  // Sort the tags so that produced face is deterministic.
  hb_ویکټور_t<hb_pair_t <hb_نښان_t, face_جدول_info_t>> sorted_entries;
  معلومات->tables.iter () | hb_sink (sorted_entries);
  إذا (unlikely (sorted_entries.دننه_error ()))
  {
    hb_free (buf);
    أرجع تش_اشاره;
  }

  sorted_entries.qsort (compare_entries);

  منطقي ret = f->serialize_single (&c,
                                  sfnt_نښان,
                                  + sorted_entries.iter()
                                  | hb_map ([&] (hb_pair_t<hb_نښان_t, face_جدول_info_t> _) {
                                    أرجع hb_pair_t<hb_نښان_t, hb_blob_t*> (_.لومړی, _.second.معلومات);
                                  }));

  c.پای_ټکی_serialize ();

  إذا (unlikely (!ret))
  {
    hb_free (buf);
    أرجع تش_اشاره;
  }

  أرجع hb_blob_create (buf, face_اوږدتیا, HB_حافظه_ډېرپېښ_WRITABLE, buf, hb_free);
}

ساكن hb_blob_t *
hb_face_builder_reference_جدول (hb_face_t *face HB_ناکارول_شوی, hb_نښان_t نښان_2, فراغ *user_معلومات)
{
  hb_face_builder_معلومات_t *معلومات = (hb_face_builder_معلومات_t *) user_معلومات;

  إذا (!نښان_2)
    أرجع hb_face_builder_معلومات_reference_blob (معلومات);

  أرجع hb_blob_reference (معلومات->tables[نښان_2].معلومات);
}

ساكن بینښه
hb_face_builder_get_جدول_نښانونه (ثابت hb_face_t *face HB_ناکارول_شوی,
				 بینښه صحيح پیل_دننه_واټن,
				 بینښه صحيح *جدول_شمېر_2,
				 hb_نښان_t *جدول_نښانونه,
				 فراغ *user_معلومات)
{
  hb_face_builder_معلومات_t *معلومات = (hb_face_builder_معلومات_t *) user_معلومات;

  بینښه population = معلومات->tables.get_population ();

  إذا (!جدول_شمېر_2)
    أرجع population;

  إذا (unlikely (پیل_دننه_واټن >= population))
  {
    *جدول_شمېر_2 = 0;
    أرجع population;
  }

  // Sort the tags.
  hb_ویکټور_t<hb_نښان_t> sorted_نښانونه;
  معلومات->tables.کلیګانې () | hb_sink (sorted_نښانونه);
  إذا (unlikely (sorted_نښانونه.دننه_error ()))
  {
    // Not much to do...
  }
  sorted_نښانونه.qsort ([] (ثابت hb_نښان_t &a, ثابت hb_نښان_t &b) {
    أرجع (a > b) - (a < b);
  });

  خپلکاری array = sorted_نښانونه.as_array ().sub_array (پیل_دننه_واټن, جدول_شمېر_2);
  إذا (جدول_نښانونه)
  {
    خپلکاری بهر = hb_array (جدول_نښانونه, *جدول_شمېر_2);

    + array.iter ()
    | hb_sink (بهر)
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
  hb_face_builder_معلومات_t *معلومات = hb_face_builder_معلومات_create ();
  إذا (unlikely (!معلومات)) أرجع hb_face_get_تشه ();

  hb_face_t *face = hb_face_create_for_tables (hb_face_builder_reference_جدول,
					       معلومات,
					       hb_face_builder_معلومات_destroy);

  hb_face_سېټ_get_جدول_نښانونه_func (face,
				   hb_face_builder_get_جدول_نښانونه,
				   معلومات,
				   تش_اشاره);

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
hb_face_builder_جمع_کړه_جدول (hb_face_t *face, hb_نښان_t نښان_2, hb_blob_t *blob)
{
  إذا (unlikely (face->destroy != (hb_destroy_func_t) hb_face_builder_معلومات_destroy))
    أرجع خطأ;

  إذا (نښان_2 == HB_MAP_ارزښت_INVALID)
    أرجع خطأ;

  hb_face_builder_معلومات_t *معلومات = (hb_face_builder_معلومات_t *) face->user_معلومات;

  hb_blob_t* پخوانی = معلومات->tables.get (نښان_2).معلومات;
  إذا (!معلومات->tables.سېټ (نښان_2, face_جدول_info_t {hb_blob_reference (blob), (بینښه) -1}))
  {
    hb_blob_destroy (blob);
    أرجع خطأ;
  }

  hb_blob_destroy (پخوانی);
  أرجع ریښتیا;
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
hb_face_builder_ترتیبول_tables (hb_face_t *face,
                             ثابت hb_نښان_t  *نښانونه)
{
  إذا (unlikely (face->destroy != (hb_destroy_func_t) hb_face_builder_معلومات_destroy))
    أرجع;

  hb_face_builder_معلومات_t *معلومات = (hb_face_builder_معلومات_t *) face->user_معلومات;

  // Sort all unspecified tables after any specified tables.
  لكل (خپلکاری& info : معلومات->tables.ارزښتونه_ref())
    info.ترتیب = (بینښه) -1;

  نښهدار ترتیب = 0;
  لكل (ثابت hb_نښان_t* نښان_2 = نښانونه;
       *نښان_2;
       نښان_2++)
  {
    face_جدول_info_t* info;
    إذا (!معلومات->tables.has (*نښان_2, &info)) استمر;
    info->ترتیب = ترتیب++;
  }
}
