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

#ଅନ୍ତର୍ଭାଵ "hb.hh"

#ଅନ୍ତର୍ଭାଵ "hb-face.hh"

#ଅନ୍ତର୍ଭାଵ "hb-map.hh"
#ଅନ୍ତର୍ଭାଵ "hb-open-file.hh"
#ଅନ୍ତର୍ଭାଵ "hb-serialize.hh"


/*
 * face-builder: A face that has add_table().
 */

ସଂରଚନା face_table_info_t
{
  hb_blob_t* data;
  unsigned order;
};

ସଂରଚନା hb_face_builder_data_t
{
  hb_hashmap_t<hb_tag_t, face_table_info_t> tables;
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା compare_entries (ଅଚଳ ଶୂନ୍ୟ* pa, ଅଚଳ ଶୂନ୍ୟ* pb)
{
  ଅଚଳ auto& a = * (ଅଚଳ hb_pair_t<hb_tag_t, face_table_info_t> *) pa;
  ଅଚଳ auto& b = * (ଅଚଳ hb_pair_t<hb_tag_t, face_table_info_t> *) pb;

  /* Order by blob size first (smallest to largest) and then table tag */

  ଯଦି (a.second.order != b.second.order)
    ଫେରନ୍ତୁ a.second.order < b.second.order ? -1 : +1;

  ଯଦି (a.second.data->length != b.second.data->length)
    ଫେରନ୍ତୁ a.second.data->length < b.second.data->length ? -1 : +1;

  ଫେରନ୍ତୁ a.first < b.first ? -1 : a.first == b.first ? 0 : +1;
}

ସ୍ଥିର hb_face_builder_data_t *
_hb_face_builder_data_create ()
{
  hb_face_builder_data_t *data = (hb_face_builder_data_t *) hb_calloc (1, ଆକାର (hb_face_builder_data_t));
  ଯଦି (unlikely (!data))
    ଫେରନ୍ତୁ nullptr;

  data->tables.init ();

  ଫେରନ୍ତୁ data;
}

ସ୍ଥିର ଶୂନ୍ୟ
_hb_face_builder_data_destroy (ଶୂନ୍ୟ *user_data)
{
  hb_face_builder_data_t *data = (hb_face_builder_data_t *) user_data;

  ପାଇଁ (auto info : data->tables.values())
    hb_blob_destroy (info.data);

  data->tables.fini ();

  hb_free (data);
}

ସ୍ଥିର hb_blob_t *
_hb_face_builder_data_reference_blob (hb_face_builder_data_t *data)
{

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା table_ଗଣନା_2 = data->tables.get_population ();
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା face_length = table_ଗଣନା_2 * 16 + 12;

  ପାଇଁ (auto info : data->tables.values())
    face_length += hb_ceil_to_4 (hb_blob_get_length (info.data));

  ଅକ୍ଷର *buf = (ଅକ୍ଷର *) hb_malloc (face_length);
  ଯଦି (unlikely (!buf))
    ଫେରନ୍ତୁ nullptr;

  hb_serialize_context_t c (buf, face_length);
  c.propagate_error (data->tables);
  OT::OpenTypeFontFile *f = c.start_serialize<OT::OpenTypeFontFile> ();

  ତାର୍କିକ is_cff = (data->tables.has (HB_TAG ('C','F','F',' '))
                 || data->tables.has (HB_TAG ('C','F','F','2')));
  hb_tag_t sfnt_tag = is_cff ? OT::OpenTypeFontFile::CFFTag : OT::OpenTypeFontFile::TrueTypeTag;

  // Sort the tags so that produced face is deterministic.
  hb_vector_t<hb_pair_t <hb_tag_t, face_table_info_t>> sorted_entries;
  data->tables.iter () | hb_sink (sorted_entries);
  ଯଦି (unlikely (sorted_entries.in_error ()))
  {
    hb_free (buf);
    ଫେରନ୍ତୁ nullptr;
  }

  sorted_entries.qsort (compare_entries);

  ତାର୍କିକ ret = f->serialize_single (&c,
                                  sfnt_tag,
                                  + sorted_entries.iter()
                                  | hb_map ([&] (hb_pair_t<hb_tag_t, face_table_info_t> _) {
                                    ଫେରନ୍ତୁ hb_pair_t<hb_tag_t, hb_blob_t*> (_.first, _.second.data);
                                  }));

  c.end_serialize ();

  ଯଦି (unlikely (!ret))
  {
    hb_free (buf);
    ଫେରନ୍ତୁ nullptr;
  }

  ଫେରନ୍ତୁ hb_blob_create (buf, face_length, HB_MEMORY_MODE_WRITABLE, buf, hb_free);
}

ସ୍ଥିର hb_blob_t *
_hb_face_builder_reference_table (hb_face_t *face HB_UNUSED, hb_tag_t tag, ଶୂନ୍ୟ *user_data)
{
  hb_face_builder_data_t *data = (hb_face_builder_data_t *) user_data;

  ଯଦି (!tag)
    ଫେରନ୍ତୁ _hb_face_builder_data_reference_blob (data);

  ଫେରନ୍ତୁ hb_blob_reference (data->tables[tag].data);
}

ସ୍ଥିର unsigned
_hb_face_builder_get_table_tags (ଅଚଳ hb_face_t *face HB_UNUSED,
				 unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start_offset,
				 unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *table_ଗଣନା_2,
				 hb_tag_t *table_tags,
				 ଶୂନ୍ୟ *user_data)
{
  hb_face_builder_data_t *data = (hb_face_builder_data_t *) user_data;

  unsigned population = data->tables.get_population ();

  ଯଦି (!table_ଗଣନା_2)
    ଫେରନ୍ତୁ population;

  ଯଦି (unlikely (start_offset >= population))
  {
    *table_ଗଣନା_2 = 0;
    ଫେରନ୍ତୁ population;
  }

  // Sort the tags.
  hb_vector_t<hb_tag_t> sorted_tags;
  data->tables.ଚାବିଗୁଡ଼ିକ () | hb_sink (sorted_tags);
  ଯଦି (unlikely (sorted_tags.in_error ()))
  {
    // Not much to do...
  }
  sorted_tags.qsort ([] (ଅଚଳ hb_tag_t &a, ଅଚଳ hb_tag_t &b) {
    ଫେରନ୍ତୁ (a > b) - (a < b);
  });

  auto array = sorted_tags.as_array ().sub_array (start_offset, table_ଗଣନା_2);
  ଯଦି (table_tags)
  {
    auto out = hb_array (table_tags, *table_ଗଣନା_2);

    + array.iter ()
    | hb_sink (out)
    ;
  }

  ଫେରନ୍ତୁ population;
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
  ଯଦି (unlikely (!data)) ଫେରନ୍ତୁ hb_face_get_empty ();

  hb_face_t *face = hb_face_create_for_tables (_hb_face_builder_reference_table,
					       data,
					       _hb_face_builder_data_destroy);

  hb_face_set_get_table_tags_func (face,
				   _hb_face_builder_get_table_tags,
				   data,
				   nullptr);

  ଫେରନ୍ତୁ face;
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
  ଯଦି (unlikely (face->destroy != (hb_destroy_func_t) _hb_face_builder_data_destroy))
    ଫେରନ୍ତୁ ମିଥ୍ୟା;

  ଯଦି (tag == HB_MAP_ମୂଲ୍ୟ_INVALID)
    ଫେରନ୍ତୁ ମିଥ୍ୟା;

  hb_face_builder_data_t *data = (hb_face_builder_data_t *) face->user_data;

  hb_blob_t* previous = data->tables.get (tag).data;
  ଯଦି (!data->tables.set (tag, face_table_info_t {hb_blob_reference (blob), (unsigned) -1}))
  {
    hb_blob_destroy (blob);
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }

  hb_blob_destroy (previous);
  ଫେରନ୍ତୁ ସତ୍ୟ;
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
ଶୂନ୍ୟ
hb_face_builder_sort_tables (hb_face_t *face,
                             ଅଚଳ hb_tag_t  *tags)
{
  ଯଦି (unlikely (face->destroy != (hb_destroy_func_t) _hb_face_builder_data_destroy))
    ଫେରନ୍ତୁ;

  hb_face_builder_data_t *data = (hb_face_builder_data_t *) face->user_data;

  // Sort all unspecified tables after any specified tables.
  ପାଇଁ (auto& info : data->tables.ମୂଲ୍ୟଗୁଡ଼ିକ_ref())
    info.order = (unsigned) -1;

  signed order = 0;
  ପାଇଁ (ଅଚଳ hb_tag_t* tag = tags;
       *tag;
       tag++)
  {
    face_table_info_t* info;
    ଯଦି (!data->tables.has (*tag, &info)) ଜାରି;
    info->order = order++;
  }
}
