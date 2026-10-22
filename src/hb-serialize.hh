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

#अन्तर्भाव "hb.hh"
#अन्तर्भाव "hb-blob.hh"
#अन्तर्भाव "hb-map.hh"
#अन्तर्भाव "hb-free-pool.hh"

#अन्तर्भाव "hb-subset-serialize.h"

/*
 * Serialize
 */

गणना hb_serialize_error_t {
  HB_SERIALIZE_ERROR_NONE =            0x00000000u,
  HB_SERIALIZE_ERROR_OTHER =           0x00000001u,
  HB_SERIALIZE_ERROR_OFFSET_OVERFLOW = 0x00000002u,
  HB_SERIALIZE_ERROR_OUT_OF_ROOM =     0x00000004u,
  HB_SERIALIZE_ERROR_INT_OVERFLOW =    0x00000008u,
  HB_SERIALIZE_ERROR_ARRAY_OVERFLOW =  0x00000010u
};
HB_MARK_AS_FLAG_T (hb_serialize_error_t);

संरचना hb_serialize_context_t
{
  प्रकारनिर्देश unsigned objidx_t;

  गणना whence_t {
     Head,	/* Relative to the current object head (default). */
     Tail,	/* Relative to the current object tail after packed. */
     Absolute	/* Absolute: from the start of the serialize buffer. */
   };



  संरचना object_t
  {
    शून्य fini () {
      real_links.fini ();
      virtual_links.fini ();
    }

    object_t () = स्वभाव;

    object_t (ध्रुव hb_subset_serialize_object_t &o)
    {
      head = o.head;
      tail = o.tail;
      next = nullptr;
      real_links.alloc_exact (o.अंक_real_links);
      प्रति (unsigned i = 0 ; i < o.अंक_real_links; i++)
        real_links.push (o.real_links[i]);

      virtual_links.alloc_exact (o.अंक_virtual_links);
      प्रति (unsigned i = 0; i < o.अंक_virtual_links; i++)
        virtual_links.push (o.virtual_links[i]);
    }

    तार्किक in_error () ध्रुव
    {
      प्रत्यागम real_links.in_error() || virtual_links.in_error();
    }

    तार्किक add_virtual_link (objidx_t objidx)
    {
      यदि (!objidx)
        प्रत्यागम असत्य;

      auto& link = *virtual_links.push ();
      यदि (virtual_links.in_error ())
        प्रत्यागम असत्य;

      link.objidx = objidx;
      // Remaining fields were previously zero'd by push():
      // link.width = 0;
      // link.is_signed = 0;
      // link.whence = 0;
      // link.position = 0;
      // link.bias = 0;

      प्रत्यागम सत्य;
    }

    friend शून्य swap (object_t& a, object_t& b) noexcept
    {
      hb_swap (a.head, b.head);
      hb_swap (a.tail, b.tail);
      hb_swap (a.next, b.next);
      hb_swap (a.real_links, b.real_links);
      hb_swap (a.virtual_links, b.virtual_links);
    }

    तार्किक operator == (ध्रुव object_t &o) ध्रुव
    {
      // Virtual links aren't considered for equality since they don't affect the functionality
      // of the object.
      प्रत्यागम (tail - head == o.tail - o.head)
	  && (real_links.length == o.real_links.length)
	  && 0 == hb_memcmp (head, o.head, tail - head)
	  && real_links.as_bytes () == o.real_links.as_bytes ();
    }
    uint32_t hash () ध्रुव
    {
      // Virtual links aren't considered for equality since they don't affect the functionality
      // of the object.
      प्रत्यागम hb_bytes_t (head, hb_min (128, tail - head)).hash () ^
          real_links.as_bytes ().hash ();
    }

    संरचना link_t
    {
      unsigned width: 3;
      unsigned is_signed: 1;
      unsigned whence: 2;
      unsigned bias : 26;
      unsigned position;
      objidx_t objidx;

      link_t () = स्वभाव;

      link_t (ध्रुव hb_subset_serialize_link_t &o)
      {
        width = o.width;
        is_signed = 0;
        whence = 0;
        position = o.position;
        bias = 0;
        objidx = o.objidx;
      }

      HB_INTERNAL स्थिर पूर्णाङ्क cmp (ध्रुव शून्य* a, ध्रुव शून्य* b)
      {
        पूर्णाङ्क cmp = ((ध्रुव link_t*)a)->position - ((ध्रुव link_t*)b)->position;
        यदि (cmp) प्रत्यागम cmp;

        प्रत्यागम ((ध्रुव link_t*)a)->objidx - ((ध्रुव link_t*)b)->objidx;
      }
    };

    वर्ण *head;
    वर्ण *tail;
    hb_vector_t<link_t> real_links;
    hb_vector_t<link_t> virtual_links;
    object_t *next;

    auto all_links () ध्रुव HB_AUTO_RETURN
        (( hb_concat (real_links, virtual_links) ));
    auto all_links_writer () HB_AUTO_RETURN
        (( hb_concat (real_links.writer (), virtual_links.writer ()) ));
  };

  संरचना snapshot_t
  {
    वर्ण *head;
    वर्ण *tail;
    object_t *current; // Just for sanity check
    unsigned अंक_real_links;
    unsigned अंक_virtual_links;
    hb_serialize_error_t errors;
  };

  snapshot_t snapshot ()
  {
    प्रत्यागम snapshot_t {
      head, tail, current,
      current ? current->real_links.length : 0,
      current ? current->virtual_links.length : 0,
      errors
     };
  }

  hb_serialize_context_t (शून्य *start_, size_t size) :
    start ((वर्ण *) start_),
    end (start + size),
    current (nullptr)
  { reset (); }
  ~hb_serialize_context_t () { fini (); }

  शून्य fini ()
  {
    प्रति (object_t *_ : ++hb_iter (packed)) _->fini ();
    packed.fini ();
    एतत्->packed_map.fini ();

    यावत् (current)
    {
      auto *_ = current;
      current = current->next;
      _->fini ();
    }
  }

  तार्किक in_error () ध्रुव { प्रत्यागम तार्किक (errors); }

  तार्किक successful () ध्रुव { प्रत्यागम !तार्किक (errors); }

  HB_NODISCARD तार्किक ran_out_of_room () ध्रुव { प्रत्यागम errors & HB_SERIALIZE_ERROR_OUT_OF_ROOM; }
  HB_NODISCARD तार्किक offset_overflow () ध्रुव { प्रत्यागम errors & HB_SERIALIZE_ERROR_OFFSET_OVERFLOW; }
  HB_NODISCARD तार्किक only_offset_overflow () ध्रुव { प्रत्यागम errors == HB_SERIALIZE_ERROR_OFFSET_OVERFLOW; }
  HB_NODISCARD तार्किक only_overflow () ध्रुव
  {
    प्रत्यागम errors == HB_SERIALIZE_ERROR_OFFSET_OVERFLOW
        || errors == HB_SERIALIZE_ERROR_INT_OVERFLOW
        || errors == HB_SERIALIZE_ERROR_ARRAY_OVERFLOW;
  }

  शून्य reset (शून्य *start_, size_t size)
  {
    start = (वर्ण*) start_;
    end = start + size;
    reset ();
    current = nullptr;
  }

  शून्य reset ()
  {
    एतत्->errors = HB_SERIALIZE_ERROR_NONE;
    एतत्->head = एतत्->start;
    एतत्->tail = एतत्->end;
    एतत्->zerocopy = nullptr;
    एतत्->debug_depth = 0;

    fini ();
    एतत्->packed.push (nullptr);
    एतत्->packed_map.init ();
  }

  तार्किक check_success (तार्किक success,
                      hb_serialize_error_t err_type = HB_SERIALIZE_ERROR_OTHER)
  {
    प्रत्यागम successful ()
        && (success || err (err_type));
  }

  template <typename T1, typename T2>
  तार्किक check_equal (T1 &&v1, T2 &&v2, hb_serialize_error_t err_type)
  {
    यदि ((long long) v1 != (long long) v2)
    {
      प्रत्यागम err (err_type);
    }
    प्रत्यागम सत्य;
  }

  template <typename T1, typename T2>
  तार्किक check_assign (T1 &v1, T2 &&v2, hb_serialize_error_t err_type)
  { प्रत्यागम check_equal (v1 = v2, v2, err_type); }

  template <typename T> तार्किक propagate_error (T &&obj)
  { प्रत्यागम check_success (!hb_deref (obj).in_error ()); }

  template <typename T1, typename... Ts> तार्किक propagate_error (T1 &&o1, Ts&&... os)
  { प्रत्यागम propagate_error (std::forward<T1> (o1)) &&
	   propagate_error (std::forward<Ts> (os)...); }

  /* To be called around main operation. */
  template <typename Type=वर्ण>
  __attribute__((returns_nonnull))
  Type *start_serialize ()
  {
    DEBUG_संदेश_LEVEL (SERIALIZE, एतत्->start, 0, +1,
		     "start [%p..%p] (%lu bytes)",
		     एतत्->start, एतत्->end,
		     (unsigned long) (एतत्->end - एतत्->start));

    प्रतिज्ञा (!current);
    प्रत्यागम push<Type> ();
  }
  शून्य end_serialize ()
  {
    DEBUG_संदेश_LEVEL (SERIALIZE, एतत्->start, 0, -1,
		     "end [%p..%p] serialized %u bytes; %s",
		     एतत्->start, एतत्->end,
		     (unsigned) (एतत्->head - एतत्->start),
		     successful () ? "successful" : "UNSUCCESSFUL");

    propagate_error (packed, packed_map);

    यदि (unlikely (!current)) प्रत्यागम;
    यदि (unlikely (in_error()))
    {
      // Offset overflows that occur before link resolution cannot be handled
      // by repacking, so set a more general error.
      यदि (offset_overflow ()) err (HB_SERIALIZE_ERROR_OTHER);
      प्रत्यागम;
    }

    प्रतिज्ञा (!current->next);

    /* Only "pack" if there exist other objects... Otherwise, don't bother.
     * Saves a move. */
    यदि (packed.length <= 1)
      प्रत्यागम;

    काढा_pack (असत्य);

    resolve_links ();
  }

  template <typename Type = शून्य>
  __attribute__((returns_nonnull))
  Type *push ()
  {
    यदि (unlikely (in_error ())) प्रत्यागम start_embed<Type> ();

    object_t *obj = object_pool.alloc ();
    यदि (unlikely (!obj))
      check_success (असत्य);
    अन्यथा
    {
      obj->head = head;
      obj->tail = tail;
      obj->next = current;
      current = obj;
    }
    प्रत्यागम start_embed<Type> ();
  }
  शून्य काढा_discard ()
  {
    object_t *obj = current;
    यदि (unlikely (!obj)) प्रत्यागम;
    // Allow cleanup when we've error'd out on int overflows which don't compromise
    // the serializer state.
    यदि (unlikely (in_error() && !only_overflow ())) प्रत्यागम;

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
  objidx_t काढा_pack (तार्किक share=सत्य)
  {
    object_t *obj = current;
    यदि (unlikely (!obj)) प्रत्यागम 0;
    // Allow cleanup when we've error'd out on int overflows which don't compromise
    // the serializer state.
    यदि (unlikely (in_error()  && !only_overflow ())) प्रत्यागम 0;

    current = current->next;
    obj->tail = head;
    obj->next = nullptr;
    प्रतिज्ञा (obj->head <= obj->tail);
    unsigned len = obj->tail - obj->head;
    head = zerocopy ? zerocopy : obj->head; /* Rewind head. */
    तार्किक was_zerocopy = zerocopy;
    zerocopy = nullptr;

    यदि (!len)
    {
      प्रतिज्ञा (!obj->real_links.length);
      प्रतिज्ञा (!obj->virtual_links.length);
      प्रत्यागम 0;
    }

    objidx_t objidx;
    uint32_t hash = 0;
    यदि (share)
    {
      hash = hb_hash (obj);
      objidx = packed_map.get_with_hash (obj, hash);
      यदि (objidx)
      {
        merge_virtual_links (obj, objidx);
	obj->fini ();
        object_pool.release (obj);
	प्रत्यागम objidx;
      }
    }

    tail -= len;
    यदि (was_zerocopy)
      प्रतिज्ञा (tail == obj->head);
    अन्यथा
      memmove (tail, obj->head, len);

    obj->head = tail;
    obj->tail = tail + len;

    packed.push (obj);

    यदि (unlikely (!propagate_error (packed)))
    {
      /* Obj wasn't successfully added to packed, so clean it up otherwise its
       * links will be leaked. When we use constructor/destructors properly, we
       * can remove these. */
      obj->fini ();
      प्रत्यागम 0;
    }

    objidx = packed.length - 1;

    यदि (share) packed_map.set_with_hash (obj, hash, objidx);
    propagate_error (packed_map);

    प्रत्यागम objidx;
  }

  शून्य revert (snapshot_t snap)
  {
    // Overflows that happened after the snapshot will be erased by the revert.
    यदि (unlikely (in_error () && !only_overflow ())) प्रत्यागम;
    प्रतिज्ञा (snap.current == current);
    यदि (current)
    {
      current->real_links.shrink (snap.अंक_real_links);
      current->virtual_links.shrink (snap.अंक_virtual_links);
    }
    errors = snap.errors;
    revert (snap.head, snap.tail);
  }

  शून्य revert (वर्ण *snap_head,
	       वर्ण *snap_tail)
  {
    यदि (unlikely (in_error ())) प्रत्यागम;
    प्रतिज्ञा (snap_head <= head);
    प्रतिज्ञा (tail <= snap_tail);
    head = snap_head;
    tail = snap_tail;
    discard_stale_objects ();
  }

  शून्य discard_stale_objects ()
  {
    यदि (unlikely (in_error ())) प्रत्यागम;
    यावत् (packed.length > 1 &&
	   packed.tail ()->head < tail)
    {
      object_t *obj = packed.tail ();
      packed_map.del (obj);
      प्रतिज्ञा (!obj->next);
      obj->fini ();
      object_pool.release (obj);
      packed.काढा ();
    }
    यदि (packed.length > 1)
      प्रतिज्ञा (packed.tail ()->head == tail);
  }

  // Adds a virtual link from the current object to objidx. A virtual link is not associated with
  // an actual offset field. They are solely used to enforce ordering constraints between objects.
  // Adding a virtual link from object a to object b will ensure that object b is always packed after
  // object a in the final serialized order.
  //
  // This is useful in certain situations where there needs to be a specific ordering in the
  // final serialization. Such as when platform bugs require certain orderings, or to provide
  //  guidance to the repacker for better offset overflow resolution.
  शून्य add_virtual_link (objidx_t objidx)
  {
    यदि (unlikely (in_error ())) प्रत्यागम;

    यदि (!objidx)
      प्रत्यागम;

    प्रतिज्ञा (current);

    यदि (!current->add_virtual_link(objidx))
      err (HB_SERIALIZE_ERROR_OTHER);
  }

  objidx_t last_added_child_index() ध्रुव {
    यदि (unlikely (in_error ())) प्रत्यागम (objidx_t) -1;

    प्रतिज्ञा (current);
    यदि (!तार्किक(current->real_links)) {
      प्रत्यागम (objidx_t) -1;
    }

    प्रत्यागम current->real_links[current->real_links.length - 1].objidx;
  }

  // For the current object ensure that the sub-table bytes for child objidx are always placed
  // after the subtable bytes for any other existing children. This only ensures that the
  // repacker will not move the target subtable before the other children
  // (by adding virtual links). It is up to the caller to ensure the initial serialization
  // order is correct.
  शून्य repack_last(objidx_t objidx) {
    यदि (unlikely (in_error ())) प्रत्यागम;

    यदि (!objidx)
      प्रत्यागम;

    प्रतिज्ञा (current);
    प्रति (auto& l : current->real_links) {
      यदि (l.objidx == objidx) {
        अनुवर्त;
      }

      packed[l.objidx]->add_virtual_link(objidx);
    }
  }

  template <typename T>
  शून्य add_link (T &ofs, objidx_t objidx,
		 whence_t whence = Head,
		 unsigned bias = 0)
  {
    यदि (unlikely (in_error ())) प्रत्यागम;

    यदि (!objidx)
      प्रत्यागम;

    प्रतिज्ञा (current);
    प्रतिज्ञा (current->head <= (ध्रुव वर्ण *) &ofs);

    auto& link = *current->real_links.push ();
    यदि (current->real_links.in_error ())
      err (HB_SERIALIZE_ERROR_OTHER);

    link.width = परिमाण (T);
    link.objidx = objidx;
    यदि (unlikely (!परिमाण (T)))
    {
      // This link is not associated with an actual offset and exists merely to enforce
      // an ordering constraint.
      link.is_signed = 0;
      link.whence = 0;
      link.position = 0;
      link.bias = 0;
      प्रत्यागम;
    }

    link.is_signed = std::is_signed<hb_unwrap_type (T)>::value;
    link.whence = (unsigned) whence;
    link.position = (ध्रुव वर्ण *) &ofs - current->head;
    link.bias = bias;
  }

  unsigned to_bias (ध्रुव शून्य *base) ध्रुव
  {
    यदि (unlikely (in_error ())) प्रत्यागम 0;
    यदि (!base) प्रत्यागम 0;
    प्रतिज्ञा (current);
    प्रतिज्ञा (current->head <= (ध्रुव वर्ण *) base);
    प्रत्यागम (ध्रुव वर्ण *) base - current->head;
  }

  शून्य resolve_links ()
  {
    यदि (unlikely (in_error ())) प्रत्यागम;

    प्रतिज्ञा (!current);
    प्रतिज्ञा (packed.length > 1);

    प्रति (ध्रुव object_t* parent : ++hb_iter (packed))
      प्रति (ध्रुव object_t::link_t &link : parent->real_links)
      {
	ध्रुव object_t* child = packed[link.objidx];
	यदि (unlikely (!child)) { err (HB_SERIALIZE_ERROR_OTHER); प्रत्यागम; }
	unsigned offset = 0;
	विकल्प ((whence_t) link.whence) {
	पक्ष Head:     offset = child->head - parent->head; भङ्ग;
	पक्ष Tail:     offset = child->head - parent->tail; भङ्ग;
	पक्ष Absolute: offset = (head - start) + (child->head - tail); भङ्ग;
	}

	प्रतिज्ञा (offset >= link.bias);
	offset -= link.bias;
	यदि (link.is_signed)
	{
	  प्रतिज्ञा (link.width == 2 || link.width == 4);
	  यदि (link.width == 4)
	    assign_offset<int32_t> (parent, link, offset);
	  अन्यथा
	    assign_offset<int16_t> (parent, link, offset);
	}
	अन्यथा
	{
	  प्रतिज्ञा (link.width == 2 || link.width == 3 || link.width == 4);
	  यदि (link.width == 4)
	    assign_offset<uint32_t> (parent, link, offset);
	  अन्यथा यदि (link.width == 3)
	    assign_offset<uint32_t, 3> (parent, link, offset);
	  अन्यथा
	    assign_offset<uint16_t> (parent, link, offset);
	}
      }
  }

  unsigned पूर्णाङ्क length () ध्रुव
  {
    यदि (unlikely (!current)) प्रत्यागम 0;
    प्रत्यागम एतत्->head - current->head;
  }

  शून्य align (unsigned पूर्णाङ्क alignment)
  {
    unsigned पूर्णाङ्क l = length () % alignment;
    यदि (l)
      (शून्य) allocate_size<शून्य> (alignment - l);
  }

  template <typename Type = शून्य>
  __attribute__((returns_nonnull))
  Type *start_embed (ध्रुव Type *obj HB_UNUSED = nullptr) ध्रुव
  { प्रत्यागम reinterpret_cast<Type *> (एतत्->head); }
  template <typename Type>
  __attribute__((returns_nonnull))
  Type *start_embed (ध्रुव Type &obj) ध्रुव
  { प्रत्यागम start_embed (std::addressof (obj)); }

  तार्किक err (hb_serialize_error_t err_type)
  {
    प्रत्यागम !तार्किक ((errors = (errors | err_type)));
  }

  तार्किक start_zerocopy (size_t size)
  {
    यदि (unlikely (in_error ())) प्रत्यागम असत्य;

    यदि (unlikely (size > INT_MAX || एतत्->tail - एतत्->head < ptrdiff_t (size)))
    {
      err (HB_SERIALIZE_ERROR_OUT_OF_ROOM);
      प्रत्यागम असत्य;
    }

    प्रतिज्ञा (!एतत्->zerocopy);
    एतत्->zerocopy = एतत्->head;

    प्रतिज्ञा (एतत्->current->head == एतत्->head);
    एतत्->current->head = एतत्->current->tail = एतत्->head = एतत्->tail - size;
    प्रत्यागम सत्य;
  }

  template <typename Type>
  HB_NODISCARD
  Type *allocate_size (size_t size, तार्किक clear = सत्य)
  {
    यदि (unlikely (in_error ())) प्रत्यागम nullptr;

    यदि (unlikely (size > INT_MAX || एतत्->tail - एतत्->head < ptrdiff_t (size)))
    {
      err (HB_SERIALIZE_ERROR_OUT_OF_ROOM);
      प्रत्यागम nullptr;
    }
    यदि (clear)
      hb_memset (एतत्->head, 0, size);
    वर्ण *ret = एतत्->head;
    एतत्->head += size;
    प्रत्यागम reinterpret_cast<Type *> (ret);
  }

  template <typename Type>
  Type *allocate_min ()
  { प्रत्यागम एतत्->allocate_size<Type> (Type::min_size); }

  template <typename Type>
  HB_NODISCARD
  Type *embed (ध्रुव Type *obj)
  {
    size_t size = obj->get_size ();
    Type *ret = एतत्->allocate_size<Type> (size, असत्य);
    यदि (unlikely (!ret)) प्रत्यागम nullptr;
    hb_memcpy (ret, obj, size);
    प्रत्यागम ret;
  }
  template <typename Type>
  HB_NODISCARD
  Type *embed (ध्रुव Type &obj)
  { प्रत्यागम embed (std::addressof (obj)); }
  वर्ण *embed (ध्रुव वर्ण *obj, unsigned size)
  {
    वर्ण *ret = एतत्->allocate_size<वर्ण> (size, असत्य);
    यदि (unlikely (!ret)) प्रत्यागम nullptr;
    hb_memcpy (ret, obj, size);
    प्रत्यागम ret;
  }

  template <typename Type, typename ...Ts> auto
  _copy (ध्रुव Type &src, hb_priority<1>, Ts&&... ds) HB_RETURN
  (Type *, src.copy (एतत्, std::forward<Ts> (ds)...))

  template <typename Type> auto
  _copy (ध्रुव Type &src, hb_priority<0>) -> decltype (&(hb_declval<Type> () = src))
  {
    Type *ret = एतत्->allocate_size<Type> (परिमाण (Type));
    यदि (unlikely (!ret)) प्रत्यागम nullptr;
    *ret = src;
    प्रत्यागम ret;
  }

  /* Like embed, but active: calls obj.operator=() or obj.copy() to transfer data
   * instead of hb_memcpy(). */
  template <typename Type, typename ...Ts>
  Type *copy (ध्रुव Type &src, Ts&&... ds)
  { प्रत्यागम _copy (src, hb_prioritize, std::forward<Ts> (ds)...); }
  template <typename Type, typename ...Ts>
  Type *copy (ध्रुव Type *src, Ts&&... ds)
  { प्रत्यागम copy (*src, std::forward<Ts> (ds)...); }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator)),
	   typename ...Ts>
  शून्य copy_all (Iterator it, Ts&&... ds)
  { प्रति (decltype (*it) _ : it) copy (_, ds...); }

  template <typename Type>
  hb_serialize_context_t& operator << (ध्रुव Type &obj) & { embed (obj); प्रत्यागम *एतत्; }

  template <typename Type>
  Type *extend_size (Type *obj, size_t size, तार्किक clear = सत्य)
  {
    यदि (unlikely (in_error ())) प्रत्यागम nullptr;
    यदि (unlikely (size >= INT_MAX)) { err (HB_SERIALIZE_ERROR_OTHER); प्रत्यागम nullptr; }

    प्रतिज्ञा (एतत्->start <= (वर्ण *) obj);
    प्रतिज्ञा ((वर्ण *) obj <= एतत्->head);
    प्रतिज्ञा ((size_t) (एतत्->head - (वर्ण *) obj) <= size);
    यदि (unlikely (((वर्ण *) obj + size < (वर्ण *) obj) ||
		  !एतत्->allocate_size<Type> (((वर्ण *) obj) + size - एतत्->head, clear))) प्रत्यागम nullptr;
    प्रत्यागम reinterpret_cast<Type *> (obj);
  }
  template <typename Type>
  Type *extend_size (Type &obj, size_t size, तार्किक clear = सत्य)
  { प्रत्यागम extend_size (std::addressof (obj), size, clear); }

  template <typename Type>
  Type *extend_min (Type *obj) { प्रत्यागम extend_size (obj, obj->min_size); }
  template <typename Type>
  Type *extend_min (Type &obj) { प्रत्यागम extend_min (std::addressof (obj)); }

  template <typename Type, typename ...Ts>
  Type *extend (Type *obj, Ts&&... ds)
  { प्रत्यागम extend_size (obj, obj->get_size (std::forward<Ts> (ds)...)); }
  template <typename Type, typename ...Ts>
  Type *extend (Type &obj, Ts&&... ds)
  { प्रत्यागम extend (std::addressof (obj), std::forward<Ts> (ds)...); }

  /* Output routines. */
  hb_bytes_t copy_bytes () ध्रुव
  {
    प्रतिज्ञा (successful ());
    /* Copy both items from head side and tail side... */
    unsigned पूर्णाङ्क len = (एतत्->head - एतत्->start)
		     + (एतत्->end  - एतत्->tail);

    // If len is zero don't hb_malloc as the memory won't get properly
    // cleaned up later.
    यदि (!len) प्रत्यागम hb_bytes_t ();

    वर्ण *p = (वर्ण *) hb_malloc (len);
    यदि (unlikely (!p)) प्रत्यागम hb_bytes_t ();

    hb_memcpy (p, एतत्->start, एतत्->head - एतत्->start);
    hb_memcpy (p + (एतत्->head - एतत्->start), एतत्->tail, एतत्->end - एतत्->tail);
    प्रत्यागम hb_bytes_t (p, len);
  }
  template <typename Type>
  Type *copy () ध्रुव
  { प्रत्यागम reinterpret_cast<Type *> ((वर्ण *) copy_bytes ().arrayZ); }
  hb_blob_t *copy_blob () ध्रुव
  {
    hb_bytes_t b = copy_bytes ();
    प्रत्यागम hb_blob_create (b.arrayZ, b.length,
			   HB_MEMORY_MODE_WRITABLE,
			   (वर्ण *) b.arrayZ, hb_free);
  }

  ध्रुव hb_vector_t<object_t *>& object_graph() ध्रुव
  { प्रत्यागम packed; }

  गुप्त:
  template <typename T, unsigned Size = परिमाण (T)>
  शून्य assign_offset (ध्रुव object_t* parent, ध्रुव object_t::link_t &link, unsigned offset)
  {
    // XXX We should stop assuming big-endian!
    auto &off = * ((HBInt<सत्य, T, Size> *) (parent->head + link.position));
    प्रतिज्ञा (0 == off);
    check_assign (off, offset, HB_SERIALIZE_ERROR_OFFSET_OVERFLOW);
  }

  सार्वजनिक:
  वर्ण *start, *head, *tail, *end, *zerocopy;
  unsigned पूर्णाङ्क debug_depth;
  hb_serialize_error_t errors;

  गुप्त:

  शून्य merge_virtual_links (ध्रुव object_t* from, objidx_t to_idx) {
    object_t* to = packed[to_idx];
    प्रति (ध्रुव auto& l : from->virtual_links) {
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
  hb_hashmap_t<ध्रुव object_t *, objidx_t> packed_map;
};

#endif /* HB_SERIALIZE_HH */
