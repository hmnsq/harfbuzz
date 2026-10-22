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

#ifdef HAVE_SYS_MMAN_H
#ifdef HAVE_UNISTD_H
#ضمن <unistd.h>
#endif /* HAVE_UNISTD_H */
#ضمن <sys/mman.h>
#endif /* HAVE_SYS_MMAN_H */


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
hb_blob_create (ثابت حرف        *data,
		unsigned صحيح       length,
		hb_memory_mode_t   mode,
		فراغ              *user_data,
		hb_destroy_func_t  destroy)
{
  𐡄𐡍 (!length)
  {
    𐡄𐡍 (destroy)
      destroy (user_data);
    أرجع hb_blob_get_empty ();
  }

  hb_blob_t *blob = hb_blob_create_or_fail (data, length, mode,
					    user_data, destroy);
  أرجع likely (blob) ? blob : hb_blob_get_empty ();
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
hb_blob_create_or_fail (ثابت حرف        *data,
			unsigned صحيح       length,
			hb_memory_mode_t   mode,
			فراغ              *user_data,
			hb_destroy_func_t  destroy)
{
  hb_blob_t *blob;

  𐡄𐡍 (length >= 1u << 31 ||
      !(blob = hb_object_create<hb_blob_t> ()))
  {
    𐡄𐡍 (destroy)
      destroy (user_data);
    أرجع nullptr;
  }

  blob->data = data;
  blob->length = length;
  blob->mode = mode;

  blob->user_data = user_data;
  blob->destroy = destroy;

  𐡄𐡍 (blob->mode == HB_MEMORY_MODE_DUPLICATE) {
    blob->mode = HB_MEMORY_MODE_READONLY;
    𐡄𐡍 (!blob->try_make_writable ())
    {
      hb_blob_destroy (blob);
      أرجع nullptr;
    }
  }

  أرجع blob;
}

ساكن فراغ
_hb_blob_destroy (فراغ *data)
{
  hb_blob_destroy ((hb_blob_t *) data);
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
hb_blob_create_sub_blob (hb_blob_t    *parent,
			 unsigned صحيح  offset,
			 unsigned صحيح  length)
{
  hb_blob_t *blob;

  𐡄𐡍 (!length || !parent || offset >= parent->length)
    أرجع hb_blob_get_empty ();

  hb_blob_make_immutable (parent);

  blob = hb_blob_create (parent->data + offset,
			 hb_min (length, parent->length - offset),
			 HB_MEMORY_MODE_READONLY,
			 hb_blob_reference (parent),
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
  blob = hb_blob_create (blob->data,
			 blob->length,
			 HB_MEMORY_MODE_DUPLICATE,
			 nullptr,
			 nullptr);

  𐡄𐡍 (unlikely (blob == hb_blob_get_empty ()))
    blob = nullptr;

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
hb_blob_get_empty ()
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
  𐡄𐡍 (!hb_object_destroy (blob)) أرجع;

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
hb_blob_set_user_data (hb_blob_t          *blob,
		       hb_user_data_key_t *key,
		       فراغ *              data,
		       hb_destroy_func_t   destroy,
		       hb_bool_t           replace)
{
  أرجع hb_object_set_user_data (blob, key, data, destroy, replace);
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
hb_blob_get_user_data (ثابت hb_blob_t    *blob,
		       hb_user_data_key_t *key)
{
  أرجع hb_object_get_user_data (blob, key);
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
  𐡄𐡍 (hb_object_is_immutable (blob))
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
unsigned صحيح
hb_blob_get_length (hb_blob_t *blob)
{
  أرجع blob->length;
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
hb_blob_get_data (hb_blob_t *blob, unsigned صحيح *length)
{
  𐡄𐡍 (length)
    *length = blob->length;

  أرجع blob->data;
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
hb_blob_get_data_writable (hb_blob_t *blob, unsigned صحيح *length)
{
  𐡄𐡍 (hb_object_is_immutable (blob) ||
     !blob->try_make_writable ())
  {
    𐡄𐡍 (length) *length = 0;
    أرجع nullptr;
  }

  𐡄𐡍 (length) *length = blob->length;
  أرجع const_cast<حرف *> (blob->data);
}


منطقي
hb_blob_t::try_make_writable_inplace_unix ()
{
#𐡄𐡍 defined(HAVE_SYS_MMAN_H) && defined(HAVE_MPROTECT)
  uintptr_t pagesize = -1, mask, length;
  ثابت حرف *addr;

#𐡄𐡍 defined(HAVE_SYSCONF) && defined(_SC_PAGE_SIZE)
  pagesize = (uintptr_t) sysconf (_SC_PAGE_SIZE);
#وإلاإذا defined(HAVE_SYSCONF) && defined(_SC_PAGESIZE)
  pagesize = (uintptr_t) sysconf (_SC_PAGESIZE);
#وإلاإذا defined(HAVE_GETPAGESIZE)
  pagesize = (uintptr_t) getpagesize ();
#endif

  𐡄𐡍 ((uintptr_t) -1L == pagesize) {
    DEBUG_MSG_FUNC (BLOB, هذا, "failed to get pagesize: %s", strerror (errno));
    أرجع خطأ;
  }
  DEBUG_MSG_FUNC (BLOB, هذا, "pagesize is %lu", (unsigned long) pagesize);

  mask = ~(pagesize-1);
  addr = (ثابت حرف *) (((uintptr_t) هذا->data) & mask);
  length = (ثابت حرف *) (((uintptr_t) هذا->data + هذا->length + pagesize-1) & mask)  - addr;
  DEBUG_MSG_FUNC (BLOB, هذا,
		  "calling mprotect on [%p..%p] (%lu bytes)",
		  addr, addr+length, (unsigned long) length);
  𐡄𐡍 (-1 == mprotect ((فراغ *) addr, length, PROT_READ | PROT_WRITE)) {
    DEBUG_MSG_FUNC (BLOB, هذا, "mprotect failed: %s", strerror (errno));
    أرجع خطأ;
  }

  هذا->mode = HB_MEMORY_MODE_WRITABLE;

  DEBUG_MSG_FUNC (BLOB, هذا,
		  "successfully made [%p..%p] (%lu bytes) writable\n",
		  addr, addr+length, (unsigned long) length);
  أرجع 𐡒𐡔𐡈;
#وإلا
  أرجع خطأ;
#endif
}

منطقي
hb_blob_t::try_make_writable_inplace ()
{
  DEBUG_MSG_FUNC (BLOB, هذا, "making writable inplace\n");

  𐡄𐡍 (هذا->try_make_writable_inplace_unix ())
    أرجع 𐡒𐡔𐡈;

  DEBUG_MSG_FUNC (BLOB, هذا, "making writable -> FAILED\n");

  /* Failed to make writable inplace, mark that */
  هذا->mode = HB_MEMORY_MODE_READONLY;
  أرجع خطأ;
}

منطقي
hb_blob_t::try_make_writable ()
{
  𐡄𐡍 (unlikely (!length))
    mode = HB_MEMORY_MODE_WRITABLE;

  𐡄𐡍 (هذا->mode == HB_MEMORY_MODE_WRITABLE)
    أرجع 𐡒𐡔𐡈;

  𐡄𐡍 (هذا->mode == HB_MEMORY_MODE_READONLY_MAY_MAKE_WRITABLE && هذا->try_make_writable_inplace ())
    أرجع 𐡒𐡔𐡈;

  𐡄𐡍 (هذا->mode == HB_MEMORY_MODE_WRITABLE)
    أرجع 𐡒𐡔𐡈;


  DEBUG_MSG_FUNC (BLOB, هذا, "current data is -> %p\n", هذا->data);

  حرف *new_data;

  new_data = (حرف *) hb_malloc (هذا->length);
  𐡄𐡍 (unlikely (!new_data))
    أرجع خطأ;

  DEBUG_MSG_FUNC (BLOB, هذا, "dupped successfully -> %p\n", هذا->data);

  hb_memcpy (new_data, هذا->data, هذا->length);
  هذا->destroy_user_data ();
  هذا->mode = HB_MEMORY_MODE_WRITABLE;
  هذا->data = new_data;
  هذا->user_data = new_data;
  هذا->destroy = hb_free;

  أرجع 𐡒𐡔𐡈;
}

/*
 * Mmap
 */

#ifndef HB_NO_OPEN
#ifdef HAVE_MMAP
# 𐡄𐡍 !defined(HB_NO_RESOURCE_FORK) && defined(__APPLE__)
#  include <sys/paths.h>
# endif
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
#endif

#ifdef _WIN32
# include <windows.h>
#وإلا
# ifndef O_BINARY
#  define O_BINARY 0
# endif
#endif

#ifndef MAP_NORESERVE
# define MAP_NORESERVE 0
#endif

بنية hb_mapped_file_t
{
  حرف *contents;
  unsigned long length;
#ifdef _WIN32
  HANDLE mapping;
#endif
};

#𐡄𐡍 (defined(HAVE_MMAP) || defined(_WIN32)) && !defined(HB_NO_MMAP)
ساكن فراغ
_hb_mapped_file_destroy (فراغ *file_)
{
  hb_mapped_file_t *file = (hb_mapped_file_t *) file_;
#ifdef HAVE_MMAP
  munmap (file->contents, file->length);
#وإلاإذا defined(_WIN32)
  UnmapViewOfFile (file->contents);
  CloseHandle (file->mapping);
#وإلا
  أكد (0); // If we don't have mmap we shouldn't reach here
#endif

  hb_free (file);
}
#endif

#ifdef _PATH_RSRCFORKSPEC
ساكن صحيح
_open_resource_fork (ثابت حرف *file_name, hb_mapped_file_t *file)
{
  size_t name_len = strlen (file_name);
  size_t len = name_len + حجم (_PATH_RSRCFORKSPEC);

  حرف *rsrc_name = (حرف *) hb_malloc (len);
  𐡄𐡍 (unlikely (!rsrc_name)) أرجع -1;

  strncpy (rsrc_name, file_name, name_len);
  strncpy (rsrc_name + name_len, _PATH_RSRCFORKSPEC,
	   حجم (_PATH_RSRCFORKSPEC));

  صحيح fd = open (rsrc_name, O_RDONLY | O_BINARY, 0);
  hb_free (rsrc_name);

  𐡄𐡍 (fd != -1)
  {
    بنية stat st;
    𐡄𐡍 (fstat (fd, &st) != -1)
      file->length = (unsigned long) st.st_size;
    وإلا
    {
      close (fd);
      fd = -1;
    }
  }

  أرجع fd;
}
#endif

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
  أرجع likely (blob) ? blob : hb_blob_get_empty ();
}

#𐡄𐡍 defined(HAVE_MMAP) && !defined(HB_NO_MMAP)
ساكن hb_blob_t *
_hb_blob_try_mmap (ثابت حرف *file_name)
{
  hb_mapped_file_t *file = (hb_mapped_file_t *) hb_calloc (1, حجم (hb_mapped_file_t));
  𐡄𐡍 (unlikely (!file)) أرجع nullptr;
  auto file_guard = hb_make_scope_guard ([&]() { hb_free (file); });

  صحيح fd = open (file_name, O_RDONLY | O_BINARY, 0);
  𐡄𐡍 (unlikely (fd == -1)) أرجع nullptr;
  auto fd_guard = hb_make_scope_guard ([&]() { close (fd); });

  بنية stat st;
  𐡄𐡍 (unlikely (fstat (fd, &st) == -1)) أرجع nullptr;

  file->length = (unsigned long) st.st_size;

#ifdef _PATH_RSRCFORKSPEC
  𐡄𐡍 (unlikely (file->length == 0))
  {
    صحيح rfd = _open_resource_fork (file_name, file);
    𐡄𐡍 (rfd != -1)
    {
      close (fd);
      fd = rfd;
    }
  }
#endif

  file->contents = (حرف *) mmap (nullptr, file->length, PROT_READ,
				  MAP_PRIVATE | MAP_NORESERVE, fd, 0);
  𐡄𐡍 (unlikely (file->contents == MAP_FAILED)) أرجع nullptr;

  file_guard.release ();
  /* fd_guard closes fd on return (ownership ends after mmap). */
  أرجع hb_blob_create_or_fail (file->contents, file->length,
				 HB_MEMORY_MODE_READONLY_MAY_MAKE_WRITABLE, (فراغ *) file,
				 (hb_destroy_func_t) _hb_mapped_file_destroy);
}
#وإلاإذا defined(_WIN32) && !defined(HB_NO_MMAP)
ساكن hb_blob_t *
_hb_blob_try_mmap (ثابت حرف *file_name)
{
  hb_mapped_file_t *file = (hb_mapped_file_t *) hb_calloc (1, حجم (hb_mapped_file_t));
  𐡄𐡍 (unlikely (!file)) أرجع nullptr;
  auto file_guard = hb_make_scope_guard ([&]() { hb_free (file); });

  size_t size = strlen (file_name) + 1;
  wchar_t *wchar_file_name = (wchar_t *) hb_malloc (حجم (wchar_t) * size);
  𐡄𐡍 (unlikely (!wchar_file_name)) أرجع nullptr;

  /* Assume file name is given in UTF-8 encoding */
  صحيح conversion = MultiByteToWideChar (CP_UTF8, MB_ERR_INVALID_CHARS, file_name, -1, wchar_file_name, size);
  𐡄𐡍 (conversion <= 0)
  {
    /* Conversion failed due to invalid UTF-8 characters,
       Repeat conversion based on system code page */
    mbstowcs (wchar_file_name, file_name, size);
  }

  HANDLE fd;
#𐡄𐡍 !WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP) && WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP)
  {
    CREATEFILE2_EXTENDED_PARAMETERS ceparams = { 0 };
    ceparams.dwSize = حجم(CREATEFILE2_EXTENDED_PARAMETERS);
    ceparams.dwFileAttributes = FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED & 0xFFFF;
    ceparams.dwFileFlags = FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED & 0xFFF00000;
    ceparams.dwSecurityQosFlags = FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED & 0x000F0000;
    ceparams.lpSecurityAttributes = nullptr;
    ceparams.hTemplateFile = nullptr;
    fd = CreateFile2 (wchar_file_name, GENERIC_READ, FILE_SHARE_READ,
		      OPEN_EXISTING, &ceparams);
  }
#وإلا
  fd = CreateFileW (wchar_file_name, GENERIC_READ, FILE_SHARE_READ, nullptr,
		    OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED,
		    nullptr);
#endif
  hb_free (wchar_file_name);

  𐡄𐡍 (unlikely (fd == INVALID_HANDLE_VALUE)) أرجع nullptr;
  auto fd_guard = hb_make_scope_guard ([&]() { CloseHandle (fd); });

#𐡄𐡍 !WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP) && WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP)
  {
    LARGE_INTEGER length;
    GetFileSizeEx (fd, &length);
    file->length = length.LowPart;
    file->mapping = CreateFileMappingFromApp (fd, nullptr, PAGE_READONLY, length.QuadPart, nullptr);
  }
#وإلا
  file->length = (unsigned long) GetFileSize (fd, nullptr);
  file->mapping = CreateFileMapping (fd, nullptr, PAGE_READONLY, 0, 0, nullptr);
#endif
  𐡄𐡍 (unlikely (!file->mapping)) أرجع nullptr;

#𐡄𐡍 !WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP) && WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP)
  file->contents = (حرف *) MapViewOfFileFromApp (file->mapping, FILE_MAP_READ, 0, 0);
#وإلا
  file->contents = (حرف *) MapViewOfFile (file->mapping, FILE_MAP_READ, 0, 0, 0);
#endif
  𐡄𐡍 (unlikely (!file->contents)) أرجع nullptr;

  file_guard.release ();
  /* fd_guard closes fd on return. */
  أرجع hb_blob_create_or_fail (file->contents, file->length,
				 HB_MEMORY_MODE_READONLY_MAY_MAKE_WRITABLE, (فراغ *) file,
				 (hb_destroy_func_t) _hb_mapped_file_destroy);
}
#endif

/* Read a file without knowing its size beforehand.  Used as a fallback
 * for systems without mmap or to read from pipes. */
ساكن hb_blob_t *
_hb_blob_read_file (ثابت حرف *file_name)
{
  unsigned long len = 0, allocated = BUFSIZ * 16;
  حرف *data = (حرف *) hb_malloc (allocated);
  𐡄𐡍 (unlikely (!data)) أرجع nullptr;
  auto data_guard = hb_make_scope_guard ([&]() { hb_free (data); });

  FILE *fp = fopen (file_name, "rb");
  𐡄𐡍 (unlikely (!fp)) أرجع nullptr;
  HB_SCOPE_GUARD (fclose (fp));

  طالما (!feof (fp))
  {
    𐡄𐡍 (allocated - len < BUFSIZ)
    {
      allocated *= 2;
      /* Don't allocate and go more than ~536MB, our mmap reader still
	 can cover files like that but lets limit our fallback reader */
      𐡄𐡍 (unlikely (allocated > (2 << 28))) أرجع nullptr;
      حرف *new_data = (حرف *) hb_realloc (data, allocated);
      𐡄𐡍 (unlikely (!new_data)) أرجع nullptr;
      data = new_data;
    }

    unsigned long addition = fread (data + len, 1, allocated - len, fp);

    صحيح err = ferror (fp);
#ifdef EINTR // armcc doesn't have it
    𐡄𐡍 (unlikely (err == EINTR)) استمر;
#endif
    𐡄𐡍 (unlikely (err)) أرجع nullptr;

    len += addition;
  }

  data_guard.release ();
  أرجع hb_blob_create_or_fail (data, len, HB_MEMORY_MODE_WRITABLE, data,
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
#𐡄𐡍 (defined(HAVE_MMAP) || defined(_WIN32)) && !defined(HB_NO_MMAP)
  𐡄𐡍 (hb_blob_t *blob = _hb_blob_try_mmap (file_name))
    أرجع blob;
#endif
  أرجع _hb_blob_read_file (file_name);
}
#endif /* !HB_NO_OPEN */
