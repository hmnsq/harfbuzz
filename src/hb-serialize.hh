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

#અન્તર્ભાવ "hb.hh"
#અન્તર્ભાવ "hb-blob.hh"
#અન્તર્ભાવ "hb-map.hh"
#અન્તર્ભાવ "hb-free-pool.hh"

#અન્તર્ભાવ "hb-subset-serialize.h"

/*
 * Serialize
 */

ગણના hb_serialize_error_t {
  HB_SERIALIZE_ERROR_NONE =            0x00000000u,
  HB_SERIALIZE_ERROR_OTHER =           0x00000001u,
  HB_SERIALIZE_ERROR_OFFSET_OVERFLOW = 0x00000002u,
  HB_SERIALIZE_ERROR_OUT_OF_ROOM =     0x00000004u,
  HB_SERIALIZE_ERROR_INT_OVERFLOW =    0x00000008u,
  HB_SERIALIZE_ERROR_ARRAY_OVERFLOW =  0x00000010u
};
HB_MARK_AS_FLAG_T (hb_serialize_error_t);

સંરચના hb_serialize_context_t
{
  પ્રકારવ્યાખ્યા unsigned objidx_t;

  ગણના whence_t {
     Head,	/* Relative to the current object head (default). */
     Tail,	/* Relative to the current object tail after packed. */
     Absolute	/* Absolute: from the start of the serialize buffer. */
   };



  સંરચના object_t
  {
    શૂન્ય fini () {
      real_links.fini ();
      virtual_links.fini ();
    }

    object_t () = મૂળભૂત;

    object_t (અચલ hb_subset_serialize_object_t &o)
    {
      head = o.head;
      tail = o.tail;
      next = nullptr;
      real_links.alloc_exact (o.અંક_real_links);
      માટે (unsigned i = 0 ; i < o.અંક_real_links; i++)
        real_links.push (o.real_links[i]);

      virtual_links.alloc_exact (o.અંક_virtual_links);
      માટે (unsigned i = 0; i < o.અંક_virtual_links; i++)
        virtual_links.push (o.virtual_links[i]);
    }

    તાર્કિક in_error () અચલ
    {
      પાછા real_links.in_error() || virtual_links.in_error();
    }

    તાર્કિક add_virtual_link (objidx_t objidx)
    {
      જો (!objidx)
        પાછા ખોટું;

      auto& link = *virtual_links.push ();
      જો (virtual_links.in_error ())
        પાછા ખોટું;

      link.objidx = objidx;
      // Remaining fields were previously zero'd by push():
      // link.width = 0;
      // link.is_signed = 0;
      // link.whence = 0;
      // link.position = 0;
      // link.bias = 0;

      પાછા સાચું;
    }

    friend શૂન્ય swap (object_t& a, object_t& b) noexcept
    {
      hb_swap (a.head, b.head);
      hb_swap (a.tail, b.tail);
      hb_swap (a.next, b.next);
      hb_swap (a.real_links, b.real_links);
      hb_swap (a.virtual_links, b.virtual_links);
    }

    તાર્કિક operator == (અચલ object_t &o) અચલ
    {
      // Virtual links aren't considered for equality since they don't affect the functionality
      // of the object.
      પાછા (tail - head == o.tail - o.head)
	  && (real_links.length == o.real_links.length)
	  && 0 == hb_memcmp (head, o.head, tail - head)
	  && real_links.as_bytes () == o.real_links.as_bytes ();
    }
    uint32_t hash () અચલ
    {
      // Virtual links aren't considered for equality since they don't affect the functionality
      // of the object.
      પાછા hb_bytes_t (head, hb_min (128, tail - head)).hash () ^
          real_links.as_bytes ().hash ();
    }

    સંરચના link_t
    {
      unsigned width: 3;
      unsigned is_signed: 1;
      unsigned whence: 2;
      unsigned bias : 26;
      unsigned position;
      objidx_t objidx;

      link_t () = મૂળભૂત;

      link_t (અચલ hb_subset_serialize_link_t &o)
      {
        width = o.width;
        is_signed = 0;
        whence = 0;
        position = o.position;
        bias = 0;
        objidx = o.objidx;
      }

      HB_INTERNAL સ્થિર પૂર્ણાંક cmp (અચલ શૂન્ય* a, અચલ શૂન્ય* b)
      {
        પૂર્ણાંક cmp = ((અચલ link_t*)a)->position - ((અચલ link_t*)b)->position;
        જો (cmp) પાછા cmp;

        પાછા ((અચલ link_t*)a)->objidx - ((અચલ link_t*)b)->objidx;
      }
    };

    અક્ષર *head;
    અક્ષર *tail;
    hb_vector_t<link_t> real_links;
    hb_vector_t<link_t> virtual_links;
    object_t *next;

    auto all_links () અચલ HB_AUTO_RETURN
        (( hb_concat (real_links, virtual_links) ));
    auto all_links_writer () HB_AUTO_RETURN
        (( hb_concat (real_links.writer (), virtual_links.writer ()) ));
  };

  સંરચના snapshot_t
  {
    અક્ષર *head;
    અક્ષર *tail;
    object_t *current; // Just for sanity check
    unsigned અંક_real_links;
    unsigned અંક_virtual_links;
    hb_serialize_error_t errors;
  };

  snapshot_t snapshot ()
  {
    પાછા snapshot_t {
      head, tail, current,
      current ? current->real_links.length : 0,
      current ? current->virtual_links.length : 0,
      errors
     };
  }

  hb_serialize_context_t (શૂન્ય *start_, size_t size) :
    start ((અક્ષર *) start_),
    end (start + size),
    current (nullptr)
  { reset (); }
  ~hb_serialize_context_t () { fini (); }

  શૂન્ય fini ()
  {
    માટે (object_t *_ : ++hb_iter (packed)) _->fini ();
    packed.fini ();
    આ->packed_map.fini ();

    જ્યારે (current)
    {
      auto *_ = current;
      current = current->next;
      _->fini ();
    }
  }

  તાર્કિક in_error () અચલ { પાછા તાર્કિક (errors); }

  તાર્કિક successful () અચલ { પાછા !તાર્કિક (errors); }

  HB_NODISCARD તાર્કિક ran_out_of_room () અચલ { પાછા errors & HB_SERIALIZE_ERROR_OUT_OF_ROOM; }
  HB_NODISCARD તાર્કિક offset_overflow () અચલ { પાછા errors & HB_SERIALIZE_ERROR_OFFSET_OVERFLOW; }
  HB_NODISCARD તાર્કિક only_offset_overflow () અચલ { પાછા errors == HB_SERIALIZE_ERROR_OFFSET_OVERFLOW; }
  HB_NODISCARD તાર્કિક only_overflow () અચલ
  {
    પાછા errors == HB_SERIALIZE_ERROR_OFFSET_OVERFLOW
        || errors == HB_SERIALIZE_ERROR_INT_OVERFLOW
        || errors == HB_SERIALIZE_ERROR_ARRAY_OVERFLOW;
  }

  શૂન્ય reset (શૂન્ય *start_, size_t size)
  {
    start = (અક્ષર*) start_;
    end = start + size;
    reset ();
    current = nullptr;
  }

  શૂન્ય reset ()
  {
    આ->errors = HB_SERIALIZE_ERROR_NONE;
    આ->head = આ->start;
    આ->tail = આ->end;
    આ->zerocopy = nullptr;
    આ->debug_depth = 0;

    fini ();
    આ->packed.push (nullptr);
    આ->packed_map.init ();
  }

  તાર્કિક check_success (તાર્કિક success,
                      hb_serialize_error_t err_type = HB_SERIALIZE_ERROR_OTHER)
  {
    પાછા successful ()
        && (success || err (err_type));
  }

  template <typename T1, typename T2>
  તાર્કિક check_equal (T1 &&v1, T2 &&v2, hb_serialize_error_t err_type)
  {
    જો ((long long) v1 != (long long) v2)
    {
      પાછા err (err_type);
    }
    પાછા સાચું;
  }

  template <typename T1, typename T2>
  તાર્કિક check_assign (T1 &v1, T2 &&v2, hb_serialize_error_t err_type)
  { પાછા check_equal (v1 = v2, v2, err_type); }

  template <typename T> તાર્કિક propagate_error (T &&obj)
  { પાછા check_success (!hb_deref (obj).in_error ()); }

  template <typename T1, typename... Ts> તાર્કિક propagate_error (T1 &&o1, Ts&&... os)
  { પાછા propagate_error (std::forward<T1> (o1)) &&
	   propagate_error (std::forward<Ts> (os)...); }

  /* To be called around main operation. */
  template <typename Type=અક્ષર>
  __attribute__((returns_nonnull))
  Type *start_serialize ()
  {
    DEBUG_સંદેશ_LEVEL (SERIALIZE, આ->start, 0, +1,
		     "start [%p..%p] (%lu bytes)",
		     આ->start, આ->end,
		     (unsigned long) (આ->end - આ->start));

    પ્રતિજ્ઞા (!current);
    પાછા push<Type> ();
  }
  શૂન્ય end_serialize ()
  {
    DEBUG_સંદેશ_LEVEL (SERIALIZE, આ->start, 0, -1,
		     "end [%p..%p] serialized %u bytes; %s",
		     આ->start, આ->end,
		     (unsigned) (આ->head - આ->start),
		     successful () ? "successful" : "UNSUCCESSFUL");

    propagate_error (packed, packed_map);

    જો (unlikely (!current)) પાછા;
    જો (unlikely (in_error()))
    {
      // Offset overflows that occur before link resolution cannot be handled
      // by repacking, so set a more general error.
      જો (offset_overflow ()) err (HB_SERIALIZE_ERROR_OTHER);
      પાછા;
    }

    પ્રતિજ્ઞા (!current->next);

    /* Only "pack" if there exist other objects... Otherwise, don't bother.
     * Saves a move. */
    જો (packed.length <= 1)
      પાછા;

    કાઢો_pack (ખોટું);

    resolve_links ();
  }

  template <typename Type = શૂન્ય>
  __attribute__((returns_nonnull))
  Type *push ()
  {
    જો (unlikely (in_error ())) પાછા start_embed<Type> ();

    object_t *obj = object_pool.alloc ();
    જો (unlikely (!obj))
      check_success (ખોટું);
    નહીંતર
    {
      obj->head = head;
      obj->tail = tail;
      obj->next = current;
      current = obj;
    }
    પાછા start_embed<Type> ();
  }
  શૂન્ય કાઢો_discard ()
  {
    object_t *obj = current;
    જો (unlikely (!obj)) પાછા;
    // Allow cleanup when we've error'd out on int overflows which don't compromise
    // the serializer state.
    જો (unlikely (in_error() && !only_overflow ())) પાછા;

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
  objidx_t કાઢો_pack (તાર્કિક share=સાચું)
  {
    object_t *obj = current;
    જો (unlikely (!obj)) પાછા 0;
    // Allow cleanup when we've error'd out on int overflows which don't compromise
    // the serializer state.
    જો (unlikely (in_error()  && !only_overflow ())) પાછા 0;

    current = current->next;
    obj->tail = head;
    obj->next = nullptr;
    પ્રતિજ્ઞા (obj->head <= obj->tail);
    unsigned len = obj->tail - obj->head;
    head = zerocopy ? zerocopy : obj->head; /* Rewind head. */
    તાર્કિક was_zerocopy = zerocopy;
    zerocopy = nullptr;

    જો (!len)
    {
      પ્રતિજ્ઞા (!obj->real_links.length);
      પ્રતિજ્ઞા (!obj->virtual_links.length);
      પાછા 0;
    }

    objidx_t objidx;
    uint32_t hash = 0;
    જો (share)
    {
      hash = hb_hash (obj);
      objidx = packed_map.get_with_hash (obj, hash);
      જો (objidx)
      {
        merge_virtual_links (obj, objidx);
	obj->fini ();
        object_pool.release (obj);
	પાછા objidx;
      }
    }

    tail -= len;
    જો (was_zerocopy)
      પ્રતિજ્ઞા (tail == obj->head);
    નહીંતર
      memmove (tail, obj->head, len);

    obj->head = tail;
    obj->tail = tail + len;

    packed.push (obj);

    જો (unlikely (!propagate_error (packed)))
    {
      /* Obj wasn't successfully added to packed, so clean it up otherwise its
       * links will be leaked. When we use constructor/destructors properly, we
       * can remove these. */
      obj->fini ();
      પાછા 0;
    }

    objidx = packed.length - 1;

    જો (share) packed_map.set_with_hash (obj, hash, objidx);
    propagate_error (packed_map);

    પાછા objidx;
  }

  શૂન્ય revert (snapshot_t snap)
  {
    // Overflows that happened after the snapshot will be erased by the revert.
    જો (unlikely (in_error () && !only_overflow ())) પાછા;
    પ્રતિજ્ઞા (snap.current == current);
    જો (current)
    {
      current->real_links.shrink (snap.અંક_real_links);
      current->virtual_links.shrink (snap.અંક_virtual_links);
    }
    errors = snap.errors;
    revert (snap.head, snap.tail);
  }

  શૂન્ય revert (અક્ષર *snap_head,
	       અક્ષર *snap_tail)
  {
    જો (unlikely (in_error ())) પાછા;
    પ્રતિજ્ઞા (snap_head <= head);
    પ્રતિજ્ઞા (tail <= snap_tail);
    head = snap_head;
    tail = snap_tail;
    discard_stale_objects ();
  }

  શૂન્ય discard_stale_objects ()
  {
    જો (unlikely (in_error ())) પાછા;
    જ્યારે (packed.length > 1 &&
	   packed.tail ()->head < tail)
    {
      object_t *obj = packed.tail ();
      packed_map.del (obj);
      પ્રતિજ્ઞા (!obj->next);
      obj->fini ();
      object_pool.release (obj);
      packed.કાઢો ();
    }
    જો (packed.length > 1)
      પ્રતિજ્ઞા (packed.tail ()->head == tail);
  }

  // Adds a virtual link from the current object to objidx. A virtual link is not associated with
  // an actual offset field. They are solely used to enforce ordering constraints between objects.
  // Adding a virtual link from object a to object b will ensure that object b is always packed after
  // object a in the final serialized order.
  //
  // This is useful in certain situations where there needs to be a specific ordering in the
  // final serialization. Such as when platform bugs require certain orderings, or to provide
  //  guidance to the repacker for better offset overflow resolution.
  શૂન્ય add_virtual_link (objidx_t objidx)
  {
    જો (unlikely (in_error ())) પાછા;

    જો (!objidx)
      પાછા;

    પ્રતિજ્ઞા (current);

    જો (!current->add_virtual_link(objidx))
      err (HB_SERIALIZE_ERROR_OTHER);
  }

  objidx_t last_added_child_index() અચલ {
    જો (unlikely (in_error ())) પાછા (objidx_t) -1;

    પ્રતિજ્ઞા (current);
    જો (!તાર્કિક(current->real_links)) {
      પાછા (objidx_t) -1;
    }

    પાછા current->real_links[current->real_links.length - 1].objidx;
  }

  // For the current object ensure that the sub-table bytes for child objidx are always placed
  // after the subtable bytes for any other existing children. This only ensures that the
  // repacker will not move the target subtable before the other children
  // (by adding virtual links). It is up to the caller to ensure the initial serialization
  // order is correct.
  શૂન્ય repack_last(objidx_t objidx) {
    જો (unlikely (in_error ())) પાછા;

    જો (!objidx)
      પાછા;

    પ્રતિજ્ઞા (current);
    માટે (auto& l : current->real_links) {
      જો (l.objidx == objidx) {
        ચાલુ;
      }

      packed[l.objidx]->add_virtual_link(objidx);
    }
  }

  template <typename T>
  શૂન્ય add_link (T &ofs, objidx_t objidx,
		 whence_t whence = Head,
		 unsigned bias = 0)
  {
    જો (unlikely (in_error ())) પાછા;

    જો (!objidx)
      પાછા;

    પ્રતિજ્ઞા (current);
    પ્રતિજ્ઞા (current->head <= (અચલ અક્ષર *) &ofs);

    auto& link = *current->real_links.push ();
    જો (current->real_links.in_error ())
      err (HB_SERIALIZE_ERROR_OTHER);

    link.width = કદ (T);
    link.objidx = objidx;
    જો (unlikely (!કદ (T)))
    {
      // This link is not associated with an actual offset and exists merely to enforce
      // an ordering constraint.
      link.is_signed = 0;
      link.whence = 0;
      link.position = 0;
      link.bias = 0;
      પાછા;
    }

    link.is_signed = std::is_signed<hb_unwrap_type (T)>::value;
    link.whence = (unsigned) whence;
    link.position = (અચલ અક્ષર *) &ofs - current->head;
    link.bias = bias;
  }

  unsigned to_bias (અચલ શૂન્ય *base) અચલ
  {
    જો (unlikely (in_error ())) પાછા 0;
    જો (!base) પાછા 0;
    પ્રતિજ્ઞા (current);
    પ્રતિજ્ઞા (current->head <= (અચલ અક્ષર *) base);
    પાછા (અચલ અક્ષર *) base - current->head;
  }

  શૂન્ય resolve_links ()
  {
    જો (unlikely (in_error ())) પાછા;

    પ્રતિજ્ઞા (!current);
    પ્રતિજ્ઞા (packed.length > 1);

    માટે (અચલ object_t* parent : ++hb_iter (packed))
      માટે (અચલ object_t::link_t &link : parent->real_links)
      {
	અચલ object_t* child = packed[link.objidx];
	જો (unlikely (!child)) { err (HB_SERIALIZE_ERROR_OTHER); પાછા; }
	unsigned offset = 0;
	પસંદગી ((whence_t) link.whence) {
	સ્થિતિ Head:     offset = child->head - parent->head; તોડો;
	સ્થિતિ Tail:     offset = child->head - parent->tail; તોડો;
	સ્થિતિ Absolute: offset = (head - start) + (child->head - tail); તોડો;
	}

	પ્રતિજ્ઞા (offset >= link.bias);
	offset -= link.bias;
	જો (link.is_signed)
	{
	  પ્રતિજ્ઞા (link.width == 2 || link.width == 4);
	  જો (link.width == 4)
	    assign_offset<int32_t> (parent, link, offset);
	  નહીંતર
	    assign_offset<int16_t> (parent, link, offset);
	}
	નહીંતર
	{
	  પ્રતિજ્ઞા (link.width == 2 || link.width == 3 || link.width == 4);
	  જો (link.width == 4)
	    assign_offset<uint32_t> (parent, link, offset);
	  નહીંતર જો (link.width == 3)
	    assign_offset<uint32_t, 3> (parent, link, offset);
	  નહીંતર
	    assign_offset<uint16_t> (parent, link, offset);
	}
      }
  }

  unsigned પૂર્ણાંક length () અચલ
  {
    જો (unlikely (!current)) પાછા 0;
    પાછા આ->head - current->head;
  }

  શૂન્ય align (unsigned પૂર્ણાંક alignment)
  {
    unsigned પૂર્ણાંક l = length () % alignment;
    જો (l)
      (શૂન્ય) allocate_size<શૂન્ય> (alignment - l);
  }

  template <typename Type = શૂન્ય>
  __attribute__((returns_nonnull))
  Type *start_embed (અચલ Type *obj HB_UNUSED = nullptr) અચલ
  { પાછા reinterpret_cast<Type *> (આ->head); }
  template <typename Type>
  __attribute__((returns_nonnull))
  Type *start_embed (અચલ Type &obj) અચલ
  { પાછા start_embed (std::addressof (obj)); }

  તાર્કિક err (hb_serialize_error_t err_type)
  {
    પાછા !તાર્કિક ((errors = (errors | err_type)));
  }

  તાર્કિક start_zerocopy (size_t size)
  {
    જો (unlikely (in_error ())) પાછા ખોટું;

    જો (unlikely (size > INT_MAX || આ->tail - આ->head < ptrdiff_t (size)))
    {
      err (HB_SERIALIZE_ERROR_OUT_OF_ROOM);
      પાછા ખોટું;
    }

    પ્રતિજ્ઞા (!આ->zerocopy);
    આ->zerocopy = આ->head;

    પ્રતિજ્ઞા (આ->current->head == આ->head);
    આ->current->head = આ->current->tail = આ->head = આ->tail - size;
    પાછા સાચું;
  }

  template <typename Type>
  HB_NODISCARD
  Type *allocate_size (size_t size, તાર્કિક clear = સાચું)
  {
    જો (unlikely (in_error ())) પાછા nullptr;

    જો (unlikely (size > INT_MAX || આ->tail - આ->head < ptrdiff_t (size)))
    {
      err (HB_SERIALIZE_ERROR_OUT_OF_ROOM);
      પાછા nullptr;
    }
    જો (clear)
      hb_memset (આ->head, 0, size);
    અક્ષર *ret = આ->head;
    આ->head += size;
    પાછા reinterpret_cast<Type *> (ret);
  }

  template <typename Type>
  Type *allocate_min ()
  { પાછા આ->allocate_size<Type> (Type::min_size); }

  template <typename Type>
  HB_NODISCARD
  Type *embed (અચલ Type *obj)
  {
    size_t size = obj->get_size ();
    Type *ret = આ->allocate_size<Type> (size, ખોટું);
    જો (unlikely (!ret)) પાછા nullptr;
    hb_memcpy (ret, obj, size);
    પાછા ret;
  }
  template <typename Type>
  HB_NODISCARD
  Type *embed (અચલ Type &obj)
  { પાછા embed (std::addressof (obj)); }
  અક્ષર *embed (અચલ અક્ષર *obj, unsigned size)
  {
    અક્ષર *ret = આ->allocate_size<અક્ષર> (size, ખોટું);
    જો (unlikely (!ret)) પાછા nullptr;
    hb_memcpy (ret, obj, size);
    પાછા ret;
  }

  template <typename Type, typename ...Ts> auto
  _copy (અચલ Type &src, hb_priority<1>, Ts&&... ds) HB_RETURN
  (Type *, src.copy (આ, std::forward<Ts> (ds)...))

  template <typename Type> auto
  _copy (અચલ Type &src, hb_priority<0>) -> decltype (&(hb_declval<Type> () = src))
  {
    Type *ret = આ->allocate_size<Type> (કદ (Type));
    જો (unlikely (!ret)) પાછા nullptr;
    *ret = src;
    પાછા ret;
  }

  /* Like embed, but active: calls obj.operator=() or obj.copy() to transfer data
   * instead of hb_memcpy(). */
  template <typename Type, typename ...Ts>
  Type *copy (અચલ Type &src, Ts&&... ds)
  { પાછા _copy (src, hb_prioritize, std::forward<Ts> (ds)...); }
  template <typename Type, typename ...Ts>
  Type *copy (અચલ Type *src, Ts&&... ds)
  { પાછા copy (*src, std::forward<Ts> (ds)...); }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator)),
	   typename ...Ts>
  શૂન્ય copy_all (Iterator it, Ts&&... ds)
  { માટે (decltype (*it) _ : it) copy (_, ds...); }

  template <typename Type>
  hb_serialize_context_t& operator << (અચલ Type &obj) & { embed (obj); પાછા *આ; }

  template <typename Type>
  Type *extend_size (Type *obj, size_t size, તાર્કિક clear = સાચું)
  {
    જો (unlikely (in_error ())) પાછા nullptr;
    જો (unlikely (size >= INT_MAX)) { err (HB_SERIALIZE_ERROR_OTHER); પાછા nullptr; }

    પ્રતિજ્ઞા (આ->start <= (અક્ષર *) obj);
    પ્રતિજ્ઞા ((અક્ષર *) obj <= આ->head);
    પ્રતિજ્ઞા ((size_t) (આ->head - (અક્ષર *) obj) <= size);
    જો (unlikely (((અક્ષર *) obj + size < (અક્ષર *) obj) ||
		  !આ->allocate_size<Type> (((અક્ષર *) obj) + size - આ->head, clear))) પાછા nullptr;
    પાછા reinterpret_cast<Type *> (obj);
  }
  template <typename Type>
  Type *extend_size (Type &obj, size_t size, તાર્કિક clear = સાચું)
  { પાછા extend_size (std::addressof (obj), size, clear); }

  template <typename Type>
  Type *extend_min (Type *obj) { પાછા extend_size (obj, obj->min_size); }
  template <typename Type>
  Type *extend_min (Type &obj) { પાછા extend_min (std::addressof (obj)); }

  template <typename Type, typename ...Ts>
  Type *extend (Type *obj, Ts&&... ds)
  { પાછા extend_size (obj, obj->get_size (std::forward<Ts> (ds)...)); }
  template <typename Type, typename ...Ts>
  Type *extend (Type &obj, Ts&&... ds)
  { પાછા extend (std::addressof (obj), std::forward<Ts> (ds)...); }

  /* Output routines. */
  hb_bytes_t copy_bytes () અચલ
  {
    પ્રતિજ્ઞા (successful ());
    /* Copy both items from head side and tail side... */
    unsigned પૂર્ણાંક len = (આ->head - આ->start)
		     + (આ->end  - આ->tail);

    // If len is zero don't hb_malloc as the memory won't get properly
    // cleaned up later.
    જો (!len) પાછા hb_bytes_t ();

    અક્ષર *p = (અક્ષર *) hb_malloc (len);
    જો (unlikely (!p)) પાછા hb_bytes_t ();

    hb_memcpy (p, આ->start, આ->head - આ->start);
    hb_memcpy (p + (આ->head - આ->start), આ->tail, આ->end - આ->tail);
    પાછા hb_bytes_t (p, len);
  }
  template <typename Type>
  Type *copy () અચલ
  { પાછા reinterpret_cast<Type *> ((અક્ષર *) copy_bytes ().arrayZ); }
  hb_blob_t *copy_blob () અચલ
  {
    hb_bytes_t b = copy_bytes ();
    પાછા hb_blob_create (b.arrayZ, b.length,
			   HB_MEMORY_MODE_WRITABLE,
			   (અક્ષર *) b.arrayZ, hb_free);
  }

  અચલ hb_vector_t<object_t *>& object_graph() અચલ
  { પાછા packed; }

  ખાનગી:
  template <typename T, unsigned Size = કદ (T)>
  શૂન્ય assign_offset (અચલ object_t* parent, અચલ object_t::link_t &link, unsigned offset)
  {
    // XXX We should stop assuming big-endian!
    auto &off = * ((HBInt<સાચું, T, Size> *) (parent->head + link.position));
    પ્રતિજ્ઞા (0 == off);
    check_assign (off, offset, HB_SERIALIZE_ERROR_OFFSET_OVERFLOW);
  }

  જાહેર:
  અક્ષર *start, *head, *tail, *end, *zerocopy;
  unsigned પૂર્ણાંક debug_depth;
  hb_serialize_error_t errors;

  ખાનગી:

  શૂન્ય merge_virtual_links (અચલ object_t* from, objidx_t to_idx) {
    object_t* to = packed[to_idx];
    માટે (અચલ auto& l : from->virtual_links) {
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
  hb_hashmap_t<અચલ object_t *, objidx_t> packed_map;
};

#endif /* HB_SERIALIZE_HH */
