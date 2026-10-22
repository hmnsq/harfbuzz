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

#شامل "hb.hh"
#شامل "hb-blob.hh"
#شامل "hb-map.hh"
#شامل "hb-free-pool.hh"

#شامل "hb-subset-serialize.h"

/*
 * Serialize
 */

شمارش hb_serialize_error_t {
  HB_SERIALIZE_ERROR_NONE =            0x00000000u,
  HB_SERIALIZE_ERROR_OTHER =           0x00000001u,
  HB_SERIALIZE_ERROR_OFFSET_OVERFLOW = 0x00000002u,
  HB_SERIALIZE_ERROR_OUT_OF_ROOM =     0x00000004u,
  HB_SERIALIZE_ERROR_INT_OVERFLOW =    0x00000008u,
  HB_SERIALIZE_ERROR_ARRAY_OVERFLOW =  0x00000010u
};
HB_MARK_AS_FLAG_T (hb_serialize_error_t);

ساختار hb_serialize_context_t
{
  تعریفنوع unsigned objidx_t;

  شمارش whence_t {
     Head,	/* Relative to the current object head (default). */
     Tail,	/* Relative to the current object tail after packed. */
     Absolute	/* Absolute: from the start of the serialize buffer. */
   };



  ساختار object_t
  {
    تهی fini () {
      real_links.fini ();
      virtual_links.fini ();
    }

    object_t () = پیشفرض;

    object_t (ثابت hb_subset_serialize_object_t &o)
    {
      head = o.head;
      tail = o.tail;
      next = nullptr;
      real_links.alloc_exact (o.رقم_real_links);
      برای (unsigned i = 0 ; i < o.رقم_real_links; i++)
        real_links.push (o.real_links[i]);

      virtual_links.alloc_exact (o.رقم_virtual_links);
      برای (unsigned i = 0; i < o.رقم_virtual_links; i++)
        virtual_links.push (o.virtual_links[i]);
    }

    منطقی in_error () ثابت
    {
      بازگشت real_links.in_error() || virtual_links.in_error();
    }

    منطقی add_virtual_link (objidx_t objidx)
    {
      اگر (!objidx)
        بازگشت نادرست;

      auto& link = *virtual_links.push ();
      اگر (virtual_links.in_error ())
        بازگشت نادرست;

      link.objidx = objidx;
      // Remaining fields were previously zero'd by push():
      // link.width = 0;
      // link.is_signed = 0;
      // link.whence = 0;
      // link.position = 0;
      // link.bias = 0;

      بازگشت درست;
    }

    friend تهی swap (object_t& a, object_t& b) noexcept
    {
      hb_swap (a.head, b.head);
      hb_swap (a.tail, b.tail);
      hb_swap (a.next, b.next);
      hb_swap (a.real_links, b.real_links);
      hb_swap (a.virtual_links, b.virtual_links);
    }

    منطقی operator == (ثابت object_t &o) ثابت
    {
      // Virtual links aren't considered for equality since they don't affect the functionality
      // of the object.
      بازگشت (tail - head == o.tail - o.head)
	  && (real_links.length == o.real_links.length)
	  && 0 == hb_memcmp (head, o.head, tail - head)
	  && real_links.as_bytes () == o.real_links.as_bytes ();
    }
    uint32_t hash () ثابت
    {
      // Virtual links aren't considered for equality since they don't affect the functionality
      // of the object.
      بازگشت hb_bytes_t (head, hb_min (128, tail - head)).hash () ^
          real_links.as_bytes ().hash ();
    }

    ساختار link_t
    {
      unsigned width: 3;
      unsigned is_signed: 1;
      unsigned whence: 2;
      unsigned bias : 26;
      unsigned position;
      objidx_t objidx;

      link_t () = پیشفرض;

      link_t (ثابت hb_subset_serialize_link_t &o)
      {
        width = o.width;
        is_signed = 0;
        whence = 0;
        position = o.position;
        bias = 0;
        objidx = o.objidx;
      }

      HB_INTERNAL ایستا صحیح cmp (ثابت تهی* a, ثابت تهی* b)
      {
        صحیح cmp = ((ثابت link_t*)a)->position - ((ثابت link_t*)b)->position;
        اگر (cmp) بازگشت cmp;

        بازگشت ((ثابت link_t*)a)->objidx - ((ثابت link_t*)b)->objidx;
      }
    };

    نویسه *head;
    نویسه *tail;
    hb_vector_t<link_t> real_links;
    hb_vector_t<link_t> virtual_links;
    object_t *next;

    auto all_links () ثابت HB_AUTO_RETURN
        (( hb_concat (real_links, virtual_links) ));
    auto all_links_writer () HB_AUTO_RETURN
        (( hb_concat (real_links.writer (), virtual_links.writer ()) ));
  };

  ساختار snapshot_t
  {
    نویسه *head;
    نویسه *tail;
    object_t *current; // Just for sanity check
    unsigned رقم_real_links;
    unsigned رقم_virtual_links;
    hb_serialize_error_t errors;
  };

  snapshot_t snapshot ()
  {
    بازگشت snapshot_t {
      head, tail, current,
      current ? current->real_links.length : 0,
      current ? current->virtual_links.length : 0,
      errors
     };
  }

  hb_serialize_context_t (تهی *start_, size_t size) :
    start ((نویسه *) start_),
    end (start + size),
    current (nullptr)
  { reset (); }
  ~hb_serialize_context_t () { fini (); }

  تهی fini ()
  {
    برای (object_t *_ : ++hb_iter (packed)) _->fini ();
    packed.fini ();
    این->packed_map.fini ();

    وقتی (current)
    {
      auto *_ = current;
      current = current->next;
      _->fini ();
    }
  }

  منطقی in_error () ثابت { بازگشت منطقی (errors); }

  منطقی successful () ثابت { بازگشت !منطقی (errors); }

  HB_NODISCARD منطقی ran_out_of_room () ثابت { بازگشت errors & HB_SERIALIZE_ERROR_OUT_OF_ROOM; }
  HB_NODISCARD منطقی offset_overflow () ثابت { بازگشت errors & HB_SERIALIZE_ERROR_OFFSET_OVERFLOW; }
  HB_NODISCARD منطقی only_offset_overflow () ثابت { بازگشت errors == HB_SERIALIZE_ERROR_OFFSET_OVERFLOW; }
  HB_NODISCARD منطقی only_overflow () ثابت
  {
    بازگشت errors == HB_SERIALIZE_ERROR_OFFSET_OVERFLOW
        || errors == HB_SERIALIZE_ERROR_INT_OVERFLOW
        || errors == HB_SERIALIZE_ERROR_ARRAY_OVERFLOW;
  }

  تهی reset (تهی *start_, size_t size)
  {
    start = (نویسه*) start_;
    end = start + size;
    reset ();
    current = nullptr;
  }

  تهی reset ()
  {
    این->errors = HB_SERIALIZE_ERROR_NONE;
    این->head = این->start;
    این->tail = این->end;
    این->zerocopy = nullptr;
    این->debug_depth = 0;

    fini ();
    این->packed.push (nullptr);
    این->packed_map.init ();
  }

  منطقی check_success (منطقی success,
                      hb_serialize_error_t err_type = HB_SERIALIZE_ERROR_OTHER)
  {
    بازگشت successful ()
        && (success || err (err_type));
  }

  template <typename T1, typename T2>
  منطقی check_equal (T1 &&v1, T2 &&v2, hb_serialize_error_t err_type)
  {
    اگر ((long long) v1 != (long long) v2)
    {
      بازگشت err (err_type);
    }
    بازگشت درست;
  }

  template <typename T1, typename T2>
  منطقی check_assign (T1 &v1, T2 &&v2, hb_serialize_error_t err_type)
  { بازگشت check_equal (v1 = v2, v2, err_type); }

  template <typename T> منطقی propagate_error (T &&obj)
  { بازگشت check_success (!hb_deref (obj).in_error ()); }

  template <typename T1, typename... Ts> منطقی propagate_error (T1 &&o1, Ts&&... os)
  { بازگشت propagate_error (std::forward<T1> (o1)) &&
	   propagate_error (std::forward<Ts> (os)...); }

  /* To be called around main operation. */
  template <typename Type=نویسه>
  __attribute__((returns_nonnull))
  Type *start_serialize ()
  {
    DEBUG_پیام_LEVEL (SERIALIZE, این->start, 0, +1,
		     "start [%p..%p] (%lu bytes)",
		     این->start, این->end,
		     (unsigned long) (این->end - این->start));

    أكد (!current);
    بازگشت push<Type> ();
  }
  تهی end_serialize ()
  {
    DEBUG_پیام_LEVEL (SERIALIZE, این->start, 0, -1,
		     "end [%p..%p] serialized %u bytes; %s",
		     این->start, این->end,
		     (unsigned) (این->head - این->start),
		     successful () ? "successful" : "UNSUCCESSFUL");

    propagate_error (packed, packed_map);

    اگر (unlikely (!current)) بازگشت;
    اگر (unlikely (in_error()))
    {
      // Offset overflows that occur before link resolution cannot be handled
      // by repacking, so set a more general error.
      اگر (offset_overflow ()) err (HB_SERIALIZE_ERROR_OTHER);
      بازگشت;
    }

    أكد (!current->next);

    /* Only "pack" if there exist other objects... Otherwise, don't bother.
     * Saves a move. */
    اگر (packed.length <= 1)
      بازگشت;

    برداشت_pack (نادرست);

    resolve_links ();
  }

  template <typename Type = تهی>
  __attribute__((returns_nonnull))
  Type *push ()
  {
    اگر (unlikely (in_error ())) بازگشت start_embed<Type> ();

    object_t *obj = object_pool.alloc ();
    اگر (unlikely (!obj))
      check_success (نادرست);
    وگرنه
    {
      obj->head = head;
      obj->tail = tail;
      obj->next = current;
      current = obj;
    }
    بازگشت start_embed<Type> ();
  }
  تهی برداشت_discard ()
  {
    object_t *obj = current;
    اگر (unlikely (!obj)) بازگشت;
    // Allow cleanup when we've error'd out on int overflows which don't compromise
    // the serializer state.
    اگر (unlikely (in_error() && !only_overflow ())) بازگشت;

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
  objidx_t برداشت_pack (منطقی share=درست)
  {
    object_t *obj = current;
    اگر (unlikely (!obj)) بازگشت 0;
    // Allow cleanup when we've error'd out on int overflows which don't compromise
    // the serializer state.
    اگر (unlikely (in_error()  && !only_overflow ())) بازگشت 0;

    current = current->next;
    obj->tail = head;
    obj->next = nullptr;
    أكد (obj->head <= obj->tail);
    unsigned len = obj->tail - obj->head;
    head = zerocopy ? zerocopy : obj->head; /* Rewind head. */
    منطقی was_zerocopy = zerocopy;
    zerocopy = nullptr;

    اگر (!len)
    {
      أكد (!obj->real_links.length);
      أكد (!obj->virtual_links.length);
      بازگشت 0;
    }

    objidx_t objidx;
    uint32_t hash = 0;
    اگر (share)
    {
      hash = hb_hash (obj);
      objidx = packed_map.get_with_hash (obj, hash);
      اگر (objidx)
      {
        merge_virtual_links (obj, objidx);
	obj->fini ();
        object_pool.release (obj);
	بازگشت objidx;
      }
    }

    tail -= len;
    اگر (was_zerocopy)
      أكد (tail == obj->head);
    وگرنه
      memmove (tail, obj->head, len);

    obj->head = tail;
    obj->tail = tail + len;

    packed.push (obj);

    اگر (unlikely (!propagate_error (packed)))
    {
      /* Obj wasn't successfully added to packed, so clean it up otherwise its
       * links will be leaked. When we use constructor/destructors properly, we
       * can remove these. */
      obj->fini ();
      بازگشت 0;
    }

    objidx = packed.length - 1;

    اگر (share) packed_map.set_with_hash (obj, hash, objidx);
    propagate_error (packed_map);

    بازگشت objidx;
  }

  تهی revert (snapshot_t snap)
  {
    // Overflows that happened after the snapshot will be erased by the revert.
    اگر (unlikely (in_error () && !only_overflow ())) بازگشت;
    أكد (snap.current == current);
    اگر (current)
    {
      current->real_links.shrink (snap.رقم_real_links);
      current->virtual_links.shrink (snap.رقم_virtual_links);
    }
    errors = snap.errors;
    revert (snap.head, snap.tail);
  }

  تهی revert (نویسه *snap_head,
	       نویسه *snap_tail)
  {
    اگر (unlikely (in_error ())) بازگشت;
    أكد (snap_head <= head);
    أكد (tail <= snap_tail);
    head = snap_head;
    tail = snap_tail;
    discard_stale_objects ();
  }

  تهی discard_stale_objects ()
  {
    اگر (unlikely (in_error ())) بازگشت;
    وقتی (packed.length > 1 &&
	   packed.tail ()->head < tail)
    {
      object_t *obj = packed.tail ();
      packed_map.del (obj);
      أكد (!obj->next);
      obj->fini ();
      object_pool.release (obj);
      packed.برداشت ();
    }
    اگر (packed.length > 1)
      أكد (packed.tail ()->head == tail);
  }

  // Adds a virtual link from the current object to objidx. A virtual link is not associated with
  // an actual offset field. They are solely used to enforce ordering constraints between objects.
  // Adding a virtual link from object a to object b will ensure that object b is always packed after
  // object a in the final serialized order.
  //
  // This is useful in certain situations where there needs to be a specific ordering in the
  // final serialization. Such as when platform bugs require certain orderings, or to provide
  //  guidance to the repacker for better offset overflow resolution.
  تهی add_virtual_link (objidx_t objidx)
  {
    اگر (unlikely (in_error ())) بازگشت;

    اگر (!objidx)
      بازگشت;

    أكد (current);

    اگر (!current->add_virtual_link(objidx))
      err (HB_SERIALIZE_ERROR_OTHER);
  }

  objidx_t last_added_child_index() ثابت {
    اگر (unlikely (in_error ())) بازگشت (objidx_t) -1;

    أكد (current);
    اگر (!منطقی(current->real_links)) {
      بازگشت (objidx_t) -1;
    }

    بازگشت current->real_links[current->real_links.length - 1].objidx;
  }

  // For the current object ensure that the sub-table bytes for child objidx are always placed
  // after the subtable bytes for any other existing children. This only ensures that the
  // repacker will not move the target subtable before the other children
  // (by adding virtual links). It is up to the caller to ensure the initial serialization
  // order is correct.
  تهی repack_last(objidx_t objidx) {
    اگر (unlikely (in_error ())) بازگشت;

    اگر (!objidx)
      بازگشت;

    أكد (current);
    برای (auto& l : current->real_links) {
      اگر (l.objidx == objidx) {
        ادامه;
      }

      packed[l.objidx]->add_virtual_link(objidx);
    }
  }

  template <typename T>
  تهی add_link (T &ofs, objidx_t objidx,
		 whence_t whence = Head,
		 unsigned bias = 0)
  {
    اگر (unlikely (in_error ())) بازگشت;

    اگر (!objidx)
      بازگشت;

    أكد (current);
    أكد (current->head <= (ثابت نویسه *) &ofs);

    auto& link = *current->real_links.push ();
    اگر (current->real_links.in_error ())
      err (HB_SERIALIZE_ERROR_OTHER);

    link.width = اندازه (T);
    link.objidx = objidx;
    اگر (unlikely (!اندازه (T)))
    {
      // This link is not associated with an actual offset and exists merely to enforce
      // an ordering constraint.
      link.is_signed = 0;
      link.whence = 0;
      link.position = 0;
      link.bias = 0;
      بازگشت;
    }

    link.is_signed = std::is_signed<hb_unwrap_type (T)>::value;
    link.whence = (unsigned) whence;
    link.position = (ثابت نویسه *) &ofs - current->head;
    link.bias = bias;
  }

  unsigned to_bias (ثابت تهی *base) ثابت
  {
    اگر (unlikely (in_error ())) بازگشت 0;
    اگر (!base) بازگشت 0;
    أكد (current);
    أكد (current->head <= (ثابت نویسه *) base);
    بازگشت (ثابت نویسه *) base - current->head;
  }

  تهی resolve_links ()
  {
    اگر (unlikely (in_error ())) بازگشت;

    أكد (!current);
    أكد (packed.length > 1);

    برای (ثابت object_t* parent : ++hb_iter (packed))
      برای (ثابت object_t::link_t &link : parent->real_links)
      {
	ثابت object_t* child = packed[link.objidx];
	اگر (unlikely (!child)) { err (HB_SERIALIZE_ERROR_OTHER); بازگشت; }
	unsigned offset = 0;
	انتخاب ((whence_t) link.whence) {
	حالت Head:     offset = child->head - parent->head; بشکن;
	حالت Tail:     offset = child->head - parent->tail; بشکن;
	حالت Absolute: offset = (head - start) + (child->head - tail); بشکن;
	}

	أكد (offset >= link.bias);
	offset -= link.bias;
	اگر (link.is_signed)
	{
	  أكد (link.width == 2 || link.width == 4);
	  اگر (link.width == 4)
	    assign_offset<int32_t> (parent, link, offset);
	  وگرنه
	    assign_offset<int16_t> (parent, link, offset);
	}
	وگرنه
	{
	  أكد (link.width == 2 || link.width == 3 || link.width == 4);
	  اگر (link.width == 4)
	    assign_offset<uint32_t> (parent, link, offset);
	  وگرنه اگر (link.width == 3)
	    assign_offset<uint32_t, 3> (parent, link, offset);
	  وگرنه
	    assign_offset<uint16_t> (parent, link, offset);
	}
      }
  }

  unsigned صحیح length () ثابت
  {
    اگر (unlikely (!current)) بازگشت 0;
    بازگشت این->head - current->head;
  }

  تهی align (unsigned صحیح alignment)
  {
    unsigned صحیح l = length () % alignment;
    اگر (l)
      (تهی) allocate_size<تهی> (alignment - l);
  }

  template <typename Type = تهی>
  __attribute__((returns_nonnull))
  Type *start_embed (ثابت Type *obj HB_UNUSED = nullptr) ثابت
  { بازگشت reinterpret_cast<Type *> (این->head); }
  template <typename Type>
  __attribute__((returns_nonnull))
  Type *start_embed (ثابت Type &obj) ثابت
  { بازگشت start_embed (std::addressof (obj)); }

  منطقی err (hb_serialize_error_t err_type)
  {
    بازگشت !منطقی ((errors = (errors | err_type)));
  }

  منطقی start_zerocopy (size_t size)
  {
    اگر (unlikely (in_error ())) بازگشت نادرست;

    اگر (unlikely (size > INT_MAX || این->tail - این->head < ptrdiff_t (size)))
    {
      err (HB_SERIALIZE_ERROR_OUT_OF_ROOM);
      بازگشت نادرست;
    }

    أكد (!این->zerocopy);
    این->zerocopy = این->head;

    أكد (این->current->head == این->head);
    این->current->head = این->current->tail = این->head = این->tail - size;
    بازگشت درست;
  }

  template <typename Type>
  HB_NODISCARD
  Type *allocate_size (size_t size, منطقی clear = درست)
  {
    اگر (unlikely (in_error ())) بازگشت nullptr;

    اگر (unlikely (size > INT_MAX || این->tail - این->head < ptrdiff_t (size)))
    {
      err (HB_SERIALIZE_ERROR_OUT_OF_ROOM);
      بازگشت nullptr;
    }
    اگر (clear)
      hb_memset (این->head, 0, size);
    نویسه *ret = این->head;
    این->head += size;
    بازگشت reinterpret_cast<Type *> (ret);
  }

  template <typename Type>
  Type *allocate_min ()
  { بازگشت این->allocate_size<Type> (Type::min_size); }

  template <typename Type>
  HB_NODISCARD
  Type *embed (ثابت Type *obj)
  {
    size_t size = obj->get_size ();
    Type *ret = این->allocate_size<Type> (size, نادرست);
    اگر (unlikely (!ret)) بازگشت nullptr;
    hb_memcpy (ret, obj, size);
    بازگشت ret;
  }
  template <typename Type>
  HB_NODISCARD
  Type *embed (ثابت Type &obj)
  { بازگشت embed (std::addressof (obj)); }
  نویسه *embed (ثابت نویسه *obj, unsigned size)
  {
    نویسه *ret = این->allocate_size<نویسه> (size, نادرست);
    اگر (unlikely (!ret)) بازگشت nullptr;
    hb_memcpy (ret, obj, size);
    بازگشت ret;
  }

  template <typename Type, typename ...Ts> auto
  _copy (ثابت Type &src, hb_priority<1>, Ts&&... ds) HB_RETURN
  (Type *, src.copy (این, std::forward<Ts> (ds)...))

  template <typename Type> auto
  _copy (ثابت Type &src, hb_priority<0>) -> decltype (&(hb_declval<Type> () = src))
  {
    Type *ret = این->allocate_size<Type> (اندازه (Type));
    اگر (unlikely (!ret)) بازگشت nullptr;
    *ret = src;
    بازگشت ret;
  }

  /* Like embed, but active: calls obj.operator=() or obj.copy() to transfer data
   * instead of hb_memcpy(). */
  template <typename Type, typename ...Ts>
  Type *copy (ثابت Type &src, Ts&&... ds)
  { بازگشت _copy (src, hb_prioritize, std::forward<Ts> (ds)...); }
  template <typename Type, typename ...Ts>
  Type *copy (ثابت Type *src, Ts&&... ds)
  { بازگشت copy (*src, std::forward<Ts> (ds)...); }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator)),
	   typename ...Ts>
  تهی copy_all (Iterator it, Ts&&... ds)
  { برای (decltype (*it) _ : it) copy (_, ds...); }

  template <typename Type>
  hb_serialize_context_t& operator << (ثابت Type &obj) & { embed (obj); بازگشت *این; }

  template <typename Type>
  Type *extend_size (Type *obj, size_t size, منطقی clear = درست)
  {
    اگر (unlikely (in_error ())) بازگشت nullptr;
    اگر (unlikely (size >= INT_MAX)) { err (HB_SERIALIZE_ERROR_OTHER); بازگشت nullptr; }

    أكد (این->start <= (نویسه *) obj);
    أكد ((نویسه *) obj <= این->head);
    أكد ((size_t) (این->head - (نویسه *) obj) <= size);
    اگر (unlikely (((نویسه *) obj + size < (نویسه *) obj) ||
		  !این->allocate_size<Type> (((نویسه *) obj) + size - این->head, clear))) بازگشت nullptr;
    بازگشت reinterpret_cast<Type *> (obj);
  }
  template <typename Type>
  Type *extend_size (Type &obj, size_t size, منطقی clear = درست)
  { بازگشت extend_size (std::addressof (obj), size, clear); }

  template <typename Type>
  Type *extend_min (Type *obj) { بازگشت extend_size (obj, obj->min_size); }
  template <typename Type>
  Type *extend_min (Type &obj) { بازگشت extend_min (std::addressof (obj)); }

  template <typename Type, typename ...Ts>
  Type *extend (Type *obj, Ts&&... ds)
  { بازگشت extend_size (obj, obj->get_size (std::forward<Ts> (ds)...)); }
  template <typename Type, typename ...Ts>
  Type *extend (Type &obj, Ts&&... ds)
  { بازگشت extend (std::addressof (obj), std::forward<Ts> (ds)...); }

  /* Output routines. */
  hb_bytes_t copy_bytes () ثابت
  {
    أكد (successful ());
    /* Copy both items from head side and tail side... */
    unsigned صحیح len = (این->head - این->start)
		     + (این->end  - این->tail);

    // If len is zero don't hb_malloc as the memory won't get properly
    // cleaned up later.
    اگر (!len) بازگشت hb_bytes_t ();

    نویسه *p = (نویسه *) hb_malloc (len);
    اگر (unlikely (!p)) بازگشت hb_bytes_t ();

    hb_memcpy (p, این->start, این->head - این->start);
    hb_memcpy (p + (این->head - این->start), این->tail, این->end - این->tail);
    بازگشت hb_bytes_t (p, len);
  }
  template <typename Type>
  Type *copy () ثابت
  { بازگشت reinterpret_cast<Type *> ((نویسه *) copy_bytes ().arrayZ); }
  hb_blob_t *copy_blob () ثابت
  {
    hb_bytes_t b = copy_bytes ();
    بازگشت hb_blob_create (b.arrayZ, b.length,
			   HB_MEMORY_MODE_WRITABLE,
			   (نویسه *) b.arrayZ, hb_free);
  }

  ثابت hb_vector_t<object_t *>& object_graph() ثابت
  { بازگشت packed; }

  خصوصی:
  template <typename T, unsigned Size = اندازه (T)>
  تهی assign_offset (ثابت object_t* parent, ثابت object_t::link_t &link, unsigned offset)
  {
    // XXX We should stop assuming big-endian!
    auto &off = * ((HBInt<درست, T, Size> *) (parent->head + link.position));
    أكد (0 == off);
    check_assign (off, offset, HB_SERIALIZE_ERROR_OFFSET_OVERFLOW);
  }

  عمومی:
  نویسه *start, *head, *tail, *end, *zerocopy;
  unsigned صحیح debug_depth;
  hb_serialize_error_t errors;

  خصوصی:

  تهی merge_virtual_links (ثابت object_t* from, objidx_t to_idx) {
    object_t* to = packed[to_idx];
    برای (ثابت auto& l : from->virtual_links) {
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
  hb_hashmap_t<ثابت object_t *, objidx_t> packed_map;
};

#endif /* HB_SERIALIZE_HH */
