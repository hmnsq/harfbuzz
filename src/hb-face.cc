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

#شامل "hb.hh"

#شامل "hb-face.hh"
#شامل "hb-blob.hh"
#شامل "hb-open-file.hh"
#شامل "hb-ot-face.hh"
#شامل "hb-ot-cmap-table.hh"

#اگر_تعریف_شدہ HAVE_FREETYPE
#شامل "hb-ft.h"
#اگر_ختم
#اگر_تعریف_شدہ HAVE_CORETEXT
#شامل "hb-coretext.h"
#اگر_ختم
#اگر_تعریف_شدہ HAVE_DIRECTWRITE
#شامل "hb-directwrite.h"
#اگر_ختم


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
غیرعلامتی صحیح
hb_face_گنتی (hb_blob_t *blob)
{
  اگر (unlikely (!blob))
    واپس 0;

  hb_sanitize_context_t c (blob);

  خودکار *ot = blob->as<OT::OpenTypeFontFile> ();
  اگر (unlikely (!ot->sanitize (&c)))
    واپس 0;

  واپس ot->get_face_گنتی ();
}

/*
 * hb_face_t
 */

DEFINE_NULL_INSTANCE (hb_face_t) =
{
  HB_OBJECT_سرخی_STATIC,

  0,    /*اشاریہ*/
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
			   خالی                      *user_ڈیٹا,
			   hb_destroy_func_t          destroy)
{
  hb_face_t *face;

  اگر (!reference_جدول_func || !(face = hb_object_create<hb_face_t> ())) {
    اگر (destroy)
      destroy (user_ڈیٹا);
    واپس hb_face_get_خالی_خانے ();
  }

  face->reference_جدول_func = reference_جدول_func;
  face->user_ڈیٹا = user_ڈیٹا;
  face->destroy = destroy;

  face->عدد_glyphs = -1;

  face->ڈیٹا.init0 (face);
  face->جدول.init0 (face);

  واپس face;
}


قسمتعریف ساخت hb_face_for_ڈیٹا_احاطہ_t {
  hb_blob_t *blob;
  uint16_t  اشاریہ_2;
} hb_face_for_ڈیٹا_احاطہ_t;

ساکن hb_face_for_ڈیٹا_احاطہ_t *
hb_face_for_ڈیٹا_احاطہ_create (hb_blob_t *blob, غیرعلامتی صحیح اشاریہ_2)
{
  hb_face_for_ڈیٹا_احاطہ_t *احاطہ;

  احاطہ = (hb_face_for_ڈیٹا_احاطہ_t *) hb_calloc (1, حجم (hb_face_for_ڈیٹا_احاطہ_t));
  اگر (unlikely (!احاطہ))
    واپس خالی_اشاریہ;

  احاطہ->blob = blob;
  احاطہ->اشاریہ_2 = (uint16_t) (اشاریہ_2 & 0xFFFFu);

  واپس احاطہ;
}

ساکن خالی
hb_face_for_ڈیٹا_احاطہ_destroy (خالی *ڈیٹا)
{
  hb_face_for_ڈیٹا_احاطہ_t *احاطہ = (hb_face_for_ڈیٹا_احاطہ_t *) ڈیٹا;

  hb_blob_destroy (احاطہ->blob);
  hb_free (احاطہ);
}

ساکن hb_blob_t *
hb_face_for_ڈیٹا_reference_جدول (hb_face_t *face HB_غیر_مستعمل, hb_لیبل_t لیبل_2, خالی *user_ڈیٹا)
{
  hb_face_for_ڈیٹا_احاطہ_t *ڈیٹا = (hb_face_for_ڈیٹا_احاطہ_t *) user_ڈیٹا;

  اگر (لیبل_2 == HB_لیبل_NONE)
    واپس hb_blob_reference (ڈیٹا->blob);

  ثابت OT::OpenTypeFontFile &ot_file = *ڈیٹا->blob->as<OT::OpenTypeFontFile> ();
  غیرعلامتی صحیح بنیاد_انحراف;
  ثابت OT::OpenTypeFontFace &ot_face = ot_file.get_face (ڈیٹا->اشاریہ_2, &بنیاد_انحراف);

  ثابت OT::Open_Type_جدول &جدول = ot_face.get_جدول_by_لیبل (لیبل_2);

  hb_blob_t *blob = hb_blob_create_sub_blob (ڈیٹا->blob, بنیاد_انحراف + جدول.انحراف, جدول.لمبائی);

  واپس blob;
}

