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

#अन्तर्भाव "hb.hh"


/*
 * hb_blob_t
 */

संरचना hb_blob_t
{
  ~hb_blob_t () { destroy_user_data (); }

  शून्य destroy_user_data ()
  {
    जर (destroy)
    {
      destroy (user_data);
      user_data = nullptr;
      destroy = nullptr;
    }
  }

  शून्य replace_buffer (स्थिरांक अक्षर       *new_data,
		       अचिन्हित          new_length,
		       hb_memory_mode_t  new_mode,
		       शून्य             *new_user_data,
		       hb_destroy_func_t new_destroy)
  {
    जर (new_data != data)
      destroy_user_data ();
    data = new_data;
    length = new_length;
    mode = new_mode;
    user_data = new_user_data;
    destroy = new_destroy;
  }

  HB_INTERNAL तार्किक try_make_writable ();
  HB_INTERNAL तार्किक try_make_writable_inplace ();
  HB_INTERNAL तार्किक try_make_writable_inplace_unix ();

  hb_bytes_t as_bytes () स्थिरांक { परत hb_bytes_t (data, length); }
  ढाँचा <typename Type>
  स्थिरांक Type* as () स्थिरांक { परत as_bytes ().as<Type> (); }

  सार्वजनिक:
  hb_object_header_t header;

  स्थिरांक अक्षर *data = nullptr;
  अचिन्हित पूर्णांक length = 0;
  hb_memory_mode_t mode = (hb_memory_mode_t) 0;

  शून्य *user_data = nullptr;
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

  संरचना recycle_data_t
  {
    अक्षर *buf;
    अचिन्हित capacity;
  };

  स्थिर ओळीत शून्य recycle_data_destroy (शून्य *user_data)
  {
    स्वतः *bd = (recycle_data_t *) user_data;
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
  स्थिर ओळीत अक्षर *
  recycle_acquire (hb_blob_t *recycled,
		   अचिन्हित   needed,
		   अचिन्हित  *out_capacity,
		   अक्षर     **out_replaced_buf)
  {
    *out_replaced_buf = nullptr;

    जर (recycled && recycled->destroy == recycle_data_destroy)
    {
      स्वतः *bd = (recycle_data_t *) recycled->user_data;
      जर (bd->capacity >= needed)
      {
	*out_capacity = bd->capacity;
	परत bd->buf;
      }
      /* Grow with a 1.5x ramp to amortize repeated growth. */
      अचिन्हित alloc_bytes = needed;
      जर (unlikely (hb_unsigned_add_overflows (needed, needed / 2,
					       &alloc_bytes)))
	alloc_bytes = needed;
      अक्षर *new_buf = (अक्षर *) hb_realloc (bd->buf, alloc_bytes);
      जर (new_buf)
      {
	bd->buf = new_buf;
	bd->capacity = alloc_bytes;
	*out_capacity = alloc_bytes;
	परत new_buf;
      }
      /* Realloc failed.  Fall through to a fresh hb_malloc and stash
       * the old buf for the caller to free after recycle_finalize. */
      *out_replaced_buf = bd->buf;
    }

    अक्षर *buf = (अक्षर *) hb_malloc (needed);
    जर (unlikely (!buf))
      परत nullptr;
    *out_capacity = needed;
    परत buf;
  }

  /* Wrap @buf (of @capacity, with @length used) into an hb_blob_t.
   * If @recycled is one of our blobs, update and return it (cheap);
   * otherwise create a new blob.  Pass @replaced_recycled_buf from
   * recycle_acquire(). */
  स्थिर ओळीत hb_blob_t *
  recycle_finalize (अक्षर      *buf,
		    अचिन्हित   capacity,
		    अचिन्हित   length,
		    hb_blob_t *recycled,
		    अक्षर      *replaced_recycled_buf)
  {
    जर (recycled && recycled->destroy == recycle_data_destroy)
    {
      स्वतः *bd = (recycle_data_t *) recycled->user_data;
      जर (replaced_recycled_buf && replaced_recycled_buf != buf)
	hb_free (replaced_recycled_buf);
      bd->buf = buf;
      bd->capacity = capacity;
      recycled->data = (स्थिरांक अक्षर *) buf;
      recycled->length = length;
      परत recycled;
    }

    /* No recycled blob to update -- create a fresh one with our
     * destroy closure so the next recycle round can reuse it. */
    recycle_data_t *bd = (recycle_data_t *) hb_malloc (आकार (*bd));
    जर (unlikely (!bd))
    {
      hb_free (buf);
      परत nullptr;
    }
    bd->buf = buf;
    bd->capacity = capacity;

    परत hb_blob_create ((स्थिरांक अक्षर *) buf, length,
			   HB_MEMORY_MODE_WRITABLE,
			   bd, recycle_data_destroy);
  }

  /* Discard @buf returned by recycle_acquire without committing to
   * a blob.  Frees @buf if it was a fresh allocation; leaves any
   * recycled buffer untouched. */
  स्थिर ओळीत शून्य
  recycle_abort (अक्षर *buf, hb_blob_t *recycled)
  {
    जर (!buf) परत;
    जर (recycled && recycled->destroy == recycle_data_destroy)
    {
      स्वतः *bd = (recycle_data_t *) recycled->user_data;
      जर (buf == bd->buf) परत;  /* owned by the recycled blob */
    }
    hb_free (buf);
  }

  /* Stash @blob in @slot as the recycled output for the next
   * encode.  Destroys any previously stashed blob.  Safe to call
   * with @blob = nullptr or the empty-singleton blob (treated as
   * "drop"). */
  स्थिर ओळीत शून्य
  recycle_stash (hb_blob_t **slot, hb_blob_t *blob)
  {
    hb_blob_destroy (*slot);
    *slot = nullptr;
    जर (!blob || blob == hb_blob_get_empty ())
      परत;
    *slot = blob;
  }
};


/*
 * hb_blob_ptr_t
 */

ढाँचा <typename P>
संरचना hb_blob_ptr_t
{
  प्रकारव्याख्या hb_remove_pointer<P> T;

  hb_blob_ptr_t (hb_blob_t *b_ = nullptr) : b (b_) {}
  hb_blob_t * चालक = (hb_blob_t *b_) { परत b = b_; }
  स्थिरांक T * चालक -> () स्थिरांक { परत get (); }
  स्थिरांक T & चालक * () स्थिरांक  { परत *get (); }
  ढाँचा <typename C> चालक स्थिरांक C * () स्थिरांक { परत get (); }
  चालक स्थिरांक अक्षर * () स्थिरांक { परत (स्थिरांक अक्षर *) get (); }
  स्थिरांक T * get () स्थिरांक { परत b->as<T> (); }
  hb_blob_t * get_blob () स्थिरांक { परत b.get_raw (); }
  अचिन्हित पूर्णांक get_length () स्थिरांक { परत b.get ()->length; }
  शून्य destroy () { hb_blob_destroy (b.get_raw ()); b = nullptr; }

  खाजगी:
  hb_nonnull_ptr_t<hb_blob_t> b;
};


#endif /* HB_BLOB_HH */
