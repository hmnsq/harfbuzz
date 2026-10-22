/*
 * Copyright © 2009  Red Hat, Inc.
 * Copyright © 2018  Ebrahim Byagowi
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
 */

#شامل "hb.hh"
#شامل "hb-blob.hh"

#اگر_تعریف_شدہ HAVE_SYS_MMAN_H
#اگر_تعریف_شدہ HAVE_UNISTD_H
#شامل <unistd.h>
#اگر_ختم /* HAVE_UNISTD_H */
#شامل <sys/mman.h>
#اگر_ختم /* HAVE_SYS_MMAN_H */


/**
 * SECTION: hb-blob
 * @title: hb-blob
 * @short_description: Binary data containers
 * @include: hb.h
 *
 * Blobs wrap a chunk of binary data to handle lifecycle management of data
 * while it is passed between client and HarfBuzz.  Blobs are primarily used
 * to create font faces, but also to access font face tables, as well as
 * pass around other binary data.
 **/


/**
 * hb_blob_create: (skip)
 * @data: Pointer to blob data.
 * @length: Length of @data in bytes.
 * @mode: Memory mode for @data.
 * @user_data: Data parameter to pass to @destroy.
 * @destroy: (nullable): Callback to call when @data is not needed anymore.
 *
 * Creates a new "blob" object wrapping @data.  The @mode parameter is used
 * to negotiate ownership and lifecycle of @data.
 *
 * Return value: New blob, or the empty blob if something failed or if @length is
 * zero.  Destroy with hb_blob_destroy().
 *
 * Since: 0.9.2
 **/
hb_blob_t *
hb_blob_create (ثابت حرف        *ڈیٹا,
		غیرعلامتی صحیح       لمبائی,
		hb_حافظہ_منوال_t   منوال,
		خالی              *user_ڈیٹا,
		hb_destroy_func_t  destroy)
{
  اگر (!لمبائی)
  {
    اگر (destroy)
      destroy (user_ڈیٹا);
    واپس hb_blob_get_خالی_خانے ();
  }

  hb_blob_t *blob = hb_blob_create_or_fail (ڈیٹا, لمبائی, منوال,
					    user_ڈیٹا, destroy);
  واپس likely (blob) ? blob : hb_blob_get_خالی_خانے ();
}

/**
 * hb_blob_create_or_fail: (skip)
 * @data: Pointer to blob data.
 * @length: Length of @data in bytes.
 * @mode: Memory mode for @data.
 * @user_data: Data parameter to pass to @destroy.
 * @destroy: (nullable): Callback to call when @data is not needed anymore.
 *
 * Creates a new "blob" object wrapping @data.  The @mode parameter is used
 * to negotiate ownership and lifecycle of @data.
 *
 * Note that this function returns a freshly-allocated empty blob even if @length
 * is zero. This is in contrast to hb_blob_create(), which returns the singleton
 * empty blob (as returned by hb_blob_get_empty()) if @length is zero.
 *
 * Return value: New blob, or `NULL` if failed.  Destroy with hb_blob_destroy().
 *
 * Since: 2.8.2
 **/
hb_blob_t *
hb_blob_create_or_fail (ثابت حرف        *ڈیٹا,
			غیرعلامتی صحیح       لمبائی,
			hb_حافظہ_منوال_t   منوال,
			خالی              *user_ڈیٹا,
			hb_destroy_func_t  destroy)
{
  hb_blob_t *blob;

  اگر (لمبائی >= 1u << 31 ||
      !(blob = hb_object_create<hb_blob_t> ()))
  {
    اگر (destroy)
      destroy (user_ڈیٹا);
    واپس خالی_اشاریہ;
  }

  blob->ڈیٹا = ڈیٹا;
  blob->لمبائی = لمبائی;
  blob->منوال = منوال;

  blob->user_ڈیٹا = user_ڈیٹا;
  blob->destroy = destroy;

  اگر (blob->منوال == HB_حافظہ_منوال_DUPLICATE) {
    blob->منوال = HB_حافظہ_منوال_READONLY;
    اگر (!blob->try_make_writable ())
    {
      hb_blob_destroy (blob);
      واپس خالی_اشاریہ;
    }
  }

  واپس blob;
}