ساکن غیرعلامتی
hb_face_for_ڈیٹا_get_جدول_لیبل_فہرست (ثابت hb_face_t *face HB_غیر_مستعمل,
				  غیرعلامتی صحیح آغاز_انحراف,
				  غیرعلامتی صحیح *جدول_گنتی_2,
				  hb_لیبل_t *جدول_لیبل_فہرست,
				  خالی *user_ڈیٹا)
{
  hb_face_for_ڈیٹا_احاطہ_t *ڈیٹا = (hb_face_for_ڈیٹا_احاطہ_t *) user_ڈیٹا;

  ثابت OT::OpenTypeFontFile &ot_file = *ڈیٹا->blob->as<OT::OpenTypeFontFile> ();
  ثابت OT::OpenTypeFontFace &ot_face = ot_file.get_face (ڈیٹا->اشاریہ_2);

  واپس ot_face.get_جدول_لیبل_فہرست (آغاز_انحراف, جدول_گنتی_2, جدول_لیبل_فہرست);
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
		غیرعلامتی صحیح  اشاریہ_2)
{
  hb_face_t *face;

  اگر (unlikely (!blob))
    blob = hb_blob_get_خالی_خانے ();

  blob = hb_sanitize_context_t ().sanitize_blob<OT::OpenTypeFontFile> (hb_blob_reference (blob));

  hb_face_for_ڈیٹا_احاطہ_t *احاطہ = hb_face_for_ڈیٹا_احاطہ_create (blob, اشاریہ_2);

  اگر (unlikely (!احاطہ))
  {
    hb_blob_destroy (blob);
    واپس hb_face_get_خالی_خانے ();
  }

  face = hb_face_create_for_tables (hb_face_for_ڈیٹا_reference_جدول,
				    احاطہ,
				    hb_face_for_ڈیٹا_احاطہ_destroy);
  hb_face_سیٹ_get_جدول_لیبل_فہرست_func (face,
				   hb_face_for_ڈیٹا_get_جدول_لیبل_فہرست,
				   احاطہ,
				   خالی_اشاریہ);

  face->اشاریہ_2 = اشاریہ_2;

  واپس face;
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
			غیرعلامتی صحیح  اشاریہ_2)
{
  غیرعلامتی عدد_faces = hb_face_گنتی (blob);
  اگر (اشاریہ_2 >= عدد_faces)
    واپس خالی_اشاریہ;

  hb_face_t *face = hb_face_create (blob, اشاریہ_2);
  اگر (hb_object_is_immutable (face))
    واپس خالی_اشاریہ;

  واپس face;
}

#اگر_غیر_تعریف_شدہ HB_NO_OPEN
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
				  غیرعلامتی صحیح  اشاریہ_2)
{
  hb_blob_t *blob = hb_blob_create_from_file_or_fail (file_name);
  اگر (unlikely (!blob))
    واپس خالی_اشاریہ;

  hb_face_t *face = hb_face_create_or_fail (blob, اشاریہ_2);
  hb_blob_destroy (blob);

  واپس face;
}

ساکن ثابت ساخت supported_face_loaders_t {
	حرف name[16];
	hb_face_t * (*from_file) (ثابت حرف *font_file, غیرعلامتی face_اشاریہ);
	hb_face_t * (*from_blob) (hb_blob_t *blob, غیرعلامتی face_اشاریہ);
} supported_face_loaders[] =
{
  {"ot",
#اگر_غیر_تعریف_شدہ HB_NO_OPEN
   hb_face_create_from_file_or_fail,
#ورنہ
   خالی_اشاریہ,
#اگر_ختم
   hb_face_create_or_fail
  },
#اگر_تعریف_شدہ HAVE_FREETYPE
  {"ft",
   hb_ft_face_create_from_file_or_fail,
   hb_ft_face_create_from_blob_or_fail
  },
#اگر_ختم
#اگر_تعریف_شدہ HAVE_CORETEXT
  {"coretext",
   hb_coretext_face_create_from_file_or_fail,
   hb_coretext_face_create_from_blob_or_fail
  },
#اگر_ختم
#اگر_تعریف_شدہ HAVE_DIRECTWRITE
  {"directwrite",
   hb_directwrite_face_create_from_file_or_fail,
   hb_directwrite_face_create_from_blob_or_fail
  },
#اگر_ختم
};

