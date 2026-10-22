/*
 * Copyright © 2007,2008,2009,2010  Red Hat, Inc.
 * Copyright © 2012,2018  Google, Inc.
 * Copyright © 2019  Facebook, Inc.
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
 * Facebook Author(s): Behdad Esfahbod
 */

#నిర్వచితం_కాకపోతే HB_SERIALIZE_HH
#నిర్వచించు HB_SERIALIZE_HH

#అన్తర్భావ "hb.hh"
#అన్తర్భావ "hb-blob.hh"
#అన్తర్భావ "hb-map.hh"
#అన్తర్భావ "hb-free-pool.hh"

#అన్తర్భావ "hb-subset-serialize.h"

/*
 * Serialize
 */

గణన hb_serialize_error_t {
  HB_SERIALIZE_ERROR_NONE =            0x00000000u,
  HB_SERIALIZE_ERROR_OTHER =           0x00000001u,
  HB_SERIALIZE_ERROR_స్థానభ్రంశం_అధికప్రవాహం = 0x00000002u,
  HB_SERIALIZE_ERROR_బహిర్గతం_యొక్క_స్థలం =     0x00000004u,
  HB_SERIALIZE_ERROR_INT_అధికప్రవాహం =    0x00000008u,
  HB_SERIALIZE_ERROR_ARRAY_అధికప్రవాహం =  0x00000010u
};
HB_MARK_AS_FLAG_T (hb_serialize_error_t);

