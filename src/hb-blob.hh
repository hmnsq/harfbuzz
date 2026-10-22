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

#నిర్వచితం_కాకపోతే HB_BLOB_HH
#నిర్వచించు HB_BLOB_HH

#అన్తర్భావ "hb.hh"


/*
 * hb_blob_t
 */

నిర్మాణం hb_blob_t
{
  ~hb_blob_t () { destroy_user_దత్తాంశం (); }

  శూన్య destroy_user_దత్తాంశం ()
  {
    ఒకవేళ (destroy)
    {
      destroy (user_దత్తాంశం);
      user_దత్తాంశం = శూన్యసూచిక;
      destroy = శూన్యసూచిక;
    }
  }

  శూన్య replace_బఫరు (స్థిరరాశి అక్షరం       *new_దత్తాంశం,
		       గుర్తులేని          new_నిడివి,
		       hb_జ్ఞాపకం_బహుళకం_t  new_బహుళకం,
		       శూన్య             *new_user_దత్తాంశం,
		       hb_destroy_func_t new_destroy)
  {
    ఒకవేళ (new_దత్తాంశం != దత్తాంశం)
      destroy_user_దత్తాంశం ();
    దత్తాంశం = new_దత్తాంశం;
    నిడివి = new_నిడివి;
    బహుళకం = new_బహుళకం;
    user_దత్తాంశం = new_user_దత్తాంశం;
    destroy = new_destroy;
  }

  HB_అంతర్గత తార్కిక try_make_writable ();
  HB_అంతర్గత తార్కిక try_make_writable_inplace ();
  HB_అంతర్గత తార్కిక try_make_writable_inplace_unix ();

  hb_బైట్లు_t as_బైట్లు () స్థిరరాశి { తిరిగి hb_బైట్లు_t (దత్తాంశం, నిడివి); }
  మూస <రకం_పేరు Type>
  స్థిరరాశి Type* as () స్థిరరాశి { తిరిగి as_బైట్లు ().as<Type> (); }

  బహిరంగ:
  hb_object_శీర్షిక_t శీర్షిక;

  స్థిరరాశి అక్షరం *దత్తాంశం = శూన్యసూచిక;
  గుర్తులేని పూర్ణాంకం నిడివి = 0;
  hb_జ్ఞాపకం_బహుళకం_t బహుళకం = (hb_జ్ఞాపకం_బహుళకం_t) 0;

  శూన్య *user_దత్తాంశం = శూన్యసూచిక;
  hb_destroy_func_t destroy = శూన్యసూచిక;

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

  నిర్మాణం recycle_దత్తాంశం_t
  {
    అక్షరం *buf;
    గుర్తులేని capacity;
  };

  స్థిర అంతఃవరుస శూన్య recycle_దత్తాంశం_destroy (శూన్య *user_దత్తాంశం)
  {
    స్వయంచాలక *bd = (recycle_దత్తాంశం_t *) user_దత్తాంశం;
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
  స్థిర అంతఃవరుస అక్షరం *
  recycle_acquire (hb_blob_t *recycled,
		   గుర్తులేని   needed,
		   గుర్తులేని  *బహిర్గతం_సామర్థ్యం,
		   అక్షరం     **బహిర్గతం_replaced_buf)
  {
    *బహిర్గతం_replaced_buf = శూన్యసూచిక;

    ఒకవేళ (recycled && recycled->destroy == recycle_దత్తాంశం_destroy)
    {
      స్వయంచాలక *bd = (recycle_దత్తాంశం_t *) recycled->user_దత్తాంశం;
      ఒకవేళ (bd->capacity >= needed)
      {
	*బహిర్గతం_సామర్థ్యం = bd->capacity;
	తిరిగి bd->buf;
      }
      /* Grow with a 1.5x ramp to amortize repeated growth. */
      గుర్తులేని alloc_బైట్లు = needed;
      ఒకవేళ (unlikely (hb_unsigned_కూడు_overflows (needed, needed / 2,
					       &alloc_బైట్లు)))
	alloc_బైట్లు = needed;
      అక్షరం *new_buf = (అక్షరం *) hb_realloc (bd->buf, alloc_బైట్లు);
      ఒకవేళ (new_buf)
      {
	bd->buf = new_buf;
	bd->capacity = alloc_బైట్లు;
	*బహిర్గతం_సామర్థ్యం = alloc_బైట్లు;
	తిరిగి new_buf;
      }
      /* Realloc failed.  Fall through to a fresh hb_malloc and stash
       * the old buf for the caller to free after recycle_finalize. */
      *బహిర్గతం_replaced_buf = bd->buf;
    }

    అక్షరం *buf = (అక్షరం *) hb_malloc (needed);
    ఒకవేళ (unlikely (!buf))
      తిరిగి శూన్యసూచిక;
    *బహిర్గతం_సామర్థ్యం = needed;
    తిరిగి buf;
  }

  /* Wrap @buf (of @capacity, with @length used) into an hb_blob_t.
   * If @recycled is one of our blobs, update and return it (cheap);
   * otherwise create a new blob.  Pass @replaced_recycled_buf from
   * recycle_acquire(). */
  స్థిర అంతఃవరుస hb_blob_t *
  recycle_finalize (అక్షరం      *buf,
		    గుర్తులేని   capacity,
		    గుర్తులేని   నిడివి,
		    hb_blob_t *recycled,
		    అక్షరం      *replaced_recycled_buf)
  {
    ఒకవేళ (recycled && recycled->destroy == recycle_దత్తాంశం_destroy)
    {
      స్వయంచాలక *bd = (recycle_దత్తాంశం_t *) recycled->user_దత్తాంశం;
      ఒకవేళ (replaced_recycled_buf && replaced_recycled_buf != buf)
	hb_free (replaced_recycled_buf);
      bd->buf = buf;
      bd->capacity = capacity;
      recycled->దత్తాంశం = (స్థిరరాశి అక్షరం *) buf;
      recycled->నిడివి = నిడివి;
      తిరిగి recycled;
    }

    /* No recycled blob to update -- create a fresh one with our
     * destroy closure so the next recycle round can reuse it. */
    recycle_దత్తాంశం_t *bd = (recycle_దత్తాంశం_t *) hb_malloc (పరిమాణం (*bd));
    ఒకవేళ (unlikely (!bd))
    {
      hb_free (buf);
      తిరిగి శూన్యసూచిక;
    }
    bd->buf = buf;
    bd->capacity = capacity;

    తిరిగి hb_blob_create ((స్థిరరాశి అక్షరం *) buf, నిడివి,
			   HB_జ్ఞాపకం_బహుళకం_WRITABLE,
			   bd, recycle_దత్తాంశం_destroy);
  }

  /* Discard @buf returned by recycle_acquire without committing to
   * a blob.  Frees @buf if it was a fresh allocation; leaves any
   * recycled buffer untouched. */
  స్థిర అంతఃవరుస శూన్య
  recycle_abort (అక్షరం *buf, hb_blob_t *recycled)
  {
    ఒకవేళ (!buf) తిరిగి;
    ఒకవేళ (recycled && recycled->destroy == recycle_దత్తాంశం_destroy)
    {
      స్వయంచాలక *bd = (recycle_దత్తాంశం_t *) recycled->user_దత్తాంశం;
      ఒకవేళ (buf == bd->buf) తిరిగి;  /* owned by the recycled blob */
    }
    hb_free (buf);
  }

  /* Stash @blob in @slot as the recycled output for the next
   * encode.  Destroys any previously stashed blob.  Safe to call
   * with @blob = nullptr or the empty-singleton blob (treated as
   * "drop"). */
  స్థిర అంతఃవరుస శూన్య
  recycle_stash (hb_blob_t **స్థానం, hb_blob_t *blob)
  {
    hb_blob_destroy (*స్థానం);
    *స్థానం = శూన్యసూచిక;
    ఒకవేళ (!blob || blob == hb_blob_get_ఖాళీలు ())
      తిరిగి;
    *స్థానం = blob;
  }
};


/*
 * hb_blob_ptr_t
 */

మూస <రకం_పేరు P>
నిర్మాణం hb_blob_ptr_t
{
  రకనిర్వచనం hb_remove_సూచిక<P> T;

  hb_blob_ptr_t (hb_blob_t *b_ = శూన్యసూచిక) : b (b_) {}
  hb_blob_t * క్రియాకారకం = (hb_blob_t *b_) { తిరిగి b = b_; }
  స్థిరరాశి T * క్రియాకారకం -> () స్థిరరాశి { తిరిగి get (); }
  స్థిరరాశి T & క్రియాకారకం * () స్థిరరాశి  { తిరిగి *get (); }
  మూస <రకం_పేరు C> క్రియాకారకం స్థిరరాశి C * () స్థిరరాశి { తిరిగి get (); }
  క్రియాకారకం స్థిరరాశి అక్షరం * () స్థిరరాశి { తిరిగి (స్థిరరాశి అక్షరం *) get (); }
  స్థిరరాశి T * get () స్థిరరాశి { తిరిగి b->as<T> (); }
  hb_blob_t * get_blob () స్థిరరాశి { తిరిగి b.get_raw (); }
  గుర్తులేని పూర్ణాంకం get_నిడివి () స్థిరరాశి { తిరిగి b.get ()->నిడివి; }
  శూన్య destroy () { hb_blob_destroy (b.get_raw ()); b = శూన్యసూచిక; }

  అంతరంగ:
  hb_nonnull_ptr_t<hb_blob_t> b;
};


#అయితే_ముగింపు /* HB_BLOB_HH */
