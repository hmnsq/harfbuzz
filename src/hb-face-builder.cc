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

সংরচনা face_table_info_t
{
  hb_blob_t* data;
  unsigned order;
};

সংরচনা hb_face_builder_data_t
{
  hb_hashmap_t<hb_tag_t, face_table_info_t> tables;
};

স্থির পূর্ণাঙ্ক compare_entries (ধ্রুব শূন্য* pa, ধ্রুব শূন্য* pb)
{
  ধ্রুব auto& a = * (ধ্রুব hb_pair_t<hb_tag_t, face_table_info_t> *) pa;
  ধ্রুব auto& b = * (ধ্রুব hb_pair_t<hb_tag_t, face_table_info_t> *) pb;

  /* Order by blob size first (smallest to largest) and then table tag */

  যদি (a.second.order != b.second.order)
    প্রত্যাগম a.second.order < b.second.order ? -1 : +1;

  যদি (a.second.data->length != b.second.data->length)
    প্রত্যাগম a.second.data->length < b.second.data->length ? -1 : +1;

  প্রত্যাগম a.first < b.first ? -1 : a.first == b.first ? 0 : +1;
}

স্থির hb_face_builder_data_t *
_hb_face_builder_data_create ()
{
  hb_face_builder_data_t *data = (hb_face_builder_data_t *) hb_calloc (1, পরিমাণ (hb_face_builder_data_t));
  যদি (unlikely (!data))
    প্রত্যাগম nullptr;

  data->tables.init ();

  প্রত্যাগম data;
}

স্থির শূন্য
_hb_face_builder_data_destroy (শূন্য *user_data)
{
  hb_face_builder_data_t *data = (hb_face_builder_data_t *) user_data;

  প্রতি (auto info : data->tables.values())
    hb_blob_destroy (info.data);

  data->tables.fini ();

  hb_free (data);
}

স্থির hb_blob_t *
_hb_face_builder_data_reference_blob (hb_face_builder_data_t *data)
{

  unsigned পূর্ণাঙ্ক table_গুনতি_2 = data->tables.get_population ();
  unsigned পূর্ণাঙ্ক face_length = table_গুনতি_2 * 16 + 12;

  প্রতি (auto info : data->tables.values())
    face_length += hb_ceil_to_4 (hb_blob_get_length (info.data));

  বর্ণ *buf = (বর্ণ *) hb_malloc (face_length);
  যদি (unlikely (!buf))
    প্রত্যাগম nullptr;

  hb_serialize_context_t c (buf, face_length);
  c.propagate_error (data->tables);
  OT::OpenTypeFontFile *f = c.start_serialize<OT::OpenTypeFontFile> ();

  তার্কিক is_cff = (data->tables.has (HB_TAG ('C','F','F',' '))
                 || data->tables.has (HB_TAG ('C','F','F','2')));
  hb_tag_t sfnt_tag = is_cff ? OT::OpenTypeFontFile::CFFTag : OT::OpenTypeFontFile::TrueTypeTag;

  // Sort the tags so that produced face is deterministic.
  hb_vector_t<hb_pair_t <hb_tag_t, face_table_info_t>> sorted_entries;
  data->tables.iter () | hb_sink (sorted_entries);
  যদি (unlikely (sorted_entries.in_error ()))
  {
    hb_free (buf);
    প্রত্যাগম nullptr;
  }

  sorted_entries.qsort (compare_entries);

  তার্কিক ret = f->serialize_single (&c,
                                  sfnt_tag,
                                  + sorted_entries.iter()
                                  | hb_map ([&] (hb_pair_t<hb_tag_t, face_table_info_t> _) {
                                    প্রত্যাগম hb_pair_t<hb_tag_t, hb_blob_t*> (_.first, _.second.data);
                                  }));

  c.end_serialize ();

  যদি (unlikely (!ret))
  {
    hb_free (buf);
    প্রত্যাগম nullptr;
  }

  প্রত্যাগম hb_blob_create (buf, face_length, HB_MEMORY_MODE_WRITABLE, buf, hb_free);
}

