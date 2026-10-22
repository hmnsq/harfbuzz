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

#അന്തര്ഭാവ "hb.hh"

#അന്തര്ഭാവ "hb-face.hh"

#അന്തര്ഭാവ "hb-map.hh"
#അന്തര്ഭാവ "hb-open-file.hh"
#അന്തര്ഭാവ "hb-serialize.hh"


/*
 * face-builder: A face that has add_table().
 */

ഘടന face_table_info_t
{
  hb_blob_t* data;
  unsigned order;
};

ഘടന hb_face_builder_data_t
{
  hb_hashmap_t<hb_tag_t, face_table_info_t> tables;
};

സ്ഥിതം പൂർണ്ണസംഖ്യ compare_entries (സ്ഥിരം ശൂന്യം* pa, സ്ഥിരം ശൂന്യം* pb)
{
  സ്ഥിരം auto& a = * (സ്ഥിരം hb_pair_t<hb_tag_t, face_table_info_t> *) pa;
  സ്ഥിരം auto& b = * (സ്ഥിരം hb_pair_t<hb_tag_t, face_table_info_t> *) pb;

  /* Order by blob size first (smallest to largest) and then table tag */

  എങ്കിൽ (a.second.order != b.second.order)
    തിരികെ a.second.order < b.second.order ? -1 : +1;

  എങ്കിൽ (a.second.data->length != b.second.data->length)
    തിരികെ a.second.data->length < b.second.data->length ? -1 : +1;

  തിരികെ a.first < b.first ? -1 : a.first == b.first ? 0 : +1;
}

സ്ഥിതം hb_face_builder_data_t *
_hb_face_builder_data_create ()
{
  hb_face_builder_data_t *data = (hb_face_builder_data_t *) hb_calloc (1, വലുപ്പം (hb_face_builder_data_t));
  എങ്കിൽ (unlikely (!data))
    തിരികെ nullptr;

  data->tables.init ();

  തിരികെ data;
}

സ്ഥിതം ശൂന്യം
_hb_face_builder_data_destroy (ശൂന്യം *user_data)
{
  hb_face_builder_data_t *data = (hb_face_builder_data_t *) user_data;

  വേണ്ടി (auto info : data->tables.values())
    hb_blob_destroy (info.data);

  data->tables.fini ();

  hb_free (data);
}

സ്ഥിതം hb_blob_t *
_hb_face_builder_data_reference_blob (hb_face_builder_data_t *data)
{

  unsigned പൂർണ്ണസംഖ്യ table_എണ്ണം_2 = data->tables.get_population ();
  unsigned പൂർണ്ണസംഖ്യ face_length = table_എണ്ണം_2 * 16 + 12;

  വേണ്ടി (auto info : data->tables.values())
    face_length += hb_ceil_to_4 (hb_blob_get_length (info.data));

  അക്ഷരം *buf = (അക്ഷരം *) hb_malloc (face_length);
  എങ്കിൽ (unlikely (!buf))
    തിരികെ nullptr;

  hb_serialize_context_t c (buf, face_length);
  c.propagate_error (data->tables);
  OT::OpenTypeFontFile *f = c.start_serialize<OT::OpenTypeFontFile> ();

  തർക്കം is_cff = (data->tables.has (HB_TAG ('C','F','F',' '))
                 || data->tables.has (HB_TAG ('C','F','F','2')));
  hb_tag_t sfnt_tag = is_cff ? OT::OpenTypeFontFile::CFFTag : OT::OpenTypeFontFile::TrueTypeTag;

  // Sort the tags so that produced face is deterministic.
  hb_vector_t<hb_pair_t <hb_tag_t, face_table_info_t>> sorted_entries;
  data->tables.iter () | hb_sink (sorted_entries);
  എങ്കിൽ (unlikely (sorted_entries.in_error ()))
  {
    hb_free (buf);
    തിരികെ nullptr;
  }

  sorted_entries.qsort (compare_entries);

  തർക്കം ret = f->serialize_single (&c,
                                  sfnt_tag,
                                  + sorted_entries.iter()
                                  | hb_map ([&] (hb_pair_t<hb_tag_t, face_table_info_t> _) {
                                    തിരികെ hb_pair_t<hb_tag_t, hb_blob_t*> (_.first, _.second.data);
                                  }));

  c.end_serialize ();

  എങ്കിൽ (unlikely (!ret))
  {
    hb_free (buf);
    തിരികെ nullptr;
  }

  തിരികെ hb_blob_create (buf, face_length, HB_MEMORY_MODE_WRITABLE, buf, hb_free);
}

