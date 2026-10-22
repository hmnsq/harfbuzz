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
#অন্তর্ভাব "hb-blob.hh"
#অন্তর্ভাব "hb-open-file.hh"
#অন্তর্ভাব "hb-ot-face.hh"
#অন্তর্ভাব "hb-ot-cmap-table.hh"

#ifdef HAVE_FREETYPE
#অন্তর্ভাব "hb-ft.h"
#endif
#ifdef HAVE_CORETEXT
#অন্তর্ভাব "hb-coretext.h"
#endif
#ifdef HAVE_DIRECTWRITE
#অন্তর্ভাব "hb-directwrite.h"
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
unsigned পূর্ণসংখ্যা
hb_face_গুনতি (hb_blob_t *blob)
{
  যদি (unlikely (!blob))
    ফেরত 0;

  hb_sanitize_context_t c (blob);

  auto *ot = blob->as<OT::OpenTypeFontFile> ();
  যদি (unlikely (!ot->sanitize (&c)))
    ফেরত 0;

  ফেরত ot->get_face_গুনতি ();
}

/*
 * hb_face_t
 */

DEFINE_NULL_INSTANCE (hb_face_t) =
{
  HB_OBJECT_HEADER_STATIC,

  0,    /*সূচক*/
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
			   শূন্য                      *user_তথ্য,
			   hb_destroy_func_t          destroy)
{
  hb_face_t *face;

  যদি (!reference_table_func || !(face = hb_object_create<hb_face_t> ())) {
    যদি (destroy)
      destroy (user_তথ্য);
    ফেরত hb_face_get_empty ();
  }

  face->reference_table_func = reference_table_func;
  face->user_তথ্য = user_তথ্য;
  face->destroy = destroy;

  face->অঙ্ক_glyphs = -1;

  face->তথ্য.init0 (face);
  face->table.init0 (face);

  ফেরত face;
}


প্রকারলেখ কাঠামো hb_face_for_তথ্য_closure_t {
  hb_blob_t *blob;
  uint16_t  সূচক_2;
} hb_face_for_তথ্য_closure_t;

স্থির hb_face_for_তথ্য_closure_t *
hb_face_for_তথ্য_closure_create (hb_blob_t *blob, unsigned পূর্ণসংখ্যা সূচক_2)
{
  hb_face_for_তথ্য_closure_t *closure;

  closure = (hb_face_for_তথ্য_closure_t *) hb_calloc (1, আকার (hb_face_for_তথ্য_closure_t));
  যদি (unlikely (!closure))
    ফেরত nullptr;

  closure->blob = blob;
  closure->সূচক_2 = (uint16_t) (সূচক_2 & 0xFFFFu);

  ফেরত closure;
}

স্থির শূন্য
hb_face_for_তথ্য_closure_destroy (শূন্য *তথ্য)
{
  hb_face_for_তথ্য_closure_t *closure = (hb_face_for_তথ্য_closure_t *) তথ্য;

  hb_blob_destroy (closure->blob);
  hb_free (closure);
}

স্থির hb_blob_t *
hb_face_for_তথ্য_reference_table (hb_face_t *face HB_UNUSED, hb_tag_t tag, শূন্য *user_তথ্য)
{
  hb_face_for_তথ্য_closure_t *তথ্য = (hb_face_for_তথ্য_closure_t *) user_তথ্য;

  যদি (tag == HB_TAG_NONE)
    ফেরত hb_blob_reference (তথ্য->blob);

  ধ্রুবক OT::OpenTypeFontFile &ot_file = *তথ্য->blob->as<OT::OpenTypeFontFile> ();
  unsigned পূর্ণসংখ্যা base_offset;
  ধ্রুবক OT::OpenTypeFontFace &ot_face = ot_file.get_face (তথ্য->সূচক_2, &base_offset);

  ধ্রুবক OT::OpenTypeTable &table = ot_face.get_table_by_tag (tag);

  hb_blob_t *blob = hb_blob_create_sub_blob (তথ্য->blob, base_offset + table.offset, table.দৈর্ঘ্য);

  ফেরত blob;
}

