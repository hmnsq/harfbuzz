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

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-face.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-blob.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-open-file.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-face.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-cmap-table.hh"

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_FREETYPE
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ft.h"
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CORETEXT
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-coretext.h"
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_DIRECTWRITE
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-directwrite.h"
#ਜੇ_ਅੰਤ


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
ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_face_ਗਿਣਤੀ (hb_blob_t *blob)
{
  ਜੇ (unlikely (!blob))
    ਵਾਪਸ 0;

  hb_sanitize_context_t c (blob);

  ਸਵੈਚਾਲਿਤ *ot = blob->as<OT::OpenTypeFontFile> ();
  ਜੇ (unlikely (!ot->sanitize (&c)))
    ਵਾਪਸ 0;

  ਵਾਪਸ ot->get_face_ਗਿਣਤੀ ();
}

/*
 * hb_face_t
 */

DEFINE_NULL_INSTANCE (hb_face_t) =
{
  HB_OBJECT_ਸਿਰਲੇਖ_STATIC,

  0,    /*ਸੂਚਕਾਂਕ*/
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
hb_face_create_for_tables (hb_reference_ਸਾਰਣੀ_func_t  reference_ਸਾਰਣੀ_func,
			   ਖਾਲੀ                      *user_ਅੰਕੜੇ,
			   hb_destroy_func_t          destroy)
{
  hb_face_t *face;

  ਜੇ (!reference_ਸਾਰਣੀ_func || !(face = hb_object_create<hb_face_t> ())) {
    ਜੇ (destroy)
      destroy (user_ਅੰਕੜੇ);
    ਵਾਪਸ hb_face_get_ਖਾਲੀ_ਥਾਵਾਂ ();
  }

  face->reference_ਸਾਰਣੀ_func = reference_ਸਾਰਣੀ_func;
  face->user_ਅੰਕੜੇ = user_ਅੰਕੜੇ;
  face->destroy = destroy;

  face->ਅੰਕ_glyphs = -1;

  face->ਅੰਕੜੇ.init0 (face);
  face->ਸਾਰਣੀ.init0 (face);

  ਵਾਪਸ face;
}


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ hb_face_for_ਅੰਕੜੇ_ਘੇਰਾ_t {
  hb_blob_t *blob;
  uint16_t  ਸੂਚਕਾਂਕ_2;
} hb_face_for_ਅੰਕੜੇ_ਘੇਰਾ_t;

ਸਥਿਰ hb_face_for_ਅੰਕੜੇ_ਘੇਰਾ_t *
hb_face_for_ਅੰਕੜੇ_ਘੇਰਾ_create (hb_blob_t *blob, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੂਚਕਾਂਕ_2)
{
  hb_face_for_ਅੰਕੜੇ_ਘੇਰਾ_t *ਘੇਰਾ;

  ਘੇਰਾ = (hb_face_for_ਅੰਕੜੇ_ਘੇਰਾ_t *) hb_calloc (1, ਆਕਾਰ (hb_face_for_ਅੰਕੜੇ_ਘੇਰਾ_t));
  ਜੇ (unlikely (!ਘੇਰਾ))
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  ਘੇਰਾ->blob = blob;
  ਘੇਰਾ->ਸੂਚਕਾਂਕ_2 = (uint16_t) (ਸੂਚਕਾਂਕ_2 & 0xFFFFu);

  ਵਾਪਸ ਘੇਰਾ;
}

ਸਥਿਰ ਖਾਲੀ
hb_face_for_ਅੰਕੜੇ_ਘੇਰਾ_destroy (ਖਾਲੀ *ਅੰਕੜੇ)
{
  hb_face_for_ਅੰਕੜੇ_ਘੇਰਾ_t *ਘੇਰਾ = (hb_face_for_ਅੰਕੜੇ_ਘੇਰਾ_t *) ਅੰਕੜੇ;

  hb_blob_destroy (ਘੇਰਾ->blob);
  hb_free (ਘੇਰਾ);
}

ਸਥਿਰ hb_blob_t *
hb_face_for_ਅੰਕੜੇ_reference_ਸਾਰਣੀ (hb_face_t *face HB_ਅਣਵਰਤਿਆ, hb_ਟੈਗ_t ਟੈਗ_2, ਖਾਲੀ *user_ਅੰਕੜੇ)
{
  hb_face_for_ਅੰਕੜੇ_ਘੇਰਾ_t *ਅੰਕੜੇ = (hb_face_for_ਅੰਕੜੇ_ਘੇਰਾ_t *) user_ਅੰਕੜੇ;

  ਜੇ (ਟੈਗ_2 == HB_ਟੈਗ_NONE)
    ਵਾਪਸ hb_blob_reference (ਅੰਕੜੇ->blob);

  ਅਚਲ OT::OpenTypeFontFile &ot_file = *ਅੰਕੜੇ->blob->as<OT::OpenTypeFontFile> ();
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅਧਾਰ_ਵਿੱਥ;
  ਅਚਲ OT::OpenTypeFontFace &ot_face = ot_file.get_face (ਅੰਕੜੇ->ਸੂਚਕਾਂਕ_2, &ਅਧਾਰ_ਵਿੱਥ);

  ਅਚਲ OT::Open_Type_ਸਾਰਣੀ &ਸਾਰਣੀ = ot_face.get_ਸਾਰਣੀ_by_ਟੈਗ (ਟੈਗ_2);

  hb_blob_t *blob = hb_blob_create_sub_blob (ਅੰਕੜੇ->blob, ਅਧਾਰ_ਵਿੱਥ + ਸਾਰਣੀ.ਵਿੱਥ, ਸਾਰਣੀ.ਲੰਮਾਈ);

  ਵਾਪਸ blob;
}

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ
hb_face_for_ਅੰਕੜੇ_get_ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ (ਅਚਲ hb_face_t *face HB_ਅਣਵਰਤਿਆ,
				  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ_ਵਿੱਥ,
				  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *ਸਾਰਣੀ_ਗਿਣਤੀ_2,
				  hb_ਟੈਗ_t *ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ,
				  ਖਾਲੀ *user_ਅੰਕੜੇ)
{
  hb_face_for_ਅੰਕੜੇ_ਘੇਰਾ_t *ਅੰਕੜੇ = (hb_face_for_ਅੰਕੜੇ_ਘੇਰਾ_t *) user_ਅੰਕੜੇ;

  ਅਚਲ OT::OpenTypeFontFile &ot_file = *ਅੰਕੜੇ->blob->as<OT::OpenTypeFontFile> ();
  ਅਚਲ OT::OpenTypeFontFace &ot_face = ot_file.get_face (ਅੰਕੜੇ->ਸੂਚਕਾਂਕ_2);

  ਵਾਪਸ ot_face.get_ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ (ਸ਼ੁਰੂ_ਵਿੱਥ, ਸਾਰਣੀ_ਗਿਣਤੀ_2, ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ);
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
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  ਸੂਚਕਾਂਕ_2)
{
  hb_face_t *face;

  ਜੇ (unlikely (!blob))
    blob = hb_blob_get_ਖਾਲੀ_ਥਾਵਾਂ ();

  blob = hb_sanitize_context_t ().sanitize_blob<OT::OpenTypeFontFile> (hb_blob_reference (blob));

  hb_face_for_ਅੰਕੜੇ_ਘੇਰਾ_t *ਘੇਰਾ = hb_face_for_ਅੰਕੜੇ_ਘੇਰਾ_create (blob, ਸੂਚਕਾਂਕ_2);

  ਜੇ (unlikely (!ਘੇਰਾ))
  {
    hb_blob_destroy (blob);
    ਵਾਪਸ hb_face_get_ਖਾਲੀ_ਥਾਵਾਂ ();
  }

  face = hb_face_create_for_tables (hb_face_for_ਅੰਕੜੇ_reference_ਸਾਰਣੀ,
				    ਘੇਰਾ,
				    hb_face_for_ਅੰਕੜੇ_ਘੇਰਾ_destroy);
  hb_face_ਸੈੱਟ_get_ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ_func (face,
				   hb_face_for_ਅੰਕੜੇ_get_ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ,
				   ਘੇਰਾ,
				   ਖਾਲੀ_ਸੂਚਕ);

  face->ਸੂਚਕਾਂਕ_2 = ਸੂਚਕਾਂਕ_2;

  ਵਾਪਸ face;
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
			ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  ਸੂਚਕਾਂਕ_2)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_faces = hb_face_ਗਿਣਤੀ (blob);
  ਜੇ (ਸੂਚਕਾਂਕ_2 >= ਅੰਕ_faces)
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  hb_face_t *face = hb_face_create (blob, ਸੂਚਕਾਂਕ_2);
  ਜੇ (hb_object_is_immutable (face))
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  ਵਾਪਸ face;
}

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OPEN
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
hb_face_create_from_file_or_fail (ਅਚਲ ਅੱਖਰ   *file_name,
				  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  ਸੂਚਕਾਂਕ_2)
{
  hb_blob_t *blob = hb_blob_create_from_file_or_fail (file_name);
  ਜੇ (unlikely (!blob))
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  hb_face_t *face = hb_face_create_or_fail (blob, ਸੂਚਕਾਂਕ_2);
  hb_blob_destroy (blob);

  ਵਾਪਸ face;
}