ساکن خالی
_hb_blob_destroy (خالی *ڈیٹا)
{
  hb_blob_destroy ((hb_blob_t *) ڈیٹا);
}

/**
 * hb_blob_create_sub_blob:
 * @parent: Parent blob.
 * @offset: Start offset of sub-blob within @parent, in bytes.
 * @length: Length of sub-blob.
 *
 * Returns a blob that represents a range of bytes in @parent.  The new
 * blob is always created with #HB_MEMORY_MODE_READONLY, meaning that it
 * will never modify data in the parent blob.  The parent data is not
 * expected to be modified, and will result in undefined behavior if it
 * is.
 *
 * Makes @parent immutable.
 *
 * Return value: New blob, or the empty blob if something failed or if
 * @length is zero or @offset is beyond the end of @parent's data.  Destroy
 * with hb_blob_destroy().
 *
 * Since: 0.9.2
 **/
hb_blob_t *
hb_blob_create_sub_blob (hb_blob_t    *والد,
			 غیرعلامتی صحیح  انحراف,
			 غیرعلامتی صحیح  لمبائی)
{
  hb_blob_t *blob;

  اگر (!لمبائی || !والد || انحراف >= والد->لمبائی)
    واپس hb_blob_get_خالی_خانے ();

  hb_blob_make_immutable (والد);

  blob = hb_blob_create (والد->ڈیٹا + انحراف,
			 hb_min (لمبائی, والد->لمبائی - انحراف),
			 HB_حافظہ_منوال_READONLY,
			 hb_blob_reference (والد),
			 _hb_blob_destroy);

  واپس blob;
}

/**
 * hb_blob_copy_writable_or_fail:
 * @blob: A blob.
 *
 * Makes a writable copy of @blob.
 *
 * Return value: The new blob, or nullptr if allocation failed
 *
 * Since: 1.8.0
 **/
hb_blob_t *
hb_blob_copy_writable_or_fail (hb_blob_t *blob)
{
  blob = hb_blob_create (blob->ڈیٹا,
			 blob->لمبائی,
			 HB_حافظہ_منوال_DUPLICATE,
			 خالی_اشاریہ,
			 خالی_اشاریہ);

  اگر (unlikely (blob == hb_blob_get_خالی_خانے ()))
    blob = خالی_اشاریہ;

  واپس blob;
}

/**
 * hb_blob_get_empty:
 *
 * Returns the singleton empty blob.
 *
 * See TODO:link object types for more information.
 *
 * Return value: (transfer full): The empty blob.
 *
 * Since: 0.9.2
 **/
hb_blob_t *
hb_blob_get_خالی_خانے ()
{
  واپس const_cast<hb_blob_t *> (&Null (hb_blob_t));
}

/**
 * hb_blob_reference: (skip)
 * @blob: a blob.
 *
 * Increases the reference count on @blob.
 *
 * See TODO:link object types for more information.
 *
 * Return value: @blob.
 *
 * Since: 0.9.2
 **/
hb_blob_t *
hb_blob_reference (hb_blob_t *blob)
{
  واپس hb_object_reference (blob);
}

/**
 * hb_blob_destroy: (skip)
 * @blob: a blob.
 *
 * Decreases the reference count on @blob, and if it reaches zero, destroys
 * @blob, freeing all memory, possibly calling the destroy-callback the blob
 * was created for if it has not been called already.
 *
 * See TODO:link object types for more information.
 *
 * Since: 0.9.2
 **/
خالی
hb_blob_destroy (hb_blob_t *blob)
{
  اگر (!hb_object_destroy (blob)) واپس;

  hb_free (blob);
}

/**
 * hb_blob_set_user_data: (skip)
 * @blob: An #hb_blob_t
 * @key: The user-data key to set
 * @data: A pointer to the user data to set
 * @destroy: (nullable): A callback to call when @data is not needed anymore
 * @replace: Whether to replace an existing data with the same key
 *
 * Attaches a user-data key/data pair to the specified blob.
 *
 * Return value: `true` if success, `false` otherwise
 *
 * Since: 0.9.2
 **/
