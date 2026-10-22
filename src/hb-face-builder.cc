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

#அந்தர்பாவ "hb.hh"

#அந்தர்பாவ "hb-face.hh"

#அந்தர்பாவ "hb-map.hh"
#அந்தர்பாவ "hb-open-file.hh"
#அந்தர்பாவ "hb-serialize.hh"


/*
 * face-builder: A face that has add_table().
 */

கட்டமைப்பு face_table_info_t
{
  hb_blob_t* data;
  unsigned order;
};

கட்டமைப்பு hb_face_builder_data_t
{
  hb_hashmap_t<hb_tag_t, face_table_info_t> tables;
};

நிலையான முழு compare_entries (மாறிலி வெறுமை* pa, மாறிலி வெறுமை* pb)
{
  மாறிலி auto& a = * (மாறிலி hb_pair_t<hb_tag_t, face_table_info_t> *) pa;
  மாறிலி auto& b = * (மாறிலி hb_pair_t<hb_tag_t, face_table_info_t> *) pb;

  /* Order by blob size first (smallest to largest) and then table tag */

  எனில் (a.second.order != b.second.order)
    திரும்பு a.second.order < b.second.order ? -1 : +1;

  எனில் (a.second.data->length != b.second.data->length)
    திரும்பு a.second.data->length < b.second.data->length ? -1 : +1;

  திரும்பு a.first < b.first ? -1 : a.first == b.first ? 0 : +1;
}

நிலையான hb_face_builder_data_t *
_hb_face_builder_data_create ()
{
  hb_face_builder_data_t *data = (hb_face_builder_data_t *) hb_calloc (1, அளவு (hb_face_builder_data_t));
  எனில் (unlikely (!data))
    திரும்பு nullptr;

  data->tables.init ();

  திரும்பு data;
}

நிலையான வெறுமை
_hb_face_builder_data_destroy (வெறுமை *user_data)
{
  hb_face_builder_data_t *data = (hb_face_builder_data_t *) user_data;

  க்கு (auto info : data->tables.values())
    hb_blob_destroy (info.data);

  data->tables.fini ();

  hb_free (data);
}

நிலையான hb_blob_t *
_hb_face_builder_data_reference_blob (hb_face_builder_data_t *data)
{

  unsigned முழு table_மொத்தம்_2 = data->tables.get_population ();
  unsigned முழு face_length = table_மொத்தம்_2 * 16 + 12;

  க்கு (auto info : data->tables.values())
    face_length += hb_ceil_to_4 (hb_blob_get_length (info.data));

  எழுத்து *buf = (எழுத்து *) hb_malloc (face_length);
  எனில் (unlikely (!buf))
    திரும்பு nullptr;

  hb_serialize_context_t c (buf, face_length);
  c.propagate_error (data->tables);
  OT::OpenTypeFontFile *f = c.start_serialize<OT::OpenTypeFontFile> ();

  தருக்க is_cff = (data->tables.has (HB_TAG ('C','F','F',' '))
                 || data->tables.has (HB_TAG ('C','F','F','2')));
  hb_tag_t sfnt_tag = is_cff ? OT::OpenTypeFontFile::CFFTag : OT::OpenTypeFontFile::TrueTypeTag;

  // Sort the tags so that produced face is deterministic.
  hb_vector_t<hb_pair_t <hb_tag_t, face_table_info_t>> sorted_entries;
  data->tables.iter () | hb_sink (sorted_entries);
  எனில் (unlikely (sorted_entries.in_error ()))
  {
    hb_free (buf);
    திரும்பு nullptr;
  }

  sorted_entries.qsort (compare_entries);

  தருக்க ret = f->serialize_single (&c,
                                  sfnt_tag,
                                  + sorted_entries.iter()
                                  | hb_map ([&] (hb_pair_t<hb_tag_t, face_table_info_t> _) {
                                    திரும்பு hb_pair_t<hb_tag_t, hb_blob_t*> (_.first, _.second.data);
                                  }));

  c.end_serialize ();

  எனில் (unlikely (!ret))
  {
    hb_free (buf);
    திரும்பு nullptr;
  }

  திரும்பு hb_blob_create (buf, face_length, HB_MEMORY_MODE_WRITABLE, buf, hb_free);
}

