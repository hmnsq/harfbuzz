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

#अन्तर्भाव "hb.hh"

#अन्तर्भाव "hb-face.hh"

#अन्तर्भाव "hb-map.hh"
#अन्तर्भाव "hb-open-file.hh"
#अन्तर्भाव "hb-serialize.hh"


/*
 * face-builder: A face that has add_table().
 */

संरचना face_table_info_t
{
  hb_blob_t* data;
  अचिन्हित order;
};

संरचना hb_face_builder_data_t
{
  hb_hashmap_t<hb_tag_t, face_table_info_t> tables;
};

स्थिर पूर्णांक compare_entries (स्थिरांक शून्य* pa, स्थिरांक शून्य* pb)
{
  स्थिरांक स्वतः& a = * (स्थिरांक hb_pair_t<hb_tag_t, face_table_info_t> *) pa;
  स्थिरांक स्वतः& b = * (स्थिरांक hb_pair_t<hb_tag_t, face_table_info_t> *) pb;

  /* Order by blob size first (smallest to largest) and then table tag */

  जर (a.second.order != b.second.order)
    परत a.second.order < b.second.order ? -1 : +1;

  जर (a.second.data->length != b.second.data->length)
    परत a.second.data->length < b.second.data->length ? -1 : +1;

  परत a.first < b.first ? -1 : a.first == b.first ? 0 : +1;
}

स्थिर hb_face_builder_data_t *
_hb_face_builder_data_create ()
{
  hb_face_builder_data_t *data = (hb_face_builder_data_t *) hb_calloc (1, आकार (hb_face_builder_data_t));
  जर (unlikely (!data))
    परत nullptr;

  data->tables.init ();

  परत data;
}

स्थिर शून्य
_hb_face_builder_data_destroy (शून्य *user_data)
{
  hb_face_builder_data_t *data = (hb_face_builder_data_t *) user_data;

  साठी (स्वतः info : data->tables.values())
    hb_blob_destroy (info.data);

  data->tables.fini ();

  hb_free (data);
}

स्थिर hb_blob_t *
_hb_face_builder_data_reference_blob (hb_face_builder_data_t *data)
{

  अचिन्हित पूर्णांक table_मोजणी_2 = data->tables.get_population ();
  अचिन्हित पूर्णांक face_length = table_मोजणी_2 * 16 + 12;

  साठी (स्वतः info : data->tables.values())
    face_length += hb_ceil_to_4 (hb_blob_get_length (info.data));

  अक्षर *buf = (अक्षर *) hb_malloc (face_length);
  जर (unlikely (!buf))
    परत nullptr;

  hb_serialize_context_t c (buf, face_length);
  c.propagate_error (data->tables);
  OT::OpenTypeFontFile *f = c.start_serialize<OT::OpenTypeFontFile> ();

  तार्किक is_cff = (data->tables.has (HB_TAG ('C','F','F',' '))
                 || data->tables.has (HB_TAG ('C','F','F','2')));
  hb_tag_t sfnt_tag = is_cff ? OT::OpenTypeFontFile::CFFTag : OT::OpenTypeFontFile::TrueTypeTag;

  // Sort the tags so that produced face is deterministic.
  hb_vector_t<hb_pair_t <hb_tag_t, face_table_info_t>> sorted_entries;
  data->tables.iter () | hb_sink (sorted_entries);
  जर (unlikely (sorted_entries.in_error ()))
  {
    hb_free (buf);
    परत nullptr;
  }

  sorted_entries.क्विक (compare_entries);

  तार्किक ret = f->serialize_single (&c,
                                  sfnt_tag,
                                  + sorted_entries.iter()
                                  | hb_map ([&] (hb_pair_t<hb_tag_t, face_table_info_t> _) {
                                    परत hb_pair_t<hb_tag_t, hb_blob_t*> (_.first, _.second.data);
                                  }));

  c.end_serialize ();

  जर (unlikely (!ret))
  {
    hb_free (buf);
    परत nullptr;
  }

  परत hb_blob_create (buf, face_length, HB_MEMORY_MODE_WRITABLE, buf, hb_free);
}

स्थिर hb_blob_t *
_hb_face_builder_reference_table (hb_face_t *face HB_UNUSED, hb_tag_t tag, शून्य *user_data)
{
  hb_face_builder_data_t *data = (hb_face_builder_data_t *) user_data;

  जर (!tag)
    परत _hb_face_builder_data_reference_blob (data);

  परत hb_blob_reference (data->tables[tag].data);
}

स्थिर अचिन्हित
_hb_face_builder_get_table_tags (स्थिरांक hb_face_t *face HB_UNUSED,
				 अचिन्हित पूर्णांक start_offset,
				 अचिन्हित पूर्णांक *table_मोजणी_2,
				 hb_tag_t *table_tags,
				 शून्य *user_data)
{
  hb_face_builder_data_t *data = (hb_face_builder_data_t *) user_data;

  अचिन्हित population = data->tables.get_population ();

  जर (!table_मोजणी_2)
    परत population;

  जर (unlikely (start_offset >= population))
  {
    *table_मोजणी_2 = 0;
    परत population;
  }

  // Sort the tags.
  hb_vector_t<hb_tag_t> sorted_tags;
  data->tables.किल्ल्या () | hb_sink (sorted_tags);
  जर (unlikely (sorted_tags.in_error ()))
  {
    // Not much to do...
  }
  sorted_tags.क्विक ([] (स्थिरांक hb_tag_t &a, स्थिरांक hb_tag_t &b) {
    परत (a > b) - (a < b);
  });

  स्वतः array = sorted_tags.as_array ().sub_array (start_offset, table_मोजणी_2);
  जर (table_tags)
  {
    स्वतः out = hb_array (table_tags, *table_मोजणी_2);

    + array.iter ()
    | hb_sink (out)
    ;
  }

  परत population;
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
  जर (unlikely (!data)) परत hb_face_get_empty ();

  hb_face_t *face = hb_face_create_for_tables (_hb_face_builder_reference_table,
					       data,
					       _hb_face_builder_data_destroy);

  hb_face_set_get_table_tags_func (face,
				   _hb_face_builder_get_table_tags,
				   data,
				   nullptr);

  परत face;
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
  जर (unlikely (face->destroy != (hb_destroy_func_t) _hb_face_builder_data_destroy))
    परत असत्य;

  जर (tag == HB_MAP_मूल्य_INVALID)
    परत असत्य;

  hb_face_builder_data_t *data = (hb_face_builder_data_t *) face->user_data;

  hb_blob_t* previous = data->tables.get (tag).data;
  जर (!data->tables.set (tag, face_table_info_t {hb_blob_reference (blob), (अचिन्हित) -1}))
  {
    hb_blob_destroy (blob);
    परत असत्य;
  }

  hb_blob_destroy (previous);
  परत सत्य;
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
शून्य
hb_face_builder_sort_tables (hb_face_t *face,
                             स्थिरांक hb_tag_t  *tags)
{
  जर (unlikely (face->destroy != (hb_destroy_func_t) _hb_face_builder_data_destroy))
    परत;

  hb_face_builder_data_t *data = (hb_face_builder_data_t *) face->user_data;

  // Sort all unspecified tables after any specified tables.
  साठी (स्वतः& info : data->tables.मूल्ये_ref())
    info.order = (अचिन्हित) -1;

  चिन्हित order = 0;
  साठी (स्थिरांक hb_tag_t* tag = tags;
       *tag;
       tag++)
  {
    face_table_info_t* info;
    जर (!data->tables.has (*tag, &info)) सुरूठेवा;
    info->order = order++;
  }
}