hb_bool_t
hb_blob_سیٹ_user_ڈیٹا (hb_blob_t          *blob,
		       hb_user_ڈیٹا_کلید_t *key,
		       خالی *              ڈیٹا,
		       hb_destroy_func_t   destroy,
		       hb_bool_t           replace)
{
  واپس hb_object_سیٹ_user_ڈیٹا (blob, key, ڈیٹا, destroy, replace);
}

/**
 * hb_blob_get_user_data: (skip)
 * @blob: a blob
 * @key: The user-data key to query
 *
 * Fetches the user data associated with the specified key,
 * attached to the specified font-functions structure.
 *
 * Return value: (transfer none): A pointer to the user data
 *
 * Since: 0.9.2
 **/
خالی *
hb_blob_get_user_ڈیٹا (ثابت hb_blob_t    *blob,
		       hb_user_ڈیٹا_کلید_t *key)
{
  واپس hb_object_get_user_ڈیٹا (blob, key);
}


/**
 * hb_blob_make_immutable:
 * @blob: a blob
 *
 * Makes a blob immutable.
 *
 * Since: 0.9.2
 **/
خالی
hb_blob_make_immutable (hb_blob_t *blob)
{
  اگر (hb_object_is_immutable (blob))
    واپس;

  hb_object_make_immutable (blob);
}

/**
 * hb_blob_is_immutable:
 * @blob: a blob.
 *
 * Tests whether a blob is immutable.
 *
 * Return value: `true` if @blob is immutable, `false` otherwise
 *
 * Since: 0.9.2
 **/
hb_bool_t
hb_blob_is_immutable (hb_blob_t *blob)
{
  واپس hb_object_is_immutable (blob);
}


/**
 * hb_blob_get_length:
 * @blob: a blob.
 *
 * Fetches the length of a blob's data.
 *
 * Return value: the length of @blob data in bytes.
 *
 * Since: 0.9.2
 **/
غیرعلامتی صحیح
hb_blob_get_لمبائی (hb_blob_t *blob)
{
  واپس blob->لمبائی;
}

/**
 * hb_blob_get_data:
 * @blob: a blob.
 * @length: (out): The length in bytes of the data retrieved
 *
 * Fetches the data from a blob.
 *
 * Returns: (nullable) (transfer none) (array length=length): the byte data of @blob.
 *
 * Since: 0.9.2
 **/
ثابت حرف *
hb_blob_get_ڈیٹا (hb_blob_t *blob, غیرعلامتی صحیح *لمبائی)
{
  اگر (لمبائی)
    *لمبائی = blob->لمبائی;

  واپس blob->ڈیٹا;
}

/**
 * hb_blob_get_data_writable:
 * @blob: a blob.
 * @length: (out): output length of the writable data.
 *
 * Tries to make blob data writable (possibly copying it) and
 * return pointer to data.
 *
 * Fails if blob has been made immutable, or if memory allocation
 * fails.
 *
 * Returns: (transfer none) (array length=length): Writable blob data,
 * or `NULL` if failed.
 *
 * Since: 0.9.2
 **/
حرف *
hb_blob_get_ڈیٹا_writable (hb_blob_t *blob, غیرعلامتی صحیح *لمبائی)
{
  اگر (hb_object_is_immutable (blob) ||
     !blob->try_make_writable ())
  {
    اگر (لمبائی) *لمبائی = 0;
    واپس خالی_اشاریہ;
  }

  اگر (لمبائی) *لمبائی = blob->لمبائی;
  واپس const_cast<حرف *> (blob->ڈیٹا);
}


