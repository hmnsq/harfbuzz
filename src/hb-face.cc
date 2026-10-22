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
#अन्तर्भाव "hb-blob.hh"
#अन्तर्भाव "hb-open-file.hh"
#अन्तर्भाव "hb-ot-face.hh"
#अन्तर्भाव "hb-ot-cmap-table.hh"

#ifdef HAVE_FREETYPE
#अन्तर्भाव "hb-ft.h"
#endif
#ifdef HAVE_CORETEXT
#अन्तर्भाव "hb-coretext.h"
#endif
#ifdef HAVE_DIRECTWRITE
#अन्तर्भाव "hb-directwrite.h"
#endif


/**
 * SECTION:hb-face
 * @title: hb-face
 * @short_description: Font face objects
 * @include: hb.h
 *
 * A font face is an object that represents a single face from within a
 * font family.
 *
 * More precisely, a font face represents a single face in a binary font file.
 * Font faces are typically built from a binary blob and a face index.
 * Font faces are used to create fonts.
 *
 * A font face can be created from a binary blob using hb_face_create().
 * The face index is used to select a face from a binary blob that contains
 * multiple faces.  For example, a binary blob that contains both a regular
 * and a bold face can be used to create two font faces, one for each face
 * index.
 **/


/**
 * hb_face_count:
 * @blob: a blob.
 *
 * Fetches the number of faces in a blob.
 *
 * Return value: Number of faces in @blob
 *
 * Since: 1.7.7
 **/
unsigned पूर्णाङ्क
hb_face_गिनती (hb_blob_t *blob)
{
  यदि (unlikely (!blob))
    प्रत्यागम 0;

  hb_sanitize_context_t c (blob);

  auto *ot = blob->as<OT::OpenTypeFontFile> ();
  यदि (unlikely (!ot->sanitize (&c)))
    प्रत्यागम 0;

  प्रत्यागम ot->get_face_गिनती ();
}

/*
 * hb_face_t
 */

DEFINE_NULL_INSTANCE (hb_face_t) =
{
  HB_OBJECT_HEADER_STATIC,

  0,    /* index */
  1000, /* upem */
  0,    /* num_glyphs */

  /* Zero for the rest is fine. */
};


/**
 * hb_face_create_for_tables:
 * @reference_table_func: (closure user_data) (destroy destroy) (scope notified): Table-referencing function
 * @user_data: A pointer to the user data
 * @destroy: (nullable): A callback to call when @data is not needed anymore
 *
 * Variant of hb_face_create(), built for those cases where it is more
 * convenient to provide data for individual tables instead of the whole font
 * data. With the caveat that hb_face_get_table_tags() would not work
 * with faces created this way. You can address that by calling the
 * hb_face_set_get_table_tags_func() function and setting the appropriate callback.
 *
 * Creates a new face object from the specified @user_data and @reference_table_func,
 * with the @destroy callback.
 *
 * Return value: (transfer full): The new face object
 *
 * Since: 0.9.2
 **/
hb_face_t *
hb_face_create_for_tables (hb_reference_table_func_t  reference_table_func,
			   शून्य                      *user_data,
			   hb_destroy_func_t          destroy)
{
  hb_face_t *face;

  यदि (!reference_table_func || !(face = hb_object_create<hb_face_t> ())) {
    यदि (destroy)
      destroy (user_data);
    प्रत्यागम hb_face_get_empty ();
  }

  face->reference_table_func = reference_table_func;
  face->user_data = user_data;
  face->destroy = destroy;

  face->अंक_glyphs = -1;

  face->data.init0 (face);
  face->table.init0 (face);

  प्रत्यागम face;
}


प्रकारनिर्देश संरचना hb_face_for_data_closure_t {
  hb_blob_t *blob;
  uint16_t  index;
} hb_face_for_data_closure_t;

स्थिर hb_face_for_data_closure_t *
_hb_face_for_data_closure_create (hb_blob_t *blob, unsigned पूर्णाङ्क index)
{
  hb_face_for_data_closure_t *closure;

  closure = (hb_face_for_data_closure_t *) hb_calloc (1, परिमाण (hb_face_for_data_closure_t));
  यदि (unlikely (!closure))
    प्रत्यागम nullptr;

  closure->blob = blob;
  closure->index = (uint16_t) (index & 0xFFFFu);

  प्रत्यागम closure;
}

