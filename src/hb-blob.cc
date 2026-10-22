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

#अन्तर्भाव "hb.hh"
#अन्तर्भाव "hb-blob.hh"

#ifdef HAVE_SYS_MMAN_H
#ifdef HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif /* HAVE_UNISTD_H */
#अन्तर्भाव <sys/mman.h>
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
hb_blob_create (अचर वर्ण        *data,
		अचिन्हित पूर्णाङ्क       length,
		hb_memory_mode_t   mode,
		शून्य              *user_data,
		hb_destroy_func_t  destroy)
{
  यदि (!length)
  {
    यदि (destroy)
      destroy (user_data);
    फर्काऊ hb_blob_get_empty ();
  }

  hb_blob_t *blob = hb_blob_create_or_fail (data, length, mode,
					    user_data, destroy);
  फर्काऊ likely (blob) ? blob : hb_blob_get_empty ();
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
hb_blob_create_or_fail (अचर वर्ण        *data,
			अचिन्हित पूर्णाङ्क       length,
			hb_memory_mode_t   mode,
			शून्य              *user_data,
			hb_destroy_func_t  destroy)
{
  hb_blob_t *blob;

  यदि (length >= 1u << 31 ||
      !(blob = hb_object_create<hb_blob_t> ()))
  {
    यदि (destroy)
      destroy (user_data);
    फर्काऊ nullptr;
  }

  blob->data = data;
  blob->length = length;
  blob->mode = mode;

  blob->user_data = user_data;
  blob->destroy = destroy;

  यदि (blob->mode == HB_MEMORY_MODE_DUPLICATE) {
    blob->mode = HB_MEMORY_MODE_READONLY;
    यदि (!blob->try_make_writable ())
    {
      hb_blob_destroy (blob);
      फर्काऊ nullptr;
    }
  }

  फर्काऊ blob;
}

स्थिर शून्य
_hb_blob_destroy (शून्य *data)
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
			 अचिन्हित पूर्णाङ्क  offset,
			 अचिन्हित पूर्णाङ्क  length)
{
  hb_blob_t *blob;

  यदि (!length || !parent || offset >= parent->length)
    फर्काऊ hb_blob_get_empty ();

  hb_blob_make_immutable (parent);

  blob = hb_blob_create (parent->data + offset,
			 hb_min (length, parent->length - offset),
			 HB_MEMORY_MODE_READONLY,
			 hb_blob_reference (parent),
			 _hb_blob_destroy);

  फर्काऊ blob;
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

  यदि (unlikely (blob == hb_blob_get_empty ()))
    blob = nullptr;

  फर्काऊ blob;
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
  फर्काऊ const_cast<hb_blob_t *> (&Null (hb_blob_t));
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
  फर्काऊ hb_object_reference (blob);
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
शून्य
hb_blob_destroy (hb_blob_t *blob)
{
  यदि (!hb_object_destroy (blob)) फर्काऊ;

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
		       hb_user_data_साँचो_t *key,
		       शून्य *              data,
		       hb_destroy_func_t   destroy,
		       hb_bool_t           replace)
{
  फर्काऊ hb_object_set_user_data (blob, key, data, destroy, replace);
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
शून्य *
hb_blob_get_user_data (अचर hb_blob_t    *blob,
		       hb_user_data_साँचो_t *key)
{
  फर्काऊ hb_object_get_user_data (blob, key);
}


/**
 * hb_blob_make_immutable:
 * @blob: a blob
 *
 * Makes a blob immutable.
 *
 * Since: 0.9.2
 **/
शून्य
hb_blob_make_immutable (hb_blob_t *blob)
{
  यदि (hb_object_is_immutable (blob))
    फर्काऊ;

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
  फर्काऊ hb_object_is_immutable (blob);
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
अचिन्हित पूर्णाङ्क
hb_blob_get_length (hb_blob_t *blob)
{
  फर्काऊ blob->length;
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
अचर वर्ण *
hb_blob_get_data (hb_blob_t *blob, अचिन्हित पूर्णाङ्क *length)
{
  यदि (length)
    *length = blob->length;

  फर्काऊ blob->data;
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
वर्ण *
hb_blob_get_data_writable (hb_blob_t *blob, अचिन्हित पूर्णाङ्क *length)
{
  यदि (hb_object_is_immutable (blob) ||
     !blob->try_make_writable ())
  {
    यदि (length) *length = 0;
    फर्काऊ nullptr;
  }

  यदि (length) *length = blob->length;
  फर्काऊ const_cast<वर्ण *> (blob->data);
}


तार्किक
hb_blob_t::try_make_writable_inplace_unix ()
{
#यदि defined(HAVE_SYS_MMAN_H) && defined(HAVE_MPROTECT)
  uintptr_t pagesize = -1, mask, length;
  अचर वर्ण *addr;

#यदि defined(HAVE_SYSCONF) && defined(_SC_PAGE_SIZE)
  pagesize = (uintptr_t) sysconf (_SC_PAGE_SIZE);
#अथवायदि defined(HAVE_SYSCONF) && defined(_SC_PAGESIZE)
  pagesize = (uintptr_t) sysconf (_SC_PAGESIZE);
#अथवायदि defined(HAVE_GETPAGESIZE)
  pagesize = (uintptr_t) getpagesize ();
#endif

  यदि ((uintptr_t) -1L == pagesize) {
    DEBUG_सन्देश_FUNC (BLOB, एतत्, "failed to get pagesize: %s", म_त्रुटि (त्रुटि_सं));
    फर्काऊ असत्य;
  }
  DEBUG_सन्देश_FUNC (BLOB, एतत्, "pagesize is %lu", (अचिन्हित दीर्घ) pagesize);

  mask = ~(pagesize-1);
  addr = (अचर वर्ण *) (((uintptr_t) एतत्->data) & mask);
  length = (अचर वर्ण *) (((uintptr_t) एतत्->data + एतत्->length + pagesize-1) & mask)  - addr;
  DEBUG_सन्देश_FUNC (BLOB, एतत्,
		  "calling mprotect on [%p..%p] (%lu bytes)",
		  addr, addr+length, (अचिन्हित दीर्घ) length);
  यदि (-1 == mprotect ((शून्य *) addr, length, PROT_READ | PROT_WRITE)) {
    DEBUG_सन्देश_FUNC (BLOB, एतत्, "mprotect failed: %s", म_त्रुटि (त्रुटि_सं));
    फर्काऊ असत्य;
  }

  एतत्->mode = HB_MEMORY_MODE_WRITABLE;

  DEBUG_सन्देश_FUNC (BLOB, एतत्,
		  "successfully made [%p..%p] (%lu bytes) writable\n",
		  addr, addr+length, (अचिन्हित दीर्घ) length);
  फर्काऊ सत्य;
#अन्यथा
  फर्काऊ असत्य;
#endif
}

तार्किक
hb_blob_t::try_make_writable_inplace ()
{
  DEBUG_सन्देश_FUNC (BLOB, एतत्, "making writable inplace\n");

  यदि (एतत्->try_make_writable_inplace_unix ())
    फर्काऊ सत्य;

  DEBUG_सन्देश_FUNC (BLOB, एतत्, "making writable -> FAILED\n");

  /* Failed to make writable inplace, mark that */
  एतत्->mode = HB_MEMORY_MODE_READONLY;
  फर्काऊ असत्य;
}

तार्किक
hb_blob_t::try_make_writable ()
{
  यदि (unlikely (!length))
    mode = HB_MEMORY_MODE_WRITABLE;

  यदि (एतत्->mode == HB_MEMORY_MODE_WRITABLE)
    फर्काऊ सत्य;

  यदि (एतत्->mode == HB_MEMORY_MODE_READONLY_MAY_MAKE_WRITABLE && एतत्->try_make_writable_inplace ())
    फर्काऊ सत्य;

  यदि (एतत्->mode == HB_MEMORY_MODE_WRITABLE)
    फर्काऊ सत्य;


  DEBUG_सन्देश_FUNC (BLOB, एतत्, "current data is -> %p\n", एतत्->data);

  वर्ण *new_data;

  new_data = (वर्ण *) hb_malloc (एतत्->length);
  यदि (unlikely (!new_data))
    फर्काऊ असत्य;

  DEBUG_सन्देश_FUNC (BLOB, एतत्, "dupped successfully -> %p\n", एतत्->data);

  hb_memcpy (new_data, एतत्->data, एतत्->length);
  एतत्->destroy_user_data ();
  एतत्->mode = HB_MEMORY_MODE_WRITABLE;
  एतत्->data = new_data;
  एतत्->user_data = new_data;
  एतत्->destroy = hb_free;

  फर्काऊ सत्य;
}

/*
 * Mmap
 */

#ifndef HB_NO_OPEN
#ifdef HAVE_MMAP
# यदि !defined(HB_NO_RESOURCE_FORK) && defined(__APPLE__)
#  include <sys/paths.h>
# endif
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
#endif

#ifdef _WIN32
# include <windows.h>
#अन्यथा
# ifndef O_द्विआधारी
#  define O_द्विआधारी 0
# endif
#endif

#ifndef MAP_NORESERVE
# define MAP_NORESERVE 0
#endif

संरचना hb_mapped_file_t
{
  वर्ण *contents;
  अचिन्हित दीर्घ length;
#ifdef _WIN32
  HANDLE mapping;
#endif
};

#यदि (defined(HAVE_MMAP) || defined(_WIN32)) && !defined(HB_NO_MMAP)
स्थिर शून्य
_hb_mapped_file_destroy (शून्य *file_)
{
  hb_mapped_file_t *file = (hb_mapped_file_t *) file_;
#ifdef HAVE_MMAP
  munmap (file->contents, file->length);
#अथवायदि defined(_WIN32)
  UnmapViewOfFile (file->contents);
  CloseHandle (file->mapping);
#अन्यथा
  प्रतिज्ञा (0); // If we don't have mmap we shouldn't reach here
#endif

  hb_free (file);
}
#endif

#ifdef _PATH_RSRCFORKSPEC
स्थिर पूर्णाङ्क
_open_resource_fork (अचर वर्ण *file_name, hb_mapped_file_t *file)
{
  माप_प्रकार name_len = म_माप (file_name);
  माप_प्रकार len = name_len + परिमाण (_PATH_RSRCFORKSPEC);

  वर्ण *rsrc_name = (वर्ण *) hb_malloc (len);
  यदि (unlikely (!rsrc_name)) फर्काऊ -1;

  म_नकलन (rsrc_name, file_name, name_len);
  म_नकलन (rsrc_name + name_len, _PATH_RSRCFORKSPEC,
	   परिमाण (_PATH_RSRCFORKSPEC));

  पूर्णाङ्क fd = खोलो (rsrc_name, O_RDONLY | O_द्विआधारी, 0);
  hb_free (rsrc_name);

  यदि (fd != -1)
  {
    संरचना stat st;
    यदि (ख_स्थिति (fd, &st) != -1)
      file->length = (अचिन्हित दीर्घ) st.st_size;
    अन्यथा
    {
      बंद (fd);
      fd = -1;
    }
  }

  फर्काऊ fd;
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
hb_blob_create_from_file (अचर वर्ण *file_name)
{
  hb_blob_t *blob = hb_blob_create_from_file_or_fail (file_name);
  फर्काऊ likely (blob) ? blob : hb_blob_get_empty ();
}

#यदि defined(HAVE_MMAP) && !defined(HB_NO_MMAP)
स्थिर hb_blob_t *
_hb_blob_try_mmap (अचर वर्ण *file_name)
{
  hb_mapped_file_t *file = (hb_mapped_file_t *) hb_calloc (1, परिमाण (hb_mapped_file_t));
  यदि (unlikely (!file)) फर्काऊ nullptr;
  स्वतः file_guard = hb_make_scope_guard ([&]() { hb_free (file); });

  पूर्णाङ्क fd = खोलो (file_name, O_RDONLY | O_द्विआधारी, 0);
  यदि (unlikely (fd == -1)) फर्काऊ nullptr;
  स्वतः fd_guard = hb_make_scope_guard ([&]() { बंद (fd); });

  संरचना stat st;
  यदि (unlikely (ख_स्थिति (fd, &st) == -1)) फर्काऊ nullptr;

  file->length = (अचिन्हित दीर्घ) st.st_size;

#ifdef _PATH_RSRCFORKSPEC
  यदि (unlikely (file->length == 0))
  {
    पूर्णाङ्क rfd = _open_resource_fork (file_name, file);
    यदि (rfd != -1)
    {
      बंद (fd);
      fd = rfd;
    }
  }
#endif

  file->contents = (वर्ण *) mmap (nullptr, file->length, PROT_READ,
				  MAP_PRIVATE | MAP_NORESERVE, fd, 0);
  यदि (unlikely (file->contents == MAP_FAILED)) फर्काऊ nullptr;

  file_guard.release ();
  /* fd_guard closes fd on return (ownership ends after mmap). */
  फर्काऊ hb_blob_create_or_fail (file->contents, file->length,
				 HB_MEMORY_MODE_READONLY_MAY_MAKE_WRITABLE, (शून्य *) file,
				 (hb_destroy_func_t) _hb_mapped_file_destroy);
}
#अथवायदि defined(_WIN32) && !defined(HB_NO_MMAP)
स्थिर hb_blob_t *
_hb_blob_try_mmap (अचर वर्ण *file_name)
{
  hb_mapped_file_t *file = (hb_mapped_file_t *) hb_calloc (1, परिमाण (hb_mapped_file_t));
  यदि (unlikely (!file)) फर्काऊ nullptr;
  स्वतः file_guard = hb_make_scope_guard ([&]() { hb_free (file); });

  माप_प्रकार size = म_माप (file_name) + 1;
  ब_अक्षर_प्रकार *wchar_file_name = (ब_अक्षर_प्रकार *) hb_malloc (परिमाण (ब_अक्षर_प्रकार) * size);
  यदि (unlikely (!wchar_file_name)) फर्काऊ nullptr;

  /* Assume file name is given in UTF-8 encoding */
  पूर्णाङ्क conversion = MultiByteToWideChar (CP_UTF8, MB_ERR_INVALID_CHARS, file_name, -1, wchar_file_name, size);
  यदि (conversion <= 0)
  {
    /* Conversion failed due to invalid UTF-8 characters,
       Repeat conversion based on system code page */
    mbstowcs (wchar_file_name, file_name, size);
  }

  HANDLE fd;
#यदि !WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP) && WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP)
  {
    CREATEFILE2_EXTENDED_PARAMETERS ceparams = { 0 };
    ceparams.dwSize = परिमाण(CREATEFILE2_EXTENDED_PARAMETERS);
    ceparams.dwFileAttributes = FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED & 0xFFFF;
    ceparams.dwFileFlags = FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED & 0xFFF00000;
    ceparams.dwSecurityQosFlags = FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED & 0x000F0000;
    ceparams.lpSecurityAttributes = nullptr;
    ceparams.hTemplateFile = nullptr;
    fd = CreateFile2 (wchar_file_name, GENERIC_READ, FILE_SHARE_READ,
		      OPEN_EXISTING, &ceparams);
  }
#अन्यथा
  fd = CreateFileW (wchar_file_name, GENERIC_READ, FILE_SHARE_READ, nullptr,
		    OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED,
		    nullptr);
#endif
  hb_free (wchar_file_name);

  यदि (unlikely (fd == INVALID_HANDLE_मान)) फर्काऊ nullptr;
  स्वतः fd_guard = hb_make_scope_guard ([&]() { CloseHandle (fd); });

#यदि !WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP) && WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP)
  {
    LARGE_INTEGER length;
    GetFileSizeEx (fd, &length);
    file->length = length.तल्लो_Part;
    file->mapping = CreateFileMappingFromApp (fd, nullptr, PAGE_READONLY, length.QuadPart, nullptr);
  }
#अन्यथा
  file->length = (अचिन्हित दीर्घ) GetFileSize (fd, nullptr);
  file->mapping = CreateFileMapping (fd, nullptr, PAGE_READONLY, 0, 0, nullptr);
#endif
  यदि (unlikely (!file->mapping)) फर्काऊ nullptr;

#यदि !WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP) && WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP)
  file->contents = (वर्ण *) MapViewOfFileFromApp (file->mapping, FILE_MAP_READ, 0, 0);
#अन्यथा
  file->contents = (वर्ण *) MapViewOfFile (file->mapping, FILE_MAP_READ, 0, 0, 0);
#endif
  यदि (unlikely (!file->contents)) फर्काऊ nullptr;

  file_guard.release ();
  /* fd_guard closes fd on return. */
  फर्काऊ hb_blob_create_or_fail (file->contents, file->length,
				 HB_MEMORY_MODE_READONLY_MAY_MAKE_WRITABLE, (शून्य *) file,
				 (hb_destroy_func_t) _hb_mapped_file_destroy);
}
#endif

/* Read a file without knowing its size beforehand.  Used as a fallback
 * for systems without mmap or to read from pipes. */
स्थिर hb_blob_t *
_hb_blob_read_file (अचर वर्ण *file_name)
{
  अचिन्हित दीर्घ len = 0, allocated = बफ_मान * 16;
  वर्ण *data = (वर्ण *) hb_malloc (allocated);
  यदि (unlikely (!data)) फर्काऊ nullptr;
  स्वतः data_guard = hb_make_scope_guard ([&]() { hb_free (data); });

  खाता *fp = ख_खोलो (file_name, "rb");
  यदि (unlikely (!fp)) फर्काऊ nullptr;
  HB_SCOPE_GUARD (ख_बंद (fp));

  जबसम्म (!ख_पूर्ण (fp))
  {
    यदि (allocated - len < बफ_मान)
    {
      allocated *= 2;
      /* Don't allocate and go more than ~536MB, our mmap reader still
	 can cover files like that but lets limit our fallback reader */
      यदि (unlikely (allocated > (2 << 28))) फर्काऊ nullptr;
      वर्ण *new_data = (वर्ण *) hb_realloc (data, allocated);
      यदि (unlikely (!new_data)) फर्काऊ nullptr;
      data = new_data;
    }

    अचिन्हित दीर्घ addition = ख_पढ़ो (data + len, 1, allocated - len, fp);

    पूर्णाङ्क err = ख_त्रुटि (fp);
#ifdef EINTR // armcc doesn't have it
    यदि (unlikely (err == EINTR)) जारी;
#endif
    यदि (unlikely (err)) फर्काऊ nullptr;

    len += addition;
  }

  data_guard.release ();
  फर्काऊ hb_blob_create_or_fail (data, len, HB_MEMORY_MODE_WRITABLE, data,
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
hb_blob_create_from_file_or_fail (अचर वर्ण *file_name)
{
  /* Adopted from glib's gmappedfile.c with Matthias Clasen and
     Allison Lortie permission but changed a lot to suit our need. */
#यदि (defined(HAVE_MMAP) || defined(_WIN32)) && !defined(HB_NO_MMAP)
  यदि (hb_blob_t *blob = _hb_blob_try_mmap (file_name))
    फर्काऊ blob;
#endif
  फर्काऊ _hb_blob_read_file (file_name);
}
#endif /* !HB_NO_OPEN */
