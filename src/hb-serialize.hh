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

#ضمن "hb.hh"
#ضمن "hb-blob.hh"
#ضمن "hb-map.hh"
#ضمن "hb-free-pool.hh"

#ضمن "hb-subset-serialize.h"

/*
 * Serialize
 */

تعداد hb_serialize_error_t {
  HB_SERIALIZE_ERROR_NONE =            0x00000000u,
  HB_SERIALIZE_ERROR_OTHER =           0x00000001u,
  HB_SERIALIZE_ERROR_OFFSET_OVERFLOW = 0x00000002u,
  HB_SERIALIZE_ERROR_OUT_OF_ROOM =     0x00000004u,
  HB_SERIALIZE_ERROR_INT_OVERFLOW =    0x00000008u,
  HB_SERIALIZE_ERROR_ARRAY_OVERFLOW =  0x00000010u
};
HB_MARK_AS_FLAG_T (hb_serialize_error_t);

بنية hb_serialize_context_t
{
  تعريفنوع unsigned objidx_t;

  تعداد whence_t {
     Head,	/* Relative to the current object head (default). */
     Tail,	/* Relative to the current object tail after packed. */
     Absolute	/* Absolute: from the start of the serialize buffer. */
   };



  بنية object_t
  {
    فراغ fini () {
      real_links.fini ();
      virtual_links.fini ();
    }

    object_t () = افتراضي;

    object_t (ثابت hb_subset_serialize_object_t &o)
    {
      head = o.head;
      tail = o.tail;
      next = nullptr;
      real_links.alloc_exact (o.رەقەم_real_links);
      لكل (unsigned i = 0 ; i < o.رەقەم_real_links; i++)
        real_links.push (o.real_links[i]);

      virtual_links.alloc_exact (o.رەقەم_virtual_links);
      لكل (unsigned i = 0; i < o.رەقەم_virtual_links; i++)
        virtual_links.push (o.virtual_links[i]);
    }

    منطقي in_error () ثابت
    {
      أرجع real_links.in_error() || virtual_links.in_error();
    }

    منطقي add_virtual_link (objidx_t objidx)
    {
      إذا (!objidx)
        أرجع خطأ;

      auto& link = *virtual_links.push ();
      إذا (virtual_links.in_error ())
        أرجع خطأ;

      link.objidx = objidx;
      // Remaining fields were previously zero'd by push():
      // link.width = 0;
      // link.is_signed = 0;
      // link.whence = 0;
      // link.position = 0;
      // link.bias = 0;

      أرجع true;
    }

    friend فراغ swap (object_t& a, object_t& b) noexcept
    {
      hb_swap (a.head, b.head);
      hb_swap (a.tail, b.tail);
      hb_swap (a.next, b.next);
      hb_swap (a.real_links, b.real_links);
      hb_swap (a.virtual_links, b.virtual_links);
    }

    منطقي operator == (ثابت object_t &o) ثابت
    {
      // Virtual links aren't considered for equality since they don't affect the functionality
      // of the object.
      أرجع (tail - head == o.tail - o.head)
	  && (real_links.length == o.real_links.length)
	  && 0 == hb_memcmp (head, o.head, tail - head)
	  && real_links.as_bytes () == o.real_links.as_bytes ();
    }
    uint32_t hash () ثابت
    {
      // Virtual links aren't considered for equality since they don't affect the functionality
      // of the object.
      أرجع hb_bytes_t (head, hb_min (128, tail - head)).hash () ^
          real_links.as_bytes ().hash ();
    }

    بنية link_t
    {
      unsigned width: 3;
      unsigned is_signed: 1;
      unsigned whence: 2;
      unsigned bias : 26;
      unsigned position;
      objidx_t objidx;

      link_t () = افتراضي;

      link_t (ثابت hb_subset_serialize_link_t &o)
      {
        width = o.width;
        is_signed = 0;
        whence = 0;
        position = o.position;
        bias = 0;
        objidx = o.objidx;
      }

      HB_INTERNAL ساكن صحيح cmp (ثابت فراغ* a, ثابت فراغ* b)
      {
        صحيح cmp = ((ثابت link_t*)a)->position - ((ثابت link_t*)b)->position;
        إذا (cmp) أرجع cmp;

        أرجع ((ثابت link_t*)a)->objidx - ((ثابت link_t*)b)->objidx;
      }
    };

    حرف *head;
    حرف *tail;
    hb_vector_t<link_t> real_links;
    hb_vector_t<link_t> virtual_links;
    object_t *next;

    auto all_links () ثابت HB_AUTO_RETURN
        (( hb_concat (real_links, virtual_links) ));
    auto all_links_writer () HB_AUTO_RETURN
        (( hb_concat (real_links.writer (), virtual_links.writer ()) ));
  };

  بنية snapshot_t
  {
    حرف *head;
    حرف *tail;
    object_t *current; // Just for sanity check
    unsigned رەقەم_real_links;
    unsigned رەقەم_virtual_links;
    hb_serialize_error_t errors;
  };

  snapshot_t snapshot ()
  {
    أرجع snapshot_t {
      head, tail, current,
      current ? current->real_links.length : 0,
      current ? current->virtual_links.length : 0,
      errors
     };
  }

  hb_serialize_context_t (فراغ *start_, size_t size) :
    start ((حرف *) start_),
    end (start + size),
    current (nullptr)
  { reset (); }
  ~hb_serialize_context_t () { fini (); }

  فراغ fini ()
  {
    لكل (object_t *_ : ++hb_iter (packed)) _->fini ();
    packed.fini ();
    هذا->packed_map.fini ();

    طالما (current)
    {
      auto *_ = current;
      current = current->next;
      _->fini ();
    }
  }

  منطقي in_error () ثابت { أرجع منطقي (errors); }

  منطقي successful () ثابت { أرجع !منطقي (errors); }

  HB_NODISCARD منطقي ran_out_of_room () ثابت { أرجع errors & HB_SERIALIZE_ERROR_OUT_OF_ROOM; }
  HB_NODISCARD منطقي offset_overflow () ثابت { أرجع errors & HB_SERIALIZE_ERROR_OFFSET_OVERFLOW; }
  HB_NODISCARD منطقي only_offset_overflow () ثابت { أرجع errors == HB_SERIALIZE_ERROR_OFFSET_OVERFLOW; }
  HB_NODISCARD منطقي only_overflow () ثابت
  {
    أرجع errors == HB_SERIALIZE_ERROR_OFFSET_OVERFLOW
        || errors == HB_SERIALIZE_ERROR_INT_OVERFLOW
        || errors == HB_SERIALIZE_ERROR_ARRAY_OVERFLOW;
  }

  فراغ reset (فراغ *start_, size_t size)
  {
    start = (حرف*) start_;
    end = start + size;
    reset ();
    current = nullptr;
  }

  فراغ reset ()
  {
    هذا->errors = HB_SERIALIZE_ERROR_NONE;
    هذا->head = هذا->start;
    هذا->tail = هذا->end;
    هذا->zerocopy = nullptr;
    هذا->debug_depth = 0;

    fini ();
    هذا->packed.push (nullptr);
    هذا->packed_map.init ();
  }

  منطقي check_success (منطقي success,
                      hb_serialize_error_t err_type = HB_SERIALIZE_ERROR_OTHER)
  {
    أرجع successful ()
        && (success || err (err_type));
  }

  template <typename T1, typename T2>
  منطقي check_equal (T1 &&v1, T2 &&v2, hb_serialize_error_t err_type)
  {
    إذا ((long long) v1 != (long long) v2)
    {
      أرجع err (err_type);
    }
    أرجع true;
  }

  template <typename T1, typename T2>
  منطقي check_assign (T1 &v1, T2 &&v2, hb_serialize_error_t err_type)
  { أرجع check_equal (v1 = v2, v2, err_type); }

  template <typename T> منطقي propagate_error (T &&obj)
  { أرجع check_success (!hb_deref (obj).in_error ()); }

  template <typename T1, typename... Ts> منطقي propagate_error (T1 &&o1, Ts&&... os)
  { أرجع propagate_error (std::forward<T1> (o1)) &&
	   propagate_error (std::forward<Ts> (os)...); }

  /* To be called around main operation. */
  template <typename Type=حرف>
  __attribute__((returns_nonnull))
  Type *start_serialize ()
  {
    DEBUG_ئۇچۇر_LEVEL (SERIALIZE, هذا->start, 0, +1,
		     "start [%p..%p] (%lu bytes)",
		     هذا->start, هذا->end,
		     (unsigned long) (هذا->end - هذا->start));

    أكد (!current);
    أرجع push<Type> ();
  }
  فراغ end_serialize ()
  {
    DEBUG_ئۇچۇر_LEVEL (SERIALIZE, هذا->start, 0, -1,
		     "end [%p..%p] serialized %u bytes; %s",
		     هذا->start, هذا->end,
		     (unsigned) (هذا->head - هذا->start),
		     successful () ? "successful" : "UNSUCCESSFUL");

    propagate_error (packed, packed_map);

    إذا (unlikely (!current)) أرجع;
    إذا (unlikely (in_error()))
    {
      // Offset overflows that occur before link resolution cannot be handled
      // by repacking, so set a more general error.
      إذا (offset_overflow ()) err (HB_SERIALIZE_ERROR_OTHER);
      أرجع;
    }

    أكد (!current->next);

    /* Only "pack" if there exist other objects... Otherwise, don't bother.
     * Saves a move. */
    إذا (packed.length <= 1)
      أرجع;

    چىقىرىش_pack (خطأ);

    resolve_links ();
  }

  template <typename Type = فراغ>
  __attribute__((returns_nonnull))
  Type *push ()
  {
    إذا (unlikely (in_error ())) أرجع start_embed<Type> ();

    object_t *obj = object_pool.alloc ();
    إذا (unlikely (!obj))
      check_success (خطأ);
    وإلا
    {
      obj->head = head;
      obj->tail = tail;
      obj->next = current;
      current = obj;
    }
    أرجع start_embed<Type> ();
  }
  فراغ چىقىرىش_discard ()
  {
    object_t *obj = current;
    إذا (unlikely (!obj)) أرجع;
    // Allow cleanup when we've error'd out on int overflows which don't compromise
    // the serializer state.
    إذا (unlikely (in_error() && !only_overflow ())) أرجع;

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
  objidx_t چىقىرىش_pack (منطقي share=true)
  {
    object_t *obj = current;
    إذا (unlikely (!obj)) أرجع 0;
    // Allow cleanup when we've error'd out on int overflows which don't compromise
    // the serializer state.
    إذا (unlikely (in_error()  && !only_overflow ())) أرجع 0;

    current = current->next;
    obj->tail = head;
    obj->next = nullptr;
    أكد (obj->head <= obj->tail);
    unsigned len = obj->tail - obj->head;
    head = zerocopy ? zerocopy : obj->head; /* Rewind head. */
    منطقي was_zerocopy = zerocopy;
    zerocopy = nullptr;

    إذا (!len)
    {
      أكد (!obj->real_links.length);
      أكد (!obj->virtual_links.length);
      أرجع 0;
    }

    objidx_t objidx;
    uint32_t hash = 0;
    إذا (share)
    {
      hash = hb_hash (obj);
      objidx = packed_map.get_with_hash (obj, hash);
      إذا (objidx)
      {
        merge_virtual_links (obj, objidx);
	obj->fini ();
        object_pool.release (obj);
	أرجع objidx;
      }
    }

    tail -= len;
    إذا (was_zerocopy)
      أكد (tail == obj->head);
    وإلا
      memmove (tail, obj->head, len);

    obj->head = tail;
    obj->tail = tail + len;

    packed.push (obj);

    إذا (unlikely (!propagate_error (packed)))
    {
      /* Obj wasn't successfully added to packed, so clean it up otherwise its
       * links will be leaked. When we use constructor/destructors properly, we
       * can remove these. */
      obj->fini ();
      أرجع 0;
    }

    objidx = packed.length - 1;

    إذا (share) packed_map.set_with_hash (obj, hash, objidx);
    propagate_error (packed_map);

    أرجع objidx;
  }

  فراغ revert (snapshot_t snap)
  {
    // Overflows that happened after the snapshot will be erased by the revert.
    إذا (unlikely (in_error () && !only_overflow ())) أرجع;
    أكد (snap.current == current);
    إذا (current)
    {
      current->real_links.shrink (snap.رەقەم_real_links);
      current->virtual_links.shrink (snap.رەقەم_virtual_links);
    }
    errors = snap.errors;
    revert (snap.head, snap.tail);
  }

  فراغ revert (حرف *snap_head,
	       حرف *snap_tail)
  {
    إذا (unlikely (in_error ())) أرجع;
    أكد (snap_head <= head);
    أكد (tail <= snap_tail);
    head = snap_head;
    tail = snap_tail;
    discard_stale_objects ();
  }

  فراغ discard_stale_objects ()
  {
    إذا (unlikely (in_error ())) أرجع;
    طالما (packed.length > 1 &&
	   packed.tail ()->head < tail)
    {
      object_t *obj = packed.tail ();
      packed_map.del (obj);
      أكد (!obj->next);
      obj->fini ();
      object_pool.release (obj);
      packed.چىقىرىش ();
    }
    إذا (packed.length > 1)
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
  فراغ add_virtual_link (objidx_t objidx)
  {
    إذا (unlikely (in_error ())) أرجع;

    إذا (!objidx)
      أرجع;

    أكد (current);

    إذا (!current->add_virtual_link(objidx))
      err (HB_SERIALIZE_ERROR_OTHER);
  }

  objidx_t last_added_child_index() ثابت {
    إذا (unlikely (in_error ())) أرجع (objidx_t) -1;

    أكد (current);
    إذا (!منطقي(current->real_links)) {
      أرجع (objidx_t) -1;
    }

    أرجع current->real_links[current->real_links.length - 1].objidx;
  }

  // For the current object ensure that the sub-table bytes for child objidx are always placed
  // after the subtable bytes for any other existing children. This only ensures that the
  // repacker will not move the target subtable before the other children
  // (by adding virtual links). It is up to the caller to ensure the initial serialization
  // order is correct.
  فراغ repack_last(objidx_t objidx) {
    إذا (unlikely (in_error ())) أرجع;

    إذا (!objidx)
      أرجع;

    أكد (current);
    لكل (auto& l : current->real_links) {
      إذا (l.objidx == objidx) {
        استمر;
      }

      packed[l.objidx]->add_virtual_link(objidx);
    }
  }

  template <typename T>
  فراغ add_link (T &ofs, objidx_t objidx,
		 whence_t whence = Head,
		 unsigned bias = 0)
  {
    إذا (unlikely (in_error ())) أرجع;

    إذا (!objidx)
      أرجع;

    أكد (current);
    أكد (current->head <= (ثابت حرف *) &ofs);

    auto& link = *current->real_links.push ();
    إذا (current->real_links.in_error ())
      err (HB_SERIALIZE_ERROR_OTHER);

    link.width = حجم (T);
    link.objidx = objidx;
    إذا (unlikely (!حجم (T)))
    {
      // This link is not associated with an actual offset and exists merely to enforce
      // an ordering constraint.
      link.is_signed = 0;
      link.whence = 0;
      link.position = 0;
      link.bias = 0;
      أرجع;
    }

    link.is_signed = std::is_signed<hb_unwrap_type (T)>::value;
    link.whence = (unsigned) whence;
    link.position = (ثابت حرف *) &ofs - current->head;
    link.bias = bias;
  }

  unsigned to_bias (ثابت فراغ *base) ثابت
  {
    إذا (unlikely (in_error ())) أرجع 0;
    إذا (!base) أرجع 0;
    أكد (current);
    أكد (current->head <= (ثابت حرف *) base);
    أرجع (ثابت حرف *) base - current->head;
  }

  فراغ resolve_links ()
  {
    إذا (unlikely (in_error ())) أرجع;

    أكد (!current);
    أكد (packed.length > 1);

    لكل (ثابت object_t* parent : ++hb_iter (packed))
      لكل (ثابت object_t::link_t &link : parent->real_links)
      {
	ثابت object_t* child = packed[link.objidx];
	إذا (unlikely (!child)) { err (HB_SERIALIZE_ERROR_OTHER); أرجع; }
	unsigned offset = 0;
	بدل ((whence_t) link.whence) {
	حالة Head:     offset = child->head - parent->head; اكسر;
	حالة Tail:     offset = child->head - parent->tail; اكسر;
	حالة Absolute: offset = (head - start) + (child->head - tail); اكسر;
	}

	أكد (offset >= link.bias);
	offset -= link.bias;
	إذا (link.is_signed)
	{
	  أكد (link.width == 2 || link.width == 4);
	  إذا (link.width == 4)
	    assign_offset<int32_t> (parent, link, offset);
	  وإلا
	    assign_offset<int16_t> (parent, link, offset);
	}
	وإلا
	{
	  أكد (link.width == 2 || link.width == 3 || link.width == 4);
	  إذا (link.width == 4)
	    assign_offset<uint32_t> (parent, link, offset);
	  وإلا إذا (link.width == 3)
	    assign_offset<uint32_t, 3> (parent, link, offset);
	  وإلا
	    assign_offset<uint16_t> (parent, link, offset);
	}
      }
  }

  unsigned صحيح length () ثابت
  {
    إذا (unlikely (!current)) أرجع 0;
    أرجع هذا->head - current->head;
  }

  فراغ align (unsigned صحيح alignment)
  {
    unsigned صحيح l = length () % alignment;
    إذا (l)
      (فراغ) allocate_size<فراغ> (alignment - l);
  }

  template <typename Type = فراغ>
  __attribute__((returns_nonnull))
  Type *start_embed (ثابت Type *obj HB_UNUSED = nullptr) ثابت
  { أرجع reinterpret_cast<Type *> (هذا->head); }
  template <typename Type>
  __attribute__((returns_nonnull))
  Type *start_embed (ثابت Type &obj) ثابت
  { أرجع start_embed (std::addressof (obj)); }

  منطقي err (hb_serialize_error_t err_type)
  {
    أرجع !منطقي ((errors = (errors | err_type)));
  }

  منطقي start_zerocopy (size_t size)
  {
    إذا (unlikely (in_error ())) أرجع خطأ;

    إذا (unlikely (size > INT_MAX || هذا->tail - هذا->head < ptrdiff_t (size)))
    {
      err (HB_SERIALIZE_ERROR_OUT_OF_ROOM);
      أرجع خطأ;
    }

    أكد (!هذا->zerocopy);
    هذا->zerocopy = هذا->head;

    أكد (هذا->current->head == هذا->head);
    هذا->current->head = هذا->current->tail = هذا->head = هذا->tail - size;
    أرجع true;
  }

  template <typename Type>
  HB_NODISCARD
  Type *allocate_size (size_t size, منطقي clear = true)
  {
    إذا (unlikely (in_error ())) أرجع nullptr;

    إذا (unlikely (size > INT_MAX || هذا->tail - هذا->head < ptrdiff_t (size)))
    {
      err (HB_SERIALIZE_ERROR_OUT_OF_ROOM);
      أرجع nullptr;
    }
    إذا (clear)
      hb_memset (هذا->head, 0, size);
    حرف *ret = هذا->head;
    هذا->head += size;
    أرجع reinterpret_cast<Type *> (ret);
  }

  template <typename Type>
  Type *allocate_min ()
  { أرجع هذا->allocate_size<Type> (Type::min_size); }

  template <typename Type>
  HB_NODISCARD
  Type *embed (ثابت Type *obj)
  {
    size_t size = obj->get_size ();
    Type *ret = هذا->allocate_size<Type> (size, خطأ);
    إذا (unlikely (!ret)) أرجع nullptr;
    hb_memcpy (ret, obj, size);
    أرجع ret;
  }
  template <typename Type>
  HB_NODISCARD
  Type *embed (ثابت Type &obj)
  { أرجع embed (std::addressof (obj)); }
  حرف *embed (ثابت حرف *obj, unsigned size)
  {
    حرف *ret = هذا->allocate_size<حرف> (size, خطأ);
    إذا (unlikely (!ret)) أرجع nullptr;
    hb_memcpy (ret, obj, size);
    أرجع ret;
  }

  template <typename Type, typename ...Ts> auto
  _copy (ثابت Type &src, hb_priority<1>, Ts&&... ds) HB_RETURN
  (Type *, src.copy (هذا, std::forward<Ts> (ds)...))

  template <typename Type> auto
  _copy (ثابت Type &src, hb_priority<0>) -> decltype (&(hb_declval<Type> () = src))
  {
    Type *ret = هذا->allocate_size<Type> (حجم (Type));
    إذا (unlikely (!ret)) أرجع nullptr;
    *ret = src;
    أرجع ret;
  }

  /* Like embed, but active: calls obj.operator=() or obj.copy() to transfer data
   * instead of hb_memcpy(). */
  template <typename Type, typename ...Ts>
  Type *copy (ثابت Type &src, Ts&&... ds)
  { أرجع _copy (src, hb_prioritize, std::forward<Ts> (ds)...); }
  template <typename Type, typename ...Ts>
  Type *copy (ثابت Type *src, Ts&&... ds)
  { أرجع copy (*src, std::forward<Ts> (ds)...); }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator)),
	   typename ...Ts>
  فراغ copy_all (Iterator it, Ts&&... ds)
  { لكل (decltype (*it) _ : it) copy (_, ds...); }

  template <typename Type>
  hb_serialize_context_t& operator << (ثابت Type &obj) & { embed (obj); أرجع *هذا; }

  template <typename Type>
  Type *extend_size (Type *obj, size_t size, منطقي clear = true)
  {
    إذا (unlikely (in_error ())) أرجع nullptr;
    إذا (unlikely (size >= INT_MAX)) { err (HB_SERIALIZE_ERROR_OTHER); أرجع nullptr; }

    أكد (هذا->start <= (حرف *) obj);
    أكد ((حرف *) obj <= هذا->head);
    أكد ((size_t) (هذا->head - (حرف *) obj) <= size);
    إذا (unlikely (((حرف *) obj + size < (حرف *) obj) ||
		  !هذا->allocate_size<Type> (((حرف *) obj) + size - هذا->head, clear))) أرجع nullptr;
    أرجع reinterpret_cast<Type *> (obj);
  }
  template <typename Type>
  Type *extend_size (Type &obj, size_t size, منطقي clear = true)
  { أرجع extend_size (std::addressof (obj), size, clear); }

  template <typename Type>
  Type *extend_min (Type *obj) { أرجع extend_size (obj, obj->min_size); }
  template <typename Type>
  Type *extend_min (Type &obj) { أرجع extend_min (std::addressof (obj)); }

  template <typename Type, typename ...Ts>
  Type *extend (Type *obj, Ts&&... ds)
  { أرجع extend_size (obj, obj->get_size (std::forward<Ts> (ds)...)); }
  template <typename Type, typename ...Ts>
  Type *extend (Type &obj, Ts&&... ds)
  { أرجع extend (std::addressof (obj), std::forward<Ts> (ds)...); }

  /* Output routines. */
  hb_bytes_t copy_bytes () ثابت
  {
    أكد (successful ());
    /* Copy both items from head side and tail side... */
    unsigned صحيح len = (هذا->head - هذا->start)
		     + (هذا->end  - هذا->tail);

    // If len is zero don't hb_malloc as the memory won't get properly
    // cleaned up later.
    إذا (!len) أرجع hb_bytes_t ();

    حرف *p = (حرف *) hb_malloc (len);
    إذا (unlikely (!p)) أرجع hb_bytes_t ();

    hb_memcpy (p, هذا->start, هذا->head - هذا->start);
    hb_memcpy (p + (هذا->head - هذا->start), هذا->tail, هذا->end - هذا->tail);
    أرجع hb_bytes_t (p, len);
  }
  template <typename Type>
  Type *copy () ثابت
  { أرجع reinterpret_cast<Type *> ((حرف *) copy_bytes ().arrayZ); }
  hb_blob_t *copy_blob () ثابت
  {
    hb_bytes_t b = copy_bytes ();
    أرجع hb_blob_create (b.arrayZ, b.length,
			   HB_MEMORY_MODE_WRITABLE,
			   (حرف *) b.arrayZ, hb_free);
  }

  ثابت hb_vector_t<object_t *>& object_graph() ثابت
  { أرجع packed; }

  خاص:
  template <typename T, unsigned Size = حجم (T)>
  فراغ assign_offset (ثابت object_t* parent, ثابت object_t::link_t &link, unsigned offset)
  {
    // XXX We should stop assuming big-endian!
    auto &off = * ((HBInt<true, T, Size> *) (parent->head + link.position));
    أكد (0 == off);
    check_assign (off, offset, HB_SERIALIZE_ERROR_OFFSET_OVERFLOW);
  }

  عام:
  حرف *start, *head, *tail, *end, *zerocopy;
  unsigned صحيح debug_depth;
  hb_serialize_error_t errors;

  خاص:

  فراغ merge_virtual_links (ثابت object_t* from, objidx_t to_idx) {
    object_t* to = packed[to_idx];
    لكل (ثابت auto& l : from->virtual_links) {
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
