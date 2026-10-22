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

#include "hb.hh"

#include "hb-face.hh"

#include "hb-map.hh"
#include "hb-open-file.hh"
#include "hb-serialize.hh"


/*
 * face-builder: A face that has add_table().
 */

struct face_table_info_t
{
  hb_blob_t* data;
  unsigned order;
};

struct hb_face_builder_data_t
{
  hb_hashmap_t<hb_tag_t, face_table_info_t> tables;
};

static цео compare_entries (const void* pa, const void* pb)
{
  const auto& a = * (const hb_pair_t<hb_tag_t, face_table_info_t> *) pa;
  const auto& b = * (const hb_pair_t<hb_tag_t, face_table_info_t> *) pb;

  /* Order by blob size first (smallest to largest) and then table tag */

  ако (a.second.order != b.second.order)
    врати a.second.order < b.second.order ? -1 : +1;

  ако (a.second.data->length != b.second.data->length)
    врати a.second.data->length < b.second.data->length ? -1 : +1;

  врати a.first < b.first ? -1 : a.first == b.first ? 0 : +1;
}

static hb_face_builder_data_t *
_hb_face_builder_data_create ()
{
  hb_face_builder_data_t *data = (hb_face_builder_data_t *) hb_calloc (1, sizeof (hb_face_builder_data_t));
  ако (unlikely (!data))
    врати nullptr;

  data->tables.init ();

  врати data;
}

static void
_hb_face_builder_data_destroy (void *user_data)
{
  hb_face_builder_data_t *data = (hb_face_builder_data_t *) user_data;

  за (auto info : data->tables.values())
    hb_blob_destroy (info.data);

  data->tables.fini ();

  hb_free (data);
}

static hb_blob_t *
_hb_face_builder_data_reference_blob (hb_face_builder_data_t *data)
{

  unsigned цео table_count = data->tables.get_population ();
  unsigned цео face_length = table_count * 16 + 12;

  за (auto info : data->tables.values())
    face_length += hb_ceil_to_4 (hb_blob_get_length (info.data));

  char *buf = (char *) hb_malloc (face_length);
  ако (unlikely (!buf))
    врати nullptr;

  hb_serialize_context_t c (buf, face_length);
  c.propagate_error (data->tables);
  OT::OpenTypeFontFile *f = c.start_serialize<OT::OpenTypeFontFile> ();

  bool is_cff = (data->tables.has (HB_TAG ('C','F','F',' '))
                 || data->tables.has (HB_TAG ('C','F','F','2')));
  hb_tag_t sfnt_tag = is_cff ? OT::OpenTypeFontFile::CFFTag : OT::OpenTypeFontFile::TrueTypeTag;

  // Sort the tags so that produced face is deterministic.
  hb_vector_t<hb_pair_t <hb_tag_t, face_table_info_t>> sorted_entries;
  data->tables.iter () | hb_sink (sorted_entries);
  ако (unlikely (sorted_entries.in_error ()))
  {
    hb_free (buf);
    врати nullptr;
  }

  sorted_entries.qsort (compare_entries);

  bool ret = f->serialize_single (&c,
                                  sfnt_tag,
                                  + sorted_entries.iter()
                                  | hb_map ([&] (hb_pair_t<hb_tag_t, face_table_info_t> _) {
                                    врати hb_pair_t<hb_tag_t, hb_blob_t*> (_.first, _.second.data);
                                  }));

  c.end_serialize ();

  ако (unlikely (!ret))
  {
    hb_free (buf);
    врати nullptr;
  }

  врати hb_blob_create (buf, face_length, HB_MEMORY_MODE_WRITABLE, buf, hb_free);
}

static hb_blob_t *
_hb_face_builder_reference_table (hb_face_t *face HB_UNUSED, hb_tag_t tag, void *user_data)
{
  hb_face_builder_data_t *data = (hb_face_builder_data_t *) user_data;

  ако (!tag)
    врати _hb_face_builder_data_reference_blob (data);

  врати hb_blob_reference (data->tables[tag].data);
}

static unsigned
_hb_face_builder_get_table_tags (const hb_face_t *face HB_UNUSED,
				 unsigned цео start_offset,
				 unsigned цео *table_count,
				 hb_tag_t *table_tags,
				 void *user_data)
{
  hb_face_builder_data_t *data = (hb_face_builder_data_t *) user_data;

  unsigned population = data->tables.get_population ();

  ако (!table_count)
    врати population;

  ако (unlikely (start_offset >= population))
  {
    *table_count = 0;
    врати population;
  }

  // Sort the tags.
  hb_vector_t<hb_tag_t> sorted_tags;
  data->tables.keys () | hb_sink (sorted_tags);
  ако (unlikely (sorted_tags.in_error ()))
  {
    // Not much to do...
  }
  sorted_tags.qsort ([] (const hb_tag_t &a, const hb_tag_t &b) {
    врати (a > b) - (a < b);
  });

  auto array = sorted_tags.as_array ().sub_array (start_offset, table_count);
  ако (table_tags)
  {
    auto out = hb_array (table_tags, *table_count);

    + array.iter ()
    | hb_sink (out)
    ;
  }

  врати population;
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
  ако (unlikely (!data)) врати hb_face_get_empty ();

  hb_face_t *face = hb_face_create_for_tables (_hb_face_builder_reference_table,
					       data,
					       _hb_face_builder_data_destroy);

  hb_face_set_get_table_tags_func (face,
				   _hb_face_builder_get_table_tags,
				   data,
				   nullptr);

  врати face;
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
  ако (unlikely (face->destroy != (hb_destroy_func_t) _hb_face_builder_data_destroy))
    врати false;

  ако (tag == HB_MAP_VALUE_INVALID)
    врати false;

  hb_face_builder_data_t *data = (hb_face_builder_data_t *) face->user_data;

  hb_blob_t* previous = data->tables.get (tag).data;
  ако (!data->tables.set (tag, face_table_info_t {hb_blob_reference (blob), (unsigned) -1}))
  {
    hb_blob_destroy (blob);
    врати false;
  }

  hb_blob_destroy (previous);
  врати true;
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
void
hb_face_builder_sort_tables (hb_face_t *face,
                             const hb_tag_t  *tags)
{
  ако (unlikely (face->destroy != (hb_destroy_func_t) _hb_face_builder_data_destroy))
    врати;

  hb_face_builder_data_t *data = (hb_face_builder_data_t *) face->user_data;

  // Sort all unspecified tables after any specified tables.
  за (auto& info : data->tables.values_ref())
    info.order = (unsigned) -1;

  signed order = 0;
  за (const hb_tag_t* tag = tags;
       *tag;
       tag++)
  {
    face_table_info_t* info;
    ако (!data->tables.has (*tag, &info)) continue;
    info->order = order++;
  }
}
