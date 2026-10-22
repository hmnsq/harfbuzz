/*
 * Copyright © 2022  Google, Inc.
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
 * Google Author(s): Garret Rieger
 */

#ضمن "../hb-set.hh"
#ضمن "../hb-priority-queue.hh"
#ضمن "../hb-serialize.hh"
#ضمن "graph-result.hh"

#اگر_تعریف_نشده گراف_گراف_HH
#تعریف_کن گراف_گراف_HH

#تعریف_کن HB_گراف_INVALID ((بیعلامت) -1)

نطاق گراف {

/**
 * Represents a serialized table in the form of a graph.
 * Provides methods for modifying and reordering the graph.
 */
بنية گراف_t
{
  // TODO(garretrieger): experiment with using scratch vector for visited sets instead of allocating
  // a new hb_set_t each time.

  بنية رأس_t
  {
    // TODO(garretrieger): have vertex_t gaurantee that head/tail are non null and head < tail
    // so downstream users don't need to constantly check that.

    // TODO(garretrieger): have vertex_t gaurantee that all link indexes are valid (ie < vertices_.length)
    // so downstream users don't need to constantly check that.

    // TODO(garretrieger): track where the real links are currently sorted, and automatically sort
    // prior to accessing iterator for real links. Avoids needing to manually trigger sorting by
    // downstream users.

    int64_t distance = 0 ;
    بیعلامت space = 0 ;
    بیعلامت شروع = 0;
    بیعلامت انتها = 0;
    بیعلامت اولویت = 0;

    خاص:
    hb_serialize_context_t::object_t obj_;
    بیعلامت incoming_یالها = 0;
    بیعلامت single_والد = HB_گراف_INVALID;
    منطقي has_incoming_virtual_یالها = خطأ;
    hb_hashmap_t<بیعلامت, بیعلامت> parents;
    hb_مجموعه_t virtual_parents;
    عام:

    رأس_t ()
    {}

    صریح رأس_t(ثابت hb_serialize_context_t::object_t& obj) : obj_(obj)
    {}

    خودکار parents_iter () ثابت HB_AUTO_RETURN
    (
      hb_concat (
	hb_iter (&single_والد, single_والد != HB_گراف_INVALID),
	parents.کلیدها_ref ()
      )
    )

    فراغ مجموعه_میانگیر (حرف* سر_2, حرف* دم) {
      obj_.سر_2 = سر_2;
      obj_.دم = دم;
    }

    فراغ shrink_میانگیر (size_t شمار)
    {
      obj_.دم -= شمار;
    }

    خودکار all_links_writer () HB_AUTO_RETURN (( obj_.all_links_writer() ));
    خودکار real_links_writer () HB_AUTO_RETURN (( obj_.real_links.writer() ));
    خودکار virtual_links_writer () HB_AUTO_RETURN (( obj_.virtual_links.writer() ));

    فراغ مرتب_سازی_real_links () {
      obj_.real_links.qsort ();
    }

    گراف_نتیجه_t<فراغ> جمع_کن_real_link (بیعلامت عرض, بیعلامت obj_idx, بیعلامت position) {
      خودکار* link = obj_.real_links.push ();
      TRY (گراف_نتیجه_t<فراغ>::from (obj_.real_links, تخصیص_FAILURE));
      link->عرض = عرض;
      link->objidx = obj_idx;
      link->position = position;
      أرجع Ok();
    }

    گراف_نتیجه_t<فراغ> جمع_کن_virtual_link (بیعلامت obj_idx) {
      خودکار* link = obj_.virtual_links.push ();
      TRY (گراف_نتیجه_t<فراغ>::from (obj_.virtual_links, تخصیص_FAILURE));
      link->objidx = obj_idx;
      أرجع Ok();
    }

    گراف_نتیجه_t<فراغ> جمع_کن_real_link (ثابت hb_serialize_context_t::object_t::link_t& link)
    {
      obj_.real_links.push (link);
      أرجع گراف_نتیجه_t<فراغ>::from (obj_.real_links, تخصیص_FAILURE);
    }

    گراف_نتیجه_t<فراغ> مجموعه_real_links (hb_وکتور_t<hb_serialize_context_t::object_t::link_t>&& links)
    {
      TRY(گراف_نتیجه_t<فراغ>::from (links, تخصیص_FAILURE));
      obj_.real_links = links;
      أرجع Ok();
    }

    فراغ clear_real_links ()
    {
      obj_.real_links.reset();
    }

    فراغ clear_virtual_links ()
    {
      obj_.virtual_links.reset();
    }

    گراف_نتیجه_t<فراغ> جمع_کن_virtual_link (ثابت hb_serialize_context_t::object_t::link_t& link)
    {
      obj_.virtual_links.push (link);
      أرجع گراف_نتیجه_t<فراغ>::from (obj_.virtual_links, تخصیص_FAILURE);
    }

    ثابت hb_serialize_context_t::object_t& obj() ثابت { أرجع obj_; }

    قالب<نام_نوع T>
    T as () ثابت
    {
      أرجع reinterpret_cast<T> (obj_.سر_2);
    }

    منطقي has_incoming_virtual_یالها_2 () ثابت
    {
      أرجع has_incoming_virtual_یالها;
    }

    گراف_نتیجه_t<فراغ> link_positions_valid (بیعلامت رقم_objects, منطقي removed_nil)
    {
      hb_مجموعه_t assigned_بایتها;
      لكل (ثابت خودکار& l : obj_.real_links)
      {
        إذا (unlikely (l.objidx >= رقم_objects
            || (removed_nil && !l.objidx)))
        {
          DEBUG_پیام (SUBSET_REPACK, اشاره_تهی,
                     "Invalid graph. Invalid object index.");
          أرجع Err(INVALID_آرگومان);
        }

        بیعلامت شروع = l.position;
        بیعلامت انتها = شروع + l.عرض - 1;

        إذا (unlikely (l.عرض < 2 || l.عرض > 4))
        {
          DEBUG_پیام (SUBSET_REPACK, اشاره_تهی,
                     "Invalid graph. Invalid link width.");
          أرجع Err(INVALID_آرگومان);
        }

        إذا (unlikely (انتها >= جدول_اندازه ()))
        {
          DEBUG_پیام (SUBSET_REPACK, اشاره_تهی,
                     "Invalid graph. Link position is out of bounds.");
          أرجع Err(بیرون_از_BOUNDS);
        }

        إذا (unlikely (assigned_بایتها.intersects (شروع, انتها)))
        {
          DEBUG_پیام (SUBSET_REPACK, اشاره_تهی,
                     "Invalid graph. Found offsets whose positions overlap.");
          أرجع Err(INVALID_آرگومان);
        }

        assigned_بایتها.جمع_کن_range (شروع, انتها);
      }

      أرجع گراف_نتیجه_t<فراغ>::from (assigned_بایتها, تخصیص_FAILURE);
    }

    فراغ normalize ()
    {
      obj_.real_links.qsort ();
      لكل (ثابت خودکار& l : obj_.real_links)
      {
        لكل (بیعلامت i = 0; i < l.عرض; i++)
        {
          obj_.سر_2[l.position + i] = 0;
        }
      }
    }

    منطقي equals (بیعلامت this_شاخص,
                 بیعلامت other_شاخص,
                 ثابت رأس_t& other,
                 ثابت گراف_t& گراف,
                 ثابت گراف_t& other_گراف,
                 بیعلامت عمق) ثابت
    {
      إذا (!(as_بایتها () == other.as_بایتها ()))
      {
        DEBUG_پیام (SUBSET_REPACK, اشاره_تهی,
                   "vertex %u [%lu bytes] != %u [%lu bytes], depth = %u",
                   this_شاخص,
                   (بیعلامت دراز) جدول_اندازه (),
                   other_شاخص,
                   (بیعلامت دراز) other.جدول_اندازه (),
                   عمق);

        خودکار a = as_بایتها ();
        خودکار b = other.as_بایتها ();
        طالما (a || b)
        {
          DEBUG_پیام (SUBSET_REPACK, اشاره_تهی,
                     "  0x%x %s 0x%x", (بیعلامت) *a, (*a == *b) ? "==" : "!=", (بیعلامت) *b);
          a++;
          b++;
        }
        أرجع خطأ;
      }

      أرجع links_equal (obj_.real_links, other.obj_.real_links, گراف, other_گراف, عمق);
    }

    hb_بایتها_t as_بایتها () ثابت
    {
      أرجع hb_بایتها_t (obj_.سر_2, جدول_اندازه ());
    }

    دوست فراغ swap (رأس_t& a, رأس_t& b)
    {
      hb_جابجا (a.obj_, b.obj_);
      hb_جابجا (a.distance, b.distance);
      hb_جابجا (a.space, b.space);
      hb_جابجا (a.single_والد, b.single_والد);
      hb_جابجا (a.parents, b.parents);
      hb_جابجا (a.virtual_parents, b.virtual_parents);
      hb_جابجا (a.incoming_یالها, b.incoming_یالها);
      hb_جابجا (a.has_incoming_virtual_یالها, b.has_incoming_virtual_یالها);
      hb_جابجا (a.شروع, b.شروع);
      hb_جابجا (a.انتها, b.انتها);
      hb_جابجا (a.اولویت, b.اولویت);
    }

    گراف_نتیجه_t<hb_hashmap_t<بیعلامت, بیعلامت>>
    position_مقصد_شاخص_map () ثابت
    {
      hb_hashmap_t<بیعلامت, بیعلامت> نتیجه;

      نتیجه.alloc (obj_.real_links.درازی);
      لكل (ثابت خودکار& l : obj_.real_links) {
        نتیجه.مجموعه (l.position, l.objidx);
      }

      أرجع گراف_نتیجه_t<hb_hashmap_t<بیعلامت, بیعلامت>>::from (std::move(نتیجه), تخصیص_FAILURE);
    }

    منطقي is_shared () ثابت
    {
      أرجع parents.get_population () > virtual_parents.get_population () + 1;
    }

    بیعلامت incoming_یالها_2 () ثابت
    {
      إذا (HB_DEBUG_SUBSET_REPACK)
       {
	أكد (incoming_یالها == (single_والد != HB_گراف_INVALID) +
		(parents.مقادیر_ref () | hb_reduce (hb_جمع_کن, 0)));
       }
      أرجع incoming_یالها;
    }

    بیعلامت incoming_یالها_from_والد (بیعلامت والد_شاخص) ثابت {
      إذا (single_والد != HB_گراف_INVALID) {
        أرجع single_والد == والد_شاخص ? 1 : 0;
      }

      بیعلامت* شمار;
      أرجع  parents.has(والد_شاخص, &شمار) ? *شمار : 0;
    }

    فراغ reset_parents ()
    {
      incoming_یالها = 0;
      has_incoming_virtual_یالها = خطأ;
      single_والد = HB_گراف_INVALID;
      parents.reset ();
      virtual_parents.reset ();
    }

    گراف_نتیجه_t<فراغ> جمع_کن_والد (بیعلامت والد_شاخص, منطقي is_virtual)
    {
      أكد (والد_شاخص != HB_گراف_INVALID);
      has_incoming_virtual_یالها |= is_virtual;
      إذا (is_virtual)
        virtual_parents.جمع_کن (والد_شاخص);

      إذا (incoming_یالها == 0)
      {
	single_والد = والد_شاخص;
	incoming_یالها = 1;
	أرجع مقصد_نتیجه();
      }
      وإلا إذا (single_والد != HB_گراف_INVALID)
      {
        أكد (incoming_یالها == 1);
	إذا (!parents.مجموعه (single_والد, 1))
	  أرجع مقصد_نتیجه ();
	single_والد = HB_گراف_INVALID;
      }

      بیعلامت *v;
      إذا (parents.has (والد_شاخص, &v))
      {
        (*v)++;
	incoming_یالها++;
      }
      وإلا إذا (parents.مجموعه (والد_شاخص, 1))
	incoming_یالها++;

      أرجع مقصد_نتیجه();
    }

    فراغ remove_والد (بیعلامت والد_شاخص)
    {
      إذا (والد_شاخص == single_والد)
      {
	single_والد = HB_گراف_INVALID;
	incoming_یالها--;
	virtual_parents.reset ();
	أرجع;
      }

      بیعلامت *v;
      إذا (parents.has (والد_شاخص, &v))
      {
	incoming_یالها--;
	إذا (*v > 1)
	  (*v)--;
	وإلا
        {
	  parents.del (والد_شاخص);
          virtual_parents.del (والد_شاخص);
        }

	إذا (incoming_یالها == 1)
	{
	  single_والد = *parents.کلیدها ();
	  parents.reset ();
	}
      }
    }

    فراغ remove_real_link_unordered (بیعلامت link_array_شاخص) {
      obj_.real_links.remove_unordered (link_array_شاخص);
    }

    فراغ remove_real_link_unordered (بیعلامت فرزند_شاخص, ثابت فراغ* جابه_جایی)
    {
      بیعلامت شمار = obj_.real_links.درازی;
      لكل (بیعلامت i = 0; i < شمار; i++)
      {
        خودکار& link = obj_.real_links.arrayZ[i];
        إذا (link.objidx != فرزند_شاخص)
          استمر;

        إذا ((obj_.سر_2 + link.position) != جابه_جایی)
          استمر;

        obj_.real_links.remove_unordered (i);
        أرجع;
      }
    }

    گراف_نتیجه_t<فراغ> remap_والد (بیعلامت old_شاخص, بیعلامت new_شاخص)
    {
      إذا (single_والد != HB_گراف_INVALID)
      {
        إذا (single_والد == old_شاخص)
        {
	  single_والد = new_شاخص;
          إذا (virtual_parents.has (old_شاخص))
          {
            virtual_parents.del (old_شاخص);
            virtual_parents.جمع_کن (new_شاخص);
          }
        }
        أرجع گراف_نتیجه_t<فراغ>::from(virtual_parents, تخصیص_FAILURE);
      }

      ثابت بیعلامت *pv;
      إذا (parents.has (old_شاخص, &pv))
      {
        بیعلامت v = *pv;
	إذا (!parents.مجموعه (new_شاخص, v))
          incoming_یالها -= v;
	parents.del (old_شاخص);

        إذا (incoming_یالها == 1)
	{
	  single_والد = *parents.کلیدها ();
	  parents.reset ();
	}
      }

      إذا (virtual_parents.has (old_شاخص))
      {
        virtual_parents.del (old_شاخص);
        virtual_parents.جمع_کن (new_شاخص);
      }

      أرجع گراف_نتیجه_t<فراغ>::from(virtual_parents, تخصیص_FAILURE);
    }

    منطقي is_برگ () ثابت
    {
      أرجع !obj_.real_links.درازی && !obj_.virtual_links.درازی;
    }

    منطقي raise_اولویت ()
    {
      إذا (has_max_اولویت ()) أرجع خطأ;
      اولویت++;
      أرجع درست;
    }

    منطقي give_max_اولویت ()
    {
      منطقي نتیجه = خطأ;
      طالما (!has_max_اولویت()) {
        نتیجه = درست;
        اولویت++;
      }
      أرجع نتیجه;
    }

    منطقي has_max_اولویت () ثابت {
      أرجع اولویت >= 3;
    }

    size_t جدول_اندازه () ثابت {
      أرجع obj_.دم - obj_.سر_2;
    }

    int64_t modified_فاصله (بیعلامت ترتیب) ثابت
    {
      // TODO(garretrieger): once priority is high enough, should try
      // setting distance = 0 which will force to sort immediately after
      // it's parent where possible.

      int64_t modified_فاصله =
          hb_clamp (distance + فاصله_modifier (), (int64_t) 0,
                    hb_int_max (int64_t) >> 18);
      إذا (has_max_اولویت ()) {
        modified_فاصله = 0;
      }
      أرجع (modified_فاصله << 18) | (0x003FFFF & ترتیب);
    }

    int64_t فاصله_modifier () ثابت
    {
      إذا (!اولویت) أرجع 0;
      int64_t جدول_اندازه = obj_.دم - obj_.سر_2;

      إذا (اولویت == 1)
        أرجع -جدول_اندازه / 2;

      أرجع -جدول_اندازه;
    }

   خاص:
    گراف_نتیجه_t<فراغ> مقصد_نتیجه() ثابت {
      إذا (unlikely (
        parents.درون_error() || virtual_parents.درون_error() ||
        obj_.real_links.درون_error() || obj_.virtual_links.درون_error()))
        أرجع Err(تخصیص_FAILURE);
      أرجع Ok();
    }

    منطقي links_equal (ثابت hb_وکتور_t<hb_serialize_context_t::object_t::link_t>& this_links,
                      ثابت hb_وکتور_t<hb_serialize_context_t::object_t::link_t>& other_links,
                      ثابت گراف_t& گراف,
                      ثابت گراف_t& other_گراف,
                      بیعلامت عمق) ثابت
    {
      خودکار a = this_links.iter ();
      خودکار b = other_links.iter ();

      طالما (a && b)
      {
        ثابت خودکار& link_a = *a;
        ثابت خودکار& link_b = *b;

        إذا (link_a.عرض != link_b.عرض ||
            link_a.is_signed != link_b.is_signed ||
            link_a.whence != link_b.whence ||
            link_a.position != link_b.position ||
            link_a.bias != link_b.bias)
          أرجع خطأ;

        إذا (!گراف.vertices_[link_a.objidx].equals (link_a.objidx, link_b.objidx,
                other_گراف.vertices_[link_b.objidx], گراف, other_گراف, عمق + 1))
          أرجع خطأ;

        a++;
        b++;
      }

      إذا (منطقي (a) != منطقي (b))
        أرجع خطأ;

      أرجع درست;
    }
  };

  تعداد رأس_mutability_t
  {
    Immutable,
    Mutable
  };

  قالب <نام_نوع T, رأس_mutability_t mutability = Mutable>
  بنية رأس_and_جدول_t
  {
    رأس_and_جدول_t () : شاخص_2 (0), رأس (اشاره_تهی), جدول (اشاره_تهی)
    {}

    قالب <رأس_mutability_t other_mutability>
    رأس_and_جدول_t (ثابت رأس_and_جدول_t<T, other_mutability>& o)
      : شاخص_2 (o.شاخص_2), رأس (o.رأس), جدول (o.جدول)
    {}

    بیعلامت شاخص_2;
    نام_نوع std::conditional<mutability == Immutable, ثابت رأس_t*, رأس_t*>::type رأس;
    نام_نوع std::conditional<mutability == Immutable, ثابت T*, T*>::type جدول;

  };

  /*
   * A topological sorting of an object graph. Ordered
   * in reverse serialization order (first object in the
   * serialization is at the end of the list). This matches
   * the 'packed' object stack used internally in the
   * serializer
   */
  قالب<نام_نوع T>
  ساكن گراف_نتیجه_t<گراف_t> create (ثابت T& objects)
  {
    إذا (unlikely (objects.درازی > HB_REPACKER_MAX_VERTICES))
    {
      DEBUG_پیام (SUBSET_REPACK, اشاره_تهی,
                 "constructing graph: num of objects %u exceeds HB_REPACKER_MAX_VERTICES.",
                 objects.درازی);
      أرجع Err(حد_EXCEEDED);
    }

    گراف_t g;
    g.رقم_roots_for_space.push (1);
    TRY (گراف_نتیجه_t<فراغ>::from (g.رقم_roots_for_space, تخصیص_FAILURE));

    منطقي removed_nil = خطأ;
    ثابت بیعلامت شمار = objects.درازی;
    g.vertices_.alloc (شمار);
    g.ordering_.resize (شمار);
    g.ordering_scratch_.alloc (شمار);
    TRY (گراف_نتیجه_t<فراغ>::from (g.vertices_, تخصیص_FAILURE));
    TRY (گراف_نتیجه_t<فراغ>::from (g.ordering_, تخصیص_FAILURE));
    TRY (گراف_نتیجه_t<فراغ>::from (g.ordering_scratch_, تخصیص_FAILURE));

    بیعلامت ترتیب = objects.درازی;
    بیعلامت skip = 0;
    لكل (بیعلامت i = 0; i < شمار; i++)
    {
      // If this graph came from a serialization buffer object 0 is the
      // nil object. We don't need it for our purposes here so drop it.
      إذا (i == 0 && !objects.arrayZ[i])
      {
        removed_nil = درست;
        ترتیب--;
        g.ordering_.resize (شمار - 1);
        skip++;
        استمر;
      }

      hb_serialize_context_t::object_t obj (*objects.arrayZ[i]);
      إذا (unlikely (obj.درون_error ())) {
        أرجع Err(تخصیص_FAILURE);
      }
      رأس_t* v = g.vertices_.push (std::move (obj));

      TRY (v->link_positions_valid (شمار, removed_nil));

      // To start we set the ordering to match the provided objects
      // list. Note: objects are provided to us in reverse order (ie.
      // the last object is the root).
      بیعلامت obj_idx = i - skip;
      g.ordering_[--ترتیب] = obj_idx;

      إذا (!removed_nil) استمر;
      // Fix indices to account for removed nil object.
      لكل (خودکار& l : v->all_links_writer ()) {
        l.objidx--;
      }
    }

    خودکار r = g.is_fully_connected();
    إذا (r.is_err() && r.error() == ORPHANED_NODES) {
      g.print_orphaned_nodes ();
    }
    TRY(r);

    أرجع Ok (std::move (g));
  }

  گراف_t (ثابت گراف_t&) = حذف;
  گراف_t& عملگر = (ثابت گراف_t&) = حذف;
  گراف_t (گراف_t&&) = افتراضي;
  گراف_t& عملگر = (گراف_t&&) = افتراضي;

  ~گراف_t ()
  {
    لكل (حرف* b : buffers)
      hb_free (b);
  }

خاص:
  گراف_t () = افتراضي;

عام:

  منطقي عملگر== (ثابت گراف_t& other) ثابت
  {
    أرجع ریشه ().equals (ریشه_idx(), other.ریشه_idx(), other.ریشه (), *هذا, other, 0);
  }

  فراغ print () ثابت {
    لكل (بیعلامت id : ordering_)
    {
      ثابت خودکار& v = vertices_[id];
      چاپ_کن("%u: %u [", id, (بیعلامت صحيح)v.جدول_اندازه());
      لكل (ثابت خودکار &l : v.obj().real_links) {
        چاپ_کن("%u, ", l.objidx);
      }
      لكل (ثابت خودکار &l : v.obj().virtual_links) {
        چاپ_کن("v%u, ", l.objidx);
      }
      چاپ_کن("]\n");
    }
  }

  // Sorts links of all objects in a consistent manner and zeroes all offsets.
  فراغ normalize ()
  {
    لكل (خودکار& v : vertices_.writer ())
      v.normalize ();
  }

  ثابت رأس_t& ریشه () ثابت
  {
    أرجع vertices_[ریشه_idx ()];
  }

  بیعلامت ریشه_idx () ثابت
  {
    // First element of ordering_ is the root.
    // Since the graph is topologically sorted it's safe to
    // assume the first object has no incoming edges.
    أرجع ordering_[0];
  }

  ثابت hb_serialize_context_t::object_t& object (بیعلامت i) ثابت
  {
    أرجع vertices_[i].obj();
  }

  گراف_نتیجه_t<فراغ> جمع_کن_میانگیر (حرف* میانگیر)
  {
    buffers.push (میانگیر);
    أرجع گراف_نتیجه_t<فراغ>::from (buffers, تخصیص_FAILURE);
  }

  /*
   * Adds a 16 bit link from parent_id to child_id
   */
  قالب<نام_نوع T>
  گراف_نتیجه_t<فراغ> جمع_کن_link (T* جابه_جایی,
                                 بیعلامت والد_id,
                                 بیعلامت فرزند_id)
  {
    إذا (unlikely (والد_id >= vertices_.درازی || فرزند_id >= vertices_.درازی))
      أرجع Err(بیرون_از_BOUNDS);
    خودکار& v = vertices_[والد_id];
    TRY(v.جمع_کن_real_link(2, فرزند_id, (حرف*) جابه_جایی - (حرف*) v.obj().سر_2));
    أرجع vertices_[فرزند_id].جمع_کن_والد (والد_id, خطأ);
  }

  /*
   * Generates a new topological sorting of graph ordered by the shortest
   * distance to each node if positions are marked as invalid.
   */
  گراف_نتیجه_t<فراغ> مرتب_سازی_کوتاهترین_فاصله_if_needed ()
  {
    إذا (!positions_invalid) أرجع Ok();
    أرجع مرتب_سازی_کوتاهترین_فاصله ();
  }


  /*
   * Generates a new topological sorting of graph ordered by the shortest
   * distance to each node.
   */
  گراف_نتیجه_t<فراغ> مرتب_سازی_کوتاهترین_فاصله ()
  {
    positions_invalid = درست;

    إذا (vertices_.درازی <= 1) {
      // Graph of 1 or less doesn't need sorting.
      أرجع Ok();
    }

    TRY (update_distances ());

    hb_اولویت_صف_t<int64_t> صف;
    صف.alloc (vertices_.درازی);
    TRY (گراف_نتیجه_t<فراغ>::from (صف, تخصیص_FAILURE));

    hb_وکتور_t<بیعلامت> &new_ordering = ordering_scratch_;
    new_ordering.resize (vertices_.درازی);
    TRY (گراف_نتیجه_t<فراغ>::from (new_ordering, تخصیص_FAILURE));

    hb_وکتور_t<بیعلامت> removed_یالها;
    removed_یالها.resize (vertices_.درازی);
    TRY (گراف_نتیجه_t<فراغ>::from (removed_یالها, تخصیص_FAILURE));

    TRY (update_parents ());

    صف.insert (ریشه ().modified_فاصله (0), ریشه_idx ());
    بیعلامت ترتیب = 1;
    بیعلامت pos = 0;
    طالما (!صف.درون_error () && !صف.is_empty ())
    {
      بیعلامت بعدی_id = صف.برداشت_minimum().second;

      إذا (unlikely (pos >= new_ordering.درازی)) {
        // We are out of ids. Which means we've visited a node more than once.
        // This graph contains a cycle which is not allowed.
        DEBUG_پیام (SUBSET_REPACK, اشاره_تهی, "Invalid graph. Contains cycle.");
        أرجع Err(چرخه_DETECTED);
      }
      new_ordering[pos++] = بعدی_id;
      ثابت رأس_t& next = vertices_[بعدی_id];

      لكل (ثابت خودکار& link : next.obj().all_links ()) {
        إذا (unlikely (link.objidx >= vertices_.درازی))
          أرجع Err(بیرون_از_BOUNDS);
        removed_یالها[link.objidx]++;
        ثابت خودکار& v = vertices_[link.objidx];
        إذا (!(v.incoming_یالها_2 () - removed_یالها[link.objidx]))
          // Add the order that the links were encountered to the priority.
          // This ensures that ties between priorities objects are broken in a consistent
          // way. More specifically this is set up so that if a set of objects have the same
          // distance they'll be added to the topological order in the order that they are
          // referenced from the parent object.
          صف.insert (v.modified_فاصله (ترتیب++),
                        link.objidx);
      }
    }

    TRY (گراف_نتیجه_t<فراغ>::from (صف, تخصیص_FAILURE));
    TRY (گراف_نتیجه_t<فراغ>::from (new_ordering, تخصیص_FAILURE));

    hb_جابجا (ordering_, new_ordering);

    إذا (unlikely (pos != vertices_.درازی)) {
      print_orphaned_nodes ();
      أرجع Err(ORPHANED_NODES);
    }

    أرجع Ok();
  }

  /*
   * Finds the set of nodes (placed into roots) that should be assigned unique spaces.
   * More specifically this looks for the top most 24 bit or 32 bit links in the graph.
   * Some special casing is done that is specific to the layout of GSUB/GPOS tables.
   */
  گراف_نتیجه_t<فراغ> پیدا_کن_space_roots (hb_مجموعه_t& دیدهشده, hb_مجموعه_t& roots)
  {
    بیعلامت ریشه_شاخص = ریشه_idx ();
    لكل (بیعلامت i : ordering_)
    {
      إذا (دیدهشده.has (i)) استمر;

      // Only real links can form 32 bit spaces
      لكل (ثابت خودکار& l : vertices_[i].obj().real_links)
      {
        إذا (l.is_signed || l.عرض < 3)
          استمر;

        إذا (i == ریشه_شاخص && l.عرض == 3)
          // Ignore 24bit links from the root node, this skips past the single 24bit
          // pointer to the lookup list.
          استمر;

        إذا (l.عرض >= 3)
        {
          // A wide offset forms a root, unless there are 32-bit offsets somewhere
          // in its subgraph, then those become the roots instead. This is to make sure
          // that extension subtables beneath a 24-bit lookup and lookups beneath a
          // 32-bit lookup list become the spaces instead of their enclosing lists.
          hb_مجموعه_t sub_roots;
          TRY (پیدا_کن_32bit_roots (l.objidx, sub_roots));
          إذا (sub_roots) {
            لكل (بیعلامت sub_ریشه_idx : sub_roots) {
              roots.جمع_کن (sub_ریشه_idx);
              TRY (پیدا_کن_subgraph (sub_ریشه_idx, دیدهشده));
            }
            استمر;
          }
        }

        roots.جمع_کن (l.objidx);
        TRY (پیدا_کن_subgraph (l.objidx, دیدهشده));
      }
    }

    TRY(گراف_نتیجه_t<فراغ>::from (دیدهشده, تخصیص_FAILURE));
    TRY(گراف_نتیجه_t<فراغ>::from (roots, تخصیص_FAILURE));
    أرجع Ok();
  }

  قالب <نام_نوع T, نام_نوع ...Ts>
  گراف_نتیجه_t<رأس_and_جدول_t<T, Immutable>> as_جدول (بیعلامت والد, ثابت فراغ* جابه_جایی, Ts... ds)
  {
    TRY_تخصیص_بده (بیعلامت idx, شاخص_for_جابه_جایی (والد, جابه_جایی));
    أرجع as_جدول_from_شاخص<T, Immutable> (idx, std::forward<Ts>(ds)...);
  }

  قالب <نام_نوع T, نام_نوع ...Ts>
  گراف_نتیجه_t<رأس_and_جدول_t<T, Mutable>> as_mutable_جدول (بیعلامت والد, ثابت فراغ* جابه_جایی, Ts... ds)
  {
    TRY_تخصیص_بده (خودکار idx, mutable_شاخص_for_جابه_جایی (والد, جابه_جایی));
    أرجع as_جدول_from_شاخص<T, Mutable> (idx, std::forward<Ts>(ds)...);
  }

  قالب <نام_نوع T, رأس_mutability_t mutability = Mutable, نام_نوع ...Ts>
  گراف_نتیجه_t<رأس_and_جدول_t<T, mutability>> as_جدول_from_شاخص (بیعلامت شاخص_2, Ts... ds)
  {
    إذا (unlikely (شاخص_2 >= vertices_.درازی))
      أرجع Err(بیرون_از_BOUNDS);

    رأس_and_جدول_t<T, mutability> r;
    r.رأس = (نام_نوع std::conditional<mutability == Immutable, ثابت رأس_t*, رأس_t*>::type) &vertices_[شاخص_2];
    r.جدول = r.رأس->قالب as<نام_نوع std::conditional<mutability == Immutable, ثابت T*, T*>::type> ();
    r.شاخص_2 = شاخص_2;
    إذا (unlikely (!r.جدول))
      أرجع Err(INVALID_آرگومان);

    TRY(r.جدول->sanitize (*(r.رأس), std::forward<Ts>(ds)...));
    أرجع r;
  }

  // Finds the object id of the object pointed to by the offset at 'offset'
  // within object[node_idx].
  گراف_نتیجه_t<بیعلامت> شاخص_for_جابه_جایی (بیعلامت گره_idx, ثابت فراغ* جابه_جایی) ثابت
  {
    إذا (unlikely (گره_idx >= vertices_.درازی)) أرجع Err(بیرون_از_BOUNDS);
    ثابت خودکار& node = object (گره_idx);
    إذا (unlikely (جابه_جایی < node.سر_2 || جابه_جایی >= node.دم)) أرجع Err(بیرون_از_BOUNDS);

    بیعلامت شمار = node.real_links.درازی;
    لكل (بیعلامت i = 0; i < شمار; i++)
    {
      // Use direct access for increased performance, this is a hot method.
      ثابت خودکار& link = node.real_links.arrayZ[i];
      إذا (جابه_جایی != node.سر_2 + link.position)
        استمر;
      أرجع Ok(link.objidx);
    }

    أرجع Err(INVALID_آرگومان);
  }

  // Finds the object id of the object pointed to by the offset at 'offset'
  // within object[node_idx]. Ensures that the returned object is safe to mutate.
  // That is, if the original child object is shared by parents other than node_idx
  // it will be duplicated and the duplicate will be returned instead.
  گراف_نتیجه_t<بیعلامت> mutable_شاخص_for_جابه_جایی (بیعلامت گره_idx, ثابت فراغ* جابه_جایی)
  {
    TRY_تخصیص_بده (بیعلامت فرزند_idx, شاخص_for_جابه_جایی (گره_idx, جابه_جایی));
    إذا (unlikely (فرزند_idx >= vertices_.درازی))
      أرجع Err(بیرون_از_BOUNDS);

    خودکار& فرزند = vertices_[فرزند_idx];
    لكل (بیعلامت p : فرزند.parents_iter ())
    {
      إذا (p != گره_idx) {
        أرجع duplicate (گره_idx, فرزند_idx, درست);
      }
    }

    أرجع Ok(فرزند_idx);
  }


  /*
   * Assign unique space numbers to each connected subgraph of 24 bit and/or 32 bit offset(s).
   * Currently, this is implemented specifically tailored to the structure of a GPOS/GSUB
   * (including with 24bit offsets) table.
   */
  گراف_نتیجه_t<منطقي> تخصیص_بده_spaces ()
  {
    TRY (update_parents ());

    لكل (خودکار& رأس : vertices_.writer ())
    {
      رأس.space = 0;
      رأس.اولویت = 0;
    }
    رقم_roots_for_space.resize (1);
    TRY (گراف_نتیجه_t<فراغ>::from (رقم_roots_for_space, تخصیص_FAILURE));
    رقم_roots_for_space[0] = 1;
    فاصله_invalid = درست;
    positions_invalid = درست;

    hb_مجموعه_t دیدهشده;
    hb_مجموعه_t roots;
    TRY (پیدا_کن_space_roots (دیدهشده, roots));

    // Mark everything not in the subgraphs of the roots as visited. This prevents
    // subgraphs from being connected via nodes not in those subgraphs.
    دیدهشده.invert ();

    إذا (!roots) أرجع Ok(خطأ);

    طالما (roots)
    {
      uint32_t next = HB_مجموعه_مقدار_INVALID;
      TRY (گراف_نتیجه_t<فراغ>::from (roots, تخصیص_FAILURE));
      إذا (!roots.next (&next)) اكسر;

      hb_مجموعه_t connected_roots;
      TRY (پیدا_کن_connected_nodes (next, roots, دیدهشده, connected_roots));
      TRY (گراف_نتیجه_t<فراغ>::from (connected_roots, تخصیص_FAILURE));

      TRY (isolate_subgraph (connected_roots));
      TRY (گراف_نتیجه_t<فراغ>::from (connected_roots, تخصیص_FAILURE));

      بیعلامت بعدی_space = هذا->بعدی_space ();
      إذا (unlikely (بعدی_space >= HB_REPACKER_MAX_SPACES))
      {
        DEBUG_پیام (SUBSET_REPACK, اشاره_تهی,
                 "num of spaces %u exceeds HB_REPACKER_MAX_SPACES.",
                 بعدی_space);
        أرجع Err(حد_EXCEEDED);
      }
      رقم_roots_for_space.push (0);
      TRY(گراف_نتیجه_t<فراغ>::from (رقم_roots_for_space, تخصیص_FAILURE));

      لكل (بیعلامت ریشه : connected_roots)
      {
        DEBUG_پیام (SUBSET_REPACK, اشاره_تهی, "Subgraph %u gets space %u", ریشه, بعدی_space);
        vertices_[ریشه].space = بعدی_space;
        رقم_roots_for_space[بعدی_space] = رقم_roots_for_space[بعدی_space] + 1;
        فاصله_invalid = درست;
        positions_invalid = درست;
      }

      // TODO(grieger): special case for GSUB/GPOS use extension promotions to move 16 bit space
      //                into the 32 bit space as needed, instead of using isolation.
    }

    أرجع Ok(درست);
  }

  /*
   * Isolates the subgraph of nodes reachable from root. Any links to nodes in the subgraph
   * that originate from outside of the subgraph will be removed by duplicating the linked to
   * object.
   *
   * Indices stored in roots will be updated if any of the roots are duplicated to new indices.
   */
  گراف_نتیجه_t<منطقي> isolate_subgraph (hb_مجموعه_t& roots)
  {
    TRY(update_parents ());
    hb_map_t subgraph;

    // incoming edges to root_idx should be all 32 bit in length so we don't need to de-dup these
    // set the subgraph incoming edge count to match all of root_idx's incoming edges
    hb_مجموعه_t parents;
    لكل (بیعلامت ریشه_idx : roots)
    {
      إذا (unlikely (ریشه_idx >= vertices_.درازی))
        أرجع Err(بیرون_از_BOUNDS);
      subgraph.مجموعه (ریشه_idx, wide_parents (ریشه_idx, parents));
    }
    لكل (بیعلامت ریشه_idx : roots)
    {
      TRY (پیدا_کن_subgraph (ریشه_idx, subgraph));
    }
    TRY (گراف_نتیجه_t<فراغ>::from (subgraph, تخصیص_FAILURE));

    hb_map_t شاخص_map;
    منطقي made_changes = خطأ;
    لكل (خودکار مدخل : subgraph.iter ())
    {
      أكد (مدخل.اولی < vertices_.درازی);
      ثابت خودکار& node = vertices_[مدخل.اولی];
      بیعلامت subgraph_incoming_یالها = مدخل.second;

      إذا (subgraph_incoming_یالها < node.incoming_یالها_2 ())
      {
        // Only  de-dup objects with incoming links from outside the subgraph.
        made_changes = درست;
        TRY (duplicate_subgraph (مدخل.اولی, شاخص_map));
      }
    }

    TRY (گراف_نتیجه_t<فراغ>::from (شاخص_map, تخصیص_FAILURE));

    إذا (!made_changes)
      أرجع Ok(خطأ);

    خودکار new_subgraph =
        + subgraph.کلیدها ()
        | hb_map([&] (uint32_t گره_idx) {
          ثابت uint32_t *v;
          إذا (شاخص_map.has (گره_idx, &v)) أرجع *v;
          أرجع گره_idx;
        })
        ;

    TRY(remap_obj_indices (شاخص_map, new_subgraph));
    TRY(remap_obj_indices (شاخص_map, parents.iter (), درست));

    // Update roots set with new indices as needed.
    لكل (خودکار next : roots)
    {
      ثابت uint32_t *v;
      إذا (شاخص_map.has (next, &v))
      {
        roots.del (next);
        roots.جمع_کن (*v);
      }
    }

    أرجع Ok(درست);
  }

  // BFS graph traversal starting at start_idx.
  //
  // The visit_edge function will be called once for the root node and then for each traversed edge
  // with the following signature:
  //
  // bool VisitEdgeFunc(unsigned parent, const link_t* link, unsigned child, unsigned depth)
  //
  // Where a return value of false signals that traversal should not continue into child's outgoing
  // edges. parent/child are the vertex indices. depth starts at 0 for the root node.
  // link will be null when called for entering the root node at the start of the traversal.
  //
  // This traversal does not use a visited set internally, it is the responsibility of visit_edge
  // to track and filter visited nodes if required for the particular traversal.
  قالب <نام_نوع بازدید_یال_Func>
  گراف_نتیجه_t<فراغ> traverse_directed_bfs (بیعلامت شروع_idx, بازدید_یال_Func&& بازدید_یال)
  {
    إذا (unlikely (شروع_idx >= vertices_.درازی))
    {
      DEBUG_پیام (SUBSET_REPACK, اشاره_تهی,
                 "traverse_directed_bfs: unexpected start_idx out of bounds.");
      أرجع Err(بیرون_از_BOUNDS);
    }

    // For performance we want to avoid allocating extra memory. So use the ordering_scratch_
    // buffer to implement a queue for BFS.
    ordering_scratch_.resize (vertices_.درازی);
    TRY (گراف_نتیجه_t<فراغ>::from (ordering_scratch_, تخصیص_FAILURE));

    إذا (!بازدید_یال (HB_CODEPOINT_INVALID, اشاره_تهی, شروع_idx, 0))
      أرجع Ok();

    بیعلامت سر_2 = 0;
    بیعلامت دم = 0;
    خودکار& صف = ordering_scratch_;
    صف[دم++] = شروع_idx;

    بیعلامت عمق = 0;
    طالما (سر_2 < دم)
    {
      بیعلامت level_انتها = دم;
      طالما (سر_2 < level_انتها)
      {
        بیعلامت گره_idx = صف[سر_2++];
        ثابت خودکار& v = vertices_[گره_idx];

        بیعلامت رقم_real = v.obj().real_links.درازی;
        بیعلامت مجموع_links = رقم_real + v.obj().virtual_links.درازی;

        لكل (بیعلامت i = 0; i < مجموع_links; i++)
        {
          // vertices_ may have re-alloc'd inside an op() call, so reassign the v ref.
          ثابت خودکار& v = vertices_[گره_idx];
          ثابت خودکار& link = (i < رقم_real)
                             ? v.obj().real_links[i]
                             : v.obj().virtual_links[i - رقم_real];
          بیعلامت فرزند_idx = link.objidx;

          إذا (!بازدید_یال (گره_idx, &link, فرزند_idx, عمق + 1))
            استمر;

          إذا (unlikely (دم >= صف.درازی))
            أرجع Err(چرخه_DETECTED);

          صف[دم++] = فرزند_idx;
        }
      }
      عمق++;
    }
    أرجع Ok();
  }

  گراف_نتیجه_t<فراغ> پیدا_کن_subgraph (بیعلامت گره_idx, hb_map_t& subgraph)
  {
    أرجع traverse_directed_bfs (گره_idx, [&] (
      بیعلامت والد,
      ثابت hb_serialize_context_t::object_t::link_t* link,
      بیعلامت فرزند,
      بیعلامت عمق) {
      إذا (عمق == 0) أرجع درست;
      hb_codepoint_t *شمار;
      إذا (subgraph.has (فرزند, &شمار))
      {
        (*شمار)++;
        أرجع خطأ;
      }
      subgraph.مجموعه (فرزند, 1);
      أرجع درست;
    });
  }

  گراف_نتیجه_t<فراغ> پیدا_کن_subgraph (بیعلامت گره_idx, hb_مجموعه_t& subgraph)
  {
    أرجع traverse_directed_bfs (گره_idx, [&] (
      بیعلامت والد,
      ثابت hb_serialize_context_t::object_t::link_t* link,
      بیعلامت فرزند,
      بیعلامت عمق) {
      إذا (subgraph.has (فرزند)) أرجع خطأ;
      subgraph.جمع_کن (فرزند);
      أرجع درست;
    });
  }

  گراف_نتیجه_t<size_t> پیدا_کن_subgraph_اندازه (بیعلامت گره_idx, hb_مجموعه_t& subgraph, بیعلامت max_عمق = HB_گراف_INVALID)
  {
    size_t اندازه = 0;
    TRY (traverse_directed_bfs (گره_idx, [&] (
      بیعلامت والد,
      ثابت hb_serialize_context_t::object_t::link_t* link,
      بیعلامت فرزند,
      بیعلامت عمق) {
      إذا (subgraph.has (فرزند)) أرجع خطأ;
      subgraph.جمع_کن (فرزند);

      ثابت خودکار& o = vertices_[فرزند].obj();
      اندازه += o.دم - o.سر_2;
      أرجع عمق < max_عمق;
    }));
    أرجع Ok(اندازه);
  }

  /*
   * Finds the topmost children of 32bit offsets in the subgraph starting
   * at node_idx. Found indices are placed into 'found'.
   */
  گراف_نتیجه_t<فراغ> پیدا_کن_32bit_roots (بیعلامت گره_idx, hb_مجموعه_t& یافت)
  {
    // Note: this specifically requires a BFS based traversal to ensure we don't recurse through
    // a node that is accessible via both 32bit and non-32 bit links.
    hb_مجموعه_t دیدهشده;
    أرجع traverse_directed_bfs (گره_idx, [&] (
      بیعلامت والد,
      ثابت hb_serialize_context_t::object_t::link_t* link,
      بیعلامت فرزند,
      بیعلامت _) {

      إذا (link && یافت.has(والد))
        // Don't traverse from something that's already marked as a root.
        أرجع خطأ;

      إذا (link && !link->is_signed && link->عرض == 4)
      {
        یافت.جمع_کن (link->objidx);
        دیدهشده.جمع_کن (link->objidx);
        أرجع خطأ;
      }

      إذا (دیدهشده.has (فرزند)) أرجع خطأ;
      دیدهشده.جمع_کن (فرزند);
      أرجع درست;
    });
  }

  /*
   * Moves the child of old_parent_idx pointed to by old_offset to a new
   * vertex at the new_offset.
   *
   * Returns the id of the child node that was moved.
   */
  قالب<نام_نوع O>
  گراف_نتیجه_t<بیعلامت> move_فرزند (بیعلامت old_والد_idx,
                                       ثابت O* old_جابه_جایی,
                                       بیعلامت new_والد_idx,
                                       ثابت O* new_جابه_جایی)
  {
    فاصله_invalid = درست;
    positions_invalid = درست;

    إذا (unlikely (old_والد_idx >= vertices_.درازی ||
                  new_والد_idx >= vertices_.درازی))
      أرجع Err(بیرون_از_BOUNDS);

    خودکار& old_v = vertices_[old_والد_idx];
    خودکار& new_v = vertices_[new_والد_idx];

    TRY_تخصیص_بده (بیعلامت فرزند_id, شاخص_for_جابه_جایی (old_والد_idx,
                                                     old_جابه_جایی));
    إذا (unlikely (فرزند_id >= vertices_.درازی))
      أرجع Err(بیرون_از_BOUNDS);

    TRY(new_v.جمع_کن_real_link(O::static_اندازه, فرزند_id, (ثابت حرف*) new_جابه_جایی - (ثابت حرف*) new_v.obj().سر_2));

    خودکار& فرزند = vertices_[فرزند_id];
    TRY(فرزند.جمع_کن_والد (new_والد_idx, خطأ));

    old_v.remove_real_link_unordered (فرزند_id, old_جابه_جایی);
    فرزند.remove_والد (old_والد_idx);

    أرجع Ok(فرزند_id);
  }

  /*
   * Moves all outgoing links in old parent that have
   * a link position between [old_post_start, old_pos_end)
   * to the new parent. Links are placed serially in the new
   * parent starting at new_pos_start.
   */
  قالب<نام_نوع O>
  گراف_نتیجه_t<فراغ> move_children (بیعلامت old_والد_idx,
                                      بیعلامت old_pos_شروع,
                                      بیعلامت old_pos_انتها,
                                      بیعلامت new_والد_idx,
                                      بیعلامت new_pos_شروع)
  {
    إذا (unlikely (old_والد_idx >= vertices_.درازی ||
                  new_والد_idx >= vertices_.درازی))
      أرجع Err(بیرون_از_BOUNDS);

    فاصله_invalid = درست;
    positions_invalid = درست;

    خودکار& old_v = vertices_[old_والد_idx];
    خودکار& new_v = vertices_[new_والد_idx];

    hb_وکتور_t<hb_serialize_context_t::object_t::link_t> old_links;
    لكل (ثابت خودکار& l : old_v.obj().real_links)
    {
      إذا (l.position < old_pos_شروع || l.position >= old_pos_انتها)
      {
        old_links.push (l);
        استمر;
      }

      بیعلامت array_pos = l.position - old_pos_شروع;

      بیعلامت فرزند_id = l.objidx;
      إذا (unlikely (فرزند_id >= vertices_.درازی))
        أرجع Err(بیرون_از_BOUNDS);

      TRY(new_v.جمع_کن_real_link(O::static_اندازه, فرزند_id, new_pos_شروع + array_pos));

      خودکار& فرزند = vertices_[فرزند_id];
      TRY(فرزند.جمع_کن_والد (new_والد_idx, خطأ));
      فرزند.remove_والد (old_والد_idx);
    }

    TRY (گراف_نتیجه_t<فراغ>::from (old_links, تخصیص_FAILURE));
    أرجع old_v.مجموعه_real_links(std::move (old_links));
  }

  /*
   * duplicates all nodes in the subgraph reachable from node_idx. Does not re-assign
   * links. index_map is updated with mappings from old id to new id. If a duplication has already
   * been performed for a given index, then it will be skipped.
   */
  گراف_نتیجه_t<فراغ> duplicate_subgraph (بیعلامت گره_idx, hb_map_t& شاخص_map)
  {
    گراف_نتیجه_t<فراغ> نتیجه = Ok();
    TRY (traverse_directed_bfs (گره_idx, [&] (
      بیعلامت والد,
      ثابت hb_serialize_context_t::object_t::link_t* link,
      بیعلامت فرزند,
      بیعلامت _) {
      إذا (نتیجه.is_err () || شاخص_map.has (فرزند)) أرجع خطأ;
      خودکار r = duplicate (فرزند);
      إذا (unlikely (r.is_err ()))
      {
        نتیجه = Err(r.error ());
        أرجع خطأ;
      }
      شاخص_map.مجموعه (فرزند, *r);
      أرجع درست;
    }));

    أرجع گراف_نتیجه_t<فراغ>::from(شاخص_map, تخصیص_FAILURE);
  }

  /*
   * Creates a copy of node_idx and returns it's new index.
   */
  گراف_نتیجه_t<بیعلامت> duplicate (بیعلامت گره_idx, منطقي copy_جدول = خطأ)
  {
    إذا (unlikely (گره_idx >= vertices_.درازی))
      أرجع Err(بیرون_از_BOUNDS);

    إذا (unlikely (vertices_.درازی >= HB_REPACKER_MAX_VERTICES))
    {
      DEBUG_پیام (SUBSET_REPACK, اشاره_تهی,
                 "duplicating node: num of vertices %u exceeds HB_REPACKER_MAX_VERTICES.",
                 vertices_.درازی);
      أرجع Err(حد_EXCEEDED);
    }

    positions_invalid = درست;
    فاصله_invalid = درست;

    خودکار* clone = vertices_.push ();
    TRY (گراف_نتیجه_t<فراغ>::from (vertices_, تخصیص_FAILURE));

    بیعلامت clone_idx = vertices_.درازی - 1;
    ordering_.push (clone_idx);
    TRY (گراف_نتیجه_t<فراغ>::from (ordering_, تخصیص_FAILURE));

    خودکار& فرزند = vertices_[گره_idx];

    بیعلامت جدول_اندازه = فرزند.obj().دم - فرزند.obj().سر_2;
    إذا (copy_جدول && جدول_اندازه)
    {
      حرف* میانگیر = (حرف*) hb_malloc (جدول_اندازه);
      إذا (unlikely (!میانگیر))
        أرجع Err(تخصیص_FAILURE);
      خودکار res = جمع_کن_میانگیر (میانگیر);
      إذا (unlikely (res.is_err ()))
      {
        hb_free (میانگیر);
        أرجع Err(res.error ());
      }
      hb_memcpy (میانگیر, فرزند.obj().سر_2, جدول_اندازه);
      clone->مجموعه_میانگیر(میانگیر, میانگیر + جدول_اندازه);
    }
    وإلا
    {
      clone->مجموعه_میانگیر(فرزند.obj().سر_2, فرزند.obj().دم);
    }
    clone->distance = فرزند.distance;
    clone->space = فرزند.space;
    clone->reset_parents ();

    لكل (ثابت خودکار& l : فرزند.obj().real_links)
    {
      TRY(clone->جمع_کن_real_link (l));
      TRY(vertices_[l.objidx].جمع_کن_والد (clone_idx, خطأ));
    }

    لكل (ثابت خودکار& l : فرزند.obj().virtual_links)
    {
      TRY(clone->جمع_کن_virtual_link (l));
      TRY(vertices_[l.objidx].جمع_کن_والد (clone_idx, درست));
    }

    أرجع Ok(clone_idx);
  }

  /*
   * Creates a copy of child and re-assigns the link from
   * parent to the clone. The copy is a shallow copy, objects
   * linked from child are not duplicated.
   *
   * Returns the index of the newly created duplicate.
   *
   * If the child_idx only has incoming edges from parent_idx,
   * duplication isn't possible and this will return an error.
   */
  گراف_نتیجه_t<بیعلامت> duplicate (بیعلامت والد_idx, بیعلامت فرزند_idx, منطقي copy_جدول = خطأ)
  {
    إذا (unlikely (والد_idx >= vertices_.درازی || فرزند_idx >= vertices_.درازی))
      أرجع Err(بیرون_از_BOUNDS);

    TRY (update_parents ());

    ثابت خودکار& فرزند = vertices_[فرزند_idx];
    بیعلامت links_مقصد_فرزند = فرزند.incoming_یالها_from_والد (والد_idx);

    إذا (unlikely (فرزند.incoming_یالها_2 () <= links_مقصد_فرزند || فرزند.has_incoming_virtual_یالها_2 ()))
    {
      // Can't duplicate this node, doing so would orphan the original one as all remaining links
      // to child are from parent.
      //
      // We don't allow duplication of nodes with incoming virtual edges because we don't track
      // the number of virtual vs real incoming edges. As a result we can't tell if a node
      // with virtual edges may end up orphaned by duplication (ie. where one copy is only pointed
      // to by virtual edges).
      DEBUG_پیام (SUBSET_REPACK, اشاره_تهی, "  Not duplicating %u => %u",
                 والد_idx, فرزند_idx);
      أرجع Err(INVALID_آرگومان);
    }

    DEBUG_پیام (SUBSET_REPACK, اشاره_تهی, "  Duplicating %u => %u",
               والد_idx, فرزند_idx);

    TRY_تخصیص_بده (بیعلامت clone_idx, duplicate (فرزند_idx, copy_جدول));
    // duplicate shifts the root node idx, so if parent_idx was root update it.
    إذا (والد_idx == clone_idx) والد_idx++;

    خودکار& والد = vertices_[والد_idx];
    بیعلامت شمار = 0;
    بیعلامت رقم_real = والد.obj().real_links.درازی;
    لكل (خودکار& l : والد.all_links_writer ())
    {
      شمار++;
      إذا (l.objidx != فرزند_idx)
        استمر;

      TRY(reassign_link (l, والد_idx, clone_idx, شمار > رقم_real));
    }

    أرجع Ok(clone_idx);
  }

  /*
   * Creates a copy of child and re-assigns the links from
   * parents to the clone. The copy is a shallow copy, objects
   * linked from child are not duplicated.
   *
   * Returns the index of the newly created duplicate.
   *
   * If the child_idx only has incoming edges from parents,
   * duplication isn't possible or duplication fails and this will
   * return an error.
   */
  گراف_نتیجه_t<بیعلامت> duplicate (ثابت hb_مجموعه_t* parents, بیعلامت فرزند_idx)
  {
    إذا (unlikely (!parents || parents->is_empty ()))
      أرجع Err(INVALID_آرگومان);

    إذا (unlikely (فرزند_idx >= vertices_.درازی))
      أرجع Err(بیرون_از_BOUNDS);

    TRY (update_parents ());

    ثابت خودکار& فرزند = vertices_[فرزند_idx];
    بیعلامت links_مقصد_فرزند = 0;
    بیعلامت آخرین_والد = parents->get_max ();
    بیعلامت اولی_والد = parents->get_min ();
    لكل (بیعلامت والد_idx : *parents) {
      إذا (unlikely (والد_idx >= vertices_.درازی))
        أرجع Err(بیرون_از_BOUNDS);
      links_مقصد_فرزند += فرزند.incoming_یالها_from_والد (والد_idx);
    }

    إذا (unlikely (فرزند.incoming_یالها_2 () <= links_مقصد_فرزند || فرزند.has_incoming_virtual_یالها_2 ()))
    {
      // Can't duplicate this node, doing so would orphan the original one as all remaining links
      // to child are from parent.
      //
      // We don't allow duplication of nodes with incoming virtual edges because we don't track
      // the number of virtual vs real incoming edges. As a result we can't tell if a node
      // with virtual edges may end up orphaned by duplication (ie. where one copy is only pointed
      // to by virtual edges).
      DEBUG_پیام (SUBSET_REPACK, اشاره_تهی, "  Not duplicating %u, ..., %u => %u", اولی_والد, آخرین_والد, فرزند_idx);
      أرجع Err(INVALID_آرگومان);
    }

    DEBUG_پیام (SUBSET_REPACK, اشاره_تهی, "  Duplicating %u, ..., %u => %u", اولی_والد, آخرین_والد, فرزند_idx);

    TRY_تخصیص_بده (بیعلامت clone_idx, duplicate (فرزند_idx));

    لكل (بیعلامت والد_idx : *parents) {
      // duplicate shifts the root node idx, so if parent_idx was root update it.
      إذا (والد_idx == clone_idx) والد_idx++;
      خودکار& والد = vertices_[والد_idx];
      بیعلامت شمار = 0;
      بیعلامت رقم_real = والد.obj().real_links.درازی;
      لكل (خودکار& l : والد.all_links_writer ())
      {
        شمار++;
        إذا (l.objidx != فرزند_idx)
          استمر;

        TRY(reassign_link (l, والد_idx, clone_idx, شمار > رقم_real));
      }
    }

    أرجع Ok(clone_idx);
  }


  /*
   * Adds a new node to the graph, not connected to anything.
   */
  گراف_نتیجه_t<بیعلامت> new_گره (حرف* سر_2, حرف* دم)
  {
    إذا (unlikely (vertices_.درازی >= HB_REPACKER_MAX_VERTICES))
    {
      DEBUG_پیام (SUBSET_REPACK, اشاره_تهی,
                 "creating new node: num of vertices %u exceeds HB_REPACKER_MAX_VERTICES.",
                 vertices_.درازی);
      أرجع Err(حد_EXCEEDED);
    }

    positions_invalid = درست;
    فاصله_invalid = درست;

    خودکار* clone = vertices_.push ();
    TRY (گراف_نتیجه_t<فراغ>::from (vertices_, تخصیص_FAILURE));

    بیعلامت clone_idx = vertices_.درازی - 1;
    ordering_.push (clone_idx);
    TRY (گراف_نتیجه_t<فراغ>::from (ordering_, تخصیص_FAILURE));

    clone->مجموعه_میانگیر (سر_2, دم);
    clone->distance = 0;
    clone->space = 0;

    أرجع Ok(clone_idx);
  }

  /*
   * Creates a new child node and remap the old child to it.
   *
   * Returns the index of the newly created child.
   *
   */
  گراف_نتیجه_t<بیعلامت> remap_فرزند (بیعلامت والد_idx, بیعلامت old_فرزند_idx)
  {
    إذا (unlikely (والد_idx >= vertices_.درازی || old_فرزند_idx >= vertices_.درازی))
      أرجع Err(بیرون_از_BOUNDS);

    TRY_تخصیص_بده (بیعلامت new_فرزند_idx, duplicate (old_فرزند_idx));

    خودکار& والد = vertices_[والد_idx];
    لكل (خودکار& l : والد.real_links_writer ())
    {
      إذا (l.objidx != old_فرزند_idx)
        استمر;
      TRY(reassign_link (l, والد_idx, new_فرزند_idx, خطأ));
    }

    لكل (خودکار& l : والد.virtual_links_writer ())
    {
      إذا (l.objidx != old_فرزند_idx)
        استمر;
      TRY(reassign_link (l, والد_idx, new_فرزند_idx, درست));
    }
    أرجع Ok(new_فرزند_idx);
  }

  /*
   * Raises the sorting priority of all children.
   */
  گراف_نتیجه_t<منطقي> raise_childrens_اولویت (بیعلامت والد_idx)
  {
    DEBUG_پیام (SUBSET_REPACK, اشاره_تهی, "  Raising priority of all children of %u",
               والد_idx);
    // This operation doesn't change ordering until a sort is run, so no need
    // to invalidate positions. It does not change graph structure so no need
    // to update distances or edge counts.
    إذا (unlikely (والد_idx >= vertices_.درازی)) أرجع Err(بیرون_از_BOUNDS);
    منطقي made_change = خطأ;
    لكل (خودکار& l : vertices_[والد_idx].all_links_writer ())
      made_change |= vertices_[l.objidx].raise_اولویت ();
    أرجع Ok(made_change);
  }

  گراف_نتیجه_t<فراغ> is_fully_connected ()
  {
    TRY(update_parents());

    إذا (unlikely (ریشه().incoming_یالها_2 ()))
      // Root cannot have parents.
      أرجع Err(ORPHANED_NODES);

    لكل (بیعلامت i = 0; i < ریشه_idx (); i++)
    {
      إذا (unlikely (!vertices_[i].incoming_یالها_2 ()))
        أرجع Err(ORPHANED_NODES);
    }
    أرجع Ok();
  }

#إذا 0
  /*
   * Saves the current graph to a packed binary format which the repacker fuzzer takes
   * as a seed.
   */
  فراغ save_fuzzer_دانه (hb_برچسب_t برچسب_2) ثابت
  {
    FILE* f = fopen ("./repacker_fuzzer_seed", "w");
    fwrite ((فراغ*) &برچسب_2, حجم (برچسب_2), 1, f);

    uint16_t رقم_objects = vertices_.درازی;
    fwrite ((فراغ*) &رقم_objects, حجم (رقم_objects), 1, f);

    لكل (ثابت خودکار& v : vertices_)
    {
      uint16_t blob_اندازه = v.جدول_اندازه ();
      fwrite ((فراغ*) &blob_اندازه, حجم (blob_اندازه), 1, f);
      fwrite ((ثابت فراغ*) v.obj.سر_2, blob_اندازه, 1, f);
    }

    uint16_t link_شمار = 0;
    لكل (ثابت خودکار& v : vertices_)
      link_شمار += v.obj.real_links.درازی;

    fwrite ((فراغ*) &link_شمار, حجم (link_شمار), 1, f);

    تعريفنوع بنية
    {
      uint16_t والد;
      uint16_t فرزند;
      uint16_t position;
      uint8_t عرض;
    } link_t;

    لكل (بیعلامت i = 0; i < vertices_.درازی; i++)
    {
      لكل (ثابت خودکار& l : vertices_[i].obj.real_links)
      {
        link_t link {
          (uint16_t) i, (uint16_t) l.objidx,
          (uint16_t) l.position, (uint8_t) l.عرض
        };
        fwrite ((فراغ*) &link, حجم (link), 1, f);
      }
    }

    fclose (f);
  }
#پایان_اگر

  بیعلامت رقم_roots_for_space_2 (بیعلامت space) ثابت
  {
    أرجع رقم_roots_for_space[space];
  }

  بیعلامت بعدی_space () ثابت
  {
    أرجع رقم_roots_for_space.درازی;
  }

  گراف_نتیجه_t<فراغ> move_مقصد_new_space (ثابت hb_مجموعه_t& indices)
  {
    إذا (unlikely (رقم_roots_for_space.درازی >= HB_REPACKER_MAX_SPACES))
    {
      DEBUG_پیام (SUBSET_REPACK, اشاره_تهی,
                 "move_to_new_space: num of spaces %u exceeds HB_REPACKER_MAX_SPACES.",
                 رقم_roots_for_space.درازی);
      أرجع Err(حد_EXCEEDED);
    }
    رقم_roots_for_space.push (0);
    TRY (گراف_نتیجه_t<فراغ>::from (رقم_roots_for_space, تخصیص_FAILURE));
    بیعلامت new_space = رقم_roots_for_space.درازی - 1;

    لكل (بیعلامت شاخص_2 : indices) {
      إذا (unlikely (شاخص_2 >= vertices_.درازی))
        أرجع Err(بیرون_از_BOUNDS);
      خودکار& node = vertices_[شاخص_2];
      رقم_roots_for_space[node.space] = رقم_roots_for_space[node.space] - 1;
      رقم_roots_for_space[new_space] = رقم_roots_for_space[new_space] + 1;
      node.space = new_space;
      فاصله_invalid = درست;
      positions_invalid = درست;
    }
    أرجع Ok();
  }

  بیعلامت space_for (بیعلامت شاخص_2, بیعلامت* ریشه = اشاره_تهی) ثابت
  {
    طالما (درست)
    {
      أكد (شاخص_2 < vertices_.درازی);
      ثابت خودکار& node = vertices_[شاخص_2];
      إذا (node.space)
      {
        إذا (ریشه != اشاره_تهی)
          *ریشه = شاخص_2;
        أرجع node.space;
      }

      إذا (!node.incoming_یالها_2 ())
      {
        إذا (ریشه)
          *ریشه = شاخص_2;
        أرجع 0;
      }

      شاخص_2 = *node.parents_iter ();
    }
  }

  size_t مجموع_اندازه_درون_بایتها () ثابت {
    size_t مجموع_اندازه = 0;
    بیعلامت شمار = vertices_.درازی;
    لكل (بیعلامت i = 0; i < شمار; i++) {
      size_t اندازه = vertices_.arrayZ[i].جدول_اندازه ();
      مجموع_اندازه += اندازه;
    }
    أرجع مجموع_اندازه;
  }


 خاص:

  فراغ print_orphaned_nodes ()
  {
    إذا (!DEBUG_ENABLED(SUBSET_REPACK)) أرجع;

    DEBUG_پیام (SUBSET_REPACK, اشاره_تهی, "Graph is not fully connected.");

    parents_invalid = درست;
    (فراغ) update_parents();

    إذا (ریشه().incoming_یالها_2 ()) {
      DEBUG_پیام (SUBSET_REPACK, اشاره_تهی, "Root node has incoming edges.");
    }

    لكل (بیعلامت i = 0; i < vertices_.درازی; i++)
    {
      ثابت خودکار& v = vertices_[i];
      إذا (!v.incoming_یالها_2 ())
        DEBUG_پیام (SUBSET_REPACK, اشاره_تهی, "Node %u is orphaned.", i);
    }
  }

  /*
   * Returns the numbers of incoming edges that are 24 or 32 bits wide.
   */
  بیعلامت wide_parents (بیعلامت گره_idx, hb_مجموعه_t& parents) ثابت
  {
    بیعلامت شمار = 0;
    لكل (بیعلامت p : vertices_[گره_idx].parents_iter ())
    {
      // Only real links can be wide
      لكل (ثابت خودکار& l : vertices_[p].obj().real_links)
      {
        إذا (l.objidx == گره_idx
            && (l.عرض == 3 || l.عرض == 4)
            && !l.is_signed)
        {
          شمار++;
          parents.جمع_کن (p);
        }
      }
    }
    أرجع شمار;
  }

 عام:
  /*
   * Creates a map from objid to # of incoming edges.
   */
  گراف_نتیجه_t<فراغ> update_parents ()
  {
    إذا (!parents_invalid) أرجع Ok();

    بیعلامت شمار = vertices_.درازی;

    لكل (بیعلامت i = 0; i < شمار; i++)
      vertices_.arrayZ[i].reset_parents ();

    لكل (بیعلامت p = 0; p < شمار; p++)
    {
      لكل (ثابت خودکار& l : vertices_.arrayZ[p].obj().real_links)
      {
        إذا (unlikely (l.objidx >= شمار)) أرجع Err(INVALID_آرگومان);
        TRY(vertices_[l.objidx].جمع_کن_والد (p, خطأ));
      }

      لكل (ثابت خودکار& l : vertices_.arrayZ[p].obj().virtual_links)
      {
        إذا (unlikely (l.objidx >= شمار)) أرجع Err(INVALID_آرگومان);
        TRY(vertices_[l.objidx].جمع_کن_والد (p, درست));
      }
    }

    parents_invalid = خطأ;
    أرجع Ok();
  }

  /*
   * compute the serialized start and end positions for each vertex.
   */
  فراغ update_positions ()
  {
    إذا (!positions_invalid) أرجع;

    بیعلامت current_pos = 0;
    لكل (بیعلامت i : ordering_)
    {
      خودکار& v = vertices_[i];
      v.شروع = current_pos;
      current_pos += v.obj().دم - v.obj().سر_2;
      v.انتها = current_pos;
    }

    positions_invalid = خطأ;
  }

  /*
   * Finds the distance to each object in the graph
   * from the initial node.
   */
  گراف_نتیجه_t<فراغ> update_distances ()
  {
    إذا (!فاصله_invalid) أرجع Ok();

    // Uses Dijkstra's algorithm to find all of the shortest distances.
    // https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm
    //
    // Implementation Note:
    // Since our priority queue doesn't support fast priority decreases
    // we instead just add new entries into the queue when a priority changes.
    // Redundant ones are filtered out later on by the visited set.
    // According to https://www3.cs.stonybrook.edu/~rezaul/papers/TR-07-54.pdf
    // for practical performance this is faster then using a more advanced queue
    // (such as a fibonacci queue) with a fast decrease priority.
    بیعلامت شمار = vertices_.درازی;
    لكل (بیعلامت i = 0; i < شمار; i++)
      vertices_.arrayZ[i].distance = hb_int_max (int64_t);
    vertices_[ریشه_idx ()].distance = 0;

    hb_اولویت_صف_t<int64_t> صف;
    صف.alloc (شمار);
    صف.insert (0, ریشه_idx ());
    TRY (گراف_نتیجه_t<فراغ>::from (صف, تخصیص_FAILURE));

    hb_وکتور_t<منطقي> دیدهشده;
    دیدهشده.resize (vertices_.درازی);
    TRY (گراف_نتیجه_t<فراغ>::from (دیدهشده, تخصیص_FAILURE));

    طالما (!صف.درون_error () && !صف.is_empty ())
    {
      بیعلامت بعدی_idx = صف.برداشت_minimum ().second;
      إذا (دیدهشده[بعدی_idx]) استمر;
      ثابت خودکار& next = vertices_[بعدی_idx];
      int64_t بعدی_فاصله = next.distance;
      دیدهشده[بعدی_idx] = درست;

      لكل (ثابت خودکار& link : next.obj().all_links ())
      {
        إذا (unlikely (link.objidx >= vertices_.درازی))
          أرجع Err(بیرون_از_BOUNDS);

        إذا (دیدهشده[link.objidx]) استمر;

        خودکار& فرزند_v = vertices_.arrayZ[link.objidx];
        ثابت خودکار& فرزند = فرزند_v.obj ();
        بیعلامت link_عرض = link.عرض ? link.عرض : 4; // treat virtual offsets as 32 bits wide
        int64_t فرزند_وزن = (فرزند.دم - فرزند.سر_2) +
                               ((int64_t) 1 << (link_عرض * 8)) * (فرزند_v.space + 1);
        int64_t فرزند_فاصله = بعدی_فاصله + فرزند_وزن;

        إذا (فرزند_فاصله < فرزند_v.distance)
        {
          فرزند_v.distance = فرزند_فاصله;
          صف.insert (فرزند_فاصله, link.objidx);
        }
      }
    }

    TRY (گراف_نتیجه_t<فراغ>::from (صف, تخصیص_FAILURE));
    إذا (unlikely (!صف.is_empty ()))
    {
      print_orphaned_nodes ();
      أرجع Err(ORPHANED_NODES);
    }

    فاصله_invalid = خطأ;
    أرجع Ok();
  }

 خاص:
  /*
   * Updates a link in the graph to point to a different object. Corrects the
   * parents vector on the previous and new child nodes.
   */
  گراف_نتیجه_t<فراغ> reassign_link (hb_serialize_context_t::object_t::link_t& link,
                      بیعلامت والد_idx,
                      بیعلامت new_idx,
                      منطقي is_virtual)
  {
    بیعلامت old_idx = link.objidx;
    link.objidx = new_idx;
    vertices_[old_idx].remove_والد (والد_idx);
    أرجع vertices_[new_idx].جمع_کن_والد (والد_idx, is_virtual);
  }

  /*
   * Updates all objidx's in all links using the provided mapping. Corrects incoming edge counts.
   */
  قالب<نام_نوع Iterator, hb_requires (hb_is_iterator (Iterator))>
  گراف_نتیجه_t<فراغ> remap_obj_indices (ثابت hb_map_t& id_map,
                                          Iterator subgraph,
                                          منطقي only_wide = خطأ)
  {
    إذا (!id_map) أرجع Ok();
    لكل (بیعلامت i : subgraph)
    {
      خودکار& رأس = vertices_[i];
      بیعلامت رقم_real = رأس.obj().real_links.درازی;
      بیعلامت شمار = 0;
      لكل (خودکار& link : رأس.all_links_writer ())
      {
        شمار++;
        ثابت uint32_t *v;
        إذا (!id_map.has (link.objidx, &v)) استمر;
        إذا (only_wide && (link.is_signed || (link.عرض != 4 && link.عرض != 3))) استمر;

        TRY(reassign_link (link, i, *v, شمار > رقم_real));
      }
    }
    أرجع Ok();
  }

  /*
   * Finds all nodes in targets that are reachable from start_idx, nodes in visited will be skipped.
   * For this search the graph is treated as being undirected.
   *
   * Connected targets will be added to connected and removed from targets. All visited nodes
   * will be added to visited.
   */
  گراف_نتیجه_t<فراغ> پیدا_کن_connected_nodes (بیعلامت شروع_idx,
                                             hb_مجموعه_t& targets,
                                             hb_مجموعه_t& دیدهشده,
                                             hb_مجموعه_t& connected)
  {
    TRY (گراف_نتیجه_t<فراغ>::from (دیدهشده, تخصیص_FAILURE));
    إذا (دیدهشده.has (شروع_idx)) أرجع Ok();
    إذا (unlikely (شروع_idx >= vertices_.درازی))
    {
      DEBUG_پیام (SUBSET_REPACK, اشاره_تهی,
                 "find_connected_nodes: unexpected start_idx out of bounds.");
      أرجع Err(بیرون_از_BOUNDS);
    }

    // For performance we want to avoid allocating extra memory. So use the ordering_scratch_
    // buffer to implement a stack for DFS.
    ordering_scratch_.resize (vertices_.درازی);
    TRY (گراف_نتیجه_t<فراغ>::from (ordering_scratch_, تخصیص_FAILURE));

    خودکار& پشته = ordering_scratch_;
    بیعلامت پشته_len = 0;

    گراف_نتیجه_t<فراغ> res = Ok();
    خودکار handle_گره = [&] (بیعلامت گره_idx) {
      دیدهشده.جمع_کن (گره_idx);
      إذا (targets.has (گره_idx)) {
        targets.del (گره_idx);
        connected.جمع_کن (گره_idx);
      }

      إذا (unlikely (پشته_len >= پشته.درازی))
      {
        res = Err(چرخه_DETECTED);
        أرجع خطأ;
      }
      پشته[پشته_len++] = گره_idx;
      أرجع درست;
    };

    إذا (!handle_گره (شروع_idx)) أرجع res;

    طالما (پشته_len > 0)
    {
      بیعلامت گره_idx = پشته[--پشته_len];
      ثابت خودکار& v = vertices_[گره_idx];

      // Graph is treated as undirected so search children and parents of node_idx
      لكل (ثابت خودکار& l : v.obj().all_links ())
      {
        بیعلامت فرزند_idx = l.objidx;
        إذا (دیدهشده.has (فرزند_idx)) استمر;
        إذا (!handle_گره (فرزند_idx)) أرجع res;
      }

      لكل (بیعلامت والد_idx : v.parents_iter ())
      {
        إذا (دیدهشده.has (والد_idx)) استمر;
        إذا (!handle_گره (والد_idx)) أرجع res;
      }
    }

    TRY (گراف_نتیجه_t<فراغ>::from (دیدهشده, تخصیص_FAILURE));
    TRY (گراف_نتیجه_t<فراغ>::from (connected, تخصیص_FAILURE));

    أرجع Ok();
  }

 عام:
  // TODO(garretrieger): make private, will need to move most of offset overflow code into graph.
  hb_وکتور_t<رأس_t> vertices_;

  // Specifies the current topological ordering of this graph
  //
  // ordering_[pos] = obj index
  //
  // specifies that the 'pos'th spot is filled by the object
  // given by obj index.
  hb_وکتور_t<بیعلامت> ordering_;
  hb_وکتور_t<بیعلامت> ordering_scratch_;

 خاص:
  منطقي parents_invalid = درست;
  منطقي فاصله_invalid = درست;
  منطقي positions_invalid = درست;
  hb_وکتور_t<بیعلامت> رقم_roots_for_space;
  hb_وکتور_t<حرف*> buffers;
};

}

#پایان_اگر  // GRAPH_GRAPH_HH
