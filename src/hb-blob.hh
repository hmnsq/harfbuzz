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

#அந்தர்பாவ "hb.hh"


/*
 * hb_blob_t
 */

கட்டமைப்பு hb_blob_t
{
  ~hb_blob_t () { destroy_user_data (); }

  வெறுமை destroy_user_data ()
  {
    எனில் (destroy)
    {
      destroy (user_data);
      user_data = nullptr;
      destroy = nullptr;
    }
  }

  வெறுமை replace_buffer (மாறிலி எழுத்து       *new_data,
		       unsigned          new_length,
		       hb_memory_mode_t  new_mode,
		       வெறுமை             *new_user_data,
		       hb_destroy_func_t new_destroy)
  {
    எனில் (new_data != data)
      destroy_user_data ();
    data = new_data;
    length = new_length;
    mode = new_mode;
    user_data = new_user_data;
    destroy = new_destroy;
  }

  HB_INTERNAL தருக்க try_make_writable ();
  HB_INTERNAL தருக்க try_make_writable_inplace ();
  HB_INTERNAL தருக்க try_make_writable_inplace_unix ();

  hb_bytes_t as_bytes () மாறிலி { திரும்பு hb_bytes_t (data, length); }
  template <typename Type>
  மாறிலி Type* as () மாறிலி { திரும்பு as_bytes ().as<Type> (); }

  பொது:
  hb_object_header_t header;

  மாறிலி எழுத்து *data = nullptr;
  unsigned முழு length = 0;
  hb_memory_mode_t mode = (hb_memory_mode_t) 0;

  வெறுமை *user_data = nullptr;
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

  கட்டமைப்பு recycle_data_t
  {
    எழுத்து *buf;
    unsigned capacity;
  };

  நிலையான உள்வரி வெறுமை recycle_data_destroy (வெறுமை *user_data)
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
  நிலையான உள்வரி எழுத்து *
  recycle_acquire (hb_blob_t *recycled,
		   unsigned   needed,
		   unsigned  *out_capacity,
		   எழுத்து     **out_replaced_buf)
  {
    *out_replaced_buf = nullptr;

    எனில் (recycled && recycled->destroy == recycle_data_destroy)
    {
      auto *bd = (recycle_data_t *) recycled->user_data;
      எனில் (bd->capacity >= needed)
      {
	*out_capacity = bd->capacity;
	திரும்பு bd->buf;
      }
      /* Grow with a 1.5x ramp to amortize repeated growth. */
      unsigned alloc_bytes = needed;
      எனில் (unlikely (hb_unsigned_add_overflows (needed, needed / 2,
					       &alloc_bytes)))
	alloc_bytes = needed;
      எழுத்து *new_buf = (எழுத்து *) hb_realloc (bd->buf, alloc_bytes);
      எனில் (new_buf)
      {
	bd->buf = new_buf;
	bd->capacity = alloc_bytes;
	*out_capacity = alloc_bytes;
	திரும்பு new_buf;
      }
      /* Realloc failed.  Fall through to a fresh hb_malloc and stash
       * the old buf for the caller to free after recycle_finalize. */
      *out_replaced_buf = bd->buf;
    }

    எழுத்து *buf = (எழுத்து *) hb_malloc (needed);
    எனில் (unlikely (!buf))
      திரும்பு nullptr;
    *out_capacity = needed;
    திரும்பு buf;
  }

  /* Wrap @buf (of @capacity, with @length used) into an hb_blob_t.
   * If @recycled is one of our blobs, update and return it (cheap);
   * otherwise create a new blob.  Pass @replaced_recycled_buf from
   * recycle_acquire(). */
  நிலையான உள்வரி hb_blob_t *
  recycle_finalize (எழுத்து      *buf,
		    unsigned   capacity,
		    unsigned   length,
		    hb_blob_t *recycled,
		    எழுத்து      *replaced_recycled_buf)
  {
    எனில் (recycled && recycled->destroy == recycle_data_destroy)
    {
      auto *bd = (recycle_data_t *) recycled->user_data;
      எனில் (replaced_recycled_buf && replaced_recycled_buf != buf)
	hb_free (replaced_recycled_buf);
      bd->buf = buf;
      bd->capacity = capacity;
      recycled->data = (மாறிலி எழுத்து *) buf;
      recycled->length = length;
      திரும்பு recycled;
    }

    /* No recycled blob to update -- create a fresh one with our
     * destroy closure so the next recycle round can reuse it. */
    recycle_data_t *bd = (recycle_data_t *) hb_malloc (அளவு (*bd));
    எனில் (unlikely (!bd))
    {
      hb_free (buf);
      திரும்பு nullptr;
    }
    bd->buf = buf;
    bd->capacity = capacity;

    திரும்பு hb_blob_create ((மாறிலி எழுத்து *) buf, length,
			   HB_MEMORY_MODE_WRITABLE,
			   bd, recycle_data_destroy);
  }

  /* Discard @buf returned by recycle_acquire without committing to
   * a blob.  Frees @buf if it was a fresh allocation; leaves any
   * recycled buffer untouched. */
  நிலையான உள்வரி வெறுமை
  recycle_abort (எழுத்து *buf, hb_blob_t *recycled)
  {
    எனில் (!buf) திரும்பு;
    எனில் (recycled && recycled->destroy == recycle_data_destroy)
    {
      auto *bd = (recycle_data_t *) recycled->user_data;
      எனில் (buf == bd->buf) திரும்பு;  /* owned by the recycled blob */
    }
    hb_free (buf);
  }

  /* Stash @blob in @slot as the recycled output for the next
   * encode.  Destroys any previously stashed blob.  Safe to call
   * with @blob = nullptr or the empty-singleton blob (treated as
   * "drop"). */
  நிலையான உள்வரி வெறுமை
  recycle_stash (hb_blob_t **slot, hb_blob_t *blob)
  {
    hb_blob_destroy (*slot);
    *slot = nullptr;
    எனில் (!blob || blob == hb_blob_get_empty ())
      திரும்பு;
    *slot = blob;
  }
};


/*
 * hb_blob_ptr_t
 */

template <typename P>
கட்டமைப்பு hb_blob_ptr_t
{
  வகைவரை hb_remove_pointer<P> T;

  hb_blob_ptr_t (hb_blob_t *b_ = nullptr) : b (b_) {}
  hb_blob_t * operator = (hb_blob_t *b_) { திரும்பு b = b_; }
  மாறிலி T * operator -> () மாறிலி { திரும்பு get (); }
  மாறிலி T & operator * () மாறிலி  { திரும்பு *get (); }
  template <typename C> operator மாறிலி C * () மாறிலி { திரும்பு get (); }
  operator மாறிலி எழுத்து * () மாறிலி { திரும்பு (மாறிலி எழுத்து *) get (); }
  மாறிலி T * get () மாறிலி { திரும்பு b->as<T> (); }
  hb_blob_t * get_blob () மாறிலி { திரும்பு b.get_raw (); }
  unsigned முழு get_length () மாறிலி { திரும்பு b.get ()->length; }
  வெறுமை destroy () { hb_blob_destroy (b.get_raw ()); b = nullptr; }

  தனிப்பட்ட:
  hb_nonnull_ptr_t<hb_blob_t> b;
};


#endif /* HB_BLOB_HH */