ਸਥਿਰ ਅਚਲ ਬਣਤਰ supported_face_loaders_t {
	ਅੱਖਰ name[16];
	hb_face_t * (*from_file) (ਅਚਲ ਅੱਖਰ *font_file, ਬਿਨਾਨਿਸ਼ਾਨੀ face_ਸੂਚਕਾਂਕ);
	hb_face_t * (*from_blob) (hb_blob_t *blob, ਬਿਨਾਨਿਸ਼ਾਨੀ face_ਸੂਚਕਾਂਕ);
} supported_face_loaders[] =
{
  {"ot",
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OPEN
   hb_face_create_from_file_or_fail,
#ਨਹੀਂਤਾਂ
   ਖਾਲੀ_ਸੂਚਕ,
#ਜੇ_ਅੰਤ
   hb_face_create_or_fail
  },
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_FREETYPE
  {"ft",
   hb_ft_face_create_from_file_or_fail,
   hb_ft_face_create_from_blob_or_fail
  },
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CORETEXT
  {"coretext",
   hb_coretext_face_create_from_file_or_fail,
   hb_coretext_face_create_from_blob_or_fail
  },
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_DIRECTWRITE
  {"directwrite",
   hb_directwrite_face_create_from_file_or_fail,
   hb_directwrite_face_create_from_blob_or_fail
  },
#ਜੇ_ਅੰਤ
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *get_default_loader_name ()
{
  ਸਥਿਰ hb_atomic_t<ਅਚਲ ਅੱਖਰ *> static_loader_name;
  ਅਚਲ ਅੱਖਰ *loader_name = static_loader_name.get_acquire ();
  ਜੇ (!loader_name)
  {
    loader_name = getenv ("HB_FACE_LOADER");
    ਜੇ (!loader_name)
      loader_name = "";
    ਜੇ (!static_loader_name.cmpexch (ਖਾਲੀ_ਸੂਚਕ, loader_name))
      loader_name = static_loader_name.get_acquire ();
  }
  ਵਾਪਸ loader_name;
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
hb_face_create_from_file_or_fail_using (ਅਚਲ ਅੱਖਰ   *file_name,
					ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  ਸੂਚਕਾਂਕ_2,
					ਅਚਲ ਅੱਖਰ   *loader_name)
{
  // Duplicated in hb_face_create_or_fail_using
  ਤਾਰਕਿਕ retry = ਝੂਠ;
  ਜੇ (!loader_name || !*loader_name)
  {
    loader_name = get_default_loader_name ();
    retry = ਸੱਚ;
  }
  ਜੇ (loader_name && !*loader_name) loader_name = ਖਾਲੀ_ਸੂਚਕ;

retry:
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ARRAY_ਲੰਮਾਈ (supported_face_loaders); i++)
  {
    ਜੇ (!loader_name || (supported_face_loaders[i].from_file && !strcmp (supported_face_loaders[i].name, loader_name)))
      ਵਾਪਸ supported_face_loaders[i].from_file (file_name, ਸੂਚਕਾਂਕ_2);
  }

  ਜੇ (retry)
  {
    retry = ਝੂਠ;
    loader_name = ਖਾਲੀ_ਸੂਚਕ;
    ਜਾਓ retry;
  }

  ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
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
			      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  ਸੂਚਕਾਂਕ_2,
			      ਅਚਲ ਅੱਖਰ   *loader_name)
{
  // Duplicated in hb_face_create_from_file_or_fail_using
  ਤਾਰਕਿਕ retry = ਝੂਠ;
  ਜੇ (!loader_name || !*loader_name)
  {
    loader_name = get_default_loader_name ();
    retry = ਸੱਚ;
  }
  ਜੇ (loader_name && !*loader_name) loader_name = ਖਾਲੀ_ਸੂਚਕ;

retry:
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ARRAY_ਲੰਮਾਈ (supported_face_loaders); i++)
  {
    ਜੇ (!loader_name || (supported_face_loaders[i].from_blob && !strcmp (supported_face_loaders[i].name, loader_name)))
      ਵਾਪਸ supported_face_loaders[i].from_blob (blob, ਸੂਚਕਾਂਕ_2);
  }

  ਜੇ (retry)
  {
    retry = ਝੂਠ;
    loader_name = ਖਾਲੀ_ਸੂਚਕ;
    ਜਾਓ retry;
  }

  ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ free_static_face_loader_list ();

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ nil_face_loader_list[] = {ਖਾਲੀ_ਸੂਚਕ};

ਸਥਿਰ ਬਣਤਰ hb_face_loader_list_lazy_loader_t : hb_lazy_loader_t<ਅਚਲ ਅੱਖਰ *,
								  hb_face_loader_list_lazy_loader_t>
{
  ਸਥਿਰ ਅਚਲ ਅੱਖਰ ** create ()
  {
    ਅਚਲ ਅੱਖਰ **face_loader_list = (ਅਚਲ ਅੱਖਰ **) hb_calloc (1 + ARRAY_ਲੰਮਾਈ (supported_face_loaders), ਆਕਾਰ (ਅਚਲ ਅੱਖਰ *));
    ਜੇ (unlikely (!face_loader_list))
      ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

    ਬਿਨਾਨਿਸ਼ਾਨੀ i;
    ਲਈ (i = 0; i < ARRAY_ਲੰਮਾਈ (supported_face_loaders); i++)
      face_loader_list[i] = supported_face_loaders[i].name;
    face_loader_list[i] = ਖਾਲੀ_ਸੂਚਕ;

    hb_atexit (free_static_face_loader_list);

    ਵਾਪਸ face_loader_list;
  }
  ਸਥਿਰ ਖਾਲੀ destroy (ਅਚਲ ਅੱਖਰ **l)
  { hb_free (l); }
  ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ * get_null ()
  { ਵਾਪਸ nil_face_loader_list; }
} static_face_loader_list;

ਸਥਿਰ ਸਤਰਅੰਦਰ
ਖਾਲੀ free_static_face_loader_list ()
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
ਅਚਲ ਅੱਖਰ **
hb_face_list_loaders ()
{
  ਵਾਪਸ static_face_loader_list.get_unconst ();
}
#ਜੇ_ਅੰਤ


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
hb_face_get_ਖਾਲੀ_ਥਾਵਾਂ ()
{
  ਵਾਪਸ const_cast<hb_face_t *> (&Null (hb_face_t));
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
  ਵਾਪਸ hb_object_reference (face);
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
ਖਾਲੀ
hb_face_destroy (hb_face_t *face)
{
  ਜੇ (!hb_object_destroy (face)) ਵਾਪਸ;

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_SHAPER
  ਲਈ (hb_face_t::plan_ਗੰਢ_t *node = face->shape_plans; node; )
  {
    hb_face_t::plan_ਗੰਢ_t *next = node->next;
    hb_shape_plan_destroy (node->shape_plan);
    hb_free (node);
    node = next;
  }
#ਜੇ_ਅੰਤ

  face->ਅੰਕੜੇ.fini ();
  face->ਸਾਰਣੀ.fini ();

  ਜੇ (face->get_ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ_destroy)
    face->get_ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ_destroy (face->get_ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ_user_ਅੰਕੜੇ);

  ਜੇ (face->destroy)
    face->destroy (face->user_ਅੰਕੜੇ);

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
hb_face_ਸੈੱਟ_user_ਅੰਕੜੇ (hb_face_t          *face,
		       hb_user_ਅੰਕੜੇ_ਕੁੰਜੀ_t *key,
		       ਖਾਲੀ *              ਅੰਕੜੇ,
		       hb_destroy_func_t   destroy,
		       hb_bool_t           replace)
{
  ਵਾਪਸ hb_object_ਸੈੱਟ_user_ਅੰਕੜੇ (face, key, ਅੰਕੜੇ, destroy, replace);
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
ਖਾਲੀ *
hb_face_get_user_ਅੰਕੜੇ (ਅਚਲ hb_face_t    *face,
		       hb_user_ਅੰਕੜੇ_ਕੁੰਜੀ_t *key)
{
  ਵਾਪਸ hb_object_get_user_ਅੰਕੜੇ (face, key);
}

/**
 * hb_face_make_immutable:
 * @face: A face object
 *
 * Makes the given face object immutable.
 *
 * Since: 0.9.2
 **/
ਖਾਲੀ
hb_face_make_immutable (hb_face_t *face)
{
  ਜੇ (hb_object_is_immutable (face))
    ਵਾਪਸ;

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
  ਵਾਪਸ hb_object_is_immutable (face);
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
hb_face_reference_ਸਾਰਣੀ (ਅਚਲ hb_face_t *face,
			 hb_ਟੈਗ_t ਟੈਗ_2)
{
  ਜੇ (unlikely (ਟੈਗ_2 == HB_ਟੈਗ_NONE))
    ਵਾਪਸ hb_blob_get_ਖਾਲੀ_ਥਾਵਾਂ ();

  ਵਾਪਸ face->reference_ਸਾਰਣੀ (ਟੈਗ_2);
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
  hb_blob_t *blob = face->reference_ਸਾਰਣੀ (HB_ਟੈਗ_NONE);

  ਜੇ (blob == hb_blob_get_ਖਾਲੀ_ਥਾਵਾਂ ())
  {
    // If referencing the face blob is not possible (e.g. not implemented by the
    // font functions), use face builder to create a blob out of individual
    // table blobs.
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਕੁੱਲ_ਗਿਣਤੀ = hb_face_get_ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ (face, 0, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    ਜੇ (ਕੁੱਲ_ਗਿਣਤੀ)
    {
      hb_ਟੈਗ_t ਟੈਗ_ਸੂਚੀ[64];
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = ARRAY_ਲੰਮਾਈ (ਟੈਗ_ਸੂਚੀ);
      hb_face_t* builder = hb_face_builder_create ();

      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਵਿੱਥ = 0; ਵਿੱਥ < ਕੁੱਲ_ਗਿਣਤੀ; ਵਿੱਥ += ਗਿਣਤੀ_2)
      {
        hb_face_get_ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ (face, ਵਿੱਥ, &ਗਿਣਤੀ_2, ਟੈਗ_ਸੂਚੀ);
	ਜੇ (unlikely (!ਗਿਣਤੀ_2))
	  ਰੋਕੋ; // Allocation error
        ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2; i++)
        {
	  ਜੇ (unlikely (!ਟੈਗ_ਸੂਚੀ[i]))
	    ਜਾਰੀ;
	  hb_blob_t *ਸਾਰਣੀ = hb_face_reference_ਸਾਰਣੀ (face, ਟੈਗ_ਸੂਚੀ[i]);
	  hb_face_builder_ਜੋੜੋ_ਸਾਰਣੀ (builder, ਟੈਗ_ਸੂਚੀ[i], ਸਾਰਣੀ);
	  hb_blob_destroy (ਸਾਰਣੀ);
        }
      }

      blob = hb_face_reference_blob (builder);
      hb_face_destroy (builder);
    }
  }

  ਵਾਪਸ blob;
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
ਖਾਲੀ
hb_face_ਸੈੱਟ_ਸੂਚਕਾਂਕ (hb_face_t    *face,
		   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  ਸੂਚਕਾਂਕ_2)
{
  ਜੇ (hb_object_is_immutable (face))
    ਵਾਪਸ;

  face->ਸੂਚਕਾਂਕ_2 = ਸੂਚਕਾਂਕ_2;
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
ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_face_get_ਸੂਚਕਾਂਕ (ਅਚਲ hb_face_t *face)
{
  ਵਾਪਸ face->ਸੂਚਕਾਂਕ_2;
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
ਖਾਲੀ
hb_face_ਸੈੱਟ_upem (hb_face_t    *face,
		  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  upem)
{
  ਜੇ (hb_object_is_immutable (face))
    ਵਾਪਸ;

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
ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_face_get_upem (ਅਚਲ hb_face_t *face)
{
  ਵਾਪਸ face->get_upem ();
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
ਖਾਲੀ
hb_face_ਸੈੱਟ_glyph_ਗਿਣਤੀ (hb_face_t    *face,
			 ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  glyph_ਗਿਣਤੀ)
{
  ਜੇ (hb_object_is_immutable (face))
    ਵਾਪਸ;

  face->ਅੰਕ_glyphs = glyph_ਗਿਣਤੀ;
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
ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_face_get_glyph_ਗਿਣਤੀ (ਅਚਲ hb_face_t *face)
{
  ਵਾਪਸ face->get_ਅੰਕ_glyphs ();
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
HB_EXTERN ਖਾਲੀ
hb_face_ਸੈੱਟ_get_ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ_func (hb_face_t *face,
				 hb_get_ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ_func_t func,
				 ਖਾਲੀ                    *user_ਅੰਕੜੇ,
				 hb_destroy_func_t        destroy)
{
  ਜੇ (hb_object_is_immutable (face))
  {
    ਜੇ (destroy)
      destroy (user_ਅੰਕੜੇ);
    ਵਾਪਸ;
  }

  ਜੇ (face->get_ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ_destroy)
    face->get_ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ_destroy (face->get_ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ_user_ਅੰਕੜੇ);

  face->get_ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ_func = func;
  face->get_ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ_user_ਅੰਕੜੇ = user_ਅੰਕੜੇ;
  face->get_ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ_destroy = destroy;
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
ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_face_get_ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ (ਅਚਲ hb_face_t *face,
			ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  ਸ਼ੁਰੂ_ਵਿੱਥ,
			ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *ਸਾਰਣੀ_ਗਿਣਤੀ_2, /* IN/OUT */
			hb_ਟੈਗ_t     *ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ /*ਬਾਹਰਲਾ*/)
{
  ਜੇ (!face->get_ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ_func)
  {
    ਜੇ (ਸਾਰਣੀ_ਗਿਣਤੀ_2)
      *ਸਾਰਣੀ_ਗਿਣਤੀ_2 = 0;
    ਵਾਪਸ 0;
  }

  ਵਾਪਸ face->get_ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ_func (face, ਸ਼ੁਰੂ_ਵਿੱਥ, ਸਾਰਣੀ_ਗਿਣਤੀ_2, ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ, face->get_ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ_user_ਅੰਕੜੇ);
}


/*
 * Character set.
 */


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_FACE_COLLECT_UNICODES
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
ਖਾਲੀ
hb_face_collect_unicodes (hb_face_t *face,
			  hb_ਸੈੱਟ_t  *ਬਾਹਰਲਾ)
{
  face->ਸਾਰਣੀ.cmap->collect_unicodes (ਬਾਹਰਲਾ, face->get_ਅੰਕ_glyphs ());
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
ਖਾਲੀ
hb_face_collect_nominal_glyph_mapping (hb_face_t *face,
				       hb_map_t  *mapping,
				       hb_ਸੈੱਟ_t  *unicodes)
{
  hb_ਸੈੱਟ_t ਢੇਰ_unicodes;
  ਜੇ (!unicodes)
    unicodes = &ਢੇਰ_unicodes;
  face->ਸਾਰਣੀ.cmap->collect_mapping (unicodes, mapping, face->get_ਅੰਕ_glyphs ());
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
ਖਾਲੀ
hb_face_collect_variation_selectors (hb_face_t *face,
				     hb_ਸੈੱਟ_t  *ਬਾਹਰਲਾ)
{
  face->ਸਾਰਣੀ.cmap->collect_variation_selectors (ਬਾਹਰਲਾ);
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
ਖਾਲੀ
hb_face_collect_variation_unicodes (hb_face_t *face,
				    hb_codepoint_t variation_selector,
				    hb_ਸੈੱਟ_t  *ਬਾਹਰਲਾ)
{
  face->ਸਾਰਣੀ.cmap->collect_variation_unicodes (variation_selector, ਬਾਹਰਲਾ);
}
#ਜੇ_ਅੰਤ