স্থির unsigned
hb_face_for_তথ্য_get_table_tags (ধ্রুবক hb_face_t *face HB_UNUSED,
				  unsigned পূর্ণসংখ্যা start_offset,
				  unsigned পূর্ণসংখ্যা *table_গুনতি_2,
				  hb_tag_t *table_tags,
				  শূন্য *user_তথ্য)
{
  hb_face_for_তথ্য_closure_t *তথ্য = (hb_face_for_তথ্য_closure_t *) user_তথ্য;

  ধ্রুবক OT::OpenTypeFontFile &ot_file = *তথ্য->blob->as<OT::OpenTypeFontFile> ();
  ধ্রুবক OT::OpenTypeFontFace &ot_face = ot_file.get_face (তথ্য->সূচক_2);

  ফেরত ot_face.get_table_tags (start_offset, table_গুনতি_2, table_tags);
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
		unsigned পূর্ণসংখ্যা  সূচক_2)
{
  hb_face_t *face;

  যদি (unlikely (!blob))
    blob = hb_blob_get_empty ();

  blob = hb_sanitize_context_t ().sanitize_blob<OT::OpenTypeFontFile> (hb_blob_reference (blob));

  hb_face_for_তথ্য_closure_t *closure = hb_face_for_তথ্য_closure_create (blob, সূচক_2);

  যদি (unlikely (!closure))
  {
    hb_blob_destroy (blob);
    ফেরত hb_face_get_empty ();
  }

  face = hb_face_create_for_tables (hb_face_for_তথ্য_reference_table,
				    closure,
				    hb_face_for_তথ্য_closure_destroy);
  hb_face_set_get_table_tags_func (face,
				   hb_face_for_তথ্য_get_table_tags,
				   closure,
				   nullptr);

  face->সূচক_2 = সূচক_2;

  ফেরত face;
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
			unsigned পূর্ণসংখ্যা  সূচক_2)
{
  unsigned অঙ্ক_faces = hb_face_গুনতি (blob);
  যদি (সূচক_2 >= অঙ্ক_faces)
    ফেরত nullptr;

  hb_face_t *face = hb_face_create (blob, সূচক_2);
  যদি (hb_object_is_immutable (face))
    ফেরত nullptr;

  ফেরত face;
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
hb_face_create_from_file_or_fail (ধ্রুবক অক্ষর   *file_name,
				  unsigned পূর্ণসংখ্যা  সূচক_2)
{
  hb_blob_t *blob = hb_blob_create_from_file_or_fail (file_name);
  যদি (unlikely (!blob))
    ফেরত nullptr;

  hb_face_t *face = hb_face_create_or_fail (blob, সূচক_2);
  hb_blob_destroy (blob);

  ফেরত face;
}

স্থির ধ্রুবক কাঠামো supported_face_loaders_t {
	অক্ষর name[16];
	hb_face_t * (*from_file) (ধ্রুবক অক্ষর *font_file, unsigned face_সূচক);
	hb_face_t * (*from_blob) (hb_blob_t *blob, unsigned face_সূচক);
} supported_face_loaders[] =
{
  {"ot",
#ifndef HB_NO_OPEN
   hb_face_create_from_file_or_fail,
#অন্যথা
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

স্থির ধ্রুবক অক্ষর *get_default_loader_name ()
{
  স্থির hb_atomic_t<ধ্রুবক অক্ষর *> static_loader_name;
  ধ্রুবক অক্ষর *loader_name = static_loader_name.get_acquire ();
  যদি (!loader_name)
  {
    loader_name = getenv ("HB_FACE_LOADER");
    যদি (!loader_name)
      loader_name = "";
    যদি (!static_loader_name.cmpexch (nullptr, loader_name))
      loader_name = static_loader_name.get_acquire ();
  }
  ফেরত loader_name;
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
hb_face_create_from_file_or_fail_using (ধ্রুবক অক্ষর   *file_name,
					unsigned পূর্ণসংখ্যা  সূচক_2,
					ধ্রুবক অক্ষর   *loader_name)
{
  // Duplicated in hb_face_create_or_fail_using
  যৌক্তিক retry = মিথ্যা;
  যদি (!loader_name || !*loader_name)
  {
    loader_name = get_default_loader_name ();
    retry = সত্য;
  }
  যদি (loader_name && !*loader_name) loader_name = nullptr;

retry:
  জন্য (unsigned i = 0; i < ARRAY_দৈর্ঘ্য (supported_face_loaders); i++)
  {
    যদি (!loader_name || (supported_face_loaders[i].from_file && !strcmp (supported_face_loaders[i].name, loader_name)))
      ফেরত supported_face_loaders[i].from_file (file_name, সূচক_2);
  }

  যদি (retry)
  {
    retry = মিথ্যা;
    loader_name = nullptr;
    যাও retry;
  }

  ফেরত nullptr;
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
			      unsigned পূর্ণসংখ্যা  সূচক_2,
			      ধ্রুবক অক্ষর   *loader_name)
{
  // Duplicated in hb_face_create_from_file_or_fail_using
  যৌক্তিক retry = মিথ্যা;
  যদি (!loader_name || !*loader_name)
  {
    loader_name = get_default_loader_name ();
    retry = সত্য;
  }
  যদি (loader_name && !*loader_name) loader_name = nullptr;

retry:
  জন্য (unsigned i = 0; i < ARRAY_দৈর্ঘ্য (supported_face_loaders); i++)
  {
    যদি (!loader_name || (supported_face_loaders[i].from_blob && !strcmp (supported_face_loaders[i].name, loader_name)))
      ফেরত supported_face_loaders[i].from_blob (blob, সূচক_2);
  }

  যদি (retry)
  {
    retry = মিথ্যা;
    loader_name = nullptr;
    যাও retry;
  }

  ফেরত nullptr;
}

স্থির অন্তঃরেখ শূন্য free_static_face_loader_list ();

স্থির ধ্রুবক অক্ষর * ধ্রুবক nil_face_loader_list[] = {nullptr};

স্থির কাঠামো hb_face_loader_list_lazy_loader_t : hb_lazy_loader_t<ধ্রুবক অক্ষর *,
								  hb_face_loader_list_lazy_loader_t>
{
  স্থির ধ্রুবক অক্ষর ** create ()
  {
    ধ্রুবক অক্ষর **face_loader_list = (ধ্রুবক অক্ষর **) hb_calloc (1 + ARRAY_দৈর্ঘ্য (supported_face_loaders), আকার (ধ্রুবক অক্ষর *));
    যদি (unlikely (!face_loader_list))
      ফেরত nullptr;

    unsigned i;
    জন্য (i = 0; i < ARRAY_দৈর্ঘ্য (supported_face_loaders); i++)
      face_loader_list[i] = supported_face_loaders[i].name;
    face_loader_list[i] = nullptr;

    hb_atexit (free_static_face_loader_list);

    ফেরত face_loader_list;
  }
  স্থির শূন্য destroy (ধ্রুবক অক্ষর **l)
  { hb_free (l); }
  স্থির ধ্রুবক অক্ষর * ধ্রুবক * get_null ()
  { ফেরত nil_face_loader_list; }
} static_face_loader_list;

স্থির অন্তঃরেখ
শূন্য free_static_face_loader_list ()
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
ধ্রুবক অক্ষর **
hb_face_list_loaders ()
{
  ফেরত static_face_loader_list.get_unconst ();
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
  ফেরত const_cast<hb_face_t *> (&Null (hb_face_t));
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
  ফেরত hb_object_reference (face);
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
শূন্য
hb_face_destroy (hb_face_t *face)
{
  যদি (!hb_object_destroy (face)) ফেরত;

#ifndef HB_NO_SHAPER
  জন্য (hb_face_t::plan_গ্রন্থি_t *node = face->shape_plans; node; )
  {
    hb_face_t::plan_গ্রন্থি_t *next = node->next;
    hb_shape_plan_destroy (node->shape_plan);
    hb_free (node);
    node = next;
  }
#endif

  face->তথ্য.fini ();
  face->table.fini ();

  যদি (face->get_table_tags_destroy)
    face->get_table_tags_destroy (face->get_table_tags_user_তথ্য);

  যদি (face->destroy)
    face->destroy (face->user_তথ্য);

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
hb_face_set_user_তথ্য (hb_face_t          *face,
		       hb_user_তথ্য_চাবি_t *key,
		       শূন্য *              তথ্য,
		       hb_destroy_func_t   destroy,
		       hb_bool_t           replace)
{
  ফেরত hb_object_set_user_তথ্য (face, key, তথ্য, destroy, replace);
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
শূন্য *
hb_face_get_user_তথ্য (ধ্রুবক hb_face_t    *face,
		       hb_user_তথ্য_চাবি_t *key)
{
  ফেরত hb_object_get_user_তথ্য (face, key);
}

/**
 * hb_face_make_immutable:
 * @face: A face object
 *
 * Makes the given face object immutable.
 *
 * Since: 0.9.2
 **/
শূন্য
hb_face_make_immutable (hb_face_t *face)
{
  যদি (hb_object_is_immutable (face))
    ফেরত;

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
  ফেরত hb_object_is_immutable (face);
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
hb_face_reference_table (ধ্রুবক hb_face_t *face,
			 hb_tag_t tag)
{
  যদি (unlikely (tag == HB_TAG_NONE))
    ফেরত hb_blob_get_empty ();

  ফেরত face->reference_table (tag);
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

  যদি (blob == hb_blob_get_empty ())
  {
    // If referencing the face blob is not possible (e.g. not implemented by the
    // font functions), use face builder to create a blob out of individual
    // table blobs.
    unsigned যোগ_গুনতি = hb_face_get_table_tags (face, 0, nullptr, nullptr);
    যদি (যোগ_গুনতি)
    {
      hb_tag_t tags[64];
      unsigned গুনতি = ARRAY_দৈর্ঘ্য (tags);
      hb_face_t* builder = hb_face_builder_create ();

      জন্য (unsigned offset = 0; offset < যোগ_গুনতি; offset += গুনতি)
      {
        hb_face_get_table_tags (face, offset, &গুনতি, tags);
	যদি (unlikely (!গুনতি))
	  বিরতি; // Allocation error
        জন্য (unsigned i = 0; i < গুনতি; i++)
        {
	  যদি (unlikely (!tags[i]))
	    চালু;
	  hb_blob_t *table = hb_face_reference_table (face, tags[i]);
	  hb_face_builder_add_table (builder, tags[i], table);
	  hb_blob_destroy (table);
        }
      }

      blob = hb_face_reference_blob (builder);
      hb_face_destroy (builder);
    }
  }

  ফেরত blob;
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
শূন্য
hb_face_set_সূচক (hb_face_t    *face,
		   unsigned পূর্ণসংখ্যা  সূচক_2)
{
  যদি (hb_object_is_immutable (face))
    ফেরত;

  face->সূচক_2 = সূচক_2;
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
unsigned পূর্ণসংখ্যা
hb_face_get_সূচক (ধ্রুবক hb_face_t *face)
{
  ফেরত face->সূচক_2;
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
শূন্য
hb_face_set_upem (hb_face_t    *face,
		  unsigned পূর্ণসংখ্যা  upem)
{
  যদি (hb_object_is_immutable (face))
    ফেরত;

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
unsigned পূর্ণসংখ্যা
hb_face_get_upem (ধ্রুবক hb_face_t *face)
{
  ফেরত face->get_upem ();
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
শূন্য
hb_face_set_glyph_গুনতি (hb_face_t    *face,
			 unsigned পূর্ণসংখ্যা  glyph_গুনতি)
{
  যদি (hb_object_is_immutable (face))
    ফেরত;

  face->অঙ্ক_glyphs = glyph_গুনতি;
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
unsigned পূর্ণসংখ্যা
hb_face_get_glyph_গুনতি (ধ্রুবক hb_face_t *face)
{
  ফেরত face->get_অঙ্ক_glyphs ();
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
HB_EXTERN শূন্য
hb_face_set_get_table_tags_func (hb_face_t *face,
				 hb_get_table_tags_func_t func,
				 শূন্য                    *user_তথ্য,
				 hb_destroy_func_t        destroy)
{
  যদি (hb_object_is_immutable (face))
  {
    যদি (destroy)
      destroy (user_তথ্য);
    ফেরত;
  }

  যদি (face->get_table_tags_destroy)
    face->get_table_tags_destroy (face->get_table_tags_user_তথ্য);

  face->get_table_tags_func = func;
  face->get_table_tags_user_তথ্য = user_তথ্য;
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
unsigned পূর্ণসংখ্যা
hb_face_get_table_tags (ধ্রুবক hb_face_t *face,
			unsigned পূর্ণসংখ্যা  start_offset,
			unsigned পূর্ণসংখ্যা *table_গুনতি_2, /* IN/OUT */
			hb_tag_t     *table_tags /* OUT */)
{
  যদি (!face->get_table_tags_func)
  {
    যদি (table_গুনতি_2)
      *table_গুনতি_2 = 0;
    ফেরত 0;
  }

  ফেরত face->get_table_tags_func (face, start_offset, table_গুনতি_2, table_tags, face->get_table_tags_user_তথ্য);
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
শূন্য
hb_face_collect_unicodes (hb_face_t *face,
			  hb_set_t  *out)
{
  face->table.cmap->collect_unicodes (out, face->get_অঙ্ক_glyphs ());
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
শূন্য
hb_face_collect_nominal_glyph_mapping (hb_face_t *face,
				       hb_map_t  *mapping,
				       hb_set_t  *unicodes)
{
  hb_set_t স্তূপ_unicodes;
  যদি (!unicodes)
    unicodes = &স্তূপ_unicodes;
  face->table.cmap->collect_mapping (unicodes, mapping, face->get_অঙ্ক_glyphs ());
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
শূন্য
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
শূন্য
hb_face_collect_variation_unicodes (hb_face_t *face,
				    hb_codepoint_t variation_selector,
				    hb_set_t  *out)
{
  face->table.cmap->collect_variation_unicodes (variation_selector, out);
}
#endif
