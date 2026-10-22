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

#অন্তর্ভাব "hb.hh"
#অন্তর্ভাব "hb-blob.hh"
#অন্তর্ভাব "hb-map.hh"
#অন্তর্ভাব "hb-free-pool.hh"

#অন্তর্ভাব "hb-subset-serialize.h"

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

কাঠামো hb_serialize_context_t
{
  প্রকারলেখ unsigned objidx_t;

  গণনা whence_t {
     শীর্ষ,	/* Relative to the current object head (default). */
     লেজ_2,	/* Relative to the current object tail after packed. */
     Absolute	/* Absolute: from the start of the serialize buffer. */
   };



  কাঠামো object_t
  {
    শূন্য fini () {
      real_links.fini ();
      virtual_links.fini ();
    }

    object_t () = পূর্বনির্ধারিত;

    object_t (ধ্রুবক hb_subset_serialize_object_t &o)
    {
      শীর্ষ_2 = o.শীর্ষ_2;
      লেজ = o.লেজ;
      next = nullptr;
      real_links.alloc_exact (o.অঙ্ক_real_links);
      জন্য (unsigned i = 0 ; i < o.অঙ্ক_real_links; i++)
        real_links.push (o.real_links[i]);

      virtual_links.alloc_exact (o.অঙ্ক_virtual_links);
      জন্য (unsigned i = 0; i < o.অঙ্ক_virtual_links; i++)
        virtual_links.push (o.virtual_links[i]);
    }

    যৌক্তিক in_error () ধ্রুবক
    {
      ফেরত real_links.in_error() || virtual_links.in_error();
    }

    যৌক্তিক add_virtual_link (objidx_t objidx)
    {
      যদি (!objidx)
        ফেরত মিথ্যা;

      auto& link = *virtual_links.push ();
      যদি (virtual_links.in_error ())
        ফেরত মিথ্যা;

      link.objidx = objidx;
      // Remaining fields were previously zero'd by push():
      // link.width = 0;
      // link.is_signed = 0;
      // link.whence = 0;
      // link.position = 0;
      // link.bias = 0;

      ফেরত সত্য;
    }

    friend শূন্য swap (object_t& a, object_t& b) noexcept
    {
      hb_swap (a.শীর্ষ_2, b.শীর্ষ_2);
      hb_swap (a.লেজ, b.লেজ);
      hb_swap (a.next, b.next);
      hb_swap (a.real_links, b.real_links);
      hb_swap (a.virtual_links, b.virtual_links);
    }

    যৌক্তিক operator == (ধ্রুবক object_t &o) ধ্রুবক
    {
      // Virtual links aren't considered for equality since they don't affect the functionality
      // of the object.
      ফেরত (লেজ - শীর্ষ_2 == o.লেজ - o.শীর্ষ_2)
	  && (real_links.দৈর্ঘ্য == o.real_links.দৈর্ঘ্য)
	  && 0 == hb_memcmp (শীর্ষ_2, o.শীর্ষ_2, লেজ - শীর্ষ_2)
	  && real_links.as_bytes () == o.real_links.as_bytes ();
    }
    uint32_t hash () ধ্রুবক
    {
      // Virtual links aren't considered for equality since they don't affect the functionality
      // of the object.
      ফেরত hb_bytes_t (শীর্ষ_2, hb_min (128, লেজ - শীর্ষ_2)).hash () ^
          real_links.as_bytes ().hash ();
    }

    কাঠামো link_t
    {
      unsigned width: 3;
      unsigned is_signed: 1;
      unsigned whence: 2;
      unsigned bias : 26;
      unsigned position;
      objidx_t objidx;

      link_t () = পূর্বনির্ধারিত;

      link_t (ধ্রুবক hb_subset_serialize_link_t &o)
      {
        width = o.width;
        is_signed = 0;
        whence = 0;
        position = o.position;
        bias = 0;
        objidx = o.objidx;
      }

      HB_INTERNAL স্থির পূর্ণসংখ্যা cmp (ধ্রুবক শূন্য* a, ধ্রুবক শূন্য* b)
      {
        পূর্ণসংখ্যা cmp = ((ধ্রুবক link_t*)a)->position - ((ধ্রুবক link_t*)b)->position;
        যদি (cmp) ফেরত cmp;

        ফেরত ((ধ্রুবক link_t*)a)->objidx - ((ধ্রুবক link_t*)b)->objidx;
      }
    };

    অক্ষর *শীর্ষ_2;
    অক্ষর *লেজ;
    hb_vector_t<link_t> real_links;
    hb_vector_t<link_t> virtual_links;
    object_t *next;

    auto all_links () ধ্রুবক HB_AUTO_RETURN
        (( hb_concat (real_links, virtual_links) ));
    auto all_links_writer () HB_AUTO_RETURN
        (( hb_concat (real_links.writer (), virtual_links.writer ()) ));
  };

  কাঠামো snapshot_t
  {
    অক্ষর *শীর্ষ_2;
    অক্ষর *লেজ;
    object_t *current; // Just for sanity check
    unsigned অঙ্ক_real_links;
    unsigned অঙ্ক_virtual_links;
    hb_serialize_error_t errors;
  };

  snapshot_t snapshot ()
  {
    ফেরত snapshot_t {
      শীর্ষ_2, লেজ, current,
      current ? current->real_links.দৈর্ঘ্য : 0,
      current ? current->virtual_links.দৈর্ঘ্য : 0,
      errors
     };
  }

  hb_serialize_context_t (শূন্য *start_, size_t আকার_2) :
    start ((অক্ষর *) start_),
    end (start + আকার_2),
    current (nullptr)
  { reset (); }
  ~hb_serialize_context_t () { fini (); }

  শূন্য fini ()
  {
    জন্য (object_t *_ : ++hb_iter (packed)) _->fini ();
    packed.fini ();
    এই->packed_map.fini ();

    যখন (current)
    {
      auto *_ = current;
      current = current->next;
      _->fini ();
    }
  }

  যৌক্তিক in_error () ধ্রুবক { ফেরত যৌক্তিক (errors); }

  যৌক্তিক successful () ধ্রুবক { ফেরত !যৌক্তিক (errors); }

  HB_NODISCARD যৌক্তিক ran_out_of_room () ধ্রুবক { ফেরত errors & HB_SERIALIZE_ERROR_OUT_OF_ROOM; }
  HB_NODISCARD যৌক্তিক offset_overflow () ধ্রুবক { ফেরত errors & HB_SERIALIZE_ERROR_OFFSET_OVERFLOW; }
  HB_NODISCARD যৌক্তিক only_offset_overflow () ধ্রুবক { ফেরত errors == HB_SERIALIZE_ERROR_OFFSET_OVERFLOW; }
  HB_NODISCARD যৌক্তিক only_overflow () ধ্রুবক
  {
    ফেরত errors == HB_SERIALIZE_ERROR_OFFSET_OVERFLOW
        || errors == HB_SERIALIZE_ERROR_INT_OVERFLOW
        || errors == HB_SERIALIZE_ERROR_ARRAY_OVERFLOW;
  }

  শূন্য reset (শূন্য *start_, size_t আকার_2)
  {
    start = (অক্ষর*) start_;
    end = start + আকার_2;
    reset ();
    current = nullptr;
  }

  শূন্য reset ()
  {
    এই->errors = HB_SERIALIZE_ERROR_NONE;
    এই->শীর্ষ_2 = এই->start;
    এই->লেজ = এই->end;
    এই->zerocopy = nullptr;
    এই->debug_depth = 0;

    fini ();
    এই->packed.push (nullptr);
    এই->packed_map.init ();
  }

  যৌক্তিক check_success (যৌক্তিক success,
                      hb_serialize_error_t err_type = HB_SERIALIZE_ERROR_OTHER)
  {
    ফেরত successful ()
        && (success || err (err_type));
  }

  template <typename T1, typename T2>
  যৌক্তিক check_equal (T1 &&v1, T2 &&v2, hb_serialize_error_t err_type)
  {
    যদি ((long long) v1 != (long long) v2)
    {
      ফেরত err (err_type);
    }
    ফেরত সত্য;
  }

  template <typename T1, typename T2>
  যৌক্তিক check_assign (T1 &v1, T2 &&v2, hb_serialize_error_t err_type)
  { ফেরত check_equal (v1 = v2, v2, err_type); }

  template <typename T> যৌক্তিক propagate_error (T &&obj)
  { ফেরত check_success (!hb_deref (obj).in_error ()); }

  template <typename T1, typename... Ts> যৌক্তিক propagate_error (T1 &&o1, Ts&&... os)
  { ফেরত propagate_error (std::forward<T1> (o1)) &&
	   propagate_error (std::forward<Ts> (os)...); }

  /* To be called around main operation. */
  template <typename Type=অক্ষর>
  __attribute__((returns_nonnull))
  Type *start_serialize ()
  {
    DEBUG_বার্তা_LEVEL (SERIALIZE, এই->start, 0, +1,
		     "start [%p..%p] (%lu bytes)",
		     এই->start, এই->end,
		     (unsigned long) (এই->end - এই->start));

    প্রতিজ্ঞা (!current);
    ফেরত push<Type> ();
  }
  শূন্য end_serialize ()
  {
    DEBUG_বার্তা_LEVEL (SERIALIZE, এই->start, 0, -1,
		     "end [%p..%p] serialized %u bytes; %s",
		     এই->start, এই->end,
		     (unsigned) (এই->শীর্ষ_2 - এই->start),
		     successful () ? "successful" : "UNSUCCESSFUL");

    propagate_error (packed, packed_map);

    যদি (unlikely (!current)) ফেরত;
    যদি (unlikely (in_error()))
    {
      // Offset overflows that occur before link resolution cannot be handled
      // by repacking, so set a more general error.
      যদি (offset_overflow ()) err (HB_SERIALIZE_ERROR_OTHER);
      ফেরত;
    }

    প্রতিজ্ঞা (!current->next);

    /* Only "pack" if there exist other objects... Otherwise, don't bother.
     * Saves a move. */
    যদি (packed.দৈর্ঘ্য <= 1)
      ফেরত;

    বের_করো_pack (মিথ্যা);

    resolve_links ();
  }

  template <typename Type = শূন্য>
  __attribute__((returns_nonnull))
  Type *push ()
  {
    যদি (unlikely (in_error ())) ফেরত start_embed<Type> ();

    object_t *obj = object_pool.alloc ();
    যদি (unlikely (!obj))
      check_success (মিথ্যা);
    অন্যথা
    {
      obj->শীর্ষ_2 = শীর্ষ_2;
      obj->লেজ = লেজ;
      obj->next = current;
      current = obj;
    }
    ফেরত start_embed<Type> ();
  }
  শূন্য বের_করো_discard ()
  {
    object_t *obj = current;
    যদি (unlikely (!obj)) ফেরত;
    // Allow cleanup when we've error'd out on int overflows which don't compromise
    // the serializer state.
    যদি (unlikely (in_error() && !only_overflow ())) ফেরত;

    current = current->next;
    revert (zerocopy ? zerocopy : obj->শীর্ষ_2, obj->লেজ);
    zerocopy = nullptr;
    obj->fini ();
    object_pool.release (obj);
  }

  /* Set share to false when an object is unlikely shareable with others
   * so not worth an attempt, or a contiguous table is serialized as
   * multiple consecutive objects in the reverse order so can't be shared.
   */
  objidx_t বের_করো_pack (যৌক্তিক share=সত্য)
  {
    object_t *obj = current;
    যদি (unlikely (!obj)) ফেরত 0;
    // Allow cleanup when we've error'd out on int overflows which don't compromise
    // the serializer state.
    যদি (unlikely (in_error()  && !only_overflow ())) ফেরত 0;

    current = current->next;
    obj->লেজ = শীর্ষ_2;
    obj->next = nullptr;
    প্রতিজ্ঞা (obj->শীর্ষ_2 <= obj->লেজ);
    unsigned len = obj->লেজ - obj->শীর্ষ_2;
    শীর্ষ_2 = zerocopy ? zerocopy : obj->শীর্ষ_2; /* Rewind head. */
    যৌক্তিক was_zerocopy = zerocopy;
    zerocopy = nullptr;

    যদি (!len)
    {
      প্রতিজ্ঞা (!obj->real_links.দৈর্ঘ্য);
      প্রতিজ্ঞা (!obj->virtual_links.দৈর্ঘ্য);
      ফেরত 0;
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
	ফেরত objidx;
      }
    }

    লেজ -= len;
    যদি (was_zerocopy)
      প্রতিজ্ঞা (লেজ == obj->শীর্ষ_2);
    অন্যথা
      memmove (লেজ, obj->শীর্ষ_2, len);

    obj->শীর্ষ_2 = লেজ;
    obj->লেজ = লেজ + len;

    packed.push (obj);

    যদি (unlikely (!propagate_error (packed)))
    {
      /* Obj wasn't successfully added to packed, so clean it up otherwise its
       * links will be leaked. When we use constructor/destructors properly, we
       * can remove these. */
      obj->fini ();
      ফেরত 0;
    }

    objidx = packed.দৈর্ঘ্য - 1;

    যদি (share) packed_map.set_with_hash (obj, hash, objidx);
    propagate_error (packed_map);

    ফেরত objidx;
  }

  শূন্য revert (snapshot_t snap)
  {
    // Overflows that happened after the snapshot will be erased by the revert.
    যদি (unlikely (in_error () && !only_overflow ())) ফেরত;
    প্রতিজ্ঞা (snap.current == current);
    যদি (current)
    {
      current->real_links.shrink (snap.অঙ্ক_real_links);
      current->virtual_links.shrink (snap.অঙ্ক_virtual_links);
    }
    errors = snap.errors;
    revert (snap.শীর্ষ_2, snap.লেজ);
  }

  শূন্য revert (অক্ষর *snap_শীর্ষ,
	       অক্ষর *snap_লেজ)
  {
    যদি (unlikely (in_error ())) ফেরত;
    প্রতিজ্ঞা (snap_শীর্ষ <= শীর্ষ_2);
    প্রতিজ্ঞা (লেজ <= snap_লেজ);
    শীর্ষ_2 = snap_শীর্ষ;
    লেজ = snap_লেজ;
    discard_stale_objects ();
  }

  শূন্য discard_stale_objects ()
  {
    যদি (unlikely (in_error ())) ফেরত;
    যখন (packed.দৈর্ঘ্য > 1 &&
	   packed.লেজ ()->শীর্ষ_2 < লেজ)
    {
      object_t *obj = packed.লেজ ();
      packed_map.del (obj);
      প্রতিজ্ঞা (!obj->next);
      obj->fini ();
      object_pool.release (obj);
      packed.বের_করো ();
    }
    যদি (packed.দৈর্ঘ্য > 1)
      প্রতিজ্ঞা (packed.লেজ ()->শীর্ষ_2 == লেজ);
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
    যদি (unlikely (in_error ())) ফেরত;

    যদি (!objidx)
      ফেরত;

    প্রতিজ্ঞা (current);

    যদি (!current->add_virtual_link(objidx))
      err (HB_SERIALIZE_ERROR_OTHER);
  }

  objidx_t last_added_child_সূচক() ধ্রুবক {
    যদি (unlikely (in_error ())) ফেরত (objidx_t) -1;

    প্রতিজ্ঞা (current);
    যদি (!যৌক্তিক(current->real_links)) {
      ফেরত (objidx_t) -1;
    }

    ফেরত current->real_links[current->real_links.দৈর্ঘ্য - 1].objidx;
  }

  // For the current object ensure that the sub-table bytes for child objidx are always placed
  // after the subtable bytes for any other existing children. This only ensures that the
  // repacker will not move the target subtable before the other children
  // (by adding virtual links). It is up to the caller to ensure the initial serialization
  // order is correct.
  শূন্য repack_last(objidx_t objidx) {
    যদি (unlikely (in_error ())) ফেরত;

    যদি (!objidx)
      ফেরত;

    প্রতিজ্ঞা (current);
    জন্য (auto& l : current->real_links) {
      যদি (l.objidx == objidx) {
        চালু;
      }

      packed[l.objidx]->add_virtual_link(objidx);
    }
  }

  template <typename T>
  শূন্য add_link (T &ofs, objidx_t objidx,
		 whence_t whence = শীর্ষ,
		 unsigned bias = 0)
  {
    যদি (unlikely (in_error ())) ফেরত;

    যদি (!objidx)
      ফেরত;

    প্রতিজ্ঞা (current);
    প্রতিজ্ঞা (current->শীর্ষ_2 <= (ধ্রুবক অক্ষর *) &ofs);

    auto& link = *current->real_links.push ();
    যদি (current->real_links.in_error ())
      err (HB_SERIALIZE_ERROR_OTHER);

    link.width = আকার (T);
    link.objidx = objidx;
    যদি (unlikely (!আকার (T)))
    {
      // This link is not associated with an actual offset and exists merely to enforce
      // an ordering constraint.
      link.is_signed = 0;
      link.whence = 0;
      link.position = 0;
      link.bias = 0;
      ফেরত;
    }

    link.is_signed = std::is_signed<hb_unwrap_type (T)>::value;
    link.whence = (unsigned) whence;
    link.position = (ধ্রুবক অক্ষর *) &ofs - current->শীর্ষ_2;
    link.bias = bias;
  }

  unsigned to_bias (ধ্রুবক শূন্য *base) ধ্রুবক
  {
    যদি (unlikely (in_error ())) ফেরত 0;
    যদি (!base) ফেরত 0;
    প্রতিজ্ঞা (current);
    প্রতিজ্ঞা (current->শীর্ষ_2 <= (ধ্রুবক অক্ষর *) base);
    ফেরত (ধ্রুবক অক্ষর *) base - current->শীর্ষ_2;
  }

  শূন্য resolve_links ()
  {
    যদি (unlikely (in_error ())) ফেরত;

    প্রতিজ্ঞা (!current);
    প্রতিজ্ঞা (packed.দৈর্ঘ্য > 1);

    জন্য (ধ্রুবক object_t* parent : ++hb_iter (packed))
      জন্য (ধ্রুবক object_t::link_t &link : parent->real_links)
      {
	ধ্রুবক object_t* child = packed[link.objidx];
	যদি (unlikely (!child)) { err (HB_SERIALIZE_ERROR_OTHER); ফেরত; }
	unsigned offset = 0;
	নির্বাচন ((whence_t) link.whence) {
	ক্ষেত্র শীর্ষ:     offset = child->শীর্ষ_2 - parent->শীর্ষ_2; বিরতি;
	ক্ষেত্র লেজ_2:     offset = child->শীর্ষ_2 - parent->লেজ; বিরতি;
	ক্ষেত্র Absolute: offset = (শীর্ষ_2 - start) + (child->শীর্ষ_2 - লেজ); বিরতি;
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

  unsigned পূর্ণসংখ্যা দৈর্ঘ্য () ধ্রুবক
  {
    যদি (unlikely (!current)) ফেরত 0;
    ফেরত এই->শীর্ষ_2 - current->শীর্ষ_2;
  }

  শূন্য align (unsigned পূর্ণসংখ্যা alignment)
  {
    unsigned পূর্ণসংখ্যা l = দৈর্ঘ্য () % alignment;
    যদি (l)
      (শূন্য) allocate_আকার<শূন্য> (alignment - l);
  }

  template <typename Type = শূন্য>
  __attribute__((returns_nonnull))
  Type *start_embed (ধ্রুবক Type *obj HB_UNUSED = nullptr) ধ্রুবক
  { ফেরত reinterpret_cast<Type *> (এই->শীর্ষ_2); }
  template <typename Type>
  __attribute__((returns_nonnull))
  Type *start_embed (ধ্রুবক Type &obj) ধ্রুবক
  { ফেরত start_embed (std::addressof (obj)); }

  যৌক্তিক err (hb_serialize_error_t err_type)
  {
    ফেরত !যৌক্তিক ((errors = (errors | err_type)));
  }

  যৌক্তিক start_zerocopy (size_t আকার_2)
  {
    যদি (unlikely (in_error ())) ফেরত মিথ্যা;

    যদি (unlikely (আকার_2 > INT_MAX || এই->লেজ - এই->শীর্ষ_2 < ptrdiff_t (আকার_2)))
    {
      err (HB_SERIALIZE_ERROR_OUT_OF_ROOM);
      ফেরত মিথ্যা;
    }

    প্রতিজ্ঞা (!এই->zerocopy);
    এই->zerocopy = এই->শীর্ষ_2;

    প্রতিজ্ঞা (এই->current->শীর্ষ_2 == এই->শীর্ষ_2);
    এই->current->শীর্ষ_2 = এই->current->লেজ = এই->শীর্ষ_2 = এই->লেজ - আকার_2;
    ফেরত সত্য;
  }

  template <typename Type>
  HB_NODISCARD
  Type *allocate_আকার (size_t আকার_2, যৌক্তিক clear = সত্য)
  {
    যদি (unlikely (in_error ())) ফেরত nullptr;

    যদি (unlikely (আকার_2 > INT_MAX || এই->লেজ - এই->শীর্ষ_2 < ptrdiff_t (আকার_2)))
    {
      err (HB_SERIALIZE_ERROR_OUT_OF_ROOM);
      ফেরত nullptr;
    }
    যদি (clear)
      hb_memset (এই->শীর্ষ_2, 0, আকার_2);
    অক্ষর *ret = এই->শীর্ষ_2;
    এই->শীর্ষ_2 += আকার_2;
    ফেরত reinterpret_cast<Type *> (ret);
  }

  template <typename Type>
  Type *allocate_min ()
  { ফেরত এই->allocate_আকার<Type> (Type::min_আকার); }

  template <typename Type>
  HB_NODISCARD
  Type *embed (ধ্রুবক Type *obj)
  {
    size_t আকার_2 = obj->get_আকার ();
    Type *ret = এই->allocate_আকার<Type> (আকার_2, মিথ্যা);
    যদি (unlikely (!ret)) ফেরত nullptr;
    hb_memcpy (ret, obj, আকার_2);
    ফেরত ret;
  }
  template <typename Type>
  HB_NODISCARD
  Type *embed (ধ্রুবক Type &obj)
  { ফেরত embed (std::addressof (obj)); }
  অক্ষর *embed (ধ্রুবক অক্ষর *obj, unsigned আকার_2)
  {
    অক্ষর *ret = এই->allocate_আকার<অক্ষর> (আকার_2, মিথ্যা);
    যদি (unlikely (!ret)) ফেরত nullptr;
    hb_memcpy (ret, obj, আকার_2);
    ফেরত ret;
  }

  template <typename Type, typename ...Ts> auto
  _copy (ধ্রুবক Type &src, hb_priority<1>, Ts&&... ds) HB_RETURN
  (Type *, src.copy (এই, std::forward<Ts> (ds)...))

  template <typename Type> auto
  _copy (ধ্রুবক Type &src, hb_priority<0>) -> decltype (&(hb_declval<Type> () = src))
  {
    Type *ret = এই->allocate_আকার<Type> (আকার (Type));
    যদি (unlikely (!ret)) ফেরত nullptr;
    *ret = src;
    ফেরত ret;
  }

  /* Like embed, but active: calls obj.operator=() or obj.copy() to transfer data
   * instead of hb_memcpy(). */
  template <typename Type, typename ...Ts>
  Type *copy (ধ্রুবক Type &src, Ts&&... ds)
  { ফেরত _copy (src, hb_prioritize, std::forward<Ts> (ds)...); }
  template <typename Type, typename ...Ts>
  Type *copy (ধ্রুবক Type *src, Ts&&... ds)
  { ফেরত copy (*src, std::forward<Ts> (ds)...); }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator)),
	   typename ...Ts>
  শূন্য copy_all (Iterator it, Ts&&... ds)
  { জন্য (decltype (*it) _ : it) copy (_, ds...); }

  template <typename Type>
  hb_serialize_context_t& operator << (ধ্রুবক Type &obj) & { embed (obj); ফেরত *এই; }

  template <typename Type>
  Type *extend_আকার (Type *obj, size_t আকার_2, যৌক্তিক clear = সত্য)
  {
    যদি (unlikely (in_error ())) ফেরত nullptr;
    যদি (unlikely (আকার_2 >= INT_MAX)) { err (HB_SERIALIZE_ERROR_OTHER); ফেরত nullptr; }

    প্রতিজ্ঞা (এই->start <= (অক্ষর *) obj);
    প্রতিজ্ঞা ((অক্ষর *) obj <= এই->শীর্ষ_2);
    প্রতিজ্ঞা ((size_t) (এই->শীর্ষ_2 - (অক্ষর *) obj) <= আকার_2);
    যদি (unlikely (((অক্ষর *) obj + আকার_2 < (অক্ষর *) obj) ||
		  !এই->allocate_আকার<Type> (((অক্ষর *) obj) + আকার_2 - এই->শীর্ষ_2, clear))) ফেরত nullptr;
    ফেরত reinterpret_cast<Type *> (obj);
  }
  template <typename Type>
  Type *extend_আকার (Type &obj, size_t আকার_2, যৌক্তিক clear = সত্য)
  { ফেরত extend_আকার (std::addressof (obj), আকার_2, clear); }

  template <typename Type>
  Type *extend_min (Type *obj) { ফেরত extend_আকার (obj, obj->min_আকার); }
  template <typename Type>
  Type *extend_min (Type &obj) { ফেরত extend_min (std::addressof (obj)); }

  template <typename Type, typename ...Ts>
  Type *extend (Type *obj, Ts&&... ds)
  { ফেরত extend_আকার (obj, obj->get_আকার (std::forward<Ts> (ds)...)); }
  template <typename Type, typename ...Ts>
  Type *extend (Type &obj, Ts&&... ds)
  { ফেরত extend (std::addressof (obj), std::forward<Ts> (ds)...); }

  /* Output routines. */
  hb_bytes_t copy_bytes () ধ্রুবক
  {
    প্রতিজ্ঞা (successful ());
    /* Copy both items from head side and tail side... */
    unsigned পূর্ণসংখ্যা len = (এই->শীর্ষ_2 - এই->start)
		     + (এই->end  - এই->লেজ);

    // If len is zero don't hb_malloc as the memory won't get properly
    // cleaned up later.
    যদি (!len) ফেরত hb_bytes_t ();

    অক্ষর *p = (অক্ষর *) hb_malloc (len);
    যদি (unlikely (!p)) ফেরত hb_bytes_t ();

    hb_memcpy (p, এই->start, এই->শীর্ষ_2 - এই->start);
    hb_memcpy (p + (এই->শীর্ষ_2 - এই->start), এই->লেজ, এই->end - এই->লেজ);
    ফেরত hb_bytes_t (p, len);
  }
  template <typename Type>
  Type *copy () ধ্রুবক
  { ফেরত reinterpret_cast<Type *> ((অক্ষর *) copy_bytes ().arrayZ); }
  hb_blob_t *copy_blob () ধ্রুবক
  {
    hb_bytes_t b = copy_bytes ();
    ফেরত hb_blob_create (b.arrayZ, b.দৈর্ঘ্য,
			   HB_MEMORY_MODE_WRITABLE,
			   (অক্ষর *) b.arrayZ, hb_free);
  }

  ধ্রুবক hb_vector_t<object_t *>& object_graph() ধ্রুবক
  { ফেরত packed; }

  ব্যক্তিগত:
  template <typename T, unsigned আকার_3 = আকার (T)>
  শূন্য assign_offset (ধ্রুবক object_t* parent, ধ্রুবক object_t::link_t &link, unsigned offset)
  {
    // XXX We should stop assuming big-endian!
    auto &off = * ((HBInt<সত্য, T, আকার_3> *) (parent->শীর্ষ_2 + link.position));
    প্রতিজ্ঞা (0 == off);
    check_assign (off, offset, HB_SERIALIZE_ERROR_OFFSET_OVERFLOW);
  }

  সর্বজনীন:
  অক্ষর *start, *শীর্ষ_2, *লেজ, *end, *zerocopy;
  unsigned পূর্ণসংখ্যা debug_depth;
  hb_serialize_error_t errors;

  ব্যক্তিগত:

  শূন্য merge_virtual_links (ধ্রুবক object_t* from, objidx_t to_idx) {
    object_t* to = packed[to_idx];
    জন্য (ধ্রুবক auto& l : from->virtual_links) {
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
  hb_hashmap_t<ধ্রুবক object_t *, objidx_t> packed_map;
};

#endif /* HB_SERIALIZE_HH */
