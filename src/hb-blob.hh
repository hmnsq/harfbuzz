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

#അന്തര്ഭാവ "hb.hh"


/*
 * hb_blob_t
 */

ഘടന hb_blob_t
{
  ~hb_blob_t () { destroy_user_data (); }

  ശൂന്യം destroy_user_data ()
  {
    എങ്കിൽ (destroy)
    {
      destroy (user_data);
      user_data = nullptr;
      destroy = nullptr;
    }
  }

  ശൂന്യം replace_buffer (സ്ഥിരം അക്ഷരം       *new_data,
		       unsigned          new_length,
		       hb_memory_mode_t  new_mode,
		       ശൂന്യം             *new_user_data,
		       hb_destroy_func_t new_destroy)
  {
    എങ്കിൽ (new_data != data)
      destroy_user_data ();
    data = new_data;
    length = new_length;
    mode = new_mode;
    user_data = new_user_data;
    destroy = new_destroy;
  }

  HB_INTERNAL തർക്കം try_make_writable ();
  HB_INTERNAL തർക്കം try_make_writable_inplace ();
  HB_INTERNAL തർക്കം try_make_writable_inplace_unix ();

  hb_bytes_t as_bytes () സ്ഥിരം { തിരികെ hb_bytes_t (data, length); }
  template <typename Type>
  സ്ഥിരം Type* as () സ്ഥിരം { തിരികെ as_bytes ().as<Type> (); }

  പൊതു:
  hb_object_header_t header;

  സ്ഥിരം അക്ഷരം *data = nullptr;
  unsigned പൂർണ്ണസംഖ്യ length = 0;
  hb_memory_mode_t mode = (hb_memory_mode_t) 0;

  ശൂന്യം *user_data = nullptr;
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

  ഘടന recycle_data_t
  {
    അക്ഷരം *buf;
    unsigned capacity;
  };

  സ്ഥിതം വരിയിൽ ശൂന്യം recycle_data_destroy (ശൂന്യം *user_data)
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
  സ്ഥിതം വരിയിൽ അക്ഷരം *
  recycle_acquire (hb_blob_t *recycled,
		   unsigned   needed,
		   unsigned  *out_capacity,
		   അക്ഷരം     **out_replaced_buf)
  {
    *out_replaced_buf = nullptr;

    എങ്കിൽ (recycled && recycled->destroy == recycle_data_destroy)
    {
      auto *bd = (recycle_data_t *) recycled->user_data;
      എങ്കിൽ (bd->capacity >= needed)
      {
	*out_capacity = bd->capacity;
	തിരികെ bd->buf;
      }
      /* Grow with a 1.5x ramp to amortize repeated growth. */
      unsigned alloc_bytes = needed;
      എങ്കിൽ (unlikely (hb_unsigned_add_overflows (needed, needed / 2,
					       &alloc_bytes)))
	alloc_bytes = needed;
      അക്ഷരം *new_buf = (അക്ഷരം *) hb_realloc (bd->buf, alloc_bytes);
      എങ്കിൽ (new_buf)
      {
	bd->buf = new_buf;
	bd->capacity = alloc_bytes;
	*out_capacity = alloc_bytes;
	തിരികെ new_buf;
      }
      /* Realloc failed.  Fall through to a fresh hb_malloc and stash
       * the old buf for the caller to free after recycle_finalize. */
      *out_replaced_buf = bd->buf;
    }

    അക്ഷരം *buf = (അക്ഷരം *) hb_malloc (needed);
    എങ്കിൽ (unlikely (!buf))
      തിരികെ nullptr;
    *out_capacity = needed;
    തിരികെ buf;
  }

  /* Wrap @buf (of @capacity, with @length used) into an hb_blob_t.
   * If @recycled is one of our blobs, update and return it (cheap);
   * otherwise create a new blob.  Pass @replaced_recycled_buf from
   * recycle_acquire(). */
  സ്ഥിതം വരിയിൽ hb_blob_t *
  recycle_finalize (അക്ഷരം      *buf,
		    unsigned   capacity,
		    unsigned   length,
		    hb_blob_t *recycled,
		    അക്ഷരം      *replaced_recycled_buf)
  {
    എങ്കിൽ (recycled && recycled->destroy == recycle_data_destroy)
    {
      auto *bd = (recycle_data_t *) recycled->user_data;
      എങ്കിൽ (replaced_recycled_buf && replaced_recycled_buf != buf)
	hb_free (replaced_recycled_buf);
      bd->buf = buf;
      bd->capacity = capacity;
      recycled->data = (സ്ഥിരം അക്ഷരം *) buf;
      recycled->length = length;
      തിരികെ recycled;
    }

    /* No recycled blob to update -- create a fresh one with our
     * destroy closure so the next recycle round can reuse it. */
    recycle_data_t *bd = (recycle_data_t *) hb_malloc (വലുപ്പം (*bd));
    എങ്കിൽ (unlikely (!bd))
    {
      hb_free (buf);
      തിരികെ nullptr;
    }
    bd->buf = buf;
    bd->capacity = capacity;

    തിരികെ hb_blob_create ((സ്ഥിരം അക്ഷരം *) buf, length,
			   HB_MEMORY_MODE_WRITABLE,
			   bd, recycle_data_destroy);
  }

  /* Discard @buf returned by recycle_acquire without committing to
   * a blob.  Frees @buf if it was a fresh allocation; leaves any
   * recycled buffer untouched. */
  സ്ഥിതം വരിയിൽ ശൂന്യം
  recycle_abort (അക്ഷരം *buf, hb_blob_t *recycled)
  {
    എങ്കിൽ (!buf) തിരികെ;
    എങ്കിൽ (recycled && recycled->destroy == recycle_data_destroy)
    {
      auto *bd = (recycle_data_t *) recycled->user_data;
      എങ്കിൽ (buf == bd->buf) തിരികെ;  /* owned by the recycled blob */
    }
    hb_free (buf);
  }

  /* Stash @blob in @slot as the recycled output for the next
   * encode.  Destroys any previously stashed blob.  Safe to call
   * with @blob = nullptr or the empty-singleton blob (treated as
   * "drop"). */
  സ്ഥിതം വരിയിൽ ശൂന്യം
  recycle_stash (hb_blob_t **slot, hb_blob_t *blob)
  {
    hb_blob_destroy (*slot);
    *slot = nullptr;
    എങ്കിൽ (!blob || blob == hb_blob_get_empty ())
      തിരികെ;
    *slot = blob;
  }
};


