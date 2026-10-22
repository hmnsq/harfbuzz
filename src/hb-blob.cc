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

#समावेश "hb.hh"
#समावेश "hb-blob.hh"

#अगर_घोषित HAVE_SYS_MMAN_H
#अगर_घोषित HAVE_UNISTD_H
#समावेश <unistd.h>
#पूर्ण_अगर /* HAVE_UNISTD_H */
#समावेश <sys/mman.h>
#पूर्ण_अगर /* HAVE_SYS_MMAN_H */


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
hb_blob_create (स्थिर अक्षर        *तथ्य,
		अचिन्हित पूर्णांक       लंबाई,
		hb_memory_mode_t   mode,
		व्योम              *user_तथ्य,
		hb_destroy_func_t  destroy)
{
  अगर (!लंबाई)
  {
    अगर (destroy)
      destroy (user_तथ्य);
    वापस hb_blob_get_empty ();
  }

  hb_blob_t *blob = hb_blob_create_or_fail (तथ्य, लंबाई, mode,
					    user_तथ्य, destroy);
  वापस likely (blob) ? blob : hb_blob_get_empty ();
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
hb_blob_create_or_fail (स्थिर अक्षर        *तथ्य,
			अचिन्हित पूर्णांक       लंबाई,
			hb_memory_mode_t   mode,
			व्योम              *user_तथ्य,
			hb_destroy_func_t  destroy)
{
  hb_blob_t *blob;

  अगर (लंबाई >= 1u << 31 ||
      !(blob = hb_object_create<hb_blob_t> ()))
  {
    अगर (destroy)
      destroy (user_तथ्य);
    वापस nullptr;
  }

  blob->तथ्य = तथ्य;
  blob->लंबाई = लंबाई;
  blob->mode = mode;

  blob->user_तथ्य = user_तथ्य;
  blob->destroy = destroy;

  अगर (blob->mode == HB_MEMORY_MODE_DUPLICATE) {
    blob->mode = HB_MEMORY_MODE_READONLY;
    अगर (!blob->try_make_writable ())
    {
      hb_blob_destroy (blob);
      वापस nullptr;
    }
  }

  वापस blob;
}

अटल व्योम
_hb_blob_destroy (व्योम *तथ्य)
{
  hb_blob_destroy ((hb_blob_t *) तथ्य);
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
			 अचिन्हित पूर्णांक  offset,
			 अचिन्हित पूर्णांक  लंबाई)
{
  hb_blob_t *blob;

  अगर (!लंबाई || !parent || offset >= parent->लंबाई)
    वापस hb_blob_get_empty ();

  hb_blob_make_immutable (parent);

  blob = hb_blob_create (parent->तथ्य + offset,
			 hb_min (लंबाई, parent->लंबाई - offset),
			 HB_MEMORY_MODE_READONLY,
			 hb_blob_reference (parent),
			 _hb_blob_destroy);

  वापस blob;
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
  blob = hb_blob_create (blob->तथ्य,
			 blob->लंबाई,
			 HB_MEMORY_MODE_DUPLICATE,
			 nullptr,
			 nullptr);

  अगर (unlikely (blob == hb_blob_get_empty ()))
    blob = nullptr;

  वापस blob;
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
  वापस const_cast<hb_blob_t *> (&Null (hb_blob_t));
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
  वापस hb_object_reference (blob);
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
व्योम
hb_blob_destroy (hb_blob_t *blob)
{
  अगर (!hb_object_destroy (blob)) वापस;

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
hb_blob_set_user_तथ्य (hb_blob_t          *blob,
		       hb_user_तथ्य_कुंजी_t *key,
		       व्योम *              तथ्य,
		       hb_destroy_func_t   destroy,
		       hb_bool_t           replace)
{
  वापस hb_object_set_user_तथ्य (blob, key, तथ्य, destroy, replace);
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
व्योम *
hb_blob_get_user_तथ्य (स्थिर hb_blob_t    *blob,
		       hb_user_तथ्य_कुंजी_t *key)
{
  वापस hb_object_get_user_तथ्य (blob, key);
}


/**
 * hb_blob_make_immutable:
 * @blob: a blob
 *
 * Makes a blob immutable.
 *
 * Since: 0.9.2
 **/
व्योम
hb_blob_make_immutable (hb_blob_t *blob)
{
  अगर (hb_object_is_immutable (blob))
    वापस;

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
  वापस hb_object_is_immutable (blob);
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
अचिन्हित पूर्णांक
hb_blob_get_लंबाई (hb_blob_t *blob)
{
  वापस blob->लंबाई;
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
स्थिर अक्षर *
hb_blob_get_तथ्य (hb_blob_t *blob, अचिन्हित पूर्णांक *लंबाई)
{
  अगर (लंबाई)
    *लंबाई = blob->लंबाई;

  वापस blob->तथ्य;
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
अक्षर *
hb_blob_get_तथ्य_writable (hb_blob_t *blob, अचिन्हित पूर्णांक *लंबाई)
{
  अगर (hb_object_is_immutable (blob) ||
     !blob->try_make_writable ())
  {
    अगर (लंबाई) *लंबाई = 0;
    वापस nullptr;
  }

  अगर (लंबाई) *लंबाई = blob->लंबाई;
  वापस const_cast<अक्षर *> (blob->तथ्य);
}


bool
hb_blob_t::try_make_writable_inplace_unix ()
{
#अगर defined(HAVE_SYS_MMAN_H) && defined(HAVE_MPROTECT)
  uintptr_t pagesize = -1, mask, लंबाई;
  स्थिर अक्षर *addr;

#अगर defined(HAVE_SYSCONF) && defined(SC_PAGE_आकार)
  pagesize = (uintptr_t) sysconf (SC_PAGE_आकार);
#या_अगर defined(HAVE_SYSCONF) && defined(_SC_PAGESIZE)
  pagesize = (uintptr_t) sysconf (_SC_PAGESIZE);
#या_अगर defined(HAVE_GETPAGESIZE)
  pagesize = (uintptr_t) getpagesize ();
#पूर्ण_अगर

  अगर ((uintptr_t) -1L == pagesize) {
    DEBUG_संदेश_FUNC (BLOB, यह, "failed to get pagesize: %s", म_त्रुटि (त्रुटि_सं));
    वापस false;
  }
  DEBUG_संदेश_FUNC (BLOB, यह, "pagesize is %lu", (अचिन्हित दीर्घ) pagesize);

  mask = ~(pagesize-1);
  addr = (स्थिर अक्षर *) (((uintptr_t) यह->तथ्य) & mask);
  लंबाई = (स्थिर अक्षर *) (((uintptr_t) यह->तथ्य + यह->लंबाई + pagesize-1) & mask)  - addr;
  DEBUG_संदेश_FUNC (BLOB, यह,
		  "calling mprotect on [%p..%p] (%lu bytes)",
		  addr, addr+लंबाई, (अचिन्हित दीर्घ) लंबाई);
  अगर (-1 == mprotect ((व्योम *) addr, लंबाई, PROT_READ | PROT_WRITE)) {
    DEBUG_संदेश_FUNC (BLOB, यह, "mprotect failed: %s", म_त्रुटि (त्रुटि_सं));
    वापस false;
  }

  यह->mode = HB_MEMORY_MODE_WRITABLE;

  DEBUG_संदेश_FUNC (BLOB, यह,
		  "successfully made [%p..%p] (%lu bytes) writable\n",
		  addr, addr+लंबाई, (अचिन्हित दीर्घ) लंबाई);
  वापस true;
#अन्यथा
  वापस false;
#पूर्ण_अगर
}

bool
hb_blob_t::try_make_writable_inplace ()
{
  DEBUG_संदेश_FUNC (BLOB, यह, "making writable inplace\n");

  अगर (यह->try_make_writable_inplace_unix ())
    वापस true;

  DEBUG_संदेश_FUNC (BLOB, यह, "making writable -> FAILED\n");

  /* Failed to make writable inplace, mark that */
  यह->mode = HB_MEMORY_MODE_READONLY;
  वापस false;
}

bool
hb_blob_t::try_make_writable ()
{
  अगर (unlikely (!लंबाई))
    mode = HB_MEMORY_MODE_WRITABLE;

  अगर (यह->mode == HB_MEMORY_MODE_WRITABLE)
    वापस true;

  अगर (यह->mode == HB_MEMORY_MODE_READONLY_MAY_MAKE_WRITABLE && यह->try_make_writable_inplace ())
    वापस true;

  अगर (यह->mode == HB_MEMORY_MODE_WRITABLE)
    वापस true;


  DEBUG_संदेश_FUNC (BLOB, यह, "current data is -> %p\n", यह->तथ्य);

  अक्षर *new_तथ्य;

  new_तथ्य = (अक्षर *) hb_malloc (यह->लंबाई);
  अगर (unlikely (!new_तथ्य))
    वापस false;

  DEBUG_संदेश_FUNC (BLOB, यह, "dupped successfully -> %p\n", यह->तथ्य);

  hb_memcpy (new_तथ्य, यह->तथ्य, यह->लंबाई);
  यह->destroy_user_तथ्य ();
  यह->mode = HB_MEMORY_MODE_WRITABLE;
  यह->तथ्य = new_तथ्य;
  यह->user_तथ्य = new_तथ्य;
  यह->destroy = hb_free;

  वापस true;
}

/*
 * Mmap
 */

#अगर_अघोषित HB_NO_OPEN
#अगर_घोषित HAVE_MMAP
# अगर !defined(HB_NO_RESOURCE_FORK) && defined(__APPLE__)
#  include <sys/paths.h>
# endif
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
#पूर्ण_अगर

#अगर_घोषित _WIN32
# include <windows.h>
#अन्यथा
# ifndef O_द्विआधारी
#  define O_द्विआधारी 0
# endif
#पूर्ण_अगर

#अगर_अघोषित MAP_NORESERVE
# define MAP_NORESERVE 0
#पूर्ण_अगर

काष्ठा hb_mapped_file_t
{
  अक्षर *contents;
  अचिन्हित दीर्घ लंबाई;
#अगर_घोषित _WIN32
  HANDLE mapping;
#पूर्ण_अगर
};

#अगर (defined(HAVE_MMAP) || defined(_WIN32)) && !defined(HB_NO_MMAP)
अटल व्योम
_hb_mapped_file_destroy (व्योम *file_)
{
  hb_mapped_file_t *file = (hb_mapped_file_t *) file_;
#अगर_घोषित HAVE_MMAP
  munmap (file->contents, file->लंबाई);
#या_अगर defined(_WIN32)
  UnmapViewOfFile (file->contents);
  CloseHandle (file->mapping);
#अन्यथा
  निश्चित (0); // If we don't have mmap we shouldn't reach here
#पूर्ण_अगर

  hb_free (file);
}
#पूर्ण_अगर

#अगर_घोषित _PATH_RSRCFORKSPEC
अटल पूर्णांक
_open_resource_fork (स्थिर अक्षर *file_name, hb_mapped_file_t *file)
{
  माप_प्रकार name_len = म_माप (file_name);
  माप_प्रकार len = name_len + माप (_PATH_RSRCFORKSPEC);

  अक्षर *rsrc_name = (अक्षर *) hb_malloc (len);
  अगर (unlikely (!rsrc_name)) वापस -1;

  म_नकलन (rsrc_name, file_name, name_len);
  म_नकलन (rsrc_name + name_len, _PATH_RSRCFORKSPEC,
	   माप (_PATH_RSRCFORKSPEC));

  पूर्णांक fd = खोलो (rsrc_name, O_RDONLY | O_द्विआधारी, 0);
  hb_free (rsrc_name);

  अगर (fd != -1)
  {
    काष्ठा stat st;
    अगर (ख_स्थिति (fd, &st) != -1)
      file->लंबाई = (अचिन्हित दीर्घ) st.st_आकार;
    अन्यथा
    {
      बंद (fd);
      fd = -1;
    }
  }

  वापस fd;
}
#पूर्ण_अगर

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
hb_blob_create_from_file (स्थिर अक्षर *file_name)
{
  hb_blob_t *blob = hb_blob_create_from_file_or_fail (file_name);
  वापस likely (blob) ? blob : hb_blob_get_empty ();
}

#अगर defined(HAVE_MMAP) && !defined(HB_NO_MMAP)
अटल hb_blob_t *
_hb_blob_try_mmap (स्थिर अक्षर *file_name)
{
  hb_mapped_file_t *file = (hb_mapped_file_t *) hb_calloc (1, माप (hb_mapped_file_t));
  अगर (unlikely (!file)) वापस nullptr;
  स्वतः file_guard = hb_make_scope_guard ([&]() { hb_free (file); });

  पूर्णांक fd = खोलो (file_name, O_RDONLY | O_द्विआधारी, 0);
  अगर (unlikely (fd == -1)) वापस nullptr;
  स्वतः fd_guard = hb_make_scope_guard ([&]() { बंद (fd); });

  काष्ठा stat st;
  अगर (unlikely (ख_स्थिति (fd, &st) == -1)) वापस nullptr;

  file->लंबाई = (अचिन्हित दीर्घ) st.st_आकार;

#अगर_घोषित _PATH_RSRCFORKSPEC
  अगर (unlikely (file->लंबाई == 0))
  {
    पूर्णांक rfd = _open_resource_fork (file_name, file);
    अगर (rfd != -1)
    {
      बंद (fd);
      fd = rfd;
    }
  }
#पूर्ण_अगर

  file->contents = (अक्षर *) mmap (nullptr, file->लंबाई, PROT_READ,
				  MAP_PRIVATE | MAP_NORESERVE, fd, 0);
  अगर (unlikely (file->contents == MAP_FAILED)) वापस nullptr;

  file_guard.release ();
  /* fd_guard closes fd on return (ownership ends after mmap). */
  वापस hb_blob_create_or_fail (file->contents, file->लंबाई,
				 HB_MEMORY_MODE_READONLY_MAY_MAKE_WRITABLE, (व्योम *) file,
				 (hb_destroy_func_t) _hb_mapped_file_destroy);
}
#या_अगर defined(_WIN32) && !defined(HB_NO_MMAP)
अटल hb_blob_t *
_hb_blob_try_mmap (स्थिर अक्षर *file_name)
{
  hb_mapped_file_t *file = (hb_mapped_file_t *) hb_calloc (1, माप (hb_mapped_file_t));
  अगर (unlikely (!file)) वापस nullptr;
  स्वतः file_guard = hb_make_scope_guard ([&]() { hb_free (file); });

  माप_प्रकार आकार = म_माप (file_name) + 1;
  ब_अक्षर_प्रकार *wchar_file_name = (ब_अक्षर_प्रकार *) hb_malloc (माप (ब_अक्षर_प्रकार) * आकार);
  अगर (unlikely (!wchar_file_name)) वापस nullptr;

  /* Assume file name is given in UTF-8 encoding */
  पूर्णांक conversion = MultiByteToWideChar (CP_UTF8, MB_ERR_INVALID_CHARS, file_name, -1, wchar_file_name, आकार);
  अगर (conversion <= 0)
  {
    /* Conversion failed due to invalid UTF-8 characters,
       Repeat conversion based on system code page */
    mbstowcs (wchar_file_name, file_name, आकार);
  }

  HANDLE fd;
#अगर !WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP) && WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP)
  {
    CREATEFILE2_EXTENDED_PARAMETERS ceparams = { 0 };
    ceparams.dw_आकार = माप(CREATEFILE2_EXTENDED_PARAMETERS);
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
#पूर्ण_अगर
  hb_free (wchar_file_name);

  अगर (unlikely (fd == INVALID_HANDLE_मूल्य)) वापस nullptr;
  स्वतः fd_guard = hb_make_scope_guard ([&]() { CloseHandle (fd); });

#अगर !WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP) && WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP)
  {
    LARGE_INTEGER लंबाई;
    Get_File_आकार_Ex (fd, &लंबाई);
    file->लंबाई = लंबाई.निचला_Part;
    file->mapping = CreateFileMappingFromApp (fd, nullptr, PAGE_READONLY, लंबाई.QuadPart, nullptr);
  }
#अन्यथा
  file->लंबाई = (अचिन्हित दीर्घ) Get_File_आकार (fd, nullptr);
  file->mapping = CreateFileMapping (fd, nullptr, PAGE_READONLY, 0, 0, nullptr);
#पूर्ण_अगर
  अगर (unlikely (!file->mapping)) वापस nullptr;

#अगर !WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP) && WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP)
  file->contents = (अक्षर *) MapViewOfFileFromApp (file->mapping, FILE_MAP_READ, 0, 0);
#अन्यथा
  file->contents = (अक्षर *) MapViewOfFile (file->mapping, FILE_MAP_READ, 0, 0, 0);
#पूर्ण_अगर
  अगर (unlikely (!file->contents)) वापस nullptr;

  file_guard.release ();
  /* fd_guard closes fd on return. */
  वापस hb_blob_create_or_fail (file->contents, file->लंबाई,
				 HB_MEMORY_MODE_READONLY_MAY_MAKE_WRITABLE, (व्योम *) file,
				 (hb_destroy_func_t) _hb_mapped_file_destroy);
}
#पूर्ण_अगर

/* Read a file without knowing its size beforehand.  Used as a fallback
 * for systems without mmap or to read from pipes. */
अटल hb_blob_t *
_hb_blob_read_file (स्थिर अक्षर *file_name)
{
  अचिन्हित दीर्घ len = 0, allocated = बफ_मान * 16;
  अक्षर *तथ्य = (अक्षर *) hb_malloc (allocated);
  अगर (unlikely (!तथ्य)) वापस nullptr;
  स्वतः तथ्य_guard = hb_make_scope_guard ([&]() { hb_free (तथ्य); });

  खाता *fp = ख_खोलो (file_name, "rb");
  अगर (unlikely (!fp)) वापस nullptr;
  HB_SCOPE_GUARD (ख_बंद (fp));

  जबतक (!ख_पूर्ण (fp))
  {
    अगर (allocated - len < बफ_मान)
    {
      allocated *= 2;
      /* Don't allocate and go more than ~536MB, our mmap reader still
	 can cover files like that but lets limit our fallback reader */
      अगर (unlikely (allocated > (2 << 28))) वापस nullptr;
      अक्षर *new_तथ्य = (अक्षर *) hb_realloc (तथ्य, allocated);
      अगर (unlikely (!new_तथ्य)) वापस nullptr;
      तथ्य = new_तथ्य;
    }

    अचिन्हित दीर्घ addition = ख_पढ़ो (तथ्य + len, 1, allocated - len, fp);

    पूर्णांक err = ख_त्रुटि (fp);
#अगर_घोषित EINTR // armcc doesn't have it
    अगर (unlikely (err == EINTR)) जारी;
#पूर्ण_अगर
    अगर (unlikely (err)) वापस nullptr;

    len += addition;
  }

  तथ्य_guard.release ();
  वापस hb_blob_create_or_fail (तथ्य, len, HB_MEMORY_MODE_WRITABLE, तथ्य,
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
hb_blob_create_from_file_or_fail (स्थिर अक्षर *file_name)
{
  /* Adopted from glib's gmappedfile.c with Matthias Clasen and
     Allison Lortie permission but changed a lot to suit our need. */
#अगर (defined(HAVE_MMAP) || defined(_WIN32)) && !defined(HB_NO_MMAP)
  अगर (hb_blob_t *blob = _hb_blob_try_mmap (file_name))
    वापस blob;
#पूर्ण_अगर
  वापस _hb_blob_read_file (file_name);
}
#पूर्ण_अगर /* !HB_NO_OPEN */
