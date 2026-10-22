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

#ಅನ್ತರ್ಭಾವ "hb.hh"

#ಅನ್ತರ್ಭಾವ "hb-face.hh"

#ಅನ್ತರ್ಭಾವ "hb-map.hh"
#ಅನ್ತರ್ಭಾವ "hb-open-file.hh"
#ಅನ್ತರ್ಭಾವ "hb-serialize.hh"


/*
 * face-builder: A face that has add_table().
 */

ಸಂರಚನಾ face_table_info_t
{
  hb_blob_t* data;
  unsigned order;
};

ಸಂರಚನಾ hb_face_builder_data_t
{
  hb_hashmap_t<hb_tag_t, face_table_info_t> tables;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ compare_entries (ಧ್ರುವ ಶೂನ್ಯ* pa, ಧ್ರುವ ಶೂನ್ಯ* pb)
{
  ಧ್ರುವ auto& a = * (ಧ್ರುವ hb_pair_t<hb_tag_t, face_table_info_t> *) pa;
  ಧ್ರುವ auto& b = * (ಧ್ರುವ hb_pair_t<hb_tag_t, face_table_info_t> *) pb;

  /* Order by blob size first (smallest to largest) and then table tag */

  ಯದಿ (a.second.order != b.second.order)
    ಪ್ರತ್ಯಾಗಮ a.second.order < b.second.order ? -1 : +1;

  ಯದಿ (a.second.data->length != b.second.data->length)
    ಪ್ರತ್ಯಾಗಮ a.second.data->length < b.second.data->length ? -1 : +1;

  ಪ್ರತ್ಯಾಗಮ a.first < b.first ? -1 : a.first == b.first ? 0 : +1;
}

ಸ್ಥಿರ hb_face_builder_data_t *
_hb_face_builder_data_create ()
{
  hb_face_builder_data_t *data = (hb_face_builder_data_t *) hb_calloc (1, ಪರಿಮಾಣ (hb_face_builder_data_t));
  ಯದಿ (unlikely (!data))
    ಪ್ರತ್ಯಾಗಮ nullptr;

  data->tables.init ();

  ಪ್ರತ್ಯಾಗಮ data;
}

ಸ್ಥಿರ ಶೂನ್ಯ
_hb_face_builder_data_destroy (ಶೂನ್ಯ *user_data)
{
  hb_face_builder_data_t *data = (hb_face_builder_data_t *) user_data;

  ಪ್ರತಿ (auto info : data->tables.values())
    hb_blob_destroy (info.data);

  data->tables.fini ();

  hb_free (data);
}

ಸ್ಥಿರ hb_blob_t *
_hb_face_builder_data_reference_blob (hb_face_builder_data_t *data)
{

  unsigned ಪೂರ್ಣಾಙ್ಕ table_ಎಣಿಕೆ_2 = data->tables.get_population ();
  unsigned ಪೂರ್ಣಾಙ್ಕ face_length = table_ಎಣಿಕೆ_2 * 16 + 12;

  ಪ್ರತಿ (auto info : data->tables.values())
    face_length += hb_ceil_to_4 (hb_blob_get_length (info.data));

  ವರ್ಣ *buf = (ವರ್ಣ *) hb_malloc (face_length);
  ಯದಿ (unlikely (!buf))
    ಪ್ರತ್ಯಾಗಮ nullptr;

  hb_serialize_context_t c (buf, face_length);
  c.propagate_error (data->tables);
  OT::OpenTypeFontFile *f = c.start_serialize<OT::OpenTypeFontFile> ();

  ತಾರ್ಕಿಕ is_cff = (data->tables.has (HB_TAG ('C','F','F',' '))
                 || data->tables.has (HB_TAG ('C','F','F','2')));
  hb_tag_t sfnt_tag = is_cff ? OT::OpenTypeFontFile::CFFTag : OT::OpenTypeFontFile::TrueTypeTag;

  // Sort the tags so that produced face is deterministic.
  hb_vector_t<hb_pair_t <hb_tag_t, face_table_info_t>> sorted_entries;
  data->tables.iter () | hb_sink (sorted_entries);
  ಯದಿ (unlikely (sorted_entries.in_error ()))
  {
    hb_free (buf);
    ಪ್ರತ್ಯಾಗಮ nullptr;
  }

  sorted_entries.qsort (compare_entries);

  ತಾರ್ಕಿಕ ret = f->serialize_single (&c,
                                  sfnt_tag,
                                  + sorted_entries.iter()
                                  | hb_map ([&] (hb_pair_t<hb_tag_t, face_table_info_t> _) {
                                    ಪ್ರತ್ಯಾಗಮ hb_pair_t<hb_tag_t, hb_blob_t*> (_.first, _.second.data);
                                  }));

  c.end_serialize ();

  ಯದಿ (unlikely (!ret))
  {
    hb_free (buf);
    ಪ್ರತ್ಯಾಗಮ nullptr;
  }

  ಪ್ರತ್ಯಾಗಮ hb_blob_create (buf, face_length, HB_MEMORY_MODE_WRITABLE, buf, hb_free);
}

ಸ್ಥಿರ hb_blob_t *
_hb_face_builder_reference_table (hb_face_t *face HB_UNUSED, hb_tag_t tag, ಶೂನ್ಯ *user_data)
{
  hb_face_builder_data_t *data = (hb_face_builder_data_t *) user_data;

  ಯದಿ (!tag)
    ಪ್ರತ್ಯಾಗಮ _hb_face_builder_data_reference_blob (data);

  ಪ್ರತ್ಯಾಗಮ hb_blob_reference (data->tables[tag].data);
}

ಸ್ಥಿರ unsigned
_hb_face_builder_get_table_tags (ಧ್ರುವ hb_face_t *face HB_UNUSED,
				 unsigned ಪೂರ್ಣಾಙ್ಕ start_offset,
				 unsigned ಪೂರ್ಣಾಙ್ಕ *table_ಎಣಿಕೆ_2,
				 hb_tag_t *table_tags,
				 ಶೂನ್ಯ *user_data)
{
  hb_face_builder_data_t *data = (hb_face_builder_data_t *) user_data;

  unsigned population = data->tables.get_population ();

  ಯದಿ (!table_ಎಣಿಕೆ_2)
    ಪ್ರತ್ಯಾಗಮ population;

  ಯದಿ (unlikely (start_offset >= population))
  {
    *table_ಎಣಿಕೆ_2 = 0;
    ಪ್ರತ್ಯಾಗಮ population;
  }

  // Sort the tags.
  hb_vector_t<hb_tag_t> sorted_tags;
  data->tables.ಕೀಲಿಗಳು () | hb_sink (sorted_tags);
  ಯದಿ (unlikely (sorted_tags.in_error ()))
  {
    // Not much to do...
  }
  sorted_tags.qsort ([] (ಧ್ರುವ hb_tag_t &a, ಧ್ರುವ hb_tag_t &b) {
    ಪ್ರತ್ಯಾಗಮ (a > b) - (a < b);
  });

  auto array = sorted_tags.as_array ().sub_array (start_offset, table_ಎಣಿಕೆ_2);
  ಯದಿ (table_tags)
  {
    auto out = hb_array (table_tags, *table_ಎಣಿಕೆ_2);

    + array.iter ()
    | hb_sink (out)
    ;
  }

  ಪ್ರತ್ಯಾಗಮ population;
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
  ಯದಿ (unlikely (!data)) ಪ್ರತ್ಯಾಗಮ hb_face_get_empty ();

  hb_face_t *face = hb_face_create_for_tables (_hb_face_builder_reference_table,
					       data,
					       _hb_face_builder_data_destroy);

  hb_face_set_get_table_tags_func (face,
				   _hb_face_builder_get_table_tags,
				   data,
				   nullptr);

  ಪ್ರತ್ಯಾಗಮ face;
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
  ಯದಿ (unlikely (face->destroy != (hb_destroy_func_t) _hb_face_builder_data_destroy))
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

  ಯದಿ (tag == HB_MAP_ಮೌಲ್ಯ_INVALID)
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

  hb_face_builder_data_t *data = (hb_face_builder_data_t *) face->user_data;

  hb_blob_t* previous = data->tables.get (tag).data;
  ಯದಿ (!data->tables.set (tag, face_table_info_t {hb_blob_reference (blob), (unsigned) -1}))
  {
    hb_blob_destroy (blob);
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }

  hb_blob_destroy (previous);
  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
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
ಶೂನ್ಯ
hb_face_builder_sort_tables (hb_face_t *face,
                             ಧ್ರುವ hb_tag_t  *tags)
{
  ಯದಿ (unlikely (face->destroy != (hb_destroy_func_t) _hb_face_builder_data_destroy))
    ಪ್ರತ್ಯಾಗಮ;

  hb_face_builder_data_t *data = (hb_face_builder_data_t *) face->user_data;

  // Sort all unspecified tables after any specified tables.
  ಪ್ರತಿ (auto& info : data->tables.ಮೌಲ್ಯಗಳು_ref())
    info.order = (unsigned) -1;

  signed order = 0;
  ಪ್ರತಿ (ಧ್ರುವ hb_tag_t* tag = tags;
       *tag;
       tag++)
  {
    face_table_info_t* info;
    ಯದಿ (!data->tables.has (*tag, &info)) ಅನುವರ್ತ;
    info->order = order++;
  }
}