నిర్మాణం hb_serialize_context_t
{
  రకనిర్వచనం గుర్తులేని objidx_t;

  గణన whence_t {
     తల,	/* Relative to the current object head (default). */
     తోక_2,	/* Relative to the current object tail after packed. */
     Absolute	/* Absolute: from the start of the serialize buffer. */
   };



  నిర్మాణం object_t
  {
    శూన్య fini () {
      real_links.fini ();
      virtual_links.fini ();
    }

    object_t () = అప్రమేయ;

    object_t (స్థిరరాశి hb_subset_serialize_object_t &o)
    {
      తల_2 = o.తల_2;
      తోక = o.తోక;
      next = శూన్యసూచిక;
      real_links.alloc_exact (o.అంకె_real_links);
      కొరకు (గుర్తులేని i = 0 ; i < o.అంకె_real_links; i++)
        real_links.push (o.real_links[i]);

      virtual_links.alloc_exact (o.అంకె_virtual_links);
      కొరకు (గుర్తులేని i = 0; i < o.అంకె_virtual_links; i++)
        virtual_links.push (o.virtual_links[i]);
    }

    తార్కిక లోపల_error () స్థిరరాశి
    {
      తిరిగి real_links.లోపల_error() || virtual_links.లోపల_error();
    }

    తార్కిక కూడు_virtual_link (objidx_t objidx)
    {
      ఒకవేళ (!objidx)
        తిరిగి అబద్ధం;

      స్వయంచాలక& link = *virtual_links.push ();
      ఒకవేళ (virtual_links.లోపల_error ())
        తిరిగి అబద్ధం;

      link.objidx = objidx;
      // Remaining fields were previously zero'd by push():
      // link.width = 0;
      // link.is_signed = 0;
      // link.whence = 0;
      // link.position = 0;
      // link.bias = 0;

      తిరిగి నిజం;
    }

    మిత్ర శూన్య swap (object_t& a, object_t& b) noexcept
    {
      hb_మార్పిడి (a.తల_2, b.తల_2);
      hb_మార్పిడి (a.తోక, b.తోక);
      hb_మార్పిడి (a.next, b.next);
      hb_మార్పిడి (a.real_links, b.real_links);
      hb_మార్పిడి (a.virtual_links, b.virtual_links);
    }

    తార్కిక క్రియాకారకం == (స్థిరరాశి object_t &o) స్థిరరాశి
    {
      // Virtual links aren't considered for equality since they don't affect the functionality
      // of the object.
      తిరిగి (తోక - తల_2 == o.తోక - o.తల_2)
	  && (real_links.నిడివి == o.real_links.నిడివి)
	  && 0 == hb_memcmp (తల_2, o.తల_2, తోక - తల_2)
	  && real_links.as_బైట్లు () == o.real_links.as_బైట్లు ();
    }
    uint32_t hash () స్థిరరాశి
    {
      // Virtual links aren't considered for equality since they don't affect the functionality
      // of the object.
      తిరిగి hb_బైట్లు_t (తల_2, hb_min (128, తోక - తల_2)).hash () ^
          real_links.as_బైట్లు ().hash ();
    }

    నిర్మాణం link_t
    {
      గుర్తులేని వెడల్పు: 3;
      గుర్తులేని is_signed: 1;
      గుర్తులేని whence: 2;
      గుర్తులేని bias : 26;
      గుర్తులేని position;
      objidx_t objidx;

      link_t () = అప్రమేయ;

      link_t (స్థిరరాశి hb_subset_serialize_link_t &o)
      {
        వెడల్పు = o.వెడల్పు;
        is_signed = 0;
        whence = 0;
        position = o.position;
        bias = 0;
        objidx = o.objidx;
      }

      HB_అంతర్గత స్థిర పూర్ణాంకం cmp (స్థిరరాశి శూన్య* a, స్థిరరాశి శూన్య* b)
      {
        పూర్ణాంకం cmp = ((స్థిరరాశి link_t*)a)->position - ((స్థిరరాశి link_t*)b)->position;
        ఒకవేళ (cmp) తిరిగి cmp;

        తిరిగి ((స్థిరరాశి link_t*)a)->objidx - ((స్థిరరాశి link_t*)b)->objidx;
      }
    };

    అక్షరం *తల_2;
    అక్షరం *తోక;
    hb_సదిశ_t<link_t> real_links;
    hb_సదిశ_t<link_t> virtual_links;
    object_t *next;

    స్వయంచాలక all_links () స్థిరరాశి HB_AUTO_RETURN
        (( hb_concat (real_links, virtual_links) ));
    స్వయంచాలక all_links_writer () HB_AUTO_RETURN
        (( hb_concat (real_links.writer (), virtual_links.writer ()) ));
  };

  నిర్మాణం snapshot_t
  {
    అక్షరం *తల_2;
    అక్షరం *తోక;
    object_t *current; // Just for sanity check
    గుర్తులేని అంకె_real_links;
    గుర్తులేని అంకె_virtual_links;
    hb_serialize_error_t errors;
  };

  snapshot_t snapshot ()
  {
    తిరిగి snapshot_t {
      తల_2, తోక, current,
      current ? current->real_links.నిడివి : 0,
      current ? current->virtual_links.నిడివి : 0,
      errors
     };
  }

  hb_serialize_context_t (శూన్య *ప్రారంభం_2, size_t కొలత) :
    ప్రారంభం ((అక్షరం *) ప్రారంభం_2),
    చివర (ప్రారంభం + కొలత),
    current (శూన్యసూచిక)
  { reset (); }
  ~hb_serialize_context_t () { fini (); }

  శూన్య fini ()
  {
    కొరకు (object_t *_ : ++hb_iter (packed)) _->fini ();
    packed.fini ();
    ఇది->packed_map.fini ();

    అయితే (current)
    {
      స్వయంచాలక *_ = current;
      current = current->next;
      _->fini ();
    }
  }

  తార్కిక లోపల_error () స్థిరరాశి { తిరిగి తార్కిక (errors); }

  తార్కిక successful () స్థిరరాశి { తిరిగి !తార్కిక (errors); }

  HB_NODISCARD తార్కిక ran_బహిర్గతం_యొక్క_స్థలం () స్థిరరాశి { తిరిగి errors & HB_SERIALIZE_ERROR_బహిర్గతం_యొక్క_స్థలం; }
  HB_NODISCARD తార్కిక స్థానభ్రంశం_అధికప్రవాహం () స్థిరరాశి { తిరిగి errors & HB_SERIALIZE_ERROR_స్థానభ్రంశం_అధికప్రవాహం; }
  HB_NODISCARD తార్కిక only_స్థానభ్రంశం_అధికప్రవాహం () స్థిరరాశి { తిరిగి errors == HB_SERIALIZE_ERROR_స్థానభ్రంశం_అధికప్రవాహం; }
  HB_NODISCARD తార్కిక only_అధికప్రవాహం () స్థిరరాశి
  {
    తిరిగి errors == HB_SERIALIZE_ERROR_స్థానభ్రంశం_అధికప్రవాహం
        || errors == HB_SERIALIZE_ERROR_INT_అధికప్రవాహం
        || errors == HB_SERIALIZE_ERROR_ARRAY_అధికప్రవాహం;
  }

  శూన్య reset (శూన్య *ప్రారంభం_2, size_t కొలత)
  {
    ప్రారంభం = (అక్షరం*) ప్రారంభం_2;
    చివర = ప్రారంభం + కొలత;
    reset ();
    current = శూన్యసూచిక;
  }

  శూన్య reset ()
  {
    ఇది->errors = HB_SERIALIZE_ERROR_NONE;
    ఇది->తల_2 = ఇది->ప్రారంభం;
    ఇది->తోక = ఇది->చివర;
    ఇది->zerocopy = శూన్యసూచిక;
    ఇది->debug_లోతు = 0;

    fini ();
    ఇది->packed.push (శూన్యసూచిక);
    ఇది->packed_map.init ();
  }

  తార్కిక check_success (తార్కిక success,
                      hb_serialize_error_t err_type = HB_SERIALIZE_ERROR_OTHER)
  {
    తిరిగి successful ()
        && (success || err (err_type));
  }

  మూస <రకం_పేరు T1, రకం_పేరు T2>
  తార్కిక check_equal (T1 &&v1, T2 &&v2, hb_serialize_error_t err_type)
  {
    ఒకవేళ ((దీర్ఘ దీర్ఘ) v1 != (దీర్ఘ దీర్ఘ) v2)
    {
      తిరిగి err (err_type);
    }
    తిరిగి నిజం;
  }

  మూస <రకం_పేరు T1, రకం_పేరు T2>
  తార్కిక check_కేటాయించు (T1 &v1, T2 &&v2, hb_serialize_error_t err_type)
  { తిరిగి check_equal (v1 = v2, v2, err_type); }

  మూస <రకం_పేరు T> తార్కిక propagate_error (T &&obj)
  { తిరిగి check_success (!hb_deref (obj).లోపల_error ()); }

  మూస <రకం_పేరు T1, రకం_పేరు... Ts> తార్కిక propagate_error (T1 &&o1, Ts&&... os)
  { తిరిగి propagate_error (std::forward<T1> (o1)) &&
	   propagate_error (std::forward<Ts> (os)...); }

  /* To be called around main operation. */
  మూస <రకం_పేరు Type=అక్షరం>
  __attribute__((returns_nonnull))
  Type *ప్రారంభం_serialize ()
  {
    DEBUG_సందేశం_LEVEL (SERIALIZE, ఇది->ప్రారంభం, 0, +1,
		     "start [%p..%p] (%lu bytes)",
		     ఇది->ప్రారంభం, ఇది->చివర,
		     (గుర్తులేని దీర్ఘ) (ఇది->చివర - ఇది->ప్రారంభం));

    ప్రతిజ్ఞా (!current);
    తిరిగి push<Type> ();
  }
  శూన్య చివర_serialize ()
  {
    DEBUG_సందేశం_LEVEL (SERIALIZE, ఇది->ప్రారంభం, 0, -1,
		     "end [%p..%p] serialized %u bytes; %s",
		     ఇది->ప్రారంభం, ఇది->చివర,
		     (గుర్తులేని) (ఇది->తల_2 - ఇది->ప్రారంభం),
		     successful () ? "successful" : "UNSUCCESSFUL");

    propagate_error (packed, packed_map);

    ఒకవేళ (unlikely (!current)) తిరిగి;
    ఒకవేళ (unlikely (లోపల_error()))
    {
      // Offset overflows that occur before link resolution cannot be handled
      // by repacking, so set a more general error.
      ఒకవేళ (స్థానభ్రంశం_అధికప్రవాహం ()) err (HB_SERIALIZE_ERROR_OTHER);
      తిరిగి;
    }

    ప్రతిజ్ఞా (!current->next);

    /* Only "pack" if there exist other objects... Otherwise, don't bother.
     * Saves a move. */
    ఒకవేళ (packed.నిడివి <= 1)
      తిరిగి;

    తీయి_pack (అబద్ధం);

    resolve_links ();
  }

  మూస <రకం_పేరు Type = శూన్య>
  __attribute__((returns_nonnull))
  Type *push ()
  {
    ఒకవేళ (unlikely (లోపల_error ())) తిరిగి ప్రారంభం_embed<Type> ();

    object_t *obj = object_pool.alloc ();
    ఒకవేళ (unlikely (!obj))
      check_success (అబద్ధం);
    లేకపోతే
    {
      obj->తల_2 = తల_2;
      obj->తోక = తోక;
      obj->next = current;
      current = obj;
    }
    తిరిగి ప్రారంభం_embed<Type> ();
  }
  శూన్య తీయి_discard ()
  {
    object_t *obj = current;
    ఒకవేళ (unlikely (!obj)) తిరిగి;
    // Allow cleanup when we've error'd out on int overflows which don't compromise
    // the serializer state.
    ఒకవేళ (unlikely (లోపల_error() && !only_అధికప్రవాహం ())) తిరిగి;

    current = current->next;
    revert (zerocopy ? zerocopy : obj->తల_2, obj->తోక);
    zerocopy = శూన్యసూచిక;
    obj->fini ();
    object_pool.release (obj);
  }

  /* Set share to false when an object is unlikely shareable with others
   * so not worth an attempt, or a contiguous table is serialized as
   * multiple consecutive objects in the reverse order so can't be shared.
   */
  objidx_t తీయి_pack (తార్కిక share=నిజం)
  {
    object_t *obj = current;
    ఒకవేళ (unlikely (!obj)) తిరిగి 0;
    // Allow cleanup when we've error'd out on int overflows which don't compromise
    // the serializer state.
    ఒకవేళ (unlikely (లోపల_error()  && !only_అధికప్రవాహం ())) తిరిగి 0;

    current = current->next;
    obj->తోక = తల_2;
    obj->next = శూన్యసూచిక;
    ప్రతిజ్ఞా (obj->తల_2 <= obj->తోక);
    గుర్తులేని len = obj->తోక - obj->తల_2;
    తల_2 = zerocopy ? zerocopy : obj->తల_2; /* Rewind head. */
    తార్కిక was_zerocopy = zerocopy;
    zerocopy = శూన్యసూచిక;

    ఒకవేళ (!len)
    {
      ప్రతిజ్ఞా (!obj->real_links.నిడివి);
      ప్రతిజ్ఞా (!obj->virtual_links.నిడివి);
      తిరిగి 0;
    }

    objidx_t objidx;
    uint32_t hash = 0;
    ఒకవేళ (share)
    {
      hash = hb_hash (obj);
      objidx = packed_map.get_with_hash (obj, hash);
      ఒకవేళ (objidx)
      {
        విలీనం_virtual_links (obj, objidx);
	obj->fini ();
        object_pool.release (obj);
	తిరిగి objidx;
      }
    }

    తోక -= len;
    ఒకవేళ (was_zerocopy)
      ప్రతిజ్ఞా (తోక == obj->తల_2);
    లేకపోతే
      memmove (తోక, obj->తల_2, len);

    obj->తల_2 = తోక;
    obj->తోక = తోక + len;

    packed.push (obj);

    ఒకవేళ (unlikely (!propagate_error (packed)))
    {
      /* Obj wasn't successfully added to packed, so clean it up otherwise its
       * links will be leaked. When we use constructor/destructors properly, we
       * can remove these. */
      obj->fini ();
      తిరిగి 0;
    }

    objidx = packed.నిడివి - 1;

    ఒకవేళ (share) packed_map.సమితి_with_hash (obj, hash, objidx);
    propagate_error (packed_map);

    తిరిగి objidx;
  }

  శూన్య revert (snapshot_t snap)
  {
    // Overflows that happened after the snapshot will be erased by the revert.
    ఒకవేళ (unlikely (లోపల_error () && !only_అధికప్రవాహం ())) తిరిగి;
    ప్రతిజ్ఞా (snap.current == current);
    ఒకవేళ (current)
    {
      current->real_links.shrink (snap.అంకె_real_links);
      current->virtual_links.shrink (snap.అంకె_virtual_links);
    }
    errors = snap.errors;
    revert (snap.తల_2, snap.తోక);
  }

  శూన్య revert (అక్షరం *snap_తల,
	       అక్షరం *snap_తోక)
  {
    ఒకవేళ (unlikely (లోపల_error ())) తిరిగి;
    ప్రతిజ్ఞా (snap_తల <= తల_2);
    ప్రతిజ్ఞా (తోక <= snap_తోక);
    తల_2 = snap_తల;
    తోక = snap_తోక;
    discard_stale_objects ();
  }

  శూన్య discard_stale_objects ()
  {
    ఒకవేళ (unlikely (లోపల_error ())) తిరిగి;
    అయితే (packed.నిడివి > 1 &&
	   packed.తోక ()->తల_2 < తోక)
    {
      object_t *obj = packed.తోక ();
      packed_map.del (obj);
      ప్రతిజ్ఞా (!obj->next);
      obj->fini ();
      object_pool.release (obj);
      packed.తీయి ();
    }
    ఒకవేళ (packed.నిడివి > 1)
      ప్రతిజ్ఞా (packed.తోక ()->తల_2 == తోక);
  }

  // Adds a virtual link from the current object to objidx. A virtual link is not associated with
  // an actual offset field. They are solely used to enforce ordering constraints between objects.
  // Adding a virtual link from object a to object b will ensure that object b is always packed after
  // object a in the final serialized order.
  //
  // This is useful in certain situations where there needs to be a specific ordering in the
  // final serialization. Such as when platform bugs require certain orderings, or to provide
  //  guidance to the repacker for better offset overflow resolution.
  శూన్య కూడు_virtual_link (objidx_t objidx)
  {
    ఒకవేళ (unlikely (లోపల_error ())) తిరిగి;

    ఒకవేళ (!objidx)
      తిరిగి;

    ప్రతిజ్ఞా (current);

    ఒకవేళ (!current->కూడు_virtual_link(objidx))
      err (HB_SERIALIZE_ERROR_OTHER);
  }

  objidx_t చివరి_added_బిడ్డ_సూచీ() స్థిరరాశి {
    ఒకవేళ (unlikely (లోపల_error ())) తిరిగి (objidx_t) -1;

    ప్రతిజ్ఞా (current);
    ఒకవేళ (!తార్కిక(current->real_links)) {
      తిరిగి (objidx_t) -1;
    }

    తిరిగి current->real_links[current->real_links.నిడివి - 1].objidx;
  }

  // For the current object ensure that the sub-table bytes for child objidx are always placed
  // after the subtable bytes for any other existing children. This only ensures that the
  // repacker will not move the target subtable before the other children
  // (by adding virtual links). It is up to the caller to ensure the initial serialization
  // order is correct.
  శూన్య repack_చివరి(objidx_t objidx) {
    ఒకవేళ (unlikely (లోపల_error ())) తిరిగి;

    ఒకవేళ (!objidx)
      తిరిగి;

    ప్రతిజ్ఞా (current);
    కొరకు (స్వయంచాలక& l : current->real_links) {
      ఒకవేళ (l.objidx == objidx) {
        కొనసాగు;
      }

      packed[l.objidx]->కూడు_virtual_link(objidx);
    }
  }

  మూస <రకం_పేరు T>
  శూన్య కూడు_link (T &ofs, objidx_t objidx,
		 whence_t whence = తల,
		 గుర్తులేని bias = 0)
  {
    ఒకవేళ (unlikely (లోపల_error ())) తిరిగి;

    ఒకవేళ (!objidx)
      తిరిగి;

    ప్రతిజ్ఞా (current);
    ప్రతిజ్ఞా (current->తల_2 <= (స్థిరరాశి అక్షరం *) &ofs);

    స్వయంచాలక& link = *current->real_links.push ();
    ఒకవేళ (current->real_links.లోపల_error ())
      err (HB_SERIALIZE_ERROR_OTHER);

    link.వెడల్పు = పరిమాణం (T);
    link.objidx = objidx;
    ఒకవేళ (unlikely (!పరిమాణం (T)))
    {
      // This link is not associated with an actual offset and exists merely to enforce
      // an ordering constraint.
      link.is_signed = 0;
      link.whence = 0;
      link.position = 0;
      link.bias = 0;
      తిరిగి;
    }

    link.is_signed = std::is_signed<hb_unwrap_type (T)>::value;
    link.whence = (గుర్తులేని) whence;
    link.position = (స్థిరరాశి అక్షరం *) &ofs - current->తల_2;
    link.bias = bias;
  }

  గుర్తులేని గమ్యం_bias (స్థిరరాశి శూన్య *పునాది) స్థిరరాశి
  {
    ఒకవేళ (unlikely (లోపల_error ())) తిరిగి 0;
    ఒకవేళ (!పునాది) తిరిగి 0;
    ప్రతిజ్ఞా (current);
    ప్రతిజ్ఞా (current->తల_2 <= (స్థిరరాశి అక్షరం *) పునాది);
    తిరిగి (స్థిరరాశి అక్షరం *) పునాది - current->తల_2;
  }

  శూన్య resolve_links ()
  {
    ఒకవేళ (unlikely (లోపల_error ())) తిరిగి;

    ప్రతిజ్ఞా (!current);
    ప్రతిజ్ఞా (packed.నిడివి > 1);

    కొరకు (స్థిరరాశి object_t* మాతృ : ++hb_iter (packed))
      కొరకు (స్థిరరాశి object_t::link_t &link : మాతృ->real_links)
      {
	స్థిరరాశి object_t* బిడ్డ = packed[link.objidx];
	ఒకవేళ (unlikely (!బిడ్డ)) { err (HB_SERIALIZE_ERROR_OTHER); తిరిగి; }
	గుర్తులేని స్థానభ్రంశం = 0;
	ఎంపిక ((whence_t) link.whence) {
	స్థితి తల:     స్థానభ్రంశం = బిడ్డ->తల_2 - మాతృ->తల_2; ఆపు;
	స్థితి తోక_2:     స్థానభ్రంశం = బిడ్డ->తల_2 - మాతృ->తోక; ఆపు;
	స్థితి Absolute: స్థానభ్రంశం = (తల_2 - ప్రారంభం) + (బిడ్డ->తల_2 - తోక); ఆపు;
	}

	ప్రతిజ్ఞా (స్థానభ్రంశం >= link.bias);
	స్థానభ్రంశం -= link.bias;
	ఒకవేళ (link.is_signed)
	{
	  ప్రతిజ్ఞా (link.వెడల్పు == 2 || link.వెడల్పు == 4);
	  ఒకవేళ (link.వెడల్పు == 4)
	    కేటాయించు_స్థానభ్రంశం<int32_t> (మాతృ, link, స్థానభ్రంశం);
	  లేకపోతే
	    కేటాయించు_స్థానభ్రంశం<int16_t> (మాతృ, link, స్థానభ్రంశం);
	}
	లేకపోతే
	{
	  ప్రతిజ్ఞా (link.వెడల్పు == 2 || link.వెడల్పు == 3 || link.వెడల్పు == 4);
	  ఒకవేళ (link.వెడల్పు == 4)
	    కేటాయించు_స్థానభ్రంశం<uint32_t> (మాతృ, link, స్థానభ్రంశం);
	  లేకపోతే ఒకవేళ (link.వెడల్పు == 3)
	    కేటాయించు_స్థానభ్రంశం<uint32_t, 3> (మాతృ, link, స్థానభ్రంశం);
	  లేకపోతే
	    కేటాయించు_స్థానభ్రంశం<uint16_t> (మాతృ, link, స్థానభ్రంశం);
	}
      }
  }

  గుర్తులేని పూర్ణాంకం నిడివి () స్థిరరాశి
  {
    ఒకవేళ (unlikely (!current)) తిరిగి 0;
    తిరిగి ఇది->తల_2 - current->తల_2;
  }

  శూన్య align (గుర్తులేని పూర్ణాంకం alignment)
  {
    గుర్తులేని పూర్ణాంకం l = నిడివి () % alignment;
    ఒకవేళ (l)
      (శూన్య) allocate_కొలత<శూన్య> (alignment - l);
  }

  మూస <రకం_పేరు Type = శూన్య>
  __attribute__((returns_nonnull))
  Type *ప్రారంభం_embed (స్థిరరాశి Type *obj HB_వాడని = శూన్యసూచిక) స్థిరరాశి
  { తిరిగి reinterpret_cast<Type *> (ఇది->తల_2); }
  మూస <రకం_పేరు Type>
  __attribute__((returns_nonnull))
  Type *ప్రారంభం_embed (స్థిరరాశి Type &obj) స్థిరరాశి
  { తిరిగి ప్రారంభం_embed (std::addressof (obj)); }

  తార్కిక err (hb_serialize_error_t err_type)
  {
    తిరిగి !తార్కిక ((errors = (errors | err_type)));
  }

  తార్కిక ప్రారంభం_zerocopy (size_t కొలత)
  {
    ఒకవేళ (unlikely (లోపల_error ())) తిరిగి అబద్ధం;

    ఒకవేళ (unlikely (కొలత > గరిష్ఠ_పూర్ణాంకం || ఇది->తోక - ఇది->తల_2 < ptrdiff_t (కొలత)))
    {
      err (HB_SERIALIZE_ERROR_బహిర్గతం_యొక్క_స్థలం);
      తిరిగి అబద్ధం;
    }

    ప్రతిజ్ఞా (!ఇది->zerocopy);
    ఇది->zerocopy = ఇది->తల_2;

    ప్రతిజ్ఞా (ఇది->current->తల_2 == ఇది->తల_2);
    ఇది->current->తల_2 = ఇది->current->తోక = ఇది->తల_2 = ఇది->తోక - కొలత;
    తిరిగి నిజం;
  }

  మూస <రకం_పేరు Type>
  HB_NODISCARD
  Type *allocate_కొలత (size_t కొలత, తార్కిక clear = నిజం)
  {
    ఒకవేళ (unlikely (లోపల_error ())) తిరిగి శూన్యసూచిక;

    ఒకవేళ (unlikely (కొలత > గరిష్ఠ_పూర్ణాంకం || ఇది->తోక - ఇది->తల_2 < ptrdiff_t (కొలత)))
    {
      err (HB_SERIALIZE_ERROR_బహిర్గతం_యొక్క_స్థలం);
      తిరిగి శూన్యసూచిక;
    }
    ఒకవేళ (clear)
      hb_memset (ఇది->తల_2, 0, కొలత);
    అక్షరం *ret = ఇది->తల_2;
    ఇది->తల_2 += కొలత;
    తిరిగి reinterpret_cast<Type *> (ret);
  }

  మూస <రకం_పేరు Type>
  Type *allocate_min ()
  { తిరిగి ఇది->allocate_కొలత<Type> (Type::min_కొలత); }

  మూస <రకం_పేరు Type>
  HB_NODISCARD
  Type *embed (స్థిరరాశి Type *obj)
  {
    size_t కొలత = obj->get_కొలత ();
    Type *ret = ఇది->allocate_కొలత<Type> (కొలత, అబద్ధం);
    ఒకవేళ (unlikely (!ret)) తిరిగి శూన్యసూచిక;
    hb_memcpy (ret, obj, కొలత);
    తిరిగి ret;
  }
  మూస <రకం_పేరు Type>
  HB_NODISCARD
  Type *embed (స్థిరరాశి Type &obj)
  { తిరిగి embed (std::addressof (obj)); }
  అక్షరం *embed (స్థిరరాశి అక్షరం *obj, గుర్తులేని కొలత)
  {
    అక్షరం *ret = ఇది->allocate_కొలత<అక్షరం> (కొలత, అబద్ధం);
    ఒకవేళ (unlikely (!ret)) తిరిగి శూన్యసూచిక;
    hb_memcpy (ret, obj, కొలత);
    తిరిగి ret;
  }

  మూస <రకం_పేరు Type, రకం_పేరు ...Ts> స్వయంచాలక
  _copy (స్థిరరాశి Type &src, hb_ప్రాధాన్యత<1>, Ts&&... ds) HB_RETURN
  (Type *, src.copy (ఇది, std::forward<Ts> (ds)...))

  మూస <రకం_పేరు Type> స్వయంచాలక
  _copy (స్థిరరాశి Type &src, hb_ప్రాధాన్యత<0>) -> decltype (&(hb_declval<Type> () = src))
  {
    Type *ret = ఇది->allocate_కొలత<Type> (పరిమాణం (Type));
    ఒకవేళ (unlikely (!ret)) తిరిగి శూన్యసూచిక;
    *ret = src;
    తిరిగి ret;
  }

  /* Like embed, but active: calls obj.operator=() or obj.copy() to transfer data
   * instead of hb_memcpy(). */
  మూస <రకం_పేరు Type, రకం_పేరు ...Ts>
  Type *copy (స్థిరరాశి Type &src, Ts&&... ds)
  { తిరిగి _copy (src, hb_prioritize, std::forward<Ts> (ds)...); }
  మూస <రకం_పేరు Type, రకం_పేరు ...Ts>
  Type *copy (స్థిరరాశి Type *src, Ts&&... ds)
  { తిరిగి copy (*src, std::forward<Ts> (ds)...); }

  మూస<రకం_పేరు Iterator,
	   hb_requires (hb_is_iterator (Iterator)),
	   రకం_పేరు ...Ts>
  శూన్య copy_all (Iterator it, Ts&&... ds)
  { కొరకు (decltype (*it) _ : it) copy (_, ds...); }

  మూస <రకం_పేరు Type>
  hb_serialize_context_t& క్రియాకారకం << (స్థిరరాశి Type &obj) & { embed (obj); తిరిగి *ఇది; }

  మూస <రకం_పేరు Type>
  Type *extend_కొలత (Type *obj, size_t కొలత, తార్కిక clear = నిజం)
  {
    ఒకవేళ (unlikely (లోపల_error ())) తిరిగి శూన్యసూచిక;
    ఒకవేళ (unlikely (కొలత >= గరిష్ఠ_పూర్ణాంకం)) { err (HB_SERIALIZE_ERROR_OTHER); తిరిగి శూన్యసూచిక; }

    ప్రతిజ్ఞా (ఇది->ప్రారంభం <= (అక్షరం *) obj);
    ప్రతిజ్ఞా ((అక్షరం *) obj <= ఇది->తల_2);
    ప్రతిజ్ఞా ((size_t) (ఇది->తల_2 - (అక్షరం *) obj) <= కొలత);
    ఒకవేళ (unlikely (((అక్షరం *) obj + కొలత < (అక్షరం *) obj) ||
		  !ఇది->allocate_కొలత<Type> (((అక్షరం *) obj) + కొలత - ఇది->తల_2, clear))) తిరిగి శూన్యసూచిక;
    తిరిగి reinterpret_cast<Type *> (obj);
  }
  మూస <రకం_పేరు Type>
  Type *extend_కొలత (Type &obj, size_t కొలత, తార్కిక clear = నిజం)
  { తిరిగి extend_కొలత (std::addressof (obj), కొలత, clear); }

  మూస <రకం_పేరు Type>
  Type *extend_min (Type *obj) { తిరిగి extend_కొలత (obj, obj->min_కొలత); }
  మూస <రకం_పేరు Type>
  Type *extend_min (Type &obj) { తిరిగి extend_min (std::addressof (obj)); }

  మూస <రకం_పేరు Type, రకం_పేరు ...Ts>
  Type *extend (Type *obj, Ts&&... ds)
  { తిరిగి extend_కొలత (obj, obj->get_కొలత (std::forward<Ts> (ds)...)); }
  మూస <రకం_పేరు Type, రకం_పేరు ...Ts>
  Type *extend (Type &obj, Ts&&... ds)
  { తిరిగి extend (std::addressof (obj), std::forward<Ts> (ds)...); }

  /* Output routines. */
  hb_బైట్లు_t copy_బైట్లు () స్థిరరాశి
  {
    ప్రతిజ్ఞా (successful ());
    /* Copy both items from head side and tail side... */
    గుర్తులేని పూర్ణాంకం len = (ఇది->తల_2 - ఇది->ప్రారంభం)
		     + (ఇది->చివర  - ఇది->తోక);

    // If len is zero don't hb_malloc as the memory won't get properly
    // cleaned up later.
    ఒకవేళ (!len) తిరిగి hb_బైట్లు_t ();

    అక్షరం *p = (అక్షరం *) hb_malloc (len);
    ఒకవేళ (unlikely (!p)) తిరిగి hb_బైట్లు_t ();

    hb_memcpy (p, ఇది->ప్రారంభం, ఇది->తల_2 - ఇది->ప్రారంభం);
    hb_memcpy (p + (ఇది->తల_2 - ఇది->ప్రారంభం), ఇది->తోక, ఇది->చివర - ఇది->తోక);
    తిరిగి hb_బైట్లు_t (p, len);
  }
  మూస <రకం_పేరు Type>
  Type *copy () స్థిరరాశి
  { తిరిగి reinterpret_cast<Type *> ((అక్షరం *) copy_బైట్లు ().arrayZ); }
  hb_blob_t *copy_blob () స్థిరరాశి
  {
    hb_బైట్లు_t b = copy_బైట్లు ();
    తిరిగి hb_blob_create (b.arrayZ, b.నిడివి,
			   HB_జ్ఞాపకం_బహుళకం_WRITABLE,
			   (అక్షరం *) b.arrayZ, hb_free);
  }

  స్థిరరాశి hb_సదిశ_t<object_t *>& object_లేఖాచిత్రం() స్థిరరాశి
  { తిరిగి packed; }

  అంతరంగ:
  మూస <రకం_పేరు T, గుర్తులేని కొలత_2 = పరిమాణం (T)>
  శూన్య కేటాయించు_స్థానభ్రంశం (స్థిరరాశి object_t* మాతృ, స్థిరరాశి object_t::link_t &link, గుర్తులేని స్థానభ్రంశం)
  {
    // XXX We should stop assuming big-endian!
    స్వయంచాలక &off = * ((HBInt<నిజం, T, కొలత_2> *) (మాతృ->తల_2 + link.position));
    ప్రతిజ్ఞా (0 == off);
    check_కేటాయించు (off, స్థానభ్రంశం, HB_SERIALIZE_ERROR_స్థానభ్రంశం_అధికప్రవాహం);
  }

  బహిరంగ:
  అక్షరం *ప్రారంభం, *తల_2, *తోక, *చివర, *zerocopy;
  గుర్తులేని పూర్ణాంకం debug_లోతు;
  hb_serialize_error_t errors;

  అంతరంగ:

  శూన్య విలీనం_virtual_links (స్థిరరాశి object_t* from, objidx_t గమ్యం_idx) {
    object_t* to = packed[గమ్యం_idx];
    కొరకు (స్థిరరాశి స్వయంచాలక& l : from->virtual_links) {
      to->virtual_links.push (l);
    }
  }

  /* Object memory pool. */
  hb_free_pool_t<object_t> object_pool;

  /* Stack of currently under construction objects. */
  object_t *current;

  /* Stack of packed objects.  Object 0 is always nil object. */
  hb_సదిశ_t<object_t *> packed;

  /* Map view of packed objects. */
  hb_hashmap_t<స్థిరరాశి object_t *, objidx_t> packed_map;
};

#అయితే_ముగింపు /* HB_SERIALIZE_HH */
