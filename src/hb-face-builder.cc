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

#অন্তর্ভাব "hb.hh"

#অন্তর্ভাব "hb-face.hh"

#অন্তর্ভাব "hb-map.hh"
#অন্তর্ভাব "hb-open-file.hh"
#অন্তর্ভাব "hb-serialize.hh"


/*
 * face-builder: A face that has add_table().
 */

কাঠামো face_table_info_t
{
  hb_blob_t* তথ্য;
  unsigned order;
};

কাঠামো hb_face_builder_তথ্য_t
{
  hb_hashmap_t<hb_tag_t, face_table_info_t> tables;
};

স্থির পূর্ণসংখ্যা compare_entries (ধ্রুবক শূন্য* pa, ধ্রুবক শূন্য* pb)
{
  ধ্রুবক auto& a = * (ধ্রুবক hb_pair_t<hb_tag_t, face_table_info_t> *) pa;
  ধ্রুবক auto& b = * (ধ্রুবক hb_pair_t<hb_tag_t, face_table_info_t> *) pb;

  /* Order by blob size first (smallest to largest) and then table tag */

  যদি (a.second.order != b.second.order)
    ফেরত a.second.order < b.second.order ? -1 : +1;

  যদি (a.second.তথ্য->দৈর্ঘ্য != b.second.তথ্য->দৈর্ঘ্য)
    ফেরত a.second.তথ্য->দৈর্ঘ্য < b.second.তথ্য->দৈর্ঘ্য ? -1 : +1;

  ফেরত a.first < b.first ? -1 : a.first == b.first ? 0 : +1;
}

স্থির hb_face_builder_তথ্য_t *
hb_face_builder_তথ্য_create ()
{
  hb_face_builder_তথ্য_t *তথ্য = (hb_face_builder_তথ্য_t *) hb_calloc (1, আকার (hb_face_builder_তথ্য_t));
  যদি (unlikely (!তথ্য))
    ফেরত nullptr;

  তথ্য->tables.init ();

  ফেরত তথ্য;
}

স্থির শূন্য
hb_face_builder_তথ্য_destroy (শূন্য *user_তথ্য)
{
  hb_face_builder_তথ্য_t *তথ্য = (hb_face_builder_তথ্য_t *) user_তথ্য;

  জন্য (auto info : তথ্য->tables.values())
    hb_blob_destroy (info.তথ্য);

  তথ্য->tables.fini ();

  hb_free (তথ্য);
}

স্থির hb_blob_t *
hb_face_builder_তথ্য_reference_blob (hb_face_builder_তথ্য_t *তথ্য)
{

  unsigned পূর্ণসংখ্যা table_গুনতি_2 = তথ্য->tables.get_population ();
  unsigned পূর্ণসংখ্যা face_দৈর্ঘ্য = table_গুনতি_2 * 16 + 12;

  জন্য (auto info : তথ্য->tables.values())
    face_দৈর্ঘ্য += hb_ceil_to_4 (hb_blob_get_দৈর্ঘ্য (info.তথ্য));

  অক্ষর *buf = (অক্ষর *) hb_malloc (face_দৈর্ঘ্য);
  যদি (unlikely (!buf))
    ফেরত nullptr;

  hb_serialize_context_t c (buf, face_দৈর্ঘ্য);
  c.propagate_error (তথ্য->tables);
  OT::OpenTypeFontFile *f = c.start_serialize<OT::OpenTypeFontFile> ();

  যৌক্তিক is_cff = (তথ্য->tables.has (HB_TAG ('C','F','F',' '))
                 || তথ্য->tables.has (HB_TAG ('C','F','F','2')));
  hb_tag_t sfnt_tag = is_cff ? OT::OpenTypeFontFile::CFFTag : OT::OpenTypeFontFile::TrueTypeTag;

  // Sort the tags so that produced face is deterministic.
  hb_vector_t<hb_pair_t <hb_tag_t, face_table_info_t>> sorted_entries;
  তথ্য->tables.iter () | hb_sink (sorted_entries);
  যদি (unlikely (sorted_entries.in_error ()))
  {
    hb_free (buf);
    ফেরত nullptr;
  }

  sorted_entries.qsort (compare_entries);

  যৌক্তিক ret = f->serialize_single (&c,
                                  sfnt_tag,
                                  + sorted_entries.iter()
                                  | hb_map ([&] (hb_pair_t<hb_tag_t, face_table_info_t> _) {
                                    ফেরত hb_pair_t<hb_tag_t, hb_blob_t*> (_.first, _.second.তথ্য);
                                  }));

  c.end_serialize ();

  যদি (unlikely (!ret))
  {
    hb_free (buf);
    ফেরত nullptr;
  }

  ফেরত hb_blob_create (buf, face_দৈর্ঘ্য, HB_MEMORY_MODE_WRITABLE, buf, hb_free);
}

