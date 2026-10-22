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

#समावेश "hb.hh"

#समावेश "hb-face.hh"

#समावेश "hb-map.hh"
#समावेश "hb-open-file.hh"
#समावेश "hb-serialize.hh"


/*
 * face-builder: A face that has add_table().
 */

काष्ठा face_table_info_t
{
  hb_blob_t* तथ्य;
  अचिन्हित order;
};

काष्ठा hb_face_builder_तथ्य_t
{
  hb_hashmap_t<hb_tag_t, face_table_info_t> tables;
};

अटल पूर्णांक compare_entries (स्थिर व्योम* pa, स्थिर व्योम* pb)
{
  स्थिर स्वतः& a = * (स्थिर hb_pair_t<hb_tag_t, face_table_info_t> *) pa;
  स्थिर स्वतः& b = * (स्थिर hb_pair_t<hb_tag_t, face_table_info_t> *) pb;

  /* Order by blob size first (smallest to largest) and then table tag */

  अगर (a.second.order != b.second.order)
    वापस a.second.order < b.second.order ? -1 : +1;

  अगर (a.second.तथ्य->लंबाई != b.second.तथ्य->लंबाई)
    वापस a.second.तथ्य->लंबाई < b.second.तथ्य->लंबाई ? -1 : +1;

  वापस a.first < b.first ? -1 : a.first == b.first ? 0 : +1;
}

अटल hb_face_builder_तथ्य_t *
hb_face_builder_तथ्य_create ()
{
  hb_face_builder_तथ्य_t *तथ्य = (hb_face_builder_तथ्य_t *) hb_calloc (1, माप (hb_face_builder_तथ्य_t));
  अगर (unlikely (!तथ्य))
    वापस nullptr;

  तथ्य->tables.init ();

  वापस तथ्य;
}

अटल व्योम
hb_face_builder_तथ्य_destroy (व्योम *user_तथ्य)
{
  hb_face_builder_तथ्य_t *तथ्य = (hb_face_builder_तथ्य_t *) user_तथ्य;

  क्रम (स्वतः info : तथ्य->tables.values())
    hb_blob_destroy (info.तथ्य);

  तथ्य->tables.fini ();

  hb_free (तथ्य);
}

अटल hb_blob_t *
hb_face_builder_तथ्य_reference_blob (hb_face_builder_तथ्य_t *तथ्य)
{

  अचिन्हित पूर्णांक table_गिनती_2 = तथ्य->tables.get_population ();
  अचिन्हित पूर्णांक face_लंबाई = table_गिनती_2 * 16 + 12;

  क्रम (स्वतः info : तथ्य->tables.values())
    face_लंबाई += hb_ceil_to_4 (hb_blob_get_लंबाई (info.तथ्य));

  अक्षर *buf = (अक्षर *) hb_malloc (face_लंबाई);
  अगर (unlikely (!buf))
    वापस nullptr;

  hb_serialize_context_t c (buf, face_लंबाई);
  c.propagate_error (तथ्य->tables);
  OT::OpenTypeFontFile *f = c.start_serialize<OT::OpenTypeFontFile> ();

  bool is_cff = (तथ्य->tables.has (HB_TAG ('C','F','F',' '))
                 || तथ्य->tables.has (HB_TAG ('C','F','F','2')));
  hb_tag_t sfnt_tag = is_cff ? OT::OpenTypeFontFile::CFFTag : OT::OpenTypeFontFile::TrueTypeTag;

  // Sort the tags so that produced face is deterministic.
  hb_vector_t<hb_pair_t <hb_tag_t, face_table_info_t>> sorted_entries;
  तथ्य->tables.iter () | hb_sink (sorted_entries);
  अगर (unlikely (sorted_entries.in_error ()))
  {
    hb_free (buf);
    वापस nullptr;
  }

  sorted_entries.क्विक (compare_entries);

  bool ret = f->serialize_single (&c,
                                  sfnt_tag,
                                  + sorted_entries.iter()
                                  | hb_map ([&] (hb_pair_t<hb_tag_t, face_table_info_t> _) {
                                    वापस hb_pair_t<hb_tag_t, hb_blob_t*> (_.first, _.second.तथ्य);
                                  }));

  c.end_serialize ();

  अगर (unlikely (!ret))
  {
    hb_free (buf);
    वापस nullptr;
  }

  वापस hb_blob_create (buf, face_लंबाई, HB_MEMORY_MODE_WRITABLE, buf, hb_free);
}

