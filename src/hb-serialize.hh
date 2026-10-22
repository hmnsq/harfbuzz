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

#include "hb.hh"
#include "hb-blob.hh"
#include "hb-map.hh"
#include "hb-free-pool.hh"

#include "hb-subset-serialize.h"

/*
 * Serialize
 */

ga_nanaa hb_serialize_error_t {
  HB_SERIALIZE_ERROR_NONE =            0x00000000u,
  HB_SERIALIZE_ERROR_OTHER =           0x00000001u,
  HB_SERIALIZE_ERROR_OFFSET_OVERFLOW = 0x00000002u,
  HB_SERIALIZE_ERROR_OUT_OF_ROOM =     0x00000004u,
  HB_SERIALIZE_ERROR_INT_OVERFLOW =    0x00000008u,
  HB_SERIALIZE_ERROR_ARRAY_OVERFLOW =  0x00000010u
};
HB_MARK_AS_FLAG_T (hb_serialize_error_t);

sa_nracanaa hb_serialize_context_t
{
  typedef unsigned objidx_t;

  ga_nanaa whence_t {
     Head,	/* Relative to the current object head (default). */
     Tail,	/* Relative to the current object tail after packed. */
     Absolute	/* Absolute: from the start of the serialize buffer. */
   };



  sa_nracanaa object_t
  {
    void fini () {
      real_links.fini ();
      virtual_links.fini ();
    }

    object_t () = default;

    object_t (const hb_subset_serialize_object_t &o)
    {
      head = o.head;
      tail = o.tail;
      next = nullptr;
      real_links.alloc_exact (o.num_real_links);
      အတွက် (unsigned i = 0 ; i < o.num_real_links; i++)
        real_links.push (o.real_links[i]);

      virtual_links.alloc_exact (o.num_virtual_links);
      အတွက် (unsigned i = 0; i < o.num_virtual_links; i++)
        virtual_links.push (o.virtual_links[i]);
    }

    bool in_error () const
    {
      ပြန်ပေး real_links.in_error() || virtual_links.in_error();
    }

    bool add_virtual_link (objidx_t objidx)
    {
      အကယ်၍ (!objidx)
        ပြန်ပေး false;

      auto& link = *virtual_links.push ();
      အကယ်၍ (virtual_links.in_error ())
        ပြန်ပေး false;

      link.objidx = objidx;
      // Remaining fields were previously zero'd by push():
      // link.width = 0;
      // link.is_signed = 0;
      // link.whence = 0;
      // link.position = 0;
      // link.bias = 0;

      ပြန်ပေး true;
    }

    friend void swap (object_t& a, object_t& b) noexcept
    {
      hb_swap (a.head, b.head);
      hb_swap (a.tail, b.tail);
      hb_swap (a.next, b.next);
      hb_swap (a.real_links, b.real_links);
      hb_swap (a.virtual_links, b.virtual_links);
    }

    bool operator == (const object_t &o) const
    {
      // Virtual links aren't considered for equality since they don't affect the functionality
      // of the object.
      ပြန်ပေး (tail - head == o.tail - o.head)
	  && (real_links.length == o.real_links.length)
	  && 0 == hb_memcmp (head, o.head, tail - head)
	  && real_links.as_bytes () == o.real_links.as_bytes ();
    }
    uint32_t hash () const
    {
      // Virtual links aren't considered for equality since they don't affect the functionality
      // of the object.
      ပြန်ပေး hb_bytes_t (head, hb_min (128, tail - head)).hash () ^
          real_links.as_bytes ().hash ();
    }

    sa_nracanaa link_t
    {
      unsigned width: 3;
      unsigned is_signed: 1;
      unsigned whence: 2;
      unsigned bias : 26;
      unsigned position;
      objidx_t objidx;

      link_t () = default;

      link_t (const hb_subset_serialize_link_t &o)
      {
        width = o.width;
        is_signed = 0;
        whence = 0;
        position = o.position;
        bias = 0;
        objidx = o.objidx;
      }

      HB_INTERNAL static ကိန်းပြည့် cmp (const void* a, const void* b)
      {
        ကိန်းပြည့် cmp = ((const link_t*)a)->position - ((const link_t*)b)->position;
        အကယ်၍ (cmp) ပြန်ပေး cmp;

        ပြန်ပေး ((const link_t*)a)->objidx - ((const link_t*)b)->objidx;
      }
    };

    var_na *head;
    var_na *tail;
    hb_vector_t<link_t> real_links;
    hb_vector_t<link_t> virtual_links;
    object_t *next;

    auto all_links () const HB_AUTO_RETURN
        (( hb_concat (real_links, virtual_links) ));
    auto all_links_writer () HB_AUTO_RETURN
        (( hb_concat (real_links.writer (), virtual_links.writer ()) ));
  };

  sa_nracanaa snapshot_t
  {
    var_na *head;
    var_na *tail;
    object_t *current; // Just for sanity check
    unsigned num_real_links;
    unsigned num_virtual_links;
    hb_serialize_error_t errors;
  };

  snapshot_t snapshot ()
  {
    ပြန်ပေး snapshot_t {
      head, tail, current,
      current ? current->real_links.length : 0,
      current ? current->virtual_links.length : 0,
      errors
     };
  }

  hb_serialize_context_t (void *start_, size_t size) :
    start ((var_na *) start_),
    end (start + size),
    current (nullptr)
  { reset (); }
  ~hb_serialize_context_t () { fini (); }

  void fini ()
  {
    အတွက် (object_t *_ : ++hb_iter (packed)) _->fini ();
    packed.fini ();
    this->packed_map.fini ();

    နေစဉ် (current)
    {
      auto *_ = current;
      current = current->next;
      _->fini ();
    }
  }

  bool in_error () const { ပြန်ပေး bool (errors); }

  bool successful () const { ပြန်ပေး !bool (errors); }

  HB_NODISCARD bool ran_out_of_room () const { ပြန်ပေး errors & HB_SERIALIZE_ERROR_OUT_OF_ROOM; }
  HB_NODISCARD bool offset_overflow () const { ပြန်ပေး errors & HB_SERIALIZE_ERROR_OFFSET_OVERFLOW; }
  HB_NODISCARD bool only_offset_overflow () const { ပြန်ပေး errors == HB_SERIALIZE_ERROR_OFFSET_OVERFLOW; }
  HB_NODISCARD bool only_overflow () const
  {
    ပြန်ပေး errors == HB_SERIALIZE_ERROR_OFFSET_OVERFLOW
        || errors == HB_SERIALIZE_ERROR_INT_OVERFLOW
        || errors == HB_SERIALIZE_ERROR_ARRAY_OVERFLOW;
  }

  void reset (void *start_, size_t size)
  {
    start = (var_na*) start_;
    end = start + size;
    reset ();
    current = nullptr;
  }

  void reset ()
  {
    this->errors = HB_SERIALIZE_ERROR_NONE;
    this->head = this->start;
    this->tail = this->end;
    this->zerocopy = nullptr;
    this->debug_depth = 0;

    fini ();
    this->packed.push (nullptr);
    this->packed_map.init ();
  }

  bool check_success (bool success,
                      hb_serialize_error_t err_type = HB_SERIALIZE_ERROR_OTHER)
  {
    ပြန်ပေး successful ()
        && (success || err (err_type));
  }

  template <typename T1, typename T2>
  bool check_equal (T1 &&v1, T2 &&v2, hb_serialize_error_t err_type)
  {
    အကယ်၍ ((long long) v1 != (long long) v2)
    {
      ပြန်ပေး err (err_type);
    }
    ပြန်ပေး true;
  }

  template <typename T1, typename T2>
  bool check_assign (T1 &v1, T2 &&v2, hb_serialize_error_t err_type)
  { ပြန်ပေး check_equal (v1 = v2, v2, err_type); }

  template <typename T> bool propagate_error (T &&obj)
  { ပြန်ပေး check_success (!hb_deref (obj).in_error ()); }

  template <typename T1, typename... Ts> bool propagate_error (T1 &&o1, Ts&&... os)
  { ပြန်ပေး propagate_error (std::forward<T1> (o1)) &&
	   propagate_error (std::forward<Ts> (os)...); }

  /* To be called around main operation. */
  template <typename Type=var_na>
  __attribute__((returns_nonnull))
  Type *start_serialize ()
  {
    DEBUG_MSG_LEVEL (SERIALIZE, this->start, 0, +1,
		     "start [%p..%p] (%lu bytes)",
		     this->start, this->end,
		     (unsigned long) (this->end - this->start));

    assert (!current);
    ပြန်ပေး push<Type> ();
  }
  void end_serialize ()
  {
    DEBUG_MSG_LEVEL (SERIALIZE, this->start, 0, -1,
		     "end [%p..%p] serialized %u bytes; %s",
		     this->start, this->end,
		     (unsigned) (this->head - this->start),
		     successful () ? "successful" : "UNSUCCESSFUL");

    propagate_error (packed, packed_map);

    အကယ်၍ (unlikely (!current)) ပြန်ပေး;
    အကယ်၍ (unlikely (in_error()))
    {
      // Offset overflows that occur before link resolution cannot be handled
      // by repacking, so set a more general error.
      အကယ်၍ (offset_overflow ()) err (HB_SERIALIZE_ERROR_OTHER);
      ပြန်ပေး;
    }

    assert (!current->next);

    /* Only "pack" if there exist other objects... Otherwise, don't bother.
     * Saves a move. */
    အကယ်၍ (packed.length <= 1)
      ပြန်ပေး;

    pop_pack (false);

    resolve_links ();
  }

  template <typename Type = void>
  __attribute__((returns_nonnull))
  Type *push ()
  {
    အကယ်၍ (unlikely (in_error ())) ပြန်ပေး start_embed<Type> ();

    object_t *obj = object_pool.alloc ();
    အကယ်၍ (unlikely (!obj))
      check_success (false);
    မဟုတ်ပါက
    {
      obj->head = head;
      obj->tail = tail;
      obj->next = current;
      current = obj;
    }
    ပြန်ပေး start_embed<Type> ();
  }
  void pop_discard ()
  {
    object_t *obj = current;
    အကယ်၍ (unlikely (!obj)) ပြန်ပေး;
    // Allow cleanup when we've error'd out on int overflows which don't compromise
    // the serializer state.
    အကယ်၍ (unlikely (in_error() && !only_overflow ())) ပြန်ပေး;

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
  objidx_t pop_pack (bool share=true)
  {
    object_t *obj = current;
    အကယ်၍ (unlikely (!obj)) ပြန်ပေး 0;
    // Allow cleanup when we've error'd out on int overflows which don't compromise
    // the serializer state.
    အကယ်၍ (unlikely (in_error()  && !only_overflow ())) ပြန်ပေး 0;

    current = current->next;
    obj->tail = head;
    obj->next = nullptr;
    assert (obj->head <= obj->tail);
    unsigned len = obj->tail - obj->head;
    head = zerocopy ? zerocopy : obj->head; /* Rewind head. */
    bool was_zerocopy = zerocopy;
    zerocopy = nullptr;

    အကယ်၍ (!len)
    {
      assert (!obj->real_links.length);
      assert (!obj->virtual_links.length);
      ပြန်ပေး 0;
    }

    objidx_t objidx;
    uint32_t hash = 0;
    အကယ်၍ (share)
    {
      hash = hb_hash (obj);
      objidx = packed_map.get_with_hash (obj, hash);
      အကယ်၍ (objidx)
      {
        merge_virtual_links (obj, objidx);
	obj->fini ();
        object_pool.release (obj);
	ပြန်ပေး objidx;
      }
    }

    tail -= len;
    အကယ်၍ (was_zerocopy)
      assert (tail == obj->head);
    မဟုတ်ပါက
      memmove (tail, obj->head, len);

    obj->head = tail;
    obj->tail = tail + len;

    packed.push (obj);

    အကယ်၍ (unlikely (!propagate_error (packed)))
    {
      /* Obj wasn't successfully added to packed, so clean it up otherwise its
       * links will be leaked. When we use constructor/destructors properly, we
       * can remove these. */
      obj->fini ();
      ပြန်ပေး 0;
    }

    objidx = packed.length - 1;

    အကယ်၍ (share) packed_map.set_with_hash (obj, hash, objidx);
    propagate_error (packed_map);

    ပြန်ပေး objidx;
  }

  void revert (snapshot_t snap)
  {
    // Overflows that happened after the snapshot will be erased by the revert.
    အကယ်၍ (unlikely (in_error () && !only_overflow ())) ပြန်ပေး;
    assert (snap.current == current);
    အကယ်၍ (current)
    {
      current->real_links.shrink (snap.num_real_links);
      current->virtual_links.shrink (snap.num_virtual_links);
    }
    errors = snap.errors;
    revert (snap.head, snap.tail);
  }

  void revert (var_na *snap_head,
	       var_na *snap_tail)
  {
    အကယ်၍ (unlikely (in_error ())) ပြန်ပေး;
    assert (snap_head <= head);
    assert (tail <= snap_tail);
    head = snap_head;
    tail = snap_tail;
    discard_stale_objects ();
  }

  void discard_stale_objects ()
  {
    အကယ်၍ (unlikely (in_error ())) ပြန်ပေး;
    နေစဉ် (packed.length > 1 &&
	   packed.tail ()->head < tail)
    {
      object_t *obj = packed.tail ();
      packed_map.del (obj);
      assert (!obj->next);
      obj->fini ();
      object_pool.release (obj);
      packed.pop ();
    }
    အကယ်၍ (packed.length > 1)
      assert (packed.tail ()->head == tail);
  }

  // Adds a virtual link from the current object to objidx. A virtual link is not associated with
  // an actual offset field. They are solely used to enforce ordering constraints between objects.
  // Adding a virtual link from object a to object b will ensure that object b is always packed after
  // object a in the final serialized order.
  //
  // This is useful in certain situations where there needs to be a specific ordering in the
  // final serialization. Such as when platform bugs require certain orderings, or to provide
  //  guidance to the repacker for better offset overflow resolution.
  void add_virtual_link (objidx_t objidx)
  {
    အကယ်၍ (unlikely (in_error ())) ပြန်ပေး;

    အကယ်၍ (!objidx)
      ပြန်ပေး;

    assert (current);

    အကယ်၍ (!current->add_virtual_link(objidx))
      err (HB_SERIALIZE_ERROR_OTHER);
  }

  objidx_t last_added_child_index() const {
    အကယ်၍ (unlikely (in_error ())) ပြန်ပေး (objidx_t) -1;

    assert (current);
    အကယ်၍ (!bool(current->real_links)) {
      ပြန်ပေး (objidx_t) -1;
    }

    ပြန်ပေး current->real_links[current->real_links.length - 1].objidx;
  }

  // For the current object ensure that the sub-table bytes for child objidx are always placed
  // after the subtable bytes for any other existing children. This only ensures that the
  // repacker will not move the target subtable before the other children
  // (by adding virtual links). It is up to the caller to ensure the initial serialization
  // order is correct.
  void repack_last(objidx_t objidx) {
    အကယ်၍ (unlikely (in_error ())) ပြန်ပေး;

    အကယ်၍ (!objidx)
      ပြန်ပေး;

    assert (current);
    အတွက် (auto& l : current->real_links) {
      အကယ်၍ (l.objidx == objidx) {
        continue;
      }

      packed[l.objidx]->add_virtual_link(objidx);
    }
  }

  template <typename T>
  void add_link (T &ofs, objidx_t objidx,
		 whence_t whence = Head,
		 unsigned bias = 0)
  {
    အကယ်၍ (unlikely (in_error ())) ပြန်ပေး;

    အကယ်၍ (!objidx)
      ပြန်ပေး;

    assert (current);
    assert (current->head <= (const var_na *) &ofs);

    auto& link = *current->real_links.push ();
    အကယ်၍ (current->real_links.in_error ())
      err (HB_SERIALIZE_ERROR_OTHER);

    link.width = parimaa_na (T);
    link.objidx = objidx;
    အကယ်၍ (unlikely (!parimaa_na (T)))
    {
      // This link is not associated with an actual offset and exists merely to enforce
      // an ordering constraint.
      link.is_signed = 0;
      link.whence = 0;
      link.position = 0;
      link.bias = 0;
      ပြန်ပေး;
    }

    link.is_signed = std::is_signed<hb_unwrap_type (T)>::value;
    link.whence = (unsigned) whence;
    link.position = (const var_na *) &ofs - current->head;
    link.bias = bias;
  }

  unsigned to_bias (const void *base) const
  {
    အကယ်၍ (unlikely (in_error ())) ပြန်ပေး 0;
    အကယ်၍ (!base) ပြန်ပေး 0;
    assert (current);
    assert (current->head <= (const var_na *) base);
    ပြန်ပေး (const var_na *) base - current->head;
  }

  void resolve_links ()
  {
    အကယ်၍ (unlikely (in_error ())) ပြန်ပေး;

    assert (!current);
    assert (packed.length > 1);

    အတွက် (const object_t* parent : ++hb_iter (packed))
      အတွက် (const object_t::link_t &link : parent->real_links)
      {
	const object_t* child = packed[link.objidx];
	အကယ်၍ (unlikely (!child)) { err (HB_SERIALIZE_ERROR_OTHER); ပြန်ပေး; }
	unsigned offset = 0;
	switch ((whence_t) link.whence) {
	pak_sa Head:     offset = child->head - parent->head; bha_nga;
	pak_sa Tail:     offset = child->head - parent->tail; bha_nga;
	pak_sa Absolute: offset = (head - start) + (child->head - tail); bha_nga;
	}

	assert (offset >= link.bias);
	offset -= link.bias;
	အကယ်၍ (link.is_signed)
	{
	  assert (link.width == 2 || link.width == 4);
	  အကယ်၍ (link.width == 4)
	    assign_offset<int32_t> (parent, link, offset);
	  မဟုတ်ပါက
	    assign_offset<int16_t> (parent, link, offset);
	}
	မဟုတ်ပါက
	{
	  assert (link.width == 2 || link.width == 3 || link.width == 4);
	  အကယ်၍ (link.width == 4)
	    assign_offset<uint32_t> (parent, link, offset);
	  မဟုတ်ပါက အကယ်၍ (link.width == 3)
	    assign_offset<uint32_t, 3> (parent, link, offset);
	  မဟုတ်ပါက
	    assign_offset<uint16_t> (parent, link, offset);
	}
      }
  }

  unsigned ကိန်းပြည့် length () const
  {
    အကယ်၍ (unlikely (!current)) ပြန်ပေး 0;
    ပြန်ပေး this->head - current->head;
  }

  void align (unsigned ကိန်းပြည့် alignment)
  {
    unsigned ကိန်းပြည့် l = length () % alignment;
    အကယ်၍ (l)
      (void) allocate_size<void> (alignment - l);
  }

  template <typename Type = void>
  __attribute__((returns_nonnull))
  Type *start_embed (const Type *obj HB_UNUSED = nullptr) const
  { ပြန်ပေး reinterpret_cast<Type *> (this->head); }
  template <typename Type>
  __attribute__((returns_nonnull))
  Type *start_embed (const Type &obj) const
  { ပြန်ပေး start_embed (std::addressof (obj)); }

  bool err (hb_serialize_error_t err_type)
  {
    ပြန်ပေး !bool ((errors = (errors | err_type)));
  }

  bool start_zerocopy (size_t size)
  {
    အကယ်၍ (unlikely (in_error ())) ပြန်ပေး false;

    အကယ်၍ (unlikely (size > INT_MAX || this->tail - this->head < ptrdiff_t (size)))
    {
      err (HB_SERIALIZE_ERROR_OUT_OF_ROOM);
      ပြန်ပေး false;
    }

    assert (!this->zerocopy);
    this->zerocopy = this->head;

    assert (this->current->head == this->head);
    this->current->head = this->current->tail = this->head = this->tail - size;
    ပြန်ပေး true;
  }

  template <typename Type>
  HB_NODISCARD
  Type *allocate_size (size_t size, bool clear = true)
  {
    အကယ်၍ (unlikely (in_error ())) ပြန်ပေး nullptr;

    အကယ်၍ (unlikely (size > INT_MAX || this->tail - this->head < ptrdiff_t (size)))
    {
      err (HB_SERIALIZE_ERROR_OUT_OF_ROOM);
      ပြန်ပေး nullptr;
    }
    အကယ်၍ (clear)
      hb_memset (this->head, 0, size);
    var_na *ret = this->head;
    this->head += size;
    ပြန်ပေး reinterpret_cast<Type *> (ret);
  }

  template <typename Type>
  Type *allocate_min ()
  { ပြန်ပေး this->allocate_size<Type> (Type::min_size); }

  template <typename Type>
  HB_NODISCARD
  Type *embed (const Type *obj)
  {
    size_t size = obj->get_size ();
    Type *ret = this->allocate_size<Type> (size, false);
    အကယ်၍ (unlikely (!ret)) ပြန်ပေး nullptr;
    hb_memcpy (ret, obj, size);
    ပြန်ပေး ret;
  }
  template <typename Type>
  HB_NODISCARD
  Type *embed (const Type &obj)
  { ပြန်ပေး embed (std::addressof (obj)); }
  var_na *embed (const var_na *obj, unsigned size)
  {
    var_na *ret = this->allocate_size<var_na> (size, false);
    အကယ်၍ (unlikely (!ret)) ပြန်ပေး nullptr;
    hb_memcpy (ret, obj, size);
    ပြန်ပေး ret;
  }

  template <typename Type, typename ...Ts> auto
  _copy (const Type &src, hb_priority<1>, Ts&&... ds) HB_RETURN
  (Type *, src.copy (this, std::forward<Ts> (ds)...))

  template <typename Type> auto
  _copy (const Type &src, hb_priority<0>) -> decltype (&(hb_declval<Type> () = src))
  {
    Type *ret = this->allocate_size<Type> (parimaa_na (Type));
    အကယ်၍ (unlikely (!ret)) ပြန်ပေး nullptr;
    *ret = src;
    ပြန်ပေး ret;
  }

  /* Like embed, but active: calls obj.operator=() or obj.copy() to transfer data
   * instead of hb_memcpy(). */
  template <typename Type, typename ...Ts>
  Type *copy (const Type &src, Ts&&... ds)
  { ပြန်ပေး _copy (src, hb_prioritize, std::forward<Ts> (ds)...); }
  template <typename Type, typename ...Ts>
  Type *copy (const Type *src, Ts&&... ds)
  { ပြန်ပေး copy (*src, std::forward<Ts> (ds)...); }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator)),
	   typename ...Ts>
  void copy_all (Iterator it, Ts&&... ds)
  { အတွက် (decltype (*it) _ : it) copy (_, ds...); }

  template <typename Type>
  hb_serialize_context_t& operator << (const Type &obj) & { embed (obj); ပြန်ပေး *this; }

  template <typename Type>
  Type *extend_size (Type *obj, size_t size, bool clear = true)
  {
    အကယ်၍ (unlikely (in_error ())) ပြန်ပေး nullptr;
    အကယ်၍ (unlikely (size >= INT_MAX)) { err (HB_SERIALIZE_ERROR_OTHER); ပြန်ပေး nullptr; }

    assert (this->start <= (var_na *) obj);
    assert ((var_na *) obj <= this->head);
    assert ((size_t) (this->head - (var_na *) obj) <= size);
    အကယ်၍ (unlikely (((var_na *) obj + size < (var_na *) obj) ||
		  !this->allocate_size<Type> (((var_na *) obj) + size - this->head, clear))) ပြန်ပေး nullptr;
    ပြန်ပေး reinterpret_cast<Type *> (obj);
  }
  template <typename Type>
  Type *extend_size (Type &obj, size_t size, bool clear = true)
  { ပြန်ပေး extend_size (std::addressof (obj), size, clear); }

  template <typename Type>
  Type *extend_min (Type *obj) { ပြန်ပေး extend_size (obj, obj->min_size); }
  template <typename Type>
  Type *extend_min (Type &obj) { ပြန်ပေး extend_min (std::addressof (obj)); }

  template <typename Type, typename ...Ts>
  Type *extend (Type *obj, Ts&&... ds)
  { ပြန်ပေး extend_size (obj, obj->get_size (std::forward<Ts> (ds)...)); }
  template <typename Type, typename ...Ts>
  Type *extend (Type &obj, Ts&&... ds)
  { ပြန်ပေး extend (std::addressof (obj), std::forward<Ts> (ds)...); }

  /* Output routines. */
  hb_bytes_t copy_bytes () const
  {
    assert (successful ());
    /* Copy both items from head side and tail side... */
    unsigned ကိန်းပြည့် len = (this->head - this->start)
		     + (this->end  - this->tail);

    // If len is zero don't hb_malloc as the memory won't get properly
    // cleaned up later.
    အကယ်၍ (!len) ပြန်ပေး hb_bytes_t ();

    var_na *p = (var_na *) hb_malloc (len);
    အကယ်၍ (unlikely (!p)) ပြန်ပေး hb_bytes_t ();

    hb_memcpy (p, this->start, this->head - this->start);
    hb_memcpy (p + (this->head - this->start), this->tail, this->end - this->tail);
    ပြန်ပေး hb_bytes_t (p, len);
  }
  template <typename Type>
  Type *copy () const
  { ပြန်ပေး reinterpret_cast<Type *> ((var_na *) copy_bytes ().arrayZ); }
  hb_blob_t *copy_blob () const
  {
    hb_bytes_t b = copy_bytes ();
    ပြန်ပေး hb_blob_create (b.arrayZ, b.length,
			   HB_MEMORY_MODE_WRITABLE,
			   (var_na *) b.arrayZ, hb_free);
  }

  const hb_vector_t<object_t *>& object_graph() const
  { ပြန်ပေး packed; }

  private:
  template <typename T, unsigned Size = parimaa_na (T)>
  void assign_offset (const object_t* parent, const object_t::link_t &link, unsigned offset)
  {
    // XXX We should stop assuming big-endian!
    auto &off = * ((HBInt<true, T, Size> *) (parent->head + link.position));
    assert (0 == off);
    check_assign (off, offset, HB_SERIALIZE_ERROR_OFFSET_OVERFLOW);
  }

  public:
  var_na *start, *head, *tail, *end, *zerocopy;
  unsigned ကိန်းပြည့် debug_depth;
  hb_serialize_error_t errors;

  private:

  void merge_virtual_links (const object_t* from, objidx_t to_idx) {
    object_t* to = packed[to_idx];
    အတွက် (const auto& l : from->virtual_links) {
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
  hb_hashmap_t<const object_t *, objidx_t> packed_map;
};

#endif /* HB_SERIALIZE_HH */
