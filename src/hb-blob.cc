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

#అన్తర్భావ "hb.hh"
#అన్తర్భావ "hb-blob.hh"

#నిర్వచితమైతే HAVE_SYS_MMAN_H
#నిర్వచితమైతే HAVE_UNISTD_H
#అన్తర్భావ <unistd.h>
#అయితే_ముగింపు /* HAVE_UNISTD_H */
#అన్తర్భావ <sys/mman.h>
#అయితే_ముగింపు /* HAVE_SYS_MMAN_H */


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
hb_blob_create (స్థిరరాశి అక్షరం        *దత్తాంశం,
		గుర్తులేని పూర్ణాంకం       నిడివి,
		hb_జ్ఞాపకం_బహుళకం_t   బహుళకం,
		శూన్య              *user_దత్తాంశం,
		hb_destroy_func_t  destroy)
{
  ఒకవేళ (!నిడివి)
  {
    ఒకవేళ (destroy)
      destroy (user_దత్తాంశం);
    తిరిగి hb_blob_get_ఖాళీలు ();
  }

  hb_blob_t *blob = hb_blob_create_or_fail (దత్తాంశం, నిడివి, బహుళకం,
					    user_దత్తాంశం, destroy);
  తిరిగి likely (blob) ? blob : hb_blob_get_ఖాళీలు ();
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
hb_blob_create_or_fail (స్థిరరాశి అక్షరం        *దత్తాంశం,
			గుర్తులేని పూర్ణాంకం       నిడివి,
			hb_జ్ఞాపకం_బహుళకం_t   బహుళకం,
			శూన్య              *user_దత్తాంశం,
			hb_destroy_func_t  destroy)
{
  hb_blob_t *blob;

  ఒకవేళ (నిడివి >= 1u << 31 ||
      !(blob = hb_object_create<hb_blob_t> ()))
  {
    ఒకవేళ (destroy)
      destroy (user_దత్తాంశం);
    తిరిగి శూన్యసూచిక;
  }

  blob->దత్తాంశం = దత్తాంశం;
  blob->నిడివి = నిడివి;
  blob->బహుళకం = బహుళకం;

  blob->user_దత్తాంశం = user_దత్తాంశం;
  blob->destroy = destroy;

  ఒకవేళ (blob->బహుళకం == HB_జ్ఞాపకం_బహుళకం_DUPLICATE) {
    blob->బహుళకం = HB_జ్ఞాపకం_బహుళకం_READONLY;
    ఒకవేళ (!blob->try_make_writable ())
    {
      hb_blob_destroy (blob);
      తిరిగి శూన్యసూచిక;
    }
  }

  తిరిగి blob;
}

స్థిర శూన్య
_hb_blob_destroy (శూన్య *దత్తాంశం)
{
  hb_blob_destroy ((hb_blob_t *) దత్తాంశం);
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
hb_blob_create_sub_blob (hb_blob_t    *మాతృ,
			 గుర్తులేని పూర్ణాంకం  స్థానభ్రంశం,
			 గుర్తులేని పూర్ణాంకం  నిడివి)
{
  hb_blob_t *blob;

  ఒకవేళ (!నిడివి || !మాతృ || స్థానభ్రంశం >= మాతృ->నిడివి)
    తిరిగి hb_blob_get_ఖాళీలు ();

  hb_blob_make_immutable (మాతృ);

  blob = hb_blob_create (మాతృ->దత్తాంశం + స్థానభ్రంశం,
			 hb_min (నిడివి, మాతృ->నిడివి - స్థానభ్రంశం),
			 HB_జ్ఞాపకం_బహుళకం_READONLY,
			 hb_blob_reference (మాతృ),
			 _hb_blob_destroy);

  తిరిగి blob;
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
  blob = hb_blob_create (blob->దత్తాంశం,
			 blob->నిడివి,
			 HB_జ్ఞాపకం_బహుళకం_DUPLICATE,
			 శూన్యసూచిక,
			 శూన్యసూచిక);

  ఒకవేళ (unlikely (blob == hb_blob_get_ఖాళీలు ()))
    blob = శూన్యసూచిక;

  తిరిగి blob;
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
hb_blob_get_ఖాళీలు ()
{
  తిరిగి const_cast<hb_blob_t *> (&Null (hb_blob_t));
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
  తిరిగి hb_object_reference (blob);
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
శూన్య
hb_blob_destroy (hb_blob_t *blob)
{
  ఒకవేళ (!hb_object_destroy (blob)) తిరిగి;

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
hb_blob_సమితి_user_దత్తాంశం (hb_blob_t          *blob,
		       hb_user_దత్తాంశం_కీలకం_t *key,
		       శూన్య *              దత్తాంశం,
		       hb_destroy_func_t   destroy,
		       hb_bool_t           replace)
{
  తిరిగి hb_object_సమితి_user_దత్తాంశం (blob, key, దత్తాంశం, destroy, replace);
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
శూన్య *
hb_blob_get_user_దత్తాంశం (స్థిరరాశి hb_blob_t    *blob,
		       hb_user_దత్తాంశం_కీలకం_t *key)
{
  తిరిగి hb_object_get_user_దత్తాంశం (blob, key);
}


/**
 * hb_blob_make_immutable:
 * @blob: a blob
 *
 * Makes a blob immutable.
 *
 * Since: 0.9.2
 **/
శూన్య
hb_blob_make_immutable (hb_blob_t *blob)
{
  ఒకవేళ (hb_object_is_immutable (blob))
    తిరిగి;

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
  తిరిగి hb_object_is_immutable (blob);
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
గుర్తులేని పూర్ణాంకం
hb_blob_get_నిడివి (hb_blob_t *blob)
{
  తిరిగి blob->నిడివి;
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
స్థిరరాశి అక్షరం *
hb_blob_get_దత్తాంశం (hb_blob_t *blob, గుర్తులేని పూర్ణాంకం *నిడివి)
{
  ఒకవేళ (నిడివి)
    *నిడివి = blob->నిడివి;

  తిరిగి blob->దత్తాంశం;
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
అక్షరం *
hb_blob_get_దత్తాంశం_writable (hb_blob_t *blob, గుర్తులేని పూర్ణాంకం *నిడివి)
{
  ఒకవేళ (hb_object_is_immutable (blob) ||
     !blob->try_make_writable ())
  {
    ఒకవేళ (నిడివి) *నిడివి = 0;
    తిరిగి శూన్యసూచిక;
  }

  ఒకవేళ (నిడివి) *నిడివి = blob->నిడివి;
  తిరిగి const_cast<అక్షరం *> (blob->దత్తాంశం);
}


తార్కిక
hb_blob_t::try_make_writable_inplace_unix ()
{
#ఒకవేళ defined(HAVE_SYS_MMAN_H) && defined(HAVE_MPROTECT)
  uintptr_t pagesize = -1, ముసుగు, నిడివి;
  స్థిరరాశి అక్షరం *addr;

#ఒకవేళ defined(HAVE_SYSCONF) && defined(SC_పుట_కొలత)
  pagesize = (uintptr_t) sysconf (SC_పుట_కొలత);
#లేదావేళ defined(HAVE_SYSCONF) && defined(_SC_PAGESIZE)
  pagesize = (uintptr_t) sysconf (_SC_PAGESIZE);
#లేదావేళ defined(HAVE_GETPAGESIZE)
  pagesize = (uintptr_t) getpagesize ();
#అయితే_ముగింపు

  ఒకవేళ ((uintptr_t) -1L == pagesize) {
    DEBUG_సందేశం_FUNC (BLOB, ఇది, "failed to get pagesize: %s", strerror (errno));
    తిరిగి అబద్ధం;
  }
  DEBUG_సందేశం_FUNC (BLOB, ఇది, "pagesize is %lu", (గుర్తులేని దీర్ఘ) pagesize);

  ముసుగు = ~(pagesize-1);
  addr = (స్థిరరాశి అక్షరం *) (((uintptr_t) ఇది->దత్తాంశం) & ముసుగు);
  నిడివి = (స్థిరరాశి అక్షరం *) (((uintptr_t) ఇది->దత్తాంశం + ఇది->నిడివి + pagesize-1) & ముసుగు)  - addr;
  DEBUG_సందేశం_FUNC (BLOB, ఇది,
		  "calling mprotect on [%p..%p] (%lu bytes)",
		  addr, addr+నిడివి, (గుర్తులేని దీర్ఘ) నిడివి);
  ఒకవేళ (-1 == mprotect ((శూన్య *) addr, నిడివి, PROT_READ | PROT_WRITE)) {
    DEBUG_సందేశం_FUNC (BLOB, ఇది, "mprotect failed: %s", strerror (errno));
    తిరిగి అబద్ధం;
  }

  ఇది->బహుళకం = HB_జ్ఞాపకం_బహుళకం_WRITABLE;

  DEBUG_సందేశం_FUNC (BLOB, ఇది,
		  "successfully made [%p..%p] (%lu bytes) writable\n",
		  addr, addr+నిడివి, (గుర్తులేని దీర్ఘ) నిడివి);
  తిరిగి నిజం;
#లేకపోతే
  తిరిగి అబద్ధం;
#అయితే_ముగింపు
}

తార్కిక
hb_blob_t::try_make_writable_inplace ()
{
  DEBUG_సందేశం_FUNC (BLOB, ఇది, "making writable inplace\n");

  ఒకవేళ (ఇది->try_make_writable_inplace_unix ())
    తిరిగి నిజం;

  DEBUG_సందేశం_FUNC (BLOB, ఇది, "making writable -> FAILED\n");

  /* Failed to make writable inplace, mark that */
  ఇది->బహుళకం = HB_జ్ఞాపకం_బహుళకం_READONLY;
  తిరిగి అబద్ధం;
}

తార్కిక
hb_blob_t::try_make_writable ()
{
  ఒకవేళ (unlikely (!నిడివి))
    బహుళకం = HB_జ్ఞాపకం_బహుళకం_WRITABLE;

  ఒకవేళ (ఇది->బహుళకం == HB_జ్ఞాపకం_బహుళకం_WRITABLE)
    తిరిగి నిజం;

  ఒకవేళ (ఇది->బహుళకం == HB_జ్ఞాపకం_బహుళకం_READONLY_MAY_MAKE_WRITABLE && ఇది->try_make_writable_inplace ())
    తిరిగి నిజం;

  ఒకవేళ (ఇది->బహుళకం == HB_జ్ఞాపకం_బహుళకం_WRITABLE)
    తిరిగి నిజం;


  DEBUG_సందేశం_FUNC (BLOB, ఇది, "current data is -> %p\n", ఇది->దత్తాంశం);

  అక్షరం *new_దత్తాంశం;

  new_దత్తాంశం = (అక్షరం *) hb_malloc (ఇది->నిడివి);
  ఒకవేళ (unlikely (!new_దత్తాంశం))
    తిరిగి అబద్ధం;

  DEBUG_సందేశం_FUNC (BLOB, ఇది, "dupped successfully -> %p\n", ఇది->దత్తాంశం);

  hb_memcpy (new_దత్తాంశం, ఇది->దత్తాంశం, ఇది->నిడివి);
  ఇది->destroy_user_దత్తాంశం ();
  ఇది->బహుళకం = HB_జ్ఞాపకం_బహుళకం_WRITABLE;
  ఇది->దత్తాంశం = new_దత్తాంశం;
  ఇది->user_దత్తాంశం = new_దత్తాంశం;
  ఇది->destroy = hb_free;

  తిరిగి నిజం;
}

/*
 * Mmap
 */

#నిర్వచితం_కాకపోతే HB_NO_OPEN
#నిర్వచితమైతే HAVE_MMAP
# ఒకవేళ !defined(HB_NO_RESOURCE_FORK) && defined(__APPLE__)
#  include <sys/paths.h>
# endif
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
#అయితే_ముగింపు

#నిర్వచితమైతే _WIN32
# include <windows.h>
#లేకపోతే
# ifndef O_ద్విమాన
#  define O_ద్విమాన 0
# endif
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే MAP_NORESERVE
# define MAP_NORESERVE 0
#అయితే_ముగింపు

నిర్మాణం hb_mapped_file_t
{
  అక్షరం *contents;
  గుర్తులేని దీర్ఘ నిడివి;
#నిర్వచితమైతే _WIN32
  HANDLE mapping;
#అయితే_ముగింపు
};

#ఒకవేళ (defined(HAVE_MMAP) || defined(_WIN32)) && !defined(HB_NO_MMAP)
స్థిర శూన్య
_hb_mapped_file_destroy (శూన్య *file_)
{
  hb_mapped_file_t *file = (hb_mapped_file_t *) file_;
#నిర్వచితమైతే HAVE_MMAP
  munmap (file->contents, file->నిడివి);
#లేదావేళ defined(_WIN32)
  Unmap_View_యొక్క_File (file->contents);
  CloseHandle (file->mapping);
#లేకపోతే
  ప్రతిజ్ఞా (0); // If we don't have mmap we shouldn't reach here
#అయితే_ముగింపు

  hb_free (file);
}
#అయితే_ముగింపు

#నిర్వచితమైతే మార్గం_RSRCFORKSPEC
స్థిర పూర్ణాంకం
_open_resource_fork (స్థిరరాశి అక్షరం *file_name, hb_mapped_file_t *file)
{
  size_t name_len = అక్షరశ్రేణి_పొడవు (file_name);
  size_t len = name_len + పరిమాణం (మార్గం_RSRCFORKSPEC);

  అక్షరం *rsrc_name = (అక్షరం *) hb_malloc (len);
  ఒకవేళ (unlikely (!rsrc_name)) తిరిగి -1;

  strncpy (rsrc_name, file_name, name_len);
  strncpy (rsrc_name + name_len, మార్గం_RSRCFORKSPEC,
	   పరిమాణం (మార్గం_RSRCFORKSPEC));

  పూర్ణాంకం fd = open (rsrc_name, O_RDONLY | O_ద్విమాన, 0);
  hb_free (rsrc_name);

  ఒకవేళ (fd != -1)
  {
    నిర్మాణం stat st;
    ఒకవేళ (fstat (fd, &st) != -1)
      file->నిడివి = (గుర్తులేని దీర్ఘ) st.st_కొలత;
    లేకపోతే
    {
      close (fd);
      fd = -1;
    }
  }

  తిరిగి fd;
}
#అయితే_ముగింపు

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
hb_blob_create_from_file (స్థిరరాశి అక్షరం *file_name)
{
  hb_blob_t *blob = hb_blob_create_from_file_or_fail (file_name);
  తిరిగి likely (blob) ? blob : hb_blob_get_ఖాళీలు ();
}

#ఒకవేళ defined(HAVE_MMAP) && !defined(HB_NO_MMAP)
స్థిర hb_blob_t *
_hb_blob_try_mmap (స్థిరరాశి అక్షరం *file_name)
{
  hb_mapped_file_t *file = (hb_mapped_file_t *) hb_calloc (1, పరిమాణం (hb_mapped_file_t));
  ఒకవేళ (unlikely (!file)) తిరిగి శూన్యసూచిక;
  స్వయంచాలక file_guard = hb_make_scope_guard ([&]() { hb_free (file); });

  పూర్ణాంకం fd = open (file_name, O_RDONLY | O_ద్విమాన, 0);
  ఒకవేళ (unlikely (fd == -1)) తిరిగి శూన్యసూచిక;
  స్వయంచాలక fd_guard = hb_make_scope_guard ([&]() { close (fd); });

  నిర్మాణం stat st;
  ఒకవేళ (unlikely (fstat (fd, &st) == -1)) తిరిగి శూన్యసూచిక;

  file->నిడివి = (గుర్తులేని దీర్ఘ) st.st_కొలత;

#నిర్వచితమైతే మార్గం_RSRCFORKSPEC
  ఒకవేళ (unlikely (file->నిడివి == 0))
  {
    పూర్ణాంకం rfd = _open_resource_fork (file_name, file);
    ఒకవేళ (rfd != -1)
    {
      close (fd);
      fd = rfd;
    }
  }
#అయితే_ముగింపు

  file->contents = (అక్షరం *) mmap (శూన్యసూచిక, file->నిడివి, PROT_READ,
				  MAP_వ్యక్తిగత | MAP_NORESERVE, fd, 0);
  ఒకవేళ (unlikely (file->contents == MAP_FAILED)) తిరిగి శూన్యసూచిక;

  file_guard.release ();
  /* fd_guard closes fd on return (ownership ends after mmap). */
  తిరిగి hb_blob_create_or_fail (file->contents, file->నిడివి,
				 HB_జ్ఞాపకం_బహుళకం_READONLY_MAY_MAKE_WRITABLE, (శూన్య *) file,
				 (hb_destroy_func_t) _hb_mapped_file_destroy);
}
#లేదావేళ defined(_WIN32) && !defined(HB_NO_MMAP)
స్థిర hb_blob_t *
_hb_blob_try_mmap (స్థిరరాశి అక్షరం *file_name)
{
  hb_mapped_file_t *file = (hb_mapped_file_t *) hb_calloc (1, పరిమాణం (hb_mapped_file_t));
  ఒకవేళ (unlikely (!file)) తిరిగి శూన్యసూచిక;
  స్వయంచాలక file_guard = hb_make_scope_guard ([&]() { hb_free (file); });

  size_t కొలత = అక్షరశ్రేణి_పొడవు (file_name) + 1;
  wchar_t *wchar_file_name = (wchar_t *) hb_malloc (పరిమాణం (wchar_t) * కొలత);
  ఒకవేళ (unlikely (!wchar_file_name)) తిరిగి శూన్యసూచిక;

  /* Assume file name is given in UTF-8 encoding */
  పూర్ణాంకం conversion = Multi_Byte_గమ్యం_Wide_Char (CP_UTF8, MB_ERR_INVALID_CHARS, file_name, -1, wchar_file_name, కొలత);
  ఒకవేళ (conversion <= 0)
  {
    /* Conversion failed due to invalid UTF-8 characters,
       Repeat conversion based on system code page */
    mbstowcs (wchar_file_name, file_name, కొలత);
  }

  HANDLE fd;
#ఒకవేళ !WINAPI_FAMILY_విభజన(WINAPI_విభజన_DESKTOP) && WINAPI_FAMILY_విభజన(WINAPI_విభజన_APP)
  {
    CREATEFILE2_EXTENDED_PARAMETERS ceparams = { 0 };
    ceparams.dw_కొలత = పరిమాణం(CREATEFILE2_EXTENDED_PARAMETERS);
    ceparams.dw_File_లక్షణాలు = FILE_ATTRIBUTE_సాధారణ | FILE_FLAG_OVERLAPPED & 0xFFFF;
    ceparams.dwFileFlags = FILE_ATTRIBUTE_సాధారణ | FILE_FLAG_OVERLAPPED & 0xFFF00000;
    ceparams.dwSecurityQosFlags = FILE_ATTRIBUTE_సాధారణ | FILE_FLAG_OVERLAPPED & 0x000F0000;
    ceparams.lp_Security_లక్షణాలు = శూన్యసూచిక;
    ceparams.hTemplateFile = శూన్యసూచిక;
    fd = CreateFile2 (wchar_file_name, GENERIC_READ, FILE_SHARE_READ,
		      OPEN_EXISTING, &ceparams);
  }
#లేకపోతే
  fd = CreateFileW (wchar_file_name, GENERIC_READ, FILE_SHARE_READ, శూన్యసూచిక,
		    OPEN_EXISTING, FILE_ATTRIBUTE_సాధారణ|FILE_FLAG_OVERLAPPED,
		    శూన్యసూచిక);
#అయితే_ముగింపు
  hb_free (wchar_file_name);

  ఒకవేళ (unlikely (fd == INVALID_HANDLE_విలువ)) తిరిగి శూన్యసూచిక;
  స్వయంచాలక fd_guard = hb_make_scope_guard ([&]() { CloseHandle (fd); });

#ఒకవేళ !WINAPI_FAMILY_విభజన(WINAPI_విభజన_DESKTOP) && WINAPI_FAMILY_విభజన(WINAPI_విభజన_APP)
  {
    LARGE_INTEGER నిడివి;
    Get_File_కొలత_Ex (fd, &నిడివి);
    file->నిడివి = నిడివి.దిగువ_Part;
    file->mapping = CreateFileMappingFromApp (fd, శూన్యసూచిక, పుట_READONLY, నిడివి.QuadPart, శూన్యసూచిక);
  }
#లేకపోతే
  file->నిడివి = (గుర్తులేని దీర్ఘ) Get_File_కొలత (fd, శూన్యసూచిక);
  file->mapping = CreateFileMapping (fd, శూన్యసూచిక, పుట_READONLY, 0, 0, శూన్యసూచిక);
#అయితే_ముగింపు
  ఒకవేళ (unlikely (!file->mapping)) తిరిగి శూన్యసూచిక;

#ఒకవేళ !WINAPI_FAMILY_విభజన(WINAPI_విభజన_DESKTOP) && WINAPI_FAMILY_విభజన(WINAPI_విభజన_APP)
  file->contents = (అక్షరం *) Map_View_యొక్క_File_From_App (file->mapping, FILE_MAP_READ, 0, 0);
#లేకపోతే
  file->contents = (అక్షరం *) Map_View_యొక్క_File (file->mapping, FILE_MAP_READ, 0, 0, 0);
#అయితే_ముగింపు
  ఒకవేళ (unlikely (!file->contents)) తిరిగి శూన్యసూచిక;

  file_guard.release ();
  /* fd_guard closes fd on return. */
  తిరిగి hb_blob_create_or_fail (file->contents, file->నిడివి,
				 HB_జ్ఞాపకం_బహుళకం_READONLY_MAY_MAKE_WRITABLE, (శూన్య *) file,
				 (hb_destroy_func_t) _hb_mapped_file_destroy);
}
#అయితే_ముగింపు

/* Read a file without knowing its size beforehand.  Used as a fallback
 * for systems without mmap or to read from pipes. */
స్థిర hb_blob_t *
_hb_blob_read_file (స్థిరరాశి అక్షరం *file_name)
{
  గుర్తులేని దీర్ఘ len = 0, allocated = BUFSIZ * 16;
  అక్షరం *దత్తాంశం = (అక్షరం *) hb_malloc (allocated);
  ఒకవేళ (unlikely (!దత్తాంశం)) తిరిగి శూన్యసూచిక;
  స్వయంచాలక దత్తాంశం_guard = hb_make_scope_guard ([&]() { hb_free (దత్తాంశం); });

  FILE *fp = fopen (file_name, "rb");
  ఒకవేళ (unlikely (!fp)) తిరిగి శూన్యసూచిక;
  HB_SCOPE_GUARD (fclose (fp));

  అయితే (!feof (fp))
  {
    ఒకవేళ (allocated - len < BUFSIZ)
    {
      allocated *= 2;
      /* Don't allocate and go more than ~536MB, our mmap reader still
	 can cover files like that but lets limit our fallback reader */
      ఒకవేళ (unlikely (allocated > (2 << 28))) తిరిగి శూన్యసూచిక;
      అక్షరం *new_దత్తాంశం = (అక్షరం *) hb_realloc (దత్తాంశం, allocated);
      ఒకవేళ (unlikely (!new_దత్తాంశం)) తిరిగి శూన్యసూచిక;
      దత్తాంశం = new_దత్తాంశం;
    }

    గుర్తులేని దీర్ఘ addition = fread (దత్తాంశం + len, 1, allocated - len, fp);

    పూర్ణాంకం err = ferror (fp);
#నిర్వచితమైతే EINTR // armcc doesn't have it
    ఒకవేళ (unlikely (err == EINTR)) కొనసాగు;
#అయితే_ముగింపు
    ఒకవేళ (unlikely (err)) తిరిగి శూన్యసూచిక;

    len += addition;
  }

  దత్తాంశం_guard.release ();
  తిరిగి hb_blob_create_or_fail (దత్తాంశం, len, HB_జ్ఞాపకం_బహుళకం_WRITABLE, దత్తాంశం,
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
hb_blob_create_from_file_or_fail (స్థిరరాశి అక్షరం *file_name)
{
  /* Adopted from glib's gmappedfile.c with Matthias Clasen and
     Allison Lortie permission but changed a lot to suit our need. */
#ఒకవేళ (defined(HAVE_MMAP) || defined(_WIN32)) && !defined(HB_NO_MMAP)
  ఒకవేళ (hb_blob_t *blob = _hb_blob_try_mmap (file_name))
    తిరిగి blob;
#అయితే_ముగింపు
  తిరిగి _hb_blob_read_file (file_name);
}
#అయితే_ముగింపు /* !HB_NO_OPEN */