स्थिर शून्य
_hb_face_for_data_closure_destroy (शून्य *data)
{
  hb_face_for_data_closure_t *closure = (hb_face_for_data_closure_t *) data;

  hb_blob_destroy (closure->blob);
  hb_free (closure);
}

स्थिर hb_blob_t *
_hb_face_for_data_reference_table (hb_face_t *face HB_UNUSED, hb_tag_t tag, शून्य *user_data)
{
  hb_face_for_data_closure_t *data = (hb_face_for_data_closure_t *) user_data;

  यदि (tag == HB_TAG_NONE)
    प्रत्यागम hb_blob_reference (data->blob);

  ध्रुव OT::OpenTypeFontFile &ot_file = *data->blob->as<OT::OpenTypeFontFile> ();
  unsigned पूर्णाङ्क base_offset;
  ध्रुव OT::OpenTypeFontFace &ot_face = ot_file.get_face (data->index, &base_offset);

  ध्रुव OT::OpenTypeTable &table = ot_face.get_table_by_tag (tag);

  hb_blob_t *blob = hb_blob_create_sub_blob (data->blob, base_offset + table.offset, table.length);

  प्रत्यागम blob;
}

स्थिर unsigned
_hb_face_for_data_get_table_tags (ध्रुव hb_face_t *face HB_UNUSED,
				  unsigned पूर्णाङ्क start_offset,
				  unsigned पूर्णाङ्क *table_गिनती_2,
				  hb_tag_t *table_tags,
				  शून्य *user_data)
{
  hb_face_for_data_closure_t *data = (hb_face_for_data_closure_t *) user_data;

  ध्रुव OT::OpenTypeFontFile &ot_file = *data->blob->as<OT::OpenTypeFontFile> ();
  ध्रुव OT::OpenTypeFontFace &ot_face = ot_file.get_face (data->index);

  प्रत्यागम ot_face.get_table_tags (start_offset, table_गिनती_2, table_tags);
}


/**
 * hb_face_create:
 * @blob: #hb_blob_t to work upon
 * @index: The index of the face within @blob
 *
 * Constructs a new face object from the specified blob and
 * a face index into that blob.
 *
 * The face index is used for blobs of file formats such as TTC and
 * DFont that can contain more than one face.  Face indices within
 * such collections are zero-based.
 *
 * <note>Note: If the blob font format is not a collection, @index
 * is ignored.  Otherwise, only the lower 16-bits of @index are used.
 * The unmodified @index can be accessed via hb_face_get_index().</note>
 *
 * <note>Note: The high 16-bits of @index, if non-zero, are used by
 * hb_font_create() to load named-instances in variable fonts.  See
 * hb_font_create() for details.</note>
 *
 * Return value: (transfer full): The new face object
 *
 * Since: 0.9.2
 **/
hb_face_t *
hb_face_create (hb_blob_t    *blob,
		unsigned पूर्णाङ्क  index)
{
  hb_face_t *face;

  यदि (unlikely (!blob))
    blob = hb_blob_get_empty ();

  blob = hb_sanitize_context_t ().sanitize_blob<OT::OpenTypeFontFile> (hb_blob_reference (blob));

  hb_face_for_data_closure_t *closure = _hb_face_for_data_closure_create (blob, index);

  यदि (unlikely (!closure))
  {
    hb_blob_destroy (blob);
    प्रत्यागम hb_face_get_empty ();
  }

  face = hb_face_create_for_tables (_hb_face_for_data_reference_table,
				    closure,
				    _hb_face_for_data_closure_destroy);
  hb_face_set_get_table_tags_func (face,
				   _hb_face_for_data_get_table_tags,
				   closure,
				   nullptr);

  face->index = index;

  प्रत्यागम face;
}

/**
 * hb_face_create_or_fail:
 * @blob: #hb_blob_t to work upon
 * @index: The index of the face within @blob
 *
 * Like hb_face_create(), but returns `NULL` if the blob data
 * contains no usable font face at the specified index.
 *
 * Return value: (transfer full): The new face object, or `NULL` if
 * no face is found at the specified index.
 *
 * Since: 10.1.0
 **/
hb_face_t *
hb_face_create_or_fail (hb_blob_t    *blob,
			unsigned पूर्णाङ्क  index)
{
  unsigned अंक_faces = hb_face_गिनती (blob);
  यदि (index >= अंक_faces)
    प्रत्यागम nullptr;

  hb_face_t *face = hb_face_create (blob, index);
  यदि (hb_object_is_immutable (face))
    प्रत्यागम nullptr;

  प्रत्यागम face;
}