স্থির hb_blob_t *
_hb_face_builder_reference_table (hb_face_t *face HB_UNUSED, hb_tag_t tag, শূন্য *user_data)
{
  hb_face_builder_data_t *data = (hb_face_builder_data_t *) user_data;

  যদি (!tag)
    প্রত্যাগম _hb_face_builder_data_reference_blob (data);

  প্রত্যাগম hb_blob_reference (data->tables[tag].data);
}

স্থির unsigned
_hb_face_builder_get_table_tags (ধ্রুব hb_face_t *face HB_UNUSED,
				 unsigned পূর্ণাঙ্ক start_offset,
				 unsigned পূর্ণাঙ্ক *table_গুনতি_2,
				 hb_tag_t *table_tags,
				 শূন্য *user_data)
{
  hb_face_builder_data_t *data = (hb_face_builder_data_t *) user_data;

  unsigned population = data->tables.get_population ();

  যদি (!table_গুনতি_2)
    প্রত্যাগম population;

  যদি (unlikely (start_offset >= population))
  {
    *table_গুনতি_2 = 0;
    প্রত্যাগম population;
  }

  // Sort the tags.
  hb_vector_t<hb_tag_t> sorted_tags;
  data->tables.চাবিগুলি () | hb_sink (sorted_tags);
  যদি (unlikely (sorted_tags.in_error ()))
  {
    // Not much to do...
  }
  sorted_tags.qsort ([] (ধ্রুব hb_tag_t &a, ধ্রুব hb_tag_t &b) {
    প্রত্যাগম (a > b) - (a < b);
  });

  auto array = sorted_tags.as_array ().sub_array (start_offset, table_গুনতি_2);
  যদি (table_tags)
  {
    auto out = hb_array (table_tags, *table_গুনতি_2);

    + array.iter ()
    | hb_sink (out)
    ;
  }

  প্রত্যাগম population;
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
  hb_face_builder_data_t *data = _hb_face_builder_data_create ();
  যদি (unlikely (!data)) প্রত্যাগম hb_face_get_empty ();

  hb_face_t *face = hb_face_create_for_tables (_hb_face_builder_reference_table,
					       data,
					       _hb_face_builder_data_destroy);

  hb_face_set_get_table_tags_func (face,
				   _hb_face_builder_get_table_tags,
				   data,
				   nullptr);

  প্রত্যাগম face;
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
  যদি (unlikely (face->destroy != (hb_destroy_func_t) _hb_face_builder_data_destroy))
    প্রত্যাগম অসত্য;

  যদি (tag == HB_MAP_মান_INVALID)
    প্রত্যাগম অসত্য;

  hb_face_builder_data_t *data = (hb_face_builder_data_t *) face->user_data;

  hb_blob_t* previous = data->tables.get (tag).data;
  যদি (!data->tables.set (tag, face_table_info_t {hb_blob_reference (blob), (unsigned) -1}))
  {
    hb_blob_destroy (blob);
    প্রত্যাগম অসত্য;
  }

  hb_blob_destroy (previous);
  প্রত্যাগম সত্য;
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
                             ধ্রুব hb_tag_t  *tags)
{
  যদি (unlikely (face->destroy != (hb_destroy_func_t) _hb_face_builder_data_destroy))
    প্রত্যাগম;

  hb_face_builder_data_t *data = (hb_face_builder_data_t *) face->user_data;

  // Sort all unspecified tables after any specified tables.
  প্রতি (auto& info : data->tables.মানসমূহ_ref())
    info.order = (unsigned) -1;

  signed order = 0;
  প্রতি (ধ্রুব hb_tag_t* tag = tags;
       *tag;
       tag++)
  {
    face_table_info_t* info;
    যদি (!data->tables.has (*tag, &info)) অনুবর্ত;
    info->order = order++;
  }
}
