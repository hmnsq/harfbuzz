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

#ಅನ್ತರ್ಭಾವ "hb.hh"
#ಅನ್ತರ್ಭಾವ "hb-blob.hh"

#ifdef HAVE_SYS_MMAN_H
#ifdef HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#endif /* HAVE_UNISTD_H */
#ಅನ್ತರ್ಭಾವ <sys/mman.h>
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
hb_blob_create (ಧ್ರುವ ವರ್ಣ        *data,
		unsigned ಪೂರ್ಣಾಙ್ಕ       length,
		hb_memory_mode_t   mode,
		ಶೂನ್ಯ              *user_data,
		hb_destroy_func_t  destroy)
{
  ಯದಿ (!length)
  {
    ಯದಿ (destroy)
      destroy (user_data);
    ಪ್ರತ್ಯಾಗಮ hb_blob_get_empty ();
  }

  hb_blob_t *blob = hb_blob_create_or_fail (data, length, mode,
					    user_data, destroy);
  ಪ್ರತ್ಯಾಗಮ likely (blob) ? blob : hb_blob_get_empty ();
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
hb_blob_create_or_fail (ಧ್ರುವ ವರ್ಣ        *data,
			unsigned ಪೂರ್ಣಾಙ್ಕ       length,
			hb_memory_mode_t   mode,
			ಶೂನ್ಯ              *user_data,
			hb_destroy_func_t  destroy)
{
  hb_blob_t *blob;

  ಯದಿ (length >= 1u << 31 ||
      !(blob = hb_object_create<hb_blob_t> ()))
  {
    ಯದಿ (destroy)
      destroy (user_data);
    ಪ್ರತ್ಯಾಗಮ nullptr;
  }

  blob->data = data;
  blob->length = length;
  blob->mode = mode;

  blob->user_data = user_data;
  blob->destroy = destroy;

  ಯದಿ (blob->mode == HB_MEMORY_MODE_DUPLICATE) {
    blob->mode = HB_MEMORY_MODE_READONLY;
    ಯದಿ (!blob->try_make_writable ())
    {
      hb_blob_destroy (blob);
      ಪ್ರತ್ಯಾಗಮ nullptr;
    }
  }

  ಪ್ರತ್ಯಾಗಮ blob;
}

ಸ್ಥಿರ ಶೂನ್ಯ
_hb_blob_destroy (ಶೂನ್ಯ *data)
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
			 unsigned ಪೂರ್ಣಾಙ್ಕ  offset,
			 unsigned ಪೂರ್ಣಾಙ್ಕ  length)
{
  hb_blob_t *blob;

  ಯದಿ (!length || !parent || offset >= parent->length)
    ಪ್ರತ್ಯಾಗಮ hb_blob_get_empty ();

  hb_blob_make_immutable (parent);

  blob = hb_blob_create (parent->data + offset,
			 hb_min (length, parent->length - offset),
			 HB_MEMORY_MODE_READONLY,
			 hb_blob_reference (parent),
			 _hb_blob_destroy);

  ಪ್ರತ್ಯಾಗಮ blob;
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

  ಯದಿ (unlikely (blob == hb_blob_get_empty ()))
    blob = nullptr;

  ಪ್ರತ್ಯಾಗಮ blob;
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
  ಪ್ರತ್ಯಾಗಮ const_cast<hb_blob_t *> (&Null (hb_blob_t));
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
  ಪ್ರತ್ಯಾಗಮ hb_object_reference (blob);
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
ಶೂನ್ಯ
hb_blob_destroy (hb_blob_t *blob)
{
  ಯದಿ (!hb_object_destroy (blob)) ಪ್ರತ್ಯಾಗಮ;

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
		       hb_user_data_ಕೀಲಿ_t *key,
		       ಶೂನ್ಯ *              data,
		       hb_destroy_func_t   destroy,
		       hb_bool_t           replace)
{
  ಪ್ರತ್ಯಾಗಮ hb_object_set_user_data (blob, key, data, destroy, replace);
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
ಶೂನ್ಯ *
hb_blob_get_user_data (ಧ್ರುವ hb_blob_t    *blob,
		       hb_user_data_ಕೀಲಿ_t *key)
{
  ಪ್ರತ್ಯಾಗಮ hb_object_get_user_data (blob, key);
}


/**
 * hb_blob_make_immutable:
 * @blob: a blob
 *
 * Makes a blob immutable.
 *
 * Since: 0.9.2
 **/
ಶೂನ್ಯ
hb_blob_make_immutable (hb_blob_t *blob)
{
  ಯದಿ (hb_object_is_immutable (blob))
    ಪ್ರತ್ಯಾಗಮ;

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
  ಪ್ರತ್ಯಾಗಮ hb_object_is_immutable (blob);
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
unsigned ಪೂರ್ಣಾಙ್ಕ
hb_blob_get_length (hb_blob_t *blob)
{
  ಪ್ರತ್ಯಾಗಮ blob->length;
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
ಧ್ರುವ ವರ್ಣ *
hb_blob_get_data (hb_blob_t *blob, unsigned ಪೂರ್ಣಾಙ್ಕ *length)
{
  ಯದಿ (length)
    *length = blob->length;

  ಪ್ರತ್ಯಾಗಮ blob->data;
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
ವರ್ಣ *
hb_blob_get_data_writable (hb_blob_t *blob, unsigned ಪೂರ್ಣಾಙ್ಕ *length)
{
  ಯದಿ (hb_object_is_immutable (blob) ||
     !blob->try_make_writable ())
  {
    ಯದಿ (length) *length = 0;
    ಪ್ರತ್ಯಾಗಮ nullptr;
  }

  ಯದಿ (length) *length = blob->length;
  ಪ್ರತ್ಯಾಗಮ const_cast<ವರ್ಣ *> (blob->data);
}


ತಾರ್ಕಿಕ
hb_blob_t::try_make_writable_inplace_unix ()
{
#ಯದಿ defined(HAVE_SYS_MMAN_H) && defined(HAVE_MPROTECT)
  uintptr_t pagesize = -1, mask, length;
  ಧ್ರುವ ವರ್ಣ *addr;

#ಯದಿ defined(HAVE_SYSCONF) && defined(_SC_PAGE_SIZE)
  pagesize = (uintptr_t) sysconf (_SC_PAGE_SIZE);
#ನೋಚೇದ್ಯದಿ defined(HAVE_SYSCONF) && defined(_SC_PAGESIZE)
  pagesize = (uintptr_t) sysconf (_SC_PAGESIZE);
#ನೋಚೇದ್ಯದಿ defined(HAVE_GETPAGESIZE)
  pagesize = (uintptr_t) getpagesize ();
#endif

  ಯದಿ ((uintptr_t) -1L == pagesize) {
    DEBUG_ಸಂದೇಶ_FUNC (BLOB, ಏತತ್, "failed to get pagesize: %s", strerror (errno));
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }
  DEBUG_ಸಂದೇಶ_FUNC (BLOB, ಏತತ್, "pagesize is %lu", (unsigned long) pagesize);

  mask = ~(pagesize-1);
  addr = (ಧ್ರುವ ವರ್ಣ *) (((uintptr_t) ಏತತ್->data) & mask);
  length = (ಧ್ರುವ ವರ್ಣ *) (((uintptr_t) ಏತತ್->data + ಏತತ್->length + pagesize-1) & mask)  - addr;
  DEBUG_ಸಂದೇಶ_FUNC (BLOB, ಏತತ್,
		  "calling mprotect on [%p..%p] (%lu bytes)",
		  addr, addr+length, (unsigned long) length);
  ಯದಿ (-1 == mprotect ((ಶೂನ್ಯ *) addr, length, PROT_READ | PROT_WRITE)) {
    DEBUG_ಸಂದೇಶ_FUNC (BLOB, ಏತತ್, "mprotect failed: %s", strerror (errno));
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }

  ಏತತ್->mode = HB_MEMORY_MODE_WRITABLE;

  DEBUG_ಸಂದೇಶ_FUNC (BLOB, ಏತತ್,
		  "successfully made [%p..%p] (%lu bytes) writable\n",
		  addr, addr+length, (unsigned long) length);
  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
#ಅನ್ಯಥಾ
  ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
#endif
}

ತಾರ್ಕಿಕ
hb_blob_t::try_make_writable_inplace ()
{
  DEBUG_ಸಂದೇಶ_FUNC (BLOB, ಏತತ್, "making writable inplace\n");

  ಯದಿ (ಏತತ್->try_make_writable_inplace_unix ())
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;

  DEBUG_ಸಂದೇಶ_FUNC (BLOB, ಏತತ್, "making writable -> FAILED\n");

  /* Failed to make writable inplace, mark that */
  ಏತತ್->mode = HB_MEMORY_MODE_READONLY;
  ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
}

ತಾರ್ಕಿಕ
hb_blob_t::try_make_writable ()
{
  ಯದಿ (unlikely (!length))
    mode = HB_MEMORY_MODE_WRITABLE;

  ಯದಿ (ಏತತ್->mode == HB_MEMORY_MODE_WRITABLE)
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;

  ಯದಿ (ಏತತ್->mode == HB_MEMORY_MODE_READONLY_MAY_MAKE_WRITABLE && ಏತತ್->try_make_writable_inplace ())
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;

  ಯದಿ (ಏತತ್->mode == HB_MEMORY_MODE_WRITABLE)
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;


  DEBUG_ಸಂದೇಶ_FUNC (BLOB, ಏತತ್, "current data is -> %p\n", ಏತತ್->data);

  ವರ್ಣ *new_data;

  new_data = (ವರ್ಣ *) hb_malloc (ಏತತ್->length);
  ಯದಿ (unlikely (!new_data))
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

  DEBUG_ಸಂದೇಶ_FUNC (BLOB, ಏತತ್, "dupped successfully -> %p\n", ಏತತ್->data);

  hb_memcpy (new_data, ಏತತ್->data, ಏತತ್->length);
  ಏತತ್->destroy_user_data ();
  ಏತತ್->mode = HB_MEMORY_MODE_WRITABLE;
  ಏತತ್->data = new_data;
  ಏತತ್->user_data = new_data;
  ಏತತ್->destroy = hb_free;

  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}

/*
 * Mmap
 */

#ifndef HB_NO_OPEN
#ifdef HAVE_MMAP
# ಯದಿ !defined(HB_NO_RESOURCE_FORK) && defined(__APPLE__)
#  include <sys/paths.h>
# endif
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
#endif

#ifdef _WIN32
# include <windows.h>
#ಅನ್ಯಥಾ
# ifndef O_ದ್ವಿಮಾನ
#  define O_ದ್ವಿಮಾನ 0
# endif
#endif

#ifndef MAP_NORESERVE
# define MAP_NORESERVE 0
#endif

ಸಂರಚನಾ hb_mapped_file_t
{
  ವರ್ಣ *contents;
  unsigned long length;
#ifdef _WIN32
  HANDLE mapping;
#endif
};

#ಯದಿ (defined(HAVE_MMAP) || defined(_WIN32)) && !defined(HB_NO_MMAP)
ಸ್ಥಿರ ಶೂನ್ಯ
_hb_mapped_file_destroy (ಶೂನ್ಯ *file_)
{
  hb_mapped_file_t *file = (hb_mapped_file_t *) file_;
#ifdef HAVE_MMAP
  munmap (file->contents, file->length);
#ನೋಚೇದ್ಯದಿ defined(_WIN32)
  UnmapViewOfFile (file->contents);
  CloseHandle (file->mapping);
#ಅನ್ಯಥಾ
  ಪ್ರತಿಜ್ಞಾ (0); // If we don't have mmap we shouldn't reach here
#endif

  hb_free (file);
}
#endif

#ifdef _PATH_RSRCFORKSPEC
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
_open_resource_fork (ಧ್ರುವ ವರ್ಣ *file_name, hb_mapped_file_t *file)
{
  size_t name_len = strlen (file_name);
  size_t len = name_len + ಪರಿಮಾಣ (_PATH_RSRCFORKSPEC);

  ವರ್ಣ *rsrc_name = (ವರ್ಣ *) hb_malloc (len);
  ಯದಿ (unlikely (!rsrc_name)) ಪ್ರತ್ಯಾಗಮ -1;

  strncpy (rsrc_name, file_name, name_len);
  strncpy (rsrc_name + name_len, _PATH_RSRCFORKSPEC,
	   ಪರಿಮಾಣ (_PATH_RSRCFORKSPEC));

  ಪೂರ್ಣಾಙ್ಕ fd = open (rsrc_name, O_RDONLY | O_ದ್ವಿಮಾನ, 0);
  hb_free (rsrc_name);

  ಯದಿ (fd != -1)
  {
    ಸಂರಚನಾ stat st;
    ಯದಿ (fstat (fd, &st) != -1)
      file->length = (unsigned long) st.st_size;
    ಅನ್ಯಥಾ
    {
      close (fd);
      fd = -1;
    }
  }

  ಪ್ರತ್ಯಾಗಮ fd;
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
hb_blob_create_from_file (ಧ್ರುವ ವರ್ಣ *file_name)
{
  hb_blob_t *blob = hb_blob_create_from_file_or_fail (file_name);
  ಪ್ರತ್ಯಾಗಮ likely (blob) ? blob : hb_blob_get_empty ();
}

#ಯದಿ defined(HAVE_MMAP) && !defined(HB_NO_MMAP)
ಸ್ಥಿರ hb_blob_t *
_hb_blob_try_mmap (ಧ್ರುವ ವರ್ಣ *file_name)
{
  hb_mapped_file_t *file = (hb_mapped_file_t *) hb_calloc (1, ಪರಿಮಾಣ (hb_mapped_file_t));
  ಯದಿ (unlikely (!file)) ಪ್ರತ್ಯಾಗಮ nullptr;
  auto file_guard = hb_make_scope_guard ([&]() { hb_free (file); });

  ಪೂರ್ಣಾಙ್ಕ fd = open (file_name, O_RDONLY | O_ದ್ವಿಮಾನ, 0);
  ಯದಿ (unlikely (fd == -1)) ಪ್ರತ್ಯಾಗಮ nullptr;
  auto fd_guard = hb_make_scope_guard ([&]() { close (fd); });

  ಸಂರಚನಾ stat st;
  ಯದಿ (unlikely (fstat (fd, &st) == -1)) ಪ್ರತ್ಯಾಗಮ nullptr;

  file->length = (unsigned long) st.st_size;

#ifdef _PATH_RSRCFORKSPEC
  ಯದಿ (unlikely (file->length == 0))
  {
    ಪೂರ್ಣಾಙ್ಕ rfd = _open_resource_fork (file_name, file);
    ಯದಿ (rfd != -1)
    {
      close (fd);
      fd = rfd;
    }
  }
#endif

  file->contents = (ವರ್ಣ *) mmap (nullptr, file->length, PROT_READ,
				  MAP_PRIVATE | MAP_NORESERVE, fd, 0);
  ಯದಿ (unlikely (file->contents == MAP_FAILED)) ಪ್ರತ್ಯಾಗಮ nullptr;

  file_guard.release ();
  /* fd_guard closes fd on return (ownership ends after mmap). */
  ಪ್ರತ್ಯಾಗಮ hb_blob_create_or_fail (file->contents, file->length,
				 HB_MEMORY_MODE_READONLY_MAY_MAKE_WRITABLE, (ಶೂನ್ಯ *) file,
				 (hb_destroy_func_t) _hb_mapped_file_destroy);
}
#ನೋಚೇದ್ಯದಿ defined(_WIN32) && !defined(HB_NO_MMAP)
ಸ್ಥಿರ hb_blob_t *
_hb_blob_try_mmap (ಧ್ರುವ ವರ್ಣ *file_name)
{
  hb_mapped_file_t *file = (hb_mapped_file_t *) hb_calloc (1, ಪರಿಮಾಣ (hb_mapped_file_t));
  ಯದಿ (unlikely (!file)) ಪ್ರತ್ಯಾಗಮ nullptr;
  auto file_guard = hb_make_scope_guard ([&]() { hb_free (file); });

  size_t size = strlen (file_name) + 1;
  wchar_t *wchar_file_name = (wchar_t *) hb_malloc (ಪರಿಮಾಣ (wchar_t) * size);
  ಯದಿ (unlikely (!wchar_file_name)) ಪ್ರತ್ಯಾಗಮ nullptr;

  /* Assume file name is given in UTF-8 encoding */
  ಪೂರ್ಣಾಙ್ಕ conversion = MultiByteToWideChar (CP_UTF8, MB_ERR_INVALID_CHARS, file_name, -1, wchar_file_name, size);
  ಯದಿ (conversion <= 0)
  {
    /* Conversion failed due to invalid UTF-8 characters,
       Repeat conversion based on system code page */
    mbstowcs (wchar_file_name, file_name, size);
  }

  HANDLE fd;
#ಯದಿ !WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP) && WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP)
  {
    CREATEFILE2_EXTENDED_PARAMETERS ceparams = { 0 };
    ceparams.dwSize = ಪರಿಮಾಣ(CREATEFILE2_EXTENDED_PARAMETERS);
    ceparams.dwFileAttributes = FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED & 0xFFFF;
    ceparams.dwFileFlags = FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED & 0xFFF00000;
    ceparams.dwSecurityQosFlags = FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED & 0x000F0000;
    ceparams.lpSecurityAttributes = nullptr;
    ceparams.hTemplateFile = nullptr;
    fd = CreateFile2 (wchar_file_name, GENERIC_READ, FILE_SHARE_READ,
		      OPEN_EXISTING, &ceparams);
  }
#ಅನ್ಯಥಾ
  fd = CreateFileW (wchar_file_name, GENERIC_READ, FILE_SHARE_READ, nullptr,
		    OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED,
		    nullptr);
#endif
  hb_free (wchar_file_name);

  ಯದಿ (unlikely (fd == INVALID_HANDLE_ಮೌಲ್ಯ)) ಪ್ರತ್ಯಾಗಮ nullptr;
  auto fd_guard = hb_make_scope_guard ([&]() { CloseHandle (fd); });

#ಯದಿ !WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP) && WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP)
  {
    LARGE_INTEGER length;
    GetFileSizeEx (fd, &length);
    file->length = length.ಕೆಳಗಿನ_Part;
    file->mapping = CreateFileMappingFromApp (fd, nullptr, PAGE_READONLY, length.QuadPart, nullptr);
  }
#ಅನ್ಯಥಾ
  file->length = (unsigned long) GetFileSize (fd, nullptr);
  file->mapping = CreateFileMapping (fd, nullptr, PAGE_READONLY, 0, 0, nullptr);
#endif
  ಯದಿ (unlikely (!file->mapping)) ಪ್ರತ್ಯಾಗಮ nullptr;

#ಯದಿ !WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP) && WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP)
  file->contents = (ವರ್ಣ *) MapViewOfFileFromApp (file->mapping, FILE_MAP_READ, 0, 0);
#ಅನ್ಯಥಾ
  file->contents = (ವರ್ಣ *) MapViewOfFile (file->mapping, FILE_MAP_READ, 0, 0, 0);
#endif
  ಯದಿ (unlikely (!file->contents)) ಪ್ರತ್ಯಾಗಮ nullptr;

  file_guard.release ();
  /* fd_guard closes fd on return. */
  ಪ್ರತ್ಯಾಗಮ hb_blob_create_or_fail (file->contents, file->length,
				 HB_MEMORY_MODE_READONLY_MAY_MAKE_WRITABLE, (ಶೂನ್ಯ *) file,
				 (hb_destroy_func_t) _hb_mapped_file_destroy);
}
#endif

/* Read a file without knowing its size beforehand.  Used as a fallback
 * for systems without mmap or to read from pipes. */
ಸ್ಥಿರ hb_blob_t *
_hb_blob_read_file (ಧ್ರುವ ವರ್ಣ *file_name)
{
  unsigned long len = 0, allocated = BUFSIZ * 16;
  ವರ್ಣ *data = (ವರ್ಣ *) hb_malloc (allocated);
  ಯದಿ (unlikely (!data)) ಪ್ರತ್ಯಾಗಮ nullptr;
  auto data_guard = hb_make_scope_guard ([&]() { hb_free (data); });

  FILE *fp = fopen (file_name, "rb");
  ಯದಿ (unlikely (!fp)) ಪ್ರತ್ಯಾಗಮ nullptr;
  HB_SCOPE_GUARD (fclose (fp));

  ಯಾವತ್ (!feof (fp))
  {
    ಯದಿ (allocated - len < BUFSIZ)
    {
      allocated *= 2;
      /* Don't allocate and go more than ~536MB, our mmap reader still
	 can cover files like that but lets limit our fallback reader */
      ಯದಿ (unlikely (allocated > (2 << 28))) ಪ್ರತ್ಯಾಗಮ nullptr;
      ವರ್ಣ *new_data = (ವರ್ಣ *) hb_realloc (data, allocated);
      ಯದಿ (unlikely (!new_data)) ಪ್ರತ್ಯಾಗಮ nullptr;
      data = new_data;
    }

    unsigned long addition = fread (data + len, 1, allocated - len, fp);

    ಪೂರ್ಣಾಙ್ಕ err = ferror (fp);
#ifdef EINTR // armcc doesn't have it
    ಯದಿ (unlikely (err == EINTR)) ಅನುವರ್ತ;
#endif
    ಯದಿ (unlikely (err)) ಪ್ರತ್ಯಾಗಮ nullptr;

    len += addition;
  }

  data_guard.release ();
  ಪ್ರತ್ಯಾಗಮ hb_blob_create_or_fail (data, len, HB_MEMORY_MODE_WRITABLE, data,
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
hb_blob_create_from_file_or_fail (ಧ್ರುವ ವರ್ಣ *file_name)
{
  /* Adopted from glib's gmappedfile.c with Matthias Clasen and
     Allison Lortie permission but changed a lot to suit our need. */
#ಯದಿ (defined(HAVE_MMAP) || defined(_WIN32)) && !defined(HB_NO_MMAP)
  ಯದಿ (hb_blob_t *blob = _hb_blob_try_mmap (file_name))
    ಪ್ರತ್ಯಾಗಮ blob;
#endif
  ಪ್ರತ್ಯಾಗಮ _hb_blob_read_file (file_name);
}
#endif /* !HB_NO_OPEN */