நிலையான hb_blob_t *
_hb_face_builder_reference_table (hb_face_t *face HB_UNUSED, hb_tag_t tag, வெறுமை *user_data)
{
  hb_face_builder_data_t *data = (hb_face_builder_data_t *) user_data;

  எனில் (!tag)
    திரும்பு _hb_face_builder_data_reference_blob (data);

  திரும்பு hb_blob_reference (data->tables[tag].data);
}

நிலையான unsigned
_hb_face_builder_get_table_tags (மாறிலி hb_face_t *face HB_UNUSED,
				 unsigned முழு start_offset,
				 unsigned முழு *table_மொத்தம்_2,
				 hb_tag_t *table_tags,
				 வெறுமை *user_data)
{
  hb_face_builder_data_t *data = (hb_face_builder_data_t *) user_data;

  unsigned population = data->tables.get_population ();

  எனில் (!table_மொத்தம்_2)
    திரும்பு population;

  எனில் (unlikely (start_offset >= population))
  {
    *table_மொத்தம்_2 = 0;
    திரும்பு population;
  }

  // Sort the tags.
  hb_vector_t<hb_tag_t> sorted_tags;
  data->tables.சாவிகள் () | hb_sink (sorted_tags);
  எனில் (unlikely (sorted_tags.in_error ()))
  {
    // Not much to do...
  }
  sorted_tags.qsort ([] (மாறிலி hb_tag_t &a, மாறிலி hb_tag_t &b) {
    திரும்பு (a > b) - (a < b);
  });

  auto array = sorted_tags.as_array ().sub_array (start_offset, table_மொத்தம்_2);
  எனில் (table_tags)
  {
    auto out = hb_array (table_tags, *table_மொத்தம்_2);

    + array.iter ()
    | hb_sink (out)
    ;
  }

  திரும்பு population;
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
  எனில் (unlikely (!data)) திரும்பு hb_face_get_empty ();

  hb_face_t *face = hb_face_create_for_tables (_hb_face_builder_reference_table,
					       data,
					       _hb_face_builder_data_destroy);

  hb_face_set_get_table_tags_func (face,
				   _hb_face_builder_get_table_tags,
				   data,
				   nullptr);

  திரும்பு face;
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
  எனில் (unlikely (face->destroy != (hb_destroy_func_t) _hb_face_builder_data_destroy))
    திரும்பு பொய்;

  எனில் (tag == HB_MAP_மதிப்பு_INVALID)
    திரும்பு பொய்;

  hb_face_builder_data_t *data = (hb_face_builder_data_t *) face->user_data;

  hb_blob_t* previous = data->tables.get (tag).data;
  எனில் (!data->tables.set (tag, face_table_info_t {hb_blob_reference (blob), (unsigned) -1}))
  {
    hb_blob_destroy (blob);
    திரும்பு பொய்;
  }

  hb_blob_destroy (previous);
  திரும்பு உண்மை;
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
வெறுமை
hb_face_builder_sort_tables (hb_face_t *face,
                             மாறிலி hb_tag_t  *tags)
{
  எனில் (unlikely (face->destroy != (hb_destroy_func_t) _hb_face_builder_data_destroy))
    திரும்பு;

  hb_face_builder_data_t *data = (hb_face_builder_data_t *) face->user_data;

  // Sort all unspecified tables after any specified tables.
  க்கு (auto& info : data->tables.மதிப்புகள்_ref())
    info.order = (unsigned) -1;

  signed order = 0;
  க்கு (மாறிலி hb_tag_t* tag = tags;
       *tag;
       tag++)
  {
    face_table_info_t* info;
    எனில் (!data->tables.has (*tag, &info)) தொடர்;
    info->order = order++;
  }
}
