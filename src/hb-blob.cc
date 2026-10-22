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

#ضمن "hb.hh"
#ضمن "hb-blob.hh"

#جے_تعریف_شدہ HAVE_SYS_MMAN_H
#جے_تعریف_شدہ HAVE_UNISTD_H
#ضمن <unistd.h>
#جے_انت /* HAVE_UNISTD_H */
#ضمن <sys/mman.h>
#جے_انت /* HAVE_SYS_MMAN_H */


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
hb_blob_create (ثابت حرف        *انکڑے,
		بنانشانی صحيح       لمائی,
		hb_حافظہ_منوال_t   منوال,
		فراغ              *user_انکڑے,
		hb_destroy_func_t  destroy)
{
  إذا (!لمائی)
  {
    إذا (destroy)
      destroy (user_انکڑے);
    أرجع hb_blob_get_خالی_تھاواں ();
  }

  hb_blob_t *blob = hb_blob_create_or_fail (انکڑے, لمائی, منوال,
					    user_انکڑے, destroy);
  أرجع likely (blob) ? blob : hb_blob_get_خالی_تھاواں ();
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
hb_blob_create_or_fail (ثابت حرف        *انکڑے,
			بنانشانی صحيح       لمائی,
			hb_حافظہ_منوال_t   منوال,
			فراغ              *user_انکڑے,
			hb_destroy_func_t  destroy)
{
  hb_blob_t *blob;

  إذا (لمائی >= 1u << 31 ||
      !(blob = hb_object_create<hb_blob_t> ()))
  {
    إذا (destroy)
      destroy (user_انکڑے);
    أرجع خالی_سوچک;
  }

  blob->انکڑے = انکڑے;
  blob->لمائی = لمائی;
  blob->منوال = منوال;

  blob->user_انکڑے = user_انکڑے;
  blob->destroy = destroy;

  إذا (blob->منوال == HB_حافظہ_منوال_DUPLICATE) {
    blob->منوال = HB_حافظہ_منوال_READONLY;
    إذا (!blob->try_make_writable ())
    {
      hb_blob_destroy (blob);
      أرجع خالی_سوچک;
    }
  }

  أرجع blob;
}

ساكن فراغ
_hb_blob_destroy (فراغ *انکڑے)
{
  hb_blob_destroy ((hb_blob_t *) انکڑے);
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
hb_blob_create_sub_blob (hb_blob_t    *ماپا,
			 بنانشانی صحيح  وتھ,
			 بنانشانی صحيح  لمائی)
{
  hb_blob_t *blob;

  إذا (!لمائی || !ماپا || وتھ >= ماپا->لمائی)
    أرجع hb_blob_get_خالی_تھاواں ();

  hb_blob_make_immutable (ماپا);

  blob = hb_blob_create (ماپا->انکڑے + وتھ,
			 hb_min (لمائی, ماپا->لمائی - وتھ),
			 HB_حافظہ_منوال_READONLY,
			 hb_blob_reference (ماپا),
			 _hb_blob_destroy);

  أرجع blob;
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
  blob = hb_blob_create (blob->انکڑے,
			 blob->لمائی,
			 HB_حافظہ_منوال_DUPLICATE,
			 خالی_سوچک,
			 خالی_سوچک);

  إذا (unlikely (blob == hb_blob_get_خالی_تھاواں ()))
    blob = خالی_سوچک;

  أرجع blob;
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
hb_blob_get_خالی_تھاواں ()
{
  أرجع const_cast<hb_blob_t *> (&Null (hb_blob_t));
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
  أرجع hb_object_reference (blob);
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
فراغ
hb_blob_destroy (hb_blob_t *blob)
{
  إذا (!hb_object_destroy (blob)) أرجع;

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
hb_blob_سیٹ_user_انکڑے (hb_blob_t          *blob,
		       hb_user_انکڑے_کلید_t *key,
		       فراغ *              انکڑے,
		       hb_destroy_func_t   destroy,
		       hb_bool_t           replace)
{
  أرجع hb_object_سیٹ_user_انکڑے (blob, key, انکڑے, destroy, replace);
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
فراغ *
hb_blob_get_user_انکڑے (ثابت hb_blob_t    *blob,
		       hb_user_انکڑے_کلید_t *key)
{
  أرجع hb_object_get_user_انکڑے (blob, key);
}


/**
 * hb_blob_make_immutable:
 * @blob: a blob
 *
 * Makes a blob immutable.
 *
 * Since: 0.9.2
 **/
فراغ
hb_blob_make_immutable (hb_blob_t *blob)
{
  إذا (hb_object_is_immutable (blob))
    أرجع;

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
  أرجع hb_object_is_immutable (blob);
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
بنانشانی صحيح
hb_blob_get_لمائی (hb_blob_t *blob)
{
  أرجع blob->لمائی;
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
hb_blob_get_انکڑے (hb_blob_t *blob, بنانشانی صحيح *لمائی)
{
  إذا (لمائی)
    *لمائی = blob->لمائی;

  أرجع blob->انکڑے;
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
hb_blob_get_انکڑے_writable (hb_blob_t *blob, بنانشانی صحيح *لمائی)
{
  إذا (hb_object_is_immutable (blob) ||
     !blob->try_make_writable ())
  {
    إذا (لمائی) *لمائی = 0;
    أرجع خالی_سوچک;
  }

  إذا (لمائی) *لمائی = blob->لمائی;
  أرجع const_cast<حرف *> (blob->انکڑے);
}


منطقي
hb_blob_t::try_make_writable_inplace_unix ()
{
#إذا defined(HAVE_SYS_MMAN_H) && defined(HAVE_MPROTECT)
  uintptr_t pagesize = -1, نقاب, لمائی;
  ثابت حرف *addr;

#إذا defined(HAVE_SYSCONF) && defined(SC_ورقہ_ناپ)
  pagesize = (uintptr_t) sysconf (SC_ورقہ_ناپ);
#وإلاإذا defined(HAVE_SYSCONF) && defined(_SC_PAGESIZE)
  pagesize = (uintptr_t) sysconf (_SC_PAGESIZE);
#وإلاإذا defined(HAVE_GETPAGESIZE)
  pagesize = (uintptr_t) getpagesize ();
#جے_انت

  إذا ((uintptr_t) -1L == pagesize) {
    DEBUG_پیغام_FUNC (BLOB, هذا, "failed to get pagesize: %s", strerror (errno));
    أرجع خطأ;
  }
  DEBUG_پیغام_FUNC (BLOB, هذا, "pagesize is %lu", (بنانشانی لما) pagesize);

  نقاب = ~(pagesize-1);
  addr = (ثابت حرف *) (((uintptr_t) هذا->انکڑے) & نقاب);
  لمائی = (ثابت حرف *) (((uintptr_t) هذا->انکڑے + هذا->لمائی + pagesize-1) & نقاب)  - addr;
  DEBUG_پیغام_FUNC (BLOB, هذا,
		  "calling mprotect on [%p..%p] (%lu bytes)",
		  addr, addr+لمائی, (بنانشانی لما) لمائی);
  إذا (-1 == mprotect ((فراغ *) addr, لمائی, PROT_READ | PROT_WRITE)) {
    DEBUG_پیغام_FUNC (BLOB, هذا, "mprotect failed: %s", strerror (errno));
    أرجع خطأ;
  }

  هذا->منوال = HB_حافظہ_منوال_WRITABLE;

  DEBUG_پیغام_FUNC (BLOB, هذا,
		  "successfully made [%p..%p] (%lu bytes) writable\n",
		  addr, addr+لمائی, (بنانشانی لما) لمائی);
  أرجع سچ;
#وإلا
  أرجع خطأ;
#جے_انت
}

منطقي
hb_blob_t::try_make_writable_inplace ()
{
  DEBUG_پیغام_FUNC (BLOB, هذا, "making writable inplace\n");

  إذا (هذا->try_make_writable_inplace_unix ())
    أرجع سچ;

  DEBUG_پیغام_FUNC (BLOB, هذا, "making writable -> FAILED\n");

  /* Failed to make writable inplace, mark that */
  هذا->منوال = HB_حافظہ_منوال_READONLY;
  أرجع خطأ;
}

منطقي
hb_blob_t::try_make_writable ()
{
  إذا (unlikely (!لمائی))
    منوال = HB_حافظہ_منوال_WRITABLE;

  إذا (هذا->منوال == HB_حافظہ_منوال_WRITABLE)
    أرجع سچ;

  إذا (هذا->منوال == HB_حافظہ_منوال_READONLY_MAY_MAKE_WRITABLE && هذا->try_make_writable_inplace ())
    أرجع سچ;

  إذا (هذا->منوال == HB_حافظہ_منوال_WRITABLE)
    أرجع سچ;


  DEBUG_پیغام_FUNC (BLOB, هذا, "current data is -> %p\n", هذا->انکڑے);

  حرف *new_انکڑے;

  new_انکڑے = (حرف *) hb_malloc (هذا->لمائی);
  إذا (unlikely (!new_انکڑے))
    أرجع خطأ;

  DEBUG_پیغام_FUNC (BLOB, هذا, "dupped successfully -> %p\n", هذا->انکڑے);

  hb_memcpy (new_انکڑے, هذا->انکڑے, هذا->لمائی);
  هذا->destroy_user_انکڑے ();
  هذا->منوال = HB_حافظہ_منوال_WRITABLE;
  هذا->انکڑے = new_انکڑے;
  هذا->user_انکڑے = new_انکڑے;
  هذا->destroy = hb_free;

  أرجع سچ;
}

/*
 * Mmap
 */

#جے_بنا_تعریف HB_NO_OPEN
#جے_تعریف_شدہ HAVE_MMAP
# إذا !defined(HB_NO_RESOURCE_FORK) && defined(__APPLE__)
#  include <sys/paths.h>
# endif
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
#جے_انت

#جے_تعریف_شدہ _WIN32
# include <windows.h>
#وإلا
# ifndef O_ثنائی
#  define O_ثنائی 0
# endif
#جے_انت

#جے_بنا_تعریف MAP_NORESERVE
# define MAP_NORESERVE 0
#جے_انت

بنية hb_mapped_file_t
{
  حرف *contents;
  بنانشانی لما لمائی;
#جے_تعریف_شدہ _WIN32
  HANDLE mapping;
#جے_انت
};

#إذا (defined(HAVE_MMAP) || defined(_WIN32)) && !defined(HB_NO_MMAP)
ساكن فراغ
_hb_mapped_file_destroy (فراغ *file_)
{
  hb_mapped_file_t *file = (hb_mapped_file_t *) file_;
#جے_تعریف_شدہ HAVE_MMAP
  munmap (file->contents, file->لمائی);
#وإلاإذا defined(_WIN32)
  Unmap_View_دا_File (file->contents);
  CloseHandle (file->mapping);
#وإلا
  أكد (0); // If we don't have mmap we shouldn't reach here
#جے_انت

  hb_free (file);
}
#جے_انت

#جے_تعریف_شدہ رستہ_RSRCFORKSPEC
ساكن صحيح
_open_resource_fork (ثابت حرف *file_name, hb_mapped_file_t *file)
{
  size_t name_len = لڑی_لمبائی (file_name);
  size_t len = name_len + حجم (رستہ_RSRCFORKSPEC);

  حرف *rsrc_name = (حرف *) hb_malloc (len);
  إذا (unlikely (!rsrc_name)) أرجع -1;

  strncpy (rsrc_name, file_name, name_len);
  strncpy (rsrc_name + name_len, رستہ_RSRCFORKSPEC,
	   حجم (رستہ_RSRCFORKSPEC));

  صحيح fd = open (rsrc_name, O_RDONLY | O_ثنائی, 0);
  hb_free (rsrc_name);

  إذا (fd != -1)
  {
    بنية stat st;
    إذا (fstat (fd, &st) != -1)
      file->لمائی = (بنانشانی لما) st.st_ناپ;
    وإلا
    {
      close (fd);
      fd = -1;
    }
  }

  أرجع fd;
}
#جے_انت

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
  أرجع likely (blob) ? blob : hb_blob_get_خالی_تھاواں ();
}

#إذا defined(HAVE_MMAP) && !defined(HB_NO_MMAP)
ساكن hb_blob_t *
_hb_blob_try_mmap (ثابت حرف *file_name)
{
  hb_mapped_file_t *file = (hb_mapped_file_t *) hb_calloc (1, حجم (hb_mapped_file_t));
  إذا (unlikely (!file)) أرجع خالی_سوچک;
  خودکار file_guard = hb_make_scope_guard ([&]() { hb_free (file); });

  صحيح fd = open (file_name, O_RDONLY | O_ثنائی, 0);
  إذا (unlikely (fd == -1)) أرجع خالی_سوچک;
  خودکار fd_guard = hb_make_scope_guard ([&]() { close (fd); });

  بنية stat st;
  إذا (unlikely (fstat (fd, &st) == -1)) أرجع خالی_سوچک;

  file->لمائی = (بنانشانی لما) st.st_ناپ;

#جے_تعریف_شدہ رستہ_RSRCFORKSPEC
  إذا (unlikely (file->لمائی == 0))
  {
    صحيح rfd = _open_resource_fork (file_name, file);
    إذا (rfd != -1)
    {
      close (fd);
      fd = rfd;
    }
  }
#جے_انت

  file->contents = (حرف *) mmap (خالی_سوچک, file->لمائی, PROT_READ,
				  MAP_نجی | MAP_NORESERVE, fd, 0);
  إذا (unlikely (file->contents == MAP_FAILED)) أرجع خالی_سوچک;

  file_guard.release ();
  /* fd_guard closes fd on return (ownership ends after mmap). */
  أرجع hb_blob_create_or_fail (file->contents, file->لمائی,
				 HB_حافظہ_منوال_READONLY_MAY_MAKE_WRITABLE, (فراغ *) file,
				 (hb_destroy_func_t) _hb_mapped_file_destroy);
}
#وإلاإذا defined(_WIN32) && !defined(HB_NO_MMAP)
ساكن hb_blob_t *
_hb_blob_try_mmap (ثابت حرف *file_name)
{
  hb_mapped_file_t *file = (hb_mapped_file_t *) hb_calloc (1, حجم (hb_mapped_file_t));
  إذا (unlikely (!file)) أرجع خالی_سوچک;
  خودکار file_guard = hb_make_scope_guard ([&]() { hb_free (file); });

  size_t ناپ = لڑی_لمبائی (file_name) + 1;
  wchar_t *wchar_file_name = (wchar_t *) hb_malloc (حجم (wchar_t) * ناپ);
  إذا (unlikely (!wchar_file_name)) أرجع خالی_سوچک;

  /* Assume file name is given in UTF-8 encoding */
  صحيح conversion = Multi_Byte_منزل_Wide_Char (CP_UTF8, MB_ERR_INVALID_CHARS, file_name, -1, wchar_file_name, ناپ);
  إذا (conversion <= 0)
  {
    /* Conversion failed due to invalid UTF-8 characters,
       Repeat conversion based on system code page */
    mbstowcs (wchar_file_name, file_name, ناپ);
  }

  HANDLE fd;
#إذا !WINAPI_FAMILY_حصے_کرو(WINAPI_حصے_کرو_DESKTOP) && WINAPI_FAMILY_حصے_کرو(WINAPI_حصے_کرو_APP)
  {
    CREATEFILE2_EXTENDED_PARAMETERS ceparams = { 0 };
    ceparams.dw_ناپ = حجم(CREATEFILE2_EXTENDED_PARAMETERS);
    ceparams.dw_File_گن = FILE_ATTRIBUTE_عام | FILE_FLAG_OVERLAPPED & 0xFFFF;
    ceparams.dwFileFlags = FILE_ATTRIBUTE_عام | FILE_FLAG_OVERLAPPED & 0xFFF00000;
    ceparams.dwSecurityQosFlags = FILE_ATTRIBUTE_عام | FILE_FLAG_OVERLAPPED & 0x000F0000;
    ceparams.lp_Security_گن = خالی_سوچک;
    ceparams.hTemplateFile = خالی_سوچک;
    fd = CreateFile2 (wchar_file_name, GENERIC_READ, FILE_SHARE_READ,
		      OPEN_EXISTING, &ceparams);
  }
#وإلا
  fd = CreateFileW (wchar_file_name, GENERIC_READ, FILE_SHARE_READ, خالی_سوچک,
		    OPEN_EXISTING, FILE_ATTRIBUTE_عام|FILE_FLAG_OVERLAPPED,
		    خالی_سوچک);
#جے_انت
  hb_free (wchar_file_name);

  إذا (unlikely (fd == INVALID_HANDLE_قدر)) أرجع خالی_سوچک;
  خودکار fd_guard = hb_make_scope_guard ([&]() { CloseHandle (fd); });

#إذا !WINAPI_FAMILY_حصے_کرو(WINAPI_حصے_کرو_DESKTOP) && WINAPI_FAMILY_حصے_کرو(WINAPI_حصے_کرو_APP)
  {
    LARGE_INTEGER لمائی;
    Get_File_ناپ_Ex (fd, &لمائی);
    file->لمائی = لمائی.کم_Part;
    file->mapping = CreateFileMappingFromApp (fd, خالی_سوچک, ورقہ_READONLY, لمائی.QuadPart, خالی_سوچک);
  }
#وإلا
  file->لمائی = (بنانشانی لما) Get_File_ناپ (fd, خالی_سوچک);
  file->mapping = CreateFileMapping (fd, خالی_سوچک, ورقہ_READONLY, 0, 0, خالی_سوچک);
#جے_انت
  إذا (unlikely (!file->mapping)) أرجع خالی_سوچک;

#إذا !WINAPI_FAMILY_حصے_کرو(WINAPI_حصے_کرو_DESKTOP) && WINAPI_FAMILY_حصے_کرو(WINAPI_حصے_کرو_APP)
  file->contents = (حرف *) Map_View_دا_File_From_App (file->mapping, FILE_MAP_READ, 0, 0);
#وإلا
  file->contents = (حرف *) Map_View_دا_File (file->mapping, FILE_MAP_READ, 0, 0, 0);
#جے_انت
  إذا (unlikely (!file->contents)) أرجع خالی_سوچک;

  file_guard.release ();
  /* fd_guard closes fd on return. */
  أرجع hb_blob_create_or_fail (file->contents, file->لمائی,
				 HB_حافظہ_منوال_READONLY_MAY_MAKE_WRITABLE, (فراغ *) file,
				 (hb_destroy_func_t) _hb_mapped_file_destroy);
}
#جے_انت

/* Read a file without knowing its size beforehand.  Used as a fallback
 * for systems without mmap or to read from pipes. */
ساكن hb_blob_t *
_hb_blob_read_file (ثابت حرف *file_name)
{
  بنانشانی لما len = 0, allocated = BUFSIZ * 16;
  حرف *انکڑے = (حرف *) hb_malloc (allocated);
  إذا (unlikely (!انکڑے)) أرجع خالی_سوچک;
  خودکار انکڑے_guard = hb_make_scope_guard ([&]() { hb_free (انکڑے); });

  FILE *fp = fopen (file_name, "rb");
  إذا (unlikely (!fp)) أرجع خالی_سوچک;
  HB_SCOPE_GUARD (fclose (fp));

  طالما (!feof (fp))
  {
    إذا (allocated - len < BUFSIZ)
    {
      allocated *= 2;
      /* Don't allocate and go more than ~536MB, our mmap reader still
	 can cover files like that but lets limit our fallback reader */
      إذا (unlikely (allocated > (2 << 28))) أرجع خالی_سوچک;
      حرف *new_انکڑے = (حرف *) hb_realloc (انکڑے, allocated);
      إذا (unlikely (!new_انکڑے)) أرجع خالی_سوچک;
      انکڑے = new_انکڑے;
    }

    بنانشانی لما addition = fread (انکڑے + len, 1, allocated - len, fp);

    صحيح err = ferror (fp);
#جے_تعریف_شدہ EINTR // armcc doesn't have it
    إذا (unlikely (err == EINTR)) استمر;
#جے_انت
    إذا (unlikely (err)) أرجع خالی_سوچک;

    len += addition;
  }

  انکڑے_guard.release ();
  أرجع hb_blob_create_or_fail (انکڑے, len, HB_حافظہ_منوال_WRITABLE, انکڑے,
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
#إذا (defined(HAVE_MMAP) || defined(_WIN32)) && !defined(HB_NO_MMAP)
  إذا (hb_blob_t *blob = _hb_blob_try_mmap (file_name))
    أرجع blob;
#جے_انت
  أرجع _hb_blob_read_file (file_name);
}
#جے_انت /* !HB_NO_OPEN */
