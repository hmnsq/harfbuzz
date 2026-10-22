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

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-blob.hh"

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_SYS_MMAN_H
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_UNISTD_H
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਜੇ_ਅੰਤ /* HAVE_UNISTD_H */
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/mman.h>
#ਜੇ_ਅੰਤ /* HAVE_SYS_MMAN_H */


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
hb_blob_create (ਅਚਲ ਅੱਖਰ        *ਅੰਕੜੇ,
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ       ਲੰਮਾਈ,
		hb_ਸਮ੍ਰਿਤੀ_ਬਹੁਲਕ_t   ਬਹੁਲਕ,
		ਖਾਲੀ              *user_ਅੰਕੜੇ,
		hb_destroy_func_t  destroy)
{
  ਜੇ (!ਲੰਮਾਈ)
  {
    ਜੇ (destroy)
      destroy (user_ਅੰਕੜੇ);
    ਵਾਪਸ hb_blob_get_ਖਾਲੀ_ਥਾਵਾਂ ();
  }

  hb_blob_t *blob = hb_blob_create_or_fail (ਅੰਕੜੇ, ਲੰਮਾਈ, ਬਹੁਲਕ,
					    user_ਅੰਕੜੇ, destroy);
  ਵਾਪਸ likely (blob) ? blob : hb_blob_get_ਖਾਲੀ_ਥਾਵਾਂ ();
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
hb_blob_create_or_fail (ਅਚਲ ਅੱਖਰ        *ਅੰਕੜੇ,
			ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ       ਲੰਮਾਈ,
			hb_ਸਮ੍ਰਿਤੀ_ਬਹੁਲਕ_t   ਬਹੁਲਕ,
			ਖਾਲੀ              *user_ਅੰਕੜੇ,
			hb_destroy_func_t  destroy)
{
  hb_blob_t *blob;

  ਜੇ (ਲੰਮਾਈ >= 1u << 31 ||
      !(blob = hb_object_create<hb_blob_t> ()))
  {
    ਜੇ (destroy)
      destroy (user_ਅੰਕੜੇ);
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }

  blob->ਅੰਕੜੇ = ਅੰਕੜੇ;
  blob->ਲੰਮਾਈ = ਲੰਮਾਈ;
  blob->ਬਹੁਲਕ = ਬਹੁਲਕ;

  blob->user_ਅੰਕੜੇ = user_ਅੰਕੜੇ;
  blob->destroy = destroy;

  ਜੇ (blob->ਬਹੁਲਕ == HB_ਸਮ੍ਰਿਤੀ_ਬਹੁਲਕ_DUPLICATE) {
    blob->ਬਹੁਲਕ = HB_ਸਮ੍ਰਿਤੀ_ਬਹੁਲਕ_READONLY;
    ਜੇ (!blob->try_make_writable ())
    {
      hb_blob_destroy (blob);
      ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
    }
  }

  ਵਾਪਸ blob;
}

ਸਥਿਰ ਖਾਲੀ
_hb_blob_destroy (ਖਾਲੀ *ਅੰਕੜੇ)
{
  hb_blob_destroy ((hb_blob_t *) ਅੰਕੜੇ);
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
hb_blob_create_sub_blob (hb_blob_t    *ਮਾਪਾ,
			 ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  ਵਿੱਥ,
			 ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  ਲੰਮਾਈ)
{
  hb_blob_t *blob;

  ਜੇ (!ਲੰਮਾਈ || !ਮਾਪਾ || ਵਿੱਥ >= ਮਾਪਾ->ਲੰਮਾਈ)
    ਵਾਪਸ hb_blob_get_ਖਾਲੀ_ਥਾਵਾਂ ();

  hb_blob_make_immutable (ਮਾਪਾ);

  blob = hb_blob_create (ਮਾਪਾ->ਅੰਕੜੇ + ਵਿੱਥ,
			 hb_min (ਲੰਮਾਈ, ਮਾਪਾ->ਲੰਮਾਈ - ਵਿੱਥ),
			 HB_ਸਮ੍ਰਿਤੀ_ਬਹੁਲਕ_READONLY,
			 hb_blob_reference (ਮਾਪਾ),
			 _hb_blob_destroy);

  ਵਾਪਸ blob;
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
  blob = hb_blob_create (blob->ਅੰਕੜੇ,
			 blob->ਲੰਮਾਈ,
			 HB_ਸਮ੍ਰਿਤੀ_ਬਹੁਲਕ_DUPLICATE,
			 ਖਾਲੀ_ਸੂਚਕ,
			 ਖਾਲੀ_ਸੂਚਕ);

  ਜੇ (unlikely (blob == hb_blob_get_ਖਾਲੀ_ਥਾਵਾਂ ()))
    blob = ਖਾਲੀ_ਸੂਚਕ;

  ਵਾਪਸ blob;
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
hb_blob_get_ਖਾਲੀ_ਥਾਵਾਂ ()
{
  ਵਾਪਸ const_cast<hb_blob_t *> (&Null (hb_blob_t));
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
  ਵਾਪਸ hb_object_reference (blob);
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
ਖਾਲੀ
hb_blob_destroy (hb_blob_t *blob)
{
  ਜੇ (!hb_object_destroy (blob)) ਵਾਪਸ;

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
hb_blob_ਸੈੱਟ_user_ਅੰਕੜੇ (hb_blob_t          *blob,
		       hb_user_ਅੰਕੜੇ_ਕੁੰਜੀ_t *key,
		       ਖਾਲੀ *              ਅੰਕੜੇ,
		       hb_destroy_func_t   destroy,
		       hb_bool_t           replace)
{
  ਵਾਪਸ hb_object_ਸੈੱਟ_user_ਅੰਕੜੇ (blob, key, ਅੰਕੜੇ, destroy, replace);
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
ਖਾਲੀ *
hb_blob_get_user_ਅੰਕੜੇ (ਅਚਲ hb_blob_t    *blob,
		       hb_user_ਅੰਕੜੇ_ਕੁੰਜੀ_t *key)
{
  ਵਾਪਸ hb_object_get_user_ਅੰਕੜੇ (blob, key);
}


/**
 * hb_blob_make_immutable:
 * @blob: a blob
 *
 * Makes a blob immutable.
 *
 * Since: 0.9.2
 **/
ਖਾਲੀ
hb_blob_make_immutable (hb_blob_t *blob)
{
  ਜੇ (hb_object_is_immutable (blob))
    ਵਾਪਸ;

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
  ਵਾਪਸ hb_object_is_immutable (blob);
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
ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_blob_get_ਲੰਮਾਈ (hb_blob_t *blob)
{
  ਵਾਪਸ blob->ਲੰਮਾਈ;
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
ਅਚਲ ਅੱਖਰ *
hb_blob_get_ਅੰਕੜੇ (hb_blob_t *blob, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *ਲੰਮਾਈ)
{
  ਜੇ (ਲੰਮਾਈ)
    *ਲੰਮਾਈ = blob->ਲੰਮਾਈ;

  ਵਾਪਸ blob->ਅੰਕੜੇ;
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
ਅੱਖਰ *
hb_blob_get_ਅੰਕੜੇ_writable (hb_blob_t *blob, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *ਲੰਮਾਈ)
{
  ਜੇ (hb_object_is_immutable (blob) ||
     !blob->try_make_writable ())
  {
    ਜੇ (ਲੰਮਾਈ) *ਲੰਮਾਈ = 0;
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }

  ਜੇ (ਲੰਮਾਈ) *ਲੰਮਾਈ = blob->ਲੰਮਾਈ;
  ਵਾਪਸ const_cast<ਅੱਖਰ *> (blob->ਅੰਕੜੇ);
}


ਤਾਰਕਿਕ
hb_blob_t::try_make_writable_inplace_unix ()
{
#ਜੇ defined(HAVE_SYS_MMAN_H) && defined(HAVE_MPROTECT)
  uintptr_t pagesize = -1, ਨਕਾਬ, ਲੰਮਾਈ;
  ਅਚਲ ਅੱਖਰ *addr;

#ਜੇ defined(HAVE_SYSCONF) && defined(SC_ਪੰਨਾ_ਮਾਪ)
  pagesize = (uintptr_t) sysconf (SC_ਪੰਨਾ_ਮਾਪ);
#ਜਾਂਜੇ defined(HAVE_SYSCONF) && defined(_SC_PAGESIZE)
  pagesize = (uintptr_t) sysconf (_SC_PAGESIZE);
#ਜਾਂਜੇ defined(HAVE_GETPAGESIZE)
  pagesize = (uintptr_t) getpagesize ();
#ਜੇ_ਅੰਤ

  ਜੇ ((uintptr_t) -1L == pagesize) {
    DEBUG_ਸੁਨੇਹਾ_FUNC (BLOB, ਇਹ, "failed to get pagesize: %s", strerror (errno));
    ਵਾਪਸ ਝੂਠ;
  }
  DEBUG_ਸੁਨੇਹਾ_FUNC (BLOB, ਇਹ, "pagesize is %lu", (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ) pagesize);

  ਨਕਾਬ = ~(pagesize-1);
  addr = (ਅਚਲ ਅੱਖਰ *) (((uintptr_t) ਇਹ->ਅੰਕੜੇ) & ਨਕਾਬ);
  ਲੰਮਾਈ = (ਅਚਲ ਅੱਖਰ *) (((uintptr_t) ਇਹ->ਅੰਕੜੇ + ਇਹ->ਲੰਮਾਈ + pagesize-1) & ਨਕਾਬ)  - addr;
  DEBUG_ਸੁਨੇਹਾ_FUNC (BLOB, ਇਹ,
		  "calling mprotect on [%p..%p] (%lu bytes)",
		  addr, addr+ਲੰਮਾਈ, (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ) ਲੰਮਾਈ);
  ਜੇ (-1 == mprotect ((ਖਾਲੀ *) addr, ਲੰਮਾਈ, PROT_READ | PROT_WRITE)) {
    DEBUG_ਸੁਨੇਹਾ_FUNC (BLOB, ਇਹ, "mprotect failed: %s", strerror (errno));
    ਵਾਪਸ ਝੂਠ;
  }

  ਇਹ->ਬਹੁਲਕ = HB_ਸਮ੍ਰਿਤੀ_ਬਹੁਲਕ_WRITABLE;

  DEBUG_ਸੁਨੇਹਾ_FUNC (BLOB, ਇਹ,
		  "successfully made [%p..%p] (%lu bytes) writable\n",
		  addr, addr+ਲੰਮਾਈ, (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ) ਲੰਮਾਈ);
  ਵਾਪਸ ਸੱਚ;
#ਨਹੀਂਤਾਂ
  ਵਾਪਸ ਝੂਠ;
#ਜੇ_ਅੰਤ
}

ਤਾਰਕਿਕ
hb_blob_t::try_make_writable_inplace ()
{
  DEBUG_ਸੁਨੇਹਾ_FUNC (BLOB, ਇਹ, "making writable inplace\n");

  ਜੇ (ਇਹ->try_make_writable_inplace_unix ())
    ਵਾਪਸ ਸੱਚ;

  DEBUG_ਸੁਨੇਹਾ_FUNC (BLOB, ਇਹ, "making writable -> FAILED\n");

  /* Failed to make writable inplace, mark that */
  ਇਹ->ਬਹੁਲਕ = HB_ਸਮ੍ਰਿਤੀ_ਬਹੁਲਕ_READONLY;
  ਵਾਪਸ ਝੂਠ;
}

ਤਾਰਕਿਕ
hb_blob_t::try_make_writable ()
{
  ਜੇ (unlikely (!ਲੰਮਾਈ))
    ਬਹੁਲਕ = HB_ਸਮ੍ਰਿਤੀ_ਬਹੁਲਕ_WRITABLE;

  ਜੇ (ਇਹ->ਬਹੁਲਕ == HB_ਸਮ੍ਰਿਤੀ_ਬਹੁਲਕ_WRITABLE)
    ਵਾਪਸ ਸੱਚ;

  ਜੇ (ਇਹ->ਬਹੁਲਕ == HB_ਸਮ੍ਰਿਤੀ_ਬਹੁਲਕ_READONLY_MAY_MAKE_WRITABLE && ਇਹ->try_make_writable_inplace ())
    ਵਾਪਸ ਸੱਚ;

  ਜੇ (ਇਹ->ਬਹੁਲਕ == HB_ਸਮ੍ਰਿਤੀ_ਬਹੁਲਕ_WRITABLE)
    ਵਾਪਸ ਸੱਚ;


  DEBUG_ਸੁਨੇਹਾ_FUNC (BLOB, ਇਹ, "current data is -> %p\n", ਇਹ->ਅੰਕੜੇ);

  ਅੱਖਰ *new_ਅੰਕੜੇ;

  new_ਅੰਕੜੇ = (ਅੱਖਰ *) hb_malloc (ਇਹ->ਲੰਮਾਈ);
  ਜੇ (unlikely (!new_ਅੰਕੜੇ))
    ਵਾਪਸ ਝੂਠ;

  DEBUG_ਸੁਨੇਹਾ_FUNC (BLOB, ਇਹ, "dupped successfully -> %p\n", ਇਹ->ਅੰਕੜੇ);

  hb_memcpy (new_ਅੰਕੜੇ, ਇਹ->ਅੰਕੜੇ, ਇਹ->ਲੰਮਾਈ);
  ਇਹ->destroy_user_ਅੰਕੜੇ ();
  ਇਹ->ਬਹੁਲਕ = HB_ਸਮ੍ਰਿਤੀ_ਬਹੁਲਕ_WRITABLE;
  ਇਹ->ਅੰਕੜੇ = new_ਅੰਕੜੇ;
  ਇਹ->user_ਅੰਕੜੇ = new_ਅੰਕੜੇ;
  ਇਹ->destroy = hb_free;

  ਵਾਪਸ ਸੱਚ;
}

/*
 * Mmap
 */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OPEN
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_MMAP
# ਜੇ !defined(HB_NO_RESOURCE_FORK) && defined(__APPLE__)
#  include <sys/paths.h>
# endif
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _WIN32
# include <windows.h>
#ਨਹੀਂਤਾਂ
# ifndef O_ਦੁਆਧਾਰੀ
#  define O_ਦੁਆਧਾਰੀ 0
# endif
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ MAP_NORESERVE
# define MAP_NORESERVE 0
#ਜੇ_ਅੰਤ

ਬਣਤਰ hb_mapped_file_t
{
  ਅੱਖਰ *contents;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾਈ;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _WIN32
  HANDLE mapping;
#ਜੇ_ਅੰਤ
};

#ਜੇ (defined(HAVE_MMAP) || defined(_WIN32)) && !defined(HB_NO_MMAP)
ਸਥਿਰ ਖਾਲੀ
_hb_mapped_file_destroy (ਖਾਲੀ *file_)
{
  hb_mapped_file_t *file = (hb_mapped_file_t *) file_;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_MMAP
  munmap (file->contents, file->ਲੰਮਾਈ);
#ਜਾਂਜੇ defined(_WIN32)
  Unmap_View_ਦਾ_File (file->contents);
  CloseHandle (file->mapping);
#ਨਹੀਂਤਾਂ
  ਪ੍ਰਤਿਜ੍ਞਾ (0); // If we don't have mmap we shouldn't reach here
#ਜੇ_ਅੰਤ

  hb_free (file);
}
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ਰਸਤਾ_RSRCFORKSPEC
ਸਥਿਰ ਪੂਰਨ
_open_resource_fork (ਅਚਲ ਅੱਖਰ *file_name, hb_mapped_file_t *file)
{
  size_t name_len = ਲੜੀ_ਲੰਬਾਈ (file_name);
  size_t len = name_len + ਆਕਾਰ (ਰਸਤਾ_RSRCFORKSPEC);

  ਅੱਖਰ *rsrc_name = (ਅੱਖਰ *) hb_malloc (len);
  ਜੇ (unlikely (!rsrc_name)) ਵਾਪਸ -1;

  strncpy (rsrc_name, file_name, name_len);
  strncpy (rsrc_name + name_len, ਰਸਤਾ_RSRCFORKSPEC,
	   ਆਕਾਰ (ਰਸਤਾ_RSRCFORKSPEC));

  ਪੂਰਨ fd = open (rsrc_name, O_RDONLY | O_ਦੁਆਧਾਰੀ, 0);
  hb_free (rsrc_name);

  ਜੇ (fd != -1)
  {
    ਬਣਤਰ stat st;
    ਜੇ (fstat (fd, &st) != -1)
      file->ਲੰਮਾਈ = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ) st.st_ਮਾਪ;
    ਨਹੀਂਤਾਂ
    {
      close (fd);
      fd = -1;
    }
  }

  ਵਾਪਸ fd;
}
#ਜੇ_ਅੰਤ

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
hb_blob_create_from_file (ਅਚਲ ਅੱਖਰ *file_name)
{
  hb_blob_t *blob = hb_blob_create_from_file_or_fail (file_name);
  ਵਾਪਸ likely (blob) ? blob : hb_blob_get_ਖਾਲੀ_ਥਾਵਾਂ ();
}

#ਜੇ defined(HAVE_MMAP) && !defined(HB_NO_MMAP)
ਸਥਿਰ hb_blob_t *
_hb_blob_try_mmap (ਅਚਲ ਅੱਖਰ *file_name)
{
  hb_mapped_file_t *file = (hb_mapped_file_t *) hb_calloc (1, ਆਕਾਰ (hb_mapped_file_t));
  ਜੇ (unlikely (!file)) ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  ਸਵੈਚਾਲਿਤ file_guard = hb_make_scope_guard ([&]() { hb_free (file); });

  ਪੂਰਨ fd = open (file_name, O_RDONLY | O_ਦੁਆਧਾਰੀ, 0);
  ਜੇ (unlikely (fd == -1)) ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  ਸਵੈਚਾਲਿਤ fd_guard = hb_make_scope_guard ([&]() { close (fd); });

  ਬਣਤਰ stat st;
  ਜੇ (unlikely (fstat (fd, &st) == -1)) ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  file->ਲੰਮਾਈ = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ) st.st_ਮਾਪ;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ਰਸਤਾ_RSRCFORKSPEC
  ਜੇ (unlikely (file->ਲੰਮਾਈ == 0))
  {
    ਪੂਰਨ rfd = _open_resource_fork (file_name, file);
    ਜੇ (rfd != -1)
    {
      close (fd);
      fd = rfd;
    }
  }
#ਜੇ_ਅੰਤ

  file->contents = (ਅੱਖਰ *) mmap (ਖਾਲੀ_ਸੂਚਕ, file->ਲੰਮਾਈ, PROT_READ,
				  MAP_ਨਿੱਜੀ | MAP_NORESERVE, fd, 0);
  ਜੇ (unlikely (file->contents == MAP_FAILED)) ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  file_guard.release ();
  /* fd_guard closes fd on return (ownership ends after mmap). */
  ਵਾਪਸ hb_blob_create_or_fail (file->contents, file->ਲੰਮਾਈ,
				 HB_ਸਮ੍ਰਿਤੀ_ਬਹੁਲਕ_READONLY_MAY_MAKE_WRITABLE, (ਖਾਲੀ *) file,
				 (hb_destroy_func_t) _hb_mapped_file_destroy);
}
#ਜਾਂਜੇ defined(_WIN32) && !defined(HB_NO_MMAP)
ਸਥਿਰ hb_blob_t *
_hb_blob_try_mmap (ਅਚਲ ਅੱਖਰ *file_name)
{
  hb_mapped_file_t *file = (hb_mapped_file_t *) hb_calloc (1, ਆਕਾਰ (hb_mapped_file_t));
  ਜੇ (unlikely (!file)) ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  ਸਵੈਚਾਲਿਤ file_guard = hb_make_scope_guard ([&]() { hb_free (file); });

  size_t ਮਾਪ = ਲੜੀ_ਲੰਬਾਈ (file_name) + 1;
  wchar_t *wchar_file_name = (wchar_t *) hb_malloc (ਆਕਾਰ (wchar_t) * ਮਾਪ);
  ਜੇ (unlikely (!wchar_file_name)) ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  /* Assume file name is given in UTF-8 encoding */
  ਪੂਰਨ conversion = Multi_Byte_ਮੰਜ਼ਿਲ_Wide_Char (CP_UTF8, MB_ERR_INVALID_CHARS, file_name, -1, wchar_file_name, ਮਾਪ);
  ਜੇ (conversion <= 0)
  {
    /* Conversion failed due to invalid UTF-8 characters,
       Repeat conversion based on system code page */
    mbstowcs (wchar_file_name, file_name, ਮਾਪ);
  }

  HANDLE fd;
#ਜੇ !WINAPI_FAMILY_ਹਿੱਸੇ_ਕਰੋ(WINAPI_ਹਿੱਸੇ_ਕਰੋ_DESKTOP) && WINAPI_FAMILY_ਹਿੱਸੇ_ਕਰੋ(WINAPI_ਹਿੱਸੇ_ਕਰੋ_APP)
  {
    CREATEFILE2_EXTENDED_PARAMETERS ceparams = { 0 };
    ceparams.dw_ਮਾਪ = ਆਕਾਰ(CREATEFILE2_EXTENDED_PARAMETERS);
    ceparams.dw_File_ਗੁਣ = FILE_ATTRIBUTE_ਸਾਧਾਰਨ | FILE_FLAG_OVERLAPPED & 0xFFFF;
    ceparams.dwFileFlags = FILE_ATTRIBUTE_ਸਾਧਾਰਨ | FILE_FLAG_OVERLAPPED & 0xFFF00000;
    ceparams.dwSecurityQosFlags = FILE_ATTRIBUTE_ਸਾਧਾਰਨ | FILE_FLAG_OVERLAPPED & 0x000F0000;
    ceparams.lp_Security_ਗੁਣ = ਖਾਲੀ_ਸੂਚਕ;
    ceparams.hTemplateFile = ਖਾਲੀ_ਸੂਚਕ;
    fd = CreateFile2 (wchar_file_name, GENERIC_READ, FILE_SHARE_READ,
		      OPEN_EXISTING, &ceparams);
  }
#ਨਹੀਂਤਾਂ
  fd = CreateFileW (wchar_file_name, GENERIC_READ, FILE_SHARE_READ, ਖਾਲੀ_ਸੂਚਕ,
		    OPEN_EXISTING, FILE_ATTRIBUTE_ਸਾਧਾਰਨ|FILE_FLAG_OVERLAPPED,
		    ਖਾਲੀ_ਸੂਚਕ);
#ਜੇ_ਅੰਤ
  hb_free (wchar_file_name);

  ਜੇ (unlikely (fd == INVALID_HANDLE_ਮੁੱਲ)) ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  ਸਵੈਚਾਲਿਤ fd_guard = hb_make_scope_guard ([&]() { CloseHandle (fd); });

#ਜੇ !WINAPI_FAMILY_ਹਿੱਸੇ_ਕਰੋ(WINAPI_ਹਿੱਸੇ_ਕਰੋ_DESKTOP) && WINAPI_FAMILY_ਹਿੱਸੇ_ਕਰੋ(WINAPI_ਹਿੱਸੇ_ਕਰੋ_APP)
  {
    LARGE_INTEGER ਲੰਮਾਈ;
    Get_File_ਮਾਪ_Ex (fd, &ਲੰਮਾਈ);
    file->ਲੰਮਾਈ = ਲੰਮਾਈ.ਨੀਵਾਂ_Part;
    file->mapping = CreateFileMappingFromApp (fd, ਖਾਲੀ_ਸੂਚਕ, ਪੰਨਾ_READONLY, ਲੰਮਾਈ.QuadPart, ਖਾਲੀ_ਸੂਚਕ);
  }
#ਨਹੀਂਤਾਂ
  file->ਲੰਮਾਈ = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ) Get_File_ਮਾਪ (fd, ਖਾਲੀ_ਸੂਚਕ);
  file->mapping = CreateFileMapping (fd, ਖਾਲੀ_ਸੂਚਕ, ਪੰਨਾ_READONLY, 0, 0, ਖਾਲੀ_ਸੂਚਕ);
#ਜੇ_ਅੰਤ
  ਜੇ (unlikely (!file->mapping)) ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

#ਜੇ !WINAPI_FAMILY_ਹਿੱਸੇ_ਕਰੋ(WINAPI_ਹਿੱਸੇ_ਕਰੋ_DESKTOP) && WINAPI_FAMILY_ਹਿੱਸੇ_ਕਰੋ(WINAPI_ਹਿੱਸੇ_ਕਰੋ_APP)
  file->contents = (ਅੱਖਰ *) Map_View_ਦਾ_File_From_App (file->mapping, FILE_MAP_READ, 0, 0);
#ਨਹੀਂਤਾਂ
  file->contents = (ਅੱਖਰ *) Map_View_ਦਾ_File (file->mapping, FILE_MAP_READ, 0, 0, 0);
#ਜੇ_ਅੰਤ
  ਜੇ (unlikely (!file->contents)) ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  file_guard.release ();
  /* fd_guard closes fd on return. */
  ਵਾਪਸ hb_blob_create_or_fail (file->contents, file->ਲੰਮਾਈ,
				 HB_ਸਮ੍ਰਿਤੀ_ਬਹੁਲਕ_READONLY_MAY_MAKE_WRITABLE, (ਖਾਲੀ *) file,
				 (hb_destroy_func_t) _hb_mapped_file_destroy);
}
#ਜੇ_ਅੰਤ

/* Read a file without knowing its size beforehand.  Used as a fallback
 * for systems without mmap or to read from pipes. */
ਸਥਿਰ hb_blob_t *
_hb_blob_read_file (ਅਚਲ ਅੱਖਰ *file_name)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ len = 0, allocated = BUFSIZ * 16;
  ਅੱਖਰ *ਅੰਕੜੇ = (ਅੱਖਰ *) hb_malloc (allocated);
  ਜੇ (unlikely (!ਅੰਕੜੇ)) ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  ਸਵੈਚਾਲਿਤ ਅੰਕੜੇ_guard = hb_make_scope_guard ([&]() { hb_free (ਅੰਕੜੇ); });

  FILE *fp = fopen (file_name, "rb");
  ਜੇ (unlikely (!fp)) ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  HB_SCOPE_GUARD (fclose (fp));

  ਜਦੋਂ (!feof (fp))
  {
    ਜੇ (allocated - len < BUFSIZ)
    {
      allocated *= 2;
      /* Don't allocate and go more than ~536MB, our mmap reader still
	 can cover files like that but lets limit our fallback reader */
      ਜੇ (unlikely (allocated > (2 << 28))) ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
      ਅੱਖਰ *new_ਅੰਕੜੇ = (ਅੱਖਰ *) hb_realloc (ਅੰਕੜੇ, allocated);
      ਜੇ (unlikely (!new_ਅੰਕੜੇ)) ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
      ਅੰਕੜੇ = new_ਅੰਕੜੇ;
    }

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ addition = fread (ਅੰਕੜੇ + len, 1, allocated - len, fp);

    ਪੂਰਨ err = ferror (fp);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ EINTR // armcc doesn't have it
    ਜੇ (unlikely (err == EINTR)) ਜਾਰੀ;
#ਜੇ_ਅੰਤ
    ਜੇ (unlikely (err)) ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

    len += addition;
  }

  ਅੰਕੜੇ_guard.release ();
  ਵਾਪਸ hb_blob_create_or_fail (ਅੰਕੜੇ, len, HB_ਸਮ੍ਰਿਤੀ_ਬਹੁਲਕ_WRITABLE, ਅੰਕੜੇ,
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
hb_blob_create_from_file_or_fail (ਅਚਲ ਅੱਖਰ *file_name)
{
  /* Adopted from glib's gmappedfile.c with Matthias Clasen and
     Allison Lortie permission but changed a lot to suit our need. */
#ਜੇ (defined(HAVE_MMAP) || defined(_WIN32)) && !defined(HB_NO_MMAP)
  ਜੇ (hb_blob_t *blob = _hb_blob_try_mmap (file_name))
    ਵਾਪਸ blob;
#ਜੇ_ਅੰਤ
  ਵਾਪਸ _hb_blob_read_file (file_name);
}
#ਜੇ_ਅੰਤ /* !HB_NO_OPEN */