ساکن ثابت حرف *get_default_loader_name ()
{
  ساکن hb_atomic_t<ثابت حرف *> static_loader_name;
  ثابت حرف *loader_name = static_loader_name.get_acquire ();
  اگر (!loader_name)
  {
    loader_name = getenv ("HB_FACE_LOADER");
    اگر (!loader_name)
      loader_name = "";
    اگر (!static_loader_name.cmpexch (خالی_اشاریہ, loader_name))
      loader_name = static_loader_name.get_acquire ();
  }
  واپس loader_name;
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
					غیرعلامتی صحیح  اشاریہ_2,
					ثابت حرف   *loader_name)
{
  // Duplicated in hb_face_create_or_fail_using
  منطقی retry = جھوٹ;
  اگر (!loader_name || !*loader_name)
  {
    loader_name = get_default_loader_name ();
    retry = سچ;
  }
  اگر (loader_name && !*loader_name) loader_name = خالی_اشاریہ;

retry:
  برائے (غیرعلامتی i = 0; i < ARRAY_لمبائی (supported_face_loaders); i++)
  {
    اگر (!loader_name || (supported_face_loaders[i].from_file && !strcmp (supported_face_loaders[i].name, loader_name)))
      واپس supported_face_loaders[i].from_file (file_name, اشاریہ_2);
  }

  اگر (retry)
  {
    retry = جھوٹ;
    loader_name = خالی_اشاریہ;
    جاؤ retry;
  }

  واپس خالی_اشاریہ;
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
			      غیرعلامتی صحیح  اشاریہ_2,
			      ثابت حرف   *loader_name)
{
  // Duplicated in hb_face_create_from_file_or_fail_using
  منطقی retry = جھوٹ;
  اگر (!loader_name || !*loader_name)
  {
    loader_name = get_default_loader_name ();
    retry = سچ;
  }
  اگر (loader_name && !*loader_name) loader_name = خالی_اشاریہ;

retry:
  برائے (غیرعلامتی i = 0; i < ARRAY_لمبائی (supported_face_loaders); i++)
  {
    اگر (!loader_name || (supported_face_loaders[i].from_blob && !strcmp (supported_face_loaders[i].name, loader_name)))
      واپس supported_face_loaders[i].from_blob (blob, اشاریہ_2);
  }

  اگر (retry)
  {
    retry = جھوٹ;
    loader_name = خالی_اشاریہ;
    جاؤ retry;
  }

  واپس خالی_اشاریہ;
}

ساکن اندرونی خالی free_static_face_loader_list ();

ساکن ثابت حرف * ثابت nil_face_loader_list[] = {خالی_اشاریہ};

ساکن ساخت hb_face_loader_list_lazy_loader_t : hb_lazy_loader_t<ثابت حرف *,
								  hb_face_loader_list_lazy_loader_t>
{
  ساکن ثابت حرف ** create ()
  {
    ثابت حرف **face_loader_list = (ثابت حرف **) hb_calloc (1 + ARRAY_لمبائی (supported_face_loaders), حجم (ثابت حرف *));
    اگر (unlikely (!face_loader_list))
      واپس خالی_اشاریہ;

    غیرعلامتی i;
    برائے (i = 0; i < ARRAY_لمبائی (supported_face_loaders); i++)
      face_loader_list[i] = supported_face_loaders[i].name;
    face_loader_list[i] = خالی_اشاریہ;

    hb_atexit (free_static_face_loader_list);

    واپس face_loader_list;
  }
  ساکن خالی destroy (ثابت حرف **l)
  { hb_free (l); }
  ساکن ثابت حرف * ثابت * get_null ()
  { واپس nil_face_loader_list; }
} static_face_loader_list;