#ifndef HB_NO_OPEN
/**
 * hb_face_create_from_file_or_fail:
 * @file_name: A font filename
 * @index: The index of the face within the file
 *
 * A thin wrapper around hb_blob_create_from_file_or_fail()
 * followed by hb_face_create_or_fail().
 *
 * Return value: (transfer full): The new face object, or `NULL` if
 * no face is found at the specified index or the file cannot be read.
 *
 * Since: 10.1.0
 **/
HB_EXTERN hb_face_t *
hb_face_create_from_file_or_fail (ध्रुव वर्ण   *file_name,
				  unsigned पूर्णाङ्क  index)
{
  hb_blob_t *blob = hb_blob_create_from_file_or_fail (file_name);
  यदि (unlikely (!blob))
    प्रत्यागम nullptr;

  hb_face_t *face = hb_face_create_or_fail (blob, index);
  hb_blob_destroy (blob);

  प्रत्यागम face;
}

स्थिर ध्रुव संरचना supported_face_loaders_t {
	वर्ण name[16];
	hb_face_t * (*from_file) (ध्रुव वर्ण *font_file, unsigned face_index);
	hb_face_t * (*from_blob) (hb_blob_t *blob, unsigned face_index);
} supported_face_loaders[] =
{
  {"ot",
#ifndef HB_NO_OPEN
   hb_face_create_from_file_or_fail,
#अन्यथा
   nullptr,
#endif
   hb_face_create_or_fail
  },
#ifdef HAVE_FREETYPE
  {"ft",
   hb_ft_face_create_from_file_or_fail,
   hb_ft_face_create_from_blob_or_fail
  },
#endif
#ifdef HAVE_CORETEXT
  {"coretext",
   hb_coretext_face_create_from_file_or_fail,
   hb_coretext_face_create_from_blob_or_fail
  },
#endif
#ifdef HAVE_DIRECTWRITE
  {"directwrite",
   hb_directwrite_face_create_from_file_or_fail,
   hb_directwrite_face_create_from_blob_or_fail
  },
#endif
};

स्थिर ध्रुव वर्ण *get_default_loader_name ()
{
  स्थिर hb_atomic_t<ध्रुव वर्ण *> static_loader_name;
  ध्रुव वर्ण *loader_name = static_loader_name.get_acquire ();
  यदि (!loader_name)
  {
    loader_name = getenv ("HB_FACE_LOADER");
    यदि (!loader_name)
      loader_name = "";
    यदि (!static_loader_name.cmpexch (nullptr, loader_name))
      loader_name = static_loader_name.get_acquire ();
  }
  प्रत्यागम loader_name;
}

/**
 * hb_face_create_from_file_or_fail_using:
 * @file_name: A font filename
 * @index: The index of the face within the file
 * @loader_name: (nullable): The name of the loader to use, or `NULL`
 *
 * A thin wrapper around the face loader functions registered with HarfBuzz.
 * If @loader_name is `NULL` or the empty string, the first available loader
 * is used.
 *
 * For example, the FreeType ("ft") loader might be able to load
 * WOFF and WOFF2 files if FreeType is built with those features,
 * whereas the OpenType ("ot") loader will not.
 *
 * Return value: (transfer full): The new face object, or `NULL` if
 * the file cannot be read or the loader fails to load the face.
 *
 * Since: 11.0.0
 **/
hb_face_t *
hb_face_create_from_file_or_fail_using (ध्रुव वर्ण   *file_name,
					unsigned पूर्णाङ्क  index,
					ध्रुव वर्ण   *loader_name)
{
  // Duplicated in hb_face_create_or_fail_using
  तार्किक retry = असत्य;
  यदि (!loader_name || !*loader_name)
  {
    loader_name = get_default_loader_name ();
    retry = सत्य;
  }
  यदि (loader_name && !*loader_name) loader_name = nullptr;

retry:
  प्रति (unsigned i = 0; i < ARRAY_LENGTH (supported_face_loaders); i++)
  {
    यदि (!loader_name || (supported_face_loaders[i].from_file && !strcmp (supported_face_loaders[i].name, loader_name)))
      प्रत्यागम supported_face_loaders[i].from_file (file_name, index);
  }

  यदि (retry)
  {
    retry = असत्य;
    loader_name = nullptr;
    गच्छ retry;
  }

  प्रत्यागम nullptr;
}