منطقی
hb_blob_t::try_make_writable_inplace_unix ()
{
#اگر defined(HAVE_SYS_MMAN_H) && defined(HAVE_MPROTECT)
  uintptr_t pagesize = -1, نقاب, لمبائی;
  ثابت حرف *addr;

#اگر defined(HAVE_SYSCONF) && defined(SC_صفحہ_سائز)
  pagesize = (uintptr_t) sysconf (SC_صفحہ_سائز);
#وگرنہ_اگر defined(HAVE_SYSCONF) && defined(_SC_PAGESIZE)
  pagesize = (uintptr_t) sysconf (_SC_PAGESIZE);
#وگرنہ_اگر defined(HAVE_GETPAGESIZE)
  pagesize = (uintptr_t) getpagesize ();
#اگر_ختم

  اگر ((uintptr_t) -1L == pagesize) {
    DEBUG_پیغام_FUNC (BLOB, یہ, "failed to get pagesize: %s", strerror (errno));
    واپس جھوٹ;
  }
  DEBUG_پیغام_FUNC (BLOB, یہ, "pagesize is %lu", (غیرعلامتی طویل) pagesize);

  نقاب = ~(pagesize-1);
  addr = (ثابت حرف *) (((uintptr_t) یہ->ڈیٹا) & نقاب);
  لمبائی = (ثابت حرف *) (((uintptr_t) یہ->ڈیٹا + یہ->لمبائی + pagesize-1) & نقاب)  - addr;
  DEBUG_پیغام_FUNC (BLOB, یہ,
		  "calling mprotect on [%p..%p] (%lu bytes)",
		  addr, addr+لمبائی, (غیرعلامتی طویل) لمبائی);
  اگر (-1 == mprotect ((خالی *) addr, لمبائی, PROT_READ | PROT_WRITE)) {
    DEBUG_پیغام_FUNC (BLOB, یہ, "mprotect failed: %s", strerror (errno));
    واپس جھوٹ;
  }

  یہ->منوال = HB_حافظہ_منوال_WRITABLE;

  DEBUG_پیغام_FUNC (BLOB, یہ,
		  "successfully made [%p..%p] (%lu bytes) writable\n",
		  addr, addr+لمبائی, (غیرعلامتی طویل) لمبائی);
  واپس سچ;
#ورنہ
  واپس جھوٹ;
#اگر_ختم
}

منطقی
hb_blob_t::try_make_writable_inplace ()
{
  DEBUG_پیغام_FUNC (BLOB, یہ, "making writable inplace\n");

  اگر (یہ->try_make_writable_inplace_unix ())
    واپس سچ;

  DEBUG_پیغام_FUNC (BLOB, یہ, "making writable -> FAILED\n");

  /* Failed to make writable inplace, mark that */
  یہ->منوال = HB_حافظہ_منوال_READONLY;
  واپس جھوٹ;
}

منطقی
hb_blob_t::try_make_writable ()
{
  اگر (unlikely (!لمبائی))
    منوال = HB_حافظہ_منوال_WRITABLE;

  اگر (یہ->منوال == HB_حافظہ_منوال_WRITABLE)
    واپس سچ;

  اگر (یہ->منوال == HB_حافظہ_منوال_READONLY_MAY_MAKE_WRITABLE && یہ->try_make_writable_inplace ())
    واپس سچ;

  اگر (یہ->منوال == HB_حافظہ_منوال_WRITABLE)
    واپس سچ;


  DEBUG_پیغام_FUNC (BLOB, یہ, "current data is -> %p\n", یہ->ڈیٹا);

  حرف *new_ڈیٹا;

  new_ڈیٹا = (حرف *) hb_malloc (یہ->لمبائی);
  اگر (unlikely (!new_ڈیٹا))
    واپس جھوٹ;

  DEBUG_پیغام_FUNC (BLOB, یہ, "dupped successfully -> %p\n", یہ->ڈیٹا);

  hb_memcpy (new_ڈیٹا, یہ->ڈیٹا, یہ->لمبائی);
  یہ->destroy_user_ڈیٹا ();
  یہ->منوال = HB_حافظہ_منوال_WRITABLE;
  یہ->ڈیٹا = new_ڈیٹا;
  یہ->user_ڈیٹا = new_ڈیٹا;
  یہ->destroy = hb_free;

  واپس سچ;
}

/*
 * Mmap
 */

#اگر_غیر_تعریف_شدہ HB_NO_OPEN
#اگر_تعریف_شدہ HAVE_MMAP
# اگر !defined(HB_NO_RESOURCE_FORK) && defined(__APPLE__)
#  include <sys/paths.h>
# endif
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
#اگر_ختم

#اگر_تعریف_شدہ _WIN32
# include <windows.h>
#ورنہ
# ifndef O_ثنائی
#  define O_ثنائی 0
# endif
#اگر_ختم

#اگر_غیر_تعریف_شدہ MAP_NORESERVE
# define MAP_NORESERVE 0
#اگر_ختم

