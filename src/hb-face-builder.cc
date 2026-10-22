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
  بنانشان ترتيب;
};

بنية hb_face_builder_معلومات_t
{
  hb_hashmap_t<hb_ٽيگ_t, face_جدول_info_t> tables;
};

ساكن صحيح compare_entries (ثابت فراغ* pa, ثابت فراغ* pb)
{
  ثابت خودڪار& a = * (ثابت hb_pair_t<hb_ٽيگ_t, face_جدول_info_t> *) pa;
  ثابت خودڪار& b = * (ثابت hb_pair_t<hb_ٽيگ_t, face_جدول_info_t> *) pb;

  /* Order by blob size first (smallest to largest) and then table tag */

  إذا (a.second.ترتيب != b.second.ترتيب)
    أرجع a.second.ترتيب < b.second.ترتيب ? -1 : +1;

  إذا (a.second.معلومات->ڊيگهائي != b.second.معلومات->ڊيگهائي)
    أرجع a.second.معلومات->ڊيگهائي < b.second.معلومات->ڊيگهائي ? -1 : +1;

  أرجع a.پهريون < b.پهريون ? -1 : a.پهريون == b.پهريون ? 0 : +1;
}

ساكن hb_face_builder_معلومات_t *
hb_face_builder_معلومات_create ()
{
  hb_face_builder_معلومات_t *معلومات = (hb_face_builder_معلومات_t *) hb_calloc (1, حجم (hb_face_builder_معلومات_t));
  إذا (unlikely (!معلومات))
    أرجع خالي_اشارو;

  معلومات->tables.init ();

  أرجع معلومات;
}

ساكن فراغ
hb_face_builder_معلومات_destroy (فراغ *user_معلومات)
{
  hb_face_builder_معلومات_t *معلومات = (hb_face_builder_معلومات_t *) user_معلومات;

  لكل (خودڪار info : معلومات->tables.values())
    hb_blob_destroy (info.معلومات);

  معلومات->tables.fini ();

  hb_free (معلومات);
}

ساكن hb_blob_t *
hb_face_builder_معلومات_reference_blob (hb_face_builder_معلومات_t *معلومات)
{

  بنانشان صحيح جدول_ڳڻپ_2 = معلومات->tables.get_population ();
  بنانشان صحيح face_ڊيگهائي = جدول_ڳڻپ_2 * 16 + 12;

  لكل (خودڪار info : معلومات->tables.values())
    face_ڊيگهائي += hb_ceil_منزل_4 (hb_blob_get_ڊيگهائي (info.معلومات));

  حرف *buf = (حرف *) hb_malloc (face_ڊيگهائي);
  إذا (unlikely (!buf))
    أرجع خالي_اشارو;

  hb_serialize_context_t c (buf, face_ڊيگهائي);
  c.propagate_error (معلومات->tables);
  OT::OpenTypeFontFile *f = c.شروعات_serialize<OT::OpenTypeFontFile> ();

  منطقي is_cff = (معلومات->tables.has (HB_ٽيگ ('C','F','F',' '))
                 || معلومات->tables.has (HB_ٽيگ ('C','F','F','2')));
  hb_ٽيگ_t sfnt_ٽيگ = is_cff ? OT::OpenTypeFontFile::CFFTag : OT::OpenTypeFontFile::True_Type_ٽيگ;

  // Sort the tags so that produced face is deterministic.
  hb_ويڪٽر_t<hb_pair_t <hb_ٽيگ_t, face_جدول_info_t>> sorted_entries;
  معلومات->tables.iter () | hb_sink (sorted_entries);
  إذا (unlikely (sorted_entries.اندريون_error ()))
  {
    hb_free (buf);
    أرجع خالي_اشارو;
  }

  sorted_entries.qsort (compare_entries);

  منطقي ret = f->serialize_single (&c,
                                  sfnt_ٽيگ,
                                  + sorted_entries.iter()
                                  | hb_map ([&] (hb_pair_t<hb_ٽيگ_t, face_جدول_info_t> _) {
                                    أرجع hb_pair_t<hb_ٽيگ_t, hb_blob_t*> (_.پهريون, _.second.معلومات);
                                  }));

  c.پڇاڙي_serialize ();

  إذا (unlikely (!ret))
  {
    hb_free (buf);
    أرجع خالي_اشارو;
  }

  أرجع hb_blob_create (buf, face_ڊيگهائي, HB_يادگيري_گهڻائي_WRITABLE, buf, hb_free);
}