/*
 * hb_blob_ptr_t
 */

template <typename P>
ഘടന hb_blob_ptr_t
{
  തരനിർവചനം hb_remove_pointer<P> T;

  hb_blob_ptr_t (hb_blob_t *b_ = nullptr) : b (b_) {}
  hb_blob_t * operator = (hb_blob_t *b_) { തിരികെ b = b_; }
  സ്ഥിരം T * operator -> () സ്ഥിരം { തിരികെ get (); }
  സ്ഥിരം T & operator * () സ്ഥിരം  { തിരികെ *get (); }
  template <typename C> operator സ്ഥിരം C * () സ്ഥിരം { തിരികെ get (); }
  operator സ്ഥിരം അക്ഷരം * () സ്ഥിരം { തിരികെ (സ്ഥിരം അക്ഷരം *) get (); }
  സ്ഥിരം T * get () സ്ഥിരം { തിരികെ b->as<T> (); }
  hb_blob_t * get_blob () സ്ഥിരം { തിരികെ b.get_raw (); }
  unsigned പൂർണ്ണസംഖ്യ get_length () സ്ഥിരം { തിരികെ b.get ()->length; }
  ശൂന്യം destroy () { hb_blob_destroy (b.get_raw ()); b = nullptr; }

  സ്വകാര്യ:
  hb_nonnull_ptr_t<hb_blob_t> b;
};


#endif /* HB_BLOB_HH */