/**
 * hb_face_create_or_fail_using:
 * @blob: #hb_blob_t to work upon
 * @index: The index of the face within @blob
 * @loader_name: (nullable): The name of the loader to use, or `NULL`
 *
 * A thin wrapper around the face loader functions registered with HarfBuzz.
 * If @loader_name is `NULL` or the empty string, the first available loader
 * is used.
 *
 * For example, the FreeType ("ft") loader might be able to load
 * WOFF and WOFF2 files if FreeType is built with those features,
 * whereas the OpenType ("ot") loader will not.
 *
 * Return value: (transfer full): The new face object, or `NULL` if
 * the loader fails to load the face.
 *
 * Since: 11.0.0
 **/
hb_face_t *
hb_face_create_or_fail_using (hb_blob_t    *blob,
			      unsigned पूर्णाङ्क  index,
			      ध्रुव वर्ण   *loader_name)
{
  // Duplicated in hb_face_create_from_file_or_fail_using
  तार्किक retry = असत्य;
  यदि (!loader_name || !*loader_name)
  {
    loader_name = get_default_loader_name ();
    retry = सत्य;
  }
  यदि (loader_name && !*loader_name) loader_name = nullptr;

retry:
  प्रति (unsigned i = 0; i < ARRAY_LENGTH (supported_face_loaders); i++)
  {
    यदि (!loader_name || (supported_face_loaders[i].from_blob && !strcmp (supported_face_loaders[i].name, loader_name)))
      प्रत्यागम supported_face_loaders[i].from_blob (blob, index);
  }

  यदि (retry)
  {
    retry = असत्य;
    loader_name = nullptr;
    गच्छ retry;
  }

  प्रत्यागम nullptr;
}

स्थिर अन्तःस्थ शून्य free_static_face_loader_list ();

स्थिर ध्रुव वर्ण * ध्रुव nil_face_loader_list[] = {nullptr};

स्थिर संरचना hb_face_loader_list_lazy_loader_t : hb_lazy_loader_t<ध्रुव वर्ण *,
								  hb_face_loader_list_lazy_loader_t>
{
  स्थिर ध्रुव वर्ण ** create ()
  {
    ध्रुव वर्ण **face_loader_list = (ध्रुव वर्ण **) hb_calloc (1 + ARRAY_LENGTH (supported_face_loaders), परिमाण (ध्रुव वर्ण *));
    यदि (unlikely (!face_loader_list))
      प्रत्यागम nullptr;

    unsigned i;
    प्रति (i = 0; i < ARRAY_LENGTH (supported_face_loaders); i++)
      face_loader_list[i] = supported_face_loaders[i].name;
    face_loader_list[i] = nullptr;

    hb_atexit (free_static_face_loader_list);

    प्रत्यागम face_loader_list;
  }
  स्थिर शून्य destroy (ध्रुव वर्ण **l)
  { hb_free (l); }
  स्थिर ध्रुव वर्ण * ध्रुव * get_null ()
  { प्रत्यागम nil_face_loader_list; }
} static_face_loader_list;

स्थिर अन्तःस्थ
शून्य free_static_face_loader_list ()
{
  static_face_loader_list.free_instance ();
}

/**
 * hb_face_list_loaders:
 *
 * Retrieves the list of face loaders supported by HarfBuzz.
 *
 * Return value: (transfer none) (array zero-terminated=1): a
 *    `NULL`-terminated array of supported face loaders
 *    constant strings. The returned array is owned by HarfBuzz
 *    and should not be modified or freed.
 *
 * Since: 11.0.0
 **/
ध्रुव वर्ण **
hb_face_list_loaders ()
{
  प्रत्यागम static_face_loader_list.get_unconst ();
}
#endif


/**
 * hb_face_get_empty:
 *
 * Fetches the singleton empty face object.
 *
 * Return value: (transfer full): The empty face object
 *
 * Since: 0.9.2
 **/
hb_face_t *
hb_face_get_empty ()
{
  प्रत्यागम const_cast<hb_face_t *> (&Null (hb_face_t));
}


/**
 * hb_face_reference: (skip)
 * @face: A face object
 *
 * Increases the reference count on a face object.
 *
 * Return value: The @face object
 *
 * Since: 0.9.2
 **/
hb_face_t *
hb_face_reference (hb_face_t *face)
{
  प्रत्यागम hb_object_reference (face);
}

