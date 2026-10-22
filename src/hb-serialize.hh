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

#ifndef HB_SERIALIZE_HH
#define HB_SERIALIZE_HH

#ଅନ୍ତର୍ଭାଵ "hb.hh"
#ଅନ୍ତର୍ଭାଵ "hb-blob.hh"
#ଅନ୍ତର୍ଭାଵ "hb-map.hh"
#ଅନ୍ତର୍ଭାଵ "hb-free-pool.hh"

#ଅନ୍ତର୍ଭାଵ "hb-subset-serialize.h"

/*
 * Serialize
 */

ଗଣନା hb_serialize_error_t {
  HB_SERIALIZE_ERROR_NONE =            0x00000000u,
  HB_SERIALIZE_ERROR_OTHER =           0x00000001u,
  HB_SERIALIZE_ERROR_OFFSET_OVERFLOW = 0x00000002u,
  HB_SERIALIZE_ERROR_OUT_OF_ROOM =     0x00000004u,
  HB_SERIALIZE_ERROR_INT_OVERFLOW =    0x00000008u,
  HB_SERIALIZE_ERROR_ARRAY_OVERFLOW =  0x00000010u
};
HB_MARK_AS_FLAG_T (hb_serialize_error_t);

ସଂରଚନା hb_serialize_context_t
{
  ପ୍ରକାରବ୍ୟାଖ୍ୟା unsigned objidx_t;

  ଗଣନା whence_t {
     Head,	/* Relative to the current object head (default). */
     Tail,	/* Relative to the current object tail after packed. */
     Absolute	/* Absolute: from the start of the serialize buffer. */
   };



  ସଂରଚନା object_t
  {
    ଶୂନ୍ୟ fini () {
      real_links.fini ();
      virtual_links.fini ();
    }

    object_t () = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;

    object_t (ଅଚଳ hb_subset_serialize_object_t &o)
    {
      head = o.head;
      tail = o.tail;
      next = nullptr;
      real_links.alloc_exact (o.ଅଙ୍କ_real_links);
      ପାଇଁ (unsigned i = 0 ; i < o.ଅଙ୍କ_real_links; i++)
        real_links.push (o.real_links[i]);

      virtual_links.alloc_exact (o.ଅଙ୍କ_virtual_links);
      ପାଇଁ (unsigned i = 0; i < o.ଅଙ୍କ_virtual_links; i++)
        virtual_links.push (o.virtual_links[i]);
    }

    ତାର୍କିକ in_error () ଅଚଳ
    {
      ଫେରନ୍ତୁ real_links.in_error() || virtual_links.in_error();
    }

    ତାର୍କିକ add_virtual_link (objidx_t objidx)
    {
      ଯଦି (!objidx)
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

      auto& link = *virtual_links.push ();
      ଯଦି (virtual_links.in_error ())
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

      link.objidx = objidx;
      // Remaining fields were previously zero'd by push():
      // link.width = 0;
      // link.is_signed = 0;
      // link.whence = 0;
      // link.position = 0;
      // link.bias = 0;

      ଫେରନ୍ତୁ ସତ୍ୟ;
    }

    friend ଶୂନ୍ୟ swap (object_t& a, object_t& b) noexcept
    {
      hb_swap (a.head, b.head);
      hb_swap (a.tail, b.tail);
      hb_swap (a.next, b.next);
      hb_swap (a.real_links, b.real_links);
      hb_swap (a.virtual_links, b.virtual_links);
    }

    ତାର୍କିକ operator == (ଅଚଳ object_t &o) ଅଚଳ
    {
      // Virtual links aren't considered for equality since they don't affect the functionality
      // of the object.
      ଫେରନ୍ତୁ (tail - head == o.tail - o.head)
	  && (real_links.length == o.real_links.length)
	  && 0 == hb_memcmp (head, o.head, tail - head)
	  && real_links.as_bytes () == o.real_links.as_bytes ();
    }
    uint32_t hash () ଅଚଳ
    {
      // Virtual links aren't considered for equality since they don't affect the functionality
      // of the object.
      ଫେରନ୍ତୁ hb_bytes_t (head, hb_min (128, tail - head)).hash () ^
          real_links.as_bytes ().hash ();
    }

    ସଂରଚନା link_t
    {
      unsigned width: 3;
      unsigned is_signed: 1;
      unsigned whence: 2;
      unsigned bias : 26;
      unsigned position;
      objidx_t objidx;

      link_t () = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;

      link_t (ଅଚଳ hb_subset_serialize_link_t &o)
      {
        width = o.width;
        is_signed = 0;
        whence = 0;
        position = o.position;
        bias = 0;
        objidx = o.objidx;
      }

      HB_INTERNAL ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp (ଅଚଳ ଶୂନ୍ୟ* a, ଅଚଳ ଶୂନ୍ୟ* b)
      {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp = ((ଅଚଳ link_t*)a)->position - ((ଅଚଳ link_t*)b)->position;
        ଯଦି (cmp) ଫେରନ୍ତୁ cmp;

        ଫେରନ୍ତୁ ((ଅଚଳ link_t*)a)->objidx - ((ଅଚଳ link_t*)b)->objidx;
      }
    };

    ଅକ୍ଷର *head;
    ଅକ୍ଷର *tail;
    hb_vector_t<link_t> real_links;
    hb_vector_t<link_t> virtual_links;
    object_t *next;

    auto all_links () ଅଚଳ HB_AUTO_RETURN
        (( hb_concat (real_links, virtual_links) ));
    auto all_links_writer () HB_AUTO_RETURN
        (( hb_concat (real_links.writer (), virtual_links.writer ()) ));
  };

  ସଂରଚନା snapshot_t
  {
    ଅକ୍ଷର *head;
    ଅକ୍ଷର *tail;
    object_t *current; // Just for sanity check
    unsigned ଅଙ୍କ_real_links;
    unsigned ଅଙ୍କ_virtual_links;
    hb_serialize_error_t errors;
  };

  snapshot_t snapshot ()
  {
    ଫେରନ୍ତୁ snapshot_t {
      head, tail, current,
      current ? current->real_links.length : 0,
      current ? current->virtual_links.length : 0,
      errors
     };
  }

  hb_serialize_context_t (ଶୂନ୍ୟ *start_, size_t size) :
    start ((ଅକ୍ଷର *) start_),
    end (start + size),
    current (nullptr)
  { reset (); }
  ~hb_serialize_context_t () { fini (); }

  ଶୂନ୍ୟ fini ()
  {
    ପାଇଁ (object_t *_ : ++hb_iter (packed)) _->fini ();
    packed.fini ();
    ଏହି->packed_map.fini ();

    ଯେତେବେଳେ (current)
    {
      auto *_ = current;
      current = current->next;
      _->fini ();
    }
  }

  ତାର୍କିକ in_error () ଅଚଳ { ଫେରନ୍ତୁ ତାର୍କିକ (errors); }

  ତାର୍କିକ successful () ଅଚଳ { ଫେରନ୍ତୁ !ତାର୍କିକ (errors); }

  HB_NODISCARD ତାର୍କିକ ran_out_of_room () ଅଚଳ { ଫେରନ୍ତୁ errors & HB_SERIALIZE_ERROR_OUT_OF_ROOM; }
  HB_NODISCARD ତାର୍କିକ offset_overflow () ଅଚଳ { ଫେରନ୍ତୁ errors & HB_SERIALIZE_ERROR_OFFSET_OVERFLOW; }
  HB_NODISCARD ତାର୍କିକ only_offset_overflow () ଅଚଳ { ଫେରନ୍ତୁ errors == HB_SERIALIZE_ERROR_OFFSET_OVERFLOW; }
  HB_NODISCARD ତାର୍କିକ only_overflow () ଅଚଳ
  {
    ଫେରନ୍ତୁ errors == HB_SERIALIZE_ERROR_OFFSET_OVERFLOW
        || errors == HB_SERIALIZE_ERROR_INT_OVERFLOW
        || errors == HB_SERIALIZE_ERROR_ARRAY_OVERFLOW;
  }

  ଶୂନ୍ୟ reset (ଶୂନ୍ୟ *start_, size_t size)
  {
    start = (ଅକ୍ଷର*) start_;
    end = start + size;
    reset ();
    current = nullptr;
  }

  ଶୂନ୍ୟ reset ()
  {
    ଏହି->errors = HB_SERIALIZE_ERROR_NONE;
    ଏହି->head = ଏହି->start;
    ଏହି->tail = ଏହି->end;
    ଏହି->zerocopy = nullptr;
    ଏହି->debug_depth = 0;

    fini ();
    ଏହି->packed.push (nullptr);
    ଏହି->packed_map.init ();
  }

  ତାର୍କିକ check_success (ତାର୍କିକ success,
                      hb_serialize_error_t err_type = HB_SERIALIZE_ERROR_OTHER)
  {
    ଫେରନ୍ତୁ successful ()
        && (success || err (err_type));
  }

  template <typename T1, typename T2>
  ତାର୍କିକ check_equal (T1 &&v1, T2 &&v2, hb_serialize_error_t err_type)
  {
    ଯଦି ((long long) v1 != (long long) v2)
    {
      ଫେରନ୍ତୁ err (err_type);
    }
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  template <typename T1, typename T2>
  ତାର୍କିକ check_assign (T1 &v1, T2 &&v2, hb_serialize_error_t err_type)
  { ଫେରନ୍ତୁ check_equal (v1 = v2, v2, err_type); }

  template <typename T> ତାର୍କିକ propagate_error (T &&obj)
  { ଫେରନ୍ତୁ check_success (!hb_deref (obj).in_error ()); }

  template <typename T1, typename... Ts> ତାର୍କିକ propagate_error (T1 &&o1, Ts&&... os)
  { ଫେରନ୍ତୁ propagate_error (std::forward<T1> (o1)) &&
	   propagate_error (std::forward<Ts> (os)...); }

  /* To be called around main operation. */
  template <typename Type=ଅକ୍ଷର>
  __attribute__((returns_nonnull))
  Type *start_serialize ()
  {
    DEBUG_ସନ୍ଦେଶ_LEVEL (SERIALIZE, ଏହି->start, 0, +1,
		     "start [%p..%p] (%lu bytes)",
		     ଏହି->start, ଏହି->end,
		     (unsigned long) (ଏହି->end - ଏହି->start));

    ପ୍ରତିଜ୍ଞା (!current);
    ଫେରନ୍ତୁ push<Type> ();
  }
  ଶୂନ୍ୟ end_serialize ()
  {
    DEBUG_ସନ୍ଦେଶ_LEVEL (SERIALIZE, ଏହି->start, 0, -1,
		     "end [%p..%p] serialized %u bytes; %s",
		     ଏହି->start, ଏହି->end,
		     (unsigned) (ଏହି->head - ଏହି->start),
		     successful () ? "successful" : "UNSUCCESSFUL");

    propagate_error (packed, packed_map);

    ଯଦି (unlikely (!current)) ଫେରନ୍ତୁ;
    ଯଦି (unlikely (in_error()))
    {
      // Offset overflows that occur before link resolution cannot be handled
      // by repacking, so set a more general error.
      ଯଦି (offset_overflow ()) err (HB_SERIALIZE_ERROR_OTHER);
      ଫେରନ୍ତୁ;
    }

    ପ୍ରତିଜ୍ଞା (!current->next);

    /* Only "pack" if there exist other objects... Otherwise, don't bother.
     * Saves a move. */
    ଯଦି (packed.length <= 1)
      ଫେରନ୍ତୁ;

    କାଢ଼_pack (ମିଥ୍ୟା);

    resolve_links ();
  }

  template <typename Type = ଶୂନ୍ୟ>
  __attribute__((returns_nonnull))
  Type *push ()
  {
    ଯଦି (unlikely (in_error ())) ଫେରନ୍ତୁ start_embed<Type> ();

    object_t *obj = object_pool.alloc ();
    ଯଦି (unlikely (!obj))
      check_success (ମିଥ୍ୟା);
    ଅନ୍ୟଥା
    {
      obj->head = head;
      obj->tail = tail;
      obj->next = current;
      current = obj;
    }
    ଫେରନ୍ତୁ start_embed<Type> ();
  }
  ଶୂନ୍ୟ କାଢ଼_discard ()
  {
    object_t *obj = current;
    ଯଦି (unlikely (!obj)) ଫେରନ୍ତୁ;
    // Allow cleanup when we've error'd out on int overflows which don't compromise
    // the serializer state.
    ଯଦି (unlikely (in_error() && !only_overflow ())) ଫେରନ୍ତୁ;

    current = current->next;
    revert (zerocopy ? zerocopy : obj->head, obj->tail);
    zerocopy = nullptr;
    obj->fini ();
    object_pool.release (obj);
  }

  /* Set share to false when an object is unlikely shareable with others
   * so not worth an attempt, or a contiguous table is serialized as
   * multiple consecutive objects in the reverse order so can't be shared.
   */
  objidx_t କାଢ଼_pack (ତାର୍କିକ share=ସତ୍ୟ)
  {
    object_t *obj = current;
    ଯଦି (unlikely (!obj)) ଫେରନ୍ତୁ 0;
    // Allow cleanup when we've error'd out on int overflows which don't compromise
    // the serializer state.
    ଯଦି (unlikely (in_error()  && !only_overflow ())) ଫେରନ୍ତୁ 0;

    current = current->next;
    obj->tail = head;
    obj->next = nullptr;
    ପ୍ରତିଜ୍ଞା (obj->head <= obj->tail);
    unsigned len = obj->tail - obj->head;
    head = zerocopy ? zerocopy : obj->head; /* Rewind head. */
    ତାର୍କିକ was_zerocopy = zerocopy;
    zerocopy = nullptr;

    ଯଦି (!len)
    {
      ପ୍ରତିଜ୍ଞା (!obj->real_links.length);
      ପ୍ରତିଜ୍ଞା (!obj->virtual_links.length);
      ଫେରନ୍ତୁ 0;
    }

    objidx_t objidx;
    uint32_t hash = 0;
    ଯଦି (share)
    {
      hash = hb_hash (obj);
      objidx = packed_map.get_with_hash (obj, hash);
      ଯଦି (objidx)
      {
        merge_virtual_links (obj, objidx);
	obj->fini ();
        object_pool.release (obj);
	ଫେରନ୍ତୁ objidx;
      }
    }

    tail -= len;
    ଯଦି (was_zerocopy)
      ପ୍ରତିଜ୍ଞା (tail == obj->head);
    ଅନ୍ୟଥା
      memmove (tail, obj->head, len);

    obj->head = tail;
    obj->tail = tail + len;

    packed.push (obj);

    ଯଦି (unlikely (!propagate_error (packed)))
    {
      /* Obj wasn't successfully added to packed, so clean it up otherwise its
       * links will be leaked. When we use constructor/destructors properly, we
       * can remove these. */
      obj->fini ();
      ଫେରନ୍ତୁ 0;
    }

    objidx = packed.length - 1;

    ଯଦି (share) packed_map.set_with_hash (obj, hash, objidx);
    propagate_error (packed_map);

    ଫେରନ୍ତୁ objidx;
  }

  ଶୂନ୍ୟ revert (snapshot_t snap)
  {
    // Overflows that happened after the snapshot will be erased by the revert.
    ଯଦି (unlikely (in_error () && !only_overflow ())) ଫେରନ୍ତୁ;
    ପ୍ରତିଜ୍ଞା (snap.current == current);
    ଯଦି (current)
    {
      current->real_links.shrink (snap.ଅଙ୍କ_real_links);
      current->virtual_links.shrink (snap.ଅଙ୍କ_virtual_links);
    }
    errors = snap.errors;
    revert (snap.head, snap.tail);
  }

  ଶୂନ୍ୟ revert (ଅକ୍ଷର *snap_head,
	       ଅକ୍ଷର *snap_tail)
  {
    ଯଦି (unlikely (in_error ())) ଫେରନ୍ତୁ;
    ପ୍ରତିଜ୍ଞା (snap_head <= head);
    ପ୍ରତିଜ୍ଞା (tail <= snap_tail);
    head = snap_head;
    tail = snap_tail;
    discard_stale_objects ();
  }

  ଶୂନ୍ୟ discard_stale_objects ()
  {
    ଯଦି (unlikely (in_error ())) ଫେରନ୍ତୁ;
    ଯେତେବେଳେ (packed.length > 1 &&
	   packed.tail ()->head < tail)
    {
      object_t *obj = packed.tail ();
      packed_map.del (obj);
      ପ୍ରତିଜ୍ଞା (!obj->next);
      obj->fini ();
      object_pool.release (obj);
      packed.କାଢ଼ ();
    }
    ଯଦି (packed.length > 1)
      ପ୍ରତିଜ୍ଞା (packed.tail ()->head == tail);
  }

  // Adds a virtual link from the current object to objidx. A virtual link is not associated with
  // an actual offset field. They are solely used to enforce ordering constraints between objects.
  // Adding a virtual link from object a to object b will ensure that object b is always packed after
  // object a in the final serialized order.
  //
  // This is useful in certain situations where there needs to be a specific ordering in the
  // final serialization. Such as when platform bugs require certain orderings, or to provide
  //  guidance to the repacker for better offset overflow resolution.
  ଶୂନ୍ୟ add_virtual_link (objidx_t objidx)
  {
    ଯଦି (unlikely (in_error ())) ଫେରନ୍ତୁ;

    ଯଦି (!objidx)
      ଫେରନ୍ତୁ;

    ପ୍ରତିଜ୍ଞା (current);

    ଯଦି (!current->add_virtual_link(objidx))
      err (HB_SERIALIZE_ERROR_OTHER);
  }

  objidx_t last_added_child_index() ଅଚଳ {
    ଯଦି (unlikely (in_error ())) ଫେରନ୍ତୁ (objidx_t) -1;

    ପ୍ରତିଜ୍ଞା (current);
    ଯଦି (!ତାର୍କିକ(current->real_links)) {
      ଫେରନ୍ତୁ (objidx_t) -1;
    }

    ଫେରନ୍ତୁ current->real_links[current->real_links.length - 1].objidx;
  }

  // For the current object ensure that the sub-table bytes for child objidx are always placed
  // after the subtable bytes for any other existing children. This only ensures that the
  // repacker will not move the target subtable before the other children
  // (by adding virtual links). It is up to the caller to ensure the initial serialization
  // order is correct.
  ଶୂନ୍ୟ repack_last(objidx_t objidx) {
    ଯଦି (unlikely (in_error ())) ଫେରନ୍ତୁ;

    ଯଦି (!objidx)
      ଫେରନ୍ତୁ;

    ପ୍ରତିଜ୍ଞା (current);
    ପାଇଁ (auto& l : current->real_links) {
      ଯଦି (l.objidx == objidx) {
        ଜାରି;
      }

      packed[l.objidx]->add_virtual_link(objidx);
    }
  }

  template <typename T>
  ଶୂନ୍ୟ add_link (T &ofs, objidx_t objidx,
		 whence_t whence = Head,
		 unsigned bias = 0)
  {
    ଯଦି (unlikely (in_error ())) ଫେରନ୍ତୁ;

    ଯଦି (!objidx)
      ଫେରନ୍ତୁ;

    ପ୍ରତିଜ୍ଞା (current);
    ପ୍ରତିଜ୍ଞା (current->head <= (ଅଚଳ ଅକ୍ଷର *) &ofs);

    auto& link = *current->real_links.push ();
    ଯଦି (current->real_links.in_error ())
      err (HB_SERIALIZE_ERROR_OTHER);

    link.width = ଆକାର (T);
    link.objidx = objidx;
    ଯଦି (unlikely (!ଆକାର (T)))
    {
      // This link is not associated with an actual offset and exists merely to enforce
      // an ordering constraint.
      link.is_signed = 0;
      link.whence = 0;
      link.position = 0;
      link.bias = 0;
      ଫେରନ୍ତୁ;
    }

    link.is_signed = std::is_signed<hb_unwrap_type (T)>::value;
    link.whence = (unsigned) whence;
    link.position = (ଅଚଳ ଅକ୍ଷର *) &ofs - current->head;
    link.bias = bias;
  }

  unsigned to_bias (ଅଚଳ ଶୂନ୍ୟ *base) ଅଚଳ
  {
    ଯଦି (unlikely (in_error ())) ଫେରନ୍ତୁ 0;
    ଯଦି (!base) ଫେରନ୍ତୁ 0;
    ପ୍ରତିଜ୍ଞା (current);
    ପ୍ରତିଜ୍ଞା (current->head <= (ଅଚଳ ଅକ୍ଷର *) base);
    ଫେରନ୍ତୁ (ଅଚଳ ଅକ୍ଷର *) base - current->head;
  }

  ଶୂନ୍ୟ resolve_links ()
  {
    ଯଦି (unlikely (in_error ())) ଫେରନ୍ତୁ;

    ପ୍ରତିଜ୍ଞା (!current);
    ପ୍ରତିଜ୍ଞା (packed.length > 1);

    ପାଇଁ (ଅଚଳ object_t* parent : ++hb_iter (packed))
      ପାଇଁ (ଅଚଳ object_t::link_t &link : parent->real_links)
      {
	ଅଚଳ object_t* child = packed[link.objidx];
	ଯଦି (unlikely (!child)) { err (HB_SERIALIZE_ERROR_OTHER); ଫେରନ୍ତୁ; }
	unsigned offset = 0;
	ବାଛ ((whence_t) link.whence) {
	ସ୍ଥିତି Head:     offset = child->head - parent->head; ଭାଙ୍ଗ;
	ସ୍ଥିତି Tail:     offset = child->head - parent->tail; ଭାଙ୍ଗ;
	ସ୍ଥିତି Absolute: offset = (head - start) + (child->head - tail); ଭାଙ୍ଗ;
	}

	ପ୍ରତିଜ୍ଞା (offset >= link.bias);
	offset -= link.bias;
	ଯଦି (link.is_signed)
	{
	  ପ୍ରତିଜ୍ଞା (link.width == 2 || link.width == 4);
	  ଯଦି (link.width == 4)
	    assign_offset<int32_t> (parent, link, offset);
	  ଅନ୍ୟଥା
	    assign_offset<int16_t> (parent, link, offset);
	}
	ଅନ୍ୟଥା
	{
	  ପ୍ରତିଜ୍ଞା (link.width == 2 || link.width == 3 || link.width == 4);
	  ଯଦି (link.width == 4)
	    assign_offset<uint32_t> (parent, link, offset);
	  ଅନ୍ୟଥା ଯଦି (link.width == 3)
	    assign_offset<uint32_t, 3> (parent, link, offset);
	  ଅନ୍ୟଥା
	    assign_offset<uint16_t> (parent, link, offset);
	}
      }
  }

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା length () ଅଚଳ
  {
    ଯଦି (unlikely (!current)) ଫେରନ୍ତୁ 0;
    ଫେରନ୍ତୁ ଏହି->head - current->head;
  }

  ଶୂନ୍ୟ align (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା alignment)
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା l = length () % alignment;
    ଯଦି (l)
      (ଶୂନ୍ୟ) allocate_size<ଶୂନ୍ୟ> (alignment - l);
  }

  template <typename Type = ଶୂନ୍ୟ>
  __attribute__((returns_nonnull))
  Type *start_embed (ଅଚଳ Type *obj HB_UNUSED = nullptr) ଅଚଳ
  { ଫେରନ୍ତୁ reinterpret_cast<Type *> (ଏହି->head); }
  template <typename Type>
  __attribute__((returns_nonnull))
  Type *start_embed (ଅଚଳ Type &obj) ଅଚଳ
  { ଫେରନ୍ତୁ start_embed (std::addressof (obj)); }

  ତାର୍କିକ err (hb_serialize_error_t err_type)
  {
    ଫେରନ୍ତୁ !ତାର୍କିକ ((errors = (errors | err_type)));
  }

  ତାର୍କିକ start_zerocopy (size_t size)
  {
    ଯଦି (unlikely (in_error ())) ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ଯଦି (unlikely (size > INT_MAX || ଏହି->tail - ଏହି->head < ptrdiff_t (size)))
    {
      err (HB_SERIALIZE_ERROR_OUT_OF_ROOM);
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }

    ପ୍ରତିଜ୍ଞା (!ଏହି->zerocopy);
    ଏହି->zerocopy = ଏହି->head;

    ପ୍ରତିଜ୍ଞା (ଏହି->current->head == ଏହି->head);
    ଏହି->current->head = ଏହି->current->tail = ଏହି->head = ଏହି->tail - size;
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  template <typename Type>
  HB_NODISCARD
  Type *allocate_size (size_t size, ତାର୍କିକ clear = ସତ୍ୟ)
  {
    ଯଦି (unlikely (in_error ())) ଫେରନ୍ତୁ nullptr;

    ଯଦି (unlikely (size > INT_MAX || ଏହି->tail - ଏହି->head < ptrdiff_t (size)))
    {
      err (HB_SERIALIZE_ERROR_OUT_OF_ROOM);
      ଫେରନ୍ତୁ nullptr;
    }
    ଯଦି (clear)
      hb_memset (ଏହି->head, 0, size);
    ଅକ୍ଷର *ret = ଏହି->head;
    ଏହି->head += size;
    ଫେରନ୍ତୁ reinterpret_cast<Type *> (ret);
  }

  template <typename Type>
  Type *allocate_min ()
  { ଫେରନ୍ତୁ ଏହି->allocate_size<Type> (Type::min_size); }

  template <typename Type>
  HB_NODISCARD
  Type *embed (ଅଚଳ Type *obj)
  {
    size_t size = obj->get_size ();
    Type *ret = ଏହି->allocate_size<Type> (size, ମିଥ୍ୟା);
    ଯଦି (unlikely (!ret)) ଫେରନ୍ତୁ nullptr;
    hb_memcpy (ret, obj, size);
    ଫେରନ୍ତୁ ret;
  }
  template <typename Type>
  HB_NODISCARD
  Type *embed (ଅଚଳ Type &obj)
  { ଫେରନ୍ତୁ embed (std::addressof (obj)); }
  ଅକ୍ଷର *embed (ଅଚଳ ଅକ୍ଷର *obj, unsigned size)
  {
    ଅକ୍ଷର *ret = ଏହି->allocate_size<ଅକ୍ଷର> (size, ମିଥ୍ୟା);
    ଯଦି (unlikely (!ret)) ଫେରନ୍ତୁ nullptr;
    hb_memcpy (ret, obj, size);
    ଫେରନ୍ତୁ ret;
  }

  template <typename Type, typename ...Ts> auto
  _copy (ଅଚଳ Type &src, hb_priority<1>, Ts&&... ds) HB_RETURN
  (Type *, src.copy (ଏହି, std::forward<Ts> (ds)...))

  template <typename Type> auto
  _copy (ଅଚଳ Type &src, hb_priority<0>) -> decltype (&(hb_declval<Type> () = src))
  {
    Type *ret = ଏହି->allocate_size<Type> (ଆକାର (Type));
    ଯଦି (unlikely (!ret)) ଫେରନ୍ତୁ nullptr;
    *ret = src;
    ଫେରନ୍ତୁ ret;
  }

  /* Like embed, but active: calls obj.operator=() or obj.copy() to transfer data
   * instead of hb_memcpy(). */
  template <typename Type, typename ...Ts>
  Type *copy (ଅଚଳ Type &src, Ts&&... ds)
  { ଫେରନ୍ତୁ _copy (src, hb_prioritize, std::forward<Ts> (ds)...); }
  template <typename Type, typename ...Ts>
  Type *copy (ଅଚଳ Type *src, Ts&&... ds)
  { ଫେରନ୍ତୁ copy (*src, std::forward<Ts> (ds)...); }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator)),
	   typename ...Ts>
  ଶୂନ୍ୟ copy_all (Iterator it, Ts&&... ds)
  { ପାଇଁ (decltype (*it) _ : it) copy (_, ds...); }

  template <typename Type>
  hb_serialize_context_t& operator << (ଅଚଳ Type &obj) & { embed (obj); ଫେରନ୍ତୁ *ଏହି; }

  template <typename Type>
  Type *extend_size (Type *obj, size_t size, ତାର୍କିକ clear = ସତ୍ୟ)
  {
    ଯଦି (unlikely (in_error ())) ଫେରନ୍ତୁ nullptr;
    ଯଦି (unlikely (size >= INT_MAX)) { err (HB_SERIALIZE_ERROR_OTHER); ଫେରନ୍ତୁ nullptr; }

    ପ୍ରତିଜ୍ଞା (ଏହି->start <= (ଅକ୍ଷର *) obj);
    ପ୍ରତିଜ୍ଞା ((ଅକ୍ଷର *) obj <= ଏହି->head);
    ପ୍ରତିଜ୍ଞା ((size_t) (ଏହି->head - (ଅକ୍ଷର *) obj) <= size);
    ଯଦି (unlikely (((ଅକ୍ଷର *) obj + size < (ଅକ୍ଷର *) obj) ||
		  !ଏହି->allocate_size<Type> (((ଅକ୍ଷର *) obj) + size - ଏହି->head, clear))) ଫେରନ୍ତୁ nullptr;
    ଫେରନ୍ତୁ reinterpret_cast<Type *> (obj);
  }
  template <typename Type>
  Type *extend_size (Type &obj, size_t size, ତାର୍କିକ clear = ସତ୍ୟ)
  { ଫେରନ୍ତୁ extend_size (std::addressof (obj), size, clear); }

  template <typename Type>
  Type *extend_min (Type *obj) { ଫେରନ୍ତୁ extend_size (obj, obj->min_size); }
  template <typename Type>
  Type *extend_min (Type &obj) { ଫେରନ୍ତୁ extend_min (std::addressof (obj)); }

  template <typename Type, typename ...Ts>
  Type *extend (Type *obj, Ts&&... ds)
  { ଫେରନ୍ତୁ extend_size (obj, obj->get_size (std::forward<Ts> (ds)...)); }
  template <typename Type, typename ...Ts>
  Type *extend (Type &obj, Ts&&... ds)
  { ଫେରନ୍ତୁ extend (std::addressof (obj), std::forward<Ts> (ds)...); }

  /* Output routines. */
  hb_bytes_t copy_bytes () ଅଚଳ
  {
    ପ୍ରତିଜ୍ଞା (successful ());
    /* Copy both items from head side and tail side... */
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା len = (ଏହି->head - ଏହି->start)
		     + (ଏହି->end  - ଏହି->tail);

    // If len is zero don't hb_malloc as the memory won't get properly
    // cleaned up later.
    ଯଦି (!len) ଫେରନ୍ତୁ hb_bytes_t ();

    ଅକ୍ଷର *p = (ଅକ୍ଷର *) hb_malloc (len);
    ଯଦି (unlikely (!p)) ଫେରନ୍ତୁ hb_bytes_t ();

    hb_memcpy (p, ଏହି->start, ଏହି->head - ଏହି->start);
    hb_memcpy (p + (ଏହି->head - ଏହି->start), ଏହି->tail, ଏହି->end - ଏହି->tail);
    ଫେରନ୍ତୁ hb_bytes_t (p, len);
  }
  template <typename Type>
  Type *copy () ଅଚଳ
  { ଫେରନ୍ତୁ reinterpret_cast<Type *> ((ଅକ୍ଷର *) copy_bytes ().arrayZ); }
  hb_blob_t *copy_blob () ଅଚଳ
  {
    hb_bytes_t b = copy_bytes ();
    ଫେରନ୍ତୁ hb_blob_create (b.arrayZ, b.length,
			   HB_MEMORY_MODE_WRITABLE,
			   (ଅକ୍ଷର *) b.arrayZ, hb_free);
  }

  ଅଚଳ hb_vector_t<object_t *>& object_graph() ଅଚଳ
  { ଫେରନ୍ତୁ packed; }

  ବ୍ୟକ୍ତିଗତ:
  template <typename T, unsigned Size = ଆକାର (T)>
  ଶୂନ୍ୟ assign_offset (ଅଚଳ object_t* parent, ଅଚଳ object_t::link_t &link, unsigned offset)
  {
    // XXX We should stop assuming big-endian!
    auto &off = * ((HBInt<ସତ୍ୟ, T, Size> *) (parent->head + link.position));
    ପ୍ରତିଜ୍ଞା (0 == off);
    check_assign (off, offset, HB_SERIALIZE_ERROR_OFFSET_OVERFLOW);
  }

  ସର୍ବସାଧାରଣ:
  ଅକ୍ଷର *start, *head, *tail, *end, *zerocopy;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା debug_depth;
  hb_serialize_error_t errors;

  ବ୍ୟକ୍ତିଗତ:

  ଶୂନ୍ୟ merge_virtual_links (ଅଚଳ object_t* from, objidx_t to_idx) {
    object_t* to = packed[to_idx];
    ପାଇଁ (ଅଚଳ auto& l : from->virtual_links) {
      to->virtual_links.push (l);
    }
  }

  /* Object memory pool. */
  hb_free_pool_t<object_t> object_pool;

  /* Stack of currently under construction objects. */
  object_t *current;

  /* Stack of packed objects.  Object 0 is always nil object. */
  hb_vector_t<object_t *> packed;

  /* Map view of packed objects. */
  hb_hashmap_t<ଅଚଳ object_t *, objidx_t> packed_map;
};

#endif /* HB_SERIALIZE_HH */