ساكن hb_blob_t *
hb_face_builder_reference_جدول (hb_face_t *face HB_اڻ_استعمال_ٿيل, hb_ٽيگ_t ٽيگ_2, فراغ *user_معلومات)
{
  hb_face_builder_معلومات_t *معلومات = (hb_face_builder_معلومات_t *) user_معلومات;

  إذا (!ٽيگ_2)
    أرجع hb_face_builder_معلومات_reference_blob (معلومات);

  أرجع hb_blob_reference (معلومات->tables[ٽيگ_2].معلومات);
}

ساكن بنانشان
hb_face_builder_get_جدول_ٽيگ_فهرست (ثابت hb_face_t *face HB_اڻ_استعمال_ٿيل,
				 بنانشان صحيح شروعات_وٿي,
				 بنانشان صحيح *جدول_ڳڻپ_2,
				 hb_ٽيگ_t *جدول_ٽيگ_فهرست,
				 فراغ *user_معلومات)
{
  hb_face_builder_معلومات_t *معلومات = (hb_face_builder_معلومات_t *) user_معلومات;

  بنانشان population = معلومات->tables.get_population ();

  إذا (!جدول_ڳڻپ_2)
    أرجع population;

  إذا (unlikely (شروعات_وٿي >= population))
  {
    *جدول_ڳڻپ_2 = 0;
    أرجع population;
  }

  // Sort the tags.
  hb_ويڪٽر_t<hb_ٽيگ_t> sorted_ٽيگ_فهرست;
  معلومات->tables.ڪنجيون () | hb_sink (sorted_ٽيگ_فهرست);
  إذا (unlikely (sorted_ٽيگ_فهرست.اندريون_error ()))
  {
    // Not much to do...
  }
  sorted_ٽيگ_فهرست.qsort ([] (ثابت hb_ٽيگ_t &a, ثابت hb_ٽيگ_t &b) {
    أرجع (a > b) - (a < b);
  });

  خودڪار array = sorted_ٽيگ_فهرست.as_array ().sub_array (شروعات_وٿي, جدول_ڳڻپ_2);
  إذا (جدول_ٽيگ_فهرست)
  {
    خودڪار ٻاهريون = hb_array (جدول_ٽيگ_فهرست, *جدول_ڳڻپ_2);

    + array.iter ()
    | hb_sink (ٻاهريون)
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
  إذا (unlikely (!معلومات)) أرجع hb_face_get_خالي_جايون ();

  hb_face_t *face = hb_face_create_for_tables (hb_face_builder_reference_جدول,
					       معلومات,
					       hb_face_builder_معلومات_destroy);

  hb_face_سيٽ_get_جدول_ٽيگ_فهرست_func (face,
				   hb_face_builder_get_جدول_ٽيگ_فهرست,
				   معلومات,
				   خالي_اشارو);

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
hb_face_builder_جوڙيو_جدول (hb_face_t *face, hb_ٽيگ_t ٽيگ_2, hb_blob_t *blob)
{
  إذا (unlikely (face->destroy != (hb_destroy_func_t) hb_face_builder_معلومات_destroy))
    أرجع خطأ;

  إذا (ٽيگ_2 == HB_MAP_قدر_INVALID)
    أرجع خطأ;

  hb_face_builder_معلومات_t *معلومات = (hb_face_builder_معلومات_t *) face->user_معلومات;

  hb_blob_t* اڳيون = معلومات->tables.get (ٽيگ_2).معلومات;
  إذا (!معلومات->tables.سيٽ (ٽيگ_2, face_جدول_info_t {hb_blob_reference (blob), (بنانشان) -1}))
  {
    hb_blob_destroy (blob);
    أرجع خطأ;
  }

  hb_blob_destroy (اڳيون);
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
hb_face_builder_ترتيب_ڏيو_tables (hb_face_t *face,
                             ثابت hb_ٽيگ_t  *ٽيگ_فهرست)
{
  إذا (unlikely (face->destroy != (hb_destroy_func_t) hb_face_builder_معلومات_destroy))
    أرجع;

  hb_face_builder_معلومات_t *معلومات = (hb_face_builder_معلومات_t *) face->user_معلومات;

  // Sort all unspecified tables after any specified tables.
  لكل (خودڪار& info : معلومات->tables.قدرون_ref())
    info.ترتيب = (بنانشان) -1;

  نشانوارو ترتيب = 0;
  لكل (ثابت hb_ٽيگ_t* ٽيگ_2 = ٽيگ_فهرست;
       *ٽيگ_2;
       ٽيگ_2++)
  {
    face_جدول_info_t* info;
    إذا (!معلومات->tables.has (*ٽيگ_2, &info)) استمر;
    info->ترتيب = ترتيب++;
  }
}
