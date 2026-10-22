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

#ضمن "hb.hh"


/*
 * hb_blob_t
 */

بنية hb_blob_t
{
  ~hb_blob_t () { destroy_user_data (); }

  فراغ destroy_user_data ()
  {
    𐡄𐡍 (destroy)
    {
      destroy (user_data);
      user_data = nullptr;
      destroy = nullptr;
    }
  }

  فراغ replace_buffer (ثابت حرف       *new_data,
		       unsigned          new_length,
		       hb_memory_mode_t  new_mode,
		       فراغ             *new_user_data,
		       hb_destroy_func_t new_destroy)
  {
    𐡄𐡍 (new_data != data)
      destroy_user_data ();
    data = new_data;
    length = new_length;
    mode = new_mode;
    user_data = new_user_data;
    destroy = new_destroy;
  }

  HB_INTERNAL منطقي try_make_writable ();
  HB_INTERNAL منطقي try_make_writable_inplace ();
  HB_INTERNAL منطقي try_make_writable_inplace_unix ();

  hb_bytes_t as_bytes () ثابت { أرجع hb_bytes_t (data, length); }
  template <typename Type>
  ثابت Type* as () ثابت { أرجع as_bytes ().as<Type> (); }

  عام:
  hb_object_header_t header;

  ثابت حرف *data = nullptr;
  unsigned صحيح length = 0;
  hb_memory_mode_t mode = (hb_memory_mode_t) 0;

  فراغ *user_data = nullptr;
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

  بنية recycle_data_t
  {
    حرف *buf;
    unsigned capacity;
  };

  ساكن مضمن فراغ recycle_data_destroy (فراغ *user_data)
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
  ساكن مضمن حرف *
  recycle_acquire (hb_blob_t *recycled,
		   unsigned   needed,
		   unsigned  *out_capacity,
		   حرف     **out_replaced_buf)
  {
    *out_replaced_buf = nullptr;

    𐡄𐡍 (recycled && recycled->destroy == recycle_data_destroy)
    {
      auto *bd = (recycle_data_t *) recycled->user_data;
      𐡄𐡍 (bd->capacity >= needed)
      {
	*out_capacity = bd->capacity;
	أرجع bd->buf;
      }
      /* Grow with a 1.5x ramp to amortize repeated growth. */
      unsigned alloc_bytes = needed;
      𐡄𐡍 (unlikely (hb_unsigned_add_overflows (needed, needed / 2,
					       &alloc_bytes)))
	alloc_bytes = needed;
      حرف *new_buf = (حرف *) hb_realloc (bd->buf, alloc_bytes);
      𐡄𐡍 (new_buf)
      {
	bd->buf = new_buf;
	bd->capacity = alloc_bytes;
	*out_capacity = alloc_bytes;
	أرجع new_buf;
      }
      /* Realloc failed.  Fall through to a fresh hb_malloc and stash
       * the old buf for the caller to free after recycle_finalize. */
      *out_replaced_buf = bd->buf;
    }

    حرف *buf = (حرف *) hb_malloc (needed);
    𐡄𐡍 (unlikely (!buf))
      أرجع nullptr;
    *out_capacity = needed;
    أرجع buf;
  }

  /* Wrap @buf (of @capacity, with @length used) into an hb_blob_t.
   * If @recycled is one of our blobs, update and return it (cheap);
   * otherwise create a new blob.  Pass @replaced_recycled_buf from
   * recycle_acquire(). */
  ساكن مضمن hb_blob_t *
  recycle_finalize (حرف      *buf,
		    unsigned   capacity,
		    unsigned   length,
		    hb_blob_t *recycled,
		    حرف      *replaced_recycled_buf)
  {
    𐡄𐡍 (recycled && recycled->destroy == recycle_data_destroy)
    {
      auto *bd = (recycle_data_t *) recycled->user_data;
      𐡄𐡍 (replaced_recycled_buf && replaced_recycled_buf != buf)
	hb_free (replaced_recycled_buf);
      bd->buf = buf;
      bd->capacity = capacity;
      recycled->data = (ثابت حرف *) buf;
      recycled->length = length;
      أرجع recycled;
    }

    /* No recycled blob to update -- create a fresh one with our
     * destroy closure so the next recycle round can reuse it. */
    recycle_data_t *bd = (recycle_data_t *) hb_malloc (حجم (*bd));
    𐡄𐡍 (unlikely (!bd))
    {
      hb_free (buf);
      أرجع nullptr;
    }
    bd->buf = buf;
    bd->capacity = capacity;

    أرجع hb_blob_create ((ثابت حرف *) buf, length,
			   HB_MEMORY_MODE_WRITABLE,
			   bd, recycle_data_destroy);
  }

  /* Discard @buf returned by recycle_acquire without committing to
   * a blob.  Frees @buf if it was a fresh allocation; leaves any
   * recycled buffer untouched. */
  ساكن مضمن فراغ
  recycle_abort (حرف *buf, hb_blob_t *recycled)
  {
    𐡄𐡍 (!buf) أرجع;
    𐡄𐡍 (recycled && recycled->destroy == recycle_data_destroy)
    {
      auto *bd = (recycle_data_t *) recycled->user_data;
      𐡄𐡍 (buf == bd->buf) أرجع;  /* owned by the recycled blob */
    }
    hb_free (buf);
  }

  /* Stash @blob in @slot as the recycled output for the next
   * encode.  Destroys any previously stashed blob.  Safe to call
   * with @blob = nullptr or the empty-singleton blob (treated as
   * "drop"). */
  ساكن مضمن فراغ
  recycle_stash (hb_blob_t **slot, hb_blob_t *blob)
  {
    hb_blob_destroy (*slot);
    *slot = nullptr;
    𐡄𐡍 (!blob || blob == hb_blob_get_empty ())
      أرجع;
    *slot = blob;
  }
};


/*
 * hb_blob_ptr_t
 */

template <typename P>
بنية hb_blob_ptr_t
{
  تعريفنوع hb_remove_pointer<P> T;

  hb_blob_ptr_t (hb_blob_t *b_ = nullptr) : b (b_) {}
  hb_blob_t * operator = (hb_blob_t *b_) { أرجع b = b_; }
  ثابت T * operator -> () ثابت { أرجع get (); }
  ثابت T & operator * () ثابت  { أرجع *get (); }
  template <typename C> operator ثابت C * () ثابت { أرجع get (); }
  operator ثابت حرف * () ثابت { أرجع (ثابت حرف *) get (); }
  ثابت T * get () ثابت { أرجع b->as<T> (); }
  hb_blob_t * get_blob () ثابت { أرجع b.get_raw (); }
  unsigned صحيح get_length () ثابت { أرجع b.get ()->length; }
  فراغ destroy () { hb_blob_destroy (b.get_raw ()); b = nullptr; }

  خاص:
  hb_nonnull_ptr_t<hb_blob_t> b;
};


#endif /* HB_BLOB_HH */