স্থির hb_blob_t *
_hb_face_builder_reference_table (hb_face_t *face HB_UNUSED, hb_tag_t tag, শূন্য *user_তথ্য)
{
  hb_face_builder_তথ্য_t *তথ্য = (hb_face_builder_তথ্য_t *) user_তথ্য;

  যদি (!tag)
    ফেরত hb_face_builder_তথ্য_reference_blob (তথ্য);

  ফেরত hb_blob_reference (তথ্য->tables[tag].তথ্য);
}

স্থির unsigned
_hb_face_builder_get_table_tags (ধ্রুবক hb_face_t *face HB_UNUSED,
				 unsigned পূর্ণসংখ্যা start_offset,
				 unsigned পূর্ণসংখ্যা *table_গুনতি_2,
				 hb_tag_t *table_tags,
				 শূন্য *user_তথ্য)
{
  hb_face_builder_তথ্য_t *তথ্য = (hb_face_builder_তথ্য_t *) user_তথ্য;

  unsigned population = তথ্য->tables.get_population ();

  যদি (!table_গুনতি_2)
    ফেরত population;

  যদি (unlikely (start_offset >= population))
  {
    *table_গুনতি_2 = 0;
    ফেরত population;
  }

  // Sort the tags.
  hb_vector_t<hb_tag_t> sorted_tags;
  তথ্য->tables.চাবিগুলি () | hb_sink (sorted_tags);
  যদি (unlikely (sorted_tags.in_error ()))
  {
    // Not much to do...
  }
  sorted_tags.qsort ([] (ধ্রুবক hb_tag_t &a, ধ্রুবক hb_tag_t &b) {
    ফেরত (a > b) - (a < b);
  });

  auto array = sorted_tags.as_array ().sub_array (start_offset, table_গুনতি_2);
  যদি (table_tags)
  {
    auto out = hb_array (table_tags, *table_গুনতি_2);

    + array.iter ()
    | hb_sink (out)
    ;
  }

  ফেরত population;
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
  hb_face_builder_তথ্য_t *তথ্য = hb_face_builder_তথ্য_create ();
  যদি (unlikely (!তথ্য)) ফেরত hb_face_get_empty ();

  hb_face_t *face = hb_face_create_for_tables (_hb_face_builder_reference_table,
					       তথ্য,
					       hb_face_builder_তথ্য_destroy);

  hb_face_set_get_table_tags_func (face,
				   _hb_face_builder_get_table_tags,
				   তথ্য,
				   nullptr);

  ফেরত face;
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
hb_face_builder_add_table (hb_face_t *face, hb_tag_t tag, hb_blob_t *blob)
{
  যদি (unlikely (face->destroy != (hb_destroy_func_t) hb_face_builder_তথ্য_destroy))
    ফেরত মিথ্যা;

  যদি (tag == HB_MAP_মান_INVALID)
    ফেরত মিথ্যা;

  hb_face_builder_তথ্য_t *তথ্য = (hb_face_builder_তথ্য_t *) face->user_তথ্য;

  hb_blob_t* previous = তথ্য->tables.get (tag).তথ্য;
  যদি (!তথ্য->tables.set (tag, face_table_info_t {hb_blob_reference (blob), (unsigned) -1}))
  {
    hb_blob_destroy (blob);
    ফেরত মিথ্যা;
  }

  hb_blob_destroy (previous);
  ফেরত সত্য;
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
শূন্য
hb_face_builder_sort_tables (hb_face_t *face,
                             ধ্রুবক hb_tag_t  *tags)
{
  যদি (unlikely (face->destroy != (hb_destroy_func_t) hb_face_builder_তথ্য_destroy))
    ফেরত;

  hb_face_builder_তথ্য_t *তথ্য = (hb_face_builder_তথ্য_t *) face->user_তথ্য;

  // Sort all unspecified tables after any specified tables.
  জন্য (auto& info : তথ্য->tables.মানসমূহ_ref())
    info.order = (unsigned) -1;

  signed order = 0;
  জন্য (ধ্রুবক hb_tag_t* tag = tags;
       *tag;
       tag++)
  {
    face_table_info_t* info;
    যদি (!তথ্য->tables.has (*tag, &info)) চালু;
    info->order = order++;
  }
}