സ്ഥിതം hb_blob_t *
_hb_face_builder_reference_table (hb_face_t *face HB_UNUSED, hb_tag_t tag, ശൂന്യം *user_data)
{
  hb_face_builder_data_t *data = (hb_face_builder_data_t *) user_data;

  എങ്കിൽ (!tag)
    തിരികെ _hb_face_builder_data_reference_blob (data);

  തിരികെ hb_blob_reference (data->tables[tag].data);
}

സ്ഥിതം unsigned
_hb_face_builder_get_table_tags (സ്ഥിരം hb_face_t *face HB_UNUSED,
				 unsigned പൂർണ്ണസംഖ്യ start_offset,
				 unsigned പൂർണ്ണസംഖ്യ *table_എണ്ണം_2,
				 hb_tag_t *table_tags,
				 ശൂന്യം *user_data)
{
  hb_face_builder_data_t *data = (hb_face_builder_data_t *) user_data;

  unsigned population = data->tables.get_population ();

  എങ്കിൽ (!table_എണ്ണം_2)
    തിരികെ population;

  എങ്കിൽ (unlikely (start_offset >= population))
  {
    *table_എണ്ണം_2 = 0;
    തിരികെ population;
  }

  // Sort the tags.
  hb_vector_t<hb_tag_t> sorted_tags;
  data->tables.താക്കോലുകൾ () | hb_sink (sorted_tags);
  എങ്കിൽ (unlikely (sorted_tags.in_error ()))
  {
    // Not much to do...
  }
  sorted_tags.qsort ([] (സ്ഥിരം hb_tag_t &a, സ്ഥിരം hb_tag_t &b) {
    തിരികെ (a > b) - (a < b);
  });

  auto array = sorted_tags.as_array ().sub_array (start_offset, table_എണ്ണം_2);
  എങ്കിൽ (table_tags)
  {
    auto out = hb_array (table_tags, *table_എണ്ണം_2);

    + array.iter ()
    | hb_sink (out)
    ;
  }

  തിരികെ population;
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
  എങ്കിൽ (unlikely (!data)) തിരികെ hb_face_get_empty ();

  hb_face_t *face = hb_face_create_for_tables (_hb_face_builder_reference_table,
					       data,
					       _hb_face_builder_data_destroy);

  hb_face_set_get_table_tags_func (face,
				   _hb_face_builder_get_table_tags,
				   data,
				   nullptr);

  തിരികെ face;
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
  എങ്കിൽ (unlikely (face->destroy != (hb_destroy_func_t) _hb_face_builder_data_destroy))
    തിരികെ അസത്യം;

  എങ്കിൽ (tag == HB_MAP_മൂല്യം_INVALID)
    തിരികെ അസത്യം;

  hb_face_builder_data_t *data = (hb_face_builder_data_t *) face->user_data;

  hb_blob_t* previous = data->tables.get (tag).data;
  എങ്കിൽ (!data->tables.set (tag, face_table_info_t {hb_blob_reference (blob), (unsigned) -1}))
  {
    hb_blob_destroy (blob);
    തിരികെ അസത്യം;
  }

  hb_blob_destroy (previous);
  തിരികെ സത്യം;
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
ശൂന്യം
hb_face_builder_sort_tables (hb_face_t *face,
                             സ്ഥിരം hb_tag_t  *tags)
{
  എങ്കിൽ (unlikely (face->destroy != (hb_destroy_func_t) _hb_face_builder_data_destroy))
    തിരികെ;

  hb_face_builder_data_t *data = (hb_face_builder_data_t *) face->user_data;

  // Sort all unspecified tables after any specified tables.
  വേണ്ടി (auto& info : data->tables.മൂല്യങ്ങൾ_ref())
    info.order = (unsigned) -1;

  signed order = 0;
  വേണ്ടി (സ്ഥിരം hb_tag_t* tag = tags;
       *tag;
       tag++)
  {
    face_table_info_t* info;
    എങ്കിൽ (!data->tables.has (*tag, &info)) തുടരുക;
    info->order = order++;
  }
}
