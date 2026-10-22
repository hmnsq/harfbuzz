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

#അന്തര്ഭാവ "hb.hh"
#അന്തര്ഭാവ "hb-blob.hh"
#അന്തര്ഭാവ "hb-map.hh"
#അന്തര്ഭാവ "hb-free-pool.hh"

#അന്തര്ഭാവ "hb-subset-serialize.h"

/*
 * Serialize
 */

എണ്ണൽ hb_serialize_error_t {
  HB_SERIALIZE_ERROR_NONE =            0x00000000u,
  HB_SERIALIZE_ERROR_OTHER =           0x00000001u,
  HB_SERIALIZE_ERROR_OFFSET_OVERFLOW = 0x00000002u,
  HB_SERIALIZE_ERROR_OUT_OF_ROOM =     0x00000004u,
  HB_SERIALIZE_ERROR_INT_OVERFLOW =    0x00000008u,
  HB_SERIALIZE_ERROR_ARRAY_OVERFLOW =  0x00000010u
};
HB_MARK_AS_FLAG_T (hb_serialize_error_t);

ഘടന hb_serialize_context_t
{
  തരനിർവചനം unsigned objidx_t;

  എണ്ണൽ whence_t {
     Head,	/* Relative to the current object head (default). */
     Tail,	/* Relative to the current object tail after packed. */
     Absolute	/* Absolute: from the start of the serialize buffer. */
   };



  ഘടന object_t
  {
    ശൂന്യം fini () {
      real_links.fini ();
      virtual_links.fini ();
    }

    object_t () = സ്ഥിരസ്ഥിതി;

    object_t (സ്ഥിരം hb_subset_serialize_object_t &o)
    {
      head = o.head;
      tail = o.tail;
      next = nullptr;
      real_links.alloc_exact (o.അക്കം_real_links);
      വേണ്ടി (unsigned i = 0 ; i < o.അക്കം_real_links; i++)
        real_links.push (o.real_links[i]);

      virtual_links.alloc_exact (o.അക്കം_virtual_links);
      വേണ്ടി (unsigned i = 0; i < o.അക്കം_virtual_links; i++)
        virtual_links.push (o.virtual_links[i]);
    }

    തർക്കം in_error () സ്ഥിരം
    {
      തിരികെ real_links.in_error() || virtual_links.in_error();
    }

    തർക്കം add_virtual_link (objidx_t objidx)
    {
      എങ്കിൽ (!objidx)
        തിരികെ അസത്യം;

      auto& link = *virtual_links.push ();
      എങ്കിൽ (virtual_links.in_error ())
        തിരികെ അസത്യം;

      link.objidx = objidx;
      // Remaining fields were previously zero'd by push():
      // link.width = 0;
      // link.is_signed = 0;
      // link.whence = 0;
      // link.position = 0;
      // link.bias = 0;

      തിരികെ സത്യം;
    }

    friend ശൂന്യം swap (object_t& a, object_t& b) noexcept
    {
      hb_swap (a.head, b.head);
      hb_swap (a.tail, b.tail);
      hb_swap (a.next, b.next);
      hb_swap (a.real_links, b.real_links);
      hb_swap (a.virtual_links, b.virtual_links);
    }

    തർക്കം operator == (സ്ഥിരം object_t &o) സ്ഥിരം
    {
      // Virtual links aren't considered for equality since they don't affect the functionality
      // of the object.
      തിരികെ (tail - head == o.tail - o.head)
	  && (real_links.length == o.real_links.length)
	  && 0 == hb_memcmp (head, o.head, tail - head)
	  && real_links.as_bytes () == o.real_links.as_bytes ();
    }
    uint32_t hash () സ്ഥിരം
    {
      // Virtual links aren't considered for equality since they don't affect the functionality
      // of the object.
      തിരികെ hb_bytes_t (head, hb_min (128, tail - head)).hash () ^
          real_links.as_bytes ().hash ();
    }

    ഘടന link_t
    {
      unsigned width: 3;
      unsigned is_signed: 1;
      unsigned whence: 2;
      unsigned bias : 26;
      unsigned position;
      objidx_t objidx;

      link_t () = സ്ഥിരസ്ഥിതി;

      link_t (സ്ഥിരം hb_subset_serialize_link_t &o)
      {
        width = o.width;
        is_signed = 0;
        whence = 0;
        position = o.position;
        bias = 0;
        objidx = o.objidx;
      }

      HB_INTERNAL സ്ഥിതം പൂർണ്ണസംഖ്യ cmp (സ്ഥിരം ശൂന്യം* a, സ്ഥിരം ശൂന്യം* b)
      {
        പൂർണ്ണസംഖ്യ cmp = ((സ്ഥിരം link_t*)a)->position - ((സ്ഥിരം link_t*)b)->position;
        എങ്കിൽ (cmp) തിരികെ cmp;

        തിരികെ ((സ്ഥിരം link_t*)a)->objidx - ((സ്ഥിരം link_t*)b)->objidx;
      }
    };

    അക്ഷരം *head;
    അക്ഷരം *tail;
    hb_vector_t<link_t> real_links;
    hb_vector_t<link_t> virtual_links;
    object_t *next;

    auto all_links () സ്ഥിരം HB_AUTO_RETURN
        (( hb_concat (real_links, virtual_links) ));
    auto all_links_writer () HB_AUTO_RETURN
        (( hb_concat (real_links.writer (), virtual_links.writer ()) ));
  };

  ഘടന snapshot_t
  {
    അക്ഷരം *head;
    അക്ഷരം *tail;
    object_t *current; // Just for sanity check
    unsigned അക്കം_real_links;
    unsigned അക്കം_virtual_links;
    hb_serialize_error_t errors;
  };

  snapshot_t snapshot ()
  {
    തിരികെ snapshot_t {
      head, tail, current,
      current ? current->real_links.length : 0,
      current ? current->virtual_links.length : 0,
      errors
     };
  }

  hb_serialize_context_t (ശൂന്യം *start_, size_t size) :
    start ((അക്ഷരം *) start_),
    end (start + size),
    current (nullptr)
  { reset (); }
  ~hb_serialize_context_t () { fini (); }

  ശൂന്യം fini ()
  {
    വേണ്ടി (object_t *_ : ++hb_iter (packed)) _->fini ();
    packed.fini ();
    ഇത്->packed_map.fini ();

    ആകുമ്പോൾ (current)
    {
      auto *_ = current;
      current = current->next;
      _->fini ();
    }
  }

  തർക്കം in_error () സ്ഥിരം { തിരികെ തർക്കം (errors); }

  തർക്കം successful () സ്ഥിരം { തിരികെ !തർക്കം (errors); }

  HB_NODISCARD തർക്കം ran_out_of_room () സ്ഥിരം { തിരികെ errors & HB_SERIALIZE_ERROR_OUT_OF_ROOM; }
  HB_NODISCARD തർക്കം offset_overflow () സ്ഥിരം { തിരികെ errors & HB_SERIALIZE_ERROR_OFFSET_OVERFLOW; }
  HB_NODISCARD തർക്കം only_offset_overflow () സ്ഥിരം { തിരികെ errors == HB_SERIALIZE_ERROR_OFFSET_OVERFLOW; }
  HB_NODISCARD തർക്കം only_overflow () സ്ഥിരം
  {
    തിരികെ errors == HB_SERIALIZE_ERROR_OFFSET_OVERFLOW
        || errors == HB_SERIALIZE_ERROR_INT_OVERFLOW
        || errors == HB_SERIALIZE_ERROR_ARRAY_OVERFLOW;
  }

  ശൂന്യം reset (ശൂന്യം *start_, size_t size)
  {
    start = (അക്ഷരം*) start_;
    end = start + size;
    reset ();
    current = nullptr;
  }

  ശൂന്യം reset ()
  {
    ഇത്->errors = HB_SERIALIZE_ERROR_NONE;
    ഇത്->head = ഇത്->start;
    ഇത്->tail = ഇത്->end;
    ഇത്->zerocopy = nullptr;
    ഇത്->debug_depth = 0;

    fini ();
    ഇത്->packed.push (nullptr);
    ഇത്->packed_map.init ();
  }

  തർക്കം check_success (തർക്കം success,
                      hb_serialize_error_t err_type = HB_SERIALIZE_ERROR_OTHER)
  {
    തിരികെ successful ()
        && (success || err (err_type));
  }

  template <typename T1, typename T2>
  തർക്കം check_equal (T1 &&v1, T2 &&v2, hb_serialize_error_t err_type)
  {
    എങ്കിൽ ((long long) v1 != (long long) v2)
    {
      തിരികെ err (err_type);
    }
    തിരികെ സത്യം;
  }

  template <typename T1, typename T2>
  തർക്കം check_assign (T1 &v1, T2 &&v2, hb_serialize_error_t err_type)
  { തിരികെ check_equal (v1 = v2, v2, err_type); }

  template <typename T> തർക്കം propagate_error (T &&obj)
  { തിരികെ check_success (!hb_deref (obj).in_error ()); }

  template <typename T1, typename... Ts> തർക്കം propagate_error (T1 &&o1, Ts&&... os)
  { തിരികെ propagate_error (std::forward<T1> (o1)) &&
	   propagate_error (std::forward<Ts> (os)...); }

  /* To be called around main operation. */
  template <typename Type=അക്ഷരം>
  __attribute__((returns_nonnull))
  Type *start_serialize ()
  {
    DEBUG_സന്ദേശം_LEVEL (SERIALIZE, ഇത്->start, 0, +1,
		     "start [%p..%p] (%lu bytes)",
		     ഇത്->start, ഇത്->end,
		     (unsigned long) (ഇത്->end - ഇത്->start));

    പ്രതിജ്ഞാ (!current);
    തിരികെ push<Type> ();
  }
  ശൂന്യം end_serialize ()
  {
    DEBUG_സന്ദേശം_LEVEL (SERIALIZE, ഇത്->start, 0, -1,
		     "end [%p..%p] serialized %u bytes; %s",
		     ഇത്->start, ഇത്->end,
		     (unsigned) (ഇത്->head - ഇത്->start),
		     successful () ? "successful" : "UNSUCCESSFUL");

    propagate_error (packed, packed_map);

    എങ്കിൽ (unlikely (!current)) തിരികെ;
    എങ്കിൽ (unlikely (in_error()))
    {
      // Offset overflows that occur before link resolution cannot be handled
      // by repacking, so set a more general error.
      എങ്കിൽ (offset_overflow ()) err (HB_SERIALIZE_ERROR_OTHER);
      തിരികെ;
    }

    പ്രതിജ്ഞാ (!current->next);

    /* Only "pack" if there exist other objects... Otherwise, don't bother.
     * Saves a move. */
    എങ്കിൽ (packed.length <= 1)
      തിരികെ;

    എടുക്കുക_pack (അസത്യം);

    resolve_links ();
  }

  template <typename Type = ശൂന്യം>
  __attribute__((returns_nonnull))
  Type *push ()
  {
    എങ്കിൽ (unlikely (in_error ())) തിരികെ start_embed<Type> ();

    object_t *obj = object_pool.alloc ();
    എങ്കിൽ (unlikely (!obj))
      check_success (അസത്യം);
    അല്ലെങ്കിൽ
    {
      obj->head = head;
      obj->tail = tail;
      obj->next = current;
      current = obj;
    }
    തിരികെ start_embed<Type> ();
  }
  ശൂന്യം എടുക്കുക_discard ()
  {
    object_t *obj = current;
    എങ്കിൽ (unlikely (!obj)) തിരികെ;
    // Allow cleanup when we've error'd out on int overflows which don't compromise
    // the serializer state.
    എങ്കിൽ (unlikely (in_error() && !only_overflow ())) തിരികെ;

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
  objidx_t എടുക്കുക_pack (തർക്കം share=സത്യം)
  {
    object_t *obj = current;
    എങ്കിൽ (unlikely (!obj)) തിരികെ 0;
    // Allow cleanup when we've error'd out on int overflows which don't compromise
    // the serializer state.
    എങ്കിൽ (unlikely (in_error()  && !only_overflow ())) തിരികെ 0;

    current = current->next;
    obj->tail = head;
    obj->next = nullptr;
    പ്രതിജ്ഞാ (obj->head <= obj->tail);
    unsigned len = obj->tail - obj->head;
    head = zerocopy ? zerocopy : obj->head; /* Rewind head. */
    തർക്കം was_zerocopy = zerocopy;
    zerocopy = nullptr;

    എങ്കിൽ (!len)
    {
      പ്രതിജ്ഞാ (!obj->real_links.length);
      പ്രതിജ്ഞാ (!obj->virtual_links.length);
      തിരികെ 0;
    }

    objidx_t objidx;
    uint32_t hash = 0;
    എങ്കിൽ (share)
    {
      hash = hb_hash (obj);
      objidx = packed_map.get_with_hash (obj, hash);
      എങ്കിൽ (objidx)
      {
        merge_virtual_links (obj, objidx);
	obj->fini ();
        object_pool.release (obj);
	തിരികെ objidx;
      }
    }

    tail -= len;
    എങ്കിൽ (was_zerocopy)
      പ്രതിജ്ഞാ (tail == obj->head);
    അല്ലെങ്കിൽ
      memmove (tail, obj->head, len);

    obj->head = tail;
    obj->tail = tail + len;

    packed.push (obj);

    എങ്കിൽ (unlikely (!propagate_error (packed)))
    {
      /* Obj wasn't successfully added to packed, so clean it up otherwise its
       * links will be leaked. When we use constructor/destructors properly, we
       * can remove these. */
      obj->fini ();
      തിരികെ 0;
    }

    objidx = packed.length - 1;

    എങ്കിൽ (share) packed_map.set_with_hash (obj, hash, objidx);
    propagate_error (packed_map);

    തിരികെ objidx;
  }

  ശൂന്യം revert (snapshot_t snap)
  {
    // Overflows that happened after the snapshot will be erased by the revert.
    എങ്കിൽ (unlikely (in_error () && !only_overflow ())) തിരികെ;
    പ്രതിജ്ഞാ (snap.current == current);
    എങ്കിൽ (current)
    {
      current->real_links.shrink (snap.അക്കം_real_links);
      current->virtual_links.shrink (snap.അക്കം_virtual_links);
    }
    errors = snap.errors;
    revert (snap.head, snap.tail);
  }

  ശൂന്യം revert (അക്ഷരം *snap_head,
	       അക്ഷരം *snap_tail)
  {
    എങ്കിൽ (unlikely (in_error ())) തിരികെ;
    പ്രതിജ്ഞാ (snap_head <= head);
    പ്രതിജ്ഞാ (tail <= snap_tail);
    head = snap_head;
    tail = snap_tail;
    discard_stale_objects ();
  }

  ശൂന്യം discard_stale_objects ()
  {
    എങ്കിൽ (unlikely (in_error ())) തിരികെ;
    ആകുമ്പോൾ (packed.length > 1 &&
	   packed.tail ()->head < tail)
    {
      object_t *obj = packed.tail ();
      packed_map.del (obj);
      പ്രതിജ്ഞാ (!obj->next);
      obj->fini ();
      object_pool.release (obj);
      packed.എടുക്കുക ();
    }
    എങ്കിൽ (packed.length > 1)
      പ്രതിജ്ഞാ (packed.tail ()->head == tail);
  }

  // Adds a virtual link from the current object to objidx. A virtual link is not associated with
  // an actual offset field. They are solely used to enforce ordering constraints between objects.
  // Adding a virtual link from object a to object b will ensure that object b is always packed after
  // object a in the final serialized order.
  //
  // This is useful in certain situations where there needs to be a specific ordering in the
  // final serialization. Such as when platform bugs require certain orderings, or to provide
  //  guidance to the repacker for better offset overflow resolution.
  ശൂന്യം add_virtual_link (objidx_t objidx)
  {
    എങ്കിൽ (unlikely (in_error ())) തിരികെ;

    എങ്കിൽ (!objidx)
      തിരികെ;

    പ്രതിജ്ഞാ (current);

    എങ്കിൽ (!current->add_virtual_link(objidx))
      err (HB_SERIALIZE_ERROR_OTHER);
  }

  objidx_t last_added_child_index() സ്ഥിരം {
    എങ്കിൽ (unlikely (in_error ())) തിരികെ (objidx_t) -1;

    പ്രതിജ്ഞാ (current);
    എങ്കിൽ (!തർക്കം(current->real_links)) {
      തിരികെ (objidx_t) -1;
    }

    തിരികെ current->real_links[current->real_links.length - 1].objidx;
  }

  // For the current object ensure that the sub-table bytes for child objidx are always placed
  // after the subtable bytes for any other existing children. This only ensures that the
  // repacker will not move the target subtable before the other children
  // (by adding virtual links). It is up to the caller to ensure the initial serialization
  // order is correct.
  ശൂന്യം repack_last(objidx_t objidx) {
    എങ്കിൽ (unlikely (in_error ())) തിരികെ;

    എങ്കിൽ (!objidx)
      തിരികെ;

    പ്രതിജ്ഞാ (current);
    വേണ്ടി (auto& l : current->real_links) {
      എങ്കിൽ (l.objidx == objidx) {
        തുടരുക;
      }

      packed[l.objidx]->add_virtual_link(objidx);
    }
  }

  template <typename T>
  ശൂന്യം add_link (T &ofs, objidx_t objidx,
		 whence_t whence = Head,
		 unsigned bias = 0)
  {
    എങ്കിൽ (unlikely (in_error ())) തിരികെ;

    എങ്കിൽ (!objidx)
      തിരികെ;

    പ്രതിജ്ഞാ (current);
    പ്രതിജ്ഞാ (current->head <= (സ്ഥിരം അക്ഷരം *) &ofs);

    auto& link = *current->real_links.push ();
    എങ്കിൽ (current->real_links.in_error ())
      err (HB_SERIALIZE_ERROR_OTHER);

    link.width = വലുപ്പം (T);
    link.objidx = objidx;
    എങ്കിൽ (unlikely (!വലുപ്പം (T)))
    {
      // This link is not associated with an actual offset and exists merely to enforce
      // an ordering constraint.
      link.is_signed = 0;
      link.whence = 0;
      link.position = 0;
      link.bias = 0;
      തിരികെ;
    }

    link.is_signed = std::is_signed<hb_unwrap_type (T)>::value;
    link.whence = (unsigned) whence;
    link.position = (സ്ഥിരം അക്ഷരം *) &ofs - current->head;
    link.bias = bias;
  }

  unsigned to_bias (സ്ഥിരം ശൂന്യം *base) സ്ഥിരം
  {
    എങ്കിൽ (unlikely (in_error ())) തിരികെ 0;
    എങ്കിൽ (!base) തിരികെ 0;
    പ്രതിജ്ഞാ (current);
    പ്രതിജ്ഞാ (current->head <= (സ്ഥിരം അക്ഷരം *) base);
    തിരികെ (സ്ഥിരം അക്ഷരം *) base - current->head;
  }

  ശൂന്യം resolve_links ()
  {
    എങ്കിൽ (unlikely (in_error ())) തിരികെ;

    പ്രതിജ്ഞാ (!current);
    പ്രതിജ്ഞാ (packed.length > 1);

    വേണ്ടി (സ്ഥിരം object_t* parent : ++hb_iter (packed))
      വേണ്ടി (സ്ഥിരം object_t::link_t &link : parent->real_links)
      {
	സ്ഥിരം object_t* child = packed[link.objidx];
	എങ്കിൽ (unlikely (!child)) { err (HB_SERIALIZE_ERROR_OTHER); തിരികെ; }
	unsigned offset = 0;
	തിരഞ്ഞെടുപ്പ് ((whence_t) link.whence) {
	അവസ്ഥ Head:     offset = child->head - parent->head; നിർത്തുക;
	അവസ്ഥ Tail:     offset = child->head - parent->tail; നിർത്തുക;
	അവസ്ഥ Absolute: offset = (head - start) + (child->head - tail); നിർത്തുക;
	}

	പ്രതിജ്ഞാ (offset >= link.bias);
	offset -= link.bias;
	എങ്കിൽ (link.is_signed)
	{
	  പ്രതിജ്ഞാ (link.width == 2 || link.width == 4);
	  എങ്കിൽ (link.width == 4)
	    assign_offset<int32_t> (parent, link, offset);
	  അല്ലെങ്കിൽ
	    assign_offset<int16_t> (parent, link, offset);
	}
	അല്ലെങ്കിൽ
	{
	  പ്രതിജ്ഞാ (link.width == 2 || link.width == 3 || link.width == 4);
	  എങ്കിൽ (link.width == 4)
	    assign_offset<uint32_t> (parent, link, offset);
	  അല്ലെങ്കിൽ എങ്കിൽ (link.width == 3)
	    assign_offset<uint32_t, 3> (parent, link, offset);
	  അല്ലെങ്കിൽ
	    assign_offset<uint16_t> (parent, link, offset);
	}
      }
  }

  unsigned പൂർണ്ണസംഖ്യ length () സ്ഥിരം
  {
    എങ്കിൽ (unlikely (!current)) തിരികെ 0;
    തിരികെ ഇത്->head - current->head;
  }

  ശൂന്യം align (unsigned പൂർണ്ണസംഖ്യ alignment)
  {
    unsigned പൂർണ്ണസംഖ്യ l = length () % alignment;
    എങ്കിൽ (l)
      (ശൂന്യം) allocate_size<ശൂന്യം> (alignment - l);
  }

  template <typename Type = ശൂന്യം>
  __attribute__((returns_nonnull))
  Type *start_embed (സ്ഥിരം Type *obj HB_UNUSED = nullptr) സ്ഥിരം
  { തിരികെ reinterpret_cast<Type *> (ഇത്->head); }
  template <typename Type>
  __attribute__((returns_nonnull))
  Type *start_embed (സ്ഥിരം Type &obj) സ്ഥിരം
  { തിരികെ start_embed (std::addressof (obj)); }

  തർക്കം err (hb_serialize_error_t err_type)
  {
    തിരികെ !തർക്കം ((errors = (errors | err_type)));
  }

  തർക്കം start_zerocopy (size_t size)
  {
    എങ്കിൽ (unlikely (in_error ())) തിരികെ അസത്യം;

    എങ്കിൽ (unlikely (size > INT_MAX || ഇത്->tail - ഇത്->head < ptrdiff_t (size)))
    {
      err (HB_SERIALIZE_ERROR_OUT_OF_ROOM);
      തിരികെ അസത്യം;
    }

    പ്രതിജ്ഞാ (!ഇത്->zerocopy);
    ഇത്->zerocopy = ഇത്->head;

    പ്രതിജ്ഞാ (ഇത്->current->head == ഇത്->head);
    ഇത്->current->head = ഇത്->current->tail = ഇത്->head = ഇത്->tail - size;
    തിരികെ സത്യം;
  }

  template <typename Type>
  HB_NODISCARD
  Type *allocate_size (size_t size, തർക്കം clear = സത്യം)
  {
    എങ്കിൽ (unlikely (in_error ())) തിരികെ nullptr;

    എങ്കിൽ (unlikely (size > INT_MAX || ഇത്->tail - ഇത്->head < ptrdiff_t (size)))
    {
      err (HB_SERIALIZE_ERROR_OUT_OF_ROOM);
      തിരികെ nullptr;
    }
    എങ്കിൽ (clear)
      hb_memset (ഇത്->head, 0, size);
    അക്ഷരം *ret = ഇത്->head;
    ഇത്->head += size;
    തിരികെ reinterpret_cast<Type *> (ret);
  }

  template <typename Type>
  Type *allocate_min ()
  { തിരികെ ഇത്->allocate_size<Type> (Type::min_size); }

  template <typename Type>
  HB_NODISCARD
  Type *embed (സ്ഥിരം Type *obj)
  {
    size_t size = obj->get_size ();
    Type *ret = ഇത്->allocate_size<Type> (size, അസത്യം);
    എങ്കിൽ (unlikely (!ret)) തിരികെ nullptr;
    hb_memcpy (ret, obj, size);
    തിരികെ ret;
  }
  template <typename Type>
  HB_NODISCARD
  Type *embed (സ്ഥിരം Type &obj)
  { തിരികെ embed (std::addressof (obj)); }
  അക്ഷരം *embed (സ്ഥിരം അക്ഷരം *obj, unsigned size)
  {
    അക്ഷരം *ret = ഇത്->allocate_size<അക്ഷരം> (size, അസത്യം);
    എങ്കിൽ (unlikely (!ret)) തിരികെ nullptr;
    hb_memcpy (ret, obj, size);
    തിരികെ ret;
  }

  template <typename Type, typename ...Ts> auto
  _copy (സ്ഥിരം Type &src, hb_priority<1>, Ts&&... ds) HB_RETURN
  (Type *, src.copy (ഇത്, std::forward<Ts> (ds)...))

  template <typename Type> auto
  _copy (സ്ഥിരം Type &src, hb_priority<0>) -> decltype (&(hb_declval<Type> () = src))
  {
    Type *ret = ഇത്->allocate_size<Type> (വലുപ്പം (Type));
    എങ്കിൽ (unlikely (!ret)) തിരികെ nullptr;
    *ret = src;
    തിരികെ ret;
  }

  /* Like embed, but active: calls obj.operator=() or obj.copy() to transfer data
   * instead of hb_memcpy(). */
  template <typename Type, typename ...Ts>
  Type *copy (സ്ഥിരം Type &src, Ts&&... ds)
  { തിരികെ _copy (src, hb_prioritize, std::forward<Ts> (ds)...); }
  template <typename Type, typename ...Ts>
  Type *copy (സ്ഥിരം Type *src, Ts&&... ds)
  { തിരികെ copy (*src, std::forward<Ts> (ds)...); }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator)),
	   typename ...Ts>
  ശൂന്യം copy_all (Iterator it, Ts&&... ds)
  { വേണ്ടി (decltype (*it) _ : it) copy (_, ds...); }

  template <typename Type>
  hb_serialize_context_t& operator << (സ്ഥിരം Type &obj) & { embed (obj); തിരികെ *ഇത്; }

  template <typename Type>
  Type *extend_size (Type *obj, size_t size, തർക്കം clear = സത്യം)
  {
    എങ്കിൽ (unlikely (in_error ())) തിരികെ nullptr;
    എങ്കിൽ (unlikely (size >= INT_MAX)) { err (HB_SERIALIZE_ERROR_OTHER); തിരികെ nullptr; }

    പ്രതിജ്ഞാ (ഇത്->start <= (അക്ഷരം *) obj);
    പ്രതിജ്ഞാ ((അക്ഷരം *) obj <= ഇത്->head);
    പ്രതിജ്ഞാ ((size_t) (ഇത്->head - (അക്ഷരം *) obj) <= size);
    എങ്കിൽ (unlikely (((അക്ഷരം *) obj + size < (അക്ഷരം *) obj) ||
		  !ഇത്->allocate_size<Type> (((അക്ഷരം *) obj) + size - ഇത്->head, clear))) തിരികെ nullptr;
    തിരികെ reinterpret_cast<Type *> (obj);
  }
  template <typename Type>
  Type *extend_size (Type &obj, size_t size, തർക്കം clear = സത്യം)
  { തിരികെ extend_size (std::addressof (obj), size, clear); }

  template <typename Type>
  Type *extend_min (Type *obj) { തിരികെ extend_size (obj, obj->min_size); }
  template <typename Type>
  Type *extend_min (Type &obj) { തിരികെ extend_min (std::addressof (obj)); }

  template <typename Type, typename ...Ts>
  Type *extend (Type *obj, Ts&&... ds)
  { തിരികെ extend_size (obj, obj->get_size (std::forward<Ts> (ds)...)); }
  template <typename Type, typename ...Ts>
  Type *extend (Type &obj, Ts&&... ds)
  { തിരികെ extend (std::addressof (obj), std::forward<Ts> (ds)...); }

  /* Output routines. */
  hb_bytes_t copy_bytes () സ്ഥിരം
  {
    പ്രതിജ്ഞാ (successful ());
    /* Copy both items from head side and tail side... */
    unsigned പൂർണ്ണസംഖ്യ len = (ഇത്->head - ഇത്->start)
		     + (ഇത്->end  - ഇത്->tail);

    // If len is zero don't hb_malloc as the memory won't get properly
    // cleaned up later.
    എങ്കിൽ (!len) തിരികെ hb_bytes_t ();

    അക്ഷരം *p = (അക്ഷരം *) hb_malloc (len);
    എങ്കിൽ (unlikely (!p)) തിരികെ hb_bytes_t ();

    hb_memcpy (p, ഇത്->start, ഇത്->head - ഇത്->start);
    hb_memcpy (p + (ഇത്->head - ഇത്->start), ഇത്->tail, ഇത്->end - ഇത്->tail);
    തിരികെ hb_bytes_t (p, len);
  }
  template <typename Type>
  Type *copy () സ്ഥിരം
  { തിരികെ reinterpret_cast<Type *> ((അക്ഷരം *) copy_bytes ().arrayZ); }
  hb_blob_t *copy_blob () സ്ഥിരം
  {
    hb_bytes_t b = copy_bytes ();
    തിരികെ hb_blob_create (b.arrayZ, b.length,
			   HB_MEMORY_MODE_WRITABLE,
			   (അക്ഷരം *) b.arrayZ, hb_free);
  }

  സ്ഥിരം hb_vector_t<object_t *>& object_graph() സ്ഥിരം
  { തിരികെ packed; }

  സ്വകാര്യ:
  template <typename T, unsigned Size = വലുപ്പം (T)>
  ശൂന്യം assign_offset (സ്ഥിരം object_t* parent, സ്ഥിരം object_t::link_t &link, unsigned offset)
  {
    // XXX We should stop assuming big-endian!
    auto &off = * ((HBInt<സത്യം, T, Size> *) (parent->head + link.position));
    പ്രതിജ്ഞാ (0 == off);
    check_assign (off, offset, HB_SERIALIZE_ERROR_OFFSET_OVERFLOW);
  }

  പൊതു:
  അക്ഷരം *start, *head, *tail, *end, *zerocopy;
  unsigned പൂർണ്ണസംഖ്യ debug_depth;
  hb_serialize_error_t errors;

  സ്വകാര്യ:

  ശൂന്യം merge_virtual_links (സ്ഥിരം object_t* from, objidx_t to_idx) {
    object_t* to = packed[to_idx];
    വേണ്ടി (സ്ഥിരം auto& l : from->virtual_links) {
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
  hb_hashmap_t<സ്ഥിരം object_t *, objidx_t> packed_map;
};

#endif /* HB_SERIALIZE_HH */
