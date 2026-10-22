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

#অন্তর্ভাব "hb.hh"
#অন্তর্ভাব "hb-blob.hh"

#ifdef HAVE_SYS_MMAN_H
#ifdef HAVE_UNISTD_H
#অন্তর্ভাব <unistd.h>
#endif /* HAVE_UNISTD_H */
#অন্তর্ভাব <sys/mman.h>
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
hb_blob_create (ধ্রুব বর্ণ        *data,
		unsigned পূর্ণাঙ্ক       length,
		hb_memory_mode_t   mode,
		শূন্য              *user_data,
		hb_destroy_func_t  destroy)
{
  যদি (!length)
  {
    যদি (destroy)
      destroy (user_data);
    প্রত্যাগম hb_blob_get_empty ();
  }

  hb_blob_t *blob = hb_blob_create_or_fail (data, length, mode,
					    user_data, destroy);
  প্রত্যাগম likely (blob) ? blob : hb_blob_get_empty ();
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
hb_blob_create_or_fail (ধ্রুব বর্ণ        *data,
			unsigned পূর্ণাঙ্ক       length,
			hb_memory_mode_t   mode,
			শূন্য              *user_data,
			hb_destroy_func_t  destroy)
{
  hb_blob_t *blob;

  যদি (length >= 1u << 31 ||
      !(blob = hb_object_create<hb_blob_t> ()))
  {
    যদি (destroy)
      destroy (user_data);
    প্রত্যাগম nullptr;
  }

  blob->data = data;
  blob->length = length;
  blob->mode = mode;

  blob->user_data = user_data;
  blob->destroy = destroy;

  যদি (blob->mode == HB_MEMORY_MODE_DUPLICATE) {
    blob->mode = HB_MEMORY_MODE_READONLY;
    যদি (!blob->try_make_writable ())
    {
      hb_blob_destroy (blob);
      প্রত্যাগম nullptr;
    }
  }

  প্রত্যাগম blob;
}

স্থির শূন্য
_hb_blob_destroy (শূন্য *data)
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
			 unsigned পূর্ণাঙ্ক  offset,
			 unsigned পূর্ণাঙ্ক  length)
{
  hb_blob_t *blob;

  যদি (!length || !parent || offset >= parent->length)
    প্রত্যাগম hb_blob_get_empty ();

  hb_blob_make_immutable (parent);

  blob = hb_blob_create (parent->data + offset,
			 hb_min (length, parent->length - offset),
			 HB_MEMORY_MODE_READONLY,
			 hb_blob_reference (parent),
			 _hb_blob_destroy);

  প্রত্যাগম blob;
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

  যদি (unlikely (blob == hb_blob_get_empty ()))
    blob = nullptr;

  প্রত্যাগম blob;
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
  প্রত্যাগম const_cast<hb_blob_t *> (&Null (hb_blob_t));
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
  প্রত্যাগম hb_object_reference (blob);
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
শূন্য
hb_blob_destroy (hb_blob_t *blob)
{
  যদি (!hb_object_destroy (blob)) প্রত্যাগম;

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
		       hb_user_data_চাবি_t *key,
		       শূন্য *              data,
		       hb_destroy_func_t   destroy,
		       hb_bool_t           replace)
{
  প্রত্যাগম hb_object_set_user_data (blob, key, data, destroy, replace);
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
শূন্য *
hb_blob_get_user_data (ধ্রুব hb_blob_t    *blob,
		       hb_user_data_চাবি_t *key)
{
  প্রত্যাগম hb_object_get_user_data (blob, key);
}


/**
 * hb_blob_make_immutable:
 * @blob: a blob
 *
 * Makes a blob immutable.
 *
 * Since: 0.9.2
 **/
শূন্য
hb_blob_make_immutable (hb_blob_t *blob)
{
  যদি (hb_object_is_immutable (blob))
    প্রত্যাগম;

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
  প্রত্যাগম hb_object_is_immutable (blob);
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
unsigned পূর্ণাঙ্ক
hb_blob_get_length (hb_blob_t *blob)
{
  প্রত্যাগম blob->length;
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
ধ্রুব বর্ণ *
hb_blob_get_data (hb_blob_t *blob, unsigned পূর্ণাঙ্ক *length)
{
  যদি (length)
    *length = blob->length;

  প্রত্যাগম blob->data;
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
বর্ণ *
hb_blob_get_data_writable (hb_blob_t *blob, unsigned পূর্ণাঙ্ক *length)
{
  যদি (hb_object_is_immutable (blob) ||
     !blob->try_make_writable ())
  {
    যদি (length) *length = 0;
    প্রত্যাগম nullptr;
  }

  যদি (length) *length = blob->length;
  প্রত্যাগম const_cast<বর্ণ *> (blob->data);
}


তার্কিক
hb_blob_t::try_make_writable_inplace_unix ()
{
#যদি defined(HAVE_SYS_MMAN_H) && defined(HAVE_MPROTECT)
  uintptr_t pagesize = -1, mask, length;
  ধ্রুব বর্ণ *addr;

#যদি defined(HAVE_SYSCONF) && defined(_SC_PAGE_SIZE)
  pagesize = (uintptr_t) sysconf (_SC_PAGE_SIZE);
#নোচেদ্যদি defined(HAVE_SYSCONF) && defined(_SC_PAGESIZE)
  pagesize = (uintptr_t) sysconf (_SC_PAGESIZE);
#নোচেদ্যদি defined(HAVE_GETPAGESIZE)
  pagesize = (uintptr_t) getpagesize ();
#endif

  যদি ((uintptr_t) -1L == pagesize) {
    DEBUG_বার্তা_FUNC (BLOB, এতত্, "failed to get pagesize: %s", strerror (errno));
    প্রত্যাগম অসত্য;
  }
  DEBUG_বার্তা_FUNC (BLOB, এতত্, "pagesize is %lu", (unsigned long) pagesize);

  mask = ~(pagesize-1);
  addr = (ধ্রুব বর্ণ *) (((uintptr_t) এতত্->data) & mask);
  length = (ধ্রুব বর্ণ *) (((uintptr_t) এতত্->data + এতত্->length + pagesize-1) & mask)  - addr;
  DEBUG_বার্তা_FUNC (BLOB, এতত্,
		  "calling mprotect on [%p..%p] (%lu bytes)",
		  addr, addr+length, (unsigned long) length);
  যদি (-1 == mprotect ((শূন্য *) addr, length, PROT_READ | PROT_WRITE)) {
    DEBUG_বার্তা_FUNC (BLOB, এতত্, "mprotect failed: %s", strerror (errno));
    প্রত্যাগম অসত্য;
  }

  এতত্->mode = HB_MEMORY_MODE_WRITABLE;

  DEBUG_বার্তা_FUNC (BLOB, এতত্,
		  "successfully made [%p..%p] (%lu bytes) writable\n",
		  addr, addr+length, (unsigned long) length);
  প্রত্যাগম সত্য;
#অন্যথা
  প্রত্যাগম অসত্য;
#endif
}

তার্কিক
hb_blob_t::try_make_writable_inplace ()
{
  DEBUG_বার্তা_FUNC (BLOB, এতত্, "making writable inplace\n");

  যদি (এতত্->try_make_writable_inplace_unix ())
    প্রত্যাগম সত্য;

  DEBUG_বার্তা_FUNC (BLOB, এতত্, "making writable -> FAILED\n");

  /* Failed to make writable inplace, mark that */
  এতত্->mode = HB_MEMORY_MODE_READONLY;
  প্রত্যাগম অসত্য;
}

তার্কিক
hb_blob_t::try_make_writable ()
{
  যদি (unlikely (!length))
    mode = HB_MEMORY_MODE_WRITABLE;

  যদি (এতত্->mode == HB_MEMORY_MODE_WRITABLE)
    প্রত্যাগম সত্য;

  যদি (এতত্->mode == HB_MEMORY_MODE_READONLY_MAY_MAKE_WRITABLE && এতত্->try_make_writable_inplace ())
    প্রত্যাগম সত্য;

  যদি (এতত্->mode == HB_MEMORY_MODE_WRITABLE)
    প্রত্যাগম সত্য;


  DEBUG_বার্তা_FUNC (BLOB, এতত্, "current data is -> %p\n", এতত্->data);

  বর্ণ *new_data;

  new_data = (বর্ণ *) hb_malloc (এতত্->length);
  যদি (unlikely (!new_data))
    প্রত্যাগম অসত্য;

  DEBUG_বার্তা_FUNC (BLOB, এতত্, "dupped successfully -> %p\n", এতত্->data);

  hb_memcpy (new_data, এতত্->data, এতত্->length);
  এতত্->destroy_user_data ();
  এতত্->mode = HB_MEMORY_MODE_WRITABLE;
  এতত্->data = new_data;
  এতত্->user_data = new_data;
  এতত্->destroy = hb_free;

  প্রত্যাগম সত্য;
}

/*
 * Mmap
 */

#ifndef HB_NO_OPEN
#ifdef HAVE_MMAP
# যদি !defined(HB_NO_RESOURCE_FORK) && defined(__APPLE__)
#  include <sys/paths.h>
# endif
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
#endif

#ifdef _WIN32
# include <windows.h>
#অন্যথা
# ifndef O_দ্বিমিক
#  define O_দ্বিমিক 0
# endif
#endif

#ifndef MAP_NORESERVE
# define MAP_NORESERVE 0
#endif

সংরচনা hb_mapped_file_t
{
  বর্ণ *contents;
  unsigned long length;
#ifdef _WIN32
  HANDLE mapping;
#endif
};

#যদি (defined(HAVE_MMAP) || defined(_WIN32)) && !defined(HB_NO_MMAP)
স্থির শূন্য
_hb_mapped_file_destroy (শূন্য *file_)
{
  hb_mapped_file_t *file = (hb_mapped_file_t *) file_;
#ifdef HAVE_MMAP
  munmap (file->contents, file->length);
#নোচেদ্যদি defined(_WIN32)
  UnmapViewOfFile (file->contents);
  CloseHandle (file->mapping);
#অন্যথা
  প্রতিজ্ঞা (0); // If we don't have mmap we shouldn't reach here
#endif

  hb_free (file);
}
#endif

#ifdef _PATH_RSRCFORKSPEC
স্থির পূর্ণাঙ্ক
_open_resource_fork (ধ্রুব বর্ণ *file_name, hb_mapped_file_t *file)
{
  size_t name_len = strlen (file_name);
  size_t len = name_len + পরিমাণ (_PATH_RSRCFORKSPEC);

  বর্ণ *rsrc_name = (বর্ণ *) hb_malloc (len);
  যদি (unlikely (!rsrc_name)) প্রত্যাগম -1;

  strncpy (rsrc_name, file_name, name_len);
  strncpy (rsrc_name + name_len, _PATH_RSRCFORKSPEC,
	   পরিমাণ (_PATH_RSRCFORKSPEC));

  পূর্ণাঙ্ক fd = open (rsrc_name, O_RDONLY | O_দ্বিমিক, 0);
  hb_free (rsrc_name);

  যদি (fd != -1)
  {
    সংরচনা stat st;
    যদি (fstat (fd, &st) != -1)
      file->length = (unsigned long) st.st_size;
    অন্যথা
    {
      close (fd);
      fd = -1;
    }
  }

  প্রত্যাগম fd;
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
hb_blob_create_from_file (ধ্রুব বর্ণ *file_name)
{
  hb_blob_t *blob = hb_blob_create_from_file_or_fail (file_name);
  প্রত্যাগম likely (blob) ? blob : hb_blob_get_empty ();
}

#যদি defined(HAVE_MMAP) && !defined(HB_NO_MMAP)
স্থির hb_blob_t *
_hb_blob_try_mmap (ধ্রুব বর্ণ *file_name)
{
  hb_mapped_file_t *file = (hb_mapped_file_t *) hb_calloc (1, পরিমাণ (hb_mapped_file_t));
  যদি (unlikely (!file)) প্রত্যাগম nullptr;
  auto file_guard = hb_make_scope_guard ([&]() { hb_free (file); });

  পূর্ণাঙ্ক fd = open (file_name, O_RDONLY | O_দ্বিমিক, 0);
  যদি (unlikely (fd == -1)) প্রত্যাগম nullptr;
  auto fd_guard = hb_make_scope_guard ([&]() { close (fd); });

  সংরচনা stat st;
  যদি (unlikely (fstat (fd, &st) == -1)) প্রত্যাগম nullptr;

  file->length = (unsigned long) st.st_size;

#ifdef _PATH_RSRCFORKSPEC
  যদি (unlikely (file->length == 0))
  {
    পূর্ণাঙ্ক rfd = _open_resource_fork (file_name, file);
    যদি (rfd != -1)
    {
      close (fd);
      fd = rfd;
    }
  }
#endif

  file->contents = (বর্ণ *) mmap (nullptr, file->length, PROT_READ,
				  MAP_PRIVATE | MAP_NORESERVE, fd, 0);
  যদি (unlikely (file->contents == MAP_FAILED)) প্রত্যাগম nullptr;

  file_guard.release ();
  /* fd_guard closes fd on return (ownership ends after mmap). */
  প্রত্যাগম hb_blob_create_or_fail (file->contents, file->length,
				 HB_MEMORY_MODE_READONLY_MAY_MAKE_WRITABLE, (শূন্য *) file,
				 (hb_destroy_func_t) _hb_mapped_file_destroy);
}
#নোচেদ্যদি defined(_WIN32) && !defined(HB_NO_MMAP)
স্থির hb_blob_t *
_hb_blob_try_mmap (ধ্রুব বর্ণ *file_name)
{
  hb_mapped_file_t *file = (hb_mapped_file_t *) hb_calloc (1, পরিমাণ (hb_mapped_file_t));
  যদি (unlikely (!file)) প্রত্যাগম nullptr;
  auto file_guard = hb_make_scope_guard ([&]() { hb_free (file); });

  size_t size = strlen (file_name) + 1;
  wchar_t *wchar_file_name = (wchar_t *) hb_malloc (পরিমাণ (wchar_t) * size);
  যদি (unlikely (!wchar_file_name)) প্রত্যাগম nullptr;

  /* Assume file name is given in UTF-8 encoding */
  পূর্ণাঙ্ক conversion = MultiByteToWideChar (CP_UTF8, MB_ERR_INVALID_CHARS, file_name, -1, wchar_file_name, size);
  যদি (conversion <= 0)
  {
    /* Conversion failed due to invalid UTF-8 characters,
       Repeat conversion based on system code page */
    mbstowcs (wchar_file_name, file_name, size);
  }

  HANDLE fd;
#যদি !WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP) && WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP)
  {
    CREATEFILE2_EXTENDED_PARAMETERS ceparams = { 0 };
    ceparams.dwSize = পরিমাণ(CREATEFILE2_EXTENDED_PARAMETERS);
    ceparams.dwFileAttributes = FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED & 0xFFFF;
    ceparams.dwFileFlags = FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED & 0xFFF00000;
    ceparams.dwSecurityQosFlags = FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED & 0x000F0000;
    ceparams.lpSecurityAttributes = nullptr;
    ceparams.hTemplateFile = nullptr;
    fd = CreateFile2 (wchar_file_name, GENERIC_READ, FILE_SHARE_READ,
		      OPEN_EXISTING, &ceparams);
  }
#অন্যথা
  fd = CreateFileW (wchar_file_name, GENERIC_READ, FILE_SHARE_READ, nullptr,
		    OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED,
		    nullptr);
#endif
  hb_free (wchar_file_name);

  যদি (unlikely (fd == INVALID_HANDLE_মান)) প্রত্যাগম nullptr;
  auto fd_guard = hb_make_scope_guard ([&]() { CloseHandle (fd); });

#যদি !WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP) && WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP)
  {
    LARGE_INTEGER length;
    GetFileSizeEx (fd, &length);
    file->length = length.নিচের_Part;
    file->mapping = CreateFileMappingFromApp (fd, nullptr, PAGE_READONLY, length.QuadPart, nullptr);
  }
#অন্যথা
  file->length = (unsigned long) GetFileSize (fd, nullptr);
  file->mapping = CreateFileMapping (fd, nullptr, PAGE_READONLY, 0, 0, nullptr);
#endif
  যদি (unlikely (!file->mapping)) প্রত্যাগম nullptr;

#যদি !WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP) && WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP)
  file->contents = (বর্ণ *) MapViewOfFileFromApp (file->mapping, FILE_MAP_READ, 0, 0);
#অন্যথা
  file->contents = (বর্ণ *) MapViewOfFile (file->mapping, FILE_MAP_READ, 0, 0, 0);
#endif
  যদি (unlikely (!file->contents)) প্রত্যাগম nullptr;

  file_guard.release ();
  /* fd_guard closes fd on return. */
  প্রত্যাগম hb_blob_create_or_fail (file->contents, file->length,
				 HB_MEMORY_MODE_READONLY_MAY_MAKE_WRITABLE, (শূন্য *) file,
				 (hb_destroy_func_t) _hb_mapped_file_destroy);
}
#endif

/* Read a file without knowing its size beforehand.  Used as a fallback
 * for systems without mmap or to read from pipes. */
স্থির hb_blob_t *
_hb_blob_read_file (ধ্রুব বর্ণ *file_name)
{
  unsigned long len = 0, allocated = BUFSIZ * 16;
  বর্ণ *data = (বর্ণ *) hb_malloc (allocated);
  যদি (unlikely (!data)) প্রত্যাগম nullptr;
  auto data_guard = hb_make_scope_guard ([&]() { hb_free (data); });

  FILE *fp = fopen (file_name, "rb");
  যদি (unlikely (!fp)) প্রত্যাগম nullptr;
  HB_SCOPE_GUARD (fclose (fp));

  যাবত্ (!feof (fp))
  {
    যদি (allocated - len < BUFSIZ)
    {
      allocated *= 2;
      /* Don't allocate and go more than ~536MB, our mmap reader still
	 can cover files like that but lets limit our fallback reader */
      যদি (unlikely (allocated > (2 << 28))) প্রত্যাগম nullptr;
      বর্ণ *new_data = (বর্ণ *) hb_realloc (data, allocated);
      যদি (unlikely (!new_data)) প্রত্যাগম nullptr;
      data = new_data;
    }

    unsigned long addition = fread (data + len, 1, allocated - len, fp);

    পূর্ণাঙ্ক err = ferror (fp);
#ifdef EINTR // armcc doesn't have it
    যদি (unlikely (err == EINTR)) অনুবর্ত;
#endif
    যদি (unlikely (err)) প্রত্যাগম nullptr;

    len += addition;
  }

  data_guard.release ();
  প্রত্যাগম hb_blob_create_or_fail (data, len, HB_MEMORY_MODE_WRITABLE, data,
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
hb_blob_create_from_file_or_fail (ধ্রুব বর্ণ *file_name)
{
  /* Adopted from glib's gmappedfile.c with Matthias Clasen and
     Allison Lortie permission but changed a lot to suit our need. */
#যদি (defined(HAVE_MMAP) || defined(_WIN32)) && !defined(HB_NO_MMAP)
  যদি (hb_blob_t *blob = _hb_blob_try_mmap (file_name))
    প্রত্যাগম blob;
#endif
  প্রত্যাগম _hb_blob_read_file (file_name);
}
#endif /* !HB_NO_OPEN */
