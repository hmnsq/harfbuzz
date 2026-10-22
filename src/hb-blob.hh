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

#अगर_अघोषित HB_BLOB_HH
#घोषणा HB_BLOB_HH

#समावेश "hb.hh"


/*
 * hb_blob_t
 */

काष्ठा hb_blob_t
{
  ~hb_blob_t () { destroy_user_तथ्य (); }

  व्योम destroy_user_तथ्य ()
  {
    अगर (destroy)
    {
      destroy (user_तथ्य);
      user_तथ्य = nullptr;
      destroy = nullptr;
    }
  }

  व्योम replace_बफर (स्थिर अक्षर       *new_तथ्य,
		       अचिन्हित          new_लंबाई,
		       hb_memory_mode_t  new_mode,
		       व्योम             *new_user_तथ्य,
		       hb_destroy_func_t new_destroy)
  {
    अगर (new_तथ्य != तथ्य)
      destroy_user_तथ्य ();
    तथ्य = new_तथ्य;
    लंबाई = new_लंबाई;
    mode = new_mode;
    user_तथ्य = new_user_तथ्य;
    destroy = new_destroy;
  }

  HB_INTERNAL bool try_make_writable ();
  HB_INTERNAL bool try_make_writable_inplace ();
  HB_INTERNAL bool try_make_writable_inplace_unix ();

  hb_bytes_t as_bytes () स्थिर { वापस hb_bytes_t (तथ्य, लंबाई); }
  ढाँचा <typename Type>
  स्थिर Type* as () स्थिर { वापस as_bytes ().as<Type> (); }

  खुला:
  hb_object_header_t header;

  स्थिर अक्षर *तथ्य = nullptr;
  अचिन्हित पूर्णांक लंबाई = 0;
  hb_memory_mode_t mode = (hb_memory_mode_t) 0;

  व्योम *user_तथ्य = nullptr;
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

  काष्ठा recycle_तथ्य_t
  {
    अक्षर *buf;
    अचिन्हित capacity;
  };

  अटल अंतरभूत व्योम recycle_तथ्य_destroy (व्योम *user_तथ्य)
  {
    स्वतः *bd = (recycle_तथ्य_t *) user_तथ्य;
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
  अटल अंतरभूत अक्षर *
  recycle_acquire (hb_blob_t *recycled,
		   अचिन्हित   needed,
		   अचिन्हित  *out_capacity,
		   अक्षर     **out_replaced_buf)
  {
    *out_replaced_buf = nullptr;

    अगर (recycled && recycled->destroy == recycle_तथ्य_destroy)
    {
      स्वतः *bd = (recycle_तथ्य_t *) recycled->user_तथ्य;
      अगर (bd->capacity >= needed)
      {
	*out_capacity = bd->capacity;
	वापस bd->buf;
      }
      /* Grow with a 1.5x ramp to amortize repeated growth. */
      अचिन्हित alloc_bytes = needed;
      अगर (unlikely (hb_unsigned_add_overflows (needed, needed / 2,
					       &alloc_bytes)))
	alloc_bytes = needed;
      अक्षर *new_buf = (अक्षर *) hb_realloc (bd->buf, alloc_bytes);
      अगर (new_buf)
      {
	bd->buf = new_buf;
	bd->capacity = alloc_bytes;
	*out_capacity = alloc_bytes;
	वापस new_buf;
      }
      /* Realloc failed.  Fall through to a fresh hb_malloc and stash
       * the old buf for the caller to free after recycle_finalize. */
      *out_replaced_buf = bd->buf;
    }

    अक्षर *buf = (अक्षर *) hb_malloc (needed);
    अगर (unlikely (!buf))
      वापस nullptr;
    *out_capacity = needed;
    वापस buf;
  }

  /* Wrap @buf (of @capacity, with @length used) into an hb_blob_t.
   * If @recycled is one of our blobs, update and return it (cheap);
   * otherwise create a new blob.  Pass @replaced_recycled_buf from
   * recycle_acquire(). */
  अटल अंतरभूत hb_blob_t *
  recycle_finalize (अक्षर      *buf,
		    अचिन्हित   capacity,
		    अचिन्हित   लंबाई,
		    hb_blob_t *recycled,
		    अक्षर      *replaced_recycled_buf)
  {
    अगर (recycled && recycled->destroy == recycle_तथ्य_destroy)
    {
      स्वतः *bd = (recycle_तथ्य_t *) recycled->user_तथ्य;
      अगर (replaced_recycled_buf && replaced_recycled_buf != buf)
	hb_free (replaced_recycled_buf);
      bd->buf = buf;
      bd->capacity = capacity;
      recycled->तथ्य = (स्थिर अक्षर *) buf;
      recycled->लंबाई = लंबाई;
      वापस recycled;
    }

    /* No recycled blob to update -- create a fresh one with our
     * destroy closure so the next recycle round can reuse it. */
    recycle_तथ्य_t *bd = (recycle_तथ्य_t *) hb_malloc (माप (*bd));
    अगर (unlikely (!bd))
    {
      hb_free (buf);
      वापस nullptr;
    }
    bd->buf = buf;
    bd->capacity = capacity;

    वापस hb_blob_create ((स्थिर अक्षर *) buf, लंबाई,
			   HB_MEMORY_MODE_WRITABLE,
			   bd, recycle_तथ्य_destroy);
  }

  /* Discard @buf returned by recycle_acquire without committing to
   * a blob.  Frees @buf if it was a fresh allocation; leaves any
   * recycled buffer untouched. */
  अटल अंतरभूत व्योम
  recycle_abort (अक्षर *buf, hb_blob_t *recycled)
  {
    अगर (!buf) वापस;
    अगर (recycled && recycled->destroy == recycle_तथ्य_destroy)
    {
      स्वतः *bd = (recycle_तथ्य_t *) recycled->user_तथ्य;
      अगर (buf == bd->buf) वापस;  /* owned by the recycled blob */
    }
    hb_free (buf);
  }

  /* Stash @blob in @slot as the recycled output for the next
   * encode.  Destroys any previously stashed blob.  Safe to call
   * with @blob = nullptr or the empty-singleton blob (treated as
   * "drop"). */
  अटल अंतरभूत व्योम
  recycle_stash (hb_blob_t **slot, hb_blob_t *blob)
  {
    hb_blob_destroy (*slot);
    *slot = nullptr;
    अगर (!blob || blob == hb_blob_get_empty ())
      वापस;
    *slot = blob;
  }
};


/*
 * hb_blob_ptr_t
 */

ढाँचा <typename P>
काष्ठा hb_blob_ptr_t
{
  प्रकार hb_remove_pointer<P> T;

  hb_blob_ptr_t (hb_blob_t *b_ = nullptr) : b (b_) {}
  hb_blob_t * चालक = (hb_blob_t *b_) { वापस b = b_; }
  स्थिर T * चालक -> () स्थिर { वापस लो (); }
  स्थिर T & चालक * () स्थिर  { वापस *लो (); }
  ढाँचा <typename C> चालक स्थिर C * () स्थिर { वापस लो (); }
  चालक स्थिर अक्षर * () स्थिर { वापस (स्थिर अक्षर *) लो (); }
  स्थिर T * लो () स्थिर { वापस b->as<T> (); }
  hb_blob_t * get_blob () स्थिर { वापस b.get_raw (); }
  अचिन्हित पूर्णांक get_लंबाई () स्थिर { वापस b.लो ()->लंबाई; }
  व्योम destroy () { hb_blob_destroy (b.get_raw ()); b = nullptr; }

  निजी:
  hb_nonnull_ptr_t<hb_blob_t> b;
};


#पूर्ण_अगर /* HB_BLOB_HH */