ساخت hb_mapped_file_t
{
  حرف *contents;
  غیرعلامتی طویل لمبائی;
#اگر_تعریف_شدہ _WIN32
  HANDLE mapping;
#اگر_ختم
};

#اگر (defined(HAVE_MMAP) || defined(_WIN32)) && !defined(HB_NO_MMAP)
ساکن خالی
_hb_mapped_file_destroy (خالی *file_)
{
  hb_mapped_file_t *file = (hb_mapped_file_t *) file_;
#اگر_تعریف_شدہ HAVE_MMAP
  munmap (file->contents, file->لمبائی);
#وگرنہ_اگر defined(_WIN32)
  Unmap_View_کا_File (file->contents);
  CloseHandle (file->mapping);
#ورنہ
  أكد (0); // If we don't have mmap we shouldn't reach here
#اگر_ختم

  hb_free (file);
}
#اگر_ختم

#اگر_تعریف_شدہ راستہ_RSRCFORKSPEC
ساکن صحیح
_open_resource_fork (ثابت حرف *file_name, hb_mapped_file_t *file)
{
  size_t name_len = لڑی_لمبائی (file_name);
  size_t len = name_len + حجم (راستہ_RSRCFORKSPEC);

  حرف *rsrc_name = (حرف *) hb_malloc (len);
  اگر (unlikely (!rsrc_name)) واپس -1;

  strncpy (rsrc_name, file_name, name_len);
  strncpy (rsrc_name + name_len, راستہ_RSRCFORKSPEC,
	   حجم (راستہ_RSRCFORKSPEC));

  صحیح fd = open (rsrc_name, O_RDONLY | O_ثنائی, 0);
  hb_free (rsrc_name);

  اگر (fd != -1)
  {
    ساخت stat st;
    اگر (fstat (fd, &st) != -1)
      file->لمبائی = (غیرعلامتی طویل) st.st_سائز;
    ورنہ
    {
      close (fd);
      fd = -1;
    }
  }

  واپس fd;
}
#اگر_ختم

/**
 * hb_blob_create_from_file:
 * @file_name: A font filename
 *
 * Creates a new blob containing the data from the
 * specified binary font file.
 *
 * The filename is passed directly to the system on all platforms,
 * except on Windows, where the filename is interpreted as UTF-8.
 * Only if the filename is not valid UTF-8, it will be interpreted
 * according to the system codepage.
 *
 * Returns: An #hb_blob_t pointer with the content of the file,
 * or hb_blob_get_empty() if failed.
 *
 * Since: 1.7.7
 **/
hb_blob_t *
hb_blob_create_from_file (ثابت حرف *file_name)
{
  hb_blob_t *blob = hb_blob_create_from_file_or_fail (file_name);
  واپس likely (blob) ? blob : hb_blob_get_خالی_خانے ();
}

