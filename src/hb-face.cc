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

#ضمن "hb.hh"

#ضمن "hb-face.hh"
#ضمن "hb-blob.hh"
#ضمن "hb-open-file.hh"
#ضمن "hb-ot-face.hh"
#ضمن "hb-ot-cmap-table.hh"

#اگر_تعریف_شده HAVE_FREETYPE
#ضمن "hb-ft.h"
#پایان_اگر
#اگر_تعریف_شده HAVE_CORETEXT
#ضمن "hb-coretext.h"
#پایان_اگر
#اگر_تعریف_شده HAVE_DIRECTWRITE
#ضمن "hb-directwrite.h"
#پایان_اگر


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
بیعلامت صحيح
hb_face_شمار (hb_blob_t *blob)
{
  إذا (unlikely (!blob))
    أرجع 0;

  hb_sanitize_context_t c (blob);

  خودکار *ot = blob->as<OT::OpenTypeFontFile> ();
  إذا (unlikely (!ot->sanitize (&c)))
    أرجع 0;

  أرجع ot->get_face_شمار ();
}

/*
 * hb_face_t
 */

DEFINE_NULL_INSTANCE (hb_face_t) =
{
  HB_OBJECT_سرآیند_STATIC,

  0,    /*شاخص*/
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
hb_face_create_for_tables (hb_reference_جدول_func_t  reference_جدول_func,
			   فراغ                      *user_داده,
			   hb_destroy_func_t          destroy)
{
  hb_face_t *face;

  إذا (!reference_جدول_func || !(face = hb_object_create<hb_face_t> ())) {
    إذا (destroy)
      destroy (user_داده);
    أرجع hb_face_get_خالی ();
  }

  face->reference_جدول_func = reference_جدول_func;
  face->user_داده = user_داده;
  face->destroy = destroy;

  face->رقم_glyphs = -1;

  face->داده.init0 (face);
  face->جدول.init0 (face);

  أرجع face;
}


تعريفنوع بنية hb_face_for_داده_بستگی_t {
  hb_blob_t *blob;
  uint16_t  شاخص_2;
} hb_face_for_داده_بستگی_t;

ساكن hb_face_for_داده_بستگی_t *
hb_face_for_داده_بستگی_create (hb_blob_t *blob, بیعلامت صحيح شاخص_2)
{
  hb_face_for_داده_بستگی_t *بستگی;

  بستگی = (hb_face_for_داده_بستگی_t *) hb_calloc (1, حجم (hb_face_for_داده_بستگی_t));
  إذا (unlikely (!بستگی))
    أرجع اشاره_تهی;

  بستگی->blob = blob;
  بستگی->شاخص_2 = (uint16_t) (شاخص_2 & 0xFFFFu);

  أرجع بستگی;
}

ساكن فراغ
hb_face_for_داده_بستگی_destroy (فراغ *داده)
{
  hb_face_for_داده_بستگی_t *بستگی = (hb_face_for_داده_بستگی_t *) داده;

  hb_blob_destroy (بستگی->blob);
  hb_free (بستگی);
}

ساكن hb_blob_t *
hb_face_for_داده_reference_جدول (hb_face_t *face HB_استفادهنشده, hb_برچسب_t برچسب_2, فراغ *user_داده)
{
  hb_face_for_داده_بستگی_t *داده = (hb_face_for_داده_بستگی_t *) user_داده;

  إذا (برچسب_2 == HB_برچسب_NONE)
    أرجع hb_blob_reference (داده->blob);

  ثابت OT::OpenTypeFontFile &ot_file = *داده->blob->as<OT::OpenTypeFontFile> ();
  بیعلامت صحيح پایه_جابه_جایی;
  ثابت OT::OpenTypeFontFace &ot_face = ot_file.get_face (داده->شاخص_2, &پایه_جابه_جایی);

  ثابت OT::Open_Type_جدول &جدول = ot_face.get_جدول_by_برچسب (برچسب_2);

  hb_blob_t *blob = hb_blob_create_sub_blob (داده->blob, پایه_جابه_جایی + جدول.جابه_جایی, جدول.درازی);

  أرجع blob;
}

ساكن بیعلامت
hb_face_for_داده_get_جدول_برچسبها (ثابت hb_face_t *face HB_استفادهنشده,
				  بیعلامت صحيح شروع_جابه_جایی,
				  بیعلامت صحيح *جدول_شمار_2,
				  hb_برچسب_t *جدول_برچسبها,
				  فراغ *user_داده)
{
  hb_face_for_داده_بستگی_t *داده = (hb_face_for_داده_بستگی_t *) user_داده;

  ثابت OT::OpenTypeFontFile &ot_file = *داده->blob->as<OT::OpenTypeFontFile> ();
  ثابت OT::OpenTypeFontFace &ot_face = ot_file.get_face (داده->شاخص_2);

  أرجع ot_face.get_جدول_برچسبها (شروع_جابه_جایی, جدول_شمار_2, جدول_برچسبها);
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
		بیعلامت صحيح  شاخص_2)
{
  hb_face_t *face;

  إذا (unlikely (!blob))
    blob = hb_blob_get_خالی ();

  blob = hb_sanitize_context_t ().sanitize_blob<OT::OpenTypeFontFile> (hb_blob_reference (blob));

  hb_face_for_داده_بستگی_t *بستگی = hb_face_for_داده_بستگی_create (blob, شاخص_2);

  إذا (unlikely (!بستگی))
  {
    hb_blob_destroy (blob);
    أرجع hb_face_get_خالی ();
  }

  face = hb_face_create_for_tables (hb_face_for_داده_reference_جدول,
				    بستگی,
				    hb_face_for_داده_بستگی_destroy);
  hb_face_مجموعه_get_جدول_برچسبها_func (face,
				   hb_face_for_داده_get_جدول_برچسبها,
				   بستگی,
				   اشاره_تهی);

  face->شاخص_2 = شاخص_2;

  أرجع face;
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
			بیعلامت صحيح  شاخص_2)
{
  بیعلامت رقم_faces = hb_face_شمار (blob);
  إذا (شاخص_2 >= رقم_faces)
    أرجع اشاره_تهی;

  hb_face_t *face = hb_face_create (blob, شاخص_2);
  إذا (hb_object_is_immutable (face))
    أرجع اشاره_تهی;

  أرجع face;
}

#اگر_تعریف_نشده HB_NO_OPEN
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
hb_face_create_from_file_or_fail (ثابت حرف   *file_name,
				  بیعلامت صحيح  شاخص_2)
{
  hb_blob_t *blob = hb_blob_create_from_file_or_fail (file_name);
  إذا (unlikely (!blob))
    أرجع اشاره_تهی;

  hb_face_t *face = hb_face_create_or_fail (blob, شاخص_2);
  hb_blob_destroy (blob);

  أرجع face;
}

ساكن ثابت بنية supported_face_loaders_t {
	حرف name[16];
	hb_face_t * (*from_file) (ثابت حرف *font_file, بیعلامت face_شاخص);
	hb_face_t * (*from_blob) (hb_blob_t *blob, بیعلامت face_شاخص);
} supported_face_loaders[] =
{
  {"ot",
#اگر_تعریف_نشده HB_NO_OPEN
   hb_face_create_from_file_or_fail,
#وإلا
   اشاره_تهی,
#پایان_اگر
   hb_face_create_or_fail
  },
#اگر_تعریف_شده HAVE_FREETYPE
  {"ft",
   hb_ft_face_create_from_file_or_fail,
   hb_ft_face_create_from_blob_or_fail
  },
#پایان_اگر
#اگر_تعریف_شده HAVE_CORETEXT
  {"coretext",
   hb_coretext_face_create_from_file_or_fail,
   hb_coretext_face_create_from_blob_or_fail
  },
#پایان_اگر
#اگر_تعریف_شده HAVE_DIRECTWRITE
  {"directwrite",
   hb_directwrite_face_create_from_file_or_fail,
   hb_directwrite_face_create_from_blob_or_fail
  },
#پایان_اگر
};

ساكن ثابت حرف *get_default_loader_name ()
{
  ساكن hb_atomic_t<ثابت حرف *> static_loader_name;
  ثابت حرف *loader_name = static_loader_name.get_acquire ();
  إذا (!loader_name)
  {
    loader_name = getenv ("HB_FACE_LOADER");
    إذا (!loader_name)
      loader_name = "";
    إذا (!static_loader_name.cmpexch (اشاره_تهی, loader_name))
      loader_name = static_loader_name.get_acquire ();
  }
  أرجع loader_name;
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
hb_face_create_from_file_or_fail_using (ثابت حرف   *file_name,
					بیعلامت صحيح  شاخص_2,
					ثابت حرف   *loader_name)
{
  // Duplicated in hb_face_create_or_fail_using
  منطقي retry = خطأ;
  إذا (!loader_name || !*loader_name)
  {
    loader_name = get_default_loader_name ();
    retry = درست;
  }
  إذا (loader_name && !*loader_name) loader_name = اشاره_تهی;

retry:
  لكل (بیعلامت i = 0; i < ARRAY_درازی (supported_face_loaders); i++)
  {
    إذا (!loader_name || (supported_face_loaders[i].from_file && !strcmp (supported_face_loaders[i].name, loader_name)))
      أرجع supported_face_loaders[i].from_file (file_name, شاخص_2);
  }

  إذا (retry)
  {
    retry = خطأ;
    loader_name = اشاره_تهی;
    اذهب retry;
  }

  أرجع اشاره_تهی;
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
			      بیعلامت صحيح  شاخص_2,
			      ثابت حرف   *loader_name)
{
  // Duplicated in hb_face_create_from_file_or_fail_using
  منطقي retry = خطأ;
  إذا (!loader_name || !*loader_name)
  {
    loader_name = get_default_loader_name ();
    retry = درست;
  }
  إذا (loader_name && !*loader_name) loader_name = اشاره_تهی;

retry:
  لكل (بیعلامت i = 0; i < ARRAY_درازی (supported_face_loaders); i++)
  {
    إذا (!loader_name || (supported_face_loaders[i].from_blob && !strcmp (supported_face_loaders[i].name, loader_name)))
      أرجع supported_face_loaders[i].from_blob (blob, شاخص_2);
  }

  إذا (retry)
  {
    retry = خطأ;
    loader_name = اشاره_تهی;
    اذهب retry;
  }

  أرجع اشاره_تهی;
}

ساكن مضمن فراغ free_static_face_loader_list ();

ساكن ثابت حرف * ثابت nil_face_loader_list[] = {اشاره_تهی};

ساكن بنية hb_face_loader_list_lazy_loader_t : hb_lazy_loader_t<ثابت حرف *,
								  hb_face_loader_list_lazy_loader_t>
{
  ساكن ثابت حرف ** create ()
  {
    ثابت حرف **face_loader_list = (ثابت حرف **) hb_calloc (1 + ARRAY_درازی (supported_face_loaders), حجم (ثابت حرف *));
    إذا (unlikely (!face_loader_list))
      أرجع اشاره_تهی;

    بیعلامت i;
    لكل (i = 0; i < ARRAY_درازی (supported_face_loaders); i++)
      face_loader_list[i] = supported_face_loaders[i].name;
    face_loader_list[i] = اشاره_تهی;

    hb_atexit (free_static_face_loader_list);

    أرجع face_loader_list;
  }
  ساكن فراغ destroy (ثابت حرف **l)
  { hb_free (l); }
  ساكن ثابت حرف * ثابت * get_null ()
  { أرجع nil_face_loader_list; }
} static_face_loader_list;

ساكن مضمن
فراغ free_static_face_loader_list ()
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
ثابت حرف **
hb_face_list_loaders ()
{
  أرجع static_face_loader_list.get_unconst ();
}
#پایان_اگر


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
hb_face_get_خالی ()
{
  أرجع const_cast<hb_face_t *> (&Null (hb_face_t));
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
  أرجع hb_object_reference (face);
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
فراغ
hb_face_destroy (hb_face_t *face)
{
  إذا (!hb_object_destroy (face)) أرجع;

#اگر_تعریف_نشده HB_NO_SHAPER
  لكل (hb_face_t::plan_گره_t *node = face->shape_plans; node; )
  {
    hb_face_t::plan_گره_t *next = node->next;
    hb_shape_plan_destroy (node->shape_plan);
    hb_free (node);
    node = next;
  }
#پایان_اگر

  face->داده.fini ();
  face->جدول.fini ();

  إذا (face->get_جدول_برچسبها_destroy)
    face->get_جدول_برچسبها_destroy (face->get_جدول_برچسبها_user_داده);

  إذا (face->destroy)
    face->destroy (face->user_داده);

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
hb_face_مجموعه_user_داده (hb_face_t          *face,
		       hb_user_داده_کلید_t *key,
		       فراغ *              داده,
		       hb_destroy_func_t   destroy,
		       hb_bool_t           replace)
{
  أرجع hb_object_مجموعه_user_داده (face, key, داده, destroy, replace);
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
فراغ *
hb_face_get_user_داده (ثابت hb_face_t    *face,
		       hb_user_داده_کلید_t *key)
{
  أرجع hb_object_get_user_داده (face, key);
}

/**
 * hb_face_make_immutable:
 * @face: A face object
 *
 * Makes the given face object immutable.
 *
 * Since: 0.9.2
 **/
فراغ
hb_face_make_immutable (hb_face_t *face)
{
  إذا (hb_object_is_immutable (face))
    أرجع;

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
  أرجع hb_object_is_immutable (face);
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
hb_face_reference_جدول (ثابت hb_face_t *face,
			 hb_برچسب_t برچسب_2)
{
  إذا (unlikely (برچسب_2 == HB_برچسب_NONE))
    أرجع hb_blob_get_خالی ();

  أرجع face->reference_جدول (برچسب_2);
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
  hb_blob_t *blob = face->reference_جدول (HB_برچسب_NONE);

  إذا (blob == hb_blob_get_خالی ())
  {
    // If referencing the face blob is not possible (e.g. not implemented by the
    // font functions), use face builder to create a blob out of individual
    // table blobs.
    بیعلامت مجموع_شمار = hb_face_get_جدول_برچسبها (face, 0, اشاره_تهی, اشاره_تهی);
    إذا (مجموع_شمار)
    {
      hb_برچسب_t برچسبها[64];
      بیعلامت شمار = ARRAY_درازی (برچسبها);
      hb_face_t* builder = hb_face_builder_create ();

      لكل (بیعلامت جابه_جایی = 0; جابه_جایی < مجموع_شمار; جابه_جایی += شمار)
      {
        hb_face_get_جدول_برچسبها (face, جابه_جایی, &شمار, برچسبها);
	إذا (unlikely (!شمار))
	  اكسر; // Allocation error
        لكل (بیعلامت i = 0; i < شمار; i++)
        {
	  إذا (unlikely (!برچسبها[i]))
	    استمر;
	  hb_blob_t *جدول = hb_face_reference_جدول (face, برچسبها[i]);
	  hb_face_builder_جمع_کن_جدول (builder, برچسبها[i], جدول);
	  hb_blob_destroy (جدول);
        }
      }

      blob = hb_face_reference_blob (builder);
      hb_face_destroy (builder);
    }
  }

  أرجع blob;
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
فراغ
hb_face_مجموعه_شاخص (hb_face_t    *face,
		   بیعلامت صحيح  شاخص_2)
{
  إذا (hb_object_is_immutable (face))
    أرجع;

  face->شاخص_2 = شاخص_2;
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
بیعلامت صحيح
hb_face_get_شاخص (ثابت hb_face_t *face)
{
  أرجع face->شاخص_2;
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
فراغ
hb_face_مجموعه_upem (hb_face_t    *face,
		  بیعلامت صحيح  upem)
{
  إذا (hb_object_is_immutable (face))
    أرجع;

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
بیعلامت صحيح
hb_face_get_upem (ثابت hb_face_t *face)
{
  أرجع face->get_upem ();
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
فراغ
hb_face_مجموعه_glyph_شمار (hb_face_t    *face,
			 بیعلامت صحيح  glyph_شمار)
{
  إذا (hb_object_is_immutable (face))
    أرجع;

  face->رقم_glyphs = glyph_شمار;
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
بیعلامت صحيح
hb_face_get_glyph_شمار (ثابت hb_face_t *face)
{
  أرجع face->get_رقم_glyphs ();
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
HB_EXTERN فراغ
hb_face_مجموعه_get_جدول_برچسبها_func (hb_face_t *face,
				 hb_get_جدول_برچسبها_func_t func,
				 فراغ                    *user_داده,
				 hb_destroy_func_t        destroy)
{
  إذا (hb_object_is_immutable (face))
  {
    إذا (destroy)
      destroy (user_داده);
    أرجع;
  }

  إذا (face->get_جدول_برچسبها_destroy)
    face->get_جدول_برچسبها_destroy (face->get_جدول_برچسبها_user_داده);

  face->get_جدول_برچسبها_func = func;
  face->get_جدول_برچسبها_user_داده = user_داده;
  face->get_جدول_برچسبها_destroy = destroy;
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
بیعلامت صحيح
hb_face_get_جدول_برچسبها (ثابت hb_face_t *face,
			بیعلامت صحيح  شروع_جابه_جایی,
			بیعلامت صحيح *جدول_شمار_2, /* IN/OUT */
			hb_برچسب_t     *جدول_برچسبها /*بیرون*/)
{
  إذا (!face->get_جدول_برچسبها_func)
  {
    إذا (جدول_شمار_2)
      *جدول_شمار_2 = 0;
    أرجع 0;
  }

  أرجع face->get_جدول_برچسبها_func (face, شروع_جابه_جایی, جدول_شمار_2, جدول_برچسبها, face->get_جدول_برچسبها_user_داده);
}


/*
 * Character set.
 */


#اگر_تعریف_نشده HB_NO_FACE_COLLECT_UNICODES
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
فراغ
hb_face_collect_unicodes (hb_face_t *face,
			  hb_مجموعه_t  *بیرون)
{
  face->جدول.cmap->collect_unicodes (بیرون, face->get_رقم_glyphs ());
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
فراغ
hb_face_collect_nominal_glyph_mapping (hb_face_t *face,
				       hb_map_t  *mapping,
				       hb_مجموعه_t  *unicodes)
{
  hb_مجموعه_t پشته_unicodes;
  إذا (!unicodes)
    unicodes = &پشته_unicodes;
  face->جدول.cmap->collect_mapping (unicodes, mapping, face->get_رقم_glyphs ());
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
فراغ
hb_face_collect_variation_selectors (hb_face_t *face,
				     hb_مجموعه_t  *بیرون)
{
  face->جدول.cmap->collect_variation_selectors (بیرون);
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
فراغ
hb_face_collect_variation_unicodes (hb_face_t *face,
				    hb_codepoint_t variation_selector,
				    hb_مجموعه_t  *بیرون)
{
  face->جدول.cmap->collect_variation_unicodes (variation_selector, بیرون);
}
#پایان_اگر