अटल hb_blob_t *
_hb_face_builder_reference_table (hb_face_t *face HB_UNUSED, hb_tag_t tag, व्योम *user_तथ्य)
{
  hb_face_builder_तथ्य_t *तथ्य = (hb_face_builder_तथ्य_t *) user_तथ्य;

  अगर (!tag)
    वापस hb_face_builder_तथ्य_reference_blob (तथ्य);

  वापस hb_blob_reference (तथ्य->tables[tag].तथ्य);
}

अटल अचिन्हित
_hb_face_builder_get_table_tags (स्थिर hb_face_t *face HB_UNUSED,
				 अचिन्हित पूर्णांक start_offset,
				 अचिन्हित पूर्णांक *table_गिनती_2,
				 hb_tag_t *table_tags,
				 व्योम *user_तथ्य)
{
  hb_face_builder_तथ्य_t *तथ्य = (hb_face_builder_तथ्य_t *) user_तथ्य;

  अचिन्हित population = तथ्य->tables.get_population ();

  अगर (!table_गिनती_2)
    वापस population;

  अगर (unlikely (start_offset >= population))
  {
    *table_गिनती_2 = 0;
    वापस population;
  }

  // Sort the tags.
  hb_vector_t<hb_tag_t> sorted_tags;
  तथ्य->tables.कुंजियाँ () | hb_sink (sorted_tags);
  अगर (unlikely (sorted_tags.in_error ()))
  {
    // Not much to do...
  }
  sorted_tags.क्विक ([] (स्थिर hb_tag_t &a, स्थिर hb_tag_t &b) {
    वापस (a > b) - (a < b);
  });

  स्वतः array = sorted_tags.as_array ().sub_array (start_offset, table_गिनती_2);
  अगर (table_tags)
  {
    स्वतः out = hb_array (table_tags, *table_गिनती_2);

    + array.iter ()
    | hb_sink (out)
    ;
  }

  वापस population;
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
  hb_face_builder_तथ्य_t *तथ्य = hb_face_builder_तथ्य_create ();
  अगर (unlikely (!तथ्य)) वापस hb_face_get_empty ();

  hb_face_t *face = hb_face_create_for_tables (_hb_face_builder_reference_table,
					       तथ्य,
					       hb_face_builder_तथ्य_destroy);

  hb_face_set_get_table_tags_func (face,
				   _hb_face_builder_get_table_tags,
				   तथ्य,
				   nullptr);

  वापस face;
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
  अगर (unlikely (face->destroy != (hb_destroy_func_t) hb_face_builder_तथ्य_destroy))
    वापस false;

  अगर (tag == HB_MAP_मूल्य_INVALID)
    वापस false;

  hb_face_builder_तथ्य_t *तथ्य = (hb_face_builder_तथ्य_t *) face->user_तथ्य;

  hb_blob_t* previous = तथ्य->tables.लो (tag).तथ्य;
  अगर (!तथ्य->tables.set (tag, face_table_info_t {hb_blob_reference (blob), (अचिन्हित) -1}))
  {
    hb_blob_destroy (blob);
    वापस false;
  }

  hb_blob_destroy (previous);
  वापस true;
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
व्योम
hb_face_builder_sort_tables (hb_face_t *face,
                             स्थिर hb_tag_t  *tags)
{
  अगर (unlikely (face->destroy != (hb_destroy_func_t) hb_face_builder_तथ्य_destroy))
    वापस;

  hb_face_builder_तथ्य_t *तथ्य = (hb_face_builder_तथ्य_t *) face->user_तथ्य;

  // Sort all unspecified tables after any specified tables.
  क्रम (स्वतः& info : तथ्य->tables.मूल्यावली_ref())
    info.order = (अचिन्हित) -1;

  चिन्हित order = 0;
  क्रम (स्थिर hb_tag_t* tag = tags;
       *tag;
       tag++)
  {
    face_table_info_t* info;
    अगर (!तथ्य->tables.has (*tag, &info)) जारी;
    info->order = order++;
  }
}
