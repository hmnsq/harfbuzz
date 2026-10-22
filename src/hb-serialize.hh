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

#அந்தர்பாவ "hb.hh"
#அந்தர்பாவ "hb-blob.hh"
#அந்தர்பாவ "hb-map.hh"
#அந்தர்பாவ "hb-free-pool.hh"

#அந்தர்பாவ "hb-subset-serialize.h"

/*
 * Serialize
 */

எண்ணிக்கை hb_serialize_error_t {
  HB_SERIALIZE_ERROR_NONE =            0x00000000u,
  HB_SERIALIZE_ERROR_OTHER =           0x00000001u,
  HB_SERIALIZE_ERROR_OFFSET_OVERFLOW = 0x00000002u,
  HB_SERIALIZE_ERROR_OUT_OF_ROOM =     0x00000004u,
  HB_SERIALIZE_ERROR_INT_OVERFLOW =    0x00000008u,
  HB_SERIALIZE_ERROR_ARRAY_OVERFLOW =  0x00000010u
};
HB_MARK_AS_FLAG_T (hb_serialize_error_t);

கட்டமைப்பு hb_serialize_context_t
{
  வகைவரை unsigned objidx_t;

  எண்ணிக்கை whence_t {
     Head,	/* Relative to the current object head (default). */
     Tail,	/* Relative to the current object tail after packed. */
     Absolute	/* Absolute: from the start of the serialize buffer. */
   };



  கட்டமைப்பு object_t
  {
    வெறுமை fini () {
      real_links.fini ();
      virtual_links.fini ();
    }

    object_t () = இயல்பு;

    object_t (மாறிலி hb_subset_serialize_object_t &o)
    {
      head = o.head;
      tail = o.tail;
      next = nullptr;
      real_links.alloc_exact (o.இலக்கம்_real_links);
      க்கு (unsigned i = 0 ; i < o.இலக்கம்_real_links; i++)
        real_links.push (o.real_links[i]);

      virtual_links.alloc_exact (o.இலக்கம்_virtual_links);
      க்கு (unsigned i = 0; i < o.இலக்கம்_virtual_links; i++)
        virtual_links.push (o.virtual_links[i]);
    }

    தருக்க in_error () மாறிலி
    {
      திரும்பு real_links.in_error() || virtual_links.in_error();
    }

    தருக்க add_virtual_link (objidx_t objidx)
    {
      எனில் (!objidx)
        திரும்பு பொய்;

      auto& link = *virtual_links.push ();
      எனில் (virtual_links.in_error ())
        திரும்பு பொய்;

      link.objidx = objidx;
      // Remaining fields were previously zero'd by push():
      // link.width = 0;
      // link.is_signed = 0;
      // link.whence = 0;
      // link.position = 0;
      // link.bias = 0;

      திரும்பு உண்மை;
    }

    friend வெறுமை swap (object_t& a, object_t& b) noexcept
    {
      hb_swap (a.head, b.head);
      hb_swap (a.tail, b.tail);
      hb_swap (a.next, b.next);
      hb_swap (a.real_links, b.real_links);
      hb_swap (a.virtual_links, b.virtual_links);
    }

    தருக்க operator == (மாறிலி object_t &o) மாறிலி
    {
      // Virtual links aren't considered for equality since they don't affect the functionality
      // of the object.
      திரும்பு (tail - head == o.tail - o.head)
	  && (real_links.length == o.real_links.length)
	  && 0 == hb_memcmp (head, o.head, tail - head)
	  && real_links.as_bytes () == o.real_links.as_bytes ();
    }
    uint32_t hash () மாறிலி
    {
      // Virtual links aren't considered for equality since they don't affect the functionality
      // of the object.
      திரும்பு hb_bytes_t (head, hb_min (128, tail - head)).hash () ^
          real_links.as_bytes ().hash ();
    }

    கட்டமைப்பு link_t
    {
      unsigned width: 3;
      unsigned is_signed: 1;
      unsigned whence: 2;
      unsigned bias : 26;
      unsigned position;
      objidx_t objidx;

      link_t () = இயல்பு;

      link_t (மாறிலி hb_subset_serialize_link_t &o)
      {
        width = o.width;
        is_signed = 0;
        whence = 0;
        position = o.position;
        bias = 0;
        objidx = o.objidx;
      }

      HB_INTERNAL நிலையான முழு cmp (மாறிலி வெறுமை* a, மாறிலி வெறுமை* b)
      {
        முழு cmp = ((மாறிலி link_t*)a)->position - ((மாறிலி link_t*)b)->position;
        எனில் (cmp) திரும்பு cmp;

        திரும்பு ((மாறிலி link_t*)a)->objidx - ((மாறிலி link_t*)b)->objidx;
      }
    };

    எழுத்து *head;
    எழுத்து *tail;
    hb_vector_t<link_t> real_links;
    hb_vector_t<link_t> virtual_links;
    object_t *next;

    auto all_links () மாறிலி HB_AUTO_RETURN
        (( hb_concat (real_links, virtual_links) ));
    auto all_links_writer () HB_AUTO_RETURN
        (( hb_concat (real_links.writer (), virtual_links.writer ()) ));
  };

  கட்டமைப்பு snapshot_t
  {
    எழுத்து *head;
    எழுத்து *tail;
    object_t *current; // Just for sanity check
    unsigned இலக்கம்_real_links;
    unsigned இலக்கம்_virtual_links;
    hb_serialize_error_t errors;
  };

  snapshot_t snapshot ()
  {
    திரும்பு snapshot_t {
      head, tail, current,
      current ? current->real_links.length : 0,
      current ? current->virtual_links.length : 0,
      errors
     };
  }

  hb_serialize_context_t (வெறுமை *start_, size_t size) :
    start ((எழுத்து *) start_),
    end (start + size),
    current (nullptr)
  { reset (); }
  ~hb_serialize_context_t () { fini (); }

  வெறுமை fini ()
  {
    க்கு (object_t *_ : ++hb_iter (packed)) _->fini ();
    packed.fini ();
    இது->packed_map.fini ();

    போது (current)
    {
      auto *_ = current;
      current = current->next;
      _->fini ();
    }
  }

  தருக்க in_error () மாறிலி { திரும்பு தருக்க (errors); }

  தருக்க successful () மாறிலி { திரும்பு !தருக்க (errors); }

  HB_NODISCARD தருக்க ran_out_of_room () மாறிலி { திரும்பு errors & HB_SERIALIZE_ERROR_OUT_OF_ROOM; }
  HB_NODISCARD தருக்க offset_overflow () மாறிலி { திரும்பு errors & HB_SERIALIZE_ERROR_OFFSET_OVERFLOW; }
  HB_NODISCARD தருக்க only_offset_overflow () மாறிலி { திரும்பு errors == HB_SERIALIZE_ERROR_OFFSET_OVERFLOW; }
  HB_NODISCARD தருக்க only_overflow () மாறிலி
  {
    திரும்பு errors == HB_SERIALIZE_ERROR_OFFSET_OVERFLOW
        || errors == HB_SERIALIZE_ERROR_INT_OVERFLOW
        || errors == HB_SERIALIZE_ERROR_ARRAY_OVERFLOW;
  }

  வெறுமை reset (வெறுமை *start_, size_t size)
  {
    start = (எழுத்து*) start_;
    end = start + size;
    reset ();
    current = nullptr;
  }

  வெறுமை reset ()
  {
    இது->errors = HB_SERIALIZE_ERROR_NONE;
    இது->head = இது->start;
    இது->tail = இது->end;
    இது->zerocopy = nullptr;
    இது->debug_depth = 0;

    fini ();
    இது->packed.push (nullptr);
    இது->packed_map.init ();
  }

  தருக்க check_success (தருக்க success,
                      hb_serialize_error_t err_type = HB_SERIALIZE_ERROR_OTHER)
  {
    திரும்பு successful ()
        && (success || err (err_type));
  }

  template <typename T1, typename T2>
  தருக்க check_equal (T1 &&v1, T2 &&v2, hb_serialize_error_t err_type)
  {
    எனில் ((long long) v1 != (long long) v2)
    {
      திரும்பு err (err_type);
    }
    திரும்பு உண்மை;
  }

  template <typename T1, typename T2>
  தருக்க check_assign (T1 &v1, T2 &&v2, hb_serialize_error_t err_type)
  { திரும்பு check_equal (v1 = v2, v2, err_type); }

  template <typename T> தருக்க propagate_error (T &&obj)
  { திரும்பு check_success (!hb_deref (obj).in_error ()); }

  template <typename T1, typename... Ts> தருக்க propagate_error (T1 &&o1, Ts&&... os)
  { திரும்பு propagate_error (std::forward<T1> (o1)) &&
	   propagate_error (std::forward<Ts> (os)...); }

  /* To be called around main operation. */
  template <typename Type=எழுத்து>
  __attribute__((returns_nonnull))
  Type *start_serialize ()
  {
    DEBUG_செய்தி_LEVEL (SERIALIZE, இது->start, 0, +1,
		     "start [%p..%p] (%lu bytes)",
		     இது->start, இது->end,
		     (unsigned long) (இது->end - இது->start));

    ப்ரதிஜ்ஞா (!current);
    திரும்பு push<Type> ();
  }
  வெறுமை end_serialize ()
  {
    DEBUG_செய்தி_LEVEL (SERIALIZE, இது->start, 0, -1,
		     "end [%p..%p] serialized %u bytes; %s",
		     இது->start, இது->end,
		     (unsigned) (இது->head - இது->start),
		     successful () ? "successful" : "UNSUCCESSFUL");

    propagate_error (packed, packed_map);

    எனில் (unlikely (!current)) திரும்பு;
    எனில் (unlikely (in_error()))
    {
      // Offset overflows that occur before link resolution cannot be handled
      // by repacking, so set a more general error.
      எனில் (offset_overflow ()) err (HB_SERIALIZE_ERROR_OTHER);
      திரும்பு;
    }

    ப்ரதிஜ்ஞா (!current->next);

    /* Only "pack" if there exist other objects... Otherwise, don't bother.
     * Saves a move. */
    எனில் (packed.length <= 1)
      திரும்பு;

    எடு_pack (பொய்);

    resolve_links ();
  }

  template <typename Type = வெறுமை>
  __attribute__((returns_nonnull))
  Type *push ()
  {
    எனில் (unlikely (in_error ())) திரும்பு start_embed<Type> ();

    object_t *obj = object_pool.alloc ();
    எனில் (unlikely (!obj))
      check_success (பொய்);
    இல்லையெனில்
    {
      obj->head = head;
      obj->tail = tail;
      obj->next = current;
      current = obj;
    }
    திரும்பு start_embed<Type> ();
  }
  வெறுமை எடு_discard ()
  {
    object_t *obj = current;
    எனில் (unlikely (!obj)) திரும்பு;
    // Allow cleanup when we've error'd out on int overflows which don't compromise
    // the serializer state.
    எனில் (unlikely (in_error() && !only_overflow ())) திரும்பு;

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
  objidx_t எடு_pack (தருக்க share=உண்மை)
  {
    object_t *obj = current;
    எனில் (unlikely (!obj)) திரும்பு 0;
    // Allow cleanup when we've error'd out on int overflows which don't compromise
    // the serializer state.
    எனில் (unlikely (in_error()  && !only_overflow ())) திரும்பு 0;

    current = current->next;
    obj->tail = head;
    obj->next = nullptr;
    ப்ரதிஜ்ஞா (obj->head <= obj->tail);
    unsigned len = obj->tail - obj->head;
    head = zerocopy ? zerocopy : obj->head; /* Rewind head. */
    தருக்க was_zerocopy = zerocopy;
    zerocopy = nullptr;

    எனில் (!len)
    {
      ப்ரதிஜ்ஞா (!obj->real_links.length);
      ப்ரதிஜ்ஞா (!obj->virtual_links.length);
      திரும்பு 0;
    }

    objidx_t objidx;
    uint32_t hash = 0;
    எனில் (share)
    {
      hash = hb_hash (obj);
      objidx = packed_map.get_with_hash (obj, hash);
      எனில் (objidx)
      {
        merge_virtual_links (obj, objidx);
	obj->fini ();
        object_pool.release (obj);
	திரும்பு objidx;
      }
    }

    tail -= len;
    எனில் (was_zerocopy)
      ப்ரதிஜ்ஞா (tail == obj->head);
    இல்லையெனில்
      memmove (tail, obj->head, len);

    obj->head = tail;
    obj->tail = tail + len;

    packed.push (obj);

    எனில் (unlikely (!propagate_error (packed)))
    {
      /* Obj wasn't successfully added to packed, so clean it up otherwise its
       * links will be leaked. When we use constructor/destructors properly, we
       * can remove these. */
      obj->fini ();
      திரும்பு 0;
    }

    objidx = packed.length - 1;

    எனில் (share) packed_map.set_with_hash (obj, hash, objidx);
    propagate_error (packed_map);

    திரும்பு objidx;
  }

  வெறுமை revert (snapshot_t snap)
  {
    // Overflows that happened after the snapshot will be erased by the revert.
    எனில் (unlikely (in_error () && !only_overflow ())) திரும்பு;
    ப்ரதிஜ்ஞா (snap.current == current);
    எனில் (current)
    {
      current->real_links.shrink (snap.இலக்கம்_real_links);
      current->virtual_links.shrink (snap.இலக்கம்_virtual_links);
    }
    errors = snap.errors;
    revert (snap.head, snap.tail);
  }

  வெறுமை revert (எழுத்து *snap_head,
	       எழுத்து *snap_tail)
  {
    எனில் (unlikely (in_error ())) திரும்பு;
    ப்ரதிஜ்ஞா (snap_head <= head);
    ப்ரதிஜ்ஞா (tail <= snap_tail);
    head = snap_head;
    tail = snap_tail;
    discard_stale_objects ();
  }

  வெறுமை discard_stale_objects ()
  {
    எனில் (unlikely (in_error ())) திரும்பு;
    போது (packed.length > 1 &&
	   packed.tail ()->head < tail)
    {
      object_t *obj = packed.tail ();
      packed_map.del (obj);
      ப்ரதிஜ்ஞா (!obj->next);
      obj->fini ();
      object_pool.release (obj);
      packed.எடு ();
    }
    எனில் (packed.length > 1)
      ப்ரதிஜ்ஞா (packed.tail ()->head == tail);
  }

  // Adds a virtual link from the current object to objidx. A virtual link is not associated with
  // an actual offset field. They are solely used to enforce ordering constraints between objects.
  // Adding a virtual link from object a to object b will ensure that object b is always packed after
  // object a in the final serialized order.
  //
  // This is useful in certain situations where there needs to be a specific ordering in the
  // final serialization. Such as when platform bugs require certain orderings, or to provide
  //  guidance to the repacker for better offset overflow resolution.
  வெறுமை add_virtual_link (objidx_t objidx)
  {
    எனில் (unlikely (in_error ())) திரும்பு;

    எனில் (!objidx)
      திரும்பு;

    ப்ரதிஜ்ஞா (current);

    எனில் (!current->add_virtual_link(objidx))
      err (HB_SERIALIZE_ERROR_OTHER);
  }

  objidx_t last_added_child_index() மாறிலி {
    எனில் (unlikely (in_error ())) திரும்பு (objidx_t) -1;

    ப்ரதிஜ்ஞா (current);
    எனில் (!தருக்க(current->real_links)) {
      திரும்பு (objidx_t) -1;
    }

    திரும்பு current->real_links[current->real_links.length - 1].objidx;
  }

  // For the current object ensure that the sub-table bytes for child objidx are always placed
  // after the subtable bytes for any other existing children. This only ensures that the
  // repacker will not move the target subtable before the other children
  // (by adding virtual links). It is up to the caller to ensure the initial serialization
  // order is correct.
  வெறுமை repack_last(objidx_t objidx) {
    எனில் (unlikely (in_error ())) திரும்பு;

    எனில் (!objidx)
      திரும்பு;

    ப்ரதிஜ்ஞா (current);
    க்கு (auto& l : current->real_links) {
      எனில் (l.objidx == objidx) {
        தொடர்;
      }

      packed[l.objidx]->add_virtual_link(objidx);
    }
  }

  template <typename T>
  வெறுமை add_link (T &ofs, objidx_t objidx,
		 whence_t whence = Head,
		 unsigned bias = 0)
  {
    எனில் (unlikely (in_error ())) திரும்பு;

    எனில் (!objidx)
      திரும்பு;

    ப்ரதிஜ்ஞா (current);
    ப்ரதிஜ்ஞா (current->head <= (மாறிலி எழுத்து *) &ofs);

    auto& link = *current->real_links.push ();
    எனில் (current->real_links.in_error ())
      err (HB_SERIALIZE_ERROR_OTHER);

    link.width = அளவு (T);
    link.objidx = objidx;
    எனில் (unlikely (!அளவு (T)))
    {
      // This link is not associated with an actual offset and exists merely to enforce
      // an ordering constraint.
      link.is_signed = 0;
      link.whence = 0;
      link.position = 0;
      link.bias = 0;
      திரும்பு;
    }

    link.is_signed = std::is_signed<hb_unwrap_type (T)>::value;
    link.whence = (unsigned) whence;
    link.position = (மாறிலி எழுத்து *) &ofs - current->head;
    link.bias = bias;
  }

  unsigned to_bias (மாறிலி வெறுமை *base) மாறிலி
  {
    எனில் (unlikely (in_error ())) திரும்பு 0;
    எனில் (!base) திரும்பு 0;
    ப்ரதிஜ்ஞா (current);
    ப்ரதிஜ்ஞா (current->head <= (மாறிலி எழுத்து *) base);
    திரும்பு (மாறிலி எழுத்து *) base - current->head;
  }

  வெறுமை resolve_links ()
  {
    எனில் (unlikely (in_error ())) திரும்பு;

    ப்ரதிஜ்ஞா (!current);
    ப்ரதிஜ்ஞா (packed.length > 1);

    க்கு (மாறிலி object_t* parent : ++hb_iter (packed))
      க்கு (மாறிலி object_t::link_t &link : parent->real_links)
      {
	மாறிலி object_t* child = packed[link.objidx];
	எனில் (unlikely (!child)) { err (HB_SERIALIZE_ERROR_OTHER); திரும்பு; }
	unsigned offset = 0;
	தேர்வு ((whence_t) link.whence) {
	நிலை Head:     offset = child->head - parent->head; நிறுத்து;
	நிலை Tail:     offset = child->head - parent->tail; நிறுத்து;
	நிலை Absolute: offset = (head - start) + (child->head - tail); நிறுத்து;
	}

	ப்ரதிஜ்ஞா (offset >= link.bias);
	offset -= link.bias;
	எனில் (link.is_signed)
	{
	  ப்ரதிஜ்ஞா (link.width == 2 || link.width == 4);
	  எனில் (link.width == 4)
	    assign_offset<int32_t> (parent, link, offset);
	  இல்லையெனில்
	    assign_offset<int16_t> (parent, link, offset);
	}
	இல்லையெனில்
	{
	  ப்ரதிஜ்ஞா (link.width == 2 || link.width == 3 || link.width == 4);
	  எனில் (link.width == 4)
	    assign_offset<uint32_t> (parent, link, offset);
	  இல்லையெனில் எனில் (link.width == 3)
	    assign_offset<uint32_t, 3> (parent, link, offset);
	  இல்லையெனில்
	    assign_offset<uint16_t> (parent, link, offset);
	}
      }
  }

  unsigned முழு length () மாறிலி
  {
    எனில் (unlikely (!current)) திரும்பு 0;
    திரும்பு இது->head - current->head;
  }

  வெறுமை align (unsigned முழு alignment)
  {
    unsigned முழு l = length () % alignment;
    எனில் (l)
      (வெறுமை) allocate_size<வெறுமை> (alignment - l);
  }

  template <typename Type = வெறுமை>
  __attribute__((returns_nonnull))
  Type *start_embed (மாறிலி Type *obj HB_UNUSED = nullptr) மாறிலி
  { திரும்பு reinterpret_cast<Type *> (இது->head); }
  template <typename Type>
  __attribute__((returns_nonnull))
  Type *start_embed (மாறிலி Type &obj) மாறிலி
  { திரும்பு start_embed (std::addressof (obj)); }

  தருக்க err (hb_serialize_error_t err_type)
  {
    திரும்பு !தருக்க ((errors = (errors | err_type)));
  }

  தருக்க start_zerocopy (size_t size)
  {
    எனில் (unlikely (in_error ())) திரும்பு பொய்;

    எனில் (unlikely (size > INT_MAX || இது->tail - இது->head < ptrdiff_t (size)))
    {
      err (HB_SERIALIZE_ERROR_OUT_OF_ROOM);
      திரும்பு பொய்;
    }

    ப்ரதிஜ்ஞா (!இது->zerocopy);
    இது->zerocopy = இது->head;

    ப்ரதிஜ்ஞா (இது->current->head == இது->head);
    இது->current->head = இது->current->tail = இது->head = இது->tail - size;
    திரும்பு உண்மை;
  }

  template <typename Type>
  HB_NODISCARD
  Type *allocate_size (size_t size, தருக்க clear = உண்மை)
  {
    எனில் (unlikely (in_error ())) திரும்பு nullptr;

    எனில் (unlikely (size > INT_MAX || இது->tail - இது->head < ptrdiff_t (size)))
    {
      err (HB_SERIALIZE_ERROR_OUT_OF_ROOM);
      திரும்பு nullptr;
    }
    எனில் (clear)
      hb_memset (இது->head, 0, size);
    எழுத்து *ret = இது->head;
    இது->head += size;
    திரும்பு reinterpret_cast<Type *> (ret);
  }

  template <typename Type>
  Type *allocate_min ()
  { திரும்பு இது->allocate_size<Type> (Type::min_size); }

  template <typename Type>
  HB_NODISCARD
  Type *embed (மாறிலி Type *obj)
  {
    size_t size = obj->get_size ();
    Type *ret = இது->allocate_size<Type> (size, பொய்);
    எனில் (unlikely (!ret)) திரும்பு nullptr;
    hb_memcpy (ret, obj, size);
    திரும்பு ret;
  }
  template <typename Type>
  HB_NODISCARD
  Type *embed (மாறிலி Type &obj)
  { திரும்பு embed (std::addressof (obj)); }
  எழுத்து *embed (மாறிலி எழுத்து *obj, unsigned size)
  {
    எழுத்து *ret = இது->allocate_size<எழுத்து> (size, பொய்);
    எனில் (unlikely (!ret)) திரும்பு nullptr;
    hb_memcpy (ret, obj, size);
    திரும்பு ret;
  }

  template <typename Type, typename ...Ts> auto
  _copy (மாறிலி Type &src, hb_priority<1>, Ts&&... ds) HB_RETURN
  (Type *, src.copy (இது, std::forward<Ts> (ds)...))

  template <typename Type> auto
  _copy (மாறிலி Type &src, hb_priority<0>) -> decltype (&(hb_declval<Type> () = src))
  {
    Type *ret = இது->allocate_size<Type> (அளவு (Type));
    எனில் (unlikely (!ret)) திரும்பு nullptr;
    *ret = src;
    திரும்பு ret;
  }

  /* Like embed, but active: calls obj.operator=() or obj.copy() to transfer data
   * instead of hb_memcpy(). */
  template <typename Type, typename ...Ts>
  Type *copy (மாறிலி Type &src, Ts&&... ds)
  { திரும்பு _copy (src, hb_prioritize, std::forward<Ts> (ds)...); }
  template <typename Type, typename ...Ts>
  Type *copy (மாறிலி Type *src, Ts&&... ds)
  { திரும்பு copy (*src, std::forward<Ts> (ds)...); }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator)),
	   typename ...Ts>
  வெறுமை copy_all (Iterator it, Ts&&... ds)
  { க்கு (decltype (*it) _ : it) copy (_, ds...); }

  template <typename Type>
  hb_serialize_context_t& operator << (மாறிலி Type &obj) & { embed (obj); திரும்பு *இது; }

  template <typename Type>
  Type *extend_size (Type *obj, size_t size, தருக்க clear = உண்மை)
  {
    எனில் (unlikely (in_error ())) திரும்பு nullptr;
    எனில் (unlikely (size >= INT_MAX)) { err (HB_SERIALIZE_ERROR_OTHER); திரும்பு nullptr; }

    ப்ரதிஜ்ஞா (இது->start <= (எழுத்து *) obj);
    ப்ரதிஜ்ஞா ((எழுத்து *) obj <= இது->head);
    ப்ரதிஜ்ஞா ((size_t) (இது->head - (எழுத்து *) obj) <= size);
    எனில் (unlikely (((எழுத்து *) obj + size < (எழுத்து *) obj) ||
		  !இது->allocate_size<Type> (((எழுத்து *) obj) + size - இது->head, clear))) திரும்பு nullptr;
    திரும்பு reinterpret_cast<Type *> (obj);
  }
  template <typename Type>
  Type *extend_size (Type &obj, size_t size, தருக்க clear = உண்மை)
  { திரும்பு extend_size (std::addressof (obj), size, clear); }

  template <typename Type>
  Type *extend_min (Type *obj) { திரும்பு extend_size (obj, obj->min_size); }
  template <typename Type>
  Type *extend_min (Type &obj) { திரும்பு extend_min (std::addressof (obj)); }

  template <typename Type, typename ...Ts>
  Type *extend (Type *obj, Ts&&... ds)
  { திரும்பு extend_size (obj, obj->get_size (std::forward<Ts> (ds)...)); }
  template <typename Type, typename ...Ts>
  Type *extend (Type &obj, Ts&&... ds)
  { திரும்பு extend (std::addressof (obj), std::forward<Ts> (ds)...); }

  /* Output routines. */
  hb_bytes_t copy_bytes () மாறிலி
  {
    ப்ரதிஜ்ஞா (successful ());
    /* Copy both items from head side and tail side... */
    unsigned முழு len = (இது->head - இது->start)
		     + (இது->end  - இது->tail);

    // If len is zero don't hb_malloc as the memory won't get properly
    // cleaned up later.
    எனில் (!len) திரும்பு hb_bytes_t ();

    எழுத்து *p = (எழுத்து *) hb_malloc (len);
    எனில் (unlikely (!p)) திரும்பு hb_bytes_t ();

    hb_memcpy (p, இது->start, இது->head - இது->start);
    hb_memcpy (p + (இது->head - இது->start), இது->tail, இது->end - இது->tail);
    திரும்பு hb_bytes_t (p, len);
  }
  template <typename Type>
  Type *copy () மாறிலி
  { திரும்பு reinterpret_cast<Type *> ((எழுத்து *) copy_bytes ().arrayZ); }
  hb_blob_t *copy_blob () மாறிலி
  {
    hb_bytes_t b = copy_bytes ();
    திரும்பு hb_blob_create (b.arrayZ, b.length,
			   HB_MEMORY_MODE_WRITABLE,
			   (எழுத்து *) b.arrayZ, hb_free);
  }

  மாறிலி hb_vector_t<object_t *>& object_graph() மாறிலி
  { திரும்பு packed; }

  தனிப்பட்ட:
  template <typename T, unsigned Size = அளவு (T)>
  வெறுமை assign_offset (மாறிலி object_t* parent, மாறிலி object_t::link_t &link, unsigned offset)
  {
    // XXX We should stop assuming big-endian!
    auto &off = * ((HBInt<உண்மை, T, Size> *) (parent->head + link.position));
    ப்ரதிஜ்ஞா (0 == off);
    check_assign (off, offset, HB_SERIALIZE_ERROR_OFFSET_OVERFLOW);
  }

  பொது:
  எழுத்து *start, *head, *tail, *end, *zerocopy;
  unsigned முழு debug_depth;
  hb_serialize_error_t errors;

  தனிப்பட்ட:

  வெறுமை merge_virtual_links (மாறிலி object_t* from, objidx_t to_idx) {
    object_t* to = packed[to_idx];
    க்கு (மாறிலி auto& l : from->virtual_links) {
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
  hb_hashmap_t<மாறிலி object_t *, objidx_t> packed_map;
};

#endif /* HB_SERIALIZE_HH */