#اگر defined(HAVE_MMAP) && !defined(HB_NO_MMAP)
ساکن hb_blob_t *
_hb_blob_try_mmap (ثابت حرف *file_name)
{
  hb_mapped_file_t *file = (hb_mapped_file_t *) hb_calloc (1, حجم (hb_mapped_file_t));
  اگر (unlikely (!file)) واپس خالی_اشاریہ;
  خودکار file_guard = hb_make_scope_guard ([&]() { hb_free (file); });

  صحیح fd = open (file_name, O_RDONLY | O_ثنائی, 0);
  اگر (unlikely (fd == -1)) واپس خالی_اشاریہ;
  خودکار fd_guard = hb_make_scope_guard ([&]() { close (fd); });

  ساخت stat st;
  اگر (unlikely (fstat (fd, &st) == -1)) واپس خالی_اشاریہ;

  file->لمبائی = (غیرعلامتی طویل) st.st_سائز;

#اگر_تعریف_شدہ راستہ_RSRCFORKSPEC
  اگر (unlikely (file->لمبائی == 0))
  {
    صحیح rfd = _open_resource_fork (file_name, file);
    اگر (rfd != -1)
    {
      close (fd);
      fd = rfd;
    }
  }
#اگر_ختم

  file->contents = (حرف *) mmap (خالی_اشاریہ, file->لمبائی, PROT_READ,
				  MAP_نجی | MAP_NORESERVE, fd, 0);
  اگر (unlikely (file->contents == MAP_FAILED)) واپس خالی_اشاریہ;

  file_guard.release ();
  /* fd_guard closes fd on return (ownership ends after mmap). */
  واپس hb_blob_create_or_fail (file->contents, file->لمبائی,
				 HB_حافظہ_منوال_READONLY_MAY_MAKE_WRITABLE, (خالی *) file,
				 (hb_destroy_func_t) _hb_mapped_file_destroy);
}
#وگرنہ_اگر defined(_WIN32) && !defined(HB_NO_MMAP)
ساکن hb_blob_t *
_hb_blob_try_mmap (ثابت حرف *file_name)
{
  hb_mapped_file_t *file = (hb_mapped_file_t *) hb_calloc (1, حجم (hb_mapped_file_t));
  اگر (unlikely (!file)) واپس خالی_اشاریہ;
  خودکار file_guard = hb_make_scope_guard ([&]() { hb_free (file); });

  size_t سائز = لڑی_لمبائی (file_name) + 1;
  wchar_t *wchar_file_name = (wchar_t *) hb_malloc (حجم (wchar_t) * سائز);
  اگر (unlikely (!wchar_file_name)) واپس خالی_اشاریہ;

  /* Assume file name is given in UTF-8 encoding */
  صحیح conversion = Multi_Byte_مقصد_Wide_Char (CP_UTF8, MB_ERR_INVALID_CHARS, file_name, -1, wchar_file_name, سائز);
  اگر (conversion <= 0)
  {
    /* Conversion failed due to invalid UTF-8 characters,
       Repeat conversion based on system code page */
    mbstowcs (wchar_file_name, file_name, سائز);
  }

  HANDLE fd;
#اگر !WINAPI_FAMILY_تقسیم(WINAPI_تقسیم_DESKTOP) && WINAPI_FAMILY_تقسیم(WINAPI_تقسیم_APP)
  {
    CREATEFILE2_EXTENDED_PARAMETERS ceparams = { 0 };
    ceparams.dw_سائز = حجم(CREATEFILE2_EXTENDED_PARAMETERS);
    ceparams.dw_File_اوصاف = FILE_ATTRIBUTE_معمول | FILE_FLAG_OVERLAPPED & 0xFFFF;
    ceparams.dwFileFlags = FILE_ATTRIBUTE_معمول | FILE_FLAG_OVERLAPPED & 0xFFF00000;
    ceparams.dwSecurityQosFlags = FILE_ATTRIBUTE_معمول | FILE_FLAG_OVERLAPPED & 0x000F0000;
    ceparams.lp_Security_اوصاف = خالی_اشاریہ;
    ceparams.hTemplateFile = خالی_اشاریہ;
    fd = CreateFile2 (wchar_file_name, GENERIC_READ, FILE_SHARE_READ,
		      OPEN_EXISTING, &ceparams);
  }
#ورنہ
  fd = CreateFileW (wchar_file_name, GENERIC_READ, FILE_SHARE_READ, خالی_اشاریہ,
		    OPEN_EXISTING, FILE_ATTRIBUTE_معمول|FILE_FLAG_OVERLAPPED,
		    خالی_اشاریہ);
#اگر_ختم
  hb_free (wchar_file_name);

  اگر (unlikely (fd == INVALID_HANDLE_قدر)) واپس خالی_اشاریہ;
  خودکار fd_guard = hb_make_scope_guard ([&]() { CloseHandle (fd); });

#اگر !WINAPI_FAMILY_تقسیم(WINAPI_تقسیم_DESKTOP) && WINAPI_FAMILY_تقسیم(WINAPI_تقسیم_APP)
  {
    LARGE_INTEGER لمبائی;
    Get_File_سائز_Ex (fd, &لمبائی);
    file->لمبائی = لمبائی.کم_Part;
    file->mapping = CreateFileMappingFromApp (fd, خالی_اشاریہ, صفحہ_READONLY, لمبائی.QuadPart, خالی_اشاریہ);
  }
#ورنہ
  file->لمبائی = (غیرعلامتی طویل) Get_File_سائز (fd, خالی_اشاریہ);
  file->mapping = CreateFileMapping (fd, خالی_اشاریہ, صفحہ_READONLY, 0, 0, خالی_اشاریہ);
#اگر_ختم
  اگر (unlikely (!file->mapping)) واپس خالی_اشاریہ;

#اگر !WINAPI_FAMILY_تقسیم(WINAPI_تقسیم_DESKTOP) && WINAPI_FAMILY_تقسیم(WINAPI_تقسیم_APP)
  file->contents = (حرف *) Map_View_کا_File_From_App (file->mapping, FILE_MAP_READ, 0, 0);
#ورنہ
  file->contents = (حرف *) Map_View_کا_File (file->mapping, FILE_MAP_READ, 0, 0, 0);
#اگر_ختم
  اگر (unlikely (!file->contents)) واپس خالی_اشاریہ;

  file_guard.release ();
  /* fd_guard closes fd on return. */
  واپس hb_blob_create_or_fail (file->contents, file->لمبائی,
				 HB_حافظہ_منوال_READONLY_MAY_MAKE_WRITABLE, (خالی *) file,
				 (hb_destroy_func_t) _hb_mapped_file_destroy);
}
#اگر_ختم

