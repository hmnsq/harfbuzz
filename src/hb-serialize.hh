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
  प्रकारव्याख्या अचिन्हित objidx_t;

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

    object_t () = पूर्वनिर्धारित;

    object_t (स्थिरांक hb_subset_serialize_object_t &o)
    {
      head = o.head;
      tail = o.tail;
      next = nullptr;
      real_links.alloc_exact (o.अंक_real_links);
      साठी (अचिन्हित i = 0 ; i < o.अंक_real_links; i++)
        real_links.push (o.real_links[i]);

      virtual_links.alloc_exact (o.अंक_virtual_links);
      साठी (अचिन्हित i = 0; i < o.अंक_virtual_links; i++)
        virtual_links.push (o.virtual_links[i]);
    }

    तार्किक in_error () स्थिरांक
    {
      परत real_links.in_error() || virtual_links.in_error();
    }

    तार्किक add_virtual_link (objidx_t objidx)
    {
      जर (!objidx)
        परत असत्य;

      स्वतः& link = *virtual_links.push ();
      जर (virtual_links.in_error ())
        परत असत्य;

      link.objidx = objidx;
      // Remaining fields were previously zero'd by push():
      // link.width = 0;
      // link.is_signed = 0;
      // link.whence = 0;
      // link.position = 0;
      // link.bias = 0;

      परत सत्य;
    }

    मित्र शून्य swap (object_t& a, object_t& b) noexcept
    {
      hb_swap (a.head, b.head);
      hb_swap (a.tail, b.tail);
      hb_swap (a.next, b.next);
      hb_swap (a.real_links, b.real_links);
      hb_swap (a.virtual_links, b.virtual_links);
    }

    तार्किक चालक == (स्थिरांक object_t &o) स्थिरांक
    {
      // Virtual links aren't considered for equality since they don't affect the functionality
      // of the object.
      परत (tail - head == o.tail - o.head)
	  && (real_links.length == o.real_links.length)
	  && 0 == hb_memcmp (head, o.head, tail - head)
	  && real_links.as_bytes () == o.real_links.as_bytes ();
    }
    uint32_t hash () स्थिरांक
    {
      // Virtual links aren't considered for equality since they don't affect the functionality
      // of the object.
      परत hb_bytes_t (head, hb_min (128, tail - head)).hash () ^
          real_links.as_bytes ().hash ();
    }

    संरचना link_t
    {
      अचिन्हित width: 3;
      अचिन्हित is_signed: 1;
      अचिन्हित whence: 2;
      अचिन्हित bias : 26;
      अचिन्हित position;
      objidx_t objidx;

      link_t () = पूर्वनिर्धारित;

      link_t (स्थिरांक hb_subset_serialize_link_t &o)
      {
        width = o.width;
        is_signed = 0;
        whence = 0;
        position = o.position;
        bias = 0;
        objidx = o.objidx;
      }

      HB_INTERNAL स्थिर पूर्णांक cmp (स्थिरांक शून्य* a, स्थिरांक शून्य* b)
      {
        पूर्णांक cmp = ((स्थिरांक link_t*)a)->position - ((स्थिरांक link_t*)b)->position;
        जर (cmp) परत cmp;

        परत ((स्थिरांक link_t*)a)->objidx - ((स्थिरांक link_t*)b)->objidx;
      }
    };

    अक्षर *head;
    अक्षर *tail;
    hb_vector_t<link_t> real_links;
    hb_vector_t<link_t> virtual_links;
    object_t *next;

    स्वतः all_links () स्थिरांक HB_AUTO_RETURN
        (( hb_concat (real_links, virtual_links) ));
    स्वतः all_links_writer () HB_AUTO_RETURN
        (( hb_concat (real_links.writer (), virtual_links.writer ()) ));
  };

  संरचना snapshot_t
  {
    अक्षर *head;
    अक्षर *tail;
    object_t *current; // Just for sanity check
    अचिन्हित अंक_real_links;
    अचिन्हित अंक_virtual_links;
    hb_serialize_error_t errors;
  };

  snapshot_t snapshot ()
  {
    परत snapshot_t {
      head, tail, current,
      current ? current->real_links.length : 0,
      current ? current->virtual_links.length : 0,
      errors
     };
  }

  hb_serialize_context_t (शून्य *start_, माप_प्रकार size) :
    start ((अक्षर *) start_),
    end (start + size),
    current (nullptr)
  { reset (); }
  ~hb_serialize_context_t () { fini (); }

  शून्य fini ()
  {
    साठी (object_t *_ : ++hb_iter (packed)) _->fini ();
    packed.fini ();
    हे->packed_map.fini ();

    पर्यंत (current)
    {
      स्वतः *_ = current;
      current = current->next;
      _->fini ();
    }
  }

  तार्किक in_error () स्थिरांक { परत तार्किक (errors); }

  तार्किक successful () स्थिरांक { परत !तार्किक (errors); }

  HB_NODISCARD तार्किक ran_out_of_room () स्थिरांक { परत errors & HB_SERIALIZE_ERROR_OUT_OF_ROOM; }
  HB_NODISCARD तार्किक offset_overflow () स्थिरांक { परत errors & HB_SERIALIZE_ERROR_OFFSET_OVERFLOW; }
  HB_NODISCARD तार्किक only_offset_overflow () स्थिरांक { परत errors == HB_SERIALIZE_ERROR_OFFSET_OVERFLOW; }
  HB_NODISCARD तार्किक only_overflow () स्थिरांक
  {
    परत errors == HB_SERIALIZE_ERROR_OFFSET_OVERFLOW
        || errors == HB_SERIALIZE_ERROR_INT_OVERFLOW
        || errors == HB_SERIALIZE_ERROR_ARRAY_OVERFLOW;
  }

  शून्य reset (शून्य *start_, माप_प्रकार size)
  {
    start = (अक्षर*) start_;
    end = start + size;
    reset ();
    current = nullptr;
  }

  शून्य reset ()
  {
    हे->errors = HB_SERIALIZE_ERROR_NONE;
    हे->head = हे->start;
    हे->tail = हे->end;
    हे->zerocopy = nullptr;
    हे->debug_depth = 0;

    fini ();
    हे->packed.push (nullptr);
    हे->packed_map.init ();
  }

  तार्किक check_success (तार्किक success,
                      hb_serialize_error_t err_type = HB_SERIALIZE_ERROR_OTHER)
  {
    परत successful ()
        && (success || err (err_type));
  }

  ढाँचा <typename T1, typename T2>
  तार्किक check_equal (T1 &&v1, T2 &&v2, hb_serialize_error_t err_type)
  {
    जर ((दीर्घ दीर्घ) v1 != (दीर्घ दीर्घ) v2)
    {
      परत err (err_type);
    }
    परत सत्य;
  }

  ढाँचा <typename T1, typename T2>
  तार्किक check_assign (T1 &v1, T2 &&v2, hb_serialize_error_t err_type)
  { परत check_equal (v1 = v2, v2, err_type); }

  ढाँचा <typename T> तार्किक propagate_error (T &&obj)
  { परत check_success (!hb_deref (obj).in_error ()); }

  ढाँचा <typename T1, typename... Ts> तार्किक propagate_error (T1 &&o1, Ts&&... os)
  { परत propagate_error (std::forward<T1> (o1)) &&
	   propagate_error (std::forward<Ts> (os)...); }

  /* To be called around main operation. */
  ढाँचा <typename Type=अक्षर>
  __attribute__((returns_nonnull))
  Type *start_serialize ()
  {
    DEBUG_संदेश_LEVEL (SERIALIZE, हे->start, 0, +1,
		     "start [%p..%p] (%lu bytes)",
		     हे->start, हे->end,
		     (अचिन्हित दीर्घ) (हे->end - हे->start));

    प्रतिज्ञा (!current);
    परत push<Type> ();
  }
  शून्य end_serialize ()
  {
    DEBUG_संदेश_LEVEL (SERIALIZE, हे->start, 0, -1,
		     "end [%p..%p] serialized %u bytes; %s",
		     हे->start, हे->end,
		     (अचिन्हित) (हे->head - हे->start),
		     successful () ? "successful" : "UNSUCCESSFUL");

    propagate_error (packed, packed_map);

    जर (unlikely (!current)) परत;
    जर (unlikely (in_error()))
    {
      // Offset overflows that occur before link resolution cannot be handled
      // by repacking, so set a more general error.
      जर (offset_overflow ()) err (HB_SERIALIZE_ERROR_OTHER);
      परत;
    }

    प्रतिज्ञा (!current->next);

    /* Only "pack" if there exist other objects... Otherwise, don't bother.
     * Saves a move. */
    जर (packed.length <= 1)
      परत;

    काढा_pack (असत्य);

    resolve_links ();
  }

  ढाँचा <typename Type = शून्य>
  __attribute__((returns_nonnull))
  Type *push ()
  {
    जर (unlikely (in_error ())) परत start_embed<Type> ();

    object_t *obj = object_pool.alloc ();
    जर (unlikely (!obj))
      check_success (असत्य);
    अन्यथा
    {
      obj->head = head;
      obj->tail = tail;
      obj->next = current;
      current = obj;
    }
    परत start_embed<Type> ();
  }
  शून्य काढा_discard ()
  {
    object_t *obj = current;
    जर (unlikely (!obj)) परत;
    // Allow cleanup when we've error'd out on int overflows which don't compromise
    // the serializer state.
    जर (unlikely (in_error() && !only_overflow ())) परत;

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
    जर (unlikely (!obj)) परत 0;
    // Allow cleanup when we've error'd out on int overflows which don't compromise
    // the serializer state.
    जर (unlikely (in_error()  && !only_overflow ())) परत 0;

    current = current->next;
    obj->tail = head;
    obj->next = nullptr;
    प्रतिज्ञा (obj->head <= obj->tail);
    अचिन्हित len = obj->tail - obj->head;
    head = zerocopy ? zerocopy : obj->head; /* Rewind head. */
    तार्किक was_zerocopy = zerocopy;
    zerocopy = nullptr;

    जर (!len)
    {
      प्रतिज्ञा (!obj->real_links.length);
      प्रतिज्ञा (!obj->virtual_links.length);
      परत 0;
    }

    objidx_t objidx;
    uint32_t hash = 0;
    जर (share)
    {
      hash = hb_hash (obj);
      objidx = packed_map.get_with_hash (obj, hash);
      जर (objidx)
      {
        merge_virtual_links (obj, objidx);
	obj->fini ();
        object_pool.release (obj);
	परत objidx;
      }
    }

    tail -= len;
    जर (was_zerocopy)
      प्रतिज्ञा (tail == obj->head);
    अन्यथा
      स_हटाओ (tail, obj->head, len);

    obj->head = tail;
    obj->tail = tail + len;

    packed.push (obj);

    जर (unlikely (!propagate_error (packed)))
    {
      /* Obj wasn't successfully added to packed, so clean it up otherwise its
       * links will be leaked. When we use constructor/destructors properly, we
       * can remove these. */
      obj->fini ();
      परत 0;
    }

    objidx = packed.length - 1;

    जर (share) packed_map.set_with_hash (obj, hash, objidx);
    propagate_error (packed_map);

    परत objidx;
  }

  शून्य revert (snapshot_t snap)
  {
    // Overflows that happened after the snapshot will be erased by the revert.
    जर (unlikely (in_error () && !only_overflow ())) परत;
    प्रतिज्ञा (snap.current == current);
    जर (current)
    {
      current->real_links.shrink (snap.अंक_real_links);
      current->virtual_links.shrink (snap.अंक_virtual_links);
    }
    errors = snap.errors;
    revert (snap.head, snap.tail);
  }

  शून्य revert (अक्षर *snap_head,
	       अक्षर *snap_tail)
  {
    जर (unlikely (in_error ())) परत;
    प्रतिज्ञा (snap_head <= head);
    प्रतिज्ञा (tail <= snap_tail);
    head = snap_head;
    tail = snap_tail;
    discard_stale_objects ();
  }

  शून्य discard_stale_objects ()
  {
    जर (unlikely (in_error ())) परत;
    पर्यंत (packed.length > 1 &&
	   packed.tail ()->head < tail)
    {
      object_t *obj = packed.tail ();
      packed_map.del (obj);
      प्रतिज्ञा (!obj->next);
      obj->fini ();
      object_pool.release (obj);
      packed.काढा ();
    }
    जर (packed.length > 1)
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
    जर (unlikely (in_error ())) परत;

    जर (!objidx)
      परत;

    प्रतिज्ञा (current);

    जर (!current->add_virtual_link(objidx))
      err (HB_SERIALIZE_ERROR_OTHER);
  }

  objidx_t last_added_child_index() स्थिरांक {
    जर (unlikely (in_error ())) परत (objidx_t) -1;

    प्रतिज्ञा (current);
    जर (!तार्किक(current->real_links)) {
      परत (objidx_t) -1;
    }

    परत current->real_links[current->real_links.length - 1].objidx;
  }

  // For the current object ensure that the sub-table bytes for child objidx are always placed
  // after the subtable bytes for any other existing children. This only ensures that the
  // repacker will not move the target subtable before the other children
  // (by adding virtual links). It is up to the caller to ensure the initial serialization
  // order is correct.
  शून्य repack_last(objidx_t objidx) {
    जर (unlikely (in_error ())) परत;

    जर (!objidx)
      परत;

    प्रतिज्ञा (current);
    साठी (स्वतः& l : current->real_links) {
      जर (l.objidx == objidx) {
        सुरूठेवा;
      }

      packed[l.objidx]->add_virtual_link(objidx);
    }
  }

  ढाँचा <typename T>
  शून्य add_link (T &ofs, objidx_t objidx,
		 whence_t whence = Head,
		 अचिन्हित bias = 0)
  {
    जर (unlikely (in_error ())) परत;

    जर (!objidx)
      परत;

    प्रतिज्ञा (current);
    प्रतिज्ञा (current->head <= (स्थिरांक अक्षर *) &ofs);

    स्वतः& link = *current->real_links.push ();
    जर (current->real_links.in_error ())
      err (HB_SERIALIZE_ERROR_OTHER);

    link.width = आकार (T);
    link.objidx = objidx;
    जर (unlikely (!आकार (T)))
    {
      // This link is not associated with an actual offset and exists merely to enforce
      // an ordering constraint.
      link.is_signed = 0;
      link.whence = 0;
      link.position = 0;
      link.bias = 0;
      परत;
    }

    link.is_signed = std::is_signed<hb_unwrap_type (T)>::value;
    link.whence = (अचिन्हित) whence;
    link.position = (स्थिरांक अक्षर *) &ofs - current->head;
    link.bias = bias;
  }

  अचिन्हित to_bias (स्थिरांक शून्य *base) स्थिरांक
  {
    जर (unlikely (in_error ())) परत 0;
    जर (!base) परत 0;
    प्रतिज्ञा (current);
    प्रतिज्ञा (current->head <= (स्थिरांक अक्षर *) base);
    परत (स्थिरांक अक्षर *) base - current->head;
  }

  शून्य resolve_links ()
  {
    जर (unlikely (in_error ())) परत;

    प्रतिज्ञा (!current);
    प्रतिज्ञा (packed.length > 1);

    साठी (स्थिरांक object_t* parent : ++hb_iter (packed))
      साठी (स्थिरांक object_t::link_t &link : parent->real_links)
      {
	स्थिरांक object_t* child = packed[link.objidx];
	जर (unlikely (!child)) { err (HB_SERIALIZE_ERROR_OTHER); परत; }
	अचिन्हित offset = 0;
	निवड ((whence_t) link.whence) {
	स्थिती Head:     offset = child->head - parent->head; थांबा;
	स्थिती Tail:     offset = child->head - parent->tail; थांबा;
	स्थिती Absolute: offset = (head - start) + (child->head - tail); थांबा;
	}

	प्रतिज्ञा (offset >= link.bias);
	offset -= link.bias;
	जर (link.is_signed)
	{
	  प्रतिज्ञा (link.width == 2 || link.width == 4);
	  जर (link.width == 4)
	    assign_offset<int32_t> (parent, link, offset);
	  अन्यथा
	    assign_offset<int16_t> (parent, link, offset);
	}
	अन्यथा
	{
	  प्रतिज्ञा (link.width == 2 || link.width == 3 || link.width == 4);
	  जर (link.width == 4)
	    assign_offset<uint32_t> (parent, link, offset);
	  अन्यथा जर (link.width == 3)
	    assign_offset<uint32_t, 3> (parent, link, offset);
	  अन्यथा
	    assign_offset<uint16_t> (parent, link, offset);
	}
      }
  }

  अचिन्हित पूर्णांक length () स्थिरांक
  {
    जर (unlikely (!current)) परत 0;
    परत हे->head - current->head;
  }

  शून्य align (अचिन्हित पूर्णांक alignment)
  {
    अचिन्हित पूर्णांक l = length () % alignment;
    जर (l)
      (शून्य) allocate_size<शून्य> (alignment - l);
  }

  ढाँचा <typename Type = शून्य>
  __attribute__((returns_nonnull))
  Type *start_embed (स्थिरांक Type *obj HB_UNUSED = nullptr) स्थिरांक
  { परत reinterpret_cast<Type *> (हे->head); }
  ढाँचा <typename Type>
  __attribute__((returns_nonnull))
  Type *start_embed (स्थिरांक Type &obj) स्थिरांक
  { परत start_embed (std::addressof (obj)); }

  तार्किक err (hb_serialize_error_t err_type)
  {
    परत !तार्किक ((errors = (errors | err_type)));
  }

  तार्किक start_zerocopy (माप_प्रकार size)
  {
    जर (unlikely (in_error ())) परत असत्य;

    जर (unlikely (size > पूर्णांक_उच्च || हे->tail - हे->head < सूचक_भेद_प्रकार (size)))
    {
      err (HB_SERIALIZE_ERROR_OUT_OF_ROOM);
      परत असत्य;
    }

    प्रतिज्ञा (!हे->zerocopy);
    हे->zerocopy = हे->head;

    प्रतिज्ञा (हे->current->head == हे->head);
    हे->current->head = हे->current->tail = हे->head = हे->tail - size;
    परत सत्य;
  }

  ढाँचा <typename Type>
  HB_NODISCARD
  Type *allocate_size (माप_प्रकार size, तार्किक clear = सत्य)
  {
    जर (unlikely (in_error ())) परत nullptr;

    जर (unlikely (size > पूर्णांक_उच्च || हे->tail - हे->head < सूचक_भेद_प्रकार (size)))
    {
      err (HB_SERIALIZE_ERROR_OUT_OF_ROOM);
      परत nullptr;
    }
    जर (clear)
      hb_memset (हे->head, 0, size);
    अक्षर *ret = हे->head;
    हे->head += size;
    परत reinterpret_cast<Type *> (ret);
  }

  ढाँचा <typename Type>
  Type *allocate_min ()
  { परत हे->allocate_size<Type> (Type::min_size); }

  ढाँचा <typename Type>
  HB_NODISCARD
  Type *embed (स्थिरांक Type *obj)
  {
    माप_प्रकार size = obj->get_size ();
    Type *ret = हे->allocate_size<Type> (size, असत्य);
    जर (unlikely (!ret)) परत nullptr;
    hb_memcpy (ret, obj, size);
    परत ret;
  }
  ढाँचा <typename Type>
  HB_NODISCARD
  Type *embed (स्थिरांक Type &obj)
  { परत embed (std::addressof (obj)); }
  अक्षर *embed (स्थिरांक अक्षर *obj, अचिन्हित size)
  {
    अक्षर *ret = हे->allocate_size<अक्षर> (size, असत्य);
    जर (unlikely (!ret)) परत nullptr;
    hb_memcpy (ret, obj, size);
    परत ret;
  }

  ढाँचा <typename Type, typename ...Ts> स्वतः
  _copy (स्थिरांक Type &src, hb_priority<1>, Ts&&... ds) HB_RETURN
  (Type *, src.copy (हे, std::forward<Ts> (ds)...))

  ढाँचा <typename Type> स्वतः
  _copy (स्थिरांक Type &src, hb_priority<0>) -> decltype (&(hb_declval<Type> () = src))
  {
    Type *ret = हे->allocate_size<Type> (आकार (Type));
    जर (unlikely (!ret)) परत nullptr;
    *ret = src;
    परत ret;
  }

  /* Like embed, but active: calls obj.operator=() or obj.copy() to transfer data
   * instead of hb_memcpy(). */
  ढाँचा <typename Type, typename ...Ts>
  Type *copy (स्थिरांक Type &src, Ts&&... ds)
  { परत _copy (src, hb_prioritize, std::forward<Ts> (ds)...); }
  ढाँचा <typename Type, typename ...Ts>
  Type *copy (स्थिरांक Type *src, Ts&&... ds)
  { परत copy (*src, std::forward<Ts> (ds)...); }

  ढाँचा<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator)),
	   typename ...Ts>
  शून्य copy_all (Iterator it, Ts&&... ds)
  { साठी (decltype (*it) _ : it) copy (_, ds...); }

  ढाँचा <typename Type>
  hb_serialize_context_t& चालक << (स्थिरांक Type &obj) & { embed (obj); परत *हे; }

  ढाँचा <typename Type>
  Type *extend_size (Type *obj, माप_प्रकार size, तार्किक clear = सत्य)
  {
    जर (unlikely (in_error ())) परत nullptr;
    जर (unlikely (size >= पूर्णांक_उच्च)) { err (HB_SERIALIZE_ERROR_OTHER); परत nullptr; }

    प्रतिज्ञा (हे->start <= (अक्षर *) obj);
    प्रतिज्ञा ((अक्षर *) obj <= हे->head);
    प्रतिज्ञा ((माप_प्रकार) (हे->head - (अक्षर *) obj) <= size);
    जर (unlikely (((अक्षर *) obj + size < (अक्षर *) obj) ||
		  !हे->allocate_size<Type> (((अक्षर *) obj) + size - हे->head, clear))) परत nullptr;
    परत reinterpret_cast<Type *> (obj);
  }
  ढाँचा <typename Type>
  Type *extend_size (Type &obj, माप_प्रकार size, तार्किक clear = सत्य)
  { परत extend_size (std::addressof (obj), size, clear); }

  ढाँचा <typename Type>
  Type *extend_min (Type *obj) { परत extend_size (obj, obj->min_size); }
  ढाँचा <typename Type>
  Type *extend_min (Type &obj) { परत extend_min (std::addressof (obj)); }

  ढाँचा <typename Type, typename ...Ts>
  Type *extend (Type *obj, Ts&&... ds)
  { परत extend_size (obj, obj->get_size (std::forward<Ts> (ds)...)); }
  ढाँचा <typename Type, typename ...Ts>
  Type *extend (Type &obj, Ts&&... ds)
  { परत extend (std::addressof (obj), std::forward<Ts> (ds)...); }

  /* Output routines. */
  hb_bytes_t copy_bytes () स्थिरांक
  {
    प्रतिज्ञा (successful ());
    /* Copy both items from head side and tail side... */
    अचिन्हित पूर्णांक len = (हे->head - हे->start)
		     + (हे->end  - हे->tail);

    // If len is zero don't hb_malloc as the memory won't get properly
    // cleaned up later.
    जर (!len) परत hb_bytes_t ();

    अक्षर *p = (अक्षर *) hb_malloc (len);
    जर (unlikely (!p)) परत hb_bytes_t ();

    hb_memcpy (p, हे->start, हे->head - हे->start);
    hb_memcpy (p + (हे->head - हे->start), हे->tail, हे->end - हे->tail);
    परत hb_bytes_t (p, len);
  }
  ढाँचा <typename Type>
  Type *copy () स्थिरांक
  { परत reinterpret_cast<Type *> ((अक्षर *) copy_bytes ().arrayZ); }
  hb_blob_t *copy_blob () स्थिरांक
  {
    hb_bytes_t b = copy_bytes ();
    परत hb_blob_create (b.arrayZ, b.length,
			   HB_MEMORY_MODE_WRITABLE,
			   (अक्षर *) b.arrayZ, hb_free);
  }

  स्थिरांक hb_vector_t<object_t *>& object_graph() स्थिरांक
  { परत packed; }

  खाजगी:
  ढाँचा <typename T, अचिन्हित Size = आकार (T)>
  शून्य assign_offset (स्थिरांक object_t* parent, स्थिरांक object_t::link_t &link, अचिन्हित offset)
  {
    // XXX We should stop assuming big-endian!
    स्वतः &off = * ((HBInt<सत्य, T, Size> *) (parent->head + link.position));
    प्रतिज्ञा (0 == off);
    check_assign (off, offset, HB_SERIALIZE_ERROR_OFFSET_OVERFLOW);
  }

  सार्वजनिक:
  अक्षर *start, *head, *tail, *end, *zerocopy;
  अचिन्हित पूर्णांक debug_depth;
  hb_serialize_error_t errors;

  खाजगी:

  शून्य merge_virtual_links (स्थिरांक object_t* from, objidx_t to_idx) {
    object_t* to = packed[to_idx];
    साठी (स्थिरांक स्वतः& l : from->virtual_links) {
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
  hb_hashmap_t<स्थिरांक object_t *, objidx_t> packed_map;
};

#endif /* HB_SERIALIZE_HH */