/**
 * hb_face_destroy: (skip)
 * @face: A face object
 *
 * Decreases the reference count on a face object. When the
 * reference count reaches zero, the face is destroyed,
 * freeing all memory.
 *
 * Since: 0.9.2
 **/
शून्य
hb_face_destroy (hb_face_t *face)
{
  यदि (!hb_object_destroy (face)) प्रत्यागम;

#ifndef HB_NO_SHAPER
  प्रति (hb_face_t::plan_गाँठ_t *node = face->shape_plans; node; )
  {
    hb_face_t::plan_गाँठ_t *next = node->next;
    hb_shape_plan_destroy (node->shape_plan);
    hb_free (node);
    node = next;
  }
#endif

  face->data.fini ();
  face->table.fini ();

  यदि (face->get_table_tags_destroy)
    face->get_table_tags_destroy (face->get_table_tags_user_data);

  यदि (face->destroy)
    face->destroy (face->user_data);

  hb_free (face);
}

/**
 * hb_face_set_user_data: (skip)
 * @face: A face object
 * @key: The user-data key to set
 * @data: A pointer to the user data
 * @destroy: (nullable): A callback to call when @data is not needed anymore
 * @replace: Whether to replace an existing data with the same key
 *
 * Attaches a user-data key/data pair to the given face object.
 *
 * Return value: `true` if success, `false` otherwise
 *
 * Since: 0.9.2
 **/
hb_bool_t
hb_face_set_user_data (hb_face_t          *face,
		       hb_user_data_कुंजी_t *key,
		       शून्य *              data,
		       hb_destroy_func_t   destroy,
		       hb_bool_t           replace)
{
  प्रत्यागम hb_object_set_user_data (face, key, data, destroy, replace);
}

/**
 * hb_face_get_user_data: (skip)
 * @face: A face object
 * @key: The user-data key to query
 *
 * Fetches the user data associated with the specified key,
 * attached to the specified face object.
 *
 * Return value: (transfer none): A pointer to the user data
 *
 * Since: 0.9.2
 **/
शून्य *
hb_face_get_user_data (ध्रुव hb_face_t    *face,
		       hb_user_data_कुंजी_t *key)
{
  प्रत्यागम hb_object_get_user_data (face, key);
}

/**
 * hb_face_make_immutable:
 * @face: A face object
 *
 * Makes the given face object immutable.
 *
 * Since: 0.9.2
 **/
शून्य
hb_face_make_immutable (hb_face_t *face)
{
  यदि (hb_object_is_immutable (face))
    प्रत्यागम;

  hb_object_make_immutable (face);
}

/**
 * hb_face_is_immutable:
 * @face: A face object
 *
 * Tests whether the given face object is immutable.
 *
 * Return value: `true` is @face is immutable, `false` otherwise
 *
 * Since: 0.9.2
 **/
hb_bool_t
hb_face_is_immutable (hb_face_t *face)
{
  प्रत्यागम hb_object_is_immutable (face);
}


/**
 * hb_face_reference_table:
 * @face: A face object
 * @tag: The #hb_tag_t of the table to query
 *
 * Fetches a reference to the specified table within
 * the specified face. Returns an empty blob if referencing table data is not
 * possible.
 *
 * Return value: (transfer full): A pointer to the @tag table within @face
 *
 * Since: 0.9.2
 **/
hb_blob_t *
hb_face_reference_table (ध्रुव hb_face_t *face,
			 hb_tag_t tag)
{
  यदि (unlikely (tag == HB_TAG_NONE))
    प्रत्यागम hb_blob_get_empty ();

  प्रत्यागम face->reference_table (tag);
}

/**
 * hb_face_reference_blob:
 * @face: A face object
 *
 * Fetches a pointer to the binary blob that contains the specified face.
 * If referencing the face data is not possible, this function creates a blob
 * out of individual table blobs if hb_face_get_table_tags() works with this
 * face, otherwise it returns an empty blob.
 *
 * Return value: (transfer full): A pointer to the blob for @face
 *
 * Since: 0.9.2
 **/