/* Read a file without knowing its size beforehand.  Used as a fallback
 * for systems without mmap or to read from pipes. */
ساکن hb_blob_t *
_hb_blob_read_file (ثابت حرف *file_name)
{
  غیرعلامتی طویل len = 0, allocated = BUFSIZ * 16;
  حرف *ڈیٹا = (حرف *) hb_malloc (allocated);
  اگر (unlikely (!ڈیٹا)) واپس خالی_اشاریہ;
  خودکار ڈیٹا_guard = hb_make_scope_guard ([&]() { hb_free (ڈیٹا); });

  FILE *fp = fopen (file_name, "rb");
  اگر (unlikely (!fp)) واپس خالی_اشاریہ;
  HB_SCOPE_GUARD (fclose (fp));

  جب (!feof (fp))
  {
    اگر (allocated - len < BUFSIZ)
    {
      allocated *= 2;
      /* Don't allocate and go more than ~536MB, our mmap reader still
	 can cover files like that but lets limit our fallback reader */
      اگر (unlikely (allocated > (2 << 28))) واپس خالی_اشاریہ;
      حرف *new_ڈیٹا = (حرف *) hb_realloc (ڈیٹا, allocated);
      اگر (unlikely (!new_ڈیٹا)) واپس خالی_اشاریہ;
      ڈیٹا = new_ڈیٹا;
    }

    غیرعلامتی طویل addition = fread (ڈیٹا + len, 1, allocated - len, fp);

    صحیح err = ferror (fp);
#اگر_تعریف_شدہ EINTR // armcc doesn't have it
    اگر (unlikely (err == EINTR)) جاری;
#اگر_ختم
    اگر (unlikely (err)) واپس خالی_اشاریہ;

    len += addition;
  }

  ڈیٹا_guard.release ();
  واپس hb_blob_create_or_fail (ڈیٹا, len, HB_حافظہ_منوال_WRITABLE, ڈیٹا,
				 (hb_destroy_func_t) hb_free);
}

/**
 * hb_blob_create_from_file_or_fail:
 * @file_name: A filename
 *
 * Creates a new blob containing the data from the specified file.
 *
 * The filename is passed directly to the system on all platforms,
 * except on Windows, where the filename is interpreted as UTF-8.
 * Only if the filename is not valid UTF-8, it will be interpreted
 * according to the system codepage.
 *
 * Returns: An #hb_blob_t pointer with the content of the file,
 * or `NULL` if failed.
 *
 * Since: 2.8.2
 **/
hb_blob_t *
hb_blob_create_from_file_or_fail (ثابت حرف *file_name)
{
  /* Adopted from glib's gmappedfile.c with Matthias Clasen and
     Allison Lortie permission but changed a lot to suit our need. */
#اگر (defined(HAVE_MMAP) || defined(_WIN32)) && !defined(HB_NO_MMAP)
  اگر (hb_blob_t *blob = _hb_blob_try_mmap (file_name))
    واپس blob;
#اگر_ختم
  واپس _hb_blob_read_file (file_name);
}
#اگر_ختم /* !HB_NO_OPEN */
