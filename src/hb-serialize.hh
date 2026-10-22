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

#অন্তর্ভাৱ "hb.hh"
#অন্তর্ভাৱ "hb-blob.hh"
#অন্তর্ভাৱ "hb-map.hh"
#অন্তর্ভাৱ "hb-free-pool.hh"

#অন্তর্ভাৱ "hb-subset-serialize.h"

/*
 * Serialize
 */

গণনা hb_serialize_error_t {
  HB_SERIALIZE_ERROR_NONE =            0x00000000u,
  HB_SERIALIZE_ERROR_OTHER =           0x00000001u,
  HB_SERIALIZE_ERROR_OFFSET_OVERFLOW = 0x00000002u,
  HB_SERIALIZE_ERROR_OUT_OF_ROOM =     0x00000004u,
  HB_SERIALIZE_ERROR_INT_OVERFLOW =    0x00000008u,
  HB_SERIALIZE_ERROR_ARRAY_OVERFLOW =  0x00000010u
};
HB_MARK_AS_FLAG_T (hb_serialize_error_t);

সংরচনা hb_serialize_context_t
{
  প্রকারনির্দেশ unsigned objidx_t;

  গণনা whence_t {
     Head,	/* Relative to the current object head (default). */
     Tail,	/* Relative to the current object tail after packed. */
     Absolute	/* Absolute: from the start of the serialize buffer. */
   };



  সংরচনা object_t
  {
    শূন্য fini () {
      real_links.fini ();
      virtual_links.fini ();
    }

    object_t () = স্ৱভাৱ;

    object_t (ধ্রুৱ hb_subset_serialize_object_t &o)
    {
      head = o.head;
      tail = o.tail;
      next = nullptr;
      real_links.alloc_exact (o.অঙ্ক_real_links);
      বাবে (unsigned i = 0 ; i < o.অঙ্ক_real_links; i++)
        real_links.push (o.real_links[i]);

      virtual_links.alloc_exact (o.অঙ্ক_virtual_links);
      বাবে (unsigned i = 0; i < o.অঙ্ক_virtual_links; i++)
        virtual_links.push (o.virtual_links[i]);
    }

    তার্কিক in_error () ধ্রুৱ
    {
      ঘূৰাই real_links.in_error() || virtual_links.in_error();
    }

    তার্কিক add_virtual_link (objidx_t objidx)
    {
      যদি (!objidx)
        ঘূৰাই মিছা;

      auto& link = *virtual_links.push ();
      যদি (virtual_links.in_error ())
        ঘূৰাই মিছা;

      link.objidx = objidx;
      // Remaining fields were previously zero'd by push():
      // link.width = 0;
      // link.is_signed = 0;
      // link.whence = 0;
      // link.position = 0;
      // link.bias = 0;

      ঘূৰাই সঁচা;
    }

    friend শূন্য swap (object_t& a, object_t& b) noexcept
    {
      hb_swap (a.head, b.head);
      hb_swap (a.tail, b.tail);
      hb_swap (a.next, b.next);
      hb_swap (a.real_links, b.real_links);
      hb_swap (a.virtual_links, b.virtual_links);
    }

    তার্কিক operator == (ধ্রুৱ object_t &o) ধ্রুৱ
    {
      // Virtual links aren't considered for equality since they don't affect the functionality
      // of the object.
      ঘূৰাই (tail - head == o.tail - o.head)
	  && (real_links.length == o.real_links.length)
	  && 0 == hb_memcmp (head, o.head, tail - head)
	  && real_links.as_bytes () == o.real_links.as_bytes ();
    }
    uint32_t hash () ধ্রুৱ
    {
      // Virtual links aren't considered for equality since they don't affect the functionality
      // of the object.
      ঘূৰাই hb_bytes_t (head, hb_min (128, tail - head)).hash () ^
          real_links.as_bytes ().hash ();
    }

    সংরচনা link_t
    {
      unsigned width: 3;
      unsigned is_signed: 1;
      unsigned whence: 2;
      unsigned bias : 26;
      unsigned position;
      objidx_t objidx;

      link_t () = স্ৱভাৱ;

      link_t (ধ্রুৱ hb_subset_serialize_link_t &o)
      {
        width = o.width;
        is_signed = 0;
        whence = 0;
        position = o.position;
        bias = 0;
        objidx = o.objidx;
      }

      HB_INTERNAL স্থির পূর্ণাঙ্ক cmp (ধ্রুৱ শূন্য* a, ধ্রুৱ শূন্য* b)
      {
        পূর্ণাঙ্ক cmp = ((ধ্রুৱ link_t*)a)->position - ((ধ্রুৱ link_t*)b)->position;
        যদি (cmp) ঘূৰাই cmp;

        ঘূৰাই ((ধ্রুৱ link_t*)a)->objidx - ((ধ্রুৱ link_t*)b)->objidx;
      }
    };

    ৱর্ণ *head;
    ৱর্ণ *tail;
    hb_vector_t<link_t> real_links;
    hb_vector_t<link_t> virtual_links;
    object_t *next;

    auto all_links () ধ্রুৱ HB_AUTO_RETURN
        (( hb_concat (real_links, virtual_links) ));
    auto all_links_writer () HB_AUTO_RETURN
        (( hb_concat (real_links.writer (), virtual_links.writer ()) ));
  };

  সংরচনা snapshot_t
  {
    ৱর্ণ *head;
    ৱর্ণ *tail;
    object_t *current; // Just for sanity check
    unsigned অঙ্ক_real_links;
    unsigned অঙ্ক_virtual_links;
    hb_serialize_error_t errors;
  };

  snapshot_t snapshot ()
  {
    ঘূৰাই snapshot_t {
      head, tail, current,
      current ? current->real_links.length : 0,
      current ? current->virtual_links.length : 0,
      errors
     };
  }

  hb_serialize_context_t (শূন্য *start_, size_t size) :
    start ((ৱর্ণ *) start_),
    end (start + size),
    current (nullptr)
  { reset (); }
  ~hb_serialize_context_t () { fini (); }

  শূন্য fini ()
  {
    বাবে (object_t *_ : ++hb_iter (packed)) _->fini ();
    packed.fini ();
    এতত্->packed_map.fini ();

    যেতিয়ালৈ (current)
    {
      auto *_ = current;
      current = current->next;
      _->fini ();
    }
  }

  তার্কিক in_error () ধ্রুৱ { ঘূৰাই তার্কিক (errors); }

  তার্কিক successful () ধ্রুৱ { ঘূৰাই !তার্কিক (errors); }

  HB_NODISCARD তার্কিক ran_out_of_room () ধ্রুৱ { ঘূৰাই errors & HB_SERIALIZE_ERROR_OUT_OF_ROOM; }
  HB_NODISCARD তার্কিক offset_overflow () ধ্রুৱ { ঘূৰাই errors & HB_SERIALIZE_ERROR_OFFSET_OVERFLOW; }
  HB_NODISCARD তার্কিক only_offset_overflow () ধ্রুৱ { ঘূৰাই errors == HB_SERIALIZE_ERROR_OFFSET_OVERFLOW; }
  HB_NODISCARD তার্কিক only_overflow () ধ্রুৱ
  {
    ঘূৰাই errors == HB_SERIALIZE_ERROR_OFFSET_OVERFLOW
        || errors == HB_SERIALIZE_ERROR_INT_OVERFLOW
        || errors == HB_SERIALIZE_ERROR_ARRAY_OVERFLOW;
  }

  শূন্য reset (শূন্য *start_, size_t size)
  {
    start = (ৱর্ণ*) start_;
    end = start + size;
    reset ();
    current = nullptr;
  }

  শূন্য reset ()
  {
    এতত্->errors = HB_SERIALIZE_ERROR_NONE;
    এতত্->head = এতত্->start;
    এতত্->tail = এতত্->end;
    এতত্->zerocopy = nullptr;
    এতত্->debug_depth = 0;

    fini ();
    এতত্->packed.push (nullptr);
    এতত্->packed_map.init ();
  }

  তার্কিক check_success (তার্কিক success,
                      hb_serialize_error_t err_type = HB_SERIALIZE_ERROR_OTHER)
  {
    ঘূৰাই successful ()
        && (success || err (err_type));
  }

  template <typename T1, typename T2>
  তার্কিক check_equal (T1 &&v1, T2 &&v2, hb_serialize_error_t err_type)
  {
    যদি ((long long) v1 != (long long) v2)
    {
      ঘূৰাই err (err_type);
    }
    ঘূৰাই সঁচা;
  }

  template <typename T1, typename T2>
  তার্কিক check_assign (T1 &v1, T2 &&v2, hb_serialize_error_t err_type)
  { ঘূৰাই check_equal (v1 = v2, v2, err_type); }

  template <typename T> তার্কিক propagate_error (T &&obj)
  { ঘূৰাই check_success (!hb_deref (obj).in_error ()); }

  template <typename T1, typename... Ts> তার্কিক propagate_error (T1 &&o1, Ts&&... os)
  { ঘূৰাই propagate_error (std::forward<T1> (o1)) &&
	   propagate_error (std::forward<Ts> (os)...); }

  /* To be called around main operation. */
  template <typename Type=ৱর্ণ>
  __attribute__((returns_nonnull))
  Type *start_serialize ()
  {
    DEBUG_বাৰ্তা_LEVEL (SERIALIZE, এতত্->start, 0, +1,
		     "start [%p..%p] (%lu bytes)",
		     এতত্->start, এতত্->end,
		     (unsigned long) (এতত্->end - এতত্->start));

    প্রতিজ্ঞা (!current);
    ঘূৰাই push<Type> ();
  }
  শূন্য end_serialize ()
  {
    DEBUG_বাৰ্তা_LEVEL (SERIALIZE, এতত্->start, 0, -1,
		     "end [%p..%p] serialized %u bytes; %s",
		     এতত্->start, এতত্->end,
		     (unsigned) (এতত্->head - এতত্->start),
		     successful () ? "successful" : "UNSUCCESSFUL");

    propagate_error (packed, packed_map);

    যদি (unlikely (!current)) ঘূৰাই;
    যদি (unlikely (in_error()))
    {
      // Offset overflows that occur before link resolution cannot be handled
      // by repacking, so set a more general error.
      যদি (offset_overflow ()) err (HB_SERIALIZE_ERROR_OTHER);
      ঘূৰাই;
    }

    প্রতিজ্ঞা (!current->next);

    /* Only "pack" if there exist other objects... Otherwise, don't bother.
     * Saves a move. */
    যদি (packed.length <= 1)
      ঘূৰাই;

    বেৰ_কৰো_pack (মিছা);

    resolve_links ();
  }

  template <typename Type = শূন্য>
  __attribute__((returns_nonnull))
  Type *push ()
  {
    যদি (unlikely (in_error ())) ঘূৰাই start_embed<Type> ();

    object_t *obj = object_pool.alloc ();
    যদি (unlikely (!obj))
      check_success (মিছা);
    অন্যথা
    {
      obj->head = head;
      obj->tail = tail;
      obj->next = current;
      current = obj;
    }
    ঘূৰাই start_embed<Type> ();
  }
  শূন্য বেৰ_কৰো_discard ()
  {
    object_t *obj = current;
    যদি (unlikely (!obj)) ঘূৰাই;
    // Allow cleanup when we've error'd out on int overflows which don't compromise
    // the serializer state.
    যদি (unlikely (in_error() && !only_overflow ())) ঘূৰাই;

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
  objidx_t বেৰ_কৰো_pack (তার্কিক share=সঁচা)
  {
    object_t *obj = current;
    যদি (unlikely (!obj)) ঘূৰাই 0;
    // Allow cleanup when we've error'd out on int overflows which don't compromise
    // the serializer state.
    যদি (unlikely (in_error()  && !only_overflow ())) ঘূৰাই 0;

    current = current->next;
    obj->tail = head;
    obj->next = nullptr;
    প্রতিজ্ঞা (obj->head <= obj->tail);
    unsigned len = obj->tail - obj->head;
    head = zerocopy ? zerocopy : obj->head; /* Rewind head. */
    তার্কিক was_zerocopy = zerocopy;
    zerocopy = nullptr;

    যদি (!len)
    {
      প্রতিজ্ঞা (!obj->real_links.length);
      প্রতিজ্ঞা (!obj->virtual_links.length);
      ঘূৰাই 0;
    }

    objidx_t objidx;
    uint32_t hash = 0;
    যদি (share)
    {
      hash = hb_hash (obj);
      objidx = packed_map.get_with_hash (obj, hash);
      যদি (objidx)
      {
        merge_virtual_links (obj, objidx);
	obj->fini ();
        object_pool.release (obj);
	ঘূৰাই objidx;
      }
    }

    tail -= len;
    যদি (was_zerocopy)
      প্রতিজ্ঞা (tail == obj->head);
    অন্যথা
      memmove (tail, obj->head, len);

    obj->head = tail;
    obj->tail = tail + len;

    packed.push (obj);

    যদি (unlikely (!propagate_error (packed)))
    {
      /* Obj wasn't successfully added to packed, so clean it up otherwise its
       * links will be leaked. When we use constructor/destructors properly, we
       * can remove these. */
      obj->fini ();
      ঘূৰাই 0;
    }

    objidx = packed.length - 1;

    যদি (share) packed_map.set_with_hash (obj, hash, objidx);
    propagate_error (packed_map);

    ঘূৰাই objidx;
  }

  শূন্য revert (snapshot_t snap)
  {
    // Overflows that happened after the snapshot will be erased by the revert.
    যদি (unlikely (in_error () && !only_overflow ())) ঘূৰাই;
    প্রতিজ্ঞা (snap.current == current);
    যদি (current)
    {
      current->real_links.shrink (snap.অঙ্ক_real_links);
      current->virtual_links.shrink (snap.অঙ্ক_virtual_links);
    }
    errors = snap.errors;
    revert (snap.head, snap.tail);
  }

  শূন্য revert (ৱর্ণ *snap_head,
	       ৱর্ণ *snap_tail)
  {
    যদি (unlikely (in_error ())) ঘূৰাই;
    প্রতিজ্ঞা (snap_head <= head);
    প্রতিজ্ঞা (tail <= snap_tail);
    head = snap_head;
    tail = snap_tail;
    discard_stale_objects ();
  }

  শূন্য discard_stale_objects ()
  {
    যদি (unlikely (in_error ())) ঘূৰাই;
    যেতিয়ালৈ (packed.length > 1 &&
	   packed.tail ()->head < tail)
    {
      object_t *obj = packed.tail ();
      packed_map.del (obj);
      প্রতিজ্ঞা (!obj->next);
      obj->fini ();
      object_pool.release (obj);
      packed.বেৰ_কৰো ();
    }
    যদি (packed.length > 1)
      প্রতিজ্ঞা (packed.tail ()->head == tail);
  }

  // Adds a virtual link from the current object to objidx. A virtual link is not associated with
  // an actual offset field. They are solely used to enforce ordering constraints between objects.
  // Adding a virtual link from object a to object b will ensure that object b is always packed after
  // object a in the final serialized order.
  //
  // This is useful in certain situations where there needs to be a specific ordering in the
  // final serialization. Such as when platform bugs require certain orderings, or to provide
  //  guidance to the repacker for better offset overflow resolution.
  শূন্য add_virtual_link (objidx_t objidx)
  {
    যদি (unlikely (in_error ())) ঘূৰাই;

    যদি (!objidx)
      ঘূৰাই;

    প্রতিজ্ঞা (current);

    যদি (!current->add_virtual_link(objidx))
      err (HB_SERIALIZE_ERROR_OTHER);
  }

  objidx_t last_added_child_index() ধ্রুৱ {
    যদি (unlikely (in_error ())) ঘূৰাই (objidx_t) -1;

    প্রতিজ্ঞা (current);
    যদি (!তার্কিক(current->real_links)) {
      ঘূৰাই (objidx_t) -1;
    }

    ঘূৰাই current->real_links[current->real_links.length - 1].objidx;
  }

  // For the current object ensure that the sub-table bytes for child objidx are always placed
  // after the subtable bytes for any other existing children. This only ensures that the
  // repacker will not move the target subtable before the other children
  // (by adding virtual links). It is up to the caller to ensure the initial serialization
  // order is correct.
  শূন্য repack_last(objidx_t objidx) {
    যদি (unlikely (in_error ())) ঘূৰাই;

    যদি (!objidx)
      ঘূৰাই;

    প্রতিজ্ঞা (current);
    বাবে (auto& l : current->real_links) {
      যদি (l.objidx == objidx) {
        অনুৱর্ত;
      }

      packed[l.objidx]->add_virtual_link(objidx);
    }
  }

  template <typename T>
  শূন্য add_link (T &ofs, objidx_t objidx,
		 whence_t whence = Head,
		 unsigned bias = 0)
  {
    যদি (unlikely (in_error ())) ঘূৰাই;

    যদি (!objidx)
      ঘূৰাই;

    প্রতিজ্ঞা (current);
    প্রতিজ্ঞা (current->head <= (ধ্রুৱ ৱর্ণ *) &ofs);

    auto& link = *current->real_links.push ();
    যদি (current->real_links.in_error ())
      err (HB_SERIALIZE_ERROR_OTHER);

    link.width = পরিমাণ (T);
    link.objidx = objidx;
    যদি (unlikely (!পরিমাণ (T)))
    {
      // This link is not associated with an actual offset and exists merely to enforce
      // an ordering constraint.
      link.is_signed = 0;
      link.whence = 0;
      link.position = 0;
      link.bias = 0;
      ঘূৰাই;
    }

    link.is_signed = std::is_signed<hb_unwrap_type (T)>::value;
    link.whence = (unsigned) whence;
    link.position = (ধ্রুৱ ৱর্ণ *) &ofs - current->head;
    link.bias = bias;
  }

  unsigned to_bias (ধ্রুৱ শূন্য *base) ধ্রুৱ
  {
    যদি (unlikely (in_error ())) ঘূৰাই 0;
    যদি (!base) ঘূৰাই 0;
    প্রতিজ্ঞা (current);
    প্রতিজ্ঞা (current->head <= (ধ্রুৱ ৱর্ণ *) base);
    ঘূৰাই (ধ্রুৱ ৱর্ণ *) base - current->head;
  }

  শূন্য resolve_links ()
  {
    যদি (unlikely (in_error ())) ঘূৰাই;

    প্রতিজ্ঞা (!current);
    প্রতিজ্ঞা (packed.length > 1);

    বাবে (ধ্রুৱ object_t* parent : ++hb_iter (packed))
      বাবে (ধ্রুৱ object_t::link_t &link : parent->real_links)
      {
	ধ্রুৱ object_t* child = packed[link.objidx];
	যদি (unlikely (!child)) { err (HB_SERIALIZE_ERROR_OTHER); ঘূৰাই; }
	unsigned offset = 0;
	ৱিকল্প ((whence_t) link.whence) {
	পক্ষ Head:     offset = child->head - parent->head; ভঙ্গ;
	পক্ষ Tail:     offset = child->head - parent->tail; ভঙ্গ;
	পক্ষ Absolute: offset = (head - start) + (child->head - tail); ভঙ্গ;
	}

	প্রতিজ্ঞা (offset >= link.bias);
	offset -= link.bias;
	যদি (link.is_signed)
	{
	  প্রতিজ্ঞা (link.width == 2 || link.width == 4);
	  যদি (link.width == 4)
	    assign_offset<int32_t> (parent, link, offset);
	  অন্যথা
	    assign_offset<int16_t> (parent, link, offset);
	}
	অন্যথা
	{
	  প্রতিজ্ঞা (link.width == 2 || link.width == 3 || link.width == 4);
	  যদি (link.width == 4)
	    assign_offset<uint32_t> (parent, link, offset);
	  অন্যথা যদি (link.width == 3)
	    assign_offset<uint32_t, 3> (parent, link, offset);
	  অন্যথা
	    assign_offset<uint16_t> (parent, link, offset);
	}
      }
  }

  unsigned পূর্ণাঙ্ক length () ধ্রুৱ
  {
    যদি (unlikely (!current)) ঘূৰাই 0;
    ঘূৰাই এতত্->head - current->head;
  }

  শূন্য align (unsigned পূর্ণাঙ্ক alignment)
  {
    unsigned পূর্ণাঙ্ক l = length () % alignment;
    যদি (l)
      (শূন্য) allocate_size<শূন্য> (alignment - l);
  }

  template <typename Type = শূন্য>
  __attribute__((returns_nonnull))
  Type *start_embed (ধ্রুৱ Type *obj HB_UNUSED = nullptr) ধ্রুৱ
  { ঘূৰাই reinterpret_cast<Type *> (এতত্->head); }
  template <typename Type>
  __attribute__((returns_nonnull))
  Type *start_embed (ধ্রুৱ Type &obj) ধ্রুৱ
  { ঘূৰাই start_embed (std::addressof (obj)); }

  তার্কিক err (hb_serialize_error_t err_type)
  {
    ঘূৰাই !তার্কিক ((errors = (errors | err_type)));
  }

  তার্কিক start_zerocopy (size_t size)
  {
    যদি (unlikely (in_error ())) ঘূৰাই মিছা;

    যদি (unlikely (size > INT_MAX || এতত্->tail - এতত্->head < ptrdiff_t (size)))
    {
      err (HB_SERIALIZE_ERROR_OUT_OF_ROOM);
      ঘূৰাই মিছা;
    }

    প্রতিজ্ঞা (!এতত্->zerocopy);
    এতত্->zerocopy = এতত্->head;

    প্রতিজ্ঞা (এতত্->current->head == এতত্->head);
    এতত্->current->head = এতত্->current->tail = এতত্->head = এতত্->tail - size;
    ঘূৰাই সঁচা;
  }

  template <typename Type>
  HB_NODISCARD
  Type *allocate_size (size_t size, তার্কিক clear = সঁচা)
  {
    যদি (unlikely (in_error ())) ঘূৰাই nullptr;

    যদি (unlikely (size > INT_MAX || এতত্->tail - এতত্->head < ptrdiff_t (size)))
    {
      err (HB_SERIALIZE_ERROR_OUT_OF_ROOM);
      ঘূৰাই nullptr;
    }
    যদি (clear)
      hb_memset (এতত্->head, 0, size);
    ৱর্ণ *ret = এতত্->head;
    এতত্->head += size;
    ঘূৰাই reinterpret_cast<Type *> (ret);
  }

  template <typename Type>
  Type *allocate_min ()
  { ঘূৰাই এতত্->allocate_size<Type> (Type::min_size); }

  template <typename Type>
  HB_NODISCARD
  Type *embed (ধ্রুৱ Type *obj)
  {
    size_t size = obj->get_size ();
    Type *ret = এতত্->allocate_size<Type> (size, মিছা);
    যদি (unlikely (!ret)) ঘূৰাই nullptr;
    hb_memcpy (ret, obj, size);
    ঘূৰাই ret;
  }
  template <typename Type>
  HB_NODISCARD
  Type *embed (ধ্রুৱ Type &obj)
  { ঘূৰাই embed (std::addressof (obj)); }
  ৱর্ণ *embed (ধ্রুৱ ৱর্ণ *obj, unsigned size)
  {
    ৱর্ণ *ret = এতত্->allocate_size<ৱর্ণ> (size, মিছা);
    যদি (unlikely (!ret)) ঘূৰাই nullptr;
    hb_memcpy (ret, obj, size);
    ঘূৰাই ret;
  }

  template <typename Type, typename ...Ts> auto
  _copy (ধ্রুৱ Type &src, hb_priority<1>, Ts&&... ds) HB_RETURN
  (Type *, src.copy (এতত্, std::forward<Ts> (ds)...))

  template <typename Type> auto
  _copy (ধ্রুৱ Type &src, hb_priority<0>) -> decltype (&(hb_declval<Type> () = src))
  {
    Type *ret = এতত্->allocate_size<Type> (পরিমাণ (Type));
    যদি (unlikely (!ret)) ঘূৰাই nullptr;
    *ret = src;
    ঘূৰাই ret;
  }

  /* Like embed, but active: calls obj.operator=() or obj.copy() to transfer data
   * instead of hb_memcpy(). */
  template <typename Type, typename ...Ts>
  Type *copy (ধ্রুৱ Type &src, Ts&&... ds)
  { ঘূৰাই _copy (src, hb_prioritize, std::forward<Ts> (ds)...); }
  template <typename Type, typename ...Ts>
  Type *copy (ধ্রুৱ Type *src, Ts&&... ds)
  { ঘূৰাই copy (*src, std::forward<Ts> (ds)...); }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator)),
	   typename ...Ts>
  শূন্য copy_all (Iterator it, Ts&&... ds)
  { বাবে (decltype (*it) _ : it) copy (_, ds...); }

  template <typename Type>
  hb_serialize_context_t& operator << (ধ্রুৱ Type &obj) & { embed (obj); ঘূৰাই *এতত্; }

  template <typename Type>
  Type *extend_size (Type *obj, size_t size, তার্কিক clear = সঁচা)
  {
    যদি (unlikely (in_error ())) ঘূৰাই nullptr;
    যদি (unlikely (size >= INT_MAX)) { err (HB_SERIALIZE_ERROR_OTHER); ঘূৰাই nullptr; }

    প্রতিজ্ঞা (এতত্->start <= (ৱর্ণ *) obj);
    প্রতিজ্ঞা ((ৱর্ণ *) obj <= এতত্->head);
    প্রতিজ্ঞা ((size_t) (এতত্->head - (ৱর্ণ *) obj) <= size);
    যদি (unlikely (((ৱর্ণ *) obj + size < (ৱর্ণ *) obj) ||
		  !এতত্->allocate_size<Type> (((ৱর্ণ *) obj) + size - এতত্->head, clear))) ঘূৰাই nullptr;
    ঘূৰাই reinterpret_cast<Type *> (obj);
  }
  template <typename Type>
  Type *extend_size (Type &obj, size_t size, তার্কিক clear = সঁচা)
  { ঘূৰাই extend_size (std::addressof (obj), size, clear); }

  template <typename Type>
  Type *extend_min (Type *obj) { ঘূৰাই extend_size (obj, obj->min_size); }
  template <typename Type>
  Type *extend_min (Type &obj) { ঘূৰাই extend_min (std::addressof (obj)); }

  template <typename Type, typename ...Ts>
  Type *extend (Type *obj, Ts&&... ds)
  { ঘূৰাই extend_size (obj, obj->get_size (std::forward<Ts> (ds)...)); }
  template <typename Type, typename ...Ts>
  Type *extend (Type &obj, Ts&&... ds)
  { ঘূৰাই extend (std::addressof (obj), std::forward<Ts> (ds)...); }

  /* Output routines. */
  hb_bytes_t copy_bytes () ধ্রুৱ
  {
    প্রতিজ্ঞা (successful ());
    /* Copy both items from head side and tail side... */
    unsigned পূর্ণাঙ্ক len = (এতত্->head - এতত্->start)
		     + (এতত্->end  - এতত্->tail);

    // If len is zero don't hb_malloc as the memory won't get properly
    // cleaned up later.
    যদি (!len) ঘূৰাই hb_bytes_t ();

    ৱর্ণ *p = (ৱর্ণ *) hb_malloc (len);
    যদি (unlikely (!p)) ঘূৰাই hb_bytes_t ();

    hb_memcpy (p, এতত্->start, এতত্->head - এতত্->start);
    hb_memcpy (p + (এতত্->head - এতত্->start), এতত্->tail, এতত্->end - এতত্->tail);
    ঘূৰাই hb_bytes_t (p, len);
  }
  template <typename Type>
  Type *copy () ধ্রুৱ
  { ঘূৰাই reinterpret_cast<Type *> ((ৱর্ণ *) copy_bytes ().arrayZ); }
  hb_blob_t *copy_blob () ধ্রুৱ
  {
    hb_bytes_t b = copy_bytes ();
    ঘূৰাই hb_blob_create (b.arrayZ, b.length,
			   HB_MEMORY_MODE_WRITABLE,
			   (ৱর্ণ *) b.arrayZ, hb_free);
  }

  ধ্রুৱ hb_vector_t<object_t *>& object_graph() ধ্রুৱ
  { ঘূৰাই packed; }

  গুপ্ত:
  template <typename T, unsigned Size = পরিমাণ (T)>
  শূন্য assign_offset (ধ্রুৱ object_t* parent, ধ্রুৱ object_t::link_t &link, unsigned offset)
  {
    // XXX We should stop assuming big-endian!
    auto &off = * ((HBInt<সঁচা, T, Size> *) (parent->head + link.position));
    প্রতিজ্ঞা (0 == off);
    check_assign (off, offset, HB_SERIALIZE_ERROR_OFFSET_OVERFLOW);
  }

  সার্ৱজনিক:
  ৱর্ণ *start, *head, *tail, *end, *zerocopy;
  unsigned পূর্ণাঙ্ক debug_depth;
  hb_serialize_error_t errors;

  গুপ্ত:

  শূন্য merge_virtual_links (ধ্রুৱ object_t* from, objidx_t to_idx) {
    object_t* to = packed[to_idx];
    বাবে (ধ্রুৱ auto& l : from->virtual_links) {
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
  hb_hashmap_t<ধ্রুৱ object_t *, objidx_t> packed_map;
};

#endif /* HB_SERIALIZE_HH */