hb_blob_t *
hb_face_reference_blob (hb_face_t *face)
{
  hb_blob_t *blob = face->reference_table (HB_TAG_NONE);

  यदि (blob == hb_blob_get_empty ())
  {
    // If referencing the face blob is not possible (e.g. not implemented by the
    // font functions), use face builder to create a blob out of individual
    // table blobs.
    unsigned total_गिनती = hb_face_get_table_tags (face, 0, nullptr, nullptr);
    यदि (total_गिनती)
    {
      hb_tag_t tags[64];
      unsigned गिनती = ARRAY_LENGTH (tags);
      hb_face_t* builder = hb_face_builder_create ();

      प्रति (unsigned offset = 0; offset < total_गिनती; offset += गिनती)
      {
        hb_face_get_table_tags (face, offset, &गिनती, tags);
	यदि (unlikely (!गिनती))
	  भङ्ग; // Allocation error
        प्रति (unsigned i = 0; i < गिनती; i++)
        {
	  यदि (unlikely (!tags[i]))
	    अनुवर्त;
	  hb_blob_t *table = hb_face_reference_table (face, tags[i]);
	  hb_face_builder_add_table (builder, tags[i], table);
	  hb_blob_destroy (table);
        }
      }

      blob = hb_face_reference_blob (builder);
      hb_face_destroy (builder);
    }
  }

  प्रत्यागम blob;
}

/**
 * hb_face_set_index:
 * @face: A face object
 * @index: The index to assign
 *
 * Assigns the specified face-index to @face. Fails if the
 * face is immutable.
 *
 * <note>Note: changing the index has no effect on the face itself
 * This only changes the value returned by hb_face_get_index().</note>
 *
 * Since: 0.9.2
 **/
शून्य
hb_face_set_index (hb_face_t    *face,
		   unsigned पूर्णाङ्क  index)
{
  यदि (hb_object_is_immutable (face))
    प्रत्यागम;

  face->index = index;
}

/**
 * hb_face_get_index:
 * @face: A face object
 *
 * Fetches the face-index corresponding to the given face.
 *
 * <note>Note: face indices within a collection are zero-based.</note>
 *
 * Return value: The index of @face.
 *
 * Since: 0.9.2
 **/
unsigned पूर्णाङ्क
hb_face_get_index (ध्रुव hb_face_t *face)
{
  प्रत्यागम face->index;
}

/**
 * hb_face_set_upem:
 * @face: A face object
 * @upem: The units-per-em value to assign
 *
 * Sets the units-per-em (upem) for a face object to the specified value.
 *
 * This API is used in rare circumstances.
 *
 * Since: 0.9.2
 **/
शून्य
hb_face_set_upem (hb_face_t    *face,
		  unsigned पूर्णाङ्क  upem)
{
  यदि (hb_object_is_immutable (face))
    प्रत्यागम;

  face->upem = upem;
}

/**
 * hb_face_get_upem:
 * @face: A face object
 *
 * Fetches the units-per-em (UPEM) value of the specified face object.
 *
 * Typical UPEM values for fonts are 1000, or 2048, but any value
 * in between 16 and 16,384 is allowed for OpenType fonts.
 *
 * Return value: The upem value of @face
 *
 * Since: 0.9.2
 **/
unsigned पूर्णाङ्क
hb_face_get_upem (ध्रुव hb_face_t *face)
{
  प्रत्यागम face->get_upem ();
}

/**
 * hb_face_set_glyph_count:
 * @face: A face object
 * @glyph_count: The glyph-count value to assign
 *
 * Sets the glyph count for a face object to the specified value.
 *
 * This API is used in rare circumstances.
 *
 * Since: 0.9.7
 **/
शून्य
hb_face_set_glyph_गिनती (hb_face_t    *face,
			 unsigned पूर्णाङ्क  glyph_गिनती)
{
  यदि (hb_object_is_immutable (face))
    प्रत्यागम;

  face->अंक_glyphs = glyph_गिनती;
}

/**
 * hb_face_get_glyph_count:
 * @face: A face object
 *
 * Fetches the glyph-count value of the specified face object.
 *
 * Return value: The glyph-count value of @face
 *
 * Since: 0.9.7
 **/
unsigned पूर्णाङ्क
hb_face_get_glyph_गिनती (ध्रुव hb_face_t *face)
{
  प्रत्यागम face->get_अंक_glyphs ();
}

/**
 * hb_face_set_get_table_tags_func:
 * @face: A face object
 * @func: (closure user_data) (destroy destroy) (scope notified): The table-tag-fetching function
 * @user_data: A pointer to the user data, to be destroyed by @destroy when not needed anymore
 * @destroy: (nullable): A callback to call when @func is not needed anymore
 *
 * Sets the table-tag-fetching function for the specified face object.
 *
 * Since: 10.0.0
 */