ساکن اندرونی
خالی free_static_face_loader_list ()
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
  واپس static_face_loader_list.get_unconst ();
}
#اگر_ختم


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
hb_face_get_خالی_خانے ()
{
  واپس const_cast<hb_face_t *> (&Null (hb_face_t));
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
  واپس hb_object_reference (face);
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
خالی
hb_face_destroy (hb_face_t *face)
{
  اگر (!hb_object_destroy (face)) واپس;

#اگر_غیر_تعریف_شدہ HB_NO_SHAPER
  برائے (hb_face_t::plan_گرہ_t *node = face->shape_plans; node; )
  {
    hb_face_t::plan_گرہ_t *next = node->next;
    hb_shape_plan_destroy (node->shape_plan);
    hb_free (node);
    node = next;
  }
#اگر_ختم

  face->ڈیٹا.fini ();
  face->جدول.fini ();

  اگر (face->get_جدول_لیبل_فہرست_destroy)
    face->get_جدول_لیبل_فہرست_destroy (face->get_جدول_لیبل_فہرست_user_ڈیٹا);

  اگر (face->destroy)
    face->destroy (face->user_ڈیٹا);

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
hb_face_سیٹ_user_ڈیٹا (hb_face_t          *face,
		       hb_user_ڈیٹا_کلید_t *key,
		       خالی *              ڈیٹا,
		       hb_destroy_func_t   destroy,
		       hb_bool_t           replace)
{
  واپس hb_object_سیٹ_user_ڈیٹا (face, key, ڈیٹا, destroy, replace);
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
خالی *
hb_face_get_user_ڈیٹا (ثابت hb_face_t    *face,
		       hb_user_ڈیٹا_کلید_t *key)
{
  واپس hb_object_get_user_ڈیٹا (face, key);
}

/**
 * hb_face_make_immutable:
 * @face: A face object
 *
 * Makes the given face object immutable.
 *
 * Since: 0.9.2
 **/
خالی
hb_face_make_immutable (hb_face_t *face)
{
  اگر (hb_object_is_immutable (face))
    واپس;

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
  واپس hb_object_is_immutable (face);
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
			 hb_لیبل_t لیبل_2)
{
  اگر (unlikely (لیبل_2 == HB_لیبل_NONE))
    واپس hb_blob_get_خالی_خانے ();

  واپس face->reference_جدول (لیبل_2);
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
  hb_blob_t *blob = face->reference_جدول (HB_لیبل_NONE);

  اگر (blob == hb_blob_get_خالی_خانے ())
  {
    // If referencing the face blob is not possible (e.g. not implemented by the
    // font functions), use face builder to create a blob out of individual
    // table blobs.
    غیرعلامتی کل_گنتی = hb_face_get_جدول_لیبل_فہرست (face, 0, خالی_اشاریہ, خالی_اشاریہ);
    اگر (کل_گنتی)
    {
      hb_لیبل_t لیبل_فہرست[64];
      غیرعلامتی گنتی = ARRAY_لمبائی (لیبل_فہرست);
      hb_face_t* builder = hb_face_builder_create ();

      برائے (غیرعلامتی انحراف = 0; انحراف < کل_گنتی; انحراف += گنتی)
      {
        hb_face_get_جدول_لیبل_فہرست (face, انحراف, &گنتی, لیبل_فہرست);
	اگر (unlikely (!گنتی))
	  توڑو; // Allocation error
        برائے (غیرعلامتی i = 0; i < گنتی; i++)
        {
	  اگر (unlikely (!لیبل_فہرست[i]))
	    جاری;
	  hb_blob_t *جدول = hb_face_reference_جدول (face, لیبل_فہرست[i]);
	  hb_face_builder_جوڑو_جدول (builder, لیبل_فہرست[i], جدول);
	  hb_blob_destroy (جدول);
        }
      }

      blob = hb_face_reference_blob (builder);
      hb_face_destroy (builder);
    }
  }

  واپس blob;
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
خالی
hb_face_سیٹ_اشاریہ (hb_face_t    *face,
		   غیرعلامتی صحیح  اشاریہ_2)
{
  اگر (hb_object_is_immutable (face))
    واپس;

  face->اشاریہ_2 = اشاریہ_2;
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
غیرعلامتی صحیح
hb_face_get_اشاریہ (ثابت hb_face_t *face)
{
  واپس face->اشاریہ_2;
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
خالی
hb_face_سیٹ_upem (hb_face_t    *face,
		  غیرعلامتی صحیح  upem)
{
  اگر (hb_object_is_immutable (face))
    واپس;

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
غیرعلامتی صحیح
hb_face_get_upem (ثابت hb_face_t *face)
{
  واپس face->get_upem ();
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
خالی
hb_face_سیٹ_glyph_گنتی (hb_face_t    *face,
			 غیرعلامتی صحیح  glyph_گنتی)
{
  اگر (hb_object_is_immutable (face))
    واپس;

  face->عدد_glyphs = glyph_گنتی;
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
غیرعلامتی صحیح
hb_face_get_glyph_گنتی (ثابت hb_face_t *face)
{
  واپس face->get_عدد_glyphs ();
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
HB_EXTERN خالی
hb_face_سیٹ_get_جدول_لیبل_فہرست_func (hb_face_t *face,
				 hb_get_جدول_لیبل_فہرست_func_t func,
				 خالی                    *user_ڈیٹا,
				 hb_destroy_func_t        destroy)
{
  اگر (hb_object_is_immutable (face))
  {
    اگر (destroy)
      destroy (user_ڈیٹا);
    واپس;
  }

  اگر (face->get_جدول_لیبل_فہرست_destroy)
    face->get_جدول_لیبل_فہرست_destroy (face->get_جدول_لیبل_فہرست_user_ڈیٹا);

  face->get_جدول_لیبل_فہرست_func = func;
  face->get_جدول_لیبل_فہرست_user_ڈیٹا = user_ڈیٹا;
  face->get_جدول_لیبل_فہرست_destroy = destroy;
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
غیرعلامتی صحیح
hb_face_get_جدول_لیبل_فہرست (ثابت hb_face_t *face,
			غیرعلامتی صحیح  آغاز_انحراف,
			غیرعلامتی صحیح *جدول_گنتی_2, /* IN/OUT */
			hb_لیبل_t     *جدول_لیبل_فہرست /*برآمد*/)
{
  اگر (!face->get_جدول_لیبل_فہرست_func)
  {
    اگر (جدول_گنتی_2)
      *جدول_گنتی_2 = 0;
    واپس 0;
  }

  واپس face->get_جدول_لیبل_فہرست_func (face, آغاز_انحراف, جدول_گنتی_2, جدول_لیبل_فہرست, face->get_جدول_لیبل_فہرست_user_ڈیٹا);
}


/*
 * Character set.
 */


#اگر_غیر_تعریف_شدہ HB_NO_FACE_COLLECT_UNICODES
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
خالی
hb_face_collect_unicodes (hb_face_t *face,
			  hb_سیٹ_t  *برآمد)
{
  face->جدول.cmap->collect_unicodes (برآمد, face->get_عدد_glyphs ());
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
خالی
hb_face_collect_nominal_glyph_mapping (hb_face_t *face,
				       hb_map_t  *mapping,
				       hb_سیٹ_t  *unicodes)
{
  hb_سیٹ_t ڈھیر_unicodes;
  اگر (!unicodes)
    unicodes = &ڈھیر_unicodes;
  face->جدول.cmap->collect_mapping (unicodes, mapping, face->get_عدد_glyphs ());
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
خالی
hb_face_collect_variation_selectors (hb_face_t *face,
				     hb_سیٹ_t  *برآمد)
{
  face->جدول.cmap->collect_variation_selectors (برآمد);
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
خالی
hb_face_collect_variation_unicodes (hb_face_t *face,
				    hb_codepoint_t variation_selector,
				    hb_سیٹ_t  *برآمد)
{
  face->جدول.cmap->collect_variation_unicodes (variation_selector, برآمد);
}
#اگر_ختم
