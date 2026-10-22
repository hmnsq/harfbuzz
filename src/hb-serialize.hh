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

#अगर_अघोषित HB_SERIALIZE_HH
#घोषणा HB_SERIALIZE_HH

#समावेश "hb.hh"
#समावेश "hb-blob.hh"
#समावेश "hb-map.hh"
#समावेश "hb-free-pool.hh"

#समावेश "hb-subset-serialize.h"

/*
 * Serialize
 */

क्रमागत hb_serialize_error_t {
  HB_SERIALIZE_ERROR_NONE =            0x00000000u,
  HB_SERIALIZE_ERROR_OTHER =           0x00000001u,
  HB_SERIALIZE_ERROR_OFFSET_OVERFLOW = 0x00000002u,
  HB_SERIALIZE_ERROR_OUT_OF_ROOM =     0x00000004u,
  HB_SERIALIZE_ERROR_INT_OVERFLOW =    0x00000008u,
  HB_SERIALIZE_ERROR_ARRAY_OVERFLOW =  0x00000010u
};
HB_MARK_AS_FLAG_T (hb_serialize_error_t);

काष्ठा hb_serialize_context_t
{
  प्रकार अचिन्हित objidx_t;

  क्रमागत whence_t {
     शीर्ष,	/* Relative to the current object head (default). */
     पुच्छ_2,	/* Relative to the current object tail after packed. */
     Absolute	/* Absolute: from the start of the serialize buffer. */
   };



  काष्ठा object_t
  {
    व्योम fini () {
      real_links.fini ();
      virtual_links.fini ();
    }

    object_t () = शेष;

    object_t (स्थिर hb_subset_serialize_object_t &o)
    {
      शीर्ष_2 = o.शीर्ष_2;
      पुच्छ = o.पुच्छ;
      next = nullptr;
      real_links.alloc_exact (o.अंक_real_links);
      क्रम (अचिन्हित i = 0 ; i < o.अंक_real_links; i++)
        real_links.push (o.real_links[i]);

      virtual_links.alloc_exact (o.अंक_virtual_links);
      क्रम (अचिन्हित i = 0; i < o.अंक_virtual_links; i++)
        virtual_links.push (o.virtual_links[i]);
    }

    bool in_error () स्थिर
    {
      वापस real_links.in_error() || virtual_links.in_error();
    }

    bool add_virtual_link (objidx_t objidx)
    {
      अगर (!objidx)
        वापस false;

      स्वतः& link = *virtual_links.push ();
      अगर (virtual_links.in_error ())
        वापस false;

      link.objidx = objidx;
      // Remaining fields were previously zero'd by push():
      // link.width = 0;
      // link.is_signed = 0;
      // link.whence = 0;
      // link.position = 0;
      // link.bias = 0;

      वापस true;
    }

    मित्र व्योम swap (object_t& a, object_t& b) noexcept
    {
      hb_swap (a.शीर्ष_2, b.शीर्ष_2);
      hb_swap (a.पुच्छ, b.पुच्छ);
      hb_swap (a.next, b.next);
      hb_swap (a.real_links, b.real_links);
      hb_swap (a.virtual_links, b.virtual_links);
    }

    bool चालक == (स्थिर object_t &o) स्थिर
    {
      // Virtual links aren't considered for equality since they don't affect the functionality
      // of the object.
      वापस (पुच्छ - शीर्ष_2 == o.पुच्छ - o.शीर्ष_2)
	  && (real_links.लंबाई == o.real_links.लंबाई)
	  && 0 == hb_memcmp (शीर्ष_2, o.शीर्ष_2, पुच्छ - शीर्ष_2)
	  && real_links.as_bytes () == o.real_links.as_bytes ();
    }
    uint32_t hash () स्थिर
    {
      // Virtual links aren't considered for equality since they don't affect the functionality
      // of the object.
      वापस hb_bytes_t (शीर्ष_2, hb_min (128, पुच्छ - शीर्ष_2)).hash () ^
          real_links.as_bytes ().hash ();
    }

    काष्ठा link_t
    {
      अचिन्हित width: 3;
      अचिन्हित is_signed: 1;
      अचिन्हित whence: 2;
      अचिन्हित bias : 26;
      अचिन्हित position;
      objidx_t objidx;

      link_t () = शेष;

      link_t (स्थिर hb_subset_serialize_link_t &o)
      {
        width = o.width;
        is_signed = 0;
        whence = 0;
        position = o.position;
        bias = 0;
        objidx = o.objidx;
      }

      HB_INTERNAL अटल पूर्णांक cmp (स्थिर व्योम* a, स्थिर व्योम* b)
      {
        पूर्णांक cmp = ((स्थिर link_t*)a)->position - ((स्थिर link_t*)b)->position;
        अगर (cmp) वापस cmp;

        वापस ((स्थिर link_t*)a)->objidx - ((स्थिर link_t*)b)->objidx;
      }
    };

    अक्षर *शीर्ष_2;
    अक्षर *पुच्छ;
    hb_vector_t<link_t> real_links;
    hb_vector_t<link_t> virtual_links;
    object_t *next;

    स्वतः all_links () स्थिर HB_AUTO_RETURN
        (( hb_concat (real_links, virtual_links) ));
    स्वतः all_links_writer () HB_AUTO_RETURN
        (( hb_concat (real_links.writer (), virtual_links.writer ()) ));
  };

  काष्ठा snapshot_t
  {
    अक्षर *शीर्ष_2;
    अक्षर *पुच्छ;
    object_t *current; // Just for sanity check
    अचिन्हित अंक_real_links;
    अचिन्हित अंक_virtual_links;
    hb_serialize_error_t errors;
  };

  snapshot_t snapshot ()
  {
    वापस snapshot_t {
      शीर्ष_2, पुच्छ, current,
      current ? current->real_links.लंबाई : 0,
      current ? current->virtual_links.लंबाई : 0,
      errors
     };
  }

  hb_serialize_context_t (व्योम *start_, माप_प्रकार आकार) :
    start ((अक्षर *) start_),
    end (start + आकार),
    current (nullptr)
  { reset (); }
  ~hb_serialize_context_t () { fini (); }

  व्योम fini ()
  {
    क्रम (object_t *_ : ++hb_iter (packed)) _->fini ();
    packed.fini ();
    यह->packed_map.fini ();

    जबतक (current)
    {
      स्वतः *_ = current;
      current = current->next;
      _->fini ();
    }
  }

  bool in_error () स्थिर { वापस bool (errors); }

  bool successful () स्थिर { वापस !bool (errors); }

  HB_NODISCARD bool ran_out_of_room () स्थिर { वापस errors & HB_SERIALIZE_ERROR_OUT_OF_ROOM; }
  HB_NODISCARD bool offset_overflow () स्थिर { वापस errors & HB_SERIALIZE_ERROR_OFFSET_OVERFLOW; }
  HB_NODISCARD bool only_offset_overflow () स्थिर { वापस errors == HB_SERIALIZE_ERROR_OFFSET_OVERFLOW; }
  HB_NODISCARD bool only_overflow () स्थिर
  {
    वापस errors == HB_SERIALIZE_ERROR_OFFSET_OVERFLOW
        || errors == HB_SERIALIZE_ERROR_INT_OVERFLOW
        || errors == HB_SERIALIZE_ERROR_ARRAY_OVERFLOW;
  }

  व्योम reset (व्योम *start_, माप_प्रकार आकार)
  {
    start = (अक्षर*) start_;
    end = start + आकार;
    reset ();
    current = nullptr;
  }

  व्योम reset ()
  {
    यह->errors = HB_SERIALIZE_ERROR_NONE;
    यह->शीर्ष_2 = यह->start;
    यह->पुच्छ = यह->end;
    यह->zerocopy = nullptr;
    यह->debug_depth = 0;

    fini ();
    यह->packed.push (nullptr);
    यह->packed_map.init ();
  }

  bool check_success (bool success,
                      hb_serialize_error_t err_type = HB_SERIALIZE_ERROR_OTHER)
  {
    वापस successful ()
        && (success || err (err_type));
  }

  ढाँचा <typename T1, typename T2>
  bool check_equal (T1 &&v1, T2 &&v2, hb_serialize_error_t err_type)
  {
    अगर ((दीर्घ दीर्घ) v1 != (दीर्घ दीर्घ) v2)
    {
      वापस err (err_type);
    }
    वापस true;
  }

  ढाँचा <typename T1, typename T2>
  bool check_assign (T1 &v1, T2 &&v2, hb_serialize_error_t err_type)
  { वापस check_equal (v1 = v2, v2, err_type); }

  ढाँचा <typename T> bool propagate_error (T &&obj)
  { वापस check_success (!hb_deref (obj).in_error ()); }

  ढाँचा <typename T1, typename... Ts> bool propagate_error (T1 &&o1, Ts&&... os)
  { वापस propagate_error (std::forward<T1> (o1)) &&
	   propagate_error (std::forward<Ts> (os)...); }

  /* To be called around main operation. */
  ढाँचा <typename Type=अक्षर>
  __attribute__((returns_nonnull))
  Type *start_serialize ()
  {
    DEBUG_संदेश_LEVEL (SERIALIZE, यह->start, 0, +1,
		     "start [%p..%p] (%lu bytes)",
		     यह->start, यह->end,
		     (अचिन्हित दीर्घ) (यह->end - यह->start));

    निश्चित (!current);
    वापस push<Type> ();
  }
  व्योम end_serialize ()
  {
    DEBUG_संदेश_LEVEL (SERIALIZE, यह->start, 0, -1,
		     "end [%p..%p] serialized %u bytes; %s",
		     यह->start, यह->end,
		     (अचिन्हित) (यह->शीर्ष_2 - यह->start),
		     successful () ? "successful" : "UNSUCCESSFUL");

    propagate_error (packed, packed_map);

    अगर (unlikely (!current)) वापस;
    अगर (unlikely (in_error()))
    {
      // Offset overflows that occur before link resolution cannot be handled
      // by repacking, so set a more general error.
      अगर (offset_overflow ()) err (HB_SERIALIZE_ERROR_OTHER);
      वापस;
    }

    निश्चित (!current->next);

    /* Only "pack" if there exist other objects... Otherwise, don't bother.
     * Saves a move. */
    अगर (packed.लंबाई <= 1)
      वापस;

    निकालो_pack (false);

    resolve_links ();
  }

  ढाँचा <typename Type = व्योम>
  __attribute__((returns_nonnull))
  Type *push ()
  {
    अगर (unlikely (in_error ())) वापस start_embed<Type> ();

    object_t *obj = object_pool.alloc ();
    अगर (unlikely (!obj))
      check_success (false);
    अन्यथा
    {
      obj->शीर्ष_2 = शीर्ष_2;
      obj->पुच्छ = पुच्छ;
      obj->next = current;
      current = obj;
    }
    वापस start_embed<Type> ();
  }
  व्योम निकालो_discard ()
  {
    object_t *obj = current;
    अगर (unlikely (!obj)) वापस;
    // Allow cleanup when we've error'd out on int overflows which don't compromise
    // the serializer state.
    अगर (unlikely (in_error() && !only_overflow ())) वापस;

    current = current->next;
    revert (zerocopy ? zerocopy : obj->शीर्ष_2, obj->पुच्छ);
    zerocopy = nullptr;
    obj->fini ();
    object_pool.release (obj);
  }

  /* Set share to false when an object is unlikely shareable with others
   * so not worth an attempt, or a contiguous table is serialized as
   * multiple consecutive objects in the reverse order so can't be shared.
   */
  objidx_t निकालो_pack (bool share=true)
  {
    object_t *obj = current;
    अगर (unlikely (!obj)) वापस 0;
    // Allow cleanup when we've error'd out on int overflows which don't compromise
    // the serializer state.
    अगर (unlikely (in_error()  && !only_overflow ())) वापस 0;

    current = current->next;
    obj->पुच्छ = शीर्ष_2;
    obj->next = nullptr;
    निश्चित (obj->शीर्ष_2 <= obj->पुच्छ);
    अचिन्हित len = obj->पुच्छ - obj->शीर्ष_2;
    शीर्ष_2 = zerocopy ? zerocopy : obj->शीर्ष_2; /* Rewind head. */
    bool was_zerocopy = zerocopy;
    zerocopy = nullptr;

    अगर (!len)
    {
      निश्चित (!obj->real_links.लंबाई);
      निश्चित (!obj->virtual_links.लंबाई);
      वापस 0;
    }

    objidx_t objidx;
    uint32_t hash = 0;
    अगर (share)
    {
      hash = hb_hash (obj);
      objidx = packed_map.get_with_hash (obj, hash);
      अगर (objidx)
      {
        merge_virtual_links (obj, objidx);
	obj->fini ();
        object_pool.release (obj);
	वापस objidx;
      }
    }

    पुच्छ -= len;
    अगर (was_zerocopy)
      निश्चित (पुच्छ == obj->शीर्ष_2);
    अन्यथा
      स_हटाओ (पुच्छ, obj->शीर्ष_2, len);

    obj->शीर्ष_2 = पुच्छ;
    obj->पुच्छ = पुच्छ + len;

    packed.push (obj);

    अगर (unlikely (!propagate_error (packed)))
    {
      /* Obj wasn't successfully added to packed, so clean it up otherwise its
       * links will be leaked. When we use constructor/destructors properly, we
       * can remove these. */
      obj->fini ();
      वापस 0;
    }

    objidx = packed.लंबाई - 1;

    अगर (share) packed_map.set_with_hash (obj, hash, objidx);
    propagate_error (packed_map);

    वापस objidx;
  }

  व्योम revert (snapshot_t snap)
  {
    // Overflows that happened after the snapshot will be erased by the revert.
    अगर (unlikely (in_error () && !only_overflow ())) वापस;
    निश्चित (snap.current == current);
    अगर (current)
    {
      current->real_links.shrink (snap.अंक_real_links);
      current->virtual_links.shrink (snap.अंक_virtual_links);
    }
    errors = snap.errors;
    revert (snap.शीर्ष_2, snap.पुच्छ);
  }

  व्योम revert (अक्षर *snap_शीर्ष,
	       अक्षर *snap_पुच्छ)
  {
    अगर (unlikely (in_error ())) वापस;
    निश्चित (snap_शीर्ष <= शीर्ष_2);
    निश्चित (पुच्छ <= snap_पुच्छ);
    शीर्ष_2 = snap_शीर्ष;
    पुच्छ = snap_पुच्छ;
    discard_stale_objects ();
  }

  व्योम discard_stale_objects ()
  {
    अगर (unlikely (in_error ())) वापस;
    जबतक (packed.लंबाई > 1 &&
	   packed.पुच्छ ()->शीर्ष_2 < पुच्छ)
    {
      object_t *obj = packed.पुच्छ ();
      packed_map.del (obj);
      निश्चित (!obj->next);
      obj->fini ();
      object_pool.release (obj);
      packed.निकालो ();
    }
    अगर (packed.लंबाई > 1)
      निश्चित (packed.पुच्छ ()->शीर्ष_2 == पुच्छ);
  }

  // Adds a virtual link from the current object to objidx. A virtual link is not associated with
  // an actual offset field. They are solely used to enforce ordering constraints between objects.
  // Adding a virtual link from object a to object b will ensure that object b is always packed after
  // object a in the final serialized order.
  //
  // This is useful in certain situations where there needs to be a specific ordering in the
  // final serialization. Such as when platform bugs require certain orderings, or to provide
  //  guidance to the repacker for better offset overflow resolution.
  व्योम add_virtual_link (objidx_t objidx)
  {
    अगर (unlikely (in_error ())) वापस;

    अगर (!objidx)
      वापस;

    निश्चित (current);

    अगर (!current->add_virtual_link(objidx))
      err (HB_SERIALIZE_ERROR_OTHER);
  }

  objidx_t last_added_child_सूचकांक() स्थिर {
    अगर (unlikely (in_error ())) वापस (objidx_t) -1;

    निश्चित (current);
    अगर (!bool(current->real_links)) {
      वापस (objidx_t) -1;
    }

    वापस current->real_links[current->real_links.लंबाई - 1].objidx;
  }

  // For the current object ensure that the sub-table bytes for child objidx are always placed
  // after the subtable bytes for any other existing children. This only ensures that the
  // repacker will not move the target subtable before the other children
  // (by adding virtual links). It is up to the caller to ensure the initial serialization
  // order is correct.
  व्योम repack_last(objidx_t objidx) {
    अगर (unlikely (in_error ())) वापस;

    अगर (!objidx)
      वापस;

    निश्चित (current);
    क्रम (स्वतः& l : current->real_links) {
      अगर (l.objidx == objidx) {
        जारी;
      }

      packed[l.objidx]->add_virtual_link(objidx);
    }
  }

  ढाँचा <typename T>
  व्योम add_link (T &ofs, objidx_t objidx,
		 whence_t whence = शीर्ष,
		 अचिन्हित bias = 0)
  {
    अगर (unlikely (in_error ())) वापस;

    अगर (!objidx)
      वापस;

    निश्चित (current);
    निश्चित (current->शीर्ष_2 <= (स्थिर अक्षर *) &ofs);

    स्वतः& link = *current->real_links.push ();
    अगर (current->real_links.in_error ())
      err (HB_SERIALIZE_ERROR_OTHER);

    link.width = माप (T);
    link.objidx = objidx;
    अगर (unlikely (!माप (T)))
    {
      // This link is not associated with an actual offset and exists merely to enforce
      // an ordering constraint.
      link.is_signed = 0;
      link.whence = 0;
      link.position = 0;
      link.bias = 0;
      वापस;
    }

    link.is_signed = std::is_signed<hb_unwrap_type (T)>::value;
    link.whence = (अचिन्हित) whence;
    link.position = (स्थिर अक्षर *) &ofs - current->शीर्ष_2;
    link.bias = bias;
  }

  अचिन्हित to_bias (स्थिर व्योम *base) स्थिर
  {
    अगर (unlikely (in_error ())) वापस 0;
    अगर (!base) वापस 0;
    निश्चित (current);
    निश्चित (current->शीर्ष_2 <= (स्थिर अक्षर *) base);
    वापस (स्थिर अक्षर *) base - current->शीर्ष_2;
  }

  व्योम resolve_links ()
  {
    अगर (unlikely (in_error ())) वापस;

    निश्चित (!current);
    निश्चित (packed.लंबाई > 1);

    क्रम (स्थिर object_t* parent : ++hb_iter (packed))
      क्रम (स्थिर object_t::link_t &link : parent->real_links)
      {
	स्थिर object_t* child = packed[link.objidx];
	अगर (unlikely (!child)) { err (HB_SERIALIZE_ERROR_OTHER); वापस; }
	अचिन्हित offset = 0;
	चयन ((whence_t) link.whence) {
	हाल शीर्ष:     offset = child->शीर्ष_2 - parent->शीर्ष_2; अवरोध;
	हाल पुच्छ_2:     offset = child->शीर्ष_2 - parent->पुच्छ; अवरोध;
	हाल Absolute: offset = (शीर्ष_2 - start) + (child->शीर्ष_2 - पुच्छ); अवरोध;
	}

	निश्चित (offset >= link.bias);
	offset -= link.bias;
	अगर (link.is_signed)
	{
	  निश्चित (link.width == 2 || link.width == 4);
	  अगर (link.width == 4)
	    assign_offset<int32_t> (parent, link, offset);
	  अन्यथा
	    assign_offset<int16_t> (parent, link, offset);
	}
	अन्यथा
	{
	  निश्चित (link.width == 2 || link.width == 3 || link.width == 4);
	  अगर (link.width == 4)
	    assign_offset<uint32_t> (parent, link, offset);
	  अन्यथा अगर (link.width == 3)
	    assign_offset<uint32_t, 3> (parent, link, offset);
	  अन्यथा
	    assign_offset<uint16_t> (parent, link, offset);
	}
      }
  }

  अचिन्हित पूर्णांक लंबाई () स्थिर
  {
    अगर (unlikely (!current)) वापस 0;
    वापस यह->शीर्ष_2 - current->शीर्ष_2;
  }

  व्योम align (अचिन्हित पूर्णांक alignment)
  {
    अचिन्हित पूर्णांक l = लंबाई () % alignment;
    अगर (l)
      (व्योम) allocate_आकार<व्योम> (alignment - l);
  }

  ढाँचा <typename Type = व्योम>
  __attribute__((returns_nonnull))
  Type *start_embed (स्थिर Type *obj HB_UNUSED = nullptr) स्थिर
  { वापस reinterpret_cast<Type *> (यह->शीर्ष_2); }
  ढाँचा <typename Type>
  __attribute__((returns_nonnull))
  Type *start_embed (स्थिर Type &obj) स्थिर
  { वापस start_embed (std::addressof (obj)); }

  bool err (hb_serialize_error_t err_type)
  {
    वापस !bool ((errors = (errors | err_type)));
  }

  bool start_zerocopy (माप_प्रकार आकार)
  {
    अगर (unlikely (in_error ())) वापस false;

    अगर (unlikely (आकार > पूर्णांक_उच्च || यह->पुच्छ - यह->शीर्ष_2 < सूचक_भेद_प्रकार (आकार)))
    {
      err (HB_SERIALIZE_ERROR_OUT_OF_ROOM);
      वापस false;
    }

    निश्चित (!यह->zerocopy);
    यह->zerocopy = यह->शीर्ष_2;

    निश्चित (यह->current->शीर्ष_2 == यह->शीर्ष_2);
    यह->current->शीर्ष_2 = यह->current->पुच्छ = यह->शीर्ष_2 = यह->पुच्छ - आकार;
    वापस true;
  }

  ढाँचा <typename Type>
  HB_NODISCARD
  Type *allocate_आकार (माप_प्रकार आकार, bool सफा = true)
  {
    अगर (unlikely (in_error ())) वापस nullptr;

    अगर (unlikely (आकार > पूर्णांक_उच्च || यह->पुच्छ - यह->शीर्ष_2 < सूचक_भेद_प्रकार (आकार)))
    {
      err (HB_SERIALIZE_ERROR_OUT_OF_ROOM);
      वापस nullptr;
    }
    अगर (सफा)
      hb_memset (यह->शीर्ष_2, 0, आकार);
    अक्षर *ret = यह->शीर्ष_2;
    यह->शीर्ष_2 += आकार;
    वापस reinterpret_cast<Type *> (ret);
  }

  ढाँचा <typename Type>
  Type *allocate_min ()
  { वापस यह->allocate_आकार<Type> (Type::min_आकार); }

  ढाँचा <typename Type>
  HB_NODISCARD
  Type *embed (स्थिर Type *obj)
  {
    माप_प्रकार आकार = obj->get_आकार ();
    Type *ret = यह->allocate_आकार<Type> (आकार, false);
    अगर (unlikely (!ret)) वापस nullptr;
    hb_memcpy (ret, obj, आकार);
    वापस ret;
  }
  ढाँचा <typename Type>
  HB_NODISCARD
  Type *embed (स्थिर Type &obj)
  { वापस embed (std::addressof (obj)); }
  अक्षर *embed (स्थिर अक्षर *obj, अचिन्हित आकार)
  {
    अक्षर *ret = यह->allocate_आकार<अक्षर> (आकार, false);
    अगर (unlikely (!ret)) वापस nullptr;
    hb_memcpy (ret, obj, आकार);
    वापस ret;
  }

  ढाँचा <typename Type, typename ...Ts> स्वतः
  _copy (स्थिर Type &src, hb_priority<1>, Ts&&... ds) HB_RETURN
  (Type *, src.copy (यह, std::forward<Ts> (ds)...))

  ढाँचा <typename Type> स्वतः
  _copy (स्थिर Type &src, hb_priority<0>) -> decltype (&(hb_declval<Type> () = src))
  {
    Type *ret = यह->allocate_आकार<Type> (माप (Type));
    अगर (unlikely (!ret)) वापस nullptr;
    *ret = src;
    वापस ret;
  }

  /* Like embed, but active: calls obj.operator=() or obj.copy() to transfer data
   * instead of hb_memcpy(). */
  ढाँचा <typename Type, typename ...Ts>
  Type *copy (स्थिर Type &src, Ts&&... ds)
  { वापस _copy (src, hb_prioritize, std::forward<Ts> (ds)...); }
  ढाँचा <typename Type, typename ...Ts>
  Type *copy (स्थिर Type *src, Ts&&... ds)
  { वापस copy (*src, std::forward<Ts> (ds)...); }

  ढाँचा<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator)),
	   typename ...Ts>
  व्योम copy_all (Iterator it, Ts&&... ds)
  { क्रम (decltype (*it) _ : it) copy (_, ds...); }

  ढाँचा <typename Type>
  hb_serialize_context_t& चालक << (स्थिर Type &obj) & { embed (obj); वापस *यह; }

  ढाँचा <typename Type>
  Type *extend_आकार (Type *obj, माप_प्रकार आकार, bool सफा = true)
  {
    अगर (unlikely (in_error ())) वापस nullptr;
    अगर (unlikely (आकार >= पूर्णांक_उच्च)) { err (HB_SERIALIZE_ERROR_OTHER); वापस nullptr; }

    निश्चित (यह->start <= (अक्षर *) obj);
    निश्चित ((अक्षर *) obj <= यह->शीर्ष_2);
    निश्चित ((माप_प्रकार) (यह->शीर्ष_2 - (अक्षर *) obj) <= आकार);
    अगर (unlikely (((अक्षर *) obj + आकार < (अक्षर *) obj) ||
		  !यह->allocate_आकार<Type> (((अक्षर *) obj) + आकार - यह->शीर्ष_2, सफा))) वापस nullptr;
    वापस reinterpret_cast<Type *> (obj);
  }
  ढाँचा <typename Type>
  Type *extend_आकार (Type &obj, माप_प्रकार आकार, bool सफा = true)
  { वापस extend_आकार (std::addressof (obj), आकार, सफा); }

  ढाँचा <typename Type>
  Type *extend_min (Type *obj) { वापस extend_आकार (obj, obj->min_आकार); }
  ढाँचा <typename Type>
  Type *extend_min (Type &obj) { वापस extend_min (std::addressof (obj)); }

  ढाँचा <typename Type, typename ...Ts>
  Type *extend (Type *obj, Ts&&... ds)
  { वापस extend_आकार (obj, obj->get_आकार (std::forward<Ts> (ds)...)); }
  ढाँचा <typename Type, typename ...Ts>
  Type *extend (Type &obj, Ts&&... ds)
  { वापस extend (std::addressof (obj), std::forward<Ts> (ds)...); }

  /* Output routines. */
  hb_bytes_t copy_bytes () स्थिर
  {
    निश्चित (successful ());
    /* Copy both items from head side and tail side... */
    अचिन्हित पूर्णांक len = (यह->शीर्ष_2 - यह->start)
		     + (यह->end  - यह->पुच्छ);

    // If len is zero don't hb_malloc as the memory won't get properly
    // cleaned up later.
    अगर (!len) वापस hb_bytes_t ();

    अक्षर *p = (अक्षर *) hb_malloc (len);
    अगर (unlikely (!p)) वापस hb_bytes_t ();

    hb_memcpy (p, यह->start, यह->शीर्ष_2 - यह->start);
    hb_memcpy (p + (यह->शीर्ष_2 - यह->start), यह->पुच्छ, यह->end - यह->पुच्छ);
    वापस hb_bytes_t (p, len);
  }
  ढाँचा <typename Type>
  Type *copy () स्थिर
  { वापस reinterpret_cast<Type *> ((अक्षर *) copy_bytes ().arrayZ); }
  hb_blob_t *copy_blob () स्थिर
  {
    hb_bytes_t b = copy_bytes ();
    वापस hb_blob_create (b.arrayZ, b.लंबाई,
			   HB_MEMORY_MODE_WRITABLE,
			   (अक्षर *) b.arrayZ, hb_free);
  }

  स्थिर hb_vector_t<object_t *>& object_graph() स्थिर
  { वापस packed; }

  निजी:
  ढाँचा <typename T, अचिन्हित आकार_2 = माप (T)>
  व्योम assign_offset (स्थिर object_t* parent, स्थिर object_t::link_t &link, अचिन्हित offset)
  {
    // XXX We should stop assuming big-endian!
    स्वतः &off = * ((HBInt<true, T, आकार_2> *) (parent->शीर्ष_2 + link.position));
    निश्चित (0 == off);
    check_assign (off, offset, HB_SERIALIZE_ERROR_OFFSET_OVERFLOW);
  }

  खुला:
  अक्षर *start, *शीर्ष_2, *पुच्छ, *end, *zerocopy;
  अचिन्हित पूर्णांक debug_depth;
  hb_serialize_error_t errors;

  निजी:

  व्योम merge_virtual_links (स्थिर object_t* from, objidx_t to_idx) {
    object_t* to = packed[to_idx];
    क्रम (स्थिर स्वतः& l : from->virtual_links) {
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
  hb_hashmap_t<स्थिर object_t *, objidx_t> packed_map;
};

#पूर्ण_अगर /* HB_SERIALIZE_HH */
