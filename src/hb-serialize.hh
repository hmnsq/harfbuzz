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

#ಅನ್ತರ್ಭಾವ "hb.hh"
#ಅನ್ತರ್ಭಾವ "hb-blob.hh"
#ಅನ್ತರ್ಭಾವ "hb-map.hh"
#ಅನ್ತರ್ಭಾವ "hb-free-pool.hh"

#ಅನ್ತರ್ಭಾವ "hb-subset-serialize.h"

/*
 * Serialize
 */

ಗಣನಾ hb_serialize_error_t {
  HB_SERIALIZE_ERROR_NONE =            0x00000000u,
  HB_SERIALIZE_ERROR_OTHER =           0x00000001u,
  HB_SERIALIZE_ERROR_OFFSET_OVERFLOW = 0x00000002u,
  HB_SERIALIZE_ERROR_OUT_OF_ROOM =     0x00000004u,
  HB_SERIALIZE_ERROR_INT_OVERFLOW =    0x00000008u,
  HB_SERIALIZE_ERROR_ARRAY_OVERFLOW =  0x00000010u
};
HB_MARK_AS_FLAG_T (hb_serialize_error_t);

ಸಂರಚನಾ hb_serialize_context_t
{
  ಪ್ರಕಾರನಿರ್ದೇಶ unsigned objidx_t;

  ಗಣನಾ whence_t {
     Head,	/* Relative to the current object head (default). */
     Tail,	/* Relative to the current object tail after packed. */
     Absolute	/* Absolute: from the start of the serialize buffer. */
   };



  ಸಂರಚನಾ object_t
  {
    ಶೂನ್ಯ fini () {
      real_links.fini ();
      virtual_links.fini ();
    }

    object_t () = ಸ್ವಭಾವ;

    object_t (ಧ್ರುವ hb_subset_serialize_object_t &o)
    {
      head = o.head;
      tail = o.tail;
      next = nullptr;
      real_links.alloc_exact (o.ಅಂಕೆ_real_links);
      ಪ್ರತಿ (unsigned i = 0 ; i < o.ಅಂಕೆ_real_links; i++)
        real_links.push (o.real_links[i]);

      virtual_links.alloc_exact (o.ಅಂಕೆ_virtual_links);
      ಪ್ರತಿ (unsigned i = 0; i < o.ಅಂಕೆ_virtual_links; i++)
        virtual_links.push (o.virtual_links[i]);
    }

    ತಾರ್ಕಿಕ in_error () ಧ್ರುವ
    {
      ಪ್ರತ್ಯಾಗಮ real_links.in_error() || virtual_links.in_error();
    }

    ತಾರ್ಕಿಕ add_virtual_link (objidx_t objidx)
    {
      ಯದಿ (!objidx)
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

      auto& link = *virtual_links.push ();
      ಯದಿ (virtual_links.in_error ())
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

      link.objidx = objidx;
      // Remaining fields were previously zero'd by push():
      // link.width = 0;
      // link.is_signed = 0;
      // link.whence = 0;
      // link.position = 0;
      // link.bias = 0;

      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }

    friend ಶೂನ್ಯ swap (object_t& a, object_t& b) noexcept
    {
      hb_swap (a.head, b.head);
      hb_swap (a.tail, b.tail);
      hb_swap (a.next, b.next);
      hb_swap (a.real_links, b.real_links);
      hb_swap (a.virtual_links, b.virtual_links);
    }

    ತಾರ್ಕಿಕ operator == (ಧ್ರುವ object_t &o) ಧ್ರುವ
    {
      // Virtual links aren't considered for equality since they don't affect the functionality
      // of the object.
      ಪ್ರತ್ಯಾಗಮ (tail - head == o.tail - o.head)
	  && (real_links.length == o.real_links.length)
	  && 0 == hb_memcmp (head, o.head, tail - head)
	  && real_links.as_bytes () == o.real_links.as_bytes ();
    }
    uint32_t hash () ಧ್ರುವ
    {
      // Virtual links aren't considered for equality since they don't affect the functionality
      // of the object.
      ಪ್ರತ್ಯಾಗಮ hb_bytes_t (head, hb_min (128, tail - head)).hash () ^
          real_links.as_bytes ().hash ();
    }

    ಸಂರಚನಾ link_t
    {
      unsigned width: 3;
      unsigned is_signed: 1;
      unsigned whence: 2;
      unsigned bias : 26;
      unsigned position;
      objidx_t objidx;

      link_t () = ಸ್ವಭಾವ;

      link_t (ಧ್ರುವ hb_subset_serialize_link_t &o)
      {
        width = o.width;
        is_signed = 0;
        whence = 0;
        position = o.position;
        bias = 0;
        objidx = o.objidx;
      }

      HB_INTERNAL ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cmp (ಧ್ರುವ ಶೂನ್ಯ* a, ಧ್ರುವ ಶೂನ್ಯ* b)
      {
        ಪೂರ್ಣಾಙ್ಕ cmp = ((ಧ್ರುವ link_t*)a)->position - ((ಧ್ರುವ link_t*)b)->position;
        ಯದಿ (cmp) ಪ್ರತ್ಯಾಗಮ cmp;

        ಪ್ರತ್ಯಾಗಮ ((ಧ್ರುವ link_t*)a)->objidx - ((ಧ್ರುವ link_t*)b)->objidx;
      }
    };

    ವರ್ಣ *head;
    ವರ್ಣ *tail;
    hb_vector_t<link_t> real_links;
    hb_vector_t<link_t> virtual_links;
    object_t *next;

    auto all_links () ಧ್ರುವ HB_AUTO_RETURN
        (( hb_concat (real_links, virtual_links) ));
    auto all_links_writer () HB_AUTO_RETURN
        (( hb_concat (real_links.writer (), virtual_links.writer ()) ));
  };

  ಸಂರಚನಾ snapshot_t
  {
    ವರ್ಣ *head;
    ವರ್ಣ *tail;
    object_t *current; // Just for sanity check
    unsigned ಅಂಕೆ_real_links;
    unsigned ಅಂಕೆ_virtual_links;
    hb_serialize_error_t errors;
  };

  snapshot_t snapshot ()
  {
    ಪ್ರತ್ಯಾಗಮ snapshot_t {
      head, tail, current,
      current ? current->real_links.length : 0,
      current ? current->virtual_links.length : 0,
      errors
     };
  }

  hb_serialize_context_t (ಶೂನ್ಯ *start_, size_t size) :
    start ((ವರ್ಣ *) start_),
    end (start + size),
    current (nullptr)
  { reset (); }
  ~hb_serialize_context_t () { fini (); }

  ಶೂನ್ಯ fini ()
  {
    ಪ್ರತಿ (object_t *_ : ++hb_iter (packed)) _->fini ();
    packed.fini ();
    ಏತತ್->packed_map.fini ();

    ಯಾವತ್ (current)
    {
      auto *_ = current;
      current = current->next;
      _->fini ();
    }
  }

  ತಾರ್ಕಿಕ in_error () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ತಾರ್ಕಿಕ (errors); }

  ತಾರ್ಕಿಕ successful () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ !ತಾರ್ಕಿಕ (errors); }

  HB_NODISCARD ತಾರ್ಕಿಕ ran_out_of_room () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ errors & HB_SERIALIZE_ERROR_OUT_OF_ROOM; }
  HB_NODISCARD ತಾರ್ಕಿಕ offset_overflow () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ errors & HB_SERIALIZE_ERROR_OFFSET_OVERFLOW; }
  HB_NODISCARD ತಾರ್ಕಿಕ only_offset_overflow () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ errors == HB_SERIALIZE_ERROR_OFFSET_OVERFLOW; }
  HB_NODISCARD ತಾರ್ಕಿಕ only_overflow () ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ errors == HB_SERIALIZE_ERROR_OFFSET_OVERFLOW
        || errors == HB_SERIALIZE_ERROR_INT_OVERFLOW
        || errors == HB_SERIALIZE_ERROR_ARRAY_OVERFLOW;
  }

  ಶೂನ್ಯ reset (ಶೂನ್ಯ *start_, size_t size)
  {
    start = (ವರ್ಣ*) start_;
    end = start + size;
    reset ();
    current = nullptr;
  }

  ಶೂನ್ಯ reset ()
  {
    ಏತತ್->errors = HB_SERIALIZE_ERROR_NONE;
    ಏತತ್->head = ಏತತ್->start;
    ಏತತ್->tail = ಏತತ್->end;
    ಏತತ್->zerocopy = nullptr;
    ಏತತ್->debug_depth = 0;

    fini ();
    ಏತತ್->packed.push (nullptr);
    ಏತತ್->packed_map.init ();
  }

  ತಾರ್ಕಿಕ check_success (ತಾರ್ಕಿಕ success,
                      hb_serialize_error_t err_type = HB_SERIALIZE_ERROR_OTHER)
  {
    ಪ್ರತ್ಯಾಗಮ successful ()
        && (success || err (err_type));
  }

  template <typename T1, typename T2>
  ತಾರ್ಕಿಕ check_equal (T1 &&v1, T2 &&v2, hb_serialize_error_t err_type)
  {
    ಯದಿ ((long long) v1 != (long long) v2)
    {
      ಪ್ರತ್ಯಾಗಮ err (err_type);
    }
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  template <typename T1, typename T2>
  ತಾರ್ಕಿಕ check_assign (T1 &v1, T2 &&v2, hb_serialize_error_t err_type)
  { ಪ್ರತ್ಯಾಗಮ check_equal (v1 = v2, v2, err_type); }

  template <typename T> ತಾರ್ಕಿಕ propagate_error (T &&obj)
  { ಪ್ರತ್ಯಾಗಮ check_success (!hb_deref (obj).in_error ()); }

  template <typename T1, typename... Ts> ತಾರ್ಕಿಕ propagate_error (T1 &&o1, Ts&&... os)
  { ಪ್ರತ್ಯಾಗಮ propagate_error (std::forward<T1> (o1)) &&
	   propagate_error (std::forward<Ts> (os)...); }

  /* To be called around main operation. */
  template <typename Type=ವರ್ಣ>
  __attribute__((returns_nonnull))
  Type *start_serialize ()
  {
    DEBUG_ಸಂದೇಶ_LEVEL (SERIALIZE, ಏತತ್->start, 0, +1,
		     "start [%p..%p] (%lu bytes)",
		     ಏತತ್->start, ಏತತ್->end,
		     (unsigned long) (ಏತತ್->end - ಏತತ್->start));

    ಪ್ರತಿಜ್ಞಾ (!current);
    ಪ್ರತ್ಯಾಗಮ push<Type> ();
  }
  ಶೂನ್ಯ end_serialize ()
  {
    DEBUG_ಸಂದೇಶ_LEVEL (SERIALIZE, ಏತತ್->start, 0, -1,
		     "end [%p..%p] serialized %u bytes; %s",
		     ಏತತ್->start, ಏತತ್->end,
		     (unsigned) (ಏತತ್->head - ಏತತ್->start),
		     successful () ? "successful" : "UNSUCCESSFUL");

    propagate_error (packed, packed_map);

    ಯದಿ (unlikely (!current)) ಪ್ರತ್ಯಾಗಮ;
    ಯದಿ (unlikely (in_error()))
    {
      // Offset overflows that occur before link resolution cannot be handled
      // by repacking, so set a more general error.
      ಯದಿ (offset_overflow ()) err (HB_SERIALIZE_ERROR_OTHER);
      ಪ್ರತ್ಯಾಗಮ;
    }

    ಪ್ರತಿಜ್ಞಾ (!current->next);

    /* Only "pack" if there exist other objects... Otherwise, don't bother.
     * Saves a move. */
    ಯದಿ (packed.length <= 1)
      ಪ್ರತ್ಯಾಗಮ;

    ತೆಗೆ_pack (ಅಸತ್ಯ);

    resolve_links ();
  }

  template <typename Type = ಶೂನ್ಯ>
  __attribute__((returns_nonnull))
  Type *push ()
  {
    ಯದಿ (unlikely (in_error ())) ಪ್ರತ್ಯಾಗಮ start_embed<Type> ();

    object_t *obj = object_pool.alloc ();
    ಯದಿ (unlikely (!obj))
      check_success (ಅಸತ್ಯ);
    ಅನ್ಯಥಾ
    {
      obj->head = head;
      obj->tail = tail;
      obj->next = current;
      current = obj;
    }
    ಪ್ರತ್ಯಾಗಮ start_embed<Type> ();
  }
  ಶೂನ್ಯ ತೆಗೆ_discard ()
  {
    object_t *obj = current;
    ಯದಿ (unlikely (!obj)) ಪ್ರತ್ಯಾಗಮ;
    // Allow cleanup when we've error'd out on int overflows which don't compromise
    // the serializer state.
    ಯದಿ (unlikely (in_error() && !only_overflow ())) ಪ್ರತ್ಯಾಗಮ;

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
  objidx_t ತೆಗೆ_pack (ತಾರ್ಕಿಕ share=ಸತ್ಯ)
  {
    object_t *obj = current;
    ಯದಿ (unlikely (!obj)) ಪ್ರತ್ಯಾಗಮ 0;
    // Allow cleanup when we've error'd out on int overflows which don't compromise
    // the serializer state.
    ಯದಿ (unlikely (in_error()  && !only_overflow ())) ಪ್ರತ್ಯಾಗಮ 0;

    current = current->next;
    obj->tail = head;
    obj->next = nullptr;
    ಪ್ರತಿಜ್ಞಾ (obj->head <= obj->tail);
    unsigned len = obj->tail - obj->head;
    head = zerocopy ? zerocopy : obj->head; /* Rewind head. */
    ತಾರ್ಕಿಕ was_zerocopy = zerocopy;
    zerocopy = nullptr;

    ಯದಿ (!len)
    {
      ಪ್ರತಿಜ್ಞಾ (!obj->real_links.length);
      ಪ್ರತಿಜ್ಞಾ (!obj->virtual_links.length);
      ಪ್ರತ್ಯಾಗಮ 0;
    }

    objidx_t objidx;
    uint32_t hash = 0;
    ಯದಿ (share)
    {
      hash = hb_hash (obj);
      objidx = packed_map.get_with_hash (obj, hash);
      ಯದಿ (objidx)
      {
        merge_virtual_links (obj, objidx);
	obj->fini ();
        object_pool.release (obj);
	ಪ್ರತ್ಯಾಗಮ objidx;
      }
    }

    tail -= len;
    ಯದಿ (was_zerocopy)
      ಪ್ರತಿಜ್ಞಾ (tail == obj->head);
    ಅನ್ಯಥಾ
      memmove (tail, obj->head, len);

    obj->head = tail;
    obj->tail = tail + len;

    packed.push (obj);

    ಯದಿ (unlikely (!propagate_error (packed)))
    {
      /* Obj wasn't successfully added to packed, so clean it up otherwise its
       * links will be leaked. When we use constructor/destructors properly, we
       * can remove these. */
      obj->fini ();
      ಪ್ರತ್ಯಾಗಮ 0;
    }

    objidx = packed.length - 1;

    ಯದಿ (share) packed_map.set_with_hash (obj, hash, objidx);
    propagate_error (packed_map);

    ಪ್ರತ್ಯಾಗಮ objidx;
  }

  ಶೂನ್ಯ revert (snapshot_t snap)
  {
    // Overflows that happened after the snapshot will be erased by the revert.
    ಯದಿ (unlikely (in_error () && !only_overflow ())) ಪ್ರತ್ಯಾಗಮ;
    ಪ್ರತಿಜ್ಞಾ (snap.current == current);
    ಯದಿ (current)
    {
      current->real_links.shrink (snap.ಅಂಕೆ_real_links);
      current->virtual_links.shrink (snap.ಅಂಕೆ_virtual_links);
    }
    errors = snap.errors;
    revert (snap.head, snap.tail);
  }

  ಶೂನ್ಯ revert (ವರ್ಣ *snap_head,
	       ವರ್ಣ *snap_tail)
  {
    ಯದಿ (unlikely (in_error ())) ಪ್ರತ್ಯಾಗಮ;
    ಪ್ರತಿಜ್ಞಾ (snap_head <= head);
    ಪ್ರತಿಜ್ಞಾ (tail <= snap_tail);
    head = snap_head;
    tail = snap_tail;
    discard_stale_objects ();
  }

  ಶೂನ್ಯ discard_stale_objects ()
  {
    ಯದಿ (unlikely (in_error ())) ಪ್ರತ್ಯಾಗಮ;
    ಯಾವತ್ (packed.length > 1 &&
	   packed.tail ()->head < tail)
    {
      object_t *obj = packed.tail ();
      packed_map.del (obj);
      ಪ್ರತಿಜ್ಞಾ (!obj->next);
      obj->fini ();
      object_pool.release (obj);
      packed.ತೆಗೆ ();
    }
    ಯದಿ (packed.length > 1)
      ಪ್ರತಿಜ್ಞಾ (packed.tail ()->head == tail);
  }

  // Adds a virtual link from the current object to objidx. A virtual link is not associated with
  // an actual offset field. They are solely used to enforce ordering constraints between objects.
  // Adding a virtual link from object a to object b will ensure that object b is always packed after
  // object a in the final serialized order.
  //
  // This is useful in certain situations where there needs to be a specific ordering in the
  // final serialization. Such as when platform bugs require certain orderings, or to provide
  //  guidance to the repacker for better offset overflow resolution.
  ಶೂನ್ಯ add_virtual_link (objidx_t objidx)
  {
    ಯದಿ (unlikely (in_error ())) ಪ್ರತ್ಯಾಗಮ;

    ಯದಿ (!objidx)
      ಪ್ರತ್ಯಾಗಮ;

    ಪ್ರತಿಜ್ಞಾ (current);

    ಯದಿ (!current->add_virtual_link(objidx))
      err (HB_SERIALIZE_ERROR_OTHER);
  }

  objidx_t last_added_child_index() ಧ್ರುವ {
    ಯದಿ (unlikely (in_error ())) ಪ್ರತ್ಯಾಗಮ (objidx_t) -1;

    ಪ್ರತಿಜ್ಞಾ (current);
    ಯದಿ (!ತಾರ್ಕಿಕ(current->real_links)) {
      ಪ್ರತ್ಯಾಗಮ (objidx_t) -1;
    }

    ಪ್ರತ್ಯಾಗಮ current->real_links[current->real_links.length - 1].objidx;
  }

  // For the current object ensure that the sub-table bytes for child objidx are always placed
  // after the subtable bytes for any other existing children. This only ensures that the
  // repacker will not move the target subtable before the other children
  // (by adding virtual links). It is up to the caller to ensure the initial serialization
  // order is correct.
  ಶೂನ್ಯ repack_last(objidx_t objidx) {
    ಯದಿ (unlikely (in_error ())) ಪ್ರತ್ಯಾಗಮ;

    ಯದಿ (!objidx)
      ಪ್ರತ್ಯಾಗಮ;

    ಪ್ರತಿಜ್ಞಾ (current);
    ಪ್ರತಿ (auto& l : current->real_links) {
      ಯದಿ (l.objidx == objidx) {
        ಅನುವರ್ತ;
      }

      packed[l.objidx]->add_virtual_link(objidx);
    }
  }

  template <typename T>
  ಶೂನ್ಯ add_link (T &ofs, objidx_t objidx,
		 whence_t whence = Head,
		 unsigned bias = 0)
  {
    ಯದಿ (unlikely (in_error ())) ಪ್ರತ್ಯಾಗಮ;

    ಯದಿ (!objidx)
      ಪ್ರತ್ಯಾಗಮ;

    ಪ್ರತಿಜ್ಞಾ (current);
    ಪ್ರತಿಜ್ಞಾ (current->head <= (ಧ್ರುವ ವರ್ಣ *) &ofs);

    auto& link = *current->real_links.push ();
    ಯದಿ (current->real_links.in_error ())
      err (HB_SERIALIZE_ERROR_OTHER);

    link.width = ಪರಿಮಾಣ (T);
    link.objidx = objidx;
    ಯದಿ (unlikely (!ಪರಿಮಾಣ (T)))
    {
      // This link is not associated with an actual offset and exists merely to enforce
      // an ordering constraint.
      link.is_signed = 0;
      link.whence = 0;
      link.position = 0;
      link.bias = 0;
      ಪ್ರತ್ಯಾಗಮ;
    }

    link.is_signed = std::is_signed<hb_unwrap_type (T)>::value;
    link.whence = (unsigned) whence;
    link.position = (ಧ್ರುವ ವರ್ಣ *) &ofs - current->head;
    link.bias = bias;
  }

  unsigned to_bias (ಧ್ರುವ ಶೂನ್ಯ *base) ಧ್ರುವ
  {
    ಯದಿ (unlikely (in_error ())) ಪ್ರತ್ಯಾಗಮ 0;
    ಯದಿ (!base) ಪ್ರತ್ಯಾಗಮ 0;
    ಪ್ರತಿಜ್ಞಾ (current);
    ಪ್ರತಿಜ್ಞಾ (current->head <= (ಧ್ರುವ ವರ್ಣ *) base);
    ಪ್ರತ್ಯಾಗಮ (ಧ್ರುವ ವರ್ಣ *) base - current->head;
  }

  ಶೂನ್ಯ resolve_links ()
  {
    ಯದಿ (unlikely (in_error ())) ಪ್ರತ್ಯಾಗಮ;

    ಪ್ರತಿಜ್ಞಾ (!current);
    ಪ್ರತಿಜ್ಞಾ (packed.length > 1);

    ಪ್ರತಿ (ಧ್ರುವ object_t* parent : ++hb_iter (packed))
      ಪ್ರತಿ (ಧ್ರುವ object_t::link_t &link : parent->real_links)
      {
	ಧ್ರುವ object_t* child = packed[link.objidx];
	ಯದಿ (unlikely (!child)) { err (HB_SERIALIZE_ERROR_OTHER); ಪ್ರತ್ಯಾಗಮ; }
	unsigned offset = 0;
	ವಿಕಲ್ಪ ((whence_t) link.whence) {
	ಪಕ್ಷ Head:     offset = child->head - parent->head; ಭಙ್ಗ;
	ಪಕ್ಷ Tail:     offset = child->head - parent->tail; ಭಙ್ಗ;
	ಪಕ್ಷ Absolute: offset = (head - start) + (child->head - tail); ಭಙ್ಗ;
	}

	ಪ್ರತಿಜ್ಞಾ (offset >= link.bias);
	offset -= link.bias;
	ಯದಿ (link.is_signed)
	{
	  ಪ್ರತಿಜ್ಞಾ (link.width == 2 || link.width == 4);
	  ಯದಿ (link.width == 4)
	    assign_offset<int32_t> (parent, link, offset);
	  ಅನ್ಯಥಾ
	    assign_offset<int16_t> (parent, link, offset);
	}
	ಅನ್ಯಥಾ
	{
	  ಪ್ರತಿಜ್ಞಾ (link.width == 2 || link.width == 3 || link.width == 4);
	  ಯದಿ (link.width == 4)
	    assign_offset<uint32_t> (parent, link, offset);
	  ಅನ್ಯಥಾ ಯದಿ (link.width == 3)
	    assign_offset<uint32_t, 3> (parent, link, offset);
	  ಅನ್ಯಥಾ
	    assign_offset<uint16_t> (parent, link, offset);
	}
      }
  }

  unsigned ಪೂರ್ಣಾಙ್ಕ length () ಧ್ರುವ
  {
    ಯದಿ (unlikely (!current)) ಪ್ರತ್ಯಾಗಮ 0;
    ಪ್ರತ್ಯಾಗಮ ಏತತ್->head - current->head;
  }

  ಶೂನ್ಯ align (unsigned ಪೂರ್ಣಾಙ್ಕ alignment)
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ l = length () % alignment;
    ಯದಿ (l)
      (ಶೂನ್ಯ) allocate_size<ಶೂನ್ಯ> (alignment - l);
  }

  template <typename Type = ಶೂನ್ಯ>
  __attribute__((returns_nonnull))
  Type *start_embed (ಧ್ರುವ Type *obj HB_UNUSED = nullptr) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ reinterpret_cast<Type *> (ಏತತ್->head); }
  template <typename Type>
  __attribute__((returns_nonnull))
  Type *start_embed (ಧ್ರುವ Type &obj) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ start_embed (std::addressof (obj)); }

  ತಾರ್ಕಿಕ err (hb_serialize_error_t err_type)
  {
    ಪ್ರತ್ಯಾಗಮ !ತಾರ್ಕಿಕ ((errors = (errors | err_type)));
  }

  ತಾರ್ಕಿಕ start_zerocopy (size_t size)
  {
    ಯದಿ (unlikely (in_error ())) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಯದಿ (unlikely (size > INT_MAX || ಏತತ್->tail - ಏತತ್->head < ptrdiff_t (size)))
    {
      err (HB_SERIALIZE_ERROR_OUT_OF_ROOM);
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }

    ಪ್ರತಿಜ್ಞಾ (!ಏತತ್->zerocopy);
    ಏತತ್->zerocopy = ಏತತ್->head;

    ಪ್ರತಿಜ್ಞಾ (ಏತತ್->current->head == ಏತತ್->head);
    ಏತತ್->current->head = ಏತತ್->current->tail = ಏತತ್->head = ಏತತ್->tail - size;
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  template <typename Type>
  HB_NODISCARD
  Type *allocate_size (size_t size, ತಾರ್ಕಿಕ clear = ಸತ್ಯ)
  {
    ಯದಿ (unlikely (in_error ())) ಪ್ರತ್ಯಾಗಮ nullptr;

    ಯದಿ (unlikely (size > INT_MAX || ಏತತ್->tail - ಏತತ್->head < ptrdiff_t (size)))
    {
      err (HB_SERIALIZE_ERROR_OUT_OF_ROOM);
      ಪ್ರತ್ಯಾಗಮ nullptr;
    }
    ಯದಿ (clear)
      hb_memset (ಏತತ್->head, 0, size);
    ವರ್ಣ *ret = ಏತತ್->head;
    ಏತತ್->head += size;
    ಪ್ರತ್ಯಾಗಮ reinterpret_cast<Type *> (ret);
  }

  template <typename Type>
  Type *allocate_min ()
  { ಪ್ರತ್ಯಾಗಮ ಏತತ್->allocate_size<Type> (Type::min_size); }

  template <typename Type>
  HB_NODISCARD
  Type *embed (ಧ್ರುವ Type *obj)
  {
    size_t size = obj->get_size ();
    Type *ret = ಏತತ್->allocate_size<Type> (size, ಅಸತ್ಯ);
    ಯದಿ (unlikely (!ret)) ಪ್ರತ್ಯಾಗಮ nullptr;
    hb_memcpy (ret, obj, size);
    ಪ್ರತ್ಯಾಗಮ ret;
  }
  template <typename Type>
  HB_NODISCARD
  Type *embed (ಧ್ರುವ Type &obj)
  { ಪ್ರತ್ಯಾಗಮ embed (std::addressof (obj)); }
  ವರ್ಣ *embed (ಧ್ರುವ ವರ್ಣ *obj, unsigned size)
  {
    ವರ್ಣ *ret = ಏತತ್->allocate_size<ವರ್ಣ> (size, ಅಸತ್ಯ);
    ಯದಿ (unlikely (!ret)) ಪ್ರತ್ಯಾಗಮ nullptr;
    hb_memcpy (ret, obj, size);
    ಪ್ರತ್ಯಾಗಮ ret;
  }

  template <typename Type, typename ...Ts> auto
  _copy (ಧ್ರುವ Type &src, hb_priority<1>, Ts&&... ds) HB_RETURN
  (Type *, src.copy (ಏತತ್, std::forward<Ts> (ds)...))

  template <typename Type> auto
  _copy (ಧ್ರುವ Type &src, hb_priority<0>) -> decltype (&(hb_declval<Type> () = src))
  {
    Type *ret = ಏತತ್->allocate_size<Type> (ಪರಿಮಾಣ (Type));
    ಯದಿ (unlikely (!ret)) ಪ್ರತ್ಯಾಗಮ nullptr;
    *ret = src;
    ಪ್ರತ್ಯಾಗಮ ret;
  }

  /* Like embed, but active: calls obj.operator=() or obj.copy() to transfer data
   * instead of hb_memcpy(). */
  template <typename Type, typename ...Ts>
  Type *copy (ಧ್ರುವ Type &src, Ts&&... ds)
  { ಪ್ರತ್ಯಾಗಮ _copy (src, hb_prioritize, std::forward<Ts> (ds)...); }
  template <typename Type, typename ...Ts>
  Type *copy (ಧ್ರುವ Type *src, Ts&&... ds)
  { ಪ್ರತ್ಯಾಗಮ copy (*src, std::forward<Ts> (ds)...); }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator)),
	   typename ...Ts>
  ಶೂನ್ಯ copy_all (Iterator it, Ts&&... ds)
  { ಪ್ರತಿ (decltype (*it) _ : it) copy (_, ds...); }

  template <typename Type>
  hb_serialize_context_t& operator << (ಧ್ರುವ Type &obj) & { embed (obj); ಪ್ರತ್ಯಾಗಮ *ಏತತ್; }

  template <typename Type>
  Type *extend_size (Type *obj, size_t size, ತಾರ್ಕಿಕ clear = ಸತ್ಯ)
  {
    ಯದಿ (unlikely (in_error ())) ಪ್ರತ್ಯಾಗಮ nullptr;
    ಯದಿ (unlikely (size >= INT_MAX)) { err (HB_SERIALIZE_ERROR_OTHER); ಪ್ರತ್ಯಾಗಮ nullptr; }

    ಪ್ರತಿಜ್ಞಾ (ಏತತ್->start <= (ವರ್ಣ *) obj);
    ಪ್ರತಿಜ್ಞಾ ((ವರ್ಣ *) obj <= ಏತತ್->head);
    ಪ್ರತಿಜ್ಞಾ ((size_t) (ಏತತ್->head - (ವರ್ಣ *) obj) <= size);
    ಯದಿ (unlikely (((ವರ್ಣ *) obj + size < (ವರ್ಣ *) obj) ||
		  !ಏತತ್->allocate_size<Type> (((ವರ್ಣ *) obj) + size - ಏತತ್->head, clear))) ಪ್ರತ್ಯಾಗಮ nullptr;
    ಪ್ರತ್ಯಾಗಮ reinterpret_cast<Type *> (obj);
  }
  template <typename Type>
  Type *extend_size (Type &obj, size_t size, ತಾರ್ಕಿಕ clear = ಸತ್ಯ)
  { ಪ್ರತ್ಯಾಗಮ extend_size (std::addressof (obj), size, clear); }

  template <typename Type>
  Type *extend_min (Type *obj) { ಪ್ರತ್ಯಾಗಮ extend_size (obj, obj->min_size); }
  template <typename Type>
  Type *extend_min (Type &obj) { ಪ್ರತ್ಯಾಗಮ extend_min (std::addressof (obj)); }

  template <typename Type, typename ...Ts>
  Type *extend (Type *obj, Ts&&... ds)
  { ಪ್ರತ್ಯಾಗಮ extend_size (obj, obj->get_size (std::forward<Ts> (ds)...)); }
  template <typename Type, typename ...Ts>
  Type *extend (Type &obj, Ts&&... ds)
  { ಪ್ರತ್ಯಾಗಮ extend (std::addressof (obj), std::forward<Ts> (ds)...); }

  /* Output routines. */
  hb_bytes_t copy_bytes () ಧ್ರುವ
  {
    ಪ್ರತಿಜ್ಞಾ (successful ());
    /* Copy both items from head side and tail side... */
    unsigned ಪೂರ್ಣಾಙ್ಕ len = (ಏತತ್->head - ಏತತ್->start)
		     + (ಏತತ್->end  - ಏತತ್->tail);

    // If len is zero don't hb_malloc as the memory won't get properly
    // cleaned up later.
    ಯದಿ (!len) ಪ್ರತ್ಯಾಗಮ hb_bytes_t ();

    ವರ್ಣ *p = (ವರ್ಣ *) hb_malloc (len);
    ಯದಿ (unlikely (!p)) ಪ್ರತ್ಯಾಗಮ hb_bytes_t ();

    hb_memcpy (p, ಏತತ್->start, ಏತತ್->head - ಏತತ್->start);
    hb_memcpy (p + (ಏತತ್->head - ಏತತ್->start), ಏತತ್->tail, ಏತತ್->end - ಏತತ್->tail);
    ಪ್ರತ್ಯಾಗಮ hb_bytes_t (p, len);
  }
  template <typename Type>
  Type *copy () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ reinterpret_cast<Type *> ((ವರ್ಣ *) copy_bytes ().arrayZ); }
  hb_blob_t *copy_blob () ಧ್ರುವ
  {
    hb_bytes_t b = copy_bytes ();
    ಪ್ರತ್ಯಾಗಮ hb_blob_create (b.arrayZ, b.length,
			   HB_MEMORY_MODE_WRITABLE,
			   (ವರ್ಣ *) b.arrayZ, hb_free);
  }

  ಧ್ರುವ hb_vector_t<object_t *>& object_graph() ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ packed; }

  ಗುಪ್ತ:
  template <typename T, unsigned Size = ಪರಿಮಾಣ (T)>
  ಶೂನ್ಯ assign_offset (ಧ್ರುವ object_t* parent, ಧ್ರುವ object_t::link_t &link, unsigned offset)
  {
    // XXX We should stop assuming big-endian!
    auto &off = * ((HBInt<ಸತ್ಯ, T, Size> *) (parent->head + link.position));
    ಪ್ರತಿಜ್ಞಾ (0 == off);
    check_assign (off, offset, HB_SERIALIZE_ERROR_OFFSET_OVERFLOW);
  }

  ಸಾರ್ವಜನಿಕ:
  ವರ್ಣ *start, *head, *tail, *end, *zerocopy;
  unsigned ಪೂರ್ಣಾಙ್ಕ debug_depth;
  hb_serialize_error_t errors;

  ಗುಪ್ತ:

  ಶೂನ್ಯ merge_virtual_links (ಧ್ರುವ object_t* from, objidx_t to_idx) {
    object_t* to = packed[to_idx];
    ಪ್ರತಿ (ಧ್ರುವ auto& l : from->virtual_links) {
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
  hb_hashmap_t<ಧ್ರುವ object_t *, objidx_t> packed_map;
};

#endif /* HB_SERIALIZE_HH */