HB_EXTERN शून्य
hb_face_set_get_table_tags_func (hb_face_t *face,
				 hb_get_table_tags_func_t func,
				 शून्य                    *user_data,
				 hb_destroy_func_t        destroy)
{
  यदि (hb_object_is_immutable (face))
  {
    यदि (destroy)
      destroy (user_data);
    प्रत्यागम;
  }

  यदि (face->get_table_tags_destroy)
    face->get_table_tags_destroy (face->get_table_tags_user_data);

  face->get_table_tags_func = func;
  face->get_table_tags_user_data = user_data;
  face->get_table_tags_destroy = destroy;
}

/**
 * hb_face_get_table_tags:
 * @face: A face object
 * @start_offset: The index of first table tag to retrieve
 * @table_count: (inout): Input = the maximum number of table tags to return;
 *                Output = the actual number of table tags returned (may be zero)
 * @table_tags: (out) (array length=table_count) (nullable): The array of table tags found
 *
 * Fetches a list of all table tags for a face, if possible. The list returned will
 * begin at the offset provided
 *
 * Return value: Total number of tables, or zero if it is not possible to list
 *
 * Since: 1.6.0
 **/
unsigned पूर्णाङ्क
hb_face_get_table_tags (ध्रुव hb_face_t *face,
			unsigned पूर्णाङ्क  start_offset,
			unsigned पूर्णाङ्क *table_गिनती_2, /* IN/OUT */
			hb_tag_t     *table_tags /* OUT */)
{
  यदि (!face->get_table_tags_func)
  {
    यदि (table_गिनती_2)
      *table_गिनती_2 = 0;
    प्रत्यागम 0;
  }

  प्रत्यागम face->get_table_tags_func (face, start_offset, table_गिनती_2, table_tags, face->get_table_tags_user_data);
}


/*
 * Character set.
 */


#ifndef HB_NO_FACE_COLLECT_UNICODES
/**
 * hb_face_collect_unicodes:
 * @face: A face object
 * @out: (out): The set to add Unicode characters to
 *
 * Collects all of the Unicode characters covered by @face and adds
 * them to the #hb_set_t set @out.
 *
 * Since: 1.9.0
 */
शून्य
hb_face_collect_unicodes (hb_face_t *face,
			  hb_set_t  *out)
{
  face->table.cmap->collect_unicodes (out, face->get_अंक_glyphs ());
}
/**
 * hb_face_collect_nominal_glyph_mapping:
 * @face: A face object
 * @mapping: (out): The map to add Unicode-to-glyph mapping to
 * @unicodes: (nullable) (out): The set to add Unicode characters to, or `NULL`
 *
 * Collects the mapping from Unicode characters to nominal glyphs of the @face,
 * and optionally all of the Unicode characters covered by @face.
 *
 * Since: 7.0.0
 */
शून्य
hb_face_collect_nominal_glyph_mapping (hb_face_t *face,
				       hb_map_t  *mapping,
				       hb_set_t  *unicodes)
{
  hb_set_t ढेर_unicodes;
  यदि (!unicodes)
    unicodes = &ढेर_unicodes;
  face->table.cmap->collect_mapping (unicodes, mapping, face->get_अंक_glyphs ());
}
/**
 * hb_face_collect_variation_selectors:
 * @face: A face object
 * @out: (out): The set to add Variation Selector characters to
 *
 * Collects all Unicode "Variation Selector" characters covered by @face and adds
 * them to the #hb_set_t set @out.
 *
 * Since: 1.9.0
 */
शून्य
hb_face_collect_variation_selectors (hb_face_t *face,
				     hb_set_t  *out)
{
  face->table.cmap->collect_variation_selectors (out);
}
/**
 * hb_face_collect_variation_unicodes:
 * @face: A face object
 * @variation_selector: The Variation Selector to query
 * @out: (out): The set to add Unicode characters to
 *
 * Collects all Unicode characters for @variation_selector covered by @face and adds
 * them to the #hb_set_t set @out.
 *
 * Since: 1.9.0
 */
शून्य
hb_face_collect_variation_unicodes (hb_face_t *face,
				    hb_codepoint_t variation_selector,
				    hb_set_t  *out)
{
  face->table.cmap->collect_variation_unicodes (variation_selector, out);
}
#endif
