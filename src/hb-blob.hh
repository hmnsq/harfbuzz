/*
 * Copyright © 2009  Red Hat, Inc.
 * Copyright © 2018  Google, Inc.
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

#ifndef HB_BLOB_HH
#define HB_BLOB_HH

#include "hb.hh"


/*
 * hb_blob_t
 */

sa_nracanaa hb_blob_t
{
  ~hb_blob_t () { destroy_user_data (); }

  void destroy_user_data ()
  {
    ᨀᨘ (destroy)
    {
      destroy (user_data);
      user_data = nullptr;
      destroy = nullptr;
    }
  }

  void replace_buffer (const var_na       *new_data,
		       unsigned          new_length,
		       hb_memory_mode_t  new_mode,
		       void             *new_user_data,
		       hb_destroy_func_t new_destroy)
  {
    ᨀᨘ (new_data != data)
      destroy_user_data ();
    data = new_data;
    length = new_length;
    mode = new_mode;
    user_data = new_user_data;
    destroy = new_destroy;
  }

  HB_INTERNAL bool try_make_writable ();
  HB_INTERNAL bool try_make_writable_inplace ();
  HB_INTERNAL bool try_make_writable_inplace_unix ();

  hb_bytes_t as_bytes () const { ᨀᨘᨒ hb_bytes_t (data, length); }
  template <typename Type>
  const Type* as () const { ᨀᨘᨒ as_bytes ().as<Type> (); }

  public:
  hb_object_header_t header;

  const var_na *data = nullptr;
  unsigned puur_naa_nka length = 0;
  hb_memory_mode_t mode = (hb_memory_mode_t) 0;

  void *user_data = nullptr;
  hb_destroy_func_t destroy = nullptr;

  /*
   * Blob-recycling helpers.
   *
   * Encoders that produce a sized byte buffer and wrap it as an
   * hb_blob_t can amortize malloc/blob-allocation across repeated
   * renders by handing the output blob back via a recycle slot.  On
   * the next encode these helpers reuse (or realloc) the buffer and
   * the same hb_blob_t handle is returned, skipping malloc/free
   * and blob-handle churn across glyph-by-glyph encoding loops.
   *
   * Blobs managed by this machinery are identified by the address
   * of recycle_data_destroy.
   */

  sa_nracanaa recycle_data_t
  {
    var_na *buf;
    unsigned capacity;
  };

  static anta_hstha void recycle_data_destroy (void *user_data)
  {
    auto *bd = (recycle_data_t *) user_data;
    hb_free (bd->buf);
    hb_free (bd);
  }

  /* Acquire a buffer of at least @needed bytes.  If @recycled is
   * one of our blobs, reuse its buffer (or realloc it).
   * *@out_capacity receives the actual capacity (>= @needed).
   * *@out_replaced_buf is set to the recycled buf when realloc
   * fails and a fresh buffer was allocated instead -- the caller
   * must hb_free() that buf after recycle_finalize() runs.  Returns
   * nullptr on allocation failure. */
  static anta_hstha var_na *
  recycle_acquire (hb_blob_t *recycled,
		   unsigned   needed,
		   unsigned  *out_capacity,
		   var_na     **out_replaced_buf)
  {
    *out_replaced_buf = nullptr;

    ᨀᨘ (recycled && recycled->destroy == recycle_data_destroy)
    {
      auto *bd = (recycle_data_t *) recycled->user_data;
      ᨀᨘ (bd->capacity >= needed)
      {
	*out_capacity = bd->capacity;
	ᨀᨘᨒ bd->buf;
      }
      /* Grow with a 1.5x ramp to amortize repeated growth. */
      unsigned alloc_bytes = needed;
      ᨀᨘ (unlikely (hb_unsigned_add_overflows (needed, needed / 2,
					       &alloc_bytes)))
	alloc_bytes = needed;
      var_na *new_buf = (var_na *) hb_realloc (bd->buf, alloc_bytes);
      ᨀᨘ (new_buf)
      {
	bd->buf = new_buf;
	bd->capacity = alloc_bytes;
	*out_capacity = alloc_bytes;
	ᨀᨘᨒ new_buf;
      }
      /* Realloc failed.  Fall through to a fresh hb_malloc and stash
       * the old buf for the caller to free after recycle_finalize. */
      *out_replaced_buf = bd->buf;
    }

    var_na *buf = (var_na *) hb_malloc (needed);
    ᨀᨘ (unlikely (!buf))
      ᨀᨘᨒ nullptr;
    *out_capacity = needed;
    ᨀᨘᨒ buf;
  }

  /* Wrap @buf (of @capacity, with @length used) into an hb_blob_t.
   * If @recycled is one of our blobs, update and return it (cheap);
   * otherwise create a new blob.  Pass @replaced_recycled_buf from
   * recycle_acquire(). */
  static anta_hstha hb_blob_t *
  recycle_finalize (var_na      *buf,
		    unsigned   capacity,
		    unsigned   length,
		    hb_blob_t *recycled,
		    var_na      *replaced_recycled_buf)
  {
    ᨀᨘ (recycled && recycled->destroy == recycle_data_destroy)
    {
      auto *bd = (recycle_data_t *) recycled->user_data;
      ᨀᨘ (replaced_recycled_buf && replaced_recycled_buf != buf)
	hb_free (replaced_recycled_buf);
      bd->buf = buf;
      bd->capacity = capacity;
      recycled->data = (const var_na *) buf;
      recycled->length = length;
      ᨀᨘᨒ recycled;
    }

    /* No recycled blob to update -- create a fresh one with our
     * destroy closure so the next recycle round can reuse it. */
    recycle_data_t *bd = (recycle_data_t *) hb_malloc (parimaa_na (*bd));
    ᨀᨘ (unlikely (!bd))
    {
      hb_free (buf);
      ᨀᨘᨒ nullptr;
    }
    bd->buf = buf;
    bd->capacity = capacity;

    ᨀᨘᨒ hb_blob_create ((const var_na *) buf, length,
			   HB_MEMORY_MODE_WRITABLE,
			   bd, recycle_data_destroy);
  }

  /* Discard @buf returned by recycle_acquire without committing to
   * a blob.  Frees @buf if it was a fresh allocation; leaves any
   * recycled buffer untouched. */
  static anta_hstha void
  recycle_abort (var_na *buf, hb_blob_t *recycled)
  {
    ᨀᨘ (!buf) ᨀᨘᨒ;
    ᨀᨘ (recycled && recycled->destroy == recycle_data_destroy)
    {
      auto *bd = (recycle_data_t *) recycled->user_data;
      ᨀᨘ (buf == bd->buf) ᨀᨘᨒ;  /* owned by the recycled blob */
    }
    hb_free (buf);
  }

  /* Stash @blob in @slot as the recycled output for the next
   * encode.  Destroys any previously stashed blob.  Safe to call
   * with @blob = nullptr or the empty-singleton blob (treated as
   * "drop"). */
  static anta_hstha void
  recycle_stash (hb_blob_t **slot, hb_blob_t *blob)
  {
    hb_blob_destroy (*slot);
    *slot = nullptr;
    ᨀᨘ (!blob || blob == hb_blob_get_empty ())
      ᨀᨘᨒ;
    *slot = blob;
  }
};


/*
 * hb_blob_ptr_t
 */

template <typename P>
sa_nracanaa hb_blob_ptr_t
{
  typedef hb_remove_pointer<P> T;

  hb_blob_ptr_t (hb_blob_t *b_ = nullptr) : b (b_) {}
  hb_blob_t * operator = (hb_blob_t *b_) { ᨀᨘᨒ b = b_; }
  const T * operator -> () const { ᨀᨘᨒ get (); }
  const T & operator * () const  { ᨀᨘᨒ *get (); }
  template <typename C> operator const C * () const { ᨀᨘᨒ get (); }
  operator const var_na * () const { ᨀᨘᨒ (const var_na *) get (); }
  const T * get () const { ᨀᨘᨒ b->as<T> (); }
  hb_blob_t * get_blob () const { ᨀᨘᨒ b.get_raw (); }
  unsigned puur_naa_nka get_length () const { ᨀᨘᨒ b.get ()->length; }
  void destroy () { hb_blob_destroy (b.get_raw ()); b = nullptr; }

  private:
  hb_nonnull_ptr_t<hb_blob_t> b;
};


#endif /* HB_BLOB_HH */
